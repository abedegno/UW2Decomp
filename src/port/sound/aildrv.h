/* aildrv.h: replaces nothing; the interface between the port's AIL (ail.c) and the C drivers
   that replace the .ADV driver files UW2 loads from SOUND\ (docs/PORT.md, "Sound").

   The drivers are C translations of John Miles' public-domain AIL 2.14 driver source
   (XMIDI.ASM, YAMAHA.INC, MT32.INC, MPU401.INC, DMASOUND.ASM), corrected to the builds UW2
   ships, which differ from 2.14 in two places found by assembling 2.14's sources with TASM
   and comparing with the GOG files byte for byte:

   - the XMIDI shell is an earlier revision (before 1.10's "new time signature function"):
     the beat and bar counters are kept as 1.07 to 1.09 kept them;
   - the FM drivers (DM02, DM03, DM04, DM06, DM07) are built with OSI_ALE, Origin's
     time-variant effects (TVFX) for the AdLib family, whose source (ALE.INC) was never
     released; yamaha.c translates it from DM03.ADV's code (0530h..0ADAh).

   The digital drivers (DD01..DD03) are AIL 2.14's SBDIG, SBPDIG and PASDIG byte for byte.
   None of the drivers' code is in the repository: the C is written from the source and the
   disassembly, and the user's own .ADV files are read only for their description tables.

   A driver writes to its hardware through the functions below, which ail.c timestamps with
   the AIL timer clock and hands to the synthesiser backends (OPL, MT-32) and the mixer. */
#ifndef UW2_AILDRV_H
#define UW2_AILDRV_H

#include <stdint.h>
#include <stddef.h>

/* AIL.INC */
#define SEQ_STOPPED     0
#define SEQ_PLAYING     1
#define SEQ_DONE        2

#define DAC_STOPPED     0
#define DAC_PAUSED      1
#define DAC_PLAYING     2
#define DAC_DONE        3

#define CHAN_LOCK       110
#define CHAN_PROTECT    111
#define VOICE_PROTECT   112
#define TIMBRE_PROTECT  113
#define PATCH_BANK_SEL  114
#define INDIRECT_C_PFX  115
#define FOR_LOOP        116
#define NEXT_LOOP       117
#define CLEAR_BEAT_BAR  118
#define CALLBACK_TRIG   119
#define SEQ_INDEX       120

#define MODULATION      1
#define PART_VOLUME     7
#define PANPOT          10
#define EXPRESSION      11
#define SUSTAIN         64
#define RESET_ALL_CTRLS 121
#define ALL_NOTES_OFF   123

#define START_MSB_1     32
#define FINAL_BYTE_1    36
#define RHYTHM_KEY_TIMB 58
#define PATCH_REVERB    59
#define PATCH_BENDER    60
#define REVERB_MODE     61
#define REVERB_TIME     62
#define REVERB_LEVEL    63

#define NUM_CHANS       16

/* The kinds of driver, recognised by the device names in the user's .ADV file. */
enum {
    DRV_NONE = 0,
    DRV_ADLIB,          /* DM02: Ad Lib, YM3812 at 388h */
    DRV_SBFM,           /* DM03: Sound Blaster FM, YM3812 at IO+8 */
    DRV_SBPRO1,         /* DM04: Sound Blaster Pro FM, two YM3812s */
    DRV_PASFM,          /* DM06: Pro Audio Spectrum FM, two YM3812s */
    DRV_SBPRO2,         /* DM07: Sound Blaster Pro FM, YMF262 */
    DRV_MT32,           /* DM05: Roland MT-32 through an MPU-401 */
    DRV_SPKR,           /* DM01: the PC speaker (not emulated: refused) */
    DRV_DIG_SB,         /* DD01: Sound Blaster digital */
    DRV_DIG_SBPRO,      /* DD02: Sound Blaster Pro digital */
    DRV_DIG_PAS         /* DD03: Pro Audio Spectrum digital */
};

/* ---- the synthesiser side of an XMIDI driver (yamaha.c, mt32.c) -------------------- */

struct Xmidi;
typedef struct Synth {
    int min_true_chan, max_true_chan, max_rec_chan;     /* 1-based, as the .INC files say */
    int def_synth_vol;
    void (*reset)(struct Synth *);
    void (*init)(struct Synth *);
    void (*shutdown)(struct Synth *);
    void (*serve)(struct Synth *);                      /* serve_synth, or 0 */
    void (*send)(struct Synth *, unsigned stat, unsigned d1, unsigned d2);
    void (*sysex)(struct Synth *, const uint8_t *data, int type, unsigned len); /* or 0 */
    unsigned (*cache_size)(struct Synth *);
    void (*define_cache)(struct Synth *, uint8_t *addr, unsigned size);
    int (*index_timbre)(struct Synth *, unsigned gnum);
    unsigned (*request_filter)(struct Synth *, unsigned gnum);  /* MT-32: skip banks 0, 127 */
    void (*install)(struct Synth *, unsigned bank, unsigned num, const uint8_t *addr);
    void (*protect)(struct Synth *, unsigned bank, unsigned num, int on);
    unsigned (*timbre_status)(struct Synth *, unsigned bank, unsigned num);
    void *priv;
} Synth;

/* ---- the XMIDI shell (xmidi.c) ------------------------------------------------------ */

Synth *yamaha_new(int kind);
Synth *mt32_new(void);
struct Xmidi *xmidi_new(Synth *s);
void xmidi_init_driver(struct Xmidi *x);
void xmidi_shutdown_driver(struct Xmidi *x);
void xmidi_serve(struct Xmidi *x);
unsigned xmidi_state_size(struct Xmidi *x);
int xmidi_register_seq(struct Xmidi *x, uint8_t *xmid, int num, void *state, uint8_t *ctrl);
void xmidi_release_seq(struct Xmidi *x, int seq);
void xmidi_start_seq(struct Xmidi *x, int seq);
void xmidi_stop_seq(struct Xmidi *x, int seq);
void xmidi_resume_seq(struct Xmidi *x, int seq);
unsigned xmidi_seq_status(struct Xmidi *x, int seq);
unsigned xmidi_rel_volume(struct Xmidi *x, int seq);
unsigned xmidi_rel_tempo(struct Xmidi *x, int seq);
void xmidi_set_rel_volume(struct Xmidi *x, int seq, unsigned vol, unsigned ms);
void xmidi_set_rel_tempo(struct Xmidi *x, int seq, unsigned tempo, unsigned ms);
int xmidi_control_val(struct Xmidi *x, int seq, int chan, int ctrl);
void xmidi_set_control_val(struct Xmidi *x, int seq, int chan, int ctrl, int val);
unsigned xmidi_chan_notes(struct Xmidi *x, int seq, int chan);
void xmidi_map_seq_channel(struct Xmidi *x, int seq, int seqchan, int physchan);
int xmidi_true_seq_channel(struct Xmidi *x, int seq, int seqchan);
int xmidi_beat_count(struct Xmidi *x, int seq);
int xmidi_bar_count(struct Xmidi *x, int seq);
void xmidi_branch_index(struct Xmidi *x, int seq, int marker);
void xmidi_send_cv(struct Xmidi *x, unsigned stat, unsigned d1, unsigned d2);
unsigned xmidi_lock_channel(struct Xmidi *x);
void xmidi_release_channel(struct Xmidi *x, int chan);
unsigned xmidi_timbre_request(struct Xmidi *x, int seq);

/* ---- the digital driver (dmasound.c) ------------------------------------------------ */

struct SoundBuffDesc {              /* AIL's sbuffer, as the game's struct SoundBuff holds it */
    unsigned pack_type, sample_rate;
    const uint8_t *data;
    uint32_t len;
};
struct Digi;
struct Digi *digi_new(int kind);
void digi_init(struct Digi *d);
void digi_tick(struct Digi *d);
void digi_set_lenient(int on);
void digi_shutdown(struct Digi *d);
int digi_index_voc(struct Digi *d, const uint8_t *file, int block, struct SoundBuffDesc *out);
void digi_register_sb(struct Digi *d, int n, const struct SoundBuffDesc *b);
unsigned digi_sb_status(struct Digi *d, int n);
void digi_start(struct Digi *d);
void digi_stop(struct Digi *d);
void digi_pause(struct Digi *d);
void digi_resume(struct Digi *d);
void digi_set_volume(struct Digi *d, unsigned v);
void digi_set_pan(struct Digi *d, unsigned p);
unsigned digi_volume(struct Digi *d);
unsigned digi_pan(struct Digi *d);

/* ---- the hardware, in ail.c / audio.c ----------------------------------------------- */

/* An FM chip register write: chip 0 (left, or the only one) or 1 (the right YM3812 of a
   dual-chip card), reg 0..1FFh (100h up the YMF262's second array). */
void hw_opl_write(int chip, unsigned reg, unsigned val);
void hw_opl_mode(int kind);                 /* which chips the music card has */
/* A byte to the MPU-401's data port (UART mode), for the MT-32. */
void hw_mpu_byte(unsigned b);
/* The digital channel: play n bytes of 8-bit unsigned mono PCM at the SB time constant tc
   (rate 1000000 / (256 - tc)), from virtual time t_us on, at a volume pair (0..127 each).
   Returns the virtual time at which the last sample has been played. */
uint64_t hw_dac_play(const uint8_t *data, uint32_t n, unsigned tc, uint64_t t_us);
void hw_dac_stop(uint64_t t_us);
void hw_dac_level(unsigned left, unsigned right, uint64_t t_us);
/* The AIL timer clock: microseconds of the emulated PIT since AIL_startup. */
uint64_t ail_now_us(void);
void ail_log(const char *fmt, ...);

#endif
