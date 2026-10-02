# Sound and music

This page describes how UW2 makes sound: `src/sound/SOUND.C` (DOS resident segment seg016), which plays the effects and the music, and `src/sound/AIL.ASM` (seg022), Miles Design's Audio Interface Library, which owns the timer interrupt and passes everything else to a loadable driver. The declarations are in `src/include/sound.h`. Statements marked "probably" are inferences; the reason is given with each.

## Files

| File | Segment | Name | What it does |
|---|---|---|---|
| `SOUND.C` | seg016 | descriptive | effects (digital and MIDI), music themes, timbres, the sound settings |
| `AIL.ASM` | seg022 | original (the procedures of `AIL.ASM` in Miles' public-domain AIL 2.14 source) | AIL 2 application interface: timers, driver registration, driver call stubs |

Function names in `SOUND.C` are the FM Towns originals where that build has them; the FM Towns build has this file's functions in the same order, minus a few DOS-only ones (see the file's header). `AIL.ASM`'s names are all FM Towns names, since the FM Towns build carries the same library.

## The pieces

**AIL and the drivers.** AIL itself only manages timers and a table of up to 16 drivers. A driver is an `.ADV` file from `SOUND\`, loaded whole into a far block so that it starts at offset 0 of a paragraph (`load_sound_driver`), and registered with `AIL_register_driver`, which checks for `Copy` at offset 2 and finds the driver's function table through its first word. Almost every `AIL_` routine is a six-byte stub that loads a function number (64h to 0C2h) and jumps to `call_driver`, which finds the function in the driver's table and jumps to it with the C caller's stack untouched. The GOG release has seven music drivers, `DM01.ADV` to `DM07.ADV` (PC speaker, AdLib, Sound Blaster FM, Sound Blaster Pro FM, Roland MT-32, Pro Audio Spectrum FM, Sound Blaster Pro FM, by the strings in the files) and three digital ones, `DD01.ADV` to `DD03.ADV` (Sound Blaster, Sound Blaster Pro, Pro Audio Spectrum). `UW.CFG` gives the music card number and the speech card number with their IRQ, port and DMA (`seg016_1E73_2FCB`); the number picks the file, written as two octal digits.

**Timers.** `API_timer` replaces int 8. It runs up to 16 timers at their own periods, plus the old BIOS handler as a seventeenth at 18.2 Hz. The PIT is set to the shortest period any timer needs, and each tick adds that period to every running timer's accumulator, firing the timer when it reaches its period. It switches to a 512-byte stack inside the code segment and checks a `Test` marker below it on the way out. UW2 registers two timers: the game clock (`cllbck_tst`, 256 Hz, which increments `*Time`, so game time is in 1/256 seconds) and the effects timer (`digi_fx_timer` or `fx_timer`, 16 Hz). `AIL_init_driver` registers a third for any driver that asks for periodic service.

**Effects.** An effect number below 100 is an entry of `SOUND\SOUNDS.DAT` (`struct Effect` in SOUND.C: patch, note, volume, length, a flag for a digital sample, priority); 100 and up name `SOUND\UWnn.VOC` directly. `play_effect` (a position), `play_effect_here` (a fixed pan), `play_effect_on_mobile` and `play_effect_on_mobile_src` (an object, the second following it as it moves) all try the digital channel first, then a MIDI note.

- Digital: one channel. `digi_fx_play` reads `SOUND\SPnn.VOC` into an EMS cache of four 16 KB pages (four slots, `struct EmsSound`), evicting idle sounds and then lower-priority playing ones, and starts it if its priority is at least the playing sound's. Playback streams through two 2 KB buffers that AIL plays in turn; `update_digi_playback`, called from the main loop, refills them from EMS (mapping the sound's page into frame page 2 and restoring whatever the game had there), handles looping, and re-pans a moving source.
- MIDI: three effect slots, each on a channel locked from the music driver (`fx_play`), with the effect's patch from timbre bank 1. The 16 Hz timer counts each note's length down and releases the channel. On the PC speaker six effects play as fixed notes on channel 2.
- `sound_move` gives pan and volume from the player's position and heading: full volume within one tile, silence beyond six, linear between.

**Music.** Themes are XMIDI files, `SOUND\UWAnn.XMI`, or `UWRnn.XMI` for the MT-32 (card 5), where `nn` is the theme number in octal (the files run 01 to 07, 10 to 17, 30, 31). `load_new_music` loads a theme into a 9000-byte buffer, registers it as an AIL sequence and installs each timbre the sequence requests from the global timbre library `SOUND\UW.<suffix>` (the driver's `data_suffix`: `UW.AD`, `UW.MT`, `UW.OPL` in the GOG release). `change_music_maybe` and `loop_music_maybe`, from the main loop, choose what plays: themes 2 to 4 are combat themes, 5 plays with the weapon drawn, 6 must finish before it is replaced, 8 to 15 are walking themes chosen per world from a table of three each (`walking_music`, nine worlds), and themes 1, 6 and 7 are followed by theme 10. Combat music gives way ten seconds after the last combat. Other files request themes with `set_new_music`.

**Instruments.** `play_instrument` turns the keyboard into a musical instrument for one of three patches.

## Data flow

1. Start-up (`game/UWEDIT.C`): `seg016_1E73_2FCB` reads `UW.CFG`; `init_sounds` runs `AIL_startup`, loads and initialises the music driver, the speech driver (`init_speech`), the effects (`init_fx`, which reads `SOUNDS.DAT` and starts the 16 Hz timer) and the timbres; `init_timers` starts the 256 Hz clock. `load_digi_fx` preloads effects 1, 2 and 0xB into EMS.
2. Each pass of the main loop: `update_digi_playback` and `change_music_maybe`.
3. Game code: `play_effect*`, `kill_effect`, `set_new_music`, `turn_music`, `turn_fx`.
4. Shutdown: `free_timers`, `free_sounds` (`AIL_shutdown`, which shuts down every driver, releases every timer and restores int 8 and the PIT).

## Key structures

- `struct SoundBuff`, `struct DrvrDesc` (`sound.h`): AIL's sound buffer and driver description.
- `struct Effect`, `struct EmsSound`, `struct DsChannel`, `struct DigiSrc`, `struct GtlHdr` (`SOUND.C`): a SOUNDS.DAT entry, an EMS cache slot, the digital channel's playback state, the moving source, a timbre library directory entry.
- AIL's own tables are in its code segment; the comment above `L0000` in `AIL.ASM` lists them.

## Open questions

- The AIL 2.14 source is not in this workspace, so `AIL.ASM`'s comments come from the code, not from Miles' comments. Comparing the two would name the data labels. The version word 0D3h that drivers are checked against probably corresponds to an AIL 2.1x release; that is an inference from the number only.
- What `AIL_init_driver` reads at offset 14h of the driver description is used as a frequency; it is probably the service rate field of AIL's driver description, which `struct DrvrDesc` does not yet include.
- Effects 0x5A and 0x5B play as effects 1 and 2, and as MIDI only with speech card 1. Why card 1 differs is not known.
- Effect lengths of 5000 and more loop `(length >> 6) - 1` times; the unit behind that formula is not known.
- `seg016_1E73_19DE` (load a file into a new block), the effects toggle `seg016_1E73_1BC3`, and the static `toggle_music`, `fade_music`, `stop_speech`, `init_voc_playback` and `voc_stub` are never called.
- UW-Formats' song titles for the UW2 themes match the code's use of themes 2 to 6; the others have not been checked against the game.
