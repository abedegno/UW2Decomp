/* target: seg045 */
/* opts: -mm -Y -d */
/* The label table of the 3D view's render database: the whole of UW1's resident
   segment seg045 (UW2's seg045_379C), in original order. Despite this file's place
   under conv/, it has nothing to do with conversations: the render database is the word
   bytecode that VIEW3D.C, GRIDDB.C and DRAWOBJ.C write through dbptr into the buffer at
   cDbase and that cRender runs through the model interpreter. This file lets that
   bytecode jump to places not yet written.

   Entry points: grdb_blank resets the table and points dbptr at cDbase; gr_tostrt moves
   dbptr to cEntryStrt and gr_entry stores dbptr's offset at *cDbbase (init_3d in
   VIEW3D.C calls the three in that order); grdb_size is the number of bytes written; Ref
   emits the word of a jump to a label; gr_putlab places a label at dbptr and patches the
   references already emitted; gr_getpre, gr_getlab and gr_freelab hand out label numbers
   and take them back; Clk gives the offset of a word in the table 0x18 bytes before
   cDbase. In UW1 nothing calls grdb_size, gr_getpre, gr_getlab or gr_freelab (IDA made
   no procedure of them; the target table counts each into the function before it).

   Labels 0..31 are the free labels gr_getlab hands out, each with up to 16 references
   waiting for gr_putlab; 32..95 are the ones gr_getpre hands out in sequence; 96..159
   are read back from the table just before the bytecode (word label - 172 counted from
   cDbase, a byte offset, halved to words, -1 for none). Label 0xA0 is the one pending
   relative reference (lstpos, lstrel), which Ref(0xA0, rel) opens and gr_putlab(0xA0)
   closes. A reference is stored as a byte offset from the word after it,
   (target - position - 1) * 2. Running out of labels or reference slots ends the program
   with exit(-20).

   UW1 against UW2: the table before the bytecode is 0x18 bytes (UW2 0x24) and the
   preset labels sit 172 words before it (UW2 178); Ref advances dbptr in each branch
   rather than once after them. The code is otherwise UW2's.

   Switches: no -1, -G or -O, which the bytes prove (`mov ax,0FFECh; push ax` and
   `pop cx` around exit, `mov sp,bp` rather than `leave`, the `jmp short $+2` before each
   value return, grdb_blank's counter on the stack). Standard frames on the argument-less
   functions rule out UW2's -k-. -Y and -d are kept from the project default: the file
   has no data and no function address for them to show in.

   Data: the label tables below; dbptr, the output pointer, is the one global other files
   use.

   Name: UW2's (map/filenames.tsv: the grdb_ prefix of grdb_blank and grdb_size). */
/* name: The names are UW2's FM Towns ones, the routines being the same; symbols.tsv has
   grdb_blank, gr_entry, gr_tostrt and gr_putlab from VIEW3D.C. The target table's
   reset_db for seg045_10B is a kin false hit (VIEW3D.C's reset_db has the same bytes
   with another global): VIEW3D.C calls it second of the three, as gr_tostrt. */

#include <dos.h>
#include <stdlib.h>
#include "sys.h"

/* match: This file's _BSS, DS:742A..79B6, laid out by name (tools/bssorder.py). */
int16 lstrel;                           /* DS:742A */
unsigned char prelblnum;                /* DS:742C */
uint16 lstpos;                          /* DS:742E */
unsigned char freelblptr;               /* DS:7430 */
unsigned char refs[32];                 /* DS:7431, references per label */
int16 far *dbptr;                       /* DS:7452, output pointer */
uint16 lblrefs[32][16];                 /* DS:7456, reference positions */
uint16 loc[160];                        /* DS:7856, label positions */
unsigned char freelbls[32];             /* DS:7996, free label stack */

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
        if (*(dbptr + i - 172) != -1)
            loc[i] = *(dbptr + i - 172) >> 1;
        else
            loc[i] = -1;
    }
    freelblptr = 31;
    prelblnum = 32;
}

/* The bytes written so far. Not called in UW1. */
int far grdb_size(void)
{
    return (dbptr - cDbase) * 2;
}

/* The offset of word n of the table 0x18 bytes before the bytecode buffer. */
/* match: Written as a near pointer from the buffer's offset: an int subtraction is kept
   as `sub ax,18h` where far pointer arithmetic would fold it into one `add` after the
   index. */
int far Clk(int n)
{
    return (int)(NEARPTR)((int16 *)(NEARPTR)(FP_OFF(cDbase) - 0x18) + n);
}

/* Stores dbptr's offset as the database's entry point (*cDbbase). */
void far gr_entry(void)
{
    *cDbbase = FP_OFF(dbptr);
}

/* Moves dbptr to cEntryStrt, where the bytecode starts. */
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
        dbptr++;
    } else if (loc[lab] == (uint16)-1) {
        if (refs[lab] == 16 || lab >= 32 || lab < 0)
            exit(-20);
        lblrefs[lab][refs[lab]] = dbptr - cDbase;
        refs[lab] = refs[lab] + 1;
        *dbptr = 0;
        dbptr++;
    } else {
        *dbptr = (loc[lab] - (dbptr - cDbase) - 1) * 2;
        dbptr++;
    }
}

/* The next of labels 32..95, in sequence. Not called in UW1. */
unsigned char far gr_getpre(void)
{
    if (prelblnum == 0x60)
        exit(-20);
    return prelblnum++;
}

/* A free label from the stack (0..31). Not called in UW1. freelblptr is unsigned, so
   the test for an empty stack can never be true. */
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

/* Forgets label lab's place and, for a free label, pushes it back. Not called in UW1; the
   push is bounded at 0x5F, not at the 32 entries of freelbls. */
void far gr_freelab(register unsigned char lab)
{
    loc[lab] = -1;
    if (lab < 32) {
        refs[lab] = 0;
        if (freelblptr < 0x5F)
            freelbls[++freelblptr] = lab;
    }
}
