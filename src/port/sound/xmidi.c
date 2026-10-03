/* xmidi.c: replaces the XMIDI shell of the DMnn.ADV music drivers (docs/PORT.md, "Sound"): the
   Extended MIDI sequencer of John Miles' public-domain XMIDI.ASM (AIL 2.14), in the revision
   UW2's drivers were built from. Assembling 2.14's XMIDI.ASM with TASM and comparing it with
   DM03.ADV shows UW2's is the one before 1.10 ("new time signature function for improved
   precision", 14-Oct-92): rewind_seq starts the bar count at 0 and the beat fraction at one
   interval's worth, a time signature resets only the fractions, the beat and bar queries
   return the counters as they are (no QUANT_ADVANCE), and serve_driver advances the beat
   before it plays the interval's notes. Everything else follows XMIDI.ASM, routine by
   routine, with its names; the synthesiser below it (yamaha.c, mt32.c) is reached through
   struct Synth, as XMIDI.ASM included YAMAHA.INC or MT32.INC.

   The sequence state lives here, not in the state table the game passes (the game only
   allocates it, AIL_state_table_size bytes, and never reads it); the pointers into the
   XMIDI image are host pointers into the game's buffer. Sequence handles are the byte offsets
   XMIDI.ASM uses (0, 4, 8, ...), since the game keeps them. */
#include <stdlib.h>
#include <string.h>
#include "aildrv.h"

#define QUANT_RATE      120
#define QUANT_TIME      8333
#define QUANT_TIME_16   0x208D5L        /* 16,000,000 / QUANT_RATE */
#define MAX_NOTES       32
#define FOR_NEST        4
#define NSEQS           8
#define DEF_PITCH_L     0x00
#define DEF_PITCH_H     0x40
#define NUM_CONTROLS    9
#define STATE_SIZE      0x208           /* SIZE state_table, read from every DMnn.ADV */

enum { C_PV, C_MODUL, C_PAN, C_EXP, C_SUS, C_PBS, C_LOCK, C_PROT, C_VPROT };
static const uint8_t logged_ctrls[NUM_CONTROLS] = {
    PART_VOLUME, MODULATION, PANPOT, EXPRESSION, SUSTAIN, PATCH_BANK_SEL,
    CHAN_LOCK, CHAN_PROTECT, VOICE_PROTECT
};
static const uint8_t ctrl_default[NUM_CONTROLS] = { 127, 0, 64, 127, 0, 0, 0, 0, 0 };
static const int8_t prg_default[9] = { 68, 48, 95, 78, 41, 3, 110, 122, -1 };

typedef struct Seq {
    int used;                           /* sequence_state[] not null */
    uint8_t *TIMB, *RBRN, *EVNT, *EVNT_ptr;
    int16_t cur_callback;
    uint8_t *ctrl_ptr;
    int seq_handle, seq_started, status, post_release;
    int16_t interval_cnt, note_count;
    int16_t vol_error, vol_percent, vol_target;
    int32_t vol_accum, vol_period;
    int16_t tempo_error, tempo_percent, tempo_target;
    int32_t tempo_accum, tempo_period;
    int16_t beat_count, measure_count, time_numerator;
    int32_t time_fraction, beat_fraction, time_per_beat;
    uint8_t *FOR_ptrs[FOR_NEST];
    int16_t FOR_loop_cnt[FOR_NEST];
    uint8_t chan_map[NUM_CHANS], chan_program[NUM_CHANS], chan_pitch_l[NUM_CHANS],
            chan_pitch_h[NUM_CHANS], chan_indirect[NUM_CHANS];
    uint8_t chan_controls[NUM_CONTROLS * NUM_CHANS];    /* ctrl_log: PV, MODUL, ... by channel */
    uint8_t note_chan[MAX_NOTES], note_num[MAX_NOTES];
    int32_t note_time[MAX_NOTES];
} Seq;

typedef struct Xmidi {
    Synth *s;
    Seq seq[NSEQS];
    int sequence_count, current_handle, service_active;
    uint8_t global_controls[NUM_CONTROLS * NUM_CHANS];
    uint8_t global_program[NUM_CHANS], global_pitch_l[NUM_CHANS], global_pitch_h[NUM_CHANS];
    uint8_t active_notes[NUM_CHANS];
    uint8_t lock_status[NUM_CHANS];     /* bit 7 locked, bit 6 lock-protected */
    uint8_t ctrl_hash[256];
    int init_OK;
} Xmidi;

static void send(Xmidi *x, unsigned stat, unsigned d1, unsigned d2)
{
    x->s->send(x->s, stat & 0xFF, d1 & 0xFF, d2 & 0xFF);
}

static Seq *seq_of(Xmidi *x, int h)
{
    if (h < 0 || (h & 3) || h / 4 >= NSEQS) return 0;
    return &x->seq[h / 4];
}

static uint32_t be32(const uint8_t *p) { return (uint32_t)p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3]; }
static int tag(const uint8_t *p, const char *t) { return memcmp(p, t, 4) == 0; }

/* the variable-length number at *p, which is advanced past it */
static uint32_t vln(const uint8_t **p)
{
    uint32_t v = 0;
    uint8_t c;
    do {
        c = *(*p)++;
        v = v << 7 | (c & 0x7F);
    } while (c & 0x80);
    return v;
}

Xmidi *xmidi_new(Synth *s)
{
    Xmidi *x = calloc(1, sizeof *x);
    x->s = s;
    return x;
}

unsigned xmidi_state_size(Xmidi *x)
{
    (void)x;
    return STATE_SIZE;
}

/* find_seq: the SeqNum'th FORM XMID of an IFF CAT or FORM XMID image, or 0 */
static uint8_t *find_seq(uint8_t *si, int seqnum)
{
    int cx = seqnum + 1;
    int32_t end;
    for (;;) {
        if (!tag(si, "CAT ") && !tag(si, "FORM")) return 0;
        if (tag(si + 8, "XMID")) break;
        si += be32(si + 4) + 8;
    }
    end = (int32_t)be32(si + 4) - 5;
    if (tag(si, "FORM")) return cx == 1 ? si : 0;
    si += 12;
    for (;;) {
        if (tag(si + 8, "XMID") && --cx == 0) return si;
        end -= (int32_t)(be32(si + 4) + 8);
        if (end < 0) return 0;
        si += be32(si + 4) + 8;
    }
}

static void rewind_seq(Xmidi *x, Seq *q)
{
    int i;
    for (i = 0; i < FOR_NEST; i++) q->FOR_loop_cnt[i] = -1;
    for (i = NUM_CHANS - 1; i >= 0; i--) {
        q->chan_map[i] = (uint8_t)i;
        q->chan_program[i] = 0xFF;
        q->chan_pitch_l[i] = 0xFF;
        q->chan_pitch_h[i] = 0xFF;
        q->chan_indirect[i] = 0xFF;
    }
    memset(q->chan_controls, 0xFF, sizeof q->chan_controls);
    memset(q->note_chan, 0xFF, sizeof q->note_chan);
    q->cur_callback = -1;
    q->interval_cnt = 0;
    q->note_count = 0;
    q->vol_percent = q->vol_target = (int16_t)x->s->def_synth_vol;
    q->tempo_percent = q->tempo_target = 100;
    q->tempo_error = 0;
    q->beat_count = 0;
    q->measure_count = 0;
    q->beat_fraction = QUANT_TIME_16;   /* 1.07: "beat fraction initialized nonzero" */
    q->time_numerator = 4;              /* 4/4 */
    q->time_fraction = QUANT_TIME_16;
    q->time_per_beat = 0x7A1200;        /* 500000 us a beat * 16, 120 beats a minute */
}

static void flush_channel_notes(Xmidi *x, int chan)
{
    int h, b;
    for (h = 0; h < NSEQS; h++) {
        Seq *q = &x->seq[h];
        if (!q->used || !q->note_count) continue;
        for (b = 0; b < MAX_NOTES; b++) {
            if (q->note_chan[b] != chan) continue;
            q->note_chan[b] = 0xFF;
            x->active_notes[q->chan_map[chan]]--;
            send(x, 0x80 | q->chan_map[chan], q->note_num[b], 0);
            q->note_count--;
        }
    }
}

static void flush_note_queue(Xmidi *x, Seq *q)
{
    int b, c;
    for (b = 0; b < MAX_NOTES; b++) {
        if (q->note_chan[b] == 0xFF) continue;
        c = q->note_chan[b];
        q->note_chan[b] = 0xFF;
        x->active_notes[q->chan_map[c]]--;
        send(x, 0x80 | q->chan_map[c], q->note_num[b], 0);
    }
    q->note_count = 0;
}

unsigned xmidi_lock_channel(Xmidi *x)
{
    int cl = 0xFF, si = -1, di;
    unsigned mask = 0xC0;           /* skip locked and protected channels */
    for (;;) {
        for (di = x->s->max_true_chan - 1; di >= x->s->min_true_chan - 1; di--) {
            if (x->lock_status[di] & mask) continue;
            if (x->active_notes[di] >= cl) continue;     /* jae: higher channels win ties */
            cl = x->active_notes[di];
            si = di;
        }
        if (si != -1) break;
        if (mask == 0x80) return 0;
        mask = 0x80;                /* none: ignore lock protection and try again */
    }
    send(x, 0xB0 | si, SUSTAIN, 0);
    flush_channel_notes(x, si);
    x->active_notes[si] = 0;
    x->lock_status[si] |= 0x80;
    return (unsigned)si + 1;
}

void xmidi_release_channel(Xmidi *x, int chan)
{
    int si = chan - 1, c, i;
    if (si < 0 || si >= NUM_CHANS || !(x->lock_status[si] & 0x80)) return;
    x->lock_status[si] &= 0x7F;
    x->active_notes[si] = 0;
    send(x, 0xB0 | si, SUSTAIN, 0);
    send(x, 0xB0 | si, ALL_NOTES_OFF, 0);
    for (i = 0; i < NUM_CONTROLS; i++) {
        c = x->global_controls[i * NUM_CHANS + si];
        if (c != 0xFF) send(x, 0xB0 | si, logged_ctrls[i], c);
    }
    if (x->global_program[si] != 0xFF) send(x, 0xC0 | si, x->global_program[si], 0);
    if (x->global_pitch_l[si] != 0xFF && x->global_pitch_h[si] != 0xFF)
        send(x, 0xE0 | si, x->global_pitch_l[si], x->global_pitch_h[si]);
}

static void reset_sequence(Xmidi *x, Seq *q)
{
    int di;
    for (di = 0; di < NUM_CHANS; di++) {
        if ((int8_t)q->chan_controls[C_SUS * NUM_CHANS + di] >= 64) {
            x->global_controls[C_SUS * NUM_CHANS + di] = 0;
            send(x, 0xB0 | di, SUSTAIN, 0);
        }
        if ((int8_t)q->chan_controls[C_LOCK * NUM_CHANS + di] >= 64) {
            flush_channel_notes(x, di);
            xmidi_release_channel(x, q->chan_map[di] + 1);
            q->chan_map[di] = (uint8_t)di;
        }
        if ((int8_t)q->chan_controls[C_PROT * NUM_CHANS + di] >= 64)
            x->lock_status[di] &= 0xBF;
        if ((int8_t)q->chan_controls[C_VPROT * NUM_CHANS + di] >= 64)
            send(x, 0xB0 | di, VOICE_PROTECT, 0);
    }
}

static int XMIDI_control(Xmidi *x, Seq *q, int chan, int con, int val);

static void restore_sequence(Xmidi *x, Seq *q)
{
    int di, con, ctrl, index;
    unsigned a;
    for (di = 0; di < NUM_CHANS; di++) {
        uint8_t c = q->chan_controls[C_LOCK * NUM_CHANS + di];
        if (c == 0xFF || (int8_t)c < 64) continue;
        a = xmidi_lock_channel(x) - 1;
        if (a == 0xFFFF || a == (unsigned)-1) a = (unsigned)di;
        q->chan_map[di] = (uint8_t)a;
    }
    for (con = 0; con < NUM_CONTROLS; con++) {
        ctrl = logged_ctrls[con];
        if (ctrl == CHAN_LOCK) continue;
        index = x->ctrl_hash[ctrl];
        for (di = 0; di < NUM_CHANS; di++) {
            uint8_t v = q->chan_controls[index + di];
            if (v != 0xFF) XMIDI_control(x, q, di, ctrl, v);
        }
    }
    for (di = 0; di < NUM_CHANS; di++) {
        if (q->chan_pitch_l[di] != 0xFF && q->chan_pitch_h[di] != 0xFF)
            send(x, 0xE0 | q->chan_map[di], q->chan_pitch_l[di], q->chan_pitch_h[di]);
        if (q->chan_program[di] != 0xFF)
            send(x, 0xC0 | q->chan_map[di], q->chan_program[di], 0);
    }
}

static void XMIDI_volume(Xmidi *x, Seq *q)
{
    int b;
    unsigned v;
    for (b = 0; b < NUM_CHANS; b++) {
        uint8_t pv = q->chan_controls[C_PV * NUM_CHANS + b];
        if (pv == 0xFF) continue;
        v = (unsigned)pv * (uint16_t)q->vol_percent / 100;
        if (v >= 127) v = 127;
        x->global_controls[C_PV * NUM_CHANS + b] = (uint8_t)v;
        if (x->lock_status[b] & 0x80) continue;
        send(x, 0xB0 | q->chan_map[b], PART_VOLUME, v);
    }
}

/* XMIDI_control: a Control Change of the sequence; returns the event's size, 3 */
static int XMIDI_control(Xmidi *x, Seq *q, int chan, int con, int val)
{
    int i, h;
    unsigned dx = (unsigned)val & 0xFF;
    uint8_t ind = q->chan_indirect[chan];
    if (ind != 0xFF) {
        q->chan_indirect[chan] = 0xFF;
        if (q->ctrl_ptr) dx = q->ctrl_ptr[ind];
    }
    h = x->ctrl_hash[con & 0xFF];
    if (h != 0xFF) {
        x->global_controls[h + chan] = (uint8_t)dx;
        q->chan_controls[h + chan] = (uint8_t)dx;
    }
    switch (con) {
    case PART_VOLUME:
        if (q->vol_percent != 100) {
            dx = dx * (uint16_t)q->vol_percent / 100;
            if (dx >= 127) dx = 127;
            x->global_controls[C_PV * NUM_CHANS + chan] = (uint8_t)dx;
        }
        break;
    case CLEAR_BEAT_BAR:
        q->beat_count = 0;
        q->measure_count = 0;
        q->beat_fraction = q->time_fraction;    /* the old way: one interval's worth */
        return 3;
    case CALLBACK_TRIG:
        q->cur_callback = (int16_t)dx;          /* UW2 installs no callback function */
        return 3;
    case FOR_LOOP:
        for (i = 0; i < FOR_NEST; i++)
            if (q->FOR_loop_cnt[i] == -1) {
                q->FOR_loop_cnt[i] = (int16_t)dx;
                q->FOR_ptrs[i] = q->EVNT_ptr;   /* NEXT goes back to after the FOR */
                break;
            }
        return 3;
    case NEXT_LOOP:
        if ((int8_t)dx < 64) return 3;          /* a BREAK */
        for (i = FOR_NEST - 1; i >= 0; i--)
            if (q->FOR_loop_cnt[i] != -1) break;
        if (i < 0) return 3;
        if (q->FOR_loop_cnt[i] != 0 && --q->FOR_loop_cnt[i] == 0) {
            q->FOR_loop_cnt[i] = -1;
            return 3;
        }
        q->EVNT_ptr = q->FOR_ptrs[i];
        return 3;
    case CHAN_PROTECT:
        x->lock_status[chan] |= 0x40;
        if ((int8_t)dx < 64) x->lock_status[chan] &= 0xBF;
        return 3;
    case CHAN_LOCK:
        if ((int8_t)dx >= 64) {
            unsigned a = xmidi_lock_channel(x) - 1;
            if (a == (unsigned)-1) a = (unsigned)chan;
            q->chan_map[chan] = (uint8_t)a;
        } else {
            flush_channel_notes(x, chan);
            xmidi_release_channel(x, q->chan_map[chan] + 1);
            q->chan_map[chan] = (uint8_t)chan;
        }
        return 3;
    case INDIRECT_C_PFX:
        q->chan_indirect[chan] = (uint8_t)dx;
        return 3;
    }
    if (!(x->lock_status[chan] & 0x80))
        send(x, 0xB0 | q->chan_map[chan], (unsigned)con, dx);
    return 3;
}

/* XMIDI_note_on: the note goes into the note queue with its duration; returns the event's size */
static int XMIDI_note_on(Xmidi *x, Seq *q)
{
    const uint8_t *e = q->EVNT_ptr, *p = e + 3;
    int chan = e[0] & 0x0F, note = e[1], vel = e[2], b;
    int32_t dur = (int32_t)vln(&p);
    int len = (int)(p - e);
    if (x->lock_status[chan] & 0x80) return len;
    for (b = 0; b < MAX_NOTES && q->note_chan[b] != 0xFF; b++)
        ;
    if (b == MAX_NOTES) b = 0;              /* overwrite entry 0 if the queue is full */
    else q->note_count++;
    q->note_chan[b] = (uint8_t)chan;
    q->note_num[b] = (uint8_t)note;
    q->note_time[b] = dur - 1;              /* the queue watches for negative durations */
    x->active_notes[q->chan_map[chan]]++;
    send(x, 0x90 | q->chan_map[chan], note, vel);
    return len;
}

static void release_seq_h(Xmidi *x, int h);

static int XMIDI_meta(Xmidi *x, Seq *q)
{
    const uint8_t *e = q->EVNT_ptr, *p = e + 2;
    int type = e[1];
    uint32_t len = vln(&p);
    int size = (int)(len + (uint32_t)(p - e));
    int cl, i;
    int32_t f;
    switch (type) {
    case 0x2F:                              /* end of track */
        reset_sequence(x, q);
        q->status = SEQ_DONE;
        if (q->post_release) release_seq_h(x, x->current_handle);
        break;
    case 0x58:                              /* time signature */
        q->time_numerator = p[0];
        cl = p[1] - 2;
        if (cl < 0) {
            f = QUANT_TIME_16;
            for (i = 0; i < -cl; i++) f = (int32_t)((uint32_t)f >> 1);
        } else {
            f = 0;
            for (i = 0; i < (1 << cl); i++) f += QUANT_TIME_16;
        }
        q->time_fraction = f;
        q->beat_fraction = q->time_fraction;
        break;
    case 0x51:                              /* tempo, microseconds a beat, times 16 */
        q->time_per_beat = (int32_t)(((uint32_t)p[0] << 16 | (uint32_t)p[1] << 8 | p[2]) << 4);
        break;
    }
    return size;
}

static int XMIDI_sysex(Xmidi *x, Seq *q)
{
    const uint8_t *e = q->EVNT_ptr, *p = e + 1;
    uint32_t len = vln(&p);
    if (x->s->sysex) x->s->sysex(x->s, p, e[0], (unsigned)len);
    return (int)(len + (uint32_t)(p - e));
}

/* serve_driver's comparison of the beat fraction with the time per beat: the high words
   signed, and on a tie the low words signed too (jge), as XMIDI.ASM compares them */
static int beat_due(int32_t f, int32_t tpb)
{
    int16_t fh = (int16_t)(f >> 16), th = (int16_t)(tpb >> 16);
    if (fh != th) return fh > th;
    return (int16_t)f >= (int16_t)tpb;
}

/* ---- the driver's entry points ----------------------------------------------------- */

void xmidi_serve(Xmidi *x)
{
    int seqcnt, b, ax, cx, st, chan, d1, d2, n;
    Seq *q;
    if (x->service_active) return;
    x->service_active++;
    x->current_handle = -4;
    seqcnt = x->sequence_count;
    if (seqcnt == 0) goto end_seqs;
    for (;;) {
        do {
            x->current_handle += 4;
            q = &x->seq[x->current_handle / 4];
        } while (!q->used);
        if (q->status != SEQ_PLAYING) goto next_seq;
        q->tempo_error = (int16_t)(q->tempo_error + q->tempo_percent);
        ax = q->tempo_error - 100;
        if (ax < 0) goto chk_t_grad;
        for (;;) {
            q->tempo_error = (int16_t)ax;
            /* the beat, before the interval (1.09's order) */
            q->beat_fraction += q->time_fraction;
            if (beat_due(q->beat_fraction, q->time_per_beat)) {
                q->beat_fraction -= q->time_per_beat;
                if ((uint16_t)++q->beat_count >= (uint16_t)q->time_numerator) {
                    q->beat_count = 0;
                    q->measure_count++;
                }
            }
            if (q->note_count) {            /* turn off any expired notes */
                for (b = 0; b < MAX_NOTES && q->note_count; b++) {
                    if (q->note_chan[b] == 0xFF) continue;
                    if (--q->note_time[b] >= 0) continue;
                    chan = q->note_chan[b];
                    q->note_chan[b] = 0xFF;
                    x->active_notes[q->chan_map[chan]]--;
                    send(x, 0x80 | q->chan_map[chan], q->note_num[b], 0);
                    q->note_count--;
                }
            }
            if (--q->interval_cnt <= 0) {   /* the next interval is due: play it */
                for (;;) {
                    const uint8_t *e = q->EVNT_ptr;
                    st = e[0];
                    if (st < 0x80) {        /* an interval count */
                        q->EVNT_ptr++;
                        q->interval_cnt = (int16_t)st;
                        break;
                    }
                    chan = st & 0x0F;
                    d1 = e[1];
                    d2 = e[2];
                    st &= 0xF0;
                    if (st >= 0xF0) {
                        n = chan == 0x0F ? XMIDI_meta(x, q) : XMIDI_sysex(x, q);
                        goto end_event;
                    }
                    if (st >= 0xE0) {
                        q->chan_pitch_l[chan] = (uint8_t)d1;
                        q->chan_pitch_h[chan] = (uint8_t)d2;
                        x->global_pitch_l[chan] = (uint8_t)d1;
                        x->global_pitch_h[chan] = (uint8_t)d2;
                        n = 3;
                    } else if (st >= 0xD0) {
                        n = 2;
                    } else if (st >= 0xC0) {
                        q->chan_program[chan] = (uint8_t)d1;
                        x->global_program[chan] = (uint8_t)d1;
                        n = 2;
                    } else if (st >= 0xB0) {
                        XMIDI_control(x, q, chan, d1, d2);
                        n = 3;
                        goto end_event;
                    } else if (st >= 0xA0) {
                        n = 3;
                    } else {
                        n = XMIDI_note_on(x, q);
                        goto end_event;
                    }
                    if (!(x->lock_status[chan] & 0x80))
                        send(x, (unsigned)st | q->chan_map[chan], d1, d2);
end_event:
                    q->EVNT_ptr += n;
                    if (q->status != SEQ_PLAYING) goto next_seq;
                }
            }
            ax = q->tempo_error - 100;
            if (ax < 0) break;
        }
chk_t_grad:
        if (q->tempo_percent != q->tempo_target) {
            int up = q->tempo_percent < q->tempo_target;    /* the flags pushed before */
            int32_t a = q->tempo_accum + QUANT_TIME / 100;
            cx = -1;
            do {
                cx++;
                q->tempo_accum = a;
                a -= q->tempo_period;
            } while (a >= 0);
            if (cx) {
                int t = q->tempo_percent;
                if (up) { t += cx; if (t > q->tempo_target) t = q->tempo_target; }
                else { t -= cx; if (t < q->tempo_target) t = q->tempo_target; }
                q->tempo_percent = (int16_t)t;
            }
        }
        if (q->vol_percent != q->vol_target) {
            int up = q->vol_percent < q->vol_target;
            int32_t a = q->vol_accum + QUANT_TIME / 100;
            cx = -1;
            do {
                cx++;
                q->vol_accum = a;
                a -= q->vol_period;
            } while (a >= 0);
            if (cx) {
                int v = q->vol_percent;
                if (up) { v += cx; if (v > q->vol_target) v = q->vol_target; }
                else { v -= cx; if (v < q->vol_target) v = q->vol_target; }
                q->vol_percent = (int16_t)v;
                XMIDI_volume(x, q);
            }
        }
next_seq:
        if (--seqcnt == 0) break;
    }
end_seqs:
    if (x->s->serve) x->s->serve(x->s);
    x->service_active--;
}

void xmidi_init_driver(Xmidi *x)
{
    int i, di, si, c, h;
    Synth *s = x->s;
    x->service_active = 0;
    x->sequence_count = 0;
    memset(x->global_controls, 0xFF, sizeof x->global_controls);
    memset(x->global_program, 0xFF, sizeof x->global_program);
    memset(x->global_pitch_l, 0xFF, sizeof x->global_pitch_l);
    memset(x->global_pitch_h, 0xFF, sizeof x->global_pitch_h);
    memset(x->ctrl_hash, 0xFF, sizeof x->ctrl_hash);
    for (i = 0; i < NSEQS; i++) x->seq[i].used = 0;
    memset(x->lock_status, 0, sizeof x->lock_status);
    memset(x->active_notes, 0, sizeof x->active_notes);
    for (i = 0; i < NUM_CONTROLS; i++) x->ctrl_hash[logged_ctrls[i]] = (uint8_t)(i * NUM_CHANS);
    s->reset(s);
    s->init(s);
    for (si = 0; si < NUM_CONTROLS; si++) {
        c = ctrl_default[si];
        h = x->ctrl_hash[logged_ctrls[si]];
        for (di = s->min_true_chan - 1; di <= s->max_rec_chan - 1; di++) {
            x->global_controls[h + di] = (uint8_t)c;
            send(x, 0xB0 | di, logged_ctrls[si], c);
        }
    }
    for (di = s->min_true_chan - 1; di <= s->max_rec_chan - 1; di++) {
        x->global_pitch_l[di] = DEF_PITCH_L;
        x->global_pitch_h[di] = DEF_PITCH_H;
        send(x, 0xE0 | di, DEF_PITCH_L, DEF_PITCH_H);
        c = prg_default[di - (s->min_true_chan - 1)];
        if (c == -1) continue;
        x->global_program[di] = (uint8_t)c;
        send(x, 0xC0 | di, (unsigned)c, 0);
    }
    x->init_OK = 1;
}

static void stop_seq_h(Xmidi *x, int h);

void xmidi_shutdown_driver(Xmidi *x)
{
    int i;
    if (!x->init_OK) return;
    for (i = 0; i < NSEQS; i++)
        if (x->seq[i].used) {
            stop_seq_h(x, i * 4);
            release_seq_h(x, i * 4);
        }
    x->s->reset(x->s);
    if (x->s->shutdown) x->s->shutdown(x->s);
    x->init_OK = 0;
}

int xmidi_register_seq(Xmidi *x, uint8_t *xmid, int num, void *state, uint8_t *ctrl)
{
    int i;
    uint8_t *di;
    Seq *q;
    (void)state;
    for (i = 0; i < NSEQS && x->seq[i].used; i++)
        ;
    if (i == NSEQS) return -1;
    di = find_seq(xmid, num);
    if (!di) return -1;
    q = &x->seq[i];
    memset(q, 0, sizeof *q);
    q->used = 1;
    di += 12;                               /* past FORM <len> XMID */
    for (;;) {
        uint32_t len = be32(di + 4) + 8;
        if (tag(di, "TIMB")) q->TIMB = di;
        else if (tag(di, "RBRN")) q->RBRN = di;
        else if (tag(di, "EVNT")) break;
        di += len;
    }
    q->seq_handle = i * 4;
    q->EVNT = di;
    q->ctrl_ptr = ctrl;
    q->post_release = 0;
    q->seq_started = 0;
    q->status = SEQ_STOPPED;
    x->sequence_count++;
    rewind_seq(x, q);
    return i * 4;
}

static void release_seq_h(Xmidi *x, int h)
{
    Seq *q = seq_of(x, h);
    if (!q || !q->used) return;
    if (q->status == SEQ_PLAYING) {
        q->post_release = 1;
        return;
    }
    q->used = 0;
    x->sequence_count--;
}

void xmidi_release_seq(Xmidi *x, int h)
{
    release_seq_h(x, h);
}

static void stop_seq_h(Xmidi *x, int h)
{
    Seq *q = seq_of(x, h);
    if (!q || !q->used || q->status != SEQ_PLAYING) return;
    flush_note_queue(x, q);
    reset_sequence(x, q);
    q->status = SEQ_STOPPED;
}

void xmidi_stop_seq(Xmidi *x, int h)
{
    stop_seq_h(x, h);
}

void xmidi_start_seq(Xmidi *x, int h)
{
    Seq *q = seq_of(x, h);
    if (!q || !q->used) return;             /* (XMIDI.ASM does not check registration) */
    if (q->status == SEQ_PLAYING) stop_seq_h(x, h);
    rewind_seq(x, q);
    q->EVNT_ptr = q->EVNT + 8;
    q->status = SEQ_PLAYING;
    q->seq_started = 1;
}

void xmidi_resume_seq(Xmidi *x, int h)
{
    Seq *q = seq_of(x, h);
    if (!q || !q->used || q->status != SEQ_STOPPED || !q->seq_started) return;
    restore_sequence(x, q);
    q->status = SEQ_PLAYING;
}

/* get_seq_status: XMIDI.ASM reads the state table of any handle but -1, registered or not */
unsigned xmidi_seq_status(Xmidi *x, int h)
{
    Seq *q = seq_of(x, h);
    if (h == -1) return 0xFFFF;
    return q ? (unsigned)q->status : 0;
}

int xmidi_beat_count(Xmidi *x, int h)
{
    Seq *q = seq_of(x, h);
    return q ? q->beat_count : -1;
}

int xmidi_bar_count(Xmidi *x, int h)
{
    Seq *q = seq_of(x, h);
    return q ? q->measure_count : -1;
}

void xmidi_map_seq_channel(Xmidi *x, int h, int seqchan, int physchan)
{
    Seq *q = seq_of(x, h);
    if (q) q->chan_map[(seqchan - 1) & 15] = (uint8_t)(physchan - 1);
}

int xmidi_true_seq_channel(Xmidi *x, int h, int seqchan)
{
    Seq *q = seq_of(x, h);
    return q ? q->chan_map[(seqchan - 1) & 15] + 1 : -1;
}

void xmidi_branch_index(Xmidi *x, int h, int marker)
{
    Seq *q = seq_of(x, h);
    uint8_t *di;
    unsigned cx, i;
    if (!q || !q->RBRN || !tag(q->RBRN, "RBRN")) return;
    cx = q->RBRN[8] | q->RBRN[9] << 8;
    for (di = q->RBRN + 10; cx; cx--, di += 6)
        if (di[0] == (uint8_t)marker) {
            uint32_t off = (uint32_t)(di[2] | di[3] << 8) | (uint32_t)(di[4] | di[5] << 8) << 16;
            q->EVNT_ptr = q->EVNT + off + 8;
            q->interval_cnt = 0;
            flush_note_queue(x, q);
            for (i = 0; i < FOR_NEST; i++) q->FOR_loop_cnt[i] = -1;
            return;
        }
}

unsigned xmidi_rel_tempo(Xmidi *x, int h)
{
    Seq *q = seq_of(x, h);
    return q ? (uint16_t)q->tempo_percent : 0xFFFF;
}

unsigned xmidi_rel_volume(Xmidi *x, int h)
{
    Seq *q = seq_of(x, h);
    return q ? (uint16_t)q->vol_percent : 0xFFFF;
}

/* the period of a gradual change: Grad ms in 100 us periods per step */
static int32_t grad_period(unsigned grad, int delta)
{
    uint32_t n = 10UL * (uint16_t)grad, d = (uint16_t)(delta < 0 ? -delta : delta);
    uint32_t r = n / d;
    return r ? (int32_t)r : 1;
}

void xmidi_set_rel_tempo(Xmidi *x, int h, unsigned tempo, unsigned ms)
{
    Seq *q = seq_of(x, h);
    int delta;
    if (!q) return;
    q->tempo_target = (int16_t)tempo;
    if (ms == 0) {
        q->tempo_percent = (int16_t)tempo;
        return;
    }
    delta = (int16_t)(q->tempo_target - q->tempo_percent);
    if (!delta) return;
    q->tempo_period = grad_period(ms, delta);
    q->tempo_accum = 0;
}

void xmidi_set_rel_volume(Xmidi *x, int h, unsigned vol, unsigned ms)
{
    Seq *q = seq_of(x, h);
    int delta;
    if (!q) return;
    q->vol_target = (int16_t)vol;
    if (ms == 0) {
        q->vol_percent = (int16_t)vol;
        XMIDI_volume(x, q);
        return;
    }
    delta = (int16_t)(q->vol_target - q->vol_percent);
    if (!delta) return;
    q->vol_period = grad_period(ms, delta);
    q->vol_accum = 0;
}

int xmidi_control_val(Xmidi *x, int h, int chan, int ctrl)
{
    Seq *q = seq_of(x, h);
    int i;
    if (!q) return -1;
    if (ctrl == CALLBACK_TRIG) return q->cur_callback;
    i = x->ctrl_hash[ctrl & 0xFF];
    if (i == 0xFF) return -1;
    return (int8_t)q->chan_controls[(i + chan - 1) & 0xFF];
}

void xmidi_set_control_val(Xmidi *x, int h, int chan, int ctrl, int val)
{
    Seq *q = seq_of(x, h);
    if (q) XMIDI_control(x, q, (chan - 1) & 15, ctrl, val);
}

unsigned xmidi_chan_notes(Xmidi *x, int h, int chan)
{
    Seq *q = seq_of(x, h);
    unsigned n = 0;
    int b;
    if (!q) return 0xFFFF;
    for (b = 0; b < MAX_NOTES; b++)
        if (q->note_chan[b] == (uint8_t)(chan - 1)) n++;
    return n;
}

void xmidi_send_cv(Xmidi *x, unsigned stat, unsigned d1, unsigned d2)
{
    send(x, stat, d1, d2);
}

/* get_request (in YAMAHA.INC and MT32.INC): the first timbre the sequence's TIMB chunk lists
   that is not in the cache, AL its number and AH its bank, or FFFFh */
unsigned xmidi_timbre_request(Xmidi *x, int h)
{
    Seq *q = seq_of(x, h);
    const uint8_t *si;
    unsigned n, g;
    if (!q || !q->TIMB || !tag(q->TIMB, "TIMB")) return 0xFFFF;
    si = q->TIMB + 8;
    n = si[0] | si[1] << 8;
    while (n--) {
        si += 2;
        g = si[0] | si[1] << 8;
        if (x->s->request_filter && !x->s->request_filter(x->s, g)) continue;
        if (x->s->index_timbre(x->s, g) == -1) return g;
    }
    return 0xFFFF;
}
