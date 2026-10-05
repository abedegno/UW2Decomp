/* vidmode_x.c: replaces src/gfx/VIDMODE.ASM (seg003_0272_21D4, 21D4..3205 of its
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

uint32_t asm_mod_VIDMODE(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x222B: goto L222B;
    case 0x2230: goto L2230;
    case 0x2232: goto L2232;
    case 0x2238: goto L2238;
    case 0x223A: goto L223A;
    case 0x2240: goto L2240;
    case 0x2242: goto L2242;
    case 0x2248: goto L2248;
    case 0x224D: goto L224D;
    case 0x249C: goto L249C;
    case 0x249F: goto L249F;
    case 0x24A1: goto L24A1;
    case 0x24A3: goto L24A3;
    case 0x24A5: goto L24A5;
    case 0x24A8: goto L24A8;
    case 0x24AA: goto L24AA;
    case 0x24AC: goto L24AC;
    case 0x24AF: goto L24AF;
    case 0x24B1: goto L24B1;
    case 0x24B3: goto L24B3;
    case 0x24B9: goto L24B9;
    case 0x24BB: goto L24BB;
    case 0x24BD: goto L24BD;
    case 0x24C0: goto L24C0;
    case 0x24C3: goto L24C3;
    case 0x24C9: goto L24C9;
    case 0x24CB: goto L24CB;
    case 0x24CE: goto L24CE;
    case 0x24D1: goto L24D1;
    case 0x24D3: goto L24D3;
    case 0x24D5: goto L24D5;
    case 0x24D7: goto L24D7;
    case 0x24DA: goto L24DA;
    case 0x24DD: goto L24DD;
    case 0x24E0: goto L24E0;
    case 0x24E2: goto L24E2;
    case 0x24E5: goto L24E5;
    case 0x24E8: goto L24E8;
    case 0x24EA: goto L24EA;
    case 0x24EC: goto L24EC;
    case 0x24EE: goto L24EE;
    case 0x24EF: goto L24EF;
    case 0x24F1: goto L24F1;
    case 0x24F3: goto L24F3;
    case 0x24F5: goto L24F5;
    case 0x24F8: goto L24F8;
    case 0x24FB: goto L24FB;
    case 0x24FE: goto L24FE;
    case 0x2500: goto L2500;
    case 0x2502: goto L2502;
    case 0x2504: goto L2504;
    case 0x2506: goto L2506;
    case 0x2509: goto L2509;
    case 0x250B: goto L250B;
    case 0x250E: goto L250E;
    case 0x2510: goto L2510;
    case 0x2513: goto L2513;
    case 0x2516: goto L2516;
    case 0x2519: goto L2519;
    case 0x251B: goto L251B;
    case 0x251D: goto L251D;
    case 0x251F: goto L251F;
    case 0x2520: goto L2520;
    case 0x2522: goto L2522;
    case 0x2524: goto L2524;
    case 0x2527: goto L2527;
    case 0x252A: goto L252A;
    case 0x252D: goto L252D;
    case 0x252F: goto L252F;
    case 0x2532: goto L2532;
    case 0x2535: goto L2535;
    case 0x2539: goto L2539;
    case 0x253C: goto L253C;
    case 0x253E: goto L253E;
    case 0x2540: goto L2540;
    case 0x2543: goto L2543;
    case 0x2546: goto L2546;
    case 0x2548: goto L2548;
    case 0x254A: goto L254A;
    case 0x254C: goto L254C;
    case 0x254F: goto L254F;
    case 0x2551: goto L2551;
    case 0x2554: goto L2554;
    case 0x2556: goto L2556;
    case 0x2558: goto L2558;
    case 0x255A: goto L255A;
    case 0x255C: goto L255C;
    case 0x255E: goto L255E;
    case 0x2560: goto L2560;
    case 0x2563: goto L2563;
    case 0x2566: goto L2566;
    case 0x2568: goto L2568;
    case 0x256B: goto L256B;
    case 0x2570: goto L2570;
    case 0x2573: goto L2573;
    case 0x2577: goto L2577;
    case 0x257A: goto L257A;
    case 0x257C: goto L257C;
    case 0x257E: goto L257E;
    case 0x2581: goto L2581;
    case 0x2583: goto L2583;
    case 0x2585: goto L2585;
    case 0x2587: goto L2587;
    case 0x2589: goto L2589;
    case 0x258C: goto L258C;
    case 0x258F: goto L258F;
    case 0x2591: goto L2591;
    case 0x2594: goto L2594;
    case 0x2599: goto L2599;
    case 0x259C: goto L259C;
    case 0x259E: goto L259E;
    case 0x25A1: goto L25A1;
    case 0x25A4: goto L25A4;
    case 0x25A7: goto L25A7;
    case 0x25AA: goto L25AA;
    case 0x25AF: goto L25AF;
    case 0x25B2: goto L25B2;
    case 0x25B6: goto L25B6;
    case 0x25BA: goto L25BA;
    case 0x25BD: goto L25BD;
    case 0x25BF: goto L25BF;
    case 0x25C1: goto L25C1;
    case 0x25C4: goto L25C4;
    case 0x25C8: goto L25C8;
    case 0x25CB: goto L25CB;
    case 0x25CD: goto L25CD;
    case 0x25CE: goto L25CE;
    case 0x25D1: goto L25D1;
    case 0x25D3: goto L25D3;
    case 0x25D6: goto L25D6;
    case 0x25D8: goto L25D8;
    case 0x25DA: goto L25DA;
    case 0x25DE: goto L25DE;
    case 0x25E0: goto L25E0;
    case 0x25E2: goto L25E2;
    case 0x25E4: goto L25E4;
    case 0x25E6: goto L25E6;
    case 0x25E8: goto L25E8;
    case 0x25EC: goto L25EC;
    case 0x25EF: goto L25EF;
    case 0x25F2: goto L25F2;
    case 0x25F6: goto L25F6;
    case 0x25F8: goto L25F8;
    case 0x25FA: goto L25FA;
    case 0x25FD: goto L25FD;
    case 0x2600: goto L2600;
    case 0x2606: goto L2606;
    case 0x260A: goto L260A;
    case 0x260D: goto L260D;
    case 0x260F: goto L260F;
    case 0x2613: goto L2613;
    case 0x2615: goto L2615;
    case 0x2616: goto L2616;
    case 0x2618: goto L2618;
    case 0x261B: goto L261B;
    case 0x261D: goto L261D;
    case 0x261E: goto L261E;
    case 0x2620: goto L2620;
    case 0x2624: goto L2624;
    case 0x2626: goto L2626;
    case 0x2628: goto L2628;
    case 0x262C: goto L262C;
    case 0x2630: goto L2630;
    case 0x2632: goto L2632;
    case 0x2634: goto L2634;
    case 0x2636: goto L2636;
    case 0x2637: goto L2637;
    case 0x263A: goto L263A;
    case 0x263C: goto L263C;
    case 0x263E: goto L263E;
    case 0x2640: goto L2640;
    case 0x2641: goto L2641;
    case 0x2644: goto L2644;
    case 0x2646: goto L2646;
    case 0x2648: goto L2648;
    case 0x264A: goto L264A;
    case 0x264E: goto L264E;
    case 0x2650: goto L2650;
    case 0x2654: goto L2654;
    case 0x2656: goto L2656;
    case 0x2658: goto L2658;
    case 0x265B: goto L265B;
    case 0x265E: goto L265E;
    case 0x2660: goto L2660;
    case 0x2663: goto L2663;
    case 0x2667: goto L2667;
    case 0x266B: goto L266B;
    case 0x266E: goto L266E;
    case 0x2671: goto L2671;
    case 0x2674: goto L2674;
    case 0x2677: goto L2677;
    case 0x2679: goto L2679;
    case 0x267C: goto L267C;
    case 0x267F: goto L267F;
    case 0x2681: goto L2681;
    case 0x2683: goto L2683;
    case 0x2686: goto L2686;
    case 0x2689: goto L2689;
    case 0x268B: goto L268B;
    case 0x268E: goto L268E;
    case 0x2692: goto L2692;
    case 0x2694: goto L2694;
    case 0x2698: goto L2698;
    case 0x269C: goto L269C;
    case 0x269F: goto L269F;
    case 0x26A2: goto L26A2;
    case 0x26A5: goto L26A5;
    case 0x26A8: goto L26A8;
    case 0x26AB: goto L26AB;
    case 0x26AE: goto L26AE;
    case 0x26B2: goto L26B2;
    case 0x26B5: goto L26B5;
    case 0x26B7: goto L26B7;
    case 0x26B9: goto L26B9;
    case 0x26BB: goto L26BB;
    case 0x26BC: goto L26BC;
    case 0x26BE: goto L26BE;
    case 0x26C2: goto L26C2;
    case 0x26C4: goto L26C4;
    case 0x26C6: goto L26C6;
    case 0x26C7: goto L26C7;
    case 0x26C9: goto L26C9;
    case 0x26CB: goto L26CB;
    case 0x26CC: goto L26CC;
    case 0x26CD: goto L26CD;
    case 0x26D1: goto L26D1;
    case 0x26D3: goto L26D3;
    case 0x26D5: goto L26D5;
    case 0x26D7: goto L26D7;
    case 0x26D9: goto L26D9;
    case 0x26DA: goto L26DA;
    case 0x26DC: goto L26DC;
    case 0x26E0: goto L26E0;
    case 0x26E2: goto L26E2;
    case 0x26E4: goto L26E4;
    case 0x26E5: goto L26E5;
    case 0x26E7: goto L26E7;
    case 0x26E9: goto L26E9;
    case 0x26EA: goto L26EA;
    case 0x26EB: goto L26EB;
    case 0x26EF: goto L26EF;
    case 0x26F1: goto L26F1;
    case 0x26F3: goto L26F3;
    case 0x26F5: goto L26F5;
    case 0x26F7: goto L26F7;
    case 0x26FB: goto L26FB;
    case 0x26FE: goto L26FE;
    case 0x2702: goto L2702;
    case 0x2705: goto L2705;
    case 0x2708: goto L2708;
    case 0x270B: goto L270B;
    case 0x270D: goto L270D;
    case 0x270E: goto L270E;
    case 0x2710: goto L2710;
    case 0x2714: goto L2714;
    case 0x2716: goto L2716;
    case 0x2718: goto L2718;
    case 0x2719: goto L2719;
    case 0x271B: goto L271B;
    case 0x271D: goto L271D;
    case 0x271E: goto L271E;
    case 0x271F: goto L271F;
    case 0x2723: goto L2723;
    case 0x2725: goto L2725;
    case 0x2727: goto L2727;
    case 0x2728: goto L2728;
    case 0x272A: goto L272A;
    case 0x272E: goto L272E;
    case 0x2730: goto L2730;
    case 0x2732: goto L2732;
    case 0x2733: goto L2733;
    case 0x2735: goto L2735;
    case 0x2737: goto L2737;
    case 0x2738: goto L2738;
    case 0x2739: goto L2739;
    case 0x273D: goto L273D;
    case 0x273F: goto L273F;
    case 0x2742: goto L2742;
    case 0x2745: goto L2745;
    case 0x2749: goto L2749;
    case 0x274D: goto L274D;
    case 0x2751: goto L2751;
    case 0x2754: goto L2754;
    case 0x2758: goto L2758;
    case 0x275B: goto L275B;
    case 0x275E: goto L275E;
    case 0x2761: goto L2761;
    case 0x2763: goto L2763;
    case 0x2767: goto L2767;
    case 0x2769: goto L2769;
    case 0x276A: goto L276A;
    case 0x276C: goto L276C;
    case 0x276E: goto L276E;
    case 0x2771: goto L2771;
    case 0x2774: goto L2774;
    case 0x2777: goto L2777;
    case 0x2778: goto L2778;
    case 0x277A: goto L277A;
    case 0x277D: goto L277D;
    case 0x2780: goto L2780;
    case 0x2783: goto L2783;
    case 0x2785: goto L2785;
    case 0x2787: goto L2787;
    case 0x2789: goto L2789;
    case 0x278B: goto L278B;
    case 0x278D: goto L278D;
    case 0x278F: goto L278F;
    case 0x2791: goto L2791;
    case 0x2793: goto L2793;
    case 0x2795: goto L2795;
    case 0x2798: goto L2798;
    case 0x279A: goto L279A;
    case 0x279B: goto L279B;
    case 0x279D: goto L279D;
    case 0x27A0: goto L27A0;
    case 0x27A2: goto L27A2;
    case 0x27A3: goto L27A3;
    case 0x27A7: goto L27A7;
    case 0x27AB: goto L27AB;
    case 0x27AE: goto L27AE;
    case 0x27B0: goto L27B0;
    case 0x27B2: goto L27B2;
    case 0x27B4: goto L27B4;
    case 0x27B6: goto L27B6;
    case 0x27B9: goto L27B9;
    case 0x27BB: goto L27BB;
    case 0x27BC: goto L27BC;
    case 0x27BE: goto L27BE;
    case 0x27C0: goto L27C0;
    case 0x27C3: goto L27C3;
    case 0x27C6: goto L27C6;
    case 0x27C9: goto L27C9;
    case 0x27CC: goto L27CC;
    case 0x27CE: goto L27CE;
    case 0x27D0: goto L27D0;
    case 0x27D3: goto L27D3;
    case 0x27D5: goto L27D5;
    case 0x27D7: goto L27D7;
    case 0x27D9: goto L27D9;
    case 0x27DB: goto L27DB;
    case 0x27DD: goto L27DD;
    case 0x27DF: goto L27DF;
    case 0x27E3: goto L27E3;
    case 0x27E6: goto L27E6;
    case 0x27EA: goto L27EA;
    case 0x27EC: goto L27EC;
    case 0x27EE: goto L27EE;
    case 0x27F0: goto L27F0;
    case 0x27F2: goto L27F2;
    case 0x27F3: goto L27F3;
    case 0x27F4: goto L27F4;
    case 0x27F6: goto L27F6;
    case 0x27F9: goto L27F9;
    case 0x27FC: goto L27FC;
    case 0x27FF: goto L27FF;
    case 0x2802: goto L2802;
    case 0x2804: goto L2804;
    case 0x2805: goto L2805;
    case 0x2806: goto L2806;
    case 0x2808: goto L2808;
    case 0x280B: goto L280B;
    case 0x280E: goto L280E;
    case 0x2811: goto L2811;
    case 0x2814: goto L2814;
    case 0x2816: goto L2816;
    case 0x2817: goto L2817;
    case 0x2819: goto L2819;
    case 0x281B: goto L281B;
    case 0x281F: goto L281F;
    case 0x2820: goto L2820;
    case 0x2824: goto L2824;
    case 0x2828: goto L2828;
    case 0x282A: goto L282A;
    case 0x282D: goto L282D;
    case 0x2830: goto L2830;
    case 0x2834: goto L2834;
    case 0x2838: goto L2838;
    case 0x283C: goto L283C;
    case 0x2914: goto L2914;
    case 0x2919: goto L2919;
    case 0x291B: goto L291B;
    case 0x291C: goto L291C;
    case 0x2922: goto L2922;
    case 0x2925: goto L2925;
    case 0x2928: goto L2928;
    case 0x292A: goto L292A;
    case 0x292D: goto L292D;
    case 0x292E: goto L292E;
    case 0x2931: goto L2931;
    case 0x2932: goto L2932;
    case 0x2934: goto L2934;
    case 0x2936: goto L2936;
    case 0x2938: goto L2938;
    case 0x293B: goto L293B;
    case 0x293C: goto L293C;
    case 0x293D: goto L293D;
    case 0x2940: goto L2940;
    case 0x2943: goto L2943;
    case 0x2944: goto L2944;
    case 0x2945: goto L2945;
    case 0x294B: goto L294B;
    case 0x294D: goto L294D;
    case 0x294F: goto L294F;
    case 0x2951: goto L2951;
    case 0x2953: goto L2953;
    case 0x2955: goto L2955;
    case 0x2956: goto L2956;
    case 0x2957: goto L2957;
    case 0x2958: goto L2958;
    case 0x2959: goto L2959;
    case 0x295A: goto L295A;
    case 0x295B: goto L295B;
    case 0x295C: goto L295C;
    case 0x295E: goto L295E;
    case 0x2B62: goto L2B62;
    case 0x2B66: goto L2B66;
    case 0x2B69: goto L2B69;
    case 0x2B6B: goto L2B6B;
    case 0x2B6C: goto L2B6C;
    case 0x2B6E: goto L2B6E;
    case 0x2B70: goto L2B70;
    case 0x2B72: goto L2B72;
    case 0x2B73: goto L2B73;
    case 0x2B74: goto L2B74;
    case 0x2B76: goto L2B76;
    case 0x2B78: goto L2B78;
    case 0x2B79: goto L2B79;
    case 0x2B7A: goto L2B7A;
    case 0x2B7D: goto L2B7D;
    case 0x2B7F: goto L2B7F;
    case 0x2B81: goto L2B81;
    case 0x2B83: goto L2B83;
    case 0x2B86: goto L2B86;
    case 0x2B88: goto L2B88;
    case 0x2B8B: goto L2B8B;
    case 0x2B8D: goto L2B8D;
    case 0x2B90: goto L2B90;
    case 0x2B92: goto L2B92;
    case 0x2B95: goto L2B95;
    case 0x2B97: goto L2B97;
    case 0x2B98: goto L2B98;
    case 0x2B99: goto L2B99;
    case 0x2B9A: goto L2B9A;
    case 0x2C9A: goto L2C9A;
    case 0x2C9D: goto L2C9D;
    case 0x2CA0: goto L2CA0;
    case 0x2CA1: goto L2CA1;
    case 0x2CA4: goto L2CA4;
    case 0x2CA6: goto L2CA6;
    case 0x2CA7: goto L2CA7;
    case 0x2CAB: goto L2CAB;
    case 0x2CAD: goto L2CAD;
    case 0x2CB1: goto L2CB1;
    case 0x2CB3: goto L2CB3;
    case 0x2CB4: goto L2CB4;
    case 0x2CB6: goto L2CB6;
    case 0x2CB9: goto L2CB9;
    case 0x2CBA: goto L2CBA;
    case 0x2CBB: goto L2CBB;
    case 0x2CBD: goto L2CBD;
    case 0x2CBF: goto L2CBF;
    case 0x2CC1: goto L2CC1;
    case 0x2CC2: goto L2CC2;
    case 0x2CC4: goto L2CC4;
    case 0x2CC5: goto L2CC5;
    case 0x2CC7: goto L2CC7;
    case 0x2CCB: goto L2CCB;
    case 0x2CCF: goto L2CCF;
    case 0x2CD1: goto L2CD1;
    case 0x2CD3: goto L2CD3;
    case 0x2CD5: goto L2CD5;
    case 0x2CD7: goto L2CD7;
    case 0x2CD9: goto L2CD9;
    case 0x2CDB: goto L2CDB;
    case 0x2CDD: goto L2CDD;
    case 0x2CDE: goto L2CDE;
    case 0x2CE2: goto L2CE2;
    case 0x2CE4: goto L2CE4;
    case 0x2CE7: goto L2CE7;
    case 0x2CE8: goto L2CE8;
    case 0x2CE9: goto L2CE9;
    case 0x2CEB: goto L2CEB;
    case 0x2CED: goto L2CED;
    case 0x2CEE: goto L2CEE;
    case 0x2CF0: goto L2CF0;
    case 0x2CF3: goto L2CF3;
    case 0x2CF4: goto L2CF4;
    case 0x2CF6: goto L2CF6;
    case 0x2CF8: goto L2CF8;
    case 0x2CF9: goto L2CF9;
    case 0x2CFB: goto L2CFB;
    case 0x2CFE: goto L2CFE;
    case 0x2CFF: goto L2CFF;
    case 0x2D01: goto L2D01;
    case 0x2D04: goto L2D04;
    case 0x2D07: goto L2D07;
    case 0x2D08: goto L2D08;
    case 0x2D0A: goto L2D0A;
    case 0x2D0B: goto L2D0B;
    case 0x2D0C: goto L2D0C;
    case 0x2D0D: goto L2D0D;
    case 0x2D0E: goto L2D0E;
    case 0x2D11: goto L2D11;
    case 0x2D12: goto L2D12;
    case 0x2D14: goto L2D14;
    case 0x2D15: goto L2D15;
    case 0x2D16: goto L2D16;
    case 0x2D19: goto L2D19;
    case 0x2D1C: goto L2D1C;
    case 0x2D20: goto L2D20;
    case 0x2D21: goto L2D21;
    case 0x2D25: goto L2D25;
    case 0x2D27: goto L2D27;
    case 0x2D29: goto L2D29;
    case 0x2D2B: goto L2D2B;
    case 0x2D2D: goto L2D2D;
    case 0x2D2F: goto L2D2F;
    case 0x2D31: goto L2D31;
    case 0x2D33: goto L2D33;
    case 0x2D35: goto L2D35;
    case 0x2D37: goto L2D37;
    case 0x2D39: goto L2D39;
    case 0x2D3D: goto L2D3D;
    case 0x2D3F: goto L2D3F;
    case 0x2D40: goto L2D40;
    case 0x2D42: goto L2D42;
    case 0x2D43: goto L2D43;
    case 0x2D44: goto L2D44;
    case 0x2D46: goto L2D46;
    case 0x2D48: goto L2D48;
    case 0x2D4A: goto L2D4A;
    case 0x2D4B: goto L2D4B;
    case 0x2D4D: goto L2D4D;
    case 0x2D4E: goto L2D4E;
    case 0x2D50: goto L2D50;
    case 0x2D54: goto L2D54;
    case 0x2D58: goto L2D58;
    case 0x2D5A: goto L2D5A;
    case 0x2D5C: goto L2D5C;
    case 0x2D5E: goto L2D5E;
    case 0x2D60: goto L2D60;
    case 0x2D62: goto L2D62;
    case 0x2D64: goto L2D64;
    case 0x2D66: goto L2D66;
    case 0x2D67: goto L2D67;
    case 0x2D6B: goto L2D6B;
    case 0x2D6D: goto L2D6D;
    case 0x2D6E: goto L2D6E;
    case 0x2D6F: goto L2D6F;
    case 0x2D71: goto L2D71;
    case 0x2D72: goto L2D72;
    case 0x2D74: goto L2D74;
    case 0x2D76: goto L2D76;
    case 0x2D78: goto L2D78;
    case 0x2D79: goto L2D79;
    case 0x2D7B: goto L2D7B;
    case 0x2D7C: goto L2D7C;
    case 0x2D7E: goto L2D7E;
    case 0x2D7F: goto L2D7F;
    case 0x2D80: goto L2D80;
    case 0x2E2E: goto L2E2E;
    case 0x2E31: goto L2E31;
    case 0x2E34: goto L2E34;
    case 0x2E35: goto L2E35;
    case 0x2E36: goto L2E36;
    case 0x2E37: goto L2E37;
    case 0x2E3A: goto L2E3A;
    case 0x2E3D: goto L2E3D;
    case 0x2E40: goto L2E40;
    case 0x2E41: goto L2E41;
    case 0x2E44: goto L2E44;
    case 0x2E46: goto L2E46;
    case 0x2E47: goto L2E47;
    case 0x2E49: goto L2E49;
    case 0x2E4D: goto L2E4D;
    case 0x2E4E: goto L2E4E;
    case 0x2E52: goto L2E52;
    case 0x2E54: goto L2E54;
    case 0x2E56: goto L2E56;
    case 0x2E58: goto L2E58;
    case 0x2E5B: goto L2E5B;
    case 0x2E5E: goto L2E5E;
    case 0x2E61: goto L2E61;
    case 0x2E63: goto L2E63;
    case 0x2E65: goto L2E65;
    case 0x2E67: goto L2E67;
    case 0x2E69: goto L2E69;
    case 0x2E6B: goto L2E6B;
    case 0x2E6C: goto L2E6C;
    case 0x2E70: goto L2E70;
    case 0x2E72: goto L2E72;
    case 0x2E74: goto L2E74;
    case 0x2E77: goto L2E77;
    case 0x2E79: goto L2E79;
    case 0x2E7A: goto L2E7A;
    case 0x2E7D: goto L2E7D;
    case 0x2E7E: goto L2E7E;
    case 0x2E81: goto L2E81;
    case 0x2E85: goto L2E85;
    case 0x2E89: goto L2E89;
    case 0x2E8C: goto L2E8C;
    case 0x2E8E: goto L2E8E;
    case 0x2E91: goto L2E91;
    case 0x2E94: goto L2E94;
    case 0x2E95: goto L2E95;
    case 0x2E96: goto L2E96;
    case 0x2E97: goto L2E97;
    case 0x2E9A: goto L2E9A;
    case 0x2E9D: goto L2E9D;
    case 0x2EA0: goto L2EA0;
    case 0x2EA1: goto L2EA1;
    case 0x2EA4: goto L2EA4;
    case 0x2EA6: goto L2EA6;
    case 0x2EA7: goto L2EA7;
    case 0x2EAB: goto L2EAB;
    case 0x2EAD: goto L2EAD;
    case 0x2EAF: goto L2EAF;
    case 0x2EB1: goto L2EB1;
    case 0x2EB3: goto L2EB3;
    case 0x2EB5: goto L2EB5;
    case 0x2EB7: goto L2EB7;
    case 0x2EB9: goto L2EB9;
    case 0x2EBB: goto L2EBB;
    case 0x2EBD: goto L2EBD;
    case 0x2EC1: goto L2EC1;
    case 0x2EC5: goto L2EC5;
    case 0x2EC7: goto L2EC7;
    case 0x2EC8: goto L2EC8;
    case 0x2ECB: goto L2ECB;
    case 0x2ECC: goto L2ECC;
    case 0x2ECE: goto L2ECE;
    case 0x2ED0: goto L2ED0;
    case 0x2ED2: goto L2ED2;
    case 0x2ED5: goto L2ED5;
    case 0x2ED8: goto L2ED8;
    case 0x2EDB: goto L2EDB;
    case 0x2EDF: goto L2EDF;
    case 0x2EE3: goto L2EE3;
    case 0x2EE5: goto L2EE5;
    case 0x2EE7: goto L2EE7;
    case 0x2EE9: goto L2EE9;
    case 0x2EEB: goto L2EEB;
    case 0x2EED: goto L2EED;
    case 0x2EEF: goto L2EEF;
    case 0x2EF1: goto L2EF1;
    case 0x2EF5: goto L2EF5;
    case 0x2EF7: goto L2EF7;
    case 0x2EFB: goto L2EFB;
    case 0x2EFC: goto L2EFC;
    case 0x2EFE: goto L2EFE;
    case 0x2EFF: goto L2EFF;
    case 0x2F01: goto L2F01;
    case 0x2F02: goto L2F02;
    case 0x2F05: goto L2F05;
    case 0x2F07: goto L2F07;
    case 0x2F08: goto L2F08;
    case 0x2F0A: goto L2F0A;
    case 0x2F0C: goto L2F0C;
    case 0x2F0F: goto L2F0F;
    case 0x2F12: goto L2F12;
    case 0x2F13: goto L2F13;
    case 0x2F14: goto L2F14;
    case 0x2F15: goto L2F15;
    case 0x2FD1: goto L2FD1;
    case 0x2FD2: goto L2FD2;
    case 0x2FD3: goto L2FD3;
    case 0x2FD6: goto L2FD6;
    case 0x2FD9: goto L2FD9;
    case 0x2FDB: goto L2FDB;
    case 0x2FDC: goto L2FDC;
    case 0x2FDF: goto L2FDF;
    case 0x2FE1: goto L2FE1;
    case 0x2FE2: goto L2FE2;
    case 0x2FE3: goto L2FE3;
    case 0x2FE5: goto L2FE5;
    case 0x2FE9: goto L2FE9;
    case 0x2FEA: goto L2FEA;
    case 0x2FEE: goto L2FEE;
    case 0x2FF1: goto L2FF1;
    case 0x2FF3: goto L2FF3;
    case 0x2FF5: goto L2FF5;
    case 0x2FF8: goto L2FF8;
    case 0x2FFB: goto L2FFB;
    case 0x2FFE: goto L2FFE;
    case 0x3000: goto L3000;
    case 0x3002: goto L3002;
    case 0x3004: goto L3004;
    case 0x3006: goto L3006;
    case 0x3008: goto L3008;
    case 0x3009: goto L3009;
    case 0x300E: goto L300E;
    case 0x3010: goto L3010;
    case 0x3012: goto L3012;
    case 0x3013: goto L3013;
    case 0x3015: goto L3015;
    case 0x3017: goto L3017;
    case 0x3019: goto L3019;
    case 0x301B: goto L301B;
    case 0x301C: goto L301C;
    case 0x301E: goto L301E;
    case 0x3020: goto L3020;
    case 0x3022: goto L3022;
    case 0x3024: goto L3024;
    case 0x3025: goto L3025;
    case 0x3027: goto L3027;
    case 0x3029: goto L3029;
    case 0x302B: goto L302B;
    case 0x302D: goto L302D;
    case 0x302E: goto L302E;
    case 0x3030: goto L3030;
    case 0x3032: goto L3032;
    case 0x3033: goto L3033;
    case 0x3034: goto L3034;
    case 0x3036: goto L3036;
    case 0x3037: goto L3037;
    case 0x3038: goto L3038;
    case 0x303B: goto L303B;
    case 0x303E: goto L303E;
    case 0x3040: goto L3040;
    case 0x3041: goto L3041;
    case 0x3044: goto L3044;
    case 0x3046: goto L3046;
    case 0x3047: goto L3047;
    case 0x3048: goto L3048;
    case 0x304A: goto L304A;
    case 0x304E: goto L304E;
    case 0x304F: goto L304F;
    case 0x3053: goto L3053;
    case 0x3056: goto L3056;
    case 0x3058: goto L3058;
    case 0x305A: goto L305A;
    case 0x305D: goto L305D;
    case 0x3060: goto L3060;
    case 0x3063: goto L3063;
    case 0x3065: goto L3065;
    case 0x3067: goto L3067;
    case 0x3069: goto L3069;
    case 0x306B: goto L306B;
    case 0x306D: goto L306D;
    case 0x306E: goto L306E;
    case 0x3073: goto L3073;
    case 0x3075: goto L3075;
    case 0x3077: goto L3077;
    case 0x3078: goto L3078;
    case 0x307A: goto L307A;
    case 0x307C: goto L307C;
    case 0x307E: goto L307E;
    case 0x307F: goto L307F;
    case 0x3080: goto L3080;
    case 0x3082: goto L3082;
    case 0x3084: goto L3084;
    case 0x3086: goto L3086;
    case 0x3087: goto L3087;
    case 0x3088: goto L3088;
    case 0x308A: goto L308A;
    case 0x308C: goto L308C;
    case 0x308E: goto L308E;
    case 0x308F: goto L308F;
    case 0x3090: goto L3090;
    case 0x3092: goto L3092;
    case 0x30CF: goto L30CF;
    case 0x30D1: goto L30D1;
    case 0x30D4: goto L30D4;
    case 0x30D6: goto L30D6;
    case 0x30D7: goto L30D7;
    case 0x30D9: goto L30D9;
    case 0x30DC: goto L30DC;
    case 0x30DE: goto L30DE;
    case 0x30DF: goto L30DF;
    case 0x30E2: goto L30E2;
    case 0x30E3: goto L30E3;
    case 0x30E7: goto L30E7;
    case 0x30E9: goto L30E9;
    case 0x30ED: goto L30ED;
    case 0x30EF: goto L30EF;
    case 0x30F3: goto L30F3;
    case 0x30F5: goto L30F5;
    case 0x30F9: goto L30F9;
    case 0x3121: goto L3121;
    case 0x3123: goto L3123;
    case 0x3125: goto L3125;
    case 0x3127: goto L3127;
    case 0x3128: goto L3128;
    case 0x312B: goto L312B;
    case 0x312E: goto L312E;
    case 0x312F: goto L312F;
    case 0x3133: goto L3133;
    case 0x3137: goto L3137;
    case 0x313A: goto L313A;
    case 0x313B: goto L313B;
    case 0x313D: goto L313D;
    case 0x313F: goto L313F;
    case 0x3141: goto L3141;
    case 0x3145: goto L3145;
    case 0x3149: goto L3149;
    case 0x314D: goto L314D;
    case 0x314E: goto L314E;
    case 0x314F: goto L314F;
    case 0x3153: goto L3153;
    case 0x3156: goto L3156;
    case 0x3157: goto L3157;
    case 0x3159: goto L3159;
    case 0x315B: goto L315B;
    case 0x315C: goto L315C;
    case 0x315F: goto L315F;
    case 0x3162: goto L3162;
    case 0x3163: goto L3163;
    case 0x3164: goto L3164;
    case 0x3166: goto L3166;
    case 0x3168: goto L3168;
    case 0x316A: goto L316A;
    case 0x316B: goto L316B;
    case 0x316F: goto L316F;
    case 0x3173: goto L3173;
    case 0x3176: goto L3176;
    case 0x3177: goto L3177;
    case 0x3179: goto L3179;
    case 0x317B: goto L317B;
    case 0x317D: goto L317D;
    case 0x3181: goto L3181;
    case 0x3185: goto L3185;
    case 0x3186: goto L3186;
    case 0x3189: goto L3189;
    case 0x318A: goto L318A;
    case 0x318E: goto L318E;
    case 0x318F: goto L318F;
    case 0x3191: goto L3191;
    case 0x3193: goto L3193;
    case 0x3194: goto L3194;
    case 0x31E5: goto L31E5;
    case 0x31E6: goto L31E6;
    default: asm_bad_entry("VIDMODE.ASM", entry);
    }
L222B: /* _seg003_0272_222B */
    /* 222B  cmp     byte ptr ds:[_Transparency],0 */
    sub8(rb(pDS, 0xDC5), 0x0, 0);
L2230:
    /* 2230  jne     L223A */
    if (!ZF) goto L223A;
L2232:
    /* 2232  mov     word ptr ds:[0DC2h],offset _seg003_0272_58C7 */
    ww(pDS, 0xDC2, 0x58C7);
L2238:
    /* 2238  jmp     short L2296 */
    return ASM_JMP(0x0085, 0x2296);
L223A: /* L223A */
    /* 223A  mov     word ptr ds:[0DC2h],offset _seg003_0272_58F8 */
    ww(pDS, 0xDC2, 0x58F8);
L2240:
    /* 2240  jmp     short L2296 */
    return ASM_JMP(0x0085, 0x2296);
L2242: /* _seg003_0272_2242 */
    /* 2242  mov     word ptr ds:[0DC2h],offset _seg003_0272_A7F */
    ww(pDS, 0xDC2, 0xA7F);
L2248:
    /* 2248  mov     byte ptr ds:[0DCAh],0 */
    wb(pDS, 0xDCA, 0x0);
L224D:
    /* 224D  jmp     L2373 */
    return ASM_JMP(0x0085, 0x2373);
L249C:
    /* 249C  call    _seg003_0272_34C2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x34C2), 0x249F)) != 0) return c;
L249F:
    /* 249F  mov     ax,cx */
    AX = CX;
L24A1:
    /* 24A1  shl     cx,1 */
    CX = (uint16_t)(CX << 1);
L24A3:
    /* 24A3  shl     cx,1 */
    CX = (uint16_t)(CX << 1);
L24A5:
    /* 24A5  mov     si,415Eh */
    SI = 0x415E;
L24A8:
    /* 24A8  mov     di,si */
    DI = SI;
L24AA:
    /* 24AA  add     di,cx */
    DI = (uint16_t)(DI + CX);
L24AC:
    /* 24AC  mov     cx,8 */
    CX = 0x8;
L24AF:
    /* 24AF  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L24B1:
    /* 24B1  mov     cx,ax */
    CX = AX;
L24B3:
    /* 24B3  mov     word ptr ds:[1B86h],2D42h */
    ww(pDS, 0x1B86, 0x2D42);
L24B9:
    /* 24B9  mov     ch,cl */
    CH = CL;
L24BB:
    /* 24BB  mov     cl,5 */
    CL = 0x5;
L24BD:
    /* 24BD  mov     si,4162h */
    SI = 0x4162;
L24C0:
    /* 24C0  mov     di,1422h */
    DI = 0x1422;
L24C3:
    /* 24C3  mov     word ptr ds:[19EAh],0 */
    ww(pDS, 0x19EA, 0x0);
L24C9: /* L24C9 */
    /* 24C9  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L24CB:
    /* 24CB  mov     bx,word ptr [si+2] */
    BX = rw(pDS, SI + 0x2);
L24CE:
    /* 24CE  cmp     bx,word ptr [si+6] */
    sub16(BX, rw(pDS, SI + 0x6), 0);
L24D1:
    /* 24D1  je      L254C */
    if (ZF) goto L254C;
L24D3:
    /* 24D3  jg      L2504 */
    if (!ZF && SF == OF) goto L2504;
L24D5:
    /* 24D5  shl     ax,cl */
    AX = shl16(AX, CL);
L24D7:
    /* 24D7  mov     word ptr [di+4],ax */
    ww(pDS, DI + 0x4, AX);
L24DA:
    /* 24DA  mov     word ptr [di+6],bx */
    ww(pDS, DI + 0x6, BX);
L24DD:
    /* 24DD  mov     dx,word ptr [si+4] */
    DX = rw(pDS, SI + 0x4);
L24E0:
    /* 24E0  shl     dx,cl */
    DX = (uint16_t)(DX << (CL & 31));
L24E2:
    /* 24E2  mov     word ptr [di+2],dx */
    ww(pDS, DI + 0x2, DX);
L24E5:
    /* 24E5  mov     bp,word ptr [si+6] */
    BP = rw(pDS, SI + 0x6);
L24E8:
    /* 24E8  mov     word ptr [di],bp */
    ww(pDS, DI, BP);
L24EA:
    /* 24EA  sub     ax,dx */
    AX = sub16(AX, DX, 0);
L24EC:
    /* 24EC  je      L24F8 */
    if (ZF) goto L24F8;
L24EE:
    /* 24EE  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L24EF:
    /* 24EF  sub     bx,bp */
    BX = (uint16_t)(BX - BP);
L24F1:
    /* 24F1  neg     bx */
    BX = (uint16_t)-BX;
L24F3:
    /* 24F3  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x0085, 0x24F3, 2)) != 0) return c;
L24F5:
    /* 24F5  sub     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, (uint16_t)(rw(pDS, DI + 0x2) - AX));
L24F8: /* L24F8 */
    /* 24F8  mov     word ptr [di+8],ax */
    ww(pDS, DI + 0x8, AX);
L24FB:
    /* 24FB  cmp     bp,word ptr [si+0Ah] */
    sub16(BP, rw(pDS, SI + 0xA), 0);
L24FE:
    /* 24FE  jg      L2532 */
    if (!ZF && SF == OF) goto L2532;
L2500:
    /* 2500  dec     word ptr [di] */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) - 1));
L2502:
    /* 2502  jmp     short L2532 */
    goto L2532;
L2504: /* L2504 */
    /* 2504  shl     ax,cl */
    AX = shl16(AX, CL);
L2506:
    /* 2506  mov     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, AX);
L2509:
    /* 2509  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L250B:
    /* 250B  mov     dx,word ptr [si+4] */
    DX = rw(pDS, SI + 0x4);
L250E:
    /* 250E  shl     dx,cl */
    DX = (uint16_t)(DX << (CL & 31));
L2510:
    /* 2510  mov     word ptr [di+4],dx */
    ww(pDS, DI + 0x4, DX);
L2513:
    /* 2513  mov     bp,word ptr [si+6] */
    BP = rw(pDS, SI + 0x6);
L2516:
    /* 2516  mov     word ptr [di+6],bp */
    ww(pDS, DI + 0x6, BP);
L2519:
    /* 2519  sub     dx,ax */
    DX = sub16(DX, AX, 0);
L251B:
    /* 251B  mov     ax,dx */
    AX = DX;
L251D:
    /* 251D  je      L2527 */
    if (ZF) goto L2527;
L251F:
    /* 251F  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L2520:
    /* 2520  sub     bx,bp */
    BX = (uint16_t)(BX - BP);
L2522:
    /* 2522  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x0085, 0x2522, 2)) != 0) return c;
L2524:
    /* 2524  sub     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, (uint16_t)(rw(pDS, DI + 0x2) - AX));
L2527: /* L2527 */
    /* 2527  mov     word ptr [di+8],ax */
    ww(pDS, DI + 0x8, AX);
L252A:
    /* 252A  cmp     bp,word ptr [si+0Ah] */
    sub16(BP, rw(pDS, SI + 0xA), 0);
L252D:
    /* 252D  jl      L2532 */
    if (SF != OF) goto L2532;
L252F:
    /* 252F  inc     word ptr [di+6] */
    ww(pDS, DI + 0x6, (uint16_t)(rw(pDS, DI + 0x6) + 1));
L2532: /* L2532 */
    /* 2532  add     di,0Ch */
    DI = (uint16_t)(DI + 0xC);
L2535:
    /* 2535  inc     word ptr ds:[19EAh] */
    ww(pDS, 0x19EA, (uint16_t)(rw(pDS, 0x19EA) + 1));
L2539:
    /* 2539  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L253C:
    /* 253C  dec     ch */
    CH = dec8(CH);
L253E:
    /* 253E  jg      L24C9 */
    if (!ZF && SF == OF) goto L24C9;
L2540:
    /* 2540  jmp     L25C4 */
    goto L25C4;
L2543: /* L2543 */
    /* 2543  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L2546:
    /* 2546  dec     ch */
    CH = dec8(CH);
L2548:
    /* 2548  jne     L254F */
    if (!ZF) goto L254F;
L254A:
    /* 254A  jmp     short L25C4 */
    goto L25C4;
L254C: /* L254C */
    /* 254C  mov     bp,word ptr [si-2] */
    BP = rw(pDS, SI + 0xFFFE);
L254F: /* L254F */
    /* 254F  mov     dx,bx */
    DX = BX;
L2551:
    /* 2551  sub     dx,word ptr [si+0Ah] */
    DX = sub16(DX, rw(pDS, SI + 0xA), 0);
L2554:
    /* 2554  je      L2543 */
    if (ZF) goto L2543;
L2556:
    /* 2556  sub     bp,bx */
    BP = (uint16_t)(BP - BX);
L2558:
    /* 2558  neg     bp */
    BP = (uint16_t)-BP;
L255A:
    /* 255A  xor     bp,dx */
    BP = logic16((uint16_t)(BP ^ DX));
L255C:
    /* 255C  js      L2583 */
    if (SF) goto L2583;
L255E:
    /* 255E  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2560:
    /* 2560  mov     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, AX);
L2563:
    /* 2563  mov     word ptr [di+4],ax */
    ww(pDS, DI + 0x4, AX);
L2566:
    /* 2566  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2568:
    /* 2568  mov     word ptr [di+6],bx */
    ww(pDS, DI + 0x6, BX);
L256B:
    /* 256B  mov     word ptr [di+8],0 */
    ww(pDS, DI + 0x8, 0x0);
L2570:
    /* 2570  add     di,0Ch */
    DI = (uint16_t)(DI + 0xC);
L2573:
    /* 2573  inc     word ptr ds:[19EAh] */
    ww(pDS, 0x19EA, (uint16_t)(rw(pDS, 0x19EA) + 1));
L2577:
    /* 2577  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L257A:
    /* 257A  dec     ch */
    CH = dec8(CH);
L257C:
    /* 257C  jle     L2581 */
    if (ZF || SF != OF) goto L2581;
L257E:
    /* 257E  jmp     L24C9 */
    goto L24C9;
L2581: /* L2581 */
    /* 2581  jmp     short L25C4 */
    goto L25C4;
L2583: /* L2583 */
    /* 2583  or      dx,dx */
    DX = logic16((uint16_t)(DX | DX));
L2585:
    /* 2585  js      L25BA */
    if (SF) goto L25BA;
L2587:
    /* 2587  shl     ax,cl */
    AX = shl16(AX, CL);
L2589:
    /* 2589  mov     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, AX);
L258C:
    /* 258C  mov     word ptr [di+4],ax */
    ww(pDS, DI + 0x4, AX);
L258F:
    /* 258F  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2591:
    /* 2591  mov     word ptr [di+6],bx */
    ww(pDS, DI + 0x6, BX);
L2594:
    /* 2594  mov     word ptr [di+8],0 */
    ww(pDS, DI + 0x8, 0x0);
L2599:
    /* 2599  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L259C:
    /* 259C  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L259E:
    /* 259E  mov     word ptr [di+0Eh],ax */
    ww(pDS, DI + 0xE, AX);
L25A1:
    /* 25A1  mov     word ptr [di+10h],ax */
    ww(pDS, DI + 0x10, AX);
L25A4:
    /* 25A4  mov     word ptr [di+0Ch],bx */
    ww(pDS, DI + 0xC, BX);
L25A7:
    /* 25A7  mov     word ptr [di+12h],bx */
    ww(pDS, DI + 0x12, BX);
L25AA:
    /* 25AA  mov     word ptr [di+14h],0 */
    ww(pDS, DI + 0x14, 0x0);
L25AF:
    /* 25AF  add     di,18h */
    DI = (uint16_t)(DI + 0x18);
L25B2:
    /* 25B2  inc     word ptr ds:[19EAh] */
    ww(pDS, 0x19EA, (uint16_t)(rw(pDS, 0x19EA) + 1));
L25B6:
    /* 25B6  inc     word ptr ds:[19EAh] */
    ww(pDS, 0x19EA, (uint16_t)(rw(pDS, 0x19EA) + 1));
L25BA: /* L25BA */
    /* 25BA  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L25BD:
    /* 25BD  dec     ch */
    CH = dec8(CH);
L25BF:
    /* 25BF  jle     L25C4 */
    if (ZF || SF != OF) goto L25C4;
L25C1:
    /* 25C1  jmp     L24C9 */
    goto L24C9;
L25C4: /* L25C4 */
    /* 25C4  mov     cx,word ptr ds:[19EAh] */
    CX = rw(pDS, 0x19EA);
L25C8:
    /* 25C8  cmp     cx,2 */
    sub16(CX, 0x2, 0);
L25CB:
    /* 25CB  jge     L25CE */
    if (SF == OF) goto L25CE;
L25CD:
    /* 25CD  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L25CE: /* L25CE */
    /* 25CE  mov     si,1422h */
    SI = 0x1422;
L25D1:
    /* 25D1  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L25D3:
    /* 25D3  mov     bp,0Ch */
    BP = 0xC;
L25D6:
    /* 25D6  mov     dl,0FFh */
    DL = 0xFF;
L25D8: /* L25D8 */
    /* 25D8  mov     bx,word ptr [si] */
    BX = rw(pDS, SI);
L25DA:
    /* 25DA  mov     byte ptr [bx+1B8Ah],dl */
    wb(pDS, BX + 0x1B8A, DL);
L25DE:
    /* 25DE  cmp     ax,bx */
    sub16(AX, BX, 0);
L25E0:
    /* 25E0  jg      L25E4 */
    if (!ZF && SF == OF) goto L25E4;
L25E2:
    /* 25E2  mov     ax,bx */
    AX = BX;
L25E4: /* L25E4 */
    /* 25E4  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L25E6:
    /* 25E6  mov     di,si */
    DI = SI;
L25E8:
    /* 25E8  xchg    di,word ptr [bx+19F0h] */
    { uint16_t t_ = rw(pDS, BX + 0x19F0);
    ww(pDS, BX + 0x19F0, DI);
    DI = t_; }
L25EC:
    /* 25EC  mov     word ptr [si+0Ah],di */
    ww(pDS, SI + 0xA, DI);
L25EF:
    /* 25EF  mov     bx,word ptr [si+6] */
    BX = rw(pDS, SI + 0x6);
L25F2:
    /* 25F2  mov     byte ptr [bx+1B89h],dl */
    wb(pDS, BX + 0x1B89, DL);
L25F6:
    /* 25F6  add     si,bp */
    SI = (uint16_t)(SI + BP);
L25F8:
    /* 25F8  loop    L25D8 */
    if (--CX) goto L25D8;
L25FA:
    /* 25FA  mov     word ptr ds:[1420h],ax */
    ww(pDS, 0x1420, AX);
L25FD:
    /* 25FD  mov     word ptr ds:[1B84h],ax */
    ww(pDS, 0x1B84, AX);
L2600:
    /* 2600  mov     word ptr ds:[19ECh],0 */
    ww(pDS, 0x19EC, 0x0);
L2606: /* L2606 */
    /* 2606  mov     bx,word ptr ds:[1B84h] */
    BX = rw(pDS, 0x1B84);
L260A:
    /* 260A  mov     di,19C2h */
    DI = 0x19C2;
L260D:
    /* 260D  mov     si,di */
    SI = DI;
L260F:
    /* 260F  mov     cx,word ptr ds:[19ECh] */
    CX = rw(pDS, 0x19EC);
L2613:
    /* 2613  jcxz    L2620 */
    if (!CX) goto L2620;
L2615: /* L2615 */
    /* 2615  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2616:
    /* 2616  mov     bp,ax */
    BP = AX;
L2618:
    /* 2618  cmp     bx,word ptr [bp+6] */
    sub16(BX, rw(pSS, BP + 0x6), 0);
L261B:
    /* 261B  jl      L261E */
    if (SF != OF) goto L261E;
L261D:
    /* 261D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L261E: /* L261E */
    /* 261E  loop    L2615 */
    if (--CX) goto L2615;
L2620: /* L2620 */
    /* 2620  mov     byte ptr [bx+1B8Ah],cl */
    wb(pDS, BX + 0x1B8A, CL);
L2624:
    /* 2624  mov     si,bx */
    SI = BX;
L2626:
    /* 2626  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L2628:
    /* 2628  mov     ax,word ptr [si+19F0h] */
    AX = rw(pDS, SI + 0x19F0);
L262C:
    /* 262C  mov     word ptr [si+19F0h],cx */
    ww(pDS, SI + 0x19F0, CX);
L2630:
    /* 2630  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L2632:
    /* 2632  je      L2648 */
    if (ZF) goto L2648;
L2634: /* L2634 */
    /* 2634  mov     si,ax */
    SI = AX;
L2636:
    /* 2636  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2637:
    /* 2637  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L263A:
    /* 263A  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L263C:
    /* 263C  je      L2648 */
    if (ZF) goto L2648;
L263E:
    /* 263E  mov     si,ax */
    SI = AX;
L2640:
    /* 2640  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2641:
    /* 2641  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L2644:
    /* 2644  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L2646:
    /* 2646  jne     L2634 */
    if (!ZF) goto L2634;
L2648: /* L2648 */
    /* 2648  mov     dx,di */
    DX = DI;
L264A:
    /* 264A  sub     dx,19C2h */
    DX = (uint16_t)(DX - 0x19C2);
L264E:
    /* 264E  shr     dx,1 */
    DX = (uint16_t)(DX >> 1);
L2650:
    /* 2650  mov     word ptr ds:[19ECh],dx */
    ww(pDS, 0x19EC, DX);
L2654:
    /* 2654  or      dx,dx */
    DX = logic16((uint16_t)(DX | DX));
L2656:
    /* 2656  jne     L265B */
    if (!ZF) goto L265B;
L2658:
    /* 2658  jmp     L282D */
    goto L282D;
L265B: /* L265B */
    /* 265B  cmp     dl,2 */
    sub8(DL, 0x2, 0);
L265E:
    /* 265E  je      L2663 */
    if (ZF) goto L2663;
L2660:
    /* 2660  jmp     L275E */
    goto L275E;
L2663: /* L2663 */
    /* 2663  mov     bx,word ptr ds:[19C2h] */
    BX = rw(pDS, 0x19C2);
L2667:
    /* 2667  mov     bp,word ptr ds:[19C4h] */
    BP = rw(pDS, 0x19C4);
L266B:
    /* 266B  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L266E:
    /* 266E  add     ax,word ptr [bx+8] */
    AX = (uint16_t)(AX + rw(pDS, BX + 0x8));
L2671:
    /* 2671  sub     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX - rw(pSS, BP + 0x8));
L2674:
    /* 2674  cmp     ax,word ptr [bp+2] */
    sub16(AX, rw(pSS, BP + 0x2), 0);
L2677:
    /* 2677  jne     L267F */
    if (!ZF) goto L267F;
L2679:
    /* 2679  mov     ax,word ptr [bx+4] */
    AX = rw(pDS, BX + 0x4);
L267C:
    /* 267C  cmp     ax,word ptr [bp+4] */
    sub16(AX, rw(pSS, BP + 0x4), 0);
L267F: /* L267F */
    /* 267F  jl      L2683 */
    if (SF != OF) goto L2683;
L2681:
    /* 2681  xchg    bx,bp */
    { uint16_t t_ = BP;
    BP = BX;
    BX = t_; }
L2683: /* L2683 */
    /* 2683  mov     ax,word ptr [bx+4] */
    AX = rw(pDS, BX + 0x4);
L2686:
    /* 2686  cmp     ax,word ptr [bp+4] */
    sub16(AX, rw(pSS, BP + 0x4), 0);
L2689:
    /* 2689  jle     L268E */
    if (ZF || SF != OF) goto L268E;
L268B:
    /* 268B  jmp     L27AE */
    goto L27AE;
L268E: /* L268E */
    /* 268E  mov     di,word ptr ds:[1B86h] */
    DI = rw(pDS, 0x1B86);
L2692:
    /* 2692  mov     cl,5 */
    CL = 0x5;
L2694:
    /* 2694  mov     word ptr ds:[19C2h],bx */
    ww(pDS, 0x19C2, BX);
L2698:
    /* 2698  mov     word ptr ds:[19C4h],bp */
    ww(pDS, 0x19C4, BP);
L269C:
    /* 269C  mov     dx,word ptr [bx+2] */
    DX = rw(pDS, BX + 0x2);
L269F:
    /* 269F  mov     si,word ptr [bx+8] */
    SI = rw(pDS, BX + 0x8);
L26A2:
    /* 26A2  mov     ax,word ptr [bp+8] */
    AX = rw(pSS, BP + 0x8);
L26A5:
    /* 26A5  mov     bp,word ptr [bp+2] */
    BP = rw(pSS, BP + 0x2);
L26A8:
    /* 26A8  sub     dx,10h */
    DX = (uint16_t)(DX - 0x10);
L26AB:
    /* 26AB  add     bp,10h */
    BP = (uint16_t)(BP + 0x10);
L26AE:
    /* 26AE  mov     bx,word ptr ds:[1B84h] */
    BX = rw(pDS, 0x1B84);
L26B2:
    /* 26B2  mov     word ptr ds:[141Eh],ax */
    ww(pDS, 0x141E, AX);
L26B5:
    /* 26B5  cmp     si,ax */
    sub16(SI, AX, 0);
L26B7:
    /* 26B7  jle     L270B */
    if (ZF || SF != OF) goto L270B;
L26B9: /* L26B9 */
    /* 26B9  mov     ax,bx */
    AX = BX;
L26BB:
    /* 26BB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L26BC:
    /* 26BC  add     dx,si */
    DX = (uint16_t)(DX + SI);
L26BE:
    /* 26BE  add     bp,word ptr ds:[141Eh] */
    BP = add16(BP, rw(pDS, 0x141E), 0);
L26C2:
    /* 26C2  mov     ax,dx */
    AX = DX;
L26C4:
    /* 26C4  sar     ax,cl */
    AX = sar16(AX, CL);
L26C6:
    /* 26C6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L26C7:
    /* 26C7  mov     ax,bp */
    AX = BP;
L26C9:
    /* 26C9  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L26CB:
    /* 26CB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L26CC:
    /* 26CC  dec     bx */
    BX = (uint16_t)(BX - 1);
L26CD:
    /* 26CD  test    byte ptr [bx+1B8Ah],cl */
    logic8((uint8_t)(rb(pDS, BX + 0x1B8A) & CL));
L26D1:
    /* 26D1  jne     L273F */
    if (!ZF) goto L273F;
L26D3:
    /* 26D3  cmp     dx,bp */
    sub16(DX, BP, 0);
L26D5:
    /* 26D5  jg      L26F5 */
    if (!ZF && SF == OF) goto L26F5;
L26D7:
    /* 26D7  mov     ax,bx */
    AX = BX;
L26D9:
    /* 26D9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L26DA:
    /* 26DA  add     dx,si */
    DX = (uint16_t)(DX + SI);
L26DC:
    /* 26DC  add     bp,word ptr ds:[141Eh] */
    BP = add16(BP, rw(pDS, 0x141E), 0);
L26E0:
    /* 26E0  mov     ax,dx */
    AX = DX;
L26E2:
    /* 26E2  sar     ax,cl */
    AX = sar16(AX, CL);
L26E4:
    /* 26E4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L26E5:
    /* 26E5  mov     ax,bp */
    AX = BP;
L26E7:
    /* 26E7  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L26E9:
    /* 26E9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L26EA:
    /* 26EA  dec     bx */
    BX = (uint16_t)(BX - 1);
L26EB:
    /* 26EB  test    byte ptr [bx+1B8Ah],cl */
    logic8((uint8_t)(rb(pDS, BX + 0x1B8A) & CL));
L26EF:
    /* 26EF  jne     L273F */
    if (!ZF) goto L273F;
L26F1:
    /* 26F1  cmp     dx,bp */
    sub16(DX, BP, 0);
L26F3:
    /* 26F3  jle     L26B9 */
    if (ZF || SF != OF) goto L26B9;
L26F5: /* L26F5 */
    /* 26F5  xchg    dx,bp */
    { uint16_t t_ = BP;
    BP = DX;
    DX = t_; }
L26F7:
    /* 26F7  xchg    si,word ptr ds:[141Eh] */
    { uint16_t t_ = rw(pDS, 0x141E);
    ww(pDS, 0x141E, SI);
    SI = t_; }
L26FB:
    /* 26FB  mov     ax,word ptr ds:[19C2h] */
    AX = rw(pDS, 0x19C2);
L26FE:
    /* 26FE  xchg    ax,word ptr ds:[19C4h] */
    { uint16_t t_ = rw(pDS, 0x19C4);
    ww(pDS, 0x19C4, AX);
    AX = t_; }
L2702:
    /* 2702  mov     word ptr ds:[19C2h],ax */
    ww(pDS, 0x19C2, AX);
L2705:
    /* 2705  sub     dx,20h */
    DX = (uint16_t)(DX - 0x20);
L2708:
    /* 2708  add     bp,20h */
    BP = (uint16_t)(BP + 0x20);
L270B: /* L270B */
    /* 270B  mov     ax,bx */
    AX = BX;
L270D:
    /* 270D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L270E:
    /* 270E  add     dx,si */
    DX = (uint16_t)(DX + SI);
L2710:
    /* 2710  add     bp,word ptr ds:[141Eh] */
    BP = add16(BP, rw(pDS, 0x141E), 0);
L2714:
    /* 2714  mov     ax,dx */
    AX = DX;
L2716:
    /* 2716  sar     ax,cl */
    AX = sar16(AX, CL);
L2718:
    /* 2718  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2719:
    /* 2719  mov     ax,bp */
    AX = BP;
L271B:
    /* 271B  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L271D:
    /* 271D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L271E:
    /* 271E  dec     bx */
    BX = (uint16_t)(BX - 1);
L271F:
    /* 271F  test    byte ptr [bx+1B8Ah],cl */
    logic8((uint8_t)(rb(pDS, BX + 0x1B8A) & CL));
L2723:
    /* 2723  jne     L273F */
    if (!ZF) goto L273F;
L2725:
    /* 2725  mov     ax,bx */
    AX = BX;
L2727:
    /* 2727  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2728:
    /* 2728  add     dx,si */
    DX = (uint16_t)(DX + SI);
L272A:
    /* 272A  add     bp,word ptr ds:[141Eh] */
    BP = add16(BP, rw(pDS, 0x141E), 0);
L272E:
    /* 272E  mov     ax,dx */
    AX = DX;
L2730:
    /* 2730  sar     ax,cl */
    AX = sar16(AX, CL);
L2732:
    /* 2732  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2733:
    /* 2733  mov     ax,bp */
    AX = BP;
L2735:
    /* 2735  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2737:
    /* 2737  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2738:
    /* 2738  dec     bx */
    BX = (uint16_t)(BX - 1);
L2739:
    /* 2739  test    byte ptr [bx+1B8Ah],cl */
    logic8((uint8_t)(rb(pDS, BX + 0x1B8A) & CL));
L273D:
    /* 273D  je      L270B */
    if (ZF) goto L270B;
L273F: /* L273F */
    /* 273F  add     dx,10h */
    DX = (uint16_t)(DX + 0x10);
L2742:
    /* 2742  sub     bp,10h */
    BP = (uint16_t)(BP - 0x10);
L2745:
    /* 2745  mov     word ptr ds:[1B86h],di */
    ww(pDS, 0x1B86, DI);
L2749:
    /* 2749  mov     word ptr ds:[1B84h],bx */
    ww(pDS, 0x1B84, BX);
L274D:
    /* 274D  mov     si,word ptr ds:[19C2h] */
    SI = rw(pDS, 0x19C2);
L2751:
    /* 2751  mov     word ptr [si+2],dx */
    ww(pDS, SI + 0x2, DX);
L2754:
    /* 2754  mov     si,word ptr ds:[19C4h] */
    SI = rw(pDS, 0x19C4);
L2758:
    /* 2758  mov     word ptr [si+2],bp */
    ww(pDS, SI + 0x2, BP);
L275B:
    /* 275B  jmp     L2606 */
    goto L2606;
L275E: /* L275E */
    /* 275E  test    dl,1 */
    logic8((uint8_t)(DL & 0x1));
L2761:
    /* 2761  je      L27AE */
    if (ZF) goto L27AE;
L2763:
    /* 2763  mov     di,word ptr ds:[1B86h] */
    DI = rw(pDS, 0x1B86);
L2767:
    /* 2767  mov     ax,bx */
    AX = BX;
L2769:
    /* 2769  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L276A:
    /* 276A  mov     cl,dl */
    CL = DL;
L276C:
    /* 276C  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L276E:
    /* 276E  mov     dx,8000h */
    DX = 0x8000;
L2771:
    /* 2771  mov     bp,7FFFh */
    BP = 0x7FFF;
L2774:
    /* 2774  mov     si,19C2h */
    SI = 0x19C2;
L2777: /* L2777 */
    /* 2777  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2778:
    /* 2778  mov     bx,ax */
    BX = AX;
L277A:
    /* 277A  mov     ax,word ptr [bx+8] */
    AX = rw(pDS, BX + 0x8);
L277D:
    /* 277D  add     ax,word ptr [bx+2] */
    AX = (uint16_t)(AX + rw(pDS, BX + 0x2));
L2780:
    /* 2780  mov     word ptr [bx+2],ax */
    ww(pDS, BX + 0x2, AX);
L2783:
    /* 2783  cmp     dx,ax */
    sub16(DX, AX, 0);
L2785:
    /* 2785  jg      L2789 */
    if (!ZF && SF == OF) goto L2789;
L2787:
    /* 2787  mov     dx,ax */
    DX = AX;
L2789: /* L2789 */
    /* 2789  cmp     bp,ax */
    sub16(BP, AX, 0);
L278B:
    /* 278B  jl      L278F */
    if (SF != OF) goto L278F;
L278D:
    /* 278D  mov     bp,ax */
    BP = AX;
L278F: /* L278F */
    /* 278F  loop    L2777 */
    if (--CX) goto L2777;
L2791:
    /* 2791  mov     cl,5 */
    CL = 0x5;
L2793:
    /* 2793  mov     ax,bp */
    AX = BP;
L2795:
    /* 2795  sub     ax,10h */
    AX = sub16(AX, 0x10, 0);
L2798:
    /* 2798  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L279A:
    /* 279A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L279B:
    /* 279B  mov     ax,dx */
    AX = DX;
L279D:
    /* 279D  add     ax,10h */
    AX = add16(AX, 0x10, 0);
L27A0:
    /* 27A0  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L27A2:
    /* 27A2  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L27A3:
    /* 27A3  mov     word ptr ds:[1B86h],di */
    ww(pDS, 0x1B86, DI);
L27A7:
    /* 27A7  dec     word ptr ds:[1B84h] */
    ww(pDS, 0x1B84, (uint16_t)(rw(pDS, 0x1B84) - 1));
L27AB:
    /* 27AB  jmp     L2606 */
    goto L2606;
L27AE: /* L27AE */
    /* 27AE  mov     dh,dl */
    DH = DL;
L27B0:
    /* 27B0  dec     dh */
    DH = (uint8_t)(DH - 1);
L27B2:
    /* 27B2  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L27B4: /* L27B4 */
    /* 27B4  mov     cl,dh */
    CL = DH;
L27B6:
    /* 27B6  mov     si,19C2h */
    SI = 0x19C2;
L27B9:
    /* 27B9  xor     bl,bl */
    BL = (uint8_t)(BL ^ BL);
L27BB: /* L27BB */
    /* 27BB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L27BC:
    /* 27BC  mov     di,ax */
    DI = AX;
L27BE:
    /* 27BE  mov     bp,word ptr [si] */
    BP = rw(pDS, SI);
L27C0:
    /* 27C0  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L27C3:
    /* 27C3  add     ax,word ptr [di+8] */
    AX = (uint16_t)(AX + rw(pDS, DI + 0x8));
L27C6:
    /* 27C6  sub     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX - rw(pSS, BP + 0x8));
L27C9:
    /* 27C9  cmp     ax,word ptr [bp+2] */
    sub16(AX, rw(pSS, BP + 0x2), 0);
L27CC:
    /* 27CC  jl      L27D5 */
    if (SF != OF) goto L27D5;
L27CE:
    /* 27CE  inc     bl */
    BL = (uint8_t)(BL + 1);
L27D0:
    /* 27D0  mov     word ptr [si-2],bp */
    ww(pDS, SI + 0xFFFE, BP);
L27D3:
    /* 27D3  mov     word ptr [si],di */
    ww(pDS, SI, DI);
L27D5: /* L27D5 */
    /* 27D5  loop    L27BB */
    if (--CX) goto L27BB;
L27D7:
    /* 27D7  or      bl,bl */
    BL = logic8((uint8_t)(BL | BL));
L27D9:
    /* 27D9  je      L27DF */
    if (ZF) goto L27DF;
L27DB:
    /* 27DB  dec     dh */
    DH = dec8(DH);
L27DD:
    /* 27DD  jne     L27B4 */
    if (!ZF) goto L27B4;
L27DF: /* L27DF */
    /* 27DF  mov     di,word ptr ds:[1B86h] */
    DI = rw(pDS, 0x1B86);
L27E3:
    /* 27E3  mov     si,19C2h */
    SI = 0x19C2;
L27E6:
    /* 27E6  mov     bp,word ptr ds:[1B84h] */
    BP = rw(pDS, 0x1B84);
L27EA:
    /* 27EA  mov     dh,dl */
    DH = DL;
L27EC:
    /* 27EC  shr     dh,1 */
    DH = (uint8_t)(DH >> 1);
L27EE:
    /* 27EE  mov     cl,5 */
    CL = 0x5;
L27F0: /* L27F0 */
    /* 27F0  mov     ax,bp */
    AX = BP;
L27F2:
    /* 27F2  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L27F3:
    /* 27F3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L27F4:
    /* 27F4  mov     bx,ax */
    BX = AX;
L27F6:
    /* 27F6  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L27F9:
    /* 27F9  add     ax,word ptr [bx+8] */
    AX = (uint16_t)(AX + rw(pDS, BX + 0x8));
L27FC:
    /* 27FC  mov     word ptr [bx+2],ax */
    ww(pDS, BX + 0x2, AX);
L27FF:
    /* 27FF  sub     ax,10h */
    AX = sub16(AX, 0x10, 0);
L2802:
    /* 2802  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2804:
    /* 2804  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2805:
    /* 2805  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2806:
    /* 2806  mov     bx,ax */
    BX = AX;
L2808:
    /* 2808  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L280B:
    /* 280B  add     ax,word ptr [bx+8] */
    AX = (uint16_t)(AX + rw(pDS, BX + 0x8));
L280E:
    /* 280E  mov     word ptr [bx+2],ax */
    ww(pDS, BX + 0x2, AX);
L2811:
    /* 2811  add     ax,10h */
    AX = add16(AX, 0x10, 0);
L2814:
    /* 2814  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2816:
    /* 2816  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2817:
    /* 2817  dec     dh */
    DH = dec8(DH);
L2819:
    /* 2819  jne     L27F0 */
    if (!ZF) goto L27F0;
L281B:
    /* 281B  mov     word ptr ds:[1B86h],di */
    ww(pDS, 0x1B86, DI);
L281F:
    /* 281F  dec     bp */
    BP = (uint16_t)(BP - 1);
L2820:
    /* 2820  mov     word ptr ds:[1B84h],bp */
    ww(pDS, 0x1B84, BP);
L2824:
    /* 2824  test    byte ptr [bp+1B8Ah],cl */
    logic8((uint8_t)(rb(pSS, BP + 0x1B8A) & CL));
L2828:
    /* 2828  je      L27AE */
    if (ZF) goto L27AE;
L282A:
    /* 282A  jmp     L2606 */
    goto L2606;
L282D: /* L282D */
    /* 282D  mov     si,2D42h */
    SI = 0x2D42;
L2830:
    /* 2830  mov     bx,word ptr ds:[1B86h] */
    BX = rw(pDS, 0x1B86);
L2834:
    /* 2834  mov     word ptr [bx],0FFFFh */
    ww(pDS, BX, 0xFFFF);
L2838:
    /* 2838  jmp     word ptr ds:[4112h] */
    return ASM_JMP(0x0085, rw(pDS, 0x4112));
L283C: /* _seg003_0272_283C */
    /* 283C  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2914  (+2914)
       _2914: when 3DFE is set, clear it, copy a palette (183h words) from SI to 3E00 and load it.
       _292D: wait for vertical retrace and write 256 colours from SI to the DAC (_2945). */
L2914: /* _seg003_0272_2914 */
    /* 2914  cmp     word ptr ds:[3DFEh],0 */
    sub16(rw(pDS, 0x3DFE), 0x0, 0);
L2919:
    /* 2919  jne     L291C */
    if (!ZF) goto L291C;
L291B:
    /* 291B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L291C: /* L291C */
    /* 291C  mov     word ptr ds:[3DFEh],0 */
    ww(pDS, 0x3DFE, 0x0);
L2922:
    /* 2922  mov     di,3E00h */
    DI = 0x3E00;
L2925:
    /* 2925  mov     cx,183h */
    CX = 0x183;
L2928:
    /* 2928  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L292A:
    /* 292A  mov     si,3E00h */
    SI = 0x3E00;
L292D: /* _seg003_0272_292D */
    /* 292D  cli */
    ;
L292E:
    /* 292E  mov     dx,INPUT_STATUS */
    DX = 0x3DA;
L2931: /* L2931 */
    /* 2931  in      al,dx */
    AL = asm_in8(DX);
L2932:
    /* 2932  and     al,8 */
    AL = logic8((uint8_t)(AL & 0x8));
L2934:
    /* 2934  je      L2931 */
    if (ZF) goto L2931;
L2936:
    /* 2936  mov     al,0 */
    AL = 0x0;
L2938:
    /* 2938  mov     dx,DAC_WRITE_INDEX */
    DX = 0x3C8;
L293B:
    /* 293B  out     dx,al */
    asm_out8(DX, AL);
L293C:
    /* 293C  inc     dx */
    DX = (uint16_t)(DX + 1);
L293D:
    /* 293D  mov     cx,100h */
    CX = 0x100;
L2940:
    /* 2940  call    _seg003_0272_2945 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2945), 0x2943)) != 0) return c;
L2943:
    /* 2943  sti */
    ;
L2944:
    /* 2944  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2945  (+2945)
       _2945: write CX colours from SI to the DAC data port (DX = 3C9h), with rep outsb on a 186 or
       later (36A2 at least 0BAh), else with single out instructions, since the 8086 has no outsb. */
L2945: /* _seg003_0272_2945 */
    /* 2945  cmp     word ptr ds:[36A2h],0BAh */
    sub16(rw(pDS, 0x36A2), 0xBA, 0);
L294B:
    /* 294B  jl      L2956 */
    if (SF != OF) goto L2956;
L294D:
    /* 294D  mov     ax,cx */
    AX = CX;
L294F:
    /* 294F  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L2951:
    /* 2951  add     cx,ax */
    CX = add16(CX, AX, 0);
L2953:
    /* 2953  rep outsb */
    while (CX) { asm_out8(DX, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); CX--; }
L2955:
    /* 2955  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2956: /* L2956 */
    /* 2956  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L2957:
    /* 2957  out     dx,al */
    asm_out8(DX, AL);
L2958:
    /* 2958  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L2959:
    /* 2959  out     dx,al */
    asm_out8(DX, AL);
L295A:
    /* 295A  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L295B:
    /* 295B  out     dx,al */
    asm_out8(DX, AL);
L295C:
    /* 295C  loop    L2956 */
    if (--CX) goto L2956;
L295E:
    /* 295E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2B62  (+2B62)
       _2B62: set the linear buffer _222B draws into (GRCORE's _50E7): DX its segment (39D0), and a
       row table at 39D2 for CX rows of stride BX starting at AX, highest row first. */
L2B62: /* _seg003_0272_2B62 */
    /* 2B62  mov     word ptr ds:[39D0h],dx */
    ww(pDS, 0x39D0, DX);
L2B66:
    /* 2B66  mov     di,39D2h */
    DI = 0x39D2;
L2B69:
    /* 2B69  mov     dx,cx */
    DX = CX;
L2B6B:
    /* 2B6B  dec     dx */
    DX = (uint16_t)(DX - 1);
L2B6C:
    /* 2B6C  shl     dx,1 */
    DX = (uint16_t)(DX << 1);
L2B6E:
    /* 2B6E  shl     dx,1 */
    DX = (uint16_t)(DX << 1);
L2B70:
    /* 2B70  add     di,dx */
    DI = (uint16_t)(DI + DX);
L2B72:
    /* 2B72  std */
    DF = 1;
L2B73: /* L2B73 */
    /* 2B73  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B74:
    /* 2B74  add     ax,bx */
    AX = add16(AX, BX, 0);
L2B76:
    /* 2B76  loop    L2B73 */
    if (--CX) goto L2B73;
L2B78:
    /* 2B78  cld */
    DF = 0;
L2B79:
    /* 2B79  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2B7A  (+2B7A)
       _2B7A: is there a VGA? int 10h, AX = 1A00h (read display combination): carry clear for an
       active VGA (codes 7 or 8), set otherwise. */
L2B7A: /* _seg003_0272_2B7A */
    /* 2B7A  mov     ax,VID_DISPLAY_CODE shl 8 */
    AX = 0x1A00;
L2B7D:
    /* 2B7D  int     10h */
    asm_int(0x10);
L2B7F:
    /* 2B7F  cmp     al,1Ah */
    sub8(AL, 0x1A, 0);
L2B81:
    /* 2B81  jne     L2B99 */
    if (!ZF) goto L2B99;
L2B83:
    /* 2B83  cmp     bl,0Bh */
    sub8(BL, 0xB, 0);
L2B86:
    /* 2B86  je      L2B99 */
    if (ZF) goto L2B99;
L2B88:
    /* 2B88  cmp     bl,0Ch */
    sub8(BL, 0xC, 0);
L2B8B:
    /* 2B8B  je      L2B99 */
    if (ZF) goto L2B99;
L2B8D:
    /* 2B8D  cmp     bl,7 */
    sub8(BL, 0x7, 0);
L2B90:
    /* 2B90  je      L2B97 */
    if (ZF) goto L2B97;
L2B92:
    /* 2B92  cmp     bl,8 */
    sub8(BL, 0x8, 0);
L2B95:
    /* 2B95  jne     L2B99 */
    if (!ZF) goto L2B99;
L2B97: /* L2B97 */
    /* 2B97  clc */
    CF = 0;
L2B98:
    /* 2B98  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2B99: /* L2B99 */
    /* 2B99  stc */
    CF = 1;
L2B9A:
    /* 2B9A  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2C9A: /* _seg003_0272_2C9A */
    /* 2C9A  mov     ax,1803h */
    AX = 0x1803;
L2C9D:
    /* 2C9D  mov     dx,GC_INDEX */
    DX = 0x3CE;
L2CA0:
    /* 2CA0  out     dx,ax */
    asm_out16(DX, AX);
L2CA1:
    /* 2CA1  mov     dx,SC_DATA */
    DX = 0x3C5;
L2CA4:
    /* 2CA4  mov     bl,0Fh */
    BL = 0xF;
L2CA6:
    /* 2CA6  push    es */
    push16(asm_es);
L2CA7:
    /* 2CA7  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L2CAB:
    /* 2CAB  jmp     short L2CBA */
    goto L2CBA;
L2CAD: /* L2CAD */
    /* 2CAD  add     di,word ptr [bp+36ACh] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AC));
L2CB1:
    /* 2CB1  and     al,bh */
    AL = (uint8_t)(AL & BH);
L2CB3:
    /* 2CB3  out     dx,al */
    asm_out8(DX, AL);
L2CB4:
    /* 2CB4  mov     al,bl */
    AL = BL;
L2CB6:
    /* 2CB6  test    byte ptr es:[di],al */
L2CB9:
    /* 2CB9  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2CBA: /* L2CBA */
    /* 2CBA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2CBB:
    /* 2CBB  shl     ax,1 */
    AX = shl16(AX, 1);
L2CBD:
    /* 2CBD  jb      L2D01 */
    if (CF) goto L2D01;
L2CBF:
    /* 2CBF  mov     cx,ax */
    CX = AX;
L2CC1:
    /* 2CC1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2CC2:
    /* 2CC2  mov     di,ax */
    DI = AX;
L2CC4:
    /* 2CC4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2CC5:
    /* 2CC5  mov     bp,ax */
    BP = AX;
L2CC7:
    /* 2CC7  mov     bh,byte ptr [bp+3CA8h] */
    BH = rb(pSS, BP + 0x3CA8);
L2CCB:
    /* 2CCB  mov     al,byte ptr [di+3B64h] */
    AL = rb(pDS, DI + 0x3B64);
L2CCF:
    /* 2CCF  xchg    cx,bp */
    { uint16_t t_ = BP;
    BP = CX;
    CX = t_; }
L2CD1:
    /* 2CD1  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2CD3:
    /* 2CD3  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2CD5:
    /* 2CD5  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2CD7:
    /* 2CD7  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2CD9:
    /* 2CD9  sub     cx,di */
    CX = sub16(CX, DI, 0);
L2CDB:
    /* 2CDB  jle     L2CAD */
    if (ZF || SF != OF) goto L2CAD;
L2CDD:
    /* 2CDD  out     dx,al */
    asm_out8(DX, AL);
L2CDE:
    /* 2CDE  add     di,word ptr [bp+36ACh] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AC));
L2CE2:
    /* 2CE2  mov     al,bl */
    AL = BL;
L2CE4:
    /* 2CE4  test    byte ptr es:[di],al */
L2CE7:
    /* 2CE7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2CE8:
    /* 2CE8  dec     cx */
    CX = (uint16_t)(CX - 1);
L2CE9:
    /* 2CE9  jcxz    L2CF6 */
    if (!CX) goto L2CF6;
L2CEB:
    /* 2CEB  mov     al,0Fh */
    AL = 0xF;
L2CED:
    /* 2CED  out     dx,al */
    asm_out8(DX, AL);
L2CEE:
    /* 2CEE  mov     al,bl */
    AL = BL;
L2CF0: /* L2CF0 */
    /* 2CF0  test    byte ptr es:[di],al */
L2CF3:
    /* 2CF3  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2CF4:
    /* 2CF4  loop    L2CF0 */
    if (--CX) goto L2CF0;
L2CF6: /* L2CF6 */
    /* 2CF6  mov     al,bh */
    AL = BH;
L2CF8:
    /* 2CF8  out     dx,al */
    asm_out8(DX, AL);
L2CF9:
    /* 2CF9  mov     al,bl */
    AL = BL;
L2CFB:
    /* 2CFB  test    byte ptr es:[di],al */
L2CFE:
    /* 2CFE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2CFF:
    /* 2CFF  jmp     L2CBA */
    goto L2CBA;
L2D01: /* L2D01 */
    /* 2D01  mov     ax,3 */
    AX = 0x3;
L2D04:
    /* 2D04  mov     dx,GC_INDEX */
    DX = 0x3CE;
L2D07:
    /* 2D07  out     dx,ax */
    asm_out16(DX, AX);
L2D08:
    /* 2D08  mov     al,8 */
    AL = 0x8;
L2D0A:
    /* 2D0A  out     dx,al */
    asm_out8(DX, AL);
L2D0B:
    /* 2D0B  pop     es */
    SET_ES(pop16());
L2D0C:
    /* 2D0C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2D0D: /* _seg003_0272_2D0D */
    /* 2D0D  push    si */
    push16(SI);
L2D0E:
    /* 2D0E  call    _seg003_0272_2D83 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2D83), 0x2D11)) != 0) return c;
L2D11:
    /* 2D11  pop     si */
    SI = pop16();
L2D12:
    /* 2D12  jmp     short L2D19 */
    goto L2D19;
L2D14: /* L2D14 */
    /* 2D14  pop     es */
    SET_ES(pop16());
L2D15:
    /* 2D15  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2D16:
    /* 2D16  call    _nullsub_2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x43F4), 0x2D19)) != 0) return c;
L2D19: /* L2D19 */
    /* 2D19  mov     dx,SC_DATA */
    DX = 0x3C5;
L2D1C:
    /* 2D1C  mov     bl,byte ptr ds:[4111h] */
    BL = rb(pDS, 0x4111);
L2D20:
    /* 2D20  push    es */
    push16(asm_es);
L2D21:
    /* 2D21  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L2D25:
    /* 2D25  jmp     short L2D43 */
    goto L2D43;
L2D27: /* L2D27 */
    /* 2D27  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L2D29:
    /* 2D29  shr     bh,1 */
    BH = (uint8_t)(BH >> 1);
L2D2B:
    /* 2D2B  not     al */
    AL = (uint8_t)~AL;
L2D2D:
    /* 2D2D  not     bh */
    BH = (uint8_t)~BH;
L2D2F:
    /* 2D2F  xchg    al,bh */
    { uint8_t t_ = BH;
    BH = AL;
    AL = t_; }
L2D31:
    /* 2D31  add     di,cx */
    DI = (uint16_t)(DI + CX);
L2D33:
    /* 2D33  neg     cx */
    CX = (uint16_t)-CX;
L2D35:
    /* 2D35  jmp     short L2D66 */
    goto L2D66;
L2D37: /* L2D37 */
    /* 2D37  jl      L2D27 */
    if (SF != OF) goto L2D27;
L2D39:
    /* 2D39  add     di,word ptr [bp+3840h] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x3840));
L2D3D:
    /* 2D3D  and     al,bh */
    AL = (uint8_t)(AL & BH);
L2D3F:
    /* 2D3F  out     dx,al */
    asm_out8(DX, AL);
L2D40:
    /* 2D40  mov     al,bl */
    AL = BL;
L2D42:
    /* 2D42  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2D43: /* L2D43 */
    /* 2D43  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D44:
    /* 2D44  shl     ax,1 */
    AX = shl16(AX, 1);
L2D46:
    /* 2D46  jb      L2D14 */
    if (CF) goto L2D14;
L2D48:
    /* 2D48  mov     cx,ax */
    CX = AX;
L2D4A:
    /* 2D4A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D4B:
    /* 2D4B  mov     di,ax */
    DI = AX;
L2D4D:
    /* 2D4D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D4E:
    /* 2D4E  mov     bp,ax */
    BP = AX;
L2D50:
    /* 2D50  mov     bh,byte ptr [bp+3CA8h] */
    BH = rb(pSS, BP + 0x3CA8);
L2D54:
    /* 2D54  mov     al,byte ptr [di+3B64h] */
    AL = rb(pDS, DI + 0x3B64);
L2D58:
    /* 2D58  xchg    cx,bp */
    { uint16_t t_ = BP;
    BP = CX;
    CX = t_; }
L2D5A:
    /* 2D5A  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2D5C:
    /* 2D5C  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2D5E:
    /* 2D5E  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2D60:
    /* 2D60  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2D62:
    /* 2D62  sub     cx,di */
    CX = sub16(CX, DI, 0);
L2D64:
    /* 2D64  jle     L2D37 */
    if (ZF || SF != OF) goto L2D37;
L2D66: /* L2D66 */
    /* 2D66  out     dx,al */
    asm_out8(DX, AL);
L2D67:
    /* 2D67  add     di,word ptr [bp+3840h] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x3840));
L2D6B:
    /* 2D6B  mov     al,bl */
    AL = BL;
L2D6D:
    /* 2D6D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2D6E:
    /* 2D6E  dec     cx */
    CX = (uint16_t)(CX - 1);
L2D6F:
    /* 2D6F  mov     al,0Fh */
    AL = 0xF;
L2D71:
    /* 2D71  out     dx,al */
    asm_out8(DX, AL);
L2D72:
    /* 2D72  mov     al,bl */
    AL = BL;
L2D74:
    /* 2D74  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L2D76:
    /* 2D76  mov     al,bh */
    AL = BH;
L2D78:
    /* 2D78  out     dx,al */
    asm_out8(DX, AL);
L2D79:
    /* 2D79  mov     al,bl */
    AL = BL;
L2D7B:
    /* 2D7B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2D7C:
    /* 2D7C  jmp     L2D43 */
    goto L2D43;
L2D7E: /* L2D7E */
    /* 2D7E  pop     es */
    SET_ES(pop16());
L2D7F:
    /* 2D7F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2D80:
    /* 2D80  call    _nullsub_2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x43F4), 0x2D83)) != 0) return c;
    return ASM_JMP(0x0085, 0x2D83);   /* into hand-written C */
L2E2E: /* L2E2E */
    /* 2E2E  mov     ax,0FFh */
    AX = 0xFF;
L2E31:
    /* 2E31  mov     dx,GC_DATA */
    DX = 0x3CF;
L2E34:
    /* 2E34  out     dx,al */
    asm_out8(DX, AL);
L2E35:
    /* 2E35  pop     es */
    SET_ES(pop16());
L2E36:
    /* 2E36  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2E37:
    /* 2E37  call    _nullsub_2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x43F4), 0x2E3A)) != 0) return c;
L2E3A: /* _seg003_0272_2E3A */
    /* 2E3A  mov     ax,0 */
    AX = 0x0;
L2E3D:
    /* 2E3D  mov     dx,GC_DATA */
    DX = 0x3CF;
L2E40:
    /* 2E40  out     dx,al */
    asm_out8(DX, AL);
L2E41:
    /* 2E41  mov     dx,SC_DATA */
    DX = 0x3C5;
L2E44:
    /* 2E44  mov     al,0Fh */
    AL = 0xF;
L2E46:
    /* 2E46  out     dx,al */
    asm_out8(DX, AL);
L2E47:
    /* 2E47  mov     bx,si */
    BX = SI;
L2E49:
    /* 2E49  mov     dx,word ptr ds:[410Ch] */
    DX = rw(pDS, 0x410C);
L2E4D:
    /* 2E4D  push    es */
    push16(asm_es);
L2E4E:
    /* 2E4E  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L2E52: /* L2E52 */
    /* 2E52  mov     bp,word ptr [bx] */
    BP = rw(pDS, BX);
L2E54:
    /* 2E54  shl     bp,1 */
    BP = shl16(BP, 1);
L2E56:
    /* 2E56  jb      L2E2E */
    if (CF) goto L2E2E;
L2E58:
    /* 2E58  mov     si,word ptr [bx+2] */
    SI = rw(pDS, BX + 0x2);
L2E5B:
    /* 2E5B  mov     cx,word ptr [bx+4] */
    CX = rw(pDS, BX + 0x4);
L2E5E:
    /* 2E5E  add     bx,6 */
    BX = (uint16_t)(BX + 0x6);
L2E61:
    /* 2E61  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L2E63:
    /* 2E63  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L2E65:
    /* 2E65  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2E67:
    /* 2E67  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2E69:
    /* 2E69  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L2E6B:
    /* 2E6B  inc     cx */
    CX = (uint16_t)(CX + 1);
L2E6C:
    /* 2E6C  add     si,word ptr [bp+36ACh] */
    SI = (uint16_t)(SI + rw(pSS, BP + 0x36AC));
L2E70:
    /* 2E70  mov     di,si */
    DI = SI;
L2E72:
    /* 2E72  add     si,dx */
    SI = (uint16_t)(SI + DX);
L2E74:
    /* 2E74  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L2E77:
    /* 2E77  jmp     L2E52 */
    goto L2E52;
L2E79: /* _seg003_0272_2E79 */
    /* 2E79  push    si */
    push16(SI);
L2E7A:
    /* 2E7A  call    _seg003_0272_2F18 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2F18), 0x2E7D)) != 0) return c;
L2E7D:
    /* 2E7D  pop     si */
    SI = pop16();
L2E7E:
    /* 2E7E  mov     ax,word ptr ds:[410Ch] */
    AX = rw(pDS, 0x410C);
L2E81:
    /* 2E81  add     ax,word ptr ds:[36ACh] */
    AX = (uint16_t)(AX + rw(pDS, 0x36AC));
L2E85:
    /* 2E85  sub     ax,word ptr ds:[3840h] */
    AX = (uint16_t)(AX - rw(pDS, 0x3840));
L2E89:
    /* 2E89  mov     word ptr ds:[410Eh],ax */
    ww(pDS, 0x410E, AX);
L2E8C:
    /* 2E8C  jmp     short L2E9A */
    goto L2E9A;
L2E8E: /* L2E8E */
    /* 2E8E  mov     ax,0FFh */
    AX = 0xFF;
L2E91:
    /* 2E91  mov     dx,GC_DATA */
    DX = 0x3CF;
L2E94:
    /* 2E94  out     dx,al */
    asm_out8(DX, AL);
L2E95:
    /* 2E95  pop     es */
    SET_ES(pop16());
L2E96:
    /* 2E96  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2E97:
    /* 2E97  call    _nullsub_2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x43F4), 0x2E9A)) != 0) return c;
L2E9A: /* L2E9A */
    /* 2E9A  mov     ax,0 */
    AX = 0x0;
L2E9D:
    /* 2E9D  mov     dx,GC_DATA */
    DX = 0x3CF;
L2EA0:
    /* 2EA0  out     dx,al */
    asm_out8(DX, AL);
L2EA1:
    /* 2EA1  mov     dx,SC_DATA */
    DX = 0x3C5;
L2EA4:
    /* 2EA4  mov     bx,si */
    BX = SI;
L2EA6:
    /* 2EA6  push    es */
    push16(asm_es);
L2EA7:
    /* 2EA7  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L2EAB:
    /* 2EAB  jmp     short L2ECC */
    goto L2ECC;
L2EAD: /* L2EAD */
    /* 2EAD  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L2EAF:
    /* 2EAF  shr     ah,1 */
    AH = (uint8_t)(AH >> 1);
L2EB1:
    /* 2EB1  not     ax */
    AX = (uint16_t)~AX;
L2EB3:
    /* 2EB3  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L2EB5:
    /* 2EB5  add     di,cx */
    DI = (uint16_t)(DI + CX);
L2EB7:
    /* 2EB7  neg     cx */
    CX = (uint16_t)-CX;
L2EB9:
    /* 2EB9  jmp     short L2EF1 */
    goto L2EF1;
L2EBB: /* L2EBB */
    /* 2EBB  jl      L2EAD */
    if (SF != OF) goto L2EAD;
L2EBD:
    /* 2EBD  add     di,word ptr [si+3840h] */
    DI = (uint16_t)(DI + rw(pDS, SI + 0x3840));
L2EC1:
    /* 2EC1  mov     bp,word ptr ds:[410Eh] */
    BP = rw(pDS, 0x410E);
L2EC5:
    /* 2EC5  and     al,ah */
    AL = (uint8_t)(AL & AH);
L2EC7:
    /* 2EC7  out     dx,al */
    asm_out8(DX, AL);
L2EC8:
    /* 2EC8  mov     al,byte ptr es:[bp+di] */
    AL = rb(pES, BP + DI);
L2ECB:
    /* 2ECB  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2ECC: /* L2ECC */
    /* 2ECC  mov     si,word ptr [bx] */
    SI = rw(pDS, BX);
L2ECE:
    /* 2ECE  shl     si,1 */
    SI = shl16(SI, 1);
L2ED0:
    /* 2ED0  jb      L2E8E */
    if (CF) goto L2E8E;
L2ED2:
    /* 2ED2  mov     di,word ptr [bx+2] */
    DI = rw(pDS, BX + 0x2);
L2ED5:
    /* 2ED5  mov     bp,word ptr [bx+4] */
    BP = rw(pDS, BX + 0x4);
L2ED8:
    /* 2ED8  add     bx,6 */
    BX = (uint16_t)(BX + 0x6);
L2EDB:
    /* 2EDB  mov     al,byte ptr [di+3B64h] */
    AL = rb(pDS, DI + 0x3B64);
L2EDF:
    /* 2EDF  mov     ah,byte ptr [bp+3CA8h] */
    AH = rb(pSS, BP + 0x3CA8);
L2EE3:
    /* 2EE3  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2EE5:
    /* 2EE5  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2EE7:
    /* 2EE7  mov     cx,bp */
    CX = BP;
L2EE9:
    /* 2EE9  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2EEB:
    /* 2EEB  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2EED:
    /* 2EED  sub     cx,di */
    CX = sub16(CX, DI, 0);
L2EEF:
    /* 2EEF  jle     L2EBB */
    if (ZF || SF != OF) goto L2EBB;
L2EF1: /* L2EF1 */
    /* 2EF1  add     di,word ptr [si+3840h] */
    DI = (uint16_t)(DI + rw(pDS, SI + 0x3840));
L2EF5:
    /* 2EF5  mov     si,di */
    SI = DI;
L2EF7:
    /* 2EF7  add     si,word ptr ds:[410Eh] */
    SI = (uint16_t)(SI + rw(pDS, 0x410E));
L2EFB:
    /* 2EFB  out     dx,al */
    asm_out8(DX, AL);
L2EFC:
    /* 2EFC  movs    byte ptr es:[di],byte ptr es:[si] */
    wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L2EFE:
    /* 2EFE  dec     cx */
    CX = (uint16_t)(CX - 1);
L2EFF:
    /* 2EFF  mov     al,0Fh */
    AL = 0xF;
L2F01:
    /* 2F01  out     dx,al */
    asm_out8(DX, AL);
L2F02:
    /* 2F02  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L2F05:
    /* 2F05  mov     al,ah */
    AL = AH;
L2F07:
    /* 2F07  out     dx,al */
    asm_out8(DX, AL);
L2F08:
    /* 2F08  movs    byte ptr es:[di],byte ptr es:[si] */
    wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L2F0A:
    /* 2F0A  jmp     L2ECC */
    goto L2ECC;
L2F0C: /* L2F0C */
    /* 2F0C  mov     ax,0FFh */
    AX = 0xFF;
L2F0F:
    /* 2F0F  mov     dx,GC_DATA */
    DX = 0x3CF;
L2F12:
    /* 2F12  out     dx,al */
    asm_out8(DX, AL);
L2F13:
    /* 2F13  pop     es */
    SET_ES(pop16());
L2F14:
    /* 2F14  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2F15:
    /* 2F15  call    _nullsub_2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x43F4), 0x2F18)) != 0) return c;
    return ASM_JMP(0x0085, 0x2F18);   /* into hand-written C */
L2FD1: /* L2FD1 */
    /* 2FD1  pop     es */
    SET_ES(pop16());
L2FD2:
    /* 2FD2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2FD3:
    /* 2FD3  call    _nullsub_2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x43F4), 0x2FD6)) != 0) return c;
L2FD6: /* _seg003_0272_2FD6 */
    /* 2FD6  mov     dx,SC_DATA */
    DX = 0x3C5;
L2FD9:
    /* 2FD9  mov     al,0Fh */
    AL = 0xF;
L2FDB:
    /* 2FDB  out     dx,al */
    asm_out8(DX, AL);
L2FDC:
    /* 2FDC  mov     dx,SC_INDEX */
    DX = 0x3C4;
L2FDF:
    /* 2FDF  mov     al,2 */
    AL = 0x2;
L2FE1:
    /* 2FE1  out     dx,al */
    asm_out8(DX, AL);
L2FE2:
    /* 2FE2  inc     dx */
    DX = (uint16_t)(DX + 1);
L2FE3:
    /* 2FE3  mov     bp,si */
    BP = SI;
L2FE5:
    /* 2FE5  mov     di,word ptr ds:[4116h] */
    DI = rw(pDS, 0x4116);
L2FE9:
    /* 2FE9  push    ds */
    push16(asm_ds);
L2FEA:
    /* 2FEA  mov     ds,word ptr ds:[3DFCh] */
    SET_DS(rw(pDS, 0x3DFC));
L2FEE: /* L2FEE */
    /* 2FEE  mov     bx,word ptr [bp] */
    BX = rw(pSS, BP);
L2FF1:
    /* 2FF1  shl     bx,1 */
    BX = shl16(BX, 1);
L2FF3:
    /* 2FF3  jb      L2FD1 */
    if (CF) goto L2FD1;
L2FF5:
    /* 2FF5  mov     si,word ptr [bp+2] */
    SI = rw(pSS, BP + 0x2);
L2FF8:
    /* 2FF8  mov     cx,word ptr [bp+4] */
    CX = rw(pSS, BP + 0x4);
L2FFB:
    /* 2FFB  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L2FFE:
    /* 2FFE  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L3000:
    /* 3000  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L3002:
    /* 3002  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L3004:
    /* 3004  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L3006:
    /* 3006  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L3008:
    /* 3008  inc     cx */
    CX = (uint16_t)(CX + 1);
L3009:
    /* 3009  add     si,word ptr ss:[bx+36ACh] */
    SI = (uint16_t)(SI + rw(pSS, BX + 0x36AC));
L300E:
    /* 300E  mov     bx,cx */
    BX = CX;
L3010:
    /* 3010  mov     al,1 */
    AL = 0x1;
L3012:
    /* 3012  out     dx,al */
    asm_out8(DX, AL);
L3013:
    /* 3013  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3015:
    /* 3015  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L3017:
    /* 3017  mov     cx,bx */
    CX = BX;
L3019:
    /* 3019  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L301B:
    /* 301B  out     dx,al */
    asm_out8(DX, AL);
L301C:
    /* 301C  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L301E:
    /* 301E  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L3020:
    /* 3020  mov     cx,bx */
    CX = BX;
L3022:
    /* 3022  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L3024:
    /* 3024  out     dx,al */
    asm_out8(DX, AL);
L3025:
    /* 3025  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3027:
    /* 3027  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L3029:
    /* 3029  mov     cx,bx */
    CX = BX;
L302B:
    /* 302B  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L302D:
    /* 302D  out     dx,al */
    asm_out8(DX, AL);
L302E:
    /* 302E  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3030:
    /* 3030  jmp     L2FEE */
    goto L2FEE;
L3032: /* L3032 */
    /* 3032  pop     es */
    SET_ES(pop16());
L3033:
    /* 3033  dec     dx */
    DX = dec16(DX);
L3034:
    /* 3034  mov     al,8 */
    AL = 0x8;
L3036:
    /* 3036  out     dx,al */
    asm_out8(DX, AL);
L3037:
    /* 3037  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3038:
    /* 3038  call    _nullsub_2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x43F4), 0x303B)) != 0) return c;
L303B: /* _seg003_0272_303B */
    /* 303B  mov     dx,SC_DATA */
    DX = 0x3C5;
L303E:
    /* 303E  mov     al,0Fh */
    AL = 0xF;
L3040:
    /* 3040  out     dx,al */
    asm_out8(DX, AL);
L3041:
    /* 3041  mov     dx,GC_INDEX */
    DX = 0x3CE;
L3044:
    /* 3044  mov     al,4 */
    AL = 0x4;
L3046:
    /* 3046  out     dx,al */
    asm_out8(DX, AL);
L3047:
    /* 3047  inc     dx */
    DX = (uint16_t)(DX + 1);
L3048:
    /* 3048  mov     bp,si */
    BP = SI;
L304A:
    /* 304A  mov     si,word ptr ds:[4116h] */
    SI = rw(pDS, 0x4116);
L304E:
    /* 304E  push    ds */
    push16(asm_ds);
L304F:
    /* 304F  mov     ds,word ptr ds:[3DFCh] */
    SET_DS(rw(pDS, 0x3DFC));
L3053: /* L3053 */
    /* 3053  mov     bx,word ptr [bp] */
    BX = rw(pSS, BP);
L3056:
    /* 3056  shl     bx,1 */
    BX = shl16(BX, 1);
L3058:
    /* 3058  jb      L3032 */
    if (CF) goto L3032;
L305A:
    /* 305A  mov     di,word ptr [bp+2] */
    DI = rw(pSS, BP + 0x2);
L305D:
    /* 305D  mov     cx,word ptr [bp+4] */
    CX = rw(pSS, BP + 0x4);
L3060:
    /* 3060  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L3063:
    /* 3063  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L3065:
    /* 3065  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L3067:
    /* 3067  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L3069:
    /* 3069  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L306B:
    /* 306B  sub     cx,di */
    CX = (uint16_t)(CX - DI);
L306D:
    /* 306D  inc     cx */
    CX = (uint16_t)(CX + 1);
L306E:
    /* 306E  add     di,word ptr ss:[bx+36ACh] */
    DI = (uint16_t)(DI + rw(pSS, BX + 0x36AC));
L3073:
    /* 3073  mov     bx,cx */
    BX = CX;
L3075:
    /* 3075  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L3077:
    /* 3077  out     dx,al */
    asm_out8(DX, AL);
L3078:
    /* 3078  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L307A:
    /* 307A  sub     di,bx */
    DI = (uint16_t)(DI - BX);
L307C:
    /* 307C  mov     cx,bx */
    CX = BX;
L307E:
    /* 307E  inc     ax */
    AX = (uint16_t)(AX + 1);
L307F:
    /* 307F  out     dx,al */
    asm_out8(DX, AL);
L3080:
    /* 3080  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3082:
    /* 3082  sub     di,bx */
    DI = (uint16_t)(DI - BX);
L3084:
    /* 3084  mov     cx,bx */
    CX = BX;
L3086:
    /* 3086  inc     ax */
    AX = (uint16_t)(AX + 1);
L3087:
    /* 3087  out     dx,al */
    asm_out8(DX, AL);
L3088:
    /* 3088  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L308A:
    /* 308A  sub     di,bx */
    DI = (uint16_t)(DI - BX);
L308C:
    /* 308C  mov     cx,bx */
    CX = BX;
L308E:
    /* 308E  inc     ax */
    AX = (uint16_t)(AX + 1);
L308F:
    /* 308F  out     dx,al */
    asm_out8(DX, AL);
L3090:
    /* 3090  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3092:
    /* 3092  jmp     L3053 */
    goto L3053;
L30CF: /* _seg003_0272_30CF */
    /* 30CF  mov     ch,cl */
    CH = CL;
L30D1:
    /* 30D1  call    _seg003_0272_30E3 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x30E3), 0x30D4)) != 0) return c;
L30D4:
    /* 30D4  cmp     al,ch */
    sub8(AL, CH, 0);
L30D6:
    /* 30D6  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L30D7: /* _seg003_0272_30D7 */
    /* 30D7  mov     ch,cl */
    CH = CL;
L30D9:
    /* 30D9  call    _seg003_0272_30FB */
    if ((c = asm_call(ASM_JMP(0x0085, 0x30FB), 0x30DC)) != 0) return c;
L30DC:
    /* 30DC  cmp     al,ch */
    sub8(AL, CH, 0);
L30DE:
    /* 30DE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L30DF: /* L30DF */
    /* 30DF  mov     ax,0FFFFh */
    AX = 0xFFFF;
L30E2:
    /* 30E2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_30E3  (+30E3)
       _30E3: read the pixel at (AX, BX), or AX = -1 outside the window; falls into _30FB. */
L30E3: /* _seg003_0272_30E3 */
    /* 30E3  cmp     bx,word ptr ds:[3DF6h] */
    sub16(BX, rw(pDS, 0x3DF6), 0);
L30E7:
    /* 30E7  jg      L30DF */
    if (!ZF && SF == OF) goto L30DF;
L30E9:
    /* 30E9  cmp     bx,word ptr ds:[3DFAh] */
    sub16(BX, rw(pDS, 0x3DFA), 0);
L30ED:
    /* 30ED  jl      L30DF */
    if (SF != OF) goto L30DF;
L30EF:
    /* 30EF  cmp     ax,word ptr ds:[3DF8h] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L30F3:
    /* 30F3  jg      L30DF */
    if (!ZF && SF == OF) goto L30DF;
L30F5:
    /* 30F5  cmp     ax,word ptr ds:[3DF4h] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L30F9:
    /* 30F9  jl      L30DF */
    if (SF != OF) goto L30DF;
    return ASM_JMP(0x0085, 0x30FB);   /* into hand-written C */

    /* seg003_0272_3121  (+3121)
       _3121: copy a vertical line (x AX, rows BX down to DX) from the other page (410C) through the
       latches (FM copy_uvline). */
L3121: /* _seg003_0272_3121 */
    /* 3121  mov     di,ax */
    DI = AX;
L3123:
    /* 3123  mov     cx,bx */
    CX = BX;
L3125:
    /* 3125  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L3127:
    /* 3127  inc     cx */
    CX = (uint16_t)(CX + 1);
L3128:
    /* 3128  mov     ax,0 */
    AX = 0x0;
L312B:
    /* 312B  mov     dx,GC_DATA */
    DX = 0x3CF;
L312E:
    /* 312E  out     dx,al */
    asm_out8(DX, AL);
L312F:
    /* 312F  mov     al,byte ptr [di+3B64h] */
    AL = rb(pDS, DI + 0x3B64);
L3133:
    /* 3133  and     al,byte ptr [di+3CA8h] */
    AL = (uint8_t)(AL & rb(pDS, DI + 0x3CA8));
L3137:
    /* 3137  mov     dx,SC_DATA */
    DX = 0x3C5;
L313A:
    /* 313A  out     dx,al */
    asm_out8(DX, AL);
L313B:
    /* 313B  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L313D:
    /* 313D  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L313F:
    /* 313F  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3141:
    /* 3141  add     di,word ptr [bx+36ACh] */
    DI = (uint16_t)(DI + rw(pDS, BX + 0x36AC));
L3145:
    /* 3145  mov     bx,word ptr ds:[410Ch] */
    BX = rw(pDS, 0x410C);
L3149:
    /* 3149  mov     dx,word ptr ds:[36A8h] */
    DX = rw(pDS, 0x36A8);
L314D:
    /* 314D  dec     dx */
    DX = (uint16_t)(DX - 1);
L314E:
    /* 314E  push    es */
    push16(asm_es);
L314F:
    /* 314F  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L3153: /* L3153 */
    /* 3153  mov     al,byte ptr es:[bx+di] */
    AL = rb(pES, BX + DI);
L3156:
    /* 3156  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3157:
    /* 3157  add     di,dx */
    DI = add16(DI, DX, 0);
L3159:
    /* 3159  loop    L3153 */
    if (--CX) goto L3153;
L315B:
    /* 315B  pop     es */
    SET_ES(pop16());
L315C:
    /* 315C  mov     ax,0FFh */
    AX = 0xFF;
L315F:
    /* 315F  mov     dx,GC_DATA */
    DX = 0x3CF;
L3162:
    /* 3162  out     dx,al */
    asm_out8(DX, AL);
L3163:
    /* 3163  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_3164  (+3164)
       _3164: a solid vertical line (x AX, rows BX down to DX) in the colour 4111 (FM solid_uvline). */
L3164: /* _seg003_0272_3164 */
    /* 3164  mov     di,ax */
    DI = AX;
L3166:
    /* 3166  mov     cx,bx */
    CX = BX;
L3168:
    /* 3168  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L316A:
    /* 316A  inc     cx */
    CX = (uint16_t)(CX + 1);
L316B:
    /* 316B  mov     al,byte ptr [di+3B64h] */
    AL = rb(pDS, DI + 0x3B64);
L316F:
    /* 316F  and     al,byte ptr [di+3CA8h] */
    AL = (uint8_t)(AL & rb(pDS, DI + 0x3CA8));
L3173:
    /* 3173  mov     dx,SC_DATA */
    DX = 0x3C5;
L3176:
    /* 3176  out     dx,al */
    asm_out8(DX, AL);
L3177:
    /* 3177  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L3179:
    /* 3179  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L317B:
    /* 317B  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L317D:
    /* 317D  add     di,word ptr [bx+36ACh] */
    DI = (uint16_t)(DI + rw(pDS, BX + 0x36AC));
L3181:
    /* 3181  mov     bx,word ptr ds:[36A8h] */
    BX = rw(pDS, 0x36A8);
L3185:
    /* 3185  dec     bx */
    BX = (uint16_t)(BX - 1);
L3186:
    /* 3186  mov     al,byte ptr ds:[4111h] */
    AL = rb(pDS, 0x4111);
L3189:
    /* 3189  push    es */
    push16(asm_es);
L318A:
    /* 318A  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L318E: /* L318E */
    /* 318E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L318F:
    /* 318F  add     di,bx */
    DI = add16(DI, BX, 0);
L3191:
    /* 3191  loop    L318E */
    if (--CX) goto L318E;
L3193:
    /* 3193  pop     es */
    SET_ES(pop16());
L3194:
    /* 3194  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* nullsub_1  (+31E5) */
L31E5: /* _nullsub_1 */
    /* 31E5  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L31E6:
    /* 31E6  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
