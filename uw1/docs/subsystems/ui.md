# The user interface and input

This page describes how UW1 reads the mouse and keyboard and routes them to handlers, the 3D view's interaction modes, the screen furniture around the view, the message scroll, the main menu and options, the game's strings and the automap. The sources are in `src/ui`; the declarations, the key codes, the interaction and input modes and the string blocks are in `src/include/ui.h`. The screen furniture's element numbers and icon numbers are in `src/include/gfx.h`, the screen modes in `src/include/sys.h`. The candidates for the findings page are in [ui-findings.md](ui-findings.md).

## Files

| File | Segment | What it does |
|---|---|---|
| [`INPUT.C`](../../src/ui/INPUT.C) | seg010 | the input dispatcher: mouse regions and key handlers by screen mode |
| [`MOUSE.C`](../../src/ui/MOUSE.C) | seg013_1CC9 | the pointer, its cursor pictures and save-under, the input queue, the keyboard reader, keyboard warping |
| [`INTERACT.C`](../../src/ui/INTERACT.C) | seg024_24DC | the game screen's set-up and status clicks, and clicks in the 3D view and the panels: picking, reach, get, look, use, talk, fight; the interaction icons |
| [`PANELS.C`](../../src/ui/PANELS.C) | seg036_3087 | the flasks, compass, power gem, dragons, gargoyle eyes, first-person weapon, rune shelf, active spells and the turning panel |
| [`STATS.C`](../../src/ui/STATS.C) | ovr145 | the statistics page of the character panel |
| [`SCROLLIO.C`](../../src/ui/SCROLLIO.C) | seg043_37F0 | the message scroll: printing, wrapping, escapes, [MORE], typed lines, yes or no |
| [`GAMESTRN.C`](../../src/ui/GAMESTRN.C) | seg039_3495 | `STRINGS.PAK` and the message printers |
| [`MAINMENU.C`](../../src/ui/MAINMENU.C) | ovr138 | the main menu |
| [`OPTIONS.C`](../../src/ui/OPTIONS.C) | ovr130 | the options panel (UW1's own; UW2's `WRAPPER.C` does the job differently) |
| [`AUTOMAP.C`](../../src/ui/AUTOMAP.C) | ovr092 | the automap and its notes |
| [`OVR096.C`](../../src/ui/OVR096.C) | ovr096 | seven browse hooks nothing calls (UW2's `BROWSE.C`) |
| [`OVR137.C`](../../src/ui/OVR137.C) | ovr137 | one empty function nothing calls |

UW1 has no build with symbols. Function and global names are UW2's (the FM Towns originals) where the routine is the same, else the listing's or ours; each file's header says which, and where a name was chosen to fit the overlay stub order. File names follow UW2Decomp's.

## How UW1 differs from UW2 here

- UW2's `GAMESCR.C` and `INTERACT.C` are one segment in UW1 (`INTERACT.C`), and UW2's `SCROLL.C` and `SCROLLIO.C` another (`SCROLLIO.C`).
- There is no joystick code in the C (MOUSE.C has no `joymovecur`; nothing calls seg019's joystick readers, see [sys.md](sys.md)) and no left-handed button swap.
- The icons on the left are drawn by `INTERACT.C` itself (`new_IconSelect`, `new_IconUnselect`), and the options panel is `OPTIONS.C`, a different design from UW2's button groups.
- The right-hand panel turns over (a squeeze drawn column by column from copies kept in EMS) where UW2's slides, and two animated dragons sit beside the view.
- The automap has no map scraps, gem or worlds; its notes and maps live in `SAVE0\LEV.ARK`, opened through an archive record the caller keeps (`struct Arc`, file.h).

## Input

Every wait loop reads input through `MOUSE.C`'s `get_input` (via `mouse_get_input` and `mouse_get_input_sp`). It alternates between the mouse and the keyboard and returns 1 to 3 for the buttons held, a key code, or -1. Key codes are the character for ordinary keys and 0x80 and up for special keys (ui.h's `KEY_*`); Ctrl, Alt and Shift add `KEY_CTRL`, `KEY_ALT` and `KEY_SHIFT`. The separate cursor block gives its own codes (`KEY_GHOME` to `KEY_GPGDN`, 0xA5 to 0xAC): KEYQUEUE.ASM turns the 16 E0-prefixed scan codes in its table at seg063:0288 into 60h to 6Fh, and the scan code table `Asc` maps those. Screen y runs from the bottom up.

The main loop hands each pass to `INPUT.C`'s `input_dispatch`, which calls the first registered mouse region or key handler that takes the event and is live in the current screen mode (`inplist->mode`, sys.h's `MODE_GAME` 1, `MODE_MAP` 2, `MODE_CONV` 4). Mouse handles count up and key handles down, so a handle's sign says which table it is in. `DEBUG.C`'s `init_debug` binds Alt+F4 in every mode to `ovr127_0`, which raises the COM1 interrupt.

## The 3D view and the panels

The icons choose the interaction mode, `RightButtonThing` (ui.h's `IMODE_*`): 0 the default (look, and drag to pick up), 1 use, 2 fight, 3 look, 4 get, 5 talk; the sixth icon opens the options panel. `mous_in_3d` finds the object under the pointer with `pick_3d` and runs the mode's action through `player_disp`. `GameInputMode` (`GIM_*`) overrides the mode while something owns the pointer: 1 an object on the cursor, 2 a spell or object wanting a target (`ObjectActor`), 3 a missile spell being aimed.

`pick_3d` has the renderer draw a pick frame (VIEW3D.C's `do_3d_grab`), in which every object and face is drawn in a colour that names it (view3d.h's `PICK_*`): objects 1 to 0xBF, walls 0xC0 plus the texture, floors 0xF0 plus the texture, the ceiling 0xFA. The colour under the pointer gives the object through `color_to_obj` and its tile through `color_to_map`; a wall, floor or ceiling only notes its texture for `look_nothing`, which prints its description from string block 10.

Rules found in the code:

- **Reach** (`InPickRange`, `BlockingTerrain`). An object can be taken or used within a squared fine distance of `PickDist` (0x90) and a height window of 12 below to 24 above the player, doubled with a pole. The straight line to it must not rise above the player, nor cross another terrain than the one he stands on, unless a bridge carries it.
- **Identifying** (`inv_look`). The first look at an object in the inventory rolls `skill_check(Lore, 10) + 1` (at least 1) and keeps the best result in the object's heading, so it is not rolled again until something clears the marks.
- **The compass** (`print_info`) reports hunger, fatigue, the level, the day (`game_clock / 0x1C2000 / 12`) and one of twelve times of day.
- **Every frame** (`display_scr`): the flasks and compass are set, the screen frame flashes when the player took enough damage, the palette cycles, death is checked; once a second of game time the music may change, and every 21 seconds `duration_check` runs. On level 9 a random one in 32 frames runs the ethereal void's effects.

The right-hand panel shows the inventory, the rune bag or the statistics (`RightPanel`, gfx.h's `PANEL_*`). `PANELS.C` keeps the screen furniture: callers set a goal with `set_screen_frame(which, value)` (gfx.h's `SCR_*`), and `update_screen` steps each element towards it, some at once, some every 32 ticks and some every 64: the flasks in 12 steps, the compass in 16 headings, the power gem, the two dragons (three animations each), the turning panel, the gargoyle's eyes and the first-person weapon (`WEAP_*`: a swing of one of three kinds, drawing, ready, sheathing, sheathed). The weapon's pictures come from `WEAPONS.GR` by weapon kind and hand, with their positions from `DATA\weapons.dat`.

## The message scroll

`scroll_print` prints to the current scroll (the game's or a conversation's; UW1 has two) with word wrap and backslash escapes (`\0`..`\6` colours, pauses and a forced [MORE]). `scroll_more` asks for [MORE] when lines of the message being printed would scroll away. `wdialog` reads a typed line into the scroll and `wyorn` a yes or no. The scroll prints only in `MODE_GAME` and `MODE_CONV`.

## Strings

`STRINGS.PAK` is Huffman coded (`GAMESTRN.C` has the layout). A string id is `block << 9 | index`; ui.h has the block bases (`STR_GAME` the messages, `STR_CHARGEN`, `STR_BOOKS`, `STR_OBJNAMES`, `STR_OBJLOOK`, `STR_SPELLS`, `STR_CONV`, `STR_WRITING`, `STR_TRAPTEXT`, `STR_TEXTURES`; conversations 0xE00 + n, cutscenes 0xC00 + n). Block 0 means the current conversation's or cutscene's block, and blocks 0x7C and 0x7D are made at run time (a conversation's strings, the player's name). `game_sprint(n)` prints string n of block 1; most messages go out through it.

## Menus and options

`real_start` (`MAINMENU.C`) shows `DATA\OPSCR.BYT` with four buttons (introduction, create character, acknowledgements, journey onward) and cycles palette colours 0x40 to 0x7F while it waits. `OPTIONS.C` runs the options panel in the left-hand panel: seven pages (save, restore, music, sound effects, detail, quit, and the top page), each a drawing function and a handler for a pressed row. The Ctrl shortcuts press a top-page button at once.

## The automap

`PlayersMap` (`AUTOMAP.C`) is the player's map of the current level, a byte per tile, written by `GRIDDB.C` as tiles come into view: the tile type in bits 0 to 3 (0 never seen), the floor's terrain in bits 4 and 5, and in bits 6 and 7 the kind (`curautocode`: 1 a door, 2 and 3 drawn specially). Each level's map is block 0x1A + level of `SAVE0\LEV.ARK` (level.h's `LEVARK_AUTOMAP`) and its notes, up to 100 records of 0x36 bytes (`struct ATM`), block 0x23 + level (`LEVARK_NOTES`). On the map screen the player writes notes anywhere, erases them, and moves between levels.

## Open questions

- What the kinds 2 and 3 of a `PlayersMap` byte stand for exactly (`DoTile` draws one in the 0xE9 colours and the other dark).
- Whether the gargoyle eyes' sequence (`adjust_eyes`, `SCR_EYES`) reflects anything in the game; its callers have not been traced.
- What screen modes 8 and 0x10 were for: `place_3d_view` sets another zoom for 8, and the empty overlays ovr090 and ovr152 switch to 0x10.
- `ovr127_0`'s COM1 interrupt and `dprintf` are what is left of a debugging setup; what the development build did with them is unknown.
