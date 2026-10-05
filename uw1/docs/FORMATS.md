# Data file formats

The layouts of both games' data files, as the matched code reads and writes them, are in [UW2Decomp's FORMATS.md](https://github.com/abedegno/UW2Decomp/blob/main/docs/FORMATS.md). Each format there gives UW2's layout and UW1's differences, with the UW1 source line for each, and ends with the places where the community document UW-Formats disagrees with the code.

This page lists what is UW1's alone, with the source that reads or writes it. **measured** means a script parsed the shipped GOG data or DOS-written saves.

| file | UW1's layout | source |
| --- | --- | --- |
| `LEV.ARK`, `CNV.ARK` | a word n, then n long block offsets at 2, 0 for an absent block. No flags, lengths or compression. A block runs to the smallest offset greater than its own, or to the end of the file. A rewritten block of a new length is moved to the end of the file, so DOS saves have blocks out of index order | [sys/ARC.C:36](../src/sys/ARC.C#L36), [ARC.C:153](../src/sys/ARC.C#L153), [ARC.C:89](../src/sys/ARC.C#L89) |
| `LEV.ARK` blocks | 135 (measured): level map at level − 1, animation overlays at level + 8, texture map at level + 0x11, automap at level + 0x1A, notes at level + 0x23. The shipped file has only the first three kinds (measured) | [include/level.h:39](../src/include/level.h#L39) |
| level block | 0x7C08 bytes, ending at the magic word 0x7577; UW2's carries overlays and timers after it, UW1's does not | [include/level.h:17](../src/include/level.h#L17), [map/MAP.C:86](../src/map/MAP.C#L86) |
| overlay block | 64 entries of 6 bytes, 0x180. A block of another length loads as no overlays. Saving zeroes only the object index of the entry at the count, so entries after it are written back as they were | [obj/EFFECT.C:480](../src/obj/EFFECT.C#L480), [EFFECT.C:499](../src/obj/EFFECT.C#L499) |
| texture map | 0x7A bytes: 48 wall words (`W64.TR`), 10 floor words (`F32.TR`), 3 words of door textures | [map/TEXTMAPS.C:118](../src/map/TEXTMAPS.C#L118) |
| `TERRAIN.DAT` | 512 words: walls from 0, floors from 0x200 | [map/TEXTMAPS.C:232](../src/map/TEXTMAPS.C#L232) |
| automap byte | bits 0–3 the tile type; bits 4–5 the floor's terrain word as it is (0x10 water, 0x20 lava); bits 6–7 the kind (1 door, 2 bridge, 3 stairs). UW2 has the two upper fields the other way round | [3d/GRIDDB.C:562](../src/3d/GRIDDB.C#L562), [GRIDDB.C:641](../src/3d/GRIDDB.C#L641), [ui/AUTOMAP.C:288](../src/ui/AUTOMAP.C#L288) |
| `PLAYER.DAT` | key byte (name[0] xor 0xAA), the 0xD2-byte record xored with key + 3, key + 6, ... restarting every 0x50 bytes, then the count word at 0xD3 and the inventory workspace at 0xD5 | [game/PLAYDATA.C:104](../src/game/PLAYDATA.C#L104), [sys/MISCUTIL.C:268](../src/sys/MISCUTIL.C#L268), [inv/INVSAVE.C:101](../src/inv/INVSAVE.C#L101) |
| player record | `struct Player`, 0xD2 bytes; the same as UW2's to 0x5D, then UW1's own fields | [include/player.h:26](../src/include/player.h#L26) |
| `.BYT` | 320 × 200 8-bit screens, 64000 bytes, read raw | [game/CHARGEN.C:741](../src/game/CHARGEN.C#L741) |
| `WEAPONS.DAT` | 8 records of 0x38 bytes, 28 x then 28 y, record (1 − lefty) × 4 + weapon kind; pictures from `WEAPONS.GR` at (1 − lefty) × 0x70 + kind × 0x1C | [ui/PANELS.C:756](../src/ui/PANELS.C#L756) |
| `XFER.DAT` | six 256-byte tables, 0x600 | [map/LIGHTING.C:95](../src/map/LIGHTING.C#L95) |
| `SOUNDS.DAT` | a count byte, then 5 bytes an effect: patch, note, volume, length high byte first. 24 effects (measured) | [sound/SOUND.C:681](../src/sound/SOUND.C#L681) |
| music | theme n is `SOUND\UWnn.XMI` for the MT-32 and `AWnn.XMI` otherwise, nn in octal | [sound/SOUND.C:319](../src/sound/SOUND.C#L319) |
| digital sound | `SOUND\nn.VOC` | [sound/SOUND.C:1075](../src/sound/SOUND.C#L1075) |
