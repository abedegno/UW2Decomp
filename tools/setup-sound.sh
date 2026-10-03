#!/bin/sh
# Fetch the port's OPL emulator, Nuked OPL3 (github.com/nukeykt/Nuked-OPL3, LGPL-2.1), into
# tools/nuked-opl3 (ignored by git) at a pinned commit, and check the two files by SHA-256. The
# port's OPL music backend (src/port/sound/opl.c) compiles opl3.c from there; without it the
# port builds with no OPL chip and plays no FM music (docs/BUILDING.md, "Sound"). Nothing of it
# is committed: the repository has no license yet and vendors no third-party source.
# Needs curl. Safe to run again.   usage: tools/setup-sound.sh   (make setup-sound)
#
# The MT-32 backend uses libmt32emu from Homebrew (brew install mt32emu, LGPL-2.1-or-later),
# found through pkg-config when the port is built; this script only says whether it is there.
set -e
root=$(cd "$(dirname "$0")/.." && pwd); dir="$root/tools/nuked-opl3"
commit=765ec962e473aeb767e4cba74ffdc8f588ffbfe8
sum_c=59eb873fdb6d52bc7977a0fcbb97c1bae03dd71cc88e3acddbcc01b7121bfe03
sum_h=a84266b8d71a4929f15f573afbe407fd24310b77757d855835dacabf68763679
sha() { shasum -a 256 "$1" 2>/dev/null | cut -d' ' -f1 || sha256sum "$1" | cut -d' ' -f1; }
ok() { [ -f "$dir/opl3.c" ] && [ -f "$dir/opl3.h" ] && [ "$(sha "$dir/opl3.c")" = "$sum_c" ] && [ "$(sha "$dir/opl3.h")" = "$sum_h" ]; }
if ! ok; then
  mkdir -p "$dir"
  for f in opl3.c opl3.h LICENSE; do
    curl -fsSL "https://raw.githubusercontent.com/nukeykt/Nuked-OPL3/$commit/$f" -o "$dir/$f.tmp" && mv "$dir/$f.tmp" "$dir/$f"
  done
  ok || { echo "nuked-opl3: the files fetched do not match their pinned SHA-256"; exit 1; }
fi
echo "nuked-opl3: tools/nuked-opl3 (nukeykt/Nuked-OPL3 at ${commit%"${commit#???????}"}, LGPL-2.1)"
if pkg-config --exists mt32emu 2>/dev/null; then
  echo "mt32emu: $(pkg-config --modversion mt32emu) (LGPL-2.1-or-later), the MT-32 backend is built"
else
  echo "mt32emu: not found (brew install mt32emu for the MT-32 and CM-32L backend)"
fi
