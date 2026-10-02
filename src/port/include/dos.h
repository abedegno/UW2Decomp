/* dos.h for the port: Borland's <dos.h> as the game uses it. Declarations only; the port
   provides each name (docs/PORT.md). Shadows nothing: the DOS build never sees src/port. */
#ifndef UW2_PORT_DOS_H
#define UW2_PORT_DOS_H
#include <stdint.h>

/* MK_FP, FP_SEG and FP_OFF are in compat.h, which every port compile includes first. */

struct WORDREGS { unsigned short ax, bx, cx, dx, si, di, cflag, flags; };
struct BYTEREGS { unsigned char al, ah, bl, bh, cl, ch, dl, dh; };
union REGS { struct WORDREGS x; struct BYTEREGS h; };
struct SREGS { unsigned short es, cs, ss, ds; };

void bc_geninterrupt(int intno);
#define geninterrupt(n) bc_geninterrupt(n)
int int86(int intno, union REGS *in, union REGS *out);
int int86x(int intno, union REGS *in, union REGS *out, struct SREGS *s);
int intdos(union REGS *in, union REGS *out);
int intdosx(union REGS *in, union REGS *out, struct SREGS *s);
void segread(struct SREGS *s);

void outportb(unsigned port, unsigned char value);
unsigned char inportb(unsigned port);
void outport(unsigned port, unsigned value);
unsigned inport(unsigned port);

void (*getvect(int intno))();
void setvect(int intno, void (*isr)());
void disable(void);
void enable(void);

void movedata(unsigned srcseg, unsigned srcoff, unsigned dstseg, unsigned dstoff, unsigned n);

void sound(unsigned hz);
void nosound(void);
void delay(unsigned ms);

struct dfree { unsigned df_avail, df_total, df_bsec, df_sclus; };
void getdfree(unsigned char drive, struct dfree *df);

/* Borland's overlay manager (OVERLAY.LIB): the port has no overlays, so this does nothing. */
int _OvrInitEms(unsigned emsHandle, unsigned firstPage, unsigned pages);

extern unsigned _psp;
#endif
