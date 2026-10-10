#!/bin/sh
# Builds both games for the web, packs the deployer's own game files and MT-32 ROMs, checks the
# page, and with --push force-pushes the site to SITE_REPO's gh-pages branch (docs/WEB.md). The
# game data goes there and nowhere else. Without --push it is a dry run: it builds, assembles and
# checks the site, says what it would push and where, and contacts nothing.
# usage: web/deploy.sh [--push [--replace] [--own-identity] [--https]] [--skip-replays] OWNER/NAME UW1_DIR UW2_DIR ROMS_DIR
#   --replace       with --push: replace a gh-pages branch the repository already has (without
#                   it, an existing gh-pages is refused: the push is forced and would destroy it)
#   --own-identity  with --push: the site's commit made as the deployer's own git user.name and
#                   user.email; without it the commit is made as "Underworld Exhumed site"
#                   <site@invalid>, so a public repository of the game's files names no one
#   --skip-replays  only make web, not make webcheck (every session replayed under Node.js,
#                   minutes): for a quick redeploy of builds already checked. The page checks
#                   (tools/webcheck.mjs) always run.
set -e
usage() { echo "usage: web/deploy.sh [--push [--replace] [--own-identity] [--https]] [--skip-replays] OWNER/NAME UW1_DIR UW2_DIR ROMS_DIR" >&2; exit 2; }
push=; replace=; replays=1; own=; https=
while [ $# -gt 0 ]; do
  case $1 in --push) push=1;; --replace) replace=1;; --own-identity) own=1;; --https) https=1;; --skip-replays) replays=;; -*) usage;; *) break;; esac; shift
done
[ $# -eq 4 ] || usage
[ -z "$replace" ] || [ -n "$push" ] || usage
[ -z "$own" ] || [ -n "$push" ] || usage
repo=${1%.git}; uw1=$2; uw2=$3; roms=$4
case $repo in */*/*|/*|*/|*[!A-Za-z0-9._/-]*) usage;; */*) ;; *) usage;; esac
for d in "$uw1" "$uw2" "$roms"; do [ -d "$d" ] || { echo "web/deploy.sh: no folder $d" >&2; exit 2; }; done
here=$(cd "$(dirname "$0")/.." && pwd)
owner=${repo%/*}; name=${repo#*/}
url="https://$owner.github.io/$name/"

# Never a project's own repository: the site holds the game data and the ROMs, and the push is
# forced. Refused: this repository's origin, Exhume's, and the project's other repositories by name.
lc() { printf '%s' "$1" | tr '[:upper:]' '[:lower:]'; }
for d in "$here" "$here/exhume"; do
  o=$(git -C "$d" remote get-url origin 2>/dev/null | sed -E 's#/$##; s#\.git$##; s#^.*[:/]([^/:]+/[^/:]+)$#\1#') || o=
  if [ -n "$o" ] && [ "$(lc "$o")" = "$(lc "$repo")" ]; then
    echo "web/deploy.sh: refusing $repo: it is the origin of $d, not a site repository" >&2; exit 1
  fi
done
case $(lc "$name") in
  underworld-exhumed|exhume|uw1decomp|uw2decomp|uw1-ci-assets|uw2-ci-assets|uwreverseengineering|underworldgodot|emulators|dos-mcp|uw2-personal-notes)
    echo "web/deploy.sh: refusing $repo: $name is one of the project's own repositories, not a site repository" >&2; exit 1;;
esac

if [ -n "$replays" ]; then
  make -C "$here/uw1" web webcheck && make -C "$here/uw2" web webcheck
else
  make -C "$here/uw1" web && make -C "$here/uw2" web
fi

site=$(mktemp -d); trap 'rm -rf "$site"' EXIT
# the site never inside a git work tree but its own (web/pack.sh refuses one too, the same way)
command -v git >/dev/null || { echo "web/deploy.sh: no git" >&2; exit 1; }
if err=$(LC_ALL=C git -C "$site" rev-parse --git-dir 2>&1); then
  echo "web/deploy.sh: refusing $site: it is inside a git repository" >&2; exit 1
fi
case $err in *"not a git repository"*) ;; *) echo "web/deploy.sh: cannot tell whether $site is in a git repository: $err" >&2; exit 1;; esac
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

rev=$(git -C "$here" describe --always --dirty)
# --https: through https and the GitHub CLI's login, where ssh to GitHub is not set up
if [ -n "$https" ]; then target="https://github.com/$repo.git"; else target="git@github.com:$repo.git"; fi
# the site's commit: a neutral author and committer unless --own-identity
# (set as both author and committer, over any GIT_AUTHOR_* or GIT_COMMITTER_* already set)
if [ -n "$own" ]; then who="your own git identity ($(git config user.name) <$(git config user.email)>)"
else
  who='"Underworld Exhumed site" <site@invalid>'
  GIT_AUTHOR_NAME="Underworld Exhumed site"; GIT_AUTHOR_EMAIL=site@invalid
  GIT_COMMITTER_NAME=$GIT_AUTHOR_NAME; GIT_COMMITTER_EMAIL=$GIT_AUTHOR_EMAIL
  export GIT_AUTHOR_NAME GIT_AUTHOR_EMAIL GIT_COMMITTER_NAME GIT_COMMITTER_EMAIL
fi
if [ -z "$push" ]; then
  echo "dry run: nothing pushed. With --push this would force-push to $target, branch gh-pages:"
  (cd "$site" && find . -type f | sed 's#^\./##' | sort | while read -r f; do
     printf '  %10s  %s\n' "$(wc -c < "$f" | tr -d ' ')" "$f"; done)
  echo "  total $(du -sh "$site" | cut -f1), commit \"Site build $rev\" as $who"
  echo "and the site would be at $url (once Pages serves the gh-pages branch)"
  exit 0
fi
# The push is forced, so a gh-pages the repository already has is replaced only with --replace
# (ls-remote exits 2 when the branch is not there; any other failure stops the deploy).
if LC_ALL=C git ls-remote --exit-code --heads "$target" gh-pages >/dev/null; then
  [ -n "$replace" ] || { echo "web/deploy.sh: refusing: $repo already has a gh-pages branch (--replace replaces it)" >&2; exit 1; }
elif [ $? -ne 2 ]; then
  echo "web/deploy.sh: cannot read $target's branches" >&2; exit 1
fi
cd "$site" && git init -q -b gh-pages && git add -A && git commit -q -m "Site build $rev" && \
  git push -q -f "$target" gh-pages
echo "deployed to $url"
