/* target: ovr127 */
/* opts: -mm -1 -G -O -Y -d */
/* LZSS compression of save-file blocks: the whole of DOS overlay ovr127, in original order.
   This is Haruhiko Okumura's LZSS.C of 1989 (a 4096-byte ring buffer, matches of 3 to 18
   bytes, a binary search tree over the buffer), with its globals gathered into one work
   area reached through the far pointer globals, and its getc and putc turned into reads
   from and writes to a memory buffer refilled from or flushed to a file. ac_unshrink_disk
   and ac_shrink_disk in ovr153 set the work area up and call the two public entries.

   Names: DecompressLZW_disk and CompressLZW_disk are the originals, anchored by the map.
   The two helpers carried IDA names (DataCompressionSubFunction_ovr127_0 and
   DataCompressionRelatedFunction_ovr127_23C); in FM Towns the two functions just before
   DecompressLZW_disk_ are InsertNode_ and DeleteNode_, in the same order, with the same
   work-area offsets (match_position at +0Eh, match_length at +10h, text_buf at +12h,
   lson at +1025h), and they are Okumura's InsertNode and DeleteNode line for line, so
   they take those names. The local variable and field names are Okumura's where the code
   is his. */

#include "file.h"
#include "sys.h"

#define N         4096  /* size of the ring buffer */
#define F         18    /* upper limit for match_length */
#define THRESHOLD 2     /* encode a string as a position and length if longer than this */
#define NIL       N     /* the tree's empty-node index */

/* The work area. ovr153 places it at the start of stdat and the file buffer straight after
   it, at +722Fh. The bytes at 0 and 0Dh and the words at 722Bh and 722Dh are set or
   tested here but not otherwise used, so their meaning is unknown. text_buf's length is
   measured only as the distance to lson. */
struct LzwWork {
    unsigned char flag0;                /* 0x0000, tested (to no effect) at the end of compression */
    unsigned long textsize;             /* 0x0001 */
    unsigned long codesize;             /* 0x0005 */
    unsigned long printcount;           /* 0x0009 */
    unsigned char flagD;                /* 0x000D, set to 1 when compressing */
    int match_position;                 /* 0x000E */
    int match_length;                   /* 0x0010 */
    unsigned char text_buf[N + F + 1];  /* 0x0012 */
    int lson[N + 1];                    /* 0x1025 */
    int rson[N + 257];                  /* 0x3027 */
    int dad[N + 1];                     /* 0x5229 */
    int w722B;                          /* 0x722B, set to -1 when compressing */
    int w722D;                          /* 0x722D, set to 0 when compressing */
};

/* ovr153 declares this char far *; here it is the work area. */
extern struct LzwWork far *globals;

/* Inserts the string of length F at text_buf[r] into the tree, and sets match_position
   and match_length to the longest match found. A match of F bytes replaces the old node
   with the new one. */
void far InsertNode(register int r)
{
    int cmp;
    unsigned char far *key;
    register int i, p;

    cmp = 1;
    key = &globals->text_buf[r];
    p = N + 1 + key[0];
    globals->rson[r] = globals->lson[r] = NIL;
    globals->match_length = 0;
    for (;;) {
        if (cmp >= 0) {
            if (globals->rson[p] != NIL)
                p = globals->rson[p];
            else {
                globals->rson[p] = r;
                globals->dad[r] = p;
                return;
            }
        } else {
            if (globals->lson[p] != NIL)
                p = globals->lson[p];
            else {
                globals->lson[p] = r;
                globals->dad[r] = p;
                return;
            }
        }
        for (i = 1; i < F; i++)
            if ((cmp = key[i] - globals->text_buf[p + i]) != 0)
                break;
        if (i > globals->match_length) {
            globals->match_position = p;
            if ((globals->match_length = i) >= F)
                break;
        }
    }
    globals->dad[r] = globals->dad[p];
    globals->lson[r] = globals->lson[p];
    globals->rson[r] = globals->rson[p];
    globals->dad[globals->lson[p]] = r;
    globals->dad[globals->rson[p]] = r;
    if (globals->rson[globals->dad[p]] == p)
        globals->rson[globals->dad[p]] = r;
    else
        globals->lson[globals->dad[p]] = r;
    globals->dad[p] = NIL;
}

/* Removes node p from the tree. */
void far DeleteNode(register int p)
{
    register int q;

    if (globals->dad[p] == NIL)
        return;
    if (globals->rson[p] == NIL)
        q = globals->lson[p];
    else if (globals->lson[p] == NIL)
        q = globals->rson[p];
    else {
        q = globals->lson[p];
        if (globals->rson[q] != NIL) {
            do {
                q = globals->rson[q];
            } while (globals->rson[q] != NIL);
            globals->rson[globals->dad[q]] = globals->lson[q];
            globals->dad[globals->lson[q]] = globals->dad[q];
            globals->lson[q] = globals->lson[p];
            globals->dad[globals->lson[p]] = q;
        }
        globals->rson[q] = globals->rson[p];
        globals->dad[globals->rson[p]] = q;
    }
    globals->dad[q] = globals->dad[p];
    if (globals->rson[globals->dad[p]] == p)
        globals->rson[globals->dad[p]] = q;
    else
        globals->lson[globals->dad[p]] = q;
    globals->dad[p] = NIL;
}

#define min(a, b) (((a) < (b)) ? (a) : (b))

/* Reading the compressed stream: buf and left track the part of work not yet used, and
   n counts what is still to be read from the file. GETC stores the byte in tmp before
   advancing buf: written tmp = *buf++ the increment comes first. */
#define FILL() (len = min(n, worksize), buf = work, \
                left = intoFarBuffer_ovr167_5DA(fd, work, len), n -= left)
#define GETC() (left ? (left--, tmp = *buf, buf++, tmp) : \
                (FILL(), left ? (left--, tmp = *buf, buf++, tmp) : -1))

/* Decompresses n bytes of file fd into dst, using work (worksize bytes) as the read
   buffer. Returns the number of bytes written. */
unsigned far DecompressLZW_disk(unsigned char far *dst, int fd, unsigned char far *work,
                                unsigned worksize, unsigned n)
{
    int i, j, k, c;
    unsigned flags;
    unsigned char far *buf;
    unsigned count;
    unsigned len;
    int tmp;
    register unsigned left;
    register int r;

    left = 0;
    count = 0;
    FILL();
    for (i = 0; i < N - F; i++)
        globals->text_buf[i] = ' ';
    r = N - F;
    flags = 0;
    globals->textsize = 0;
    globals->printcount = 1024;
    for (;;) {
        if (((flags >>= 1) & 256) == 0) {
            if ((c = GETC()) == -1)
                break;
            flags = c | 0xFF00;
        }
        if (flags & 1) {
            if ((c = GETC()) == -1)
                break;
            count++;
            *dst++ = c;
            globals->textsize++;
            if (globals->textsize > globals->printcount)
                globals->printcount += 1024;
            globals->text_buf[r++] = c;
            r &= N - 1;
        } else {
            if ((i = GETC()) == -1)
                break;
            if ((j = GETC()) == -1)
                break;
            i |= (j & 0xF0) << 4;
            j = (j & 0x0F) + THRESHOLD;
            for (k = 0; k <= j; k++) {
                c = globals->text_buf[(i + k) & (N - 1)];
                count++;
                *dst++ = c;
                globals->textsize++;
                if (globals->textsize > globals->printcount)
                    globals->printcount += 1024;
                globals->text_buf[r++] = c;
                r &= N - 1;
            }
        }
    }
    return count;
}

#undef GETC

/* Compressing: src is read directly, and the output collects in work until it fills, when
   it is written to fd. fd -1 means compress into memory, with work big enough for all of
   it. */
#define GETC() (n ? (n--, tmp = *src, src++, tmp) : -1)
#define FLUSH() (FarWrite_ovr167_627(fd, work, out - work) != (int)(out - work) ? \
                 pfatal_code(0x4002) : (void)0, out = work)
#define PUTC(c) (out < end ? (*out = (c), ++out) : (FLUSH(), *out = (c), ++out))

/* Compresses n bytes from src to file fd, using work (worksize bytes) as the write
   buffer. Returns the compressed length, or 0 if compressing saved nothing. */
unsigned far CompressLZW_disk(unsigned char far *src, int fd, unsigned char far *work,
                              unsigned worksize, unsigned n)
{
    int c, len, r, last_match_length, code_buf_ptr;
    unsigned char code_buf[17], mask;
    int tmp;
    unsigned char far *out;
    unsigned char far *end;
    register int i, s;

    out = work;
    end = fd == -1 ? work + n : work + worksize;
    globals->textsize = 0;
    globals->codesize = 0;
    globals->printcount = 0;
    globals->flagD = 1;
    globals->w722B = -1;
    globals->w722D = 0;
    for (i = N + 1; i <= N + 256; i++)
        globals->rson[i] = NIL;
    for (i = 0; i < N; i++)
        globals->dad[i] = NIL;
    code_buf[0] = 0;
    code_buf_ptr = mask = 1;
    s = 0;
    r = N - F;
    for (i = s; i < r; i++)
        globals->text_buf[i] = ' ';
    for (len = 0; len < F && (c = GETC()) != -1; len++)
        globals->text_buf[r + len] = c;
    if ((globals->textsize = len) == 0)
        return 0;
    for (i = 1; i <= F; i++)
        InsertNode(r - i);
    InsertNode(r);
    do {
        if (globals->match_length > len)
            globals->match_length = len;
        if (globals->match_length <= THRESHOLD) {
            globals->match_length = 1;
            code_buf[0] |= mask;
            code_buf[code_buf_ptr++] = globals->text_buf[r];
        } else {
            code_buf[code_buf_ptr++] = (unsigned char)globals->match_position;
            code_buf[code_buf_ptr++] = (unsigned char)
                (((globals->match_position >> 4) & 0xF0)
                 | (globals->match_length - (THRESHOLD + 1)));
        }
        if ((mask <<= 1) == 0) {
            for (i = 0; i < code_buf_ptr; i++)
                PUTC(code_buf[i]);
            globals->codesize += code_buf_ptr;
            code_buf[0] = 0;
            code_buf_ptr = mask = 1;
        }
        last_match_length = globals->match_length;
        for (i = 0; i < last_match_length && (c = GETC()) != -1; i++) {
            DeleteNode(s);
            globals->text_buf[s] = c;
            if (s < F - 1)
                globals->text_buf[s + N] = c;
            s = (s + 1) & (N - 1);
            r = (r + 1) & (N - 1);
            InsertNode(r);
        }
        if ((globals->textsize += i) > globals->printcount)
            globals->printcount += 1024;
        while (i++ < last_match_length) {
            DeleteNode(s);
            s = (s + 1) & (N - 1);
            r = (r + 1) & (N - 1);
            if (--len)
                InsertNode(r);
        }
    } while (len > 0);
    if (code_buf_ptr > 1) {
        for (i = 0; i < code_buf_ptr; i++)
            PUTC(code_buf[i]);
        globals->codesize += code_buf_ptr;
    }
    if (fd != -1)
        FLUSH();
    /* Both exits test flag0 and do nothing with it: Okumura prints the sizes here, so
       probably a report compiled out. */
    if (globals->textsize > globals->codesize) {
        if (!globals->flag0)
            ;
        return globals->codesize;
    }
    if (!globals->flag0)
        ;
    return 0;
}
