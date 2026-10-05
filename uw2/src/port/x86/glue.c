/* glue.c: replaces nothing. Where the translated modules (x86/asmrt.h) reach code that is
   hand-written C in the port: seg021's entries the renderer far-calls, seg003's routines the
   graphics library's near jump table leads to, the routines of translated modules that the port
   has as hand-written C instead (tools/asm2c.py's HANDWRITTEN, docs/PORT.md, "One implementation
   per routine"), the span writers (through seg003_call), and the DOS and EMS interrupts. Each
   does the routine's work on the emulated registers and returns as the routine did, popping
   what the call pushed. */
#include <stdio.h>
#include "x86/asmrt.h"
#include "gfx/grlib.h"

void rectangle(int x, int y, int w, int h);       /* gfx/grcore.c: clip_rect and urectangle */

unsigned seg021_22FD_C00(void);                 /* sys/sysentry.c: TICKREAD.ASM */
void MousQUp(int);                               /* ui/MOUSE.C */
char MapMemory_seg013_1D3C_C7(char physical, unsigned logical);   /* mem/ems.c */
int bc_open(const char *path, int access, ...);
int bc_read(int fd, void *buf, unsigned n);
int bc_close(int fd);

/* seg021's _C00 (TICKREAD.ASM): AX = the game clock's low word (seg021_22FD_710). */
static uint32_t g_c00(void)
{
    AX = (uint16_t)seg021_22FD_C00();
    return asm_glue_retf();
}

/* seg021's _C10 (SYSLIBP.ASM), render_3d's hook: a bare retf. */
static uint32_t g_c10(void)
{
    return asm_glue_retf();
}

/* seg021's _102B (C3DENTRY.ASM), do_mouseq's: MousQUp(1) on a DGROUP stack, SI, DS and ES kept.
   The registers the C leaves are not modelled: the next opcode's dispatch overwrites AX and BX,
   and no handler is known to read another before setting it. */
static uint32_t g_102b(void)
{
    uint16_t si = SI, ds = asm_ds, es = asm_es;
    MousQUp(1);
    SI = si;
    SET_DS(ds);
    SET_ES(es);
    return asm_glue_retf();
}

/* GRCORE.ASM's near jump table, entries that lead to routines ported by hand */
static uint32_t g_52b7(void) { seg003_0272_3195(AX); return asm_glue_ret(); }        /* set_the_color */
static uint32_t g_52a8(void) { seg003_0272_30AC(AX, BX); return asm_glue_ret(); }    /* upixel */
static uint32_t g_52b1(void) { return ASM_JMP(0x0085, 0x3164); }                  /* jmp solid_uvline */
static uint32_t g_52b4(void) { return ASM_JMP(0x0085, 0x3121); }                  /* jmp copy_uvline */
static uint32_t g_5299(void) { seg003_0272_2AC1(); return asm_glue_ret(); }          /* grSoftPageFlip */

/* the routines of translated modules that are hand-written C (tools/asm2c.py's HANDWRITTEN),
   where the translated code calls or jumps to them, with the registers each takes; the span
   writers among them go through seg003_call (asm_seg003_fallback) instead */
static uint32_t g_2296(void)                    /* L2296: a bitmap's row records (_222B) */
{
    seg003_l2296((int16_t)AX, (int16_t)BX, DI, SI, (int16_t)CX, (int16_t)DX);
    return asm_glue_ret();
}
static uint32_t g_2373(void)                    /* L2373: a copy's row records (_2242) */
{
    seg003_l2373((int16_t)AX, (int16_t)BX, (int16_t)CX, (int16_t)DX, DI);
    return asm_glue_ret();
}
static uint32_t g_30fb(void)                    /* _30FB: read a pixel (from _30E3) */
{
    uint16_t v = seg003_0272_30FB(AX, BX);
    AX = (uint16_t)(v << 8 | v);                /* mov ah,es:[di]; ...; mov al,ah */
    DX = GC_INDEX;
    return asm_glue_ret();
}
static uint32_t g_326d(void) { seg003_0272_326D(); return asm_glue_ret(); }        /* the whole screen */
static uint32_t g_3324(void) { seg003_0272_3324((int16_t)AX, (int16_t)BX, (int16_t)DX); return asm_glue_ret(); }  /* uvline */
static uint32_t g_34ae(void) { seg003_0272_34AE((int16_t)AX, (int16_t)BX, (int16_t)CX); return asm_glue_ret(); }  /* uhline (and its body, L34B3) */
static uint32_t g_3416(void)                    /* _3416: rectangle from the four words at SI */
{
    int16_t ax = (int16_t)rw(pDS, SI), bx = (int16_t)rw(pDS, SI + 2), cx = (int16_t)rw(pDS, SI + 4),
            dx = (int16_t)rw(pDS, SI + 6);
    SI = (uint16_t)(SI + 8);
    rectangle(ax, bx, cx, dx);
    return asm_glue_ret();
}
static uint32_t g_3c61(void) { seg003_text_blit(AX); return asm_glue_ret(); }      /* the single-colour blitter */
uint32_t glue_expand_4str(void);                /* 3d/expand.c */
uint32_t glue_expand_4run(void);
uint32_t glue_expand_63(void);
uint32_t glue_expand_18d(void);
uint32_t glue_imath_a30(void);                  /* sys/imath.c */
uint32_t glue_imath_a34(void);

const struct asm_glue asm_glues[] = {
    { 0x2110, 0x0C00, g_c00, "TICKREAD _C00" },
    { 0x2110, 0x0C10, g_c10, "SYSLIBP _C10" },
    { 0x2110, 0x102B, g_102b, "C3DENTRY _102B (MousQUp)" },
    { 0x0085, 0x52B7, g_52b7, "GRCORE _52B7" },
    { 0x0085, 0x52A8, g_52a8, "GRCORE _52A8" },
    { 0x0085, 0x52B1, g_52b1, "GRCORE _52B1" },
    { 0x0085, 0x52B4, g_52b4, "GRCORE _52B4" },
    { 0x0085, 0x5299, g_5299, "GRCORE _5299" },
    { 0x0085, 0x2296, g_2296, "VIDMODE L2296" },
    { 0x0085, 0x2373, g_2373, "VIDMODE L2373" },
    { 0x0085, 0x30FB, g_30fb, "VIDMODE _30FB" },
    { 0x0085, 0x326D, g_326d, "GRLIBF _326D" },
    { 0x0085, 0x3324, g_3324, "GRLIBF _3324" },
    { 0x0085, 0x3416, g_3416, "GRLIBF _3416" },
    { 0x0085, 0x34AE, g_34ae, "GRLIBF _34AE" },
    { 0x0085, 0x34B3, g_34ae, "GRLIBF L34B3" },
    { 0x0085, 0x3C61, g_3c61, "GRLIBI _3C61" },
    { 0x065C, 0x0039, glue_expand_4str, "EXPAND exp_4str" },
    { 0x065C, 0x0063, glue_expand_63, "EXPAND _63" },
    { 0x065C, 0x00D9, glue_expand_4run, "EXPAND exp_4run" },
    { 0x065C, 0x018D, glue_expand_18d, "EXPAND _18D" },
    { 0x2110, 0x0A30, glue_imath_a30, "IMATH _A30 (lsqrt)" },
    { 0x2110, 0x0A34, glue_imath_a34, "IMATH _A34 (sincos)" },
};
const int asm_nglues = sizeof asm_glues / sizeof asm_glues[0];

/* Any other seg003 address: a span writer or transfer the port has in C (seg003_call, which
   stops at an offset it has no C for), entered with SI at its list. */
uint32_t asm_seg003_fallback(uint16_t off)
{
    seg003_call(off, SI);
    return asm_glue_ret();
}

/* int 21h (PGCACHE.ASM's load_cpg: open, read and close a critter page file) and int 67h
   (the EMS page mapping of do_fetchmap, seg004_0849_7EDE and lcpg_pghere). */
uint32_t asm_int(uint8_t n)
{
    char why[80];
    if (n == 0x21) {
        switch (AH) {
        case 0x3D: {
            const char *path = (const char *)(pDS + DX);
            int fd = bc_open(path, 0x8001);     /* Borland's O_RDONLY | O_BINARY */
            if (fd < 0) { AX = 2; CF = 1; } else { AX = (uint16_t)fd; CF = 0; }
            return 0;
        }
        case 0x3F: {
            int r = bc_read(BX, pDS + DX, CX);
            if (r < 0) { AX = 5; CF = 1; } else { AX = (uint16_t)r; CF = 0; }
            return 0;
        }
        case 0x3E:
            CF = bc_close(BX) < 0;
            return 0;
        default:
            break;
        }
    } else if (n == 0x67) {
        switch (AH) {
        case 0x44:
            AH = MapMemory_seg013_1D3C_C7((char)AL, BX) ? 0 : 0x8A;
            return 0;
        case 0x50: {
            uint16_t i;
            if (AL != 0) break;
            for (i = 0; i < CX; i++) {
                uint16_t logical = rw(pDS, SI + 4 * i), physical = rw(pDS, SI + 4 * i + 2);
                if (!MapMemory_seg013_1D3C_C7((char)physical, logical)) { AH = 0x8A; return 0; }
            }
            AH = 0;
            return 0;
        }
        default:
            break;
        }
    }
    snprintf(why, sizeof why, "int %02Xh function %02Xh is not emulated", n, AH);
    port_halt(why);
}
