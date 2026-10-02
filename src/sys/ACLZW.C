/* target: ovr153 */
/* opts: -mm -1 -G -O -Y -d */
/* Reading and writing a compressed block of an .ark archive: the whole of DOS overlay
   ovr153, in original order. ARC.C's get_arc calls ac_unshrink_disk for a block whose
   flags say compressed (bit 1), and put_arc calls ac_shrink_disk for one flagged
   compress-on-write (bit 0). Both set up the work area in stdat (the large far buffer in
   FARDATA.ASM) with ac_setup_lzw and hand the work to LZSS.C; the compressed data is
   prefixed with its uncompressed length as a long. This file owns `globals`, the far
   pointer LZSS.C reaches its work area through. Function and global names are the
   originals from the FM Towns symbol table.
   name: descriptive (ACLZW, after the ac_ prefix and the "LZW" of the FM Towns names; the
   algorithm is in fact LZSS, see LZSS.C). */

#include "file.h"
#include "gfx.h"

/* This file's _DATA, DS:1AD4 (ovr152's strings end there; SKILLS.C's data starts at 1AD8):
   the LZSS work area, set up here and used by ovr127. */
struct LzwWork far *globals = 0;

/* Points globals at stdat (LZSS.C's struct LzwWork, 722Fh bytes) and returns the rest of
   the buffer after it, 8DD0h bytes from stdat+722Fh, as the file read or write buffer: so
   stdat is at least 10000h bytes long. */
void far ac_setup_lzw(char far **work, unsigned *worksize)
{
    globals = (struct LzwWork far *)stdat;
    *work = (char far *)globals + sizeof(struct LzwWork);
    *worksize = 0x8DD0;
}

/* Reads a compressed block of n bytes at fd's position, the long uncompressed length and
   then the LZSS stream, and decompresses it into dst. Returns the bytes produced, which
   get_arc returns as the block's length. The stored length is read but not used to limit
   the output. */
unsigned far ac_unshrink_disk(char far *dst, int fd, unsigned n)
{
    unsigned long len;
    char far *work;
    unsigned worksize;

    intoFarBuffer_ovr167_5DA(fd, &len, 4);
    ac_setup_lzw(&work, &worksize);
    len = DecompressLZW_disk(dst, fd, work, worksize, n - 4);
    /* match: DOS compares the length with 0xFFFF and jumps to the same place either way:
       an empty statement, perhaps a check that was compiled out. The test's sense is not
       recoverable from the bytes. */
    if (len > 0xFFFFL)
        ;
    return len;
}

/* Writes n bytes of src to fd as the long length n and an LZSS stream. Returns the bytes
   written including the 4-byte length, or 0 if compression saved nothing; put_arc then
   writes the block uncompressed (fd is its temporary file _mem.tmp, so what was written
   there is discarded). */
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
