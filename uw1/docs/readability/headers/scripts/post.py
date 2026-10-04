"""Hand edits after hdr.py (stage 2e): SaveGame in UW1's form, stale comments, empty
header sections, the data sections merged, spells' size."""
import os, re, glob
os.chdir(os.path.expanduser('~/UW1Decomp/src'))
def rd(p): return open(p, encoding='latin1').read()
def wr(p, t): open(p, 'w', encoding='latin1', newline='').write(t)
def sub(p, o, n, must=True):
    t = rd(p)
    if o not in t:
        assert not must, (p, o[:60]); return
    wr(p, t.replace(o, n))
# SaveGame: UW1 returns char; the header now says so, GAMEWRAP.C's rename goes
sub('include/file.h', 'int far SaveGame OLDSTYLE((char slot, char *desc));', 'char far SaveGame OLDSTYLE((char slot, char *desc));')
sub('game/GAMEWRAP.C', '#define SaveGame UW2_SaveGame\n', '')
sub('game/GAMEWRAP.C', '#undef SaveGame\n', '')
sub('game/GAMEWRAP.C', 'char far SaveGame OLDSTYLE((char slot, char *desc));\n', '')
sub('game/GAMEWRAP.C', '''/* UW1: LEV.ARK's archive access (ovr091), the map's load and save (ovr123), the texture
   map's (ovr131) and the automap's (ovr092), none matched yet: the listing's names. arc
   is a 12-byte archive record. */''', '''/* LEV.ARK's archive access (ARC.C), declared as this file uses it: arc is a 12-byte buffer
   holding the archive record (file.h's struct Arc). */''')
# UW1's joystick readers are seg019_7CD and seg019_809 (SYSENTRY.ASM; UW2: seg021_22FD_*)
sub('include/portable.h', '#define JOY_READ()      seg021_22FD_7CD()', '#define JOY_READ()      seg019_7CD()')
sub('include/portable.h', '#define JOY_BUTTONS()   seg021_22FD_809()', '#define JOY_BUTTONS()   seg019_809()')
sub('include/sys.h', 'void far seg021_22FD_7CD(void);', 'void far seg019_7CD(void);   ')
sub('include/sys.h', 'void far seg021_22FD_809(void);', 'void far seg019_809(void);   ')
# MAINMENU.C: RestoreGame as file.h declares it; file.h is included now
sub('ui/MAINMENU.C', '''/* UW1: the save-file module, ovr140 (UW2's GAMEWRAP.C, ovr149). file.h is not included:
   copy_file takes a source and a destination file name, not two directories and a name. */
''', '')
sub('ui/MAINMENU.C', 'char far RestoreGame();\n', '')
# comments about the rename tricks, all gone now
for f in glob.glob('**/*.C', recursive=True):
    t = rd(f)
    n = re.sub(r'(?m)^/\*(?:(?!\*/).)*?renamed out(?:(?!\*/).)*?\*/\n', '', t, flags=re.S)
    n = re.sub(r'(?m)^/\* UW1: (callees|declarations this file needs that) the headers lack or give otherwise\. \*/\n\n?', '', n)
    if n != t: wr(f, n)
# per-file copies of spells: SPELLS.C defines 53 (UW2 had 69)
for f in glob.glob('**/*.C', recursive=True):
    t = rd(f)
    if 'extern struct Spell far spells[69];' in t: wr(f, t.replace('extern struct Spell far spells[69];', 'extern struct Spell far spells[53];'))
# headers: empty section titles; one data section per header
for h in glob.glob('include/*.h'):
    L = rd(h).split('\n'); out = []
    for q, l in enumerate(L):
        nxt = L[q + 1] if q + 1 < len(L) else ''
        if re.match(r'/\* [A-Za-z0-9_]+\.(C|ASM)\b.*\*/$', l) and (not nxt.strip() or re.match(r'/\* [A-Za-z0-9_]+\.(C|ASM)\b', nxt) or nxt.startswith('#endif')):
            if out and not out[-1].strip() and not nxt.strip(): continue
            continue
        out.append(l)
    t = '\n'.join(out)
    t = t.replace('/* Defined where no source has it yet: data the link takes from the program */',
                  '/* Defined where no source has it yet: data the link takes from the EXE. */')
    # an empty "Defined where..." section directly before another
    t = re.sub(r'/\* Defined where no source has it yet: data the link takes from the EXE\. \*/\n\n(?=/\* Defined where)', '', t)
    T = '/* Defined where no source has it yet: data the link takes from the EXE.'
    i = t.find(T)
    if i >= 0:
        j = t.find('\n\n' + T + ' */\n', i + 1)
        while j >= 0:
            t = t[:j] + '\n' + t[j + len('\n\n' + T + ' */\n'):]
            j = t.find('\n\n' + T + ' */\n', i + 1)
    t = re.sub(r'\n{3,}', '\n\n', t)
    wr(h, t)
print('post done')
# comments that spoke of UW2's headers, now UW1's
for p, o, n in [
 ('sound/SOUND.C', '''/* UW1: sound.h and file.h are UW2's and declare several of this file's functions with
   other types (UW1 returns plain char where UW2 returns unsigned char; load_sound_driver
   takes no slot), so this file declares what it uses itself. */

''', ''),
 ('sound/SOUND.C', '''/* AIL's sound buffer, 12 bytes (sound.h). */
HOST_LAYOUT_BEGIN
HOST_LAYOUT_END

''', ''),
 ('sound/SOUND.C', '''/* AIL's description of a driver (sound.h). */
HOST_LAYOUT_BEGIN
HOST_LAYOUT_END

''', ''),
 ('ui/GAMESTRN.C', '''/* UW1: ui.h has these under their UW2 listing names (seg039_3452_*); here they take UW1's. */

''', ''),
 ('obj/LOOK.C', '''/* UW1: decode_obj_spell's flag is a plain char (object.h: unsigned char). */

''', ''),
 ('obj/USEITEMS.C', '''/* UW1: how is a plain char throughout (object.h: unsigned char); CloseDoor takes only the
   door; decode_obj_spell's flag is a plain char; useNSpellCharges spends one charge and
   returns nothing. */

''', ''),
 ('obj/USEITEMS.C', '''/* UW1: player_eat returns a plain char (player.h: unsigned char). */''',
  '''/* UW1: player_eat returns a plain char here (SKILLS.C defines it unsigned char). */'''),
 ('obj/USEITEMS.C', '''/* UW1: ObjectActorArg is a char; add_animobj takes the tile as chars. */

''', ''),
 ('obj/USEITEMS.C', '''/* UW1: not in the headers. */''', '''/* Declared as this file calls them (in no header). */'''),
 ('inv/INVPANEL.C', '''/* UW1: AddToEmptySlot returns a plain char (inv.h: unsigned char). */

''', ''),
 ('motion/OBJPHYS.C', '''/* UW1: struct Player differs from UW2's (player.h). mob_to_static reads bit 2 of the word
   at 0x62, the byte at 0x6D and the word at 0x6E (see mob_to_static). */

''', ''),
 ('game/SKILLCHK.C', '''/* UW1: GRFX.C's font loader takes the font's file name; UW1 has no grfx_quikfont (gfx.h
   declares UW2's, which takes an index). */''', '''/* UW1: GRFX.C's font loader takes the font's file name (UW2's grfx_quikfont an index). */'''),
 ('gfx/SCRSHOT.C', '''/* UW1: the overlay's own names (file.h declares UW2's ovr116 ones) */''', '''/* UW1: the overlay's own names (UW2's are ovr116's) */'''),
 ('conv/BARTER.C', '''/* UW1: declarations the headers have in UW2's form, or not at all. */''', '''/* Declared as this file uses them (in no header). */'''),
 ('ui/INTERACT.C', '''/* UW1: signed where UW2's headers have unsigned char (cbw). */''', '''/* Signed here (cbw), unsigned char where they are defined: declared in no header. */'''),
 ('ui/MAINMENU.C', '''grfx_load_font takes the font's file name (gfx.h has UW2's index,
   so the calls cast it, as BAGS.C does)''', '''grfx_load_font takes the font's file name (the calls cast it to int,
   UW2's index, as BAGS.C does)'''),
 ('inv/BAGS.C', '''/* UW1: takes the font's file name; gfx.h has UW2's index */''', '''/* UW1: takes the font's file name; the cast is to UW2's index */'''),
 ('conv/BABLHACK.C', '''/* UW1: CloseDoor takes the door alone; object.h has UW2's two-argument form */''', '''/* UW1: CloseDoor takes the door alone (UW2's takes two) */'''),
]:
    sub(p, o, n)
print('comments done')
sub('include/gfx.h', '''/* match: cutsop_skip and cutsop_wait share a public-order key (875); Turbo C lists such
   publics in reverse order of first sight, and the stub order puts cutsop_skip first, so
   it is declared later */
''', '''/* cutsop_wait, cutsop_skip, cutsop_stop and read_lp_inc are declared in CUTS.C alone, in the
   order its public-order ties need (cutsop_wait and cutsop_skip share the key 875). */
''')
