#!/bin/sh
# Find Exhume, whose runtime the port, the gate and the replay build compile where it is
# (docs/BUILDING.md, "Exhume"), or fetch it: $EXHUME, else .exhume in this repository, else
# ~/Exhume (as tools/exhume.py looks); when none has the runtime, clone
# https://github.com/abedegno/Exhume into .exhume (ignored by git) at the commit tools/exhume-ref
# names. A checkout that does not contain that commit gets a warning, never a change. Safe to
# run again.
set -e
root=$(cd "$(dirname "$0")/.." && pwd)
pin=$(cut -d' ' -f1 "$root/tools/exhume-ref")
if [ -n "$EXHUME" ]; then dir=$EXHUME
elif [ -d "$root/.exhume/runtime" ]; then dir=$root/.exhume
elif [ -d "$HOME/Exhume/runtime" ]; then dir=$HOME/Exhume
else
  echo "Exhume: cloning https://github.com/abedegno/Exhume into .exhume at $pin"
  git clone -q https://github.com/abedegno/Exhume.git "$root/.exhume"
  git -C "$root/.exhume" checkout -q "$pin"
  dir=$root/.exhume
fi
if [ ! -f "$dir/runtime/include/portable.h" ]; then
  echo "Exhume: no runtime at $dir (set EXHUME to an Exhume checkout)"; exit 1
fi
if [ -d "$dir/.git" ] && ! git -C "$dir" merge-base --is-ancestor "$pin" HEAD 2>/dev/null; then
  echo "Exhume: warning: $dir does not contain $pin, the commit tools/exhume-ref names (git -C $dir pull)"
fi
echo "Exhume: $dir"
