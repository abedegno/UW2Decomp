"""Find addresses written as plain numbers: the layout assumptions a modding build has to know.

    .venv/bin/python tools/addrscan.py [--all] [--ida] [--jumps] [--entries] [STEM ...]

The objects are the matched builds (build/STEM/STEM.OBJ; seg046's, lib/OVERLAY.ASM, is skipped, the link takes the
overlay manager from OVERLAY.LIB). Every operand with no fixup on it that could be an address
is listed, with the segment it goes through; lines marked * may reach DGROUP (or a segment
the scan cannot name) and need reading. docs/LAYOUT.md has the review of the last run.

- A memory operand with a 16-bit displacement (`[1234h]`, `[bx+1234h]`, `ss:[580h]`). In
  compiled C, DS and SS are DGROUP and only direct operands count (`[bx+N]` is a struct
  field: every global has a fixup); `[bp+N]` frames never count. Displacements past the end
  of DGROUP's static data (where its stack and heap are) are listed but not marked.
- A number moved into BX, SI, DI or BP while DS or ES (SS for BP) may be DGROUP; a number
  (moved or added along) in the base, index, SI or DI of a memory use or of a call while its
  segment may be DGROUP; `mov sp,N` (or `mov reg,N` just before `mov sp,reg`) when SS is or
  becomes DGROUP.
- A 16-bit immediate that IDA's listing writes as `offset X` (with --all, also any immediate
  equal to a DS address named in symbols.tsv).

What a segment register holds, in assembly, is followed through the code: from the publics
that compiled C calls (DS = SS = DGROUP), from each public as the other assembly modules
call it (the state at their calls, iterated until it settles), from labels loaded into BP for
seg003's dispatcher (DS = ES = SS = seg_370D, as _seg003_0272_5311 sets them). Tracked: `mov
reg,seg NAME` (a fixup), moves between registers, push/pop, `mov sreg,cs:[x]` after `mov
cs:[x],reg`, `mov sreg,[x]` where the EXE has a relocation at x (the paragraph it holds),
and `lds`/`les` (mem). Values are kept as sets where paths meet. Code no path reaches is
entered with what the segment's computed jumps hold (roots first, then the rest), or nothing
known when there are none. The far data segments are named by their segment table entries
(FD51 is seg_370D, FD58 seg052_519C, FD71 dseg062_62a6).

Each line: OBJECT +offset (EXE file offset res|ovl)  instruction  what  [--ida: IDA's text].
--jumps prints the state at each computed jump, --entries each public's entry state."""
import sys, os, re, struct, glob
here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
from fixups import fixups
from match import load_targets, EXE
from iced_x86 import (Decoder, Formatter, FormatterSyntax, OpKind, Register, FlowControl, Code,
                      InstructionInfoFactory, OpAccess, RegisterExt, Mnemonic)

ASM = os.path.expanduser(os.environ.get('UW2_ASM', '~/UWReverseEngineering/uw2_asm.asm'))
IDA_BIAS = 0x1ED
exe = open(EXE, 'rb').read()
w16 = lambda b, i: struct.unpack_from('<H', b, i)[0]
HDR = w16(exe, 8) * 16
MZEND = (w16(exe, 4) - 1) * 512 + w16(exe, 2) if w16(exe, 2) else w16(exe, 4) * 512
SIZE = {0: 1, 1: 2, 2: 2, 3: 4, 4: 1, 5: 2}
SREG = {Register.DS: 'ds', Register.ES: 'es', Register.SS: 'ss', Register.CS: 'cs', Register.FS: 'fs', Register.GS: 'gs'}
UNK = '?'
RNAME = {Register.BX: 'bx', Register.SI: 'si', Register.DI: 'di', Register.BP: 'bp'}
CREFS = set()
STRING_OPS = set()
PUBSTATE = {}   # public name -> State at its callers in other assembly modules
CALLS = {}      # filled while scanning: extern name -> State at the call
INDIRECT = {}   # code segment name -> State at its computed jumps and calls
INDIRECT_PREV = {}
JUMPS = {}
DGROUP_END = 0x10000
SEGFILE = {}    # segment name (FDnn, DGROUP) -> file offset of its paragraph
PARANAME = {}   # paragraph -> segment name
RELOCS = {}     # file offset -> the paragraph the EXE relocates there
BPTAKEN = set() # publics whose offset some module loads into BP for seg003's dispatcher
DISPATCHED = None

def ida_text():
    """{file offset: IDA's text of the instruction there}, from the labels IDA puts on each one.
    Segments are found by the paragraph in IDA's name, overlays (ovrNNN) by their target table."""
    out = {}; seg = None; pend = None
    lab = re.compile(r'^([A-Za-z_]\w*?)_([0-9A-Fa-f]+)(?::|\s+proc\b)')
    for raw in open(ASM, 'rb'):
        l = raw.decode('latin1').rstrip('\r\n')
        m = re.match(r'^(\S+)\s+segment\b', l)
        if m:
            nm = m.group(1); mm = re.search(r'_([0-9A-Fa-f]{4})$', nm)
            if re.fullmatch(r'ovr\d{3}', nm) and os.path.exists(os.path.join(root, 'targets', nm + '.tsv')):
                seg = (nm, load_targets(nm)[0])
            elif mm: seg = (nm, HDR + (int(mm.group(1), 16) - IDA_BIAS) * 16)
            else: seg = None
            continue
        m = lab.match(l)
        if m and seg and m.group(1) == seg[0]:
            pend = seg[1] + int(m.group(2), 16); continue
        if pend is not None and l.strip() and not l.startswith(('assume', ';')):
            out[pend] = l.split(';')[0].strip(); pend = None
    return out

def objects():
    from sources import all_sources, stem as stem_of
    for src in all_sources():
        m = re.search(r'/\*\s*target:\s*(\w+)\s*\*/', open(src, encoding='latin1').read(3000))
        if not m: continue
        stem = stem_of(src)
        obj = os.path.join(root, 'build', stem, stem + '.OBJ')
        if os.path.exists(obj): yield stem, src, m.group(1), obj

class State:
    """Possible values of the segment registers and of the 16-bit general registers, and a
    stack of pushed values (each a frozenset of names)."""
    __slots__ = ('r', 'stack')
    def __init__(s, r=None, stack=()):
        s.r = dict(r or {}); s.stack = tuple(stack)
    def get(s, reg): return s.r.get(reg, frozenset([UNK]))
    def copy(s): return State(s.r, s.stack)
    def merge(s, o):
        keys = set(s.r) | set(o.r)
        r = {k: s.get(k) | o.get(k) for k in keys}
        # stacks of different depths are lined up at the top, what is popped next
        n = min(len(s.stack), len(o.stack))
        st = tuple(a | b for a, b in zip(s.stack[len(s.stack) - n:], o.stack[len(o.stack) - n:]))
        return State(r, st)
    def key(s): return (tuple(sorted((k, tuple(sorted(v))) for k, v in s.r.items())), s.stack)

R16 = {Register.AX, Register.BX, Register.CX, Register.DX, Register.SI, Register.DI, Register.BP, Register.SP}
def full16(reg):
    """The 16-bit register a register is part of (eax -> ax, al -> ax), or None."""
    try: f = RegisterExt.full_register32(reg)
    except Exception: return None
    m = {Register.EAX: Register.AX, Register.EBX: Register.BX, Register.ECX: Register.CX, Register.EDX: Register.DX,
         Register.ESI: Register.SI, Register.EDI: Register.DI, Register.EBP: Register.BP, Register.ESP: Register.SP}
    return m.get(f, reg if reg in SREG else None)

def scan(stem, src, seg, obj, ida, a, dsnames, segname):
    o = fixups(open(obj, 'rb').read())
    base, size, rows, org = load_targets(seg)
    code = next(i for i, s in enumerate(o['segs']) if s and s[1].upper().endswith('CODE'))
    data = bytes(o['data'][code]); mask = bytearray(len(data)); fx = {}
    for f in o['fixups']:
        if f['seg'] == code:
            fx[f['off']] = f
            for k in range(SIZE.get(f['loc'], 2)):
                if f['off'] + k < len(mask): mask[f['off'] + k] = 1
    is_c = src.upper().endswith('.C')
    where = 'res' if base < MZEND else 'ovl'
    fmt = Formatter(FormatterSyntax.MASM); info = InstructionInfoFactory()
    insns = {}; consts = {}
    dec = Decoder(16, data, ip=0)
    for ins in dec: insns[ins.ip] = ins; consts[ins.ip] = dec.get_constant_offsets(ins)
    # decoding from every entry point too: code a linear sweep misaligns (data before it)
    def decode_from(at):
        d2 = Decoder(16, data, ip=0); d2.position = at
        while d2.can_decode:
            p = d2.position; i = d2.decode()
            if p in insns: break
            insns[p] = i; consts[p] = d2.get_constant_offsets(i)
    def fixname(f):
        tm, ti = f['target']
        if tm == 1: return o['groups'][ti][0]                 # DGROUP
        if tm == 0: return 'CS' if ti == code else o['segs'][ti][0]
        if tm == 2: return segname(o['ext'][ti])
        return UNK
    # entry points: a public that compiled C calls is entered with DS and SS on DGROUP; any
    # other entry (publics only other assembly calls, labels taken by address) with them
    # unknown, so that what they point at stays open rather than assumed
    entry = State({Register.DS: frozenset(['DGROUP']), Register.SS: frozenset(['DGROUP']), Register.CS: frozenset(['CS'])})
    other = State({Register.DS: frozenset([UNK]), Register.SS: frozenset([UNK]), Register.CS: frozenset(['CS'])})
    starts = {}
    for n, (si, off) in o['pubs'].items():
        if si != code: continue
        st0 = PUBSTATE.get(n)
        if n in CREFS: st0 = entry if st0 is None else st0.merge(entry)
        if st0 is not None: starts[off] = st0 if off not in starts else starts[off].merge(st0)
    # seg003's dispatcher (_seg003_0272_5311 in GRDISP) is entered with the routine's offset
    # in BP (`mov bp,offset X`) and calls it with DS = ES = SS = seg_370D, the segment the
    # graphics data (FD51) is in: `mov ax,seg seg_370D; mov ds,ax; mov es,ax; ... mov
    # ss,ds:[558Ch]` (558C holds that paragraph: see the summary). So a label loaded into BP
    # is entered that way.
    for at, ins in insns.items():
        if ins.mnemonic == Mnemonic.MOV and ins.op_count == 2 and ins.op0_kind == OpKind.REGISTER and \
                ins.op0_register == Register.BP and ins.op1_kind == OpKind.IMMEDIATE16:
            f = fx.get(at + 1)
            if not f or f['loc'] not in (1, 5): continue
            g = DISPATCHED
            if f['target'] == (0, code): 
                t = (w16(data, at + 1) + f['disp']) & 0xFFFF
                starts[t] = g if t not in starts or starts[t] is other else starts[t].merge(g)
            elif f['target'][0] == 2:
                n = o['ext'][f['target'][1]]
                BPTAKEN.add(n)
    for n, (si, off) in o['pubs'].items():
        if si == code and n in BPTAKEN:
            starts[off] = DISPATCHED if off not in starts or starts[off] is other else starts[off].merge(DISPATCHED)
    if '--entries' in a:
        for n, (si, off) in sorted(o['pubs'].items(), key=lambda x: x[1][1]):
            if si == code:
                st0 = starts.get(off)
                print(f'  entry {stem} {n} +{off:X}: ' + ('none' if st0 is None else ' '.join(f'{SREG[r]}={"|".join(sorted(st0.get(r)))}' for r in (Register.DS, Register.SS, Register.ES))))
    # instruction starts of the linear sweep and of the entry points (not the misaligned
    # decodes in between)
    states = {}; work = []
    mask_starts = {k for k in range(len(mask)) if mask[k]}
    def push_state(at, st):
        if at not in insns: return
        old = states.get(at)
        new = st if old is None else old.merge(st)
        if old is None or new.key() != old.key():
            states[at] = new; work.append(at)
    for s0 in sorted(starts):
        if s0 < len(data): decode_from(s0)
    aligned = set(insns)
    internal = set()
    for p_, i_ in insns.items():
        if p_ in aligned and i_.op_count and i_.op0_kind == OpKind.NEAR_BRANCH16 and not any(mask[p_ + 1:p_ + i_.len]):
            internal.add(i_.near_branch16)
    csmem = {}      # cs:[x] -> values stored there
    seen_cs = None
    while not is_c and seen_cs != {k: set(v) for k, v in csmem.items()}:
      # passes until what is stored in code-segment variables stops changing
      seen_cs = {k: set(v) for k, v in csmem.items()}; states.clear(); work.clear()
      for s0 in sorted(starts): push_state(s0, starts[s0])
      steps = 0
      seeded = False; stage = 0
      while work or stage < 2:
          if not work:
              # code no path reaches is entered through a computed jump or call (a table of
              # handlers): it gets what the segment's computed jumps hold. Roots first (block
              # starts nothing in the module jumps or calls to), then whatever is still left.
              seeded = True; stage += 1; ind = INDIRECT_PREV.get(o['segs'][code][0])
              if ind is None: ind = other            # no computed jump reached: nothing is known
              falls = True      # whether the instruction before falls through into this one
              for p_ in sorted(p for p in insns if p in aligned):
                  if p_ not in states and (not falls or p_ == 0) and p_ not in mask_starts \
                          and not (stage == 1 and p_ in internal):
                      push_state(p_, State(ind.r, ()))
                      if '--seeds' in a: print(f'  seed {stem} +{p_:X} stage {stage}', file=sys.stderr)
                  falls = insns[p_].flow_control in (FlowControl.NEXT, FlowControl.CONDITIONAL_BRANCH, FlowControl.CALL,
                                                     FlowControl.INDIRECT_CALL, FlowControl.INTERRUPT)
              continue
          at = work.pop(); st = states[at].copy(); ins = insns[at]; steps += 1
          if steps > 400000: break
          cof = consts[at]
          mn = ins.mnemonic
          op0 = ins.op0_register if ins.op_count and ins.op0_kind == OpKind.REGISTER else None
          op1 = ins.op1_register if ins.op_count > 1 and ins.op1_kind == OpKind.REGISTER else None
          def val_imm():
              f = fx.get(at + cof.immediate_offset) if cof.immediate_size == 2 else None
              if f and f['loc'] == 2: return frozenset([fixname(f)])
              if f: return frozenset(['sym'])           # an offset the linker fills in: not a number
              return frozenset(['const:%04X' % ins.immediate16]) if ins.op_count > 1 and ins.op1_kind == OpKind.IMMEDIATE16 else frozenset([UNK])
          if mn == Mnemonic.MOV and op0 == Register.SP:
              st.r[Register.SP] = val_imm() if ins.op1_kind == OpKind.IMMEDIATE16 else (st.get(op1) if op1 is not None else frozenset([UNK]))
          elif mn == Mnemonic.MOV and op0 is not None and full16(op0) == op0 and op0 in R16 | set(SREG):
              if ins.op1_kind == OpKind.IMMEDIATE16: st.r[op0] = val_imm()
              elif op1 is not None: st.r[op0] = st.get(op1)
              elif ins.op1_kind == OpKind.MEMORY:
                  d = ins.memory_displacement & 0xFFFF
                  if ins.memory_segment == Register.CS and ins.memory_base == Register.NONE and d in csmem: st.r[op0] = csmem[d]
                  elif ins.memory_base == Register.NONE and ins.memory_index == Register.NONE and \
                          not any(mask[at + cof.displacement_offset:at + cof.displacement_offset + 2]):
                      # a segment word in a known segment: the EXE's relocation there says
                      # which paragraph it holds (the bytes are the proof)
                      vals = set()
                      for sv in st.get(ins.memory_segment):
                          fo = SEGFILE.get(sv)
                          v = None if fo is None else RELOCS.get(fo + d)
                          vals.add(PARANAME.get(v, f'para:{v:04X}') if v is not None else 'mem')
                      st.r[op0] = frozenset(vals)
                  else: st.r[op0] = frozenset(['mem'])
              else: st.r[op0] = frozenset([UNK])
          elif mn == Mnemonic.MOV and ins.op0_kind == OpKind.MEMORY and op1 is not None and ins.memory_segment == Register.CS \
                  and ins.memory_base == Register.NONE:
              d = ins.memory_displacement & 0xFFFF
              v = st.get(op1); csmem[d] = csmem.get(d, frozenset()) | v
          elif mn == Mnemonic.PUSH:
              if op0 is not None: st.stack = st.stack + (st.get(full16(op0) or op0),)
              elif ins.op0_kind == OpKind.IMMEDIATE16 or ins.op0_kind == OpKind.IMMEDIATE8TO16:
                  f = fx.get(at + cof.immediate_offset) if cof.immediate_size == 2 else None
                  st.stack = st.stack + (frozenset([fixname(f)]) if f and f['loc'] == 2 else frozenset([UNK]),)
              else: st.stack = st.stack + (frozenset([UNK]),)
          elif mn == Mnemonic.POP and op0 is not None:
              v = st.stack[-1] if st.stack else frozenset([UNK]); st.stack = st.stack[:-1]
              st.r[full16(op0) or op0] = v
          elif mn == Mnemonic.POP:
              st.stack = st.stack[:-1]
          elif mn in (Mnemonic.ADD, Mnemonic.SUB) and op0 == Register.SP and ins.op1_kind in (OpKind.IMMEDIATE8TO16, OpKind.IMMEDIATE16):
              n = (ins.immediate(1) & 0xFFFF) // 2
              if mn == Mnemonic.ADD: st.stack = st.stack[:max(0, len(st.stack) - n)]
              else: st.stack = st.stack + (frozenset([UNK]),) * n
          elif mn in (Mnemonic.PUSHA, Mnemonic.PUSHAD):
              st.stack = st.stack + tuple(st.get(r) for r in (Register.AX, Register.CX, Register.DX, Register.BX, Register.SP, Register.BP, Register.SI, Register.DI))
          elif mn in (Mnemonic.POPA, Mnemonic.POPAD):
              vals = st.stack[-8:] if len(st.stack) >= 8 else (frozenset([UNK]),) * 8; st.stack = st.stack[:-8]
              for r, v in zip((Register.AX, Register.CX, Register.DX, Register.BX, Register.SP, Register.BP, Register.SI, Register.DI), vals):
                  if r != Register.SP: st.r[r] = v
          elif mn in (Mnemonic.LDS, Mnemonic.LES, Mnemonic.LSS, Mnemonic.LFS, Mnemonic.LGS):
              sr = {Mnemonic.LDS: Register.DS, Mnemonic.LES: Register.ES, Mnemonic.LSS: Register.SS,
                    Mnemonic.LFS: Register.FS, Mnemonic.LGS: Register.GS}[mn]
              st.r[sr] = frozenset(['mem']); st.r[full16(op0) or op0] = frozenset([UNK])
          elif ins.is_string_instruction:
              # SI and DI move along the same buffer: their origin is kept
              if mn in (Mnemonic.LODSB, Mnemonic.LODSW, Mnemonic.LODSD): st.r[Register.AX] = frozenset([UNK])
              if ins.has_rep_prefix or ins.has_repne_prefix: st.r[Register.CX] = frozenset([UNK])
          elif mn == Mnemonic.XCHG and op0 is not None and op1 is not None:
              a0, a1 = full16(op0) or op0, full16(op1) or op1; v0, v1 = st.get(a0), st.get(a1); st.r[a0] = v1; st.r[a1] = v0
          else:
              # a number added to or moved along keeps its origin (cptr:N), so that a pointer
              # built from a plain number is still seen where it is used
              keep = None
              if mn in (Mnemonic.ADD, Mnemonic.SUB, Mnemonic.INC, Mnemonic.DEC, Mnemonic.ADC, Mnemonic.SBB) \
                      and op0 is not None and full16(op0) == op0 and op0 in R16:
                  keep = frozenset('cptr:' + t.split(':')[1] for t in st.get(op0) if t.startswith(('const:', 'cptr:')))
              for r in info.info(ins).used_registers():
                  if r.access in (OpAccess.WRITE, OpAccess.READ_WRITE, OpAccess.COND_WRITE, OpAccess.READ_COND_WRITE):
                      f = full16(r.register)
                      if f is not None and f != Register.SP: st.r[f] = frozenset([UNK])
              if keep: st.r[op0] = keep | {UNK}
          fc = ins.flow_control
          if fc in (FlowControl.CALL, FlowControl.UNCONDITIONAL_BRANCH, FlowControl.CONDITIONAL_BRANCH) and not seeded:
              f = fx.get(at + 1)
              if f and f['target'][0] == 2:
                  n = o['ext'][f['target'][1]]
                  CALLS[n] = st if n not in CALLS else CALLS[n].merge(st)
          tgt = ins.near_branch16 if ins.op_count and ins.op0_kind in (OpKind.NEAR_BRANCH16, OpKind.NEAR_BRANCH32) else None
          branch_fixed = any(mask[at + 1:at + ins.len])           # a branch with a fixup goes elsewhere
          if fc in (FlowControl.NEXT, FlowControl.INTERRUPT):
              push_state(at + ins.len, st)
          elif fc == FlowControl.CONDITIONAL_BRANCH:
              if tgt is not None and not branch_fixed: push_state(tgt, st)
              push_state(at + ins.len, st)
          elif fc == FlowControl.UNCONDITIONAL_BRANCH:
              if tgt is not None and not branch_fixed: push_state(tgt, st)
          elif fc == FlowControl.CALL:
              if tgt is not None and not branch_fixed: push_state(tgt, State(st.r, st.stack + (frozenset([UNK]),)))
              push_state(at + ins.len, st)          # the callee is taken to keep the segment registers
          elif fc in (FlowControl.INDIRECT_CALL,):
              push_state(at + ins.len, st)
          if fc in (FlowControl.INDIRECT_CALL, FlowControl.INDIRECT_BRANCH) and not seeded:
              # only computed jumps reached from real entries count, so that the seeded code
              # does not feed its own guesses back
              k = o['segs'][code][0]
              INDIRECT[k] = st if k not in INDIRECT else INDIRECT[k].merge(st)
              if '--jumps' in a: JUMPS[(stem, at)] = (fmt.format(ins), st)
    # report
    out = []
    for at, ins in sorted(insns.items()):
        co = consts[at]
        st = None if is_c else states.get(at)
        if st is not None:
            # a memory operand whose base or index register holds a plain number (moved or
            # added along from `mov reg,N`) while its segment may be DGROUP
            uses = []
            for k in range(ins.op_count):
                kd = ins.op_kind(k)
                if kd == OpKind.MEMORY:
                    uses.append((ins.memory_segment, [r for r in (ins.memory_base, ins.memory_index) if r != Register.NONE]))
                elif kd in (OpKind.MEMORY_SEG_SI, OpKind.MEMORY_SEG_ESI):
                    uses.append((ins.memory_segment, [Register.SI]))
                elif kd in (OpKind.MEMORY_ESDI, OpKind.MEMORY_ESEDI):
                    uses.append((Register.ES, [Register.DI]))
                elif kd in (OpKind.MEMORY_SEG_DI, OpKind.MEMORY_SEG_EDI):
                    uses.append((ins.memory_segment, [Register.DI]))
            # a stack placed at a numbered address: `mov sp,N`, or `mov reg,N` shortly before
            # `mov sp,reg`, with SS on DGROUP then or set to DGROUP right after
            if ins.mnemonic == Mnemonic.MOV and ins.op0_kind == OpKind.REGISTER and ins.op0_register == Register.SP:
                num = None
                if ins.op1_kind == OpKind.IMMEDIATE16 and not any(mask[at + 1:at + ins.len]): num = ins.immediate16
                elif ins.op1_kind == OpKind.REGISTER:
                    back = [p_ for p_ in sorted(aligned) if p_ < at][-4:]
                    for p_ in reversed(back):
                        j = insns[p_]
                        if j.mnemonic == Mnemonic.MOV and j.op_count == 2 and j.op0_kind == OpKind.REGISTER and j.op0_register == ins.op1_register:
                            if j.op1_kind == OpKind.IMMEDIATE16 and not any(mask[p_ + 1:p_ + j.len]): num = j.immediate16
                            break
                nxt = [p_ for p_ in sorted(aligned) if p_ > at][:4]
                ss_after = set(st.get(Register.SS))
                for p_ in nxt:
                    j = insns[p_]
                    if j.mnemonic == Mnemonic.MOV and j.op0_kind == OpKind.REGISTER and j.op0_register == Register.SS and p_ in states:
                        ss_after = set(states[p_].get(j.op1_register)) if j.op1_kind == OpKind.REGISTER else {'mem'}
                        break
                if num is not None and 0x10 <= num < DGROUP_END and ss_after & {'DGROUP', UNK}:
                    out.append((at, ins, f'stack at the number {num:04X} with ss in ' + '|'.join(sorted(ss_after & {'DGROUP', UNK})), True))
            if ins.flow_control in (FlowControl.CALL, FlowControl.INDIRECT_CALL, FlowControl.INDIRECT_BRANCH):
                # a number handed to a routine in a pointer register, with DS or ES on DGROUP
                uses += [(Register.DS, [Register.SI]), (Register.DS, [Register.BX]), (Register.ES, [Register.DI]), (Register.DS, [Register.DI])]
            for sr, regs in uses:
                if 'DGROUP' not in st.get(sr) and UNK not in st.get(sr): continue
                for r in regs:
                    if r == Register.BP: continue
                    nums = sorted({int(t.split(':')[1], 16) for t in st.get(r) if t.startswith(('const:', 'cptr:'))})
                    nums = [v for v in nums if 0x10 <= v < DGROUP_END]
                    if nums:
                        out.append((at, ins, f'{SREG.get(sr)}:[{RNAME.get(r, "?")}] with {RNAME.get(r, "?")} from the number ' +
                                    '/'.join(f'{v:04X}' for v in nums) + ' in ' + '|'.join(sorted(st.get(sr) & {'DGROUP', UNK})), True))
        for k in range(ins.op_count):
            kind = ins.op_kind(k)
            if kind == OpKind.MEMORY and co.displacement_size == 2:
                d = ins.memory_displacement & 0xFFFF
                if any(mask[at + co.displacement_offset:at + co.displacement_offset + 2]): continue
                regs = ins.memory_base != Register.NONE or ins.memory_index != Register.NONE
                if ins.memory_base == Register.BP and ins.segment_prefix == Register.NONE: continue
                if ins.memory_base in (Register.SP,): continue
                if d < 0x10 or (regs and d < 0x100 and '--all' not in a) or (regs and d >= 0xFF00): continue
                if regs and is_c and '--all' not in a: continue
                sr = ins.memory_segment
                if is_c: holds = {'ds': 'DGROUP', 'ss': 'DGROUP'}.get(SREG.get(sr), '?')
                elif st is None: holds = 'unreached'
                else: holds = '|'.join(sorted(st.get(sr)))
                # past the end of DGROUP's data (its _BSS ends at DGROUP_END) a number cannot
                # be the address of a DGROUP variable, whatever the register holds
                risky = bool({'DGROUP', '?', 'unreached'} & set(holds.split('|'))) and d < DGROUP_END
                out.append((at, ins, f'{SREG.get(sr, "?")}:{d:04X} in {holds}', risky))
            elif kind == OpKind.IMMEDIATE16 and co.immediate_size == 2:
                if any(mask[at + co.immediate_offset:at + co.immediate_offset + 2]): continue
                v = ins.immediate16; txt = ida.get(base + at, '')
                # in assembly, a number loaded into a pointer register while DS, ES or SS may
                # be DGROUP could be a DGROUP address
                if not is_c and st is not None and ins.mnemonic == Mnemonic.MOV and ins.op0_kind == OpKind.REGISTER and \
                        ins.op0_register in (Register.BX, Register.SI, Register.DI, Register.BP) and 0x10 <= v < DGROUP_END:
                    regs_ = (Register.SS,) if ins.op0_register == Register.BP else (Register.DS, Register.ES)
                    segs_ = set().union(*(st.get(r) for r in regs_))
                    if 'DGROUP' in segs_ or UNK in segs_:
                        out.append((at, ins, f'imm {v:04X} into a pointer register, ' + ('ss' if ins.op0_register == Register.BP else 'ds/es') + ' in ' +
                                    '|'.join(sorted(segs_ & {'DGROUP', UNK})), True)); continue
                if 'offset' in txt or ('--all' in a and v in dsnames):
                    out.append((at, ins, f'imm {v:04X}' + (f' (= {dsnames[v]}?)' if v in dsnames else ''), True))
    lines = []
    for at, ins, what, risky in out:
        t = f'  [ida: {ida.get(base + at, "")}]' if '--ida' in a else ''
        lines.append((risky, f'{stem} +{at:X} ({base + at:X} {where})  {fmt.format(ins):<38} {what}{t}'))
    return lines

def main():
    a = sys.argv[1:]; want = {x.upper() for x in a if not x.startswith('--')}
    dsnames = {}; addr = {}
    for l in open(os.path.join(root, 'symbols.tsv')):
        if l.startswith('#') or not l.strip(): continue
        n, v = l.split('\t')[:2]; addr[n] = v
        if v.startswith('DS:'): dsnames.setdefault(int(v[3:], 16), n)
    # a name used for its segment: the far data segments by their segment table entry
    cblp, cp = w16(exe, 2), w16(exe, 4)
    segtab = struct.unpack_from('<I', exe, MZEND + 8)[0]; nseg = struct.unpack_from('<I', exe, MZEND + 12)[0]
    paras = {}
    for i in range(nseg):
        p, mx, fl, mn = struct.unpack_from('<4H', exe, segtab + 8 * i)
        if 50 <= i <= 78 and mx != 0xFFFF and mx > mn: paras.setdefault(p, f'FD{i}')
        if i == 168: paras[p] = 'DGROUP'; global DGROUP_END; DGROUP_END = mx
        if paras.get(p) and paras[p] not in SEGFILE: SEGFILE[paras[p]] = HDR + p * 16
    PARANAME.update(paras)
    for k in range(w16(exe, 6)):
        o_, s_ = struct.unpack_from('<HH', exe, w16(exe, 0x18) + 4 * k); RELOCS[HDR + s_ * 16 + o_] = w16(exe, HDR + s_ * 16 + o_)
    global DISPATCHED
    f51 = frozenset(['FD51'])
    DISPATCHED = State({Register.DS: f51, Register.ES: f51, Register.SS: f51, Register.CS: frozenset(['CS'])})
    def segname(n):
        v = addr.get(n, '')
        if v.startswith('DS:'): return 'DGROUP'
        if ':' in v:
            p = int(v.split(':')[0], 16)
            return paras.get(p, f'{n}@{p:04X}')
        return n
    ida = ida_text()
    # the names compiled C calls: entered with DS = SS = DGROUP
    for stem, src, seg, obj in objects():
        if src.upper().endswith('.C'):
            o = fixups(open(obj, 'rb').read()); CREFS.update(n for n in o['ext'] if n)
    risky = 0; total = 0
    objs = [x for x in objects() if not x[2].startswith('seg046')]     # the overlay manager links from OVERLAY.LIB
    # the assembly modules call each other: an entry's state is what its callers hold, so
    # scan them all until that stops changing
    for rnd in range(20):
        CALLS.clear(); INDIRECT.clear()
        for stem, src, seg, obj in objs:
            if not src.upper().endswith('.C'): scan(stem, src, seg, obj, ida, a, dsnames, segname)
        new = {n: v for n, v in CALLS.items()}
        same = {n: v.key() for n, v in new.items()} == {n: v.key() for n, v in PUBSTATE.items()} and \
               {n: v.key() for n, v in INDIRECT.items()} == {n: v.key() for n, v in INDIRECT_PREV.items()}
        PUBSTATE.clear(); PUBSTATE.update(new); INDIRECT_PREV.clear(); INDIRECT_PREV.update(INDIRECT)
        if same: break
    for stem, src, seg, obj in objs:
        if want and stem not in want: continue
        for r, line in scan(stem, src, seg, obj, ida, a, dsnames, segname):
            total += 1; risky += r
            if r or '--all' in a or want: print(('* ' if r else '  ') + line)
    for (stem, at), (t, st) in sorted(JUMPS.items()):
        print(f'jump {stem} +{at:X} {t:<30} ' + ' '.join(f'{SREG[r]}={"|".join(sorted(st.get(r)))}' for r in (Register.DS, Register.ES, Register.SS)), file=sys.stderr)
    print(f'-- {total} numeric operands, {risky} that may go through DGROUP (or an unknown segment), marked *', file=sys.stderr)

if __name__ == '__main__':
    main()
