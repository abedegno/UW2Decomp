/* ail.c: replaces part of src/sound/AIL.ASM, the Miles Audio Interface Library 2.0 API
   (docs/PORT.md, "Sound"). Milestone 3 gives the game the AIL of a DOS machine with no sound
   card: the timer services work (AIL owns the PIT, and the game clock is an AIL timer), and no
   driver registers, which is the path UW.CFG's "0 -1 -1 -1 sound" takes anyway. The audio
   stream is opened and fed silence, so the platform's audio path is exercised from the start.
   The driver calls return 0 as AIL does for a handle with no driver. */
#include "compat.h"
#include "sound.h"
#include "port.h"
#include "plat.h"

static void silence(int16_t *s, int frames)
{
    memset(s, 0, (size_t)frames * 4);
}

void AIL_startup(void)
{
    static int open;
    if (!open) {
        open = 1;
        if (plat_audio_open(44100, silence)) port_log("ail: no audio device; running silent\n");
    }
}

void AIL_shutdown(char *msg)
{
    (void)msg;
}

int AIL_register_timer(void (*fn)(void))
{
    return pit_timer_register(fn);
}

void AIL_set_timer_frequency(int timer, uint32 hertz)
{
    pit_timer_rate(timer, hertz);
}

void AIL_start_timer(int timer)
{
    pit_timer_run(timer, 1);
}

void AIL_stop_timer(int timer)
{
    pit_timer_run(timer, 0);
}

void AIL_release_timer_handle(int timer)
{
    pit_timer_release(timer);
}

/* No driver: AIL_register_driver fails, as for a driver file that is not a valid AIL driver. */
int AIL_register_driver(void *driver)
{
    (void)driver;
    return -1;
}

/* The driver calls. AIL.ASM passes each to the registered driver through call_driver and
   find_proc, which return 0 in DX:AX without calling anything when the handle has no driver
   (a handle of 10h or more, or an empty slot). No driver is ever registered here, so each
   call does nothing and returns 0, as in DOS without a sound card. */
unsigned AIL_default_timbre_cache_size(int drv) { (void)drv; return 0; }
void AIL_define_timbre_cache(int drv, void *cache, unsigned size) { (void)drv; (void)cache; (void)size; }
struct DrvrDesc *AIL_describe_driver(int drv) { (void)drv; return 0; }
int AIL_detect_device(int drv, int io, int irq, int dma, int drq) { (void)drv; (void)io; (void)irq; (void)dma; (void)drq; return 0; }
int AIL_index_VOC_block(int drv, void *voc, int block, struct SoundBuff *buf) { (void)drv; (void)voc; (void)block; (void)buf; return 0; }
void AIL_init_driver(int drv, int io, int irq, int dma, int drq) { (void)drv; (void)io; (void)irq; (void)dma; (void)drq; }
void AIL_install_timbre(int drv, int bank, int patch, void *src) { (void)drv; (void)bank; (void)patch; (void)src; }
int AIL_lock_channel(int drv) { (void)drv; return 0; }
void AIL_pause_digital_playback(int drv) { (void)drv; }
int AIL_register_sequence(int drv, void *xmid, int n, void *state, void *ctrl) { (void)drv; (void)xmid; (void)n; (void)state; (void)ctrl; return 0; }
void AIL_register_sound_buffer(int drv, int n, struct SoundBuff *buf) { (void)drv; (void)n; (void)buf; }
void AIL_release_channel(int drv, int ch) { (void)drv; (void)ch; }
void AIL_release_sequence_handle(int drv, int seq) { (void)drv; (void)seq; }
void AIL_resume_digital_playback(int drv) { (void)drv; }
void AIL_send_channel_voice_message(int drv, int status, int d1, int d2) { (void)drv; (void)status; (void)d1; (void)d2; }
unsigned AIL_sequence_status(int drv, int seq) { (void)drv; (void)seq; return 0; }
void AIL_set_digital_playback_panpot(int drv, int p) { (void)drv; (void)p; }
void AIL_set_digital_playback_volume(int drv, int v) { (void)drv; (void)v; }
void AIL_set_relative_tempo(int drv, int seq, int percent, int ms) { (void)drv; (void)seq; (void)percent; (void)ms; }
void AIL_set_relative_volume(int drv, int seq, int percent, int ms) { (void)drv; (void)seq; (void)percent; (void)ms; }
void AIL_shutdown_driver(int drv, char *msg) { (void)drv; (void)msg; }
unsigned AIL_sound_buffer_status(int driver, int buffer) { (void)driver; (void)buffer; return 0; }
void AIL_start_digital_playback(int driver) { (void)driver; }
void AIL_start_sequence(int drv, int seq) { (void)drv; (void)seq; }
unsigned AIL_state_table_size(int drv) { (void)drv; return 0; }
void AIL_stop_digital_playback(int driver) { (void)driver; }
void AIL_stop_sequence(int drv, int seq) { (void)drv; (void)seq; }
unsigned AIL_timbre_request(int drv, int seq) { (void)drv; (void)seq; return 0; }
int AIL_timbre_status(int drv, int bank, int patch) { (void)drv; (void)bank; (void)patch; return 0; }
