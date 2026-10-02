/* portable.h: the types and macros that let the same sources build for DOS and for the port
   (docs/PORT.md). Under Turbo C (__TURBOC__) every name here maps to the original tokens, so
   the DOS build compiles to the same bytes and the gate proves it; on a modern host (the port,
   with src/port/compat.h force-included) they take the host's meaning. Included by uw2.h, so
   every source that includes a shared header sees it. */
#ifndef PORTABLE_H
#define PORTABLE_H

/* OLDSTYLE((params)): the parameter list of a function whose callers push an int where the
   definition takes a char (docs/CONTRIBUTING.md, "Where files disagreed"). Turbo C sees an
   old-style declaration, f(), so the callers' pushes keep their bytes; the port sees the
   real prototype, so the call converts the argument as the DOS callee reads it (the low
   byte of the pushed word). */
#ifdef __TURBOC__
#define OLDSTYLE(params) ()
#else
#define OLDSTYLE(params) params
#endif

/* Explicit widths. Turbo C's int is 16 bits and its long 32; a modern host's int is 32 bits
   and its long 64. Declarations whose width matters (file records, structs laid over
   buffers, globals, and the locals the promotion audit lists) use these names, which are
   the original types under Turbo C and the exact-width types on the host, so a stored value
   wraps as it did in DOS. Plain int stays where any width of at least 16 bits gives the
   same result (docs/PORT.md, "Integer widths and wrap", has the policy). */
#ifdef __TURBOC__
typedef int int16;
typedef unsigned uint16;
typedef long int32;
typedef unsigned long uint32;
#else
#include <stdint.h>
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
#endif

/* NEARPTR: a near pointer carried in an integer, a parameter or a field that holds either a
   number or the address of near data (gronk_critid's row, an input handler's argument).
   An int under Turbo C, where a near pointer is 16 bits; pointer-sized on the host, so the
   address survives the round trip. UNEARPTR is the unsigned form, for comparing two near
   pointers as Turbo C does (by offset, unsigned). */
#ifdef __TURBOC__
typedef int NEARPTR;
typedef unsigned UNEARPTR;
#else
typedef intptr_t NEARPTR;
typedef uintptr_t UNEARPTR;
#endif

/* NULLTRAP(p) and FARNULLTRAP(p): the pointer p, at a dereference that the null-pointer
   audit found can see a null pointer in the original game (docs/PORT.md, "Null pointers").
   NULLTRAP is for a near pointer (in DOS a null one reads DS:0), FARNULLTRAP for a far one
   (in DOS a null one reads the interrupt vector table at 0000:0000).

   - The DOS build: the original tokens, (p), so the bytes are the same; the gate proves it.
   - A modding build of chosen sources compiled with -DNULLTRAP (tools/tcc.mjs, then
     tools/link.py --obj STEM=PATH): each hit appends the file and line to NULLTRAP.LOG and
     the code goes on with the null pointer, as DOS does.
   - The port: a null p becomes a pointer to the port's copy of the bytes DOS would read
     (port_null_near: DGROUP from DS:0; port_null_far: the interrupt vector table), and the
     hit is logged, so the faithful port gives DOS's result and never crashes. */
#ifdef __TURBOC__
#ifdef NULLTRAP
#undef NULLTRAP
#include <io.h>
#include <fcntl.h>
#include <string.h>
static void nulltrap_hit(char *file, int line)
{
    char buf[48];
    int fd = open("NULLTRAP.LOG", O_WRONLY | O_CREAT | O_APPEND | O_TEXT, 0x180);
    if (fd < 0) return;
    write(fd, file, strlen(file));
    itoa(line, buf, 10);
    write(fd, ":", 1); write(fd, buf, strlen(buf)); write(fd, "\n", 1);
    close(fd);
}
#define NULLTRAP(p)     ((p) ? (p) : (nulltrap_hit(__FILE__, __LINE__), (p)))
#define FARNULLTRAP(p)  ((p) ? (p) : (nulltrap_hit(__FILE__, __LINE__), (p)))
#else
#define NULLTRAP(p)     (p)
#define FARNULLTRAP(p)  (p)
#endif
#else
void *port_null_near(const char *file, int line);
void *port_null_far(const char *file, int line);
#define NULLTRAP(p)     ((p) ? (p) : (__typeof__(p))port_null_near(__FILE__, __LINE__))
#define FARNULLTRAP(p)  ((p) ? (p) : (__typeof__(p))port_null_far(__FILE__, __LINE__))
#endif

/* FAR_COPY(dst, src, n): copy n bytes between far buffers, written in DOS as
   movedata(FP_SEG(src), FP_OFF(src), FP_SEG(dst), FP_OFF(dst), n), which is what the macro
   expands to under Turbo C, token for token (FP_SEG and FP_OFF come from <dos.h>, which every
   user includes). The port copies with flat pointers, forwards a byte at a time as
   movedata does, with no paragraph map lookup. */
#ifdef __TURBOC__
#define FAR_COPY(dst, src, n) movedata(FP_SEG(src), FP_OFF(src), FP_SEG(dst), FP_OFF(dst), n)
#else
void port_far_copy(void *dst, const void *src, unsigned n);
#define FAR_COPY(dst, src, n) port_far_copy((void *)(dst), (const void *)(src), (unsigned)(n))
#endif

/* AX_RESULT(v), at the end of a function the original wrote with no return statement, whose
   callers use what it left in AX: v is that value. Nothing under Turbo C (an empty statement,
   no code); `return v;` on the host, where falling off the end returns nothing defined. */
#ifdef __TURBOC__
#define AX_RESULT(v)
#else
#define AX_RESULT(v) return (v)
#endif

/* AX_LAST(e): the last statement of such a function when it is an expression (a call) whose
   value the callers find in AX: the statement as it was, (e);, under Turbo C, and `return (e);`
   on the host. */
#ifdef __TURBOC__
#define AX_LAST(e) (e)
#else
#define AX_LAST(e) return (e)
#endif

/* The record and replay hooks (docs/PORT.md, "The differential test"): the few places where
   the game reads the world outside the program. GAME_TIME() is a read of the 1/256 s game
   clock *Time, KEY() the next key event with the key states it leaves (key_on, Shift, Ctrl,
   Alt, CapsLock), MOUSE() the mouse motion into *MouseDx and *MouseDy, MBUTTONS() the
   buttons, JOY_READ() and JOY_BUTTONS() the joystick into joy_position and joy_buttons,
   WALL_TIME(p) the time of day (its callers all pass a null p), SRAND(s) a seed given to
   srand, and CHECKPOINT(n) a named point where both builds dump the game state.

   - The DOS build: the original tokens (CHECKPOINT is nothing), so the bytes are the same;
     the gate proves it.
   - The replay DOS build, the modding build with the sources that use these compiled with
     -DREPLAY and src/replay/REPLAY.C linked in (tools/replay.py): calls into REPLAY.C, which
     records each value the game reads to a file, or replays the values from one.
   - The port: the same calls, into the same REPLAY.C; with neither --record nor --replay
     they read the world as the DOS build does. */
#if defined(__TURBOC__) && !defined(REPLAY)
#define GAME_TIME()     (*Time)
#define KEY()           key()
#define MOUSE()         mouse()
#define MBUTTONS()      mbuttons()
#define JOY_READ()      seg021_22FD_7CD()
#define JOY_BUTTONS()   seg021_22FD_809()
#define WALL_TIME(p)    time(p)
#define SRAND(s)        srand(s)
#define CHECKPOINT(n)
#else
uint32 far rp_time(void);
int far rp_key(void);
void far rp_mouse(void);
int far rp_mbuttons(void);
void far rp_joy(void);
void far rp_joyb(void);
int32 far rp_walltime(void);
void far rp_srand(unsigned seed);
void far rp_checkpoint(int n);
#define GAME_TIME()     rp_time()
#define KEY()           rp_key()
#define MOUSE()         rp_mouse()
#define MBUTTONS()      rp_mbuttons()
#define JOY_READ()      rp_joy()
#define JOY_BUTTONS()   rp_joyb()
#define WALL_TIME(p)    rp_walltime()
#define SRAND(s)        rp_srand(s)
#define CHECKPOINT(n)   rp_checkpoint(n)
#endif

/* PLANAR_STORE(p, v): store the byte v through p, a far pointer into the VGA's window at
   A000:0000, where the sequencer's map mask (set with outportb beforehand) chooses the planes
   the byte goes to. The original store under Turbo C; on the host a pointer cannot apply the
   map mask, so the store goes to the emulated VGA (port_vga_store, src/port/gfx/vga.c), which
   does what the card does with a CPU write. CUTS.C's delta decoder writes the screen this way. */
#ifdef __TURBOC__
#define PLANAR_STORE(p, v) (*(p) = (v))
#else
void port_vga_store(volatile void *p, unsigned char v);
#define PLANAR_STORE(p, v) port_vga_store((p), (v))
#endif

/* FILE_RECORDS(T, p, n, layout) and FILE_RECORDS_END(q, n): n records of struct T that the
   original lays straight over file data at p (CHARGEN.C's DATA\chrgen.dat), and what follows
   them in the file. Under Turbo C the original tokens, (T far *)(p) and (q + n). On the host
   such a struct holds pointers, so it has the host's layout (HOST_LAYOUT_BEGIN), not the
   file's: the records are copied into host structs field by field (src/port/sys/records.c),
   by layout, one letter a field in order: w a 16-bit word, n a near pointer (2 bytes in the
   file), f a far pointer (4 bytes); the pointers start null, as the code sets them before it
   reads them. FILE_RECORDS_END then gives the file's bytes after the n records. */
#ifdef __TURBOC__
#define FILE_RECORDS(T, p, n, layout) ((T far *)(p))
#define FILE_RECORDS_END(q, n) ((q) + (n))
#else
void *port_file_records(void *p, int n, const char *layout, unsigned host_size);
void *port_file_records_end(const void *q);
#define FILE_RECORDS(T, p, n, layout) ((T *)port_file_records((p), (n), (layout), sizeof(T)))
#define FILE_RECORDS_END(q, n) port_file_records_end(q)
#endif

/* STACK_JUNK(v): the initialiser of a local the original reads before it ever sets it. In DOS
   the read gets whatever the stack held there, which depends on the calls before and on the
   timer interrupts that push onto the game's stack, so two DOS runs of one recording can
   differ (docs/FINDINGS.md lists the sites). Nothing under Turbo C, so the DOS bytes are the
   same; `= v` in the replay build and the port, which then both start from v, the value with
   which DOS takes the path it takes when the stack holds zero. */
#if defined(__TURBOC__) && !defined(REPLAY)
#define STACK_JUNK(v)
#else
#define STACK_JUNK(v) = (v)
#endif

/* HOST_LAYOUT_BEGIN and HOST_LAYOUT_END, around a struct that holds pointers. The port packs
   every struct as Turbo C does (src/port/compat.h), so that file records and the structs laid
   over buffers keep their DOS layout; a struct with pointer fields cannot keep it, since a
   host pointer is 8 bytes, and it only ever lives in memory (tools/layoutcheck.py checks
   that none is copied as bytes). Between these two the port lays structs out naturally, so
   their pointers are aligned. Nothing at all under Turbo C. */
#ifdef __TURBOC__
#define HOST_LAYOUT_BEGIN
#define HOST_LAYOUT_END
#else
#define HOST_LAYOUT_BEGIN _Pragma("pack(push, 8)")
#define HOST_LAYOUT_END _Pragma("pack(pop)")
#endif

#endif
