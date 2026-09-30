#!/bin/sh
# Extract Turbo C++ 1.01 from Borland's four 720K disk images into ./TC.
# Borland released it free of charge through its "Antique Software" programme; it is not
# redistributed here. Needs mtools (mcopy) and 7z:  brew install mtools p7zip
# usage: tools/setup-tc.sh DIR_WITH_Disk01.img..Disk04.img
set -e
src=${1:?usage: setup-tc.sh DIR_WITH_DISK_IMAGES}
root=$(cd "$(dirname "$0")/.." && pwd)
tmp=$(mktemp -d)
for d in "$src"/Disk0[1-4].img; do mcopy -n -i "$d" '::*.ZIP' "$tmp"/; done
mkdir -p "$root/TC"
for z in TCC BIN1 BIN2 INCLUDE STARTUP LLIB MLIB SLIB CLIB HLIB; do
  7z x -y -bso0 -o"$root/TC" "$tmp/$z.ZIP"
done
rm -rf "$tmp"
sum=$(md5 -q "$root/TC/TCC.EXE" 2>/dev/null || md5sum "$root/TC/TCC.EXE" | cut -d' ' -f1)
[ "$sum" = db4c0704f7091b8d875be038bee2bf1e ] && echo "TC ready: TCC.EXE is the expected 1.01 build" \
  || { echo "TCC.EXE checksum $sum is not the expected build"; exit 1; }
