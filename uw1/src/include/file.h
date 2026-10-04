/* file.h: files: the .ark archives (ARC.C), raw file I/O with far buffers (MISCUTIL.C),
   the compression of archive blocks (ACLZW.C sets up the work area, LZSS.C is Okumura's
   LZSS), the inventory and game saves (INVSAVE.C, GAMEWRAP.C) and the screenshot writer
   (SCRSHOT.C). Several of these share stdat, the big far work buffer in FARDATA.ASM.
   docs/subsystems/sys.md describes the archive and compression side. */
#ifndef FILE_H
#define FILE_H

#include "uw2.h"

struct Object;

#include "object.h"

/* ARC.C: the .ark archives */
/* UW1: the archive functions (sys/ARC.C: open_arc, close_arc, put_arc, get_arc, check_arc,
   count_arc) take UW1's 11-byte struct Arc or a file name, not UW2's archive numbers; each
   caller declares them for now (docs/NOTES.md, ARC.C), until the readability pass. */

/* MISCUTIL.C: file I/O helpers */
void far flip_bool(char *p);
int far octant(char x, char y);
void far print_path_to(char far *s, int px, int py, int ignored, int ox, int oy, int previous,
                       int radius);
/* name: FM Towns get_theta_ (IDA's DartSatelliteVectoring_ovr167_313). */
int far get_theta(int sx, int sy, int x, int y);
void far errmsg(char *a, char *b);
void far check_dirs(void);
void far check_fds(void);
int far FarWrite_ovr167_627(int fd, void far *buf, unsigned n);
/* Far-buffer file reads and writes in ovr167. FM Towns, being flat, calls the library's
   read() and write() here; DOS cannot (_read is the near-buffer library call at 0E72:1FCD).
   name: the DOS helpers' original names are not known, so these are the IDA names. */
int far intoFarBuffer_ovr167_5DA(int fd, void far *buf, unsigned n);
int far our_open(char *name, int directory, int mode);
int far OkEnoughMem_ovr167_463(void);
/* name: FM Towns bltfromdrive_ (read_file_to_mbuf_ and load_sound_driver_ call it where
   DOS calls 65E0:007A). */
unsigned char far bltfromdrive(char *name, void far *buf, unsigned n);
unsigned char far blttodrive(void far *buf, char *name, unsigned n);
FILE * far data_fopen(char *name, char *mode);
int far mpos(char x, char y);
int far xorread(int fd, unsigned char key, unsigned char far *buf, unsigned n);
int far xorwrite(int fd, unsigned char key, unsigned char far *buf, unsigned n);

/* LZSS.C: LZSS compression of archive blocks (Okumura's LZSS; the "LZW" in the original
   names is not the algorithm). Both return the number of bytes produced. */
unsigned far DecompressLZW_disk(unsigned char far *dst, int fd, unsigned char far *work,
                                unsigned worksize, unsigned n);
unsigned far CompressLZW_disk(unsigned char far *src, int fd, unsigned char far *work,
                              unsigned worksize, unsigned n);

/* ACLZW.C: reading and writing a compressed archive block: the uncompressed length as a
   long, then the LZSS stream (the name says LZW; the algorithm is LZSS.C's) */
unsigned far ac_unshrink_disk(char far *dst, int fd, unsigned n);
unsigned far ac_shrink_disk(char far *src, int fd, unsigned n);
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
extern struct LzwWork far *globals;

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
unsigned char far clear_dir(char *dir);
unsigned char far copy_dir(char *src, char *dst);
void far do_level_hacks(int level, int mode);
int far GetLevel(int level);
void far get_save_descs(char descs[][40], int16 *found);
void far ShowSaveRest(void);
void far DoSaveRest(int restore, int slot);
unsigned char far copy_file(char *srcdir, char *dstdir, char *name);
void far gruesome_door_hack(int x, int y);
unsigned char far init_save(void);
/* RestoreGame(char slot) and SaveGame(char slot, char *desc). */
/* match: no prototypes under Turbo C (OLDSTYLE, portable.h): their callers push the slot as
   an int. SaveGame is declared before
   SaveLevel because TLINK numbers the overlay's stub entries in the order Turbo C lists
   the publics, which for names with the same hash key is the order they were first seen:
   the EXE's stub has SaveGame before SaveLevel. */
char far RestoreGame OLDSTYLE((char slot));
int far SaveGame OLDSTYLE((char slot, char *desc));
char far ChangeLevel(int from, int to);

/* SCRSHOT.C: screenshots */
void far ovr116_194(int fd, char size);
void far ovr116_2A3(int fd, int bits);
int far GifPixel_ovr116_420(void);
void far save_screenshot(int seg);

#endif
