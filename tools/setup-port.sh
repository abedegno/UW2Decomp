#!/bin/sh
# Install what the native port needs to build (make setup-port), and nothing else: no Borland
# toolchain, no DOS emulator and no game data, which only the DOS build and the tests need
# (docs/BUILDING.md, "Building the port"). Safe to run again.
#
#   macOS    Homebrew's sdl3, mt32emu and pkgconf (needs Homebrew and the Xcode command line tools)
#   Linux    with apt (Debian, Ubuntu): prints the apt-get command for the compiler and the
#            headers when any is missing; then builds SDL3 and libmt32emu into tools/libs
#            (tools/setup-libs.sh), since few releases package SDL3 and none libmt32emu
#   Windows  MSYS2's CLANG64 shell: its packages with pacman, then libmt32emu into tools/libs
# then, everywhere, Nuked OPL3 into tools/nuked-opl3 (tools/setup-sound.sh).
# usage: tools/setup-port.sh   then: make port (make PY=python port in MSYS2)
set -e
root=$(cd "$(dirname "$0")/.." && pwd)
have() { command -v "$1" >/dev/null 2>&1; }

case "$(uname -s)" in
Darwin)
  have clang || { echo "Install the Xcode command line tools first: xcode-select --install"; exit 1; }
  have brew || { echo "Install Homebrew first (https://brew.sh), then run make setup-port again."; exit 1; }
  HOMEBREW_NO_AUTO_UPDATE=1 brew install sdl3 mt32emu pkgconf
  ;;
Linux)
  pkgs="clang pkg-config cmake ninja-build git curl python3 libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxss-dev libxtst-dev libxkbcommon-dev libwayland-dev wayland-protocols libdecor-0-dev libegl-dev libgl-dev libasound2-dev libpulse-dev libpipewire-0.3-dev libdbus-1-dev libudev-dev"
  if have dpkg-query; then
    missing=
    for p in $pkgs; do dpkg-query -W -f='${Status}' "$p" 2>/dev/null | grep -q 'ok installed' || missing="$missing $p"; done
    if [ -n "$missing" ]; then
      echo "Install the compiler and the headers SDL3 needs for a window and sound, then run make setup-port again:"
      echo "  sudo apt-get install$missing"
      exit 1
    fi
  else
    for t in clang pkg-config cmake git curl python3; do have "$t" || { echo "Install $t (and the X11 or Wayland, ALSA and PulseAudio development headers), then run make setup-port again."; exit 1; }; done
  fi
  sh "$root/tools/setup-libs.sh"
  ;;
MINGW*|MSYS*)
  if [ "$MSYSTEM" != CLANG64 ]; then
    echo "Run this from MSYS2's CLANG64 shell (the 'MSYS2 CLANG64' item in the Start menu)."; exit 1
  fi
  pacman -S --needed --noconfirm make git curl mingw-w64-clang-x86_64-clang mingw-w64-clang-x86_64-pkgconf \
    mingw-w64-clang-x86_64-sdl3 mingw-w64-clang-x86_64-python mingw-w64-clang-x86_64-cmake mingw-w64-clang-x86_64-ninja
  UW2_LIBS=mt32emu sh "$root/tools/setup-libs.sh"
  ;;
*)
  echo "$(uname -s): no recipe; install clang, pkg-config and SDL3 3.2 or later, then run make port."; exit 1
  ;;
esac
sh "$root/tools/setup-sound.sh"
echo "Ready: make port builds build/port/uw2port${MSYSTEM:+ (make PY=python port)}"
