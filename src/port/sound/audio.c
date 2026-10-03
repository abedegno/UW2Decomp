/* audio.c: replaces the PC's sound hardware for the port (docs/PORT.md, "Sound"): the FM
   chips the music drivers program (one YM3812, two YM3812s for the Sound Blaster Pro 1 and the
   Pro Audio Spectrum, or a YMF262), emulated by Nuked OPL3 (LGPL-2.1, fetched into
   tools/nuked-opl3 by tools/setup-sound.sh, never committed); the MT-32 or CM-32L behind an
   MPU-401, emulated by libmt32emu (LGPL-2.1-or-later, Homebrew's mt32emu) with the user's own
   ROMs; and the Sound Blaster's 8-bit DAC. Without Nuked OPL3 or libmt32emu at build time the
   chips are silent and the drivers still run.

   ail.c calls audio_render_to at every tick of the AIL clock, before the tick's timers run:
   everything up to that time is rendered with the registers as they were, so a register
   written during a tick sounds from that tick on, as on the card. The DAC plays each buffer
   from the microsecond the driver started it, resampled from its own rate. The frames go to a
   ring the platform's audio stream drains (plat_audio_open), and to --audio-wav's file, which
   gets every frame of the session in the AIL clock's time, however fast the port ran. */
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "audio.h"
#include "aildrv.h"
#include "plat.h"
#ifdef UW2_HAVE_OPL
#include "opl3.h"
#endif
#ifdef UW2_HAVE_MT32EMU
#include <mt32emu/c_interface/c_interface.h>
#endif

void port_log(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

static const char *wav_path, *rom_dir;
static int use_device = 1;
static FILE *wav;
static uint64_t wav_frames;
static int music_kind;

#ifdef UW2_HAVE_OPL
static opl3_chip chip[2];
#endif
static int nchips;

#ifdef UW2_HAVE_MT32EMU
static mt32emu_context mt;
static int mt_ok;
#endif

/* ---- the ring the audio stream drains ------------------------------------------------ */

#define RING 32768                      /* frames, about 0.74 s */
#define PREBUFFER 2048
static int16_t ring[RING * 2];
static _Atomic uint32_t ring_head, ring_tail;   /* written by the renderer, read by the stream */
static int priming = 1;

static void ring_put(const int16_t *f, int n)
{
    uint32_t h = atomic_load(&ring_head), t = atomic_load(&ring_tail);
    int i;
    for (i = 0; i < n; i++) {
        if (h - t >= RING) break;       /* full: the port runs ahead of the device */
        ring[(h % RING) * 2] = f[i * 2];
        ring[(h % RING) * 2 + 1] = f[i * 2 + 1];
        h++;
    }
    atomic_store(&ring_head, h);
}

static void fill(int16_t *out, int frames)
{
    uint32_t h = atomic_load(&ring_head), t = atomic_load(&ring_tail);
    int i;
    if (priming && h - t < PREBUFFER) {
        memset(out, 0, (size_t)frames * 4);
        return;
    }
    priming = 0;
    for (i = 0; i < frames; i++) {
        if (t == h) {                   /* underflow: silence until the ring fills again */
            memset(out + i * 2, 0, (size_t)(frames - i) * 4);
            priming = 1;
            break;
        }
        out[i * 2] = ring[(t % RING) * 2];
        out[i * 2 + 1] = ring[(t % RING) * 2 + 1];
        t++;
    }
    atomic_store(&ring_tail, t);
}

/* ---- the WAV file --------------------------------------------------------------------- */

static void le32(uint8_t *p, uint32_t v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); p[2] = (uint8_t)(v >> 16); p[3] = (uint8_t)(v >> 24); }
static void le16(uint8_t *p, unsigned v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); }

static void wav_header(void)
{
    uint8_t h[44];
    uint32_t data = (uint32_t)(wav_frames * 4);
    memcpy(h, "RIFF", 4);
    le32(h + 4, 36 + data);
    memcpy(h + 8, "WAVEfmt ", 8);
    le32(h + 16, 16);
    le16(h + 20, 1);
    le16(h + 22, 2);
    le32(h + 24, AUDIO_RATE);
    le32(h + 28, AUDIO_RATE * 4);
    le16(h + 32, 4);
    le16(h + 34, 16);
    memcpy(h + 36, "data", 4);
    le32(h + 40, data);
    fseek(wav, 0, SEEK_SET);
    fwrite(h, 1, 44, wav);
    fseek(wav, 0, SEEK_END);
}

void audio_flush(void)
{
    if (!wav) return;
    wav_header();
    fflush(wav);
}

#define NLOGS 4
static FILE *logs[NLOGS];

void *audio_host_fopen_v(const char *path)
{
    FILE *f = fopen(path, "w");
    int i;
    for (i = 0; f && i < NLOGS; i++)
        if (!logs[i]) { logs[i] = f; break; }
    return f;
}

static void at_exit(void)
{
    int i;
    for (i = 0; i < NLOGS; i++)
        if (logs[i]) fflush(logs[i]);
    if (wav) {
        audio_flush();
        fclose(wav);
        wav = NULL;
    }
}

/* ---- set-up ----------------------------------------------------------------------------- */

void audio_config(const char *wav_file, const char *mt32_roms, int device)
{
    wav_path = wav_file;
    rom_dir = mt32_roms;
    use_device = device;
}

void audio_start(void)
{
    atexit(at_exit);
    if (wav_path) {
        wav = fopen(wav_path, "wb");
        if (!wav) fprintf(stderr, "uw2port: cannot write %s\n", wav_path);
        else {
            wav_header();
        }
    }
    if (use_device && plat_audio_open(AUDIO_RATE, fill)) port_log("audio: no audio device; running silent\n");
}

#ifdef UW2_HAVE_MT32EMU
static void mt_open(void)
{
    static const char *names[] = { "CM32L_CONTROL.ROM", "CM32L_PCM.ROM", "MT32_CONTROL.ROM", "MT32_PCM.ROM" };
    mt32emu_report_handler_i rh;
    char path[1200];
    int i, roms = 0;
    if (mt) return;
    rh.v0 = NULL;
    mt = mt32emu_create_context(rh, NULL);
    if (!rom_dir) rom_dir = getenv("UW2PORT_MT32_ROMS");
    for (i = 0; rom_dir && i < 4; i++) {
        snprintf(path, sizeof path, "%s/%s", rom_dir, names[i]);
        /* a CM-32L pair first; MT-32 files only fill what is missing */
        if (mt32emu_add_rom_file(mt, path) > 0) roms++;
        if (i == 1 && roms == 2) break;
    }
    mt32emu_set_stereo_output_samplerate(mt, AUDIO_RATE);
    mt_ok = roms >= 2 && mt32emu_open_synth(mt) == MT32EMU_RC_OK;
    if (!mt_ok)
        fprintf(stderr, "uw2port: no MT-32 or CM-32L ROMs%s%s (--mt32-roms DIR); the MT-32 is silent\n",
                rom_dir ? " in " : "", rom_dir ? rom_dir : "");
}
#endif

void audio_music_device(int kind)
{
    int i;
    music_kind = kind;
    nchips = kind == DRV_SBPRO1 || kind == DRV_PASFM ? 2 : kind == DRV_MT32 || kind == DRV_SPKR ? 0 : 1;
#ifdef UW2_HAVE_OPL
    for (i = 0; i < nchips; i++) OPL3_Reset(&chip[i], AUDIO_RATE);
#else
    (void)i;
    if (nchips) fprintf(stderr, "uw2port: built without Nuked OPL3 (tools/setup-sound.sh); FM music is silent\n");
#endif
#ifdef UW2_HAVE_MT32EMU
    if (kind == DRV_MT32) mt_open();
#else
    if (kind == DRV_MT32) fprintf(stderr, "uw2port: built without libmt32emu (brew install mt32emu); the MT-32 is silent\n");
#endif
}

void audio_opl_write(int c, unsigned reg, unsigned val)
{
#ifdef UW2_HAVE_OPL
    if (c < nchips) OPL3_WriteReg(&chip[c], (uint16_t)reg, (uint8_t)val);
#else
    (void)c; (void)reg; (void)val;
#endif
}

/* the MPU-401 in UART mode: bytes into messages for the MT-32 */
void audio_mpu_byte(unsigned b)
{
#ifdef UW2_HAVE_MT32EMU
    uint8_t c = (uint8_t)b;
    if (mt_ok) mt32emu_parse_stream(mt, &c, 1);
#else
    (void)b;
#endif
}

/* ---- the DAC -------------------------------------------------------------------------- */

#define NSEG 8
static struct Seg {
    uint8_t *data;
    uint32_t n, rate;
    uint64_t start, stop;               /* microseconds of the AIL clock */
} seg[NSEG];
static unsigned dac_l = 127, dac_r = 127;

void audio_dac_play(const uint8_t *data, uint32_t n, uint32_t rate, uint64_t t)
{
    int i, free_i = -1;
    for (i = 0; i < NSEG; i++)
        if (!seg[i].data) { free_i = i; break; }
    if (free_i < 0 || !n || !rate) return;
    seg[free_i].data = malloc(n);
    memcpy(seg[free_i].data, data, n);
    seg[free_i].n = n;
    seg[free_i].rate = rate;
    seg[free_i].start = t;
    seg[free_i].stop = t + (uint64_t)n * 1000000u / rate;
}

void audio_dac_stop(uint64_t t)
{
    int i;
    for (i = 0; i < NSEG; i++)
        if (seg[i].data && seg[i].stop > t) seg[i].stop = t > seg[i].start ? t : seg[i].start;
}

void audio_dac_level(unsigned l, unsigned r, uint64_t t)
{
    (void)t;
    dac_l = l;
    dac_r = r;
}

/* ---- rendering -------------------------------------------------------------------------- */

static uint64_t rendered;               /* output frames rendered so far */

static int16_t clamp16(int32_t v)
{
    return (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
}

void audio_render_to(uint64_t t_us)
{
    uint64_t target = t_us * AUDIO_RATE / 1000000u;
    int16_t out[2 * 512];
#ifdef UW2_HAVE_OPL
    int16_t fm[2][2 * 512];
#endif
#ifdef UW2_HAVE_MT32EMU
    int16_t mtb[2 * 512];
#endif
    while (rendered < target) {
        int n = (int)(target - rendered > 512 ? 512 : target - rendered), i, k;
        memset(out, 0, sizeof out);
#ifdef UW2_HAVE_OPL
        for (k = 0; k < nchips; k++) OPL3_GenerateStream(&chip[k], fm[k], (uint32_t)n);
        for (i = 0; i < n && nchips; i++) {
            if (nchips == 2) {          /* left chip, right chip */
                out[i * 2] = fm[0][i * 2];
                out[i * 2 + 1] = fm[1][i * 2 + 1];
            } else {
                out[i * 2] = fm[0][i * 2];
                out[i * 2 + 1] = fm[0][i * 2 + 1];
            }
        }
#endif
#ifdef UW2_HAVE_MT32EMU
        if (music_kind == DRV_MT32 && mt_ok) {
            mt32emu_render_bit16s(mt, mtb, (uint32_t)n);
            for (i = 0; i < 2 * n; i++) out[i] = clamp16(out[i] + mtb[i]);
        }
#endif
        for (k = 0; k < NSEG; k++) {
            struct Seg *s = &seg[k];
            if (!s->data) continue;
            for (i = 0; i < n; i++) {
                uint64_t tf = (rendered + (uint64_t)i) * 1000000u / AUDIO_RATE;
                uint64_t pos;
                int32_t v;
                if (tf < s->start || tf >= s->stop) continue;
                pos = (tf - s->start) * s->rate / 1000000u;
                if (pos >= s->n) continue;
                v = ((int32_t)s->data[pos] - 128) << 8;
                out[i * 2] = clamp16(out[i * 2] + v * (int32_t)dac_l / 127);
                out[i * 2 + 1] = clamp16(out[i * 2 + 1] + v * (int32_t)dac_r / 127);
            }
            if ((rendered + (uint64_t)n) * 1000000u / AUDIO_RATE >= s->stop) {
                free(s->data);
                s->data = NULL;
            }
        }
        if (use_device) ring_put(out, n);
        if (wav) {
            fwrite(out, 4, (size_t)n, wav);
            wav_frames += (uint64_t)n;
        }
        rendered += (uint64_t)n;
    }
}
