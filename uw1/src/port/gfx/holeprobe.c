/* holeprobe.c: a test switch, UW1PORT_HOLE_PROBE=N (Exhume's tools/enhcheck.py holes). The 3D view's
   frame buffer is cleared before each frame (GRENTRY.ASM's clear_fbuf, colour 0); with the switch
   it is cleared to the byte N instead, so a pixel still N after the frame is one no face covered,
   a hole (--enhance wide-pitch draws what is behind the player so that steep looks have fewer).
   After each frame establish_view reports the count, with the pitch, on stderr. */
#include <stdio.h>
#include <stdlib.h>

int port_hole_mark(void)
{
    static int v = -2;
    if (v == -2) {
        const char *e = getenv("UW1PORT_HOLE_PROBE");
        v = e ? (int)(strtol(e, NULL, 0) & 0xFF) : -1;
    }
    return v;
}

void port_hole_report(const unsigned char *fb, unsigned len, int pitch)
{
    unsigned i, n = 0;
    for (i = 2; i < len; i++)           /* the clear starts at offset 2 */
        if (fb[i] == port_hole_mark()) n++;
    fprintf(stderr, "hole-probe: pitch %d holes %u\n", pitch, n);
}
