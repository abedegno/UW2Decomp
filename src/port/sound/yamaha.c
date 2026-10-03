/* yamaha.c: replaces the synthesiser half of an AIL 2 game's FM music drivers (UW2: DM02, DM03,
   DM04, DM06 and DM07.ADV; docs/PORT.md, "Sound"): the XMIDI interpreter for the Ad Lib family
   of John Miles' public-domain YAMAHA.INC (AIL 2.14, version 1.02), translated from the source
   routine by routine, with its names.

   YAMAHA.INC's OSI_ALE assembles Origin's time-variant effects (TVFX, ALE.INC, never released)
   into the driver. This file holds none of it: where YAMAHA.INC calls ALE.INC (TV_phase,
   TV_switch_voice, serve_synth) it calls the hooks of a game-specific extension, struct
   AilFmExt in yamaha.h, which the project supplies from its own decompilation and names to
   ail.c as AIL_FM_EXT. Without one this is YAMAHA.INC as released, OSI_ALE FALSE: yamaha.h
   says exactly what that does with a TVFX timbre.

   The variants: YM3812 (Ad Lib and Sound Blaster, 9 voices, 16 virtual slots); two YM3812s
   in stereo (Sound Blaster Pro 1, Pro Audio Spectrum: levels are panned per chip); YMF262
   (Sound Blaster Pro 2, 18 voices or 6 four-operator ones, 20 slots, panning by the output
   bits). Register writes go to hw_opl_write (ail.c). */
#include <stdlib.h>
#include <string.h>
#include "yamaha.h"

#define MAX_TIMBS       YAM_MAX_TIMBS
#define DEF_TC_SIZE     3584
#define DEF_PITCH_RANGE 12
#define DEF_AV_DEPTH    0xC0
#define R_PAN_THRESH    27
#define L_PAN_THRESH    100

#define FREE            YAM_FREE        /* S_status */
#define KEYON           YAM_KEYON
#define KEYOFF          YAM_KEYOFF
#define BNK_INST        YAM_BNK_INST    /* S_type */
#define TV_INST         YAM_TV_INST
#define TV_EFFECT       YAM_TV_EFFECT
#define OPL3_INST       YAM_OPL3_INST

#define U_ALL_REGS      YAM_U_ALL_REGS  /* S_update */
#define U_AVEKM         YAM_U_AVEKM
#define U_KSLTL         YAM_U_KSLTL
#define U_ADSR          YAM_U_ADSR
#define U_WS            YAM_U_WS
#define U_FBC           YAM_U_FBC
#define U_FREQ          YAM_U_FREQ

#define BNK_SIZE        14              /* SIZE BNK, SIZE OPL3BNK */
#define OPL3BNK_SIZE    25

static const uint16_t freq_table[] = {
    0x02b2,0x02b4,0x02b7,0x02b9,0x02bc,0x02be,0x02c1,0x02c3,0x02c6,0x02c9,
    0x02cb,0x02ce,0x02d0,0x02d3,0x02d6,0x02d8,0x02db,0x02dd,0x02e0,0x02e3,
    0x02e5,0x02e8,0x02eb,0x02ed,0x02f0,0x02f3,0x02f6,0x02f8,0x02fb,0x02fe,
    0x0301,0x0303,0x0306,0x0309,0x030c,0x030f,0x0311,0x0314,0x0317,0x031a,
    0x031d,0x0320,0x0323,0x0326,0x0329,0x032b,0x032e,0x0331,0x0334,0x0337,
    0x033a,0x033d,0x0340,0x0343,0x0346,0x0349,0x034c,0x034f,0x0352,0x0356,
    0x0359,0x035c,0x035f,0x0362,0x0365,0x0368,0x036b,0x036f,0x0372,0x0375,
    0x0378,0x037b,0x037f,0x0382,0x0385,0x0388,0x038c,0x038f,0x0392,0x0395,
    0x0399,0x039c,0x039f,0x03a3,0x03a6,0x03a9,0x03ad,0x03b0,0x03b4,0x03b7,
    0x03bb,0x03be,0x03c1,0x03c5,0x03c8,0x03cc,0x03cf,0x03d3,0x03d7,0x03da,
    0x03de,0x03e1,0x03e5,0x03e8,0x03ec,0x03f0,0x03f3,0x03f7,0x03fb,0x03fe,
    0xfe01,0xfe03,0xfe05,0xfe07,0xfe08,0xfe0a,0xfe0c,0xfe0e,0xfe10,0xfe12,
    0xfe14,0xfe16,0xfe18,0xfe1a,0xfe1c,0xfe1e,0xfe20,0xfe21,0xfe23,0xfe25,
    0xfe27,0xfe29,0xfe2b,0xfe2d,0xfe2f,0xfe31,0xfe34,0xfe36,0xfe38,0xfe3a,
    0xfe3c,0xfe3e,0xfe40,0xfe42,0xfe44,0xfe46,0xfe48,0xfe4a,0xfe4c,0xfe4f,
    0xfe51,0xfe53,0xfe55,0xfe57,0xfe59,0xfe5c,0xfe5e,0xfe60,0xfe62,0xfe64,
    0xfe67,0xfe69,0xfe6b,0xfe6d,0xfe6f,0xfe72,0xfe74,0xfe76,0xfe79,0xfe7b,
    0xfe7d,0xfe7f,0xfe82,0xfe84,0xfe86,0xfe89,0xfe8b,0xfe8d,0xfe90,0xfe92,
    0xfe95,0xfe97,0xfe99,0xfe9c,0xfe9e,0xfea1,0xfea3,0xfea5,0xfea8,0xfeaa,
    0xfead,0xfeaf
};

static const uint8_t note_octave[96] = {
    0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1, 1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,
    2,2,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4, 4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,
    5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,7, 7,7,7,7,7,7,7,7,7,7,7
};

static const uint8_t note_halftone[96] = {
    0,1,2,3,4,5,6,7,8,9,10,11,0,1,2,3,4, 5,6,7,8,9,10,11,0,1,2,3,4,5,6,7,8,9,
    10,11,0,1,2,3,4,5,6,7,8,9,10,11,0,1,2, 3,4,5,6,7,8,9,10,11,0,1,2,3,4,5,6,7,
    8,9,10,11,0,1,2,3,4,5,6,7,8,9,10,11,0, 1,2,3,4,5,6,7,8,9,10,11
};

/* registers 01h..F5h at reset */
static const uint8_t array0_init[0xF5] = {
    0x20,0,0,0x60,0,0,0,0,0,0,0,0,0,0,0,                    /* 01-0f */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
    63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,
    63,63,63,63,63,63,0,0,0,0,0,0,0,0,0,0,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,
    15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
    15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,DEF_AV_DEPTH,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0
};
static const uint8_t array1_init[0xF5] = {
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
    63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,
    63,63,63,63,63,63,0,0,0,0,0,0,0,0,0,0,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,
    15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
    15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0
};

static const uint8_t vel_graph[16] = { 82,85,88,91,94,97,100,103,106,109,112,115,118,121,124,127 };

static uint8_t pan_graph(int i)         /* STEREO_3812 */
{
    return (uint8_t)(i < 63 ? i * 2 : 127);
}

static const uint8_t op_0[18] = { 0,1,2,6,7,8,12,13,14,18,19,20,24,25,26,30,31,32 };
static const uint8_t op_1[18] = { 3,4,5,9,10,11,15,16,17,21,22,23,27,28,29,33,34,35 };
static const uint8_t op_index[36] = { 0,1,2,3,4,5,8,9,10,11,12,13,16,17,18,19,20,21,
                                      0,1,2,3,4,5,8,9,10,11,12,13,16,17,18,19,20,21 };
static const uint8_t op_array[36] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
static const uint8_t voice_num[18] = { 0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8 };
static const uint8_t voice_array[18] = { 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1 };
static const uint8_t op4_base[18] = { 1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0 };
static const int8_t alt_voice[18] = { 3,4,5,0,1,2,-1,-1,-1,12,13,14,9,10,11,-1,-1,-1 };
static const int8_t alt_op_0[18] = { 6,7,8,0,1,2,-1,-1,-1,24,25,26,18,19,20,-1,-1,-1 };
static const int8_t alt_op_1[18] = { 9,10,11,3,4,5,-1,-1,-1,27,28,29,21,22,23,-1,-1,-1 };
static const uint8_t conn_sel[18] = { 1,2,4,1,2,4,0,0,0,8,16,32,8,16,32,0,0,0 };
static const uint8_t op4_voice[6] = { 0,1,2,9,10,11 };
static const uint8_t carrier_01[4] = { 0, 1, 2, 1 };
static const uint8_t carrier_23[4] = { 2, 2, 2, 3 };

static uint16_t rd16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }

/* update_reg: CL to register BX (BH the YMF262 array) */
static void update_reg(Yam *y, unsigned reg, unsigned v)
{
    hw_opl_write(0, reg, v & 0xFF);
    if (y->stereo) hw_opl_write(1, reg & 0xFF, v & 0xFF);
}

static void write_register(Yam *y, int op, unsigned base, unsigned v)
{
    update_reg(y, (unsigned)op_array[op] << 8 | ((op_index[op] + base) & 0xFF), v);
}

static void send_byte(Yam *y, int voice, unsigned base, unsigned v)
{
    update_reg(y, (unsigned)voice_array[voice] << 8 | ((voice_num[voice] + base) & 0xFF), v);
}

/* stereo_register: the left chip gets RLValues' high byte, the right its low byte */
static void stereo_register(Yam *y, int part, unsigned base, unsigned rl)
{
    unsigned reg = (base + op_index[part]) & 0xFF;
    hw_opl_write(0, reg, rl >> 8 & 0xFF);
    hw_opl_write(1, reg, rl & 0xFF);
}

static void reset_synth(Synth *s)
{
    Yam *y = (Yam *)s;
    unsigned r;
    if (y->ymf262) {
        update_reg(y, 0x105, 1);
        update_reg(y, 0x104, 0);
        y->conn_shadow = 0;
    }
    for (r = 1; r <= 0xF5; r++) update_reg(y, r, array0_init[r - 1]);
    if (y->ymf262)
        for (r = 0x101; r <= 0x1F5; r++) update_reg(y, r, array1_init[r - 0x101]);
}

static void shutdown_synth(Synth *s)
{
    Yam *y = (Yam *)s;
    if (y->ymf262) update_reg(y, 0x105, 0);
}

/* ---- the timbre cache --------------------------------------------------------------- */

static int index_timbre(Synth *s, unsigned gnum)
{
    Yam *y = (Yam *)s;
    int si;
    for (si = 0; si < MAX_TIMBS; si++)
        if ((y->timb_attribs[si] & 0x80) && y->timb_bank[si] == (gnum >> 8 & 0xFF)
                && y->timb_num[si] == (gnum & 0xFF))
            return si;
    return -1;
}

static void protect_timbre(Synth *s, unsigned bank, unsigned num, int on)
{
    Yam *y = (Yam *)s;
    int i;
    unsigned ax = (bank & 0xFF) << 8 | (num & 0xFF);
    if (ax == 0xFFFF) {
        for (i = 0; i < MAX_TIMBS; i++)
            y->timb_attribs[i] = on ? y->timb_attribs[i] | 0x40 : y->timb_attribs[i] & 0xBF;
        return;
    }
    i = index_timbre(s, ax);
    if (i == -1) return;
    y->timb_attribs[i] = on ? y->timb_attribs[i] | 0x40 : y->timb_attribs[i] & 0xBF;
}

static unsigned timbre_status(Synth *s, unsigned bank, unsigned num)
{
    Yam *y = (Yam *)s;
    int i = index_timbre(s, (bank & 0xFF) << 8 | (num & 0xFF));
    if (i == -1) return 0;
    return (uint16_t)(y->timb_offsets[i] + 1);  /* the offset + 1 in the local cache */
}

static unsigned cache_size(Synth *s)
{
    (void)s;
    return DEF_TC_SIZE;
}

static void define_cache(Synth *s, uint8_t *addr, unsigned size)
{
    Yam *y = (Yam *)s;
    y->cache_base = addr;
    y->cache_size = size;
    y->cache_end = 0;
}

static void release_voice(Yam *y, int si);

static void delete_LRU(Yam *y)
{
    int index = -1, si, di;
    uint32_t best = 0xFFFFFFFFu;
    unsigned toff, tsize;
    uint8_t *t;
    for (si = 0; si < MAX_TIMBS; si++) {
        if (!(y->timb_attribs[si] & 0x80) || (y->timb_attribs[si] & 0x40)) continue;
        if (y->timb_hist[si] > best) continue;
        best = y->timb_hist[si];
        index = si;
    }
    if (index == -1) return;
    toff = y->timb_offsets[index];
    t = y->cache_base + toff;
    tsize = rd16(t);
    memmove(t, t + tsize, (size_t)(y->cache_end - toff - tsize));  /* REP_MOVSB, forward */
    y->timb_attribs[index] = 0;
    y->cache_end -= tsize;
    for (di = 0; di < NUM_CHANS; di++)
        if (y->MIDI_timbre[di] != 0xFF && y->MIDI_timbre[di] == index) y->MIDI_timbre[di] = 0xFF;
    for (di = 0; di < 128; di++)
        if (y->RBS_timbres[di] == index) y->RBS_timbres[di] = 0xFF;
    for (di = 0; di < MAX_TIMBS; di++)
        if ((y->timb_attribs[di] & 0x80) && y->timb_offsets[di] > toff)
            y->timb_offsets[di] = (uint16_t)(y->timb_offsets[di] - tsize);
    for (si = 0; si < y->nslots; si++) {
        if (y->S_status[si] == FREE) continue;
        if (y->S_timbre[si] < t) continue;
        if (y->S_timbre[si] == t) {
            release_voice(y, si);
            y->S_status[si] = FREE;
        } else
            y->S_timbre[si] -= tsize;
    }
}

static void install_timbre(Synth *s, unsigned bank, unsigned num, const uint8_t *addr)
{
    Yam *y = (Yam *)s;
    int index, di;
    unsigned bx, sz;
    index = index_timbre(s, (bank & 0xFF) << 8 | (num & 0xFF));
    if (index == -1) {
        if (!addr) return;
        for (;;) {
            for (di = 0; di < MAX_TIMBS && (y->timb_attribs[di] & 0x80); di++)
                ;
            if (di < MAX_TIMBS) {
                sz = rd16(addr);
                bx = (sz + y->cache_end) & 0xFFFF;
                if (bx <= y->cache_size) break;
            }
            delete_LRU(y);              /* until the new timbre fits */
        }
        index = di;
        bx = y->cache_end;
        y->cache_end = (bx + rd16(addr)) & 0xFFFF;
        y->timb_num[di] = (uint8_t)num;
        y->timb_bank[di] = (uint8_t)bank;
        y->timb_attribs[di] = 0x80;
        y->timb_hist[di] = y->note_event++;
        y->timb_offsets[di] = (uint16_t)bx;
        memcpy(y->cache_base + bx, addr, rd16(addr));
    }
    for (di = 0; di < NUM_CHANS; di++)
        if (y->MIDI_program[di] == (uint8_t)num && y->MIDI_bank[di] == (uint8_t)bank)
            y->MIDI_timbre[di] = (uint8_t)index;
}

/* ---- voices ------------------------------------------------------------------------ */

static void init_synth(Synth *s)
{
    Yam *y = (Yam *)s;
    int i;
    y->note_event = 0;
    memset(y->timb_attribs, 0, sizeof y->timb_attribs);
    for (i = 0; i < NUM_CHANS; i++) {
        y->MIDI_timbre[i] = 0xFF;
        y->MIDI_voices[i] = 0;
        y->MIDI_program[i] = 0xFF;
        y->MIDI_bank[i] = 0;
    }
    for (i = 0; i < y->nslots; i++) y->S_status[i] = FREE;
    for (i = 0; i < y->nvoices; i++) y->V_channel[i] = 0xFF;
    memset(y->RBS_timbres, 0xFF, sizeof y->RBS_timbres);
    y->TV_accum = 0;
    y->pri_accum = 0;
    y->vol_update = 0;
    y->rover_2op = -1;
    y->rover_4op = -1;
}

static void update_voice(Yam *y, int si);
static void update_priority(Yam *y);

static void assign_voice(Yam *y, int si)
{
    int dx, bx;
    if (y->ymf262 && y->S_type[si] == OPL3_INST) {
        int di = y->rover_4op;
        for (dx = 0; dx < 6; dx++) {
            if (++di == 6) di = 0;
            y->rover_4op = di;
            bx = op4_voice[di];
            if (y->V_channel[bx] != 0xFF || y->V_channel[bx + 3] != 0xFF) continue;
            y->S_voice[si] = (uint8_t)bx;
            y->MIDI_voices[y->S_channel[si]]++;
            y->V_channel[bx] = y->S_channel[si];
            y->V_channel[bx + 3] = y->S_channel[si];
            y->S_update[si] = U_ALL_REGS;
            update_voice(y, si);
            return;
        }
        update_priority(y);
        return;
    }
    bx = y->rover_2op;
    for (dx = 0; dx < y->nvoices; dx++) {
        if (++bx == y->nvoices) bx = 0;
        y->rover_2op = bx;
        if (y->V_channel[bx] != 0xFF) continue;
        y->S_voice[si] = (uint8_t)bx;
        y->MIDI_voices[y->S_channel[si]]++;
        y->V_channel[bx] = y->S_channel[si];
        y->S_update[si] = U_ALL_REGS;
        update_voice(y, si);
        return;
    }
    update_priority(y);                 /* more active slots than voices */
}

static void release_voice(Yam *y, int si)
{
    int bx;
    if (y->S_voice[si] == 0xFF) return;
    y->S_BLOCK[si] &= 0xDF;
    y->S_update[si] |= U_FREQ;          /* KON = 0 ... */
    update_voice(y, si);                /* ... silences the note ... */
    y->MIDI_voices[y->S_channel[si]]--; /* ... and the voice is deallocated */
    bx = y->S_voice[si];
    if (y->S_type[si] == OPL3_INST) y->V_channel[bx + 3] = 0xFF;
    y->V_channel[bx] = 0xFF;
    y->S_voice[si] = 0xFF;
    if (y->S_type[si] == OPL3_INST || y->S_type[si] == BNK_INST)
        y->S_status[si] = FREE;         /* otherwise, slot remains active (a TV slot), unvoiced */
}

/* the rounding of YAMAHA.INC's level arithmetic: (AX*2)/256, then up by one unless 0 */
static uint8_t scale127(unsigned a, unsigned b)
{
    unsigned ax = (a & 0xFF) * (b & 0xFF);
    uint8_t al = (uint8_t)((ax << 1) >> 8);
    return al ? (uint8_t)(al + 1) : 0;
}

static void update_voice(Yam *y, int si)
{
    int ch, array, voice, voice0, voice1, d;
    uint8_t vol = 0, lvol = 0, rvol = 0, scale_flags, update_save = 0;
    uint8_t AVEKM_0, AVEKM_1, KSLTL_0, KSLTL_1, AD_0, AD_1, SR_0, SR_1, FBC;
    uint16_t m0_val, m1_val, v0_val, v1_val, ws_val, fb_val, f_num;
    if (y->S_voice[si] == 0xFF) return;
    ch = y->S_channel[si] & 0x0F;
    if (y->S_update[si] & U_KSLTL) {
        vol = scale127(y->MIDI_vol[ch], y->MIDI_express[ch]);
        vol = scale127(vol, y->S_velocity[si]);
        if (y->stereo) {
            lvol = scale127(pan_graph(y->MIDI_pan[ch]), vol);
            rvol = scale127(pan_graph(127 - y->MIDI_pan[ch]), vol);
        }
    }
    array = y->S_type[si] == OPL3_INST;     /* the second array first for an OPL3 4-op voice */
    if (y->ymf262) {
        uint8_t cl = y->conn_shadow, dl = conn_sel[y->S_voice[si]];
        int di = y->S_voice[si];
        if (array) {
            cl |= dl;
            if (cl != y->conn_shadow) {
                y->conn_shadow = cl;
                update_reg(y, 0x104, cl);
            }
        } else {
            cl &= (uint8_t)~dl;
            if (cl != y->conn_shadow) {
                y->conn_shadow = cl;
                update_reg(y, 0x104, cl);
                /* mute the other half of the old 4-op voice */
                write_register(y, alt_op_0[di], 0x80, 0x0F);
                write_register(y, alt_op_1[di], 0x80, 0x0F);
                send_byte(y, alt_voice[di], 0xB0, 0);
            }
        }
    }
    for (;;) {
        if (array) {
            voice = y->S_voice[si] + 3;
            voice0 = op_0[voice];
            voice1 = op_1[voice];
            m0_val = y->S_m2_val[si];
            m1_val = y->S_m3_val[si];
            AVEKM_0 = y->S_AVEKM_2[si];
            AVEKM_1 = y->S_AVEKM_3[si];
            v0_val = y->S_v2_val[si];
            v1_val = y->S_v3_val[si];
            KSLTL_0 = y->S_KSLTL_2[si];
            KSLTL_1 = y->S_KSLTL_3[si];
            AD_0 = y->S_AD_2[si];
            AD_1 = y->S_AD_3[si];
            SR_0 = y->S_SR_2[si];
            SR_1 = y->S_SR_3[si];
            fb_val = 0;
            ws_val = y->S_ws_val_2[si];
            FBC = y->S_FBC[si] >> 1;
            scale_flags = y->S_scale_23[si];
            update_save = y->S_update[si];  /* the flags again for the first pair */
        } else {
            voice = y->S_voice[si];
            voice0 = op_0[voice];
            voice1 = op_1[voice];
            m0_val = y->S_m0_val[si];
            m1_val = y->S_m1_val[si];
            AVEKM_0 = y->S_AVEKM_0[si];
            AVEKM_1 = y->S_AVEKM_1[si];
            v0_val = y->S_v0_val[si];
            v1_val = y->S_v1_val[si];
            KSLTL_0 = y->S_KSLTL_0[si];
            KSLTL_1 = y->S_KSLTL_1[si];
            AD_0 = y->S_AD_0[si];
            AD_1 = y->S_AD_1[si];
            SR_0 = y->S_SR_0[si];
            SR_1 = y->S_SR_1[si];
            fb_val = y->S_fb_val[si];
            ws_val = y->S_ws_val[si];
            FBC = y->S_FBC[si];
            scale_flags = y->S_scale_01[si];
        }
        if (y->S_update[si] & U_AVEKM) {
            unsigned vib = (int8_t)y->MIDI_mod[ch] >= 64 ? 0x40 : 0;
            write_register(y, voice0, 0x20, ((m0_val >> 12) | vib | AVEKM_0) & 0xFF);
            write_register(y, voice1, 0x20, ((m1_val >> 12) | vib | AVEKM_1) & 0xFF);
            y->S_update[si] &= (uint8_t)~U_AVEKM;
        }
        if (y->S_update[si] & U_KSLTL) {
            uint8_t lv = (uint8_t)(v0_val >> 10), rv = lv;
            if (y->stereo) {
                if (scale_flags & 1) {
                    lv = (uint8_t)((unsigned)lv * lvol / 127);
                    rv = (uint8_t)((unsigned)rv * rvol / 127);
                }
                lv = (uint8_t)((~lv & 0x3F) | KSLTL_0);
                rv = (uint8_t)((~rv & 0x3F) | KSLTL_0);
                stereo_register(y, voice0, 0x40, (unsigned)lv << 8 | rv);
            } else {
                if (scale_flags & 1) lv = (uint8_t)((unsigned)lv * vol / 127);
                write_register(y, voice0, 0x40, (uint8_t)((~lv & 0x3F) | KSLTL_0));
            }
            lv = rv = (uint8_t)(v1_val >> 10);
            if (y->stereo) {
                if (scale_flags & 2) {
                    lv = (uint8_t)((unsigned)lv * lvol / 127);
                    rv = (uint8_t)((unsigned)rv * rvol / 127);
                }
                lv = (uint8_t)((~lv & 0x3F) | KSLTL_1);
                rv = (uint8_t)((~rv & 0x3F) | KSLTL_1);
                stereo_register(y, voice1, 0x40, (unsigned)lv << 8 | rv);
            } else {
                if (scale_flags & 2) lv = (uint8_t)((unsigned)lv * vol / 127);
                write_register(y, voice1, 0x40, (uint8_t)((~lv & 0x3F) | KSLTL_1));
            }
            y->S_update[si] &= (uint8_t)~U_KSLTL;
        }
        if (y->S_update[si] & U_ADSR) {
            write_register(y, voice0, 0x60, AD_0);
            write_register(y, voice1, 0x60, AD_1);
            write_register(y, voice0, 0x80, SR_0);
            write_register(y, voice1, 0x80, SR_1);
            y->S_update[si] &= (uint8_t)~U_ADSR;
        }
        if (y->S_update[si] & U_WS) {
            write_register(y, voice1, 0xE0, ws_val & 0xFF);
            write_register(y, voice0, 0xE0, ws_val >> 8);
            y->S_update[si] &= (uint8_t)~U_WS;
        }
        if (y->S_update[si] & U_FBC) {
            unsigned al = (FBC & 1) | ((fb_val >> 12) & 0x0E);
            if (y->ymf262) {
                uint8_t pan = y->MIDI_pan[ch];
                al |= 0x30;                 /* centre */
                if (pan <= R_PAN_THRESH) al &= y->kind == DRV_SBPRO2 ? 0xEF : 0xDF;
                else if (pan >= L_PAN_THRESH) al &= y->kind == DRV_SBPRO2 ? 0xDF : 0xEF;
            }
            send_byte(y, voice, 0xC0, al);
            y->S_update[si] &= (uint8_t)~U_FBC;
        }
        if (y->S_update[si] & U_FREQ) {
            if (!array) {
                if (y->S_type[si] == TV_EFFECT) {
                    f_num = y->S_f_val[si] >> 6;    /* the TV frequency value is the F-number */
                    goto set_freq;
                }
                if (!(y->S_BLOCK[si] & 0x20)) {     /* KON 0: the note off at once */
                    send_byte(y, voice, 0xB0, y->S_KBF_shadow[si] & 0xDF);
                } else {
                    int16_t ax;
                    int bx, dxi;
                    int8_t bl;
                    uint16_t fr;
                    ax = (int16_t)(((unsigned)y->MIDI_pitch_h[y->S_channel[si]] << 7 |
                                    y->MIDI_pitch_l[y->S_channel[si]]) - 0x2000);
                    ax = (int16_t)(ax >> 5);
                    ax = (int16_t)(ax * DEF_PITCH_RANGE);
                    bx = y->S_note[si] + (int8_t)y->S_transpose[si] - 24;
                    do bx += 12; while (bx < 0);
                    bx += 12;
                    do bx -= 12; while (bx > 95);
                    ax = (int16_t)((uint16_t)ax + ((unsigned)(bx & 0xFF) << 8));
                    ax = (int16_t)(ax + 8);
                    ax = (int16_t)(ax >> 4);
                    ax = (int16_t)(ax - 12 * 16);
                    do ax = (int16_t)(ax + 12 * 16); while (ax < 0);
                    ax = (int16_t)(ax + 12 * 16);
                    do ax = (int16_t)(ax - 12 * 16); while (ax > 96 * 16 - 1);
                    dxi = (uint16_t)ax >> 4;
                    d = note_halftone[dxi] << 5;
                    d += ((uint16_t)ax << 1) & 0x1F;
                    fr = freq_table[d >> 1];
                    bl = (int8_t)(note_octave[dxi] - 1);
                    if ((int16_t)fr < 0) bl++;
                    if (bl < 0) {
                        bl++;
                        fr = (uint16_t)((int16_t)fr >> 1);
                    }
                    f_num = (uint16_t)((fr & 0x3FF) | (((unsigned)(uint8_t)(bl << 2)) << 8));
set_freq:
                    send_byte(y, voice, 0xA0, f_num & 0xFF);
                    y->S_KBF_shadow[si] = (uint8_t)((f_num >> 8) | y->S_BLOCK[si]);
                    send_byte(y, voice, 0xB0, y->S_KBF_shadow[si]);
                }
            }
            y->S_update[si] &= (uint8_t)~U_FREQ;
        }
        if (!array) return;
        array = 0;
        y->S_update[si] = update_save;
    }
}

static void update_priority(Yam *y)
{
    int slot_cnt = 0, si, low_p = 0, high_p = 0, low_4_p = 0, ch, bx;
    unsigned ax, dx, cx, di;
    for (si = 0; si < y->nslots; si++) {
        if (y->S_status[si] == FREE) continue;
        slot_cnt++;
        ch = y->S_channel[si] & 0x0F;
        ax = (int8_t)y->MIDI_vprot[ch] >= 64 ? 0xFFFF : y->S_p_val[si];
        ax = ax >= y->MIDI_voices[ch] ? ax - y->MIDI_voices[ch] : 0;
        y->S_V_priority[si] = (uint16_t)ax;
    }
    for (;;) {
        ax = 0;                         /* the highest unvoiced priority */
        dx = 0xFFFF;                    /* the lowest voiced */
        cx = 0xFFFF;                    /* the lowest voiced 4-op */
        for (si = 0; si < y->nslots; si++) {
            if (y->S_status[si] == FREE) continue;
            di = y->S_V_priority[si];
            bx = y->S_voice[si];
            if (bx == 0xFF) {
                if (di < ax) continue;
                ax = di;
                high_p = si;
                continue;
            }
            if (op4_base[bx] && di <= cx) {
                cx = di;
                low_4_p = si;
            }
            if (di > dx) continue;
            dx = di;
            low_p = si;
        }
        if (ax < dx || ax == 0) return;
        si = low_p;                     /* steal a voice */
        if (y->ymf262 && y->S_type[high_p] == OPL3_INST) {
            si = low_4_p;
            if (y->S_type[si] != OPL3_INST) {   /* a 4-op voice takes two 2-op ones */
                /* (YAMAHA.INC indexes alt_voice by the slot, not its voice; past its
                   18 entries the table that follows it, alt_op_0, is read) */
                int8_t al = si < 18 ? alt_voice[si] : alt_op_0[si - 18];
                int d2;
                for (d2 = 0; d2 < y->nslots; d2++)
                    if (y->S_status[d2] != FREE && (int8_t)y->S_voice[d2] == al) {
                        release_voice(y, d2);
                        break;
                    }
            }
        }
        bx = y->S_voice[si];
        release_voice(y, si);
        si = high_p;
        y->S_voice[si] = (uint8_t)bx;
        y->MIDI_voices[y->S_channel[si]]++;
        y->V_channel[bx] = y->S_channel[si];
        if (y->S_type[si] == OPL3_INST) y->V_channel[bx + 3] = y->S_channel[si];
        y->S_update[si] = U_ALL_REGS;
        update_voice(y, si);
        if (--slot_cnt == 0) return;
    }
}

static void BNK_phase(Yam *y, int si)
{
    const uint8_t *t = y->S_timbre[si];
    y->S_BLOCK[si] = 0x20;              /* KON, and no BLOCK */
    y->S_type[si] = BNK_INST;
    y->S_duration[si] = 0xFFFF;
    y->S_p_val[si] = 32767;             /* the average priority */
    y->S_FBC[si] = t[8] & 1;            /* B_fb_c */
    y->S_fb_val[si] = (uint16_t)(t[8] << 12);
    y->S_KSLTL_0[si] = t[4] & 0xC0;     /* B_mod_KSLTL */
    y->S_v0_val[si] = (uint16_t)((~t[4] & 0x3F) << 10);
    y->S_KSLTL_1[si] = t[10] & 0xC0;    /* B_car_KSLTL */
    y->S_v1_val[si] = (uint16_t)((~t[10] & 0x3F) << 10);
    y->S_AVEKM_0[si] = t[3] & 0xF0;     /* B_mod_AVEKM */
    y->S_m0_val[si] = (uint16_t)(t[3] << 12);
    y->S_AVEKM_1[si] = t[9] & 0xF0;     /* B_car_AVEKM */
    y->S_m1_val[si] = (uint16_t)(t[9] << 12);
    y->S_AD_0[si] = t[5];
    y->S_SR_0[si] = t[6];
    y->S_AD_1[si] = t[11];
    y->S_SR_1[si] = t[12];
    y->S_ws_val[si] = (uint16_t)(t[13] | t[7] << 8);  /* car_WS, mod_WS */
    y->S_scale_01[si] = y->S_FBC[si] | 2;   /* the carrier always scaled */
    y->S_update[si] = U_ALL_REGS;
}

static void OPL_phase(Yam *y, int si)
{
    const uint8_t *t;
    BNK_phase(y, si);
    t = y->S_timbre[si];
    y->S_type[si] = OPL3_INST;
    y->S_FBC[si] |= (t[8] & 0x80) >> 6;     /* the second connection bit */
    y->S_scale_01[si] = carrier_01[y->S_FBC[si] & 3];
    y->S_scale_23[si] = carrier_23[y->S_FBC[si] & 3];
    y->S_KSLTL_2[si] = t[15] & 0xC0;
    y->S_v2_val[si] = (uint16_t)((~t[15] & 0x3F) << 10);
    y->S_KSLTL_3[si] = t[21] & 0xC0;
    y->S_v3_val[si] = (uint16_t)((~t[21] & 0x3F) << 10);
    y->S_AVEKM_2[si] = t[14] & 0xF0;
    y->S_m2_val[si] = (uint16_t)(t[14] << 12);
    y->S_AVEKM_3[si] = t[20] & 0xF0;
    y->S_m3_val[si] = (uint16_t)(t[20] << 12);
    y->S_AD_2[si] = t[16];
    y->S_SR_2[si] = t[17];
    y->S_AD_3[si] = t[22];
    y->S_SR_3[si] = t[23];
    y->S_ws_val_2[si] = (uint16_t)(t[24] | t[18] << 8);
}

/* YAMAHA.INC's procedures, for an extension (yamaha.h) */
void yam_update_voice(Yam *y, int si) { update_voice(y, si); }
void yam_update_priority(Yam *y) { update_priority(y); }
void yam_release_voice(Yam *y, int si) { release_voice(y, si); }

/* XMIDI.ASM's IFDEF serve_synth: only an extension has one */
static void serve_ext(Synth *s)
{
    Yam *y = (Yam *)s;
    y->ext->serve(y);
}

/* ---- the MIDI interpreter ---------------------------------------------------------- */

static void note_off(Yam *y, int chan, int note)
{
    int si;
    for (si = 0; si < y->nslots; si++) {
        if (y->S_status[si] != KEYON || y->S_keynum[si] != (uint8_t)note
                || y->S_channel[si] != (uint8_t)chan)
            continue;
        if ((int8_t)y->MIDI_sus[chan & 0xFF] >= 64) {
            y->S_sustain[si] = 1;
        } else if (y->S_type[si] == OPL3_INST || y->S_type[si] == BNK_INST) {
            release_voice(y, si);
            y->S_status[si] = FREE;
            if (y->ext && y->ext->voice_freed)
                y->ext->voice_freed(y);     /* IFDEF TV_switch_voice */
        } else
            y->S_duration[si] = 1;      /* a TV note: its last cycle */
    }
}

static void note_on(Yam *y, int chan, int note, int vel)
{
    int si, bl;
    uint8_t *di;
    uint8_t cl, al;
    unsigned sz;
    bl = y->MIDI_timbre[chan];
    if (chan == 9) {                    /* the rhythm channel: a timbre per key */
        bl = y->RBS_timbres[note & 0x7F];
        if (bl == 0xFF) {
            bl = index_timbre(&y->s, 127 << 8 | (note & 0xFF)) & 0xFF;
            y->RBS_timbres[note & 0x7F] = (uint8_t)bl;
        }
    }
    if (bl == 0xFF) return;             /* the timbre is not loaded */
    di = y->cache_base + y->timb_offsets[bl];
    y->timb_hist[bl] = ++y->note_event;
    for (si = 0; si < y->nslots && y->S_status[si] != FREE; si++)
        ;
    if (si == y->nslots) return;        /* no virtual voice free */
    y->S_channel[si] = (uint8_t)chan;
    y->S_keynum[si] = (uint8_t)note;
    al = 0;
    cl = di[2];
    if (chan != 9) {
        al = cl;
        cl = (uint8_t)note;
    }
    y->S_note[si] = cl;
    y->S_transpose[si] = al;
    y->S_velocity[si] = vel_graph[(vel & 0xFF) >> 3 & 15];
    y->S_timbre[si] = di;
    y->S_status[si] = KEYON;
    y->S_sustain[si] = 0;
    sz = rd16(di);
    if (sz == OPL3BNK_SIZE) {
        if (y->ymf262) OPL_phase(y, si);
    } else if (sz == BNK_SIZE)
        BNK_phase(y, si);
    else if (y->ext)
        y->ext->phase(y, si);           /* IFDEF TV_phase */
    else
        return;                         /* no TVFX: jmp __exit, the slot left KEYON */
    y->S_voice[si] = 0xFF;
    assign_voice(y, si);
}

static void release_sustain(Yam *y, int chan)
{
    int si;
    for (si = 0; si < y->nslots; si++)
        if (y->S_status[si] != FREE && y->S_channel[si] == (uint8_t)chan && y->S_sustain[si])
            note_off(y, chan, y->S_note[si]);
}

static void flag_updates(Yam *y, int chan, uint8_t flag)
{
    int si;
    for (si = 0; si < y->nslots; si++) {
        if (y->S_status[si] == FREE || y->S_channel[si] != (uint8_t)chan) continue;
        y->S_update[si] |= flag;
        update_voice(y, si);
    }
}

static void send_MIDI_message(Synth *s, unsigned stat, unsigned d1, unsigned d2)
{
    Yam *y = (Yam *)s;
    int di = (int)(stat & 0x0F), si = (int)(d1 & 0xFF), bl;
    unsigned cx = d2 & 0xFF;
    uint8_t *v;
    uint8_t flag;
    switch (stat & 0xF0) {
    case 0x80:
        note_off(y, di, si);
        return;
    case 0x90:
        if (di < s->min_true_chan - 1 || di > s->max_rec_chan - 1) return;
        if (cx == 0) note_off(y, di, si);
        else note_on(y, di, si, (int)cx);
        return;
    case 0xE0:
        y->MIDI_pitch_l[di] = (uint8_t)si;
        y->MIDI_pitch_h[di] = (uint8_t)cx;
        flag_updates(y, di, U_FREQ);
        return;
    case 0xC0:
        y->MIDI_program[di] = (uint8_t)si;
        y->MIDI_timbre[di] = (uint8_t)index_timbre(s, (unsigned)y->MIDI_bank[di] << 8 | (unsigned)si);
        return;
    case 0xB0:
        break;
    default:
        return;
    }
    switch (si) {
    case PATCH_BANK_SEL:
        y->MIDI_bank[di] = (uint8_t)cx;
        return;
    case VOICE_PROTECT:
        y->MIDI_vprot[di] = (uint8_t)cx;
        return;
    case TIMBRE_PROTECT:
        bl = y->MIDI_timbre[di];
        if (bl == 0xFF) return;
        y->timb_attribs[bl] = (uint8_t)((y->timb_attribs[bl] & 0xBF) | ((int8_t)cx >= 64 ? 0x40 : 0));
        return;
    case MODULATION: v = y->MIDI_mod; flag = U_AVEKM; break;
    case PART_VOLUME: v = y->MIDI_vol; flag = U_KSLTL; break;
    case EXPRESSION: v = y->MIDI_express; flag = U_KSLTL; break;
    case PANPOT: v = y->MIDI_pan; flag = y->ymf262 ? U_FBC : U_KSLTL; break;
    case SUSTAIN:
        y->MIDI_sus[di] = (uint8_t)cx;
        if ((int8_t)cx < 64) release_sustain(y, di);
        return;
    case RESET_ALL_CTRLS:
        y->MIDI_sus[di] = 0;
        release_sustain(y, di);
        y->MIDI_mod[di] = 0;            /* the Roland LAPC-1's reset */
        y->MIDI_express[di] = 127;
        y->MIDI_pitch_l[di] = 0x00;
        y->MIDI_pitch_h[di] = 0x40;
        flag_updates(y, di, U_AVEKM | U_KSLTL | U_FREQ);
        return;
    case ALL_NOTES_OFF:
        for (bl = 0; bl < y->nslots; bl++)
            if (y->S_status[bl] == KEYON && y->S_channel[bl] == (uint8_t)di)
                note_off(y, di, y->S_note[bl]);
        return;
    default:
        return;
    }
    v[di] = (uint8_t)cx;
    flag_updates(y, di, flag);
}

Synth *yamaha_new(int kind, const AilFmExt *ext)
{
    Yam *y = calloc(1, sizeof *y);
    y->ext = ext;
    if (ext && ext->state_size) y->ext_state = calloc(1, ext->state_size);
    y->kind = kind;
    y->ymf262 = kind == DRV_SBPRO2;
    y->stereo = kind == DRV_SBPRO1 || kind == DRV_PASFM;
    y->nvoices = y->ymf262 ? 18 : 9;
    y->nslots = y->ymf262 ? 20 : 16;
    y->s.min_true_chan = 2;
    y->s.max_true_chan = 9;
    y->s.max_rec_chan = 10;
    y->s.def_synth_vol = 100;
    y->s.reset = reset_synth;
    y->s.init = init_synth;
    y->s.shutdown = shutdown_synth;
    y->s.serve = ext && ext->serve ? serve_ext : 0;
    y->s.send = send_MIDI_message;
    y->s.cache_size = cache_size;
    y->s.define_cache = define_cache;
    y->s.index_timbre = index_timbre;
    y->s.install = install_timbre;
    y->s.protect = protect_timbre;
    y->s.timbre_status = timbre_status;
    hw_opl_mode(kind);
    return &y->s;
}
