/* asmrt.c: replaces nothing. The machine the translated assembly modules run on: see asmrt.h.
   The module table and the glue table are in x86/modules.c. */
#include <stdio.h>
#include <stdlib.h>
#include "asmrt.h"

asm_reg asm_ax, asm_bx, asm_cx, asm_dx, asm_si, asm_di, asm_bp, asm_sp;
uint16_t asm_ds, asm_es, asm_ss, asm_fs, asm_gs;
uint8_t *pDS, *pES, *pSS, *pFS, *pGS;
uint8_t CF, ZF, SF, OF, DF;

extern const struct asm_module asm_modules[];
extern const int asm_nmodules;
extern const struct asm_glue asm_glues[];
extern const int asm_nglues;
uint32_t asm_seg003_fallback(uint16_t off);       /* x86/glue.c */

/* A segment no region holds: what DOS would read there (upper memory, ROM) is not known, so
   the port gives zeros and says so once per segment. Writes land in this page too. */
static uint8_t nowhere[0x10010];

uint8_t *asm_segbase(uint16_t seg)
{
    uint8_t *p;
    if (seg >= SEG003_CS && seg < SEG004_CS + 0x1000u)
        return port_code_block + (uint32_t)(seg - SEG003_CS) * 16u;
    p = pm_segbase(seg);
    if (p) return p;
    {
        static uint16_t said[16];
        static int nsaid;
        int i;
        for (i = 0; i < nsaid; i++) if (said[i] == seg) break;
        if (i == nsaid && nsaid < 16) {
            said[nsaid++] = seg;
            port_log("asm: segment %04X is in no region; it reads as zeros\n", seg);
        }
    }
    return nowhere;
}

uint16_t asm_flags(void)
{
    return (uint16_t)(0x7002 | CF | ZF << 6 | SF << 7 | 1u << 9 | DF << 10 | OF << 11);
}

void asm_set_flags(uint16_t f)
{
    CF = f & 1;
    ZF = f >> 6 & 1;
    SF = f >> 7 & 1;
    DF = f >> 10 & 1;
    OF = f >> 11 & 1;
}

int asm_div8(uint8_t d)
{
    uint16_t q;
    if (!d) return 1;
    q = (uint16_t)(AX / d);
    if (q > 0xFF) return 1;
    AH = (uint8_t)(AX % d);
    AL = (uint8_t)q;
    return 0;
}

int asm_idiv8(uint8_t d)
{
    int32_t n = (int16_t)AX, q;
    if (!d) return 1;
    q = n / (int8_t)d;
    if (q > 127 || q < -128) return 1;
    AH = (uint8_t)(n % (int8_t)d);
    AL = (uint8_t)q;
    return 0;
}

int asm_div16(uint16_t d)
{
    uint32_t n = (uint32_t)DX << 16 | AX, q;
    if (!d) return 1;
    q = n / d;
    if (q > 0xFFFF) return 1;
    DX = (uint16_t)(n % d);
    AX = (uint16_t)q;
    return 0;
}

int asm_idiv16(uint16_t d)
{
    int32_t n = (int32_t)((uint32_t)DX << 16 | AX);
    int64_t q;
    if (!d) return 1;
    q = (int64_t)n / (int16_t)d;
    if (q > 32767 || q < -32768) return 1;
    DX = (uint16_t)(int16_t)((int64_t)n % (int16_t)d);
    AX = (uint16_t)q;
    return 0;
}

int asm_div32(uint32_t d)
{
    uint64_t n = (uint64_t)EDX << 32 | EAX, q;
    if (!d) return 1;
    q = n / d;
    if (q > 0xFFFFFFFFu) return 1;
    EDX = (uint32_t)(n % d);
    EAX = (uint32_t)q;
    return 0;
}

int asm_idiv32(uint32_t d)
{
    int64_t n = (int64_t)((uint64_t)EDX << 32 | EAX), q;
    if (!d) return 1;
    if (n == INT64_MIN && (int32_t)d == -1) return 1;
    q = n / (int32_t)d;
    if (q > 2147483647LL || q < -2147483648LL) return 1;
    EDX = (uint32_t)(int32_t)(n % (int32_t)d);
    EAX = (uint32_t)q;
    return 0;
}

void asm_halt_at(uint16_t seg, uint16_t ip, const char *why)
{
    char msg[200];
    snprintf(msg, sizeof msg, "%04X:%04X: %s", seg, ip, why);
    port_halt(msg);
}

void asm_bad_entry(const char *module, uint16_t entry)
{
    char msg[160];
    snprintf(msg, sizeof msg, "the translated %s has no entry at %04X", module, entry);
    port_halt(msg);
}

/* The int 0 handlers of seg004 (INSTANCE.ASM, SMOOTH.ASM, INTERP.ASM, PROJPOLY.ASM), by the
   offset the renderer keeps at FD71:05A5. Each skips the faulting instruction by a fixed
   length; a fault in an instruction of another length would resume in its middle in DOS, so
   the port stops there instead. */
#define D71 ((uint8_t *)dseg062_62a6)

uint32_t asm_divfault(uint16_t seg, uint16_t ip, unsigned len)
{
    uint16_t h = rw(D71, 0x5A5);
    switch (h) {
    case 0x3C91:                       /* overflow_handler_reg: AX = +-7FFFh by DX's sign */
        if (len != 2) break;
        AX = (int16_t)DX < 0 ? 0x8001 : 0x7FFF;
        DX = (int16_t)AX < 0 ? 0xFFFF : 0;
        ww(CODE004, 0x3C8F, (uint16_t)(rw(CODE004, 0x3C8F) + 1));   /* overflow_count */
        return 0;
    case 0x3CB5:                       /* overflow_handler_mem */
        if (len != 4) break;
        AX = 0x7FFF;
        DX = 0;
        return 0;
    case 0x3CCB:                       /* overflow_handler_special_bp: by DX xor BP's sign */
        if (len != 2) break;
        AX = (int16_t)(DX ^ BP) < 0 ? 0x8001 : 0x7FFF;
        DX = 0;
        return 0;
    case 0x0D83:                       /* SMOOTH.ASM: halve the dividend and divide by CX again */
        if (len != 2) break;
        for (;;) {
            uint32_t n = (uint32_t)DX << 16 | AX;
            n = (uint32_t)((int32_t)n >> 1);
            DX = (uint16_t)(n >> 16);
            AX = (uint16_t)n;
            if (DX == CX) { AX = 0x7FFF; DX = 0; return 0; }
            if (!asm_idiv16(CX)) return 0;
        }
    case 0x4EFF:                       /* divide_overflow_handler (PROJPOLY.ASM): flag the polygon */
        wb(CODE004, 0x4EFE, 1);        /* _divide_overflow_has_occurred */
        EAX = 0x30000000u;
        return 0;
    case 0x267D:                       /* clip_overflow (INTERP.ASM) */
    case 0x0798:                       /* SMOOTH.ASM's recovery */
    case 0x11AB: case 0x11AD:          /* do_lnres's */
        /* the CPU pushes FLAGS, CS and IP; the handler drops them (sti; add sp,6) */
        push16(asm_flags());
        push16(seg + PORT_LOAD_SEG);
        push16(ip);
        return ASM_JMP(0x065C, h);
    default:
        break;
    }
    {
        char why[120];
        snprintf(why, sizeof why, "divide fault with handler %04X at FD71:05A5 (instruction of %u bytes)", h, len);
        asm_halt_at(seg, ip, why);
    }
}

static const struct asm_glue *find_glue(uint16_t seg, uint16_t off)
{
    int i;
    for (i = 0; i < asm_nglues; i++)
        if (asm_glues[i].seg == seg && asm_glues[i].off == off) return &asm_glues[i];
    return NULL;
}

static const struct asm_module *find_module(uint16_t seg, uint16_t off)
{
    int i;
    for (i = 0; i < asm_nmodules; i++)
        if (asm_modules[i].seg == seg && off >= asm_modules[i].lo && off < asm_modules[i].hi) return &asm_modules[i];
    return NULL;
}

int asm_trace = -1;

uint32_t asm_exec(uint32_t target)
{
    uint16_t seg = ASM_SEG(target), off = ASM_OFF(target);
    if (asm_trace < 0) asm_trace = getenv("UW2PORT_ASMTRACE") != NULL;
    if (asm_trace)
        fprintf(stderr, "asm %04X:%04X ax=%04X bx=%04X cx=%04X dx=%04X si=%04X di=%04X bp=%04X sp=%04X ds=%04X es=%04X ss=%04X\n",
                seg, off, AX, BX, CX, DX, SI, DI, BP, SP, asm_ds, asm_es, asm_ss);
    const struct asm_glue *g = find_glue(seg, off);
    const struct asm_module *m;
    if (g) return g->fn();
    m = find_module(seg, off);
    if (m) return m->fn(off);
    if (seg == 0x0085) return asm_seg003_fallback(off);
    asm_halt_at(seg, off, "no translated code and no glue here");
}

/* Follow the jumps from target until a return pops the address this call pushed (SP back to
   s plus the address's size), or until the stack says this call's frame is gone. */
static uint32_t run(uint32_t target, uint16_t s, unsigned size)
{
    uint32_t c = target;
    for (;;) {
        int16_t d;
        c = asm_exec(c);
        d = (int16_t)(uint16_t)(SP - s);
        if (!ASM_IS_JMP(c)) {
            int want = (int)size + (int)ASM_RETEXTRA(c);
            if (d == want && (ASM_RETKIND(c) == ASM_RET) == (size == 2)) return 0;
            if (d > want) return c;                     /* a return of an outer call */
            {
                char why[120];
                snprintf(why, sizeof why, "a return (kind %u) with SP %d bytes from the call's %d",
                         (unsigned)ASM_RETKIND(c), (int)d, want);
                port_halt(why);
            }
        }
        if (d > 0) return c;                           /* this frame was dropped: the caller's jump */
    }
}

uint32_t asm_call(uint32_t target, uint16_t retip)
{
    push16(retip);
    return run(target, SP, 2);
}

uint32_t asm_callf(uint32_t target, uint16_t retcs, uint16_t retip)
{
    push16(retcs);
    push16(retip);
    return run(target, SP, 4);
}

void asm_run_far(uint16_t seg, uint16_t off)
{
    uint32_t c = asm_callf(ASM_JMP(seg, off), 0xFFFF, 0xFFFF);
    if (c) port_halt("translated code returned past the C that called it");
}

uint8_t asm_in8(uint16_t port)
{
    return port_inb(port);
}

void asm_out8(uint16_t port, uint8_t v)
{
    port_outb(port, v);
}

void asm_out16(uint16_t port, uint16_t v)
{
    port_outb(port, (uint8_t)v);
    port_outb((uint16_t)(port + 1), (uint8_t)(v >> 8));
}
