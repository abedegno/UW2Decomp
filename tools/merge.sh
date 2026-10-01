#!/bin/sh
# Accept a source file: it must match whole and verify, then its externs go into
# symbols.tsv, its segment into matched.txt, and map/files.tsv is refreshed.
# usage: tools/merge.sh src/FILE.C      exits non-zero, changing nothing, on any failure
set -e
root=$(cd "$(dirname "$0")/.." && pwd); cd "$root"
src=$1
seg=$(sed -n 's#.*/\* target: \([A-Za-z0-9_]*\) \*/.*#\1#p' "$src" | head -1)
py=.venv/bin/python
$py tools/match.py "$src" > /tmp/merge-match.txt || { cat /tmp/merge-match.txt; exit 1; }
tail -1 /tmp/merge-match.txt
grep -q "WHOLE SEGMENT MATCHES" /tmp/merge-match.txt || { echo "not a whole-segment match"; exit 1; }
$py tools/verify.py "$src" > /dev/null || { $py tools/verify.py "$src" | grep -E "PROBLEM|^--"; exit 1; }
$py tools/verify.py "$src" --update | tail -1
grep -qx "$seg" matched.txt || echo "$seg" >> matched.txt
python3 tools/files.py | grep "matched bytes"
