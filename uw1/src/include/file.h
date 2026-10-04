/* file.h: files: the .ark archives (ARC.C), raw file I/O with far buffers (MISCUTIL.C),
   the compression of archive blocks (ACLZW.C sets up the work area, LZSS.C is Okumura's
   LZSS), the inventory and game saves (INVSAVE.C, GAMEWRAP.C) and the screenshot writer
   (SCRSHOT.C). Several of these share stdat, the big far work buffer in FARDATA.ASM.
   docs/subsystems/sys.md describes the archive and compression side. */
#ifndef FILE_H
#define FILE_H

#include "uw2.h"

struct Object;
union Link;

#include "object.h"

/* An open archive, 11 bytes, kept by the caller (UW1; UW2 keeps its archives in ARC.C by
   number). The callers that only pass it on hold it as bytes (char[12] on the stack). */
struct Arc {
    int16 fd;                           /* 0x00 */
    int16 tmpfd;                        /* 0x02, _arc.tmp */
    uint16 count;                       /* 0x04, number of blocks */
    uint32 far *offtab;                 /* 0x06, the blocks' offsets */
    unsigned char dirty;                /* 0x0A, offtab changed */
};

/* Where ARC.C keeps its data in stdat (FARDATA.ASM): the file names of the archive being
   rewritten and its temporary file, one after the other (put_arc), and the offset table
   open_arc reads. The names are ours. */
#define STDAT_ARC_NAMES  0x2800
#define STDAT_ARC_OFFTAB 0x8000

/* MISCUTIL.C: file I/O helpers */
void far flip_bool(char *p);
void far print_path_to(char far *s, int px, int py, int ignored, int ox, int oy, int previous,
                       int radius);
void far errmsg(char *a, char *b);
void far check_dirs(void);
void far check_fds(void);
int far FarWrite_ovr167_627(int fd, void far *buf, unsigned n);
/* Far-buffer file reads and writes (MISCUTIL.C, UW1's ovr154; UW2's ovr167). FM Towns,
   being flat, calls the library's read() and write() here; DOS cannot (the library's _read
   takes a near buffer). name: the DOS helpers' original names are not known, so these
   keep UW2's IDA names. */
int far intoFarBuffer_ovr167_5DA(int fd, void far *buf, unsigned n);
/* Opens a file in DATA\ with stdio (MISCUTIL.C). */
FILE * far data_fopen(char *name, char *mode);
int far mpos(char x, char y);
int far xorread(int fd, unsigned char key, unsigned char far *buf, unsigned n);
int far xorwrite(int fd, unsigned char key, unsigned char far *buf, unsigned n);
int far do_beep(int freq, int ms);
void far memcheck(void);

/* LZSS.C: LZSS compression of archive blocks (Okumura's LZSS; the "LZW" in the original
   names is not the algorithm). Both return the number of bytes produced. */

/* ACLZW.C: reading and writing a compressed archive block: the uncompressed length as a
   long, then the LZSS stream (the name says LZW; the algorithm is LZSS.C's) */
/* LZSS.C's work area. ACLZW.C places it at the start of stdat and the file buffer straight after
   it, at +722Fh. The bytes at 0 and 0Dh and the words at 722Bh and 722Dh are set or
   tested by LZSS.C but not otherwise used, so their meaning is unknown. text_buf's length is
   measured only as the distance to lson. */
struct LzwWork {
    unsigned char flag0;                /* 0x0000, tested (to no effect) at the end of compression */
    uint32 textsize;                    /* 0x0001 */
    uint32 codesize;                    /* 0x0005 */
    uint32 printcount;                  /* 0x0009 */
    unsigned char flagD;                /* 0x000D, set to 1 when compressing */
    int16 match_position;               /* 0x000E */
    int16 match_length;                 /* 0x0010 */
    unsigned char text_buf[4096 + 18 + 1]; /* 0x0012, N + F + 1 (LZSS.C) */
    int16 lson[4096 + 1];               /* 0x1025 */
    int16 rson[4096 + 257];             /* 0x3027 */
    int16 dad[4096 + 1];                /* 0x5229 */
    int16 w722B;                        /* 0x722B, set to -1 when compressing */
    int16 w722D;                        /* 0x722D, set to 0 when compressing */
};

/* INVSAVE.C: saving and restoring the player's inventory in player.dat */
void far FreePlayerInv(union Link far *head);
void far InvSaveNexts(union Link far *src, uint16 far *dst);
void far replaceInInv(union Link far *old, union Link far *new);
struct Object far * far allocSaveObj(void);
struct Object far * far getSaveObj(int n);
void far putInInv(union Link far *mem, union Link far *saved);
void far InvRestoreNexts(uint16 far *dst, union Link far *src);
void far getPlayerInvCopy(void far *ws);
void far Punt_player_inv(void);
char far SavePlayerInv(char *name);
char far RestorePlayerInv(char *name);

/* GAMEWRAP.C: saving and restoring games, and changing level */
char far clear_dir(char *dir);
unsigned char far copy_dir(char *src, char *dst);
void far do_level_hacks(int level, int mode);
int far GetLevel(int level);
void far get_save_descs(char descs[][40], int16 *found);
void far ShowSaveRest(void);
void far DoSaveRest(int restore, int slot);
char far copy_file(char *src, char *dst);
char far init_save(void);
/* RestoreGame(char slot) and SaveGame(char slot, char *desc). */
/* match: no prototypes under Turbo C (OLDSTYLE, portable.h): their callers push the slot as
   an int. SaveGame is declared before
   SaveLevel because TLINK numbers the overlay's stub entries in the order Turbo C lists
   the publics, which for names with the same hash key is the order they were first seen:
   the EXE's stub has SaveGame before SaveLevel. */
char far RestoreGame OLDSTYLE((char slot));
char far SaveGame OLDSTYLE((char slot, char *desc));
char far ChangeLevel(int from, int to);
char far SaveLevel(int level);

/* SCRSHOT.C: screenshots */
int far GifPixel_ovr116_420(void);
void far save_screenshot(int seg);
void far ovr112_194(int fd, char size);
void far ovr112_2A3(int fd, int bits);

#endif
