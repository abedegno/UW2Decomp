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

/* The sound hardware's side of the replay (docs/PORT.md, "Sound"). With a sound card the game
   reads state that time and the card change behind its back: whether the music has ended,
   whether a digital buffer has played out, which MIDI channel is free to lock, which timbres
   the driver's cache still holds. SND_READ(drv, x) is such a read of driver drv: in DOS the
   value x; under REPLAY recorded or replayed like an input (x is still evaluated, so the
   driver sees the same calls), unless drv is -1, no driver, whose answer is always 0; and
   the moment of the read within the clock tick is recorded with it (rp_sound_at, before x),
   since a digital buffer ends at any moment of a tick, so that the port's driver is asked
   at the moment DOS's was. SLAVE_TIMER(f) is a game callback that AIL ran from the timer interrupt (SOUND.C's
   16 Hz effects timer): in DOS f itself, so AIL calls it at any instruction; under REPLAY and
   in the port, REPLAY.C runs it instead, at reads of the game clock, as often as AIL's DDA
   would have by then, so that its effect on the game's state lands at the same point in two
   runs. Both are the original tokens under Turbo C. */
#if defined(__TURBOC__) && !defined(REPLAY)
#define SND_READ(drv, x) (x)
#define SLAVE_TIMER(f)  f
#else
typedef void (far *RpTimerFn)(void);
void far rp_sound_at(int drv);
unsigned far rp_sound(int drv, unsigned v);
RpTimerFn far rp_slave_timer(RpTimerFn f, unsigned hz);
#define SND_READ(drv, x) (rp_sound_at(drv), rp_sound(drv, x))
#define SLAVE_TIMER(f)  rp_slave_timer(f, 16)
#endif

/* FRAME_LEN(dos, host) and FRAME_TAIL(arr, i, var): a local array that the original copies
   past its end on purpose, into the stack slots Turbo C laid out after it, and a local that
   is in fact the array's element i (SCROLL.C's scroll_print copies 49 bytes into a 47-byte
   array and uses the 49th as its terminator, `sentinel`). Under Turbo C the original
   tokens: the array has its DOS length and the variable is itself, so the bytes are the
   same. On the host the frame is the compiler's, so the array is given the length the code
   uses and the variable becomes that element of it. */
#ifdef __TURBOC__
#define FRAME_LEN(dos, host) dos
#define FRAME_TAIL(arr, i, var) var
#else
#define FRAME_LEN(dos, host) host
#define FRAME_TAIL(arr, i, var) ((arr)[i])
#endif

/* FRAME_INDEX(arr, i, below): element i of a local array, where i can be -1 and the original
   then reads the stack slot Turbo C laid out below the array; below is what DOS finds there
   (CRITTIME.C's where_shall_we_hang_out indexes its castle tables by whoami - 0x82 for
   whoami 0x81 too: xs[-1] is ys[13], and ys[-1] the high byte of the SI it saved, which is
   gronk_whoami's arg). Under Turbo C the original tokens; on the host, where the frame is the
   compiler's, below for a negative index. */
#ifdef __TURBOC__
#define FRAME_INDEX(arr, i, below) arr[i]
#else
#define FRAME_INDEX(arr, i, below) ((i) < 0 ? (below) : (arr)[i])
#endif

/* READ_PAIR(fd, a, b): read(fd, &a, 4), where the original reads two words into a one-word
   local and relies on Turbo C having put local b right after it on the stack (BABL.C reads
   a bglobals.dat record's slot and size so). The original tokens under Turbo C; on the host
   the two words go to a and b, whatever the compiler's frame, and the count read is the
   value. */
#ifdef __TURBOC__
#define READ_PAIR(fd, a, b) read(fd, &(a), 4)
#else
#define READ_PAIR(fd, a, b) ({ uint16_t rp_w_[2] = { 0, 0 }; int rp_r_ = read(fd, rp_w_, 4); \
    if (rp_r_ >= 2) (a) = rp_w_[0]; if (rp_r_ >= 4) (b) = rp_w_[1]; rp_r_; })
#endif

/* WRITE_PAIR(fd, a, b): write(fd, &a, 4), the same two locals written as one record. */
#ifdef __TURBOC__
#define WRITE_PAIR(fd, a, b) write(fd, &(a), 4)
#else
#define WRITE_PAIR(fd, a, b) ({ uint16_t wp_w_[2]; wp_w_[0] = (uint16_t)(a); wp_w_[1] = (uint16_t)(b); \
    write(fd, wp_w_, 4); })
#endif

/* WRITABLE_STR(s): a string literal the code writes into (SOUND.C builds the driver's and the
   music's file names in place). The literal under Turbo C; on the host a literal is
   read-only, so it is an array of the same characters, which the code may change. */
#ifdef __TURBOC__
#define WRITABLE_STR(s) s
#else
#define WRITABLE_STR(s) ((char[]){ s })
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

/* DOS_SIZEOF(T, n) and HOST_TABLE(p, n): a table of n pointers the original allocates from a
   heap of its own (BABL.C's funcs, n far function pointers from bab_malloc). DOS_SIZEOF is
   sizeof(T) under Turbo C and n, DOS's size, on the host, so that the heap gives out the same
   blocks at the same places as in DOS; HOST_TABLE is the heap's block p under Turbo C and on
   the host a table of n host pointers in host memory (one per call site, kept from one call to
   the next), since the host's pointers do not fit DOS's block (and with a full heap, where DOS
   would write the table over the vector table, the port still has it). Both the original
   tokens under Turbo C. */
#ifdef __TURBOC__
#define DOS_SIZEOF(T, n) sizeof(T)
#define HOST_TABLE(p, n) p
#else
#define DOS_SIZEOF(T, n) (n)
#define HOST_TABLE(p, n) __extension__ ({ static void *port_table_; __typeof__(p) port_dos_ = (p); \
    port_table_ = realloc(port_table_, (size_t)(n) * sizeof(*port_dos_) + 1); \
    (void)port_dos_; (__typeof__(p))port_table_; })
#endif

/* TAG_SLOT(b, i) and TAG_VAL(p): BABL.C's heap tags each block with a far pointer in its last
   four bytes, slot i of the block b taken as an array of far pointers, and checks the tag
   when it frees the block. Under Turbo C the original tokens. On the host a pointer is eight
   bytes, so slot i of a pointer array lies twice as far in, past the block's end and into
   the blocks after it; the host keeps the tag in DOS's four bytes instead, as the low 32 bits
   of the pointer (TAG_VAL), so that the heap's blocks hold what they hold in DOS. */
#ifdef __TURBOC__
#define TAG_SLOT(b, i) ((char far * far *)b)[i]
#define TAG_VAL(p) p
#else
#define TAG_SLOT(b, i) (*(uint32_t *)((char *)(b) + (i) * 4))
#define TAG_VAL(p) ((uint32_t)(uintptr_t)(p))
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

/* RENDER_TAG(o): DRAWOBJ.C is about to write the sprite opcode (do_uwobj or do_uwcrit) that
   draws object o, at dbptr. The port records which object it is, so that its per-object
   sprite hook (src/port/3d/render.h) can name the object the renderer draws; nothing in DOS,
   so the DOS bytes are the same. Written as a statement, `RENDER_TAG(o);`, which is an empty
   statement under Turbo C. */
#ifdef __TURBOC__
#define RENDER_TAG(o)
#else
void port_render_tag(const void *object, const void *db);
#define RENDER_TAG(o) port_render_tag((const void *)(o), (const void *)dbptr)
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
