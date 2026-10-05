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
# the overlay manager, which the link takes from the library (tools/link.py)
7z x -y -bso0 -o"$root/TC" "$tmp/XLIB.ZIP" OVERLAY.LIB
rm -rf "$tmp"
md5() { command md5 -q "$1" 2>/dev/null || md5sum "$1" | cut -d' ' -f1; }
sum=$(md5 "$root/TC/TCC.EXE")
[ "$sum" = db4c0704f7091b8d875be038bee2bf1e ] || { echo "TCC.EXE checksum $sum is not the expected build"; exit 1; }
sum=$(md5 "$root/TC/OVERLAY.LIB")
[ "$sum" = 5ae838f3f87ccd568886fcf5dfb2e327 ] || { echo "OVERLAY.LIB checksum $sum is not the expected build"; exit 1; }
echo "TC ready: TCC.EXE and OVERLAY.LIB are the expected 1.01 build"
