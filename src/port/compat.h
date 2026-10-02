/* compat.h: the port's view of Turbo C. Force-included (cc -include src/port/compat.h, with
   -I src/port/include) into every game source when it is compiled for a modern host; never
   seen by the DOS build, which compiles the same sources with Turbo C++ 1.01 and its own
   headers (tools/tcc.mjs stages src/include only, and tools/sources.py skips src/port).
   docs/PORT.md has the design.

   The Turbo C keywords are defined away, the far pointer macros and pseudo-registers become
   calls into the paragraph map and variables of the port's register file (src/port/mem,
   src/port/sys/borland.c), and Borland's library names that the host libc also has, with
   other behaviour, are renamed to bc_, which the port defines (src/port/sys/borland.c). The
   game's main and exit are renamed too: the port's own main runs the game on a thread of its
   own, and exit ends it through the platform layer. Port C that includes a game header
   includes this file first. */
#ifndef UW2_PORT_COMPAT_H
#define UW2_PORT_COMPAT_H

/* The host headers come first, so that the renames below never reach the host's own
   declarations (Darwin's carry asm labels that would bind bc_open to libc's open). The game's
   later #include <stdio.h> and the rest are then no-ops. */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Memory-model keywords. Medium model: code is far, data is near unless said otherwise.
   On the host every pointer is flat, so all of them vanish. */
#define far
#define near
#define huge
#define _far
#define _near
#define _huge
#define interrupt
#define _interrupt
#define cdecl
#define _cdecl
#define pascal
#define _pascal
#define _loadds
#define _saveregs

/* Far pointers. In the port a far pointer is a host pointer; these map segment:offset to one
   and back through the port's paragraph map (docs/PORT.md, "Far pointers and segments"). */
void *port_mk_fp(unsigned seg, unsigned off);
unsigned port_fp_seg(const volatile void *p);
unsigned port_fp_off(const volatile void *p);
#define MK_FP(s, o) port_mk_fp((unsigned)(s), (unsigned)(o))
#define FP_SEG(p) port_fp_seg((const volatile void *)(p))
#define FP_OFF(p) port_fp_off((const volatile void *)(p))

/* Turbo C's pseudo-registers (EMS.C, UWEDIT.C): lvalues in a register file that the port's
   interrupt emulation reads and writes. Little-endian, so the byte halves overlay the words. */
union port_reg16 { uint16_t x; struct { uint8_t l, h; } b; };
extern union port_reg16 port_ax, port_bx, port_cx, port_dx;
extern uint16_t port_si, port_di, port_bp, port_sp, port_cs, port_ds, port_es, port_ss, port_flags;
#define _AX port_ax.x
#define _AL port_ax.b.l
#define _AH port_ax.b.h
#define _BX port_bx.x
#define _BL port_bx.b.l
#define _BH port_bx.b.h
#define _CX port_cx.x
#define _CL port_cx.b.l
#define _CH port_cx.b.h
#define _DX port_dx.x
#define _DL port_dx.b.l
#define _DH port_dx.b.h
#define _SI port_si
#define _DI port_di
#define _BP port_bp
#define _SP port_sp
#define _CS port_cs
#define _DS port_ds
#define _ES port_es
#define _SS port_ss
#define _FLAGS port_flags

/* Borland's open() flags and permission bits, which the sources also write as numbers
   (open(name, 0x8001), mode 0x80): the port's open takes Borland's values and translates. */
#undef O_RDONLY
#undef O_WRONLY
#undef O_RDWR
#undef O_APPEND
#undef O_CREAT
#undef O_TRUNC
#undef O_EXCL
#define O_RDONLY 0x0001
#define O_WRONLY 0x0002
#define O_RDWR   0x0004
#define O_APPEND 0x0800
#define O_CREAT  0x0100
#define O_TRUNC  0x0200
#define O_EXCL   0x0400
#define O_TEXT   0x4000
#define O_BINARY 0x8000
#undef S_IREAD
#undef S_IWRITE
#define S_IREAD  0x0100
#define S_IWRITE 0x0080

/* Borland's C library where the host libc has the same name but not the same behaviour:
   DOS paths and case, Borland's flags and text mode, one-argument mkdir, Borland's rand (its
   own LCG, RAND_MAX 7FFFh), clock at 18.2 Hz, and time and rand under the replay harness's
   control. Function-like, so only calls are renamed. */
int bc_open(const char *path, int access, ...);
int bc_creat(const char *path, int mode);
int bc_read(int fd, void *buf, unsigned n);
int bc_write(int fd, const void *buf, unsigned n);
int bc_close(int fd);
long bc_lseek(int fd, long off, int whence);
int bc_access(const char *path, int mode);
int bc_unlink(const char *path);
int bc_mkdir(const char *path);
int bc_chdir(const char *path);
int bc_stat(const char *path, struct stat *st);
int bc_fstat(int fd, struct stat *st);
FILE *bc_fopen(const char *path, const char *mode);
int bc_rand(void);
void bc_srand(unsigned seed);
long bc_time(long *t);
long bc_clock(void);
#define open(...) bc_open(__VA_ARGS__)
#define creat(p, m) bc_creat(p, m)
#define read(f, b, n) bc_read(f, b, n)
#define write(f, b, n) bc_write(f, b, n)
#define close(f) bc_close(f)
#define lseek(f, o, w) bc_lseek(f, o, w)
#define access(p, m) bc_access(p, m)
#define unlink(p) bc_unlink(p)
#define mkdir(p) bc_mkdir(p)
#define chdir(p) bc_chdir(p)
#define stat(p, b) bc_stat(p, b)
#define fstat(f, b) bc_fstat(f, b)
#define fopen(p, m) bc_fopen(p, m)
/* exit runs the termination chain seg021's init hooked (sys/borland.c), then ends the
   program through the platform layer, which owns the main thread. */
void bc_exit(int status);
#define exit(s) bc_exit(s)
#define rand() bc_rand()
#define srand(s) bc_srand(s)
#define time(t) bc_time((long *)(t))
#define clock() bc_clock()
#undef RAND_MAX
#define RAND_MAX 0x7FFF

/* Borland extensions to the standard headers (stdlib.h, string.h): declared here because a
   port header named stdlib.h would shadow the host's. */
char *itoa(int value, char *buf, int radix);
char *ltoa(long value, char *buf, int radix);
char *ultoa(unsigned long value, char *buf, int radix);
char *strupr(char *s);
char *strlwr(char *s);
int stricmp(const char *a, const char *b);
int strnicmp(const char *a, const char *b, size_t n);
extern char **environ;

/* Borland's <ctype.h> table, which GAMESTRN.C reads directly (_ctype + 1, with its bits). The
   port provides the table with Borland's contents. */
extern unsigned char _ctype[];
#define _IS_SP  1
#define _IS_DIG 2
#define _IS_UPP 4
#define _IS_LOW 8
#define _IS_HEX 16
#define _IS_CTL 32
#define _IS_PUN 64
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

/* The game's main (UWEDIT.C) is not the program's: the port's own main (src/port/sys/main.c)
   sets up the platform and the emulated machine, then runs this one on the game's thread. */
#define main uw2_main
int uw2_main(int argc, char *argv[]);

/* Struct layout. Turbo C lays structs out with byte alignment (no game source is compiled
   with -a), so a 16-bit field can sit at an odd offset and a record takes only the bytes it
   needs; the file records (PLAYER.DAT, LEV.ARK blocks, SCD rows) depend on it. Every struct
   the game declares is therefore packed, as by Turbo C. This comes after every host header
   the game includes (above), so the host's own structs keep their alignment; port C that
   includes a game header must include this file first, so that both see the same layout.
   tools/layoutcheck.py compares the result with Turbo C's, record by record. */
#pragma pack(1)

#endif
