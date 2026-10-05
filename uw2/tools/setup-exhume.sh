#!/bin/sh
# Finds Exhume, whose tools and runtime the port, the gate and the replay build use
# (docs/BUILDING.md, "Exhume"): $EXHUME; else the repository's submodule ../exhume, initialised
# here when it is empty; else .exhume; else ~/Exhume. In underworld-exhumed the submodule is the
# one used, at the commit the repository records.
root=$(cd "$(dirname "$0")/.." && pwd)
top=$(cd "$root/.." && pwd)
if [ -n "$EXHUME" ]; then dir=$EXHUME
elif [ -f "$top/.gitmodules" ] && grep -q 'path = exhume' "$top/.gitmodules"; then
  [ -f "$top/exhume/tools/gate.py" ] || git -C "$top" submodule update --init exhume
  dir=$top/exhume
elif [ -d "$root/.exhume/runtime" ]; then dir=$root/.exhume
elif [ -d "$HOME/Exhume/runtime" ]; then dir=$HOME/Exhume
else echo "Exhume: none found (clone the repository with --recursive, or set EXHUME)"; exit 1; fi
[ -d "$dir/runtime" ] || { echo "Exhume: no runtime at $dir (set EXHUME to an Exhume checkout)"; exit 1; }
echo "Exhume: $dir"
