/* yamaha.h: YAMAHA.INC's state, for yamaha.c and for a game-specific FM extension.

   John Miles' YAMAHA.INC (AIL 2.14) assembles with OSI_ALE FALSE, which is how it was released,
   or TRUE, which INCLUDEs ALE.INC: Origin's time-variant effects (TVFX), never released, which
   Origin's own drivers were built with. ALE.INC shares YAMAHA.INC's data and calls its
   routines, and YAMAHA.INC calls ALE.INC's at three places (IFDEF TV_phase, IFDEF
   TV_switch_voice, and XMIDI.ASM's IFDEF serve_synth). yamaha.c is YAMAHA.INC; those three
   places are the hooks of struct AilFmExt below, which a project fills from its own
   decompilation of its drivers (UW2: UW2Decomp's src/port/sound/tvfx.c, named by
   examples/uw2/exhume.toml's [sound] extensions). Exhume holds no extension.

   With no extension (the project's ailgame.h defines no AIL_FM_EXT; docs/PORT.md, "Sound"),
   yamaha.c is YAMAHA.INC as released, without OSI_ALE:

   - a timbre that is neither a .BNK one (14 bytes) nor an OPL3 .BNK one (25 bytes, YMF262 only)
     plays nothing: note_on has already marked the free slot KEYON and leaves it so, unvoiced,
     with the slot's other fields as the last note left them (`jmp __exit`). A later note off
     of the same key frees it again, by the path of whatever S_type the slot last had;
   - nothing runs after the XMIDI service (no serve_synth): no time-variant parameter moves,
     and update_priority runs only when assign_voice finds no free voice;
   - a note off that frees a voice gives it to nobody (no TV_switch_voice).

   With one, a time-variant timbre is set up by `phase`, freed voices are offered by
   `voice_freed`, and `serve` runs after every XMIDI service, as ALE.INC's routines ran. The
   extension reads and writes the Yam fields and calls yam_update_voice, yam_update_priority
   and yam_release_voice, as ALE.INC used YAMAHA.INC's data and procedures. */
#ifndef EXHUME_YAMAHA_H
#define EXHUME_YAMAHA_H

#include "aildrv.h"

#define YAM_MAX_TIMBS   192
#define YAM_MAXSLOTS    20              /* NUM_SLOTS: 16 for the YM3812, 20 for the YMF262 */
#define YAM_MAXVOICES   18

/* S_status */
#define YAM_FREE        0
#define YAM_KEYON       1
#define YAM_KEYOFF      2
/* S_type */
#define YAM_BNK_INST    0
#define YAM_TV_INST     1
#define YAM_TV_EFFECT   2
#define YAM_OPL3_INST   3
/* S_update */
#define YAM_U_ALL_REGS  0xF9
#define YAM_U_AVEKM     0x80
#define YAM_U_KSLTL     0x40
#define YAM_U_ADSR      0x20
#define YAM_U_WS        0x10
#define YAM_U_FBC       0x08
#define YAM_U_FREQ      0x01

struct AilFmExt;

/* YAMAHA.INC's data, by its names (the slot arrays NUM_SLOTS long, the voice ones NUM_VOICES) */
typedef struct Yam {
    Synth s;
    int kind, ymf262, stereo, nvoices, nslots;
    unsigned note_event;
    uint32_t timb_hist[YAM_MAX_TIMBS];
    uint16_t timb_offsets[YAM_MAX_TIMBS];
    uint8_t timb_bank[YAM_MAX_TIMBS], timb_num[YAM_MAX_TIMBS], timb_attribs[YAM_MAX_TIMBS];
    uint8_t *cache_base;
    unsigned cache_size, cache_end;
    unsigned TV_accum, pri_accum;       /* the DDA accumulators an extension's serve keeps */
    uint8_t vol_update;                 /* 0 | U_KSLTL */
    int rover_2op, rover_4op;
    uint8_t conn_shadow;

    uint8_t *S_timbre[YAM_MAXSLOTS];
    uint16_t S_duration[YAM_MAXSLOTS];  /* # of TV intervals left in keyon */
    uint8_t S_status[YAM_MAXSLOTS], S_type[YAM_MAXSLOTS], S_voice[YAM_MAXSLOTS],
            S_channel[YAM_MAXSLOTS], S_note[YAM_MAXSLOTS], S_keynum[YAM_MAXSLOTS],
            S_transpose[YAM_MAXSLOTS], S_velocity[YAM_MAXSLOTS], S_sustain[YAM_MAXSLOTS],
            S_update[YAM_MAXSLOTS];
    uint8_t S_KBF_shadow[YAM_MAXSLOTS], S_BLOCK[YAM_MAXSLOTS], S_FBC[YAM_MAXSLOTS],
            S_KSLTL_0[YAM_MAXSLOTS], S_KSLTL_1[YAM_MAXSLOTS], S_AVEKM_0[YAM_MAXSLOTS],
            S_AVEKM_1[YAM_MAXSLOTS], S_AD_0[YAM_MAXSLOTS], S_AD_1[YAM_MAXSLOTS],
            S_SR_0[YAM_MAXSLOTS], S_SR_1[YAM_MAXSLOTS], S_scale_01[YAM_MAXSLOTS];
    /* the YM3812 register values (IF NOT OSI_ALE in YAMAHA.INC, which leaves them to ALE.INC
       otherwise; here always), a level or frequency in the top bits of each word */
    uint16_t S_ws_val[YAM_MAXSLOTS], S_m1_val[YAM_MAXSLOTS], S_m0_val[YAM_MAXSLOTS],
             S_fb_val[YAM_MAXSLOTS], S_p_val[YAM_MAXSLOTS], S_v1_val[YAM_MAXSLOTS],
             S_v0_val[YAM_MAXSLOTS], S_f_val[YAM_MAXSLOTS];
    /* the OPL3 second operator pair */
    uint8_t S_KSLTL_2[YAM_MAXSLOTS], S_KSLTL_3[YAM_MAXSLOTS], S_AVEKM_2[YAM_MAXSLOTS],
            S_AVEKM_3[YAM_MAXSLOTS], S_AD_2[YAM_MAXSLOTS], S_AD_3[YAM_MAXSLOTS],
            S_SR_2[YAM_MAXSLOTS], S_SR_3[YAM_MAXSLOTS], S_scale_23[YAM_MAXSLOTS];
    uint16_t S_ws_val_2[YAM_MAXSLOTS], S_m3_val[YAM_MAXSLOTS], S_m2_val[YAM_MAXSLOTS],
             S_v3_val[YAM_MAXSLOTS], S_v2_val[YAM_MAXSLOTS];
    uint16_t S_V_priority[YAM_MAXSLOTS];

    uint8_t MIDI_vol[NUM_CHANS], MIDI_pan[NUM_CHANS], MIDI_pitch_l[NUM_CHANS],
            MIDI_pitch_h[NUM_CHANS], MIDI_express[NUM_CHANS], MIDI_mod[NUM_CHANS],
            MIDI_sus[NUM_CHANS], MIDI_vprot[NUM_CHANS], MIDI_timbre[NUM_CHANS],
            MIDI_bank[NUM_CHANS], MIDI_program[NUM_CHANS];
    uint8_t RBS_timbres[128];
    uint8_t MIDI_voices[NUM_CHANS];
    uint8_t V_channel[YAM_MAXVOICES];

    const struct AilFmExt *ext;         /* the extension, or 0 */
    void *ext_state;                    /* its own state: ext->state_size bytes, zeroed */
} Yam;

/* A game-specific FM extension: what ALE.INC adds to YAMAHA.INC. */
typedef struct AilFmExt {
    const char *name;                   /* for messages */
    size_t state_size;                  /* bytes of state of its own (y->ext_state), 0 for none;
                                           allocated zeroed with the synthesiser */
    /* note_on, a timbre whose length word is neither 14 nor 25 (YAMAHA.INC's __TVFX_timbre):
       set slot si up from y->S_timbre[si] (TV_phase). The slot is KEYON, its channel, key,
       note, transposition, velocity and timbre set; yamaha.c then assigns it a voice.
       Required. */
    void (*phase)(Yam *y, int si);
    /* note_off has freed a .BNK or OPL3 slot and its voice (TV_switch_voice), or 0 */
    void (*voice_freed)(Yam *y);
    /* after every XMIDI service, at the service rate (serve_synth), or 0 */
    void (*serve)(Yam *y);
} AilFmExt;

/* YAMAHA.INC's procedures an extension calls */
void yam_update_voice(Yam *y, int si);
void yam_update_priority(Yam *y);
void yam_release_voice(Yam *y, int si);

#endif
