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
    case 0x21D4: goto L21D4;
    case 0x21D9: goto L21D9;
    case 0x21DB: goto L21DB;
    case 0x21E1: goto L21E1;
    case 0x21E4: goto L21E4;
    case 0x21EA: goto L21EA;
    case 0x21ED: goto L21ED;
    case 0x21F2: goto L21F2;
    case 0x21F4: goto L21F4;
    case 0x21F7: goto L21F7;
    case 0x21F9: goto L21F9;
    case 0x21FF: goto L21FF;
    case 0x2202: goto L2202;
    case 0x2208: goto L2208;
    case 0x220B: goto L220B;
    case 0x2211: goto L2211;
    case 0x2214: goto L2214;
    case 0x2219: goto L2219;
    case 0x221B: goto L221B;
    case 0x2221: goto L2221;
    case 0x2223: goto L2223;
    case 0x2229: goto L2229;
    case 0x222B: goto L222B;
    case 0x2230: goto L2230;
    case 0x2232: goto L2232;
    case 0x2238: goto L2238;
    case 0x223A: goto L223A;
    case 0x2240: goto L2240;
    case 0x2242: goto L2242;
    case 0x2248: goto L2248;
    case 0x224D: goto L224D;
    case 0x2250: goto L2250;
    case 0x2252: goto L2252;
    case 0x2254: goto L2254;
    case 0x225A: goto L225A;
    case 0x225C: goto L225C;
    case 0x225F: goto L225F;
    case 0x2261: goto L2261;
    case 0x2264: goto L2264;
    case 0x2266: goto L2266;
    case 0x2269: goto L2269;
    case 0x226B: goto L226B;
    case 0x226D: goto L226D;
    case 0x2273: goto L2273;
    case 0x2275: goto L2275;
    case 0x227B: goto L227B;
    case 0x227F: goto L227F;
    case 0x2281: goto L2281;
    case 0x2283: goto L2283;
    case 0x2284: goto L2284;
    case 0x2286: goto L2286;
    case 0x228A: goto L228A;
    case 0x228D: goto L228D;
    case 0x228F: goto L228F;
    case 0x2294: goto L2294;
    case 0x2296: goto L2296;
    case 0x229A: goto L229A;
    case 0x229B: goto L229B;
    case 0x229F: goto L229F;
    case 0x22A3: goto L22A3;
    case 0x22A7: goto L22A7;
    case 0x22AB: goto L22AB;
    case 0x22AC: goto L22AC;
    case 0x22AE: goto L22AE;
    case 0x22B0: goto L22B0;
    case 0x22B5: goto L22B5;
    case 0x22B7: goto L22B7;
    case 0x22B9: goto L22B9;
    case 0x22BB: goto L22BB;
    case 0x22BD: goto L22BD;
    case 0x22BE: goto L22BE;
    case 0x22C2: goto L22C2;
    case 0x22C4: goto L22C4;
    case 0x22C7: goto L22C7;
    case 0x22CB: goto L22CB;
    case 0x22CD: goto L22CD;
    case 0x22CF: goto L22CF;
    case 0x22D1: goto L22D1;
    case 0x22D3: goto L22D3;
    case 0x22D5: goto L22D5;
    case 0x22D9: goto L22D9;
    case 0x22DB: goto L22DB;
    case 0x22DD: goto L22DD;
    case 0x22E0: goto L22E0;
    case 0x22E1: goto L22E1;
    case 0x22E3: goto L22E3;
    case 0x22E5: goto L22E5;
    case 0x22E7: goto L22E7;
    case 0x22E9: goto L22E9;
    case 0x22EB: goto L22EB;
    case 0x22EC: goto L22EC;
    case 0x22F0: goto L22F0;
    case 0x22F2: goto L22F2;
    case 0x22F6: goto L22F6;
    case 0x22F8: goto L22F8;
    case 0x22FA: goto L22FA;
    case 0x22FC: goto L22FC;
    case 0x22FE: goto L22FE;
    case 0x2300: goto L2300;
    case 0x2302: goto L2302;
    case 0x2306: goto L2306;
    case 0x2308: goto L2308;
    case 0x230A: goto L230A;
    case 0x230C: goto L230C;
    case 0x230E: goto L230E;
    case 0x2310: goto L2310;
    case 0x2311: goto L2311;
    case 0x2313: goto L2313;
    case 0x2315: goto L2315;
    case 0x2319: goto L2319;
    case 0x231C: goto L231C;
    case 0x2320: goto L2320;
    case 0x2322: goto L2322;
    case 0x2323: goto L2323;
    case 0x2324: goto L2324;
    case 0x2325: goto L2325;
    case 0x2327: goto L2327;
    case 0x2329: goto L2329;
    case 0x232A: goto L232A;
    case 0x2330: goto L2330;
    case 0x2332: goto L2332;
    case 0x2334: goto L2334;
    case 0x2335: goto L2335;
    case 0x2337: goto L2337;
    case 0x2339: goto L2339;
    case 0x233B: goto L233B;
    case 0x233E: goto L233E;
    case 0x2341: goto L2341;
    case 0x2344: goto L2344;
    case 0x2346: goto L2346;
    case 0x2347: goto L2347;
    case 0x2349: goto L2349;
    case 0x234B: goto L234B;
    case 0x234E: goto L234E;
    case 0x2351: goto L2351;
    case 0x2354: goto L2354;
    case 0x2357: goto L2357;
    case 0x235A: goto L235A;
    case 0x235C: goto L235C;
    case 0x235F: goto L235F;
    case 0x2361: goto L2361;
    case 0x2365: goto L2365;
    case 0x2369: goto L2369;
    case 0x236B: goto L236B;
    case 0x236F: goto L236F;
    case 0x2372: goto L2372;
    case 0x2373: goto L2373;
    case 0x2375: goto L2375;
    case 0x2376: goto L2376;
    case 0x2379: goto L2379;
    case 0x237C: goto L237C;
    case 0x237F: goto L237F;
    case 0x2381: goto L2381;
    case 0x2383: goto L2383;
    case 0x2386: goto L2386;
    case 0x2387: goto L2387;
    case 0x238C: goto L238C;
    case 0x238E: goto L238E;
    case 0x2392: goto L2392;
    case 0x2394: goto L2394;
    case 0x2396: goto L2396;
    case 0x2397: goto L2397;
    case 0x2399: goto L2399;
    case 0x239B: goto L239B;
    case 0x239E: goto L239E;
    case 0x23A0: goto L23A0;
    case 0x23A2: goto L23A2;
    case 0x23A4: goto L23A4;
    case 0x23A6: goto L23A6;
    case 0x23A7: goto L23A7;
    case 0x23A8: goto L23A8;
    case 0x23AA: goto L23AA;
    case 0x23AD: goto L23AD;
    case 0x23AF: goto L23AF;
    case 0x23B1: goto L23B1;
    case 0x23B3: goto L23B3;
    case 0x23B5: goto L23B5;
    case 0x23B6: goto L23B6;
    case 0x23B7: goto L23B7;
    case 0x23B9: goto L23B9;
    case 0x23BD: goto L23BD;
    case 0x23BF: goto L23BF;
    case 0x23C1: goto L23C1;
    case 0x23C3: goto L23C3;
    case 0x23C5: goto L23C5;
    case 0x23C9: goto L23C9;
    case 0x23CB: goto L23CB;
    case 0x23CD: goto L23CD;
    case 0x23CE: goto L23CE;
    case 0x23D0: goto L23D0;
    case 0x23D2: goto L23D2;
    case 0x23D4: goto L23D4;
    case 0x23D8: goto L23D8;
    case 0x23DA: goto L23DA;
    case 0x23DC: goto L23DC;
    case 0x23DD: goto L23DD;
    case 0x23DF: goto L23DF;
    case 0x23E1: goto L23E1;
    case 0x23E3: goto L23E3;
    case 0x23E5: goto L23E5;
    case 0x23E7: goto L23E7;
    case 0x23E9: goto L23E9;
    case 0x23EB: goto L23EB;
    case 0x23ED: goto L23ED;
    case 0x23F0: goto L23F0;
    case 0x23F2: goto L23F2;
    case 0x23F5: goto L23F5;
    case 0x23F7: goto L23F7;
    case 0x23FB: goto L23FB;
    case 0x2400: goto L2400;
    case 0x2402: goto L2402;
    case 0x2403: goto L2403;
    case 0x2405: goto L2405;
    case 0x2409: goto L2409;
    case 0x240C: goto L240C;
    case 0x2410: goto L2410;
    case 0x2412: goto L2412;
    case 0x2413: goto L2413;
    case 0x2418: goto L2418;
    case 0x241A: goto L241A;
    case 0x241E: goto L241E;
    case 0x2421: goto L2421;
    case 0x2424: goto L2424;
    case 0x2427: goto L2427;
    case 0x2429: goto L2429;
    case 0x242C: goto L242C;
    case 0x242D: goto L242D;
    case 0x242F: goto L242F;
    case 0x2433: goto L2433;
    case 0x2437: goto L2437;
    case 0x243B: goto L243B;
    case 0x243E: goto L243E;
    case 0x243F: goto L243F;
    case 0x2441: goto L2441;
    case 0x2442: goto L2442;
    case 0x2445: goto L2445;
    case 0x2448: goto L2448;
    case 0x244B: goto L244B;
    case 0x244D: goto L244D;
    case 0x244F: goto L244F;
    case 0x2452: goto L2452;
    case 0x2456: goto L2456;
    case 0x245A: goto L245A;
    case 0x245C: goto L245C;
    case 0x245D: goto L245D;
    case 0x245F: goto L245F;
    case 0x2460: goto L2460;
    case 0x2463: goto L2463;
    case 0x2464: goto L2464;
    case 0x2468: goto L2468;
    case 0x246A: goto L246A;
    case 0x246D: goto L246D;
    case 0x2470: goto L2470;
    case 0x2473: goto L2473;
    case 0x2474: goto L2474;
    case 0x2476: goto L2476;
    case 0x2479: goto L2479;
    case 0x247A: goto L247A;
    case 0x247C: goto L247C;
    case 0x2480: goto L2480;
    case 0x2484: goto L2484;
    case 0x2487: goto L2487;
    case 0x248B: goto L248B;
    case 0x248C: goto L248C;
    case 0x248F: goto L248F;
    case 0x2492: goto L2492;
    case 0x2494: goto L2494;
    case 0x2495: goto L2495;
    case 0x2498: goto L2498;
    case 0x2499: goto L2499;
    case 0x249B: goto L249B;
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
    case 0x283D: goto L283D;
    case 0x283E: goto L283E;
    case 0x2841: goto L2841;
    case 0x2843: goto L2843;
    case 0x2846: goto L2846;
    case 0x2847: goto L2847;
    case 0x284A: goto L284A;
    case 0x284D: goto L284D;
    case 0x284F: goto L284F;
    case 0x2850: goto L2850;
    case 0x2856: goto L2856;
    case 0x2859: goto L2859;
    case 0x285C: goto L285C;
    case 0x285E: goto L285E;
    case 0x285F: goto L285F;
    case 0x2860: goto L2860;
    case 0x2861: goto L2861;
    case 0x2864: goto L2864;
    case 0x2867: goto L2867;
    case 0x2869: goto L2869;
    case 0x286C: goto L286C;
    case 0x2870: goto L2870;
    case 0x2874: goto L2874;
    case 0x2876: goto L2876;
    case 0x2877: goto L2877;
    case 0x287B: goto L287B;
    case 0x287D: goto L287D;
    case 0x287F: goto L287F;
    case 0x2881: goto L2881;
    case 0x2883: goto L2883;
    case 0x2887: goto L2887;
    case 0x288A: goto L288A;
    case 0x288B: goto L288B;
    case 0x288C: goto L288C;
    case 0x288D: goto L288D;
    case 0x2890: goto L2890;
    case 0x2891: goto L2891;
    case 0x2894: goto L2894;
    case 0x2896: goto L2896;
    case 0x2899: goto L2899;
    case 0x289A: goto L289A;
    case 0x289B: goto L289B;
    case 0x289C: goto L289C;
    case 0x289D: goto L289D;
    case 0x289F: goto L289F;
    case 0x28A1: goto L28A1;
    case 0x28A4: goto L28A4;
    case 0x28A6: goto L28A6;
    case 0x28A7: goto L28A7;
    case 0x28A8: goto L28A8;
    case 0x28AB: goto L28AB;
    case 0x28AC: goto L28AC;
    case 0x28AE: goto L28AE;
    case 0x28B1: goto L28B1;
    case 0x28B3: goto L28B3;
    case 0x28B4: goto L28B4;
    case 0x28B5: goto L28B5;
    case 0x28B6: goto L28B6;
    case 0x28B8: goto L28B8;
    case 0x28B9: goto L28B9;
    case 0x28BB: goto L28BB;
    case 0x28BE: goto L28BE;
    case 0x28C1: goto L28C1;
    case 0x28C4: goto L28C4;
    case 0x28C7: goto L28C7;
    case 0x28CA: goto L28CA;
    case 0x28CD: goto L28CD;
    case 0x28D0: goto L28D0;
    case 0x28D3: goto L28D3;
    case 0x28D6: goto L28D6;
    case 0x28D7: goto L28D7;
    case 0x28D9: goto L28D9;
    case 0x28DB: goto L28DB;
    case 0x28DE: goto L28DE;
    case 0x28E1: goto L28E1;
    case 0x28E2: goto L28E2;
    case 0x28E6: goto L28E6;
    case 0x28E8: goto L28E8;
    case 0x28EB: goto L28EB;
    case 0x28ED: goto L28ED;
    case 0x28F0: goto L28F0;
    case 0x28F2: goto L28F2;
    case 0x28F5: goto L28F5;
    case 0x28F9: goto L28F9;
    case 0x28FC: goto L28FC;
    case 0x28FF: goto L28FF;
    case 0x2901: goto L2901;
    case 0x2904: goto L2904;
    case 0x2907: goto L2907;
    case 0x290A: goto L290A;
    case 0x290D: goto L290D;
    case 0x2910: goto L2910;
    case 0x2913: goto L2913;
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
    case 0x295F: goto L295F;
    case 0x2963: goto L2963;
    case 0x2965: goto L2965;
    case 0x2969: goto L2969;
    case 0x296B: goto L296B;
    case 0x296E: goto L296E;
    case 0x2970: goto L2970;
    case 0x2971: goto L2971;
    case 0x2973: goto L2973;
    case 0x2975: goto L2975;
    case 0x2976: goto L2976;
    case 0x2977: goto L2977;
    case 0x2979: goto L2979;
    case 0x297B: goto L297B;
    case 0x297E: goto L297E;
    case 0x2982: goto L2982;
    case 0x2986: goto L2986;
    case 0x2988: goto L2988;
    case 0x298A: goto L298A;
    case 0x298C: goto L298C;
    case 0x298F: goto L298F;
    case 0x2991: goto L2991;
    case 0x2992: goto L2992;
    case 0x2993: goto L2993;
    case 0x2995: goto L2995;
    case 0x2996: goto L2996;
    case 0x2999: goto L2999;
    case 0x299B: goto L299B;
    case 0x299C: goto L299C;
    case 0x299D: goto L299D;
    case 0x299E: goto L299E;
    case 0x29A0: goto L29A0;
    case 0x29A1: goto L29A1;
    case 0x29A2: goto L29A2;
    case 0x29A4: goto L29A4;
    case 0x29A6: goto L29A6;
    case 0x29A8: goto L29A8;
    case 0x29AA: goto L29AA;
    case 0x29AD: goto L29AD;
    case 0x29AF: goto L29AF;
    case 0x29B1: goto L29B1;
    case 0x29B4: goto L29B4;
    case 0x29B6: goto L29B6;
    case 0x29B9: goto L29B9;
    case 0x29BB: goto L29BB;
    case 0x29BD: goto L29BD;
    case 0x29C0: goto L29C0;
    case 0x29C2: goto L29C2;
    case 0x29C4: goto L29C4;
    case 0x29C5: goto L29C5;
    case 0x29C8: goto L29C8;
    case 0x29CB: goto L29CB;
    case 0x29CC: goto L29CC;
    case 0x29CE: goto L29CE;
    case 0x29D0: goto L29D0;
    case 0x29D1: goto L29D1;
    case 0x29D3: goto L29D3;
    case 0x29D5: goto L29D5;
    case 0x29D8: goto L29D8;
    case 0x29DA: goto L29DA;
    case 0x29DB: goto L29DB;
    case 0x29DC: goto L29DC;
    case 0x29DE: goto L29DE;
    case 0x29DF: goto L29DF;
    case 0x29E0: goto L29E0;
    case 0x29E2: goto L29E2;
    case 0x29E3: goto L29E3;
    case 0x29E4: goto L29E4;
    case 0x29E6: goto L29E6;
    case 0x29E7: goto L29E7;
    case 0x29E9: goto L29E9;
    case 0x29EB: goto L29EB;
    case 0x29ED: goto L29ED;
    case 0x29EF: goto L29EF;
    case 0x29F3: goto L29F3;
    case 0x29F5: goto L29F5;
    case 0x29F7: goto L29F7;
    case 0x29FB: goto L29FB;
    case 0x29FE: goto L29FE;
    case 0x2A00: goto L2A00;
    case 0x2A02: goto L2A02;
    case 0x2A03: goto L2A03;
    case 0x2A05: goto L2A05;
    case 0x2A08: goto L2A08;
    case 0x2A09: goto L2A09;
    case 0x2A0C: goto L2A0C;
    case 0x2A0D: goto L2A0D;
    case 0x2A0E: goto L2A0E;
    case 0x2A0F: goto L2A0F;
    case 0x2A11: goto L2A11;
    case 0x2A15: goto L2A15;
    case 0x2A19: goto L2A19;
    case 0x2A1B: goto L2A1B;
    case 0x2A1D: goto L2A1D;
    case 0x2A1F: goto L2A1F;
    case 0x2A23: goto L2A23;
    case 0x2A24: goto L2A24;
    case 0x2A26: goto L2A26;
    case 0x2A28: goto L2A28;
    case 0x2A2A: goto L2A2A;
    case 0x2A2B: goto L2A2B;
    case 0x2A2F: goto L2A2F;
    case 0x2A31: goto L2A31;
    case 0x2A33: goto L2A33;
    case 0x2A37: goto L2A37;
    case 0x2A39: goto L2A39;
    case 0x2A3A: goto L2A3A;
    case 0x2A3B: goto L2A3B;
    case 0x2A3F: goto L2A3F;
    case 0x2A40: goto L2A40;
    case 0x2A43: goto L2A43;
    case 0x2A47: goto L2A47;
    case 0x2A49: goto L2A49;
    case 0x2A4B: goto L2A4B;
    case 0x2A4F: goto L2A4F;
    case 0x2A51: goto L2A51;
    case 0x2A54: goto L2A54;
    case 0x2A57: goto L2A57;
    case 0x2A58: goto L2A58;
    case 0x2A5A: goto L2A5A;
    case 0x2A5C: goto L2A5C;
    case 0x2A5D: goto L2A5D;
    case 0x2A5F: goto L2A5F;
    case 0x2A61: goto L2A61;
    case 0x2A62: goto L2A62;
    case 0x2A65: goto L2A65;
    case 0x2A67: goto L2A67;
    case 0x2A68: goto L2A68;
    case 0x2A69: goto L2A69;
    case 0x2A6B: goto L2A6B;
    case 0x2A6C: goto L2A6C;
    case 0x2A6D: goto L2A6D;
    case 0x2A6F: goto L2A6F;
    case 0x2A70: goto L2A70;
    case 0x2A71: goto L2A71;
    case 0x2A73: goto L2A73;
    case 0x2A74: goto L2A74;
    case 0x2A77: goto L2A77;
    case 0x2A78: goto L2A78;
    case 0x2A7A: goto L2A7A;
    case 0x2A7C: goto L2A7C;
    case 0x2A7D: goto L2A7D;
    case 0x2A7F: goto L2A7F;
    case 0x2A81: goto L2A81;
    case 0x2A84: goto L2A84;
    case 0x2A86: goto L2A86;
    case 0x2A87: goto L2A87;
    case 0x2A88: goto L2A88;
    case 0x2A8B: goto L2A8B;
    case 0x2A8D: goto L2A8D;
    case 0x2A8E: goto L2A8E;
    case 0x2A8F: goto L2A8F;
    case 0x2A90: goto L2A90;
    case 0x2A93: goto L2A93;
    case 0x2A95: goto L2A95;
    case 0x2A96: goto L2A96;
    case 0x2A97: goto L2A97;
    case 0x2A98: goto L2A98;
    case 0x2A99: goto L2A99;
    case 0x2A9A: goto L2A9A;
    case 0x2A9C: goto L2A9C;
    case 0x2A9D: goto L2A9D;
    case 0x2A9E: goto L2A9E;
    case 0x2A9F: goto L2A9F;
    case 0x2AA2: goto L2AA2;
    case 0x2AA4: goto L2AA4;
    case 0x2AA6: goto L2AA6;
    case 0x2AA8: goto L2AA8;
    case 0x2AAA: goto L2AAA;
    case 0x2AAB: goto L2AAB;
    case 0x2AAC: goto L2AAC;
    case 0x2AAE: goto L2AAE;
    case 0x2AAF: goto L2AAF;
    case 0x2AB0: goto L2AB0;
    case 0x2AB1: goto L2AB1;
    case 0x2AB4: goto L2AB4;
    case 0x2AB6: goto L2AB6;
    case 0x2AB8: goto L2AB8;
    case 0x2ABA: goto L2ABA;
    case 0x2ABC: goto L2ABC;
    case 0x2ABD: goto L2ABD;
    case 0x2ABE: goto L2ABE;
    case 0x2AC1: goto L2AC1;
    case 0x2AC4: goto L2AC4;
    case 0x2AC7: goto L2AC7;
    case 0x2ACB: goto L2ACB;
    case 0x2ACC: goto L2ACC;
    case 0x2ACE: goto L2ACE;
    case 0x2AD1: goto L2AD1;
    case 0x2AD5: goto L2AD5;
    case 0x2AD7: goto L2AD7;
    case 0x2ADB: goto L2ADB;
    case 0x2ADD: goto L2ADD;
    case 0x2ADF: goto L2ADF;
    case 0x2AE1: goto L2AE1;
    case 0x2AE5: goto L2AE5;
    case 0x2AE9: goto L2AE9;
    case 0x2AEC: goto L2AEC;
    case 0x2AED: goto L2AED;
    case 0x2AF1: goto L2AF1;
    case 0x2AF3: goto L2AF3;
    case 0x2AF7: goto L2AF7;
    case 0x2AFB: goto L2AFB;
    case 0x2AFF: goto L2AFF;
    case 0x2B00: goto L2B00;
    case 0x2B01: goto L2B01;
    case 0x2B02: goto L2B02;
    case 0x2B04: goto L2B04;
    case 0x2B05: goto L2B05;
    case 0x2B07: goto L2B07;
    case 0x2B08: goto L2B08;
    case 0x2B0A: goto L2B0A;
    case 0x2B0B: goto L2B0B;
    case 0x2B0D: goto L2B0D;
    case 0x2B0E: goto L2B0E;
    case 0x2B10: goto L2B10;
    case 0x2B11: goto L2B11;
    case 0x2B13: goto L2B13;
    case 0x2B14: goto L2B14;
    case 0x2B16: goto L2B16;
    case 0x2B17: goto L2B17;
    case 0x2B19: goto L2B19;
    case 0x2B1A: goto L2B1A;
    case 0x2B1C: goto L2B1C;
    case 0x2B1D: goto L2B1D;
    case 0x2B1F: goto L2B1F;
    case 0x2B22: goto L2B22;
    case 0x2B24: goto L2B24;
    case 0x2B25: goto L2B25;
    case 0x2B2B: goto L2B2B;
    case 0x2B2C: goto L2B2C;
    case 0x2B2E: goto L2B2E;
    case 0x2B30: goto L2B30;
    case 0x2B31: goto L2B31;
    case 0x2B33: goto L2B33;
    case 0x2B37: goto L2B37;
    case 0x2B3B: goto L2B3B;
    case 0x2B3D: goto L2B3D;
    case 0x2B40: goto L2B40;
    case 0x2B41: goto L2B41;
    case 0x2B43: goto L2B43;
    case 0x2B44: goto L2B44;
    case 0x2B47: goto L2B47;
    case 0x2B4A: goto L2B4A;
    case 0x2B4D: goto L2B4D;
    case 0x2B50: goto L2B50;
    case 0x2B53: goto L2B53;
    case 0x2B56: goto L2B56;
    case 0x2B57: goto L2B57;
    case 0x2B58: goto L2B58;
    case 0x2B5B: goto L2B5B;
    case 0x2B5D: goto L2B5D;
    case 0x2B5E: goto L2B5E;
    case 0x2B5F: goto L2B5F;
    case 0x2B61: goto L2B61;
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
    case 0x2B9B: goto L2B9B;
    case 0x2B9E: goto L2B9E;
    case 0x2BA0: goto L2BA0;
    case 0x2BA3: goto L2BA3;
    case 0x2BA5: goto L2BA5;
    case 0x2BA6: goto L2BA6;
    case 0x2BA7: goto L2BA7;
    case 0x2BA8: goto L2BA8;
    case 0x2BAA: goto L2BAA;
    case 0x2BAC: goto L2BAC;
    case 0x2BAD: goto L2BAD;
    case 0x2BB0: goto L2BB0;
    case 0x2BB2: goto L2BB2;
    case 0x2BB3: goto L2BB3;
    case 0x2BB4: goto L2BB4;
    case 0x2BB5: goto L2BB5;
    case 0x2BB7: goto L2BB7;
    case 0x2BB8: goto L2BB8;
    case 0x2BB9: goto L2BB9;
    case 0x2BBB: goto L2BBB;
    case 0x2BBC: goto L2BBC;
    case 0x2BBD: goto L2BBD;
    case 0x2BBE: goto L2BBE;
    case 0x2BC0: goto L2BC0;
    case 0x2BC1: goto L2BC1;
    case 0x2BC4: goto L2BC4;
    case 0x2BC7: goto L2BC7;
    case 0x2BC8: goto L2BC8;
    case 0x2BCB: goto L2BCB;
    case 0x2BCE: goto L2BCE;
    case 0x2BCF: goto L2BCF;
    case 0x2BD0: goto L2BD0;
    case 0x2BD4: goto L2BD4;
    case 0x2BD6: goto L2BD6;
    case 0x2BD8: goto L2BD8;
    case 0x2BDB: goto L2BDB;
    case 0x2BDD: goto L2BDD;
    case 0x2BDE: goto L2BDE;
    case 0x2BE1: goto L2BE1;
    case 0x2BE3: goto L2BE3;
    case 0x2BE4: goto L2BE4;
    case 0x2BE5: goto L2BE5;
    case 0x2BE6: goto L2BE6;
    case 0x2BE8: goto L2BE8;
    case 0x2BE9: goto L2BE9;
    case 0x2BEA: goto L2BEA;
    case 0x2BEC: goto L2BEC;
    case 0x2BED: goto L2BED;
    case 0x2BEE: goto L2BEE;
    case 0x2BEF: goto L2BEF;
    case 0x2BF1: goto L2BF1;
    case 0x2BF2: goto L2BF2;
    case 0x2BF5: goto L2BF5;
    case 0x2BF7: goto L2BF7;
    case 0x2BF8: goto L2BF8;
    case 0x2BF9: goto L2BF9;
    case 0x2BFC: goto L2BFC;
    case 0x2BFE: goto L2BFE;
    case 0x2C01: goto L2C01;
    case 0x2C03: goto L2C03;
    case 0x2C06: goto L2C06;
    case 0x2C08: goto L2C08;
    case 0x2C0B: goto L2C0B;
    case 0x2C0D: goto L2C0D;
    case 0x2C10: goto L2C10;
    case 0x2C12: goto L2C12;
    case 0x2C15: goto L2C15;
    case 0x2C17: goto L2C17;
    case 0x2C1A: goto L2C1A;
    case 0x2C1E: goto L2C1E;
    case 0x2C1F: goto L2C1F;
    case 0x2C21: goto L2C21;
    case 0x2C23: goto L2C23;
    case 0x2C26: goto L2C26;
    case 0x2C2A: goto L2C2A;
    case 0x2C2D: goto L2C2D;
    case 0x2C30: goto L2C30;
    case 0x2C34: goto L2C34;
    case 0x2C35: goto L2C35;
    case 0x2C37: goto L2C37;
    case 0x2C39: goto L2C39;
    case 0x2C3C: goto L2C3C;
    case 0x2C40: goto L2C40;
    case 0x2C43: goto L2C43;
    case 0x2C44: goto L2C44;
    case 0x2C47: goto L2C47;
    case 0x2C4B: goto L2C4B;
    case 0x2C4C: goto L2C4C;
    case 0x2C4E: goto L2C4E;
    case 0x2C50: goto L2C50;
    case 0x2C53: goto L2C53;
    case 0x2C57: goto L2C57;
    case 0x2C5A: goto L2C5A;
    case 0x2C5D: goto L2C5D;
    case 0x2C61: goto L2C61;
    case 0x2C62: goto L2C62;
    case 0x2C64: goto L2C64;
    case 0x2C66: goto L2C66;
    case 0x2C69: goto L2C69;
    case 0x2C6D: goto L2C6D;
    case 0x2C70: goto L2C70;
    case 0x2C74: goto L2C74;
    case 0x2C76: goto L2C76;
    case 0x2C7A: goto L2C7A;
    case 0x2C7E: goto L2C7E;
    case 0x2C82: goto L2C82;
    case 0x2C86: goto L2C86;
    case 0x2C88: goto L2C88;
    case 0x2C8C: goto L2C8C;
    case 0x2C90: goto L2C90;
    case 0x2C94: goto L2C94;
    case 0x2C95: goto L2C95;
    case 0x2C96: goto L2C96;
    case 0x2C97: goto L2C97;
    case 0x2C98: goto L2C98;
    case 0x2C99: goto L2C99;
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
    case 0x2D83: goto L2D83;
    case 0x2D86: goto L2D86;
    case 0x2D8A: goto L2D8A;
    case 0x2D8B: goto L2D8B;
    case 0x2D8F: goto L2D8F;
    case 0x2D91: goto L2D91;
    case 0x2D93: goto L2D93;
    case 0x2D95: goto L2D95;
    case 0x2D97: goto L2D97;
    case 0x2D99: goto L2D99;
    case 0x2D9B: goto L2D9B;
    case 0x2D9D: goto L2D9D;
    case 0x2D9F: goto L2D9F;
    case 0x2DA1: goto L2DA1;
    case 0x2DA3: goto L2DA3;
    case 0x2DA7: goto L2DA7;
    case 0x2DA9: goto L2DA9;
    case 0x2DAA: goto L2DAA;
    case 0x2DAC: goto L2DAC;
    case 0x2DAD: goto L2DAD;
    case 0x2DAE: goto L2DAE;
    case 0x2DB0: goto L2DB0;
    case 0x2DB2: goto L2DB2;
    case 0x2DB4: goto L2DB4;
    case 0x2DB5: goto L2DB5;
    case 0x2DB7: goto L2DB7;
    case 0x2DB8: goto L2DB8;
    case 0x2DBA: goto L2DBA;
    case 0x2DBE: goto L2DBE;
    case 0x2DC2: goto L2DC2;
    case 0x2DC4: goto L2DC4;
    case 0x2DC6: goto L2DC6;
    case 0x2DC8: goto L2DC8;
    case 0x2DCA: goto L2DCA;
    case 0x2DCC: goto L2DCC;
    case 0x2DCE: goto L2DCE;
    case 0x2DD0: goto L2DD0;
    case 0x2DD1: goto L2DD1;
    case 0x2DD5: goto L2DD5;
    case 0x2DD7: goto L2DD7;
    case 0x2DD8: goto L2DD8;
    case 0x2DD9: goto L2DD9;
    case 0x2DDB: goto L2DDB;
    case 0x2DDC: goto L2DDC;
    case 0x2DDE: goto L2DDE;
    case 0x2DE0: goto L2DE0;
    case 0x2DE2: goto L2DE2;
    case 0x2DE3: goto L2DE3;
    case 0x2DE5: goto L2DE5;
    case 0x2DE6: goto L2DE6;
    case 0x2DE8: goto L2DE8;
    case 0x2DEA: goto L2DEA;
    case 0x2DED: goto L2DED;
    case 0x2DEE: goto L2DEE;
    case 0x2DEF: goto L2DEF;
    case 0x2DF0: goto L2DF0;
    case 0x2DF3: goto L2DF3;
    case 0x2DF6: goto L2DF6;
    case 0x2DF9: goto L2DF9;
    case 0x2DFA: goto L2DFA;
    case 0x2DFD: goto L2DFD;
    case 0x2DFF: goto L2DFF;
    case 0x2E00: goto L2E00;
    case 0x2E02: goto L2E02;
    case 0x2E06: goto L2E06;
    case 0x2E07: goto L2E07;
    case 0x2E0B: goto L2E0B;
    case 0x2E0D: goto L2E0D;
    case 0x2E0F: goto L2E0F;
    case 0x2E11: goto L2E11;
    case 0x2E14: goto L2E14;
    case 0x2E17: goto L2E17;
    case 0x2E1A: goto L2E1A;
    case 0x2E1C: goto L2E1C;
    case 0x2E1E: goto L2E1E;
    case 0x2E20: goto L2E20;
    case 0x2E22: goto L2E22;
    case 0x2E24: goto L2E24;
    case 0x2E25: goto L2E25;
    case 0x2E29: goto L2E29;
    case 0x2E2C: goto L2E2C;
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
    case 0x2F18: goto L2F18;
    case 0x2F1B: goto L2F1B;
    case 0x2F1E: goto L2F1E;
    case 0x2F1F: goto L2F1F;
    case 0x2F22: goto L2F22;
    case 0x2F24: goto L2F24;
    case 0x2F25: goto L2F25;
    case 0x2F29: goto L2F29;
    case 0x2F2B: goto L2F2B;
    case 0x2F2D: goto L2F2D;
    case 0x2F2F: goto L2F2F;
    case 0x2F31: goto L2F31;
    case 0x2F33: goto L2F33;
    case 0x2F35: goto L2F35;
    case 0x2F37: goto L2F37;
    case 0x2F39: goto L2F39;
    case 0x2F3B: goto L2F3B;
    case 0x2F3F: goto L2F3F;
    case 0x2F43: goto L2F43;
    case 0x2F45: goto L2F45;
    case 0x2F46: goto L2F46;
    case 0x2F49: goto L2F49;
    case 0x2F4A: goto L2F4A;
    case 0x2F4C: goto L2F4C;
    case 0x2F4E: goto L2F4E;
    case 0x2F50: goto L2F50;
    case 0x2F53: goto L2F53;
    case 0x2F56: goto L2F56;
    case 0x2F59: goto L2F59;
    case 0x2F5D: goto L2F5D;
    case 0x2F61: goto L2F61;
    case 0x2F63: goto L2F63;
    case 0x2F65: goto L2F65;
    case 0x2F67: goto L2F67;
    case 0x2F69: goto L2F69;
    case 0x2F6B: goto L2F6B;
    case 0x2F6D: goto L2F6D;
    case 0x2F6F: goto L2F6F;
    case 0x2F73: goto L2F73;
    case 0x2F75: goto L2F75;
    case 0x2F79: goto L2F79;
    case 0x2F7A: goto L2F7A;
    case 0x2F7C: goto L2F7C;
    case 0x2F7D: goto L2F7D;
    case 0x2F7F: goto L2F7F;
    case 0x2F80: goto L2F80;
    case 0x2F83: goto L2F83;
    case 0x2F85: goto L2F85;
    case 0x2F86: goto L2F86;
    case 0x2F88: goto L2F88;
    case 0x2F8A: goto L2F8A;
    case 0x2F8D: goto L2F8D;
    case 0x2F90: goto L2F90;
    case 0x2F91: goto L2F91;
    case 0x2F92: goto L2F92;
    case 0x2F93: goto L2F93;
    case 0x2F96: goto L2F96;
    case 0x2F99: goto L2F99;
    case 0x2F9C: goto L2F9C;
    case 0x2F9D: goto L2F9D;
    case 0x2FA0: goto L2FA0;
    case 0x2FA2: goto L2FA2;
    case 0x2FA3: goto L2FA3;
    case 0x2FA5: goto L2FA5;
    case 0x2FA9: goto L2FA9;
    case 0x2FAA: goto L2FAA;
    case 0x2FAE: goto L2FAE;
    case 0x2FB0: goto L2FB0;
    case 0x2FB2: goto L2FB2;
    case 0x2FB4: goto L2FB4;
    case 0x2FB7: goto L2FB7;
    case 0x2FBA: goto L2FBA;
    case 0x2FBD: goto L2FBD;
    case 0x2FBF: goto L2FBF;
    case 0x2FC1: goto L2FC1;
    case 0x2FC3: goto L2FC3;
    case 0x2FC5: goto L2FC5;
    case 0x2FC7: goto L2FC7;
    case 0x2FC8: goto L2FC8;
    case 0x2FCC: goto L2FCC;
    case 0x2FCF: goto L2FCF;
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
    case 0x3094: goto L3094;
    case 0x3098: goto L3098;
    case 0x309A: goto L309A;
    case 0x309E: goto L309E;
    case 0x30A0: goto L30A0;
    case 0x30A4: goto L30A4;
    case 0x30A6: goto L30A6;
    case 0x30AA: goto L30AA;
    case 0x30AC: goto L30AC;
    case 0x30AF: goto L30AF;
    case 0x30B0: goto L30B0;
    case 0x30B4: goto L30B4;
    case 0x30B6: goto L30B6;
    case 0x30BA: goto L30BA;
    case 0x30BE: goto L30BE;
    case 0x30C0: goto L30C0;
    case 0x30C2: goto L30C2;
    case 0x30C4: goto L30C4;
    case 0x30C8: goto L30C8;
    case 0x30C9: goto L30C9;
    case 0x30CC: goto L30CC;
    case 0x30CD: goto L30CD;
    case 0x30CE: goto L30CE;
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
    case 0x30FB: goto L30FB;
    case 0x30FD: goto L30FD;
    case 0x30FF: goto L30FF;
    case 0x3102: goto L3102;
    case 0x3104: goto L3104;
    case 0x3107: goto L3107;
    case 0x3108: goto L3108;
    case 0x3109: goto L3109;
    case 0x310D: goto L310D;
    case 0x310F: goto L310F;
    case 0x3111: goto L3111;
    case 0x3113: goto L3113;
    case 0x3117: goto L3117;
    case 0x311A: goto L311A;
    case 0x311B: goto L311B;
    case 0x311D: goto L311D;
    case 0x311E: goto L311E;
    case 0x3120: goto L3120;
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
    case 0x3195: goto L3195;
    case 0x3197: goto L3197;
    case 0x3199: goto L3199;
    case 0x319B: goto L319B;
    case 0x319F: goto L319F;
    case 0x31A2: goto L31A2;
    case 0x31A6: goto L31A6;
    case 0x31AD: goto L31AD;
    case 0x31B0: goto L31B0;
    case 0x31B2: goto L31B2;
    case 0x31B6: goto L31B6;
    case 0x31BA: goto L31BA;
    case 0x31BD: goto L31BD;
    case 0x31BE: goto L31BE;
    case 0x31C1: goto L31C1;
    case 0x31C4: goto L31C4;
    case 0x31CB: goto L31CB;
    case 0x31CD: goto L31CD;
    case 0x31D3: goto L31D3;
    case 0x31D6: goto L31D6;
    case 0x31D8: goto L31D8;
    case 0x31DC: goto L31DC;
    case 0x31DD: goto L31DD;
    case 0x31DE: goto L31DE;
    case 0x31E0: goto L31E0;
    case 0x31E4: goto L31E4;
    case 0x31E5: goto L31E5;
    case 0x31E6: goto L31E6;
    case 0x31E7: goto L31E7;
    case 0x31EA: goto L31EA;
    case 0x31EB: goto L31EB;
    case 0x31EE: goto L31EE;
    case 0x31EF: goto L31EF;
    case 0x31F0: goto L31F0;
    case 0x31F1: goto L31F1;
    case 0x31F2: goto L31F2;
    case 0x31F3: goto L31F3;
    case 0x31F6: goto L31F6;
    case 0x31F7: goto L31F7;
    case 0x31FA: goto L31FA;
    case 0x31FC: goto L31FC;
    case 0x31FF: goto L31FF;
    case 0x3200: goto L3200;
    case 0x3201: goto L3201;
    case 0x3202: goto L3202;
    case 0x3203: goto L3203;
    case 0x3204: goto L3204;
    default: asm_bad_entry("VIDMODE.ASM", entry);
    }

    /* seg003_0272_21D4  (+21D4)
       show: put a bitmap on the screen. In (from GRCORE's show): AX, BX the screen position, DI, SI
       the source offset and segment, CX, BP the size, 0DC6, 0DC8 the source's x and y offset.
       Chooses the transfer by _Transparency (_5372 opaque, _5467 transparent, GRLIBL.ASM) and joins
       the common path at L2296. The labelled entries after it are the same for other sources and
       targets: _21ED a bitmap already in video memory (_5578 or _568B by plane alignment, _56E3
       transparent), _2214 fbshow (into the frame buffer, GRENTRY's _6F8 or _729), _222B into the
       linear buffer set by _2B62 (_58C7 or _58F8), _2242 vcopyfb (frame buffer to screen, GRENTRY's
       _A7F) and _2250 vcopy (screen to screen, _581B or, for an overlapping copy to the right,
       _586D).

       L2296: clip the rectangle to the window when _ShowClip is set (moving the source offsets to
       match), then write one 8-byte record per row (y, left, right, source offset) into the table at
       0DCB, end it, and jump to the transfer in 0DC2. L2373 and L243F build the records forwards or,
       for vcopy, in the order an overlapping copy needs.

       After L249A's ret is the concave polygon fill (FM Towns concave and uconcave, probably): clip
       with concave_shclip (GRLIBF.ASM), then build the spans of a polygon that may be concave from
       an edge list (1B84 .. 1B8A) into 2D42 and run the span writer. */
L21D4: /* _seg003_0272_21D4 */
    /* 21D4  cmp     byte ptr ds:[_Transparency],0 */
    sub8(rb(pDS, 0xDC5), 0x0, 0);
L21D9:
    /* 21D9  jne     L21E4 */
    if (!ZF) goto L21E4;
L21DB:
    /* 21DB  mov     word ptr ds:[0DC2h],offset _seg003_0272_5372 */
    ww(pDS, 0xDC2, 0x5372);
L21E1:
    /* 21E1  jmp     L2296 */
    goto L2296;
L21E4: /* L21E4 */
    /* 21E4  mov     word ptr ds:[0DC2h],offset _seg003_0272_5467 */
    ww(pDS, 0xDC2, 0x5467);
L21EA:
    /* 21EA  jmp     L2296 */
    goto L2296;
L21ED: /* _seg003_0272_21ED */
    /* 21ED  cmp     byte ptr ds:[_Transparency],0 */
    sub8(rb(pDS, 0xDC5), 0x0, 0);
L21F2:
    /* 21F2  jne     L220B */
    if (!ZF) goto L220B;
L21F4:
    /* 21F4  test    ax,3 */
    logic16((uint16_t)(AX & 0x3));
L21F7:
    /* 21F7  je      L2202 */
    if (ZF) goto L2202;
L21F9:
    /* 21F9  mov     word ptr ds:[0DC2h],offset _seg003_0272_5578 */
    ww(pDS, 0xDC2, 0x5578);
L21FF:
    /* 21FF  jmp     L2296 */
    goto L2296;
L2202: /* L2202 */
    /* 2202  mov     word ptr ds:[0DC2h],offset _seg003_0272_568B */
    ww(pDS, 0xDC2, 0x568B);
L2208:
    /* 2208  jmp     L2296 */
    goto L2296;
L220B: /* L220B */
    /* 220B  mov     word ptr ds:[0DC2h],offset _seg003_0272_56E3 */
    ww(pDS, 0xDC2, 0x56E3);
L2211:
    /* 2211  jmp     L2296 */
    goto L2296;
L2214: /* _seg003_0272_2214 */
    /* 2214  cmp     byte ptr ds:[_Transparency],0 */
    sub8(rb(pDS, 0xDC5), 0x0, 0);
L2219:
    /* 2219  jne     L2223 */
    if (!ZF) goto L2223;
L221B:
    /* 221B  mov     word ptr ds:[0DC2h],offset _seg003_0272_6F8 */
    ww(pDS, 0xDC2, 0x6F8);
L2221:
    /* 2221  jmp     short L2296 */
    goto L2296;
L2223: /* L2223 */
    /* 2223  mov     word ptr ds:[0DC2h],offset _seg003_0272_729 */
    ww(pDS, 0xDC2, 0x729);
L2229:
    /* 2229  jmp     short L2296 */
    goto L2296;
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
    goto L2296;
L223A: /* L223A */
    /* 223A  mov     word ptr ds:[0DC2h],offset _seg003_0272_58F8 */
    ww(pDS, 0xDC2, 0x58F8);
L2240:
    /* 2240  jmp     short L2296 */
    goto L2296;
L2242: /* _seg003_0272_2242 */
    /* 2242  mov     word ptr ds:[0DC2h],offset _seg003_0272_A7F */
    ww(pDS, 0xDC2, 0xA7F);
L2248:
    /* 2248  mov     byte ptr ds:[0DCAh],0 */
    wb(pDS, 0xDCA, 0x0);
L224D:
    /* 224D  jmp     L2373 */
    goto L2373;
L2250: /* _seg003_0272_2250 */
    /* 2250  cmp     bx,di */
    sub16(BX, DI, 0);
L2252:
    /* 2252  je      L2266 */
    if (ZF) goto L2266;
L2254:
    /* 2254  mov     word ptr ds:[0DC2h],offset _seg003_0272_581B */
    ww(pDS, 0xDC2, 0x581B);
L225A:
    /* 225A  jg      L2261 */
    if (!ZF && SF == OF) goto L2261;
L225C:
    /* 225C  mov     bp,offset L2373 */
    BP = 0x2373;
L225F:
    /* 225F  jmp     short L227B */
    goto L227B;
L2261: /* L2261 */
    /* 2261  mov     bp,offset L243F */
    BP = 0x243F;
L2264:
    /* 2264  jmp     short L227B */
    goto L227B;
L2266: /* L2266 */
    /* 2266  mov     bp,offset L2373 */
    BP = 0x2373;
L2269:
    /* 2269  cmp     ax,si */
    sub16(AX, SI, 0);
L226B:
    /* 226B  jg      L2275 */
    if (!ZF && SF == OF) goto L2275;
L226D:
    /* 226D  mov     word ptr ds:[0DC2h],offset _seg003_0272_586D */
    ww(pDS, 0xDC2, 0x586D);
L2273:
    /* 2273  jmp     short L227B */
    goto L227B;
L2275: /* L2275 */
    /* 2275  mov     word ptr ds:[0DC2h],offset _seg003_0272_581B */
    ww(pDS, 0xDC2, 0x581B);
L227B: /* L227B */
    /* 227B  cmp     bp,offset L243F */
    sub16(BP, 0x243F, 0);
L227F:
    /* 227F  jne     L2284 */
    if (!ZF) goto L2284;
L2281:
    /* 2281  sub     di,dx */
    DI = (uint16_t)(DI - DX);
L2283:
    /* 2283  inc     di */
    DI = (uint16_t)(DI + 1);
L2284: /* L2284 */
    /* 2284  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L2286:
    /* 2286  mov     di,word ptr [di+36ACh] */
    DI = rw(pDS, DI + 0x36AC);
L228A:
    /* 228A  shr     si,2 */
    SI = (uint16_t)(SI >> 2);
L228D:
    /* 228D  add     di,si */
    DI = add16(DI, SI, 0);
L228F:
    /* 228F  mov     byte ptr ds:[0DCAh],1 */
    wb(pDS, 0xDCA, 0x1);
L2294:
    /* 2294  jmp     bp */
    return ASM_JMP(0x0085, BP);
L2296: /* L2296 */
    /* 2296  mov     word ptr ds:[55ECh],si */
    ww(pDS, 0x55EC, SI);
L229A:
    /* 229A  push    di */
    push16(DI);
L229B:
    /* 229B  mov     di,word ptr ds:[141Bh] */
    DI = rw(pDS, 0x141B);
L229F:
    /* 229F  and     word ptr [di],7FFFh */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) & 0x7FFF));
L22A3:
    /* 22A3  mov     bp,word ptr ds:[0DC6h] */
    BP = rw(pDS, 0xDC6);
L22A7:
    /* 22A7  mov     di,word ptr ds:[0DC8h] */
    DI = rw(pDS, 0xDC8);
L22AB:
    /* 22AB  push    cx */
    push16(CX);
L22AC:
    /* 22AC  sub     cx,bp */
    CX = (uint16_t)(CX - BP);
L22AE:
    /* 22AE  sub     dx,di */
    DX = (uint16_t)(DX - DI);
L22B0:
    /* 22B0  cmp     byte ptr ds:[_ShowClip],0 */
    sub8(rb(pDS, 0xDC4), 0x0, 0);
L22B5:
    /* 22B5  jne     L22B9 */
    if (!ZF) goto L22B9;
L22B7:
    /* 22B7  jmp     short L230E */
    goto L230E;
L22B9: /* L22B9 */
    /* 22B9  mov     si,ax */
    SI = AX;
L22BB:
    /* 22BB  add     si,cx */
    SI = (uint16_t)(SI + CX);
L22BD:
    /* 22BD  dec     si */
    SI = (uint16_t)(SI - 1);
L22BE:
    /* 22BE  sub     si,word ptr ds:[3DF4h] */
    SI = sub16(SI, rw(pDS, 0x3DF4), 0);
L22C2:
    /* 22C2  jge     L22C7 */
    if (SF == OF) goto L22C7;
L22C4:
    /* 22C4  jmp     L236F */
    goto L236F;
L22C7: /* L22C7 */
    /* 22C7  mov     si,word ptr ds:[3DF4h] */
    SI = rw(pDS, 0x3DF4);
L22CB:
    /* 22CB  sub     si,ax */
    SI = sub16(SI, AX, 0);
L22CD:
    /* 22CD  jle     L22D5 */
    if (ZF || SF != OF) goto L22D5;
L22CF:
    /* 22CF  add     ax,si */
    AX = (uint16_t)(AX + SI);
L22D1:
    /* 22D1  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L22D3:
    /* 22D3  add     bp,si */
    BP = (uint16_t)(BP + SI);
L22D5: /* L22D5 */
    /* 22D5  mov     si,word ptr ds:[3DF8h] */
    SI = rw(pDS, 0x3DF8);
L22D9:
    /* 22D9  sub     si,ax */
    SI = sub16(SI, AX, 0);
L22DB:
    /* 22DB  jge     L22E0 */
    if (SF == OF) goto L22E0;
L22DD:
    /* 22DD  jmp     L236F */
    goto L236F;
L22E0: /* L22E0 */
    /* 22E0  inc     si */
    SI = (uint16_t)(SI + 1);
L22E1:
    /* 22E1  cmp     cx,si */
    sub16(CX, SI, 0);
L22E3:
    /* 22E3  jle     L22E7 */
    if (ZF || SF != OF) goto L22E7;
L22E5:
    /* 22E5  mov     cx,si */
    CX = SI;
L22E7: /* L22E7 */
    /* 22E7  mov     si,bx */
    SI = BX;
L22E9:
    /* 22E9  sub     si,dx */
    SI = (uint16_t)(SI - DX);
L22EB:
    /* 22EB  inc     si */
    SI = (uint16_t)(SI + 1);
L22EC:
    /* 22EC  sub     si,word ptr ds:[3DF6h] */
    SI = sub16(SI, rw(pDS, 0x3DF6), 0);
L22F0:
    /* 22F0  jg      L236F */
    if (!ZF && SF == OF) goto L236F;
L22F2:
    /* 22F2  mov     si,word ptr ds:[3DF6h] */
    SI = rw(pDS, 0x3DF6);
L22F6:
    /* 22F6  sub     si,bx */
    SI = sub16(SI, BX, 0);
L22F8:
    /* 22F8  jge     L2300 */
    if (SF == OF) goto L2300;
L22FA:
    /* 22FA  sub     di,si */
    DI = (uint16_t)(DI - SI);
L22FC:
    /* 22FC  add     dx,si */
    DX = (uint16_t)(DX + SI);
L22FE:
    /* 22FE  add     bx,si */
    BX = (uint16_t)(BX + SI);
L2300: /* L2300 */
    /* 2300  mov     si,bx */
    SI = BX;
L2302:
    /* 2302  sub     si,word ptr ds:[3DFAh] */
    SI = sub16(SI, rw(pDS, 0x3DFA), 0);
L2306:
    /* 2306  jle     L236F */
    if (ZF || SF != OF) goto L236F;
L2308:
    /* 2308  cmp     dx,si */
    sub16(DX, SI, 0);
L230A:
    /* 230A  jle     L230E */
    if (ZF || SF != OF) goto L230E;
L230C:
    /* 230C  mov     dx,si */
    DX = SI;
L230E: /* L230E */
    /* 230E  mov     si,cx */
    SI = CX;
L2310:
    /* 2310  pop     cx */
    CX = pop16();
L2311:
    /* 2311  xchg    cx,dx */
    { uint16_t t_ = DX;
    DX = CX;
    CX = t_; }
L2313:
    /* 2313  neg     bx */
    BX = (uint16_t)-BX;
L2315:
    /* 2315  add     bx,0C7h */
    BX = (uint16_t)(BX + 0xC7);
L2319:
    /* 2319  shl     bx,3 */
    BX = (uint16_t)(BX << 3);
L231C:
    /* 231C  add     bx,0DCBh */
    BX = (uint16_t)(BX + 0xDCB);
L2320:
    /* 2320  add     si,ax */
    SI = (uint16_t)(SI + AX);
L2322:
    /* 2322  dec     si */
    SI = (uint16_t)(SI - 1);
L2323:
    /* 2323  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L2324:
    /* 2324  push    bp */
    push16(BP);
L2325:
    /* 2325  mov     bp,dx */
    BP = DX;
L2327:
    /* 2327  mul     dx */
    mul16(DX);
L2329:
    /* 2329  pop     dx */
    DX = pop16();
L232A:
    /* 232A  cmp     word ptr ds:[55ECh],0A000h */
    sub16(rw(pDS, 0x55EC), 0xA000, 0);
L2330:
    /* 2330  je      L233B */
    if (ZF) goto L233B;
L2332:
    /* 2332  add     dx,ax */
    DX = (uint16_t)(DX + AX);
L2334:
    /* 2334  pop     ax */
    AX = pop16();
L2335:
    /* 2335  add     dx,ax */
    DX = (uint16_t)(DX + AX);
L2337:
    /* 2337  mov     ax,bx */
    AX = BX;
L2339:
    /* 2339  jmp     short L2351 */
    goto L2351;
L233B: /* L233B */
    /* 233B  add     ax,3 */
    AX = (uint16_t)(AX + 0x3);
L233E:
    /* 233E  shr     dx,2 */
    DX = (uint16_t)(DX >> 2);
L2341:
    /* 2341  shr     ax,2 */
    AX = (uint16_t)(AX >> 2);
L2344:
    /* 2344  add     dx,ax */
    DX = (uint16_t)(DX + AX);
L2346:
    /* 2346  pop     ax */
    AX = pop16();
L2347:
    /* 2347  add     dx,ax */
    DX = (uint16_t)(DX + AX);
L2349:
    /* 2349  mov     ax,bx */
    AX = BX;
L234B:
    /* 234B  add     bp,3 */
    BP = (uint16_t)(BP + 0x3);
L234E:
    /* 234E  shr     bp,2 */
    BP = (uint16_t)(BP >> 2);
L2351: /* L2351 */
    /* 2351  mov     word ptr [bx+2],di */
    ww(pDS, BX + 0x2, DI);
L2354:
    /* 2354  mov     word ptr [bx+4],si */
    ww(pDS, BX + 0x4, SI);
L2357:
    /* 2357  mov     word ptr [bx+6],dx */
    ww(pDS, BX + 0x6, DX);
L235A:
    /* 235A  add     dx,bp */
    DX = (uint16_t)(DX + BP);
L235C:
    /* 235C  add     bx,8 */
    BX = (uint16_t)(BX + 0x8);
L235F:
    /* 235F  loop    L2351 */
    if (--CX) goto L2351;
L2361:
    /* 2361  or      word ptr [bx],8000h */
    ww(pDS, BX, logic16((uint16_t)(rw(pDS, BX) | 0x8000)));
L2365:
    /* 2365  mov     word ptr ds:[141Bh],bx */
    ww(pDS, 0x141B, BX);
L2369:
    /* 2369  mov     si,ax */
    SI = AX;
L236B:
    /* 236B  jmp     word ptr ds:[0DC2h] */
    return ASM_JMP(0x0085, rw(pDS, 0xDC2));
L236F: /* L236F */
    /* 236F  add     sp,4 */
    SP = add16(SP, 0x4, 0);
L2372:
    /* 2372  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2373: /* L2373 */
    /* 2373  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L2375:
    /* 2375  dec     cx */
    CX = (uint16_t)(CX - 1);
L2376:
    /* 2376  and     ax,0FFFCh */
    AX = (uint16_t)(AX & 0xFFFC);
L2379:
    /* 2379  and     cx,-4 */
    CX = (uint16_t)(CX & 0xFFFC);
L237C:
    /* 237C  add     cx,4 */
    CX = (uint16_t)(CX + 0x4);
L237F:
    /* 237F  sub     cx,ax */
    CX = (uint16_t)(CX - AX);
L2381:
    /* 2381  mov     si,cx */
    SI = CX;
L2383:
    /* 2383  shr     si,2 */
    SI = (uint16_t)(SI >> 2);
L2386:
    /* 2386  push    si */
    push16(SI);
L2387:
    /* 2387  cmp     byte ptr ds:[_ShowClip],0 */
    sub8(rb(pDS, 0xDC4), 0x0, 0);
L238C:
    /* 238C  je      L23F7 */
    if (ZF) goto L23F7;
L238E:
    /* 238E  mov     si,word ptr ds:[3DF6h] */
    SI = rw(pDS, 0x3DF6);
L2392:
    /* 2392  mov     bp,bx */
    BP = BX;
L2394:
    /* 2394  sub     bp,dx */
    BP = (uint16_t)(BP - DX);
L2396:
    /* 2396  inc     bp */
    BP = (uint16_t)(BP + 1);
L2397:
    /* 2397  cmp     si,bp */
    sub16(SI, BP, 0);
L2399:
    /* 2399  jge     L239E */
    if (SF == OF) goto L239E;
L239B:
    /* 239B  jmp     L243B */
    goto L243B;
L239E: /* L239E */
    /* 239E  sub     si,bx */
    SI = sub16(SI, BX, 0);
L23A0:
    /* 23A0  jge     L23B7 */
    if (SF == OF) goto L23B7;
L23A2:
    /* 23A2  add     dx,si */
    DX = (uint16_t)(DX + SI);
L23A4:
    /* 23A4  add     bx,si */
    BX = (uint16_t)(BX + SI);
L23A6:
    /* 23A6  push    ax */
    push16(AX);
L23A7:
    /* 23A7  push    dx */
    push16(DX);
L23A8:
    /* 23A8  mov     dx,cx */
    DX = CX;
L23AA:
    /* 23AA  shr     dx,2 */
    DX = (uint16_t)(DX >> 2);
L23AD:
    /* 23AD  mov     ax,si */
    AX = SI;
L23AF:
    /* 23AF  neg     ax */
    AX = (uint16_t)-AX;
L23B1:
    /* 23B1  mul     dx */
    mul16(DX);
L23B3:
    /* 23B3  add     di,ax */
    DI = (uint16_t)(DI + AX);
L23B5:
    /* 23B5  pop     dx */
    DX = pop16();
L23B6:
    /* 23B6  pop     ax */
    AX = pop16();
L23B7: /* L23B7 */
    /* 23B7  mov     si,bx */
    SI = BX;
L23B9:
    /* 23B9  sub     si,word ptr ds:[3DFAh] */
    SI = sub16(SI, rw(pDS, 0x3DFA), 0);
L23BD:
    /* 23BD  jle     L243B */
    if (ZF || SF != OF) goto L243B;
L23BF:
    /* 23BF  cmp     dx,si */
    sub16(DX, SI, 0);
L23C1:
    /* 23C1  jle     L23C5 */
    if (ZF || SF != OF) goto L23C5;
L23C3:
    /* 23C3  mov     dx,si */
    DX = SI;
L23C5: /* L23C5 */
    /* 23C5  mov     si,word ptr ds:[3DF8h] */
    SI = rw(pDS, 0x3DF8);
L23C9:
    /* 23C9  sub     si,ax */
    SI = sub16(SI, AX, 0);
L23CB:
    /* 23CB  jl      L243B */
    if (SF != OF) goto L243B;
L23CD:
    /* 23CD  inc     si */
    SI = (uint16_t)(SI + 1);
L23CE:
    /* 23CE  cmp     cx,si */
    sub16(CX, SI, 0);
L23D0:
    /* 23D0  jle     L23D4 */
    if (ZF || SF != OF) goto L23D4;
L23D2:
    /* 23D2  mov     cx,si */
    CX = SI;
L23D4: /* L23D4 */
    /* 23D4  mov     si,word ptr ds:[3DF4h] */
    SI = rw(pDS, 0x3DF4);
L23D8:
    /* 23D8  mov     bp,ax */
    BP = AX;
L23DA:
    /* 23DA  add     bp,cx */
    BP = (uint16_t)(BP + CX);
L23DC:
    /* 23DC  dec     bp */
    BP = (uint16_t)(BP - 1);
L23DD:
    /* 23DD  cmp     si,bp */
    sub16(SI, BP, 0);
L23DF:
    /* 23DF  jg      L243B */
    if (!ZF && SF == OF) goto L243B;
L23E1:
    /* 23E1  sub     si,ax */
    SI = sub16(SI, AX, 0);
L23E3:
    /* 23E3  jle     L23F7 */
    if (ZF || SF != OF) goto L23F7;
L23E5:
    /* 23E5  add     ax,si */
    AX = (uint16_t)(AX + SI);
L23E7:
    /* 23E7  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L23E9:
    /* 23E9  mov     bp,si */
    BP = SI;
L23EB:
    /* 23EB  neg     bp */
    BP = (uint16_t)-BP;
L23ED:
    /* 23ED  and     bp,3 */
    BP = (uint16_t)(BP & 0x3);
L23F0:
    /* 23F0  add     si,bp */
    SI = (uint16_t)(SI + BP);
L23F2:
    /* 23F2  sar     si,2 */
    SI = (uint16_t)((int16_t)SI >> 2);
L23F5:
    /* 23F5  add     di,si */
    DI = (uint16_t)(DI + SI);
L23F7: /* L23F7 */
    /* 23F7  mov     bp,word ptr ds:[141Bh] */
    BP = rw(pDS, 0x141B);
L23FB:
    /* 23FB  and     word ptr [bp],7FFFh */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) & 0x7FFF));
L2400:
    /* 2400  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L2402:
    /* 2402  dec     cx */
    CX = (uint16_t)(CX - 1);
L2403:
    /* 2403  neg     bx */
    BX = (uint16_t)-BX;
L2405:
    /* 2405  add     bx,0C7h */
    BX = (uint16_t)(BX + 0xC7);
L2409:
    /* 2409  shl     bx,3 */
    BX = (uint16_t)(BX << 3);
L240C:
    /* 240C  add     bx,0DCBh */
    BX = (uint16_t)(BX + 0xDCB);
L2410:
    /* 2410  mov     si,bx */
    SI = BX;
L2412:
    /* 2412  pop     bp */
    BP = pop16();
L2413:
    /* 2413  cmp     byte ptr ds:[0DCAh],0 */
    sub8(rb(pDS, 0xDCA), 0x0, 0);
L2418:
    /* 2418  je      L241E */
    if (ZF) goto L241E;
L241A:
    /* 241A  mov     bp,word ptr ds:[36A8h] */
    BP = rw(pDS, 0x36A8);
L241E: /* L241E */
    /* 241E  mov     word ptr [bx+2],ax */
    ww(pDS, BX + 0x2, AX);
L2421:
    /* 2421  mov     word ptr [bx+4],cx */
    ww(pDS, BX + 0x4, CX);
L2424:
    /* 2424  mov     word ptr [bx+6],di */
    ww(pDS, BX + 0x6, DI);
L2427:
    /* 2427  add     di,bp */
    DI = (uint16_t)(DI + BP);
L2429:
    /* 2429  add     bx,8 */
    BX = (uint16_t)(BX + 0x8);
L242C:
    /* 242C  dec     dx */
    DX = dec16(DX);
L242D:
    /* 242D  jne     L241E */
    if (!ZF) goto L241E;
L242F:
    /* 242F  or      word ptr [bx],8000h */
    ww(pDS, BX, logic16((uint16_t)(rw(pDS, BX) | 0x8000)));
L2433:
    /* 2433  mov     word ptr ds:[141Bh],bx */
    ww(pDS, 0x141B, BX);
L2437:
    /* 2437  jmp     word ptr ds:[0DC2h] */
    return ASM_JMP(0x0085, rw(pDS, 0xDC2));
L243B: /* L243B */
    /* 243B  add     sp,2 */
    SP = add16(SP, 0x2, 0);
L243E:
    /* 243E  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L243F: /* L243F */
    /* 243F  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L2441:
    /* 2441  dec     cx */
    CX = (uint16_t)(CX - 1);
L2442:
    /* 2442  and     ax,0FFFCh */
    AX = (uint16_t)(AX & 0xFFFC);
L2445:
    /* 2445  and     cx,-4 */
    CX = (uint16_t)(CX & 0xFFFC);
L2448:
    /* 2448  add     cx,4 */
    CX = (uint16_t)(CX + 0x4);
L244B:
    /* 244B  sub     cx,ax */
    CX = (uint16_t)(CX - AX);
L244D:
    /* 244D  mov     bp,cx */
    BP = CX;
L244F:
    /* 244F  shr     bp,2 */
    BP = (uint16_t)(BP >> 2);
L2452:
    /* 2452  mov     si,word ptr ds:[141Bh] */
    SI = rw(pDS, 0x141B);
L2456:
    /* 2456  and     word ptr [si],7FFFh */
    ww(pDS, SI, (uint16_t)(rw(pDS, SI) & 0x7FFF));
L245A:
    /* 245A  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L245C:
    /* 245C  dec     cx */
    CX = (uint16_t)(CX - 1);
L245D:
    /* 245D  sub     bx,dx */
    BX = (uint16_t)(BX - DX);
L245F:
    /* 245F  inc     bx */
    BX = (uint16_t)(BX + 1);
L2460:
    /* 2460  mov     si,0DCBh */
    SI = 0xDCB;
L2463:
    /* 2463  push    dx */
    push16(DX);
L2464:
    /* 2464  mov     bp,word ptr ds:[36A8h] */
    BP = rw(pDS, 0x36A8);
L2468: /* L2468 */
    /* 2468  mov     word ptr [si],bx */
    ww(pDS, SI, BX);
L246A:
    /* 246A  mov     word ptr [si+2],ax */
    ww(pDS, SI + 0x2, AX);
L246D:
    /* 246D  mov     word ptr [si+4],cx */
    ww(pDS, SI + 0x4, CX);
L2470:
    /* 2470  mov     word ptr [si+6],di */
    ww(pDS, SI + 0x6, DI);
L2473:
    /* 2473  inc     bx */
    BX = (uint16_t)(BX + 1);
L2474:
    /* 2474  sub     di,bp */
    DI = (uint16_t)(DI - BP);
L2476:
    /* 2476  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L2479:
    /* 2479  dec     dx */
    DX = dec16(DX);
L247A:
    /* 247A  jne     L2468 */
    if (!ZF) goto L2468;
L247C:
    /* 247C  or      word ptr [si],8000h */
    ww(pDS, SI, (uint16_t)(rw(pDS, SI) | 0x8000));
L2480:
    /* 2480  mov     word ptr ds:[141Bh],si */
    ww(pDS, 0x141B, SI);
L2484:
    /* 2484  mov     si,0DCBh */
    SI = 0xDCB;
L2487:
    /* 2487  call    word ptr ds:[0DC2h] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, 0xDC2)), 0x248B)) != 0) return c;
L248B:
    /* 248B  pop     dx */
    DX = pop16();
L248C:
    /* 248C  mov     bx,0DCBh */
    BX = 0xDCB;
L248F:
    /* 248F  mov     ax,0C7h */
    AX = 0xC7;
L2492: /* L2492 */
    /* 2492  mov     word ptr [bx],ax */
    ww(pDS, BX, AX);
L2494:
    /* 2494  dec     ax */
    AX = (uint16_t)(AX - 1);
L2495:
    /* 2495  add     bx,8 */
    BX = add16(BX, 0x8, 0);
L2498:
    /* 2498  dec     dx */
    DX = dec16(DX);
L2499:
    /* 2499  jne     L2492 */
    if (!ZF) goto L2492;
L249B:
    /* 249B  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
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

    /* seg003_0272_283D  (+283D)
       init_graphics: needs a VGA (_2B7A, else returns with carry set). Records the CPU type from
       FD71:0110, sets mode X (SetVideoMode), saves the CRTC maximum scan line, builds the edge mask
       tables, and falls into the mode set. _2869 sets the mode number AX first; _286C applies 3DEA:
       copies the mode's size to 3DEC and to FD71:0C38, sets the scan doubling (the saved value for
       mode 0, 1 line per row otherwise), sets the window to the whole screen, works out the row
       length and the two pages and the video memory left after them (4106, 410A), builds Ytab,
       initialises the pens, flips to page 0, and builds the text masks (GRLIBI's _3BE2). */
L283D: /* _seg003_0272_283D */
    /* 283D  push    ds */
    push16(asm_ds);
L283E:
    /* 283E  mov     ax,seg dseg062_62a6 */
    AX = (uint16_t)(0x60B9 + PORT_LOAD_SEG);
L2841:
    /* 2841  mov     ds,ax */
    SET_DS(AX);
L2843:
    /* 2843  mov     ax,word ptr ds:[110h] */
    AX = rw(pDS, 0x110);
L2846:
    /* 2846  pop     ds */
    SET_DS(pop16());
L2847:
    /* 2847  mov     word ptr ds:[36A2h],ax */
    ww(pDS, 0x36A2, AX);
L284A:
    /* 284A  call    _seg003_0272_2B7A */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2B7A), 0x284D)) != 0) return c;
L284D:
    /* 284D  jae     L2850 */
    if (!CF) goto L2850;
L284F:
    /* 284F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2850: /* L2850 */
    /* 2850  mov     word ptr ds:[36A4h],0FFFFh */
    ww(pDS, 0x36A4, 0xFFFF);
L2856:
    /* 2856  call    _SetVideoMode_seg003_0272_2B9B */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2B9B), 0x2859)) != 0) return c;
L2859:
    /* 2859  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L285C:
    /* 285C  mov     al,9 */
    AL = 0x9;
L285E:
    /* 285E  out     dx,al */
    asm_out8(DX, AL);
L285F:
    /* 285F  inc     dx */
    DX = (uint16_t)(DX + 1);
L2860:
    /* 2860  in      al,dx */
    AL = asm_in8(DX);
L2861:
    /* 2861  mov     byte ptr ds:[2D35h],al */
    wb(pDS, 0x2D35, AL);
L2864:
    /* 2864  call    _seg003_0272_2B44 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2B44), 0x2867)) != 0) return c;
L2867:
    /* 2867  jmp     short _seg003_0272_286C */
    goto L286C;
L2869: /* _seg003_0272_2869 */
    /* 2869  mov     word ptr ds:[3DEAh],ax */
    ww(pDS, 0x3DEA, AX);
L286C: /* _seg003_0272_286C */
    /* 286C  mov     bx,word ptr ds:[3DEAh] */
    BX = rw(pDS, 0x3DEA);
L2870:
    /* 2870  cmp     bx,word ptr ds:[2CD8h] */
    sub16(BX, rw(pDS, 0x2CD8), 0);
L2874:
    /* 2874  jne     L2877 */
    if (!ZF) goto L2877;
L2876:
    /* 2876  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2877: /* L2877 */
    /* 2877  mov     word ptr ds:[2CD8h],bx */
    ww(pDS, 0x2CD8, BX);
L287B:
    /* 287B  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L287D:
    /* 287D  mov     ax,bx */
    AX = BX;
L287F:
    /* 287F  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L2881:
    /* 2881  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L2883:
    /* 2883  lea     si,[bx+2CDEh] */
    SI = (uint16_t)(BX + 0x2CDE);
L2887:
    /* 2887  mov     di,3DECh */
    DI = 0x3DEC;
L288A:
    /* 288A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L288B:
    /* 288B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L288C:
    /* 288C  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L288D:
    /* 288D  mov     si,3DEAh */
    SI = 0x3DEA;
L2890:
    /* 2890  push    es */
    push16(asm_es);
L2891:
    /* 2891  mov     bx,seg dseg062_62a6 */
    BX = (uint16_t)(0x60B9 + PORT_LOAD_SEG);
L2894:
    /* 2894  mov     es,bx */
    SET_ES(BX);
L2896:
    /* 2896  mov     di,0C38h */
    DI = 0xC38;
L2899:
    /* 2899  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L289A:
    /* 289A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L289B:
    /* 289B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L289C:
    /* 289C  pop     es */
    SET_ES(pop16());
L289D:
    /* 289D  test    ax,ax */
    logic16((uint16_t)(AX & AX));
L289F:
    /* 289F  jne     L28AE */
    if (!ZF) goto L28AE;
L28A1:
    /* 28A1  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L28A4:
    /* 28A4  mov     al,9 */
    AL = 0x9;
L28A6:
    /* 28A6  out     dx,al */
    asm_out8(DX, AL);
L28A7:
    /* 28A7  inc     dx */
    DX = (uint16_t)(DX + 1);
L28A8:
    /* 28A8  mov     al,byte ptr ds:[2D35h] */
    AL = rb(pDS, 0x2D35);
L28AB:
    /* 28AB  out     dx,al */
    asm_out8(DX, AL);
L28AC:
    /* 28AC  jmp     short L28B9 */
    goto L28B9;
L28AE: /* L28AE */
    /* 28AE  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L28B1:
    /* 28B1  mov     al,9 */
    AL = 0x9;
L28B3:
    /* 28B3  out     dx,al */
    asm_out8(DX, AL);
L28B4:
    /* 28B4  inc     dx */
    DX = (uint16_t)(DX + 1);
L28B5:
    /* 28B5  in      al,dx */
    AL = asm_in8(DX);
L28B6:
    /* 28B6  and     al,0E0h */
    AL = (uint8_t)(AL & 0xE0);
L28B8:
    /* 28B8  out     dx,al */
    asm_out8(DX, AL);
L28B9: /* L28B9 */
    /* 28B9  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L28BB:
    /* 28BB  mov     word ptr ds:[3DF4h],ax */
    ww(pDS, 0x3DF4, AX);
L28BE:
    /* 28BE  mov     word ptr ds:[3DFAh],ax */
    ww(pDS, 0x3DFA, AX);
L28C1:
    /* 28C1  mov     ax,word ptr ds:[3DECh] */
    AX = rw(pDS, 0x3DEC);
L28C4:
    /* 28C4  mov     word ptr ds:[3DF8h],ax */
    ww(pDS, 0x3DF8, AX);
L28C7:
    /* 28C7  mov     ax,word ptr ds:[3DEEh] */
    AX = rw(pDS, 0x3DEE);
L28CA:
    /* 28CA  mov     word ptr ds:[3DF6h],ax */
    ww(pDS, 0x3DF6, AX);
L28CD:
    /* 28CD  mov     si,3DF4h */
    SI = 0x3DF4;
L28D0:
    /* 28D0  call    _seg003_0272_31E7 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x31E7), 0x28D3)) != 0) return c;
L28D3:
    /* 28D3  mov     ax,word ptr ds:[3DECh] */
    AX = rw(pDS, 0x3DEC);
L28D6:
    /* 28D6  inc     ax */
    AX = (uint16_t)(AX + 1);
L28D7:
    /* 28D7  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L28D9:
    /* 28D9  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L28DB:
    /* 28DB  mov     word ptr ds:[36A8h],ax */
    ww(pDS, 0x36A8, AX);
L28DE:
    /* 28DE  mov     ax,word ptr ds:[3DEEh] */
    AX = rw(pDS, 0x3DEE);
L28E1:
    /* 28E1  inc     ax */
    AX = (uint16_t)(AX + 1);
L28E2:
    /* 28E2  mul     word ptr ds:[36A8h] */
    mul16(rw(pDS, 0x36A8));
L28E6:
    /* 28E6  mov     bx,ax */
    BX = AX;
L28E8:
    /* 28E8  add     ax,0FFh */
    AX = (uint16_t)(AX + 0xFF);
L28EB:
    /* 28EB  sub     al,al */
    AL = (uint8_t)(AL - AL);
L28ED:
    /* 28ED  mov     word ptr ds:[2CD4h],ax */
    ww(pDS, 0x2CD4, AX);
L28F0:
    /* 28F0  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L28F2:
    /* 28F2  mov     word ptr ds:[2CD6h],ax */
    ww(pDS, 0x2CD6, AX);
L28F5:
    /* 28F5  add     ax,word ptr ds:[36A8h] */
    AX = (uint16_t)(AX + rw(pDS, 0x36A8));
L28F9:
    /* 28F9  mov     word ptr ds:[4106h],ax */
    ww(pDS, 0x4106, AX);
L28FC:
    /* 28FC  mov     word ptr ds:[410Ah],ax */
    ww(pDS, 0x410A, AX);
L28FF:
    /* 28FF  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L2901:
    /* 2901  call    _seg003_0272_2AED */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2AED), 0x2904)) != 0) return c;
L2904:
    /* 2904  call    _seg003_0272_31BE */
    if ((c = asm_call(ASM_JMP(0x0085, 0x31BE), 0x2907)) != 0) return c;
L2907:
    /* 2907  call    _seg003_0272_2ABE */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2ABE), 0x290A)) != 0) return c;
L290A:
    /* 290A  call    _nullsub_2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x43F4), 0x290D)) != 0) return c;
L290D:
    /* 290D  mov     ax,0 */
    AX = 0x0;
L2910:
    /* 2910  call    _seg003_0272_3BE2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3BE2), 0x2913)) != 0) return c;
L2913:
    /* 2913  ret */
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

    /* seg003_0272_295F  (+295F)
       _295F: set the CRTC display start (registers 0Ch, 0Dh) to the drawing page, Ytab of the
       highest row. */
L295F: /* _seg003_0272_295F */
    /* 295F  mov     bx,word ptr ds:[3DEEh] */
    BX = rw(pDS, 0x3DEE);
L2963:
    /* 2963  shl     bx,1 */
    BX = shl16(BX, 1);
L2965:
    /* 2965  mov     ax,word ptr [bx+36ACh] */
    AX = rw(pDS, BX + 0x36AC);
L2969:
    /* 2969  mov     bx,ax */
    BX = AX;
L296B:
    /* 296B  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L296E:
    /* 296E  mov     al,0Ch */
    AL = 0xC;
L2970:
    /* 2970  out     dx,ax */
    asm_out16(DX, AX);
L2971:
    /* 2971  mov     ah,bl */
    AH = BL;
L2973:
    /* 2973  inc     al */
    AL = inc8(AL);
L2975:
    /* 2975  out     dx,ax */
    asm_out16(DX, AX);
L2976:
    /* 2976  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2977  (+2977)
       _2977: set up a virtual screen with a split (GRCORE's _4A3A). AX = the virtual width in pixels
       (CRTC offset 13h), BX = the line where the split starts (negative: no split), CX = the height
       of the scrolling part. Turns on the attribute controller's pixel panning compatibility (so the
       part below the split does not pan), sets the line compare (_2A8F) and the display start, and
       rebuilds Ytab for both parts (_2B2C). Used with vscreen_focus for a scrolling picture;
       probably the cutscenes or the map (not traced). */
L2977: /* _seg003_0272_2977 */
    /* 2977  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L2979:
    /* 2979  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L297B:
    /* 297B  mov     word ptr ds:[36A8h],ax */
    ww(pDS, 0x36A8, AX);
L297E:
    /* 297E  mov     word ptr ds:[2CDCh],bx */
    ww(pDS, 0x2CDC, BX);
L2982:
    /* 2982  mov     word ptr ds:[2CDAh],cx */
    ww(pDS, 0x2CDA, CX);
L2986:
    /* 2986  mov     bx,cx */
    BX = CX;
L2988:
    /* 2988  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L298A:
    /* 298A  mov     ah,al */
    AH = AL;
L298C:
    /* 298C  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L298F:
    /* 298F  mov     al,13h */
    AL = 0x13;
L2991:
    /* 2991  out     dx,al */
    asm_out8(DX, AL);
L2992:
    /* 2992  inc     dx */
    DX = (uint16_t)(DX + 1);
L2993:
    /* 2993  mov     al,ah */
    AL = AH;
L2995:
    /* 2995  out     dx,al */
    asm_out8(DX, AL);
L2996:
    /* 2996  mov     dx,ATTR_INDEX */
    DX = 0x3C0;
L2999:
    /* 2999  mov     al,30h */
    AL = 0x30;
L299B:
    /* 299B  out     dx,al */
    asm_out8(DX, AL);
L299C:
    /* 299C  inc     dx */
    DX = (uint16_t)(DX + 1);
L299D:
    /* 299D  in      al,dx */
    AL = asm_in8(DX);
L299E:
    /* 299E  or      al,20h */
    AL = (uint8_t)(AL | 0x20);
L29A0:
    /* 29A0  dec     dx */
    DX = (uint16_t)(DX - 1);
L29A1:
    /* 29A1  out     dx,al */
    asm_out8(DX, AL);
L29A2:
    /* 29A2  mov     cx,bx */
    CX = BX;
L29A4:
    /* 29A4  mov     ax,bx */
    AX = BX;
L29A6:
    /* 29A6  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L29A8:
    /* 29A8  jge     L29B6 */
    if (SF == OF) goto L29B6;
L29AA:
    /* 29AA  mov     ax,3FFh */
    AX = 0x3FF;
L29AD:
    /* 29AD  xor     bx,bx */
    BX = (uint16_t)(BX ^ BX);
L29AF:
    /* 29AF  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L29B1:
    /* 29B1  call    _seg003_0272_2A8F */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2A8F), 0x29B4)) != 0) return c;
L29B4:
    /* 29B4  jmp     short L29C8 */
    goto L29C8;
L29B6: /* L29B6 */
    /* 29B6  mov     ax,word ptr ds:[36A8h] */
    AX = rw(pDS, 0x36A8);
L29B9:
    /* 29B9  mul     bx */
    mul16(BX);
L29BB:
    /* 29BB  mov     cx,ax */
    CX = AX;
L29BD:
    /* 29BD  mov     ax,word ptr ds:[3DEEh] */
    AX = rw(pDS, 0x3DEE);
L29C0:
    /* 29C0  sub     ax,bx */
    AX = (uint16_t)(AX - BX);
L29C2:
    /* 29C2  add     ax,ax */
    AX = (uint16_t)(AX + AX);
L29C4:
    /* 29C4  dec     ax */
    AX = (uint16_t)(AX - 1);
L29C5:
    /* 29C5  call    _seg003_0272_2A8F */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2A8F), 0x29C8)) != 0) return c;
L29C8: /* L29C8 */
    /* 29C8  mov     dx,INPUT_STATUS */
    DX = 0x3DA;
L29CB: /* L29CB */
    /* 29CB  in      al,dx */
    AL = asm_in8(DX);
L29CC:
    /* 29CC  and     al,8 */
    AL = logic8((uint8_t)(AL & 0x8));
L29CE:
    /* 29CE  je      L29CB */
    if (ZF) goto L29CB;
L29D0: /* L29D0 */
    /* 29D0  in      al,dx */
    AL = asm_in8(DX);
L29D1:
    /* 29D1  and     al,8 */
    AL = logic8((uint8_t)(AL & 0x8));
L29D3:
    /* 29D3  jne     L29D0 */
    if (!ZF) goto L29D0;
L29D5:
    /* 29D5  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L29D8:
    /* 29D8  mov     al,0Ch */
    AL = 0xC;
L29DA:
    /* 29DA  out     dx,al */
    asm_out8(DX, AL);
L29DB:
    /* 29DB  inc     dx */
    DX = (uint16_t)(DX + 1);
L29DC:
    /* 29DC  mov     al,ch */
    AL = CH;
L29DE:
    /* 29DE  out     dx,al */
    asm_out8(DX, AL);
L29DF:
    /* 29DF  dec     dx */
    DX = (uint16_t)(DX - 1);
L29E0:
    /* 29E0  mov     al,0Dh */
    AL = 0xD;
L29E2:
    /* 29E2  out     dx,al */
    asm_out8(DX, AL);
L29E3:
    /* 29E3  inc     dx */
    DX = (uint16_t)(DX + 1);
L29E4:
    /* 29E4  mov     al,cl */
    AL = CL;
L29E6:
    /* 29E6  out     dx,al */
    asm_out8(DX, AL);
L29E7:
    /* 29E7  mov     ax,cx */
    AX = CX;
L29E9:
    /* 29E9  mov     si,ax */
    SI = AX;
L29EB:
    /* 29EB  mov     cx,bx */
    CX = BX;
L29ED:
    /* 29ED  mov     di,cx */
    DI = CX;
L29EF:
    /* 29EF  mov     bx,word ptr ds:[3DEEh] */
    BX = rw(pDS, 0x3DEE);
L29F3:
    /* 29F3  or      si,si */
    SI = logic16((uint16_t)(SI | SI));
L29F5:
    /* 29F5  je      L2A09 */
    if (ZF) goto L2A09;
L29F7:
    /* 29F7  add     ax,word ptr ds:[36A8h] */
    AX = (uint16_t)(AX + rw(pDS, 0x36A8));
L29FB:
    /* 29FB  call    _seg003_0272_2B2C */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2B2C), 0x29FE)) != 0) return c;
L29FE:
    /* 29FE  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L2A00:
    /* 2A00  mov     bx,di */
    BX = DI;
L2A02:
    /* 2A02  dec     bx */
    BX = (uint16_t)(BX - 1);
L2A03:
    /* 2A03  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L2A05:
    /* 2A05  call    _seg003_0272_2B2C */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2B2C), 0x2A08)) != 0) return c;
L2A08:
    /* 2A08  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2A09: /* L2A09 */
    /* 2A09  call    _seg003_0272_2B2C */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2B2C), 0x2A0C)) != 0) return c;
L2A0C:
    /* 2A0C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2A0D  (+2A0D)
       vscreen_focus: scroll the virtual screen of _2977 to (AX, BX): rebuild Ytab for the visible
       part, wait for the vertical retrace, set the display start (whole bytes) and the horizontal
       pixel panning (attribute register 33h, the low two bits of x, times 2). */
L2A0D: /* _seg003_0272_2A0D */
    /* 2A0D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2A0E:
    /* 2A0E  inc     ax */
    AX = (uint16_t)(AX + 1);
L2A0F:
    /* 2A0F  neg     ax */
    AX = (uint16_t)-AX;
L2A11:
    /* 2A11  add     ax,word ptr ds:[2CDCh] */
    AX = (uint16_t)(AX + rw(pDS, 0x2CDC));
L2A15:
    /* 2A15  mov     dx,word ptr ds:[2CDAh] */
    DX = rw(pDS, 0x2CDA);
L2A19:
    /* 2A19  or      dx,dx */
    DX = logic16((uint16_t)(DX | DX));
L2A1B:
    /* 2A1B  jl      L2A1F */
    if (SF != OF) goto L2A1F;
L2A1D:
    /* 2A1D  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L2A1F: /* L2A1F */
    /* 2A1F  mul     word ptr ds:[36A8h] */
    mul16(rw(pDS, 0x36A8));
L2A23:
    /* 2A23  push    bx */
    push16(BX);
L2A24:
    /* 2A24  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L2A26:
    /* 2A26  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L2A28:
    /* 2A28  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2A2A:
    /* 2A2A  push    ax */
    push16(AX);
L2A2B:
    /* 2A2B  mov     dx,word ptr ds:[2CDAh] */
    DX = rw(pDS, 0x2CDA);
L2A2F:
    /* 2A2F  or      dx,dx */
    DX = logic16((uint16_t)(DX | DX));
L2A31:
    /* 2A31  jl      L2A4B */
    if (SF != OF) goto L2A4B;
L2A33:
    /* 2A33  mov     bx,word ptr ds:[3DEEh] */
    BX = rw(pDS, 0x3DEE);
L2A37:
    /* 2A37  mov     cx,dx */
    CX = DX;
L2A39:
    /* 2A39  inc     cx */
    CX = (uint16_t)(CX + 1);
L2A3A:
    /* 2A3A  pop     ax */
    AX = pop16();
L2A3B:
    /* 2A3B  add     ax,word ptr ds:[36A8h] */
    AX = (uint16_t)(AX + rw(pDS, 0x36A8));
L2A3F:
    /* 2A3F  push    ax */
    push16(AX);
L2A40:
    /* 2A40  call    _seg003_0272_2B2C */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2B2C), 0x2A43)) != 0) return c;
L2A43:
    /* 2A43  mov     bx,word ptr ds:[2CDAh] */
    BX = rw(pDS, 0x2CDA);
L2A47:
    /* 2A47  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L2A49:
    /* 2A49  jmp     short L2A4F */
    goto L2A4F;
L2A4B: /* L2A4B */
    /* 2A4B  mov     bx,word ptr ds:[3DEEh] */
    BX = rw(pDS, 0x3DEE);
L2A4F: /* L2A4F */
    /* 2A4F  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L2A51:
    /* 2A51  call    _seg003_0272_2B2C */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2B2C), 0x2A54)) != 0) return c;
L2A54:
    /* 2A54  mov     dx,INPUT_STATUS */
    DX = 0x3DA;
L2A57: /* L2A57 */
    /* 2A57  in      al,dx */
    AL = asm_in8(DX);
L2A58:
    /* 2A58  and     al,8 */
    AL = logic8((uint8_t)(AL & 0x8));
L2A5A:
    /* 2A5A  je      L2A57 */
    if (ZF) goto L2A57;
L2A5C: /* L2A5C */
    /* 2A5C  in      al,dx */
    AL = asm_in8(DX);
L2A5D:
    /* 2A5D  and     al,8 */
    AL = logic8((uint8_t)(AL & 0x8));
L2A5F:
    /* 2A5F  jne     L2A5C */
    if (!ZF) goto L2A5C;
L2A61:
    /* 2A61  pop     bx */
    BX = pop16();
L2A62:
    /* 2A62  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L2A65:
    /* 2A65  mov     al,0Ch */
    AL = 0xC;
L2A67:
    /* 2A67  out     dx,al */
    asm_out8(DX, AL);
L2A68:
    /* 2A68  inc     dx */
    DX = (uint16_t)(DX + 1);
L2A69:
    /* 2A69  mov     al,bh */
    AL = BH;
L2A6B:
    /* 2A6B  out     dx,al */
    asm_out8(DX, AL);
L2A6C:
    /* 2A6C  dec     dx */
    DX = (uint16_t)(DX - 1);
L2A6D:
    /* 2A6D  mov     al,0Dh */
    AL = 0xD;
L2A6F:
    /* 2A6F  out     dx,al */
    asm_out8(DX, AL);
L2A70:
    /* 2A70  inc     dx */
    DX = (uint16_t)(DX + 1);
L2A71:
    /* 2A71  mov     al,bl */
    AL = BL;
L2A73:
    /* 2A73  out     dx,al */
    asm_out8(DX, AL);
L2A74:
    /* 2A74  mov     dx,INPUT_STATUS */
    DX = 0x3DA;
L2A77: /* L2A77 */
    /* 2A77  in      al,dx */
    AL = asm_in8(DX);
L2A78:
    /* 2A78  and     al,8 */
    AL = logic8((uint8_t)(AL & 0x8));
L2A7A:
    /* 2A7A  jne     L2A77 */
    if (!ZF) goto L2A77;
L2A7C: /* L2A7C */
    /* 2A7C  in      al,dx */
    AL = asm_in8(DX);
L2A7D:
    /* 2A7D  and     al,8 */
    AL = logic8((uint8_t)(AL & 0x8));
L2A7F:
    /* 2A7F  je      L2A7C */
    if (ZF) goto L2A7C;
L2A81:
    /* 2A81  mov     dx,ATTR_INDEX */
    DX = 0x3C0;
L2A84:
    /* 2A84  mov     al,33h */
    AL = 0x33;
L2A86:
    /* 2A86  out     dx,al */
    asm_out8(DX, AL);
L2A87:
    /* 2A87  pop     ax */
    AX = pop16();
L2A88:
    /* 2A88  and     ax,3 */
    AX = (uint16_t)(AX & 0x3);
L2A8B:
    /* 2A8B  shl     ax,1 */
    AX = shl16(AX, 1);
L2A8D:
    /* 2A8D  out     dx,al */
    asm_out8(DX, AL);
L2A8E:
    /* 2A8E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2A8F  (+2A8F)
       _2A8F: set the CRTC line compare (the split-screen line) to AX, ten bits: register 18h and the
       overflow bits in registers 7 (bit 4) and 9 (bit 6). */
L2A8F: /* _seg003_0272_2A8F */
    /* 2A8F  push    ax */
    push16(AX);
L2A90:
    /* 2A90  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L2A93:
    /* 2A93  mov     al,18h */
    AL = 0x18;
L2A95:
    /* 2A95  out     dx,al */
    asm_out8(DX, AL);
L2A96:
    /* 2A96  inc     dx */
    DX = (uint16_t)(DX + 1);
L2A97:
    /* 2A97  pop     ax */
    AX = pop16();
L2A98:
    /* 2A98  out     dx,al */
    asm_out8(DX, AL);
L2A99:
    /* 2A99  dec     dx */
    DX = (uint16_t)(DX - 1);
L2A9A:
    /* 2A9A  mov     al,7 */
    AL = 0x7;
L2A9C:
    /* 2A9C  out     dx,al */
    asm_out8(DX, AL);
L2A9D:
    /* 2A9D  inc     dx */
    DX = (uint16_t)(DX + 1);
L2A9E:
    /* 2A9E  in      al,dx */
    AL = asm_in8(DX);
L2A9F:
    /* 2A9F  test    ah,1 */
    logic8((uint8_t)(AH & 0x1));
L2AA2:
    /* 2AA2  je      L2AA8 */
    if (ZF) goto L2AA8;
L2AA4:
    /* 2AA4  or      al,10h */
    AL = (uint8_t)(AL | 0x10);
L2AA6:
    /* 2AA6  jmp     short L2AAA */
    goto L2AAA;
L2AA8: /* L2AA8 */
    /* 2AA8  and     al,0EFh */
    AL = (uint8_t)(AL & 0xEF);
L2AAA: /* L2AAA */
    /* 2AAA  out     dx,al */
    asm_out8(DX, AL);
L2AAB:
    /* 2AAB  dec     dx */
    DX = (uint16_t)(DX - 1);
L2AAC:
    /* 2AAC  mov     al,9 */
    AL = 0x9;
L2AAE:
    /* 2AAE  out     dx,al */
    asm_out8(DX, AL);
L2AAF:
    /* 2AAF  inc     dx */
    DX = (uint16_t)(DX + 1);
L2AB0:
    /* 2AB0  in      al,dx */
    AL = asm_in8(DX);
L2AB1:
    /* 2AB1  test    ah,2 */
    logic8((uint8_t)(AH & 0x2));
L2AB4:
    /* 2AB4  je      L2ABA */
    if (ZF) goto L2ABA;
L2AB6:
    /* 2AB6  or      al,40h */
    AL = logic8((uint8_t)(AL | 0x40));
L2AB8:
    /* 2AB8  jmp     short L2ABC */
    goto L2ABC;
L2ABA: /* L2ABA */
    /* 2ABA  and     al,0BFh */
    AL = logic8((uint8_t)(AL & 0xBF));
L2ABC: /* L2ABC */
    /* 2ABC  out     dx,al */
    asm_out8(DX, AL);
L2ABD:
    /* 2ABD  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2ABE  (+2ABE)
       grPageFlip: display the drawing page (_295F), then fall into grSoftPageFlip (_2AC1): keep the
       current row table as the displayed page's (3840), work out the other page's start and the
       offset between them (410C), and rebuild Ytab for it, so drawing goes to the page not on
       screen. */
L2ABE: /* _seg003_0272_2ABE */
    /* 2ABE  call    _seg003_0272_295F */
    if ((c = asm_call(ASM_JMP(0x0085, 0x295F), 0x2AC1)) != 0) return c;
L2AC1: /* _seg003_0272_2AC1 */
    /* 2AC1  mov     si,36ACh */
    SI = 0x36AC;
L2AC4:
    /* 2AC4  mov     di,3840h */
    DI = 0x3840;
L2AC7:
    /* 2AC7  mov     cx,word ptr ds:[3DEEh] */
    CX = rw(pDS, 0x3DEE);
L2ACB:
    /* 2ACB  inc     cx */
    CX = (uint16_t)(CX + 1);
L2ACC:
    /* 2ACC  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L2ACE:
    /* 2ACE  mov     ax,word ptr ds:[2CD4h] */
    AX = rw(pDS, 0x2CD4);
L2AD1:
    /* 2AD1  mov     bx,word ptr ds:[3DEEh] */
    BX = rw(pDS, 0x3DEE);
L2AD5:
    /* 2AD5  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L2AD7:
    /* 2AD7  sub     ax,word ptr [bx+36ACh] */
    AX = (uint16_t)(AX - rw(pDS, BX + 0x36AC));
L2ADB:
    /* 2ADB  mov     bx,ax */
    BX = AX;
L2ADD:
    /* 2ADD  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L2ADF:
    /* 2ADF  neg     bx */
    BX = (uint16_t)-BX;
L2AE1:
    /* 2AE1  add     bx,word ptr ds:[2CD4h] */
    BX = (uint16_t)(BX + rw(pDS, 0x2CD4));
L2AE5:
    /* 2AE5  mov     word ptr ds:[410Ch],bx */
    ww(pDS, 0x410C, BX);
L2AE9:
    /* 2AE9  call    _seg003_0272_2AED */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2AED), 0x2AEC)) != 0) return c;
L2AEC:
    /* 2AEC  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2AED  (+2AED)
       _2AED: build Ytab from AX: the highest row (3DEE) gets AX and each lower row 36A8 more (ten
       rows a pass). Marks 2D1C as -1. */
L2AED: /* _seg003_0272_2AED */
    /* 2AED  mov     di,word ptr ds:[3DEEh] */
    DI = rw(pDS, 0x3DEE);
L2AF1:
    /* 2AF1  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L2AF3:
    /* 2AF3  add     di,36ACh */
    DI = (uint16_t)(DI + 0x36AC);
L2AF7:
    /* 2AF7  mov     bx,word ptr ds:[36A8h] */
    BX = rw(pDS, 0x36A8);
L2AFB:
    /* 2AFB  mov     cx,word ptr ds:[3DEEh] */
    CX = rw(pDS, 0x3DEE);
L2AFF:
    /* 2AFF  inc     cx */
    CX = (uint16_t)(CX + 1);
L2B00:
    /* 2B00  std */
    DF = 1;
L2B01: /* L2B01 */
    /* 2B01  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B02:
    /* 2B02  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2B04:
    /* 2B04  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B05:
    /* 2B05  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2B07:
    /* 2B07  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B08:
    /* 2B08  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2B0A:
    /* 2B0A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B0B:
    /* 2B0B  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2B0D:
    /* 2B0D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B0E:
    /* 2B0E  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2B10:
    /* 2B10  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B11:
    /* 2B11  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2B13:
    /* 2B13  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B14:
    /* 2B14  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2B16:
    /* 2B16  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B17:
    /* 2B17  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2B19:
    /* 2B19  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B1A:
    /* 2B1A  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2B1C:
    /* 2B1C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B1D:
    /* 2B1D  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2B1F:
    /* 2B1F  sub     cx,0Ah */
    CX = sub16(CX, 0xA, 0);
L2B22:
    /* 2B22  ja      L2B01 */
    if (!CF && !ZF) goto L2B01;
L2B24:
    /* 2B24  cld */
    DF = 0;
L2B25:
    /* 2B25  mov     word ptr ds:[2D1Ch],0FFFFh */
    ww(pDS, 0x2D1C, 0xFFFF);
L2B2B:
    /* 2B2B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2B2C  (+2B2C)
       _2B2C: set Ytab rows CX up to BX, the highest getting AX, each lower one 36A8 more (for the
       split screen). */
L2B2C: /* _seg003_0272_2B2C */
    /* 2B2C  neg     cx */
    CX = (uint16_t)-CX;
L2B2E:
    /* 2B2E  add     cx,bx */
    CX = (uint16_t)(CX + BX);
L2B30:
    /* 2B30  inc     cx */
    CX = (uint16_t)(CX + 1);
L2B31:
    /* 2B31  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L2B33:
    /* 2B33  mov     dx,word ptr ds:[36A8h] */
    DX = rw(pDS, 0x36A8);
L2B37: /* L2B37 */
    /* 2B37  mov     word ptr [bx+36ACh],ax */
    ww(pDS, BX + 0x36AC, AX);
L2B3B:
    /* 2B3B  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L2B3D:
    /* 2B3D  sub     bx,2 */
    BX = sub16(BX, 0x2, 0);
L2B40:
    /* 2B40  dec     cx */
    CX = dec16(CX);
L2B41:
    /* 2B41  jne     L2B37 */
    if (!ZF) goto L2B37;
L2B43:
    /* 2B43  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2B44  (+2B44)
       _2B44: build the edge plane mask tables: 3B64 from the four bytes at 2D2C and 3CA8 from those
       at 2D31, each repeated 80 times (_2B53), so 3B64[x] and 3CA8[x] are the planes at and right
       of, and at and left of, pixel x within its group of four. */
L2B44: /* _seg003_0272_2B44 */
    /* 2B44  mov     si,2D2Ch */
    SI = 0x2D2C;
L2B47:
    /* 2B47  mov     di,3B64h */
    DI = 0x3B64;
L2B4A:
    /* 2B4A  call    _seg003_0272_2B53 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2B53), 0x2B4D)) != 0) return c;
L2B4D:
    /* 2B4D  mov     si,2D31h */
    SI = 0x2D31;
L2B50:
    /* 2B50  mov     di,3CA8h */
    DI = 0x3CA8;

    /* seg003_0272_2B53  (+2B53) */
L2B53: /* _seg003_0272_2B53 */
    /* 2B53  mov     cx,50h */
    CX = 0x50;
L2B56: /* L2B56 */
    /* 2B56  push    cx */
    push16(CX);
L2B57:
    /* 2B57  push    si */
    push16(SI);
L2B58:
    /* 2B58  mov     cx,4 */
    CX = 0x4;
L2B5B:
    /* 2B5B  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L2B5D:
    /* 2B5D  pop     si */
    SI = pop16();
L2B5E:
    /* 2B5E  pop     cx */
    CX = pop16();
L2B5F:
    /* 2B5F  loop    L2B56 */
    if (--CX) goto L2B56;
L2B61:
    /* 2B61  ret */
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

    /* SetVideoMode_seg003_0272_2B9B  (+2B9B)
       SetVideoMode: mode 13h through the BIOS, then unchained (mode X): chain-4 off in the
       sequencer's memory mode, odd/even off in the graphics controller, bit mask and map mask all
       on, the whole 256 KB cleared, and the CRTC switched from doubleword to byte addressing. Leaves
       the sequencer index at the map mask. */
L2B9B: /* _SetVideoMode_seg003_0272_2B9B */
    /* 2B9B  mov     ax,(VID_SET_MODE shl 8) + 13h */
    AX = 0x13;
L2B9E:
    /* 2B9E  int     10h */
    asm_int(0x10);
L2BA0:
    /* 2BA0  mov     dx,SC_INDEX */
    DX = 0x3C4;
L2BA3:
    /* 2BA3  mov     al,4 */
    AL = 0x4;
L2BA5:
    /* 2BA5  out     dx,al */
    asm_out8(DX, AL);
L2BA6:
    /* 2BA6  inc     dx */
    DX = (uint16_t)(DX + 1);
L2BA7:
    /* 2BA7  in      al,dx */
    AL = asm_in8(DX);
L2BA8:
    /* 2BA8  and     al,0F7h */
    AL = (uint8_t)(AL & 0xF7);
L2BAA:
    /* 2BAA  or      al,4 */
    AL = (uint8_t)(AL | 0x4);
L2BAC:
    /* 2BAC  out     dx,al */
    asm_out8(DX, AL);
L2BAD:
    /* 2BAD  mov     dx,GC_INDEX */
    DX = 0x3CE;
L2BB0:
    /* 2BB0  mov     al,5 */
    AL = 0x5;
L2BB2:
    /* 2BB2  out     dx,al */
    asm_out8(DX, AL);
L2BB3:
    /* 2BB3  inc     dx */
    DX = (uint16_t)(DX + 1);
L2BB4:
    /* 2BB4  in      al,dx */
    AL = asm_in8(DX);
L2BB5:
    /* 2BB5  and     al,0EFh */
    AL = (uint8_t)(AL & 0xEF);
L2BB7:
    /* 2BB7  out     dx,al */
    asm_out8(DX, AL);
L2BB8:
    /* 2BB8  dec     dx */
    DX = (uint16_t)(DX - 1);
L2BB9:
    /* 2BB9  mov     al,6 */
    AL = 0x6;
L2BBB:
    /* 2BBB  out     dx,al */
    asm_out8(DX, AL);
L2BBC:
    /* 2BBC  inc     dx */
    DX = (uint16_t)(DX + 1);
L2BBD:
    /* 2BBD  in      al,dx */
    AL = asm_in8(DX);
L2BBE:
    /* 2BBE  and     al,0FDh */
    AL = (uint8_t)(AL & 0xFD);
L2BC0:
    /* 2BC0  out     dx,al */
    asm_out8(DX, AL);
L2BC1:
    /* 2BC1  mov     ax,0FF08h */
    AX = 0xFF08;
L2BC4:
    /* 2BC4  mov     dx,GC_INDEX */
    DX = 0x3CE;
L2BC7:
    /* 2BC7  out     dx,ax */
    asm_out16(DX, AX);
L2BC8:
    /* 2BC8  mov     dx,SC_INDEX */
    DX = 0x3C4;
L2BCB:
    /* 2BCB  mov     ax,0F02h */
    AX = 0xF02;
L2BCE:
    /* 2BCE  out     dx,ax */
    asm_out16(DX, AX);
L2BCF:
    /* 2BCF  push    es */
    push16(asm_es);
L2BD0:
    /* 2BD0  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L2BD4:
    /* 2BD4  sub     di,di */
    DI = (uint16_t)(DI - DI);
L2BD6:
    /* 2BD6  mov     ax,di */
    AX = DI;
L2BD8:
    /* 2BD8  mov     cx,8000h */
    CX = 0x8000;
L2BDB:
    /* 2BDB  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L2BDD:
    /* 2BDD  pop     es */
    SET_ES(pop16());
L2BDE:
    /* 2BDE  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L2BE1:
    /* 2BE1  mov     al,14h */
    AL = 0x14;
L2BE3:
    /* 2BE3  out     dx,al */
    asm_out8(DX, AL);
L2BE4:
    /* 2BE4  inc     dx */
    DX = (uint16_t)(DX + 1);
L2BE5:
    /* 2BE5  in      al,dx */
    AL = asm_in8(DX);
L2BE6:
    /* 2BE6  and     al,0BFh */
    AL = (uint8_t)(AL & 0xBF);
L2BE8:
    /* 2BE8  out     dx,al */
    asm_out8(DX, AL);
L2BE9:
    /* 2BE9  dec     dx */
    DX = (uint16_t)(DX - 1);
L2BEA:
    /* 2BEA  mov     al,17h */
    AL = 0x17;
L2BEC:
    /* 2BEC  out     dx,al */
    asm_out8(DX, AL);
L2BED:
    /* 2BED  inc     dx */
    DX = (uint16_t)(DX + 1);
L2BEE:
    /* 2BEE  in      al,dx */
    AL = asm_in8(DX);
L2BEF:
    /* 2BEF  or      al,40h */
    AL = logic8((uint8_t)(AL | 0x40));
L2BF1:
    /* 2BF1  out     dx,al */
    asm_out8(DX, AL);
L2BF2:
    /* 2BF2  mov     dx,SC_INDEX */
    DX = 0x3C4;
L2BF5:
    /* 2BF5  mov     al,2 */
    AL = 0x2;
L2BF7:
    /* 2BF7  out     dx,al */
    asm_out8(DX, AL);
L2BF8:
    /* 2BF8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2BF9  (+2BF9)
       _2BF9: guard the new window's edges for the unclipped writers: the Ytab entries just above and
       below the window point at the spare area 2CD6, and the edge mask entries just left and right
       of it get partial masks (2D2D, 2D30). _2C44 undoes this for the old window. Both are called by
       set_the_window.

       The span writers follow (in 2C44's proc). Each takes SI = a list of 6-byte spans (y, left x,
       right x) ended by a y with its top bit set, and writes the left and right partial groups with
       the edge masks and the whole groups between with all four planes:
         _2C9A  XOR colour 0Fh into the spans (graphics controller function XOR, latches loaded
                first), probably for a highlight or cursor
         _2D0D  solid fill on the drawing page (_2D83), then the same on the other page (3840) */
L2BF9: /* _seg003_0272_2BF9 */
    /* 2BF9  mov     ax,word ptr ds:[3DF6h] */
    AX = rw(pDS, 0x3DF6);
L2BFC:
    /* 2BFC  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L2BFE:
    /* 2BFE  add     ax,36AEh */
    AX = (uint16_t)(AX + 0x36AE);
L2C01:
    /* 2C01  mov     bx,ax */
    BX = AX;
L2C03:
    /* 2C03  mov     ax,word ptr ds:[2CD6h] */
    AX = rw(pDS, 0x2CD6);
L2C06:
    /* 2C06  xchg    ax,word ptr [bx] */
    { uint16_t t_ = rw(pDS, BX);
    ww(pDS, BX, AX);
    AX = t_; }
L2C08:
    /* 2C08  mov     ax,word ptr ds:[3DFAh] */
    AX = rw(pDS, 0x3DFA);
L2C0B:
    /* 2C0B  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L2C0D:
    /* 2C0D  add     ax,36AAh */
    AX = (uint16_t)(AX + 0x36AA);
L2C10:
    /* 2C10  mov     bx,ax */
    BX = AX;
L2C12:
    /* 2C12  mov     ax,word ptr ds:[2CD6h] */
    AX = rw(pDS, 0x2CD6);
L2C15:
    /* 2C15  xchg    ax,word ptr [bx] */
    { uint16_t t_ = rw(pDS, BX);
    ww(pDS, BX, AX);
    AX = t_; }
L2C17:
    /* 2C17  mov     ax,3B64h */
    AX = 0x3B64;
L2C1A:
    /* 2C1A  mov     bx,word ptr ds:[3DF4h] */
    BX = rw(pDS, 0x3DF4);
L2C1E:
    /* 2C1E  dec     bx */
    BX = (uint16_t)(BX - 1);
L2C1F:
    /* 2C1F  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2C21:
    /* 2C21  mov     bp,ax */
    BP = AX;
L2C23:
    /* 2C23  and     bx,3 */
    BX = (uint16_t)(BX & 0x3);
L2C26:
    /* 2C26  mov     al,byte ptr [bx+2D2Dh] */
    AL = rb(pDS, BX + 0x2D2D);
L2C2A:
    /* 2C2A  mov     byte ptr [bp],al */
    wb(pSS, BP, AL);
L2C2D:
    /* 2C2D  mov     ax,3CA8h */
    AX = 0x3CA8;
L2C30:
    /* 2C30  mov     bx,word ptr ds:[3DF8h] */
    BX = rw(pDS, 0x3DF8);
L2C34:
    /* 2C34  inc     bx */
    BX = (uint16_t)(BX + 1);
L2C35:
    /* 2C35  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2C37:
    /* 2C37  mov     bp,ax */
    BP = AX;
L2C39:
    /* 2C39  and     bx,3 */
    BX = logic16((uint16_t)(BX & 0x3));
L2C3C:
    /* 2C3C  mov     al,byte ptr [bx+2D30h] */
    AL = rb(pDS, BX + 0x2D30);
L2C40:
    /* 2C40  mov     byte ptr [bp],al */
    wb(pSS, BP, AL);
L2C43:
    /* 2C43  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_2C44  (+2C44) */
L2C44: /* _seg003_0272_2C44 */
    /* 2C44  mov     ax,3B64h */
    AX = 0x3B64;
L2C47:
    /* 2C47  mov     bx,word ptr ds:[3DF4h] */
    BX = rw(pDS, 0x3DF4);
L2C4B:
    /* 2C4B  dec     bx */
    BX = (uint16_t)(BX - 1);
L2C4C:
    /* 2C4C  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2C4E:
    /* 2C4E  mov     bp,ax */
    BP = AX;
L2C50:
    /* 2C50  and     bx,3 */
    BX = (uint16_t)(BX & 0x3);
L2C53:
    /* 2C53  mov     al,byte ptr [bx+2D2Ch] */
    AL = rb(pDS, BX + 0x2D2C);
L2C57:
    /* 2C57  mov     byte ptr [bp],al */
    wb(pSS, BP, AL);
L2C5A:
    /* 2C5A  mov     ax,3CA8h */
    AX = 0x3CA8;
L2C5D:
    /* 2C5D  mov     bx,word ptr ds:[3DF8h] */
    BX = rw(pDS, 0x3DF8);
L2C61:
    /* 2C61  inc     bx */
    BX = (uint16_t)(BX + 1);
L2C62:
    /* 2C62  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L2C64:
    /* 2C64  mov     bp,ax */
    BP = AX;
L2C66:
    /* 2C66  and     bx,3 */
    BX = (uint16_t)(BX & 0x3);
L2C69:
    /* 2C69  mov     al,byte ptr [bx+2D31h] */
    AL = rb(pDS, BX + 0x2D31);
L2C6D:
    /* 2C6D  mov     byte ptr [bp],al */
    wb(pSS, BP, AL);
L2C70:
    /* 2C70  mov     bx,word ptr ds:[3DF6h] */
    BX = rw(pDS, 0x3DF6);
L2C74:
    /* 2C74  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L2C76:
    /* 2C76  mov     ax,word ptr [bx+36ACh] */
    AX = rw(pDS, BX + 0x36AC);
L2C7A:
    /* 2C7A  sub     ax,word ptr ds:[36A8h] */
    AX = (uint16_t)(AX - rw(pDS, 0x36A8));
L2C7E:
    /* 2C7E  mov     word ptr [bx+36AEh],ax */
    ww(pDS, BX + 0x36AE, AX);
L2C82:
    /* 2C82  mov     bx,word ptr ds:[3DFAh] */
    BX = rw(pDS, 0x3DFA);
L2C86:
    /* 2C86  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L2C88:
    /* 2C88  mov     ax,word ptr [bx+36ACh] */
    AX = rw(pDS, BX + 0x36AC);
L2C8C:
    /* 2C8C  add     ax,word ptr ds:[36A8h] */
    AX = add16(AX, rw(pDS, 0x36A8), 0);
L2C90:
    /* 2C90  mov     word ptr [bx+36AAh],ax */
    ww(pDS, BX + 0x36AA, AX);
L2C94:
    /* 2C94  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2C95:
    /* 2C95  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2C96:
    /* 2C96  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2C97:
    /* 2C97  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2C98:
    /* 2C98  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2C99:
    /* 2C99  ret */
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

    /* seg003_0272_2D83  (+2D83)
       _2D83: fill the spans with the colour in 4111 (FM solid_ylr). The labels after it:
         _2DF3  save: copy the spans' whole groups to video memory at 4116 through the latches
         _2E3A  copy the spans' whole groups from the other page (410C) through the latches
         _2E79  copy from the other page (_2F18), then the same onto the displayed page (410E) */
L2D83: /* _seg003_0272_2D83 */
    /* 2D83  mov     dx,SC_DATA */
    DX = 0x3C5;
L2D86:
    /* 2D86  mov     bl,byte ptr ds:[4111h] */
    BL = rb(pDS, 0x4111);
L2D8A:
    /* 2D8A  push    es */
    push16(asm_es);
L2D8B:
    /* 2D8B  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L2D8F:
    /* 2D8F  jmp     short L2DAD */
    goto L2DAD;
L2D91: /* L2D91 */
    /* 2D91  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L2D93:
    /* 2D93  shr     bh,1 */
    BH = (uint8_t)(BH >> 1);
L2D95:
    /* 2D95  not     al */
    AL = (uint8_t)~AL;
L2D97:
    /* 2D97  not     bh */
    BH = (uint8_t)~BH;
L2D99:
    /* 2D99  xchg    al,bh */
    { uint8_t t_ = BH;
    BH = AL;
    AL = t_; }
L2D9B:
    /* 2D9B  add     di,cx */
    DI = (uint16_t)(DI + CX);
L2D9D:
    /* 2D9D  neg     cx */
    CX = (uint16_t)-CX;
L2D9F:
    /* 2D9F  jmp     short L2DD0 */
    goto L2DD0;
L2DA1: /* L2DA1 */
    /* 2DA1  jl      L2D91 */
    if (SF != OF) goto L2D91;
L2DA3:
    /* 2DA3  add     di,word ptr [bp+36ACh] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AC));
L2DA7:
    /* 2DA7  and     al,bh */
    AL = (uint8_t)(AL & BH);
L2DA9:
    /* 2DA9  out     dx,al */
    asm_out8(DX, AL);
L2DAA:
    /* 2DAA  mov     al,bl */
    AL = BL;
L2DAC:
    /* 2DAC  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2DAD: /* L2DAD */
    /* 2DAD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DAE:
    /* 2DAE  shl     ax,1 */
    AX = shl16(AX, 1);
L2DB0:
    /* 2DB0  jb      L2D7E */
    if (CF) goto L2D7E;
L2DB2:
    /* 2DB2  mov     cx,ax */
    CX = AX;
L2DB4:
    /* 2DB4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DB5:
    /* 2DB5  mov     di,ax */
    DI = AX;
L2DB7:
    /* 2DB7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DB8:
    /* 2DB8  mov     bp,ax */
    BP = AX;
L2DBA:
    /* 2DBA  mov     bh,byte ptr [bp+3CA8h] */
    BH = rb(pSS, BP + 0x3CA8);
L2DBE:
    /* 2DBE  mov     al,byte ptr [di+3B64h] */
    AL = rb(pDS, DI + 0x3B64);
L2DC2:
    /* 2DC2  xchg    cx,bp */
    { uint16_t t_ = BP;
    BP = CX;
    CX = t_; }
L2DC4:
    /* 2DC4  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2DC6:
    /* 2DC6  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2DC8:
    /* 2DC8  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2DCA:
    /* 2DCA  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2DCC:
    /* 2DCC  sub     cx,di */
    CX = sub16(CX, DI, 0);
L2DCE:
    /* 2DCE  jle     L2DA1 */
    if (ZF || SF != OF) goto L2DA1;
L2DD0: /* L2DD0 */
    /* 2DD0  out     dx,al */
    asm_out8(DX, AL);
L2DD1:
    /* 2DD1  add     di,word ptr [bp+36ACh] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AC));
L2DD5:
    /* 2DD5  mov     al,bl */
    AL = BL;
L2DD7:
    /* 2DD7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2DD8:
    /* 2DD8  dec     cx */
    CX = (uint16_t)(CX - 1);
L2DD9:
    /* 2DD9  mov     al,0Fh */
    AL = 0xF;
L2DDB:
    /* 2DDB  out     dx,al */
    asm_out8(DX, AL);
L2DDC:
    /* 2DDC  mov     al,bl */
    AL = BL;
L2DDE:
    /* 2DDE  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L2DE0:
    /* 2DE0  mov     al,bh */
    AL = BH;
L2DE2:
    /* 2DE2  out     dx,al */
    asm_out8(DX, AL);
L2DE3:
    /* 2DE3  mov     al,bl */
    AL = BL;
L2DE5:
    /* 2DE5  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2DE6:
    /* 2DE6  jmp     L2DAD */
    goto L2DAD;
L2DE8: /* L2DE8 */
    /* 2DE8  mov     al,0FFh */
    AL = 0xFF;
L2DEA:
    /* 2DEA  mov     dx,GC_DATA */
    DX = 0x3CF;
L2DED:
    /* 2DED  out     dx,al */
    asm_out8(DX, AL);
L2DEE:
    /* 2DEE  pop     es */
    SET_ES(pop16());
L2DEF:
    /* 2DEF  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2DF0:
    /* 2DF0  call    _nullsub_2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x43F4), 0x2DF3)) != 0) return c;
L2DF3: /* _seg003_0272_2DF3 */
    /* 2DF3  mov     ax,0 */
    AX = 0x0;
L2DF6:
    /* 2DF6  mov     dx,GC_DATA */
    DX = 0x3CF;
L2DF9:
    /* 2DF9  out     dx,al */
    asm_out8(DX, AL);
L2DFA:
    /* 2DFA  mov     dx,SC_DATA */
    DX = 0x3C5;
L2DFD:
    /* 2DFD  mov     al,0Fh */
    AL = 0xF;
L2DFF:
    /* 2DFF  out     dx,al */
    asm_out8(DX, AL);
L2E00:
    /* 2E00  mov     bx,si */
    BX = SI;
L2E02:
    /* 2E02  mov     di,word ptr ds:[4116h] */
    DI = rw(pDS, 0x4116);
L2E06:
    /* 2E06  push    es */
    push16(asm_es);
L2E07:
    /* 2E07  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L2E0B: /* L2E0B */
    /* 2E0B  mov     bp,word ptr [bx] */
    BP = rw(pDS, BX);
L2E0D:
    /* 2E0D  shl     bp,1 */
    BP = shl16(BP, 1);
L2E0F:
    /* 2E0F  jb      L2DE8 */
    if (CF) goto L2DE8;
L2E11:
    /* 2E11  mov     si,word ptr [bx+2] */
    SI = rw(pDS, BX + 0x2);
L2E14:
    /* 2E14  mov     cx,word ptr [bx+4] */
    CX = rw(pDS, BX + 0x4);
L2E17:
    /* 2E17  add     bx,6 */
    BX = (uint16_t)(BX + 0x6);
L2E1A:
    /* 2E1A  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L2E1C:
    /* 2E1C  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L2E1E:
    /* 2E1E  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2E20:
    /* 2E20  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2E22:
    /* 2E22  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L2E24:
    /* 2E24  inc     cx */
    CX = (uint16_t)(CX + 1);
L2E25:
    /* 2E25  add     si,word ptr [bp+36ACh] */
    SI = (uint16_t)(SI + rw(pSS, BP + 0x36AC));
L2E29:
    /* 2E29  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L2E2C:
    /* 2E2C  jmp     L2E0B */
    goto L2E0B;
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

    /* seg003_0272_2F18  (+2F18)
       _2F18: copy the spans from the other page (410C away) into the drawing page through the
       latches, edge masks included (FM copy_ylr). The labels after it:
         _2F96  restore: copy the spans' whole groups back from video memory at 4116
         _2FD6, _303B  plane-by-plane copies between the spans and the area at 4116, one plane at a
                time (map mask or read map select); probably the save and restore of a pen whose
                area is not in video memory, not traced */
L2F18: /* _seg003_0272_2F18 */
    /* 2F18  mov     ax,0 */
    AX = 0x0;
L2F1B:
    /* 2F1B  mov     dx,GC_DATA */
    DX = 0x3CF;
L2F1E:
    /* 2F1E  out     dx,al */
    asm_out8(DX, AL);
L2F1F:
    /* 2F1F  mov     dx,SC_DATA */
    DX = 0x3C5;
L2F22:
    /* 2F22  mov     bx,si */
    BX = SI;
L2F24:
    /* 2F24  push    es */
    push16(asm_es);
L2F25:
    /* 2F25  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L2F29:
    /* 2F29  jmp     short L2F4A */
    goto L2F4A;
L2F2B: /* L2F2B */
    /* 2F2B  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L2F2D:
    /* 2F2D  shr     ah,1 */
    AH = (uint8_t)(AH >> 1);
L2F2F:
    /* 2F2F  not     ax */
    AX = (uint16_t)~AX;
L2F31:
    /* 2F31  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L2F33:
    /* 2F33  add     di,cx */
    DI = (uint16_t)(DI + CX);
L2F35:
    /* 2F35  neg     cx */
    CX = (uint16_t)-CX;
L2F37:
    /* 2F37  jmp     short L2F6F */
    goto L2F6F;
L2F39: /* L2F39 */
    /* 2F39  jl      L2F2B */
    if (SF != OF) goto L2F2B;
L2F3B:
    /* 2F3B  add     di,word ptr [si+36ACh] */
    DI = (uint16_t)(DI + rw(pDS, SI + 0x36AC));
L2F3F:
    /* 2F3F  mov     bp,word ptr ds:[410Ch] */
    BP = rw(pDS, 0x410C);
L2F43:
    /* 2F43  and     al,ah */
    AL = (uint8_t)(AL & AH);
L2F45:
    /* 2F45  out     dx,al */
    asm_out8(DX, AL);
L2F46:
    /* 2F46  mov     al,byte ptr es:[bp+di] */
    AL = rb(pES, BP + DI);
L2F49:
    /* 2F49  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L2F4A: /* L2F4A */
    /* 2F4A  mov     si,word ptr [bx] */
    SI = rw(pDS, BX);
L2F4C:
    /* 2F4C  shl     si,1 */
    SI = shl16(SI, 1);
L2F4E:
    /* 2F4E  jb      L2F0C */
    if (CF) goto L2F0C;
L2F50:
    /* 2F50  mov     di,word ptr [bx+2] */
    DI = rw(pDS, BX + 0x2);
L2F53:
    /* 2F53  mov     bp,word ptr [bx+4] */
    BP = rw(pDS, BX + 0x4);
L2F56:
    /* 2F56  add     bx,6 */
    BX = (uint16_t)(BX + 0x6);
L2F59:
    /* 2F59  mov     al,byte ptr [di+3B64h] */
    AL = rb(pDS, DI + 0x3B64);
L2F5D:
    /* 2F5D  mov     ah,byte ptr [bp+3CA8h] */
    AH = rb(pSS, BP + 0x3CA8);
L2F61:
    /* 2F61  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2F63:
    /* 2F63  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2F65:
    /* 2F65  mov     cx,bp */
    CX = BP;
L2F67:
    /* 2F67  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2F69:
    /* 2F69  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2F6B:
    /* 2F6B  sub     cx,di */
    CX = sub16(CX, DI, 0);
L2F6D:
    /* 2F6D  jle     L2F39 */
    if (ZF || SF != OF) goto L2F39;
L2F6F: /* L2F6F */
    /* 2F6F  add     di,word ptr [si+36ACh] */
    DI = (uint16_t)(DI + rw(pDS, SI + 0x36AC));
L2F73:
    /* 2F73  mov     si,di */
    SI = DI;
L2F75:
    /* 2F75  add     si,word ptr ds:[410Ch] */
    SI = (uint16_t)(SI + rw(pDS, 0x410C));
L2F79:
    /* 2F79  out     dx,al */
    asm_out8(DX, AL);
L2F7A:
    /* 2F7A  movs    byte ptr es:[di],byte ptr es:[si] */
    wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L2F7C:
    /* 2F7C  dec     cx */
    CX = (uint16_t)(CX - 1);
L2F7D:
    /* 2F7D  mov     al,0Fh */
    AL = 0xF;
L2F7F:
    /* 2F7F  out     dx,al */
    asm_out8(DX, AL);
L2F80:
    /* 2F80  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L2F83:
    /* 2F83  mov     al,ah */
    AL = AH;
L2F85:
    /* 2F85  out     dx,al */
    asm_out8(DX, AL);
L2F86:
    /* 2F86  movs    byte ptr es:[di],byte ptr es:[si] */
    wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L2F88:
    /* 2F88  jmp     L2F4A */
    goto L2F4A;
L2F8A: /* L2F8A */
    /* 2F8A  mov     ax,0FFh */
    AX = 0xFF;
L2F8D:
    /* 2F8D  mov     dx,GC_DATA */
    DX = 0x3CF;
L2F90:
    /* 2F90  out     dx,al */
    asm_out8(DX, AL);
L2F91:
    /* 2F91  pop     es */
    SET_ES(pop16());
L2F92:
    /* 2F92  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2F93:
    /* 2F93  call    _nullsub_2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x43F4), 0x2F96)) != 0) return c;
L2F96: /* _seg003_0272_2F96 */
    /* 2F96  mov     ax,0 */
    AX = 0x0;
L2F99:
    /* 2F99  mov     dx,GC_DATA */
    DX = 0x3CF;
L2F9C:
    /* 2F9C  out     dx,al */
    asm_out8(DX, AL);
L2F9D:
    /* 2F9D  mov     dx,SC_DATA */
    DX = 0x3C5;
L2FA0:
    /* 2FA0  mov     al,0Fh */
    AL = 0xF;
L2FA2:
    /* 2FA2  out     dx,al */
    asm_out8(DX, AL);
L2FA3:
    /* 2FA3  mov     bx,si */
    BX = SI;
L2FA5:
    /* 2FA5  mov     si,word ptr ds:[4116h] */
    SI = rw(pDS, 0x4116);
L2FA9:
    /* 2FA9  push    es */
    push16(asm_es);
L2FAA:
    /* 2FAA  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L2FAE: /* L2FAE */
    /* 2FAE  mov     bp,word ptr [bx] */
    BP = rw(pDS, BX);
L2FB0:
    /* 2FB0  shl     bp,1 */
    BP = shl16(BP, 1);
L2FB2:
    /* 2FB2  jb      L2F8A */
    if (CF) goto L2F8A;
L2FB4:
    /* 2FB4  mov     di,word ptr [bx+2] */
    DI = rw(pDS, BX + 0x2);
L2FB7:
    /* 2FB7  mov     cx,word ptr [bx+4] */
    CX = rw(pDS, BX + 0x4);
L2FBA:
    /* 2FBA  add     bx,6 */
    BX = (uint16_t)(BX + 0x6);
L2FBD:
    /* 2FBD  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2FBF:
    /* 2FBF  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L2FC1:
    /* 2FC1  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2FC3:
    /* 2FC3  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L2FC5:
    /* 2FC5  sub     cx,di */
    CX = (uint16_t)(CX - DI);
L2FC7:
    /* 2FC7  inc     cx */
    CX = (uint16_t)(CX + 1);
L2FC8:
    /* 2FC8  add     di,word ptr [bp+36ACh] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AC));
L2FCC:
    /* 2FCC  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L2FCF:
    /* 2FCF  jmp     L2FAE */
    goto L2FAE;
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

    /* seg003_0272_3094  (+3094)
       _3094 (plot): plot (AX, BX) in the colour 4111 if it is inside the window. _30AC is the
       unclipped entry: map mask from the edge tables, address from Ytab. _30CF and _30D7 read a
       pixel (clipped or not) and compare it with CL, leaving the flags. */
L3094: /* _seg003_0272_3094 */
    /* 3094  cmp     bx,word ptr ds:[3DF6h] */
    sub16(BX, rw(pDS, 0x3DF6), 0);
L3098:
    /* 3098  jg      L30CE */
    if (!ZF && SF == OF) goto L30CE;
L309A:
    /* 309A  cmp     bx,word ptr ds:[3DFAh] */
    sub16(BX, rw(pDS, 0x3DFA), 0);
L309E:
    /* 309E  jl      L30CE */
    if (SF != OF) goto L30CE;
L30A0:
    /* 30A0  cmp     ax,word ptr ds:[3DF8h] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L30A4:
    /* 30A4  jg      L30CE */
    if (!ZF && SF == OF) goto L30CE;
L30A6:
    /* 30A6  cmp     ax,word ptr ds:[3DF4h] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L30AA:
    /* 30AA  jl      L30CE */
    if (SF != OF) goto L30CE;
L30AC: /* _seg003_0272_30AC */
    /* 30AC  mov     dx,SC_DATA */
    DX = 0x3C5;
L30AF:
    /* 30AF  push    es */
    push16(asm_es);
L30B0:
    /* 30B0  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L30B4:
    /* 30B4  mov     di,ax */
    DI = AX;
L30B6:
    /* 30B6  mov     al,byte ptr [di+3B64h] */
    AL = rb(pDS, DI + 0x3B64);
L30BA:
    /* 30BA  and     al,byte ptr [di+3CA8h] */
    AL = (uint8_t)(AL & rb(pDS, DI + 0x3CA8));
L30BE:
    /* 30BE  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L30C0:
    /* 30C0  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L30C2:
    /* 30C2  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L30C4:
    /* 30C4  add     di,word ptr [bx+36ACh] */
    DI = add16(DI, rw(pDS, BX + 0x36AC), 0);
L30C8:
    /* 30C8  out     dx,al */
    asm_out8(DX, AL);
L30C9:
    /* 30C9  mov     al,byte ptr ds:[4111h] */
    AL = rb(pDS, 0x4111);
L30CC:
    /* 30CC  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L30CD:
    /* 30CD  pop     es */
    SET_ES(pop16());
L30CE: /* L30CE */
    /* 30CE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
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

    /* seg003_0272_30FB  (+30FB)
       _30FB: read the pixel at (AX, BX): select its plane for reading (graphics controller register
       4) and read the byte. Out: AL = the colour. Leaves the graphics controller index at 8. */
L30FB: /* _seg003_0272_30FB */
    /* 30FB  mov     di,ax */
    DI = AX;
L30FD:
    /* 30FD  mov     ah,al */
    AH = AL;
L30FF:
    /* 30FF  and     ah,3 */
    AH = (uint8_t)(AH & 0x3);
L3102:
    /* 3102  mov     al,4 */
    AL = 0x4;
L3104:
    /* 3104  mov     dx,GC_INDEX */
    DX = 0x3CE;
L3107:
    /* 3107  out     dx,ax */
    asm_out16(DX, AX);
L3108:
    /* 3108  push    es */
    push16(asm_es);
L3109:
    /* 3109  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L310D:
    /* 310D  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L310F:
    /* 310F  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L3111:
    /* 3111  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3113:
    /* 3113  add     di,word ptr [bx+36ACh] */
    DI = add16(DI, rw(pDS, BX + 0x36AC), 0);
L3117:
    /* 3117  mov     ah,byte ptr es:[di] */
    AH = rb(pES, DI);
L311A:
    /* 311A  pop     es */
    SET_ES(pop16());
L311B:
    /* 311B  mov     al,8 */
    AL = 0x8;
L311D:
    /* 311D  out     dx,al */
    asm_out8(DX, AL);
L311E:
    /* 311E  mov     al,ah */
    AL = AH;
L3120:
    /* 3120  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

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

    /* seg003_0272_3195  (+3195)
       set_the_color: AX = a pen number (negative: no change). As GRENTRY's fbuf_setcolor, for the
       screen: colour bytes 4110 and 4116 from the tables at 21E and 434, and the span writer from
       the table at 2CEE by the pen's mode, less 5046 (the concave-window variants). */
L3195: /* _seg003_0272_3195 */
    /* 3195  shl     ax,1 */
    AX = shl16(AX, 1);
L3197:
    /* 3197  jb      L31BD */
    if (CF) goto L31BD;
L3199:
    /* 3199  mov     bx,ax */
    BX = AX;
L319B:
    /* 319B  mov     ax,word ptr [bx+21Eh] */
    AX = rw(pDS, BX + 0x21E);
L319F:
    /* 319F  mov     word ptr ds:[4110h],ax */
    ww(pDS, 0x4110, AX);
L31A2:
    /* 31A2  mov     ax,word ptr [bx+434h] */
    AX = rw(pDS, BX + 0x434);
L31A6:
    /* 31A6  mov     word ptr ds:[4116h],ax */
    ww(pDS, 0x4116, AX);
    asm_halt_at(0x0085, 0x31A9, "ran off the code into data");
L31AD:
    /* 31AD  cmp     bx,14h */
    sub16(BX, 0x14, 0);
L31B0:
    /* 31B0  jae     L31BD */
    if (!CF) goto L31BD;
L31B2:
    /* 31B2  mov     ax,word ptr [bx+2CEEh] */
    AX = rw(pDS, BX + 0x2CEE);
L31B6:
    /* 31B6  sub     ax,word ptr ds:[5046h] */
    AX = sub16(AX, rw(pDS, 0x5046), 0);
L31BA:
    /* 31BA  mov     word ptr ds:[4112h],ax */
    ww(pDS, 0x4112, AX);
L31BD: /* L31BD */
    /* 31BD  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_31BE  (+31BE)
       init_colors: run each pen's set-up routine (the table at 2D04, by the pen's mode) for every
       pen, then store DI (lowered by any pen that took video memory) as the video memory limit 4108. */
L31BE: /* _seg003_0272_31BE */
    /* 31BE  mov     di,0FFFFh */
    DI = 0xFFFF;
L31C1:
    /* 31C1  call    _nullsub_1 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x31E5), 0x31C4)) != 0) return c;
L31C4:
    /* 31C4  mov     cx,21Eh */
    CX = 0x21E;
    asm_halt_at(0x0085, 0x31C7, "ran off the code into data");
L31CB:
    /* 31CB  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L31CD:
    /* 31CD  sub     bp,bp */
    BP = sub16(BP, BP, 0);
    asm_halt_at(0x0085, 0x31CF, "ran off the code into data");
L31D3:
    /* 31D3  cmp     bx,14h */
    sub16(BX, 0x14, 0);
L31D6:
    /* 31D6  jae     L31DC */
    if (!CF) goto L31DC;
L31D8:
    /* 31D8  call    word ptr [bx+2D04h] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, BX + 0x2D04)), 0x31DC)) != 0) return c;
L31DC: /* L31DC */
    /* 31DC  inc     bp */
    BP = inc16(BP);
L31DD:
    /* 31DD  inc     bp */
    BP = inc16(BP);
L31DE:
    /* 31DE  loop    L31CF */
    if (--CX) return ASM_JMP(0x0085, 0x31CF);
L31E0:
    /* 31E0  mov     word ptr ds:[4108h],di */
    ww(pDS, 0x4108, DI);
L31E4:
    /* 31E4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* nullsub_1  (+31E5) */
L31E5: /* _nullsub_1 */
    /* 31E5  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L31E6:
    /* 31E6  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_31E7  (+31E7)
       set_the_window (far through 52E4): SI = four words, left, top, right, bottom (y counting up).
       Removes the old window's edge guards (_2C44), copies the window to 3DF4, guards the new one
       (_2BF9), and copies it to FD71:0C30, where the 3D renderer's render_3d reads it
       (INSTANCE.ASM). */
L31E7: /* _seg003_0272_31E7 */
    /* 31E7  call    _seg003_0272_2C44 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2C44), 0x31EA)) != 0) return c;
L31EA:
    /* 31EA  push    si */
    push16(SI);
L31EB:
    /* 31EB  mov     di,3DF4h */
    DI = 0x3DF4;
L31EE:
    /* 31EE  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L31EF:
    /* 31EF  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L31F0:
    /* 31F0  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L31F1:
    /* 31F1  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L31F2:
    /* 31F2  pop     si */
    SI = pop16();
L31F3:
    /* 31F3  call    _seg003_0272_2BF9 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x2BF9), 0x31F6)) != 0) return c;
L31F6:
    /* 31F6  push    es */
    push16(asm_es);
L31F7:
    /* 31F7  mov     ax,seg dseg062_62a6 */
    AX = (uint16_t)(0x60B9 + PORT_LOAD_SEG);
L31FA:
    /* 31FA  mov     es,ax */
    SET_ES(AX);
L31FC:
    /* 31FC  mov     di,0C30h */
    DI = 0xC30;
L31FF:
    /* 31FF  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3200:
    /* 3200  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3201:
    /* 3201  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3202:
    /* 3202  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3203:
    /* 3203  pop     es */
    SET_ES(pop16());
L3204:
    /* 3204  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
