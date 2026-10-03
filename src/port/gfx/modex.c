/* modex.c: replaces part of src/gfx/MODEX.ASM (seg017): the palette loader local_do_palette,
   the console print, the far string helpers (mem_set, str_len, str_copy, str_ncopy,
   FindStringDelimiter, str_str, str_cmp), gr_pixel, the video memory pictures (2A2, 320, 361)
   and grab. Each keeps the assembly's exact results:
   str_ncopy copies at most n bytes and leaves no terminator when it stops short, and str_cmp
   compares bytes as unsigned and returns their difference. */
#include "compat.h"
#include "gfx.h"
#include "sys.h"
#include "grlib.h"

/* int 21h function 9: print up to the '$'. */
void PrintStringToConsole_seg017_DE(char *text)
{
    size_t n = 0;
    while (text[n] != '$') n++;
    fwrite(text, 1, n, stdout);
    fflush(stdout);
}

/* Waits for vertical retrace, then loads count DAC entries from the palette, from first. */
void local_do_palette(int count, unsigned char first)
{
    unsigned i, n;
    const unsigned char *src;
    while (!(vga_inb(INPUT_STATUS) & 8)) ;
    vga_outb(DAC_WRITE_INDEX, first);
    src = palette + first * 3;
    n = (uint16_t)((uint16_t)count * 3);
    for (i = 0; i < n; i++) vga_outb(0x3C9, src[i]);
}

void mem_set(void *p, int value, int count)
{
    uint16_t n = (uint16_t)count, i;
    /* rep stosb into the VGA's window writes video memory through the map mask */
    if (vga_in_window(p)) {
        for (i = 0; i < n; i++) port_vga_store((unsigned char *)p + i, (unsigned char)value);
        return;
    }
    memset(p, value & 0xFF, n);
}

int str_len(char *s)
{
    return (int)(uint16_t)strlen(s);
}

char *str_copy(char *dst, char *src)
{
    memmove(dst, src, strlen(src) + 1);
    return dst;
}

void str_ncopy(char *dst, char *src, int n)
{
    int16_t len = (int16_t)(strlen(src) + 1);
    if (len > (int16_t)n) len = (int16_t)n;
    memmove(dst, src, (uint16_t)len);
}

char *FindStringDelimiter(char *s, int c)
{
    return memchr(s, c & 0xFF, strlen(s) + 1);
}

char *str_str(char *s, char *find)
{
    return strstr(s, find);
}

int str_cmp(char *a, char *b)
{
    size_t n = strlen(a) + 1, i = 0;
    while (i < n && a[i] == b[i]) i++;
    if (i == n) i--;
    return (int)(unsigned char)a[i] - (int)(unsigned char)b[i];
}

/* gr_pixel: one pixel at screen line 199 - y, on the page the word at dseg_67d6_21F4 starts. */
extern uint16 *dseg_67d6_21F4;
void gr_pixel(int x, int y, int color)
{
    uint16_t at = (uint16_t)(*dseg_67d6_21F4 + (int16_t)(0x50 * (0xC7 - y)) + ((uint16_t)x >> 2));
    vga_outb(SC_INDEX, 2);
    vga_outb(SC_DATA, (uint8_t)(1 << (x & 3)));
    vga_write(at, (uint8_t)color);
}

/* grab(dst, x, y, w, h): a w by h block of the screen, its top row at line 199 - y, into dst
   as a linear w-wide image (PANELS.C keeps the panel it flips over this way, SCRSHOT.C takes
   its GIF's rows). The assembly reads a plane at a time through the graphics controller's read
   map and stores every fourth byte, in two passes that each walk down the rows for one plane
   and back up (with the direction flag set) for the next; this is that, register for
   register, so the odd strides and the plane order come out as in DOS. */
void grab(void *dstp, int x, int y, int w, int h)
{
    unsigned char *dst = dstp;
    uint16_t si, di = 0, bp, bx, cx, passes;
    uint8_t al, ah, cl, dl, dh;
    vga_outb(GC_INDEX, 4);                          /* read map select */
    si = (uint16_t)((uint16_t)(0xC7 - y) * 0x50u + *dseg_67d6_21F4);
    dl = (uint8_t)h;
    dh = (uint8_t)w;
    bx = (uint16_t)(0x50 - ((uint16_t)(w + 3) >> 2));
    si = (uint16_t)(si + ((uint16_t)x >> 2));
    al = (uint8_t)(x & 3);
    bp = bx;
    bx = (uint16_t)(dh & 3);
    if (bx) bx = (uint16_t)(bx - 4);
    for (passes = 2; passes; passes--) {
        vga_outb(GC_DATA, al);
        for (ah = dl; ah; ah--) {                   /* down the rows */
            cl = (uint8_t)((uint8_t)(dh + 3) >> 2);
            for (cx = cl; cx; cx--) {
                dst[di] = vga_read(si);
                si++;
                di = (uint16_t)(di + 1 + 3);
            }
            si = (uint16_t)(si + bp);
            di = (uint16_t)(di + bx);
        }
        al = (uint8_t)((al + 1) & 3);
        if (!al) si++;
        dh--;
        if (!(dh & 3)) {
            bp++;
            bx = (uint16_t)(bx + 4);
        }
        vga_outb(GC_DATA, al);
        di = (uint16_t)(di - 3 - bx);
        si = (uint16_t)(si - bp - 1);
        for (ah = dl; ah; ah--) {                   /* and back up, std */
            cl = (uint8_t)((uint8_t)(dh + 3) >> 2);
            for (cx = cl; cx; cx--) {
                dst[di] = vga_read(si);
                si--;
                di = (uint16_t)(di - 1 - 3);
            }
            si = (uint16_t)(si - bp);
            di = (uint16_t)(di - bx);
        }
        al = (uint8_t)((al + 1) & 3);
        if (!al) si++;
        dh--;
        si = (uint16_t)(si + bp + 1);
        di = (uint16_t)(di + 5 + bx);
        if (!(dh & 3)) {
            bp++;
            bx = (uint16_t)(bx + 4);
        }
    }
    vga_outw(GC_INDEX, 0xFF08);                     /* bit mask FF */
}

int seg001_023B_5A(int w, int h);

/* 2A2: a one-byte header at dst holding 4, w and h in planes 0, 1 and 2, then the w by h
   image after it, a plane at a time, each plane's rows packed one after another; returns dst. */
void DRAW_RELATED_seg017_2179_2A2(unsigned char *src, unsigned dst, int w, int h)
{
    uint16_t di = (uint16_t)dst, n, rows;
    const unsigned char *row;
    uint8_t al;
    vga_outb(SC_INDEX, 2);
    vga_outb(SC_DATA, 1);
    vga_write(di, 4);
    vga_outb(SC_DATA, 2);
    vga_write(di, (uint8_t)w);
    vga_outb(SC_DATA, 4);
    vga_write(di, (uint8_t)h);
    dst = (uint16_t)(dst + 1);
    for (al = 1; al < 0x10; al = (uint8_t)(al << 1), src++) {
        vga_outb(SC_DATA, al);
        di = (uint16_t)dst;
        row = src;
        for (rows = (uint16_t)h; rows; rows--, row += (uint16_t)w)
            for (n = (uint16_t)((uint16_t)(w + 3) >> 2); n; n--) vga_write(di++, row[(((uint16_t)((uint16_t)(w + 3) >> 2) - n)) * 4]);
    }
}

/* 320: the width and height in planes 1 and 2 of the header byte at offset. */
void DRAW_RELATED_seg017_2179_320(unsigned offset, int16 *width, int16 *height)
{
    vga_outb(GC_INDEX, 4);
    vga_outb(GC_DATA, 1);
    *width = vga_read((uint16_t)offset);
    vga_outb(GC_DATA, 2);
    *height = vga_read((uint16_t)offset);
    vga_outw(GC_INDEX, 0xFF08);
}

/* 361: video memory for n bytes (valloc's register entry, n by 1) and src copied into it a
   plane at a time; the offset, or 0. The map mask is set only before planes 1 to 3, so the
   first quarter goes to whichever planes it last enabled, as in DOS. */
int DRAW_RELATED_seg017_2179_361(unsigned char *src, unsigned n, int unused)
{
    uint16_t bx, dx, di, cx;
    uint8_t al;
    const unsigned char *s;
    (void)unused;
    vga_outb(SC_INDEX, 2);
    bx = (uint16_t)seg001_023B_5A((int)n, 1);
    if (!bx) return 0;
    dx = (uint16_t)((uint16_t)(n + 3) >> 2);
    for (al = 1;; ) {
        s = src;
        di = bx;
        for (cx = dx; cx; cx--, s += 4) vga_write(di++, *s);
        src++;
        al = (uint8_t)(al << 1);
        if (al >= 0x10) break;
        vga_outb(SC_DATA, al);
    }
    return bx;
}
