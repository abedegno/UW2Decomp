# Data file formats, as the matched code reads and writes them

This page gives the layout of each data file UW2 reads or writes, taken from the matched sources, which compile to the same bytes as `UW2.EXE`. Where UW1 differs, the difference is given beside it, from [UW1Decomp](https://github.com/abedegno/UW1Decomp), which matches `UW.EXE` the same way. [UW1 only](#uw1-only) lists the layouts UW1 has and UW2 does not.

Every field cites the source line that reads or writes it. **measured** means a short script parsed the shipped GOG data (or DOS-written saves) and found the layout as stated; the scripts are not part of the repository. Field names are those of the matched headers. Many are provisional names taken from the code that uses a field; where a meaning is inferred from use rather than named by the code, the table says so.

The community document is "UW-Formats" (`uw-formats.txt`, from the Underworld Adventures project, in the version Hank Morgan's [UWReverseEngineering](https://github.com/hankmorgan/UWReverseEngineering) carries as `uw-formats (Abysmal).txt`). Section numbers below are its. The last part of this page lists where it and the code disagree.

All numbers are little-endian unless a row says otherwise. Offsets are hexadecimal.

- [Archives (.ARK)](#archives-ark)
- [LEV.ARK](#levark)
- [The automap and map notes](#the-automap-and-map-notes)
- [PLAYER.DAT](#playerdat)
- [BABGLOBS.DAT and BGLOBALS.DAT](#babglobsdat-and-bglobalsdat)
- [CNV.ARK](#cnvark)
- [SCD.ARK](#scdark)
- [STRINGS.PAK](#stringspak)
- [OBJECTS.DAT](#objectsdat)
- [COMOBJ.DAT](#comobjdat)
- [TERRAIN.DAT](#terraindat)
- [Pictures: .GR, .TR, .CR, BYT.ARK and .BYT](#pictures-gr-tr-cr-bytark-and-byt)
- [Palettes and light: PALS.DAT, ALLPALS.DAT, LIGHT.DAT, MONO.DAT, SHADES.DAT, XFER.DAT, DL.DAT](#palettes-and-light)
- [Fonts: FONT*.SYS](#fonts-fontsys)
- [Sound and music: SOUNDS.DAT, .VOC, .XMI](#sound-and-music)
- [Smaller files](#smaller-files)
- [Where UW-Formats disagrees with the code](#where-uw-formats-disagrees-with-the-code)

## Archives (.ARK)

UW2 has five archive kinds, named by number in [sys/ARC.C:114](../uw2/src/sys/ARC.C#L114): 1 `LEV.ARK`, 2 `CNV.ARK`, 3 `TEST.ARK` (not shipped), 4 `BYT.ARK`, 5 `SCD.ARK`. `LEV.ARK` and `SCD.ARK` are used from the save directory `SAVE0\`, into which [game/UWEDIT.C:539](../uw2/src/game/UWEDIT.C#L539) copies the pristine ones (the level loader can also be told to read `DATA\`, [MAP.C:85](../uw2/src/map/MAP.C#L85)); `CNV.ARK` and `BYT.ARK` are read from `DATA\`.

| offset | size | field | source |
| --- | --- | --- | --- |
| 0 | word | n, the number of blocks | [ARC.C:172](../uw2/src/sys/ARC.C#L172) |
| 2 | long | read into `arcfile.hdr` and never used; 0 in every shipped archive (measured) | [ARC.C:173](../uw2/src/sys/ARC.C#L173) |
| 6 | n longs | file offset of each block, 0 for an absent block | [ARC.C:233](../uw2/src/sys/ARC.C#L233), [ARC.C:391](../uw2/src/sys/ARC.C#L391) |
| 6 + 4n | n longs | flags: bit 0 compress when writing, bit 1 stored compressed, bit 2 room reserved | [ARC.C:234](../uw2/src/sys/ARC.C#L234), [ARC.C:256](../uw2/src/sys/ARC.C#L256) |
| 6 + 8n | n longs | stored length: the bytes on disk, including the 4-byte length prefix of a compressed block | [ARC.C:261](../uw2/src/sys/ARC.C#L261), [ARC.C:392](../uw2/src/sys/ARC.C#L392) |
| 6 + 12n | n longs | room allocated to the block | [ARC.C:236](../uw2/src/sys/ARC.C#L236), [ARC.C:298](../uw2/src/sys/ARC.C#L298) |

Reading (`get_arc`, [ARC.C:381](../uw2/src/sys/ARC.C#L381)): an absent block reads as length 0. A block with flag bit 1 goes to `ac_unshrink_disk`; otherwise the stored length is read as it is.

Writing (`put_arc`, [ARC.C:203](../uw2/src/sys/ARC.C#L203)): with flag bit 0 the data is compressed first, and if compression saves nothing the block is stored plain and bit 1 cleared ([ARC.C:243](../uw2/src/sys/ARC.C#L243)). Then one of three cases ([ARC.C:264](../uw2/src/sys/ARC.C#L264)):

- it fits: with bit 2, if the new length is at most the room; without bit 2, only if it equals the room exactly. It is written in place.
- the block is new (offset 0): it is appended, with 15% of its length in zero bytes after it as room when bit 2 is set ([ARC.C:290](../uw2/src/sys/ARC.C#L290)).
- otherwise the whole file is copied to `_arc.tmp` without the block's old copy, later offsets are reduced by the old room, and the block is appended at the end with its 15% ([ARC.C:299](../uw2/src/sys/ARC.C#L299)). So blocks do not stay in index order.

The block-number check is `blk > count`, so block number n itself passes and reads the long after the offset table ([ARC.C:231](../uw2/src/sys/ARC.C#L231), [ARC.C:389](../uw2/src/sys/ARC.C#L389)). No caller passes n.

Measured flags in the shipped files: `LEV.ARK` level blocks 7, texture blocks 3 (27 of them) or 1 (15, where compression did not help), automap and notes blocks 7; `CNV.ARK` 3 throughout; `BYT.ARK` 3, and 1 for the two empty blocks 3 and 10 (length 0, offset equal to the next block's or the file's end); `SCD.ARK` 0 throughout, so schedules are never compressed.

### Compression

A compressed block ([sys/ACLZW.C:35](../uw2/src/sys/ACLZW.C#L35)) is a long, the uncompressed length, then an LZSS stream. The algorithm is Haruhiko Okumura's LZSS of 1989 ([sys/LZSS.C:132](../uw2/src/sys/LZSS.C#L132)): a 4096-byte ring buffer, filled with spaces (0x20) in positions 0 to 4077 before decoding, with writing starting at 4078 (N minus F). The stream is groups of a flag byte, low bit first, then eight items: a 1 bit is one literal byte; a 0 bit is two bytes b0, b1 giving a ring position `b0 | (b1 & 0xF0) << 4` and a length `(b1 & 0x0F) + 3` ([LZSS.C:174](../uw2/src/sys/LZSS.C#L174)).

The decoder stops when the compressed bytes run out (the stored length less 4), not when the uncompressed length is reached: the stored long is read and not used ([ACLZW.C:41](../uw2/src/sys/ACLZW.C#L41)). In every compressed block of the shipped `LEV.ARK`, `CNV.ARK` and `BYT.ARK` the long equals what the stream decodes to (measured).

### UW1

UW1's archive ([sys/ARC.C](../uw1/src/sys/ARC.C#L36)) is a word n, then n long offsets at 2. There are no flags, lengths, room or compression. A block runs to the smallest offset greater than its own, whichever block that is, or to the end of the file ([ARC.C:164](../uw1/src/sys/ARC.C#L164)). Writing a block of the same length overwrites it; a new block is appended; a block whose length changed is removed and appended at the end of the file, and the offset table is rewritten when the archive is closed ([ARC.C:102](../uw1/src/sys/ARC.C#L102), [ARC.C:76](../uw1/src/sys/ARC.C#L76)). That is why DOS saves have blocks out of index order.

## LEV.ARK

### Block numbers

| game | count | level map | texture map | animation overlays | automap | notes | source |
| --- | --- | --- | --- | --- | --- | --- | --- |
| UW2 | 320 (measured) | level − 1 | level + 0x4F | inside the level block | level + 0x9F | level + 0xEF | [MAP.C:88](../uw2/src/map/MAP.C#L88), [TEXTMAPS.C:73](../uw2/src/map/TEXTMAPS.C#L73), [AUTOMAP.C:186](../uw2/src/ui/AUTOMAP.C#L186), [AUTOMAP.C:678](../uw2/src/ui/AUTOMAP.C#L678) |
| UW1 | 135 (measured) | level − 1 | level + 0x11 | level + 8 | level + 0x1A | level + 0x23 | [level.h:39](../uw1/src/include/level.h#L39) |

Levels are 1-based. The shipped UW2 `LEV.ARK` holds 42 level blocks and 42 texture blocks, and also the automaps of levels 1, 74 and 79 (blocks 160, 233, 238) and the notes of the same three levels (blocks 240, 313, 318: 14, 51 and 30 notes) (measured). The shipped UW1 `LEV.ARK` holds the 9 level, 9 overlay and 9 texture blocks only; automap and notes blocks appear when a game is saved (measured).

### The level block

`struct LevelBlock`, [include/level.h:16](../uw2/src/include/level.h#L16). UW2's is 0x7E08 bytes ([MAP.C:50](../uw2/src/map/MAP.C#L50), [MAP.C:134](../uw2/src/map/MAP.C#L134)); all 42 shipped blocks decompress to 0x7E08 (measured). UW1's is 0x7C08 and ends at the magic word ([UW1 level.h:17](../uw1/src/include/level.h#L17)); its 9 blocks measure 0x7C08.

| offset | size | field | source |
| --- | --- | --- | --- |
| 0000 | 64 × 64 × 4 | tiles, `struct Tile`, origin at the south-west corner, index x + 64y | [map.h:44](../uw2/src/include/map.h#L44) |
| 4000 | 256 × 0x1B | mobile objects, `struct Object` | [object.h:55](../uw2/src/include/object.h#L55) |
| 5B00 | 768 × 8 | static objects, `struct StaticObj` | [object.h:93](../uw2/src/include/object.h#L93) |
| 7300 | 254 words | free mobile object indexes (objects 2 to 0xFF) | [level.h:20](../uw2/src/include/level.h#L20) |
| 74FC | 768 words | free static object indexes | [level.h:22](../uw2/src/include/level.h#L22) |
| 7AFC | 0x104 bytes | `ActiveMob`: the active mobile objects, a byte each | [level.h:23](../uw2/src/include/level.h#L23) |
| 7C00 | word | nactive: `LastActiveMob − ActiveMob` | [MAP.C:116](../uw2/src/map/MAP.C#L116), [MAP.C:96](../uw2/src/map/MAP.C#L96) |
| 7C02 | word | index of the top entry of the mobile free list (entries less one) | [MAP.C:117](../uw2/src/map/MAP.C#L117), [MAP.C:94](../uw2/src/map/MAP.C#L94) |
| 7C04 | word | the same for the static free list | [MAP.C:119](../uw2/src/map/MAP.C#L119), [MAP.C:95](../uw2/src/map/MAP.C#L95) |
| 7C06 | word | magic 0x7577 ("uw"); anything else is fatal, error A003 | [MAP.C:89](../uw2/src/map/MAP.C#L89) |
| 7C08 | 64 × 6 | UW2 only: animation overlays, `struct Anim` | [level.h:28](../uw2/src/include/level.h#L28) |
| 7D88 | 64 words | UW2 only: timers | [level.h:29](../uw2/src/include/level.h#L29) |

Counts are not stored for the overlays and timers. `Anim_Load` counts overlays up to the first with object index 0 and timers up to the first 0 word; `Anim_Save` zeroes everything past the counts before writing ([MAP.C:147](../uw2/src/map/MAP.C#L147), [MAP.C:169](../uw2/src/map/MAP.C#L169)). `Map_Save` also runs `ObjCrunch`, and if the object lists fail its check asks the player (string 0x96) whether to save anyway ([MAP.C:124](../uw2/src/map/MAP.C#L124)). UW1's `Map_Save` does neither.

UW1 keeps the overlays as their own 0x180-byte block. `Anim_Load` rejects a block of any other length by setting the count to 0 ([UW1 EFFECT.C:485](../uw1/src/obj/EFFECT.C#L485)). `Anim_Save` only zeroes the object index of the entry at the count, so stale entries after it are written back as they were ([UW1 EFFECT.C:502](../uw1/src/obj/EFFECT.C#L502)).

`struct Anim` ([object.h:110](../uw2/src/include/object.h#L110)): word, the object's link (index in bits 6 to 15); int16 frames left, −1 for ever; byte tile x; byte tile y. Both games.

### The tile

`struct Tile`, [include/map.h:44](../uw2/src/include/map.h#L44). Both games.

| bits | field | source and meaning |
| --- | --- | --- |
| word 0, 0–3 | type: 0 solid, 1 open, 2–5 diagonals open to SE, SW, NE, NW, 6–9 slopes rising N, S, E, W | `enum TileType`, [map.h:67](../uw2/src/include/map.h#L67) |
| word 0, 4–7 | floor height; `hgt_val` turns it into a z, 0x40 a step | [MAP.C:38](../uw2/src/map/MAP.C#L38) |
| word 0, 8–9 | `light`: bit 8 is the tile light flag that `PHYSICS.C` tests when the player moves | [map.h:47](../uw2/src/include/map.h#L47) |
| word 0, 10–13 | floor texture, an index into the texture map | [map.h:48](../uw2/src/include/map.h#L48) |
| word 0, 14 | no magic (`anti_magic_p`) | [map.h:40](../uw2/src/include/map.h#L40) |
| word 0, 15 | a door is here | [map.h:41](../uw2/src/include/map.h#L41) |
| word 2, 0–5 | wall texture | `TILE_WALL`, [map.h:54](../uw2/src/include/map.h#L54) |
| word 2, 6–15 | first object in the tile | [map.h:50](../uw2/src/include/map.h#L50) |

### Objects

`struct Object` and its accessors, [include/object.h:55](../uw2/src/include/object.h#L55) to [:309](../uw2/src/include/object.h#L309). The first 8 bytes are all a static object has. Both games have the same layout ([UW1 object.h](../uw1/src/include/object.h#L66)).

| offset | bits | field | meaning (code) |
| --- | --- | --- | --- |
| 00 | 0–8 | item id: major class 6–8, minor 4–5, index 0–3 | [object.h:127](../uw2/src/include/object.h#L127) |
| 00 | 9 | `ID_FLAG9`: a lock is locked; a trigger may be set off by creatures | [object.h:134](../uw2/src/include/object.h#L134) |
| 00 | 10 | `ID_FLAG10`: a book plays a cutscene; a trigger keeps its trap after use | [object.h:137](../uw2/src/include/object.h#L137) |
| 00 | 11 | `ID_FLAG11`: a spell object's charges are shown; a trigger may be set off by the player | [object.h:139](../uw2/src/include/object.h#L139) |
| 00 | 9–12 | for a door, its state | [object.h:133](../uw2/src/include/object.h#L133) |
| 00 | 12 | enchanted | [object.h:142](../uw2/src/include/object.h#L142) |
| 00 | 13 | UW-Formats' doordir; `chkTenacious` also keeps an object with it set from being culled | [object.h:143](../uw2/src/include/object.h#L143) |
| 00 | 14 | invisible, not drawn | [object.h:145](../uw2/src/include/object.h#L145) |
| 00 | 15 | is_quant: the link field is a quantity or special property | [object.h:146](../uw2/src/include/object.h#L146) |
| 02 | 0–6, 7–9, 10–12, 13–15 | z, heading in eighths, fine y, fine x | [object.h:149](../uw2/src/include/object.h#L149) |
| 04 | 0–5, 6–15 | quality; next object | [object.h:63](../uw2/src/include/object.h#L63) |
| 06 | 0–5, 6–15 | owner; link (below 0x200 a quantity when is_quant, from 0x200 a special property) | [object.h:69](../uw2/src/include/object.h#L69), [object.h:155](../uw2/src/include/object.h#L155) |
| 08 | byte | hit points | [object.h:73](../uw2/src/include/object.h#L73) |
| 09 | byte | heading | [object.h:74](../uw2/src/include/object.h#L74) |
| 0A | 0–3, 4–6, 7 | frame counter; terrain under it; keeps to itself (names from use) | [object.h:215](../uw2/src/include/object.h#L215) |
| 0B | 0–3, 4–11, 12–15 | goal; goal target; animation frame. A missile's fine x | [object.h:223](../uw2/src/include/object.h#L223) |
| 0D | 0–3, 4–7, 8, 9, 10, 12, 13, 14–15 | saved goal (UW-Formats' npc_level); target height; summoned; no healing; powerful; inventory generated; talked to; attitude. A missile's fine y. Bit 11 unknown | [object.h:230](../uw2/src/include/object.h#L230) |
| 0F | 0–5, 6–11, 12–15 | destination tile x, y; attack frame. A missile's fine z | [object.h:252](../uw2/src/include/object.h#L252) |
| 11 | byte | damage taken | [object.h:260](../uw2/src/include/object.h#L260) |
| 12 | byte | last hit | [object.h:82](../uw2/src/include/object.h#L82) |
| 13 | 0–6, 7 | speed; gravity | [object.h:263](../uw2/src/include/object.h#L263) |
| 14 | 0–2, 3–7 | rate; pitch (a missile's) | [object.h:267](../uw2/src/include/object.h#L267) |
| 15 | 0–5, 6, 7 | animation sequence; two flags of unknown meaning | [object.h:272](../uw2/src/include/object.h#L272) |
| 16 | 0–3, 4–9, 10–15 | path index; home y; home x | [object.h:279](../uw2/src/include/object.h#L279) |
| 18 | 0–4, 5–7 | fine heading; flags | [object.h:286](../uw2/src/include/object.h#L286) |
| 19 | 0, 1, 2–3, 4, 5, 6, 7 | flags; spell being cast; flags; on the player's side; fed | [object.h:294](../uw2/src/include/object.h#L294) |
| 1A | byte | whoami, the conversation | [object.h:89](../uw2/src/include/object.h#L89) |

The names of the fields from 0x0A on come from the code that uses them; where the header says "unknown" or gives only a byte and bit, nothing in the code explains it.

The free lists work as UW-Formats 4.3 says. `Obj_Alloc` takes the entry at the top and moves the top down ([obj/OBJECTS.C:230](../uw2/src/obj/OBJECTS.C#L230)); `Obj_Free` moves it up and writes the freed index there ([OBJECTS.C:247](../uw2/src/obj/OBJECTS.C#L247)). So entries 0 to top are free, and the stored word for an empty list is −1.

### The texture map

| game | length | layout | source |
| --- | --- | --- | --- |
| UW2 | 0x86 | 64 words, texture numbers in `T64.TR`; then 3 words holding the six door textures, low byte first | [TEXTMAPS.C:64](../uw2/src/map/TEXTMAPS.C#L64), [TEXTMAPS.C:93](../uw2/src/map/TEXTMAPS.C#L93) |
| UW1 | 0x7A | 48 wall words (`W64.TR`), 10 floor words (`F32.TR`), then 3 words of door textures | [UW1 TEXTMAPS.C:118](../uw1/src/map/TEXTMAPS.C#L118) |

A block of another length makes `Txm_Load` return 0 but its buffer is used anyway ([TEXTMAPS.C:73](../uw2/src/map/TEXTMAPS.C#L73)). All shipped texture blocks are 0x86 (UW2) and 0x7A (UW1) (measured).

## The automap and map notes

The automap block is 0x1000 bytes, a byte per tile in tile order ([AUTOMAP.C:186](../uw2/src/ui/AUTOMAP.C#L186)). `GetAutoMapLevel` accepts a length of 0 or 0x1000 ([AUTOMAP.C:202](../uw2/src/ui/AUTOMAP.C#L202)). The byte is written by the renderer as it draws tiles, and only while `player->automap` is set ([3d/GRIDDB.C:624](../uw2/src/3d/GRIDDB.C#L624)).

| bits | UW2 | UW1 |
| --- | --- | --- |
| 0–3 | 0 never seen; 1–9 the tile type, seen; 0xA–0xF a tile in view range but not visible, from `tile_mapcode`: solid 0xA, open and slopes 0xB, diagonals 0xC–0xF ([GRIDDB.C:150](../uw2/src/3d/GRIDDB.C#L150), [GRIDDB.C:531](../uw2/src/3d/GRIDDB.C#L531)) | the same ([UW1 GRIDDB.C](../uw1/src/3d/GRIDDB.C#L156)) |
| 4–5 | kind, from `curautocode`: 1 a door ([DRAWOBJ.C:501](../uw2/src/3d/DRAWOBJ.C#L501)), 2 a bridge (model 2, item 0x164) drawn with a `TMOBJ.GR` texture ([DRAWOBJ.C:357](../uw2/src/3d/DRAWOBJ.C#L357)), 3 a wall texture object whose texture's terrain is 3 or 4, stairs ([DRAWOBJ.C:253](../uw2/src/3d/DRAWOBJ.C#L253)); written at [GRIDDB.C:621](../uw2/src/3d/GRIDDB.C#L621) | the floor's terrain word as it is, so 0x10 water and 0x20 lava ([UW1 GRIDDB.C:562](../uw1/src/3d/GRIDDB.C#L562)) |
| 6–7 | the floor texture's terrain class, `TxmTerr & 0xC0`: 0x40 water, 0x80 lava, 0xC0 ice ([GRIDDB.C:542](../uw2/src/3d/GRIDDB.C#L542); class names measured, see [TERRAIN.DAT](#terraindat)) | the kind, `curautocode << 6` ([UW1 GRIDDB.C:641](../uw1/src/3d/GRIDDB.C#L641)) |

So the two games swap the nibbles' halves. The map screen reads UW2's byte at [AUTOMAP.C:347](../uw2/src/ui/AUTOMAP.C#L347) and UW1's at [UW1 AUTOMAP.C:288](../uw1/src/ui/AUTOMAP.C#L288). Kinds and classes combine: the shipped level 79 automap has a tile of 0xF_, stairs on ice (measured).

A map note is `struct ATM`, 0x36 bytes ([include/ui.h:293](../uw2/src/include/ui.h#L293)): text, 0x32 bytes, zero-terminated; int16 x; int16 y, screen coordinates. An erased note has x −1 until the notes are saved. The notes block is the notes one after another; the count is the block length divided by 0x36, at most 100 ([AUTOMAP.C:689](../uw2/src/ui/AUTOMAP.C#L689)). Typing accepts 0x20 to 0x7A, upper-cased ([AUTOMAP.C:560](../uw2/src/ui/AUTOMAP.C#L560)), up to 46 characters ([AUTOMAP.C:480](../uw2/src/ui/AUTOMAP.C#L480)). UW1 is the same ([UW1 AUTOMAP.C:587](../uw1/src/ui/AUTOMAP.C#L587)).

`SaveTheWords` drops erased notes by shifting the rest down, but it moves on to the next index after each shift, so of two erased notes in a row the second is kept and written with x −1. The shift also copies 100 − i entries starting from entry i + 1, which reads one entry past the end of the array ([AUTOMAP.C:671](../uw2/src/ui/AUTOMAP.C#L671)). Both games.

## PLAYER.DAT

Written to the save directory by `SavePlayerInv` and read by `RestorePlayerInv` ([inv/INVSAVE.C:94](../uw2/src/inv/INVSAVE.C#L94), [INVSAVE.C:243](../uw2/src/inv/INVSAVE.C#L243)). No code opens `DATA\PLAYER.DAT`: the only path built is the save directory plus `player.dat` ([INVSAVE.C:107](../uw2/src/inv/INVSAVE.C#L107), [INVSAVE.C:263](../uw2/src/inv/INVSAVE.C#L263)). The 301-byte `DATA\PLAYER.DAT` shipped with both games (the same file in each) fits neither save layout (measured).

| file offset | UW2 | UW1 | source |
| --- | --- | --- | --- |
| 0 | key byte: the first letter of the name xor 0xAA | the same | [PLAYDATA.C:86](../uw2/src/game/PLAYDATA.C#L86), [UW1 PLAYDATA.C:107](../uw1/src/game/PLAYDATA.C#L107) |
| 1 | `struct Player`, 0x37D bytes, enciphered | `struct Player`, 0xD2 bytes, enciphered | [PLAYDATA.C:101](../uw2/src/game/PLAYDATA.C#L101), [UW1 PLAYDATA.C:122](../uw1/src/game/PLAYDATA.C#L122) |
| 37E / D3 | word: the number of saved objects plus one, plain | the same | [INVSAVE.C:111](../uw2/src/inv/INVSAVE.C#L111), [UW1 INVSAVE.C:101](../uw1/src/inv/INVSAVE.C#L101) |
| 380 / D5 | the workspace, (count word) × 8 + 0x5B bytes, plain | the same | [INVSAVE.C:112](../uw2/src/inv/INVSAVE.C#L112) |

The workspace ([INVSAVE.C:61](../uw2/src/inv/INVSAVE.C#L61)):

| offset | size | field |
| --- | --- | --- |
| 00 | 0x1B | the player object; its link field (0x06) is the head of the inventory list, its hp byte (0x08) the current vitality |
| 1B | 8 | the object held on the cursor, if any |
| 23 | 0x1C words | inventory slots; slots 0 to 0x12 are used |
| 5B | 8 per object | the saved objects, numbered from 1 (slot 0 unused), with links renumbered to these numbers |

So in UW2 the inventory head is at file offset 0x386, the player's hit points at 0x388, the slots at 0x3A3 and saved object 1 at 0x3E3; in UW1 0xDB, 0xDD, 0xF8 and 0x138.

Measured: 21 UW1 saves written by DOS, and 19 UW2 saves written by UW2Decomp's native build of the same C in its replay runs, all have this exact length, 1 + record + 2 + 0x5B + 8 × count, and a key byte equal to the first letter of the name xor 0xAA.

Two consequences of the order of loading. `read_player_data` copies the record's health byte (0x35) to the player object ([PLAYDATA.C:112](../uw2/src/game/PLAYDATA.C#L112)), and then `getPlayerInvCopy` overwrites the whole player object from the workspace ([INVSAVE.C:229](../uw2/src/inv/INVSAVE.C#L229)): the vitality the game ends up with is the workspace's hp byte. The flag saying a cursor object was saved is not in the file and is cleared when restoring from a file ([INVSAVE.C:261](../uw2/src/inv/INVSAVE.C#L261)), so a held object in the file is ignored. Both games.

### The cipher

UW2 ([sys/MISCUTIL.C:281](../uw2/src/sys/MISCUTIL.C#L281)): a table of 0x50 bytes is built from the key: k = key + 0x71; for i = 0x3C..0x4F, k += i + 2, t[i] = k; for i = 0..0x4F, k += 6, t[i] = k; for i = 0..0xF, k += 7, t[5i] = k; for i = 0..3, k += 0x29, t[12i] = k; for i = 0..10, k += 0x49, t[7i] = k. The record is then processed in blocks of 0x50 bytes, the chain restarting in each block: out[0] = in[0] xor t[0], and out[i] = in[i] xor (t[i] + out[i−1] + in[i−1]) ([MISCUTIL.C:292](../uw2/src/sys/MISCUTIL.C#L292)). Because the sum takes one plain and one enciphered byte either way, the same routine enciphers and deciphers. A block shorter than 2 bytes is left as it is ([MISCUTIL.C:297](../uw2/src/sys/MISCUTIL.C#L297)); the record's last block is 13 bytes, so this never happens.

UW1 ([UW1 MISCUTIL.C:268](../uw1/src/sys/MISCUTIL.C#L268)): t[i] = key + 3(i + 1), and byte i of each 0x50-byte block is xored with t[i]; the sequence restarts every 0x50 bytes, so at record offsets 0x50 and 0xA0.

### The record

UW2's `struct Player` is in [include/player.h:20](../uw2/src/include/player.h#L20), UW1's in [UW1 player.h:26](../uw1/src/include/player.h#L26). The two agree to 0x5D (name, 0x1E bytes; attributes; 20 skills; vitality, mana, hunger, fatigue; level; active spells; runes; weight; experience in tenths; skill points; position, heading and level saved at 0x54 to 0x5D) and then differ. The headers carry every field the code is known to use, with the source of each name; offsets there are record offsets, one less than file offsets.

## BABGLOBS.DAT and BGLOBALS.DAT

`BABGLOBS.DAT` (in `DATA\`) is a list of word pairs: conversation slot, number of private global words. `BGLOBALS.DAT` (in the save directory) is the same with that many words after each pair. A new game copies the first to the second with the words zeroed ([conv/BABL.C:285](../uw2/src/conv/BABL.C#L285)). The records are in slot order: lookup stops at the first slot greater than the conversation's ([BABL.C:321](../uw2/src/conv/BABL.C#L321)). At most the conversation's `babl_nvars` words are read or written back ([BABL.C:312](../uw2/src/conv/BABL.C#L312), [BABL.C:337](../uw2/src/conv/BABL.C#L337)). UW1 is the same ([UW1 BABL.C:300](../uw1/src/conv/BABL.C#L300)).

## CNV.ARK

One block per conversation slot. The slot is the critter's whoami, or for whoami 0, 0x100 plus the low 6 bits of its item id ([conv/CONVERSE.C:134](../uw2/src/conv/CONVERSE.C#L134), [CONVERSE.C:112](../uw2/src/conv/CONVERSE.C#L112)); a slot with no block gets "You get no response" ([CONVERSE.C:136](../uw2/src/conv/CONVERSE.C#L136)). UW2's `CNV.ARK` has 102 blocks, none at or above 0x100, so a UW2 critter with whoami 0 never talks; UW1's has 97, of which 15 are generic conversations from 0x106 to 0x13A (measured).

The block header (`DoReadHeader`, [BABL.C:692](../uw2/src/conv/BABL.C#L692); UW1 [BABL.C:840](../uw1/src/conv/BABL.C#L840)):

| offset | size | field |
| --- | --- | --- |
| 00 | 4 | skipped; 28 08 00 00 in all 199 blocks of both games (measured) |
| 04 | long | code size in words |
| 08 | word | kept in `hdr_word` and not used; 0 in every block (measured) |
| 0A | word | the string block of the conversation's strings |
| 0C | word | `babl_nvars`, the variable words before the stack |
| 0E | word | number of imports |
| 10 | | imports: word name length, the name, then words index (variable address or function number), count, kind (0x111 a function, anything else a variable), type |

Measured in both games: count is 1 and kind 0x10F or 0x111 in every import. The code follows the imports.

## SCD.ARK

UW2 only. 16 blocks, one schedule per X clock ([event/SCHEDULE.C](../uw2/src/event/SCHEDULE.C#L5)), read and written whole ([SCHEDULE.C:126](../uw2/src/event/SCHEDULE.C#L126), [SCHEDULE.C:144](../uw2/src/event/SCHEDULE.C#L144)). Block layout (`struct SCDWork` from `rows` on, [include/event.h:191](../uw2/src/include/event.h#L191)):

| offset | size | field |
| --- | --- | --- |
| 000 | word | number of rows |
| 002 | byte | block number; set when loaded, 0 in the shipped file (measured) |
| 003 | byte | unused |
| 004 | 80 × 4 | a clock per level, indexed by the 1-based level number: word time reached, word next row to run |
| 144 | rows × 16 | the rows, sorted by time |

The written length is rows × 16 + 0x144, and every shipped block measures exactly that (measured; blocks 0, 1, 14 and 15 have 51, 78, 6 and 84 rows, the rest none). Rows are in time order (measured).

A row (`struct SCDRow`, [event.h:61](../uw2/src/include/event.h#L61)): word time; byte level (0xFF any level, 0xF6 + n world n); byte once (delete after running); signed byte event (negative: disabled; 12 and up: unknown); 11 bytes of parameters by event (`union SCDParams`, [event.h:21](../uw2/src/include/event.h#L21)). Events ([SCDEVENT.C:515](../uw2/src/event/SCDEVENT.C#L515)): 0 nothing, 1 change goal, 2 teleport, 3 kill, 4 set a quest bit, 5 fire triggers on a square, 6 nothing, 7 special case by its hack byte, 8 attitude, 9 set a variable, 10 test variables, 11 remove. A row applies when its level matches ([SCDEVENT.C:492](../uw2/src/event/SCDEVENT.C#L492)). The shipped rows use events −11 to 11; two have time 0xFFFF (measured).

## STRINGS.PAK

Both games ([ui/GAMESTRN.C:218](../uw2/src/ui/GAMESTRN.C#L218), [GAMESTRN.C:242](../uw2/src/ui/GAMESTRN.C#L242)): word, the number of Huffman nodes; the nodes, 4 bytes each (character, a byte the code does not read, left child, right child; a node whose left child is 0xFF is a leaf; the root is the last node); word, the number of blocks; per block, word block number and long file offset. A block is a word count, then a word per string, its offset from the end of this table. A string is decoded high bit first, 1 to the right, until '|', 0xFF or 512 characters ([GAMESTRN.C:273](../uw2/src/ui/GAMESTRN.C#L273), [GAMESTRN.C:302](../uw2/src/ui/GAMESTRN.C#L302)). This agrees with UW-Formats 5; a decoder written from the code reads both games' files (measured).

## OBJECTS.DAT

Read once at start-up by `init_objects`, each major class's loader in turn ([obj/OBJCLASS.C:37](../uw2/src/obj/OBJCLASS.C#L37)). Both games, 0xDE2 bytes (measured).

| offset | entries × size | table | source |
| --- | --- | --- | --- |
| 000 | word | header, read and ignored | [OBJCLASS.C:45](../uw2/src/obj/OBJCLASS.C#L45) |
| 002 | 16 × 8 | melee weapons, `struct Weapon`: damage by swing (slash, bash, stab), then four bytes named from use min_charge, speed, max_charge, skill, then durability | [HACK.C:24](../uw2/src/obj/HACK.C#L24), [object.h:389](../uw2/src/include/object.h#L389) |
| 082 | 16 × 3 | missiles and missile weapons, `struct MissileInfo`: damage, type, signed ammo. For a launcher, ammo is the ammunition's item id less 0x10 ([COMBAT.C:555](../uw2/src/combat/COMBAT.C#L555)); ammunition entries hold other values in it, and the loot code tests for 0xC0 ([TREASURE.C:168](../uw2/src/obj/TREASURE.C#L168)) | [HACK.C:25](../uw2/src/obj/HACK.C#L25), [object.h:382](../uw2/src/include/object.h#L382) |
| 0B2 | 32 × 4 | armour, `struct Armour`: protection, durability, a byte, category | [HACK.C:26](../uw2/src/obj/HACK.C#L26), [object.h:400](../uw2/src/include/object.h#L400) |
| 132 | 64 × 0x30 | creatures, `struct Creature` | [CREATURE.C:43](../uw2/src/critter/CREATURE.C#L43), [critter.h:21](../uw2/src/include/critter.h#L21) |
| D32 | 16 × 3 | containers, `struct Container`: capacity; int16 mask, an item id below 0x200, 0x200 + kind (0 runes, 1 missiles, 2 scrolls, 3 food, 4 keys), or −1 for anything | [MISC.C:24](../uw2/src/obj/MISC.C#L24), [object.h:416](../uw2/src/include/object.h#L416) |
| D62 | 16 × 2 | lights: byte 0 the burn rate, 0 never burns out ([PLAYTIME.C:192](../uw2/src/game/PLAYTIME.C#L192)); byte 1 the brightness ([PLAYDATA.C:395](../uw2/src/game/PLAYDATA.C#L395)) | [MISC.C:25](../uw2/src/obj/MISC.C#L25) |
| D82 | 16 × 1 | food: the value `UseFood` takes as nutrition | [MISC.C:26](../uw2/src/obj/MISC.C#L26), [USEITEMS.C:218](../uw2/src/obj/USEITEMS.C#L218) |
| D92 | 16 × 1 | trigger modes, by trigger index (0 move, 2 pick up, 4 use, 5 look, ... 0xF pressure release) | [TRIGGER.C:64](../uw2/src/event/TRIGGER.C#L64) |
| DA2 | 16 × 4 | animation classes, `struct AnimClass`: word flags (1 cycle, 2 random, 4 door, 0x20 remove at the end, 0x80 finish the motion first), start frame, frame count | [ANIMOBJ.C:18](../uw2/src/obj/ANIMOBJ.C#L18), [object.h:118](../uw2/src/include/object.h#L118) |

Measured examples: the UW2 container table holds masks 0x0204 (a key ring), 0x0201 (a quiver), 0x0203 (food) and 0x0200 (the rune bag); the launcher entries for items 0x18 to 0x1A have ammo bytes 0, 2 and 1.

The creature record's names are provisional, from use; [include/critter.h:21](../uw2/src/include/critter.h#L21) gives each.

## COMOBJ.DAT

A word, read and ignored, then 512 records of 11 bytes, read as one 0x1600-byte block ([OBJCLASS.C:54](../uw2/src/obj/OBJCLASS.C#L54)). `struct ComObj`, [include/object.h:19](../uw2/src/include/object.h#L19); both games, 5634 bytes (measured).

The names are the header's, provisional except where a use is cited.

| offset | bits | field |
| --- | --- | --- |
| 00 | byte | height |
| 01 | 0–2, 3, 4–15 | radius; animated ([GAMESORT.C:266](../uw2/src/3d/GAMESORT.C#L266)); mass in tenths of a stone |
| 03 | 1, 3, 5, 6–7 | solid ([MOTION.C:571](../uw2/src/motion/MOTION.C#L571)); no hit, which also turns gravity off for a thrown object ([OBJPHYS.C:389](../uw2/src/motion/OBJPHYS.C#L389)); pick up; stacking, where 1 and 3 never merge ([INVPANEL.C:698](../uw2/src/inv/INVPANEL.C#L698)). Bits 0, 2 and 4 are unnamed |
| 04 | word | value |
| 06 | 0, 1, 2–3, 4, 5–8 | touch and usable ([OBJPHYS.C:163](../uw2/src/motion/OBJPHYS.C#L163)); quality class (× 6 + quality indexes string block 5); light; bounce |
| 07 | 1–4, 5, 7 | fate, what happens when it lands ([OBJPHYS.C:440](../uw2/src/motion/OBJPHYS.C#L440)); pickable; can have an owner. Bit 6 unnamed |
| 08 | byte | resistances |
| 09 | 0–1, 2–5 | render: 0 a sprite, 1 a critter, 2 a 3D model or door, 3 a texture-mapped model ([DRAWOBJ.C:7](../uw2/src/3d/DRAWOBJ.C#L7)); tenacity, how far away it may be culled ([OBJECTS.C:140](../uw2/src/obj/OBJECTS.C#L140)). Bits 6 and 7 unnamed |
| 0A | 0–3, 4 | quality type, a group of 6 strings in block 4; printable look description. Bits 5 to 7 unnamed |

## TERRAIN.DAT

UW2: a word per `T64.TR` texture, 256 words; `Load_Terrains` reads the word at twice the texture number for each of the level's 64 textures ([map/TEXTMAPS.C:121](../uw2/src/map/TEXTMAPS.C#L121)). UW1: walls from offset 0 and floors from 0x200, 512 words ([UW1 TEXTMAPS.C:232](../uw1/src/map/TEXTMAPS.C#L232)).

UW2's word, as the code reads it:

| bits | use |
| --- | --- |
| 0–2 | `& 7`: 3 or 4 make a wall texture object an automap stairs mark ([DRAWOBJ.C:251](../uw2/src/3d/DRAWOBJ.C#L251)); 5 makes looking at it look down a shaft ([obj/LOOK.C:290](../uw2/src/obj/LOOK.C#L290)) |
| 3–5 | on ice, slipperiness, 7 meaning not slippery ([motion/PHYSICS.C:405](../uw2/src/motion/PHYSICS.C#L405)); in water, the current's direction plus one ([PHYSICS.C:442](../uw2/src/motion/PHYSICS.C#L442)). The meanings are inferred from that use |
| 6–7 | class: 1 water, 2 lava, 3 ice (`TERR_CLASS`, [map.h:94](../uw2/src/include/map.h#L94)) |

The class names are measured: matching each texture's terrain word with its description in string block 10, the 0x40 class are "water", "a swamp", "mud" and "a waterfall", the 0x80 class "a lavafall" and lava walls, the 0xC0 class "an ice wall", "a cracked ice wall" and slick walls. Low bits 3 and 4 are on "stairs up"/"stairs down", slopes and ladders; 5 on "a window" and "blackness". Floors in the shipped levels use 0x40, 0x48, 0x50, 0x58, 0x60 (four "water" textures with currents), 0x80, 0xC0, 0xE8 and 0xF8, and never 0xC8 or 0xD8 (measured).

UW1's values are the ones UW-Formats 4.8 lists, measured the same way: 0x10 water and 0x20 lava on floors; 2 ankh, 3 and 4 stairs, 5 pipe, 6 grating, 7 drain, 8 the chained princess, 9 window, 0xA tapestry, 0xB the textured door, on walls.

## Pictures: .GR, .TR, .CR, BYT.ARK and .BYT

`_ld_open` ([gfx/LOADGR.C:101](../uw2/src/gfx/LOADGR.C#L101)):

| offset | size | field |
| --- | --- | --- |
| 0 | byte | type: must equal 1 for `.GR`, 2 for `.TR`, 3 for `.CR`, else the file is refused ([LOADGR.C:112](../uw2/src/gfx/LOADGR.C#L112)) |
| 1 | byte | `.TR` only: the texture's edge in pixels ([LOADGR.C:114](../uw2/src/gfx/LOADGR.C#L114)) |
| | word | count of pictures ([LOADGR.C:116](../uw2/src/gfx/LOADGR.C#L116)) |
| | byte + 32 per palette | `.CR` only: number of auxiliary palettes, then the palettes ([LOADGR.C:84](../uw2/src/gfx/LOADGR.C#L84)) |
| | longs | picture offsets. The code reads count + 1 longs ([LOADGR.C:125](../uw2/src/gfx/LOADGR.C#L125)) and never uses the last |

A picture's size is the next offset less its own, and the last picture's runs to the end of the file ([LOADGR.C:148](../uw2/src/gfx/LOADGR.C#L148)). UW1 checks the `.TR` type byte the same way, fatally ([UW1 TEXTMAPS.C:207](../uw1/src/map/TEXTMAPS.C#L207)).

A picture (`struct Bitmap`, [include/gfx.h:127](../uw2/src/include/gfx.h#L127)): byte type (4 8-bit, 8 4-bit run-length, 0xA 4-bit), byte width, byte height; for type 4, a word size and the data; for the 4-bit types, a byte auxiliary palette, a word size in nibbles, and the data.

`BYT.ARK` (UW2) holds 320 × 200 8-bit screens, 64000 bytes each (measured). The blocks are named by their callers ([gfx/SHOWPIC.C:6](../uw2/src/gfx/SHOWPIC.C#L6)), with the palette each is shown with:

| block | use | palette | source |
| --- | --- | --- | --- |
| 0 | automap background | 1 | [AUTOMAP.C:754](../uw2/src/ui/AUTOMAP.C#L754), [AUTOMAP.C:768](../uw2/src/ui/AUTOMAP.C#L768) |
| 1 | character creation | 3 | [CHARGEN.C:716](../uw2/src/game/CHARGEN.C#L716), [CHARGEN.C:717](../uw2/src/game/CHARGEN.C#L717) |
| 2 | conversation screen | the current one | [CONVERSE.C:177](../uw2/src/conv/CONVERSE.C#L177) |
| 3, 10 | empty (length 0) | | measured |
| 4 | the game screen | the current one | [UWEDIT.C:322](../uw2/src/game/UWEDIT.C#L322) |
| 5 | main menu | 2, read separately | [MAINMENU.C:150](../uw2/src/ui/MAINMENU.C#L150), [MAINMENU.C:157](../uw2/src/ui/MAINMENU.C#L157) |
| 6 | Origin logo | 5 | [UWEDIT.C:179](../uw2/src/game/UWEDIT.C#L179) |
| 7 | Looking Glass logo | 6 | [UWEDIT.C:184](../uw2/src/game/UWEDIT.C#L184) |
| 8 | end of game | 7 | [SKILLS.C:610](../uw2/src/game/SKILLS.C#L610) |
| 9 | end of game, second screen | the current one | [SKILLS.C:614](../uw2/src/game/SKILLS.C#L614) |

UW1's `.BYT` files are the same 64000 bytes, read raw ([UW1 CHARGEN.C:741](../uw1/src/game/CHARGEN.C#L741)).

## Palettes and light

| file | layout | source |
| --- | --- | --- |
| `PALS.DAT` | palettes of 0x300 bytes, 6-bit RGB, palette n at n × 0x300; 11 in UW2, 8 in UW1 (measured, 8448 and 6144 bytes) | [GRFX.C:105](../uw2/src/gfx/GRFX.C#L105) |
| `ALLPALS.DAT` | 32 auxiliary palettes of 16 colour indexes: the code reads 0x200 bytes; the file is 513 bytes in both games (measured). `WEAP.CM` replaces palette 30 in memory | [LOADGR.C:428](../uw2/src/gfx/LOADGR.C#L428), [PANELS.C:697](../uw2/src/ui/PANELS.C#L697) |
| `LIGHT.DAT`, `MONO.DAT` | 16 maps of 256 colour indexes, 0x1000 bytes; `MONO.DAT` replaces `LIGHT.DAT` at light level 5 | [LIGHTING.C:66](../uw2/src/map/LIGHTING.C#L66), [LIGHTING.C:93](../uw2/src/map/LIGHTING.C#L93) |
| `SHADES.DAT` | 8 rows, light level 0 (darkest) to 7, of six int16: smooth_div, smooth_base, smooth_lowpass, curvrad (view radius, 3 to 7), distpoly, dist8; read at level × 12. 96 bytes, the same in both games (measured) | [LIGHTING.C:74](../uw2/src/map/LIGHTING.C#L74) |
| `XFER.DAT` | colour translation tables of 256 bytes: UW2 reads 5 (0x500, the file's size), UW1 6 (0x600, its file's size) | [LIGHTING.C:94](../uw2/src/map/LIGHTING.C#L94), [UW1 LIGHTING.C:95](../uw1/src/map/LIGHTING.C#L95) |
| `DL.DAT` | UW2: a byte per level, 80; value mod 10 is a light level, and 10 or more makes it the level's minimum light | [PLAYDATA.C:432](../uw2/src/game/PLAYDATA.C#L432) |

No matched source names `LIGHTS.DAT` (both games, 8 bytes), `LIGHTING.DAT` or `CONTROLS.DAT` (UW2). Nothing in the code reads them.

## Fonts: FONT*.SYS

A 12-byte header, `struct FontInfo` ([include/gfx.h:13](../uw2/src/include/gfx.h#L13)): int16 width-field size (1), character size in bytes, space width, height, row width in bytes, widest character. Then each character's bitmap followed by its width. The loader reads (character size + width-field size) × 128 bytes whatever the file's length ([gfx/GRFX.C:69](../uw2/src/gfx/GRFX.C#L69)); the shipped files are shorter than that, holding 127 characters or fewer (measured: `FONT5X6P.SYS` is 901 bytes, 12 + 127 × 7).

## Sound and music

`SOUND\SOUNDS.DAT` ([sound/SOUND.C:1294](../uw2/src/sound/SOUND.C#L1294)): a byte, the number of effects, then per effect:

| offset | UW2 (8 bytes) | UW1 (5 bytes) |
| --- | --- | --- |
| 0 | patch | patch |
| 1 | note | note |
| 2 | volume | volume |
| 3–4 | length, **high byte first** ([SOUND.C:1308](../uw2/src/sound/SOUND.C#L1308)) | length, high byte first ([UW1 SOUND.C:708](../uw1/src/sound/SOUND.C#L708); the disassembly agrees, `UW1_asm.asm` line 66456) |
| 5 | digital: nonzero plays `SPnn.VOC` | |
| 6–7 | priority, byte 6 + byte 7 × 200 ([SOUND.C:1310](../uw2/src/sound/SOUND.C#L1310)) | |

Measured: UW2 has 49 effects (393 bytes), UW1 24 (121 bytes). The lengths are 0x40, 0x80, 0x180 and the like when read high byte first. Effect numbers 100 and up play `SOUND\UWnn.VOC` for n = effect − 100, the rest `SPnn.VOC` ([SOUND.C:385](../uw2/src/sound/SOUND.C#L385)).

Music: theme n is `SOUND\UWAnn.XMI`, or `UWRnn.XMI` for the Roland card, with nn the theme number **in octal** ([SOUND.C:878](../uw2/src/sound/SOUND.C#L878), [include/sound.h:41](../uw2/src/include/sound.h#L41)). So `UWA10.XMI` is theme 8 and `UWA30.XMI` theme 24. UW1 names them `UWnn.XMI` for the MT-32 and `AWnn.XMI` otherwise ([UW1 SOUND.C](../uw1/src/sound/SOUND.C#L319)).

## Smaller files

| file | layout | source |
| --- | --- | --- |
| `CMB.DAT` | 10 rules of 3 words (source, source, result; bit 15 of a source: used up), 0x3C bytes. All 10 are scanned; there is no end marker test | [COMBINE.C:32](../uw2/src/obj/COMBINE.C#L32), [COMBINE.C:54](../uw2/src/obj/COMBINE.C#L54) |
| `GRAVE.DAT` | a byte per gravestone index (the link less 0x200 if is_quant, else the owner): the picture; 0 none | [LOOK.C:306](../uw2/src/obj/LOOK.C#L306) |
| `WEAP.DAT` | UW2: 8 records of 0x61 bytes, picture count, 3 frame totals, 31 frame numbers, 31 x, 31 y; record (1 − lefty) × 4 + weapon kind. Pictures from `WEAP.GR` at (1 − lefty) × 0x7C + kind × 0x1F. 776 bytes (measured) | [PANELS.C:564](../uw2/src/ui/PANELS.C#L564), [PANELS.C:576](../uw2/src/ui/PANELS.C#L576) |
| `WEAPONS.DAT` | UW1: 8 records of 0x38 bytes, 28 x then 28 y, record (1 − lefty) × 4 + kind | [UW1 PANELS.C:758](../uw1/src/ui/PANELS.C#L758) |
| `WEAP.CM`, `WEAPONS.CM` | 16 colour indexes for the weapon pictures, the second 16 for body type 1, loaded over auxiliary palette 30. UW2's file is 64 bytes; only the first 32 are read | [PANELS.C:695](../uw2/src/ui/PANELS.C#L695) |
| `SKILLS.DAT` | 8 classes of 4 bytes (strength, dexterity, intelligence, skill points), then per class five entries of a count and that many skill numbers | [CHARGEN.C:20](../uw2/src/game/CHARGEN.C#L20), [CHARGEN.C:58](../uw2/src/game/CHARGEN.C#L58) |
| `CHRGEN.DAT` | 8 records of 0x12 bytes (`struct ChrOpt`, the pointer fields filled at run time), then each record's zero-ended list of string numbers | [CHARGEN.C:45](../uw2/src/game/CHARGEN.C#L45), [CHARGEN.C:729](../uw2/src/game/CHARGEN.C#L729) |
| `UW.CFG` | text, from `DATA\` or from `UWHOME` when that is set | [UWEDIT.C:497](../uw2/src/game/UWEDIT.C#L497) |
| `DESC` | the save's description, raw, no terminator; written by `SaveGame` | [GAMEWRAP.C](../uw2/src/game/GAMEWRAP.C#L12) |

## Where UW-Formats disagrees with the code

Each item quotes the document and gives the code's evidence.

- **9.1, the UW2 archive header.** "0004 Int32 unknown (always 0)". The long is at 2, straight after the count, and the tables start at 6 ([ARC.C:172](../uw2/src/sys/ARC.C#L172), [ARC.C:58](../uw2/src/sys/ARC.C#L58)).
- **9.1, flags.** "Bit 0: block should be compressed (always set in uw2)". `SCD.ARK`'s blocks have flags 0 (measured). "A compressed block always starts with an Int32 value that is to be ignored": it is the uncompressed length, and DOS does ignore it ([ACLZW.C:41](../uw2/src/sys/ACLZW.C#L41)). "an offset of 18 is added. The sign bit is bit 11": there is no sign; the position is an index into a 4096-byte ring that starts filled with spaces with writing at 4078, which is where the 18 comes from ([LZSS.C:147](../uw2/src/sys/LZSS.C#L147)). A copy that reaches before the start of the output yields spaces.
- **4.1, the level block.** "7afc 0104 unknown (260 bytes)" is the active mobile object list; "7c00 0002" (no description) is its count ([level.h:23](../uw2/src/include/level.h#L23), [MAP.C:116](../uw2/src/map/MAP.C#L116)). UW2's block is 0x7E08 with the overlays at 0x7C08 and timers at 0x7D88; the document gives neither.
- **4.2, the tile.** "8 1 unknown (?? special light feature ??)" is the tile light flag the player's movement tests ([map.h:39](../uw2/src/include/map.h#L39)).
- **4.2.3, mobile object extra info.** Most of the "unknown" and "?" fields have uses in the code; see the [Objects](#objects) table. Two corrections: 0x0D bits 0 to 3 ("npc_level") hold the goal saved while another runs, and 0x19 is not "npc_hunger": bits 2 and 3 are the spell being cast, bit 6 the player's side and bit 7 fed.
- **4.5, animation infos.** "Int16 unk2" is the frames left, −1 for ever ([object.h:110](../uw2/src/include/object.h#L110)).
- **4.4, UW2 texture map.** "Ceiling seems to be textured by entry 0x20" is not checked here. The block is 64 words and 3 words of door bytes, 0x86, as the document says.
- **4.6, automap.** The high nibble is two fields, not one value. In UW2, bits 4 and 5 are a kind (1 door, 2 bridge, 3 stairs) and bits 6 and 7 the floor's terrain class (1 water, 2 lava, 3 ice). The document's "TELEPORT 0x3" is the stairs kind, "BRIDGE 0x6" is a bridge over water (kind 2 plus water), and "LAVA 0xc" is ice: lava is 0x8. See [The automap and map notes](#the-automap-and-map-notes).
- **4.8, terrain.** The UW2 values are a class in bits 6 and 7 plus a subtype; "00D8" and "00E8 Ice walls (crumbling?)" are ice with slipperiness 3 and 5. See [TERRAIN.DAT](#terraindat).
- **6.3, objects.dat.** "0d82 0x20 unknown, maybe jewelry info table" is two tables of 16 bytes: food values and trigger modes. Ranged weapons: "0000 Int16 unknown, bits 9-15: ammunition needed (+0x10); 0002 Int8 durability" is damage, type and ammunition, the ammunition in byte 2. Containers: bytes 1 and 2 are one int16 mask, so "0002 number of slots available?" is its high byte. Lights: "0000 light brightness, 0001 duration" is the other way round: byte 0 the burn rate, byte 1 the brightness (measured too: the taper of sacrifice, which never burns out, has byte 0 equal to 0). Animation objects: "0000 unknown (0x00, 0x21 or 0x84)" is the class flags.
- **6.2, comobj.dat.** Byte 9, "unknown2 bits 0-1: ??", is the render type, and bits 2 to 5 the culling distance. Byte 7 bits 1 to 4, "type?", is the fate of a thrown or dropped object. Byte 3's bits are solid (1), no hit (3), pick up (5) and stacking (6 and 7), not "3d objects", "magic" and "container".
- **6.4, cmb.dat.** "3 zeros mark the end of the table": the code reads 10 rules and scans all of them.
- **7.1, conversation header.** "0004 Int16 code size" and "0006 unknown, always 0" are one long. "n+04 Int16 unknown, always seems to be 1" is a count the code stores. Any import kind other than 0x111 is a variable.
- **7, generic conversations.** "If it is 0, a generic NPC-conversation is used": the slot is 0x100 plus the creature's item id low 6 bits, and UW2 ships no such slots, so in UW2 those critters do not talk.
- **9.2.1, UW1 PLAYER.DAT.** "The first 220 bytes are encrypted": 0xD2, 210 ([UW1 PLAYDATA.C:122](../uw1/src/game/PLAYDATA.C#L122)). The sample loop resets its increment only at byte 80; the code restarts it every 80 bytes, at 160 too, and the sample loop decodes record bytes 160 to 209 wrongly (measured on DOS saves). "0000 14*char character name": the name field is 0x1E bytes. "00DC Int8 current vitality" is the player object's hit points in the plain workspace, and it is the value the game keeps on loading.
- **9.2.2, UW2 inventory.** "seems like some kind of compression is being used" and "TODO": there is none; it is the workspace described under [PLAYER.DAT](#playerdat).
- **9.3, skills.dat.** "First 0x20 bytes are unknown": the 8 classes' starting attributes and skill points.
- **9.3, shades.dat.** "12 entries, 8 byte long each": 8 entries of 12 bytes.
- **3.1.2, allpals.dat.** "0x1f (=31) such palettes": the code reads 32.
- **3.3, BYT.ARK palettes.** Character creation (block 1) uses palette 3, not 0; the Looking Glass logo (7) palette 6, not 5; the first end screen (8) palette 7, not 0.
- **3.4, textures.** "unknown, always seems to be 2" is the file type, which the loaders require.
- **3.5, fonts.** "unknown, always 1 (might be size of character width field)" is that size; the code multiplies by it.
- **3.9, XFER.DAT.** The tables are 256 bytes each, not at 0x80 intervals, and UW1's file has 6.
- **10.1, SOUNDS.DAT.** The entry format is given above; the length is stored high byte first.

## UW1 only

This section lists what is UW1's alone, with the source that reads or writes it. **measured** means a script parsed the shipped GOG data or DOS-written saves.

| file | UW1's layout | source |
| --- | --- | --- |
| `LEV.ARK`, `CNV.ARK` | a word n, then n long block offsets at 2, 0 for an absent block. No flags, lengths or compression. A block runs to the smallest offset greater than its own, or to the end of the file. A rewritten block of a new length is moved to the end of the file, so DOS saves have blocks out of index order | [sys/ARC.C:36](../uw1/src/sys/ARC.C#L36), [ARC.C:153](../uw1/src/sys/ARC.C#L153), [ARC.C:89](../uw1/src/sys/ARC.C#L89) |
| `LEV.ARK` blocks | 135 (measured): level map at level − 1, animation overlays at level + 8, texture map at level + 0x11, automap at level + 0x1A, notes at level + 0x23. The shipped file has only the first three kinds (measured) | [include/level.h:39](../uw1/src/include/level.h#L39) |
| level block | 0x7C08 bytes, ending at the magic word 0x7577; UW2's carries overlays and timers after it, UW1's does not | [include/level.h:17](../uw1/src/include/level.h#L17), [map/MAP.C:86](../uw1/src/map/MAP.C#L86) |
| overlay block | 64 entries of 6 bytes, 0x180. A block of another length loads as no overlays. Saving zeroes only the object index of the entry at the count, so entries after it are written back as they were | [obj/EFFECT.C:480](../uw1/src/obj/EFFECT.C#L480), [EFFECT.C:499](../uw1/src/obj/EFFECT.C#L499) |
| texture map | 0x7A bytes: 48 wall words (`W64.TR`), 10 floor words (`F32.TR`), 3 words of door textures | [map/TEXTMAPS.C:118](../uw1/src/map/TEXTMAPS.C#L118) |
| `TERRAIN.DAT` | 512 words: walls from 0, floors from 0x200 | [map/TEXTMAPS.C:232](../uw1/src/map/TEXTMAPS.C#L232) |
| automap byte | bits 0–3 the tile type; bits 4–5 the floor's terrain word as it is (0x10 water, 0x20 lava); bits 6–7 the kind (1 door, 2 bridge, 3 stairs). UW2 has the two upper fields the other way round | [3d/GRIDDB.C:562](../uw1/src/3d/GRIDDB.C#L562), [GRIDDB.C:641](../uw1/src/3d/GRIDDB.C#L641), [ui/AUTOMAP.C:288](../uw1/src/ui/AUTOMAP.C#L288) |
| `PLAYER.DAT` | key byte (name[0] xor 0xAA), the 0xD2-byte record xored with key + 3, key + 6, ... restarting every 0x50 bytes, then the count word at 0xD3 and the inventory workspace at 0xD5 | [game/PLAYDATA.C:104](../uw1/src/game/PLAYDATA.C#L104), [sys/MISCUTIL.C:268](../uw1/src/sys/MISCUTIL.C#L268), [inv/INVSAVE.C:101](../uw1/src/inv/INVSAVE.C#L101) |
| player record | `struct Player`, 0xD2 bytes; the same as UW2's to 0x5D, then UW1's own fields | [include/player.h:26](../uw1/src/include/player.h#L26) |
| `.BYT` | 320 × 200 8-bit screens, 64000 bytes, read raw | [game/CHARGEN.C:741](../uw1/src/game/CHARGEN.C#L741) |
| `WEAPONS.DAT` | 8 records of 0x38 bytes, 28 x then 28 y, record (1 − lefty) × 4 + weapon kind; pictures from `WEAPONS.GR` at (1 − lefty) × 0x70 + kind × 0x1C | [ui/PANELS.C:756](../uw1/src/ui/PANELS.C#L756) |
| `XFER.DAT` | six 256-byte tables, 0x600 | [map/LIGHTING.C:95](../uw1/src/map/LIGHTING.C#L95) |
| `SOUNDS.DAT` | a count byte, then 5 bytes an effect: patch, note, volume, length high byte first. 24 effects (measured) | [sound/SOUND.C:681](../uw1/src/sound/SOUND.C#L681) |
| music | theme n is `SOUND\UWnn.XMI` for the MT-32 and `AWnn.XMI` otherwise, nn in octal | [sound/SOUND.C:319](../uw1/src/sound/SOUND.C#L319) |
| digital sound | `SOUND\nn.VOC` | [sound/SOUND.C:1075](../uw1/src/sound/SOUND.C#L1075) |

## See also

[FINDINGS.md](../uw2/docs/FINDINGS.md) for what the code does with these data, [subsystems/sys.md](../uw2/docs/subsystems/sys.md) for the archive code, [subsystems/map.md](../uw2/docs/subsystems/map.md) for the level, and [LAYOUT.md](../uw2/docs/LAYOUT.md) for the EXE's own data.
