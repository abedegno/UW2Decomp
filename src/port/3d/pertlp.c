/* pertlp.c: replaces src/3d/PERTLP.ASM (seg004_0849_46F0, 46F0..4B2C of its
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

uint32_t asm_mod_PERTLP(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x46F0: goto L46F0;
    case 0x46F4: goto L46F4;
    case 0x46F8: goto L46F8;
    case 0x46FC: goto L46FC;
    case 0x4700: goto L4700;
    case 0x4704: goto L4704;
    case 0x4708: goto L4708;
    case 0x470C: goto L470C;
    case 0x4710: goto L4710;
    case 0x4715: goto L4715;
    case 0x4719: goto L4719;
    case 0x471D: goto L471D;
    case 0x4721: goto L4721;
    case 0x4725: goto L4725;
    case 0x4729: goto L4729;
    case 0x472D: goto L472D;
    case 0x4730: goto L4730;
    case 0x4733: goto L4733;
    case 0x4737: goto L4737;
    case 0x4738: goto L4738;
    case 0x473B: goto L473B;
    case 0x473D: goto L473D;
    case 0x473F: goto L473F;
    case 0x4743: goto L4743;
    case 0x4744: goto L4744;
    case 0x4748: goto L4748;
    case 0x474C: goto L474C;
    case 0x4751: goto L4751;
    case 0x4756: goto L4756;
    case 0x475B: goto L475B;
    case 0x475F: goto L475F;
    case 0x4763: goto L4763;
    case 0x4767: goto L4767;
    case 0x476B: goto L476B;
    case 0x4771: goto L4771;
    case 0x4775: goto L4775;
    case 0x4779: goto L4779;
    case 0x477D: goto L477D;
    case 0x477F: goto L477F;
    case 0x4782: goto L4782;
    case 0x4785: goto L4785;
    case 0x4789: goto L4789;
    case 0x478E: goto L478E;
    case 0x4791: goto L4791;
    case 0x4796: goto L4796;
    case 0x4798: goto L4798;
    case 0x479C: goto L479C;
    case 0x47A0: goto L47A0;
    case 0x47A5: goto L47A5;
    case 0x47A9: goto L47A9;
    case 0x47AB: goto L47AB;
    case 0x47AC: goto L47AC;
    case 0x47AD: goto L47AD;
    case 0x47B1: goto L47B1;
    case 0x47B6: goto L47B6;
    case 0x47B9: goto L47B9;
    case 0x47BE: goto L47BE;
    case 0x47C0: goto L47C0;
    case 0x47C4: goto L47C4;
    case 0x47C8: goto L47C8;
    case 0x47CD: goto L47CD;
    case 0x47D1: goto L47D1;
    case 0x47D3: goto L47D3;
    case 0x47D4: goto L47D4;
    case 0x47D5: goto L47D5;
    case 0x47D9: goto L47D9;
    case 0x47DE: goto L47DE;
    case 0x47E1: goto L47E1;
    case 0x47E6: goto L47E6;
    case 0x47E8: goto L47E8;
    case 0x47EC: goto L47EC;
    case 0x47F0: goto L47F0;
    case 0x47F5: goto L47F5;
    case 0x47F8: goto L47F8;
    case 0x47FD: goto L47FD;
    case 0x47FF: goto L47FF;
    case 0x4803: goto L4803;
    case 0x4807: goto L4807;
    case 0x480B: goto L480B;
    case 0x480F: goto L480F;
    case 0x4814: goto L4814;
    case 0x4819: goto L4819;
    case 0x481E: goto L481E;
    case 0x4822: goto L4822;
    case 0x4826: goto L4826;
    case 0x482B: goto L482B;
    case 0x4830: goto L4830;
    case 0x4834: goto L4834;
    case 0x4838: goto L4838;
    case 0x483B: goto L483B;
    case 0x483F: goto L483F;
    case 0x4841: goto L4841;
    case 0x4843: goto L4843;
    case 0x484C: goto L484C;
    case 0x484E: goto L484E;
    case 0x4857: goto L4857;
    case 0x485D: goto L485D;
    case 0x4863: goto L4863;
    case 0x4866: goto L4866;
    case 0x486A: goto L486A;
    case 0x486F: goto L486F;
    case 0x4874: goto L4874;
    case 0x4879: goto L4879;
    case 0x487C: goto L487C;
    case 0x487E: goto L487E;
    case 0x4881: goto L4881;
    case 0x4883: goto L4883;
    case 0x4886: goto L4886;
    case 0x4888: goto L4888;
    case 0x488B: goto L488B;
    case 0x488F: goto L488F;
    case 0x4892: goto L4892;
    case 0x4896: goto L4896;
    case 0x4898: goto L4898;
    case 0x489C: goto L489C;
    case 0x489F: goto L489F;
    case 0x48A3: goto L48A3;
    case 0x48A8: goto L48A8;
    case 0x48AB: goto L48AB;
    case 0x48AC: goto L48AC;
    case 0x48B5: goto L48B5;
    case 0x48B9: goto L48B9;
    case 0x48BB: goto L48BB;
    case 0x48BF: goto L48BF;
    case 0x48C2: goto L48C2;
    case 0x48C6: goto L48C6;
    case 0x48CB: goto L48CB;
    case 0x48CE: goto L48CE;
    case 0x48CF: goto L48CF;
    case 0x48D8: goto L48D8;
    case 0x48DD: goto L48DD;
    case 0x48E2: goto L48E2;
    case 0x48E7: goto L48E7;
    case 0x48EB: goto L48EB;
    case 0x48ED: goto L48ED;
    case 0x48F1: goto L48F1;
    case 0x48F3: goto L48F3;
    case 0x48F8: goto L48F8;
    case 0x48FA: goto L48FA;
    case 0x48FE: goto L48FE;
    case 0x4900: goto L4900;
    case 0x4905: goto L4905;
    case 0x4909: goto L4909;
    case 0x490D: goto L490D;
    case 0x490F: goto L490F;
    case 0x4913: goto L4913;
    case 0x4915: goto L4915;
    case 0x4918: goto L4918;
    case 0x491C: goto L491C;
    case 0x4921: goto L4921;
    case 0x4924: goto L4924;
    case 0x4925: goto L4925;
    case 0x4929: goto L4929;
    case 0x492D: goto L492D;
    case 0x4931: goto L4931;
    case 0x4935: goto L4935;
    case 0x4939: goto L4939;
    case 0x493D: goto L493D;
    case 0x4942: goto L4942;
    case 0x4946: goto L4946;
    case 0x494A: goto L494A;
    case 0x494E: goto L494E;
    case 0x4952: goto L4952;
    case 0x4956: goto L4956;
    case 0x495A: goto L495A;
    case 0x495D: goto L495D;
    case 0x4960: goto L4960;
    case 0x4964: goto L4964;
    case 0x4965: goto L4965;
    case 0x4968: goto L4968;
    case 0x496A: goto L496A;
    case 0x496C: goto L496C;
    case 0x4970: goto L4970;
    case 0x4971: goto L4971;
    case 0x4975: goto L4975;
    case 0x4979: goto L4979;
    case 0x497E: goto L497E;
    case 0x4983: goto L4983;
    case 0x4988: goto L4988;
    case 0x498C: goto L498C;
    case 0x4990: goto L4990;
    case 0x4994: goto L4994;
    case 0x4998: goto L4998;
    case 0x499E: goto L499E;
    case 0x49A2: goto L49A2;
    case 0x49A6: goto L49A6;
    case 0x49AA: goto L49AA;
    case 0x49AC: goto L49AC;
    case 0x49AF: goto L49AF;
    case 0x49B2: goto L49B2;
    case 0x49B6: goto L49B6;
    case 0x49BB: goto L49BB;
    case 0x49BF: goto L49BF;
    case 0x49C1: goto L49C1;
    case 0x49C2: goto L49C2;
    case 0x49C3: goto L49C3;
    case 0x49C7: goto L49C7;
    case 0x49CC: goto L49CC;
    case 0x49CF: goto L49CF;
    case 0x49D4: goto L49D4;
    case 0x49D6: goto L49D6;
    case 0x49DA: goto L49DA;
    case 0x49DE: goto L49DE;
    case 0x49E3: goto L49E3;
    case 0x49E7: goto L49E7;
    case 0x49E9: goto L49E9;
    case 0x49EA: goto L49EA;
    case 0x49EB: goto L49EB;
    case 0x49EF: goto L49EF;
    case 0x49F4: goto L49F4;
    case 0x49F7: goto L49F7;
    case 0x49FC: goto L49FC;
    case 0x49FE: goto L49FE;
    case 0x4A02: goto L4A02;
    case 0x4A06: goto L4A06;
    case 0x4A0B: goto L4A0B;
    case 0x4A0E: goto L4A0E;
    case 0x4A13: goto L4A13;
    case 0x4A15: goto L4A15;
    case 0x4A19: goto L4A19;
    case 0x4A1D: goto L4A1D;
    case 0x4A21: goto L4A21;
    case 0x4A26: goto L4A26;
    case 0x4A2B: goto L4A2B;
    case 0x4A30: goto L4A30;
    case 0x4A31: goto L4A31;
    case 0x4A35: goto L4A35;
    case 0x4A3A: goto L4A3A;
    case 0x4A3D: goto L4A3D;
    case 0x4A3E: goto L4A3E;
    case 0x4A44: goto L4A44;
    case 0x4A48: goto L4A48;
    case 0x4A4C: goto L4A4C;
    case 0x4A50: goto L4A50;
    case 0x4A53: goto L4A53;
    case 0x4A57: goto L4A57;
    case 0x4A59: goto L4A59;
    case 0x4A5B: goto L4A5B;
    case 0x4A64: goto L4A64;
    case 0x4A66: goto L4A66;
    case 0x4A6F: goto L4A6F;
    case 0x4A71: goto L4A71;
    case 0x4A72: goto L4A72;
    case 0x4A74: goto L4A74;
    case 0x4A77: goto L4A77;
    case 0x4A7B: goto L4A7B;
    case 0x4A7F: goto L4A7F;
    case 0x4A83: goto L4A83;
    case 0x4A85: goto L4A85;
    case 0x4A88: goto L4A88;
    case 0x4A8C: goto L4A8C;
    case 0x4A91: goto L4A91;
    case 0x4A94: goto L4A94;
    case 0x4A95: goto L4A95;
    case 0x4A9A: goto L4A9A;
    case 0x4A9F: goto L4A9F;
    case 0x4AA4: goto L4AA4;
    case 0x4AA7: goto L4AA7;
    case 0x4AAB: goto L4AAB;
    case 0x4AAF: goto L4AAF;
    case 0x4AB3: goto L4AB3;
    case 0x4AB5: goto L4AB5;
    case 0x4AB8: goto L4AB8;
    case 0x4ABC: goto L4ABC;
    case 0x4AC1: goto L4AC1;
    case 0x4AC4: goto L4AC4;
    case 0x4AC5: goto L4AC5;
    case 0x4ACA: goto L4ACA;
    case 0x4ACF: goto L4ACF;
    case 0x4AD4: goto L4AD4;
    case 0x4AD5: goto L4AD5;
    case 0x4AD7: goto L4AD7;
    case 0x4AD8: goto L4AD8;
    case 0x4ADA: goto L4ADA;
    case 0x4ADD: goto L4ADD;
    case 0x4AE1: goto L4AE1;
    case 0x4AE5: goto L4AE5;
    case 0x4AE9: goto L4AE9;
    case 0x4AEB: goto L4AEB;
    case 0x4AEE: goto L4AEE;
    case 0x4AF2: goto L4AF2;
    case 0x4AF7: goto L4AF7;
    case 0x4AFA: goto L4AFA;
    case 0x4AFB: goto L4AFB;
    case 0x4B00: goto L4B00;
    case 0x4B05: goto L4B05;
    case 0x4B09: goto L4B09;
    case 0x4B0C: goto L4B0C;
    case 0x4B10: goto L4B10;
    case 0x4B12: goto L4B12;
    case 0x4B16: goto L4B16;
    case 0x4B1A: goto L4B1A;
    case 0x4B1C: goto L4B1C;
    case 0x4B1F: goto L4B1F;
    case 0x4B23: goto L4B23;
    case 0x4B28: goto L4B28;
    case 0x4B2B: goto L4B2B;
    default: asm_bad_entry("PERTLP.ASM", entry);
    }

    /* _asm_texture_map_scanline_per_tlp: a perspective-correct shaded span. No register inputs (see
       the module header); changes EAX..EDX, ESI, EDI, EBP. It divides u * w and v * w by w once for
       every two pixels and draws both with that texel, each with its own dithered shade, so its
       texture resolution across the span is halved. The steps are doubled to match. The shade step
       is written into the two `add ...,12345678h` instructions (L48AC+5, L48CF+5) and log2 width
       into the `shl ax,2` at L488F+2: self-modifying code. */
L46F0: /* _asm_texture_map_scanline_per_tlp */
    /* 46F0  mov     eax,dword ptr ds:[0CF74h] */
    EAX = rd(pDS, 0xCF74);
L46F4:
    /* 46F4  mov     dword ptr ds:[0CCB8h],eax */
    wd(pDS, 0xCCB8, EAX);
L46F8:
    /* 46F8  mov     eax,dword ptr ds:[0CF60h] */
    EAX = rd(pDS, 0xCF60);
L46FC:
    /* 46FC  mov     dword ptr ds:[0CCB0h],eax */
    wd(pDS, 0xCCB0, EAX);
L4700:
    /* 4700  mov     eax,dword ptr ds:[0CF64h] */
    EAX = rd(pDS, 0xCF64);
L4704:
    /* 4704  mov     dword ptr ds:[0CCB4h],eax */
    wd(pDS, 0xCCB4, EAX);
L4708:
    /* 4708  mov     eax,dword ptr ds:[0CF7Ch] */
    EAX = rd(pDS, 0xCF7C);
L470C:
    /* 470C  mov     dword ptr ds:[0CCBCh],eax */
    wd(pDS, 0xCCBC, EAX);
L4710:
    /* 4710  mov     ebx,dword ptr ds:[0CF68h] */
    EBX = rd(pDS, 0xCF68);
L4715:
    /* 4715  add     ebx,41h */
    EBX = (uint32_t)(EBX + 0x41);
L4719:
    /* 4719  sar     ebx,10h */
    EBX = (uint32_t)((int32_t)EBX >> 16);
L471D:
    /* 471D  mov     word ptr ds:[0CCC8h],bx */
    ww(pDS, 0xCCC8, BX);
L4721:
    /* 4721  mov     eax,dword ptr ds:[0CF5Ch] */
    EAX = rd(pDS, 0xCF5C);
L4725:
    /* 4725  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L4729:
    /* 4729  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L472D:
    /* 472D  mov     word ptr ds:[0CCC6h],ax */
    ww(pDS, 0xCCC6, AX);
L4730:
    /* 4730  mov     word ptr ds:[0CCC4h],ax */
    ww(pDS, 0xCCC4, AX);
L4733:
    /* 4733  mov     di,word ptr ds:[0CF5Ah] */
    DI = rw(pDS, 0xCF5A);
L4737:
    /* 4737  push    ds */
    push16(asm_ds);
L4738:
    /* 4738  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L473B:
    /* 473B  mov     ds,ax */
    SET_DS(AX);
L473D:
    /* 473D  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L473F:
    /* 473F  mov     di,word ptr [di+95Ch] */
    DI = rw(pDS, DI + 0x95C);
L4743:
    /* 4743  pop     ds */
    SET_DS(pop16());
L4744:
    /* 4744  add     di,word ptr ds:[0CCC4h] */
    DI = (uint16_t)(DI + rw(pDS, 0xCCC4));
L4748:
    /* 4748  mov     eax,dword ptr ds:[0CF60h] */
    EAX = rd(pDS, 0xCF60);
L474C:
    /* 474C  or      eax,dword ptr ds:[0CF6Ch] */
    EAX = (uint32_t)(EAX | rd(pDS, 0xCF6C));
L4751:
    /* 4751  or      eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX | rd(pDS, 0xCF64));
L4756:
    /* 4756  or      eax,dword ptr ds:[0CF70h] */
    EAX = logic32((uint32_t)(EAX | rd(pDS, 0xCF70)));
L475B:
    /* 475B  js      _seg004_0849_4924 */
    if (SF) goto L4924;
L475F:
    /* 475F  sub     bx,word ptr ds:[0CCC4h] */
    BX = sub16(BX, rw(pDS, 0xCCC4), 0);
L4763:
    /* 4763  je      _seg004_0849_48ED */
    if (ZF) goto L48ED;
L4767:
    /* 4767  js      _seg004_0849_4924 */
    if (SF) goto L4924;
L476B:
    /* 476B  mov     eax,10000h */
    EAX = 0x10000;
L4771:
    /* 4771  shl     ebx,10h */
    EBX = (uint32_t)(EBX << 16);
L4775:
    /* 4775  rol     eax,10h */
    EAX = rol32(EAX, 16);
L4779:
    /* 4779  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L477D:
    /* 477D  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L477F:
    /* 477F  idiv    ebx */
    if (asm_idiv32(EBX) && (c = asm_divfault(0x065C, 0x477F, 3)) != 0) return c;
L4782:
    /* 4782  mov     ebx,eax */
    EBX = EAX;
L4785:
    /* 4785  mov     eax,dword ptr ds:[0CF78h] */
    EAX = rd(pDS, 0xCF78);
L4789:
    /* 4789  sub     eax,dword ptr ds:[0CF74h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF74));
L478E:
    /* 478E  imul    ebx */
    imul32(EBX);
L4791:
    /* 4791  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4796:
    /* 4796  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4798:
    /* 4798  mov     dword ptr ds:[0CCA8h],eax */
    wd(pDS, 0xCCA8, EAX);
L479C:
    /* 479C  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L47A0:
    /* 47A0  xor     eax,dword ptr ds:[0CF60h] */
    EAX = (uint32_t)(EAX ^ rd(pDS, 0xCF60));
L47A5:
    /* 47A5  shr     eax,10h */
    EAX = shr32(EAX, 16);
L47A9:
    /* 47A9  je      _seg004_0849_47C0 */
    if (ZF) goto L47C0;
L47AB:
    /* 47AB  nop */
    ;
L47AC:
    /* 47AC  nop */
    ;
L47AD:
    /* 47AD  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L47B1:
    /* 47B1  sub     eax,dword ptr ds:[0CF60h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF60));
L47B6:
    /* 47B6  imul    ebx */
    imul32(EBX);
L47B9:
    /* 47B9  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L47BE:
    /* 47BE  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L47C0: /* _seg004_0849_47C0 */
    /* 47C0  mov     dword ptr ds:[0CCA0h],eax */
    wd(pDS, 0xCCA0, EAX);
L47C4:
    /* 47C4  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L47C8:
    /* 47C8  xor     eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX ^ rd(pDS, 0xCF64));
L47CD:
    /* 47CD  shr     eax,10h */
    EAX = shr32(EAX, 16);
L47D1:
    /* 47D1  je      L47E8 */
    if (ZF) goto L47E8;
L47D3:
    /* 47D3  nop */
    ;
L47D4:
    /* 47D4  nop */
    ;
L47D5:
    /* 47D5  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L47D9:
    /* 47D9  sub     eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF64));
L47DE:
    /* 47DE  imul    ebx */
    imul32(EBX);
L47E1:
    /* 47E1  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L47E6:
    /* 47E6  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L47E8: /* L47E8 */
    /* 47E8  mov     dword ptr ds:[0CCA4h],eax */
    wd(pDS, 0xCCA4, EAX);
L47EC:
    /* 47EC  mov     eax,dword ptr ds:[0CF80h] */
    EAX = rd(pDS, 0xCF80);
L47F0:
    /* 47F0  sub     eax,dword ptr ds:[0CF7Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF7C));
L47F5:
    /* 47F5  imul    ebx */
    imul32(EBX);
L47F8:
    /* 47F8  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L47FD:
    /* 47FD  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L47FF:
    /* 47FF  mov     dword ptr ds:[0CCACh],eax */
    wd(pDS, 0xCCAC, EAX);
L4803:
    /* 4803  mov     cx,word ptr ds:[0CCC8h] */
    CX = rw(pDS, 0xCCC8);
L4807:
    /* 4807  sub     cx,word ptr ds:[0CCC4h] */
    CX = (uint16_t)(CX - rw(pDS, 0xCCC4));
L480B:
    /* 480B  mov     word ptr ds:[0CCCCh],cx */
    ww(pDS, 0xCCCC, CX);
L480F:
    /* 480F  mov     esi,dword ptr ds:[0CCB8h] */
    ESI = rd(pDS, 0xCCB8);
L4814:
    /* 4814  mov     ebp,dword ptr ds:[0CCB0h] */
    EBP = rd(pDS, 0xCCB0);
L4819:
    /* 4819  mov     ecx,dword ptr ds:[0CCB4h] */
    ECX = rd(pDS, 0xCCB4);
L481E:
    /* 481E  mov     eax,dword ptr ds:[0CCACh] */
    EAX = rd(pDS, 0xCCAC);
L4822:
    /* 4822  shl     eax,8 */
    EAX = (uint32_t)(EAX << 8);
L4826:
    /* 4826  mov     dword ptr cs:[L48AC+5],eax */
    wd(CODE004, 0x48B1, EAX);
L482B:
    /* 482B  mov     dword ptr cs:[L48CF+5],eax */
    wd(CODE004, 0x48D4, EAX);
L4830:
    /* 4830  mov     eax,dword ptr ds:[0CCBCh] */
    EAX = rd(pDS, 0xCCBC);
L4834:
    /* 4834  mov     dword ptr ds:[0CCC0h],eax */
    wd(pDS, 0xCCC0, EAX);
L4838:
    /* 4838  mov     ax,word ptr ds:[0CF5Eh] */
    AX = rw(pDS, 0xCF5E);
L483B:
    /* 483B  xor     ax,word ptr ds:[0CF5Ah] */
    AX = (uint16_t)(AX ^ rw(pDS, 0xCF5A));
L483F:
    /* 483F  shr     ax,1 */
    AX = shr16(AX, 1);
L4841:
    /* 4841  jb      short L484E */
    if (CF) goto L484E;
L4843:
    /* 4843  add     dword ptr ds:[0CCC0h],8000h */
    wd(pDS, 0xCCC0, (uint32_t)(rd(pDS, 0xCCC0) + 0x8000));
L484C:
    /* 484C  jmp     short L4857 */
    goto L4857;
L484E: /* L484E */
    /* 484E  add     dword ptr ds:[0CCBCh],8000h */
    wd(pDS, 0xCCBC, (uint32_t)(rd(pDS, 0xCCBC) + 0x8000));
L4857: /* L4857 */
    /* 4857  shl     dword ptr ds:[0CCBCh],8 */
    wd(pDS, 0xCCBC, (uint32_t)(rd(pDS, 0xCCBC) << 8));
L485D:
    /* 485D  shl     dword ptr ds:[0CCC0h],8 */
    wd(pDS, 0xCCC0, (uint32_t)(rd(pDS, 0xCCC0) << 8));
L4863:
    /* 4863  mov     al,byte ptr ds:[0CCECh] */
    AL = rb(pDS, 0xCCEC);
L4866:
    /* 4866  mov     byte ptr cs:[L488F+2],al */
    wb(CODE004, 0x4891, AL);
L486A:
    /* 486A  shl     dword ptr ds:[0CCA0h],1 */
    wd(pDS, 0xCCA0, (uint32_t)(rd(pDS, 0xCCA0) << 1));
L486F:
    /* 486F  shl     dword ptr ds:[0CCA4h],1 */
    wd(pDS, 0xCCA4, (uint32_t)(rd(pDS, 0xCCA4) << 1));
L4874:
    /* 4874  shl     dword ptr ds:[0CCA8h],1 */
    wd(pDS, 0xCCA8, (uint32_t)(rd(pDS, 0xCCA8) << 1));
L4879: /* L4879 */
    /* 4879  mov     eax,ebp */
    EAX = EBP;
L487C:
    /* 487C  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L487E:
    /* 487E  idiv    esi */
    if (asm_idiv32(ESI) && (c = asm_divfault(0x065C, 0x487E, 3)) != 0) return c;
L4881:
    /* 4881  mov     bx,ax */
    BX = AX;
L4883:
    /* 4883  mov     eax,ecx */
    EAX = ECX;
L4886:
    /* 4886  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4888:
    /* 4888  idiv    esi */
    if (asm_idiv32(ESI) && (c = asm_divfault(0x065C, 0x4888, 3)) != 0) return c;
L488B:
    /* 488B  and     bx,word ptr ds:[0CCE8h] */
    BX = (uint16_t)(BX & rw(pDS, 0xCCE8));
L488F: /* L488F */
    /* 488F  shl     ax,2 */
    AX = (uint16_t)(AX << (rb(CODE004, 0x4891) & 31));
L4892:
    /* 4892  and     ax,word ptr ds:[0CCEAh] */
    AX = (uint16_t)(AX & rw(pDS, 0xCCEA));
L4896:
    /* 4896  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L4898:
    /* 4898  mov     word ptr ds:[0CCCEh],bx */
    ww(pDS, 0xCCCE, BX);
L489C:
    /* 489C  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L489F:
    /* 489F  mov     bh,byte ptr ds:[0CCBFh] */
    BH = rb(pDS, 0xCCBF);
L48A3:
    /* 48A3  mov     al,byte ptr cs:lightabs[bx] */
    AL = rb(CODE004, BX + 0x6D3E);
L48A8:
    /* 48A8  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L48AB:
    /* 48AB  inc     di */
    DI = (uint16_t)(DI + 1);
L48AC: /* L48AC */
    /* 48AC  add     dword ptr ds:[0CCBCh],12345678h */
    wd(pDS, 0xCCBC, (uint32_t)(rd(pDS, 0xCCBC) + rd(CODE004, 0x48B1)));
L48B5:
    /* 48B5  dec     word ptr ds:[0CCCCh] */
    ww(pDS, 0xCCCC, dec16(rw(pDS, 0xCCCC)));
L48B9:
    /* 48B9  je      short _seg004_0849_48ED */
    if (ZF) goto L48ED;
L48BB:
    /* 48BB  mov     bx,word ptr ds:[0CCCEh] */
    BX = rw(pDS, 0xCCCE);
L48BF:
    /* 48BF  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L48C2:
    /* 48C2  mov     bh,byte ptr ds:[0CCC3h] */
    BH = rb(pDS, 0xCCC3);
L48C6:
    /* 48C6  mov     al,byte ptr cs:lightabs[bx] */
    AL = rb(CODE004, BX + 0x6D3E);
L48CB:
    /* 48CB  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L48CE:
    /* 48CE  inc     di */
    DI = (uint16_t)(DI + 1);
L48CF: /* L48CF */
    /* 48CF  add     dword ptr ds:[0CCC0h],12345678h */
    wd(pDS, 0xCCC0, (uint32_t)(rd(pDS, 0xCCC0) + rd(CODE004, 0x48D4)));
L48D8:
    /* 48D8  add     ebp,dword ptr ds:[0CCA0h] */
    EBP = (uint32_t)(EBP + rd(pDS, 0xCCA0));
L48DD:
    /* 48DD  add     ecx,dword ptr ds:[0CCA4h] */
    ECX = (uint32_t)(ECX + rd(pDS, 0xCCA4));
L48E2:
    /* 48E2  add     esi,dword ptr ds:[0CCA8h] */
    ESI = (uint32_t)(ESI + rd(pDS, 0xCCA8));
L48E7:
    /* 48E7  dec     word ptr ds:[0CCCCh] */
    ww(pDS, 0xCCCC, dec16(rw(pDS, 0xCCCC)));
L48EB:
    /* 48EB  jne     L4879 */
    if (!ZF) goto L4879;
L48ED: /* _seg004_0849_48ED */
    /* 48ED  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L48F1:
    /* 48F1  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L48F3:
    /* 48F3  idiv    dword ptr ds:[0CF78h] */
    if (asm_idiv32(rd(pDS, 0xCF78)) && (c = asm_divfault(0x065C, 0x48F3, 5)) != 0) return c;
L48F8:
    /* 48F8  mov     si,ax */
    SI = AX;
L48FA:
    /* 48FA  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L48FE:
    /* 48FE  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4900:
    /* 4900  idiv    dword ptr ds:[0CF78h] */
    if (asm_idiv32(rd(pDS, 0xCF78)) && (c = asm_divfault(0x065C, 0x4900, 5)) != 0) return c;
L4905:
    /* 4905  and     si,word ptr ds:[0CCE8h] */
    SI = logic16((uint16_t)(SI & rw(pDS, 0xCCE8)));
L4909:
    /* 4909  mov     cx,word ptr ds:[0CCECh] */
    CX = rw(pDS, 0xCCEC);
L490D:
    /* 490D  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L490F:
    /* 490F  and     ax,word ptr ds:[0CCEAh] */
    AX = (uint16_t)(AX & rw(pDS, 0xCCEA));
L4913:
    /* 4913  add     si,ax */
    SI = add16(SI, AX, 0);
L4915:
    /* 4915  mov     bl,byte ptr gs:[si] */
    BL = rb(pGS, SI);
L4918:
    /* 4918  mov     bh,byte ptr ds:[0CF82h] */
    BH = rb(pDS, 0xCF82);
L491C:
    /* 491C  mov     al,byte ptr cs:lightabs[bx] */
    AL = rb(CODE004, BX + 0x6D3E);
L4921:
    /* 4921  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L4924: /* _seg004_0849_4924 */
    /* 4924  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* _asm_texture_map_scanline_lin_tlp: a linear (affine) shaded span, two pixels per loop with an
       odd last pixel (the carry from `shr cx,1`, kept by pushf/popf). v is kept multiplied by the
       width (shifted left by log2 width) so a texel address needs no shift in the loop. A u or v
       step is 0 when the integer parts of the two ends are the same. */
L4925: /* _asm_texture_map_scanline_lin_tlp */
    /* 4925  mov     eax,dword ptr ds:[0CF60h] */
    EAX = rd(pDS, 0xCF60);
L4929:
    /* 4929  mov     dword ptr ds:[0CCB0h],eax */
    wd(pDS, 0xCCB0, EAX);
L492D:
    /* 492D  mov     eax,dword ptr ds:[0CF64h] */
    EAX = rd(pDS, 0xCF64);
L4931:
    /* 4931  mov     dword ptr ds:[0CCB4h],eax */
    wd(pDS, 0xCCB4, EAX);
L4935:
    /* 4935  mov     eax,dword ptr ds:[0CF7Ch] */
    EAX = rd(pDS, 0xCF7C);
L4939:
    /* 4939  mov     dword ptr ds:[0CCBCh],eax */
    wd(pDS, 0xCCBC, EAX);
L493D:
    /* 493D  mov     ebx,dword ptr ds:[0CF68h] */
    EBX = rd(pDS, 0xCF68);
L4942:
    /* 4942  add     ebx,41h */
    EBX = (uint32_t)(EBX + 0x41);
L4946:
    /* 4946  sar     ebx,10h */
    EBX = (uint32_t)((int32_t)EBX >> 16);
L494A:
    /* 494A  mov     word ptr ds:[0CCC8h],bx */
    ww(pDS, 0xCCC8, BX);
L494E:
    /* 494E  mov     eax,dword ptr ds:[0CF5Ch] */
    EAX = rd(pDS, 0xCF5C);
L4952:
    /* 4952  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L4956:
    /* 4956  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L495A:
    /* 495A  mov     word ptr ds:[0CCC6h],ax */
    ww(pDS, 0xCCC6, AX);
L495D:
    /* 495D  mov     word ptr ds:[0CCC4h],ax */
    ww(pDS, 0xCCC4, AX);
L4960:
    /* 4960  mov     di,word ptr ds:[0CF5Ah] */
    DI = rw(pDS, 0xCF5A);
L4964:
    /* 4964  push    ds */
    push16(asm_ds);
L4965:
    /* 4965  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L4968:
    /* 4968  mov     ds,ax */
    SET_DS(AX);
L496A:
    /* 496A  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L496C:
    /* 496C  mov     di,word ptr [di+95Ch] */
    DI = rw(pDS, DI + 0x95C);
L4970:
    /* 4970  pop     ds */
    SET_DS(pop16());
L4971:
    /* 4971  add     di,word ptr ds:[0CCC4h] */
    DI = (uint16_t)(DI + rw(pDS, 0xCCC4));
L4975:
    /* 4975  mov     eax,dword ptr ds:[0CF60h] */
    EAX = rd(pDS, 0xCF60);
L4979:
    /* 4979  or      eax,dword ptr ds:[0CF6Ch] */
    EAX = (uint32_t)(EAX | rd(pDS, 0xCF6C));
L497E:
    /* 497E  or      eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX | rd(pDS, 0xCF64));
L4983:
    /* 4983  or      eax,dword ptr ds:[0CF70h] */
    EAX = logic32((uint32_t)(EAX | rd(pDS, 0xCF70)));
L4988:
    /* 4988  js      L4B2B */
    if (SF) goto L4B2B;
L498C:
    /* 498C  sub     bx,word ptr ds:[0CCC4h] */
    BX = sub16(BX, rw(pDS, 0xCCC4), 0);
L4990:
    /* 4990  je      L4B05 */
    if (ZF) goto L4B05;
L4994:
    /* 4994  js      L4B05 */
    if (SF) goto L4B05;
L4998:
    /* 4998  mov     eax,10000h */
    EAX = 0x10000;
L499E:
    /* 499E  shl     ebx,10h */
    EBX = (uint32_t)(EBX << 16);
L49A2:
    /* 49A2  rol     eax,10h */
    EAX = rol32(EAX, 16);
L49A6:
    /* 49A6  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L49AA:
    /* 49AA  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L49AC:
    /* 49AC  idiv    ebx */
    if (asm_idiv32(EBX) && (c = asm_divfault(0x065C, 0x49AC, 3)) != 0) return c;
L49AF:
    /* 49AF  mov     ebx,eax */
    EBX = EAX;
L49B2:
    /* 49B2  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L49B6:
    /* 49B6  xor     eax,dword ptr ds:[0CF60h] */
    EAX = (uint32_t)(EAX ^ rd(pDS, 0xCF60));
L49BB:
    /* 49BB  shr     eax,10h */
    EAX = shr32(EAX, 16);
L49BF:
    /* 49BF  je      L49D6 */
    if (ZF) goto L49D6;
L49C1:
    /* 49C1  nop */
    ;
L49C2:
    /* 49C2  nop */
    ;
L49C3:
    /* 49C3  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L49C7:
    /* 49C7  sub     eax,dword ptr ds:[0CF60h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF60));
L49CC:
    /* 49CC  imul    ebx */
    imul32(EBX);
L49CF:
    /* 49CF  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L49D4:
    /* 49D4  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L49D6: /* L49D6 */
    /* 49D6  mov     dword ptr ds:[0CCA0h],eax */
    wd(pDS, 0xCCA0, EAX);
L49DA:
    /* 49DA  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L49DE:
    /* 49DE  xor     eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX ^ rd(pDS, 0xCF64));
L49E3:
    /* 49E3  shr     eax,10h */
    EAX = shr32(EAX, 16);
L49E7:
    /* 49E7  je      L49FE */
    if (ZF) goto L49FE;
L49E9:
    /* 49E9  nop */
    ;
L49EA:
    /* 49EA  nop */
    ;
L49EB:
    /* 49EB  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L49EF:
    /* 49EF  sub     eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF64));
L49F4:
    /* 49F4  imul    ebx */
    imul32(EBX);
L49F7:
    /* 49F7  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L49FC:
    /* 49FC  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L49FE: /* L49FE */
    /* 49FE  mov     dword ptr ds:[0CCA4h],eax */
    wd(pDS, 0xCCA4, EAX);
L4A02:
    /* 4A02  mov     eax,dword ptr ds:[0CF80h] */
    EAX = rd(pDS, 0xCF80);
L4A06:
    /* 4A06  sub     eax,dword ptr ds:[0CF7Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF7C));
L4A0B:
    /* 4A0B  imul    ebx */
    imul32(EBX);
L4A0E:
    /* 4A0E  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4A13:
    /* 4A13  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4A15:
    /* 4A15  mov     dword ptr ds:[0CCACh],eax */
    wd(pDS, 0xCCAC, EAX);
L4A19:
    /* 4A19  mov     cx,word ptr ds:[0CCC8h] */
    CX = rw(pDS, 0xCCC8);
L4A1D:
    /* 4A1D  sub     cx,word ptr ds:[0CCC4h] */
    CX = sub16(CX, rw(pDS, 0xCCC4), 0);
L4A21:
    /* 4A21  mov     esi,dword ptr ds:[0CCA0h] */
    ESI = rd(pDS, 0xCCA0);
L4A26:
    /* 4A26  mov     ebp,dword ptr ds:[0CCA4h] */
    EBP = rd(pDS, 0xCCA4);
L4A2B:
    /* 4A2B  mov     edx,dword ptr ds:[0CCACh] */
    EDX = rd(pDS, 0xCCAC);
L4A30:
    /* 4A30  push    cx */
    push16(CX);
L4A31:
    /* 4A31  mov     cx,word ptr ds:[0CCECh] */
    CX = rw(pDS, 0xCCEC);
L4A35:
    /* 4A35  shl     dword ptr ds:[0CCB4h],cl */
    wd(pDS, 0xCCB4, shl32(rd(pDS, 0xCCB4), CL));
L4A3A:
    /* 4A3A  shl     ebp,cl */
    EBP = (uint32_t)(EBP << (CL & 31));
L4A3D:
    /* 4A3D  pop     cx */
    CX = pop16();
L4A3E:
    /* 4A3E  shl     dword ptr ds:[0CCBCh],8 */
    wd(pDS, 0xCCBC, (uint32_t)(rd(pDS, 0xCCBC) << 8));
L4A44:
    /* 4A44  shl     edx,8 */
    EDX = (uint32_t)(EDX << 8);
L4A48:
    /* 4A48  mov     eax,dword ptr ds:[0CCBCh] */
    EAX = rd(pDS, 0xCCBC);
L4A4C:
    /* 4A4C  mov     dword ptr ds:[0CCC0h],eax */
    wd(pDS, 0xCCC0, EAX);
L4A50:
    /* 4A50  mov     ax,word ptr ds:[0CF5Eh] */
    AX = rw(pDS, 0xCF5E);
L4A53:
    /* 4A53  xor     ax,word ptr ds:[0CF5Ah] */
    AX = (uint16_t)(AX ^ rw(pDS, 0xCF5A));
L4A57:
    /* 4A57  shr     ax,1 */
    AX = shr16(AX, 1);
L4A59:
    /* 4A59  jb      short L4A66 */
    if (CF) goto L4A66;
L4A5B:
    /* 4A5B  add     dword ptr ds:[0CCC0h],800000h */
    wd(pDS, 0xCCC0, (uint32_t)(rd(pDS, 0xCCC0) + 0x800000));
L4A64:
    /* 4A64  jmp     short L4A6F */
    goto L4A6F;
L4A66: /* L4A66 */
    /* 4A66  add     dword ptr ds:[0CCBCh],800000h */
    wd(pDS, 0xCCBC, (uint32_t)(rd(pDS, 0xCCBC) + 0x800000));
L4A6F: /* L4A6F */
    /* 4A6F  shr     cx,1 */
    CX = shr16(CX, 1);
L4A71:
    /* 4A71  pushf */
    push16(asm_flags());
L4A72:
    /* 4A72  je      short L4AD7 */
    if (ZF) goto L4AD7;
L4A74: /* L4A74 */
    /* 4A74  mov     ax,word ptr ds:[0CCB6h] */
    AX = rw(pDS, 0xCCB6);
L4A77:
    /* 4A77  mov     bx,word ptr ds:[0CCB2h] */
    BX = rw(pDS, 0xCCB2);
L4A7B:
    /* 4A7B  and     ax,word ptr ds:[0CCEAh] */
    AX = (uint16_t)(AX & rw(pDS, 0xCCEA));
L4A7F:
    /* 4A7F  and     bx,word ptr ds:[0CCE8h] */
    BX = (uint16_t)(BX & rw(pDS, 0xCCE8));
L4A83:
    /* 4A83  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L4A85:
    /* 4A85  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L4A88:
    /* 4A88  mov     bh,byte ptr ds:[0CCBFh] */
    BH = rb(pDS, 0xCCBF);
L4A8C:
    /* 4A8C  mov     al,byte ptr cs:lightabs[bx] */
    AL = rb(CODE004, BX + 0x6D3E);
L4A91:
    /* 4A91  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L4A94:
    /* 4A94  inc     di */
    DI = (uint16_t)(DI + 1);
L4A95:
    /* 4A95  add     dword ptr ds:[0CCB0h],esi */
    wd(pDS, 0xCCB0, (uint32_t)(rd(pDS, 0xCCB0) + ESI));
L4A9A:
    /* 4A9A  add     dword ptr ds:[0CCB4h],ebp */
    wd(pDS, 0xCCB4, (uint32_t)(rd(pDS, 0xCCB4) + EBP));
L4A9F:
    /* 4A9F  add     dword ptr ds:[0CCBCh],edx */
    wd(pDS, 0xCCBC, (uint32_t)(rd(pDS, 0xCCBC) + EDX));
L4AA4:
    /* 4AA4  mov     ax,word ptr ds:[0CCB6h] */
    AX = rw(pDS, 0xCCB6);
L4AA7:
    /* 4AA7  mov     bx,word ptr ds:[0CCB2h] */
    BX = rw(pDS, 0xCCB2);
L4AAB:
    /* 4AAB  and     ax,word ptr ds:[0CCEAh] */
    AX = (uint16_t)(AX & rw(pDS, 0xCCEA));
L4AAF:
    /* 4AAF  and     bx,word ptr ds:[0CCE8h] */
    BX = (uint16_t)(BX & rw(pDS, 0xCCE8));
L4AB3:
    /* 4AB3  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L4AB5:
    /* 4AB5  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L4AB8:
    /* 4AB8  mov     bh,byte ptr ds:[0CCC3h] */
    BH = rb(pDS, 0xCCC3);
L4ABC:
    /* 4ABC  mov     al,byte ptr cs:lightabs[bx] */
    AL = rb(CODE004, BX + 0x6D3E);
L4AC1:
    /* 4AC1  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L4AC4:
    /* 4AC4  inc     di */
    DI = (uint16_t)(DI + 1);
L4AC5:
    /* 4AC5  add     dword ptr ds:[0CCB0h],esi */
    wd(pDS, 0xCCB0, (uint32_t)(rd(pDS, 0xCCB0) + ESI));
L4ACA:
    /* 4ACA  add     dword ptr ds:[0CCB4h],ebp */
    wd(pDS, 0xCCB4, (uint32_t)(rd(pDS, 0xCCB4) + EBP));
L4ACF:
    /* 4ACF  add     dword ptr ds:[0CCC0h],edx */
    wd(pDS, 0xCCC0, (uint32_t)(rd(pDS, 0xCCC0) + EDX));
L4AD4:
    /* 4AD4  dec     cx */
    CX = dec16(CX);
L4AD5:
    /* 4AD5  jne     L4A74 */
    if (!ZF) goto L4A74;
L4AD7: /* L4AD7 */
    /* 4AD7  popf */
    asm_set_flags(pop16());
L4AD8:
    /* 4AD8  jae     short L4B05 */
    if (!CF) goto L4B05;
L4ADA:
    /* 4ADA  mov     ax,word ptr ds:[0CCB6h] */
    AX = rw(pDS, 0xCCB6);
L4ADD:
    /* 4ADD  mov     bx,word ptr ds:[0CCB2h] */
    BX = rw(pDS, 0xCCB2);
L4AE1:
    /* 4AE1  and     ax,word ptr ds:[0CCEAh] */
    AX = (uint16_t)(AX & rw(pDS, 0xCCEA));
L4AE5:
    /* 4AE5  and     bx,word ptr ds:[0CCE8h] */
    BX = (uint16_t)(BX & rw(pDS, 0xCCE8));
L4AE9:
    /* 4AE9  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L4AEB:
    /* 4AEB  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L4AEE:
    /* 4AEE  mov     bh,byte ptr ds:[0CCBFh] */
    BH = rb(pDS, 0xCCBF);
L4AF2:
    /* 4AF2  mov     al,byte ptr cs:lightabs[bx] */
    AL = rb(CODE004, BX + 0x6D3E);
L4AF7:
    /* 4AF7  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L4AFA:
    /* 4AFA  inc     di */
    DI = (uint16_t)(DI + 1);
L4AFB:
    /* 4AFB  add     dword ptr ds:[0CCB0h],esi */
    wd(pDS, 0xCCB0, (uint32_t)(rd(pDS, 0xCCB0) + ESI));
L4B00:
    /* 4B00  add     dword ptr ds:[0CCB4h],ebp */
    wd(pDS, 0xCCB4, add32(rd(pDS, 0xCCB4), EBP, 0));
L4B05: /* L4B05 */
    /* 4B05  mov     si,word ptr ds:[0CF6Eh] */
    SI = rw(pDS, 0xCF6E);
L4B09:
    /* 4B09  mov     ax,word ptr ds:[0CF72h] */
    AX = rw(pDS, 0xCF72);
L4B0C:
    /* 4B0C  mov     cx,word ptr ds:[0CCECh] */
    CX = rw(pDS, 0xCCEC);
L4B10:
    /* 4B10  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L4B12:
    /* 4B12  and     ax,word ptr ds:[0CCEAh] */
    AX = (uint16_t)(AX & rw(pDS, 0xCCEA));
L4B16:
    /* 4B16  and     si,word ptr ds:[0CCE8h] */
    SI = (uint16_t)(SI & rw(pDS, 0xCCE8));
L4B1A:
    /* 4B1A  add     si,ax */
    SI = add16(SI, AX, 0);
L4B1C:
    /* 4B1C  mov     bl,byte ptr gs:[si] */
    BL = rb(pGS, SI);
L4B1F:
    /* 4B1F  mov     bh,byte ptr ds:[0CF82h] */
    BH = rb(pDS, 0xCF82);
L4B23:
    /* 4B23  mov     al,byte ptr cs:lightabs[bx] */
    AL = rb(CODE004, BX + 0x6D3E);
L4B28:
    /* 4B28  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L4B2B: /* L4B2B */
    /* 4B2B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
