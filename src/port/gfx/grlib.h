/* grlib.h: what the port's C for seg003, the graphics library, shares (gfx/grcore.c,
   vidmode.c, grlibf.c, grlibl.c, grlibi.c). Not a game header.

   seg003's routines keep their state in their data segment seg_370D (FD51), at fixed offsets,
   and the C written for them keeps it in the same bytes: G is 370D:0000, the base every offset
   in the assembly counts from (the symbol seg_370D itself is 370D:0008), and W, SETW and B
   read and write it as the 8086 did, little-endian, with 16-bit offsets. The tables of near
   code offsets in that data (the span writer in 4112, the pens' writers at 2CEE, the pen
   set-up routines at 2D04, the transfer in 0DC2) keep their DOS values too, and the C
   dispatches on them (seg003_call), so the data stays byte for byte what DOS had and a state
   dump can compare it. An offset with no C yet stops the port with its address. */
#ifndef UW2_PORT_GRLIB_H
#define UW2_PORT_GRLIB_H

#include <stdint.h>
#include "port.h"

#define G (seg_370D - 8)
#define B(o) (G[(uint16_t)(o)])
#define W(o) ((uint16_t)(G[(uint16_t)(o)] | G[(uint16_t)((o) + 1)] << 8))
#define SW(o) ((int16_t)W(o))
#define SETW(o, v) port_setw(&G[(uint16_t)(o)], (uint16_t)(v))
#define SETB(o, v) (G[(uint16_t)(o)] = (uint8_t)(v))

/* The VGA ports seg003 programs. */
#define SC_INDEX 0x3C4
#define SC_DATA 0x3C5
#define GC_INDEX 0x3CE
#define GC_DATA 0x3CF
#define CRTC_INDEX 0x3D4
#define DAC_WRITE_INDEX 0x3C8
#define INPUT_STATUS 0x3DA

/* Calls the routine at near offset off of SEG003_TEXT, with SI as the span or record list
   where it takes one: the hand-written C where the port has it, else the translation. */
void seg003_call(uint16_t off, uint16_t si);
int seg003_handles(uint16_t off);               /* seg003_call has hand-written C for off */

/* C calling a seg003 routine the port has only as the translation (the _x.c files): with the
   registers given (the rest 0), DS and ES seg003's data and, outside translated code, seg003's
   own stack (370D:4FA8), as GRCORE.ASM's entries called them. Returns AX. */
struct seg003_regs { uint16_t ax, bx, cx, dx, si, di, bp; };
uint16_t seg003_asm(uint16_t off, struct seg003_regs r);
uint16_t seg003_asm_far(uint16_t off, struct seg003_regs r);   /* a routine that ends in retf */

/* VIDMODE.ASM */
void seg003_0272_283D(void);                     /* init_graphics */
void seg003_0272_286C(void);
void seg003_0272_2ABE(void);                     /* grPageFlip */
void seg003_0272_2AC1(void);                     /* grSoftPageFlip */
void seg003_0272_2AED(uint16_t ax);
void seg003_0272_295F(void);
void seg003_0272_3195(uint16_t ax);              /* set_the_color */
void seg003_0272_31BE(void);                     /* init_colors */
void seg003_0272_31E7(uint16_t si);              /* set_the_window */
void seg003_0272_2D83(uint16_t si);              /* the solid span writer */
void seg003_0272_21D4(int16_t ax, int16_t bx, uint16_t di, uint16_t si, int16_t cx, int16_t dx);   /* show */
void seg003_0272_21ED(int16_t ax, int16_t bx, uint16_t di, uint16_t si, int16_t cx, int16_t dx);
void seg003_0272_2214(int16_t ax, int16_t bx, uint16_t di, uint16_t si, int16_t cx, int16_t dx);   /* fbshow */
void seg003_0272_2250(int16_t ax, int16_t bx, int16_t cx, int16_t dx, uint16_t si, uint16_t di);   /* vcopy */
void seg003_l2296(int16_t ax, int16_t bx, uint16_t di, uint16_t si, int16_t cx, int16_t dx);       /* the bitmaps' rows */
void seg003_l2373(int16_t ax, int16_t bx, int16_t cx, int16_t dx, uint16_t di);                    /* the copies' rows */
/* GRENTRY.ASM (grentry.c) */
int seg003_fb_span(uint16_t off, uint16_t si);   /* the frame buffer's span writers, by offset */
void seg003_0272_729(uint16_t si);               /* fbuf_draw_ylrpp_x */
void seg003_0272_6E4(uint8_t al);                /* fill_fbuf (the translation's) */
void seg003_0272_764(uint16_t cx);               /* cDimFB (the translation's) */
void seg003_0272_788(uint16_t cx);               /* cLiteFB (the translation's) */
void seg003_0272_7A8(uint16_t bx, uint16_t cx);  /* setup_frame_buf */
void seg003_0272_7F9(void);                      /* cFBtoScreen */
void seg003_0272_B9B(uint16_t ax);               /* fbuf_setcolor */
void seg003_span(uint16_t off, uint16_t si);      /* the other span writers, by offset */
void seg003_0272_30AC(uint16_t ax, uint16_t bx); /* upixel */
void seg003_0272_3094(int16_t ax, int16_t bx);   /* plot */
uint16_t seg003_0272_30FB(uint16_t ax, uint16_t bx); /* read a pixel */
void seg003_0272_3121(uint16_t ax, uint16_t bx, uint16_t dx);   /* copy_uvline (the translation's) */
void seg003_0272_3164(uint16_t ax, uint16_t bx, uint16_t dx);   /* solid_uvline (the translation's) */
void seg003_0272_2977(uint16_t ax, uint16_t bx, uint16_t cx);   /* the virtual screen */
void seg003_0272_2A0D(uint16_t ax, uint16_t bx);                /* vscreen_focus */
/* GRLIBF.ASM */
void seg003_0272_326D(void);
void seg003_0272_327D(void);                     /* copy_visible_to_hidden */
void seg003_0272_328F(void);                     /* copy_hidden_to_visible */
void seg003_0272_3324(int16_t ax, int16_t bx, int16_t dx);   /* uvline */
void seg003_0272_34AE(int16_t ax, int16_t bx, int16_t cx);   /* uhline */
void seg003_0272_3423(void);                     /* clear_window */
void seg003_0272_342E(int16_t ax, int16_t bx, int16_t cx, int16_t dx);   /* urectangle */
/* GRLIBL.ASM */
void seg003_0272_5372(uint16_t si);              /* opaque linear bitmap rows to the screen */
void seg003_0272_5467(uint16_t si);              /* the same, transparent */
void seg003_0272_5578(uint16_t si);
void seg003_0272_568B(uint16_t si);
void seg003_0272_56E3(uint16_t si);
void seg003_0272_581B(uint16_t si);
void seg003_0272_586D(uint16_t si);
void seg003_0272_58C7(uint16_t si);
void seg003_0272_58F8(uint16_t si);
/* GRLIBI.ASM */
void seg003_0272_3BE2(void);                     /* the text masks */
void seg003_0272_3BFD(void);                     /* setup_font */
void seg003_0272_3B36(uint16_t ax, uint16_t bx, uint16_t si);   /* string_to_screen */
uint16_t seg003_0272_43C5(uint16_t si);          /* string_width */
void seg003_text_raster(uint16_t si);            /* 4225h, the rasteriser */
void seg003_text_blit(uint16_t colour);          /* _3C61, the single-colour blitter */

#endif
