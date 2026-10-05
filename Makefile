# Underworld Exhumed: both games. Each game folder has its own Makefile (make -C uw2 help);
# this one runs a target in both, or in one with GAME=uw1 or GAME=uw2.
#
#   make setup         Exhume (the submodule) and its Python tools, then each game's setup
#   make check         each game's gate: every source matches, the exact link is the EXE
#   make test          the gate, the port, quick fuzzing and every session against DOS
#   make port          each game's native port
#   make repocheck     the whole repository: no game data or Borland software, every
#                      script compiles, every Markdown link resolves
GAMES := $(if $(GAME),$(GAME),uw1 uw2)
TARGETS := check test test-full port port-debug port-release package verify vectors \
           setup-port setup-libs setup-sound

.PHONY: $(TARGETS) setup setup-exhume repocheck help
$(TARGETS):
	@set -e; for g in $(GAMES); do echo "==== $$g: make $@"; $(MAKE) -C $$g $@; done

# the submodule, and the Python environment its tools run in (iced-x86, unicorn)
setup-exhume:
	@[ -f exhume/tools/gate.py ] || git submodule update --init exhume
	@[ -x exhume/.venv/bin/python ] || { python3 -m venv exhume/.venv && \
	  exhume/.venv/bin/pip install -q -r exhume/tools/requirements.txt; }
	@echo "Exhume: exhume/ at $$(git -C exhume rev-parse --short HEAD)"

# a game's own setup where it has one (uw2: the Borland toolchain, DOS, Node), else its port's
setup: setup-exhume
	@set -e; for g in $(GAMES); do t=$$(grep -q '^setup:' $$g/Makefile && echo setup || echo setup-port); \
	  echo "==== $$g: make $$t"; $(MAKE) -C $$g $$t; done

# also: no link into the old repositories' files (they moved into uw1/ and uw2/)
repocheck:
	@python3 exhume/tools/repocheck.py --root .
	@n=$$(git grep -I -c -E 'github\.com/abedegno/UW[12]Decomp/(blob|tree)/' -- . ':!exhume' | awk -F: '{s+=$$2} END {print s+0}'); \
	  if [ "$$n" -gt 0 ]; then echo "repocheck: $$n links into UW1Decomp or UW2Decomp's files (tools/relink.py)"; exit 1; fi

help:
	@echo "make [GAME=uw1|uw2] setup | $(TARGETS) | repocheck"
