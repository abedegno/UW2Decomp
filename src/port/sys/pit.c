/* pit.c: replaces the PC's programmable interval timer as UW2 used it: AIL.ASM reprogrammed
   channel 0 and ran the timers registered with AIL_register_timer at their own rates (the game
   clock at 256 Hz, SOUND.C's cllbck_tst), and the BIOS kept its 18.2 Hz tick. Here a thread of
   the port's own runs them from the platform's high-resolution counter: each timer is due
   whenever its rate's period has passed, so a 256 Hz timer runs 256 times a second on average
   however the host schedules the thread, and a late wake-up runs the missed calls at once, as
   queued interrupts did. The callbacks run on this thread, as interrupt handlers ran between
   the game's instructions. The keyboard's typematic repeat is driven from here too. */
#include <stdatomic.h>
#include "port.h"
#include "plat.h"

#define NTIMER 16
static struct {
    pit_fn fn;
    uint32_t hz;
    int on;
    uint64_t next;              /* counter value when it is next due */
} timers[NTIMER];

static _Atomic uint32_t bios_ticks;
static uint64_t hz, start;

int pit_timer_register(pit_fn fn)
{
    int i;
    for (i = 0; i < NTIMER; i++)
        if (!timers[i].fn) {
            timers[i].hz = 0;
            timers[i].on = 0;
            timers[i].fn = fn;
            return i;
        }
    return -1;
}

void pit_timer_rate(int h, uint32_t rate)
{
    if (h < 0 || h >= NTIMER) return;
    timers[h].hz = rate;
    timers[h].next = plat_counter() + (rate ? hz / rate : 0);
}

void pit_timer_run(int h, int on)
{
    if (h < 0 || h >= NTIMER) return;
    if (on && !timers[h].on && timers[h].hz) timers[h].next = plat_counter() + hz / timers[h].hz;
    timers[h].on = on;
}

void pit_timer_release(int h)
{
    if (h < 0 || h >= NTIMER) return;
    timers[h].on = 0;
    timers[h].fn = 0;
}

uint32_t pit_bios_ticks(void)
{
    return atomic_load(&bios_ticks);
}

static int pit_thread(void *arg)
{
    uint64_t now, last = plat_counter(), bios_next, period;
    int i, guard;
    (void)arg;
    /* 18.2065 Hz: 1193182 / 65536 */
    bios_next = start + hz * 65536 / 1193182;
    for (;;) {
        plat_sleep_ns(500000);
        now = plat_counter();
        for (i = 0; i < NTIMER; i++) {
            if (!timers[i].fn || !timers[i].on || !timers[i].hz) continue;
            period = hz / timers[i].hz;
            for (guard = 0; now >= timers[i].next && guard < 64; guard++) {
                timers[i].fn();
                timers[i].next += period;
            }
            if (now >= timers[i].next) timers[i].next = now + period;
        }
        while (now >= bios_next) {
            atomic_fetch_add(&bios_ticks, 1);
            bios_next += hz * 65536 / 1193182;
        }
        kbd_tick_ms((uint32_t)((now - last) * 1000 / hz));
        last = now - (now - last) % (hz / 1000 ? hz / 1000 : 1);
    }
    return 0;
}

void pit_start(void)
{
    hz = plat_counter_hz();
    start = plat_counter();
    if (plat_thread_start("pit", pit_thread, 0)) port_fatal("cannot start the timer thread");
}
