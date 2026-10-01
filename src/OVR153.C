/* target: ovr153 */
/* opts: -mm -1 -G -O -Y -d */
/* Reading and writing an LZW-compressed block of a save file, using a work area at the
   top of the static data: the whole of DOS overlay ovr153, in original order. Function and
   global names are the originals from the FM Towns symbol table; the source file's own
   name is not known. */

extern char far stdat[];
/* This file's _DATA, DS:1AD4 (ovr152's strings end there; PLAYER.C's data starts at 1AD8):
   the LZSS work area, set up here and used by ovr127. */
char far *globals = 0;

/* Far-buffer file reads and writes in ovr167. FM Towns, being flat, calls the library's
   read() and write() here; DOS cannot (_read is the near-buffer library call at 0E72:1FCD),
   and the DOS helpers' original names are not known, so these are the IDA names. */
int far intoFarBuffer_ovr167_5DA(int fd, void far *buf, unsigned n);
int far FarWrite_ovr167_627(int fd, void far *buf, unsigned n);
unsigned far DecompressLZW_disk(char far *dst, int fd, char far *work, unsigned worksize,
                                unsigned n);
unsigned far CompressLZW_disk(char far *src, int fd, char far *work, unsigned worksize,
                              unsigned n);

void far ac_setup_lzw(char far **work, unsigned *worksize)
{
    globals = stdat;
    *work = globals + 0x722F;
    *worksize = 0x8DD0;
}

unsigned far ac_unshrink_disk(char far *dst, int fd, unsigned n)
{
    unsigned long len;
    char far *work;
    unsigned worksize;

    intoFarBuffer_ovr167_5DA(fd, &len, 4);
    ac_setup_lzw(&work, &worksize);
    len = DecompressLZW_disk(dst, fd, work, worksize, n - 4);
    /* DOS compares the length with 0xFFFF and jumps to the same place either way: an
       empty statement, perhaps a check that was compiled out. The test's sense is not
       recoverable from the bytes. */
    if (len > 0xFFFFL)
        ;
    return len;
}

unsigned far ac_shrink_disk(char far *src, int fd, unsigned n)
{
    unsigned long len;
    char far *work;
    unsigned worksize;
    unsigned done;

    len = n;
    ac_setup_lzw(&work, &worksize);
    FarWrite_ovr167_627(fd, &len, 4);
    done = CompressLZW_disk(src, fd, work, worksize, n);
    if (done == 0)
        return 0;
    return done + 4;
}
