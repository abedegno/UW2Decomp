/* target: ovr154 */
/* opts: -mm -1 -G -O -Y -d */
/* A grab bag, the whole of UW1's DOS overlay ovr154 (UW2's ovr167), in original order:
   directions (mpos turns a vector into one of eight directions; the compass helpers), the
   "it is to the north" messages of print_path_to, the start-up checks (check_dirs,
   memcheck, check_fds), whole-file reads and writes, the DOS read and write calls for
   far buffers that every file loader uses, the PLAYER.DAT cipher (xorread, xorwrite) and
   opening files in the data directory (data_fopen). It owns no data but its strings.
   Seeded from UW2Decomp's src/sys/MISCUTIL.C.

   UW1 against UW2: no octant, get_theta or our_open; the PLAYER.DAT cipher is a plain
   XOR with a key that steps by 3 (UW2's build_xor_table and xor_xor_table chain each
   byte to the one before); errmsg says "Underworld internal error"; the memory check
   (memcheck) punts itself rather than answering; new: do_beep, a speaker beep, and
   dbg_break, which raises interrupt 2. print_path_to's direction strings start at 0x24
   and its level strings at 0x33 (UW2: 0x28, 0x37).

   name: UW2's names (the FM Towns symbol table, or UW2Decomp's provisional ones) where
   the routine is the same. TLINK numbers an overlay's stub entries in ascending
   bssorder.py key of the publics' names, so the names must also keep the EXE's stub
   order (xorread 64, do_beep 76, postodir 184, mpos 229, xorwrite 280, print_path_to
   336, check_fds 451, unsolve_compass 461, flip_bool 542, solve_compass 587, check_dirs
   627, memcheck 645, intoFarBuffer_ovr167_5DA 649, blttodrive 658, data_fopen 780,
   bltfromdrive 786, dir_exist 828, errmsg 893, FarWrite_ovr167_627 918, dbg_break 940):
   the three new names (do_beep, memcheck, dbg_break) were chosen for their keys. */

#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <stat.h>
#include <string.h>
#include <stdlib.h>
#include <alloc.h>
#include <stdio.h>
#include "file.h"
#include "gfx.h"
#include "sys.h"
#include "ui.h"

void far flip_bool(char *p) { if (*p) *p = 0; else *p = 1; }

/* Which ninth of an a by b box (x, y) falls in, as a direction: the 3 by 3 grid's
   column q and row r give 0 to 7 round the edge and 8 for the centre. With
   unsolve_compass, its inverse, this probably served a clickable compass. */
int far solve_compass(int a, int b, int x, int y)
{
    register int q = ((x * 3) / a) % 3;
    register int r = ((y * 3) / b) % 3;
    switch (q) {
    case 2: return 3 - r;
    case 0: return r + 5;
    case 1: if (r == 1) return 8; return 4 - r * 2;
    }
    return 8;
}

/* The direction of the vector (x, y), 0 to 7: 0 is +y, 2 +x, 4 -y, 6 -x, and the odd
   values the diagonals between; a component counts as zero when it is less than half the
   other. */
int far mpos(char x, char y)
{
    if (abs(x) / 2 > abs(y)) {
        if (x > 0) return 2;
        return 6;
    } else if (abs(y) / 2 > abs(x)) {
        if (y > 0) return 0;
        return 4;
    } else if (x < 0) {
        return (y > 0) * 2 + 5;
    } else {
        return (y < 0) * 2 + 1;
    }
}

int far postodir(int x1, int y1, int x2, int y2)
{ return mpos((char)x2 - (char)x1, (char)y2 - (char)y1); }

/* Prints s and then where something is, to the message scroll: with radius < 0, the
   direction -radius - 1 given by the caller; otherwise, if (ox, oy) is further than
   radius tiles (Manhattan distance) from (px, py), the direction to it. Directions are
   game strings 0x24 + mpos. If `ignored` (in fact a level) is nonzero and differs from
   `previous`, game string 0x33 + ignored - previous follows after " and "; with no
   direction and a nonzero `ignored` it says "very near". */
void far print_path_to(char far *s, int px, int py, int ignored,
                                                   int ox, int oy, int previous, int radius)
{
    char shown;
    register int how = ignored;
    register int range = radius;
    shown = 0;
    scroll_print(s);
    if (range < 0) {
        game_sprint(0x24 - range - 1);
        shown = 1;
    } else if (abs(px - ox) + abs(py - oy) > range) {
        game_sprint(postodir(px, py, ox, oy) + 0x24);
        shown = 1;
    }
    if (how != previous && how) {
        if (shown) scroll_print(" and ");
        game_sprint(how + 0x33 - previous);
    } else if (!shown && how) scroll_print("very near");
    scroll_print(".\n");
}

/* Draws a 3 by 3 box of colour n on the compass of size x by y centred at (cx, cy), at
   the place of direction t: the inverse of solve_compass. */
void far unsolve_compass(int x, int y, int t, int cx, int cy, int n)
{
    register int xx = cx + x / 2;
    register int yy = cy + y / 2;
    if (t % 4 == 0) yy += ((y + 6) / 6) * (2 - t);
    else if (t % 2 == 0) xx += (x / 6) * (4 - t);
    else {
        yy += (y / 3) * (abs(t - 4) - 2);
        xx += (x / 3) * (1 - ((t >> 2) << 1));
    }
    set_the_color(n);
    box(xx - 1, yy - 1, xx + 1, yy + 1);
}

void far errmsg(char *a, char *b)
{
    scroll_print("Underworld internal error\n ");
    scroll_print(a);
    scroll_print(" ");
    scroll_print(b);
}

/* A beep of freq hertz for ms milliseconds on the PC speaker (AUTOMAP.C, when a note
   will take no more). name: chosen for its key. */
int far do_beep(int freq, int ms)
{
    sound(freq);
    delay(ms);
    nosound();
    return 0;
}

/* Raises interrupt 2 (the NMI). Nothing calls it here; probably a debugging aid.
   name: chosen for its key. */
void far dbg_break(void)
{
    union REGS r;
    int86(2, &r, &r);
}

char far dir_exist(char *name)
{
    struct stat st;
    if (stat(name, &st) == -1) return 0;
    if (st.st_mode & S_IFDIR) return 1;
    return 0;
}

/* Start-up check that the DATA, CRIT and CUTS directories exist; else first_punt with
   "Could not read data." (code 0x3001). */
void far check_dirs(void)
{
    char ok = 1;
    ok &= dir_exist("data");
    ok &= dir_exist("crit");
    ok &= dir_exist("cuts");
    if (!ok) first_punt(ERR_READ | 1);
}

/* Start-up check for at least 2200 bytes of near heap and 1500 bytes of far heap; else
   first_punt(0x1004). UW2's OkEnoughMem_ovr167_463 answers instead. name: chosen for its
   key. */
void far memcheck(void)
{
    if (coreleft() < 0x898) first_punt(0x1004);
    if (farcoreleft() < 0x5DCL) first_punt(0x1004);
}

/* Start-up check that eight files can be open at once (a.tmp to h.tmp are created and
   deleted), so that the FILES= setting is big enough; else first_punt(6). */
void far check_fds(void)
{
    char name[6] = "a.tmp";
    char ok = 1;
    int16 handles[8];
    register int i;
    for (i = 0; i < 8; i++) {
        ok &= (handles[i] = open(name, 0x304, 0x80)) != -1;
        name[0]++;
    }
    name[0] = 'a';
    for (i = 0; i < 8; i++) {
        if (handles[i] != -1) {
            close(handles[i]);
            unlink(name);
            name[0]++;
        }
    }
    if (!ok) first_punt(6);
}

/* Write n bytes from buf as the whole file `name` (blttodrive), or read n bytes of it
   into buf (bltfromdrive); 1 if all went well. */
unsigned char far blttodrive(void far *buf, char *name, unsigned n)
{
    unsigned char ok = 1;
    register int fd;
    fd = open(name, 0x8302, 0x80);
    if (fd < 0) return 0;
    if (FarWrite_ovr167_627(fd, buf, n) != n) ok = 0;
    if (close(fd)) ok = 0;
    return ok;
}

unsigned char far bltfromdrive(char *name, void far *buf, unsigned n)
{
    unsigned char ok = 1;
    register int fd;
    fd = open(name, 0x8001);
    if (fd < 0) return 0;
    if (intoFarBuffer_ovr167_5DA(fd, buf, n) != n) ok = 0;
    if (close(fd)) ok = 0;
    return ok;
}

/* read() and write() for a far buffer: DOS functions 3Fh and 40h through intdosx, with
   errno set and -1 returned on failure. The medium-model library's read and write take
   near buffers, so every loader that fills far or EMS memory goes through these. */
int far intoFarBuffer_ovr167_5DA(int fd, void far *buf, unsigned n)
{
    struct SREGS sregs;
    union REGS in;
    union REGS out;
    in.x.ax = 0x3F00;
    in.x.bx = fd;
    in.x.cx = n;
    in.x.dx = FP_OFF(buf);
    sregs.ds = FP_SEG(buf);
    intdosx(&in, &out, &sregs);
    if (out.x.cflag) { errno = out.x.ax; return -1; }
    return out.x.ax;
}
int far FarWrite_ovr167_627(int fd, void far *buf, unsigned n)
{
    struct SREGS sregs;
    union REGS in;
    union REGS out;
    in.x.ax = 0x4000;
    in.x.bx = fd;
    in.x.cx = n;
    in.x.dx = FP_OFF(buf);
    sregs.ds = FP_SEG(buf);
    intdosx(&in, &out, &sregs);
    if (out.x.cflag) { errno = out.x.ax; return -1; }
    return out.x.ax;
}

/* The PLAYER.DAT cipher: each byte XORed with an 80-byte key table, entry i being the
   seed (the file's first byte) plus 3 * (i + 1); the table restarts every 80 bytes. The
   same XOR decodes and encodes. */
int far xorread(int fd, unsigned char key, unsigned char far *buf, unsigned n)
{
    int count;
    int total;
    unsigned char encrypted[0x50];
    unsigned char table[0x50];
    register int i;
    register unsigned remaining = n;
    total = 0;
    for (i = 0; i < 0x50; i++) table[i] = key += 3;
    while (remaining + 0x50 > remaining) {
        count = intoFarBuffer_ovr167_5DA(fd, encrypted, remaining < 0x50 ? remaining : 0x50);
        for (i = 0; i < count; i++) buf[i] = encrypted[i] ^ table[i];
        total += count;
        remaining -= 0x50;
        buf += 0x50;
    }
    return total;
}
int far xorwrite(int fd, unsigned char key, unsigned char far *buf, unsigned n)
{
    int written;
    int total;
    unsigned char encrypted[0x50];
    unsigned char table[0x50];
    register int i;
    register unsigned remaining = n;
    total = 0;
    for (i = 0; i < 0x50; i++) table[i] = key += 3;
    while (remaining + 0x50 > remaining) {
        for (i = 0; i < (remaining < 0x50 ? remaining : 0x50); i++) encrypted[i] = table[i] ^ buf[i];
        written = FarWrite_ovr167_627(fd, encrypted, remaining < 0x50 ? remaining : 0x50);
        total += written;
        remaining -= 0x50;
        buf += 0x50;
    }
    return total;
}

/* Opens a file in DATA\ with stdio. */
FILE * far data_fopen(char *name, char *mode)
{
    char path[0x42];
    strcpy(path, "DATA\\");
    strcat(path, name);
    return fopen(path, mode);
}
