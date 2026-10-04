/* target: ovr141 */
/* opts: -mm -1 -G -O -Y -d */
/* Showing a full-screen picture file, with or without changing the palette: the whole of
   DOS overlay ovr141, one function.

   UW1 against UW2: this is UW1's counterpart of UW2's SHOWPIC.C (ovr150), whose
   display_screen and disk_to_vid draw a block of BYT.ARK. UW1 has no BYT.ARK; its
   full-screen pictures are separate .BYT files (the main menu's DATA\opscr.byt), so the
   one routine takes a file name, reads the whole 320 by 200 picture (0xFA00 bytes) with
   bltfromdrive into the shared workspace, or into a farmalloc'd buffer when the workspace
   is lent out, and draws it with show. pal >= 0 clears the screen and draws on the hidden
   page, then loads palette pal and flips, as UW2's display_screen does; pal < 0 draws
   straight to the current page. It returns 0 only when it had no buffer: a file that
   could not be read still returns 1.

   File name: UW2's, for the same job (the original UW1 name is unknown).
   name: LoadBitMap_ovr141_0, the listing's descriptive name, which MAINMENU.C calls. UW2's
   display_screen takes a block number, not a file name, so its name is not reused. */

#include <alloc.h>
#include <dos.h>
#include "file.h"
#include "gfx.h"
#include "sys.h"

char far LoadBitMap_ovr141_0(int pal, char *name)
{
    unsigned char far *buf;
    char trans;
    unsigned ws;

    if ((ws = get_workspace()) == 0)
        buf = farmalloc(0xFA00L);
    else
        buf = MK_FP(ws, 0);
    if (buf != 0 && bltfromdrive(name, buf, 0xFA00)) {
        if (pal >= 0) {
            grfx_clear();
            grSoftPageFlip();
        }
        set_the_window(0, 0xC7, 0x13F, 0);
        trans = Transparency;
        Transparency = 0;
        show(0, 0xC7, buf, 0xC8, 0x140, 0, 0);
        Transparency = trans;
        if (pal >= 0) {
            grfx_quikpal(pal);
            grPageFlip();
            grSoftPageFlip();
        }
    }
    if (ws)
        release_workspace();
    else if (buf)
        farfree(buf);
    else
        return 0;
    return 1;
}
