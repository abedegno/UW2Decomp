/* target: ovr118 */
/* opts: -mm -1 -G -O -Y -d */
/* Graphics start-up, fonts and palettes, and the screen fades: the whole of DOS overlay
   ovr118.

   grfx_init starts the graphics library (seg003, through seg021's entry points), sets the
   full-screen window and loads the default font. Fonts are DATA\FONTxxxx.SYS files
   (enum Font, gfx.h): grfx_load_font reads the 12-byte header into cur_font (struct
   FontInfo) and 128 characters into bytefont, then setup_font (GRCORE.ASM) takes them up.
   Palettes are 768-byte records of DATA\PALS.DAT (read_quikpal); grfx_setpal and
   grfx_palrange copy into the library's palette and load the DAC with local_do_palette
   (MODEX.ASM). fadein and fadeout ramp the whole palette between black and a target in
   count * 8 steps, 8 ticks of *Time apart, through fade_buffer; the cutscene player
   (CUTS.C) uses them with its speech pump. out3d and in3d dissolve the 3D view, one step
   per call of a seg021 callback into the graphics library, each step sent to the screen
   with send_FB (PANELS.C); fill_FB fills the view with one colour (damage and spell
   flashes). The file owns fade_buffer and the font and palette file names.

   name: inferred, from the grfx_ prefix of the FM Towns names (grfx_init, grfx_load_font,
   grfx_setpal ...). */
#include <dos.h>
#include <string.h>
#include "file.h"
#include "gfx.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"

/* name: FM Towns names; the target table still identifies these DOS entries by IDA name. */

extern struct FontInfo far *cur_font;
extern unsigned long far *Time;
/* match: this file's _BSS, DS:5E34..6733 (ovr116's ends at 5E33; ovr119's starts at 6734, its
   keys starting again from gsize's 119): only this file uses it. */
unsigned char fade_buffer[0x900];
extern char far stdat;
extern unsigned char ShowStupidFirstPersonWeapon;
/* match: the file's _DATA, DS:14C4 to DS:14FE. FM Towns has these as statics after its public
   _grfx_driver (font_name is _grfx_driver+0xD there, font_suffixes +0x1A), which is FM
   Towns' own graphics driver block; DOS has no such block, and its first 12 bytes would
   overlap ovr117's data. The byte grfx_load_font sets (_grfx_driver+0xC in FM Towns) is
   the first byte of this file's data: a static with no recovered name. */
static unsigned char font_loaded = 0;
unsigned char font_name[13] = "font????.sys";
char font_suffixes[6][5] = { "4x5p", "5x6p", "char", "big", "5x6i", "butn" };
char sys_suffix[] = ".sys";
char pals_name[] = "pals.dat";
void far close(int);
void far lseek(int, long, int);
void far movedata(unsigned, unsigned, unsigned, unsigned, unsigned);
unsigned far get_workspace(void);
void far mouse_release(int);

void far grfx_init(void)
{
    seg021_22FD_755();
    init_graphics();
    set_the_window(0, 0xC7, 0x13F, 0);
    grSoftPageFlip();
    init_colors();
    grfx_quikfont(FONT_5X6P);
}

/* Loads a font file: its header into cur_font, then (charsize + widthsize) * 128 bytes of
   character bitmaps and widths into bytefont. Returns 0 if the file cannot be opened. */
unsigned char far grfx_load_font(char *name)
{
    register int fd;
    if ((fd = our_open(name, 1, 0)) < 0) return 0;
    font_loaded = 1;
    intoFarBuffer_ovr167_5DA(fd, cur_font, 12);
    intoFarBuffer_ovr167_5DA(fd, bytefont, (cur_font->charsize + cur_font->widthsize) << 7);
    close(fd);
    setup_font();
    return 1;
}

void far grfx_quikfont(int n)
{
    font_name[4] = 0;
    strcat((char *)font_name, font_suffixes[n]);
    strcat((char *)font_name, sys_suffix);
    grfx_load_font((char *)font_name);
}

void far grfx_close(void)
{
    mouse_hide();
    seg021_22FD_791();
}

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
    fd = our_open(pals_name, 1, 0);
    if (fd < 0) return 0;
    lseek(fd, (long)(n * 0x300), 0);
    got = intoFarBuffer_ovr167_5DA(fd, dest, 0x300);
    close(fd);
    if (got != 0x300) return 0;
    return 1;
}

unsigned char far grfx_quikpal(int n)
{
    if (read_quikpal(n, palette)) {
        local_do_palette(0x100, 0);
        return 1;
    }
    return 0;
}

void far grfx_setpal(void far *src)
{
    movedata(FP_SEG(src), FP_OFF(src), FP_SEG(palette), FP_OFF(palette), 0x300);
    local_do_palette(0x100, 0);
}

void far grfx_palrange(void far *src, int start, int count)
{
    movedata(FP_SEG(src), FP_OFF(src) + start * 3, FP_SEG(palette), FP_OFF(palette) + start * 3, count * 3);
    local_do_palette(count, (char)start);
}

/* Fades from palette src to black. count 0 goes black at once; otherwise the palette steps
   down linearly in count * 8 steps, each 8 ticks of *Time after the last (with *Time at
   256 Hz, as the cutscene work measured, count / 4 seconds). acc holds each entry scaled
   by the number of steps. pump calls the cutscene speech streamer before each step, so
   speech keeps playing during the fade. */
void far fadeout(unsigned char far *src, int count, int pump)
{
    int step;
    unsigned char far *buf;
    unsigned far *acc;
    unsigned long start;
    register int i;
    register int scale = count;
    buf = fade_buffer;
    acc = (unsigned far *)(buf + 0x300);
    start = *Time;
    if (pump) anm_sound_callback();
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
            if (pump) anm_sound_callback();
            while (*Time - start < 8) ;
            grfx_setpal(buf);
            start = *Time;
        }
    }
}

/* Fades from black to palette src, the reverse of fadeout. */
void far fadein(unsigned char far *src, int count, int pump)
{
    int step;
    unsigned char far *buf;
    unsigned far *acc;
    unsigned long start;
    register int i;
    register int scale = count;
    buf = fade_buffer;
    acc = (unsigned far *)(buf + 0x300);
    start = *Time;
    if (pump) anm_sound_callback();
    if (scale == 0) grfx_setpal(src);
    else {
        for (i = 0; i < 0x300; i++) acc[i] = 0;
        scale <<= 3;
        for (step = 0; step < scale; step++) {
            for (i = 0; i < 0x300; i++) {
                acc[i] += src[i];
                buf[i] = acc[i] / (unsigned)scale;
            }
            if (pump) anm_sound_callback();
            while (*Time - start < 8) ;
            grfx_setpal(buf);
            start = *Time;
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

/* The reverse of out3d: keeps a copy of the rendered view (the 0x6800 bytes, 208 by 128,
   at the start of stdat) in the workspace, shows colour, then for count .. 1 restores the
   copy after each callback(i) and send. Does nothing if the workspace is in use. */
void far in3d(int count, void (far *callback)(int), int colour)
{
    unsigned far *screen;
    register int i = count;
    register int seg;
    mouse_hide();
    if ((seg = get_workspace()) == 0) {
        mouse_show();
        return;
    }
    {
        screen = MK_FP(seg, 0);
        movedata(FP_SEG(&stdat), FP_OFF(&stdat), FP_SEG(screen), FP_OFF(screen), 0x6800);
        ShowStupidFirstPersonWeapon = 0;
        cFillFB(colour);
        send_FB();
        while (i > 0) {
            callback(i);
            send_FB();
            set_workspace();
            movedata(FP_SEG(screen), FP_OFF(screen), FP_SEG(&stdat), FP_OFF(&stdat), 0x6800);
            i--;
        }
        send_FB();
        ShowStupidFirstPersonWeapon = 1;
        release_workspace();
    }
    mouse_show();
}

/* fadeout3d and fadein3d ignore n and always take 12 steps of seg021's callback (which
   calls the graphics library at seg003_0272_764); new_player_pos uses them around a
   teleport. */
void far fadeout3d(int n) { out3d(12, CallbackFunctionSleepRelated_seg021_22FD_CB7, 1); }
void far fadein3d(int n) { in3d(12, CallbackFunctionSleepRelated_seg021_22FD_CB7, 1); }
/* DOS only: fadeout3d and fadein3d over n steps with another callback (seg003_0272_788).
   match: the names are provisional (IDA's ovr118_534 and ovr118_54B), chosen so that their
   tools/bssorder.py keys put them in the EXE's overlay stub order. */
void far steps_fadeout3d_ovr118_534(int n) { out3d(n, Callback_seg021_22FD_CEA, 2); }
void far steps_fadein3d_ovr118_54B(int n) { in3d(n, Callback_seg021_22FD_CEA, 2); }

/* Fills the 3D view with one colour and shows it at once (a flash), then asks for a
   redraw (editchng(2)). */
void far fill_FB(int colour)
{
    cFillFB(colour);
    mouse_hide();
    ShowStupidFirstPersonWeapon = 0;
    send_FB();
    ShowStupidFirstPersonWeapon = 1;
    mouse_show();
    editchng(2);
}

/* The crystal ball's view (PLAYER.C crystal_ball, which sets campos and camang first):
   fade to the detached camera (attach_eye(-1)), stay there while the mouse button is held
   (mouse_release, MOUSE.C), then fade back to the player's eye. */
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
