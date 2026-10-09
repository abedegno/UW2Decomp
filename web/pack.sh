#!/bin/sh
# Packs one game's files and the MT-32 ROMs for the web page (docs/WEB.md), from the user's own
# copies, into SITE/GAME/: GAME.data and GAME.data.js (Emscripten's file packager), the game at
# /game and the ROMs at /game/roms, without the game's SAVE folders. The data never goes anywhere
# but SITE: never into a repository other than the site's (web/deploy.sh).
# usage: web/pack.sh uw1|uw2 GAME_DIR ROMS_DIR SITE_DIR
set -e
[ $# -eq 4 ] || { echo "usage: web/pack.sh uw1|uw2 GAME_DIR ROMS_DIR SITE_DIR" >&2; exit 2; }
g=$1; src=$2; roms=$3; site=$4
# Never into a git work tree (this repository's or any other): the data would be one `git add` away
# from a commit. The site is checked at its nearest folder that exists, before anything is written.
d=$site; while [ ! -d "$d" ]; do d=$(dirname "$d"); done
if git -C "$d" rev-parse --git-dir >/dev/null 2>&1; then
  echo "web/pack.sh: refusing $site: it is inside a git repository (use a folder outside any, e.g. mktemp -d)" >&2; exit 1
fi
. "$HOME/emsdk/emsdk_env.sh" >/dev/null 2>&1
tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
cp -R "$src"/. "$tmp/game"
rm -rf "$tmp"/game/SAVE* "$tmp"/game/*.pristine
mkdir -p "$tmp/game/roms" && cp "$roms"/* "$tmp/game/roms/"
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
