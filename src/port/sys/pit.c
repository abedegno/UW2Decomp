/* pit.c: replaces the PC's programmable interval timer as UW2 used it. AIL.ASM reprogrammed
   channel 0 and ran the timers registered with AIL_register_timer from it, the game clock at
   256 Hz (SOUND.C's cllbck_tst) among them, and the BIOS kept its 18.2 Hz tick. Here a thread
   of the port's own measures the host's high-resolution counter and gives AIL (src/port/sound/
   ail.c, which keeps API_timer's DDA) the time that has passed, so that AIL runs as many PIT
   ticks as the PIT would have, its timers at their own rates, a late wake-up running the
   missed ticks at once, as queued interrupts did. Under replay AIL takes its ticks from the
   replayed clock instead. The BIOS tick (for clock()) and the keyboard's typematic repeat are
   driven from here too. */
#include <stdatomic.h>
#include "port.h"
#include "plat.h"
#include "sound/audio.h"

static _Atomic uint32_t bios_ticks;
static uint64_t hz, start;

uint32_t pit_bios_ticks(void)
{
    return atomic_load(&bios_ticks);
}

static int pit_thread(void *arg)
{
    uint64_t now, last = plat_counter(), prev = last, bios_next;
    (void)arg;
    /* 18.2065 Hz: 1193182 / 65536 */
    bios_next = start + hz * 65536 / 1193182;
    for (;;) {
        plat_sleep_ns(500000);
        now = plat_counter();
        ail_pit_advance((now - prev) * 1000000000u / hz);
        prev = now;
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
