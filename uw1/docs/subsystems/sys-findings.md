# The system layer: candidates for the findings page

Candidates from `src/sys` and `src/lib` for the project's findings page; [sys.md](sys.md) describes the subsystem. Each bug candidate was re-read in the matched source. What the code does is read from the code; the effect in the game is an inference unless it says it was checked.

## Possible bugs

### EMS 4.0 is used without being asked for

- **What happens:** `seg012_B` checks for the EMM driver, its status and the page counts, but not its version; UW2's requires version 4.0 (function 46h). UW1 still names its handle with function 5301h, without checking the result, and maps the workspace with function 5000h (`seg012_BB`), both EMS 4.0 calls.
- **Where:** `seg012_B` and `seg012_BB` in [sys/EMS.C](../../src/sys/EMS.C); `get_workspace` in [sys/TMPALLOC.C](../../src/sys/TMPALLOC.C).
- **Evidence:** code reading against UW2's EMS.C.
- **Confidence:** possible (EMS 3.2 drivers were rare by 1992, and UW2's check may be the fix).
- **Effect:** with an EMS 3.2 driver every `get_workspace` fails (`ws_active` stays 0), so code that needs the 64 KB workspace falls back where it can (SHOWPIC.C uses `farmalloc`) and fails where it cannot. Not tried.

### A failed page frame query leaves the EMS pages allocated

- **What happens:** if function 41h (the page frame) fails after 43h allocated the pages, `seg012_B` returns 0 with the handle allocated. `init_mem` keeps that 0 as its page count, and `free_mem` frees the handle only when the count is above 0, so the pages stay allocated after the game exits.
- **Where:** `seg012_B` in [sys/EMS.C](../../src/sys/EMS.C), `init_mem` and `free_mem` in [sys/TMPALLOC.C](../../src/sys/TMPALLOC.C).
- **Evidence:** code reading.
- **Confidence:** possible; a driver that allocates but cannot report its frame is unlikely.

### The archive block test lets the block number equal the count

- **What happens:** `get_arc` and `put_arc` refuse a block only when `arc->count < blk`, so `blk == count` (one past the last block) is accepted and reads the offset table one entry past its end. `put_arc` also reads `offtab[blk]` before the test.
- **Where:** [sys/ARC.C](../../src/sys/ARC.C).
- **Evidence:** code reading.
- **Confidence:** possible, with no known effect: the callers' block numbers are within the archives' counts (LEV.ARK's per-level blocks).

### `put_arc` reports success after a rewrite whatever happened

- **What happens:** when a block changes length, `put_arc` copies the archive through `_arc.tmp`, renames it back and returns 1 without looking at the copies', the rename's or the reopen's results. It also builds a name with `_` as its last character (`oldname`) and never uses it.
- **Where:** `put_arc` in [sys/ARC.C](../../src/sys/ARC.C).
- **Evidence:** code reading.
- **Confidence:** possible; on a full disk a save could be reported as written.

## Engine findings

- **A breakpoint on every shifted key.** `KEYQUEUE.ASM` runs `int 2` (the NMI vector) whenever a key goes down with Shift held, before translating it; UW2 removed it. Under a normal BIOS the NMI handler checks for a parity error and returns, so it does nothing visible; it is probably a debugging breakpoint left in.
- **The joystick is probed but never read.** `SYSINIT.ASM`'s start-up runs JOYPORT.ASM's axis search (`_30A`), but no C calls SYSENTRY.ASM's joystick readers (`_7CD`, `_809`), UW1 has no JOYSTICK.C and MOUSE.C has no joystick code.
- **`get_arc` has no return statement.** It returns the count read through AX (`AX_RESULT`); the original source evidently fell off the end.
- **PLAYER.DAT's cipher** (`xorread`, `xorwrite`) XORs each byte with a key that starts at the given key plus 3 and grows by 3 a byte, the sequence restarting every 0x50 bytes; UW2 chains each byte to the one before.
- **The overlay manager** is Borland's OVERLAY.LIB, unchanged; only its segment table is shorter.

## Dead code

- `OVR090.C` (ovr090) and `OVR152.C` (ovr152): empty hooks and two that switch to screen mode 16; only their overlay stubs refer to them.
- `JOYPORT.ASM`'s readers and SYSENTRY.ASM's `_7CD` and `_809` (see above).
- `TICKS.ASM`'s far routine at +714, the same increment as `cllbck_tst`, with no caller found.
- `SYSLIBP.ASM`'s `_C10`: called each frame but empty.
- `MISCUTIL.C`'s `dbg_break` raises interrupt 2 and has no caller; `DEBUG.C`'s `dprintf` is empty and `ovr127_0` (Alt+F4) raises the COM1 interrupt.
