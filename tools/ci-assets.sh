#!/bin/sh
# CI only (docs/BUILDING.md, "Continuous integration"): decrypt the asset bundle, the owner's
# own UW2 game files and Borland disk images from the private repository uw2-ci-assets, into
# DEST, check every file against the bundle's SHA256SUMS, and point the tools at it.
#
#   UW2_ASSETS_AGE_KEY=... tools/ci-assets.sh BUNDLE.tar.gz.age DEST
#
# The age secret key comes from the environment and goes to age on its standard input, never
# to a file. Nothing in the bundle is listed or printed: the checks say only how many files
# passed. On success it prints, and appends to $GITHUB_ENV when that is set, the variables the
# tools read: UW2_EXE and UW2_DIR (the game), TC_DISKS and TASM_DISKS (for make setup).
# DEST is removed by the workflow's clean-up step; nothing under it may be cached or uploaded.
set -eu
bundle=${1:?usage: ci-assets.sh BUNDLE.tar.gz.age DEST}; dest=${2:?usage: ci-assets.sh BUNDLE.tar.gz.age DEST}
[ -n "${UW2_ASSETS_AGE_KEY:-}" ] || { echo "ci-assets: UW2_ASSETS_AGE_KEY is not set"; exit 1; }
umask 077
rm -rf "$dest"; mkdir -p "$dest"; dest=$(cd "$dest" && pwd)
# GNU tar warns about the macOS extended attributes a bundle made on a Mac carries
quiet=; tar --version 2>/dev/null | grep -q GNU && quiet=--warning=no-unknown-keyword
printf '%s\n' "$UW2_ASSETS_AGE_KEY" | age -d -i - "$bundle" | tar $quiet -xzf - -C "$dest"
for f in SHA256SUMS game/UW2/UW2.EXE tc/Disk01.img tc/Disk02.img tc/Disk03.img tc/Disk04.img tasm/Disk01.img; do
  [ -f "$dest/$f" ] || { echo "ci-assets: the bundle has no $f"; exit 1; }
done
n=$(grep -c . "$dest/SHA256SUMS")
if command -v sha256sum >/dev/null 2>&1; then sum="sha256sum"; else sum="shasum -a 256"; fi
( cd "$dest" && $sum -c --quiet SHA256SUMS >/dev/null 2>&1 ) || { echo "ci-assets: SHA256SUMS does not verify"; exit 1; }
echo "ci-assets: $n files verified against SHA256SUMS"
vars="UW2_EXE=$dest/game/UW2/UW2.EXE
UW2_DIR=$dest/game/UW2
TC_DISKS=$dest/tc
TASM_DISKS=$dest/tasm"
echo "$vars"
[ -z "${GITHUB_ENV:-}" ] || echo "$vars" >> "$GITHUB_ENV"
