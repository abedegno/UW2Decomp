/* sysentry.c: replaces seg021's start-up and entry modules: src/sys/SYSENTRY.ASM (the far C
   entries and the far pointers into seg021's data), STARTUP.ASM (_0, _17), SYSINIT.ASM (init
   and the exit routine), CPUTYPE.ASM, VIDSAVE.ASM and SYSLIBL.ASM (the video mode save), the
   joystick start-up of JOYPORT.ASM (_30A), TICKS.ASM (the game clock) and TICKREAD.ASM.

   seg021's data segment, dseg062_62a6 (FD71), is the 0C40h bytes mem/fardata.c loads from the
   EXE, and every offset below is its offset there, as in the assembly (SYSENTRY.ASM's header
   has the map). The entries no longer switch to seg021's private stack: the C has its own.

   The exit chain: SYSINIT.ASM's init puts its exit routine in the PSP's terminate address, so
   it runs however the program ends; here bc_exit (sys/borland.c) calls seg021_exit_chain. */
#include "compat.h"
#include "sys.h"
#include "port.h"

#define D dseg062_62a6
#define W(o) ((uint16_t)(D[(o)] | D[(o) + 1] << 8))
#define SETW(o, v) (D[(o)] = (uint8_t)(v), D[(o) + 1] = (uint8_t)((uint16_t)(v) >> 8))

/* TICKS.ASM: the game clock, 1/256 s, in seg021's code segment (FM Towns time). */
uint32 seg021_22FD_710;

/* SYSENTRY.ASM's _DATA: the far pointers C reaches seg021's data through. */
int16 *joy_position = (int16 *)(dseg062_62a6 + 0x1A0);
int16 *joy_buttons = (int16 *)(dseg062_62a6 + 0x1B0);
int16 *cJoyInit = (int16 *)(dseg062_62a6 + 0x1A8);
unsigned char *Shift = dseg062_62a6 + 0x1E2;
unsigned char *CapsLock = dseg062_62a6 + 0x1E8;
unsigned char *Alt = dseg062_62a6 + 0x1E3;
unsigned char *Ctrl = dseg062_62a6 + 0x1E4;
unsigned char *key_on = dseg062_62a6 + 0x1EB;
unsigned char *Asc = dseg062_62a6 + 0x10;
int16 *MouseDx = (int16 *)(dseg062_62a6 + 0x510);
int16 *MouseDy = (int16 *)(dseg062_62a6 + 0x512);
int16 *MouseOn = (int16 *)(dseg062_62a6 + 0x362);
uint32 *Time = &seg021_22FD_710;
int16 *cPerror = (int16 *)(dseg062_62a6 + 0x190);
char *cExitMessage = (char *)dseg062_62a6 + 0x514;

/* the other modules' routines */
void seg021_22FD_6C8(void);
void seg021_22FD_598(void);
void seg021_22FD_5C8(void);
void Read_Mouse_Motion_seg021_6E7(int16_t *x, int16_t *y);
int seg021_22FD_703(void);

static int initialised;

/* JOYPORT.ASM's _30A: find the connected axes. The port has no game port, so every axis
   times out: no centres, no axes (01D0 0), mask 0 (01D2). */
static void joy_init(void)
{
    memset(D + 0x1A0, 0, 0x20);
    D[0x1D2] = 0;
    SETW(0x1D0, 0);
}

/* VIDSAVE.ASM's _680 and _6A0 (SYSLIBL.ASM's _730): save the video mode (0350) and the BIOS
   equipment byte (0355), and set the equipment byte to colour. The port starts in text mode 3. */
static void video_save(void)
{
    D[0x350] = 3;
    D[0x355] = 0x20;
}

/* SYSINIT.ASM's init */
static void init(void)
{
    seg021_22FD_6C8();                  /* the mouse */
    SETW(0x110, 0x386);                 /* CPUTYPE.ASM's _1D0: a 386 or better */
    SETW(0x18A, 0x4FAF + PORT_LOAD_SEG);    /* _2B6: the 3D data's segment */
    /* setvect24 (CRITERR.ASM): the port's file errors are returned, never retried */
    /* _2BD: no Tandy */
    joy_init();
    seg021_22FD_598();                  /* the keyboard handler */
    video_save();
    initialised = 1;                    /* the exit routine is hooked */
}

/* The exit routine (SYSINIT.ASM's L028C): the exit hook (empty in UW2), the keyboard handler
   removed, and the '$'-terminated message at cPerror printed if there is one. */
void seg021_exit_chain(void)
{
    uint16_t m;
    if (!initialised) return;
    initialised = 0;
    seg021_22FD_5C8();
    if ((m = W(0x190)) != 0) {
        const unsigned char *s = D + m;
        size_t n = 0;
        while (m + n < DSEG062_62A6_SIZE && s[n] != '$') n++;
        write(1, s, (unsigned)n);       /* bc_write, through compat.h */
    }
}

/* _755: STARTUP.ASM's _0 (the PSP's terminate address, the EGA palette saved, init). */
void seg021_22FD_755(void)
{
    init();
}

/* _791: STARTUP.ASM's _17: the video mode back (_73E), the int 24h vector, the EGA palette,
   and the DOS clock set from the real-time clock; only the first matters to the port. */
void seg021_22FD_791(void)
{
    vga_set_mode(D[0x350]);
}

/* mouse: the motion into MouseDx and MouseDy (FD71:0510, 0512). */
void mouse(void)
{
    int16_t x, y;
    Read_Mouse_Motion_seg021_6E7(&x, &y);
    SETW(0x510, x);
    SETW(0x512, y);
}

/* mbuttons: the buttons, bits 0 and 1. */
int mbuttons(void)
{
    return seg021_22FD_703();
}

/* TICKREAD.ASM's _C00: the low word of the clock. */
unsigned seg021_22FD_C00(void)
{
    return (uint16_t)seg021_22FD_710;
}
