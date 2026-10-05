/* asmgame.h: UW1's side of the machine the translated modules run on (the graphics library
   seg003, the sprite, video memory and picture modules seg000 to seg002, MODEX.ASM, and later the
   3D renderer seg004), Exhume's runtime/port/x86/asmrt.h, which includes this file (Exhume's
   docs/port.md, "The static recompiler").

   The code segments, at their DOS paragraphs (the EXE's own, build/LINK/out/UW.MAP, plus
   PORT_LOAD_SEG), are one block of host memory holding the program's code from the user's EXE
   as DOS loaded it, from seg000 to seg019's 64 KB window (mem/fardata.c), so the data the
   modules keep in their code segments (cXfer, lightabs, the immediates the code patches) is
   where the code reads it. CODEnnn are the names tools/asm2c_spec.py's CODESEGS gives. */
#define SEG003_CS (0x0090u + PORT_LOAD_SEG)
#define SEG004_CS (0x06E7u + PORT_LOAD_SEG)
#define SEG019_CS (0x1F3Au + PORT_LOAD_SEG)
#define CODE_PARAS 0x2F3Au                      /* the block's length in paragraphs */
extern unsigned char port_code_block[];        /* mem/fardata.c */
#define CODE000 (port_code_block)
#define CODE001 (port_code_block + 0x004Eu * 16u)
#define CODE002 (port_code_block + 0x0086u * 16u)
#define CODE003 (port_code_block + 0x0090u * 16u)
#define CODE004 (port_code_block + 0x06E7u * 16u)
#define CODE015 (port_code_block + 0x1DAEu * 16u)
#define CODE019 (port_code_block + 0x1F3Au * 16u)
#define ASM_CODE_BASE(seg) ((seg) >= PORT_LOAD_SEG && (seg) < PORT_LOAD_SEG + CODE_PARAS \
                            ? port_code_block + (uint32_t)((seg) - PORT_LOAD_SEG) * 16u : (uint8_t *)0)

#define ASM_TRACE_ENV "UW1PORT_ASMTRACE"
