/* target: ovr150 */
/* opts: -mm -1 -G -O -Y -d */
/* Showing a full-screen picture from BYT.ARK, with or without changing the palette: the
   whole of DOS overlay ovr150, in original order. Function and global names are the
   originals from the FM Towns symbol table; the source file's own name is not known. */

#include <dos.h>

/* This file's _DATA, DS:1A78..1A7D (ovr149's data ends at 1A77, odd); ovr151's starts at
   1A7E with scdBlockHasBeenModified, which only ovr151 uses, as only this file uses this. */
char DataDirectory[] = "DATA\\";
extern char far Transparency;

void far grfx_clear(void);
void far grSoftPageFlip(void);
void far grPageFlip(void);
int far set_workspace(void);
unsigned far get_workspace(void);
void far release_workspace(void);
void far grfx_quikpal(int pal);
void far open_arc(int arc, char *dir);
int far get_arc(int arc, int blk, char far *buf);
void far close_arc(int arc);
void far set_the_window(int x0, int y0, int x1, int y1);
void far show(int x, int y, char far *buf, int h, int w, int a, int b);

char far disk_to_vid(int blk, char far *buf);

char far display_screen(int pal, int blk)
{
    char ok;

    if (pal >= 0) {
        grfx_clear();
        grSoftPageFlip();
        set_workspace();
    }
    ok = disk_to_vid(blk, 0L);
    if (pal >= 0) {
        grfx_quikpal(pal);
        grPageFlip();
        grSoftPageFlip();
    }
    return ok;
}

char far disk_to_vid(int blk, char far *buf)
{
    char trans = '*';
    unsigned ws = 0;
    int ok;

    if (blk < 0 || blk > 10)
        return 0;
    if (buf == 0)
        buf = MK_FP(ws = get_workspace(), 0);
    if (buf == 0)
        return 0;
    open_arc(4, DataDirectory);
    ok = get_arc(4, blk, buf);
    close_arc(4);
    if (ok) {
        set_the_window(0, 0xC7, 0x13F, 0);
        trans = Transparency;
        Transparency = 0;
        show(0, 0xC7, buf, 0xC8, 0x140, 0, 0);
        Transparency = trans;
    }
    if (ws)
        release_workspace();
    return trans != '*';
}
