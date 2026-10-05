"""UW2's targets for Exhume's tools/vectors.py (make vectors): game rules computed by UW2.EXE's
own routines in Unicorn, for UnderworldGodot and any other reimplementation to be checked
against. vectors/README.md says what each table is; tools/vectorhost-uw2.c is the port's side,
which must agree with UW2.EXE on every case.

Every routine here is resident (no overlay) and is called as the game's C calls it: far, its
arguments on the stack, DS = SS = DGROUP. A routine that calls rand() takes the generator's
32-bit state as the input column seed (Borland's rand: seed = seed * 015A4E35h + 1, the result
the new seed's bits 16-30) and gives the state it leaves as seed_out, so the number of draws
and their values follow from the two.
"""
import struct
from vectors import V, Call, sym, image_word, s8, s16

DS = lambda name: sym(name)[1]
# Borland's rand state: srand stores it with mov [seed],ax at its byte 6, so its DGROUP offset is
# the word at srand + 7 (as runtime/replay/replay.c reads it)
_srand = sym('_srand')
SEED = image_word(_srand[0], _srand[1] + 7)
OBJ_SIZE = 27                          # struct Object, a mobile object (src/include/object.h)
COMOBJ_SIZE = 11                       # struct ComObj
COMOBJ_RESIST = 8
WEAPON_SIZE = 8                        # struct Weapon
WEAPON_SKILL = 6
EDGE_SEEDS = [0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 0x015A4E35]


def seed(rng, k):
    return EDGE_SEEDS[k] if k < len(EDGE_SEEDS) else rng.getrandbits(32)


def seeded(c, inp):
    c.ds(SEED, struct.pack('<I', inp['seed']))
    return c


# ---- skill_check (src/game/SKILLCHK.C) ---------------------------------------------------------

def g_skill(rng, k):
    # every value - target from -35 to +35 at the first seeds, then skills, attributes and defences
    # (0..63) at random seeds
    if k < 71: return dict(value=k - 5, target=30, seed=seed(rng, k))
    return dict(value=rng.randrange(0, 64), target=rng.randrange(0, 64), seed=rng.getrandbits(32))


def c_skill(inp):
    return seeded(Call().word(inp['value']).word(inp['target']), inp)


def r_seeded(name):
    def r(res, inp):
        return {name: s16(res.ax), 'seed_out': res.ds_dword(SEED)}
    return r


# ---- rollem (src/sys/UTIL.C) ------------------------------------------------------------------

def g_rollem(rng, k):
    edges = [(0, 6), (1, 0), (1, -1), (-1, 6), (1, 1), (1, 2), (1, 6), (2, 4), (3, 24), (5, 4), (10, 10), (1, 0x7FFF)]
    if k < len(edges): d, s = edges[k]
    else: d, s = rng.randrange(0, 11), rng.choice([2, 3, 4, 6, 8, 10, 12, 20, 24, 30, rng.randrange(1, 64)])
    return dict(dice=d, sides=s, seed=seed(rng, k))


def c_rollem(inp):
    return seeded(Call().word(inp['dice']).word(inp['sides']), inp)


# ---- pickloc (src/combat/COMBAT.C) -------------------------------------------------------------

def g_pickloc(rng, k):
    # the defender's bottom and top and the blow's, map heights 0..127 as the game's are, each top
    # above its bottom; the first cases put the blow's middle at every height round a 24-high defender
    if k < 48:
        dz, dtop = 40, 64
        az = 20 + k; atop = az + 8
    else:
        dz = rng.randrange(0, 120); dtop = dz + rng.randrange(1, 40)
        az = rng.randrange(0, 120); atop = az + rng.randrange(1, 40)
    return dict(dz=dz, dtop=dtop, az=az, atop=atop, seed=seed(rng, k))


def c_pickloc(inp):
    return seeded(Call().word(inp['dz']).word(inp['dtop']).word(inp['az']).word(inp['atop']), inp)


# ---- move_along (src/sys/UTIL.C) ---------------------------------------------------------------

def g_move(rng, k):
    # distances 0..128: the game's callers pass at most 128 (simple_fizix's step, 80h); above that
    # the routine's 16-bit product (sine / 80h * dist) overflows, which no caller reaches
    if k < 256: h, d = k, [1, 8, 16, 0x80, 3][k % 5]
    else: h, d = rng.randrange(0, 256), rng.choice([0, 1, 2, 3, 7, 8, 11, 16, 24, 32, 64, 127, 128, rng.randrange(0, 129)])
    return dict(heading=h, dist=d, x=rng.randrange(0, 64 * 8), y=rng.randrange(0, 64 * 8))


def c_move(inp):
    c = Call().word(inp['heading']).word(inp['dist'])
    c.x = c.near_ptr(struct.pack('<h', inp['x']))
    c.y = c.near_ptr(struct.pack('<h', inp['y']))
    return c


def r_move(res, inp):
    # the pointers' places: Call hands out locals in order, two bytes each
    from vectors import LOCALS
    return dict(x_out=s16(res.ds_word(LOCALS)), y_out=s16(res.ds_word(LOCALS + 2)))


# ---- check_res (src/combat/DAMAGE.C) -----------------------------------------------------------

def g_res(rng, k):
    types = [1, 2, 3, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x0C, 0x28, 0x09, 0x21]
    resists = [0, 1, 2, 3, 4, 8, 0x10, 0x20, 0x28, 0x40, 0x80, 0x0B, 0x23]
    if k < 13 * 13:
        t, r = types[k % 13], resists[k // 13]
        d = [0, 1, 10, 0x7E, 0x7F, 0x80, 0xFF][k % 7]
    elif k < 13 * 13 + 128:
        # fire on a cold resister and cold on a fire resister, every fourth damage 0..254 (and 255)
        j = k - 13 * 13
        t, r = (8, 0x20) if j % 2 == 0 else (0x20, 8)
        d = min(255, (j // 2) * 4 + (3 if j // 2 == 63 else 0))
    else:
        t, r, d = rng.getrandbits(8), rng.getrandbits(8), rng.getrandbits(8)
    return dict(item=rng.randrange(0x40, 0x80), resist=r, damage=d, type=t, seed=rng.getrandbits(32))


def c_res(inp):
    c = Call()
    c.ds(DS('_ComObjData') + inp['item'] * COMOBJ_SIZE + COMOBJ_RESIST, bytes([inp['resist']]))
    c.far_ptr(struct.pack('<4H', inp['item'], 0, 0, 0))
    c.word(inp['damage']).word(inp['type'])
    return seeded(c, inp)


def r_res(res, inp):
    return dict(result=res.ax & 0xFF, seed_out=res.ds_dword(SEED))


# ---- compute_hitangle (src/combat/COMBAT.C) ----------------------------------------------------

def g_angle(rng, k):
    if k < 64: dh, ah = k // 8, k % 8
    else: dh, ah = rng.randrange(8), rng.randrange(8)
    # the rest of each position word (height, fine x and y) at random: hitangle must not see them
    dpos = rng.getrandbits(16) & ~0x380 | dh << 7
    apos = rng.getrandbits(16) & ~0x380 | ah << 7
    return dict(def_pos=dpos, att_pos=apos, def_item=rng.choice([0x40, 0x41, 0x4F, 0x140, 0x20, 0x7F]))


def c_angle(inp):
    c = Call()
    objs = bytearray(OBJ_SIZE * 3)
    struct.pack_into('<HH', objs, OBJ_SIZE * 1, 0x7F, inp['att_pos'])            # fromwho 1, the player
    struct.pack_into('<HH', objs, OBJ_SIZE * 2, inp['def_item'], inp['def_pos'])  # hitobj 2
    base = c.local(bytes(objs))
    c.ds(DS('_critdata'), struct.pack('<HH', base, __import__('vectors').dgroup()))
    c.ds(DS('_hitobj'), struct.pack('<h', 2))
    c.ds(DS('_fromwho'), struct.pack('<h', 1))
    return c


def r_angle(res, inp):
    return dict(hitangle=res.ds_byte(DS('_hitangle')))


# ---- is_sharp (src/combat/COMBAT.C) ------------------------------------------------------------

def g_sharp(rng, k):
    if k < 64: item = k
    elif k < 128: item = (k - 64) * 8 + rng.randrange(8)
    else: item = rng.randrange(0, 0x200)
    return dict(item=item)


def c_sharp(inp):
    c = Call()
    c.far_ptr(struct.pack('<4H', inp['item'], 0, 0, 0))
    return c


def r_sharp(res, inp):
    return dict(sharp=s8(res.ax))


# ---- debris_type (src/combat/DAMAGE.C) ---------------------------------------------------------

def g_debris(rng, k):
    if k < 32: item = k
    elif k < 96: item = (k - 32) * 8
    else: item = rng.randrange(0, 0x200)
    return dict(item=item, type=rng.choice([4, 8, 3, 0x10, 0x20]), weapon_skill=rng.choice([3, 4, 5, 6, 1, 2]))


def c_debris(inp):
    c = Call()
    c.ds(DS('_Weapons') + (inp['item'] & 0xF) * WEAPON_SIZE + WEAPON_SKILL, bytes([inp['weapon_skill']]))
    return c.word(inp['item']).word(inp['type'])


def r_debris(res, inp):
    return dict(debris=s16(res.ax))


# ---- cSqRt and cSinCos (src/sys/C3DENTRY.ASM into IMATH.ASM) -----------------------------------

def g_sqrt(rng, k):
    edges = [0, 1, 2, 3, 4, 8, 9, 15, 16, 17, 0xFF, 0x100, 0x101, 0x3FFF, 0x4000, 0xFFFF, 0x10000, 0x10001,
             0xFFFFFF, 0x1000000, 0x3FFFFFFF, 0x40000000, 0x7FFFFFFF]
    if k < len(edges): v = edges[k]
    elif k < 200: v = max(0, (k - len(edges)) ** 2 + rng.choice([-1, 0, 1]))
    else: v = rng.getrandbits(rng.choice([8, 16, 24, 30]))
    return dict(value=v)


def c_sqrt(inp):
    return Call().dword(inp['value'])


def r_sqrt(res, inp):
    return dict(root=res.ax)


def g_sincos(rng, k):
    if k < 256: a = k << 8
    elif k < 320: a = rng.randrange(0, 0x10000)
    else: a = (k - 320) * 0x40
    return dict(angle=a & 0xFFFF)


def c_sincos(inp):
    c = Call().word(inp['angle'])
    c.near_ptr(b'\0\0'); c.near_ptr(b'\0\0')
    return c


def r_sincos(res, inp):
    from vectors import LOCALS
    return dict(a=s16(res.ds_word(LOCALS)), b=s16(res.ds_word(LOCALS + 2)))


VECTORS = [
    V('skill_check', '_skill_check', ['value', 'target', 'seed'], ['result', 'seed_out'], g_skill, c_skill,
      r_seeded('result'), port='v_skill_check', what='skill_check(value, target): -1, 0, 1 or 2'),
    V('rollem', '_rollem', ['dice', 'sides', 'seed'], ['total', 'seed_out'], g_rollem, c_rollem,
      r_seeded('total'), port='v_rollem', what='rollem(dice, sides): dice rolled, each 1..sides'),
    V('pickloc', '_pickloc', ['dz', 'dtop', 'az', 'atop', 'seed'], ['loc', 'seed_out'], g_pickloc, c_pickloc,
      r_seeded('loc'), port='v_pickloc', what='pickloc: the body location a blow lands on'),
    V('move_along', '_move_along', ['heading', 'dist', 'x', 'y'], ['x_out', 'y_out'], g_move, c_move, r_move,
      port='v_move_along', cases=400, what='move_along: a point moved dist along a byte heading'),
    V('check_res', '_check_res', ['item', 'resist', 'damage', 'type', 'seed'], ['result', 'seed_out'], g_res, c_res,
      r_res, port='v_check_res', cases=400, what='check_res: damage after the object type\'s resistances'),
    V('compute_hitangle', '_compute_hitangle', ['def_pos', 'att_pos', 'def_item'], ['hitangle'], g_angle, c_angle,
      r_angle, port='v_compute_hitangle', cases=200, what='compute_hitangle: 0 face to face .. 4 from behind'),
    V('is_sharp', '_is_sharp', ['item'], ['sharp'], g_sharp, c_sharp, r_sharp, port='v_is_sharp', cases=300,
      what='is_sharp: an edged or pointed weapon (poison weapon applies)'),
    V('debris_type', '_debris_type', ['item', 'type', 'weapon_skill'], ['debris'], g_debris, c_debris, r_debris,
      port='v_debris_type', cases=300, what='debris_type: the item a destroyed object leaves'),
    V('sqrt', '_cSqRt', ['value'], ['root'], g_sqrt, c_sqrt, r_sqrt, port='v_sqrt', cases=300,
      what='cSqRt: the integer square root of a 32-bit value'),
    V('sincos', '_cSinCos', ['angle'], ['a', 'b'], g_sincos, c_sincos, r_sincos, port='v_sincos', cases=400,
      what='cSinCos(angle, &a, &b): the interpolated sine and cosine table'),
]
