#!/bin/sh
# Packs one game's files and the MT-32 ROMs for the web page (docs/WEB.md), from the user's own
# copies, into SITE/GAME/: GAME.data and GAME.data.js (Emscripten's file packager), the game at
# /game and the ROMs at /game/roms, without the game's SAVE folders or a desktop's stray files
# (.DS_Store and the like). Of the ROMs only the pair the port would play from goes in: the
# control and PCM images that the runtime's own choice (mt32roms_pick, run by web/romsel.c)
# takes from ROMS_DIR, whatever else that folder holds. The data never goes anywhere but SITE:
# never into a repository other than the site's (web/deploy.sh).
# usage: web/pack.sh uw1|uw2 GAME_DIR ROMS_DIR SITE_DIR
set -e
[ $# -eq 4 ] || { echo "usage: web/pack.sh uw1|uw2 GAME_DIR ROMS_DIR SITE_DIR" >&2; exit 2; }
g=$1; src=$2; roms=$3; site=$4
here=$(cd "$(dirname "$0")/.." && pwd)
case $g in uw1|uw2) ;; *) echo "usage: web/pack.sh uw1|uw2 GAME_DIR ROMS_DIR SITE_DIR" >&2; exit 2;; esac
# Never into a git work tree (this repository's or any other): the data would be one `git add` away
# from a commit. The site is checked at its nearest folder that exists, before anything is written.
d=$site; while [ ! -d "$d" ]; do d=$(dirname "$d"); done
command -v git >/dev/null || { echo "web/pack.sh: no git, so it cannot check $site is outside any repository" >&2; exit 1; }
if err=$(LC_ALL=C git -C "$d" rev-parse --git-dir 2>&1); then
  echo "web/pack.sh: refusing $site: it is inside a git repository (use a folder outside any, e.g. mktemp -d)" >&2; exit 1
fi
case $err in *"not a git repository"*) ;; *) echo "web/pack.sh: cannot tell whether $site is in a git repository: $err" >&2; exit 1;; esac
. "$HOME/emsdk/emsdk_env.sh" >/dev/null 2>&1
tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
cp -R "$src"/. "$tmp/game"
rm -rf "$tmp"/game/SAVE* "$tmp"/game/*.pristine
# a desktop's own files, anywhere in the copy: macOS's .DS_Store and ._ resource forks, Windows'
# Thumbs.db and desktop.ini
find "$tmp/game" \( -name .DS_Store -o -name '._*' -o -iname thumbs.db -o -iname desktop.ini \) -exec rm -rf {} +
# The ROM pair: web/romsel.c, built here with emcc against the game's web libmt32emu (the one
# the page's port links) and run under Node.js, prints the paths of the images the port's
# mt32roms_pick chooses in ROMS_DIR (two for an image in halves), recognised by content.
libs=$here/$g/tools/libs-web
[ -f "$libs/lib/libmt32emu.a" ] || { echo "web/pack.sh: no $libs/lib/libmt32emu.a (SETUP_WEB=1 tools/setup-libs.sh, docs/WEB.md)" >&2; exit 1; }
rt=$here/exhume/runtime/port
cc1() { emcc -O1 -pthread -I"$rt" -I"$rt/platform" -I"$here/exhume/runtime/tests" -I"$libs/include" -DAUDIO_HAVE_MT32EMU -c "$1" -o "$2"; }
{ cc1 "$here/web/romsel.c" "$tmp/romsel.o" && cc1 "$rt/sound/mt32roms.c" "$tmp/mt32roms.o" &&
  em++ -O1 -pthread "$tmp/romsel.o" "$tmp/mt32roms.o" "$libs/lib/libmt32emu.a" -sENVIRONMENT=node -sNODERAWFS=1 \
    -sEXIT_RUNTIME=1 -o "$tmp/romsel.cjs"; } >"$tmp/err" 2>&1 || { cat "$tmp/err" >&2; echo "web/pack.sh: cannot build web/romsel.c" >&2; exit 1; }
node "$tmp/romsel.cjs" "$roms" >"$tmp/roms.txt" || { echo "web/pack.sh: no ROM pair to pack from $roms" >&2; exit 1; }
mkdir -p "$tmp/game/roms"
while IFS= read -r f; do [ -n "$f" ] && cp "$f" "$tmp/game/roms/"; done <"$tmp/roms.txt"
echo "roms for $g: $(cd "$tmp/game/roms" && ls | tr '\n' ' ')"
mkdir -p "$site/$g"
# Run from inside SITE/GAME with the names relative: the packager writes the .data name it is
# given into GAME.data.js (its PACKAGE_NAME, the IndexedDB cache's key too), so an absolute one
# would publish a local path. --export-name=Module: the script hangs its loader on a global
# Module's preRun, which page.js hands to the port's factory (page.js, startGame). Its warning
# about -sFORCE_FILESYSTEM is shown only on a failure: the port's FS is exported, which is what
# that warning asks for.
(cd "$site/$g" && python3 "$EMSDK/upstream/emscripten/tools/file_packager.py" "$g.data" --preload "$tmp/game@/game" \
  --js-output="$g.data.js" --use-preload-cache --no-node --export-name=Module) >/dev/null 2>"$tmp/err" || { cat "$tmp/err" >&2; exit 1; }
echo "packed $g: $(du -h "$site/$g/$g.data" | cut -f1)"
