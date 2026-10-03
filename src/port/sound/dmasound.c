/* dmasound.c: replaces the digital sound drivers DD01, DD02 and DD03.ADV (docs/PORT.md,
   "Sound"), which are AIL 2.14's SBDIG, SBPDIG and PASDIG.ADV byte for byte: the
   double-buffered playback of John Miles' public-domain DMASOUND.ASM, translated for what UW2
   uses (index_VOC_blk, register_sb, get_sb_status, start, stop, pause and resume, volume and
   panpot). The hardware, a DMA transfer that ends in an interrupt, becomes the mixer's DAC
   channel (ail.c): a buffer starts at a virtual time, the AIL timer clock, and is done when
   its samples have played at the rate the .VOC's time constant gives, 1000000 / (256 - tc).
   What the interrupt handler did at that moment (the buffer DONE, the next registered one
   started) is done when the driver is next entered, for the time it happened, so the state
   the game reads depends only on the clock. */
#include <stdlib.h>
#include <string.h>
#include "aildrv.h"

#define VOC_MODE        0
#define BUF_MODE        1

typedef struct Digi {
    int kind;
    unsigned DAC_status, buffer_mode, main_volume, panpot;
    unsigned buff_status[2], buff_pack[2], buff_sample[2];
    const uint8_t *buff_data[2];
    uint32_t buff_len[2];
    int current;                        /* current_buffer, the one playing */
    int playing;                        /* a buffer's transfer is under way (the DMA) */
    uint64_t t_start, t_end;            /* the playing buffer's start and end */
    uint64_t paused_left;               /* while paused: microseconds left of it */
    uint32_t paused_done;               /* and the samples already played */
} Digi;

static uint8_t pan_graph_tab(int i) { return (uint8_t)(i < 63 ? i * 2 : 127); }

Digi *digi_new(int kind)
{
    Digi *d = calloc(1, sizeof *d);
    d->kind = kind;
    return d;
}

static uint32_t rate_of(unsigned tc)
{
    return 1000000u / (256u - (tc & 0xFF));
}

/* set_volume: the levels the card is given, as left and right 0..127 for the mixer. The
   Sound Blaster's DSP has none: SBDIG only turns the speaker on, whatever the volume and pan. */
static void set_volume(Digi *d)
{
    unsigned l, r;
    if (d->kind == DRV_DIG_SB) {
        hw_dac_level(127, 127, ail_now_us());
        return;
    }
    r = pan_graph_tab(127 - (int)(d->panpot & 0x7F)) * (d->main_volume & 0xFF);
    l = pan_graph_tab((int)(d->panpot & 0x7F)) * (d->main_volume & 0xFF);
    if (d->kind == DRV_DIG_SBPRO) {     /* the CT1345 mixer: four bits a side */
        r = (r >> 10) & 15;
        l = (l >> 10) & 15;
        hw_dac_level(l * 127 / 15, r * 127 / 15, ail_now_us());
    } else {                            /* Pro Audio Spectrum: 0..100 a side */
        r /= 161;
        l /= 161;
        hw_dac_level(l * 127 / 100, r * 127 / 100, ail_now_us());
    }
}

static int lenient;                     /* under replay: see digi_register_sb */

void digi_set_lenient(int on)
{
    lenient = on;
}

static void process_buffer(Digi *d, int n, uint64_t t)
{
    d->buff_status[n] = DAC_PLAYING;
    d->current = n;
    d->playing = 1;
    d->t_start = t;
    d->t_end = hw_dac_play(d->buff_data[n], d->buff_len[n], d->buff_sample[n], t);
}

/* next_buffer: a registered buffer that has not played, 0 first; none ends playback */
static int next_buffer(Digi *d)
{
    if (d->buff_status[0] == DAC_STOPPED) return 0;
    if (d->buff_status[1] == DAC_STOPPED) return 1;
    d->DAC_status = DAC_DONE;
    return -1;
}

/* IRQ_play_buffer at time t: the transfer has ended, the buffer that was playing is DONE
   (whatever the game did to its status meanwhile) and the next registered one starts */
static void buffer_ends(Digi *d, uint64_t t)
{
    int n;
    d->playing = 0;
    d->buff_status[d->current] = DAC_DONE;
    n = next_buffer(d);
    if (n >= 0) process_buffer(d, n, t);
}

/* what the interrupt did up to now: each transfer that has ended, while not paused */
static void advance(Digi *d)
{
    uint64_t now = ail_now_us();
    while (d->playing && d->DAC_status != DAC_PAUSED && now >= d->t_end)
        buffer_ends(d, d->t_end);
}

/* the interrupt, at each AIL tick (ail.c), so that the next buffer starts on time even
   while the game is not calling the driver */
void digi_tick(Digi *d)
{
    advance(d);
}

void digi_init(Digi *d)
{
    d->panpot = 64;
    d->main_volume = d->kind == DRV_DIG_SBPRO ? 110 : d->kind == DRV_DIG_PAS ? 80 : 127;
    set_volume(d);
    d->DAC_status = DAC_STOPPED;
    d->buffer_mode = BUF_MODE;
    d->buff_status[0] = d->buff_status[1] = DAC_DONE;
}

void digi_shutdown(Digi *d)
{
    digi_stop(d);
}

/* index_VOC_blk: the voice data block of a .VOC image (the one after marker `block`, or the
   first with -1), with an extended block's rate and packing overriding its own; 0 if none */
int digi_index_voc(Digi *d, const uint8_t *file, int block, struct SoundBuffDesc *out)
{
    const uint8_t *si = file + (file[0x14] | file[0x15] << 8);
    int x_status = 0, bx = block;
    unsigned x_pack = 0, x_tc = 0, type, sr, pk;
    uint32_t len;
    (void)d;
    for (;;) {
        type = si[0];
        if (type == 0) return 0;
        if (type == 8) {                /* extended voice data */
            x_tc = si[5];
            x_pack = si[6] | (si[7] == 1 ? 0x80 : 0);
            x_status = 1;
        } else if (type == 1) {
            if (bx == -1) break;
        } else if (type == 4) {
            if (bx == (int)(si[4] | si[5] << 8)) bx = -1;
        }
        len = (uint32_t)(si[1] | si[2] << 8 | si[3] << 16);
        si += 4 + len;
    }
    sr = si[4];
    pk = si[5];
    if (x_status) {
        pk = x_pack;
        sr = x_tc;
    }
    out->sample_rate = sr;
    out->pack_type = pk;
    out->len = (uint32_t)(si[1] | si[2] << 8 | si[3] << 16) - 2;
    out->data = si + 6;
    return 1;
}

/* Under replay the game sees DOS's buffer statuses, not these: when it registers again the
   buffer this driver is still playing (DOS's transfer ended a little earlier than this
   driver's clock says), the transfer ends now, as it had in DOS, rather than the new data
   being marked DONE at the end, which is what DMASOUND does with a buffer registered while
   it plays. The game's state is the same either way (it reads the recorded statuses); this
   only keeps the port's sound of a replay close to DOS's. */
void digi_register_sb(Digi *d, int n, const struct SoundBuffDesc *b)
{
    advance(d);
    if (lenient && d->playing && d->current == (n & 1) && d->DAC_status == DAC_PLAYING)
        buffer_ends(d, ail_now_us());
    if (d->buffer_mode == VOC_MODE) {
        digi_stop(d);
        d->buffer_mode = BUF_MODE;
    }
    n &= 1;
    d->buff_pack[n] = b->pack_type;
    d->buff_sample[n] = b->sample_rate;
    d->buff_data[n] = b->data;
    d->buff_len[n] = b->len;
    d->buff_status[n] = DAC_STOPPED;
}

unsigned digi_sb_status(Digi *d, int n)
{
    advance(d);
    return d->buff_status[n & 1];
}

void digi_start(Digi *d)
{
    int n;
    advance(d);
    if (d->DAC_status == DAC_PLAYING) return;
    n = next_buffer(d);
    if (n < 0) return;
    d->DAC_status = DAC_PLAYING;
    process_buffer(d, n, ail_now_us());
}

void digi_stop(Digi *d)
{
    advance(d);
    d->DAC_status = DAC_STOPPED;
    d->playing = 0;
    hw_dac_stop(ail_now_us());
    d->buff_status[0] = d->buff_status[1] = DAC_DONE;
}

void digi_pause(Digi *d)
{
    uint64_t now;
    advance(d);
    if (d->DAC_status != DAC_PLAYING) return;
    d->DAC_status = DAC_PAUSED;
    now = ail_now_us();
    d->paused_left = d->t_end > now ? d->t_end - now : 0;
    d->paused_done = (uint32_t)((now - d->t_start) * rate_of(d->buff_sample[d->current]) / 1000000u);
    if (d->paused_done > d->buff_len[d->current]) d->paused_done = d->buff_len[d->current];
    hw_dac_stop(now);
}

void digi_resume(Digi *d)
{
    uint64_t now;
    int n = d->current;
    advance(d);
    if (d->DAC_status != DAC_PAUSED) return;
    d->DAC_status = DAC_PLAYING;
    now = ail_now_us();
    if (!d->playing) return;
    d->t_start = now;
    d->t_end = hw_dac_play(d->buff_data[n] + d->paused_done, d->buff_len[n] - d->paused_done,
                           d->buff_sample[n], now);
}

void digi_set_volume(Digi *d, unsigned v)
{
    advance(d);
    d->main_volume = v;
    set_volume(d);
}

void digi_set_pan(Digi *d, unsigned p)
{
    advance(d);
    d->panpot = p;
    set_volume(d);
}

unsigned digi_volume(Digi *d) { return d->main_volume; }
unsigned digi_pan(Digi *d) { return d->panpot; }
