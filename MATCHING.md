# Matching UW2 functions with Turbo C++ 1.01

UW2.EXE was built with Borland Turbo C++ 1.01, medium model with 186 instructions. **The switches vary by source file**, so each file names its own in an `/* opts: ... */` comment (match.py defaults to `-mm -1 -G -O -Z`):

- ovr154 (`PLAYER.C`): `-mm -1 -G -O -Y -d`. `-d` (merge duplicate strings) is proven by the data segment: each repeated literal is stored once. `-Y` is overlay code: taking the address of a far function in the same file is a fixup rather than `mov ..,cs`. No `-Z`, so the compiler reloads `mov bx,[player]` and `les bx,[...]` after every store through them.
- seg012 (`SEG012.C`, resident): matches with `-mm -1 -G -O -d`; `-G` is proven, `-O`, `-Z` and `-Y` make no difference in that file.
- seg038 (`SEG038.C`, resident): `-mm -1 -G -O -d`, and no `-Z` is proven (with it `player_get_exp` comes out 8 bytes short).
- seg023 and seg036 (resident): `-mm -1 -G -O -d`; seg023 proves `-G` and `-O` (without either, functions change size).
- The file holding CycleColours (file offset 0x802C4) needed `-Z` to match its register reuse.

So if reloads differ in a way restructuring can't fix, try the file with and without `-Z`.

## Tools

- `python3 tools/match.py src/FILE.C` compiles the file in DOS (headless js-dos through the dos-mcp npm package, about 30 s) and compares every public function with the EXE, named by `/* target: ovr154 */` in the source and looked up in `targets/ovr154.tsv`. It prints MATCH, or how many bytes differ and where.
- `--dis NAME` adds an instruction diff for one function (needs `.venv/bin/python`, which has iced-x86). Jump and call targets are hidden in the diff, so a length difference shows up as the instruction that caused it.
- `--no-build` re-compares the last build.
- Only functions present in the file are reported, so a work file can hold a subset.
- Bytes written by fixups (addresses of globals, call targets, segment values) are masked. So extern names and the addresses of globals don't need to be right yet; their near/far-ness does.
- `python3 tools/verify.py src/FILE.C` checks what match.py masks: fixup targets, overlay entries and the file's `_DATA`; `--update` merges the externs into `symbols.tsv` and refuses an address that already has a different name there, or a name with a different address. Use FM Towns names, and when two files disagree, the FM Towns code decides.
- Target tables come from `python3 tools/targets.py SEGNAME > targets/SEGNAME.tsv`, built from `map/` (verified segment bases and function offsets, original names where the map confirms them). Code segments are byte-aligned, so a file starts at its first function, which may be a few bytes past the segment's paragraph; the table's `org` records that, and absolute code addresses (jump tables) count from the paragraph. The last function's size runs to the first far return after it, so it can be short if the function has an earlier `retf` or a `CB` byte in its code.
- The IDA listing is `uw2_asm.asm` from [UWReverseEngineering](https://github.com/hankmorgan/UWReverseEngineering), expected at `~/UWReverseEngineering/uw2_asm.asm` (or set `UW2_ASM`) (use `command grep -a` on it; the default grep skips it as binary). Bytes are in `~/UWGOG/UW2/UW2.EXE` (or set `UW2_EXE`), at the segment base in the target table plus the function offset.
- The FM Towns build has the original names and is a second witness for what the code means: `.venv/bin/python tools/fmt.py <name_>` disassembles a named function (32-bit Watcom register-call code) with calls and globals named. DOS is the authority on bytes.

## Data

- A file's `_DATA` holds its initialised data in definition order, including the initialisers of local arrays (emitted where the function is), and then the string-literal pool in order of first use. `verify.py` compares it with the EXE, so data the code reads by a fixed DS address may belong to the file itself: look at the bytes around it.
- Static data doesn't appear in the FM Towns symbol table, so an unnamed table inside a file's data range was probably `static`.
- A function's address stored as data in an overlay points at its entry in the overlay's stub table (for example `do_gem` is stub +25h), not at its code.
- Turbo C keeps 32 characters of an identifier: `update_all_critters_whilst_player_snoozes` links as `_update_all_critters_whilst_playe`.
- Two externs resolving to one address means one function was given two names; `verify.py` reports it. The FM Towns calls (`tools/fmt.py`) show the right one.

## What the compiler tells you about the source

- **Locals** are laid out downward from `bp-2` in declaration order: the first declared local is nearest `bp`. Initialisers run in declaration order too, so `int found = -1;` declared before an initialised array is stored before the array copy.
- **Register variables**: with `-Z` and default `-r`, the compiler puts up to two `int`-sized locals or parameters in SI and DI. Which ones it picks depends on the declaration and use; try reordering declarations if SI and DI are swapped.
- **Bitfields**: `(w >> 6) & 7` in a condition compiles to `test ax,7`. The original's `and ax,7; or ax,ax` is a bitfield read (`unsigned x:3;`).
- **Parameter types**: an `int` parameter built from a byte shows `mov ah,0`; a `char` parameter doesn't. A `char` return is `mov al,N`; an `int` return is `mov ax,N`.
- **Control flow shape** changes register reuse: a nested `if` against a `continue` gives a different reload of ES or BX. When the instructions agree but the loads differ, try restructuring.
- **Near data**: medium model, so globals are near (`mov bx,[828Ah]`) and DS equals SS (no `ss:` override on stack arrays). Anything reached through `les bx,[...]` is an explicit `far` pointer.
- **Calls to functions in the same file**: a call to a function defined EARLIER in the file compiles to `push cs; call near` (`0E E8`, 4 bytes). A call to one defined LATER compiles to a 5-byte far call. In an overlay the linker rewrites it to `nop; push cs; call near` (`90 0E E8`), which match.py treats as equal; in resident code it stays a far call to the segment's own paragraph. So if the original shows `0E E8` without the `90`, the callee comes earlier in the file: in a work file, put a stub definition of it above your function. The near call's displacement is not a fixup and is not masked, so with a stub the instructions match but one or two displacement bytes still differ; only the merged file, with the real callee at its true offset, matches every byte.
- **Calls to other files** are far calls (`9A`, or `CD 3F` overlay thunks, both masked).
- **Stack cleanup**: `-G` gives `add sp,N` (or `inc sp; inc sp` for two bytes) after calls, and `push bp; mov bp,sp; sub sp,N` rather than `enter`. `leave` is used at the end.
- **Increment of an indexed byte**: `p->a[i]++` gives `inc byte [bx+N]`. The sequence `mov al,[bx+N]; inc al; push ax; <recompute bx>; pop ax; mov [bx+N],al` is `p->a[i] = p->a[i] + 1;`.
- **Constants keep their spelling**: `memset(buf, -1, n)` pushes `0FFFFh`; the original pushing `0FFh` means the source said `0xFF`.
- **A doubled load** such as `mov di,[bp-8]` twice in a row comes from a redundant assignment in the source, for example `left = count; for (left = count; ...)`.
- **Long arithmetic in an int expression**: `start + (int)(((long)RNG() * count) / 0x8000L)` gives `N_LXMUL@` then `LDIV@`, with the final add done in 16 bits.
- **Constant on the right of `|`**: `(i + 0x11) | 0x400` gives `or ax,400h`; `0x400 | (i + 0x11)` gives `mov dx,400h; or dx,ax`.
- **A bitfield in the top bits of a word** is read with a byte load: `unsigned lo:13; unsigned cls:3;` gives `mov al,[bx+hi]; shr ax,5; and ax,7`.
- **Chained assignment stores right to left**: `*a = *b = K` stores to `*b` first.
- **One register variable, many jobs**: when SI or DI holds unrelated values in turn (a strlen result, then loop counters), the source reused one variable; a third int would usually have gone on the stack.
- **CX as a third register variable**: in a function that makes no calls, a third int local can live in CX (`xor cx,cx ... inc cx`), with or without `register`.
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
- **Which candidate gets SI is not a simple rule**: with a parameter among the candidates, the local declared first got SI (ovr154); with two plain int locals, the one declared second got SI (ovr163). Try both declaration orders; neither changes the stack layout.
- **Mask tests on a byte global**: `(g & 0x16) == 0` gives `test byte [g],16h`; `!(g & 0x16)` loads and widens first.
- **Bitfields straddle bytes**: Borland places each field in the 16-bit window starting at the byte holding the next free bit, so a field can be read as `mov ax,[bx+61h]; shr ax,6` from an odd address. Clearing a field whose mask has 0xFF in one byte becomes a byte `and`.
- **Shared call tails**: `if (c) f(0xF); else f(n + 0x15);` compiles to one call with two argument paths joined by a `jmp`. The ternary `f(c ? 0xF : n + 0x15)` does not.
- **Callers reveal return types**: `!f()` compiling to `mov ah,0; neg ax; sbb ax,ax; inc ax` means `f` returns `unsigned char`; a `char` return gives `cbw`.
- **A null far pointer argument** `0L` pushes two `6A 00`.
- **Decrement-and-test of a byte global**: `if (--g == 0)` gives `mov al,[g]; add al,0FFh; mov [g],al; or al,al`, not `dec byte`.
- **Clearing bits in a global**: `g = g & ~bit;` gives `not ax` then `and dx,ax` through registers; `g |= expr` gives `or [g],ax` directly.
- **Far function-pointer table**: `if (tab[s][i]) tab[s][i]();` recomputes the index for the test (`mov ax,[bx]; or ax,[bx+2]`) and again for `call far [bx+tab]`.
- **An explicit `register`** overrides the default choice: when the original has a loop counter in SI and a parameter in DI, the counter was declared `register int` after the plain locals.
- **Early return against a nested block**: `if (x > K) return;` on an unsigned long gives `cmp hi; jb; jbe +3; jmp; cmp lo; ...`; wrapping the rest in `if (x <= K) {}` orders the branches differently.
- **Comparing a call's result with a byte field**: `if ((v = f()) > p->b)` keeps the result in AX (`mov dl,[bx+N]; mov dh,0; cmp ax,dx`); assigning first and then comparing gives `mov al,[bx+N]; cmp ax,[bp-N]`.
- **Prototypes were not shared everywhere**: seg038 calls `advance` with an `int` argument (`push si`, no conversion) although ovr154 defines `advance(char)`, so each file declared what it called for itself. Declare a callee the way the calling file's bytes show.
- **The for-increment's comma order is kept**: `for (...; i++, p -= step)` gives `inc cx; sub [bp-4],di`; the same update written at the end of the body reverses them.
- **A direct store to a global is reused without `-Z`**: `g = t >> 5; if ((g >> 1) == h)` keeps AL after `mov [g],al`. The forced reloads without `-Z` are after stores through pointers.
- **`~` constants**: `(a & ~0x3F) + (b & ~0x3F)` tested for zero gives `add ax,dx; jne` with no `or ax,ax`; spelling the mask `0xFFC0` adds the `or`, although both emit `and ax,0FFC0h`.
- **Pointer plus index keeps the source's operand order**: `base + (x + (y << 6))` and `base + ((y << 6) + x)` compile differently.
- **`-d` merges string tails too**: `" "` can be the last byte of `"You see "`, and `"\n"` the tail of `".\n"`.
- **A block-local initialised array** (`char num[3] = "00";`) puts its initialiser in `_DATA` where the function is.
- **Static uninitialised data** goes in `_BSS`, which `verify.py` checks for a consistent base (no bytes to compare).
- **`!c` against `c == 0` on a `char` parameter**: `!c` gives `mov al; cbw; or ax,ax`; `c == 0` gives `cmp byte [bp+N],0`.
- **Far pointers compare by offset only** for `<`/`>=` (`mov ax,[bp+N]; cmp ax,[g]`), while `== 0` tests both halves (`or ax,dx`).
- **`x > 0` on an unsigned bitfield** gives `or ax,ax; jbe`, not `je`, so the source said `> 0`.
- **Statement order is kept** even for independent assignments, so two branches that set the same fields in different orders were written that way.
- **`unsigned char` bitfields** are accepted and make the element one byte; a zero test on one gives `and ax,1; or al,al`, where an `unsigned` field gives `or ax,ax`.
- **An early `return` forces a reload**: `if (flag) return;` followed by more code reloads `les bx,[arg]`; `if (!flag) { ... }` reuses ES:BX from the condition.
- **`i++` against `i = i + 1` on a byte local**: `i++` gives `inc byte [bp-N]`; `i = i + 1` gives load, `inc al`, store.
- **A test both arms jump to** after an if/else sat after the else in the source.
- **A doubled mask** such as `and dx,7; and dx,7` survives only if a cast separates the two, as in a macro `((unsigned)(v) & 7) << 13` called with `x & 7`; without the cast the compiler folds them.
- **Assignment inside a condition**: `if ((x -= 16) > 0xD0)` gives `sub [x],10h; mov ax,[x]; cmp ax,...`; as two statements it becomes `cmp word [x],...`.
- **Globals in another file's gap**: unnamed globals that FM Towns keeps as statics may be declared `extern` if their home file isn't matched yet; the bytes are the same, so note them as provisional.
- **`&&` with the success body first**: `if (c >= 0 && c < 6) return c; return -1;` shares one `return -1` tail; the De Morgan form `if (c < 0 || c >= 6) return -1; return c;` duplicates the epilogue and grows.
- **Split chained divisions**: `c = x / 26; c += 3 - y / 15 * 3;` keeps the first quotient in SI; one compound expression pushes and pops it.
- **Shared call tails scale**: an if/else-if chain whose branches all end in the same call compiles to one physical call reached by jumps.
- **A far function address as an argument** is two pushes with separate segment and offset fixups; `verify.py` combines them.
- **Guard then loop**: `if (p == 0) return; while (...)` jumps straight into the loop test; `if (p != 0) { while ... }` adds a `jmp` and is 2 bytes longer.
- **How a guard is written changes the byte test**: in `if (ptr != 0 && g)` a byte global is `cmp byte [g],0`; in `if (ptr == 0 || !g) return;` it is `mov al,[g]; mov ah,0; or ax,ax`.
- **An int also stored as a char stays on the stack** (SI and DI have no byte halves), even when DI is free.
- **The first assignment and the loop step share a call tail**: `obj = f(&a->x); while (obj && c) obj = f(&obj->y);` jumps from the first call into the body's pushes. That is the compiler, not a `goto`.
- **`sizeof` widened to long**: `farmalloc(sizeof(struct Bag))` pushes `6A 00, 6A 0C`. The runtime's `farmalloc`/`farfree` are in seg005.
- **Odd-sized local arrays go after the scalars**: a `char x[11]` is placed after scalar locals declared after it, whatever the order; even-sized arrays keep declaration order. An 11-byte gap can be `char x[10]` plus a padding byte.
- **Shared `return 0`**: several exits jumping to one `mov al,0` before the epilogue come from `if (...) { ...; return 1; } return 0;`, not from early returns, which each get their own `mov al,0; jmp`.
- **`FP_SEG` of a computed near pointer re-evaluates it**: `movedata(..., FP_SEG(d), FP_OFF(d), ...)` with `d = s + strlen(s)` calls `strlen` twice and pushes `ds`; a stack array gives `push ss`.
- **Two explicit `register int`s**: the first declared gets SI.
- **Store and test in one expression**: `ok = (fd = open(...)) != -1` gives `mov [fd],ax; cmp ax,-1`; two statements compare the memory copy.
- **`x = p->bitfield--`** folds the decrement into the masked word, writes through DI, and leaves an unbalanced `push bx` that `leave` hides; that stray push is the sign of this source.
- **Assignment inside a condition** compares the register (`mov di,ax; cmp ax,1`, `or ax,dx` for a far pointer); as two statements it compares the variable.
- **Identical store tails are shared** like call tails: one branch jumps into the other's matching final instructions.
- **DI as scratch**: a free DI may be used for a temporary pointer, pushed and popped, so `push di` in the prologue alone doesn't prove a second register variable.
- **Library inlines**: `abs()` from stdlib.h is `cwd; xor ax,dx; sub ax,dx`; `isdigit()` tests `_ctype` at DS:1BF6 (`test byte [bx+1BF7h],2`).
- **Even alignment after a `char` local**: an int or array declared after a `char` starts at an even offset, leaving a one-byte gap in the frame.
- **A one-case `switch`** compiles to `cmp ax,K; je case; jmp short end` and reloads ES:BX at the case label; `if (x == K)` gives a single `jne`.
- **Stores through a far pointer parameter** don't force a reload without `-Z`, except that an if/else whose arms both store through it makes the next statement reload `les bx`. Stores through near globals always force the reload.
- **An empty-bodied `if`** keeps its test (`mov ah,0; or ax,ax`) with no jump after it.
- **`+=`/`-=` on a word global**: an `unsigned` target gives `sub [g],ax`; an `int` target gives load, subtract, store.
- **`if (c) f(A); else f(B);`** gives two push paths into one call; the ternary `f(c ? A : B)` gives `mov ax,imm; push ax`.
- Struct field offsets must be exact; use `char padN[...]` to place fields.
- Library helpers (long multiply, divide and shifts) are `N_LXMUL@`, `H_LDIV@` and so on, called as far calls; long arithmetic in C produces them automatically.
