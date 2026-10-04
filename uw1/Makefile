# A decompilation project's build driver, over Exhume's tools/gate.py. Copy it to the
# project's root beside its exhume.toml; the gate's logic and the project's facts (link
# commands, where they write the EXE, boot steps) are in exhume.toml's [gate] section.
#
#   make / make game   modding build, prints the EXE's path
#   make exact         exact link, which must be byte-identical to your original EXE
#   make check         the full gate; recompiles only sources changed since they last passed
#   make check-all     the full gate, recompiling every source
#   make fast          compile, match and verify the changed sources only (not a proof)
#   make boot          boot the modding build and screenshot it
#   make hooks         install the git pre-push hook that runs make check (make test once
#                      there is a port: HOOK="make test")
#
# A native port ([port] and [replay] in exhume.toml, docs/port.md):
#   make port-check    compile the game's C for the host, compile only
#   make port          build and link the port; make port-debug the UBSan build
#   make test          the gate, the port, the quick fuzzing, every session against its golden
#   make test-full     the long tier: goldens again from DOS, UBSan, drivers, deep fuzzing, coverage
#   make verify        the sessions against their goldens, in the port
#   make golden        every session's golden made again from DOS (twice, checked identical)
#   make golden-check  every session replayed in DOS twice by the replay DOS build and compared
#                      with its committed golden, writing nothing (needs no port)
#   make fuzz          the routine fuzzing (FUZZ=--deep for the long run)
#   make coverage      the port's coverage over the sessions and the fuzzing

EXHUME ?= $(HOME)/Exhume
PY := $(if $(wildcard $(EXHUME)/.venv/bin/python),$(EXHUME)/.venv/bin/python,python3)
GATE := $(PY) $(EXHUME)/tools/gate.py --config exhume.toml

HOOK ?= make check
PORT := $(PY) $(EXHUME)/tools

.PHONY: game exact check check-all fast boot hooks repocheck help \
        port-check port port-debug test test-full verify golden golden-check fuzz coverage
.DEFAULT_GOAL := game

game:
	@$(GATE) game

exact:
	@$(GATE) exact

check:
	@$(GATE) check

check-all:
	@$(GATE) check --all

fast:
	@$(GATE) check --fast

boot:
	@$(GATE) boot

hooks:
	@sh $(EXHUME)/tools/install-hooks.sh . "$(HOOK)"

port-check:
	@$(PORT)/portcheck.py --config exhume.toml

port:
	@$(PORT)/portbuild.py --config exhume.toml

port-debug:
	@$(PORT)/portbuild.py --config exhume.toml --debug

test:
	@$(PORT)/test.py --config exhume.toml fast

test-full:
	@$(PORT)/test.py --config exhume.toml full

verify:
	@$(PORT)/replay.py --config exhume.toml verify all

golden:
	@$(PORT)/replay.py --config exhume.toml golden all

golden-check:
	@$(PORT)/replay.py --config exhume.toml golden all --check

fuzz:
	@$(PORT)/fuzzasm.py --config exhume.toml $(FUZZ)

coverage:
	@$(PORT)/coverage.py --config exhume.toml

repocheck:
	@$(PY) $(EXHUME)/tools/repocheck.py

help:
	@sed -n '1,/^$$/p' Makefile
