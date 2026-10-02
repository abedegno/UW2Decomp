/* target: seg045_379C */
/* opts: -mm -1 -G -O -d -k- */
/* The label table of the 3D view's render database. Despite this file's place under
   conv/, it has nothing to do with conversations: the render database is the word
   bytecode that VIEW3D.C, GRIDDB.C and DRAWOBJ.C write through dbptr into the buffer at
   cDbase (sys.h, sys/C3DENTRY.ASM) and that cRender runs through the model interpreter
   (3d/INTERP.ASM). This file lets that bytecode jump to places not yet written.

   Entry points: grdb_blank resets the table and points dbptr at cDbase; gr_tostrt moves
   dbptr to cEntryStrt and gr_entry stores dbptr's offset at *cDbbase (init_3d in
   VIEW3D.C calls the three in that order); grdb_size is the number of bytes written; Ref
   emits the word of a jump to a label; gr_putlab places a label at dbptr and patches the
   references already emitted; gr_getpre, gr_getlab and gr_freelab hand out label numbers
   and take them back; Clk gives the offset of a word in the table 0x24 bytes before
   cDbase, which DRAWOBJ.C writes into the bytecode (`2, Clk(8), value`).

   Labels 0..31 are the free labels gr_getlab hands out, each with up to 16 references
   waiting for gr_putlab; 32..95 are the ones gr_getpre hands out in sequence; 96..159
   are read back from the table just before the bytecode (word label - 178 counted from
   cDbase, a byte offset, halved to words, -1 for none): DRAWOBJ.C's Ref(model + 0x60, 1)
   jumps to an object model this way. Label 0xA0 is the one pending relative reference
   (lstpos, lstrel), which GRIDDB.C opens with Ref(0xA0, 1) and VIEW3D.C closes with
   gr_putlab(0xA0). A reference is stored as a byte offset from the word after it,
   (target - position - 1) * 2. Running out of labels or reference slots ends the program
   with exit(-20).

   Data: the label tables below; dbptr, the output pointer, is the one global other files
   use.

   Name: inferred (map/filenames.tsv: the grdb_ prefix of grdb_blank and grdb_size). The
   tsv's evidence calls it the conversation bytecode assembler's label table, which is
   wrong: every caller is 3D code, and in FM Towns grdb_blank_ .. gr_freelab_ sit between
   draw_solid_tmap and cZoom_, cRender_, cInit3d_. */
/* name: Names are the FM Towns ones (tools/fmt.py): every function, and the globals,
   matched by what the FM code does with them (grdb_blank stores cDbase into dbptr,
   gr_entry writes dbptr through cDbbase, gr_tostrt loads dbptr from cEntryStrt, and so
   on). */

#include <dos.h>
#include <stdlib.h>
#include "sys.h"

/* match: This file's _BSS, DS:86F8..8C84, laid out by name (tools/bssorder.py). */
int16 lstrel;                           /* DS:86F8 */
unsigned char prelblnum;                /* DS:86FA */
uint16 lstpos;                          /* DS:86FC */
unsigned char freelblptr;               /* DS:86FE */
unsigned char refs[32];                 /* DS:86FF, references per label */
int16 far *dbptr;                       /* DS:8720, output pointer */
uint16 lblrefs[32][16];                 /* DS:8724, reference positions */
uint16 loc[160];                        /* DS:8B24, label positions */
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

/* The offset of word n of the table 0x24 bytes before the bytecode buffer. */
/* match: Written as a near pointer from the buffer's offset: an int subtraction is kept
   as `sub ax,24h` where far pointer arithmetic would fold it into one `add` after the
   index. */
int far Clk(int n)
{
    return (int)(NEARPTR)((int16 *)(NEARPTR)(FP_OFF(cDbase) - 0x24) + n);
}

void far gr_entry(void)
{
    *cDbbase = FP_OFF(dbptr);
}

void far gr_tostrt(void)
{
    dbptr = cEntryStrt;
}

/* Emit the jump word for label lab at dbptr. A placed label gets its byte offset now; an
   unplaced free label (0..31) gets 0 and is remembered for gr_putlab to patch; 0xA0
   remembers this position and rel, gr_putlab(0xA0) later storing the distance less rel. */
void far Ref(unsigned char lab, int rel)
{
    if (lab == 0xA0) {
        lstpos = dbptr - cDbase;
        lstrel = rel;
        *dbptr = 0;
    } else if (loc[lab] == (uint16)-1) {
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

/* Place label lab at dbptr and patch every reference already emitted to it. */
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
