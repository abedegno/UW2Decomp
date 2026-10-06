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

**Kinds.** *Presentation*: how the game looks or is controlled; the game itself is unchanged, which the tests prove by replaying every session with it on. *Timing*: the same game, reached by a different path or frames. *Gameplay*: the rules change; the log adds "gameplay changed".

**How they are tested** (Exhume's `tools/enhcheck.py`, part of `make test`). With none on, every session still matches DOS, and the DOS builds are byte-identical. A presentation enhancement must leave every session's game state as DOS's; only the screen may differ. A timing or gameplay one has its own session under `tests/replay/enhanced/`, with a baseline the port made (`"made_by": "port"`), run twice and checked identical. A recording made with enhancements carries them (recording format 5): its replay turns them on, and DOS never replays it.

**Credits.** The planned enhancements come largely from John Glassmyer's [UltimaHacks](https://github.com/JohnGlassmyer/UltimaHacks) and SiENcE's [uwpatch](https://github.com/SiENcE/uwpatch), both MIT; each game's THIRD-PARTY-NOTICES carries their notices.
