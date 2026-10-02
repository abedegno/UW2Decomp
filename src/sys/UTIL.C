/* target: seg041_35D7 */
/* opts: -mm -1 -G -O -Y -d */
/* Small helpers: stepping a value within a limit, moving a point along a heading, waiting
   a number of ticks, and rolling dice. The whole of DOS resident segment seg041_35D7, in
   original order. Function names are the originals from the FM Towns symbol table where
   it has them. Callers are spread across the game: mvcheck by STATS.C and PLAYER.C,
   move_along by combat, missiles, spells, physics, skills and world events, rollem by
   combat, damage, spells, treasure and character creation.
   move_along uses the table sines of cFstSinCos (C3DENTRY.ASM, IMATH.ASM); the wait reads
   the tick counter of TICKS.ASM through Time.
   name: descriptive (small helpers). */

#include <stdlib.h>
#include "sys.h"


/* Steps *val by step in direction dir (-1 or 1) unless that passes limit; returns whether
   it moved. */
int far mvcheck(int *val, int limit, int step, int dir)
{
    int ok;

    ok = dir == -1 ? *val - step >= limit : *val + step <= limit;
    if (!ok)
        return 0;
    *val += dir * step;
    return 1;
}

/* Moves (*x, *y) dist along heading, a byte angle (256 to the turn). 0x140 - heading
   turns the game's compass heading into the angle of the sine table (a quarter turn
   further round and the other way), and the high byte form (<< 8) is what cFstSinCos
   indexes by. The sine and cosine (+-7FFFh) are scaled to dist and each nonzero step is
   rounded away from zero, so a move is never lost to truncation. */
void far move_along(int heading, int dist, int *x, int *y)
{
    int dy;
    int dx;

    heading = (0x140 - heading) & 0xFF;
    heading = heading << 8;
    cFstSinCos(heading, &dy, &dx);
    dy = dy / 0x80;
    dy = dy * dist;
    dy = dy / 0x100;
    dx = dx / 0x80;
    dx = dx * dist;
    dx = dx / 0x100;
    if (dy > 0)
        dy++;
    else if (dy < 0)
        dy--;
    if (dx > 0)
        dx++;
    else if (dx < 0)
        dx--;
    *y += dy;
    *x += dx;
}

/* DOS only: FM Towns has nothing between move_along_ and rollem_, and nothing in DOS calls
   it. Waits until ticks more have passed. */
void far seg041_35D7_E9(unsigned ticks)
{
    unsigned long start;

    start = *Time;
    while (*Time < start + ticks)
        ;
}

/* Rolls dice dice of sides sides; each die counts 1 to sides, so the total starts at dice. */
int far rollem(int dice, register int sides)
{
    register int total = dice;

    if (sides > 0 && dice > 0)
        while (dice--)
            total += (int)(((long)rand() * sides) / 0x8000L);
    return total;
}
