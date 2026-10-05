#!/bin/sh
# CI only (docs/BUILDING.md, "Continuous integration"): Exhume's tools/ci-assets.sh on UW1's
# private bundle, with the variables UW1's tools read pointed into it: UW1_DATA and UW1_EXE (the
# game, the bundle's game/UW1), TC_DISKS and TASM_DISKS (the Borland disk images). The age key
# comes from ASSETS_AGE_KEY in the environment (the uw1-assets action). Nothing is printed but
# the variables.
#   usage: sh tools/ci-assets.sh BUNDLE.tar.gz.age DEST
set -eu
here=$(cd "$(dirname "$0")/.." && pwd)
exhume=${EXHUME:-$here/.exhume}
exec sh "$exhume/tools/ci-assets.sh" "$1" "$2" --require game/UW1/UW.EXE \
  --export UW1_DATA=game/UW1 --export UW1_EXE=game/UW1/UW.EXE --export TC_DISKS=tc --export TASM_DISKS=tasm
