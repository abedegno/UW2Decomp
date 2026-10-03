/* dir.h for the port: Borland's directory search, which the save code uses to copy a save
   directory (GAMEWRAP.C). The port matches DOS names case-insensitively (docs/PORT.md). */
#ifndef UW2_PORT_DIR_H
#define UW2_PORT_DIR_H
#define MAXPATH 80
#define MAXDRIVE 3
#define MAXDIR 66
#define MAXFILE 9
#define MAXEXT 5
#define FA_RDONLY 0x01
#define FA_HIDDEN 0x02
#define FA_SYSTEM 0x04
#define FA_LABEL 0x08
#define FA_DIREC 0x10
#define FA_ARCH 0x20
/* Borland's 43 bytes, packed (compat.h packs the game's view of it, and borland.c, which
   fills it, packs its own copy the same way: they must agree, or ff_name is read where it is
   not); the size is DOS's 32-bit long. */
#pragma pack(push, 1)
struct ffblk {
    char ff_reserved[21];
    char ff_attrib;
    unsigned short ff_ftime;
    unsigned short ff_fdate;
    int ff_fsize;
    char ff_name[13];
};
#pragma pack(pop)
int findfirst(const char *path, struct ffblk *ff, int attrib);
int findnext(struct ffblk *ff);
int getcurdir(int drive, char *dir);
int getdisk(void);
int setdisk(int drive);
int fnsplit(const char *path, char *drive, char *dir, char *name, char *ext);
void fnmerge(char *path, const char *drive, const char *dir, const char *name, const char *ext);
#endif
