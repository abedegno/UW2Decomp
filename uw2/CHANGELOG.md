# Changelog: Ultima Underworld II port (uw2port)

For players. Each release's notes are drafted from here. Releases are tagged `uw2-vX.Y.Z` (before the move to Underworld Exhumed, `vX.Y.Z`).

## 1.2.1 (unreleased)

- **New:** optional enhancements, all off by default (`--enhance list`; docs/ENHANCEMENTS.md). The first is `skip-intro`, which starts at the main menu. A session played with one says so in its log and recording.
- **New:** more optional enhancements: wrap-menu, fast-panels, free-heading, subtitles, skill-messages (`--enhance list`).
- **New:** optional enhancements perspective (floors and ceilings stay straight when you look up or down) and full-sprites (`--enhance list`).
- **New:** optional enhancements wide-pitch (look three times as far up and down, with no gaps in the floor or ceiling), mouse-look (the ` key: the mouse turns the view, a crosshair at the centre; `look-speed=` in the settings file) and invert-look (`--enhance list`).
- **New:** optional enhancements modern-keys (UltimaHacks' keys: WASD, Space to attack, Q and E to look and use, Z for the map, R and F for the panels) and rune-keys (Ctrl+Alt and a letter puts a rune on the shelf) (`--enhance list`).
- **New:** the port finds your MT-32 or CM-32L ROMs by itself: in a `roms` folder in its home folder, beside the game or the program, or where DOSBox keeps MT-32 ROMs. Any file names work (they are recognised by their contents), split halves are joined, and `--mt32-roms` takes a folder or one of the files. Or drop the ROM folder onto the app, the program or its window, and the music plays on the MT-32 from the next start; it moves there by itself, too, when ROMs turn up after a first run that had none.
- **New:** a settings screen: F11 (or Cmd+, on macOS) opens it over the game, with the sound cards, volume, mouse, window, enhancements and the game folder, and the first run starts with it (docs/SETTINGS.md).
- **Fixed:** the game could freeze for good, most often on some Windows PCs, after the program was paused by the system for a second or more. A creature update loop in the original game never ended after such a pause; the port now caps the step, which changes nothing the DOS game can do.
- **Fixed:** high CPU and GPU use, and a mouse pointer that stalled in other programs, when the graphics driver ignores vsync or the window is in the background. The port now never draws faster than the display refreshes, and draws nothing while minimised or hidden.
- **Changed:** the log says which renderer and refresh rate the port uses and whether vsync held, to help with bug reports.
- **Fixed:** the Linux build could crash in its first conversation, when the game printed an empty line (the original game reads the byte before an empty string; the port now skips that test for one).
- **Checked:** every release's packages are now tested before release on nine systems (Windows Server 2022 and 2025, macOS 14, 15 and 15 on Intel, Ubuntu 22.04 and 24.04, Debian 12, Fedora 41), replaying the recorded sessions from the package itself, with the MT-32 music checked too.

## 1.2.0 (4 October 2026)

- **Fixed:** going up some stairs could hang the game.
- **Fixed:** the floor could turn red after Lord British's cutscene.
- **Fixed:** walking backwards along corridors could get stuck.
- **Fixed:** crash recordings of long sessions were far larger than needed; they are now small.
