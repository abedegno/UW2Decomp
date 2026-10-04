#!/bin/sh
# Stage 1: the structs (Player, Arc, CutsState). Run on a tree at HEAD.
set -e
H=~/UW1Decomp/build/headers-scratch; PY=~/Exhume/.venv/bin/python3
cd ~/UW1Decomp/src
for f in $(grep -l -E 'struct (Player1\w+|UW1Player\w+)' $(find . -name '*.C')); do perl -pi -e 's/struct (Player1\w+|UW1Player\w+)\b/struct Player1/g' $f; done
perl -0pi -e 's/HOST_LAYOUT_END\n\n(\/\* UW1: the player record differs)/\n$1/' game/CHARGEN.C
cd ~/UW1Decomp
$PY $H/sr.py --config exhume.toml convert docs/readability/headers/structs/player.py --dir src --write > $H/convert.log
cd src
perl -0pi -e 's/(    int16 spacing;                      \/\* 0x10 \*\/\n\};\n)/$1HOST_LAYOUT_END\n/' game/CHARGEN.C
for f in $(grep -rl 'PLAYER1\|((struct Player \*)player)' --include='*.C' .); do perl -0pi -e 's/\n?#define PLAYER1 \(\(struct Player \*\)player\)\n/\n/; s/\(\(struct Player \*\)player\)/player/g; s/\bPLAYER1\b/player/g' $f; done
for f in $(find . -name '*.C'); do perl -0pi -e 's/\n\n\n+/\n\n/g' $f; done
$PY $H/structs2.py
