# Layout assumptions

The exact link (`tools/link.py`) proves the sources by rebuilding `UW.EXE`, and for that every byte may sit where the original has it. The modding build (`tools/link.py --mod`, see [LINKING.md](LINKING.md#the-modding-build)) lets sources change size. Anything that holds an address as a plain number, rather than as a fixup the linker fills in, breaks when the thing it points at moves. This file records where such numbers are, how they were looked for, and what still depends on the original layout. It follows UW2Decomp's docs/LAYOUT.md, and the method is Exhume's skills/modding-build.

The scan is Exhume's `tools/addrscan.py` (run it with `~/Exhume/.venv/bin/python3`). It reads the matched objects, the EXE's relocations, symbols.tsv and the IDA listing. It follows the segment registers through the assembly and lists every operand with no fixup that could be an address. Lines it marks with `*` may go through DGROUP or through a segment it cannot name. Each of those was read by hand against the source and `UW1_asm.asm` (below).

`exhume.toml`'s `[layout]` section sets the scan up for UW1:

- `far_data_entries = [51, 76]`: the far data segments, reported as FD51 to FD76. Entry 50 is C0's empty `_FARDATA`. FD52 is seg048 (3963, the graphics library's data), FD58 is seg051 (4723, the 3D renderer's data and object models), and FD72 is seg063 (5624, seg019's data).
- `library_objects = ["seg046"]`: the overlay manager, which the link takes from OVERLAY.LIB rather than from its object.
- `dispatch`: seg003's dispatchers (GRDISP.ASM's `_seg003_5B0B` and the path through `_seg003_5AF9`) are handed a near routine in BP. They call it with DS = ES = seg048 and SS = the word at seg048:558A, which is `dd seg048` in GRDISP's FD52 piece. So DS = ES = SS = FD52.

## What already moves with the link

Every relocation in the EXE is a fixup somewhere: in the objects (verify.py checks each one), and in the extracted modules, where `tools/extract.py` turns each relocated word into `dd NAME` or `dw seg NAME`. Every C global, call, string and pointer initialiser is a fixup too, because Turbo C emits one for each. TLINK writes the overlay stubs, the overlay manager's segment table and the FBOV header. So the numbers that can break are near offsets: DGROUP addresses, offsets into a far data segment, and code offsets, written as numbers in assembly sources, in C, or in the extracted bytes.

## Numbers in the sources

### Changed

None. The scan finds no DGROUP address written as a number in UW1's sources. The four that UW2's audit found are already names here, because UW1's sources were written from UW2's after that audit. Each carries a fixup, so the scan does not list it:

| File | Name | What it is |
| --- | --- | --- |
| ui/GAMESTRN.C | `(_ctype + 1)[...] & _IS_SP` (and `_IS_DIG`) | the C library's `_ctype` |
| sys/STARTUP.ASM | `mov bx,offset DGROUP:__psp` | C0's `__psp` |
| 3d/PGCACHE.ASM | `mov si,offset DGROUP:_Palettes` | LOADGR's object palettes (DS:573E), handed to the image decoders |
| sys/C3DENTRY.ASM | `mov ax,offset DGROUP:_joy_position` | the top of the 0D4h-byte stack that opens SYSENTRY's `_DATA`, used by `_seg019_102B` during a frame |

The compiled C has no `*` line at all: every DGROUP operand in it has a fixup.

### Checked and not addresses

The scan reports 3918 numeric operands, and 128 of them are marked `*`, in 15 objects. All 128 were read by hand:

- **Data decoded as code (6).** These are bytes with no instruction in the listing. PGCACHE +2A3, +3A3 and +4A2 are inside `lightabs`, the shading tables that start at seg004:6950, which the listing has as `db`. SCALEBM +2C5 (two lines) is inside `_cXfer`, the colour tables after `_seg003_1082`'s `retf`. CRITERR +45 is `L0195`, the error code byte `_diskerr` hands back.
- **Arithmetic (7).** MODEX +12 `mov bx,50h` is the row length it multiplies by. AIL +5BE and +5CA `mov bx,20BCh`, and +9A7 and +9B8 `mov bx,2E9Ch`, are divisors for the timer period. AIL +B02 is `call _AIL_register_timer` with BX set from `_find_proc`'s result; the scan carried AX = 67h (the driver function number) through the call. MOUSEDRV +38 `mov bx,64h` is the 100 in `AX * 100 / [0360]`.
- **BIOS and interrupt vectors (5).** KBDINT +48 is `es:[417h]` with ES = 0, the keyboard flags. KEYQUEUE +27 is `ds:[496h]` with DS = 0, also the keyboard flags. MOUSEDRV +D, +10 and +13 are `mov di,0CCh; scasw; scasw` with ES = 0, the int 33h vector.
- **seg019's own data, FD72 (102).** These are JOYPORT (44), KEYQUEUE (43), KBDINT +B2 and +CA (the scan code ring buffer at 0300), MOUSEDRV +3D, SYSINIT +44, VIDSAVE +26 and +30, IMATH +13F, and C3DENTRY +3C1 to +3F7 (8). Their DS comes from SYSENTRY's far entries, which load DS, ES and SS with `seg seg063` before calling the near routines, and from KBDINT's int 9 handler, which loads DS with `lds di,cs:_seg019_590`, whose segment word is `dw seg seg063`. The `?` beside FD72 comes from code the scan can only seed. SYSINIT +44 is `errorexit`, reached from `_init` (DS = seg063) and from `_seg019_240`, which nothing calls. IMATH +13F is the arcsine table read after a `ret` with no label; the same read at +158 has DS known to be FD72. C3DENTRY's `_FDD` and `_1004` copy the private stack with DS = SS. `_102B` calls `_FDD` while SS is still the frame's (seg063), and calls `_1004` after restoring SS from `L0C30`.
- **seg003 on its own data (7).** GRENTRY +AC `mov si,4CCEh` runs after `mov bx,seg _stdat; mov ds,bx` (the frame buffer, 3F4B), so it is the buffer's last pixel. GRENTRY +531 to +564 is `_seg003_F77` reading the span table. Its only outside caller is GRLIBM, a dispatched routine with DS = FD52, which passes SI = 5690h (the table at seg048:5690). The span routines change DS only between `push ds` and `pop ds`.
- **seg001 on seg055 (1).** VSTATS +1B `mov bx,RECS` comes after `mov ax,seg seg055; mov ds,ax`. ES is the caller's DS, and nothing calls `_valloc_stats`.

The listing names UW1's segments without their paragraphs (`seg004`, `seg051`), unlike UW2's (`seg_370D`). So the scan's `--ida` column comes out empty for UW1, and the lines were checked against the listing by `segNNN_OFFSET` labels instead.

## The extracted modules

`tools/extract.py` takes these from your EXE (as of this audit):

| Module | Address | Bytes | Contents |
| --- | --- | --- | --- |
| XFAR | 3963:0008..07D2 (FD52, seg048) | 7CAh | the graphics library's data before POLYFILL's piece |
| XFAR | 4723:0000..2860 (FD58, seg051) | 2860h | the 3D renderer's data before INSTANCE's piece, with the opcode table |
| XFAR | 5624:0000..04D5 (FD72, seg063) | 4D5h | seg019's data before INSTANCE's piece |
| XP004G | 4723:2860..B006, 5624:04D5..0AB0 | 87A6h, 5DBh | INSTANCE's far data, from its first relocated pointer |
| XP004H | 4723:B006..C4D9 | 14D3h | TMAPOPS's far data, from its first relocated pointer |
| XA012, XB021, XA027, XA095, XA105, XA123 | DS:010B, 0232, 03F7, 0DBB, 12B1, 19C7 | 3, 2, 3, 5, 15, 3 | `_DATA` gaps: zeros, and in XA105 the byte values 64h 57h 4Ah 3Dh 30h 23h 16h 09h 01h 00h |
| XB042, XB101, XA119, XB130, XB131 | DS:3638, 4A2E, 5A91, 716E, 7178 | 12h, 2, 39h, 4, 2 | `_BSS` space, with the names placed in it |

The rest (XORDER, XEMPTY16, XSEG019, XF006, XF038, XPULL, the empty overlays) only declare segments or name modules, and hold no bytes.

- **Near DGROUP pointers:** none. The DGROUP gaps are zeros or byte values. In the far data, the IDA listing types no word as a DGROUP offset, and the code that reads these segments runs with DS or ES on them, not on DGROUP.
- **Near code offsets, written as names by the modding build:** FD58 holds the model interpreter's opcode table at seg051:2738, 110 words (opcodes 0 to 0DAh) of offsets into seg004 that INTERP, INSTANCE and CAMERA jump through (`jmp word ptr [bx+2738h]` with DS = seg051). It also holds two copies of the entries for 78h..9Ch at 2814h and 283Ah (NOCLIP.ASM swaps them in), and the seg004 offsets at 2732 and 2734. The listing types these words as `dw offset seg004_...`. `--mod` writes such a table as `dw offset NAME` from the listing, as UW2's does. UW1's listing names its segments without their paragraph, so Exhume finds code segments through map/segments.tsv and seg051 through `[binary] listing_segments` in exhume.toml (`--mod: 147 near code offsets in the extracted data written as names`; with nothing changed the modding build is still UW.EXE).
- **Offsets into their own segment:** the heads hold offsets into their own segment (for example seg051:2736 = 27D0h, a place in seg051 itself). These stay right while the segment's own layout does.

## Still numbers, and what they depend on

These are addresses, but not of DGROUP or of C code, so no C change moves them. They are counted by the scan (`addrscan.py --all`, assembly only):

| Modules | Through | Operands |
| --- | --- | --- |
| seg003 (src/gfx: GRMISC, VIDMODE, GRCORE, POLYFILL, GRENTRY, SCALEBM, GRDISP, the GRLIB modules, WALLMAP, QUADFIT) | FD52 (seg048), some paths FD53 or FD72 | 1115 |
| seg004 (src/3d: SMOOTH, CAMERA, SPHERE, NOCLIP, INTERP, INSTANCE, TMAPOPS, PGCACHE) | FD58 (seg051), seg053, FD72, FD52 | 2622 |
| seg019 (src/sys: CRITERR, CPUTYPE, SYSINIT, JOYPORT, KEYQUEUE, KBDINT, VIDSAVE, MOUSEDRV, SYSLIBL, SYSENTRY, IMATH, C3DENTRY) | FD72 (seg063), a few FD52 and FD58 | 154 |
| seg001 (VALLOC, VSTATS) | seg055, FD52 | 28 |
| seg003, seg004 | each other's code segment as data (`lightabs` at seg004:696E, read by GRENTRY through ES; seg004's reads of seg003) | 10 |

These stay right while each far segment keeps its internal layout. That layout is the head extract.py takes from the EXE, followed by the assembly modules' pieces: POLYFILL, GRENTRY, SCALEBM and GRDISP in FD52; INSTANCE and TMAPOPS (XP004G, XP004H) in FD58; INSTANCE in FD72. The far buffers in src/sys/FARDATA.ASM keep their sizes for the same reason.

The same holds for the code offsets written as numbers. These are the five scaler and span writer offsets at the start of SCALEBM's FD52 piece (`1E90h, 1D91h, 1D91h, 1DDFh, 1DBFh`), the four GRLIBN offsets at the end of GRDISP's (seg048:5E74, `645Ah, 6472h, 6486h, 6490h`), and KBDINT's `mov dx,62Eh` (the int 9 handler's offset in SEG019_TEXT). seg000 to seg004 come before C0 in the image, so nothing in C moves them. seg019's modules come from the second library, after the C library, but its code segment is its own, so C changes do not move offsets inside it either.

So the assembly modules in seg001, seg003, seg004 and seg019, src/sys/FARDATA.ASM, and the extracted heads of FD52, FD58 and FD72 may change only at the same length until these numbers are written as names. The opcode table in FD58 follows seg004's handlers in the modding build, so seg004's routines may change length as far as that table is concerned; the other numbers above still hold them.

The overlay manager's own data (the listing's seg068, with two `dw offset nullsub_7`) comes from OVERLAY.LIB with its code and is not changed by any source.

## Proof

Each edit was made in a copy of the tree (a snapshot that passed `make check`), built with `tools/link.py --mod`, and booted with Exhume's `tools/rungame.mjs --data ~/UWGOG/UW1 --as UW.EXE`. Each run went through the title and main menu, Create Character (by keys: two Up and Enter on the menu, Enter through sex, handedness, class, skills, portrait and difficulty, a typed name, then Yes to keep), and into the 3D view, and then walked and turned. The screenshots were compared with the exact build's at the same points with `tools/pngdiff.py` over the whole screen. The matched tree was never edited.

| Edit | What moves | Result |
| --- | --- | --- |
| A. PATHFIND.C: a new 38-byte initialised array `layout_marker` ("UW1MOD-0 ...") before `PathingOffset`, and an unused far function; UWEDIT.C: `init_world` sets the marker's eighth byte to '1' after `init_strings` | PATHFIND's code (seg006, the first resident segment after C0) and so every resident code segment after it; the far data (PATHFIND5_FAR 55DD to 55DE); DGROUP's paragraph (5AAC to 5AAD); every `_DATA` from DS:AC on (+26h); all of `_BSS`; ovr UWEDIT's code | the memory search finds `UW1MOD-1` (and no `UW1MOD-0`); sex choice, skill choice, name, "Keep this character?" and the 3D view are pixel-identical to the exact build's (0 differing pixels); walking and turning look the same as in the exact build |
| B. A, plus a 334-byte `_BSS` array in PATHFIND.C | all of `_BSS` after DS:245E by a further 14Eh | as A: 0 differing pixels at every point up to and including the 3D view |

After the 3D view the screens differ between any two runs, even two runs of the exact build, by 30,000 to 40,000 pixels after walking: key timing decides how far the player moves. So those points were judged by eye, and they showed the same corridor, the sack and the lighting. Up to the 3D view the runs are deterministic: the rolled attributes (Str 25, Dex 17, Int 18, Vit 35) are the same every time. Nothing differed, so no bisection by `_BSS` padding was needed.

## Limits that still apply

- DGROUP must stay under 64 KB. Its static data ends at DS:79B6 (the last `_BSS` byte is load address 62475h, DS:79B5), and C0 adds the stack and near heap after it.
- `--mod` links the source files the last exact run had. A new file needs a place in the link order (extract.py's lists) first.
- The proofs moved data and resident code. They did not grow an overlay beyond UWEDIT's few bytes, and did not change any assembly module.
