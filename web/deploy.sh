#!/bin/sh
# Builds both games for the web, packs the deployer's own game files and MT-32 ROMs, checks the
# page, and with --push force-pushes the site to SITE_REPO's gh-pages branch (docs/WEB.md). The
# game data goes there and nowhere else. Without --push it is a dry run: it builds, assembles and
# checks the site, says what it would push and where, and contacts nothing.
# usage: web/deploy.sh [--push] [--skip-replays] OWNER/NAME UW1_DIR UW2_DIR ROMS_DIR
#   --skip-replays  only make web, not make webcheck (every session replayed under Node.js,
#                   minutes): for a quick redeploy of builds already checked. The page checks
#                   (tools/webcheck.mjs) always run.
set -e
usage() { echo "usage: web/deploy.sh [--push] [--skip-replays] OWNER/NAME UW1_DIR UW2_DIR ROMS_DIR" >&2; exit 2; }
push=; replays=1
while [ $# -gt 0 ]; do
  case $1 in --push) push=1;; --skip-replays) replays=;; -*) usage;; *) break;; esac; shift
done
[ $# -eq 4 ] || usage
repo=$1; uw1=$2; uw2=$3; roms=$4
case $repo in */*/*|/*|*/|*[!A-Za-z0-9._/-]*) usage;; */*) ;; *) usage;; esac
for d in "$uw1" "$uw2" "$roms"; do [ -d "$d" ] || { echo "web/deploy.sh: no folder $d" >&2; exit 2; }; done
here=$(cd "$(dirname "$0")/.." && pwd)
owner=${repo%/*}; name=${repo#*/}
url="https://$owner.github.io/$name/"

if [ -n "$replays" ]; then
  make -C "$here/uw1" web webcheck && make -C "$here/uw2" web webcheck
else
  make -C "$here/uw1" web && make -C "$here/uw2" web
fi

site=$(mktemp -d); trap 'rm -rf "$site"' EXIT
# the site never inside a git work tree but its own (web/pack.sh refuses one too)
if git -C "$site" rev-parse --git-dir >/dev/null 2>&1; then
  echo "web/deploy.sh: refusing $site: it is inside a git repository" >&2; exit 1
fi
cp "$here"/web/*.html "$here"/web/*.js "$here"/web/*.css "$site"/
for g in uw1 uw2; do mkdir -p "$site/$g" && cp "$here/$g/build/web/${g}port."* "$site/$g/"; done
sh "$here/web/pack.sh" uw1 "$uw1" "$roms" "$site" && sh "$here/web/pack.sh" uw2 "$uw2" "$roms" "$site"
node "$here/exhume/tools/webcheck.mjs" "$site"
touch "$site/.nojekyll"

# No local path in anything published: the home folder, any /Users/ path, the temporary folders
# (the site's own, $TMPDIR's, macOS's /var/folders and mktemp's default /tmp/tmp.*). The binary
# files are searched too (the .wasm holds the build's strings, the .data the packed files).
tmp=${TMPDIR:-/tmp}; tmp=${tmp%/}
set -- -e /Users/ -e /var/folders/ -e /tmp/tmp. -e "$site"
[ -n "$HOME" ] && [ "$HOME" != / ] && set -- "$@" -e "$HOME/"
[ "$tmp" != /tmp ] && set -- "$@" -e "$tmp/"
if leaks=$(LC_ALL=C grep -r -l -a -F "$@" "$site"); then
  echo "web/deploy.sh: refusing: these files name a local path (home, /Users/ or a temporary folder):" >&2
  echo "$leaks" | sed "s#^$site/#  #" >&2
  exit 1
elif [ $? -ne 1 ]; then
  echo "web/deploy.sh: the path check could not search $site" >&2; exit 1
fi
echo "path check: no local path in the site"

rev=$(git -C "$here" rev-parse --short HEAD)
if [ -z "$push" ]; then
  echo "dry run: nothing pushed. With --push this would force-push to git@github.com:$repo.git, branch gh-pages:"
  (cd "$site" && find . -type f | sed 's#^\./##' | sort | while read -r f; do
     printf '  %10s  %s\n' "$(wc -c < "$f" | tr -d ' ')" "$f"; done)
  echo "  total $(du -sh "$site" | cut -f1), commit \"Site build $rev\""
  echo "and the site would be at $url (once Pages serves the gh-pages branch)"
  exit 0
fi
cd "$site" && git init -q -b gh-pages && git add -A && git commit -q -m "Site build $rev" && \
  git push -q -f "git@github.com:$repo.git" gh-pages
echo "deployed to $url"
