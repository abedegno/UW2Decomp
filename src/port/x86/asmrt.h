/* asmrt.h: replaces nothing. The machine the translated assembly modules run on (docs/PORT.md,
   "The 3D renderer"): the 386's registers and flags, the segment registers with the host
   pointer of each segment, the stack in the DOS memory it occupied, the divide fault, and
   calls, jumps and returns between the translated modules, the hand-written C and back.

   tools/asm2c.py writes one C function per assembly module, instruction for instruction from
   the matched .ASM source and the bytes the gate proves it assembles to. The registers are
   global, as the CPU's were, so a value one routine leaves in a register for the next (the
   model interpreter's handlers rely on that) is there in the port too. Memory is reached
   through the segment registers: each segment register keeps the host pointer of its
   segment's offset 0 (asm_segbase), and every access is that pointer plus a 16-bit offset.

   Control flow. A module's function takes the address to start at and runs until the code
   returns (ASM_RET, ASM_RETF, ASM_IRET) or jumps out of the function (ASM_JMP, the target).
   A call pushes the return address on the emulated stack as the CPU did and runs the callee
   through asm_call, which keeps following the jumps the callee makes until a return pops the
   address it pushed; a jump or return made after the callee discarded that address (seg004's
   `add sp,4` out of load_cpg, CLIP.ASM's `mov sp,[C880]` abort, a divide fault's handler)
   unwinds the C calls in between as the stack pointer says, so the stack is the authority, as
   it was in DOS. */
#ifndef UW2_ASMRT_H
#define UW2_ASMRT_H

#include <stdint.h>
#include <string.h>
#include "port.h"

typedef union {
    uint32_t e;
    uint16_t x;
    struct { uint8_t l, h; } b;
} asm_reg;

extern asm_reg asm_ax, asm_bx, asm_cx, asm_dx, asm_si, asm_di, asm_bp, asm_sp;
#define EAX asm_ax.e
#define AX asm_ax.x
#define AL asm_ax.b.l
#define AH asm_ax.b.h
#define EBX asm_bx.e
#define BX asm_bx.x
#define BL asm_bx.b.l
#define BH asm_bx.b.h
#define ECX asm_cx.e
#define CX asm_cx.x
#define CL asm_cx.b.l
#define CH asm_cx.b.h
#define EDX asm_dx.e
#define DX asm_dx.x
#define DL asm_dx.b.l
#define DH asm_dx.b.h
#define ESI asm_si.e
#define SI asm_si.x
#define EDI asm_di.e
#define DI asm_di.x
#define EBP asm_bp.e
#define BP asm_bp.x
#define ESP asm_sp.e
#define SP asm_sp.x

/* The segment registers' values (DOS paragraphs, as the port's paragraph map numbers them) and
   the host pointers of their offset 0. */
extern uint16_t asm_ds, asm_es, asm_ss, asm_fs, asm_gs;
extern uint8_t *pDS, *pES, *pSS, *pFS, *pGS;
uint8_t *asm_segbase(uint16_t seg);
#define SET_DS(v) (asm_ds = (uint16_t)(v), pDS = asm_segbase(asm_ds))
#define SET_ES(v) (asm_es = (uint16_t)(v), pES = asm_segbase(asm_es))
#define SET_SS(v) (asm_ss = (uint16_t)(v), pSS = asm_segbase(asm_ss))
#define SET_FS(v) (asm_fs = (uint16_t)(v), pFS = asm_segbase(asm_fs))
#define SET_GS(v) (asm_gs = (uint16_t)(v), pGS = asm_segbase(asm_gs))

/* The code segments of the EXE the translated modules come from, at their DOS paragraphs (the
   EXE's own plus PORT_LOAD_SEG): seg003 (the graphics library) and seg004 (the renderer) are
   one block of host memory holding their bytes from the user's EXE, as DOS loaded them, so the
   data they keep in their code segments (lightabs, cXfer, the scaler SCALEBM.ASM generates,
   the immediates the renderer patches) is where the code reads it. seg021's code is not
   needed as data. */
#define SEG003_CS (0x0085u + PORT_LOAD_SEG)
#define SEG004_CS (0x065Cu + PORT_LOAD_SEG)
#define SEG021_CS (0x2110u + PORT_LOAD_SEG)
extern unsigned char port_code_block[];        /* seg003's paragraph to seg004's end (mem/fardata.c) */
#define CODE003 (port_code_block)
#define CODE004 (port_code_block + (0x065Cu - 0x0085u) * 16u)

/* The flags the modules use. The parity and auxiliary flags are never tested by them. */
extern uint8_t CF, ZF, SF, OF, DF;

/* Memory: a byte, word or dword at a 16-bit offset of a segment. */
static inline uint8_t rb(const uint8_t *p, uint32_t a) { return p[(uint16_t)a]; }
static inline uint16_t rw(const uint8_t *p, uint32_t a)
{
    const uint8_t *q = p + (uint16_t)a;
    return (uint16_t)(q[0] | q[1] << 8);
}
static inline uint32_t rd(const uint8_t *p, uint32_t a)
{
    const uint8_t *q = p + (uint16_t)a;
    return (uint32_t)q[0] | (uint32_t)q[1] << 8 | (uint32_t)q[2] << 16 | (uint32_t)q[3] << 24;
}
static inline void wb(uint8_t *p, uint32_t a, uint8_t v) { p[(uint16_t)a] = v; }
static inline void ww(uint8_t *p, uint32_t a, uint16_t v)
{
    uint8_t *q = p + (uint16_t)a;
    q[0] = (uint8_t)v;
    q[1] = (uint8_t)(v >> 8);
}
static inline void wd(uint8_t *p, uint32_t a, uint32_t v)
{
    uint8_t *q = p + (uint16_t)a;
    q[0] = (uint8_t)v;
    q[1] = (uint8_t)(v >> 8);
    q[2] = (uint8_t)(v >> 16);
    q[3] = (uint8_t)(v >> 24);
}

/* The stack, in the memory SS:SP points at. */
static inline void push16(uint16_t v) { SP = (uint16_t)(SP - 2); ww(pSS, SP, v); }
static inline uint16_t pop16(void) { uint16_t v = rw(pSS, SP); SP = (uint16_t)(SP + 2); return v; }
static inline void push32(uint32_t v) { SP = (uint16_t)(SP - 4); wd(pSS, SP, v); }
static inline uint32_t pop32(void) { uint32_t v = rd(pSS, SP); SP = (uint16_t)(SP + 4); return v; }
uint16_t asm_flags(void);
void asm_set_flags(uint16_t f);

/* Arithmetic with the flags the CPU sets. The size is in the name; c is the carry in. */
#define ASM_ARITH(N, T, W, MSB)                                                              \
static inline T add##N(T a, T b, unsigned c)                                                  \
{                                                                                            \
    W r = (W)a + (W)b + (W)c;                                                                \
    T t = (T)r;                                                                              \
    CF = (uint8_t)(r >> N & 1);                                                              \
    OF = (uint8_t)((((a ^ t) & (b ^ t)) >> (N - 1)) & 1);                                    \
    ZF = t == 0;                                                                             \
    SF = (uint8_t)(t >> (N - 1) & 1);                                                        \
    return t;                                                                                \
}                                                                                            \
static inline T sub##N(T a, T b, unsigned c)                                                  \
{                                                                                            \
    W r = (W)a - (W)b - (W)c;                                                                \
    T t = (T)r;                                                                              \
    CF = (uint8_t)(r >> N & 1);                                                              \
    OF = (uint8_t)((((a ^ b) & (a ^ t)) >> (N - 1)) & 1);                                    \
    ZF = t == 0;                                                                             \
    SF = (uint8_t)(t >> (N - 1) & 1);                                                        \
    return t;                                                                                \
}                                                                                            \
static inline T logic##N(T t)                                                                 \
{                                                                                            \
    CF = 0;                                                                                  \
    OF = 0;                                                                                  \
    ZF = t == 0;                                                                             \
    SF = (uint8_t)(t >> (N - 1) & 1);                                                        \
    return t;                                                                                \
}                                                                                            \
static inline T inc##N(T a)                                                                   \
{                                                                                            \
    T t = (T)(a + 1);                                                                        \
    OF = t == (T)MSB;                                                                        \
    ZF = t == 0;                                                                             \
    SF = (uint8_t)(t >> (N - 1) & 1);                                                        \
    return t;                                                                                \
}                                                                                            \
static inline T dec##N(T a)                                                                   \
{                                                                                            \
    T t = (T)(a - 1);                                                                        \
    OF = a == (T)MSB;                                                                        \
    ZF = t == 0;                                                                             \
    SF = (uint8_t)(t >> (N - 1) & 1);                                                        \
    return t;                                                                                \
}                                                                                            \
static inline T neg##N(T a) { T t = sub##N(0, a, 0); CF = a != 0; return t; }               \
static inline T shl##N(T a, unsigned n)                                                      \
{                                                                                            \
    T t;                                                                                     \
    n &= 31;                                                                                 \
    if (!n) return a;                                                                        \
    t = n >= N ? 0 : (T)(a << n);                                                            \
    CF = n > N ? 0 : (uint8_t)((W)a >> (N - n) & 1);                                         \
    OF = (uint8_t)(((t >> (N - 1)) & 1) ^ CF);                                               \
    ZF = t == 0;                                                                             \
    SF = (uint8_t)(t >> (N - 1) & 1);                                                        \
    return t;                                                                                \
}                                                                                            \
static inline T shr##N(T a, unsigned n)                                                      \
{                                                                                            \
    T t;                                                                                     \
    n &= 31;                                                                                 \
    if (!n) return a;                                                                        \
    t = n >= N ? 0 : (T)(a >> n);                                                            \
    CF = n > N ? 0 : (uint8_t)((W)a >> (n - 1) & 1);                                         \
    OF = (uint8_t)(a >> (N - 1) & 1);                                                        \
    ZF = t == 0;                                                                             \
    SF = (uint8_t)(t >> (N - 1) & 1);                                                        \
    return t;                                                                                \
}                                                                                            \
static inline T sar##N(T a, unsigned n)                                                      \
{                                                                                            \
    T t;                                                                                     \
    W s = (W)a;                                                                              \
    n &= 31;                                                                                 \
    if (!n) return a;                                                                        \
    if (s >> (N - 1) & 1) s |= ~(W)0 << N;                                                   \
    t = (T)(n >= N ? (s >> (N - 1)) : (s >> n));                                             \
    if (n >= N) { t = (a >> (N - 1)) & 1 ? (T)~(T)0 : 0; CF = (uint8_t)(a >> (N - 1) & 1); } \
    else CF = (uint8_t)(s >> (n - 1) & 1);                                                   \
    OF = 0;                                                                                  \
    ZF = t == 0;                                                                             \
    SF = (uint8_t)(t >> (N - 1) & 1);                                                        \
    return t;                                                                                \
}                                                                                            \
static inline T rol##N(T a, unsigned n)                                                      \
{                                                                                            \
    unsigned k;                                                                              \
    n &= 31;                                                                                 \
    if (!n) return a;                                                                        \
    k = n % N;                                                                               \
    if (k) a = (T)(a << k | a >> (N - k));                                                   \
    CF = (uint8_t)(a & 1);                                                                   \
    OF = (uint8_t)(((a >> (N - 1)) & 1) ^ CF);                                               \
    return a;                                                                                \
}                                                                                            \
static inline T ror##N(T a, unsigned n)                                                      \
{                                                                                            \
    unsigned k;                                                                              \
    n &= 31;                                                                                 \
    if (!n) return a;                                                                        \
    k = n % N;                                                                               \
    if (k) a = (T)(a >> k | a << (N - k));                                                   \
    CF = (uint8_t)(a >> (N - 1) & 1);                                                        \
    OF = (uint8_t)(((a >> (N - 1)) ^ (a >> (N - 2))) & 1);                                   \
    return a;                                                                                \
}                                                                                            \
static inline T rcl##N(T a, unsigned n)                                                      \
{                                                                                            \
    n &= 31;                                                                                 \
    n %= N + 1;                                                                              \
    while (n--) {                                                                            \
        uint8_t c = (uint8_t)(a >> (N - 1) & 1);                                             \
        a = (T)(a << 1 | CF);                                                                \
        CF = c;                                                                              \
        OF = (uint8_t)(((a >> (N - 1)) & 1) ^ CF);                                           \
    }                                                                                        \
    return a;                                                                                \
}                                                                                            \
static inline T rcr##N(T a, unsigned n)                                                      \
{                                                                                            \
    n &= 31;                                                                                 \
    n %= N + 1;                                                                              \
    while (n--) {                                                                            \
        uint8_t c = (uint8_t)(a & 1);                                                        \
        OF = (uint8_t)(((a >> (N - 1)) & 1) ^ CF);                                           \
        a = (T)(a >> 1 | (T)((T)CF << (N - 1)));                                             \
        CF = c;                                                                              \
    }                                                                                        \
    return a;                                                                                \
}

ASM_ARITH(8, uint8_t, uint32_t, 0x80)
ASM_ARITH(16, uint16_t, uint32_t, 0x8000)
ASM_ARITH(32, uint32_t, uint64_t, 0x80000000u)

/* shld and shrd, 16 and 32 bits (count masked to 5 bits; a count of 0 changes nothing). */
static inline uint32_t shld32(uint32_t d, uint32_t s, unsigned n)
{
    uint32_t t;
    n &= 31;
    if (!n) return d;
    t = d << n | s >> (32 - n);
    CF = (uint8_t)(d >> (32 - n) & 1);
    OF = (uint8_t)(((t ^ d) >> 31) & 1);
    ZF = t == 0;
    SF = (uint8_t)(t >> 31);
    return t;
}
static inline uint32_t shrd32(uint32_t d, uint32_t s, unsigned n)
{
    uint32_t t;
    n &= 31;
    if (!n) return d;
    t = d >> n | s << (32 - n);
    CF = (uint8_t)(d >> (n - 1) & 1);
    OF = (uint8_t)(((t ^ d) >> 31) & 1);
    ZF = t == 0;
    SF = (uint8_t)(t >> 31);
    return t;
}
static inline uint16_t shld16(uint16_t d, uint16_t s, unsigned n)
{
    uint32_t w = (uint32_t)d << 16 | s, t;
    n &= 31;
    if (!n) return d;
    if (n > 16) port_halt("shld with a count above 16");
    t = (uint32_t)(w << n >> 16) & 0xFFFF;
    CF = (uint8_t)(d >> (16 - n) & 1);
    OF = (uint8_t)(((t ^ d) >> 15) & 1);
    ZF = t == 0;
    SF = (uint8_t)(t >> 15 & 1);
    return (uint16_t)t;
}
static inline uint16_t shrd16(uint16_t d, uint16_t s, unsigned n)
{
    uint32_t w = (uint32_t)s << 16 | d, t;
    n &= 31;
    if (!n) return d;
    if (n > 16) port_halt("shrd with a count above 16");
    t = (w >> n) & 0xFFFF;
    CF = (uint8_t)(d >> (n - 1) & 1);
    OF = (uint8_t)(((t ^ d) >> 15) & 1);
    ZF = t == 0;
    SF = (uint8_t)(t >> 15 & 1);
    return (uint16_t)t;
}

/* Multiplies: CF and OF set when the high half is significant. */
static inline void mul8(uint8_t s) { AX = (uint16_t)(AL * s); CF = OF = AH != 0; }
static inline void mul16(uint16_t s)
{
    uint32_t r = (uint32_t)AX * s;
    AX = (uint16_t)r;
    DX = (uint16_t)(r >> 16);
    CF = OF = DX != 0;
}
static inline void mul32(uint32_t s)
{
    uint64_t r = (uint64_t)EAX * s;
    EAX = (uint32_t)r;
    EDX = (uint32_t)(r >> 32);
    CF = OF = EDX != 0;
}
static inline void imul8(uint8_t s)
{
    int16_t r = (int16_t)((int8_t)AL * (int8_t)s);
    AX = (uint16_t)r;
    CF = OF = r != (int8_t)r;
}
static inline void imul16(uint16_t s)
{
    int32_t r = (int32_t)(int16_t)AX * (int16_t)s;
    AX = (uint16_t)r;
    DX = (uint16_t)((uint32_t)r >> 16);
    CF = OF = r != (int16_t)r;
}
static inline void imul32(uint32_t s)
{
    int64_t r = (int64_t)(int32_t)EAX * (int32_t)s;
    EAX = (uint32_t)r;
    EDX = (uint32_t)((uint64_t)r >> 32);
    CF = OF = r != (int32_t)r;
}
static inline uint16_t imul16x(uint16_t a, uint16_t b)
{
    int32_t r = (int32_t)(int16_t)a * (int16_t)b;
    CF = OF = r != (int16_t)r;
    return (uint16_t)r;
}
static inline uint32_t imul32x(uint32_t a, uint32_t b)
{
    int64_t r = (int64_t)(int32_t)a * (int32_t)b;
    CF = OF = r != (int32_t)r;
    return (uint32_t)r;
}

/* Divides: 1 when the CPU would raise the divide fault (a zero divisor or a quotient out of
   range), with the registers unchanged; the caller then hands the fault to asm_divfault. */
int asm_div8(uint8_t d);
int asm_idiv8(uint8_t d);
int asm_div16(uint16_t d);
int asm_idiv16(uint16_t d);
int asm_div32(uint32_t d);
int asm_idiv32(uint32_t d);
/* The divide fault at seg:ip, an instruction of len bytes: runs the handler the renderer
   installed at FD71:05A5 (INSTANCE.ASM). 0 to go on after the instruction, or the jump to make
   (the handlers that drop the interrupt frame and clip instead). */
uint32_t asm_divfault(uint16_t seg, uint16_t ip, unsigned len);

/* Control flow. */
/* What a module's function or a glue routine returns: a return (near, far, or iret, with the
   bytes `ret n` drops in bits 8..23) or a jump. asm_call and asm_callf return 0 when the call
   came back to its caller, else the return or jump they pass outward. */
#define ASM_RET 4u
#define ASM_RETF 5u
#define ASM_IRET 6u
#define ASM_RETKIND(c) ((c) & 0xFFu)
#define ASM_RETEXTRA(c) ((c) >> 8 & 0xFFFFu)
#define ASM_JMP(seg, off) (0x80000000u | (uint32_t)(seg) << 16 | (uint16_t)(off))
#define ASM_IS_JMP(c) ((c) & 0x80000000u)
#define ASM_SEG(c) ((uint16_t)((c) >> 16 & 0x7FFF))
#define ASM_OFF(c) ((uint16_t)(c))
typedef uint32_t (*asm_module_fn)(uint16_t entry);
/* A translated module: its code segment (the EXE's paragraph), its range of offsets and its
   function. */
struct asm_module { uint16_t seg, lo, hi; asm_module_fn fn; const char *name; };
/* Hand-written C reached from the translated code at seg:off: it does the routine's work on
   the registers and returns as the routine did (it pops what the call pushed itself). */
typedef uint32_t (*asm_glue_fn)(void);
struct asm_glue { uint16_t seg, off; asm_glue_fn fn; const char *name; };
uint32_t asm_exec(uint32_t target);
uint32_t asm_call(uint32_t target, uint16_t retip);
uint32_t asm_callf(uint32_t target, uint16_t retcs, uint16_t retip);
uint32_t asm_int(uint8_t n);
void asm_bad_entry(const char *module, uint16_t entry) __attribute__((noreturn));
void asm_halt_at(uint16_t seg, uint16_t ip, const char *why) __attribute__((noreturn));
/* Hand C calling translated code: a far call of seg:off with the registers as they are. */
void asm_run_far(uint16_t seg, uint16_t off);
/* The near return of a glue routine (pop the return address) and the far one. */
static inline uint32_t asm_glue_ret(void) { SP = (uint16_t)(SP + 2); return ASM_RET; }
static inline uint32_t asm_glue_retf(void) { SP = (uint16_t)(SP + 4); return ASM_RETF; }

/* Ports. */
uint8_t asm_in8(uint16_t port);
void asm_out8(uint16_t port, uint8_t v);
void asm_out16(uint16_t port, uint16_t v);

/* The string instructions' step. */
#define STEP(n) (DF ? -(n) : (n))

/* The code a module runs to generate and run the scaled sprite rows (SCALEBM.ASM): see
   gfx/scalebm_code.c. */
void scalebm_run_generated(uint16_t ip);

#endif
