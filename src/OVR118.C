/* target: ovr118 */
/* opts: -mm -1 -G -O -Y -d */
#include <dos.h>
#include <string.h>

/* FM Towns names; the target table still identifies these DOS entries by IDA name. */

struct FontHead { unsigned width, height; };
extern struct FontHead far *cur_font;
extern unsigned char far *palette;
extern unsigned char far *bytefont;
extern unsigned long far *Time;
extern unsigned char fade_buffer[];
extern char far stdat;
extern unsigned char grfx_driver[];
extern unsigned char ShowStupidFirstPersonWeapon;
extern unsigned char font_name[];
extern char font_suffixes[][5];
extern char sys_suffix[];
extern char pals_name[];
void far seg021_22FD_755(void);
void far seg003_0272_4629(void);
void far set_the_window(int, int, int, int);
void far grSoftPageFlip(void);
void far seg003_0272_4933(void);
void far seg003_0272_4431(void);
void far mouse_hide(void);
void far seg021_22FD_791(void);
void far set_the_color(int);
void far seg003_0272_4D86(void);
void far mouse_show(void);
int far our_open(char *, int, int);
int far ReadFileToAddress(int, void far *, unsigned);
void far close(int);
void far lseek(int, long, int);
void far movedata(unsigned, unsigned, unsigned, unsigned, unsigned);
void far local_do_palette(int, char);
void far stub108_B6(void);
void far send_FB(void);
void far cFillFB(int);
unsigned far get_workspace(void);
void far set_workspace(void);
void far release_workspace(void);
void far CallbackFunctionSleepRelated_seg021_22FD_CB7(int);
void far Callback_seg021_22FD_CEA(int);
void far editchng(int);
void far render_FB(void);
void far attach_eye(int);
void far mouse_release(int);

void far grfx_quikfont(int n);
void far ovr118_0(void)
{
    seg021_22FD_755();
    seg003_0272_4629();
    set_the_window(0, 0xC7, 0x13F, 0);
    grSoftPageFlip();
    seg003_0272_4933();
    grfx_quikfont(1);
}

unsigned char far grfx_load_font(char *name)
{
    register int fd;
    if ((fd = our_open(name, 1, 0)) < 0) return 0;
    grfx_driver[12] = 1;
    ReadFileToAddress(fd, cur_font, 12);
    ReadFileToAddress(fd, bytefont, (cur_font->height + cur_font->width) << 7);
    close(fd);
    seg003_0272_4431();
    return 1;
}

void far grfx_quikfont(int n)
{
    font_name[4] = 0;
    strcat((char *)font_name, font_suffixes[n]);
    strcat((char *)font_name, sys_suffix);
    grfx_load_font((char *)font_name);
}

void far ovr118_D3(void)
{
    mouse_hide();
    seg021_22FD_791();
}

void far grfx_clear(void)
{
    mouse_hide();
    set_the_window(0, 0xC7, 0x13F, 0);
    set_the_color(0);
    seg003_0272_4D86();
    mouse_show();
}

unsigned char far read_quikpal(int n, void far *dest)
{
    register int fd;
    register int got;
    fd = our_open(pals_name, 1, 0);
    if (fd < 0) return 0;
    lseek(fd, (long)(n * 0x300), 0);
    got = ReadFileToAddress(fd, dest, 0x300);
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
    if (pump) stub108_B6();
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
            if (pump) stub108_B6();
            while (*Time - start < 8) ;
            grfx_setpal(buf);
            start = *Time;
        }
    }
}

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
    if (pump) stub108_B6();
    if (scale == 0) grfx_setpal(src);
    else {
        for (i = 0; i < 0x300; i++) acc[i] = 0;
        scale <<= 3;
        for (step = 0; step < scale; step++) {
            for (i = 0; i < 0x300; i++) {
                acc[i] += src[i];
                buf[i] = acc[i] / (unsigned)scale;
            }
            if (pump) stub108_B6();
            while (*Time - start < 8) ;
            grfx_setpal(buf);
            start = *Time;
        }
    }
}

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

void far fadeout3d(int n) { out3d(12, CallbackFunctionSleepRelated_seg021_22FD_CB7, 1); }
void far fadein3d(int n) { in3d(12, CallbackFunctionSleepRelated_seg021_22FD_CB7, 1); }
void far ovr118_534(int n) { out3d(n, Callback_seg021_22FD_CEA, 2); }
void far ovr118_54B(int n) { in3d(n, Callback_seg021_22FD_CEA, 2); }

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
