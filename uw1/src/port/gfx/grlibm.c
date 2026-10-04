/* grlibm.c: replaces src/gfx/GRLIBM.ASM (seg003_6134, 6134..634F of its
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

uint32_t asm_mod_GRLIBM(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x6134: goto L6134;
    case 0x6136: goto L6136;
    case 0x6139: goto L6139;
    case 0x613C: goto L613C;
    case 0x613F: goto L613F;
    case 0x6143: goto L6143;
    case 0x6146: goto L6146;
    case 0x6149: goto L6149;
    case 0x614B: goto L614B;
    case 0x614E: goto L614E;
    case 0x6150: goto L6150;
    case 0x6153: goto L6153;
    case 0x6155: goto L6155;
    case 0x6156: goto L6156;
    case 0x6159: goto L6159;
    case 0x615C: goto L615C;
    case 0x615E: goto L615E;
    case 0x6160: goto L6160;
    case 0x6162: goto L6162;
    case 0x6164: goto L6164;
    case 0x6167: goto L6167;
    case 0x6169: goto L6169;
    case 0x616A: goto L616A;
    case 0x616B: goto L616B;
    case 0x616C: goto L616C;
    case 0x6170: goto L6170;
    case 0x6173: goto L6173;
    case 0x6176: goto L6176;
    case 0x6179: goto L6179;
    case 0x617D: goto L617D;
    case 0x617F: goto L617F;
    case 0x6183: goto L6183;
    case 0x6184: goto L6184;
    case 0x6187: goto L6187;
    case 0x6189: goto L6189;
    case 0x618B: goto L618B;
    case 0x618D: goto L618D;
    case 0x6190: goto L6190;
    case 0x6193: goto L6193;
    case 0x6195: goto L6195;
    case 0x6197: goto L6197;
    case 0x6199: goto L6199;
    case 0x619B: goto L619B;
    case 0x619F: goto L619F;
    case 0x61A3: goto L61A3;
    case 0x61A5: goto L61A5;
    case 0x61A7: goto L61A7;
    case 0x61AB: goto L61AB;
    case 0x61AD: goto L61AD;
    case 0x61AF: goto L61AF;
    case 0x61B1: goto L61B1;
    case 0x61B5: goto L61B5;
    case 0x61B7: goto L61B7;
    case 0x61BB: goto L61BB;
    case 0x61BD: goto L61BD;
    case 0x61C0: goto L61C0;
    case 0x61C3: goto L61C3;
    case 0x61C6: goto L61C6;
    case 0x61C9: goto L61C9;
    case 0x61CC: goto L61CC;
    case 0x61CE: goto L61CE;
    case 0x61D0: goto L61D0;
    case 0x61D1: goto L61D1;
    case 0x61D4: goto L61D4;
    case 0x61D7: goto L61D7;
    case 0x61D9: goto L61D9;
    case 0x61DC: goto L61DC;
    case 0x61DF: goto L61DF;
    case 0x61E1: goto L61E1;
    case 0x61E4: goto L61E4;
    case 0x61E5: goto L61E5;
    case 0x61E6: goto L61E6;
    case 0x61E7: goto L61E7;
    case 0x61EA: goto L61EA;
    case 0x61EB: goto L61EB;
    case 0x61ED: goto L61ED;
    case 0x61F1: goto L61F1;
    case 0x61F4: goto L61F4;
    case 0x61F6: goto L61F6;
    case 0x61F9: goto L61F9;
    case 0x61FC: goto L61FC;
    case 0x6200: goto L6200;
    case 0x6202: goto L6202;
    case 0x6204: goto L6204;
    case 0x6206: goto L6206;
    case 0x6209: goto L6209;
    case 0x620B: goto L620B;
    case 0x620D: goto L620D;
    case 0x620F: goto L620F;
    case 0x6211: goto L6211;
    case 0x6214: goto L6214;
    case 0x6218: goto L6218;
    case 0x621B: goto L621B;
    case 0x621F: goto L621F;
    case 0x6222: goto L6222;
    case 0x6226: goto L6226;
    case 0x6228: goto L6228;
    case 0x6229: goto L6229;
    case 0x622A: goto L622A;
    case 0x622C: goto L622C;
    case 0x622F: goto L622F;
    case 0x6231: goto L6231;
    case 0x6233: goto L6233;
    case 0x6235: goto L6235;
    case 0x6237: goto L6237;
    case 0x6238: goto L6238;
    case 0x623A: goto L623A;
    case 0x623C: goto L623C;
    case 0x623F: goto L623F;
    case 0x6243: goto L6243;
    case 0x6245: goto L6245;
    case 0x6246: goto L6246;
    case 0x6248: goto L6248;
    case 0x624B: goto L624B;
    case 0x624D: goto L624D;
    case 0x624F: goto L624F;
    case 0x6251: goto L6251;
    case 0x6253: goto L6253;
    case 0x6254: goto L6254;
    case 0x6256: goto L6256;
    case 0x6258: goto L6258;
    case 0x625B: goto L625B;
    case 0x625F: goto L625F;
    case 0x6265: goto L6265;
    case 0x626B: goto L626B;
    case 0x626F: goto L626F;
    case 0x6273: goto L6273;
    case 0x6275: goto L6275;
    case 0x6279: goto L6279;
    case 0x627B: goto L627B;
    case 0x627D: goto L627D;
    case 0x627F: goto L627F;
    case 0x6283: goto L6283;
    case 0x6285: goto L6285;
    case 0x6289: goto L6289;
    case 0x628A: goto L628A;
    case 0x628D: goto L628D;
    case 0x628E: goto L628E;
    case 0x6292: goto L6292;
    case 0x6296: goto L6296;
    case 0x6299: goto L6299;
    case 0x629D: goto L629D;
    case 0x62A1: goto L62A1;
    case 0x62A3: goto L62A3;
    case 0x62A4: goto L62A4;
    case 0x62A8: goto L62A8;
    case 0x62AA: goto L62AA;
    case 0x62AE: goto L62AE;
    case 0x62B2: goto L62B2;
    case 0x62B5: goto L62B5;
    case 0x62B7: goto L62B7;
    case 0x62BA: goto L62BA;
    case 0x62BD: goto L62BD;
    case 0x62C0: goto L62C0;
    case 0x62C2: goto L62C2;
    case 0x62C5: goto L62C5;
    case 0x62C8: goto L62C8;
    case 0x62CB: goto L62CB;
    case 0x62CF: goto L62CF;
    case 0x62D1: goto L62D1;
    case 0x62D2: goto L62D2;
    case 0x62D3: goto L62D3;
    case 0x62D7: goto L62D7;
    case 0x62D9: goto L62D9;
    case 0x62DC: goto L62DC;
    case 0x62E0: goto L62E0;
    case 0x62E4: goto L62E4;
    case 0x62E6: goto L62E6;
    case 0x62E8: goto L62E8;
    case 0x62EC: goto L62EC;
    case 0x62F0: goto L62F0;
    case 0x62F2: goto L62F2;
    case 0x62F6: goto L62F6;
    case 0x62F9: goto L62F9;
    case 0x62FD: goto L62FD;
    case 0x6300: goto L6300;
    case 0x6301: goto L6301;
    case 0x6304: goto L6304;
    case 0x6305: goto L6305;
    case 0x6309: goto L6309;
    case 0x630A: goto L630A;
    case 0x630C: goto L630C;
    case 0x6310: goto L6310;
    case 0x6312: goto L6312;
    case 0x6314: goto L6314;
    case 0x6316: goto L6316;
    case 0x631A: goto L631A;
    case 0x631C: goto L631C;
    case 0x6320: goto L6320;
    case 0x6322: goto L6322;
    case 0x6325: goto L6325;
    case 0x6327: goto L6327;
    case 0x632A: goto L632A;
    case 0x632D: goto L632D;
    case 0x632F: goto L632F;
    case 0x6331: goto L6331;
    case 0x6332: goto L6332;
    case 0x6334: goto L6334;
    case 0x6337: goto L6337;
    case 0x6339: goto L6339;
    case 0x633C: goto L633C;
    case 0x633F: goto L633F;
    case 0x6340: goto L6340;
    case 0x6342: goto L6342;
    case 0x6345: goto L6345;
    case 0x6347: goto L6347;
    case 0x634A: goto L634A;
    case 0x634D: goto L634D;
    default: asm_bad_entry("GRLIBM.ASM", entry);
    }
L6134: /* L6134 */
    /* 6134  jne     L613C */
    if (!ZF) goto L613C;
L6136:
    /* 6136  mov     si,415Ch */
    SI = 0x415C;
L6139:
    /* 6139  jmp     _seg003_3F06 */
    return ASM_JMP(0x0090, 0x3F06);
L613C: /* L613C */
    /* 613C  mov     ax,word ptr ds:[VERTS] */
    AX = rw(pDS, 0x415C);
L613F:
    /* 613F  mov     bx,word ptr ds:[415Eh] */
    BX = rw(pDS, 0x415E);
L6143:
    /* 6143  jmp     _seg003_3B34 */
    return ASM_JMP(0x0090, 0x3B34);
L6146: /* _seg003_6146 */
    /* 6146  cmp     cx,63h */
    sub16(CX, 0x63, 0);
L6149:
    /* 6149  ja      L6155 */
    if (!CF && !ZF) goto L6155;
L614B:
    /* 614B  cmp     cx,2 */
    sub16(CX, 0x2, 0);
L614E:
    /* 614E  jbe     L6134 */
    if (CF || ZF) goto L6134;
L6150:
    /* 6150  call    _seg003_6350 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x6350), 0x6153)) != 0) return c;
L6153:
    /* 6153  jmp     short _seg003_6159 */
    goto L6159;
L6155: /* L6155 */
    /* 6155  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L6156: /* _seg003_6156 */
    /* 6156  call    _seg003_6547 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x6547), 0x6159)) != 0) return c;
L6159: /* _seg003_6159 */
    /* 6159  db      0E9h */
    goto L615C;

    /* Fill: close the vertex list by copying the first vertex after the last, find the top and
       bottom, initialise every row record to an empty span (left 3E8h, right 0FC18h), then step each
       edge (horizontal edges are handled at L630A), and finally mark the end of the table (top bit
       of the row after the last) and call _F77. */
L615C: /* L615C */
    /* 615C  mov     di,cx */
    DI = CX;
L615E:
    /* 615E  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L6160:
    /* 6160  add     di,cx */
    DI = (uint16_t)(DI + CX);
L6162:
    /* 6162  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L6164:
    /* 6164  mov     si,415Ch */
    SI = 0x415C;
L6167:
    /* 6167  add     di,si */
    DI = (uint16_t)(DI + SI);
L6169:
    /* 6169  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L616A:
    /* 616A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L616B:
    /* 616B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L616C:
    /* 616C  mov     word ptr ds:[567Eh],cx */
    ww(pDS, 0x567E, CX);
L6170:
    /* 6170  mov     bx,0FC18h */
    BX = 0xFC18;
L6173:
    /* 6173  mov     dx,3E8h */
    DX = 0x3E8;
L6176:
    /* 6176  mov     si,415Eh */
    SI = 0x415E;
L6179: /* L6179 */
    /* 6179  test    word ptr [si],0FFFFh */
    logic16((uint16_t)(rw(pDS, SI) & 0xFFFF));
L617D:
    /* 617D  jns     L6183 */
    if (!SF) goto L6183;
L617F:
    /* 617F  mov     word ptr [si],0 */
    ww(pDS, SI, 0x0);
L6183: /* L6183 */
    /* 6183  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6184:
    /* 6184  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L6187:
    /* 6187  cmp     ax,bx */
    sub16(AX, BX, 0);
L6189:
    /* 6189  jl      L6193 */
    if (SF != OF) goto L6193;
L618B:
    /* 618B  mov     bx,ax */
    BX = AX;
L618D:
    /* 618D  mov     bp,word ptr [si-8] */
    BP = rw(pDS, SI + 0xFFF8);
L6190:
    /* 6190  mov     di,word ptr [si-4] */
    DI = rw(pDS, SI + 0xFFFC);
L6193: /* L6193 */
    /* 6193  cmp     ax,dx */
    sub16(AX, DX, 0);
L6195:
    /* 6195  jg      L6199 */
    if (!ZF && SF == OF) goto L6199;
L6197:
    /* 6197  mov     dx,ax */
    DX = AX;
L6199: /* L6199 */
    /* 6199  loop    L6179 */
    if (--CX) goto L6179;
L619B:
    /* 619B  mov     word ptr ds:[566Eh],bx */
    ww(pDS, 0x566E, BX);
L619F:
    /* 619F  mov     word ptr ds:[566Ch],dx */
    ww(pDS, 0x566C, DX);
L61A3:
    /* 61A3  mov     ax,bx */
    AX = BX;
L61A5:
    /* 61A5  mov     si,bx */
    SI = BX;
L61A7:
    /* 61A7  sub     si,word ptr ds:[566Ch] */
    SI = (uint16_t)(SI - rw(pDS, 0x566C));
L61AB:
    /* 61AB  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L61AD:
    /* 61AD  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L61AF:
    /* 61AF  add     si,bx */
    SI = (uint16_t)(SI + BX);
L61B1:
    /* 61B1  sub     si,word ptr ds:[566Ch] */
    SI = (uint16_t)(SI - rw(pDS, 0x566C));
L61B5:
    /* 61B5  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L61B7:
    /* 61B7  add     si,5690h */
    SI = (uint16_t)(SI + 0x5690);
L61BB:
    /* 61BB  mov     word ptr [si],ax */
    ww(pDS, SI, AX);
L61BD:
    /* 61BD  mov     word ptr [si+2],bp */
    ww(pDS, SI + 0x2, BP);
L61C0:
    /* 61C0  mov     word ptr [si+4],bp */
    ww(pDS, SI + 0x4, BP);
L61C3:
    /* 61C3  mov     word ptr [si+6],di */
    ww(pDS, SI + 0x6, DI);
L61C6:
    /* 61C6  mov     word ptr [si+8],di */
    ww(pDS, SI + 0x8, DI);
L61C9:
    /* 61C9  lea     di,[si-8] */
    DI = (uint16_t)(SI + 0xFFF8);
L61CC:
    /* 61CC  mov     cx,bx */
    CX = BX;
L61CE:
    /* 61CE  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L61D0:
    /* 61D0  inc     cx */
    CX = (uint16_t)(CX + 1);
L61D1:
    /* 61D1  mov     ax,3E8h */
    AX = 0x3E8;
L61D4:
    /* 61D4  mov     bx,0FC18h */
    BX = 0xFC18;
L61D7: /* L61D7 */
    /* 61D7  mov     word ptr [di],ax */
    ww(pDS, DI, AX);
L61D9:
    /* 61D9  mov     word ptr [di+2],bx */
    ww(pDS, DI + 0x2, BX);
L61DC:
    /* 61DC  sub     di,0Ah */
    DI = (uint16_t)(DI - 0xA);
L61DF:
    /* 61DF  loop    L61D7 */
    if (--CX) goto L61D7;
L61E1:
    /* 61E1  mov     si,415Ch */
    SI = 0x415C;
L61E4: /* L61E4 */
    /* 61E4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L61E5:
    /* 61E5  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L61E6:
    /* 61E6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L61E7:
    /* 61E7  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L61EA:
    /* 61EA  push    si */
    push16(SI);
L61EB:
    /* 61EB  mov     bp,word ptr [si] */
    BP = rw(pDS, SI);
L61ED:
    /* 61ED  mov     word ptr ds:[5670h],bp */
    ww(pDS, 0x5670, BP);
L61F1:
    /* 61F1  mov     dx,word ptr [si+2] */
    DX = rw(pDS, SI + 0x2);
L61F4:
    /* 61F4  sub     di,bp */
    DI = (uint16_t)(DI - BP);
L61F6:
    /* 61F6  mov     bp,word ptr [si-2] */
    BP = rw(pDS, SI + 0xFFFE);
L61F9:
    /* 61F9  mov     bx,word ptr [si+4] */
    BX = rw(pDS, SI + 0x4);
L61FC:
    /* 61FC  mov     word ptr ds:[5684h],bx */
    ww(pDS, 0x5684, BX);
L6200:
    /* 6200  sub     bp,bx */
    BP = (uint16_t)(BP - BX);
L6202:
    /* 6202  sub     ax,dx */
    AX = sub16(AX, DX, 0);
L6204:
    /* 6204  jne     L6209 */
    if (!ZF) goto L6209;
L6206:
    /* 6206  jmp     L630A */
    goto L630A;
L6209: /* L6209 */
    /* 6209  js      L6222 */
    if (SF) goto L6222;
L620B:
    /* 620B  neg     bp */
    BP = (uint16_t)-BP;
L620D:
    /* 620D  neg     ax */
    AX = (uint16_t)-AX;
L620F:
    /* 620F  neg     di */
    DI = (uint16_t)-DI;
L6211:
    /* 6211  mov     cx,word ptr [si-6] */
    CX = rw(pDS, SI + 0xFFFA);
L6214:
    /* 6214  mov     word ptr ds:[5670h],cx */
    ww(pDS, 0x5670, CX);
L6218:
    /* 6218  mov     cx,word ptr [si-2] */
    CX = rw(pDS, SI + 0xFFFE);
L621B:
    /* 621B  mov     word ptr ds:[5684h],cx */
    ww(pDS, 0x5684, CX);
L621F:
    /* 621F  mov     dx,word ptr [si-4] */
    DX = rw(pDS, SI + 0xFFFC);
L6222: /* L6222 */
    /* 6222  mov     word ptr ds:[5674h],dx */
    ww(pDS, 0x5674, DX);
L6226:
    /* 6226  neg     ax */
    AX = (uint16_t)-AX;
L6228:
    /* 6228  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L6229:
    /* 6229  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L622A:
    /* 622A  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x0090, 0x622A, 2)) != 0) return c;
L622C:
    /* 622C  mov     word ptr ds:[5678h],ax */
    ww(pDS, 0x5678, AX);
L622F:
    /* 622F  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6231:
    /* 6231  sar     dx,1 */
    DX = sar16(DX, 1);
L6233:
    /* 6233  rcr     ax,1 */
    AX = rcr16(AX, 1);
L6235:
    /* 6235  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x0090, 0x6235, 2)) != 0) return c;
L6237:
    /* 6237  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6238:
    /* 6238  shl     ax,1 */
    AX = shl16(AX, 1);
L623A:
    /* 623A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L623C:
    /* 623C  mov     word ptr ds:[5676h],ax */
    ww(pDS, 0x5676, AX);
L623F:
    /* 623F  add     word ptr ds:[5678h],dx */
    ww(pDS, 0x5678, (uint16_t)(rw(pDS, 0x5678) + DX));
L6243:
    /* 6243  mov     ax,bp */
    AX = BP;
L6245:
    /* 6245  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6246:
    /* 6246  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x0090, 0x6246, 2)) != 0) return c;
L6248:
    /* 6248  mov     word ptr ds:[567Ch],ax */
    ww(pDS, 0x567C, AX);
L624B:
    /* 624B  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L624D:
    /* 624D  sar     dx,1 */
    DX = sar16(DX, 1);
L624F:
    /* 624F  rcr     ax,1 */
    AX = rcr16(AX, 1);
L6251:
    /* 6251  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x0090, 0x6251, 2)) != 0) return c;
L6253:
    /* 6253  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6254:
    /* 6254  shl     ax,1 */
    AX = shl16(AX, 1);
L6256:
    /* 6256  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6258:
    /* 6258  mov     word ptr ds:[567Ah],ax */
    ww(pDS, 0x567A, AX);
L625B:
    /* 625B  add     word ptr ds:[567Ch],dx */
    ww(pDS, 0x567C, (uint16_t)(rw(pDS, 0x567C) + DX));
L625F:
    /* 625F  mov     word ptr ds:[5672h],8000h */
    ww(pDS, 0x5672, 0x8000);
L6265:
    /* 6265  mov     word ptr ds:[5682h],8000h */
    ww(pDS, 0x5682, 0x8000);
L626B:
    /* 626B  mov     word ptr ds:[5680h],di */
    ww(pDS, 0x5680, DI);
L626F:
    /* 626F  mov     bx,word ptr ds:[5674h] */
    BX = rw(pDS, 0x5674);
L6273:
    /* 6273  mov     di,bx */
    DI = BX;
L6275:
    /* 6275  sub     di,word ptr ds:[566Ch] */
    DI = (uint16_t)(DI - rw(pDS, 0x566C));
L6279:
    /* 6279  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L627B:
    /* 627B  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L627D:
    /* 627D  add     di,bx */
    DI = (uint16_t)(DI + BX);
L627F:
    /* 627F  sub     di,word ptr ds:[566Ch] */
    DI = (uint16_t)(DI - rw(pDS, 0x566C));
L6283:
    /* 6283  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L6285:
    /* 6285  add     di,5690h */
    DI = (uint16_t)(DI + 0x5690);
L6289:
    /* 6289  dec     bx */
    BX = (uint16_t)(BX - 1);
L628A:
    /* 628A  sub     di,0Ah */
    DI = (uint16_t)(DI - 0xA);
L628D:
    /* 628D  push    cx */
    push16(CX);
L628E:
    /* 628E  mov     dx,word ptr ds:[5678h] */
    DX = rw(pDS, 0x5678);
L6292:
    /* 6292  mov     bp,word ptr ds:[5672h] */
    BP = rw(pDS, 0x5672);
L6296:
    /* 6296  mov     ax,word ptr ds:[5670h] */
    AX = rw(pDS, 0x5670);
L6299:
    /* 6299  mov     si,word ptr ds:[5682h] */
    SI = rw(pDS, 0x5682);
L629D:
    /* 629D  mov     cx,word ptr ds:[5684h] */
    CX = rw(pDS, 0x5684);
L62A1: /* L62A1 */
    /* 62A1  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L62A3:
    /* 62A3  dec     bx */
    BX = (uint16_t)(BX - 1);
L62A4:
    /* 62A4  add     bp,word ptr ds:[5676h] */
    BP = add16(BP, rw(pDS, 0x5676), 0);
L62A8:
    /* 62A8  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L62AA:
    /* 62AA  add     si,word ptr ds:[567Ah] */
    SI = add16(SI, rw(pDS, 0x567A), 0);
L62AE:
    /* 62AE  adc     cx,word ptr ds:[567Ch] */
    CX = (uint16_t)(CX + rw(pDS, 0x567C) + CF);
L62B2:
    /* 62B2  cmp     ax,word ptr [di+2] */
    sub16(AX, rw(pDS, DI + 0x2), 0);
L62B5:
    /* 62B5  jge     L62BD */
    if (SF == OF) goto L62BD;
L62B7:
    /* 62B7  mov     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, AX);
L62BA:
    /* 62BA  mov     word ptr [di+6],cx */
    ww(pDS, DI + 0x6, CX);
L62BD: /* L62BD */
    /* 62BD  cmp     ax,word ptr [di+4] */
    sub16(AX, rw(pDS, DI + 0x4), 0);
L62C0:
    /* 62C0  jle     L62C8 */
    if (ZF || SF != OF) goto L62C8;
L62C2:
    /* 62C2  mov     word ptr [di+4],ax */
    ww(pDS, DI + 0x4, AX);
L62C5:
    /* 62C5  mov     word ptr [di+8],cx */
    ww(pDS, DI + 0x8, CX);
L62C8: /* L62C8 */
    /* 62C8  sub     di,0Ah */
    DI = (uint16_t)(DI - 0xA);
L62CB:
    /* 62CB  dec     word ptr ds:[5680h] */
    ww(pDS, 0x5680, dec16(rw(pDS, 0x5680)));
L62CF:
    /* 62CF  jne     L62A1 */
    if (!ZF) goto L62A1;
L62D1:
    /* 62D1  pop     cx */
    CX = pop16();
L62D2: /* L62D2 */
    /* 62D2  pop     si */
    SI = pop16();
L62D3:
    /* 62D3  dec     word ptr ds:[567Eh] */
    ww(pDS, 0x567E, dec16(rw(pDS, 0x567E)));
L62D7:
    /* 62D7  je      L62DC */
    if (ZF) goto L62DC;
L62D9:
    /* 62D9  jmp     L61E4 */
    goto L61E4;
L62DC: /* L62DC */
    /* 62DC  mov     di,word ptr ds:[566Eh] */
    DI = rw(pDS, 0x566E);
L62E0:
    /* 62E0  sub     di,word ptr ds:[566Ch] */
    DI = (uint16_t)(DI - rw(pDS, 0x566C));
L62E4:
    /* 62E4  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L62E6:
    /* 62E6  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L62E8:
    /* 62E8  add     di,word ptr ds:[566Eh] */
    DI = (uint16_t)(DI + rw(pDS, 0x566E));
L62EC:
    /* 62EC  sub     di,word ptr ds:[566Ch] */
    DI = (uint16_t)(DI - rw(pDS, 0x566C));
L62F0:
    /* 62F0  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L62F2:
    /* 62F2  add     di,5690h */
    DI = (uint16_t)(DI + 0x5690);
L62F6:
    /* 62F6  add     di,0Ah */
    DI = (uint16_t)(DI + 0xA);
L62F9:
    /* 62F9  or      byte ptr [di+1],80h */
    wb(pDS, DI + 0x1, (uint8_t)(rb(pDS, DI + 0x1) | 0x80));
L62FD:
    /* 62FD  mov     si,5690h */
    SI = 0x5690;
L6300:
    /* 6300  push    di */
    push16(DI);
L6301:
    /* 6301  call    _seg003_F77 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0F77), 0x6304)) != 0) return c;
L6304:
    /* 6304  pop     di */
    DI = pop16();
L6305:
    /* 6305  and     byte ptr [di+1],7Fh */
    wb(pDS, DI + 0x1, logic8((uint8_t)(rb(pDS, DI + 0x1) & 0x7F)));
L6309:
    /* 6309  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L630A: /* L630A */
    /* 630A  mov     di,dx */
    DI = DX;
L630C:
    /* 630C  sub     di,word ptr ds:[566Ch] */
    DI = (uint16_t)(DI - rw(pDS, 0x566C));
L6310:
    /* 6310  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L6312:
    /* 6312  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L6314:
    /* 6314  add     di,dx */
    DI = (uint16_t)(DI + DX);
L6316:
    /* 6316  sub     di,word ptr ds:[566Ch] */
    DI = (uint16_t)(DI - rw(pDS, 0x566C));
L631A:
    /* 631A  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L631C:
    /* 631C  add     di,5690h */
    DI = (uint16_t)(DI + 0x5690);
L6320:
    /* 6320  mov     word ptr [di],dx */
    ww(pDS, DI, DX);
L6322:
    /* 6322  mov     ax,word ptr [si-6] */
    AX = rw(pDS, SI + 0xFFFA);
L6325:
    /* 6325  mov     cx,word ptr [si] */
    CX = rw(pDS, SI);
L6327:
    /* 6327  mov     bx,word ptr [si-2] */
    BX = rw(pDS, SI + 0xFFFE);
L632A:
    /* 632A  mov     dx,word ptr [si+4] */
    DX = rw(pDS, SI + 0x4);
L632D:
    /* 632D  cmp     ax,cx */
    sub16(AX, CX, 0);
L632F:
    /* 632F  jle     L6334 */
    if (ZF || SF != OF) goto L6334;
L6331:
    /* 6331  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L6332:
    /* 6332  xchg    bx,dx */
    { uint16_t t_ = DX;
    DX = BX;
    BX = t_; }
L6334: /* L6334 */
    /* 6334  cmp     ax,word ptr [di+2] */
    sub16(AX, rw(pDS, DI + 0x2), 0);
L6337:
    /* 6337  jg      L633F */
    if (!ZF && SF == OF) goto L633F;
L6339:
    /* 6339  mov     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, AX);
L633C:
    /* 633C  mov     word ptr [di+6],bx */
    ww(pDS, DI + 0x6, BX);
L633F: /* L633F */
    /* 633F  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L6340:
    /* 6340  xchg    bx,dx */
    { uint16_t t_ = DX;
    DX = BX;
    BX = t_; }
L6342:
    /* 6342  cmp     ax,word ptr [di+4] */
    sub16(AX, rw(pDS, DI + 0x4), 0);
L6345:
    /* 6345  jl      L634D */
    if (SF != OF) goto L634D;
L6347:
    /* 6347  mov     word ptr [di+4],ax */
    ww(pDS, DI + 0x4, AX);
L634A:
    /* 634A  mov     word ptr [di+8],bx */
    ww(pDS, DI + 0x8, BX);
L634D: /* L634D */
    /* 634D  jmp     L62D2 */
    goto L62D2;
}
