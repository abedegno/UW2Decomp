#!/bin/sh
# Build emu2 (github.com/dmsc/emu2), the fastest DOS for the toolchain runs, into tools/emu2
# (ignored by git), at a pinned commit with tools/emu2-date.patch applied: emu2 cannot set
# the DOS date, which the link needs (TLINK records it), and the patch lets it. Needs git,
# make and a C compiler. Safe to run again.   usage: tools/setup-emu2.sh   (make setup-emu2)
set -e
root=$(cd "$(dirname "$0")/.." && pwd); dir="$root/tools/emu2"; patch="$root/tools/emu2-date.patch"
commit=9d8698d06e67359bc8b230c2d19d5b1503e60819
[ -d "$dir/.git" ] || git clone -q https://github.com/dmsc/emu2 "$dir"
cd "$dir"
if [ "$(git rev-parse HEAD)" != "$commit" ] || ! git diff | cmp -s - "$patch"; then
  git cat-file -e "$commit^{commit}" 2>/dev/null || git fetch -q origin
  git checkout -q -f "$commit"; git apply "$patch"
fi
make -s >/dev/null 2>&1 || { echo "emu2: the build failed (run make in tools/emu2 to see why)"; exit 1; }
echo "emu2: tools/emu2/emu2 (dmsc/emu2 at $(git rev-parse --short HEAD), with tools/emu2-date.patch)"
