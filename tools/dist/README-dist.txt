Ultima Underworld II, native port @VERSION@
https://github.com/abedegno/UW2Decomp

This is a native port of Ultima Underworld II: Labyrinth of Worlds (Looking Glass Technologies and Origin Systems, 1993), built from a byte-identical decompilation of the DOS game. It contains no game data. You need your own copy of the game: the GOG release ("Ultima Underworld II" or "Ultima Underworld 1+2") is the one it checks for.

STARTING IT

macOS: open UW2.app. The app is not signed by Apple, so the first time, right-click (or Control-click) UW2.app and choose Open, then Open again. If macOS still refuses, run this in Terminal once: xattr -dr com.apple.quarantine /path/to/UW2.app

Windows: run uw2port.exe. Windows SmartScreen may warn about an unknown publisher; choose "More info" and "Run anyway".

Linux: run ./uw2 from this folder (or bin/uw2port). It needs glibc 2.39 or later (Ubuntu 24.04 or newer, or a distribution of the same age).

FINDING THE GAME

On its first run the port looks for the game by itself: GOG's install folders (/Applications on macOS, C:\GOG Games and GOG Galaxy's Games folder on Windows, ~/GOG Games and ~/Games on Linux), the current folder and the program's own folder. If it finds only GOG's CD image (game.gog, as GOG's Mac and Windows installers ship it), it copies the game out of the image once into its home folder. If it finds nothing, it asks you to choose a folder: the one that holds UW2.EXE, the GOG install folder, or the GOG app. The folder is remembered.

From a command line you can name the folder instead: uw2port --data /path/to/UW2. The environment variable UW2PORT_DATA does the same.

The port never writes to the game folder. Saved games, the sound settings and the port's settings (uw2port.cfg) go to its home folder: ~/.uw2port on macOS and Linux, and %APPDATA%\uw2port on Windows (or %HOME%\.uw2port where HOME is set, as in MSYS2). --home DIR or UW2PORT_HOME chooses another.

PLAYING

The controls are the game's own: the mouse, and the keys the game's manual lists (GOG includes the manual). Closing the window quits.

Sound: the first run sets up a Sound Blaster with its digitised effects. --sound MUSIC,SPEECH chooses other cards and is remembered: music 0 none, 2 Ad Lib, 3 Sound Blaster, 4 Sound Blaster Pro 1, 5 Roland MT-32, 6 Pro Audio Spectrum, 7 Sound Blaster Pro 2; speech 0 none, 1 Sound Blaster, 2 Sound Blaster Pro, 3 Pro Audio Spectrum. For example --sound 7,2 is a Sound Blaster Pro 2.

Roland MT-32 music needs your own MT-32 or CM-32L ROM images, which are not included: --sound 5,1 --mt32-roms DIR, where DIR holds CM32L_CONTROL.ROM and CM32L_PCM.ROM, or MT32_CONTROL.ROM and MT32_PCM.ROM. The folder is remembered.

Window: --scale N sets the starting size (3), --no-aspect shows square pixels instead of the 4:3 shape of a CRT, --no-integer scales freely. uw2port --help lists every option.

LICENCES

LICENSE.txt: the MIT licence, for this project's own work only. NOTICE.txt: Ultima Underworld II belongs to its owners, and the decompiled game code is derived from it. THIRD-PARTY-NOTICES.txt: the libraries in this package (SDL3, Nuked OPL3, libmt32emu and, on Windows, the compiler's runtime), with their licence texts in licenses/. Nuked OPL3 and libmt32emu are LGPL libraries and are separate shared libraries here, which you may replace with your own builds; THIRD-PARTY-NOTICES.txt says where their source is.
