/* tvfx.c: replaces the time-variant effects (TVFX) of UW1's FM music drivers ADLIB, SBFM, SBPFM
   and PASFM.ADV (docs/PORT.md, "Sound"): Origin's ALE.INC, which the drivers include
   (YAMAHA.INC's OSI_ALE) and which was never released. The routines below (TV_switch_voice,
   TV_cmd, serve_synth, TV_phase) are UW2Decomp's src/port/sound/tvfx.c, translated from UW2's
   DM03.ADV: UW1's SBFM.ADV has the same code 0Dh bytes lower (0523h..0ACDh), instruction for
   instruction (only the addresses of YAMAHA.INC's data differ, which is laid out differently),
   so the C is UW2's, with SBFM.ADV's addresses. YAMAHA.INC's names for its data.

   UW1's drivers are AIL's 1991 release, whose YAMAHA.INC and XMIDI.ASM differ from 2.14's in
   the places Exhume's runtime marks "1991" (yamaha.c, xmidi.c); ail.c recognises it from the
   driver file. The extension plugs into yamaha.c through its hooks (yamaha.h, struct
   AilFmExt), as ALE.INC was INCLUDEd into YAMAHA.INC: uw1_tvfx is named by src/port/ailgame.h
   (AIL_FM_EXT) and exhume.toml's [sound] extensions. */
#include "yamaha.h"

#define FREE            YAM_FREE
#define KEYOFF          YAM_KEYOFF
#define BNK_INST        YAM_BNK_INST
#define TV_INST         YAM_TV_INST
#define U_ALL_REGS      YAM_U_ALL_REGS
#define U_AVEKM         YAM_U_AVEKM
#define U_KSLTL         YAM_U_KSLTL
#define U_WS            YAM_U_WS
#define U_FBC           YAM_U_FBC
#define U_FREQ          YAM_U_FREQ

/* Each of the eight time-variant parameters has, per slot, the offset of its next command in
   the timbre, a countdown to it, a value and an increment (the four word arrays of each
   80h-byte block of ALE.INC's data, in the order f, v0, v1, p, fb, m0, m1, ws). The values are
   YAMAHA.INC's S_f_val .. S_ws_val; the rest is kept here. */
enum { TV_F, TV_V0, TV_V1, TV_P, TV_FB, TV_M0, TV_M1, TV_WS, TV_N };

typedef struct Tv {
    uint16_t ptr[TV_N][YAM_MAXSLOTS], cnt[TV_N][YAM_MAXSLOTS], inc[TV_N][YAM_MAXSLOTS];
} Tv;

#define TV(y) ((Tv *)(y)->ext_state)

static uint16_t *tv_val(Yam *y, int p)
{
    switch (p) {
    case TV_F:  return y->S_f_val;
    case TV_V0: return y->S_v0_val;
    case TV_V1: return y->S_v1_val;
    case TV_P:  return y->S_p_val;
    case TV_FB: return y->S_fb_val;
    case TV_M0: return y->S_m0_val;
    case TV_M1: return y->S_m1_val;
    default:    return y->S_ws_val;
    }
}

static uint16_t rd16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }

/* TV_switch_voice (SBFM 0523h; DM03 0530h): after a voice was freed, give it to the first active slot
   that has none, and let update_priority settle the rest */
static void TV_switch_voice(Yam *y)
{
    int si, bx;
    for (si = 0; si < y->nslots; si++)
        if (y->S_status[si] != FREE && y->S_voice[si] == 0xFF) break;
    if (si == y->nslots) return;
    for (bx = 0; bx < y->nvoices; bx++)
        if (y->V_channel[bx] == 0xFF) break;
    if (bx == y->nvoices) return;
    y->S_voice[si] = (uint8_t)bx;
    y->MIDI_voices[y->S_channel[si]]++;
    y->V_channel[bx] = y->S_channel[si];
    y->S_update[si] = U_ALL_REGS;
    yam_update_priority(y);
}

/* TV_cmd (SBFM 0574h; DM03 0581h): reads the next commands of parameter p's list in slot si's timbre, at
   most ten: 0 n, a jump of n bytes; FFFFh v, the value v; FFFEh v, a register byte (by
   parameter: AVEKM for m0/m1, KSLTL for v0/v1, the block and KON for f, FBC for fb); any
   other count n with an increment, which ends the read. Ten without a count leave the value
   still (increment 0, count FFFFh). */
static void TV_cmd(Yam *y, int si, int p)
{
    Tv *tv = TV(y);
    const uint8_t *t = y->S_timbre[si];
    int cx;
    uint16_t ax, dx;
    for (cx = 10; cx; cx--) {
        const uint8_t *di = t + tv->ptr[p][si];
        ax = rd16(di);
        dx = rd16(di + 2);
        if (ax == 0) {
            tv->ptr[p][si] = (uint16_t)(tv->ptr[p][si] + dx);
            continue;
        }
        tv->ptr[p][si] = (uint16_t)(tv->ptr[p][si] + 4);
        if (ax == 0xFFFF) {
            tv_val(y, p)[si] = dx;
            continue;
        }
        if (ax == 0xFFFE) {
            switch (p) {
            case TV_M0: y->S_AVEKM_0[si] = (uint8_t)dx; break;
            case TV_M1: y->S_AVEKM_1[si] = (uint8_t)dx; break;
            case TV_V0: y->S_KSLTL_0[si] = (uint8_t)dx; break;
            case TV_V1: y->S_KSLTL_1[si] = (uint8_t)dx; break;
            case TV_F:
                y->S_BLOCK[si] = (uint8_t)(dx >> 8);
                if (y->S_type[si] == TV_INST) y->S_BLOCK[si] &= 0xE0;
                break;
            case TV_FB: y->S_FBC[si] = (uint8_t)(dx >> 8); break;
            }
            continue;
        }
        tv->cnt[p][si] = ax;
        tv->inc[p][si] = dx;
        return;
    }
    tv->inc[p][si] = 0;
    tv->cnt[p][si] = 0xFFFF;
}

/* TV_phase (SBFM 08C1h; DM03 08CEh): sets slot si up from its TVFX timbre, for the key-on phase or, when
   the slot is in KEYOFF, the release phase's command lists */
static void TV_phase(Yam *y, int si)
{
    Tv *tv = TV(y);
    const uint8_t *t = y->S_timbre[si];
    uint8_t cl = 0x20;
    uint16_t ax = 0xFF0F, dx = 0xFF0F;
    int p;
    y->S_FBC[si] = y->S_KSLTL_0[si] = y->S_KSLTL_1[si] = 0;
    y->S_AVEKM_0[si] = y->S_AVEKM_1[si] = 0x20;
    if (t[3] != 1) cl |= 8;
    y->S_type[si] = t[3];
    y->S_BLOCK[si] = cl;
    if ((uint16_t)(rd16(t + 8) + 2) != 0x36) {     /* the timbre has envelope words */
        ax = rd16(t + 0x36);
        dx = rd16(t + 0x38);
        if (y->S_status[si] == KEYOFF) {
            ax = rd16(t + 0x3A);
            dx = rd16(t + 0x3C);
        }
    }
    y->S_AD_0[si] = (uint8_t)(dx >> 8);
    y->S_SR_0[si] = (uint8_t)dx;
    y->S_AD_1[si] = (uint8_t)(ax >> 8);
    y->S_SR_1[si] = (uint8_t)ax;
    if (y->S_status[si] == KEYOFF) {
        tv->ptr[TV_F][si] = (uint16_t)(rd16(t + 0x0A) + 2);
        tv->ptr[TV_V0][si] = (uint16_t)(rd16(t + 0x10) + 2);
        tv->ptr[TV_V1][si] = (uint16_t)(rd16(t + 0x16) + 2);
        tv->ptr[TV_M0][si] = (uint16_t)(rd16(t + 0x28) + 2);
        tv->ptr[TV_M1][si] = (uint16_t)(rd16(t + 0x2E) + 2);
        tv->ptr[TV_FB][si] = (uint16_t)(rd16(t + 0x22) + 2);
        tv->ptr[TV_WS][si] = (uint16_t)(rd16(t + 0x34) + 2);
        tv->ptr[TV_P][si] = (uint16_t)(rd16(t + 0x1C) + 2);
    } else {
        y->S_duration[si] = t[3] == 1 ? 0xFFFF : (uint16_t)(rd16(t + 4) + 1);
        y->S_f_val[si] = rd16(t + 0x06);
        tv->ptr[TV_F][si] = (uint16_t)(rd16(t + 0x08) + 2);
        y->S_v0_val[si] = rd16(t + 0x0C);
        tv->ptr[TV_V0][si] = (uint16_t)(rd16(t + 0x0E) + 2);
        y->S_v1_val[si] = rd16(t + 0x12);
        tv->ptr[TV_V1][si] = (uint16_t)(rd16(t + 0x14) + 2);
        y->S_m0_val[si] = rd16(t + 0x24);
        tv->ptr[TV_M0][si] = (uint16_t)(rd16(t + 0x26) + 2);
        y->S_m1_val[si] = rd16(t + 0x2A);
        tv->ptr[TV_M1][si] = (uint16_t)(rd16(t + 0x2C) + 2);
        y->S_fb_val[si] = rd16(t + 0x1E);
        tv->ptr[TV_FB][si] = (uint16_t)(rd16(t + 0x20) + 2);
        y->S_ws_val[si] = rd16(t + 0x30);
        tv->ptr[TV_WS][si] = (uint16_t)(rd16(t + 0x32) + 2);
        y->S_p_val[si] = rd16(t + 0x18);
        tv->ptr[TV_P][si] = (uint16_t)(rd16(t + 0x1A) + 2);
    }
    y->S_update[si] = U_ALL_REGS;
    for (p = 0; p < TV_N; p++) {
        tv->cnt[p][si] = 1;
        tv->inc[p][si] = 0;
    }
}

/* one parameter's step in serve_synth: the increment, then the countdown to the next command */
static void tv_step(Yam *y, int si, int p, uint8_t flag)
{
    Tv *tv = TV(y);
    if (tv->inc[p][si]) {
        tv_val(y, p)[si] = (uint16_t)(tv_val(y, p)[si] + tv->inc[p][si]);
        y->S_update[si] |= flag;
    }
    if (--tv->cnt[p][si] == 0) {
        TV_cmd(y, si, p);
        y->S_update[si] |= flag;
    }
}

/* a level's step: in the release phase a level that passes through 0 in the increment's
   direction stops at 0 */
static void tv_level(Yam *y, int si, int p)
{
    Tv *tv = TV(y);
    uint16_t ax = tv->inc[p][si], old, now;
    if (ax) {
        old = tv_val(y, p)[si];
        now = (uint16_t)(old + ax);
        tv_val(y, p)[si] = now;
        if (y->S_status[si] == KEYOFF && ((now ^ old) & 0x8000) && !((now ^ ax) & 0x8000))
            tv_val(y, p)[si] = 0;
        y->S_update[si] |= y->vol_update;
    }
    if (--tv->cnt[p][si] == 0) {
        TV_cmd(y, si, p);
        y->S_update[si] |= U_KSLTL;
    }
}

/* serve_synth (SBFM 063Ah; DM03 0647h), after every XMIDI service: the TVFX parameters at 60 Hz, and the
   voice priorities five times in 120 services */
static void serve_synth(Yam *y)
{
    Tv *tv = TV(y);
    int si;
    y->TV_accum += 60;
    if (y->TV_accum >= 120) {
        y->TV_accum -= 120;
        y->vol_update ^= U_KSLTL;
        for (si = 0; si < y->nslots; si++) {
            if (y->S_status[si] == FREE || y->S_type[si] == BNK_INST) continue;
            tv_step(y, si, TV_F, U_FREQ);
            tv_step(y, si, TV_FB, U_FBC);
            tv_step(y, si, TV_M0, U_AVEKM);
            tv_step(y, si, TV_M1, U_AVEKM);
            tv_level(y, si, TV_V0);
            tv_level(y, si, TV_V1);
            tv_step(y, si, TV_WS, U_WS);
            y->S_p_val[si] = (uint16_t)(y->S_p_val[si] + tv->inc[TV_P][si]);
            if (--tv->cnt[TV_P][si] == 0) TV_cmd(y, si, TV_P);
            if (y->S_update[si] & U_ALL_REGS) yam_update_voice(y, si);
            if (y->S_status[si] != KEYOFF) {
                if (--y->S_duration[si] == 0) {
                    y->S_status[si] = KEYOFF;
                    TV_phase(y, si);
                }
            } else if (y->S_v0_val[si] < 0x400 && y->S_v1_val[si] < 0x400) {
                yam_release_voice(y, si);
                y->S_status[si] = FREE;
                TV_switch_voice(y);
            }
        }
    }
    y->pri_accum += 5;
    if (y->pri_accum >= 120) {
        y->pri_accum -= 120;
        yam_update_priority(y);
    }
}

const AilFmExt uw1_tvfx = {
    "ALE.INC (SBFM.ADV)",
    sizeof(Tv),
    TV_phase,
    TV_switch_voice,
    serve_synth,
};
