"""Rewrite C declarations to the explicit-width types of src/include/portable.h (int16, uint16,
int32, uint32), for the port (docs/PORT.md, "Integer widths and wrap", Milestone 2 step 4).

    python3 tools/widths.py [--check] PATH...     rewrite the files (or directories) in place
    python3 tools/widths.py --diff PATH...        print what would change, write nothing

Under Turbo C the new names are typedefs of the original types, so the DOS bytes cannot
change, and `make check` proves it. On the host they are exact-width, so stored values wrap
as they did in DOS.

The policy, by where an integer type is written:

- long and unsigned long, everywhere (int32, uint32): the host's long is 64 bits.
- Struct and union fields, bitfields included; file-scope variables (globals, statics,
  externs, typedefs); and static locals: every int, unsigned, short (int16, uint16).
- A pointer to an integer, or an array of them, anywhere (locals, parameters, returns,
  casts): the pointee or element type, because it is DOS-layout data and sets the stride.
- sizeof of an integer type: the explicit width, so sizeof(int) is 2 as in DOS.
- Scalar locals, parameters and function return types, and scalar casts, stay plain int and
  unsigned: any width of at least 16 bits holds their values. The promotion audit
  (tools/intaudit.py) lists the ones whose 16-bit wrap matters, and those are changed by
  hand.

char, signed char and unsigned char are never changed. The tool reads C as tokens and
tracks only what it needs: braces (file scope, struct, function, block, initialiser),
parentheses (parameter lists against expressions), and where a declaration starts.
Comments, strings and #include/#if lines are left alone; #define bodies are read as
expressions. A trailing comment keeps its column where the line has room.
"""
import os, re, sys, argparse, difflib

INTKW = {'signed', 'unsigned', 'int', 'long', 'short'}
QUAL = {'far', 'near', 'huge', '_far', '_near', '_huge', 'const', 'volatile', 'cdecl', 'pascal',
        'interrupt', '_cdecl', '_pascal', '_interrupt', '_seg', '_loadds', '_saveregs'}
STORAGE = {'static', 'extern', 'register', 'auto', 'typedef'}
TYPEKW = {'void', 'char', 'float', 'double', 'struct', 'union', 'enum'} | INTKW
CTLKW = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'case', 'do', 'else', 'goto',
         'break', 'continue', 'default'}
# typedef names the sources declare (src/include and the .C files); a statement starting with
# one is a declaration
TYPEDEFS = set()

TOKEN = re.compile(r'''
    (?P<comment>/\*.*?\*/|//[^\n]*)
  | (?P<pp>^[ \t]*\#[^\n]*(?:\\\n[^\n]*)*)
  | (?P<str>"(?:\\.|[^"\\\n])*"|'(?:\\.|[^'\\\n])*')
  | (?P<ws>\s+)
  | (?P<num>\.?\d[\w.]*)
  | (?P<id>[A-Za-z_]\w*)
  | (?P<punct>\.\.\.|->|\+\+|--|<<=|>>=|<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^!~<>=]=?|[()\[\]{};,.:?\#\\])
''', re.S | re.M | re.X)

MAP = {('int',): 'int16', ('signed',): 'int16', ('signed', 'int'): 'int16', ('short',): 'int16',
       ('short', 'int'): 'int16', ('signed', 'short'): 'int16', ('signed', 'short', 'int'): 'int16',
       ('unsigned',): 'uint16', ('unsigned', 'int'): 'uint16', ('unsigned', 'short'): 'uint16',
       ('unsigned', 'short', 'int'): 'uint16',
       ('long',): 'int32', ('long', 'int'): 'int32', ('signed', 'long'): 'int32',
       ('signed', 'long', 'int'): 'int32',
       ('unsigned', 'long'): 'uint32', ('unsigned', 'long', 'int'): 'uint32'}


def tokenize(text):
    out = []; pos = 0
    while pos < len(text):
        m = TOKEN.match(text, pos)
        if not m: raise SystemExit(f'widths.py: cannot tokenize at {text[pos:pos + 40]!r}')
        kind = m.lastgroup; val = m.group()
        if kind == 'pp':
            # a directive whose trailing comment runs onto later lines takes them too
            end = m.end()
            while val.count('/*') > val.count('*/'):
                close = text.find('*/', end)
                if close < 0: break
                nl = text.find('\n', close)
                end = len(text) if nl < 0 else nl
                val = text[pos:end]
            m_end = end
        else:
            m_end = m.end()
        if kind == 'pp':
            body = re.match(r'[ \t]*#[ \t]*define[ \t]+\w+(\([^)]*\))?', val)
            if body:                      # a #define: its body is read as tokens
                out.append(('pp', body.group()))
                sub = tokenize(val[body.end():])
                out.extend(sub)
                out.append(('ppend', ''))
                pos = m_end; continue
            kind = 'ppline'
        out.append((kind, val)); pos = m_end
    return out


def sig(toks, i, step=1):
    """Index of the next significant token from i (inclusive) in direction step, or None."""
    while 0 <= i < len(toks):
        if toks[i][0] not in ('ws', 'comment'): return i
        i += step
    return None


def match_close(toks, i):
    """toks[i] is ( or [ : index of the matching close."""
    o = toks[i][1]; c = {'(': ')', '[': ']', '{': '}'}[o]; d = 0
    for j in range(i, len(toks)):
        if toks[j][0] != 'punct': continue
        if toks[j][1] in ('(', '[', '{'): d += 1
        elif toks[j][1] in (')', ']', '}'):
            d -= 1
            if d == 0: return j
    return len(toks) - 1


def declarator_kinds(toks, i, multi):
    """Classify the declarator(s) starting at significant index i, up to the end of the
    declaration (multi) or of the first declarator. Returns a list of kinds: 'pointer',
    'array', 'function', 'fnptr', 'scalar'."""
    kinds = []
    while True:
        i = sig(toks, i)
        while i is not None and toks[i][1] in QUAL: i = sig(toks, i + 1)
        if i is None: break
        t = toks[i][1]
        if t == '*':
            # a pointer, unless it is the return type of a function pointer: never here,
            # since that needs a paren first
            kinds.append('pointer')
        elif t == '(':
            j = match_close(toks, i); k = sig(toks, j + 1)
            inner = sig(toks, i + 1)
            while inner is not None and toks[inner][1] in QUAL: inner = sig(toks, inner + 1)
            if inner is not None and toks[inner][1] == '*' and k is not None and toks[k][1] == '(':
                kinds.append('fnptr')
            elif inner is not None and toks[inner][1] == '*' and k is not None and toks[k][1] == '[':
                kinds.append('pointer')
            elif inner is not None and toks[inner][1] == '*':
                kinds.append('pointer')
            else:
                kinds.append('scalar')       # (name) or a cast's (...) abstract parameter list
        elif toks[i][0] == 'id' and t not in TYPEKW:
            k = sig(toks, i + 1)
            nxt = toks[k][1] if k is not None else ''
            kinds.append('function' if nxt in ('(', 'OLDSTYLE') else 'array' if nxt == '[' else 'scalar')
        elif t == '[':
            kinds.append('array')
        else:
            kinds.append('scalar')
        if not multi: break
        # skip to the next top-level comma of this declaration, or stop at its end
        d = 0; j = i
        while j < len(toks):
            v = toks[j][1] if toks[j][0] == 'punct' else None
            if v in ('(', '[', '{'): d += 1
            elif v in (')', ']', '}'):
                if d == 0: return kinds
                d -= 1
            elif v == ';' and d == 0: return kinds
            elif v == ',' and d == 0: break
            j += 1
        else:
            return kinds
        i = j + 1
    return kinds


def rewrite_tokens(toks):
    """Returns the list of (index range, replacement) for the integer specifier runs to change."""
    edits = []
    # frames: kind in 'file', 'struct', 'enum', 'func', 'block', 'init', 'params', 'expr', 'pp'
    stack = [{'kind': 'file', 'stmt': True, 'decl': False, 'static': False}]
    i = 0
    pending_brace = None      # what the next { opens: 'struct', 'enum', 'init', 'func', or None
    in_pp = False
    while i < len(toks):
        kind, val = toks[i]
        top = stack[-1]
        if kind == 'pp':
            stack.append({'kind': 'pp', 'stmt': False, 'decl': False, 'static': False}); i += 1; continue
        if kind == 'ppend':
            while stack[-1]['kind'] != 'pp': stack.pop()
            stack.pop(); i += 1; continue
        if kind in ('ws', 'comment', 'str', 'num', 'ppline'):
            if kind in ('str', 'num'): top['stmt'] = False
            i += 1; continue
        if kind == 'punct':
            if val == '{':
                k = pending_brace
                if k is None:
                    if top['kind'] == 'file': k = 'func'
                    elif top['kind'] in ('func', 'block'): k = 'block'
                    elif top['kind'] == 'init': k = 'init'
                    else: k = 'block'
                stack.append({'kind': k, 'stmt': True, 'decl': False, 'static': False})
                pending_brace = None
            elif val == '}':
                if len(stack) > 1: stack.pop()
                stack[-1]['stmt'] = stack[-1]['kind'] in ('file', 'func', 'block', 'struct')
                stack[-1]['decl'] = False if stack[-1]['kind'] in ('func', 'block') else stack[-1]['decl']
            elif val == '(':
                p = sig(toks, i - 1, -1)
                pv = toks[p][1] if p is not None else ''
                pk = toks[p][0] if p is not None else ''
                is_params = False
                if top['decl'] == 'declarator':
                    if pk == 'id' and pv not in CTLKW and pv not in TYPEKW and pv not in QUAL: is_params = True
                    elif pv == ')': is_params = True
                elif top['kind'] == 'expr' and top.get('cast') and pv == ')':
                    is_params = True             # (void (far *)(int)) in a cast
                if is_params:
                    stack.append({'kind': 'params', 'stmt': True, 'decl': False, 'static': False})
                else:
                    # an expression paren; inside a type it is part of a declarator
                    stack.append({'kind': 'expr', 'stmt': False, 'decl': False, 'static': False,
                                  'sizeof': pv == 'sizeof', 'cast': False,
                                  'inherit': top['decl'] == 'declarator'})
            elif val == ')':
                if len(stack) > 1 and stack[-1]['kind'] in ('params', 'expr'): stack.pop()
            elif val == ';':
                top['stmt'] = top['kind'] in ('file', 'func', 'block', 'struct')
                top['decl'] = False; top['static'] = False
                pending_brace = None
            elif val == ',':
                if top['kind'] == 'params':
                    top['stmt'] = True; top['decl'] = False
                elif top['decl'] == 'init': top['decl'] = 'declarator'
            elif val == '=':
                if top['decl'] == 'declarator':
                    top['decl'] = 'init'
                    if top['kind'] in ('file', 'func', 'block'): pending_brace = 'init'
                else: pending_brace = 'init' if top['kind'] in ('file',) else pending_brace
            elif val == ':' and top['kind'] == 'struct':
                pass
            else:
                top['stmt'] = False
            i += 1; continue
        # identifiers and keywords
        if val in ('struct', 'union', 'enum'):
            j = sig(toks, i + 1)
            if j is not None and toks[j][0] == 'id': j2 = sig(toks, j + 1)
            else: j2 = j
            if j2 is not None and toks[j2][1] == '{':
                pending_brace = 'enum' if val == 'enum' else 'struct'
            if top['stmt'] or top['kind'] == 'params': top['decl'] = 'declarator'
            top['stmt'] = False
            i += 1; continue
        if val in STORAGE:
            if val == 'static': top['static'] = True
            if top['stmt'] or top['kind'] == 'params': top['decl'] = 'declarator'
            i += 1; continue
        if val in INTKW:
            # an integer specifier run
            j = i; words = []; idxs = []
            while j is not None and j < len(toks) and (toks[j][1] in INTKW or toks[j][1] in QUAL or toks[j][1] in STORAGE):
                if toks[j][1] in INTKW: words.append(toks[j][1]); idxs.append(j)
                j = sig(toks, j + 1)
            nxt = toks[j][1] if j is not None else ''
            if nxt in ('char', 'double', 'float'):
                if top['stmt'] or top['kind'] == 'params': top['decl'] = 'declarator'
                top['stmt'] = False; i = j; continue
            # context
            ctx = None
            if top['kind'] == 'expr':
                ctx = 'sizeof' if top.get('sizeof') else 'cast'
                top['cast'] = True
            elif top['kind'] == 'params': ctx = 'param'
            elif top['kind'] == 'struct': ctx = 'field'
            elif top['kind'] == 'file': ctx = 'global'
            elif top['kind'] in ('func', 'block'): ctx = 'global' if top['static'] else 'local'
            elif top['kind'] == 'pp': ctx = 'cast'
            if top['kind'] in ('file', 'struct', 'func', 'block', 'params'):
                if top['stmt'] or top['kind'] == 'params' or top['decl'] == 'declarator':
                    top['decl'] = 'declarator'
            top['stmt'] = False
            multi = ctx in ('global', 'field', 'local')
            kinds = declarator_kinds(toks, j, multi) if j is not None else ['scalar']
            long_ = 'long' in words
            change = False
            if long_: change = True
            elif ctx == 'sizeof': change = True
            elif ctx in ('global', 'field'):
                change = any(k in ('scalar', 'array', 'pointer') for k in kinds)
            elif ctx in ('local', 'param', 'cast'):
                change = any(k in ('array', 'pointer') for k in kinds)
            key = tuple(words)
            if change and key in MAP:
                edits.append((idxs[0], idxs[-1], MAP[key]))
            i = idxs[-1] + 1; continue
        if val in TYPEKW or val in TYPEDEFS:
            if top['kind'] == 'expr' and val in TYPEKW: top['cast'] = True
            if top['stmt'] or top['kind'] == 'params': top['decl'] = 'declarator'
            top['stmt'] = False
            i += 1; continue
        top['stmt'] = False
        i += 1
    return edits


def apply(text, toks, edits):
    """Rebuild the text with the edits, keeping trailing comments in their columns."""
    starts = {}; pos = 0; offs = []
    for t in toks:
        offs.append(pos); pos += len(t[1]) if t[0] != 'ppend' else 0
    # tokens were produced from text in order, except #define headers, which keep their text
    out = []; last = 0; changed_lines = set()
    for a, b, new in edits:
        sa = offs[a]; sb = offs[b] + len(toks[b][1])
        out.append(text[last:sa]); out.append(new); last = sb
    out.append(text[last:])
    return ''.join(out)


def realign(old, new):
    """For each changed line, put a trailing /* comment back at its old column if it fits."""
    ol = old.split('\n'); nl = new.split('\n')
    if len(ol) != len(nl): return new
    res = []
    for a, b in zip(ol, nl):
        if a != b:
            ma = re.search(r'\S(\s{2,})(/\*.*)$', a); mb = re.search(r'\S(\s+)(/\*.*)$', b)
            if ma and mb and ma.group(2) == mb.group(2):
                col = ma.start(2); cur = mb.start(2)
                pad = len(mb.group(1)) + (col - cur)
                b = b[:mb.start(1)] + ' ' * max(pad, 2) + b[mb.start(2):]
            # an aligned field: keep the declarator's column where spaces follow the type
        res.append(b)
    return '\n'.join(res)


def collect_typedefs(paths):
    for p in paths:
        t = open(p, encoding='latin1').read()
        for m in re.finditer(r'\btypedef\b[^;]*?\b(\w+)\s*(?:\)\s*\([^;]*)?(?:\[[^\]]*\])?\s*;', t):
            TYPEDEFS.add(m.group(1))
        for m in re.finditer(r'\btypedef\b[^;]*?\(\s*(?:far\s+|near\s+)?\*\s*(\w+)\s*\)', t):
            TYPEDEFS.add(m.group(1))
    TYPEDEFS.update({'int16', 'uint16', 'int32', 'uint32', 'NEARPTR', 'UNEARPTR', 'FILE', 'size_t'})


def files_of(paths):
    out = []
    for p in paths:
        if os.path.isdir(p):
            for d, _, fs in os.walk(p):
                if os.sep + 'port' in d: continue
                out += [os.path.join(d, f) for f in sorted(fs) if f.upper().endswith(('.C', '.H'))]
        else:
            out.append(p)
    return [p for p in out if os.path.basename(p) != 'portable.h']


def main(argv):
    ap = argparse.ArgumentParser(description='Rewrite integer declarations to explicit widths.')
    ap.add_argument('paths', nargs='+')
    ap.add_argument('--diff', action='store_true', help='print the changes, write nothing')
    a = ap.parse_args(argv)
    here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
    allsrc = files_of([os.path.join(root, 'src')])
    collect_typedefs(allsrc)
    total = 0
    for p in files_of(a.paths):
        text = open(p, encoding='latin1').read()
        toks = tokenize(text)
        edits = rewrite_tokens(toks)
        if not edits: continue
        new = realign(text, apply(text, toks, edits))
        total += len(edits)
        if a.diff:
            sys.stdout.writelines(difflib.unified_diff(text.splitlines(True), new.splitlines(True),
                                                       p, p, n=0))
        else:
            open(p, 'w', encoding='latin1').write(new)
        print(f'{os.path.relpath(p, root)}: {len(edits)}', file=sys.stderr)
    print(f'{total} specifiers rewritten', file=sys.stderr)
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
