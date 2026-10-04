/* target: ovr091 */
/* opts: -mm -1 -G -O -Y -d */
/* The .ark archives (LEV.ARK, CNV.ARK): opening one, reading a block, writing one back and
   closing it. The whole of UW1's DOS overlay ovr091, in original order. Seeded from
   UW2Decomp's src/sys/ARC.C (UW2's ovr093), with which it shares only the names and
   count_arc.

   UW1's archive is simpler than UW2's: a word, the number of blocks n, then n longs, the
   blocks' file offsets (0 for an absent block). There are no flags, lengths, room or
   compression: a block's length is the distance to the next block, or to the end of the
   file. The caller keeps the open archive in its own struct Arc (11 bytes); its offset
   table is read into stdat (offtab) and written back by close_arc when a block was added
   or moved. Writing a block that is new appends it; one of the same length is
   overwritten in place; one whose length changed is moved to the end, the archive copied
   into the temporary file _arc.tmp (in the archive's directory, opened alongside it)
   without the old copy, and the copy renamed over the archive. The archive's and the
   temporary file's names are kept in stdat too (arcstr), one after the other.

   UW1 has no symbol-bearing build: the names are UW2's (FM Towns), and their bssorder keys
   give the EXE's stub order (verify.py agrees). Name: inferred, as UW2's. */

#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
/* UW1: the archive functions take the caller's struct Arc (file.h has UW2's); those
   declarations are renamed out of the way. */
#define open_arc UW2_open_arc
#define close_arc UW2_close_arc
#define put_arc UW2_put_arc
#define get_arc UW2_get_arc
#define check_arc UW2_check_arc
#include "file.h"
#include "gfx.h"
#include "sys.h"
#undef open_arc
#undef close_arc
#undef put_arc
#undef get_arc
#undef check_arc

/* An open archive, kept by the caller (UW1). */
struct Arc {
    int16 fd;                           /* 0x00 */
    int16 tmpfd;                        /* 0x02, _arc.tmp */
    uint16 count;                       /* 0x04, number of blocks */
    uint32 far *offtab;                 /* 0x06, the blocks' offsets */
    unsigned char dirty;                /* 0x0A, offtab changed */
};

/* This file's _DATA, DS:0AB8..0AC8: the offset table and the names, in stdat. */
uint32 far *offtab = (uint32 far *)(stdat + 0x8000);
char far *arcstr = (char far *)stdat + 0x2800;

/* 0x0: opens the archive name into arc, with its temporary file, and reads its offset
   table; 1 if all went well. */
unsigned char far open_arc(struct Arc far *arc, char *name)
{
    unsigned char ok;
    int tmpfd;
    unsigned count;
    char path[0x50];
    register char *p;
    register int fd;

    strcpy(path, name);
    if ((p = strrchr(path, '\\')) != 0) {
        p++;
        *p = 0;
    } else path[0] = 0;
    strcat(path, "_arc.tmp");
    ok = (fd = open(name, 0x8004)) != -1;
    if (ok) {
        ok &= intoFarBuffer_ovr167_5DA(fd, &count, 2) == 2;
        ok &= intoFarBuffer_ovr167_5DA(fd, offtab, count << 2) == count << 2;
        tmpfd = open(path, 0x8302, 0x80);
        ok &= tmpfd != -1;
        arc->fd = fd;
        arc->tmpfd = tmpfd;
        arc->count = count;
        arc->offtab = offtab;
        arc->dirty = 0;
        str_copy(arcstr, name);
        str_copy(arcstr + str_len(name) + 1, path);
    }
    return ok;
}

/* 0x153: writes the offset table back if it changed, closes the archive and deletes the
   temporary file; 1 if all went well. */
unsigned char far close_arc(struct Arc far *arc)
{
    unsigned char ok = 1;
    char tmp[20];
    register int n = arc->count << 2;

    if (arc->dirty) {
        ok &= lseek(arc->fd, 2L, 0) == 2L;
        ok &= FarWrite_ovr167_627(arc->fd, offtab, n) == n;
    }
    ok &= close(arc->fd) == 0;
    close(arc->tmpfd);
    str_copy(tmp, arcstr + str_len(arcstr) + 1);
    unlink(tmp);
    return ok;
}

/* 0x22C: writes len bytes from buf as block blk (see the file comment); 0 if blk is out
   of range or the write fails. */
unsigned char far put_arc(struct Arc far *arc, unsigned blk, void far *buf, unsigned len)
{
    uint32 off;
    uint32 room;
    uint32 pos;
    unsigned n;
    unsigned char ok;
    uint32 diff;
    char arcname[20];
    char tmpname[20];
    char oldname[20];
    register unsigned i;

    off = arc->offtab[blk];
    pos = 0;
    if (arc->count < blk) return 0;
    if (off == 0) {
        pos = lseek(arc->fd, 0L, 2);
        ok = FarWrite_ovr167_627(arc->fd, buf, len) == len;
        arc->dirty = 1;
        arc->offtab[blk] = pos;
        return ok;
    }
    room = lseek(arc->fd, 0L, 2) - arc->offtab[blk];
    for (i = 0; i < arc->count; i++) {
        diff = arc->offtab[i] - off;
        if (arc->offtab[i] > off && diff < room) room = diff;
    }
    if (len == room) {
        lseek(arc->fd, off, 0);
        ok = FarWrite_ovr167_627(arc->fd, buf, len) == len;
        return ok;
    }
    arc->dirty = 1;
    lseek(arc->fd, 0L, 0);
    lseek(arc->tmpfd, 0L, 0);
    while (pos < off) {
        register unsigned chunk;
        chunk = off - pos > 0x2000 ? 0x2000 : off - pos;
        /* match: the count read is kept in diff's high word */
        ((int *)&diff)[1] = intoFarBuffer_ovr167_5DA(arc->fd, stdat, chunk);
        pos += FarWrite_ovr167_627(arc->tmpfd, stdat, ((int *)&diff)[1]);
    }
    lseek(arc->fd, room, 1);
    while ((n = intoFarBuffer_ovr167_5DA(arc->fd, stdat, 0x2000)) > 0)
        pos += FarWrite_ovr167_627(arc->tmpfd, stdat, n);
    FarWrite_ovr167_627(arc->tmpfd, buf, len);
    for (i = 0; i < arc->count; i++)
        if (arc->offtab[i] > 0 && arc->offtab[i] > off) arc->offtab[i] -= (unsigned)room;
    arc->offtab[blk] = pos;
    str_copy(arcname, arcstr);
    str_copy(tmpname, arcstr + str_len(arcstr) + 1);
    strcpy(oldname, tmpname);
    oldname[strlen(tmpname) - 1] = '_';
    close(arc->fd);
    close(arc->tmpfd);
    unlink(arcname);
    rename(tmpname, arcname);
    arc->fd = open(arcname, 0x8004);
    arc->tmpfd = open(tmpname, 0x8302, 0x80);
    return 1;
}

/* 0x61A: reads block blk into buf; its length, or 0 if it is out of range or absent. */
int far get_arc(struct Arc far *arc, unsigned blk, void far *buf)
{
    uint32 off;
    uint32 size;
    uint32 d;
    register unsigned i;
    register int n;

    if (arc->count < blk) return 0;
    off = arc->offtab[blk];
    if (off == 0) return 0;
    size = lseek(arc->fd, 0L, 2) - off;
    for (i = 0; i < arc->count; i++) {
        d = arc->offtab[i] > off ? arc->offtab[i] - off : 0;
        if (d > 0 && d < size) size = d;
    }
    lseek(arc->fd, off, 0);
    n = intoFarBuffer_ovr167_5DA(arc->fd, buf, size);
    AX_RESULT(n);
}

/* 0x72E: the number of blocks in the archive at name; -1 if it cannot be opened. */
int far count_arc(char *name)
{
    uint16 count;
    unsigned char ok;
    register int fd;

    if ((fd = open(name, 0x8001)) == -1)
        return -1;
    ok = read(fd, &count, 2) == 2;
    ok &= close(fd) == 0;
    if (ok)
        return count;
    return 0;
}

/* 0x798: whether the archive at name holds block blk; -1 if it cannot be read. */
int far check_arc(char *name, unsigned blk)
{
    uint32 off;
    uint32 pos;
    unsigned char ok;
    register int fd;

    if ((fd = open(name, 0x8001)) == -1)
        return -1;
    pos = blk * 4 + 2;
    ok = lseek(fd, pos, 0) == pos;
    ok &= read(fd, &off, 4) == 4;
    ok &= close(fd) == 0;
    return ok ? off != 0 : -1;
}
