/* noclip.c: replaces src/3d/NOCLIP.ASM (seg004_1FD0, 1FD0..27CB of its
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

uint32_t asm_mod_NOCLIP(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x1FD0: goto L1FD0;
    case 0x1FD3: goto L1FD3;
    case 0x1FD4: goto L1FD4;
    case 0x1FD6: goto L1FD6;
    case 0x1FDA: goto L1FDA;
    case 0x1FDE: goto L1FDE;
    case 0x1FE2: goto L1FE2;
    case 0x1FE3: goto L1FE3;
    case 0x1FE4: goto L1FE4;
    case 0x1FE8: goto L1FE8;
    case 0x1FE9: goto L1FE9;
    case 0x1FEB: goto L1FEB;
    case 0x1FED: goto L1FED;
    case 0x1FF1: goto L1FF1;
    case 0x1FF4: goto L1FF4;
    case 0x1FF5: goto L1FF5;
    case 0x1FF6: goto L1FF6;
    case 0x1FF7: goto L1FF7;
    case 0x1FF8: goto L1FF8;
    case 0x1FF9: goto L1FF9;
    case 0x1FFD: goto L1FFD;
    case 0x1FFF: goto L1FFF;
    case 0x2000: goto L2000;
    case 0x2001: goto L2001;
    case 0x2005: goto L2005;
    case 0x2006: goto L2006;
    case 0x2008: goto L2008;
    case 0x200A: goto L200A;
    case 0x200E: goto L200E;
    case 0x2012: goto L2012;
    case 0x2013: goto L2013;
    case 0x2014: goto L2014;
    case 0x2015: goto L2015;
    case 0x2016: goto L2016;
    case 0x2017: goto L2017;
    case 0x201B: goto L201B;
    case 0x201D: goto L201D;
    case 0x201E: goto L201E;
    case 0x201F: goto L201F;
    case 0x2023: goto L2023;
    case 0x2024: goto L2024;
    case 0x2026: goto L2026;
    case 0x2028: goto L2028;
    case 0x202B: goto L202B;
    case 0x202C: goto L202C;
    case 0x202D: goto L202D;
    case 0x202F: goto L202F;
    case 0x2033: goto L2033;
    case 0x2034: goto L2034;
    case 0x2035: goto L2035;
    case 0x2036: goto L2036;
    case 0x2039: goto L2039;
    case 0x203A: goto L203A;
    case 0x203C: goto L203C;
    case 0x2040: goto L2040;
    case 0x2042: goto L2042;
    case 0x2044: goto L2044;
    case 0x2048: goto L2048;
    case 0x204A: goto L204A;
    case 0x204C: goto L204C;
    case 0x2050: goto L2050;
    case 0x2053: goto L2053;
    case 0x2054: goto L2054;
    case 0x2055: goto L2055;
    case 0x2057: goto L2057;
    case 0x205B: goto L205B;
    case 0x205C: goto L205C;
    case 0x205E: goto L205E;
    case 0x205F: goto L205F;
    case 0x2060: goto L2060;
    case 0x2062: goto L2062;
    case 0x2063: goto L2063;
    case 0x2064: goto L2064;
    case 0x2067: goto L2067;
    case 0x206A: goto L206A;
    case 0x206B: goto L206B;
    case 0x206D: goto L206D;
    case 0x206E: goto L206E;
    case 0x206F: goto L206F;
    case 0x2071: goto L2071;
    case 0x2072: goto L2072;
    case 0x2073: goto L2073;
    case 0x2076: goto L2076;
    case 0x2077: goto L2077;
    case 0x2078: goto L2078;
    case 0x207A: goto L207A;
    case 0x207C: goto L207C;
    case 0x207D: goto L207D;
    case 0x207E: goto L207E;
    case 0x2080: goto L2080;
    case 0x2084: goto L2084;
    case 0x2085: goto L2085;
    case 0x2086: goto L2086;
    case 0x2087: goto L2087;
    case 0x208A: goto L208A;
    case 0x208B: goto L208B;
    case 0x208D: goto L208D;
    case 0x2091: goto L2091;
    case 0x2093: goto L2093;
    case 0x2095: goto L2095;
    case 0x2099: goto L2099;
    case 0x209B: goto L209B;
    case 0x209D: goto L209D;
    case 0x20A1: goto L20A1;
    case 0x20A2: goto L20A2;
    case 0x20A3: goto L20A3;
    case 0x20A5: goto L20A5;
    case 0x20A9: goto L20A9;
    case 0x20AA: goto L20AA;
    case 0x20AC: goto L20AC;
    case 0x20AD: goto L20AD;
    case 0x20AE: goto L20AE;
    case 0x20B0: goto L20B0;
    case 0x20B1: goto L20B1;
    case 0x20B2: goto L20B2;
    case 0x20B5: goto L20B5;
    case 0x20B8: goto L20B8;
    case 0x20B9: goto L20B9;
    case 0x20BB: goto L20BB;
    case 0x20BC: goto L20BC;
    case 0x20BD: goto L20BD;
    case 0x20BF: goto L20BF;
    case 0x20C0: goto L20C0;
    case 0x20C1: goto L20C1;
    case 0x20C4: goto L20C4;
    case 0x20C5: goto L20C5;
    case 0x20C9: goto L20C9;
    case 0x20CC: goto L20CC;
    case 0x20CD: goto L20CD;
    case 0x20CF: goto L20CF;
    case 0x20D1: goto L20D1;
    case 0x20D4: goto L20D4;
    case 0x20D5: goto L20D5;
    case 0x20D6: goto L20D6;
    case 0x20D8: goto L20D8;
    case 0x20DC: goto L20DC;
    case 0x20DD: goto L20DD;
    case 0x20DE: goto L20DE;
    case 0x20DF: goto L20DF;
    case 0x20E2: goto L20E2;
    case 0x20E3: goto L20E3;
    case 0x20E5: goto L20E5;
    case 0x20E9: goto L20E9;
    case 0x20EB: goto L20EB;
    case 0x20EF: goto L20EF;
    case 0x20F2: goto L20F2;
    case 0x20F3: goto L20F3;
    case 0x20F4: goto L20F4;
    case 0x20F6: goto L20F6;
    case 0x20FA: goto L20FA;
    case 0x20FB: goto L20FB;
    case 0x20FD: goto L20FD;
    case 0x20FE: goto L20FE;
    case 0x20FF: goto L20FF;
    case 0x2100: goto L2100;
    case 0x2103: goto L2103;
    case 0x2106: goto L2106;
    case 0x2107: goto L2107;
    case 0x2109: goto L2109;
    case 0x210A: goto L210A;
    case 0x210B: goto L210B;
    case 0x210C: goto L210C;
    case 0x210F: goto L210F;
    case 0x2110: goto L2110;
    case 0x2111: goto L2111;
    case 0x2113: goto L2113;
    case 0x2115: goto L2115;
    case 0x2116: goto L2116;
    case 0x2117: goto L2117;
    case 0x2119: goto L2119;
    case 0x211D: goto L211D;
    case 0x211E: goto L211E;
    case 0x211F: goto L211F;
    case 0x2120: goto L2120;
    case 0x2123: goto L2123;
    case 0x2124: goto L2124;
    case 0x2126: goto L2126;
    case 0x212A: goto L212A;
    case 0x212C: goto L212C;
    case 0x2130: goto L2130;
    case 0x2131: goto L2131;
    case 0x2132: goto L2132;
    case 0x2134: goto L2134;
    case 0x2138: goto L2138;
    case 0x2139: goto L2139;
    case 0x213B: goto L213B;
    case 0x213C: goto L213C;
    case 0x213D: goto L213D;
    case 0x213E: goto L213E;
    case 0x2141: goto L2141;
    case 0x2144: goto L2144;
    case 0x2145: goto L2145;
    case 0x2147: goto L2147;
    case 0x2149: goto L2149;
    case 0x214A: goto L214A;
    case 0x214B: goto L214B;
    case 0x214C: goto L214C;
    case 0x214F: goto L214F;
    case 0x2150: goto L2150;
    case 0x2154: goto L2154;
    case 0x2156: goto L2156;
    case 0x2157: goto L2157;
    case 0x2159: goto L2159;
    case 0x215C: goto L215C;
    case 0x215E: goto L215E;
    case 0x2161: goto L2161;
    case 0x2162: goto L2162;
    case 0x2163: goto L2163;
    case 0x2165: goto L2165;
    case 0x2169: goto L2169;
    case 0x216D: goto L216D;
    case 0x2171: goto L2171;
    case 0x2173: goto L2173;
    case 0x2176: goto L2176;
    case 0x2177: goto L2177;
    case 0x217B: goto L217B;
    case 0x217F: goto L217F;
    case 0x2181: goto L2181;
    case 0x2184: goto L2184;
    case 0x2185: goto L2185;
    case 0x2187: goto L2187;
    case 0x218D: goto L218D;
    case 0x218E: goto L218E;
    case 0x218F: goto L218F;
    case 0x2193: goto L2193;
    case 0x2198: goto L2198;
    case 0x219A: goto L219A;
    case 0x219C: goto L219C;
    case 0x219D: goto L219D;
    case 0x219E: goto L219E;
    case 0x219F: goto L219F;
    case 0x21A3: goto L21A3;
    case 0x21A9: goto L21A9;
    case 0x21AA: goto L21AA;
    case 0x21AD: goto L21AD;
    case 0x21B0: goto L21B0;
    case 0x21B2: goto L21B2;
    case 0x21B5: goto L21B5;
    case 0x21B9: goto L21B9;
    case 0x21BB: goto L21BB;
    case 0x21BD: goto L21BD;
    case 0x21BF: goto L21BF;
    case 0x21C1: goto L21C1;
    case 0x21C5: goto L21C5;
    case 0x21C6: goto L21C6;
    case 0x21C7: goto L21C7;
    case 0x21CA: goto L21CA;
    case 0x21CC: goto L21CC;
    case 0x21CE: goto L21CE;
    case 0x21D1: goto L21D1;
    case 0x21D2: goto L21D2;
    case 0x21D4: goto L21D4;
    case 0x21D8: goto L21D8;
    case 0x21DA: goto L21DA;
    case 0x21DD: goto L21DD;
    case 0x21DE: goto L21DE;
    case 0x21E1: goto L21E1;
    case 0x21E3: goto L21E3;
    case 0x21E4: goto L21E4;
    case 0x21E8: goto L21E8;
    case 0x21ED: goto L21ED;
    case 0x21EF: goto L21EF;
    case 0x21F1: goto L21F1;
    case 0x21F2: goto L21F2;
    case 0x21F3: goto L21F3;
    case 0x21F4: goto L21F4;
    case 0x21F8: goto L21F8;
    case 0x21F9: goto L21F9;
    case 0x21FC: goto L21FC;
    case 0x21FD: goto L21FD;
    case 0x21FF: goto L21FF;
    case 0x2203: goto L2203;
    case 0x2207: goto L2207;
    case 0x220A: goto L220A;
    case 0x220E: goto L220E;
    case 0x2210: goto L2210;
    case 0x2213: goto L2213;
    case 0x2216: goto L2216;
    case 0x221B: goto L221B;
    case 0x221F: goto L221F;
    case 0x2221: goto L2221;
    case 0x2222: goto L2222;
    case 0x2223: goto L2223;
    case 0x2227: goto L2227;
    case 0x2228: goto L2228;
    case 0x222A: goto L222A;
    case 0x222C: goto L222C;
    case 0x222D: goto L222D;
    case 0x222E: goto L222E;
    case 0x2232: goto L2232;
    case 0x2235: goto L2235;
    case 0x2236: goto L2236;
    case 0x2237: goto L2237;
    case 0x223B: goto L223B;
    case 0x223C: goto L223C;
    case 0x223E: goto L223E;
    case 0x223F: goto L223F;
    case 0x2243: goto L2243;
    case 0x2245: goto L2245;
    case 0x2249: goto L2249;
    case 0x224D: goto L224D;
    case 0x2251: goto L2251;
    case 0x2253: goto L2253;
    case 0x2257: goto L2257;
    case 0x2259: goto L2259;
    case 0x225C: goto L225C;
    case 0x225E: goto L225E;
    case 0x2260: goto L2260;
    case 0x2263: goto L2263;
    case 0x2265: goto L2265;
    case 0x2267: goto L2267;
    case 0x2268: goto L2268;
    case 0x226A: goto L226A;
    case 0x226E: goto L226E;
    case 0x2272: goto L2272;
    case 0x2276: goto L2276;
    case 0x2277: goto L2277;
    case 0x2278: goto L2278;
    case 0x227C: goto L227C;
    case 0x227D: goto L227D;
    case 0x227F: goto L227F;
    case 0x2280: goto L2280;
    case 0x2284: goto L2284;
    case 0x2286: goto L2286;
    case 0x228A: goto L228A;
    case 0x228E: goto L228E;
    case 0x2292: goto L2292;
    case 0x2294: goto L2294;
    case 0x2298: goto L2298;
    case 0x229A: goto L229A;
    case 0x229D: goto L229D;
    case 0x229F: goto L229F;
    case 0x22A1: goto L22A1;
    case 0x22A4: goto L22A4;
    case 0x22A6: goto L22A6;
    case 0x22A8: goto L22A8;
    case 0x22A9: goto L22A9;
    case 0x22AB: goto L22AB;
    case 0x22AF: goto L22AF;
    case 0x22B3: goto L22B3;
    case 0x22B7: goto L22B7;
    case 0x22B8: goto L22B8;
    case 0x22B9: goto L22B9;
    case 0x22BD: goto L22BD;
    case 0x22BE: goto L22BE;
    case 0x22C0: goto L22C0;
    case 0x22C1: goto L22C1;
    case 0x22C5: goto L22C5;
    case 0x22C7: goto L22C7;
    case 0x22CB: goto L22CB;
    case 0x22CF: goto L22CF;
    case 0x22D3: goto L22D3;
    case 0x22D5: goto L22D5;
    case 0x22D9: goto L22D9;
    case 0x22DB: goto L22DB;
    case 0x22DE: goto L22DE;
    case 0x22E0: goto L22E0;
    case 0x22E2: goto L22E2;
    case 0x22E5: goto L22E5;
    case 0x22E7: goto L22E7;
    case 0x22E9: goto L22E9;
    case 0x22EA: goto L22EA;
    case 0x22EC: goto L22EC;
    case 0x22F0: goto L22F0;
    case 0x22F4: goto L22F4;
    case 0x22F8: goto L22F8;
    case 0x22F9: goto L22F9;
    case 0x22FA: goto L22FA;
    case 0x22FE: goto L22FE;
    case 0x22FF: goto L22FF;
    case 0x2301: goto L2301;
    case 0x2302: goto L2302;
    case 0x2304: goto L2304;
    case 0x2306: goto L2306;
    case 0x230A: goto L230A;
    case 0x230E: goto L230E;
    case 0x230F: goto L230F;
    case 0x2311: goto L2311;
    case 0x2312: goto L2312;
    case 0x2313: goto L2313;
    case 0x2315: goto L2315;
    case 0x2316: goto L2316;
    case 0x2317: goto L2317;
    case 0x2319: goto L2319;
    case 0x231A: goto L231A;
    case 0x231C: goto L231C;
    case 0x231D: goto L231D;
    case 0x231E: goto L231E;
    case 0x2322: goto L2322;
    case 0x2323: goto L2323;
    case 0x2325: goto L2325;
    case 0x2326: goto L2326;
    case 0x232A: goto L232A;
    case 0x232C: goto L232C;
    case 0x232E: goto L232E;
    case 0x2330: goto L2330;
    case 0x2331: goto L2331;
    case 0x2333: goto L2333;
    case 0x2336: goto L2336;
    case 0x2338: goto L2338;
    case 0x233A: goto L233A;
    case 0x233B: goto L233B;
    case 0x233C: goto L233C;
    case 0x233E: goto L233E;
    case 0x233F: goto L233F;
    case 0x2340: goto L2340;
    case 0x2342: goto L2342;
    case 0x2343: goto L2343;
    case 0x2344: goto L2344;
    case 0x2348: goto L2348;
    case 0x2349: goto L2349;
    case 0x234B: goto L234B;
    case 0x234C: goto L234C;
    case 0x2350: goto L2350;
    case 0x2352: goto L2352;
    case 0x2356: goto L2356;
    case 0x235A: goto L235A;
    case 0x235E: goto L235E;
    case 0x2360: goto L2360;
    case 0x2364: goto L2364;
    case 0x2366: goto L2366;
    case 0x2368: goto L2368;
    case 0x236C: goto L236C;
    case 0x236E: goto L236E;
    case 0x236F: goto L236F;
    case 0x2371: goto L2371;
    case 0x2375: goto L2375;
    case 0x2379: goto L2379;
    case 0x237D: goto L237D;
    case 0x237E: goto L237E;
    case 0x237F: goto L237F;
    case 0x2383: goto L2383;
    case 0x2384: goto L2384;
    case 0x2386: goto L2386;
    case 0x2387: goto L2387;
    case 0x238B: goto L238B;
    case 0x238D: goto L238D;
    case 0x2391: goto L2391;
    case 0x2395: goto L2395;
    case 0x2399: goto L2399;
    case 0x239B: goto L239B;
    case 0x239F: goto L239F;
    case 0x23A1: goto L23A1;
    case 0x23A4: goto L23A4;
    case 0x23A6: goto L23A6;
    case 0x23A8: goto L23A8;
    case 0x23A9: goto L23A9;
    case 0x23AB: goto L23AB;
    case 0x23AF: goto L23AF;
    case 0x23B3: goto L23B3;
    case 0x23B7: goto L23B7;
    case 0x23B8: goto L23B8;
    case 0x23B9: goto L23B9;
    case 0x23BD: goto L23BD;
    case 0x23C1: goto L23C1;
    case 0x23C2: goto L23C2;
    case 0x23C4: goto L23C4;
    case 0x23C6: goto L23C6;
    case 0x23CA: goto L23CA;
    case 0x23CC: goto L23CC;
    case 0x23CF: goto L23CF;
    case 0x23D1: goto L23D1;
    case 0x23D3: goto L23D3;
    case 0x23D6: goto L23D6;
    case 0x23D8: goto L23D8;
    case 0x23D9: goto L23D9;
    case 0x23DB: goto L23DB;
    case 0x23DD: goto L23DD;
    case 0x23DF: goto L23DF;
    case 0x23E3: goto L23E3;
    case 0x23E5: goto L23E5;
    case 0x23E8: goto L23E8;
    case 0x23EA: goto L23EA;
    case 0x23EC: goto L23EC;
    case 0x23EF: goto L23EF;
    case 0x23F1: goto L23F1;
    case 0x23F3: goto L23F3;
    case 0x23F4: goto L23F4;
    case 0x23F6: goto L23F6;
    case 0x23FA: goto L23FA;
    case 0x23FE: goto L23FE;
    case 0x2402: goto L2402;
    case 0x2403: goto L2403;
    case 0x2405: goto L2405;
    case 0x2409: goto L2409;
    case 0x240D: goto L240D;
    case 0x2411: goto L2411;
    case 0x2415: goto L2415;
    case 0x2416: goto L2416;
    case 0x2417: goto L2417;
    case 0x241B: goto L241B;
    case 0x241F: goto L241F;
    case 0x2420: goto L2420;
    case 0x2422: goto L2422;
    case 0x2424: goto L2424;
    case 0x2428: goto L2428;
    case 0x242A: goto L242A;
    case 0x242D: goto L242D;
    case 0x242F: goto L242F;
    case 0x2431: goto L2431;
    case 0x2434: goto L2434;
    case 0x2436: goto L2436;
    case 0x2437: goto L2437;
    case 0x2439: goto L2439;
    case 0x243B: goto L243B;
    case 0x243D: goto L243D;
    case 0x2441: goto L2441;
    case 0x2443: goto L2443;
    case 0x2446: goto L2446;
    case 0x2448: goto L2448;
    case 0x244A: goto L244A;
    case 0x244D: goto L244D;
    case 0x244F: goto L244F;
    case 0x2451: goto L2451;
    case 0x2452: goto L2452;
    case 0x2454: goto L2454;
    case 0x2458: goto L2458;
    case 0x245C: goto L245C;
    case 0x2460: goto L2460;
    case 0x2461: goto L2461;
    case 0x2463: goto L2463;
    case 0x2467: goto L2467;
    case 0x246B: goto L246B;
    case 0x246F: goto L246F;
    case 0x2473: goto L2473;
    case 0x2474: goto L2474;
    case 0x2475: goto L2475;
    case 0x2479: goto L2479;
    case 0x247D: goto L247D;
    case 0x247E: goto L247E;
    case 0x2480: goto L2480;
    case 0x2482: goto L2482;
    case 0x2486: goto L2486;
    case 0x2488: goto L2488;
    case 0x248B: goto L248B;
    case 0x248D: goto L248D;
    case 0x248F: goto L248F;
    case 0x2492: goto L2492;
    case 0x2494: goto L2494;
    case 0x2495: goto L2495;
    case 0x2497: goto L2497;
    case 0x2499: goto L2499;
    case 0x249B: goto L249B;
    case 0x249F: goto L249F;
    case 0x24A1: goto L24A1;
    case 0x24A4: goto L24A4;
    case 0x24A6: goto L24A6;
    case 0x24A8: goto L24A8;
    case 0x24AB: goto L24AB;
    case 0x24AD: goto L24AD;
    case 0x24AF: goto L24AF;
    case 0x24B0: goto L24B0;
    case 0x24B2: goto L24B2;
    case 0x24B6: goto L24B6;
    case 0x24BA: goto L24BA;
    case 0x24BE: goto L24BE;
    case 0x24BF: goto L24BF;
    case 0x24C1: goto L24C1;
    case 0x24C5: goto L24C5;
    case 0x24C9: goto L24C9;
    case 0x24CD: goto L24CD;
    case 0x24D1: goto L24D1;
    case 0x24D2: goto L24D2;
    case 0x24D3: goto L24D3;
    case 0x24D7: goto L24D7;
    case 0x24DB: goto L24DB;
    case 0x24DC: goto L24DC;
    case 0x24DE: goto L24DE;
    case 0x24E0: goto L24E0;
    case 0x24E4: goto L24E4;
    case 0x24E6: goto L24E6;
    case 0x24E9: goto L24E9;
    case 0x24EB: goto L24EB;
    case 0x24ED: goto L24ED;
    case 0x24EE: goto L24EE;
    case 0x24F0: goto L24F0;
    case 0x24F2: goto L24F2;
    case 0x24F6: goto L24F6;
    case 0x24F8: goto L24F8;
    case 0x24FB: goto L24FB;
    case 0x24FD: goto L24FD;
    case 0x24FF: goto L24FF;
    case 0x2500: goto L2500;
    case 0x2502: goto L2502;
    case 0x2506: goto L2506;
    case 0x250A: goto L250A;
    case 0x250E: goto L250E;
    case 0x250F: goto L250F;
    case 0x2511: goto L2511;
    case 0x2515: goto L2515;
    case 0x2519: goto L2519;
    case 0x251D: goto L251D;
    case 0x2521: goto L2521;
    case 0x2522: goto L2522;
    case 0x2523: goto L2523;
    case 0x2527: goto L2527;
    case 0x252B: goto L252B;
    case 0x252C: goto L252C;
    case 0x252E: goto L252E;
    case 0x2530: goto L2530;
    case 0x2534: goto L2534;
    case 0x2536: goto L2536;
    case 0x2539: goto L2539;
    case 0x253B: goto L253B;
    case 0x253D: goto L253D;
    case 0x253E: goto L253E;
    case 0x2540: goto L2540;
    case 0x2542: goto L2542;
    case 0x2544: goto L2544;
    case 0x2545: goto L2545;
    case 0x2547: goto L2547;
    case 0x254B: goto L254B;
    case 0x254F: goto L254F;
    case 0x2553: goto L2553;
    case 0x2554: goto L2554;
    case 0x2556: goto L2556;
    case 0x255A: goto L255A;
    case 0x255E: goto L255E;
    case 0x2562: goto L2562;
    case 0x2566: goto L2566;
    case 0x2567: goto L2567;
    case 0x2568: goto L2568;
    case 0x256C: goto L256C;
    case 0x2570: goto L2570;
    case 0x2571: goto L2571;
    case 0x2573: goto L2573;
    case 0x2575: goto L2575;
    case 0x2579: goto L2579;
    case 0x257B: goto L257B;
    case 0x257E: goto L257E;
    case 0x2580: goto L2580;
    case 0x2582: goto L2582;
    case 0x2583: goto L2583;
    case 0x2585: goto L2585;
    case 0x2587: goto L2587;
    case 0x2589: goto L2589;
    case 0x258A: goto L258A;
    case 0x258C: goto L258C;
    case 0x2590: goto L2590;
    case 0x2594: goto L2594;
    case 0x2598: goto L2598;
    case 0x2599: goto L2599;
    case 0x259B: goto L259B;
    case 0x259F: goto L259F;
    case 0x25A3: goto L25A3;
    case 0x25A7: goto L25A7;
    case 0x25AB: goto L25AB;
    case 0x25AC: goto L25AC;
    case 0x25AD: goto L25AD;
    case 0x25B1: goto L25B1;
    case 0x25B2: goto L25B2;
    case 0x25B4: goto L25B4;
    case 0x25B5: goto L25B5;
    case 0x25B7: goto L25B7;
    case 0x25B8: goto L25B8;
    case 0x25BC: goto L25BC;
    case 0x25C0: goto L25C0;
    case 0x25C4: goto L25C4;
    case 0x25C6: goto L25C6;
    case 0x25CA: goto L25CA;
    case 0x25CC: goto L25CC;
    case 0x25D0: goto L25D0;
    case 0x25D4: goto L25D4;
    case 0x25D6: goto L25D6;
    case 0x25DA: goto L25DA;
    case 0x25DC: goto L25DC;
    case 0x25DE: goto L25DE;
    case 0x25E2: goto L25E2;
    case 0x25E6: goto L25E6;
    case 0x25EA: goto L25EA;
    case 0x25EC: goto L25EC;
    case 0x25F0: goto L25F0;
    case 0x25F2: goto L25F2;
    case 0x25F6: goto L25F6;
    case 0x25FA: goto L25FA;
    case 0x25FC: goto L25FC;
    case 0x2600: goto L2600;
    case 0x2602: goto L2602;
    case 0x2607: goto L2607;
    case 0x2608: goto L2608;
    case 0x2609: goto L2609;
    case 0x260A: goto L260A;
    case 0x260E: goto L260E;
    case 0x2612: goto L2612;
    case 0x2616: goto L2616;
    case 0x2618: goto L2618;
    case 0x261A: goto L261A;
    case 0x261E: goto L261E;
    case 0x2620: goto L2620;
    case 0x2622: goto L2622;
    case 0x2625: goto L2625;
    case 0x2627: goto L2627;
    case 0x2629: goto L2629;
    case 0x262B: goto L262B;
    case 0x262E: goto L262E;
    case 0x2632: goto L2632;
    case 0x2636: goto L2636;
    case 0x263A: goto L263A;
    case 0x263D: goto L263D;
    case 0x263E: goto L263E;
    case 0x2640: goto L2640;
    case 0x2641: goto L2641;
    case 0x2642: goto L2642;
    case 0x2644: goto L2644;
    case 0x2645: goto L2645;
    case 0x2647: goto L2647;
    case 0x2648: goto L2648;
    case 0x264C: goto L264C;
    case 0x264E: goto L264E;
    case 0x2650: goto L2650;
    case 0x2652: goto L2652;
    case 0x2654: goto L2654;
    case 0x2655: goto L2655;
    case 0x2657: goto L2657;
    case 0x2659: goto L2659;
    case 0x265D: goto L265D;
    case 0x265E: goto L265E;
    case 0x2660: goto L2660;
    case 0x2662: goto L2662;
    case 0x2666: goto L2666;
    case 0x2667: goto L2667;
    case 0x2669: goto L2669;
    case 0x266B: goto L266B;
    case 0x266F: goto L266F;
    case 0x2671: goto L2671;
    case 0x2674: goto L2674;
    case 0x2677: goto L2677;
    case 0x267A: goto L267A;
    case 0x267D: goto L267D;
    case 0x267F: goto L267F;
    case 0x2680: goto L2680;
    case 0x2682: goto L2682;
    case 0x2684: goto L2684;
    case 0x2688: goto L2688;
    case 0x2689: goto L2689;
    case 0x268B: goto L268B;
    case 0x268D: goto L268D;
    case 0x2691: goto L2691;
    case 0x2692: goto L2692;
    case 0x2694: goto L2694;
    case 0x2696: goto L2696;
    case 0x269A: goto L269A;
    case 0x269C: goto L269C;
    case 0x269F: goto L269F;
    case 0x26A2: goto L26A2;
    case 0x26A5: goto L26A5;
    case 0x26A7: goto L26A7;
    case 0x26A8: goto L26A8;
    case 0x26AA: goto L26AA;
    case 0x26AC: goto L26AC;
    case 0x26B0: goto L26B0;
    case 0x26B1: goto L26B1;
    case 0x26B3: goto L26B3;
    case 0x26B5: goto L26B5;
    case 0x26B9: goto L26B9;
    case 0x26BA: goto L26BA;
    case 0x26BC: goto L26BC;
    case 0x26BE: goto L26BE;
    case 0x26C2: goto L26C2;
    case 0x26C5: goto L26C5;
    case 0x26C8: goto L26C8;
    case 0x26CB: goto L26CB;
    case 0x26CE: goto L26CE;
    case 0x26D1: goto L26D1;
    case 0x26D3: goto L26D3;
    case 0x26D5: goto L26D5;
    case 0x26D6: goto L26D6;
    case 0x26D7: goto L26D7;
    case 0x26D8: goto L26D8;
    case 0x26DC: goto L26DC;
    case 0x26E0: goto L26E0;
    case 0x26E4: goto L26E4;
    case 0x26E8: goto L26E8;
    case 0x26EB: goto L26EB;
    case 0x26EF: goto L26EF;
    case 0x26F3: goto L26F3;
    case 0x26F7: goto L26F7;
    case 0x26FA: goto L26FA;
    case 0x26FD: goto L26FD;
    case 0x2700: goto L2700;
    case 0x2703: goto L2703;
    case 0x2706: goto L2706;
    case 0x2709: goto L2709;
    case 0x270C: goto L270C;
    case 0x270F: goto L270F;
    case 0x2712: goto L2712;
    case 0x2715: goto L2715;
    case 0x2716: goto L2716;
    case 0x2718: goto L2718;
    case 0x2719: goto L2719;
    case 0x271A: goto L271A;
    case 0x271C: goto L271C;
    case 0x271D: goto L271D;
    case 0x271F: goto L271F;
    case 0x2720: goto L2720;
    case 0x2724: goto L2724;
    case 0x2726: goto L2726;
    case 0x2728: goto L2728;
    case 0x272A: goto L272A;
    case 0x272C: goto L272C;
    case 0x272D: goto L272D;
    case 0x272F: goto L272F;
    case 0x2731: goto L2731;
    case 0x2735: goto L2735;
    case 0x2736: goto L2736;
    case 0x2738: goto L2738;
    case 0x273A: goto L273A;
    case 0x273E: goto L273E;
    case 0x273F: goto L273F;
    case 0x2741: goto L2741;
    case 0x2743: goto L2743;
    case 0x2747: goto L2747;
    case 0x2749: goto L2749;
    case 0x274C: goto L274C;
    case 0x274F: goto L274F;
    case 0x2752: goto L2752;
    case 0x2755: goto L2755;
    case 0x2757: goto L2757;
    case 0x2758: goto L2758;
    case 0x275A: goto L275A;
    case 0x275C: goto L275C;
    case 0x2760: goto L2760;
    case 0x2761: goto L2761;
    case 0x2763: goto L2763;
    case 0x2765: goto L2765;
    case 0x2769: goto L2769;
    case 0x276A: goto L276A;
    case 0x276C: goto L276C;
    case 0x276E: goto L276E;
    case 0x2772: goto L2772;
    case 0x2774: goto L2774;
    case 0x2777: goto L2777;
    case 0x277A: goto L277A;
    case 0x277D: goto L277D;
    case 0x277F: goto L277F;
    case 0x2780: goto L2780;
    case 0x2782: goto L2782;
    case 0x2784: goto L2784;
    case 0x2788: goto L2788;
    case 0x2789: goto L2789;
    case 0x278B: goto L278B;
    case 0x278D: goto L278D;
    case 0x2791: goto L2791;
    case 0x2792: goto L2792;
    case 0x2794: goto L2794;
    case 0x2796: goto L2796;
    case 0x279A: goto L279A;
    case 0x279D: goto L279D;
    case 0x27A0: goto L27A0;
    case 0x27A3: goto L27A3;
    case 0x27A6: goto L27A6;
    case 0x27A9: goto L27A9;
    case 0x27AB: goto L27AB;
    case 0x27AD: goto L27AD;
    case 0x27AE: goto L27AE;
    case 0x27AF: goto L27AF;
    case 0x27B0: goto L27B0;
    case 0x27B2: goto L27B2;
    case 0x27B3: goto L27B3;
    case 0x27B4: goto L27B4;
    case 0x27B8: goto L27B8;
    case 0x27B9: goto L27B9;
    case 0x27BD: goto L27BD;
    case 0x27C1: goto L27C1;
    case 0x27C5: goto L27C5;
    case 0x27C6: goto L27C6;
    case 0x27C7: goto L27C7;
    default: asm_bad_entry("NOCLIP.ASM", entry);
    }

    /* seg004_1FD0  (+1FD0)
       Opcode 7Ah in the no-clip set: INTERP.ASM's do_defres without the clip codes. */
L1FD0: /* _seg004_1FD0 */
    /* 1FD0  call    _load_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x50BE), 0x1FD3)) != 0) return c;
L1FD3:
    /* 1FD3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1FD4:
    /* 1FD4  mov     di,ax */
    DI = AX;
L1FD6:
    /* 1FD6  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L1FDA:
    /* 1FDA  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L1FDE:
    /* 1FDE  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L1FE2:
    /* 1FE2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1FE3:
    /* 1FE3  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1FE4:
    /* 1FE4  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_1FE8  (+1FE8)
       Opcode 7Ch in the no-clip set: INTERP.ASM's do_strres without the clip codes. */
L1FE8: /* _seg004_1FE8 */
    /* 1FE8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1FE9:
    /* 1FE9  mov     dx,si */
    DX = SI;
L1FEB:
    /* 1FEB  mov     si,ax */
    SI = AX;
L1FED:
    /* 1FED  add     si,PNT_X */
    SI = add16(SI, 0x1620, 0);
L1FF1:
    /* 1FF1  mov     di,VBUF0 */
    DI = 0x309;
L1FF4:
    /* 1FF4  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1FF5:
    /* 1FF5  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1FF6:
    /* 1FF6  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1FF7:
    /* 1FF7  inc     di */
    DI = inc16(DI);
L1FF8:
    /* 1FF8  inc     di */
    DI = inc16(DI);
L1FF9:
    /* 1FF9  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L1FFD:
    /* 1FFD  mov     si,dx */
    SI = DX;
L1FFF:
    /* 1FFF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2000:
    /* 2000  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2001:
    /* 2001  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2005  (+2005)
       Opcode 8Eh in the no-clip set: INTERP.ASM's do_cntres without the clip codes. */
L2005: /* _seg004_2005 */
    /* 2005  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2006:
    /* 2006  mov     dx,si */
    DX = SI;
L2008:
    /* 2008  mov     si,ax */
    SI = AX;
L200A:
    /* 200A  add     si,PNT_X */
    SI = add16(SI, 0x1620, 0);
L200E:
    /* 200E  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L2012:
    /* 2012  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2013:
    /* 2013  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2014:
    /* 2014  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2015:
    /* 2015  inc     di */
    DI = inc16(DI);
L2016:
    /* 2016  inc     di */
    DI = inc16(DI);
L2017:
    /* 2017  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L201B:
    /* 201B  mov     si,dx */
    SI = DX;
L201D:
    /* 201D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L201E:
    /* 201E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L201F:
    /* 201F  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2023  (+2023)
       Opcode 9Ch in the no-clip set: INTERP.ASM's do_rod without the clip codes. */
L2023: /* _seg004_2023 */
    /* 2023  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2024:
    /* 2024  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L2026:
    /* 2026  jne     short L203C */
    if (!ZF) goto L203C;
L2028:
    /* 2028  mov     di,VBUF0 */
    DI = 0x309;
L202B:
    /* 202B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L202C:
    /* 202C  push    si */
    push16(SI);
L202D:
    /* 202D  mov     si,ax */
    SI = AX;
L202F:
    /* 202F  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2033:
    /* 2033  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2034:
    /* 2034  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2035:
    /* 2035  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2036:
    /* 2036  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L2039:
    /* 2039  pop     si */
    SI = pop16();
L203A:
    /* 203A  jmp     short L2077 */
    goto L2077;
L203C: /* L203C */
    /* 203C  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L2040:
    /* 2040  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2042:
    /* 2042  mov     cx,ax */
    CX = AX;
L2044:
    /* 2044  imul    word ptr ds:[26D2h] */
    imul16(rw(pDS, 0x26D2));
L2048:
    /* 2048  mov     ax,cx */
    AX = CX;
L204A:
    /* 204A  mov     cx,dx */
    CX = DX;
L204C:
    /* 204C  imul    word ptr ds:[26D0h] */
    imul16(rw(pDS, 0x26D0));
L2050:
    /* 2050  mov     di,VBUF0 */
    DI = 0x309;
L2053:
    /* 2053  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2054:
    /* 2054  push    si */
    push16(SI);
L2055:
    /* 2055  mov     si,ax */
    SI = AX;
L2057:
    /* 2057  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L205B:
    /* 205B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L205C:
    /* 205C  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L205E:
    /* 205E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L205F:
    /* 205F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2060:
    /* 2060  add     ax,cx */
    AX = (uint16_t)(AX + CX);
L2062:
    /* 2062  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2063:
    /* 2063  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2064:
    /* 2064  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L2067:
    /* 2067  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L206A:
    /* 206A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L206B:
    /* 206B  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L206D:
    /* 206D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L206E:
    /* 206E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L206F:
    /* 206F  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L2071:
    /* 2071  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2072:
    /* 2072  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2073:
    /* 2073  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L2076:
    /* 2076  pop     si */
    SI = pop16();
L2077: /* L2077 */
    /* 2077  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2078:
    /* 2078  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L207A:
    /* 207A  jne     short L208D */
    if (!ZF) goto L208D;
L207C:
    /* 207C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L207D:
    /* 207D  push    si */
    push16(SI);
L207E:
    /* 207E  mov     si,ax */
    SI = AX;
L2080:
    /* 2080  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2084:
    /* 2084  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2085:
    /* 2085  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2086:
    /* 2086  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2087:
    /* 2087  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L208A:
    /* 208A  pop     si */
    SI = pop16();
L208B:
    /* 208B  jmp     short L20C5 */
    goto L20C5;
L208D: /* L208D */
    /* 208D  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L2091:
    /* 2091  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2093:
    /* 2093  mov     cx,ax */
    CX = AX;
L2095:
    /* 2095  imul    word ptr ds:[26D2h] */
    imul16(rw(pDS, 0x26D2));
L2099:
    /* 2099  mov     ax,cx */
    AX = CX;
L209B:
    /* 209B  mov     cx,dx */
    CX = DX;
L209D:
    /* 209D  imul    word ptr ds:[26D0h] */
    imul16(rw(pDS, 0x26D0));
L20A1:
    /* 20A1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20A2:
    /* 20A2  push    si */
    push16(SI);
L20A3:
    /* 20A3  mov     si,ax */
    SI = AX;
L20A5:
    /* 20A5  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L20A9:
    /* 20A9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20AA:
    /* 20AA  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L20AC:
    /* 20AC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L20AD:
    /* 20AD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20AE:
    /* 20AE  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L20B0:
    /* 20B0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L20B1:
    /* 20B1  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L20B2:
    /* 20B2  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L20B5:
    /* 20B5  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L20B8:
    /* 20B8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20B9:
    /* 20B9  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L20BB:
    /* 20BB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L20BC:
    /* 20BC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20BD:
    /* 20BD  add     ax,cx */
    AX = (uint16_t)(AX + CX);
L20BF:
    /* 20BF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L20C0:
    /* 20C0  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L20C1:
    /* 20C1  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L20C4:
    /* 20C4  pop     si */
    SI = pop16();
L20C5: /* L20C5 */
    /* 20C5  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L20C9:
    /* 20C9  jmp     _seg004_21A3 */
    goto L21A3;

    /* seg004_20CC  (+20CC)
       The level-view variant of the no-clip do_rod (as flat_rod in INTERP.ASM), installed by check_flat. */
L20CC: /* _seg004_20CC */
    /* 20CC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20CD:
    /* 20CD  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L20CF:
    /* 20CF  jne     short L20E5 */
    if (!ZF) goto L20E5;
L20D1:
    /* 20D1  mov     di,VBUF0 */
    DI = 0x309;
L20D4:
    /* 20D4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20D5:
    /* 20D5  push    si */
    push16(SI);
L20D6:
    /* 20D6  mov     si,ax */
    SI = AX;
L20D8:
    /* 20D8  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L20DC:
    /* 20DC  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L20DD:
    /* 20DD  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L20DE:
    /* 20DE  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L20DF:
    /* 20DF  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L20E2:
    /* 20E2  pop     si */
    SI = pop16();
L20E3:
    /* 20E3  jmp     short L2110 */
    goto L2110;
L20E5: /* L20E5 */
    /* 20E5  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L20E9:
    /* 20E9  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L20EB:
    /* 20EB  imul    word ptr ds:[272Ch] */
    imul16(rw(pDS, 0x272C));
L20EF:
    /* 20EF  mov     di,VBUF0 */
    DI = 0x309;
L20F2:
    /* 20F2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20F3:
    /* 20F3  push    si */
    push16(SI);
L20F4:
    /* 20F4  mov     si,ax */
    SI = AX;
L20F6:
    /* 20F6  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L20FA:
    /* 20FA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20FB:
    /* 20FB  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L20FD:
    /* 20FD  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L20FE:
    /* 20FE  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L20FF:
    /* 20FF  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2100:
    /* 2100  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L2103:
    /* 2103  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L2106:
    /* 2106  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2107:
    /* 2107  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L2109:
    /* 2109  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L210A:
    /* 210A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L210B:
    /* 210B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L210C:
    /* 210C  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L210F:
    /* 210F  pop     si */
    SI = pop16();
L2110: /* L2110 */
    /* 2110  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2111:
    /* 2111  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L2113:
    /* 2113  jne     short L2126 */
    if (!ZF) goto L2126;
L2115:
    /* 2115  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2116:
    /* 2116  push    si */
    push16(SI);
L2117:
    /* 2117  mov     si,ax */
    SI = AX;
L2119:
    /* 2119  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L211D:
    /* 211D  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L211E:
    /* 211E  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L211F:
    /* 211F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2120:
    /* 2120  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L2123:
    /* 2123  pop     si */
    SI = pop16();
L2124:
    /* 2124  jmp     short L2150 */
    goto L2150;
L2126: /* L2126 */
    /* 2126  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L212A:
    /* 212A  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L212C:
    /* 212C  imul    word ptr ds:[272Ch] */
    imul16(rw(pDS, 0x272C));
L2130:
    /* 2130  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2131:
    /* 2131  push    si */
    push16(SI);
L2132:
    /* 2132  mov     si,ax */
    SI = AX;
L2134:
    /* 2134  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2138:
    /* 2138  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2139:
    /* 2139  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L213B:
    /* 213B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L213C:
    /* 213C  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L213D:
    /* 213D  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L213E:
    /* 213E  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L2141:
    /* 2141  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L2144:
    /* 2144  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2145:
    /* 2145  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L2147:
    /* 2147  mov     bx,ax */
    BX = AX;
L2149:
    /* 2149  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L214A:
    /* 214A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L214B:
    /* 214B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L214C:
    /* 214C  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L214F:
    /* 214F  pop     si */
    SI = pop16();
L2150: /* L2150 */
    /* 2150  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L2154:
    /* 2154  jmp     short _seg004_21A3 */
    goto L21A3;

    /* seg004_2156  (+2156)
       Opcode 7Eh in the no-clip set: INTERP.ASM's do_polyres without the clip codes. */
L2156: /* _seg004_2156 */
    /* 2156  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2157:
    /* 2157  mov     cx,ax */
    CX = AX;
L2159:
    /* 2159  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L215C:
    /* 215C  mov     es,ax */
    SET_ES(AX);
L215E:
    /* 215E  mov     di,415Ch */
    DI = 0x415C;
L2161:
    /* 2161  push    cx */
    push16(CX);
L2162: /* L2162 */
    /* 2162  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2163:
    /* 2163  mov     bx,ax */
    BX = AX;
L2165:
    /* 2165  mov     bp,word ptr [bx+PNT_Z] */
    BP = rw(pDS, BX + 0x1624);
L2169:
    /* 2169  mov     ax,word ptr [bx+PNT_X] */
    AX = rw(pDS, BX + 0x1620);
L216D:
    /* 216D  imul    word ptr ds:[PROJ_X] */
    imul16(rw(pDS, 0x26B2));
L2171:
    /* 2171  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x2171, 2)) != 0) return c;
L2173: /* _seg004_2173 */
    /* 2173  add     ax,4D2h */
    AX = (uint16_t)(AX + rw(CODE004, 0x2174));
L2176:
    /* 2176  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2177:
    /* 2177  mov     ax,word ptr [bx+PNT_Y] */
    AX = rw(pDS, BX + 0x1622);
L217B:
    /* 217B  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L217F:
    /* 217F  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x217F, 2)) != 0) return c;
L2181: /* _seg004_2181 */
    /* 2181  add     ax,4D2h */
    AX = (uint16_t)(AX + rw(CODE004, 0x2182));
L2184:
    /* 2184  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2185:
    /* 2185  loop    L2162 */
    if (--CX) goto L2162;
L2187:
    /* 2187  mov     word ptr ds:[2884h],1 */
    ww(pDS, 0x2884, 0x1);
L218D:
    /* 218D  pop     cx */
    CX = pop16();
L218E:
    /* 218E  push    si */
    push16(SI);
L218F:
    /* 218F  mov     bp,word ptr ds:[POLY_FILL] */
    BP = rw(pDS, 0x15F6);
L2193:
    /* 2193  call    far ptr _seg003_5B0B */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x5B0B), 0x06E7 + PORT_LOAD_SEG, 0x2198)) != 0) return c;
L2198:
    /* 2198  mov     ax,ds */
    AX = asm_ds;
L219A:
    /* 219A  mov     es,ax */
    SET_ES(AX);
L219C:
    /* 219C  pop     si */
    SI = pop16();
L219D:
    /* 219D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L219E:
    /* 219E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L219F:
    /* 219F  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_21A3  (+21A3)
       Opcode 80h in the no-clip set: INTERP.ASM's do_closure without the clip codes. */
L21A3: /* _seg004_21A3 */
    /* 21A3  mov     word ptr ds:[2884h],1 */
    ww(pDS, 0x2884, 0x1);
L21A9:
    /* 21A9  push    si */
    push16(SI);
L21AA:
    /* 21AA  mov     si,VBUF0 */
    SI = 0x309;
L21AD:
    /* 21AD  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L21B0:
    /* 21B0  mov     es,ax */
    SET_ES(AX);
L21B2:
    /* 21B2  mov     di,415Ch */
    DI = 0x415C;
L21B5:
    /* 21B5  mov     cx,word ptr ds:[VB_END] */
    CX = rw(pDS, 0x305);
L21B9:
    /* 21B9  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L21BB:
    /* 21BB  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L21BD:
    /* 21BD  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L21BF:
    /* 21BF  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L21C1:
    /* 21C1  mov     bx,word ptr ds:[PROJ_X] */
    BX = rw(pDS, 0x26B2);
L21C5:
    /* 21C5  push    cx */
    push16(CX);
L21C6: /* L21C6 */
    /* 21C6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L21C7:
    /* 21C7  mov     bp,word ptr [si+2] */
    BP = rw(pDS, SI + 0x2);
L21CA:
    /* 21CA  imul    bx */
    imul16(BX);
L21CC:
    /* 21CC  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x21CC, 2)) != 0) return c;
L21CE: /* _seg004_21CE */
    /* 21CE  add     ax,4D2h */
    AX = (uint16_t)(AX + rw(CODE004, 0x21CF));
L21D1:
    /* 21D1  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L21D2:
    /* 21D2  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L21D4:
    /* 21D4  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L21D8:
    /* 21D8  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x21D8, 2)) != 0) return c;
L21DA: /* _seg004_21DA */
    /* 21DA  add     ax,4D2h */
    AX = (uint16_t)(AX + rw(CODE004, 0x21DB));
L21DD:
    /* 21DD  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L21DE:
    /* 21DE  add     si,6 */
    SI = (uint16_t)(SI + 0x6);
L21E1:
    /* 21E1  loop    L21C6 */
    if (--CX) goto L21C6;
L21E3:
    /* 21E3  pop     cx */
    CX = pop16();
L21E4:
    /* 21E4  mov     bp,word ptr ds:[POLY_FILL] */
    BP = rw(pDS, 0x15F6);
L21E8:
    /* 21E8  call    far ptr _seg003_5B0B */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x5B0B), 0x06E7 + PORT_LOAD_SEG, 0x21ED)) != 0) return c;
L21ED:
    /* 21ED  mov     ax,ds */
    AX = asm_ds;
L21EF:
    /* 21EF  mov     es,ax */
    SET_ES(AX);
L21F1:
    /* 21F1  pop     si */
    SI = pop16();
L21F2:
    /* 21F2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L21F3:
    /* 21F3  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L21F4:
    /* 21F4  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_21F8  (+21F8)
       Opcode 82h in the no-clip set: INTERP.ASM's do_multires without the clip codes. */
L21F8: /* _seg004_21F8 */
    /* 21F8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L21F9:
    /* 21F9  mov     word ptr ds:[271Ch],ax */
    ww(pDS, 0x271C, AX);
L21FC:
    /* 21FC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L21FD:
    /* 21FD  mov     di,ax */
    DI = AX;
L21FF:
    /* 21FF  add     di,PNT_X */
    DI = (uint16_t)(DI + 0x1620);
L2203:
    /* 2203  mov     word ptr ds:[271Eh],di */
    ww(pDS, 0x271E, DI);
L2207: /* L2207 */
    /* 2207  call    _load_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x50BE), 0x220A)) != 0) return c;
L220A:
    /* 220A  mov     di,word ptr ds:[271Eh] */
    DI = rw(pDS, 0x271E);
L220E:
    /* 220E  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2210:
    /* 2210  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L2213:
    /* 2213  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L2216:
    /* 2216  add     word ptr ds:[271Eh],8 */
    ww(pDS, 0x271E, add16(rw(pDS, 0x271E), 0x8, 0));
L221B:
    /* 221B  dec     word ptr ds:[271Ch] */
    ww(pDS, 0x271C, dec16(rw(pDS, 0x271C)));
L221F:
    /* 221F  jne     L2207 */
    if (!ZF) goto L2207;
L2221:
    /* 2221  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2222:
    /* 2222  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2223:
    /* 2223  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2227  (+2227)
       Opcode 84h in the no-clip set: INTERP.ASM's do_hull without the clip codes. */
L2227: /* _seg004_2227 */
    /* 2227  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2228:
    /* 2228  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L222A:
    /* 222A  add     si,ax */
    SI = add16(SI, AX, 0);
L222C:
    /* 222C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L222D:
    /* 222D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L222E:
    /* 222E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2232  (+2232)
       Opcode 78h in the no-clip set: INTERP.ASM's do_parapiped without the clip codes. */
L2232: /* _seg004_2232 */
    /* 2232  add     si,0Ah */
    SI = add16(SI, 0xA, 0);
L2235:
    /* 2235  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2236:
    /* 2236  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2237:
    /* 2237  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_223B  (+223B)
       Opcode 86h in the no-clip set: INTERP.ASM's do_x_rel without the clip codes. */
L223B: /* _seg004_223B */
    /* 223B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L223C:
    /* 223C  mov     di,ax */
    DI = AX;
L223E:
    /* 223E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L223F:
    /* 223F  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L2243:
    /* 2243  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2245:
    /* 2245  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L2249:
    /* 2249  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L224D:
    /* 224D  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L2251:
    /* 2251  mov     di,ax */
    DI = AX;
L2253:
    /* 2253  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L2257:
    /* 2257  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L2259:
    /* 2259  mov     ax,word ptr ds:[MAT_XY] */
    AX = rw(pDS, 0x1604);
L225C:
    /* 225C  imul    di */
    imul16(DI);
L225E:
    /* 225E  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L2260:
    /* 2260  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L2263:
    /* 2263  imul    di */
    imul16(DI);
L2265:
    /* 2265  add     bp,dx */
    BP = add16(BP, DX, 0);
L2267:
    /* 2267  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2268:
    /* 2268  mov     di,ax */
    DI = AX;
L226A:
    /* 226A  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L226E:
    /* 226E  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L2272:
    /* 2272  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L2276:
    /* 2276  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2277:
    /* 2277  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2278:
    /* 2278  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_227C  (+227C)
       Opcode 88h in the no-clip set: INTERP.ASM's do_y_rel without the clip codes. */
L227C: /* _seg004_227C */
    /* 227C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L227D:
    /* 227D  mov     di,ax */
    DI = AX;
L227F:
    /* 227F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2280:
    /* 2280  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L2284:
    /* 2284  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2286:
    /* 2286  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L228A:
    /* 228A  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L228E:
    /* 228E  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L2292:
    /* 2292  mov     di,ax */
    DI = AX;
L2294:
    /* 2294  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L2298:
    /* 2298  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L229A:
    /* 229A  mov     ax,word ptr ds:[MAT_YY] */
    AX = rw(pDS, 0x160A);
L229D:
    /* 229D  imul    di */
    imul16(DI);
L229F:
    /* 229F  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L22A1:
    /* 22A1  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L22A4:
    /* 22A4  imul    di */
    imul16(DI);
L22A6:
    /* 22A6  add     bp,dx */
    BP = add16(BP, DX, 0);
L22A8:
    /* 22A8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22A9:
    /* 22A9  mov     di,ax */
    DI = AX;
L22AB:
    /* 22AB  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L22AF:
    /* 22AF  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L22B3:
    /* 22B3  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L22B7:
    /* 22B7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22B8:
    /* 22B8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L22B9:
    /* 22B9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_22BD  (+22BD)
       Opcode 8Ah in the no-clip set: INTERP.ASM's do_z_rel without the clip codes. */
L22BD: /* _seg004_22BD */
    /* 22BD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22BE:
    /* 22BE  mov     di,ax */
    DI = AX;
L22C0:
    /* 22C0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22C1:
    /* 22C1  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L22C5:
    /* 22C5  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L22C7:
    /* 22C7  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L22CB:
    /* 22CB  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L22CF:
    /* 22CF  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L22D3:
    /* 22D3  mov     di,ax */
    DI = AX;
L22D5:
    /* 22D5  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L22D9:
    /* 22D9  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L22DB:
    /* 22DB  mov     ax,word ptr ds:[MAT_ZY] */
    AX = rw(pDS, 0x1610);
L22DE:
    /* 22DE  imul    di */
    imul16(DI);
L22E0:
    /* 22E0  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L22E2:
    /* 22E2  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L22E5:
    /* 22E5  imul    di */
    imul16(DI);
L22E7:
    /* 22E7  add     bp,dx */
    BP = add16(BP, DX, 0);
L22E9:
    /* 22E9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22EA:
    /* 22EA  mov     di,ax */
    DI = AX;
L22EC:
    /* 22EC  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L22F0:
    /* 22F0  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L22F4:
    /* 22F4  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L22F8:
    /* 22F8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22F9:
    /* 22F9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L22FA:
    /* 22FA  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_22FE  (+22FE)
       Opcode 8Ch in the no-clip set: INTERP.ASM's do_addres without the clip codes. */
L22FE: /* _seg004_22FE */
    /* 22FE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22FF:
    /* 22FF  mov     bp,ax */
    BP = AX;
L2301:
    /* 2301  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2302:
    /* 2302  mov     di,ax */
    DI = AX;
L2304:
    /* 2304  xchg    bp,si */
    { uint16_t t_ = SI;
    SI = BP;
    BP = t_; }
L2306:
    /* 2306  lea     si,[si+PNT_X] */
    SI = (uint16_t)(SI + 0x1620);
L230A:
    /* 230A  lea     di,[di+PNT_X] */
    DI = (uint16_t)(DI + 0x1620);
L230E:
    /* 230E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L230F:
    /* 230F  add     bx,word ptr [di] */
    BX = (uint16_t)(BX + rw(pDS, DI));
L2311:
    /* 2311  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2312:
    /* 2312  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2313:
    /* 2313  add     bx,word ptr [di] */
    BX = (uint16_t)(BX + rw(pDS, DI));
L2315:
    /* 2315  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2316:
    /* 2316  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2317:
    /* 2317  add     bx,word ptr [di] */
    BX = add16(BX, rw(pDS, DI), 0);
L2319:
    /* 2319  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L231A:
    /* 231A  mov     si,bp */
    SI = BP;
L231C:
    /* 231C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L231D:
    /* 231D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L231E:
    /* 231E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2322  (+2322)
       The level-view variant of the no-clip do_y_rel (as flat_y_rel in INTERP.ASM), installed by check_flat. */
L2322: /* _seg004_2322 */
    /* 2322  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2323:
    /* 2323  mov     bp,ax */
    BP = AX;
L2325:
    /* 2325  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2326:
    /* 2326  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L232A:
    /* 232A  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L232C:
    /* 232C  sar     ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
L232E:
    /* 232E  mov     cx,ax */
    CX = AX;
L2330:
    /* 2330  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2331:
    /* 2331  xchg    bp,si */
    { uint16_t t_ = SI;
    SI = BP;
    BP = t_; }
L2333:
    /* 2333  mov     di,PNT_X */
    DI = 0x1620;
L2336:
    /* 2336  add     si,di */
    SI = (uint16_t)(SI + DI);
L2338:
    /* 2338  add     di,ax */
    DI = (uint16_t)(DI + AX);
L233A:
    /* 233A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L233B:
    /* 233B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L233C:
    /* 233C  add     ax,cx */
    AX = add16(AX, CX, 0);
L233E:
    /* 233E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L233F:
    /* 233F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2340:
    /* 2340  mov     si,bp */
    SI = BP;
L2342:
    /* 2342  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2343:
    /* 2343  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2344:
    /* 2344  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2348  (+2348)
       The level-view variant of the no-clip do_x_rel (as flat_x_rel in INTERP.ASM), installed by check_flat. */
L2348: /* _seg004_2348 */
    /* 2348  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2349:
    /* 2349  mov     di,ax */
    DI = AX;
L234B:
    /* 234B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L234C:
    /* 234C  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L2350:
    /* 2350  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2352:
    /* 2352  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L2356:
    /* 2356  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L235A:
    /* 235A  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L235E:
    /* 235E  mov     di,ax */
    DI = AX;
L2360:
    /* 2360  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L2364:
    /* 2364  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L2366:
    /* 2366  mov     ax,di */
    AX = DI;
L2368:
    /* 2368  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L236C:
    /* 236C  add     bp,dx */
    BP = add16(BP, DX, 0);
L236E:
    /* 236E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L236F:
    /* 236F  mov     di,ax */
    DI = AX;
L2371:
    /* 2371  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L2375:
    /* 2375  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L2379:
    /* 2379  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L237D:
    /* 237D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L237E:
    /* 237E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L237F:
    /* 237F  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2383  (+2383)
       The level-view variant of the no-clip do_z_rel (as flat_z_rel in INTERP.ASM), installed by check_flat. */
L2383: /* _seg004_2383 */
    /* 2383  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2384:
    /* 2384  mov     di,ax */
    DI = AX;
L2386:
    /* 2386  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2387:
    /* 2387  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L238B:
    /* 238B  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L238D:
    /* 238D  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L2391:
    /* 2391  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L2395:
    /* 2395  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L2399:
    /* 2399  mov     di,ax */
    DI = AX;
L239B:
    /* 239B  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L239F:
    /* 239F  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L23A1:
    /* 23A1  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L23A4:
    /* 23A4  imul    di */
    imul16(DI);
L23A6:
    /* 23A6  add     bp,dx */
    BP = add16(BP, DX, 0);
L23A8:
    /* 23A8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L23A9:
    /* 23A9  mov     di,ax */
    DI = AX;
L23AB:
    /* 23AB  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L23AF:
    /* 23AF  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L23B3:
    /* 23B3  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L23B7:
    /* 23B7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L23B8:
    /* 23B8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L23B9:
    /* 23B9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_23BD  (+23BD)
       Opcode 92h in the no-clip set: INTERP.ASM's do_xz_rel without the clip codes. */
L23BD: /* _seg004_23BD */
    /* 23BD  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L23C1:
    /* 23C1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L23C2:
    /* 23C2  shl     ax,cl */
    AX = shl16(AX, CL);
L23C4:
    /* 23C4  mov     di,ax */
    DI = AX;
L23C6:
    /* 23C6  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L23CA:
    /* 23CA  mov     bx,dx */
    BX = DX;
L23CC:
    /* 23CC  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L23CF:
    /* 23CF  imul    di */
    imul16(DI);
L23D1:
    /* 23D1  mov     bp,dx */
    BP = DX;
L23D3:
    /* 23D3  mov     ax,word ptr ds:[MAT_XY] */
    AX = rw(pDS, 0x1604);
L23D6:
    /* 23D6  imul    di */
    imul16(DI);
L23D8:
    /* 23D8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L23D9:
    /* 23D9  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L23DB:
    /* 23DB  mov     cx,dx */
    CX = DX;
L23DD:
    /* 23DD  mov     di,ax */
    DI = AX;
L23DF:
    /* 23DF  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L23E3:
    /* 23E3  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L23E5:
    /* 23E5  mov     ax,word ptr ds:[MAT_ZY] */
    AX = rw(pDS, 0x1610);
L23E8:
    /* 23E8  imul    di */
    imul16(DI);
L23EA:
    /* 23EA  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L23EC:
    /* 23EC  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L23EF:
    /* 23EF  imul    di */
    imul16(DI);
L23F1:
    /* 23F1  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L23F3:
    /* 23F3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L23F4:
    /* 23F4  mov     di,ax */
    DI = AX;
L23F6:
    /* 23F6  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L23FA:
    /* 23FA  add     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x1622));
L23FE:
    /* 23FE  add     bp,word ptr [di+PNT_Z] */
    BP = add16(BP, rw(pDS, DI + 0x1624), 0);
L2402:
    /* 2402  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2403:
    /* 2403  mov     di,ax */
    DI = AX;
L2405:
    /* 2405  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L2409:
    /* 2409  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L240D:
    /* 240D  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L2411:
    /* 2411  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L2415:
    /* 2415  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2416:
    /* 2416  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2417:
    /* 2417  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_241B  (+241B)
       Opcode 90h in the no-clip set: INTERP.ASM's do_xy_rel without the clip codes. */
L241B: /* _seg004_241B */
    /* 241B  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L241F:
    /* 241F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2420:
    /* 2420  shl     ax,cl */
    AX = shl16(AX, CL);
L2422:
    /* 2422  mov     di,ax */
    DI = AX;
L2424:
    /* 2424  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L2428:
    /* 2428  mov     bx,dx */
    BX = DX;
L242A:
    /* 242A  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L242D:
    /* 242D  imul    di */
    imul16(DI);
L242F:
    /* 242F  mov     bp,dx */
    BP = DX;
L2431:
    /* 2431  mov     ax,word ptr ds:[MAT_XY] */
    AX = rw(pDS, 0x1604);
L2434:
    /* 2434  imul    di */
    imul16(DI);
L2436:
    /* 2436  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2437:
    /* 2437  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2439:
    /* 2439  mov     cx,dx */
    CX = DX;
L243B:
    /* 243B  mov     di,ax */
    DI = AX;
L243D:
    /* 243D  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L2441:
    /* 2441  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L2443:
    /* 2443  mov     ax,word ptr ds:[MAT_YY] */
    AX = rw(pDS, 0x160A);
L2446:
    /* 2446  imul    di */
    imul16(DI);
L2448:
    /* 2448  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L244A:
    /* 244A  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L244D:
    /* 244D  imul    di */
    imul16(DI);
L244F:
    /* 244F  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L2451:
    /* 2451  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2452:
    /* 2452  mov     di,ax */
    DI = AX;
L2454:
    /* 2454  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L2458:
    /* 2458  add     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x1622));
L245C:
    /* 245C  add     bp,word ptr [di+PNT_Z] */
    BP = add16(BP, rw(pDS, DI + 0x1624), 0);
L2460:
    /* 2460  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2461:
    /* 2461  mov     di,ax */
    DI = AX;
L2463:
    /* 2463  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L2467:
    /* 2467  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L246B:
    /* 246B  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L246F:
    /* 246F  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L2473:
    /* 2473  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2474:
    /* 2474  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2475:
    /* 2475  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2479  (+2479)
       Opcode 94h in the no-clip set: INTERP.ASM's do_yz_rel without the clip codes. */
L2479: /* _seg004_2479 */
    /* 2479  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L247D:
    /* 247D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L247E:
    /* 247E  shl     ax,cl */
    AX = shl16(AX, CL);
L2480:
    /* 2480  mov     di,ax */
    DI = AX;
L2482:
    /* 2482  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L2486:
    /* 2486  mov     bx,dx */
    BX = DX;
L2488:
    /* 2488  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L248B:
    /* 248B  imul    di */
    imul16(DI);
L248D:
    /* 248D  mov     bp,dx */
    BP = DX;
L248F:
    /* 248F  mov     ax,word ptr ds:[MAT_ZY] */
    AX = rw(pDS, 0x1610);
L2492:
    /* 2492  imul    di */
    imul16(DI);
L2494:
    /* 2494  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2495:
    /* 2495  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2497:
    /* 2497  mov     cx,dx */
    CX = DX;
L2499:
    /* 2499  mov     di,ax */
    DI = AX;
L249B:
    /* 249B  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L249F:
    /* 249F  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L24A1:
    /* 24A1  mov     ax,word ptr ds:[MAT_YY] */
    AX = rw(pDS, 0x160A);
L24A4:
    /* 24A4  imul    di */
    imul16(DI);
L24A6:
    /* 24A6  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L24A8:
    /* 24A8  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L24AB:
    /* 24AB  imul    di */
    imul16(DI);
L24AD:
    /* 24AD  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L24AF:
    /* 24AF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L24B0:
    /* 24B0  mov     di,ax */
    DI = AX;
L24B2:
    /* 24B2  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L24B6:
    /* 24B6  add     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x1622));
L24BA:
    /* 24BA  add     bp,word ptr [di+PNT_Z] */
    BP = add16(BP, rw(pDS, DI + 0x1624), 0);
L24BE:
    /* 24BE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L24BF:
    /* 24BF  mov     di,ax */
    DI = AX;
L24C1:
    /* 24C1  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L24C5:
    /* 24C5  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L24C9:
    /* 24C9  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L24CD:
    /* 24CD  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L24D1:
    /* 24D1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L24D2:
    /* 24D2  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L24D3:
    /* 24D3  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_24D7  (+24D7)
       The level-view variant of the no-clip do_xz_rel (as flat_xz_rel in INTERP.ASM), installed by check_flat. */
L24D7: /* _seg004_24D7 */
    /* 24D7  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L24DB:
    /* 24DB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L24DC:
    /* 24DC  shl     ax,cl */
    AX = shl16(AX, CL);
L24DE:
    /* 24DE  mov     di,ax */
    DI = AX;
L24E0:
    /* 24E0  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L24E4:
    /* 24E4  mov     bx,dx */
    BX = DX;
L24E6:
    /* 24E6  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L24E9:
    /* 24E9  imul    di */
    imul16(DI);
L24EB:
    /* 24EB  mov     bp,dx */
    BP = DX;
L24ED:
    /* 24ED  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L24EE:
    /* 24EE  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L24F0:
    /* 24F0  mov     di,ax */
    DI = AX;
L24F2:
    /* 24F2  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L24F6:
    /* 24F6  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L24F8:
    /* 24F8  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L24FB:
    /* 24FB  imul    di */
    imul16(DI);
L24FD:
    /* 24FD  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L24FF:
    /* 24FF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2500:
    /* 2500  mov     di,ax */
    DI = AX;
L2502:
    /* 2502  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L2506:
    /* 2506  add     bp,word ptr [di+PNT_Z] */
    BP = add16(BP, rw(pDS, DI + 0x1624), 0);
L250A:
    /* 250A  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L250E:
    /* 250E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L250F:
    /* 250F  mov     di,ax */
    DI = AX;
L2511:
    /* 2511  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L2515:
    /* 2515  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L2519:
    /* 2519  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L251D:
    /* 251D  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L2521:
    /* 2521  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2522:
    /* 2522  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2523:
    /* 2523  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2527  (+2527)
       The level-view variant of the no-clip do_xy_rel (as flat_xy_rel in INTERP.ASM), installed by check_flat. */
L2527: /* _seg004_2527 */
    /* 2527  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L252B:
    /* 252B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L252C:
    /* 252C  shl     ax,cl */
    AX = shl16(AX, CL);
L252E:
    /* 252E  mov     di,ax */
    DI = AX;
L2530:
    /* 2530  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L2534:
    /* 2534  mov     bx,dx */
    BX = DX;
L2536:
    /* 2536  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L2539:
    /* 2539  imul    di */
    imul16(DI);
L253B:
    /* 253B  mov     bp,dx */
    BP = DX;
L253D:
    /* 253D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L253E:
    /* 253E  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2540:
    /* 2540  sar     ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
L2542:
    /* 2542  mov     cx,ax */
    CX = AX;
L2544:
    /* 2544  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2545:
    /* 2545  mov     di,ax */
    DI = AX;
L2547:
    /* 2547  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L254B:
    /* 254B  add     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x1622));
L254F:
    /* 254F  add     bp,word ptr [di+PNT_Z] */
    BP = add16(BP, rw(pDS, DI + 0x1624), 0);
L2553:
    /* 2553  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2554:
    /* 2554  mov     di,ax */
    DI = AX;
L2556:
    /* 2556  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L255A:
    /* 255A  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L255E:
    /* 255E  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L2562:
    /* 2562  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L2566:
    /* 2566  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2567:
    /* 2567  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2568:
    /* 2568  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_256C  (+256C)
       The level-view variant of the no-clip do_yz_rel (as flat_yz_rel in INTERP.ASM), installed by check_flat. */
L256C: /* _seg004_256C */
    /* 256C  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L2570:
    /* 2570  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2571:
    /* 2571  shl     ax,cl */
    AX = shl16(AX, CL);
L2573:
    /* 2573  mov     di,ax */
    DI = AX;
L2575:
    /* 2575  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L2579:
    /* 2579  mov     bx,dx */
    BX = DX;
L257B:
    /* 257B  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L257E:
    /* 257E  imul    di */
    imul16(DI);
L2580:
    /* 2580  mov     bp,dx */
    BP = DX;
L2582:
    /* 2582  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2583:
    /* 2583  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2585:
    /* 2585  sar     ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
L2587:
    /* 2587  mov     cx,ax */
    CX = AX;
L2589:
    /* 2589  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L258A:
    /* 258A  mov     di,ax */
    DI = AX;
L258C:
    /* 258C  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L2590:
    /* 2590  add     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x1622));
L2594:
    /* 2594  add     bp,word ptr [di+PNT_Z] */
    BP = add16(BP, rw(pDS, DI + 0x1624), 0);
L2598:
    /* 2598  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2599:
    /* 2599  mov     di,ax */
    DI = AX;
L259B:
    /* 259B  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L259F:
    /* 259F  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L25A3:
    /* 25A3  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L25A7:
    /* 25A7  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L25AB:
    /* 25AB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25AC:
    /* 25AC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L25AD:
    /* 25AD  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_25B1  (+25B1)
       Opcode 96h in the no-clip set: INTERP.ASM's do_lnres without the clip codes. */
L25B1: /* _seg004_25B1 */
    /* 25B1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25B2:
    /* 25B2  mov     di,ax */
    DI = AX;
L25B4:
    /* 25B4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25B5:
    /* 25B5  mov     bx,ax */
    BX = AX;
L25B7:
    /* 25B7  push    si */
    push16(SI);
L25B8:
    /* 25B8  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L25BC:
    /* 25BC  mov     ax,word ptr [di+PNT_X] */
    AX = rw(pDS, DI + 0x1620);
L25C0:
    /* 25C0  imul    word ptr ds:[PROJ_X] */
    imul16(rw(pDS, 0x26B2));
L25C4:
    /* 25C4  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x25C4, 2)) != 0) return c;
L25C6:
    /* 25C6  add     ax,word ptr ds:[CENTRE_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B4));
L25CA:
    /* 25CA  mov     cx,ax */
    CX = AX;
L25CC:
    /* 25CC  mov     ax,word ptr [di+PNT_Y] */
    AX = rw(pDS, DI + 0x1622);
L25D0:
    /* 25D0  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L25D4:
    /* 25D4  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x25D4, 2)) != 0) return c;
L25D6:
    /* 25D6  add     ax,word ptr ds:[CENTRE_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B6));
L25DA:
    /* 25DA  mov     si,ax */
    SI = AX;
L25DC:
    /* 25DC  mov     di,bx */
    DI = BX;
L25DE:
    /* 25DE  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L25E2:
    /* 25E2  mov     ax,word ptr [di+PNT_Y] */
    AX = rw(pDS, DI + 0x1622);
L25E6:
    /* 25E6  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L25EA:
    /* 25EA  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x25EA, 2)) != 0) return c;
L25EC:
    /* 25EC  add     ax,word ptr ds:[CENTRE_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B6));
L25F0:
    /* 25F0  mov     bx,ax */
    BX = AX;
L25F2:
    /* 25F2  mov     ax,word ptr [di+PNT_X] */
    AX = rw(pDS, DI + 0x1620);
L25F6:
    /* 25F6  imul    word ptr ds:[PROJ_X] */
    imul16(rw(pDS, 0x26B2));
L25FA:
    /* 25FA  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x25FA, 2)) != 0) return c;
L25FC:
    /* 25FC  add     ax,word ptr ds:[CENTRE_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B4));
L2600:
    /* 2600  mov     dx,si */
    DX = SI;
L2602:
    /* 2602  call    far ptr _seg003_8 */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x0008), 0x06E7 + PORT_LOAD_SEG, 0x2607)) != 0) return c;
L2607:
    /* 2607  pop     si */
    SI = pop16();
L2608:
    /* 2608  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2609:
    /* 2609  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L260A:
    /* 260A  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_260E  (+260E)
       Opcode 9Ah in the no-clip set: INTERP.ASM's do_usepat without the clip codes. */
L260E: /* _seg004_260E */
    /* 260E  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L2612:
    /* 2612  mov     bx,word ptr ds:[OBJ_X] */
    BX = rw(pDS, 0x2886);
L2616:
    /* 2616  neg     bx */
    BX = neg16(BX);
L2618:
    /* 2618  shl     bx,cl */
    BX = (uint16_t)(BX << (CL & 31));
L261A:
    /* 261A  mov     bp,word ptr ds:[OBJ_Z] */
    BP = rw(pDS, 0x288A);
L261E:
    /* 261E  neg     bp */
    BP = neg16(BP);
L2620:
    /* 2620  shl     bp,cl */
    BP = (uint16_t)(BP << (CL & 31));
L2622:
    /* 2622  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L2625:
    /* 2625  neg     ax */
    AX = neg16(AX);
L2627:
    /* 2627  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2629:
    /* 2629  mov     cx,ax */
    CX = AX;
L262B:
    /* 262B  call    _mxmul */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x50D8), 0x262E)) != 0) return c;
L262E:
    /* 262E  mov     word ptr ds:[15FCh],bx */
    ww(pDS, 0x15FC, BX);
L2632:
    /* 2632  mov     word ptr ds:[15FEh],cx */
    ww(pDS, 0x15FE, CX);
L2636:
    /* 2636  mov     word ptr ds:[1600h],bp */
    ww(pDS, 0x1600, BP);
L263A:
    /* 263A  mov     di,PNT_X */
    DI = 0x1620;
L263D:
    /* 263D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L263E:
    /* 263E  add     di,ax */
    DI = (uint16_t)(DI + AX);
L2640:
    /* 2640  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2641:
    /* 2641  push    si */
    push16(SI);
L2642:
    /* 2642  add     si,ax */
    SI = (uint16_t)(SI + AX);
L2644:
    /* 2644  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2645:
    /* 2645  mov     dx,ax */
    DX = AX;
L2647:
    /* 2647  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2648:
    /* 2648  sub     al,byte ptr ds:[OBJ_SHIFT] */
    AL = (uint8_t)(AL - rb(pDS, 0x2880));
L264C:
    /* 264C  mov     dh,al */
    DH = AL;
L264E:
    /* 264E  shr     dl,1 */
    DL = shr8(DL, 1);
L2650:
    /* 2650  jae     short L267D */
    if (!CF) goto L267D;
L2652:
    /* 2652  mov     cl,dh */
    CL = DH;
L2654:
    /* 2654  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2655:
    /* 2655  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2657:
    /* 2657  mov     bx,ax */
    BX = AX;
L2659:
    /* 2659  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L265D:
    /* 265D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L265E:
    /* 265E  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2660:
    /* 2660  mov     bp,ax */
    BP = AX;
L2662:
    /* 2662  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L2666:
    /* 2666  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2667:
    /* 2667  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2669:
    /* 2669  mov     cx,ax */
    CX = AX;
L266B:
    /* 266B  add     cx,word ptr ds:[15FEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15FE));
L266F:
    /* 266F  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2671:
    /* 2671  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L2674:
    /* 2674  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L2677:
    /* 2677  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L267A:
    /* 267A  add     di,8 */
    DI = add16(DI, 0x8, 0);
L267D: /* L267D */
    /* 267D  mov     cl,dh */
    CL = DH;
L267F:
    /* 267F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2680:
    /* 2680  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2682:
    /* 2682  mov     bx,ax */
    BX = AX;
L2684:
    /* 2684  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L2688:
    /* 2688  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2689:
    /* 2689  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L268B:
    /* 268B  mov     bp,ax */
    BP = AX;
L268D:
    /* 268D  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L2691:
    /* 2691  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2692:
    /* 2692  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2694:
    /* 2694  mov     cx,ax */
    CX = AX;
L2696:
    /* 2696  add     cx,word ptr ds:[15FEh] */
    CX = add16(CX, rw(pDS, 0x15FE), 0);
L269A:
    /* 269A  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L269C:
    /* 269C  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L269F:
    /* 269F  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L26A2:
    /* 26A2  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L26A5:
    /* 26A5  mov     cl,dh */
    CL = DH;
L26A7:
    /* 26A7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L26A8:
    /* 26A8  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L26AA:
    /* 26AA  mov     bx,ax */
    BX = AX;
L26AC:
    /* 26AC  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L26B0:
    /* 26B0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L26B1:
    /* 26B1  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L26B3:
    /* 26B3  mov     bp,ax */
    BP = AX;
L26B5:
    /* 26B5  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L26B9:
    /* 26B9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L26BA:
    /* 26BA  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L26BC:
    /* 26BC  mov     cx,ax */
    CX = AX;
L26BE:
    /* 26BE  add     cx,word ptr ds:[15FEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15FE));
L26C2:
    /* 26C2  mov     word ptr [di+8],bx */
    ww(pDS, DI + 0x8, BX);
L26C5:
    /* 26C5  mov     word ptr [di+0Ah],cx */
    ww(pDS, DI + 0xA, CX);
L26C8:
    /* 26C8  mov     word ptr [di+0Ch],bp */
    ww(pDS, DI + 0xC, BP);
L26CB:
    /* 26CB  mov     byte ptr [di+0Eh],al */
    wb(pDS, DI + 0xE, AL);
L26CE:
    /* 26CE  add     di,10h */
    DI = add16(DI, 0x10, 0);
L26D1:
    /* 26D1  dec     dl */
    DL = dec8(DL);
L26D3:
    /* 26D3  jne     L267D */
    if (!ZF) goto L267D;
L26D5:
    /* 26D5  pop     si */
    SI = pop16();
L26D6:
    /* 26D6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L26D7:
    /* 26D7  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L26D8:
    /* 26D8  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_26DC  (+26DC)
       Opcode 98h in the no-clip set: INTERP.ASM's do_putpat without the clip codes. */
L26DC: /* _seg004_26DC */
    /* 26DC  push    word ptr ds:[OBJ_X] */
    push16(rw(pDS, 0x2886));
L26E0:
    /* 26E0  push    word ptr ds:[OBJ_Y] */
    push16(rw(pDS, 0x2888));
L26E4:
    /* 26E4  push    word ptr ds:[OBJ_Z] */
    push16(rw(pDS, 0x288A));
L26E8:
    /* 26E8  call    _load_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x50BE), 0x26EB)) != 0) return c;
L26EB:
    /* 26EB  mov     word ptr ds:[15FCh],bx */
    ww(pDS, 0x15FC, BX);
L26EF:
    /* 26EF  mov     word ptr ds:[15FEh],cx */
    ww(pDS, 0x15FE, CX);
L26F3:
    /* 26F3  mov     word ptr ds:[1600h],bp */
    ww(pDS, 0x1600, BP);
L26F7:
    /* 26F7  mov     ax,word ptr ds:[OBJ_X] */
    AX = rw(pDS, 0x2886);
L26FA:
    /* 26FA  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L26FD:
    /* 26FD  mov     word ptr ds:[OBJ_X],ax */
    ww(pDS, 0x2886, AX);
L2700:
    /* 2700  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L2703:
    /* 2703  sub     ax,word ptr [si-2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFE));
L2706:
    /* 2706  mov     word ptr ds:[OBJ_Y],ax */
    ww(pDS, 0x2888, AX);
L2709:
    /* 2709  mov     ax,word ptr ds:[OBJ_Z] */
    AX = rw(pDS, 0x288A);
L270C:
    /* 270C  sub     ax,word ptr [si-4] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFC));
L270F:
    /* 270F  mov     word ptr ds:[OBJ_Z],ax */
    ww(pDS, 0x288A, AX);
L2712:
    /* 2712  mov     bp,PNT_X */
    BP = 0x1620;
L2715:
    /* 2715  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2716:
    /* 2716  add     di,ax */
    DI = (uint16_t)(DI + AX);
L2718:
    /* 2718  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2719:
    /* 2719  push    si */
    push16(SI);
L271A:
    /* 271A  add     si,ax */
    SI = (uint16_t)(SI + AX);
L271C:
    /* 271C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L271D:
    /* 271D  mov     dx,ax */
    DX = AX;
L271F:
    /* 271F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2720:
    /* 2720  sub     al,byte ptr ds:[OBJ_SHIFT] */
    AL = (uint8_t)(AL - rb(pDS, 0x2880));
L2724:
    /* 2724  mov     dh,al */
    DH = AL;
L2726:
    /* 2726  shr     dl,1 */
    DL = shr8(DL, 1);
L2728:
    /* 2728  jae     short L2755 */
    if (!CF) goto L2755;
L272A:
    /* 272A  mov     cl,dh */
    CL = DH;
L272C:
    /* 272C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L272D:
    /* 272D  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L272F:
    /* 272F  mov     bx,ax */
    BX = AX;
L2731:
    /* 2731  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L2735:
    /* 2735  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2736:
    /* 2736  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2738:
    /* 2738  mov     bp,ax */
    BP = AX;
L273A:
    /* 273A  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L273E:
    /* 273E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L273F:
    /* 273F  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2741:
    /* 2741  mov     cx,ax */
    CX = AX;
L2743:
    /* 2743  add     cx,word ptr ds:[15FEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15FE));
L2747:
    /* 2747  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2749:
    /* 2749  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L274C:
    /* 274C  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L274F:
    /* 274F  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L2752:
    /* 2752  add     di,8 */
    DI = add16(DI, 0x8, 0);
L2755: /* L2755 */
    /* 2755  mov     cl,dh */
    CL = DH;
L2757:
    /* 2757  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2758:
    /* 2758  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L275A:
    /* 275A  mov     bx,ax */
    BX = AX;
L275C:
    /* 275C  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L2760:
    /* 2760  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2761:
    /* 2761  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2763:
    /* 2763  mov     bp,ax */
    BP = AX;
L2765:
    /* 2765  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L2769:
    /* 2769  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L276A:
    /* 276A  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L276C:
    /* 276C  mov     cx,ax */
    CX = AX;
L276E:
    /* 276E  add     cx,word ptr ds:[15FEh] */
    CX = add16(CX, rw(pDS, 0x15FE), 0);
L2772:
    /* 2772  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2774:
    /* 2774  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L2777:
    /* 2777  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L277A:
    /* 277A  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L277D:
    /* 277D  mov     cl,dh */
    CL = DH;
L277F:
    /* 277F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2780:
    /* 2780  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2782:
    /* 2782  mov     bx,ax */
    BX = AX;
L2784:
    /* 2784  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L2788:
    /* 2788  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2789:
    /* 2789  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L278B:
    /* 278B  mov     bp,ax */
    BP = AX;
L278D:
    /* 278D  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L2791:
    /* 2791  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2792:
    /* 2792  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2794:
    /* 2794  mov     cx,ax */
    CX = AX;
L2796:
    /* 2796  add     cx,word ptr ds:[15FEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15FE));
L279A:
    /* 279A  mov     word ptr [di+8],bx */
    ww(pDS, DI + 0x8, BX);
L279D:
    /* 279D  mov     word ptr [di+0Ah],cx */
    ww(pDS, DI + 0xA, CX);
L27A0:
    /* 27A0  mov     word ptr [di+0Ch],bp */
    ww(pDS, DI + 0xC, BP);
L27A3:
    /* 27A3  mov     byte ptr [di+0Eh],al */
    wb(pDS, DI + 0xE, AL);
L27A6:
    /* 27A6  add     di,10h */
    DI = add16(DI, 0x10, 0);
L27A9:
    /* 27A9  dec     dl */
    DL = dec8(DL);
L27AB:
    /* 27AB  jne     L2755 */
    if (!ZF) goto L2755;
L27AD:
    /* 27AD  pop     si */
    SI = pop16();
L27AE:
    /* 27AE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L27AF:
    /* 27AF  push    si */
    push16(SI);
L27B0:
    /* 27B0  add     si,ax */
    SI = (uint16_t)(SI + AX);
L27B2:
    /* 27B2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L27B3:
    /* 27B3  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L27B4:
    /* 27B4  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x27B8)) != 0) return c;
L27B8:
    /* 27B8  pop     si */
    SI = pop16();
L27B9:
    /* 27B9  pop     word ptr ds:[OBJ_Z] */
    { uint16_t t_ = pop16(); ww(pDS, 0x288A, t_); }
L27BD:
    /* 27BD  pop     word ptr ds:[OBJ_Y] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2888, t_); }
L27C1:
    /* 27C1  pop     word ptr ds:[OBJ_X] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2886, t_); }
L27C5:
    /* 27C5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L27C6:
    /* 27C6  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L27C7:
    /* 27C7  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
}
