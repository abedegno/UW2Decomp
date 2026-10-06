/* tmapops.c: replaces src/3d/TMAPOPS.ASM (seg004_0849_3CF0, 3CF0..40A2 of its
   segment). Written by tools/asm2c.py from the assembly and the bytes it assembles to; do not
   edit by hand: change the tool, or its OVERRIDES, and run it again. Each instruction is the
   C below its source line; the comments before a routine are the .ASM file's. */
#include "x86/asmrt.h"

uint32_t port_sprite_draw(void);        /* 3d/render.c */
#include "portgame.h"
#include "sys/enhance.h"     /* ENHANCED */
static int asm_jcc(uint8_t op)
{
    switch (op & 0x0F) {
    case 0x0: return OF; case 0x1: return !OF; case 0x2: return CF; case 0x3: return !CF;
    case 0x4: return ZF; case 0x5: return !ZF; case 0x6: return CF || ZF; case 0x7: return !CF && !ZF;
    case 0x8: return SF; case 0x9: return !SF; case 0xC: return SF != OF; case 0xD: return SF == OF;
    case 0xE: return ZF || SF != OF; case 0xF: return !ZF && SF == OF;
    default: port_halt("a patched jump on the parity flag");
    }
}

uint32_t asm_mod_TMAPOPS(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x3CF0: goto L3CF0;
    case 0x3CF1: goto L3CF1;
    case 0x3CF3: goto L3CF3;
    case 0x3CF5: goto L3CF5;
    case 0x3CF7: goto L3CF7;
    case 0x3CF9: goto L3CF9;
    case 0x3CFC: goto L3CFC;
    case 0x3CFF: goto L3CFF;
    case 0x3D00: goto L3D00;
    case 0x3D01: goto L3D01;
    case 0x3D05: goto L3D05;
    case 0x3D06: goto L3D06;
    case 0x3D08: goto L3D08;
    case 0x3D0A: goto L3D0A;
    case 0x3D10: goto L3D10;
    case 0x3D12: goto L3D12;
    case 0x3D18: goto L3D18;
    case 0x3D19: goto L3D19;
    case 0x3D1A: goto L3D1A;
    case 0x3D1E: goto L3D1E;
    case 0x3D24: goto L3D24;
    case 0x3D28: goto L3D28;
    case 0x3D2A: goto L3D2A;
    case 0x3D2E: goto L3D2E;
    case 0x3D32: goto L3D32;
    case 0x3D36: goto L3D36;
    case 0x3D38: goto L3D38;
    case 0x3D39: goto L3D39;
    case 0x3D3A: goto L3D3A;
    case 0x3D3E: goto L3D3E;
    case 0x3D44: goto L3D44;
    case 0x3D46: goto L3D46;
    case 0x3D4C: goto L3D4C;
    case 0x3D4D: goto L3D4D;
    case 0x3D50: goto L3D50;
    case 0x3D53: goto L3D53;
    case 0x3D56: goto L3D56;
    case 0x3D58: goto L3D58;
    case 0x3D5D: goto L3D5D;
    case 0x3D62: goto L3D62;
    case 0x3D65: goto L3D65;
    case 0x3D69: goto L3D69;
    case 0x3D6A: goto L3D6A;
    case 0x3D6C: goto L3D6C;
    case 0x3D6D: goto L3D6D;
    case 0x3D6F: goto L3D6F;
    case 0x3D73: goto L3D73;
    case 0x3D74: goto L3D74;
    case 0x3D78: goto L3D78;
    case 0x3D79: goto L3D79;
    case 0x3D7D: goto L3D7D;
    case 0x3D7E: goto L3D7E;
    case 0x3D82: goto L3D82;
    case 0x3D83: goto L3D83;
    case 0x3D87: goto L3D87;
    case 0x3D8B: goto L3D8B;
    case 0x3D8C: goto L3D8C;
    case 0x3D90: goto L3D90;
    case 0x3D92: goto L3D92;
    case 0x3D94: goto L3D94;
    case 0x3D95: goto L3D95;
    case 0x3D96: goto L3D96;
    case 0x3D9A: goto L3D9A;
    case 0x3D9C: goto L3D9C;
    case 0x3D9D: goto L3D9D;
    case 0x3D9F: goto L3D9F;
    case 0x3DA2: goto L3DA2;
    case 0x3DA6: goto L3DA6;
    case 0x3DAA: goto L3DAA;
    case 0x3DAE: goto L3DAE;
    case 0x3DB0: goto L3DB0;
    case 0x3DB6: goto L3DB6;
    case 0x3DB8: goto L3DB8;
    case 0x3DBE: goto L3DBE;
    case 0x3DBF: goto L3DBF;
    case 0x3DC2: goto L3DC2;
    case 0x3DC5: goto L3DC5;
    case 0x3DC8: goto L3DC8;
    case 0x3DCA: goto L3DCA;
    case 0x3DCF: goto L3DCF;
    case 0x3DD4: goto L3DD4;
    case 0x3DD7: goto L3DD7;
    case 0x3DDB: goto L3DDB;
    case 0x3DDE: goto L3DDE;
    case 0x3DE0: goto L3DE0;
    case 0x3DE1: goto L3DE1;
    case 0x3DE4: goto L3DE4;
    case 0x3DE6: goto L3DE6;
    case 0x3DEA: goto L3DEA;
    case 0x3DEB: goto L3DEB;
    case 0x3DEF: goto L3DEF;
    case 0x3DF0: goto L3DF0;
    case 0x3DF4: goto L3DF4;
    case 0x3DF5: goto L3DF5;
    case 0x3DF9: goto L3DF9;
    case 0x3DFA: goto L3DFA;
    case 0x3DFE: goto L3DFE;
    case 0x3E02: goto L3E02;
    case 0x3E04: goto L3E04;
    case 0x3E06: goto L3E06;
    case 0x3E08: goto L3E08;
    case 0x3E0A: goto L3E0A;
    case 0x3E0D: goto L3E0D;
    case 0x3E0F: goto L3E0F;
    case 0x3E13: goto L3E13;
    case 0x3E14: goto L3E14;
    case 0x3E16: goto L3E16;
    case 0x3E17: goto L3E17;
    case 0x3E19: goto L3E19;
    case 0x3E1B: goto L3E1B;
    case 0x3E1D: goto L3E1D;
    case 0x3E20: goto L3E20;
    case 0x3E22: goto L3E22;
    case 0x3E26: goto L3E26;
    case 0x3E27: goto L3E27;
    case 0x3E29: goto L3E29;
    case 0x3E2C: goto L3E2C;
    case 0x3E32: goto L3E32;
    case 0x3E34: goto L3E34;
    case 0x3E3A: goto L3E3A;
    case 0x3E3C: goto L3E3C;
    case 0x3E42: goto L3E42;
    case 0x3E43: goto L3E43;
    case 0x3E46: goto L3E46;
    case 0x3E49: goto L3E49;
    case 0x3E4C: goto L3E4C;
    case 0x3E4E: goto L3E4E;
    case 0x3E53: goto L3E53;
    case 0x3E58: goto L3E58;
    case 0x3E5B: goto L3E5B;
    case 0x3E5F: goto L3E5F;
    case 0x3E62: goto L3E62;
    case 0x3E64: goto L3E64;
    case 0x3E65: goto L3E65;
    case 0x3E68: goto L3E68;
    case 0x3E6A: goto L3E6A;
    case 0x3E6E: goto L3E6E;
    case 0x3E6F: goto L3E6F;
    case 0x3E73: goto L3E73;
    case 0x3E74: goto L3E74;
    case 0x3E78: goto L3E78;
    case 0x3E79: goto L3E79;
    case 0x3E7D: goto L3E7D;
    case 0x3E7E: goto L3E7E;
    case 0x3E82: goto L3E82;
    case 0x3E86: goto L3E86;
    case 0x3E88: goto L3E88;
    case 0x3E8A: goto L3E8A;
    case 0x3E8C: goto L3E8C;
    case 0x3E8E: goto L3E8E;
    case 0x3E91: goto L3E91;
    case 0x3E93: goto L3E93;
    case 0x3E97: goto L3E97;
    case 0x3E98: goto L3E98;
    case 0x3E9A: goto L3E9A;
    case 0x3E9B: goto L3E9B;
    case 0x3E9D: goto L3E9D;
    case 0x3E9F: goto L3E9F;
    case 0x3EA1: goto L3EA1;
    case 0x3EA4: goto L3EA4;
    case 0x3EA6: goto L3EA6;
    case 0x3EAA: goto L3EAA;
    case 0x3EAB: goto L3EAB;
    case 0x3EAD: goto L3EAD;
    case 0x3EAF: goto L3EAF;
    case 0x3EB5: goto L3EB5;
    case 0x3EB8: goto L3EB8;
    case 0x3EBA: goto L3EBA;
    case 0x3EC0: goto L3EC0;
    case 0x3EC2: goto L3EC2;
    case 0x3EC8: goto L3EC8;
    case 0x3ECB: goto L3ECB;
    case 0x3ECF: goto L3ECF;
    case 0x3ED0: goto L3ED0;
    case 0x3ED3: goto L3ED3;
    case 0x3ED6: goto L3ED6;
    case 0x3ED9: goto L3ED9;
    case 0x3EDB: goto L3EDB;
    case 0x3EE0: goto L3EE0;
    case 0x3EE5: goto L3EE5;
    case 0x3EE8: goto L3EE8;
    case 0x3EEC: goto L3EEC;
    case 0x3EED: goto L3EED;
    case 0x3EEF: goto L3EEF;
    case 0x3EF3: goto L3EF3;
    case 0x3EF4: goto L3EF4;
    case 0x3EF8: goto L3EF8;
    case 0x3EF9: goto L3EF9;
    case 0x3EFD: goto L3EFD;
    case 0x3EFE: goto L3EFE;
    case 0x3F02: goto L3F02;
    case 0x3F03: goto L3F03;
    case 0x3F07: goto L3F07;
    case 0x3F0B: goto L3F0B;
    case 0x3F0C: goto L3F0C;
    case 0x3F0E: goto L3F0E;
    case 0x3F10: goto L3F10;
    case 0x3F12: goto L3F12;
    case 0x3F16: goto L3F16;
    case 0x3F17: goto L3F17;
    case 0x3F19: goto L3F19;
    case 0x3F1B: goto L3F1B;
    case 0x3F1D: goto L3F1D;
    case 0x3F1E: goto L3F1E;
    case 0x3F1F: goto L3F1F;
    case 0x3F21: goto L3F21;
    case 0x3F23: goto L3F23;
    case 0x3F25: goto L3F25;
    case 0x3F29: goto L3F29;
    case 0x3F2B: goto L3F2B;
    case 0x3F2C: goto L3F2C;
    case 0x3F2E: goto L3F2E;
    case 0x3F32: goto L3F32;
    case 0x3F38: goto L3F38;
    case 0x3F3A: goto L3F3A;
    case 0x3F3D: goto L3F3D;
    case 0x3F3E: goto L3F3E;
    case 0x3F3F: goto L3F3F;
    case 0x3F43: goto L3F43;
    case 0x3F44: goto L3F44;
    case 0x3F48: goto L3F48;
    case 0x3F4B: goto L3F4B;
    case 0x3F4F: goto L3F4F;
    case 0x3F54: goto L3F54;
    case 0x3F59: goto L3F59;
    case 0x3F5A: goto L3F5A;
    case 0x3F5B: goto L3F5B;
    case 0x3F5C: goto L3F5C;
    case 0x3F5D: goto L3F5D;
    case 0x3F5E: goto L3F5E;
    case 0x3F62: goto L3F62;
    case 0x3F66: goto L3F66;
    case 0x3F69: goto L3F69;
    case 0x3F6D: goto L3F6D;
    case 0x3F6F: goto L3F6F;
    case 0x3F73: goto L3F73;
    case 0x3F74: goto L3F74;
    case 0x3F77: goto L3F77;
    case 0x3F7C: goto L3F7C;
    case 0x3F7E: goto L3F7E;
    case 0x3F7F: goto L3F7F;
    case 0x3F83: goto L3F83;
    case 0x3F86: goto L3F86;
    case 0x3F88: goto L3F88;
    case 0x3F8B: goto L3F8B;
    case 0x3F8D: goto L3F8D;
    case 0x3F8F: goto L3F8F;
    case 0x3F91: goto L3F91;
    case 0x3F93: goto L3F93;
    case 0x3F95: goto L3F95;
    case 0x3F96: goto L3F96;
    case 0x3F9A: goto L3F9A;
    case 0x3F9D: goto L3F9D;
    case 0x3F9E: goto L3F9E;
    case 0x3F9F: goto L3F9F;
    case 0x3FA0: goto L3FA0;
    case 0x3FA2: goto L3FA2;
    case 0x3FA6: goto L3FA6;
    case 0x3FA7: goto L3FA7;
    case 0x3FAE: goto L3FAE;
    case 0x3FB5: goto L3FB5;
    case 0x3FB8: goto L3FB8;
    case 0x3FB9: goto L3FB9;
    case 0x3FBB: goto L3FBB;
    case 0x3FBD: goto L3FBD;
    case 0x3FC1: goto L3FC1;
    case 0x3FC4: goto L3FC4;
    case 0x3FC7: goto L3FC7;
    case 0x3FC9: goto L3FC9;
    case 0x3FCD: goto L3FCD;
    case 0x3FCF: goto L3FCF;
    case 0x3FD2: goto L3FD2;
    case 0x3FD5: goto L3FD5;
    case 0x3FD7: goto L3FD7;
    case 0x3FDB: goto L3FDB;
    case 0x3FDE: goto L3FDE;
    case 0x3FE1: goto L3FE1;
    case 0x3FE4: goto L3FE4;
    case 0x3FE6: goto L3FE6;
    case 0x3FEA: goto L3FEA;
    case 0x3FEE: goto L3FEE;
    case 0x3FF1: goto L3FF1;
    case 0x3FF4: goto L3FF4;
    case 0x3FF6: goto L3FF6;
    case 0x3FFA: goto L3FFA;
    case 0x3FFE: goto L3FFE;
    case 0x3FFF: goto L3FFF;
    case 0x4002: goto L4002;
    case 0x4004: goto L4004;
    case 0x4006: goto L4006;
    case 0x4009: goto L4009;
    case 0x400C: goto L400C;
    case 0x400F: goto L400F;
    case 0x4011: goto L4011;
    case 0x4013: goto L4013;
    case 0x4015: goto L4015;
    case 0x4019: goto L4019;
    case 0x401B: goto L401B;
    case 0x401F: goto L401F;
    case 0x4023: goto L4023;
    case 0x4025: goto L4025;
    case 0x4029: goto L4029;
    case 0x402B: goto L402B;
    case 0x402F: goto L402F;
    case 0x4033: goto L4033;
    case 0x4037: goto L4037;
    case 0x403B: goto L403B;
    case 0x403E: goto L403E;
    case 0x4040: goto L4040;
    case 0x4042: goto L4042;
    case 0x4044: goto L4044;
    case 0x4048: goto L4048;
    case 0x404A: goto L404A;
    case 0x404E: goto L404E;
    case 0x4053: goto L4053;
    case 0x4057: goto L4057;
    case 0x4059: goto L4059;
    case 0x405D: goto L405D;
    case 0x405F: goto L405F;
    case 0x4063: goto L4063;
    case 0x4068: goto L4068;
    case 0x406C: goto L406C;
    case 0x406F: goto L406F;
    case 0x4070: goto L4070;
    case 0x4071: goto L4071;
    case 0x4072: goto L4072;
    case 0x4079: goto L4079;
    case 0x407B: goto L407B;
    case 0x4080: goto L4080;
    case 0x4081: goto L4081;
    case 0x4082: goto L4082;
    case 0x4083: goto L4083;
    case 0x4084: goto L4084;
    case 0x408A: goto L408A;
    case 0x408F: goto L408F;
    case 0x4091: goto L4091;
    case 0x4094: goto L4094;
    case 0x4097: goto L4097;
    case 0x409A: goto L409A;
    case 0x409C: goto L409C;
    case 0x40A1: goto L40A1;
    default: asm_bad_entry("TMAPOPS.ASM", entry);
    }

    /* seg004_0849_3CF0  (+3CF0)
       do_set_tmctxt (opcode 0xB2): operands: a bitmap slot (low byte), then the next opcode. Points
       bmap_blk_ptr (C7FE) at slot n of the table at C802 (8 bytes a slot) for the texture-mapping
       opcodes that take their texture from the context (do_ctxt_gtmap, do_ctxt_gmap,
       do_compact_map). */
L3CF0: /* _do_set_tmctxt */
    /* 3CF0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3CF1:
    /* 3CF1  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L3CF3:
    /* 3CF3  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L3CF5:
    /* 3CF5  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L3CF7:
    /* 3CF7  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L3CF9:
    /* 3CF9  add     ax,0C802h */
    AX = add16(AX, 0xC802, 0);
L3CFC:
    /* 3CFC  mov     word ptr ds:[0C7FEh],ax */
    ww(pDS, 0xC7FE, AX);
L3CFF:
    /* 3CFF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D00:
    /* 3D00  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3D01:
    /* 3D01  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_3D05  (+3D05)
       do_set_gmap_ctxt (opcode 0xD0): operand: a flag. Sets the g-map context's map routine (C78C):
       60h (floor-style) when the flag's low byte is 0, else 20Fh (wall). */
L3D05: /* _do_set_gmap_ctxt */
    /* 3D05  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D06:
    /* 3D06  test    al,0FFh */
    logic8((uint8_t)(AL & 0xFF));
L3D08:
    /* 3D08  jne     short L3D12 */
    if (!ZF) goto L3D12;
L3D0A:
    /* 3D0A  mov     word ptr ds:[0C78Ch],60h */
    ww(pDS, 0xC78C, 0x60);
L3D10:
    /* 3D10  jmp     short L3D18 */
    goto L3D18;
L3D12: /* L3D12 */
    /* 3D12  mov     word ptr ds:[0C78Ch],20Fh */
    ww(pDS, 0xC78C, 0x20F);
L3D18: /* L3D18 */
    /* 3D18  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D19:
    /* 3D19  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3D1A:
    /* 3D1A  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_3D1E  (+3D1E)
       do_ctxt_gtmap: a g-polygon textured from the context slot (do_set_tmctxt), floor-style
       mapping. Falls into do_gwtmap's vertex loop with BP = the context's bitmap block. */
L3D1E: /* _do_ctxt_gtmap */
    /* 3D1E  mov     word ptr ds:[0C788h],60h */
    ww(pDS, 0xC788, 0x60);
L3D24:
    /* 3D24  mov     bp,word ptr ds:[0C7FEh] */
    BP = rw(pDS, 0xC7FE);
L3D28:
    /* 3D28  jmp     short L3D58 */
    goto L3D58;

    /* seg004_0849_3D2A  (+3D2A)
       do_ctxt_gmap: as do_ctxt_gtmap, with the mapping style of the g-map context (C78C). */
L3D2A: /* _do_ctxt_gmap */
    /* 3D2A  mov     bp,word ptr ds:[0C78Ch] */
    BP = rw(pDS, 0xC78C);
L3D2E:
    /* 3D2E  mov     word ptr ds:[0C788h],bp */
    ww(pDS, 0xC788, BP);
L3D32:
    /* 3D32  mov     bp,word ptr ds:[0C7FEh] */
    BP = rw(pDS, 0xC7FE);
L3D36:
    /* 3D36  jmp     short L3D58 */
    goto L3D58;

    /* seg004_0849_3D38  (+3D38)
       do_gtri: does nothing but skip to the next opcode (no operands). */
L3D38: /* _do_gtri */
    /* 3D38  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D39:
    /* 3D39  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3D3A:
    /* 3D3A  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_3D3E  (+3D3E)
       do_gtmap and do_gwtmap: a textured polygon whose vertices carry fractional texture
       coordinates. Operands: the bitmap slot, the vertex count n, then n triples (point offset into
       resbuf, u, v); u and v are fractions (0..FFFFh) of the texture's width and height, scaled here
       by the block's words 0 and 1 (the `mul` keeps the 8.8 result). do_gtmap maps floor-style
       (60h), do_gwtmap wall-style (20Fh). The tail at L3F2E, shared by every handler in the module,
       records vert_ptr and either draws the polygon flat (polym set) or calls _seg004_0849_3F77 to
       texture map it. */
L3D3E: /* _do_gtmap */
    /* 3D3E  mov     word ptr ds:[0C788h],60h */
    ww(pDS, 0xC788, 0x60);
L3D44:
    /* 3D44  jmp     short L3D4C */
    goto L3D4C;

    /* seg004_0849_3D46  (+3D46) */
L3D46: /* _do_gwtmap */
    /* 3D46  mov     word ptr ds:[0C788h],20Fh */
    ww(pDS, 0xC788, 0x20F);
L3D4C: /* L3D4C */
    /* 3D4C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D4D:
    /* 3D4D  shl     ax,3 */
    AX = (uint16_t)(AX << 3);
L3D50:
    /* 3D50  add     ax,0C802h */
    AX = (uint16_t)(AX + 0xC802);
L3D53:
    /* 3D53  mov     word ptr ds:[0C7FEh],ax */
    ww(pDS, 0xC7FE, AX);
L3D56:
    /* 3D56  mov     bp,ax */
    BP = AX;
L3D58: /* L3D58 */
    /* 3D58  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L3D5D:
    /* 3D5D  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L3D62:
    /* 3D62  mov     di,1BAh */
    DI = 0x1BA;
L3D65:
    /* 3D65  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L3D69:
    /* 3D69  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D6A:
    /* 3D6A  mov     cx,ax */
    CX = AX;
L3D6C: /* L3D6C */
    /* 3D6C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D6D:
    /* 3D6D  mov     bx,ax */
    BX = AX;
L3D6F:
    /* 3D6F  mov     ax,word ptr [bx+14D0h] */
    AX = rw(pDS, BX + 0x14D0);
L3D73:
    /* 3D73  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3D74:
    /* 3D74  mov     ax,word ptr [bx+14D2h] */
    AX = rw(pDS, BX + 0x14D2);
L3D78:
    /* 3D78  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3D79:
    /* 3D79  mov     ax,word ptr [bx+14D4h] */
    AX = rw(pDS, BX + 0x14D4);
L3D7D:
    /* 3D7D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3D7E:
    /* 3D7E  mov     al,byte ptr [bx+14D6h] */
    AL = rb(pDS, BX + 0x14D6);
L3D82:
    /* 3D82  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3D83:
    /* 3D83  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L3D87:
    /* 3D87  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L3D8B:
    /* 3D8B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D8C:
    /* 3D8C  mul     word ptr ds:[bp] */
    mul16(rw(pDS, BP));
L3D90:
    /* 3D90  mov     al,ah */
    AL = AH;
L3D92:
    /* 3D92  mov     ah,dl */
    AH = DL;
L3D94:
    /* 3D94  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3D95:
    /* 3D95  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D96:
    /* 3D96  mul     word ptr ds:[bp+2] */
    mul16(rw(pDS, BP + 0x2));
L3D9A:
    /* 3D9A  mov     ax,dx */
    AX = DX;
L3D9C:
    /* 3D9C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3D9D:
    /* 3D9D  loop    L3D6C */
    if (--CX) goto L3D6C;
L3D9F:
    /* 3D9F  jmp     L3F2E */
    goto L3F2E;

    /* seg004_0849_3DA2  (+3DA2)
       do_compact_map, do_compact_tmap, do_compact_wtmap: a textured quadrilateral in a compact form.
       Operands: the bitmap slot (not for do_compact_map, which uses the context), then four point
       numbers as bytes (each times 8 is the point's offset in resbuf). The texture coordinates are
       implied: the corners get u = width-1 or 0 and v = the v limit or 0 in a fixed order, so the
       whole texture covers the quad. GRIDDB.C's txtflr and txtwal emit these for floors and walls
       (opcodes 0xA0 and 0xA2). */
L3DA2: /* _do_compact_map */
    /* 3DA2  mov     bp,word ptr ds:[0C78Ch] */
    BP = rw(pDS, 0xC78C);
L3DA6:
    /* 3DA6  mov     word ptr ds:[0C788h],bp */
    ww(pDS, 0xC788, BP);
L3DAA:
    /* 3DAA  mov     bp,word ptr ds:[0C7FEh] */
    BP = rw(pDS, 0xC7FE);
L3DAE:
    /* 3DAE  jmp     short L3DCA */
    goto L3DCA;

    /* seg004_0849_3DB0  (+3DB0) */
L3DB0: /* _do_compact_tmap */
    /* 3DB0  mov     word ptr ds:[0C788h],60h */
    ww(pDS, 0xC788, 0x60);
L3DB6:
    /* 3DB6  jmp     short L3DBE */
    goto L3DBE;

    /* seg004_0849_3DB8  (+3DB8) */
L3DB8: /* _do_compact_wtmap */
    /* 3DB8  mov     word ptr ds:[0C788h],20Fh */
    ww(pDS, 0xC788, 0x20F);
L3DBE: /* L3DBE */
    /* 3DBE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3DBF:
    /* 3DBF  shl     ax,3 */
    AX = (uint16_t)(AX << 3);
L3DC2:
    /* 3DC2  add     ax,0C802h */
    AX = (uint16_t)(AX + 0xC802);
L3DC5:
    /* 3DC5  mov     word ptr ds:[0C7FEh],ax */
    ww(pDS, 0xC7FE, AX);
L3DC8:
    /* 3DC8  mov     bp,ax */
    BP = AX;
L3DCA: /* L3DCA */
    /* 3DCA  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L3DCF:
    /* 3DCF  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L3DD4:
    /* 3DD4  mov     di,1BAh */
    DI = 0x1BA;
L3DD7:
    /* 3DD7  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L3DDB:
    /* 3DDB  mov     cx,4 */
    CX = 0x4;
L3DDE: /* L3DDE */
    /* 3DDE  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L3DE0:
    /* 3DE0  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3DE1:
    /* 3DE1  shl     ax,3 */
    AX = (uint16_t)(AX << 3);
L3DE4:
    /* 3DE4  mov     bx,ax */
    BX = AX;
L3DE6:
    /* 3DE6  mov     ax,word ptr [bx+14D0h] */
    AX = rw(pDS, BX + 0x14D0);
L3DEA:
    /* 3DEA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3DEB:
    /* 3DEB  mov     ax,word ptr [bx+14D2h] */
    AX = rw(pDS, BX + 0x14D2);
L3DEF:
    /* 3DEF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3DF0:
    /* 3DF0  mov     ax,word ptr [bx+14D4h] */
    AX = rw(pDS, BX + 0x14D4);
L3DF4:
    /* 3DF4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3DF5:
    /* 3DF5  mov     al,byte ptr [bx+14D6h] */
    AL = rb(pDS, BX + 0x14D6);
L3DF9:
    /* 3DF9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3DFA:
    /* 3DFA  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L3DFE:
    /* 3DFE  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L3E02:
    /* 3E02  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L3E04:
    /* 3E04  mov     bl,cl */
    BL = CL;
L3E06:
    /* 3E06  dec     bl */
    BL = (uint8_t)(BL - 1);
L3E08:
    /* 3E08  dec     bl */
    BL = (uint8_t)(BL - 1);
L3E0A:
    /* 3E0A  test    bl,2 */
    logic8((uint8_t)(BL & 0x2));
L3E0D:
    /* 3E0D  jne     short L3E16 */
    if (!ZF) goto L3E16;
L3E0F:
    /* 3E0F  mov     ax,word ptr ds:[bp] */
    AX = rw(pDS, BP);
L3E13:
    /* 3E13  dec     ax */
    AX = (uint16_t)(AX - 1);
L3E14:
    /* 3E14  xchg    ah,al */
    { uint8_t t_ = AL;
    AL = AH;
    AH = t_; }
L3E16: /* L3E16 */
    /* 3E16  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E17:
    /* 3E17  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L3E19:
    /* 3E19  mov     bl,cl */
    BL = CL;
L3E1B:
    /* 3E1B  dec     bl */
    BL = (uint8_t)(BL - 1);
L3E1D:
    /* 3E1D  test    bl,2 */
    logic8((uint8_t)(BL & 0x2));
L3E20:
    /* 3E20  jne     short L3E26 */
    if (!ZF) goto L3E26;
L3E22:
    /* 3E22  mov     ax,word ptr ds:[bp+2] */
    AX = rw(pDS, BP + 0x2);
L3E26: /* L3E26 */
    /* 3E26  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E27:
    /* 3E27  loop    L3DDE */
    if (--CX) goto L3DDE;
L3E29:
    /* 3E29  jmp     L3F2E */
    goto L3F2E;

    /* seg004_0849_3E2C  (+3E2C)
       do_bcompact_map, do_bcompact_tmap, do_bcompact_wtmap: as the do_compact_ handlers with the u
       corners swapped (the `je` where do_compact has `jne`), so the texture is mirrored left to
       right; probably for faces seen from behind (b for back). do_bcompact_map sets floor-style
       mapping and jumps to L3E4C with AX unset: AX there is the BX the previous handler left (the
       dispatch's xchg), where FM Towns' do_bcompact_map loads bmap_blk_ptr first. So in DOS it
       would map with a stray block pointer; probably the C never emits it. */
L3E2C: /* _do_bcompact_map */
    /* 3E2C  mov     word ptr ds:[0C788h],60h */
    ww(pDS, 0xC788, 0x60);
L3E32:
    /* 3E32  jmp     short L3E4C */
    goto L3E4C;

    /* seg004_0849_3E34  (+3E34) */
L3E34: /* _do_bcompact_tmap */
    /* 3E34  mov     word ptr ds:[0C788h],60h */
    ww(pDS, 0xC788, 0x60);
L3E3A:
    /* 3E3A  jmp     short L3E42 */
    goto L3E42;

    /* seg004_0849_3E3C  (+3E3C) */
L3E3C: /* _do_bcompact_wtmap */
    /* 3E3C  mov     word ptr ds:[0C788h],20Fh */
    ww(pDS, 0xC788, 0x20F);
L3E42: /* L3E42 */
    /* 3E42  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3E43:
    /* 3E43  shl     ax,3 */
    AX = (uint16_t)(AX << 3);
L3E46:
    /* 3E46  add     ax,0C802h */
    AX = (uint16_t)(AX + 0xC802);
L3E49:
    /* 3E49  mov     word ptr ds:[0C7FEh],ax */
    ww(pDS, 0xC7FE, AX);
L3E4C: /* L3E4C */
    /* 3E4C  mov     bp,ax */
    BP = AX;
L3E4E:
    /* 3E4E  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L3E53:
    /* 3E53  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L3E58:
    /* 3E58  mov     di,1BAh */
    DI = 0x1BA;
L3E5B:
    /* 3E5B  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L3E5F:
    /* 3E5F  mov     cx,4 */
    CX = 0x4;
L3E62: /* L3E62 */
    /* 3E62  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L3E64:
    /* 3E64  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3E65:
    /* 3E65  shl     ax,3 */
    AX = (uint16_t)(AX << 3);
L3E68:
    /* 3E68  mov     bx,ax */
    BX = AX;
L3E6A:
    /* 3E6A  mov     ax,word ptr [bx+14D0h] */
    AX = rw(pDS, BX + 0x14D0);
L3E6E:
    /* 3E6E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E6F:
    /* 3E6F  mov     ax,word ptr [bx+14D2h] */
    AX = rw(pDS, BX + 0x14D2);
L3E73:
    /* 3E73  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E74:
    /* 3E74  mov     ax,word ptr [bx+14D4h] */
    AX = rw(pDS, BX + 0x14D4);
L3E78:
    /* 3E78  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E79:
    /* 3E79  mov     al,byte ptr [bx+14D6h] */
    AL = rb(pDS, BX + 0x14D6);
L3E7D:
    /* 3E7D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E7E:
    /* 3E7E  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L3E82:
    /* 3E82  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L3E86:
    /* 3E86  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L3E88:
    /* 3E88  mov     bl,cl */
    BL = CL;
L3E8A:
    /* 3E8A  dec     bl */
    BL = (uint8_t)(BL - 1);
L3E8C:
    /* 3E8C  dec     bl */
    BL = (uint8_t)(BL - 1);
L3E8E:
    /* 3E8E  test    bl,2 */
    logic8((uint8_t)(BL & 0x2));
L3E91:
    /* 3E91  je      short L3E9A */
    if (ZF) goto L3E9A;
L3E93:
    /* 3E93  mov     ax,word ptr ds:[bp] */
    AX = rw(pDS, BP);
L3E97:
    /* 3E97  dec     ax */
    AX = (uint16_t)(AX - 1);
L3E98:
    /* 3E98  xchg    ah,al */
    { uint8_t t_ = AL;
    AL = AH;
    AH = t_; }
L3E9A: /* L3E9A */
    /* 3E9A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E9B:
    /* 3E9B  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L3E9D:
    /* 3E9D  mov     bl,cl */
    BL = CL;
L3E9F:
    /* 3E9F  dec     bl */
    BL = (uint8_t)(BL - 1);
L3EA1:
    /* 3EA1  test    bl,2 */
    logic8((uint8_t)(BL & 0x2));
L3EA4:
    /* 3EA4  jne     short L3EAA */
    if (!ZF) goto L3EAA;
L3EA6:
    /* 3EA6  mov     ax,word ptr ds:[bp+2] */
    AX = rw(pDS, BP + 0x2);
L3EAA: /* L3EAA */
    /* 3EAA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3EAB:
    /* 3EAB  loop    L3E62 */
    if (--CX) goto L3E62;
L3EAD:
    /* 3EAD  jmp     short L3F2E */
    goto L3F2E;

    /* seg004_0849_3EAF  (+3EAF)
       do_tmap_tri, do_wtmap, do_tmap: a textured triangle (do_tmap_tri) or quadrilateral,
       floor-style (do_tmap, opcode 0x36) or wall-style (do_wtmap). Operands: the bitmap slot, then
       per vertex a point offset into resbuf and a corner word: bit 0 set gives u = width-1, a value
       of 2 or more gives v = the v limit, so each vertex sits at one corner of the texture. */
L3EAF: /* _do_tmap_tri */
    /* 3EAF  mov     word ptr ds:[0C788h],60h */
    ww(pDS, 0xC788, 0x60);
L3EB5:
    /* 3EB5  mov     cx,3 */
    CX = 0x3;
L3EB8:
    /* 3EB8  jmp     short L3ECB */
    goto L3ECB;

    /* seg004_0849_3EBA  (+3EBA) */
L3EBA: /* _do_wtmap */
    /* 3EBA  mov     word ptr ds:[0C788h],20Fh */
    ww(pDS, 0xC788, 0x20F);
L3EC0:
    /* 3EC0  jmp     short L3EC8 */
    goto L3EC8;

    /* seg004_0849_3EC2  (+3EC2) */
L3EC2: /* _do_tmap */
    /* 3EC2  mov     word ptr ds:[0C788h],60h */
    ww(pDS, 0xC788, 0x60);
L3EC8: /* L3EC8 */
    /* 3EC8  mov     cx,4 */
    CX = 0x4;
L3ECB: /* L3ECB */
    /* 3ECB  mov     word ptr ds:[0C78Eh],cx */
    ww(pDS, 0xC78E, CX);
L3ECF:
    /* 3ECF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3ED0:
    /* 3ED0  shl     ax,3 */
    AX = (uint16_t)(AX << 3);
L3ED3:
    /* 3ED3  add     ax,0C802h */
    AX = (uint16_t)(AX + 0xC802);
L3ED6:
    /* 3ED6  mov     word ptr ds:[0C7FEh],ax */
    ww(pDS, 0xC7FE, AX);
L3ED9:
    /* 3ED9  mov     bp,ax */
    BP = AX;
L3EDB:
    /* 3EDB  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L3EE0:
    /* 3EE0  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L3EE5:
    /* 3EE5  mov     di,1BAh */
    DI = 0x1BA;
L3EE8:
    /* 3EE8  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L3EEC: /* L3EEC */
    /* 3EEC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3EED:
    /* 3EED  mov     bx,ax */
    BX = AX;
L3EEF:
    /* 3EEF  mov     ax,word ptr [bx+14D0h] */
    AX = rw(pDS, BX + 0x14D0);
L3EF3:
    /* 3EF3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3EF4:
    /* 3EF4  mov     ax,word ptr [bx+14D2h] */
    AX = rw(pDS, BX + 0x14D2);
L3EF8:
    /* 3EF8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3EF9:
    /* 3EF9  mov     ax,word ptr [bx+14D4h] */
    AX = rw(pDS, BX + 0x14D4);
L3EFD:
    /* 3EFD  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3EFE:
    /* 3EFE  mov     al,byte ptr [bx+14D6h] */
    AL = rb(pDS, BX + 0x14D6);
L3F02:
    /* 3F02  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3F03:
    /* 3F03  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L3F07:
    /* 3F07  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L3F0B:
    /* 3F0B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F0C:
    /* 3F0C  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L3F0E:
    /* 3F0E  test    al,1 */
    logic8((uint8_t)(AL & 0x1));
L3F10:
    /* 3F10  je      short L3F1B */
    if (ZF) goto L3F1B;
L3F12:
    /* 3F12  mov     dx,word ptr ds:[bp] */
    DX = rw(pDS, BP);
L3F16:
    /* 3F16  dec     dx */
    DX = (uint16_t)(DX - 1);
L3F17:
    /* 3F17  xchg    dh,dl */
    { uint8_t t_ = DL;
    DL = DH;
    DH = t_; }
L3F19:
    /* 3F19  mov     dl,0FFh */
    DL = 0xFF;
L3F1B: /* L3F1B */
    /* 3F1B  mov     word ptr [di],dx */
    ww(pDS, DI, DX);
L3F1D:
    /* 3F1D  inc     di */
    DI = (uint16_t)(DI + 1);
L3F1E:
    /* 3F1E  inc     di */
    DI = (uint16_t)(DI + 1);
L3F1F:
    /* 3F1F  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L3F21:
    /* 3F21  cmp     al,2 */
    sub8(AL, 0x2, 0);
L3F23:
    /* 3F23  jl      short L3F29 */
    if (SF != OF) goto L3F29;
L3F25:
    /* 3F25  mov     dx,word ptr ds:[bp+2] */
    DX = rw(pDS, BP + 0x2);
L3F29: /* L3F29 */
    /* 3F29  mov     ax,dx */
    AX = DX;
L3F2B:
    /* 3F2B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3F2C:
    /* 3F2C  loop    L3EEC */
    if (--CX) goto L3EEC;
L3F2E: /* L3F2E */
    /* 3F2E  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L3F32:
    /* 3F32  test    word ptr ds:[2690h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x2690) & 0xFFFF));
L3F38:
    /* 3F38  jne     short _draw_solid_tmap */
    if (!ZF) goto L3F43;
L3F3A:
    /* 3F3A  call    _seg004_0849_3F77 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3F77), 0x3F3D)) != 0) return c;
L3F3D:
    /* 3F3D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F3E:
    /* 3F3E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3F3F:
    /* 3F3F  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_3F43  (+3F43)
       draw_solid_tmap: draw the polygon just collected as a flat polygon instead (polym nonzero,
       texture mapping turned off). Compacts vbuf's 12-byte vertices to the 8-byte form the flat
       polygon code reads, dropping u and v, recomputes codes_or and codes_and, and jumps to
       INTERP.ASM's draw_poly_buf_ptr, which ends the opcode. */
L3F43: /* _draw_solid_tmap */
    /* 3F43  push    si */
    push16(SI);
L3F44:
    /* 3F44  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L3F48:
    /* 3F48  mov     di,1BAh */
    DI = 0x1BA;
L3F4B:
    /* 3F4B  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L3F4F:
    /* 3F4F  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L3F54:
    /* 3F54  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L3F59: /* L3F59 */
    /* 3F59  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3F5A:
    /* 3F5A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3F5B:
    /* 3F5B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3F5C:
    /* 3F5C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F5D:
    /* 3F5D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3F5E:
    /* 3F5E  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L3F62:
    /* 3F62  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L3F66:
    /* 3F66  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L3F69:
    /* 3F69  cmp     si,word ptr ds:[1B6h] */
    sub16(SI, rw(pDS, 0x1B6), 0);
L3F6D:
    /* 3F6D  jne     L3F59 */
    if (!ZF) goto L3F59;
L3F6F:
    /* 3F6F  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L3F73:
    /* 3F73  pop     si */
    SI = pop16();
L3F74:
    /* 3F74  jmp     draw_poly_buf_ptr */
    return ASM_JMP(0x065C, 0x177E);

    /* seg004_0849_3F77  (+3F77)
       seg004_0849_3F77 (FM Towns has it unnamed, at draw_solid_tmap+53h): texture map the polygon in
       vbuf unless codes_and says it is wholly off one edge. CX = the vertex count ((vert_ptr -
       buf_ptr) / 12; a remainder breaks with int 2), BX = bmap_blk_ptr, then
       asm_texture_map_from_draw_poly (PROJPOLY.ASM). Keeps every register (pusha/popa). */
L3F77: /* _seg004_0849_3F77 */
    /* 3F77  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L3F7C:
    /* 3F7C  jne     short L3F9E */
    if (!ZF) goto L3F9E;
L3F7E:
    /* 3F7E  pusha */
    { uint16_t t_ = SP; push16(AX); push16(CX); push16(DX); push16(BX); push16(t_); push16(BP); push16(SI); push16(DI); }
L3F7F:
    /* 3F7F  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L3F83:
    /* 3F83  mov     ax,word ptr ds:[1B6h] */
    AX = rw(pDS, 0x1B6);
L3F86:
    /* 3F86  sub     ax,si */
    AX = (uint16_t)(AX - SI);
L3F88:
    /* 3F88  mov     cx,0Ch */
    CX = 0xC;
L3F8B:
    /* 3F8B  div     cl */
    if (asm_div8(CL) && (c = asm_divfault(0x065C, 0x3F8B, 2)) != 0) return c;
L3F8D:
    /* 3F8D  mov     cl,al */
    CL = AL;
L3F8F:
    /* 3F8F  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L3F91:
    /* 3F91  je      short L3F96 */
    if (ZF) goto L3F96;
L3F93:
    /* 3F93  int     2 */
    asm_halt_at(0x065C, 0x3F93, "int 2h, the debugger break");
L3F95:
    /* 3F95  nop */
    ;
L3F96: /* L3F96 */
    /* 3F96  mov     bx,word ptr ds:[0C7FEh] */
    BX = rw(pDS, 0xC7FE);
L3F9A:
    /* 3F9A  call    _asm_texture_map_from_draw_poly */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4F85), 0x3F9D)) != 0) return c;
L3F9D:
    /* 3F9D  popa */
    DI = pop16(); SI = pop16(); BP = pop16(); SP = (uint16_t)(SP + 2); BX = pop16(); DX = pop16(); CX = pop16(); AX = pop16();
L3F9E: /* L3F9E */
    /* 3F9E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_3F9F  (+3F9F)
       do_scalebm: draw a bitmap scaled to its distance, standing upright at a point (a sprite).
       Operands: the point's offset in resbuf, the bitmap record (an offset from C83A: do_uwobj
       passes 1Ch, the record at C856, and do_uwcrit 0Eh, the record at C848, which they fill), and the next opcode. in_scalebm is a second entry with DI =
       the point and AX = the slot, used by PGCACHE.ASM's do_uwobj and do_uwcrit.

       It offsets the point by the bitmap's hot spot (the record's words at +0Ah and +0Ch times bytes
       +6 and +8, shifted by 3 - the zoom at DS:25E0, times the view scales at 2490 and 14BA),
       projects that corner and the opposite one (_code_pnt, INSTANCE.ASM, gives the clip codes; a
       point behind the eye or off the wrong edge abandons the sprite), stores the screen corner and
       size in seg_370D:0B1A..0B27 and calls seg003's _seg003_0272_D1E (SCALEBM.ASM) to draw it. A
       divide overflow while projecting is caught by _overflow_handler_reg (INTERP.ASM), installed
       through SS:5A5, and also abandons the sprite. */
L3F9F: /* _do_scalebm */
    /* 3F9F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FA0:
    /* 3FA0  mov     di,ax */
    DI = AX;
L3FA2:
    /* 3FA2  add     di,14D0h */
    DI = (uint16_t)(DI + 0x14D0);
L3FA6:
    /* 3FA6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FA7: /* in_scalebm */
    /* 3FA7  mov     word ptr ss:[5A5h],offset _overflow_handler_reg */
    ww(pSS, 0x5A5, 0x3C91);
L3FAE:
    /* 3FAE  mov     word ptr cs:overflow_count,0 */
    ww(CODE004, 0x3C8F, 0x0);
L3FB5:
    /* 3FB5  add     ax,0C83Ah */
    AX = (uint16_t)(AX + 0xC83A);
L3FB8:
    /* 3FB8  push    si */
    push16(SI);
L3FB9:
    /* 3FB9  mov     si,ax */
    SI = AX;
L3FBB:
    /* 3FBB  mov     cl,3 */
    CL = 0x3;
L3FBD:
    /* 3FBD  sub     cl,byte ptr ds:[25E0h] */
    CL = sub8(CL, rb(pDS, 0x25E0), 0);
L3FC1:
    /* 3FC1  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L3FC4:
    /* 3FC4  imul    byte ptr [si+6] */
    imul8(rb(pDS, SI + 0x6));
L3FC7:
    /* 3FC7  shr     ax,cl */
    AX = (uint16_t)(AX >> (CL & 31));
L3FC9:
    /* 3FC9  imul    word ptr ds:[2490h] */
    imul16(rw(pDS, 0x2490));
L3FCD:
    /* 3FCD  sub     word ptr [di],dx */
    ww(pDS, DI, sub16(rw(pDS, DI), DX, 0));
L3FCF:
    /* 3FCF  mov     ax,word ptr [si+0Ch] */
    AX = rw(pDS, SI + 0xC);
L3FD2:
    /* 3FD2  imul    byte ptr [si+8] */
    imul8(rb(pDS, SI + 0x8));
L3FD5:
    /* 3FD5  shr     ax,cl */
    AX = (uint16_t)(AX >> (CL & 31));
L3FD7:
    /* 3FD7  imul    word ptr ds:[14BAh] */
    /* by hand: do_scalebm: --enhance full-sprites keeps a sprite's height and position as with the view level (UltimaHacks' dontShrinkSprites.asm: shr ax,1; mov dx,ax); else imul word ptr ds:[MAT_YY] */
    if (ENHANCED(ENH_FULL_SPRITES)) { AX = (uint16_t)(AX >> 1); DX = AX; }  /* the view level, as UltimaHacks */
    else imul16(rw(pDS, 0x14BA));
L3FDB:
    /* 3FDB  add     word ptr [di+2],dx */
    ww(pDS, DI + 0x2, add16(rw(pDS, DI + 0x2), DX, 0));
L3FDE:
    /* 3FDE  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L3FE1:
    /* 3FE1  imul    byte ptr [si+0Ah] */
    imul8(rb(pDS, SI + 0xA));
L3FE4:
    /* 3FE4  shr     ax,cl */
    AX = shr16(AX, CL);
L3FE6:
    /* 3FE6  imul    word ptr ds:[2490h] */
    imul16(rw(pDS, 0x2490));
L3FEA:
    /* 3FEA  mov     word ptr ds:[14ACh],dx */
    ww(pDS, 0x14AC, DX);
L3FEE:
    /* 3FEE  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L3FF1:
    /* 3FF1  imul    byte ptr [si+0Ch] */
    imul8(rb(pDS, SI + 0xC));
L3FF4:
    /* 3FF4  shr     ax,cl */
    AX = (uint16_t)(AX >> (CL & 31));
L3FF6:
    /* 3FF6  imul    word ptr ds:[14BAh] */
    /* by hand: do_scalebm: --enhance full-sprites keeps a sprite's height and position as with the view level (UltimaHacks' dontShrinkSprites.asm: shr ax,1; mov dx,ax); else imul word ptr ds:[MAT_YY] */
    if (ENHANCED(ENH_FULL_SPRITES)) { AX = (uint16_t)(AX >> 1); DX = AX; }  /* the view level, as UltimaHacks */
    else imul16(rw(pDS, 0x14BA));
L3FFA:
    /* 3FFA  mov     word ptr ds:[14AEh],dx */
    ww(pDS, 0x14AE, DX);
L3FFE:
    /* 3FFE  push    es */
    push16(asm_es);
L3FFF:
    /* 3FFF  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L4002:
    /* 4002  mov     es,ax */
    SET_ES(AX);
L4004:
    /* 4004  mov     bx,word ptr [di] */
    BX = rw(pDS, DI);
L4006:
    /* 4006  mov     cx,word ptr [di+2] */
    CX = rw(pDS, DI + 0x2);
L4009:
    /* 4009  mov     bp,word ptr [di+4] */
    BP = rw(pDS, DI + 0x4);
L400C:
    /* 400C  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x400F)) != 0) return c;
L400F:
    /* 400F  test    al,82h */
    logic8((uint8_t)(AL & 0x82));
L4011:
    /* 4011  jne     short L4080 */
    if (!ZF) goto L4080;
L4013:
    /* 4013  mov     ax,bx */
    AX = BX;
L4015:
    /* 4015  imul    word ptr ds:[2472h] */
    imul16(rw(pDS, 0x2472));
L4019:
    /* 4019  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x4019, 2)) != 0) return c;
L401B:
    /* 401B  add     ax,word ptr ds:[2474h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2474));
L401F:
    /* 401F  mov     word ptr es:[0B20h],ax */
    ww(pES, 0xB20, AX);
L4023:
    /* 4023  mov     ax,cx */
    AX = CX;
L4025:
    /* 4025  imul    word ptr ds:[2470h] */
    imul16(rw(pDS, 0x2470));
L4029:
    /* 4029  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x4029, 2)) != 0) return c;
L402B:
    /* 402B  add     ax,word ptr ds:[2476h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2476));
L402F:
    /* 402F  mov     word ptr es:[0B22h],ax */
    ww(pES, 0xB22, AX);
L4033:
    /* 4033  add     bx,word ptr ds:[14ACh] */
    BX = (uint16_t)(BX + rw(pDS, 0x14AC));
L4037:
    /* 4037  add     cx,word ptr ds:[14AEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x14AE));
L403B:
    /* 403B  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x403E)) != 0) return c;
L403E:
    /* 403E  test    al,89h */
    logic8((uint8_t)(AL & 0x89));
L4040:
    /* 4040  jne     short L4080 */
    if (!ZF) goto L4080;
L4042:
    /* 4042  mov     ax,bx */
    AX = BX;
L4044:
    /* 4044  imul    word ptr ds:[2472h] */
    imul16(rw(pDS, 0x2472));
L4048:
    /* 4048  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x4048, 2)) != 0) return c;
L404A:
    /* 404A  add     ax,word ptr ds:[2474h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2474));
L404E:
    /* 404E  sub     ax,word ptr es:[0B20h] */
    AX = (uint16_t)(AX - rw(pES, 0xB20));
L4053:
    /* 4053  mov     word ptr es:[0B26h],ax */
    ww(pES, 0xB26, AX);
L4057:
    /* 4057  mov     ax,cx */
    AX = CX;
L4059:
    /* 4059  imul    word ptr ds:[2470h] */
    imul16(rw(pDS, 0x2470));
L405D:
    /* 405D  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x405D, 2)) != 0) return c;
L405F:
    /* 405F  add     ax,word ptr ds:[2476h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2476));
L4063:
    /* 4063  sub     ax,word ptr es:[0B22h] */
    AX = (uint16_t)(AX - rw(pES, 0xB22));
L4068:
    /* 4068  mov     word ptr es:[0B24h],ax */
    ww(pES, 0xB24, AX);
L406C:
    /* 406C  mov     di,0B1Ah */
    DI = 0xB1A;
L406F:
    /* 406F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L4070:
    /* 4070  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L4071:
    /* 4071  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L4072:
    /* 4072  test    word ptr cs:overflow_count,0FFFFh */
    logic16((uint16_t)(rw(CODE004, 0x3C8F) & 0xFFFF));
L4079:
    /* 4079  jne     short L4080 */
    if (!ZF) goto L4080;
L407B:
    /* 407B  call    far ptr _seg003_0272_D1E */
    /* by hand: in_scalebm's call of scale_nibble_bitmap goes through the per-object sprite hook (3d/render.c, docs/PORT.md); the default draws it as DOS did */
    if ((c = port_sprite_draw()) != 0) return c;
L4080: /* L4080 */
    /* 4080  pop     es */
    SET_ES(pop16());
L4081:
    /* 4081  pop     si */
    SI = pop16();
L4082:
    /* 4082  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L4083:
    /* 4083  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L4084:
    /* 4084  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L408A: /* L408A */
    /* 408A  mov     word ptr cs:L4088,bp */
    ww(CODE004, 0x4088, BP);
L408F:
    /* 408F  mov     bp,sp */
    BP = SP;
L4091:
    /* 4091  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L4094:
    /* 4094  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L4097:
    /* 4097  mov     ax,7FFFh */
    AX = 0x7FFF;
L409A:
    /* 409A  xor     dx,dx */
    DX = logic16((uint16_t)(DX ^ DX));
L409C:
    /* 409C  mov     bp,word ptr cs:L4088 */
    BP = rw(CODE004, 0x4088);
L40A1:
    /* 40A1  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;
}
