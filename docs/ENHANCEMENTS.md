# Enhancements

The ports play the original games exactly: every recorded session replays in them identically to DOS. Enhancements are changes the original games do not have. Each is off unless you turn it on, and a session played with any of them says so in its log and in its recording.

```
uw2port --enhance list                 the enhancements, with what each does
uw2port --enhance skip-intro           turn one on (kept for later runs)
uw2port --no-enhance skip-intro        turn it off again
```

They are kept in the settings file (`enhance=` in `uw1port.cfg` or `uw2port.cfg`, in the port's home folder). A run with any on logs `enhance: NAMES (not as DOS)`.

| Name | Games | Kind | What it does | From |
|---|---|---|---|---|
| `skip-intro` | UW1, UW2 | timing | Starts at the main menu, without the title or the introduction the game plays when there are no saved games (the menu's Introduction still plays it) | the idea from [UltimaHacks](https://github.com/JohnGlassmyer/UltimaHacks) |
| `wrap-menu` | UW1, UW2 | presentation | In the start menu and the saved games, up from the first item goes to the last and down from the last to the first | the idea from [UltimaHacks](https://github.com/JohnGlassmyer/UltimaHacks) |
| `fast-panels` | UW1, UW2 | timing | The right-hand panel slides twice as fast | the idea from [UltimaHacks](https://github.com/JohnGlassmyer/UltimaHacks) |
| `free-heading` | UW1, UW2 | gameplay | Sliding along a wall no longer turns your view to face it | the idea from [UltimaHacks](https://github.com/JohnGlassmyer/UltimaHacks) |
| `subtitles` | UW2 | presentation | Cutscenes show their text while the speech plays (the introduction's lines are text only; spoken lines are in other cutscenes) | the idea from [UltimaHacks](https://github.com/JohnGlassmyer/UltimaHacks) |
| `skill-messages` | UW2 | presentation | When a skill point or a trainer raises a skill, the conversation says which and to what | the idea from [UltimaHacks](https://github.com/JohnGlassmyer/UltimaHacks) |
| `perspective` | UW1, UW2 | presentation | Floors, ceilings and walls stay straight when the view pitches. UW1: a perspective-correct texture mapper in C (`uw1/src/port/gfx/perspmap.c`) in place of the original two, covering exactly the pixels they cover. UW2: every polygon not flat to the view is mapped perspective-correctly, where the original maps some linearly (its walls keep their columns) | [uwpatch](https://github.com/SiENcE/uwpatch) (UW1's mapper, translated) |
| `full-sprites` | UW1, UW2 | presentation | Creatures and objects keep their size when the view pitches. Within the original's small pitch range the difference is a pixel or so; it matters with a wider pitch | the idea from [UltimaHacks](https://github.com/JohnGlassmyer/UltimaHacks) |

**`perspective` in UW1**, the same moment looking down, off and on: the stone courses bend without it and run straight with it.

![UW1 pitched, perspective off](img/perspective-off.png) ![UW1 pitched, perspective on](img/perspective-on.png)

**Kinds.** *Presentation*: how the game looks or is controlled; the game itself is unchanged, which the tests prove by replaying every session with it on. *Timing*: the same game, reached by a different path or frames. *Gameplay*: the rules change; the log adds "gameplay changed".

**How they are tested** (Exhume's `tools/enhcheck.py`, part of `make test`). With none on, every session still matches DOS, and the DOS builds are byte-identical. A presentation enhancement must leave every session's game state as DOS's; only the screen may differ. A timing or gameplay one has its own session under `tests/replay/enhanced/`, recorded from an input script (`session.script`, [BUILDING.md](BUILDING.md#input-scripts)) and starting, where it says so (`stage_from`), from the saved game another session writes, with a baseline the port made (`"made_by": "port"`), run twice and checked identical. A recording made with enhancements carries them (recording format 5): its replay turns them on, and DOS never replays it. Presentation checks ignore the screen and the graphics engine's working memory (`GFX`), which a rendering enhancement changes by design. UW1's `perspective` is also checked to cover exactly the original mappers' pixels: with `UW1PORT_TEXTURE_PROBE` each face is drawn in a colour of its own, and every checkpoint's screen of the `walk` session must be the same with the enhancement on and off.

**Credits.** The planned enhancements come largely from John Glassmyer's [UltimaHacks](https://github.com/JohnGlassmyer/UltimaHacks) and SiENcE's [uwpatch](https://github.com/SiENcE/uwpatch), both MIT; each game's THIRD-PARTY-NOTICES carries their notices.
