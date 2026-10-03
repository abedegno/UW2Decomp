/* modtab.c: replaces nothing. The translated modules (tools/asm2c.py writes this file and
   them): the code segment, the range of offsets and the function of each. */
#include "x86/asmrt.h"

uint32_t asm_mod_EXPAND(uint16_t entry);
uint32_t asm_mod_SPHERE(uint16_t entry);
uint32_t asm_mod_SMOOTH(uint16_t entry);
uint32_t asm_mod_INTERP(uint16_t entry);
uint32_t asm_mod_INSTANCE(uint16_t entry);
uint32_t asm_mod_TMAPOPS(uint16_t entry);
uint32_t asm_mod_CLIP(uint16_t entry);
uint32_t asm_mod_PERTLP(uint16_t entry);
uint32_t asm_mod_PERTP(uint16_t entry);
uint32_t asm_mod_PROJPOLY(uint16_t entry);
uint32_t asm_mod_SCANLINE(uint16_t entry);
uint32_t asm_mod_TEXMAP(uint16_t entry);
uint32_t asm_mod_TEXMAPV(uint16_t entry);
uint32_t asm_mod_PGCACHE(uint16_t entry);
uint32_t asm_mod_GRMISC(uint16_t entry);
uint32_t asm_mod_GRENTRY(uint16_t entry);
uint32_t asm_mod_SCALEBM(uint16_t entry);
uint32_t asm_mod_VIDMODE(uint16_t entry);
uint32_t asm_mod_GRLIBF(uint16_t entry);
uint32_t asm_mod_GRLIBG(uint16_t entry);
uint32_t asm_mod_GRLIBH(uint16_t entry);
uint32_t asm_mod_GRLIBI(uint16_t entry);
uint32_t asm_mod_GRDISP(uint16_t entry);
uint32_t asm_mod_GRLIBM(uint16_t entry);
uint32_t asm_mod_GRLIBN(uint16_t entry);
uint32_t asm_mod_IMATH(uint16_t entry);

const struct asm_module asm_modules[] = {
    { 0x065C, 0x0020, 0x0256, asm_mod_EXPAND, "EXPAND.ASM" },
    { 0x065C, 0x0260, 0x05FB, asm_mod_SPHERE, "SPHERE.ASM" },
    { 0x065C, 0x0600, 0x0DA9, asm_mod_SMOOTH, "SMOOTH.ASM" },
    { 0x065C, 0x0DB0, 0x2FFB, asm_mod_INTERP, "INTERP.ASM" },
    { 0x065C, 0x3000, 0x3CEC, asm_mod_INSTANCE, "INSTANCE.ASM" },
    { 0x065C, 0x3CF0, 0x40A2, asm_mod_TMAPOPS, "TMAPOPS.ASM" },
    { 0x065C, 0x40B0, 0x46EE, asm_mod_CLIP, "CLIP.ASM" },
    { 0x065C, 0x46F0, 0x4B2C, asm_mod_PERTLP, "PERTLP.ASM" },
    { 0x065C, 0x4B30, 0x4E04, asm_mod_PERTP, "PERTP.ASM" },
    { 0x065C, 0x4E10, 0x51A2, asm_mod_PROJPOLY, "PROJPOLY.ASM" },
    { 0x065C, 0x51B0, 0x54B8, asm_mod_SCANLINE, "SCANLINE.ASM" },
    { 0x065C, 0x54C0, 0x6236, asm_mod_TEXMAP, "TEXMAP.ASM" },
    { 0x065C, 0x6240, 0x6D1B, asm_mod_TEXMAPV, "TEXMAPV.ASM" },
    { 0x065C, 0x6D3C, 0x8154, asm_mod_PGCACHE, "PGCACHE.ASM" },
    { 0x0085, 0x0000, 0x0062, asm_mod_GRMISC, "GRMISC.ASM" },
    { 0x0085, 0x06E2, 0x0D1E, asm_mod_GRENTRY, "GRENTRY.ASM" },
    { 0x0085, 0x0D1E, 0x1550, asm_mod_SCALEBM, "SCALEBM.ASM" },
    { 0x0085, 0x21D4, 0x3205, asm_mod_VIDMODE, "VIDMODE.ASM" },
    { 0x0085, 0x3206, 0x3685, asm_mod_GRLIBF, "GRLIBF.ASM" },
    { 0x0085, 0x3686, 0x375B, asm_mod_GRLIBG, "GRLIBG.ASM" },
    { 0x0085, 0x375C, 0x38B3, asm_mod_GRLIBH, "GRLIBH.ASM" },
    { 0x0085, 0x38B4, 0x43EF, asm_mod_GRLIBI, "GRLIBI.ASM" },
    { 0x0085, 0x52EE, 0x536F, asm_mod_GRDISP, "GRDISP.ASM" },
    { 0x0085, 0x5934, 0x5B4F, asm_mod_GRLIBM, "GRLIBM.ASM" },
    { 0x0085, 0x5B50, 0x5D6D, asm_mod_GRLIBN, "GRLIBN.ASM" },
    { 0x2110, 0x0A30, 0x0BF9, asm_mod_IMATH, "IMATH.ASM" },
};
const int asm_nmodules = 26;
