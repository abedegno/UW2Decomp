/* grjumps.c: replaces src/gfx/GRJUMPS.ASM (seg003_5A72, 5A72..5AE7 of its
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

uint32_t asm_mod_GRJUMPS(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x5A72: goto L5A72;
    case 0x5A75: goto L5A75;
    case 0x5A78: goto L5A78;
    case 0x5A7B: goto L5A7B;
    case 0x5A7E: goto L5A7E;
    case 0x5A81: goto L5A81;
    case 0x5A84: goto L5A84;
    case 0x5A87: goto L5A87;
    case 0x5A8A: goto L5A8A;
    case 0x5A8D: goto L5A8D;
    case 0x5A90: goto L5A90;
    case 0x5A93: goto L5A93;
    case 0x5A96: goto L5A96;
    case 0x5A99: goto L5A99;
    case 0x5A9C: goto L5A9C;
    case 0x5A9F: goto L5A9F;
    case 0x5AA2: goto L5AA2;
    case 0x5AA5: goto L5AA5;
    case 0x5AA8: goto L5AA8;
    case 0x5AAB: goto L5AAB;
    case 0x5AAE: goto L5AAE;
    case 0x5AB1: goto L5AB1;
    case 0x5AB4: goto L5AB4;
    case 0x5AB7: goto L5AB7;
    case 0x5ABA: goto L5ABA;
    case 0x5ABD: goto L5ABD;
    case 0x5AC0: goto L5AC0;
    case 0x5AC3: goto L5AC3;
    case 0x5AC6: goto L5AC6;
    case 0x5AC9: goto L5AC9;
    case 0x5ACC: goto L5ACC;
    case 0x5ACF: goto L5ACF;
    case 0x5AD2: goto L5AD2;
    case 0x5AD5: goto L5AD5;
    case 0x5AD8: goto L5AD8;
    case 0x5ADB: goto L5ADB;
    case 0x5ADE: goto L5ADE;
    case 0x5AE1: goto L5AE1;
    case 0x5AE4: goto L5AE4;
    default: asm_bad_entry("GRJUMPS.ASM", entry);
    }

    /* seg003_5A72  (+5A72) */
L5A72: /* _seg003_5A72 */
    /* 5A72  jmp     _seg003_4462 */
    return ASM_JMP(0x0090, 0x4462);

    /* seg003_5A75  (+5A75) */
L5A75: /* _seg003_5A75 */
    /* 5A75  jmp     _seg003_447D */
    return ASM_JMP(0x0090, 0x447D);

    /* seg003_5A78  (+5A78) */
L5A78: /* _seg003_5A78 */
    /* 5A78  jmp     _seg003_43B6 */
    return ASM_JMP(0x0090, 0x43B6);
L5A7B:
    /* 5A7B  jmp     _seg003_43BE */
    return ASM_JMP(0x0090, 0x43BE);
L5A7E:
    /* 5A7E  jmp     _seg003_43AE */
    return ASM_JMP(0x0090, 0x43AE);

    /* seg003_5A81  (+5A81) */
L5A81: /* _seg003_5A81 */
    /* 5A81  jmp     _seg003_440F */
    return ASM_JMP(0x0090, 0x440F);
L5A84:
    /* 5A84  jmp     _seg003_4448 */
    return ASM_JMP(0x0090, 0x4448);

    /* seg003_5A87  (+5A87) */
L5A87: /* _seg003_5A87 */
    /* 5A87  jmp     _seg003_439E */
    return ASM_JMP(0x0090, 0x439E);
L5A8A:
    /* 5A8A  jmp     _seg003_43A6 */
    return ASM_JMP(0x0090, 0x43A6);

    /* seg003_5A8D  (+5A8D) */
L5A8D: /* _seg003_5A8D */
    /* 5A8D  jmp     _seg003_32F4 */
    return ASM_JMP(0x0090, 0x32F4);

    /* seg003_5A90  (+5A90) */
L5A90: /* _seg003_5A90 */
    /* 5A90  jmp     _seg003_3357 */
    return ASM_JMP(0x0090, 0x3357);

    /* seg003_5A93  (+5A93) */
L5A93: /* _seg003_5A93 */
    /* 5A93  jmp     _seg003_335A */
    return ASM_JMP(0x0090, 0x335A);

    /* seg003_5A96  (+5A96) */
L5A96: /* _seg003_5A96 */
    /* 5A96  jmp     _seg003_333F */
    return ASM_JMP(0x0090, 0x333F);

    /* seg003_5A99  (+5A99) */
L5A99: /* _seg003_5A99 */
    /* 5A99  jmp     _seg003_397C */
    return ASM_JMP(0x0090, 0x397C);

    /* seg003_5A9C  (+5A9C) */
L5A9C: /* _seg003_5A9C */
    /* 5A9C  jmp     _seg003_3964 */
    return ASM_JMP(0x0090, 0x3964);

    /* seg003_5A9F  (+5A9F) */
L5A9F: /* _seg003_5A9F */
    /* 5A9F  jmp     _seg003_3915 */
    return ASM_JMP(0x0090, 0x3915);

    /* seg003_5AA2  (+5AA2) */
L5AA2: /* _seg003_5AA2 */
    /* 5AA2  jmp     _seg003_392D */
    return ASM_JMP(0x0090, 0x392D);
L5AA5:
    /* 5AA5  jmp     _seg003_3958 */
    return ASM_JMP(0x0090, 0x3958);
L5AA8:
    /* 5AA8  jmp     _seg003_3950 */
    return ASM_JMP(0x0090, 0x3950);

    /* seg003_5AAB  (+5AAB) */
L5AAB: /* _seg003_5AAB */
    /* 5AAB  jmp     _seg003_39E5 */
    return ASM_JMP(0x0090, 0x39E5);

    /* seg003_5AAE  (+5AAE) */
L5AAE: /* _seg003_5AAE */
    /* 5AAE  jmp     _seg003_39A2 */
    return ASM_JMP(0x0090, 0x39A2);

    /* seg003_5AB1  (+5AB1) */
L5AB1: /* _seg003_5AB1 */
    /* 5AB1  jmp     _seg003_3A16 */
    return ASM_JMP(0x0090, 0x3A16);

    /* seg003_5AB4  (+5AB4) */
L5AB4: /* _seg003_5AB4 */
    /* 5AB4  jmp     _seg003_3A3F */
    return ASM_JMP(0x0090, 0x3A3F);

    /* seg003_5AB7  (+5AB7) */
L5AB7: /* _seg003_5AB7 */
    /* 5AB7  jmp     _seg003_321D */
    return ASM_JMP(0x0090, 0x321D);

    /* seg003_5ABA  (+5ABA) */
L5ABA: /* _seg003_5ABA */
    /* 5ABA  jmp     _seg003_324C */
    return ASM_JMP(0x0090, 0x324C);
L5ABD:
    /* 5ABD  jmp     _seg003_3249 */
    return ASM_JMP(0x0090, 0x3249);
L5AC0:
    /* 5AC0  jmp     _seg003_330D */
    return ASM_JMP(0x0090, 0x330D);
L5AC3: /* _seg003_5AC3 */
    /* 5AC3  jmp     _seg003_3604 */
    return ASM_JMP(0x0090, 0x3604);
L5AC6: /* _seg003_5AC6 */
    /* 5AC6  jmp     _seg003_3799 */
    return ASM_JMP(0x0090, 0x3799);
L5AC9: /* _seg003_5AC9 */
    /* 5AC9  jmp     _seg003_358E */
    return ASM_JMP(0x0090, 0x358E);
L5ACC: /* _seg003_5ACC */
    /* 5ACC  jmp     _seg003_36FA */
    return ASM_JMP(0x0090, 0x36FA);
L5ACF:
    /* 5ACF  jmp     _seg003_36BB */
    return ASM_JMP(0x0090, 0x36BB);
L5AD2: /* _seg003_5AD2 */
    /* 5AD2  jmp     _seg003_38BC */
    return ASM_JMP(0x0090, 0x38BC);
L5AD5: /* _seg003_5AD5 */
    /* 5AD5  jmp     _seg003_3857 */
    return ASM_JMP(0x0090, 0x3857);
L5AD8:
    /* 5AD8  jmp     _seg003_3817 */
    return ASM_JMP(0x0090, 0x3817);
L5ADB:
    /* 5ADB  jmp     _seg003_3674 */
    return ASM_JMP(0x0090, 0x3674);

    /* seg003_5ADE  (+5ADE) */
L5ADE: /* _seg003_5ADE */
    /* 5ADE  jmp     _seg003_3A68 */
    return ASM_JMP(0x0090, 0x3A68);
L5AE1: /* _seg003_5AE1 */
    /* 5AE1  jmp     _seg003_351B */
    return ASM_JMP(0x0090, 0x351B);
L5AE4:
    /* 5AE4  jmp     _seg003_321C */
    return ASM_JMP(0x0090, 0x321C);
}
