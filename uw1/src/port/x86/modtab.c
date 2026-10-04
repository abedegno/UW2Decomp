/* modtab.c: replaces nothing. The translated modules (tools/asm2c.py writes this file and
   them): the code segment, the range of offsets and the function of each. */
#include "x86/asmrt.h"

uint32_t asm_mod_SPRITE(uint16_t entry);
uint32_t asm_mod_VALLOC(uint16_t entry);
uint32_t asm_mod_LPFDELTA(uint16_t entry);
uint32_t asm_mod_GRMISC(uint16_t entry);
uint32_t asm_mod_WALLMAP(uint16_t entry);
uint32_t asm_mod_POLYFILL(uint16_t entry);
uint32_t asm_mod_QUADFIT(uint16_t entry);
uint32_t asm_mod_GRENTRY(uint16_t entry);
uint32_t asm_mod_SCALEBM(uint16_t entry);
uint32_t asm_mod_VIDMODE(uint16_t entry);
uint32_t asm_mod_GRLIBF(uint16_t entry);
uint32_t asm_mod_GRLIBG(uint16_t entry);
uint32_t asm_mod_GRLIBH(uint16_t entry);
uint32_t asm_mod_GRLIBI(uint16_t entry);
uint32_t asm_mod_GRCORE(uint16_t entry);
uint32_t asm_mod_GRJUMPS(uint16_t entry);
uint32_t asm_mod_GRDISP(uint16_t entry);
uint32_t asm_mod_GRLIBL(uint16_t entry);
uint32_t asm_mod_GRLIBM(uint16_t entry);
uint32_t asm_mod_GRLIBN(uint16_t entry);
uint32_t asm_mod_EXPAND(uint16_t entry);
uint32_t asm_mod_SMOOTH(uint16_t entry);
uint32_t asm_mod_CAMERA(uint16_t entry);
uint32_t asm_mod_SPHERE(uint16_t entry);
uint32_t asm_mod_NOCLIP(uint16_t entry);
uint32_t asm_mod_INTERP(uint16_t entry);
uint32_t asm_mod_INSTANCE(uint16_t entry);
uint32_t asm_mod_TMAPOPS(uint16_t entry);
uint32_t asm_mod_PGCACHE(uint16_t entry);
uint32_t asm_mod_MODEX(uint16_t entry);
uint32_t asm_mod_IMATH(uint16_t entry);
uint32_t asm_mod_C3DENTRY(uint16_t entry);

const struct asm_module asm_modules[] = {
    { 0x0000, 0x0002, 0x04EA, asm_mod_SPRITE, "SPRITE.ASM" },
    { 0x004E, 0x000C, 0x02D1, asm_mod_VALLOC, "VALLOC.ASM" },
    { 0x0086, 0x000A, 0x009C, asm_mod_LPFDELTA, "LPFDELTA.ASM" },
    { 0x0090, 0x0000, 0x0051, asm_mod_GRMISC, "GRMISC.ASM" },
    { 0x0090, 0x0060, 0x03A7, asm_mod_WALLMAP, "WALLMAP.ASM" },
    { 0x0090, 0x0422, 0x08E3, asm_mod_POLYFILL, "POLYFILL.ASM" },
    { 0x0090, 0x08E8, 0x0A45, asm_mod_QUADFIT, "QUADFIT.ASM" },
    { 0x0090, 0x0A46, 0x1082, asm_mod_GRENTRY, "GRENTRY.ASM" },
    { 0x0090, 0x1082, 0x1F36, asm_mod_SCALEBM, "SCALEBM.ASM" },
    { 0x0090, 0x2BBA, 0x3A86, asm_mod_VIDMODE, "VIDMODE.ASM" },
    { 0x0090, 0x3A86, 0x3F05, asm_mod_GRLIBF, "GRLIBF.ASM" },
    { 0x0090, 0x3F06, 0x3FDB, asm_mod_GRLIBG, "GRLIBG.ASM" },
    { 0x0090, 0x3FDC, 0x4133, asm_mod_GRLIBH, "GRLIBH.ASM" },
    { 0x0090, 0x4134, 0x4C6F, asm_mod_GRLIBI, "GRLIBI.ASM" },
    { 0x0090, 0x4C74, 0x5A71, asm_mod_GRCORE, "GRCORE.ASM" },
    { 0x0090, 0x5A72, 0x5AE7, asm_mod_GRJUMPS, "GRJUMPS.ASM" },
    { 0x0090, 0x5AE8, 0x5B69, asm_mod_GRDISP, "GRDISP.ASM" },
    { 0x0090, 0x5B6A, 0x6133, asm_mod_GRLIBL, "GRLIBL.ASM" },
    { 0x0090, 0x6134, 0x634F, asm_mod_GRLIBM, "GRLIBM.ASM" },
    { 0x0090, 0x6350, 0x656D, asm_mod_GRLIBN, "GRLIBN.ASM" },
    { 0x06E7, 0x0020, 0x0256, asm_mod_EXPAND, "EXPAND.ASM" },
    { 0x06E7, 0x0260, 0x0A0B, asm_mod_SMOOTH, "SMOOTH.ASM" },
    { 0x06E7, 0x0A10, 0x1366, asm_mod_CAMERA, "CAMERA.ASM" },
    { 0x06E7, 0x1370, 0x1FC6, asm_mod_SPHERE, "SPHERE.ASM" },
    { 0x06E7, 0x1FD0, 0x27CB, asm_mod_NOCLIP, "NOCLIP.ASM" },
    { 0x06E7, 0x27D0, 0x4975, asm_mod_INTERP, "INTERP.ASM" },
    { 0x06E7, 0x4980, 0x5C25, asm_mod_INSTANCE, "INSTANCE.ASM" },
    { 0x06E7, 0x5C30, 0x6944, asm_mod_TMAPOPS, "TMAPOPS.ASM" },
    { 0x06E7, 0x696C, 0x7DDC, asm_mod_PGCACHE, "PGCACHE.ASM" },
    { 0x1DAE, 0x000E, 0x03C2, asm_mod_MODEX, "MODEX.ASM" },
    { 0x1F3A, 0x0A30, 0x0BF9, asm_mod_IMATH, "IMATH.ASM" },
    { 0x1F3A, 0x0C34, 0x102B, asm_mod_C3DENTRY, "C3DENTRY.ASM" },
};
const int asm_nmodules = 32;

/* the ranges of the modules that are hand-written C (asm2c.py's HANDWRITTEN) */
const struct asm_hand asm_hand_ranges[] = {
    { 0x1F3A, 0x102B, 0x1071, "x86/glue.c: _102B, do_mouseq's MousQUp(1) on DGROUP's stack (DGROUP is C objects)" },
    { 0, 0, 0, 0 }
};
const int asm_nhand_ranges = 1;
/* the places in them the translated code calls or jumps to: each needs its C (checked at start-up) */
const struct asm_hand_call asm_hand_calls[] = {
    { 0x1F3A, 0x102B },
    { 0, 0 }
};
const int asm_nhand_calls = 1;
