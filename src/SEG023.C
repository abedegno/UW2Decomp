/* target: seg023 */
/* opts: -mm -1 -G -O -d */
/* Palette colour cycling: DOS resident segment seg023, in original order. Function and
   global names are the originals from the FM Towns symbol table where it has them. */

extern unsigned char far *palette;          /* DS:21AC, the 256 RGB triples */
static unsigned char last_phase = 0;            /* DS:354 */
static unsigned char last_half = 0;            /* DS:355 */
static unsigned char saved[3];              /* DS:24AC */

void far local_do_palette(int count, unsigned char first);

/* Rotate COUNT palette entries starting at FIRST by one place, upward when UP is set. */
void far rotate_bank(unsigned char first, unsigned char count, unsigned char up)
{
    unsigned char far *p;
    int j;
    int step;
    int i;

    p = palette + first * 3;
    step = up ? 3 : -3;
    if (up)
        p += (count - 1) * 3;
    saved[0] = p[0];
    saved[1] = p[1];
    saved[2] = p[2];
    for (i = 0; i < count - 1; i++, p -= step)
        for (j = 0; j < 3; j++)
            p[j] = (p - step)[j];
    p[0] = saved[0];
    p[1] = saved[1];
    p[2] = saved[2];
}

void far cycle_colors(unsigned char t)
{
    if ((t >> 5) == last_phase)
        return;
    last_phase = t >> 5;
    if ((last_phase >> 1) == last_half)
        return;
    rotate_bank(0xE0, 4, 0);
    rotate_bank(0xE4, 4, 0);
    rotate_bank(0xE8, 4, 0);
    rotate_bank(0xEC, 4, 0);
    local_do_palette(0x10, 0xE0);
    rotate_bank(3, 5, 1);
    rotate_bank(8, 3, 1);
    local_do_palette(8, 3);
    last_half = last_phase >> 1;
}
