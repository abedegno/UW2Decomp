/* REPLAY.C: recording and replaying a session, and dumping the game state (docs/PORT.md,
   "The differential test"). Not a game source: the gate never compiles it (tools/sources.py
   skips src/replay). Two builds compile it:

   - the replay DOS build (tools/replay.py), Turbo C with -DREPLAY, linked into the modding
     build as one more resident module (tools/link.py --mod --add), with every source that
     uses the hooks of portable.h also compiled with -DREPLAY;
   - the port, as one of its game sources.

   The hooks (portable.h: GAME_TIME, KEY, MOUSE, MBUTTONS, JOY_READ, JOY_BUTTONS, WALL_TIME,
   SRAND, CHECKPOINT) come here. Recording, each hook reads the world as the game would and
   writes the value it got to RECORD.OUT; replaying, it reads the value from REPLAY.IN instead
   and leaves the world alone. Both modes write STATE.OUT, the state dumps. In the DOS build
   the mode is chosen by the files: REPLAY.IN in the game's directory means replay, else
   record. The port chooses it from its command line (--record, --replay) and otherwise only
   reads the world (rp_request, set by src/port/sys/main.c before the game starts).

   The recording. The game polls its inputs hundreds of thousands of times a minute, almost
   always getting what it got last time, so each kind of input is a stream of runs: a run is
   how many calls in a row got one value (a word) and the value. The program makes its calls
   in the same order on replay, so the streams need not say how they interleave. Each stream
   is written in chunks as its buffer fills: a stream number (byte), a length (word) and the
   bytes, so the file is one sequence of chunks of the seven streams mixed. RECORD.OUT (and
   REPLAY.IN, the same), little-endian: "UW2R", version (word, 3; 2 is read too, and differs
   only in SOUND), 0 (word), the number of
   hook calls at which the recording stopped (dword, 0 if it never did); then the chunks.
     1 TIME     count, then a byte d: the clock is the last run's plus d, or d = FFh and the
                clock (dword) follows
     2 KEY      count, key's result (word), n (byte) and n pairs (offset, byte): the bytes of
                seg021's key state FD71:01E2..026A (Shift, Alt, Ctrl, the other modifiers,
                CapsLock, the layout, key_on[0..127]) that changed, as offsets from 01E2;
                n = FFh is all 137 bytes in order, as the first run has them
     3 MOUSE    count, *MouseDx, *MouseDy (words)
     4 BUTTONS  count, mbuttons's result (word)
     5 JOY      count, joy_position[0], [1] (words)
     6 JOYB     count, joy_buttons[0], [1] (words)
     7 MISC     a tag byte and its value, one call each: 7 WALL time()'s result (dword),
                8 SRAND a seed (word, checked on replay, not used), 9 CKPT a CHECKPOINT's
                number (word, checked)
     8 SOUND    count, the value (word) of a read of the sound hardware's state (SND_READ:
                a sequence's or a digital buffer's status, a locked channel, a timbre's
                status or request, a device's presence), and (version 3) the moment of the
                run's first read: PIT input clocks since the start of the tick of the clock
                the game last read (pit_moment), FFFFh for none. Sessions with no sound card
                never read one, so the recordings made before the stream existed hold none.
   Recording stops at F12 (scan code 58h), which the game never sees: the call count goes
   into the header. Replay stops at that call, or when a stream runs out, with a last dump;
   then the game shuts down as at the end of main (free_world) and exits, through C0's null
   pointer check in DOS.

   The dumps, STATE.OUT: a record per checkpoint, "CKPT", kind (word: 1 a CHECKPOINT, 2 every
   400h ticks of the replayed clock, 3 an input event, 4 the end, 5 the replay lost step),
   number (word), hook calls so far (dword), the clock (dword), sections (word), then each
   section: a four-character tag, its length (dword), its bytes:
     PLYR  the player record, PlayerDat (37Eh)
     RAND  Borland's rand seed (dword)
     LEVL  the level block mapdata points at (7E08h), once a level is in it (its magic
           word is "uw"; before that the block holds whatever memory held)
     CNTS  the calls of each stream so far (eight dwords, TIME first), to show which kind
           of input a replay that went astray asked for once too often
     SEGS  the segments of the far blocks, as words: seg_370D (the graphics data),
           stdat, cmpbuf1_start, dfx_buffer, seg004's data (EmsBuff's), seg_5DFD, seg021's
           data (Shift's), mapdata, and EmsBuff itself, the EMS page frame. Far pointers
           stored in the game's data hold these, which differ between builds and between
           machines, so tools/replay.py treats a word that is block n's segment in one dump
           and block n's in the other as equal
     NULL  DS:0..30h, the C0 null-pointer checksum's result (word, 0 when intact), and the
           interrupt vector table (400h); the port's are its stand-ins (port_null_near,
           port_null_far), so tools/replay.py checks this section in each build alone
     FBUF  with UWRPFB set, once a level is in: the 3D view's frame buffer, stdat's first
           69D6h bytes, in every dump (the periodic ones too), so that with a short UWRPCK
           interval every 3D frame is compared, not only those on the screen at an input
     and in a full dump (a CHECKPOINT, a key, a change of buttons, the end):
     GFX   seg003's data, seg_370D, 370D:0000..5E7F
     PAL   the DAC, 768 six-bit values
     CRTC  CRT controller registers 07, 09, 0C, 0D, 13, 18
     VGA   the four planes of video memory, 64 KB each, plane 0 first
   tools/replay.py compares the dumps of two runs section by section. */
#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sys.h"
#include "gfx.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "view3d.h"

#define RP_OFF      0
#define RP_RECORD   1
#define RP_REPLAY   2
#define RP_AUTO     3

#define S_TIME      1
#define S_KEY       2
#define S_MOUSE     3
#define S_BUTTONS   4
#define S_JOY       5
#define S_JOYB      6
#define S_MISC      7
#define S_SOUND     8
#define NSTREAMS    9                   /* 1..8 */

#define M_WALL      7
#define M_SRAND     8
#define M_CKPT      9

#define CK_NAMED    1
#define CK_PERIODIC 2
#define CK_INPUT    3
#define CK_END      4
#define CK_DESYNC   5

#define KSTATE_LEN  0x89                /* FD71:01E2..026A */
#define STOP_SCAN   0x58                /* F12 ends a recording */
#define GFX_LEN     0x5E80
#define CHUNK       200                 /* bytes a stream buffers */
#define HDR_LEN     12

#ifndef __TURBOC__
/* The port's side (src/port): the emulated VGA's memory and registers, Borland's rand seed,
   and the stand-ins for DS:0 and the vector table. */
const unsigned char *vga_plane(int p);
unsigned char vga_reg_crtc(int i);
void vga_get_dac(unsigned char rgb6[768]);
uint32 port_rand_seed(void);
unsigned char *port_null_copy(int far_table);
#endif

/* A stream: recording, the run in hand (its count and value) and the bytes not yet written;
   replaying, where its next chunk is looked for, its chunk's bytes, and the run in hand. */
struct Stream {
    unsigned char buf[CHUNK + 8];
    int16 len, pos;
    int32 scan;                         /* replay: the file offset to look for its next chunk */
    uint16 count;                       /* the run: calls left (replay) or made (record) */
    uint32 v1;                          /* its value */
    int16 v2;
    uint16 sub;                         /* SOUND: the moment of the run's first call (rp_sound_at) */
    unsigned char kn;                   /* KEY: n */
    unsigned char kd[KSTATE_LEN * 2];   /* KEY: the pairs, or the whole state */
};

int16 rp_request = RP_AUTO;             /* the port sets it before the game starts */
static int16 rp_mode = -1;              /* RP_OFF, RP_RECORD or RP_REPLAY once started */
static int16 rp_version = 3;            /* of the file replayed: 2 has no SOUND moments */
static int16 log_fd = -1, dump_fd = -1;
static struct Stream st[NSTREAMS];
static unsigned char bounce[512];
static uint32 events, stop_at;
static uint32 t_now;                    /* the clock the game last read */
static uint32 t_last;                   /* TIME: the last run's clock, for the deltas */
static uint32 last_ck_time;
static uint32 ck_mask = ~(uint32)0x3FF;   /* periodic dumps every 400h ticks; UWRPCK=n (hex) for n */
static int dump_fb;                        /* UWRPFB: every dump holds the 3D frame buffer */
static uint32 calls[NSTREAMS];
/* UWRPTRACE=lo,hi (hex clock values): every hook call while the clock is in [lo, hi) goes to
   TRACE.OUT as a line: the call count, the stream, the value, and in DOS the caller's return
   address (CS:IP, from the hook's stack frame), to find where two runs part. */
static int16 trace_fd = -1;
static uint32 trace_lo, trace_hi;
static char trace_buf[1500];
static int16 trace_len;
static int16 last_buttons = -1;
static unsigned char kstate[KSTATE_LEN];
static char kstate_known;
static char finishing;

static void rp_finish(int kind);
static void desync(int want, int got);
static uint16 snd_moment = 0xFFFF;      /* SOUND: rp_sound_at's moment of the read in hand */
static char snd_have;                   /* replaying: rp_sound_at has taken the read's value */
static uint16 snd_value;
/* replaying in DOS: the reads where DOS's own driver gave another value than the recording's,
   and the clock ticks they spanned (SNDCHECK.OUT), the noise floor of the port's count */
static uint32 snd_reads, snd_diffs, snd_diff_ticks, snd_diff_clock;
static void snd_check_out(void);

/* ---- the recording ------------------------------------------------------------------- */

static void chunk_out(int s)
{
    unsigned char h[3];
    struct Stream *p = &st[s];
    if (!p->len) return;
    h[0] = (unsigned char)s;
    h[1] = (unsigned char)p->len;
    h[2] = (unsigned char)(p->len >> 8);
    write(log_fd, h, 3);
    write(log_fd, p->buf, p->len);
    p->len = 0;
}

static void put_byte(int s, int b)
{
    st[s].buf[st[s].len++] = (unsigned char)b;
}

static void put_word(int s, unsigned w)
{
    put_byte(s, w & 0xFF);
    put_byte(s, (w >> 8) & 0xFF);
}

static void put_dword(int s, uint32 d)
{
    put_word(s, (unsigned)(d & 0xFFFF));
    put_word(s, (unsigned)(d >> 16));
}

/* the run in hand of stream s, written out (an entry is under 300 bytes, so a stream's
   buffer is written whenever it might not hold the next) */
static void run_out(int s)
{
    struct Stream *p = &st[s];
    int i;
    if (!p->count) return;
    if (p->len > CHUNK - 8 || (s == S_KEY && p->len + 4 + (p->kn == 0xFF ? KSTATE_LEN : 2 * p->kn) > CHUNK))
        chunk_out(s);
    put_word(s, p->count);
    switch (s) {
    case S_TIME:
        if (p->v1 - t_last < 0xFF) put_byte(s, (int)(p->v1 - t_last));
        else {
            put_byte(s, 0xFF);
            put_dword(s, p->v1);
        }
        t_last = p->v1;
        break;
    case S_KEY:
        put_word(s, (unsigned)p->v1);
        put_byte(s, p->kn);
        for (i = 0; i < (p->kn == 0xFF ? KSTATE_LEN : 2 * p->kn); i++) {
            if (p->len >= CHUNK) chunk_out(s);
            put_byte(s, p->kd[i]);
        }
        break;
    case S_BUTTONS:
        put_word(s, (unsigned)p->v1);
        break;
    case S_SOUND:
        put_word(s, (unsigned)p->v1);
        put_word(s, p->sub);
        break;
    default:                            /* MOUSE, JOY, JOYB */
        put_word(s, (unsigned)p->v1);
        put_word(s, (unsigned)p->v2);
        break;
    }
    p->count = 0;
}

/* a call that got v1, v2: the run grows, or the run is written and a new one starts */
static void record(int s, uint32 v1, int v2)
{
    struct Stream *p = &st[s];
    if (p->count && p->v1 == v1 && p->v2 == v2 && p->count < 0xFFFF && (s != S_KEY || p->kn == 0)) {
        p->count++;
        return;
    }
    run_out(s);
    p->v1 = v1;
    p->v2 = v2;
    p->kn = 0;
    if (s == S_SOUND) p->sub = snd_moment;
    p->count = 1;
}

static void misc_out(int tag, uint32 v, int dword)
{
    if (st[S_MISC].len > CHUNK - 8) chunk_out(S_MISC);
    put_byte(S_MISC, tag);
    if (dword) put_dword(S_MISC, v);
    else put_word(S_MISC, (unsigned)v);
}

/* ---- the replay ---------------------------------------------------------------------- */

/* the next byte of stream s: its next chunk is found by reading the chunk headers from where
   the last one ended, or -1 at the end of the file */
static int get_byte(int s)
{
    struct Stream *p = &st[s];
    unsigned char h[3];
    unsigned n;
    while (p->pos >= p->len) {
        lseek(log_fd, p->scan, SEEK_SET);
        if (read(log_fd, h, 3) != 3) return -1;
        n = h[1] | h[2] << 8;
        p->scan += 3 + n;
        if (h[0] == s && n <= sizeof p->buf) {
            p->len = read(log_fd, p->buf, n);
            p->pos = 0;
        }
    }
    return p->buf[p->pos++];
}

static unsigned get_word(int s)
{
    unsigned lo = get_byte(s) & 0xFF;
    return lo | (get_byte(s) & 0xFF) << 8;
}

static uint32 get_dword(int s)
{
    uint32 lo = get_word(s);
    return lo | (uint32)get_word(s) << 16;
}

/* the next call of stream s: when its run is used up the next run is read (and a key run's
   state applied); a stream that has run out ends the replay */
static struct Stream *replayed(int s)
{
    struct Stream *p = &st[s];
    int i, c, n;
    if (p->count == 0) {
        c = get_byte(s);
        if (c < 0) rp_finish(CK_END);
        p->count = c | get_byte(s) << 8;
        switch (s) {
        case S_TIME:
            c = get_byte(s);
            p->v1 = c == 0xFF ? get_dword(s) : p->v1 + c;
            break;
        case S_KEY:
            p->v1 = get_word(s);
            n = get_byte(s);
            if (n == 0xFF)
                for (i = 0; i < KSTATE_LEN; i++) Shift[i] = (unsigned char)get_byte(s);
            else
                while (n-- > 0) {
                    i = get_byte(s);
                    Shift[i] = (unsigned char)get_byte(s);
                }
            break;
        case S_BUTTONS:
            p->v1 = get_word(s);
            break;
        case S_SOUND:
            p->v1 = get_word(s);
            p->sub = rp_version >= 3 ? get_word(s) : 0xFFFF;
            break;
        default:
            p->v1 = get_word(s);
            p->v2 = get_word(s);
            break;
        }
        if (p->count == 0) desync(s, 0);
    }
    p->count--;
    return p;
}

/* ---- starting and stopping -------------------------------------------------------- */

static void rp_start(void)
{
    static char hdr[] = "UW2R\3\0\0\0\0\0\0\0";
    int s;
    if (rp_mode >= 0) return;
    rp_mode = RP_OFF;
    if (rp_request == RP_OFF) return;
    if (rp_request != RP_RECORD) {
        log_fd = open("REPLAY.IN", O_RDONLY | O_BINARY);
        if (log_fd >= 0) {
            read(log_fd, bounce, HDR_LEN);
            if (memcmp(bounce, hdr, 4) || (bounce[4] != 2 && bounce[4] != 3) || bounce[5]) {
                close(log_fd);
                return;
            }
            rp_version = bounce[4];
            stop_at = bounce[8] | (uint32)bounce[9] << 8 | (uint32)bounce[10] << 16 | (uint32)bounce[11] << 24;
            for (s = 1; s < NSTREAMS; s++) st[s].scan = HDR_LEN;
            rp_mode = RP_REPLAY;
        } else if (rp_request == RP_REPLAY)
            return;
    }
    if (rp_mode == RP_OFF) {
        log_fd = open("RECORD.OUT", O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0x180);
        if (log_fd < 0) return;
        rp_mode = RP_RECORD;
        write(log_fd, hdr, HDR_LEN);
    }
    dump_fd = open("STATE.OUT", O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0x180);
    if (getenv("UWRPCK")) ck_mask = ~(strtoul(getenv("UWRPCK"), 0, 16) - 1);
    if (getenv("UWRPFB")) dump_fb = 1;
    if (getenv("UWRPTRACE")) {
        char *e = getenv("UWRPTRACE");
        trace_lo = strtoul(e, &e, 16);
        trace_hi = *e ? strtoul(e + 1, 0, 16) : 0xFFFFFFFFUL;
        trace_fd = open("TRACE.OUT", O_WRONLY | O_CREAT | O_TRUNC | O_TEXT, 0x180);
    }
}

/* recording: every run and chunk written, and the call count it stopped at in the header */
static void record_close(void)
{
    unsigned char b[4];
    int s;
    for (s = 1; s < NSTREAMS; s++) {
        run_out(s);
        chunk_out(s);
    }
    b[0] = (unsigned char)events;
    b[1] = (unsigned char)(events >> 8);
    b[2] = (unsigned char)(events >> 16);
    b[3] = (unsigned char)(events >> 24);
    lseek(log_fd, 8, SEEK_SET);
    write(log_fd, b, 4);
}

/* ---- the state dump ---------------------------------------------------------------- */

static void dump_bytes(void *p, unsigned n)
{
    write(dump_fd, p, n);
}

static void dump_word(unsigned w)
{
    unsigned char b[2];
    b[0] = (unsigned char)w;
    b[1] = (unsigned char)(w >> 8);
    dump_bytes(b, 2);
}

static void dump_dword(uint32 d)
{
    dump_word((unsigned)(d & 0xFFFF));
    dump_word((unsigned)(d >> 16));
}

static void section(char *tag, uint32 len)
{
    dump_bytes(tag, 4);
    dump_dword(len);
}

/* far bytes to the dump through a near buffer */
static void dump_far(unsigned char far *p, uint32 n)
{
    unsigned k;
    while (n) {
        k = n > sizeof bounce ? sizeof bounce : (unsigned)n;
        FAR_COPY(bounce, p, k);
        dump_bytes(bounce, k);
        p += k;
        n -= k;
    }
}

/* Borland's rand seed. In DOS it is a static of the C library's rand module; srand's code
   stores to it with mov [Seed],ax at its byte 6 (A3 lo hi), so its DGROUP offset is the word
   at srand + 7 (TC++ 1.01's CM.LIB, checked against UW2.EXE). */
static uint32 rand_seed(void)
{
#ifdef __TURBOC__
    unsigned char far *code = (unsigned char far *)srand;
    unsigned off = code[7] | code[8] << 8;
    return *(uint32 *)off;
#else
    return port_rand_seed();
#endif
}

static void dump_null(void)
{
    unsigned char far *ds0;
    unsigned char far *ivt;
    unsigned sum, i;
#ifdef __TURBOC__
    ds0 = (unsigned char far *)MK_FP(_DS, 0);
    ivt = (unsigned char far *)MK_FP(0, 0);
#else
    ds0 = port_null_copy(0);
    ivt = port_null_copy(1);
#endif
    for (sum = 0, i = 4; i < 0x31; i++)
        sum += ds0[i];
    section("NULL", 0x31 + 2 + 0x400);
    dump_far(ds0, 0x31);
    dump_word(sum - 0x0CA5);
    dump_far(ivt, 0x400);
}

static void dump_screen(void)
{
    static unsigned char regs[6] = { 0x07, 0x09, 0x0C, 0x0D, 0x13, 0x18 };
    unsigned char crtc[6];
    int p, i;
#ifdef __TURBOC__
    unsigned gi, rm, ci;
    unsigned off;
#else
    unsigned char dac[768];
#endif

    section("GFX ", GFX_LEN);
    dump_far(palette - 0x5048, GFX_LEN);
#ifdef __TURBOC__
    /* the DAC, read from the card (the game never reads it, so its read index is ours) */
    section("PAL ", 768);
    outportb(0x3C7, 0);
    for (i = 0; i < 768; i++) {
        bounce[i & 0xFF] = inportb(0x3C9);
        if ((i & 0xFF) == 0xFF) dump_bytes(bounce, 256);
    }
    ci = inportb(0x3D4);
    for (i = 0; i < 6; i++) {
        outportb(0x3D4, regs[i]);
        crtc[i] = inportb(0x3D5);
    }
    outportb(0x3D4, ci);
    section("CRTC", 6);
    dump_bytes(crtc, 6);
    /* the planes, through the graphics controller's read map; its index and the read map
       are put back */
    section("VGA ", 0x40000L);
    gi = inportb(0x3CE);
    outportb(0x3CE, 4);
    rm = inportb(0x3CF);
    for (p = 0; p < 4; p++) {
        outportb(0x3CF, p);
        off = 0;
        do {
            movedata(0xA000, off, _DS, (unsigned)bounce, sizeof bounce);
            dump_bytes(bounce, sizeof bounce);
            off += sizeof bounce;
        } while (off);
    }
    outportb(0x3CF, rm);
    outportb(0x3CE, gi);
#else
    section("PAL ", 768);
    vga_get_dac(dac);
    dump_bytes(dac, 768);
    for (i = 0; i < 6; i++)
        crtc[i] = vga_reg_crtc(regs[i]);
    section("CRTC", 6);
    dump_bytes(crtc, 6);
    section("VGA ", 0x40000L);
    for (p = 0; p < 4; p++)
        dump_far((unsigned char far *)vga_plane(p), 0x10000L);
#endif
}

static void dump_segs(void)
{
    section("SEGS", 18);
    dump_word(FP_SEG(palette));
    dump_word(FP_SEG(stdat));
    dump_word(FP_SEG(cmpbuf1_start));
    dump_word(FP_SEG(dfx_buffer));
    dump_word(FP_SEG(&EmsBuff));
    dump_word(FP_SEG(seg_5DFD));
    dump_word(FP_SEG(Shift));
    dump_word(FP_SEG(mapdata));
    dump_word(EmsBuff);
}

static void rp_dump(int kind, int n, int full)
{
    int level, i;
    if (dump_fd < 0) return;
    dump_bytes("CKPT", 4);
    dump_word(kind);
    dump_word(n);
    dump_dword(events);
    dump_dword(t_now);
    level = mapdata && LEVEL->magic == LEVEL_MAGIC;
    dump_word((level ? 6 : 5) + (full ? 4 : 0) + (dump_fb && level));
    section("PLYR", sizeof PlayerDat);
    dump_bytes(&PlayerDat, sizeof PlayerDat);
    section("RAND", 4);
    dump_dword(rand_seed());
    if (level) {
        section("LEVL", 0x7E08);
        dump_far((unsigned char far *)mapdata, 0x7E08);
    }
    dump_null();
    dump_segs();
    section("CNTS", 4 * (NSTREAMS - 1));
    for (i = 1; i < NSTREAMS; i++) dump_dword(calls[i]);
    if (dump_fb && level) {
        section("FBUF", 0x69D6);
        dump_far((unsigned char far *)stdat, 0x69D6);
    }
    if (full)
        dump_screen();
}

/* The replay has gone astray: stream want did not give what the game asked for. */
static void desync(int want, int got)
{
    rp_dump(CK_DESYNC, (want << 8) | (got & 0xFF), 1);
    rp_finish(-1);
}

/* The end: a last full dump, the files closed, and the game shut down as main does. A
   negative kind means the dump is written already (desync). */
static void rp_finish(int kind)
{
    if (finishing) return;
    finishing = 1;
    if (rp_mode == RP_RECORD) record_close();
    if (rp_mode == RP_REPLAY) snd_check_out();
    if (kind >= 0) rp_dump(kind, 0, 1);
    if (trace_fd >= 0) {
        write(trace_fd, trace_buf, trace_len);
        close(trace_fd);
    }
    close(log_fd);
    if (dump_fd >= 0) close(dump_fd);
    free_world(1);
    exit(0);
}

#ifndef __TURBOC__
/* The port stops at a stub or an unported path (port_halt): the recording so far and a last
   full dump (kind END, number 1) are written, so a replay that stops early still compares
   up to where it stopped. */
void rp_halt(void)
{
    if (rp_mode <= RP_OFF || finishing) return;
    finishing = 1;
    if (rp_mode == RP_RECORD) record_close();
    rp_dump(CK_END, 1, 1);
    close(log_fd);
    if (dump_fd >= 0) close(dump_fd);
}
#endif

/* Every hook starts here. The call count names the moment for the dumps, and a replay stops
   at the call the recording stopped at. */
static int begin(void)
{
    rp_start();
    if (rp_mode == RP_OFF || finishing) return 0;
    events++;
    if (rp_mode == RP_REPLAY && events == stop_at) rp_finish(CK_END);
    return 1;
}

static void hex(char *p, uint32 v, int n)
{
    while (n--) {
        p[n] = "0123456789ABCDEF"[(unsigned)v & 15];
        v >>= 4;
    }
}

/* one trace line: the call count, the stream, the value, the caller */
static void trace(int s, uint32 v, unsigned cs, unsigned ip)
{
    char line[48];
#ifdef __TURBOC__
    /* and the caller's caller, one frame further up the saved BP chain */
    unsigned bp2 = ((unsigned *)_BP)[0];
    unsigned bp3 = ((unsigned *)bp2)[0];
    unsigned cs2 = ((unsigned *)bp3)[2], ip2 = ((unsigned *)bp3)[1];
#else
    unsigned cs2 = 0, ip2 = 0;
#endif
    if (trace_fd < 0 || t_now < trace_lo || t_now >= trace_hi) return;
    hex(line, events, 8);
    line[8] = ' ';
    line[9] = (char)('0' + s);
    line[10] = ' ';
    hex(line + 11, v, 8);
    line[19] = ' ';
    hex(line + 20, cs, 4);
    line[24] = ':';
    hex(line + 25, ip, 4);
    line[29] = ' ';
    hex(line + 30, cs2, 4);
    line[34] = ':';
    hex(line + 35, ip2, 4);
    line[39] = '\n';
    if (trace_len + 40 > sizeof trace_buf) {
        write(trace_fd, trace_buf, trace_len);
        trace_len = 0;
    }
    memcpy(trace_buf + trace_len, line, 40);
    trace_len += 40;
}

#ifdef __TURBOC__
/* the caller of the hook that calls this: its frame's saved BP, then its return address */
#define CALLER_CS (((unsigned *)_BP)[2])
#define CALLER_IP (((unsigned *)_BP)[1])
#else
#define CALLER_CS 0
#define CALLER_IP ((unsigned)(uintptr_t)__builtin_return_address(0))
#endif

/* ---- the hooks ---------------------------------------------------------------------- */

/* ---- the timers REPLAY.C runs (SLAVE_TIMER) ------------------------------------------ */

/* A game callback AIL ran from the timer interrupt runs here instead, at a read of the game
   clock, as many times as AIL's DDA would have fired it by then: the PIT ticks once a clock
   tick (3906 us, the 256 Hz clock being the fastest timer), and the timer fires whenever the
   ticks' microseconds pass a multiple of its period. In DOS that phase is set by when the
   timers were last reprogrammed, which no run controls; here it is a function of the clock
   alone, the same in every run. */
#define NSLAVES 2
static RpTimerFn slave_fn[NSLAVES];
static uint32 slave_period[NSLAVES];
static uint32 slave_last[NSLAVES];
static char slave_started[NSLAVES];
static char slave_busy;

static void far rp_null_timer(void)
{
}

RpTimerFn far rp_slave_timer(RpTimerFn f, unsigned hz)
{
    int i;
    for (i = 0; i < NSLAVES; i++)
        if (!slave_fn[i] || slave_fn[i] == f) {
            slave_fn[i] = f;
            slave_period[i] = 1000000UL / hz;
            slave_started[i] = 0;
            return rp_null_timer;
        }
    return f;
}

static void slave_ticks(uint32 t)
{
    int i;
    uint32 n;
    if (slave_busy) return;
    slave_busy = 1;
    for (i = 0; i < NSLAVES; i++) {
        if (!slave_fn[i]) continue;
        /* the DDA's firings by clock t, t * 3906 / period without overflowing 32 bits */
        n = (t / slave_period[i]) * 3906UL + ((t % slave_period[i]) * 3906UL) / slave_period[i];
        if (!slave_started[i] || n < slave_last[i]) {
            slave_started[i] = 1;
            slave_last[i] = n;
            continue;
        }
        while (slave_last[i] < n) {
            slave_last[i]++;
            slave_fn[i]();
        }
    }
    slave_busy = 0;
}

#ifndef __TURBOC__
void port_clock_read(uint32 t);         /* src/port/sound/ail.c: AIL's ticks under replay */
#endif

uint32 far rp_time(void)
{
    uint32 t;
    if (!begin())
        t = *Time;
    else {
        if (rp_mode == RP_RECORD) {
            t_now = *Time;
            record(S_TIME, t_now, 0);
        } else
            t_now = replayed(S_TIME)->v1;
        calls[S_TIME]++;
        if (trace_fd >= 0) trace(S_TIME, t_now, CALLER_CS, CALLER_IP);
        if ((t_now ^ last_ck_time) & ck_mask) {
            last_ck_time = t_now;
            rp_dump(CK_PERIODIC, 0, 0);
        }
        t = t_now;
    }
#ifndef __TURBOC__
    port_clock_read(t);
#endif
    slave_ticks(t);
    return t;
}

int far rp_key(void)
{
    struct Stream *p = &st[S_KEY];
    int r, i, n;
    if (!begin()) return key();
    if (rp_mode == RP_RECORD) {
        r = key();
        if ((r >> 8 & 0xFF) == STOP_SCAN) rp_finish(CK_END);
        for (n = 0, i = 0; i < KSTATE_LEN; i++)
            if (Shift[i] != kstate[i]) n++;
        if (n || !kstate_known) {
            run_out(S_KEY);
            p->kn = !kstate_known || n > 64 ? 0xFF : (unsigned char)n;
            for (n = 0, i = 0; i < KSTATE_LEN; i++)
                if (p->kn == 0xFF) p->kd[n++] = Shift[i];
                else if (Shift[i] != kstate[i]) {
                    p->kd[n++] = (unsigned char)i;
                    p->kd[n++] = Shift[i];
                }
            for (i = 0; i < KSTATE_LEN; i++) kstate[i] = Shift[i];
            kstate_known = 1;
            p->v1 = (uint16)r;
            p->v2 = 0;
            p->count = 1;
        } else
            record(S_KEY, (uint16)r, 0);
    } else
        r = (int16)replayed(S_KEY)->v1;
    calls[S_KEY]++;
    if (trace_fd >= 0) trace(S_KEY, (uint16)r, CALLER_CS, CALLER_IP);
    if (r) rp_dump(CK_INPUT, r, 1);
    return r;
}

void far rp_mouse(void)
{
    struct Stream *p;
    if (!begin()) {
        mouse();
        return;
    }
    calls[S_MOUSE]++;
    if (rp_mode == RP_RECORD) {
        mouse();
        record(S_MOUSE, (uint16)*MouseDx, *MouseDy);
    } else {
        p = replayed(S_MOUSE);
        *MouseDx = (int16)p->v1;
        *MouseDy = p->v2;
    }
    if (trace_fd >= 0) trace(S_MOUSE, (uint32)(uint16)*MouseDx << 16 | (uint16)*MouseDy, CALLER_CS, CALLER_IP);
}

int far rp_mbuttons(void)
{
    int b;
    if (!begin()) return mbuttons();
    calls[S_BUTTONS]++;
    if (rp_mode == RP_RECORD) {
        b = mbuttons();
        record(S_BUTTONS, (uint16)b, 0);
    } else
        b = (int16)replayed(S_BUTTONS)->v1;
    if (trace_fd >= 0) trace(S_BUTTONS, (uint16)b, CALLER_CS, CALLER_IP);
    if (b != last_buttons) {
        last_buttons = b;
        rp_dump(CK_INPUT, 0x8000 | b, 1);
    }
    return b;
}

#ifndef __TURBOC__
void port_sound_read(unsigned own, unsigned recorded, uint32 clock);   /* src/port/sound/ail.c */
void port_sound_moment(uint32 clock, unsigned moment);
#endif

#ifdef __TURBOC__
/* The moment of a read of the sound hardware, in PIT input clocks (1193182 Hz) since the
   start of the clock tick t_now, the one the game last read: with a sound card a digital
   buffer ends at any moment of a tick, and a game that polls its status sees it change
   between two reads of one clock value. AIL runs the PIT in mode 3 at 3906 us for the 256 Hz
   clock, divisor 3906 * 10000 / 8380 = 4661 (set_PIT_period); each interrupt is one tick of
   *Time. The read-back command latches channel 0's status (bit 7, the output: high for the
   first half of the period) and its count (which mode 3 counts down twice a period, by 2);
   an interrupt the PIC holds but has not yet delivered is a tick *Time does not yet show. */
#define PIT_DIVISOR 4661u
static uint16 pit_moment(void)
{
    unsigned status, n, e, irr;
    uint32 now, d;
    disable();
    outportb(0x43, 0xC2);
    status = inportb(0x40);
    n = inportb(0x40);
    n |= inportb(0x40) << 8;
    outportb(0x20, 0x0A);
    irr = inportb(0x20);
    now = *Time;
    enable();
    e = n < PIT_DIVISOR ? (PIT_DIVISOR - n) / 2 : 0;
    if (!(status & 0x80)) e += PIT_DIVISOR / 2;
    d = (now - t_now + (irr & 1)) * PIT_DIVISOR + e;
    return d > 0xFFFEu ? 0xFFFE : (uint16)d;
}
#endif

/* SND_READ's first half, before the read: recording, the moment of the read (in DOS; the
   port records none); replaying, the read's recorded value is taken here, and the port's
   drivers are told the moment DOS read it, so that its own driver is asked at that moment */
void far rp_sound_at(int drv)
{
    if (drv < 0) return;
    rp_start();
    if (finishing) return;
    if (rp_mode == RP_RECORD) {
#ifdef __TURBOC__
        snd_moment = pit_moment();
#endif
    } else if (rp_mode == RP_REPLAY) {
        struct Stream *p = replayed(S_SOUND);
        snd_value = (uint16)p->v1;
        snd_have = 1;
#ifndef __TURBOC__
        port_sound_moment(t_now, p->sub);
#endif
        p->sub = 0xFFFF;            /* the moment is the run's first call's */
    }
}

unsigned far rp_sound(int drv, unsigned v)
{
    if (drv < 0) return v;          /* no driver: AIL answers 0, the same in every run */
    if (!begin()) return v;
    calls[S_SOUND]++;
    if (rp_mode == RP_RECORD)
        record(S_SOUND, (uint16)v, 0);
    else {
#ifndef __TURBOC__
        unsigned own = v;
#endif
        if (!snd_have) snd_value = (uint16)replayed(S_SOUND)->v1;
        snd_have = 0;
#ifdef __TURBOC__
        snd_reads++;
        if ((uint16)v != snd_value) {
            if (snd_diffs++ == 0 || t_now != snd_diff_clock) snd_diff_ticks++;
            snd_diff_clock = t_now;
        }
#endif
#ifndef __TURBOC__
        port_sound_read(own, snd_value, t_now);
#endif
        v = snd_value;
    }
    if (trace_fd >= 0) trace(S_SOUND, (uint16)v, CALLER_CS, CALLER_IP);
    return v;
}

static void snd_check_out(void)
{
#ifdef __TURBOC__
    char line[128], *p = line;
    int fd;
    if (!snd_reads) return;
    strcpy(p, "sound reads: ");
    ultoa(snd_reads, p + strlen(p), 10);
    strcat(p, ", ");
    ultoa(snd_diffs, p + strlen(p), 10);
    strcat(p, " where DOS's driver differed from the recording (over ");
    ultoa(snd_diff_ticks, p + strlen(p), 10);
    strcat(p, " clock ticks)\r\n");
    fd = open("SNDCHECK.OUT", O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0x180);
    if (fd >= 0) {
        write(fd, line, strlen(line));
        close(fd);
    }
#endif
}

void far rp_joy(void)
{
    struct Stream *p;
    if (!begin()) {
        seg021_22FD_7CD();
        return;
    }
    calls[S_JOY]++;
    if (rp_mode == RP_RECORD) {
        seg021_22FD_7CD();
        record(S_JOY, (uint16)joy_position[0], joy_position[1]);
    } else {
        p = replayed(S_JOY);
        joy_position[0] = (int16)p->v1;
        joy_position[1] = p->v2;
    }
}

void far rp_joyb(void)
{
    struct Stream *p;
    if (!begin()) {
        seg021_22FD_809();
        return;
    }
    calls[S_JOYB]++;
    if (rp_mode == RP_RECORD) {
        seg021_22FD_809();
        record(S_JOYB, (uint16)joy_buttons[0], joy_buttons[1]);
    } else {
        p = replayed(S_JOYB);
        joy_buttons[0] = (int16)p->v1;
        joy_buttons[1] = p->v2;
    }
}

/* a MISC entry: recording, written; replaying, the tag read and checked */
static uint32 misc(int tag, uint32 v, int dword)
{
    int got;
    calls[S_MISC]++;
    if (rp_mode == RP_RECORD) {
        misc_out(tag, v, dword);
        return v;
    }
    got = get_byte(S_MISC);
    if (got < 0) rp_finish(CK_END);
    if (got != tag) desync(S_MISC, got);
    return dword ? get_dword(S_MISC) : get_word(S_MISC);
}

int32 far rp_walltime(void)
{
    if (!begin()) return (int32)time(NULL);
    return (int32)misc(M_WALL, rp_mode == RP_RECORD ? (uint32)time(NULL) : 0, 1);
}

void far rp_srand(unsigned seed)
{
    if (begin() && misc(M_SRAND, seed & 0xFFFF, 0) != (seed & 0xFFFF))
        desync(S_MISC, M_SRAND);
    srand(seed);
}

void far rp_checkpoint(int n)
{
    if (!begin()) return;
    if (misc(M_CKPT, (unsigned)n, 0) != (unsigned)n) desync(S_MISC, M_CKPT);
    rp_dump(CK_NAMED, n, 1);
}
