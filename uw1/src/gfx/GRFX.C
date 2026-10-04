/* target: ovr114 */
/* opts: -mm -1 -G -O -Y -d */
/* Graphics start-up, fonts and palettes, and the screen fades: the whole of UW1's DOS
   overlay ovr114, in original order. UW1 has no symbol-bearing build; the names are UW2's
   (the FM Towns symbol table; UW2's GRFX.C, overlay ovr118), the code being the same
   routines.

   grfx_init starts the graphics library (seg003, through seg019's entry points), sets the
   full-screen window and loads the default font. Fonts are DATA\FONT*.SYS files: the
   loader (here grfx_load_font, see below) reads the 12-byte header into cur_font (struct
   FontInfo) and the characters into bytefont, then setup_font takes them up. Palettes
   are 768-byte records of DATA\PALS.DAT (read_quikpal); grfx_setpal copies into the
   library's palette and loads the DAC with local_do_palette. fadein and fadeout ramp the whole palette between
   black and a target in count * 8 steps, 8 ticks of *Time apart, through fade_buffer.
   out3d and in3d dissolve the 3D view, one step per call of a seg019 callback into the
   graphics library, each step sent to the screen with send_FB; fill_FB fills the view
   with one colour. The file owns fade_buffer and the font and palette file names.

   UW1's differences, marked "UW1:" where they are not obvious: the font loader takes
   the font's file name (it is UW2's grfx_load_font; UW1 has no font index, and callers
   pass "fontbig.sys" and the like) and opens it in DATA\ with the C library's open;
   read_quikpal opens "data/pals.dat" the same way; there is no grfx_palrange; fadein
   and fadeout have no speech pump argument; the 3D fades fill with colours 0xF1 and 0x60 (UW2 1 and 2).

   name: inferred, from the grfx_ prefix of the FM Towns names (grfx_init, grfx_load_font,
   grfx_setpal ...). The font loader, the target table's OpenFont_ovr114_35, is called
   grfx_load_font because symbols.tsv has that name for it (BAGS.C's merge). The
   evidence says it is UW2's grfx_load_font: kin pairs it with that function (UW2's
   grfx_load_font, which builds the name from an index, has no UW1 counterpart), and the
   overlay's stub table puts its entry between fill_FB's (bssorder.py key 342) and
   fadein's (558), where grfx_load_font's key (343) falls and grfx_load_font's (279) does
   not (it would come before fadeout3d, 318). So the name should be grfx_load_font. */
#include <dos.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <mem.h>
#include "file.h"
#include "gfx.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"

/* Declared in each file that uses it, its own way (no header). */
unsigned char far grfx_init(void);
void far grfx_clear(void);
unsigned char far read_quikpal(int n, void far *dest);

unsigned char far grfx_load_font(char *name);
/* UW1: seg019's start-up and shut-down and the two 3D fade callbacks (UW2's seg021_22FD_755,
   _791, CallbackFunctionSleepRelated_seg021_22FD_CB7 and Callback_seg021_22FD_CEA); the
   listing's names (seg019_CEA is not a procedure in the listing). */

/* This file's _DATA, DS:15BC..15DD: the byte grfx_load_font sets (a static with no recovered
   name, as in UW2), then the string pool. */
static unsigned char font_loaded = 0;

/* match: no return statement; callers may test what grfx_load_font leaves in AL. */
unsigned char far grfx_init(void)
{
    seg019_755();
    init_graphics();
    set_the_window(0, 0xC7, 0x13F, 0);
    grSoftPageFlip();
    init_colors();
    grfx_load_font("font5x6p.sys");
    AX_RESULT(font_loaded);
}

/* Loads the font file DATA\name: its header into cur_font, then (charsize + widthsize) *
   128 bytes of character bitmaps and widths into bytefont. Returns 0 if the file cannot be
   opened. */
unsigned char far grfx_load_font(char *name)
{
    char path[0x42];
    register int fd;

    strcpy(path, "DATA\\");
    strcat(path, name);
    if ((fd = open(path, O_RDONLY | O_BINARY)) < 0) return 0;
    font_loaded = 1;
    intoFarBuffer_ovr167_5DA(fd, cur_font, 12);
    intoFarBuffer_ovr167_5DA(fd, bytefont, (cur_font->charsize + cur_font->widthsize) << 7);
    close(fd);
    setup_font();
    return 1;
}

/* Hides the cursor and runs seg019's shut-down glue (SYSENTRY.ASM's _791), which puts
   the screen mode back. */
void far grfx_close(void)
{
    mouse_hide();
    seg019_791();
}

/* Clears the whole screen to colour 0. */
void far grfx_clear(void)
{
    mouse_hide();
    set_the_window(0, 0xC7, 0x13F, 0);
    set_the_color(0);
    clear_window();
    mouse_show();
}

/* Reads palette n of PALS.DAT (0x300 bytes, 6-bit RGB) into dest; 1 if all of it was read. */
unsigned char far read_quikpal(int n, void far *dest)
{
    register int fd;
    register int got;
    fd = open("data/pals.dat", O_RDONLY | O_BINARY);
    if (fd < 0) return 0;
    lseek(fd, (int32)(n * 0x300), 0);
    got = intoFarBuffer_ovr167_5DA(fd, dest, 0x300);
    close(fd);
    if (got != 0x300) return 0;
    return 1;
}

/* Loads palette n of DATA\PALS.DAT (read_quikpal) into the VGA; 0 if it could not be
   read. */
unsigned char far grfx_quikpal(int n)
{
    if (read_quikpal(n, palette)) {
        local_do_palette(0x100, 0);
        return 1;
    }
    return 0;
}

/* Copies a 768-byte palette into the library's palette and loads it into the VGA. */
void far grfx_setpal(void far *src)
{
    FAR_COPY(palette, src, 0x300);
    local_do_palette(0x100, 0);
}

/* Fades from palette src to black. count 0 goes black at once; otherwise the palette steps
   down linearly in count * 8 steps, each 8 ticks of *Time after the last (with *Time at
   256 Hz, as the cutscene work measured, count / 4 seconds). acc holds each entry scaled
   by the number of steps. */
void far fadeout(unsigned char far *src, int count)
{
    int step;
    unsigned char far *buf;
    uint16 far *acc;
    uint32 start;
    register int i;
    register int scale = count;
    buf = fade_buffer;
    acc = (uint16 far *)(buf + 0x300);
    start = GAME_TIME();
    if (scale == 0) {
        for (i = 0; i < 0x300; i++) buf[i] = 0;
        grfx_setpal(buf);
    } else {
        scale <<= 3;
        for (i = 0; i < 0x300; i++) acc[i] = scale * src[i];
        for (step = 0; step < scale; step++) {
            for (i = 0; i < 0x300; i++) {
                acc[i] -= src[i];
                buf[i] = acc[i] / (unsigned)scale;
            }
            while (GAME_TIME() - start < 8) ;
            grfx_setpal(buf);
            start = GAME_TIME();
        }
    }
}

/* Fades from black to palette src, the reverse of fadeout. */
void far fadein(unsigned char far *src, int count)
{
    int step;
    unsigned char far *buf;
    uint16 far *acc;
    uint32 start;
    register int i;
    register int scale = count;
    buf = fade_buffer;
    acc = (uint16 far *)(buf + 0x300);
    start = GAME_TIME();
    if (scale == 0) grfx_setpal(src);
    else {
        for (i = 0; i < 0x300; i++) acc[i] = 0;
        scale <<= 3;
        for (step = 0; step < scale; step++) {
            for (i = 0; i < 0x300; i++) {
                acc[i] += src[i];
                buf[i] = acc[i] / (unsigned)scale;
            }
            while (GAME_TIME() - start < 8) ;
            grfx_setpal(buf);
            start = GAME_TIME();
        }
    }
}

/* Calls callback(0) .. callback(count), sending the 3D view to the screen after each, then
   fills the view with colour. The first-person weapon is not drawn meanwhile. */
void far out3d(int count, void (far *callback)(int), int colour)
{
    register int i;
    register int limit = count;
    mouse_hide();
    ShowStupidFirstPersonWeapon = 0;
    for (i = 0; i <= limit; i++) { callback(i); send_FB(); }
    cFillFB(colour);
    send_FB();
    ShowStupidFirstPersonWeapon = 1;
    mouse_show();
}

/* The reverse of out3d: keeps a copy of the rendered view (UW1: the first 0x4BEC bytes
   of stdat; UW2 has 0x6800) in the workspace, shows colour, then for count .. 1 restores the
   copy after each callback(i) and send. Does nothing if the workspace is in use. */
void far in3d(int count, void (far *callback)(int), int colour)
{
    uint16 far *screen;
    register int i = count;
    register int seg;
    mouse_hide();
    if ((seg = get_workspace()) == 0) {
        mouse_show();
        return;
    }
    {
        screen = MK_FP(seg, 0);
        FAR_COPY(screen, stdat, 0x4BEC);
        ShowStupidFirstPersonWeapon = 0;
        cFillFB(colour);
        send_FB();
        while (i > 0) {
            callback(i);
            send_FB();
            set_workspace();
            FAR_COPY(stdat, screen, 0x4BEC);
            i--;
        }
        send_FB();
        ShowStupidFirstPersonWeapon = 1;
        release_workspace();
    }
    mouse_show();
}

/* fadeout3d and fadein3d ignore n and always take 12 steps of seg019's callback; UW2's
   new_player_pos uses them around a teleport. */
void far fadeout3d(int n) { out3d(12, seg019_CB7, 0xF1); }
void far fadein3d(int n) { in3d(12, seg019_CB7, 0xF1); }
/* fadeout3d and fadein3d over n steps with another callback.
   name: UW2's provisional names (chosen there for its stub order); their bssorder.py keys
   fit UW1's stub order too. */
void far steps_fadeout3d_ovr118_534(int n) { out3d(n, seg019_CEA, 0x60); }
void far steps_fadein3d_ovr118_54B(int n) { in3d(n, seg019_CEA, 0x60); }

/* Fills the 3D view with one colour and shows it at once (a flash). UW1: no editchng(2)
   after it. */
void far fill_FB(int colour)
{
    cFillFB(colour);
    mouse_hide();
    ShowStupidFirstPersonWeapon = 0;
    send_FB();
    ShowStupidFirstPersonWeapon = 1;
    mouse_show();
}

/* The crystal ball's view (UW2: PLAYER.C crystal_ball, which sets campos and camang first):
   fade to the detached camera (attach_eye(-1)), stay there while the mouse button is held
   (mouse_release), then fade back to the player's eye. */
void far cameras_fade(void)
{
    render_FB();
    attach_eye(-1);
    fadeout3d(5);
    render_FB();
    fadein3d(5);
    mouse_release(1);
    render_FB();
    fadeout3d(5);
    attach_eye(1);
    render_FB();
    fadein3d(5);
}
