"""UW1's spec for Exhume's tools/asm2c.py ([asm2c] spec in exhume.toml): the assembly modules the
port translates to C instruction by instruction (Exhume's docs/port.md, "The static
recompiler"), the code segments, the routines the port has as hand-written C instead, and the C
for single instructions the generic rules cannot express. UW2's is Exhume's
examples/uw2/port/asm2c_spec.py with UW2Decomp's tools/asm2c.py.
Paths are relative to the UW1Decomp checkout.
"""

# (source, output, C name), in the segments' order: SPRITE (seg000), VALLOC (seg001) and LPFDELTA
# (seg002), which C calls with C arguments (gfx/asmentry.c); seg003, the graphics library: its 18 modules but GRCORE (the C entries, by hand in gfx/grcore.c) and MAPDATA (data only).
MODULES = [
    ('src/gfx/SPRITE.ASM', 'src/port/gfx/sprite.c', 'SPRITE'),
    ('src/gfx/VALLOC.ASM', 'src/port/gfx/valloc.c', 'VALLOC'),
    ('src/gfx/LPFDELTA.ASM', 'src/port/gfx/lpfdelta.c', 'LPFDELTA'),
    ('src/gfx/GRMISC.ASM', 'src/port/gfx/grmisc.c', 'GRMISC'),
    ('src/gfx/WALLMAP.ASM', 'src/port/gfx/wallmap.c', 'WALLMAP'),
    ('src/gfx/POLYFILL.ASM', 'src/port/gfx/polyfill.c', 'POLYFILL'),
    ('src/gfx/QUADFIT.ASM', 'src/port/gfx/quadfit.c', 'QUADFIT'),
    ('src/gfx/GRENTRY.ASM', 'src/port/gfx/grentry.c', 'GRENTRY'),
    ('src/gfx/SCALEBM.ASM', 'src/port/gfx/scalebm.c', 'SCALEBM'),
    ('src/gfx/VIDMODE.ASM', 'src/port/gfx/vidmode.c', 'VIDMODE'),
    ('src/gfx/GRLIBF.ASM', 'src/port/gfx/grlibf.c', 'GRLIBF'),
    ('src/gfx/GRLIBG.ASM', 'src/port/gfx/grlibg.c', 'GRLIBG'),
    ('src/gfx/GRLIBH.ASM', 'src/port/gfx/grlibh.c', 'GRLIBH'),
    ('src/gfx/GRLIBI.ASM', 'src/port/gfx/grlibi.c', 'GRLIBI'),
    ('src/gfx/GRCORE.ASM', 'src/port/gfx/grcore_x.c', 'GRCORE'),
    ('src/gfx/GRJUMPS.ASM', 'src/port/gfx/grjumps.c', 'GRJUMPS'),
    ('src/gfx/GRDISP.ASM', 'src/port/gfx/grdisp.c', 'GRDISP'),
    ('src/gfx/GRLIBL.ASM', 'src/port/gfx/grlibl.c', 'GRLIBL'),
    ('src/gfx/GRLIBM.ASM', 'src/port/gfx/grlibm.c', 'GRLIBM'),
    ('src/gfx/GRLIBN.ASM', 'src/port/gfx/grlibn.c', 'GRLIBN'),
    ('src/3d/EXPAND.ASM', 'src/port/3d/expand.c', 'EXPAND'),
    ('src/3d/SMOOTH.ASM', 'src/port/3d/smooth.c', 'SMOOTH'),
    ('src/3d/CAMERA.ASM', 'src/port/3d/camera.c', 'CAMERA'),
    ('src/3d/SPHERE.ASM', 'src/port/3d/sphere.c', 'SPHERE'),
    ('src/3d/NOCLIP.ASM', 'src/port/3d/noclip.c', 'NOCLIP'),
    ('src/3d/INTERP.ASM', 'src/port/3d/interp.c', 'INTERP'),
    ('src/3d/INSTANCE.ASM', 'src/port/3d/instance.c', 'INSTANCE'),
    ('src/3d/TMAPOPS.ASM', 'src/port/3d/tmapops.c', 'TMAPOPS'),
    ('src/3d/PGCACHE.ASM', 'src/port/3d/pgcache_x.c', 'PGCACHE'),
    ('src/gfx/MODEX.ASM', 'src/port/gfx/modex.c', 'MODEX'),
    ('src/sys/IMATH.ASM', 'src/port/sys/imath.c', 'IMATH'),
    ('src/sys/C3DENTRY.ASM', 'src/port/sys/c3dentry_x.c', 'C3DENTRY'),
]
MODTAB = 'src/port/x86/modtab.c'

# The code segments, by the prefix of the target names: the EXE's paragraph (build/LINK/out/UW.MAP)
# and the C name of the host memory holding its bytes (src/port/asmgame.h).
CODESEGS = {'seg000': (0x0000, 'CODE000'), 'seg001': (0x004E, 'CODE001'), 'seg002': (0x0086, 'CODE002'),
            'seg015_1F9B': (0x1DAE, 'CODE015'),
            'seg003': (0x0090, 'CODE003'), 'seg004': (0x06E7, 'CODE004'), 'seg019': (0x1F3A, 'CODE019')}

# C written by hand for one instruction, (module, address) -> (C, why).
FAR_FROM_C = '{{ unsigned s_, o_; port_fp_split_recent({p}, &s_, &o_); {reg} = (uint16_t)o_; SET_{sreg}(s_); }}'
OVERRIDES = {
    ('MODEX', 0x0025): (FAR_FROM_C.format(p='dseg_5c99_2404', reg='DI', sreg='ES'),
                        "les di,_dseg_5c99_2404: GRCORE's far pointer to the screen's page offset (seg048:3838), "
                        "a C object in the port's DGROUP (gfx/grcore.c)"),
    ('MODEX', 0x0242): (FAR_FROM_C.format(p='palette', reg='SI', sreg='DS'),
                        "lds si,_palette: GRCORE's far pointer to the RGB triples (seg048:5046), a C object "
                        "(gfx/grcore.c)"),
    ('MODEX', 0x0262): (FAR_FROM_C.format(p='dseg_5c99_2404', reg='BX', sreg='ES'),
                        "les bx,_dseg_5c99_2404, as at 0025"),
    ('EXPAND', 0x01A6): (
        'if (CODE004[0x01A6] == 0xC3) { SP = (uint16_t)(SP + 2); return ASM_RET; }\n'
        'AX = sub16(AX, AX, 0);',
        "L01A6: the record decoder writes a ret over the sub here (C3h) and puts it back (29h), "
        "as UW2's (UW2Decomp's tools/asm2c.py)"),
    ('PGCACHE', 0x7A82): (
        'SI = (uint16_t)port_fp_off(Palettes);',
        "do_uwobj: _Palettes is LOADGR.C's array, a C object in the port, not a DGROUP offset (as UW2's)"),
    ('PGCACHE', 0x7A87): (
        'AX = (uint16_t)port_fp_seg(Palettes);',
        "do_uwobj: DGROUP's segment, here the paragraph the port's map gives _Palettes (as UW2's)"),
    ('INSTANCE', 0x50D8): (
        'if (memcmp(CODE004 + 0x50D8, CODE004 + 0x517E, 0x26) == 0) goto L517E;\n'
        'AX = BX;',
        "mxmul's body is whichever check_flat last copied over it (rep movs from L512B, the general "
        "one, 54h bytes, or L517E, the flat one, 26h bytes): the code block holds the copy, so the "
        "body runs as it says (as UW2's)"),
    ('GRENTRY', 0x0BEC): (
        '{ uint16_t p_ = 0x0BEC;\n'
        '  for (;;) {\n'
        '    uint8_t op_ = CODE003[p_];\n'
        '    if (op_ == 0xC3) { SP = (uint16_t)(SP + 2); return ASM_RET; }\n'
        '    if (op_ == 0xA4) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); p_++; }\n'
        '    else if (op_ == 0x83 && CODE003[p_ + 1] == 0xC6) { SI = (uint16_t)(SI + (int8_t)CODE003[p_ + 2]); p_ += 3; }\n'
        '    else port_halt("GRENTRY _BEC: an instruction the copier does not have");\n'
        '  } }',
        "_BEC, the unrolled copier (movsb; add si,3 for 320 pixels): setup_frame_buf (_B0C) writes "
        "a ret (C3h) at _BEC + the frame's width and puts back the movsb (A4h) the last one replaced, "
        "so the copier runs from the code block until its ret (UW2's cPlaceFB set-up does the same)"),
    ('SPRITE', 0x0342): ('AX = port_dgroup_seg();', "mov ax,DGROUP: the C stack's segment, the port's stand-in for DGROUP (x86/entry.c)"),
    ('SPRITE', 0x0445): ('AX = port_dgroup_seg();', "mov ax,DGROUP, as at 0342"),
    ('SPRITE', 0x048A): ('AX = port_dgroup_seg();', "mov ax,DGROUP, as at 0342"),
    ('SPRITE', 0x04CF): ('AX = port_dgroup_seg();', "mov ax,DGROUP, as at 0342"),
    ('SCALEBM', 0x119C): (
        'scalebm_run_generated(0x1F3A);',
        "call near ptr L1F3A: the scaler the generators wrote is interpreted (gfx/scalebm_code.c), as UW2's"),
}
# Instructions whose bytes another instruction writes, handled by OVERRIDES above.
PATCH_OVERRIDDEN = {(0x06E7, 0x01A6)}
# Routines of a translated module that the port has as hand-written C instead, by module: the
# ranges of offsets [lo, hi) not translated, and where the C is (Exhume's docs/port.md, "One
# implementation per routine").
HANDWRITTEN = {
    'C3DENTRY': [
        (0x102B, 0x1071, "x86/glue.c: _102B, do_mouseq's MousQUp(1) on DGROUP's stack (DGROUP is C objects)"),
    ],
}
# The C names the overrides use, declared in their modules' files.
EXTERNS = {
    'SCALEBM': ['void scalebm_run_generated(uint16_t ip);   /* gfx/scalebm_code.c */'],
    'SPRITE': ['uint16_t port_dgroup_seg(void);        /* x86/entry.c */'],
    'PGCACHE': ['extern unsigned char Palettes[];         /* LOADGR.C */'],
    'MODEX': ['extern uint16_t *dseg_5c99_2404;      /* gfx/grcore.c */',
              'extern unsigned char *palette;        /* gfx/grcore.c */'],
}
