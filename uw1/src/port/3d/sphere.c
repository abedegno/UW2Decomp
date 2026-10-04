/* sphere.c: replaces src/3d/SPHERE.ASM (seg004_1370, 1370..1FC6 of its
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

uint32_t asm_mod_SPHERE(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x1370: goto L1370;
    case 0x1374: goto L1374;
    case 0x1376: goto L1376;
    case 0x1378: goto L1378;
    case 0x137A: goto L137A;
    case 0x137C: goto L137C;
    case 0x137E: goto L137E;
    case 0x1380: goto L1380;
    case 0x1382: goto L1382;
    case 0x1384: goto L1384;
    case 0x1386: goto L1386;
    case 0x138A: goto L138A;
    case 0x138C: goto L138C;
    case 0x138E: goto L138E;
    case 0x1392: goto L1392;
    case 0x1394: goto L1394;
    case 0x1396: goto L1396;
    case 0x139A: goto L139A;
    case 0x139C: goto L139C;
    case 0x13A0: goto L13A0;
    case 0x13A2: goto L13A2;
    case 0x13A4: goto L13A4;
    case 0x13A6: goto L13A6;
    case 0x13A9: goto L13A9;
    case 0x13AC: goto L13AC;
    case 0x13AE: goto L13AE;
    case 0x13B2: goto L13B2;
    case 0x13B4: goto L13B4;
    case 0x13B6: goto L13B6;
    case 0x13BA: goto L13BA;
    case 0x13BC: goto L13BC;
    case 0x13BE: goto L13BE;
    case 0x13C2: goto L13C2;
    case 0x13C4: goto L13C4;
    case 0x13C6: goto L13C6;
    case 0x13CA: goto L13CA;
    case 0x13CD: goto L13CD;
    case 0x13CF: goto L13CF;
    case 0x13D1: goto L13D1;
    case 0x13D3: goto L13D3;
    case 0x13D5: goto L13D5;
    case 0x13D8: goto L13D8;
    case 0x13DB: goto L13DB;
    case 0x13DD: goto L13DD;
    case 0x13DF: goto L13DF;
    case 0x13E2: goto L13E2;
    case 0x13E6: goto L13E6;
    case 0x13E8: goto L13E8;
    case 0x13EA: goto L13EA;
    case 0x13EC: goto L13EC;
    case 0x13EF: goto L13EF;
    case 0x13F2: goto L13F2;
    case 0x13F4: goto L13F4;
    case 0x13F6: goto L13F6;
    case 0x13F9: goto L13F9;
    case 0x13FB: goto L13FB;
    case 0x13FF: goto L13FF;
    case 0x1401: goto L1401;
    case 0x1403: goto L1403;
    case 0x1407: goto L1407;
    case 0x1409: goto L1409;
    case 0x140B: goto L140B;
    case 0x140F: goto L140F;
    case 0x1411: goto L1411;
    case 0x1413: goto L1413;
    case 0x1417: goto L1417;
    case 0x141A: goto L141A;
    case 0x141C: goto L141C;
    case 0x141E: goto L141E;
    case 0x1420: goto L1420;
    case 0x1422: goto L1422;
    case 0x1425: goto L1425;
    case 0x1428: goto L1428;
    case 0x142A: goto L142A;
    case 0x142C: goto L142C;
    case 0x142F: goto L142F;
    case 0x1433: goto L1433;
    case 0x1435: goto L1435;
    case 0x1437: goto L1437;
    case 0x1439: goto L1439;
    case 0x143C: goto L143C;
    case 0x143F: goto L143F;
    case 0x1441: goto L1441;
    case 0x1443: goto L1443;
    case 0x1446: goto L1446;
    case 0x1449: goto L1449;
    case 0x144B: goto L144B;
    case 0x144D: goto L144D;
    case 0x144F: goto L144F;
    case 0x1451: goto L1451;
    case 0x1455: goto L1455;
    case 0x1457: goto L1457;
    case 0x145D: goto L145D;
    case 0x145F: goto L145F;
    case 0x1463: goto L1463;
    case 0x1465: goto L1465;
    case 0x1467: goto L1467;
    case 0x1469: goto L1469;
    case 0x146B: goto L146B;
    case 0x146D: goto L146D;
    case 0x146F: goto L146F;
    case 0x1472: goto L1472;
    case 0x1474: goto L1474;
    case 0x1476: goto L1476;
    case 0x1478: goto L1478;
    case 0x147A: goto L147A;
    case 0x147B: goto L147B;
    case 0x147E: goto L147E;
    case 0x147F: goto L147F;
    case 0x1481: goto L1481;
    case 0x1485: goto L1485;
    case 0x1487: goto L1487;
    case 0x1489: goto L1489;
    case 0x148D: goto L148D;
    case 0x148F: goto L148F;
    case 0x1491: goto L1491;
    case 0x1495: goto L1495;
    case 0x1497: goto L1497;
    case 0x1499: goto L1499;
    case 0x149D: goto L149D;
    case 0x14A0: goto L14A0;
    case 0x14A2: goto L14A2;
    case 0x14A4: goto L14A4;
    case 0x14A6: goto L14A6;
    case 0x14A8: goto L14A8;
    case 0x14AC: goto L14AC;
    case 0x14AE: goto L14AE;
    case 0x14B0: goto L14B0;
    case 0x14B2: goto L14B2;
    case 0x14B4: goto L14B4;
    case 0x14B8: goto L14B8;
    case 0x14BA: goto L14BA;
    case 0x14BC: goto L14BC;
    case 0x14C0: goto L14C0;
    case 0x14C2: goto L14C2;
    case 0x14C4: goto L14C4;
    case 0x14C8: goto L14C8;
    case 0x14CA: goto L14CA;
    case 0x14CC: goto L14CC;
    case 0x14D0: goto L14D0;
    case 0x14D3: goto L14D3;
    case 0x14D5: goto L14D5;
    case 0x14D7: goto L14D7;
    case 0x14D9: goto L14D9;
    case 0x14DB: goto L14DB;
    case 0x14DF: goto L14DF;
    case 0x14E1: goto L14E1;
    case 0x14E3: goto L14E3;
    case 0x14E5: goto L14E5;
    case 0x14E8: goto L14E8;
    case 0x14EA: goto L14EA;
    case 0x14EC: goto L14EC;
    case 0x14EE: goto L14EE;
    case 0x14F0: goto L14F0;
    case 0x14F4: goto L14F4;
    case 0x14F6: goto L14F6;
    case 0x14FC: goto L14FC;
    case 0x14FE: goto L14FE;
    case 0x1502: goto L1502;
    case 0x1504: goto L1504;
    case 0x1506: goto L1506;
    case 0x1508: goto L1508;
    case 0x150A: goto L150A;
    case 0x150C: goto L150C;
    case 0x150E: goto L150E;
    case 0x1511: goto L1511;
    case 0x1513: goto L1513;
    case 0x1515: goto L1515;
    case 0x1517: goto L1517;
    case 0x1519: goto L1519;
    case 0x151A: goto L151A;
    case 0x151C: goto L151C;
    case 0x151D: goto L151D;
    case 0x151E: goto L151E;
    case 0x151F: goto L151F;
    case 0x1520: goto L1520;
    case 0x1524: goto L1524;
    case 0x1527: goto L1527;
    case 0x1528: goto L1528;
    case 0x152A: goto L152A;
    case 0x152C: goto L152C;
    case 0x152E: goto L152E;
    case 0x1530: goto L1530;
    case 0x1533: goto L1533;
    case 0x1535: goto L1535;
    case 0x1539: goto L1539;
    case 0x153C: goto L153C;
    case 0x153D: goto L153D;
    case 0x153F: goto L153F;
    case 0x1541: goto L1541;
    case 0x1543: goto L1543;
    case 0x1545: goto L1545;
    case 0x1548: goto L1548;
    case 0x154A: goto L154A;
    case 0x154C: goto L154C;
    case 0x154E: goto L154E;
    case 0x1550: goto L1550;
    case 0x1552: goto L1552;
    case 0x1553: goto L1553;
    case 0x1555: goto L1555;
    case 0x1557: goto L1557;
    case 0x1559: goto L1559;
    case 0x155B: goto L155B;
    case 0x155D: goto L155D;
    case 0x155F: goto L155F;
    case 0x1563: goto L1563;
    case 0x1566: goto L1566;
    case 0x1567: goto L1567;
    case 0x1569: goto L1569;
    case 0x156B: goto L156B;
    case 0x156D: goto L156D;
    case 0x156F: goto L156F;
    case 0x1572: goto L1572;
    case 0x1574: goto L1574;
    case 0x1576: goto L1576;
    case 0x1578: goto L1578;
    case 0x157A: goto L157A;
    case 0x157C: goto L157C;
    case 0x157E: goto L157E;
    case 0x1580: goto L1580;
    case 0x1582: goto L1582;
    case 0x1584: goto L1584;
    case 0x1586: goto L1586;
    case 0x1588: goto L1588;
    case 0x158A: goto L158A;
    case 0x158C: goto L158C;
    case 0x158E: goto L158E;
    case 0x1590: goto L1590;
    case 0x1592: goto L1592;
    case 0x1594: goto L1594;
    case 0x1596: goto L1596;
    case 0x1598: goto L1598;
    case 0x159A: goto L159A;
    case 0x159C: goto L159C;
    case 0x159E: goto L159E;
    case 0x15A0: goto L15A0;
    case 0x15A2: goto L15A2;
    case 0x15A4: goto L15A4;
    case 0x15A6: goto L15A6;
    case 0x15A8: goto L15A8;
    case 0x15AA: goto L15AA;
    case 0x15AB: goto L15AB;
    case 0x15AC: goto L15AC;
    case 0x15AF: goto L15AF;
    case 0x15B0: goto L15B0;
    case 0x15B1: goto L15B1;
    case 0x15B3: goto L15B3;
    case 0x15B5: goto L15B5;
    case 0x15B8: goto L15B8;
    case 0x15BC: goto L15BC;
    case 0x15BD: goto L15BD;
    case 0x15BF: goto L15BF;
    case 0x15C3: goto L15C3;
    case 0x15C7: goto L15C7;
    case 0x15C8: goto L15C8;
    case 0x15CA: goto L15CA;
    case 0x15CE: goto L15CE;
    case 0x15CF: goto L15CF;
    case 0x15D1: goto L15D1;
    case 0x15D3: goto L15D3;
    case 0x15D5: goto L15D5;
    case 0x15D7: goto L15D7;
    case 0x15D9: goto L15D9;
    case 0x15DB: goto L15DB;
    case 0x15DD: goto L15DD;
    case 0x15DF: goto L15DF;
    case 0x15E1: goto L15E1;
    case 0x15E3: goto L15E3;
    case 0x15E5: goto L15E5;
    case 0x15E7: goto L15E7;
    case 0x15E8: goto L15E8;
    case 0x15EA: goto L15EA;
    case 0x15EC: goto L15EC;
    case 0x15EF: goto L15EF;
    case 0x15F2: goto L15F2;
    case 0x15F3: goto L15F3;
    case 0x15F5: goto L15F5;
    case 0x15F7: goto L15F7;
    case 0x15F9: goto L15F9;
    case 0x15FD: goto L15FD;
    case 0x15FE: goto L15FE;
    case 0x1600: goto L1600;
    case 0x1604: goto L1604;
    case 0x1608: goto L1608;
    case 0x1609: goto L1609;
    case 0x160B: goto L160B;
    case 0x160F: goto L160F;
    case 0x1610: goto L1610;
    case 0x1612: goto L1612;
    case 0x1614: goto L1614;
    case 0x1616: goto L1616;
    case 0x1618: goto L1618;
    case 0x161A: goto L161A;
    case 0x161C: goto L161C;
    case 0x161E: goto L161E;
    case 0x1620: goto L1620;
    case 0x1622: goto L1622;
    case 0x1624: goto L1624;
    case 0x1626: goto L1626;
    case 0x1628: goto L1628;
    case 0x1629: goto L1629;
    case 0x162B: goto L162B;
    case 0x162D: goto L162D;
    case 0x1630: goto L1630;
    case 0x1633: goto L1633;
    case 0x1634: goto L1634;
    case 0x1636: goto L1636;
    case 0x1638: goto L1638;
    case 0x163A: goto L163A;
    case 0x163E: goto L163E;
    case 0x163F: goto L163F;
    case 0x1641: goto L1641;
    case 0x1645: goto L1645;
    case 0x1649: goto L1649;
    case 0x164A: goto L164A;
    case 0x164C: goto L164C;
    case 0x1650: goto L1650;
    case 0x1651: goto L1651;
    case 0x1653: goto L1653;
    case 0x1655: goto L1655;
    case 0x1657: goto L1657;
    case 0x1659: goto L1659;
    case 0x165B: goto L165B;
    case 0x165D: goto L165D;
    case 0x165F: goto L165F;
    case 0x1661: goto L1661;
    case 0x1663: goto L1663;
    case 0x1665: goto L1665;
    case 0x1667: goto L1667;
    case 0x1669: goto L1669;
    case 0x166A: goto L166A;
    case 0x166C: goto L166C;
    case 0x166E: goto L166E;
    case 0x1671: goto L1671;
    case 0x1674: goto L1674;
    case 0x1675: goto L1675;
    case 0x1677: goto L1677;
    case 0x1679: goto L1679;
    case 0x167B: goto L167B;
    case 0x167C: goto L167C;
    case 0x167F: goto L167F;
    case 0x1681: goto L1681;
    case 0x1685: goto L1685;
    case 0x1687: goto L1687;
    case 0x168A: goto L168A;
    case 0x168C: goto L168C;
    case 0x168E: goto L168E;
    case 0x1691: goto L1691;
    case 0x1693: goto L1693;
    case 0x1695: goto L1695;
    case 0x1697: goto L1697;
    case 0x1698: goto L1698;
    case 0x169A: goto L169A;
    case 0x169B: goto L169B;
    case 0x169D: goto L169D;
    case 0x169F: goto L169F;
    case 0x16A3: goto L16A3;
    case 0x16A7: goto L16A7;
    case 0x16AA: goto L16AA;
    case 0x16AD: goto L16AD;
    case 0x16AF: goto L16AF;
    case 0x16B2: goto L16B2;
    case 0x16B5: goto L16B5;
    case 0x16B9: goto L16B9;
    case 0x16BB: goto L16BB;
    case 0x16BC: goto L16BC;
    case 0x16BD: goto L16BD;
    case 0x16BE: goto L16BE;
    case 0x16BF: goto L16BF;
    case 0x16C2: goto L16C2;
    case 0x16C3: goto L16C3;
    case 0x16C4: goto L16C4;
    case 0x16C5: goto L16C5;
    case 0x16C8: goto L16C8;
    case 0x16C9: goto L16C9;
    case 0x16CC: goto L16CC;
    case 0x16CD: goto L16CD;
    case 0x16D0: goto L16D0;
    case 0x16D1: goto L16D1;
    case 0x16D4: goto L16D4;
    case 0x16D5: goto L16D5;
    case 0x16D6: goto L16D6;
    case 0x16D7: goto L16D7;
    case 0x16DA: goto L16DA;
    case 0x16DD: goto L16DD;
    case 0x16DF: goto L16DF;
    case 0x16E5: goto L16E5;
    case 0x16E9: goto L16E9;
    case 0x16ED: goto L16ED;
    case 0x16EE: goto L16EE;
    case 0x16EF: goto L16EF;
    case 0x16F3: goto L16F3;
    case 0x16F5: goto L16F5;
    case 0x16F7: goto L16F7;
    case 0x16F8: goto L16F8;
    case 0x16F9: goto L16F9;
    case 0x16FA: goto L16FA;
    case 0x16FE: goto L16FE;
    case 0x1704: goto L1704;
    case 0x1706: goto L1706;
    case 0x1708: goto L1708;
    case 0x1709: goto L1709;
    case 0x170A: goto L170A;
    case 0x170B: goto L170B;
    case 0x170F: goto L170F;
    case 0x1711: goto L1711;
    case 0x1713: goto L1713;
    case 0x1714: goto L1714;
    case 0x1715: goto L1715;
    case 0x1716: goto L1716;
    case 0x1717: goto L1717;
    case 0x171B: goto L171B;
    case 0x171C: goto L171C;
    case 0x171D: goto L171D;
    case 0x171E: goto L171E;
    case 0x1720: goto L1720;
    case 0x1723: goto L1723;
    case 0x1724: goto L1724;
    case 0x1725: goto L1725;
    case 0x1726: goto L1726;
    case 0x1727: goto L1727;
    case 0x1728: goto L1728;
    case 0x1729: goto L1729;
    case 0x172A: goto L172A;
    case 0x172B: goto L172B;
    case 0x172C: goto L172C;
    case 0x1730: goto L1730;
    case 0x1731: goto L1731;
    case 0x1732: goto L1732;
    case 0x1735: goto L1735;
    case 0x1738: goto L1738;
    case 0x173A: goto L173A;
    case 0x173B: goto L173B;
    case 0x173C: goto L173C;
    case 0x173E: goto L173E;
    case 0x1740: goto L1740;
    case 0x1746: goto L1746;
    case 0x174C: goto L174C;
    case 0x174E: goto L174E;
    case 0x1752: goto L1752;
    case 0x1754: goto L1754;
    case 0x1755: goto L1755;
    case 0x1757: goto L1757;
    case 0x1759: goto L1759;
    case 0x175C: goto L175C;
    case 0x1760: goto L1760;
    case 0x1761: goto L1761;
    case 0x1762: goto L1762;
    case 0x1764: goto L1764;
    case 0x1766: goto L1766;
    case 0x1769: goto L1769;
    case 0x176B: goto L176B;
    case 0x176F: goto L176F;
    case 0x1770: goto L1770;
    case 0x1771: goto L1771;
    case 0x1773: goto L1773;
    case 0x1775: goto L1775;
    case 0x1778: goto L1778;
    case 0x177A: goto L177A;
    case 0x177C: goto L177C;
    case 0x177F: goto L177F;
    case 0x1781: goto L1781;
    case 0x1783: goto L1783;
    case 0x1785: goto L1785;
    case 0x1786: goto L1786;
    case 0x1788: goto L1788;
    case 0x1789: goto L1789;
    case 0x178B: goto L178B;
    case 0x178E: goto L178E;
    case 0x1791: goto L1791;
    case 0x1793: goto L1793;
    case 0x1796: goto L1796;
    case 0x1797: goto L1797;
    case 0x1798: goto L1798;
    case 0x179C: goto L179C;
    case 0x179D: goto L179D;
    case 0x179E: goto L179E;
    case 0x179F: goto L179F;
    case 0x17A0: goto L17A0;
    case 0x17A7: goto L17A7;
    case 0x17A8: goto L17A8;
    case 0x17A9: goto L17A9;
    case 0x17AA: goto L17AA;
    case 0x17AB: goto L17AB;
    case 0x17AF: goto L17AF;
    case 0x17B2: goto L17B2;
    case 0x17B5: goto L17B5;
    case 0x17B8: goto L17B8;
    case 0x17BB: goto L17BB;
    case 0x17BE: goto L17BE;
    case 0x17C1: goto L17C1;
    case 0x17C5: goto L17C5;
    case 0x17C8: goto L17C8;
    case 0x17CC: goto L17CC;
    case 0x17CE: goto L17CE;
    case 0x17D0: goto L17D0;
    case 0x17D2: goto L17D2;
    case 0x17D4: goto L17D4;
    case 0x17D6: goto L17D6;
    case 0x17D8: goto L17D8;
    case 0x17DA: goto L17DA;
    case 0x17DC: goto L17DC;
    case 0x17DE: goto L17DE;
    case 0x17E0: goto L17E0;
    case 0x17E2: goto L17E2;
    case 0x17E4: goto L17E4;
    case 0x17E5: goto L17E5;
    case 0x17E7: goto L17E7;
    case 0x17E9: goto L17E9;
    case 0x17EC: goto L17EC;
    case 0x17EF: goto L17EF;
    case 0x17F1: goto L17F1;
    case 0x17F3: goto L17F3;
    case 0x17F5: goto L17F5;
    case 0x17F8: goto L17F8;
    case 0x17FB: goto L17FB;
    case 0x17FE: goto L17FE;
    case 0x1802: goto L1802;
    case 0x1805: goto L1805;
    case 0x1809: goto L1809;
    case 0x180B: goto L180B;
    case 0x180D: goto L180D;
    case 0x180F: goto L180F;
    case 0x1811: goto L1811;
    case 0x1813: goto L1813;
    case 0x1815: goto L1815;
    case 0x1817: goto L1817;
    case 0x1819: goto L1819;
    case 0x181B: goto L181B;
    case 0x181D: goto L181D;
    case 0x181F: goto L181F;
    case 0x1821: goto L1821;
    case 0x1822: goto L1822;
    case 0x1824: goto L1824;
    case 0x1826: goto L1826;
    case 0x1829: goto L1829;
    case 0x182C: goto L182C;
    case 0x182E: goto L182E;
    case 0x1830: goto L1830;
    case 0x1832: goto L1832;
    case 0x1835: goto L1835;
    case 0x1838: goto L1838;
    case 0x183B: goto L183B;
    case 0x183F: goto L183F;
    case 0x1842: goto L1842;
    case 0x1846: goto L1846;
    case 0x1848: goto L1848;
    case 0x184A: goto L184A;
    case 0x184C: goto L184C;
    case 0x184E: goto L184E;
    case 0x1850: goto L1850;
    case 0x1852: goto L1852;
    case 0x1854: goto L1854;
    case 0x1856: goto L1856;
    case 0x1858: goto L1858;
    case 0x185A: goto L185A;
    case 0x185C: goto L185C;
    case 0x185E: goto L185E;
    case 0x185F: goto L185F;
    case 0x1861: goto L1861;
    case 0x1863: goto L1863;
    case 0x1866: goto L1866;
    case 0x1869: goto L1869;
    case 0x186B: goto L186B;
    case 0x186D: goto L186D;
    case 0x186F: goto L186F;
    case 0x1871: goto L1871;
    case 0x1874: goto L1874;
    case 0x1876: goto L1876;
    case 0x187A: goto L187A;
    case 0x187C: goto L187C;
    case 0x187E: goto L187E;
    case 0x1880: goto L1880;
    case 0x1883: goto L1883;
    case 0x1885: goto L1885;
    case 0x1887: goto L1887;
    case 0x1889: goto L1889;
    case 0x188A: goto L188A;
    case 0x188C: goto L188C;
    case 0x188D: goto L188D;
    case 0x188F: goto L188F;
    case 0x1891: goto L1891;
    case 0x1895: goto L1895;
    case 0x1899: goto L1899;
    case 0x189C: goto L189C;
    case 0x189F: goto L189F;
    case 0x18A1: goto L18A1;
    case 0x18A4: goto L18A4;
    case 0x18A7: goto L18A7;
    case 0x18AB: goto L18AB;
    case 0x18AD: goto L18AD;
    case 0x18AE: goto L18AE;
    case 0x18AF: goto L18AF;
    case 0x18B0: goto L18B0;
    case 0x18B1: goto L18B1;
    case 0x18B4: goto L18B4;
    case 0x18B5: goto L18B5;
    case 0x18B8: goto L18B8;
    case 0x18B9: goto L18B9;
    case 0x18BC: goto L18BC;
    case 0x18BD: goto L18BD;
    case 0x18C0: goto L18C0;
    case 0x18C1: goto L18C1;
    case 0x18C4: goto L18C4;
    case 0x18C5: goto L18C5;
    case 0x18C8: goto L18C8;
    case 0x18C9: goto L18C9;
    case 0x18CA: goto L18CA;
    case 0x18CB: goto L18CB;
    case 0x18CE: goto L18CE;
    case 0x18D1: goto L18D1;
    case 0x18D3: goto L18D3;
    case 0x18D9: goto L18D9;
    case 0x18DD: goto L18DD;
    case 0x18E1: goto L18E1;
    case 0x18E2: goto L18E2;
    case 0x18E3: goto L18E3;
    case 0x18E7: goto L18E7;
    case 0x18E9: goto L18E9;
    case 0x18EB: goto L18EB;
    case 0x18EC: goto L18EC;
    case 0x18F2: goto L18F2;
    case 0x18F4: goto L18F4;
    case 0x18F6: goto L18F6;
    case 0x18F7: goto L18F7;
    case 0x18F9: goto L18F9;
    case 0x18FB: goto L18FB;
    case 0x18FC: goto L18FC;
    case 0x18FD: goto L18FD;
    case 0x18FE: goto L18FE;
    case 0x18FF: goto L18FF;
    case 0x1900: goto L1900;
    case 0x1904: goto L1904;
    case 0x1908: goto L1908;
    case 0x190C: goto L190C;
    case 0x190E: goto L190E;
    case 0x1911: goto L1911;
    case 0x1912: goto L1912;
    case 0x1914: goto L1914;
    case 0x1915: goto L1915;
    case 0x1919: goto L1919;
    case 0x191A: goto L191A;
    case 0x191E: goto L191E;
    case 0x191F: goto L191F;
    case 0x1923: goto L1923;
    case 0x1924: goto L1924;
    case 0x1928: goto L1928;
    case 0x1929: goto L1929;
    case 0x192C: goto L192C;
    case 0x192F: goto L192F;
    case 0x1931: goto L1931;
    case 0x1933: goto L1933;
    case 0x1935: goto L1935;
    case 0x193B: goto L193B;
    case 0x1941: goto L1941;
    case 0x1944: goto L1944;
    case 0x1948: goto L1948;
    case 0x194C: goto L194C;
    case 0x1950: goto L1950;
    case 0x1954: goto L1954;
    case 0x1958: goto L1958;
    case 0x195C: goto L195C;
    case 0x1960: goto L1960;
    case 0x1964: goto L1964;
    case 0x1968: goto L1968;
    case 0x196C: goto L196C;
    case 0x196F: goto L196F;
    case 0x1972: goto L1972;
    case 0x1975: goto L1975;
    case 0x1978: goto L1978;
    case 0x197B: goto L197B;
    case 0x1980: goto L1980;
    case 0x1982: goto L1982;
    case 0x1984: goto L1984;
    case 0x1987: goto L1987;
    case 0x198A: goto L198A;
    case 0x198D: goto L198D;
    case 0x198E: goto L198E;
    case 0x1990: goto L1990;
    case 0x1993: goto L1993;
    case 0x1996: goto L1996;
    case 0x1998: goto L1998;
    case 0x199A: goto L199A;
    case 0x199B: goto L199B;
    case 0x199E: goto L199E;
    case 0x199F: goto L199F;
    case 0x19A2: goto L19A2;
    case 0x19A4: goto L19A4;
    case 0x19A6: goto L19A6;
    case 0x19A7: goto L19A7;
    case 0x19AA: goto L19AA;
    case 0x19AB: goto L19AB;
    case 0x19AE: goto L19AE;
    case 0x19B0: goto L19B0;
    case 0x19B2: goto L19B2;
    case 0x19B5: goto L19B5;
    case 0x19B8: goto L19B8;
    case 0x19B9: goto L19B9;
    case 0x19BB: goto L19BB;
    case 0x19BD: goto L19BD;
    case 0x19C0: goto L19C0;
    case 0x19C2: goto L19C2;
    case 0x19C4: goto L19C4;
    case 0x19C7: goto L19C7;
    case 0x19CA: goto L19CA;
    case 0x19CB: goto L19CB;
    case 0x19CD: goto L19CD;
    case 0x19CF: goto L19CF;
    case 0x19D2: goto L19D2;
    case 0x19D4: goto L19D4;
    case 0x19D6: goto L19D6;
    case 0x19D9: goto L19D9;
    case 0x19DA: goto L19DA;
    case 0x19DC: goto L19DC;
    case 0x19DE: goto L19DE;
    case 0x19E1: goto L19E1;
    case 0x19E3: goto L19E3;
    case 0x19E5: goto L19E5;
    case 0x19E6: goto L19E6;
    case 0x19E8: goto L19E8;
    case 0x19EB: goto L19EB;
    case 0x19ED: goto L19ED;
    case 0x19EF: goto L19EF;
    case 0x19F1: goto L19F1;
    case 0x19F2: goto L19F2;
    case 0x19F4: goto L19F4;
    case 0x19F5: goto L19F5;
    case 0x19F7: goto L19F7;
    case 0x19FA: goto L19FA;
    case 0x19FB: goto L19FB;
    case 0x19FE: goto L19FE;
    case 0x1A01: goto L1A01;
    case 0x1A05: goto L1A05;
    case 0x1A07: goto L1A07;
    case 0x1A09: goto L1A09;
    case 0x1A0D: goto L1A0D;
    case 0x1A0E: goto L1A0E;
    case 0x1A0F: goto L1A0F;
    case 0x1A13: goto L1A13;
    case 0x1A14: goto L1A14;
    case 0x1A15: goto L1A15;
    case 0x1A16: goto L1A16;
    case 0x1A1A: goto L1A1A;
    case 0x1A1E: goto L1A1E;
    case 0x1A22: goto L1A22;
    case 0x1A26: goto L1A26;
    case 0x1A2A: goto L1A2A;
    case 0x1A2E: goto L1A2E;
    case 0x1A32: goto L1A32;
    case 0x1A36: goto L1A36;
    case 0x1A3A: goto L1A3A;
    case 0x1A3E: goto L1A3E;
    case 0x1A42: goto L1A42;
    case 0x1A46: goto L1A46;
    case 0x1A4A: goto L1A4A;
    case 0x1A4D: goto L1A4D;
    case 0x1A4E: goto L1A4E;
    case 0x1A4F: goto L1A4F;
    case 0x1A50: goto L1A50;
    case 0x1A51: goto L1A51;
    case 0x1A52: goto L1A52;
    case 0x1A58: goto L1A58;
    case 0x1A5C: goto L1A5C;
    case 0x1A60: goto L1A60;
    case 0x1A64: goto L1A64;
    case 0x1A68: goto L1A68;
    case 0x1A6C: goto L1A6C;
    case 0x1A70: goto L1A70;
    case 0x1A74: goto L1A74;
    case 0x1A78: goto L1A78;
    case 0x1A7C: goto L1A7C;
    case 0x1A80: goto L1A80;
    case 0x1A84: goto L1A84;
    case 0x1A88: goto L1A88;
    case 0x1A89: goto L1A89;
    case 0x1A8A: goto L1A8A;
    case 0x1A8B: goto L1A8B;
    case 0x1A8C: goto L1A8C;
    case 0x1A8D: goto L1A8D;
    case 0x1A93: goto L1A93;
    case 0x1A94: goto L1A94;
    case 0x1A96: goto L1A96;
    case 0x1A98: goto L1A98;
    case 0x1A9B: goto L1A9B;
    case 0x1A9D: goto L1A9D;
    case 0x1AA0: goto L1AA0;
    case 0x1AA4: goto L1AA4;
    case 0x1AA7: goto L1AA7;
    case 0x1AAB: goto L1AAB;
    case 0x1AAE: goto L1AAE;
    case 0x1AB0: goto L1AB0;
    case 0x1AB2: goto L1AB2;
    case 0x1AB4: goto L1AB4;
    case 0x1AB6: goto L1AB6;
    case 0x1AB8: goto L1AB8;
    case 0x1ABA: goto L1ABA;
    case 0x1ABC: goto L1ABC;
    case 0x1ABE: goto L1ABE;
    case 0x1AC0: goto L1AC0;
    case 0x1AC2: goto L1AC2;
    case 0x1AC4: goto L1AC4;
    case 0x1AC5: goto L1AC5;
    case 0x1AC7: goto L1AC7;
    case 0x1AC9: goto L1AC9;
    case 0x1ACC: goto L1ACC;
    case 0x1ACF: goto L1ACF;
    case 0x1AD1: goto L1AD1;
    case 0x1AD3: goto L1AD3;
    case 0x1AD6: goto L1AD6;
    case 0x1AD8: goto L1AD8;
    case 0x1ADA: goto L1ADA;
    case 0x1ADD: goto L1ADD;
    case 0x1AE0: goto L1AE0;
    case 0x1AE3: goto L1AE3;
    case 0x1AE6: goto L1AE6;
    case 0x1AEA: goto L1AEA;
    case 0x1AED: goto L1AED;
    case 0x1AF1: goto L1AF1;
    case 0x1AF4: goto L1AF4;
    case 0x1AF6: goto L1AF6;
    case 0x1AF8: goto L1AF8;
    case 0x1AFA: goto L1AFA;
    case 0x1AFC: goto L1AFC;
    case 0x1AFE: goto L1AFE;
    case 0x1B00: goto L1B00;
    case 0x1B02: goto L1B02;
    case 0x1B04: goto L1B04;
    case 0x1B06: goto L1B06;
    case 0x1B08: goto L1B08;
    case 0x1B0A: goto L1B0A;
    case 0x1B0B: goto L1B0B;
    case 0x1B0D: goto L1B0D;
    case 0x1B0F: goto L1B0F;
    case 0x1B12: goto L1B12;
    case 0x1B15: goto L1B15;
    case 0x1B17: goto L1B17;
    case 0x1B19: goto L1B19;
    case 0x1B1C: goto L1B1C;
    case 0x1B1E: goto L1B1E;
    case 0x1B21: goto L1B21;
    case 0x1B23: goto L1B23;
    case 0x1B26: goto L1B26;
    case 0x1B29: goto L1B29;
    case 0x1B2C: goto L1B2C;
    case 0x1B30: goto L1B30;
    case 0x1B33: goto L1B33;
    case 0x1B37: goto L1B37;
    case 0x1B3A: goto L1B3A;
    case 0x1B3C: goto L1B3C;
    case 0x1B3E: goto L1B3E;
    case 0x1B40: goto L1B40;
    case 0x1B42: goto L1B42;
    case 0x1B44: goto L1B44;
    case 0x1B46: goto L1B46;
    case 0x1B48: goto L1B48;
    case 0x1B4A: goto L1B4A;
    case 0x1B4C: goto L1B4C;
    case 0x1B4E: goto L1B4E;
    case 0x1B50: goto L1B50;
    case 0x1B51: goto L1B51;
    case 0x1B53: goto L1B53;
    case 0x1B55: goto L1B55;
    case 0x1B58: goto L1B58;
    case 0x1B5A: goto L1B5A;
    case 0x1B5C: goto L1B5C;
    case 0x1B5F: goto L1B5F;
    case 0x1B61: goto L1B61;
    case 0x1B63: goto L1B63;
    case 0x1B66: goto L1B66;
    case 0x1B67: goto L1B67;
    case 0x1B6A: goto L1B6A;
    case 0x1B6C: goto L1B6C;
    case 0x1B70: goto L1B70;
    case 0x1B72: goto L1B72;
    case 0x1B78: goto L1B78;
    case 0x1B79: goto L1B79;
    case 0x1B7B: goto L1B7B;
    case 0x1B7E: goto L1B7E;
    case 0x1B80: goto L1B80;
    case 0x1B82: goto L1B82;
    case 0x1B84: goto L1B84;
    case 0x1B85: goto L1B85;
    case 0x1B87: goto L1B87;
    case 0x1B88: goto L1B88;
    case 0x1B8A: goto L1B8A;
    case 0x1B8B: goto L1B8B;
    case 0x1B8E: goto L1B8E;
    case 0x1B90: goto L1B90;
    case 0x1B92: goto L1B92;
    case 0x1B95: goto L1B95;
    case 0x1B97: goto L1B97;
    case 0x1B99: goto L1B99;
    case 0x1B9B: goto L1B9B;
    case 0x1B9C: goto L1B9C;
    case 0x1B9E: goto L1B9E;
    case 0x1B9F: goto L1B9F;
    case 0x1BA1: goto L1BA1;
    case 0x1BA3: goto L1BA3;
    case 0x1BA4: goto L1BA4;
    case 0x1BA8: goto L1BA8;
    case 0x1BAC: goto L1BAC;
    case 0x1BAF: goto L1BAF;
    case 0x1BB2: goto L1BB2;
    case 0x1BB4: goto L1BB4;
    case 0x1BB7: goto L1BB7;
    case 0x1BBA: goto L1BBA;
    case 0x1BBC: goto L1BBC;
    case 0x1BBF: goto L1BBF;
    case 0x1BC0: goto L1BC0;
    case 0x1BC1: goto L1BC1;
    case 0x1BC2: goto L1BC2;
    case 0x1BC6: goto L1BC6;
    case 0x1BC7: goto L1BC7;
    case 0x1BC8: goto L1BC8;
    case 0x1BC9: goto L1BC9;
    case 0x1BCD: goto L1BCD;
    case 0x1BD3: goto L1BD3;
    case 0x1BD6: goto L1BD6;
    case 0x1BD7: goto L1BD7;
    case 0x1BD8: goto L1BD8;
    case 0x1BDC: goto L1BDC;
    case 0x1BDD: goto L1BDD;
    case 0x1BDE: goto L1BDE;
    case 0x1BDF: goto L1BDF;
    case 0x1BE0: goto L1BE0;
    case 0x1BE4: goto L1BE4;
    case 0x1BEA: goto L1BEA;
    case 0x1BEB: goto L1BEB;
    case 0x1BED: goto L1BED;
    case 0x1BEF: goto L1BEF;
    case 0x1BF0: goto L1BF0;
    case 0x1BF3: goto L1BF3;
    case 0x1BF5: goto L1BF5;
    case 0x1BF6: goto L1BF6;
    case 0x1BF8: goto L1BF8;
    case 0x1BF9: goto L1BF9;
    case 0x1BFD: goto L1BFD;
    case 0x1C00: goto L1C00;
    case 0x1C01: goto L1C01;
    case 0x1C02: goto L1C02;
    case 0x1C03: goto L1C03;
    case 0x1C07: goto L1C07;
    case 0x1C0B: goto L1C0B;
    case 0x1C0D: goto L1C0D;
    case 0x1C0F: goto L1C0F;
    case 0x1C11: goto L1C11;
    case 0x1C13: goto L1C13;
    case 0x1C15: goto L1C15;
    case 0x1C17: goto L1C17;
    case 0x1C19: goto L1C19;
    case 0x1C1B: goto L1C1B;
    case 0x1C1D: goto L1C1D;
    case 0x1C1F: goto L1C1F;
    case 0x1C21: goto L1C21;
    case 0x1C22: goto L1C22;
    case 0x1C24: goto L1C24;
    case 0x1C26: goto L1C26;
    case 0x1C29: goto L1C29;
    case 0x1C2B: goto L1C2B;
    case 0x1C2E: goto L1C2E;
    case 0x1C2F: goto L1C2F;
    case 0x1C31: goto L1C31;
    case 0x1C33: goto L1C33;
    case 0x1C35: goto L1C35;
    case 0x1C37: goto L1C37;
    case 0x1C3A: goto L1C3A;
    case 0x1C3B: goto L1C3B;
    case 0x1C3D: goto L1C3D;
    case 0x1C3E: goto L1C3E;
    case 0x1C42: goto L1C42;
    case 0x1C45: goto L1C45;
    case 0x1C46: goto L1C46;
    case 0x1C47: goto L1C47;
    case 0x1C48: goto L1C48;
    case 0x1C4C: goto L1C4C;
    case 0x1C50: goto L1C50;
    case 0x1C54: goto L1C54;
    case 0x1C56: goto L1C56;
    case 0x1C58: goto L1C58;
    case 0x1C5A: goto L1C5A;
    case 0x1C5C: goto L1C5C;
    case 0x1C5E: goto L1C5E;
    case 0x1C60: goto L1C60;
    case 0x1C62: goto L1C62;
    case 0x1C64: goto L1C64;
    case 0x1C66: goto L1C66;
    case 0x1C68: goto L1C68;
    case 0x1C6A: goto L1C6A;
    case 0x1C6C: goto L1C6C;
    case 0x1C6D: goto L1C6D;
    case 0x1C6F: goto L1C6F;
    case 0x1C71: goto L1C71;
    case 0x1C74: goto L1C74;
    case 0x1C76: goto L1C76;
    case 0x1C79: goto L1C79;
    case 0x1C7A: goto L1C7A;
    case 0x1C7C: goto L1C7C;
    case 0x1C7E: goto L1C7E;
    case 0x1C80: goto L1C80;
    case 0x1C82: goto L1C82;
    case 0x1C85: goto L1C85;
    case 0x1C87: goto L1C87;
    case 0x1C88: goto L1C88;
    case 0x1C8A: goto L1C8A;
    case 0x1C8B: goto L1C8B;
    case 0x1C8F: goto L1C8F;
    case 0x1C92: goto L1C92;
    case 0x1C93: goto L1C93;
    case 0x1C94: goto L1C94;
    case 0x1C95: goto L1C95;
    case 0x1C99: goto L1C99;
    case 0x1C9D: goto L1C9D;
    case 0x1CA1: goto L1CA1;
    case 0x1CA3: goto L1CA3;
    case 0x1CA5: goto L1CA5;
    case 0x1CA7: goto L1CA7;
    case 0x1CA9: goto L1CA9;
    case 0x1CAB: goto L1CAB;
    case 0x1CAD: goto L1CAD;
    case 0x1CAF: goto L1CAF;
    case 0x1CB1: goto L1CB1;
    case 0x1CB3: goto L1CB3;
    case 0x1CB5: goto L1CB5;
    case 0x1CB7: goto L1CB7;
    case 0x1CB9: goto L1CB9;
    case 0x1CBA: goto L1CBA;
    case 0x1CBC: goto L1CBC;
    case 0x1CBE: goto L1CBE;
    case 0x1CC0: goto L1CC0;
    case 0x1CC3: goto L1CC3;
    case 0x1CC4: goto L1CC4;
    case 0x1CC6: goto L1CC6;
    case 0x1CC8: goto L1CC8;
    case 0x1CCA: goto L1CCA;
    case 0x1CCC: goto L1CCC;
    case 0x1CCE: goto L1CCE;
    case 0x1CCF: goto L1CCF;
    case 0x1CD0: goto L1CD0;
    case 0x1CD1: goto L1CD1;
    case 0x1CD2: goto L1CD2;
    case 0x1CD4: goto L1CD4;
    case 0x1CD8: goto L1CD8;
    case 0x1CDA: goto L1CDA;
    case 0x1CE0: goto L1CE0;
    case 0x1CE2: goto L1CE2;
    case 0x1CE5: goto L1CE5;
    case 0x1CE7: goto L1CE7;
    case 0x1CE9: goto L1CE9;
    case 0x1CEB: goto L1CEB;
    case 0x1CEC: goto L1CEC;
    case 0x1CEE: goto L1CEE;
    case 0x1CEF: goto L1CEF;
    case 0x1CF1: goto L1CF1;
    case 0x1CF4: goto L1CF4;
    case 0x1CF6: goto L1CF6;
    case 0x1CF8: goto L1CF8;
    case 0x1CFB: goto L1CFB;
    case 0x1CFD: goto L1CFD;
    case 0x1CFF: goto L1CFF;
    case 0x1D01: goto L1D01;
    case 0x1D02: goto L1D02;
    case 0x1D04: goto L1D04;
    case 0x1D05: goto L1D05;
    case 0x1D07: goto L1D07;
    case 0x1D09: goto L1D09;
    case 0x1D0D: goto L1D0D;
    case 0x1D11: goto L1D11;
    case 0x1D14: goto L1D14;
    case 0x1D17: goto L1D17;
    case 0x1D18: goto L1D18;
    case 0x1D1A: goto L1D1A;
    case 0x1D1D: goto L1D1D;
    case 0x1D20: goto L1D20;
    case 0x1D21: goto L1D21;
    case 0x1D24: goto L1D24;
    case 0x1D25: goto L1D25;
    case 0x1D26: goto L1D26;
    case 0x1D27: goto L1D27;
    case 0x1D2B: goto L1D2B;
    case 0x1D31: goto L1D31;
    case 0x1D33: goto L1D33;
    case 0x1D34: goto L1D34;
    case 0x1D35: goto L1D35;
    case 0x1D39: goto L1D39;
    case 0x1D3A: goto L1D3A;
    case 0x1D3B: goto L1D3B;
    case 0x1D3D: goto L1D3D;
    case 0x1D3E: goto L1D3E;
    case 0x1D3F: goto L1D3F;
    case 0x1D43: goto L1D43;
    case 0x1D45: goto L1D45;
    case 0x1D46: goto L1D46;
    case 0x1D4A: goto L1D4A;
    case 0x1D4B: goto L1D4B;
    case 0x1D4C: goto L1D4C;
    case 0x1D4D: goto L1D4D;
    case 0x1D51: goto L1D51;
    case 0x1D52: goto L1D52;
    case 0x1D54: goto L1D54;
    case 0x1D56: goto L1D56;
    case 0x1D59: goto L1D59;
    case 0x1D5C: goto L1D5C;
    case 0x1D5E: goto L1D5E;
    case 0x1D60: goto L1D60;
    case 0x1D62: goto L1D62;
    case 0x1D63: goto L1D63;
    case 0x1D67: goto L1D67;
    case 0x1D68: goto L1D68;
    case 0x1D69: goto L1D69;
    case 0x1D6A: goto L1D6A;
    case 0x1D6E: goto L1D6E;
    case 0x1D6F: goto L1D6F;
    case 0x1D71: goto L1D71;
    case 0x1D73: goto L1D73;
    case 0x1D76: goto L1D76;
    case 0x1D78: goto L1D78;
    case 0x1D7A: goto L1D7A;
    case 0x1D7C: goto L1D7C;
    case 0x1D7D: goto L1D7D;
    case 0x1D81: goto L1D81;
    case 0x1D82: goto L1D82;
    case 0x1D83: goto L1D83;
    case 0x1D84: goto L1D84;
    case 0x1D88: goto L1D88;
    case 0x1D89: goto L1D89;
    case 0x1D8B: goto L1D8B;
    case 0x1D8D: goto L1D8D;
    case 0x1D90: goto L1D90;
    case 0x1D92: goto L1D92;
    case 0x1D94: goto L1D94;
    case 0x1D96: goto L1D96;
    case 0x1D98: goto L1D98;
    case 0x1D9B: goto L1D9B;
    case 0x1D9D: goto L1D9D;
    case 0x1D9F: goto L1D9F;
    case 0x1DA1: goto L1DA1;
    case 0x1DA2: goto L1DA2;
    case 0x1DA4: goto L1DA4;
    case 0x1DA5: goto L1DA5;
    case 0x1DA7: goto L1DA7;
    case 0x1DA9: goto L1DA9;
    case 0x1DAD: goto L1DAD;
    case 0x1DAF: goto L1DAF;
    case 0x1DB3: goto L1DB3;
    case 0x1DB5: goto L1DB5;
    case 0x1DB8: goto L1DB8;
    case 0x1DBA: goto L1DBA;
    case 0x1DBC: goto L1DBC;
    case 0x1DBF: goto L1DBF;
    case 0x1DC0: goto L1DC0;
    case 0x1DC2: goto L1DC2;
    case 0x1DC6: goto L1DC6;
    case 0x1DCA: goto L1DCA;
    case 0x1DCE: goto L1DCE;
    case 0x1DD1: goto L1DD1;
    case 0x1DD5: goto L1DD5;
    case 0x1DD6: goto L1DD6;
    case 0x1DD7: goto L1DD7;
    case 0x1DDB: goto L1DDB;
    case 0x1DDD: goto L1DDD;
    case 0x1DDE: goto L1DDE;
    case 0x1DE2: goto L1DE2;
    case 0x1DE3: goto L1DE3;
    case 0x1DE7: goto L1DE7;
    case 0x1DE8: goto L1DE8;
    case 0x1DEA: goto L1DEA;
    case 0x1DEC: goto L1DEC;
    case 0x1DED: goto L1DED;
    case 0x1DF1: goto L1DF1;
    case 0x1DF2: goto L1DF2;
    case 0x1DF6: goto L1DF6;
    case 0x1DF7: goto L1DF7;
    case 0x1DF9: goto L1DF9;
    case 0x1DFB: goto L1DFB;
    case 0x1DFC: goto L1DFC;
    case 0x1E00: goto L1E00;
    case 0x1E01: goto L1E01;
    case 0x1E05: goto L1E05;
    case 0x1E06: goto L1E06;
    case 0x1E08: goto L1E08;
    case 0x1E0A: goto L1E0A;
    case 0x1E0C: goto L1E0C;
    case 0x1E0F: goto L1E0F;
    case 0x1E11: goto L1E11;
    case 0x1E13: goto L1E13;
    case 0x1E15: goto L1E15;
    case 0x1E16: goto L1E16;
    case 0x1E18: goto L1E18;
    case 0x1E19: goto L1E19;
    case 0x1E1B: goto L1E1B;
    case 0x1E1D: goto L1E1D;
    case 0x1E1F: goto L1E1F;
    case 0x1E21: goto L1E21;
    case 0x1E23: goto L1E23;
    case 0x1E25: goto L1E25;
    case 0x1E26: goto L1E26;
    case 0x1E2A: goto L1E2A;
    case 0x1E2B: goto L1E2B;
    case 0x1E2C: goto L1E2C;
    case 0x1E30: goto L1E30;
    case 0x1E32: goto L1E32;
    case 0x1E34: goto L1E34;
    case 0x1E36: goto L1E36;
    case 0x1E38: goto L1E38;
    case 0x1E3C: goto L1E3C;
    case 0x1E3D: goto L1E3D;
    case 0x1E41: goto L1E41;
    case 0x1E42: goto L1E42;
    case 0x1E43: goto L1E43;
    case 0x1E47: goto L1E47;
    case 0x1E49: goto L1E49;
    case 0x1E4B: goto L1E4B;
    case 0x1E4D: goto L1E4D;
    case 0x1E4F: goto L1E4F;
    case 0x1E53: goto L1E53;
    case 0x1E54: goto L1E54;
    case 0x1E58: goto L1E58;
    case 0x1E59: goto L1E59;
    case 0x1E5A: goto L1E5A;
    case 0x1E5E: goto L1E5E;
    case 0x1E60: goto L1E60;
    case 0x1E62: goto L1E62;
    case 0x1E64: goto L1E64;
    case 0x1E66: goto L1E66;
    case 0x1E6A: goto L1E6A;
    case 0x1E6E: goto L1E6E;
    case 0x1E72: goto L1E72;
    case 0x1E76: goto L1E76;
    case 0x1E79: goto L1E79;
    case 0x1E7A: goto L1E7A;
    case 0x1E7E: goto L1E7E;
    case 0x1E80: goto L1E80;
    case 0x1E82: goto L1E82;
    case 0x1E84: goto L1E84;
    case 0x1E85: goto L1E85;
    case 0x1E89: goto L1E89;
    case 0x1E8B: goto L1E8B;
    case 0x1E8E: goto L1E8E;
    case 0x1E91: goto L1E91;
    case 0x1E94: goto L1E94;
    case 0x1E97: goto L1E97;
    case 0x1E9A: goto L1E9A;
    case 0x1E9D: goto L1E9D;
    case 0x1EA0: goto L1EA0;
    case 0x1EA3: goto L1EA3;
    case 0x1EA7: goto L1EA7;
    case 0x1EA9: goto L1EA9;
    case 0x1EAB: goto L1EAB;
    case 0x1EAD: goto L1EAD;
    case 0x1EAF: goto L1EAF;
    case 0x1EB1: goto L1EB1;
    case 0x1EB3: goto L1EB3;
    case 0x1EB5: goto L1EB5;
    case 0x1EB7: goto L1EB7;
    case 0x1EB9: goto L1EB9;
    case 0x1EBB: goto L1EBB;
    case 0x1EBD: goto L1EBD;
    case 0x1EBF: goto L1EBF;
    case 0x1EC0: goto L1EC0;
    case 0x1EC2: goto L1EC2;
    case 0x1EC4: goto L1EC4;
    case 0x1EC7: goto L1EC7;
    case 0x1ECA: goto L1ECA;
    case 0x1ECD: goto L1ECD;
    case 0x1ED1: goto L1ED1;
    case 0x1ED3: goto L1ED3;
    case 0x1ED5: goto L1ED5;
    case 0x1ED7: goto L1ED7;
    case 0x1ED9: goto L1ED9;
    case 0x1EDB: goto L1EDB;
    case 0x1EDD: goto L1EDD;
    case 0x1EDF: goto L1EDF;
    case 0x1EE1: goto L1EE1;
    case 0x1EE3: goto L1EE3;
    case 0x1EE5: goto L1EE5;
    case 0x1EE7: goto L1EE7;
    case 0x1EE9: goto L1EE9;
    case 0x1EEA: goto L1EEA;
    case 0x1EEC: goto L1EEC;
    case 0x1EEE: goto L1EEE;
    case 0x1EF1: goto L1EF1;
    case 0x1EF4: goto L1EF4;
    case 0x1EF7: goto L1EF7;
    case 0x1EFB: goto L1EFB;
    case 0x1EFD: goto L1EFD;
    case 0x1EFF: goto L1EFF;
    case 0x1F01: goto L1F01;
    case 0x1F03: goto L1F03;
    case 0x1F05: goto L1F05;
    case 0x1F07: goto L1F07;
    case 0x1F09: goto L1F09;
    case 0x1F0B: goto L1F0B;
    case 0x1F0D: goto L1F0D;
    case 0x1F0F: goto L1F0F;
    case 0x1F11: goto L1F11;
    case 0x1F13: goto L1F13;
    case 0x1F14: goto L1F14;
    case 0x1F16: goto L1F16;
    case 0x1F18: goto L1F18;
    case 0x1F1B: goto L1F1B;
    case 0x1F1E: goto L1F1E;
    case 0x1F23: goto L1F23;
    case 0x1F25: goto L1F25;
    case 0x1F26: goto L1F26;
    case 0x1F2A: goto L1F2A;
    case 0x1F2D: goto L1F2D;
    case 0x1F32: goto L1F32;
    case 0x1F34: goto L1F34;
    case 0x1F37: goto L1F37;
    case 0x1F3A: goto L1F3A;
    case 0x1F3C: goto L1F3C;
    case 0x1F3F: goto L1F3F;
    case 0x1F41: goto L1F41;
    case 0x1F43: goto L1F43;
    case 0x1F44: goto L1F44;
    case 0x1F47: goto L1F47;
    case 0x1F48: goto L1F48;
    case 0x1F4B: goto L1F4B;
    case 0x1F4D: goto L1F4D;
    case 0x1F4F: goto L1F4F;
    case 0x1F50: goto L1F50;
    case 0x1F53: goto L1F53;
    case 0x1F54: goto L1F54;
    case 0x1F57: goto L1F57;
    case 0x1F59: goto L1F59;
    case 0x1F5B: goto L1F5B;
    case 0x1F5E: goto L1F5E;
    case 0x1F5F: goto L1F5F;
    case 0x1F62: goto L1F62;
    case 0x1F63: goto L1F63;
    case 0x1F65: goto L1F65;
    case 0x1F67: goto L1F67;
    case 0x1F6A: goto L1F6A;
    case 0x1F6C: goto L1F6C;
    case 0x1F6E: goto L1F6E;
    case 0x1F71: goto L1F71;
    case 0x1F72: goto L1F72;
    case 0x1F74: goto L1F74;
    case 0x1F76: goto L1F76;
    case 0x1F79: goto L1F79;
    case 0x1F7B: goto L1F7B;
    case 0x1F7D: goto L1F7D;
    case 0x1F80: goto L1F80;
    case 0x1F81: goto L1F81;
    case 0x1F83: goto L1F83;
    case 0x1F85: goto L1F85;
    case 0x1F88: goto L1F88;
    case 0x1F8A: goto L1F8A;
    case 0x1F8C: goto L1F8C;
    case 0x1F8D: goto L1F8D;
    case 0x1F8F: goto L1F8F;
    case 0x1F92: goto L1F92;
    case 0x1F94: goto L1F94;
    case 0x1F96: goto L1F96;
    case 0x1F98: goto L1F98;
    case 0x1F99: goto L1F99;
    case 0x1F9B: goto L1F9B;
    case 0x1F9C: goto L1F9C;
    case 0x1F9E: goto L1F9E;
    case 0x1FA1: goto L1FA1;
    case 0x1FA4: goto L1FA4;
    case 0x1FA5: goto L1FA5;
    case 0x1FA6: goto L1FA6;
    case 0x1FAA: goto L1FAA;
    case 0x1FAD: goto L1FAD;
    case 0x1FB0: goto L1FB0;
    case 0x1FB3: goto L1FB3;
    case 0x1FB6: goto L1FB6;
    case 0x1FB9: goto L1FB9;
    case 0x1FBC: goto L1FBC;
    case 0x1FC0: goto L1FC0;
    case 0x1FC1: goto L1FC1;
    case 0x1FC2: goto L1FC2;
    default: asm_bad_entry("SPHERE.ASM", entry);
    }

    /* seg004_1370  (+1370)
       Bounding-sphere test against the view frustum. In: BX, AX, BP = the object origin words
       (2886h, 2888h, 288Ah: negated eye-relative x, y, z), SI = radius, CL = shift. Shifts all
       four by CL and negates the position, takes its depth along the matrix's z column (kept at
       ds:0Eh, the shift at ds:10h), and tests it against the near plane and the four side planes
       (the x and y columns, with the radius scaled by 2728h and 272Ah). Out: CF set when the
       sphere is wholly outside one plane. Otherwise CF clear, SI = -1 when the sphere is wholly
       inside (the first path) or 0 when it crosses a plane (the second, from L147F), and AX =
       the depth shifted back by the original shift (scaled by 8000h / -24E6h when 2726h is
       negative, 7FFFh if that overflows) and halved; 0 when the centre is behind the eye. Called
       only by do_obj. */
L1370: /* _sphere_check */
    /* 1370  mov     word ptr ds:[1D0h],cx */
    ww(pDS, 0x1D0, CX);
L1374:
    /* 1374  shl     si,cl */
    SI = shl16(SI, CL);
L1376:
    /* 1376  shl     bx,cl */
    BX = (uint16_t)(BX << (CL & 31));
L1378:
    /* 1378  neg     bx */
    BX = neg16(BX);
L137A:
    /* 137A  shl     bp,cl */
    BP = (uint16_t)(BP << (CL & 31));
L137C:
    /* 137C  neg     bp */
    BP = neg16(BP);
L137E:
    /* 137E  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1380:
    /* 1380  neg     ax */
    AX = (uint16_t)-AX;
L1382:
    /* 1382  mov     cx,ax */
    CX = AX;
L1384:
    /* 1384  mov     ax,bx */
    AX = BX;
L1386:
    /* 1386  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L138A:
    /* 138A  mov     di,dx */
    DI = DX;
L138C:
    /* 138C  mov     ax,cx */
    AX = CX;
L138E:
    /* 138E  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L1392:
    /* 1392  add     di,dx */
    DI = (uint16_t)(DI + DX);
L1394:
    /* 1394  mov     ax,bp */
    AX = BP;
L1396:
    /* 1396  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L139A:
    /* 139A  add     di,dx */
    DI = add16(DI, DX, 0);
L139C:
    /* 139C  mov     word ptr ds:[1CEh],di */
    ww(pDS, 0x1CE, DI);
L13A0:
    /* 13A0  jns     short L13AC */
    if (!SF) goto L13AC;
L13A2:
    /* 13A2  add     di,si */
    DI = add16(DI, SI, 0);
L13A4:
    /* 13A4  jns     short L13A9 */
    if (!SF) goto L13A9;
L13A6:
    /* 13A6  jmp     L151D */
    goto L151D;
L13A9: /* L13A9 */
    /* 13A9  jmp     L147F */
    goto L147F;
L13AC: /* L13AC */
    /* 13AC  mov     ax,bx */
    AX = BX;
L13AE:
    /* 13AE  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L13B2:
    /* 13B2  mov     di,dx */
    DI = DX;
L13B4:
    /* 13B4  mov     ax,cx */
    AX = CX;
L13B6:
    /* 13B6  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L13BA:
    /* 13BA  add     di,dx */
    DI = (uint16_t)(DI + DX);
L13BC:
    /* 13BC  mov     ax,bp */
    AX = BP;
L13BE:
    /* 13BE  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L13C2:
    /* 13C2  add     di,dx */
    DI = (uint16_t)(DI + DX);
L13C4:
    /* 13C4  mov     ax,si */
    AX = SI;
L13C6:
    /* 13C6  mul     word ptr ds:[2728h] */
    mul16(rw(pDS, 0x2728));
L13CA:
    /* 13CA  mov     ax,word ptr ds:[1CEh] */
    AX = rw(pDS, 0x1CE);
L13CD:
    /* 13CD  sub     ax,di */
    AX = sub16(AX, DI, 0);
L13CF:
    /* 13CF  jns     short L13DB */
    if (!SF) goto L13DB;
L13D1:
    /* 13D1  add     ax,dx */
    AX = add16(AX, DX, 0);
L13D3:
    /* 13D3  jns     short L13D8 */
    if (!SF) goto L13D8;
L13D5:
    /* 13D5  jmp     L151D */
    goto L151D;
L13D8: /* L13D8 */
    /* 13D8  jmp     L14A8 */
    goto L14A8;
L13DB: /* L13DB */
    /* 13DB  sub     ax,dx */
    AX = sub16(AX, DX, 0);
L13DD:
    /* 13DD  jns     short L13E2 */
    if (!SF) goto L13E2;
L13DF:
    /* 13DF  jmp     L14A8 */
    goto L14A8;
L13E2: /* L13E2 */
    /* 13E2  add     di,word ptr ds:[1CEh] */
    DI = add16(DI, rw(pDS, 0x1CE), 0);
L13E6:
    /* 13E6  jns     short L13F2 */
    if (!SF) goto L13F2;
L13E8:
    /* 13E8  add     di,dx */
    DI = add16(DI, DX, 0);
L13EA:
    /* 13EA  jns     short L13EF */
    if (!SF) goto L13EF;
L13EC:
    /* 13EC  jmp     L151D */
    goto L151D;
L13EF: /* L13EF */
    /* 13EF  jmp     L14B2 */
    goto L14B2;
L13F2: /* L13F2 */
    /* 13F2  sub     di,dx */
    DI = sub16(DI, DX, 0);
L13F4:
    /* 13F4  jns     short L13F9 */
    if (!SF) goto L13F9;
L13F6:
    /* 13F6  jmp     L14B2 */
    goto L14B2;
L13F9: /* L13F9 */
    /* 13F9  mov     ax,bx */
    AX = BX;
L13FB:
    /* 13FB  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L13FF:
    /* 13FF  mov     di,dx */
    DI = DX;
L1401:
    /* 1401  mov     ax,cx */
    AX = CX;
L1403:
    /* 1403  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L1407:
    /* 1407  add     di,dx */
    DI = (uint16_t)(DI + DX);
L1409:
    /* 1409  mov     ax,bp */
    AX = BP;
L140B:
    /* 140B  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L140F:
    /* 140F  add     di,dx */
    DI = (uint16_t)(DI + DX);
L1411:
    /* 1411  mov     ax,si */
    AX = SI;
L1413:
    /* 1413  mul     word ptr ds:[272Ah] */
    mul16(rw(pDS, 0x272A));
L1417:
    /* 1417  mov     ax,word ptr ds:[1CEh] */
    AX = rw(pDS, 0x1CE);
L141A:
    /* 141A  sub     ax,di */
    AX = sub16(AX, DI, 0);
L141C:
    /* 141C  jns     short L1428 */
    if (!SF) goto L1428;
L141E:
    /* 141E  add     ax,dx */
    AX = add16(AX, DX, 0);
L1420:
    /* 1420  jns     short L1425 */
    if (!SF) goto L1425;
L1422:
    /* 1422  jmp     L151D */
    goto L151D;
L1425: /* L1425 */
    /* 1425  jmp     L14DB */
    goto L14DB;
L1428: /* L1428 */
    /* 1428  sub     ax,dx */
    AX = sub16(AX, DX, 0);
L142A:
    /* 142A  jns     short L142F */
    if (!SF) goto L142F;
L142C:
    /* 142C  jmp     L14DB */
    goto L14DB;
L142F: /* L142F */
    /* 142F  add     di,word ptr ds:[1CEh] */
    DI = add16(DI, rw(pDS, 0x1CE), 0);
L1433:
    /* 1433  jns     short L143F */
    if (!SF) goto L143F;
L1435:
    /* 1435  add     di,dx */
    DI = add16(DI, DX, 0);
L1437:
    /* 1437  jns     short L143C */
    if (!SF) goto L143C;
L1439:
    /* 1439  jmp     L151D */
    goto L151D;
L143C: /* L143C */
    /* 143C  jmp     L14E5 */
    goto L14E5;
L143F: /* L143F */
    /* 143F  sub     di,dx */
    DI = sub16(DI, DX, 0);
L1441:
    /* 1441  jns     short L1446 */
    if (!SF) goto L1446;
L1443:
    /* 1443  jmp     L14E5 */
    goto L14E5;
L1446: /* L1446 */
    /* 1446  mov     ax,word ptr ds:[1CEh] */
    AX = rw(pDS, 0x1CE);
L1449:
    /* 1449  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L144B:
    /* 144B  jns     short L1451 */
    if (!SF) goto L1451;
L144D:
    /* 144D  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L144F:
    /* 144F  jmp     short L1478 */
    goto L1478;
L1451: /* L1451 */
    /* 1451  mov     cx,word ptr ds:[1D0h] */
    CX = rw(pDS, 0x1D0);
L1455:
    /* 1455  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L1457:
    /* 1457  test    word ptr ds:[2726h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x2726) & 0xFFFF));
L145D:
    /* 145D  jns     short L1478 */
    if (!SF) goto L1478;
L145F:
    /* 145F  mov     cx,word ptr ds:[2726h] */
    CX = rw(pDS, 0x2726);
L1463:
    /* 1463  neg     cx */
    CX = (uint16_t)-CX;
L1465:
    /* 1465  mov     dx,ax */
    DX = AX;
L1467:
    /* 1467  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L1469:
    /* 1469  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L146B:
    /* 146B  cmp     dx,cx */
    sub16(DX, CX, 0);
L146D:
    /* 146D  jl      short L1474 */
    if (SF != OF) goto L1474;
L146F:
    /* 146F  mov     ax,7FFFh */
    AX = 0x7FFF;
L1472:
    /* 1472  jmp     short L1478 */
    goto L1478;
L1474: /* L1474 */
    /* 1474  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1476:
    /* 1476  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x06E7, 0x1476, 2)) != 0) return c;
L1478: /* L1478 */
    /* 1478  shr     ax,1 */
    AX = shr16(AX, 1);
L147A:
    /* 147A  clc */
    CF = 0;
L147B:
    /* 147B  mov     si,0FFFFh */
    SI = 0xFFFF;
L147E:
    /* 147E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L147F: /* L147F */
    /* 147F  mov     ax,bx */
    AX = BX;
L1481:
    /* 1481  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L1485:
    /* 1485  mov     di,dx */
    DI = DX;
L1487:
    /* 1487  mov     ax,cx */
    AX = CX;
L1489:
    /* 1489  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L148D:
    /* 148D  add     di,dx */
    DI = (uint16_t)(DI + DX);
L148F:
    /* 148F  mov     ax,bp */
    AX = BP;
L1491:
    /* 1491  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L1495:
    /* 1495  add     di,dx */
    DI = (uint16_t)(DI + DX);
L1497:
    /* 1497  mov     ax,si */
    AX = SI;
L1499:
    /* 1499  mul     word ptr ds:[2728h] */
    mul16(rw(pDS, 0x2728));
L149D:
    /* 149D  mov     ax,word ptr ds:[1CEh] */
    AX = rw(pDS, 0x1CE);
L14A0:
    /* 14A0  sub     ax,di */
    AX = sub16(AX, DI, 0);
L14A2:
    /* 14A2  jns     short L14A8 */
    if (!SF) goto L14A8;
L14A4:
    /* 14A4  add     ax,dx */
    AX = add16(AX, DX, 0);
L14A6:
    /* 14A6  js      short L151D */
    if (SF) goto L151D;
L14A8: /* L14A8 */
    /* 14A8  add     di,word ptr ds:[1CEh] */
    DI = add16(DI, rw(pDS, 0x1CE), 0);
L14AC:
    /* 14AC  jns     short L14B2 */
    if (!SF) goto L14B2;
L14AE:
    /* 14AE  add     di,dx */
    DI = add16(DI, DX, 0);
L14B0:
    /* 14B0  js      short L151D */
    if (SF) goto L151D;
L14B2: /* L14B2 */
    /* 14B2  mov     ax,bx */
    AX = BX;
L14B4:
    /* 14B4  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L14B8:
    /* 14B8  mov     di,dx */
    DI = DX;
L14BA:
    /* 14BA  mov     ax,cx */
    AX = CX;
L14BC:
    /* 14BC  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L14C0:
    /* 14C0  add     di,dx */
    DI = (uint16_t)(DI + DX);
L14C2:
    /* 14C2  mov     ax,bp */
    AX = BP;
L14C4:
    /* 14C4  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L14C8:
    /* 14C8  add     di,dx */
    DI = (uint16_t)(DI + DX);
L14CA:
    /* 14CA  mov     ax,si */
    AX = SI;
L14CC:
    /* 14CC  mul     word ptr ds:[272Ah] */
    mul16(rw(pDS, 0x272A));
L14D0:
    /* 14D0  mov     ax,word ptr ds:[1CEh] */
    AX = rw(pDS, 0x1CE);
L14D3:
    /* 14D3  sub     ax,di */
    AX = sub16(AX, DI, 0);
L14D5:
    /* 14D5  jns     short L14DB */
    if (!SF) goto L14DB;
L14D7:
    /* 14D7  add     ax,dx */
    AX = add16(AX, DX, 0);
L14D9:
    /* 14D9  js      short L151D */
    if (SF) goto L151D;
L14DB: /* L14DB */
    /* 14DB  add     di,word ptr ds:[1CEh] */
    DI = add16(DI, rw(pDS, 0x1CE), 0);
L14DF:
    /* 14DF  jns     short L14E5 */
    if (!SF) goto L14E5;
L14E1:
    /* 14E1  add     di,dx */
    DI = add16(DI, DX, 0);
L14E3:
    /* 14E3  js      short L151D */
    if (SF) goto L151D;
L14E5: /* L14E5 */
    /* 14E5  mov     ax,word ptr ds:[1CEh] */
    AX = rw(pDS, 0x1CE);
L14E8:
    /* 14E8  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L14EA:
    /* 14EA  jns     short L14F0 */
    if (!SF) goto L14F0;
L14EC:
    /* 14EC  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L14EE:
    /* 14EE  jmp     short L1517 */
    goto L1517;
L14F0: /* L14F0 */
    /* 14F0  mov     cx,word ptr ds:[1D0h] */
    CX = rw(pDS, 0x1D0);
L14F4:
    /* 14F4  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L14F6:
    /* 14F6  test    word ptr ds:[2726h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x2726) & 0xFFFF));
L14FC:
    /* 14FC  jns     short L1517 */
    if (!SF) goto L1517;
L14FE:
    /* 14FE  mov     cx,word ptr ds:[2726h] */
    CX = rw(pDS, 0x2726);
L1502:
    /* 1502  neg     cx */
    CX = (uint16_t)-CX;
L1504:
    /* 1504  mov     dx,ax */
    DX = AX;
L1506:
    /* 1506  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L1508:
    /* 1508  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L150A:
    /* 150A  cmp     dx,cx */
    sub16(DX, CX, 0);
L150C:
    /* 150C  jl      short L1513 */
    if (SF != OF) goto L1513;
L150E:
    /* 150E  mov     ax,7FFFh */
    AX = 0x7FFF;
L1511:
    /* 1511  jmp     short L1517 */
    goto L1517;
L1513: /* L1513 */
    /* 1513  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1515:
    /* 1515  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x06E7, 0x1515, 2)) != 0) return c;
L1517: /* L1517 */
    /* 1517  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L1519:
    /* 1519  clc */
    CF = 0;
L151A:
    /* 151A  xor     si,si */
    SI = logic16((uint16_t)(SI ^ SI));
L151C:
    /* 151C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L151D: /* L151D */
    /* 151D  stc */
    CF = 1;
L151E:
    /* 151E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_151F  (+151F)
       Distance estimate from the three 32-bit eye-relative coordinates do_obj leaves at ds:16h,
       1Ah and 1Eh: takes absolute values and combines the largest with fractions of the others
       (halvings and quarterings, an octagonal approximation). Out: BP:BX. In the y term the
       absolute value's correction is added to CX, not SI (`add cx,dx` after `xor si,dx`), which
       looks like a slip. UW2 never calls it; UW1's seg004_15AC and seg004_17AB do, so there the
       slip is live (not checked in play). */
L151F: /* _get_dist */
    /* 151F  push    si */
    push16(SI);
L1520:
    /* 1520  mov     bx,word ptr ds:[1D6h] */
    BX = rw(pDS, 0x1D6);
L1524:
    /* 1524  mov     ax,word ptr ds:[1D8h] */
    AX = rw(pDS, 0x1D8);
L1527:
    /* 1527  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1528:
    /* 1528  xor     bx,dx */
    BX = (uint16_t)(BX ^ DX);
L152A:
    /* 152A  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L152C:
    /* 152C  neg     dx */
    DX = (uint16_t)-DX;
L152E:
    /* 152E  add     bx,dx */
    BX = add16(BX, DX, 0);
L1530:
    /* 1530  adc     ax,0 */
    AX = (uint16_t)(AX + 0x0 + CF);
L1533:
    /* 1533  mov     bp,ax */
    BP = AX;
L1535:
    /* 1535  mov     cx,word ptr ds:[1DEh] */
    CX = rw(pDS, 0x1DE);
L1539:
    /* 1539  mov     ax,word ptr ds:[1E0h] */
    AX = rw(pDS, 0x1E0);
L153C:
    /* 153C  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L153D:
    /* 153D  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L153F:
    /* 153F  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1541:
    /* 1541  neg     dx */
    DX = (uint16_t)-DX;
L1543:
    /* 1543  add     cx,dx */
    CX = add16(CX, DX, 0);
L1545:
    /* 1545  adc     ax,0 */
    AX = (uint16_t)(AX + 0x0 + CF);
L1548:
    /* 1548  cmp     bp,ax */
    sub16(BP, AX, 0);
L154A:
    /* 154A  jl      short L1555 */
    if (SF != OF) goto L1555;
L154C:
    /* 154C  jg      short L1552 */
    if (!ZF && SF == OF) goto L1552;
L154E:
    /* 154E  cmp     bx,cx */
    sub16(BX, CX, 0);
L1550:
    /* 1550  jl      short L1555 */
    if (SF != OF) goto L1555;
L1552: /* L1552 */
    /* 1552  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L1553:
    /* 1553  xchg    bx,cx */
    { uint16_t t_ = CX;
    CX = BX;
    BX = t_; }
L1555: /* L1555 */
    /* 1555  sar     bp,1 */
    BP = sar16(BP, 1);
L1557:
    /* 1557  rcr     bx,1 */
    BX = rcr16(BX, 1);
L1559:
    /* 1559  sar     bp,1 */
    BP = sar16(BP, 1);
L155B:
    /* 155B  rcr     bx,1 */
    BX = rcr16(BX, 1);
L155D:
    /* 155D  mov     di,ax */
    DI = AX;
L155F:
    /* 155F  mov     si,word ptr ds:[1DAh] */
    SI = rw(pDS, 0x1DA);
L1563:
    /* 1563  mov     ax,word ptr ds:[1DCh] */
    AX = rw(pDS, 0x1DC);
L1566:
    /* 1566  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1567:
    /* 1567  xor     si,dx */
    SI = (uint16_t)(SI ^ DX);
L1569:
    /* 1569  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L156B:
    /* 156B  neg     dx */
    DX = (uint16_t)-DX;
L156D:
    /* 156D  add     cx,dx */
    CX = add16(CX, DX, 0);
L156F:
    /* 156F  adc     ax,0 */
    AX = (uint16_t)(AX + 0x0 + CF);
L1572:
    /* 1572  cmp     ax,di */
    sub16(AX, DI, 0);
L1574:
    /* 1574  jl      short L158A */
    if (SF != OF) goto L158A;
L1576:
    /* 1576  jg      short L157C */
    if (!ZF && SF == OF) goto L157C;
L1578:
    /* 1578  cmp     si,cx */
    sub16(SI, CX, 0);
L157A:
    /* 157A  jl      short L158A */
    if (SF != OF) goto L158A;
L157C: /* L157C */
    /* 157C  add     bx,cx */
    BX = add16(BX, CX, 0);
L157E:
    /* 157E  adc     bp,di */
    BP = (uint16_t)(BP + DI + CF);
L1580:
    /* 1580  sar     bp,1 */
    BP = sar16(BP, 1);
L1582:
    /* 1582  rcr     bx,1 */
    BX = rcr16(BX, 1);
L1584:
    /* 1584  sar     bp,1 */
    BP = sar16(BP, 1);
L1586:
    /* 1586  rcr     bx,1 */
    BX = rcr16(BX, 1);
L1588:
    /* 1588  jmp     short L15A6 */
    goto L15A6;
L158A: /* L158A */
    /* 158A  sar     ax,1 */
    AX = sar16(AX, 1);
L158C:
    /* 158C  rcr     si,1 */
    SI = rcr16(SI, 1);
L158E:
    /* 158E  cmp     ax,bp */
    sub16(AX, BP, 0);
L1590:
    /* 1590  jl      short L159E */
    if (SF != OF) goto L159E;
L1592:
    /* 1592  jg      short L1598 */
    if (!ZF && SF == OF) goto L1598;
L1594:
    /* 1594  cmp     si,bx */
    sub16(SI, BX, 0);
L1596:
    /* 1596  jl      short L159E */
    if (SF != OF) goto L159E;
L1598: /* L1598 */
    /* 1598  sar     bp,1 */
    BP = sar16(BP, 1);
L159A:
    /* 159A  rcr     bx,1 */
    BX = rcr16(BX, 1);
L159C:
    /* 159C  jmp     short L15A2 */
    goto L15A2;
L159E: /* L159E */
    /* 159E  sar     ax,1 */
    AX = sar16(AX, 1);
L15A0:
    /* 15A0  rcr     si,1 */
    SI = rcr16(SI, 1);
L15A2: /* L15A2 */
    /* 15A2  add     si,cx */
    SI = add16(SI, CX, 0);
L15A4:
    /* 15A4  adc     ax,di */
    AX = (uint16_t)(AX + DI + CF);
L15A6: /* L15A6 */
    /* 15A6  add     bx,si */
    BX = add16(BX, SI, 0);
L15A8:
    /* 15A8  adc     bp,ax */
    BP = add16(BP, AX, CF);
L15AA:
    /* 15AA  pop     si */
    SI = pop16();
L15AB:
    /* 15AB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_15AC  (+15AC)
       UW1 only: opcode 1Eh, an object header that calls get_dist, self_modify and seg004_5040
       (see the module header); not traced further. */
L15AC: /* _seg004_15AC */
    /* 15AC  lea     ax,[si+0Eh] */
    AX = (uint16_t)(SI + 0xE);
L15AF:
    /* 15AF  push    ax */
    push16(AX);
L15B0:
    /* 15B0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L15B1:
    /* 15B1  mov     di,ax */
    DI = AX;
L15B3:
    /* 15B3  add     di,si */
    DI = (uint16_t)(DI + SI);
L15B5:
    /* 15B5  mov     es,word ptr [di-8] */
    SET_ES(rw(pDS, DI + 0xFFF8));
L15B8:
    /* 15B8  mov     dx,word ptr ds:[EYE_X] */
    DX = rw(pDS, 0x26BA);
L15BC:
    /* 15BC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L15BD:
    /* 15BD  sub     dx,ax */
    DX = sub16(DX, AX, 0);
L15BF:
    /* 15BF  mov     word ptr ds:[1D6h],dx */
    ww(pDS, 0x1D6, DX);
L15C3:
    /* 15C3  mov     bx,word ptr ds:[EYE_X+2] */
    BX = rw(pDS, 0x26BC);
L15C7:
    /* 15C7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L15C8:
    /* 15C8  sbb     bx,ax */
    BX = (uint16_t)(BX - AX - CF);
L15CA:
    /* 15CA  mov     word ptr ds:[1D8h],bx */
    ww(pDS, 0x1D8, BX);
L15CE:
    /* 15CE  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L15CF:
    /* 15CF  mov     cx,es */
    CX = asm_es;
L15D1:
    /* 15D1  jcxz    L15E7 */
    if (!CX) goto L15E7;
L15D3:
    /* 15D3  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L15D5:
    /* 15D5  jns     short L15E1 */
    if (!SF) goto L15E1;
L15D7:
    /* 15D7  neg     cx */
    CX = (uint16_t)-CX;
L15D9: /* L15D9 */
    /* 15D9  shl     ax,1 */
    AX = shl16(AX, 1);
L15DB:
    /* 15DB  rcl     bx,1 */
    BX = rcl16(BX, 1);
L15DD:
    /* 15DD  loop    L15D9 */
    if (--CX) goto L15D9;
L15DF:
    /* 15DF  jmp     short L15E7 */
    goto L15E7;
L15E1: /* L15E1 */
    /* 15E1  sar     bx,1 */
    BX = sar16(BX, 1);
L15E3:
    /* 15E3  rcr     ax,1 */
    AX = rcr16(AX, 1);
L15E5:
    /* 15E5  loop    L15E1 */
    if (--CX) goto L15E1;
L15E7: /* L15E7 */
    /* 15E7  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L15E8:
    /* 15E8  cmp     bx,dx */
    sub16(BX, DX, 0);
L15EA:
    /* 15EA  je      short L15EF */
    if (ZF) goto L15EF;
L15EC:
    /* 15EC  jmp     L16FE */
    goto L16FE;
L15EF: /* L15EF */
    /* 15EF  mov     word ptr ds:[1C8h],ax */
    ww(pDS, 0x1C8, AX);
L15F2:
    /* 15F2  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L15F3:
    /* 15F3  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L15F5:
    /* 15F5  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L15F7:
    /* 15F7  mov     bp,ax */
    BP = AX;
L15F9:
    /* 15F9  mov     dx,word ptr ds:[EYE_Y] */
    DX = rw(pDS, 0x26BE);
L15FD:
    /* 15FD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L15FE:
    /* 15FE  sub     dx,ax */
    DX = sub16(DX, AX, 0);
L1600:
    /* 1600  mov     word ptr ds:[1DAh],dx */
    ww(pDS, 0x1DA, DX);
L1604:
    /* 1604  mov     bx,word ptr ds:[EYE_Y+2] */
    BX = rw(pDS, 0x26C0);
L1608:
    /* 1608  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1609:
    /* 1609  sbb     bx,ax */
    BX = (uint16_t)(BX - AX - CF);
L160B:
    /* 160B  mov     word ptr ds:[1DCh],bx */
    ww(pDS, 0x1DC, BX);
L160F:
    /* 160F  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L1610:
    /* 1610  mov     cx,es */
    CX = asm_es;
L1612:
    /* 1612  jcxz    L1628 */
    if (!CX) goto L1628;
L1614:
    /* 1614  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L1616:
    /* 1616  jns     short L1622 */
    if (!SF) goto L1622;
L1618:
    /* 1618  neg     cx */
    CX = (uint16_t)-CX;
L161A:
    /* 161A  shl     ax,1 */
    AX = shl16(AX, 1);
L161C:
    /* 161C  rcl     bx,1 */
    BX = rcl16(BX, 1);
L161E:
    /* 161E  loop    L15D9 */
    if (--CX) goto L15D9;
L1620:
    /* 1620  jmp     short L1628 */
    goto L1628;
L1622: /* L1622 */
    /* 1622  sar     bx,1 */
    BX = sar16(BX, 1);
L1624:
    /* 1624  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1626:
    /* 1626  loop    L1622 */
    if (--CX) goto L1622;
L1628: /* L1628 */
    /* 1628  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1629:
    /* 1629  cmp     bx,dx */
    sub16(BX, DX, 0);
L162B:
    /* 162B  je      short L1630 */
    if (ZF) goto L1630;
L162D:
    /* 162D  jmp     L16FE */
    goto L16FE;
L1630: /* L1630 */
    /* 1630  mov     word ptr ds:[1CAh],ax */
    ww(pDS, 0x1CA, AX);
L1633:
    /* 1633  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1634:
    /* 1634  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1636:
    /* 1636  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1638:
    /* 1638  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L163A:
    /* 163A  mov     dx,word ptr ds:[EYE_Z] */
    DX = rw(pDS, 0x26C2);
L163E:
    /* 163E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L163F:
    /* 163F  sub     dx,ax */
    DX = sub16(DX, AX, 0);
L1641:
    /* 1641  mov     word ptr ds:[1DEh],dx */
    ww(pDS, 0x1DE, DX);
L1645:
    /* 1645  mov     bx,word ptr ds:[EYE_Z+2] */
    BX = rw(pDS, 0x26C4);
L1649:
    /* 1649  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L164A:
    /* 164A  sbb     bx,ax */
    BX = (uint16_t)(BX - AX - CF);
L164C:
    /* 164C  mov     word ptr ds:[1E0h],bx */
    ww(pDS, 0x1E0, BX);
L1650:
    /* 1650  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L1651:
    /* 1651  mov     cx,es */
    CX = asm_es;
L1653:
    /* 1653  jcxz    L1669 */
    if (!CX) goto L1669;
L1655:
    /* 1655  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L1657:
    /* 1657  jns     short L1663 */
    if (!SF) goto L1663;
L1659:
    /* 1659  neg     cx */
    CX = (uint16_t)-CX;
L165B: /* L165B */
    /* 165B  shl     ax,1 */
    AX = shl16(AX, 1);
L165D:
    /* 165D  rcl     bx,1 */
    BX = rcl16(BX, 1);
L165F:
    /* 165F  loop    L165B */
    if (--CX) goto L165B;
L1661:
    /* 1661  jmp     short L1669 */
    goto L1669;
L1663: /* L1663 */
    /* 1663  sar     bx,1 */
    BX = sar16(BX, 1);
L1665:
    /* 1665  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1667:
    /* 1667  loop    L1663 */
    if (--CX) goto L1663;
L1669: /* L1669 */
    /* 1669  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L166A:
    /* 166A  cmp     bx,dx */
    sub16(BX, DX, 0);
L166C:
    /* 166C  je      short L1671 */
    if (ZF) goto L1671;
L166E:
    /* 166E  jmp     L16FE */
    goto L16FE;
L1671: /* L1671 */
    /* 1671  mov     word ptr ds:[1CCh],ax */
    ww(pDS, 0x1CC, AX);
L1674:
    /* 1674  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1675:
    /* 1675  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1677:
    /* 1677  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1679:
    /* 1679  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L167B:
    /* 167B  push    di */
    push16(DI);
L167C:
    /* 167C  mov     si,word ptr [di-0Ah] */
    SI = rw(pDS, DI + 0xFFF6);
L167F:
    /* 167F  mov     ax,si */
    AX = SI;
L1681:
    /* 1681  add     ax,word ptr ds:[1C8h] */
    if (CODE004[0x1681] == 0x2B) { AX = sub16(AX, rw(pDS, rw(CODE004, 0x1683)), 0); } else { AX = add16(AX, rw(pDS, rw(CODE004, 0x1683)), 0); }
L1685:
    /* 1685  jns     short L168A */
    if (!SF) goto L168A;
L1687:
    /* 1687  jmp     L170F */
    goto L170F;
L168A: /* L168A */
    /* 168A  or      bp,si */
    BP = (uint16_t)(BP | SI);
L168C:
    /* 168C  mov     ax,bp */
    AX = BP;
L168E:
    /* 168E  mov     bx,1E2h */
    BX = 0x1E2;
L1691:
    /* 1691  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L1693:
    /* 1693  je      short L169A */
    if (ZF) goto L169A;
L1695:
    /* 1695  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L1697:
    /* 1697  xlatb */
    AL = rb(pDS, BX + AL);
L1698:
    /* 1698  jmp     short L169D */
    goto L169D;
L169A: /* L169A */
    /* 169A  xlatb */
    AL = rb(pDS, BX + AL);
L169B:
    /* 169B  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L169D: /* L169D */
    /* 169D  mov     cl,al */
    CL = AL;
L169F:
    /* 169F  mov     bx,word ptr ds:[1C8h] */
    BX = rw(pDS, 0x1C8);
L16A3:
    /* 16A3  mov     bp,word ptr ds:[1CCh] */
    BP = rw(pDS, 0x1CC);
L16A7:
    /* 16A7  mov     ax,word ptr ds:[1CAh] */
    AX = rw(pDS, 0x1CA);
L16AA:
    /* 16AA  call    _sphere_check */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x1370), 0x16AD)) != 0) return c;
L16AD:
    /* 16AD  jb      short L170F */
    if (CF) goto L170F;
L16AF:
    /* 16AF  mov     word ptr ds:[1D2h],ax */
    ww(pDS, 0x1D2, AX);
L16B2:
    /* 16B2  call    _get_dist */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x151F), 0x16B5)) != 0) return c;
L16B5:
    /* 16B5  les     di,dword ptr ds:[2860h] */
    { uint16_t o_ = rw(pDS, (uint16_t)(0x2860)), s_ = rw(pDS, (uint16_t)(0x2860 + 2));
    DI = o_;
    SET_ES(s_); }
L16B9:
    /* 16B9  mov     dx,di */
    DX = DI;
L16BB:
    /* 16BB  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L16BC:
    /* 16BC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16BD:
    /* 16BD  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L16BE:
    /* 16BE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16BF:
    /* 16BF  mov     ax,0 */
    AX = 0x0;
L16C2:
    /* 16C2  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16C3:
    /* 16C3  pop     ax */
    AX = pop16();
L16C4:
    /* 16C4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16C5:
    /* 16C5  mov     ax,word ptr ds:[1C8h] */
    AX = rw(pDS, 0x1C8);
L16C8:
    /* 16C8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16C9:
    /* 16C9  mov     ax,word ptr ds:[1CAh] */
    AX = rw(pDS, 0x1CA);
L16CC:
    /* 16CC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16CD:
    /* 16CD  mov     ax,word ptr ds:[1CCh] */
    AX = rw(pDS, 0x1CC);
L16D0:
    /* 16D0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16D1:
    /* 16D1  mov     ax,word ptr ds:[1D2h] */
    AX = rw(pDS, 0x1D2);
L16D4:
    /* 16D4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16D5:
    /* 16D5  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L16D6:
    /* 16D6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16D7:
    /* 16D7  mov     si,1D6h */
    SI = 0x1D6;
L16DA:
    /* 16DA  mov     cx,6 */
    CX = 0x6;
L16DD:
    /* 16DD  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L16DF:
    /* 16DF  mov     word ptr ds:[286Ch],0 */
    ww(pDS, 0x286C, 0x0);
L16E5:
    /* 16E5  mov     word ptr ds:[2860h],di */
    ww(pDS, 0x2860, DI);
L16E9:
    /* 16E9  mov     di,word ptr ds:[2868h] */
    DI = rw(pDS, 0x2868);
L16ED:
    /* 16ED  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L16EE:
    /* 16EE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16EF:
    /* 16EF  mov     word ptr ds:[2868h],di */
    ww(pDS, 0x2868, DI);
L16F3:
    /* 16F3  mov     ax,ds */
    AX = asm_ds;
L16F5:
    /* 16F5  mov     es,ax */
    SET_ES(AX);
L16F7:
    /* 16F7  pop     si */
    SI = pop16();
L16F8:
    /* 16F8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L16F9:
    /* 16F9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L16FA:
    /* 16FA  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L16FE: /* L16FE */
    /* 16FE  mov     word ptr ds:[2882h],0FFFFh */
    ww(pDS, 0x2882, 0xFFFF);
L1704:
    /* 1704  mov     ax,ds */
    AX = asm_ds;
L1706:
    /* 1706  mov     es,ax */
    SET_ES(AX);
L1708:
    /* 1708  pop     si */
    SI = pop16();
L1709:
    /* 1709  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L170A:
    /* 170A  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L170B:
    /* 170B  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L170F: /* L170F */
    /* 170F  mov     ax,ds */
    AX = asm_ds;
L1711:
    /* 1711  mov     es,ax */
    SET_ES(AX);
L1713:
    /* 1713  pop     ax */
    AX = pop16();
L1714:
    /* 1714  pop     si */
    SI = pop16();
L1715:
    /* 1715  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1716:
    /* 1716  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1717:
    /* 1717  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L171B:
    /* 171B  push    ds */
    push16(asm_ds);
L171C:
    /* 171C  push    si */
    push16(SI);
L171D:
    /* 171D  push    cx */
    push16(CX);
L171E:
    /* 171E  mov     si,di */
    SI = DI;
L1720:
    /* 1720  add     si,6 */
    SI = (uint16_t)(SI + 0x6);
L1723:
    /* 1723  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1724:
    /* 1724  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L1725:
    /* 1725  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1726:
    /* 1726  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1727:
    /* 1727  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1728:
    /* 1728  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L1729:
    /* 1729  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L172A:
    /* 172A  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L172B:
    /* 172B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L172C:
    /* 172C  mov     word ptr es:[289Ah],ax */
    ww(pES, 0x289A, AX);
L1730:
    /* 1730  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1731:
    /* 1731  push    cx */
    push16(CX);
L1732:
    /* 1732  mov     di,1D6h */
    DI = 0x1D6;
L1735:
    /* 1735  mov     cx,6 */
    CX = 0x6;
L1738:
    /* 1738  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L173A:
    /* 173A  pop     cx */
    CX = pop16();
L173B:
    /* 173B  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L173C:
    /* 173C  mov     ax,es */
    AX = asm_es;
L173E:
    /* 173E  mov     ds,ax */
    SET_DS(AX);
L1740:
    /* 1740  mov     word ptr ds:[2882h],0 */
    ww(pDS, 0x2882, 0x0);
L1746:
    /* 1746  mov     word ptr ds:[1C4h],1 */
    ww(pDS, 0x1C4, 0x1);
L174C:
    /* 174C  mov     si,dx */
    SI = DX;
L174E:
    /* 174E  mov     word ptr ds:[OBJ_X],bx */
    ww(pDS, 0x2886, BX);
L1752:
    /* 1752  mov     ax,bx */
    AX = BX;
L1754:
    /* 1754  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1755:
    /* 1755  xor     bx,dx */
    BX = (uint16_t)(BX ^ DX);
L1757:
    /* 1757  sub     bx,dx */
    BX = (uint16_t)(BX - DX);
L1759:
    /* 1759  add     bx,word ptr [si-6] */
    BX = (uint16_t)(BX + rw(pDS, SI + 0xFFFA));
L175C:
    /* 175C  mov     word ptr ds:[OBJ_Y],cx */
    ww(pDS, 0x2888, CX);
L1760:
    /* 1760  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L1761:
    /* 1761  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1762:
    /* 1762  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1764:
    /* 1764  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1766:
    /* 1766  add     ax,word ptr [si-4] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFC));
L1769:
    /* 1769  or      bx,ax */
    BX = (uint16_t)(BX | AX);
L176B:
    /* 176B  mov     word ptr ds:[OBJ_Z],bp */
    ww(pDS, 0x288A, BP);
L176F:
    /* 176F  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L1770:
    /* 1770  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1771:
    /* 1771  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1773:
    /* 1773  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1775:
    /* 1775  add     ax,word ptr [si-2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFE));
L1778:
    /* 1778  or      bx,ax */
    BX = (uint16_t)(BX | AX);
L177A:
    /* 177A  mov     ax,bx */
    AX = BX;
L177C:
    /* 177C  mov     bx,1E2h */
    BX = 0x1E2;
L177F:
    /* 177F  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L1781:
    /* 1781  je      short L1788 */
    if (ZF) goto L1788;
L1783:
    /* 1783  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L1785:
    /* 1785  xlatb */
    AL = rb(pDS, BX + AL);
L1786:
    /* 1786  jmp     short L178B */
    goto L178B;
L1788: /* L1788 */
    /* 1788  xlatb */
    AL = rb(pDS, BX + AL);
L1789:
    /* 1789  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L178B: /* L178B */
    /* 178B  mov     byte ptr ds:[OBJ_SHIFT],al */
    wb(pDS, 0x2880, AL);
L178E:
    /* 178E  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x1791)) != 0) return c;
L1791:
    /* 1791  mov     ax,di */
    AX = DI;
L1793:
    /* 1793  call    _seg004_5040 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5040), 0x1796)) != 0) return c;
L1796:
    /* 1796  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1797:
    /* 1797  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1798:
    /* 1798  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x179C)) != 0) return c;
L179C:
    /* 179C  pop     cx */
    CX = pop16();
L179D:
    /* 179D  pop     si */
    SI = pop16();
L179E:
    /* 179E  pop     ds */
    SET_DS(pop16());
L179F:
    /* 179F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L17A0:
    /* 17A0  mov     word ptr es:[2882h],0FFFFh */
    ww(pES, 0x2882, 0xFFFF);
L17A7:
    /* 17A7  pop     cx */
    CX = pop16();
L17A8:
    /* 17A8  pop     si */
    SI = pop16();
L17A9:
    /* 17A9  pop     ds */
    SET_DS(pop16());
L17AA:
    /* 17AA  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_17AB  (+17AB)
       UW1 only: called by render_3d for each record from 161Eh before the database runs (BP = the
       record + 6); takes the record's position relative to the eye (26BAh..) and calls get_dist;
       not traced further. */
L17AB: /* _seg004_17AB */
    /* 17AB  mov     word ptr ds:[1C0h],bp */
    ww(pDS, 0x1C0, BP);
L17AF:
    /* 17AF  add     bp,2 */
    BP = (uint16_t)(BP + 0x2);
L17B2:
    /* 17B2  mov     di,word ptr [bp] */
    DI = rw(pSS, BP);
L17B5:
    /* 17B5  mov     es,word ptr [di-8] */
    SET_ES(rw(pDS, DI + 0xFFF8));
L17B8:
    /* 17B8  mov     ax,word ptr ds:[EYE_X] */
    AX = rw(pDS, 0x26BA);
L17BB:
    /* 17BB  sub     ax,word ptr [bp+2] */
    AX = sub16(AX, rw(pSS, BP + 0x2), 0);
L17BE:
    /* 17BE  mov     word ptr ds:[1D6h],ax */
    ww(pDS, 0x1D6, AX);
L17C1:
    /* 17C1  mov     bx,word ptr ds:[EYE_X+2] */
    BX = rw(pDS, 0x26BC);
L17C5:
    /* 17C5  sbb     bx,word ptr [bp+4] */
    BX = (uint16_t)(BX - rw(pSS, BP + 0x4) - CF);
L17C8:
    /* 17C8  mov     word ptr ds:[1D8h],bx */
    ww(pDS, 0x1D8, BX);
L17CC:
    /* 17CC  mov     cx,es */
    CX = asm_es;
L17CE:
    /* 17CE  jcxz    L17E4 */
    if (!CX) goto L17E4;
L17D0:
    /* 17D0  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L17D2:
    /* 17D2  jns     short L17DE */
    if (!SF) goto L17DE;
L17D4:
    /* 17D4  neg     cx */
    CX = (uint16_t)-CX;
L17D6: /* L17D6 */
    /* 17D6  shl     ax,1 */
    AX = shl16(AX, 1);
L17D8:
    /* 17D8  rcl     bx,1 */
    BX = rcl16(BX, 1);
L17DA:
    /* 17DA  loop    L17D6 */
    if (--CX) goto L17D6;
L17DC:
    /* 17DC  jmp     short L17E4 */
    goto L17E4;
L17DE: /* L17DE */
    /* 17DE  sar     bx,1 */
    BX = sar16(BX, 1);
L17E0:
    /* 17E0  rcr     ax,1 */
    AX = rcr16(AX, 1);
L17E2:
    /* 17E2  loop    L17DE */
    if (--CX) goto L17DE;
L17E4: /* L17E4 */
    /* 17E4  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L17E5:
    /* 17E5  cmp     bx,dx */
    sub16(BX, DX, 0);
L17E7:
    /* 17E7  je      short L17EC */
    if (ZF) goto L17EC;
L17E9:
    /* 17E9  jmp     L18EC */
    goto L18EC;
L17EC: /* L17EC */
    /* 17EC  mov     word ptr ds:[1C8h],ax */
    ww(pDS, 0x1C8, AX);
L17EF:
    /* 17EF  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L17F1:
    /* 17F1  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L17F3:
    /* 17F3  mov     si,ax */
    SI = AX;
L17F5:
    /* 17F5  mov     ax,word ptr ds:[EYE_Y] */
    AX = rw(pDS, 0x26BE);
L17F8:
    /* 17F8  sub     ax,word ptr [bp+6] */
    AX = sub16(AX, rw(pSS, BP + 0x6), 0);
L17FB:
    /* 17FB  mov     word ptr ds:[1DAh],ax */
    ww(pDS, 0x1DA, AX);
L17FE:
    /* 17FE  mov     bx,word ptr ds:[EYE_Y+2] */
    BX = rw(pDS, 0x26C0);
L1802:
    /* 1802  sbb     bx,word ptr [bp+8] */
    BX = (uint16_t)(BX - rw(pSS, BP + 0x8) - CF);
L1805:
    /* 1805  mov     word ptr ds:[1DCh],bx */
    ww(pDS, 0x1DC, BX);
L1809:
    /* 1809  mov     cx,es */
    CX = asm_es;
L180B:
    /* 180B  jcxz    L1821 */
    if (!CX) goto L1821;
L180D:
    /* 180D  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L180F:
    /* 180F  jns     short L181B */
    if (!SF) goto L181B;
L1811:
    /* 1811  neg     cx */
    CX = (uint16_t)-CX;
L1813: /* L1813 */
    /* 1813  shl     ax,1 */
    AX = shl16(AX, 1);
L1815:
    /* 1815  rcl     bx,1 */
    BX = rcl16(BX, 1);
L1817:
    /* 1817  loop    L1813 */
    if (--CX) goto L1813;
L1819:
    /* 1819  jmp     short L1821 */
    goto L1821;
L181B: /* L181B */
    /* 181B  sar     bx,1 */
    BX = sar16(BX, 1);
L181D:
    /* 181D  rcr     ax,1 */
    AX = rcr16(AX, 1);
L181F:
    /* 181F  loop    L181B */
    if (--CX) goto L181B;
L1821: /* L1821 */
    /* 1821  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1822:
    /* 1822  cmp     bx,dx */
    sub16(BX, DX, 0);
L1824:
    /* 1824  je      short L1829 */
    if (ZF) goto L1829;
L1826:
    /* 1826  jmp     L18EC */
    goto L18EC;
L1829: /* L1829 */
    /* 1829  mov     word ptr ds:[1CAh],ax */
    ww(pDS, 0x1CA, AX);
L182C:
    /* 182C  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L182E:
    /* 182E  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1830:
    /* 1830  or      si,ax */
    SI = logic16((uint16_t)(SI | AX));
L1832:
    /* 1832  mov     ax,word ptr ds:[EYE_Z] */
    AX = rw(pDS, 0x26C2);
L1835:
    /* 1835  sbb     ax,word ptr [bp+0Ah] */
    AX = sub16(AX, rw(pSS, BP + 0xA), CF);
L1838:
    /* 1838  mov     word ptr ds:[1DEh],ax */
    ww(pDS, 0x1DE, AX);
L183B:
    /* 183B  mov     bx,word ptr ds:[EYE_Z+2] */
    BX = rw(pDS, 0x26C4);
L183F:
    /* 183F  sbb     bx,word ptr [bp+0Ch] */
    BX = (uint16_t)(BX - rw(pSS, BP + 0xC) - CF);
L1842:
    /* 1842  mov     word ptr ds:[1E0h],bx */
    ww(pDS, 0x1E0, BX);
L1846:
    /* 1846  mov     cx,es */
    CX = asm_es;
L1848:
    /* 1848  jcxz    L185E */
    if (!CX) goto L185E;
L184A:
    /* 184A  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L184C:
    /* 184C  jns     short L1858 */
    if (!SF) goto L1858;
L184E:
    /* 184E  neg     cx */
    CX = (uint16_t)-CX;
L1850: /* L1850 */
    /* 1850  shl     ax,1 */
    AX = shl16(AX, 1);
L1852:
    /* 1852  rcl     bx,1 */
    BX = rcl16(BX, 1);
L1854:
    /* 1854  loop    L1850 */
    if (--CX) goto L1850;
L1856:
    /* 1856  jmp     short L185E */
    goto L185E;
L1858: /* L1858 */
    /* 1858  sar     bx,1 */
    BX = sar16(BX, 1);
L185A:
    /* 185A  rcr     ax,1 */
    AX = rcr16(AX, 1);
L185C:
    /* 185C  loop    L1858 */
    if (--CX) goto L1858;
L185E: /* L185E */
    /* 185E  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L185F:
    /* 185F  cmp     bx,dx */
    sub16(BX, DX, 0);
L1861:
    /* 1861  je      short L1866 */
    if (ZF) goto L1866;
L1863:
    /* 1863  jmp     L18EC */
    goto L18EC;
L1866: /* L1866 */
    /* 1866  mov     word ptr ds:[1CCh],ax */
    ww(pDS, 0x1CC, AX);
L1869:
    /* 1869  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L186B:
    /* 186B  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L186D:
    /* 186D  or      si,ax */
    SI = (uint16_t)(SI | AX);
L186F:
    /* 186F  mov     bp,si */
    BP = SI;
L1871:
    /* 1871  mov     si,word ptr [di-0Ah] */
    SI = rw(pDS, DI + 0xFFF6);
L1874:
    /* 1874  mov     ax,si */
    AX = SI;
L1876:
    /* 1876  add     ax,word ptr ds:[1C8h] */
    if (CODE004[0x1876] == 0x2B) { AX = sub16(AX, rw(pDS, rw(CODE004, 0x1878)), 0); } else { AX = add16(AX, rw(pDS, rw(CODE004, 0x1878)), 0); }
L187A:
    /* 187A  js      short L18F7 */
    if (SF) goto L18F7;
L187C:
    /* 187C  or      bp,si */
    BP = (uint16_t)(BP | SI);
L187E:
    /* 187E  mov     ax,bp */
    AX = BP;
L1880:
    /* 1880  mov     bx,1E2h */
    BX = 0x1E2;
L1883:
    /* 1883  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L1885:
    /* 1885  je      short L188C */
    if (ZF) goto L188C;
L1887:
    /* 1887  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L1889:
    /* 1889  xlatb */
    AL = rb(pDS, BX + AL);
L188A:
    /* 188A  jmp     short L188F */
    goto L188F;
L188C: /* L188C */
    /* 188C  xlatb */
    AL = rb(pDS, BX + AL);
L188D:
    /* 188D  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L188F: /* L188F */
    /* 188F  mov     cl,al */
    CL = AL;
L1891:
    /* 1891  mov     bx,word ptr ds:[1C8h] */
    BX = rw(pDS, 0x1C8);
L1895:
    /* 1895  mov     bp,word ptr ds:[1CCh] */
    BP = rw(pDS, 0x1CC);
L1899:
    /* 1899  mov     ax,word ptr ds:[1CAh] */
    AX = rw(pDS, 0x1CA);
L189C:
    /* 189C  call    _sphere_check */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x1370), 0x189F)) != 0) return c;
L189F:
    /* 189F  jb      short L18F7 */
    if (CF) goto L18F7;
L18A1:
    /* 18A1  mov     word ptr ds:[1D2h],ax */
    ww(pDS, 0x1D2, AX);
L18A4:
    /* 18A4  call    _get_dist */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x151F), 0x18A7)) != 0) return c;
L18A7:
    /* 18A7  les     di,dword ptr ds:[2860h] */
    { uint16_t o_ = rw(pDS, (uint16_t)(0x2860)), s_ = rw(pDS, (uint16_t)(0x2860 + 2));
    DI = o_;
    SET_ES(s_); }
L18AB:
    /* 18AB  mov     dx,di */
    DX = DI;
L18AD:
    /* 18AD  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L18AE:
    /* 18AE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L18AF:
    /* 18AF  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L18B0:
    /* 18B0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L18B1:
    /* 18B1  mov     ax,2 */
    AX = 0x2;
L18B4:
    /* 18B4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L18B5:
    /* 18B5  mov     ax,word ptr ds:[1C0h] */
    AX = rw(pDS, 0x1C0);
L18B8:
    /* 18B8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L18B9:
    /* 18B9  mov     ax,word ptr ds:[1C8h] */
    AX = rw(pDS, 0x1C8);
L18BC:
    /* 18BC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L18BD:
    /* 18BD  mov     ax,word ptr ds:[1CAh] */
    AX = rw(pDS, 0x1CA);
L18C0:
    /* 18C0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L18C1:
    /* 18C1  mov     ax,word ptr ds:[1CCh] */
    AX = rw(pDS, 0x1CC);
L18C4:
    /* 18C4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L18C5:
    /* 18C5  mov     ax,word ptr ds:[1D2h] */
    AX = rw(pDS, 0x1D2);
L18C8:
    /* 18C8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L18C9:
    /* 18C9  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L18CA:
    /* 18CA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L18CB:
    /* 18CB  mov     si,1D6h */
    SI = 0x1D6;
L18CE:
    /* 18CE  mov     cx,6 */
    CX = 0x6;
L18D1:
    /* 18D1  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L18D3:
    /* 18D3  mov     word ptr ds:[286Ch],0 */
    ww(pDS, 0x286C, 0x0);
L18D9:
    /* 18D9  mov     word ptr ds:[2860h],di */
    ww(pDS, 0x2860, DI);
L18DD:
    /* 18DD  mov     di,word ptr ds:[2868h] */
    DI = rw(pDS, 0x2868);
L18E1:
    /* 18E1  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L18E2:
    /* 18E2  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L18E3:
    /* 18E3  mov     word ptr ds:[2868h],di */
    ww(pDS, 0x2868, DI);
L18E7:
    /* 18E7  mov     ax,ds */
    AX = asm_ds;
L18E9:
    /* 18E9  mov     es,ax */
    SET_ES(AX);
L18EB:
    /* 18EB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L18EC: /* L18EC */
    /* 18EC  mov     word ptr ds:[2882h],0FFFFh */
    ww(pDS, 0x2882, 0xFFFF);
L18F2:
    /* 18F2  mov     ax,ds */
    AX = asm_ds;
L18F4:
    /* 18F4  mov     es,ax */
    SET_ES(AX);
L18F6:
    /* 18F6  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L18F7: /* L18F7 */
    /* 18F7  mov     ax,ds */
    AX = asm_ds;
L18F9:
    /* 18F9  mov     es,ax */
    SET_ES(AX);
L18FB:
    /* 18FB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L18FC:
    /* 18FC  push    ds */
    push16(asm_ds);
L18FD:
    /* 18FD  push    si */
    push16(SI);
L18FE:
    /* 18FE  push    di */
    push16(DI);
L18FF:
    /* 18FF  push    cx */
    push16(CX);
L1900:
    /* 1900  push    word ptr ds:[OBJ_Z] */
    push16(rw(pDS, 0x288A));
L1904:
    /* 1904  push    word ptr ds:[OBJ_Y] */
    push16(rw(pDS, 0x2888));
L1908:
    /* 1908  push    word ptr ds:[OBJ_X] */
    push16(rw(pDS, 0x2886));
L190C:
    /* 190C  mov     si,di */
    SI = DI;
L190E:
    /* 190E  add     si,6 */
    SI = (uint16_t)(SI + 0x6);
L1911:
    /* 1911  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1912:
    /* 1912  mov     bp,ax */
    BP = AX;
L1914:
    /* 1914  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1915:
    /* 1915  mov     word ptr es:[OBJ_X],ax */
    ww(pES, 0x2886, AX);
L1919:
    /* 1919  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L191A:
    /* 191A  mov     word ptr es:[OBJ_Y],ax */
    ww(pES, 0x2888, AX);
L191E:
    /* 191E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L191F:
    /* 191F  mov     word ptr es:[OBJ_Z],ax */
    ww(pES, 0x288A, AX);
L1923:
    /* 1923  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1924:
    /* 1924  mov     word ptr es:[289Ah],ax */
    ww(pES, 0x289A, AX);
L1928:
    /* 1928  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1929:
    /* 1929  mov     di,1D6h */
    DI = 0x1D6;
L192C:
    /* 192C  mov     cx,6 */
    CX = 0x6;
L192F:
    /* 192F  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L1931:
    /* 1931  mov     bx,es */
    BX = asm_es;
L1933:
    /* 1933  mov     ds,bx */
    SET_DS(BX);
L1935:
    /* 1935  mov     word ptr ds:[2882h],0 */
    ww(pDS, 0x2882, 0x0);
L193B:
    /* 193B  mov     word ptr ds:[1C4h],2 */
    ww(pDS, 0x1C4, 0x2);
L1941:
    /* 1941  call    _seg004_5040 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5040), 0x1944)) != 0) return c;
L1944:
    /* 1944  push    word ptr ds:[MAT_ZZ] */
    push16(rw(pDS, 0x1612));
L1948:
    /* 1948  push    word ptr ds:[MAT_ZY] */
    push16(rw(pDS, 0x1610));
L194C:
    /* 194C  push    word ptr ds:[MAT_ZX] */
    push16(rw(pDS, 0x160E));
L1950:
    /* 1950  push    word ptr ds:[MAT_YZ] */
    push16(rw(pDS, 0x160C));
L1954:
    /* 1954  push    word ptr ds:[MAT_YY] */
    push16(rw(pDS, 0x160A));
L1958:
    /* 1958  push    word ptr ds:[MAT_YX] */
    push16(rw(pDS, 0x1608));
L195C:
    /* 195C  push    word ptr ds:[MAT_XZ] */
    push16(rw(pDS, 0x1606));
L1960:
    /* 1960  push    word ptr ds:[MAT_XY] */
    push16(rw(pDS, 0x1604));
L1964:
    /* 1964  push    word ptr ds:[MAT_XX] */
    push16(rw(pDS, 0x1602));
L1968:
    /* 1968  mov     word ptr ds:[1C6h],bp */
    ww(pDS, 0x1C6, BP);
L196C:
    /* 196C  mov     ax,word ptr [bp] */
    AX = rw(pSS, BP);
L196F:
    /* 196F  mov     word ptr ds:[1C0h],ax */
    ww(pDS, 0x1C0, AX);
L1972:
    /* 1972  add     bp,2 */
    BP = (uint16_t)(BP + 0x2);
L1975:
    /* 1975  lea     ax,[bp+2Eh] */
    AX = (uint16_t)(BP + 0x2E);
L1978:
    /* 1978  mov     word ptr ds:[1C2h],ax */
    ww(pDS, 0x1C2, AX);
L197B:
    /* 197B  test    word ptr [bp-4],0FFFFh */
    logic16((uint16_t)(rw(pSS, BP + 0xFFFC) & 0xFFFF));
L1980:
    /* 1980  jne     short L1990 */
    if (!ZF) goto L1990;
L1982:
    /* 1982  mov     si,sp */
    SI = SP;
L1984:
    /* 1984  push    word ptr [bp] */
    push16(rw(pSS, BP));
L1987:
    /* 1987  add     bp,0Eh */
    BP = (uint16_t)(BP + 0xE);
L198A:
    /* 198A  call    _seg004_5834 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5834), 0x198D)) != 0) return c;
L198D:
    /* 198D  pop     si */
    SI = pop16();
L198E:
    /* 198E  jmp     short L19B5 */
    goto L19B5;
L1990: /* L1990 */
    /* 1990  mov     si,word ptr [bp] */
    SI = rw(pSS, BP);
L1993:
    /* 1993  mov     bx,word ptr [bp+24h] */
    BX = rw(pSS, BP + 0x24);
L1996:
    /* 1996  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L1998:
    /* 1998  je      short L199F */
    if (ZF) goto L199F;
L199A:
    /* 199A  push    bp */
    push16(BP);
L199B:
    /* 199B  call    _instance_head */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5426), 0x199E)) != 0) return c;
L199E:
    /* 199E  pop     bp */
    BP = pop16();
L199F: /* L199F */
    /* 199F  mov     bx,word ptr [bp+20h] */
    BX = rw(pSS, BP + 0x20);
L19A2:
    /* 19A2  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L19A4:
    /* 19A4  je      short L19AB */
    if (ZF) goto L19AB;
L19A6:
    /* 19A6  push    bp */
    push16(BP);
L19A7:
    /* 19A7  call    _instance_pitch */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x558E), 0x19AA)) != 0) return c;
L19AA:
    /* 19AA  pop     bp */
    BP = pop16();
L19AB: /* L19AB */
    /* 19AB  mov     bx,word ptr [bp+22h] */
    BX = rw(pSS, BP + 0x22);
L19AE:
    /* 19AE  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L19B0:
    /* 19B0  je      short L19B5 */
    if (ZF) goto L19B5;
L19B2:
    /* 19B2  call    _instance_bank */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x56F6), 0x19B5)) != 0) return c;
L19B5: /* L19B5 */
    /* 19B5  mov     ax,word ptr ds:[OBJ_X] */
    AX = rw(pDS, 0x2886);
L19B8:
    /* 19B8  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L19B9:
    /* 19B9  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L19BB:
    /* 19BB  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L19BD:
    /* 19BD  mov     bp,word ptr [si-6] */
    BP = rw(pDS, SI + 0xFFFA);
L19C0:
    /* 19C0  add     bp,ax */
    BP = add16(BP, AX, 0);
L19C2:
    /* 19C2  jno     short L19C7 */
    if (!OF) goto L19C7;
L19C4:
    /* 19C4  jmp     L1A52 */
    goto L1A52;
L19C7: /* L19C7 */
    /* 19C7  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L19CA:
    /* 19CA  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L19CB:
    /* 19CB  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L19CD:
    /* 19CD  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L19CF:
    /* 19CF  add     ax,word ptr [si-4] */
    AX = add16(AX, rw(pDS, SI + 0xFFFC), 0);
L19D2:
    /* 19D2  jo      short L1A52 */
    if (OF) goto L1A52;
L19D4:
    /* 19D4  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L19D6:
    /* 19D6  mov     ax,word ptr ds:[OBJ_Z] */
    AX = rw(pDS, 0x288A);
L19D9:
    /* 19D9  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L19DA:
    /* 19DA  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L19DC:
    /* 19DC  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L19DE:
    /* 19DE  add     ax,word ptr [si-2] */
    AX = add16(AX, rw(pDS, SI + 0xFFFE), 0);
L19E1:
    /* 19E1  jo      short L1A52 */
    if (OF) goto L1A52;
L19E3:
    /* 19E3  or      ax,bp */
    AX = (uint16_t)(AX | BP);
L19E5:
    /* 19E5  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L19E6:
    /* 19E6  mov     ax,bx */
    AX = BX;
L19E8:
    /* 19E8  mov     bx,1E2h */
    BX = 0x1E2;
L19EB:
    /* 19EB  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L19ED:
    /* 19ED  je      short L19F4 */
    if (ZF) goto L19F4;
L19EF:
    /* 19EF  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L19F1:
    /* 19F1  xlatb */
    AL = rb(pDS, BX + AL);
L19F2:
    /* 19F2  jmp     short L19F7 */
    goto L19F7;
L19F4: /* L19F4 */
    /* 19F4  xlatb */
    AL = rb(pDS, BX + AL);
L19F5:
    /* 19F5  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L19F7: /* L19F7 */
    /* 19F7  mov     byte ptr ds:[OBJ_SHIFT],al */
    wb(pDS, 0x2880, AL);
L19FA:
    /* 19FA  push    si */
    push16(SI);
L19FB:
    /* 19FB  call    _check_flat */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4E76), 0x19FE)) != 0) return c;
L19FE:
    /* 19FE  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x1A01)) != 0) return c;
L1A01:
    /* 1A01  mov     si,word ptr ds:[1C0h] */
    SI = rw(pDS, 0x1C0);
L1A05:
    /* 1A05  or      si,si */
    SI = logic16((uint16_t)(SI | SI));
L1A07:
    /* 1A07  je      short L1A13 */
    if (ZF) goto L1A13;
L1A09:
    /* 1A09  mov     bp,word ptr ds:[1C2h] */
    BP = rw(pDS, 0x1C2);
L1A0D:
    /* 1A0D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A0E:
    /* 1A0E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1A0F:
    /* 1A0F  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x1A13)) != 0) return c;
L1A13: /* L1A13 */
    /* 1A13  pop     si */
    SI = pop16();
L1A14:
    /* 1A14  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A15:
    /* 1A15  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1A16:
    /* 1A16  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x1A1A)) != 0) return c;
L1A1A:
    /* 1A1A  pop     word ptr ds:[MAT_XX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1602, t_); }
L1A1E:
    /* 1A1E  pop     word ptr ds:[MAT_XY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1604, t_); }
L1A22:
    /* 1A22  pop     word ptr ds:[MAT_XZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1606, t_); }
L1A26:
    /* 1A26  pop     word ptr ds:[MAT_YX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1608, t_); }
L1A2A:
    /* 1A2A  pop     word ptr ds:[MAT_YY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160A, t_); }
L1A2E:
    /* 1A2E  pop     word ptr ds:[MAT_YZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160C, t_); }
L1A32:
    /* 1A32  pop     word ptr ds:[MAT_ZX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160E, t_); }
L1A36:
    /* 1A36  pop     word ptr ds:[MAT_ZY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1610, t_); }
L1A3A:
    /* 1A3A  pop     word ptr ds:[MAT_ZZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1612, t_); }
L1A3E:
    /* 1A3E  pop     word ptr ds:[OBJ_X] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2886, t_); }
L1A42:
    /* 1A42  pop     word ptr ds:[OBJ_Y] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2888, t_); }
L1A46:
    /* 1A46  pop     word ptr ds:[OBJ_Z] */
    { uint16_t t_ = pop16(); ww(pDS, 0x288A, t_); }
L1A4A:
    /* 1A4A  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x1A4D)) != 0) return c;
L1A4D:
    /* 1A4D  pop     cx */
    CX = pop16();
L1A4E:
    /* 1A4E  pop     di */
    DI = pop16();
L1A4F:
    /* 1A4F  pop     si */
    SI = pop16();
L1A50:
    /* 1A50  pop     ds */
    SET_DS(pop16());
L1A51:
    /* 1A51  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1A52: /* L1A52 */
    /* 1A52  mov     word ptr ds:[2882h],0FFFFh */
    ww(pDS, 0x2882, 0xFFFF);
L1A58:
    /* 1A58  pop     word ptr ds:[MAT_XX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1602, t_); }
L1A5C:
    /* 1A5C  pop     word ptr ds:[MAT_XY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1604, t_); }
L1A60:
    /* 1A60  pop     word ptr ds:[MAT_XZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1606, t_); }
L1A64:
    /* 1A64  pop     word ptr ds:[MAT_YX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1608, t_); }
L1A68:
    /* 1A68  pop     word ptr ds:[MAT_YY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160A, t_); }
L1A6C:
    /* 1A6C  pop     word ptr ds:[MAT_YZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160C, t_); }
L1A70:
    /* 1A70  pop     word ptr ds:[MAT_ZX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160E, t_); }
L1A74:
    /* 1A74  pop     word ptr ds:[MAT_ZY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1610, t_); }
L1A78:
    /* 1A78  pop     word ptr ds:[MAT_ZZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1612, t_); }
L1A7C:
    /* 1A7C  pop     word ptr ds:[OBJ_X] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2886, t_); }
L1A80:
    /* 1A80  pop     word ptr ds:[OBJ_Y] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2888, t_); }
L1A84:
    /* 1A84  pop     word ptr ds:[OBJ_Z] */
    { uint16_t t_ = pop16(); ww(pDS, 0x288A, t_); }
L1A88:
    /* 1A88  pop     cx */
    CX = pop16();
L1A89:
    /* 1A89  pop     di */
    DI = pop16();
L1A8A:
    /* 1A8A  pop     si */
    SI = pop16();
L1A8B:
    /* 1A8B  pop     ds */
    SET_DS(pop16());
L1A8C:
    /* 1A8C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_1A8D  (+1A8D)
       UW1 only: opcode 6Ah, an object header like do_obj; not traced further. */
L1A8D: /* _seg004_1A8D */
    /* 1A8D  mov     word ptr ds:[2882h],0 */
    ww(pDS, 0x2882, 0x0);
L1A93:
    /* 1A93  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A94:
    /* 1A94  add     ax,si */
    AX = (uint16_t)(AX + SI);
L1A96:
    /* 1A96  mov     di,ax */
    DI = AX;
L1A98:
    /* 1A98  mov     ax,word ptr ds:[EYE_X] */
    AX = rw(pDS, 0x26BA);
L1A9B:
    /* 1A9B  sub     ax,word ptr [si] */
    AX = sub16(AX, rw(pDS, SI), 0);
L1A9D:
    /* 1A9D  mov     word ptr ds:[1D6h],ax */
    ww(pDS, 0x1D6, AX);
L1AA0:
    /* 1AA0  mov     bx,word ptr ds:[EYE_X+2] */
    BX = rw(pDS, 0x26BC);
L1AA4:
    /* 1AA4  sbb     bx,word ptr [si+2] */
    BX = (uint16_t)(BX - rw(pDS, SI + 0x2) - CF);
L1AA7:
    /* 1AA7  mov     word ptr ds:[1D8h],bx */
    ww(pDS, 0x1D8, BX);
L1AAB:
    /* 1AAB  mov     cx,word ptr [di-8] */
    CX = rw(pDS, DI + 0xFFF8);
L1AAE:
    /* 1AAE  jcxz    L1AC4 */
    if (!CX) goto L1AC4;
L1AB0:
    /* 1AB0  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L1AB2:
    /* 1AB2  jns     short L1ABE */
    if (!SF) goto L1ABE;
L1AB4:
    /* 1AB4  neg     cx */
    CX = (uint16_t)-CX;
L1AB6: /* L1AB6 */
    /* 1AB6  shl     ax,1 */
    AX = shl16(AX, 1);
L1AB8:
    /* 1AB8  rcl     bx,1 */
    BX = rcl16(BX, 1);
L1ABA:
    /* 1ABA  loop    L1AB6 */
    if (--CX) goto L1AB6;
L1ABC:
    /* 1ABC  jmp     short L1AC4 */
    goto L1AC4;
L1ABE: /* L1ABE */
    /* 1ABE  sar     bx,1 */
    BX = sar16(BX, 1);
L1AC0:
    /* 1AC0  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1AC2:
    /* 1AC2  loop    L1ABE */
    if (--CX) goto L1ABE;
L1AC4: /* L1AC4 */
    /* 1AC4  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1AC5:
    /* 1AC5  cmp     bx,dx */
    sub16(BX, DX, 0);
L1AC7:
    /* 1AC7  je      short L1ACC */
    if (ZF) goto L1ACC;
L1AC9:
    /* 1AC9  jmp     L1BCD */
    goto L1BCD;
L1ACC: /* L1ACC */
    /* 1ACC  mov     word ptr ds:[OBJ_X],ax */
    ww(pDS, 0x2886, AX);
L1ACF:
    /* 1ACF  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1AD1:
    /* 1AD1  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1AD3:
    /* 1AD3  mov     bp,word ptr [di-6] */
    BP = rw(pDS, DI + 0xFFFA);
L1AD6:
    /* 1AD6  add     bp,ax */
    BP = add16(BP, AX, 0);
L1AD8:
    /* 1AD8  jno     short L1ADD */
    if (!OF) goto L1ADD;
L1ADA:
    /* 1ADA  jmp     L1BCD */
    goto L1BCD;
L1ADD: /* L1ADD */
    /* 1ADD  mov     ax,word ptr ds:[EYE_Y] */
    AX = rw(pDS, 0x26BE);
L1AE0:
    /* 1AE0  sub     ax,word ptr [si+4] */
    AX = sub16(AX, rw(pDS, SI + 0x4), 0);
L1AE3:
    /* 1AE3  mov     word ptr ds:[1DAh],ax */
    ww(pDS, 0x1DA, AX);
L1AE6:
    /* 1AE6  mov     bx,word ptr ds:[EYE_Y+2] */
    BX = rw(pDS, 0x26C0);
L1AEA:
    /* 1AEA  sbb     bx,word ptr [si+6] */
    BX = (uint16_t)(BX - rw(pDS, SI + 0x6) - CF);
L1AED:
    /* 1AED  mov     word ptr ds:[1DCh],bx */
    ww(pDS, 0x1DC, BX);
L1AF1:
    /* 1AF1  mov     cx,word ptr [di-8] */
    CX = rw(pDS, DI + 0xFFF8);
L1AF4:
    /* 1AF4  jcxz    L1B0A */
    if (!CX) goto L1B0A;
L1AF6:
    /* 1AF6  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L1AF8:
    /* 1AF8  jns     short L1B04 */
    if (!SF) goto L1B04;
L1AFA:
    /* 1AFA  neg     cx */
    CX = (uint16_t)-CX;
L1AFC: /* L1AFC */
    /* 1AFC  shl     ax,1 */
    AX = shl16(AX, 1);
L1AFE:
    /* 1AFE  rcl     bx,1 */
    BX = rcl16(BX, 1);
L1B00:
    /* 1B00  loop    L1AFC */
    if (--CX) goto L1AFC;
L1B02:
    /* 1B02  jmp     short L1B0A */
    goto L1B0A;
L1B04: /* L1B04 */
    /* 1B04  sar     bx,1 */
    BX = sar16(BX, 1);
L1B06:
    /* 1B06  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1B08:
    /* 1B08  loop    L1B04 */
    if (--CX) goto L1B04;
L1B0A: /* L1B0A */
    /* 1B0A  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1B0B:
    /* 1B0B  cmp     bx,dx */
    sub16(BX, DX, 0);
L1B0D:
    /* 1B0D  je      short L1B12 */
    if (ZF) goto L1B12;
L1B0F:
    /* 1B0F  jmp     L1BCD */
    goto L1BCD;
L1B12: /* L1B12 */
    /* 1B12  mov     word ptr ds:[OBJ_Y],ax */
    ww(pDS, 0x2888, AX);
L1B15:
    /* 1B15  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1B17:
    /* 1B17  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1B19:
    /* 1B19  add     ax,word ptr [di-4] */
    AX = add16(AX, rw(pDS, DI + 0xFFFC), 0);
L1B1C:
    /* 1B1C  jno     short L1B21 */
    if (!OF) goto L1B21;
L1B1E:
    /* 1B1E  jmp     L1BCD */
    goto L1BCD;
L1B21: /* L1B21 */
    /* 1B21  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L1B23:
    /* 1B23  mov     ax,word ptr ds:[EYE_Z] */
    AX = rw(pDS, 0x26C2);
L1B26:
    /* 1B26  sub     ax,word ptr [si+8] */
    AX = sub16(AX, rw(pDS, SI + 0x8), 0);
L1B29:
    /* 1B29  mov     word ptr ds:[1DEh],ax */
    ww(pDS, 0x1DE, AX);
L1B2C:
    /* 1B2C  mov     bx,word ptr ds:[EYE_Z+2] */
    BX = rw(pDS, 0x26C4);
L1B30:
    /* 1B30  sbb     bx,word ptr [si+0Ah] */
    BX = (uint16_t)(BX - rw(pDS, SI + 0xA) - CF);
L1B33:
    /* 1B33  mov     word ptr ds:[1E0h],bx */
    ww(pDS, 0x1E0, BX);
L1B37:
    /* 1B37  mov     cx,word ptr [di-8] */
    CX = rw(pDS, DI + 0xFFF8);
L1B3A:
    /* 1B3A  jcxz    L1B50 */
    if (!CX) goto L1B50;
L1B3C:
    /* 1B3C  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L1B3E:
    /* 1B3E  jns     short L1B4A */
    if (!SF) goto L1B4A;
L1B40:
    /* 1B40  neg     cx */
    CX = (uint16_t)-CX;
L1B42: /* L1B42 */
    /* 1B42  shl     ax,1 */
    AX = shl16(AX, 1);
L1B44:
    /* 1B44  rcl     bx,1 */
    BX = rcl16(BX, 1);
L1B46:
    /* 1B46  loop    L1B42 */
    if (--CX) goto L1B42;
L1B48:
    /* 1B48  jmp     short L1B50 */
    goto L1B50;
L1B4A: /* L1B4A */
    /* 1B4A  sar     bx,1 */
    BX = sar16(BX, 1);
L1B4C:
    /* 1B4C  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1B4E:
    /* 1B4E  loop    L1B4A */
    if (--CX) goto L1B4A;
L1B50: /* L1B50 */
    /* 1B50  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1B51:
    /* 1B51  cmp     bx,dx */
    sub16(BX, DX, 0);
L1B53:
    /* 1B53  jne     short L1BCD */
    if (!ZF) goto L1BCD;
L1B55:
    /* 1B55  mov     word ptr ds:[OBJ_Z],ax */
    ww(pDS, 0x288A, AX);
L1B58:
    /* 1B58  xor     bx,ax */
    BX = (uint16_t)(BX ^ AX);
L1B5A:
    /* 1B5A  sub     bx,dx */
    BX = (uint16_t)(BX - DX);
L1B5C:
    /* 1B5C  add     bx,word ptr [di-2] */
    BX = add16(BX, rw(pDS, DI + 0xFFFE), 0);
L1B5F:
    /* 1B5F  jo      short L1BCD */
    if (OF) goto L1BCD;
L1B61:
    /* 1B61  or      bx,bp */
    BX = (uint16_t)(BX | BP);
L1B63:
    /* 1B63  add     si,0Ch */
    SI = (uint16_t)(SI + 0xC);
L1B66:
    /* 1B66  push    si */
    push16(SI);
L1B67:
    /* 1B67  mov     si,word ptr [di-0Ah] */
    SI = rw(pDS, DI + 0xFFF6);
L1B6A:
    /* 1B6A  mov     ax,si */
    AX = SI;
L1B6C:
    /* 1B6C  add     ax,word ptr ds:[OBJ_X] */
    if (CODE004[0x1B6C] == 0x2B) { AX = sub16(AX, rw(pDS, rw(CODE004, 0x1B6E)), 0); } else { AX = add16(AX, rw(pDS, rw(CODE004, 0x1B6E)), 0); }
L1B70:
    /* 1B70  js      short L1BDD */
    if (SF) goto L1BDD;
L1B72:
    /* 1B72  mov     word ptr ds:[1C4h],1 */
    ww(pDS, 0x1C4, 0x1);
L1B78:
    /* 1B78  push    bx */
    push16(BX);
L1B79:
    /* 1B79  mov     ax,bx */
    AX = BX;
L1B7B:
    /* 1B7B  mov     bx,1E2h */
    BX = 0x1E2;
L1B7E:
    /* 1B7E  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L1B80:
    /* 1B80  je      short L1B87 */
    if (ZF) goto L1B87;
L1B82:
    /* 1B82  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L1B84:
    /* 1B84  xlatb */
    AL = rb(pDS, BX + AL);
L1B85:
    /* 1B85  jmp     short L1B8A */
    goto L1B8A;
L1B87: /* L1B87 */
    /* 1B87  xlatb */
    AL = rb(pDS, BX + AL);
L1B88:
    /* 1B88  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L1B8A: /* L1B8A */
    /* 1B8A  pop     bx */
    BX = pop16();
L1B8B:
    /* 1B8B  mov     byte ptr ds:[OBJ_SHIFT],al */
    wb(pDS, 0x2880, AL);
L1B8E:
    /* 1B8E  or      bx,si */
    BX = (uint16_t)(BX | SI);
L1B90:
    /* 1B90  mov     ax,bx */
    AX = BX;
L1B92:
    /* 1B92  mov     bx,1E2h */
    BX = 0x1E2;
L1B95:
    /* 1B95  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L1B97:
    /* 1B97  je      short L1B9E */
    if (ZF) goto L1B9E;
L1B99:
    /* 1B99  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L1B9B:
    /* 1B9B  xlatb */
    AL = rb(pDS, BX + AL);
L1B9C:
    /* 1B9C  jmp     short L1BA1 */
    goto L1BA1;
L1B9E: /* L1B9E */
    /* 1B9E  xlatb */
    AL = rb(pDS, BX + AL);
L1B9F:
    /* 1B9F  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L1BA1: /* L1BA1 */
    /* 1BA1  mov     cl,al */
    CL = AL;
L1BA3:
    /* 1BA3  push    di */
    push16(DI);
L1BA4:
    /* 1BA4  mov     bx,word ptr ds:[OBJ_X] */
    BX = rw(pDS, 0x2886);
L1BA8:
    /* 1BA8  mov     bp,word ptr ds:[OBJ_Z] */
    BP = rw(pDS, 0x288A);
L1BAC:
    /* 1BAC  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L1BAF:
    /* 1BAF  call    _sphere_check */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x1370), 0x1BB2)) != 0) return c;
L1BB2:
    /* 1BB2  jb      short L1BDC */
    if (CF) goto L1BDC;
L1BB4:
    /* 1BB4  mov     word ptr ds:[289Ah],ax */
    ww(pDS, 0x289A, AX);
L1BB7:
    /* 1BB7  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x1BBA)) != 0) return c;
L1BBA:
    /* 1BBA  mov     ax,si */
    AX = SI;
L1BBC:
    /* 1BBC  call    _seg004_5040 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5040), 0x1BBF)) != 0) return c;
L1BBF:
    /* 1BBF  pop     si */
    SI = pop16();
L1BC0:
    /* 1BC0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BC1:
    /* 1BC1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1BC2:
    /* 1BC2  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x1BC6)) != 0) return c;
L1BC6:
    /* 1BC6  pop     si */
    SI = pop16();
L1BC7:
    /* 1BC7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BC8:
    /* 1BC8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1BC9:
    /* 1BC9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L1BCD: /* L1BCD */
    /* 1BCD  mov     word ptr ds:[2882h],0FFFFh */
    ww(pDS, 0x2882, 0xFFFF);
L1BD3:
    /* 1BD3  add     si,0Ch */
    SI = add16(SI, 0xC, 0);
L1BD6:
    /* 1BD6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BD7:
    /* 1BD7  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1BD8:
    /* 1BD8  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L1BDC: /* L1BDC */
    /* 1BDC  pop     si */
    SI = pop16();
L1BDD: /* L1BDD */
    /* 1BDD  pop     si */
    SI = pop16();
L1BDE:
    /* 1BDE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BDF:
    /* 1BDF  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1BE0:
    /* 1BE0  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_1BE4  (+1BE4)
       Opcode 38h, the object header. Operands: skip (the offset from after the operand to the
       end of the object's program), shift, then three (extent, 32-bit coordinate) pairs for x,
       y and z, then the radius, then the object's program. Clears 2882h; computes each
       coordinate relative to the eye (26BAh..26C4h) and shifts it by `shift` (right when
       positive, left when negative); if any relative coordinate does not fit in 16 bits the
       object is skipped with 2882h = -1 (L1D2B). Stores the negated values as the object origin
       (2886h..288Ah). The extents are added to the coordinates' magnitudes and ORed together,
       and the bit-length table at ds:22h turns that into 2880h, 15 minus its bit length: the
       left shift the point loaders apply, so a small or near object keeps the most precision.
       A first cull at modify_axis2 (FM name; the add or sub and the axis address are patched by
       seg004_51A3 to the axis the view looks most along) skips the object if its radius
       plus that coordinate is negative, behind the eye. Then sphere_check; if it passes, AX
       goes to 289Ah, self_modify loads the origin into the loaders, set_accept gets sphere_check's
       SI, and the program after the radius is run. Every rejection dispatches the opcode after
       the object (SI = the skip target). */
L1BE4: /* do_obj */
    /* 1BE4  mov     word ptr ds:[2882h],0 */
    ww(pDS, 0x2882, 0x0);
L1BEA:
    /* 1BEA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BEB:
    /* 1BEB  mov     di,ax */
    DI = AX;
L1BED:
    /* 1BED  add     di,si */
    DI = (uint16_t)(DI + SI);
L1BEF:
    /* 1BEF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BF0:
    /* 1BF0  mov     word ptr ds:[2708h],ax */
    ww(pDS, 0x2708, AX);
L1BF3:
    /* 1BF3  mov     cx,ax */
    CX = AX;
L1BF5:
    /* 1BF5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BF6:
    /* 1BF6  mov     bp,ax */
    BP = AX;
L1BF8:
    /* 1BF8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BF9:
    /* 1BF9  sub     ax,word ptr ds:[EYE_X] */
    AX = sub16(AX, rw(pDS, 0x26BA), 0);
L1BFD:
    /* 1BFD  mov     word ptr ds:[1D6h],ax */
    ww(pDS, 0x1D6, AX);
L1C00:
    /* 1C00  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1C01:
    /* 1C01  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C02:
    /* 1C02  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1C03:
    /* 1C03  sbb     bx,word ptr ds:[EYE_X+2] */
    BX = (uint16_t)(BX - rw(pDS, 0x26BC) - CF);
L1C07:
    /* 1C07  mov     word ptr ds:[1D8h],bx */
    ww(pDS, 0x1D8, BX);
L1C0B:
    /* 1C0B  jcxz    L1C21 */
    if (!CX) goto L1C21;
L1C0D:
    /* 1C0D  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L1C0F:
    /* 1C0F  jns     short L1C1B */
    if (!SF) goto L1C1B;
L1C11:
    /* 1C11  neg     cx */
    CX = (uint16_t)-CX;
L1C13: /* L1C13 */
    /* 1C13  shl     ax,1 */
    AX = shl16(AX, 1);
L1C15:
    /* 1C15  rcl     bx,1 */
    BX = rcl16(BX, 1);
L1C17:
    /* 1C17  loop    L1C13 */
    if (--CX) goto L1C13;
L1C19:
    /* 1C19  jmp     short L1C21 */
    goto L1C21;
L1C1B: /* L1C1B */
    /* 1C1B  sar     bx,1 */
    BX = sar16(BX, 1);
L1C1D:
    /* 1C1D  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1C1F:
    /* 1C1F  loop    L1C1B */
    if (--CX) goto L1C1B;
L1C21: /* L1C21 */
    /* 1C21  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1C22:
    /* 1C22  cmp     bx,dx */
    sub16(BX, DX, 0);
L1C24:
    /* 1C24  je      short L1C29 */
    if (ZF) goto L1C29;
L1C26:
    /* 1C26  jmp     L1D2B */
    goto L1D2B;
L1C29: /* L1C29 */
    /* 1C29  neg     ax */
    AX = (uint16_t)-AX;
L1C2B:
    /* 1C2B  mov     word ptr ds:[OBJ_X],ax */
    ww(pDS, 0x2886, AX);
L1C2E:
    /* 1C2E  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1C2F:
    /* 1C2F  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1C31:
    /* 1C31  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1C33:
    /* 1C33  add     bp,ax */
    BP = add16(BP, AX, 0);
L1C35:
    /* 1C35  jno     short L1C3A */
    if (!OF) goto L1C3A;
L1C37:
    /* 1C37  jmp     L1D2B */
    goto L1D2B;
L1C3A: /* L1C3A */
    /* 1C3A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C3B:
    /* 1C3B  mov     dx,ax */
    DX = AX;
L1C3D:
    /* 1C3D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C3E:
    /* 1C3E  sub     ax,word ptr ds:[EYE_Y] */
    AX = sub16(AX, rw(pDS, 0x26BE), 0);
L1C42:
    /* 1C42  mov     word ptr ds:[1DAh],ax */
    ww(pDS, 0x1DA, AX);
L1C45:
    /* 1C45  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1C46:
    /* 1C46  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C47:
    /* 1C47  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1C48:
    /* 1C48  sbb     bx,word ptr ds:[EYE_Y+2] */
    BX = (uint16_t)(BX - rw(pDS, 0x26C0) - CF);
L1C4C:
    /* 1C4C  mov     word ptr ds:[1DCh],bx */
    ww(pDS, 0x1DC, BX);
L1C50:
    /* 1C50  mov     cx,word ptr ds:[2708h] */
    CX = rw(pDS, 0x2708);
L1C54:
    /* 1C54  jcxz    L1C6A */
    if (!CX) goto L1C6A;
L1C56:
    /* 1C56  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L1C58:
    /* 1C58  jns     short L1C64 */
    if (!SF) goto L1C64;
L1C5A:
    /* 1C5A  neg     cx */
    CX = (uint16_t)-CX;
L1C5C: /* L1C5C */
    /* 1C5C  shl     ax,1 */
    AX = shl16(AX, 1);
L1C5E:
    /* 1C5E  rcl     bx,1 */
    BX = rcl16(BX, 1);
L1C60:
    /* 1C60  loop    L1C5C */
    if (--CX) goto L1C5C;
L1C62:
    /* 1C62  jmp     short L1C6A */
    goto L1C6A;
L1C64: /* L1C64 */
    /* 1C64  sar     bx,1 */
    BX = sar16(BX, 1);
L1C66:
    /* 1C66  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1C68:
    /* 1C68  loop    L1C64 */
    if (--CX) goto L1C64;
L1C6A: /* L1C6A */
    /* 1C6A  mov     cx,dx */
    CX = DX;
L1C6C:
    /* 1C6C  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1C6D:
    /* 1C6D  cmp     bx,dx */
    sub16(BX, DX, 0);
L1C6F:
    /* 1C6F  je      short L1C74 */
    if (ZF) goto L1C74;
L1C71:
    /* 1C71  jmp     L1D2B */
    goto L1D2B;
L1C74: /* L1C74 */
    /* 1C74  neg     ax */
    AX = (uint16_t)-AX;
L1C76:
    /* 1C76  mov     word ptr ds:[OBJ_Y],ax */
    ww(pDS, 0x2888, AX);
L1C79:
    /* 1C79  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1C7A:
    /* 1C7A  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1C7C:
    /* 1C7C  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1C7E:
    /* 1C7E  add     ax,cx */
    AX = add16(AX, CX, 0);
L1C80:
    /* 1C80  jno     short L1C85 */
    if (!OF) goto L1C85;
L1C82:
    /* 1C82  jmp     L1D2B */
    goto L1D2B;
L1C85: /* L1C85 */
    /* 1C85  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L1C87:
    /* 1C87  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C88:
    /* 1C88  mov     dx,ax */
    DX = AX;
L1C8A:
    /* 1C8A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C8B:
    /* 1C8B  sub     ax,word ptr ds:[EYE_Z] */
    AX = sub16(AX, rw(pDS, 0x26C2), 0);
L1C8F:
    /* 1C8F  mov     word ptr ds:[1DEh],ax */
    ww(pDS, 0x1DE, AX);
L1C92:
    /* 1C92  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1C93:
    /* 1C93  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C94:
    /* 1C94  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1C95:
    /* 1C95  sbb     bx,word ptr ds:[EYE_Z+2] */
    BX = (uint16_t)(BX - rw(pDS, 0x26C4) - CF);
L1C99:
    /* 1C99  mov     word ptr ds:[1E0h],bx */
    ww(pDS, 0x1E0, BX);
L1C9D:
    /* 1C9D  mov     cx,word ptr ds:[2708h] */
    CX = rw(pDS, 0x2708);
L1CA1:
    /* 1CA1  jcxz    L1CB7 */
    if (!CX) goto L1CB7;
L1CA3:
    /* 1CA3  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L1CA5:
    /* 1CA5  jns     short L1CB1 */
    if (!SF) goto L1CB1;
L1CA7:
    /* 1CA7  neg     cx */
    CX = (uint16_t)-CX;
L1CA9: /* L1CA9 */
    /* 1CA9  shl     ax,1 */
    AX = shl16(AX, 1);
L1CAB:
    /* 1CAB  rcl     bx,1 */
    BX = rcl16(BX, 1);
L1CAD:
    /* 1CAD  loop    L1CA9 */
    if (--CX) goto L1CA9;
L1CAF:
    /* 1CAF  jmp     short L1CB7 */
    goto L1CB7;
L1CB1: /* L1CB1 */
    /* 1CB1  sar     bx,1 */
    BX = sar16(BX, 1);
L1CB3:
    /* 1CB3  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1CB5:
    /* 1CB5  loop    L1CB1 */
    if (--CX) goto L1CB1;
L1CB7: /* L1CB7 */
    /* 1CB7  mov     cx,dx */
    CX = DX;
L1CB9:
    /* 1CB9  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1CBA:
    /* 1CBA  cmp     bx,dx */
    sub16(BX, DX, 0);
L1CBC:
    /* 1CBC  jne     short L1D2B */
    if (!ZF) goto L1D2B;
L1CBE:
    /* 1CBE  neg     ax */
    AX = (uint16_t)-AX;
L1CC0:
    /* 1CC0  mov     word ptr ds:[OBJ_Z],ax */
    ww(pDS, 0x288A, AX);
L1CC3:
    /* 1CC3  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1CC4:
    /* 1CC4  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1CC6:
    /* 1CC6  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1CC8:
    /* 1CC8  add     ax,cx */
    AX = add16(AX, CX, 0);
L1CCA:
    /* 1CCA  jo      short L1D2B */
    if (OF) goto L1D2B;
L1CCC:
    /* 1CCC  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L1CCE:
    /* 1CCE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1CCF:
    /* 1CCF  push    si */
    push16(SI);
L1CD0:
    /* 1CD0  push    di */
    push16(DI);
L1CD1:
    /* 1CD1  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L1CD2:
    /* 1CD2  mov     ax,si */
    AX = SI;
L1CD4: /* modify_axis2 */
    /* 1CD4  add     ax,word ptr ds:[OBJ_X] */
    if (CODE004[0x1CD4] == 0x2B) { AX = sub16(AX, rw(pDS, rw(CODE004, 0x1CD6)), 0); } else { AX = add16(AX, rw(pDS, rw(CODE004, 0x1CD6)), 0); }
L1CD8:
    /* 1CD8  js      short L1D39 */
    if (SF) goto L1D39;
L1CDA:
    /* 1CDA  mov     word ptr ds:[1C4h],0 */
    ww(pDS, 0x1C4, 0x0);
L1CE0:
    /* 1CE0  mov     ax,bp */
    AX = BP;
L1CE2:
    /* 1CE2  mov     bx,1E2h */
    BX = 0x1E2;
L1CE5:
    /* 1CE5  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L1CE7:
    /* 1CE7  je      short L1CEE */
    if (ZF) goto L1CEE;
L1CE9:
    /* 1CE9  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L1CEB:
    /* 1CEB  xlatb */
    AL = rb(pDS, BX + AL);
L1CEC:
    /* 1CEC  jmp     short L1CF1 */
    goto L1CF1;
L1CEE: /* L1CEE */
    /* 1CEE  xlatb */
    AL = rb(pDS, BX + AL);
L1CEF:
    /* 1CEF  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L1CF1: /* L1CF1 */
    /* 1CF1  mov     byte ptr ds:[OBJ_SHIFT],al */
    wb(pDS, 0x2880, AL);
L1CF4:
    /* 1CF4  or      bp,si */
    BP = (uint16_t)(BP | SI);
L1CF6:
    /* 1CF6  mov     ax,bp */
    AX = BP;
L1CF8:
    /* 1CF8  mov     bx,1E2h */
    BX = 0x1E2;
L1CFB:
    /* 1CFB  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L1CFD:
    /* 1CFD  je      short L1D04 */
    if (ZF) goto L1D04;
L1CFF:
    /* 1CFF  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L1D01:
    /* 1D01  xlatb */
    AL = rb(pDS, BX + AL);
L1D02:
    /* 1D02  jmp     short L1D07 */
    goto L1D07;
L1D04: /* L1D04 */
    /* 1D04  xlatb */
    AL = rb(pDS, BX + AL);
L1D05:
    /* 1D05  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L1D07: /* L1D07 */
    /* 1D07  mov     cl,al */
    CL = AL;
L1D09:
    /* 1D09  mov     bx,word ptr ds:[OBJ_X] */
    BX = rw(pDS, 0x2886);
L1D0D:
    /* 1D0D  mov     bp,word ptr ds:[OBJ_Z] */
    BP = rw(pDS, 0x288A);
L1D11:
    /* 1D11  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L1D14:
    /* 1D14  call    _sphere_check */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x1370), 0x1D17)) != 0) return c;
L1D17:
    /* 1D17  pop     di */
    DI = pop16();
L1D18:
    /* 1D18  jb      short L1D3A */
    if (CF) goto L1D3A;
L1D1A:
    /* 1D1A  mov     word ptr ds:[289Ah],ax */
    ww(pDS, 0x289A, AX);
L1D1D:
    /* 1D1D  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x1D20)) != 0) return c;
L1D20:
    /* 1D20  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L1D21:
    /* 1D21  call    _seg004_5040 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5040), 0x1D24)) != 0) return c;
L1D24:
    /* 1D24  pop     si */
    SI = pop16();
L1D25:
    /* 1D25  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D26:
    /* 1D26  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1D27:
    /* 1D27  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L1D2B: /* L1D2B */
    /* 1D2B  mov     word ptr ds:[2882h],0FFFFh */
    ww(pDS, 0x2882, 0xFFFF);
L1D31:
    /* 1D31  mov     si,di */
    SI = DI;
L1D33:
    /* 1D33  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D34:
    /* 1D34  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1D35:
    /* 1D35  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L1D39: /* L1D39 */
    /* 1D39  pop     di */
    DI = pop16();
L1D3A: /* L1D3A */
    /* 1D3A  pop     si */
    SI = pop16();
L1D3B:
    /* 1D3B  mov     si,di */
    SI = DI;
L1D3D:
    /* 1D3D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D3E:
    /* 1D3E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1D3F:
    /* 1D3F  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_1D43  (+1D43)
       UW1 only: opcode 20h, a point given in 32-bit world coordinates: taken relative to the eye
       (26BAh..), shifted to fit 16 bits (by the bit-length table at 1E2h), transformed (mxmul) and
       stored with its clip code (code_pnt); when a coordinate does not fit, all three are scaled
       down first (from 1DDBh). */
L1D43: /* _seg004_1D43 */
    /* 1D43  mov     di,si */
    DI = SI;
L1D45:
    /* 1D45  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D46:
    /* 1D46  sub     ax,word ptr ds:[EYE_X] */
    AX = sub16(AX, rw(pDS, 0x26BA), 0);
L1D4A:
    /* 1D4A  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1D4B:
    /* 1D4B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D4C:
    /* 1D4C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1D4D:
    /* 1D4D  sbb     bx,word ptr ds:[EYE_X+2] */
    BX = (uint16_t)(BX - rw(pDS, 0x26BC) - CF);
L1D51:
    /* 1D51  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1D52:
    /* 1D52  cmp     bx,dx */
    sub16(BX, DX, 0);
L1D54:
    /* 1D54  je      short L1D59 */
    if (ZF) goto L1D59;
L1D56:
    /* 1D56  jmp     L1DDB */
    goto L1DDB;
L1D59: /* L1D59 */
    /* 1D59  mov     word ptr ds:[OBJ_X],ax */
    ww(pDS, 0x2886, AX);
L1D5C:
    /* 1D5C  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1D5E:
    /* 1D5E  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1D60:
    /* 1D60  mov     bp,ax */
    BP = AX;
L1D62:
    /* 1D62  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D63:
    /* 1D63  sub     ax,word ptr ds:[EYE_Y] */
    AX = sub16(AX, rw(pDS, 0x26BE), 0);
L1D67:
    /* 1D67  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1D68:
    /* 1D68  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D69:
    /* 1D69  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1D6A:
    /* 1D6A  sbb     bx,word ptr ds:[EYE_Y+2] */
    BX = (uint16_t)(BX - rw(pDS, 0x26C0) - CF);
L1D6E:
    /* 1D6E  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1D6F:
    /* 1D6F  cmp     bx,dx */
    sub16(BX, DX, 0);
L1D71:
    /* 1D71  jne     short L1DDB */
    if (!ZF) goto L1DDB;
L1D73:
    /* 1D73  mov     word ptr ds:[OBJ_Y],ax */
    ww(pDS, 0x2888, AX);
L1D76:
    /* 1D76  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1D78:
    /* 1D78  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1D7A:
    /* 1D7A  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L1D7C:
    /* 1D7C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D7D:
    /* 1D7D  sub     ax,word ptr ds:[EYE_Z] */
    AX = sub16(AX, rw(pDS, 0x26C2), 0);
L1D81:
    /* 1D81  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1D82:
    /* 1D82  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D83:
    /* 1D83  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1D84:
    /* 1D84  sbb     bx,word ptr ds:[EYE_Z+2] */
    BX = (uint16_t)(BX - rw(pDS, 0x26C4) - CF);
L1D88:
    /* 1D88  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1D89:
    /* 1D89  cmp     bx,dx */
    sub16(BX, DX, 0);
L1D8B:
    /* 1D8B  jne     short L1DDB */
    if (!ZF) goto L1DDB;
L1D8D:
    /* 1D8D  mov     word ptr ds:[OBJ_Z],ax */
    ww(pDS, 0x288A, AX);
L1D90:
    /* 1D90  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1D92:
    /* 1D92  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1D94:
    /* 1D94  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L1D96:
    /* 1D96  mov     ax,bp */
    AX = BP;
L1D98:
    /* 1D98  mov     bx,1E2h */
    BX = 0x1E2;
L1D9B:
    /* 1D9B  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L1D9D:
    /* 1D9D  je      short L1DA4 */
    if (ZF) goto L1DA4;
L1D9F:
    /* 1D9F  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L1DA1:
    /* 1DA1  xlatb */
    AL = rb(pDS, BX + AL);
L1DA2:
    /* 1DA2  jmp     short L1DA7 */
    goto L1DA7;
L1DA4: /* L1DA4 */
    /* 1DA4  xlatb */
    AL = rb(pDS, BX + AL);
L1DA5:
    /* 1DA5  add     al,8 */
    AL = add8(AL, 0x8, 0);
L1DA7: /* L1DA7 */
    /* 1DA7  mov     cl,al */
    CL = AL;
L1DA9:
    /* 1DA9  mov     bx,word ptr ds:[OBJ_X] */
    BX = rw(pDS, 0x2886);
L1DAD:
    /* 1DAD  shl     bx,cl */
    BX = shl16(BX, CL);
L1DAF:
    /* 1DAF  mov     bp,word ptr ds:[OBJ_Z] */
    BP = rw(pDS, 0x288A);
L1DB3:
    /* 1DB3  shl     bp,cl */
    BP = shl16(BP, CL);
L1DB5:
    /* 1DB5  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L1DB8:
    /* 1DB8  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1DBA:
    /* 1DBA  mov     cx,ax */
    CX = AX;
L1DBC: /* L1DBC */
    /* 1DBC  call    _mxmul */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x50D8), 0x1DBF)) != 0) return c;
L1DBF:
    /* 1DBF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DC0:
    /* 1DC0  mov     di,ax */
    DI = AX;
L1DC2:
    /* 1DC2  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L1DC6:
    /* 1DC6  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L1DCA:
    /* 1DCA  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L1DCE:
    /* 1DCE  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x1DD1)) != 0) return c;
L1DD1:
    /* 1DD1  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L1DD5:
    /* 1DD5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DD6:
    /* 1DD6  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1DD7:
    /* 1DD7  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L1DDB: /* L1DDB */
    /* 1DDB  mov     si,di */
    SI = DI;
L1DDD:
    /* 1DDD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DDE:
    /* 1DDE  sub     ax,word ptr ds:[EYE_X] */
    AX = sub16(AX, rw(pDS, 0x26BA), 0);
L1DE2:
    /* 1DE2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DE3:
    /* 1DE3  sbb     ax,word ptr ds:[EYE_X+2] */
    AX = (uint16_t)(AX - rw(pDS, 0x26BC) - CF);
L1DE7:
    /* 1DE7  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1DE8:
    /* 1DE8  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1DEA:
    /* 1DEA  mov     bp,ax */
    BP = AX;
L1DEC:
    /* 1DEC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DED:
    /* 1DED  sub     ax,word ptr ds:[EYE_Y] */
    AX = sub16(AX, rw(pDS, 0x26BE), 0);
L1DF1:
    /* 1DF1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DF2:
    /* 1DF2  sbb     ax,word ptr ds:[EYE_Y+2] */
    AX = (uint16_t)(AX - rw(pDS, 0x26C0) - CF);
L1DF6:
    /* 1DF6  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1DF7:
    /* 1DF7  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1DF9:
    /* 1DF9  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L1DFB:
    /* 1DFB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DFC:
    /* 1DFC  sub     ax,word ptr ds:[EYE_Z] */
    AX = sub16(AX, rw(pDS, 0x26C2), 0);
L1E00:
    /* 1E00  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E01:
    /* 1E01  sbb     ax,word ptr ds:[EYE_Z+2] */
    AX = (uint16_t)(AX - rw(pDS, 0x26C4) - CF);
L1E05:
    /* 1E05  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1E06:
    /* 1E06  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1E08:
    /* 1E08  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L1E0A:
    /* 1E0A  mov     ax,bp */
    AX = BP;
L1E0C:
    /* 1E0C  mov     bx,1E2h */
    BX = 0x1E2;
L1E0F:
    /* 1E0F  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L1E11:
    /* 1E11  je      short L1E18 */
    if (ZF) goto L1E18;
L1E13:
    /* 1E13  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L1E15:
    /* 1E15  xlatb */
    AL = rb(pDS, BX + AL);
L1E16:
    /* 1E16  jmp     short L1E1B */
    goto L1E1B;
L1E18: /* L1E18 */
    /* 1E18  xlatb */
    AL = rb(pDS, BX + AL);
L1E19:
    /* 1E19  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L1E1B: /* L1E1B */
    /* 1E1B  sub     al,10h */
    AL = (uint8_t)(AL - 0x10);
L1E1D:
    /* 1E1D  neg     al */
    AL = (uint8_t)-AL;
L1E1F:
    /* 1E1F  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L1E21:
    /* 1E21  mov     bp,ax */
    BP = AX;
L1E23:
    /* 1E23  mov     si,di */
    SI = DI;
L1E25:
    /* 1E25  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E26:
    /* 1E26  sub     ax,word ptr ds:[EYE_X] */
    AX = sub16(AX, rw(pDS, 0x26BA), 0);
L1E2A:
    /* 1E2A  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1E2B:
    /* 1E2B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E2C:
    /* 1E2C  sbb     ax,word ptr ds:[EYE_X+2] */
    AX = (uint16_t)(AX - rw(pDS, 0x26BC) - CF);
L1E30:
    /* 1E30  mov     cx,bp */
    CX = BP;
L1E32: /* L1E32 */
    /* 1E32  sar     ax,1 */
    AX = sar16(AX, 1);
L1E34:
    /* 1E34  rcr     bx,1 */
    BX = rcr16(BX, 1);
L1E36:
    /* 1E36  loop    L1E32 */
    if (--CX) goto L1E32;
L1E38:
    /* 1E38  mov     word ptr ds:[OBJ_X],bx */
    ww(pDS, 0x2886, BX);
L1E3C:
    /* 1E3C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E3D:
    /* 1E3D  sub     ax,word ptr ds:[EYE_Y] */
    AX = sub16(AX, rw(pDS, 0x26BE), 0);
L1E41:
    /* 1E41  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1E42:
    /* 1E42  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E43:
    /* 1E43  sbb     ax,word ptr ds:[EYE_Y+2] */
    AX = (uint16_t)(AX - rw(pDS, 0x26C0) - CF);
L1E47:
    /* 1E47  mov     cx,bp */
    CX = BP;
L1E49: /* L1E49 */
    /* 1E49  sar     ax,1 */
    AX = sar16(AX, 1);
L1E4B:
    /* 1E4B  rcr     bx,1 */
    BX = rcr16(BX, 1);
L1E4D:
    /* 1E4D  loop    L1E49 */
    if (--CX) goto L1E49;
L1E4F:
    /* 1E4F  mov     word ptr ds:[OBJ_Y],bx */
    ww(pDS, 0x2888, BX);
L1E53:
    /* 1E53  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E54:
    /* 1E54  sub     ax,word ptr ds:[EYE_Z] */
    AX = sub16(AX, rw(pDS, 0x26C2), 0);
L1E58:
    /* 1E58  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1E59:
    /* 1E59  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E5A:
    /* 1E5A  sbb     ax,word ptr ds:[EYE_Z+2] */
    AX = (uint16_t)(AX - rw(pDS, 0x26C4) - CF);
L1E5E:
    /* 1E5E  mov     cx,bp */
    CX = BP;
L1E60: /* L1E60 */
    /* 1E60  sar     ax,1 */
    AX = sar16(AX, 1);
L1E62:
    /* 1E62  rcr     bx,1 */
    BX = rcr16(BX, 1);
L1E64:
    /* 1E64  loop    L1E60 */
    if (--CX) goto L1E60;
L1E66:
    /* 1E66  mov     word ptr ds:[OBJ_Z],bx */
    ww(pDS, 0x288A, BX);
L1E6A:
    /* 1E6A  mov     bx,word ptr ds:[OBJ_X] */
    BX = rw(pDS, 0x2886);
L1E6E:
    /* 1E6E  mov     cx,word ptr ds:[OBJ_Y] */
    CX = rw(pDS, 0x2888);
L1E72:
    /* 1E72  mov     bp,word ptr ds:[OBJ_Z] */
    BP = rw(pDS, 0x288A);
L1E76:
    /* 1E76  jmp     L1DBC */
    goto L1DBC;

    /* seg004_1E79  (+1E79)
       UW1 only: opcode 30h, as opcode 28h (seg004_1E84) with its program address read through a
       pointer. */
L1E79: /* _seg004_1E79 */
    /* 1E79  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E7A:
    /* 1E7A  mov     word ptr ds:[1D4h],si */
    ww(pDS, 0x1D4, SI);
L1E7E:
    /* 1E7E  mov     bx,ax */
    BX = AX;
L1E80:
    /* 1E80  mov     si,word ptr [bx] */
    SI = rw(pDS, BX);
L1E82:
    /* 1E82  jmp     short L1E8B */
    goto L1E8B;

    /* seg004_1E84  (+1E84)
       UW1 only: opcode 28h: runs a sub-program at a scaled offset from the current origin (saved
       at 1C8h..1CCh and restored after), with a bounding test like do_obj's; not traced further. */
L1E84: /* _seg004_1E84 */
    /* 1E84  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E85:
    /* 1E85  mov     word ptr ds:[1D4h],si */
    ww(pDS, 0x1D4, SI);
L1E89:
    /* 1E89  mov     si,ax */
    SI = AX;
L1E8B: /* L1E8B */
    /* 1E8B  mov     ax,word ptr ds:[OBJ_X] */
    AX = rw(pDS, 0x2886);
L1E8E:
    /* 1E8E  mov     word ptr ds:[1C8h],ax */
    ww(pDS, 0x1C8, AX);
L1E91:
    /* 1E91  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L1E94:
    /* 1E94  mov     word ptr ds:[1CAh],ax */
    ww(pDS, 0x1CA, AX);
L1E97:
    /* 1E97  mov     ax,word ptr ds:[OBJ_Z] */
    AX = rw(pDS, 0x288A);
L1E9A:
    /* 1E9A  mov     word ptr ds:[1CCh],ax */
    ww(pDS, 0x1CC, AX);
L1E9D:
    /* 1E9D  mov     bp,word ptr [si-8] */
    BP = rw(pDS, SI + 0xFFF8);
L1EA0:
    /* 1EA0  mov     ax,word ptr ds:[1D6h] */
    AX = rw(pDS, 0x1D6);
L1EA3:
    /* 1EA3  mov     bx,word ptr ds:[1D8h] */
    BX = rw(pDS, 0x1D8);
L1EA7:
    /* 1EA7  mov     cx,bp */
    CX = BP;
L1EA9:
    /* 1EA9  jcxz    L1EBF */
    if (!CX) goto L1EBF;
L1EAB:
    /* 1EAB  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L1EAD:
    /* 1EAD  jns     short L1EB9 */
    if (!SF) goto L1EB9;
L1EAF:
    /* 1EAF  neg     cx */
    CX = (uint16_t)-CX;
L1EB1: /* L1EB1 */
    /* 1EB1  shl     ax,1 */
    AX = shl16(AX, 1);
L1EB3:
    /* 1EB3  rcl     bx,1 */
    BX = rcl16(BX, 1);
L1EB5:
    /* 1EB5  loop    L1EB1 */
    if (--CX) goto L1EB1;
L1EB7:
    /* 1EB7  jmp     short L1EBF */
    goto L1EBF;
L1EB9: /* L1EB9 */
    /* 1EB9  sar     ax,1 */
    AX = sar16(AX, 1);
L1EBB:
    /* 1EBB  rcr     bx,1 */
    BX = rcr16(BX, 1);
L1EBD:
    /* 1EBD  loop    L1EB9 */
    if (--CX) goto L1EB9;
L1EBF: /* L1EBF */
    /* 1EBF  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1EC0:
    /* 1EC0  cmp     bx,dx */
    sub16(BX, DX, 0);
L1EC2:
    /* 1EC2  je      short L1EC7 */
    if (ZF) goto L1EC7;
L1EC4:
    /* 1EC4  jmp     L1FAA */
    goto L1FAA;
L1EC7: /* L1EC7 */
    /* 1EC7  mov     word ptr ds:[OBJ_X],ax */
    ww(pDS, 0x2886, AX);
L1ECA:
    /* 1ECA  mov     ax,word ptr ds:[1DAh] */
    AX = rw(pDS, 0x1DA);
L1ECD:
    /* 1ECD  mov     bx,word ptr ds:[1DCh] */
    BX = rw(pDS, 0x1DC);
L1ED1:
    /* 1ED1  mov     cx,bp */
    CX = BP;
L1ED3:
    /* 1ED3  jcxz    L1EE9 */
    if (!CX) goto L1EE9;
L1ED5:
    /* 1ED5  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L1ED7:
    /* 1ED7  jns     short L1EE3 */
    if (!SF) goto L1EE3;
L1ED9:
    /* 1ED9  neg     cx */
    CX = (uint16_t)-CX;
L1EDB: /* L1EDB */
    /* 1EDB  shl     ax,1 */
    AX = shl16(AX, 1);
L1EDD:
    /* 1EDD  rcl     bx,1 */
    BX = rcl16(BX, 1);
L1EDF:
    /* 1EDF  loop    L1EDB */
    if (--CX) goto L1EDB;
L1EE1:
    /* 1EE1  jmp     short L1EE9 */
    goto L1EE9;
L1EE3: /* L1EE3 */
    /* 1EE3  sar     ax,1 */
    AX = sar16(AX, 1);
L1EE5:
    /* 1EE5  rcr     bx,1 */
    BX = rcr16(BX, 1);
L1EE7:
    /* 1EE7  loop    L1EE3 */
    if (--CX) goto L1EE3;
L1EE9: /* L1EE9 */
    /* 1EE9  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1EEA:
    /* 1EEA  cmp     bx,dx */
    sub16(BX, DX, 0);
L1EEC:
    /* 1EEC  je      short L1EF1 */
    if (ZF) goto L1EF1;
L1EEE:
    /* 1EEE  jmp     L1FAA */
    goto L1FAA;
L1EF1: /* L1EF1 */
    /* 1EF1  mov     word ptr ds:[OBJ_Y],ax */
    ww(pDS, 0x2888, AX);
L1EF4:
    /* 1EF4  mov     ax,word ptr ds:[1DEh] */
    AX = rw(pDS, 0x1DE);
L1EF7:
    /* 1EF7  mov     bx,word ptr ds:[1E0h] */
    BX = rw(pDS, 0x1E0);
L1EFB:
    /* 1EFB  mov     cx,bp */
    CX = BP;
L1EFD:
    /* 1EFD  jcxz    L1F13 */
    if (!CX) goto L1F13;
L1EFF:
    /* 1EFF  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L1F01:
    /* 1F01  jns     short L1F0D */
    if (!SF) goto L1F0D;
L1F03:
    /* 1F03  neg     cx */
    CX = (uint16_t)-CX;
L1F05: /* L1F05 */
    /* 1F05  shl     ax,1 */
    AX = shl16(AX, 1);
L1F07:
    /* 1F07  rcl     bx,1 */
    BX = rcl16(BX, 1);
L1F09:
    /* 1F09  loop    L1F05 */
    if (--CX) goto L1F05;
L1F0B:
    /* 1F0B  jmp     short L1F13 */
    goto L1F13;
L1F0D: /* L1F0D */
    /* 1F0D  sar     ax,1 */
    AX = sar16(AX, 1);
L1F0F:
    /* 1F0F  rcr     bx,1 */
    BX = rcr16(BX, 1);
L1F11:
    /* 1F11  loop    L1F0D */
    if (--CX) goto L1F0D;
L1F13: /* L1F13 */
    /* 1F13  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1F14:
    /* 1F14  cmp     bx,dx */
    sub16(BX, DX, 0);
L1F16:
    /* 1F16  je      short L1F1B */
    if (ZF) goto L1F1B;
L1F18:
    /* 1F18  jmp     L1FAA */
    goto L1FAA;
L1F1B: /* L1F1B */
    /* 1F1B  mov     word ptr ds:[OBJ_Z],ax */
    ww(pDS, 0x288A, AX);
L1F1E:
    /* 1F1E  cmp     word ptr ds:[1C4h],2 */
    sub16(rw(pDS, 0x1C4), 0x2, 0);
L1F23:
    /* 1F23  jne     short L1F5F */
    if (!ZF) goto L1F5F;
L1F25:
    /* 1F25  push    si */
    push16(SI);
L1F26:
    /* 1F26  mov     bp,word ptr ds:[1C6h] */
    BP = rw(pDS, 0x1C6);
L1F2A:
    /* 1F2A  add     bp,2 */
    BP = (uint16_t)(BP + 0x2);
L1F2D:
    /* 1F2D  test    word ptr [bp-4],0FFFFh */
    logic16((uint16_t)(rw(pSS, BP + 0xFFFC) & 0xFFFF));
L1F32:
    /* 1F32  jne     short L1F3C */
    if (!ZF) goto L1F3C;
L1F34:
    /* 1F34  add     bp,0Eh */
    BP = (uint16_t)(BP + 0xE);
L1F37:
    /* 1F37  call    _seg004_5821 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5821), 0x1F3A)) != 0) return c;
L1F3A:
    /* 1F3A  jmp     short L1F5E */
    goto L1F5E;
L1F3C: /* L1F3C */
    /* 1F3C  mov     bx,word ptr [bp+24h] */
    BX = rw(pSS, BP + 0x24);
L1F3F:
    /* 1F3F  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L1F41:
    /* 1F41  je      short L1F48 */
    if (ZF) goto L1F48;
L1F43:
    /* 1F43  push    bp */
    push16(BP);
L1F44:
    /* 1F44  call    _instance_vvars_head */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x53E9), 0x1F47)) != 0) return c;
L1F47:
    /* 1F47  pop     bp */
    BP = pop16();
L1F48: /* L1F48 */
    /* 1F48  mov     bx,word ptr [bp+20h] */
    BX = rw(pSS, BP + 0x20);
L1F4B:
    /* 1F4B  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L1F4D:
    /* 1F4D  je      short L1F54 */
    if (ZF) goto L1F54;
L1F4F:
    /* 1F4F  push    bp */
    push16(BP);
L1F50:
    /* 1F50  call    _instance_vvars_pitch */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5551), 0x1F53)) != 0) return c;
L1F53:
    /* 1F53  pop     bp */
    BP = pop16();
L1F54: /* L1F54 */
    /* 1F54  mov     bx,word ptr [bp+22h] */
    BX = rw(pSS, BP + 0x22);
L1F57:
    /* 1F57  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L1F59:
    /* 1F59  je      short L1F5E */
    if (ZF) goto L1F5E;
L1F5B:
    /* 1F5B  call    _instance_vvars_bank */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x56B9), 0x1F5E)) != 0) return c;
L1F5E: /* L1F5E */
    /* 1F5E  pop     si */
    SI = pop16();
L1F5F: /* L1F5F */
    /* 1F5F  mov     ax,word ptr ds:[OBJ_X] */
    AX = rw(pDS, 0x2886);
L1F62:
    /* 1F62  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1F63:
    /* 1F63  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1F65:
    /* 1F65  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1F67:
    /* 1F67  mov     bp,word ptr [si-6] */
    BP = rw(pDS, SI + 0xFFFA);
L1F6A:
    /* 1F6A  add     bp,ax */
    BP = add16(BP, AX, 0);
L1F6C:
    /* 1F6C  jo      short L1FAA */
    if (OF) goto L1FAA;
L1F6E:
    /* 1F6E  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L1F71:
    /* 1F71  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1F72:
    /* 1F72  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1F74:
    /* 1F74  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1F76:
    /* 1F76  add     ax,word ptr [si-4] */
    AX = add16(AX, rw(pDS, SI + 0xFFFC), 0);
L1F79:
    /* 1F79  jo      short L1FAA */
    if (OF) goto L1FAA;
L1F7B:
    /* 1F7B  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L1F7D:
    /* 1F7D  mov     ax,word ptr ds:[OBJ_Z] */
    AX = rw(pDS, 0x288A);
L1F80:
    /* 1F80  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1F81:
    /* 1F81  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L1F83:
    /* 1F83  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L1F85:
    /* 1F85  add     ax,word ptr [si-2] */
    AX = add16(AX, rw(pDS, SI + 0xFFFE), 0);
L1F88:
    /* 1F88  jo      short L1FAA */
    if (OF) goto L1FAA;
L1F8A:
    /* 1F8A  or      ax,bp */
    AX = (uint16_t)(AX | BP);
L1F8C:
    /* 1F8C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1F8D:
    /* 1F8D  mov     ax,bx */
    AX = BX;
L1F8F:
    /* 1F8F  mov     bx,1E2h */
    BX = 0x1E2;
L1F92:
    /* 1F92  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L1F94:
    /* 1F94  je      short L1F9B */
    if (ZF) goto L1F9B;
L1F96:
    /* 1F96  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L1F98:
    /* 1F98  xlatb */
    AL = rb(pDS, BX + AL);
L1F99:
    /* 1F99  jmp     short L1F9E */
    goto L1F9E;
L1F9B: /* L1F9B */
    /* 1F9B  xlatb */
    AL = rb(pDS, BX + AL);
L1F9C:
    /* 1F9C  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L1F9E: /* L1F9E */
    /* 1F9E  mov     byte ptr ds:[OBJ_SHIFT],al */
    wb(pDS, 0x2880, AL);
L1FA1:
    /* 1FA1  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x1FA4)) != 0) return c;
L1FA4:
    /* 1FA4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1FA5:
    /* 1FA5  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1FA6:
    /* 1FA6  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L1FAA: /* L1FAA */
    /* 1FAA  mov     ax,word ptr ds:[1C8h] */
    AX = rw(pDS, 0x1C8);
L1FAD:
    /* 1FAD  mov     word ptr ds:[OBJ_X],ax */
    ww(pDS, 0x2886, AX);
L1FB0:
    /* 1FB0  mov     ax,word ptr ds:[1CAh] */
    AX = rw(pDS, 0x1CA);
L1FB3:
    /* 1FB3  mov     word ptr ds:[OBJ_Y],ax */
    ww(pDS, 0x2888, AX);
L1FB6:
    /* 1FB6  mov     ax,word ptr ds:[1CCh] */
    AX = rw(pDS, 0x1CC);
L1FB9:
    /* 1FB9  mov     word ptr ds:[OBJ_Z],ax */
    ww(pDS, 0x288A, AX);
L1FBC:
    /* 1FBC  mov     si,word ptr ds:[1D4h] */
    SI = rw(pDS, 0x1D4);
L1FC0:
    /* 1FC0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1FC1:
    /* 1FC1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1FC2:
    /* 1FC2  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
}
