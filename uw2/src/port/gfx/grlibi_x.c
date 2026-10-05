/* grlibi_x.c: replaces src/gfx/GRLIBI.ASM (seg003_0272_38B4, 38B4..43EF of its
   segment). Written by tools/asm2c.py from the assembly and the bytes it assembles to; do not
   edit by hand: change the tool, or its OVERRIDES, and run it again. Each instruction is the
   C below its source line; the comments before a routine are the .ASM file's. */
#include "x86/asmrt.h"

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

uint32_t asm_mod_GRLIBI(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x38B4: goto L38B4;
    case 0x38B8: goto L38B8;
    case 0x38BC: goto L38BC;
    case 0x38C0: goto L38C0;
    case 0x38C4: goto L38C4;
    case 0x38C8: goto L38C8;
    case 0x38CB: goto L38CB;
    case 0x38CE: goto L38CE;
    case 0x38D1: goto L38D1;
    case 0x38D3: goto L38D3;
    case 0x38D5: goto L38D5;
    case 0x38D8: goto L38D8;
    case 0x38D9: goto L38D9;
    case 0x38DA: goto L38DA;
    case 0x38DC: goto L38DC;
    case 0x38DD: goto L38DD;
    case 0x38DE: goto L38DE;
    case 0x38DF: goto L38DF;
    case 0x38E1: goto L38E1;
    case 0x38E3: goto L38E3;
    case 0x38E5: goto L38E5;
    case 0x38E8: goto L38E8;
    case 0x38EC: goto L38EC;
    case 0x38EE: goto L38EE;
    case 0x38F2: goto L38F2;
    case 0x38F4: goto L38F4;
    case 0x38F6: goto L38F6;
    case 0x38F8: goto L38F8;
    case 0x38FA: goto L38FA;
    case 0x38FC: goto L38FC;
    case 0x3900: goto L3900;
    case 0x3904: goto L3904;
    case 0x3907: goto L3907;
    case 0x390B: goto L390B;
    case 0x390F: goto L390F;
    case 0x3912: goto L3912;
    case 0x3915: goto L3915;
    case 0x3919: goto L3919;
    case 0x391C: goto L391C;
    case 0x3920: goto L3920;
    case 0x3922: goto L3922;
    case 0x3924: goto L3924;
    case 0x3927: goto L3927;
    case 0x392A: goto L392A;
    case 0x392D: goto L392D;
    case 0x3931: goto L3931;
    case 0x3932: goto L3932;
    case 0x3934: goto L3934;
    case 0x3936: goto L3936;
    case 0x3938: goto L3938;
    case 0x393A: goto L393A;
    case 0x393C: goto L393C;
    case 0x393E: goto L393E;
    case 0x393F: goto L393F;
    case 0x3940: goto L3940;
    case 0x3942: goto L3942;
    case 0x3944: goto L3944;
    case 0x3946: goto L3946;
    case 0x3947: goto L3947;
    case 0x3948: goto L3948;
    case 0x394A: goto L394A;
    case 0x394C: goto L394C;
    case 0x394D: goto L394D;
    case 0x394E: goto L394E;
    case 0x394F: goto L394F;
    case 0x3950: goto L3950;
    case 0x3954: goto L3954;
    case 0x3958: goto L3958;
    case 0x395C: goto L395C;
    case 0x395D: goto L395D;
    case 0x395E: goto L395E;
    case 0x395F: goto L395F;
    case 0x3960: goto L3960;
    case 0x3961: goto L3961;
    case 0x3962: goto L3962;
    case 0x3963: goto L3963;
    case 0x3964: goto L3964;
    case 0x3965: goto L3965;
    case 0x3969: goto L3969;
    case 0x396B: goto L396B;
    case 0x396F: goto L396F;
    case 0x3970: goto L3970;
    case 0x3971: goto L3971;
    case 0x3972: goto L3972;
    case 0x3973: goto L3973;
    case 0x3976: goto L3976;
    case 0x3977: goto L3977;
    case 0x3979: goto L3979;
    case 0x397B: goto L397B;
    case 0x397F: goto L397F;
    case 0x3982: goto L3982;
    case 0x3986: goto L3986;
    case 0x3988: goto L3988;
    case 0x3989: goto L3989;
    case 0x398C: goto L398C;
    case 0x398D: goto L398D;
    case 0x398E: goto L398E;
    case 0x398F: goto L398F;
    case 0x3990: goto L3990;
    case 0x3994: goto L3994;
    case 0x3996: goto L3996;
    case 0x3999: goto L3999;
    case 0x399A: goto L399A;
    case 0x399B: goto L399B;
    case 0x399C: goto L399C;
    case 0x399D: goto L399D;
    case 0x39A0: goto L39A0;
    case 0x39A2: goto L39A2;
    case 0x39A4: goto L39A4;
    case 0x39A8: goto L39A8;
    case 0x39AB: goto L39AB;
    case 0x39AF: goto L39AF;
    case 0x39B1: goto L39B1;
    case 0x39B2: goto L39B2;
    case 0x39B4: goto L39B4;
    case 0x39B6: goto L39B6;
    case 0x39B8: goto L39B8;
    case 0x39BA: goto L39BA;
    case 0x39BB: goto L39BB;
    case 0x39BD: goto L39BD;
    case 0x39C1: goto L39C1;
    case 0x39C3: goto L39C3;
    case 0x39C5: goto L39C5;
    case 0x39C6: goto L39C6;
    case 0x39C9: goto L39C9;
    case 0x39CB: goto L39CB;
    case 0x39CD: goto L39CD;
    case 0x39CE: goto L39CE;
    case 0x39CF: goto L39CF;
    case 0x39D1: goto L39D1;
    case 0x39D2: goto L39D2;
    case 0x39D4: goto L39D4;
    case 0x39D6: goto L39D6;
    case 0x39D8: goto L39D8;
    case 0x39DA: goto L39DA;
    case 0x39DB: goto L39DB;
    case 0x39DD: goto L39DD;
    case 0x39DF: goto L39DF;
    case 0x39E1: goto L39E1;
    case 0x39E2: goto L39E2;
    case 0x39E5: goto L39E5;
    case 0x39E8: goto L39E8;
    case 0x39EA: goto L39EA;
    case 0x39EC: goto L39EC;
    case 0x39F0: goto L39F0;
    case 0x39F3: goto L39F3;
    case 0x39F6: goto L39F6;
    case 0x39F9: goto L39F9;
    case 0x39FB: goto L39FB;
    case 0x39FD: goto L39FD;
    case 0x3A01: goto L3A01;
    case 0x3A02: goto L3A02;
    case 0x3A04: goto L3A04;
    case 0x3A06: goto L3A06;
    case 0x3A07: goto L3A07;
    case 0x3A0A: goto L3A0A;
    case 0x3A0C: goto L3A0C;
    case 0x3A0E: goto L3A0E;
    case 0x3A0F: goto L3A0F;
    case 0x3A12: goto L3A12;
    case 0x3A14: goto L3A14;
    case 0x3A16: goto L3A16;
    case 0x3A17: goto L3A17;
    case 0x3A1A: goto L3A1A;
    case 0x3A1C: goto L3A1C;
    case 0x3A1E: goto L3A1E;
    case 0x3A1F: goto L3A1F;
    case 0x3A22: goto L3A22;
    case 0x3A24: goto L3A24;
    case 0x3A26: goto L3A26;
    case 0x3A27: goto L3A27;
    case 0x3A2A: goto L3A2A;
    case 0x3A2C: goto L3A2C;
    case 0x3A2E: goto L3A2E;
    case 0x3A2F: goto L3A2F;
    case 0x3A32: goto L3A32;
    case 0x3A34: goto L3A34;
    case 0x3A36: goto L3A36;
    case 0x3A37: goto L3A37;
    case 0x3A3A: goto L3A3A;
    case 0x3A3C: goto L3A3C;
    case 0x3A3E: goto L3A3E;
    case 0x3A3F: goto L3A3F;
    case 0x3A42: goto L3A42;
    case 0x3A44: goto L3A44;
    case 0x3A46: goto L3A46;
    case 0x3A47: goto L3A47;
    case 0x3A4A: goto L3A4A;
    case 0x3A4C: goto L3A4C;
    case 0x3A4E: goto L3A4E;
    case 0x3A4F: goto L3A4F;
    case 0x3A52: goto L3A52;
    case 0x3A54: goto L3A54;
    case 0x3A56: goto L3A56;
    case 0x3A57: goto L3A57;
    case 0x3A5A: goto L3A5A;
    case 0x3A5C: goto L3A5C;
    case 0x3A5E: goto L3A5E;
    case 0x3A5F: goto L3A5F;
    case 0x3A62: goto L3A62;
    case 0x3A64: goto L3A64;
    case 0x3A66: goto L3A66;
    case 0x3A67: goto L3A67;
    case 0x3A6A: goto L3A6A;
    case 0x3A6C: goto L3A6C;
    case 0x3A6E: goto L3A6E;
    case 0x3A6F: goto L3A6F;
    case 0x3A72: goto L3A72;
    case 0x3A74: goto L3A74;
    case 0x3A76: goto L3A76;
    case 0x3A77: goto L3A77;
    case 0x3A7A: goto L3A7A;
    case 0x3A7C: goto L3A7C;
    case 0x3A7E: goto L3A7E;
    case 0x3A7F: goto L3A7F;
    case 0x3A82: goto L3A82;
    case 0x3A84: goto L3A84;
    case 0x3A86: goto L3A86;
    case 0x3A87: goto L3A87;
    case 0x3A8A: goto L3A8A;
    case 0x3A8C: goto L3A8C;
    case 0x3A8E: goto L3A8E;
    case 0x3A8F: goto L3A8F;
    case 0x3A92: goto L3A92;
    case 0x3A94: goto L3A94;
    case 0x3A96: goto L3A96;
    case 0x3A97: goto L3A97;
    case 0x3A9A: goto L3A9A;
    case 0x3A9C: goto L3A9C;
    case 0x3A9E: goto L3A9E;
    case 0x3A9F: goto L3A9F;
    case 0x3AA2: goto L3AA2;
    case 0x3AA4: goto L3AA4;
    case 0x3AA6: goto L3AA6;
    case 0x3AA7: goto L3AA7;
    case 0x3AAA: goto L3AAA;
    case 0x3AAC: goto L3AAC;
    case 0x3AAE: goto L3AAE;
    case 0x3AAF: goto L3AAF;
    case 0x3AB2: goto L3AB2;
    case 0x3AB4: goto L3AB4;
    case 0x3AB6: goto L3AB6;
    case 0x3AB7: goto L3AB7;
    case 0x3ABA: goto L3ABA;
    case 0x3ABC: goto L3ABC;
    case 0x3ABE: goto L3ABE;
    case 0x3ABF: goto L3ABF;
    case 0x3AC2: goto L3AC2;
    case 0x3AC4: goto L3AC4;
    case 0x3AC6: goto L3AC6;
    case 0x3AC7: goto L3AC7;
    case 0x3ACA: goto L3ACA;
    case 0x3ACC: goto L3ACC;
    case 0x3ACE: goto L3ACE;
    case 0x3ACF: goto L3ACF;
    case 0x3AD2: goto L3AD2;
    case 0x3AD4: goto L3AD4;
    case 0x3AD6: goto L3AD6;
    case 0x3AD7: goto L3AD7;
    case 0x3ADA: goto L3ADA;
    case 0x3ADC: goto L3ADC;
    case 0x3ADE: goto L3ADE;
    case 0x3ADF: goto L3ADF;
    case 0x3AE2: goto L3AE2;
    case 0x3AE4: goto L3AE4;
    case 0x3AE6: goto L3AE6;
    case 0x3AE7: goto L3AE7;
    case 0x3AEA: goto L3AEA;
    case 0x3AEC: goto L3AEC;
    case 0x3AEE: goto L3AEE;
    case 0x3AEF: goto L3AEF;
    case 0x3AF2: goto L3AF2;
    case 0x3AF4: goto L3AF4;
    case 0x3AF6: goto L3AF6;
    case 0x3AF7: goto L3AF7;
    case 0x3AFA: goto L3AFA;
    case 0x3AFB: goto L3AFB;
    case 0x3AFD: goto L3AFD;
    case 0x3B00: goto L3B00;
    case 0x3B03: goto L3B03;
    case 0x3B06: goto L3B06;
    case 0x3B0A: goto L3B0A;
    case 0x3B0D: goto L3B0D;
    case 0x3B10: goto L3B10;
    case 0x3B12: goto L3B12;
    case 0x3B15: goto L3B15;
    case 0x3B17: goto L3B17;
    case 0x3B1A: goto L3B1A;
    case 0x3B1D: goto L3B1D;
    case 0x3B1E: goto L3B1E;
    case 0x3B24: goto L3B24;
    case 0x3B26: goto L3B26;
    case 0x3B2C: goto L3B2C;
    case 0x3B2E: goto L3B2E;
    case 0x3B34: goto L3B34;
    case 0x3B3E: goto L3B3E;
    case 0x3B44: goto L3B44;
    case 0x3B46: goto L3B46;
    case 0x3B4A: goto L3B4A;
    case 0x3B4D: goto L3B4D;
    case 0x3B51: goto L3B51;
    case 0x3B54: goto L3B54;
    case 0x3B58: goto L3B58;
    case 0x3B5A: goto L3B5A;
    case 0x3B5E: goto L3B5E;
    case 0x3B60: goto L3B60;
    case 0x3B64: goto L3B64;
    case 0x3B65: goto L3B65;
    case 0x3B68: goto L3B68;
    case 0x3B6C: goto L3B6C;
    case 0x3B6E: goto L3B6E;
    case 0x3B72: goto L3B72;
    case 0x3B74: goto L3B74;
    case 0x3B77: goto L3B77;
    case 0x3B7B: goto L3B7B;
    case 0x3B7D: goto L3B7D;
    case 0x3B81: goto L3B81;
    case 0x3B83: goto L3B83;
    case 0x3B87: goto L3B87;
    case 0x3B8A: goto L3B8A;
    case 0x3B8E: goto L3B8E;
    case 0x3B8F: goto L3B8F;
    case 0x3B92: goto L3B92;
    case 0x3B96: goto L3B96;
    case 0x3B99: goto L3B99;
    case 0x3B9D: goto L3B9D;
    case 0x3B9F: goto L3B9F;
    case 0x3BA3: goto L3BA3;
    case 0x3BA5: goto L3BA5;
    case 0x3BA9: goto L3BA9;
    case 0x3BAA: goto L3BAA;
    case 0x3BAD: goto L3BAD;
    case 0x3BB1: goto L3BB1;
    case 0x3BB3: goto L3BB3;
    case 0x3BB7: goto L3BB7;
    case 0x3BB9: goto L3BB9;
    case 0x3BBC: goto L3BBC;
    case 0x3BC0: goto L3BC0;
    case 0x3BC2: goto L3BC2;
    case 0x3BC6: goto L3BC6;
    case 0x3BC8: goto L3BC8;
    case 0x3BCC: goto L3BCC;
    case 0x3BD0: goto L3BD0;
    case 0x3BD4: goto L3BD4;
    case 0x3BD7: goto L3BD7;
    case 0x3BDA: goto L3BDA;
    case 0x3BDD: goto L3BDD;
    case 0x3BE0: goto L3BE0;
    case 0x3C2F: goto L3C2F;
    case 0x3C32: goto L3C32;
    case 0x3C35: goto L3C35;
    case 0x3C39: goto L3C39;
    case 0x3C3F: goto L3C3F;
    case 0x3C42: goto L3C42;
    case 0x3C45: goto L3C45;
    case 0x3C47: goto L3C47;
    case 0x3C4A: goto L3C4A;
    case 0x3C4C: goto L3C4C;
    case 0x3C4F: goto L3C4F;
    case 0x3C51: goto L3C51;
    case 0x3C54: goto L3C54;
    case 0x3C57: goto L3C57;
    case 0x3C5A: goto L3C5A;
    case 0x3C5D: goto L3C5D;
    case 0x3C60: goto L3C60;
    case 0x4126: goto L4126;
    case 0x4129: goto L4129;
    case 0x412B: goto L412B;
    case 0x412F: goto L412F;
    case 0x4132: goto L4132;
    case 0x4136: goto L4136;
    case 0x413A: goto L413A;
    case 0x413E: goto L413E;
    case 0x4141: goto L4141;
    case 0x4142: goto L4142;
    case 0x4145: goto L4145;
    case 0x4146: goto L4146;
    case 0x4148: goto L4148;
    case 0x414A: goto L414A;
    case 0x414C: goto L414C;
    case 0x414E: goto L414E;
    case 0x4150: goto L4150;
    case 0x4151: goto L4151;
    case 0x4152: goto L4152;
    case 0x4158: goto L4158;
    case 0x415A: goto L415A;
    case 0x415E: goto L415E;
    case 0x4163: goto L4163;
    case 0x4167: goto L4167;
    case 0x416B: goto L416B;
    case 0x416E: goto L416E;
    case 0x4172: goto L4172;
    case 0x4173: goto L4173;
    case 0x4177: goto L4177;
    case 0x4179: goto L4179;
    case 0x417B: goto L417B;
    case 0x417D: goto L417D;
    case 0x417F: goto L417F;
    case 0x4181: goto L4181;
    case 0x4182: goto L4182;
    case 0x4186: goto L4186;
    case 0x4188: goto L4188;
    case 0x418B: goto L418B;
    case 0x418E: goto L418E;
    case 0x4191: goto L4191;
    case 0x4194: goto L4194;
    case 0x4197: goto L4197;
    case 0x419A: goto L419A;
    case 0x419D: goto L419D;
    case 0x41A0: goto L41A0;
    case 0x41A3: goto L41A3;
    case 0x41A6: goto L41A6;
    case 0x41A9: goto L41A9;
    case 0x41AC: goto L41AC;
    case 0x41AF: goto L41AF;
    case 0x41B2: goto L41B2;
    case 0x41B5: goto L41B5;
    case 0x41B8: goto L41B8;
    case 0x41BB: goto L41BB;
    case 0x41BE: goto L41BE;
    case 0x41C1: goto L41C1;
    case 0x41C4: goto L41C4;
    case 0x41C7: goto L41C7;
    case 0x41CA: goto L41CA;
    case 0x41CD: goto L41CD;
    case 0x41D0: goto L41D0;
    case 0x41D3: goto L41D3;
    case 0x41D6: goto L41D6;
    case 0x41D9: goto L41D9;
    case 0x41DC: goto L41DC;
    case 0x41DF: goto L41DF;
    case 0x41E2: goto L41E2;
    case 0x41E5: goto L41E5;
    case 0x41E8: goto L41E8;
    case 0x41EB: goto L41EB;
    case 0x41EE: goto L41EE;
    case 0x41F1: goto L41F1;
    case 0x41F4: goto L41F4;
    case 0x41F7: goto L41F7;
    case 0x41FA: goto L41FA;
    case 0x41FD: goto L41FD;
    case 0x4200: goto L4200;
    case 0x4203: goto L4203;
    case 0x4206: goto L4206;
    case 0x4209: goto L4209;
    case 0x420C: goto L420C;
    case 0x420F: goto L420F;
    case 0x4212: goto L4212;
    case 0x4215: goto L4215;
    case 0x4218: goto L4218;
    case 0x421B: goto L421B;
    case 0x421E: goto L421E;
    case 0x4221: goto L4221;
    case 0x4224: goto L4224;
    default: asm_bad_entry("GRLIBI.ASM", entry);
    }

    /* seg003_0272_38B4  (+38B4)
       upolygon: fill a polygon already inside the window. In: CX = the vertex count, the vertices
       (x, y words) from 415E. Finds the top vertex, steps the two chains (_395C going one way round,
       _3989 the other) to fill the row table, and jumps to the span writer. A polygon of zero height
       becomes one span from its least to its greatest x. */
L38B4: /* _seg003_0272_38B4 */
    /* 38B4  mov     si,word ptr ds:[49ACh] */
    SI = rw(pDS, 0x49AC);
L38B8:
    /* 38B8  and     word ptr [si],7FFFh */
    ww(pDS, SI, (uint16_t)(rw(pDS, SI) & 0x7FFF));
L38BC:
    /* 38BC  mov     word ptr ds:[49B8h],cx */
    ww(pDS, 0x49B8, CX);
L38C0:
    /* 38C0  mov     word ptr ds:[49BAh],cx */
    ww(pDS, 0x49BA, CX);
L38C4:
    /* 38C4  mov     word ptr ds:[49BCh],cx */
    ww(pDS, 0x49BC, CX);
L38C8:
    /* 38C8  mov     di,0FC19h */
    DI = 0xFC19;
L38CB:
    /* 38CB  mov     si,4160h */
    SI = 0x4160;
L38CE:
    /* 38CE  mov     bp,415Eh */
    BP = 0x415E;
L38D1:
    /* 38D1  jmp     short L38DC */
    goto L38DC;
L38D3: /* L38D3 */
    /* 38D3  mov     bp,si */
    BP = SI;
L38D5:
    /* 38D5  sub     bp,6 */
    BP = (uint16_t)(BP - 0x6);
L38D8:
    /* 38D8  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L38D9:
    /* 38D9  dec     cx */
    CX = dec16(CX);
L38DA:
    /* 38DA  je      L38E5 */
    if (ZF) goto L38E5;
L38DC: /* L38DC */
    /* 38DC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L38DD:
    /* 38DD  inc     si */
    SI = (uint16_t)(SI + 1);
L38DE:
    /* 38DE  inc     si */
    SI = (uint16_t)(SI + 1);
L38DF:
    /* 38DF  cmp     di,ax */
    sub16(DI, AX, 0);
L38E1:
    /* 38E1  jl      L38D3 */
    if (SF != OF) goto L38D3;
L38E3:
    /* 38E3  loop    L38DC */
    if (--CX) goto L38DC;
L38E5: /* L38E5 */
    /* 38E5  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L38E8:
    /* 38E8  mov     word ptr ds:[49C2h],si */
    ww(pDS, 0x49C2, SI);
L38EC:
    /* 38EC  mov     si,bp */
    SI = BP;
L38EE:
    /* 38EE  mov     word ptr ds:[49C0h],si */
    ww(pDS, 0x49C0, SI);
L38F2:
    /* 38F2  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L38F4:
    /* 38F4  mov     ax,di */
    AX = DI;
L38F6:
    /* 38F6  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L38F8:
    /* 38F8  add     di,ax */
    DI = (uint16_t)(DI + AX);
L38FA:
    /* 38FA  neg     di */
    DI = (uint16_t)-DI;
L38FC:
    /* 38FC  add     di,4978h */
    DI = (uint16_t)(DI + 0x4978);
L3900:
    /* 3900  mov     word ptr ds:[49BEh],di */
    ww(pDS, 0x49BE, DI);
L3904:
    /* 3904  call    _seg003_0272_395C */
    if ((c = asm_call(ASM_JMP(0x0085, 0x395C), 0x3907)) != 0) return c;
L3907:
    /* 3907  mov     si,word ptr ds:[49C0h] */
    SI = rw(pDS, 0x49C0);
L390B:
    /* 390B  mov     di,word ptr ds:[49BEh] */
    DI = rw(pDS, 0x49BE);
L390F:
    /* 390F  call    _seg003_0272_3989 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3989), 0x3912)) != 0) return c;
L3912:
    /* 3912  sub     di,4 */
    DI = (uint16_t)(DI - 0x4);
L3915:
    /* 3915  mov     bp,word ptr ds:[49BEh] */
    BP = rw(pDS, 0x49BE);
L3919:
    /* 3919  mov     ax,word ptr ds:[4A08h] */
    AX = rw(pDS, 0x4A08);
L391C:
    /* 391C  mov     bx,word ptr ds:[4A0Ah] */
    BX = rw(pDS, 0x4A0A);
L3920:
    /* 3920  cmp     bp,di */
    sub16(BP, DI, 0);
L3922:
    /* 3922  jne     L3944 */
    if (!ZF) goto L3944;
L3924:
    /* 3924  mov     si,415Eh */
    SI = 0x415E;
L3927:
    /* 3927  mov     bx,270Fh */
    BX = 0x270F;
L392A:
    /* 392A  mov     dx,0D8F1h */
    DX = 0xD8F1;
L392D:
    /* 392D  mov     cx,word ptr ds:[49B8h] */
    CX = rw(pDS, 0x49B8);
L3931: /* L3931 */
    /* 3931  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3932:
    /* 3932  cmp     ax,bx */
    sub16(AX, BX, 0);
L3934:
    /* 3934  jg      L3938 */
    if (!ZF && SF == OF) goto L3938;
L3936:
    /* 3936  mov     bx,ax */
    BX = AX;
L3938: /* L3938 */
    /* 3938  cmp     ax,dx */
    sub16(AX, DX, 0);
L393A:
    /* 393A  jl      L393E */
    if (SF != OF) goto L393E;
L393C:
    /* 393C  mov     dx,ax */
    DX = AX;
L393E: /* L393E */
    /* 393E  inc     si */
    SI = (uint16_t)(SI + 1);
L393F:
    /* 393F  inc     si */
    SI = (uint16_t)(SI + 1);
L3940:
    /* 3940  loop    L3931 */
    if (--CX) goto L3931;
L3942:
    /* 3942  mov     ax,dx */
    AX = DX;
L3944: /* L3944 */
    /* 3944  mov     si,bp */
    SI = BP;
L3946:
    /* 3946  inc     di */
    DI = (uint16_t)(DI + 1);
L3947:
    /* 3947  inc     di */
    DI = (uint16_t)(DI + 1);
L3948:
    /* 3948  cmp     ax,bx */
    sub16(AX, BX, 0);
L394A:
    /* 394A  jle     L394D */
    if (ZF || SF != OF) goto L394D;
L394C:
    /* 394C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L394D: /* L394D */
    /* 394D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L394E:
    /* 394E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L394F:
    /* 394F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3950:
    /* 3950  mov     word ptr ds:[49ACh],di */
    ww(pDS, 0x49AC, DI);
L3954:
    /* 3954  or      word ptr [di],8000h */
    ww(pDS, DI, logic16((uint16_t)(rw(pDS, DI) | 0x8000)));
L3958:
    /* 3958  jmp     word ptr ds:[4112h] */
    return ASM_JMP(0x0085, rw(pDS, 0x4112));

    /* seg003_0272_395C  (+395C)
       _395C: walk the chain from the top vertex backwards through the vertex list, filling one side
       of the row records (through _39BB) until an edge turns upwards. */
L395C: /* _seg003_0272_395C */
    /* 395C  inc     di */
    DI = (uint16_t)(DI + 1);
L395D:
    /* 395D  inc     di */
    DI = (uint16_t)(DI + 1);
L395E:
    /* 395E  inc     si */
    SI = (uint16_t)(SI + 1);
L395F:
    /* 395F  inc     si */
    SI = (uint16_t)(SI + 1);
L3960: /* L3960 */
    /* 3960  std */
    DF = 1;
L3961:
    /* 3961  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3962:
    /* 3962  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3963:
    /* 3963  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3964:
    /* 3964  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3965:
    /* 3965  cmp     si,4160h */
    sub16(SI, 0x4160, 0);
L3969:
    /* 3969  jae     L396F */
    if (!CF) goto L396F;
L396B:
    /* 396B  mov     si,word ptr ds:[49C2h] */
    SI = rw(pDS, 0x49C2);
L396F: /* L396F */
    /* 396F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3970:
    /* 3970  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L3971:
    /* 3971  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3972:
    /* 3972  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3973:
    /* 3973  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L3976:
    /* 3976  cld */
    DF = 0;
L3977:
    /* 3977  cmp     bx,dx */
    sub16(BX, DX, 0);
L3979:
    /* 3979  jl      L3988 */
    if (SF != OF) goto L3988;
L397B:
    /* 397B  mov     word ptr ds:[4A08h],cx */
    ww(pDS, 0x4A08, CX);
L397F:
    /* 397F  call    _seg003_0272_39BB */
    if ((c = asm_call(ASM_JMP(0x0085, 0x39BB), 0x3982)) != 0) return c;
L3982:
    /* 3982  dec     word ptr ds:[49BAh] */
    ww(pDS, 0x49BA, dec16(rw(pDS, 0x49BA)));
L3986:
    /* 3986  jg      L3960 */
    if (!ZF && SF == OF) goto L3960;
L3988: /* L3988 */
    /* 3988  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_3989  (+3989)
       _3989: the same walk forwards, for the other side. */
L3989: /* _seg003_0272_3989 */
    /* 3989  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L398C: /* L398C */
    /* 398C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L398D:
    /* 398D  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L398E:
    /* 398E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L398F:
    /* 398F  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3990:
    /* 3990  cmp     si,word ptr ds:[49C2h] */
    sub16(SI, rw(pDS, 0x49C2), 0);
L3994:
    /* 3994  jb      L3999 */
    if (CF) goto L3999;
L3996:
    /* 3996  mov     si,415Eh */
    SI = 0x415E;
L3999: /* L3999 */
    /* 3999  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L399A:
    /* 399A  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L399B:
    /* 399B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L399C:
    /* 399C  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L399D:
    /* 399D  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L39A0:
    /* 39A0  cmp     bx,dx */
    sub16(BX, DX, 0);
L39A2:
    /* 39A2  jl      L39B1 */
    if (SF != OF) goto L39B1;
L39A4:
    /* 39A4  mov     word ptr ds:[4A0Ah],cx */
    ww(pDS, 0x4A0A, CX);
L39A8:
    /* 39A8  call    _seg003_0272_39BB */
    if ((c = asm_call(ASM_JMP(0x0085, 0x39BB), 0x39AB)) != 0) return c;
L39AB:
    /* 39AB  dec     word ptr ds:[49BCh] */
    ww(pDS, 0x49BC, dec16(rw(pDS, 0x49BC)));
L39AF:
    /* 39AF  jg      L398C */
    if (!ZF && SF == OF) goto L398C;
L39B1: /* L39B1 */
    /* 39B1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L39B2: /* L39B2 */
    /* 39B2  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L39B4:
    /* 39B4  sub     cx,cx */
    CX = (uint16_t)(CX - CX);
L39B6:
    /* 39B6  sub     dx,dx */
    DX = (uint16_t)(DX - DX);
L39B8:
    /* 39B8  jmp     short L39E5 */
    goto L39E5;
L39BA: /* L39BA */
    /* 39BA  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_39BB  (+39BB)
       _39BB: one edge, from (AX, BX) to (CX, DX): its rows from BX down to DX get the stepped x
       (16.16 slope from two divides), 31 rows a call of the unrolled _3A02 and the remainder through
       the entry table at 49C4. */
L39BB: /* _seg003_0272_39BB */
    /* 39BB  sub     bx,dx */
    BX = sub16(BX, DX, 0);
L39BD:
    /* 39BD  mov     word ptr ds:[4A06h],bx */
    ww(pDS, 0x4A06, BX);
L39C1:
    /* 39C1  mov     bp,bx */
    BP = BX;
L39C3:
    /* 39C3  jle     L39BA */
    if (ZF || SF != OF) goto L39BA;
L39C5:
    /* 39C5  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L39C6:
    /* 39C6  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L39C9:
    /* 39C9  sub     cx,ax */
    CX = sub16(CX, AX, 0);
L39CB:
    /* 39CB  je      L39B2 */
    if (ZF) goto L39B2;
L39CD:
    /* 39CD  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L39CE:
    /* 39CE  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L39CF:
    /* 39CF  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x0085, 0x39CF, 2)) != 0) return c;
L39D1:
    /* 39D1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L39D2:
    /* 39D2  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L39D4:
    /* 39D4  sar     dx,1 */
    DX = sar16(DX, 1);
L39D6:
    /* 39D6  rcr     ax,1 */
    AX = rcr16(AX, 1);
L39D8:
    /* 39D8  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x0085, 0x39D8, 2)) != 0) return c;
L39DA:
    /* 39DA  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L39DB:
    /* 39DB  shl     ax,1 */
    AX = shl16(AX, 1);
L39DD:
    /* 39DD  rcl     dx,1 */
    DX = rcl16(DX, 1);
L39DF:
    /* 39DF  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L39E1:
    /* 39E1  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L39E2:
    /* 39E2  mov     dx,7FFFh */
    DX = 0x7FFF;
L39E5: /* L39E5 */
    /* 39E5  cmp     bp,20h */
    sub16(BP, 0x20, 0);
L39E8:
    /* 39E8  ja      L39F0 */
    if (!CF && !ZF) goto L39F0;
L39EA:
    /* 39EA  shl     bp,1 */
    BP = shl16(BP, 1);
L39EC:
    /* 39EC  jmp     word ptr [bp+49C4h] */
    return ASM_JMP(0x0085, rw(pSS, BP + 0x49C4));
L39F0: /* L39F0 */
    /* 39F0  call    _seg003_0272_3A02 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3A02), 0x39F3)) != 0) return c;
L39F3:
    /* 39F3  sub     bp,1Fh */
    BP = (uint16_t)(BP - 0x1F);
L39F6:
    /* 39F6  cmp     bp,20h */
    sub16(BP, 0x20, 0);
L39F9:
    /* 39F9  ja      L39F0 */
    if (!CF && !ZF) goto L39F0;
L39FB:
    /* 39FB  shl     bp,1 */
    BP = shl16(BP, 1);
L39FD:
    /* 39FD  jmp     word ptr [bp+49C4h] */
    return ASM_JMP(0x0085, rw(pSS, BP + 0x49C4));
L3A01:
    /* 3A01  even */
    ;

    /* seg003_0272_3A02  (+3A02)
       _3A02: 31 rows of an edge, unrolled: add the slope to the 16.16 x and store it in the next
       record. L3AFB, after it, is polygon's case for one or two vertices: a line through cline_si or
       a point through pixel. */
L3A02: /* _seg003_0272_3A02 */
    /* 3A02  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A04:
    /* 3A04  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A06:
    /* 3A06  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A07:
    /* 3A07  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A0A:
    /* 3A0A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A0C:
    /* 3A0C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A0E:
    /* 3A0E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A0F:
    /* 3A0F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A12:
    /* 3A12  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A14:
    /* 3A14  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A16:
    /* 3A16  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A17:
    /* 3A17  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A1A:
    /* 3A1A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A1C:
    /* 3A1C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A1E:
    /* 3A1E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A1F:
    /* 3A1F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A22:
    /* 3A22  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A24:
    /* 3A24  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A26:
    /* 3A26  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A27:
    /* 3A27  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A2A:
    /* 3A2A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A2C:
    /* 3A2C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A2E:
    /* 3A2E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A2F:
    /* 3A2F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A32:
    /* 3A32  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A34:
    /* 3A34  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A36:
    /* 3A36  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A37:
    /* 3A37  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A3A:
    /* 3A3A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A3C:
    /* 3A3C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A3E:
    /* 3A3E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A3F:
    /* 3A3F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A42:
    /* 3A42  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A44:
    /* 3A44  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A46:
    /* 3A46  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A47:
    /* 3A47  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A4A:
    /* 3A4A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A4C:
    /* 3A4C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A4E:
    /* 3A4E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A4F:
    /* 3A4F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A52:
    /* 3A52  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A54:
    /* 3A54  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A56:
    /* 3A56  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A57:
    /* 3A57  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A5A:
    /* 3A5A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A5C:
    /* 3A5C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A5E:
    /* 3A5E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A5F:
    /* 3A5F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A62:
    /* 3A62  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A64:
    /* 3A64  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A66:
    /* 3A66  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A67:
    /* 3A67  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A6A:
    /* 3A6A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A6C:
    /* 3A6C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A6E:
    /* 3A6E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A6F:
    /* 3A6F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A72:
    /* 3A72  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A74:
    /* 3A74  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A76:
    /* 3A76  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A77:
    /* 3A77  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A7A:
    /* 3A7A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A7C:
    /* 3A7C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A7E:
    /* 3A7E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A7F:
    /* 3A7F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A82:
    /* 3A82  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A84:
    /* 3A84  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A86:
    /* 3A86  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A87:
    /* 3A87  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A8A:
    /* 3A8A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A8C:
    /* 3A8C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A8E:
    /* 3A8E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A8F:
    /* 3A8F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A92:
    /* 3A92  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A94:
    /* 3A94  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A96:
    /* 3A96  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A97:
    /* 3A97  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A9A:
    /* 3A9A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A9C:
    /* 3A9C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A9E:
    /* 3A9E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A9F:
    /* 3A9F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AA2:
    /* 3AA2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AA4:
    /* 3AA4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AA6:
    /* 3AA6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AA7:
    /* 3AA7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AAA:
    /* 3AAA  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AAC:
    /* 3AAC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AAE:
    /* 3AAE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AAF:
    /* 3AAF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AB2:
    /* 3AB2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AB4:
    /* 3AB4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AB6:
    /* 3AB6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AB7:
    /* 3AB7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3ABA:
    /* 3ABA  add     dx,cx */
    DX = add16(DX, CX, 0);
L3ABC:
    /* 3ABC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3ABE:
    /* 3ABE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3ABF:
    /* 3ABF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AC2:
    /* 3AC2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AC4:
    /* 3AC4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AC6:
    /* 3AC6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AC7:
    /* 3AC7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3ACA:
    /* 3ACA  add     dx,cx */
    DX = add16(DX, CX, 0);
L3ACC:
    /* 3ACC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3ACE:
    /* 3ACE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3ACF:
    /* 3ACF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AD2:
    /* 3AD2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AD4:
    /* 3AD4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AD6:
    /* 3AD6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AD7:
    /* 3AD7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3ADA:
    /* 3ADA  add     dx,cx */
    DX = add16(DX, CX, 0);
L3ADC:
    /* 3ADC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3ADE:
    /* 3ADE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3ADF:
    /* 3ADF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AE2:
    /* 3AE2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AE4:
    /* 3AE4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AE6:
    /* 3AE6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AE7:
    /* 3AE7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AEA:
    /* 3AEA  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AEC:
    /* 3AEC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AEE:
    /* 3AEE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AEF:
    /* 3AEF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AF2:
    /* 3AF2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AF4:
    /* 3AF4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AF6:
    /* 3AF6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AF7:
    /* 3AF7  add     di,4 */
    DI = add16(DI, 0x4, 0);
L3AFA:
    /* 3AFA  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AFB: /* L3AFB */
    /* 3AFB  jne     L3B03 */
    if (!ZF) goto L3B03;
L3AFD:
    /* 3AFD  mov     si,415Eh */
    SI = 0x415E;
L3B00:
    /* 3B00  jmp     _seg003_0272_3686 */
    return ASM_JMP(0x0085, 0x3686);
L3B03: /* L3B03 */
    /* 3B03  mov     ax,word ptr ds:[415Eh] */
    AX = rw(pDS, 0x415E);
L3B06:
    /* 3B06  mov     bx,word ptr ds:[4160h] */
    BX = rw(pDS, 0x4160);
L3B0A:
    /* 3B0A  jmp     _seg003_0272_32B4 */
    return ASM_JMP(0x0085, 0x32B4);

    /* seg003_0272_3B0D  (+3B0D)
       polygon: CX = the vertex count (more than 99 is refused), vertices at 415E. Two or fewer go to
       L3AFB; otherwise shclip (GRLIBF.ASM), then upolygon. GRCORE's _4EFA is the C wrapper. */
L3B0D: /* _seg003_0272_3B0D */
    /* 3B0D  cmp     cx,63h */
    sub16(CX, 0x63, 0);
L3B10:
    /* 3B10  ja      L3B1D */
    if (!CF && !ZF) goto L3B1D;
L3B12:
    /* 3B12  cmp     cx,2 */
    sub16(CX, 0x2, 0);
L3B15:
    /* 3B15  jbe     L3AFB */
    if (CF || ZF) goto L3AFB;
L3B17:
    /* 3B17  call    _seg003_0272_34C7 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x34C7), 0x3B1A)) != 0) return c;
L3B1A:
    /* 3B1A  jmp     _seg003_0272_38B4 */
    goto L38B4;
L3B1D: /* L3B1D */
    /* 3B1D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_3B1E  (+3B1E)
       _3B1E, _3B26, _3B2E: string entries (see _3B36). _3B1E and _3B26 use the bitmap blitter L4126
       (_3B1E clipped, _3B26 unclipped); _3B2E is the clipped single-colour entry with the colour in
       DX. GRCORE reaches them through the jump table at 527E .. 528D. */
L3B1E: /* _seg003_0272_3B1E */
    /* 3B1E  mov     word ptr ds:[4D16h],offset L4126 */
    ww(pDS, 0x4D16, 0x4126);
L3B24:
    /* 3B24  jmp     short L3B4A */
    goto L3B4A;
L3B26: /* _seg003_0272_3B26 */
    /* 3B26  mov     word ptr ds:[4D16h],offset L4126 */
    ww(pDS, 0x4D16, 0x4126);
L3B2C:
    /* 3B2C  jmp     short L3B83 */
    goto L3B83;
L3B2E: /* _seg003_0272_3B2E */
    /* 3B2E  mov     word ptr ds:[4D16h],offset _seg003_0272_3C61 */
    ww(pDS, 0x4D16, 0x3C61);
L3B34:
    /* 3B34  jmp     short L3B46 */
    goto L3B46;
L3B3E: /* _seg003_0272_3B3E */
    /* 3B3E  mov     word ptr ds:[4D16h],offset _seg003_0272_3C61 */
    ww(pDS, 0x4D16, 0x3C61);
L3B44:
    /* 3B44  jmp     short L3B83 */
    goto L3B83;
L3B46: /* L3B46 */
    /* 3B46  mov     word ptr ds:[2D3Ah],dx */
    ww(pDS, 0x2D3A, DX);
L3B4A: /* L3B4A */
    /* 3B4A  mov     word ptr ds:[4A0Eh],ax */
    ww(pDS, 0x4A0E, AX);
L3B4D:
    /* 3B4D  mov     word ptr ds:[4A10h],bx */
    ww(pDS, 0x4A10, BX);
L3B51:
    /* 3B51  mov     ax,word ptr ds:[4A10h] */
    AX = rw(pDS, 0x4A10);
L3B54:
    /* 3B54  cmp     ax,word ptr ds:[3DFAh] */
    sub16(AX, rw(pDS, 0x3DFA), 0);
L3B58:
    /* 3B58  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3B5A:
    /* 3B5A  cmp     ax,word ptr ds:[3DF6h] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3B5E:
    /* 3B5E  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3B60:
    /* 3B60  sub     ax,word ptr ds:[4D0Eh] */
    AX = (uint16_t)(AX - rw(pDS, 0x4D0E));
L3B64:
    /* 3B64  inc     ax */
    AX = (uint16_t)(AX + 1);
L3B65:
    /* 3B65  mov     word ptr ds:[4A14h],ax */
    ww(pDS, 0x4A14, AX);
L3B68:
    /* 3B68  cmp     ax,word ptr ds:[3DFAh] */
    sub16(AX, rw(pDS, 0x3DFA), 0);
L3B6C:
    /* 3B6C  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3B6E:
    /* 3B6E  cmp     ax,word ptr ds:[3DF6h] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3B72:
    /* 3B72  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3B74:
    /* 3B74  mov     ax,word ptr ds:[4A0Eh] */
    AX = rw(pDS, 0x4A0E);
L3B77:
    /* 3B77  cmp     ax,word ptr ds:[3DF4h] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L3B7B:
    /* 3B7B  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3B7D:
    /* 3B7D  cmp     ax,word ptr ds:[3DF8h] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L3B81:
    /* 3B81  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3B83: /* L3B83 */
    /* 3B83  call    word ptr ds:[4D14h] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, 0x4D14)), 0x3B87)) != 0) return c;
L3B87:
    /* 3B87  mov     ax,word ptr ds:[2D3Ah] */
    AX = rw(pDS, 0x2D3A);
L3B8A:
    /* 3B8A  call    word ptr ds:[4D16h] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, 0x4D16)), 0x3B8E)) != 0) return c;
L3B8E: /* L3B8E */
    /* 3B8E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_3B8F  (+3B8F)
       _3B8F (shadowed_string_to_screen, FM Towns): the same box check, then the string drawn one
       pixel right and one down in the shadow colour (2D36), the bit buffer shifted one pixel back
       (_4150), and the string drawn again at (x, y) in the text colour (2D3A). _3BC8 is the
       unclipped entry. */
L3B8F: /* _seg003_0272_3B8F */
    /* 3B8F  mov     word ptr ds:[4A0Eh],ax */
    ww(pDS, 0x4A0E, AX);
L3B92:
    /* 3B92  mov     word ptr ds:[4A10h],bx */
    ww(pDS, 0x4A10, BX);
L3B96:
    /* 3B96  mov     ax,word ptr ds:[4A10h] */
    AX = rw(pDS, 0x4A10);
L3B99:
    /* 3B99  cmp     ax,word ptr ds:[3DFAh] */
    sub16(AX, rw(pDS, 0x3DFA), 0);
L3B9D:
    /* 3B9D  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3B9F:
    /* 3B9F  cmp     ax,word ptr ds:[3DF6h] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3BA3:
    /* 3BA3  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3BA5:
    /* 3BA5  sub     ax,word ptr ds:[4D0Eh] */
    AX = (uint16_t)(AX - rw(pDS, 0x4D0E));
L3BA9:
    /* 3BA9  inc     ax */
    AX = (uint16_t)(AX + 1);
L3BAA:
    /* 3BAA  mov     word ptr ds:[4A14h],ax */
    ww(pDS, 0x4A14, AX);
L3BAD:
    /* 3BAD  cmp     ax,word ptr ds:[3DFAh] */
    sub16(AX, rw(pDS, 0x3DFA), 0);
L3BB1:
    /* 3BB1  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3BB3:
    /* 3BB3  cmp     ax,word ptr ds:[3DF6h] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3BB7:
    /* 3BB7  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3BB9:
    /* 3BB9  mov     ax,word ptr ds:[4A0Eh] */
    AX = rw(pDS, 0x4A0E);
L3BBC:
    /* 3BBC  cmp     ax,word ptr ds:[3DF4h] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L3BC0:
    /* 3BC0  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3BC2:
    /* 3BC2  cmp     ax,word ptr ds:[3DF8h] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L3BC6:
    /* 3BC6  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3BC8: /* _seg003_0272_3BC8 */
    /* 3BC8  inc     word ptr ds:[4A0Eh] */
    ww(pDS, 0x4A0E, (uint16_t)(rw(pDS, 0x4A0E) + 1));
L3BCC:
    /* 3BCC  dec     word ptr ds:[4A10h] */
    ww(pDS, 0x4A10, (uint16_t)(rw(pDS, 0x4A10) - 1));
L3BD0:
    /* 3BD0  call    word ptr ds:[4D14h] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, 0x4D14)), 0x3BD4)) != 0) return c;
L3BD4:
    /* 3BD4  mov     ax,word ptr ds:[2D36h] */
    AX = rw(pDS, 0x2D36);
L3BD7:
    /* 3BD7  call    _seg003_0272_3C61 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3C61), 0x3BDA)) != 0) return c;
L3BDA:
    /* 3BDA  call    _seg003_0272_4150 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x4150), 0x3BDD)) != 0) return c;
L3BDD:
    /* 3BDD  mov     ax,word ptr ds:[2D3Ah] */
    AX = rw(pDS, 0x2D3A);
L3BE0:
    /* 3BE0  jmp     short _seg003_0272_3C61 */
    return ASM_JMP(0x0085, 0x3C61);
L3C2F:
    /* 3C2F  mov     bx,offset _seg003_0272_52CC */
    BX = 0x52CC;
L3C32: /* L3C32 */
    /* 3C32  mov     word ptr ds:[4116h],ax */
    ww(pDS, 0x4116, AX);
L3C35:
    /* 3C35  mov     word ptr ds:[4112h],bx */
    ww(pDS, 0x4112, BX);
L3C39:
    /* 3C39  mov     word ptr ds:[4114h],0FFFFh */
    ww(pDS, 0x4114, 0xFFFF);
L3C3F:
    /* 3C3F  jmp     _seg003_0272_3416 */
    return ASM_JMP(0x0085, 0x3416);
L3C42:
    /* 3C42  mov     bx,offset _seg003_0272_52DB */
    BX = 0x52DB;
L3C45:
    /* 3C45  jmp     L3C32 */
    goto L3C32;
L3C47:
    /* 3C47  mov     bx,offset _seg003_0272_52D8 */
    BX = 0x52D8;
L3C4A:
    /* 3C4A  jmp     L3C32 */
    goto L3C32;
L3C4C: /* L3C4C */
    /* 3C4C  mov     bx,offset _seg003_0272_52E7 */
    BX = 0x52E7;
L3C4F:
    /* 3C4F  jmp     L3C32 */
    goto L3C32;
L3C51:
    /* 3C51  push    word ptr [si+2] */
    push16(rw(pDS, SI + 0x2));
L3C54:
    /* 3C54  mov     ax,word ptr [si+6] */
    AX = rw(pDS, SI + 0x6);
L3C57:
    /* 3C57  mov     word ptr [si+2],ax */
    ww(pDS, SI + 0x2, AX);
L3C5A:
    /* 3C5A  call    L3C4C */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3C4C), 0x3C5D)) != 0) return c;
L3C5D:
    /* 3C5D  pop     word ptr [si+2] */
    { uint16_t t_ = pop16(); ww(pDS, SI + 0x2, t_); }
L3C60:
    /* 3C60  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L4126: /* L4126 */
    /* 4126  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L4129:
    /* 4129  mov     si,ax */
    SI = AX;
L412B:
    /* 412B  mov     di,word ptr ds:[4D06h] */
    DI = rw(pDS, 0x4D06);
L412F:
    /* 412F  mov     ax,word ptr ds:[4A0Eh] */
    AX = rw(pDS, 0x4A0E);
L4132:
    /* 4132  mov     bx,word ptr ds:[4A10h] */
    BX = rw(pDS, 0x4A10);
L4136:
    /* 4136  mov     cx,word ptr ds:[4D04h] */
    CX = rw(pDS, 0x4D04);
L413A:
    /* 413A  mov     dx,word ptr ds:[4D0Eh] */
    DX = rw(pDS, 0x4D0E);
L413E:
    /* 413E  jmp     _seg003_0272_222B */
    return ASM_JMP(0x0085, 0x222B);
L4141: /* L4141 */
    /* 4141  popf */
    asm_set_flags(pop16());
L4142:
    /* 4142  call    _seg003_0272_4186 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x4186), 0x4145)) != 0) return c;
L4145:
    /* 4145  pushf */
    push16(asm_flags());
L4146:
    /* 4146  sub     bx,ax */
    BX = (uint16_t)(BX - AX);
L4148:
    /* 4148  sub     si,ax */
    SI = (uint16_t)(SI - AX);
L414A:
    /* 414A  cmp     bx,ax */
    sub16(BX, AX, 0);
L414C:
    /* 414C  ja      L4141 */
    if (!CF && !ZF) goto L4141;
L414E:
    /* 414E  jmp     short L417B */
    goto L417B;

    /* seg003_0272_4150  (+4150)
       _4150 (shift_left_1, FM Towns): shift the whole bit buffer left by one pixel (through _4186
       and the entry table at 4C24) and move the text's x back by one, stepping the screen address
       back a byte when x crosses a multiple of 4. */
L4150: /* _seg003_0272_4150 */
    /* 4150  clc */
    CF = 0;
L4151:
    /* 4151  pushf */
    push16(asm_flags());
L4152:
    /* 4152  test    word ptr ds:[4A0Eh],3 */
    logic16((uint16_t)(rw(pDS, 0x4A0E) & 0x3));
L4158:
    /* 4158  jne     L4163 */
    if (!ZF) goto L4163;
L415A:
    /* 415A  dec     word ptr ds:[4D06h] */
    ww(pDS, 0x4D06, (uint16_t)(rw(pDS, 0x4D06) - 1));
L415E:
    /* 415E  sub     word ptr ds:[4A0Eh],4 */
    ww(pDS, 0x4A0E, (uint16_t)(rw(pDS, 0x4A0E) - 0x4));
L4163: /* L4163 */
    /* 4163  inc     word ptr ds:[4A10h] */
    ww(pDS, 0x4A10, (uint16_t)(rw(pDS, 0x4A10) + 1));
L4167:
    /* 4167  dec     word ptr ds:[4A0Eh] */
    ww(pDS, 0x4A0E, (uint16_t)(rw(pDS, 0x4A0E) - 1));
L416B:
    /* 416B  mov     ax,35h */
    AX = 0x35;
L416E:
    /* 416E  mov     bx,word ptr ds:[4D02h] */
    BX = rw(pDS, 0x4D02);
L4172:
    /* 4172  inc     bx */
    BX = (uint16_t)(BX + 1);
L4173:
    /* 4173  mov     si,word ptr ds:[4D00h] */
    SI = rw(pDS, 0x4D00);
L4177:
    /* 4177  cmp     bx,ax */
    sub16(BX, AX, 0);
L4179:
    /* 4179  ja      L4141 */
    if (!CF && !ZF) goto L4141;
L417B: /* L417B */
    /* 417B  add     si,ax */
    SI = (uint16_t)(SI + AX);
L417D:
    /* 417D  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L417F:
    /* 417F  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4181:
    /* 4181  popf */
    asm_set_flags(pop16());
L4182:
    /* 4182  jmp     word ptr [bx+4C24h] */
    return ASM_JMP(0x0085, rw(pDS, BX + 0x4C24));

    /* seg003_0272_4186  (+4186)
       _4186: one bit left through 53 bytes of a buffer row, unrolled (rcl from SI down to SI-34h);
       entered part way through for shorter rows.

       After its ret is the rasteriser (4225h, the routine 4D14 points at): measure the string
       (string_width), size the bit buffer (4D04 bytes wide, font height rows) and clear it, then OR
       each glyph's rows into it at the running bit position, through the unrolled loop L42CA..L438D
       entered by the font height (L43A5). Characters outside 20h..7Ah are drawn as '?'. */
L4186: /* _seg003_0272_4186 */
    /* 4186  rcl     byte ptr [si],1 */
    wb(pDS, SI, rcl8(rb(pDS, SI), 1));
L4188:
    /* 4188  rcl     byte ptr [si-1],1 */
    wb(pDS, SI + 0xFFFF, rcl8(rb(pDS, SI + 0xFFFF), 1));
L418B:
    /* 418B  rcl     byte ptr [si-2],1 */
    wb(pDS, SI + 0xFFFE, rcl8(rb(pDS, SI + 0xFFFE), 1));
L418E:
    /* 418E  rcl     byte ptr [si-3],1 */
    wb(pDS, SI + 0xFFFD, rcl8(rb(pDS, SI + 0xFFFD), 1));
L4191:
    /* 4191  rcl     byte ptr [si-4],1 */
    wb(pDS, SI + 0xFFFC, rcl8(rb(pDS, SI + 0xFFFC), 1));
L4194:
    /* 4194  rcl     byte ptr [si-5],1 */
    wb(pDS, SI + 0xFFFB, rcl8(rb(pDS, SI + 0xFFFB), 1));
L4197:
    /* 4197  rcl     byte ptr [si-6],1 */
    wb(pDS, SI + 0xFFFA, rcl8(rb(pDS, SI + 0xFFFA), 1));
L419A:
    /* 419A  rcl     byte ptr [si-7],1 */
    wb(pDS, SI + 0xFFF9, rcl8(rb(pDS, SI + 0xFFF9), 1));
L419D:
    /* 419D  rcl     byte ptr [si-8],1 */
    wb(pDS, SI + 0xFFF8, rcl8(rb(pDS, SI + 0xFFF8), 1));
L41A0:
    /* 41A0  rcl     byte ptr [si-9],1 */
    wb(pDS, SI + 0xFFF7, rcl8(rb(pDS, SI + 0xFFF7), 1));
L41A3:
    /* 41A3  rcl     byte ptr [si-0Ah],1 */
    wb(pDS, SI + 0xFFF6, rcl8(rb(pDS, SI + 0xFFF6), 1));
L41A6:
    /* 41A6  rcl     byte ptr [si-0Bh],1 */
    wb(pDS, SI + 0xFFF5, rcl8(rb(pDS, SI + 0xFFF5), 1));
L41A9:
    /* 41A9  rcl     byte ptr [si-0Ch],1 */
    wb(pDS, SI + 0xFFF4, rcl8(rb(pDS, SI + 0xFFF4), 1));
L41AC:
    /* 41AC  rcl     byte ptr [si-0Dh],1 */
    wb(pDS, SI + 0xFFF3, rcl8(rb(pDS, SI + 0xFFF3), 1));
L41AF:
    /* 41AF  rcl     byte ptr [si-0Eh],1 */
    wb(pDS, SI + 0xFFF2, rcl8(rb(pDS, SI + 0xFFF2), 1));
L41B2:
    /* 41B2  rcl     byte ptr [si-0Fh],1 */
    wb(pDS, SI + 0xFFF1, rcl8(rb(pDS, SI + 0xFFF1), 1));
L41B5:
    /* 41B5  rcl     byte ptr [si-10h],1 */
    wb(pDS, SI + 0xFFF0, rcl8(rb(pDS, SI + 0xFFF0), 1));
L41B8:
    /* 41B8  rcl     byte ptr [si-11h],1 */
    wb(pDS, SI + 0xFFEF, rcl8(rb(pDS, SI + 0xFFEF), 1));
L41BB:
    /* 41BB  rcl     byte ptr [si-12h],1 */
    wb(pDS, SI + 0xFFEE, rcl8(rb(pDS, SI + 0xFFEE), 1));
L41BE:
    /* 41BE  rcl     byte ptr [si-13h],1 */
    wb(pDS, SI + 0xFFED, rcl8(rb(pDS, SI + 0xFFED), 1));
L41C1:
    /* 41C1  rcl     byte ptr [si-14h],1 */
    wb(pDS, SI + 0xFFEC, rcl8(rb(pDS, SI + 0xFFEC), 1));
L41C4:
    /* 41C4  rcl     byte ptr [si-15h],1 */
    wb(pDS, SI + 0xFFEB, rcl8(rb(pDS, SI + 0xFFEB), 1));
L41C7:
    /* 41C7  rcl     byte ptr [si-16h],1 */
    wb(pDS, SI + 0xFFEA, rcl8(rb(pDS, SI + 0xFFEA), 1));
L41CA:
    /* 41CA  rcl     byte ptr [si-17h],1 */
    wb(pDS, SI + 0xFFE9, rcl8(rb(pDS, SI + 0xFFE9), 1));
L41CD:
    /* 41CD  rcl     byte ptr [si-18h],1 */
    wb(pDS, SI + 0xFFE8, rcl8(rb(pDS, SI + 0xFFE8), 1));
L41D0:
    /* 41D0  rcl     byte ptr [si-19h],1 */
    wb(pDS, SI + 0xFFE7, rcl8(rb(pDS, SI + 0xFFE7), 1));
L41D3:
    /* 41D3  rcl     byte ptr [si-1Ah],1 */
    wb(pDS, SI + 0xFFE6, rcl8(rb(pDS, SI + 0xFFE6), 1));
L41D6:
    /* 41D6  rcl     byte ptr [si-1Bh],1 */
    wb(pDS, SI + 0xFFE5, rcl8(rb(pDS, SI + 0xFFE5), 1));
L41D9:
    /* 41D9  rcl     byte ptr [si-1Ch],1 */
    wb(pDS, SI + 0xFFE4, rcl8(rb(pDS, SI + 0xFFE4), 1));
L41DC:
    /* 41DC  rcl     byte ptr [si-1Dh],1 */
    wb(pDS, SI + 0xFFE3, rcl8(rb(pDS, SI + 0xFFE3), 1));
L41DF:
    /* 41DF  rcl     byte ptr [si-1Eh],1 */
    wb(pDS, SI + 0xFFE2, rcl8(rb(pDS, SI + 0xFFE2), 1));
L41E2:
    /* 41E2  rcl     byte ptr [si-1Fh],1 */
    wb(pDS, SI + 0xFFE1, rcl8(rb(pDS, SI + 0xFFE1), 1));
L41E5:
    /* 41E5  rcl     byte ptr [si-20h],1 */
    wb(pDS, SI + 0xFFE0, rcl8(rb(pDS, SI + 0xFFE0), 1));
L41E8:
    /* 41E8  rcl     byte ptr [si-21h],1 */
    wb(pDS, SI + 0xFFDF, rcl8(rb(pDS, SI + 0xFFDF), 1));
L41EB:
    /* 41EB  rcl     byte ptr [si-22h],1 */
    wb(pDS, SI + 0xFFDE, rcl8(rb(pDS, SI + 0xFFDE), 1));
L41EE:
    /* 41EE  rcl     byte ptr [si-23h],1 */
    wb(pDS, SI + 0xFFDD, rcl8(rb(pDS, SI + 0xFFDD), 1));
L41F1:
    /* 41F1  rcl     byte ptr [si-24h],1 */
    wb(pDS, SI + 0xFFDC, rcl8(rb(pDS, SI + 0xFFDC), 1));
L41F4:
    /* 41F4  rcl     byte ptr [si-25h],1 */
    wb(pDS, SI + 0xFFDB, rcl8(rb(pDS, SI + 0xFFDB), 1));
L41F7:
    /* 41F7  rcl     byte ptr [si-26h],1 */
    wb(pDS, SI + 0xFFDA, rcl8(rb(pDS, SI + 0xFFDA), 1));
L41FA:
    /* 41FA  rcl     byte ptr [si-27h],1 */
    wb(pDS, SI + 0xFFD9, rcl8(rb(pDS, SI + 0xFFD9), 1));
L41FD:
    /* 41FD  rcl     byte ptr [si-28h],1 */
    wb(pDS, SI + 0xFFD8, rcl8(rb(pDS, SI + 0xFFD8), 1));
L4200:
    /* 4200  rcl     byte ptr [si-29h],1 */
    wb(pDS, SI + 0xFFD7, rcl8(rb(pDS, SI + 0xFFD7), 1));
L4203:
    /* 4203  rcl     byte ptr [si-2Ah],1 */
    wb(pDS, SI + 0xFFD6, rcl8(rb(pDS, SI + 0xFFD6), 1));
L4206:
    /* 4206  rcl     byte ptr [si-2Bh],1 */
    wb(pDS, SI + 0xFFD5, rcl8(rb(pDS, SI + 0xFFD5), 1));
L4209:
    /* 4209  rcl     byte ptr [si-2Ch],1 */
    wb(pDS, SI + 0xFFD4, rcl8(rb(pDS, SI + 0xFFD4), 1));
L420C:
    /* 420C  rcl     byte ptr [si-2Dh],1 */
    wb(pDS, SI + 0xFFD3, rcl8(rb(pDS, SI + 0xFFD3), 1));
L420F:
    /* 420F  rcl     byte ptr [si-2Eh],1 */
    wb(pDS, SI + 0xFFD2, rcl8(rb(pDS, SI + 0xFFD2), 1));
L4212:
    /* 4212  rcl     byte ptr [si-2Fh],1 */
    wb(pDS, SI + 0xFFD1, rcl8(rb(pDS, SI + 0xFFD1), 1));
L4215:
    /* 4215  rcl     byte ptr [si-30h],1 */
    wb(pDS, SI + 0xFFD0, rcl8(rb(pDS, SI + 0xFFD0), 1));
L4218:
    /* 4218  rcl     byte ptr [si-31h],1 */
    wb(pDS, SI + 0xFFCF, rcl8(rb(pDS, SI + 0xFFCF), 1));
L421B:
    /* 421B  rcl     byte ptr [si-32h],1 */
    wb(pDS, SI + 0xFFCE, rcl8(rb(pDS, SI + 0xFFCE), 1));
L421E:
    /* 421E  rcl     byte ptr [si-33h],1 */
    wb(pDS, SI + 0xFFCD, rcl8(rb(pDS, SI + 0xFFCD), 1));
L4221:
    /* 4221  rcl     byte ptr [si-34h],1 */
    wb(pDS, SI + 0xFFCC, rcl8(rb(pDS, SI + 0xFFCC), 1));
L4224:
    /* 4224  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
