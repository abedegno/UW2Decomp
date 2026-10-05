# Changelog: Ultima Underworld II port (uw2port)

For players. Each release's notes are drafted from here. Releases are tagged `uw2-vX.Y.Z` (before the move to Underworld Exhumed, `vX.Y.Z`).

## 1.2.1 (unreleased)

- **Fixed:** the game could freeze for good, most often on some Windows PCs, after the program was paused by the system for a second or more. A creature update loop in the original game never ended after such a pause; the port now caps the step, which changes nothing the DOS game can do.
- **Fixed:** high CPU and GPU use, and a mouse pointer that stalled in other programs, when the graphics driver ignores vsync or the window is in the background. The port now never draws faster than the display refreshes, and draws nothing while minimised or hidden.
- **Changed:** the log says which renderer and refresh rate the port uses and whether vsync held, to help with bug reports.
- **Checked:** every release's Windows package is now tested on Windows Server 2022 before release, replaying the recorded sessions from the zip itself.

## 1.2.0 (4 October 2026)

- **Fixed:** going up some stairs could hang the game.
- **Fixed:** the floor could turn red after Lord British's cutscene.
- **Fixed:** walking backwards along corridors could get stuck.
- **Fixed:** crash recordings of long sessions were far larger than needed; they are now small.
