# The user interface: candidates for the findings page

Candidates from `src/ui` for the project's findings page; [ui.md](ui.md) describes the subsystem. Each bug candidate was re-read in the matched source (and, where it says so, in the listing's bytes). What the code does is read from the code; the effect in the game is an inference unless it says it was checked.

## Likely bugs

### The dragons' idle tail swish asks for a flask redraw instead

- **What happens:** every 64 ticks `update_screen` may start a flask bubbling and a dragon's tail swishing. For the bubbles it sets the flask's bit, `1 << (r & 1)`. For the tail it sets `swishing[r & 1]` and then `slow_adjust |= 1 << (r + 4 & 1)`. In C, `+` binds tighter than `&`, so the shift is `(r + 4) & 1`, which is again 0 or 1: the bit of a flask (`SCR_VITALITY`, `SCR_MANA`), not of a dragon (`SCR_DRAGON`, `SCR_DRAGON2`, bits 4 and 5).
- **Where:** `update_screen` in [ui/PANELS.C](../../src/ui/PANELS.C).
- **Evidence:** the bytes (UW1_asm.asm, seg036_3087_69B: `add cl,4; and cl,1; shl ax,cl`). The bubble line just above uses the element's own bit, and `adjust_dragons` is the only routine that draws a swish, so `(r & 1) + 4` is evidently what was meant. UW2 has no dragons.
- **Confidence:** likely.
- **Effect:** the flask's adjust runs, finds nothing to do and clears its bit. The swish stays pending in `swishing[]` until something else sets the dragon's bit, which happens when `set_screen_frame(SCR_DRAGON, n)` starts a dragon animation (taking damage, scroll edges rolling); the tail then swishes along with it. So the dragons probably never swish their tails on their own. Not checked in the game.
- **For a port:** matching DOS means keeping the wrong bit; the intended behaviour is idle tail swishing.

## Engine findings

- **`pick_3d` reads the pick counter as a word.** `*p < *(int16 *)&PickUp` compares the colour under the pointer with `PickUp` (a byte) and the byte after it. That byte, DS:311F, is `_BSS` alignment padding before `mlowptr`, which no variable occupies and which stays 0 (C0 clears `_BSS`), so the test works as meant. A port that moves `PickUp` must keep the test a byte test. ([ui/INTERACT.C](../../src/ui/INTERACT.C))
- **The cursor block has its own key codes.** The grey Home, Up, PgUp, Left, Right, End, Down and PgDn give 0xA5 to 0xAC (ui.h's `KEY_G*`), different from the keypad's 0x8C to 0x94; Ins and Del give the keypad's codes. The main menu, the options panel and the scroll's line editor accept both sets. (seg019's KEYQUEUE.ASM table at seg063:0288 and `Asc`.)
- **Debug hooks left in the shipped game.** Alt+F4, bound in every screen mode by `DEBUG.C`'s `init_debug`, raises int 0Ch (COM1's IRQ 4) in software through `ovr127_0`. With no COM1 handler installed this does whatever the BIOS or DOS handler does, normally nothing. `INPUT.C`'s `input_del` reports a bad key handle through `dprintf`, which is empty.
- **Two scrolls.** UW1 prints to the game's scroll or a conversation's; there is no menu scroll, and the scroll prints nothing outside `MODE_GAME` and `MODE_CONV`.

## Dead code

- `OVR096.C` (ovr096): seven browse hooks, six empty and one returning 1, byte for byte UW2's `BROWSE.C`; only their overlay stubs refer to them.
- `OVR137.C` (ovr137): one empty function nothing calls.
- `INTERACT.C`'s `seg026_2716_F8A` (static) and `seg026_2716_F98`: inventory helpers no code calls.
- `OPTIONS.C`'s `options_mouse_click` increments x and never reads it again (kept for the bytes).

## Open questions

- The kinds 2 and 3 in a `PlayersMap` byte (`DoTile`).
- What the gargoyle eyes' goal (`SCR_EYES`) is set from.
- Screen modes 8 and 0x10: only `place_3d_view`'s zoom and the empty overlays' `newscr(16)` refer to them.
