# Sound and music: candidates for the findings page

Candidates from `src/sound` for the project's findings page; [sound.md](sound.md) describes the subsystem. Each was re-read in the matched source.

## Possible bugs

### UW.CFG's card numbers are stored as words into bytes

- **What happens:** `seg014_1DC5_1D0D` reads each card number with `%d` straight into a byte global (`sound_card`, `speech_card`), so `sscanf` writes two bytes. The music card's high byte lands on `speech_card`, which the next line then reads; the speech card's high byte lands on the next variable in `_DATA`, by the definition order the low byte of `music_driver`, turning its initial -1 into 0xFF00.
- **Where:** [sound/SOUND.C](../../src/sound/SOUND.C) (marked there).
- **Evidence:** code reading; the neighbour is taken from the order of definition in `_DATA`, not checked in the EXE.
- **Confidence:** possible.
- **Effect:** none found: `init_sounds` sets `music_driver` whenever there is a music card, and with none, music and effects are turned off before it is used.

### The music buffer is not checked against the file's length

- **What happens:** `read_file_to_mbuf` reads a whole `.XMI` file into `midi_buf`, which `init_sounds` allocated at 6300 bytes, without comparing the length.
- **Evidence:** the largest theme UW1 ships is 6248 bytes (`UW02.XMI`).
- **Confidence:** possible; no effect with the shipped files, but a longer theme would overrun the block.

## Engine findings

- UW1 has no digitised sound effects: every effect is a MIDI note on one of four channels locked from the music driver, and on the PC speaker only six effects sound at all.
- The theme numbers are UW1's own (`sound.h`), not UW2's: walking themes are 2 to 4 for the whole game, where UW2 has a walking theme for each world.
- The speech borrows six EMS pages from the critter art cache while a cutscene plays.

## Dead code

- `sound_b292` is set and never read; a whole-file reader (UW2's `seg016_1E73_19DE`) and the effects toggle are not called (marked in `SOUND.C`).
