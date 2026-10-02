/* target: ovr150 */
/* opts: -mm -1 -G -O -Y -d */
/* Showing a full-screen picture from BYT.ARK, with or without changing the palette: the
   whole of DOS overlay ovr150, in original order.

   BYT.ARK (archive 4 of ARC.C) holds the full-screen 320 by 200 8-bit pictures. The callers
   name the blocks: 0 the automap's background (AUTOMAP.C), 1 character creation
   (CHARGEN.C), 4 the screen drawn under the game's panels when play starts (UWEDIT.C
   strt_demscr, before init_gamedisp), 5 the main menu (MAINMENU.C), 6 and 7
   the Origin and Looking Glass logos shown at start-up with palettes 5 and 6, 8 (with
   palette 7) and 9 the end of the game (SKILLS.C). disk_to_vid reads a block into a buffer (the shared workspace by default,
   TMPALLOC.C) and draws it opaque with show (GRCORE.ASM); display_screen wraps it with a
   clear, a page flip and a palette from PALS.DAT (GRFX.C grfx_quikpal). The file owns only
   the string "DATA\\".

   name: descriptive; display_screen and disk_to_vid are the FM Towns names. */

#include <dos.h>
#include "file.h"
#include "gfx.h"
#include "sys.h"

/* match: this file's _DATA, DS:1A78..1A7D (ovr149's data ends at 1A77, odd); ovr151's starts at
   1A7E with scdBlockHasBeenModified, which only ovr151 uses, as only this file uses this. */
char DataDirectory[] = "DATA\\";
extern char far Transparency;

void far grfx_clear(void);
unsigned far get_workspace(void);
void far grfx_quikpal(int pal);
int far get_arc(int arc, int blk, char far *buf);
void far close_arc(int arc);
void far show(int x, int y, char far *buf, int h, int w, int a, int b);

char far disk_to_vid(int blk, char far *buf);

/* Shows block blk. pal >= 0: clear the screen, draw on the hidden page, then load palette
   pal and flip; pal < 0: draw straight to the current page with the current palette.
   Returns disk_to_vid's result. */
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

/* Reads BYT.ARK block blk (0..10) into buf (the workspace if buf is 0) and draws it over
   the whole screen with Transparency off. Returns nonzero if it was drawn: trans starts
   as '*' and only takes Transparency's value once the block has been read, so the result
   is wrong only if Transparency itself were '*'. */
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
