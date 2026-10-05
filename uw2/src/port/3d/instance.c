/* instance.c: replaces src/3d/INSTANCE.ASM (seg004_0849_3000, 3000..3CEC of its
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

uint32_t asm_mod_INSTANCE(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x3000: goto L3000;
    case 0x3001: goto L3001;
    case 0x3002: goto L3002;
    case 0x3003: goto L3003;
    case 0x3007: goto L3007;
    case 0x300A: goto L300A;
    case 0x300C: goto L300C;
    case 0x300E: goto L300E;
    case 0x3012: goto L3012;
    case 0x3017: goto L3017;
    case 0x3018: goto L3018;
    case 0x301A: goto L301A;
    case 0x301D: goto L301D;
    case 0x3021: goto L3021;
    case 0x3026: goto L3026;
    case 0x3027: goto L3027;
    case 0x3029: goto L3029;
    case 0x302C: goto L302C;
    case 0x3030: goto L3030;
    case 0x3035: goto L3035;
    case 0x3036: goto L3036;
    case 0x3038: goto L3038;
    case 0x303B: goto L303B;
    case 0x303F: goto L303F;
    case 0x3043: goto L3043;
    case 0x3047: goto L3047;
    case 0x304B: goto L304B;
    case 0x304F: goto L304F;
    case 0x3054: goto L3054;
    case 0x3056: goto L3056;
    case 0x3059: goto L3059;
    case 0x305D: goto L305D;
    case 0x3061: goto L3061;
    case 0x3065: goto L3065;
    case 0x3069: goto L3069;
    case 0x306D: goto L306D;
    case 0x3072: goto L3072;
    case 0x3075: goto L3075;
    case 0x3078: goto L3078;
    case 0x307B: goto L307B;
    case 0x307E: goto L307E;
    case 0x3080: goto L3080;
    case 0x3083: goto L3083;
    case 0x3087: goto L3087;
    case 0x308A: goto L308A;
    case 0x308C: goto L308C;
    case 0x308F: goto L308F;
    case 0x3093: goto L3093;
    case 0x3099: goto L3099;
    case 0x309C: goto L309C;
    case 0x309E: goto L309E;
    case 0x30A0: goto L30A0;
    case 0x30A1: goto L30A1;
    case 0x30A3: goto L30A3;
    case 0x30A6: goto L30A6;
    case 0x30A7: goto L30A7;
    case 0x30AC: goto L30AC;
    case 0x30AF: goto L30AF;
    case 0x30B1: goto L30B1;
    case 0x30B3: goto L30B3;
    case 0x30B4: goto L30B4;
    case 0x30B8: goto L30B8;
    case 0x30BC: goto L30BC;
    case 0x30BD: goto L30BD;
    case 0x30C1: goto L30C1;
    case 0x30C2: goto L30C2;
    case 0x30C3: goto L30C3;
    case 0x30C7: goto L30C7;
    case 0x30CA: goto L30CA;
    case 0x30CC: goto L30CC;
    case 0x30D0: goto L30D0;
    case 0x30D3: goto L30D3;
    case 0x30D5: goto L30D5;
    case 0x30D7: goto L30D7;
    case 0x30D9: goto L30D9;
    case 0x30DC: goto L30DC;
    case 0x30E2: goto L30E2;
    case 0x30E8: goto L30E8;
    case 0x30EB: goto L30EB;
    case 0x30EE: goto L30EE;
    case 0x30F1: goto L30F1;
    case 0x30F4: goto L30F4;
    case 0x30F7: goto L30F7;
    case 0x30FA: goto L30FA;
    case 0x30FE: goto L30FE;
    case 0x3102: goto L3102;
    case 0x3105: goto L3105;
    case 0x3107: goto L3107;
    case 0x3109: goto L3109;
    case 0x310B: goto L310B;
    case 0x310D: goto L310D;
    case 0x310F: goto L310F;
    case 0x3111: goto L3111;
    case 0x3113: goto L3113;
    case 0x3115: goto L3115;
    case 0x3117: goto L3117;
    case 0x3119: goto L3119;
    case 0x311B: goto L311B;
    case 0x311D: goto L311D;
    case 0x311F: goto L311F;
    case 0x3121: goto L3121;
    case 0x3123: goto L3123;
    case 0x3125: goto L3125;
    case 0x3127: goto L3127;
    case 0x312A: goto L312A;
    case 0x312C: goto L312C;
    case 0x312E: goto L312E;
    case 0x312F: goto L312F;
    case 0x3131: goto L3131;
    case 0x3133: goto L3133;
    case 0x3135: goto L3135;
    case 0x3137: goto L3137;
    case 0x313B: goto L313B;
    case 0x313E: goto L313E;
    case 0x3140: goto L3140;
    case 0x3142: goto L3142;
    case 0x3144: goto L3144;
    case 0x3148: goto L3148;
    case 0x314B: goto L314B;
    case 0x314D: goto L314D;
    case 0x314F: goto L314F;
    case 0x3151: goto L3151;
    case 0x3155: goto L3155;
    case 0x3158: goto L3158;
    case 0x315A: goto L315A;
    case 0x315C: goto L315C;
    case 0x315E: goto L315E;
    case 0x3162: goto L3162;
    case 0x3165: goto L3165;
    case 0x3167: goto L3167;
    case 0x3169: goto L3169;
    case 0x316B: goto L316B;
    case 0x316F: goto L316F;
    case 0x3172: goto L3172;
    case 0x3174: goto L3174;
    case 0x3176: goto L3176;
    case 0x3178: goto L3178;
    case 0x317C: goto L317C;
    case 0x317E: goto L317E;
    case 0x3180: goto L3180;
    case 0x3182: goto L3182;
    case 0x3184: goto L3184;
    case 0x3186: goto L3186;
    case 0x318A: goto L318A;
    case 0x318D: goto L318D;
    case 0x318F: goto L318F;
    case 0x3191: goto L3191;
    case 0x3193: goto L3193;
    case 0x3197: goto L3197;
    case 0x319A: goto L319A;
    case 0x319C: goto L319C;
    case 0x319E: goto L319E;
    case 0x31A0: goto L31A0;
    case 0x31A4: goto L31A4;
    case 0x31A7: goto L31A7;
    case 0x31A9: goto L31A9;
    case 0x31AB: goto L31AB;
    case 0x31AD: goto L31AD;
    case 0x31B1: goto L31B1;
    case 0x31B4: goto L31B4;
    case 0x31B6: goto L31B6;
    case 0x31B8: goto L31B8;
    case 0x31BA: goto L31BA;
    case 0x31BE: goto L31BE;
    case 0x31C1: goto L31C1;
    case 0x31C3: goto L31C3;
    case 0x31C5: goto L31C5;
    case 0x31C7: goto L31C7;
    case 0x31CB: goto L31CB;
    case 0x31CF: goto L31CF;
    case 0x31D1: goto L31D1;
    case 0x31D3: goto L31D3;
    case 0x31D6: goto L31D6;
    case 0x31DA: goto L31DA;
    case 0x31DD: goto L31DD;
    case 0x31DF: goto L31DF;
    case 0x31E1: goto L31E1;
    case 0x31E3: goto L31E3;
    case 0x31E7: goto L31E7;
    case 0x31EA: goto L31EA;
    case 0x31EC: goto L31EC;
    case 0x31EE: goto L31EE;
    case 0x31F0: goto L31F0;
    case 0x31F4: goto L31F4;
    case 0x31F7: goto L31F7;
    case 0x31F9: goto L31F9;
    case 0x31FB: goto L31FB;
    case 0x31FD: goto L31FD;
    case 0x3201: goto L3201;
    case 0x3204: goto L3204;
    case 0x3206: goto L3206;
    case 0x3208: goto L3208;
    case 0x320A: goto L320A;
    case 0x320E: goto L320E;
    case 0x3211: goto L3211;
    case 0x3213: goto L3213;
    case 0x3215: goto L3215;
    case 0x3217: goto L3217;
    case 0x321B: goto L321B;
    case 0x321D: goto L321D;
    case 0x321F: goto L321F;
    case 0x3221: goto L3221;
    case 0x3223: goto L3223;
    case 0x3226: goto L3226;
    case 0x3229: goto L3229;
    case 0x322B: goto L322B;
    case 0x322D: goto L322D;
    case 0x322F: goto L322F;
    case 0x3231: goto L3231;
    case 0x3233: goto L3233;
    case 0x3238: goto L3238;
    case 0x323A: goto L323A;
    case 0x323C: goto L323C;
    case 0x323F: goto L323F;
    case 0x3242: goto L3242;
    case 0x3245: goto L3245;
    case 0x3247: goto L3247;
    case 0x3249: goto L3249;
    case 0x324B: goto L324B;
    case 0x324D: goto L324D;
    case 0x324F: goto L324F;
    case 0x3254: goto L3254;
    case 0x3256: goto L3256;
    case 0x3258: goto L3258;
    case 0x325B: goto L325B;
    case 0x325E: goto L325E;
    case 0x3264: goto L3264;
    case 0x3266: goto L3266;
    case 0x3269: goto L3269;
    case 0x326B: goto L326B;
    case 0x326D: goto L326D;
    case 0x326F: goto L326F;
    case 0x3273: goto L3273;
    case 0x3276: goto L3276;
    case 0x3278: goto L3278;
    case 0x327A: goto L327A;
    case 0x327C: goto L327C;
    case 0x3280: goto L3280;
    case 0x3283: goto L3283;
    case 0x3285: goto L3285;
    case 0x3287: goto L3287;
    case 0x3289: goto L3289;
    case 0x328D: goto L328D;
    case 0x3290: goto L3290;
    case 0x3292: goto L3292;
    case 0x3294: goto L3294;
    case 0x3296: goto L3296;
    case 0x329A: goto L329A;
    case 0x329D: goto L329D;
    case 0x329F: goto L329F;
    case 0x32A1: goto L32A1;
    case 0x32A3: goto L32A3;
    case 0x32A7: goto L32A7;
    case 0x32AA: goto L32AA;
    case 0x32AC: goto L32AC;
    case 0x32AE: goto L32AE;
    case 0x32B0: goto L32B0;
    case 0x32B4: goto L32B4;
    case 0x32B7: goto L32B7;
    case 0x32B9: goto L32B9;
    case 0x32BB: goto L32BB;
    case 0x32BD: goto L32BD;
    case 0x32C1: goto L32C1;
    case 0x32C3: goto L32C3;
    case 0x32C6: goto L32C6;
    case 0x32C8: goto L32C8;
    case 0x32CA: goto L32CA;
    case 0x32CC: goto L32CC;
    case 0x32CE: goto L32CE;
    case 0x32D0: goto L32D0;
    case 0x32D2: goto L32D2;
    case 0x32D6: goto L32D6;
    case 0x32D8: goto L32D8;
    case 0x32DA: goto L32DA;
    case 0x32DC: goto L32DC;
    case 0x32DE: goto L32DE;
    case 0x32E2: goto L32E2;
    case 0x32E4: goto L32E4;
    case 0x32E9: goto L32E9;
    case 0x32EB: goto L32EB;
    case 0x32ED: goto L32ED;
    case 0x32F0: goto L32F0;
    case 0x32F3: goto L32F3;
    case 0x32F5: goto L32F5;
    case 0x32F7: goto L32F7;
    case 0x32F9: goto L32F9;
    case 0x32FB: goto L32FB;
    case 0x32FD: goto L32FD;
    case 0x32FF: goto L32FF;
    case 0x3303: goto L3303;
    case 0x3305: goto L3305;
    case 0x3307: goto L3307;
    case 0x3309: goto L3309;
    case 0x330B: goto L330B;
    case 0x330F: goto L330F;
    case 0x3311: goto L3311;
    case 0x3316: goto L3316;
    case 0x3318: goto L3318;
    case 0x331A: goto L331A;
    case 0x331D: goto L331D;
    case 0x3324: goto L3324;
    case 0x3327: goto L3327;
    case 0x332B: goto L332B;
    case 0x332D: goto L332D;
    case 0x332F: goto L332F;
    case 0x3332: goto L3332;
    case 0x3336: goto L3336;
    case 0x3338: goto L3338;
    case 0x333A: goto L333A;
    case 0x333C: goto L333C;
    case 0x333E: goto L333E;
    case 0x3343: goto L3343;
    case 0x3347: goto L3347;
    case 0x3349: goto L3349;
    case 0x334B: goto L334B;
    case 0x334D: goto L334D;
    case 0x334F: goto L334F;
    case 0x3352: goto L3352;
    case 0x3354: goto L3354;
    case 0x3355: goto L3355;
    case 0x3358: goto L3358;
    case 0x335C: goto L335C;
    case 0x335E: goto L335E;
    case 0x3360: goto L3360;
    case 0x3362: goto L3362;
    case 0x3364: goto L3364;
    case 0x3367: goto L3367;
    case 0x3369: goto L3369;
    case 0x336A: goto L336A;
    case 0x336D: goto L336D;
    case 0x336F: goto L336F;
    case 0x3372: goto L3372;
    case 0x3376: goto L3376;
    case 0x3378: goto L3378;
    case 0x337A: goto L337A;
    case 0x337D: goto L337D;
    case 0x3381: goto L3381;
    case 0x3383: goto L3383;
    case 0x3385: goto L3385;
    case 0x338A: goto L338A;
    case 0x338E: goto L338E;
    case 0x3390: goto L3390;
    case 0x3392: goto L3392;
    case 0x3394: goto L3394;
    case 0x3396: goto L3396;
    case 0x3399: goto L3399;
    case 0x339B: goto L339B;
    case 0x339C: goto L339C;
    case 0x339E: goto L339E;
    case 0x33A2: goto L33A2;
    case 0x33A4: goto L33A4;
    case 0x33A6: goto L33A6;
    case 0x33A8: goto L33A8;
    case 0x33AA: goto L33AA;
    case 0x33AD: goto L33AD;
    case 0x33AF: goto L33AF;
    case 0x33B0: goto L33B0;
    case 0x33B6: goto L33B6;
    case 0x33B8: goto L33B8;
    case 0x33BA: goto L33BA;
    case 0x33BC: goto L33BC;
    case 0x33C0: goto L33C0;
    case 0x33C3: goto L33C3;
    case 0x33C6: goto L33C6;
    case 0x33CA: goto L33CA;
    case 0x33CC: goto L33CC;
    case 0x33CE: goto L33CE;
    case 0x33D1: goto L33D1;
    case 0x33D3: goto L33D3;
    case 0x33D7: goto L33D7;
    case 0x33D9: goto L33D9;
    case 0x33DB: goto L33DB;
    case 0x33DD: goto L33DD;
    case 0x33DF: goto L33DF;
    case 0x33E1: goto L33E1;
    case 0x33E4: goto L33E4;
    case 0x33E8: goto L33E8;
    case 0x33EB: goto L33EB;
    case 0x33EF: goto L33EF;
    case 0x33F1: goto L33F1;
    case 0x33F3: goto L33F3;
    case 0x33F6: goto L33F6;
    case 0x33F8: goto L33F8;
    case 0x33FC: goto L33FC;
    case 0x33FE: goto L33FE;
    case 0x3400: goto L3400;
    case 0x3402: goto L3402;
    case 0x3404: goto L3404;
    case 0x3406: goto L3406;
    case 0x3409: goto L3409;
    case 0x340D: goto L340D;
    case 0x340E: goto L340E;
    case 0x3414: goto L3414;
    case 0x3416: goto L3416;
    case 0x341C: goto L341C;
    case 0x341E: goto L341E;
    case 0x3424: goto L3424;
    case 0x3426: goto L3426;
    case 0x342C: goto L342C;
    case 0x342E: goto L342E;
    case 0x3434: goto L3434;
    case 0x3436: goto L3436;
    case 0x343B: goto L343B;
    case 0x343D: goto L343D;
    case 0x3442: goto L3442;
    case 0x3448: goto L3448;
    case 0x344E: goto L344E;
    case 0x3454: goto L3454;
    case 0x345A: goto L345A;
    case 0x3460: goto L3460;
    case 0x3466: goto L3466;
    case 0x346C: goto L346C;
    case 0x346F: goto L346F;
    case 0x3472: goto L3472;
    case 0x3473: goto L3473;
    case 0x3474: goto L3474;
    case 0x3475: goto L3475;
    case 0x3478: goto L3478;
    case 0x347B: goto L347B;
    case 0x347C: goto L347C;
    case 0x347D: goto L347D;
    case 0x3482: goto L3482;
    case 0x3484: goto L3484;
    case 0x3489: goto L3489;
    case 0x348F: goto L348F;
    case 0x3495: goto L3495;
    case 0x349B: goto L349B;
    case 0x34A1: goto L34A1;
    case 0x34A7: goto L34A7;
    case 0x34AD: goto L34AD;
    case 0x34B3: goto L34B3;
    case 0x34B6: goto L34B6;
    case 0x34B9: goto L34B9;
    case 0x34BA: goto L34BA;
    case 0x34BB: goto L34BB;
    case 0x34BC: goto L34BC;
    case 0x34BF: goto L34BF;
    case 0x34C2: goto L34C2;
    case 0x34C3: goto L34C3;
    case 0x34C4: goto L34C4;
    case 0x34C5: goto L34C5;
    case 0x34C8: goto L34C8;
    case 0x34CC: goto L34CC;
    case 0x34D0: goto L34D0;
    case 0x34D3: goto L34D3;
    case 0x34D7: goto L34D7;
    case 0x34DB: goto L34DB;
    case 0x34DE: goto L34DE;
    case 0x34E2: goto L34E2;
    case 0x34E6: goto L34E6;
    case 0x34E9: goto L34E9;
    case 0x34ED: goto L34ED;
    case 0x34F1: goto L34F1;
    case 0x34F2: goto L34F2;
    case 0x34F4: goto L34F4;
    case 0x34F5: goto L34F5;
    case 0x34F6: goto L34F6;
    case 0x34F9: goto L34F9;
    case 0x34FC: goto L34FC;
    case 0x34FE: goto L34FE;
    case 0x3500: goto L3500;
    case 0x3501: goto L3501;
    case 0x3502: goto L3502;
    case 0x3505: goto L3505;
    case 0x3508: goto L3508;
    case 0x350A: goto L350A;
    case 0x350C: goto L350C;
    case 0x350D: goto L350D;
    case 0x350E: goto L350E;
    case 0x3511: goto L3511;
    case 0x3514: goto L3514;
    case 0x3516: goto L3516;
    case 0x3518: goto L3518;
    case 0x351A: goto L351A;
    case 0x351C: goto L351C;
    case 0x351D: goto L351D;
    case 0x3520: goto L3520;
    case 0x3522: goto L3522;
    case 0x3524: goto L3524;
    case 0x3525: goto L3525;
    case 0x3528: goto L3528;
    case 0x352A: goto L352A;
    case 0x352C: goto L352C;
    case 0x352D: goto L352D;
    case 0x3530: goto L3530;
    case 0x3532: goto L3532;
    case 0x3534: goto L3534;
    case 0x3536: goto L3536;
    case 0x353A: goto L353A;
    case 0x353C: goto L353C;
    case 0x353E: goto L353E;
    case 0x3542: goto L3542;
    case 0x3544: goto L3544;
    case 0x3546: goto L3546;
    case 0x354A: goto L354A;
    case 0x354C: goto L354C;
    case 0x3550: goto L3550;
    case 0x3552: goto L3552;
    case 0x3556: goto L3556;
    case 0x3558: goto L3558;
    case 0x355A: goto L355A;
    case 0x355E: goto L355E;
    case 0x3560: goto L3560;
    case 0x3562: goto L3562;
    case 0x3566: goto L3566;
    case 0x3568: goto L3568;
    case 0x356A: goto L356A;
    case 0x356E: goto L356E;
    case 0x3570: goto L3570;
    case 0x3572: goto L3572;
    case 0x3576: goto L3576;
    case 0x3578: goto L3578;
    case 0x357A: goto L357A;
    case 0x357E: goto L357E;
    case 0x3580: goto L3580;
    case 0x3582: goto L3582;
    case 0x3586: goto L3586;
    case 0x3587: goto L3587;
    case 0x3589: goto L3589;
    case 0x358D: goto L358D;
    case 0x358F: goto L358F;
    case 0x3591: goto L3591;
    case 0x3595: goto L3595;
    case 0x3597: goto L3597;
    case 0x3599: goto L3599;
    case 0x359D: goto L359D;
    case 0x359F: goto L359F;
    case 0x35A3: goto L35A3;
    case 0x35A5: goto L35A5;
    case 0x35A9: goto L35A9;
    case 0x35AB: goto L35AB;
    case 0x35AD: goto L35AD;
    case 0x35B1: goto L35B1;
    case 0x35B3: goto L35B3;
    case 0x35B5: goto L35B5;
    case 0x35B9: goto L35B9;
    case 0x35BB: goto L35BB;
    case 0x35BD: goto L35BD;
    case 0x35C1: goto L35C1;
    case 0x35C3: goto L35C3;
    case 0x35C5: goto L35C5;
    case 0x35C9: goto L35C9;
    case 0x35CB: goto L35CB;
    case 0x35CD: goto L35CD;
    case 0x35D1: goto L35D1;
    case 0x35D3: goto L35D3;
    case 0x35D5: goto L35D5;
    case 0x35D9: goto L35D9;
    case 0x35DA: goto L35DA;
    case 0x35DC: goto L35DC;
    case 0x35E0: goto L35E0;
    case 0x35E2: goto L35E2;
    case 0x35E4: goto L35E4;
    case 0x35E8: goto L35E8;
    case 0x35EA: goto L35EA;
    case 0x35EC: goto L35EC;
    case 0x35EE: goto L35EE;
    case 0x35F2: goto L35F2;
    case 0x35F4: goto L35F4;
    case 0x35F6: goto L35F6;
    case 0x35FA: goto L35FA;
    case 0x35FC: goto L35FC;
    case 0x35FE: goto L35FE;
    case 0x35FF: goto L35FF;
    case 0x3602: goto L3602;
    case 0x3603: goto L3603;
    case 0x3605: goto L3605;
    case 0x3607: goto L3607;
    case 0x360A: goto L360A;
    case 0x360C: goto L360C;
    case 0x360F: goto L360F;
    case 0x3611: goto L3611;
    case 0x3614: goto L3614;
    case 0x3615: goto L3615;
    case 0x3617: goto L3617;
    case 0x3619: goto L3619;
    case 0x361B: goto L361B;
    case 0x361D: goto L361D;
    case 0x3620: goto L3620;
    case 0x3621: goto L3621;
    case 0x3623: goto L3623;
    case 0x3625: goto L3625;
    case 0x3627: goto L3627;
    case 0x3629: goto L3629;
    case 0x362C: goto L362C;
    case 0x362E: goto L362E;
    case 0x3631: goto L3631;
    case 0x3633: goto L3633;
    case 0x3635: goto L3635;
    case 0x3639: goto L3639;
    case 0x363B: goto L363B;
    case 0x363D: goto L363D;
    case 0x3642: goto L3642;
    case 0x3645: goto L3645;
    case 0x3648: goto L3648;
    case 0x364B: goto L364B;
    case 0x364F: goto L364F;
    case 0x3650: goto L3650;
    case 0x3653: goto L3653;
    case 0x3657: goto L3657;
    case 0x3659: goto L3659;
    case 0x365B: goto L365B;
    case 0x365E: goto L365E;
    case 0x3662: goto L3662;
    case 0x3664: goto L3664;
    case 0x3666: goto L3666;
    case 0x3668: goto L3668;
    case 0x366A: goto L366A;
    case 0x366D: goto L366D;
    case 0x3671: goto L3671;
    case 0x3675: goto L3675;
    case 0x3677: goto L3677;
    case 0x3679: goto L3679;
    case 0x367C: goto L367C;
    case 0x3680: goto L3680;
    case 0x3682: goto L3682;
    case 0x3684: goto L3684;
    case 0x3686: goto L3686;
    case 0x3688: goto L3688;
    case 0x368C: goto L368C;
    case 0x368D: goto L368D;
    case 0x368F: goto L368F;
    case 0x3691: goto L3691;
    case 0x3696: goto L3696;
    case 0x3698: goto L3698;
    case 0x369A: goto L369A;
    case 0x369D: goto L369D;
    case 0x36A1: goto L36A1;
    case 0x36A3: goto L36A3;
    case 0x36A6: goto L36A6;
    case 0x36A9: goto L36A9;
    case 0x36AC: goto L36AC;
    case 0x36B0: goto L36B0;
    case 0x36B2: goto L36B2;
    case 0x36B4: goto L36B4;
    case 0x36B7: goto L36B7;
    case 0x36BB: goto L36BB;
    case 0x36BD: goto L36BD;
    case 0x36BF: goto L36BF;
    case 0x36C1: goto L36C1;
    case 0x36C3: goto L36C3;
    case 0x36C5: goto L36C5;
    case 0x36C7: goto L36C7;
    case 0x36CA: goto L36CA;
    case 0x36CC: goto L36CC;
    case 0x36CF: goto L36CF;
    case 0x36D3: goto L36D3;
    case 0x36D6: goto L36D6;
    case 0x36DA: goto L36DA;
    case 0x36DC: goto L36DC;
    case 0x36DE: goto L36DE;
    case 0x36E1: goto L36E1;
    case 0x36E5: goto L36E5;
    case 0x36E7: goto L36E7;
    case 0x36E9: goto L36E9;
    case 0x36EB: goto L36EB;
    case 0x36ED: goto L36ED;
    case 0x36EF: goto L36EF;
    case 0x36F1: goto L36F1;
    case 0x36F4: goto L36F4;
    case 0x36F6: goto L36F6;
    case 0x36F9: goto L36F9;
    case 0x36FD: goto L36FD;
    case 0x3700: goto L3700;
    case 0x3704: goto L3704;
    case 0x3706: goto L3706;
    case 0x3708: goto L3708;
    case 0x370B: goto L370B;
    case 0x370F: goto L370F;
    case 0x3711: goto L3711;
    case 0x3713: goto L3713;
    case 0x3715: goto L3715;
    case 0x3717: goto L3717;
    case 0x3719: goto L3719;
    case 0x371B: goto L371B;
    case 0x371E: goto L371E;
    case 0x3720: goto L3720;
    case 0x3723: goto L3723;
    case 0x3727: goto L3727;
    case 0x372A: goto L372A;
    case 0x372E: goto L372E;
    case 0x3730: goto L3730;
    case 0x3732: goto L3732;
    case 0x3735: goto L3735;
    case 0x3739: goto L3739;
    case 0x373B: goto L373B;
    case 0x373D: goto L373D;
    case 0x373F: goto L373F;
    case 0x3741: goto L3741;
    case 0x3743: goto L3743;
    case 0x3745: goto L3745;
    case 0x3748: goto L3748;
    case 0x374A: goto L374A;
    case 0x374D: goto L374D;
    case 0x3751: goto L3751;
    case 0x3754: goto L3754;
    case 0x3758: goto L3758;
    case 0x375A: goto L375A;
    case 0x375C: goto L375C;
    case 0x375F: goto L375F;
    case 0x3763: goto L3763;
    case 0x3765: goto L3765;
    case 0x3767: goto L3767;
    case 0x3769: goto L3769;
    case 0x376B: goto L376B;
    case 0x376D: goto L376D;
    case 0x376F: goto L376F;
    case 0x3772: goto L3772;
    case 0x3774: goto L3774;
    case 0x3777: goto L3777;
    case 0x377B: goto L377B;
    case 0x377E: goto L377E;
    case 0x3782: goto L3782;
    case 0x3784: goto L3784;
    case 0x3786: goto L3786;
    case 0x3789: goto L3789;
    case 0x378D: goto L378D;
    case 0x378F: goto L378F;
    case 0x3791: goto L3791;
    case 0x3793: goto L3793;
    case 0x3795: goto L3795;
    case 0x3797: goto L3797;
    case 0x3799: goto L3799;
    case 0x379C: goto L379C;
    case 0x379E: goto L379E;
    case 0x37A1: goto L37A1;
    case 0x37A5: goto L37A5;
    case 0x37A8: goto L37A8;
    case 0x37AB: goto L37AB;
    case 0x37AE: goto L37AE;
    case 0x37B1: goto L37B1;
    case 0x37B4: goto L37B4;
    case 0x37B7: goto L37B7;
    case 0x37B8: goto L37B8;
    case 0x37BB: goto L37BB;
    case 0x37BF: goto L37BF;
    case 0x37C1: goto L37C1;
    case 0x37C3: goto L37C3;
    case 0x37C6: goto L37C6;
    case 0x37CA: goto L37CA;
    case 0x37CC: goto L37CC;
    case 0x37CE: goto L37CE;
    case 0x37D0: goto L37D0;
    case 0x37D2: goto L37D2;
    case 0x37D5: goto L37D5;
    case 0x37D9: goto L37D9;
    case 0x37DD: goto L37DD;
    case 0x37DF: goto L37DF;
    case 0x37E1: goto L37E1;
    case 0x37E4: goto L37E4;
    case 0x37E8: goto L37E8;
    case 0x37EA: goto L37EA;
    case 0x37EC: goto L37EC;
    case 0x37EE: goto L37EE;
    case 0x37F0: goto L37F0;
    case 0x37F4: goto L37F4;
    case 0x37F5: goto L37F5;
    case 0x37F7: goto L37F7;
    case 0x37F9: goto L37F9;
    case 0x37FE: goto L37FE;
    case 0x3800: goto L3800;
    case 0x3802: goto L3802;
    case 0x3805: goto L3805;
    case 0x3809: goto L3809;
    case 0x380B: goto L380B;
    case 0x380E: goto L380E;
    case 0x3811: goto L3811;
    case 0x3814: goto L3814;
    case 0x3818: goto L3818;
    case 0x381A: goto L381A;
    case 0x381C: goto L381C;
    case 0x381F: goto L381F;
    case 0x3823: goto L3823;
    case 0x3825: goto L3825;
    case 0x3827: goto L3827;
    case 0x3829: goto L3829;
    case 0x382B: goto L382B;
    case 0x382D: goto L382D;
    case 0x382F: goto L382F;
    case 0x3832: goto L3832;
    case 0x3834: goto L3834;
    case 0x3837: goto L3837;
    case 0x383B: goto L383B;
    case 0x383E: goto L383E;
    case 0x3842: goto L3842;
    case 0x3844: goto L3844;
    case 0x3846: goto L3846;
    case 0x3849: goto L3849;
    case 0x384D: goto L384D;
    case 0x384F: goto L384F;
    case 0x3851: goto L3851;
    case 0x3853: goto L3853;
    case 0x3855: goto L3855;
    case 0x3857: goto L3857;
    case 0x3859: goto L3859;
    case 0x385C: goto L385C;
    case 0x385E: goto L385E;
    case 0x3861: goto L3861;
    case 0x3865: goto L3865;
    case 0x3868: goto L3868;
    case 0x386C: goto L386C;
    case 0x386E: goto L386E;
    case 0x3870: goto L3870;
    case 0x3873: goto L3873;
    case 0x3877: goto L3877;
    case 0x3879: goto L3879;
    case 0x387B: goto L387B;
    case 0x387D: goto L387D;
    case 0x387F: goto L387F;
    case 0x3881: goto L3881;
    case 0x3883: goto L3883;
    case 0x3886: goto L3886;
    case 0x3888: goto L3888;
    case 0x388B: goto L388B;
    case 0x388F: goto L388F;
    case 0x3892: goto L3892;
    case 0x3896: goto L3896;
    case 0x3898: goto L3898;
    case 0x389A: goto L389A;
    case 0x389D: goto L389D;
    case 0x38A1: goto L38A1;
    case 0x38A3: goto L38A3;
    case 0x38A5: goto L38A5;
    case 0x38A7: goto L38A7;
    case 0x38A9: goto L38A9;
    case 0x38AB: goto L38AB;
    case 0x38AD: goto L38AD;
    case 0x38B0: goto L38B0;
    case 0x38B2: goto L38B2;
    case 0x38B5: goto L38B5;
    case 0x38B9: goto L38B9;
    case 0x38BC: goto L38BC;
    case 0x38C0: goto L38C0;
    case 0x38C2: goto L38C2;
    case 0x38C4: goto L38C4;
    case 0x38C7: goto L38C7;
    case 0x38CB: goto L38CB;
    case 0x38CD: goto L38CD;
    case 0x38CF: goto L38CF;
    case 0x38D1: goto L38D1;
    case 0x38D3: goto L38D3;
    case 0x38D5: goto L38D5;
    case 0x38D7: goto L38D7;
    case 0x38DA: goto L38DA;
    case 0x38DC: goto L38DC;
    case 0x38DF: goto L38DF;
    case 0x38E3: goto L38E3;
    case 0x38E6: goto L38E6;
    case 0x38EA: goto L38EA;
    case 0x38EC: goto L38EC;
    case 0x38EE: goto L38EE;
    case 0x38F1: goto L38F1;
    case 0x38F5: goto L38F5;
    case 0x38F7: goto L38F7;
    case 0x38F9: goto L38F9;
    case 0x38FB: goto L38FB;
    case 0x38FD: goto L38FD;
    case 0x38FF: goto L38FF;
    case 0x3901: goto L3901;
    case 0x3904: goto L3904;
    case 0x3906: goto L3906;
    case 0x3909: goto L3909;
    case 0x390D: goto L390D;
    case 0x3910: goto L3910;
    case 0x3913: goto L3913;
    case 0x3916: goto L3916;
    case 0x3919: goto L3919;
    case 0x391C: goto L391C;
    case 0x391F: goto L391F;
    case 0x3920: goto L3920;
    case 0x3923: goto L3923;
    case 0x3927: goto L3927;
    case 0x3929: goto L3929;
    case 0x392B: goto L392B;
    case 0x392E: goto L392E;
    case 0x3932: goto L3932;
    case 0x3934: goto L3934;
    case 0x3936: goto L3936;
    case 0x3938: goto L3938;
    case 0x393A: goto L393A;
    case 0x393D: goto L393D;
    case 0x3941: goto L3941;
    case 0x3945: goto L3945;
    case 0x3947: goto L3947;
    case 0x3949: goto L3949;
    case 0x394C: goto L394C;
    case 0x3950: goto L3950;
    case 0x3952: goto L3952;
    case 0x3954: goto L3954;
    case 0x3956: goto L3956;
    case 0x3958: goto L3958;
    case 0x395C: goto L395C;
    case 0x395D: goto L395D;
    case 0x395F: goto L395F;
    case 0x3961: goto L3961;
    case 0x3966: goto L3966;
    case 0x3968: goto L3968;
    case 0x396A: goto L396A;
    case 0x396D: goto L396D;
    case 0x3971: goto L3971;
    case 0x3973: goto L3973;
    case 0x3976: goto L3976;
    case 0x3979: goto L3979;
    case 0x397C: goto L397C;
    case 0x3980: goto L3980;
    case 0x3982: goto L3982;
    case 0x3984: goto L3984;
    case 0x3987: goto L3987;
    case 0x398B: goto L398B;
    case 0x398D: goto L398D;
    case 0x398F: goto L398F;
    case 0x3991: goto L3991;
    case 0x3993: goto L3993;
    case 0x3995: goto L3995;
    case 0x3997: goto L3997;
    case 0x399A: goto L399A;
    case 0x399C: goto L399C;
    case 0x399F: goto L399F;
    case 0x39A3: goto L39A3;
    case 0x39A6: goto L39A6;
    case 0x39AA: goto L39AA;
    case 0x39AC: goto L39AC;
    case 0x39AE: goto L39AE;
    case 0x39B1: goto L39B1;
    case 0x39B5: goto L39B5;
    case 0x39B7: goto L39B7;
    case 0x39B9: goto L39B9;
    case 0x39BB: goto L39BB;
    case 0x39BD: goto L39BD;
    case 0x39BF: goto L39BF;
    case 0x39C1: goto L39C1;
    case 0x39C4: goto L39C4;
    case 0x39C6: goto L39C6;
    case 0x39C9: goto L39C9;
    case 0x39CD: goto L39CD;
    case 0x39D0: goto L39D0;
    case 0x39D4: goto L39D4;
    case 0x39D6: goto L39D6;
    case 0x39D8: goto L39D8;
    case 0x39DB: goto L39DB;
    case 0x39DF: goto L39DF;
    case 0x39E1: goto L39E1;
    case 0x39E3: goto L39E3;
    case 0x39E5: goto L39E5;
    case 0x39E7: goto L39E7;
    case 0x39E9: goto L39E9;
    case 0x39EB: goto L39EB;
    case 0x39EE: goto L39EE;
    case 0x39F0: goto L39F0;
    case 0x39F3: goto L39F3;
    case 0x39F7: goto L39F7;
    case 0x39FA: goto L39FA;
    case 0x39FE: goto L39FE;
    case 0x3A00: goto L3A00;
    case 0x3A02: goto L3A02;
    case 0x3A05: goto L3A05;
    case 0x3A09: goto L3A09;
    case 0x3A0B: goto L3A0B;
    case 0x3A0D: goto L3A0D;
    case 0x3A0F: goto L3A0F;
    case 0x3A11: goto L3A11;
    case 0x3A13: goto L3A13;
    case 0x3A15: goto L3A15;
    case 0x3A18: goto L3A18;
    case 0x3A1A: goto L3A1A;
    case 0x3A1D: goto L3A1D;
    case 0x3A21: goto L3A21;
    case 0x3A24: goto L3A24;
    case 0x3A28: goto L3A28;
    case 0x3A2A: goto L3A2A;
    case 0x3A2C: goto L3A2C;
    case 0x3A2F: goto L3A2F;
    case 0x3A33: goto L3A33;
    case 0x3A35: goto L3A35;
    case 0x3A37: goto L3A37;
    case 0x3A39: goto L3A39;
    case 0x3A3B: goto L3A3B;
    case 0x3A3D: goto L3A3D;
    case 0x3A3F: goto L3A3F;
    case 0x3A42: goto L3A42;
    case 0x3A44: goto L3A44;
    case 0x3A47: goto L3A47;
    case 0x3A4B: goto L3A4B;
    case 0x3A4E: goto L3A4E;
    case 0x3A52: goto L3A52;
    case 0x3A54: goto L3A54;
    case 0x3A56: goto L3A56;
    case 0x3A59: goto L3A59;
    case 0x3A5D: goto L3A5D;
    case 0x3A5F: goto L3A5F;
    case 0x3A61: goto L3A61;
    case 0x3A63: goto L3A63;
    case 0x3A65: goto L3A65;
    case 0x3A67: goto L3A67;
    case 0x3A69: goto L3A69;
    case 0x3A6C: goto L3A6C;
    case 0x3A6E: goto L3A6E;
    case 0x3A71: goto L3A71;
    case 0x3A75: goto L3A75;
    case 0x3A78: goto L3A78;
    case 0x3A7B: goto L3A7B;
    case 0x3A7E: goto L3A7E;
    case 0x3A81: goto L3A81;
    case 0x3A84: goto L3A84;
    case 0x3A87: goto L3A87;
    case 0x3A88: goto L3A88;
    case 0x3A8A: goto L3A8A;
    case 0x3A8C: goto L3A8C;
    case 0x3A8E: goto L3A8E;
    case 0x3A90: goto L3A90;
    case 0x3A92: goto L3A92;
    case 0x3A94: goto L3A94;
    case 0x3A96: goto L3A96;
    case 0x3A98: goto L3A98;
    case 0x3A9A: goto L3A9A;
    case 0x3A9C: goto L3A9C;
    case 0x3A9E: goto L3A9E;
    case 0x3AA0: goto L3AA0;
    case 0x3AA1: goto L3AA1;
    case 0x3AA3: goto L3AA3;
    case 0x3AA5: goto L3AA5;
    case 0x3AA7: goto L3AA7;
    case 0x3AA9: goto L3AA9;
    case 0x3AAB: goto L3AAB;
    case 0x3AAC: goto L3AAC;
    case 0x3AAE: goto L3AAE;
    case 0x3AAF: goto L3AAF;
    case 0x3AB1: goto L3AB1;
    case 0x3AB2: goto L3AB2;
    case 0x3AB4: goto L3AB4;
    case 0x3AB6: goto L3AB6;
    case 0x3AB8: goto L3AB8;
    case 0x3ABA: goto L3ABA;
    case 0x3ABC: goto L3ABC;
    case 0x3ABD: goto L3ABD;
    case 0x3ABF: goto L3ABF;
    case 0x3AC0: goto L3AC0;
    case 0x3AC2: goto L3AC2;
    case 0x3AC3: goto L3AC3;
    case 0x3AC5: goto L3AC5;
    case 0x3AC6: goto L3AC6;
    case 0x3AC8: goto L3AC8;
    case 0x3AC9: goto L3AC9;
    case 0x3ACB: goto L3ACB;
    case 0x3ACD: goto L3ACD;
    case 0x3ACF: goto L3ACF;
    case 0x3AD1: goto L3AD1;
    case 0x3AD3: goto L3AD3;
    case 0x3AD4: goto L3AD4;
    case 0x3AD6: goto L3AD6;
    case 0x3AD8: goto L3AD8;
    case 0x3ADA: goto L3ADA;
    case 0x3ADC: goto L3ADC;
    case 0x3ADE: goto L3ADE;
    case 0x3ADF: goto L3ADF;
    case 0x3AE1: goto L3AE1;
    case 0x3AE3: goto L3AE3;
    case 0x3AE5: goto L3AE5;
    case 0x3AE6: goto L3AE6;
    case 0x3AE8: goto L3AE8;
    case 0x3AEA: goto L3AEA;
    case 0x3AEC: goto L3AEC;
    case 0x3AED: goto L3AED;
    case 0x3AEF: goto L3AEF;
    case 0x3AF0: goto L3AF0;
    case 0x3AF2: goto L3AF2;
    case 0x3AF4: goto L3AF4;
    case 0x3AF6: goto L3AF6;
    case 0x3AF7: goto L3AF7;
    case 0x3AF9: goto L3AF9;
    case 0x3AFA: goto L3AFA;
    case 0x3AFC: goto L3AFC;
    case 0x3AFE: goto L3AFE;
    case 0x3B00: goto L3B00;
    case 0x3B01: goto L3B01;
    case 0x3B03: goto L3B03;
    case 0x3B04: goto L3B04;
    case 0x3B07: goto L3B07;
    case 0x3B0C: goto L3B0C;
    case 0x3B11: goto L3B11;
    case 0x3B16: goto L3B16;
    case 0x3B19: goto L3B19;
    case 0x3B1C: goto L3B1C;
    case 0x3B21: goto L3B21;
    case 0x3B26: goto L3B26;
    case 0x3B2B: goto L3B2B;
    case 0x3B2E: goto L3B2E;
    case 0x3B31: goto L3B31;
    case 0x3B36: goto L3B36;
    case 0x3B3B: goto L3B3B;
    case 0x3B40: goto L3B40;
    case 0x3B41: goto L3B41;
    case 0x3B44: goto L3B44;
    case 0x3B46: goto L3B46;
    case 0x3B48: goto L3B48;
    case 0x3B4A: goto L3B4A;
    case 0x3B4D: goto L3B4D;
    case 0x3B50: goto L3B50;
    case 0x3B52: goto L3B52;
    case 0x3B54: goto L3B54;
    case 0x3B57: goto L3B57;
    case 0x3B5A: goto L3B5A;
    case 0x3B5C: goto L3B5C;
    case 0x3B5E: goto L3B5E;
    case 0x3B60: goto L3B60;
    case 0x3B62: goto L3B62;
    case 0x3B64: goto L3B64;
    case 0x3B66: goto L3B66;
    case 0x3B67: goto L3B67;
    case 0x3B6A: goto L3B6A;
    case 0x3B6C: goto L3B6C;
    case 0x3B6E: goto L3B6E;
    case 0x3B72: goto L3B72;
    case 0x3B74: goto L3B74;
    case 0x3B77: goto L3B77;
    case 0x3B78: goto L3B78;
    case 0x3B7B: goto L3B7B;
    case 0x3B7E: goto L3B7E;
    case 0x3B80: goto L3B80;
    case 0x3B82: goto L3B82;
    case 0x3B85: goto L3B85;
    case 0x3B88: goto L3B88;
    case 0x3B8A: goto L3B8A;
    case 0x3B8C: goto L3B8C;
    case 0x3B8F: goto L3B8F;
    case 0x3B92: goto L3B92;
    case 0x3B94: goto L3B94;
    case 0x3B96: goto L3B96;
    case 0x3B98: goto L3B98;
    case 0x3B9A: goto L3B9A;
    case 0x3B9C: goto L3B9C;
    case 0x3B9E: goto L3B9E;
    case 0x3B9F: goto L3B9F;
    case 0x3BA2: goto L3BA2;
    case 0x3BA4: goto L3BA4;
    case 0x3BA6: goto L3BA6;
    case 0x3BAA: goto L3BAA;
    case 0x3BAC: goto L3BAC;
    case 0x3BAF: goto L3BAF;
    case 0x3BB2: goto L3BB2;
    case 0x3BB5: goto L3BB5;
    case 0x3BB7: goto L3BB7;
    case 0x3BB9: goto L3BB9;
    case 0x3BBC: goto L3BBC;
    case 0x3BBF: goto L3BBF;
    case 0x3BC1: goto L3BC1;
    case 0x3BC3: goto L3BC3;
    case 0x3BC6: goto L3BC6;
    case 0x3BC9: goto L3BC9;
    case 0x3BCB: goto L3BCB;
    case 0x3BCD: goto L3BCD;
    case 0x3BCF: goto L3BCF;
    case 0x3BD1: goto L3BD1;
    case 0x3BD3: goto L3BD3;
    case 0x3BD5: goto L3BD5;
    case 0x3BD6: goto L3BD6;
    case 0x3BD9: goto L3BD9;
    case 0x3BDB: goto L3BDB;
    case 0x3BDD: goto L3BDD;
    case 0x3BE1: goto L3BE1;
    case 0x3BE3: goto L3BE3;
    case 0x3BE6: goto L3BE6;
    case 0x3BE7: goto L3BE7;
    case 0x3BE8: goto L3BE8;
    case 0x3BEA: goto L3BEA;
    case 0x3BED: goto L3BED;
    case 0x3BEF: goto L3BEF;
    case 0x3BF1: goto L3BF1;
    case 0x3BF4: goto L3BF4;
    case 0x3BF7: goto L3BF7;
    case 0x3BF9: goto L3BF9;
    case 0x3BFB: goto L3BFB;
    case 0x3BFE: goto L3BFE;
    case 0x3C01: goto L3C01;
    case 0x3C03: goto L3C03;
    case 0x3C05: goto L3C05;
    case 0x3C07: goto L3C07;
    case 0x3C09: goto L3C09;
    case 0x3C0B: goto L3C0B;
    case 0x3C0D: goto L3C0D;
    case 0x3C0E: goto L3C0E;
    case 0x3C11: goto L3C11;
    case 0x3C13: goto L3C13;
    case 0x3C15: goto L3C15;
    case 0x3C19: goto L3C19;
    case 0x3C1B: goto L3C1B;
    case 0x3C1E: goto L3C1E;
    case 0x3C1F: goto L3C1F;
    case 0x3C21: goto L3C21;
    case 0x3C24: goto L3C24;
    case 0x3C26: goto L3C26;
    case 0x3C28: goto L3C28;
    case 0x3C2B: goto L3C2B;
    case 0x3C2E: goto L3C2E;
    case 0x3C30: goto L3C30;
    case 0x3C32: goto L3C32;
    case 0x3C35: goto L3C35;
    case 0x3C38: goto L3C38;
    case 0x3C3A: goto L3C3A;
    case 0x3C3C: goto L3C3C;
    case 0x3C3E: goto L3C3E;
    case 0x3C40: goto L3C40;
    case 0x3C42: goto L3C42;
    case 0x3C44: goto L3C44;
    case 0x3C45: goto L3C45;
    case 0x3C48: goto L3C48;
    case 0x3C4A: goto L3C4A;
    case 0x3C4C: goto L3C4C;
    case 0x3C50: goto L3C50;
    case 0x3C52: goto L3C52;
    case 0x3C55: goto L3C55;
    case 0x3C57: goto L3C57;
    case 0x3C5A: goto L3C5A;
    case 0x3C5C: goto L3C5C;
    case 0x3C5E: goto L3C5E;
    case 0x3C61: goto L3C61;
    case 0x3C64: goto L3C64;
    case 0x3C66: goto L3C66;
    case 0x3C68: goto L3C68;
    case 0x3C6B: goto L3C6B;
    case 0x3C6E: goto L3C6E;
    case 0x3C70: goto L3C70;
    case 0x3C72: goto L3C72;
    case 0x3C74: goto L3C74;
    case 0x3C76: goto L3C76;
    case 0x3C78: goto L3C78;
    case 0x3C7A: goto L3C7A;
    case 0x3C7B: goto L3C7B;
    case 0x3C7E: goto L3C7E;
    case 0x3C80: goto L3C80;
    case 0x3C82: goto L3C82;
    case 0x3C86: goto L3C86;
    case 0x3C88: goto L3C88;
    case 0x3C8B: goto L3C8B;
    case 0x3C8C: goto L3C8C;
    case 0x3C91: goto L3C91;
    case 0x3C96: goto L3C96;
    case 0x3C98: goto L3C98;
    case 0x3C9B: goto L3C9B;
    case 0x3C9E: goto L3C9E;
    case 0x3CA1: goto L3CA1;
    case 0x3CA5: goto L3CA5;
    case 0x3CA7: goto L3CA7;
    case 0x3CA9: goto L3CA9;
    case 0x3CAA: goto L3CAA;
    case 0x3CAF: goto L3CAF;
    case 0x3CB4: goto L3CB4;
    case 0x3CB5: goto L3CB5;
    case 0x3CBA: goto L3CBA;
    case 0x3CBC: goto L3CBC;
    case 0x3CC0: goto L3CC0;
    case 0x3CC3: goto L3CC3;
    case 0x3CC5: goto L3CC5;
    case 0x3CCA: goto L3CCA;
    case 0x3CCB: goto L3CCB;
    case 0x3CD0: goto L3CD0;
    case 0x3CD2: goto L3CD2;
    case 0x3CD5: goto L3CD5;
    case 0x3CD8: goto L3CD8;
    case 0x3CDD: goto L3CDD;
    case 0x3CDF: goto L3CDF;
    case 0x3CE1: goto L3CE1;
    case 0x3CE2: goto L3CE2;
    case 0x3CE5: goto L3CE5;
    case 0x3CE7: goto L3CE7;
    case 0x3CE9: goto L3CE9;
    case 0x3CEB: goto L3CEB;
    default: asm_bad_entry("INSTANCE.ASM", entry);
    }

    /* seg004_0849_3000  (+3000)
       seg004_0849_3000: an opcode handler that breaks into the debugger (int 3) and goes on to the
       next opcode; probably the table's entry for unused opcodes. */
L3000: /* _seg004_0849_3000 */
    /* 3000  int     3 */
    asm_halt_at(0x065C, 0x3000, "int 3, the debugger break");
L3001:
    /* 3001  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3002:
    /* 3002  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3003:
    /* 3003  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* render_3d  (+3007): FM render_3d
       render_3d: draw the frame whose render database starts at DbEntry. Called by cRender
       (src/sys/C3DENTRY.ASM) with SS on cRender's private stack in FD71 (dseg062_62a6, seg021's data
       segment), where it reads the view window: the copy of the clip window that set_the_window
       (VIDMODE.ASM) keeps at FD71:0C30, 0C30 left, 0C32 top, 0C34 right, 0C36 bottom, y counting up
       from the bottom of the screen. Works out scrw, scrh, biasx and biasy from it and patches biasx
       and biasy into the projection code; counts the frame; builds the view matrix (create_matrix in
       INTERP.ASM, then seg004_0849_35FF, scale_matrix, check_flat).

       It then switches to a stack in seg_370D (SP 5548h) with seg_370D:2CEE set to 9C9h, and calls
       seg021_22FD_C10, which only returns (SYSLIBP.ASM): a hook that does nothing in this build.
       Finally it restores the stack and runs the database: the first opcode at DbEntry is called
       through the opcode table, and the chain of handlers returns here at the end of the frame
       (do_eof). Out: DS = ES = SS; seg_370D:2CEE restored. */
L3007: /* _render_3d */
    /* 3007  mov     ax,seg seg052_519C */
    AX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L300A:
    /* 300A  mov     ds,ax */
    SET_DS(AX);
L300C:
    /* 300C  mov     es,ax */
    SET_ES(AX);
L300E:
    /* 300E  mov     ax,word ptr ss:[0C34h] */
    AX = rw(pSS, 0xC34);
L3012:
    /* 3012  sub     ax,word ptr ss:[0C30h] */
    AX = (uint16_t)(AX - rw(pSS, 0xC30));
L3017:
    /* 3017  inc     ax */
    AX = (uint16_t)(AX + 1);
L3018:
    /* 3018  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L301A:
    /* 301A  mov     word ptr ds:[2472h],ax */
    ww(pDS, 0x2472, AX);
L301D:
    /* 301D  mov     ax,word ptr ss:[0C32h] */
    AX = rw(pSS, 0xC32);
L3021:
    /* 3021  sub     ax,word ptr ss:[0C36h] */
    AX = (uint16_t)(AX - rw(pSS, 0xC36));
L3026:
    /* 3026  inc     ax */
    AX = (uint16_t)(AX + 1);
L3027:
    /* 3027  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L3029:
    /* 3029  mov     word ptr ds:[2470h],ax */
    ww(pDS, 0x2470, AX);
L302C:
    /* 302C  mov     ax,word ptr ss:[0C34h] */
    AX = rw(pSS, 0xC34);
L3030:
    /* 3030  add     ax,word ptr ss:[0C30h] */
    AX = (uint16_t)(AX + rw(pSS, 0xC30));
L3035:
    /* 3035  inc     ax */
    AX = (uint16_t)(AX + 1);
L3036:
    /* 3036  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L3038:
    /* 3038  mov     word ptr ds:[2474h],ax */
    ww(pDS, 0x2474, AX);
L303B:
    /* 303B  mov     word ptr cs:[_seg004_0849_644+1],ax */
    ww(CODE004, 0x645, AX);
L303F:
    /* 303F  mov     word ptr cs:[_seg004_0849_6B2+1],ax */
    ww(CODE004, 0x6B3, AX);
L3043:
    /* 3043  mov     word ptr cs:[modify_biasx+1],ax */
    ww(CODE004, 0x17B3, AX);
L3047:
    /* 3047  mov     word ptr cs:[modify_bx2+1],ax */
    ww(CODE004, 0x1814, AX);
L304B:
    /* 304B  mov     ax,word ptr ss:[0C32h] */
    AX = rw(pSS, 0xC32);
L304F:
    /* 304F  add     ax,word ptr ss:[0C36h] */
    AX = (uint16_t)(AX + rw(pSS, 0xC36));
L3054:
    /* 3054  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L3056:
    /* 3056  mov     word ptr ds:[2476h],ax */
    ww(pDS, 0x2476, AX);
L3059:
    /* 3059  mov     word ptr cs:[_seg004_0849_64F+1],ax */
    ww(CODE004, 0x650, AX);
L305D:
    /* 305D  mov     word ptr cs:[_seg004_0849_6C2+1],ax */
    ww(CODE004, 0x6C3, AX);
L3061:
    /* 3061  mov     word ptr cs:[modify_biasy+1],ax */
    ww(CODE004, 0x17BF, AX);
L3065:
    /* 3065  mov     word ptr cs:[modify_by2+1],ax */
    ww(CODE004, 0x1825, AX);
L3069:
    /* 3069  inc     word ptr ds:[14AAh] */
    ww(pDS, 0x14AA, (uint16_t)(rw(pDS, 0x14AA) + 1));
L306D:
    /* 306D  mov     byte ptr ds:[25E0h],0 */
    wb(pDS, 0x25E0, 0x0);
L3072:
    /* 3072  call    _create_matrix */
    if ((c = asm_call(ASM_JMP(0x065C, 0x0E15), 0x3075)) != 0) return c;
L3075:
    /* 3075  call    _seg004_0849_35FF */
    if ((c = asm_call(ASM_JMP(0x065C, 0x35FF), 0x3078)) != 0) return c;
L3078:
    /* 3078  call    _scale_matrix */
    if ((c = asm_call(ASM_JMP(0x065C, 0x30DC), 0x307B)) != 0) return c;
L307B:
    /* 307B  call    _check_flat */
    if ((c = asm_call(ASM_JMP(0x065C, 0x340E), 0x307E)) != 0) return c;
L307E:
    /* 307E  mov     ax,ss */
    AX = asm_ss;
L3080:
    /* 3080  mov     word ptr ds:[14A2h],ax */
    ww(pDS, 0x14A2, AX);
L3083:
    /* 3083  mov     word ptr ds:[14A0h],sp */
    ww(pDS, 0x14A0, SP);
L3087:
    /* 3087  mov     si,seg seg_370D */
    SI = (uint16_t)(0x370D + PORT_LOAD_SEG);
L308A:
    /* 308A  mov     ds,si */
    SET_DS(SI);
L308C:
    /* 308C  mov     ax,word ptr ds:[2CEEh] */
    AX = rw(pDS, 0x2CEE);
L308F:
    /* 308F  mov     word ptr cs:L30DA,ax */
    ww(CODE004, 0x30DA, AX);
L3093:
    /* 3093  mov     word ptr ds:[2CEEh],9C9h */
    ww(pDS, 0x2CEE, 0x9C9);
L3099:
    /* 3099  mov     ax,seg dseg062_62a6 */
    AX = (uint16_t)(0x60B9 + PORT_LOAD_SEG);
L309C:
    /* 309C  mov     es,ax */
    SET_ES(AX);
L309E:
    /* 309E  mov     es,si */
    SET_ES(SI);
L30A0:
    /* 30A0  cli */
    ;
L30A1:
    /* 30A1  mov     ss,si */
    SET_SS(SI);
L30A3:
    /* 30A3  mov     sp,5548h */
    SP = 0x5548;
L30A6:
    /* 30A6  sti */
    ;
L30A7:
    /* 30A7  call    far ptr _seg021_22FD_C10 */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0C10), 0x065C + PORT_LOAD_SEG, 0x30AC)) != 0) return c;
L30AC:
    /* 30AC  mov     bp,seg seg052_519C */
    BP = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L30AF:
    /* 30AF  mov     ds,bp */
    SET_DS(BP);
L30B1:
    /* 30B1  mov     es,bp */
    SET_ES(BP);
L30B3:
    /* 30B3  cli */
    ;
L30B4:
    /* 30B4  mov     ss,word ptr ds:[14A2h] */
    SET_SS(rw(pDS, 0x14A2));
L30B8:
    /* 30B8  mov     sp,word ptr ds:[14A0h] */
    SP = rw(pDS, 0x14A0);
L30BC:
    /* 30BC  sti */
    ;
L30BD:
    /* 30BD  mov     si,word ptr ds:[14CCh] */
    SI = rw(pDS, 0x14CC);
L30C1:
    /* 30C1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30C2:
    /* 30C2  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L30C3:
    /* 30C3  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x30C7)) != 0) return c;
L30C7:
    /* 30C7  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L30CA:
    /* 30CA  mov     ds,ax */
    SET_DS(AX);
L30CC:
    /* 30CC  mov     ax,word ptr cs:L30DA */
    AX = rw(CODE004, 0x30DA);
L30D0:
    /* 30D0  mov     word ptr ds:[2CEEh],ax */
    ww(pDS, 0x2CEE, AX);
L30D3:
    /* 30D3  mov     ax,ss */
    AX = asm_ss;
L30D5:
    /* 30D5  mov     ds,ax */
    SET_DS(AX);
L30D7:
    /* 30D7  mov     es,ax */
    SET_ES(AX);
L30D9:
    /* 30D9  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_0849_30DC  (+30DC)
       scale_matrix: fold the screen's shape and the zoom into the view matrix. Scales matrix entries
       by the ratio of the view's half width to half height (with 14A4, whose sign picks which axis
       is scaled; probably so x and y project to the square frustum CLIP.ASM's 45-degree planes
       assume) and by the zoom word 24E6 (the depth column when it is positive, the other entries
       when negative), and works out 248C/248E, a unit horizontal direction on the screen, and from
       it 2490/2492, the axes do_scalebm (TMAPOPS.ASM) uses to place sprites facing the viewer.
       Installs _overflow_handler_reg for the divides: cRender points int 0 at FD71:05A0, which holds
       `jmp far cs:[5A5]`, and the far pointer at 5A5 (FM overflow_ptr) is into seg004, so writing
       its offset word selects the handler. Uses SquareRoot_seg021_22FD_A30 (IMATH.ASM). */
L30DC: /* _scale_matrix */
    /* 30DC  mov     word ptr ds:[24E8h],7FFFh */
    ww(pDS, 0x24E8, 0x7FFF);
L30E2:
    /* 30E2  mov     word ptr ds:[24EAh],7FFFh */
    ww(pDS, 0x24EA, 0x7FFF);
L30E8:
    /* 30E8  mov     ax,word ptr ds:[14B8h] */
    AX = rw(pDS, 0x14B8);
L30EB:
    /* 30EB  mov     word ptr ds:[14C4h],ax */
    ww(pDS, 0x14C4, AX);
L30EE:
    /* 30EE  mov     ax,word ptr ds:[14BAh] */
    AX = rw(pDS, 0x14BA);
L30F1:
    /* 30F1  mov     word ptr ds:[14C6h],ax */
    ww(pDS, 0x14C6, AX);
L30F4:
    /* 30F4  mov     ax,word ptr ds:[14BCh] */
    AX = rw(pDS, 0x14BC);
L30F7:
    /* 30F7  mov     word ptr ds:[14C8h],ax */
    ww(pDS, 0x14C8, AX);
L30FA:
    /* 30FA  mov     bx,word ptr ds:[2470h] */
    BX = rw(pDS, 0x2470);
L30FE:
    /* 30FE  mov     cx,word ptr ds:[2472h] */
    CX = rw(pDS, 0x2472);
L3102:
    /* 3102  mov     ax,word ptr ds:[14A4h] */
    AX = rw(pDS, 0x14A4);
L3105:
    /* 3105  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L3107:
    /* 3107  js      short L3113 */
    if (SF) goto L3113;
L3109:
    /* 3109  imul    cx */
    imul16(CX);
L310B:
    /* 310B  shl     ax,1 */
    AX = shl16(AX, 1);
L310D:
    /* 310D  rcl     dx,1 */
    DX = rcl16(DX, 1);
L310F:
    /* 310F  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L3111:
    /* 3111  jmp     short L3123 */
    goto L3123;
L3113: /* L3113 */
    /* 3113  neg     ax */
    AX = (uint16_t)-AX;
L3115:
    /* 3115  imul    bx */
    imul16(BX);
L3117:
    /* 3117  shl     ax,1 */
    AX = shl16(AX, 1);
L3119:
    /* 3119  rcl     dx,1 */
    DX = rcl16(DX, 1);
L311B:
    /* 311B  mov     bx,dx */
    BX = DX;
L311D:
    /* 311D  mov     dx,cx */
    DX = CX;
L311F:
    /* 311F  mov     cx,ax */
    CX = AX;
L3121:
    /* 3121  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L3123: /* L3123 */
    /* 3123  cmp     dx,bx */
    sub16(DX, BX, 0);
L3125:
    /* 3125  jne     short L312A */
    if (!ZF) goto L312A;
L3127:
    /* 3127  jmp     L31CB */
    goto L31CB;
L312A: /* L312A */
    /* 312A  jl      short L317E */
    if (SF != OF) goto L317E;
L312C:
    /* 312C  xchg    dx,bx */
    { uint16_t t_ = BX;
    BX = DX;
    DX = t_; }
L312E:
    /* 312E  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L312F:
    /* 312F  shr     dx,1 */
    DX = shr16(DX, 1);
L3131:
    /* 3131  rcr     ax,1 */
    AX = rcr16(AX, 1);
L3133:
    /* 3133  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x065C, 0x3133, 2)) != 0) return c;
L3135:
    /* 3135  mov     cx,ax */
    CX = AX;
L3137:
    /* 3137  mov     word ptr ds:[24E8h],cx */
    ww(pDS, 0x24E8, CX);
L313B:
    /* 313B  mov     ax,word ptr ds:[14B2h] */
    AX = rw(pDS, 0x14B2);
L313E:
    /* 313E  imul    cx */
    imul16(CX);
L3140:
    /* 3140  shl     ax,1 */
    AX = shl16(AX, 1);
L3142:
    /* 3142  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3144:
    /* 3144  mov     word ptr ds:[14B2h],dx */
    ww(pDS, 0x14B2, DX);
L3148:
    /* 3148  mov     ax,word ptr ds:[14B8h] */
    AX = rw(pDS, 0x14B8);
L314B:
    /* 314B  imul    cx */
    imul16(CX);
L314D:
    /* 314D  shl     ax,1 */
    AX = shl16(AX, 1);
L314F:
    /* 314F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3151:
    /* 3151  mov     word ptr ds:[14B8h],dx */
    ww(pDS, 0x14B8, DX);
L3155:
    /* 3155  mov     ax,word ptr ds:[14BEh] */
    AX = rw(pDS, 0x14BE);
L3158:
    /* 3158  imul    cx */
    imul16(CX);
L315A:
    /* 315A  shl     ax,1 */
    AX = shl16(AX, 1);
L315C:
    /* 315C  rcl     dx,1 */
    DX = rcl16(DX, 1);
L315E:
    /* 315E  mov     word ptr ds:[14BEh],dx */
    ww(pDS, 0x14BE, DX);
L3162:
    /* 3162  mov     ax,word ptr ds:[14C6h] */
    AX = rw(pDS, 0x14C6);
L3165:
    /* 3165  imul    cx */
    imul16(CX);
L3167:
    /* 3167  shl     ax,1 */
    AX = shl16(AX, 1);
L3169:
    /* 3169  rcl     dx,1 */
    DX = rcl16(DX, 1);
L316B:
    /* 316B  mov     word ptr ds:[14C6h],dx */
    ww(pDS, 0x14C6, DX);
L316F:
    /* 316F  mov     ax,word ptr ds:[14C8h] */
    AX = rw(pDS, 0x14C8);
L3172:
    /* 3172  imul    cx */
    imul16(CX);
L3174:
    /* 3174  shl     ax,1 */
    AX = shl16(AX, 1);
L3176:
    /* 3176  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3178:
    /* 3178  mov     word ptr ds:[14C8h],dx */
    ww(pDS, 0x14C8, DX);
L317C:
    /* 317C  jmp     short L31CB */
    goto L31CB;
L317E: /* L317E */
    /* 317E  shr     dx,1 */
    DX = shr16(DX, 1);
L3180:
    /* 3180  rcr     ax,1 */
    AX = rcr16(AX, 1);
L3182:
    /* 3182  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x065C, 0x3182, 2)) != 0) return c;
L3184:
    /* 3184  mov     cx,ax */
    CX = AX;
L3186:
    /* 3186  mov     word ptr ds:[24EAh],cx */
    ww(pDS, 0x24EA, CX);
L318A:
    /* 318A  mov     ax,word ptr ds:[14B4h] */
    AX = rw(pDS, 0x14B4);
L318D:
    /* 318D  imul    cx */
    imul16(CX);
L318F:
    /* 318F  shl     ax,1 */
    AX = shl16(AX, 1);
L3191:
    /* 3191  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3193:
    /* 3193  mov     word ptr ds:[14B4h],dx */
    ww(pDS, 0x14B4, DX);
L3197:
    /* 3197  mov     ax,word ptr ds:[14BAh] */
    AX = rw(pDS, 0x14BA);
L319A:
    /* 319A  imul    cx */
    imul16(CX);
L319C:
    /* 319C  shl     ax,1 */
    AX = shl16(AX, 1);
L319E:
    /* 319E  rcl     dx,1 */
    DX = rcl16(DX, 1);
L31A0:
    /* 31A0  mov     word ptr ds:[14BAh],dx */
    ww(pDS, 0x14BA, DX);
L31A4:
    /* 31A4  mov     ax,word ptr ds:[14C0h] */
    AX = rw(pDS, 0x14C0);
L31A7:
    /* 31A7  imul    cx */
    imul16(CX);
L31A9:
    /* 31A9  shl     ax,1 */
    AX = shl16(AX, 1);
L31AB:
    /* 31AB  rcl     dx,1 */
    DX = rcl16(DX, 1);
L31AD:
    /* 31AD  mov     word ptr ds:[14C0h],dx */
    ww(pDS, 0x14C0, DX);
L31B1:
    /* 31B1  mov     ax,word ptr ds:[14C4h] */
    AX = rw(pDS, 0x14C4);
L31B4:
    /* 31B4  imul    cx */
    imul16(CX);
L31B6:
    /* 31B6  shl     ax,1 */
    AX = shl16(AX, 1);
L31B8:
    /* 31B8  rcl     dx,1 */
    DX = rcl16(DX, 1);
L31BA:
    /* 31BA  mov     word ptr ds:[14C4h],dx */
    ww(pDS, 0x14C4, DX);
L31BE:
    /* 31BE  mov     ax,word ptr ds:[14C8h] */
    AX = rw(pDS, 0x14C8);
L31C1:
    /* 31C1  imul    cx */
    imul16(CX);
L31C3:
    /* 31C3  shl     ax,1 */
    AX = shl16(AX, 1);
L31C5:
    /* 31C5  rcl     dx,1 */
    DX = rcl16(DX, 1);
L31C7:
    /* 31C7  mov     word ptr ds:[14C8h],dx */
    ww(pDS, 0x14C8, DX);
L31CB: /* L31CB */
    /* 31CB  mov     cx,word ptr ds:[24E6h] */
    CX = rw(pDS, 0x24E6);
L31CF:
    /* 31CF  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L31D1:
    /* 31D1  jns     short L31D6 */
    if (!SF) goto L31D6;
L31D3:
    /* 31D3  jmp     L325E */
    goto L325E;
L31D6: /* L31D6 */
    /* 31D6  mov     word ptr ds:[24F0h],cx */
    ww(pDS, 0x24F0, CX);
L31DA:
    /* 31DA  mov     ax,word ptr ds:[14B6h] */
    AX = rw(pDS, 0x14B6);
L31DD:
    /* 31DD  imul    cx */
    imul16(CX);
L31DF:
    /* 31DF  shl     ax,1 */
    AX = shl16(AX, 1);
L31E1:
    /* 31E1  rcl     dx,1 */
    DX = rcl16(DX, 1);
L31E3:
    /* 31E3  mov     word ptr ds:[14B6h],dx */
    ww(pDS, 0x14B6, DX);
L31E7:
    /* 31E7  mov     ax,word ptr ds:[14BCh] */
    AX = rw(pDS, 0x14BC);
L31EA:
    /* 31EA  imul    cx */
    imul16(CX);
L31EC:
    /* 31EC  shl     ax,1 */
    AX = shl16(AX, 1);
L31EE:
    /* 31EE  rcl     dx,1 */
    DX = rcl16(DX, 1);
L31F0:
    /* 31F0  mov     word ptr ds:[14BCh],dx */
    ww(pDS, 0x14BC, DX);
L31F4:
    /* 31F4  mov     ax,word ptr ds:[14C2h] */
    AX = rw(pDS, 0x14C2);
L31F7:
    /* 31F7  imul    cx */
    imul16(CX);
L31F9:
    /* 31F9  shl     ax,1 */
    AX = shl16(AX, 1);
L31FB:
    /* 31FB  rcl     dx,1 */
    DX = rcl16(DX, 1);
L31FD:
    /* 31FD  mov     word ptr ds:[14C2h],dx */
    ww(pDS, 0x14C2, DX);
L3201:
    /* 3201  mov     ax,word ptr ds:[14C6h] */
    AX = rw(pDS, 0x14C6);
L3204:
    /* 3204  imul    cx */
    imul16(CX);
L3206:
    /* 3206  shl     ax,1 */
    AX = shl16(AX, 1);
L3208:
    /* 3208  rcl     dx,1 */
    DX = rcl16(DX, 1);
L320A:
    /* 320A  mov     word ptr ds:[14C6h],dx */
    ww(pDS, 0x14C6, DX);
L320E:
    /* 320E  mov     ax,word ptr ds:[14C4h] */
    AX = rw(pDS, 0x14C4);
L3211:
    /* 3211  imul    cx */
    imul16(CX);
L3213:
    /* 3213  shl     ax,1 */
    AX = shl16(AX, 1);
L3215:
    /* 3215  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3217:
    /* 3217  mov     word ptr ds:[14C4h],dx */
    ww(pDS, 0x14C4, DX);
L321B:
    /* 321B  mov     ax,cx */
    AX = CX;
L321D:
    /* 321D  imul    ax */
    imul16(AX);
L321F:
    /* 321F  mov     bp,ax */
    BP = AX;
L3221:
    /* 3221  mov     si,dx */
    SI = DX;
L3223:
    /* 3223  mov     ax,word ptr ds:[24E8h] */
    AX = rw(pDS, 0x24E8);
L3226:
    /* 3226  mov     word ptr ds:[24ECh],ax */
    ww(pDS, 0x24EC, AX);
L3229:
    /* 3229  imul    ax */
    imul16(AX);
L322B:
    /* 322B  add     ax,bp */
    AX = add16(AX, BP, 0);
L322D:
    /* 322D  adc     dx,si */
    DX = (uint16_t)(DX + SI + CF);
L322F:
    /* 322F  mov     bx,dx */
    BX = DX;
L3231:
    /* 3231  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L3233:
    /* 3233  call    far ptr _SquareRoot_seg021_22FD_A30 */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A30), 0x065C + PORT_LOAD_SEG, 0x3238)) != 0) return c;
L3238:
    /* 3238  mov     ax,di */
    AX = DI;
L323A:
    /* 323A  xchg    ah,al */
    { uint8_t t_ = AL;
    AL = AH;
    AH = t_; }
L323C:
    /* 323C  mov     word ptr ds:[24E8h],ax */
    ww(pDS, 0x24E8, AX);
L323F:
    /* 323F  mov     ax,word ptr ds:[24EAh] */
    AX = rw(pDS, 0x24EA);
L3242:
    /* 3242  mov     word ptr ds:[24EEh],ax */
    ww(pDS, 0x24EE, AX);
L3245:
    /* 3245  imul    ax */
    imul16(AX);
L3247:
    /* 3247  add     ax,bp */
    AX = add16(AX, BP, 0);
L3249:
    /* 3249  adc     dx,si */
    DX = (uint16_t)(DX + SI + CF);
L324B:
    /* 324B  mov     bx,dx */
    BX = DX;
L324D:
    /* 324D  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L324F:
    /* 324F  call    far ptr _SquareRoot_seg021_22FD_A30 */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A30), 0x065C + PORT_LOAD_SEG, 0x3254)) != 0) return c;
L3254:
    /* 3254  mov     ax,di */
    AX = DI;
L3256:
    /* 3256  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L3258:
    /* 3258  mov     word ptr ds:[24EAh],ax */
    ww(pDS, 0x24EA, AX);
L325B:
    /* 325B  jmp     L331D */
    goto L331D;
L325E: /* L325E */
    /* 325E  mov     word ptr ds:[24F0h],7FFFh */
    ww(pDS, 0x24F0, 0x7FFF);
L3264:
    /* 3264  neg     cx */
    CX = (uint16_t)-CX;
L3266:
    /* 3266  mov     ax,word ptr ds:[14B2h] */
    AX = rw(pDS, 0x14B2);
L3269:
    /* 3269  imul    cx */
    imul16(CX);
L326B:
    /* 326B  shl     ax,1 */
    AX = shl16(AX, 1);
L326D:
    /* 326D  rcl     dx,1 */
    DX = rcl16(DX, 1);
L326F:
    /* 326F  mov     word ptr ds:[14B2h],dx */
    ww(pDS, 0x14B2, DX);
L3273:
    /* 3273  mov     ax,word ptr ds:[14B8h] */
    AX = rw(pDS, 0x14B8);
L3276:
    /* 3276  imul    cx */
    imul16(CX);
L3278:
    /* 3278  shl     ax,1 */
    AX = shl16(AX, 1);
L327A:
    /* 327A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L327C:
    /* 327C  mov     word ptr ds:[14B8h],dx */
    ww(pDS, 0x14B8, DX);
L3280:
    /* 3280  mov     ax,word ptr ds:[14BEh] */
    AX = rw(pDS, 0x14BE);
L3283:
    /* 3283  imul    cx */
    imul16(CX);
L3285:
    /* 3285  shl     ax,1 */
    AX = shl16(AX, 1);
L3287:
    /* 3287  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3289:
    /* 3289  mov     word ptr ds:[14BEh],dx */
    ww(pDS, 0x14BE, DX);
L328D:
    /* 328D  mov     ax,word ptr ds:[14B4h] */
    AX = rw(pDS, 0x14B4);
L3290:
    /* 3290  imul    cx */
    imul16(CX);
L3292:
    /* 3292  shl     ax,1 */
    AX = shl16(AX, 1);
L3294:
    /* 3294  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3296:
    /* 3296  mov     word ptr ds:[14B4h],dx */
    ww(pDS, 0x14B4, DX);
L329A:
    /* 329A  mov     ax,word ptr ds:[14BAh] */
    AX = rw(pDS, 0x14BA);
L329D:
    /* 329D  imul    cx */
    imul16(CX);
L329F:
    /* 329F  shl     ax,1 */
    AX = shl16(AX, 1);
L32A1:
    /* 32A1  rcl     dx,1 */
    DX = rcl16(DX, 1);
L32A3:
    /* 32A3  mov     word ptr ds:[14BAh],dx */
    ww(pDS, 0x14BA, DX);
L32A7:
    /* 32A7  mov     ax,word ptr ds:[14C0h] */
    AX = rw(pDS, 0x14C0);
L32AA:
    /* 32AA  imul    cx */
    imul16(CX);
L32AC:
    /* 32AC  shl     ax,1 */
    AX = shl16(AX, 1);
L32AE:
    /* 32AE  rcl     dx,1 */
    DX = rcl16(DX, 1);
L32B0:
    /* 32B0  mov     word ptr ds:[14C0h],dx */
    ww(pDS, 0x14C0, DX);
L32B4:
    /* 32B4  mov     ax,word ptr ds:[14C8h] */
    AX = rw(pDS, 0x14C8);
L32B7:
    /* 32B7  imul    cx */
    imul16(CX);
L32B9:
    /* 32B9  shl     ax,1 */
    AX = shl16(AX, 1);
L32BB:
    /* 32BB  rcl     dx,1 */
    DX = rcl16(DX, 1);
L32BD:
    /* 32BD  mov     word ptr ds:[14C8h],dx */
    ww(pDS, 0x14C8, DX);
L32C1:
    /* 32C1  mov     si,cx */
    SI = CX;
L32C3:
    /* 32C3  mov     ax,word ptr ds:[24E8h] */
    AX = rw(pDS, 0x24E8);
L32C6:
    /* 32C6  imul    cx */
    imul16(CX);
L32C8:
    /* 32C8  shl     ax,1 */
    AX = shl16(AX, 1);
L32CA:
    /* 32CA  rcl     dx,1 */
    DX = rcl16(DX, 1);
L32CC:
    /* 32CC  mov     bp,dx */
    BP = DX;
L32CE:
    /* 32CE  shl     ax,1 */
    AX = shl16(AX, 1);
L32D0:
    /* 32D0  rcl     dx,1 */
    DX = rcl16(DX, 1);
L32D2:
    /* 32D2  mov     word ptr ds:[24ECh],dx */
    ww(pDS, 0x24EC, DX);
L32D6:
    /* 32D6  mov     dx,bp */
    DX = BP;
L32D8:
    /* 32D8  mov     ax,dx */
    AX = DX;
L32DA:
    /* 32DA  imul    ax */
    imul16(AX);
L32DC:
    /* 32DC  mov     bx,dx */
    BX = DX;
L32DE:
    /* 32DE  add     bx,3FFFh */
    BX = (uint16_t)(BX + 0x3FFF);
L32E2:
    /* 32E2  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L32E4:
    /* 32E4  call    far ptr _SquareRoot_seg021_22FD_A30 */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A30), 0x065C + PORT_LOAD_SEG, 0x32E9)) != 0) return c;
L32E9:
    /* 32E9  mov     ax,di */
    AX = DI;
L32EB:
    /* 32EB  xchg    ah,al */
    { uint8_t t_ = AL;
    AL = AH;
    AH = t_; }
L32ED:
    /* 32ED  mov     word ptr ds:[24E8h],ax */
    ww(pDS, 0x24E8, AX);
L32F0:
    /* 32F0  mov     ax,word ptr ds:[24EAh] */
    AX = rw(pDS, 0x24EA);
L32F3:
    /* 32F3  imul    si */
    imul16(SI);
L32F5:
    /* 32F5  shl     ax,1 */
    AX = shl16(AX, 1);
L32F7:
    /* 32F7  rcl     dx,1 */
    DX = rcl16(DX, 1);
L32F9:
    /* 32F9  mov     bp,dx */
    BP = DX;
L32FB:
    /* 32FB  shl     ax,1 */
    AX = shl16(AX, 1);
L32FD:
    /* 32FD  rcl     dx,1 */
    DX = rcl16(DX, 1);
L32FF:
    /* 32FF  mov     word ptr ds:[24EEh],dx */
    ww(pDS, 0x24EE, DX);
L3303:
    /* 3303  mov     dx,bp */
    DX = BP;
L3305:
    /* 3305  mov     ax,dx */
    AX = DX;
L3307:
    /* 3307  imul    ax */
    imul16(AX);
L3309:
    /* 3309  mov     bx,dx */
    BX = DX;
L330B:
    /* 330B  add     bx,3FFFh */
    BX = (uint16_t)(BX + 0x3FFF);
L330F:
    /* 330F  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L3311:
    /* 3311  call    far ptr _SquareRoot_seg021_22FD_A30 */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A30), 0x065C + PORT_LOAD_SEG, 0x3316)) != 0) return c;
L3316:
    /* 3316  mov     ax,di */
    AX = DI;
L3318:
    /* 3318  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L331A:
    /* 331A  mov     word ptr ds:[24EAh],ax */
    ww(pDS, 0x24EA, AX);
L331D: /* L331D */
    /* 331D  mov     word ptr ss:[5A5h],offset _overflow_handler_reg */
    ww(pSS, 0x5A5, 0x3C91);
L3324:
    /* 3324  mov     ax,word ptr ds:[14B6h] */
    AX = rw(pDS, 0x14B6);
L3327:
    /* 3327  imul    word ptr ds:[14B6h] */
    imul16(rw(pDS, 0x14B6));
L332B:
    /* 332B  mov     cx,dx */
    CX = DX;
L332D:
    /* 332D  mov     bx,ax */
    BX = AX;
L332F:
    /* 332F  mov     ax,word ptr ds:[14C2h] */
    AX = rw(pDS, 0x14C2);
L3332:
    /* 3332  imul    word ptr ds:[14C2h] */
    imul16(rw(pDS, 0x14C2));
L3336:
    /* 3336  add     bx,ax */
    BX = add16(BX, AX, 0);
L3338:
    /* 3338  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L333A:
    /* 333A  or      ch,ch */
    CH = logic8((uint8_t)(CH | CH));
L333C:
    /* 333C  je      short L336F */
    if (ZF) goto L336F;
L333E:
    /* 333E  call    far ptr _SquareRoot_seg021_22FD_A30 */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A30), 0x065C + PORT_LOAD_SEG, 0x3343)) != 0) return c;
L3343:
    /* 3343  mov     dx,word ptr ds:[14B6h] */
    DX = rw(pDS, 0x14B6);
L3347:
    /* 3347  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L3349:
    /* 3349  sar     dx,1 */
    DX = sar16(DX, 1);
L334B:
    /* 334B  rcr     ax,1 */
    AX = rcr16(AX, 1);
L334D:
    /* 334D  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x065C, 0x334D, 2)) != 0) return c;
L334F:
    /* 334F  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L3352:
    /* 3352  jne     short L3355 */
    if (!ZF) goto L3355;
L3354:
    /* 3354  inc     ax */
    AX = (uint16_t)(AX + 1);
L3355: /* L3355 */
    /* 3355  mov     word ptr ds:[248Ch],ax */
    ww(pDS, 0x248C, AX);
L3358:
    /* 3358  mov     dx,word ptr ds:[14C2h] */
    DX = rw(pDS, 0x14C2);
L335C:
    /* 335C  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L335E:
    /* 335E  sar     dx,1 */
    DX = sar16(DX, 1);
L3360:
    /* 3360  rcr     ax,1 */
    AX = rcr16(AX, 1);
L3362:
    /* 3362  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x065C, 0x3362, 2)) != 0) return c;
L3364:
    /* 3364  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L3367:
    /* 3367  jne     short L336A */
    if (!ZF) goto L336A;
L3369:
    /* 3369  inc     ax */
    AX = (uint16_t)(AX + 1);
L336A: /* L336A */
    /* 336A  mov     word ptr ds:[248Eh],ax */
    ww(pDS, 0x248E, AX);
L336D:
    /* 336D  jmp     short L33C3 */
    goto L33C3;
L336F: /* L336F */
    /* 336F  mov     ax,word ptr ds:[14B4h] */
    AX = rw(pDS, 0x14B4);
L3372:
    /* 3372  imul    word ptr ds:[14B4h] */
    imul16(rw(pDS, 0x14B4));
L3376:
    /* 3376  mov     cx,dx */
    CX = DX;
L3378:
    /* 3378  mov     bx,ax */
    BX = AX;
L337A:
    /* 337A  mov     ax,word ptr ds:[14C0h] */
    AX = rw(pDS, 0x14C0);
L337D:
    /* 337D  imul    word ptr ds:[14C0h] */
    imul16(rw(pDS, 0x14C0));
L3381:
    /* 3381  add     bx,ax */
    BX = add16(BX, AX, 0);
L3383:
    /* 3383  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3385:
    /* 3385  call    far ptr _SquareRoot_seg021_22FD_A30 */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A30), 0x065C + PORT_LOAD_SEG, 0x338A)) != 0) return c;
L338A:
    /* 338A  mov     dx,word ptr ds:[14B4h] */
    DX = rw(pDS, 0x14B4);
L338E:
    /* 338E  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L3390:
    /* 3390  sar     dx,1 */
    DX = sar16(DX, 1);
L3392:
    /* 3392  rcr     ax,1 */
    AX = rcr16(AX, 1);
L3394:
    /* 3394  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x065C, 0x3394, 2)) != 0) return c;
L3396:
    /* 3396  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L3399:
    /* 3399  jne     short L339C */
    if (!ZF) goto L339C;
L339B:
    /* 339B  inc     ax */
    AX = (uint16_t)(AX + 1);
L339C: /* L339C */
    /* 339C  mov     bx,ax */
    BX = AX;
L339E:
    /* 339E  mov     dx,word ptr ds:[14C0h] */
    DX = rw(pDS, 0x14C0);
L33A2:
    /* 33A2  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L33A4:
    /* 33A4  sar     dx,1 */
    DX = sar16(DX, 1);
L33A6:
    /* 33A6  rcr     ax,1 */
    AX = rcr16(AX, 1);
L33A8:
    /* 33A8  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x065C, 0x33A8, 2)) != 0) return c;
L33AA:
    /* 33AA  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L33AD:
    /* 33AD  jne     short L33B0 */
    if (!ZF) goto L33B0;
L33AF:
    /* 33AF  inc     ax */
    AX = (uint16_t)(AX + 1);
L33B0: /* L33B0 */
    /* 33B0  test    word ptr ds:[14BCh],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x14BC) & 0xFFFF));
L33B6:
    /* 33B6  js      short L33BC */
    if (SF) goto L33BC;
L33B8:
    /* 33B8  neg     bx */
    BX = (uint16_t)-BX;
L33BA:
    /* 33BA  neg     ax */
    AX = (uint16_t)-AX;
L33BC: /* L33BC */
    /* 33BC  mov     word ptr ds:[248Ch],bx */
    ww(pDS, 0x248C, BX);
L33C0:
    /* 33C0  mov     word ptr ds:[248Eh],ax */
    ww(pDS, 0x248E, AX);
L33C3: /* L33C3 */
    /* 33C3  mov     ax,word ptr ds:[248Eh] */
    AX = rw(pDS, 0x248E);
L33C6:
    /* 33C6  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L33CA:
    /* 33CA  mov     bx,ax */
    BX = AX;
L33CC:
    /* 33CC  mov     cx,dx */
    CX = DX;
L33CE:
    /* 33CE  mov     ax,word ptr ds:[248Ch] */
    AX = rw(pDS, 0x248C);
L33D1:
    /* 33D1  neg     ax */
    AX = (uint16_t)-AX;
L33D3:
    /* 33D3  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L33D7:
    /* 33D7  add     ax,bx */
    AX = add16(AX, BX, 0);
L33D9:
    /* 33D9  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L33DB:
    /* 33DB  shl     ax,1 */
    AX = shl16(AX, 1);
L33DD:
    /* 33DD  rcl     dx,1 */
    DX = rcl16(DX, 1);
L33DF:
    /* 33DF  shl     ax,1 */
    AX = shl16(AX, 1);
L33E1:
    /* 33E1  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L33E4:
    /* 33E4  mov     word ptr ds:[2490h],dx */
    ww(pDS, 0x2490, DX);
L33E8:
    /* 33E8  mov     ax,word ptr ds:[248Eh] */
    AX = rw(pDS, 0x248E);
L33EB:
    /* 33EB  imul    word ptr ds:[14B4h] */
    imul16(rw(pDS, 0x14B4));
L33EF:
    /* 33EF  mov     bx,ax */
    BX = AX;
L33F1:
    /* 33F1  mov     cx,dx */
    CX = DX;
L33F3:
    /* 33F3  mov     ax,word ptr ds:[248Ch] */
    AX = rw(pDS, 0x248C);
L33F6:
    /* 33F6  neg     ax */
    AX = (uint16_t)-AX;
L33F8:
    /* 33F8  imul    word ptr ds:[14C0h] */
    imul16(rw(pDS, 0x14C0));
L33FC:
    /* 33FC  add     ax,bx */
    AX = add16(AX, BX, 0);
L33FE:
    /* 33FE  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L3400:
    /* 3400  shl     ax,1 */
    AX = shl16(AX, 1);
L3402:
    /* 3402  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3404:
    /* 3404  shl     ax,1 */
    AX = shl16(AX, 1);
L3406:
    /* 3406  adc     dx,0 */
    DX = add16(DX, 0x0, CF);
L3409:
    /* 3409  mov     word ptr ds:[2492h],dx */
    ww(pDS, 0x2492, DX);
L340D:
    /* 340D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_340E  (+340E)
       check_flat: when the view is level (the matrix's middle row and column are 0 except the middle
       entry, at least 7FFBh), install the flat variants of the relative-move opcode handlers
       (flat_x_rel ... flat_rod, INTERP.ASM) in the opcode table and copy the short mxmul body
       (L35DA) over mxmul; otherwise install the general handlers and the full body (L3587). 25D4
       records which set is in, so the copy is done only on a change. A level view is probably the
       common case: the view pitches only while the player looks up or down. */
L340E: /* _check_flat */
    /* 340E  test    word ptr ds:[14B8h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x14B8) & 0xFFFF));
L3414:
    /* 3414  jne     short L3436 */
    if (!ZF) goto L3436;
L3416:
    /* 3416  test    word ptr ds:[14B4h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x14B4) & 0xFFFF));
L341C:
    /* 341C  jne     short L3436 */
    if (!ZF) goto L3436;
L341E:
    /* 341E  test    word ptr ds:[14C0h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x14C0) & 0xFFFF));
L3424:
    /* 3424  jne     short L3436 */
    if (!ZF) goto L3436;
L3426:
    /* 3426  test    word ptr ds:[14BCh],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x14BC) & 0xFFFF));
L342C:
    /* 342C  jne     short L3436 */
    if (!ZF) goto L3436;
L342E:
    /* 342E  cmp     word ptr ds:[14BAh],7FFBh */
    sub16(rw(pDS, 0x14BA), 0x7FFB, 0);
L3434:
    /* 3434  jge     short is_flat */
    if (SF == OF) goto L347D;
L3436: /* L3436 */
    /* 3436  test    byte ptr ds:[25D4h],0FFh */
    logic8((uint8_t)(rb(pDS, 0x25D4) & 0xFF));
L343B:
    /* 343B  je      short L347C */
    if (ZF) goto L347C;
L343D:
    /* 343D  mov     byte ptr ds:[25D4h],0 */
    wb(pDS, 0x25D4, 0x0);
L3442:
    /* 3442  mov     word ptr ds:[257Ah],offset _do_x_rel */
    ww(pDS, 0x257A, 0x1D01);
L3448:
    /* 3448  mov     word ptr ds:[257Ch],offset _do_y_rel */
    ww(pDS, 0x257C, 0x1B4B);
L344E:
    /* 344E  mov     word ptr ds:[257Eh],offset _do_z_rel */
    ww(pDS, 0x257E, 0x1DA4);
L3454:
    /* 3454  mov     word ptr ds:[2584h],offset _do_xy_rel */
    ww(pDS, 0x2584, 0x1E4D);
L345A:
    /* 345A  mov     word ptr ds:[2588h],offset _do_yz_rel */
    ww(pDS, 0x2588, 0x1EAE);
L3460:
    /* 3460  mov     word ptr ds:[2586h],offset _do_xz_rel */
    ww(pDS, 0x2586, 0x1DEC);
L3466:
    /* 3466  mov     word ptr ds:[2590h],offset _do_rod */
    ww(pDS, 0x2590, 0x154A);
L346C:
    /* 346C  mov     si,offset L3587 */
    SI = 0x3587;
L346F:
    /* 346F  mov     di,offset _mxmul */
    DI = 0x3534;
L3472:
    /* 3472  push    es */
    push16(asm_es);
L3473:
    /* 3473  push    cs */
    push16((uint16_t)(0x065C + PORT_LOAD_SEG));
L3474:
    /* 3474  pop     es */
    SET_ES(pop16());
L3475:
    /* 3475  mov     cx,54h */
    CX = 0x54;
L3478:
    /* 3478  rep movs byte ptr es:[di],byte ptr cs:[si] */
    while (CX) { wb(pES, DI, rb(CODE004, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L347B:
    /* 347B  pop     es */
    SET_ES(pop16());
L347C: /* L347C */
    /* 347C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L347D: /* is_flat */
    /* 347D  test    byte ptr ds:[25D4h],0FFh */
    logic8((uint8_t)(rb(pDS, 0x25D4) & 0xFF));
L3482:
    /* 3482  jne     L347C */
    if (!ZF) goto L347C;
L3484:
    /* 3484  mov     byte ptr ds:[25D4h],0FFh */
    wb(pDS, 0x25D4, 0xFF);
L3489:
    /* 3489  mov     word ptr ds:[257Ah],offset _flat_x_rel */
    ww(pDS, 0x257A, 0x1CA5);
L348F:
    /* 348F  mov     word ptr ds:[257Ch],offset _flat_y_rel */
    ww(pDS, 0x257C, 0x1AF8);
L3495:
    /* 3495  mov     word ptr ds:[257Eh],offset _flat_z_rel */
    ww(pDS, 0x257E, 0x1D49);
L349B:
    /* 349B  mov     word ptr ds:[2584h],offset _flat_xy_rel */
    ww(pDS, 0x2584, 0x1F62);
L34A1:
    /* 34A1  mov     word ptr ds:[2588h],offset _flat_yz_rel */
    ww(pDS, 0x2588, 0x1FAA);
L34A7:
    /* 34A7  mov     word ptr ds:[2586h],offset _flat_xz_rel */
    ww(pDS, 0x2586, 0x1F0F);
L34AD:
    /* 34AD  mov     word ptr ds:[2590h],offset _flat_rod */
    ww(pDS, 0x2590, 0x164F);
L34B3:
    /* 34B3  mov     si,offset L35DA */
    SI = 0x35DA;
L34B6:
    /* 34B6  mov     di,offset _mxmul */
    DI = 0x3534;
L34B9:
    /* 34B9  push    es */
    push16(asm_es);
L34BA:
    /* 34BA  push    cs */
    push16((uint16_t)(0x065C + PORT_LOAD_SEG));
L34BB:
    /* 34BB  pop     es */
    SET_ES(pop16());
L34BC:
    /* 34BC  mov     cx,26h */
    CX = 0x26;
L34BF:
    /* 34BF  rep movs byte ptr es:[di],byte ptr cs:[si] */
    while (CX) { wb(pES, DI, rb(CODE004, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L34C2:
    /* 34C2  pop     es */
    SET_ES(pop16());
L34C3:
    /* 34C3  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_34C4  (+34C4)
       set_accept: empty here and in FM Towns (accept_flag, which it would set, is never written; see
       CLIP.ASM). */
L34C4: /* _set_accept */
    /* 34C4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_34C5  (+34C5)
       self_modify: patch the eye position in the current object's frame (25E6, 25E8, 25EA) and the
       point shift (25E0) into the immediates of mini_xlate_rotate_pnt and load_xlate_rotate_pnt, so
       that turning a model point into view space needs no memory reads for them. */
L34C5: /* _self_modify */
    /* 34C5  mov     ax,word ptr ds:[25E6h] */
    AX = rw(pDS, 0x25E6);
L34C8:
    /* 34C8  mov     word ptr cs:[modify_mex+1],ax */
    ww(CODE004, 0x351E, AX);
L34CC:
    /* 34CC  mov     word ptr cs:[L34F9+1],ax */
    ww(CODE004, 0x34FA, AX);
L34D0:
    /* 34D0  mov     ax,word ptr ds:[25E8h] */
    AX = rw(pDS, 0x25E8);
L34D3:
    /* 34D3  mov     word ptr cs:[modify_mey+1],ax */
    ww(CODE004, 0x352E, AX);
L34D7:
    /* 34D7  mov     word ptr cs:[L3511+1],ax */
    ww(CODE004, 0x3512, AX);
L34DB:
    /* 34DB  mov     ax,word ptr ds:[25EAh] */
    AX = rw(pDS, 0x25EA);
L34DE:
    /* 34DE  mov     word ptr cs:[modify_mez+1],ax */
    ww(CODE004, 0x3526, AX);
L34E2:
    /* 34E2  mov     word ptr cs:[L3505+1],ax */
    ww(CODE004, 0x3506, AX);
L34E6:
    /* 34E6  mov     al,byte ptr ds:[25E0h] */
    AL = rb(pDS, 0x25E0);
L34E9:
    /* 34E9  mov     byte ptr cs:[modify_mes+1],al */
    wb(CODE004, 0x351B, AL);
L34ED:
    /* 34ED  mov     byte ptr cs:[_mini_xlate_rotate_pnt+1],al */
    wb(CODE004, 0x34F3, AL);
L34F1:
    /* 34F1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_34F2  (+34F2)
       mini_xlate_rotate_pnt: in: SI = three signed bytes, a model point in the order x, z, y, each
       times 32. Subtracts the patched eye, shifts by the patched shift and falls into mxmul. Out:
       BX, CX, BP = the view-space point (see mxmul); SI advanced by 3. load_xlate_rotate_pnt below
       is the same for three words. */
L34F2: /* _mini_xlate_rotate_pnt */
    /* 34F2  mov     cl,5 */
    CL = rb(CODE004, 0x34F3);
L34F4:
    /* 34F4  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L34F5:
    /* 34F5  cbw */
    AX = (uint16_t)(int8_t)AL;
L34F6:
    /* 34F6  shl     ax,5 */
    AX = (uint16_t)(AX << 5);
L34F9: /* L34F9 */
    /* 34F9  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x34FA), 0);
L34FC:
    /* 34FC  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L34FE:
    /* 34FE  mov     bx,ax */
    BX = AX;
L3500:
    /* 3500  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3501:
    /* 3501  cbw */
    AX = (uint16_t)(int8_t)AL;
L3502:
    /* 3502  shl     ax,5 */
    AX = (uint16_t)(AX << 5);
L3505: /* L3505 */
    /* 3505  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x3506), 0);
L3508:
    /* 3508  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L350A:
    /* 350A  mov     bp,ax */
    BP = AX;
L350C:
    /* 350C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L350D:
    /* 350D  cbw */
    AX = (uint16_t)(int8_t)AL;
L350E:
    /* 350E  shl     ax,5 */
    AX = (uint16_t)(AX << 5);
L3511: /* L3511 */
    /* 3511  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x3512), 0);
L3514:
    /* 3514  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3516:
    /* 3516  mov     cx,ax */
    CX = AX;
L3518:
    /* 3518  jmp     short _mxmul */
    goto L3534;

    /* seg004_0849_351A  (+351A) */
L351A: /* _load_xlate_rotate_pnt, modify_mes */
    /* 351A  mov     cl,5 */
    CL = rb(CODE004, 0x351B);
L351C:
    /* 351C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L351D: /* modify_mex */
    /* 351D  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x351E), 0);
L3520:
    /* 3520  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3522:
    /* 3522  mov     bx,ax */
    BX = AX;
L3524:
    /* 3524  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3525: /* modify_mez */
    /* 3525  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x3526), 0);
L3528:
    /* 3528  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L352A:
    /* 352A  mov     bp,ax */
    BP = AX;
L352C:
    /* 352C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L352D: /* modify_mey */
    /* 352D  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x352E), 0);
L3530:
    /* 3530  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3532:
    /* 3532  mov     cx,ax */
    CX = AX;

    /* seg004_0849_3534  (+3534)
       mxmul: rotate a point by the view matrix. In: BX, CX, BP = the point's x, y (vertical) and z.
       Out: BX, CX, BP = view x, y and depth z, each the high word of its 1.15 products (so at half
       scale); DI, DX changed. The general body (copied from L3587) does all nine products; the flat
       body (L35DA, for a level view) skips the products with the zero entries and takes y as it is,
       halved. */
L3534: /* _mxmul */
    /* 3534  mov     ax,bx */
    /* by hand: mxmul's body is whichever check_flat last copied over it (L3587, the general one, or L35DA, the flat one): the code block holds the copy, so the body runs as it says */
    if (memcmp(CODE004 + 0x3534, CODE004 + 0x35DA, 0x26) == 0) goto L35DA;
    AX = BX;
L3536:
    /* 3536  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L353A:
    /* 353A  mov     di,dx */
    DI = DX;
L353C:
    /* 353C  mov     ax,cx */
    AX = CX;
L353E:
    /* 353E  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L3542:
    /* 3542  add     di,dx */
    DI = (uint16_t)(DI + DX);
L3544:
    /* 3544  mov     ax,bp */
    AX = BP;
L3546:
    /* 3546  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L354A:
    /* 354A  add     di,dx */
    DI = (uint16_t)(DI + DX);
L354C:
    /* 354C  mov     word ptr ds:[24C8h],di */
    ww(pDS, 0x24C8, DI);
L3550:
    /* 3550  mov     ax,bx */
    AX = BX;
L3552:
    /* 3552  imul    word ptr ds:[14B4h] */
    imul16(rw(pDS, 0x14B4));
L3556:
    /* 3556  mov     di,dx */
    DI = DX;
L3558:
    /* 3558  mov     ax,cx */
    AX = CX;
L355A:
    /* 355A  imul    word ptr ds:[14BAh] */
    imul16(rw(pDS, 0x14BA));
L355E:
    /* 355E  add     di,dx */
    DI = (uint16_t)(DI + DX);
L3560:
    /* 3560  mov     ax,bp */
    AX = BP;
L3562:
    /* 3562  imul    word ptr ds:[14C0h] */
    imul16(rw(pDS, 0x14C0));
L3566:
    /* 3566  add     di,dx */
    DI = (uint16_t)(DI + DX);
L3568:
    /* 3568  mov     ax,bp */
    AX = BP;
L356A:
    /* 356A  imul    word ptr ds:[14C2h] */
    imul16(rw(pDS, 0x14C2));
L356E:
    /* 356E  mov     bp,dx */
    BP = DX;
L3570:
    /* 3570  mov     ax,bx */
    AX = BX;
L3572:
    /* 3572  imul    word ptr ds:[14B6h] */
    imul16(rw(pDS, 0x14B6));
L3576:
    /* 3576  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L3578:
    /* 3578  mov     ax,cx */
    AX = CX;
L357A:
    /* 357A  imul    word ptr ds:[14BCh] */
    imul16(rw(pDS, 0x14BC));
L357E:
    /* 357E  add     bp,dx */
    BP = add16(BP, DX, 0);
L3580:
    /* 3580  mov     cx,di */
    CX = DI;
L3582:
    /* 3582  mov     bx,word ptr ds:[24C8h] */
    BX = rw(pDS, 0x24C8);
L3586:
    /* 3586  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* L3587 and L35DA: the two bodies check_flat and is_flat copy over the one above */
L3587: /* L3587 */
    /* 3587  mov     ax,bx */
    AX = BX;
L3589:
    /* 3589  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L358D:
    /* 358D  mov     di,dx */
    DI = DX;
L358F:
    /* 358F  mov     ax,cx */
    AX = CX;
L3591:
    /* 3591  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L3595:
    /* 3595  add     di,dx */
    DI = (uint16_t)(DI + DX);
L3597:
    /* 3597  mov     ax,bp */
    AX = BP;
L3599:
    /* 3599  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L359D:
    /* 359D  add     di,dx */
    DI = (uint16_t)(DI + DX);
L359F:
    /* 359F  mov     word ptr ds:[24C8h],di */
    ww(pDS, 0x24C8, DI);
L35A3:
    /* 35A3  mov     ax,bx */
    AX = BX;
L35A5:
    /* 35A5  imul    word ptr ds:[14B4h] */
    imul16(rw(pDS, 0x14B4));
L35A9:
    /* 35A9  mov     di,dx */
    DI = DX;
L35AB:
    /* 35AB  mov     ax,cx */
    AX = CX;
L35AD:
    /* 35AD  imul    word ptr ds:[14BAh] */
    imul16(rw(pDS, 0x14BA));
L35B1:
    /* 35B1  add     di,dx */
    DI = (uint16_t)(DI + DX);
L35B3:
    /* 35B3  mov     ax,bp */
    AX = BP;
L35B5:
    /* 35B5  imul    word ptr ds:[14C0h] */
    imul16(rw(pDS, 0x14C0));
L35B9:
    /* 35B9  add     di,dx */
    DI = (uint16_t)(DI + DX);
L35BB:
    /* 35BB  mov     ax,bp */
    AX = BP;
L35BD:
    /* 35BD  imul    word ptr ds:[14C2h] */
    imul16(rw(pDS, 0x14C2));
L35C1:
    /* 35C1  mov     bp,dx */
    BP = DX;
L35C3:
    /* 35C3  mov     ax,bx */
    AX = BX;
L35C5:
    /* 35C5  imul    word ptr ds:[14B6h] */
    imul16(rw(pDS, 0x14B6));
L35C9:
    /* 35C9  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L35CB:
    /* 35CB  mov     ax,cx */
    AX = CX;
L35CD:
    /* 35CD  imul    word ptr ds:[14BCh] */
    imul16(rw(pDS, 0x14BC));
L35D1:
    /* 35D1  add     bp,dx */
    BP = add16(BP, DX, 0);
L35D3:
    /* 35D3  mov     cx,di */
    CX = DI;
L35D5:
    /* 35D5  mov     bx,word ptr ds:[24C8h] */
    BX = rw(pDS, 0x24C8);
L35D9:
    /* 35D9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L35DA: /* L35DA */
    /* 35DA  mov     ax,bx */
    AX = BX;
L35DC:
    /* 35DC  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L35E0:
    /* 35E0  mov     di,dx */
    DI = DX;
L35E2:
    /* 35E2  mov     ax,bp */
    AX = BP;
L35E4:
    /* 35E4  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L35E8:
    /* 35E8  add     di,dx */
    DI = (uint16_t)(DI + DX);
L35EA:
    /* 35EA  mov     ax,bx */
    AX = BX;
L35EC:
    /* 35EC  mov     bx,di */
    BX = DI;
L35EE:
    /* 35EE  imul    word ptr ds:[14B6h] */
    imul16(rw(pDS, 0x14B6));
L35F2:
    /* 35F2  mov     ax,bp */
    AX = BP;
L35F4:
    /* 35F4  mov     bp,dx */
    BP = DX;
L35F6:
    /* 35F6  imul    word ptr ds:[14C2h] */
    imul16(rw(pDS, 0x14C2));
L35FA:
    /* 35FA  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L35FC:
    /* 35FC  sar     cx,1 */
    CX = sar16(CX, 1);
L35FE:
    /* 35FE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_35FF  (+35FF)
       seg004_0849_35FF: choose the axis for SPHERE.ASM's first cull (modify_axis2): y when the view
       is pitched by more than 45 degrees (|14BC| > 5A82h, sin 45 in 1.15), else whichever of x and z
       the view looks along more. Patches the instruction to add or subtract (by the sign) the eye's
       coordinate on that axis (25E6 + offset). */
L35FF: /* _seg004_0849_35FF */
    /* 35FF  mov     ax,word ptr ds:[14BCh] */
    AX = rw(pDS, 0x14BC);
L3602:
    /* 3602  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3603:
    /* 3603  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L3605:
    /* 3605  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L3607:
    /* 3607  cmp     ax,5A82h */
    sub16(AX, 0x5A82, 0);
L360A:
    /* 360A  jle     short L3611 */
    if (ZF || SF != OF) goto L3611;
L360C:
    /* 360C  mov     ax,2 */
    AX = 0x2;
L360F:
    /* 360F  jmp     short L3633 */
    goto L3633;
L3611: /* L3611 */
    /* 3611  mov     ax,word ptr ds:[14B6h] */
    AX = rw(pDS, 0x14B6);
L3614:
    /* 3614  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3615:
    /* 3615  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L3617:
    /* 3617  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L3619:
    /* 3619  mov     bx,ax */
    BX = AX;
L361B:
    /* 361B  mov     cx,dx */
    CX = DX;
L361D:
    /* 361D  mov     ax,word ptr ds:[14C2h] */
    AX = rw(pDS, 0x14C2);
L3620:
    /* 3620  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3621:
    /* 3621  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L3623:
    /* 3623  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L3625:
    /* 3625  cmp     bx,ax */
    sub16(BX, AX, 0);
L3627:
    /* 3627  jg      short L362E */
    if (!ZF && SF == OF) goto L362E;
L3629:
    /* 3629  mov     ax,4 */
    AX = 0x4;
L362C:
    /* 362C  jmp     short L3633 */
    goto L3633;
L362E: /* L362E */
    /* 362E  mov     ax,0 */
    AX = 0x0;
L3631:
    /* 3631  mov     dx,cx */
    DX = CX;
L3633: /* L3633 */
    /* 3633  mov     bl,2Bh */
    BL = 0x2B;
L3635:
    /* 3635  test    dx,0FFFFh */
    logic16((uint16_t)(DX & 0xFFFF));
L3639:
    /* 3639  jns     short L363D */
    if (!SF) goto L363D;
L363B:
    /* 363B  mov     bl,3 */
    BL = 0x3;
L363D: /* L363D */
    /* 363D  mov     byte ptr cs:modify_axis2,bl */
    wb(CODE004, 0x58C, BL);
L3642:
    /* 3642  add     ax,8 */
    AX = (uint16_t)(AX + 0x8);
L3645:
    /* 3645  sub     ax,8 */
    AX = (uint16_t)(AX - 0x8);
L3648:
    /* 3648  add     ax,25E6h */
    AX = add16(AX, 0x25E6, 0);
L364B:
    /* 364B  mov     word ptr cs:[modify_axis2+2],ax */
    ww(CODE004, 0x58E, AX);
L364F:
    /* 364F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_3650  (+3650)
       instance_vvars_head: rotate the eye position in the object's frame (25E6 x, 25EA z) by the
       instance heading in 24CA..24CE, so later points can be taken relative to the rotated object. */
L3650: /* _instance_vvars_head */
    /* 3650  mov     ax,word ptr ds:[25E6h] */
    AX = rw(pDS, 0x25E6);
L3653:
    /* 3653  imul    word ptr ds:[24CAh] */
    imul16(rw(pDS, 0x24CA));
L3657:
    /* 3657  mov     cx,dx */
    CX = DX;
L3659:
    /* 3659  mov     bx,ax */
    BX = AX;
L365B:
    /* 365B  mov     ax,word ptr ds:[25EAh] */
    AX = rw(pDS, 0x25EA);
L365E:
    /* 365E  imul    word ptr ds:[24CEh] */
    imul16(rw(pDS, 0x24CE));
L3662:
    /* 3662  add     bx,ax */
    BX = add16(BX, AX, 0);
L3664:
    /* 3664  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3666:
    /* 3666  shl     bx,1 */
    BX = shl16(BX, 1);
L3668:
    /* 3668  rcl     cx,1 */
    CX = rcl16(CX, 1);
L366A:
    /* 366A  mov     ax,word ptr ds:[25E6h] */
    AX = rw(pDS, 0x25E6);
L366D:
    /* 366D  mov     word ptr ds:[25E6h],cx */
    ww(pDS, 0x25E6, CX);
L3671:
    /* 3671  imul    word ptr ds:[24CCh] */
    imul16(rw(pDS, 0x24CC));
L3675:
    /* 3675  mov     bx,ax */
    BX = AX;
L3677:
    /* 3677  mov     cx,dx */
    CX = DX;
L3679:
    /* 3679  mov     ax,word ptr ds:[25EAh] */
    AX = rw(pDS, 0x25EA);
L367C:
    /* 367C  imul    word ptr ds:[24CAh] */
    imul16(rw(pDS, 0x24CA));
L3680:
    /* 3680  add     ax,bx */
    AX = add16(AX, BX, 0);
L3682:
    /* 3682  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L3684:
    /* 3684  shl     ax,1 */
    AX = shl16(AX, 1);
L3686:
    /* 3686  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3688:
    /* 3688  mov     word ptr ds:[25EAh],dx */
    ww(pDS, 0x25EA, DX);
L368C:
    /* 368C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_368D  (+368D)
       instance_head: enter a sub-object turned by a heading. In: AX = the angle. Gets sin and cos
       (sincos, IMATH.ASM, called with DS = SS), rotates the eye (instance_vvars_head) and multiplies
       the view matrix by the rotation about the vertical axis, saturating each entry to 1.15. The
       interpreter saves and restores the matrix around the sub-object (INTERP.ASM, opcode 50h and
       the do_ihcall family). */
L368D: /* _instance_head */
    /* 368D  mov     ax,ss */
    AX = asm_ss;
L368F:
    /* 368F  mov     ds,ax */
    SET_DS(AX);
L3691:
    /* 3691  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A34), 0x065C + PORT_LOAD_SEG, 0x3696)) != 0) return c;
L3696:
    /* 3696  mov     dx,es */
    DX = asm_es;
L3698:
    /* 3698  mov     ds,dx */
    SET_DS(DX);
L369A:
    /* 369A  mov     word ptr ds:[24CCh],ax */
    ww(pDS, 0x24CC, AX);
L369D:
    /* 369D  mov     word ptr ds:[24CAh],bx */
    ww(pDS, 0x24CA, BX);
L36A1:
    /* 36A1  neg     ax */
    AX = (uint16_t)-AX;
L36A3:
    /* 36A3  mov     word ptr ds:[24CEh],ax */
    ww(pDS, 0x24CE, AX);
L36A6:
    /* 36A6  call    _instance_vvars_head */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3650), 0x36A9)) != 0) return c;
L36A9:
    /* 36A9  mov     ax,word ptr ds:[24CAh] */
    AX = rw(pDS, 0x24CA);
L36AC:
    /* 36AC  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L36B0:
    /* 36B0  mov     bx,ax */
    BX = AX;
L36B2:
    /* 36B2  mov     cx,dx */
    CX = DX;
L36B4:
    /* 36B4  mov     ax,word ptr ds:[24CEh] */
    AX = rw(pDS, 0x24CE);
L36B7:
    /* 36B7  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L36BB:
    /* 36BB  add     bx,ax */
    BX = add16(BX, AX, 0);
L36BD:
    /* 36BD  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L36BF:
    /* 36BF  add     bx,bx */
    BX = add16(BX, BX, 0);
L36C1:
    /* 36C1  adc     cx,cx */
    CX = add16(CX, CX, CF);
L36C3:
    /* 36C3  jno     short L36CF */
    if (!OF) goto L36CF;
L36C5:
    /* 36C5  jg      short L36CC */
    if (!ZF && SF == OF) goto L36CC;
L36C7:
    /* 36C7  mov     cx,8000h */
    CX = 0x8000;
L36CA:
    /* 36CA  jmp     short L36CF */
    goto L36CF;
L36CC: /* L36CC */
    /* 36CC  mov     cx,7FFFh */
    CX = 0x7FFF;
L36CF: /* L36CF */
    /* 36CF  mov     word ptr ds:[2494h],cx */
    ww(pDS, 0x2494, CX);
L36D3:
    /* 36D3  mov     ax,word ptr ds:[24CAh] */
    AX = rw(pDS, 0x24CA);
L36D6:
    /* 36D6  imul    word ptr ds:[14B4h] */
    imul16(rw(pDS, 0x14B4));
L36DA:
    /* 36DA  mov     bx,ax */
    BX = AX;
L36DC:
    /* 36DC  mov     cx,dx */
    CX = DX;
L36DE:
    /* 36DE  mov     ax,word ptr ds:[24CEh] */
    AX = rw(pDS, 0x24CE);
L36E1:
    /* 36E1  imul    word ptr ds:[14C0h] */
    imul16(rw(pDS, 0x14C0));
L36E5:
    /* 36E5  add     bx,ax */
    BX = add16(BX, AX, 0);
L36E7:
    /* 36E7  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L36E9:
    /* 36E9  add     bx,bx */
    BX = add16(BX, BX, 0);
L36EB:
    /* 36EB  adc     cx,cx */
    CX = add16(CX, CX, CF);
L36ED:
    /* 36ED  jno     short L36F9 */
    if (!OF) goto L36F9;
L36EF:
    /* 36EF  jg      short L36F6 */
    if (!ZF && SF == OF) goto L36F6;
L36F1:
    /* 36F1  mov     cx,8000h */
    CX = 0x8000;
L36F4:
    /* 36F4  jmp     short L36F9 */
    goto L36F9;
L36F6: /* L36F6 */
    /* 36F6  mov     cx,7FFFh */
    CX = 0x7FFF;
L36F9: /* L36F9 */
    /* 36F9  mov     word ptr ds:[2496h],cx */
    ww(pDS, 0x2496, CX);
L36FD:
    /* 36FD  mov     ax,word ptr ds:[24CAh] */
    AX = rw(pDS, 0x24CA);
L3700:
    /* 3700  imul    word ptr ds:[14B6h] */
    imul16(rw(pDS, 0x14B6));
L3704:
    /* 3704  mov     bx,ax */
    BX = AX;
L3706:
    /* 3706  mov     cx,dx */
    CX = DX;
L3708:
    /* 3708  mov     ax,word ptr ds:[24CEh] */
    AX = rw(pDS, 0x24CE);
L370B:
    /* 370B  imul    word ptr ds:[14C2h] */
    imul16(rw(pDS, 0x14C2));
L370F:
    /* 370F  add     bx,ax */
    BX = add16(BX, AX, 0);
L3711:
    /* 3711  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3713:
    /* 3713  add     bx,bx */
    BX = add16(BX, BX, 0);
L3715:
    /* 3715  adc     cx,cx */
    CX = add16(CX, CX, CF);
L3717:
    /* 3717  jno     short L3723 */
    if (!OF) goto L3723;
L3719:
    /* 3719  jg      short L3720 */
    if (!ZF && SF == OF) goto L3720;
L371B:
    /* 371B  mov     cx,8000h */
    CX = 0x8000;
L371E:
    /* 371E  jmp     short L3723 */
    goto L3723;
L3720: /* L3720 */
    /* 3720  mov     cx,7FFFh */
    CX = 0x7FFF;
L3723: /* L3723 */
    /* 3723  mov     word ptr ds:[2498h],cx */
    ww(pDS, 0x2498, CX);
L3727:
    /* 3727  mov     ax,word ptr ds:[24CCh] */
    AX = rw(pDS, 0x24CC);
L372A:
    /* 372A  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L372E:
    /* 372E  mov     bx,ax */
    BX = AX;
L3730:
    /* 3730  mov     cx,dx */
    CX = DX;
L3732:
    /* 3732  mov     ax,word ptr ds:[24CAh] */
    AX = rw(pDS, 0x24CA);
L3735:
    /* 3735  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L3739:
    /* 3739  add     bx,ax */
    BX = add16(BX, AX, 0);
L373B:
    /* 373B  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L373D:
    /* 373D  add     bx,bx */
    BX = add16(BX, BX, 0);
L373F:
    /* 373F  adc     cx,cx */
    CX = add16(CX, CX, CF);
L3741:
    /* 3741  jno     short L374D */
    if (!OF) goto L374D;
L3743:
    /* 3743  jg      short L374A */
    if (!ZF && SF == OF) goto L374A;
L3745:
    /* 3745  mov     cx,8000h */
    CX = 0x8000;
L3748:
    /* 3748  jmp     short L374D */
    goto L374D;
L374A: /* L374A */
    /* 374A  mov     cx,7FFFh */
    CX = 0x7FFF;
L374D: /* L374D */
    /* 374D  mov     word ptr ds:[14BEh],cx */
    ww(pDS, 0x14BE, CX);
L3751:
    /* 3751  mov     ax,word ptr ds:[24CCh] */
    AX = rw(pDS, 0x24CC);
L3754:
    /* 3754  imul    word ptr ds:[14B4h] */
    imul16(rw(pDS, 0x14B4));
L3758:
    /* 3758  mov     bx,ax */
    BX = AX;
L375A:
    /* 375A  mov     cx,dx */
    CX = DX;
L375C:
    /* 375C  mov     ax,word ptr ds:[24CAh] */
    AX = rw(pDS, 0x24CA);
L375F:
    /* 375F  imul    word ptr ds:[14C0h] */
    imul16(rw(pDS, 0x14C0));
L3763:
    /* 3763  add     bx,ax */
    BX = add16(BX, AX, 0);
L3765:
    /* 3765  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3767:
    /* 3767  add     bx,bx */
    BX = add16(BX, BX, 0);
L3769:
    /* 3769  adc     cx,cx */
    CX = add16(CX, CX, CF);
L376B:
    /* 376B  jno     short L3777 */
    if (!OF) goto L3777;
L376D:
    /* 376D  jg      short L3774 */
    if (!ZF && SF == OF) goto L3774;
L376F:
    /* 376F  mov     cx,8000h */
    CX = 0x8000;
L3772:
    /* 3772  jmp     short L3777 */
    goto L3777;
L3774: /* L3774 */
    /* 3774  mov     cx,7FFFh */
    CX = 0x7FFF;
L3777: /* L3777 */
    /* 3777  mov     word ptr ds:[14C0h],cx */
    ww(pDS, 0x14C0, CX);
L377B:
    /* 377B  mov     ax,word ptr ds:[24CCh] */
    AX = rw(pDS, 0x24CC);
L377E:
    /* 377E  imul    word ptr ds:[14B6h] */
    imul16(rw(pDS, 0x14B6));
L3782:
    /* 3782  mov     bx,ax */
    BX = AX;
L3784:
    /* 3784  mov     cx,dx */
    CX = DX;
L3786:
    /* 3786  mov     ax,word ptr ds:[24CAh] */
    AX = rw(pDS, 0x24CA);
L3789:
    /* 3789  imul    word ptr ds:[14C2h] */
    imul16(rw(pDS, 0x14C2));
L378D:
    /* 378D  add     bx,ax */
    BX = add16(BX, AX, 0);
L378F:
    /* 378F  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3791:
    /* 3791  add     bx,bx */
    BX = add16(BX, BX, 0);
L3793:
    /* 3793  adc     cx,cx */
    CX = add16(CX, CX, CF);
L3795:
    /* 3795  jno     short L37A1 */
    if (!OF) goto L37A1;
L3797:
    /* 3797  jg      short L379E */
    if (!ZF && SF == OF) goto L379E;
L3799:
    /* 3799  mov     cx,8000h */
    CX = 0x8000;
L379C:
    /* 379C  jmp     short L37A1 */
    goto L37A1;
L379E: /* L379E */
    /* 379E  mov     cx,7FFFh */
    CX = 0x7FFF;
L37A1: /* L37A1 */
    /* 37A1  mov     word ptr ds:[14C2h],cx */
    ww(pDS, 0x14C2, CX);
L37A5:
    /* 37A5  mov     ax,word ptr ds:[2494h] */
    AX = rw(pDS, 0x2494);
L37A8:
    /* 37A8  mov     word ptr ds:[14B2h],ax */
    ww(pDS, 0x14B2, AX);
L37AB:
    /* 37AB  mov     ax,word ptr ds:[2496h] */
    AX = rw(pDS, 0x2496);
L37AE:
    /* 37AE  mov     word ptr ds:[14B4h],ax */
    ww(pDS, 0x14B4, AX);
L37B1:
    /* 37B1  mov     ax,word ptr ds:[2498h] */
    AX = rw(pDS, 0x2498);
L37B4:
    /* 37B4  mov     word ptr ds:[14B6h],ax */
    ww(pDS, 0x14B6, AX);
L37B7:
    /* 37B7  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_37B8  (+37B8)
       instance_vvars_pitch and instance_pitch, instance_vvars_bank and instance_bank: as
       instance_vvars_head and instance_head for rotations about the other two axes (pitch and bank). */
L37B8: /* _instance_vvars_pitch */
    /* 37B8  mov     ax,word ptr ds:[25E8h] */
    AX = rw(pDS, 0x25E8);
L37BB:
    /* 37BB  imul    word ptr ds:[24D6h] */
    imul16(rw(pDS, 0x24D6));
L37BF:
    /* 37BF  mov     cx,dx */
    CX = DX;
L37C1:
    /* 37C1  mov     bx,ax */
    BX = AX;
L37C3:
    /* 37C3  mov     ax,word ptr ds:[25EAh] */
    AX = rw(pDS, 0x25EA);
L37C6:
    /* 37C6  imul    word ptr ds:[24DAh] */
    imul16(rw(pDS, 0x24DA));
L37CA:
    /* 37CA  add     ax,bx */
    AX = add16(AX, BX, 0);
L37CC:
    /* 37CC  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L37CE:
    /* 37CE  shl     ax,1 */
    AX = shl16(AX, 1);
L37D0:
    /* 37D0  rcl     dx,1 */
    DX = rcl16(DX, 1);
L37D2:
    /* 37D2  mov     ax,word ptr ds:[25E8h] */
    AX = rw(pDS, 0x25E8);
L37D5:
    /* 37D5  mov     word ptr ds:[25E8h],dx */
    ww(pDS, 0x25E8, DX);
L37D9:
    /* 37D9  imul    word ptr ds:[24D8h] */
    imul16(rw(pDS, 0x24D8));
L37DD:
    /* 37DD  mov     bx,ax */
    BX = AX;
L37DF:
    /* 37DF  mov     cx,dx */
    CX = DX;
L37E1:
    /* 37E1  mov     ax,word ptr ds:[25EAh] */
    AX = rw(pDS, 0x25EA);
L37E4:
    /* 37E4  imul    word ptr ds:[24D6h] */
    imul16(rw(pDS, 0x24D6));
L37E8:
    /* 37E8  add     ax,bx */
    AX = add16(AX, BX, 0);
L37EA:
    /* 37EA  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L37EC:
    /* 37EC  shl     ax,1 */
    AX = shl16(AX, 1);
L37EE:
    /* 37EE  rcl     dx,1 */
    DX = rcl16(DX, 1);
L37F0:
    /* 37F0  mov     word ptr ds:[25EAh],dx */
    ww(pDS, 0x25EA, DX);
L37F4:
    /* 37F4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_37F5  (+37F5) */
L37F5: /* _instance_pitch */
    /* 37F5  mov     ax,ss */
    AX = asm_ss;
L37F7:
    /* 37F7  mov     ds,ax */
    SET_DS(AX);
L37F9:
    /* 37F9  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A34), 0x065C + PORT_LOAD_SEG, 0x37FE)) != 0) return c;
L37FE:
    /* 37FE  mov     dx,es */
    DX = asm_es;
L3800:
    /* 3800  mov     ds,dx */
    SET_DS(DX);
L3802:
    /* 3802  mov     word ptr ds:[24D8h],ax */
    ww(pDS, 0x24D8, AX);
L3805:
    /* 3805  mov     word ptr ds:[24D6h],bx */
    ww(pDS, 0x24D6, BX);
L3809:
    /* 3809  neg     ax */
    AX = (uint16_t)-AX;
L380B:
    /* 380B  mov     word ptr ds:[24DAh],ax */
    ww(pDS, 0x24DA, AX);
L380E:
    /* 380E  call    _instance_vvars_pitch */
    if ((c = asm_call(ASM_JMP(0x065C, 0x37B8), 0x3811)) != 0) return c;
L3811:
    /* 3811  mov     ax,word ptr ds:[24D6h] */
    AX = rw(pDS, 0x24D6);
L3814:
    /* 3814  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L3818:
    /* 3818  mov     bx,ax */
    BX = AX;
L381A:
    /* 381A  mov     cx,dx */
    CX = DX;
L381C:
    /* 381C  mov     ax,word ptr ds:[24DAh] */
    AX = rw(pDS, 0x24DA);
L381F:
    /* 381F  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L3823:
    /* 3823  add     bx,ax */
    BX = add16(BX, AX, 0);
L3825:
    /* 3825  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3827:
    /* 3827  add     bx,bx */
    BX = add16(BX, BX, 0);
L3829:
    /* 3829  adc     cx,cx */
    CX = add16(CX, CX, CF);
L382B:
    /* 382B  jno     short L3837 */
    if (!OF) goto L3837;
L382D:
    /* 382D  jg      short L3834 */
    if (!ZF && SF == OF) goto L3834;
L382F:
    /* 382F  mov     cx,8000h */
    CX = 0x8000;
L3832:
    /* 3832  jmp     short L3837 */
    goto L3837;
L3834: /* L3834 */
    /* 3834  mov     cx,7FFFh */
    CX = 0x7FFF;
L3837: /* L3837 */
    /* 3837  mov     word ptr ds:[2494h],cx */
    ww(pDS, 0x2494, CX);
L383B:
    /* 383B  mov     ax,word ptr ds:[24D6h] */
    AX = rw(pDS, 0x24D6);
L383E:
    /* 383E  imul    word ptr ds:[14BAh] */
    imul16(rw(pDS, 0x14BA));
L3842:
    /* 3842  mov     bx,ax */
    BX = AX;
L3844:
    /* 3844  mov     cx,dx */
    CX = DX;
L3846:
    /* 3846  mov     ax,word ptr ds:[24DAh] */
    AX = rw(pDS, 0x24DA);
L3849:
    /* 3849  imul    word ptr ds:[14C0h] */
    imul16(rw(pDS, 0x14C0));
L384D:
    /* 384D  add     bx,ax */
    BX = add16(BX, AX, 0);
L384F:
    /* 384F  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3851:
    /* 3851  add     bx,bx */
    BX = add16(BX, BX, 0);
L3853:
    /* 3853  adc     cx,cx */
    CX = add16(CX, CX, CF);
L3855:
    /* 3855  jno     short L3861 */
    if (!OF) goto L3861;
L3857:
    /* 3857  jg      short L385E */
    if (!ZF && SF == OF) goto L385E;
L3859:
    /* 3859  mov     cx,8000h */
    CX = 0x8000;
L385C:
    /* 385C  jmp     short L3861 */
    goto L3861;
L385E: /* L385E */
    /* 385E  mov     cx,7FFFh */
    CX = 0x7FFF;
L3861: /* L3861 */
    /* 3861  mov     word ptr ds:[2496h],cx */
    ww(pDS, 0x2496, CX);
L3865:
    /* 3865  mov     ax,word ptr ds:[24D6h] */
    AX = rw(pDS, 0x24D6);
L3868:
    /* 3868  imul    word ptr ds:[14BCh] */
    imul16(rw(pDS, 0x14BC));
L386C:
    /* 386C  mov     bx,ax */
    BX = AX;
L386E:
    /* 386E  mov     cx,dx */
    CX = DX;
L3870:
    /* 3870  mov     ax,word ptr ds:[24DAh] */
    AX = rw(pDS, 0x24DA);
L3873:
    /* 3873  imul    word ptr ds:[14C2h] */
    imul16(rw(pDS, 0x14C2));
L3877:
    /* 3877  add     bx,ax */
    BX = add16(BX, AX, 0);
L3879:
    /* 3879  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L387B:
    /* 387B  add     bx,bx */
    BX = add16(BX, BX, 0);
L387D:
    /* 387D  adc     cx,cx */
    CX = add16(CX, CX, CF);
L387F:
    /* 387F  jno     short L388B */
    if (!OF) goto L388B;
L3881:
    /* 3881  jg      short L3888 */
    if (!ZF && SF == OF) goto L3888;
L3883:
    /* 3883  mov     cx,8000h */
    CX = 0x8000;
L3886:
    /* 3886  jmp     short L388B */
    goto L388B;
L3888: /* L3888 */
    /* 3888  mov     cx,7FFFh */
    CX = 0x7FFF;
L388B: /* L388B */
    /* 388B  mov     word ptr ds:[2498h],cx */
    ww(pDS, 0x2498, CX);
L388F:
    /* 388F  mov     ax,word ptr ds:[24D8h] */
    AX = rw(pDS, 0x24D8);
L3892:
    /* 3892  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L3896:
    /* 3896  mov     bx,ax */
    BX = AX;
L3898:
    /* 3898  mov     cx,dx */
    CX = DX;
L389A:
    /* 389A  mov     ax,word ptr ds:[24D6h] */
    AX = rw(pDS, 0x24D6);
L389D:
    /* 389D  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L38A1:
    /* 38A1  add     bx,ax */
    BX = add16(BX, AX, 0);
L38A3:
    /* 38A3  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L38A5:
    /* 38A5  add     bx,bx */
    BX = add16(BX, BX, 0);
L38A7:
    /* 38A7  adc     cx,cx */
    CX = add16(CX, CX, CF);
L38A9:
    /* 38A9  jno     short L38B5 */
    if (!OF) goto L38B5;
L38AB:
    /* 38AB  jg      short L38B2 */
    if (!ZF && SF == OF) goto L38B2;
L38AD:
    /* 38AD  mov     cx,8000h */
    CX = 0x8000;
L38B0:
    /* 38B0  jmp     short L38B5 */
    goto L38B5;
L38B2: /* L38B2 */
    /* 38B2  mov     cx,7FFFh */
    CX = 0x7FFF;
L38B5: /* L38B5 */
    /* 38B5  mov     word ptr ds:[14BEh],cx */
    ww(pDS, 0x14BE, CX);
L38B9:
    /* 38B9  mov     ax,word ptr ds:[24D8h] */
    AX = rw(pDS, 0x24D8);
L38BC:
    /* 38BC  imul    word ptr ds:[14BAh] */
    imul16(rw(pDS, 0x14BA));
L38C0:
    /* 38C0  mov     bx,ax */
    BX = AX;
L38C2:
    /* 38C2  mov     cx,dx */
    CX = DX;
L38C4:
    /* 38C4  mov     ax,word ptr ds:[24D6h] */
    AX = rw(pDS, 0x24D6);
L38C7:
    /* 38C7  imul    word ptr ds:[14C0h] */
    imul16(rw(pDS, 0x14C0));
L38CB:
    /* 38CB  add     bx,ax */
    BX = add16(BX, AX, 0);
L38CD:
    /* 38CD  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L38CF:
    /* 38CF  add     bx,bx */
    BX = add16(BX, BX, 0);
L38D1:
    /* 38D1  adc     cx,cx */
    CX = add16(CX, CX, CF);
L38D3:
    /* 38D3  jno     short L38DF */
    if (!OF) goto L38DF;
L38D5:
    /* 38D5  jg      short L38DC */
    if (!ZF && SF == OF) goto L38DC;
L38D7:
    /* 38D7  mov     cx,8000h */
    CX = 0x8000;
L38DA:
    /* 38DA  jmp     short L38DF */
    goto L38DF;
L38DC: /* L38DC */
    /* 38DC  mov     cx,7FFFh */
    CX = 0x7FFF;
L38DF: /* L38DF */
    /* 38DF  mov     word ptr ds:[14C0h],cx */
    ww(pDS, 0x14C0, CX);
L38E3:
    /* 38E3  mov     ax,word ptr ds:[24D8h] */
    AX = rw(pDS, 0x24D8);
L38E6:
    /* 38E6  imul    word ptr ds:[14BCh] */
    imul16(rw(pDS, 0x14BC));
L38EA:
    /* 38EA  mov     bx,ax */
    BX = AX;
L38EC:
    /* 38EC  mov     cx,dx */
    CX = DX;
L38EE:
    /* 38EE  mov     ax,word ptr ds:[24D6h] */
    AX = rw(pDS, 0x24D6);
L38F1:
    /* 38F1  imul    word ptr ds:[14C2h] */
    imul16(rw(pDS, 0x14C2));
L38F5:
    /* 38F5  add     bx,ax */
    BX = add16(BX, AX, 0);
L38F7:
    /* 38F7  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L38F9:
    /* 38F9  add     bx,bx */
    BX = add16(BX, BX, 0);
L38FB:
    /* 38FB  adc     cx,cx */
    CX = add16(CX, CX, CF);
L38FD:
    /* 38FD  jno     short L3909 */
    if (!OF) goto L3909;
L38FF:
    /* 38FF  jg      short L3906 */
    if (!ZF && SF == OF) goto L3906;
L3901:
    /* 3901  mov     cx,8000h */
    CX = 0x8000;
L3904:
    /* 3904  jmp     short L3909 */
    goto L3909;
L3906: /* L3906 */
    /* 3906  mov     cx,7FFFh */
    CX = 0x7FFF;
L3909: /* L3909 */
    /* 3909  mov     word ptr ds:[14C2h],cx */
    ww(pDS, 0x14C2, CX);
L390D:
    /* 390D  mov     ax,word ptr ds:[2494h] */
    AX = rw(pDS, 0x2494);
L3910:
    /* 3910  mov     word ptr ds:[14B8h],ax */
    ww(pDS, 0x14B8, AX);
L3913:
    /* 3913  mov     ax,word ptr ds:[2496h] */
    AX = rw(pDS, 0x2496);
L3916:
    /* 3916  mov     word ptr ds:[14BAh],ax */
    ww(pDS, 0x14BA, AX);
L3919:
    /* 3919  mov     ax,word ptr ds:[2498h] */
    AX = rw(pDS, 0x2498);
L391C:
    /* 391C  mov     word ptr ds:[14BCh],ax */
    ww(pDS, 0x14BC, AX);
L391F:
    /* 391F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_3920  (+3920) */
L3920: /* _instance_vvars_bank */
    /* 3920  mov     ax,word ptr ds:[25E8h] */
    AX = rw(pDS, 0x25E8);
L3923:
    /* 3923  imul    word ptr ds:[24D0h] */
    imul16(rw(pDS, 0x24D0));
L3927:
    /* 3927  mov     bx,ax */
    BX = AX;
L3929:
    /* 3929  mov     cx,dx */
    CX = DX;
L392B:
    /* 392B  mov     ax,word ptr ds:[25E6h] */
    AX = rw(pDS, 0x25E6);
L392E:
    /* 392E  imul    word ptr ds:[24D4h] */
    imul16(rw(pDS, 0x24D4));
L3932:
    /* 3932  add     ax,bx */
    AX = add16(AX, BX, 0);
L3934:
    /* 3934  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L3936:
    /* 3936  shl     ax,1 */
    AX = shl16(AX, 1);
L3938:
    /* 3938  rcl     dx,1 */
    DX = rcl16(DX, 1);
L393A:
    /* 393A  mov     ax,word ptr ds:[25E8h] */
    AX = rw(pDS, 0x25E8);
L393D:
    /* 393D  mov     word ptr ds:[25E8h],dx */
    ww(pDS, 0x25E8, DX);
L3941:
    /* 3941  imul    word ptr ds:[24D2h] */
    imul16(rw(pDS, 0x24D2));
L3945:
    /* 3945  mov     cx,dx */
    CX = DX;
L3947:
    /* 3947  mov     bx,ax */
    BX = AX;
L3949:
    /* 3949  mov     ax,word ptr ds:[25E6h] */
    AX = rw(pDS, 0x25E6);
L394C:
    /* 394C  imul    word ptr ds:[24D0h] */
    imul16(rw(pDS, 0x24D0));
L3950:
    /* 3950  add     bx,ax */
    BX = add16(BX, AX, 0);
L3952:
    /* 3952  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3954:
    /* 3954  shl     bx,1 */
    BX = shl16(BX, 1);
L3956:
    /* 3956  rcl     cx,1 */
    CX = rcl16(CX, 1);
L3958:
    /* 3958  mov     word ptr ds:[25E6h],cx */
    ww(pDS, 0x25E6, CX);
L395C:
    /* 395C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_395D  (+395D) */
L395D: /* _instance_bank */
    /* 395D  mov     ax,ss */
    AX = asm_ss;
L395F:
    /* 395F  mov     ds,ax */
    SET_DS(AX);
L3961:
    /* 3961  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A34), 0x065C + PORT_LOAD_SEG, 0x3966)) != 0) return c;
L3966:
    /* 3966  mov     dx,es */
    DX = asm_es;
L3968:
    /* 3968  mov     ds,dx */
    SET_DS(DX);
L396A:
    /* 396A  mov     word ptr ds:[24D2h],ax */
    ww(pDS, 0x24D2, AX);
L396D:
    /* 396D  mov     word ptr ds:[24D0h],bx */
    ww(pDS, 0x24D0, BX);
L3971:
    /* 3971  neg     ax */
    AX = (uint16_t)-AX;
L3973:
    /* 3973  mov     word ptr ds:[24D4h],ax */
    ww(pDS, 0x24D4, AX);
L3976:
    /* 3976  call    _instance_vvars_bank */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3920), 0x3979)) != 0) return c;
L3979:
    /* 3979  mov     ax,word ptr ds:[24D0h] */
    AX = rw(pDS, 0x24D0);
L397C:
    /* 397C  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L3980:
    /* 3980  mov     bx,ax */
    BX = AX;
L3982:
    /* 3982  mov     cx,dx */
    CX = DX;
L3984:
    /* 3984  mov     ax,word ptr ds:[24D2h] */
    AX = rw(pDS, 0x24D2);
L3987:
    /* 3987  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L398B:
    /* 398B  add     bx,ax */
    BX = add16(BX, AX, 0);
L398D:
    /* 398D  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L398F:
    /* 398F  add     bx,bx */
    BX = add16(BX, BX, 0);
L3991:
    /* 3991  adc     cx,cx */
    CX = add16(CX, CX, CF);
L3993:
    /* 3993  jno     short L399F */
    if (!OF) goto L399F;
L3995:
    /* 3995  jg      short L399C */
    if (!ZF && SF == OF) goto L399C;
L3997:
    /* 3997  mov     cx,8000h */
    CX = 0x8000;
L399A:
    /* 399A  jmp     short L399F */
    goto L399F;
L399C: /* L399C */
    /* 399C  mov     cx,7FFFh */
    CX = 0x7FFF;
L399F: /* L399F */
    /* 399F  mov     word ptr ds:[2494h],cx */
    ww(pDS, 0x2494, CX);
L39A3:
    /* 39A3  mov     ax,word ptr ds:[24D0h] */
    AX = rw(pDS, 0x24D0);
L39A6:
    /* 39A6  imul    word ptr ds:[14B4h] */
    imul16(rw(pDS, 0x14B4));
L39AA:
    /* 39AA  mov     bx,ax */
    BX = AX;
L39AC:
    /* 39AC  mov     cx,dx */
    CX = DX;
L39AE:
    /* 39AE  mov     ax,word ptr ds:[24D2h] */
    AX = rw(pDS, 0x24D2);
L39B1:
    /* 39B1  imul    word ptr ds:[14BAh] */
    imul16(rw(pDS, 0x14BA));
L39B5:
    /* 39B5  add     bx,ax */
    BX = add16(BX, AX, 0);
L39B7:
    /* 39B7  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L39B9:
    /* 39B9  add     bx,bx */
    BX = add16(BX, BX, 0);
L39BB:
    /* 39BB  adc     cx,cx */
    CX = add16(CX, CX, CF);
L39BD:
    /* 39BD  jno     short L39C9 */
    if (!OF) goto L39C9;
L39BF:
    /* 39BF  jg      short L39C6 */
    if (!ZF && SF == OF) goto L39C6;
L39C1:
    /* 39C1  mov     cx,8000h */
    CX = 0x8000;
L39C4:
    /* 39C4  jmp     short L39C9 */
    goto L39C9;
L39C6: /* L39C6 */
    /* 39C6  mov     cx,7FFFh */
    CX = 0x7FFF;
L39C9: /* L39C9 */
    /* 39C9  mov     word ptr ds:[2496h],cx */
    ww(pDS, 0x2496, CX);
L39CD:
    /* 39CD  mov     ax,word ptr ds:[24D0h] */
    AX = rw(pDS, 0x24D0);
L39D0:
    /* 39D0  imul    word ptr ds:[14B6h] */
    imul16(rw(pDS, 0x14B6));
L39D4:
    /* 39D4  mov     bx,ax */
    BX = AX;
L39D6:
    /* 39D6  mov     cx,dx */
    CX = DX;
L39D8:
    /* 39D8  mov     ax,word ptr ds:[24D2h] */
    AX = rw(pDS, 0x24D2);
L39DB:
    /* 39DB  imul    word ptr ds:[14BCh] */
    imul16(rw(pDS, 0x14BC));
L39DF:
    /* 39DF  add     bx,ax */
    BX = add16(BX, AX, 0);
L39E1:
    /* 39E1  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L39E3:
    /* 39E3  add     bx,bx */
    BX = add16(BX, BX, 0);
L39E5:
    /* 39E5  adc     cx,cx */
    CX = add16(CX, CX, CF);
L39E7:
    /* 39E7  jno     short L39F3 */
    if (!OF) goto L39F3;
L39E9:
    /* 39E9  jg      short L39F0 */
    if (!ZF && SF == OF) goto L39F0;
L39EB:
    /* 39EB  mov     cx,8000h */
    CX = 0x8000;
L39EE:
    /* 39EE  jmp     short L39F3 */
    goto L39F3;
L39F0: /* L39F0 */
    /* 39F0  mov     cx,7FFFh */
    CX = 0x7FFF;
L39F3: /* L39F3 */
    /* 39F3  mov     word ptr ds:[2498h],cx */
    ww(pDS, 0x2498, CX);
L39F7:
    /* 39F7  mov     ax,word ptr ds:[24D4h] */
    AX = rw(pDS, 0x24D4);
L39FA:
    /* 39FA  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L39FE:
    /* 39FE  mov     bx,ax */
    BX = AX;
L3A00:
    /* 3A00  mov     cx,dx */
    CX = DX;
L3A02:
    /* 3A02  mov     ax,word ptr ds:[24D0h] */
    AX = rw(pDS, 0x24D0);
L3A05:
    /* 3A05  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L3A09:
    /* 3A09  add     bx,ax */
    BX = add16(BX, AX, 0);
L3A0B:
    /* 3A0B  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3A0D:
    /* 3A0D  add     bx,bx */
    BX = add16(BX, BX, 0);
L3A0F:
    /* 3A0F  adc     cx,cx */
    CX = add16(CX, CX, CF);
L3A11:
    /* 3A11  jno     short L3A1D */
    if (!OF) goto L3A1D;
L3A13:
    /* 3A13  jg      short L3A1A */
    if (!ZF && SF == OF) goto L3A1A;
L3A15:
    /* 3A15  mov     cx,8000h */
    CX = 0x8000;
L3A18:
    /* 3A18  jmp     short L3A1D */
    goto L3A1D;
L3A1A: /* L3A1A */
    /* 3A1A  mov     cx,7FFFh */
    CX = 0x7FFF;
L3A1D: /* L3A1D */
    /* 3A1D  mov     word ptr ds:[14B8h],cx */
    ww(pDS, 0x14B8, CX);
L3A21:
    /* 3A21  mov     ax,word ptr ds:[24D4h] */
    AX = rw(pDS, 0x24D4);
L3A24:
    /* 3A24  imul    word ptr ds:[14B4h] */
    imul16(rw(pDS, 0x14B4));
L3A28:
    /* 3A28  mov     bx,ax */
    BX = AX;
L3A2A:
    /* 3A2A  mov     cx,dx */
    CX = DX;
L3A2C:
    /* 3A2C  mov     ax,word ptr ds:[24D0h] */
    AX = rw(pDS, 0x24D0);
L3A2F:
    /* 3A2F  imul    word ptr ds:[14BAh] */
    imul16(rw(pDS, 0x14BA));
L3A33:
    /* 3A33  add     bx,ax */
    BX = add16(BX, AX, 0);
L3A35:
    /* 3A35  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3A37:
    /* 3A37  add     bx,bx */
    BX = add16(BX, BX, 0);
L3A39:
    /* 3A39  adc     cx,cx */
    CX = add16(CX, CX, CF);
L3A3B:
    /* 3A3B  jno     short L3A47 */
    if (!OF) goto L3A47;
L3A3D:
    /* 3A3D  jg      short L3A44 */
    if (!ZF && SF == OF) goto L3A44;
L3A3F:
    /* 3A3F  mov     cx,8000h */
    CX = 0x8000;
L3A42:
    /* 3A42  jmp     short L3A47 */
    goto L3A47;
L3A44: /* L3A44 */
    /* 3A44  mov     cx,7FFFh */
    CX = 0x7FFF;
L3A47: /* L3A47 */
    /* 3A47  mov     word ptr ds:[14BAh],cx */
    ww(pDS, 0x14BA, CX);
L3A4B:
    /* 3A4B  mov     ax,word ptr ds:[24D4h] */
    AX = rw(pDS, 0x24D4);
L3A4E:
    /* 3A4E  imul    word ptr ds:[14B6h] */
    imul16(rw(pDS, 0x14B6));
L3A52:
    /* 3A52  mov     bx,ax */
    BX = AX;
L3A54:
    /* 3A54  mov     cx,dx */
    CX = DX;
L3A56:
    /* 3A56  mov     ax,word ptr ds:[24D0h] */
    AX = rw(pDS, 0x24D0);
L3A59:
    /* 3A59  imul    word ptr ds:[14BCh] */
    imul16(rw(pDS, 0x14BC));
L3A5D:
    /* 3A5D  add     bx,ax */
    BX = add16(BX, AX, 0);
L3A5F:
    /* 3A5F  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3A61:
    /* 3A61  add     bx,bx */
    BX = add16(BX, BX, 0);
L3A63:
    /* 3A63  adc     cx,cx */
    CX = add16(CX, CX, CF);
L3A65:
    /* 3A65  jno     short L3A71 */
    if (!OF) goto L3A71;
L3A67:
    /* 3A67  jg      short L3A6E */
    if (!ZF && SF == OF) goto L3A6E;
L3A69:
    /* 3A69  mov     cx,8000h */
    CX = 0x8000;
L3A6C:
    /* 3A6C  jmp     short L3A71 */
    goto L3A71;
L3A6E: /* L3A6E */
    /* 3A6E  mov     cx,7FFFh */
    CX = 0x7FFF;
L3A71: /* L3A71 */
    /* 3A71  mov     word ptr ds:[14BCh],cx */
    ww(pDS, 0x14BC, CX);
L3A75:
    /* 3A75  mov     ax,word ptr ds:[2494h] */
    AX = rw(pDS, 0x2494);
L3A78:
    /* 3A78  mov     word ptr ds:[14B2h],ax */
    ww(pDS, 0x14B2, AX);
L3A7B:
    /* 3A7B  mov     ax,word ptr ds:[2496h] */
    AX = rw(pDS, 0x2496);
L3A7E:
    /* 3A7E  mov     word ptr ds:[14B4h],ax */
    ww(pDS, 0x14B4, AX);
L3A81:
    /* 3A81  mov     ax,word ptr ds:[2498h] */
    AX = rw(pDS, 0x2498);
L3A84:
    /* 3A84  mov     word ptr ds:[14B6h],ax */
    ww(pDS, 0x14B6, AX);
L3A87:
    /* 3A87  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_3A88  (+3A88)
       code_pnt: the clip codes of a view-space point. In: BX = x, CX = y, BP = z. Out: AL = 0 inside
       the frustum, else bits 1 (x < -z), 2 (x > z), 4 (y > z), 8 (y < -z), and 80h when z <= 0
       (behind the eye, with the side bits set from the signs). The four planes are CLIP.ASM's.
       Changes AX only. */
L3A88: /* _code_pnt */
    /* 3A88  mov     ax,bp */
    AX = BP;
L3A8A:
    /* 3A8A  neg     ax */
    AX = neg16(AX);
L3A8C:
    /* 3A8C  jge     short L3AC9 */
    if (SF == OF) goto L3AC9;
L3A8E:
    /* 3A8E  cmp     bx,ax */
    sub16(BX, AX, 0);
L3A90:
    /* 3A90  jl      short L3AA1 */
    if (SF != OF) goto L3AA1;
L3A92:
    /* 3A92  cmp     bx,bp */
    sub16(BX, BP, 0);
L3A94:
    /* 3A94  jg      short L3AB2 */
    if (!ZF && SF == OF) goto L3AB2;
L3A96:
    /* 3A96  cmp     cx,bp */
    sub16(CX, BP, 0);
L3A98:
    /* 3A98  jg      short L3AC3 */
    if (!ZF && SF == OF) goto L3AC3;
L3A9A:
    /* 3A9A  cmp     cx,ax */
    sub16(CX, AX, 0);
L3A9C:
    /* 3A9C  jl      short L3AC6 */
    if (SF != OF) goto L3AC6;
L3A9E:
    /* 3A9E  xor     al,al */
    AL = logic8((uint8_t)(AL ^ AL));
L3AA0:
    /* 3AA0  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AA1: /* L3AA1 */
    /* 3AA1  cmp     cx,bp */
    sub16(CX, BP, 0);
L3AA3:
    /* 3AA3  jg      short L3AAC */
    if (!ZF && SF == OF) goto L3AAC;
L3AA5:
    /* 3AA5  cmp     cx,ax */
    sub16(CX, AX, 0);
L3AA7:
    /* 3AA7  jl      short L3AAF */
    if (SF != OF) goto L3AAF;
L3AA9:
    /* 3AA9  mov     al,1 */
    AL = 0x1;
L3AAB:
    /* 3AAB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AAC: /* L3AAC */
    /* 3AAC  mov     al,5 */
    AL = 0x5;
L3AAE:
    /* 3AAE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AAF: /* L3AAF */
    /* 3AAF  mov     al,9 */
    AL = 0x9;
L3AB1:
    /* 3AB1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AB2: /* L3AB2 */
    /* 3AB2  cmp     cx,bp */
    sub16(CX, BP, 0);
L3AB4:
    /* 3AB4  jg      short L3ABD */
    if (!ZF && SF == OF) goto L3ABD;
L3AB6:
    /* 3AB6  cmp     cx,ax */
    sub16(CX, AX, 0);
L3AB8:
    /* 3AB8  jl      short L3AC0 */
    if (SF != OF) goto L3AC0;
L3ABA:
    /* 3ABA  mov     al,2 */
    AL = 0x2;
L3ABC:
    /* 3ABC  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3ABD: /* L3ABD */
    /* 3ABD  mov     al,6 */
    AL = 0x6;
L3ABF:
    /* 3ABF  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AC0: /* L3AC0 */
    /* 3AC0  mov     al,0Ah */
    AL = 0xA;
L3AC2:
    /* 3AC2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AC3: /* L3AC3 */
    /* 3AC3  mov     al,4 */
    AL = 0x4;
L3AC5:
    /* 3AC5  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AC6: /* L3AC6 */
    /* 3AC6  mov     al,8 */
    AL = 0x8;
L3AC8:
    /* 3AC8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AC9: /* L3AC9 */
    /* 3AC9  cmp     bx,ax */
    sub16(BX, AX, 0);
L3ACB:
    /* 3ACB  jl      short L3AD4 */
    if (SF != OF) goto L3AD4;
L3ACD:
    /* 3ACD  cmp     cx,bp */
    sub16(CX, BP, 0);
L3ACF:
    /* 3ACF  jg      short L3AFA */
    if (!ZF && SF == OF) goto L3AFA;
L3AD1:
    /* 3AD1  mov     al,8Ah */
    AL = 0x8A;
L3AD3:
    /* 3AD3  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AD4: /* L3AD4 */
    /* 3AD4  cmp     bx,bp */
    sub16(BX, BP, 0);
L3AD6:
    /* 3AD6  jg      short L3ADF */
    if (!ZF && SF == OF) goto L3ADF;
L3AD8:
    /* 3AD8  cmp     cx,bp */
    sub16(CX, BP, 0);
L3ADA:
    /* 3ADA  jg      short L3AF0 */
    if (!ZF && SF == OF) goto L3AF0;
L3ADC:
    /* 3ADC  mov     al,89h */
    AL = 0x89;
L3ADE:
    /* 3ADE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3ADF: /* L3ADF */
    /* 3ADF  cmp     cx,bp */
    sub16(CX, BP, 0);
L3AE1:
    /* 3AE1  jg      short L3AE6 */
    if (!ZF && SF == OF) goto L3AE6;
L3AE3:
    /* 3AE3  mov     al,8Bh */
    AL = 0x8B;
L3AE5:
    /* 3AE5  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AE6: /* L3AE6 */
    /* 3AE6  cmp     cx,ax */
    sub16(CX, AX, 0);
L3AE8:
    /* 3AE8  jl      short L3AED */
    if (SF != OF) goto L3AED;
L3AEA:
    /* 3AEA  mov     al,87h */
    AL = 0x87;
L3AEC:
    /* 3AEC  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AED: /* L3AED */
    /* 3AED  mov     al,8Fh */
    AL = 0x8F;
L3AEF:
    /* 3AEF  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AF0: /* L3AF0 */
    /* 3AF0  cmp     cx,ax */
    sub16(CX, AX, 0);
L3AF2:
    /* 3AF2  jl      short L3AF7 */
    if (SF != OF) goto L3AF7;
L3AF4:
    /* 3AF4  mov     al,85h */
    AL = 0x85;
L3AF6:
    /* 3AF6  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AF7: /* L3AF7 */
    /* 3AF7  mov     al,8Dh */
    AL = 0x8D;
L3AF9:
    /* 3AF9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AFA: /* L3AFA */
    /* 3AFA  cmp     cx,ax */
    sub16(CX, AX, 0);
L3AFC:
    /* 3AFC  jl      short L3B01 */
    if (SF != OF) goto L3B01;
L3AFE:
    /* 3AFE  mov     al,86h */
    AL = 0x86;
L3B00:
    /* 3B00  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3B01: /* L3B01 */
    /* 3B01  mov     al,8Eh */
    AL = 0x8E;
L3B03:
    /* 3B03  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_3B04  (+3B04)
       mm9x9: the view matrix (written through ES) = the 3 x 3 matrix at BP times the one at SI, row
       by row through mm3x9_bpsi. BP is advanced by 18 bytes. */
L3B04: /* _mm9x9 */
    /* 3B04  call    _mm3x9_bpsi */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3B41), 0x3B07)) != 0) return c;
L3B07:
    /* 3B07  mov     word ptr es:[14B2h],bx */
    ww(pES, 0x14B2, BX);
L3B0C:
    /* 3B0C  mov     word ptr es:[14B4h],cx */
    ww(pES, 0x14B4, CX);
L3B11:
    /* 3B11  mov     word ptr es:[14B6h],di */
    ww(pES, 0x14B6, DI);
L3B16:
    /* 3B16  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L3B19:
    /* 3B19  call    _mm3x9_bpsi */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3B41), 0x3B1C)) != 0) return c;
L3B1C:
    /* 3B1C  mov     word ptr es:[14B8h],bx */
    ww(pES, 0x14B8, BX);
L3B21:
    /* 3B21  mov     word ptr es:[14BAh],cx */
    ww(pES, 0x14BA, CX);
L3B26:
    /* 3B26  mov     word ptr es:[14BCh],di */
    ww(pES, 0x14BC, DI);
L3B2B:
    /* 3B2B  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L3B2E:
    /* 3B2E  call    _mm3x9_bpsi */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3B41), 0x3B31)) != 0) return c;
L3B31:
    /* 3B31  mov     word ptr es:[14BEh],bx */
    ww(pES, 0x14BE, BX);
L3B36:
    /* 3B36  mov     word ptr es:[14C0h],cx */
    ww(pES, 0x14C0, CX);
L3B3B:
    /* 3B3B  mov     word ptr es:[14C2h],di */
    ww(pES, 0x14C2, DI);
L3B40:
    /* 3B40  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_3B41  (+3B41)
       mm3x9_bpsi: a row times a matrix: BP = a row of three 1.15 words, SI = a 3 x 3 matrix by rows.
       Out: BX, CX, DI = the row's three products, saturated to +-8001h on overflow (8000h becomes
       8001h, so negation is safe). The overflow fix-up of the third product reads and writes CX, not
       DI, so an overflow there overwrites the second result and leaves the third wrapped; FM Towns'
       mm3x9_bpsi saturates DI. Probably a slip in the DOS source. */
L3B41: /* _mm3x9_bpsi */
    /* 3B41  mov     ax,word ptr [bp] */
    AX = rw(pSS, BP);
L3B44:
    /* 3B44  imul    word ptr [si] */
    imul16(rw(pDS, SI));
L3B46:
    /* 3B46  mov     bx,ax */
    BX = AX;
L3B48:
    /* 3B48  mov     cx,dx */
    CX = DX;
L3B4A:
    /* 3B4A  mov     ax,word ptr [bp+2] */
    AX = rw(pSS, BP + 0x2);
L3B4D:
    /* 3B4D  imul    word ptr [si+6] */
    imul16(rw(pDS, SI + 0x6));
L3B50:
    /* 3B50  add     bx,ax */
    BX = add16(BX, AX, 0);
L3B52:
    /* 3B52  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3B54:
    /* 3B54  mov     ax,word ptr [bp+4] */
    AX = rw(pSS, BP + 0x4);
L3B57:
    /* 3B57  imul    word ptr [si+0Ch] */
    imul16(rw(pDS, SI + 0xC));
L3B5A:
    /* 3B5A  add     bx,ax */
    BX = add16(BX, AX, 0);
L3B5C:
    /* 3B5C  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3B5E:
    /* 3B5E  shl     bx,1 */
    BX = shl16(BX, 1);
L3B60:
    /* 3B60  rcl     cx,1 */
    CX = rcl16(CX, 1);
L3B62:
    /* 3B62  jno     short L3B6E */
    if (!OF) goto L3B6E;
L3B64:
    /* 3B64  mov     ax,cx */
    AX = CX;
L3B66:
    /* 3B66  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3B67:
    /* 3B67  mov     cx,8001h */
    CX = 0x8001;
L3B6A:
    /* 3B6A  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L3B6C:
    /* 3B6C  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L3B6E: /* L3B6E */
    /* 3B6E  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L3B72:
    /* 3B72  jne     short L3B77 */
    if (!ZF) goto L3B77;
L3B74:
    /* 3B74  mov     cx,8001h */
    CX = 0x8001;
L3B77: /* L3B77 */
    /* 3B77  push    cx */
    push16(CX);
L3B78:
    /* 3B78  mov     ax,word ptr [bp] */
    AX = rw(pSS, BP);
L3B7B:
    /* 3B7B  imul    word ptr [si+2] */
    imul16(rw(pDS, SI + 0x2));
L3B7E:
    /* 3B7E  mov     bx,ax */
    BX = AX;
L3B80:
    /* 3B80  mov     cx,dx */
    CX = DX;
L3B82:
    /* 3B82  mov     ax,word ptr [bp+2] */
    AX = rw(pSS, BP + 0x2);
L3B85:
    /* 3B85  imul    word ptr [si+8] */
    imul16(rw(pDS, SI + 0x8));
L3B88:
    /* 3B88  add     bx,ax */
    BX = add16(BX, AX, 0);
L3B8A:
    /* 3B8A  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3B8C:
    /* 3B8C  mov     ax,word ptr [bp+4] */
    AX = rw(pSS, BP + 0x4);
L3B8F:
    /* 3B8F  imul    word ptr [si+0Eh] */
    imul16(rw(pDS, SI + 0xE));
L3B92:
    /* 3B92  add     bx,ax */
    BX = add16(BX, AX, 0);
L3B94:
    /* 3B94  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3B96:
    /* 3B96  shl     bx,1 */
    BX = shl16(BX, 1);
L3B98:
    /* 3B98  rcl     cx,1 */
    CX = rcl16(CX, 1);
L3B9A:
    /* 3B9A  jno     short L3BA6 */
    if (!OF) goto L3BA6;
L3B9C:
    /* 3B9C  mov     ax,cx */
    AX = CX;
L3B9E:
    /* 3B9E  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3B9F:
    /* 3B9F  mov     cx,8001h */
    CX = 0x8001;
L3BA2:
    /* 3BA2  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L3BA4:
    /* 3BA4  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L3BA6: /* L3BA6 */
    /* 3BA6  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L3BAA:
    /* 3BAA  jne     short L3BAF */
    if (!ZF) goto L3BAF;
L3BAC:
    /* 3BAC  mov     cx,8001h */
    CX = 0x8001;
L3BAF: /* L3BAF */
    /* 3BAF  mov     ax,word ptr [bp] */
    AX = rw(pSS, BP);
L3BB2:
    /* 3BB2  imul    word ptr [si+4] */
    imul16(rw(pDS, SI + 0x4));
L3BB5:
    /* 3BB5  mov     bx,ax */
    BX = AX;
L3BB7:
    /* 3BB7  mov     di,dx */
    DI = DX;
L3BB9:
    /* 3BB9  mov     ax,word ptr [bp+2] */
    AX = rw(pSS, BP + 0x2);
L3BBC:
    /* 3BBC  imul    word ptr [si+0Ah] */
    imul16(rw(pDS, SI + 0xA));
L3BBF:
    /* 3BBF  add     bx,ax */
    BX = add16(BX, AX, 0);
L3BC1:
    /* 3BC1  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L3BC3:
    /* 3BC3  mov     ax,word ptr [bp+4] */
    AX = rw(pSS, BP + 0x4);
L3BC6:
    /* 3BC6  imul    word ptr [si+10h] */
    imul16(rw(pDS, SI + 0x10));
L3BC9:
    /* 3BC9  add     bx,ax */
    BX = add16(BX, AX, 0);
L3BCB:
    /* 3BCB  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L3BCD:
    /* 3BCD  shl     bx,1 */
    BX = shl16(BX, 1);
L3BCF:
    /* 3BCF  rcl     di,1 */
    DI = rcl16(DI, 1);
L3BD1:
    /* 3BD1  jno     short L3BDD */
    if (!OF) goto L3BDD;
L3BD3:
    /* 3BD3  mov     ax,cx */
    AX = CX;
L3BD5:
    /* 3BD5  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3BD6:
    /* 3BD6  mov     cx,8001h */
    CX = 0x8001;
L3BD9:
    /* 3BD9  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L3BDB:
    /* 3BDB  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L3BDD: /* L3BDD */
    /* 3BDD  cmp     di,8000h */
    sub16(DI, 0x8000, 0);
L3BE1:
    /* 3BE1  jne     short L3BE6 */
    if (!ZF) goto L3BE6;
L3BE3:
    /* 3BE3  mov     di,8001h */
    DI = 0x8001;
L3BE6: /* L3BE6 */
    /* 3BE6  pop     bx */
    BX = pop16();
L3BE7:
    /* 3BE7  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_3BE8  (+3BE8)
       mm3x9t: as mm3x9_bpsi with the matrix at BP taken transposed (a row at SI times the columns of
       BP's matrix), with the same slip in the third product's overflow fix-up. */
L3BE8: /* _mm3x9t */
    /* 3BE8  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L3BEA:
    /* 3BEA  imul    word ptr [bp] */
    imul16(rw(pSS, BP));
L3BED:
    /* 3BED  mov     bx,ax */
    BX = AX;
L3BEF:
    /* 3BEF  mov     cx,dx */
    CX = DX;
L3BF1:
    /* 3BF1  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L3BF4:
    /* 3BF4  imul    word ptr [bp+2] */
    imul16(rw(pSS, BP + 0x2));
L3BF7:
    /* 3BF7  add     bx,ax */
    BX = add16(BX, AX, 0);
L3BF9:
    /* 3BF9  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3BFB:
    /* 3BFB  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L3BFE:
    /* 3BFE  imul    word ptr [bp+4] */
    imul16(rw(pSS, BP + 0x4));
L3C01:
    /* 3C01  add     bx,ax */
    BX = add16(BX, AX, 0);
L3C03:
    /* 3C03  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3C05:
    /* 3C05  shl     bx,1 */
    BX = shl16(BX, 1);
L3C07:
    /* 3C07  rcl     cx,1 */
    CX = rcl16(CX, 1);
L3C09:
    /* 3C09  jno     short L3C15 */
    if (!OF) goto L3C15;
L3C0B:
    /* 3C0B  mov     ax,cx */
    AX = CX;
L3C0D:
    /* 3C0D  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3C0E:
    /* 3C0E  mov     cx,8001h */
    CX = 0x8001;
L3C11:
    /* 3C11  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L3C13:
    /* 3C13  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L3C15: /* L3C15 */
    /* 3C15  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L3C19:
    /* 3C19  jne     short L3C1E */
    if (!ZF) goto L3C1E;
L3C1B:
    /* 3C1B  mov     cx,8001h */
    CX = 0x8001;
L3C1E: /* L3C1E */
    /* 3C1E  push    cx */
    push16(CX);
L3C1F:
    /* 3C1F  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L3C21:
    /* 3C21  imul    word ptr [bp+6] */
    imul16(rw(pSS, BP + 0x6));
L3C24:
    /* 3C24  mov     bx,ax */
    BX = AX;
L3C26:
    /* 3C26  mov     cx,dx */
    CX = DX;
L3C28:
    /* 3C28  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L3C2B:
    /* 3C2B  imul    word ptr [bp+8] */
    imul16(rw(pSS, BP + 0x8));
L3C2E:
    /* 3C2E  add     bx,ax */
    BX = add16(BX, AX, 0);
L3C30:
    /* 3C30  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3C32:
    /* 3C32  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L3C35:
    /* 3C35  imul    word ptr [bp+0Ah] */
    imul16(rw(pSS, BP + 0xA));
L3C38:
    /* 3C38  add     bx,ax */
    BX = add16(BX, AX, 0);
L3C3A:
    /* 3C3A  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3C3C:
    /* 3C3C  shl     bx,1 */
    BX = shl16(BX, 1);
L3C3E:
    /* 3C3E  rcl     cx,1 */
    CX = rcl16(CX, 1);
L3C40:
    /* 3C40  jno     short L3C4C */
    if (!OF) goto L3C4C;
L3C42:
    /* 3C42  mov     ax,cx */
    AX = CX;
L3C44:
    /* 3C44  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3C45:
    /* 3C45  mov     cx,8001h */
    CX = 0x8001;
L3C48:
    /* 3C48  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L3C4A:
    /* 3C4A  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L3C4C: /* L3C4C */
    /* 3C4C  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L3C50:
    /* 3C50  jne     short L3C55 */
    if (!ZF) goto L3C55;
L3C52:
    /* 3C52  mov     cx,8001h */
    CX = 0x8001;
L3C55: /* L3C55 */
    /* 3C55  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L3C57:
    /* 3C57  imul    word ptr [bp+0Ch] */
    imul16(rw(pSS, BP + 0xC));
L3C5A:
    /* 3C5A  mov     bx,ax */
    BX = AX;
L3C5C:
    /* 3C5C  mov     di,dx */
    DI = DX;
L3C5E:
    /* 3C5E  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L3C61:
    /* 3C61  imul    word ptr [bp+0Eh] */
    imul16(rw(pSS, BP + 0xE));
L3C64:
    /* 3C64  add     bx,ax */
    BX = add16(BX, AX, 0);
L3C66:
    /* 3C66  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L3C68:
    /* 3C68  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L3C6B:
    /* 3C6B  imul    word ptr [bp+10h] */
    imul16(rw(pSS, BP + 0x10));
L3C6E:
    /* 3C6E  add     bx,ax */
    BX = add16(BX, AX, 0);
L3C70:
    /* 3C70  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L3C72:
    /* 3C72  shl     bx,1 */
    BX = shl16(BX, 1);
L3C74:
    /* 3C74  rcl     di,1 */
    DI = rcl16(DI, 1);
L3C76:
    /* 3C76  jno     short L3C82 */
    if (!OF) goto L3C82;
L3C78:
    /* 3C78  mov     ax,cx */
    AX = CX;
L3C7A:
    /* 3C7A  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3C7B:
    /* 3C7B  mov     cx,8001h */
    CX = 0x8001;
L3C7E:
    /* 3C7E  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L3C80:
    /* 3C80  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L3C82: /* L3C82 */
    /* 3C82  cmp     di,8000h */
    sub16(DI, 0x8000, 0);
L3C86:
    /* 3C86  jne     short L3C8B */
    if (!ZF) goto L3C8B;
L3C88:
    /* 3C88  mov     di,8001h */
    DI = 0x8001;
L3C8B: /* L3C8B */
    /* 3C8B  pop     bx */
    BX = pop16();
L3C8C:
    /* 3C8C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_3C91  (+3C91)
       overflow_handler_reg: the int 0 handler for a 2-byte `idiv reg`: skips the instruction and
       returns AX = +-7FFFh by the sign of DX (DX the sign extension), and counts the overflow
       (do_scalebm abandons a sprite that overflowed). */
L3C91: /* _overflow_handler_reg */
    /* 3C91  mov     word ptr cs:L3C8D,bp */
    ww(CODE004, 0x3C8D, BP);
L3C96:
    /* 3C96  mov     bp,sp */
    BP = SP;
L3C98:
    /* 3C98  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L3C9B:
    /* 3C9B  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L3C9E:
    /* 3C9E  mov     ax,7FFFh */
    AX = 0x7FFF;
L3CA1:
    /* 3CA1  test    dx,0FFFFh */
    logic16((uint16_t)(DX & 0xFFFF));
L3CA5:
    /* 3CA5  jns     short L3CA9 */
    if (!SF) goto L3CA9;
L3CA7:
    /* 3CA7  neg     ax */
    AX = neg16(AX);
L3CA9: /* L3CA9 */
    /* 3CA9  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3CAA:
    /* 3CAA  mov     bp,word ptr cs:L3C8D */
    BP = rw(CODE004, 0x3C8D);
L3CAF:
    /* 3CAF  inc     word ptr cs:overflow_count */
    ww(CODE004, 0x3C8F, inc16(rw(CODE004, 0x3C8F)));
L3CB4:
    /* 3CB4  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;

    /* seg004_0849_3CB5  (+3CB5)
       overflow_handler_mem: for a 4-byte divide by a memory operand: skips it, AX = 7FFFh, DX = 0. */
L3CB5: /* _overflow_handler_mem */
    /* 3CB5  mov     word ptr cs:L3C8D,bp */
    ww(CODE004, 0x3C8D, BP);
L3CBA:
    /* 3CBA  mov     bp,sp */
    BP = SP;
L3CBC:
    /* 3CBC  add     word ptr [bp],4 */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 0x4));
L3CC0:
    /* 3CC0  mov     ax,7FFFh */
    AX = 0x7FFF;
L3CC3:
    /* 3CC3  xor     dx,dx */
    DX = logic16((uint16_t)(DX ^ DX));
L3CC5:
    /* 3CC5  mov     bp,word ptr cs:L3C8D */
    BP = rw(CODE004, 0x3C8D);
L3CCA:
    /* 3CCA  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;

    /* seg004_0849_3CCB  (+3CCB)
       overflow_handler_special_bp: for a 2-byte divide by BP: skips it and returns +-7FFFh by the
       sign of DX xor BP (the quotient's sign), DX = 0. */
L3CCB: /* _overflow_handler_special_bp */
    /* 3CCB  mov     word ptr cs:L3C8D,bp */
    ww(CODE004, 0x3C8D, BP);
L3CD0:
    /* 3CD0  mov     bp,sp */
    BP = SP;
L3CD2:
    /* 3CD2  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L3CD5:
    /* 3CD5  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L3CD8:
    /* 3CD8  mov     bp,word ptr cs:L3C8D */
    BP = rw(CODE004, 0x3C8D);
L3CDD:
    /* 3CDD  mov     ax,dx */
    AX = DX;
L3CDF:
    /* 3CDF  xor     ax,bp */
    AX = (uint16_t)(AX ^ BP);
L3CE1:
    /* 3CE1  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3CE2:
    /* 3CE2  mov     ax,7FFFh */
    AX = 0x7FFF;
L3CE5:
    /* 3CE5  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L3CE7:
    /* 3CE7  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L3CE9:
    /* 3CE9  xor     dx,dx */
    DX = logic16((uint16_t)(DX ^ DX));
L3CEB:
    /* 3CEB  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;
}
