/* asmgame.h: UW2's side of the machine the translated modules run on (the 3D renderer seg004,
   the graphics library seg003, IMATH in seg021), Exhume's runtime/port/x86/asmrt.h, which
   includes this file (docs/PORT.md, "The runtime").

   The code segments the translated modules come from, at their DOS paragraphs (the EXE's own
   plus PORT_LOAD_SEG): seg003 (the graphics library) and seg004 (the renderer) are one block of
   host memory holding their bytes from the user's EXE, as DOS loaded them (mem/fardata.c), so
   the data they keep in their code segments (lightabs, cXfer, the scaler SCALEBM.ASM generates,
   the immediates the renderer patches) is where the code reads it. seg021's code is not needed
   as data. CODE003 and CODE004 are the names tools/asm2c.py's spec (CODESEGS) gives. */
#define SEG003_CS (0x0085u + PORT_LOAD_SEG)
#define SEG004_CS (0x065Cu + PORT_LOAD_SEG)
#define SEG021_CS (0x2110u + PORT_LOAD_SEG)
extern unsigned char port_code_block[];        /* seg003's paragraph to seg004's end (mem/fardata.c) */
#define CODE003 (port_code_block)
#define CODE004 (port_code_block + (0x065Cu - 0x0085u) * 16u)
#define ASM_CODE_BASE(seg) ((seg) >= SEG003_CS && (seg) < SEG004_CS + 0x1000u \
                            ? port_code_block + (uint32_t)((seg) - SEG003_CS) * 16u : (uint8_t *)0)

/* seg003's routines that are hand-written C are reached through seg003_call's own dispatch
   (gfx/grcore.c), not glue entries: the span writers with their list at SI. */
uint32_t asm_seg003_fallback(uint16_t off);       /* x86/glue.c */
int seg003_handles(uint16_t off);                 /* gfx/grcore.c */
#define ASM_HAND_HANDLED(seg, off) ((seg) == 0x0085 && seg003_handles(off))
#define ASM_HAS_FALLBACK(seg) ((seg) == 0x0085)
#define ASM_FALLBACK(seg, off) asm_seg003_fallback(off)

#define ASM_TRACE_ENV "UW2PORT_ASMTRACE"

/* The code a module runs to generate and run the scaled sprite rows (SCALEBM.ASM): see
   gfx/scalebm_code.c. */
void scalebm_run_generated(uint16_t ip);
