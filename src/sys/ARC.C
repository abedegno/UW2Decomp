/* target: ovr093 */
/* opts: -mm -1 -G -O -Y -d */
/* The .ark archives (LEV.ARK, CNV.ARK, TEST.ARK, BYT.ARK, SCD.ARK): naming them, opening
   and closing the current one, reading a block and writing one back, growing the file when
   a block no longer fits. The whole of DOS overlay ovr093, in original order. Function and
   global names are the originals from the FM Towns symbol table, where its functions sit in
   the same order (arc_read_tables_ to count_arc_).

   An archive starts with a word, the number of blocks n, and a long, then four tables of n
   longs: the blocks' file offsets (at 6), their flags (at 6 + 4n: bit 0 compress when
   writing, bit 1 compressed, bit 2 has room reserved), their lengths (at 6 + 8n) and the
   room allocated to each (at 6 + 12n). An offset of 0 means the block is absent. A
   compressed block is ACLZW.C's format: the uncompressed length as a long, then LZSS.
   (UW-Formats 9.1 describes the same layout but places the long at 0004; the code reads
   it straight after the count, at 2.)

   Only one archive is open at a time, in arcfile: open_arc opens it, get_arc and put_arc
   act on it, close_arc closes it, and their archive-number arguments are ignored. Callers:
   MAP.C and TEXTMAPS.C (LEV.ARK, the level maps and texture lists), BABL.C (CNV.ARK, the
   conversations), SHOWPIC.C (BYT.ARK, full-screen pictures), SCHEDULE.C (SCD.ARK),
   AUTOMAP.C and GAMEWRAP.C. Writing a block that has grown rebuilds the whole file. The
   table copies live in stdat (FARDATA.ASM), the shared big buffer, which put_arc also
   uses as its copy buffer.
   name: inferred, from the arc_ prefix of the FM Towns names (open_arc, close_arc,
   get_arc, put_arc ...). */

#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "file.h"
#include "gfx.h"
#include "sys.h"

/* The open archive. */
struct ArcFile {
    int fd;                             /* 0x00 */
    unsigned count;                     /* 0x02, number of blocks */
    unsigned long hdr;                  /* 0x04, the long after the count */
    char name[0x50];                    /* 0x08, the file's path */
};

/* The open archive: far, so its own segment (637E:0000, segment table entry 75).
   name: FM Towns keeps it at 7BDA0 with no symbol of its own and its open_arc_ stores to
   it unnamed, so it was static; the name is provisional. */
static struct ArcFile far arcfile;

/* The table copies, in the big shared buffer stdat; arc_read_tables fills them and
   arc_write_tables writes them back, both only when put_arc rebuilds a file. arcstr is
   not used in this file or elsewhere. */
unsigned long far *offtab = (unsigned long far *)(stdat + 0x2000);
unsigned long far *flagtab = (unsigned long far *)(stdat + 0x2800);
unsigned long far *lentab = (unsigned long far *)(stdat + 0x3000);
unsigned long far *alloctab = (unsigned long far *)(stdat + 0x3800);
char far *arcstr = (char far *)stdat + 0x4000;

/* Reads the four tables into offtab and the rest, leaving the file position alone. */
void far arc_read_tables(register int fd)
{
    long pos = tell(fd);
    register int ok = 1;
    unsigned n;
    long hdr;

    lseek(fd, 0L, 0);
    ok &= intoFarBuffer_ovr167_5DA(fd, &n, 2) == 2;
    ok &= intoFarBuffer_ovr167_5DA(fd, &hdr, 4) == 4;
    ok &= intoFarBuffer_ovr167_5DA(fd, offtab, n * 4) == n * 4;
    ok &= intoFarBuffer_ovr167_5DA(fd, flagtab, n * 4) == n * 4;
    ok &= intoFarBuffer_ovr167_5DA(fd, lentab, n * 4) == n * 4;
    ok &= intoFarBuffer_ovr167_5DA(fd, alloctab, n * 4) == n * 4;
    lseek(fd, pos, 0);
}

/* Writes the four tables of n blocks back at offset 6; nonzero if all went out. */
int far arc_write_tables(int fd, register unsigned n)
{
    long pos = tell(fd);
    register int ok = 1;

    lseek(fd, 6L, 0);
    ok &= FarWrite_ovr167_627(fd, offtab, n * 4) == n * 4;
    ok &= FarWrite_ovr167_627(fd, flagtab, n * 4) == n * 4;
    ok &= FarWrite_ovr167_627(fd, lentab, n * 4) == n * 4;
    ok &= FarWrite_ovr167_627(fd, alloctab, n * 4) == n * 4;
    lseek(fd, pos, 0);
    return ok;
}

/* Reads (get_ulong) or writes (put_ulong) the long at offset off of the file, leaving
   the file position where it was. The block tables are read and patched through these,
   one entry at a time. */
unsigned long far get_ulong(register int fd, int off)
{
    unsigned long val;
    long pos = tell(fd);

    lseek(fd, (long)off, 0);
    read(fd, &val, 4);
    lseek(fd, pos, 0);
    return val;
}

void far put_ulong(register int fd, int off, unsigned long val)
{
    long pos = tell(fd);

    lseek(fd, (long)off, 0);
    write(fd, &val, 4);
    lseek(fd, pos, 0);
}

/* Builds the path of archive `which` in directory `dir`. */
char * far decode_ark(register int which, char *dir, register char *buf)
{
    strcpy(buf, dir);
    if (which == 1)
        strcat(buf, "lev.ark");
    else if (which == 2)
        strcat(buf, "cnv.ark");
    else if (which == 3)
        strcat(buf, "test.ark");
    else if (which == 4)
        strcat(buf, "byt.ark");
    else if (which == 5)
        strcat(buf, "scd.ark");
    else
        *buf = 0;
    return buf;
}

/* Copies size bytes from src to dst, 100 at a time; the count copied (0 if a write came
   up short). put_arc uses it to copy a compressed block from its temporary file. */
unsigned far ac_suck_data(int dst, int src, register unsigned size)
{
    unsigned total;
    unsigned toread;
    unsigned wrote;
    char buf[100];
    register unsigned got;

    total = 0;
    got = 1;
    while (size && got) {
        toread = size < 100 ? size : 100;
        got = read(src, buf, toread);
        wrote = write(dst, buf, got);
        if (wrote != got)
            return !size;
        total += got;
        size -= got;
    }
    return total;
}

/* Opens archive `which` (1 LEV.ARK, 2 CNV.ARK, 3 TEST.ARK, 4 BYT.ARK, 5 SCD.ARK) in
   directory dir (callers pass "DATA\\" for the shipped files or HomeDir for the save
   game's working copies) for reading and writing, and reads its block count. 1 if it
   opened. */
unsigned char far open_arc(int which, char *dir)
{
    unsigned char ok = 1;
    unsigned count;
    unsigned long hdr;
    char name[0x50];
    register int fd;

    decode_ark(which, dir, name);
    str_copy(arcfile.name, name);
    ok = (fd = open(name, O_RDWR | O_BINARY)) != -1;
    if (ok) {
        ok &= intoFarBuffer_ovr167_5DA(fd, &count, 2) == 2;
        ok &= intoFarBuffer_ovr167_5DA(fd, &hdr, 4) == 4;
        if (ok) {
            arcfile.fd = fd;
            arcfile.count = count;
            arcfile.hdr = hdr;
        } else
            close(fd);
    }
    return ok;
}

unsigned char far close_arc(int arc)
{
    unsigned char ok = 1;

    ok &= close(arcfile.fd) == 0;
    return ok;
}

/* Writes len bytes from buf as block blk of the open archive, compressing them first
   (into the temporary file _mem.tmp in HomeDir) if the block's flags ask for it and
   compression helps. Then one of three cases:
   - the data fits the block's room (its allocation, if it has room reserved, else its
     exact old length): it is written in place;
   - the block is new (offset 0): it is appended, with 15% of its size in zeros as room to
     grow if the block is flagged for reserved room;
   - it has outgrown its room: the archive is copied to _arc.tmp without the block's old
     copy, the later blocks' offsets are moved down by the old allocation, the block is
     appended (with its 15%) and the copy replaces the archive.
   Any short write is fatal (E002). Returns 1. */
unsigned char far put_arc(int arc, unsigned blk, char far *buf, unsigned len)
{
    unsigned long off;
    unsigned long flags;
    unsigned long alloc;
    unsigned long size;
    unsigned long writepos;
    unsigned long allocsize;
    unsigned long newflags;
    unsigned i;
    int ok;
    int memfd;
    unsigned char extra;
    unsigned char compressed;
    long done;
    unsigned n;
    int m;
    unsigned w;
    unsigned chunk;
    char memname[0x50];
    char arcname[0x50];
    char tmpname[0x50];
    register int fd;
    register unsigned t;

    ok = 1;
    memfd = -1;
    fd = arcfile.fd;
    if (blk > arcfile.count)
        return 0;
    off = get_ulong(fd, blk * 4 + 6);
    flags = get_ulong(fd, arcfile.count * 4 + blk * 4 + 6);
    extra = flags & 4;
    alloc = get_ulong(fd, arcfile.count * 4 * 3 + blk * 4 + 6);
    if (flags & 1) {
        strcpy(memname, HomeDir);
        strcat(memname, "_mem.tmp");
        memfd = open(memname, O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0x80);
        if (memfd == -1)
            return 0;
        t = ac_shrink_disk(buf, memfd, len);
        if (t == 0) {
            size = len;
            compressed = 0;
        } else {
            size = t;
            compressed = 1;
        }
        lseek(memfd, 0L, 0);
    } else {
        size = len;
        compressed = 0;
    }
    newflags = flags;
    if (compressed)
        newflags |= 2;
    else
        newflags &= ~2L;
    put_ulong(fd, arcfile.count * 4 * 2 + blk * 4 + 6, size);
    put_ulong(fd, arcfile.count * 4 + blk * 4 + 6, newflags);
    if (size) {
        if (extra && size <= alloc || size == alloc) {
            /* it fits where it is */
            lseek(fd, off, 0);
            if (compressed) {
                t = ac_suck_data(fd, memfd, (unsigned)size);
                if (t != (unsigned)size) {
                    close(memfd);
                    unlink(memname);
                    pfatal_code(ERR_WRITE | 2);
                }
            } else {
                t = FarWrite_ovr167_627(fd, buf, (unsigned)size);
                if (t != (unsigned)size) {
                    close(memfd);
                    unlink(memname);
                    pfatal_code(ERR_WRITE | 2);
                }
            }
        } else if (off == 0) {
            /* a new block: append it */
            lseek(fd, 0L, 2);
            writepos = tell(fd);
            if (compressed)
                ac_suck_data(fd, memfd, (unsigned)size);
            else
                FarWrite_ovr167_627(fd, buf, (unsigned)size);
            if (extra) {
                t = 15 * size / 100;
                mem_set(stdat, 0, t);
                FarWrite_ovr167_627(fd, stdat, t);
                allocsize = size + t;
            } else
                allocsize = size;
            put_ulong(fd, blk * 4 + 6, writepos);
            put_ulong(fd, arcfile.count * 4 * 3 + blk * 4 + 6, allocsize);
        } else {
            /* it has grown: rebuild the archive without its old copy and append it */
            done = 0;
            strcpy(tmpname, HomeDir);
            strcat(tmpname, "_arc.tmp");
            t = open(tmpname, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0x80);
            lseek(fd, 0L, 0);
            while (done < off) {
                chunk = off - done > 0x2000 ? 0x2000 : off - done;
                m = intoFarBuffer_ovr167_5DA(fd, stdat, chunk);
                if ((w = FarWrite_ovr167_627(t, stdat, m)) != m) {
                    close(t);
                    unlink(tmpname);
                    close(memfd);
                    unlink(memname);
                    pfatal_code(ERR_WRITE | 2);
                }
                done += w;
            }
            lseek(fd, alloc, 1);
            while ((n = intoFarBuffer_ovr167_5DA(fd, stdat, 0x2000)) > 0) {
                if ((m = FarWrite_ovr167_627(t, stdat, n)) != n) {
                    close(t);
                    unlink(tmpname);
                    close(memfd);
                    unlink(memname);
                    pfatal_code(ERR_WRITE | 2);
                }
                done += m;
            }
            writepos = tell(t);
            if (compressed)
                n = ac_suck_data(t, memfd, (unsigned)size);
            else if ((n = FarWrite_ovr167_627(t, buf, (unsigned)size)) != (unsigned)size) {
                close(t);
                unlink(tmpname);
                close(memfd);
                unlink(memname);
                pfatal_code(ERR_WRITE | 2);
            }
            if (extra) {
                m = 15 * size / 100;
                mem_set(stdat, 0, m);
                if ((w = FarWrite_ovr167_627(t, stdat, m)) != m) {
                    close(t);
                    unlink(tmpname);
                    close(memfd);
                    unlink(memname);
                    pfatal_code(ERR_WRITE | 2);
                }
                allocsize = size + (unsigned)m;
            } else
                allocsize = size;
            arc_read_tables(fd);
            for (i = 0; i < arcfile.count; i++)
                if (offtab[i] > 0 && offtab[i] > off)
                    offtab[i] -= alloc;
            if (!arc_write_tables(t, arcfile.count)) {
                close(t);
                unlink(tmpname);
                close(memfd);
                unlink(memname);
                pfatal_code(ERR_WRITE | 2);
            }
            put_ulong(t, blk * 4 + 6, writepos);
            put_ulong(t, arcfile.count * 4 * 3 + blk * 4 + 6, allocsize);
            close(fd);
            close(t);
            str_copy(arcname, arcfile.name);
            unlink(arcname);
            rename(tmpname, arcname);
            arcfile.fd = open(arcname, O_RDWR | O_BINARY);
        }
    }
    if (memfd != -1) {
        close(memfd);
        unlink(memname);
    }
    return ok;
}

/* Reads block blk into buf, decompressing it if need be; its length, or 0. */
unsigned far get_arc(int arc, unsigned blk, char far *buf)
{
    unsigned long off;
    unsigned long flags;
    unsigned t;
    register unsigned size;
    register unsigned n;

    if (blk > arcfile.count)
        return 0;
    off = get_ulong(arcfile.fd, blk * 4 + 6);
    size = get_ulong(arcfile.fd, arcfile.count * 4 * 2 + blk * 4 + 6);
    flags = get_ulong(arcfile.fd, arcfile.count * 4 + blk * 4 + 6);
    if (off == 0)
        return 0;
    lseek(arcfile.fd, off, 0);
    if (flags & 2) {
        t = ac_unshrink_disk(buf, arcfile.fd, size);
        if (t == 0)
            n = 0;
        else
            n = t;
    } else {
        intoFarBuffer_ovr167_5DA(arcfile.fd, buf, size);
        n = size;
    }
    return n;
}

/* Whether archive `which` in `dir` holds block blk; -1 if it cannot be opened. */
int far check_arc(int which, char *dir, int blk)
{
    unsigned count;
    unsigned long off;
    char name[0x50];
    register int fd;

    decode_ark(which, dir, name);
    if ((fd = open(name, O_RDONLY | O_BINARY)) == -1)
        return -1;
    read(fd, &count, 2);
    off = get_ulong(fd, blk * 4 + 6);
    close(fd);
    return off != 0;
}

/* Empty in FM Towns too. */
void far explode_arc(void)
{
}

/* The number of blocks in the archive at `name`; -1 if it cannot be opened. Nothing in
   the sources calls it. */
int far count_arc(char *name)
{
    unsigned count;
    unsigned char ok = 1;
    register int fd;

    if ((fd = open(name, O_RDONLY | O_BINARY)) == -1)
        return -1;
    ok = read(fd, &count, 2) == 2;
    ok &= close(fd) == 0;
    if (ok)
        return count;
    return 0;
}
