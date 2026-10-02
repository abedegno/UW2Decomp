# The user interface and input

This page describes how UW2 reads the mouse, keyboard and joystick and routes them to handlers, the 3D view's interaction modes, the screen furniture around the view, the message scroll, the main menu and options, the game's strings and the automap. The sources are in `src/ui`; the declarations are in `src/include/ui.h`.

## Files

| File | Segment | Name | What it does |
|---|---|---|---|
| [`INPUT.C`](../../src/ui/INPUT.C) | seg010 | original (`init_input` is in System Shock's `INPUT.C`) | the input dispatcher: mouse regions and key handlers by screen mode |
| [`MOUSE.C`](../../src/ui/MOUSE.C) | seg015 | inferred (System Shock's `MOUSE.C` has the same calls) | the pointer, its cursor pictures and save-under, the input queue, the keyboard reader |
| [`JOYSTICK.C`](../../src/ui/JOYSTICK.C) | seg011 | descriptive | the joystick (DOS only) |
| [`ICONS.C`](../../src/ui/ICONS.C) | seg014 | descriptive | the six command icons and the options panel's button groups |
| [`INTERACT.C`](../../src/ui/INTERACT.C) | seg026 | descriptive | clicks in the 3D view and the panels: picking, reach, get, look, use, talk, fight |
| [`PANELS.C`](../../src/ui/PANELS.C) | seg037 | descriptive | the flasks, compass, power gem, eyes, first-person weapon, rune shelf, sliding panel |
| [`GAMESCR.C`](../../src/ui/GAMESCR.C) | ovr137 | descriptive | the game screen's set-up, the compass and flask messages, the save and rest guards |
| [`STATS.C`](../../src/ui/STATS.C) | ovr158 | descriptive | the statistics page of the character panel |
| [`SCROLL.C`](../../src/ui/SCROLL.C) | seg043 | descriptive | the message scroll: printing, wrapping, escapes |
| [`SCROLLIO.C`](../../src/ui/SCROLLIO.C) | ovr139 | descriptive | the scroll's input side: [MORE], typed lines, yes or no |
| [`GAMESTRN.C`](../../src/ui/GAMESTRN.C) | seg039 | original (`init_strings`, `get_string` in System Shock's `GAMESTRN.C`) | `STRINGS.PAK` and the message printers |
| [`MAINMENU.C`](../../src/ui/MAINMENU.C) | ovr147 | descriptive | the main menu |
| [`WRAPPER.C`](../../src/ui/WRAPPER.C) | ovr136 | original (`draw_button` is in System Shock's `WRAPPER.C`) | the options panel |
| [`AUTOMAP.C`](../../src/ui/AUTOMAP.C) | ovr094 | inferred (System Shock's `AUTOMAP.C`) | the automap, its notes and map scraps |
| [`BROWSE.C`](../../src/ui/BROWSE.C) | ovr099 | inferred (the `_browse` names) | seven empty hooks nothing calls |

Function and global names are the FM Towns originals where that build has them. `map/filenames.tsv` has the evidence for each file name.

## Input

Every wait loop in the game reads input through `MOUSE.C`'s `get_input` (via `mouse_get_input` and `mouse_get_input_sp`), which alternates between the mouse and the keyboard and returns 1 to 3 for the buttons held, a key code with `KEY_CTRL`, `KEY_ALT` and `KEY_SHIFT` added (`ui.h`), or -1. The mouse's y runs from the bottom of the screen up. `moveMouse` polls the driver, or the joystick when `IsJoy` is set, and redraws the cursor; INS and DEL act as the mouse buttons and the keypad moves the pointer.

The main loop hands each pass to `INPUT.C`'s `input_dispatch`, which finds the first registered mouse region or key handler that takes the event and is live in the current screen mode (`inplist->mode`: 1 the 3D view, 2 the automap, 4 a conversation), fills in `struct Inplist` (position relative to the region, buttons, key) and calls it. Screens register their regions with `input_addmouse` and keys with `_input_addkey`; `game/PLAYER.C`'s `init_player` registers the game's keys.

## The 3D view and the panels

The icons (`ICONS.C`) choose the interaction mode, `RightButtonThing`: 0 the default (look, and drag to pick up), 1 use, 2 fight, 3 look, 4 get, 5 talk; the sixth icon opens the options. `INTERACT.C`'s `mous_in_3d` finds the object under the pointer from the renderer's pick buffer (`pick_3d`: each drawn object has a colour in `stdat`, mapped back to the object and its tile by `color_to_obj` and `color_to_map`) and runs the mode's action. `GameInputMode` overrides the mode while something owns the pointer: 1 an object on the cursor, 2 a targeted spell or an object being used on another (`ObjectActor`), 3 a missile spell being aimed.

Rules found in the code:

- **Reach** (`InPickRange`, `BlockingTerrain`). An object can be taken or used within a squared distance of 0x90 fine units and a height window of 12 below to 24 above the player (doubled with a pole). The straight line to it must not climb above the player or cross a different terrain class (water, lava, ice) from the one he stands on, unless a bridge carries it.
- **Identifying** (`inv_look`). The first look at an object in the inventory rolls `skill_check(Lore, 8) + 1`; the result is kept in the object's heading so it is not rolled again until the Lore skill rises.
- **Taking** a book (0x138) that belongs to someone sets a quest bit and is a crime against the humans; picking up a moonstone frees its Gate Travel record.

The right-hand panel shows the inventory, the rune bag or the statistics (`RightPanel`; `mous_in_panel` routes clicks). `PANELS.C` keeps the screen furniture: callers set a goal with `set_screen_frame(which, value)` and `update_screen` steps each element towards it every frame (the flasks in 12 steps, the compass in 16 headings, the power gem, the sliding panel, the gargoyle eyes, the first-person weapon's draw, swing and sheathe frames from `WEAP.GR` and `DATA\weap.dat`).

## The message scroll

`scroll_print` (`SCROLL.C`) prints to the current scroll (the game's, a conversation's or the menu's) with word wrap and backslash escapes: `\0`..`\6` colours, `\p` and `\P` pauses, `\m` a forced [MORE]. A full scroll asks for [MORE] (`SCROLLIO.C`'s `scroll_more`) only when the lines that would scroll away belong to the message being printed. `wdialog` reads a typed line into the scroll (quantities, save descriptions, the player's words in conversations) and `wyorn` a yes or no.

## Strings

`STRINGS.PAK` is Huffman-coded (`GAMESTRN.C` has the layout). A string id is `block << 9 | index`; `ui.h` has the block bases (`STR_GAME` block 1 the messages, `STR_CHARGEN` 2, `STR_BOOKS` 3, `STR_OBJNAMES` 4, `STR_OBJLOOK` 5, `STR_SPELLS` 6, `STR_CONV` 7, `STR_WRITING` 8, `STR_TRAPTEXT` 9, `STR_TEXTURES` 10; conversations are 0xE00 + n). Block 0 means the current conversation's or cutscene's block, and blocks 0x7C and 0x7D are made at run time (a conversation's strings, the player's name). `game_sprint(n)` prints string n of block 1; most messages go out through it. The C sources mark many `game_sprint` calls with the text they print.

## Menus and options

`real_start` (`MAINMENU.C`) shows the four menu buttons (introduction, create character, acknowledgements, journey onward). `WRAPPER.C`'s `busywaiting_new_options` runs the options panel: pages of up to seven buttons (`struct buttongroup`, `ui.h`; the groups themselves are `ICONS.C`'s data) for saving, restoring, detail, music, sound and quitting.

## The automap

`PlayersMap` (`AUTOMAP.C`) is the player's map of the current level, a byte per tile: the tile type in the low four bits (0 never seen), a kind in bits 4 and 5 (a door, water or dark), and the floor's terrain class in bits 6 and 7. Each level's map is block 0x9F + level of the save directory's `LEV.ARK`, and its notes, up to 100 of 0x36 bytes (`struct ATM`), block 0xEF + level. On the map screen the player writes notes, erases them, moves between the levels of a world and picks other worlds on the gem (lit for the worlds visited, `QB_WORLDS_VISITED`). A map scrap's map is kept as the automap of level 0x47 + its quality; copying it (`update_map_scraps`) adds the sections it covers to the player's map and gives a little experience for each new tile.

## Open questions

- What the 0x20 and 0x30 kinds of a `PlayersMap` byte mean exactly (`DoTile` draws one with colours 0xE9.. and the other dark).
- `BROWSE.C`'s seven hooks were compiled out of both builds; what they browsed is unknown.
- The roles of several `struct Scroll` fields (`ui.h`) are named by use only.
- Whether the gargoyle eyes' sequence (`adjust_eyes`) reflects anything in the game, such as the player being seen; the callers of `set_screen_frame(7, ...)` have not been traced.
