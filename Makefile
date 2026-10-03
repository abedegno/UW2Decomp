# UW2Decomp build driver. The work is done in headless DOS by the tools in tools/; this file
# only names the entry points. tools/uw2.py holds the gate's logic.
#
#   make / make game   modding build (tools/link.py --mod), prints the EXE's path
#   make exact         exact link, compared with your UW2.EXE (tools/exediff.py)
#   make check         the full gate; recompiles only sources changed since they last passed
#   make check-all     the full gate, recompiling every source
#   make boot          boot the modding build to the intro and screenshot it
#   make setup         toolchain, Python venv, npm packages and emu2 (idempotent)
#   make setup-emu2    only build emu2, the fast DOS for the toolchain (tools/setup-emu2.sh)
#   make setup-sound   fetch the port's OPL emulator, Nuked OPL3 (tools/setup-sound.sh)
#   make hooks         install the git pre-push hook that runs make check
#   make port-check    compile the C for the host, compile only (docs/PORT.md, Milestone 1)
#   make port          compile the C for the host and link it with the stubs: build/port/uw2port
#   make port-debug    the same with -g and UBSan's -fsanitize=null: build/port-debug/uw2port
#
# make setup needs the Borland disk images the first time:
#   make setup TC_DISKS="/path/to/Turbo C++ 1.01" TASM_DISKS="/path/to/Turbo Assembler 2.0"

PY := $(if $(wildcard .venv/bin/python),.venv/bin/python,python3)
TC_DISKS ?=
TASM_DISKS ?=

.PHONY: game exact check check-all boot setup setup-emu2 setup-sound hooks port-check port port-debug help
.DEFAULT_GOAL := game

game:
	@$(PY) tools/uw2.py game

exact:
	@$(PY) tools/uw2.py exact

check:
	@$(PY) tools/uw2.py check

check-all:
	@$(PY) tools/uw2.py check --all

boot:
	@$(PY) tools/uw2.py boot

setup:
	@if [ -f TC/TCC.EXE ] && [ "$$(md5 -q TC/TCC.EXE 2>/dev/null || md5sum TC/TCC.EXE | cut -d' ' -f1)" = db4c0704f7091b8d875be038bee2bf1e ]; then \
	  echo "TC: Turbo C++ 1.01 already in TC/"; \
	elif [ -n "$(TC_DISKS)" ]; then sh tools/setup-tc.sh "$(TC_DISKS)"; \
	else echo "TC/ is missing: make setup TC_DISKS=DIR (the directory holding Disk01.img..Disk04.img)"; exit 1; fi
	@if [ -f TASM/TASM.EXE ] && [ "$$(md5 -q TASM/TASM.EXE 2>/dev/null || md5sum TASM/TASM.EXE | cut -d' ' -f1)" = b68a63d6a94672910d4149fad4c18f00 ]; then \
	  echo "TASM: Turbo Assembler 2.0 already in TASM/"; \
	elif [ -n "$(TASM_DISKS)" ]; then sh tools/setup-tasm.sh "$(TASM_DISKS)"; \
	else echo "TASM/ is missing: make setup TASM_DISKS=DIR (the directory holding Disk01.img)"; exit 1; fi
	@[ -x .venv/bin/python ] || python3 -m venv .venv
	@.venv/bin/python -c "import iced_x86" 2>/dev/null || .venv/bin/pip install -q iced-x86
	@echo "Python: .venv with iced-x86"
	@[ -d node_modules/dos-mcp ] || npm install
	@echo "Node: dos-mcp installed"
	@sh tools/setup-emu2.sh || echo "emu2: not built; the toolchain runs in DOSBox-X if installed (brew install dosbox-x), else js-dos"
	@echo "DOS for the toolchain: $$(node tools/dosbackend.mjs) (docs/BUILDING.md, Choosing the DOS)"
	@sh tools/setup-sound.sh || echo "nuked-opl3: not fetched; the port builds without FM music"
	@[ -f "$${UW2_EXE:-$$HOME/UWGOG/UW2/UW2.EXE}" ] && echo "UW2.EXE: $${UW2_EXE:-$$HOME/UWGOG/UW2/UW2.EXE}" \
	  || { echo "UW2.EXE not found: put the game at ~/UWGOG/UW2 or set UW2_EXE"; exit 1; }

setup-emu2:
	@sh tools/setup-emu2.sh

setup-sound:
	@sh tools/setup-sound.sh

hooks:
	@sh tools/install-hooks.sh

port-check:
	@$(PY) tools/portcheck.py

port:
	@$(PY) tools/portbuild.py

port-debug:
	@$(PY) tools/portbuild.py --debug

help:
	@sed -n '1,/^$$/p' Makefile
