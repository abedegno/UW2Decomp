/* target: seg045_379C */
/* opts: -mm -1 -G -O -d -k- */
/* The label table of the conversation (babl) bytecode assembler: grdb_blank clears it,
   Ref emits a reference to a label, gr_putlab places one and patches the references
   already emitted, gr_getpre, gr_getlab and gr_freelab hand out label numbers.

   Names are the FM Towns ones (tools/fmt.py): every function, and the globals, matched
   by what the FM code does with them (grdb_blank stores cDbase into dbptr, gr_entry
   writes dbptr through cDbbase, gr_tostrt loads dbptr from cEntryStrt, and so on).

   Labels 0..31 are the free labels gr_getlab hands out, each with up to 16 references
   waiting for gr_putlab; 32..95 are the ones gr_getpre hands out in sequence; 96..159
   are read back from the table just before the bytecode. Label 0xA0 is the one pending
   relative reference (lstpos, lstrel). */

#include <stdlib.h>
#include "sys.h"

/* This file's _BSS, DS:86F8..8C84, laid out by name (tools/bssorder.py). */
int lstrel;                             /* DS:86F8 */
unsigned char prelblnum;                /* DS:86FA */
unsigned lstpos;                        /* DS:86FC */
unsigned char freelblptr;               /* DS:86FE */
unsigned char refs[32];                 /* DS:86FF, references per label */
int far *dbptr;                         /* DS:8720, output pointer */
unsigned lblrefs[32][16];               /* DS:8724, reference positions */
unsigned loc[160];                      /* DS:8B24, label positions */
unsigned char freelbls[32];             /* DS:8C64, free label stack */

void far grdb_blank(void)
{
    int i;

    dbptr = cDbase;
    for (i = 0; i < 32; i++) {
        loc[i] = -1;
        freelbls[i] = i;
        lblrefs[i][0] = -1;
        refs[i] = 0;
    }
    for (; i < 96; i++)
        loc[i] = -1;
    for (; i < 160; i++) {
        if (*(dbptr + i - 178) != -1)
            loc[i] = *(dbptr + i - 178) >> 1;
        else
            loc[i] = -1;
    }
    freelblptr = 31;
    prelblnum = 32;
}

int far grdb_size(void)
{
    return (dbptr - cDbase) * 2;
}

/* The offset of word n of the table 0x24 bytes before the bytecode buffer. Written as a
   near pointer from the buffer's offset: an int subtraction is kept as `sub ax,24h`
   where far pointer arithmetic would fold it into one `add` after the index. */
int far Clk(int n)
{
    return (int)((int *)((unsigned)cDbase - 0x24) + n);
}

void far gr_entry(void)
{
    *cDbbase = (int)dbptr;
}

void far gr_tostrt(void)
{
    dbptr = cEntryStrt;
}

void far Ref(unsigned char lab, int rel)
{
    if (lab == 0xA0) {
        lstpos = dbptr - cDbase;
        lstrel = rel;
        *dbptr = 0;
    } else if (loc[lab] == -1) {
        if (refs[lab] == 16 || lab >= 32 || lab < 0)
            exit(-20);
        lblrefs[lab][refs[lab]] = dbptr - cDbase;
        refs[lab] = refs[lab] + 1;
        *dbptr = 0;
    } else {
        *dbptr = (loc[lab] - (dbptr - cDbase) - 1) * 2;
    }
    dbptr++;
}

unsigned char far gr_getpre(void)
{
    if (prelblnum == 0x60)
        exit(-20);
    return prelblnum++;
}

unsigned char far gr_getlab(void)
{
    if (freelblptr < 0)
        exit(-20);
    return freelbls[freelblptr--];
}

void far gr_putlab(unsigned char lab)
{
    register int i;

    if (lab == 0xA0) {
        cDbase[lstpos] = ((dbptr - cDbase) - lstpos - lstrel) * 2;
    } else {
        loc[lab] = dbptr - cDbase;
        if (lab < 32)
            for (i = 0; i < refs[lab]; i++)
                cDbase[lblrefs[lab][i]] = (loc[lab] - lblrefs[lab][i] - 1) * 2;
    }
}

void far gr_freelab(register unsigned char lab)
{
    loc[lab] = -1;
    if (lab < 32) {
        refs[lab] = 0;
        if (freelblptr < 0x5F)
            freelbls[++freelblptr] = lab;
    }
}
