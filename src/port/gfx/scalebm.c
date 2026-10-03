/* scalebm.c: replaces src/gfx/SCALEBM.ASM (seg003_0272_D1E, 0D1E..21D4 of its
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

uint32_t asm_mod_SCALEBM(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0D1E: goto L0D1E;
    case 0x0D1F: goto L0D1F;
    case 0x0D20: goto L0D20;
    case 0x0D23: goto L0D23;
    case 0x0D25: goto L0D25;
    case 0x0D27: goto L0D27;
    case 0x0D2C: goto L0D2C;
    case 0x0D31: goto L0D31;
    case 0x0D32: goto L0D32;
    case 0x0D34: goto L0D34;
    case 0x0D38: goto L0D38;
    case 0x0D39: goto L0D39;
    case 0x0D3D: goto L0D3D;
    case 0x0D40: goto L0D40;
    case 0x0D42: goto L0D42;
    case 0x0D45: goto L0D45;
    case 0x0D4A: goto L0D4A;
    case 0x0D4C: goto L0D4C;
    case 0x0D4F: goto L0D4F;
    case 0x0D52: goto L0D52;
    case 0x0D53: goto L0D53;
    case 0x0D55: goto L0D55;
    case 0x0D59: goto L0D59;
    case 0x0D5B: goto L0D5B;
    case 0x0D5D: goto L0D5D;
    case 0x0D5F: goto L0D5F;
    case 0x0D60: goto L0D60;
    case 0x0D62: goto L0D62;
    case 0x0D63: goto L0D63;
    case 0x0D65: goto L0D65;
    case 0x0D67: goto L0D67;
    case 0x0D6A: goto L0D6A;
    case 0x0D6D: goto L0D6D;
    case 0x0D6F: goto L0D6F;
    case 0x0D71: goto L0D71;
    case 0x0D74: goto L0D74;
    case 0x0D76: goto L0D76;
    case 0x0D77: goto L0D77;
    case 0x0D7B: goto L0D7B;
    case 0x0D7C: goto L0D7C;
    case 0x0D7E: goto L0D7E;
    case 0x0D80: goto L0D80;
    case 0x0D82: goto L0D82;
    case 0x0D85: goto L0D85;
    case 0x0D89: goto L0D89;
    case 0x0D8C: goto L0D8C;
    case 0x0D90: goto L0D90;
    case 0x0D92: goto L0D92;
    case 0x0D94: goto L0D94;
    case 0x0D96: goto L0D96;
    case 0x0D98: goto L0D98;
    case 0x0D9B: goto L0D9B;
    case 0x0D9E: goto L0D9E;
    case 0x0DA2: goto L0DA2;
    case 0x0DA6: goto L0DA6;
    case 0x0DA8: goto L0DA8;
    case 0x0DAB: goto L0DAB;
    case 0x0DB1: goto L0DB1;
    case 0x0DB3: goto L0DB3;
    case 0x0DB6: goto L0DB6;
    case 0x0DB9: goto L0DB9;
    case 0x0DBD: goto L0DBD;
    case 0x0DBF: goto L0DBF;
    case 0x0DC1: goto L0DC1;
    case 0x0DC5: goto L0DC5;
    case 0x0DC9: goto L0DC9;
    case 0x0DCD: goto L0DCD;
    case 0x0DD0: goto L0DD0;
    case 0x0DD2: goto L0DD2;
    case 0x0DD6: goto L0DD6;
    case 0x0DDA: goto L0DDA;
    case 0x0DDE: goto L0DDE;
    case 0x0DE0: goto L0DE0;
    case 0x0DE4: goto L0DE4;
    case 0x0DE6: goto L0DE6;
    case 0x0DE8: goto L0DE8;
    case 0x0DEA: goto L0DEA;
    case 0x0DEE: goto L0DEE;
    case 0x0DF0: goto L0DF0;
    case 0x0DF4: goto L0DF4;
    case 0x0DF6: goto L0DF6;
    case 0x0DF8: goto L0DF8;
    case 0x0DF9: goto L0DF9;
    case 0x0DFD: goto L0DFD;
    case 0x0DFF: goto L0DFF;
    case 0x0E03: goto L0E03;
    case 0x0E05: goto L0E05;
    case 0x0E07: goto L0E07;
    case 0x0E09: goto L0E09;
    case 0x0E0D: goto L0E0D;
    case 0x0E10: goto L0E10;
    case 0x0E14: goto L0E14;
    case 0x0E16: goto L0E16;
    case 0x0E18: goto L0E18;
    case 0x0E1A: goto L0E1A;
    case 0x0E1C: goto L0E1C;
    case 0x0E20: goto L0E20;
    case 0x0E24: goto L0E24;
    case 0x0E28: goto L0E28;
    case 0x0E2C: goto L0E2C;
    case 0x0E30: goto L0E30;
    case 0x0E32: goto L0E32;
    case 0x0E35: goto L0E35;
    case 0x0E38: goto L0E38;
    case 0x0E3B: goto L0E3B;
    case 0x0E40: goto L0E40;
    case 0x0E44: goto L0E44;
    case 0x0E47: goto L0E47;
    case 0x0E48: goto L0E48;
    case 0x0E49: goto L0E49;
    case 0x0E4D: goto L0E4D;
    case 0x0E50: goto L0E50;
    case 0x0E54: goto L0E54;
    case 0x0E58: goto L0E58;
    case 0x0E5A: goto L0E5A;
    case 0x0E5E: goto L0E5E;
    case 0x0E62: goto L0E62;
    case 0x0E64: goto L0E64;
    case 0x0E65: goto L0E65;
    case 0x0E6A: goto L0E6A;
    case 0x0E6F: goto L0E6F;
    case 0x0E70: goto L0E70;
    case 0x0E71: goto L0E71;
    case 0x0E72: goto L0E72;
    case 0x1373: goto L1373;
    case 0x1376: goto L1376;
    case 0x1377: goto L1377;
    case 0x1379: goto L1379;
    case 0x137B: goto L137B;
    case 0x137E: goto L137E;
    case 0x1380: goto L1380;
    case 0x1381: goto L1381;
    case 0x1383: goto L1383;
    case 0x1384: goto L1384;
    case 0x1385: goto L1385;
    case 0x1387: goto L1387;
    case 0x1388: goto L1388;
    case 0x138B: goto L138B;
    case 0x138C: goto L138C;
    case 0x138E: goto L138E;
    case 0x1390: goto L1390;
    case 0x1393: goto L1393;
    case 0x1395: goto L1395;
    case 0x1396: goto L1396;
    case 0x1398: goto L1398;
    case 0x1399: goto L1399;
    case 0x139A: goto L139A;
    case 0x139C: goto L139C;
    case 0x139D: goto L139D;
    case 0x139F: goto L139F;
    case 0x13A2: goto L13A2;
    case 0x13A4: goto L13A4;
    case 0x13A7: goto L13A7;
    case 0x13A9: goto L13A9;
    case 0x13AB: goto L13AB;
    case 0x13AE: goto L13AE;
    case 0x13AF: goto L13AF;
    case 0x13B0: goto L13B0;
    case 0x13B2: goto L13B2;
    case 0x13B4: goto L13B4;
    case 0x13B6: goto L13B6;
    case 0x13B8: goto L13B8;
    case 0x13BA: goto L13BA;
    case 0x13BB: goto L13BB;
    case 0x13BC: goto L13BC;
    case 0x13BE: goto L13BE;
    case 0x13C0: goto L13C0;
    case 0x13C2: goto L13C2;
    case 0x13C3: goto L13C3;
    case 0x13C4: goto L13C4;
    case 0x13C6: goto L13C6;
    case 0x13C8: goto L13C8;
    case 0x13CA: goto L13CA;
    case 0x13CC: goto L13CC;
    case 0x13CE: goto L13CE;
    case 0x13D0: goto L13D0;
    case 0x13D2: goto L13D2;
    case 0x13D4: goto L13D4;
    case 0x13D5: goto L13D5;
    case 0x13D6: goto L13D6;
    case 0x13D8: goto L13D8;
    case 0x13D9: goto L13D9;
    case 0x13DA: goto L13DA;
    case 0x13DB: goto L13DB;
    case 0x13DD: goto L13DD;
    case 0x13DF: goto L13DF;
    case 0x13E1: goto L13E1;
    case 0x13E2: goto L13E2;
    case 0x13E3: goto L13E3;
    case 0x13E5: goto L13E5;
    case 0x13E7: goto L13E7;
    case 0x13E9: goto L13E9;
    case 0x13EA: goto L13EA;
    case 0x13EB: goto L13EB;
    case 0x13ED: goto L13ED;
    case 0x13EF: goto L13EF;
    case 0x13F1: goto L13F1;
    case 0x13F3: goto L13F3;
    case 0x13F5: goto L13F5;
    case 0x13F7: goto L13F7;
    case 0x13F8: goto L13F8;
    case 0x13F9: goto L13F9;
    case 0x13FD: goto L13FD;
    case 0x13FE: goto L13FE;
    case 0x13FF: goto L13FF;
    case 0x1401: goto L1401;
    case 0x1403: goto L1403;
    case 0x1405: goto L1405;
    case 0x1407: goto L1407;
    case 0x1408: goto L1408;
    case 0x1409: goto L1409;
    case 0x140B: goto L140B;
    case 0x140D: goto L140D;
    case 0x140F: goto L140F;
    case 0x1410: goto L1410;
    case 0x1411: goto L1411;
    case 0x1413: goto L1413;
    case 0x1415: goto L1415;
    case 0x1417: goto L1417;
    case 0x1419: goto L1419;
    case 0x141B: goto L141B;
    case 0x141D: goto L141D;
    case 0x141F: goto L141F;
    case 0x1420: goto L1420;
    case 0x1421: goto L1421;
    case 0x1423: goto L1423;
    case 0x1425: goto L1425;
    case 0x1428: goto L1428;
    case 0x142A: goto L142A;
    case 0x142D: goto L142D;
    case 0x1432: goto L1432;
    case 0x1434: goto L1434;
    case 0x1436: goto L1436;
    case 0x1438: goto L1438;
    case 0x143B: goto L143B;
    case 0x143D: goto L143D;
    case 0x143F: goto L143F;
    case 0x1442: goto L1442;
    case 0x1444: goto L1444;
    case 0x1446: goto L1446;
    case 0x1448: goto L1448;
    case 0x144B: goto L144B;
    case 0x144D: goto L144D;
    case 0x144F: goto L144F;
    case 0x1450: goto L1450;
    case 0x1452: goto L1452;
    case 0x1455: goto L1455;
    case 0x1456: goto L1456;
    case 0x1458: goto L1458;
    case 0x1459: goto L1459;
    case 0x145B: goto L145B;
    case 0x145C: goto L145C;
    case 0x145E: goto L145E;
    case 0x145F: goto L145F;
    case 0x1461: goto L1461;
    case 0x1462: goto L1462;
    case 0x1465: goto L1465;
    case 0x1466: goto L1466;
    case 0x1467: goto L1467;
    case 0x146B: goto L146B;
    case 0x146D: goto L146D;
    case 0x146F: goto L146F;
    case 0x1472: goto L1472;
    case 0x1474: goto L1474;
    case 0x1476: goto L1476;
    case 0x1478: goto L1478;
    case 0x147A: goto L147A;
    case 0x147C: goto L147C;
    case 0x1480: goto L1480;
    case 0x1482: goto L1482;
    case 0x1485: goto L1485;
    case 0x1486: goto L1486;
    case 0x1489: goto L1489;
    case 0x148A: goto L148A;
    case 0x148C: goto L148C;
    case 0x148E: goto L148E;
    case 0x148F: goto L148F;
    case 0x1492: goto L1492;
    case 0x1493: goto L1493;
    case 0x1496: goto L1496;
    case 0x1497: goto L1497;
    case 0x1499: goto L1499;
    case 0x149A: goto L149A;
    case 0x149C: goto L149C;
    case 0x149D: goto L149D;
    case 0x149F: goto L149F;
    case 0x14A0: goto L14A0;
    case 0x14A4: goto L14A4;
    case 0x14A6: goto L14A6;
    case 0x14A8: goto L14A8;
    case 0x14A9: goto L14A9;
    case 0x14AA: goto L14AA;
    case 0x14AE: goto L14AE;
    case 0x14B1: goto L14B1;
    case 0x14B4: goto L14B4;
    case 0x14B7: goto L14B7;
    case 0x14BD: goto L14BD;
    case 0x14BF: goto L14BF;
    case 0x14C3: goto L14C3;
    case 0x14C5: goto L14C5;
    case 0x14C7: goto L14C7;
    case 0x14C9: goto L14C9;
    case 0x14CC: goto L14CC;
    case 0x14CF: goto L14CF;
    case 0x14D1: goto L14D1;
    case 0x14D4: goto L14D4;
    case 0x14D6: goto L14D6;
    case 0x14D9: goto L14D9;
    case 0x14DB: goto L14DB;
    case 0x14DD: goto L14DD;
    case 0x14DF: goto L14DF;
    case 0x14E0: goto L14E0;
    case 0x14E2: goto L14E2;
    case 0x14E4: goto L14E4;
    case 0x14E6: goto L14E6;
    case 0x14E7: goto L14E7;
    case 0x14E9: goto L14E9;
    case 0x14EC: goto L14EC;
    case 0x14EE: goto L14EE;
    case 0x14EF: goto L14EF;
    case 0x14F1: goto L14F1;
    case 0x14F3: goto L14F3;
    case 0x14F5: goto L14F5;
    case 0x14F7: goto L14F7;
    case 0x14F9: goto L14F9;
    case 0x14FB: goto L14FB;
    case 0x14FC: goto L14FC;
    case 0x14FE: goto L14FE;
    case 0x1500: goto L1500;
    case 0x1502: goto L1502;
    case 0x1504: goto L1504;
    case 0x1506: goto L1506;
    case 0x1508: goto L1508;
    case 0x150A: goto L150A;
    case 0x150C: goto L150C;
    case 0x150E: goto L150E;
    case 0x150F: goto L150F;
    case 0x1511: goto L1511;
    case 0x1513: goto L1513;
    case 0x1515: goto L1515;
    case 0x1516: goto L1516;
    case 0x1518: goto L1518;
    case 0x151A: goto L151A;
    case 0x151C: goto L151C;
    case 0x151D: goto L151D;
    case 0x151F: goto L151F;
    case 0x1521: goto L1521;
    case 0x1523: goto L1523;
    case 0x1525: goto L1525;
    case 0x1527: goto L1527;
    case 0x152A: goto L152A;
    case 0x152C: goto L152C;
    case 0x152E: goto L152E;
    case 0x152F: goto L152F;
    case 0x1531: goto L1531;
    case 0x1533: goto L1533;
    case 0x1535: goto L1535;
    case 0x1538: goto L1538;
    case 0x153A: goto L153A;
    case 0x153C: goto L153C;
    case 0x153E: goto L153E;
    case 0x153F: goto L153F;
    case 0x1541: goto L1541;
    case 0x1544: goto L1544;
    case 0x1545: goto L1545;
    case 0x1547: goto L1547;
    case 0x1548: goto L1548;
    case 0x154A: goto L154A;
    case 0x154B: goto L154B;
    case 0x154D: goto L154D;
    default: asm_bad_entry("SCALEBM.ASM", entry);
    }

    /* seg003_0272_D1E  (+D1E)
       scale_nibble_bitmap (FM Towns; far): draw the sprite described at 0B0A .. 0B3F. Gives up for a
       sprite 2 pixels or less in either direction, one wider than 200h, or one wholly outside the
       window. Works out the steps, clips left, right, top and bottom, generates the row scaler into
       L1554 (through 0B0E), then for each screen row from the top down: when the source row changes,
       unpack and scale it into 0B40 by calling L1554; write the scaled row into the frame buffer at
       row 095C[y] + x through the span writer (0B10). Runs on the library's stack. */
L0D1E: /* _seg003_0272_D1E */
    /* 0D1E  push    es */
    push16(asm_es);
L0D1F:
    /* 0D1F  push    ds */
    push16(asm_ds);
L0D20:
    /* 0D20  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L0D23:
    /* 0D23  mov     ds,ax */
    SET_DS(AX);
L0D25:
    /* 0D25  mov     bx,ss */
    BX = asm_ss;
L0D27:
    /* 0D27  mov     word ptr cs:L1550,bx */
    ww(CODE003, 0x1550, BX);
L0D2C:
    /* 0D2C  mov     word ptr cs:L1552,sp */
    ww(CODE003, 0x1552, SP);
L0D31:
    /* 0D31  cli */
    ;
L0D32:
    /* 0D32  mov     ss,ax */
    SET_SS(AX);
L0D34:
    /* 0D34  mov     sp,word ptr ds:[5588h] */
    SP = rw(pDS, 0x5588);
L0D38:
    /* 0D38  sti */
    ;
L0D39:
    /* 0D39  mov     cx,word ptr ds:[0B26h] */
    CX = rw(pDS, 0xB26);
L0D3D:
    /* 0D3D  cmp     cx,2 */
    sub16(CX, 0x2, 0);
L0D40:
    /* 0D40  jg      L0D45 */
    if (!ZF && SF == OF) goto L0D45;
L0D42:
    /* 0D42  jmp     L0E64 */
    goto L0E64;
L0D45: /* L0D45 */
    /* 0D45  cmp     word ptr ds:[0B24h],2 */
    sub16(rw(pDS, 0xB24), 0x2, 0);
L0D4A:
    /* 0D4A  jg      L0D4F */
    if (!ZF && SF == OF) goto L0D4F;
L0D4C:
    /* 0D4C  jmp     L0E64 */
    goto L0E64;
L0D4F: /* L0D4F */
    /* 0D4F  mov     ax,word ptr ds:[0B1Ch] */
    AX = rw(pDS, 0xB1C);
L0D52:
    /* 0D52  dec     ax */
    AX = (uint16_t)(AX - 1);
L0D53:
    /* 0D53  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0D55:
    /* 0D55  mov     word ptr ds:[0B3Eh],dx */
    ww(pDS, 0xB3E, DX);
L0D59:
    /* 0D59  mov     dl,ah */
    DL = AH;
L0D5B:
    /* 0D5B  mov     ah,al */
    AH = AL;
L0D5D:
    /* 0D5D  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L0D5F:
    /* 0D5F  dec     cx */
    CX = (uint16_t)(CX - 1);
L0D60:
    /* 0D60  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0085, 0x0D60, 2)) != 0) return c;
L0D62:
    /* 0D62  inc     cx */
    CX = (uint16_t)(CX + 1);
L0D63:
    /* 0D63  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0D65:
    /* 0D65  jne     L0D6A */
    if (!ZF) goto L0D6A;
L0D67:
    /* 0D67  jmp     L0E64 */
    goto L0E64;
L0D6A: /* L0D6A */
    /* 0D6A  mov     word ptr ds:[0B38h],ax */
    ww(pDS, 0xB38, AX);
L0D6D:
    /* 0D6D  mov     ax,cs */
    AX = (uint16_t)(0x0085 + PORT_LOAD_SEG);
L0D6F:
    /* 0D6F  mov     es,ax */
    SET_ES(AX);
L0D71:
    /* 0D71  mov     ax,word ptr ds:[0B20h] */
    AX = rw(pDS, 0xB20);
L0D74:
    /* 0D74  add     ax,cx */
    AX = (uint16_t)(AX + CX);
L0D76:
    /* 0D76  dec     ax */
    AX = (uint16_t)(AX - 1);
L0D77:
    /* 0D77  sub     ax,word ptr ds:[3DF8h] */
    AX = (uint16_t)(AX - rw(pDS, 0x3DF8));
L0D7B:
    /* 0D7B  dec     ax */
    AX = dec16(AX);
L0D7C:
    /* 0D7C  jle     L0D89 */
    if (ZF || SF != OF) goto L0D89;
L0D7E:
    /* 0D7E  sub     cx,ax */
    CX = sub16(CX, AX, 0);
L0D80:
    /* 0D80  jg      L0D85 */
    if (!ZF && SF == OF) goto L0D85;
L0D82:
    /* 0D82  jmp     L0E64 */
    goto L0E64;
L0D85: /* L0D85 */
    /* 0D85  mov     word ptr ds:[0B26h],cx */
    ww(pDS, 0xB26, CX);
L0D89: /* L0D89 */
    /* 0D89  mov     ax,word ptr ds:[0B20h] */
    AX = rw(pDS, 0xB20);
L0D8C:
    /* 0D8C  mov     si,word ptr ds:[3DF4h] */
    SI = rw(pDS, 0x3DF4);
L0D90:
    /* 0D90  sub     ax,si */
    AX = sub16(AX, SI, 0);
L0D92:
    /* 0D92  jge     L0DAB */
    if (SF == OF) goto L0DAB;
L0D94:
    /* 0D94  neg     ax */
    AX = neg16(AX);
L0D96:
    /* 0D96  jg      L0D9B */
    if (!ZF && SF == OF) goto L0D9B;
L0D98:
    /* 0D98  jmp     L0E64 */
    goto L0E64;
L0D9B: /* L0D9B */
    /* 0D9B  mov     word ptr ds:[0B3Eh],ax */
    ww(pDS, 0xB3E, AX);
L0D9E:
    /* 0D9E  mov     word ptr ds:[0B20h],si */
    ww(pDS, 0xB20, SI);
L0DA2:
    /* 0DA2  sub     word ptr ds:[0B26h],ax */
    ww(pDS, 0xB26, sub16(rw(pDS, 0xB26), AX, 0));
L0DA6:
    /* 0DA6  jg      L0DAB */
    if (!ZF && SF == OF) goto L0DAB;
L0DA8:
    /* 0DA8  jmp     L0E64 */
    goto L0E64;
L0DAB: /* L0DAB */
    /* 0DAB  cmp     word ptr ds:[0B26h],200h */
    sub16(rw(pDS, 0xB26), 0x200, 0);
L0DB1:
    /* 0DB1  jl      L0DB6 */
    if (SF != OF) goto L0DB6;
L0DB3:
    /* 0DB3  jmp     L0E64 */
    goto L0E64;
L0DB6: /* L0DB6 */
    /* 0DB6  mov     di,offset L1554 */
    DI = 0x1554;
L0DB9:
    /* 0DB9  call    word ptr ds:[0B0Eh] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, 0xB0E)), 0x0DBD)) != 0) return c;
L0DBD:
    /* 0DBD  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0DBF:
    /* 0DBF  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0DC1:
    /* 0DC1  mov     ah,byte ptr ds:[0B1Eh] */
    AH = rb(pDS, 0xB1E);
L0DC5:
    /* 0DC5  mov     dl,byte ptr ds:[0B1Fh] */
    DL = rb(pDS, 0xB1F);
L0DC9:
    /* 0DC9  div     word ptr ds:[0B24h] */
    if (asm_div16(rw(pDS, 0xB24)) && (c = asm_divfault(0x0085, 0x0DC9, 4)) != 0) return c;
L0DCD:
    /* 0DCD  mov     word ptr ds:[0B3Ah],ax */
    ww(pDS, 0xB3A, AX);
L0DD0:
    /* 0DD0  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0DD2:
    /* 0DD2  mov     cx,word ptr ds:[0B24h] */
    CX = rw(pDS, 0xB24);
L0DD6:
    /* 0DD6  mov     bp,word ptr ds:[0B22h] */
    BP = rw(pDS, 0xB22);
L0DDA:
    /* 0DDA  cmp     bp,word ptr ds:[3DF6h] */
    sub16(BP, rw(pDS, 0x3DF6), 0);
L0DDE:
    /* 0DDE  jle     L0DF4 */
    if (ZF || SF != OF) goto L0DF4;
L0DE0:
    /* 0DE0  sub     bp,word ptr ds:[3DF6h] */
    BP = (uint16_t)(BP - rw(pDS, 0x3DF6));
L0DE4:
    /* 0DE4  sub     cx,bp */
    CX = sub16(CX, BP, 0);
L0DE6:
    /* 0DE6  jle     L0E64 */
    if (ZF || SF != OF) goto L0E64;
L0DE8:
    /* 0DE8  mov     ax,bp */
    AX = BP;
L0DEA:
    /* 0DEA  mul     word ptr ds:[0B3Ah] */
    mul16(rw(pDS, 0xB3A));
L0DEE:
    /* 0DEE  mov     dx,ax */
    DX = AX;
L0DF0:
    /* 0DF0  mov     bp,word ptr ds:[3DF6h] */
    BP = rw(pDS, 0x3DF6);
L0DF4: /* L0DF4 */
    /* 0DF4  mov     bx,bp */
    BX = BP;
L0DF6:
    /* 0DF6  sub     bx,cx */
    BX = (uint16_t)(BX - CX);
L0DF8:
    /* 0DF8  inc     bx */
    BX = (uint16_t)(BX + 1);
L0DF9:
    /* 0DF9  cmp     bx,word ptr ds:[3DFAh] */
    sub16(BX, rw(pDS, 0x3DFA), 0);
L0DFD:
    /* 0DFD  jge     L0E07 */
    if (SF == OF) goto L0E07;
L0DFF:
    /* 0DFF  sub     bx,word ptr ds:[3DFAh] */
    BX = (uint16_t)(BX - rw(pDS, 0x3DFA));
L0E03:
    /* 0E03  add     cx,bx */
    CX = add16(CX, BX, 0);
L0E05:
    /* 0E05  jle     L0E64 */
    if (ZF || SF != OF) goto L0E64;
L0E07: /* L0E07 */
    /* 0E07  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L0E09:
    /* 0E09  add     bp,95Ch */
    BP = (uint16_t)(BP + 0x95C);
L0E0D:
    /* 0E0D  mov     bx,0B28h */
    BX = 0xB28;
L0E10:
    /* 0E10  mov     word ptr ds:[0B3Ch],cx */
    ww(pDS, 0xB3C, CX);
L0E14:
    /* 0E14  mov     cl,0FFh */
    CL = 0xFF;
L0E16: /* L0E16 */
    /* 0E16  cmp     cl,dh */
    sub8(CL, DH, 0);
L0E18:
    /* 0E18  je      L0E40 */
    if (ZF) goto L0E40;
L0E1A:
    /* 0E1A  mov     al,dh */
    AL = DH;
L0E1C:
    /* 0E1C  mul     byte ptr ds:[0DC0h] */
    mul8(rb(pDS, 0xDC0));
L0E20:
    /* 0E20  mov     si,word ptr ds:[0B18h] */
    SI = rw(pDS, 0xB18);
L0E24:
    /* 0E24  add     si,word ptr ds:[0B3Eh] */
    SI = (uint16_t)(SI + rw(pDS, 0xB3E));
L0E28:
    /* 0E28  mov     es,word ptr ds:[0B0Ch] */
    SET_ES(rw(pDS, 0xB0C));
L0E2C:
    /* 0E2C  mov     ds,word ptr ds:[0B1Ah] */
    SET_DS(rw(pDS, 0xB1A));
L0E30:
    /* 0E30  add     si,ax */
    SI = (uint16_t)(SI + AX);
L0E32:
    /* 0E32  mov     di,0B40h */
    DI = 0xB40;
L0E35:
    /* 0E35  mov     cx,0F04h */
    CX = 0xF04;
L0E38:
    /* 0E38  call    near ptr L1554 */
    /* by hand: call near ptr L1554: the scaler the generators wrote is interpreted (gfx/scalebm_code.c) */
    scalebm_run_generated(0x1554);
L0E3B:
    /* 0E3B  mov     ds,word ptr ss:[0B0Ch] */
    SET_DS(rw(pSS, 0xB0C));
L0E40: /* L0E40 */
    /* 0E40  mov     es,word ptr ds:[958h] */
    SET_ES(rw(pDS, 0x958));
L0E44:
    /* 0E44  mov     di,word ptr [bp] */
    DI = rw(pSS, BP);
L0E47:
    /* 0E47  dec     bp */
    BP = (uint16_t)(BP - 1);
L0E48:
    /* 0E48  dec     bp */
    BP = (uint16_t)(BP - 1);
L0E49:
    /* 0E49  add     di,word ptr ds:[0B20h] */
    DI = (uint16_t)(DI + rw(pDS, 0xB20));
L0E4D:
    /* 0E4D  mov     si,0B40h */
    SI = 0xB40;
L0E50:
    /* 0E50  mov     cx,word ptr ds:[0B26h] */
    CX = rw(pDS, 0xB26);
L0E54:
    /* 0E54  call    word ptr ds:[0B10h] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, 0xB10)), 0x0E58)) != 0) return c;
L0E58:
    /* 0E58  mov     cl,dh */
    CL = DH;
L0E5A:
    /* 0E5A  add     dx,word ptr ds:[0B3Ah] */
    DX = add16(DX, rw(pDS, 0xB3A), 0);
L0E5E:
    /* 0E5E  dec     word ptr ds:[0B3Ch] */
    ww(pDS, 0xB3C, dec16(rw(pDS, 0xB3C)));
L0E62:
    /* 0E62  jne     L0E16 */
    if (!ZF) goto L0E16;
L0E64: /* L0E64 */
    /* 0E64  cli */
    ;
L0E65:
    /* 0E65  mov     ss,word ptr cs:L1550 */
    SET_SS(rw(CODE003, 0x1550));
L0E6A:
    /* 0E6A  mov     sp,word ptr cs:L1552 */
    SP = rw(CODE003, 0x1552);
L0E6F:
    /* 0E6F  sti */
    ;
L0E70:
    /* 0E70  pop     ds */
    SET_DS(pop16());
L0E71:
    /* 0E71  pop     es */
    SET_ES(pop16());
L0E72:
    /* 0E72  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_0272_1373  (+1373): span routines and the scaler generators, reached through
       The span writers (CX pixels from SI at 0B40 to ES:DI in the frame buffer): _1373 remaps the
       screen pixel under each non-zero source pixel through cXfer table 4 (a translucent sprite),
       L1388 the same with table 3; the unlabelled writer after L139D writes non-zero pixels, those
       from 0F9h through the cXfer table they name; then a plain copy (rep movsb), a transparent copy
       (skip 0), and a writer that puts bmcolor (0DC1) wherever the source is non-zero, for pick
       frames.

       The generators (write code to ES:DI = L1554, ending it with a ret): the first (after L1420)
       unpacks four-bit pixels, choosing the nibble by the parity of the source position and
       translating through a 16-entry palette with xlat ss:; the one after L14A6 scales a byte row,
       emitting movsb for pixels kept once, lodsb then several stosb for pixels repeated when
       enlarging (step below 1.0), or skipping source bytes with inc si / add si,n when shrinking
       (step above 1.0). */
L1373: /* _seg003_0272_1373 */
    /* 1373  mov     bx,offset _cXfer+400h */
    BX = 0x1273;
L1376: /* L1376 */
    /* 1376  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1377:
    /* 1377  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1379:
    /* 1379  je      L1384 */
    if (ZF) goto L1384;
L137B:
    /* 137B  mov     al,byte ptr es:[di] */
    AL = rb(pES, DI);
L137E:
    /* 137E  xlat    byte ptr cs:[bx] */
    AL = rb(CODE003, BX + AL);
L1380: /* L1380 */
    /* 1380  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1381:
    /* 1381  loop    L1376 */
    if (--CX) goto L1376;
L1383:
    /* 1383  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1384: /* L1384 */
    /* 1384  inc     di */
    DI = inc16(DI);
L1385:
    /* 1385  loop    L1376 */
    if (--CX) goto L1376;
L1387:
    /* 1387  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1388: /* L1388 */
    /* 1388  mov     bx,offset _cXfer+300h */
    BX = 0x1173;
L138B: /* L138B */
    /* 138B  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L138C:
    /* 138C  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L138E:
    /* 138E  je      L1399 */
    if (ZF) goto L1399;
L1390:
    /* 1390  mov     al,byte ptr es:[di] */
    AL = rb(pES, DI);
L1393:
    /* 1393  xlat    byte ptr cs:[bx] */
    AL = rb(CODE003, BX + AL);
L1395:
    /* 1395  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1396:
    /* 1396  loop    L138B */
    if (--CX) goto L138B;
L1398:
    /* 1398  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1399: /* L1399 */
    /* 1399  inc     di */
    DI = inc16(DI);
L139A:
    /* 139A  loop    L138B */
    if (--CX) goto L138B;
L139C:
    /* 139C  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L139D: /* L139D */
    /* 139D  sub     al,0F9h */
    AL = (uint8_t)(AL - 0xF9);
L139F:
    /* 139F  mov     bx,offset _cXfer */
    BX = 0xE73;
L13A2:
    /* 13A2  add     bh,al */
    BH = (uint8_t)(BH + AL);
L13A4:
    /* 13A4  mov     al,byte ptr es:[di] */
    AL = rb(pES, DI);
L13A7:
    /* 13A7  xlat    byte ptr cs:[bx] */
    AL = rb(CODE003, BX + AL);
L13A9:
    /* 13A9  jmp     short L13BA */
    goto L13BA;
L13AB:
    /* 13AB  mov     bx,offset _cXfer */
    BX = 0xE73;
L13AE:
    /* 13AE  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L13AF:
    /* 13AF  dec     cx */
    CX = dec16(CX);
L13B0:
    /* 13B0  je      L13CC */
    if (ZF) goto L13CC;
L13B2:
    /* 13B2  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L13B4:
    /* 13B4  je      L13C2 */
    if (ZF) goto L13C2;
L13B6: /* L13B6 */
    /* 13B6  cmp     al,0F9h */
    sub8(AL, 0xF9, 0);
L13B8:
    /* 13B8  jae     L139D */
    if (!CF) goto L139D;
L13BA: /* L13BA */
    /* 13BA  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L13BB:
    /* 13BB  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L13BC:
    /* 13BC  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L13BE:
    /* 13BE  loopne  L13B6 */
    if (--CX && !ZF) goto L13B6;
L13C0:
    /* 13C0  jcxz    L13CE */
    if (!CX) goto L13CE;
L13C2: /* L13C2 */
    /* 13C2  inc     di */
    DI = (uint16_t)(DI + 1);
L13C3:
    /* 13C3  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L13C4:
    /* 13C4  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L13C6:
    /* 13C6  loope   L13C2 */
    if (--CX && ZF) goto L13C2;
L13C8:
    /* 13C8  jcxz    L13CE */
    if (!CX) goto L13CE;
L13CA:
    /* 13CA  jmp     L13B6 */
    goto L13B6;
L13CC: /* L13CC */
    /* 13CC  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L13CE: /* L13CE */
    /* 13CE  je      L13D5 */
    if (ZF) goto L13D5;
L13D0:
    /* 13D0  cmp     al,0F9h */
    sub8(AL, 0xF9, 0);
L13D2:
    /* 13D2  jae     L13D5 */
    if (!CF) goto L13D5;
L13D4:
    /* 13D4  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L13D5: /* L13D5 */
    /* 13D5  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L13D6:
    /* 13D6  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L13D8:
    /* 13D8  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L13D9:
    /* 13D9  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L13DA:
    /* 13DA  dec     cx */
    CX = dec16(CX);
L13DB:
    /* 13DB  je      L13F3 */
    if (ZF) goto L13F3;
L13DD:
    /* 13DD  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L13DF:
    /* 13DF  je      L13E9 */
    if (ZF) goto L13E9;
L13E1: /* L13E1 */
    /* 13E1  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L13E2:
    /* 13E2  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L13E3:
    /* 13E3  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L13E5:
    /* 13E5  loopne  L13E1 */
    if (--CX && !ZF) goto L13E1;
L13E7:
    /* 13E7  jcxz    L13F5 */
    if (!CX) goto L13F5;
L13E9: /* L13E9 */
    /* 13E9  inc     di */
    DI = (uint16_t)(DI + 1);
L13EA:
    /* 13EA  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L13EB:
    /* 13EB  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L13ED:
    /* 13ED  loope   L13E9 */
    if (--CX && ZF) goto L13E9;
L13EF:
    /* 13EF  jcxz    L13F5 */
    if (!CX) goto L13F5;
L13F1:
    /* 13F1  jmp     L13E1 */
    goto L13E1;
L13F3: /* L13F3 */
    /* 13F3  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L13F5: /* L13F5 */
    /* 13F5  je      L13F8 */
    if (ZF) goto L13F8;
L13F7:
    /* 13F7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L13F8: /* L13F8 */
    /* 13F8  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L13F9:
    /* 13F9  mov     bl,byte ptr ds:[0DC1h] */
    BL = rb(pDS, 0xDC1);
L13FD:
    /* 13FD  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L13FE:
    /* 13FE  dec     cx */
    CX = dec16(CX);
L13FF:
    /* 13FF  je      L1419 */
    if (ZF) goto L1419;
L1401:
    /* 1401  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1403:
    /* 1403  je      L140F */
    if (ZF) goto L140F;
L1405: /* L1405 */
    /* 1405  mov     al,bl */
    AL = BL;
L1407:
    /* 1407  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1408:
    /* 1408  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1409:
    /* 1409  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L140B:
    /* 140B  loopne  L1405 */
    if (--CX && !ZF) goto L1405;
L140D:
    /* 140D  jcxz    L141B */
    if (!CX) goto L141B;
L140F: /* L140F */
    /* 140F  inc     di */
    DI = (uint16_t)(DI + 1);
L1410:
    /* 1410  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1411:
    /* 1411  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1413:
    /* 1413  loope   L140F */
    if (--CX && ZF) goto L140F;
L1415:
    /* 1415  jcxz    L141B */
    if (!CX) goto L141B;
L1417:
    /* 1417  jmp     L1405 */
    goto L1405;
L1419: /* L1419 */
    /* 1419  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L141B: /* L141B */
    /* 141B  je      L1420 */
    if (ZF) goto L1420;
L141D:
    /* 141D  mov     al,bl */
    AL = BL;
L141F:
    /* 141F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1420: /* L1420 */
    /* 1420  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1421:
    /* 1421  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L1423:
    /* 1423  xor     bp,bp */
    BP = (uint16_t)(BP ^ BP);
L1425:
    /* 1425  mov     ax,word ptr ds:[0B1Ch] */
    AX = rw(pDS, 0xB1C);
L1428:
    /* 1428  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L142A:
    /* 142A  mov     byte ptr ds:[0DC0h],al */
    wb(pDS, 0xDC0, AL);
L142D:
    /* 142D  test    byte ptr ds:[0B39h],0FFh */
    logic8((uint8_t)(rb(pDS, 0xB39) & 0xFF));
L1432:
    /* 1432  je      L146F */
    if (ZF) goto L146F;
L1434: /* L1434 */
    /* 1434  xor     bx,bx */
    BX = (uint16_t)(BX ^ BX);
L1436:
    /* 1436  mov     bl,dh */
    BL = DH;
L1438:
    /* 1438  mov     si,0E820h */
    SI = 0xE820;
L143B:
    /* 143B  shr     bx,1 */
    BX = shr16(BX, 1);
L143D:
    /* 143D  jb      L1442 */
    if (CF) goto L1442;
L143F:
    /* 143F  mov     si,0E8D2h */
    SI = 0xE8D2;
L1442: /* L1442 */
    /* 1442  sub     bx,bp */
    BX = sub16(BX, BP, 0);
L1444:
    /* 1444  je      L1459 */
    if (ZF) goto L1459;
L1446:
    /* 1446  add     bp,bx */
    BP = (uint16_t)(BP + BX);
L1448:
    /* 1448  cmp     bl,0FFh */
    sub8(BL, 0xFF, 0);
L144B:
    /* 144B  jne     L1452 */
    if (!ZF) goto L1452;
L144D:
    /* 144D  mov     al,4Eh */
    AL = 0x4E;
L144F:
    /* 144F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1450:
    /* 1450  jmp     short L1459 */
    goto L1459;
L1452: /* L1452 */
    /* 1452  mov     ax,0C683h */
    AX = 0xC683;
L1455:
    /* 1455  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1456:
    /* 1456  mov     al,bl */
    AL = BL;
L1458:
    /* 1458  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1459: /* L1459 */
    /* 1459  mov     al,0ACh */
    AL = 0xAC;
L145B:
    /* 145B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L145C:
    /* 145C  mov     ax,si */
    AX = SI;
L145E:
    /* 145E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L145F:
    /* 145F  mov     al,36h */
    AL = 0x36;
L1461:
    /* 1461  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1462:
    /* 1462  mov     ax,0AAD7h */
    AX = 0xAAD7;
L1465:
    /* 1465  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1466:
    /* 1466  inc     bp */
    BP = (uint16_t)(BP + 1);
L1467:
    /* 1467  add     dx,word ptr ds:[0B38h] */
    DX = add16(DX, rw(pDS, 0xB38), 0);
L146B:
    /* 146B  loop    L1434 */
    if (--CX) goto L1434;
L146D:
    /* 146D  jmp     short L14A6 */
    goto L14A6;
L146F: /* L146F */
    /* 146F  mov     bp,0FFF8h */
    BP = 0xFFF8;
L1472: /* L1472 */
    /* 1472  xor     bx,bx */
    BX = (uint16_t)(BX ^ BX);
L1474:
    /* 1474  mov     bl,dh */
    BL = DH;
L1476:
    /* 1476  cmp     bx,bp */
    sub16(BX, BP, 0);
L1478:
    /* 1478  je      L149D */
    if (ZF) goto L149D;
L147A:
    /* 147A  mov     bp,bx */
    BP = BX;
L147C:
    /* 147C  test    bx,1 */
    logic16((uint16_t)(BX & 0x1));
L1480:
    /* 1480  je      L148C */
    if (ZF) goto L148C;
L1482:
    /* 1482  mov     ax,0E088h */
    AX = 0xE088;
L1485:
    /* 1485  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1486:
    /* 1486  mov     ax,0E820h */
    AX = 0xE820;
L1489:
    /* 1489  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L148A:
    /* 148A  jmp     short L1497 */
    goto L1497;
L148C: /* L148C */
    /* 148C  mov     al,0ACh */
    AL = 0xAC;
L148E:
    /* 148E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L148F:
    /* 148F  mov     ax,0C488h */
    AX = 0xC488;
L1492:
    /* 1492  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1493:
    /* 1493  mov     ax,0E8D2h */
    AX = 0xE8D2;
L1496:
    /* 1496  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1497: /* L1497 */
    /* 1497  mov     al,36h */
    AL = 0x36;
L1499:
    /* 1499  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L149A:
    /* 149A  mov     al,0D7h */
    AL = 0xD7;
L149C:
    /* 149C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L149D: /* L149D */
    /* 149D  mov     al,0AAh */
    AL = 0xAA;
L149F:
    /* 149F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L14A0:
    /* 14A0  add     dx,word ptr ds:[0B38h] */
    DX = add16(DX, rw(pDS, 0xB38), 0);
L14A4:
    /* 14A4  loop    L1472 */
    if (--CX) goto L1472;
L14A6: /* L14A6 */
    /* 14A6  mov     al,0C3h */
    AL = 0xC3;
L14A8:
    /* 14A8  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L14A9:
    /* 14A9  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L14AA:
    /* 14AA  mov     bp,word ptr ds:[0B26h] */
    BP = rw(pDS, 0xB26);
L14AE:
    /* 14AE  mov     ax,word ptr ds:[0B1Ch] */
    AX = rw(pDS, 0xB1C);
L14B1:
    /* 14B1  mov     byte ptr ds:[0DC0h],al */
    wb(pDS, 0xDC0, AL);
L14B4:
    /* 14B4  mov     ax,word ptr ds:[0B38h] */
    AX = rw(pDS, 0xB38);
L14B7:
    /* 14B7  test    word ptr ds:[0B3Eh],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0xB3E) & 0xFFFF));
L14BD:
    /* 14BD  je      L14CF */
    if (ZF) goto L14CF;
L14BF:
    /* 14BF  mov     bx,word ptr ds:[0B3Eh] */
    BX = rw(pDS, 0xB3E);
L14C3:
    /* 14C3  imul    bx */
    imul16(BX);
L14C5:
    /* 14C5  mov     al,ah */
    AL = AH;
L14C7:
    /* 14C7  mov     ah,dl */
    AH = DL;
L14C9:
    /* 14C9  mov     word ptr ds:[0B3Eh],ax */
    ww(pDS, 0xB3E, AX);
L14CC:
    /* 14CC  mov     ax,word ptr ds:[0B38h] */
    AX = rw(pDS, 0xB38);
L14CF: /* L14CF */
    /* 14CF  mov     bx,ax */
    BX = AX;
L14D1:
    /* 14D1  test    ah,0FFh */
    logic8((uint8_t)(AH & 0xFF));
L14D4:
    /* 14D4  jne     L152A */
    if (!ZF) goto L152A;
L14D6:
    /* 14D6  cmp     ax,128h */
    sub16(AX, 0x128, 0);
L14D9:
    /* 14D9  jle     L1511 */
    if (ZF || SF != OF) goto L1511;
L14DB:
    /* 14DB  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L14DD:
    /* 14DD  mov     al,51h */
    AL = 0x51;
L14DF:
    /* 14DF  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L14E0:
    /* 14E0  xor     si,si */
    SI = (uint16_t)(SI ^ SI);
L14E2: /* L14E2 */
    /* 14E2  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L14E4: /* L14E4 */
    /* 14E4  add     bh,bl */
    BH = add8(BH, BL, 0);
L14E6:
    /* 14E6  inc     dx */
    DX = inc16(DX);
L14E7:
    /* 14E7  jae     L14E4 */
    if (!CF) goto L14E4;
L14E9:
    /* 14E9  cmp     dx,1 */
    sub16(DX, 0x1, 0);
L14EC:
    /* 14EC  jne     L14F1 */
    if (!ZF) goto L14F1;
L14EE:
    /* 14EE  inc     si */
    SI = (uint16_t)(SI + 1);
L14EF:
    /* 14EF  jmp     short L1502 */
    goto L1502;
L14F1: /* L14F1 */
    /* 14F1  mov     al,0A4h */
    AL = 0xA4;
L14F3:
    /* 14F3  mov     cx,si */
    CX = SI;
L14F5:
    /* 14F5  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L14F7:
    /* 14F7  xor     si,si */
    SI = (uint16_t)(SI ^ SI);
L14F9:
    /* 14F9  mov     al,0ACh */
    AL = 0xAC;
L14FB:
    /* 14FB  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L14FC:
    /* 14FC  mov     al,0AAh */
    AL = 0xAA;
L14FE:
    /* 14FE  mov     cx,dx */
    CX = DX;
L1500:
    /* 1500  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L1502: /* L1502 */
    /* 1502  sub     bp,dx */
    BP = sub16(BP, DX, 0);
L1504:
    /* 1504  jg      L14E2 */
    if (!ZF && SF == OF) goto L14E2;
L1506:
    /* 1506  mov     al,0A4h */
    AL = 0xA4;
L1508:
    /* 1508  mov     cx,si */
    CX = SI;
L150A:
    /* 150A  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L150C:
    /* 150C  mov     al,59h */
    AL = 0x59;
L150E:
    /* 150E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L150F:
    /* 150F  jmp     L14A6 */
    goto L14A6;
L1511: /* L1511 */
    /* 1511  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L1513: /* L1513 */
    /* 1513  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L1515: /* L1515 */
    /* 1515  inc     dx */
    DX = (uint16_t)(DX + 1);
L1516:
    /* 1516  add     bh,bl */
    BH = add8(BH, BL, 0);
L1518:
    /* 1518  jae     L1515 */
    if (!CF) goto L1515;
L151A:
    /* 151A  mov     al,0ACh */
    AL = 0xAC;
L151C:
    /* 151C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L151D:
    /* 151D  mov     al,0AAh */
    AL = 0xAA;
L151F:
    /* 151F  mov     cx,dx */
    CX = DX;
L1521:
    /* 1521  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L1523:
    /* 1523  sub     bp,dx */
    BP = sub16(BP, DX, 0);
L1525:
    /* 1525  jg      L1513 */
    if (!ZF && SF == OF) goto L1513;
L1527:
    /* 1527  jmp     L14A6 */
    goto L14A6;
L152A: /* L152A */
    /* 152A  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L152C: /* L152C */
    /* 152C  mov     al,0A4h */
    AL = 0xA4;
L152E:
    /* 152E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L152F:
    /* 152F  add     dx,bx */
    DX = add16(DX, BX, 0);
L1531:
    /* 1531  dec     dh */
    DH = dec8(DH);
L1533:
    /* 1533  je      L154A */
    if (ZF) goto L154A;
L1535:
    /* 1535  cmp     dh,1 */
    sub8(DH, 0x1, 0);
L1538:
    /* 1538  jg      L1541 */
    if (!ZF && SF == OF) goto L1541;
L153A:
    /* 153A  mov     al,46h */
    AL = 0x46;
L153C:
    /* 153C  dec     dh */
    DH = dec8(DH);
L153E:
    /* 153E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L153F:
    /* 153F  jmp     short L154A */
    goto L154A;
L1541: /* L1541 */
    /* 1541  mov     ax,0C683h */
    AX = 0xC683;
L1544:
    /* 1544  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1545:
    /* 1545  mov     al,dh */
    AL = DH;
L1547:
    /* 1547  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1548:
    /* 1548  xor     dh,dh */
    DH = logic8((uint8_t)(DH ^ DH));
L154A: /* L154A */
    /* 154A  dec     bp */
    BP = dec16(BP);
L154B:
    /* 154B  jne     L152C */
    if (!ZF) goto L152C;
L154D:
    /* 154D  jmp     L14A6 */
    goto L14A6;
}
