/* target: ovr151 */
/* opts: -mm -1 -G -O -Y -d */
/* The event scheduler: the schedules in SCD.ARK, their clocks, and running the rows that
   fall due. The whole of DOS overlay ovr151, in original order.

   What it does in the game: SCD.ARK (archive 5, in the save directory) has 16 blocks, one
   schedule per X clock (player->xclock[], player.h's XC_*). A block is a list of 16-byte
   rows (struct SCDRow, event.h) sorted by time, each an event for SCDEVENT.C's
   Sched_DoEvent, and a table of 80 clocks, one per level, each holding the time that level
   has reached on this schedule and the next row it has not run. Bringing a schedule up to
   date runs every row whose time has been passed, for the current level only (Sched_DoEvent
   checks the row's level), so a level the player is away from catches up when he comes back.
   Sched_SetAllClocks walks all 16 blocks and sets each clock from its X clock: block 0 is the
   time of day (XC_TIME, 72 steps, wrapping), the others are the plot counters (castle,
   gems, djinn ...). It runs after conversations (CONVERSE.C), level changes and restores
   (GAMEWRAP.C), and X clock changes from traps and world events (TRIGGER.C, WORLDEV.C);
   PLAYTIME.C advances the day clock.

   A row that moves an NPC to another schedule is not inserted at once but queued
   (Sched_Migrate, up to 16), and do_migrations adds the queue to block 0 when the block in
   hand is saved.

   Data owned: the work area pointer (in the workspace, Sched_SetBuf) and the modified flag.
   Function names are FM Towns originals where it has them (Sched_* are); the two static
   helpers keep IDA's names.
   Name: inferred (the event scheduler: the Sched_ prefix, and System Shock's scheduler is
   SCHEDULE.C). */
#include <dos.h>
#include <string.h>
#include "event.h"
#include "file.h"
#include "player.h"
#include "sys.h"

/* name: FM Towns names of the six IDA-labelled entry points. Keep the DOS public
   spellings through aliases until targets/ovr151.tsv is updated. The routines
   occur between the same neighbours and have the same behaviour in both builds. */

/* One level's place in a schedule: the time it has reached and the next row to run. */
struct SCDClock { unsigned time, next; };
/* The work area: the migration queue, then the block as stored in SCD.ARK (row count,
   block number, the 80 clocks, the rows). */
struct SCDWork {
    int migrations;
    struct SCDRow migrationRecord[16];
    unsigned rows;
    unsigned char block;
    char spare;
    struct SCDClock clocks[80];
    struct SCDRow record[1];
};
/* This file's _BSS, DS:8634..8637: the schedule work area, which Sched_SetBuf sets;
   ovr113 reads it too. */
/* match: ovr147's _BSS ends at 8634; inanmMapX (key 41) after it starts another run. */
struct SCDWork far *SCD_dseg_67d6_8634;
/* This file's _DATA, DS:1A7E..1AAA: scdBlockHasBeenModified (the block in hand must be
   written back), then the string pool, which holds only two debugging formats that no
   code reads. */
/* match: ovr150's DataDirectory ends at 1A7D, so this file starts at 1A7E; ovr152's data
   starts at 1AAC. */
unsigned char scdBlockHasBeenModified_dseg_67d6_1A7E = 0;
int far printf(const char *format, ...);
unsigned far get_arc(int, unsigned, char far *);
unsigned char far put_arc(int, unsigned, char far *, unsigned);
void far close_arc(int);
unsigned far get_workspace(void);
void far movedata(unsigned, unsigned, unsigned, unsigned, unsigned);

/* Runs (mode set) or skips the rows from the level's next row up to its clock time.
   Sched_DoEvent's result: 0 the row ran and changed the block, 4 the row deleted itself (so
   the same index is looked at again), 5 nothing to do; anything else stops here and is
   returned. Block 15 is rescanned from row 0 each time. */
static unsigned char far FindSCDRowsToExecute_ovr151_0(unsigned char mode)
{
    unsigned char result;
    register int i;
    i = SCD_dseg_67d6_8634->clocks[PlayerLevel].next;
    if (SCD_dseg_67d6_8634->block != 15) ;
    else i = 0;
    while (i < SCD_dseg_67d6_8634->rows) {
        if (SCD_dseg_67d6_8634->record[i].time > SCD_dseg_67d6_8634->clocks[PlayerLevel].time) break;
        if (mode) {
            result = Sched_DoEvent((unsigned char far *)&SCD_dseg_67d6_8634->record[i]);
            switch (result) {
            case 4: i--;
            case 0: scdBlockHasBeenModified_dseg_67d6_1A7E = 1; break;
            case 5: break;
            default:
                SCD_dseg_67d6_8634->clocks[PlayerLevel].next = i;
                return result;
            }
        }
        i++;
    }
    SCD_dseg_67d6_8634->clocks[PlayerLevel].next = i;
    if (SCD_dseg_67d6_8634->block == 15) i = 0;
    return 0;
}

/* The clock went back without running: moves the level's next row back to the first row
   after the clock time. */
static unsigned char far SetSomeValuesInSCDRows_ovr151_D3(unsigned char mode)
{
    register int i = SCD_dseg_67d6_8634->clocks[PlayerLevel].next;
    while (i > 0) {
        if (SCD_dseg_67d6_8634->record[i].time <= SCD_dseg_67d6_8634->clocks[PlayerLevel].time) { i++; break; }
        i--;
    }
    SCD_dseg_67d6_8634->clocks[PlayerLevel].next = i;
    return 0;
}

/* Inserts the queued migration rows into block 0 (loading it first if another block is
   in hand) and saves it. */
unsigned char far do_migrations(void)
{
    unsigned char result;
    register int i;
    if (SCD_dseg_67d6_8634->migrations == 0) return 0;
    if (SCD_dseg_67d6_8634->block) {
        result = Sched_Load(0);
        if (result) return result;
    }
    for (i = 0; i < SCD_dseg_67d6_8634->migrations; i++) {
        result = Sched_Insert(&SCD_dseg_67d6_8634->migrationRecord[i], 0);
        if (result) return result;
    }
    SCD_dseg_67d6_8634->migrations = 0;
    Sched_Save(0);
}

void far Sched_SetBuf(int ofs, int seg)
{
    SCD_dseg_67d6_8634 = MK_FP(seg, ofs);
    SCD_dseg_67d6_8634->migrations = 0;
}

/* Reads schedule block from the save directory's SCD.ARK into the work area. 0 on success. */
unsigned char far Sched_Load(unsigned char block)
{
    register int ok = open_arc(5, HomeDir);
    if (ok) {
        get_arc(5, block, (char far *)&SCD_dseg_67d6_8634->rows);
        close_arc(5);
        SCD_dseg_67d6_8634->block = block;
        scdBlockHasBeenModified_dseg_67d6_1A7E = 0;
        return 0;
    }
    return 1;
}

/* Writes the block in hand back to SCD.ARK if save is set (16 bytes a row plus the 0x144
   byte header), then performs any queued migrations. 0 on success. */
unsigned char far Sched_Should_Save(unsigned char block, unsigned char save)
{
    register int ok;
    if (!save) return 0;
    ok = open_arc(5, HomeDir);
    if (ok) {
        ok &= put_arc(5, block, (char far *)&SCD_dseg_67d6_8634->rows,
                      (SCD_dseg_67d6_8634->rows << 4) + 0x144);
        close_arc(5);
        if (ok) {
            do_migrations();
            scdBlockHasBeenModified_dseg_67d6_1A7E = 0;
            return 0;
        }
    }
    return 1;
}

unsigned char far Sched_Save(unsigned char block)
{
    return Sched_Should_Save(block, scdBlockHasBeenModified_dseg_67d6_1A7E);
}

unsigned char far Sched_GetTime(unsigned *out)
{
    *out = SCD_dseg_67d6_8634->clocks[PlayerLevel].time;
    return 0;
}

/* Sets the current level's clock on the block in hand to time and, with mode set, runs the
   rows that fall due. A clock going back restarts from row 0 when running, otherwise only
   repositions. */
unsigned char far Sched_SetTime(register unsigned time, unsigned char mode)
{
    unsigned char result;
    register unsigned old;
    old = SCD_dseg_67d6_8634->clocks[PlayerLevel].time;
    SCD_dseg_67d6_8634->clocks[PlayerLevel].time = time;
    if (old == time) return 0;
    if (old > time) {
        if (mode) SCD_dseg_67d6_8634->clocks[PlayerLevel].next = 0;
        else { result = SetSomeValuesInSCDRows_ovr151_D3(mode); goto done; }
    }
    result = FindSCDRowsToExecute_ovr151_0(mode);
done:
    return result;
}

/* Sets a cyclic clock (the day clock, span 72): time is taken modulo span, and a clock
   that has to wrap first runs to the end of the cycle and then from 0. */
unsigned char far Sched_WrapTime(register unsigned time, unsigned span, unsigned char mode)
{
    unsigned char result;
    register unsigned level = PlayerLevel;
    time %= span;
    if (SCD_dseg_67d6_8634->clocks[level].time > time) {
        result = Sched_SetTime(span - 1, mode);
        if (result) return result;
        SCD_dseg_67d6_8634->clocks[level].time = 0;
        SCD_dseg_67d6_8634->clocks[level].next = 0;
    }
    return Sched_SetTime(time, mode);
}

/* Brings all 16 schedules up to date for the current level: each block is loaded, its
   clock set from player->xclock[block] (block 0 wrapping at 72) and, if changed, saved. Uses
   the workspace; returns 3 if it is not free, else the first error. */
unsigned char far Sched_SetAllClocks(unsigned char mode)
{
    unsigned char block, error;
    register unsigned clock;
    error = 0;
    if (!get_workspace()) return 3;
    Sched_SetBuf(0, set_workspace());
    for (block = 0; block < 16 && error == 0; block++) {
        clock = player->xclock[block];
        if ((error = Sched_Load(block)) != 0) break;
        if (block == 0) error = Sched_WrapTime(player->xclock[XC_TIME], 72, mode);
        else error = Sched_SetTime(clock, mode);
        if (error) break;
        error = Sched_Save(block);
        if (error) ;
    }
    release_workspace();
    return error;
}

unsigned char far Sched_IncrTime(unsigned n, unsigned char mode)
{
    return Sched_SetTime(SCD_dseg_67d6_8634->clocks[PlayerLevel].time + n, mode);
}

/* Inserts count rows into the block in hand, in time order after rows of the same time.
   The index fix-up looks odd: for every one of the 80 clocks that is past the new row's
   time it bumps the current level's next index, not that clock's own (a slip in the
   original, probably). With run set, rows whose time has already
   passed for this level are run at once. */
unsigned char far Sched_InsertLong(int count, struct SCDRow far *src, unsigned char run)
{
    union { int tmp; struct { unsigned char lo, result; } b; } v;
    register int i, j;
    if (count < 1) return 0;
    for (i = SCD_dseg_67d6_8634->rows - 1; i >= 0; i--) {
        v.tmp = SCD_dseg_67d6_8634->record[i].time;
        /* match: compiled-out debugging: Turbo C drops the call but keeps the strings
           (DS:1A7F, 1A9B); where in the file they were is not known, only their order */
        if (0) printf("i = %d comparison: %d < %d\n", i, src->time, v.tmp);
        if (0) printf("looking at %lx\n", (long)i);
        if (src->time >= v.tmp) break;
        SCD_dseg_67d6_8634->record[i + count] = SCD_dseg_67d6_8634->record[i];
    }
    i++;
    for (j = 0; j < count; j++) {
        SCD_dseg_67d6_8634->record[i+j] = src[j];
        if (SCD_dseg_67d6_8634->record[i+j].time == src[j].time) ;
    }
    SCD_dseg_67d6_8634->rows += count;
    for (i = 0; i < 80; i++) {
        if (SCD_dseg_67d6_8634->clocks[i].time > src->time)
            SCD_dseg_67d6_8634->clocks[PlayerLevel].next++;
    }
    if (run && SCD_dseg_67d6_8634->clocks[PlayerLevel].time > src->time) {
        for (i = 0; i < count; i++) {
            v.b.result = Sched_DoEvent((unsigned char far *)&src[i]);
            if (v.b.result && v.b.result != 4) return v.b.result;
        }
    }
    return 0;
}

/* Removes a row from the block in hand and fixes the clocks' next indexes. */
unsigned char far Sched_Delete(struct SCDRow far *row)
{
    register int i;
    register int n = row - &SCD_dseg_67d6_8634->record[0];
    movedata(FP_SEG((char far *)row + 16), FP_OFF((char far *)row + 16), FP_SEG(row), FP_OFF(row),
             (SCD_dseg_67d6_8634->rows - n - 1) << 4);
    for (i = 0; i < 80; i++)
        if (SCD_dseg_67d6_8634->clocks[i].next > n) SCD_dseg_67d6_8634->clocks[i].next--;
    SCD_dseg_67d6_8634->rows--;
    return 0;
}

/* Queues a row for block 0 (do_migrations); returns 2 when the queue of 16 is full. */
unsigned char far Sched_Migrate(struct SCDRow far *row)
{
    if (SCD_dseg_67d6_8634->migrations >= 16) return 2;
    SCD_dseg_67d6_8634->migrationRecord[SCD_dseg_67d6_8634->migrations++] = *row;
    return 0;
}

unsigned char far Sched_Insert(struct SCDRow far *row, unsigned char run)
{
    return Sched_InsertLong(1, row, run);
}

/* The number of the block in hand, which is the X clock it follows. */
unsigned char far Sched_GetClock(void)
{
    return SCD_dseg_67d6_8634->block;
}
