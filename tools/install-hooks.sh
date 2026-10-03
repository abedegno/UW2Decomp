#!/bin/sh
# Install the pre-push hook: git push runs `make test` first (the gate, the port build, the routine
# fuzzing's quick run and every replay session against its golden; docs/BUILDING.md, "Testing")
# and stops when it fails. It needs the toolchain and your UW2.EXE, which hosted CI cannot have,
# so it runs here. Run `make hooks` again to update a hook an older version installed.
# Bypass for one push (a docs-only change, say):  git push --no-verify   or   SKIP_CHECK=1 git push
set -e
root=$(cd "$(dirname "$0")/.." && pwd)
hook=$(git -C "$root" rev-parse --git-path hooks)/pre-push
case "$hook" in /*) ;; *) hook="$root/$hook" ;; esac
mkdir -p "$(dirname "$hook")"
if [ -f "$hook" ] && ! grep -q 'uw2decomp pre-push' "$hook"; then
  echo "$hook exists and is not ours; leaving it alone"; exit 1
fi
cat > "$hook" <<'EOF'
#!/bin/sh
# uw2decomp pre-push: run make test (the gate, the port, the fuzzing, the replays against their
# goldens) before anything leaves this machine.
# Bypass: git push --no-verify, or SKIP_CHECK=1 git push
[ -n "$SKIP_CHECK" ] && { echo "pre-push: SKIP_CHECK set, not running make test"; exit 0; }
cd "$(git rev-parse --show-toplevel)" || exit 1
echo "pre-push: make test (bypass with git push --no-verify)"
make test || { echo "pre-push: make test failed, push stopped"; exit 1; }
EOF
chmod +x "$hook"
echo "installed $hook"
