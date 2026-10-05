/* interp.c: replaces src/3d/INTERP.ASM (seg004_0849_DB0, 0DB0..2FFB of its
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

uint32_t asm_mod_INTERP(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0DB0: goto L0DB0;
    case 0x0DB1: goto L0DB1;
    case 0x0DB4: goto L0DB4;
    case 0x0DB6: goto L0DB6;
    case 0x0DB9: goto L0DB9;
    case 0x0DBC: goto L0DBC;
    case 0x0DBD: goto L0DBD;
    case 0x0DBE: goto L0DBE;
    case 0x0DBF: goto L0DBF;
    case 0x0DC2: goto L0DC2;
    case 0x0DC4: goto L0DC4;
    case 0x0DC7: goto L0DC7;
    case 0x0DCA: goto L0DCA;
    case 0x0DD0: goto L0DD0;
    case 0x0DD1: goto L0DD1;
    case 0x0DD2: goto L0DD2;
    case 0x0DD3: goto L0DD3;
    case 0x0DD6: goto L0DD6;
    case 0x0DD8: goto L0DD8;
    case 0x0DDB: goto L0DDB;
    case 0x0DDD: goto L0DDD;
    case 0x0DDF: goto L0DDF;
    case 0x0DE5: goto L0DE5;
    case 0x0DEB: goto L0DEB;
    case 0x0DF2: goto L0DF2;
    case 0x0DF4: goto L0DF4;
    case 0x0DF7: goto L0DF7;
    case 0x0DF9: goto L0DF9;
    case 0x0DFC: goto L0DFC;
    case 0x0DFF: goto L0DFF;
    case 0x0E01: goto L0E01;
    case 0x0E06: goto L0E06;
    case 0x0E08: goto L0E08;
    case 0x0E0F: goto L0E0F;
    case 0x0E10: goto L0E10;
    case 0x0E11: goto L0E11;
    case 0x0E13: goto L0E13;
    case 0x0E15: goto L0E15;
    case 0x0E18: goto L0E18;
    case 0x0E1B: goto L0E1B;
    case 0x0E1F: goto L0E1F;
    case 0x0E23: goto L0E23;
    case 0x0E25: goto L0E25;
    case 0x0E27: goto L0E27;
    case 0x0E28: goto L0E28;
    case 0x0E2A: goto L0E2A;
    case 0x0E2E: goto L0E2E;
    case 0x0E30: goto L0E30;
    case 0x0E33: goto L0E33;
    case 0x0E36: goto L0E36;
    case 0x0E39: goto L0E39;
    case 0x0E3C: goto L0E3C;
    case 0x0E3F: goto L0E3F;
    case 0x0E41: goto L0E41;
    case 0x0E44: goto L0E44;
    case 0x0E45: goto L0E45;
    case 0x0E48: goto L0E48;
    case 0x0E49: goto L0E49;
    case 0x0E4C: goto L0E4C;
    case 0x0E4D: goto L0E4D;
    case 0x0E50: goto L0E50;
    case 0x0E51: goto L0E51;
    case 0x0E52: goto L0E52;
    case 0x0E53: goto L0E53;
    case 0x0E56: goto L0E56;
    case 0x0E57: goto L0E57;
    case 0x0E59: goto L0E59;
    case 0x0E5A: goto L0E5A;
    case 0x0E5C: goto L0E5C;
    case 0x0E5E: goto L0E5E;
    case 0x0E65: goto L0E65;
    case 0x0E67: goto L0E67;
    case 0x0E69: goto L0E69;
    case 0x0E6C: goto L0E6C;
    case 0x0E6F: goto L0E6F;
    case 0x0E74: goto L0E74;
    case 0x0E76: goto L0E76;
    case 0x0E7B: goto L0E7B;
    case 0x0E7C: goto L0E7C;
    case 0x0E7D: goto L0E7D;
    case 0x0E80: goto L0E80;
    case 0x0E82: goto L0E82;
    case 0x0E85: goto L0E85;
    case 0x0E88: goto L0E88;
    case 0x0E89: goto L0E89;
    case 0x0E8A: goto L0E8A;
    case 0x0E8D: goto L0E8D;
    case 0x0E8F: goto L0E8F;
    case 0x0E91: goto L0E91;
    case 0x0E94: goto L0E94;
    case 0x0E97: goto L0E97;
    case 0x0E9C: goto L0E9C;
    case 0x0E9E: goto L0E9E;
    case 0x0E9F: goto L0E9F;
    case 0x0EA2: goto L0EA2;
    case 0x0EA3: goto L0EA3;
    case 0x0EA6: goto L0EA6;
    case 0x0EA8: goto L0EA8;
    case 0x0EA9: goto L0EA9;
    case 0x0EAC: goto L0EAC;
    case 0x0EAF: goto L0EAF;
    case 0x0EB2: goto L0EB2;
    case 0x0EB6: goto L0EB6;
    case 0x0EB8: goto L0EB8;
    case 0x0EB9: goto L0EB9;
    case 0x0EBC: goto L0EBC;
    case 0x0EBF: goto L0EBF;
    case 0x0EC2: goto L0EC2;
    case 0x0EC6: goto L0EC6;
    case 0x0EC8: goto L0EC8;
    case 0x0EC9: goto L0EC9;
    case 0x0ECC: goto L0ECC;
    case 0x0ECF: goto L0ECF;
    case 0x0ED2: goto L0ED2;
    case 0x0ED6: goto L0ED6;
    case 0x0ED9: goto L0ED9;
    case 0x0EDC: goto L0EDC;
    case 0x0EDD: goto L0EDD;
    case 0x0EDF: goto L0EDF;
    case 0x0EE2: goto L0EE2;
    case 0x0EE4: goto L0EE4;
    case 0x0EE6: goto L0EE6;
    case 0x0EE8: goto L0EE8;
    case 0x0EEA: goto L0EEA;
    case 0x0EEC: goto L0EEC;
    case 0x0EEF: goto L0EEF;
    case 0x0EF1: goto L0EF1;
    case 0x0EF3: goto L0EF3;
    case 0x0EF5: goto L0EF5;
    case 0x0EF8: goto L0EF8;
    case 0x0EFA: goto L0EFA;
    case 0x0EFD: goto L0EFD;
    case 0x0EFF: goto L0EFF;
    case 0x0F01: goto L0F01;
    case 0x0F03: goto L0F03;
    case 0x0F06: goto L0F06;
    case 0x0F07: goto L0F07;
    case 0x0F08: goto L0F08;
    case 0x0F0B: goto L0F0B;
    case 0x0F0C: goto L0F0C;
    case 0x0F0E: goto L0F0E;
    case 0x0F10: goto L0F10;
    case 0x0F11: goto L0F11;
    case 0x0F13: goto L0F13;
    case 0x0F16: goto L0F16;
    case 0x0F18: goto L0F18;
    case 0x0F1D: goto L0F1D;
    case 0x0F22: goto L0F22;
    case 0x0F26: goto L0F26;
    case 0x0F29: goto L0F29;
    case 0x0F2B: goto L0F2B;
    case 0x0F30: goto L0F30;
    case 0x0F35: goto L0F35;
    case 0x0F39: goto L0F39;
    case 0x0F3C: goto L0F3C;
    case 0x0F3E: goto L0F3E;
    case 0x0F43: goto L0F43;
    case 0x0F48: goto L0F48;
    case 0x0F4C: goto L0F4C;
    case 0x0F4D: goto L0F4D;
    case 0x0F4F: goto L0F4F;
    case 0x0F51: goto L0F51;
    case 0x0F54: goto L0F54;
    case 0x0F56: goto L0F56;
    case 0x0F59: goto L0F59;
    case 0x0F5C: goto L0F5C;
    case 0x0F60: goto L0F60;
    case 0x0F62: goto L0F62;
    case 0x0F64: goto L0F64;
    case 0x0F66: goto L0F66;
    case 0x0F69: goto L0F69;
    case 0x0F6D: goto L0F6D;
    case 0x0F6F: goto L0F6F;
    case 0x0F71: goto L0F71;
    case 0x0F73: goto L0F73;
    case 0x0F74: goto L0F74;
    case 0x0F78: goto L0F78;
    case 0x0F7A: goto L0F7A;
    case 0x0F7C: goto L0F7C;
    case 0x0F7E: goto L0F7E;
    case 0x0F81: goto L0F81;
    case 0x0F84: goto L0F84;
    case 0x0F88: goto L0F88;
    case 0x0F8A: goto L0F8A;
    case 0x0F8C: goto L0F8C;
    case 0x0F8E: goto L0F8E;
    case 0x0F91: goto L0F91;
    case 0x0F94: goto L0F94;
    case 0x0F98: goto L0F98;
    case 0x0F9A: goto L0F9A;
    case 0x0F9C: goto L0F9C;
    case 0x0F9E: goto L0F9E;
    case 0x0FA1: goto L0FA1;
    case 0x0FA5: goto L0FA5;
    case 0x0FA7: goto L0FA7;
    case 0x0FA9: goto L0FA9;
    case 0x0FAB: goto L0FAB;
    case 0x0FAC: goto L0FAC;
    case 0x0FB0: goto L0FB0;
    case 0x0FB2: goto L0FB2;
    case 0x0FB4: goto L0FB4;
    case 0x0FB6: goto L0FB6;
    case 0x0FB8: goto L0FB8;
    case 0x0FBB: goto L0FBB;
    case 0x0FBD: goto L0FBD;
    case 0x0FC1: goto L0FC1;
    case 0x0FC3: goto L0FC3;
    case 0x0FC5: goto L0FC5;
    case 0x0FC7: goto L0FC7;
    case 0x0FCA: goto L0FCA;
    case 0x0FCD: goto L0FCD;
    case 0x0FD1: goto L0FD1;
    case 0x0FD3: goto L0FD3;
    case 0x0FD5: goto L0FD5;
    case 0x0FD8: goto L0FD8;
    case 0x0FDA: goto L0FDA;
    case 0x0FDE: goto L0FDE;
    case 0x0FE0: goto L0FE0;
    case 0x0FE2: goto L0FE2;
    case 0x0FE4: goto L0FE4;
    case 0x0FE7: goto L0FE7;
    case 0x0FEA: goto L0FEA;
    case 0x0FEE: goto L0FEE;
    case 0x0FF0: goto L0FF0;
    case 0x0FF2: goto L0FF2;
    case 0x0FF4: goto L0FF4;
    case 0x0FF7: goto L0FF7;
    case 0x0FFA: goto L0FFA;
    case 0x0FFE: goto L0FFE;
    case 0x1000: goto L1000;
    case 0x1002: goto L1002;
    case 0x1005: goto L1005;
    case 0x1006: goto L1006;
    case 0x1008: goto L1008;
    case 0x100A: goto L100A;
    case 0x100D: goto L100D;
    case 0x100F: goto L100F;
    case 0x1014: goto L1014;
    case 0x1019: goto L1019;
    case 0x101D: goto L101D;
    case 0x1021: goto L1021;
    case 0x1023: goto L1023;
    case 0x1028: goto L1028;
    case 0x102D: goto L102D;
    case 0x1031: goto L1031;
    case 0x1033: goto L1033;
    case 0x1035: goto L1035;
    case 0x1038: goto L1038;
    case 0x103A: goto L103A;
    case 0x103D: goto L103D;
    case 0x1041: goto L1041;
    case 0x1043: goto L1043;
    case 0x1045: goto L1045;
    case 0x1047: goto L1047;
    case 0x104A: goto L104A;
    case 0x104D: goto L104D;
    case 0x1051: goto L1051;
    case 0x1053: goto L1053;
    case 0x1055: goto L1055;
    case 0x1057: goto L1057;
    case 0x105A: goto L105A;
    case 0x105F: goto L105F;
    case 0x1062: goto L1062;
    case 0x1065: goto L1065;
    case 0x1068: goto L1068;
    case 0x106A: goto L106A;
    case 0x106D: goto L106D;
    case 0x1070: goto L1070;
    case 0x1073: goto L1073;
    case 0x1076: goto L1076;
    case 0x107A: goto L107A;
    case 0x107C: goto L107C;
    case 0x107E: goto L107E;
    case 0x1081: goto L1081;
    case 0x1084: goto L1084;
    case 0x1088: goto L1088;
    case 0x108A: goto L108A;
    case 0x108C: goto L108C;
    case 0x108F: goto L108F;
    case 0x1090: goto L1090;
    case 0x1092: goto L1092;
    case 0x1093: goto L1093;
    case 0x1094: goto L1094;
    case 0x1098: goto L1098;
    case 0x1099: goto L1099;
    case 0x109A: goto L109A;
    case 0x109C: goto L109C;
    case 0x10A0: goto L10A0;
    case 0x10A5: goto L10A5;
    case 0x10A7: goto L10A7;
    case 0x10AB: goto L10AB;
    case 0x10AD: goto L10AD;
    case 0x10B0: goto L10B0;
    case 0x10B3: goto L10B3;
    case 0x10B7: goto L10B7;
    case 0x10B9: goto L10B9;
    case 0x10BD: goto L10BD;
    case 0x10BF: goto L10BF;
    case 0x10C1: goto L10C1;
    case 0x10C5: goto L10C5;
    case 0x10C7: goto L10C7;
    case 0x10CB: goto L10CB;
    case 0x10CC: goto L10CC;
    case 0x10D1: goto L10D1;
    case 0x10D2: goto L10D2;
    case 0x10D3: goto L10D3;
    case 0x10D4: goto L10D4;
    case 0x10D8: goto L10D8;
    case 0x10D9: goto L10D9;
    case 0x10DB: goto L10DB;
    case 0x10DC: goto L10DC;
    case 0x10DE: goto L10DE;
    case 0x10E2: goto L10E2;
    case 0x10E6: goto L10E6;
    case 0x10E8: goto L10E8;
    case 0x10E9: goto L10E9;
    case 0x10ED: goto L10ED;
    case 0x10EF: goto L10EF;
    case 0x10F3: goto L10F3;
    case 0x10F7: goto L10F7;
    case 0x10FB: goto L10FB;
    case 0x10FD: goto L10FD;
    case 0x1101: goto L1101;
    case 0x1103: goto L1103;
    case 0x1107: goto L1107;
    case 0x110B: goto L110B;
    case 0x110D: goto L110D;
    case 0x1111: goto L1111;
    case 0x1113: goto L1113;
    case 0x1115: goto L1115;
    case 0x1119: goto L1119;
    case 0x111D: goto L111D;
    case 0x1121: goto L1121;
    case 0x1123: goto L1123;
    case 0x1127: goto L1127;
    case 0x1129: goto L1129;
    case 0x112D: goto L112D;
    case 0x1131: goto L1131;
    case 0x1133: goto L1133;
    case 0x1137: goto L1137;
    case 0x1139: goto L1139;
    case 0x113E: goto L113E;
    case 0x113F: goto L113F;
    case 0x1140: goto L1140;
    case 0x1141: goto L1141;
    case 0x1145: goto L1145;
    case 0x1149: goto L1149;
    case 0x114D: goto L114D;
    case 0x114F: goto L114F;
    case 0x1151: goto L1151;
    case 0x1158: goto L1158;
    case 0x115B: goto L115B;
    case 0x115D: goto L115D;
    case 0x1161: goto L1161;
    case 0x1163: goto L1163;
    case 0x1167: goto L1167;
    case 0x1169: goto L1169;
    case 0x116C: goto L116C;
    case 0x1170: goto L1170;
    case 0x1172: goto L1172;
    case 0x1176: goto L1176;
    case 0x1178: goto L1178;
    case 0x117F: goto L117F;
    case 0x1182: goto L1182;
    case 0x1185: goto L1185;
    case 0x1189: goto L1189;
    case 0x118B: goto L118B;
    case 0x118F: goto L118F;
    case 0x1191: goto L1191;
    case 0x1193: goto L1193;
    case 0x1197: goto L1197;
    case 0x1199: goto L1199;
    case 0x119D: goto L119D;
    case 0x119F: goto L119F;
    case 0x11A4: goto L11A4;
    case 0x11A5: goto L11A5;
    case 0x11A6: goto L11A6;
    case 0x11A7: goto L11A7;
    case 0x11AB: goto L11AB;
    case 0x11AD: goto L11AD;
    case 0x11AE: goto L11AE;
    case 0x11B1: goto L11B1;
    case 0x11B4: goto L11B4;
    case 0x11B6: goto L11B6;
    case 0x11B8: goto L11B8;
    case 0x11BB: goto L11BB;
    case 0x11BD: goto L11BD;
    case 0x11BF: goto L11BF;
    case 0x11C2: goto L11C2;
    case 0x11C4: goto L11C4;
    case 0x11C6: goto L11C6;
    case 0x11C9: goto L11C9;
    case 0x11CC: goto L11CC;
    case 0x11CE: goto L11CE;
    case 0x11D0: goto L11D0;
    case 0x11D2: goto L11D2;
    case 0x11D5: goto L11D5;
    case 0x11D7: goto L11D7;
    case 0x11D9: goto L11D9;
    case 0x11E0: goto L11E0;
    case 0x11E3: goto L11E3;
    case 0x11E6: goto L11E6;
    case 0x11E9: goto L11E9;
    case 0x11EC: goto L11EC;
    case 0x11EF: goto L11EF;
    case 0x11F2: goto L11F2;
    case 0x11F4: goto L11F4;
    case 0x11F6: goto L11F6;
    case 0x11F8: goto L11F8;
    case 0x11FA: goto L11FA;
    case 0x11FC: goto L11FC;
    case 0x11FF: goto L11FF;
    case 0x1202: goto L1202;
    case 0x1204: goto L1204;
    case 0x1206: goto L1206;
    case 0x1208: goto L1208;
    case 0x120A: goto L120A;
    case 0x120D: goto L120D;
    case 0x1210: goto L1210;
    case 0x1212: goto L1212;
    case 0x1214: goto L1214;
    case 0x1216: goto L1216;
    case 0x1218: goto L1218;
    case 0x121A: goto L121A;
    case 0x121C: goto L121C;
    case 0x121E: goto L121E;
    case 0x1220: goto L1220;
    case 0x1222: goto L1222;
    case 0x1225: goto L1225;
    case 0x1227: goto L1227;
    case 0x122A: goto L122A;
    case 0x122C: goto L122C;
    case 0x122E: goto L122E;
    case 0x1231: goto L1231;
    case 0x1233: goto L1233;
    case 0x1236: goto L1236;
    case 0x1239: goto L1239;
    case 0x123B: goto L123B;
    case 0x123D: goto L123D;
    case 0x1240: goto L1240;
    case 0x1242: goto L1242;
    case 0x1244: goto L1244;
    case 0x1246: goto L1246;
    case 0x1248: goto L1248;
    case 0x124A: goto L124A;
    case 0x124C: goto L124C;
    case 0x124E: goto L124E;
    case 0x1250: goto L1250;
    case 0x1252: goto L1252;
    case 0x1255: goto L1255;
    case 0x1258: goto L1258;
    case 0x125F: goto L125F;
    case 0x1262: goto L1262;
    case 0x1265: goto L1265;
    case 0x1268: goto L1268;
    case 0x126B: goto L126B;
    case 0x126E: goto L126E;
    case 0x1271: goto L1271;
    case 0x1273: goto L1273;
    case 0x1275: goto L1275;
    case 0x1277: goto L1277;
    case 0x1279: goto L1279;
    case 0x127B: goto L127B;
    case 0x127E: goto L127E;
    case 0x1281: goto L1281;
    case 0x1283: goto L1283;
    case 0x1285: goto L1285;
    case 0x1287: goto L1287;
    case 0x1289: goto L1289;
    case 0x128C: goto L128C;
    case 0x128F: goto L128F;
    case 0x1291: goto L1291;
    case 0x1293: goto L1293;
    case 0x1295: goto L1295;
    case 0x1297: goto L1297;
    case 0x1299: goto L1299;
    case 0x129B: goto L129B;
    case 0x129D: goto L129D;
    case 0x12A0: goto L12A0;
    case 0x12A2: goto L12A2;
    case 0x12A5: goto L12A5;
    case 0x12A7: goto L12A7;
    case 0x12A9: goto L12A9;
    case 0x12AC: goto L12AC;
    case 0x12AE: goto L12AE;
    case 0x12B1: goto L12B1;
    case 0x12B3: goto L12B3;
    case 0x12B5: goto L12B5;
    case 0x12B8: goto L12B8;
    case 0x12BA: goto L12BA;
    case 0x12BC: goto L12BC;
    case 0x12BE: goto L12BE;
    case 0x12C0: goto L12C0;
    case 0x12C2: goto L12C2;
    case 0x12C4: goto L12C4;
    case 0x12C6: goto L12C6;
    case 0x12C8: goto L12C8;
    case 0x12CA: goto L12CA;
    case 0x12CD: goto L12CD;
    case 0x12D0: goto L12D0;
    case 0x12D3: goto L12D3;
    case 0x12DA: goto L12DA;
    case 0x12DD: goto L12DD;
    case 0x12DF: goto L12DF;
    case 0x12E1: goto L12E1;
    case 0x12E3: goto L12E3;
    case 0x12E6: goto L12E6;
    case 0x12E9: goto L12E9;
    case 0x12EB: goto L12EB;
    case 0x12ED: goto L12ED;
    case 0x12EF: goto L12EF;
    case 0x12F1: goto L12F1;
    case 0x12F3: goto L12F3;
    case 0x12F5: goto L12F5;
    case 0x12F7: goto L12F7;
    case 0x12F9: goto L12F9;
    case 0x12FB: goto L12FB;
    case 0x12FD: goto L12FD;
    case 0x12FF: goto L12FF;
    case 0x1302: goto L1302;
    case 0x1304: goto L1304;
    case 0x1306: goto L1306;
    case 0x1308: goto L1308;
    case 0x130A: goto L130A;
    case 0x130D: goto L130D;
    case 0x1310: goto L1310;
    case 0x1312: goto L1312;
    case 0x1314: goto L1314;
    case 0x1316: goto L1316;
    case 0x1318: goto L1318;
    case 0x131B: goto L131B;
    case 0x131E: goto L131E;
    case 0x1321: goto L1321;
    case 0x1323: goto L1323;
    case 0x1325: goto L1325;
    case 0x1328: goto L1328;
    case 0x132A: goto L132A;
    case 0x132D: goto L132D;
    case 0x132F: goto L132F;
    case 0x1331: goto L1331;
    case 0x1334: goto L1334;
    case 0x1336: goto L1336;
    case 0x1338: goto L1338;
    case 0x133A: goto L133A;
    case 0x133C: goto L133C;
    case 0x133E: goto L133E;
    case 0x1340: goto L1340;
    case 0x1342: goto L1342;
    case 0x1344: goto L1344;
    case 0x1347: goto L1347;
    case 0x134A: goto L134A;
    case 0x134D: goto L134D;
    case 0x1354: goto L1354;
    case 0x1356: goto L1356;
    case 0x1359: goto L1359;
    case 0x135C: goto L135C;
    case 0x135F: goto L135F;
    case 0x1361: goto L1361;
    case 0x1363: goto L1363;
    case 0x1365: goto L1365;
    case 0x1367: goto L1367;
    case 0x1369: goto L1369;
    case 0x136B: goto L136B;
    case 0x136D: goto L136D;
    case 0x1370: goto L1370;
    case 0x1373: goto L1373;
    case 0x1375: goto L1375;
    case 0x1377: goto L1377;
    case 0x1379: goto L1379;
    case 0x137B: goto L137B;
    case 0x137E: goto L137E;
    case 0x1381: goto L1381;
    case 0x1383: goto L1383;
    case 0x1385: goto L1385;
    case 0x1387: goto L1387;
    case 0x1389: goto L1389;
    case 0x138B: goto L138B;
    case 0x138D: goto L138D;
    case 0x138F: goto L138F;
    case 0x1392: goto L1392;
    case 0x1394: goto L1394;
    case 0x1397: goto L1397;
    case 0x1399: goto L1399;
    case 0x139B: goto L139B;
    case 0x139E: goto L139E;
    case 0x13A0: goto L13A0;
    case 0x13A3: goto L13A3;
    case 0x13A6: goto L13A6;
    case 0x13A8: goto L13A8;
    case 0x13AA: goto L13AA;
    case 0x13AD: goto L13AD;
    case 0x13AF: goto L13AF;
    case 0x13B1: goto L13B1;
    case 0x13B3: goto L13B3;
    case 0x13B5: goto L13B5;
    case 0x13B7: goto L13B7;
    case 0x13B9: goto L13B9;
    case 0x13BB: goto L13BB;
    case 0x13BE: goto L13BE;
    case 0x13C1: goto L13C1;
    case 0x13C4: goto L13C4;
    case 0x13C5: goto L13C5;
    case 0x13C7: goto L13C7;
    case 0x13C8: goto L13C8;
    case 0x13CA: goto L13CA;
    case 0x13CC: goto L13CC;
    case 0x13CE: goto L13CE;
    case 0x13D0: goto L13D0;
    case 0x13D1: goto L13D1;
    case 0x13D2: goto L13D2;
    case 0x13D6: goto L13D6;
    case 0x13D7: goto L13D7;
    case 0x13D9: goto L13D9;
    case 0x13DA: goto L13DA;
    case 0x13DB: goto L13DB;
    case 0x13DC: goto L13DC;
    case 0x13E0: goto L13E0;
    case 0x13E1: goto L13E1;
    case 0x13E3: goto L13E3;
    case 0x13E5: goto L13E5;
    case 0x13E6: goto L13E6;
    case 0x13E7: goto L13E7;
    case 0x13E9: goto L13E9;
    case 0x13EA: goto L13EA;
    case 0x13EB: goto L13EB;
    case 0x13EF: goto L13EF;
    case 0x13F0: goto L13F0;
    case 0x13F1: goto L13F1;
    case 0x13F3: goto L13F3;
    case 0x13F4: goto L13F4;
    case 0x13F5: goto L13F5;
    case 0x13F7: goto L13F7;
    case 0x13F8: goto L13F8;
    case 0x13F9: goto L13F9;
    case 0x13FD: goto L13FD;
    case 0x13FE: goto L13FE;
    case 0x1401: goto L1401;
    case 0x1402: goto L1402;
    case 0x1404: goto L1404;
    case 0x1408: goto L1408;
    case 0x140C: goto L140C;
    case 0x140F: goto L140F;
    case 0x1412: goto L1412;
    case 0x1416: goto L1416;
    case 0x1418: goto L1418;
    case 0x141B: goto L141B;
    case 0x141E: goto L141E;
    case 0x1421: goto L1421;
    case 0x1426: goto L1426;
    case 0x142A: goto L142A;
    case 0x142C: goto L142C;
    case 0x142D: goto L142D;
    case 0x142E: goto L142E;
    case 0x1432: goto L1432;
    case 0x1433: goto L1433;
    case 0x1435: goto L1435;
    case 0x1438: goto L1438;
    case 0x1439: goto L1439;
    case 0x143B: goto L143B;
    case 0x143E: goto L143E;
    case 0x1440: goto L1440;
    case 0x1444: goto L1444;
    case 0x1448: goto L1448;
    case 0x144B: goto L144B;
    case 0x144E: goto L144E;
    case 0x1452: goto L1452;
    case 0x1454: goto L1454;
    case 0x1457: goto L1457;
    case 0x145A: goto L145A;
    case 0x145D: goto L145D;
    case 0x1462: goto L1462;
    case 0x1466: goto L1466;
    case 0x1468: goto L1468;
    case 0x1469: goto L1469;
    case 0x146A: goto L146A;
    case 0x146E: goto L146E;
    case 0x146F: goto L146F;
    case 0x1471: goto L1471;
    case 0x1474: goto L1474;
    case 0x1475: goto L1475;
    case 0x1477: goto L1477;
    case 0x1478: goto L1478;
    case 0x147B: goto L147B;
    case 0x147D: goto L147D;
    case 0x147E: goto L147E;
    case 0x1481: goto L1481;
    case 0x1482: goto L1482;
    case 0x1483: goto L1483;
    case 0x1487: goto L1487;
    case 0x1488: goto L1488;
    case 0x148A: goto L148A;
    case 0x148B: goto L148B;
    case 0x148D: goto L148D;
    case 0x148F: goto L148F;
    case 0x1494: goto L1494;
    case 0x1498: goto L1498;
    case 0x149B: goto L149B;
    case 0x149C: goto L149C;
    case 0x149E: goto L149E;
    case 0x149F: goto L149F;
    case 0x14A1: goto L14A1;
    case 0x14A3: goto L14A3;
    case 0x14A5: goto L14A5;
    case 0x14A7: goto L14A7;
    case 0x14AA: goto L14AA;
    case 0x14AC: goto L14AC;
    case 0x14AD: goto L14AD;
    case 0x14B0: goto L14B0;
    case 0x14B1: goto L14B1;
    case 0x14B2: goto L14B2;
    case 0x14B6: goto L14B6;
    case 0x14B7: goto L14B7;
    case 0x14BA: goto L14BA;
    case 0x14BE: goto L14BE;
    case 0x14C1: goto L14C1;
    case 0x14C2: goto L14C2;
    case 0x14C4: goto L14C4;
    case 0x14C8: goto L14C8;
    case 0x14CC: goto L14CC;
    case 0x14D0: goto L14D0;
    case 0x14D3: goto L14D3;
    case 0x14D7: goto L14D7;
    case 0x14D8: goto L14D8;
    case 0x14D9: goto L14D9;
    case 0x14DD: goto L14DD;
    case 0x14E0: goto L14E0;
    case 0x14E1: goto L14E1;
    case 0x14E3: goto L14E3;
    case 0x14E6: goto L14E6;
    case 0x14E8: goto L14E8;
    case 0x14EC: goto L14EC;
    case 0x14F0: goto L14F0;
    case 0x14F4: goto L14F4;
    case 0x14F7: goto L14F7;
    case 0x14FB: goto L14FB;
    case 0x14FC: goto L14FC;
    case 0x14FD: goto L14FD;
    case 0x1501: goto L1501;
    case 0x1502: goto L1502;
    case 0x1504: goto L1504;
    case 0x1506: goto L1506;
    case 0x150A: goto L150A;
    case 0x150D: goto L150D;
    case 0x150E: goto L150E;
    case 0x150F: goto L150F;
    case 0x1510: goto L1510;
    case 0x1511: goto L1511;
    case 0x1512: goto L1512;
    case 0x1515: goto L1515;
    case 0x1518: goto L1518;
    case 0x151C: goto L151C;
    case 0x151E: goto L151E;
    case 0x151F: goto L151F;
    case 0x1520: goto L1520;
    case 0x1524: goto L1524;
    case 0x1525: goto L1525;
    case 0x1527: goto L1527;
    case 0x1529: goto L1529;
    case 0x152D: goto L152D;
    case 0x1531: goto L1531;
    case 0x1532: goto L1532;
    case 0x1533: goto L1533;
    case 0x1534: goto L1534;
    case 0x1535: goto L1535;
    case 0x1536: goto L1536;
    case 0x153A: goto L153A;
    case 0x153E: goto L153E;
    case 0x1542: goto L1542;
    case 0x1544: goto L1544;
    case 0x1545: goto L1545;
    case 0x1546: goto L1546;
    case 0x154A: goto L154A;
    case 0x154F: goto L154F;
    case 0x1554: goto L1554;
    case 0x1555: goto L1555;
    case 0x1557: goto L1557;
    case 0x1559: goto L1559;
    case 0x155C: goto L155C;
    case 0x155D: goto L155D;
    case 0x155E: goto L155E;
    case 0x1560: goto L1560;
    case 0x1564: goto L1564;
    case 0x1565: goto L1565;
    case 0x1566: goto L1566;
    case 0x1567: goto L1567;
    case 0x1568: goto L1568;
    case 0x156C: goto L156C;
    case 0x1570: goto L1570;
    case 0x1571: goto L1571;
    case 0x1572: goto L1572;
    case 0x1574: goto L1574;
    case 0x1578: goto L1578;
    case 0x157A: goto L157A;
    case 0x157C: goto L157C;
    case 0x1580: goto L1580;
    case 0x1582: goto L1582;
    case 0x1584: goto L1584;
    case 0x1588: goto L1588;
    case 0x158B: goto L158B;
    case 0x158C: goto L158C;
    case 0x158D: goto L158D;
    case 0x158F: goto L158F;
    case 0x1593: goto L1593;
    case 0x1594: goto L1594;
    case 0x1596: goto L1596;
    case 0x1598: goto L1598;
    case 0x1599: goto L1599;
    case 0x159A: goto L159A;
    case 0x159C: goto L159C;
    case 0x159D: goto L159D;
    case 0x159F: goto L159F;
    case 0x15A0: goto L15A0;
    case 0x15A1: goto L15A1;
    case 0x15A3: goto L15A3;
    case 0x15A4: goto L15A4;
    case 0x15A7: goto L15A7;
    case 0x15AB: goto L15AB;
    case 0x15AF: goto L15AF;
    case 0x15B0: goto L15B0;
    case 0x15B1: goto L15B1;
    case 0x15B4: goto L15B4;
    case 0x15B5: goto L15B5;
    case 0x15B7: goto L15B7;
    case 0x15B9: goto L15B9;
    case 0x15BA: goto L15BA;
    case 0x15BB: goto L15BB;
    case 0x15BD: goto L15BD;
    case 0x15BF: goto L15BF;
    case 0x15C0: goto L15C0;
    case 0x15C1: goto L15C1;
    case 0x15C3: goto L15C3;
    case 0x15C4: goto L15C4;
    case 0x15C7: goto L15C7;
    case 0x15CB: goto L15CB;
    case 0x15CF: goto L15CF;
    case 0x15D0: goto L15D0;
    case 0x15D1: goto L15D1;
    case 0x15D2: goto L15D2;
    case 0x15D4: goto L15D4;
    case 0x15D6: goto L15D6;
    case 0x15D7: goto L15D7;
    case 0x15D8: goto L15D8;
    case 0x15DA: goto L15DA;
    case 0x15DE: goto L15DE;
    case 0x15DF: goto L15DF;
    case 0x15E0: goto L15E0;
    case 0x15E1: goto L15E1;
    case 0x15E2: goto L15E2;
    case 0x15E6: goto L15E6;
    case 0x15EA: goto L15EA;
    case 0x15EB: goto L15EB;
    case 0x15EC: goto L15EC;
    case 0x15EE: goto L15EE;
    case 0x15F2: goto L15F2;
    case 0x15F4: goto L15F4;
    case 0x15F6: goto L15F6;
    case 0x15FA: goto L15FA;
    case 0x15FC: goto L15FC;
    case 0x15FE: goto L15FE;
    case 0x1602: goto L1602;
    case 0x1603: goto L1603;
    case 0x1604: goto L1604;
    case 0x1606: goto L1606;
    case 0x160A: goto L160A;
    case 0x160B: goto L160B;
    case 0x160D: goto L160D;
    case 0x160F: goto L160F;
    case 0x1610: goto L1610;
    case 0x1611: goto L1611;
    case 0x1613: goto L1613;
    case 0x1614: goto L1614;
    case 0x1616: goto L1616;
    case 0x1617: goto L1617;
    case 0x1618: goto L1618;
    case 0x161A: goto L161A;
    case 0x161B: goto L161B;
    case 0x161E: goto L161E;
    case 0x1622: goto L1622;
    case 0x1626: goto L1626;
    case 0x1627: goto L1627;
    case 0x1628: goto L1628;
    case 0x162B: goto L162B;
    case 0x162C: goto L162C;
    case 0x162E: goto L162E;
    case 0x1630: goto L1630;
    case 0x1631: goto L1631;
    case 0x1632: goto L1632;
    case 0x1634: goto L1634;
    case 0x1636: goto L1636;
    case 0x1637: goto L1637;
    case 0x1638: goto L1638;
    case 0x163A: goto L163A;
    case 0x163B: goto L163B;
    case 0x163E: goto L163E;
    case 0x1642: goto L1642;
    case 0x1646: goto L1646;
    case 0x1647: goto L1647;
    case 0x1648: goto L1648;
    case 0x164C: goto L164C;
    case 0x164F: goto L164F;
    case 0x1654: goto L1654;
    case 0x1659: goto L1659;
    case 0x165A: goto L165A;
    case 0x165C: goto L165C;
    case 0x165E: goto L165E;
    case 0x1661: goto L1661;
    case 0x1662: goto L1662;
    case 0x1663: goto L1663;
    case 0x1665: goto L1665;
    case 0x1669: goto L1669;
    case 0x166A: goto L166A;
    case 0x166B: goto L166B;
    case 0x166C: goto L166C;
    case 0x166D: goto L166D;
    case 0x1671: goto L1671;
    case 0x1675: goto L1675;
    case 0x1676: goto L1676;
    case 0x1677: goto L1677;
    case 0x1679: goto L1679;
    case 0x167D: goto L167D;
    case 0x167F: goto L167F;
    case 0x1683: goto L1683;
    case 0x1686: goto L1686;
    case 0x1687: goto L1687;
    case 0x1688: goto L1688;
    case 0x168A: goto L168A;
    case 0x168E: goto L168E;
    case 0x168F: goto L168F;
    case 0x1691: goto L1691;
    case 0x1693: goto L1693;
    case 0x1694: goto L1694;
    case 0x1695: goto L1695;
    case 0x1697: goto L1697;
    case 0x1698: goto L1698;
    case 0x1699: goto L1699;
    case 0x169B: goto L169B;
    case 0x169C: goto L169C;
    case 0x169F: goto L169F;
    case 0x16A3: goto L16A3;
    case 0x16A7: goto L16A7;
    case 0x16A8: goto L16A8;
    case 0x16AB: goto L16AB;
    case 0x16AC: goto L16AC;
    case 0x16AE: goto L16AE;
    case 0x16B0: goto L16B0;
    case 0x16B1: goto L16B1;
    case 0x16B2: goto L16B2;
    case 0x16B4: goto L16B4;
    case 0x16B5: goto L16B5;
    case 0x16B6: goto L16B6;
    case 0x16B8: goto L16B8;
    case 0x16B9: goto L16B9;
    case 0x16BC: goto L16BC;
    case 0x16C0: goto L16C0;
    case 0x16C4: goto L16C4;
    case 0x16C5: goto L16C5;
    case 0x16C6: goto L16C6;
    case 0x16C7: goto L16C7;
    case 0x16C9: goto L16C9;
    case 0x16CB: goto L16CB;
    case 0x16CC: goto L16CC;
    case 0x16CD: goto L16CD;
    case 0x16CF: goto L16CF;
    case 0x16D3: goto L16D3;
    case 0x16D4: goto L16D4;
    case 0x16D5: goto L16D5;
    case 0x16D6: goto L16D6;
    case 0x16D7: goto L16D7;
    case 0x16DB: goto L16DB;
    case 0x16DF: goto L16DF;
    case 0x16E0: goto L16E0;
    case 0x16E1: goto L16E1;
    case 0x16E3: goto L16E3;
    case 0x16E7: goto L16E7;
    case 0x16E9: goto L16E9;
    case 0x16ED: goto L16ED;
    case 0x16EE: goto L16EE;
    case 0x16EF: goto L16EF;
    case 0x16F1: goto L16F1;
    case 0x16F5: goto L16F5;
    case 0x16F6: goto L16F6;
    case 0x16F8: goto L16F8;
    case 0x16FA: goto L16FA;
    case 0x16FB: goto L16FB;
    case 0x16FC: goto L16FC;
    case 0x16FE: goto L16FE;
    case 0x16FF: goto L16FF;
    case 0x1700: goto L1700;
    case 0x1702: goto L1702;
    case 0x1703: goto L1703;
    case 0x1706: goto L1706;
    case 0x170A: goto L170A;
    case 0x170E: goto L170E;
    case 0x170F: goto L170F;
    case 0x1712: goto L1712;
    case 0x1713: goto L1713;
    case 0x1715: goto L1715;
    case 0x1717: goto L1717;
    case 0x1718: goto L1718;
    case 0x1719: goto L1719;
    case 0x171B: goto L171B;
    case 0x171C: goto L171C;
    case 0x171D: goto L171D;
    case 0x171F: goto L171F;
    case 0x1720: goto L1720;
    case 0x1723: goto L1723;
    case 0x1727: goto L1727;
    case 0x172B: goto L172B;
    case 0x172C: goto L172C;
    case 0x172D: goto L172D;
    case 0x1731: goto L1731;
    case 0x1733: goto L1733;
    case 0x1734: goto L1734;
    case 0x1736: goto L1736;
    case 0x1739: goto L1739;
    case 0x173B: goto L173B;
    case 0x173D: goto L173D;
    case 0x1740: goto L1740;
    case 0x1741: goto L1741;
    case 0x1742: goto L1742;
    case 0x1744: goto L1744;
    case 0x1745: goto L1745;
    case 0x1746: goto L1746;
    case 0x1747: goto L1747;
    case 0x1749: goto L1749;
    case 0x174B: goto L174B;
    case 0x174D: goto L174D;
    case 0x174E: goto L174E;
    case 0x1750: goto L1750;
    case 0x1752: goto L1752;
    case 0x1754: goto L1754;
    case 0x175A: goto L175A;
    case 0x175B: goto L175B;
    case 0x175C: goto L175C;
    case 0x1760: goto L1760;
    case 0x1764: goto L1764;
    case 0x1768: goto L1768;
    case 0x176C: goto L176C;
    case 0x176F: goto L176F;
    case 0x1773: goto L1773;
    case 0x1779: goto L1779;
    case 0x177B: goto L177B;
    case 0x177E: goto L177E;
    case 0x1784: goto L1784;
    case 0x1789: goto L1789;
    case 0x178B: goto L178B;
    case 0x178E: goto L178E;
    case 0x1793: goto L1793;
    case 0x1795: goto L1795;
    case 0x1796: goto L1796;
    case 0x1799: goto L1799;
    case 0x179B: goto L179B;
    case 0x179E: goto L179E;
    case 0x17A2: goto L17A2;
    case 0x17A6: goto L17A6;
    case 0x17AA: goto L17AA;
    case 0x17AB: goto L17AB;
    case 0x17AE: goto L17AE;
    case 0x17B0: goto L17B0;
    case 0x17B2: goto L17B2;
    case 0x17B5: goto L17B5;
    case 0x17B6: goto L17B6;
    case 0x17B8: goto L17B8;
    case 0x17BC: goto L17BC;
    case 0x17BE: goto L17BE;
    case 0x17C1: goto L17C1;
    case 0x17C2: goto L17C2;
    case 0x17C5: goto L17C5;
    case 0x17C7: goto L17C7;
    case 0x17C9: goto L17C9;
    case 0x17CD: goto L17CD;
    case 0x17CF: goto L17CF;
    case 0x17D1: goto L17D1;
    case 0x17D3: goto L17D3;
    case 0x17D5: goto L17D5;
    case 0x17D9: goto L17D9;
    case 0x17DD: goto L17DD;
    case 0x17E2: goto L17E2;
    case 0x17E4: goto L17E4;
    case 0x17E6: goto L17E6;
    case 0x17E7: goto L17E7;
    case 0x17E8: goto L17E8;
    case 0x17E9: goto L17E9;
    case 0x17ED: goto L17ED;
    case 0x17EF: goto L17EF;
    case 0x17F0: goto L17F0;
    case 0x17F7: goto L17F7;
    case 0x17FA: goto L17FA;
    case 0x17FC: goto L17FC;
    case 0x17FF: goto L17FF;
    case 0x1803: goto L1803;
    case 0x1807: goto L1807;
    case 0x180B: goto L180B;
    case 0x180C: goto L180C;
    case 0x180F: goto L180F;
    case 0x1811: goto L1811;
    case 0x1813: goto L1813;
    case 0x1816: goto L1816;
    case 0x1818: goto L1818;
    case 0x181B: goto L181B;
    case 0x181C: goto L181C;
    case 0x181E: goto L181E;
    case 0x1822: goto L1822;
    case 0x1824: goto L1824;
    case 0x1827: goto L1827;
    case 0x1829: goto L1829;
    case 0x182C: goto L182C;
    case 0x182D: goto L182D;
    case 0x1830: goto L1830;
    case 0x1832: goto L1832;
    case 0x1834: goto L1834;
    case 0x1836: goto L1836;
    case 0x183A: goto L183A;
    case 0x183C: goto L183C;
    case 0x183E: goto L183E;
    case 0x1840: goto L1840;
    case 0x1844: goto L1844;
    case 0x1848: goto L1848;
    case 0x184D: goto L184D;
    case 0x184F: goto L184F;
    case 0x1851: goto L1851;
    case 0x1852: goto L1852;
    case 0x1853: goto L1853;
    case 0x1854: goto L1854;
    case 0x1858: goto L1858;
    case 0x185F: goto L185F;
    case 0x1864: goto L1864;
    case 0x1866: goto L1866;
    case 0x1867: goto L1867;
    case 0x186A: goto L186A;
    case 0x186B: goto L186B;
    case 0x1870: goto L1870;
    case 0x1872: goto L1872;
    case 0x1877: goto L1877;
    case 0x1879: goto L1879;
    case 0x187B: goto L187B;
    case 0x187E: goto L187E;
    case 0x1881: goto L1881;
    case 0x1886: goto L1886;
    case 0x1888: goto L1888;
    case 0x1889: goto L1889;
    case 0x188C: goto L188C;
    case 0x188D: goto L188D;
    case 0x1892: goto L1892;
    case 0x1894: goto L1894;
    case 0x1899: goto L1899;
    case 0x189B: goto L189B;
    case 0x189D: goto L189D;
    case 0x18A0: goto L18A0;
    case 0x18A3: goto L18A3;
    case 0x18A8: goto L18A8;
    case 0x18AA: goto L18AA;
    case 0x18AB: goto L18AB;
    case 0x18AE: goto L18AE;
    case 0x18AF: goto L18AF;
    case 0x18B4: goto L18B4;
    case 0x18B6: goto L18B6;
    case 0x18BB: goto L18BB;
    case 0x18BD: goto L18BD;
    case 0x18BF: goto L18BF;
    case 0x18C2: goto L18C2;
    case 0x18C5: goto L18C5;
    case 0x18CA: goto L18CA;
    case 0x18CC: goto L18CC;
    case 0x18CD: goto L18CD;
    case 0x18D0: goto L18D0;
    case 0x18D1: goto L18D1;
    case 0x18D6: goto L18D6;
    case 0x18D8: goto L18D8;
    case 0x18DD: goto L18DD;
    case 0x18DF: goto L18DF;
    case 0x18E1: goto L18E1;
    case 0x18E4: goto L18E4;
    case 0x18E7: goto L18E7;
    case 0x18E8: goto L18E8;
    case 0x18E9: goto L18E9;
    case 0x18ED: goto L18ED;
    case 0x18EE: goto L18EE;
    case 0x18F0: goto L18F0;
    case 0x18F1: goto L18F1;
    case 0x18F2: goto L18F2;
    case 0x18F6: goto L18F6;
    case 0x18F7: goto L18F7;
    case 0x18F8: goto L18F8;
    case 0x18FA: goto L18FA;
    case 0x18FB: goto L18FB;
    case 0x18FC: goto L18FC;
    case 0x1900: goto L1900;
    case 0x1901: goto L1901;
    case 0x1902: goto L1902;
    case 0x1903: goto L1903;
    case 0x1907: goto L1907;
    case 0x1908: goto L1908;
    case 0x190A: goto L190A;
    case 0x190B: goto L190B;
    case 0x190D: goto L190D;
    case 0x190E: goto L190E;
    case 0x1910: goto L1910;
    case 0x1912: goto L1912;
    case 0x1914: goto L1914;
    case 0x1915: goto L1915;
    case 0x1916: goto L1916;
    case 0x191A: goto L191A;
    case 0x191B: goto L191B;
    case 0x191D: goto L191D;
    case 0x191E: goto L191E;
    case 0x1920: goto L1920;
    case 0x1921: goto L1921;
    case 0x1923: goto L1923;
    case 0x1925: goto L1925;
    case 0x1927: goto L1927;
    case 0x1928: goto L1928;
    case 0x1929: goto L1929;
    case 0x192D: goto L192D;
    case 0x192E: goto L192E;
    case 0x192F: goto L192F;
    case 0x1930: goto L1930;
    case 0x1932: goto L1932;
    case 0x1934: goto L1934;
    case 0x1935: goto L1935;
    case 0x1936: goto L1936;
    case 0x1938: goto L1938;
    case 0x1939: goto L1939;
    case 0x193A: goto L193A;
    case 0x193E: goto L193E;
    case 0x193F: goto L193F;
    case 0x1940: goto L1940;
    case 0x1941: goto L1941;
    case 0x1943: goto L1943;
    case 0x1944: goto L1944;
    case 0x1945: goto L1945;
    case 0x1949: goto L1949;
    case 0x194A: goto L194A;
    case 0x194B: goto L194B;
    case 0x194C: goto L194C;
    case 0x1950: goto L1950;
    case 0x1951: goto L1951;
    case 0x1953: goto L1953;
    case 0x1955: goto L1955;
    case 0x1956: goto L1956;
    case 0x1957: goto L1957;
    case 0x1958: goto L1958;
    case 0x195A: goto L195A;
    case 0x195B: goto L195B;
    case 0x195C: goto L195C;
    case 0x1960: goto L1960;
    case 0x1961: goto L1961;
    case 0x1962: goto L1962;
    case 0x1963: goto L1963;
    case 0x1967: goto L1967;
    case 0x1968: goto L1968;
    case 0x1969: goto L1969;
    case 0x196A: goto L196A;
    case 0x196E: goto L196E;
    case 0x196F: goto L196F;
    case 0x1971: goto L1971;
    case 0x1972: goto L1972;
    case 0x1976: goto L1976;
    case 0x1978: goto L1978;
    case 0x197A: goto L197A;
    case 0x197C: goto L197C;
    case 0x197D: goto L197D;
    case 0x197F: goto L197F;
    case 0x1980: goto L1980;
    case 0x1984: goto L1984;
    case 0x1986: goto L1986;
    case 0x1988: goto L1988;
    case 0x198A: goto L198A;
    case 0x198B: goto L198B;
    case 0x198D: goto L198D;
    case 0x198E: goto L198E;
    case 0x1992: goto L1992;
    case 0x1994: goto L1994;
    case 0x1996: goto L1996;
    case 0x1998: goto L1998;
    case 0x199A: goto L199A;
    case 0x199B: goto L199B;
    case 0x199C: goto L199C;
    case 0x199E: goto L199E;
    case 0x199F: goto L199F;
    case 0x19A0: goto L19A0;
    case 0x19A4: goto L19A4;
    case 0x19A5: goto L19A5;
    case 0x19A6: goto L19A6;
    case 0x19A7: goto L19A7;
    case 0x19A9: goto L19A9;
    case 0x19AA: goto L19AA;
    case 0x19AB: goto L19AB;
    case 0x19AF: goto L19AF;
    case 0x19B0: goto L19B0;
    case 0x19B1: goto L19B1;
    case 0x19B2: goto L19B2;
    case 0x19B6: goto L19B6;
    case 0x19B7: goto L19B7;
    case 0x19B9: goto L19B9;
    case 0x19BA: goto L19BA;
    case 0x19BE: goto L19BE;
    case 0x19C0: goto L19C0;
    case 0x19C2: goto L19C2;
    case 0x19C4: goto L19C4;
    case 0x19C5: goto L19C5;
    case 0x19C7: goto L19C7;
    case 0x19C8: goto L19C8;
    case 0x19CC: goto L19CC;
    case 0x19CE: goto L19CE;
    case 0x19D0: goto L19D0;
    case 0x19D2: goto L19D2;
    case 0x19D4: goto L19D4;
    case 0x19D7: goto L19D7;
    case 0x19D8: goto L19D8;
    case 0x19D9: goto L19D9;
    case 0x19DB: goto L19DB;
    case 0x19DC: goto L19DC;
    case 0x19DD: goto L19DD;
    case 0x19E1: goto L19E1;
    case 0x19E2: goto L19E2;
    case 0x19E3: goto L19E3;
    case 0x19E4: goto L19E4;
    case 0x19E6: goto L19E6;
    case 0x19E7: goto L19E7;
    case 0x19E8: goto L19E8;
    case 0x19EC: goto L19EC;
    case 0x19ED: goto L19ED;
    case 0x19EE: goto L19EE;
    case 0x19EF: goto L19EF;
    case 0x19F3: goto L19F3;
    case 0x19F4: goto L19F4;
    case 0x19F6: goto L19F6;
    case 0x19F7: goto L19F7;
    case 0x19FB: goto L19FB;
    case 0x19FD: goto L19FD;
    case 0x19FF: goto L19FF;
    case 0x1A01: goto L1A01;
    case 0x1A02: goto L1A02;
    case 0x1A04: goto L1A04;
    case 0x1A05: goto L1A05;
    case 0x1A09: goto L1A09;
    case 0x1A0B: goto L1A0B;
    case 0x1A0D: goto L1A0D;
    case 0x1A0F: goto L1A0F;
    case 0x1A11: goto L1A11;
    case 0x1A14: goto L1A14;
    case 0x1A15: goto L1A15;
    case 0x1A16: goto L1A16;
    case 0x1A18: goto L1A18;
    case 0x1A19: goto L1A19;
    case 0x1A1A: goto L1A1A;
    case 0x1A1E: goto L1A1E;
    case 0x1A1F: goto L1A1F;
    case 0x1A20: goto L1A20;
    case 0x1A21: goto L1A21;
    case 0x1A23: goto L1A23;
    case 0x1A24: goto L1A24;
    case 0x1A25: goto L1A25;
    case 0x1A29: goto L1A29;
    case 0x1A2A: goto L1A2A;
    case 0x1A2B: goto L1A2B;
    case 0x1A2C: goto L1A2C;
    case 0x1A30: goto L1A30;
    case 0x1A31: goto L1A31;
    case 0x1A33: goto L1A33;
    case 0x1A34: goto L1A34;
    case 0x1A38: goto L1A38;
    case 0x1A3A: goto L1A3A;
    case 0x1A3C: goto L1A3C;
    case 0x1A3E: goto L1A3E;
    case 0x1A3F: goto L1A3F;
    case 0x1A41: goto L1A41;
    case 0x1A42: goto L1A42;
    case 0x1A46: goto L1A46;
    case 0x1A48: goto L1A48;
    case 0x1A4A: goto L1A4A;
    case 0x1A4C: goto L1A4C;
    case 0x1A4E: goto L1A4E;
    case 0x1A51: goto L1A51;
    case 0x1A52: goto L1A52;
    case 0x1A53: goto L1A53;
    case 0x1A55: goto L1A55;
    case 0x1A56: goto L1A56;
    case 0x1A57: goto L1A57;
    case 0x1A5B: goto L1A5B;
    case 0x1A5C: goto L1A5C;
    case 0x1A5D: goto L1A5D;
    case 0x1A5E: goto L1A5E;
    case 0x1A60: goto L1A60;
    case 0x1A61: goto L1A61;
    case 0x1A62: goto L1A62;
    case 0x1A66: goto L1A66;
    case 0x1A67: goto L1A67;
    case 0x1A68: goto L1A68;
    case 0x1A69: goto L1A69;
    case 0x1A6D: goto L1A6D;
    case 0x1A73: goto L1A73;
    case 0x1A74: goto L1A74;
    case 0x1A78: goto L1A78;
    case 0x1A7A: goto L1A7A;
    case 0x1A7B: goto L1A7B;
    case 0x1A7C: goto L1A7C;
    case 0x1A80: goto L1A80;
    case 0x1A81: goto L1A81;
    case 0x1A83: goto L1A83;
    case 0x1A85: goto L1A85;
    case 0x1A87: goto L1A87;
    case 0x1A8A: goto L1A8A;
    case 0x1A8B: goto L1A8B;
    case 0x1A8F: goto L1A8F;
    case 0x1A91: goto L1A91;
    case 0x1A92: goto L1A92;
    case 0x1A93: goto L1A93;
    case 0x1A97: goto L1A97;
    case 0x1A98: goto L1A98;
    case 0x1A9A: goto L1A9A;
    case 0x1A9C: goto L1A9C;
    case 0x1A9E: goto L1A9E;
    case 0x1AA1: goto L1AA1;
    case 0x1AA2: goto L1AA2;
    case 0x1AA6: goto L1AA6;
    case 0x1AA8: goto L1AA8;
    case 0x1AA9: goto L1AA9;
    case 0x1AAA: goto L1AAA;
    case 0x1AAE: goto L1AAE;
    case 0x1AAF: goto L1AAF;
    case 0x1AB1: goto L1AB1;
    case 0x1AB3: goto L1AB3;
    case 0x1AB5: goto L1AB5;
    case 0x1AB8: goto L1AB8;
    case 0x1ABB: goto L1ABB;
    case 0x1ABC: goto L1ABC;
    case 0x1ABD: goto L1ABD;
    case 0x1AC1: goto L1AC1;
    case 0x1AC7: goto L1AC7;
    case 0x1AC8: goto L1AC8;
    case 0x1AC9: goto L1AC9;
    case 0x1ACD: goto L1ACD;
    case 0x1ACE: goto L1ACE;
    case 0x1AD0: goto L1AD0;
    case 0x1AD1: goto L1AD1;
    case 0x1AD4: goto L1AD4;
    case 0x1AD5: goto L1AD5;
    case 0x1AD7: goto L1AD7;
    case 0x1AD9: goto L1AD9;
    case 0x1ADB: goto L1ADB;
    case 0x1ADC: goto L1ADC;
    case 0x1ADE: goto L1ADE;
    case 0x1AE0: goto L1AE0;
    case 0x1AE2: goto L1AE2;
    case 0x1AE4: goto L1AE4;
    case 0x1AE6: goto L1AE6;
    case 0x1AE9: goto L1AE9;
    case 0x1AEB: goto L1AEB;
    case 0x1AED: goto L1AED;
    case 0x1AEF: goto L1AEF;
    case 0x1AF2: goto L1AF2;
    case 0x1AF3: goto L1AF3;
    case 0x1AF4: goto L1AF4;
    case 0x1AF8: goto L1AF8;
    case 0x1AF9: goto L1AF9;
    case 0x1AFB: goto L1AFB;
    case 0x1AFC: goto L1AFC;
    case 0x1B00: goto L1B00;
    case 0x1B02: goto L1B02;
    case 0x1B04: goto L1B04;
    case 0x1B06: goto L1B06;
    case 0x1B07: goto L1B07;
    case 0x1B09: goto L1B09;
    case 0x1B0C: goto L1B0C;
    case 0x1B0E: goto L1B0E;
    case 0x1B10: goto L1B10;
    case 0x1B11: goto L1B11;
    case 0x1B12: goto L1B12;
    case 0x1B14: goto L1B14;
    case 0x1B15: goto L1B15;
    case 0x1B17: goto L1B17;
    case 0x1B18: goto L1B18;
    case 0x1B19: goto L1B19;
    case 0x1B1B: goto L1B1B;
    case 0x1B1C: goto L1B1C;
    case 0x1B1E: goto L1B1E;
    case 0x1B20: goto L1B20;
    case 0x1B22: goto L1B22;
    case 0x1B24: goto L1B24;
    case 0x1B26: goto L1B26;
    case 0x1B28: goto L1B28;
    case 0x1B29: goto L1B29;
    case 0x1B2B: goto L1B2B;
    case 0x1B2C: goto L1B2C;
    case 0x1B2D: goto L1B2D;
    case 0x1B31: goto L1B31;
    case 0x1B33: goto L1B33;
    case 0x1B34: goto L1B34;
    case 0x1B36: goto L1B36;
    case 0x1B37: goto L1B37;
    case 0x1B38: goto L1B38;
    case 0x1B3C: goto L1B3C;
    case 0x1B3E: goto L1B3E;
    case 0x1B40: goto L1B40;
    case 0x1B42: goto L1B42;
    case 0x1B43: goto L1B43;
    case 0x1B45: goto L1B45;
    case 0x1B46: goto L1B46;
    case 0x1B47: goto L1B47;
    case 0x1B4B: goto L1B4B;
    case 0x1B4C: goto L1B4C;
    case 0x1B4E: goto L1B4E;
    case 0x1B4F: goto L1B4F;
    case 0x1B53: goto L1B53;
    case 0x1B55: goto L1B55;
    case 0x1B59: goto L1B59;
    case 0x1B5D: goto L1B5D;
    case 0x1B61: goto L1B61;
    case 0x1B63: goto L1B63;
    case 0x1B67: goto L1B67;
    case 0x1B69: goto L1B69;
    case 0x1B6C: goto L1B6C;
    case 0x1B6E: goto L1B6E;
    case 0x1B70: goto L1B70;
    case 0x1B73: goto L1B73;
    case 0x1B75: goto L1B75;
    case 0x1B77: goto L1B77;
    case 0x1B78: goto L1B78;
    case 0x1B7A: goto L1B7A;
    case 0x1B7E: goto L1B7E;
    case 0x1B82: goto L1B82;
    case 0x1B86: goto L1B86;
    case 0x1B89: goto L1B89;
    case 0x1B8D: goto L1B8D;
    case 0x1B8E: goto L1B8E;
    case 0x1B8F: goto L1B8F;
    case 0x1B93: goto L1B93;
    case 0x1B96: goto L1B96;
    case 0x1B98: goto L1B98;
    case 0x1B9B: goto L1B9B;
    case 0x1B9D: goto L1B9D;
    case 0x1BA0: goto L1BA0;
    case 0x1BA1: goto L1BA1;
    case 0x1BA3: goto L1BA3;
    case 0x1BA6: goto L1BA6;
    case 0x1BA8: goto L1BA8;
    case 0x1BAA: goto L1BAA;
    case 0x1BAC: goto L1BAC;
    case 0x1BAF: goto L1BAF;
    case 0x1BB4: goto L1BB4;
    case 0x1BB6: goto L1BB6;
    case 0x1BB8: goto L1BB8;
    case 0x1BBA: goto L1BBA;
    case 0x1BBC: goto L1BBC;
    case 0x1BBE: goto L1BBE;
    case 0x1BC0: goto L1BC0;
    case 0x1BC3: goto L1BC3;
    case 0x1BC5: goto L1BC5;
    case 0x1BC8: goto L1BC8;
    case 0x1BCD: goto L1BCD;
    case 0x1BCF: goto L1BCF;
    case 0x1BD1: goto L1BD1;
    case 0x1BD3: goto L1BD3;
    case 0x1BD5: goto L1BD5;
    case 0x1BD7: goto L1BD7;
    case 0x1BD9: goto L1BD9;
    case 0x1BDC: goto L1BDC;
    case 0x1BE0: goto L1BE0;
    case 0x1BE4: goto L1BE4;
    case 0x1BE8: goto L1BE8;
    case 0x1BE9: goto L1BE9;
    case 0x1BED: goto L1BED;
    case 0x1BEE: goto L1BEE;
    case 0x1BF2: goto L1BF2;
    case 0x1BF3: goto L1BF3;
    case 0x1BF7: goto L1BF7;
    case 0x1BF8: goto L1BF8;
    case 0x1BFB: goto L1BFB;
    case 0x1BFD: goto L1BFD;
    case 0x1BFF: goto L1BFF;
    case 0x1C03: goto L1C03;
    case 0x1C07: goto L1C07;
    case 0x1C0B: goto L1C0B;
    case 0x1C0F: goto L1C0F;
    case 0x1C13: goto L1C13;
    case 0x1C17: goto L1C17;
    case 0x1C1B: goto L1C1B;
    case 0x1C1F: goto L1C1F;
    case 0x1C23: goto L1C23;
    case 0x1C25: goto L1C25;
    case 0x1C26: goto L1C26;
    case 0x1C29: goto L1C29;
    case 0x1C2A: goto L1C2A;
    case 0x1C2D: goto L1C2D;
    case 0x1C2E: goto L1C2E;
    case 0x1C2F: goto L1C2F;
    case 0x1C33: goto L1C33;
    case 0x1C37: goto L1C37;
    case 0x1C3B: goto L1C3B;
    case 0x1C3F: goto L1C3F;
    case 0x1C43: goto L1C43;
    case 0x1C47: goto L1C47;
    case 0x1C4B: goto L1C4B;
    case 0x1C4F: goto L1C4F;
    case 0x1C53: goto L1C53;
    case 0x1C57: goto L1C57;
    case 0x1C58: goto L1C58;
    case 0x1C5C: goto L1C5C;
    case 0x1C60: goto L1C60;
    case 0x1C64: goto L1C64;
    case 0x1C67: goto L1C67;
    case 0x1C68: goto L1C68;
    case 0x1C69: goto L1C69;
    case 0x1C6D: goto L1C6D;
    case 0x1C70: goto L1C70;
    case 0x1C71: goto L1C71;
    case 0x1C72: goto L1C72;
    case 0x1C76: goto L1C76;
    case 0x1C77: goto L1C77;
    case 0x1C7B: goto L1C7B;
    case 0x1C7F: goto L1C7F;
    case 0x1C83: goto L1C83;
    case 0x1C86: goto L1C86;
    case 0x1C87: goto L1C87;
    case 0x1C88: goto L1C88;
    case 0x1C8C: goto L1C8C;
    case 0x1C8D: goto L1C8D;
    case 0x1C8F: goto L1C8F;
    case 0x1C90: goto L1C90;
    case 0x1C91: goto L1C91;
    case 0x1C95: goto L1C95;
    case 0x1C99: goto L1C99;
    case 0x1C9A: goto L1C9A;
    case 0x1C9B: goto L1C9B;
    case 0x1C9C: goto L1C9C;
    case 0x1C9D: goto L1C9D;
    case 0x1C9F: goto L1C9F;
    case 0x1CA0: goto L1CA0;
    case 0x1CA1: goto L1CA1;
    case 0x1CA5: goto L1CA5;
    case 0x1CA6: goto L1CA6;
    case 0x1CA8: goto L1CA8;
    case 0x1CA9: goto L1CA9;
    case 0x1CAD: goto L1CAD;
    case 0x1CAF: goto L1CAF;
    case 0x1CB3: goto L1CB3;
    case 0x1CB7: goto L1CB7;
    case 0x1CBB: goto L1CBB;
    case 0x1CBD: goto L1CBD;
    case 0x1CC1: goto L1CC1;
    case 0x1CC3: goto L1CC3;
    case 0x1CC5: goto L1CC5;
    case 0x1CC9: goto L1CC9;
    case 0x1CCA: goto L1CCA;
    case 0x1CCC: goto L1CCC;
    case 0x1CD0: goto L1CD0;
    case 0x1CD4: goto L1CD4;
    case 0x1CD6: goto L1CD6;
    case 0x1CD8: goto L1CD8;
    case 0x1CDC: goto L1CDC;
    case 0x1CDE: goto L1CDE;
    case 0x1CE0: goto L1CE0;
    case 0x1CE2: goto L1CE2;
    case 0x1CE4: goto L1CE4;
    case 0x1CE5: goto L1CE5;
    case 0x1CE6: goto L1CE6;
    case 0x1CE8: goto L1CE8;
    case 0x1CEA: goto L1CEA;
    case 0x1CEC: goto L1CEC;
    case 0x1CEE: goto L1CEE;
    case 0x1CF0: goto L1CF0;
    case 0x1CF1: goto L1CF1;
    case 0x1CF3: goto L1CF3;
    case 0x1CF5: goto L1CF5;
    case 0x1CF7: goto L1CF7;
    case 0x1CFB: goto L1CFB;
    case 0x1CFC: goto L1CFC;
    case 0x1CFD: goto L1CFD;
    case 0x1D01: goto L1D01;
    case 0x1D02: goto L1D02;
    case 0x1D04: goto L1D04;
    case 0x1D05: goto L1D05;
    case 0x1D09: goto L1D09;
    case 0x1D0B: goto L1D0B;
    case 0x1D0F: goto L1D0F;
    case 0x1D13: goto L1D13;
    case 0x1D17: goto L1D17;
    case 0x1D19: goto L1D19;
    case 0x1D1D: goto L1D1D;
    case 0x1D1F: goto L1D1F;
    case 0x1D22: goto L1D22;
    case 0x1D24: goto L1D24;
    case 0x1D26: goto L1D26;
    case 0x1D29: goto L1D29;
    case 0x1D2B: goto L1D2B;
    case 0x1D2D: goto L1D2D;
    case 0x1D2E: goto L1D2E;
    case 0x1D30: goto L1D30;
    case 0x1D34: goto L1D34;
    case 0x1D38: goto L1D38;
    case 0x1D3C: goto L1D3C;
    case 0x1D3F: goto L1D3F;
    case 0x1D43: goto L1D43;
    case 0x1D44: goto L1D44;
    case 0x1D45: goto L1D45;
    case 0x1D49: goto L1D49;
    case 0x1D4A: goto L1D4A;
    case 0x1D4C: goto L1D4C;
    case 0x1D4D: goto L1D4D;
    case 0x1D51: goto L1D51;
    case 0x1D53: goto L1D53;
    case 0x1D57: goto L1D57;
    case 0x1D5B: goto L1D5B;
    case 0x1D5F: goto L1D5F;
    case 0x1D61: goto L1D61;
    case 0x1D65: goto L1D65;
    case 0x1D67: goto L1D67;
    case 0x1D6A: goto L1D6A;
    case 0x1D6C: goto L1D6C;
    case 0x1D6D: goto L1D6D;
    case 0x1D6F: goto L1D6F;
    case 0x1D73: goto L1D73;
    case 0x1D77: goto L1D77;
    case 0x1D79: goto L1D79;
    case 0x1D7B: goto L1D7B;
    case 0x1D7F: goto L1D7F;
    case 0x1D81: goto L1D81;
    case 0x1D83: goto L1D83;
    case 0x1D85: goto L1D85;
    case 0x1D87: goto L1D87;
    case 0x1D88: goto L1D88;
    case 0x1D89: goto L1D89;
    case 0x1D8B: goto L1D8B;
    case 0x1D8D: goto L1D8D;
    case 0x1D8F: goto L1D8F;
    case 0x1D91: goto L1D91;
    case 0x1D93: goto L1D93;
    case 0x1D94: goto L1D94;
    case 0x1D96: goto L1D96;
    case 0x1D98: goto L1D98;
    case 0x1D9A: goto L1D9A;
    case 0x1D9E: goto L1D9E;
    case 0x1D9F: goto L1D9F;
    case 0x1DA0: goto L1DA0;
    case 0x1DA4: goto L1DA4;
    case 0x1DA5: goto L1DA5;
    case 0x1DA7: goto L1DA7;
    case 0x1DA8: goto L1DA8;
    case 0x1DAC: goto L1DAC;
    case 0x1DAE: goto L1DAE;
    case 0x1DB2: goto L1DB2;
    case 0x1DB6: goto L1DB6;
    case 0x1DBA: goto L1DBA;
    case 0x1DBC: goto L1DBC;
    case 0x1DC0: goto L1DC0;
    case 0x1DC2: goto L1DC2;
    case 0x1DC5: goto L1DC5;
    case 0x1DC7: goto L1DC7;
    case 0x1DC9: goto L1DC9;
    case 0x1DCC: goto L1DCC;
    case 0x1DCE: goto L1DCE;
    case 0x1DD0: goto L1DD0;
    case 0x1DD1: goto L1DD1;
    case 0x1DD3: goto L1DD3;
    case 0x1DD7: goto L1DD7;
    case 0x1DDB: goto L1DDB;
    case 0x1DDF: goto L1DDF;
    case 0x1DE2: goto L1DE2;
    case 0x1DE6: goto L1DE6;
    case 0x1DE7: goto L1DE7;
    case 0x1DE8: goto L1DE8;
    case 0x1DEC: goto L1DEC;
    case 0x1DF0: goto L1DF0;
    case 0x1DF1: goto L1DF1;
    case 0x1DF3: goto L1DF3;
    case 0x1DF5: goto L1DF5;
    case 0x1DF9: goto L1DF9;
    case 0x1DFB: goto L1DFB;
    case 0x1DFE: goto L1DFE;
    case 0x1E00: goto L1E00;
    case 0x1E02: goto L1E02;
    case 0x1E05: goto L1E05;
    case 0x1E07: goto L1E07;
    case 0x1E08: goto L1E08;
    case 0x1E0A: goto L1E0A;
    case 0x1E0C: goto L1E0C;
    case 0x1E0E: goto L1E0E;
    case 0x1E12: goto L1E12;
    case 0x1E14: goto L1E14;
    case 0x1E17: goto L1E17;
    case 0x1E19: goto L1E19;
    case 0x1E1B: goto L1E1B;
    case 0x1E1E: goto L1E1E;
    case 0x1E20: goto L1E20;
    case 0x1E22: goto L1E22;
    case 0x1E23: goto L1E23;
    case 0x1E25: goto L1E25;
    case 0x1E29: goto L1E29;
    case 0x1E2D: goto L1E2D;
    case 0x1E31: goto L1E31;
    case 0x1E32: goto L1E32;
    case 0x1E34: goto L1E34;
    case 0x1E38: goto L1E38;
    case 0x1E3C: goto L1E3C;
    case 0x1E40: goto L1E40;
    case 0x1E43: goto L1E43;
    case 0x1E47: goto L1E47;
    case 0x1E48: goto L1E48;
    case 0x1E49: goto L1E49;
    case 0x1E4D: goto L1E4D;
    case 0x1E51: goto L1E51;
    case 0x1E52: goto L1E52;
    case 0x1E54: goto L1E54;
    case 0x1E56: goto L1E56;
    case 0x1E5A: goto L1E5A;
    case 0x1E5C: goto L1E5C;
    case 0x1E5F: goto L1E5F;
    case 0x1E61: goto L1E61;
    case 0x1E63: goto L1E63;
    case 0x1E66: goto L1E66;
    case 0x1E68: goto L1E68;
    case 0x1E69: goto L1E69;
    case 0x1E6B: goto L1E6B;
    case 0x1E6D: goto L1E6D;
    case 0x1E6F: goto L1E6F;
    case 0x1E73: goto L1E73;
    case 0x1E75: goto L1E75;
    case 0x1E78: goto L1E78;
    case 0x1E7A: goto L1E7A;
    case 0x1E7C: goto L1E7C;
    case 0x1E7F: goto L1E7F;
    case 0x1E81: goto L1E81;
    case 0x1E83: goto L1E83;
    case 0x1E84: goto L1E84;
    case 0x1E86: goto L1E86;
    case 0x1E8A: goto L1E8A;
    case 0x1E8E: goto L1E8E;
    case 0x1E92: goto L1E92;
    case 0x1E93: goto L1E93;
    case 0x1E95: goto L1E95;
    case 0x1E99: goto L1E99;
    case 0x1E9D: goto L1E9D;
    case 0x1EA1: goto L1EA1;
    case 0x1EA4: goto L1EA4;
    case 0x1EA8: goto L1EA8;
    case 0x1EA9: goto L1EA9;
    case 0x1EAA: goto L1EAA;
    case 0x1EAE: goto L1EAE;
    case 0x1EB2: goto L1EB2;
    case 0x1EB3: goto L1EB3;
    case 0x1EB5: goto L1EB5;
    case 0x1EB7: goto L1EB7;
    case 0x1EBB: goto L1EBB;
    case 0x1EBD: goto L1EBD;
    case 0x1EC0: goto L1EC0;
    case 0x1EC2: goto L1EC2;
    case 0x1EC4: goto L1EC4;
    case 0x1EC7: goto L1EC7;
    case 0x1EC9: goto L1EC9;
    case 0x1ECA: goto L1ECA;
    case 0x1ECC: goto L1ECC;
    case 0x1ECE: goto L1ECE;
    case 0x1ED0: goto L1ED0;
    case 0x1ED4: goto L1ED4;
    case 0x1ED6: goto L1ED6;
    case 0x1ED9: goto L1ED9;
    case 0x1EDB: goto L1EDB;
    case 0x1EDD: goto L1EDD;
    case 0x1EE0: goto L1EE0;
    case 0x1EE2: goto L1EE2;
    case 0x1EE4: goto L1EE4;
    case 0x1EE5: goto L1EE5;
    case 0x1EE7: goto L1EE7;
    case 0x1EEB: goto L1EEB;
    case 0x1EEF: goto L1EEF;
    case 0x1EF3: goto L1EF3;
    case 0x1EF4: goto L1EF4;
    case 0x1EF6: goto L1EF6;
    case 0x1EFA: goto L1EFA;
    case 0x1EFE: goto L1EFE;
    case 0x1F02: goto L1F02;
    case 0x1F05: goto L1F05;
    case 0x1F09: goto L1F09;
    case 0x1F0A: goto L1F0A;
    case 0x1F0B: goto L1F0B;
    case 0x1F0F: goto L1F0F;
    case 0x1F13: goto L1F13;
    case 0x1F14: goto L1F14;
    case 0x1F16: goto L1F16;
    case 0x1F18: goto L1F18;
    case 0x1F1C: goto L1F1C;
    case 0x1F1E: goto L1F1E;
    case 0x1F21: goto L1F21;
    case 0x1F23: goto L1F23;
    case 0x1F25: goto L1F25;
    case 0x1F26: goto L1F26;
    case 0x1F28: goto L1F28;
    case 0x1F2A: goto L1F2A;
    case 0x1F2E: goto L1F2E;
    case 0x1F30: goto L1F30;
    case 0x1F33: goto L1F33;
    case 0x1F35: goto L1F35;
    case 0x1F37: goto L1F37;
    case 0x1F38: goto L1F38;
    case 0x1F3A: goto L1F3A;
    case 0x1F3E: goto L1F3E;
    case 0x1F42: goto L1F42;
    case 0x1F46: goto L1F46;
    case 0x1F47: goto L1F47;
    case 0x1F49: goto L1F49;
    case 0x1F4D: goto L1F4D;
    case 0x1F51: goto L1F51;
    case 0x1F55: goto L1F55;
    case 0x1F58: goto L1F58;
    case 0x1F5C: goto L1F5C;
    case 0x1F5D: goto L1F5D;
    case 0x1F5E: goto L1F5E;
    case 0x1F62: goto L1F62;
    case 0x1F66: goto L1F66;
    case 0x1F67: goto L1F67;
    case 0x1F69: goto L1F69;
    case 0x1F6B: goto L1F6B;
    case 0x1F6F: goto L1F6F;
    case 0x1F71: goto L1F71;
    case 0x1F74: goto L1F74;
    case 0x1F76: goto L1F76;
    case 0x1F78: goto L1F78;
    case 0x1F79: goto L1F79;
    case 0x1F7B: goto L1F7B;
    case 0x1F7D: goto L1F7D;
    case 0x1F7F: goto L1F7F;
    case 0x1F80: goto L1F80;
    case 0x1F82: goto L1F82;
    case 0x1F86: goto L1F86;
    case 0x1F8A: goto L1F8A;
    case 0x1F8E: goto L1F8E;
    case 0x1F8F: goto L1F8F;
    case 0x1F91: goto L1F91;
    case 0x1F95: goto L1F95;
    case 0x1F99: goto L1F99;
    case 0x1F9D: goto L1F9D;
    case 0x1FA0: goto L1FA0;
    case 0x1FA4: goto L1FA4;
    case 0x1FA5: goto L1FA5;
    case 0x1FA6: goto L1FA6;
    case 0x1FAA: goto L1FAA;
    case 0x1FAE: goto L1FAE;
    case 0x1FAF: goto L1FAF;
    case 0x1FB1: goto L1FB1;
    case 0x1FB3: goto L1FB3;
    case 0x1FB7: goto L1FB7;
    case 0x1FB9: goto L1FB9;
    case 0x1FBC: goto L1FBC;
    case 0x1FBE: goto L1FBE;
    case 0x1FC0: goto L1FC0;
    case 0x1FC1: goto L1FC1;
    case 0x1FC3: goto L1FC3;
    case 0x1FC5: goto L1FC5;
    case 0x1FC7: goto L1FC7;
    case 0x1FC8: goto L1FC8;
    case 0x1FCA: goto L1FCA;
    case 0x1FCE: goto L1FCE;
    case 0x1FD2: goto L1FD2;
    case 0x1FD6: goto L1FD6;
    case 0x1FD7: goto L1FD7;
    case 0x1FD9: goto L1FD9;
    case 0x1FDD: goto L1FDD;
    case 0x1FE1: goto L1FE1;
    case 0x1FE5: goto L1FE5;
    case 0x1FE8: goto L1FE8;
    case 0x1FEC: goto L1FEC;
    case 0x1FED: goto L1FED;
    case 0x1FEE: goto L1FEE;
    case 0x1FF2: goto L1FF2;
    case 0x1FF3: goto L1FF3;
    case 0x1FF6: goto L1FF6;
    case 0x1FFA: goto L1FFA;
    case 0x1FFD: goto L1FFD;
    case 0x2001: goto L2001;
    case 0x2004: goto L2004;
    case 0x2006: goto L2006;
    case 0x200B: goto L200B;
    case 0x200E: goto L200E;
    case 0x2010: goto L2010;
    case 0x2011: goto L2011;
    case 0x2012: goto L2012;
    case 0x2016: goto L2016;
    case 0x2017: goto L2017;
    case 0x2019: goto L2019;
    case 0x201A: goto L201A;
    case 0x201C: goto L201C;
    case 0x201E: goto L201E;
    case 0x201F: goto L201F;
    case 0x2021: goto L2021;
    case 0x2023: goto L2023;
    case 0x2024: goto L2024;
    case 0x2025: goto L2025;
    case 0x2027: goto L2027;
    case 0x2028: goto L2028;
    case 0x202B: goto L202B;
    case 0x202D: goto L202D;
    case 0x202F: goto L202F;
    case 0x2034: goto L2034;
    case 0x2035: goto L2035;
    case 0x2038: goto L2038;
    case 0x203E: goto L203E;
    case 0x2044: goto L2044;
    case 0x2045: goto L2045;
    case 0x2046: goto L2046;
    case 0x204A: goto L204A;
    case 0x2050: goto L2050;
    case 0x2052: goto L2052;
    case 0x2058: goto L2058;
    case 0x205E: goto L205E;
    case 0x2060: goto L2060;
    case 0x2063: goto L2063;
    case 0x2065: goto L2065;
    case 0x206C: goto L206C;
    case 0x2073: goto L2073;
    case 0x2075: goto L2075;
    case 0x2076: goto L2076;
    case 0x2077: goto L2077;
    case 0x207B: goto L207B;
    case 0x207D: goto L207D;
    case 0x2080: goto L2080;
    case 0x2082: goto L2082;
    case 0x2086: goto L2086;
    case 0x2088: goto L2088;
    case 0x208A: goto L208A;
    case 0x208D: goto L208D;
    case 0x2093: goto L2093;
    case 0x2099: goto L2099;
    case 0x209B: goto L209B;
    case 0x209E: goto L209E;
    case 0x20A0: goto L20A0;
    case 0x20A7: goto L20A7;
    case 0x20AE: goto L20AE;
    case 0x20B0: goto L20B0;
    case 0x20B1: goto L20B1;
    case 0x20B2: goto L20B2;
    case 0x20B6: goto L20B6;
    case 0x20BC: goto L20BC;
    case 0x20C2: goto L20C2;
    case 0x20C3: goto L20C3;
    case 0x20C4: goto L20C4;
    case 0x20C8: goto L20C8;
    case 0x20CE: goto L20CE;
    case 0x20D4: goto L20D4;
    case 0x20D5: goto L20D5;
    case 0x20D6: goto L20D6;
    case 0x20DA: goto L20DA;
    case 0x20DB: goto L20DB;
    case 0x20DC: goto L20DC;
    case 0x20DE: goto L20DE;
    case 0x20DF: goto L20DF;
    case 0x20E2: goto L20E2;
    case 0x20E3: goto L20E3;
    case 0x20E4: goto L20E4;
    case 0x20E8: goto L20E8;
    case 0x20EA: goto L20EA;
    case 0x20EC: goto L20EC;
    case 0x20EE: goto L20EE;
    case 0x20F0: goto L20F0;
    case 0x20F1: goto L20F1;
    case 0x20F3: goto L20F3;
    case 0x20F4: goto L20F4;
    case 0x20F6: goto L20F6;
    case 0x20F7: goto L20F7;
    case 0x20F9: goto L20F9;
    case 0x20FC: goto L20FC;
    case 0x2100: goto L2100;
    case 0x2102: goto L2102;
    case 0x2105: goto L2105;
    case 0x2108: goto L2108;
    case 0x210D: goto L210D;
    case 0x2111: goto L2111;
    case 0x2113: goto L2113;
    case 0x2114: goto L2114;
    case 0x2115: goto L2115;
    case 0x2116: goto L2116;
    case 0x211A: goto L211A;
    case 0x211E: goto L211E;
    case 0x2122: goto L2122;
    case 0x2126: goto L2126;
    case 0x2129: goto L2129;
    case 0x212D: goto L212D;
    case 0x2131: goto L2131;
    case 0x2135: goto L2135;
    case 0x2138: goto L2138;
    case 0x213B: goto L213B;
    case 0x213E: goto L213E;
    case 0x2141: goto L2141;
    case 0x2144: goto L2144;
    case 0x2147: goto L2147;
    case 0x214A: goto L214A;
    case 0x214D: goto L214D;
    case 0x2150: goto L2150;
    case 0x2153: goto L2153;
    case 0x2154: goto L2154;
    case 0x2156: goto L2156;
    case 0x2157: goto L2157;
    case 0x2158: goto L2158;
    case 0x215A: goto L215A;
    case 0x215B: goto L215B;
    case 0x215D: goto L215D;
    case 0x215E: goto L215E;
    case 0x2162: goto L2162;
    case 0x2164: goto L2164;
    case 0x2166: goto L2166;
    case 0x2168: goto L2168;
    case 0x216A: goto L216A;
    case 0x216B: goto L216B;
    case 0x216D: goto L216D;
    case 0x216F: goto L216F;
    case 0x2173: goto L2173;
    case 0x2174: goto L2174;
    case 0x2176: goto L2176;
    case 0x2178: goto L2178;
    case 0x217C: goto L217C;
    case 0x217D: goto L217D;
    case 0x217F: goto L217F;
    case 0x2181: goto L2181;
    case 0x2185: goto L2185;
    case 0x2188: goto L2188;
    case 0x218A: goto L218A;
    case 0x218D: goto L218D;
    case 0x2190: goto L2190;
    case 0x2193: goto L2193;
    case 0x2196: goto L2196;
    case 0x2198: goto L2198;
    case 0x2199: goto L2199;
    case 0x219B: goto L219B;
    case 0x219D: goto L219D;
    case 0x21A1: goto L21A1;
    case 0x21A2: goto L21A2;
    case 0x21A4: goto L21A4;
    case 0x21A6: goto L21A6;
    case 0x21AA: goto L21AA;
    case 0x21AB: goto L21AB;
    case 0x21AD: goto L21AD;
    case 0x21AF: goto L21AF;
    case 0x21B3: goto L21B3;
    case 0x21B6: goto L21B6;
    case 0x21B8: goto L21B8;
    case 0x21BB: goto L21BB;
    case 0x21BE: goto L21BE;
    case 0x21C1: goto L21C1;
    case 0x21C3: goto L21C3;
    case 0x21C4: goto L21C4;
    case 0x21C6: goto L21C6;
    case 0x21C8: goto L21C8;
    case 0x21CC: goto L21CC;
    case 0x21CD: goto L21CD;
    case 0x21CF: goto L21CF;
    case 0x21D1: goto L21D1;
    case 0x21D5: goto L21D5;
    case 0x21D6: goto L21D6;
    case 0x21D8: goto L21D8;
    case 0x21DA: goto L21DA;
    case 0x21DE: goto L21DE;
    case 0x21E1: goto L21E1;
    case 0x21E4: goto L21E4;
    case 0x21E7: goto L21E7;
    case 0x21EA: goto L21EA;
    case 0x21ED: goto L21ED;
    case 0x21F0: goto L21F0;
    case 0x21F2: goto L21F2;
    case 0x21F4: goto L21F4;
    case 0x21F5: goto L21F5;
    case 0x21F6: goto L21F6;
    case 0x21F7: goto L21F7;
    case 0x21F9: goto L21F9;
    case 0x21FA: goto L21FA;
    case 0x21FB: goto L21FB;
    case 0x21FF: goto L21FF;
    case 0x2200: goto L2200;
    case 0x2204: goto L2204;
    case 0x2208: goto L2208;
    case 0x220C: goto L220C;
    case 0x220D: goto L220D;
    case 0x220E: goto L220E;
    case 0x2212: goto L2212;
    case 0x2216: goto L2216;
    case 0x221A: goto L221A;
    case 0x221C: goto L221C;
    case 0x221E: goto L221E;
    case 0x2222: goto L2222;
    case 0x2224: goto L2224;
    case 0x2226: goto L2226;
    case 0x2229: goto L2229;
    case 0x222B: goto L222B;
    case 0x222D: goto L222D;
    case 0x222F: goto L222F;
    case 0x2232: goto L2232;
    case 0x2236: goto L2236;
    case 0x223A: goto L223A;
    case 0x223E: goto L223E;
    case 0x2241: goto L2241;
    case 0x2242: goto L2242;
    case 0x2244: goto L2244;
    case 0x2245: goto L2245;
    case 0x2246: goto L2246;
    case 0x2248: goto L2248;
    case 0x2249: goto L2249;
    case 0x224B: goto L224B;
    case 0x224C: goto L224C;
    case 0x2250: goto L2250;
    case 0x2252: goto L2252;
    case 0x2254: goto L2254;
    case 0x2256: goto L2256;
    case 0x2258: goto L2258;
    case 0x225A: goto L225A;
    case 0x225B: goto L225B;
    case 0x225D: goto L225D;
    case 0x225F: goto L225F;
    case 0x2263: goto L2263;
    case 0x2264: goto L2264;
    case 0x2266: goto L2266;
    case 0x2268: goto L2268;
    case 0x226C: goto L226C;
    case 0x226D: goto L226D;
    case 0x226F: goto L226F;
    case 0x2271: goto L2271;
    case 0x2275: goto L2275;
    case 0x2278: goto L2278;
    case 0x227A: goto L227A;
    case 0x227D: goto L227D;
    case 0x2280: goto L2280;
    case 0x2283: goto L2283;
    case 0x2286: goto L2286;
    case 0x2288: goto L2288;
    case 0x2289: goto L2289;
    case 0x228B: goto L228B;
    case 0x228D: goto L228D;
    case 0x2291: goto L2291;
    case 0x2292: goto L2292;
    case 0x2294: goto L2294;
    case 0x2296: goto L2296;
    case 0x229A: goto L229A;
    case 0x229B: goto L229B;
    case 0x229D: goto L229D;
    case 0x229F: goto L229F;
    case 0x22A3: goto L22A3;
    case 0x22A6: goto L22A6;
    case 0x22A8: goto L22A8;
    case 0x22AB: goto L22AB;
    case 0x22AE: goto L22AE;
    case 0x22B1: goto L22B1;
    case 0x22B3: goto L22B3;
    case 0x22B4: goto L22B4;
    case 0x22B6: goto L22B6;
    case 0x22B8: goto L22B8;
    case 0x22BC: goto L22BC;
    case 0x22BD: goto L22BD;
    case 0x22BF: goto L22BF;
    case 0x22C1: goto L22C1;
    case 0x22C5: goto L22C5;
    case 0x22C6: goto L22C6;
    case 0x22C8: goto L22C8;
    case 0x22CA: goto L22CA;
    case 0x22CE: goto L22CE;
    case 0x22D1: goto L22D1;
    case 0x22D4: goto L22D4;
    case 0x22D7: goto L22D7;
    case 0x22DA: goto L22DA;
    case 0x22DD: goto L22DD;
    case 0x22E0: goto L22E0;
    case 0x22E2: goto L22E2;
    case 0x22E4: goto L22E4;
    case 0x22E5: goto L22E5;
    case 0x22E6: goto L22E6;
    case 0x22E7: goto L22E7;
    case 0x22EB: goto L22EB;
    case 0x22EC: goto L22EC;
    case 0x22EE: goto L22EE;
    case 0x22EF: goto L22EF;
    case 0x22F0: goto L22F0;
    case 0x22F4: goto L22F4;
    case 0x22F5: goto L22F5;
    case 0x22F7: goto L22F7;
    case 0x22F9: goto L22F9;
    case 0x22FA: goto L22FA;
    case 0x22FB: goto L22FB;
    case 0x22FF: goto L22FF;
    case 0x2300: goto L2300;
    case 0x2304: goto L2304;
    case 0x2305: goto L2305;
    case 0x2309: goto L2309;
    case 0x230A: goto L230A;
    case 0x230E: goto L230E;
    case 0x2311: goto L2311;
    case 0x2312: goto L2312;
    case 0x2313: goto L2313;
    case 0x2317: goto L2317;
    case 0x231B: goto L231B;
    case 0x231C: goto L231C;
    case 0x231E: goto L231E;
    case 0x2320: goto L2320;
    case 0x2321: goto L2321;
    case 0x2323: goto L2323;
    case 0x2325: goto L2325;
    case 0x2326: goto L2326;
    case 0x2328: goto L2328;
    case 0x232A: goto L232A;
    case 0x232D: goto L232D;
    case 0x232E: goto L232E;
    case 0x2330: goto L2330;
    case 0x2334: goto L2334;
    case 0x2338: goto L2338;
    case 0x233C: goto L233C;
    case 0x233D: goto L233D;
    case 0x233E: goto L233E;
    case 0x2342: goto L2342;
    case 0x2343: goto L2343;
    case 0x2345: goto L2345;
    case 0x2346: goto L2346;
    case 0x2348: goto L2348;
    case 0x234D: goto L234D;
    case 0x2351: goto L2351;
    case 0x2356: goto L2356;
    case 0x235A: goto L235A;
    case 0x235F: goto L235F;
    case 0x2363: goto L2363;
    case 0x2364: goto L2364;
    case 0x2366: goto L2366;
    case 0x2369: goto L2369;
    case 0x236D: goto L236D;
    case 0x2371: goto L2371;
    case 0x2375: goto L2375;
    case 0x2379: goto L2379;
    case 0x237A: goto L237A;
    case 0x237B: goto L237B;
    case 0x237F: goto L237F;
    case 0x2380: goto L2380;
    case 0x2382: goto L2382;
    case 0x2383: goto L2383;
    case 0x2385: goto L2385;
    case 0x238A: goto L238A;
    case 0x238E: goto L238E;
    case 0x2393: goto L2393;
    case 0x2397: goto L2397;
    case 0x239C: goto L239C;
    case 0x23A0: goto L23A0;
    case 0x23A1: goto L23A1;
    case 0x23A3: goto L23A3;
    case 0x23A6: goto L23A6;
    case 0x23AA: goto L23AA;
    case 0x23AE: goto L23AE;
    case 0x23B2: goto L23B2;
    case 0x23B6: goto L23B6;
    case 0x23B7: goto L23B7;
    case 0x23B8: goto L23B8;
    case 0x23BC: goto L23BC;
    case 0x23BD: goto L23BD;
    case 0x23BF: goto L23BF;
    case 0x23C1: goto L23C1;
    case 0x23C3: goto L23C3;
    case 0x23C4: goto L23C4;
    case 0x23C6: goto L23C6;
    case 0x23CA: goto L23CA;
    case 0x23CE: goto L23CE;
    case 0x23D2: goto L23D2;
    case 0x23D6: goto L23D6;
    case 0x23DA: goto L23DA;
    case 0x23DE: goto L23DE;
    case 0x23E2: goto L23E2;
    case 0x23E6: goto L23E6;
    case 0x23EA: goto L23EA;
    case 0x23EE: goto L23EE;
    case 0x23F2: goto L23F2;
    case 0x23F6: goto L23F6;
    case 0x23F9: goto L23F9;
    case 0x23FA: goto L23FA;
    case 0x23FD: goto L23FD;
    case 0x23FE: goto L23FE;
    case 0x2401: goto L2401;
    case 0x2402: goto L2402;
    case 0x2403: goto L2403;
    case 0x2405: goto L2405;
    case 0x2406: goto L2406;
    case 0x2407: goto L2407;
    case 0x240B: goto L240B;
    case 0x240C: goto L240C;
    case 0x2410: goto L2410;
    case 0x2414: goto L2414;
    case 0x2418: goto L2418;
    case 0x241C: goto L241C;
    case 0x2420: goto L2420;
    case 0x2424: goto L2424;
    case 0x2428: goto L2428;
    case 0x242C: goto L242C;
    case 0x2430: goto L2430;
    case 0x2434: goto L2434;
    case 0x2438: goto L2438;
    case 0x243C: goto L243C;
    case 0x243F: goto L243F;
    case 0x2440: goto L2440;
    case 0x2441: goto L2441;
    case 0x2445: goto L2445;
    case 0x2446: goto L2446;
    case 0x2448: goto L2448;
    case 0x244A: goto L244A;
    case 0x244C: goto L244C;
    case 0x244D: goto L244D;
    case 0x244F: goto L244F;
    case 0x2453: goto L2453;
    case 0x2457: goto L2457;
    case 0x245B: goto L245B;
    case 0x245F: goto L245F;
    case 0x2463: goto L2463;
    case 0x2467: goto L2467;
    case 0x246B: goto L246B;
    case 0x246F: goto L246F;
    case 0x2473: goto L2473;
    case 0x2477: goto L2477;
    case 0x247B: goto L247B;
    case 0x247F: goto L247F;
    case 0x2482: goto L2482;
    case 0x2483: goto L2483;
    case 0x2486: goto L2486;
    case 0x2487: goto L2487;
    case 0x248A: goto L248A;
    case 0x248B: goto L248B;
    case 0x248C: goto L248C;
    case 0x248E: goto L248E;
    case 0x248F: goto L248F;
    case 0x2490: goto L2490;
    case 0x2494: goto L2494;
    case 0x2495: goto L2495;
    case 0x2499: goto L2499;
    case 0x249D: goto L249D;
    case 0x24A1: goto L24A1;
    case 0x24A5: goto L24A5;
    case 0x24A9: goto L24A9;
    case 0x24AD: goto L24AD;
    case 0x24B1: goto L24B1;
    case 0x24B5: goto L24B5;
    case 0x24B9: goto L24B9;
    case 0x24BD: goto L24BD;
    case 0x24C1: goto L24C1;
    case 0x24C5: goto L24C5;
    case 0x24C8: goto L24C8;
    case 0x24C9: goto L24C9;
    case 0x24CA: goto L24CA;
    case 0x24CE: goto L24CE;
    case 0x24CF: goto L24CF;
    case 0x24D1: goto L24D1;
    case 0x24D3: goto L24D3;
    case 0x24D5: goto L24D5;
    case 0x24D6: goto L24D6;
    case 0x24D8: goto L24D8;
    case 0x24DC: goto L24DC;
    case 0x24E0: goto L24E0;
    case 0x24E4: goto L24E4;
    case 0x24E8: goto L24E8;
    case 0x24EC: goto L24EC;
    case 0x24F0: goto L24F0;
    case 0x24F4: goto L24F4;
    case 0x24F8: goto L24F8;
    case 0x24FC: goto L24FC;
    case 0x2500: goto L2500;
    case 0x2504: goto L2504;
    case 0x2508: goto L2508;
    case 0x250B: goto L250B;
    case 0x250C: goto L250C;
    case 0x250F: goto L250F;
    case 0x2510: goto L2510;
    case 0x2513: goto L2513;
    case 0x2514: goto L2514;
    case 0x2515: goto L2515;
    case 0x2517: goto L2517;
    case 0x2518: goto L2518;
    case 0x2519: goto L2519;
    case 0x251D: goto L251D;
    case 0x251E: goto L251E;
    case 0x2522: goto L2522;
    case 0x2526: goto L2526;
    case 0x252A: goto L252A;
    case 0x252E: goto L252E;
    case 0x2532: goto L2532;
    case 0x2536: goto L2536;
    case 0x253A: goto L253A;
    case 0x253E: goto L253E;
    case 0x2542: goto L2542;
    case 0x2546: goto L2546;
    case 0x254A: goto L254A;
    case 0x254E: goto L254E;
    case 0x2551: goto L2551;
    case 0x2552: goto L2552;
    case 0x2553: goto L2553;
    case 0x2557: goto L2557;
    case 0x255B: goto L255B;
    case 0x255F: goto L255F;
    case 0x2563: goto L2563;
    case 0x2567: goto L2567;
    case 0x2568: goto L2568;
    case 0x2569: goto L2569;
    case 0x256D: goto L256D;
    case 0x2571: goto L2571;
    case 0x2575: goto L2575;
    case 0x2579: goto L2579;
    case 0x257D: goto L257D;
    case 0x2580: goto L2580;
    case 0x2581: goto L2581;
    case 0x2582: goto L2582;
    case 0x2586: goto L2586;
    case 0x2587: goto L2587;
    case 0x2589: goto L2589;
    case 0x258A: goto L258A;
    case 0x258C: goto L258C;
    case 0x258D: goto L258D;
    case 0x2591: goto L2591;
    case 0x2593: goto L2593;
    case 0x2595: goto L2595;
    case 0x2597: goto L2597;
    case 0x2598: goto L2598;
    case 0x259A: goto L259A;
    case 0x259B: goto L259B;
    case 0x259F: goto L259F;
    case 0x25A1: goto L25A1;
    case 0x25A3: goto L25A3;
    case 0x25A5: goto L25A5;
    case 0x25A7: goto L25A7;
    case 0x25A9: goto L25A9;
    case 0x25AA: goto L25AA;
    case 0x25AB: goto L25AB;
    case 0x25AF: goto L25AF;
    case 0x25B0: goto L25B0;
    case 0x25B2: goto L25B2;
    case 0x25B3: goto L25B3;
    case 0x25B5: goto L25B5;
    case 0x25B6: goto L25B6;
    case 0x25BA: goto L25BA;
    case 0x25BC: goto L25BC;
    case 0x25BE: goto L25BE;
    case 0x25C0: goto L25C0;
    case 0x25C1: goto L25C1;
    case 0x25C3: goto L25C3;
    case 0x25C4: goto L25C4;
    case 0x25C8: goto L25C8;
    case 0x25CA: goto L25CA;
    case 0x25CC: goto L25CC;
    case 0x25CE: goto L25CE;
    case 0x25D0: goto L25D0;
    case 0x25D2: goto L25D2;
    case 0x25D3: goto L25D3;
    case 0x25D4: goto L25D4;
    case 0x25D8: goto L25D8;
    case 0x25D9: goto L25D9;
    case 0x25DB: goto L25DB;
    case 0x25DC: goto L25DC;
    case 0x25DE: goto L25DE;
    case 0x25DF: goto L25DF;
    case 0x25E3: goto L25E3;
    case 0x25E5: goto L25E5;
    case 0x25E7: goto L25E7;
    case 0x25E9: goto L25E9;
    case 0x25EA: goto L25EA;
    case 0x25EC: goto L25EC;
    case 0x25ED: goto L25ED;
    case 0x25F1: goto L25F1;
    case 0x25F3: goto L25F3;
    case 0x25F5: goto L25F5;
    case 0x25F7: goto L25F7;
    case 0x25F9: goto L25F9;
    case 0x25FB: goto L25FB;
    case 0x25FC: goto L25FC;
    case 0x25FD: goto L25FD;
    case 0x2601: goto L2601;
    case 0x2602: goto L2602;
    case 0x2604: goto L2604;
    case 0x2605: goto L2605;
    case 0x2607: goto L2607;
    case 0x2608: goto L2608;
    case 0x260C: goto L260C;
    case 0x260E: goto L260E;
    case 0x2610: goto L2610;
    case 0x2612: goto L2612;
    case 0x2613: goto L2613;
    case 0x2614: goto L2614;
    case 0x2618: goto L2618;
    case 0x2619: goto L2619;
    case 0x261B: goto L261B;
    case 0x261C: goto L261C;
    case 0x261E: goto L261E;
    case 0x261F: goto L261F;
    case 0x2623: goto L2623;
    case 0x2625: goto L2625;
    case 0x2627: goto L2627;
    case 0x2629: goto L2629;
    case 0x262A: goto L262A;
    case 0x262B: goto L262B;
    case 0x262F: goto L262F;
    case 0x2630: goto L2630;
    case 0x2632: goto L2632;
    case 0x2633: goto L2633;
    case 0x2635: goto L2635;
    case 0x2636: goto L2636;
    case 0x263A: goto L263A;
    case 0x263C: goto L263C;
    case 0x263E: goto L263E;
    case 0x2640: goto L2640;
    case 0x2641: goto L2641;
    case 0x2642: goto L2642;
    case 0x2646: goto L2646;
    case 0x2647: goto L2647;
    case 0x2649: goto L2649;
    case 0x264A: goto L264A;
    case 0x264C: goto L264C;
    case 0x264D: goto L264D;
    case 0x2651: goto L2651;
    case 0x2653: goto L2653;
    case 0x2655: goto L2655;
    case 0x2657: goto L2657;
    case 0x2658: goto L2658;
    case 0x265A: goto L265A;
    case 0x265B: goto L265B;
    case 0x265F: goto L265F;
    case 0x2661: goto L2661;
    case 0x2663: goto L2663;
    case 0x2665: goto L2665;
    case 0x2666: goto L2666;
    case 0x2668: goto L2668;
    case 0x2669: goto L2669;
    case 0x266D: goto L266D;
    case 0x266F: goto L266F;
    case 0x2671: goto L2671;
    case 0x2673: goto L2673;
    case 0x2675: goto L2675;
    case 0x2677: goto L2677;
    case 0x2678: goto L2678;
    case 0x2679: goto L2679;
    case 0x267D: goto L267D;
    case 0x267E: goto L267E;
    case 0x2681: goto L2681;
    case 0x2688: goto L2688;
    case 0x268A: goto L268A;
    case 0x268C: goto L268C;
    case 0x2691: goto L2691;
    case 0x2693: goto L2693;
    case 0x2696: goto L2696;
    case 0x269B: goto L269B;
    case 0x269D: goto L269D;
    case 0x26A2: goto L26A2;
    case 0x26A4: goto L26A4;
    case 0x26A7: goto L26A7;
    case 0x26AC: goto L26AC;
    case 0x26AE: goto L26AE;
    case 0x26B3: goto L26B3;
    case 0x26B5: goto L26B5;
    case 0x26B8: goto L26B8;
    case 0x26BD: goto L26BD;
    case 0x26BF: goto L26BF;
    case 0x26C4: goto L26C4;
    case 0x26C6: goto L26C6;
    case 0x26C9: goto L26C9;
    case 0x26CE: goto L26CE;
    case 0x26D0: goto L26D0;
    case 0x26D3: goto L26D3;
    case 0x26D4: goto L26D4;
    case 0x26D7: goto L26D7;
    case 0x26D9: goto L26D9;
    case 0x26DB: goto L26DB;
    case 0x26DC: goto L26DC;
    case 0x26DD: goto L26DD;
    case 0x26E1: goto L26E1;
    case 0x26E6: goto L26E6;
    case 0x26EB: goto L26EB;
    case 0x26EF: goto L26EF;
    case 0x26F3: goto L26F3;
    case 0x26F6: goto L26F6;
    case 0x26F8: goto L26F8;
    case 0x26FD: goto L26FD;
    case 0x2701: goto L2701;
    case 0x2704: goto L2704;
    case 0x2708: goto L2708;
    case 0x270A: goto L270A;
    case 0x270D: goto L270D;
    case 0x2711: goto L2711;
    case 0x2714: goto L2714;
    case 0x2718: goto L2718;
    case 0x271A: goto L271A;
    case 0x271D: goto L271D;
    case 0x2720: goto L2720;
    case 0x2722: goto L2722;
    case 0x2724: goto L2724;
    case 0x2728: goto L2728;
    case 0x272C: goto L272C;
    case 0x272F: goto L272F;
    case 0x2731: goto L2731;
    case 0x2733: goto L2733;
    case 0x2737: goto L2737;
    case 0x2739: goto L2739;
    case 0x273C: goto L273C;
    case 0x273F: goto L273F;
    case 0x2741: goto L2741;
    case 0x2744: goto L2744;
    case 0x2747: goto L2747;
    case 0x2749: goto L2749;
    case 0x274C: goto L274C;
    case 0x274E: goto L274E;
    case 0x2750: goto L2750;
    case 0x2752: goto L2752;
    case 0x2754: goto L2754;
    case 0x2756: goto L2756;
    case 0x2758: goto L2758;
    case 0x2759: goto L2759;
    case 0x275B: goto L275B;
    case 0x275E: goto L275E;
    case 0x275F: goto L275F;
    case 0x2761: goto L2761;
    case 0x2764: goto L2764;
    case 0x2767: goto L2767;
    case 0x2769: goto L2769;
    case 0x276B: goto L276B;
    case 0x276D: goto L276D;
    case 0x276F: goto L276F;
    case 0x2771: goto L2771;
    case 0x2773: goto L2773;
    case 0x2774: goto L2774;
    case 0x2776: goto L2776;
    case 0x2779: goto L2779;
    case 0x277A: goto L277A;
    case 0x277B: goto L277B;
    case 0x277D: goto L277D;
    case 0x277F: goto L277F;
    case 0x2782: goto L2782;
    case 0x2783: goto L2783;
    case 0x2787: goto L2787;
    case 0x278B: goto L278B;
    case 0x278F: goto L278F;
    case 0x2791: goto L2791;
    case 0x2794: goto L2794;
    case 0x2797: goto L2797;
    case 0x2799: goto L2799;
    case 0x279C: goto L279C;
    case 0x279F: goto L279F;
    case 0x27A2: goto L27A2;
    case 0x27A4: goto L27A4;
    case 0x27A6: goto L27A6;
    case 0x27A8: goto L27A8;
    case 0x27AA: goto L27AA;
    case 0x27AC: goto L27AC;
    case 0x27AE: goto L27AE;
    case 0x27B0: goto L27B0;
    case 0x27B1: goto L27B1;
    case 0x27B3: goto L27B3;
    case 0x27B5: goto L27B5;
    case 0x27B6: goto L27B6;
    case 0x27B8: goto L27B8;
    case 0x27BB: goto L27BB;
    case 0x27BE: goto L27BE;
    case 0x27C0: goto L27C0;
    case 0x27C2: goto L27C2;
    case 0x27C4: goto L27C4;
    case 0x27C6: goto L27C6;
    case 0x27C8: goto L27C8;
    case 0x27CA: goto L27CA;
    case 0x27CB: goto L27CB;
    case 0x27CD: goto L27CD;
    case 0x27D0: goto L27D0;
    case 0x27D1: goto L27D1;
    case 0x27D2: goto L27D2;
    case 0x27D4: goto L27D4;
    case 0x27D6: goto L27D6;
    case 0x27D9: goto L27D9;
    case 0x27DA: goto L27DA;
    case 0x27DE: goto L27DE;
    case 0x27E2: goto L27E2;
    case 0x27E5: goto L27E5;
    case 0x27E9: goto L27E9;
    case 0x27EA: goto L27EA;
    case 0x27EF: goto L27EF;
    case 0x27F4: goto L27F4;
    case 0x27F8: goto L27F8;
    case 0x27FC: goto L27FC;
    case 0x27FF: goto L27FF;
    case 0x2801: goto L2801;
    case 0x2806: goto L2806;
    case 0x280A: goto L280A;
    case 0x280D: goto L280D;
    case 0x2811: goto L2811;
    case 0x2813: goto L2813;
    case 0x2816: goto L2816;
    case 0x281A: goto L281A;
    case 0x281D: goto L281D;
    case 0x2821: goto L2821;
    case 0x2823: goto L2823;
    case 0x2826: goto L2826;
    case 0x2829: goto L2829;
    case 0x282B: goto L282B;
    case 0x282D: goto L282D;
    case 0x2831: goto L2831;
    case 0x2835: goto L2835;
    case 0x2838: goto L2838;
    case 0x283A: goto L283A;
    case 0x283C: goto L283C;
    case 0x2840: goto L2840;
    case 0x2842: goto L2842;
    case 0x2845: goto L2845;
    case 0x2848: goto L2848;
    case 0x284A: goto L284A;
    case 0x284D: goto L284D;
    case 0x2850: goto L2850;
    case 0x2852: goto L2852;
    case 0x2855: goto L2855;
    case 0x2857: goto L2857;
    case 0x2859: goto L2859;
    case 0x285B: goto L285B;
    case 0x285D: goto L285D;
    case 0x285F: goto L285F;
    case 0x2861: goto L2861;
    case 0x2862: goto L2862;
    case 0x2864: goto L2864;
    case 0x2867: goto L2867;
    case 0x2868: goto L2868;
    case 0x286A: goto L286A;
    case 0x286D: goto L286D;
    case 0x2870: goto L2870;
    case 0x2872: goto L2872;
    case 0x2874: goto L2874;
    case 0x2876: goto L2876;
    case 0x2878: goto L2878;
    case 0x287A: goto L287A;
    case 0x287C: goto L287C;
    case 0x287D: goto L287D;
    case 0x287F: goto L287F;
    case 0x2882: goto L2882;
    case 0x2883: goto L2883;
    case 0x2885: goto L2885;
    case 0x2887: goto L2887;
    case 0x2888: goto L2888;
    case 0x288A: goto L288A;
    case 0x288D: goto L288D;
    case 0x288E: goto L288E;
    case 0x2892: goto L2892;
    case 0x2896: goto L2896;
    case 0x289A: goto L289A;
    case 0x289C: goto L289C;
    case 0x289F: goto L289F;
    case 0x28A2: goto L28A2;
    case 0x28A5: goto L28A5;
    case 0x28A7: goto L28A7;
    case 0x28AA: goto L28AA;
    case 0x28AD: goto L28AD;
    case 0x28B0: goto L28B0;
    case 0x28B2: goto L28B2;
    case 0x28B4: goto L28B4;
    case 0x28B6: goto L28B6;
    case 0x28B8: goto L28B8;
    case 0x28BA: goto L28BA;
    case 0x28BC: goto L28BC;
    case 0x28BE: goto L28BE;
    case 0x28BF: goto L28BF;
    case 0x28C1: goto L28C1;
    case 0x28C3: goto L28C3;
    case 0x28C4: goto L28C4;
    case 0x28C6: goto L28C6;
    case 0x28C9: goto L28C9;
    case 0x28CC: goto L28CC;
    case 0x28CE: goto L28CE;
    case 0x28D0: goto L28D0;
    case 0x28D2: goto L28D2;
    case 0x28D4: goto L28D4;
    case 0x28D6: goto L28D6;
    case 0x28D8: goto L28D8;
    case 0x28D9: goto L28D9;
    case 0x28DB: goto L28DB;
    case 0x28DE: goto L28DE;
    case 0x28DF: goto L28DF;
    case 0x28E1: goto L28E1;
    case 0x28E3: goto L28E3;
    case 0x28E4: goto L28E4;
    case 0x28E6: goto L28E6;
    case 0x28E9: goto L28E9;
    case 0x28EA: goto L28EA;
    case 0x28EE: goto L28EE;
    case 0x28F2: goto L28F2;
    case 0x28F5: goto L28F5;
    case 0x28F9: goto L28F9;
    case 0x28FA: goto L28FA;
    case 0x28FF: goto L28FF;
    case 0x2904: goto L2904;
    case 0x2908: goto L2908;
    case 0x290C: goto L290C;
    case 0x290F: goto L290F;
    case 0x2911: goto L2911;
    case 0x2916: goto L2916;
    case 0x291A: goto L291A;
    case 0x291D: goto L291D;
    case 0x2921: goto L2921;
    case 0x2923: goto L2923;
    case 0x2926: goto L2926;
    case 0x292A: goto L292A;
    case 0x292D: goto L292D;
    case 0x2931: goto L2931;
    case 0x2933: goto L2933;
    case 0x2936: goto L2936;
    case 0x2939: goto L2939;
    case 0x293B: goto L293B;
    case 0x293D: goto L293D;
    case 0x2941: goto L2941;
    case 0x2945: goto L2945;
    case 0x2948: goto L2948;
    case 0x294A: goto L294A;
    case 0x294C: goto L294C;
    case 0x2950: goto L2950;
    case 0x2952: goto L2952;
    case 0x2955: goto L2955;
    case 0x2958: goto L2958;
    case 0x295A: goto L295A;
    case 0x295D: goto L295D;
    case 0x295F: goto L295F;
    case 0x2961: goto L2961;
    case 0x2964: goto L2964;
    case 0x2966: goto L2966;
    case 0x2968: goto L2968;
    case 0x296A: goto L296A;
    case 0x296C: goto L296C;
    case 0x296E: goto L296E;
    case 0x2970: goto L2970;
    case 0x2971: goto L2971;
    case 0x2973: goto L2973;
    case 0x2976: goto L2976;
    case 0x2977: goto L2977;
    case 0x2979: goto L2979;
    case 0x297C: goto L297C;
    case 0x297F: goto L297F;
    case 0x2981: goto L2981;
    case 0x2983: goto L2983;
    case 0x2985: goto L2985;
    case 0x2987: goto L2987;
    case 0x2989: goto L2989;
    case 0x298B: goto L298B;
    case 0x298C: goto L298C;
    case 0x298E: goto L298E;
    case 0x2991: goto L2991;
    case 0x2992: goto L2992;
    case 0x2994: goto L2994;
    case 0x2996: goto L2996;
    case 0x2997: goto L2997;
    case 0x2999: goto L2999;
    case 0x299C: goto L299C;
    case 0x299D: goto L299D;
    case 0x29A1: goto L29A1;
    case 0x29A5: goto L29A5;
    case 0x29A9: goto L29A9;
    case 0x29AB: goto L29AB;
    case 0x29AE: goto L29AE;
    case 0x29B0: goto L29B0;
    case 0x29B3: goto L29B3;
    case 0x29B5: goto L29B5;
    case 0x29B8: goto L29B8;
    case 0x29BB: goto L29BB;
    case 0x29BE: goto L29BE;
    case 0x29C0: goto L29C0;
    case 0x29C2: goto L29C2;
    case 0x29C4: goto L29C4;
    case 0x29C6: goto L29C6;
    case 0x29C8: goto L29C8;
    case 0x29CA: goto L29CA;
    case 0x29CC: goto L29CC;
    case 0x29CD: goto L29CD;
    case 0x29CF: goto L29CF;
    case 0x29D1: goto L29D1;
    case 0x29D2: goto L29D2;
    case 0x29D4: goto L29D4;
    case 0x29D7: goto L29D7;
    case 0x29DA: goto L29DA;
    case 0x29DC: goto L29DC;
    case 0x29DE: goto L29DE;
    case 0x29E0: goto L29E0;
    case 0x29E2: goto L29E2;
    case 0x29E4: goto L29E4;
    case 0x29E6: goto L29E6;
    case 0x29E7: goto L29E7;
    case 0x29E9: goto L29E9;
    case 0x29EC: goto L29EC;
    case 0x29ED: goto L29ED;
    case 0x29EF: goto L29EF;
    case 0x29F1: goto L29F1;
    case 0x29F2: goto L29F2;
    case 0x29F4: goto L29F4;
    case 0x29F7: goto L29F7;
    case 0x29F8: goto L29F8;
    case 0x29FC: goto L29FC;
    case 0x2A00: goto L2A00;
    case 0x2A03: goto L2A03;
    case 0x2A07: goto L2A07;
    case 0x2A08: goto L2A08;
    case 0x2A0D: goto L2A0D;
    case 0x2A12: goto L2A12;
    case 0x2A16: goto L2A16;
    case 0x2A1A: goto L2A1A;
    case 0x2A1D: goto L2A1D;
    case 0x2A1F: goto L2A1F;
    case 0x2A24: goto L2A24;
    case 0x2A28: goto L2A28;
    case 0x2A2B: goto L2A2B;
    case 0x2A2F: goto L2A2F;
    case 0x2A31: goto L2A31;
    case 0x2A34: goto L2A34;
    case 0x2A38: goto L2A38;
    case 0x2A3B: goto L2A3B;
    case 0x2A3F: goto L2A3F;
    case 0x2A41: goto L2A41;
    case 0x2A44: goto L2A44;
    case 0x2A47: goto L2A47;
    case 0x2A49: goto L2A49;
    case 0x2A4B: goto L2A4B;
    case 0x2A4F: goto L2A4F;
    case 0x2A53: goto L2A53;
    case 0x2A56: goto L2A56;
    case 0x2A58: goto L2A58;
    case 0x2A5A: goto L2A5A;
    case 0x2A5E: goto L2A5E;
    case 0x2A60: goto L2A60;
    case 0x2A63: goto L2A63;
    case 0x2A66: goto L2A66;
    case 0x2A68: goto L2A68;
    case 0x2A6B: goto L2A6B;
    case 0x2A6D: goto L2A6D;
    case 0x2A6F: goto L2A6F;
    case 0x2A72: goto L2A72;
    case 0x2A74: goto L2A74;
    case 0x2A76: goto L2A76;
    case 0x2A78: goto L2A78;
    case 0x2A7A: goto L2A7A;
    case 0x2A7C: goto L2A7C;
    case 0x2A7E: goto L2A7E;
    case 0x2A7F: goto L2A7F;
    case 0x2A81: goto L2A81;
    case 0x2A84: goto L2A84;
    case 0x2A85: goto L2A85;
    case 0x2A87: goto L2A87;
    case 0x2A8A: goto L2A8A;
    case 0x2A8D: goto L2A8D;
    case 0x2A8F: goto L2A8F;
    case 0x2A91: goto L2A91;
    case 0x2A93: goto L2A93;
    case 0x2A95: goto L2A95;
    case 0x2A97: goto L2A97;
    case 0x2A99: goto L2A99;
    case 0x2A9A: goto L2A9A;
    case 0x2A9C: goto L2A9C;
    case 0x2A9F: goto L2A9F;
    case 0x2AA0: goto L2AA0;
    case 0x2AA2: goto L2AA2;
    case 0x2AA4: goto L2AA4;
    case 0x2AA6: goto L2AA6;
    case 0x2AA7: goto L2AA7;
    case 0x2AA9: goto L2AA9;
    case 0x2AAC: goto L2AAC;
    case 0x2AAD: goto L2AAD;
    case 0x2AB1: goto L2AB1;
    case 0x2AB5: goto L2AB5;
    case 0x2AB9: goto L2AB9;
    case 0x2ABB: goto L2ABB;
    case 0x2ABE: goto L2ABE;
    case 0x2AC1: goto L2AC1;
    case 0x2AC3: goto L2AC3;
    case 0x2AC5: goto L2AC5;
    case 0x2AC8: goto L2AC8;
    case 0x2ACB: goto L2ACB;
    case 0x2ACE: goto L2ACE;
    case 0x2AD0: goto L2AD0;
    case 0x2AD2: goto L2AD2;
    case 0x2AD4: goto L2AD4;
    case 0x2AD6: goto L2AD6;
    case 0x2AD8: goto L2AD8;
    case 0x2ADA: goto L2ADA;
    case 0x2ADC: goto L2ADC;
    case 0x2ADD: goto L2ADD;
    case 0x2ADF: goto L2ADF;
    case 0x2AE1: goto L2AE1;
    case 0x2AE2: goto L2AE2;
    case 0x2AE4: goto L2AE4;
    case 0x2AE7: goto L2AE7;
    case 0x2AEA: goto L2AEA;
    case 0x2AEC: goto L2AEC;
    case 0x2AEE: goto L2AEE;
    case 0x2AF0: goto L2AF0;
    case 0x2AF2: goto L2AF2;
    case 0x2AF4: goto L2AF4;
    case 0x2AF6: goto L2AF6;
    case 0x2AF7: goto L2AF7;
    case 0x2AF9: goto L2AF9;
    case 0x2AFC: goto L2AFC;
    case 0x2AFD: goto L2AFD;
    case 0x2AFF: goto L2AFF;
    case 0x2B01: goto L2B01;
    case 0x2B03: goto L2B03;
    case 0x2B04: goto L2B04;
    case 0x2B06: goto L2B06;
    case 0x2B09: goto L2B09;
    case 0x2B0A: goto L2B0A;
    case 0x2B0E: goto L2B0E;
    case 0x2B12: goto L2B12;
    case 0x2B15: goto L2B15;
    case 0x2B19: goto L2B19;
    case 0x2B1A: goto L2B1A;
    case 0x2B1B: goto L2B1B;
    case 0x2B1D: goto L2B1D;
    case 0x2B21: goto L2B21;
    case 0x2B22: goto L2B22;
    case 0x2B24: goto L2B24;
    case 0x2B26: goto L2B26;
    case 0x2B2A: goto L2B2A;
    case 0x2B2E: goto L2B2E;
    case 0x2B30: goto L2B30;
    case 0x2B34: goto L2B34;
    case 0x2B38: goto L2B38;
    case 0x2B3A: goto L2B3A;
    case 0x2B3E: goto L2B3E;
    case 0x2B42: goto L2B42;
    case 0x2B43: goto L2B43;
    case 0x2B45: goto L2B45;
    case 0x2B47: goto L2B47;
    case 0x2B4B: goto L2B4B;
    case 0x2B4F: goto L2B4F;
    case 0x2B51: goto L2B51;
    case 0x2B55: goto L2B55;
    case 0x2B59: goto L2B59;
    case 0x2B5B: goto L2B5B;
    case 0x2B5F: goto L2B5F;
    case 0x2B63: goto L2B63;
    case 0x2B64: goto L2B64;
    case 0x2B66: goto L2B66;
    case 0x2B68: goto L2B68;
    case 0x2B6C: goto L2B6C;
    case 0x2B70: goto L2B70;
    case 0x2B72: goto L2B72;
    case 0x2B76: goto L2B76;
    case 0x2B7A: goto L2B7A;
    case 0x2B7C: goto L2B7C;
    case 0x2B80: goto L2B80;
    case 0x2B84: goto L2B84;
    case 0x2B88: goto L2B88;
    case 0x2B8A: goto L2B8A;
    case 0x2B8C: goto L2B8C;
    case 0x2B8F: goto L2B8F;
    case 0x2B93: goto L2B93;
    case 0x2B97: goto L2B97;
    case 0x2B9B: goto L2B9B;
    case 0x2B9F: goto L2B9F;
    case 0x2BA3: goto L2BA3;
    case 0x2BA7: goto L2BA7;
    case 0x2BAB: goto L2BAB;
    case 0x2BAF: goto L2BAF;
    case 0x2BB3: goto L2BB3;
    case 0x2BB7: goto L2BB7;
    case 0x2BBB: goto L2BBB;
    case 0x2BBF: goto L2BBF;
    case 0x2BC1: goto L2BC1;
    case 0x2BC4: goto L2BC4;
    case 0x2BC6: goto L2BC6;
    case 0x2BC8: goto L2BC8;
    case 0x2BCB: goto L2BCB;
    case 0x2BCD: goto L2BCD;
    case 0x2BCF: goto L2BCF;
    case 0x2BD2: goto L2BD2;
    case 0x2BD4: goto L2BD4;
    case 0x2BD6: goto L2BD6;
    case 0x2BD8: goto L2BD8;
    case 0x2BDB: goto L2BDB;
    case 0x2BDD: goto L2BDD;
    case 0x2BDF: goto L2BDF;
    case 0x2BE2: goto L2BE2;
    case 0x2BE6: goto L2BE6;
    case 0x2BEA: goto L2BEA;
    case 0x2BEE: goto L2BEE;
    case 0x2BF2: goto L2BF2;
    case 0x2BF6: goto L2BF6;
    case 0x2BFA: goto L2BFA;
    case 0x2BFE: goto L2BFE;
    case 0x2C02: goto L2C02;
    case 0x2C06: goto L2C06;
    case 0x2C0A: goto L2C0A;
    case 0x2C0E: goto L2C0E;
    case 0x2C12: goto L2C12;
    case 0x2C14: goto L2C14;
    case 0x2C17: goto L2C17;
    case 0x2C19: goto L2C19;
    case 0x2C1B: goto L2C1B;
    case 0x2C1E: goto L2C1E;
    case 0x2C20: goto L2C20;
    case 0x2C22: goto L2C22;
    case 0x2C25: goto L2C25;
    case 0x2C27: goto L2C27;
    case 0x2C29: goto L2C29;
    case 0x2C2B: goto L2C2B;
    case 0x2C2E: goto L2C2E;
    case 0x2C30: goto L2C30;
    case 0x2C32: goto L2C32;
    case 0x2C35: goto L2C35;
    case 0x2C39: goto L2C39;
    case 0x2C3D: goto L2C3D;
    case 0x2C41: goto L2C41;
    case 0x2C45: goto L2C45;
    case 0x2C49: goto L2C49;
    case 0x2C4D: goto L2C4D;
    case 0x2C51: goto L2C51;
    case 0x2C55: goto L2C55;
    case 0x2C59: goto L2C59;
    case 0x2C5D: goto L2C5D;
    case 0x2C61: goto L2C61;
    case 0x2C65: goto L2C65;
    case 0x2C67: goto L2C67;
    case 0x2C6A: goto L2C6A;
    case 0x2C6C: goto L2C6C;
    case 0x2C6E: goto L2C6E;
    case 0x2C71: goto L2C71;
    case 0x2C73: goto L2C73;
    case 0x2C75: goto L2C75;
    case 0x2C78: goto L2C78;
    case 0x2C7A: goto L2C7A;
    case 0x2C7C: goto L2C7C;
    case 0x2C7E: goto L2C7E;
    case 0x2C81: goto L2C81;
    case 0x2C83: goto L2C83;
    case 0x2C85: goto L2C85;
    case 0x2C88: goto L2C88;
    case 0x2C8C: goto L2C8C;
    case 0x2C90: goto L2C90;
    case 0x2C94: goto L2C94;
    case 0x2C98: goto L2C98;
    case 0x2C9C: goto L2C9C;
    case 0x2CA0: goto L2CA0;
    case 0x2CA4: goto L2CA4;
    case 0x2CA8: goto L2CA8;
    case 0x2CAC: goto L2CAC;
    case 0x2CB0: goto L2CB0;
    case 0x2CB4: goto L2CB4;
    case 0x2CB8: goto L2CB8;
    case 0x2CBA: goto L2CBA;
    case 0x2CBD: goto L2CBD;
    case 0x2CBF: goto L2CBF;
    case 0x2CC1: goto L2CC1;
    case 0x2CC4: goto L2CC4;
    case 0x2CC6: goto L2CC6;
    case 0x2CC8: goto L2CC8;
    case 0x2CCB: goto L2CCB;
    case 0x2CCD: goto L2CCD;
    case 0x2CCF: goto L2CCF;
    case 0x2CD1: goto L2CD1;
    case 0x2CD4: goto L2CD4;
    case 0x2CD6: goto L2CD6;
    case 0x2CD8: goto L2CD8;
    case 0x2CDB: goto L2CDB;
    case 0x2CDF: goto L2CDF;
    case 0x2CE3: goto L2CE3;
    case 0x2CE7: goto L2CE7;
    case 0x2CEB: goto L2CEB;
    case 0x2CEF: goto L2CEF;
    case 0x2CF3: goto L2CF3;
    case 0x2CF7: goto L2CF7;
    case 0x2CFB: goto L2CFB;
    case 0x2CFF: goto L2CFF;
    case 0x2D03: goto L2D03;
    case 0x2D07: goto L2D07;
    case 0x2D0B: goto L2D0B;
    case 0x2D0D: goto L2D0D;
    case 0x2D10: goto L2D10;
    case 0x2D12: goto L2D12;
    case 0x2D14: goto L2D14;
    case 0x2D17: goto L2D17;
    case 0x2D19: goto L2D19;
    case 0x2D1B: goto L2D1B;
    case 0x2D1E: goto L2D1E;
    case 0x2D20: goto L2D20;
    case 0x2D22: goto L2D22;
    case 0x2D24: goto L2D24;
    case 0x2D27: goto L2D27;
    case 0x2D29: goto L2D29;
    case 0x2D2B: goto L2D2B;
    case 0x2D2E: goto L2D2E;
    case 0x2D32: goto L2D32;
    case 0x2D36: goto L2D36;
    case 0x2D3A: goto L2D3A;
    case 0x2D3E: goto L2D3E;
    case 0x2D42: goto L2D42;
    case 0x2D46: goto L2D46;
    case 0x2D4A: goto L2D4A;
    case 0x2D4E: goto L2D4E;
    case 0x2D52: goto L2D52;
    case 0x2D56: goto L2D56;
    case 0x2D5A: goto L2D5A;
    case 0x2D5E: goto L2D5E;
    case 0x2D60: goto L2D60;
    case 0x2D63: goto L2D63;
    case 0x2D65: goto L2D65;
    case 0x2D67: goto L2D67;
    case 0x2D6A: goto L2D6A;
    case 0x2D6C: goto L2D6C;
    case 0x2D6E: goto L2D6E;
    case 0x2D71: goto L2D71;
    case 0x2D73: goto L2D73;
    case 0x2D75: goto L2D75;
    case 0x2D77: goto L2D77;
    case 0x2D7A: goto L2D7A;
    case 0x2D7C: goto L2D7C;
    case 0x2D7E: goto L2D7E;
    case 0x2D81: goto L2D81;
    case 0x2D85: goto L2D85;
    case 0x2D89: goto L2D89;
    case 0x2D8D: goto L2D8D;
    case 0x2D91: goto L2D91;
    case 0x2D95: goto L2D95;
    case 0x2D99: goto L2D99;
    case 0x2D9D: goto L2D9D;
    case 0x2DA1: goto L2DA1;
    case 0x2DA5: goto L2DA5;
    case 0x2DA9: goto L2DA9;
    case 0x2DAD: goto L2DAD;
    case 0x2DB1: goto L2DB1;
    case 0x2DB3: goto L2DB3;
    case 0x2DB5: goto L2DB5;
    case 0x2DB7: goto L2DB7;
    case 0x2DB9: goto L2DB9;
    case 0x2DBB: goto L2DBB;
    case 0x2DBD: goto L2DBD;
    case 0x2DBF: goto L2DBF;
    case 0x2DC1: goto L2DC1;
    case 0x2DC3: goto L2DC3;
    case 0x2DC5: goto L2DC5;
    case 0x2DC9: goto L2DC9;
    case 0x2DCD: goto L2DCD;
    case 0x2DD1: goto L2DD1;
    case 0x2DD5: goto L2DD5;
    case 0x2DD9: goto L2DD9;
    case 0x2DDD: goto L2DDD;
    case 0x2DE1: goto L2DE1;
    case 0x2DE5: goto L2DE5;
    case 0x2DE9: goto L2DE9;
    case 0x2DED: goto L2DED;
    case 0x2DF1: goto L2DF1;
    case 0x2DF5: goto L2DF5;
    case 0x2DF7: goto L2DF7;
    case 0x2DF9: goto L2DF9;
    case 0x2DFB: goto L2DFB;
    case 0x2DFD: goto L2DFD;
    case 0x2DFF: goto L2DFF;
    case 0x2E01: goto L2E01;
    case 0x2E03: goto L2E03;
    case 0x2E05: goto L2E05;
    case 0x2E07: goto L2E07;
    case 0x2E09: goto L2E09;
    case 0x2E0C: goto L2E0C;
    case 0x2E0F: goto L2E0F;
    case 0x2E12: goto L2E12;
    case 0x2E13: goto L2E13;
    case 0x2E14: goto L2E14;
    case 0x2E18: goto L2E18;
    case 0x2E1B: goto L2E1B;
    case 0x2E1C: goto L2E1C;
    case 0x2E1D: goto L2E1D;
    case 0x2E21: goto L2E21;
    case 0x2E23: goto L2E23;
    case 0x2E27: goto L2E27;
    case 0x2E2B: goto L2E2B;
    case 0x2E2F: goto L2E2F;
    case 0x2E33: goto L2E33;
    case 0x2E37: goto L2E37;
    case 0x2E3B: goto L2E3B;
    case 0x2E3F: goto L2E3F;
    case 0x2E43: goto L2E43;
    case 0x2E47: goto L2E47;
    case 0x2E4B: goto L2E4B;
    case 0x2E4F: goto L2E4F;
    case 0x2E53: goto L2E53;
    case 0x2E56: goto L2E56;
    case 0x2E58: goto L2E58;
    case 0x2E5A: goto L2E5A;
    case 0x2E5E: goto L2E5E;
    case 0x2E62: goto L2E62;
    case 0x2E66: goto L2E66;
    case 0x2E6A: goto L2E6A;
    case 0x2E6E: goto L2E6E;
    case 0x2E72: goto L2E72;
    case 0x2E76: goto L2E76;
    case 0x2E7A: goto L2E7A;
    case 0x2E7E: goto L2E7E;
    case 0x2E82: goto L2E82;
    case 0x2E86: goto L2E86;
    case 0x2E8A: goto L2E8A;
    case 0x2E8D: goto L2E8D;
    case 0x2E8F: goto L2E8F;
    case 0x2E91: goto L2E91;
    case 0x2E95: goto L2E95;
    case 0x2E99: goto L2E99;
    case 0x2E9D: goto L2E9D;
    case 0x2EA1: goto L2EA1;
    case 0x2EA5: goto L2EA5;
    case 0x2EA9: goto L2EA9;
    case 0x2EAD: goto L2EAD;
    case 0x2EB1: goto L2EB1;
    case 0x2EB5: goto L2EB5;
    case 0x2EB9: goto L2EB9;
    case 0x2EBD: goto L2EBD;
    case 0x2EC1: goto L2EC1;
    case 0x2EC4: goto L2EC4;
    case 0x2EC6: goto L2EC6;
    case 0x2EC8: goto L2EC8;
    case 0x2ECB: goto L2ECB;
    case 0x2ECF: goto L2ECF;
    case 0x2ED3: goto L2ED3;
    case 0x2ED7: goto L2ED7;
    case 0x2EDB: goto L2EDB;
    case 0x2EDF: goto L2EDF;
    case 0x2EE3: goto L2EE3;
    case 0x2EE7: goto L2EE7;
    case 0x2EEB: goto L2EEB;
    case 0x2EEF: goto L2EEF;
    case 0x2EF3: goto L2EF3;
    case 0x2EF7: goto L2EF7;
    case 0x2EFB: goto L2EFB;
    case 0x2EFE: goto L2EFE;
    case 0x2F00: goto L2F00;
    case 0x2F02: goto L2F02;
    case 0x2F05: goto L2F05;
    case 0x2F09: goto L2F09;
    case 0x2F0D: goto L2F0D;
    case 0x2F11: goto L2F11;
    case 0x2F15: goto L2F15;
    case 0x2F19: goto L2F19;
    case 0x2F1D: goto L2F1D;
    case 0x2F21: goto L2F21;
    case 0x2F25: goto L2F25;
    case 0x2F29: goto L2F29;
    case 0x2F2D: goto L2F2D;
    case 0x2F31: goto L2F31;
    case 0x2F35: goto L2F35;
    case 0x2F38: goto L2F38;
    case 0x2F3A: goto L2F3A;
    case 0x2F3C: goto L2F3C;
    case 0x2F3F: goto L2F3F;
    case 0x2F43: goto L2F43;
    case 0x2F47: goto L2F47;
    case 0x2F4B: goto L2F4B;
    case 0x2F4F: goto L2F4F;
    case 0x2F53: goto L2F53;
    case 0x2F57: goto L2F57;
    case 0x2F5B: goto L2F5B;
    case 0x2F5F: goto L2F5F;
    case 0x2F63: goto L2F63;
    case 0x2F67: goto L2F67;
    case 0x2F6B: goto L2F6B;
    case 0x2F6F: goto L2F6F;
    case 0x2F72: goto L2F72;
    case 0x2F74: goto L2F74;
    case 0x2F76: goto L2F76;
    case 0x2F79: goto L2F79;
    case 0x2F7D: goto L2F7D;
    case 0x2F81: goto L2F81;
    case 0x2F85: goto L2F85;
    case 0x2F89: goto L2F89;
    case 0x2F8D: goto L2F8D;
    case 0x2F91: goto L2F91;
    case 0x2F95: goto L2F95;
    case 0x2F99: goto L2F99;
    case 0x2F9D: goto L2F9D;
    case 0x2FA1: goto L2FA1;
    case 0x2FA5: goto L2FA5;
    case 0x2FA9: goto L2FA9;
    case 0x2FAC: goto L2FAC;
    case 0x2FAE: goto L2FAE;
    case 0x2FB0: goto L2FB0;
    case 0x2FB3: goto L2FB3;
    case 0x2FB7: goto L2FB7;
    case 0x2FBB: goto L2FBB;
    case 0x2FBF: goto L2FBF;
    case 0x2FC3: goto L2FC3;
    case 0x2FC7: goto L2FC7;
    case 0x2FCB: goto L2FCB;
    case 0x2FCF: goto L2FCF;
    case 0x2FD3: goto L2FD3;
    case 0x2FD7: goto L2FD7;
    case 0x2FDB: goto L2FDB;
    case 0x2FDF: goto L2FDF;
    case 0x2FE3: goto L2FE3;
    case 0x2FE6: goto L2FE6;
    case 0x2FE8: goto L2FE8;
    case 0x2FEA: goto L2FEA;
    case 0x2FED: goto L2FED;
    case 0x2FEE: goto L2FEE;
    case 0x2FF0: goto L2FF0;
    case 0x2FF2: goto L2FF2;
    case 0x2FF4: goto L2FF4;
    case 0x2FF5: goto L2FF5;
    case 0x2FF6: goto L2FF6;
    case 0x2FFA: goto L2FFA;
    default: asm_bad_entry("INTERP.ASM", entry);
    }

    /* seg004_0849_DB0  (+DB0): three far routines nothing in the EXE calls. All three work on
       seg052_519C words 158h (the current view description, which create_matrix requires to be
       130h), 15Ah and 1A6h. DB0 saves 158h in 1A6h; DBE restores it and clears 15Ah. */
L0DB0: /* _seg004_0849_DB0 */
    /* 0DB0  push    ds */
    push16(asm_ds);
L0DB1:
    /* 0DB1  mov     cx,seg seg052_519C */
    CX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L0DB4:
    /* 0DB4  mov     ds,cx */
    SET_DS(CX);
L0DB6:
    /* 0DB6  mov     ax,word ptr ds:[158h] */
    AX = rw(pDS, 0x158);
L0DB9:
    /* 0DB9  mov     word ptr ds:[1A6h],ax */
    ww(pDS, 0x1A6, AX);
L0DBC:
    /* 0DBC  pop     ds */
    SET_DS(pop16());
L0DBD:
    /* 0DBD  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_0849_DBE  (+DBE) */
L0DBE: /* _seg004_0849_DBE */
    /* 0DBE  push    ds */
    push16(asm_ds);
L0DBF:
    /* 0DBF  mov     cx,seg seg052_519C */
    CX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L0DC2:
    /* 0DC2  mov     ds,cx */
    SET_DS(CX);
L0DC4:
    /* 0DC4  mov     ax,word ptr ds:[1A6h] */
    AX = rw(pDS, 0x1A6);
L0DC7:
    /* 0DC7  mov     word ptr ds:[158h],ax */
    ww(pDS, 0x158, AX);
L0DCA:
    /* 0DCA  mov     word ptr ds:[15Ah],0 */
    ww(pDS, 0x15A, 0x0);
L0DD0:
    /* 0DD0  pop     ds */
    SET_DS(pop16());
L0DD1:
    /* 0DD1  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_0849_DCC  (+DD2)
       In: AX = a view description (stored at 158h), BX nonzero to clear 15Ah. Clears 190h and
       sets ss:[580h] (dseg062_62a6) to -1, or to 0 when the description's words 0, 2 and 3 are
       0, 0A44h and 0. Nothing in the EXE calls it; what the record and the flag mean is not known. */
L0DD2: /* _seg004_0849_DD2 */
    /* 0DD2  push    ds */
    push16(asm_ds);
L0DD3:
    /* 0DD3  mov     cx,seg seg052_519C */
    CX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L0DD6:
    /* 0DD6  mov     ds,cx */
    SET_DS(CX);
L0DD8:
    /* 0DD8  mov     word ptr ds:[158h],ax */
    ww(pDS, 0x158, AX);
L0DDB:
    /* 0DDB  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L0DDD:
    /* 0DDD  je      short L0DE5 */
    if (ZF) goto L0DE5;
L0DDF:
    /* 0DDF  mov     word ptr ds:[15Ah],0 */
    ww(pDS, 0x15A, 0x0);
L0DE5: /* L0DE5 */
    /* 0DE5  mov     word ptr ds:[190h],0 */
    ww(pDS, 0x190, 0x0);
L0DEB:
    /* 0DEB  mov     word ptr ss:[580h],0FFFFh */
    ww(pSS, 0x580, 0xFFFF);
L0DF2:
    /* 0DF2  mov     di,ax */
    DI = AX;
L0DF4:
    /* 0DF4  cmp     word ptr [di],0 */
    sub16(rw(pDS, DI), 0x0, 0);
L0DF7:
    /* 0DF7  jne     short L0E0F */
    if (!ZF) goto L0E0F;
L0DF9:
    /* 0DF9  mov     ax,word ptr [di+4] */
    AX = rw(pDS, DI + 0x4);
L0DFC:
    /* 0DFC  cmp     ax,0A44h */
    sub16(AX, 0xA44, 0);
L0DFF:
    /* 0DFF  jne     short L0E0F */
    if (!ZF) goto L0E0F;
L0E01:
    /* 0E01  test    word ptr [di+6],0FFFFh */
    logic16((uint16_t)(rw(pDS, DI + 0x6) & 0xFFFF));
L0E06:
    /* 0E06  jne     short L0E0F */
    if (!ZF) goto L0E0F;
L0E08:
    /* 0E08  mov     word ptr ss:[580h],0 */
    ww(pSS, 0x580, 0x0);
L0E0F: /* L0E0F */
    /* 0E0F  pop     ds */
    SET_DS(pop16());
L0E10:
    /* 0E10  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_0849_E11  (+E11)
       A bare `int 2` and a jump to create_matrix's return; nothing calls it. */
L0E11: /* _seg004_0849_E11 */
    /* 0E11  int     2 */
    asm_halt_at(0x065C, 0x0E11, "int 2h, the debugger break");
L0E13:
    /* 0E13  jmp     short L0E51 */
    goto L0E51;

    /* seg004_0849_E15  (+E15)
       Builds the frame's view matrix. Requires the view description pointer at 158h to be 130h
       (int 2 otherwise), calls the camera routine its first word selects through the table at
       15Ch (head_view or lag_view), records 158h in 15Ah, and keeps a copy of the eye position
       (247Ah, 12 bytes) and of the matrix's depth column and 24E6h at 192h. Called by render_3d.
       Sets 168h to -1 first; head_view may set it to a model address. */
L0E15: /* _create_matrix */
    /* 0E15  mov     ax,0FFFFh */
    AX = 0xFFFF;
L0E18:
    /* 0E18  mov     word ptr ds:[168h],ax */
    ww(pDS, 0x168, AX);
L0E1B:
    /* 0E1B  mov     si,word ptr ds:[158h] */
    SI = rw(pDS, 0x158);
L0E1F:
    /* 0E1F  cmp     si,130h */
    sub16(SI, 0x130, 0);
L0E23:
    /* 0E23  je      short L0E27 */
    if (ZF) goto L0E27;
L0E25:
    /* 0E25  int     2 */
    asm_halt_at(0x065C, 0x0E25, "int 2h, the debugger break");
L0E27: /* L0E27 */
    /* 0E27  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0E28:
    /* 0E28  mov     bx,ax */
    BX = AX;
L0E2A:
    /* 0E2A  mov     ax,word ptr [bx+15Ch] */
    AX = rw(pDS, BX + 0x15C);
L0E2E:
    /* 0E2E  call    ax */
    if ((c = asm_call(ASM_JMP(0x065C, AX), 0x0E30)) != 0) return c;
L0E30:
    /* 0E30  mov     ax,word ptr ds:[158h] */
    AX = rw(pDS, 0x158);
L0E33:
    /* 0E33  mov     word ptr ds:[15Ah],ax */
    ww(pDS, 0x15A, AX);
L0E36:
    /* 0E36  mov     si,247Ah */
    SI = 0x247A;
L0E39:
    /* 0E39  mov     di,192h */
    DI = 0x192;
L0E3C:
    /* 0E3C  mov     cx,6 */
    CX = 0x6;
L0E3F:
    /* 0E3F  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0E41:
    /* 0E41  mov     ax,word ptr ds:[14B6h] */
    AX = rw(pDS, 0x14B6);
L0E44:
    /* 0E44  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0E45:
    /* 0E45  mov     ax,word ptr ds:[14BCh] */
    AX = rw(pDS, 0x14BC);
L0E48:
    /* 0E48  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0E49:
    /* 0E49  mov     ax,word ptr ds:[14C2h] */
    AX = rw(pDS, 0x14C2);
L0E4C:
    /* 0E4C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0E4D:
    /* 0E4D  mov     ax,word ptr ds:[24E6h] */
    AX = rw(pDS, 0x24E6);
L0E50:
    /* 0E50  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0E51: /* L0E51 */
    /* 0E51  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_E52  (+E52)
       The camera routine (FM head_view). In: SI = the view record after its selector word, ES =
       DS = seg052_519C. Reads the zoom word (24E6h) and the address BP of the eye record (and
       sets 168h when the third word is 0 and ss:[0A82h] is clear). On first use (the record's
       word +6 zero, then set to 1) it builds a two-angle matrix in the record
       (seg004_0849_1006) and, when its word +1Eh is nonzero, seg004_0849_EDD's offset. It builds
       the eye's matrix with angles_2_matrix when the eye record's first word is set, rotates the
       offset (mm3x9t) and adds the eye record's 32-bit coordinates to give the eye position at
       247Ah..2484h, and composes the final matrix at 14B2h (mm9x9). What each record field means
       beyond that has not been traced. */
L0E52: /* _head_view */
    /* 0E52  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0E53:
    /* 0E53  mov     word ptr ds:[24E6h],ax */
    ww(pDS, 0x24E6, AX);
L0E56:
    /* 0E56  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0E57:
    /* 0E57  mov     bp,ax */
    BP = AX;
L0E59:
    /* 0E59  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0E5A:
    /* 0E5A  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0E5C:
    /* 0E5C  jne     short L0E6F */
    if (!ZF) goto L0E6F;
L0E5E:
    /* 0E5E  test    word ptr ss:[0A82h],0FFFFh */
    logic16((uint16_t)(rw(pSS, 0xA82) & 0xFFFF));
L0E65:
    /* 0E65  jne     short L0E6F */
    if (!ZF) goto L0E6F;
L0E67:
    /* 0E67  mov     ax,bp */
    AX = BP;
L0E69:
    /* 0E69  sub     ax,4 */
    AX = (uint16_t)(AX - 0x4);
L0E6C:
    /* 0E6C  mov     word ptr ds:[168h],ax */
    ww(pDS, 0x168, AX);
L0E6F: /* L0E6F */
    /* 0E6F  test    word ptr [si+6],0FFFFh */
    logic16((uint16_t)(rw(pDS, SI + 0x6) & 0xFFFF));
L0E74:
    /* 0E74  jne     short L0E94 */
    if (!ZF) goto L0E94;
L0E76:
    /* 0E76  mov     word ptr [si+6],1 */
    ww(pDS, SI + 0x6, 0x1);
L0E7B:
    /* 0E7B  push    si */
    push16(SI);
L0E7C:
    /* 0E7C  push    bp */
    push16(BP);
L0E7D:
    /* 0E7D  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L0E80:
    /* 0E80  mov     di,si */
    DI = SI;
L0E82:
    /* 0E82  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L0E85:
    /* 0E85  call    _seg004_0849_1006 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x1006), 0x0E88)) != 0) return c;
L0E88:
    /* 0E88  pop     bp */
    BP = pop16();
L0E89:
    /* 0E89  pop     si */
    SI = pop16();
L0E8A:
    /* 0E8A  mov     di,word ptr [si+1Eh] */
    DI = rw(pDS, SI + 0x1E);
L0E8D:
    /* 0E8D  or      di,di */
    DI = logic16((uint16_t)(DI | DI));
L0E8F:
    /* 0E8F  je      short L0E94 */
    if (ZF) goto L0E94;
L0E91:
    /* 0E91  call    _seg004_0849_EDD */
    if ((c = asm_call(ASM_JMP(0x065C, 0x0EDD), 0x0E94)) != 0) return c;
L0E94: /* L0E94 */
    /* 0E94  add     bp,12h */
    BP = (uint16_t)(BP + 0x12);
L0E97:
    /* 0E97  test    word ptr [bp-12h],0FFFFh */
    logic16((uint16_t)(rw(pSS, BP + 0xFFEE) & 0xFFFF));
L0E9C:
    /* 0E9C  je      short L0EA3 */
    if (ZF) goto L0EA3;
L0E9E:
    /* 0E9E  push    si */
    push16(SI);
L0E9F:
    /* 0E9F  call    _angles_2_matrix */
    if ((c = asm_call(ASM_JMP(0x065C, 0x0F0C), 0x0EA2)) != 0) return c;
L0EA2:
    /* 0EA2  pop     si */
    SI = pop16();
L0EA3: /* L0EA3 */
    /* 0EA3  call    _mm3x9t */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3BE8), 0x0EA6)) != 0) return c;
L0EA6:
    /* 0EA6  mov     ax,bx */
    AX = BX;
L0EA8:
    /* 0EA8  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0EA9:
    /* 0EA9  add     ax,word ptr [bp-0Ch] */
    AX = add16(AX, rw(pSS, BP + 0xFFF4), 0);
L0EAC:
    /* 0EAC  adc     dx,word ptr [bp-0Ah] */
    DX = (uint16_t)(DX + rw(pSS, BP + 0xFFF6) + CF);
L0EAF:
    /* 0EAF  mov     word ptr ds:[247Ah],ax */
    ww(pDS, 0x247A, AX);
L0EB2:
    /* 0EB2  mov     word ptr ds:[247Ch],dx */
    ww(pDS, 0x247C, DX);
L0EB6:
    /* 0EB6  mov     ax,cx */
    AX = CX;
L0EB8:
    /* 0EB8  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0EB9:
    /* 0EB9  add     ax,word ptr [bp-8] */
    AX = add16(AX, rw(pSS, BP + 0xFFF8), 0);
L0EBC:
    /* 0EBC  adc     dx,word ptr [bp-6] */
    DX = (uint16_t)(DX + rw(pSS, BP + 0xFFFA) + CF);
L0EBF:
    /* 0EBF  mov     word ptr ds:[247Eh],ax */
    ww(pDS, 0x247E, AX);
L0EC2:
    /* 0EC2  mov     word ptr ds:[2480h],dx */
    ww(pDS, 0x2480, DX);
L0EC6:
    /* 0EC6  mov     ax,di */
    AX = DI;
L0EC8:
    /* 0EC8  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0EC9:
    /* 0EC9  add     ax,word ptr [bp-4] */
    AX = add16(AX, rw(pSS, BP + 0xFFFC), 0);
L0ECC:
    /* 0ECC  adc     dx,word ptr [bp-2] */
    DX = (uint16_t)(DX + rw(pSS, BP + 0xFFFE) + CF);
L0ECF:
    /* 0ECF  mov     word ptr ds:[2482h],ax */
    ww(pDS, 0x2482, AX);
L0ED2:
    /* 0ED2  mov     word ptr ds:[2484h],dx */
    ww(pDS, 0x2484, DX);
L0ED6:
    /* 0ED6  add     si,0Ch */
    SI = (uint16_t)(SI + 0xC);
L0ED9:
    /* 0ED9  call    _mm9x9 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3B04), 0x0EDC)) != 0) return c;
L0EDC:
    /* 0EDC  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_EDD  (+EDD)
       In: DI = a distance, SI = a matrix record. Writes -(DI * column) for the matrix column at
       SI+10h, +16h, +1Ch into SI+0..+4, a vector DI units back along that axis; probably the
       camera's offset behind the eye (inferred from head_view's use when the record's +1Eh word
       is set; not checked in play). */
L0EDD: /* _seg004_0849_EDD */
    /* 0EDD  mov     ax,di */
    AX = DI;
L0EDF:
    /* 0EDF  imul    word ptr [si+10h] */
    imul16(rw(pDS, SI + 0x10));
L0EE2:
    /* 0EE2  shl     ax,1 */
    AX = shl16(AX, 1);
L0EE4:
    /* 0EE4  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0EE6:
    /* 0EE6  neg     dx */
    DX = (uint16_t)-DX;
L0EE8:
    /* 0EE8  mov     word ptr [si],dx */
    ww(pDS, SI, DX);
L0EEA:
    /* 0EEA  mov     ax,di */
    AX = DI;
L0EEC:
    /* 0EEC  imul    word ptr [si+16h] */
    imul16(rw(pDS, SI + 0x16));
L0EEF:
    /* 0EEF  shl     ax,1 */
    AX = shl16(AX, 1);
L0EF1:
    /* 0EF1  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0EF3:
    /* 0EF3  neg     dx */
    DX = (uint16_t)-DX;
L0EF5:
    /* 0EF5  mov     word ptr [si+2],dx */
    ww(pDS, SI + 0x2, DX);
L0EF8:
    /* 0EF8  mov     ax,di */
    AX = DI;
L0EFA:
    /* 0EFA  imul    word ptr [si+1Ch] */
    imul16(rw(pDS, SI + 0x1C));
L0EFD:
    /* 0EFD  shl     ax,1 */
    AX = shl16(AX, 1);
L0EFF:
    /* 0EFF  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0F01:
    /* 0F01  neg     dx */
    DX = neg16(DX);
L0F03:
    /* 0F03  mov     word ptr [si+4],dx */
    ww(pDS, SI + 0x4, DX);
L0F06:
    /* 0F06  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0F07: /* lag_view */
    /* 0F07  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_F08  (+F08)
       Far wrapper for angles_2_matrix; nothing in the EXE calls it. */
L0F08: /* _seg004_0849_F08 */
    /* 0F08  call    _angles_2_matrix */
    if ((c = asm_call(ASM_JMP(0x065C, 0x0F0C), 0x0F0B)) != 0) return c;
L0F0B:
    /* 0F0B  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_0849_F0C  (+F0C)
       In: BP (and DI) = a record whose words +12h, +14h and +16h are three angles, ES = the 3D
       data segment. Calls seg021's sincos for each negated angle (with DS = SS, as sincos needs),
       keeping the sines and cosines at es:182h..18Ch, and writes the 3x3 rotation matrix they make
       to BP+0..+10h in 1.15 fixed point. Which angle is heading, pitch and bank has not been
       checked. */
L0F0C: /* _angles_2_matrix */
    /* 0F0C  mov     ax,ss */
    AX = asm_ss;
L0F0E:
    /* 0F0E  mov     ds,ax */
    SET_DS(AX);
L0F10:
    /* 0F10  push    bp */
    push16(BP);
L0F11:
    /* 0F11  mov     di,bp */
    DI = BP;
L0F13:
    /* 0F13  mov     bx,word ptr [di+12h] */
    BX = rw(pDS, DI + 0x12);
L0F16:
    /* 0F16  neg     bx */
    BX = (uint16_t)-BX;
L0F18:
    /* 0F18  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A34), 0x065C + PORT_LOAD_SEG, 0x0F1D)) != 0) return c;
L0F1D:
    /* 0F1D  mov     word ptr es:[18Ah],bx */
    ww(pES, 0x18A, BX);
L0F22:
    /* 0F22  mov     word ptr es:[18Ch],ax */
    ww(pES, 0x18C, AX);
L0F26:
    /* 0F26  mov     bx,word ptr [di+14h] */
    BX = rw(pDS, DI + 0x14);
L0F29:
    /* 0F29  neg     bx */
    BX = (uint16_t)-BX;
L0F2B:
    /* 0F2B  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A34), 0x065C + PORT_LOAD_SEG, 0x0F30)) != 0) return c;
L0F30:
    /* 0F30  mov     word ptr es:[186h],bx */
    ww(pES, 0x186, BX);
L0F35:
    /* 0F35  mov     word ptr es:[188h],ax */
    ww(pES, 0x188, AX);
L0F39:
    /* 0F39  mov     bx,word ptr [di+16h] */
    BX = rw(pDS, DI + 0x16);
L0F3C:
    /* 0F3C  neg     bx */
    BX = (uint16_t)-BX;
L0F3E:
    /* 0F3E  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A34), 0x065C + PORT_LOAD_SEG, 0x0F43)) != 0) return c;
L0F43:
    /* 0F43  mov     word ptr es:[182h],bx */
    ww(pES, 0x182, BX);
L0F48:
    /* 0F48  mov     word ptr es:[184h],ax */
    ww(pES, 0x184, AX);
L0F4C:
    /* 0F4C  pop     bp */
    BP = pop16();
L0F4D:
    /* 0F4D  mov     ax,es */
    AX = asm_es;
L0F4F:
    /* 0F4F  mov     ds,ax */
    SET_DS(AX);
L0F51:
    /* 0F51  mov     ax,word ptr ds:[18Ch] */
    AX = rw(pDS, 0x18C);
L0F54:
    /* 0F54  neg     ax */
    AX = (uint16_t)-AX;
L0F56:
    /* 0F56  mov     word ptr [bp+0Ah],ax */
    ww(pSS, BP + 0xA, AX);
L0F59:
    /* 0F59  mov     ax,word ptr ds:[186h] */
    AX = rw(pDS, 0x186);
L0F5C:
    /* 0F5C  imul    word ptr ds:[182h] */
    imul16(rw(pDS, 0x182));
L0F60:
    /* 0F60  shl     ax,1 */
    AX = shl16(AX, 1);
L0F62:
    /* 0F62  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0F64:
    /* 0F64  mov     bx,dx */
    BX = DX;
L0F66:
    /* 0F66  mov     ax,word ptr ds:[188h] */
    AX = rw(pDS, 0x188);
L0F69:
    /* 0F69  imul    word ptr ds:[184h] */
    imul16(rw(pDS, 0x184));
L0F6D:
    /* 0F6D  shl     ax,1 */
    AX = shl16(AX, 1);
L0F6F:
    /* 0F6F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0F71:
    /* 0F71  mov     di,dx */
    DI = DX;
L0F73:
    /* 0F73  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L0F74:
    /* 0F74  imul    word ptr ds:[18Ch] */
    imul16(rw(pDS, 0x18C));
L0F78:
    /* 0F78  shl     ax,1 */
    AX = shl16(AX, 1);
L0F7A:
    /* 0F7A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0F7C:
    /* 0F7C  add     dx,bx */
    DX = (uint16_t)(DX + BX);
L0F7E:
    /* 0F7E  mov     word ptr [bp],dx */
    ww(pSS, BP, DX);
L0F81:
    /* 0F81  mov     ax,word ptr ds:[188h] */
    AX = rw(pDS, 0x188);
L0F84:
    /* 0F84  imul    word ptr ds:[18Ah] */
    imul16(rw(pDS, 0x18A));
L0F88:
    /* 0F88  shl     ax,1 */
    AX = shl16(AX, 1);
L0F8A:
    /* 0F8A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0F8C:
    /* 0F8C  neg     dx */
    DX = (uint16_t)-DX;
L0F8E:
    /* 0F8E  mov     word ptr [bp+6],dx */
    ww(pSS, BP + 0x6, DX);
L0F91:
    /* 0F91  mov     ax,word ptr ds:[186h] */
    AX = rw(pDS, 0x186);
L0F94:
    /* 0F94  imul    word ptr ds:[184h] */
    imul16(rw(pDS, 0x184));
L0F98:
    /* 0F98  shl     ax,1 */
    AX = shl16(AX, 1);
L0F9A:
    /* 0F9A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0F9C:
    /* 0F9C  mov     cx,dx */
    CX = DX;
L0F9E:
    /* 0F9E  mov     ax,word ptr ds:[188h] */
    AX = rw(pDS, 0x188);
L0FA1:
    /* 0FA1  imul    word ptr ds:[182h] */
    imul16(rw(pDS, 0x182));
L0FA5:
    /* 0FA5  shl     ax,1 */
    AX = shl16(AX, 1);
L0FA7:
    /* 0FA7  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0FA9:
    /* 0FA9  mov     si,dx */
    SI = DX;
L0FAB:
    /* 0FAB  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L0FAC:
    /* 0FAC  imul    word ptr ds:[18Ch] */
    imul16(rw(pDS, 0x18C));
L0FB0:
    /* 0FB0  shl     ax,1 */
    AX = shl16(AX, 1);
L0FB2:
    /* 0FB2  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0FB4:
    /* 0FB4  sub     dx,cx */
    DX = (uint16_t)(DX - CX);
L0FB6:
    /* 0FB6  neg     dx */
    DX = (uint16_t)-DX;
L0FB8:
    /* 0FB8  mov     word ptr [bp+0Ch],dx */
    ww(pSS, BP + 0xC, DX);
L0FBB:
    /* 0FBB  mov     ax,cx */
    AX = CX;
L0FBD:
    /* 0FBD  imul    word ptr ds:[18Ch] */
    imul16(rw(pDS, 0x18C));
L0FC1:
    /* 0FC1  shl     ax,1 */
    AX = shl16(AX, 1);
L0FC3:
    /* 0FC3  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0FC5:
    /* 0FC5  sub     si,dx */
    SI = (uint16_t)(SI - DX);
L0FC7:
    /* 0FC7  mov     word ptr [bp+2],si */
    ww(pSS, BP + 0x2, SI);
L0FCA:
    /* 0FCA  mov     ax,word ptr ds:[186h] */
    AX = rw(pDS, 0x186);
L0FCD:
    /* 0FCD  imul    word ptr ds:[18Ah] */
    imul16(rw(pDS, 0x18A));
L0FD1:
    /* 0FD1  shl     ax,1 */
    AX = shl16(AX, 1);
L0FD3:
    /* 0FD3  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0FD5:
    /* 0FD5  mov     word ptr [bp+8],dx */
    ww(pSS, BP + 0x8, DX);
L0FD8:
    /* 0FD8  mov     ax,bx */
    AX = BX;
L0FDA:
    /* 0FDA  imul    word ptr ds:[18Ch] */
    imul16(rw(pDS, 0x18C));
L0FDE:
    /* 0FDE  shl     ax,1 */
    AX = shl16(AX, 1);
L0FE0:
    /* 0FE0  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0FE2:
    /* 0FE2  add     dx,di */
    DX = (uint16_t)(DX + DI);
L0FE4:
    /* 0FE4  mov     word ptr [bp+0Eh],dx */
    ww(pSS, BP + 0xE, DX);
L0FE7:
    /* 0FE7  mov     ax,word ptr ds:[184h] */
    AX = rw(pDS, 0x184);
L0FEA:
    /* 0FEA  imul    word ptr ds:[18Ah] */
    imul16(rw(pDS, 0x18A));
L0FEE:
    /* 0FEE  shl     ax,1 */
    AX = shl16(AX, 1);
L0FF0:
    /* 0FF0  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0FF2:
    /* 0FF2  neg     dx */
    DX = (uint16_t)-DX;
L0FF4:
    /* 0FF4  mov     word ptr [bp+4],dx */
    ww(pSS, BP + 0x4, DX);
L0FF7:
    /* 0FF7  mov     ax,word ptr ds:[182h] */
    AX = rw(pDS, 0x182);
L0FFA:
    /* 0FFA  imul    word ptr ds:[18Ah] */
    imul16(rw(pDS, 0x18A));
L0FFE:
    /* 0FFE  shl     ax,1 */
    AX = shl16(AX, 1);
L1000:
    /* 1000  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1002:
    /* 1002  mov     word ptr [bp+10h],dx */
    ww(pSS, BP + 0x10, DX);
L1005:
    /* 1005  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_1006  (+1006)
       Two-angle version of angles_2_matrix: angles at es:[si] and es:[si+2], matrix written to
       DI+0..+10h with element +6 zero (no roll term). Called by head_view for the eye's own
       rotation. */
L1006: /* _seg004_0849_1006 */
    /* 1006  mov     ax,ss */
    AX = asm_ss;
L1008:
    /* 1008  mov     ds,ax */
    SET_DS(AX);
L100A:
    /* 100A  mov     bx,word ptr es:[si] */
    BX = rw(pES, SI);
L100D:
    /* 100D  neg     bx */
    BX = (uint16_t)-BX;
L100F:
    /* 100F  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A34), 0x065C + PORT_LOAD_SEG, 0x1014)) != 0) return c;
L1014:
    /* 1014  mov     word ptr es:[18Ah],bx */
    ww(pES, 0x18A, BX);
L1019:
    /* 1019  mov     word ptr es:[18Ch],ax */
    ww(pES, 0x18C, AX);
L101D:
    /* 101D  mov     bx,word ptr es:[si+2] */
    BX = rw(pES, SI + 0x2);
L1021:
    /* 1021  neg     bx */
    BX = (uint16_t)-BX;
L1023:
    /* 1023  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0A34), 0x065C + PORT_LOAD_SEG, 0x1028)) != 0) return c;
L1028:
    /* 1028  mov     word ptr es:[182h],bx */
    ww(pES, 0x182, BX);
L102D:
    /* 102D  mov     word ptr es:[184h],ax */
    ww(pES, 0x184, AX);
L1031:
    /* 1031  mov     ax,es */
    AX = asm_es;
L1033:
    /* 1033  mov     ds,ax */
    SET_DS(AX);
L1035:
    /* 1035  mov     ax,word ptr ds:[182h] */
    AX = rw(pDS, 0x182);
L1038:
    /* 1038  mov     word ptr [di],ax */
    ww(pDS, DI, AX);
L103A:
    /* 103A  mov     ax,word ptr ds:[184h] */
    AX = rw(pDS, 0x184);
L103D:
    /* 103D  imul    word ptr ds:[18Ch] */
    imul16(rw(pDS, 0x18C));
L1041:
    /* 1041  shl     ax,1 */
    AX = shl16(AX, 1);
L1043:
    /* 1043  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1045:
    /* 1045  neg     dx */
    DX = (uint16_t)-DX;
L1047:
    /* 1047  mov     word ptr [di+2],dx */
    ww(pDS, DI + 0x2, DX);
L104A:
    /* 104A  mov     ax,word ptr ds:[184h] */
    AX = rw(pDS, 0x184);
L104D:
    /* 104D  imul    word ptr ds:[18Ah] */
    imul16(rw(pDS, 0x18A));
L1051:
    /* 1051  shl     ax,1 */
    AX = shl16(AX, 1);
L1053:
    /* 1053  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1055:
    /* 1055  neg     dx */
    DX = (uint16_t)-DX;
L1057:
    /* 1057  mov     word ptr [di+4],dx */
    ww(pDS, DI + 0x4, DX);
L105A:
    /* 105A  mov     word ptr [di+6],0 */
    ww(pDS, DI + 0x6, 0x0);
L105F:
    /* 105F  mov     ax,word ptr ds:[18Ah] */
    AX = rw(pDS, 0x18A);
L1062:
    /* 1062  mov     word ptr [di+8],ax */
    ww(pDS, DI + 0x8, AX);
L1065:
    /* 1065  mov     ax,word ptr ds:[18Ch] */
    AX = rw(pDS, 0x18C);
L1068:
    /* 1068  neg     ax */
    AX = (uint16_t)-AX;
L106A:
    /* 106A  mov     word ptr [di+0Ah],ax */
    ww(pDS, DI + 0xA, AX);
L106D:
    /* 106D  mov     ax,word ptr ds:[184h] */
    AX = rw(pDS, 0x184);
L1070:
    /* 1070  mov     word ptr [di+0Ch],ax */
    ww(pDS, DI + 0xC, AX);
L1073:
    /* 1073  mov     ax,word ptr ds:[182h] */
    AX = rw(pDS, 0x182);
L1076:
    /* 1076  imul    word ptr ds:[18Ch] */
    imul16(rw(pDS, 0x18C));
L107A:
    /* 107A  shl     ax,1 */
    AX = shl16(AX, 1);
L107C:
    /* 107C  rcl     dx,1 */
    DX = rcl16(DX, 1);
L107E:
    /* 107E  mov     word ptr [di+0Eh],dx */
    ww(pDS, DI + 0xE, DX);
L1081:
    /* 1081  mov     ax,word ptr ds:[182h] */
    AX = rw(pDS, 0x182);
L1084:
    /* 1084  imul    word ptr ds:[18Ah] */
    imul16(rw(pDS, 0x18A));
L1088:
    /* 1088  shl     ax,1 */
    AX = shl16(AX, 1);
L108A:
    /* 108A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L108C:
    /* 108C  mov     word ptr [di+10h],dx */
    ww(pDS, DI + 0x10, DX);
L108F:
    /* 108F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_1090  (+1090)
       Opcode for every unused table slot: `int 2`, then carries on with the next opcode. A
       program that reaches it is malformed; int 2 is NMI, so presumably a debugger catch. */
L1090: /* _do_int2 */
    /* 1090  int     2 */
    asm_halt_at(0x065C, 0x1090, "int 2h, the debugger break");
L1092:
    /* 1092  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1093:
    /* 1093  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1094:
    /* 1094  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1098  (+1098)
       Opcode 00h: end of a program or sub-program. Returns from the `call [bx+24F4h]` that
       started it (render_3d, do_sfcal, a sort or call opcode). */
L1098: /* _do_eof */
    /* 1098  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_1099  (+1099)
       Opcode 08h, operand: point. Projects the point and plots one pixel through seg003
       (_seg003_0272_0) when it is in front of the eye with no clip code (or always, when byte 25D3h
       is set). */
L1099: /* _do_pntres */
    /* 1099  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L109A:
    /* 109A  mov     di,ax */
    DI = AX;
L109C:
    /* 109C  add     di,14D0h */
    DI = (uint16_t)(DI + 0x14D0);
L10A0:
    /* 10A0  test    byte ptr ds:[25D3h],0FFh */
    logic8((uint8_t)(rb(pDS, 0x25D3) & 0xFF));
L10A5:
    /* 10A5  jne     short L10AD */
    if (!ZF) goto L10AD;
L10A7:
    /* 10A7  test    byte ptr [di+6],0FFh */
    logic8((uint8_t)(rb(pDS, DI + 0x6) & 0xFF));
L10AB:
    /* 10AB  jne     short L10D2 */
    if (!ZF) goto L10D2;
L10AD: /* L10AD */
    /* 10AD  mov     cx,word ptr [di+4] */
    CX = rw(pDS, DI + 0x4);
L10B0:
    /* 10B0  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L10B3:
    /* 10B3  imul    word ptr ds:[2470h] */
    imul16(rw(pDS, 0x2470));
L10B7:
    /* 10B7  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x10B7, 2)) != 0) return c;
L10B9:
    /* 10B9  add     ax,word ptr ds:[2476h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2476));
L10BD:
    /* 10BD  mov     bx,ax */
    BX = AX;
L10BF:
    /* 10BF  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L10C1:
    /* 10C1  imul    word ptr ds:[2472h] */
    imul16(rw(pDS, 0x2472));
L10C5:
    /* 10C5  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x10C5, 2)) != 0) return c;
L10C7:
    /* 10C7  add     ax,word ptr ds:[2474h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2474));
L10CB:
    /* 10CB  push    si */
    push16(SI);
L10CC:
    /* 10CC  call    far ptr _seg003_0272_0 */
    if ((c = asm_callf(ASM_JMP(0x0085, 0x0000), 0x065C + PORT_LOAD_SEG, 0x10D1)) != 0) return c;
L10D1:
    /* 10D1  pop     si */
    SI = pop16();
L10D2: /* L10D2 */
    /* 10D2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L10D3:
    /* 10D3  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L10D4:
    /* 10D4  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_10D8  (+10D8)
       Opcode 96h, operands: two points. Draws a line (seg003 _seg003_0272_8, or _10 after
       clipping). Lines whose codes AND to nonzero are skipped; others are clipped in 3D here, one
       plane at a time, inline (FM has these sections as lclip_top, lclip_bot, lclip_left and
       lclip_right). The two entry points after the first dispatch (11ABh and 11ADh) are the
       divide-overflow recovery addresses stored at ss:[5A5h] while projecting each endpoint (FM
       line_overflow2): they drop the interrupt frame and clip that endpoint. The clipped endpoint
       is written to a scratch point at 147Ah or 1482h. */
L10D8: /* _do_lnres */
    /* 10D8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L10D9:
    /* 10D9  mov     di,ax */
    DI = AX;
L10DB:
    /* 10DB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L10DC:
    /* 10DC  mov     bx,ax */
    BX = AX;
L10DE:
    /* 10DE  mov     al,byte ptr [di+14D6h] */
    AL = rb(pDS, DI + 0x14D6);
L10E2:
    /* 10E2  test    byte ptr [bx+14D6h],al */
    logic8((uint8_t)(rb(pDS, BX + 0x14D6) & AL));
L10E6:
    /* 10E6  jne     short L113F */
    if (!ZF) goto L113F;
L10E8:
    /* 10E8  push    si */
    push16(SI);
L10E9:
    /* 10E9  or      al,byte ptr [bx+14D6h] */
    AL = logic8((uint8_t)(AL | rb(pDS, BX + 0x14D6)));
L10ED:
    /* 10ED  jne     short L1145 */
    if (!ZF) goto L1145;
L10EF:
    /* 10EF  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L10F3:
    /* 10F3  mov     ax,word ptr [di+14D0h] */
    AX = rw(pDS, DI + 0x14D0);
L10F7:
    /* 10F7  imul    word ptr ds:[2472h] */
    imul16(rw(pDS, 0x2472));
L10FB:
    /* 10FB  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x10FB, 2)) != 0) return c;
L10FD:
    /* 10FD  add     ax,word ptr ds:[2474h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2474));
L1101:
    /* 1101  mov     cx,ax */
    CX = AX;
L1103:
    /* 1103  mov     ax,word ptr [di+14D2h] */
    AX = rw(pDS, DI + 0x14D2);
L1107:
    /* 1107  imul    word ptr ds:[2470h] */
    imul16(rw(pDS, 0x2470));
L110B:
    /* 110B  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x110B, 2)) != 0) return c;
L110D:
    /* 110D  add     ax,word ptr ds:[2476h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2476));
L1111:
    /* 1111  mov     si,ax */
    SI = AX;
L1113:
    /* 1113  mov     di,bx */
    DI = BX;
L1115:
    /* 1115  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L1119:
    /* 1119  mov     ax,word ptr [di+14D2h] */
    AX = rw(pDS, DI + 0x14D2);
L111D:
    /* 111D  imul    word ptr ds:[2470h] */
    imul16(rw(pDS, 0x2470));
L1121:
    /* 1121  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x1121, 2)) != 0) return c;
L1123:
    /* 1123  add     ax,word ptr ds:[2476h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2476));
L1127:
    /* 1127  mov     bx,ax */
    BX = AX;
L1129:
    /* 1129  mov     ax,word ptr [di+14D0h] */
    AX = rw(pDS, DI + 0x14D0);
L112D:
    /* 112D  imul    word ptr ds:[2472h] */
    imul16(rw(pDS, 0x2472));
L1131:
    /* 1131  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x1131, 2)) != 0) return c;
L1133:
    /* 1133  add     ax,word ptr ds:[2474h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2474));
L1137:
    /* 1137  mov     dx,si */
    DX = SI;
L1139:
    /* 1139  call    far ptr _seg003_0272_8 */
    if ((c = asm_callf(ASM_JMP(0x0085, 0x0008), 0x065C + PORT_LOAD_SEG, 0x113E)) != 0) return c;
L113E: /* L113E */
    /* 113E  pop     si */
    SI = pop16();
L113F: /* L113F */
    /* 113F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1140:
    /* 1140  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1141:
    /* 1141  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L1145: /* L1145 */
    /* 1145  add     bx,14D0h */
    BX = (uint16_t)(BX + 0x14D0);
L1149:
    /* 1149  add     di,14D0h */
    DI = (uint16_t)(DI + 0x14D0);
L114D:
    /* 114D  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L114F:
    /* 114F  js      short L11C9 */
    if (SF) goto L11C9;
L1151: /* L1151 */
    /* 1151  mov     word ptr ss:[5A5h],11ABh */
    ww(pSS, 0x5A5, 0x11AB);
L1158:
    /* 1158  mov     bp,word ptr [bx+4] */
    BP = rw(pDS, BX + 0x4);
L115B:
    /* 115B  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L115D:
    /* 115D  imul    word ptr ds:[2472h] */
    imul16(rw(pDS, 0x2472));
L1161:
    /* 1161  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x1161, 2)) != 0) return c;
L1163:
    /* 1163  add     ax,word ptr ds:[2474h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2474));
L1167:
    /* 1167  mov     cx,ax */
    CX = AX;
L1169:
    /* 1169  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L116C:
    /* 116C  imul    word ptr ds:[2470h] */
    imul16(rw(pDS, 0x2470));
L1170:
    /* 1170  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x1170, 2)) != 0) return c;
L1172:
    /* 1172  add     ax,word ptr ds:[2476h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2476));
L1176:
    /* 1176  mov     si,ax */
    SI = AX;
L1178:
    /* 1178  mov     word ptr ss:[5A5h],11ADh */
    ww(pSS, 0x5A5, 0x11AD);
L117F:
    /* 117F  mov     bp,word ptr [di+4] */
    BP = rw(pDS, DI + 0x4);
L1182:
    /* 1182  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L1185:
    /* 1185  imul    word ptr ds:[2470h] */
    imul16(rw(pDS, 0x2470));
L1189:
    /* 1189  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x1189, 2)) != 0) return c;
L118B:
    /* 118B  add     ax,word ptr ds:[2476h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2476));
L118F:
    /* 118F  mov     bx,ax */
    BX = AX;
L1191:
    /* 1191  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L1193:
    /* 1193  imul    word ptr ds:[2472h] */
    imul16(rw(pDS, 0x2472));
L1197:
    /* 1197  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x1197, 2)) != 0) return c;
L1199:
    /* 1199  add     ax,word ptr ds:[2474h] */
    AX = (uint16_t)(AX + rw(pDS, 0x2474));
L119D:
    /* 119D  mov     dx,si */
    DX = SI;
L119F:
    /* 119F  call    far ptr _seg003_0272_10 */
    if ((c = asm_callf(ASM_JMP(0x0085, 0x0010), 0x065C + PORT_LOAD_SEG, 0x11A4)) != 0) return c;
L11A4:
    /* 11A4  pop     si */
    SI = pop16();
L11A5:
    /* 11A5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L11A6:
    /* 11A6  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L11A7:
    /* 11A7  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L11AB:
    /* 11AB  xchg    bx,di */
    { uint16_t t_ = DI;
    DI = BX;
    BX = t_; }
L11AD:
    /* 11AD  sti */
    ;
L11AE:
    /* 11AE  add     sp,6 */
    SP = (uint16_t)(SP + 0x6);
L11B1:
    /* 11B1  mov     al,byte ptr [di+6] */
    AL = rb(pDS, DI + 0x6);
L11B4:
    /* 11B4  test    al,1 */
    logic8((uint8_t)(AL & 0x1));
L11B6:
    /* 11B6  je      short L11BB */
    if (ZF) goto L11BB;
L11B8:
    /* 11B8  jmp     L12D3 */
    goto L12D3;
L11BB: /* L11BB */
    /* 11BB  test    al,2 */
    logic8((uint8_t)(AL & 0x2));
L11BD:
    /* 11BD  je      short L11C2 */
    if (ZF) goto L11C2;
L11BF:
    /* 11BF  jmp     L134D */
    goto L134D;
L11C2: /* L11C2 */
    /* 11C2  test    al,4 */
    logic8((uint8_t)(AL & 0x4));
L11C4:
    /* 11C4  jne     short L11D9 */
    if (!ZF) goto L11D9;
L11C6:
    /* 11C6  jmp     L1258 */
    goto L1258;
L11C9: /* L11C9 */
    /* 11C9  mov     al,byte ptr [di+6] */
    AL = rb(pDS, DI + 0x6);
L11CC:
    /* 11CC  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L11CE:
    /* 11CE  js      short L11D5 */
    if (SF) goto L11D5;
L11D0:
    /* 11D0  xchg    di,bx */
    { uint16_t t_ = BX;
    BX = DI;
    DI = t_; }
L11D2:
    /* 11D2  mov     al,byte ptr [di+6] */
    AL = rb(pDS, DI + 0x6);
L11D5: /* L11D5 */
    /* 11D5  test    al,4 */
    logic8((uint8_t)(AL & 0x4));
L11D7:
    /* 11D7  je      short L1258 */
    if (ZF) goto L1258;
L11D9: /* L11D9 */
    /* 11D9  mov     word ptr ss:[5A5h],offset _overflow_handler_reg */
    ww(pSS, 0x5A5, 0x3C91);
L11E0:
    /* 11E0  mov     dx,word ptr [bx+4] */
    DX = rw(pDS, BX + 0x4);
L11E3:
    /* 11E3  sub     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX - rw(pDS, BX + 0x2));
L11E6:
    /* 11E6  mov     cx,word ptr [di+2] */
    CX = rw(pDS, DI + 0x2);
L11E9:
    /* 11E9  sub     cx,word ptr [bx+2] */
    CX = (uint16_t)(CX - rw(pDS, BX + 0x2));
L11EC:
    /* 11EC  sub     cx,word ptr [di+4] */
    CX = (uint16_t)(CX - rw(pDS, DI + 0x4));
L11EF:
    /* 11EF  add     cx,word ptr [bx+4] */
    CX = (uint16_t)(CX + rw(pDS, BX + 0x4));
L11F2:
    /* 11F2  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L11F4:
    /* 11F4  sar     dx,1 */
    DX = sar16(DX, 1);
L11F6:
    /* 11F6  rcr     ax,1 */
    AX = rcr16(AX, 1);
L11F8:
    /* 11F8  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x11F8, 2)) != 0) return c;
L11FA:
    /* 11FA  mov     bp,ax */
    BP = AX;
L11FC:
    /* 11FC  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L11FF:
    /* 11FF  sub     ax,word ptr [bx+2] */
    AX = (uint16_t)(AX - rw(pDS, BX + 0x2));
L1202:
    /* 1202  imul    bp */
    imul16(BP);
L1204:
    /* 1204  shl     ax,1 */
    AX = shl16(AX, 1);
L1206:
    /* 1206  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1208:
    /* 1208  shl     ax,1 */
    AX = shl16(AX, 1);
L120A:
    /* 120A  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L120D:
    /* 120D  add     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX + rw(pDS, BX + 0x2));
L1210:
    /* 1210  or      dx,dx */
    DX = logic16((uint16_t)(DX | DX));
L1212:
    /* 1212  js      short L1258 */
    if (SF) goto L1258;
L1214:
    /* 1214  mov     cx,dx */
    CX = DX;
L1216:
    /* 1216  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L1218:
    /* 1218  sub     ax,word ptr [bx] */
    AX = (uint16_t)(AX - rw(pDS, BX));
L121A:
    /* 121A  imul    bp */
    imul16(BP);
L121C:
    /* 121C  shl     ax,1 */
    AX = shl16(AX, 1);
L121E:
    /* 121E  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1220:
    /* 1220  shl     ax,1 */
    AX = shl16(AX, 1);
L1222:
    /* 1222  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L1225:
    /* 1225  add     dx,word ptr [bx] */
    DX = (uint16_t)(DX + rw(pDS, BX));
L1227:
    /* 1227  mov     di,1482h */
    DI = 0x1482;
L122A:
    /* 122A  cmp     di,bx */
    sub16(DI, BX, 0);
L122C:
    /* 122C  jne     short L1231 */
    if (!ZF) goto L1231;
L122E:
    /* 122E  mov     si,147Ah */
    SI = 0x147A;
L1231: /* L1231 */
    /* 1231  mov     word ptr [di],dx */
    ww(pDS, DI, DX);
L1233:
    /* 1233  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L1236:
    /* 1236  mov     word ptr [di+4],cx */
    ww(pDS, DI + 0x4, CX);
L1239:
    /* 1239  jcxz    L123D */
    if (!CX) goto L123D;
L123B:
    /* 123B  jmp     short L1240 */
    goto L1240;
L123D: /* L123D */
    /* 123D  jmp     L113E */
    goto L113E;
L1240: /* L1240 */
    /* 1240  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L1242:
    /* 1242  cmp     dx,cx */
    sub16(DX, CX, 0);
L1244:
    /* 1244  jle     short L124A */
    if (ZF || SF != OF) goto L124A;
L1246:
    /* 1246  inc     al */
    AL = (uint8_t)(AL + 1);
L1248:
    /* 1248  inc     al */
    AL = (uint8_t)(AL + 1);
L124A: /* L124A */
    /* 124A  neg     cx */
    CX = (uint16_t)-CX;
L124C:
    /* 124C  cmp     dx,cx */
    sub16(DX, CX, 0);
L124E:
    /* 124E  jge     short L1252 */
    if (SF == OF) goto L1252;
L1250:
    /* 1250  inc     al */
    AL = (uint8_t)(AL + 1);
L1252: /* L1252 */
    /* 1252  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L1255:
    /* 1255  jmp     L1151 */
    goto L1151;
L1258: /* L1258 */
    /* 1258  mov     word ptr ss:[5A5h],offset _overflow_handler_reg */
    ww(pSS, 0x5A5, 0x3C91);
L125F:
    /* 125F  mov     dx,word ptr [bx+4] */
    DX = rw(pDS, BX + 0x4);
L1262:
    /* 1262  add     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX + rw(pDS, BX + 0x2));
L1265:
    /* 1265  mov     cx,word ptr [bx+2] */
    CX = rw(pDS, BX + 0x2);
L1268:
    /* 1268  sub     cx,word ptr [di+2] */
    CX = (uint16_t)(CX - rw(pDS, DI + 0x2));
L126B:
    /* 126B  sub     cx,word ptr [di+4] */
    CX = (uint16_t)(CX - rw(pDS, DI + 0x4));
L126E:
    /* 126E  add     cx,word ptr [bx+4] */
    CX = (uint16_t)(CX + rw(pDS, BX + 0x4));
L1271:
    /* 1271  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L1273:
    /* 1273  sar     dx,1 */
    DX = sar16(DX, 1);
L1275:
    /* 1275  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1277:
    /* 1277  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x1277, 2)) != 0) return c;
L1279:
    /* 1279  mov     bp,ax */
    BP = AX;
L127B:
    /* 127B  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L127E:
    /* 127E  sub     ax,word ptr [bx+2] */
    AX = (uint16_t)(AX - rw(pDS, BX + 0x2));
L1281:
    /* 1281  imul    bp */
    imul16(BP);
L1283:
    /* 1283  shl     ax,1 */
    AX = shl16(AX, 1);
L1285:
    /* 1285  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1287:
    /* 1287  shl     ax,1 */
    AX = shl16(AX, 1);
L1289:
    /* 1289  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L128C:
    /* 128C  add     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX + rw(pDS, BX + 0x2));
L128F:
    /* 128F  mov     cx,dx */
    CX = DX;
L1291:
    /* 1291  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L1293:
    /* 1293  sub     ax,word ptr [bx] */
    AX = (uint16_t)(AX - rw(pDS, BX));
L1295:
    /* 1295  imul    bp */
    imul16(BP);
L1297:
    /* 1297  shl     ax,1 */
    AX = shl16(AX, 1);
L1299:
    /* 1299  rcl     dx,1 */
    DX = rcl16(DX, 1);
L129B:
    /* 129B  shl     ax,1 */
    AX = shl16(AX, 1);
L129D:
    /* 129D  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L12A0:
    /* 12A0  add     dx,word ptr [bx] */
    DX = (uint16_t)(DX + rw(pDS, BX));
L12A2:
    /* 12A2  mov     di,1482h */
    DI = 0x1482;
L12A5:
    /* 12A5  cmp     di,bx */
    sub16(DI, BX, 0);
L12A7:
    /* 12A7  jne     short L12AC */
    if (!ZF) goto L12AC;
L12A9:
    /* 12A9  mov     si,147Ah */
    SI = 0x147A;
L12AC: /* L12AC */
    /* 12AC  mov     word ptr [di],dx */
    ww(pDS, DI, DX);
L12AE:
    /* 12AE  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L12B1:
    /* 12B1  jcxz    L12B5 */
    if (!CX) goto L12B5;
L12B3:
    /* 12B3  jmp     short L12B8 */
    goto L12B8;
L12B5: /* L12B5 */
    /* 12B5  jmp     L113E */
    goto L113E;
L12B8: /* L12B8 */
    /* 12B8  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L12BA:
    /* 12BA  cmp     dx,cx */
    sub16(DX, CX, 0);
L12BC:
    /* 12BC  jge     short L12C0 */
    if (SF == OF) goto L12C0;
L12BE:
    /* 12BE  inc     al */
    AL = (uint8_t)(AL + 1);
L12C0: /* L12C0 */
    /* 12C0  neg     cx */
    CX = (uint16_t)-CX;
L12C2:
    /* 12C2  cmp     dx,cx */
    sub16(DX, CX, 0);
L12C4:
    /* 12C4  jle     short L12CA */
    if (ZF || SF != OF) goto L12CA;
L12C6:
    /* 12C6  inc     al */
    AL = (uint8_t)(AL + 1);
L12C8:
    /* 12C8  inc     al */
    AL = (uint8_t)(AL + 1);
L12CA: /* L12CA */
    /* 12CA  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L12CD:
    /* 12CD  mov     word ptr [di+4],cx */
    ww(pDS, DI + 0x4, CX);
L12D0:
    /* 12D0  jmp     L1151 */
    goto L1151;
L12D3: /* L12D3 */
    /* 12D3  mov     word ptr ss:[5A5h],offset _overflow_handler_reg */
    ww(pSS, 0x5A5, 0x3C91);
L12DA:
    /* 12DA  mov     dx,word ptr [bx+4] */
    DX = rw(pDS, BX + 0x4);
L12DD:
    /* 12DD  add     dx,word ptr [bx] */
    DX = (uint16_t)(DX + rw(pDS, BX));
L12DF:
    /* 12DF  mov     cx,word ptr [bx] */
    CX = rw(pDS, BX);
L12E1:
    /* 12E1  sub     cx,word ptr [di] */
    CX = (uint16_t)(CX - rw(pDS, DI));
L12E3:
    /* 12E3  sub     cx,word ptr [di+4] */
    CX = (uint16_t)(CX - rw(pDS, DI + 0x4));
L12E6:
    /* 12E6  add     cx,word ptr [bx+4] */
    CX = (uint16_t)(CX + rw(pDS, BX + 0x4));
L12E9:
    /* 12E9  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L12EB:
    /* 12EB  sar     dx,1 */
    DX = sar16(DX, 1);
L12ED:
    /* 12ED  rcr     ax,1 */
    AX = rcr16(AX, 1);
L12EF:
    /* 12EF  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x12EF, 2)) != 0) return c;
L12F1:
    /* 12F1  mov     bp,ax */
    BP = AX;
L12F3:
    /* 12F3  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L12F5:
    /* 12F5  sub     ax,word ptr [bx] */
    AX = (uint16_t)(AX - rw(pDS, BX));
L12F7:
    /* 12F7  imul    bp */
    imul16(BP);
L12F9:
    /* 12F9  shl     ax,1 */
    AX = shl16(AX, 1);
L12FB:
    /* 12FB  rcl     dx,1 */
    DX = rcl16(DX, 1);
L12FD:
    /* 12FD  shl     ax,1 */
    AX = shl16(AX, 1);
L12FF:
    /* 12FF  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L1302:
    /* 1302  add     dx,word ptr [bx] */
    DX = (uint16_t)(DX + rw(pDS, BX));
L1304:
    /* 1304  or      dx,dx */
    DX = logic16((uint16_t)(DX | DX));
L1306:
    /* 1306  jns     short L134D */
    if (!SF) goto L134D;
L1308:
    /* 1308  mov     cx,dx */
    CX = DX;
L130A:
    /* 130A  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L130D:
    /* 130D  sub     ax,word ptr [bx+2] */
    AX = (uint16_t)(AX - rw(pDS, BX + 0x2));
L1310:
    /* 1310  imul    bp */
    imul16(BP);
L1312:
    /* 1312  shl     ax,1 */
    AX = shl16(AX, 1);
L1314:
    /* 1314  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1316:
    /* 1316  shl     ax,1 */
    AX = shl16(AX, 1);
L1318:
    /* 1318  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L131B:
    /* 131B  add     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX + rw(pDS, BX + 0x2));
L131E:
    /* 131E  mov     di,1482h */
    DI = 0x1482;
L1321:
    /* 1321  cmp     di,bx */
    sub16(DI, BX, 0);
L1323:
    /* 1323  jne     short L1328 */
    if (!ZF) goto L1328;
L1325:
    /* 1325  mov     si,147Ah */
    SI = 0x147A;
L1328: /* L1328 */
    /* 1328  mov     word ptr [di],cx */
    ww(pDS, DI, CX);
L132A:
    /* 132A  mov     word ptr [di+2],dx */
    ww(pDS, DI + 0x2, DX);
L132D:
    /* 132D  jcxz    L1331 */
    if (!CX) goto L1331;
L132F:
    /* 132F  jmp     short L1334 */
    goto L1334;
L1331: /* L1331 */
    /* 1331  jmp     L113E */
    goto L113E;
L1334: /* L1334 */
    /* 1334  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L1336:
    /* 1336  cmp     dx,cx */
    sub16(DX, CX, 0);
L1338:
    /* 1338  jge     short L133C */
    if (SF == OF) goto L133C;
L133A:
    /* 133A  or      al,8 */
    AL = (uint8_t)(AL | 0x8);
L133C: /* L133C */
    /* 133C  neg     cx */
    CX = (uint16_t)-CX;
L133E:
    /* 133E  cmp     dx,cx */
    sub16(DX, CX, 0);
L1340:
    /* 1340  jge     short L1344 */
    if (SF == OF) goto L1344;
L1342:
    /* 1342  or      al,4 */
    AL = (uint8_t)(AL | 0x4);
L1344: /* L1344 */
    /* 1344  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L1347:
    /* 1347  mov     word ptr [di+4],cx */
    ww(pDS, DI + 0x4, CX);
L134A:
    /* 134A  jmp     L1151 */
    goto L1151;
L134D: /* L134D */
    /* 134D  mov     word ptr ss:[5A5h],offset _overflow_handler_reg */
    ww(pSS, 0x5A5, 0x3C91);
L1354:
    /* 1354  mov     dx,word ptr [bx] */
    DX = rw(pDS, BX);
L1356:
    /* 1356  sub     dx,word ptr [bx+4] */
    DX = (uint16_t)(DX - rw(pDS, BX + 0x4));
L1359:
    /* 1359  mov     cx,word ptr [di+4] */
    CX = rw(pDS, DI + 0x4);
L135C:
    /* 135C  sub     cx,word ptr [bx+4] */
    CX = (uint16_t)(CX - rw(pDS, BX + 0x4));
L135F:
    /* 135F  add     cx,word ptr [bx] */
    CX = (uint16_t)(CX + rw(pDS, BX));
L1361:
    /* 1361  sub     cx,word ptr [di] */
    CX = (uint16_t)(CX - rw(pDS, DI));
L1363:
    /* 1363  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L1365:
    /* 1365  sar     dx,1 */
    DX = sar16(DX, 1);
L1367:
    /* 1367  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1369:
    /* 1369  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x1369, 2)) != 0) return c;
L136B:
    /* 136B  mov     bp,ax */
    BP = AX;
L136D:
    /* 136D  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L1370:
    /* 1370  sub     ax,word ptr [bx+2] */
    AX = (uint16_t)(AX - rw(pDS, BX + 0x2));
L1373:
    /* 1373  imul    bp */
    imul16(BP);
L1375:
    /* 1375  shl     ax,1 */
    AX = shl16(AX, 1);
L1377:
    /* 1377  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1379:
    /* 1379  shl     ax,1 */
    AX = shl16(AX, 1);
L137B:
    /* 137B  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L137E:
    /* 137E  add     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX + rw(pDS, BX + 0x2));
L1381:
    /* 1381  mov     cx,dx */
    CX = DX;
L1383:
    /* 1383  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L1385:
    /* 1385  sub     ax,word ptr [bx] */
    AX = (uint16_t)(AX - rw(pDS, BX));
L1387:
    /* 1387  imul    bp */
    imul16(BP);
L1389:
    /* 1389  shl     ax,1 */
    AX = shl16(AX, 1);
L138B:
    /* 138B  rcl     dx,1 */
    DX = rcl16(DX, 1);
L138D:
    /* 138D  shl     ax,1 */
    AX = shl16(AX, 1);
L138F:
    /* 138F  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L1392:
    /* 1392  add     dx,word ptr [bx] */
    DX = (uint16_t)(DX + rw(pDS, BX));
L1394:
    /* 1394  mov     di,1482h */
    DI = 0x1482;
L1397:
    /* 1397  cmp     di,bx */
    sub16(DI, BX, 0);
L1399:
    /* 1399  jne     short L139E */
    if (!ZF) goto L139E;
L139B:
    /* 139B  mov     si,147Ah */
    SI = 0x147A;
L139E: /* L139E */
    /* 139E  mov     word ptr [di],dx */
    ww(pDS, DI, DX);
L13A0:
    /* 13A0  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L13A3:
    /* 13A3  mov     word ptr [di+4],dx */
    ww(pDS, DI + 0x4, DX);
L13A6:
    /* 13A6  jcxz    L13AA */
    if (!CX) goto L13AA;
L13A8:
    /* 13A8  jmp     short L13AD */
    goto L13AD;
L13AA: /* L13AA */
    /* 13AA  jmp     L113E */
    goto L113E;
L13AD: /* L13AD */
    /* 13AD  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L13AF:
    /* 13AF  cmp     cx,dx */
    sub16(CX, DX, 0);
L13B1:
    /* 13B1  jle     short L13B5 */
    if (ZF || SF != OF) goto L13B5;
L13B3:
    /* 13B3  mov     al,4 */
    AL = 0x4;
L13B5: /* L13B5 */
    /* 13B5  neg     dx */
    DX = (uint16_t)-DX;
L13B7:
    /* 13B7  cmp     cx,dx */
    sub16(CX, DX, 0);
L13B9:
    /* 13B9  jge     short L13BE */
    if (SF == OF) goto L13BE;
L13BB:
    /* 13BB  mov     ax,8 */
    AX = 0x8;
L13BE: /* L13BE */
    /* 13BE  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L13C1:
    /* 13C1  jmp     L1151 */
    goto L1151;

    /* seg004_0849_13C4  (+13C4)
       Opcode BEh, operands: a, b. Model variables: *[b] = [a] (b holds the address to store to). */
L13C4: /* _do_movei */
    /* 13C4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13C5:
    /* 13C5  mov     di,ax */
    DI = AX;
L13C7:
    /* 13C7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13C8:
    /* 13C8  mov     bx,ax */
    BX = AX;
L13CA:
    /* 13CA  mov     bx,word ptr [bx] */
    BX = rw(pDS, BX);
L13CC:
    /* 13CC  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L13CE:
    /* 13CE  mov     word ptr [bx],ax */
    ww(pDS, BX, AX);
L13D0:
    /* 13D0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13D1:
    /* 13D1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L13D2:
    /* 13D2  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_13D6  (+13D6)
       Opcode 02h, operands: address, value. Stores the constant in the model variable at address
       (GRIDDB.C's Clk(n) slots). */
L13D6: /* _do_movec */
    /* 13D6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13D7:
    /* 13D7  mov     di,ax */
    DI = AX;
L13D9:
    /* 13D9  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L13DA:
    /* 13DA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13DB:
    /* 13DB  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L13DC:
    /* 13DC  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_13E0  (+13E0)
       Opcode 04h, operands: a, b. Model variables: [b] = [a]. */
L13E0: /* _do_movem */
    /* 13E0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13E1:
    /* 13E1  mov     bx,ax */
    BX = AX;
L13E3:
    /* 13E3  mov     bx,word ptr [bx] */
    BX = rw(pDS, BX);
L13E5:
    /* 13E5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13E6:
    /* 13E6  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L13E7:
    /* 13E7  mov     word ptr [bx],ax */
    ww(pDS, BX, AX);
L13E9:
    /* 13E9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13EA:
    /* 13EA  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L13EB:
    /* 13EB  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_13EF  (+13EF)
       Opcode 0Ah, operands: offset, address. [address] = ss:[BP + offset] (a [bp+di] operand,
       so the stack segment), with BP as the caller left it. */
L13EF: /* _do_move_odata */
    /* 13EF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13F0:
    /* 13F0  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L13F1:
    /* 13F1  mov     bx,word ptr [bp+di] */
    BX = rw(pSS, BP + DI);
L13F3:
    /* 13F3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13F4:
    /* 13F4  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L13F5:
    /* 13F5  mov     word ptr [bx],ax */
    ww(pDS, BX, AX);
L13F7:
    /* 13F7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13F8:
    /* 13F8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L13F9:
    /* 13F9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_13FD  (+13FD)
       Opcode 82h, operands: count, first point, then count (x, z, y) coordinate triples. Each
       is offset by the object origin, shifted, rotated (load_xlate_rotate_pnt), clip-coded
       (code_pnt) and stored in consecutive points. */
L13FD: /* _do_multires */
    /* 13FD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L13FE:
    /* 13FE  mov     word ptr ds:[24DCh],ax */
    ww(pDS, 0x24DC, AX);
L1401:
    /* 1401  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1402:
    /* 1402  mov     di,ax */
    DI = AX;
L1404:
    /* 1404  add     di,14D0h */
    DI = (uint16_t)(DI + 0x14D0);
L1408:
    /* 1408  mov     word ptr ds:[24DEh],di */
    ww(pDS, 0x24DE, DI);
L140C: /* L140C */
    /* 140C  call    _load_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x351A), 0x140F)) != 0) return c;
L140F:
    /* 140F  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1412)) != 0) return c;
L1412:
    /* 1412  mov     di,word ptr ds:[24DEh] */
    DI = rw(pDS, 0x24DE);
L1416:
    /* 1416  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L1418:
    /* 1418  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L141B:
    /* 141B  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L141E:
    /* 141E  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L1421:
    /* 1421  add     word ptr ds:[24DEh],8 */
    ww(pDS, 0x24DE, add16(rw(pDS, 0x24DE), 0x8, 0));
L1426:
    /* 1426  dec     word ptr ds:[24DCh] */
    ww(pDS, 0x24DC, dec16(rw(pDS, 0x24DC)));
L142A:
    /* 142A  jne     L140C */
    if (!ZF) goto L140C;
L142C:
    /* 142C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L142D:
    /* 142D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L142E:
    /* 142E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1432  (+1432)
       Opcode B8h: do_multires with byte operands: a byte count, a byte point number, and byte
       coordinates (mini_xlate_rotate_pnt scales each by 32). */
L1432: /* _do_minimulti */
    /* 1432  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1433:
    /* 1433  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L1435:
    /* 1435  mov     word ptr ds:[24DCh],ax */
    ww(pDS, 0x24DC, AX);
L1438:
    /* 1438  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1439:
    /* 1439  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L143B:
    /* 143B  shl     ax,3 */
    AX = (uint16_t)(AX << 3);
L143E:
    /* 143E  mov     di,ax */
    DI = AX;
L1440:
    /* 1440  add     di,14D0h */
    DI = (uint16_t)(DI + 0x14D0);
L1444:
    /* 1444  mov     word ptr ds:[24DEh],di */
    ww(pDS, 0x24DE, DI);
L1448: /* L1448 */
    /* 1448  call    _mini_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34F2), 0x144B)) != 0) return c;
L144B:
    /* 144B  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x144E)) != 0) return c;
L144E:
    /* 144E  mov     di,word ptr ds:[24DEh] */
    DI = rw(pDS, 0x24DE);
L1452:
    /* 1452  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L1454:
    /* 1454  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L1457:
    /* 1457  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L145A:
    /* 145A  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L145D:
    /* 145D  add     word ptr ds:[24DEh],8 */
    ww(pDS, 0x24DE, add16(rw(pDS, 0x24DE), 0x8, 0));
L1462:
    /* 1462  dec     word ptr ds:[24DCh] */
    ww(pDS, 0x24DC, dec16(rw(pDS, 0x24DC)));
L1466:
    /* 1466  jne     L1448 */
    if (!ZF) goto L1448;
L1468:
    /* 1468  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1469:
    /* 1469  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L146A:
    /* 146A  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_146E  (+146E)
       Opcode CCh, operands: count, then count (point, shade byte) pairs, padded to a word. Sets
       each point's shade (+7) for Gouraud polygons. */
L146E: /* _do_setshade */
    /* 146E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L146F:
    /* 146F  mov     cx,ax */
    CX = AX;
L1471:
    /* 1471  mov     di,14D0h */
    DI = 0x14D0;
L1474: /* L1474 */
    /* 1474  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1475:
    /* 1475  mov     bx,ax */
    BX = AX;
L1477:
    /* 1477  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1478:
    /* 1478  mov     byte ptr [bx+di+7],al */
    wb(pDS, BX + DI + 0x7, AL);
L147B:
    /* 147B  loop    L1474 */
    if (--CX) goto L1474;
L147D:
    /* 147D  inc     si */
    SI = (uint16_t)(SI + 1);
L147E:
    /* 147E  and     si,-2 */
    SI = logic16((uint16_t)(SI & 0xFFFE));
L1481:
    /* 1481  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1482:
    /* 1482  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1483:
    /* 1483  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1487  (+1487)
       Opcode D4h (UW-specific), operands: count, the address of a model variable, then count
       (point, shade byte) pairs, padded to a word. Writes the variable's low byte to seg004:6D20h
       (PGCACHE's first byte, which seg003 reads) and sets each point's shade to its byte plus the
       shade bias at 2694h, at most 0Eh. */
L1487: /* _do_uwshade */
    /* 1487  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1488:
    /* 1488  mov     cx,ax */
    CX = AX;
L148A:
    /* 148A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L148B:
    /* 148B  mov     bx,ax */
    BX = AX;
L148D:
    /* 148D  mov     dx,word ptr [bx] */
    DX = rw(pDS, BX);
L148F:
    /* 148F  mov     byte ptr cs:_seg004_0849_6D20,dl */
    wb(CODE004, 0x6D20, DL);
L1494:
    /* 1494  mov     dx,word ptr ds:[2694h] */
    DX = rw(pDS, 0x2694);
L1498:
    /* 1498  mov     di,14D0h */
    DI = 0x14D0;
L149B: /* L149B */
    /* 149B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L149C:
    /* 149C  mov     bx,ax */
    BX = AX;
L149E:
    /* 149E  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L149F:
    /* 149F  add     al,dl */
    AL = (uint8_t)(AL + DL);
L14A1:
    /* 14A1  cmp     al,0Eh */
    sub8(AL, 0xE, 0);
L14A3:
    /* 14A3  jbe     short L14A7 */
    if (CF || ZF) goto L14A7;
L14A5:
    /* 14A5  mov     al,0Eh */
    AL = 0xE;
L14A7: /* L14A7 */
    /* 14A7  mov     byte ptr [bx+di+7],al */
    wb(pDS, BX + DI + 0x7, AL);
L14AA:
    /* 14AA  loop    L149B */
    if (--CX) goto L149B;
L14AC:
    /* 14AC  inc     si */
    SI = (uint16_t)(SI + 1);
L14AD:
    /* 14AD  and     si,-2 */
    SI = logic16((uint16_t)(SI & 0xFFFE));
L14B0:
    /* 14B0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L14B1:
    /* 14B1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L14B2:
    /* 14B2  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L14B6:
    /* 14B6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L14B7:
    /* 14B7  mov     bx,word ptr [si+6] */
    BX = rw(pDS, SI + 0x6);
L14BA:
    /* 14BA  mov     byte ptr [bx+14D7h],al */
    wb(pDS, BX + 0x14D7, AL);

    /* seg004_0849_14BE  (+14BE)
       Opcode 7Ah, operands: (x, z, y), point. Transforms one coordinate triple and stores it
       with its clip code. The unlabelled entry just before it (14B6h) is FM's do_defresg: it
       takes a shade word first and sets the point's shade from it, then falls in here; the DOS
       opcode table has no entry for it. */
L14BE: /* _do_defres */
    /* 14BE  call    _load_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x351A), 0x14C1)) != 0) return c;
L14C1:
    /* 14C1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L14C2:
    /* 14C2  mov     di,ax */
    DI = AX;
L14C4:
    /* 14C4  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L14C8:
    /* 14C8  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L14CC:
    /* 14CC  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L14D0:
    /* 14D0  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x14D3)) != 0) return c;
L14D3:
    /* 14D3  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L14D7:
    /* 14D7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L14D8:
    /* 14D8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L14D9:
    /* 14D9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_14DD  (+14DD)
       Opcode B6h: do_defres with byte coordinates and a byte point number (GRIDDB's SetPnt
       records). */
L14DD: /* _do_minires */
    /* 14DD  call    _mini_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34F2), 0x14E0)) != 0) return c;
L14E0:
    /* 14E0  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L14E1:
    /* 14E1  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L14E3:
    /* 14E3  shl     ax,3 */
    AX = (uint16_t)(AX << 3);
L14E6:
    /* 14E6  mov     di,ax */
    DI = AX;
L14E8:
    /* 14E8  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L14EC:
    /* 14EC  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L14F0:
    /* 14F0  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L14F4:
    /* 14F4  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x14F7)) != 0) return c;
L14F7:
    /* 14F7  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L14FB:
    /* 14FB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L14FC:
    /* 14FC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L14FD:
    /* 14FD  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1501  (+1501)
       Opcode 7Ch, operand: point. Starts a polygon: copies the point to the vertex buffer at
       1BAh and sets the codes OR and AND from it. */
L1501: /* _do_strres */
    /* 1501  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1502:
    /* 1502  mov     dx,si */
    DX = SI;
L1504:
    /* 1504  mov     si,ax */
    SI = AX;
L1506:
    /* 1506  add     si,14D0h */
    SI = add16(SI, 0x14D0, 0);
L150A:
    /* 150A  mov     di,1BAh */
    DI = 0x1BA;
L150D:
    /* 150D  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L150E:
    /* 150E  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L150F:
    /* 150F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1510:
    /* 1510  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1511:
    /* 1511  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1512:
    /* 1512  mov     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, AL);
L1515:
    /* 1515  mov     byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, AL);
L1518:
    /* 1518  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L151C:
    /* 151C  mov     si,dx */
    SI = DX;
L151E:
    /* 151E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L151F:
    /* 151F  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1520:
    /* 1520  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1524  (+1524)
       Opcode 8Eh, operand: point. Appends a point to the polygon do_strres started (falls into
       the dispatch; do_closure draws it). */
L1524: /* _do_cntres */
    /* 1524  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1525:
    /* 1525  mov     dx,si */
    DX = SI;
L1527:
    /* 1527  mov     si,ax */
    SI = AX;
L1529:
    /* 1529  add     si,14D0h */
    SI = (uint16_t)(SI + 0x14D0);
L152D:
    /* 152D  mov     di,word ptr ds:[1B6h] */
    DI = rw(pDS, 0x1B6);
L1531:
    /* 1531  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1532:
    /* 1532  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1533:
    /* 1533  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1534:
    /* 1534  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1535:
    /* 1535  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1536:
    /* 1536  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L153A:
    /* 153A  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, logic8((uint8_t)(rb(pDS, 0x14CA) | AL)));
L153E:
    /* 153E  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L1542:
    /* 1542  mov     si,dx */
    SI = DX;
L1544: /* L1544 */
    /* 1544  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1545:
    /* 1545  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1546:
    /* 1546  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_154A  (+154A)
       Opcode 9Ch: a rod, a quadrilateral of fixed width between two points, facing the eye. For
       each end: operand 0 then a point (that end is a single vertex), or a half-width then a
       point (two vertices, offset sideways by the width rotated through 2490h/2492h, which
       scale_matrix computes). Then draws it with do_closure. */
L154A: /* _do_rod */
    /* 154A  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L154F:
    /* 154F  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L1554:
    /* 1554  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1555:
    /* 1555  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L1557:
    /* 1557  jne     short L1574 */
    if (!ZF) goto L1574;
L1559:
    /* 1559  mov     di,1BAh */
    DI = 0x1BA;
L155C:
    /* 155C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L155D:
    /* 155D  push    si */
    push16(SI);
L155E:
    /* 155E  mov     si,ax */
    SI = AX;
L1560:
    /* 1560  add     si,14D0h */
    SI = (uint16_t)(SI + 0x14D0);
L1564:
    /* 1564  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1565:
    /* 1565  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1566:
    /* 1566  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1567:
    /* 1567  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1568:
    /* 1568  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L156C:
    /* 156C  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L1570:
    /* 1570  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1571:
    /* 1571  pop     si */
    SI = pop16();
L1572:
    /* 1572  jmp     short L15D1 */
    goto L15D1;
L1574: /* L1574 */
    /* 1574  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1578:
    /* 1578  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L157A:
    /* 157A  mov     cx,ax */
    CX = AX;
L157C:
    /* 157C  imul    word ptr ds:[2492h] */
    imul16(rw(pDS, 0x2492));
L1580:
    /* 1580  mov     ax,cx */
    AX = CX;
L1582:
    /* 1582  mov     cx,dx */
    CX = DX;
L1584:
    /* 1584  imul    word ptr ds:[2490h] */
    imul16(rw(pDS, 0x2490));
L1588:
    /* 1588  mov     di,1BAh */
    DI = 0x1BA;
L158B:
    /* 158B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L158C:
    /* 158C  push    si */
    push16(SI);
L158D:
    /* 158D  mov     si,ax */
    SI = AX;
L158F:
    /* 158F  add     si,14D0h */
    SI = (uint16_t)(SI + 0x14D0);
L1593:
    /* 1593  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1594:
    /* 1594  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L1596:
    /* 1596  mov     bx,ax */
    BX = AX;
L1598:
    /* 1598  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1599:
    /* 1599  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L159A:
    /* 159A  add     ax,cx */
    AX = (uint16_t)(AX + CX);
L159C:
    /* 159C  push    cx */
    push16(CX);
L159D:
    /* 159D  mov     cx,ax */
    CX = AX;
L159F:
    /* 159F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L15A0:
    /* 15A0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L15A1:
    /* 15A1  mov     bp,ax */
    BP = AX;
L15A3:
    /* 15A3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L15A4:
    /* 15A4  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x15A7)) != 0) return c;
L15A7:
    /* 15A7  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L15AB:
    /* 15AB  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L15AF:
    /* 15AF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L15B0:
    /* 15B0  pop     cx */
    CX = pop16();
L15B1:
    /* 15B1  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L15B4:
    /* 15B4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L15B5:
    /* 15B5  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L15B7:
    /* 15B7  mov     bx,ax */
    BX = AX;
L15B9:
    /* 15B9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L15BA:
    /* 15BA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L15BB:
    /* 15BB  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L15BD:
    /* 15BD  mov     cx,ax */
    CX = AX;
L15BF:
    /* 15BF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L15C0:
    /* 15C0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L15C1:
    /* 15C1  mov     bp,ax */
    BP = AX;
L15C3:
    /* 15C3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L15C4:
    /* 15C4  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x15C7)) != 0) return c;
L15C7:
    /* 15C7  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L15CB:
    /* 15CB  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L15CF:
    /* 15CF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L15D0:
    /* 15D0  pop     si */
    SI = pop16();
L15D1: /* L15D1 */
    /* 15D1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L15D2:
    /* 15D2  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L15D4:
    /* 15D4  jne     short L15EE */
    if (!ZF) goto L15EE;
L15D6:
    /* 15D6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L15D7:
    /* 15D7  push    si */
    push16(SI);
L15D8:
    /* 15D8  mov     si,ax */
    SI = AX;
L15DA:
    /* 15DA  add     si,14D0h */
    SI = (uint16_t)(SI + 0x14D0);
L15DE:
    /* 15DE  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L15DF:
    /* 15DF  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L15E0:
    /* 15E0  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L15E1:
    /* 15E1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L15E2:
    /* 15E2  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L15E6:
    /* 15E6  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L15EA:
    /* 15EA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L15EB:
    /* 15EB  pop     si */
    SI = pop16();
L15EC:
    /* 15EC  jmp     short L1648 */
    goto L1648;
L15EE: /* L15EE */
    /* 15EE  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L15F2:
    /* 15F2  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L15F4:
    /* 15F4  mov     cx,ax */
    CX = AX;
L15F6:
    /* 15F6  imul    word ptr ds:[2492h] */
    imul16(rw(pDS, 0x2492));
L15FA:
    /* 15FA  mov     ax,cx */
    AX = CX;
L15FC:
    /* 15FC  mov     cx,dx */
    CX = DX;
L15FE:
    /* 15FE  imul    word ptr ds:[2490h] */
    imul16(rw(pDS, 0x2490));
L1602:
    /* 1602  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1603:
    /* 1603  push    si */
    push16(SI);
L1604:
    /* 1604  mov     si,ax */
    SI = AX;
L1606:
    /* 1606  add     si,14D0h */
    SI = (uint16_t)(SI + 0x14D0);
L160A:
    /* 160A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L160B:
    /* 160B  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L160D:
    /* 160D  mov     bx,ax */
    BX = AX;
L160F:
    /* 160F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1610:
    /* 1610  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1611:
    /* 1611  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L1613:
    /* 1613  push    cx */
    push16(CX);
L1614:
    /* 1614  mov     cx,ax */
    CX = AX;
L1616:
    /* 1616  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1617:
    /* 1617  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1618:
    /* 1618  mov     bp,ax */
    BP = AX;
L161A:
    /* 161A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L161B:
    /* 161B  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x161E)) != 0) return c;
L161E:
    /* 161E  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L1622:
    /* 1622  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L1626:
    /* 1626  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1627:
    /* 1627  pop     cx */
    CX = pop16();
L1628:
    /* 1628  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L162B:
    /* 162B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L162C:
    /* 162C  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L162E:
    /* 162E  mov     bx,ax */
    BX = AX;
L1630:
    /* 1630  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1631:
    /* 1631  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1632:
    /* 1632  add     ax,cx */
    AX = (uint16_t)(AX + CX);
L1634:
    /* 1634  mov     cx,ax */
    CX = AX;
L1636:
    /* 1636  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1637:
    /* 1637  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1638:
    /* 1638  mov     bp,ax */
    BP = AX;
L163A:
    /* 163A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L163B:
    /* 163B  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x163E)) != 0) return c;
L163E:
    /* 163E  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L1642:
    /* 1642  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L1646:
    /* 1646  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1647:
    /* 1647  pop     si */
    SI = pop16();
L1648: /* L1648 */
    /* 1648  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L164C:
    /* 164C  jmp     _do_closure */
    goto L176C;

    /* seg004_0849_164F  (+164F)
       do_rod for a level view (check_flat installs it in the opcode table): the sideways
       offset is along x only, using 24ECh. */
L164F: /* _flat_rod */
    /* 164F  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L1654:
    /* 1654  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L1659:
    /* 1659  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L165A:
    /* 165A  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L165C:
    /* 165C  jne     short L1679 */
    if (!ZF) goto L1679;
L165E:
    /* 165E  mov     di,1BAh */
    DI = 0x1BA;
L1661:
    /* 1661  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1662:
    /* 1662  push    si */
    push16(SI);
L1663:
    /* 1663  mov     si,ax */
    SI = AX;
L1665:
    /* 1665  add     si,14D0h */
    SI = (uint16_t)(SI + 0x14D0);
L1669:
    /* 1669  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L166A:
    /* 166A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L166B:
    /* 166B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L166C:
    /* 166C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L166D:
    /* 166D  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L1671:
    /* 1671  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L1675:
    /* 1675  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1676:
    /* 1676  pop     si */
    SI = pop16();
L1677:
    /* 1677  jmp     short L16C6 */
    goto L16C6;
L1679: /* L1679 */
    /* 1679  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L167D:
    /* 167D  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L167F:
    /* 167F  imul    word ptr ds:[24ECh] */
    imul16(rw(pDS, 0x24EC));
L1683:
    /* 1683  mov     di,1BAh */
    DI = 0x1BA;
L1686:
    /* 1686  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1687:
    /* 1687  push    si */
    push16(SI);
L1688:
    /* 1688  mov     si,ax */
    SI = AX;
L168A:
    /* 168A  add     si,14D0h */
    SI = (uint16_t)(SI + 0x14D0);
L168E:
    /* 168E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L168F:
    /* 168F  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L1691:
    /* 1691  mov     bx,ax */
    BX = AX;
L1693:
    /* 1693  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1694:
    /* 1694  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1695:
    /* 1695  mov     cx,ax */
    CX = AX;
L1697:
    /* 1697  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1698:
    /* 1698  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1699:
    /* 1699  mov     bp,ax */
    BP = AX;
L169B:
    /* 169B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L169C:
    /* 169C  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x169F)) != 0) return c;
L169F:
    /* 169F  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L16A3:
    /* 16A3  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L16A7:
    /* 16A7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16A8:
    /* 16A8  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L16AB:
    /* 16AB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L16AC:
    /* 16AC  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L16AE:
    /* 16AE  mov     bx,ax */
    BX = AX;
L16B0:
    /* 16B0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16B1:
    /* 16B1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L16B2:
    /* 16B2  mov     cx,ax */
    CX = AX;
L16B4:
    /* 16B4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16B5:
    /* 16B5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L16B6:
    /* 16B6  mov     bp,ax */
    BP = AX;
L16B8:
    /* 16B8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16B9:
    /* 16B9  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x16BC)) != 0) return c;
L16BC:
    /* 16BC  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L16C0:
    /* 16C0  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L16C4:
    /* 16C4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16C5:
    /* 16C5  pop     si */
    SI = pop16();
L16C6: /* L16C6 */
    /* 16C6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L16C7:
    /* 16C7  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L16C9:
    /* 16C9  jne     short L16E3 */
    if (!ZF) goto L16E3;
L16CB:
    /* 16CB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L16CC:
    /* 16CC  push    si */
    push16(SI);
L16CD:
    /* 16CD  mov     si,ax */
    SI = AX;
L16CF:
    /* 16CF  add     si,14D0h */
    SI = (uint16_t)(SI + 0x14D0);
L16D3:
    /* 16D3  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L16D4:
    /* 16D4  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L16D5:
    /* 16D5  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L16D6:
    /* 16D6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L16D7:
    /* 16D7  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L16DB:
    /* 16DB  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L16DF:
    /* 16DF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16E0:
    /* 16E0  pop     si */
    SI = pop16();
L16E1:
    /* 16E1  jmp     short L172D */
    goto L172D;
L16E3: /* L16E3 */
    /* 16E3  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L16E7:
    /* 16E7  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L16E9:
    /* 16E9  imul    word ptr ds:[24ECh] */
    imul16(rw(pDS, 0x24EC));
L16ED:
    /* 16ED  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L16EE:
    /* 16EE  push    si */
    push16(SI);
L16EF:
    /* 16EF  mov     si,ax */
    SI = AX;
L16F1:
    /* 16F1  add     si,14D0h */
    SI = (uint16_t)(SI + 0x14D0);
L16F5:
    /* 16F5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L16F6:
    /* 16F6  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L16F8:
    /* 16F8  mov     bx,ax */
    BX = AX;
L16FA:
    /* 16FA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16FB:
    /* 16FB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L16FC:
    /* 16FC  mov     cx,ax */
    CX = AX;
L16FE:
    /* 16FE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L16FF:
    /* 16FF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1700:
    /* 1700  mov     bp,ax */
    BP = AX;
L1702:
    /* 1702  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1703:
    /* 1703  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1706)) != 0) return c;
L1706:
    /* 1706  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L170A:
    /* 170A  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L170E:
    /* 170E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L170F:
    /* 170F  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L1712:
    /* 1712  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1713:
    /* 1713  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L1715:
    /* 1715  mov     bx,ax */
    BX = AX;
L1717:
    /* 1717  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1718:
    /* 1718  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1719:
    /* 1719  mov     cx,ax */
    CX = AX;
L171B:
    /* 171B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L171C:
    /* 171C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L171D:
    /* 171D  mov     bp,ax */
    BP = AX;
L171F:
    /* 171F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1720:
    /* 1720  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1723)) != 0) return c;
L1723:
    /* 1723  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L1727:
    /* 1727  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L172B:
    /* 172B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L172C:
    /* 172C  pop     si */
    SI = pop16();
L172D: /* L172D */
    /* 172D  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L1731:
    /* 1731  jmp     short _do_closure */
    goto L176C;

    /* seg004_0849_1733  (+1733)
       Opcodes 22h and 7Eh, operands: count, then count points. Builds a polygon in the vertex
       buffer, with the codes OR in BH and AND in BL; if they AND to nonzero it is skipped,
       otherwise it falls into do_closure to be drawn. (System Shock: do_polyres.) */
L1733: /* _do_polyres */
    /* 1733  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1734:
    /* 1734  mov     cx,ax */
    CX = AX;
L1736:
    /* 1736  mov     di,1BAh */
    DI = 0x1BA;
L1739:
    /* 1739  xor     bx,bx */
    BX = (uint16_t)(BX ^ BX);
L173B:
    /* 173B  not     bl */
    BL = (uint8_t)~BL;
L173D:
    /* 173D  mov     bp,14D0h */
    BP = 0x14D0;
L1740: /* L1740 */
    /* 1740  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1741:
    /* 1741  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L1742:
    /* 1742  add     si,bp */
    SI = (uint16_t)(SI + BP);
L1744:
    /* 1744  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1745:
    /* 1745  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1746:
    /* 1746  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1747:
    /* 1747  mov     dl,byte ptr [si] */
    DL = rb(pDS, SI);
L1749:
    /* 1749  or      bh,dl */
    BH = (uint8_t)(BH | DL);
L174B:
    /* 174B  and     bl,dl */
    BL = logic8((uint8_t)(BL & DL));
L174D:
    /* 174D  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L174E:
    /* 174E  mov     si,ax */
    SI = AX;
L1750:
    /* 1750  loop    L1740 */
    if (--CX) goto L1740;
L1752:
    /* 1752  je      short L1760 */
    if (ZF) goto L1760;
L1754:
    /* 1754  mov     word ptr ds:[25E4h],0 */
    ww(pDS, 0x25E4, 0x0);
L175A:
    /* 175A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L175B:
    /* 175B  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L175C:
    /* 175C  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L1760: /* L1760 */
    /* 1760  mov     byte ptr ds:[14CAh],bh */
    wb(pDS, 0x14CA, BH);
L1764:
    /* 1764  mov     byte ptr ds:[14CBh],bl */
    wb(pDS, 0x14CB, BL);
L1768:
    /* 1768  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);

    /* seg004_0849_176C  (+176C)
       Opcode 80h: draws the polygon in the vertex buffer. Gouraud polygons (14A6h = the seg003
       Gouraud routine) go to SMOOTH.ASM's smooth_closure. Otherwise: AND of codes nonzero, skip;
       OR zero, project each vertex (x*[2472]/z + centre, y*[2470]/z + centre) to seg_370D:415Eh
       and call seg003's polygon routine (_seg003_0272_5311) with BP = the fill routine and CX =
       the vertex count; behind the eye (80h), clip in 3D first (must_clip_3d, FM); otherwise
       (clip_needed, FM) project with overflow checks and fall back to do_3d_clip if a screen
       coordinate overflows. The labels inside are FM names; the add immediates at modify_biasx,
       modify_biasy, modify_bx2 and modify_by2 are the screen centre, patched by render_3d. */
L176C: /* _do_closure */
    /* 176C  mov     di,1BAh */
    DI = 0x1BA;
L176F:
    /* 176F  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L1773:
    /* 1773  cmp     word ptr ds:[14A6h],offset _seg003_0272_5946 */
    sub16(rw(pDS, 0x14A6), 0x5946, 0);
L1779:
    /* 1779  jne     short draw_poly_buf_ptr */
    if (!ZF) goto L177E;
L177B:
    /* 177B  jmp     _smooth_closure */
    return ASM_JMP(0x065C, 0x0600);
L177E: /* draw_poly_buf_ptr */
    /* 177E  mov     word ptr ds:[25E4h],0 */
    ww(pDS, 0x25E4, 0x0);
L1784:
    /* 1784  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L1789:
    /* 1789  je      short L178E */
    if (ZF) goto L178E;
L178B:
    /* 178B  jmp     L1544 */
    goto L1544;
L178E: /* L178E */
    /* 178E  test    byte ptr ds:[14CAh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0xFF));
L1793:
    /* 1793  jne     short clip_needed */
    if (!ZF) goto L17ED;
L1795: /* L1795 */
    /* 1795  push    si */
    push16(SI);
L1796:
    /* 1796  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L1799:
    /* 1799  mov     es,ax */
    SET_ES(AX);
L179B:
    /* 179B  mov     di,415Eh */
    DI = 0x415E;
L179E:
    /* 179E  mov     bx,word ptr ds:[1B6h] */
    BX = rw(pDS, 0x1B6);
L17A2:
    /* 17A2  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L17A6:
    /* 17A6  mov     cx,word ptr ds:[2472h] */
    CX = rw(pDS, 0x2472);
L17AA: /* L17AA */
    /* 17AA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L17AB:
    /* 17AB  mov     bp,word ptr [si+2] */
    BP = rw(pDS, SI + 0x2);
L17AE:
    /* 17AE  imul    cx */
    imul16(CX);
L17B0:
    /* 17B0  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x17B0, 2)) != 0) return c;
L17B2: /* modify_biasx */
    /* 17B2  add     ax,4D2h */
    AX = (uint16_t)(AX + rw(CODE004, 0x17B3));
L17B5:
    /* 17B5  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L17B6:
    /* 17B6  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L17B8:
    /* 17B8  imul    word ptr ds:[2470h] */
    imul16(rw(pDS, 0x2470));
L17BC:
    /* 17BC  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x17BC, 2)) != 0) return c;
L17BE: /* modify_biasy */
    /* 17BE  add     ax,4D2h */
    AX = (uint16_t)(AX + rw(CODE004, 0x17BF));
L17C1:
    /* 17C1  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L17C2:
    /* 17C2  add     si,6 */
    SI = (uint16_t)(SI + 0x6);
L17C5:
    /* 17C5  cmp     si,bx */
    sub16(SI, BX, 0);
L17C7:
    /* 17C7  jne     L17AA */
    if (!ZF) goto L17AA;
L17C9:
    /* 17C9  sub     bx,word ptr ds:[1B8h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1B8));
L17CD:
    /* 17CD  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L17CF:
    /* 17CF  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L17D1:
    /* 17D1  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L17D3:
    /* 17D3  mov     cx,bx */
    CX = BX;
L17D5:
    /* 17D5  inc     word ptr ds:[25E4h] */
    ww(pDS, 0x25E4, (uint16_t)(rw(pDS, 0x25E4) + 1));
L17D9:
    /* 17D9  mov     bp,word ptr ds:[14A6h] */
    BP = rw(pDS, 0x14A6);
L17DD:
    /* 17DD  call    far ptr _seg003_0272_5311 */
    if ((c = asm_callf(ASM_JMP(0x0085, 0x5311), 0x065C + PORT_LOAD_SEG, 0x17E2)) != 0) return c;
L17E2:
    /* 17E2  mov     ax,ds */
    AX = asm_ds;
L17E4:
    /* 17E4  mov     es,ax */
    SET_ES(AX);
L17E6:
    /* 17E6  pop     si */
    SI = pop16();
L17E7:
    /* 17E7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L17E8:
    /* 17E8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L17E9:
    /* 17E9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L17ED: /* clip_needed */
    /* 17ED  js      short must_clip_3d */
    if (SF) goto L1858;
L17EF:
    /* 17EF  push    si */
    push16(SI);
L17F0: /* L17F0 */
    /* 17F0  mov     word ptr ss:[5A5h],offset _clip_overflow */
    ww(pSS, 0x5A5, 0x267D);
L17F7:
    /* 17F7  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L17FA:
    /* 17FA  mov     es,ax */
    SET_ES(AX);
L17FC:
    /* 17FC  mov     di,415Eh */
    DI = 0x415E;
L17FF:
    /* 17FF  mov     bx,word ptr ds:[1B6h] */
    BX = rw(pDS, 0x1B6);
L1803:
    /* 1803  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L1807:
    /* 1807  mov     cx,word ptr ds:[2472h] */
    CX = rw(pDS, 0x2472);
L180B: /* L180B */
    /* 180B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L180C:
    /* 180C  mov     bp,word ptr [si+2] */
    BP = rw(pDS, SI + 0x2);
L180F:
    /* 180F  imul    cx */
    imul16(CX);
L1811:
    /* 1811  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x1811, 2)) != 0) return c;
L1813: /* modify_bx2 */
    /* 1813  add     ax,4D2h */
    AX = add16(AX, rw(CODE004, 0x1814), 0);
L1816:
    /* 1816  jno     short L181B */
    if (!OF) goto L181B;
L1818:
    /* 1818  jmp     _do_3d_clip */
    goto L2681;
L181B: /* L181B */
    /* 181B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L181C:
    /* 181C  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L181E:
    /* 181E  imul    word ptr ds:[2470h] */
    imul16(rw(pDS, 0x2470));
L1822:
    /* 1822  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x1822, 2)) != 0) return c;
L1824: /* modify_by2 */
    /* 1824  add     ax,4D2h */
    AX = add16(AX, rw(CODE004, 0x1825), 0);
L1827:
    /* 1827  jno     short L182C */
    if (!OF) goto L182C;
L1829:
    /* 1829  jmp     _do_3d_clip */
    goto L2681;
L182C: /* L182C */
    /* 182C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L182D:
    /* 182D  add     si,6 */
    SI = (uint16_t)(SI + 0x6);
L1830:
    /* 1830  cmp     si,bx */
    sub16(SI, BX, 0);
L1832:
    /* 1832  jne     L180B */
    if (!ZF) goto L180B;
L1834:
    /* 1834  mov     cx,bx */
    CX = BX;
L1836:
    /* 1836  sub     cx,word ptr ds:[1B8h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1B8));
L183A:
    /* 183A  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L183C:
    /* 183C  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L183E:
    /* 183E  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L1840:
    /* 1840  inc     word ptr ds:[25E4h] */
    ww(pDS, 0x25E4, (uint16_t)(rw(pDS, 0x25E4) + 1));
L1844:
    /* 1844  mov     bp,word ptr ds:[14A6h] */
    BP = rw(pDS, 0x14A6);
L1848:
    /* 1848  call    far ptr _seg003_0272_5311 */
    if ((c = asm_callf(ASM_JMP(0x0085, 0x5311), 0x065C + PORT_LOAD_SEG, 0x184D)) != 0) return c;
L184D:
    /* 184D  mov     ax,ds */
    AX = asm_ds;
L184F:
    /* 184F  mov     es,ax */
    SET_ES(AX);
L1851:
    /* 1851  pop     si */
    SI = pop16();
L1852:
    /* 1852  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1853:
    /* 1853  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1854:
    /* 1854  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L1858: /* must_clip_3d */
    /* 1858  mov     word ptr ss:[5A5h],offset _overflow_handler_reg */
    ww(pSS, 0x5A5, 0x3C91);
L185F:
    /* 185F  test    byte ptr ds:[14CAh],4 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x4));
L1864:
    /* 1864  je      short L1881 */
    if (ZF) goto L1881;
L1866:
    /* 1866  push    si */
    push16(SI);
L1867:
    /* 1867  call    _clip_top */
    if ((c = asm_call(ASM_JMP(0x065C, 0x26E1), 0x186A)) != 0) return c;
L186A:
    /* 186A  pop     si */
    SI = pop16();
L186B:
    /* 186B  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L1870:
    /* 1870  jne     short L18E7 */
    if (!ZF) goto L18E7;
L1872:
    /* 1872  test    byte ptr ds:[14CAh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0xFF));
L1877:
    /* 1877  js      short L1881 */
    if (SF) goto L1881;
L1879:
    /* 1879  jne     short L187E */
    if (!ZF) goto L187E;
L187B:
    /* 187B  jmp     L1795 */
    goto L1795;
L187E: /* L187E */
    /* 187E  jmp     clip_needed */
    goto L17ED;
L1881: /* L1881 */
    /* 1881  test    byte ptr ds:[14CAh],8 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x8));
L1886:
    /* 1886  je      short L18A3 */
    if (ZF) goto L18A3;
L1888:
    /* 1888  push    si */
    push16(SI);
L1889:
    /* 1889  call    _clip_bot */
    if ((c = asm_call(ASM_JMP(0x065C, 0x27EA), 0x188C)) != 0) return c;
L188C:
    /* 188C  pop     si */
    SI = pop16();
L188D:
    /* 188D  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L1892:
    /* 1892  jne     short L18E7 */
    if (!ZF) goto L18E7;
L1894:
    /* 1894  test    byte ptr ds:[14CAh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0xFF));
L1899:
    /* 1899  js      short L18A3 */
    if (SF) goto L18A3;
L189B:
    /* 189B  jne     short L18A0 */
    if (!ZF) goto L18A0;
L189D:
    /* 189D  jmp     L1795 */
    goto L1795;
L18A0: /* L18A0 */
    /* 18A0  jmp     clip_needed */
    goto L17ED;
L18A3: /* L18A3 */
    /* 18A3  test    byte ptr ds:[14CAh],1 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x1));
L18A8:
    /* 18A8  je      short L18C5 */
    if (ZF) goto L18C5;
L18AA:
    /* 18AA  push    si */
    push16(SI);
L18AB:
    /* 18AB  call    _clip_left */
    if ((c = asm_call(ASM_JMP(0x065C, 0x2A08), 0x18AE)) != 0) return c;
L18AE:
    /* 18AE  pop     si */
    SI = pop16();
L18AF:
    /* 18AF  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L18B4:
    /* 18B4  jne     short L18E7 */
    if (!ZF) goto L18E7;
L18B6:
    /* 18B6  test    byte ptr ds:[14CAh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0xFF));
L18BB:
    /* 18BB  js      short L18C5 */
    if (SF) goto L18C5;
L18BD:
    /* 18BD  jne     short L18C2 */
    if (!ZF) goto L18C2;
L18BF:
    /* 18BF  jmp     L1795 */
    goto L1795;
L18C2: /* L18C2 */
    /* 18C2  jmp     clip_needed */
    goto L17ED;
L18C5: /* L18C5 */
    /* 18C5  test    byte ptr ds:[14CAh],2 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x2));
L18CA:
    /* 18CA  je      short L18E7 */
    if (ZF) goto L18E7;
L18CC:
    /* 18CC  push    si */
    push16(SI);
L18CD:
    /* 18CD  call    _clip_right */
    if ((c = asm_call(ASM_JMP(0x065C, 0x28FA), 0x18D0)) != 0) return c;
L18D0:
    /* 18D0  pop     si */
    SI = pop16();
L18D1:
    /* 18D1  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L18D6:
    /* 18D6  jne     short L18E7 */
    if (!ZF) goto L18E7;
L18D8:
    /* 18D8  test    byte ptr ds:[14CAh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0xFF));
L18DD:
    /* 18DD  js      short L18E7 */
    if (SF) goto L18E7;
L18DF:
    /* 18DF  jne     short L18E4 */
    if (!ZF) goto L18E4;
L18E1:
    /* 18E1  jmp     L1795 */
    goto L1795;
L18E4: /* L18E4 */
    /* 18E4  jmp     clip_needed */
    goto L17ED;
L18E7: /* L18E7 */
    /* 18E7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L18E8:
    /* 18E8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L18E9:
    /* 18E9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_18ED  (+18ED)
       Opcode 1Ah, operand: address. Calls a near routine in this segment and continues. */
L18ED: /* _do_asmcal */
    /* 18ED  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L18EE:
    /* 18EE  call    ax */
    if ((c = asm_call(ASM_JMP(0x065C, AX), 0x18F0)) != 0) return c;
L18F0:
    /* 18F0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L18F1:
    /* 18F1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L18F2:
    /* 18F2  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_18F6  (+18F6)
       Opcode 12h, operand: offset. Runs the sub-program at SI + offset (it ends with do_eof),
       then continues after the operand. (System Shock: do_sfcal.) */
L18F6: /* _do_sfcal */
    /* 18F6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L18F7:
    /* 18F7  push    si */
    push16(SI);
L18F8:
    /* 18F8  add     si,ax */
    SI = (uint16_t)(SI + AX);
L18FA:
    /* 18FA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L18FB:
    /* 18FB  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L18FC:
    /* 18FC  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x1900)) != 0) return c;
L1900:
    /* 1900  pop     si */
    SI = pop16();
L1901: /* L1901 */
    /* 1901  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1902:
    /* 1902  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1903:
    /* 1903  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1907  (+1907)
       Opcode 16h, operands: skip, address, value. If [address] >= value, skips `skip` bytes of
       program; otherwise continues (through do_sfcal's dispatch at L1901). */
L1907: /* _do_ifge */
    /* 1907  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1908:
    /* 1908  mov     cx,ax */
    CX = AX;
L190A:
    /* 190A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L190B:
    /* 190B  mov     bx,ax */
    BX = AX;
L190D:
    /* 190D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L190E:
    /* 190E  cmp     word ptr [bx],ax */
    sub16(rw(pDS, BX), AX, 0);
L1910:
    /* 1910  jl      L1901 */
    if (SF != OF) goto L1901;
L1912:
    /* 1912  add     si,cx */
    SI = add16(SI, CX, 0);
L1914:
    /* 1914  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1915:
    /* 1915  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1916:
    /* 1916  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_191A  (+191A)
       Opcode 14h: as do_ifge, skipping when [address] <= value. */
L191A: /* _do_ifle */
    /* 191A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L191B:
    /* 191B  mov     cx,ax */
    CX = AX;
L191D:
    /* 191D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L191E:
    /* 191E  mov     bx,ax */
    BX = AX;
L1920:
    /* 1920  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1921:
    /* 1921  cmp     word ptr [bx],ax */
    sub16(rw(pDS, BX), AX, 0);
L1923:
    /* 1923  jg      L1901 */
    if (!ZF && SF == OF) goto L1901;
L1925:
    /* 1925  add     si,cx */
    SI = add16(SI, CX, 0);
L1927:
    /* 1927  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1928:
    /* 1928  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1929:
    /* 1929  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_192D  (+192D)
       Opcode 6Ch, operands: address, value, offset a, offset b. Runs two sub-programs in an
       order chosen by a variable: a then b when [address] >= value, else b then a (the shared
       tail L1950 runs the second-listed first). The model's own back-to-front order switch. */
L192D: /* _do_sortvar */
    /* 192D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L192E:
    /* 192E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L192F:
    /* 192F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1930:
    /* 1930  cmp     word ptr [bx],ax */
    sub16(rw(pDS, BX), AX, 0);
L1932:
    /* 1932  jl      short L1950 */
    if (SF != OF) goto L1950;
L1934:
    /* 1934  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1935:
    /* 1935  push    si */
    push16(SI);
L1936:
    /* 1936  add     si,ax */
    SI = (uint16_t)(SI + AX);
L1938:
    /* 1938  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1939:
    /* 1939  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L193A:
    /* 193A  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x193E)) != 0) return c;
L193E:
    /* 193E  pop     si */
    SI = pop16();
L193F:
    /* 193F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1940:
    /* 1940  push    si */
    push16(SI);
L1941:
    /* 1941  add     si,ax */
    SI = (uint16_t)(SI + AX);
L1943:
    /* 1943  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1944:
    /* 1944  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1945:
    /* 1945  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x1949)) != 0) return c;
L1949:
    /* 1949  pop     si */
    SI = pop16();
L194A:
    /* 194A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L194B:
    /* 194B  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L194C:
    /* 194C  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L1950: /* L1950 */
    /* 1950  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1951:
    /* 1951  add     ax,si */
    AX = (uint16_t)(AX + SI);
L1953:
    /* 1953  mov     bx,ax */
    BX = AX;
L1955:
    /* 1955  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1956:
    /* 1956  push    si */
    push16(SI);
L1957:
    /* 1957  push    bx */
    push16(BX);
L1958:
    /* 1958  add     si,ax */
    SI = (uint16_t)(SI + AX);
L195A:
    /* 195A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L195B:
    /* 195B  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L195C:
    /* 195C  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x1960)) != 0) return c;
L1960:
    /* 1960  pop     si */
    SI = pop16();
L1961:
    /* 1961  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1962:
    /* 1962  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1963:
    /* 1963  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x1967)) != 0) return c;
L1967:
    /* 1967  pop     si */
    SI = pop16();
L1968:
    /* 1968  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1969:
    /* 1969  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L196A:
    /* 196A  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_196E  (+196E)
       Opcode 06h, operands: three (n, p) word pairs for x, y and z, then two sub-program offsets
       a and b. Computes the sign of the sum of n * (p + origin) over the axes (the origin at
       25E6h..25EAh is the negated eye-relative position, so this is the side of the plane the
       eye is on) and runs both sub-programs, a then b when it is non-negative, b then a
       otherwise: a BSP split, so that the half nearer the eye is drawn last (System Shock:
       do_sortnorm). */
L196E: /* _do_sortnorm */
    /* 196E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L196F:
    /* 196F  mov     cx,ax */
    CX = AX;
L1971:
    /* 1971  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1972:
    /* 1972  add     ax,word ptr ds:[25E6h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E6));
L1976:
    /* 1976  imul    cx */
    imul16(CX);
L1978:
    /* 1978  mov     cx,dx */
    CX = DX;
L197A:
    /* 197A  mov     bp,ax */
    BP = AX;
L197C:
    /* 197C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L197D:
    /* 197D  mov     di,ax */
    DI = AX;
L197F:
    /* 197F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1980:
    /* 1980  add     ax,word ptr ds:[25E8h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E8));
L1984:
    /* 1984  imul    di */
    imul16(DI);
L1986:
    /* 1986  add     bp,ax */
    BP = add16(BP, AX, 0);
L1988:
    /* 1988  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L198A:
    /* 198A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L198B:
    /* 198B  mov     di,ax */
    DI = AX;
L198D:
    /* 198D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L198E:
    /* 198E  add     ax,word ptr ds:[25EAh] */
    AX = (uint16_t)(AX + rw(pDS, 0x25EA));
L1992:
    /* 1992  imul    di */
    imul16(DI);
L1994:
    /* 1994  add     bp,ax */
    BP = add16(BP, AX, 0);
L1996:
    /* 1996  adc     cx,dx */
    CX = add16(CX, DX, CF);
L1998:
    /* 1998  js      L1950 */
    if (SF) goto L1950;
L199A:
    /* 199A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L199B:
    /* 199B  push    si */
    push16(SI);
L199C:
    /* 199C  add     si,ax */
    SI = (uint16_t)(SI + AX);
L199E:
    /* 199E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L199F:
    /* 199F  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L19A0:
    /* 19A0  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x19A4)) != 0) return c;
L19A4:
    /* 19A4  pop     si */
    SI = pop16();
L19A5:
    /* 19A5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19A6:
    /* 19A6  push    si */
    push16(SI);
L19A7:
    /* 19A7  add     si,ax */
    SI = (uint16_t)(SI + AX);
L19A9:
    /* 19A9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19AA:
    /* 19AA  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L19AB:
    /* 19AB  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x19AF)) != 0) return c;
L19AF:
    /* 19AF  pop     si */
    SI = pop16();
L19B0:
    /* 19B0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19B1:
    /* 19B1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L19B2:
    /* 19B2  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_19B6  (+19B6)
       Opcode 0Ch: do_sortnorm with no x term in the normal. */
L19B6: /* _do_sortnorm_x0 */
    /* 19B6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19B7:
    /* 19B7  mov     di,ax */
    DI = AX;
L19B9:
    /* 19B9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19BA:
    /* 19BA  add     ax,word ptr ds:[25E8h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E8));
L19BE:
    /* 19BE  imul    di */
    imul16(DI);
L19C0:
    /* 19C0  mov     bp,ax */
    BP = AX;
L19C2:
    /* 19C2  mov     cx,dx */
    CX = DX;
L19C4:
    /* 19C4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19C5:
    /* 19C5  mov     di,ax */
    DI = AX;
L19C7:
    /* 19C7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19C8:
    /* 19C8  add     ax,word ptr ds:[25EAh] */
    AX = (uint16_t)(AX + rw(pDS, 0x25EA));
L19CC:
    /* 19CC  imul    di */
    imul16(DI);
L19CE:
    /* 19CE  add     bp,ax */
    BP = add16(BP, AX, 0);
L19D0:
    /* 19D0  adc     cx,dx */
    CX = add16(CX, DX, CF);
L19D2:
    /* 19D2  jns     short L19D7 */
    if (!SF) goto L19D7;
L19D4:
    /* 19D4  jmp     L1950 */
    goto L1950;
L19D7: /* L19D7 */
    /* 19D7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19D8:
    /* 19D8  push    si */
    push16(SI);
L19D9:
    /* 19D9  add     si,ax */
    SI = (uint16_t)(SI + AX);
L19DB:
    /* 19DB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19DC:
    /* 19DC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L19DD:
    /* 19DD  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x19E1)) != 0) return c;
L19E1:
    /* 19E1  pop     si */
    SI = pop16();
L19E2:
    /* 19E2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19E3:
    /* 19E3  push    si */
    push16(SI);
L19E4:
    /* 19E4  add     si,ax */
    SI = (uint16_t)(SI + AX);
L19E6:
    /* 19E6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19E7:
    /* 19E7  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L19E8:
    /* 19E8  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x19EC)) != 0) return c;
L19EC:
    /* 19EC  pop     si */
    SI = pop16();
L19ED:
    /* 19ED  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19EE:
    /* 19EE  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L19EF:
    /* 19EF  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_19F3  (+19F3)
       Opcode 0Eh: do_sortnorm with no y term. */
L19F3: /* _do_sortnorm_y0 */
    /* 19F3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19F4:
    /* 19F4  mov     di,ax */
    DI = AX;
L19F6:
    /* 19F6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L19F7:
    /* 19F7  add     ax,word ptr ds:[25E6h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E6));
L19FB:
    /* 19FB  imul    di */
    imul16(DI);
L19FD:
    /* 19FD  mov     bp,ax */
    BP = AX;
L19FF:
    /* 19FF  mov     cx,dx */
    CX = DX;
L1A01:
    /* 1A01  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A02:
    /* 1A02  mov     di,ax */
    DI = AX;
L1A04:
    /* 1A04  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A05:
    /* 1A05  add     ax,word ptr ds:[25EAh] */
    AX = (uint16_t)(AX + rw(pDS, 0x25EA));
L1A09:
    /* 1A09  imul    di */
    imul16(DI);
L1A0B:
    /* 1A0B  add     bp,ax */
    BP = add16(BP, AX, 0);
L1A0D:
    /* 1A0D  adc     cx,dx */
    CX = add16(CX, DX, CF);
L1A0F:
    /* 1A0F  jns     short L1A14 */
    if (!SF) goto L1A14;
L1A11:
    /* 1A11  jmp     L1950 */
    goto L1950;
L1A14: /* L1A14 */
    /* 1A14  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A15:
    /* 1A15  push    si */
    push16(SI);
L1A16:
    /* 1A16  add     si,ax */
    SI = (uint16_t)(SI + AX);
L1A18:
    /* 1A18  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A19:
    /* 1A19  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1A1A:
    /* 1A1A  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x1A1E)) != 0) return c;
L1A1E:
    /* 1A1E  pop     si */
    SI = pop16();
L1A1F:
    /* 1A1F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A20:
    /* 1A20  push    si */
    push16(SI);
L1A21:
    /* 1A21  add     si,ax */
    SI = (uint16_t)(SI + AX);
L1A23:
    /* 1A23  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A24:
    /* 1A24  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1A25:
    /* 1A25  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x1A29)) != 0) return c;
L1A29:
    /* 1A29  pop     si */
    SI = pop16();
L1A2A:
    /* 1A2A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A2B:
    /* 1A2B  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1A2C:
    /* 1A2C  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1A30  (+1A30)
       Opcode 10h: do_sortnorm with no z term. */
L1A30: /* _do_sortnorm_z0 */
    /* 1A30  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A31:
    /* 1A31  mov     di,ax */
    DI = AX;
L1A33:
    /* 1A33  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A34:
    /* 1A34  add     ax,word ptr ds:[25E6h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E6));
L1A38:
    /* 1A38  imul    di */
    imul16(DI);
L1A3A:
    /* 1A3A  mov     bp,ax */
    BP = AX;
L1A3C:
    /* 1A3C  mov     cx,dx */
    CX = DX;
L1A3E:
    /* 1A3E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A3F:
    /* 1A3F  mov     di,ax */
    DI = AX;
L1A41:
    /* 1A41  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A42:
    /* 1A42  add     ax,word ptr ds:[25E8h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E8));
L1A46:
    /* 1A46  imul    di */
    imul16(DI);
L1A48:
    /* 1A48  add     bp,ax */
    BP = add16(BP, AX, 0);
L1A4A:
    /* 1A4A  adc     cx,dx */
    CX = add16(CX, DX, CF);
L1A4C:
    /* 1A4C  jns     short L1A51 */
    if (!SF) goto L1A51;
L1A4E:
    /* 1A4E  jmp     L1950 */
    goto L1950;
L1A51: /* L1A51 */
    /* 1A51  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A52:
    /* 1A52  push    si */
    push16(SI);
L1A53:
    /* 1A53  add     si,ax */
    SI = (uint16_t)(SI + AX);
L1A55:
    /* 1A55  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A56:
    /* 1A56  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1A57:
    /* 1A57  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x1A5B)) != 0) return c;
L1A5B:
    /* 1A5B  pop     si */
    SI = pop16();
L1A5C:
    /* 1A5C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A5D:
    /* 1A5D  push    si */
    push16(SI);
L1A5E:
    /* 1A5E  add     si,ax */
    SI = (uint16_t)(SI + AX);
L1A60:
    /* 1A60  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A61:
    /* 1A61  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1A62:
    /* 1A62  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x1A66)) != 0) return c;
L1A66:
    /* 1A66  pop     si */
    SI = pop16();
L1A67:
    /* 1A67  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A68:
    /* 1A68  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1A69:
    /* 1A69  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1A6D  (+1A6D)
       Opcode 18h, operands: three 32-bit world coordinates. Sets the object origin (25E6h..
       25EAh) to the negated position relative to the eye. If a relative coordinate does not fit
       16 bits, sets 25E2h = -1 and stops, with the axes before it already updated. Calls
       self_modify. */
L1A6D: /* _do_org */
    /* 1A6D  mov     word ptr ds:[25E2h],0 */
    ww(pDS, 0x25E2, 0x0);
L1A73:
    /* 1A73  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A74:
    /* 1A74  sub     ax,word ptr ds:[247Ah] */
    AX = sub16(AX, rw(pDS, 0x247A), 0);
L1A78:
    /* 1A78  mov     bx,word ptr [si] */
    BX = rw(pDS, SI);
L1A7A:
    /* 1A7A  inc     si */
    SI = inc16(SI);
L1A7B:
    /* 1A7B  inc     si */
    SI = inc16(SI);
L1A7C:
    /* 1A7C  sbb     bx,word ptr ds:[247Ch] */
    BX = (uint16_t)(BX - rw(pDS, 0x247C) - CF);
L1A80:
    /* 1A80  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1A81:
    /* 1A81  cmp     bx,dx */
    sub16(BX, DX, 0);
L1A83:
    /* 1A83  jne     short L1AC1 */
    if (!ZF) goto L1AC1;
L1A85:
    /* 1A85  neg     ax */
    AX = (uint16_t)-AX;
L1A87:
    /* 1A87  mov     word ptr ds:[25E6h],ax */
    ww(pDS, 0x25E6, AX);
L1A8A:
    /* 1A8A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1A8B:
    /* 1A8B  sub     ax,word ptr ds:[247Eh] */
    AX = sub16(AX, rw(pDS, 0x247E), 0);
L1A8F:
    /* 1A8F  mov     bx,word ptr [si] */
    BX = rw(pDS, SI);
L1A91:
    /* 1A91  inc     si */
    SI = inc16(SI);
L1A92:
    /* 1A92  inc     si */
    SI = inc16(SI);
L1A93:
    /* 1A93  sbb     bx,word ptr ds:[2480h] */
    BX = (uint16_t)(BX - rw(pDS, 0x2480) - CF);
L1A97:
    /* 1A97  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1A98:
    /* 1A98  cmp     bx,dx */
    sub16(BX, DX, 0);
L1A9A:
    /* 1A9A  jne     short L1AC1 */
    if (!ZF) goto L1AC1;
L1A9C:
    /* 1A9C  neg     ax */
    AX = (uint16_t)-AX;
L1A9E:
    /* 1A9E  mov     word ptr ds:[25E8h],ax */
    ww(pDS, 0x25E8, AX);
L1AA1:
    /* 1AA1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1AA2:
    /* 1AA2  sub     ax,word ptr ds:[2482h] */
    AX = sub16(AX, rw(pDS, 0x2482), 0);
L1AA6:
    /* 1AA6  mov     bx,word ptr [si] */
    BX = rw(pDS, SI);
L1AA8:
    /* 1AA8  inc     si */
    SI = inc16(SI);
L1AA9:
    /* 1AA9  inc     si */
    SI = inc16(SI);
L1AAA:
    /* 1AAA  sbb     bx,word ptr ds:[2484h] */
    BX = (uint16_t)(BX - rw(pDS, 0x2484) - CF);
L1AAE:
    /* 1AAE  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L1AAF:
    /* 1AAF  cmp     bx,dx */
    sub16(BX, DX, 0);
L1AB1:
    /* 1AB1  jne     short L1AC1 */
    if (!ZF) goto L1AC1;
L1AB3:
    /* 1AB3  neg     ax */
    AX = (uint16_t)-AX;
L1AB5:
    /* 1AB5  mov     word ptr ds:[25EAh],ax */
    ww(pDS, 0x25EA, AX);
L1AB8:
    /* 1AB8  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x1ABB)) != 0) return c;
L1ABB:
    /* 1ABB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1ABC:
    /* 1ABC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1ABD:
    /* 1ABD  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L1AC1: /* L1AC1 */
    /* 1AC1  mov     word ptr ds:[25E2h],0FFFFh */
    ww(pDS, 0x25E2, 0xFFFF);
L1AC7:
    /* 1AC7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1AC8:
    /* 1AC8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1AC9:
    /* 1AC9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1ACD  (+1ACD)
       Opcode 84h, operands: count, then count points. A visibility test: if all the points share
       a clip-code bit (entirely outside one plane) the program ends here (jumps to do_eof's ret);
       if none has a code, calls set_accept (a no-op in UW2); otherwise continues. */
L1ACD: /* _do_hull */
    /* 1ACD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1ACE:
    /* 1ACE  mov     cx,ax */
    CX = AX;
L1AD0:
    /* 1AD0  dec     cx */
    CX = (uint16_t)(CX - 1);
L1AD1:
    /* 1AD1  mov     bx,14D6h */
    BX = 0x14D6;
L1AD4:
    /* 1AD4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1AD5:
    /* 1AD5  mov     di,ax */
    DI = AX;
L1AD7:
    /* 1AD7  mov     dl,byte ptr [bx+di] */
    DL = rb(pDS, BX + DI);
L1AD9:
    /* 1AD9  mov     bp,dx */
    BP = DX;
L1ADB: /* L1ADB */
    /* 1ADB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1ADC:
    /* 1ADC  mov     di,ax */
    DI = AX;
L1ADE:
    /* 1ADE  or      dl,byte ptr [bx+di] */
    DL = (uint8_t)(DL | rb(pDS, BX + DI));
L1AE0:
    /* 1AE0  and     bp,word ptr [bx+di] */
    BP = logic16((uint16_t)(BP & rw(pDS, BX + DI)));
L1AE2:
    /* 1AE2  loop    L1ADB */
    if (--CX) goto L1ADB;
L1AE4:
    /* 1AE4  je      short L1AE9 */
    if (ZF) goto L1AE9;
L1AE6:
    /* 1AE6  jmp     _do_eof */
    goto L1098;
L1AE9: /* L1AE9 */
    /* 1AE9  or      dl,dl */
    DL = logic8((uint8_t)(DL | DL));
L1AEB:
    /* 1AEB  jne     short L1AF2 */
    if (!ZF) goto L1AF2;
L1AED:
    /* 1AED  mov     al,0FFh */
    AL = 0xFF;
L1AEF:
    /* 1AEF  call    _set_accept */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C4), 0x1AF2)) != 0) return c;
L1AF2: /* L1AF2 */
    /* 1AF2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1AF3:
    /* 1AF3  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1AF4:
    /* 1AF4  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1AF8  (+1AF8)
       flat_ variant of do_y_rel (opcode 88h when the view is level): the y offset adds straight to
       view y, halved; recomputes the clip code bits 4 and 8 only. */
L1AF8: /* _flat_y_rel */
    /* 1AF8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1AF9:
    /* 1AF9  mov     bp,ax */
    BP = AX;
L1AFB:
    /* 1AFB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1AFC:
    /* 1AFC  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1B00:
    /* 1B00  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1B02:
    /* 1B02  sar     ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
L1B04:
    /* 1B04  mov     cx,ax */
    CX = AX;
L1B06:
    /* 1B06  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1B07:
    /* 1B07  xchg    bp,si */
    { uint16_t t_ = SI;
    SI = BP;
    BP = t_; }
L1B09:
    /* 1B09  mov     di,14D0h */
    DI = 0x14D0;
L1B0C:
    /* 1B0C  add     si,di */
    SI = (uint16_t)(SI + DI);
L1B0E:
    /* 1B0E  add     di,ax */
    DI = (uint16_t)(DI + AX);
L1B10:
    /* 1B10  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1B11:
    /* 1B11  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1B12:
    /* 1B12  add     ax,cx */
    AX = (uint16_t)(AX + CX);
L1B14:
    /* 1B14  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1B15:
    /* 1B15  mov     cx,ax */
    CX = AX;
L1B17:
    /* 1B17  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1B18:
    /* 1B18  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1B19:
    /* 1B19  mov     dx,ax */
    DX = AX;
L1B1B:
    /* 1B1B  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1B1C:
    /* 1B1C  and     al,0F3h */
    AL = (uint8_t)(AL & 0xF3);
L1B1E:
    /* 1B1E  cmp     cx,dx */
    sub16(CX, DX, 0);
L1B20:
    /* 1B20  jle     short L1B3C */
    if (ZF || SF != OF) goto L1B3C;
L1B22:
    /* 1B22  add     cx,dx */
    CX = add16(CX, DX, 0);
L1B24:
    /* 1B24  jge     short L1B31 */
    if (SF == OF) goto L1B31;
L1B26:
    /* 1B26  or      al,0Ch */
    AL = logic8((uint8_t)(AL | 0xC));
L1B28:
    /* 1B28  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1B29:
    /* 1B29  mov     si,bp */
    SI = BP;
L1B2B:
    /* 1B2B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1B2C:
    /* 1B2C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1B2D:
    /* 1B2D  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L1B31: /* L1B31 */
    /* 1B31  or      al,4 */
    AL = logic8((uint8_t)(AL | 0x4));
L1B33:
    /* 1B33  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1B34:
    /* 1B34  mov     si,bp */
    SI = BP;
L1B36:
    /* 1B36  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1B37:
    /* 1B37  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1B38:
    /* 1B38  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L1B3C: /* L1B3C */
    /* 1B3C  add     cx,dx */
    CX = add16(CX, DX, 0);
L1B3E:
    /* 1B3E  jge     short L1B42 */
    if (SF == OF) goto L1B42;
L1B40:
    /* 1B40  or      al,8 */
    AL = logic8((uint8_t)(AL | 0x8));
L1B42: /* L1B42 */
    /* 1B42  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1B43:
    /* 1B43  mov     si,bp */
    SI = BP;
L1B45:
    /* 1B45  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1B46:
    /* 1B46  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1B47:
    /* 1B47  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1B4B  (+1B4B)
       Opcode 88h (also 1Ch), operands: source point, dy, destination point. New point = source +
       dy along the model's y axis (rotated through the matrix's y row), clip-coded. (System Shock:
       do_y_rel.) The three unlabelled entries after it are FM's do_animate_p, do_animate_b and
       do_animate_h (same order, seven bytes apart, there as here; the DOS opcode table has no
       entry for them): each loads BP with instance_pitch, instance_bank or instance_head, moves
       an animated angle variable ([di+2]) towards its target ([di]) by its rate ([di+4]) times
       the frame time at ss:[0A80h], offsets the origin, and runs the sub-program [di+6] rotated by
       that angle, restoring the matrix and origin afterwards. */
L1B4B: /* _do_y_rel */
    /* 1B4B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1B4C:
    /* 1B4C  mov     di,ax */
    DI = AX;
L1B4E:
    /* 1B4E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1B4F:
    /* 1B4F  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1B53:
    /* 1B53  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1B55:
    /* 1B55  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L1B59:
    /* 1B59  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L1B5D:
    /* 1B5D  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L1B61:
    /* 1B61  mov     di,ax */
    DI = AX;
L1B63:
    /* 1B63  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L1B67:
    /* 1B67  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L1B69:
    /* 1B69  mov     ax,word ptr ds:[14BAh] */
    AX = rw(pDS, 0x14BA);
L1B6C:
    /* 1B6C  imul    di */
    imul16(DI);
L1B6E:
    /* 1B6E  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L1B70:
    /* 1B70  mov     ax,word ptr ds:[14BCh] */
    AX = rw(pDS, 0x14BC);
L1B73:
    /* 1B73  imul    di */
    imul16(DI);
L1B75:
    /* 1B75  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L1B77:
    /* 1B77  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1B78:
    /* 1B78  mov     di,ax */
    DI = AX;
L1B7A:
    /* 1B7A  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L1B7E:
    /* 1B7E  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L1B82:
    /* 1B82  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L1B86:
    /* 1B86  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1B89)) != 0) return c;
L1B89:
    /* 1B89  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L1B8D:
    /* 1B8D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1B8E:
    /* 1B8E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1B8F:
    /* 1B8F  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L1B93:
    /* 1B93  mov     bp,offset _instance_pitch */
    BP = 0x37F5;
L1B96:
    /* 1B96  jmp     short L1BA0 */
    goto L1BA0;
L1B98:
    /* 1B98  mov     bp,offset _instance_bank */
    BP = 0x395D;
L1B9B:
    /* 1B9B  jmp     short L1BA0 */
    goto L1BA0;
L1B9D:
    /* 1B9D  mov     bp,offset _instance_head */
    BP = 0x368D;
L1BA0: /* L1BA0 */
    /* 1BA0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BA1:
    /* 1BA1  mov     di,ax */
    DI = AX;
L1BA3:
    /* 1BA3  mov     bx,word ptr [di+2] */
    BX = rw(pDS, DI + 0x2);
L1BA6:
    /* 1BA6  cmp     bx,word ptr [di] */
    sub16(BX, rw(pDS, DI), 0);
L1BA8:
    /* 1BA8  je      short L1BDC */
    if (ZF) goto L1BDC;
L1BAA:
    /* 1BAA  jg      short L1BC5 */
    if (!ZF && SF == OF) goto L1BC5;
L1BAC:
    /* 1BAC  mov     ax,word ptr [di+4] */
    AX = rw(pDS, DI + 0x4);
L1BAF:
    /* 1BAF  imul    word ptr ss:[0A80h] */
    imul16(rw(pSS, 0xA80));
L1BB4:
    /* 1BB4  shl     ax,1 */
    AX = shl16(AX, 1);
L1BB6:
    /* 1BB6  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1BB8:
    /* 1BB8  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L1BBA:
    /* 1BBA  cmp     bx,word ptr [di] */
    sub16(BX, rw(pDS, DI), 0);
L1BBC:
    /* 1BBC  jle     short L1BC0 */
    if (ZF || SF != OF) goto L1BC0;
L1BBE:
    /* 1BBE  mov     bx,word ptr [di] */
    BX = rw(pDS, DI);
L1BC0: /* L1BC0 */
    /* 1BC0  mov     word ptr [di+2],bx */
    ww(pDS, DI + 0x2, BX);
L1BC3:
    /* 1BC3  jmp     short L1BDC */
    goto L1BDC;
L1BC5: /* L1BC5 */
    /* 1BC5  mov     ax,word ptr [di+4] */
    AX = rw(pDS, DI + 0x4);
L1BC8:
    /* 1BC8  imul    word ptr ss:[0A80h] */
    imul16(rw(pSS, 0xA80));
L1BCD:
    /* 1BCD  shl     ax,1 */
    AX = shl16(AX, 1);
L1BCF:
    /* 1BCF  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1BD1:
    /* 1BD1  sub     bx,dx */
    BX = (uint16_t)(BX - DX);
L1BD3:
    /* 1BD3  cmp     bx,word ptr [di] */
    sub16(BX, rw(pDS, DI), 0);
L1BD5:
    /* 1BD5  jge     short L1BD9 */
    if (SF == OF) goto L1BD9;
L1BD7:
    /* 1BD7  mov     bx,word ptr [di] */
    BX = rw(pDS, DI);
L1BD9: /* L1BD9 */
    /* 1BD9  mov     word ptr [di+2],bx */
    ww(pDS, DI + 0x2, BX);
L1BDC: /* L1BDC */
    /* 1BDC  push    word ptr ds:[25E6h] */
    push16(rw(pDS, 0x25E6));
L1BE0:
    /* 1BE0  push    word ptr ds:[25E8h] */
    push16(rw(pDS, 0x25E8));
L1BE4:
    /* 1BE4  push    word ptr ds:[25EAh] */
    push16(rw(pDS, 0x25EA));
L1BE8:
    /* 1BE8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BE9:
    /* 1BE9  sub     word ptr ds:[25E6h],ax */
    ww(pDS, 0x25E6, (uint16_t)(rw(pDS, 0x25E6) - AX));
L1BED:
    /* 1BED  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BEE:
    /* 1BEE  sub     word ptr ds:[25E8h],ax */
    ww(pDS, 0x25E8, (uint16_t)(rw(pDS, 0x25E8) - AX));
L1BF2:
    /* 1BF2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1BF3:
    /* 1BF3  sub     word ptr ds:[25EAh],ax */
    ww(pDS, 0x25EA, (uint16_t)(rw(pDS, 0x25EA) - AX));
L1BF7:
    /* 1BF7  push    si */
    push16(SI);
L1BF8:
    /* 1BF8  mov     si,word ptr [di+6] */
    SI = rw(pDS, DI + 0x6);
L1BFB:
    /* 1BFB  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L1BFD:
    /* 1BFD  je      short L1C6D */
    if (ZF) goto L1C6D;
L1BFF:
    /* 1BFF  push    word ptr ds:[14B2h] */
    push16(rw(pDS, 0x14B2));
L1C03:
    /* 1C03  push    word ptr ds:[14B8h] */
    push16(rw(pDS, 0x14B8));
L1C07:
    /* 1C07  push    word ptr ds:[14BEh] */
    push16(rw(pDS, 0x14BE));
L1C0B:
    /* 1C0B  push    word ptr ds:[14B4h] */
    push16(rw(pDS, 0x14B4));
L1C0F:
    /* 1C0F  push    word ptr ds:[14BAh] */
    push16(rw(pDS, 0x14BA));
L1C13:
    /* 1C13  push    word ptr ds:[14C0h] */
    push16(rw(pDS, 0x14C0));
L1C17:
    /* 1C17  push    word ptr ds:[14B6h] */
    push16(rw(pDS, 0x14B6));
L1C1B:
    /* 1C1B  push    word ptr ds:[14BCh] */
    push16(rw(pDS, 0x14BC));
L1C1F:
    /* 1C1F  push    word ptr ds:[14C2h] */
    push16(rw(pDS, 0x14C2));
L1C23:
    /* 1C23  call    bp */
    if ((c = asm_call(ASM_JMP(0x065C, BP), 0x1C25)) != 0) return c;
L1C25:
    /* 1C25  push    si */
    push16(SI);
L1C26:
    /* 1C26  call    _check_flat */
    if ((c = asm_call(ASM_JMP(0x065C, 0x340E), 0x1C29)) != 0) return c;
L1C29:
    /* 1C29  pop     si */
    SI = pop16();
L1C2A:
    /* 1C2A  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x1C2D)) != 0) return c;
L1C2D:
    /* 1C2D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C2E:
    /* 1C2E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1C2F:
    /* 1C2F  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x1C33)) != 0) return c;
L1C33:
    /* 1C33  pop     word ptr ds:[14C2h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14C2, t_); }
L1C37:
    /* 1C37  pop     word ptr ds:[14BCh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BC, t_); }
L1C3B:
    /* 1C3B  pop     word ptr ds:[14B6h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B6, t_); }
L1C3F:
    /* 1C3F  pop     word ptr ds:[14C0h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14C0, t_); }
L1C43:
    /* 1C43  pop     word ptr ds:[14BAh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BA, t_); }
L1C47:
    /* 1C47  pop     word ptr ds:[14B4h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B4, t_); }
L1C4B:
    /* 1C4B  pop     word ptr ds:[14BEh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BE, t_); }
L1C4F:
    /* 1C4F  pop     word ptr ds:[14B8h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B8, t_); }
L1C53:
    /* 1C53  pop     word ptr ds:[14B2h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B2, t_); }
L1C57:
    /* 1C57  pop     si */
    SI = pop16();
L1C58:
    /* 1C58  pop     word ptr ds:[25EAh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25EA, t_); }
L1C5C:
    /* 1C5C  pop     word ptr ds:[25E8h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E8, t_); }
L1C60:
    /* 1C60  pop     word ptr ds:[25E6h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E6, t_); }
L1C64:
    /* 1C64  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x1C67)) != 0) return c;
L1C67:
    /* 1C67  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C68:
    /* 1C68  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1C69:
    /* 1C69  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L1C6D: /* L1C6D */
    /* 1C6D  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x1C70)) != 0) return c;
L1C70:
    /* 1C70  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C71:
    /* 1C71  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1C72:
    /* 1C72  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x1C76)) != 0) return c;
L1C76:
    /* 1C76  pop     si */
    SI = pop16();
L1C77:
    /* 1C77  pop     word ptr ds:[25EAh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25EA, t_); }
L1C7B:
    /* 1C7B  pop     word ptr ds:[25E8h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E8, t_); }
L1C7F:
    /* 1C7F  pop     word ptr ds:[25E6h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E6, t_); }
L1C83:
    /* 1C83  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x1C86)) != 0) return c;
L1C86:
    /* 1C86  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C87:
    /* 1C87  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1C88:
    /* 1C88  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1C8C  (+1C8C)
       Opcode 26h, operands: destination point, source point. Copies the point (7 bytes: x, y, z,
       codes). */
L1C8C: /* _do_movres */
    /* 1C8C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C8D:
    /* 1C8D  mov     di,ax */
    DI = AX;
L1C8F:
    /* 1C8F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1C90:
    /* 1C90  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L1C91:
    /* 1C91  add     di,14D0h */
    DI = (uint16_t)(DI + 0x14D0);
L1C95:
    /* 1C95  add     si,14D0h */
    SI = add16(SI, 0x14D0, 0);
L1C99:
    /* 1C99  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1C9A:
    /* 1C9A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1C9B:
    /* 1C9B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1C9C:
    /* 1C9C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L1C9D:
    /* 1C9D  mov     si,ax */
    SI = AX;
L1C9F:
    /* 1C9F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1CA0:
    /* 1CA0  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1CA1:
    /* 1CA1  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1CA5  (+1CA5)
       flat_ variant of do_x_rel: with the view level only the matrix's x and z terms are used and
       the clip code is recomputed inline. */
L1CA5: /* _flat_x_rel */
    /* 1CA5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1CA6:
    /* 1CA6  mov     di,ax */
    DI = AX;
L1CA8:
    /* 1CA8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1CA9:
    /* 1CA9  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1CAD:
    /* 1CAD  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1CAF:
    /* 1CAF  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L1CB3:
    /* 1CB3  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L1CB7:
    /* 1CB7  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L1CBB:
    /* 1CBB  mov     di,ax */
    DI = AX;
L1CBD:
    /* 1CBD  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L1CC1:
    /* 1CC1  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L1CC3:
    /* 1CC3  mov     ax,di */
    AX = DI;
L1CC5:
    /* 1CC5  imul    word ptr ds:[14B6h] */
    imul16(rw(pDS, 0x14B6));
L1CC9:
    /* 1CC9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1CCA:
    /* 1CCA  mov     di,ax */
    DI = AX;
L1CCC:
    /* 1CCC  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L1CD0:
    /* 1CD0  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L1CD4:
    /* 1CD4  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L1CD6:
    /* 1CD6  add     bp,dx */
    BP = add16(BP, DX, 0);
L1CD8:
    /* 1CD8  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L1CDC:
    /* 1CDC  jg      short L1CE0 */
    if (!ZF && SF == OF) goto L1CE0;
L1CDE:
    /* 1CDE  mov     al,80h */
    AL = 0x80;
L1CE0: /* L1CE0 */
    /* 1CE0  cmp     bx,bp */
    sub16(BX, BP, 0);
L1CE2:
    /* 1CE2  jle     short L1CE6 */
    if (ZF || SF != OF) goto L1CE6;
L1CE4:
    /* 1CE4  inc     ax */
    AX = (uint16_t)(AX + 1);
L1CE5:
    /* 1CE5  inc     ax */
    AX = (uint16_t)(AX + 1);
L1CE6: /* L1CE6 */
    /* 1CE6  cmp     cx,bp */
    sub16(CX, BP, 0);
L1CE8:
    /* 1CE8  jle     short L1CEC */
    if (ZF || SF != OF) goto L1CEC;
L1CEA:
    /* 1CEA  add     al,4 */
    AL = (uint8_t)(AL + 0x4);
L1CEC: /* L1CEC */
    /* 1CEC  add     bx,bp */
    BX = add16(BX, BP, 0);
L1CEE:
    /* 1CEE  jge     short L1CF1 */
    if (SF == OF) goto L1CF1;
L1CF0:
    /* 1CF0  inc     ax */
    AX = (uint16_t)(AX + 1);
L1CF1: /* L1CF1 */
    /* 1CF1  add     cx,bp */
    CX = add16(CX, BP, 0);
L1CF3:
    /* 1CF3  jge     short L1CF7 */
    if (SF == OF) goto L1CF7;
L1CF5:
    /* 1CF5  add     al,8 */
    AL = add8(AL, 0x8, 0);
L1CF7: /* L1CF7 */
    /* 1CF7  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L1CFB:
    /* 1CFB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1CFC:
    /* 1CFC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1CFD:
    /* 1CFD  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1D01  (+1D01)
       Opcodes 86h and 2Ah, operands: source point, dx, destination point. New point = source +
       dx along the model's x axis, clip-coded. (System Shock: do_x_rel.) */
L1D01: /* _do_x_rel */
    /* 1D01  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D02:
    /* 1D02  mov     di,ax */
    DI = AX;
L1D04:
    /* 1D04  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D05:
    /* 1D05  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1D09:
    /* 1D09  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1D0B:
    /* 1D0B  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L1D0F:
    /* 1D0F  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L1D13:
    /* 1D13  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L1D17:
    /* 1D17  mov     di,ax */
    DI = AX;
L1D19:
    /* 1D19  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L1D1D:
    /* 1D1D  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L1D1F:
    /* 1D1F  mov     ax,word ptr ds:[14B4h] */
    AX = rw(pDS, 0x14B4);
L1D22:
    /* 1D22  imul    di */
    imul16(DI);
L1D24:
    /* 1D24  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L1D26:
    /* 1D26  mov     ax,word ptr ds:[14B6h] */
    AX = rw(pDS, 0x14B6);
L1D29:
    /* 1D29  imul    di */
    imul16(DI);
L1D2B:
    /* 1D2B  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L1D2D:
    /* 1D2D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D2E:
    /* 1D2E  mov     di,ax */
    DI = AX;
L1D30:
    /* 1D30  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L1D34:
    /* 1D34  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L1D38:
    /* 1D38  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L1D3C:
    /* 1D3C  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1D3F)) != 0) return c;
L1D3F:
    /* 1D3F  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L1D43:
    /* 1D43  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D44:
    /* 1D44  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1D45:
    /* 1D45  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1D49  (+1D49)
       flat_ variant of do_z_rel. */
L1D49: /* _flat_z_rel */
    /* 1D49  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D4A:
    /* 1D4A  mov     di,ax */
    DI = AX;
L1D4C:
    /* 1D4C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D4D:
    /* 1D4D  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1D51:
    /* 1D51  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1D53:
    /* 1D53  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L1D57:
    /* 1D57  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L1D5B:
    /* 1D5B  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L1D5F:
    /* 1D5F  mov     di,ax */
    DI = AX;
L1D61:
    /* 1D61  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L1D65:
    /* 1D65  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L1D67:
    /* 1D67  mov     ax,word ptr ds:[14C2h] */
    AX = rw(pDS, 0x14C2);
L1D6A:
    /* 1D6A  imul    di */
    imul16(DI);
L1D6C:
    /* 1D6C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D6D:
    /* 1D6D  mov     di,ax */
    DI = AX;
L1D6F:
    /* 1D6F  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L1D73:
    /* 1D73  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L1D77:
    /* 1D77  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L1D79:
    /* 1D79  add     bp,dx */
    BP = add16(BP, DX, 0);
L1D7B:
    /* 1D7B  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L1D7F:
    /* 1D7F  jg      short L1D83 */
    if (!ZF && SF == OF) goto L1D83;
L1D81:
    /* 1D81  mov     al,80h */
    AL = 0x80;
L1D83: /* L1D83 */
    /* 1D83  cmp     bx,bp */
    sub16(BX, BP, 0);
L1D85:
    /* 1D85  jle     short L1D89 */
    if (ZF || SF != OF) goto L1D89;
L1D87:
    /* 1D87  inc     ax */
    AX = (uint16_t)(AX + 1);
L1D88:
    /* 1D88  inc     ax */
    AX = (uint16_t)(AX + 1);
L1D89: /* L1D89 */
    /* 1D89  cmp     cx,bp */
    sub16(CX, BP, 0);
L1D8B:
    /* 1D8B  jle     short L1D8F */
    if (ZF || SF != OF) goto L1D8F;
L1D8D:
    /* 1D8D  add     al,4 */
    AL = (uint8_t)(AL + 0x4);
L1D8F: /* L1D8F */
    /* 1D8F  add     bx,bp */
    BX = add16(BX, BP, 0);
L1D91:
    /* 1D91  jge     short L1D94 */
    if (SF == OF) goto L1D94;
L1D93:
    /* 1D93  inc     ax */
    AX = (uint16_t)(AX + 1);
L1D94: /* L1D94 */
    /* 1D94  add     cx,bp */
    CX = add16(CX, BP, 0);
L1D96:
    /* 1D96  jge     short L1D9A */
    if (SF == OF) goto L1D9A;
L1D98:
    /* 1D98  add     al,8 */
    AL = add8(AL, 0x8, 0);
L1D9A: /* L1D9A */
    /* 1D9A  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L1D9E:
    /* 1D9E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1D9F:
    /* 1D9F  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1DA0:
    /* 1DA0  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1DA4  (+1DA4)
       Opcodes 8Ah and 2Ch, operands: source point, dz, destination point. As do_x_rel along z. */
L1DA4: /* _do_z_rel */
    /* 1DA4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DA5:
    /* 1DA5  mov     di,ax */
    DI = AX;
L1DA7:
    /* 1DA7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DA8:
    /* 1DA8  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1DAC:
    /* 1DAC  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1DAE:
    /* 1DAE  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L1DB2:
    /* 1DB2  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L1DB6:
    /* 1DB6  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L1DBA:
    /* 1DBA  mov     di,ax */
    DI = AX;
L1DBC:
    /* 1DBC  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L1DC0:
    /* 1DC0  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L1DC2:
    /* 1DC2  mov     ax,word ptr ds:[14C0h] */
    AX = rw(pDS, 0x14C0);
L1DC5:
    /* 1DC5  imul    di */
    imul16(DI);
L1DC7:
    /* 1DC7  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L1DC9:
    /* 1DC9  mov     ax,word ptr ds:[14C2h] */
    AX = rw(pDS, 0x14C2);
L1DCC:
    /* 1DCC  imul    di */
    imul16(DI);
L1DCE:
    /* 1DCE  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L1DD0:
    /* 1DD0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DD1:
    /* 1DD1  mov     di,ax */
    DI = AX;
L1DD3:
    /* 1DD3  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L1DD7:
    /* 1DD7  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L1DDB:
    /* 1DDB  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L1DDF:
    /* 1DDF  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1DE2)) != 0) return c;
L1DE2:
    /* 1DE2  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L1DE6:
    /* 1DE6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DE7:
    /* 1DE7  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1DE8:
    /* 1DE8  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1DEC  (+1DEC)
       Opcode 92h, operands: dx, dz, source point, destination point. Offset along two axes. */
L1DEC: /* _do_xz_rel */
    /* 1DEC  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1DF0:
    /* 1DF0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1DF1:
    /* 1DF1  shl     ax,cl */
    AX = shl16(AX, CL);
L1DF3:
    /* 1DF3  mov     di,ax */
    DI = AX;
L1DF5:
    /* 1DF5  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L1DF9:
    /* 1DF9  mov     bx,dx */
    BX = DX;
L1DFB:
    /* 1DFB  mov     ax,word ptr ds:[14B6h] */
    AX = rw(pDS, 0x14B6);
L1DFE:
    /* 1DFE  imul    di */
    imul16(DI);
L1E00:
    /* 1E00  mov     bp,dx */
    BP = DX;
L1E02:
    /* 1E02  mov     ax,word ptr ds:[14B4h] */
    AX = rw(pDS, 0x14B4);
L1E05:
    /* 1E05  imul    di */
    imul16(DI);
L1E07:
    /* 1E07  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E08:
    /* 1E08  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1E0A:
    /* 1E0A  mov     cx,dx */
    CX = DX;
L1E0C:
    /* 1E0C  mov     di,ax */
    DI = AX;
L1E0E:
    /* 1E0E  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L1E12:
    /* 1E12  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L1E14:
    /* 1E14  mov     ax,word ptr ds:[14C0h] */
    AX = rw(pDS, 0x14C0);
L1E17:
    /* 1E17  imul    di */
    imul16(DI);
L1E19:
    /* 1E19  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L1E1B:
    /* 1E1B  mov     ax,word ptr ds:[14C2h] */
    AX = rw(pDS, 0x14C2);
L1E1E:
    /* 1E1E  imul    di */
    imul16(DI);
L1E20:
    /* 1E20  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L1E22:
    /* 1E22  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E23:
    /* 1E23  mov     di,ax */
    DI = AX;
L1E25:
    /* 1E25  add     bx,word ptr [di+14D0h] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x14D0));
L1E29:
    /* 1E29  add     cx,word ptr [di+14D2h] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x14D2));
L1E2D:
    /* 1E2D  add     bp,word ptr [di+14D4h] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x14D4));
L1E31:
    /* 1E31  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E32:
    /* 1E32  mov     di,ax */
    DI = AX;
L1E34:
    /* 1E34  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L1E38:
    /* 1E38  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L1E3C:
    /* 1E3C  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L1E40:
    /* 1E40  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1E43)) != 0) return c;
L1E43:
    /* 1E43  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L1E47:
    /* 1E47  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E48:
    /* 1E48  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1E49:
    /* 1E49  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1E4D  (+1E4D)
       Opcode 90h, operands: dx, dy, source point, destination point. */
L1E4D: /* _do_xy_rel */
    /* 1E4D  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1E51:
    /* 1E51  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E52:
    /* 1E52  shl     ax,cl */
    AX = shl16(AX, CL);
L1E54:
    /* 1E54  mov     di,ax */
    DI = AX;
L1E56:
    /* 1E56  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L1E5A:
    /* 1E5A  mov     bx,dx */
    BX = DX;
L1E5C:
    /* 1E5C  mov     ax,word ptr ds:[14B6h] */
    AX = rw(pDS, 0x14B6);
L1E5F:
    /* 1E5F  imul    di */
    imul16(DI);
L1E61:
    /* 1E61  mov     bp,dx */
    BP = DX;
L1E63:
    /* 1E63  mov     ax,word ptr ds:[14B4h] */
    AX = rw(pDS, 0x14B4);
L1E66:
    /* 1E66  imul    di */
    imul16(DI);
L1E68:
    /* 1E68  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E69:
    /* 1E69  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1E6B:
    /* 1E6B  mov     cx,dx */
    CX = DX;
L1E6D:
    /* 1E6D  mov     di,ax */
    DI = AX;
L1E6F:
    /* 1E6F  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L1E73:
    /* 1E73  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L1E75:
    /* 1E75  mov     ax,word ptr ds:[14BAh] */
    AX = rw(pDS, 0x14BA);
L1E78:
    /* 1E78  imul    di */
    imul16(DI);
L1E7A:
    /* 1E7A  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L1E7C:
    /* 1E7C  mov     ax,word ptr ds:[14BCh] */
    AX = rw(pDS, 0x14BC);
L1E7F:
    /* 1E7F  imul    di */
    imul16(DI);
L1E81:
    /* 1E81  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L1E83:
    /* 1E83  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E84:
    /* 1E84  mov     di,ax */
    DI = AX;
L1E86:
    /* 1E86  add     bx,word ptr [di+14D0h] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x14D0));
L1E8A:
    /* 1E8A  add     cx,word ptr [di+14D2h] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x14D2));
L1E8E:
    /* 1E8E  add     bp,word ptr [di+14D4h] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x14D4));
L1E92:
    /* 1E92  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1E93:
    /* 1E93  mov     di,ax */
    DI = AX;
L1E95:
    /* 1E95  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L1E99:
    /* 1E99  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L1E9D:
    /* 1E9D  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L1EA1:
    /* 1EA1  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1EA4)) != 0) return c;
L1EA4:
    /* 1EA4  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L1EA8:
    /* 1EA8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1EA9:
    /* 1EA9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1EAA:
    /* 1EAA  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1EAE  (+1EAE)
       Opcode 94h, operands: dy, dz, source point, destination point. (Note the operand order: the
       first operand is multiplied by the z row and the second by the y row, as in the code.) */
L1EAE: /* _do_yz_rel */
    /* 1EAE  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1EB2:
    /* 1EB2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1EB3:
    /* 1EB3  shl     ax,cl */
    AX = shl16(AX, CL);
L1EB5:
    /* 1EB5  mov     di,ax */
    DI = AX;
L1EB7:
    /* 1EB7  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L1EBB:
    /* 1EBB  mov     bx,dx */
    BX = DX;
L1EBD:
    /* 1EBD  mov     ax,word ptr ds:[14C2h] */
    AX = rw(pDS, 0x14C2);
L1EC0:
    /* 1EC0  imul    di */
    imul16(DI);
L1EC2:
    /* 1EC2  mov     bp,dx */
    BP = DX;
L1EC4:
    /* 1EC4  mov     ax,word ptr ds:[14C0h] */
    AX = rw(pDS, 0x14C0);
L1EC7:
    /* 1EC7  imul    di */
    imul16(DI);
L1EC9:
    /* 1EC9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1ECA:
    /* 1ECA  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1ECC:
    /* 1ECC  mov     cx,dx */
    CX = DX;
L1ECE:
    /* 1ECE  mov     di,ax */
    DI = AX;
L1ED0:
    /* 1ED0  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L1ED4:
    /* 1ED4  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L1ED6:
    /* 1ED6  mov     ax,word ptr ds:[14BAh] */
    AX = rw(pDS, 0x14BA);
L1ED9:
    /* 1ED9  imul    di */
    imul16(DI);
L1EDB:
    /* 1EDB  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L1EDD:
    /* 1EDD  mov     ax,word ptr ds:[14BCh] */
    AX = rw(pDS, 0x14BC);
L1EE0:
    /* 1EE0  imul    di */
    imul16(DI);
L1EE2:
    /* 1EE2  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L1EE4:
    /* 1EE4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1EE5:
    /* 1EE5  mov     di,ax */
    DI = AX;
L1EE7:
    /* 1EE7  add     bx,word ptr [di+14D0h] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x14D0));
L1EEB:
    /* 1EEB  add     cx,word ptr [di+14D2h] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x14D2));
L1EEF:
    /* 1EEF  add     bp,word ptr [di+14D4h] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x14D4));
L1EF3:
    /* 1EF3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1EF4:
    /* 1EF4  mov     di,ax */
    DI = AX;
L1EF6:
    /* 1EF6  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L1EFA:
    /* 1EFA  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L1EFE:
    /* 1EFE  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L1F02:
    /* 1F02  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1F05)) != 0) return c;
L1F05:
    /* 1F05  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L1F09:
    /* 1F09  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1F0A:
    /* 1F0A  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1F0B:
    /* 1F0B  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1F0F  (+1F0F)
       flat_ variant of do_xz_rel. */
L1F0F: /* _flat_xz_rel */
    /* 1F0F  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1F13:
    /* 1F13  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1F14:
    /* 1F14  shl     ax,cl */
    AX = shl16(AX, CL);
L1F16:
    /* 1F16  mov     di,ax */
    DI = AX;
L1F18:
    /* 1F18  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L1F1C:
    /* 1F1C  mov     bx,dx */
    BX = DX;
L1F1E:
    /* 1F1E  mov     ax,word ptr ds:[14B6h] */
    AX = rw(pDS, 0x14B6);
L1F21:
    /* 1F21  imul    di */
    imul16(DI);
L1F23:
    /* 1F23  mov     bp,dx */
    BP = DX;
L1F25:
    /* 1F25  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1F26:
    /* 1F26  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1F28:
    /* 1F28  mov     di,ax */
    DI = AX;
L1F2A:
    /* 1F2A  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L1F2E:
    /* 1F2E  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L1F30:
    /* 1F30  mov     ax,word ptr ds:[14C2h] */
    AX = rw(pDS, 0x14C2);
L1F33:
    /* 1F33  imul    di */
    imul16(DI);
L1F35:
    /* 1F35  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L1F37:
    /* 1F37  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1F38:
    /* 1F38  mov     di,ax */
    DI = AX;
L1F3A:
    /* 1F3A  add     bx,word ptr [di+14D0h] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x14D0));
L1F3E:
    /* 1F3E  add     bp,word ptr [di+14D4h] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x14D4));
L1F42:
    /* 1F42  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L1F46:
    /* 1F46  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1F47:
    /* 1F47  mov     di,ax */
    DI = AX;
L1F49:
    /* 1F49  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L1F4D:
    /* 1F4D  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L1F51:
    /* 1F51  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L1F55:
    /* 1F55  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1F58)) != 0) return c;
L1F58:
    /* 1F58  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L1F5C:
    /* 1F5C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1F5D:
    /* 1F5D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1F5E:
    /* 1F5E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1F62  (+1F62)
       flat_ variant of do_xy_rel. */
L1F62: /* _flat_xy_rel */
    /* 1F62  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1F66:
    /* 1F66  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1F67:
    /* 1F67  shl     ax,cl */
    AX = shl16(AX, CL);
L1F69:
    /* 1F69  mov     di,ax */
    DI = AX;
L1F6B:
    /* 1F6B  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L1F6F:
    /* 1F6F  mov     bx,dx */
    BX = DX;
L1F71:
    /* 1F71  mov     ax,word ptr ds:[14B6h] */
    AX = rw(pDS, 0x14B6);
L1F74:
    /* 1F74  imul    di */
    imul16(DI);
L1F76:
    /* 1F76  mov     bp,dx */
    BP = DX;
L1F78:
    /* 1F78  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1F79:
    /* 1F79  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1F7B:
    /* 1F7B  sar     ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
L1F7D:
    /* 1F7D  mov     cx,ax */
    CX = AX;
L1F7F:
    /* 1F7F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1F80:
    /* 1F80  mov     di,ax */
    DI = AX;
L1F82:
    /* 1F82  add     bx,word ptr [di+14D0h] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x14D0));
L1F86:
    /* 1F86  add     cx,word ptr [di+14D2h] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x14D2));
L1F8A:
    /* 1F8A  add     bp,word ptr [di+14D4h] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x14D4));
L1F8E:
    /* 1F8E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1F8F:
    /* 1F8F  mov     di,ax */
    DI = AX;
L1F91:
    /* 1F91  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L1F95:
    /* 1F95  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L1F99:
    /* 1F99  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L1F9D:
    /* 1F9D  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1FA0)) != 0) return c;
L1FA0:
    /* 1FA0  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
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
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1FAA  (+1FAA)
       flat_ variant of do_yz_rel. */
L1FAA: /* _flat_yz_rel */
    /* 1FAA  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L1FAE:
    /* 1FAE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1FAF:
    /* 1FAF  shl     ax,cl */
    AX = shl16(AX, CL);
L1FB1:
    /* 1FB1  mov     di,ax */
    DI = AX;
L1FB3:
    /* 1FB3  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L1FB7:
    /* 1FB7  mov     bx,dx */
    BX = DX;
L1FB9:
    /* 1FB9  mov     ax,word ptr ds:[14C2h] */
    AX = rw(pDS, 0x14C2);
L1FBC:
    /* 1FBC  imul    di */
    imul16(DI);
L1FBE:
    /* 1FBE  mov     bp,dx */
    BP = DX;
L1FC0:
    /* 1FC0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1FC1:
    /* 1FC1  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L1FC3:
    /* 1FC3  sar     ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
L1FC5:
    /* 1FC5  mov     cx,ax */
    CX = AX;
L1FC7:
    /* 1FC7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1FC8:
    /* 1FC8  mov     di,ax */
    DI = AX;
L1FCA:
    /* 1FCA  add     bx,word ptr [di+14D0h] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x14D0));
L1FCE:
    /* 1FCE  add     cx,word ptr [di+14D2h] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x14D2));
L1FD2:
    /* 1FD2  add     bp,word ptr [di+14D4h] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x14D4));
L1FD6:
    /* 1FD6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1FD7:
    /* 1FD7  mov     di,ax */
    DI = AX;
L1FD9:
    /* 1FD9  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L1FDD:
    /* 1FDD  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L1FE1:
    /* 1FE1  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L1FE5:
    /* 1FE5  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x1FE8)) != 0) return c;
L1FE8:
    /* 1FE8  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L1FEC:
    /* 1FEC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1FED:
    /* 1FED  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L1FEE:
    /* 1FEE  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_1FF2  (+1FF2)
       Opcode 2Eh, operand: colour. Selects flat filling (14A6h = 3B0Dh, 14A8h = 38B4h, seg003
       offsets) and sets the colour through seg003's _seg003_0272_536B (with DS = seg_370D). Its
       second label, setcol_common (FM), is shared by do_setcolv, do_uwsurf and PGCACHE. */
L1FF2: /* _do_setcolor */
    /* 1FF2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L1FF3: /* setcol_common */
    /* 1FF3  mov     bx,3B0Dh */
    BX = 0x3B0D;
L1FF6:
    /* 1FF6  mov     word ptr ds:[14A6h],bx */
    ww(pDS, 0x14A6, BX);
L1FFA:
    /* 1FFA  mov     bx,38B4h */
    BX = 0x38B4;
L1FFD:
    /* 1FFD  mov     word ptr ds:[14A8h],bx */
    ww(pDS, 0x14A8, BX);
L2001:
    /* 2001  mov     bx,seg seg_370D */
    BX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L2004:
    /* 2004  mov     ds,bx */
    SET_DS(BX);
L2006:
    /* 2006  call    far ptr _seg003_0272_536B */
    if ((c = asm_callf(ASM_JMP(0x0085, 0x536B), 0x065C + PORT_LOAD_SEG, 0x200B)) != 0) return c;
L200B:
    /* 200B  mov     bp,seg seg052_519C */
    BP = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L200E:
    /* 200E  mov     ds,bp */
    SET_DS(BP);
L2010:
    /* 2010  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2011:
    /* 2011  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2012:
    /* 2012  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_2016  (+2016)
       Opcode 5Ch, operands: address, colour. Colour = [address] + colour, then setcol_common. */
L2016: /* _do_setcolv */
    /* 2016  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2017:
    /* 2017  mov     di,ax */
    DI = AX;
L2019:
    /* 2019  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L201A:
    /* 201A  add     ax,word ptr [di] */
    AX = (uint16_t)(AX + rw(pDS, DI));
L201C:
    /* 201C  jmp     setcol_common */
    goto L1FF3;

    /* seg004_0849_201E  (+201E)
       Opcode CAh, operand: address. do_goursurf with the colour taken from a model variable: the
       `db 0A8h` turns do_goursurf's lodsw into the operand of a `test al,` so AX is kept. */
L201E: /* _do_goursurfv */
    /* 201E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L201F:
    /* 201F  mov     bx,ax */
    BX = AX;
L2021:
    /* 2021  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L2023:
    /* 2023  db      0A8h */
    goto L2025;

    /* seg004_0849_2024  (+2024)
       Opcode C8h, operand: colour. Selects Gouraud filling (14A6h = _seg003_0272_5946, 14A8h =
       _5956) and stores the colour's shading-table byte (seg_370D:21Fh + 2 * colour) at 1B0h. */
L2024: /* _do_goursurf */
    /* 2024  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2025:
    /* 2025  mov     bx,ax */
    BX = AX;
L2027:
    /* 2027  push    es */
    push16(asm_es);
L2028:
    /* 2028  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L202B:
    /* 202B  mov     es,ax */
    SET_ES(AX);
L202D:
    /* 202D  shl     bx,1 */
    BX = shl16(BX, 1);
L202F:
    /* 202F  mov     al,byte ptr es:[bx+21Fh] */
    AL = rb(pES, BX + 0x21F);
L2034:
    /* 2034  pop     es */
    SET_ES(pop16());
L2035:
    /* 2035  mov     byte ptr ds:[1B0h],al */
    wb(pDS, 0x1B0, AL);
L2038:
    /* 2038  mov     word ptr ds:[14A6h],offset _seg003_0272_5946 */
    ww(pDS, 0x14A6, 0x5946);
L203E:
    /* 203E  mov     word ptr ds:[14A8h],offset _seg003_0272_5956 */
    ww(pDS, 0x14A8, 0x5956);
L2044:
    /* 2044  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2045:
    /* 2045  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2046:
    /* 2046  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_204A  (+204A)
       Opcode D6h (UW-specific): the standard surface. When the low byte of 2692h is zero,
       selects Gouraud filling and points seg003's handler pair at 0BC3h/0BC5h at 0BC7h/0C93h;
       otherwise sets the flat colour from seg_370D:0DC1h through setcol_common. */
L204A: /* _do_uwsurf */
    /* 204A  test    word ptr ds:[2692h],0FFh */
    logic16((uint16_t)(rw(pDS, 0x2692) & 0xFF));
L2050:
    /* 2050  jne     short L207B */
    if (!ZF) goto L207B;
L2052:
    /* 2052  mov     word ptr ds:[14A6h],offset _seg003_0272_5946 */
    ww(pDS, 0x14A6, 0x5946);
L2058:
    /* 2058  mov     word ptr ds:[14A8h],offset _seg003_0272_5956 */
    ww(pDS, 0x14A8, 0x5956);
L205E:
    /* 205E  mov     bx,es */
    BX = asm_es;
L2060:
    /* 2060  mov     ax,seg seg003_0272 */
    AX = (uint16_t)(0x0085 + PORT_LOAD_SEG);
L2063:
    /* 2063  mov     es,ax */
    SET_ES(AX);
L2065:
    /* 2065  mov     word ptr es:[0BC3h],0BC7h */
    ww(pES, 0xBC3, 0xBC7);
L206C:
    /* 206C  mov     word ptr es:[0BC5h],0C93h */
    ww(pES, 0xBC5, 0xC93);
L2073:
    /* 2073  mov     es,bx */
    SET_ES(BX);
L2075:
    /* 2075  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2076:
    /* 2076  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2077:
    /* 2077  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L207B: /* L207B */
    /* 207B  mov     bx,es */
    BX = asm_es;
L207D:
    /* 207D  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L2080:
    /* 2080  mov     es,ax */
    SET_ES(AX);
L2082:
    /* 2082  mov     al,byte ptr es:[0DC1h] */
    AL = rb(pES, 0xDC1);
L2086:
    /* 2086  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L2088:
    /* 2088  mov     es,bx */
    SET_ES(BX);
L208A:
    /* 208A  jmp     setcol_common */
    goto L1FF3;

    /* seg004_0849_208D  (+208D)
       Opcode D8h: a translucent surface. Gouraud filling with seg003's handler pair at
       0BC3h/0BC5h pointed at 0BF2h/0CD9h (the same pair smooth_over uses). */
L208D: /* _do_transsurf */
    /* 208D  mov     word ptr ds:[14A6h],offset _seg003_0272_5946 */
    ww(pDS, 0x14A6, 0x5946);
L2093:
    /* 2093  mov     word ptr ds:[14A8h],offset _seg003_0272_5956 */
    ww(pDS, 0x14A8, 0x5956);
L2099:
    /* 2099  mov     bx,es */
    BX = asm_es;
L209B:
    /* 209B  mov     ax,seg seg003_0272 */
    AX = (uint16_t)(0x0085 + PORT_LOAD_SEG);
L209E:
    /* 209E  mov     es,ax */
    SET_ES(AX);
L20A0:
    /* 20A0  mov     word ptr es:[0BC3h],0BF2h */
    ww(pES, 0xBC3, 0xBF2);
L20A7:
    /* 20A7  mov     word ptr es:[0BC5h],0CD9h */
    ww(pES, 0xBC5, 0xCD9);
L20AE:
    /* 20AE  mov     es,bx */
    SET_ES(BX);
L20B0:
    /* 20B0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20B1:
    /* 20B1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L20B2:
    /* 20B2  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_20B6  (+20B6)
       Opcode 44h: selects flat filling (3B0Dh, 38B4h) without setting a colour. */
L20B6: /* _do_vexsurf */
    /* 20B6  mov     word ptr ds:[14A6h],3B0Dh */
    ww(pDS, 0x14A6, 0x3B0D);
L20BC:
    /* 20BC  mov     word ptr ds:[14A8h],38B4h */
    ww(pDS, 0x14A8, 0x38B4);
L20C2:
    /* 20C2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20C3:
    /* 20C3  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L20C4:
    /* 20C4  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_20C8  (+20C8)
       Opcode 40h: selects the fill routine 249Ch for both slots (seg003; what it draws is not
       known). The unlabelled code after its dispatch is FM's do_setpat (20DAh), do_putpat
       (211Ah) and do_usepat (2212h), in FM's order; the DOS opcode table has no entry for any of
       them. Roughly: do_setpat rotates a list of vectors into a table, and do_putpat and
       do_usepat place points from such a table at a rotated offset (do_putpat then runs a
       sub-program); not traced further. */
L20C8: /* _do_cavesurf */
    /* 20C8  mov     word ptr ds:[14A6h],249Ch */
    ww(pDS, 0x14A6, 0x249C);
L20CE:
    /* 20CE  mov     word ptr ds:[14A8h],249Ch */
    ww(pDS, 0x14A8, 0x249C);
L20D4:
    /* 20D4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20D5:
    /* 20D5  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L20D6:
    /* 20D6  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L20DA:
    /* 20DA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20DB:
    /* 20DB  push    si */
    push16(SI);
L20DC:
    /* 20DC  add     si,ax */
    SI = (uint16_t)(SI + AX);
L20DE:
    /* 20DE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20DF:
    /* 20DF  mov     byte ptr ds:[25D6h],al */
    wb(pDS, 0x25D6, AL);
L20E2:
    /* 20E2  inc     si */
    SI = (uint16_t)(SI + 1);
L20E3:
    /* 20E3  inc     si */
    SI = (uint16_t)(SI + 1);
L20E4:
    /* 20E4  mov     word ptr ds:[24B8h],si */
    ww(pDS, 0x24B8, SI);
L20E8:
    /* 20E8  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L20EA:
    /* 20EA  add     si,ax */
    SI = (uint16_t)(SI + AX);
L20EC:
    /* 20EC  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L20EE:
    /* 20EE  add     si,ax */
    SI = (uint16_t)(SI + AX);
L20F0: /* L20F0 */
    /* 20F0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20F1:
    /* 20F1  mov     bx,ax */
    BX = AX;
L20F3:
    /* 20F3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20F4:
    /* 20F4  mov     cx,ax */
    CX = AX;
L20F6:
    /* 20F6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L20F7:
    /* 20F7  mov     bp,ax */
    BP = AX;
L20F9:
    /* 20F9  call    _mxmul */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3534), 0x20FC)) != 0) return c;
L20FC:
    /* 20FC  mov     di,word ptr ds:[24B8h] */
    DI = rw(pDS, 0x24B8);
L2100:
    /* 2100  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2102:
    /* 2102  mov     word ptr [di+4],cx */
    ww(pDS, DI + 0x4, CX);
L2105:
    /* 2105  mov     word ptr [di+2],bp */
    ww(pDS, DI + 0x2, BP);
L2108:
    /* 2108  add     word ptr ds:[24B8h],6 */
    ww(pDS, 0x24B8, add16(rw(pDS, 0x24B8), 0x6, 0));
L210D:
    /* 210D  dec     byte ptr ds:[25D6h] */
    wb(pDS, 0x25D6, dec8(rb(pDS, 0x25D6)));
L2111:
    /* 2111  jne     L20F0 */
    if (!ZF) goto L20F0;
L2113:
    /* 2113  pop     si */
    SI = pop16();
L2114:
    /* 2114  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2115:
    /* 2115  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2116:
    /* 2116  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L211A:
    /* 211A  push    word ptr ds:[25E6h] */
    push16(rw(pDS, 0x25E6));
L211E:
    /* 211E  push    word ptr ds:[25E8h] */
    push16(rw(pDS, 0x25E8));
L2122:
    /* 2122  push    word ptr ds:[25EAh] */
    push16(rw(pDS, 0x25EA));
L2126:
    /* 2126  call    _load_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x351A), 0x2129)) != 0) return c;
L2129:
    /* 2129  mov     word ptr ds:[14ACh],bx */
    ww(pDS, 0x14AC, BX);
L212D:
    /* 212D  mov     word ptr ds:[14AEh],cx */
    ww(pDS, 0x14AE, CX);
L2131:
    /* 2131  mov     word ptr ds:[14B0h],bp */
    ww(pDS, 0x14B0, BP);
L2135:
    /* 2135  mov     ax,word ptr ds:[25E6h] */
    AX = rw(pDS, 0x25E6);
L2138:
    /* 2138  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L213B:
    /* 213B  mov     word ptr ds:[25E6h],ax */
    ww(pDS, 0x25E6, AX);
L213E:
    /* 213E  mov     ax,word ptr ds:[25E8h] */
    AX = rw(pDS, 0x25E8);
L2141:
    /* 2141  sub     ax,word ptr [si-2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFE));
L2144:
    /* 2144  mov     word ptr ds:[25E8h],ax */
    ww(pDS, 0x25E8, AX);
L2147:
    /* 2147  mov     ax,word ptr ds:[25EAh] */
    AX = rw(pDS, 0x25EA);
L214A:
    /* 214A  sub     ax,word ptr [si-4] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFC));
L214D:
    /* 214D  mov     word ptr ds:[25EAh],ax */
    ww(pDS, 0x25EA, AX);
L2150:
    /* 2150  mov     bp,14D0h */
    BP = 0x14D0;
L2153:
    /* 2153  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2154:
    /* 2154  add     di,ax */
    DI = (uint16_t)(DI + AX);
L2156:
    /* 2156  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2157:
    /* 2157  push    si */
    push16(SI);
L2158:
    /* 2158  add     si,ax */
    SI = (uint16_t)(SI + AX);
L215A:
    /* 215A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L215B:
    /* 215B  mov     dx,ax */
    DX = AX;
L215D:
    /* 215D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L215E:
    /* 215E  sub     al,byte ptr ds:[25E0h] */
    AL = (uint8_t)(AL - rb(pDS, 0x25E0));
L2162:
    /* 2162  mov     dh,al */
    DH = AL;
L2164:
    /* 2164  shr     dl,1 */
    DL = shr8(DL, 1);
L2166:
    /* 2166  jae     short L2196 */
    if (!CF) goto L2196;
L2168:
    /* 2168  mov     cl,dh */
    CL = DH;
L216A:
    /* 216A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L216B:
    /* 216B  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L216D:
    /* 216D  mov     bx,ax */
    BX = AX;
L216F:
    /* 216F  add     bx,word ptr ds:[14ACh] */
    BX = add16(BX, rw(pDS, 0x14AC), 0);
L2173:
    /* 2173  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2174:
    /* 2174  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2176:
    /* 2176  mov     bp,ax */
    BP = AX;
L2178:
    /* 2178  add     bp,word ptr ds:[14B0h] */
    BP = add16(BP, rw(pDS, 0x14B0), 0);
L217C:
    /* 217C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L217D:
    /* 217D  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L217F:
    /* 217F  mov     cx,ax */
    CX = AX;
L2181:
    /* 2181  add     cx,word ptr ds:[14AEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x14AE));
L2185:
    /* 2185  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2188)) != 0) return c;
L2188:
    /* 2188  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L218A:
    /* 218A  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L218D:
    /* 218D  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L2190:
    /* 2190  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L2193:
    /* 2193  add     di,8 */
    DI = add16(DI, 0x8, 0);
L2196: /* L2196 */
    /* 2196  mov     cl,dh */
    CL = DH;
L2198:
    /* 2198  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2199:
    /* 2199  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L219B:
    /* 219B  mov     bx,ax */
    BX = AX;
L219D:
    /* 219D  add     bx,word ptr ds:[14ACh] */
    BX = add16(BX, rw(pDS, 0x14AC), 0);
L21A1:
    /* 21A1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L21A2:
    /* 21A2  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L21A4:
    /* 21A4  mov     bp,ax */
    BP = AX;
L21A6:
    /* 21A6  add     bp,word ptr ds:[14B0h] */
    BP = add16(BP, rw(pDS, 0x14B0), 0);
L21AA:
    /* 21AA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L21AB:
    /* 21AB  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L21AD:
    /* 21AD  mov     cx,ax */
    CX = AX;
L21AF:
    /* 21AF  add     cx,word ptr ds:[14AEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x14AE));
L21B3:
    /* 21B3  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x21B6)) != 0) return c;
L21B6:
    /* 21B6  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L21B8:
    /* 21B8  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L21BB:
    /* 21BB  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L21BE:
    /* 21BE  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L21C1:
    /* 21C1  mov     cl,dh */
    CL = DH;
L21C3:
    /* 21C3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L21C4:
    /* 21C4  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L21C6:
    /* 21C6  mov     bx,ax */
    BX = AX;
L21C8:
    /* 21C8  add     bx,word ptr ds:[14ACh] */
    BX = add16(BX, rw(pDS, 0x14AC), 0);
L21CC:
    /* 21CC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L21CD:
    /* 21CD  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L21CF:
    /* 21CF  mov     bp,ax */
    BP = AX;
L21D1:
    /* 21D1  add     bp,word ptr ds:[14B0h] */
    BP = add16(BP, rw(pDS, 0x14B0), 0);
L21D5:
    /* 21D5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L21D6:
    /* 21D6  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L21D8:
    /* 21D8  mov     cx,ax */
    CX = AX;
L21DA:
    /* 21DA  add     cx,word ptr ds:[14AEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x14AE));
L21DE:
    /* 21DE  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x21E1)) != 0) return c;
L21E1:
    /* 21E1  mov     word ptr [di+8],bx */
    ww(pDS, DI + 0x8, BX);
L21E4:
    /* 21E4  mov     word ptr [di+0Ah],cx */
    ww(pDS, DI + 0xA, CX);
L21E7:
    /* 21E7  mov     word ptr [di+0Ch],bp */
    ww(pDS, DI + 0xC, BP);
L21EA:
    /* 21EA  mov     byte ptr [di+0Eh],al */
    wb(pDS, DI + 0xE, AL);
L21ED:
    /* 21ED  add     di,10h */
    DI = add16(DI, 0x10, 0);
L21F0:
    /* 21F0  dec     dl */
    DL = dec8(DL);
L21F2:
    /* 21F2  jne     L2196 */
    if (!ZF) goto L2196;
L21F4:
    /* 21F4  pop     si */
    SI = pop16();
L21F5:
    /* 21F5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L21F6:
    /* 21F6  push    si */
    push16(SI);
L21F7:
    /* 21F7  add     si,ax */
    SI = (uint16_t)(SI + AX);
L21F9:
    /* 21F9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L21FA:
    /* 21FA  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L21FB:
    /* 21FB  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x21FF)) != 0) return c;
L21FF:
    /* 21FF  pop     si */
    SI = pop16();
L2200:
    /* 2200  pop     word ptr ds:[25EAh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25EA, t_); }
L2204:
    /* 2204  pop     word ptr ds:[25E8h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E8, t_); }
L2208:
    /* 2208  pop     word ptr ds:[25E6h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E6, t_); }
L220C:
    /* 220C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L220D:
    /* 220D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L220E:
    /* 220E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L2212:
    /* 2212  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L2216:
    /* 2216  mov     bx,word ptr ds:[25E6h] */
    BX = rw(pDS, 0x25E6);
L221A:
    /* 221A  neg     bx */
    BX = neg16(BX);
L221C:
    /* 221C  shl     bx,cl */
    BX = (uint16_t)(BX << (CL & 31));
L221E:
    /* 221E  mov     bp,word ptr ds:[25EAh] */
    BP = rw(pDS, 0x25EA);
L2222:
    /* 2222  neg     bp */
    BP = neg16(BP);
L2224:
    /* 2224  shl     bp,cl */
    BP = (uint16_t)(BP << (CL & 31));
L2226:
    /* 2226  mov     ax,word ptr ds:[25E8h] */
    AX = rw(pDS, 0x25E8);
L2229:
    /* 2229  neg     ax */
    AX = neg16(AX);
L222B:
    /* 222B  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L222D:
    /* 222D  mov     cx,ax */
    CX = AX;
L222F:
    /* 222F  call    _mxmul */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3534), 0x2232)) != 0) return c;
L2232:
    /* 2232  mov     word ptr ds:[14ACh],bx */
    ww(pDS, 0x14AC, BX);
L2236:
    /* 2236  mov     word ptr ds:[14AEh],cx */
    ww(pDS, 0x14AE, CX);
L223A:
    /* 223A  mov     word ptr ds:[14B0h],bp */
    ww(pDS, 0x14B0, BP);
L223E:
    /* 223E  mov     di,14D0h */
    DI = 0x14D0;
L2241:
    /* 2241  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2242:
    /* 2242  add     di,ax */
    DI = (uint16_t)(DI + AX);
L2244:
    /* 2244  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2245:
    /* 2245  push    si */
    push16(SI);
L2246:
    /* 2246  add     si,ax */
    SI = (uint16_t)(SI + AX);
L2248:
    /* 2248  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2249:
    /* 2249  mov     cx,ax */
    CX = AX;
L224B:
    /* 224B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L224C:
    /* 224C  sub     al,byte ptr ds:[25E0h] */
    AL = (uint8_t)(AL - rb(pDS, 0x25E0));
L2250:
    /* 2250  mov     dl,al */
    DL = AL;
L2252:
    /* 2252  shr     cl,1 */
    CL = shr8(CL, 1);
L2254:
    /* 2254  mov     dh,cl */
    DH = CL;
L2256:
    /* 2256  jae     short L2286 */
    if (!CF) goto L2286;
L2258:
    /* 2258  mov     cl,dl */
    CL = DL;
L225A:
    /* 225A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L225B:
    /* 225B  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L225D:
    /* 225D  mov     bx,ax */
    BX = AX;
L225F:
    /* 225F  add     bx,word ptr ds:[14ACh] */
    BX = add16(BX, rw(pDS, 0x14AC), 0);
L2263:
    /* 2263  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2264:
    /* 2264  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2266:
    /* 2266  mov     bp,ax */
    BP = AX;
L2268:
    /* 2268  add     bp,word ptr ds:[14B0h] */
    BP = add16(BP, rw(pDS, 0x14B0), 0);
L226C:
    /* 226C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L226D:
    /* 226D  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L226F:
    /* 226F  mov     cx,ax */
    CX = AX;
L2271:
    /* 2271  add     cx,word ptr ds:[14AEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x14AE));
L2275:
    /* 2275  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2278)) != 0) return c;
L2278:
    /* 2278  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L227A:
    /* 227A  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L227D:
    /* 227D  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L2280:
    /* 2280  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L2283:
    /* 2283  add     di,8 */
    DI = add16(DI, 0x8, 0);
L2286: /* L2286 */
    /* 2286  mov     cl,dl */
    CL = DL;
L2288:
    /* 2288  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2289:
    /* 2289  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L228B:
    /* 228B  mov     bx,ax */
    BX = AX;
L228D:
    /* 228D  add     bx,word ptr ds:[14ACh] */
    BX = add16(BX, rw(pDS, 0x14AC), 0);
L2291:
    /* 2291  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2292:
    /* 2292  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L2294:
    /* 2294  mov     bp,ax */
    BP = AX;
L2296:
    /* 2296  add     bp,word ptr ds:[14B0h] */
    BP = add16(BP, rw(pDS, 0x14B0), 0);
L229A:
    /* 229A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L229B:
    /* 229B  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L229D:
    /* 229D  mov     cx,ax */
    CX = AX;
L229F:
    /* 229F  add     cx,word ptr ds:[14AEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x14AE));
L22A3:
    /* 22A3  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x22A6)) != 0) return c;
L22A6:
    /* 22A6  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L22A8:
    /* 22A8  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L22AB:
    /* 22AB  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L22AE:
    /* 22AE  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L22B1:
    /* 22B1  mov     cl,dl */
    CL = DL;
L22B3:
    /* 22B3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22B4:
    /* 22B4  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L22B6:
    /* 22B6  mov     bx,ax */
    BX = AX;
L22B8:
    /* 22B8  add     bx,word ptr ds:[14ACh] */
    BX = add16(BX, rw(pDS, 0x14AC), 0);
L22BC:
    /* 22BC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22BD:
    /* 22BD  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L22BF:
    /* 22BF  mov     bp,ax */
    BP = AX;
L22C1:
    /* 22C1  add     bp,word ptr ds:[14B0h] */
    BP = add16(BP, rw(pDS, 0x14B0), 0);
L22C5:
    /* 22C5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22C6:
    /* 22C6  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L22C8:
    /* 22C8  mov     cx,ax */
    CX = AX;
L22CA:
    /* 22CA  add     cx,word ptr ds:[14AEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x14AE));
L22CE:
    /* 22CE  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x22D1)) != 0) return c;
L22D1:
    /* 22D1  mov     word ptr [di+8],bx */
    ww(pDS, DI + 0x8, BX);
L22D4:
    /* 22D4  mov     word ptr [di+0Ah],cx */
    ww(pDS, DI + 0xA, CX);
L22D7:
    /* 22D7  mov     word ptr [di+0Ch],bp */
    ww(pDS, DI + 0xC, BP);
L22DA:
    /* 22DA  mov     byte ptr [di+0Eh],al */
    wb(pDS, DI + 0xE, AL);
L22DD:
    /* 22DD  add     di,10h */
    DI = add16(DI, 0x10, 0);
L22E0:
    /* 22E0  dec     dh */
    DH = dec8(DH);
L22E2:
    /* 22E2  jne     L2286 */
    if (!ZF) goto L2286;
L22E4:
    /* 22E4  pop     si */
    SI = pop16();
L22E5:
    /* 22E5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22E6:
    /* 22E6  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L22E7:
    /* 22E7  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_22EB  (+22EB)
       Opcode 48h, operand: offset. Jumps: SI += offset. */
L22EB: /* _do_ijmp */
    /* 22EB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22EC:
    /* 22EC  add     si,ax */
    SI = add16(SI, AX, 0);
L22EE:
    /* 22EE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22EF:
    /* 22EF  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L22F0:
    /* 22F0  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_22F4  (+22F4)
       Opcode 32h, operand: address. Jumps to the program address held in the model variable. */
L22F4: /* _do_ijmp_i */
    /* 22F4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22F5:
    /* 22F5  mov     bx,ax */
    BX = AX;
L22F7:
    /* 22F7  mov     si,word ptr [bx] */
    SI = rw(pDS, BX);
L22F9:
    /* 22F9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L22FA:
    /* 22FA  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L22FB:
    /* 22FB  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_22FF  (+22FF)
       Opcode 4Ah, operands: dx, dy, dz. Moves the object origin by a relative offset (subtracts
       from 25E6h..25EAh) and calls self_modify. */
L22FF: /* _do_rorg */
    /* 22FF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2300:
    /* 2300  sub     word ptr ds:[25E6h],ax */
    ww(pDS, 0x25E6, (uint16_t)(rw(pDS, 0x25E6) - AX));
L2304:
    /* 2304  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2305:
    /* 2305  sub     word ptr ds:[25E8h],ax */
    ww(pDS, 0x25E8, (uint16_t)(rw(pDS, 0x25E8) - AX));
L2309:
    /* 2309  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L230A:
    /* 230A  sub     word ptr ds:[25EAh],ax */
    ww(pDS, 0x25EA, (uint16_t)(rw(pDS, 0x25EA) - AX));
L230E:
    /* 230E  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x2311)) != 0) return c;
L2311:
    /* 2311  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2312:
    /* 2312  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2313:
    /* 2313  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_2317  (+2317)
       Opcode 4Ch, operands: an (x, z, y) offset, point. Stores the rotated offset (no origin, no
       clip
       code) as a point, for do_addres and do_subres. */
L2317: /* _do_defdelta */
    /* 2317  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L231B:
    /* 231B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L231C:
    /* 231C  shl     ax,cl */
    AX = shl16(AX, CL);
L231E:
    /* 231E  mov     bx,ax */
    BX = AX;
L2320:
    /* 2320  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2321:
    /* 2321  shl     ax,cl */
    AX = shl16(AX, CL);
L2323:
    /* 2323  mov     bp,ax */
    BP = AX;
L2325:
    /* 2325  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2326:
    /* 2326  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2328:
    /* 2328  mov     cx,ax */
    CX = AX;
L232A:
    /* 232A  call    _mxmul */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3534), 0x232D)) != 0) return c;
L232D:
    /* 232D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L232E:
    /* 232E  mov     di,ax */
    DI = AX;
L2330:
    /* 2330  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L2334:
    /* 2334  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L2338:
    /* 2338  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L233C:
    /* 233C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L233D:
    /* 233D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L233E:
    /* 233E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_2342  (+2342)
       Opcode 8Ch, operands: point a, point b, destination. Destination = a + b, clip-coded. */
L2342: /* _do_addres */
    /* 2342  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2343:
    /* 2343  mov     bp,ax */
    BP = AX;
L2345:
    /* 2345  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2346:
    /* 2346  mov     di,ax */
    DI = AX;
L2348:
    /* 2348  mov     bx,word ptr ds:[bp+14D0h] */
    BX = rw(pDS, BP + 0x14D0);
L234D:
    /* 234D  add     bx,word ptr [di+14D0h] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x14D0));
L2351:
    /* 2351  mov     cx,word ptr ds:[bp+14D2h] */
    CX = rw(pDS, BP + 0x14D2);
L2356:
    /* 2356  add     cx,word ptr [di+14D2h] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x14D2));
L235A:
    /* 235A  mov     bp,word ptr ds:[bp+14D4h] */
    BP = rw(pDS, BP + 0x14D4);
L235F:
    /* 235F  add     bp,word ptr [di+14D4h] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x14D4));
L2363:
    /* 2363  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2364:
    /* 2364  mov     di,ax */
    DI = AX;
L2366:
    /* 2366  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2369)) != 0) return c;
L2369:
    /* 2369  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L236D:
    /* 236D  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L2371:
    /* 2371  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L2375:
    /* 2375  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L2379:
    /* 2379  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L237A:
    /* 237A  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L237B:
    /* 237B  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_237F  (+237F)
       Opcode C6h, operands: point a, point b, destination. Destination = a - b, clip-coded. */
L237F: /* _do_subres */
    /* 237F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2380:
    /* 2380  mov     bp,ax */
    BP = AX;
L2382:
    /* 2382  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2383:
    /* 2383  mov     di,ax */
    DI = AX;
L2385:
    /* 2385  mov     bx,word ptr ds:[bp+14D0h] */
    BX = rw(pDS, BP + 0x14D0);
L238A:
    /* 238A  sub     bx,word ptr [di+14D0h] */
    BX = (uint16_t)(BX - rw(pDS, DI + 0x14D0));
L238E:
    /* 238E  mov     cx,word ptr ds:[bp+14D2h] */
    CX = rw(pDS, BP + 0x14D2);
L2393:
    /* 2393  sub     cx,word ptr [di+14D2h] */
    CX = (uint16_t)(CX - rw(pDS, DI + 0x14D2));
L2397:
    /* 2397  mov     bp,word ptr ds:[bp+14D4h] */
    BP = rw(pDS, BP + 0x14D4);
L239C:
    /* 239C  sub     bp,word ptr [di+14D4h] */
    BP = (uint16_t)(BP - rw(pDS, DI + 0x14D4));
L23A0:
    /* 23A0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L23A1:
    /* 23A1  mov     di,ax */
    DI = AX;
L23A3:
    /* 23A3  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x23A6)) != 0) return c;
L23A6:
    /* 23A6  mov     word ptr [di+14D0h],bx */
    ww(pDS, DI + 0x14D0, BX);
L23AA:
    /* 23AA  mov     word ptr [di+14D2h],cx */
    ww(pDS, DI + 0x14D2, CX);
L23AE:
    /* 23AE  mov     word ptr [di+14D4h],bp */
    ww(pDS, DI + 0x14D4, BP);
L23B2:
    /* 23B2  mov     byte ptr [di+14D6h],al */
    wb(pDS, DI + 0x14D6, AL);
L23B6:
    /* 23B6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L23B7:
    /* 23B7  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L23B8:
    /* 23B8  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_23BC  (+23BC)
       Opcode BAh: do_ihcall with the angle taken from a model variable. */
L23BC: /* _do_ihcall_ind */
    /* 23BC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L23BD:
    /* 23BD  mov     bx,ax */
    BX = AX;
L23BF:
    /* 23BF  mov     bx,word ptr [bx] */
    BX = rw(pDS, BX);
L23C1:
    /* 23C1  jmp     short L23C6 */
    goto L23C6;

    /* seg004_0849_23C3  (+23C3)
       Opcode 50h, operands: angle, offset. Saves the matrix and origin, rotates the matrix by the
       heading angle (instance_head, which also rotates the origin), runs the sub-program at
       SI + offset, and restores both. The b and p variants below do the same for bank and pitch. */
L23C3: /* _do_ihcall */
    /* 23C3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L23C4:
    /* 23C4  mov     bx,ax */
    BX = AX;
L23C6: /* L23C6 */
    /* 23C6  push    word ptr ds:[14B2h] */
    push16(rw(pDS, 0x14B2));
L23CA:
    /* 23CA  push    word ptr ds:[14B8h] */
    push16(rw(pDS, 0x14B8));
L23CE:
    /* 23CE  push    word ptr ds:[14BEh] */
    push16(rw(pDS, 0x14BE));
L23D2:
    /* 23D2  push    word ptr ds:[14B4h] */
    push16(rw(pDS, 0x14B4));
L23D6:
    /* 23D6  push    word ptr ds:[14BAh] */
    push16(rw(pDS, 0x14BA));
L23DA:
    /* 23DA  push    word ptr ds:[14C0h] */
    push16(rw(pDS, 0x14C0));
L23DE:
    /* 23DE  push    word ptr ds:[14B6h] */
    push16(rw(pDS, 0x14B6));
L23E2:
    /* 23E2  push    word ptr ds:[14BCh] */
    push16(rw(pDS, 0x14BC));
L23E6:
    /* 23E6  push    word ptr ds:[14C2h] */
    push16(rw(pDS, 0x14C2));
L23EA:
    /* 23EA  push    word ptr ds:[25E6h] */
    push16(rw(pDS, 0x25E6));
L23EE:
    /* 23EE  push    word ptr ds:[25E8h] */
    push16(rw(pDS, 0x25E8));
L23F2:
    /* 23F2  push    word ptr ds:[25EAh] */
    push16(rw(pDS, 0x25EA));
L23F6:
    /* 23F6  call    _instance_head */
    if ((c = asm_call(ASM_JMP(0x065C, 0x368D), 0x23F9)) != 0) return c;
L23F9:
    /* 23F9  push    si */
    push16(SI);
L23FA:
    /* 23FA  call    _check_flat */
    if ((c = asm_call(ASM_JMP(0x065C, 0x340E), 0x23FD)) != 0) return c;
L23FD:
    /* 23FD  pop     si */
    SI = pop16();
L23FE:
    /* 23FE  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x2401)) != 0) return c;
L2401:
    /* 2401  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2402:
    /* 2402  push    si */
    push16(SI);
L2403:
    /* 2403  add     si,ax */
    SI = (uint16_t)(SI + AX);
L2405:
    /* 2405  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2406:
    /* 2406  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2407:
    /* 2407  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x240B)) != 0) return c;
L240B:
    /* 240B  pop     si */
    SI = pop16();
L240C:
    /* 240C  pop     word ptr ds:[25EAh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25EA, t_); }
L2410:
    /* 2410  pop     word ptr ds:[25E8h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E8, t_); }
L2414:
    /* 2414  pop     word ptr ds:[25E6h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E6, t_); }
L2418:
    /* 2418  pop     word ptr ds:[14C2h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14C2, t_); }
L241C:
    /* 241C  pop     word ptr ds:[14BCh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BC, t_); }
L2420:
    /* 2420  pop     word ptr ds:[14B6h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B6, t_); }
L2424:
    /* 2424  pop     word ptr ds:[14C0h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14C0, t_); }
L2428:
    /* 2428  pop     word ptr ds:[14BAh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BA, t_); }
L242C:
    /* 242C  pop     word ptr ds:[14B4h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B4, t_); }
L2430:
    /* 2430  pop     word ptr ds:[14BEh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BE, t_); }
L2434:
    /* 2434  pop     word ptr ds:[14B8h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B8, t_); }
L2438:
    /* 2438  pop     word ptr ds:[14B2h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B2, t_); }
L243C:
    /* 243C  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x243F)) != 0) return c;
L243F:
    /* 243F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2440:
    /* 2440  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2441:
    /* 2441  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_2445  (+2445)
       Opcode C4h: do_ibcall with the angle from a model variable. */
L2445: /* _do_ibcall_ind */
    /* 2445  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2446:
    /* 2446  mov     bx,ax */
    BX = AX;
L2448:
    /* 2448  mov     bx,word ptr [bx] */
    BX = rw(pDS, BX);
L244A:
    /* 244A  jmp     short L244F */
    goto L244F;

    /* seg004_0849_244C  (+244C)
       Opcode 70h: a sub-program rotated by a bank angle (instance_bank). */
L244C: /* _do_ibcall */
    /* 244C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L244D:
    /* 244D  mov     bx,ax */
    BX = AX;
L244F: /* L244F */
    /* 244F  push    word ptr ds:[14B2h] */
    push16(rw(pDS, 0x14B2));
L2453:
    /* 2453  push    word ptr ds:[14B8h] */
    push16(rw(pDS, 0x14B8));
L2457:
    /* 2457  push    word ptr ds:[14BEh] */
    push16(rw(pDS, 0x14BE));
L245B:
    /* 245B  push    word ptr ds:[14B4h] */
    push16(rw(pDS, 0x14B4));
L245F:
    /* 245F  push    word ptr ds:[14BAh] */
    push16(rw(pDS, 0x14BA));
L2463:
    /* 2463  push    word ptr ds:[14C0h] */
    push16(rw(pDS, 0x14C0));
L2467:
    /* 2467  push    word ptr ds:[14B6h] */
    push16(rw(pDS, 0x14B6));
L246B:
    /* 246B  push    word ptr ds:[14BCh] */
    push16(rw(pDS, 0x14BC));
L246F:
    /* 246F  push    word ptr ds:[14C2h] */
    push16(rw(pDS, 0x14C2));
L2473:
    /* 2473  push    word ptr ds:[25E6h] */
    push16(rw(pDS, 0x25E6));
L2477:
    /* 2477  push    word ptr ds:[25E8h] */
    push16(rw(pDS, 0x25E8));
L247B:
    /* 247B  push    word ptr ds:[25EAh] */
    push16(rw(pDS, 0x25EA));
L247F:
    /* 247F  call    _instance_bank */
    if ((c = asm_call(ASM_JMP(0x065C, 0x395D), 0x2482)) != 0) return c;
L2482:
    /* 2482  push    si */
    push16(SI);
L2483:
    /* 2483  call    _check_flat */
    if ((c = asm_call(ASM_JMP(0x065C, 0x340E), 0x2486)) != 0) return c;
L2486:
    /* 2486  pop     si */
    SI = pop16();
L2487:
    /* 2487  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x248A)) != 0) return c;
L248A:
    /* 248A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L248B:
    /* 248B  push    si */
    push16(SI);
L248C:
    /* 248C  add     si,ax */
    SI = (uint16_t)(SI + AX);
L248E:
    /* 248E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L248F:
    /* 248F  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2490:
    /* 2490  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x2494)) != 0) return c;
L2494:
    /* 2494  pop     si */
    SI = pop16();
L2495:
    /* 2495  pop     word ptr ds:[25EAh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25EA, t_); }
L2499:
    /* 2499  pop     word ptr ds:[25E8h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E8, t_); }
L249D:
    /* 249D  pop     word ptr ds:[25E6h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E6, t_); }
L24A1:
    /* 24A1  pop     word ptr ds:[14C2h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14C2, t_); }
L24A5:
    /* 24A5  pop     word ptr ds:[14BCh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BC, t_); }
L24A9:
    /* 24A9  pop     word ptr ds:[14B6h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B6, t_); }
L24AD:
    /* 24AD  pop     word ptr ds:[14C0h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14C0, t_); }
L24B1:
    /* 24B1  pop     word ptr ds:[14BAh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BA, t_); }
L24B5:
    /* 24B5  pop     word ptr ds:[14B4h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B4, t_); }
L24B9:
    /* 24B9  pop     word ptr ds:[14BEh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BE, t_); }
L24BD:
    /* 24BD  pop     word ptr ds:[14B8h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B8, t_); }
L24C1:
    /* 24C1  pop     word ptr ds:[14B2h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B2, t_); }
L24C5:
    /* 24C5  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x24C8)) != 0) return c;
L24C8:
    /* 24C8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L24C9:
    /* 24C9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L24CA:
    /* 24CA  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_24CE  (+24CE)
       Opcode C2h: do_ipcall with the angle from a model variable. */
L24CE: /* _do_ipcall_ind */
    /* 24CE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L24CF:
    /* 24CF  mov     bx,ax */
    BX = AX;
L24D1:
    /* 24D1  mov     bx,word ptr [bx] */
    BX = rw(pDS, BX);
L24D3:
    /* 24D3  jmp     short L24D8 */
    goto L24D8;

    /* seg004_0849_24D5  (+24D5)
       Opcode 72h: a sub-program rotated by a pitch angle (instance_pitch). */
L24D5: /* _do_ipcall */
    /* 24D5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L24D6:
    /* 24D6  mov     bx,ax */
    BX = AX;
L24D8: /* L24D8 */
    /* 24D8  push    word ptr ds:[14B2h] */
    push16(rw(pDS, 0x14B2));
L24DC:
    /* 24DC  push    word ptr ds:[14B8h] */
    push16(rw(pDS, 0x14B8));
L24E0:
    /* 24E0  push    word ptr ds:[14BEh] */
    push16(rw(pDS, 0x14BE));
L24E4:
    /* 24E4  push    word ptr ds:[14B4h] */
    push16(rw(pDS, 0x14B4));
L24E8:
    /* 24E8  push    word ptr ds:[14BAh] */
    push16(rw(pDS, 0x14BA));
L24EC:
    /* 24EC  push    word ptr ds:[14C0h] */
    push16(rw(pDS, 0x14C0));
L24F0:
    /* 24F0  push    word ptr ds:[14B6h] */
    push16(rw(pDS, 0x14B6));
L24F4:
    /* 24F4  push    word ptr ds:[14BCh] */
    push16(rw(pDS, 0x14BC));
L24F8:
    /* 24F8  push    word ptr ds:[14C2h] */
    push16(rw(pDS, 0x14C2));
L24FC:
    /* 24FC  push    word ptr ds:[25E6h] */
    push16(rw(pDS, 0x25E6));
L2500:
    /* 2500  push    word ptr ds:[25E8h] */
    push16(rw(pDS, 0x25E8));
L2504:
    /* 2504  push    word ptr ds:[25EAh] */
    push16(rw(pDS, 0x25EA));
L2508:
    /* 2508  call    _instance_pitch */
    if ((c = asm_call(ASM_JMP(0x065C, 0x37F5), 0x250B)) != 0) return c;
L250B:
    /* 250B  push    si */
    push16(SI);
L250C:
    /* 250C  call    _check_flat */
    if ((c = asm_call(ASM_JMP(0x065C, 0x340E), 0x250F)) != 0) return c;
L250F:
    /* 250F  pop     si */
    SI = pop16();
L2510:
    /* 2510  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x2513)) != 0) return c;
L2513:
    /* 2513  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2514:
    /* 2514  push    si */
    push16(SI);
L2515:
    /* 2515  add     si,ax */
    SI = (uint16_t)(SI + AX);
L2517:
    /* 2517  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2518:
    /* 2518  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2519:
    /* 2519  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, BX + 0x24F4)), 0x251D)) != 0) return c;
L251D:
    /* 251D  pop     si */
    SI = pop16();
L251E:
    /* 251E  pop     word ptr ds:[25EAh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25EA, t_); }
L2522:
    /* 2522  pop     word ptr ds:[25E8h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E8, t_); }
L2526:
    /* 2526  pop     word ptr ds:[25E6h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E6, t_); }
L252A:
    /* 252A  pop     word ptr ds:[14C2h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14C2, t_); }
L252E:
    /* 252E  pop     word ptr ds:[14BCh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BC, t_); }
L2532:
    /* 2532  pop     word ptr ds:[14B6h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B6, t_); }
L2536:
    /* 2536  pop     word ptr ds:[14C0h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14C0, t_); }
L253A:
    /* 253A  pop     word ptr ds:[14BAh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BA, t_); }
L253E:
    /* 253E  pop     word ptr ds:[14B4h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B4, t_); }
L2542:
    /* 2542  pop     word ptr ds:[14BEh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14BE, t_); }
L2546:
    /* 2546  pop     word ptr ds:[14B8h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B8, t_); }
L254A:
    /* 254A  pop     word ptr ds:[14B2h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x14B2, t_); }
L254E:
    /* 254E  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x2551)) != 0) return c;
L2551:
    /* 2551  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2552:
    /* 2552  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2553:
    /* 2553  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_2557  (+2557)
       Opcode 54h: pushes the object origin and the coordinate shift on the stack (the interpreter's
       own stack, SS = dseg062_62a6). */
L2557: /* _do_pushc */
    /* 2557  push    word ptr ds:[25E6h] */
    push16(rw(pDS, 0x25E6));
L255B:
    /* 255B  push    word ptr ds:[25E8h] */
    push16(rw(pDS, 0x25E8));
L255F:
    /* 255F  push    word ptr ds:[25EAh] */
    push16(rw(pDS, 0x25EA));
L2563:
    /* 2563  push    word ptr ds:[25E0h] */
    push16(rw(pDS, 0x25E0));
L2567:
    /* 2567  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2568:
    /* 2568  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2569:
    /* 2569  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_256D  (+256D)
       Opcode 56h: pops what do_pushc pushed and calls self_modify. */
L256D: /* _do_popc */
    /* 256D  pop     word ptr ds:[25E0h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E0, t_); }
L2571:
    /* 2571  pop     word ptr ds:[25EAh] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25EA, t_); }
L2575:
    /* 2575  pop     word ptr ds:[25E8h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E8, t_); }
L2579:
    /* 2579  pop     word ptr ds:[25E6h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x25E6, t_); }
L257D:
    /* 257D  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x2580)) != 0) return c;
L2580:
    /* 2580  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2581:
    /* 2581  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2582:
    /* 2582  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_2586  (+2586)
       Opcode 5Eh: do_jnorm with no x term. */
L2586: /* _do_jnorm_x0 */
    /* 2586  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2587:
    /* 2587  mov     bx,ax */
    BX = AX;
L2589:
    /* 2589  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L258A:
    /* 258A  mov     di,ax */
    DI = AX;
L258C:
    /* 258C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L258D:
    /* 258D  add     ax,word ptr ds:[25E8h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E8));
L2591:
    /* 2591  imul    di */
    imul16(DI);
L2593:
    /* 2593  mov     bp,ax */
    BP = AX;
L2595:
    /* 2595  mov     cx,dx */
    CX = DX;
L2597:
    /* 2597  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2598:
    /* 2598  mov     di,ax */
    DI = AX;
L259A:
    /* 259A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L259B:
    /* 259B  add     ax,word ptr ds:[25EAh] */
    AX = (uint16_t)(AX + rw(pDS, 0x25EA));
L259F:
    /* 259F  imul    di */
    imul16(DI);
L25A1:
    /* 25A1  add     bp,ax */
    BP = add16(BP, AX, 0);
L25A3:
    /* 25A3  adc     cx,dx */
    CX = add16(CX, DX, CF);
L25A5:
    /* 25A5  jns     short L25A9 */
    if (!SF) goto L25A9;
L25A7:
    /* 25A7  add     si,bx */
    SI = add16(SI, BX, 0);
L25A9: /* L25A9 */
    /* 25A9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25AA:
    /* 25AA  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L25AB:
    /* 25AB  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_25AF  (+25AF)
       Opcode 60h: do_jnorm with no y term. */
L25AF: /* _do_jnorm_y0 */
    /* 25AF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25B0:
    /* 25B0  mov     bx,ax */
    BX = AX;
L25B2:
    /* 25B2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25B3:
    /* 25B3  mov     cx,ax */
    CX = AX;
L25B5:
    /* 25B5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25B6:
    /* 25B6  add     ax,word ptr ds:[25E6h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E6));
L25BA:
    /* 25BA  imul    cx */
    imul16(CX);
L25BC:
    /* 25BC  mov     cx,dx */
    CX = DX;
L25BE:
    /* 25BE  mov     bp,ax */
    BP = AX;
L25C0:
    /* 25C0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25C1:
    /* 25C1  mov     di,ax */
    DI = AX;
L25C3:
    /* 25C3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25C4:
    /* 25C4  add     ax,word ptr ds:[25EAh] */
    AX = (uint16_t)(AX + rw(pDS, 0x25EA));
L25C8:
    /* 25C8  imul    di */
    imul16(DI);
L25CA:
    /* 25CA  add     bp,ax */
    BP = add16(BP, AX, 0);
L25CC:
    /* 25CC  adc     cx,dx */
    CX = add16(CX, DX, CF);
L25CE:
    /* 25CE  jns     short L25D2 */
    if (!SF) goto L25D2;
L25D0:
    /* 25D0  add     si,bx */
    SI = add16(SI, BX, 0);
L25D2: /* L25D2 */
    /* 25D2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25D3:
    /* 25D3  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L25D4:
    /* 25D4  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_25D8  (+25D8)
       Opcode 62h: do_jnorm with no z term. */
L25D8: /* _do_jnorm_z0 */
    /* 25D8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25D9:
    /* 25D9  mov     bx,ax */
    BX = AX;
L25DB:
    /* 25DB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25DC:
    /* 25DC  mov     cx,ax */
    CX = AX;
L25DE:
    /* 25DE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25DF:
    /* 25DF  add     ax,word ptr ds:[25E6h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E6));
L25E3:
    /* 25E3  imul    cx */
    imul16(CX);
L25E5:
    /* 25E5  mov     cx,dx */
    CX = DX;
L25E7:
    /* 25E7  mov     bp,ax */
    BP = AX;
L25E9:
    /* 25E9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25EA:
    /* 25EA  mov     di,ax */
    DI = AX;
L25EC:
    /* 25EC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25ED:
    /* 25ED  add     ax,word ptr ds:[25E8h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E8));
L25F1:
    /* 25F1  imul    di */
    imul16(DI);
L25F3:
    /* 25F3  add     bp,ax */
    BP = add16(BP, AX, 0);
L25F5:
    /* 25F5  adc     cx,dx */
    CX = add16(CX, DX, CF);
L25F7:
    /* 25F7  jns     short L25FB */
    if (!SF) goto L25FB;
L25F9:
    /* 25F9  add     si,bx */
    SI = add16(SI, BX, 0);
L25FB: /* L25FB */
    /* 25FB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L25FC:
    /* 25FC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L25FD:
    /* 25FD  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_2601  (+2601)
       Opcode 64h, operands: skip, nx, px. do_jnorm for a normal along x alone: only the sign of
       (px + origin x) against nx is tested. */
L2601: /* _do_jnorm_yz0 */
    /* 2601  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2602:
    /* 2602  mov     bx,ax */
    BX = AX;
L2604:
    /* 2604  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2605:
    /* 2605  mov     cx,ax */
    CX = AX;
L2607:
    /* 2607  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2608:
    /* 2608  add     ax,word ptr ds:[25E6h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E6));
L260C:
    /* 260C  xor     ax,cx */
    AX = logic16((uint16_t)(AX ^ CX));
L260E:
    /* 260E  jns     short L2612 */
    if (!SF) goto L2612;
L2610:
    /* 2610  add     si,bx */
    SI = add16(SI, BX, 0);
L2612: /* L2612 */
    /* 2612  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2613:
    /* 2613  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2614:
    /* 2614  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_2618  (+2618)
       Opcode 66h: the same for a normal along y. */
L2618: /* _do_jnorm_xz0 */
    /* 2618  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2619:
    /* 2619  mov     bx,ax */
    BX = AX;
L261B:
    /* 261B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L261C:
    /* 261C  mov     di,ax */
    DI = AX;
L261E:
    /* 261E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L261F:
    /* 261F  add     ax,word ptr ds:[25E8h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E8));
L2623:
    /* 2623  xor     ax,di */
    AX = logic16((uint16_t)(AX ^ DI));
L2625:
    /* 2625  jns     short L2629 */
    if (!SF) goto L2629;
L2627:
    /* 2627  add     si,bx */
    SI = add16(SI, BX, 0);
L2629: /* L2629 */
    /* 2629  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L262A:
    /* 262A  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L262B:
    /* 262B  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_262F  (+262F)
       Opcode 68h: the same for a normal along z. */
L262F: /* _do_jnorm_xy0 */
    /* 262F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2630:
    /* 2630  mov     bx,ax */
    BX = AX;
L2632:
    /* 2632  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2633:
    /* 2633  mov     di,ax */
    DI = AX;
L2635:
    /* 2635  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2636:
    /* 2636  add     ax,word ptr ds:[25EAh] */
    AX = (uint16_t)(AX + rw(pDS, 0x25EA));
L263A:
    /* 263A  xor     ax,di */
    AX = logic16((uint16_t)(AX ^ DI));
L263C:
    /* 263C  jns     short L2640 */
    if (!SF) goto L2640;
L263E:
    /* 263E  add     si,bx */
    SI = add16(SI, BX, 0);
L2640: /* L2640 */
    /* 2640  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2641:
    /* 2641  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2642:
    /* 2642  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_2646  (+2646)
       Opcode 58h, operands: skip, then (n, p) for x, y, z. Back-face test: the same sum as
       do_sortnorm; when it is negative (the eye on the back of the plane) skips `skip` bytes,
       normally the face that follows. (System Shock: do_jnorm.) The most used opcode in UW1's
       models after polygons and x_rel (model_opcodes.tsv). */
L2646: /* _do_jnorm */
    /* 2646  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2647:
    /* 2647  mov     bx,ax */
    BX = AX;
L2649:
    /* 2649  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L264A:
    /* 264A  mov     cx,ax */
    CX = AX;
L264C:
    /* 264C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L264D:
    /* 264D  add     ax,word ptr ds:[25E6h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E6));
L2651:
    /* 2651  imul    cx */
    imul16(CX);
L2653:
    /* 2653  mov     cx,dx */
    CX = DX;
L2655:
    /* 2655  mov     bp,ax */
    BP = AX;
L2657:
    /* 2657  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2658:
    /* 2658  mov     di,ax */
    DI = AX;
L265A:
    /* 265A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L265B:
    /* 265B  add     ax,word ptr ds:[25E8h] */
    AX = (uint16_t)(AX + rw(pDS, 0x25E8));
L265F:
    /* 265F  imul    di */
    imul16(DI);
L2661:
    /* 2661  add     bp,ax */
    BP = add16(BP, AX, 0);
L2663:
    /* 2663  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L2665:
    /* 2665  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2666:
    /* 2666  mov     di,ax */
    DI = AX;
L2668:
    /* 2668  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2669:
    /* 2669  add     ax,word ptr ds:[25EAh] */
    AX = (uint16_t)(AX + rw(pDS, 0x25EA));
L266D:
    /* 266D  imul    di */
    imul16(DI);
L266F:
    /* 266F  add     bp,ax */
    BP = add16(BP, AX, 0);
L2671:
    /* 2671  adc     cx,dx */
    CX = add16(CX, DX, CF);
L2673:
    /* 2673  jns     short L2677 */
    if (!SF) goto L2677;
L2675:
    /* 2675  add     si,bx */
    SI = add16(SI, BX, 0);
L2677: /* L2677 */
    /* 2677  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2678:
    /* 2678  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2679:
    /* 2679  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_267D  (+267D)
       Recovery for do_closure's projection (stored at ss:[5A5h] in clip_needed): drops the int 0
       frame (sti; add sp,6) and falls into do_3d_clip. */
L267D: /* _clip_overflow */
    /* 267D  sti */
    ;
L267E:
    /* 267E  add     sp,6 */
    SP = (uint16_t)(SP + 0x6);

    /* seg004_0849_2681  (+2681)
       Clips the polygon in the vertex buffer in 3D against each plane its codes OR shows it
       crossing (right, left, top, bottom), stopping as soon as the codes AND to nonzero (nothing
       left), and then projects and draws it (back to do_closure's L17F0). Restores DS and ES to
       seg052_519C when it gives up. */
L2681: /* _do_3d_clip */
    /* 2681  mov     word ptr ss:[5A5h],offset _overflow_handler_reg */
    ww(pSS, 0x5A5, 0x3C91);
L2688:
    /* 2688  mov     ax,ds */
    AX = asm_ds;
L268A:
    /* 268A  mov     es,ax */
    SET_ES(AX);
L268C:
    /* 268C  test    byte ptr ds:[14CAh],2 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x2));
L2691:
    /* 2691  je      short L269D */
    if (ZF) goto L269D;
L2693:
    /* 2693  call    _clip_right */
    if ((c = asm_call(ASM_JMP(0x065C, 0x28FA), 0x2696)) != 0) return c;
L2696:
    /* 2696  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L269B:
    /* 269B  jne     short L26D3 */
    if (!ZF) goto L26D3;
L269D: /* L269D */
    /* 269D  test    byte ptr ds:[14CAh],1 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x1));
L26A2:
    /* 26A2  je      short L26AE */
    if (ZF) goto L26AE;
L26A4:
    /* 26A4  call    _clip_left */
    if ((c = asm_call(ASM_JMP(0x065C, 0x2A08), 0x26A7)) != 0) return c;
L26A7:
    /* 26A7  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L26AC:
    /* 26AC  jne     short L26D3 */
    if (!ZF) goto L26D3;
L26AE: /* L26AE */
    /* 26AE  test    byte ptr ds:[14CAh],4 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x4));
L26B3:
    /* 26B3  je      short L26BF */
    if (ZF) goto L26BF;
L26B5:
    /* 26B5  call    _clip_top */
    if ((c = asm_call(ASM_JMP(0x065C, 0x26E1), 0x26B8)) != 0) return c;
L26B8:
    /* 26B8  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L26BD:
    /* 26BD  jne     short L26D3 */
    if (!ZF) goto L26D3;
L26BF: /* L26BF */
    /* 26BF  test    byte ptr ds:[14CAh],8 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x8));
L26C4:
    /* 26C4  je      short L26D0 */
    if (ZF) goto L26D0;
L26C6:
    /* 26C6  call    _clip_bot */
    if ((c = asm_call(ASM_JMP(0x065C, 0x27EA), 0x26C9)) != 0) return c;
L26C9:
    /* 26C9  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L26CE:
    /* 26CE  jne     short L26D3 */
    if (!ZF) goto L26D3;
L26D0: /* L26D0 */
    /* 26D0  jmp     L17F0 */
    goto L17F0;
L26D3: /* L26D3 */
    /* 26D3  pop     si */
    SI = pop16();
L26D4:
    /* 26D4  mov     ax,seg seg052_519C */
    AX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L26D7:
    /* 26D7  mov     ds,ax */
    SET_DS(AX);
L26D9:
    /* 26D9  mov     es,ax */
    SET_ES(AX);
L26DB:
    /* 26DB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L26DC:
    /* 26DC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L26DD:
    /* 26DD  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_26E1  (+26E1)
       Clips the vertex buffer against the y = z plane (code bit 4): copies the first vertices past
       the end to close the loop, then walks the edges writing the surviving vertices and the
       intersections into the other buffer (0B1Ah or 1BAh), recomputing the codes OR and AND.
       The intersection uses fixed-point interpolation with rounding (sar ax,1; adc). The other
       three are the same for the other planes. */
L26E1: /* _clip_top */
    /* 26E1  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L26E6:
    /* 26E6  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L26EB:
    /* 26EB  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L26EF:
    /* 26EF  mov     di,word ptr ds:[1B6h] */
    DI = rw(pDS, 0x1B6);
L26F3:
    /* 26F3  mov     cx,8 */
    CX = 0x8;
L26F6:
    /* 26F6  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L26F8:
    /* 26F8  add     word ptr ds:[1B6h],8 */
    ww(pDS, 0x1B6, (uint16_t)(rw(pDS, 0x1B6) + 0x8));
L26FD:
    /* 26FD  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L2701:
    /* 2701  mov     di,0B1Ah */
    DI = 0xB1A;
L2704:
    /* 2704  cmp     si,1BAh */
    sub16(SI, 0x1BA, 0);
L2708:
    /* 2708  je      short L270D */
    if (ZF) goto L270D;
L270A:
    /* 270A  mov     di,1BAh */
    DI = 0x1BA;
L270D: /* L270D */
    /* 270D  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L2711: /* L2711 */
    /* 2711  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L2714: /* L2714 */
    /* 2714  cmp     si,word ptr ds:[1B6h] */
    sub16(SI, rw(pDS, 0x1B6), 0);
L2718:
    /* 2718  jne     short L271D */
    if (!ZF) goto L271D;
L271A:
    /* 271A  jmp     L27E5 */
    goto L27E5;
L271D: /* L271D */
    /* 271D  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L2720:
    /* 2720  test    al,4 */
    logic8((uint8_t)(AL & 0x4));
L2722:
    /* 2722  jne     short L2733 */
    if (!ZF) goto L2733;
L2724:
    /* 2724  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L2728:
    /* 2728  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L272C:
    /* 272C  mov     cx,4 */
    CX = 0x4;
L272F:
    /* 272F  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L2731:
    /* 2731  jmp     L2714 */
    goto L2714;
L2733: /* L2733 */
    /* 2733  test    byte ptr [si-2],4 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x4));
L2737:
    /* 2737  jne     short L278B */
    if (!ZF) goto L278B;
L2739:
    /* 2739  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L273C:
    /* 273C  sub     bp,word ptr [si-6] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0xFFFA));
L273F:
    /* 273F  mov     cx,bp */
    CX = BP;
L2741:
    /* 2741  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L2744:
    /* 2744  add     cx,word ptr [si+2] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x2));
L2747:
    /* 2747  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L2749:
    /* 2749  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L274C:
    /* 274C  imul    bp */
    imul16(BP);
L274E:
    /* 274E  shl     ax,1 */
    AX = shl16(AX, 1);
L2750:
    /* 2750  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2752:
    /* 2752  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x2752, 2)) != 0) return c;
L2754:
    /* 2754  sar     ax,1 */
    AX = sar16(AX, 1);
L2756:
    /* 2756  jae     short L275B */
    if (!CF) goto L275B;
L2758:
    /* 2758  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L2759:
    /* 2759  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L275B: /* L275B */
    /* 275B  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L275E:
    /* 275E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L275F:
    /* 275F  mov     bx,ax */
    BX = AX;
L2761:
    /* 2761  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L2764:
    /* 2764  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L2767:
    /* 2767  imul    bp */
    imul16(BP);
L2769:
    /* 2769  shl     ax,1 */
    AX = shl16(AX, 1);
L276B:
    /* 276B  rcl     dx,1 */
    DX = rcl16(DX, 1);
L276D:
    /* 276D  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x276D, 2)) != 0) return c;
L276F:
    /* 276F  sar     ax,1 */
    AX = sar16(AX, 1);
L2771:
    /* 2771  jae     short L2776 */
    if (!CF) goto L2776;
L2773:
    /* 2773  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L2774:
    /* 2774  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L2776: /* L2776 */
    /* 2776  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L2779:
    /* 2779  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L277A:
    /* 277A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L277B:
    /* 277B  mov     cx,ax */
    CX = AX;
L277D:
    /* 277D  mov     bp,ax */
    BP = AX;
L277F:
    /* 277F  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2782)) != 0) return c;
L2782:
    /* 2782  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2783:
    /* 2783  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L2787:
    /* 2787  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L278B: /* L278B */
    /* 278B  test    byte ptr [si+0Eh],4 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x4));
L278F:
    /* 278F  jne     L2711 */
    if (!ZF) goto L2711;
L2791:
    /* 2791  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L2794:
    /* 2794  sub     bp,word ptr [si+2] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0x2));
L2797:
    /* 2797  mov     cx,bp */
    CX = BP;
L2799:
    /* 2799  add     cx,word ptr [si+0Ah] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0xA));
L279C:
    /* 279C  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L279F:
    /* 279F  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L27A2:
    /* 27A2  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L27A4:
    /* 27A4  imul    bp */
    imul16(BP);
L27A6:
    /* 27A6  shl     ax,1 */
    AX = shl16(AX, 1);
L27A8:
    /* 27A8  rcl     dx,1 */
    DX = rcl16(DX, 1);
L27AA:
    /* 27AA  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x27AA, 2)) != 0) return c;
L27AC:
    /* 27AC  sar     ax,1 */
    AX = sar16(AX, 1);
L27AE:
    /* 27AE  jae     short L27B3 */
    if (!CF) goto L27B3;
L27B0:
    /* 27B0  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L27B1:
    /* 27B1  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L27B3: /* L27B3 */
    /* 27B3  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L27B5:
    /* 27B5  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L27B6:
    /* 27B6  mov     bx,ax */
    BX = AX;
L27B8:
    /* 27B8  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L27BB:
    /* 27BB  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L27BE:
    /* 27BE  imul    bp */
    imul16(BP);
L27C0:
    /* 27C0  shl     ax,1 */
    AX = shl16(AX, 1);
L27C2:
    /* 27C2  rcl     dx,1 */
    DX = rcl16(DX, 1);
L27C4:
    /* 27C4  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x27C4, 2)) != 0) return c;
L27C6:
    /* 27C6  sar     ax,1 */
    AX = sar16(AX, 1);
L27C8:
    /* 27C8  jae     short L27CD */
    if (!CF) goto L27CD;
L27CA:
    /* 27CA  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L27CB:
    /* 27CB  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L27CD: /* L27CD */
    /* 27CD  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L27D0:
    /* 27D0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L27D1:
    /* 27D1  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L27D2:
    /* 27D2  mov     cx,ax */
    CX = AX;
L27D4:
    /* 27D4  mov     bp,ax */
    BP = AX;
L27D6:
    /* 27D6  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x27D9)) != 0) return c;
L27D9:
    /* 27D9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L27DA:
    /* 27DA  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L27DE:
    /* 27DE  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L27E2:
    /* 27E2  jmp     L2711 */
    goto L2711;
L27E5: /* L27E5 */
    /* 27E5  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L27E9:
    /* 27E9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_27EA  (+27EA)
       Clips against y = -z (code bit 8). See clip_top. */
L27EA: /* _clip_bot */
    /* 27EA  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L27EF:
    /* 27EF  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L27F4:
    /* 27F4  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L27F8:
    /* 27F8  mov     di,word ptr ds:[1B6h] */
    DI = rw(pDS, 0x1B6);
L27FC:
    /* 27FC  mov     cx,8 */
    CX = 0x8;
L27FF:
    /* 27FF  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L2801:
    /* 2801  add     word ptr ds:[1B6h],8 */
    ww(pDS, 0x1B6, (uint16_t)(rw(pDS, 0x1B6) + 0x8));
L2806:
    /* 2806  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L280A:
    /* 280A  mov     di,0B1Ah */
    DI = 0xB1A;
L280D:
    /* 280D  cmp     si,1BAh */
    sub16(SI, 0x1BA, 0);
L2811:
    /* 2811  je      short L2816 */
    if (ZF) goto L2816;
L2813:
    /* 2813  mov     di,1BAh */
    DI = 0x1BA;
L2816: /* L2816 */
    /* 2816  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L281A: /* L281A */
    /* 281A  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L281D: /* L281D */
    /* 281D  cmp     si,word ptr ds:[1B6h] */
    sub16(SI, rw(pDS, 0x1B6), 0);
L2821:
    /* 2821  jne     short L2826 */
    if (!ZF) goto L2826;
L2823:
    /* 2823  jmp     L28F5 */
    goto L28F5;
L2826: /* L2826 */
    /* 2826  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L2829:
    /* 2829  test    al,8 */
    logic8((uint8_t)(AL & 0x8));
L282B:
    /* 282B  jne     short L283C */
    if (!ZF) goto L283C;
L282D:
    /* 282D  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L2831:
    /* 2831  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L2835:
    /* 2835  mov     cx,4 */
    CX = 0x4;
L2838:
    /* 2838  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L283A:
    /* 283A  jmp     L281D */
    goto L281D;
L283C: /* L283C */
    /* 283C  test    byte ptr [si-2],8 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x8));
L2840:
    /* 2840  jne     short L2896 */
    if (!ZF) goto L2896;
L2842:
    /* 2842  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L2845:
    /* 2845  add     bp,word ptr [si-6] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0xFFFA));
L2848:
    /* 2848  mov     cx,bp */
    CX = BP;
L284A:
    /* 284A  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L284D:
    /* 284D  sub     cx,word ptr [si+2] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x2));
L2850:
    /* 2850  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L2852:
    /* 2852  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L2855:
    /* 2855  imul    bp */
    imul16(BP);
L2857:
    /* 2857  shl     ax,1 */
    AX = shl16(AX, 1);
L2859:
    /* 2859  rcl     dx,1 */
    DX = rcl16(DX, 1);
L285B:
    /* 285B  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x285B, 2)) != 0) return c;
L285D:
    /* 285D  sar     ax,1 */
    AX = sar16(AX, 1);
L285F:
    /* 285F  jae     short L2864 */
    if (!CF) goto L2864;
L2861:
    /* 2861  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L2862:
    /* 2862  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L2864: /* L2864 */
    /* 2864  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L2867:
    /* 2867  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2868:
    /* 2868  mov     bx,ax */
    BX = AX;
L286A:
    /* 286A  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L286D:
    /* 286D  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L2870:
    /* 2870  imul    bp */
    imul16(BP);
L2872:
    /* 2872  shl     ax,1 */
    AX = shl16(AX, 1);
L2874:
    /* 2874  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2876:
    /* 2876  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x2876, 2)) != 0) return c;
L2878:
    /* 2878  sar     ax,1 */
    AX = sar16(AX, 1);
L287A:
    /* 287A  jae     short L287F */
    if (!CF) goto L287F;
L287C:
    /* 287C  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L287D:
    /* 287D  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L287F: /* L287F */
    /* 287F  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L2882:
    /* 2882  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2883:
    /* 2883  mov     cx,ax */
    CX = AX;
L2885:
    /* 2885  neg     ax */
    AX = (uint16_t)-AX;
L2887:
    /* 2887  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2888:
    /* 2888  mov     bp,ax */
    BP = AX;
L288A:
    /* 288A  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x288D)) != 0) return c;
L288D:
    /* 288D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L288E:
    /* 288E  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L2892:
    /* 2892  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L2896: /* L2896 */
    /* 2896  test    byte ptr [si+0Eh],8 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x8));
L289A:
    /* 289A  je      short L289F */
    if (ZF) goto L289F;
L289C:
    /* 289C  jmp     L281A */
    goto L281A;
L289F: /* L289F */
    /* 289F  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L28A2:
    /* 28A2  add     bp,word ptr [si+2] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0x2));
L28A5:
    /* 28A5  mov     cx,bp */
    CX = BP;
L28A7:
    /* 28A7  sub     cx,word ptr [si+0Ah] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xA));
L28AA:
    /* 28AA  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L28AD:
    /* 28AD  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L28B0:
    /* 28B0  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L28B2:
    /* 28B2  imul    bp */
    imul16(BP);
L28B4:
    /* 28B4  shl     ax,1 */
    AX = shl16(AX, 1);
L28B6:
    /* 28B6  rcl     dx,1 */
    DX = rcl16(DX, 1);
L28B8:
    /* 28B8  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x28B8, 2)) != 0) return c;
L28BA:
    /* 28BA  sar     ax,1 */
    AX = sar16(AX, 1);
L28BC:
    /* 28BC  jae     short L28C1 */
    if (!CF) goto L28C1;
L28BE:
    /* 28BE  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L28BF:
    /* 28BF  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L28C1: /* L28C1 */
    /* 28C1  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L28C3:
    /* 28C3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L28C4:
    /* 28C4  mov     bx,ax */
    BX = AX;
L28C6:
    /* 28C6  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L28C9:
    /* 28C9  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L28CC:
    /* 28CC  imul    bp */
    imul16(BP);
L28CE:
    /* 28CE  shl     ax,1 */
    AX = shl16(AX, 1);
L28D0:
    /* 28D0  rcl     dx,1 */
    DX = rcl16(DX, 1);
L28D2:
    /* 28D2  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x28D2, 2)) != 0) return c;
L28D4:
    /* 28D4  sar     ax,1 */
    AX = sar16(AX, 1);
L28D6:
    /* 28D6  jae     short L28DB */
    if (!CF) goto L28DB;
L28D8:
    /* 28D8  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L28D9:
    /* 28D9  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L28DB: /* L28DB */
    /* 28DB  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L28DE:
    /* 28DE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L28DF:
    /* 28DF  mov     cx,ax */
    CX = AX;
L28E1:
    /* 28E1  neg     ax */
    AX = (uint16_t)-AX;
L28E3:
    /* 28E3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L28E4:
    /* 28E4  mov     bp,ax */
    BP = AX;
L28E6:
    /* 28E6  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x28E9)) != 0) return c;
L28E9:
    /* 28E9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L28EA:
    /* 28EA  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L28EE:
    /* 28EE  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L28F2:
    /* 28F2  jmp     L281A */
    goto L281A;
L28F5: /* L28F5 */
    /* 28F5  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L28F9:
    /* 28F9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_28FA  (+28FA)
       Clips against x = z (code bit 2). See clip_top. */
L28FA: /* _clip_right */
    /* 28FA  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L28FF:
    /* 28FF  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L2904:
    /* 2904  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L2908:
    /* 2908  mov     di,word ptr ds:[1B6h] */
    DI = rw(pDS, 0x1B6);
L290C:
    /* 290C  mov     cx,8 */
    CX = 0x8;
L290F:
    /* 290F  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L2911:
    /* 2911  add     word ptr ds:[1B6h],8 */
    ww(pDS, 0x1B6, (uint16_t)(rw(pDS, 0x1B6) + 0x8));
L2916:
    /* 2916  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L291A:
    /* 291A  mov     di,0B1Ah */
    DI = 0xB1A;
L291D:
    /* 291D  cmp     si,1BAh */
    sub16(SI, 0x1BA, 0);
L2921:
    /* 2921  je      short L2926 */
    if (ZF) goto L2926;
L2923:
    /* 2923  mov     di,1BAh */
    DI = 0x1BA;
L2926: /* L2926 */
    /* 2926  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L292A: /* L292A */
    /* 292A  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L292D: /* L292D */
    /* 292D  cmp     si,word ptr ds:[1B6h] */
    sub16(SI, rw(pDS, 0x1B6), 0);
L2931:
    /* 2931  jne     short L2936 */
    if (!ZF) goto L2936;
L2933:
    /* 2933  jmp     L2A03 */
    goto L2A03;
L2936: /* L2936 */
    /* 2936  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L2939:
    /* 2939  test    al,2 */
    logic8((uint8_t)(AL & 0x2));
L293B:
    /* 293B  jne     short L294C */
    if (!ZF) goto L294C;
L293D:
    /* 293D  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L2941:
    /* 2941  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L2945:
    /* 2945  mov     cx,4 */
    CX = 0x4;
L2948:
    /* 2948  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L294A:
    /* 294A  jmp     L292D */
    goto L292D;
L294C: /* L294C */
    /* 294C  test    byte ptr [si-2],2 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x2));
L2950:
    /* 2950  jne     short L29A5 */
    if (!ZF) goto L29A5;
L2952:
    /* 2952  mov     bp,word ptr [si-8] */
    BP = rw(pDS, SI + 0xFFF8);
L2955:
    /* 2955  sub     bp,word ptr [si-4] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0xFFFC));
L2958:
    /* 2958  mov     cx,bp */
    CX = BP;
L295A:
    /* 295A  add     cx,word ptr [si+4] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x4));
L295D:
    /* 295D  sub     cx,word ptr [si] */
    CX = (uint16_t)(CX - rw(pDS, SI));
L295F:
    /* 295F  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L2961:
    /* 2961  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L2964:
    /* 2964  imul    bp */
    imul16(BP);
L2966:
    /* 2966  shl     ax,1 */
    AX = shl16(AX, 1);
L2968:
    /* 2968  rcl     dx,1 */
    DX = rcl16(DX, 1);
L296A:
    /* 296A  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x296A, 2)) != 0) return c;
L296C:
    /* 296C  sar     ax,1 */
    AX = sar16(AX, 1);
L296E:
    /* 296E  jae     short L2973 */
    if (!CF) goto L2973;
L2970:
    /* 2970  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L2971:
    /* 2971  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L2973: /* L2973 */
    /* 2973  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L2976:
    /* 2976  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2977:
    /* 2977  mov     bx,ax */
    BX = AX;
L2979:
    /* 2979  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L297C:
    /* 297C  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L297F:
    /* 297F  imul    bp */
    imul16(BP);
L2981:
    /* 2981  shl     ax,1 */
    AX = shl16(AX, 1);
L2983:
    /* 2983  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2985:
    /* 2985  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x2985, 2)) != 0) return c;
L2987:
    /* 2987  sar     ax,1 */
    AX = sar16(AX, 1);
L2989:
    /* 2989  jae     short L298E */
    if (!CF) goto L298E;
L298B:
    /* 298B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L298C:
    /* 298C  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L298E: /* L298E */
    /* 298E  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L2991:
    /* 2991  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2992:
    /* 2992  mov     cx,ax */
    CX = AX;
L2994:
    /* 2994  mov     ax,bx */
    AX = BX;
L2996:
    /* 2996  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2997:
    /* 2997  mov     bp,ax */
    BP = AX;
L2999:
    /* 2999  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x299C)) != 0) return c;
L299C:
    /* 299C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L299D:
    /* 299D  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L29A1:
    /* 29A1  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L29A5: /* L29A5 */
    /* 29A5  test    byte ptr [si+0Eh],2 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x2));
L29A9:
    /* 29A9  je      short L29AE */
    if (ZF) goto L29AE;
L29AB:
    /* 29AB  jmp     L292A */
    goto L292A;
L29AE: /* L29AE */
    /* 29AE  mov     bp,word ptr [si] */
    BP = rw(pDS, SI);
L29B0:
    /* 29B0  sub     bp,word ptr [si+4] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0x4));
L29B3:
    /* 29B3  mov     cx,bp */
    CX = BP;
L29B5:
    /* 29B5  add     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0xC));
L29B8:
    /* 29B8  sub     cx,word ptr [si+8] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x8));
L29BB:
    /* 29BB  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L29BE:
    /* 29BE  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L29C0:
    /* 29C0  imul    bp */
    imul16(BP);
L29C2:
    /* 29C2  shl     ax,1 */
    AX = shl16(AX, 1);
L29C4:
    /* 29C4  rcl     dx,1 */
    DX = rcl16(DX, 1);
L29C6:
    /* 29C6  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x29C6, 2)) != 0) return c;
L29C8:
    /* 29C8  sar     ax,1 */
    AX = sar16(AX, 1);
L29CA:
    /* 29CA  jae     short L29CF */
    if (!CF) goto L29CF;
L29CC:
    /* 29CC  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L29CD:
    /* 29CD  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L29CF: /* L29CF */
    /* 29CF  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L29D1:
    /* 29D1  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L29D2:
    /* 29D2  mov     bx,ax */
    BX = AX;
L29D4:
    /* 29D4  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L29D7:
    /* 29D7  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L29DA:
    /* 29DA  imul    bp */
    imul16(BP);
L29DC:
    /* 29DC  shl     ax,1 */
    AX = shl16(AX, 1);
L29DE:
    /* 29DE  rcl     dx,1 */
    DX = rcl16(DX, 1);
L29E0:
    /* 29E0  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x29E0, 2)) != 0) return c;
L29E2:
    /* 29E2  sar     ax,1 */
    AX = sar16(AX, 1);
L29E4:
    /* 29E4  jae     short L29E9 */
    if (!CF) goto L29E9;
L29E6:
    /* 29E6  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L29E7:
    /* 29E7  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L29E9: /* L29E9 */
    /* 29E9  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L29EC:
    /* 29EC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L29ED:
    /* 29ED  mov     cx,ax */
    CX = AX;
L29EF:
    /* 29EF  mov     ax,bx */
    AX = BX;
L29F1:
    /* 29F1  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L29F2:
    /* 29F2  mov     bp,ax */
    BP = AX;
L29F4:
    /* 29F4  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x29F7)) != 0) return c;
L29F7:
    /* 29F7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L29F8:
    /* 29F8  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L29FC:
    /* 29FC  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L2A00:
    /* 2A00  jmp     L292A */
    goto L292A;
L2A03: /* L2A03 */
    /* 2A03  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L2A07:
    /* 2A07  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_2A08  (+2A08)
       Clips against x = -z (code bit 1). See clip_top. */
L2A08: /* _clip_left */
    /* 2A08  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L2A0D:
    /* 2A0D  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L2A12:
    /* 2A12  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L2A16:
    /* 2A16  mov     di,word ptr ds:[1B6h] */
    DI = rw(pDS, 0x1B6);
L2A1A:
    /* 2A1A  mov     cx,8 */
    CX = 0x8;
L2A1D:
    /* 2A1D  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L2A1F:
    /* 2A1F  add     word ptr ds:[1B6h],8 */
    ww(pDS, 0x1B6, (uint16_t)(rw(pDS, 0x1B6) + 0x8));
L2A24:
    /* 2A24  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L2A28:
    /* 2A28  mov     di,0B1Ah */
    DI = 0xB1A;
L2A2B:
    /* 2A2B  cmp     si,1BAh */
    sub16(SI, 0x1BA, 0);
L2A2F:
    /* 2A2F  je      short L2A34 */
    if (ZF) goto L2A34;
L2A31:
    /* 2A31  mov     di,1BAh */
    DI = 0x1BA;
L2A34: /* L2A34 */
    /* 2A34  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L2A38: /* L2A38 */
    /* 2A38  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L2A3B: /* L2A3B */
    /* 2A3B  cmp     si,word ptr ds:[1B6h] */
    sub16(SI, rw(pDS, 0x1B6), 0);
L2A3F:
    /* 2A3F  jne     short L2A44 */
    if (!ZF) goto L2A44;
L2A41:
    /* 2A41  jmp     L2B15 */
    goto L2B15;
L2A44: /* L2A44 */
    /* 2A44  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L2A47:
    /* 2A47  test    al,1 */
    logic8((uint8_t)(AL & 0x1));
L2A49:
    /* 2A49  jne     short L2A5A */
    if (!ZF) goto L2A5A;
L2A4B:
    /* 2A4B  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L2A4F:
    /* 2A4F  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L2A53:
    /* 2A53  mov     cx,4 */
    CX = 0x4;
L2A56:
    /* 2A56  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L2A58:
    /* 2A58  jmp     L2A3B */
    goto L2A3B;
L2A5A: /* L2A5A */
    /* 2A5A  test    byte ptr [si-2],1 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x1));
L2A5E:
    /* 2A5E  jne     short L2AB5 */
    if (!ZF) goto L2AB5;
L2A60:
    /* 2A60  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L2A63:
    /* 2A63  add     bp,word ptr [si-8] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0xFFF8));
L2A66:
    /* 2A66  mov     cx,bp */
    CX = BP;
L2A68:
    /* 2A68  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L2A6B:
    /* 2A6B  sub     cx,word ptr [si] */
    CX = (uint16_t)(CX - rw(pDS, SI));
L2A6D:
    /* 2A6D  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L2A6F:
    /* 2A6F  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L2A72:
    /* 2A72  imul    bp */
    imul16(BP);
L2A74:
    /* 2A74  shl     ax,1 */
    AX = shl16(AX, 1);
L2A76:
    /* 2A76  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2A78:
    /* 2A78  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x2A78, 2)) != 0) return c;
L2A7A:
    /* 2A7A  sar     ax,1 */
    AX = sar16(AX, 1);
L2A7C:
    /* 2A7C  jae     short L2A81 */
    if (!CF) goto L2A81;
L2A7E:
    /* 2A7E  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L2A7F:
    /* 2A7F  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L2A81: /* L2A81 */
    /* 2A81  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L2A84:
    /* 2A84  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2A85:
    /* 2A85  mov     bx,ax */
    BX = AX;
L2A87:
    /* 2A87  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L2A8A:
    /* 2A8A  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L2A8D:
    /* 2A8D  imul    bp */
    imul16(BP);
L2A8F:
    /* 2A8F  shl     ax,1 */
    AX = shl16(AX, 1);
L2A91:
    /* 2A91  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2A93:
    /* 2A93  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x2A93, 2)) != 0) return c;
L2A95:
    /* 2A95  sar     ax,1 */
    AX = sar16(AX, 1);
L2A97:
    /* 2A97  jae     short L2A9C */
    if (!CF) goto L2A9C;
L2A99:
    /* 2A99  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L2A9A:
    /* 2A9A  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L2A9C: /* L2A9C */
    /* 2A9C  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L2A9F:
    /* 2A9F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2AA0:
    /* 2AA0  mov     cx,ax */
    CX = AX;
L2AA2:
    /* 2AA2  mov     ax,bx */
    AX = BX;
L2AA4:
    /* 2AA4  neg     ax */
    AX = (uint16_t)-AX;
L2AA6:
    /* 2AA6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2AA7:
    /* 2AA7  mov     bp,ax */
    BP = AX;
L2AA9:
    /* 2AA9  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2AAC)) != 0) return c;
L2AAC:
    /* 2AAC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2AAD:
    /* 2AAD  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L2AB1:
    /* 2AB1  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L2AB5: /* L2AB5 */
    /* 2AB5  test    byte ptr [si+0Eh],1 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x1));
L2AB9:
    /* 2AB9  je      short L2ABE */
    if (ZF) goto L2ABE;
L2ABB:
    /* 2ABB  jmp     L2A38 */
    goto L2A38;
L2ABE: /* L2ABE */
    /* 2ABE  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L2AC1:
    /* 2AC1  add     bp,word ptr [si] */
    BP = (uint16_t)(BP + rw(pDS, SI));
L2AC3:
    /* 2AC3  mov     cx,bp */
    CX = BP;
L2AC5:
    /* 2AC5  sub     cx,word ptr [si+8] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x8));
L2AC8:
    /* 2AC8  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L2ACB:
    /* 2ACB  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L2ACE:
    /* 2ACE  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L2AD0:
    /* 2AD0  imul    bp */
    imul16(BP);
L2AD2:
    /* 2AD2  shl     ax,1 */
    AX = shl16(AX, 1);
L2AD4:
    /* 2AD4  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2AD6:
    /* 2AD6  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x2AD6, 2)) != 0) return c;
L2AD8:
    /* 2AD8  sar     ax,1 */
    AX = sar16(AX, 1);
L2ADA:
    /* 2ADA  jae     short L2ADF */
    if (!CF) goto L2ADF;
L2ADC:
    /* 2ADC  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L2ADD:
    /* 2ADD  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L2ADF: /* L2ADF */
    /* 2ADF  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L2AE1:
    /* 2AE1  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2AE2:
    /* 2AE2  mov     bx,ax */
    BX = AX;
L2AE4:
    /* 2AE4  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L2AE7:
    /* 2AE7  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L2AEA:
    /* 2AEA  imul    bp */
    imul16(BP);
L2AEC:
    /* 2AEC  shl     ax,1 */
    AX = shl16(AX, 1);
L2AEE:
    /* 2AEE  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2AF0:
    /* 2AF0  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x2AF0, 2)) != 0) return c;
L2AF2:
    /* 2AF2  sar     ax,1 */
    AX = sar16(AX, 1);
L2AF4:
    /* 2AF4  jae     short L2AF9 */
    if (!CF) goto L2AF9;
L2AF6:
    /* 2AF6  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L2AF7:
    /* 2AF7  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L2AF9: /* L2AF9 */
    /* 2AF9  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L2AFC:
    /* 2AFC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2AFD:
    /* 2AFD  mov     cx,ax */
    CX = AX;
L2AFF:
    /* 2AFF  mov     ax,bx */
    AX = BX;
L2B01:
    /* 2B01  neg     ax */
    AX = (uint16_t)-AX;
L2B03:
    /* 2B03  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B04:
    /* 2B04  mov     bp,ax */
    BP = AX;
L2B06:
    /* 2B06  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2B09)) != 0) return c;
L2B09:
    /* 2B09  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2B0A:
    /* 2B0A  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L2B0E:
    /* 2B0E  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L2B12:
    /* 2B12  jmp     L2A38 */
    goto L2A38;
L2B15: /* L2B15 */
    /* 2B15  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L2B19:
    /* 2B19  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_2B1A  (+2B1A)
       Opcode 78h (FM has two names at this address, do_parapiped and done_clipping_here),
       operands: point, three half-extents (scaled by the coordinate shift), offset. A bounding-box
       test: rotates the three extents into vectors (148Ah..149Ah) and tests the eight corners of
       the box around the point. All eight inside the frustum: calls set_accept (a no-op in UW2).
       All outside one plane (codes AND nonzero): skips `offset` bytes, or ends the program when
       the offset is 0. Otherwise continues after the operand. */
L2B1A: /* _do_parapiped */
    /* 2B1A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B1B:
    /* 2B1B  mov     di,ax */
    DI = AX;
L2B1D:
    /* 2B1D  mov     cl,byte ptr ds:[25E0h] */
    CL = rb(pDS, 0x25E0);
L2B21:
    /* 2B21  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B22:
    /* 2B22  shl     ax,cl */
    AX = shl16(AX, CL);
L2B24:
    /* 2B24  mov     bx,ax */
    BX = AX;
L2B26:
    /* 2B26  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L2B2A:
    /* 2B2A  mov     word ptr ds:[148Ah],dx */
    ww(pDS, 0x148A, DX);
L2B2E:
    /* 2B2E  mov     ax,bx */
    AX = BX;
L2B30:
    /* 2B30  imul    word ptr ds:[14B4h] */
    imul16(rw(pDS, 0x14B4));
L2B34:
    /* 2B34  mov     word ptr ds:[148Ch],dx */
    ww(pDS, 0x148C, DX);
L2B38:
    /* 2B38  mov     ax,bx */
    AX = BX;
L2B3A:
    /* 2B3A  imul    word ptr ds:[14B6h] */
    imul16(rw(pDS, 0x14B6));
L2B3E:
    /* 2B3E  mov     word ptr ds:[148Eh],dx */
    ww(pDS, 0x148E, DX);
L2B42:
    /* 2B42  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B43:
    /* 2B43  shl     ax,cl */
    AX = shl16(AX, CL);
L2B45:
    /* 2B45  mov     bx,ax */
    BX = AX;
L2B47:
    /* 2B47  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L2B4B:
    /* 2B4B  mov     word ptr ds:[1490h],dx */
    ww(pDS, 0x1490, DX);
L2B4F:
    /* 2B4F  mov     ax,bx */
    AX = BX;
L2B51:
    /* 2B51  imul    word ptr ds:[14BAh] */
    imul16(rw(pDS, 0x14BA));
L2B55:
    /* 2B55  mov     word ptr ds:[1492h],dx */
    ww(pDS, 0x1492, DX);
L2B59:
    /* 2B59  mov     ax,bx */
    AX = BX;
L2B5B:
    /* 2B5B  imul    word ptr ds:[14BCh] */
    imul16(rw(pDS, 0x14BC));
L2B5F:
    /* 2B5F  mov     word ptr ds:[1494h],dx */
    ww(pDS, 0x1494, DX);
L2B63:
    /* 2B63  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B64:
    /* 2B64  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2B66:
    /* 2B66  mov     bx,ax */
    BX = AX;
L2B68:
    /* 2B68  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L2B6C:
    /* 2B6C  mov     word ptr ds:[1496h],dx */
    ww(pDS, 0x1496, DX);
L2B70:
    /* 2B70  mov     ax,bx */
    AX = BX;
L2B72:
    /* 2B72  imul    word ptr ds:[14C0h] */
    imul16(rw(pDS, 0x14C0));
L2B76:
    /* 2B76  mov     word ptr ds:[1498h],dx */
    ww(pDS, 0x1498, DX);
L2B7A:
    /* 2B7A  mov     ax,bx */
    AX = BX;
L2B7C:
    /* 2B7C  imul    word ptr ds:[14C2h] */
    imul16(rw(pDS, 0x14C2));
L2B80:
    /* 2B80  mov     word ptr ds:[149Ah],dx */
    ww(pDS, 0x149A, DX);
L2B84:
    /* 2B84  mov     al,byte ptr [di+14D6h] */
    AL = rb(pDS, DI + 0x14D6);
L2B88:
    /* 2B88  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L2B8A:
    /* 2B8A  je      short L2B8F */
    if (ZF) goto L2B8F;
L2B8C:
    /* 2B8C  jmp     L2E21 */
    goto L2E21;
L2B8F: /* L2B8F */
    /* 2B8F  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2B93:
    /* 2B93  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2B97:
    /* 2B97  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2B9B:
    /* 2B9B  add     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX + rw(pDS, 0x148A));
L2B9F:
    /* 2B9F  add     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX + rw(pDS, 0x148C));
L2BA3:
    /* 2BA3  add     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP + rw(pDS, 0x148E));
L2BA7:
    /* 2BA7  add     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1490));
L2BAB:
    /* 2BAB  add     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1492));
L2BAF:
    /* 2BAF  add     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP + rw(pDS, 0x1494));
L2BB3:
    /* 2BB3  add     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1496));
L2BB7:
    /* 2BB7  add     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1498));
L2BBB:
    /* 2BBB  add     bp,word ptr ds:[149Ah] */
    BP = add16(BP, rw(pDS, 0x149A), 0);
L2BBF:
    /* 2BBF  jns     short L2BC4 */
    if (!SF) goto L2BC4;
L2BC1:
    /* 2BC1  jmp     L2E18 */
    goto L2E18;
L2BC4: /* L2BC4 */
    /* 2BC4  cmp     bx,bp */
    sub16(BX, BP, 0);
L2BC6:
    /* 2BC6  jle     short L2BCB */
    if (ZF || SF != OF) goto L2BCB;
L2BC8:
    /* 2BC8  jmp     L2E18 */
    goto L2E18;
L2BCB: /* L2BCB */
    /* 2BCB  cmp     cx,bp */
    sub16(CX, BP, 0);
L2BCD:
    /* 2BCD  jle     short L2BD2 */
    if (ZF || SF != OF) goto L2BD2;
L2BCF:
    /* 2BCF  jmp     L2E18 */
    goto L2E18;
L2BD2: /* L2BD2 */
    /* 2BD2  neg     bp */
    BP = (uint16_t)-BP;
L2BD4:
    /* 2BD4  cmp     bx,bp */
    sub16(BX, BP, 0);
L2BD6:
    /* 2BD6  jge     short L2BDB */
    if (SF == OF) goto L2BDB;
L2BD8:
    /* 2BD8  jmp     L2E18 */
    goto L2E18;
L2BDB: /* L2BDB */
    /* 2BDB  cmp     cx,bp */
    sub16(CX, BP, 0);
L2BDD:
    /* 2BDD  jge     short L2BE2 */
    if (SF == OF) goto L2BE2;
L2BDF:
    /* 2BDF  jmp     L2E18 */
    goto L2E18;
L2BE2: /* L2BE2 */
    /* 2BE2  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2BE6:
    /* 2BE6  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2BEA:
    /* 2BEA  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2BEE:
    /* 2BEE  sub     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX - rw(pDS, 0x148A));
L2BF2:
    /* 2BF2  sub     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX - rw(pDS, 0x148C));
L2BF6:
    /* 2BF6  sub     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP - rw(pDS, 0x148E));
L2BFA:
    /* 2BFA  add     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1490));
L2BFE:
    /* 2BFE  add     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1492));
L2C02:
    /* 2C02  add     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP + rw(pDS, 0x1494));
L2C06:
    /* 2C06  add     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1496));
L2C0A:
    /* 2C0A  add     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1498));
L2C0E:
    /* 2C0E  add     bp,word ptr ds:[149Ah] */
    BP = add16(BP, rw(pDS, 0x149A), 0);
L2C12:
    /* 2C12  jns     short L2C17 */
    if (!SF) goto L2C17;
L2C14:
    /* 2C14  jmp     L2E18 */
    goto L2E18;
L2C17: /* L2C17 */
    /* 2C17  cmp     bx,bp */
    sub16(BX, BP, 0);
L2C19:
    /* 2C19  jle     short L2C1E */
    if (ZF || SF != OF) goto L2C1E;
L2C1B:
    /* 2C1B  jmp     L2E18 */
    goto L2E18;
L2C1E: /* L2C1E */
    /* 2C1E  cmp     cx,bp */
    sub16(CX, BP, 0);
L2C20:
    /* 2C20  jle     short L2C25 */
    if (ZF || SF != OF) goto L2C25;
L2C22:
    /* 2C22  jmp     L2E18 */
    goto L2E18;
L2C25: /* L2C25 */
    /* 2C25  neg     bp */
    BP = (uint16_t)-BP;
L2C27:
    /* 2C27  cmp     bx,bp */
    sub16(BX, BP, 0);
L2C29:
    /* 2C29  jge     short L2C2E */
    if (SF == OF) goto L2C2E;
L2C2B:
    /* 2C2B  jmp     L2E18 */
    goto L2E18;
L2C2E: /* L2C2E */
    /* 2C2E  cmp     cx,bp */
    sub16(CX, BP, 0);
L2C30:
    /* 2C30  jge     short L2C35 */
    if (SF == OF) goto L2C35;
L2C32:
    /* 2C32  jmp     L2E18 */
    goto L2E18;
L2C35: /* L2C35 */
    /* 2C35  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2C39:
    /* 2C39  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2C3D:
    /* 2C3D  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2C41:
    /* 2C41  add     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX + rw(pDS, 0x148A));
L2C45:
    /* 2C45  add     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX + rw(pDS, 0x148C));
L2C49:
    /* 2C49  add     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP + rw(pDS, 0x148E));
L2C4D:
    /* 2C4D  sub     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1490));
L2C51:
    /* 2C51  sub     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1492));
L2C55:
    /* 2C55  sub     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP - rw(pDS, 0x1494));
L2C59:
    /* 2C59  add     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1496));
L2C5D:
    /* 2C5D  add     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1498));
L2C61:
    /* 2C61  add     bp,word ptr ds:[149Ah] */
    BP = add16(BP, rw(pDS, 0x149A), 0);
L2C65:
    /* 2C65  jns     short L2C6A */
    if (!SF) goto L2C6A;
L2C67:
    /* 2C67  jmp     L2E18 */
    goto L2E18;
L2C6A: /* L2C6A */
    /* 2C6A  cmp     bx,bp */
    sub16(BX, BP, 0);
L2C6C:
    /* 2C6C  jle     short L2C71 */
    if (ZF || SF != OF) goto L2C71;
L2C6E:
    /* 2C6E  jmp     L2E18 */
    goto L2E18;
L2C71: /* L2C71 */
    /* 2C71  cmp     cx,bp */
    sub16(CX, BP, 0);
L2C73:
    /* 2C73  jle     short L2C78 */
    if (ZF || SF != OF) goto L2C78;
L2C75:
    /* 2C75  jmp     L2E18 */
    goto L2E18;
L2C78: /* L2C78 */
    /* 2C78  neg     bp */
    BP = (uint16_t)-BP;
L2C7A:
    /* 2C7A  cmp     bx,bp */
    sub16(BX, BP, 0);
L2C7C:
    /* 2C7C  jge     short L2C81 */
    if (SF == OF) goto L2C81;
L2C7E:
    /* 2C7E  jmp     L2E18 */
    goto L2E18;
L2C81: /* L2C81 */
    /* 2C81  cmp     cx,bp */
    sub16(CX, BP, 0);
L2C83:
    /* 2C83  jge     short L2C88 */
    if (SF == OF) goto L2C88;
L2C85:
    /* 2C85  jmp     L2E18 */
    goto L2E18;
L2C88: /* L2C88 */
    /* 2C88  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2C8C:
    /* 2C8C  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2C90:
    /* 2C90  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2C94:
    /* 2C94  sub     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX - rw(pDS, 0x148A));
L2C98:
    /* 2C98  sub     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX - rw(pDS, 0x148C));
L2C9C:
    /* 2C9C  sub     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP - rw(pDS, 0x148E));
L2CA0:
    /* 2CA0  sub     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1490));
L2CA4:
    /* 2CA4  sub     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1492));
L2CA8:
    /* 2CA8  sub     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP - rw(pDS, 0x1494));
L2CAC:
    /* 2CAC  add     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1496));
L2CB0:
    /* 2CB0  add     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1498));
L2CB4:
    /* 2CB4  add     bp,word ptr ds:[149Ah] */
    BP = add16(BP, rw(pDS, 0x149A), 0);
L2CB8:
    /* 2CB8  jns     short L2CBD */
    if (!SF) goto L2CBD;
L2CBA:
    /* 2CBA  jmp     L2E18 */
    goto L2E18;
L2CBD: /* L2CBD */
    /* 2CBD  cmp     bx,bp */
    sub16(BX, BP, 0);
L2CBF:
    /* 2CBF  jle     short L2CC4 */
    if (ZF || SF != OF) goto L2CC4;
L2CC1:
    /* 2CC1  jmp     L2E18 */
    goto L2E18;
L2CC4: /* L2CC4 */
    /* 2CC4  cmp     cx,bp */
    sub16(CX, BP, 0);
L2CC6:
    /* 2CC6  jle     short L2CCB */
    if (ZF || SF != OF) goto L2CCB;
L2CC8:
    /* 2CC8  jmp     L2E18 */
    goto L2E18;
L2CCB: /* L2CCB */
    /* 2CCB  neg     bp */
    BP = (uint16_t)-BP;
L2CCD:
    /* 2CCD  cmp     bx,bp */
    sub16(BX, BP, 0);
L2CCF:
    /* 2CCF  jge     short L2CD4 */
    if (SF == OF) goto L2CD4;
L2CD1:
    /* 2CD1  jmp     L2E18 */
    goto L2E18;
L2CD4: /* L2CD4 */
    /* 2CD4  cmp     cx,bp */
    sub16(CX, BP, 0);
L2CD6:
    /* 2CD6  jge     short L2CDB */
    if (SF == OF) goto L2CDB;
L2CD8:
    /* 2CD8  jmp     L2E18 */
    goto L2E18;
L2CDB: /* L2CDB */
    /* 2CDB  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2CDF:
    /* 2CDF  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2CE3:
    /* 2CE3  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2CE7:
    /* 2CE7  add     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX + rw(pDS, 0x148A));
L2CEB:
    /* 2CEB  add     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX + rw(pDS, 0x148C));
L2CEF:
    /* 2CEF  add     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP + rw(pDS, 0x148E));
L2CF3:
    /* 2CF3  add     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1490));
L2CF7:
    /* 2CF7  add     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1492));
L2CFB:
    /* 2CFB  add     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP + rw(pDS, 0x1494));
L2CFF:
    /* 2CFF  sub     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1496));
L2D03:
    /* 2D03  sub     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1498));
L2D07:
    /* 2D07  sub     bp,word ptr ds:[149Ah] */
    BP = sub16(BP, rw(pDS, 0x149A), 0);
L2D0B:
    /* 2D0B  jns     short L2D10 */
    if (!SF) goto L2D10;
L2D0D:
    /* 2D0D  jmp     L2E18 */
    goto L2E18;
L2D10: /* L2D10 */
    /* 2D10  cmp     bx,bp */
    sub16(BX, BP, 0);
L2D12:
    /* 2D12  jle     short L2D17 */
    if (ZF || SF != OF) goto L2D17;
L2D14:
    /* 2D14  jmp     L2E18 */
    goto L2E18;
L2D17: /* L2D17 */
    /* 2D17  cmp     cx,bp */
    sub16(CX, BP, 0);
L2D19:
    /* 2D19  jle     short L2D1E */
    if (ZF || SF != OF) goto L2D1E;
L2D1B:
    /* 2D1B  jmp     L2E18 */
    goto L2E18;
L2D1E: /* L2D1E */
    /* 2D1E  neg     bp */
    BP = (uint16_t)-BP;
L2D20:
    /* 2D20  cmp     bx,bp */
    sub16(BX, BP, 0);
L2D22:
    /* 2D22  jge     short L2D27 */
    if (SF == OF) goto L2D27;
L2D24:
    /* 2D24  jmp     L2E18 */
    goto L2E18;
L2D27: /* L2D27 */
    /* 2D27  cmp     cx,bp */
    sub16(CX, BP, 0);
L2D29:
    /* 2D29  jge     short L2D2E */
    if (SF == OF) goto L2D2E;
L2D2B:
    /* 2D2B  jmp     L2E18 */
    goto L2E18;
L2D2E: /* L2D2E */
    /* 2D2E  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2D32:
    /* 2D32  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2D36:
    /* 2D36  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2D3A:
    /* 2D3A  sub     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX - rw(pDS, 0x148A));
L2D3E:
    /* 2D3E  sub     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX - rw(pDS, 0x148C));
L2D42:
    /* 2D42  sub     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP - rw(pDS, 0x148E));
L2D46:
    /* 2D46  add     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1490));
L2D4A:
    /* 2D4A  add     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1492));
L2D4E:
    /* 2D4E  add     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP + rw(pDS, 0x1494));
L2D52:
    /* 2D52  sub     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1496));
L2D56:
    /* 2D56  sub     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1498));
L2D5A:
    /* 2D5A  sub     bp,word ptr ds:[149Ah] */
    BP = sub16(BP, rw(pDS, 0x149A), 0);
L2D5E:
    /* 2D5E  jns     short L2D63 */
    if (!SF) goto L2D63;
L2D60:
    /* 2D60  jmp     L2E18 */
    goto L2E18;
L2D63: /* L2D63 */
    /* 2D63  cmp     bx,bp */
    sub16(BX, BP, 0);
L2D65:
    /* 2D65  jle     short L2D6A */
    if (ZF || SF != OF) goto L2D6A;
L2D67:
    /* 2D67  jmp     L2E18 */
    goto L2E18;
L2D6A: /* L2D6A */
    /* 2D6A  cmp     cx,bp */
    sub16(CX, BP, 0);
L2D6C:
    /* 2D6C  jle     short L2D71 */
    if (ZF || SF != OF) goto L2D71;
L2D6E:
    /* 2D6E  jmp     L2E18 */
    goto L2E18;
L2D71: /* L2D71 */
    /* 2D71  neg     bp */
    BP = (uint16_t)-BP;
L2D73:
    /* 2D73  cmp     bx,bp */
    sub16(BX, BP, 0);
L2D75:
    /* 2D75  jge     short L2D7A */
    if (SF == OF) goto L2D7A;
L2D77:
    /* 2D77  jmp     L2E18 */
    goto L2E18;
L2D7A: /* L2D7A */
    /* 2D7A  cmp     cx,bp */
    sub16(CX, BP, 0);
L2D7C:
    /* 2D7C  jge     short L2D81 */
    if (SF == OF) goto L2D81;
L2D7E:
    /* 2D7E  jmp     L2E18 */
    goto L2E18;
L2D81: /* L2D81 */
    /* 2D81  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2D85:
    /* 2D85  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2D89:
    /* 2D89  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2D8D:
    /* 2D8D  add     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX + rw(pDS, 0x148A));
L2D91:
    /* 2D91  add     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX + rw(pDS, 0x148C));
L2D95:
    /* 2D95  add     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP + rw(pDS, 0x148E));
L2D99:
    /* 2D99  sub     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1490));
L2D9D:
    /* 2D9D  sub     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1492));
L2DA1:
    /* 2DA1  sub     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP - rw(pDS, 0x1494));
L2DA5:
    /* 2DA5  sub     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1496));
L2DA9:
    /* 2DA9  sub     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1498));
L2DAD:
    /* 2DAD  sub     bp,word ptr ds:[149Ah] */
    BP = sub16(BP, rw(pDS, 0x149A), 0);
L2DB1:
    /* 2DB1  js      short L2E18 */
    if (SF) goto L2E18;
L2DB3:
    /* 2DB3  cmp     bx,bp */
    sub16(BX, BP, 0);
L2DB5:
    /* 2DB5  jg      short L2E18 */
    if (!ZF && SF == OF) goto L2E18;
L2DB7:
    /* 2DB7  cmp     cx,bp */
    sub16(CX, BP, 0);
L2DB9:
    /* 2DB9  jg      short L2E18 */
    if (!ZF && SF == OF) goto L2E18;
L2DBB:
    /* 2DBB  neg     bp */
    BP = (uint16_t)-BP;
L2DBD:
    /* 2DBD  cmp     bx,bp */
    sub16(BX, BP, 0);
L2DBF:
    /* 2DBF  jl      short L2E18 */
    if (SF != OF) goto L2E18;
L2DC1:
    /* 2DC1  cmp     cx,bp */
    sub16(CX, BP, 0);
L2DC3:
    /* 2DC3  jl      short L2E18 */
    if (SF != OF) goto L2E18;
L2DC5:
    /* 2DC5  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2DC9:
    /* 2DC9  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2DCD:
    /* 2DCD  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2DD1:
    /* 2DD1  sub     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX - rw(pDS, 0x148A));
L2DD5:
    /* 2DD5  sub     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX - rw(pDS, 0x148C));
L2DD9:
    /* 2DD9  sub     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP - rw(pDS, 0x148E));
L2DDD:
    /* 2DDD  sub     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1490));
L2DE1:
    /* 2DE1  sub     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1492));
L2DE5:
    /* 2DE5  sub     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP - rw(pDS, 0x1494));
L2DE9:
    /* 2DE9  sub     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1496));
L2DED:
    /* 2DED  sub     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1498));
L2DF1:
    /* 2DF1  sub     bp,word ptr ds:[149Ah] */
    BP = sub16(BP, rw(pDS, 0x149A), 0);
L2DF5:
    /* 2DF5  js      short L2E18 */
    if (SF) goto L2E18;
L2DF7:
    /* 2DF7  cmp     bx,bp */
    sub16(BX, BP, 0);
L2DF9:
    /* 2DF9  jg      short L2E18 */
    if (!ZF && SF == OF) goto L2E18;
L2DFB:
    /* 2DFB  cmp     cx,bp */
    sub16(CX, BP, 0);
L2DFD:
    /* 2DFD  jg      short L2E18 */
    if (!ZF && SF == OF) goto L2E18;
L2DFF:
    /* 2DFF  neg     bp */
    BP = (uint16_t)-BP;
L2E01:
    /* 2E01  cmp     bx,bp */
    sub16(BX, BP, 0);
L2E03:
    /* 2E03  jl      short L2E18 */
    if (SF != OF) goto L2E18;
L2E05:
    /* 2E05  cmp     cx,bp */
    sub16(CX, BP, 0);
L2E07:
    /* 2E07  jl      short L2E18 */
    if (SF != OF) goto L2E18;
L2E09:
    /* 2E09  mov     ax,0FFFFh */
    AX = 0xFFFF;
L2E0C:
    /* 2E0C  call    _set_accept */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C4), 0x2E0F)) != 0) return c;
L2E0F:
    /* 2E0F  add     si,2 */
    SI = add16(SI, 0x2, 0);
L2E12:
    /* 2E12  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E13:
    /* 2E13  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2E14:
    /* 2E14  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L2E18: /* L2E18 */
    /* 2E18  add     si,2 */
    SI = add16(SI, 0x2, 0);
L2E1B:
    /* 2E1B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E1C:
    /* 2E1C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2E1D:
    /* 2E1D  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L2E21: /* L2E21 */
    /* 2E21  mov     dl,al */
    DL = AL;
L2E23:
    /* 2E23  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2E27:
    /* 2E27  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2E2B:
    /* 2E2B  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2E2F:
    /* 2E2F  add     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX + rw(pDS, 0x148A));
L2E33:
    /* 2E33  add     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX + rw(pDS, 0x148C));
L2E37:
    /* 2E37  add     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP + rw(pDS, 0x148E));
L2E3B:
    /* 2E3B  add     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1490));
L2E3F:
    /* 2E3F  add     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1492));
L2E43:
    /* 2E43  add     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP + rw(pDS, 0x1494));
L2E47:
    /* 2E47  add     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1496));
L2E4B:
    /* 2E4B  add     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1498));
L2E4F:
    /* 2E4F  add     bp,word ptr ds:[149Ah] */
    BP = (uint16_t)(BP + rw(pDS, 0x149A));
L2E53:
    /* 2E53  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2E56)) != 0) return c;
L2E56:
    /* 2E56  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L2E58:
    /* 2E58  je      L2E18 */
    if (ZF) goto L2E18;
L2E5A:
    /* 2E5A  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2E5E:
    /* 2E5E  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2E62:
    /* 2E62  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2E66:
    /* 2E66  sub     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX - rw(pDS, 0x148A));
L2E6A:
    /* 2E6A  sub     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX - rw(pDS, 0x148C));
L2E6E:
    /* 2E6E  sub     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP - rw(pDS, 0x148E));
L2E72:
    /* 2E72  add     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1490));
L2E76:
    /* 2E76  add     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1492));
L2E7A:
    /* 2E7A  add     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP + rw(pDS, 0x1494));
L2E7E:
    /* 2E7E  add     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1496));
L2E82:
    /* 2E82  add     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1498));
L2E86:
    /* 2E86  add     bp,word ptr ds:[149Ah] */
    BP = (uint16_t)(BP + rw(pDS, 0x149A));
L2E8A:
    /* 2E8A  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2E8D)) != 0) return c;
L2E8D:
    /* 2E8D  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L2E8F:
    /* 2E8F  je      L2E18 */
    if (ZF) goto L2E18;
L2E91:
    /* 2E91  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2E95:
    /* 2E95  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2E99:
    /* 2E99  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2E9D:
    /* 2E9D  add     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX + rw(pDS, 0x148A));
L2EA1:
    /* 2EA1  add     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX + rw(pDS, 0x148C));
L2EA5:
    /* 2EA5  add     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP + rw(pDS, 0x148E));
L2EA9:
    /* 2EA9  sub     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1490));
L2EAD:
    /* 2EAD  sub     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1492));
L2EB1:
    /* 2EB1  sub     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP - rw(pDS, 0x1494));
L2EB5:
    /* 2EB5  add     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1496));
L2EB9:
    /* 2EB9  add     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1498));
L2EBD:
    /* 2EBD  add     bp,word ptr ds:[149Ah] */
    BP = (uint16_t)(BP + rw(pDS, 0x149A));
L2EC1:
    /* 2EC1  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2EC4)) != 0) return c;
L2EC4:
    /* 2EC4  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L2EC6:
    /* 2EC6  jne     short L2ECB */
    if (!ZF) goto L2ECB;
L2EC8:
    /* 2EC8  jmp     L2E18 */
    goto L2E18;
L2ECB: /* L2ECB */
    /* 2ECB  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2ECF:
    /* 2ECF  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2ED3:
    /* 2ED3  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2ED7:
    /* 2ED7  sub     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX - rw(pDS, 0x148A));
L2EDB:
    /* 2EDB  sub     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX - rw(pDS, 0x148C));
L2EDF:
    /* 2EDF  sub     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP - rw(pDS, 0x148E));
L2EE3:
    /* 2EE3  sub     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1490));
L2EE7:
    /* 2EE7  sub     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1492));
L2EEB:
    /* 2EEB  sub     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP - rw(pDS, 0x1494));
L2EEF:
    /* 2EEF  add     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1496));
L2EF3:
    /* 2EF3  add     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1498));
L2EF7:
    /* 2EF7  add     bp,word ptr ds:[149Ah] */
    BP = (uint16_t)(BP + rw(pDS, 0x149A));
L2EFB:
    /* 2EFB  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2EFE)) != 0) return c;
L2EFE:
    /* 2EFE  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L2F00:
    /* 2F00  jne     short L2F05 */
    if (!ZF) goto L2F05;
L2F02:
    /* 2F02  jmp     L2E18 */
    goto L2E18;
L2F05: /* L2F05 */
    /* 2F05  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2F09:
    /* 2F09  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2F0D:
    /* 2F0D  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2F11:
    /* 2F11  add     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX + rw(pDS, 0x148A));
L2F15:
    /* 2F15  add     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX + rw(pDS, 0x148C));
L2F19:
    /* 2F19  add     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP + rw(pDS, 0x148E));
L2F1D:
    /* 2F1D  add     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1490));
L2F21:
    /* 2F21  add     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1492));
L2F25:
    /* 2F25  add     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP + rw(pDS, 0x1494));
L2F29:
    /* 2F29  sub     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1496));
L2F2D:
    /* 2F2D  sub     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1498));
L2F31:
    /* 2F31  sub     bp,word ptr ds:[149Ah] */
    BP = (uint16_t)(BP - rw(pDS, 0x149A));
L2F35:
    /* 2F35  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2F38)) != 0) return c;
L2F38:
    /* 2F38  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L2F3A:
    /* 2F3A  jne     short L2F3F */
    if (!ZF) goto L2F3F;
L2F3C:
    /* 2F3C  jmp     L2E18 */
    goto L2E18;
L2F3F: /* L2F3F */
    /* 2F3F  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2F43:
    /* 2F43  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2F47:
    /* 2F47  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2F4B:
    /* 2F4B  sub     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX - rw(pDS, 0x148A));
L2F4F:
    /* 2F4F  sub     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX - rw(pDS, 0x148C));
L2F53:
    /* 2F53  sub     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP - rw(pDS, 0x148E));
L2F57:
    /* 2F57  add     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX + rw(pDS, 0x1490));
L2F5B:
    /* 2F5B  add     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX + rw(pDS, 0x1492));
L2F5F:
    /* 2F5F  add     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP + rw(pDS, 0x1494));
L2F63:
    /* 2F63  sub     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1496));
L2F67:
    /* 2F67  sub     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1498));
L2F6B:
    /* 2F6B  sub     bp,word ptr ds:[149Ah] */
    BP = (uint16_t)(BP - rw(pDS, 0x149A));
L2F6F:
    /* 2F6F  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2F72)) != 0) return c;
L2F72:
    /* 2F72  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L2F74:
    /* 2F74  jne     short L2F79 */
    if (!ZF) goto L2F79;
L2F76:
    /* 2F76  jmp     L2E18 */
    goto L2E18;
L2F79: /* L2F79 */
    /* 2F79  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2F7D:
    /* 2F7D  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2F81:
    /* 2F81  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2F85:
    /* 2F85  add     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX + rw(pDS, 0x148A));
L2F89:
    /* 2F89  add     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX + rw(pDS, 0x148C));
L2F8D:
    /* 2F8D  add     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP + rw(pDS, 0x148E));
L2F91:
    /* 2F91  sub     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1490));
L2F95:
    /* 2F95  sub     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1492));
L2F99:
    /* 2F99  sub     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP - rw(pDS, 0x1494));
L2F9D:
    /* 2F9D  sub     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1496));
L2FA1:
    /* 2FA1  sub     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1498));
L2FA5:
    /* 2FA5  sub     bp,word ptr ds:[149Ah] */
    BP = (uint16_t)(BP - rw(pDS, 0x149A));
L2FA9:
    /* 2FA9  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2FAC)) != 0) return c;
L2FAC:
    /* 2FAC  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L2FAE:
    /* 2FAE  jne     short L2FB3 */
    if (!ZF) goto L2FB3;
L2FB0:
    /* 2FB0  jmp     L2E18 */
    goto L2E18;
L2FB3: /* L2FB3 */
    /* 2FB3  mov     bx,word ptr [di+14D0h] */
    BX = rw(pDS, DI + 0x14D0);
L2FB7:
    /* 2FB7  mov     cx,word ptr [di+14D2h] */
    CX = rw(pDS, DI + 0x14D2);
L2FBB:
    /* 2FBB  mov     bp,word ptr [di+14D4h] */
    BP = rw(pDS, DI + 0x14D4);
L2FBF:
    /* 2FBF  sub     bx,word ptr ds:[148Ah] */
    BX = (uint16_t)(BX - rw(pDS, 0x148A));
L2FC3:
    /* 2FC3  sub     cx,word ptr ds:[148Ch] */
    CX = (uint16_t)(CX - rw(pDS, 0x148C));
L2FC7:
    /* 2FC7  sub     bp,word ptr ds:[148Eh] */
    BP = (uint16_t)(BP - rw(pDS, 0x148E));
L2FCB:
    /* 2FCB  sub     bx,word ptr ds:[1490h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1490));
L2FCF:
    /* 2FCF  sub     cx,word ptr ds:[1492h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1492));
L2FD3:
    /* 2FD3  sub     bp,word ptr ds:[1494h] */
    BP = (uint16_t)(BP - rw(pDS, 0x1494));
L2FD7:
    /* 2FD7  sub     bx,word ptr ds:[1496h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1496));
L2FDB:
    /* 2FDB  sub     cx,word ptr ds:[1498h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1498));
L2FDF:
    /* 2FDF  sub     bp,word ptr ds:[149Ah] */
    BP = (uint16_t)(BP - rw(pDS, 0x149A));
L2FE3:
    /* 2FE3  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x2FE6)) != 0) return c;
L2FE6:
    /* 2FE6  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L2FE8:
    /* 2FE8  jne     short L2FED */
    if (!ZF) goto L2FED;
L2FEA:
    /* 2FEA  jmp     L2E18 */
    goto L2E18;
L2FED: /* L2FED */
    /* 2FED  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2FEE:
    /* 2FEE  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L2FF0:
    /* 2FF0  je      short L2FFA */
    if (ZF) goto L2FFA;
L2FF2:
    /* 2FF2  add     si,ax */
    SI = add16(SI, AX, 0);
L2FF4:
    /* 2FF4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2FF5:
    /* 2FF5  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2FF6:
    /* 2FF6  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L2FFA: /* L2FFA */
    /* 2FFA  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
