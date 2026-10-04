import re, os
os.chdir(os.path.expanduser('~/UW1Decomp/src'))
def rd(p): return open(p, encoding='latin1').read()
def wr(p, t): open(p, 'w', encoding='latin1', newline='').write(t)
# player.h
p = 'include/player.h'; t = rd(p)
spec = rd('../docs/readability/headers/structs/player.py')
body = re.search(r"TEXT = r'''\n(.*?)'''", spec, re.S).group(1).replace('struct Player1 {', 'struct Player {')
a = t.index("/* The player's record, 0x37D bytes"); b = t.index('};', a) + 3
t = t[:a] + '''/* UW1's player record, 0xD2 bytes, saved in PLAYER.DAT and reached through the near
   pointer player (PLAYER.C's PlayerDat holds it). Reconciled from the bytes of the 30
   files that use it: every field below is at the offset, width and signedness the code
   compiled from. Fields 0x00..0x5D are laid out as in UW2; from 0x5E on UW1 packs its
   flags far tighter (UW2's lefty at 0x65 is UW1's 0x64, its game clock 0x369 UW1's 0xCE).
   Unsigned bitfield runs take a byte at a time (the byte holding the next free bit), so
   the 0x64 run is one byte and quests follows at 0x65. Names are provisional, taken from
   the sources that use the fields. */
''' + body.strip('\n') + '\n' + t[b:]
a = t.index('/* Quest variables 128 and up'); b = t.index('#define XC_CHANGED'); b = t.index('\n', b) + 1
t = t[:a] + t[b:].lstrip('\n')
old = '''/* The player record's storage (player points at it), a byte longer than the record. */
union PlayerStore {
    struct Player rec;
    char bytes[0x37E];
};
extern union PlayerStore PlayerDat;
'''
assert old in t
t = t.replace(old, '''/* The player record's storage as its users see it (player points at it). PLAYER.C defines
   PlayerDat as unsigned char[0xD2] and the files that reach it by name declare it
   themselves as this union, so it is in no header. */
union PlayerStore {
    struct Player rec;
    char bytes[0xD2];
};
''')
assert 'extern unsigned char IsJoy;\n' in t
t = t.replace('extern unsigned char IsJoy;\n', '')
wr(p, t)
p = 'game/PLAYER.C'; t = rd(p)
old = '''/* UW1: the player record's storage is 0xD2 bytes and there is no IsJoy (below); player.h
   declares UW2's, so the declarations are renamed out of the way. */
#define PlayerDat UW2_PlayerDat
#define IsJoy UW2_IsJoy
'''
assert old in t
t = t.replace(old, '').replace('#undef PlayerDat\n#undef IsJoy\n', '')
wr(p, t)
for p in ['inv/INVPANEL.C', 'inv/INVDATA.C', 'inv/BAGS.C', 'combat/SPELLS.C']:
    t = rd(p)
    m = list(re.finditer(r'^#include "[^"]+"\n', t, re.M))[-1]; pos = m.end()
    while True:
        mm = re.match(r'#undef \w+[^\n]*\n', t[pos:])
        if not mm: break
        pos += mm.end()
    t = t[:pos] + "\n/* PLAYER.C defines the record's storage as a byte array; this file reaches it as the\n   union (player.h). */\nextern union PlayerStore PlayerDat;\n" + t[pos:]
    wr(p, t)
# Arc
p = 'include/file.h'; t = rd(p)
old = '''/* ARC.C: the .ark archives */
/* UW1: the archive functions (sys/ARC.C: open_arc, close_arc, put_arc, get_arc, check_arc,
   count_arc) take UW1's 11-byte struct Arc or a file name, not UW2's archive numbers; each
   caller declares them for now (docs/NOTES.md, ARC.C), until the readability pass. */
'''
assert old in t
t = t.replace(old, '''/* An open archive, 11 bytes, kept by the caller (UW1; UW2 keeps its archives in ARC.C by
   number). The callers that only pass it on hold it as bytes (char[12] on the stack). */
struct Arc {
    int16 fd;                           /* 0x00 */
    int16 tmpfd;                        /* 0x02, _arc.tmp */
    uint16 count;                       /* 0x04, number of blocks */
    uint32 far *offtab;                 /* 0x06, the blocks' offsets */
    unsigned char dirty;                /* 0x0A, offtab changed */
};

/* ARC.C: the .ark archives */
''')
wr(p, t)
for p, old in [('sys/ARC.C', '''/* An open archive, kept by the caller (UW1). */
struct Arc {
    int16 fd;                           /* 0x00 */
    int16 tmpfd;                        /* 0x02, _arc.tmp */
    uint16 count;                       /* 0x04, number of blocks */
    uint32 far *offtab;                 /* 0x06, the blocks' offsets */
    unsigned char dirty;                /* 0x0A, offtab changed */
};

'''), ('map/MAP.C', '''/* An open archive, ovr091's record (11 bytes). */
struct Arc {
    char b[11];
};

'''), ('ui/AUTOMAP.C', '''/* UW1: an open archive, as ovr091 (LEV.ARK access) keeps it; 11 bytes (the copies in
   SaveAutoMapLevel), its fields not used here. */
struct Arc {
    char b[11];
};
''')]:
    t = rd(p); assert old in t; wr(p, t.replace(old, ''))
# CutsState
p = 'gfx/CUTS.C'; t = rd(p)
a = t.index("/* UW1: the cutscene's state, on show_anm's stack, 0x4A bytes"); b = t.index('};\n', a) + 3
local = t[a:b]
t = t[:a] + t[b:].lstrip('\n')
t = t.replace('#define CutsState UW2_CutsState\n', '').replace('#undef CutsState\n', '')
wr(p, t)
p = 'include/gfx.h'; h = rd(p)
a = h.index("/* The state of a cutscene, on show_anm's stack, 0x5C bytes."); b = h.index('HOST_LAYOUT_BEGIN\nstruct CutsState {', a)
c = h.index('};\n', b) + 3
body = local[local.index('struct CutsState {'):]
h = h[:a] + '''/* The state of a cutscene, on show_anm's stack, 0x4A bytes (UW1: UW2's without repeat43,
   so the speech and fade fields and the flags come two bytes earlier). Field names are
   ours, from the code that uses them (CUTS.C). Flag bits: b0 drawing (cleared while a key
   skips frames: UW2's b0 is the reverse), b1 a key arrived, b2 play on in this file, b3
   play on in the cutscene, b4 Escape may end it, b5 speech is available, b6 speech is
   playing, b7 the current wait ignores keys. */
HOST_LAYOUT_BEGIN
''' + body + h[c:]
wr(p, h)
print('structs2 done')
