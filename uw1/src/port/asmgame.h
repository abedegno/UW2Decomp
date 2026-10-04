/* asmgame.h: UW1's side of the machine the translated modules run on (the graphics library
   seg003 and the 3D renderer seg004), Exhume's runtime/port/x86/asmrt.h, which includes this
   file (Exhume's docs/port.md, "The static recompiler").

   The code segments the translated modules come from, at their DOS paragraphs (the EXE's own,
   build/LINK/out/UW.MAP, plus PORT_LOAD_SEG): seg003 (SEG003_TEXT, 0090:0000) and seg004
   (SEG004_TEXT, 06E7:0000) are one block of host memory holding their bytes from the user's
   EXE, as DOS loaded them (mem/fardata.c), so the data they keep in their code segments
   (lightabs, cXfer, the immediates the code patches) is where the code reads it. */
#define SEG003_CS (0x0090u + PORT_LOAD_SEG)
#define SEG004_CS (0x06E7u + PORT_LOAD_SEG)
#define SEG019_CS (0x1F3Au + PORT_LOAD_SEG)
extern unsigned char port_code_block[];        /* seg003's paragraph to seg004's 64 KB end (mem/fardata.c) */
#define CODE003 (port_code_block)
#define CODE004 (port_code_block + (0x06E7u - 0x0090u) * 16u)
#define ASM_CODE_BASE(seg) ((seg) >= SEG003_CS && (seg) < SEG004_CS + 0x1000u \
                            ? port_code_block + (uint32_t)((seg) - SEG003_CS) * 16u : (uint8_t *)0)

#define ASM_TRACE_ENV "UW1PORT_ASMTRACE"
