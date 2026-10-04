"""Check the port's sound drivers against the real ones (docs/PORT.md, "Sound").

    python3 tools/ailcheck.py AIL_LOG HW_LOG [--driver FILE] [--max N]

AIL_LOG and HW_LOG are written by the port's --ail-log and --hw-log (Exhume's runtime/port/sound/ail.c):
every AIL call the game made to its music driver, with the data it passed (the XMIDI image,
each timbre), every service tick of the driver, and every register write or MIDI byte the
port's C driver made, each with the AIL tick it belongs to. This tool loads the user's own
.ADV file (SOUND\\DMnn.ADV for the music card the log names, or --driver) into an x86
emulator (Unicorn, `pip install unicorn`, GPL-2.0; a development tool only, never part of the
port), makes the same calls in the same order with the same data, records every byte the
real driver writes to the FM chip's or the MPU-401's ports, and compares the two streams
write for write, tick for tick, and the values the calls returned (timbre requests, locked
channels, timbre statuses, sequence handles). Identical streams mean the C driver programs the
chip exactly as the DOS driver did for that session.

Unicorn is not a dependency of anything else in the repository; install it in the .venv or
any Python (3.9 or later, unicorn 2.1.4 tested; 2.1.0 crashes on macOS 26).
"""
import os, re, sys, struct, argparse

try:
    from unicorn import Uc, UcError, UC_ARCH_X86, UC_MODE_16, UC_HOOK_INSN
    from unicorn.x86_const import (UC_X86_REG_CS, UC_X86_REG_IP, UC_X86_REG_SS, UC_X86_REG_SP, UC_X86_REG_DS,
                                   UC_X86_REG_ES, UC_X86_REG_AX, UC_X86_REG_DX, UC_X86_INS_OUT,
                                   UC_X86_INS_IN)
except ImportError:
    sys.exit('ailcheck.py: needs Unicorn (pip install unicorn)')

DATA = os.environ.get('UW2_DIR') or (os.path.dirname(os.environ['UW2_EXE']) if os.environ.get('UW2_EXE')
                                     else os.path.expanduser('~/UWGOG/UW2'))
# the driver files by the kind numbers of the runtime's port/sound/aildrv.h
KIND_FILE = {1: 'DM02.ADV', 2: 'DM03.ADV', 3: 'DM04.ADV', 4: 'DM06.ADV', 5: 'DM07.ADV', 6: 'DM05.ADV'}
# AIL.INC's driver function numbers
FN = dict(init_driver=0x66, serve=0x67, shutdown=0x68, state_size=0x96, register_sequence=0x97,
          release_sequence=0x98, cache_size=0x99, define_timbre_cache=0x9A, timbre_request=0x9B,
          install_timbre=0x9C, protect_timbre=0x9D, unprotect_timbre=0x9E, timbre_status=0x9F,
          start_sequence=0xAA, stop_sequence=0xAB, resume_sequence=0xAD, sequence_status=0xAE,
          set_relative_volume=0xB1, set_relative_tempo=0xB2, send_cv=0xBA, lock_channel=0xBF,
          release_channel=0xC1)
DRV_SEG, XMI_SEG, STATE_SEG, CACHE_SEG, TIMB_SEG, STACK_SEG, RET_SEG = \
    0x1000, 0x2000, 0x2300, 0x2400, 0x2500, 0x7000, 0x0F00


class Driver:
    def __init__(self, path, kind, io):
        self.img = open(path, 'rb').read()
        t = struct.unpack_from('<H', self.img, 0)[0]
        self.table = {}
        while True:
            fn, off = struct.unpack_from('<HH', self.img, t); t += 4
            if fn == 0xFFFF: break
            self.table[fn] = off
        self.mu = Uc(UC_ARCH_X86, UC_MODE_16)
        self.mu.mem_map(0, 0x100000)
        self.mu.mem_write(DRV_SEG * 16, self.img)
        self.mu.mem_write(0x463, struct.pack('<H', 0x3D4))  # the BIOS's CRTC port, for sysex_wait
        self.writes = []                # (tick, chip, reg, val) or (tick, 'M', byte)
        self.tick = 0
        self.reg = {}                   # the register each index port selected
        self.kind, self.io = kind, io
        self.mu.hook_add(UC_HOOK_INSN, self.on_out, None, 1, 0, UC_X86_INS_OUT)
        self.mu.hook_add(UC_HOOK_INSN, self.on_in, None, 1, 0, UC_X86_INS_IN)

    def ports(self):
        """index port -> (chip, array) and data port -> index port, as YAMAHA.INC's set_IO_parms
        and update_reg address them"""
        io, k = self.io, self.kind
        if k == 1: return {0x388: (0, 0)}, {0x389: 0x388}
        if k == 2: return {io + 8: (0, 0)}, {io + 9: io + 8}
        if k == 3: return {io: (0, 0), io + 2: (1, 0)}, {io + 1: io, io + 3: io + 2}
        if k == 4: return {0x388: (0, 0), 0x38A: (1, 0)}, {0x389: 0x388, 0x38B: 0x38A}
        if k == 5: return {io: (0, 0), io + 2: (0, 1)}, {io + 1: io, io + 3: io + 2}
        return {}, {}

    def on_out(self, uc, port, size, value, ud):
        idx, dat = self.ports()
        value &= 0xFF
        if self.kind == 6:
            if port == self.io: self.writes.append((self.tick, 'M', value))
            elif port == self.io + 1: self.ack = 1    # a command: the MPU-401 acknowledges it
            return
        if port in idx: self.reg[port] = value
        elif port in dat:
            chip, arr = idx[dat[port]]
            self.writes.append((self.tick, chip, arr << 8 | self.reg.get(dat[port], 0), value))

    def on_in(self, uc, port, size, ud):
        if port == 0x3DA:               # the VGA's status: sysex_wait counts vertical retraces
            self.vbl = getattr(self, 'vbl', 0) ^ 8
            return self.vbl
        if self.kind == 6 and port == self.io + 1:  # the MPU-401's status: ready to take a
            return 0x00 if getattr(self, 'ack', 0) else 0x80   # byte; data only for an ACK
        if self.kind == 6 and port == self.io:
            if getattr(self, 'ack', 0):
                self.ack = 0
                return 0xFE
            return 0
        return 0                        # the FM chip's status: ready, no timer

    def call(self, fn, *words):
        mu = self.mu
        sp = 0xFFF0
        for w in reversed(words):
            sp -= 2
            mu.mem_write(STACK_SEG * 16 + sp, struct.pack('<H', w & 0xFFFF))
        sp -= 4
        mu.mem_write(STACK_SEG * 16 + sp, struct.pack('<HH', 0, RET_SEG))
        for r, v in ((UC_X86_REG_SS, STACK_SEG), (UC_X86_REG_SP, sp), (UC_X86_REG_CS, DRV_SEG),
                     (UC_X86_REG_DS, XMI_SEG), (UC_X86_REG_ES, XMI_SEG)):
            mu.reg_write(r, v)
        mu.emu_start(self.table[fn], RET_SEG * 16, count=50_000_000)
        if (mu.reg_read(UC_X86_REG_CS), mu.reg_read(UC_X86_REG_IP)) != (RET_SEG, 0):
            sys.exit(f'ailcheck.py: driver function {fn:X}h did not return (stopped at '
                     f'{mu.reg_read(UC_X86_REG_CS):04X}:{mu.reg_read(UC_X86_REG_IP):04X})')
        return mu.reg_read(UC_X86_REG_AX), mu.reg_read(UC_X86_REG_DX)


def parse_hw(path):
    out = []
    for line in open(path):
        p = line.split()
        if len(p) == 4 and p[1].startswith('O'):
            out.append((int(p[0]), int(p[1][1:]), int(p[2], 16), int(p[3], 16)))
        elif len(p) == 3 and p[1] == 'M':
            out.append((int(p[0]), 'M', int(p[2], 16)))
    return out


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('ail_log'); ap.add_argument('hw_log')
    ap.add_argument('--driver'); ap.add_argument('--max', type=int, default=20)
    a = ap.parse_args(argv)
    lines = open(a.ail_log).read().split('\n')
    kind = None; io = 0x220; drvh = None
    for l in lines:
        m = re.match(r'C \d+ register_driver (\d+) kind (\d+)', l)
        if m and drvh is None and int(m.group(2)) in KIND_FILE: drvh, kind = int(m.group(1)), int(m.group(2))
        m = re.match(r'C \d+ init_driver (\d+) ([0-9a-f]+)', l)
        if m and int(m.group(1)) == drvh: io = int(m.group(2), 16)
    if kind is None: sys.exit('ailcheck.py: the log has no music driver')
    path = a.driver or os.path.join(DATA, 'SOUND', KIND_FILE[kind])
    d = Driver(path, kind, io)
    print(f'{os.path.basename(path)} (kind {kind}, I/O {io:X}h) against {a.hw_log}')
    data = {}; returned = []; calls = 0
    for l in lines:
        p = l.split()
        if not p: continue
        if p[0] == 'D':
            data[p[1]] = bytes.fromhex(p[3]) if len(p) > 3 else b''
            continue
        if p[0] == 'S':
            d.tick = int(p[1])
            if int(p[2]) == drvh: d.call(FN['serve'])
            continue
        if p[0] != 'C' or len(p) < 4: continue
        d.tick = int(p[1]); fn = p[2]
        if fn in ('startup', 'register_driver', 'register_timer', 'set_timer_frequency', 'start_timer'): continue
        args = p[3:]
        res = None
        if '->' in args: res = int(args[args.index('->') + 1], 16 if fn in ('timbre_request', 'timbre_status') else 10); args = args[:args.index('->')]
        if int(args[0]) != drvh: continue
        calls += 1
        if fn == 'init_driver':
            io_, irq, dma, drq = int(args[1], 16), int(args[2]), int(args[3]), int(args[4])
            r = d.call(FN[fn], 0, io_, irq, dma, drq)
        elif fn == 'shutdown_driver':
            r = d.call(FN['shutdown'], 0, 0, 0)
        elif fn == 'define_timbre_cache':
            r = d.call(FN[fn], 0, 0, CACHE_SEG, int(args[1]))
        elif fn == 'register_sequence':
            d.mu.mem_write(XMI_SEG * 16, data['xmid'])
            r = d.call(FN[fn], 0, 0, XMI_SEG, int(args[1]), 0, STATE_SEG, 0, 0)
        elif fn == 'install_timbre':
            t = data.pop('timbre', b'')
            if t: d.mu.mem_write(TIMB_SEG * 16, t)
            r = d.call(FN[fn], 0, int(args[1]), int(args[2]), 0, TIMB_SEG if t else 0)
        elif fn == 'send_cv':
            r = d.call(FN[fn], 0, int(args[1], 16), int(args[2], 16), int(args[3], 16))
        elif fn in FN:
            r = d.call(FN[fn], 0, *[int(x) for x in args[1:]])
        else:
            continue
        if res is not None:
            got = r[0] & 0xFFFF
            want = res & 0xFFFF
            if fn == 'register_sequence': want = res & 0xFFFF
            returned.append((d.tick, fn, want, got))
    port = parse_hw(a.hw_log)
    if kind != 6: port = [w for w in port if w[1] != 'M']
    real = d.writes
    bad = 0
    for i in range(min(len(port), len(real))):
        if port[i] != real[i]:
            print(f'write {i}: the port wrote {fmt(port[i])}, the driver {fmt(real[i])}')
            for j in range(max(0, i - 3), min(i + 4, len(port), len(real))):
                print(f'   {j}: port {fmt(port[j])}   driver {fmt(real[j])}')
            bad = 1
            break
    if not bad and len(port) != len(real):
        print(f'the port wrote {len(port)} times, the driver {len(real)}; the first {min(len(port), len(real))} agree')
        bad = 1
    rbad = [x for x in returned if x[2] != x[3]]
    for t, fn, want, got in rbad[:a.max]:
        print(f'tick {t}: {fn} returned {want:X} in the port, {got:X} in the driver')
    print(f'{calls} calls, {len(real)} writes by the driver, {len(port)} by the port: ' +
          ('identical' if not bad and not rbad else 'DIFFERENT'))
    return 1 if bad or rbad else 0


def fmt(w):
    if w[1] == 'M': return f'tick {w[0]} MIDI {w[2]:02X}'
    return f'tick {w[0]} chip {w[1]} reg {w[2]:03X} = {w[3]:02X}'


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
