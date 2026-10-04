/* glue.c: replaces nothing. Where the translated modules (x86/asmrt.h) reach code that is
   hand-written C in the port: seg019's entries the renderer far-calls, seg003's routines that
   are hand-written, and the DOS and EMS interrupts. Each does the routine's work on the
   emulated registers and returns as the routine did. */
#include <stdio.h>
#include "x86/asmrt.h"

char seg012_10F(char physical, unsigned logical);     /* mem/ems.c */
int bc_open(const char *path, int access, ...);
int bc_read(int fd, void *buf, unsigned n);
int bc_write(int fd, const void *buf, unsigned n);
int bc_close(int fd);

void MousQUp(char from3d);                      /* ui/MOUSE.C */

/* seg019's _102B (C3DENTRY.ASM), do_mouseq's: MousQUp(1) on DGROUP's small stack below
   joy_position, with SI, DS and ES kept; the private stack it saves and restores around the
   call is the emulated one, which the C does not touch. */
static uint32_t g_102b(void)
{
    uint16_t si = SI, ds = asm_ds, es = asm_es;
    MousQUp(1);
    SI = si;
    SET_DS(ds);
    SET_ES(es);
    return asm_glue_retf();
}

/* The game's C that the translated assembly far-calls (SPRITE.ASM's update_sprites), each at
   its DOS address: the arguments are on the emulated stack above the return address, as
   Turbo C's callee reads them, and the caller pops them. */
void mouse_hide(void);                          /* ui/MOUSE.C */
void mouse_show(void);
void pic_to_screen(int icon, int x, int y, int height, int width);     /* gfx/GRSPIC.C */
void mask_to_screen(int icon, int x, int y, int width, int height, int clip);
#define ARG(i) ((int16_t)rw(pSS, (uint16_t)(SP + 4 + 2 * (i))))
static uint32_t g_mouse_hide(void) { mouse_hide(); return asm_glue_retf(); }
static uint32_t g_mouse_show(void) { mouse_show(); return asm_glue_retf(); }
static uint32_t g_pic_to_screen(void)
{
    pic_to_screen(ARG(0), ARG(1), ARG(2), ARG(3), ARG(4));
    return asm_glue_retf();
}
static uint32_t g_mask_to_screen(void)
{
    mask_to_screen(ARG(0), ARG(1), ARG(2), ARG(3), ARG(4), ARG(5));
    return asm_glue_retf();
}

unsigned seg019_C00(void);                      /* sys/sysentry.c: TICKREAD.ASM */

/* seg019's _C00 (TICKREAD.ASM): AX = the game clock's low word. */
static uint32_t g_c00(void)
{
    AX = (uint16_t)seg019_C00();
    return asm_glue_retf();
}

/* seg019's _C10 (SYSLIBP.ASM), render_3d's hook: a bare retf. */
static uint32_t g_c10(void)
{
    return asm_glue_retf();
}

const struct asm_glue asm_glues[] = {
    { 0x1F3A, 0x0C00, g_c00, "TICKREAD _C00" },
    { 0x1F3A, 0x0C10, g_c10, "SYSLIBP _C10" },
    { 0x1ADC, 0x00D7, g_mouse_hide, "MOUSE.C mouse_hide" },
    { 0x1ADC, 0x00C1, g_mouse_show, "MOUSE.C mouse_show" },
    { 0x1A31, 0x0265, g_pic_to_screen, "GRSPIC.C pic_to_screen" },
    { 0x1A31, 0x034D, g_mask_to_screen, "GRSPIC.C mask_to_screen" },
    { 0x1F3A, 0x102B, g_102b, "C3DENTRY _102B (MousQUp)" },
};
const int asm_nglues = sizeof asm_glues / sizeof asm_glues[0];

/* The interrupts the translated modules make: int 10h's mode set (VIDMODE.ASM), int 21h's
   string print (MODEX.ASM's console print) and file calls, int 67h's page mapping. Anything
   else stops the port with its number. */
uint32_t asm_int(uint8_t n)
{
    char why[80];
    if (n == 0x10) {
        switch (AH) {
        case 0x00:
            vga_set_mode(AL);
            return 0;
        case 0x1A:                      /* read the display combination: a VGA with a colour
                                           display, code 08h (VIDMODE.ASM's _33FB wants 7 or 8) */
            if (AL != 0) break;
            AL = 0x1A;
            BX = 0x0008;
            return 0;
        default:
            break;
        }
    } else if (n == 0x21) {
        switch (AH) {
        case 0x09: {                    /* print the '$'-terminated string at DS:DX */
            uint16_t i = DX;
            while (rb(pDS, i) != '$') {
                char c = (char)rb(pDS, i);
                bc_write(1, &c, 1);
                i++;
            }
            AL = '$';
            return 0;
        }
        case 0x25:                      /* set int AL's vector to DS:DX: cRender points int 0 at
                                           the renderer's handler, FD72:04D0 (SYSENTRY.ASM's map);
                                           a translated divide that faults goes to asm_divfault
                                           (x86/divfault.c) instead */
            if (AL != 0) break;
            return 0;
        case 0x3D: {
            const char *path = (const char *)(pDS + DX);
            int fd = bc_open(path, 0x8001);     /* Borland's O_RDONLY | O_BINARY */
            if (fd < 0) { AX = 2; CF = 1; } else { AX = (uint16_t)fd; CF = 0; }
            return 0;
        }
        case 0x3F: {
            int r = bc_read(BX, pDS + DX, CX);
            if (r < 0) { AX = 5; CF = 1; } else { AX = (uint16_t)r; CF = 0; }
            return 0;
        }
        case 0x3E:
            CF = bc_close(BX) < 0;
            return 0;
        default:
            break;
        }
    } else if (n == 0x67) {
        switch (AH) {
        case 0x44:
            AH = seg012_10F((char)AL, BX) ? 0 : 0x8A;
            return 0;
        default:
            break;
        }
    }
    snprintf(why, sizeof why, "int %02Xh function %02Xh is not emulated", n, AH);
    port_halt(why);
}
