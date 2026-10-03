#!/bin/sh
# Extract Turbo Assembler 2.0 from Borland's two 360K disk images into ./TASM.
# Not redistributed here. Needs mtools and 7z.   usage: tools/setup-tasm.sh DIR_WITH_Disk01.img
set -e
src=${1:?usage: setup-tasm.sh DIR_WITH_DISK_IMAGES}
root=$(cd "$(dirname "$0")/.." && pwd); tmp=$(mktemp -d)
mcopy -n -i "$src/Disk01.img" ::TASM.ZIP "$tmp"/
mkdir -p "$root/TASM"; 7z x -y -bso0 -o"$root/TASM" "$tmp/TASM.ZIP"; rm -rf "$tmp"
sum=$(md5 -q "$root/TASM/TASM.EXE" 2>/dev/null) || sum=$(md5sum "$root/TASM/TASM.EXE" | cut -d' ' -f1)
[ "$sum" = b68a63d6a94672910d4149fad4c18f00 ] || { echo "TASM.EXE checksum $sum is not the expected build"; exit 1; }
echo "TASM ready: TASM.EXE is the expected 2.0 build"
