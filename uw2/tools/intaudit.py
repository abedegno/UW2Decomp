"""The promotion and overflow audit (docs/PORT.md, "Integer widths and wrap", Milestone 2 step 6):
list the integer expressions in the game's C whose value on the host can differ from the value
Turbo C computes, where int is 16 bits.

    python3 tools/intaudit.py                 summary by category, and build/port/intaudit.txt
    python3 tools/intaudit.py --list CAT      every finding in one category
    python3 tools/intaudit.py --file STEM     every finding in one source

Each C source is compiled for the host as the port compiles it (tools/portcheck.py's flags,
the runtime's compat.h), and clang's AST (-Xclang -ast-dump=json) is walked. For every integer
expression the tool works out two types: the host's (from the AST) and the one Turbo C gives
it, by the 16-bit rules: int and unsigned are 16 bits and do not promote, char promotes to
int, an unsigned bitfield stays unsigned (MATCHING.md: `x > 0` on one compiles to `jbe`), a
hex literal from 0x8000 to 0xFFFF is unsigned and a decimal one is long, and int16, uint16,
int32 and uint32 (portable.h, the runtime's) are int, unsigned, long and unsigned long.

Two things can make the host's value differ:

- overflow: an operation that can carry past 16 bits (+, -, *, <<, unary -, ~) is done in
  16 bits in DOS and in 32 on the host, so the host keeps the high bits that DOS drops;
- signedness: Turbo C does an operation as unsigned (an operand is unsigned or a uint16) while
  the host does it as signed int, because uint16 promotes to int. A negative operand then
  gives another result.

A difference only matters where the value is used whole. The tool follows each value up the
expression to its first consumer. Truncation to 16 bits or less (an assignment or conversion
to int16, uint16, char, a bitfield or a 16-bit parameter) ends it: the host stores what DOS
stores. +, -, *, &, |, ^, << and ~ pass it on, since their low 16 bits depend only on the low
16 bits of their operands. Every other consumer is a finding, in one of these categories:

  compare       a relational or equality operator
  divide        / or %
  shift-right   the left operand of >>
  condition     tested for truth (if, while, for, ?:, !, && and ||)
  widen         converted to a 32-bit or wider type (int32, uint32, a long, a pointer)
  index         an array index or a pointer offset
  int-store     stored in a plain int or unsigned (32 bits on the host)
  int-arg       passed as a plain int or unsigned argument (or to a variadic function)
  return        returned from a function returning plain int or unsigned
  other         anything else

and each finding is marked overflow, signedness or both. Comparisons, divisions and shifts
whose operands Turbo C converts to unsigned while the host compares signed are reported as
signedness findings even with no arithmetic. Literal overflow (a constant expression whose
value does not fit in 16 bits) is reported as overflow.

A finding is a place to look, not a bug: most values never leave 16 bits. The ones that can
are fixed with an explicit-width cast that keeps the DOS bytes (the gate proves it), and the
rest are recorded in docs/PORT.md.
"""
import os, re, sys, json, argparse, subprocess, collections
from concurrent.futures import ThreadPoolExecutor

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
import sources, portcheck

OUT = os.path.join(root, 'build', 'port')
CATS = ['compare', 'divide', 'shift-right', 'condition', 'widen', 'index', 'int-store',
        'int-arg', 'return', 'other']

# DOS types: (bits, signed)
I8, U8, I16, U16, I32, U32, PTR = (8, 1), (8, 0), (16, 1), (16, 0), (32, 1), (32, 0), ('p', 0)


def dos_of_type(t, bitfield=False):
    """Turbo C's view of a (sugared) host type name."""
    t = re.sub(r'\b(const|volatile)\b', '', t).strip()
    if '*' in t or '[' in t or t.startswith('struct') or t.startswith('union'): return PTR
    names = {'char': I8, 'signed char': I8, 'unsigned char': U8, 'int16': I16, 'short': I16,
             'uint16': U16, 'unsigned short': U16, 'int': I16, 'unsigned int': U16, 'unsigned': U16,
             'int32': I32, 'long': I32, 'uint32': U32, 'unsigned long': U32, 'NEARPTR': I16,
             'UNEARPTR': U16, 'size_t': U16, 'long long': I32, 'unsigned long long': U32,
             'int32_t': I32, 'uint32_t': U32, 'int16_t': I16, 'uint16_t': U16, 'uint8_t': U8,
             'int8_t': I8, 'intptr_t': I16, 'uintptr_t': U16, 'ptrdiff_t': I16, '_Bool': I16}
    if t.startswith('enum'): return I16
    return names.get(t)


def host_of_type(t):
    """(bits, signed) of a desugared host type."""
    t = re.sub(r'\b(const|volatile)\b', '', t).strip()
    if '*' in t or '[' in t: return ('p', 0)
    names = {'char': (8, 1), 'signed char': (8, 1), 'unsigned char': (8, 0), 'short': (16, 1),
             'unsigned short': (16, 0), 'int': (32, 1), 'unsigned int': (32, 0), 'long': (64, 1),
             'unsigned long': (64, 0), 'long long': (64, 1), 'unsigned long long': (64, 0),
             '_Bool': (8, 0)}
    if t.startswith('enum'): return (32, 1)
    return names.get(t)


def promote(d):
    if d in (I8, U8): return I16
    return d


def usual(a, b):
    a, b = promote(a), promote(b)
    if a == PTR or b == PTR: return PTR
    if U32 in (a, b): return U32
    if I32 in (a, b): return I32           # long holds every unsigned value
    if U16 in (a, b): return U16
    return I16


def ty(node, key='qualType'):
    t = node.get('type') or {}
    return t.get(key) or t.get('qualType') or ''


def host(node):
    t = node.get('type') or {}
    return host_of_type(t.get('desugaredQualType') or t.get('qualType') or '')


class Audit:
    def __init__(self, path, ast):
        self.path = path; self.ast = ast
        self.text = open(path, encoding='latin1').read()
        self.lines = self.text.split('\n')
        self.lstart = [0]
        for l in self.lines: self.lstart.append(self.lstart[-1] + len(l) + 1)
        self.fields = {}; self.findings = []; self.memo = {}
        self.cur = {'file': ''}
        self.index_fields(ast)

    def index_fields(self, n):
        if isinstance(n, dict):
            if n.get('kind') == 'FieldDecl':
                w = None
                if n.get('isBitfield'):
                    for c in n.get('inner', []) or []:
                        if c.get('kind') == 'ConstantExpr' and 'value' in c: w = int(c['value'])
                self.fields[n['id']] = (w, ty(n))
            for c in n.get('inner', []) or []: self.index_fields(c)

    # --- locations: the JSON gives a file only where it changes, so follow it in order
    def note(self, loc):
        if not isinstance(loc, dict): return None
        if 'expansionLoc' in loc:
            self.note(loc.get('spellingLoc'))
            return self.note(loc['expansionLoc'])
        if 'file' in loc: self.cur['file'] = loc['file']
        return loc.get('offset') if self.cur['file'] == self.path else None

    def where(self, node):
        off = self.note(node.get('loc'))
        rg = node.get('range') or {}
        b = self.note(rg.get('begin')); self.note(rg.get('end'))
        return b if b is not None else off

    def line_of(self, off):
        lo, hi = 0, len(self.lstart) - 1
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if self.lstart[mid] <= off: lo = mid
            else: hi = mid - 1
        return lo + 1

    # --- DOS types of expressions
    def dos(self, n):
        k = n.get('kind')
        inner = n.get('inner') or []
        if k in ('ParenExpr',) and inner: return self.dos(inner[0])
        if k == 'IntegerLiteral':
            v = int(n.get('value', '0'))
            src = self.tok(n)
            if re.search(r'[lL]$', src or ''): return I32
            if v < 0x8000: return I16
            if src and src.lower().startswith('0x') and v <= 0xFFFF: return U16
            if src is None and v <= 0xFFFF: return U16      # spelt in a header: the tree's
            return I32                                      # constants from 0x8000 are hex
        if k == 'CharacterLiteral': return I16
        if k == 'ImplicitCastExpr':
            ck = n.get('castKind')
            if ck in ('LValueToRValue', 'NoOp'): return self.dos(inner[0])
            if ck in ('IntegralCast', 'IntegralToBoolean'):
                sub = self.dos(inner[0])
                tgt = dos_of_type(ty(n)) if ty(n) not in ('int', 'unsigned int') else None
                if tgt: return tgt
                return promote(sub) if sub else dos_of_type(ty(n))
            return dos_of_type(ty(n)) or PTR
        if k == 'CStyleCastExpr': return dos_of_type(ty(n)) or PTR
        if k == 'MemberExpr':
            w, t = self.fields.get(n.get('referencedMemberDecl'), (None, ty(n)))
            return dos_of_type(t)
        if k in ('DeclRefExpr', 'ArraySubscriptExpr', 'CallExpr'):
            return dos_of_type(ty(n))
        if k == 'UnaryOperator':
            op = n.get('opcode')
            if op in ('*',): return dos_of_type(ty(n))
            if op == '!': return I16
            if op in ('&',): return PTR
            if op in ('++', '--'): return self.dos(inner[0])
            return promote(self.dos(inner[0]) or I16)
        if k in ('BinaryOperator', 'CompoundAssignOperator'):
            op = n.get('opcode')
            a, b = self.dos(inner[0]), self.dos(inner[1])
            if op in ('<', '>', '<=', '>=', '==', '!=', '&&', '||'): return I16
            if op in ('<<', '>>'): return promote(a or I16)
            if op == ',': return b
            if k == 'CompoundAssignOperator' or op == '=': return a
            if a is None or b is None: return dos_of_type(ty(n))
            return usual(a, b)
        if k == 'ConditionalOperator':
            a, b = self.dos(inner[1]), self.dos(inner[2])
            if a and b: return usual(a, b)
            return dos_of_type(ty(n))
        if k in ('UnaryExprOrTypeTraitExpr',): return U16
        return dos_of_type(ty(n))

    def tok(self, n):
        rg = n.get('range') or {}
        b = rg.get('begin') or {}
        if 'expansionLoc' in b: b = b.get('spellingLoc') or {}
        off = b.get('offset'); ln = b.get('tokLen')
        f = b.get('file')
        if off is None or ln is None: return None
        if f and f != self.path: return None
        return self.text[off:off + ln]

    # --- the walk
    def run(self):
        for top in self.ast.get('inner', []):
            self.where(top)
            if top.get('kind') == 'FunctionDecl' and any(c.get('kind') == 'CompoundStmt' for c in top.get('inner', []) or []):
                if self.cur['file'] != self.path: self.skip(top); continue
                self.fn = top
                ret = re.match(r'^(.*?)\(', ty(top)).group(1).strip() if '(' in ty(top) else ''
                self.ret_plain = ret in ('int', 'unsigned int', 'unsigned')
                self.walk(top, [])
            else:
                self.skip(top)
        return self.findings

    def skip(self, n):
        # keep the location state right through nodes we do not examine
        for c in n.get('inner', []) or []:
            if isinstance(c, dict):
                self.where(c); self.skip(c)

    def walk(self, n, parents):
        off = self.where(n)
        n['_off'] = off
        k = n.get('kind')
        inner = [c for c in (n.get('inner') or []) if isinstance(c, dict)]
        for c in inner: self.walk(c, parents + [n])
        if k in ('BinaryOperator', 'UnaryOperator', 'CompoundAssignOperator'):
            self.check(n, parents)

    TYPE_RANGE = {I8: (-128, 127), U8: (0, 255), I16: (-0x8000, 0x7FFF), U16: (0, 0xFFFF),
                  I32: (-0x80000000, 0x7FFFFFFF), U32: (0, 0xFFFFFFFF)}

    def rng(self, n):
        """The interval of n's true value, from the DOS ranges of its leaves (no wrap)."""
        key = n.get('id')
        if key in self.memo: return self.memo[key]
        r = self._rng(n)
        if key: self.memo[key] = r
        return r

    def _rng(self, n):
        k = n.get('kind'); op = n.get('opcode'); inner = [c for c in (n.get('inner') or []) if isinstance(c, dict)]
        full = self.TYPE_RANGE.get(self.dos(n), (-0x80000000, 0xFFFFFFFF))
        if k in ('ParenExpr', 'ConstantExpr') and inner: return self.rng(inner[0])
        if k == 'IntegerLiteral': v = int(n.get('value', '0')); return (v, v)
        if k == 'CharacterLiteral': v = int(n.get('value', '0')); return (v, v)
        if k == 'ImplicitCastExpr':
            ck = n.get('castKind')
            if ck in ('LValueToRValue', 'NoOp', 'IntegralCast'):
                sub = self.rng(inner[0]); tgt = self.TYPE_RANGE.get(dos_of_type(ty(n)))
                if ck == 'IntegralCast' and tgt and ty(n) not in ('int', 'unsigned int') and not (tgt[0] <= sub[0] and sub[1] <= tgt[1]):
                    return tgt
                return sub
            if ck == 'IntegralToBoolean': return (0, 1)
            return full
        if k == 'CStyleCastExpr':
            sub = self.rng(inner[0]) if inner else full
            tgt = self.TYPE_RANGE.get(dos_of_type(ty(n)))
            if tgt and tgt[0] <= sub[0] and sub[1] <= tgt[1]: return sub
            return tgt or full
        if k == 'MemberExpr':
            w, t = self.fields.get(n.get('referencedMemberDecl'), (None, ty(n)))
            if w:
                d = dos_of_type(t)
                return (0, (1 << w) - 1) if d and d[1] == 0 else (-(1 << (w - 1)), (1 << (w - 1)) - 1)
            return full
        if k == 'UnaryOperator':
            a = self.rng(inner[0]) if inner else full
            if op == '-': return (-a[1], -a[0])
            if op == '~': return (-a[1] - 1, -a[0] - 1)
            if op == '!': return (0, 1)
            if op == '+': return a
            return full
        if k == 'BinaryOperator' and len(inner) == 2:
            a, b = self.rng(inner[0]), self.rng(inner[1])
            if op == '+': return (a[0] + b[0], a[1] + b[1])
            if op == '-': return (a[0] - b[1], a[1] - b[0])
            if op == '*':
                c = [a[0] * b[0], a[0] * b[1], a[1] * b[0], a[1] * b[1]]; return (min(c), max(c))
            if op in ('<', '>', '<=', '>=', '==', '!=', '&&', '||'): return (0, 1)
            if op == '&':
                if a[0] >= 0 and b[0] >= 0: return (0, min(a[1], b[1]))
                if a[0] >= 0: return (0, a[1])
                if b[0] >= 0: return (0, b[1])
                return full
            if op in ('|', '^'):
                if a[0] >= 0 and b[0] >= 0:
                    m = max(a[1], b[1]); top = 1
                    while top <= m: top <<= 1
                    return (0, top - 1)
                return full
            if op == '<<' and b[0] == b[1] and 0 <= b[0] < 32: return (a[0] << b[0], a[1] << b[0])
            if op == '<<' and b[0] >= 0 and b[1] < 32:
                c = [a[0] << b[0], a[0] << b[1], a[1] << b[0], a[1] << b[1]]; return (min(c), max(c))
            if op == '>>' and b[0] >= 0: return (min(a[0] >> b[0], a[0] >> min(b[1], 40)), max(a[1] >> b[0], a[1] >> min(b[1], 40)))
            if op == '/' and b[0] == b[1] and b[0] != 0:
                c = [int(a[0] / b[0]), int(a[1] / b[0])]; return (min(c), max(c))
            if op == '/' and b[0] > 0: return (min(a[0], 0), max(a[1], 0))
            if op == '%' and b[0] == b[1] and b[0] > 0:
                return (0 if a[0] >= 0 else -(b[0] - 1), b[0] - 1)
            if op == ',': return b
            if op == '=': return self.TYPE_RANGE.get(self.dos(n), full)
            return full
        if k == 'ConditionalOperator' and len(inner) == 3:
            a, b = self.rng(inner[1]), self.rng(inner[2]); return (min(a[0], b[0]), max(a[1], b[1]))
        if k == 'UnaryExprOrTypeTraitExpr': return (0, 0xFFFF)
        if k == 'DeclRefExpr' and (n.get('referencedDecl') or {}).get('kind') == 'EnumConstantDecl':
            return (0, 0x7FFF)                  # the game's enums are all small codes
        return full

    def wide(self, n):
        """Why the host value of n may differ from DOS's: 'overflow', 'signedness', or None.
        Turbo C computes n in 16 bits (int or unsigned) and the host in 32; they differ when
        the true value can leave the DOS type's range."""
        k = n.get('kind'); op = n.get('opcode')
        h = host(n)
        if not h or h[0] == 'p' or h[0] < 32: return None
        d = self.dos(n)
        if d not in (I16, U16): return None
        if k == 'CompoundAssignOperator' or op in ('=', ',', '&&', '||', '!', '<', '>', '<=', '>=', '==', '!='):
            return None
        lo, hi = self.rng(n)
        tlo, thi = self.TYPE_RANGE[d]
        if tlo <= lo and hi <= thi: return None
        if self.const(n): return None
        if d == U16 and h[1] == 1 and lo < 0: return 'signedness' if hi <= thi else 'overflow+signedness'
        return 'overflow'

    def const(self, n):
        k = n.get('kind')
        if k in ('IntegerLiteral', 'CharacterLiteral', 'UnaryExprOrTypeTraitExpr'): return True
        if k in ('DeclRefExpr',): return ((n.get('referencedDecl') or {}).get('kind') == 'EnumConstantDecl')
        if k in ('MemberExpr', 'CallExpr', 'ArraySubscriptExpr'): return False
        inner = [c for c in (n.get('inner') or []) if isinstance(c, dict)]
        return bool(inner) and all(self.const(c) for c in inner)

    def consumer(self, n, parents):
        """Walk up from n past ops that keep the low 16 bits; return (category, node) or None
        if the value is truncated first."""
        child = n
        for p in reversed(parents):
            k = p.get('kind'); op = p.get('opcode')
            if k == 'ParenExpr': child = p; continue
            if k in ('ImplicitCastExpr', 'CStyleCastExpr'):
                h = host(p)
                if p.get('castKind') in ('LValueToRValue', 'NoOp'): child = p; continue
                if p.get('castKind') == 'IntegralToBoolean': return ('condition', p)
                if h and h[0] != 'p' and h[0] <= 16: return None          # truncated
                if h and (h[0] == 'p' or h[0] > 32): return ('widen', p)
                if dos_of_type(ty(p)) in (I32, U32): return ('widen', p)
                child = p; continue
            if k == 'BinaryOperator':
                if op in ('+', '-', '*', '&', '|', '^'):
                    if host(p) and host(p)[0] == 'p': return ('index', p)
                    if self.dos(p) in (I32, U32): return ('widen', p)
                    child = p; continue
                if op == '<<':
                    if child is (p.get('inner') or [None])[0]: child = p; continue
                    return ('other', p)
                if op == '>>': return ('shift-right', p) if child is p['inner'][0] else ('other', p)
                if op in ('/', '%'): return ('divide', p)
                if op in ('<', '>', '<=', '>=', '==', '!='): return ('compare', p)
                if op in ('&&', '||'): return ('condition', p)
                if op == '=':
                    h = host(p)
                    if h and h[0] != 'p' and h[0] <= 16: return None
                    if dos_of_type(ty(p)) in (I32, U32): return ('widen', p)
                    return ('int-store', p)
                if op == ',': child = p; continue
                return ('other', p)
            if k == 'CompoundAssignOperator':
                h = host(p)
                if h and h[0] != 'p' and h[0] <= 16: return None
                if op in ('/=', '%='): return ('divide', p)
                return ('int-store', p)
            if k == 'UnaryOperator':
                if op in ('-', '~'): child = p; continue
                if op == '!': return ('condition', p)
                return ('other', p)
            if k == 'ArraySubscriptExpr': return ('index', p)
            if k == 'CallExpr':
                if (p.get('inner') or [None])[0] is child: return ('other', p)
                return ('int-arg', p)
            if k == 'ReturnStmt': return ('return', p) if self.ret_plain else None
            if k in ('IfStmt', 'WhileStmt', 'DoStmt', 'ForStmt'): return ('condition', p)
            if k == 'ConditionalOperator':
                if (p.get('inner') or [None])[0] is child: return ('condition', p)
                child = p; continue
            if k == 'VarDecl':
                h = host_of_type((p.get('type') or {}).get('desugaredQualType') or ty(p))
                if h and h[0] != 'p' and h[0] <= 16: return None
                if dos_of_type(ty(p)) in (I32, U32): return ('widen', p)
                return ('int-store', p)
            if k == 'SwitchStmt': return ('compare', p)
            if k in ('CompoundStmt',): return None                # a statement on its own
            if k == 'InitListExpr': return ('other', p)
            return ('other', p)
        return None

    def check(self, n, parents):
        op = n.get('opcode')
        inner = n.get('inner') or []
        why = self.wide(n)
        found = None
        if why:
            c = self.consumer(n, parents)
            if c: found = (c[0], why, n)
        # comparisons, divisions and right shifts that Turbo C does unsigned and the host signed
        if n.get('kind') == 'BinaryOperator' and op in ('<', '>', '<=', '>=', '==', '!=', '/', '%', '>>') and len(inner) == 2:
            a, b = self.dos(inner[0]), self.dos(inner[1])
            if op == '>>': conv = promote(a) if a else None
            else: conv = usual(a, b) if a and b else None
            ha = host(inner[0])
            eq_ok = True
            if op in ('==', '!='):
                # equality: only a negative constant against an unsigned 16-bit value is sure
                # to differ (the host never sees the unsigned side equal it)
                ra, rb = self.rng(inner[0]), self.rng(inner[1])
                eq_ok = (self.const(inner[1]) and rb[1] < 0 and ra[0] >= 0) or \
                        (self.const(inner[0]) and ra[1] < 0 and rb[0] >= 0)
            if eq_ok and conv == U16 and ha and ha[0] != 'p' and ha[1] == 1 and min(self.rng(c)[0] for c in inner) < 0:
                cat = {'>>': 'shift-right', '/': 'divide', '%': 'divide'}.get(op, 'compare')
                found = found or (cat, 'signedness', n)
        if found:
            cat, why, node = found
            off = n.get('_off')
            ln = self.line_of(off) if off is not None else 0
            self.findings.append(dict(file=os.path.relpath(self.path, root), line=ln, cat=cat, why=why,
                                      op=op, text=self.lines[ln - 1].strip() if ln else ''))

    def nonneg(self, inner, op):
        """True when the signed host operation gives the unsigned result anyway: every operand
        is a uint16/uint8 value (promoted, so non-negative) or a non-negative literal."""
        for c in inner:
            d = self.dos(c)
            h = host(c)
            if self.is_unsigned_value(c): continue
            return False
        return True

    def is_unsigned_value(self, n):
        k = n.get('kind'); inner = n.get('inner') or []
        if k in ('ParenExpr',) and inner: return self.is_unsigned_value(inner[0])
        if k == 'IntegerLiteral': return True
        if k == 'ImplicitCastExpr' and n.get('castKind') in ('IntegralCast', 'LValueToRValue', 'NoOp'):
            sub = inner[0]
            h = host(sub)
            if h and h[0] != 'p' and h[0] <= 16 and h[1] == 0: return True
            return self.is_unsigned_value(sub)
        if k == 'MemberExpr':
            bf, t = self.fields.get(n.get('referencedMemberDecl'), (False, ''))
            h = host(n)
            return bool(h and h[1] == 0 and h[0] != 'p' and h[0] <= 16)
        h = host(n)
        return bool(h and h[0] != 'p' and h[0] <= 16 and h[1] == 0)


def audit_one(cc, path):
    flags = [f for f in portcheck.FLAGS if not f.startswith('-W') and f != '-ferror-limit=0']
    r = subprocess.run([cc] + flags + ['-w', '-fsyntax-only', '-Xclang', '-ast-dump=json', path],
                       capture_output=True, text=True, cwd=root)
    if r.returncode: return path, None, r.stderr[-2000:]
    ast = json.loads(r.stdout)
    return path, Audit(path, ast).run(), None


def main(argv):
    ap = argparse.ArgumentParser(description='List integer expressions whose value differs between 16 and 32 bits.')
    ap.add_argument('--cc', default=os.environ.get('CC', 'cc'))
    ap.add_argument('--list', metavar='CAT')
    ap.add_argument('--file', metavar='STEM')
    ap.add_argument('--why', metavar='WHY', help='with --list: only overflow or only signedness')
    a = ap.parse_args(argv)
    srcs = [p for p in sources.all_sources() if p.upper().endswith('.C') and not portcheck.dos_only(p)]
    if a.file: srcs = [p for p in srcs if sources.stem(p) == a.file.upper()]
    with ThreadPoolExecutor(max_workers=min(8, os.cpu_count() or 4)) as ex:
        res = list(ex.map(lambda p: audit_one(a.cc, p), srcs))
    allf = []
    for p, f, err in res:
        if f is None: print(f'{os.path.relpath(p, root)}: clang failed\n{err}'); continue
        allf += f
    seen = set(); uniq = []
    for f in allf:
        key = (f['file'], f['line'], f['cat'], f['why'], f['op'])
        if key in seen: continue
        seen.add(key); uniq.append(f)
    if a.list or a.file:
        for f in uniq:
            if a.list and f['cat'] != a.list: continue
            if a.why and a.why not in f['why']: continue
            print(f"{f['file']}:{f['line']}: {f['cat']} {f['why']} ({f['op']}): {f['text']}")
        return 0
    os.makedirs(OUT, exist_ok=True)
    with open(os.path.join(OUT, 'intaudit.txt'), 'w') as out:
        for f in uniq:
            out.write(f"{f['file']}:{f['line']}: {f['cat']} {f['why']} ({f['op']}): {f['text']}\n")
    print(f'{len(srcs)} sources, {len(uniq)} findings (build/port/intaudit.txt)\n')
    tab = collections.defaultdict(collections.Counter)
    for f in uniq:
        for w in f['why'].split('+'): tab[f['cat']][w] += 1
    print(f"{'category':14} {'overflow':>9} {'signedness':>11}")
    for c in CATS:
        if tab[c]: print(f"{c:14} {tab[c]['overflow']:9} {tab[c]['signedness']:11}")
    byfile = collections.Counter(f['file'] for f in uniq)
    print('\nmost findings: ' + ', '.join(f'{os.path.basename(k)} {v}' for k, v in byfile.most_common(8)))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
