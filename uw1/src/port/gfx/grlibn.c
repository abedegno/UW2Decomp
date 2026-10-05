/* grlibn.c: replaces src/gfx/GRLIBN.ASM (seg003_6350, 6350..656D of its
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

uint32_t asm_mod_GRLIBN(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x6350: goto L6350;
    case 0x6353: goto L6353;
    case 0x6356: goto L6356;
    case 0x6358: goto L6358;
    case 0x635A: goto L635A;
    case 0x635C: goto L635C;
    case 0x635E: goto L635E;
    case 0x6360: goto L6360;
    case 0x6361: goto L6361;
    case 0x6362: goto L6362;
    case 0x6363: goto L6363;
    case 0x6366: goto L6366;
    case 0x636A: goto L636A;
    case 0x636E: goto L636E;
    case 0x6371: goto L6371;
    case 0x6372: goto L6372;
    case 0x6373: goto L6373;
    case 0x6376: goto L6376;
    case 0x6378: goto L6378;
    case 0x637A: goto L637A;
    case 0x637C: goto L637C;
    case 0x637E: goto L637E;
    case 0x6380: goto L6380;
    case 0x6382: goto L6382;
    case 0x6383: goto L6383;
    case 0x6385: goto L6385;
    case 0x638B: goto L638B;
    case 0x6391: goto L6391;
    case 0x6397: goto L6397;
    case 0x639A: goto L639A;
    case 0x639D: goto L639D;
    case 0x639F: goto L639F;
    case 0x63A3: goto L63A3;
    case 0x63A6: goto L63A6;
    case 0x63AC: goto L63AC;
    case 0x63AF: goto L63AF;
    case 0x63B2: goto L63B2;
    case 0x63B4: goto L63B4;
    case 0x63B8: goto L63B8;
    case 0x63BB: goto L63BB;
    case 0x63BF: goto L63BF;
    case 0x63C3: goto L63C3;
    case 0x63C6: goto L63C6;
    case 0x63C7: goto L63C7;
    case 0x63C8: goto L63C8;
    case 0x63CB: goto L63CB;
    case 0x63CD: goto L63CD;
    case 0x63CF: goto L63CF;
    case 0x63D1: goto L63D1;
    case 0x63D3: goto L63D3;
    case 0x63D5: goto L63D5;
    case 0x63D7: goto L63D7;
    case 0x63D8: goto L63D8;
    case 0x63DA: goto L63DA;
    case 0x63E0: goto L63E0;
    case 0x63E6: goto L63E6;
    case 0x63EC: goto L63EC;
    case 0x63EF: goto L63EF;
    case 0x63F2: goto L63F2;
    case 0x63F4: goto L63F4;
    case 0x63F8: goto L63F8;
    case 0x63FB: goto L63FB;
    case 0x6401: goto L6401;
    case 0x6404: goto L6404;
    case 0x6407: goto L6407;
    case 0x6409: goto L6409;
    case 0x640D: goto L640D;
    case 0x6410: goto L6410;
    case 0x6412: goto L6412;
    case 0x6413: goto L6413;
    case 0x6414: goto L6414;
    case 0x6415: goto L6415;
    case 0x6416: goto L6416;
    case 0x6418: goto L6418;
    case 0x6419: goto L6419;
    case 0x641D: goto L641D;
    case 0x6421: goto L6421;
    case 0x6425: goto L6425;
    case 0x6429: goto L6429;
    case 0x642E: goto L642E;
    case 0x6430: goto L6430;
    case 0x6434: goto L6434;
    case 0x6435: goto L6435;
    case 0x6437: goto L6437;
    case 0x6439: goto L6439;
    case 0x643C: goto L643C;
    case 0x643F: goto L643F;
    case 0x6440: goto L6440;
    case 0x6444: goto L6444;
    case 0x6446: goto L6446;
    case 0x6448: goto L6448;
    case 0x644B: goto L644B;
    case 0x644F: goto L644F;
    case 0x6453: goto L6453;
    case 0x6455: goto L6455;
    case 0x6459: goto L6459;
    case 0x645A: goto L645A;
    case 0x645B: goto L645B;
    case 0x645C: goto L645C;
    case 0x645D: goto L645D;
    case 0x645E: goto L645E;
    case 0x645F: goto L645F;
    case 0x6460: goto L6460;
    case 0x6461: goto L6461;
    case 0x6464: goto L6464;
    case 0x6468: goto L6468;
    case 0x646E: goto L646E;
    case 0x6472: goto L6472;
    case 0x6473: goto L6473;
    case 0x6474: goto L6474;
    case 0x6475: goto L6475;
    case 0x6478: goto L6478;
    case 0x647C: goto L647C;
    case 0x6482: goto L6482;
    case 0x6486: goto L6486;
    case 0x648C: goto L648C;
    case 0x6490: goto L6490;
    case 0x6494: goto L6494;
    case 0x6497: goto L6497;
    case 0x6498: goto L6498;
    case 0x649C: goto L649C;
    case 0x649D: goto L649D;
    case 0x649F: goto L649F;
    case 0x64A0: goto L64A0;
    case 0x64A1: goto L64A1;
    case 0x64A2: goto L64A2;
    case 0x64A3: goto L64A3;
    case 0x64A4: goto L64A4;
    case 0x64A7: goto L64A7;
    case 0x64A8: goto L64A8;
    case 0x64A9: goto L64A9;
    case 0x64AA: goto L64AA;
    case 0x64AD: goto L64AD;
    case 0x64AF: goto L64AF;
    case 0x64B1: goto L64B1;
    case 0x64B3: goto L64B3;
    case 0x64B5: goto L64B5;
    case 0x64B7: goto L64B7;
    case 0x64B8: goto L64B8;
    case 0x64B9: goto L64B9;
    case 0x64BB: goto L64BB;
    case 0x64BD: goto L64BD;
    case 0x64BF: goto L64BF;
    case 0x64C3: goto L64C3;
    case 0x64C5: goto L64C5;
    case 0x64C9: goto L64C9;
    case 0x64CB: goto L64CB;
    case 0x64CF: goto L64CF;
    case 0x64D0: goto L64D0;
    case 0x64D1: goto L64D1;
    case 0x64D2: goto L64D2;
    case 0x64D3: goto L64D3;
    case 0x64D6: goto L64D6;
    case 0x64D9: goto L64D9;
    case 0x64DB: goto L64DB;
    case 0x64DD: goto L64DD;
    case 0x64E0: goto L64E0;
    case 0x64E1: goto L64E1;
    case 0x64E2: goto L64E2;
    case 0x64E3: goto L64E3;
    case 0x64E4: goto L64E4;
    case 0x64E6: goto L64E6;
    case 0x64E8: goto L64E8;
    case 0x64EC: goto L64EC;
    case 0x64F0: goto L64F0;
    case 0x64F5: goto L64F5;
    case 0x64F6: goto L64F6;
    case 0x64F7: goto L64F7;
    case 0x64F8: goto L64F8;
    case 0x64F9: goto L64F9;
    case 0x64FC: goto L64FC;
    case 0x64FD: goto L64FD;
    case 0x64FE: goto L64FE;
    case 0x64FF: goto L64FF;
    case 0x6502: goto L6502;
    case 0x6503: goto L6503;
    case 0x6505: goto L6505;
    case 0x6507: goto L6507;
    case 0x6509: goto L6509;
    case 0x650B: goto L650B;
    case 0x650D: goto L650D;
    case 0x650F: goto L650F;
    case 0x6510: goto L6510;
    case 0x6511: goto L6511;
    case 0x6513: goto L6513;
    case 0x6515: goto L6515;
    case 0x6517: goto L6517;
    case 0x651B: goto L651B;
    case 0x651D: goto L651D;
    case 0x6521: goto L6521;
    case 0x6523: goto L6523;
    case 0x6527: goto L6527;
    case 0x6528: goto L6528;
    case 0x652C: goto L652C;
    case 0x652D: goto L652D;
    case 0x652E: goto L652E;
    case 0x652F: goto L652F;
    case 0x6530: goto L6530;
    case 0x6533: goto L6533;
    case 0x6536: goto L6536;
    case 0x6538: goto L6538;
    case 0x653A: goto L653A;
    case 0x653D: goto L653D;
    case 0x653E: goto L653E;
    case 0x653F: goto L653F;
    case 0x6540: goto L6540;
    case 0x6541: goto L6541;
    case 0x6543: goto L6543;
    case 0x6545: goto L6545;
    case 0x6547: goto L6547;
    case 0x654A: goto L654A;
    case 0x654D: goto L654D;
    case 0x6550: goto L6550;
    case 0x6551: goto L6551;
    case 0x6552: goto L6552;
    case 0x6553: goto L6553;
    case 0x6555: goto L6555;
    case 0x6558: goto L6558;
    case 0x6559: goto L6559;
    case 0x655B: goto L655B;
    case 0x655C: goto L655C;
    case 0x655D: goto L655D;
    case 0x655F: goto L655F;
    case 0x6561: goto L6561;
    case 0x6563: goto L6563;
    case 0x6566: goto L6566;
    case 0x6569: goto L6569;
    case 0x656B: goto L656B;
    case 0x656C: goto L656C;
    default: asm_bad_entry("GRLIBN.ASM", entry);
    }

    /* seg003_6350  (+6350)
       s_shclip: attach intensities (_6547), close the list, then clip top and bottom (skipped when
       every vertex is inside them) and left and right. L6416 is one pass; L649C and L64F0 work out
       an intersection with a horizontal or vertical edge (imul/idiv, both terms halved on overflow)
       and nudge it one pixel inward unless it lies on the edge. */
L6350: /* _seg003_6350 */
    /* 6350  call    _seg003_6547 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x6547), 0x6353)) != 0) return c;
L6353:
    /* 6353  mov     si,415Ch */
    SI = 0x415C;
L6356:
    /* 6356  mov     di,cx */
    DI = CX;
L6358:
    /* 6358  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L635A:
    /* 635A  add     di,cx */
    DI = (uint16_t)(DI + CX);
L635C:
    /* 635C  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L635E:
    /* 635E  add     di,si */
    DI = (uint16_t)(DI + SI);
L6360:
    /* 6360  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6361:
    /* 6361  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6362:
    /* 6362  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6363:
    /* 6363  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L6366:
    /* 6366  mov     bx,word ptr ds:[WIN_TOP] */
    BX = rw(pDS, 0x3DF4);
L636A:
    /* 636A  mov     dx,word ptr ds:[WIN_BOTTOM] */
    DX = rw(pDS, 0x3DF8);
L636E:
    /* 636E  mov     si,415Eh */
    SI = 0x415E;
L6371:
    /* 6371  push    cx */
    push16(CX);
L6372: /* L6372 */
    /* 6372  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6373:
    /* 6373  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L6376:
    /* 6376  cmp     ax,bx */
    sub16(AX, BX, 0);
L6378:
    /* 6378  jg      L6380 */
    if (!ZF && SF == OF) goto L6380;
L637A:
    /* 637A  cmp     ax,dx */
    sub16(AX, DX, 0);
L637C:
    /* 637C  jl      L6380 */
    if (SF != OF) goto L6380;
L637E:
    /* 637E  loop    L6372 */
    if (--CX) goto L6372;
L6380: /* L6380 */
    /* 6380  test    cx,cx */
    logic16((uint16_t)(CX & CX));
L6382:
    /* 6382  pop     cx */
    CX = pop16();
L6383:
    /* 6383  je      L63BB */
    if (ZF) goto L63BB;
L6385:
    /* 6385  mov     word ptr ds:[4480h],2 */
    ww(pDS, 0x4480, 0x2);
L638B:
    /* 638B  mov     word ptr ds:[4482h],0Ah */
    ww(pDS, 0x4482, 0xA);
L6391:
    /* 6391  mov     word ptr ds:[4484h],offset L64F0 */
    ww(pDS, 0x4484, 0x64F0);
L6397:
    /* 6397  mov     si,415Ch */
    SI = 0x415C;
L639A:
    /* 639A  mov     di,42ECh */
    DI = 0x42EC;
L639D:
    /* 639D  mov     al,7Ch */
    AL = 0x7C;
L639F:
    /* 639F  mov     dx,word ptr ds:[WIN_TOP] */
    DX = rw(pDS, 0x3DF4);
L63A3:
    /* 63A3  call    L6416 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x6416), 0x63A6)) != 0) return c;
L63A6:
    /* 63A6  mov     word ptr ds:[4484h],offset L64EC */
    ww(pDS, 0x4484, 0x64EC);
L63AC:
    /* 63AC  mov     si,42ECh */
    SI = 0x42EC;
L63AF:
    /* 63AF  mov     di,415Ch */
    DI = 0x415C;
L63B2:
    /* 63B2  mov     al,7Fh */
    AL = 0x7F;
L63B4:
    /* 63B4  mov     dx,word ptr ds:[WIN_BOTTOM] */
    DX = rw(pDS, 0x3DF8);
L63B8:
    /* 63B8  call    L6416 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x6416), 0x63BB)) != 0) return c;
L63BB: /* L63BB */
    /* 63BB  mov     bx,word ptr ds:[WIN_RIGHT] */
    BX = rw(pDS, 0x3DF6);
L63BF:
    /* 63BF  mov     dx,word ptr ds:[WIN_LEFT] */
    DX = rw(pDS, 0x3DF2);
L63C3:
    /* 63C3  mov     si,415Ch */
    SI = 0x415C;
L63C6:
    /* 63C6  push    cx */
    push16(CX);
L63C7: /* L63C7 */
    /* 63C7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L63C8:
    /* 63C8  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L63CB:
    /* 63CB  cmp     ax,bx */
    sub16(AX, BX, 0);
L63CD:
    /* 63CD  jg      L63D5 */
    if (!ZF && SF == OF) goto L63D5;
L63CF:
    /* 63CF  cmp     ax,dx */
    sub16(AX, DX, 0);
L63D1:
    /* 63D1  jl      L63D5 */
    if (SF != OF) goto L63D5;
L63D3:
    /* 63D3  loop    L63C7 */
    if (--CX) goto L63C7;
L63D5: /* L63D5 */
    /* 63D5  test    cx,cx */
    logic16((uint16_t)(CX & CX));
L63D7:
    /* 63D7  pop     cx */
    CX = pop16();
L63D8:
    /* 63D8  je      L6410 */
    if (ZF) goto L6410;
L63DA:
    /* 63DA  mov     word ptr ds:[4480h],0 */
    ww(pDS, 0x4480, 0x0);
L63E0:
    /* 63E0  mov     word ptr ds:[4482h],8 */
    ww(pDS, 0x4482, 0x8);
L63E6:
    /* 63E6  mov     word ptr ds:[4484h],offset L649C */
    ww(pDS, 0x4484, 0x649C);
L63EC:
    /* 63EC  mov     si,415Ch */
    SI = 0x415C;
L63EF:
    /* 63EF  mov     di,42ECh */
    DI = 0x42EC;
L63F2:
    /* 63F2  mov     al,7Fh */
    AL = 0x7F;
L63F4:
    /* 63F4  mov     dx,word ptr ds:[WIN_LEFT] */
    DX = rw(pDS, 0x3DF2);
L63F8:
    /* 63F8  call    L6416 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x6416), 0x63FB)) != 0) return c;
L63FB:
    /* 63FB  mov     word ptr ds:[4484h],offset L6498 */
    ww(pDS, 0x4484, 0x6498);
L6401:
    /* 6401  mov     si,42ECh */
    SI = 0x42EC;
L6404:
    /* 6404  mov     di,415Ch */
    DI = 0x415C;
L6407:
    /* 6407  mov     al,7Ch */
    AL = 0x7C;
L6409:
    /* 6409  mov     dx,word ptr ds:[WIN_RIGHT] */
    DX = rw(pDS, 0x3DF6);
L640D:
    /* 640D  call    L6416 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x6416), 0x6410)) != 0) return c;
L6410: /* L6410 */
    /* 6410  jcxz    L6414 */
    if (!CX) goto L6414;
L6412:
    /* 6412  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L6413: /* L6413 */
    /* 6413  pop     ax */
    AX = pop16();
L6414: /* L6414 */
    /* 6414  pop     ax */
    AX = pop16();
L6415:
    /* 6415  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L6416: /* L6416 */
    /* 6416  jcxz    L6413 */
    if (!CX) goto L6413;
L6418:
    /* 6418  push    di */
    push16(DI);
L6419:
    /* 6419  mov     byte ptr cs:L6437,al */
    wb(CODE003, 0x6437, AL);
L641D:
    /* 641D  mov     byte ptr cs:L6446,al */
    wb(CODE003, 0x6446, AL);
L6421:
    /* 6421  mov     word ptr ds:[7D1h],cx */
    ww(pDS, 0x7D1, CX);
L6425:
    /* 6425  mov     word ptr ds:[447Ch],cx */
    ww(pDS, 0x447C, CX);
L6429:
    /* 6429  db      0EAh */
    return ASM_JMP(0x0090, 0x642E);
L642E: /* L642E */
    /* 642E  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L6430:
    /* 6430  add     si,word ptr ds:[4480h] */
    SI = (uint16_t)(SI + rw(pDS, 0x4480));
L6434:
    /* 6434  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6435:
    /* 6435  cmp     ax,dx */
    sub16(AX, DX, 0);
L6437: /* L6437 */
    /* 6437  jg      L643C */
    if (asm_jcc(CODE003[0x6437])) goto L643C;
L6439:
    /* 6439  mov     bx,4 */
    BX = 0x4;
L643C: /* L643C */
    /* 643C  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L643F:
    /* 643F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6440:
    /* 6440  sub     si,word ptr ds:[4482h] */
    SI = (uint16_t)(SI - rw(pDS, 0x4482));
L6444:
    /* 6444  cmp     ax,dx */
    sub16(AX, DX, 0);
L6446: /* L6446 */
    /* 6446  jg      L644B */
    if (asm_jcc(CODE003[0x6446])) goto L644B;
L6448:
    /* 6448  or      bx,2 */
    BX = (uint16_t)(BX | 0x2);
L644B: /* L644B */
    /* 644B  call    word ptr [bx+5E74h] */
    if ((c = asm_call(ASM_JMP(0x0090, rw(pDS, BX + 0x5E74)), 0x644F)) != 0) return c;
L644F:
    /* 644F  dec     word ptr ds:[7D1h] */
    ww(pDS, 0x7D1, dec16(rw(pDS, 0x7D1)));
L6453:
    /* 6453  jg      L642E */
    if (!ZF && SF == OF) goto L642E;
L6455:
    /* 6455  mov     cx,word ptr ds:[447Ch] */
    CX = rw(pDS, 0x447C);
L6459:
    /* 6459  pop     si */
    SI = pop16();
L645A:
    /* 645A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L645B:
    /* 645B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L645C:
    /* 645C  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L645D:
    /* 645D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L645E:
    /* 645E  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L645F:
    /* 645F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6460:
    /* 6460  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6461:
    /* 6461  sub     si,6 */
    SI = sub16(SI, 0x6, 0);
L6464:
    /* 6464  inc     word ptr ds:[447Ch] */
    ww(pDS, 0x447C, inc16(rw(pDS, 0x447C)));
L6468:
    /* 6468  mov     word ptr ds:[4486h],0 */
    ww(pDS, 0x4486, 0x0);
L646E:
    /* 646E  jmp     word ptr ds:[4484h] */
    return ASM_JMP(0x0090, rw(pDS, 0x4484));
L6472:
    /* 6472  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6473:
    /* 6473  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6474:
    /* 6474  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6475:
    /* 6475  sub     si,6 */
    SI = sub16(SI, 0x6, 0);
L6478:
    /* 6478  inc     word ptr ds:[447Ch] */
    ww(pDS, 0x447C, inc16(rw(pDS, 0x447C)));
L647C:
    /* 647C  mov     word ptr ds:[4486h],0 */
    ww(pDS, 0x4486, 0x0);
L6482:
    /* 6482  jmp     word ptr ds:[4484h] */
    return ASM_JMP(0x0090, rw(pDS, 0x4484));
L6486:
    /* 6486  mov     word ptr ds:[4486h],1 */
    ww(pDS, 0x4486, 0x1);
L648C:
    /* 648C  jmp     word ptr ds:[4484h] */
    return ASM_JMP(0x0090, rw(pDS, 0x4484));
L6490:
    /* 6490  dec     word ptr ds:[447Ch] */
    ww(pDS, 0x447C, (uint16_t)(rw(pDS, 0x447C) - 1));
L6494:
    /* 6494  add     si,6 */
    SI = add16(SI, 0x6, 0);
L6497:
    /* 6497  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L6498: /* L6498 */
    /* 6498  neg     word ptr ds:[4486h] */
    ww(pDS, 0x4486, (uint16_t)-rw(pDS, 0x4486));
L649C: /* L649C */
    /* 649C  push    dx */
    push16(DX);
L649D:
    /* 649D  mov     ax,dx */
    AX = DX;
L649F:
    /* 649F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L64A0:
    /* 64A0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L64A1:
    /* 64A1  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L64A2:
    /* 64A2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L64A3:
    /* 64A3  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L64A4:
    /* 64A4  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L64A7:
    /* 64A7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L64A8:
    /* 64A8  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L64A9:
    /* 64A9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L64AA:
    /* 64AA  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L64AD:
    /* 64AD  sub     ax,bx */
    AX = (uint16_t)(AX - BX);
L64AF:
    /* 64AF  sub     dx,cx */
    DX = (uint16_t)(DX - CX);
L64B1:
    /* 64B1  neg     cx */
    CX = (uint16_t)-CX;
L64B3:
    /* 64B3  add     cx,bp */
    CX = add16(CX, BP, 0);
L64B5:
    /* 64B5  jo      L64E4 */
    if (OF) goto L64E4;
L64B7: /* L64B7 */
    /* 64B7  push    dx */
    push16(DX);
L64B8:
    /* 64B8  push    cx */
    push16(CX);
L64B9:
    /* 64B9  imul    dx */
    imul16(DX);
L64BB:
    /* 64BB  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x64BB, 2)) != 0) return c;
L64BD:
    /* 64BD  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L64BF:
    /* 64BF  cmp     ax,word ptr ds:[WIN_TOP] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L64C3:
    /* 64C3  jge     L64CF */
    if (SF == OF) goto L64CF;
L64C5:
    /* 64C5  cmp     ax,word ptr ds:[WIN_BOTTOM] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L64C9:
    /* 64C9  jle     L64CF */
    if (ZF || SF != OF) goto L64CF;
L64CB:
    /* 64CB  add     ax,word ptr ds:[4486h] */
    AX = (uint16_t)(AX + rw(pDS, 0x4486));
L64CF: /* L64CF */
    /* 64CF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L64D0:
    /* 64D0  pop     cx */
    CX = pop16();
L64D1:
    /* 64D1  pop     dx */
    DX = pop16();
L64D2:
    /* 64D2  push    ax */
    push16(AX);
L64D3:
    /* 64D3  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L64D6:
    /* 64D6  sub     ax,word ptr [si-2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFE));
L64D9:
    /* 64D9  imul    dx */
    imul16(DX);
L64DB:
    /* 64DB  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x64DB, 2)) != 0) return c;
L64DD:
    /* 64DD  add     ax,word ptr [si-2] */
    AX = add16(AX, rw(pDS, SI + 0xFFFE), 0);
L64E0:
    /* 64E0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L64E1:
    /* 64E1  pop     ax */
    AX = pop16();
L64E2:
    /* 64E2  pop     dx */
    DX = pop16();
L64E3:
    /* 64E3  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L64E4: /* L64E4 */
    /* 64E4  rcr     cx,1 */
    CX = rcr16(CX, 1);
L64E6:
    /* 64E6  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L64E8:
    /* 64E8  jmp     L64B7 */
    goto L64B7;
L64EC: /* L64EC */
    /* 64EC  neg     word ptr ds:[4486h] */
    ww(pDS, 0x4486, (uint16_t)-rw(pDS, 0x4486));
L64F0: /* L64F0 */
    /* 64F0  mov     word ptr cs:L64EA,dx */
    ww(CODE003, 0x64EA, DX);
L64F5:
    /* 64F5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L64F6:
    /* 64F6  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L64F7:
    /* 64F7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L64F8:
    /* 64F8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L64F9:
    /* 64F9  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L64FC:
    /* 64FC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L64FD:
    /* 64FD  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L64FE:
    /* 64FE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L64FF:
    /* 64FF  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L6502:
    /* 6502  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L6503:
    /* 6503  xchg    bx,cx */
    { uint16_t t_ = CX;
    CX = BX;
    BX = t_; }
L6505:
    /* 6505  sub     ax,bx */
    AX = (uint16_t)(AX - BX);
L6507:
    /* 6507  sub     dx,cx */
    DX = (uint16_t)(DX - CX);
L6509:
    /* 6509  neg     cx */
    CX = (uint16_t)-CX;
L650B:
    /* 650B  add     cx,bp */
    CX = add16(CX, BP, 0);
L650D:
    /* 650D  jo      L6541 */
    if (OF) goto L6541;
L650F: /* L650F */
    /* 650F  push    dx */
    push16(DX);
L6510:
    /* 6510  push    cx */
    push16(CX);
L6511:
    /* 6511  imul    dx */
    imul16(DX);
L6513:
    /* 6513  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x6513, 2)) != 0) return c;
L6515:
    /* 6515  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L6517:
    /* 6517  cmp     ax,word ptr ds:[WIN_LEFT] */
    sub16(AX, rw(pDS, 0x3DF2), 0);
L651B:
    /* 651B  jle     L6527 */
    if (ZF || SF != OF) goto L6527;
L651D:
    /* 651D  cmp     ax,word ptr ds:[WIN_RIGHT] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L6521:
    /* 6521  jge     L6527 */
    if (SF == OF) goto L6527;
L6523:
    /* 6523  add     ax,word ptr ds:[4486h] */
    AX = (uint16_t)(AX + rw(pDS, 0x4486));
L6527: /* L6527 */
    /* 6527  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6528:
    /* 6528  mov     ax,word ptr cs:L64EA */
    AX = rw(CODE003, 0x64EA);
L652C:
    /* 652C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L652D:
    /* 652D  pop     cx */
    CX = pop16();
L652E:
    /* 652E  pop     dx */
    DX = pop16();
L652F:
    /* 652F  push    ax */
    push16(AX);
L6530:
    /* 6530  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L6533:
    /* 6533  sub     ax,word ptr [si-2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFE));
L6536:
    /* 6536  imul    dx */
    imul16(DX);
L6538:
    /* 6538  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x6538, 2)) != 0) return c;
L653A:
    /* 653A  add     ax,word ptr [si-2] */
    AX = add16(AX, rw(pDS, SI + 0xFFFE), 0);
L653D:
    /* 653D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L653E:
    /* 653E  pop     ax */
    AX = pop16();
L653F:
    /* 653F  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L6540:
    /* 6540  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L6541: /* L6541 */
    /* 6541  rcr     cx,1 */
    CX = rcr16(CX, 1);
L6543:
    /* 6543  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L6545:
    /* 6545  jmp     L650F */
    goto L650F;

    /* seg003_6547  (+6547)
       _6547 (probably FM Towns' pack_vertices, which comes next to s_shclip there): turn CX (x, y)
       vertices at 415C into (x, y, intensity) ones, taking the intensities in order from the word
       table at 55EC (assembled in 42EC and copied back). */
L6547: /* _seg003_6547 */
    /* 6547  mov     si,415Ch */
    SI = 0x415C;
L654A:
    /* 654A  mov     bx,55ECh */
    BX = 0x55EC;
L654D:
    /* 654D  mov     di,42ECh */
    DI = 0x42EC;
L6550:
    /* 6550  push    cx */
    push16(CX);
L6551: /* L6551 */
    /* 6551  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6552:
    /* 6552  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6553:
    /* 6553  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L6555:
    /* 6555  add     bx,2 */
    BX = (uint16_t)(BX + 0x2);
L6558:
    /* 6558  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6559:
    /* 6559  loop    L6551 */
    if (--CX) goto L6551;
L655B:
    /* 655B  pop     cx */
    CX = pop16();
L655C:
    /* 655C  push    cx */
    push16(CX);
L655D:
    /* 655D  mov     ax,cx */
    AX = CX;
L655F:
    /* 655F  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L6561:
    /* 6561  add     cx,ax */
    CX = add16(CX, AX, 0);
L6563:
    /* 6563  mov     si,42ECh */
    SI = 0x42EC;
L6566:
    /* 6566  mov     di,415Ch */
    DI = 0x415C;
L6569:
    /* 6569  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L656B:
    /* 656B  pop     cx */
    CX = pop16();
L656C:
    /* 656C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
