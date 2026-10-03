/* mt32.c: replaces the synthesiser half of DM05.ADV, the Roland MT-32 driver (docs/PORT.md,
   "Sound"): John Miles' public-domain MT32.INC and MPU401.INC (AIL 2.14), which DM05 has
   unchanged (its XMIDI shell is the older one xmidi.c translates). Everything the driver sent
   to the MPU-401's data port in UART mode goes to hw_mpu_byte, which feeds the MT-32 backend
   (libmt32emu, in audio.c): channel messages, the system exclusive messages that load UW.MT's
   timbres into the MT-32's timbre memory and point its patches at them, the reverb and the
   partial reserve. The sysex_wait delays (vertical retraces, about 14 ms each) are not
   waited: the emulated MT-32 needs no time between messages. */
#include <stdlib.h>
#include <string.h>
#include "aildrv.h"

#define SYSEX_SIZE      32
#define SYSEX_Q_CNT     3
#define NUM_TIMBS       64
#define SYSEX_BLK_SIZE  (FINAL_BYTE_1 - START_MSB_1 + 1)
#define SYSEX_RANGE_BEG START_MSB_1
#define SYSEX_RANGE_END (SYSEX_RANGE_BEG + SYSEX_BLK_SIZE * SYSEX_Q_CNT - 1)

static const uint8_t init_reverb[3] = { 0, 3, 2 };
static const uint8_t part_chans[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
static const uint8_t part_rsv[9] = { 3, 4, 3, 4, 3, 4, 3, 4, 4 };

typedef struct Mt {
    Synth s;
    uint8_t patch_bank[128];
    uint8_t sysex_queues[SYSEX_SIZE * SYSEX_Q_CNT];
    uint8_t M_ad[SYSEX_Q_CNT], K_ad[SYSEX_Q_CNT], L_ad[SYSEX_Q_CNT], queue_ptrs[SYSEX_Q_CNT];
    uint32_t note_event;
    uint32_t timb_hist[NUM_TIMBS];
    uint8_t timb_bank[NUM_TIMBS], timb_num[NUM_TIMBS], timb_attribs[NUM_TIMBS];
    uint8_t chan_timbs[NUM_CHANS], MIDI_bank[NUM_CHANS], MIDI_program[NUM_CHANS];
} Mt;

static void send_byte(unsigned b) { hw_mpu_byte(b & 0xFF); }

/* send_sysex_msg: a Roland DT1 message to address a, b, c with its checksum */
static void send_sysex_msg(unsigned a, unsigned b, unsigned c, const uint8_t *data, unsigned size)
{
    unsigned sum = (a & 0xFF) + (b & 0xFF) + (c & 0xFF);
    send_byte(0xF0);
    send_byte(0x41);
    send_byte(0x10);
    send_byte(0x16);
    send_byte(0x12);
    send_byte(a);
    send_byte(b);
    send_byte(c);
    while (size--) {
        sum += *data;
        send_byte(*data++);
    }
    send_byte((0x80 - (sum & 0x7F)) & 0x7F);
    send_byte(0xF7);
}

/* add_sysex_addr: n added to a 21-bit address of 7-bit bytes; returns MSB << 16 | KSB << 8 | LSB */
static uint32_t add_sysex_addr(unsigned n, unsigned msb, unsigned ksb, unsigned lsb)
{
    unsigned l = (lsb & 0xFF) + n, k = ksb & 0xFF, m = msb & 0xFF;
    while (l >= 0x80) { l -= 0x80; k++; }
    while (k >= 0x80) { k -= 0x80; m++; }
    return (uint32_t)m << 16 | k << 8 | l;
}

static void write_system(unsigned index, unsigned value)
{
    uint8_t v = (uint8_t)value;
    send_sysex_msg(0x10, 0, index, &v, 1);
}

static void write_rhythm_setup(unsigned keynum, unsigned offset, unsigned value)
{
    uint8_t v = (uint8_t)value;
    uint32_t a = add_sysex_addr(((keynum - 24) << 2) + offset, 3, 1, 16);
    send_sysex_msg(a >> 16 & 0xFF, a >> 8 & 0xFF, a & 0xFF, &v, 1);
}

static void write_patch(unsigned patch, unsigned index, unsigned value, unsigned size)
{
    uint8_t v[2];
    uint32_t a = add_sysex_addr((patch << 3) + index, 5, 0, 0);
    v[0] = (uint8_t)value;
    v[1] = (uint8_t)(value >> 8);
    send_sysex_msg(a >> 16 & 0xFF, a >> 8 & 0xFF, a & 0xFF, v, size);
}

static int index_timbre(Synth *s, unsigned gnum)
{
    Mt *m = (Mt *)s;
    int si;
    for (si = 0; si < NUM_TIMBS; si++)
        if ((m->timb_attribs[si] & 0x80) && m->timb_bank[si] == (gnum >> 8 & 0xFF)
                && m->timb_num[si] == (gnum & 0xFF))
            return si;
    return -1;
}

static void setup_patch(Mt *m, unsigned patch, unsigned bank)
{
    unsigned ax;
    int i;
    patch &= 0xFF;
    bank &= 0xFF;
    m->patch_bank[patch & 0x7F] = (uint8_t)bank;
    if (bank != 0 && (i = index_timbre(&m->s, bank << 8 | patch)) != -1)
        ax = (unsigned)i << 8 | 2;      /* TIMBRE NUMBER i of group 2, MEMORY */
    else
        ax = (patch & 63) << 8 | (patch >= 64 ? 1 : 0);  /* the built-in timbre, group A or B */
    write_patch(patch, 0, ax, 2);
}

static void reset_synth(Synth *s)
{
    (void)s;
    send_sysex_msg(0x7F, 0, 0, part_chans, 1);      /* CLEAR_SYNTH: reset everything */
    send_sysex_msg(0x10, 0, 0x0D, part_chans, 9);
}

static void init_synth(Synth *s)
{
    Mt *m = (Mt *)s;
    int i;
    send_sysex_msg(0x10, 0, 0x04, part_rsv, 9);     /* ADJUST_PART_RSV */
    send_sysex_msg(0x10, 0, 0x01, init_reverb, 3);
    memset(m->queue_ptrs, 0, sizeof m->queue_ptrs);
    m->note_event = 0;
    memset(m->timb_attribs, 0, sizeof m->timb_attribs);
    for (i = 0; i < NUM_CHANS; i++) {
        m->chan_timbs[i] = 0xFF;
        m->MIDI_program[i] = 0xFF;
        m->MIDI_bank[i] = 0;
    }
    memset(m->patch_bank, 0, sizeof m->patch_bank);
}

static void send_MIDI_sysex(Synth *s, const uint8_t *src, int type, unsigned len)
{
    (void)s;
    if (type == 0xF0) send_byte(0xF0);
    while (len--) send_byte(*src++);
}

static void send_MIDI_message(Synth *s, unsigned stat, unsigned d1, unsigned d2)
{
    Mt *m = (Mt *)s;
    unsigned si = d1 & 0xFF, di = stat & 0x0F, st = stat & 0xF0, bx, op, q;
    int t;
    switch (st) {
    case 0xB0:
        if (si < SYSEX_RANGE_BEG) break;
        if (si <= SYSEX_RANGE_END) {
            si -= SYSEX_RANGE_BEG;
            bx = si / SYSEX_BLK_SIZE;
            op = si % SYSEX_BLK_SIZE;
            if (op == 0) { m->M_ad[bx] = (uint8_t)d2; return; }
            if (op == 1) { m->K_ad[bx] = (uint8_t)d2; return; }
            if (op == 2) { m->L_ad[bx] = (uint8_t)d2; return; }
            m->sysex_queues[bx * SYSEX_SIZE + m->queue_ptrs[bx]] = (uint8_t)d2;
            if (op == 3 && m->queue_ptrs[bx] < SYSEX_SIZE - 1) {
                m->queue_ptrs[bx]++;    /* a data byte: the next one */
                return;
            }
            q = m->queue_ptrs[bx] + 1u;  /* a final byte, or an overflow: send the queue */
            send_sysex_msg(m->M_ad[bx], m->K_ad[bx], m->L_ad[bx], m->sysex_queues + bx * SYSEX_SIZE, q);
            if (op != 3) q--;           /* the final byte will address the same byte again */
            {
                uint32_t a = add_sysex_addr(q, m->M_ad[bx], m->K_ad[bx], m->L_ad[bx]);
                m->L_ad[bx] = (uint8_t)a;
                m->K_ad[bx] = (uint8_t)(a >> 8);
                m->M_ad[bx] = (uint8_t)(a >> 16);
            }
            m->queue_ptrs[bx] = 0;
            return;
        }
        switch (si) {
        case PATCH_REVERB:
        case PATCH_BENDER:
            if (m->MIDI_program[di] == 0xFF) return;
            write_patch(m->MIDI_program[di], si == PATCH_REVERB ? 6 : 4, d2 & 0xFF, 1);
            send_byte(0xC0 | di);
            send_byte(m->MIDI_program[di]);
            return;
        case REVERB_MODE: write_system(1, d2); return;
        case REVERB_TIME: write_system(2, d2); return;
        case REVERB_LEVEL: write_system(3, d2); return;
        case PATCH_BANK_SEL: m->MIDI_bank[di] = (uint8_t)d2; return;
        case RHYTHM_KEY_TIMB:
            if (m->chan_timbs[di] == 0xFF) return;
            write_rhythm_setup(d2 & 0xFF, 0, m->chan_timbs[di]);
            return;
        case TIMBRE_PROTECT:
            bx = m->chan_timbs[di];
            if (bx == 0xFF) return;
            m->timb_attribs[bx] = (uint8_t)((m->timb_attribs[bx] & 0xBF) | ((int8_t)d2 >= 64 ? 0x40 : 0));
            return;
        }
        if (si >= CHAN_LOCK && si <= SEQ_INDEX) return;  /* XMIDI's own controllers stay out */
        break;
    case 0xC0:
        m->MIDI_program[di] = (uint8_t)si;
        if (m->MIDI_bank[di] != m->patch_bank[si & 0x7F]) setup_patch(m, si, m->MIDI_bank[di]);
        t = index_timbre(s, (unsigned)m->MIDI_bank[di] << 8 | si);
        m->chan_timbs[di] = (uint8_t)t;
        break;
    case 0x90:
        m->note_event++;                /* the timbre cache's LRU count */
        if (m->chan_timbs[di] != 0xFF) m->timb_hist[m->chan_timbs[di]] = m->note_event;
        break;
    }
    send_byte(stat);
    send_byte(si);
    if (st != 0xC0 && st != 0xD0) send_byte(d2);
}

static unsigned cache_size(Synth *s) { (void)s; return 0; }    /* no cache for the MT-32 */
static void define_cache(Synth *s, uint8_t *a, unsigned n) { (void)s; (void)a; (void)n; }

/* the MT-32 driver never requests banks 0 (the built-in timbres) and 127 (rhythm) */
static unsigned request_filter(Synth *s, unsigned g)
{
    (void)s;
    return (g >> 8 & 0xFF) != 0 && (g >> 8 & 0xFF) != 127;
}

static unsigned timbre_status(Synth *s, unsigned bank, unsigned num)
{
    bank &= 0xFF;
    if (bank == 0 || bank == 127) return (bank << 8 | (num & 0xFF)) + 1;   /* "present" */
    return (unsigned)(index_timbre(s, bank << 8 | (num & 0xFF)) + 1);
}

static void protect_timbre(Synth *s, unsigned bank, unsigned num, int on)
{
    Mt *m = (Mt *)s;
    int i;
    unsigned ax = (bank & 0xFF) << 8 | (num & 0xFF);
    if (ax == 0xFFFF) {
        for (i = 0; i < NUM_TIMBS; i++)
            m->timb_attribs[i] = on ? m->timb_attribs[i] | 0x40 : m->timb_attribs[i] & 0xBF;
        return;
    }
    i = index_timbre(s, ax);
    if (i != -1) m->timb_attribs[i] = on ? m->timb_attribs[i] | 0x40 : m->timb_attribs[i] & 0xBF;
}

static void install_timbre(Synth *s, unsigned bank, unsigned num, const uint8_t *addr)
{
    Mt *m = (Mt *)s;
    int si;
    unsigned dest;
    uint32_t best;
    const uint8_t *di;
    bank &= 0xFF;
    num &= 0xFF;
    if (bank == 0) goto set_patch;      /* the built-in timbres: the normal patch again */
    if (bank == 127) return;            /* melodic-mode rhythm: nothing to install */
    if (index_timbre(s, bank << 8 | num) != -1) goto set_patch;
    if (!addr) return;
    for (si = 0; si < NUM_TIMBS && (m->timb_attribs[si] & 0x80); si++)
        ;
    if (si == NUM_TIMBS) {              /* replace the least recently used unprotected one */
        int i, cx = -1;
        best = 0xFFFFFFFFu;
        for (i = 0; i < NUM_TIMBS; i++) {
            if (m->timb_attribs[i] & 0x40) continue;
            if (m->timb_hist[i] > best) continue;
            best = m->timb_hist[i];
            cx = i;
        }
        if (cx == -1) return;
        si = cx;
    }
    m->timb_hist[si] = m->note_event++;
    m->timb_num[si] = (uint8_t)num;
    m->timb_bank[si] = (uint8_t)bank;
    m->timb_attribs[si] = 0x80;
    di = addr + 2;                      /* past the length (normally F6h) */
    dest = (unsigned)si << 1;           /* MT-32 address 8, dest, 0 */
    send_sysex_msg(8, dest, 0x00, di, 0x0E);        /* the common parameter */
    di += 0x0E;
    send_sysex_msg(8, dest, 0x0E, di, 0x3A);        /* partials 1 to 4 */
    di += 0x3A;
    send_sysex_msg(8, dest, 0x48, di, 0x3A);
    di += 0x3A;
    dest++;
    send_sysex_msg(8, dest, 0x02, di, 0x3A);
    di += 0x3A;
    send_sysex_msg(8, dest, 0x3C, di, 0x3A);
set_patch:
    setup_patch(m, num, bank);
}

Synth *mt32_new(void)
{
    Mt *m = calloc(1, sizeof *m);
    m->s.min_true_chan = 2;
    m->s.max_true_chan = 9;
    m->s.max_rec_chan = 10;
    m->s.def_synth_vol = 90;            /* "avoid MT-32 distortion" */
    m->s.reset = reset_synth;
    m->s.init = init_synth;
    m->s.shutdown = 0;
    m->s.serve = 0;
    m->s.send = send_MIDI_message;
    m->s.sysex = send_MIDI_sysex;
    m->s.cache_size = cache_size;
    m->s.define_cache = define_cache;
    m->s.index_timbre = index_timbre;
    m->s.request_filter = request_filter;
    m->s.install = install_timbre;
    m->s.protect = protect_timbre;
    m->s.timbre_status = timbre_status;
    hw_opl_mode(DRV_MT32);
    return &m->s;
}
