/* target: ovr167 */
/* opts: -mm -1 -G -O -Y -d */
/* A grab bag, the whole of DOS overlay ovr167: directions (mpos and octant, which turn
   a vector into one of eight directions; the compass helpers; get_theta, a vector's
   angle), the "it is to the north" messages of print_path_to, the start-up checks
   (check_dirs, check_fds, OkEnoughMem_ovr167_463), whole-file reads and writes, the DOS
   read and write calls for far buffers that every file loader uses, the PLAYER.DAT
   cipher (build_xor_table .. xorwrite, used by PLAYDATA.C), and opening files in the
   data or save directory (data_fopen, our_open). It owns no data but its strings.
   name: descriptive (miscellaneous utilities). */

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

/* name: FM Towns names are used where the same operation is clear in its code:
   flip_bool toggles a byte; check_dirs checks the data directories; check_fds
   probes eight temporary files; blttodrive writes a buffer; build_xor_table
   makes the 80-byte key table; xor_xor_table transforms one block; xorread
   and xorwrite process successive 80-byte blocks. The rest of the FM Towns file
   lines up with DOS function by function, and each name's tools/bssorder.py key
   puts it where the EXE's overlay stub order needs it: solve_compass and
   unsolve_compass (the same arithmetic as IDA's ovr167_17 and ovr167_252),
   get_theta (cSqRt then cAtan2, IDA's DartSatelliteVectoring_ovr167_313),
   dir_exist (stat and S_IFDIR, IDA's ListFiles_ovr167_3F6), bltfromdrive (open,
   read, close, after blttodrive) and data_fopen (the data directory, strcat,
   fopen, just before our_open). FM Towns has nothing for the far read and write
   helpers, which it does with read_ and write_, or for the memory check; their
   names (intoFarBuffer_ovr167_5DA, FarWrite_ovr167_627, OkEnoughMem_ovr167_463)
   are provisional, chosen for their keys. */

void far flip_bool(char *p) { if (*p) *p = 0; else *p = 1; }

/* Which ninth of an a by b box (x, y) falls in, as a direction: the 3 by 3 grid's
   column q and row r give 0 to 7 round the edge and 8 for the centre. With
   unsolve_compass, its inverse, this probably served a clickable compass; nothing in the
   sources calls either. */
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
   other. Used by the detect-monster spell (SPELLS2.C) and world events. */
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

/* The octant of the vector (x, y), 0 to 7, split at the axes and the diagonals (unlike
   mpos, which centres its sectors on them). */
int far octant(char x, char y)
{
    if (y > 0) {
        if (abs(x) < y) return (x < 0) + 1;
        return (x < 0) * 3;
    } else {
        if (abs(x) < -y) return (x > 0) + 5;
        return (x > 0) * 3 + 4;
    }
}

int far postodir(int x1, int y1, int x2, int y2)
{ return mpos((char)x2 - (char)x1, (char)y2 - (char)y1); }

/* Prints s and then where something is, to the message scroll: with radius < 0, the
   direction -radius - 1 given by the caller; otherwise, if (ox, oy) is further than
   radius tiles (Manhattan distance) from (px, py), the direction to it. Directions are
   game strings 0x28 + mpos (presumably the eight direction phrases). If `ignored`
   (despite its name, it is used) is nonzero and differs from `previous`, game string
   0x37 + ignored - previous follows after " and "; with no direction and a nonzero
   `ignored` it says "very near". Used by the detect-monster spell and a trap
   (TRIGGER.C). */
void far print_path_to(char far *s, int px, int py, int ignored,
                                                   int ox, int oy, int previous, int radius)
{
    unsigned char shown;
    register int how = ignored;
    register int range = radius;
    shown = 0;
    scroll_print(s);
    if (range < 0) {
        game_sprint(0x28 - range - 1);
        shown = 1;
    } else if (abs(px - ox) + abs(py - oy) > range) {
        game_sprint(postodir(px, py, ox, oy) + 0x28);
        shown = 1;
    }
    if (how != previous && how) {
        if (shown) scroll_print(" and ");
        game_sprint(how + 0x37 - previous);
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

/* The angle of the vector from (sx, sy) to (x, y) as a 16-bit binary angle: the vector
   is scaled to length 7FFFh (cSqRt) and cAtan2 (IMATH.ASM) turns the unit vector into an
   angle from its arcsine or arccosine table. 0 for a zero vector. Used by PATHFIND.C. */
int far get_theta(int sx, int sy, int x, int y)
{
    int dx = x - sx;
    int result;
    int32 vx, vy;
    register int dy = y - sy;
    register int dist;
    dist = cSqRt((int32)(dx * dx + dy * dy));
    if (dist == 0) return 0;
    vx = (int32)dx * 0x7FFF;
    vy = (int32)dy * 0x7FFF;
    vx /= dist;
    vy /= dist;
    result = cAtan2((int)vx, (int)vy);
    return result;
}

void far errmsg(char *a, char *b)
{
    scroll_print("Error: ");
    scroll_print(a);
    scroll_print(" ");
    scroll_print(b);
}

char far dir_exist(char *name)
{
    struct stat st;
    if (stat(name, &st) == -1) return 0;
    if (st.st_mode & S_IFDIR) return 1;
    return 0;
}

/* Start-up check that the DATA, CRIT and CUTS directories exist; else first_punt with
   "Could not read data." (D001). */
void far check_dirs(void)
{
    unsigned char ok = 1;
    ok &= dir_exist("data");
    ok &= dir_exist("crit");
    ok &= dir_exist("cuts");
    if (!ok) first_punt(ERR_READ | 1);
}

/* 1 if at least 2200 bytes of near heap and 1500 bytes of far heap are free. */
int far OkEnoughMem_ovr167_463(void)
{
    if (coreleft() >= 0x898 && farcoreleft() >= 0x5DCL) return 1;
    return 0;
}

/* Start-up check that eight files can be open at once (a.tmp to h.tmp are created and
   deleted), so that the FILES= setting is big enough; else first_punt(6), shown as A006,
   "Resource problem or internal error". */
void far check_fds(void)
{
    int16 handles[8];
    char name[6] = "a.tmp";
    unsigned char ok = 1;
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

/* The PLAYER.DAT cipher (UW-Formats 9.2.2). build_xor_table fills the 80-byte key table
   from the seed byte, the file's first byte: four rounds of strides over the table, after
   a first loop over entries 3Ch-4Fh that the next round overwrites. That first loop only
   adds 7 to the seed in the end; UW-Formats found it in a trace of uw2edit.exe, and UW2
   has it too. xor_xor_table then codes one block of at most 80 bytes, each byte combined
   with the key, the previous output and the previous input, which makes the same routine
   decode and encode. Blocks shorter than 2 bytes are left untouched. xorread and xorwrite
   run it over a buffer 80 bytes at a time. */
void far build_xor_table(unsigned char key, unsigned char *tab)
{
    register int i;
    key += 0x71;
    for (i = 0x3C; i < 0x50; i++) tab[i] = key += i + 2;
    for (i = 0; i < 0x50; i++) tab[i] = key += 6;
    for (i = 0; i < 0x10; i++) tab[i * 5] = key += 7;
    for (i = 0; i < 4; i++) tab[i * 12] = key += 0x29;
    for (i = 0; i < 11; i++) tab[i * 7] = key += 0x49;
}

void far xor_xor_table(unsigned char far *dst, unsigned char far *src, unsigned char far *key, int n)
{
    register int i;
    register int count = n;
    if (count > 0x50) pfatal_code(7);
    if (count >= 2) {
        dst[0] = src[0] ^ key[0];
        for (i = 1; i < count; i++)
            dst[i] = src[i] ^ (key[i] + dst[i - 1] + src[i - 1]);
    }
}

int far xorread(int fd, unsigned char key, unsigned char far *buf, unsigned n)
{
    int total;
    unsigned char encrypted[0x50];
    unsigned char table[0x50];
    register unsigned remaining = n;
    register int count;
    total = 0;
    build_xor_table(key, table);
    while (remaining + 0x50 > remaining) {
        count = intoFarBuffer_ovr167_5DA(fd, encrypted, remaining < 0x50 ? remaining : 0x50);
        xor_xor_table(buf, encrypted, table, count);
        total += count;
        remaining -= 0x50;
        buf += 0x50;
    }
    return total;
}
int far xorwrite(int fd, unsigned char key, unsigned char far *buf, unsigned n)
{
    int written;
    unsigned char encrypted[0x50];
    unsigned char table[0x50];
    register unsigned remaining = n;
    register int total = 0;
    build_xor_table(key, table);
    while (remaining + 0x50 > remaining) {
        xor_xor_table(encrypted, buf, table, remaining < 0x50 ? remaining : 0x50);
        written = FarWrite_ovr167_627(fd, encrypted, remaining < 0x50 ? remaining : 0x50);
        total += written;
        remaining -= 0x50;
        buf += 0x50;
    }
    return total;
}

/* Opens a file in DATA\ (data_fopen, with stdio), or, with our_open, in the save game's
   working directory HomeDir (directory 0) or DATA\ (otherwise): mode 0 read, 1 create for
   writing, 2 create for reading and writing, 3 read and write, all binary. */
FILE * far data_fopen(char *name, char *mode)
{
    char path[0x42];
    strcpy(path, "DATA\\");
    strcat(path, name);
    return fopen(path, mode);
}
int far our_open(char *name, int directory, int mode)
{
    char path[0x50];
    register int flags;
    strcpy(path, directory == 0 ? HomeDir : "DATA\\");
    strcat(path, name);
    switch (mode) {
    case 0: flags = 0x8001; break;
    case 1: flags = 0x8302; break;
    case 2: flags = 0x8304; break;
    case 3: flags = 0x8004; break;
    }
    return open(path, flags, 0x180);
}
