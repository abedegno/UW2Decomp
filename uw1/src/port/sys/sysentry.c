/* sysentry.c: replaces seg019's start-up and entry modules: src/sys/SYSENTRY.ASM (the far C
   entries and the far pointers into seg019's data), STARTUP.ASM (_0, _17), SYSINIT.ASM (init
   and the exit routine), CPUTYPE.ASM, VIDSAVE.ASM and SYSLIBL.ASM (the video mode save), the
   joystick start-up of JOYPORT.ASM (_30A), TICKS.ASM (the game clock) and TICKREAD.ASM.
   seg019 is UW2's seg021 almost byte for byte (docs/NOTES.md), so this is UW2Decomp's
   sysentry.c with UW1's names and offsets.

   seg019's data segment, seg063 (FD72), is the 0AB0h bytes mem/fardata.c loads from the EXE,
   and every offset below is its offset there, as in the assembly (SYSENTRY.ASM's header has
   the map). UW1's private stack tops out at 448h, where UW2's is at 510h, so MouseDx, MouseDy
   and cExitMessage are at 0448, 044A and 044C. The entries no longer switch to that stack: the
   C has its own.

   The exit chain: SYSINIT.ASM's init puts its exit routine in the PSP's terminate address, so
   it runs however the program ends; here bc_exit (Exhume's sys/borland.c) calls
   seg019_exit_chain (portgame.h). */
#include "compat.h"
#include "sys.h"
#include "port.h"
#include "x86/asmrt.h"

#define D seg063
#define W(o) ((uint16_t)(D[(o)] | D[(o) + 1] << 8))
#define SETW(o, v) port_setw(&D[(o)], (uint16_t)(v))

/* TICKS.ASM: the game clock, 1/256 s, a doubleword in seg019's code segment at 0710h (SOUND.C's
   cllbck_tst counts it through Time), kept where DOS keeps it, in the code block (asmgame.h), so
   that the translated modules that read it through cs: (C3DENTRY.ASM's cInit3d) see it. */
#define CLOCK ((uint32 *)(CODE019 + 0x710))

/* SYSENTRY.ASM's _DATA: the far pointers C reaches seg019's data through. UW1 has no cJoyInit. */
int16 *joy_position = (int16 *)(seg063 + 0x1A0);
int16 *joy_buttons = (int16 *)(seg063 + 0x1B0);
unsigned char *Shift = seg063 + 0x1E2;
unsigned char *CapsLock = seg063 + 0x1E8;
unsigned char *Alt = seg063 + 0x1E3;
unsigned char *Ctrl = seg063 + 0x1E4;
unsigned char *key_on = seg063 + 0x1EB;
unsigned char *Asc = seg063 + 0x10;
int16 *MouseDx = (int16 *)(seg063 + 0x448);
int16 *MouseDy = (int16 *)(seg063 + 0x44A);
int16 *MouseOn = (int16 *)(seg063 + 0x362);
uint32 *Time = CLOCK;
int16 *cPerror = (int16 *)(seg063 + 0x190);
char *cExitMessage = (char *)seg063 + 0x44C;

/* the other modules' routines (sys/keyqueue.c, sys/mousedrv.c) */
void seg019_6C8(void);
void seg019_598(void);
void seg019_5C8(void);
void Read_Mouse_Motion_seg019_6E7(int16_t *x, int16_t *y);
int seg019_703(void);

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
    seg019_6C8();                       /* the mouse */
    SETW(0x110, 0x386);                 /* CPUTYPE.ASM's _1D0: a 386 or better */
    SETW(0x18A, 0x4723 + PORT_LOAD_SEG);    /* _2B6: seg051's segment, the 3D data */
    /* setvect24 (CRITERR.ASM): the port's file errors are returned, never retried */
    /* _2BD: no Tandy */
    joy_init();
    seg019_598();                       /* the keyboard handler */
    video_save();
    initialised = 1;                    /* the exit routine is hooked */
}

/* The exit routine (SYSINIT.ASM's L028C): the exit hook, the keyboard handler removed, and the
   '$'-terminated message at cPerror printed if there is one. */
void seg019_exit_chain(void)
{
    uint16_t m;
    if (!initialised) return;
    initialised = 0;
    seg019_5C8();
    if ((m = W(0x190)) != 0) {
        const unsigned char *s = D + m;
        size_t n = 0;
        while (m + n < SEG063_SIZE && s[n] != '$') n++;
        write(1, s, (unsigned)n);       /* bc_write, through compat.h */
    }
}

/* _755: STARTUP.ASM's _0 (the PSP's terminate address, the EGA palette saved, init). */
void seg019_755(void)
{
    init();
}

/* _791: STARTUP.ASM's _17: the video mode back (_73E), the int 24h vector, the EGA palette,
   and the DOS clock set from the real-time clock; only the first matters to the port. */
void seg019_791(void)
{
    vga_set_mode(D[0x350]);
}

/* _7CD and _809: the joystick reads (JOYPORT.ASM). No UW1 source calls them; with no game
   port every axis times out. */
void seg019_7CD(void)
{
}

void seg019_809(void)
{
}

/* mouse: the motion into MouseDx and MouseDy (FD72:0448, 044A). */
void mouse(void)
{
    int16_t x, y;
    Read_Mouse_Motion_seg019_6E7(&x, &y);
    SETW(0x448, x);
    SETW(0x44A, y);
}

/* mbuttons: the buttons, bits 0 and 1. */
int mbuttons(void)
{
    return seg019_703();
}

/* TICKREAD.ASM's _C00: the low word of the clock. */
unsigned seg019_C00(void)
{
    return (uint16_t)*CLOCK;
}
