#!/bin/sh
# Packs one game's files and the MT-32 ROMs for the web page (docs/WEB.md), from the user's own
# copies, into SITE/GAME/: GAME.data and GAME.data.js (Emscripten's file packager), the game at
# /game and the ROMs at /game/roms, without the game's SAVE folders. The data never goes anywhere
# but SITE: never into a repository other than the site's (web/deploy.sh).
# usage: web/pack.sh uw1|uw2 GAME_DIR ROMS_DIR SITE_DIR
set -e
[ $# -eq 4 ] || { echo "usage: web/pack.sh uw1|uw2 GAME_DIR ROMS_DIR SITE_DIR" >&2; exit 2; }
g=$1; src=$2; roms=$3; site=$4
. "$HOME/emsdk/emsdk_env.sh" >/dev/null 2>&1
tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
cp -R "$src"/. "$tmp/game"
rm -rf "$tmp"/game/SAVE* "$tmp"/game/*.pristine
mkdir -p "$tmp/game/roms" && cp "$roms"/* "$tmp/game/roms/"
mkdir -p "$site/$g"
# --export-name=Module: the script hangs its loader on a global Module's preRun, which page.js
# hands to the port's factory (page.js, startGame). Its warning about -sFORCE_FILESYSTEM is
# shown only on a failure: the port's FS is exported, which is what that warning asks for.
python3 "$EMSDK/upstream/emscripten/tools/file_packager.py" "$site/$g/$g.data" --preload "$tmp/game@/game" \
  --js-output="$site/$g/$g.data.js" --use-preload-cache --no-node --export-name=Module >/dev/null 2>"$tmp/err" || { cat "$tmp/err" >&2; exit 1; }
echo "packed $g: $(du -h "$site/$g/$g.data" | cut -f1)"
