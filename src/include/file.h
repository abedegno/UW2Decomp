/* file.h: Files: the .ark archives, raw file I/O, compression, save games and the
   screenshot writer. */
#ifndef FILE_H
#define FILE_H

#include "uw2.h"

struct Object;

#include "object.h"

/* OVR093.C: the .ark archives */
unsigned char far open_arc(int which, char *dir);

/* OVR167.C: file I/O helpers */
void far flip_bool(char *p);
int far octant(char x, char y);
void far print_path_to(char far *s, int px, int py, int ignored, int ox, int oy, int previous,
                       int radius);
/* FM Towns get_theta_ (IDA's DartSatelliteVectoring_ovr167_313). */
int far get_theta(int sx, int sy, int x, int y);
void far errmsg(char *a, char *b);
void far check_dirs(void);
void far check_fds(void);
int far FarWrite_ovr167_627(int fd, void far *buf, unsigned n);
/* Far-buffer file reads and writes in ovr167. FM Towns, being flat, calls the library's
   read() and write() here; DOS cannot (_read is the near-buffer library call at 0E72:1FCD),
   and the DOS helpers' original names are not known, so these are the IDA names. */
int far intoFarBuffer_ovr167_5DA(int fd, void far *buf, unsigned n);
int far our_open(char *name, int directory, int mode);

/* OVR127.C: LZSS compression of save-file blocks */
unsigned far DecompressLZW_disk(unsigned char far *dst, int fd, unsigned char far *work,
                                unsigned worksize, unsigned n);
unsigned far CompressLZW_disk(unsigned char far *src, int fd, unsigned char far *work,
                              unsigned worksize, unsigned n);

/* OVR153.C: LZW-compressed save blocks */
unsigned far ac_unshrink_disk(char far *dst, int fd, unsigned n);
unsigned far ac_shrink_disk(char far *src, int fd, unsigned n);

/* OVR122.C: saving and restoring the player's inventory in player.dat */
void far FreePlayerInv(unsigned far *head);
void far InvSaveNexts(unsigned far *src, unsigned far *dst);
void far replaceInInv(union Link far *old, union Link far *new);
struct Object far * far allocSaveObj(void);
struct Object far * far getSaveObj(int n);
void far putInInv(union Link far *mem, union Link far *saved);
void far InvRestoreNexts(unsigned far *dst, unsigned far *src);
void far getPlayerInvCopy(void far *ws);
void far Punt_player_inv(void);
char far SavePlayerInv(char *name);
char far RestorePlayerInv(char *name);

/* OVR149.C: saving and restoring games, and changing level */
unsigned char far clear_dir(char *dir);
unsigned char far copy_dir(char *src, char *dst);
void far do_level_hacks(int level, int mode);
int far GetLevel(int level);
void far get_save_descs(char descs[][40], int *found);
void far ShowSaveRest(void);
void far DoSaveRest(int restore, int slot);
unsigned char far copy_file(char *srcdir, char *dstdir, char *name);
void far gruesome_door_hack(int x, int y);

/* OVR116.C: screenshots */
void far ovr116_194(int fd, char size);
void far ovr116_2A3(int fd, int bits);
int far GifPixel_ovr116_420(void);
void far save_screenshot(int seg);

#endif
