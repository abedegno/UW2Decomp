/* parmap.c: replaces the 8086's segmented addressing, and Borland's far heap (farmalloc,
   farfree, farcoreleft, coreleft). The paragraph map of docs/PORT.md, "Far pointers and
   segments".

   A region is a block of host memory with a DOS paragraph number. Three kinds:
   - a segment: a far data segment of the EXE (mem/fardata.c) or the EMS page frame. Its
     pointers come back as that segment and an offset from it, as DOS had them (gr_offs is
     43A0:A800, palette 370D:5048), so FP_OFF means what it meant in DOS.
   - the heap: the emulated conventional memory from PORT_HEAP_FIRST to PORT_HEAP_END that
     farmalloc hands out. Its pointers come back normalised (offset below 16), and each block
     starts at offset 4 of its paragraph, as Borland's far heap gives them, so FP_SEG(p) + 1
     is the paragraph after the block's start, as SOUND.C expects.
   - a window: any other host pointer the game takes apart with FP_SEG (a near buffer passed
     to intdosx or movedata). It gets a 64 KB window of paragraphs based at the pointer, so
     MK_FP(FP_SEG(p), FP_OFF(p) + n) is p + n. Windows are reused in turn; each new one is
     logged when tracing, so the idiom can be found and given a named macro.
   MK_FP finds the region holding the linear address seg * 16 + off. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"

struct region {
    const char *name;
    unsigned char *base;
    size_t size;
    unsigned seg;
    int kind;               /* 0 segment, 1 heap, 2 window */
};
#define MAXREG 256
static struct region reg[MAXREG];
static int nreg;

/* The windows: paragraphs B000 to DFFF, three windows of 1000h paragraphs. */
#define WIN_FIRST 0xB000u
#define WIN_COUNT 3
static int win_next;

void pm_add(const char *name, void *base, size_t size, unsigned seg)
{
    if (nreg == MAXREG) port_fatal("paragraph map full");
    reg[nreg].name = name;
    reg[nreg].base = base;
    reg[nreg].size = size;
    reg[nreg].seg = seg;
    reg[nreg].kind = 0;
    nreg++;
}

void pm_remove(void *base)
{
    int i;
    for (i = 0; i < nreg; i++)
        if (reg[i].base == base) { reg[i] = reg[--nreg]; return; }
}

static struct region *by_ptr(const unsigned char *p)
{
    int i;
    for (i = 0; i < nreg; i++)
        if (p >= reg[i].base && p < reg[i].base + reg[i].size) return &reg[i];
    return NULL;
}

static struct region *window_for(const unsigned char *p)
{
    unsigned seg = WIN_FIRST + 0x1000u * (unsigned)win_next;
    int i;
    for (i = 0; i < nreg; i++)
        if (reg[i].kind == 2 && reg[i].seg == seg) break;
    if (i == nreg) {
        if (nreg == MAXREG) port_fatal("paragraph map full");
        nreg++;
    }
    reg[i].name = "window";
    reg[i].base = (unsigned char *)p;
    reg[i].size = 0x10000;
    reg[i].seg = seg;
    reg[i].kind = 2;
    win_next = (win_next + 1) % WIN_COUNT;
    port_log("parmap: window %04X:0000 at host %p\n", seg, (void *)p);
    return &reg[i];
}

void *port_mk_fp(unsigned seg, unsigned off)
{
    uint32_t lin = ((seg & 0xFFFFu) << 4) + (off & 0xFFFFu);
    int i;
    if (seg == 0 && off == 0) return NULL;
    for (i = 0; i < nreg; i++) {
        uint32_t start = reg[i].seg << 4;
        if (lin >= start && lin < start + reg[i].size) return reg[i].base + (lin - start);
    }
    port_log("parmap: MK_FP(%04X, %04X) is in no region\n", seg, off);
    return NULL;
}

static struct region *region_of(const volatile void *vp)
{
    const unsigned char *p = (const unsigned char *)vp;
    struct region *r;
    if (!p) return NULL;
    r = by_ptr(p);
    if (!r || (r->kind == 2 && p - r->base >= 0xF000)) r = window_for(p);
    return r;
}

unsigned port_fp_seg(const volatile void *p)
{
    struct region *r = region_of(p);
    size_t d;
    if (!r) return 0;
    d = (size_t)((const unsigned char *)p - r->base);
    if (r->kind == 1) return r->seg + (unsigned)(d >> 4);
    if (d >= 0x10000) return r->seg + (unsigned)((d - 0xFFF0) >> 4);
    return r->seg;
}

unsigned port_fp_off(const volatile void *p)
{
    struct region *r = region_of(p);
    size_t d;
    if (!r) return 0;
    d = (size_t)((const unsigned char *)p - r->base);
    if (r->kind == 1) return (unsigned)(d & 15);
    if (d >= 0x10000) return (unsigned)(d - ((d - 0xFFF0) & ~(size_t)15));
    return (unsigned)d;
}

void port_far_copy(void *dst, const void *src, unsigned n)
{
    memmove(dst, src, n);
}

/* Borland's movedata: n bytes from srcseg:srcoff to dstseg:dstoff. */
void movedata(unsigned srcseg, unsigned srcoff, unsigned dstseg, unsigned dstoff, unsigned n)
{
    unsigned char *s = port_mk_fp(srcseg, srcoff), *d = port_mk_fp(dstseg, dstoff);
    if (!s || !d) port_fatal("movedata %04X:%04X -> %04X:%04X, %u bytes: no region", srcseg, srcoff, dstseg, dstoff, n);
    memmove(d, s, n);
}

/* The far heap: first fit over the paragraphs of the emulated conventional memory. A block of
   n bytes takes (n + 4 + 15) / 16 paragraphs; the 4 bytes are Borland's block header. */
static unsigned char *heap;
static uint8_t heap_used[PORT_HEAP_END - PORT_HEAP_FIRST];      /* 1 a paragraph in use */
static uint16_t heap_len[PORT_HEAP_END - PORT_HEAP_FIRST];      /* paragraphs, at a block's first */

static void heap_init(void)
{
    if (heap) return;
    heap = calloc(PORT_HEAP_END - PORT_HEAP_FIRST, 16);
    if (!heap) port_fatal("no memory for the far heap");
    if (nreg == MAXREG) port_fatal("paragraph map full");
    reg[nreg].name = "far heap";
    reg[nreg].base = heap;
    reg[nreg].size = (size_t)(PORT_HEAP_END - PORT_HEAP_FIRST) * 16;
    reg[nreg].seg = PORT_HEAP_FIRST;
    reg[nreg].kind = 1;
    nreg++;
}

void *farmalloc(unsigned long n)
{
    unsigned long paras = (n + 4 + 15) / 16;
    unsigned i, run = 0;
    heap_init();
    if (n == 0 || paras > PORT_HEAP_END - PORT_HEAP_FIRST) return NULL;
    for (i = 0; i < PORT_HEAP_END - PORT_HEAP_FIRST; i++) {
        run = heap_used[i] ? 0 : run + 1;
        if (run == paras) {
            unsigned first = i + 1 - (unsigned)paras;
            memset(heap_used + first, 1, paras);
            heap_len[first] = (uint16_t)paras;
            memset(heap + (size_t)first * 16, 0, (size_t)paras * 16);
            return heap + (size_t)first * 16 + 4;
        }
    }
    return NULL;
}

void *farcalloc(unsigned long count, unsigned long size)
{
    return farmalloc(count * size);
}

void farfree(void *block)
{
    unsigned char *p = block;
    size_t first;
    if (!p) return;
    heap_init();
    if (p < heap + 4 || p >= heap + (size_t)(PORT_HEAP_END - PORT_HEAP_FIRST) * 16) {
        port_log("farfree of %p, not a far heap block\n", block);
        return;
    }
    first = (size_t)(p - 4 - heap) / 16;
    memset(heap_used + first, 0, heap_len[first]);
    heap_len[first] = 0;
}

void *farrealloc(void *block, unsigned long n)
{
    unsigned char *p = block, *q;
    size_t old;
    if (!p) return farmalloc(n);
    q = farmalloc(n);
    if (!q) return NULL;
    old = (size_t)heap_len[(size_t)(p - 4 - heap) / 16] * 16 - 4;
    memcpy(q, p, old < n ? old : n);
    farfree(p);
    return q;
}

/* Borland's farcoreleft: the bytes above the highest block in use. */
unsigned long farcoreleft(void)
{
    unsigned i = PORT_HEAP_END - PORT_HEAP_FIRST;
    heap_init();
    while (i > 0 && !heap_used[i - 1]) i--;
    return (unsigned long)(PORT_HEAP_END - PORT_HEAP_FIRST - i) * 16;
}

/* coreleft: the free near heap. The near heap is the host's, so this reports a fixed amount,
   more than the 2200 bytes OkEnoughMem asks for, as DOS did on the reference set-up. */
unsigned coreleft(void)
{
    return 0x4000;
}

/* The null pointers (docs/PORT.md, "Null pointers"): copies of what DOS reads at DS:0 and at
   0000:0000. */
static unsigned char null_near[0x100];
static unsigned char null_far[0x400];
static int null_init;

static void nulls(void)
{
    if (null_init) return;
    null_init = 1;
    memcpy(null_near, port_dgroup_image, sizeof null_near);
    /* DS:0..2, the tail of ovr167's last overlay stub entry while ovr167 is not loaded */
    null_near[0] = 0x27; null_near[1] = 0x06; null_near[2] = 0x00; null_near[3] = 0x00;
    /* int 0 -> int0_trap: offset 0019h, segment 1FC7h plus the load segment */
    null_far[0] = 0x19; null_far[1] = 0x00;
    null_far[2] = (unsigned char)((0x1FC7 + PORT_LOAD_SEG) & 0xFF);
    null_far[3] = (unsigned char)((0x1FC7 + PORT_LOAD_SEG) >> 8);
}

void *port_null_near(const char *file, int line)
{
    nulls();
    fprintf(stderr, "uw2port: near null pointer read at %s:%d\n", file, line);
    return null_near;
}

void *port_null_far(const char *file, int line)
{
    nulls();
    fprintf(stderr, "uw2port: far null pointer read at %s:%d\n", file, line);
    return null_far;
}

/* The copies themselves, for the state dump's NULL section (src/replay/REPLAY.C): DS:0 on
   (far_table 0) or the vector table (1). */
unsigned char *port_null_copy(int far_table)
{
    nulls();
    return far_table ? null_far : null_near;
}
