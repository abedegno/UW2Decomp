#!/bin/sh
# Ask the build queue (tools/buildd.py, running outside the sandbox) to run match.py or
# verify.py, wait, and print the result.   usage: tools/remote.sh match|verify src/FILE.C [--dis NAME]
root=$(cd "$(dirname "$0")/.." && pwd); id=$$-$(date +%s)
mkdir -p "$root/build/queue"; echo "$*" > "$root/build/queue/$id.req.tmp"
mv "$root/build/queue/$id.req.tmp" "$root/build/queue/$id.req"
while [ ! -f "$root/build/queue/$id.out" ]; do sleep 2; done
cat "$root/build/queue/$id.out"; rm -f "$root/build/queue/$id.out"
