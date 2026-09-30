# Matching UW2 functions with Turbo C++ 1.01

UW2.EXE was built with Borland Turbo C++ 1.01, medium model with 186 instructions. **The switches vary by source file**, so each file names its own in an `/* opts: ... */` comment (match.py defaults to `-mm -1 -G -O -Z`):

- ovr154 (`PLAYER.C`): `-mm -1 -G -O -Y`. `-Y` is overlay code: taking the address of a far function in the same file is a fixup rather than `mov ..,cs`. No `-Z`, so the compiler reloads `mov bx,[player]` and `les bx,[...]` after every store through them.
- The file holding CycleColours (file offset 0x802C4) needed `-Z` to match its register reuse.

So if reloads differ in a way restructuring can't fix, try the file with and without `-Z`.

## Tools

- `python3 tools/match.py src/FILE.C` compiles the file in DOS (headless js-dos through the dos-mcp npm package, about 30 s) and compares every public function with the EXE, named by `/* target: ovr154 */` in the source and looked up in `targets/ovr154.tsv`. It prints MATCH, or how many bytes differ and where.
- `--dis NAME` adds an instruction diff for one function (needs `.venv/bin/python`, which has iced-x86). Jump and call targets are hidden in the diff, so a length difference shows up as the instruction that caused it.
- `--no-build` re-compares the last build.
- Only functions present in the file are reported, so a work file can hold a subset.
- Bytes written by fixups (addresses of globals, call targets, segment values) are masked. So extern names and the addresses of globals don't need to be right yet; their near/far-ness does.
- The IDA listing is `uw2_asm.asm` from [UWReverseEngineering](https://github.com/hankmorgan/UWReverseEngineering), expected at `~/UWReverseEngineering/uw2_asm.asm` (or set `UW2_ASM`) (use `command grep -a` on it; the default grep skips it as binary). Bytes are in `~/UWGOG/UW2/UW2.EXE` (or set `UW2_EXE`), at the segment base in the target table plus the function offset.
- The FM Towns build has the original names and is a second witness for what the code means: `.venv/bin/python tools/fmt.py <name_>` disassembles a named function (32-bit Watcom register-call code) with calls and globals named. DOS is the authority on bytes.

## What the compiler tells you about the source

- **Locals** are laid out downward from `bp-2` in declaration order: the first declared local is nearest `bp`. Initialisers run in declaration order too, so `int found = -1;` declared before an initialised array is stored before the array copy.
- **Register variables**: with `-Z` and default `-r`, the compiler puts up to two `int`-sized locals or parameters in SI and DI. Which ones it picks depends on the declaration and use; try reordering declarations if SI and DI are swapped.
- **Bitfields**: `(w >> 6) & 7` in a condition compiles to `test ax,7`. The original's `and ax,7; or ax,ax` is a bitfield read (`unsigned x:3;`).
- **Parameter types**: an `int` parameter built from a byte shows `mov ah,0`; a `char` parameter doesn't. A `char` return is `mov al,N`; an `int` return is `mov ax,N`.
- **Control flow shape** changes register reuse: a nested `if` against a `continue` gives a different reload of ES or BX. When the instructions agree but the loads differ, try restructuring.
- **Near data**: medium model, so globals are near (`mov bx,[828Ah]`) and DS equals SS (no `ss:` override on stack arrays). Anything reached through `les bx,[...]` is an explicit `far` pointer.
- **Calls to functions in the same file**: a call to a function defined EARLIER in the file compiles to `push cs; call near` (`0E E8`, 4 bytes). A call to one defined LATER compiles to a 5-byte far call, which the linker rewrites to `nop; push cs; call near` (`90 0E E8`); match.py treats that as equal. So if the original shows `0E E8` without the `90`, the callee comes earlier in the file: in a work file, put a stub definition of it above your function. The near call's displacement is not a fixup and is not masked, so with a stub the instructions match but one or two displacement bytes still differ; only the merged file, with the real callee at its true offset, matches every byte.
- **Calls to other files** are far calls (`9A`, or `CD 3F` overlay thunks, both masked).
- **Stack cleanup**: `-G` gives `add sp,N` (or `inc sp; inc sp` for two bytes) after calls, and `push bp; mov bp,sp; sub sp,N` rather than `enter`. `leave` is used at the end.
- **Increment of an indexed byte**: `p->a[i]++` gives `inc byte [bx+N]`. The sequence `mov al,[bx+N]; inc al; push ax; <recompute bx>; pop ax; mov [bx+N],al` is `p->a[i] = p->a[i] + 1;`.
- **Constants keep their spelling**: `memset(buf, -1, n)` pushes `0FFFFh`; the original pushing `0FFh` means the source said `0xFF`.
- **A doubled load** such as `mov di,[bp-8]` twice in a row comes from a redundant assignment in the source, for example `left = count; for (left = count; ...)`.
- **Long arithmetic in an int expression**: `start + (int)(((long)RNG() * count) / 0x8000L)` gives `N_LXMUL@` then `LDIV@`, with the final add done in 16 bits.
- **Constant on the right of `|`**: `(i + 0x11) | 0x400` gives `or ax,400h`; `0x400 | (i + 0x11)` gives `mov dx,400h; or dx,ax`.
- **A bitfield in the top bits of a word** is read with a byte load: `unsigned lo:13; unsigned cls:3;` gives `mov al,[bx+hi]; shr ax,5; and ax,7`.
- **Chained assignment stores right to left**: `*a = *b = K` stores to `*b` first.
- **One register variable, many jobs**: when SI or DI holds unrelated values in turn (a strlen result, then loop counters), the source reused one variable; a third int would have gone on the stack.
- **Testing a far pointer in a loop condition**: the store, reload and `or ax,[bp-6]` sequence comes from the comma form, `for (...; trig = f(...), trig; ...)`. `(trig = f()) != 0` gives the shorter `or ax,dx`.
- **Bitfields versus macros**: a real bitfield reads shift-then-mask (`shr ax,N; and ax,M`), or as a byte load with no `mov ah,0` when it sits at bit 0. Mask-then-shift, including a telltale `shr ax,0`, is a macro written `((w & mask) >> shift)`. The object struct uses both.
- **Argument forms**: a string literal passed as `char far *` is `push ds; push offset`; a local array is `push ss; lea ax; push ax`.
- **Duplicate strings (open question)**: the EXE shares one copy of repeated literals ("trap", "poison trap"), which suggests `-d` (merge duplicate strings). Code bytes don't depend on it; the data segment will.
- **Block-scoped locals share stack slots**: variables declared inside different `{}` blocks can overlap in the frame, so a small `sub sp,N` with overlapping offsets means block-local declarations.
- **A `jmp short $+2`** is the compiler merging identical tails, such as the same call in an `else` at two nesting levels. The source had the duplicate.
- **`!x` against `x == 0`**: on a byte expression promoted to int, `!(...)` gives `or ax,ax` and `(...) == 0` gives `or al,al`.
- **Assignment evaluates the right side first**: `f(i)->w = f(i)->w & M | V;` pushes the value, calls `f` again for the target, then pops and stores. A push/pop around a repeated call is this, not a bitfield.
- **Bitfield writes**: setting a 1-bit field is a byte `and` or `or`; a field spanning two bytes is written with a word `and`/`or`.
- **Reloads without `-Z`**: after any store through `player`, the next statement reloads `mov bx,[player]`. Within one statement, and from an `if` condition into its body, BX is reused.
- **`x += y` against `x = x + y` on bytes**: with a byte target and a `char` right side, `+=` gives `add [bx+N],al`; the load, add, store form is `x = x + y`. With an `int` right side both give load, add, store, which reveals the right side's type.
- **SI goes to the first register candidate declared**: moving a local to be declared first can swap SI and DI between it and a parameter without changing the stack layout.
- **Mask tests on a byte global**: `(g & 0x16) == 0` gives `test byte [g],16h`; `!(g & 0x16)` loads and widens first.
- **Bitfields straddle bytes**: Borland places each field in the 16-bit window starting at the byte holding the next free bit, so a field can be read as `mov ax,[bx+61h]; shr ax,6` from an odd address. Clearing a field whose mask has 0xFF in one byte becomes a byte `and`.
- **Shared call tails**: `if (c) f(0xF); else f(n + 0x15);` compiles to one call with two argument paths joined by a `jmp`. The ternary `f(c ? 0xF : n + 0x15)` does not.
- **Callers reveal return types**: `!f()` compiling to `mov ah,0; neg ax; sbb ax,ax; inc ax` means `f` returns `unsigned char`; a `char` return gives `cbw`.
- **A null far pointer argument** `0L` pushes two `6A 00`.
- Struct field offsets must be exact; use `char padN[...]` to place fields.
- Library helpers (long multiply, divide and shifts) are `N_LXMUL@`, `H_LDIV@` and so on, called as far calls; long arithmetic in C produces them automatically.
