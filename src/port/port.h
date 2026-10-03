/* port.h: what the port's own C shares (docs/PORT.md). Not a game header and not the platform
   API (platform/plat.h): the paragraph map, the far data blocks loaded from the user's UW2.EXE,
   the emulated hardware (the VGA, the PIT, the keyboard controller, the mouse driver), and the
   port's diagnostics. Port C that also includes game headers includes compat.h first. */
#ifndef UW2_PORT_H
#define UW2_PORT_H

#include <stddef.h>
#include <stdint.h>

/* A little-endian word store that evaluates its value once: the port's SETW macros, written
   as two byte stores, evaluated the value twice, so `SETW(o, W(o) + n)` saw its own new low
   byte the second time and got the high byte wrong whenever the low byte carried. */
static inline void port_setw(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
}

/* Diagnostics. port_log prints when UW2PORT_TRACE is set in the environment (or -v), and
   port_fatal stops the program with a message. port_halt is where a stub or an unported path
   ends up: the game's thread parks, the window stays, and the message says why. */
extern int port_trace;
void port_log(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void port_fatal(const char *fmt, ...) __attribute__((format(printf, 1, 2), noreturn));
void port_halt(const char *why) __attribute__((noreturn));
/* Called after each grPageFlip from C (a full screen the game has just shown): writes the
   screenshots --shot-at-flip asks for (sys/main.c). */
void port_on_flip(void);

/* The paragraph map (mem/parmap.c, docs/PORT.md "Far pointers and segments"): host memory
   given DOS paragraph numbers, so that MK_FP, FP_SEG, FP_OFF and segment arithmetic work.
   pm_add gives a block the paragraphs from seg; the block's byte 0 is seg:0000. */
void pm_add(const char *name, void *base, size_t size, unsigned seg);
void *port_mk_fp(unsigned seg, unsigned off);
unsigned port_fp_seg(const volatile void *p);
unsigned port_fp_off(const volatile void *p);
/* A far pointer's segment and offset as the DOS code made it, when it was derived from one of
   the last few MK_FP results (a picture's pixels after its header); else FP_SEG and FP_OFF. */
void port_fp_split_recent(const void *p, unsigned *seg, unsigned *off);
void pm_remove(void *base);
void *pm_segbase(unsigned seg);
/* The program's load segment: far data blocks are at their EXE paragraph plus this. */
#define PORT_LOAD_SEG 0x0800u
/* The emulated conventional memory heap that farmalloc hands out (mem/parmap.c). */
#define PORT_HEAP_FIRST 0x7000u
#define PORT_HEAP_END   0xA000u

/* The far data blocks of UW2.EXE that no source defines yet (mem/fardata.c): their initial
   bytes are read from the user's own UW2.EXE at start-up, never shipped. */
extern unsigned char seg_370D[];           /* seg003's data, the graphics library's */
extern unsigned char seg052_519C[];        /* seg004's data, the 3D renderer's */
extern unsigned char dseg062_62a6[];       /* seg021's data (FD71) */
#define SEG_370D_SIZE     0x5E76
#define SEG052_519C_SIZE  0xE4D6
#define DSEG062_62A6_SIZE 0x0C40
int port_load_exe(const char *path);      /* 0, or -1 with a message */
int port_check_exe(const char *path);     /* 0 if it is the GOG UW2.EXE, else < 0, no message */
/* Finding the game and the settings file (sys/gamedir.c). */
int port_find_game(const char *home, char *out, size_t outsz);              /* 0 and the game directory */
int port_game_in(const char *dir, const char *home, char *out, size_t outsz); /* in a folder the user chose */
const char *port_game_refused(void);                                         /* a UW2.EXE found and refused */
int port_config_get(const char *home, const char *key, char *out, size_t outsz);
int port_config_set(const char *home, const char *key, const char *value);
/* The EXE's DGROUP image (its initialised data, DS:0 on), for the C library's tables and the
   null-pointer copies. */
extern unsigned char port_dgroup_image[0x10000];

/* The emulated VGA (gfx/vga.c). */
void vga_outb(unsigned port, uint8_t v);
uint8_t vga_inb(unsigned port);
void vga_outw(unsigned port, uint16_t v);
void vga_write(uint16_t off, uint8_t v);       /* a CPU write to A000:off */
uint8_t vga_read(uint16_t off);                /* a CPU read of A000:off (loads the latches) */
void vga_set_mode(int mode);                   /* int 10h, AH = 0 */
void vga_scanout(uint8_t *pixels, int *w, int *h, uint8_t rgb6[768]);
void vga_get_dac(uint8_t rgb6[768]);
const uint8_t *vga_plane(int p);               /* for the state dump */
void vga_window_init(void);                    /* A000:0000 in the paragraph map */
int vga_in_window(const volatile void *p);     /* p points into it */
void port_vga_store(volatile void *p, unsigned char v);
uint8_t vga_reg_crtc(int i);

/* Ports other than the VGA's (sys/borland.c dispatches outportb and inportb). */
void port_outb(unsigned port, uint8_t v);
uint8_t port_inb(unsigned port);

/* The PIT (sys/pit.c): the port's timer thread gives AIL's timers (sound/ail.c) the time
   that passes, and runs the BIOS tick at 18.2 Hz and the keyboard's typematic repeat. */
void pit_start(void);
uint32_t pit_bios_ticks(void);                 /* 18.2 Hz ticks since start, as 0040:006C */

/* The keyboard controller (sys/kbdint.c): a byte from the platform, as from port 60h. */
void kbd_byte(uint8_t scancode);
void kbd_tick_ms(uint32_t ms);                 /* the typematic repeat, from the PIT thread */

/* The mouse driver (sys/mousedrv.c): int 33h's state, fed by pointer events. */
struct PlatPointer;
void mouse_event(const struct PlatPointer *ev);
int mouse_int33(uint16_t *ax, uint16_t *bx, uint16_t *cx, uint16_t *dx);

/* The divide trap (sys/int0trap.c): the handler the game installed for int 0, called on a
   host SIGFPE (x86-64; arm64's divide does not trap). */
void int0_install(void);

#endif
