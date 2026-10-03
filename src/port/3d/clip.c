/* clip.c: replaces src/3d/CLIP.ASM (seg004_0849_40B0, 40B0..46EE of its
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

uint32_t asm_mod_CLIP(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x40B0: goto L40B0;
    case 0x40B1: goto L40B1;
    case 0x40B4: goto L40B4;
    case 0x40B7: goto L40B7;
    case 0x40BB: goto L40BB;
    case 0x40BF: goto L40BF;
    case 0x40C3: goto L40C3;
    case 0x40C7: goto L40C7;
    case 0x40CA: goto L40CA;
    case 0x40CD: goto L40CD;
    case 0x40D1: goto L40D1;
    case 0x40D5: goto L40D5;
    case 0x40D7: goto L40D7;
    case 0x40DA: goto L40DA;
    case 0x40DD: goto L40DD;
    case 0x40E0: goto L40E0;
    case 0x40E3: goto L40E3;
    case 0x40E6: goto L40E6;
    case 0x40EB: goto L40EB;
    case 0x40ED: goto L40ED;
    case 0x40F0: goto L40F0;
    case 0x40F3: goto L40F3;
    case 0x40F7: goto L40F7;
    case 0x40FB: goto L40FB;
    case 0x40FE: goto L40FE;
    case 0x4103: goto L4103;
    case 0x4105: goto L4105;
    case 0x4109: goto L4109;
    case 0x410D: goto L410D;
    case 0x4111: goto L4111;
    case 0x4115: goto L4115;
    case 0x4118: goto L4118;
    case 0x411D: goto L411D;
    case 0x411F: goto L411F;
    case 0x4123: goto L4123;
    case 0x4127: goto L4127;
    case 0x412B: goto L412B;
    case 0x412F: goto L412F;
    case 0x4132: goto L4132;
    case 0x4137: goto L4137;
    case 0x4139: goto L4139;
    case 0x413D: goto L413D;
    case 0x4141: goto L4141;
    case 0x4144: goto L4144;
    case 0x4147: goto L4147;
    case 0x414B: goto L414B;
    case 0x414D: goto L414D;
    case 0x4150: goto L4150;
    case 0x4152: goto L4152;
    case 0x4155: goto L4155;
    case 0x4156: goto L4156;
    case 0x4157: goto L4157;
    case 0x4158: goto L4158;
    case 0x4159: goto L4159;
    case 0x415A: goto L415A;
    case 0x415B: goto L415B;
    case 0x415E: goto L415E;
    case 0x4160: goto L4160;
    case 0x4161: goto L4161;
    case 0x4162: goto L4162;
    case 0x4163: goto L4163;
    case 0x4164: goto L4164;
    case 0x4165: goto L4165;
    case 0x4166: goto L4166;
    case 0x4167: goto L4167;
    case 0x416B: goto L416B;
    case 0x416F: goto L416F;
    case 0x4173: goto L4173;
    case 0x4177: goto L4177;
    case 0x417B: goto L417B;
    case 0x417F: goto L417F;
    case 0x4182: goto L4182;
    case 0x4186: goto L4186;
    case 0x418A: goto L418A;
    case 0x418E: goto L418E;
    case 0x4190: goto L4190;
    case 0x4193: goto L4193;
    case 0x4196: goto L4196;
    case 0x4199: goto L4199;
    case 0x419C: goto L419C;
    case 0x419F: goto L419F;
    case 0x41A4: goto L41A4;
    case 0x41A6: goto L41A6;
    case 0x41A9: goto L41A9;
    case 0x41AC: goto L41AC;
    case 0x41B0: goto L41B0;
    case 0x41B4: goto L41B4;
    case 0x41B7: goto L41B7;
    case 0x41BC: goto L41BC;
    case 0x41BE: goto L41BE;
    case 0x41C2: goto L41C2;
    case 0x41C6: goto L41C6;
    case 0x41CA: goto L41CA;
    case 0x41CE: goto L41CE;
    case 0x41D1: goto L41D1;
    case 0x41D6: goto L41D6;
    case 0x41D8: goto L41D8;
    case 0x41DC: goto L41DC;
    case 0x41E0: goto L41E0;
    case 0x41E4: goto L41E4;
    case 0x41E8: goto L41E8;
    case 0x41EB: goto L41EB;
    case 0x41F0: goto L41F0;
    case 0x41F2: goto L41F2;
    case 0x41F6: goto L41F6;
    case 0x41FA: goto L41FA;
    case 0x41FE: goto L41FE;
    case 0x4201: goto L4201;
    case 0x4205: goto L4205;
    case 0x4207: goto L4207;
    case 0x420A: goto L420A;
    case 0x420C: goto L420C;
    case 0x420F: goto L420F;
    case 0x4210: goto L4210;
    case 0x4211: goto L4211;
    case 0x4212: goto L4212;
    case 0x4213: goto L4213;
    case 0x4214: goto L4214;
    case 0x4215: goto L4215;
    case 0x4218: goto L4218;
    case 0x421A: goto L421A;
    case 0x421B: goto L421B;
    case 0x421C: goto L421C;
    case 0x421D: goto L421D;
    case 0x421E: goto L421E;
    case 0x421F: goto L421F;
    case 0x4220: goto L4220;
    case 0x4221: goto L4221;
    case 0x4224: goto L4224;
    case 0x4227: goto L4227;
    case 0x422B: goto L422B;
    case 0x422F: goto L422F;
    case 0x4233: goto L4233;
    case 0x4237: goto L4237;
    case 0x423A: goto L423A;
    case 0x423E: goto L423E;
    case 0x4242: goto L4242;
    case 0x4244: goto L4244;
    case 0x4247: goto L4247;
    case 0x424A: goto L424A;
    case 0x424D: goto L424D;
    case 0x4250: goto L4250;
    case 0x4253: goto L4253;
    case 0x4258: goto L4258;
    case 0x425A: goto L425A;
    case 0x425D: goto L425D;
    case 0x4260: goto L4260;
    case 0x4264: goto L4264;
    case 0x4268: goto L4268;
    case 0x426B: goto L426B;
    case 0x4270: goto L4270;
    case 0x4272: goto L4272;
    case 0x4276: goto L4276;
    case 0x427A: goto L427A;
    case 0x427E: goto L427E;
    case 0x4282: goto L4282;
    case 0x4285: goto L4285;
    case 0x428A: goto L428A;
    case 0x428C: goto L428C;
    case 0x4290: goto L4290;
    case 0x4294: goto L4294;
    case 0x4298: goto L4298;
    case 0x429C: goto L429C;
    case 0x429F: goto L429F;
    case 0x42A4: goto L42A4;
    case 0x42A6: goto L42A6;
    case 0x42AA: goto L42AA;
    case 0x42AE: goto L42AE;
    case 0x42B1: goto L42B1;
    case 0x42B4: goto L42B4;
    case 0x42B8: goto L42B8;
    case 0x42BA: goto L42BA;
    case 0x42BD: goto L42BD;
    case 0x42BF: goto L42BF;
    case 0x42C2: goto L42C2;
    case 0x42C3: goto L42C3;
    case 0x42C4: goto L42C4;
    case 0x42C5: goto L42C5;
    case 0x42C6: goto L42C6;
    case 0x42C7: goto L42C7;
    case 0x42C8: goto L42C8;
    case 0x42CB: goto L42CB;
    case 0x42CD: goto L42CD;
    case 0x42CE: goto L42CE;
    case 0x42CF: goto L42CF;
    case 0x42D0: goto L42D0;
    case 0x42D1: goto L42D1;
    case 0x42D2: goto L42D2;
    case 0x42D3: goto L42D3;
    case 0x42D4: goto L42D4;
    case 0x42D8: goto L42D8;
    case 0x42DC: goto L42DC;
    case 0x42E0: goto L42E0;
    case 0x42E4: goto L42E4;
    case 0x42E8: goto L42E8;
    case 0x42EC: goto L42EC;
    case 0x42F0: goto L42F0;
    case 0x42F4: goto L42F4;
    case 0x42F8: goto L42F8;
    case 0x42FA: goto L42FA;
    case 0x42FD: goto L42FD;
    case 0x4300: goto L4300;
    case 0x4303: goto L4303;
    case 0x4306: goto L4306;
    case 0x4309: goto L4309;
    case 0x430E: goto L430E;
    case 0x4310: goto L4310;
    case 0x4313: goto L4313;
    case 0x4316: goto L4316;
    case 0x431A: goto L431A;
    case 0x431E: goto L431E;
    case 0x4321: goto L4321;
    case 0x4326: goto L4326;
    case 0x4328: goto L4328;
    case 0x432C: goto L432C;
    case 0x4330: goto L4330;
    case 0x4334: goto L4334;
    case 0x4338: goto L4338;
    case 0x433B: goto L433B;
    case 0x4340: goto L4340;
    case 0x4342: goto L4342;
    case 0x4346: goto L4346;
    case 0x434A: goto L434A;
    case 0x434E: goto L434E;
    case 0x4352: goto L4352;
    case 0x4355: goto L4355;
    case 0x435A: goto L435A;
    case 0x435C: goto L435C;
    case 0x4360: goto L4360;
    case 0x4364: goto L4364;
    case 0x4368: goto L4368;
    case 0x436B: goto L436B;
    case 0x436F: goto L436F;
    case 0x4371: goto L4371;
    case 0x4374: goto L4374;
    case 0x4376: goto L4376;
    case 0x4379: goto L4379;
    case 0x437A: goto L437A;
    case 0x437B: goto L437B;
    case 0x437C: goto L437C;
    case 0x437D: goto L437D;
    case 0x437E: goto L437E;
    case 0x437F: goto L437F;
    case 0x4382: goto L4382;
    case 0x4384: goto L4384;
    case 0x4385: goto L4385;
    case 0x4386: goto L4386;
    case 0x4387: goto L4387;
    case 0x4388: goto L4388;
    case 0x4389: goto L4389;
    case 0x438A: goto L438A;
    case 0x4390: goto L4390;
    case 0x4394: goto L4394;
    case 0x4399: goto L4399;
    case 0x439C: goto L439C;
    case 0x439F: goto L439F;
    case 0x43A3: goto L43A3;
    case 0x43A5: goto L43A5;
    case 0x43A6: goto L43A6;
    case 0x43A7: goto L43A7;
    case 0x43AC: goto L43AC;
    case 0x43AE: goto L43AE;
    case 0x43B1: goto L43B1;
    case 0x43B4: goto L43B4;
    case 0x43B7: goto L43B7;
    case 0x43BB: goto L43BB;
    case 0x43C0: goto L43C0;
    case 0x43C1: goto L43C1;
    case 0x43C2: goto L43C2;
    case 0x43C3: goto L43C3;
    case 0x43C4: goto L43C4;
    case 0x43C5: goto L43C5;
    case 0x43C6: goto L43C6;
    case 0x43C9: goto L43C9;
    case 0x43CB: goto L43CB;
    case 0x43CC: goto L43CC;
    case 0x43CD: goto L43CD;
    case 0x43CE: goto L43CE;
    case 0x43CF: goto L43CF;
    case 0x43D2: goto L43D2;
    case 0x43D6: goto L43D6;
    case 0x43D8: goto L43D8;
    case 0x43D9: goto L43D9;
    case 0x43DE: goto L43DE;
    case 0x43E0: goto L43E0;
    case 0x43E3: goto L43E3;
    case 0x43E4: goto L43E4;
    case 0x43E6: goto L43E6;
    case 0x43E9: goto L43E9;
    case 0x43EA: goto L43EA;
    case 0x43ED: goto L43ED;
    case 0x43F1: goto L43F1;
    case 0x43F6: goto L43F6;
    case 0x43F9: goto L43F9;
    case 0x43FB: goto L43FB;
    case 0x4400: goto L4400;
    case 0x4402: goto L4402;
    case 0x4406: goto L4406;
    case 0x4409: goto L4409;
    case 0x440C: goto L440C;
    case 0x4410: goto L4410;
    case 0x4412: goto L4412;
    case 0x4413: goto L4413;
    case 0x4414: goto L4414;
    case 0x4415: goto L4415;
    case 0x4418: goto L4418;
    case 0x441A: goto L441A;
    case 0x441D: goto L441D;
    case 0x441E: goto L441E;
    case 0x4421: goto L4421;
    case 0x4425: goto L4425;
    case 0x4427: goto L4427;
    case 0x4428: goto L4428;
    case 0x442C: goto L442C;
    case 0x442F: goto L442F;
    case 0x4432: goto L4432;
    case 0x4436: goto L4436;
    case 0x4438: goto L4438;
    case 0x4439: goto L4439;
    case 0x443C: goto L443C;
    case 0x443F: goto L443F;
    case 0x4440: goto L4440;
    case 0x4443: goto L4443;
    case 0x4447: goto L4447;
    case 0x444B: goto L444B;
    case 0x444C: goto L444C;
    case 0x4452: goto L4452;
    case 0x4456: goto L4456;
    case 0x445B: goto L445B;
    case 0x445F: goto L445F;
    case 0x4462: goto L4462;
    case 0x4466: goto L4466;
    case 0x4468: goto L4468;
    case 0x4469: goto L4469;
    case 0x446A: goto L446A;
    case 0x446F: goto L446F;
    case 0x4471: goto L4471;
    case 0x4474: goto L4474;
    case 0x4477: goto L4477;
    case 0x447A: goto L447A;
    case 0x447E: goto L447E;
    case 0x4483: goto L4483;
    case 0x4484: goto L4484;
    case 0x4485: goto L4485;
    case 0x4486: goto L4486;
    case 0x4487: goto L4487;
    case 0x4488: goto L4488;
    case 0x4489: goto L4489;
    case 0x448C: goto L448C;
    case 0x448E: goto L448E;
    case 0x448F: goto L448F;
    case 0x4490: goto L4490;
    case 0x4491: goto L4491;
    case 0x4492: goto L4492;
    case 0x4495: goto L4495;
    case 0x4499: goto L4499;
    case 0x449B: goto L449B;
    case 0x449C: goto L449C;
    case 0x44A1: goto L44A1;
    case 0x44A3: goto L44A3;
    case 0x44A6: goto L44A6;
    case 0x44A7: goto L44A7;
    case 0x44A9: goto L44A9;
    case 0x44AC: goto L44AC;
    case 0x44AD: goto L44AD;
    case 0x44B0: goto L44B0;
    case 0x44B4: goto L44B4;
    case 0x44B9: goto L44B9;
    case 0x44BC: goto L44BC;
    case 0x44BE: goto L44BE;
    case 0x44C3: goto L44C3;
    case 0x44C5: goto L44C5;
    case 0x44C9: goto L44C9;
    case 0x44CD: goto L44CD;
    case 0x44D0: goto L44D0;
    case 0x44D4: goto L44D4;
    case 0x44D6: goto L44D6;
    case 0x44D7: goto L44D7;
    case 0x44D8: goto L44D8;
    case 0x44D9: goto L44D9;
    case 0x44DC: goto L44DC;
    case 0x44DE: goto L44DE;
    case 0x44E1: goto L44E1;
    case 0x44E2: goto L44E2;
    case 0x44E5: goto L44E5;
    case 0x44E9: goto L44E9;
    case 0x44EB: goto L44EB;
    case 0x44EC: goto L44EC;
    case 0x44F0: goto L44F0;
    case 0x44F4: goto L44F4;
    case 0x44F7: goto L44F7;
    case 0x44FB: goto L44FB;
    case 0x44FD: goto L44FD;
    case 0x44FE: goto L44FE;
    case 0x4501: goto L4501;
    case 0x4504: goto L4504;
    case 0x4505: goto L4505;
    case 0x4508: goto L4508;
    case 0x450C: goto L450C;
    case 0x4510: goto L4510;
    case 0x4511: goto L4511;
    case 0x4517: goto L4517;
    case 0x451B: goto L451B;
    case 0x4520: goto L4520;
    case 0x4523: goto L4523;
    case 0x4527: goto L4527;
    case 0x4529: goto L4529;
    case 0x452A: goto L452A;
    case 0x452B: goto L452B;
    case 0x4530: goto L4530;
    case 0x4532: goto L4532;
    case 0x4535: goto L4535;
    case 0x4538: goto L4538;
    case 0x453B: goto L453B;
    case 0x453F: goto L453F;
    case 0x4544: goto L4544;
    case 0x4545: goto L4545;
    case 0x4546: goto L4546;
    case 0x4547: goto L4547;
    case 0x4548: goto L4548;
    case 0x4549: goto L4549;
    case 0x454A: goto L454A;
    case 0x454D: goto L454D;
    case 0x454F: goto L454F;
    case 0x4550: goto L4550;
    case 0x4551: goto L4551;
    case 0x4552: goto L4552;
    case 0x4553: goto L4553;
    case 0x4556: goto L4556;
    case 0x455A: goto L455A;
    case 0x455C: goto L455C;
    case 0x455D: goto L455D;
    case 0x4562: goto L4562;
    case 0x4564: goto L4564;
    case 0x4567: goto L4567;
    case 0x4568: goto L4568;
    case 0x456A: goto L456A;
    case 0x456D: goto L456D;
    case 0x456E: goto L456E;
    case 0x4571: goto L4571;
    case 0x4575: goto L4575;
    case 0x457A: goto L457A;
    case 0x457D: goto L457D;
    case 0x457F: goto L457F;
    case 0x4584: goto L4584;
    case 0x4586: goto L4586;
    case 0x458A: goto L458A;
    case 0x458D: goto L458D;
    case 0x4591: goto L4591;
    case 0x4593: goto L4593;
    case 0x4594: goto L4594;
    case 0x4595: goto L4595;
    case 0x4596: goto L4596;
    case 0x4599: goto L4599;
    case 0x459B: goto L459B;
    case 0x459E: goto L459E;
    case 0x459F: goto L459F;
    case 0x45A2: goto L45A2;
    case 0x45A6: goto L45A6;
    case 0x45A8: goto L45A8;
    case 0x45A9: goto L45A9;
    case 0x45AD: goto L45AD;
    case 0x45B0: goto L45B0;
    case 0x45B4: goto L45B4;
    case 0x45B6: goto L45B6;
    case 0x45B7: goto L45B7;
    case 0x45BA: goto L45BA;
    case 0x45BD: goto L45BD;
    case 0x45BE: goto L45BE;
    case 0x45C1: goto L45C1;
    case 0x45C5: goto L45C5;
    case 0x45C9: goto L45C9;
    case 0x45CA: goto L45CA;
    case 0x45D0: goto L45D0;
    case 0x45D4: goto L45D4;
    case 0x45D9: goto L45D9;
    case 0x45DD: goto L45DD;
    case 0x45E1: goto L45E1;
    case 0x45E3: goto L45E3;
    case 0x45E4: goto L45E4;
    case 0x45E5: goto L45E5;
    case 0x45EA: goto L45EA;
    case 0x45EC: goto L45EC;
    case 0x45EF: goto L45EF;
    case 0x45F2: goto L45F2;
    case 0x45F5: goto L45F5;
    case 0x45F9: goto L45F9;
    case 0x45FE: goto L45FE;
    case 0x45FF: goto L45FF;
    case 0x4600: goto L4600;
    case 0x4601: goto L4601;
    case 0x4602: goto L4602;
    case 0x4603: goto L4603;
    case 0x4604: goto L4604;
    case 0x4607: goto L4607;
    case 0x4609: goto L4609;
    case 0x460A: goto L460A;
    case 0x460B: goto L460B;
    case 0x460C: goto L460C;
    case 0x460D: goto L460D;
    case 0x4610: goto L4610;
    case 0x4614: goto L4614;
    case 0x4616: goto L4616;
    case 0x4617: goto L4617;
    case 0x461C: goto L461C;
    case 0x461E: goto L461E;
    case 0x4621: goto L4621;
    case 0x4622: goto L4622;
    case 0x4624: goto L4624;
    case 0x4627: goto L4627;
    case 0x4628: goto L4628;
    case 0x462B: goto L462B;
    case 0x462F: goto L462F;
    case 0x4634: goto L4634;
    case 0x4637: goto L4637;
    case 0x4639: goto L4639;
    case 0x463E: goto L463E;
    case 0x4640: goto L4640;
    case 0x4644: goto L4644;
    case 0x4648: goto L4648;
    case 0x464C: goto L464C;
    case 0x464E: goto L464E;
    case 0x464F: goto L464F;
    case 0x4650: goto L4650;
    case 0x4651: goto L4651;
    case 0x4654: goto L4654;
    case 0x4656: goto L4656;
    case 0x4659: goto L4659;
    case 0x465A: goto L465A;
    case 0x465D: goto L465D;
    case 0x4661: goto L4661;
    case 0x4663: goto L4663;
    case 0x4664: goto L4664;
    case 0x4668: goto L4668;
    case 0x466C: goto L466C;
    case 0x4670: goto L4670;
    case 0x4672: goto L4672;
    case 0x4673: goto L4673;
    case 0x4676: goto L4676;
    case 0x4679: goto L4679;
    case 0x467A: goto L467A;
    case 0x467D: goto L467D;
    case 0x4681: goto L4681;
    case 0x4685: goto L4685;
    case 0x4686: goto L4686;
    case 0x468A: goto L468A;
    case 0x468E: goto L468E;
    case 0x468F: goto L468F;
    case 0x4690: goto L4690;
    case 0x4691: goto L4691;
    case 0x4692: goto L4692;
    case 0x4693: goto L4693;
    case 0x4696: goto L4696;
    case 0x4699: goto L4699;
    case 0x469B: goto L469B;
    case 0x469C: goto L469C;
    case 0x46A0: goto L46A0;
    case 0x46A3: goto L46A3;
    case 0x46A6: goto L46A6;
    case 0x46A8: goto L46A8;
    case 0x46AD: goto L46AD;
    case 0x46B1: goto L46B1;
    case 0x46B4: goto L46B4;
    case 0x46B7: goto L46B7;
    case 0x46BB: goto L46BB;
    case 0x46BE: goto L46BE;
    case 0x46C0: goto L46C0;
    case 0x46C3: goto L46C3;
    case 0x46C6: goto L46C6;
    case 0x46C9: goto L46C9;
    case 0x46CB: goto L46CB;
    case 0x46CE: goto L46CE;
    case 0x46D1: goto L46D1;
    case 0x46D4: goto L46D4;
    case 0x46D6: goto L46D6;
    case 0x46D9: goto L46D9;
    case 0x46DC: goto L46DC;
    case 0x46DF: goto L46DF;
    case 0x46E0: goto L46E0;
    case 0x46E1: goto L46E1;
    case 0x46E4: goto L46E4;
    case 0x46E5: goto L46E5;
    case 0x46E9: goto L46E9;
    case 0x46EA: goto L46EA;
    case 0x46EB: goto L46EB;
    case 0x46ED: goto L46ED;
    default: asm_bad_entry("CLIP.ASM", entry);
    }

    /* seg004_0849_40B0  (+40B0)
       clip_vertex_x_neg_z: the point where the edge from A to B crosses the plane x = -z.
         in:  SI = vertex A, BX = vertex B, DI = output vertex
         out: [DI] = A + t (B - A) for x, y, u and v, with t = (-zA - xA) / ((xB - xA) + (zB - zA))
              in 16.16; z = -x. CX is preserved.

       If the denominator is 0, or the new vertex lies at x = 0 (and so at the eye), it jumps to
       _asm_clip_polygon's abort path, which drops the stack back to _asm_clip_polygon's frame and
       returns CX = 0: the whole polygon is rejected.

       The push/rep movsb/pop sequence after each `jmp L46E5` in these four routines is never reached
       (it would copy A to the output unchanged).

       The other three are the same for the other planes: _y_neg_z (y = -z), _x_z (x = z), _y_z (y =
       z). */
L40B0: /* _clip_vertex_x_neg_z */
    /* 40B0  push    cx */
    push16(CX);
L40B1:
    /* 40B1  mov     ecx,dword ptr [bx] */
    ECX = rd(pDS, BX);
L40B4:
    /* 40B4  sub     ecx,dword ptr [si] */
    ECX = (uint32_t)(ECX - rd(pDS, SI));
L40B7:
    /* 40B7  add     ecx,dword ptr [bx+8] */
    ECX = (uint32_t)(ECX + rd(pDS, BX + 0x8));
L40BB:
    /* 40BB  sub     ecx,dword ptr [si+8] */
    ECX = sub32(ECX, rd(pDS, SI + 0x8), 0);
L40BF:
    /* 40BF  je      L4152 */
    if (ZF) goto L4152;
L40C3:
    /* 40C3  mov     eax,dword ptr [si+8] */
    EAX = rd(pDS, SI + 0x8);
L40C7:
    /* 40C7  neg     eax */
    EAX = (uint32_t)-EAX;
L40CA:
    /* 40CA  sub     eax,dword ptr [si] */
    EAX = (uint32_t)(EAX - rd(pDS, SI));
L40CD:
    /* 40CD  rol     eax,10h */
    EAX = rol32(EAX, 16);
L40D1:
    /* 40D1  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L40D5:
    /* 40D5  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L40D7:
    /* 40D7  idiv    ecx */
    if (asm_idiv32(ECX) && (c = asm_divfault(0x065C, 0x40D7, 3)) != 0) return c;
L40DA:
    /* 40DA  mov     ecx,eax */
    ECX = EAX;
L40DD:
    /* 40DD  mov     eax,dword ptr [bx] */
    EAX = rd(pDS, BX);
L40E0:
    /* 40E0  sub     eax,dword ptr [si] */
    EAX = (uint32_t)(EAX - rd(pDS, SI));
L40E3:
    /* 40E3  imul    ecx */
    imul32(ECX);
L40E6:
    /* 40E6  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L40EB:
    /* 40EB  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L40ED:
    /* 40ED  add     eax,dword ptr [si] */
    EAX = (uint32_t)(EAX + rd(pDS, SI));
L40F0:
    /* 40F0  mov     dword ptr [di],eax */
    wd(pDS, DI, EAX);
L40F3:
    /* 40F3  mov     eax,dword ptr [bx+4] */
    EAX = rd(pDS, BX + 0x4);
L40F7:
    /* 40F7  sub     eax,dword ptr [si+4] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x4));
L40FB:
    /* 40FB  imul    ecx */
    imul32(ECX);
L40FE:
    /* 40FE  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4103:
    /* 4103  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4105:
    /* 4105  add     eax,dword ptr [si+4] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x4));
L4109:
    /* 4109  mov     dword ptr [di+4],eax */
    wd(pDS, DI + 0x4, EAX);
L410D:
    /* 410D  mov     eax,dword ptr [bx+14h] */
    EAX = rd(pDS, BX + 0x14);
L4111:
    /* 4111  sub     eax,dword ptr [si+14h] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x14));
L4115:
    /* 4115  imul    ecx */
    imul32(ECX);
L4118:
    /* 4118  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L411D:
    /* 411D  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L411F:
    /* 411F  add     eax,dword ptr [si+14h] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x14));
L4123:
    /* 4123  mov     dword ptr [di+14h],eax */
    wd(pDS, DI + 0x14, EAX);
L4127:
    /* 4127  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L412B:
    /* 412B  sub     eax,dword ptr [si+18h] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x18));
L412F:
    /* 412F  imul    ecx */
    imul32(ECX);
L4132:
    /* 4132  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4137:
    /* 4137  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4139:
    /* 4139  add     eax,dword ptr [si+18h] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x18));
L413D:
    /* 413D  mov     dword ptr [di+18h],eax */
    wd(pDS, DI + 0x18, EAX);
L4141:
    /* 4141  mov     eax,dword ptr [di] */
    EAX = rd(pDS, DI);
L4144:
    /* 4144  neg     eax */
    EAX = neg32(EAX);
L4147:
    /* 4147  mov     dword ptr [di+8],eax */
    wd(pDS, DI + 0x8, EAX);
L414B:
    /* 414B  jne     short L4150 */
    if (!ZF) goto L4150;
L414D:
    /* 414D  jmp     L46E5 */
    goto L46E5;
L4150: /* L4150 */
    /* 4150  jmp     short L4164 */
    goto L4164;
L4152: /* L4152 */
    /* 4152  jmp     L46E5 */
    goto L46E5;
L4155:
    /* 4155  push    cx */
    push16(CX);
L4156:
    /* 4156  push    es */
    push16(asm_es);
L4157:
    /* 4157  push    si */
    push16(SI);
L4158:
    /* 4158  push    di */
    push16(DI);
L4159:
    /* 4159  push    ds */
    push16(asm_ds);
L415A:
    /* 415A  pop     es */
    SET_ES(pop16());
L415B:
    /* 415B  mov     cx,20h */
    CX = 0x20;
L415E:
    /* 415E  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L4160:
    /* 4160  pop     di */
    DI = pop16();
L4161:
    /* 4161  pop     si */
    SI = pop16();
L4162:
    /* 4162  pop     es */
    SET_ES(pop16());
L4163:
    /* 4163  pop     cx */
    CX = pop16();
L4164: /* L4164 */
    /* 4164  pop     cx */
    CX = pop16();
L4165:
    /* 4165  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_4166  (+4166) */
L4166: /* _clip_vertex_y_neg_z */
    /* 4166  push    cx */
    push16(CX);
L4167:
    /* 4167  mov     ecx,dword ptr [bx+4] */
    ECX = rd(pDS, BX + 0x4);
L416B:
    /* 416B  sub     ecx,dword ptr [si+4] */
    ECX = (uint32_t)(ECX - rd(pDS, SI + 0x4));
L416F:
    /* 416F  add     ecx,dword ptr [bx+8] */
    ECX = (uint32_t)(ECX + rd(pDS, BX + 0x8));
L4173:
    /* 4173  sub     ecx,dword ptr [si+8] */
    ECX = sub32(ECX, rd(pDS, SI + 0x8), 0);
L4177:
    /* 4177  je      L420C */
    if (ZF) goto L420C;
L417B:
    /* 417B  mov     eax,dword ptr [si+8] */
    EAX = rd(pDS, SI + 0x8);
L417F:
    /* 417F  neg     eax */
    EAX = (uint32_t)-EAX;
L4182:
    /* 4182  sub     eax,dword ptr [si+4] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x4));
L4186:
    /* 4186  rol     eax,10h */
    EAX = rol32(EAX, 16);
L418A:
    /* 418A  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L418E:
    /* 418E  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4190:
    /* 4190  idiv    ecx */
    if (asm_idiv32(ECX) && (c = asm_divfault(0x065C, 0x4190, 3)) != 0) return c;
L4193:
    /* 4193  mov     ecx,eax */
    ECX = EAX;
L4196:
    /* 4196  mov     eax,dword ptr [bx] */
    EAX = rd(pDS, BX);
L4199:
    /* 4199  sub     eax,dword ptr [si] */
    EAX = (uint32_t)(EAX - rd(pDS, SI));
L419C:
    /* 419C  imul    ecx */
    imul32(ECX);
L419F:
    /* 419F  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L41A4:
    /* 41A4  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L41A6:
    /* 41A6  add     eax,dword ptr [si] */
    EAX = (uint32_t)(EAX + rd(pDS, SI));
L41A9:
    /* 41A9  mov     dword ptr [di],eax */
    wd(pDS, DI, EAX);
L41AC:
    /* 41AC  mov     eax,dword ptr [bx+4] */
    EAX = rd(pDS, BX + 0x4);
L41B0:
    /* 41B0  sub     eax,dword ptr [si+4] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x4));
L41B4:
    /* 41B4  imul    ecx */
    imul32(ECX);
L41B7:
    /* 41B7  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L41BC:
    /* 41BC  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L41BE:
    /* 41BE  add     eax,dword ptr [si+4] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x4));
L41C2:
    /* 41C2  mov     dword ptr [di+4],eax */
    wd(pDS, DI + 0x4, EAX);
L41C6:
    /* 41C6  mov     eax,dword ptr [bx+14h] */
    EAX = rd(pDS, BX + 0x14);
L41CA:
    /* 41CA  sub     eax,dword ptr [si+14h] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x14));
L41CE:
    /* 41CE  imul    ecx */
    imul32(ECX);
L41D1:
    /* 41D1  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L41D6:
    /* 41D6  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L41D8:
    /* 41D8  add     eax,dword ptr [si+14h] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x14));
L41DC:
    /* 41DC  mov     dword ptr [di+14h],eax */
    wd(pDS, DI + 0x14, EAX);
L41E0:
    /* 41E0  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L41E4:
    /* 41E4  sub     eax,dword ptr [si+18h] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x18));
L41E8:
    /* 41E8  imul    ecx */
    imul32(ECX);
L41EB:
    /* 41EB  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L41F0:
    /* 41F0  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L41F2:
    /* 41F2  add     eax,dword ptr [si+18h] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x18));
L41F6:
    /* 41F6  mov     dword ptr [di+18h],eax */
    wd(pDS, DI + 0x18, EAX);
L41FA:
    /* 41FA  mov     eax,dword ptr [di+4] */
    EAX = rd(pDS, DI + 0x4);
L41FE:
    /* 41FE  neg     eax */
    EAX = neg32(EAX);
L4201:
    /* 4201  mov     dword ptr [di+8],eax */
    wd(pDS, DI + 0x8, EAX);
L4205:
    /* 4205  jne     short L420A */
    if (!ZF) goto L420A;
L4207:
    /* 4207  jmp     L46E5 */
    goto L46E5;
L420A: /* L420A */
    /* 420A  jmp     short L421E */
    goto L421E;
L420C: /* L420C */
    /* 420C  jmp     L46E5 */
    goto L46E5;
L420F:
    /* 420F  push    cx */
    push16(CX);
L4210:
    /* 4210  push    es */
    push16(asm_es);
L4211:
    /* 4211  push    si */
    push16(SI);
L4212:
    /* 4212  push    di */
    push16(DI);
L4213:
    /* 4213  push    ds */
    push16(asm_ds);
L4214:
    /* 4214  pop     es */
    SET_ES(pop16());
L4215:
    /* 4215  mov     cx,20h */
    CX = 0x20;
L4218:
    /* 4218  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L421A:
    /* 421A  pop     di */
    DI = pop16();
L421B:
    /* 421B  pop     si */
    SI = pop16();
L421C:
    /* 421C  pop     es */
    SET_ES(pop16());
L421D:
    /* 421D  pop     cx */
    CX = pop16();
L421E: /* L421E */
    /* 421E  pop     cx */
    CX = pop16();
L421F:
    /* 421F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_4220  (+4220) */
L4220: /* _clip_vertex_x_z */
    /* 4220  push    cx */
    push16(CX);
L4221:
    /* 4221  mov     ecx,dword ptr [bx] */
    ECX = rd(pDS, BX);
L4224:
    /* 4224  sub     ecx,dword ptr [si] */
    ECX = (uint32_t)(ECX - rd(pDS, SI));
L4227:
    /* 4227  sub     ecx,dword ptr [bx+8] */
    ECX = (uint32_t)(ECX - rd(pDS, BX + 0x8));
L422B:
    /* 422B  add     ecx,dword ptr [si+8] */
    ECX = add32(ECX, rd(pDS, SI + 0x8), 0);
L422F:
    /* 422F  je      L42BF */
    if (ZF) goto L42BF;
L4233:
    /* 4233  mov     eax,dword ptr [si+8] */
    EAX = rd(pDS, SI + 0x8);
L4237:
    /* 4237  sub     eax,dword ptr [si] */
    EAX = (uint32_t)(EAX - rd(pDS, SI));
L423A:
    /* 423A  rol     eax,10h */
    EAX = rol32(EAX, 16);
L423E:
    /* 423E  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L4242:
    /* 4242  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4244:
    /* 4244  idiv    ecx */
    if (asm_idiv32(ECX) && (c = asm_divfault(0x065C, 0x4244, 3)) != 0) return c;
L4247:
    /* 4247  mov     ecx,eax */
    ECX = EAX;
L424A:
    /* 424A  mov     eax,dword ptr [bx] */
    EAX = rd(pDS, BX);
L424D:
    /* 424D  sub     eax,dword ptr [si] */
    EAX = (uint32_t)(EAX - rd(pDS, SI));
L4250:
    /* 4250  imul    ecx */
    imul32(ECX);
L4253:
    /* 4253  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4258:
    /* 4258  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L425A:
    /* 425A  add     eax,dword ptr [si] */
    EAX = (uint32_t)(EAX + rd(pDS, SI));
L425D:
    /* 425D  mov     dword ptr [di],eax */
    wd(pDS, DI, EAX);
L4260:
    /* 4260  mov     eax,dword ptr [bx+4] */
    EAX = rd(pDS, BX + 0x4);
L4264:
    /* 4264  sub     eax,dword ptr [si+4] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x4));
L4268:
    /* 4268  imul    ecx */
    imul32(ECX);
L426B:
    /* 426B  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4270:
    /* 4270  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4272:
    /* 4272  add     eax,dword ptr [si+4] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x4));
L4276:
    /* 4276  mov     dword ptr [di+4],eax */
    wd(pDS, DI + 0x4, EAX);
L427A:
    /* 427A  mov     eax,dword ptr [bx+14h] */
    EAX = rd(pDS, BX + 0x14);
L427E:
    /* 427E  sub     eax,dword ptr [si+14h] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x14));
L4282:
    /* 4282  imul    ecx */
    imul32(ECX);
L4285:
    /* 4285  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L428A:
    /* 428A  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L428C:
    /* 428C  add     eax,dword ptr [si+14h] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x14));
L4290:
    /* 4290  mov     dword ptr [di+14h],eax */
    wd(pDS, DI + 0x14, EAX);
L4294:
    /* 4294  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L4298:
    /* 4298  sub     eax,dword ptr [si+18h] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x18));
L429C:
    /* 429C  imul    ecx */
    imul32(ECX);
L429F:
    /* 429F  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L42A4:
    /* 42A4  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L42A6:
    /* 42A6  add     eax,dword ptr [si+18h] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x18));
L42AA:
    /* 42AA  mov     dword ptr [di+18h],eax */
    wd(pDS, DI + 0x18, EAX);
L42AE:
    /* 42AE  mov     eax,dword ptr [di] */
    EAX = rd(pDS, DI);
L42B1:
    /* 42B1  or      eax,eax */
    EAX = logic32((uint32_t)(EAX | EAX));
L42B4:
    /* 42B4  mov     dword ptr [di+8],eax */
    wd(pDS, DI + 0x8, EAX);
L42B8:
    /* 42B8  jne     short L42BD */
    if (!ZF) goto L42BD;
L42BA:
    /* 42BA  jmp     L46E5 */
    goto L46E5;
L42BD: /* L42BD */
    /* 42BD  jmp     short L42D1 */
    goto L42D1;
L42BF: /* L42BF */
    /* 42BF  jmp     L46E5 */
    goto L46E5;
L42C2:
    /* 42C2  push    cx */
    push16(CX);
L42C3:
    /* 42C3  push    es */
    push16(asm_es);
L42C4:
    /* 42C4  push    si */
    push16(SI);
L42C5:
    /* 42C5  push    di */
    push16(DI);
L42C6:
    /* 42C6  push    ds */
    push16(asm_ds);
L42C7:
    /* 42C7  pop     es */
    SET_ES(pop16());
L42C8:
    /* 42C8  mov     cx,20h */
    CX = 0x20;
L42CB:
    /* 42CB  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L42CD:
    /* 42CD  pop     di */
    DI = pop16();
L42CE:
    /* 42CE  pop     si */
    SI = pop16();
L42CF:
    /* 42CF  pop     es */
    SET_ES(pop16());
L42D0:
    /* 42D0  pop     cx */
    CX = pop16();
L42D1: /* L42D1 */
    /* 42D1  pop     cx */
    CX = pop16();
L42D2:
    /* 42D2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_42D3  (+42D3) */
L42D3: /* _clip_vertex_y_z */
    /* 42D3  push    cx */
    push16(CX);
L42D4:
    /* 42D4  mov     ecx,dword ptr [bx+4] */
    ECX = rd(pDS, BX + 0x4);
L42D8:
    /* 42D8  sub     ecx,dword ptr [si+4] */
    ECX = (uint32_t)(ECX - rd(pDS, SI + 0x4));
L42DC:
    /* 42DC  sub     ecx,dword ptr [bx+8] */
    ECX = (uint32_t)(ECX - rd(pDS, BX + 0x8));
L42E0:
    /* 42E0  add     ecx,dword ptr [si+8] */
    ECX = add32(ECX, rd(pDS, SI + 0x8), 0);
L42E4:
    /* 42E4  je      L4376 */
    if (ZF) goto L4376;
L42E8:
    /* 42E8  mov     eax,dword ptr [si+8] */
    EAX = rd(pDS, SI + 0x8);
L42EC:
    /* 42EC  sub     eax,dword ptr [si+4] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x4));
L42F0:
    /* 42F0  rol     eax,10h */
    EAX = rol32(EAX, 16);
L42F4:
    /* 42F4  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L42F8:
    /* 42F8  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L42FA:
    /* 42FA  idiv    ecx */
    if (asm_idiv32(ECX) && (c = asm_divfault(0x065C, 0x42FA, 3)) != 0) return c;
L42FD:
    /* 42FD  mov     ecx,eax */
    ECX = EAX;
L4300:
    /* 4300  mov     eax,dword ptr [bx] */
    EAX = rd(pDS, BX);
L4303:
    /* 4303  sub     eax,dword ptr [si] */
    EAX = (uint32_t)(EAX - rd(pDS, SI));
L4306:
    /* 4306  imul    ecx */
    imul32(ECX);
L4309:
    /* 4309  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L430E:
    /* 430E  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4310:
    /* 4310  add     eax,dword ptr [si] */
    EAX = (uint32_t)(EAX + rd(pDS, SI));
L4313:
    /* 4313  mov     dword ptr [di],eax */
    wd(pDS, DI, EAX);
L4316:
    /* 4316  mov     eax,dword ptr [bx+4] */
    EAX = rd(pDS, BX + 0x4);
L431A:
    /* 431A  sub     eax,dword ptr [si+4] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x4));
L431E:
    /* 431E  imul    ecx */
    imul32(ECX);
L4321:
    /* 4321  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4326:
    /* 4326  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4328:
    /* 4328  add     eax,dword ptr [si+4] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x4));
L432C:
    /* 432C  mov     dword ptr [di+4],eax */
    wd(pDS, DI + 0x4, EAX);
L4330:
    /* 4330  mov     eax,dword ptr [bx+14h] */
    EAX = rd(pDS, BX + 0x14);
L4334:
    /* 4334  sub     eax,dword ptr [si+14h] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x14));
L4338:
    /* 4338  imul    ecx */
    imul32(ECX);
L433B:
    /* 433B  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4340:
    /* 4340  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4342:
    /* 4342  add     eax,dword ptr [si+14h] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x14));
L4346:
    /* 4346  mov     dword ptr [di+14h],eax */
    wd(pDS, DI + 0x14, EAX);
L434A:
    /* 434A  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L434E:
    /* 434E  sub     eax,dword ptr [si+18h] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x18));
L4352:
    /* 4352  imul    ecx */
    imul32(ECX);
L4355:
    /* 4355  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L435A:
    /* 435A  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L435C:
    /* 435C  add     eax,dword ptr [si+18h] */
    EAX = (uint32_t)(EAX + rd(pDS, SI + 0x18));
L4360:
    /* 4360  mov     dword ptr [di+18h],eax */
    wd(pDS, DI + 0x18, EAX);
L4364:
    /* 4364  mov     eax,dword ptr [di+4] */
    EAX = rd(pDS, DI + 0x4);
L4368:
    /* 4368  or      eax,eax */
    EAX = logic32((uint32_t)(EAX | EAX));
L436B:
    /* 436B  mov     dword ptr [di+8],eax */
    wd(pDS, DI + 0x8, EAX);
L436F:
    /* 436F  jne     short L4374 */
    if (!ZF) goto L4374;
L4371:
    /* 4371  jmp     L46E5 */
    goto L46E5;
L4374: /* L4374 */
    /* 4374  jmp     short L4388 */
    goto L4388;
L4376: /* L4376 */
    /* 4376  jmp     L46E5 */
    goto L46E5;
L4379:
    /* 4379  push    cx */
    push16(CX);
L437A:
    /* 437A  push    es */
    push16(asm_es);
L437B:
    /* 437B  push    si */
    push16(SI);
L437C:
    /* 437C  push    di */
    push16(DI);
L437D:
    /* 437D  push    ds */
    push16(asm_ds);
L437E:
    /* 437E  pop     es */
    SET_ES(pop16());
L437F:
    /* 437F  mov     cx,20h */
    CX = 0x20;
L4382:
    /* 4382  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L4384:
    /* 4384  pop     di */
    DI = pop16();
L4385:
    /* 4385  pop     si */
    SI = pop16();
L4386:
    /* 4386  pop     es */
    SET_ES(pop16());
L4387:
    /* 4387  pop     cx */
    CX = pop16();
L4388: /* L4388 */
    /* 4388  pop     cx */
    CX = pop16();
L4389:
    /* 4389  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_438A  (+438A)
       _asm_clip_polygon_left: one Sutherland-Hodgman stage against x = -z, keeping the side where -x
       < z.
         in:  SI = input vertices, CX = their count, DI = output buffer
         out: CX = output count (0 if nothing is left); DI past the last output vertex

       For each vertex: inside after outside emits the crossing and then the vertex; inside after
       inside emits the vertex; outside after inside emits the crossing. The edge from the last
       vertex back to the first is done after the loop. The first vertex has no previous state
       (_inside = 0FFh), so its incoming edge is handled only by that closing step.

       _bottom (-y < z), _right (x < z) and _top (y < z) are the same with the other test and
       intersection routine. */
L438A: /* __asm_clip_polygon_left */
    /* 438A  mov     word ptr ds:[0C88Ah],0 */
    ww(pDS, 0xC88A, 0x0);
L4390:
    /* 4390  mov     word ptr ds:[0C888h],si */
    ww(pDS, 0xC888, SI);
L4394:
    /* 4394  mov     byte ptr ds:[0C88Ch],0FFh */
    wb(pDS, 0xC88C, 0xFF);
L4399: /* L4399 */
    /* 4399  mov     eax,dword ptr [si] */
    EAX = rd(pDS, SI);
L439C:
    /* 439C  neg     eax */
    EAX = (uint32_t)-EAX;
L439F:
    /* 439F  cmp     eax,dword ptr [si+8] */
    sub32(EAX, rd(pDS, SI + 0x8), 0);
L43A3:
    /* 43A3  jge     L43D9 */
    if (SF == OF) goto L43D9;
L43A5:
    /* 43A5  nop */
    ;
L43A6:
    /* 43A6  nop */
    ;
L43A7:
    /* 43A7  test    byte ptr ds:[0C88Ch],0FFh */
    logic8((uint8_t)(rb(pDS, 0xC88C) & 0xFF));
L43AC:
    /* 43AC  jne     short L43BB */
    if (!ZF) goto L43BB;
L43AE:
    /* 43AE  lea     bx,[si-20h] */
    BX = (uint16_t)(SI + 0xFFE0);
L43B1:
    /* 43B1  call    _clip_vertex_x_neg_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x40B0), 0x43B4)) != 0) return c;
L43B4:
    /* 43B4  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L43B7:
    /* 43B7  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L43BB: /* L43BB */
    /* 43BB  mov     byte ptr ds:[0C88Ch],1 */
    wb(pDS, 0xC88C, 0x1);
L43C0:
    /* 43C0  push    cx */
    push16(CX);
L43C1:
    /* 43C1  push    es */
    push16(asm_es);
L43C2:
    /* 43C2  push    si */
    push16(SI);
L43C3:
    /* 43C3  push    di */
    push16(DI);
L43C4:
    /* 43C4  push    ds */
    push16(asm_ds);
L43C5:
    /* 43C5  pop     es */
    SET_ES(pop16());
L43C6:
    /* 43C6  mov     cx,20h */
    CX = 0x20;
L43C9:
    /* 43C9  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L43CB:
    /* 43CB  pop     di */
    DI = pop16();
L43CC:
    /* 43CC  pop     si */
    SI = pop16();
L43CD:
    /* 43CD  pop     es */
    SET_ES(pop16());
L43CE:
    /* 43CE  pop     cx */
    CX = pop16();
L43CF:
    /* 43CF  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L43D2:
    /* 43D2  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L43D6:
    /* 43D6  jmp     L43F6 */
    goto L43F6;
L43D8: /* L43D9 */
    /* 43D8  nop */
    ;
L43D9:
    /* 43D9  cmp     byte ptr ds:[0C88Ch],1 */
    sub8(rb(pDS, 0xC88C), 0x1, 0);
L43DE:
    /* 43DE  jne     short L43F1 */
    if (!ZF) goto L43F1;
L43E0:
    /* 43E0  lea     bx,[si-20h] */
    BX = (uint16_t)(SI + 0xFFE0);
L43E3:
    /* 43E3  push    si */
    push16(SI);
L43E4:
    /* 43E4  xchg    bx,si */
    { uint16_t t_ = SI;
    SI = BX;
    BX = t_; }
L43E6:
    /* 43E6  call    _clip_vertex_x_neg_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x40B0), 0x43E9)) != 0) return c;
L43E9:
    /* 43E9  pop     si */
    SI = pop16();
L43EA:
    /* 43EA  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L43ED:
    /* 43ED  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L43F1: /* L43F1 */
    /* 43F1  mov     byte ptr ds:[0C88Ch],0 */
    wb(pDS, 0xC88C, 0x0);
L43F6: /* L43F6 */
    /* 43F6  add     si,20h */
    SI = (uint16_t)(SI + 0x20);
L43F9:
    /* 43F9  loop    L4399 */
    if (--CX) goto L4399;
L43FB:
    /* 43FB  test    byte ptr ds:[0C88Ch],0FFh */
    logic8((uint8_t)(rb(pDS, 0xC88C) & 0xFF));
L4400:
    /* 4400  jne     short L4428 */
    if (!ZF) goto L4428;
L4402:
    /* 4402  mov     bx,word ptr ds:[0C888h] */
    BX = rw(pDS, 0xC888);
L4406:
    /* 4406  mov     eax,dword ptr [bx] */
    EAX = rd(pDS, BX);
L4409:
    /* 4409  neg     eax */
    EAX = (uint32_t)-EAX;
L440C:
    /* 440C  cmp     eax,dword ptr [bx+8] */
    sub32(EAX, rd(pDS, BX + 0x8), 0);
L4410:
    /* 4410  jge     L4447 */
    if (SF == OF) goto L4447;
L4412:
    /* 4412  nop */
    ;
L4413:
    /* 4413  nop */
    ;
L4414:
    /* 4414  push    si */
    push16(SI);
L4415:
    /* 4415  sub     si,20h */
    SI = (uint16_t)(SI - 0x20);
L4418:
    /* 4418  xchg    bx,si */
    { uint16_t t_ = SI;
    SI = BX;
    BX = t_; }
L441A:
    /* 441A  call    _clip_vertex_x_neg_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x40B0), 0x441D)) != 0) return c;
L441D:
    /* 441D  pop     si */
    SI = pop16();
L441E:
    /* 441E  add     di,20h */
    DI = add16(DI, 0x20, 0);
L4421:
    /* 4421  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, inc16(rw(pDS, 0xC88A)));
L4425:
    /* 4425  jmp     L4447 */
    goto L4447;
L4427: /* L4428 */
    /* 4427  nop */
    ;
L4428:
    /* 4428  mov     bx,word ptr ds:[0C888h] */
    BX = rw(pDS, 0xC888);
L442C:
    /* 442C  mov     eax,dword ptr [bx] */
    EAX = rd(pDS, BX);
L442F:
    /* 442F  neg     eax */
    EAX = (uint32_t)-EAX;
L4432:
    /* 4432  cmp     eax,dword ptr [bx+8] */
    sub32(EAX, rd(pDS, BX + 0x8), 0);
L4436:
    /* 4436  jl      short L4447 */
    if (SF != OF) goto L4447;
L4438:
    /* 4438  push    si */
    push16(SI);
L4439:
    /* 4439  sub     si,20h */
    SI = (uint16_t)(SI - 0x20);
L443C:
    /* 443C  call    _clip_vertex_x_neg_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x40B0), 0x443F)) != 0) return c;
L443F:
    /* 443F  pop     si */
    SI = pop16();
L4440:
    /* 4440  add     di,20h */
    DI = add16(DI, 0x20, 0);
L4443:
    /* 4443  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, inc16(rw(pDS, 0xC88A)));
L4447: /* L4447 */
    /* 4447  mov     cx,word ptr ds:[0C88Ah] */
    CX = rw(pDS, 0xC88A);
L444B:
    /* 444B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_444C  (+444C) */
L444C: /* __asm_clip_polygon_bottom */
    /* 444C  mov     word ptr ds:[0C88Ah],0 */
    ww(pDS, 0xC88A, 0x0);
L4452:
    /* 4452  mov     word ptr ds:[0C888h],si */
    ww(pDS, 0xC888, SI);
L4456:
    /* 4456  mov     byte ptr ds:[0C88Ch],0FFh */
    wb(pDS, 0xC88C, 0xFF);
L445B: /* L445B */
    /* 445B  mov     eax,dword ptr [si+4] */
    EAX = rd(pDS, SI + 0x4);
L445F:
    /* 445F  neg     eax */
    EAX = (uint32_t)-EAX;
L4462:
    /* 4462  cmp     eax,dword ptr [si+8] */
    sub32(EAX, rd(pDS, SI + 0x8), 0);
L4466:
    /* 4466  jge     L449C */
    if (SF == OF) goto L449C;
L4468:
    /* 4468  nop */
    ;
L4469:
    /* 4469  nop */
    ;
L446A:
    /* 446A  test    byte ptr ds:[0C88Ch],0FFh */
    logic8((uint8_t)(rb(pDS, 0xC88C) & 0xFF));
L446F:
    /* 446F  jne     short L447E */
    if (!ZF) goto L447E;
L4471:
    /* 4471  lea     bx,[si-20h] */
    BX = (uint16_t)(SI + 0xFFE0);
L4474:
    /* 4474  call    _clip_vertex_y_neg_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4166), 0x4477)) != 0) return c;
L4477:
    /* 4477  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L447A:
    /* 447A  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L447E: /* L447E */
    /* 447E  mov     byte ptr ds:[0C88Ch],1 */
    wb(pDS, 0xC88C, 0x1);
L4483:
    /* 4483  push    cx */
    push16(CX);
L4484:
    /* 4484  push    es */
    push16(asm_es);
L4485:
    /* 4485  push    si */
    push16(SI);
L4486:
    /* 4486  push    di */
    push16(DI);
L4487:
    /* 4487  push    ds */
    push16(asm_ds);
L4488:
    /* 4488  pop     es */
    SET_ES(pop16());
L4489:
    /* 4489  mov     cx,20h */
    CX = 0x20;
L448C:
    /* 448C  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L448E:
    /* 448E  pop     di */
    DI = pop16();
L448F:
    /* 448F  pop     si */
    SI = pop16();
L4490:
    /* 4490  pop     es */
    SET_ES(pop16());
L4491:
    /* 4491  pop     cx */
    CX = pop16();
L4492:
    /* 4492  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L4495:
    /* 4495  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L4499:
    /* 4499  jmp     L44B9 */
    goto L44B9;
L449B: /* L449C */
    /* 449B  nop */
    ;
L449C:
    /* 449C  cmp     byte ptr ds:[0C88Ch],1 */
    sub8(rb(pDS, 0xC88C), 0x1, 0);
L44A1:
    /* 44A1  jne     short L44B4 */
    if (!ZF) goto L44B4;
L44A3:
    /* 44A3  lea     bx,[si-20h] */
    BX = (uint16_t)(SI + 0xFFE0);
L44A6:
    /* 44A6  push    si */
    push16(SI);
L44A7:
    /* 44A7  xchg    bx,si */
    { uint16_t t_ = SI;
    SI = BX;
    BX = t_; }
L44A9:
    /* 44A9  call    _clip_vertex_y_neg_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4166), 0x44AC)) != 0) return c;
L44AC:
    /* 44AC  pop     si */
    SI = pop16();
L44AD:
    /* 44AD  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L44B0:
    /* 44B0  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L44B4: /* L44B4 */
    /* 44B4  mov     byte ptr ds:[0C88Ch],0 */
    wb(pDS, 0xC88C, 0x0);
L44B9: /* L44B9 */
    /* 44B9  add     si,20h */
    SI = (uint16_t)(SI + 0x20);
L44BC:
    /* 44BC  loop    L445B */
    if (--CX) goto L445B;
L44BE:
    /* 44BE  test    byte ptr ds:[0C88Ch],0FFh */
    logic8((uint8_t)(rb(pDS, 0xC88C) & 0xFF));
L44C3:
    /* 44C3  jne     short L44EC */
    if (!ZF) goto L44EC;
L44C5:
    /* 44C5  mov     bx,word ptr ds:[0C888h] */
    BX = rw(pDS, 0xC888);
L44C9:
    /* 44C9  mov     eax,dword ptr [bx+4] */
    EAX = rd(pDS, BX + 0x4);
L44CD:
    /* 44CD  neg     eax */
    EAX = (uint32_t)-EAX;
L44D0:
    /* 44D0  cmp     eax,dword ptr [bx+8] */
    sub32(EAX, rd(pDS, BX + 0x8), 0);
L44D4:
    /* 44D4  jge     L450C */
    if (SF == OF) goto L450C;
L44D6:
    /* 44D6  nop */
    ;
L44D7:
    /* 44D7  nop */
    ;
L44D8:
    /* 44D8  push    si */
    push16(SI);
L44D9:
    /* 44D9  sub     si,20h */
    SI = (uint16_t)(SI - 0x20);
L44DC:
    /* 44DC  xchg    bx,si */
    { uint16_t t_ = SI;
    SI = BX;
    BX = t_; }
L44DE:
    /* 44DE  call    _clip_vertex_y_neg_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4166), 0x44E1)) != 0) return c;
L44E1:
    /* 44E1  pop     si */
    SI = pop16();
L44E2:
    /* 44E2  add     di,20h */
    DI = add16(DI, 0x20, 0);
L44E5:
    /* 44E5  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, inc16(rw(pDS, 0xC88A)));
L44E9:
    /* 44E9  jmp     L450C */
    goto L450C;
L44EB: /* L44EC */
    /* 44EB  nop */
    ;
L44EC:
    /* 44EC  mov     bx,word ptr ds:[0C888h] */
    BX = rw(pDS, 0xC888);
L44F0:
    /* 44F0  mov     eax,dword ptr [bx+4] */
    EAX = rd(pDS, BX + 0x4);
L44F4:
    /* 44F4  neg     eax */
    EAX = (uint32_t)-EAX;
L44F7:
    /* 44F7  cmp     eax,dword ptr [bx+8] */
    sub32(EAX, rd(pDS, BX + 0x8), 0);
L44FB:
    /* 44FB  jl      short L450C */
    if (SF != OF) goto L450C;
L44FD:
    /* 44FD  push    si */
    push16(SI);
L44FE:
    /* 44FE  sub     si,20h */
    SI = (uint16_t)(SI - 0x20);
L4501:
    /* 4501  call    _clip_vertex_y_neg_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4166), 0x4504)) != 0) return c;
L4504:
    /* 4504  pop     si */
    SI = pop16();
L4505:
    /* 4505  add     di,20h */
    DI = add16(DI, 0x20, 0);
L4508:
    /* 4508  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, inc16(rw(pDS, 0xC88A)));
L450C: /* L450C */
    /* 450C  mov     cx,word ptr ds:[0C88Ah] */
    CX = rw(pDS, 0xC88A);
L4510:
    /* 4510  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_4511  (+4511) */
L4511: /* __asm_clip_polygon_right */
    /* 4511  mov     word ptr ds:[0C88Ah],0 */
    ww(pDS, 0xC88A, 0x0);
L4517:
    /* 4517  mov     word ptr ds:[0C888h],si */
    ww(pDS, 0xC888, SI);
L451B:
    /* 451B  mov     byte ptr ds:[0C88Ch],0FFh */
    wb(pDS, 0xC88C, 0xFF);
L4520: /* L4520 */
    /* 4520  mov     eax,dword ptr [si] */
    EAX = rd(pDS, SI);
L4523:
    /* 4523  cmp     eax,dword ptr [si+8] */
    sub32(EAX, rd(pDS, SI + 0x8), 0);
L4527:
    /* 4527  jge     L455D */
    if (SF == OF) goto L455D;
L4529:
    /* 4529  nop */
    ;
L452A:
    /* 452A  nop */
    ;
L452B:
    /* 452B  test    byte ptr ds:[0C88Ch],0FFh */
    logic8((uint8_t)(rb(pDS, 0xC88C) & 0xFF));
L4530:
    /* 4530  jne     short L453F */
    if (!ZF) goto L453F;
L4532:
    /* 4532  lea     bx,[si-20h] */
    BX = (uint16_t)(SI + 0xFFE0);
L4535:
    /* 4535  call    _clip_vertex_x_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4220), 0x4538)) != 0) return c;
L4538:
    /* 4538  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L453B:
    /* 453B  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L453F: /* L453F */
    /* 453F  mov     byte ptr ds:[0C88Ch],1 */
    wb(pDS, 0xC88C, 0x1);
L4544:
    /* 4544  push    cx */
    push16(CX);
L4545:
    /* 4545  push    es */
    push16(asm_es);
L4546:
    /* 4546  push    si */
    push16(SI);
L4547:
    /* 4547  push    di */
    push16(DI);
L4548:
    /* 4548  push    ds */
    push16(asm_ds);
L4549:
    /* 4549  pop     es */
    SET_ES(pop16());
L454A:
    /* 454A  mov     cx,20h */
    CX = 0x20;
L454D:
    /* 454D  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L454F:
    /* 454F  pop     di */
    DI = pop16();
L4550:
    /* 4550  pop     si */
    SI = pop16();
L4551:
    /* 4551  pop     es */
    SET_ES(pop16());
L4552:
    /* 4552  pop     cx */
    CX = pop16();
L4553:
    /* 4553  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L4556:
    /* 4556  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L455A:
    /* 455A  jmp     L457A */
    goto L457A;
L455C: /* L455D */
    /* 455C  nop */
    ;
L455D:
    /* 455D  cmp     byte ptr ds:[0C88Ch],1 */
    sub8(rb(pDS, 0xC88C), 0x1, 0);
L4562:
    /* 4562  jne     short L4575 */
    if (!ZF) goto L4575;
L4564:
    /* 4564  lea     bx,[si-20h] */
    BX = (uint16_t)(SI + 0xFFE0);
L4567:
    /* 4567  push    si */
    push16(SI);
L4568:
    /* 4568  xchg    bx,si */
    { uint16_t t_ = SI;
    SI = BX;
    BX = t_; }
L456A:
    /* 456A  call    _clip_vertex_x_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4220), 0x456D)) != 0) return c;
L456D:
    /* 456D  pop     si */
    SI = pop16();
L456E:
    /* 456E  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L4571:
    /* 4571  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L4575: /* L4575 */
    /* 4575  mov     byte ptr ds:[0C88Ch],0 */
    wb(pDS, 0xC88C, 0x0);
L457A: /* L457A */
    /* 457A  add     si,20h */
    SI = (uint16_t)(SI + 0x20);
L457D:
    /* 457D  loop    L4520 */
    if (--CX) goto L4520;
L457F:
    /* 457F  test    byte ptr ds:[0C88Ch],0FFh */
    logic8((uint8_t)(rb(pDS, 0xC88C) & 0xFF));
L4584:
    /* 4584  jne     short L45A9 */
    if (!ZF) goto L45A9;
L4586:
    /* 4586  mov     bx,word ptr ds:[0C888h] */
    BX = rw(pDS, 0xC888);
L458A:
    /* 458A  mov     eax,dword ptr [bx] */
    EAX = rd(pDS, BX);
L458D:
    /* 458D  cmp     eax,dword ptr [bx+8] */
    sub32(EAX, rd(pDS, BX + 0x8), 0);
L4591:
    /* 4591  jge     L45C5 */
    if (SF == OF) goto L45C5;
L4593:
    /* 4593  nop */
    ;
L4594:
    /* 4594  nop */
    ;
L4595:
    /* 4595  push    si */
    push16(SI);
L4596:
    /* 4596  sub     si,20h */
    SI = (uint16_t)(SI - 0x20);
L4599:
    /* 4599  xchg    bx,si */
    { uint16_t t_ = SI;
    SI = BX;
    BX = t_; }
L459B:
    /* 459B  call    _clip_vertex_x_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4220), 0x459E)) != 0) return c;
L459E:
    /* 459E  pop     si */
    SI = pop16();
L459F:
    /* 459F  add     di,20h */
    DI = add16(DI, 0x20, 0);
L45A2:
    /* 45A2  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, inc16(rw(pDS, 0xC88A)));
L45A6:
    /* 45A6  jmp     L45C5 */
    goto L45C5;
L45A8: /* L45A9 */
    /* 45A8  nop */
    ;
L45A9:
    /* 45A9  mov     bx,word ptr ds:[0C888h] */
    BX = rw(pDS, 0xC888);
L45AD:
    /* 45AD  mov     eax,dword ptr [bx] */
    EAX = rd(pDS, BX);
L45B0:
    /* 45B0  cmp     eax,dword ptr [bx+8] */
    sub32(EAX, rd(pDS, BX + 0x8), 0);
L45B4:
    /* 45B4  jl      short L45C5 */
    if (SF != OF) goto L45C5;
L45B6:
    /* 45B6  push    si */
    push16(SI);
L45B7:
    /* 45B7  sub     si,20h */
    SI = (uint16_t)(SI - 0x20);
L45BA:
    /* 45BA  call    _clip_vertex_x_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4220), 0x45BD)) != 0) return c;
L45BD:
    /* 45BD  pop     si */
    SI = pop16();
L45BE:
    /* 45BE  add     di,20h */
    DI = add16(DI, 0x20, 0);
L45C1:
    /* 45C1  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, inc16(rw(pDS, 0xC88A)));
L45C5: /* L45C5 */
    /* 45C5  mov     cx,word ptr ds:[0C88Ah] */
    CX = rw(pDS, 0xC88A);
L45C9:
    /* 45C9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_45CA  (+45CA) */
L45CA: /* __asm_clip_polygon_top */
    /* 45CA  mov     word ptr ds:[0C88Ah],0 */
    ww(pDS, 0xC88A, 0x0);
L45D0:
    /* 45D0  mov     word ptr ds:[0C888h],si */
    ww(pDS, 0xC888, SI);
L45D4:
    /* 45D4  mov     byte ptr ds:[0C88Ch],0FFh */
    wb(pDS, 0xC88C, 0xFF);
L45D9: /* L45D9 */
    /* 45D9  mov     eax,dword ptr [si+4] */
    EAX = rd(pDS, SI + 0x4);
L45DD:
    /* 45DD  cmp     eax,dword ptr [si+8] */
    sub32(EAX, rd(pDS, SI + 0x8), 0);
L45E1:
    /* 45E1  jge     L4617 */
    if (SF == OF) goto L4617;
L45E3:
    /* 45E3  nop */
    ;
L45E4:
    /* 45E4  nop */
    ;
L45E5:
    /* 45E5  test    byte ptr ds:[0C88Ch],0FFh */
    logic8((uint8_t)(rb(pDS, 0xC88C) & 0xFF));
L45EA:
    /* 45EA  jne     short L45F9 */
    if (!ZF) goto L45F9;
L45EC:
    /* 45EC  lea     bx,[si-20h] */
    BX = (uint16_t)(SI + 0xFFE0);
L45EF:
    /* 45EF  call    _clip_vertex_y_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x42D3), 0x45F2)) != 0) return c;
L45F2:
    /* 45F2  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L45F5:
    /* 45F5  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L45F9: /* L45F9 */
    /* 45F9  mov     byte ptr ds:[0C88Ch],1 */
    wb(pDS, 0xC88C, 0x1);
L45FE:
    /* 45FE  push    cx */
    push16(CX);
L45FF:
    /* 45FF  push    es */
    push16(asm_es);
L4600:
    /* 4600  push    si */
    push16(SI);
L4601:
    /* 4601  push    di */
    push16(DI);
L4602:
    /* 4602  push    ds */
    push16(asm_ds);
L4603:
    /* 4603  pop     es */
    SET_ES(pop16());
L4604:
    /* 4604  mov     cx,20h */
    CX = 0x20;
L4607:
    /* 4607  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L4609:
    /* 4609  pop     di */
    DI = pop16();
L460A:
    /* 460A  pop     si */
    SI = pop16();
L460B:
    /* 460B  pop     es */
    SET_ES(pop16());
L460C:
    /* 460C  pop     cx */
    CX = pop16();
L460D:
    /* 460D  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L4610:
    /* 4610  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L4614:
    /* 4614  jmp     L4634 */
    goto L4634;
L4616: /* L4617 */
    /* 4616  nop */
    ;
L4617:
    /* 4617  cmp     byte ptr ds:[0C88Ch],1 */
    sub8(rb(pDS, 0xC88C), 0x1, 0);
L461C:
    /* 461C  jne     short L462F */
    if (!ZF) goto L462F;
L461E:
    /* 461E  lea     bx,[si-20h] */
    BX = (uint16_t)(SI + 0xFFE0);
L4621:
    /* 4621  push    si */
    push16(SI);
L4622:
    /* 4622  xchg    bx,si */
    { uint16_t t_ = SI;
    SI = BX;
    BX = t_; }
L4624:
    /* 4624  call    _clip_vertex_y_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x42D3), 0x4627)) != 0) return c;
L4627:
    /* 4627  pop     si */
    SI = pop16();
L4628:
    /* 4628  add     di,20h */
    DI = (uint16_t)(DI + 0x20);
L462B:
    /* 462B  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, (uint16_t)(rw(pDS, 0xC88A) + 1));
L462F: /* L462F */
    /* 462F  mov     byte ptr ds:[0C88Ch],0 */
    wb(pDS, 0xC88C, 0x0);
L4634: /* L4634 */
    /* 4634  add     si,20h */
    SI = (uint16_t)(SI + 0x20);
L4637:
    /* 4637  loop    L45D9 */
    if (--CX) goto L45D9;
L4639:
    /* 4639  test    byte ptr ds:[0C88Ch],0FFh */
    logic8((uint8_t)(rb(pDS, 0xC88C) & 0xFF));
L463E:
    /* 463E  jne     short L4664 */
    if (!ZF) goto L4664;
L4640:
    /* 4640  mov     bx,word ptr ds:[0C888h] */
    BX = rw(pDS, 0xC888);
L4644:
    /* 4644  mov     eax,dword ptr [bx+4] */
    EAX = rd(pDS, BX + 0x4);
L4648:
    /* 4648  cmp     eax,dword ptr [bx+8] */
    sub32(EAX, rd(pDS, BX + 0x8), 0);
L464C:
    /* 464C  jge     L4681 */
    if (SF == OF) goto L4681;
L464E:
    /* 464E  nop */
    ;
L464F:
    /* 464F  nop */
    ;
L4650:
    /* 4650  push    si */
    push16(SI);
L4651:
    /* 4651  sub     si,20h */
    SI = (uint16_t)(SI - 0x20);
L4654:
    /* 4654  xchg    bx,si */
    { uint16_t t_ = SI;
    SI = BX;
    BX = t_; }
L4656:
    /* 4656  call    _clip_vertex_y_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x42D3), 0x4659)) != 0) return c;
L4659:
    /* 4659  pop     si */
    SI = pop16();
L465A:
    /* 465A  add     di,20h */
    DI = add16(DI, 0x20, 0);
L465D:
    /* 465D  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, inc16(rw(pDS, 0xC88A)));
L4661:
    /* 4661  jmp     L4681 */
    goto L4681;
L4663: /* L4664 */
    /* 4663  nop */
    ;
L4664:
    /* 4664  mov     bx,word ptr ds:[0C888h] */
    BX = rw(pDS, 0xC888);
L4668:
    /* 4668  mov     eax,dword ptr [bx+4] */
    EAX = rd(pDS, BX + 0x4);
L466C:
    /* 466C  cmp     eax,dword ptr [bx+8] */
    sub32(EAX, rd(pDS, BX + 0x8), 0);
L4670:
    /* 4670  jl      short L4681 */
    if (SF != OF) goto L4681;
L4672:
    /* 4672  push    si */
    push16(SI);
L4673:
    /* 4673  sub     si,20h */
    SI = (uint16_t)(SI - 0x20);
L4676:
    /* 4676  call    _clip_vertex_y_z */
    if ((c = asm_call(ASM_JMP(0x065C, 0x42D3), 0x4679)) != 0) return c;
L4679:
    /* 4679  pop     si */
    SI = pop16();
L467A:
    /* 467A  add     di,20h */
    DI = add16(DI, 0x20, 0);
L467D:
    /* 467D  inc     word ptr ds:[0C88Ah] */
    ww(pDS, 0xC88A, inc16(rw(pDS, 0xC88A)));
L4681: /* L4681 */
    /* 4681  mov     cx,word ptr ds:[0C88Ah] */
    CX = rw(pDS, 0xC88A);
L4685:
    /* 4685  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_4686  (+4686)
       _asm_clip_polygon: clip a polygon against the four planes.
         in:  SI = vertices (32-byte records), CX = count (at most 16, the buffers' size)
         out: SI = DS:C892, the clipped polygon; CX = its vertex count, 0 if it is invisible or was
              rejected

       Copies the input to C892, then runs left (C892 to CA92), right (back to C892), top and bottom,
       stopping as soon as a stage leaves nothing. It saves SP in C880 so the intersection routines
       can abandon the polygon from any depth (L46E5).

       The loop at L46A3 only advances SI, which is then reloaded: it does nothing. */
L4686: /* __asm_clip_polygon */
    /* 4686  mov     word ptr ds:[0C890h],cx */
    ww(pDS, 0xC890, CX);
L468A:
    /* 468A  mov     word ptr ds:[0C888h],si */
    ww(pDS, 0xC888, SI);
L468E:
    /* 468E  push    di */
    push16(DI);
L468F:
    /* 468F  push    si */
    push16(SI);
L4690:
    /* 4690  push    es */
    push16(asm_es);
L4691:
    /* 4691  push    ds */
    push16(asm_ds);
L4692:
    /* 4692  pop     es */
    SET_ES(pop16());
L4693:
    /* 4693  mov     di,0C892h */
    DI = 0xC892;
L4696:
    /* 4696  imul    cx,20h */
    CX = imul16x(CX, 0x20);
L4699:
    /* 4699  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L469B:
    /* 469B  pop     es */
    SET_ES(pop16());
L469C:
    /* 469C  mov     cx,word ptr ds:[0C890h] */
    CX = rw(pDS, 0xC890);
L46A0:
    /* 46A0  mov     si,0C892h */
    SI = 0xC892;
L46A3: /* L46A3 */
    /* 46A3  add     si,20h */
    SI = (uint16_t)(SI + 0x20);
L46A6:
    /* 46A6  loop    L46A3 */
    if (--CX) goto L46A3;
L46A8:
    /* 46A8  mov     byte ptr ds:[0C882h],0 */
    wb(pDS, 0xC882, 0x0);
L46AD:
    /* 46AD  mov     word ptr ds:[0C880h],sp */
    ww(pDS, 0xC880, SP);
L46B1:
    /* 46B1  mov     si,0C892h */
    SI = 0xC892;
L46B4:
    /* 46B4  mov     di,0CA92h */
    DI = 0xCA92;
L46B7:
    /* 46B7  mov     cx,word ptr ds:[0C890h] */
    CX = rw(pDS, 0xC890);
L46BB:
    /* 46BB  call    __asm_clip_polygon_left */
    if ((c = asm_call(ASM_JMP(0x065C, 0x438A), 0x46BE)) != 0) return c;
L46BE:
    /* 46BE  jcxz    L46DF */
    if (!CX) goto L46DF;
L46C0:
    /* 46C0  mov     si,0CA92h */
    SI = 0xCA92;
L46C3:
    /* 46C3  mov     di,0C892h */
    DI = 0xC892;
L46C6:
    /* 46C6  call    __asm_clip_polygon_right */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4511), 0x46C9)) != 0) return c;
L46C9:
    /* 46C9  jcxz    L46DF */
    if (!CX) goto L46DF;
L46CB:
    /* 46CB  mov     si,0C892h */
    SI = 0xC892;
L46CE:
    /* 46CE  mov     di,0CA92h */
    DI = 0xCA92;
L46D1:
    /* 46D1  call    __asm_clip_polygon_top */
    if ((c = asm_call(ASM_JMP(0x065C, 0x45CA), 0x46D4)) != 0) return c;
L46D4:
    /* 46D4  jcxz    L46DF */
    if (!CX) goto L46DF;
L46D6:
    /* 46D6  mov     si,0CA92h */
    SI = 0xCA92;
L46D9:
    /* 46D9  mov     di,0C892h */
    DI = 0xC892;
L46DC:
    /* 46DC  call    __asm_clip_polygon_bottom */
    if ((c = asm_call(ASM_JMP(0x065C, 0x444C), 0x46DF)) != 0) return c;
L46DF: /* L46DF */
    /* 46DF  pop     si */
    SI = pop16();
L46E0:
    /* 46E0  pop     di */
    DI = pop16();
L46E1:
    /* 46E1  mov     si,0C892h */
    SI = 0xC892;
L46E4:
    /* 46E4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L46E5: /* L46E5 */
    /* 46E5  mov     sp,word ptr ds:[0C880h] */
    SP = rw(pDS, 0xC880);
L46E9:
    /* 46E9  pop     si */
    SI = pop16();
L46EA:
    /* 46EA  pop     di */
    DI = pop16();
L46EB:
    /* 46EB  sub     cx,cx */
    CX = sub16(CX, CX, 0);
L46ED:
    /* 46ED  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
