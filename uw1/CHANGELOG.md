# Changelog: Ultima Underworld port (uw1port)

For players. Each release's notes are drafted from here. Releases are tagged `uw1-vX.Y.Z`.

## 1.0.0 (unreleased)

The first release of the native port of *Ultima Underworld: The Stygian Abyss* for macOS, Windows and Linux.

- Plays the game from your own GOG copy, found by itself (including inside GOG's Mac app), with FM, MT-32 (your own ROMs) and Sound Blaster speech.
- **New:** the port finds your MT-32 or CM-32L ROMs by itself: in a `roms` folder in its home folder, beside the game or the program, or where DOSBox keeps MT-32 ROMs. Any file names work (they are recognised by their contents), and `--mt32-roms` takes a folder or one of the files.
- Ten recorded DOS sessions replay in it identically to DOS, on all three systems.
- **Checked:** the packages are tested before release on nine systems (Windows Server 2022 and 2025, macOS 14, 15 and 15 on Intel, Ubuntu 22.04 and 24.04, Debian 12, Fedora 41), replaying the recorded sessions from the package itself, with the MT-32 music checked too.
- **Fixed before release:** the same two problems found in the UW2 port in October 2026: a freeze for good after the program was paused for a second or more, and runaway drawing when vsync is ignored or the window is in the background.
