Ultima Underworld, native port @VERSION@
https://github.com/abedegno/underworld-exhumed

This is a native port of Ultima Underworld: The Stygian Abyss (Blue Sky Productions and Origin Systems, 1992), built from a byte-identical decompilation of the DOS game. It contains no game data. You need your own copy of the game: the GOG release ("Ultima Underworld 1+2", or "Ultima Underworld: The Stygian Abyss") is the one it checks for.

STARTING IT

macOS: open UW1.app (macOS 11 or later, Apple silicon or Intel). @MACOS_OPEN@

Windows: run uw1port.exe. Windows SmartScreen may warn about an unknown publisher; choose "More info" and "Run anyway".

Linux: the AppImage (UW1-...-linux-x86_64.AppImage) is one file: make it executable (chmod +x) and run it. From the tarball, run ./uw1 from its folder (or bin/uw1port); uw1.desktop and uw1.png there are a menu entry and its icon, if you want one (put the folder's path in the Exec line and copy the file to ~/.local/share/applications). Both need glibc 2.35 or later (Ubuntu 22.04 or newer, or a distribution of the same age) and X11 or Wayland.

FINDING THE GAME

On its first run the port looks for the game by itself: the current folder and the program's own folder, then, on Windows, the folders GOG's installers record in the registry, then GOG's install folders (/Applications on macOS, C:\GOG Games and GOG Galaxy's Games folder on Windows, ~/GOG Games and ~/Games on Linux). If it finds only GOG's CD image (game.gog, as GOG's Mac app ships it), it copies the game out of the image once into its home folder. If it finds nothing, it asks you to choose a folder: the one that holds UW.EXE, the GOG install folder, or the GOG app. The folder is remembered.

From a command line you can name the folder instead: uw1port --data /path/to/UW1. The environment variable UW1PORT_DATA does the same.

The port never writes to the game folder. Saved games, the sound settings and the port's settings (uw1port.cfg) go to its home folder: ~/.uw1port on macOS and Linux, and %APPDATA%\uw1port on Windows (or %HOME%\.uw1port where HOME is set, as in MSYS2). --home DIR or UW1PORT_HOME chooses another.

PLAYING

The controls are the game's own: the mouse, and the keys the game's manual lists (GOG includes the manual). Closing the window quits.

Sound: the first run sets up a Sound Blaster, its FM music and its digitised speech (--sound 3,1); or, when you have given MT-32 ROMs (below) by then, the Roland MT-32 for the music and the Sound Blaster for the speech (--sound 6,1). --sound MUSIC,SPEECH chooses other cards and is remembered: music 0 none, 1 PC speaker, 2 Ad Lib, 3 Sound Blaster, 4 Sound Blaster Pro, 5 Pro Audio Spectrum, 6 Roland MT-32; speech 0 none, 1 Sound Blaster, 2 Sound Blaster Pro, 3 Pro Audio Spectrum. For example --sound 4,2 is a Sound Blaster Pro.

Roland MT-32 music needs your own MT-32 or CM-32L ROM images, which are not included: --sound 6,1. The port finds them by itself in a roms or mt32-roms folder in its home folder, beside the game or the program, or where DOSBox Staging keeps MT-32 ROMs; or give --mt32-roms a folder or one of the files. Any file names work: the ROMs are recognised by their contents, and a CM-32L pair is preferred to an MT-32 pair. The folder is remembered.

Window: --scale N sets the starting size (3), --no-aspect shows square pixels instead of the 4:3 shape of a CRT, --no-integer scales freely, --mouse lock captures the pointer as DOSBox does (Ctrl+F10 releases it). uw1port --help lists every option.

LICENCES

LICENSE.txt: the MIT licence, for this project's own work only. NOTICE.txt: Ultima Underworld belongs to its owners, and the decompiled game code is derived from it. THIRD-PARTY-NOTICES.txt: the libraries in this package (SDL3, Nuked OPL3, libmt32emu and, on Windows, the compiler's runtime), with their licence texts in licenses/. Nuked OPL3 and libmt32emu are LGPL libraries and are separate shared libraries here, which you may replace with your own builds; THIRD-PARTY-NOTICES.txt says where their source is.
