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
    { 0x065C, 0x0020, 0x018D, asm_mod_EXPAND, "EXPAND.ASM" },
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
    { 0x0085, 0x222B, 0x31E7, asm_mod_VIDMODE, "VIDMODE.ASM" },
    { 0x0085, 0x321B, 0x3685, asm_mod_GRLIBF, "GRLIBF.ASM" },
    { 0x0085, 0x3686, 0x375B, asm_mod_GRLIBG, "GRLIBG.ASM" },
    { 0x0085, 0x375C, 0x38B3, asm_mod_GRLIBH, "GRLIBH.ASM" },
    { 0x0085, 0x38B4, 0x4225, asm_mod_GRLIBI, "GRLIBI.ASM" },
    { 0x0085, 0x52EE, 0x536F, asm_mod_GRDISP, "GRDISP.ASM" },
    { 0x0085, 0x5934, 0x5B4F, asm_mod_GRLIBM, "GRLIBM.ASM" },
    { 0x0085, 0x5B50, 0x5D6D, asm_mod_GRLIBN, "GRLIBN.ASM" },
    { 0x2110, 0x0B78, 0x0BF9, asm_mod_IMATH, "IMATH.ASM" },
};
const int asm_nmodules = 26;

/* the ranges of the modules that are hand-written C (asm2c.py's HANDWRITTEN) */
const struct asm_hand asm_hand_ranges[] = {
    { 0x065C, 0x0039, 0x0063, "3d/expand.c: exp_4str" },
    { 0x065C, 0x0063, 0x00CB, "3d/expand.c: build_pal (_63)" },
    { 0x065C, 0x00D9, 0x011C, "3d/expand.c: exp_4run" },
    { 0x065C, 0x018D, 0x0260, "3d/expand.c: run (_18D) and decode (_194)" },
    { 0x0085, 0x0729, 0x0764, "gfx/grentry.c: fbuf_draw_ylrpp_x (_729)" },
    { 0x0085, 0x07A8, 0x09C9, "gfx/grentry.c: setup_frame_buf (_7A8), cFBtoScreen (_7F9) and its copier (_888)" },
    { 0x0085, 0x09FC, 0x0A7F, "gfx/grentry.c: fbuf_save_vylr (_9FC)" },
    { 0x0085, 0x0B9B, 0x0BC0, "gfx/grentry.c: fbuf_setcolor (_B9B)" },
    { 0x0085, 0x21D4, 0x222B, "gfx/vidmode.c: show (_21D4), _21ED, fbshow (_2214)" },
    { 0x0085, 0x2250, 0x249C, "gfx/vidmode.c: vcopy (_2250) and the row routines L2296, L2373, L243F" },
    { 0x0085, 0x283D, 0x2914, "gfx/vidmode.c: init_graphics (_283D), the mode (_286C)" },
    { 0x0085, 0x295F, 0x2B62, "gfx/vidmode.c: the display start, the virtual screen, line compare, page flips, Ytab, edge masks" },
    { 0x0085, 0x2B9B, 0x2C9A, "gfx/vidmode.c: SetVideoMode (_2B9B), the window guards (_2BF9, _2C44)" },
    { 0x0085, 0x2D83, 0x2E2E, "gfx/vidmode.c: the solid span writer (_2D83), the group save (_2DF3)" },
    { 0x0085, 0x2F18, 0x2FD1, "gfx/vidmode.c: the copy span writers (_2F18, _2F96)" },
    { 0x0085, 0x3094, 0x30CF, "gfx/vidmode.c: plot (_3094, _30AC)" },
    { 0x0085, 0x30FB, 0x3121, "gfx/vidmode.c: read a pixel (_30FB)" },
    { 0x0085, 0x3195, 0x31E5, "gfx/vidmode.c: set_the_color (_3195), init_colors (_31BE)" },
    { 0x0085, 0x31E7, 0x3206, "gfx/vidmode.c: set_the_window (_31E7)" },
    { 0x0085, 0x3206, 0x321B, "gfx/grcore.c: the video memory bump allocator (_3206, through _49AE)" },
    { 0x0085, 0x326D, 0x3299, "gfx/grlibf.c: the whole screen (_326D), copy_visible_to_hidden (_327D), copy_hidden_to_visible (_328F)" },
    { 0x0085, 0x3324, 0x336E, "gfx/grlibf.c: uvline (_3324)" },
    { 0x0085, 0x336E, 0x3371, "gfx/grcore.c: box's clip (_336E, clip_rect)" },
    { 0x0085, 0x33BE, 0x3423, "gfx/grcore.c: clip_rect (_33BE) and rectangle (_3416, _341E)" },
    { 0x0085, 0x3423, 0x347E, "gfx/grlibf.c: clear_window (_3423), urectangle (_342E)" },
    { 0x0085, 0x34AE, 0x34C2, "gfx/grlibf.c: uhline (_34AE)" },
    { 0x0085, 0x3B36, 0x3B3E, "gfx/grlibi.c: string_to_screen (_3B36)" },
    { 0x0085, 0x3BE2, 0x3C2F, "gfx/grlibi.c: the text masks (_3BE2), setup_font (_3BFD)" },
    { 0x0085, 0x3C61, 0x4126, "gfx/grlibi.c: the single-colour blitter (_3C61)" },
    { 0x0085, 0x4225, 0x43F0, "gfx/grlibi.c: the rasteriser (4225h), string_width (_43C5)" },
    { 0x0085, 0x5363, 0x536B, "sys/c3dentry.c: cInit3d's view window (_5363)" },
    { 0x2110, 0x0A30, 0x0B78, "sys/imath.c: lsqrt (_A78, far _A30), sincos (_A38, far _A34), fast_sincos (_A69)" },
    { 0, 0, 0, 0 }
};
const int asm_nhand_ranges = 32;
/* the places in them the translated code calls or jumps to: each needs its C (checked at start-up) */
const struct asm_hand_call asm_hand_calls[] = {
    { 0x0085, 0x2296 },
    { 0x0085, 0x2373 },
    { 0x0085, 0x2D83 },
    { 0x0085, 0x2F18 },
    { 0x0085, 0x30FB },
    { 0x0085, 0x326D },
    { 0x0085, 0x3324 },
    { 0x0085, 0x3416 },
    { 0x0085, 0x34AE },
    { 0x0085, 0x34B3 },
    { 0x0085, 0x3C61 },
    { 0x065C, 0x0063 },
    { 0x065C, 0x018D },
    { 0x2110, 0x0A30 },
    { 0x2110, 0x0A34 },
    { 0, 0 }
};
const int asm_nhand_calls = 15;
