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
#   make setup-exhume  find Exhume ($EXHUME, .exhume, ~/Exhume), or clone it into .exhume at the
#                      commit tools/exhume-ref names
#   make setup-port    what the port needs to build, per OS, and nothing else: no DOS
#                      toolchain, DOS or game data (Exhume's tools/setup-port.sh)
#   make setup-libs    build SDL3 and libmt32emu from source into tools/libs, where no package
#                      has them (Linux; tools/setup-libs.sh)
#   make setup-sound   fetch the OPL emulator, Nuked OPL3, into tools/nuked-opl3
#   make port-check    compile the game's C for the host, compile only
#   make port          build and link the port; make port-debug the UBSan build
#   make port-release  the build the players' packages are made from (PORT_ARCHS="--arch arm64
#                      --arch x86_64" for a universal macOS build)
#   make package       package the release build for this OS into build/dist (tools/package.py;
#                      PACKAGE_ARGS="--strict --appimage" as CI does)
#   make icons         the icon files from their SVG sources ([package]; tools/icons.py)
#   make test          the gate, the port, the quick fuzzing, every session against its golden
#   make test-full     the long tier: goldens again from DOS, UBSan, drivers, deep fuzzing, coverage
#   make verify        the sessions against their goldens, in the port
#   make golden        every session's golden made again from DOS (twice, checked identical)
#   make golden-check  every session replayed in DOS twice by the replay DOS build and compared
#                      with its committed golden, writing nothing (needs no port)
#   make fuzz          the routine fuzzing (FUZZ=--deep for the long run)
#   make coverage      the port's coverage over the sessions and the fuzzing

EXHUME ?= $(if $(wildcard .exhume/tools/gate.py),.exhume,$(HOME)/Exhume)
PY ?= $(if $(wildcard $(EXHUME)/.venv/bin/python),$(EXHUME)/.venv/bin/python,python3)
GATE := $(PY) $(EXHUME)/tools/gate.py --config exhume.toml

HOOK ?= make check
PORT := $(PY) $(EXHUME)/tools

.PHONY: game exact check check-all fast boot hooks repocheck help \
        setup-exhume setup-port setup-libs setup-sound port-check port port-debug port-release package icons \
        test test-full verify golden golden-check fuzz coverage
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

setup-exhume:
	@if [ -f "$(EXHUME)/tools/gate.py" ]; then echo "Exhume: $(EXHUME)"; \
	else pin=$$(cut -d' ' -f1 tools/exhume-ref) && echo "Exhume: cloning it into .exhume at $$pin" && \
	  git clone -q https://github.com/abedegno/Exhume.git .exhume && git -C .exhume checkout -q "$$pin"; fi

setup-port: setup-exhume
	@sh $(if $(wildcard .exhume/tools/gate.py),.exhume,$(EXHUME))/tools/setup-port.sh tools/libs tools/nuked-opl3

setup-libs:
	@sh $(EXHUME)/tools/setup-libs.sh tools/libs

setup-sound:
	@sh $(EXHUME)/tools/setup-sound.sh tools/nuked-opl3

port-check:
	@$(PORT)/portcheck.py --config exhume.toml

port:
	@$(PORT)/portbuild.py --config exhume.toml

port-debug:
	@$(PORT)/portbuild.py --config exhume.toml --debug

port-release:
	@$(PORT)/portbuild.py --config exhume.toml --release $(PORT_ARCHS)

package:
	@$(PORT)/package.py --config exhume.toml $(PACKAGE_ARGS)

icons:
	@$(PORT)/icons.py --config exhume.toml

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
