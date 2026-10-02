/* target: ovr151 */
/* opts: -mm -1 -G -O -Y -d */
#include <dos.h>
#include <string.h>
#include "event.h"
#include "file.h"
#include "player.h"
#include "sys.h"

/* FM Towns names of the six IDA-labelled entry points. Keep the DOS public
   spellings through aliases until targets/ovr151.tsv is updated. The routines
   occur between the same neighbours and have the same behaviour in both builds. */

struct SCDClock { unsigned time, next; };
struct SCDWork {
    int migrations;
    struct SCDRow migrationRecord[16];
    unsigned rows;
    unsigned char block;
    char spare;
    struct SCDClock clocks[80];
    struct SCDRow record[1];
};
/* This file's _BSS, DS:8634..8637 (ovr147's ends at 8634): the schedule work area, which
   Sched_SetBuf sets; ovr113 reads it too. inanmMapX (key 41) after it starts another run. */
struct SCDWork far *SCD_dseg_67d6_8634;
/* This file's _DATA, DS:1A7E..1AAA: scdBlockHasBeenModified, then the string pool, which
   holds only two debugging formats that no code reads. ovr150's DataDirectory ends at 1A7D,
   so this file starts at 1A7E; ovr152's data starts at 1AAC. */
unsigned char scdBlockHasBeenModified_dseg_67d6_1A7E = 0;
int far printf(const char *format, ...);
unsigned far get_arc(int, unsigned, char far *);
unsigned char far put_arc(int, unsigned, char far *, unsigned);
void far close_arc(int);
unsigned far get_workspace(void);
void far movedata(unsigned, unsigned, unsigned, unsigned, unsigned);

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
        result = Sched_Insert((struct SCDRow far *)((unsigned char far *)&SCD_dseg_67d6_8634->migrations + i * 16 + 2), 0);
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

unsigned char far Sched_Load(unsigned char block)
{
    register int ok = open_arc(5, HomeDir);
    if (ok) {
        get_arc(5, block, (char far *)SCD_dseg_67d6_8634 + 0x102);
        close_arc(5);
        SCD_dseg_67d6_8634->block = block;
        scdBlockHasBeenModified_dseg_67d6_1A7E = 0;
        return 0;
    }
    return 1;
}

unsigned char far Sched_Should_Save(unsigned char block, unsigned char save)
{
    register int ok;
    if (!save) return 0;
    ok = open_arc(5, HomeDir);
    if (ok) {
        ok &= put_arc(5, block, (char far *)SCD_dseg_67d6_8634 + 0x102,
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

unsigned char far Sched_InsertLong(int count, struct SCDRow far *src, unsigned char run)
{
    union { int tmp; struct { unsigned char lo, result; } b; } v;
    register int i, j;
    if (count < 1) return 0;
    for (i = SCD_dseg_67d6_8634->rows - 1; i >= 0; i--) {
        v.tmp = SCD_dseg_67d6_8634->record[i].time;
        /* compiled-out debugging: Turbo C drops the call but keeps the strings (DS:1A7F,
           1A9B); where in the file they were is not known, only their order */
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

unsigned char far Sched_GetClock(void)
{
    return SCD_dseg_67d6_8634->block;
}
