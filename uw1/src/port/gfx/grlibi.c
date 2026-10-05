/* grlibi.c: replaces src/gfx/GRLIBI.ASM (seg003_4134, 4134..4C6F of its
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

uint32_t asm_mod_GRLIBI(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x4134: goto L4134;
    case 0x4138: goto L4138;
    case 0x413C: goto L413C;
    case 0x4140: goto L4140;
    case 0x4144: goto L4144;
    case 0x4148: goto L4148;
    case 0x414B: goto L414B;
    case 0x414E: goto L414E;
    case 0x4151: goto L4151;
    case 0x4153: goto L4153;
    case 0x4155: goto L4155;
    case 0x4158: goto L4158;
    case 0x4159: goto L4159;
    case 0x415A: goto L415A;
    case 0x415C: goto L415C;
    case 0x415D: goto L415D;
    case 0x415E: goto L415E;
    case 0x415F: goto L415F;
    case 0x4161: goto L4161;
    case 0x4163: goto L4163;
    case 0x4165: goto L4165;
    case 0x4168: goto L4168;
    case 0x416C: goto L416C;
    case 0x416E: goto L416E;
    case 0x4172: goto L4172;
    case 0x4174: goto L4174;
    case 0x4176: goto L4176;
    case 0x4178: goto L4178;
    case 0x417A: goto L417A;
    case 0x417C: goto L417C;
    case 0x4180: goto L4180;
    case 0x4184: goto L4184;
    case 0x4187: goto L4187;
    case 0x418B: goto L418B;
    case 0x418F: goto L418F;
    case 0x4192: goto L4192;
    case 0x4195: goto L4195;
    case 0x4199: goto L4199;
    case 0x419C: goto L419C;
    case 0x41A0: goto L41A0;
    case 0x41A2: goto L41A2;
    case 0x41A4: goto L41A4;
    case 0x41A7: goto L41A7;
    case 0x41AA: goto L41AA;
    case 0x41AD: goto L41AD;
    case 0x41B1: goto L41B1;
    case 0x41B2: goto L41B2;
    case 0x41B4: goto L41B4;
    case 0x41B6: goto L41B6;
    case 0x41B8: goto L41B8;
    case 0x41BA: goto L41BA;
    case 0x41BC: goto L41BC;
    case 0x41BE: goto L41BE;
    case 0x41BF: goto L41BF;
    case 0x41C0: goto L41C0;
    case 0x41C2: goto L41C2;
    case 0x41C4: goto L41C4;
    case 0x41C6: goto L41C6;
    case 0x41C7: goto L41C7;
    case 0x41C8: goto L41C8;
    case 0x41CA: goto L41CA;
    case 0x41CC: goto L41CC;
    case 0x41CD: goto L41CD;
    case 0x41CE: goto L41CE;
    case 0x41CF: goto L41CF;
    case 0x41D0: goto L41D0;
    case 0x41D4: goto L41D4;
    case 0x41D8: goto L41D8;
    case 0x41DC: goto L41DC;
    case 0x41DD: goto L41DD;
    case 0x41DE: goto L41DE;
    case 0x41DF: goto L41DF;
    case 0x41E0: goto L41E0;
    case 0x41E1: goto L41E1;
    case 0x41E2: goto L41E2;
    case 0x41E3: goto L41E3;
    case 0x41E4: goto L41E4;
    case 0x41E5: goto L41E5;
    case 0x41E9: goto L41E9;
    case 0x41EB: goto L41EB;
    case 0x41EF: goto L41EF;
    case 0x41F0: goto L41F0;
    case 0x41F1: goto L41F1;
    case 0x41F2: goto L41F2;
    case 0x41F3: goto L41F3;
    case 0x41F6: goto L41F6;
    case 0x41F7: goto L41F7;
    case 0x41F9: goto L41F9;
    case 0x41FB: goto L41FB;
    case 0x41FF: goto L41FF;
    case 0x4202: goto L4202;
    case 0x4206: goto L4206;
    case 0x4208: goto L4208;
    case 0x4209: goto L4209;
    case 0x420C: goto L420C;
    case 0x420D: goto L420D;
    case 0x420E: goto L420E;
    case 0x420F: goto L420F;
    case 0x4210: goto L4210;
    case 0x4214: goto L4214;
    case 0x4216: goto L4216;
    case 0x4219: goto L4219;
    case 0x421A: goto L421A;
    case 0x421B: goto L421B;
    case 0x421C: goto L421C;
    case 0x421D: goto L421D;
    case 0x4220: goto L4220;
    case 0x4222: goto L4222;
    case 0x4224: goto L4224;
    case 0x4228: goto L4228;
    case 0x422B: goto L422B;
    case 0x422F: goto L422F;
    case 0x4231: goto L4231;
    case 0x4232: goto L4232;
    case 0x4234: goto L4234;
    case 0x4236: goto L4236;
    case 0x4238: goto L4238;
    case 0x423A: goto L423A;
    case 0x423B: goto L423B;
    case 0x423D: goto L423D;
    case 0x4241: goto L4241;
    case 0x4243: goto L4243;
    case 0x4245: goto L4245;
    case 0x4246: goto L4246;
    case 0x4249: goto L4249;
    case 0x424B: goto L424B;
    case 0x424D: goto L424D;
    case 0x424E: goto L424E;
    case 0x424F: goto L424F;
    case 0x4251: goto L4251;
    case 0x4252: goto L4252;
    case 0x4254: goto L4254;
    case 0x4256: goto L4256;
    case 0x4258: goto L4258;
    case 0x425A: goto L425A;
    case 0x425B: goto L425B;
    case 0x425D: goto L425D;
    case 0x425F: goto L425F;
    case 0x4261: goto L4261;
    case 0x4262: goto L4262;
    case 0x4265: goto L4265;
    case 0x4268: goto L4268;
    case 0x426A: goto L426A;
    case 0x426C: goto L426C;
    case 0x4270: goto L4270;
    case 0x4273: goto L4273;
    case 0x4276: goto L4276;
    case 0x4279: goto L4279;
    case 0x427B: goto L427B;
    case 0x427D: goto L427D;
    case 0x4281: goto L4281;
    case 0x4282: goto L4282;
    case 0x4284: goto L4284;
    case 0x4286: goto L4286;
    case 0x4287: goto L4287;
    case 0x428A: goto L428A;
    case 0x428C: goto L428C;
    case 0x428E: goto L428E;
    case 0x428F: goto L428F;
    case 0x4292: goto L4292;
    case 0x4294: goto L4294;
    case 0x4296: goto L4296;
    case 0x4297: goto L4297;
    case 0x429A: goto L429A;
    case 0x429C: goto L429C;
    case 0x429E: goto L429E;
    case 0x429F: goto L429F;
    case 0x42A2: goto L42A2;
    case 0x42A4: goto L42A4;
    case 0x42A6: goto L42A6;
    case 0x42A7: goto L42A7;
    case 0x42AA: goto L42AA;
    case 0x42AC: goto L42AC;
    case 0x42AE: goto L42AE;
    case 0x42AF: goto L42AF;
    case 0x42B2: goto L42B2;
    case 0x42B4: goto L42B4;
    case 0x42B6: goto L42B6;
    case 0x42B7: goto L42B7;
    case 0x42BA: goto L42BA;
    case 0x42BC: goto L42BC;
    case 0x42BE: goto L42BE;
    case 0x42BF: goto L42BF;
    case 0x42C2: goto L42C2;
    case 0x42C4: goto L42C4;
    case 0x42C6: goto L42C6;
    case 0x42C7: goto L42C7;
    case 0x42CA: goto L42CA;
    case 0x42CC: goto L42CC;
    case 0x42CE: goto L42CE;
    case 0x42CF: goto L42CF;
    case 0x42D2: goto L42D2;
    case 0x42D4: goto L42D4;
    case 0x42D6: goto L42D6;
    case 0x42D7: goto L42D7;
    case 0x42DA: goto L42DA;
    case 0x42DC: goto L42DC;
    case 0x42DE: goto L42DE;
    case 0x42DF: goto L42DF;
    case 0x42E2: goto L42E2;
    case 0x42E4: goto L42E4;
    case 0x42E6: goto L42E6;
    case 0x42E7: goto L42E7;
    case 0x42EA: goto L42EA;
    case 0x42EC: goto L42EC;
    case 0x42EE: goto L42EE;
    case 0x42EF: goto L42EF;
    case 0x42F2: goto L42F2;
    case 0x42F4: goto L42F4;
    case 0x42F6: goto L42F6;
    case 0x42F7: goto L42F7;
    case 0x42FA: goto L42FA;
    case 0x42FC: goto L42FC;
    case 0x42FE: goto L42FE;
    case 0x42FF: goto L42FF;
    case 0x4302: goto L4302;
    case 0x4304: goto L4304;
    case 0x4306: goto L4306;
    case 0x4307: goto L4307;
    case 0x430A: goto L430A;
    case 0x430C: goto L430C;
    case 0x430E: goto L430E;
    case 0x430F: goto L430F;
    case 0x4312: goto L4312;
    case 0x4314: goto L4314;
    case 0x4316: goto L4316;
    case 0x4317: goto L4317;
    case 0x431A: goto L431A;
    case 0x431C: goto L431C;
    case 0x431E: goto L431E;
    case 0x431F: goto L431F;
    case 0x4322: goto L4322;
    case 0x4324: goto L4324;
    case 0x4326: goto L4326;
    case 0x4327: goto L4327;
    case 0x432A: goto L432A;
    case 0x432C: goto L432C;
    case 0x432E: goto L432E;
    case 0x432F: goto L432F;
    case 0x4332: goto L4332;
    case 0x4334: goto L4334;
    case 0x4336: goto L4336;
    case 0x4337: goto L4337;
    case 0x433A: goto L433A;
    case 0x433C: goto L433C;
    case 0x433E: goto L433E;
    case 0x433F: goto L433F;
    case 0x4342: goto L4342;
    case 0x4344: goto L4344;
    case 0x4346: goto L4346;
    case 0x4347: goto L4347;
    case 0x434A: goto L434A;
    case 0x434C: goto L434C;
    case 0x434E: goto L434E;
    case 0x434F: goto L434F;
    case 0x4352: goto L4352;
    case 0x4354: goto L4354;
    case 0x4356: goto L4356;
    case 0x4357: goto L4357;
    case 0x435A: goto L435A;
    case 0x435C: goto L435C;
    case 0x435E: goto L435E;
    case 0x435F: goto L435F;
    case 0x4362: goto L4362;
    case 0x4364: goto L4364;
    case 0x4366: goto L4366;
    case 0x4367: goto L4367;
    case 0x436A: goto L436A;
    case 0x436C: goto L436C;
    case 0x436E: goto L436E;
    case 0x436F: goto L436F;
    case 0x4372: goto L4372;
    case 0x4374: goto L4374;
    case 0x4376: goto L4376;
    case 0x4377: goto L4377;
    case 0x437A: goto L437A;
    case 0x437B: goto L437B;
    case 0x437D: goto L437D;
    case 0x4380: goto L4380;
    case 0x4383: goto L4383;
    case 0x4386: goto L4386;
    case 0x438A: goto L438A;
    case 0x438D: goto L438D;
    case 0x4390: goto L4390;
    case 0x4392: goto L4392;
    case 0x4395: goto L4395;
    case 0x4397: goto L4397;
    case 0x439A: goto L439A;
    case 0x439D: goto L439D;
    case 0x439E: goto L439E;
    case 0x43A4: goto L43A4;
    case 0x43A6: goto L43A6;
    case 0x43AC: goto L43AC;
    case 0x43AE: goto L43AE;
    case 0x43B4: goto L43B4;
    case 0x43B6: goto L43B6;
    case 0x43BC: goto L43BC;
    case 0x43BE: goto L43BE;
    case 0x43C4: goto L43C4;
    case 0x43C6: goto L43C6;
    case 0x43CA: goto L43CA;
    case 0x43CD: goto L43CD;
    case 0x43D1: goto L43D1;
    case 0x43D4: goto L43D4;
    case 0x43D8: goto L43D8;
    case 0x43DA: goto L43DA;
    case 0x43DE: goto L43DE;
    case 0x43E0: goto L43E0;
    case 0x43E4: goto L43E4;
    case 0x43E5: goto L43E5;
    case 0x43E8: goto L43E8;
    case 0x43EC: goto L43EC;
    case 0x43EE: goto L43EE;
    case 0x43F2: goto L43F2;
    case 0x43F4: goto L43F4;
    case 0x43F7: goto L43F7;
    case 0x43FB: goto L43FB;
    case 0x43FD: goto L43FD;
    case 0x4401: goto L4401;
    case 0x4403: goto L4403;
    case 0x4407: goto L4407;
    case 0x440A: goto L440A;
    case 0x440E: goto L440E;
    case 0x440F: goto L440F;
    case 0x4412: goto L4412;
    case 0x4416: goto L4416;
    case 0x4419: goto L4419;
    case 0x441D: goto L441D;
    case 0x441F: goto L441F;
    case 0x4423: goto L4423;
    case 0x4425: goto L4425;
    case 0x4429: goto L4429;
    case 0x442A: goto L442A;
    case 0x442D: goto L442D;
    case 0x4431: goto L4431;
    case 0x4433: goto L4433;
    case 0x4437: goto L4437;
    case 0x4439: goto L4439;
    case 0x443C: goto L443C;
    case 0x4440: goto L4440;
    case 0x4442: goto L4442;
    case 0x4446: goto L4446;
    case 0x4448: goto L4448;
    case 0x444C: goto L444C;
    case 0x4450: goto L4450;
    case 0x4454: goto L4454;
    case 0x4457: goto L4457;
    case 0x445A: goto L445A;
    case 0x445D: goto L445D;
    case 0x4460: goto L4460;
    case 0x4462: goto L4462;
    case 0x4465: goto L4465;
    case 0x4468: goto L4468;
    case 0x446B: goto L446B;
    case 0x446C: goto L446C;
    case 0x446D: goto L446D;
    case 0x446E: goto L446E;
    case 0x4471: goto L4471;
    case 0x4474: goto L4474;
    case 0x4475: goto L4475;
    case 0x4476: goto L4476;
    case 0x4478: goto L4478;
    case 0x4479: goto L4479;
    case 0x447A: goto L447A;
    case 0x447C: goto L447C;
    case 0x447D: goto L447D;
    case 0x4481: goto L4481;
    case 0x4485: goto L4485;
    case 0x4487: goto L4487;
    case 0x4489: goto L4489;
    case 0x448C: goto L448C;
    case 0x448F: goto L448F;
    case 0x4492: goto L4492;
    case 0x4496: goto L4496;
    case 0x4497: goto L4497;
    case 0x4499: goto L4499;
    case 0x449B: goto L449B;
    case 0x449E: goto L449E;
    case 0x44A1: goto L44A1;
    case 0x44A4: goto L44A4;
    case 0x44A5: goto L44A5;
    case 0x44A8: goto L44A8;
    case 0x44AB: goto L44AB;
    case 0x44AE: goto L44AE;
    case 0x44AF: goto L44AF;
    case 0x44B2: goto L44B2;
    case 0x44B5: goto L44B5;
    case 0x44B9: goto L44B9;
    case 0x44BF: goto L44BF;
    case 0x44C2: goto L44C2;
    case 0x44C5: goto L44C5;
    case 0x44C7: goto L44C7;
    case 0x44CA: goto L44CA;
    case 0x44CC: goto L44CC;
    case 0x44CF: goto L44CF;
    case 0x44D1: goto L44D1;
    case 0x44D4: goto L44D4;
    case 0x44D7: goto L44D7;
    case 0x44DA: goto L44DA;
    case 0x44DD: goto L44DD;
    case 0x44E0: goto L44E0;
    case 0x44E1: goto L44E1;
    case 0x44E4: goto L44E4;
    case 0x44E6: goto L44E6;
    case 0x44EA: goto L44EA;
    case 0x44ED: goto L44ED;
    case 0x44EE: goto L44EE;
    case 0x44F0: goto L44F0;
    case 0x44F2: goto L44F2;
    case 0x44F4: goto L44F4;
    case 0x44F8: goto L44F8;
    case 0x44FB: goto L44FB;
    case 0x44FE: goto L44FE;
    case 0x4501: goto L4501;
    case 0x4504: goto L4504;
    case 0x4508: goto L4508;
    case 0x450A: goto L450A;
    case 0x450C: goto L450C;
    case 0x4510: goto L4510;
    case 0x4512: goto L4512;
    case 0x4516: goto L4516;
    case 0x451A: goto L451A;
    case 0x451C: goto L451C;
    case 0x4520: goto L4520;
    case 0x4522: goto L4522;
    case 0x4526: goto L4526;
    case 0x4529: goto L4529;
    case 0x452D: goto L452D;
    case 0x4531: goto L4531;
    case 0x4532: goto L4532;
    case 0x4536: goto L4536;
    case 0x453A: goto L453A;
    case 0x453D: goto L453D;
    case 0x4541: goto L4541;
    case 0x4542: goto L4542;
    case 0x4543: goto L4543;
    case 0x4545: goto L4545;
    case 0x4547: goto L4547;
    case 0x4549: goto L4549;
    case 0x454D: goto L454D;
    case 0x454E: goto L454E;
    case 0x4550: goto L4550;
    case 0x4551: goto L4551;
    case 0x4553: goto L4553;
    case 0x4554: goto L4554;
    case 0x4556: goto L4556;
    case 0x4557: goto L4557;
    case 0x4558: goto L4558;
    case 0x455A: goto L455A;
    case 0x455C: goto L455C;
    case 0x455E: goto L455E;
    case 0x4562: goto L4562;
    case 0x4563: goto L4563;
    case 0x4565: goto L4565;
    case 0x4566: goto L4566;
    case 0x4568: goto L4568;
    case 0x4569: goto L4569;
    case 0x456B: goto L456B;
    case 0x456C: goto L456C;
    case 0x456D: goto L456D;
    case 0x456F: goto L456F;
    case 0x4571: goto L4571;
    case 0x4573: goto L4573;
    case 0x4577: goto L4577;
    case 0x4578: goto L4578;
    case 0x457A: goto L457A;
    case 0x457B: goto L457B;
    case 0x457D: goto L457D;
    case 0x457E: goto L457E;
    case 0x4580: goto L4580;
    case 0x4581: goto L4581;
    case 0x4582: goto L4582;
    case 0x4584: goto L4584;
    case 0x4586: goto L4586;
    case 0x4588: goto L4588;
    case 0x458C: goto L458C;
    case 0x458D: goto L458D;
    case 0x458F: goto L458F;
    case 0x4590: goto L4590;
    case 0x4592: goto L4592;
    case 0x4593: goto L4593;
    case 0x4595: goto L4595;
    case 0x4596: goto L4596;
    case 0x4597: goto L4597;
    case 0x4599: goto L4599;
    case 0x459B: goto L459B;
    case 0x459D: goto L459D;
    case 0x45A1: goto L45A1;
    case 0x45A2: goto L45A2;
    case 0x45A4: goto L45A4;
    case 0x45A5: goto L45A5;
    case 0x45A7: goto L45A7;
    case 0x45A8: goto L45A8;
    case 0x45AA: goto L45AA;
    case 0x45AB: goto L45AB;
    case 0x45AC: goto L45AC;
    case 0x45AE: goto L45AE;
    case 0x45B0: goto L45B0;
    case 0x45B2: goto L45B2;
    case 0x45B6: goto L45B6;
    case 0x45B7: goto L45B7;
    case 0x45B9: goto L45B9;
    case 0x45BA: goto L45BA;
    case 0x45BC: goto L45BC;
    case 0x45BD: goto L45BD;
    case 0x45BF: goto L45BF;
    case 0x45C0: goto L45C0;
    case 0x45C1: goto L45C1;
    case 0x45C3: goto L45C3;
    case 0x45C5: goto L45C5;
    case 0x45C7: goto L45C7;
    case 0x45CB: goto L45CB;
    case 0x45CC: goto L45CC;
    case 0x45CE: goto L45CE;
    case 0x45CF: goto L45CF;
    case 0x45D1: goto L45D1;
    case 0x45D2: goto L45D2;
    case 0x45D4: goto L45D4;
    case 0x45D5: goto L45D5;
    case 0x45D6: goto L45D6;
    case 0x45D8: goto L45D8;
    case 0x45DA: goto L45DA;
    case 0x45DC: goto L45DC;
    case 0x45E0: goto L45E0;
    case 0x45E1: goto L45E1;
    case 0x45E3: goto L45E3;
    case 0x45E4: goto L45E4;
    case 0x45E6: goto L45E6;
    case 0x45E7: goto L45E7;
    case 0x45E9: goto L45E9;
    case 0x45EA: goto L45EA;
    case 0x45EB: goto L45EB;
    case 0x45ED: goto L45ED;
    case 0x45EF: goto L45EF;
    case 0x45F1: goto L45F1;
    case 0x45F5: goto L45F5;
    case 0x45F6: goto L45F6;
    case 0x45F8: goto L45F8;
    case 0x45F9: goto L45F9;
    case 0x45FB: goto L45FB;
    case 0x45FC: goto L45FC;
    case 0x45FE: goto L45FE;
    case 0x45FF: goto L45FF;
    case 0x4600: goto L4600;
    case 0x4602: goto L4602;
    case 0x4604: goto L4604;
    case 0x4606: goto L4606;
    case 0x460A: goto L460A;
    case 0x460B: goto L460B;
    case 0x460D: goto L460D;
    case 0x460E: goto L460E;
    case 0x4610: goto L4610;
    case 0x4611: goto L4611;
    case 0x4613: goto L4613;
    case 0x4614: goto L4614;
    case 0x4615: goto L4615;
    case 0x4617: goto L4617;
    case 0x4619: goto L4619;
    case 0x461B: goto L461B;
    case 0x461F: goto L461F;
    case 0x4620: goto L4620;
    case 0x4622: goto L4622;
    case 0x4623: goto L4623;
    case 0x4625: goto L4625;
    case 0x4626: goto L4626;
    case 0x4628: goto L4628;
    case 0x4629: goto L4629;
    case 0x462A: goto L462A;
    case 0x462C: goto L462C;
    case 0x462E: goto L462E;
    case 0x4630: goto L4630;
    case 0x4634: goto L4634;
    case 0x4635: goto L4635;
    case 0x4637: goto L4637;
    case 0x4638: goto L4638;
    case 0x463A: goto L463A;
    case 0x463B: goto L463B;
    case 0x463D: goto L463D;
    case 0x463E: goto L463E;
    case 0x463F: goto L463F;
    case 0x4641: goto L4641;
    case 0x4643: goto L4643;
    case 0x4645: goto L4645;
    case 0x4649: goto L4649;
    case 0x464A: goto L464A;
    case 0x464C: goto L464C;
    case 0x464D: goto L464D;
    case 0x464F: goto L464F;
    case 0x4650: goto L4650;
    case 0x4652: goto L4652;
    case 0x4653: goto L4653;
    case 0x4654: goto L4654;
    case 0x4656: goto L4656;
    case 0x4658: goto L4658;
    case 0x465A: goto L465A;
    case 0x465E: goto L465E;
    case 0x465F: goto L465F;
    case 0x4661: goto L4661;
    case 0x4662: goto L4662;
    case 0x4664: goto L4664;
    case 0x4665: goto L4665;
    case 0x4667: goto L4667;
    case 0x4668: goto L4668;
    case 0x4669: goto L4669;
    case 0x466B: goto L466B;
    case 0x466D: goto L466D;
    case 0x466F: goto L466F;
    case 0x4673: goto L4673;
    case 0x4674: goto L4674;
    case 0x4676: goto L4676;
    case 0x4677: goto L4677;
    case 0x4679: goto L4679;
    case 0x467A: goto L467A;
    case 0x467C: goto L467C;
    case 0x467D: goto L467D;
    case 0x467E: goto L467E;
    case 0x4680: goto L4680;
    case 0x4682: goto L4682;
    case 0x4684: goto L4684;
    case 0x4688: goto L4688;
    case 0x4689: goto L4689;
    case 0x468B: goto L468B;
    case 0x468C: goto L468C;
    case 0x468E: goto L468E;
    case 0x468F: goto L468F;
    case 0x4691: goto L4691;
    case 0x4692: goto L4692;
    case 0x4693: goto L4693;
    case 0x4695: goto L4695;
    case 0x4697: goto L4697;
    case 0x4699: goto L4699;
    case 0x469D: goto L469D;
    case 0x469E: goto L469E;
    case 0x46A0: goto L46A0;
    case 0x46A1: goto L46A1;
    case 0x46A3: goto L46A3;
    case 0x46A4: goto L46A4;
    case 0x46A6: goto L46A6;
    case 0x46A7: goto L46A7;
    case 0x46A8: goto L46A8;
    case 0x46AA: goto L46AA;
    case 0x46AC: goto L46AC;
    case 0x46AE: goto L46AE;
    case 0x46B2: goto L46B2;
    case 0x46B3: goto L46B3;
    case 0x46B5: goto L46B5;
    case 0x46B6: goto L46B6;
    case 0x46B8: goto L46B8;
    case 0x46B9: goto L46B9;
    case 0x46BB: goto L46BB;
    case 0x46BC: goto L46BC;
    case 0x46BD: goto L46BD;
    case 0x46BF: goto L46BF;
    case 0x46C1: goto L46C1;
    case 0x46C3: goto L46C3;
    case 0x46C7: goto L46C7;
    case 0x46C8: goto L46C8;
    case 0x46CA: goto L46CA;
    case 0x46CB: goto L46CB;
    case 0x46CD: goto L46CD;
    case 0x46CE: goto L46CE;
    case 0x46D0: goto L46D0;
    case 0x46D1: goto L46D1;
    case 0x46D2: goto L46D2;
    case 0x46D4: goto L46D4;
    case 0x46D6: goto L46D6;
    case 0x46D8: goto L46D8;
    case 0x46DC: goto L46DC;
    case 0x46DD: goto L46DD;
    case 0x46DF: goto L46DF;
    case 0x46E0: goto L46E0;
    case 0x46E2: goto L46E2;
    case 0x46E3: goto L46E3;
    case 0x46E5: goto L46E5;
    case 0x46E6: goto L46E6;
    case 0x46E7: goto L46E7;
    case 0x46E9: goto L46E9;
    case 0x46EB: goto L46EB;
    case 0x46ED: goto L46ED;
    case 0x46F1: goto L46F1;
    case 0x46F2: goto L46F2;
    case 0x46F4: goto L46F4;
    case 0x46F5: goto L46F5;
    case 0x46F7: goto L46F7;
    case 0x46F8: goto L46F8;
    case 0x46FA: goto L46FA;
    case 0x46FB: goto L46FB;
    case 0x46FC: goto L46FC;
    case 0x46FE: goto L46FE;
    case 0x4700: goto L4700;
    case 0x4702: goto L4702;
    case 0x4706: goto L4706;
    case 0x4707: goto L4707;
    case 0x4709: goto L4709;
    case 0x470A: goto L470A;
    case 0x470C: goto L470C;
    case 0x470D: goto L470D;
    case 0x470F: goto L470F;
    case 0x4710: goto L4710;
    case 0x4711: goto L4711;
    case 0x4713: goto L4713;
    case 0x4715: goto L4715;
    case 0x4717: goto L4717;
    case 0x471B: goto L471B;
    case 0x471C: goto L471C;
    case 0x471E: goto L471E;
    case 0x471F: goto L471F;
    case 0x4721: goto L4721;
    case 0x4722: goto L4722;
    case 0x4724: goto L4724;
    case 0x4725: goto L4725;
    case 0x4726: goto L4726;
    case 0x4728: goto L4728;
    case 0x472A: goto L472A;
    case 0x472C: goto L472C;
    case 0x4730: goto L4730;
    case 0x4731: goto L4731;
    case 0x4733: goto L4733;
    case 0x4734: goto L4734;
    case 0x4736: goto L4736;
    case 0x4737: goto L4737;
    case 0x4739: goto L4739;
    case 0x473A: goto L473A;
    case 0x473B: goto L473B;
    case 0x473D: goto L473D;
    case 0x473F: goto L473F;
    case 0x4741: goto L4741;
    case 0x4745: goto L4745;
    case 0x4746: goto L4746;
    case 0x4748: goto L4748;
    case 0x4749: goto L4749;
    case 0x474B: goto L474B;
    case 0x474C: goto L474C;
    case 0x474E: goto L474E;
    case 0x474F: goto L474F;
    case 0x4750: goto L4750;
    case 0x4752: goto L4752;
    case 0x4754: goto L4754;
    case 0x4756: goto L4756;
    case 0x475A: goto L475A;
    case 0x475B: goto L475B;
    case 0x475D: goto L475D;
    case 0x475E: goto L475E;
    case 0x4760: goto L4760;
    case 0x4761: goto L4761;
    case 0x4763: goto L4763;
    case 0x4764: goto L4764;
    case 0x4765: goto L4765;
    case 0x4767: goto L4767;
    case 0x4769: goto L4769;
    case 0x476B: goto L476B;
    case 0x476F: goto L476F;
    case 0x4770: goto L4770;
    case 0x4772: goto L4772;
    case 0x4773: goto L4773;
    case 0x4775: goto L4775;
    case 0x4776: goto L4776;
    case 0x4778: goto L4778;
    case 0x4779: goto L4779;
    case 0x477A: goto L477A;
    case 0x477C: goto L477C;
    case 0x477E: goto L477E;
    case 0x4780: goto L4780;
    case 0x4784: goto L4784;
    case 0x4785: goto L4785;
    case 0x4787: goto L4787;
    case 0x4788: goto L4788;
    case 0x478A: goto L478A;
    case 0x478B: goto L478B;
    case 0x478D: goto L478D;
    case 0x478E: goto L478E;
    case 0x478F: goto L478F;
    case 0x4791: goto L4791;
    case 0x4793: goto L4793;
    case 0x4795: goto L4795;
    case 0x4799: goto L4799;
    case 0x479A: goto L479A;
    case 0x479C: goto L479C;
    case 0x479D: goto L479D;
    case 0x479F: goto L479F;
    case 0x47A0: goto L47A0;
    case 0x47A2: goto L47A2;
    case 0x47A3: goto L47A3;
    case 0x47A4: goto L47A4;
    case 0x47A6: goto L47A6;
    case 0x47A8: goto L47A8;
    case 0x47AA: goto L47AA;
    case 0x47AE: goto L47AE;
    case 0x47AF: goto L47AF;
    case 0x47B1: goto L47B1;
    case 0x47B2: goto L47B2;
    case 0x47B4: goto L47B4;
    case 0x47B5: goto L47B5;
    case 0x47B7: goto L47B7;
    case 0x47B8: goto L47B8;
    case 0x47B9: goto L47B9;
    case 0x47BB: goto L47BB;
    case 0x47BD: goto L47BD;
    case 0x47BF: goto L47BF;
    case 0x47C3: goto L47C3;
    case 0x47C4: goto L47C4;
    case 0x47C6: goto L47C6;
    case 0x47C7: goto L47C7;
    case 0x47C9: goto L47C9;
    case 0x47CA: goto L47CA;
    case 0x47CC: goto L47CC;
    case 0x47CD: goto L47CD;
    case 0x47CE: goto L47CE;
    case 0x47D0: goto L47D0;
    case 0x47D2: goto L47D2;
    case 0x47D4: goto L47D4;
    case 0x47D8: goto L47D8;
    case 0x47D9: goto L47D9;
    case 0x47DB: goto L47DB;
    case 0x47DC: goto L47DC;
    case 0x47DE: goto L47DE;
    case 0x47DF: goto L47DF;
    case 0x47E1: goto L47E1;
    case 0x47E2: goto L47E2;
    case 0x47E3: goto L47E3;
    case 0x47E5: goto L47E5;
    case 0x47E7: goto L47E7;
    case 0x47E9: goto L47E9;
    case 0x47ED: goto L47ED;
    case 0x47EE: goto L47EE;
    case 0x47F0: goto L47F0;
    case 0x47F1: goto L47F1;
    case 0x47F3: goto L47F3;
    case 0x47F4: goto L47F4;
    case 0x47F6: goto L47F6;
    case 0x47F7: goto L47F7;
    case 0x47F8: goto L47F8;
    case 0x47FA: goto L47FA;
    case 0x47FC: goto L47FC;
    case 0x47FE: goto L47FE;
    case 0x4802: goto L4802;
    case 0x4803: goto L4803;
    case 0x4805: goto L4805;
    case 0x4806: goto L4806;
    case 0x4808: goto L4808;
    case 0x4809: goto L4809;
    case 0x480B: goto L480B;
    case 0x480C: goto L480C;
    case 0x480D: goto L480D;
    case 0x480F: goto L480F;
    case 0x4811: goto L4811;
    case 0x4813: goto L4813;
    case 0x4817: goto L4817;
    case 0x4818: goto L4818;
    case 0x481A: goto L481A;
    case 0x481B: goto L481B;
    case 0x481D: goto L481D;
    case 0x481E: goto L481E;
    case 0x4820: goto L4820;
    case 0x4821: goto L4821;
    case 0x4822: goto L4822;
    case 0x4824: goto L4824;
    case 0x4826: goto L4826;
    case 0x4828: goto L4828;
    case 0x482C: goto L482C;
    case 0x482D: goto L482D;
    case 0x482F: goto L482F;
    case 0x4830: goto L4830;
    case 0x4832: goto L4832;
    case 0x4833: goto L4833;
    case 0x4835: goto L4835;
    case 0x4836: goto L4836;
    case 0x4837: goto L4837;
    case 0x4839: goto L4839;
    case 0x483B: goto L483B;
    case 0x483D: goto L483D;
    case 0x4841: goto L4841;
    case 0x4842: goto L4842;
    case 0x4844: goto L4844;
    case 0x4845: goto L4845;
    case 0x4847: goto L4847;
    case 0x4848: goto L4848;
    case 0x484A: goto L484A;
    case 0x484B: goto L484B;
    case 0x484C: goto L484C;
    case 0x484E: goto L484E;
    case 0x4850: goto L4850;
    case 0x4852: goto L4852;
    case 0x4856: goto L4856;
    case 0x4857: goto L4857;
    case 0x4859: goto L4859;
    case 0x485A: goto L485A;
    case 0x485C: goto L485C;
    case 0x485D: goto L485D;
    case 0x485F: goto L485F;
    case 0x4860: goto L4860;
    case 0x4861: goto L4861;
    case 0x4863: goto L4863;
    case 0x4865: goto L4865;
    case 0x4867: goto L4867;
    case 0x486B: goto L486B;
    case 0x486C: goto L486C;
    case 0x486E: goto L486E;
    case 0x486F: goto L486F;
    case 0x4871: goto L4871;
    case 0x4872: goto L4872;
    case 0x4874: goto L4874;
    case 0x4875: goto L4875;
    case 0x4876: goto L4876;
    case 0x4878: goto L4878;
    case 0x487A: goto L487A;
    case 0x487C: goto L487C;
    case 0x4880: goto L4880;
    case 0x4881: goto L4881;
    case 0x4883: goto L4883;
    case 0x4884: goto L4884;
    case 0x4886: goto L4886;
    case 0x4887: goto L4887;
    case 0x4889: goto L4889;
    case 0x488A: goto L488A;
    case 0x488B: goto L488B;
    case 0x488D: goto L488D;
    case 0x488F: goto L488F;
    case 0x4891: goto L4891;
    case 0x4895: goto L4895;
    case 0x4896: goto L4896;
    case 0x4898: goto L4898;
    case 0x4899: goto L4899;
    case 0x489B: goto L489B;
    case 0x489C: goto L489C;
    case 0x489E: goto L489E;
    case 0x489F: goto L489F;
    case 0x48A0: goto L48A0;
    case 0x48A2: goto L48A2;
    case 0x48A4: goto L48A4;
    case 0x48A6: goto L48A6;
    case 0x48AA: goto L48AA;
    case 0x48AB: goto L48AB;
    case 0x48AD: goto L48AD;
    case 0x48AE: goto L48AE;
    case 0x48B0: goto L48B0;
    case 0x48B1: goto L48B1;
    case 0x48B3: goto L48B3;
    case 0x48B4: goto L48B4;
    case 0x48B5: goto L48B5;
    case 0x48B7: goto L48B7;
    case 0x48B9: goto L48B9;
    case 0x48BB: goto L48BB;
    case 0x48BF: goto L48BF;
    case 0x48C0: goto L48C0;
    case 0x48C2: goto L48C2;
    case 0x48C3: goto L48C3;
    case 0x48C5: goto L48C5;
    case 0x48C6: goto L48C6;
    case 0x48C8: goto L48C8;
    case 0x48C9: goto L48C9;
    case 0x48CA: goto L48CA;
    case 0x48CC: goto L48CC;
    case 0x48CE: goto L48CE;
    case 0x48D0: goto L48D0;
    case 0x48D4: goto L48D4;
    case 0x48D5: goto L48D5;
    case 0x48D7: goto L48D7;
    case 0x48D8: goto L48D8;
    case 0x48DA: goto L48DA;
    case 0x48DB: goto L48DB;
    case 0x48DD: goto L48DD;
    case 0x48DE: goto L48DE;
    case 0x48DF: goto L48DF;
    case 0x48E1: goto L48E1;
    case 0x48E3: goto L48E3;
    case 0x48E5: goto L48E5;
    case 0x48E9: goto L48E9;
    case 0x48EA: goto L48EA;
    case 0x48EC: goto L48EC;
    case 0x48ED: goto L48ED;
    case 0x48EF: goto L48EF;
    case 0x48F0: goto L48F0;
    case 0x48F2: goto L48F2;
    case 0x48F3: goto L48F3;
    case 0x48F4: goto L48F4;
    case 0x48F6: goto L48F6;
    case 0x48F8: goto L48F8;
    case 0x48FA: goto L48FA;
    case 0x48FE: goto L48FE;
    case 0x48FF: goto L48FF;
    case 0x4901: goto L4901;
    case 0x4902: goto L4902;
    case 0x4904: goto L4904;
    case 0x4905: goto L4905;
    case 0x4907: goto L4907;
    case 0x4908: goto L4908;
    case 0x4909: goto L4909;
    case 0x490B: goto L490B;
    case 0x490D: goto L490D;
    case 0x490F: goto L490F;
    case 0x4913: goto L4913;
    case 0x4914: goto L4914;
    case 0x4916: goto L4916;
    case 0x4917: goto L4917;
    case 0x4919: goto L4919;
    case 0x491A: goto L491A;
    case 0x491C: goto L491C;
    case 0x491D: goto L491D;
    case 0x491E: goto L491E;
    case 0x4920: goto L4920;
    case 0x4922: goto L4922;
    case 0x4924: goto L4924;
    case 0x4928: goto L4928;
    case 0x4929: goto L4929;
    case 0x492B: goto L492B;
    case 0x492C: goto L492C;
    case 0x492E: goto L492E;
    case 0x492F: goto L492F;
    case 0x4931: goto L4931;
    case 0x4932: goto L4932;
    case 0x4933: goto L4933;
    case 0x4935: goto L4935;
    case 0x4937: goto L4937;
    case 0x4939: goto L4939;
    case 0x493D: goto L493D;
    case 0x493E: goto L493E;
    case 0x4940: goto L4940;
    case 0x4941: goto L4941;
    case 0x4943: goto L4943;
    case 0x4944: goto L4944;
    case 0x4946: goto L4946;
    case 0x4947: goto L4947;
    case 0x4948: goto L4948;
    case 0x494A: goto L494A;
    case 0x494C: goto L494C;
    case 0x494E: goto L494E;
    case 0x4952: goto L4952;
    case 0x4953: goto L4953;
    case 0x4955: goto L4955;
    case 0x4956: goto L4956;
    case 0x4958: goto L4958;
    case 0x4959: goto L4959;
    case 0x495B: goto L495B;
    case 0x495C: goto L495C;
    case 0x495D: goto L495D;
    case 0x495F: goto L495F;
    case 0x4961: goto L4961;
    case 0x4963: goto L4963;
    case 0x4967: goto L4967;
    case 0x4968: goto L4968;
    case 0x496A: goto L496A;
    case 0x496B: goto L496B;
    case 0x496D: goto L496D;
    case 0x496E: goto L496E;
    case 0x4970: goto L4970;
    case 0x4971: goto L4971;
    case 0x4972: goto L4972;
    case 0x4974: goto L4974;
    case 0x4976: goto L4976;
    case 0x4978: goto L4978;
    case 0x497C: goto L497C;
    case 0x497D: goto L497D;
    case 0x497F: goto L497F;
    case 0x4980: goto L4980;
    case 0x4982: goto L4982;
    case 0x4983: goto L4983;
    case 0x4985: goto L4985;
    case 0x4986: goto L4986;
    case 0x4987: goto L4987;
    case 0x4989: goto L4989;
    case 0x498B: goto L498B;
    case 0x498D: goto L498D;
    case 0x4991: goto L4991;
    case 0x4992: goto L4992;
    case 0x4994: goto L4994;
    case 0x4995: goto L4995;
    case 0x4997: goto L4997;
    case 0x4998: goto L4998;
    case 0x499A: goto L499A;
    case 0x499B: goto L499B;
    case 0x499D: goto L499D;
    case 0x499F: goto L499F;
    case 0x49A1: goto L49A1;
    case 0x49A4: goto L49A4;
    case 0x49A5: goto L49A5;
    case 0x49A6: goto L49A6;
    case 0x49A9: goto L49A9;
    case 0x49AB: goto L49AB;
    case 0x49AF: goto L49AF;
    case 0x49B2: goto L49B2;
    case 0x49B6: goto L49B6;
    case 0x49BA: goto L49BA;
    case 0x49BE: goto L49BE;
    case 0x49C1: goto L49C1;
    case 0x49C2: goto L49C2;
    case 0x49C5: goto L49C5;
    case 0x49C6: goto L49C6;
    case 0x49C8: goto L49C8;
    case 0x49CA: goto L49CA;
    case 0x49CC: goto L49CC;
    case 0x49CE: goto L49CE;
    case 0x49D0: goto L49D0;
    case 0x49D1: goto L49D1;
    case 0x49D2: goto L49D2;
    case 0x49D8: goto L49D8;
    case 0x49DA: goto L49DA;
    case 0x49DE: goto L49DE;
    case 0x49E3: goto L49E3;
    case 0x49E7: goto L49E7;
    case 0x49EB: goto L49EB;
    case 0x49EE: goto L49EE;
    case 0x49F2: goto L49F2;
    case 0x49F3: goto L49F3;
    case 0x49F7: goto L49F7;
    case 0x49F9: goto L49F9;
    case 0x49FB: goto L49FB;
    case 0x49FD: goto L49FD;
    case 0x49FF: goto L49FF;
    case 0x4A01: goto L4A01;
    case 0x4A02: goto L4A02;
    case 0x4A06: goto L4A06;
    case 0x4A08: goto L4A08;
    case 0x4A0B: goto L4A0B;
    case 0x4A0E: goto L4A0E;
    case 0x4A11: goto L4A11;
    case 0x4A14: goto L4A14;
    case 0x4A17: goto L4A17;
    case 0x4A1A: goto L4A1A;
    case 0x4A1D: goto L4A1D;
    case 0x4A20: goto L4A20;
    case 0x4A23: goto L4A23;
    case 0x4A26: goto L4A26;
    case 0x4A29: goto L4A29;
    case 0x4A2C: goto L4A2C;
    case 0x4A2F: goto L4A2F;
    case 0x4A32: goto L4A32;
    case 0x4A35: goto L4A35;
    case 0x4A38: goto L4A38;
    case 0x4A3B: goto L4A3B;
    case 0x4A3E: goto L4A3E;
    case 0x4A41: goto L4A41;
    case 0x4A44: goto L4A44;
    case 0x4A47: goto L4A47;
    case 0x4A4A: goto L4A4A;
    case 0x4A4D: goto L4A4D;
    case 0x4A50: goto L4A50;
    case 0x4A53: goto L4A53;
    case 0x4A56: goto L4A56;
    case 0x4A59: goto L4A59;
    case 0x4A5C: goto L4A5C;
    case 0x4A5F: goto L4A5F;
    case 0x4A62: goto L4A62;
    case 0x4A65: goto L4A65;
    case 0x4A68: goto L4A68;
    case 0x4A6B: goto L4A6B;
    case 0x4A6E: goto L4A6E;
    case 0x4A71: goto L4A71;
    case 0x4A74: goto L4A74;
    case 0x4A77: goto L4A77;
    case 0x4A7A: goto L4A7A;
    case 0x4A7D: goto L4A7D;
    case 0x4A80: goto L4A80;
    case 0x4A83: goto L4A83;
    case 0x4A86: goto L4A86;
    case 0x4A89: goto L4A89;
    case 0x4A8C: goto L4A8C;
    case 0x4A8F: goto L4A8F;
    case 0x4A92: goto L4A92;
    case 0x4A95: goto L4A95;
    case 0x4A98: goto L4A98;
    case 0x4A9B: goto L4A9B;
    case 0x4A9E: goto L4A9E;
    case 0x4AA1: goto L4AA1;
    case 0x4AA4: goto L4AA4;
    case 0x4AA5: goto L4AA5;
    case 0x4AA7: goto L4AA7;
    case 0x4AAA: goto L4AAA;
    case 0x4AAC: goto L4AAC;
    case 0x4AAF: goto L4AAF;
    case 0x4AB2: goto L4AB2;
    case 0x4AB4: goto L4AB4;
    case 0x4AB6: goto L4AB6;
    case 0x4AB8: goto L4AB8;
    case 0x4ABA: goto L4ABA;
    case 0x4ABC: goto L4ABC;
    case 0x4ABD: goto L4ABD;
    case 0x4AC1: goto L4AC1;
    case 0x4AC3: goto L4AC3;
    case 0x4AC6: goto L4AC6;
    case 0x4AC8: goto L4AC8;
    case 0x4ACA: goto L4ACA;
    case 0x4ACB: goto L4ACB;
    case 0x4ACF: goto L4ACF;
    case 0x4AD2: goto L4AD2;
    case 0x4AD4: goto L4AD4;
    case 0x4AD5: goto L4AD5;
    case 0x4AD9: goto L4AD9;
    case 0x4ADB: goto L4ADB;
    case 0x4ADE: goto L4ADE;
    case 0x4AE0: goto L4AE0;
    case 0x4AE1: goto L4AE1;
    case 0x4AE3: goto L4AE3;
    case 0x4AE5: goto L4AE5;
    case 0x4AE9: goto L4AE9;
    case 0x4AEB: goto L4AEB;
    case 0x4AF0: goto L4AF0;
    case 0x4AF3: goto L4AF3;
    case 0x4AF5: goto L4AF5;
    case 0x4AF9: goto L4AF9;
    case 0x4AFC: goto L4AFC;
    case 0x4AFE: goto L4AFE;
    case 0x4B00: goto L4B00;
    case 0x4B02: goto L4B02;
    case 0x4B05: goto L4B05;
    case 0x4B07: goto L4B07;
    case 0x4B09: goto L4B09;
    case 0x4B0B: goto L4B0B;
    case 0x4B0D: goto L4B0D;
    case 0x4B0F: goto L4B0F;
    case 0x4B11: goto L4B11;
    case 0x4B13: goto L4B13;
    case 0x4B16: goto L4B16;
    case 0x4B18: goto L4B18;
    case 0x4B19: goto L4B19;
    case 0x4B1C: goto L4B1C;
    case 0x4B1E: goto L4B1E;
    case 0x4B20: goto L4B20;
    case 0x4B21: goto L4B21;
    case 0x4B23: goto L4B23;
    case 0x4B25: goto L4B25;
    case 0x4B27: goto L4B27;
    case 0x4B29: goto L4B29;
    case 0x4B2C: goto L4B2C;
    case 0x4B2E: goto L4B2E;
    case 0x4B30: goto L4B30;
    case 0x4B32: goto L4B32;
    case 0x4B34: goto L4B34;
    case 0x4B36: goto L4B36;
    case 0x4B38: goto L4B38;
    case 0x4B3A: goto L4B3A;
    case 0x4B3E: goto L4B3E;
    case 0x4B41: goto L4B41;
    case 0x4B43: goto L4B43;
    case 0x4B44: goto L4B44;
    case 0x4B46: goto L4B46;
    case 0x4B4A: goto L4B4A;
    case 0x4B4C: goto L4B4C;
    case 0x4B4D: goto L4B4D;
    case 0x4B51: goto L4B51;
    case 0x4B53: goto L4B53;
    case 0x4B55: goto L4B55;
    case 0x4B57: goto L4B57;
    case 0x4B59: goto L4B59;
    case 0x4B5A: goto L4B5A;
    case 0x4B5E: goto L4B5E;
    case 0x4B60: goto L4B60;
    case 0x4B62: goto L4B62;
    case 0x4B64: goto L4B64;
    case 0x4B66: goto L4B66;
    case 0x4B67: goto L4B67;
    case 0x4B6B: goto L4B6B;
    case 0x4B6D: goto L4B6D;
    case 0x4B6F: goto L4B6F;
    case 0x4B71: goto L4B71;
    case 0x4B73: goto L4B73;
    case 0x4B74: goto L4B74;
    case 0x4B78: goto L4B78;
    case 0x4B7A: goto L4B7A;
    case 0x4B7C: goto L4B7C;
    case 0x4B7E: goto L4B7E;
    case 0x4B80: goto L4B80;
    case 0x4B81: goto L4B81;
    case 0x4B85: goto L4B85;
    case 0x4B87: goto L4B87;
    case 0x4B89: goto L4B89;
    case 0x4B8B: goto L4B8B;
    case 0x4B8D: goto L4B8D;
    case 0x4B8E: goto L4B8E;
    case 0x4B92: goto L4B92;
    case 0x4B94: goto L4B94;
    case 0x4B96: goto L4B96;
    case 0x4B98: goto L4B98;
    case 0x4B9A: goto L4B9A;
    case 0x4B9B: goto L4B9B;
    case 0x4B9F: goto L4B9F;
    case 0x4BA1: goto L4BA1;
    case 0x4BA3: goto L4BA3;
    case 0x4BA5: goto L4BA5;
    case 0x4BA7: goto L4BA7;
    case 0x4BA8: goto L4BA8;
    case 0x4BAC: goto L4BAC;
    case 0x4BAE: goto L4BAE;
    case 0x4BB0: goto L4BB0;
    case 0x4BB2: goto L4BB2;
    case 0x4BB4: goto L4BB4;
    case 0x4BB5: goto L4BB5;
    case 0x4BB9: goto L4BB9;
    case 0x4BBB: goto L4BBB;
    case 0x4BBD: goto L4BBD;
    case 0x4BBF: goto L4BBF;
    case 0x4BC1: goto L4BC1;
    case 0x4BC2: goto L4BC2;
    case 0x4BC6: goto L4BC6;
    case 0x4BC8: goto L4BC8;
    case 0x4BCA: goto L4BCA;
    case 0x4BCC: goto L4BCC;
    case 0x4BCE: goto L4BCE;
    case 0x4BCF: goto L4BCF;
    case 0x4BD3: goto L4BD3;
    case 0x4BD5: goto L4BD5;
    case 0x4BD7: goto L4BD7;
    case 0x4BD9: goto L4BD9;
    case 0x4BDB: goto L4BDB;
    case 0x4BDC: goto L4BDC;
    case 0x4BE0: goto L4BE0;
    case 0x4BE2: goto L4BE2;
    case 0x4BE4: goto L4BE4;
    case 0x4BE6: goto L4BE6;
    case 0x4BE8: goto L4BE8;
    case 0x4BE9: goto L4BE9;
    case 0x4BED: goto L4BED;
    case 0x4BEF: goto L4BEF;
    case 0x4BF1: goto L4BF1;
    case 0x4BF3: goto L4BF3;
    case 0x4BF5: goto L4BF5;
    case 0x4BF6: goto L4BF6;
    case 0x4BFA: goto L4BFA;
    case 0x4BFC: goto L4BFC;
    case 0x4BFE: goto L4BFE;
    case 0x4C00: goto L4C00;
    case 0x4C02: goto L4C02;
    case 0x4C03: goto L4C03;
    case 0x4C07: goto L4C07;
    case 0x4C09: goto L4C09;
    case 0x4C0B: goto L4C0B;
    case 0x4C0D: goto L4C0D;
    case 0x4C0F: goto L4C0F;
    case 0x4C10: goto L4C10;
    case 0x4C14: goto L4C14;
    case 0x4C16: goto L4C16;
    case 0x4C18: goto L4C18;
    case 0x4C1A: goto L4C1A;
    case 0x4C1D: goto L4C1D;
    case 0x4C1F: goto L4C1F;
    case 0x4C20: goto L4C20;
    case 0x4C21: goto L4C21;
    case 0x4C24: goto L4C24;
    case 0x4C45: goto L4C45;
    case 0x4C49: goto L4C49;
    case 0x4C4D: goto L4C4D;
    case 0x4C4F: goto L4C4F;
    case 0x4C52: goto L4C52;
    case 0x4C54: goto L4C54;
    case 0x4C56: goto L4C56;
    case 0x4C57: goto L4C57;
    case 0x4C59: goto L4C59;
    case 0x4C5B: goto L4C5B;
    case 0x4C5D: goto L4C5D;
    case 0x4C5F: goto L4C5F;
    case 0x4C63: goto L4C63;
    case 0x4C65: goto L4C65;
    case 0x4C68: goto L4C68;
    case 0x4C6A: goto L4C6A;
    case 0x4C6C: goto L4C6C;
    case 0x4C6E: goto L4C6E;
    default: asm_bad_entry("GRLIBI.ASM", entry);
    }

    /* seg003_4134  (+4134)
       upolygon: fill a polygon already inside the window. In: CX = the vertex count, the vertices
       (x, y words) from 415C. Finds the top vertex, steps the two chains (_41DC going one way round,
       _4209 the other) to fill the row table, and jumps to the span writer. A polygon of zero height
       becomes one span from its least to its greatest x. */
L4134: /* _seg003_4134 */
    /* 4134  mov     si,word ptr ds:[49AAh] */
    SI = rw(pDS, 0x49AA);
L4138:
    /* 4138  and     word ptr [si],7FFFh */
    ww(pDS, SI, (uint16_t)(rw(pDS, SI) & 0x7FFF));
L413C:
    /* 413C  mov     word ptr ds:[49B6h],cx */
    ww(pDS, 0x49B6, CX);
L4140:
    /* 4140  mov     word ptr ds:[49B8h],cx */
    ww(pDS, 0x49B8, CX);
L4144:
    /* 4144  mov     word ptr ds:[49BAh],cx */
    ww(pDS, 0x49BA, CX);
L4148:
    /* 4148  mov     di,0FC19h */
    DI = 0xFC19;
L414B:
    /* 414B  mov     si,415Eh */
    SI = 0x415E;
L414E:
    /* 414E  mov     bp,415Ch */
    BP = 0x415C;
L4151:
    /* 4151  jmp     short L415C */
    goto L415C;
L4153: /* L4153 */
    /* 4153  mov     bp,si */
    BP = SI;
L4155:
    /* 4155  sub     bp,6 */
    BP = (uint16_t)(BP - 0x6);
L4158:
    /* 4158  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L4159:
    /* 4159  dec     cx */
    CX = dec16(CX);
L415A:
    /* 415A  je      L4165 */
    if (ZF) goto L4165;
L415C: /* L415C */
    /* 415C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L415D:
    /* 415D  inc     si */
    SI = (uint16_t)(SI + 1);
L415E:
    /* 415E  inc     si */
    SI = (uint16_t)(SI + 1);
L415F:
    /* 415F  cmp     di,ax */
    sub16(DI, AX, 0);
L4161:
    /* 4161  jl      L4153 */
    if (SF != OF) goto L4153;
L4163:
    /* 4163  loop    L415C */
    if (--CX) goto L415C;
L4165: /* L4165 */
    /* 4165  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L4168:
    /* 4168  mov     word ptr ds:[49C0h],si */
    ww(pDS, 0x49C0, SI);
L416C:
    /* 416C  mov     si,bp */
    SI = BP;
L416E:
    /* 416E  mov     word ptr ds:[49BEh],si */
    ww(pDS, 0x49BE, SI);
L4172:
    /* 4172  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L4174:
    /* 4174  mov     ax,di */
    AX = DI;
L4176:
    /* 4176  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L4178:
    /* 4178  add     di,ax */
    DI = (uint16_t)(DI + AX);
L417A:
    /* 417A  neg     di */
    DI = (uint16_t)-DI;
L417C:
    /* 417C  add     di,4976h */
    DI = (uint16_t)(DI + 0x4976);
L4180:
    /* 4180  mov     word ptr ds:[49BCh],di */
    ww(pDS, 0x49BC, DI);
L4184:
    /* 4184  call    _seg003_41DC */
    if ((c = asm_call(ASM_JMP(0x0090, 0x41DC), 0x4187)) != 0) return c;
L4187:
    /* 4187  mov     si,word ptr ds:[49BEh] */
    SI = rw(pDS, 0x49BE);
L418B:
    /* 418B  mov     di,word ptr ds:[49BCh] */
    DI = rw(pDS, 0x49BC);
L418F:
    /* 418F  call    _seg003_4209 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4209), 0x4192)) != 0) return c;
L4192:
    /* 4192  sub     di,4 */
    DI = (uint16_t)(DI - 0x4);
L4195:
    /* 4195  mov     bp,word ptr ds:[49BCh] */
    BP = rw(pDS, 0x49BC);
L4199:
    /* 4199  mov     ax,word ptr ds:[4A06h] */
    AX = rw(pDS, 0x4A06);
L419C:
    /* 419C  mov     bx,word ptr ds:[4A08h] */
    BX = rw(pDS, 0x4A08);
L41A0:
    /* 41A0  cmp     bp,di */
    sub16(BP, DI, 0);
L41A2:
    /* 41A2  jne     L41C4 */
    if (!ZF) goto L41C4;
L41A4:
    /* 41A4  mov     si,415Ch */
    SI = 0x415C;
L41A7:
    /* 41A7  mov     bx,270Fh */
    BX = 0x270F;
L41AA:
    /* 41AA  mov     dx,0D8F1h */
    DX = 0xD8F1;
L41AD:
    /* 41AD  mov     cx,word ptr ds:[49B6h] */
    CX = rw(pDS, 0x49B6);
L41B1: /* L41B1 */
    /* 41B1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L41B2:
    /* 41B2  cmp     ax,bx */
    sub16(AX, BX, 0);
L41B4:
    /* 41B4  jg      L41B8 */
    if (!ZF && SF == OF) goto L41B8;
L41B6:
    /* 41B6  mov     bx,ax */
    BX = AX;
L41B8: /* L41B8 */
    /* 41B8  cmp     ax,dx */
    sub16(AX, DX, 0);
L41BA:
    /* 41BA  jl      L41BE */
    if (SF != OF) goto L41BE;
L41BC:
    /* 41BC  mov     dx,ax */
    DX = AX;
L41BE: /* L41BE */
    /* 41BE  inc     si */
    SI = (uint16_t)(SI + 1);
L41BF:
    /* 41BF  inc     si */
    SI = (uint16_t)(SI + 1);
L41C0:
    /* 41C0  loop    L41B1 */
    if (--CX) goto L41B1;
L41C2:
    /* 41C2  mov     ax,dx */
    AX = DX;
L41C4: /* L41C4 */
    /* 41C4  mov     si,bp */
    SI = BP;
L41C6:
    /* 41C6  inc     di */
    DI = (uint16_t)(DI + 1);
L41C7:
    /* 41C7  inc     di */
    DI = (uint16_t)(DI + 1);
L41C8:
    /* 41C8  cmp     ax,bx */
    sub16(AX, BX, 0);
L41CA:
    /* 41CA  jle     L41CD */
    if (ZF || SF != OF) goto L41CD;
L41CC:
    /* 41CC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L41CD: /* L41CD */
    /* 41CD  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L41CE:
    /* 41CE  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L41CF:
    /* 41CF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L41D0:
    /* 41D0  mov     word ptr ds:[49AAh],di */
    ww(pDS, 0x49AA, DI);
L41D4:
    /* 41D4  or      word ptr [di],8000h */
    ww(pDS, DI, logic16((uint16_t)(rw(pDS, DI) | 0x8000)));
L41D8:
    /* 41D8  jmp     word ptr ds:[SPAN_WRITER] */
    return ASM_JMP(0x0090, rw(pDS, 0x4110));

    /* seg003_41DC  (+41DC)
       _41DC: walk the chain from the top vertex backwards through the vertex list, filling one side
       of the row records (through _423B) until an edge turns upwards. */
L41DC: /* _seg003_41DC */
    /* 41DC  inc     di */
    DI = (uint16_t)(DI + 1);
L41DD:
    /* 41DD  inc     di */
    DI = (uint16_t)(DI + 1);
L41DE:
    /* 41DE  inc     si */
    SI = (uint16_t)(SI + 1);
L41DF:
    /* 41DF  inc     si */
    SI = (uint16_t)(SI + 1);
L41E0: /* L41E0 */
    /* 41E0  std */
    DF = 1;
L41E1:
    /* 41E1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L41E2:
    /* 41E2  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L41E3:
    /* 41E3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L41E4:
    /* 41E4  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L41E5:
    /* 41E5  cmp     si,415Eh */
    sub16(SI, 0x415E, 0);
L41E9:
    /* 41E9  jae     L41EF */
    if (!CF) goto L41EF;
L41EB:
    /* 41EB  mov     si,word ptr ds:[49C0h] */
    SI = rw(pDS, 0x49C0);
L41EF: /* L41EF */
    /* 41EF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L41F0:
    /* 41F0  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L41F1:
    /* 41F1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L41F2:
    /* 41F2  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L41F3:
    /* 41F3  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L41F6:
    /* 41F6  cld */
    DF = 0;
L41F7:
    /* 41F7  cmp     bx,dx */
    sub16(BX, DX, 0);
L41F9:
    /* 41F9  jl      L4208 */
    if (SF != OF) goto L4208;
L41FB:
    /* 41FB  mov     word ptr ds:[4A06h],cx */
    ww(pDS, 0x4A06, CX);
L41FF:
    /* 41FF  call    _seg003_423B */
    if ((c = asm_call(ASM_JMP(0x0090, 0x423B), 0x4202)) != 0) return c;
L4202:
    /* 4202  dec     word ptr ds:[49B8h] */
    ww(pDS, 0x49B8, dec16(rw(pDS, 0x49B8)));
L4206:
    /* 4206  jg      L41E0 */
    if (!ZF && SF == OF) goto L41E0;
L4208: /* L4208 */
    /* 4208  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_4209  (+4209)
       _4209: the same walk forwards, for the other side. */
L4209: /* _seg003_4209 */
    /* 4209  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L420C: /* L420C */
    /* 420C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L420D:
    /* 420D  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L420E:
    /* 420E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L420F:
    /* 420F  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L4210:
    /* 4210  cmp     si,word ptr ds:[49C0h] */
    sub16(SI, rw(pDS, 0x49C0), 0);
L4214:
    /* 4214  jb      L4219 */
    if (CF) goto L4219;
L4216:
    /* 4216  mov     si,415Ch */
    SI = 0x415C;
L4219: /* L4219 */
    /* 4219  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L421A:
    /* 421A  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L421B:
    /* 421B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L421C:
    /* 421C  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L421D:
    /* 421D  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L4220:
    /* 4220  cmp     bx,dx */
    sub16(BX, DX, 0);
L4222:
    /* 4222  jl      L4231 */
    if (SF != OF) goto L4231;
L4224:
    /* 4224  mov     word ptr ds:[4A08h],cx */
    ww(pDS, 0x4A08, CX);
L4228:
    /* 4228  call    _seg003_423B */
    if ((c = asm_call(ASM_JMP(0x0090, 0x423B), 0x422B)) != 0) return c;
L422B:
    /* 422B  dec     word ptr ds:[49BAh] */
    ww(pDS, 0x49BA, dec16(rw(pDS, 0x49BA)));
L422F:
    /* 422F  jg      L420C */
    if (!ZF && SF == OF) goto L420C;
L4231: /* L4231 */
    /* 4231  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L4232: /* L4232 */
    /* 4232  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4234:
    /* 4234  sub     cx,cx */
    CX = (uint16_t)(CX - CX);
L4236:
    /* 4236  sub     dx,dx */
    DX = (uint16_t)(DX - DX);
L4238:
    /* 4238  jmp     short L4265 */
    goto L4265;
L423A: /* L423A */
    /* 423A  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_423B  (+423B)
       _423B: one edge, from (AX, BX) to (CX, DX): its rows from BX down to DX get the stepped x
       (16.16 slope from two divides), 31 rows a call of the unrolled _4282 and the remainder through
       the entry table at 49C2. */
L423B: /* _seg003_423B */
    /* 423B  sub     bx,dx */
    BX = sub16(BX, DX, 0);
L423D:
    /* 423D  mov     word ptr ds:[4A04h],bx */
    ww(pDS, 0x4A04, BX);
L4241:
    /* 4241  mov     bp,bx */
    BP = BX;
L4243:
    /* 4243  jle     L423A */
    if (ZF || SF != OF) goto L423A;
L4245:
    /* 4245  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4246:
    /* 4246  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4249:
    /* 4249  sub     cx,ax */
    CX = sub16(CX, AX, 0);
L424B:
    /* 424B  je      L4232 */
    if (ZF) goto L4232;
L424D:
    /* 424D  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L424E:
    /* 424E  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L424F:
    /* 424F  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x0090, 0x424F, 2)) != 0) return c;
L4251:
    /* 4251  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L4252:
    /* 4252  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4254:
    /* 4254  sar     dx,1 */
    DX = sar16(DX, 1);
L4256:
    /* 4256  rcr     ax,1 */
    AX = rcr16(AX, 1);
L4258:
    /* 4258  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x0090, 0x4258, 2)) != 0) return c;
L425A:
    /* 425A  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L425B:
    /* 425B  shl     ax,1 */
    AX = shl16(AX, 1);
L425D:
    /* 425D  rcl     dx,1 */
    DX = rcl16(DX, 1);
L425F:
    /* 425F  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L4261:
    /* 4261  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L4262:
    /* 4262  mov     dx,7FFFh */
    DX = 0x7FFF;
L4265: /* L4265 */
    /* 4265  cmp     bp,20h */
    sub16(BP, 0x20, 0);
L4268:
    /* 4268  ja      L4270 */
    if (!CF && !ZF) goto L4270;
L426A:
    /* 426A  shl     bp,1 */
    BP = shl16(BP, 1);
L426C:
    /* 426C  jmp     word ptr [bp+49C2h] */
    return ASM_JMP(0x0090, rw(pSS, BP + 0x49C2));
L4270: /* L4270 */
    /* 4270  call    _seg003_4282 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4282), 0x4273)) != 0) return c;
L4273:
    /* 4273  sub     bp,1Fh */
    BP = (uint16_t)(BP - 0x1F);
L4276:
    /* 4276  cmp     bp,20h */
    sub16(BP, 0x20, 0);
L4279:
    /* 4279  ja      L4270 */
    if (!CF && !ZF) goto L4270;
L427B:
    /* 427B  shl     bp,1 */
    BP = shl16(BP, 1);
L427D:
    /* 427D  jmp     word ptr [bp+49C2h] */
    return ASM_JMP(0x0090, rw(pSS, BP + 0x49C2));
L4281:
    /* 4281  even */
    ;

    /* seg003_4282  (+4282)
       _4282: 31 rows of an edge, unrolled: add the slope to the 16.16 x and store it in the next
       record. L437B, after it, is polygon's case for one or two vertices: a line through cline_si or
       a point through pixel. */
L4282: /* _seg003_4282 */
    /* 4282  add     dx,cx */
    DX = add16(DX, CX, 0);
L4284:
    /* 4284  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4286:
    /* 4286  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4287:
    /* 4287  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L428A:
    /* 428A  add     dx,cx */
    DX = add16(DX, CX, 0);
L428C:
    /* 428C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L428E:
    /* 428E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L428F:
    /* 428F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4292:
    /* 4292  add     dx,cx */
    DX = add16(DX, CX, 0);
L4294:
    /* 4294  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4296:
    /* 4296  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4297:
    /* 4297  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L429A:
    /* 429A  add     dx,cx */
    DX = add16(DX, CX, 0);
L429C:
    /* 429C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L429E:
    /* 429E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L429F:
    /* 429F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42A2:
    /* 42A2  add     dx,cx */
    DX = add16(DX, CX, 0);
L42A4:
    /* 42A4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42A6:
    /* 42A6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42A7:
    /* 42A7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42AA:
    /* 42AA  add     dx,cx */
    DX = add16(DX, CX, 0);
L42AC:
    /* 42AC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42AE:
    /* 42AE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42AF:
    /* 42AF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42B2:
    /* 42B2  add     dx,cx */
    DX = add16(DX, CX, 0);
L42B4:
    /* 42B4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42B6:
    /* 42B6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42B7:
    /* 42B7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42BA:
    /* 42BA  add     dx,cx */
    DX = add16(DX, CX, 0);
L42BC:
    /* 42BC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42BE:
    /* 42BE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42BF:
    /* 42BF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42C2:
    /* 42C2  add     dx,cx */
    DX = add16(DX, CX, 0);
L42C4:
    /* 42C4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42C6:
    /* 42C6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42C7:
    /* 42C7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42CA:
    /* 42CA  add     dx,cx */
    DX = add16(DX, CX, 0);
L42CC:
    /* 42CC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42CE:
    /* 42CE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42CF:
    /* 42CF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42D2:
    /* 42D2  add     dx,cx */
    DX = add16(DX, CX, 0);
L42D4:
    /* 42D4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42D6:
    /* 42D6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42D7:
    /* 42D7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42DA:
    /* 42DA  add     dx,cx */
    DX = add16(DX, CX, 0);
L42DC:
    /* 42DC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42DE:
    /* 42DE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42DF:
    /* 42DF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42E2:
    /* 42E2  add     dx,cx */
    DX = add16(DX, CX, 0);
L42E4:
    /* 42E4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42E6:
    /* 42E6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42E7:
    /* 42E7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42EA:
    /* 42EA  add     dx,cx */
    DX = add16(DX, CX, 0);
L42EC:
    /* 42EC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42EE:
    /* 42EE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42EF:
    /* 42EF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42F2:
    /* 42F2  add     dx,cx */
    DX = add16(DX, CX, 0);
L42F4:
    /* 42F4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42F6:
    /* 42F6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42F7:
    /* 42F7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L42FA:
    /* 42FA  add     dx,cx */
    DX = add16(DX, CX, 0);
L42FC:
    /* 42FC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L42FE:
    /* 42FE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42FF:
    /* 42FF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4302:
    /* 4302  add     dx,cx */
    DX = add16(DX, CX, 0);
L4304:
    /* 4304  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4306:
    /* 4306  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4307:
    /* 4307  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L430A:
    /* 430A  add     dx,cx */
    DX = add16(DX, CX, 0);
L430C:
    /* 430C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L430E:
    /* 430E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L430F:
    /* 430F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4312:
    /* 4312  add     dx,cx */
    DX = add16(DX, CX, 0);
L4314:
    /* 4314  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4316:
    /* 4316  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4317:
    /* 4317  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L431A:
    /* 431A  add     dx,cx */
    DX = add16(DX, CX, 0);
L431C:
    /* 431C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L431E:
    /* 431E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L431F:
    /* 431F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4322:
    /* 4322  add     dx,cx */
    DX = add16(DX, CX, 0);
L4324:
    /* 4324  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4326:
    /* 4326  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4327:
    /* 4327  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L432A:
    /* 432A  add     dx,cx */
    DX = add16(DX, CX, 0);
L432C:
    /* 432C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L432E:
    /* 432E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L432F:
    /* 432F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4332:
    /* 4332  add     dx,cx */
    DX = add16(DX, CX, 0);
L4334:
    /* 4334  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4336:
    /* 4336  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4337:
    /* 4337  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L433A:
    /* 433A  add     dx,cx */
    DX = add16(DX, CX, 0);
L433C:
    /* 433C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L433E:
    /* 433E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L433F:
    /* 433F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4342:
    /* 4342  add     dx,cx */
    DX = add16(DX, CX, 0);
L4344:
    /* 4344  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4346:
    /* 4346  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4347:
    /* 4347  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L434A:
    /* 434A  add     dx,cx */
    DX = add16(DX, CX, 0);
L434C:
    /* 434C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L434E:
    /* 434E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L434F:
    /* 434F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4352:
    /* 4352  add     dx,cx */
    DX = add16(DX, CX, 0);
L4354:
    /* 4354  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4356:
    /* 4356  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4357:
    /* 4357  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L435A:
    /* 435A  add     dx,cx */
    DX = add16(DX, CX, 0);
L435C:
    /* 435C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L435E:
    /* 435E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L435F:
    /* 435F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4362:
    /* 4362  add     dx,cx */
    DX = add16(DX, CX, 0);
L4364:
    /* 4364  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4366:
    /* 4366  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4367:
    /* 4367  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L436A:
    /* 436A  add     dx,cx */
    DX = add16(DX, CX, 0);
L436C:
    /* 436C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L436E:
    /* 436E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L436F:
    /* 436F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4372:
    /* 4372  add     dx,cx */
    DX = add16(DX, CX, 0);
L4374:
    /* 4374  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4376:
    /* 4376  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4377:
    /* 4377  add     di,4 */
    DI = add16(DI, 0x4, 0);
L437A:
    /* 437A  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L437B: /* L437B */
    /* 437B  jne     L4383 */
    if (!ZF) goto L4383;
L437D:
    /* 437D  mov     si,415Ch */
    SI = 0x415C;
L4380:
    /* 4380  jmp     _seg003_3F06 */
    return ASM_JMP(0x0090, 0x3F06);
L4383: /* L4383 */
    /* 4383  mov     ax,word ptr ds:[VERTS] */
    AX = rw(pDS, 0x415C);
L4386:
    /* 4386  mov     bx,word ptr ds:[415Eh] */
    BX = rw(pDS, 0x415E);
L438A:
    /* 438A  jmp     _seg003_3B34 */
    return ASM_JMP(0x0090, 0x3B34);

    /* seg003_438D  (+438D)
       polygon: CX = the vertex count (more than 99 is refused), vertices at 415C. Two or fewer go to
       L437B; otherwise shclip (GRLIBF.ASM), then upolygon. GRCORE's _56F3 is the C wrapper. */
L438D: /* _seg003_438D */
    /* 438D  cmp     cx,63h */
    sub16(CX, 0x63, 0);
L4390:
    /* 4390  ja      L439D */
    if (!CF && !ZF) goto L439D;
L4392:
    /* 4392  cmp     cx,2 */
    sub16(CX, 0x2, 0);
L4395:
    /* 4395  jbe     L437B */
    if (CF || ZF) goto L437B;
L4397:
    /* 4397  call    _seg003_3D47 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3D47), 0x439A)) != 0) return c;
L439A:
    /* 439A  jmp     _seg003_4134 */
    goto L4134;
L439D: /* L439D */
    /* 439D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_439E  (+439E)
       _439E, _43A6, _43AE: string entries (see _43B6). _439E and _43A6 use the bitmap blitter L49A6
       (_439E clipped, _43A6 unclipped); _43AE is the clipped single-colour entry with the colour in
       DX. GRCORE reaches them through the jump table at 527E .. 528D. */
L439E: /* _seg003_439E */
    /* 439E  mov     word ptr ds:[4D14h],offset L49A6 */
    ww(pDS, 0x4D14, 0x49A6);
L43A4:
    /* 43A4  jmp     short L43CA */
    goto L43CA;
L43A6: /* _seg003_43A6 */
    /* 43A6  mov     word ptr ds:[4D14h],offset L49A6 */
    ww(pDS, 0x4D14, 0x49A6);
L43AC:
    /* 43AC  jmp     short L4403 */
    goto L4403;
L43AE: /* _seg003_43AE */
    /* 43AE  mov     word ptr ds:[4D14h],offset _seg003_44E1 */
    ww(pDS, 0x4D14, 0x44E1);
L43B4:
    /* 43B4  jmp     short L43C6 */
    goto L43C6;

    /* seg003_43B6  (+43B6)
       _43B6 (string_to_screen, FM Towns): draw the string at SI with its top-left corner at (AX, BX)
       in the colour at 2D38, through the single-colour blitter. The clipped entries give up unless
       the whole text box (font height up from y, and x) is inside the window: text is clipped whole,
       not cut. _43BE is the unclipped entry (ustring_to_screen). */
L43B6: /* _seg003_43B6 */
    /* 43B6  mov     word ptr ds:[4D14h],offset _seg003_44E1 */
    ww(pDS, 0x4D14, 0x44E1);
L43BC:
    /* 43BC  jmp     short L43CA */
    goto L43CA;
L43BE: /* _seg003_43BE */
    /* 43BE  mov     word ptr ds:[4D14h],offset _seg003_44E1 */
    ww(pDS, 0x4D14, 0x44E1);
L43C4:
    /* 43C4  jmp     short L4403 */
    goto L4403;
L43C6: /* L43C6 */
    /* 43C6  mov     word ptr ds:[2D38h],dx */
    ww(pDS, 0x2D38, DX);
L43CA: /* L43CA */
    /* 43CA  mov     word ptr ds:[TEXT_X],ax */
    ww(pDS, 0x4A0C, AX);
L43CD:
    /* 43CD  mov     word ptr ds:[TEXT_Y],bx */
    ww(pDS, 0x4A0E, BX);
L43D1:
    /* 43D1  mov     ax,word ptr ds:[TEXT_Y] */
    AX = rw(pDS, 0x4A0E);
L43D4:
    /* 43D4  cmp     ax,word ptr ds:[WIN_BOTTOM] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L43D8:
    /* 43D8  jl      L440E */
    if (SF != OF) goto L440E;
L43DA:
    /* 43DA  cmp     ax,word ptr ds:[WIN_TOP] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L43DE:
    /* 43DE  jg      L440E */
    if (!ZF && SF == OF) goto L440E;
L43E0:
    /* 43E0  sub     ax,word ptr ds:[4D0Ch] */
    AX = (uint16_t)(AX - rw(pDS, 0x4D0C));
L43E4:
    /* 43E4  inc     ax */
    AX = (uint16_t)(AX + 1);
L43E5:
    /* 43E5  mov     word ptr ds:[4A12h],ax */
    ww(pDS, 0x4A12, AX);
L43E8:
    /* 43E8  cmp     ax,word ptr ds:[WIN_BOTTOM] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L43EC:
    /* 43EC  jl      L440E */
    if (SF != OF) goto L440E;
L43EE:
    /* 43EE  cmp     ax,word ptr ds:[WIN_TOP] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L43F2:
    /* 43F2  jg      L440E */
    if (!ZF && SF == OF) goto L440E;
L43F4:
    /* 43F4  mov     ax,word ptr ds:[TEXT_X] */
    AX = rw(pDS, 0x4A0C);
L43F7:
    /* 43F7  cmp     ax,word ptr ds:[WIN_LEFT] */
    sub16(AX, rw(pDS, 0x3DF2), 0);
L43FB:
    /* 43FB  jl      L440E */
    if (SF != OF) goto L440E;
L43FD:
    /* 43FD  cmp     ax,word ptr ds:[WIN_RIGHT] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L4401:
    /* 4401  jg      L440E */
    if (!ZF && SF == OF) goto L440E;
L4403: /* L4403 */
    /* 4403  call    word ptr ds:[4D12h] */
    if ((c = asm_call(ASM_JMP(0x0090, rw(pDS, 0x4D12)), 0x4407)) != 0) return c;
L4407:
    /* 4407  mov     ax,word ptr ds:[2D38h] */
    AX = rw(pDS, 0x2D38);
L440A:
    /* 440A  call    word ptr ds:[4D14h] */
    if ((c = asm_call(ASM_JMP(0x0090, rw(pDS, 0x4D14)), 0x440E)) != 0) return c;
L440E: /* L440E */
    /* 440E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_440F  (+440F)
       _440F (shadowed_string_to_screen, FM Towns): the same box check, then the string drawn one
       pixel right and one down in the shadow colour (2D34), the bit buffer shifted one pixel back
       (_49D0), and the string drawn again at (x, y) in the text colour (2D38). _4448 is the
       unclipped entry. */
L440F: /* _seg003_440F */
    /* 440F  mov     word ptr ds:[TEXT_X],ax */
    ww(pDS, 0x4A0C, AX);
L4412:
    /* 4412  mov     word ptr ds:[TEXT_Y],bx */
    ww(pDS, 0x4A0E, BX);
L4416:
    /* 4416  mov     ax,word ptr ds:[TEXT_Y] */
    AX = rw(pDS, 0x4A0E);
L4419:
    /* 4419  cmp     ax,word ptr ds:[WIN_BOTTOM] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L441D:
    /* 441D  jl      L440E */
    if (SF != OF) goto L440E;
L441F:
    /* 441F  cmp     ax,word ptr ds:[WIN_TOP] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L4423:
    /* 4423  jg      L440E */
    if (!ZF && SF == OF) goto L440E;
L4425:
    /* 4425  sub     ax,word ptr ds:[4D0Ch] */
    AX = (uint16_t)(AX - rw(pDS, 0x4D0C));
L4429:
    /* 4429  inc     ax */
    AX = (uint16_t)(AX + 1);
L442A:
    /* 442A  mov     word ptr ds:[4A12h],ax */
    ww(pDS, 0x4A12, AX);
L442D:
    /* 442D  cmp     ax,word ptr ds:[WIN_BOTTOM] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L4431:
    /* 4431  jl      L440E */
    if (SF != OF) goto L440E;
L4433:
    /* 4433  cmp     ax,word ptr ds:[WIN_TOP] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L4437:
    /* 4437  jg      L440E */
    if (!ZF && SF == OF) goto L440E;
L4439:
    /* 4439  mov     ax,word ptr ds:[TEXT_X] */
    AX = rw(pDS, 0x4A0C);
L443C:
    /* 443C  cmp     ax,word ptr ds:[WIN_LEFT] */
    sub16(AX, rw(pDS, 0x3DF2), 0);
L4440:
    /* 4440  jl      L440E */
    if (SF != OF) goto L440E;
L4442:
    /* 4442  cmp     ax,word ptr ds:[WIN_RIGHT] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L4446:
    /* 4446  jg      L440E */
    if (!ZF && SF == OF) goto L440E;
L4448: /* _seg003_4448 */
    /* 4448  inc     word ptr ds:[TEXT_X] */
    ww(pDS, 0x4A0C, (uint16_t)(rw(pDS, 0x4A0C) + 1));
L444C:
    /* 444C  dec     word ptr ds:[TEXT_Y] */
    ww(pDS, 0x4A0E, (uint16_t)(rw(pDS, 0x4A0E) - 1));
L4450:
    /* 4450  call    word ptr ds:[4D12h] */
    if ((c = asm_call(ASM_JMP(0x0090, rw(pDS, 0x4D12)), 0x4454)) != 0) return c;
L4454:
    /* 4454  mov     ax,word ptr ds:[2D34h] */
    AX = rw(pDS, 0x2D34);
L4457:
    /* 4457  call    _seg003_44E1 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x44E1), 0x445A)) != 0) return c;
L445A:
    /* 445A  call    _seg003_49D0 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x49D0), 0x445D)) != 0) return c;
L445D:
    /* 445D  mov     ax,word ptr ds:[2D38h] */
    AX = rw(pDS, 0x2D38);
L4460:
    /* 4460  jmp     short _seg003_44E1 */
    goto L44E1;

    /* seg003_4462  (+4462)
       _4462: build the 256-entry table at 4A24 from the 16 plane masks at 4A14: for each byte of a
       one-bit row, the map masks of its high and low four pixels. GRCORE's _4C75 runs it through the
       jump table at 5278. */
L4462: /* _seg003_4462 */
    /* 4462  mov     cx,10h */
    CX = 0x10;
L4465:
    /* 4465  mov     si,4A14h */
    SI = 0x4A14;
L4468:
    /* 4468  mov     di,4A24h */
    DI = 0x4A24;
L446B: /* L446B */
    /* 446B  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L446C:
    /* 446C  push    cx */
    push16(CX);
L446D:
    /* 446D  push    si */
    push16(SI);
L446E:
    /* 446E  mov     cx,10h */
    CX = 0x10;
L4471:
    /* 4471  mov     si,4A14h */
    SI = 0x4A14;
L4474: /* L4474 */
    /* 4474  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4475:
    /* 4475  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L4476:
    /* 4476  loop    L4474 */
    if (--CX) goto L4474;
L4478:
    /* 4478  pop     si */
    SI = pop16();
L4479:
    /* 4479  pop     cx */
    CX = pop16();
L447A:
    /* 447A  loop    L446B */
    if (--CX) goto L446B;
L447C:
    /* 447C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_447D  (+447D)
       setup_font: after a font has been loaded into 4D06..4D10, build the glyph pointer table at
       4D16 for characters 0 to 127, from 1C56h in steps of (height << (bytes per row - 1)) + 4D06
       and copy the font's height and row width for string_width.

       After its ret are small entries that set a colour (4114) and a span writer (4110: 52CC, 52DB,
       52D8 or 52E7) and draw a rectangle from four words at SI (GRLIBF's _3C96); nothing in the
       sources jumps to them by name. */
L447D: /* _seg003_447D */
    /* 447D  mov     bx,word ptr ds:[4D0Ch] */
    BX = rw(pDS, 0x4D0C);
L4481:
    /* 4481  mov     cx,word ptr ds:[4D0Eh] */
    CX = rw(pDS, 0x4D0E);
L4485:
    /* 4485  dec     cl */
    CL = dec8(CL);
L4487:
    /* 4487  shl     bx,cl */
    BX = (uint16_t)(BX << (CL & 31));
L4489:
    /* 4489  mov     cx,80h */
    CX = 0x80;
L448C:
    /* 448C  mov     ax,1C56h */
    AX = 0x1C56;
L448F:
    /* 448F  mov     di,4D16h */
    DI = 0x4D16;
L4492:
    /* 4492  add     bx,word ptr ds:[4D06h] */
    BX = (uint16_t)(BX + rw(pDS, 0x4D06));
L4496: /* L4496 */
    /* 4496  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4497:
    /* 4497  add     ax,bx */
    AX = add16(AX, BX, 0);
L4499:
    /* 4499  loop    L4496 */
    if (--CX) goto L4496;
L449B:
    /* 449B  mov     ax,word ptr ds:[4D0Ch] */
    AX = rw(pDS, 0x4D0C);
L449E:
    /* 449E  mov     word ptr ds:[2D3Ah],ax */
    ww(pDS, 0x2D3A, AX);
L44A1:
    /* 44A1  mov     ax,word ptr ds:[4D0Eh] */
    AX = rw(pDS, 0x4D0E);
L44A4:
    /* 44A4  dec     ax */
    AX = dec16(AX);
L44A5:
    /* 44A5  mov     word ptr ds:[2D3Ch],ax */
    ww(pDS, 0x2D3C, AX);
L44A8:
    /* 44A8  mov     ax,word ptr ds:[4D10h] */
    AX = rw(pDS, 0x4D10);
L44AB:
    /* 44AB  mov     word ptr ds:[2D3Eh],ax */
    ww(pDS, 0x2D3E, AX);
L44AE:
    /* 44AE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L44AF:
    /* 44AF  mov     bx,offset _seg003_5AC6 */
    BX = 0x5AC6;
L44B2: /* L44B2 */
    /* 44B2  mov     word ptr ds:[PEN_WORD2],ax */
    ww(pDS, 0x4114, AX);
L44B5:
    /* 44B5  mov     word ptr ds:[SPAN_WRITER],bx */
    ww(pDS, 0x4110, BX);
L44B9:
    /* 44B9  mov     word ptr ds:[4112h],0FFFFh */
    ww(pDS, 0x4112, 0xFFFF);
L44BF:
    /* 44BF  jmp     _seg003_3C96 */
    return ASM_JMP(0x0090, 0x3C96);
L44C2:
    /* 44C2  mov     bx,offset _seg003_5AD5 */
    BX = 0x5AD5;
L44C5:
    /* 44C5  jmp     L44B2 */
    goto L44B2;
L44C7:
    /* 44C7  mov     bx,offset _seg003_5AD2 */
    BX = 0x5AD2;
L44CA:
    /* 44CA  jmp     L44B2 */
    goto L44B2;
L44CC: /* L44CC */
    /* 44CC  mov     bx,offset _seg003_5AE1 */
    BX = 0x5AE1;
L44CF:
    /* 44CF  jmp     L44B2 */
    goto L44B2;
L44D1:
    /* 44D1  push    word ptr [si+2] */
    push16(rw(pDS, SI + 0x2));
L44D4:
    /* 44D4  mov     ax,word ptr [si+6] */
    AX = rw(pDS, SI + 0x6);
L44D7:
    /* 44D7  mov     word ptr [si+2],ax */
    ww(pDS, SI + 0x2, AX);
L44DA:
    /* 44DA  call    L44CC */
    if ((c = asm_call(ASM_JMP(0x0090, 0x44CC), 0x44DD)) != 0) return c;
L44DD:
    /* 44DD  pop     word ptr [si+2] */
    { uint16_t t_ = pop16(); ww(pDS, SI + 0x2, t_); }
L44E0:
    /* 44E0  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_44E1  (+44E1)
       _44E1: the single-colour blitter. In: AX = the colour. Works out the screen address of (4A0C,
       4A0E) from Ytab, and writes the bit buffer row by row: for each byte, the two plane masks from
       4A24 to port 3C5h, each followed by a store of the colour. The code at the second entry (after
       the jmp) sets the colour from AH and draws a filled box behind the text first. L49A6, near the
       end, is the other blitter. */
L44E1: /* _seg003_44E1 */
    /* 44E1  mov     word ptr ds:[4A0Ah],ax */
    ww(pDS, 0x4A0A, AX);
L44E4:
    /* 44E4  jmp     short L4504 */
    goto L4504;
L44E6:
    /* 44E6  mov     byte ptr ds:[PEN_COLOR],ah */
    wb(pDS, 0x410F, AH);
L44EA:
    /* 44EA  mov     ax,word ptr ds:[4D02h] */
    AX = rw(pDS, 0x4D02);
L44ED:
    /* 44ED  dec     ax */
    AX = (uint16_t)(AX - 1);
L44EE:
    /* 44EE  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L44F0:
    /* 44F0  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L44F2:
    /* 44F2  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L44F4:
    /* 44F4  add     ax,word ptr ds:[TEXT_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x4A0C));
L44F8:
    /* 44F8  mov     word ptr ds:[4A10h],ax */
    ww(pDS, 0x4A10, AX);
L44FB:
    /* 44FB  mov     bx,5AC3h */
    BX = 0x5AC3;
L44FE:
    /* 44FE  mov     si,4A0Ch */
    SI = 0x4A0C;
L4501:
    /* 4501  call    L44B2 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x44B2), 0x4504)) != 0) return c;
L4504: /* L4504 */
    /* 4504  mov     di,word ptr ds:[TEXT_X] */
    DI = rw(pDS, 0x4A0C);
L4508:
    /* 4508  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L450A:
    /* 450A  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L450C:
    /* 450C  mov     bp,word ptr ds:[TEXT_Y] */
    BP = rw(pDS, 0x4A0E);
L4510:
    /* 4510  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L4512:
    /* 4512  add     di,word ptr [bp+YTAB] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AA));
L4516:
    /* 4516  mov     bx,word ptr ds:[4D02h] */
    BX = rw(pDS, 0x4D02);
L451A:
    /* 451A  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L451C:
    /* 451C  mov     bp,word ptr ds:[ROW_BYTES] */
    BP = rw(pDS, 0x36A6);
L4520:
    /* 4520  sub     bp,bx */
    BP = sub16(BP, BX, 0);
L4522:
    /* 4522  mov     ax,word ptr [bx+4C8Ch] */
    AX = rw(pDS, BX + 0x4C8C);
L4526:
    /* 4526  mov     word ptr ds:[4CF8h],ax */
    ww(pDS, 0x4CF8, AX);
L4529:
    /* 4529  mov     ch,byte ptr ds:[4D0Ch] */
    CH = rb(pDS, 0x4D0C);
L452D:
    /* 452D  mov     si,word ptr ds:[4D04h] */
    SI = rw(pDS, 0x4D04);
L4531:
    /* 4531  push    es */
    push16(asm_es);
L4532:
    /* 4532  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L4536:
    /* 4536  mov     cl,byte ptr ds:[4A0Ah] */
    CL = rb(pDS, 0x4A0A);
L453A:
    /* 453A  mov     dx,SC_DATA */
    DX = 0x3C5;
L453D: /* L453D */
    /* 453D  jmp     word ptr ds:[4CF8h] */
    return ASM_JMP(0x0090, rw(pDS, 0x4CF8));
L4541:
    /* 4541  even */
    ;
L4542:
    /* 4542  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4543:
    /* 4543  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4545:
    /* 4545  mov     bl,al */
    BL = AL;
L4547:
    /* 4547  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4549:
    /* 4549  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L454D:
    /* 454D  out     dx,al */
    asm_out8(DX, AL);
L454E:
    /* 454E  mov     al,cl */
    AL = CL;
L4550:
    /* 4550  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4551:
    /* 4551  mov     al,ah */
    AL = AH;
L4553:
    /* 4553  out     dx,al */
    asm_out8(DX, AL);
L4554:
    /* 4554  mov     al,cl */
    AL = CL;
L4556:
    /* 4556  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4557:
    /* 4557  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4558:
    /* 4558  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L455A:
    /* 455A  mov     bl,al */
    BL = AL;
L455C:
    /* 455C  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L455E:
    /* 455E  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4562:
    /* 4562  out     dx,al */
    asm_out8(DX, AL);
L4563:
    /* 4563  mov     al,cl */
    AL = CL;
L4565:
    /* 4565  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4566:
    /* 4566  mov     al,ah */
    AL = AH;
L4568:
    /* 4568  out     dx,al */
    asm_out8(DX, AL);
L4569:
    /* 4569  mov     al,cl */
    AL = CL;
L456B:
    /* 456B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L456C:
    /* 456C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L456D:
    /* 456D  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L456F:
    /* 456F  mov     bl,al */
    BL = AL;
L4571:
    /* 4571  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4573:
    /* 4573  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4577:
    /* 4577  out     dx,al */
    asm_out8(DX, AL);
L4578:
    /* 4578  mov     al,cl */
    AL = CL;
L457A:
    /* 457A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L457B:
    /* 457B  mov     al,ah */
    AL = AH;
L457D:
    /* 457D  out     dx,al */
    asm_out8(DX, AL);
L457E:
    /* 457E  mov     al,cl */
    AL = CL;
L4580:
    /* 4580  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4581:
    /* 4581  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4582:
    /* 4582  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4584:
    /* 4584  mov     bl,al */
    BL = AL;
L4586:
    /* 4586  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4588:
    /* 4588  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L458C:
    /* 458C  out     dx,al */
    asm_out8(DX, AL);
L458D:
    /* 458D  mov     al,cl */
    AL = CL;
L458F:
    /* 458F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4590:
    /* 4590  mov     al,ah */
    AL = AH;
L4592:
    /* 4592  out     dx,al */
    asm_out8(DX, AL);
L4593:
    /* 4593  mov     al,cl */
    AL = CL;
L4595:
    /* 4595  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4596:
    /* 4596  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4597:
    /* 4597  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4599:
    /* 4599  mov     bl,al */
    BL = AL;
L459B:
    /* 459B  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L459D:
    /* 459D  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L45A1:
    /* 45A1  out     dx,al */
    asm_out8(DX, AL);
L45A2:
    /* 45A2  mov     al,cl */
    AL = CL;
L45A4:
    /* 45A4  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L45A5:
    /* 45A5  mov     al,ah */
    AL = AH;
L45A7:
    /* 45A7  out     dx,al */
    asm_out8(DX, AL);
L45A8:
    /* 45A8  mov     al,cl */
    AL = CL;
L45AA:
    /* 45AA  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L45AB:
    /* 45AB  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L45AC:
    /* 45AC  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L45AE:
    /* 45AE  mov     bl,al */
    BL = AL;
L45B0:
    /* 45B0  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L45B2:
    /* 45B2  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L45B6:
    /* 45B6  out     dx,al */
    asm_out8(DX, AL);
L45B7:
    /* 45B7  mov     al,cl */
    AL = CL;
L45B9:
    /* 45B9  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L45BA:
    /* 45BA  mov     al,ah */
    AL = AH;
L45BC:
    /* 45BC  out     dx,al */
    asm_out8(DX, AL);
L45BD:
    /* 45BD  mov     al,cl */
    AL = CL;
L45BF:
    /* 45BF  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L45C0:
    /* 45C0  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L45C1:
    /* 45C1  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L45C3:
    /* 45C3  mov     bl,al */
    BL = AL;
L45C5:
    /* 45C5  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L45C7:
    /* 45C7  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L45CB:
    /* 45CB  out     dx,al */
    asm_out8(DX, AL);
L45CC:
    /* 45CC  mov     al,cl */
    AL = CL;
L45CE:
    /* 45CE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L45CF:
    /* 45CF  mov     al,ah */
    AL = AH;
L45D1:
    /* 45D1  out     dx,al */
    asm_out8(DX, AL);
L45D2:
    /* 45D2  mov     al,cl */
    AL = CL;
L45D4:
    /* 45D4  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L45D5:
    /* 45D5  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L45D6:
    /* 45D6  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L45D8:
    /* 45D8  mov     bl,al */
    BL = AL;
L45DA:
    /* 45DA  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L45DC:
    /* 45DC  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L45E0:
    /* 45E0  out     dx,al */
    asm_out8(DX, AL);
L45E1:
    /* 45E1  mov     al,cl */
    AL = CL;
L45E3:
    /* 45E3  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L45E4:
    /* 45E4  mov     al,ah */
    AL = AH;
L45E6:
    /* 45E6  out     dx,al */
    asm_out8(DX, AL);
L45E7:
    /* 45E7  mov     al,cl */
    AL = CL;
L45E9:
    /* 45E9  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L45EA:
    /* 45EA  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L45EB:
    /* 45EB  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L45ED:
    /* 45ED  mov     bl,al */
    BL = AL;
L45EF:
    /* 45EF  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L45F1:
    /* 45F1  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L45F5:
    /* 45F5  out     dx,al */
    asm_out8(DX, AL);
L45F6:
    /* 45F6  mov     al,cl */
    AL = CL;
L45F8:
    /* 45F8  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L45F9:
    /* 45F9  mov     al,ah */
    AL = AH;
L45FB:
    /* 45FB  out     dx,al */
    asm_out8(DX, AL);
L45FC:
    /* 45FC  mov     al,cl */
    AL = CL;
L45FE:
    /* 45FE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L45FF:
    /* 45FF  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4600:
    /* 4600  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4602:
    /* 4602  mov     bl,al */
    BL = AL;
L4604:
    /* 4604  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4606:
    /* 4606  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L460A:
    /* 460A  out     dx,al */
    asm_out8(DX, AL);
L460B:
    /* 460B  mov     al,cl */
    AL = CL;
L460D:
    /* 460D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L460E:
    /* 460E  mov     al,ah */
    AL = AH;
L4610:
    /* 4610  out     dx,al */
    asm_out8(DX, AL);
L4611:
    /* 4611  mov     al,cl */
    AL = CL;
L4613:
    /* 4613  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4614:
    /* 4614  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4615:
    /* 4615  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4617:
    /* 4617  mov     bl,al */
    BL = AL;
L4619:
    /* 4619  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L461B:
    /* 461B  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L461F:
    /* 461F  out     dx,al */
    asm_out8(DX, AL);
L4620:
    /* 4620  mov     al,cl */
    AL = CL;
L4622:
    /* 4622  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4623:
    /* 4623  mov     al,ah */
    AL = AH;
L4625:
    /* 4625  out     dx,al */
    asm_out8(DX, AL);
L4626:
    /* 4626  mov     al,cl */
    AL = CL;
L4628:
    /* 4628  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4629:
    /* 4629  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L462A:
    /* 462A  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L462C:
    /* 462C  mov     bl,al */
    BL = AL;
L462E:
    /* 462E  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4630:
    /* 4630  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4634:
    /* 4634  out     dx,al */
    asm_out8(DX, AL);
L4635:
    /* 4635  mov     al,cl */
    AL = CL;
L4637:
    /* 4637  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4638:
    /* 4638  mov     al,ah */
    AL = AH;
L463A:
    /* 463A  out     dx,al */
    asm_out8(DX, AL);
L463B:
    /* 463B  mov     al,cl */
    AL = CL;
L463D:
    /* 463D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L463E:
    /* 463E  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L463F:
    /* 463F  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4641:
    /* 4641  mov     bl,al */
    BL = AL;
L4643:
    /* 4643  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4645:
    /* 4645  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4649:
    /* 4649  out     dx,al */
    asm_out8(DX, AL);
L464A:
    /* 464A  mov     al,cl */
    AL = CL;
L464C:
    /* 464C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L464D:
    /* 464D  mov     al,ah */
    AL = AH;
L464F:
    /* 464F  out     dx,al */
    asm_out8(DX, AL);
L4650:
    /* 4650  mov     al,cl */
    AL = CL;
L4652:
    /* 4652  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4653:
    /* 4653  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4654:
    /* 4654  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4656:
    /* 4656  mov     bl,al */
    BL = AL;
L4658:
    /* 4658  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L465A:
    /* 465A  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L465E:
    /* 465E  out     dx,al */
    asm_out8(DX, AL);
L465F:
    /* 465F  mov     al,cl */
    AL = CL;
L4661:
    /* 4661  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4662:
    /* 4662  mov     al,ah */
    AL = AH;
L4664:
    /* 4664  out     dx,al */
    asm_out8(DX, AL);
L4665:
    /* 4665  mov     al,cl */
    AL = CL;
L4667:
    /* 4667  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4668:
    /* 4668  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4669:
    /* 4669  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L466B:
    /* 466B  mov     bl,al */
    BL = AL;
L466D:
    /* 466D  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L466F:
    /* 466F  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4673:
    /* 4673  out     dx,al */
    asm_out8(DX, AL);
L4674:
    /* 4674  mov     al,cl */
    AL = CL;
L4676:
    /* 4676  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4677:
    /* 4677  mov     al,ah */
    AL = AH;
L4679:
    /* 4679  out     dx,al */
    asm_out8(DX, AL);
L467A:
    /* 467A  mov     al,cl */
    AL = CL;
L467C:
    /* 467C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L467D:
    /* 467D  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L467E:
    /* 467E  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4680:
    /* 4680  mov     bl,al */
    BL = AL;
L4682:
    /* 4682  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4684:
    /* 4684  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4688:
    /* 4688  out     dx,al */
    asm_out8(DX, AL);
L4689:
    /* 4689  mov     al,cl */
    AL = CL;
L468B:
    /* 468B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L468C:
    /* 468C  mov     al,ah */
    AL = AH;
L468E:
    /* 468E  out     dx,al */
    asm_out8(DX, AL);
L468F:
    /* 468F  mov     al,cl */
    AL = CL;
L4691:
    /* 4691  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4692:
    /* 4692  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4693:
    /* 4693  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4695:
    /* 4695  mov     bl,al */
    BL = AL;
L4697:
    /* 4697  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4699:
    /* 4699  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L469D:
    /* 469D  out     dx,al */
    asm_out8(DX, AL);
L469E:
    /* 469E  mov     al,cl */
    AL = CL;
L46A0:
    /* 46A0  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L46A1:
    /* 46A1  mov     al,ah */
    AL = AH;
L46A3:
    /* 46A3  out     dx,al */
    asm_out8(DX, AL);
L46A4:
    /* 46A4  mov     al,cl */
    AL = CL;
L46A6:
    /* 46A6  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L46A7:
    /* 46A7  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L46A8:
    /* 46A8  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L46AA:
    /* 46AA  mov     bl,al */
    BL = AL;
L46AC:
    /* 46AC  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L46AE:
    /* 46AE  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L46B2:
    /* 46B2  out     dx,al */
    asm_out8(DX, AL);
L46B3:
    /* 46B3  mov     al,cl */
    AL = CL;
L46B5:
    /* 46B5  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L46B6:
    /* 46B6  mov     al,ah */
    AL = AH;
L46B8:
    /* 46B8  out     dx,al */
    asm_out8(DX, AL);
L46B9:
    /* 46B9  mov     al,cl */
    AL = CL;
L46BB:
    /* 46BB  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L46BC:
    /* 46BC  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L46BD:
    /* 46BD  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L46BF:
    /* 46BF  mov     bl,al */
    BL = AL;
L46C1:
    /* 46C1  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L46C3:
    /* 46C3  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L46C7:
    /* 46C7  out     dx,al */
    asm_out8(DX, AL);
L46C8:
    /* 46C8  mov     al,cl */
    AL = CL;
L46CA:
    /* 46CA  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L46CB:
    /* 46CB  mov     al,ah */
    AL = AH;
L46CD:
    /* 46CD  out     dx,al */
    asm_out8(DX, AL);
L46CE:
    /* 46CE  mov     al,cl */
    AL = CL;
L46D0:
    /* 46D0  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L46D1:
    /* 46D1  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L46D2:
    /* 46D2  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L46D4:
    /* 46D4  mov     bl,al */
    BL = AL;
L46D6:
    /* 46D6  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L46D8:
    /* 46D8  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L46DC:
    /* 46DC  out     dx,al */
    asm_out8(DX, AL);
L46DD:
    /* 46DD  mov     al,cl */
    AL = CL;
L46DF:
    /* 46DF  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L46E0:
    /* 46E0  mov     al,ah */
    AL = AH;
L46E2:
    /* 46E2  out     dx,al */
    asm_out8(DX, AL);
L46E3:
    /* 46E3  mov     al,cl */
    AL = CL;
L46E5:
    /* 46E5  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L46E6:
    /* 46E6  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L46E7:
    /* 46E7  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L46E9:
    /* 46E9  mov     bl,al */
    BL = AL;
L46EB:
    /* 46EB  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L46ED:
    /* 46ED  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L46F1:
    /* 46F1  out     dx,al */
    asm_out8(DX, AL);
L46F2:
    /* 46F2  mov     al,cl */
    AL = CL;
L46F4:
    /* 46F4  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L46F5:
    /* 46F5  mov     al,ah */
    AL = AH;
L46F7:
    /* 46F7  out     dx,al */
    asm_out8(DX, AL);
L46F8:
    /* 46F8  mov     al,cl */
    AL = CL;
L46FA:
    /* 46FA  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L46FB:
    /* 46FB  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L46FC:
    /* 46FC  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L46FE:
    /* 46FE  mov     bl,al */
    BL = AL;
L4700:
    /* 4700  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4702:
    /* 4702  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4706:
    /* 4706  out     dx,al */
    asm_out8(DX, AL);
L4707:
    /* 4707  mov     al,cl */
    AL = CL;
L4709:
    /* 4709  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L470A:
    /* 470A  mov     al,ah */
    AL = AH;
L470C:
    /* 470C  out     dx,al */
    asm_out8(DX, AL);
L470D:
    /* 470D  mov     al,cl */
    AL = CL;
L470F:
    /* 470F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4710:
    /* 4710  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4711:
    /* 4711  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4713:
    /* 4713  mov     bl,al */
    BL = AL;
L4715:
    /* 4715  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4717:
    /* 4717  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L471B:
    /* 471B  out     dx,al */
    asm_out8(DX, AL);
L471C:
    /* 471C  mov     al,cl */
    AL = CL;
L471E:
    /* 471E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L471F:
    /* 471F  mov     al,ah */
    AL = AH;
L4721:
    /* 4721  out     dx,al */
    asm_out8(DX, AL);
L4722:
    /* 4722  mov     al,cl */
    AL = CL;
L4724:
    /* 4724  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4725:
    /* 4725  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4726:
    /* 4726  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4728:
    /* 4728  mov     bl,al */
    BL = AL;
L472A:
    /* 472A  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L472C:
    /* 472C  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4730:
    /* 4730  out     dx,al */
    asm_out8(DX, AL);
L4731:
    /* 4731  mov     al,cl */
    AL = CL;
L4733:
    /* 4733  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4734:
    /* 4734  mov     al,ah */
    AL = AH;
L4736:
    /* 4736  out     dx,al */
    asm_out8(DX, AL);
L4737:
    /* 4737  mov     al,cl */
    AL = CL;
L4739:
    /* 4739  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L473A:
    /* 473A  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L473B:
    /* 473B  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L473D:
    /* 473D  mov     bl,al */
    BL = AL;
L473F:
    /* 473F  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4741:
    /* 4741  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4745:
    /* 4745  out     dx,al */
    asm_out8(DX, AL);
L4746:
    /* 4746  mov     al,cl */
    AL = CL;
L4748:
    /* 4748  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4749:
    /* 4749  mov     al,ah */
    AL = AH;
L474B:
    /* 474B  out     dx,al */
    asm_out8(DX, AL);
L474C:
    /* 474C  mov     al,cl */
    AL = CL;
L474E:
    /* 474E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L474F:
    /* 474F  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4750:
    /* 4750  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4752:
    /* 4752  mov     bl,al */
    BL = AL;
L4754:
    /* 4754  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4756:
    /* 4756  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L475A:
    /* 475A  out     dx,al */
    asm_out8(DX, AL);
L475B:
    /* 475B  mov     al,cl */
    AL = CL;
L475D:
    /* 475D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L475E:
    /* 475E  mov     al,ah */
    AL = AH;
L4760:
    /* 4760  out     dx,al */
    asm_out8(DX, AL);
L4761:
    /* 4761  mov     al,cl */
    AL = CL;
L4763:
    /* 4763  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4764:
    /* 4764  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4765:
    /* 4765  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4767:
    /* 4767  mov     bl,al */
    BL = AL;
L4769:
    /* 4769  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L476B:
    /* 476B  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L476F:
    /* 476F  out     dx,al */
    asm_out8(DX, AL);
L4770:
    /* 4770  mov     al,cl */
    AL = CL;
L4772:
    /* 4772  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4773:
    /* 4773  mov     al,ah */
    AL = AH;
L4775:
    /* 4775  out     dx,al */
    asm_out8(DX, AL);
L4776:
    /* 4776  mov     al,cl */
    AL = CL;
L4778:
    /* 4778  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4779:
    /* 4779  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L477A:
    /* 477A  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L477C:
    /* 477C  mov     bl,al */
    BL = AL;
L477E:
    /* 477E  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4780:
    /* 4780  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4784:
    /* 4784  out     dx,al */
    asm_out8(DX, AL);
L4785:
    /* 4785  mov     al,cl */
    AL = CL;
L4787:
    /* 4787  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4788:
    /* 4788  mov     al,ah */
    AL = AH;
L478A:
    /* 478A  out     dx,al */
    asm_out8(DX, AL);
L478B:
    /* 478B  mov     al,cl */
    AL = CL;
L478D:
    /* 478D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L478E:
    /* 478E  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L478F:
    /* 478F  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4791:
    /* 4791  mov     bl,al */
    BL = AL;
L4793:
    /* 4793  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4795:
    /* 4795  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4799:
    /* 4799  out     dx,al */
    asm_out8(DX, AL);
L479A:
    /* 479A  mov     al,cl */
    AL = CL;
L479C:
    /* 479C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L479D:
    /* 479D  mov     al,ah */
    AL = AH;
L479F:
    /* 479F  out     dx,al */
    asm_out8(DX, AL);
L47A0:
    /* 47A0  mov     al,cl */
    AL = CL;
L47A2:
    /* 47A2  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L47A3:
    /* 47A3  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L47A4:
    /* 47A4  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L47A6:
    /* 47A6  mov     bl,al */
    BL = AL;
L47A8:
    /* 47A8  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L47AA:
    /* 47AA  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L47AE:
    /* 47AE  out     dx,al */
    asm_out8(DX, AL);
L47AF:
    /* 47AF  mov     al,cl */
    AL = CL;
L47B1:
    /* 47B1  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L47B2:
    /* 47B2  mov     al,ah */
    AL = AH;
L47B4:
    /* 47B4  out     dx,al */
    asm_out8(DX, AL);
L47B5:
    /* 47B5  mov     al,cl */
    AL = CL;
L47B7:
    /* 47B7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L47B8:
    /* 47B8  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L47B9:
    /* 47B9  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L47BB:
    /* 47BB  mov     bl,al */
    BL = AL;
L47BD:
    /* 47BD  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L47BF:
    /* 47BF  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L47C3:
    /* 47C3  out     dx,al */
    asm_out8(DX, AL);
L47C4:
    /* 47C4  mov     al,cl */
    AL = CL;
L47C6:
    /* 47C6  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L47C7:
    /* 47C7  mov     al,ah */
    AL = AH;
L47C9:
    /* 47C9  out     dx,al */
    asm_out8(DX, AL);
L47CA:
    /* 47CA  mov     al,cl */
    AL = CL;
L47CC:
    /* 47CC  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L47CD:
    /* 47CD  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L47CE:
    /* 47CE  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L47D0:
    /* 47D0  mov     bl,al */
    BL = AL;
L47D2:
    /* 47D2  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L47D4:
    /* 47D4  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L47D8:
    /* 47D8  out     dx,al */
    asm_out8(DX, AL);
L47D9:
    /* 47D9  mov     al,cl */
    AL = CL;
L47DB:
    /* 47DB  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L47DC:
    /* 47DC  mov     al,ah */
    AL = AH;
L47DE:
    /* 47DE  out     dx,al */
    asm_out8(DX, AL);
L47DF:
    /* 47DF  mov     al,cl */
    AL = CL;
L47E1:
    /* 47E1  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L47E2:
    /* 47E2  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L47E3:
    /* 47E3  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L47E5:
    /* 47E5  mov     bl,al */
    BL = AL;
L47E7:
    /* 47E7  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L47E9:
    /* 47E9  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L47ED:
    /* 47ED  out     dx,al */
    asm_out8(DX, AL);
L47EE:
    /* 47EE  mov     al,cl */
    AL = CL;
L47F0:
    /* 47F0  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L47F1:
    /* 47F1  mov     al,ah */
    AL = AH;
L47F3:
    /* 47F3  out     dx,al */
    asm_out8(DX, AL);
L47F4:
    /* 47F4  mov     al,cl */
    AL = CL;
L47F6:
    /* 47F6  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L47F7:
    /* 47F7  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L47F8:
    /* 47F8  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L47FA:
    /* 47FA  mov     bl,al */
    BL = AL;
L47FC:
    /* 47FC  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L47FE:
    /* 47FE  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4802:
    /* 4802  out     dx,al */
    asm_out8(DX, AL);
L4803:
    /* 4803  mov     al,cl */
    AL = CL;
L4805:
    /* 4805  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4806:
    /* 4806  mov     al,ah */
    AL = AH;
L4808:
    /* 4808  out     dx,al */
    asm_out8(DX, AL);
L4809:
    /* 4809  mov     al,cl */
    AL = CL;
L480B:
    /* 480B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L480C:
    /* 480C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L480D:
    /* 480D  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L480F:
    /* 480F  mov     bl,al */
    BL = AL;
L4811:
    /* 4811  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4813:
    /* 4813  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4817:
    /* 4817  out     dx,al */
    asm_out8(DX, AL);
L4818:
    /* 4818  mov     al,cl */
    AL = CL;
L481A:
    /* 481A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L481B:
    /* 481B  mov     al,ah */
    AL = AH;
L481D:
    /* 481D  out     dx,al */
    asm_out8(DX, AL);
L481E:
    /* 481E  mov     al,cl */
    AL = CL;
L4820:
    /* 4820  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4821:
    /* 4821  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4822:
    /* 4822  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4824:
    /* 4824  mov     bl,al */
    BL = AL;
L4826:
    /* 4826  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4828:
    /* 4828  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L482C:
    /* 482C  out     dx,al */
    asm_out8(DX, AL);
L482D:
    /* 482D  mov     al,cl */
    AL = CL;
L482F:
    /* 482F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4830:
    /* 4830  mov     al,ah */
    AL = AH;
L4832:
    /* 4832  out     dx,al */
    asm_out8(DX, AL);
L4833:
    /* 4833  mov     al,cl */
    AL = CL;
L4835:
    /* 4835  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4836:
    /* 4836  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4837:
    /* 4837  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4839:
    /* 4839  mov     bl,al */
    BL = AL;
L483B:
    /* 483B  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L483D:
    /* 483D  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4841:
    /* 4841  out     dx,al */
    asm_out8(DX, AL);
L4842:
    /* 4842  mov     al,cl */
    AL = CL;
L4844:
    /* 4844  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4845:
    /* 4845  mov     al,ah */
    AL = AH;
L4847:
    /* 4847  out     dx,al */
    asm_out8(DX, AL);
L4848:
    /* 4848  mov     al,cl */
    AL = CL;
L484A:
    /* 484A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L484B:
    /* 484B  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L484C:
    /* 484C  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L484E:
    /* 484E  mov     bl,al */
    BL = AL;
L4850:
    /* 4850  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4852:
    /* 4852  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4856:
    /* 4856  out     dx,al */
    asm_out8(DX, AL);
L4857:
    /* 4857  mov     al,cl */
    AL = CL;
L4859:
    /* 4859  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L485A:
    /* 485A  mov     al,ah */
    AL = AH;
L485C:
    /* 485C  out     dx,al */
    asm_out8(DX, AL);
L485D:
    /* 485D  mov     al,cl */
    AL = CL;
L485F:
    /* 485F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4860:
    /* 4860  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4861:
    /* 4861  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4863:
    /* 4863  mov     bl,al */
    BL = AL;
L4865:
    /* 4865  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4867:
    /* 4867  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L486B:
    /* 486B  out     dx,al */
    asm_out8(DX, AL);
L486C:
    /* 486C  mov     al,cl */
    AL = CL;
L486E:
    /* 486E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L486F:
    /* 486F  mov     al,ah */
    AL = AH;
L4871:
    /* 4871  out     dx,al */
    asm_out8(DX, AL);
L4872:
    /* 4872  mov     al,cl */
    AL = CL;
L4874:
    /* 4874  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4875:
    /* 4875  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4876:
    /* 4876  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4878:
    /* 4878  mov     bl,al */
    BL = AL;
L487A:
    /* 487A  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L487C:
    /* 487C  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4880:
    /* 4880  out     dx,al */
    asm_out8(DX, AL);
L4881:
    /* 4881  mov     al,cl */
    AL = CL;
L4883:
    /* 4883  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4884:
    /* 4884  mov     al,ah */
    AL = AH;
L4886:
    /* 4886  out     dx,al */
    asm_out8(DX, AL);
L4887:
    /* 4887  mov     al,cl */
    AL = CL;
L4889:
    /* 4889  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L488A:
    /* 488A  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L488B:
    /* 488B  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L488D:
    /* 488D  mov     bl,al */
    BL = AL;
L488F:
    /* 488F  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4891:
    /* 4891  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4895:
    /* 4895  out     dx,al */
    asm_out8(DX, AL);
L4896:
    /* 4896  mov     al,cl */
    AL = CL;
L4898:
    /* 4898  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4899:
    /* 4899  mov     al,ah */
    AL = AH;
L489B:
    /* 489B  out     dx,al */
    asm_out8(DX, AL);
L489C:
    /* 489C  mov     al,cl */
    AL = CL;
L489E:
    /* 489E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L489F:
    /* 489F  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L48A0:
    /* 48A0  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L48A2:
    /* 48A2  mov     bl,al */
    BL = AL;
L48A4:
    /* 48A4  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L48A6:
    /* 48A6  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L48AA:
    /* 48AA  out     dx,al */
    asm_out8(DX, AL);
L48AB:
    /* 48AB  mov     al,cl */
    AL = CL;
L48AD:
    /* 48AD  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L48AE:
    /* 48AE  mov     al,ah */
    AL = AH;
L48B0:
    /* 48B0  out     dx,al */
    asm_out8(DX, AL);
L48B1:
    /* 48B1  mov     al,cl */
    AL = CL;
L48B3:
    /* 48B3  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L48B4:
    /* 48B4  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L48B5:
    /* 48B5  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L48B7:
    /* 48B7  mov     bl,al */
    BL = AL;
L48B9:
    /* 48B9  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L48BB:
    /* 48BB  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L48BF:
    /* 48BF  out     dx,al */
    asm_out8(DX, AL);
L48C0:
    /* 48C0  mov     al,cl */
    AL = CL;
L48C2:
    /* 48C2  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L48C3:
    /* 48C3  mov     al,ah */
    AL = AH;
L48C5:
    /* 48C5  out     dx,al */
    asm_out8(DX, AL);
L48C6:
    /* 48C6  mov     al,cl */
    AL = CL;
L48C8:
    /* 48C8  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L48C9:
    /* 48C9  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L48CA:
    /* 48CA  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L48CC:
    /* 48CC  mov     bl,al */
    BL = AL;
L48CE:
    /* 48CE  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L48D0:
    /* 48D0  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L48D4:
    /* 48D4  out     dx,al */
    asm_out8(DX, AL);
L48D5:
    /* 48D5  mov     al,cl */
    AL = CL;
L48D7:
    /* 48D7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L48D8:
    /* 48D8  mov     al,ah */
    AL = AH;
L48DA:
    /* 48DA  out     dx,al */
    asm_out8(DX, AL);
L48DB:
    /* 48DB  mov     al,cl */
    AL = CL;
L48DD:
    /* 48DD  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L48DE:
    /* 48DE  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L48DF:
    /* 48DF  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L48E1:
    /* 48E1  mov     bl,al */
    BL = AL;
L48E3:
    /* 48E3  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L48E5:
    /* 48E5  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L48E9:
    /* 48E9  out     dx,al */
    asm_out8(DX, AL);
L48EA:
    /* 48EA  mov     al,cl */
    AL = CL;
L48EC:
    /* 48EC  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L48ED:
    /* 48ED  mov     al,ah */
    AL = AH;
L48EF:
    /* 48EF  out     dx,al */
    asm_out8(DX, AL);
L48F0:
    /* 48F0  mov     al,cl */
    AL = CL;
L48F2:
    /* 48F2  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L48F3:
    /* 48F3  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L48F4:
    /* 48F4  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L48F6:
    /* 48F6  mov     bl,al */
    BL = AL;
L48F8:
    /* 48F8  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L48FA:
    /* 48FA  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L48FE:
    /* 48FE  out     dx,al */
    asm_out8(DX, AL);
L48FF:
    /* 48FF  mov     al,cl */
    AL = CL;
L4901:
    /* 4901  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4902:
    /* 4902  mov     al,ah */
    AL = AH;
L4904:
    /* 4904  out     dx,al */
    asm_out8(DX, AL);
L4905:
    /* 4905  mov     al,cl */
    AL = CL;
L4907:
    /* 4907  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4908:
    /* 4908  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4909:
    /* 4909  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L490B:
    /* 490B  mov     bl,al */
    BL = AL;
L490D:
    /* 490D  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L490F:
    /* 490F  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4913:
    /* 4913  out     dx,al */
    asm_out8(DX, AL);
L4914:
    /* 4914  mov     al,cl */
    AL = CL;
L4916:
    /* 4916  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4917:
    /* 4917  mov     al,ah */
    AL = AH;
L4919:
    /* 4919  out     dx,al */
    asm_out8(DX, AL);
L491A:
    /* 491A  mov     al,cl */
    AL = CL;
L491C:
    /* 491C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L491D:
    /* 491D  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L491E:
    /* 491E  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4920:
    /* 4920  mov     bl,al */
    BL = AL;
L4922:
    /* 4922  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4924:
    /* 4924  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4928:
    /* 4928  out     dx,al */
    asm_out8(DX, AL);
L4929:
    /* 4929  mov     al,cl */
    AL = CL;
L492B:
    /* 492B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L492C:
    /* 492C  mov     al,ah */
    AL = AH;
L492E:
    /* 492E  out     dx,al */
    asm_out8(DX, AL);
L492F:
    /* 492F  mov     al,cl */
    AL = CL;
L4931:
    /* 4931  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4932:
    /* 4932  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4933:
    /* 4933  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4935:
    /* 4935  mov     bl,al */
    BL = AL;
L4937:
    /* 4937  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4939:
    /* 4939  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L493D:
    /* 493D  out     dx,al */
    asm_out8(DX, AL);
L493E:
    /* 493E  mov     al,cl */
    AL = CL;
L4940:
    /* 4940  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4941:
    /* 4941  mov     al,ah */
    AL = AH;
L4943:
    /* 4943  out     dx,al */
    asm_out8(DX, AL);
L4944:
    /* 4944  mov     al,cl */
    AL = CL;
L4946:
    /* 4946  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4947:
    /* 4947  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4948:
    /* 4948  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L494A:
    /* 494A  mov     bl,al */
    BL = AL;
L494C:
    /* 494C  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L494E:
    /* 494E  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4952:
    /* 4952  out     dx,al */
    asm_out8(DX, AL);
L4953:
    /* 4953  mov     al,cl */
    AL = CL;
L4955:
    /* 4955  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4956:
    /* 4956  mov     al,ah */
    AL = AH;
L4958:
    /* 4958  out     dx,al */
    asm_out8(DX, AL);
L4959:
    /* 4959  mov     al,cl */
    AL = CL;
L495B:
    /* 495B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L495C:
    /* 495C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L495D:
    /* 495D  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L495F:
    /* 495F  mov     bl,al */
    BL = AL;
L4961:
    /* 4961  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4963:
    /* 4963  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4967:
    /* 4967  out     dx,al */
    asm_out8(DX, AL);
L4968:
    /* 4968  mov     al,cl */
    AL = CL;
L496A:
    /* 496A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L496B:
    /* 496B  mov     al,ah */
    AL = AH;
L496D:
    /* 496D  out     dx,al */
    asm_out8(DX, AL);
L496E:
    /* 496E  mov     al,cl */
    AL = CL;
L4970:
    /* 4970  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4971:
    /* 4971  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4972:
    /* 4972  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4974:
    /* 4974  mov     bl,al */
    BL = AL;
L4976:
    /* 4976  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4978:
    /* 4978  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L497C:
    /* 497C  out     dx,al */
    asm_out8(DX, AL);
L497D:
    /* 497D  mov     al,cl */
    AL = CL;
L497F:
    /* 497F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4980:
    /* 4980  mov     al,ah */
    AL = AH;
L4982:
    /* 4982  out     dx,al */
    asm_out8(DX, AL);
L4983:
    /* 4983  mov     al,cl */
    AL = CL;
L4985:
    /* 4985  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4986:
    /* 4986  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4987:
    /* 4987  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4989:
    /* 4989  mov     bl,al */
    BL = AL;
L498B:
    /* 498B  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L498D:
    /* 498D  mov     ax,word ptr [bx+TEXT_MASKS] */
    AX = rw(pDS, BX + 0x4A24);
L4991:
    /* 4991  out     dx,al */
    asm_out8(DX, AL);
L4992:
    /* 4992  mov     al,cl */
    AL = CL;
L4994:
    /* 4994  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4995:
    /* 4995  mov     al,ah */
    AL = AH;
L4997:
    /* 4997  out     dx,al */
    asm_out8(DX, AL);
L4998:
    /* 4998  mov     al,cl */
    AL = CL;
L499A:
    /* 499A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L499B:
    /* 499B  add     di,bp */
    DI = add16(DI, BP, 0);
L499D:
    /* 499D  dec     ch */
    CH = dec8(CH);
L499F:
    /* 499F  je      L49A4 */
    if (ZF) goto L49A4;
L49A1:
    /* 49A1  jmp     L453D */
    goto L453D;
L49A4: /* L49A4 */
    /* 49A4  pop     es */
    SET_ES(pop16());
L49A5:
    /* 49A5  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L49A6: /* L49A6 */
    /* 49A6  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L49A9:
    /* 49A9  mov     si,ax */
    SI = AX;
L49AB:
    /* 49AB  mov     di,word ptr ds:[4D04h] */
    DI = rw(pDS, 0x4D04);
L49AF:
    /* 49AF  mov     ax,word ptr ds:[TEXT_X] */
    AX = rw(pDS, 0x4A0C);
L49B2:
    /* 49B2  mov     bx,word ptr ds:[TEXT_Y] */
    BX = rw(pDS, 0x4A0E);
L49B6:
    /* 49B6  mov     cx,word ptr ds:[4D02h] */
    CX = rw(pDS, 0x4D02);
L49BA:
    /* 49BA  mov     dx,word ptr ds:[4D0Ch] */
    DX = rw(pDS, 0x4D0C);
L49BE:
    /* 49BE  jmp     _seg003_2C11 */
    return ASM_JMP(0x0090, 0x2C11);
L49C1: /* L49C1 */
    /* 49C1  popf */
    asm_set_flags(pop16());
L49C2:
    /* 49C2  call    _seg003_4A06 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4A06), 0x49C5)) != 0) return c;
L49C5:
    /* 49C5  pushf */
    push16(asm_flags());
L49C6:
    /* 49C6  sub     bx,ax */
    BX = (uint16_t)(BX - AX);
L49C8:
    /* 49C8  sub     si,ax */
    SI = (uint16_t)(SI - AX);
L49CA:
    /* 49CA  cmp     bx,ax */
    sub16(BX, AX, 0);
L49CC:
    /* 49CC  ja      L49C1 */
    if (!CF && !ZF) goto L49C1;
L49CE:
    /* 49CE  jmp     short L49FB */
    goto L49FB;

    /* seg003_49D0  (+49D0)
       _49D0 (shift_left_1, FM Towns): shift the whole bit buffer left by one pixel (through _4A06
       and the entry table at 4C22) and move the text's x back by one, stepping the screen address
       back a byte when x crosses a multiple of 4. */
L49D0: /* _seg003_49D0 */
    /* 49D0  clc */
    CF = 0;
L49D1:
    /* 49D1  pushf */
    push16(asm_flags());
L49D2:
    /* 49D2  test    word ptr ds:[TEXT_X],3 */
    logic16((uint16_t)(rw(pDS, 0x4A0C) & 0x3));
L49D8:
    /* 49D8  jne     L49E3 */
    if (!ZF) goto L49E3;
L49DA:
    /* 49DA  dec     word ptr ds:[4D04h] */
    ww(pDS, 0x4D04, (uint16_t)(rw(pDS, 0x4D04) - 1));
L49DE:
    /* 49DE  sub     word ptr ds:[TEXT_X],4 */
    ww(pDS, 0x4A0C, (uint16_t)(rw(pDS, 0x4A0C) - 0x4));
L49E3: /* L49E3 */
    /* 49E3  inc     word ptr ds:[TEXT_Y] */
    ww(pDS, 0x4A0E, (uint16_t)(rw(pDS, 0x4A0E) + 1));
L49E7:
    /* 49E7  dec     word ptr ds:[TEXT_X] */
    ww(pDS, 0x4A0C, (uint16_t)(rw(pDS, 0x4A0C) - 1));
L49EB:
    /* 49EB  mov     ax,35h */
    AX = 0x35;
L49EE:
    /* 49EE  mov     bx,word ptr ds:[4D00h] */
    BX = rw(pDS, 0x4D00);
L49F2:
    /* 49F2  inc     bx */
    BX = (uint16_t)(BX + 1);
L49F3:
    /* 49F3  mov     si,word ptr ds:[4CFEh] */
    SI = rw(pDS, 0x4CFE);
L49F7:
    /* 49F7  cmp     bx,ax */
    sub16(BX, AX, 0);
L49F9:
    /* 49F9  ja      L49C1 */
    if (!CF && !ZF) goto L49C1;
L49FB: /* L49FB */
    /* 49FB  add     si,ax */
    SI = (uint16_t)(SI + AX);
L49FD:
    /* 49FD  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L49FF:
    /* 49FF  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4A01:
    /* 4A01  popf */
    asm_set_flags(pop16());
L4A02:
    /* 4A02  jmp     word ptr [bx+4C22h] */
    return ASM_JMP(0x0090, rw(pDS, BX + 0x4C22));

    /* seg003_4A06  (+4A06)
       _4A06: one bit left through 53 bytes of a buffer row, unrolled (rcl from SI down to SI-34h);
       entered part way through for shorter rows.

       After its ret is the rasteriser (4225h, the routine 4D12 points at): measure the string
       (string_width), size the bit buffer (4D02 bytes wide, font height rows) and clear it, then OR
       each glyph's rows into it at the running bit position, through the unrolled loop L4B4A..L4C0D
       entered by the font height (L4C25). Characters outside 20h..7Ah are drawn as '?'. */
L4A06: /* _seg003_4A06 */
    /* 4A06  rcl     byte ptr [si],1 */
    wb(pDS, SI, rcl8(rb(pDS, SI), 1));
L4A08:
    /* 4A08  rcl     byte ptr [si-1],1 */
    wb(pDS, SI + 0xFFFF, rcl8(rb(pDS, SI + 0xFFFF), 1));
L4A0B:
    /* 4A0B  rcl     byte ptr [si-2],1 */
    wb(pDS, SI + 0xFFFE, rcl8(rb(pDS, SI + 0xFFFE), 1));
L4A0E:
    /* 4A0E  rcl     byte ptr [si-3],1 */
    wb(pDS, SI + 0xFFFD, rcl8(rb(pDS, SI + 0xFFFD), 1));
L4A11:
    /* 4A11  rcl     byte ptr [si-4],1 */
    wb(pDS, SI + 0xFFFC, rcl8(rb(pDS, SI + 0xFFFC), 1));
L4A14:
    /* 4A14  rcl     byte ptr [si-5],1 */
    wb(pDS, SI + 0xFFFB, rcl8(rb(pDS, SI + 0xFFFB), 1));
L4A17:
    /* 4A17  rcl     byte ptr [si-6],1 */
    wb(pDS, SI + 0xFFFA, rcl8(rb(pDS, SI + 0xFFFA), 1));
L4A1A:
    /* 4A1A  rcl     byte ptr [si-7],1 */
    wb(pDS, SI + 0xFFF9, rcl8(rb(pDS, SI + 0xFFF9), 1));
L4A1D:
    /* 4A1D  rcl     byte ptr [si-8],1 */
    wb(pDS, SI + 0xFFF8, rcl8(rb(pDS, SI + 0xFFF8), 1));
L4A20:
    /* 4A20  rcl     byte ptr [si-9],1 */
    wb(pDS, SI + 0xFFF7, rcl8(rb(pDS, SI + 0xFFF7), 1));
L4A23:
    /* 4A23  rcl     byte ptr [si-0Ah],1 */
    wb(pDS, SI + 0xFFF6, rcl8(rb(pDS, SI + 0xFFF6), 1));
L4A26:
    /* 4A26  rcl     byte ptr [si-0Bh],1 */
    wb(pDS, SI + 0xFFF5, rcl8(rb(pDS, SI + 0xFFF5), 1));
L4A29:
    /* 4A29  rcl     byte ptr [si-0Ch],1 */
    wb(pDS, SI + 0xFFF4, rcl8(rb(pDS, SI + 0xFFF4), 1));
L4A2C:
    /* 4A2C  rcl     byte ptr [si-0Dh],1 */
    wb(pDS, SI + 0xFFF3, rcl8(rb(pDS, SI + 0xFFF3), 1));
L4A2F:
    /* 4A2F  rcl     byte ptr [si-0Eh],1 */
    wb(pDS, SI + 0xFFF2, rcl8(rb(pDS, SI + 0xFFF2), 1));
L4A32:
    /* 4A32  rcl     byte ptr [si-0Fh],1 */
    wb(pDS, SI + 0xFFF1, rcl8(rb(pDS, SI + 0xFFF1), 1));
L4A35:
    /* 4A35  rcl     byte ptr [si-10h],1 */
    wb(pDS, SI + 0xFFF0, rcl8(rb(pDS, SI + 0xFFF0), 1));
L4A38:
    /* 4A38  rcl     byte ptr [si-11h],1 */
    wb(pDS, SI + 0xFFEF, rcl8(rb(pDS, SI + 0xFFEF), 1));
L4A3B:
    /* 4A3B  rcl     byte ptr [si-12h],1 */
    wb(pDS, SI + 0xFFEE, rcl8(rb(pDS, SI + 0xFFEE), 1));
L4A3E:
    /* 4A3E  rcl     byte ptr [si-13h],1 */
    wb(pDS, SI + 0xFFED, rcl8(rb(pDS, SI + 0xFFED), 1));
L4A41:
    /* 4A41  rcl     byte ptr [si-14h],1 */
    wb(pDS, SI + 0xFFEC, rcl8(rb(pDS, SI + 0xFFEC), 1));
L4A44:
    /* 4A44  rcl     byte ptr [si-15h],1 */
    wb(pDS, SI + 0xFFEB, rcl8(rb(pDS, SI + 0xFFEB), 1));
L4A47:
    /* 4A47  rcl     byte ptr [si-16h],1 */
    wb(pDS, SI + 0xFFEA, rcl8(rb(pDS, SI + 0xFFEA), 1));
L4A4A:
    /* 4A4A  rcl     byte ptr [si-17h],1 */
    wb(pDS, SI + 0xFFE9, rcl8(rb(pDS, SI + 0xFFE9), 1));
L4A4D:
    /* 4A4D  rcl     byte ptr [si-18h],1 */
    wb(pDS, SI + 0xFFE8, rcl8(rb(pDS, SI + 0xFFE8), 1));
L4A50:
    /* 4A50  rcl     byte ptr [si-19h],1 */
    wb(pDS, SI + 0xFFE7, rcl8(rb(pDS, SI + 0xFFE7), 1));
L4A53:
    /* 4A53  rcl     byte ptr [si-1Ah],1 */
    wb(pDS, SI + 0xFFE6, rcl8(rb(pDS, SI + 0xFFE6), 1));
L4A56:
    /* 4A56  rcl     byte ptr [si-1Bh],1 */
    wb(pDS, SI + 0xFFE5, rcl8(rb(pDS, SI + 0xFFE5), 1));
L4A59:
    /* 4A59  rcl     byte ptr [si-1Ch],1 */
    wb(pDS, SI + 0xFFE4, rcl8(rb(pDS, SI + 0xFFE4), 1));
L4A5C:
    /* 4A5C  rcl     byte ptr [si-1Dh],1 */
    wb(pDS, SI + 0xFFE3, rcl8(rb(pDS, SI + 0xFFE3), 1));
L4A5F:
    /* 4A5F  rcl     byte ptr [si-1Eh],1 */
    wb(pDS, SI + 0xFFE2, rcl8(rb(pDS, SI + 0xFFE2), 1));
L4A62:
    /* 4A62  rcl     byte ptr [si-1Fh],1 */
    wb(pDS, SI + 0xFFE1, rcl8(rb(pDS, SI + 0xFFE1), 1));
L4A65:
    /* 4A65  rcl     byte ptr [si-20h],1 */
    wb(pDS, SI + 0xFFE0, rcl8(rb(pDS, SI + 0xFFE0), 1));
L4A68:
    /* 4A68  rcl     byte ptr [si-21h],1 */
    wb(pDS, SI + 0xFFDF, rcl8(rb(pDS, SI + 0xFFDF), 1));
L4A6B:
    /* 4A6B  rcl     byte ptr [si-22h],1 */
    wb(pDS, SI + 0xFFDE, rcl8(rb(pDS, SI + 0xFFDE), 1));
L4A6E:
    /* 4A6E  rcl     byte ptr [si-23h],1 */
    wb(pDS, SI + 0xFFDD, rcl8(rb(pDS, SI + 0xFFDD), 1));
L4A71:
    /* 4A71  rcl     byte ptr [si-24h],1 */
    wb(pDS, SI + 0xFFDC, rcl8(rb(pDS, SI + 0xFFDC), 1));
L4A74:
    /* 4A74  rcl     byte ptr [si-25h],1 */
    wb(pDS, SI + 0xFFDB, rcl8(rb(pDS, SI + 0xFFDB), 1));
L4A77:
    /* 4A77  rcl     byte ptr [si-26h],1 */
    wb(pDS, SI + 0xFFDA, rcl8(rb(pDS, SI + 0xFFDA), 1));
L4A7A:
    /* 4A7A  rcl     byte ptr [si-27h],1 */
    wb(pDS, SI + 0xFFD9, rcl8(rb(pDS, SI + 0xFFD9), 1));
L4A7D:
    /* 4A7D  rcl     byte ptr [si-28h],1 */
    wb(pDS, SI + 0xFFD8, rcl8(rb(pDS, SI + 0xFFD8), 1));
L4A80:
    /* 4A80  rcl     byte ptr [si-29h],1 */
    wb(pDS, SI + 0xFFD7, rcl8(rb(pDS, SI + 0xFFD7), 1));
L4A83:
    /* 4A83  rcl     byte ptr [si-2Ah],1 */
    wb(pDS, SI + 0xFFD6, rcl8(rb(pDS, SI + 0xFFD6), 1));
L4A86:
    /* 4A86  rcl     byte ptr [si-2Bh],1 */
    wb(pDS, SI + 0xFFD5, rcl8(rb(pDS, SI + 0xFFD5), 1));
L4A89:
    /* 4A89  rcl     byte ptr [si-2Ch],1 */
    wb(pDS, SI + 0xFFD4, rcl8(rb(pDS, SI + 0xFFD4), 1));
L4A8C:
    /* 4A8C  rcl     byte ptr [si-2Dh],1 */
    wb(pDS, SI + 0xFFD3, rcl8(rb(pDS, SI + 0xFFD3), 1));
L4A8F:
    /* 4A8F  rcl     byte ptr [si-2Eh],1 */
    wb(pDS, SI + 0xFFD2, rcl8(rb(pDS, SI + 0xFFD2), 1));
L4A92:
    /* 4A92  rcl     byte ptr [si-2Fh],1 */
    wb(pDS, SI + 0xFFD1, rcl8(rb(pDS, SI + 0xFFD1), 1));
L4A95:
    /* 4A95  rcl     byte ptr [si-30h],1 */
    wb(pDS, SI + 0xFFD0, rcl8(rb(pDS, SI + 0xFFD0), 1));
L4A98:
    /* 4A98  rcl     byte ptr [si-31h],1 */
    wb(pDS, SI + 0xFFCF, rcl8(rb(pDS, SI + 0xFFCF), 1));
L4A9B:
    /* 4A9B  rcl     byte ptr [si-32h],1 */
    wb(pDS, SI + 0xFFCE, rcl8(rb(pDS, SI + 0xFFCE), 1));
L4A9E:
    /* 4A9E  rcl     byte ptr [si-33h],1 */
    wb(pDS, SI + 0xFFCD, rcl8(rb(pDS, SI + 0xFFCD), 1));
L4AA1:
    /* 4AA1  rcl     byte ptr [si-34h],1 */
    wb(pDS, SI + 0xFFCC, rcl8(rb(pDS, SI + 0xFFCC), 1));
L4AA4:
    /* 4AA4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L4AA5:
    /* 4AA5  mov     di,si */
    DI = SI;
L4AA7:
    /* 4AA7  call    _seg003_4C45 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C45), 0x4AAA)) != 0) return c;
L4AAA:
    /* 4AAA  mov     si,di */
    SI = DI;
L4AAC:
    /* 4AAC  mov     ax,word ptr ds:[TEXT_X] */
    AX = rw(pDS, 0x4A0C);
L4AAF:
    /* 4AAF  and     ax,3 */
    AX = (uint16_t)(AX & 0x3);
L4AB2:
    /* 4AB2  add     dx,ax */
    DX = (uint16_t)(DX + AX);
L4AB4:
    /* 4AB4  shr     dx,1 */
    DX = (uint16_t)(DX >> 1);
L4AB6:
    /* 4AB6  shr     dx,1 */
    DX = (uint16_t)(DX >> 1);
L4AB8:
    /* 4AB8  shr     dx,1 */
    DX = (uint16_t)(DX >> 1);
L4ABA:
    /* 4ABA  mov     cx,dx */
    CX = DX;
L4ABC:
    /* 4ABC  inc     cx */
    CX = (uint16_t)(CX + 1);
L4ABD:
    /* 4ABD  mov     word ptr ds:[4D02h],cx */
    ww(pDS, 0x4D02, CX);
L4AC1:
    /* 4AC1  mov     bp,cx */
    BP = CX;
L4AC3:
    /* 4AC3  mov     ax,word ptr ds:[4D0Ch] */
    AX = rw(pDS, 0x4D0C);
L4AC6:
    /* 4AC6  mul     cx */
    mul16(CX);
L4AC8:
    /* 4AC8  mov     cx,ax */
    CX = AX;
L4ACA:
    /* 4ACA  inc     cx */
    CX = (uint16_t)(CX + 1);
L4ACB:
    /* 4ACB  mov     word ptr ds:[4D00h],cx */
    ww(pDS, 0x4D00, CX);
L4ACF:
    /* 4ACF  mov     di,2D41h */
    DI = 0x2D41;
L4AD2:
    /* 4AD2  mov     dx,di */
    DX = DI;
L4AD4:
    /* 4AD4  inc     dx */
    DX = (uint16_t)(DX + 1);
L4AD5:
    /* 4AD5  mov     word ptr ds:[4D04h],dx */
    ww(pDS, 0x4D04, DX);
L4AD9:
    /* 4AD9  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L4ADB:
    /* 4ADB  mov     word ptr ds:[4CFEh],ax */
    ww(pDS, 0x4CFE, AX);
L4ADE:
    /* 4ADE  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4AE0:
    /* 4AE0  inc     cx */
    CX = (uint16_t)(CX + 1);
L4AE1:
    /* 4AE1  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L4AE3:
    /* 4AE3  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L4AE5:
    /* 4AE5  mov     bx,word ptr ds:[4D0Ch] */
    BX = rw(pDS, 0x4D0C);
L4AE9:
    /* 4AE9  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4AEB:
    /* 4AEB  mov     ax,word ptr cs:L4C25[bx-2] */
    AX = rw(CODE003, BX + 0x4C23);
L4AF0:
    /* 4AF0  mov     word ptr ds:[4CF8h],ax */
    ww(pDS, 0x4CF8, AX);
L4AF3:
    /* 4AF3  mov     bx,si */
    BX = SI;
L4AF5:
    /* 4AF5  mov     cx,word ptr ds:[TEXT_X] */
    CX = rw(pDS, 0x4A0C);
L4AF9:
    /* 4AF9  and     cx,3 */
    CX = (uint16_t)(CX & 0x3);
L4AFC:
    /* 4AFC  jmp     short L4B21 */
    goto L4B21;
L4AFE: /* L4AFE */
    /* 4AFE  cmp     al,8 */
    sub8(AL, 0x8, 0);
L4B00:
    /* 4B00  jle     L4B11 */
    if (ZF || SF != OF) goto L4B11;
L4B02:
    /* 4B02  cmp     ch,8 */
    sub8(CH, 0x8, 0);
L4B05:
    /* 4B05  jne     L4B0D */
    if (!ZF) goto L4B0D;
L4B07:
    /* 4B07  sub     al,ch */
    AL = (uint8_t)(AL - CH);
L4B09:
    /* 4B09  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L4B0B:
    /* 4B0B  jmp     short L4B11 */
    goto L4B11;
L4B0D: /* L4B0D */
    /* 4B0D  mov     al,8 */
    AL = 0x8;
L4B0F:
    /* 4B0F  mov     ch,al */
    CH = AL;
L4B11: /* L4B11 */
    /* 4B11  add     cl,al */
    CL = (uint8_t)(CL + AL);
L4B13:
    /* 4B13  cmp     cl,8 */
    sub8(CL, 0x8, 0);
L4B16:
    /* 4B16  jb      L4B1C */
    if (CF) goto L4B1C;
L4B18:
    /* 4B18  inc     dx */
    DX = (uint16_t)(DX + 1);
L4B19:
    /* 4B19  sub     cl,8 */
    CL = (uint8_t)(CL - 0x8);
L4B1C: /* L4B1C */
    /* 4B1C  cmp     al,ch */
    sub8(AL, CH, 0);
L4B1E:
    /* 4B1E  je      L4B21 */
    if (ZF) goto L4B21;
L4B20:
    /* 4B20  inc     bx */
    BX = (uint16_t)(BX + 1);
L4B21: /* L4B21 */
    /* 4B21  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4B23:
    /* 4B23  mov     al,byte ptr [bx] */
    AL = rb(pDS, BX);
L4B25:
    /* 4B25  test    al,al */
    logic8((uint8_t)(AL & AL));
L4B27:
    /* 4B27  jne     L4B2C */
    if (!ZF) goto L4B2C;
L4B29:
    /* 4B29  jmp     L4C24 */
    goto L4C24;
L4B2C: /* L4B2C */
    /* 4B2C  cmp     al,20h */
    sub8(AL, 0x20, 0);
L4B2E:
    /* 4B2E  jl      L4B34 */
    if (SF != OF) goto L4B34;
L4B30:
    /* 4B30  cmp     al,7Ah */
    sub8(AL, 0x7A, 0);
L4B32:
    /* 4B32  jle     L4B36 */
    if (ZF || SF != OF) goto L4B36;
L4B34: /* L4B34 */
    /* 4B34  mov     al,3Fh */
    AL = 0x3F;
L4B36: /* L4B36 */
    /* 4B36  mov     si,ax */
    SI = AX;
L4B38:
    /* 4B38  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L4B3A:
    /* 4B3A  mov     si,word ptr [si+4D16h] */
    SI = rw(pDS, SI + 0x4D16);
L4B3E:
    /* 4B3E  cmp     ch,8 */
    sub8(CH, 0x8, 0);
L4B41:
    /* 4B41  jne     L4B44 */
    if (!ZF) goto L4B44;
L4B43:
    /* 4B43  inc     si */
    SI = inc16(SI);
L4B44: /* L4B44 */
    /* 4B44  mov     di,dx */
    DI = DX;
L4B46:
    /* 4B46  jmp     word ptr ds:[4CF8h] */
    return ASM_JMP(0x0090, rw(pDS, 0x4CF8));
L4B4A: /* L4B4A */
    /* 4B4A  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4B4C:
    /* 4B4C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4B4D:
    /* 4B4D  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4B51:
    /* 4B51  ror     ax,cl */
    AX = ror16(AX, CL);
L4B53:
    /* 4B53  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4B55:
    /* 4B55  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4B57: /* L4B57 */
    /* 4B57  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4B59:
    /* 4B59  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4B5A:
    /* 4B5A  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4B5E:
    /* 4B5E  ror     ax,cl */
    AX = ror16(AX, CL);
L4B60:
    /* 4B60  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4B62:
    /* 4B62  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4B64: /* L4B64 */
    /* 4B64  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4B66:
    /* 4B66  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4B67:
    /* 4B67  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4B6B:
    /* 4B6B  ror     ax,cl */
    AX = ror16(AX, CL);
L4B6D:
    /* 4B6D  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4B6F:
    /* 4B6F  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4B71: /* L4B71 */
    /* 4B71  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4B73:
    /* 4B73  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4B74:
    /* 4B74  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4B78:
    /* 4B78  ror     ax,cl */
    AX = ror16(AX, CL);
L4B7A:
    /* 4B7A  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4B7C:
    /* 4B7C  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4B7E: /* L4B7E */
    /* 4B7E  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4B80:
    /* 4B80  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4B81:
    /* 4B81  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4B85:
    /* 4B85  ror     ax,cl */
    AX = ror16(AX, CL);
L4B87:
    /* 4B87  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4B89:
    /* 4B89  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4B8B: /* L4B8B */
    /* 4B8B  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4B8D:
    /* 4B8D  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4B8E:
    /* 4B8E  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4B92:
    /* 4B92  ror     ax,cl */
    AX = ror16(AX, CL);
L4B94:
    /* 4B94  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4B96:
    /* 4B96  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4B98: /* L4B98 */
    /* 4B98  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4B9A:
    /* 4B9A  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4B9B:
    /* 4B9B  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4B9F:
    /* 4B9F  ror     ax,cl */
    AX = ror16(AX, CL);
L4BA1:
    /* 4BA1  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4BA3:
    /* 4BA3  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4BA5: /* L4BA5 */
    /* 4BA5  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4BA7:
    /* 4BA7  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4BA8:
    /* 4BA8  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4BAC:
    /* 4BAC  ror     ax,cl */
    AX = ror16(AX, CL);
L4BAE:
    /* 4BAE  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4BB0:
    /* 4BB0  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4BB2: /* L4BB2 */
    /* 4BB2  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4BB4:
    /* 4BB4  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4BB5:
    /* 4BB5  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4BB9:
    /* 4BB9  ror     ax,cl */
    AX = ror16(AX, CL);
L4BBB:
    /* 4BBB  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4BBD:
    /* 4BBD  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4BBF: /* L4BBF */
    /* 4BBF  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4BC1:
    /* 4BC1  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4BC2:
    /* 4BC2  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4BC6:
    /* 4BC6  ror     ax,cl */
    AX = ror16(AX, CL);
L4BC8:
    /* 4BC8  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4BCA:
    /* 4BCA  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4BCC: /* L4BCC */
    /* 4BCC  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4BCE:
    /* 4BCE  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4BCF:
    /* 4BCF  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4BD3:
    /* 4BD3  ror     ax,cl */
    AX = ror16(AX, CL);
L4BD5:
    /* 4BD5  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4BD7:
    /* 4BD7  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4BD9: /* L4BD9 */
    /* 4BD9  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4BDB:
    /* 4BDB  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4BDC:
    /* 4BDC  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4BE0:
    /* 4BE0  ror     ax,cl */
    AX = ror16(AX, CL);
L4BE2:
    /* 4BE2  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4BE4:
    /* 4BE4  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4BE6: /* L4BE6 */
    /* 4BE6  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4BE8:
    /* 4BE8  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4BE9:
    /* 4BE9  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4BED:
    /* 4BED  ror     ax,cl */
    AX = ror16(AX, CL);
L4BEF:
    /* 4BEF  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4BF1:
    /* 4BF1  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4BF3: /* L4BF3 */
    /* 4BF3  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4BF5:
    /* 4BF5  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4BF6:
    /* 4BF6  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4BFA:
    /* 4BFA  ror     ax,cl */
    AX = ror16(AX, CL);
L4BFC:
    /* 4BFC  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4BFE:
    /* 4BFE  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4C00: /* L4C00 */
    /* 4C00  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4C02:
    /* 4C02  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4C03:
    /* 4C03  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4C07:
    /* 4C07  ror     ax,cl */
    AX = ror16(AX, CL);
L4C09:
    /* 4C09  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4C0B:
    /* 4C0B  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4C0D: /* L4C0D */
    /* 4C0D  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4C0F:
    /* 4C0F  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4C10:
    /* 4C10  add     si,word ptr ds:[2D3Ch] */
    SI = add16(SI, rw(pDS, 0x2D3C), 0);
L4C14:
    /* 4C14  ror     ax,cl */
    AX = ror16(AX, CL);
L4C16:
    /* 4C16  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4C18:
    /* 4C18  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4C1A:
    /* 4C1A  cmp     ch,8 */
    sub8(CH, 0x8, 0);
L4C1D:
    /* 4C1D  jne     L4C20 */
    if (!ZF) goto L4C20;
L4C1F:
    /* 4C1F  dec     si */
    SI = (uint16_t)(SI - 1);
L4C20: /* L4C20 */
    /* 4C20  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4C21:
    /* 4C21  jmp     L4AFE */
    goto L4AFE;
L4C24: /* L4C24 */
    /* 4C24  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_4C45  (+4C45)
       string_width: in: SI = the string. Out: AX = DX = the sum of its characters' widths (the byte
       just past each glyph's rows), stopping at the terminating 0 or after 54 characters. GRCORE's
       string_width is the C wrapper. */
L4C45: /* _seg003_4C45 */
    /* 4C45  mov     bx,word ptr ds:[2D3Ah] */
    BX = rw(pDS, 0x2D3A);
L4C49:
    /* 4C49  mov     cx,word ptr ds:[2D3Ch] */
    CX = rw(pDS, 0x2D3C);
L4C4D:
    /* 4C4D  shl     bx,cl */
    BX = (uint16_t)(BX << (CL & 31));
L4C4F:
    /* 4C4F  mov     cx,36h */
    CX = 0x36;
L4C52:
    /* 4C52  sub     dx,dx */
    DX = (uint16_t)(DX - DX);
L4C54:
    /* 4C54  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4C56: /* L4C56 */
    /* 4C56  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4C57:
    /* 4C57  test    al,al */
    logic8((uint8_t)(AL & AL));
L4C59:
    /* 4C59  je      L4C6C */
    if (ZF) goto L4C6C;
L4C5B:
    /* 4C5B  mov     bp,ax */
    BP = AX;
L4C5D:
    /* 4C5D  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L4C5F:
    /* 4C5F  mov     bp,word ptr [bp+4D16h] */
    BP = rw(pSS, BP + 0x4D16);
L4C63:
    /* 4C63  add     bp,bx */
    BP = (uint16_t)(BP + BX);
L4C65:
    /* 4C65  mov     al,byte ptr [bp] */
    AL = rb(pSS, BP);
L4C68:
    /* 4C68  add     dx,ax */
    DX = add16(DX, AX, 0);
L4C6A:
    /* 4C6A  loop    L4C56 */
    if (--CX) goto L4C56;
L4C6C: /* L4C6C */
    /* 4C6C  mov     ax,dx */
    AX = DX;
L4C6E: /* L4C6E */
    /* 4C6E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
