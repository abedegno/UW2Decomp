/* target: ovr167 */
/* opts: -mm -1 -G -O -Y -d */
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <stat.h>
#include <string.h>
#include <stdlib.h>
#include "file.h"
#include "gfx.h"
#include "sys.h"
#include "ui.h"

/* FM Towns names are used where the same operation is clear in its code:
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

int far postodir(x1, y1, x2, y2)
char x1, y1, x2, y2;
{ return mpos(x2 - x1, y2 - y1); }

void far scroll_print(char far *s);
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

int far get_theta(int sx, int sy, int x, int y)
{
    int dx = x - sx;
    int result;
    long vx, vy;
    register int dy = y - sy;
    register int dist;
    dist = cSqRt((long)(dx * dx + dy * dy));
    if (dist == 0) return 0;
    vx = (long)dx * 0x7FFF;
    vy = (long)dy * 0x7FFF;
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

void far check_dirs(void)
{
    unsigned char ok = 1;
    ok &= dir_exist("data");
    ok &= dir_exist("crit");
    ok &= dir_exist("cuts");
    if (!ok) first_punt(ERR_READ | 1);
}

unsigned far coreleft(void);
unsigned long far farcoreleft(void);
int far OkEnoughMem_ovr167_463(void)
{
    if (coreleft() >= 0x898 && farcoreleft() >= 0x5DCL) return 1;
    return 0;
}

void far check_fds(void)
{
    int handles[8];
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

char far blttodrive(void far *buf, char *name, unsigned n)
{
    unsigned char ok = 1;
    register int fd;
    fd = open(name, 0x8302, 0x80);
    if (fd < 0) return 0;
    if (FarWrite_ovr167_627(fd, buf, n) != n) ok = 0;
    if (close(fd)) ok = 0;
    return ok;
}

char far bltfromdrive(char *name, void far *buf, unsigned n)
{
    unsigned char ok = 1;
    register int fd;
    fd = open(name, 0x8001);
    if (fd < 0) return 0;
    if (intoFarBuffer_ovr167_5DA(fd, buf, n) != n) ok = 0;
    if (close(fd)) ok = 0;
    return ok;
}

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

void far fopen(char *path, int mode);
void far data_fopen(char *name, int mode)
{
    char path[0x42];
    strcpy(path, "DATA\\");
    strcat(path, name);
    fopen(path, mode);
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
