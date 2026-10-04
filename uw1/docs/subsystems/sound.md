# Sound and music

UW1's sound is MIDI throughout: the effects are notes played on spare channels of the music driver, the music is XMIDI sequences, and the only digitised sound is the cutscenes' speech. Everything goes through Miles' Audio Interface Library (AIL 2). The sources are in `src/sound`; the declarations, the theme numbers (`MUSIC_*`), the card numbers (`CARD_*`) and AIL's records are in `src/include/sound.h`. The candidates for the findings page are in [sound-findings.md](sound-findings.md).

| File | Segment | What it does |
|---|---|---|
| [`SOUND.C`](../../src/sound/SOUND.C) | seg014_1DC5 | start-up from `UW.CFG`, the game clock, effects, music themes and timbres, the speech player |
| [`AIL.ASM`](../../src/sound/AIL.ASM) | seg020 | AIL 2's application interface: the timer services and the calls passed to the loaded `.ADV` driver |

`AIL.ASM` is an earlier release of the library than UW2's (version word 0CAh); its header lists the differences. `SOUND.C`'s names are UW2's where the routine is the same.

## Start-up

`UWEDIT.C`'s `ReadCfg_ovr112_839` reads two lines of `DATA\UW.CFG` (`seg014_1DC5_1D0D`): the music card with its IRQ, port and DMA, then the speech card. `init_sounds` loads the music driver for the card (`CARD_PCSPKR` to `CARD_MT32`: `PCSPKR.ADV`, `ADLIB.ADV`, `SBFM.ADV`, `SBPFM.ADV`, `PASFM.ADV`, `MT32MPU.ADV`), the speech driver if there is a speech card (`SBDIG.ADV`, `SBPDIG.ADV`, `PASDIG.ADV`), the effects table `SOUND\SOUNDS.DAT` with a 16 Hz effects timer, a 6300-byte music buffer and the timbre library `SOUND\UW.<suffix>` (`UW.AD`, `UW.MT`); on the MT-32 every effect's timbre is installed at once. The PC speaker gets effects only. Any failure prints "Sound system initialization failed." and turns all sound off. `init_timers` starts the game clock, a 256 Hz AIL timer that counts `*Time`.

## Effects

An entry of `SOUNDS.DAT` (5 bytes) is a patch in timbre bank 1, a note, a volume and a length. `fx_play` plays it on the first of four effect slots, locking a channel from the music driver; `play_effect` pans it by the direction to the source in the player's frame and fades it with distance (silent beyond 6 tiles). On the PC speaker only six effects sound, at fixed notes.

## Music

Theme n is `SOUND\UWnn.XMI` on the MT-32 and `SOUND\AWnn.XMI` otherwise, nn in octal. UW1 ships themes 1 to 11 and 13. `sound.h` names them from the callers: 1 the start-up and menu, 2 to 4 walking (picked at random), 5 to 7 combat (foe nearly dead, fighting, the player in danger), 8 the weapon drawn, 9 a creature killed, 10 death, 13 conversations, the automap and sleep. `set_new_music` only asks; `change_music_maybe`, from the main loop, decides: themes 9 and 11 play to their end; ten seconds (0xA00 ticks) after the last combat a combat theme gives way to theme 8 or a walking theme; one combat theme replaces another at most every eight seconds; when a theme ends without a request a walking theme follows (theme 8 with the weapon drawn), unless the theme is one that repeats (`music_repeats`). `loop_music_maybe` follows theme 1 with theme 4.

## Speech

`play_speech(n)` reads `SOUND\nn.VOC` into six 16K EMS pages lent by the critter cache (`critter/CRPAGES.C`'s `ovr113_2A2`) and plays it through two 4K buffers that `update_speech` refills; the cutscenes call it.

## Open questions

- Theme 11 (`UW13.XMI`) is treated like the victory theme (it plays to its end), but no caller of `set_new_music` or `load_new_music` in the matched C asks for it.
- What the cup of wonder's tune (`play_cup_tune`) plays was not traced against the data.
