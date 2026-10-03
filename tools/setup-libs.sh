#!/bin/sh
# Build the port's libraries from source where no package has them (Linux, and CI): SDL3 and
# libmt32emu (munt), at pinned tags, installed into PREFIX (default tools/libs, ignored by git).
# macOS gets both from Homebrew instead (brew install sdl3 mt32emu; docs/BUILDING.md).
# Needs git, cmake, ninja (or make) and a C and C++ compiler; on Ubuntu the SDL3 build also
# wants the X11/Wayland/ALSA/PulseAudio development headers for a window and sound (none is
# needed for --hidden replays, which use SDL's offscreen video driver and no audio device;
# without X11 or Wayland headers SDL3 is built without windows).
# Safe to run again: it skips a library whose stamp file names the same tag and architectures.
#   usage: tools/setup-libs.sh [PREFIX]
# make port finds tools/libs by itself; for another PREFIX, export PKG_CONFIG_PATH=PREFIX/lib/pkgconfig.
# UW2_LIBS chooses the libraries (default "sdl3 mt32emu"; "mt32emu" where a package has SDL3,
# as MSYS2 does). UW2_MACOS_ARCHS="arm64;x86_64" builds universal macOS libraries for the
# release (.github/workflows/release.yml), for macOS 11 on unless MACOSX_DEPLOYMENT_TARGET says.
set -e
root=$(cd "$(dirname "$0")/.." && pwd)
prefix=${1:-$root/tools/libs}
mkdir -p "$prefix"; prefix=$(cd "$prefix" && pwd)
sdl_tag=release-3.4.16
mt_tag=libmt32emu_2_8_3
libs=${UW2_LIBS:-sdl3 mt32emu}
archs=${UW2_MACOS_ARCHS:-}
gen=; command -v ninja >/dev/null 2>&1 && gen="-G Ninja"
osx="${archs:+-DCMAKE_OSX_ARCHITECTURES=$archs}"
if [ -n "$archs" ]; then export MACOSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET:-11.0}"; fi
src=$(mktemp -d); trap 'rm -rf "$src"' EXIT

build() {   # name tag repo subdir cmake-args...
  name=$1; tag=$2; repo=$3; sub=$4; shift 4
  case " $libs " in *" $name "*) ;; *) return ;; esac
  stamp="$tag${archs:+ $archs}"
  if [ "$(cat "$prefix/.$name" 2>/dev/null)" = "$stamp" ]; then echo "$name: $tag already in $prefix"; return; fi
  git -c advice.detachedHead=false clone -q --depth 1 --branch "$tag" "$repo" "$src/$name"
  # shellcheck disable=SC2086
  cmake -S "$src/$name/$sub" -B "$src/$name/build" $gen -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$prefix" -DCMAKE_INSTALL_LIBDIR=lib $osx "$@" >/dev/null
  cmake --build "$src/$name/build" --parallel >/dev/null
  cmake --install "$src/$name/build" >/dev/null
  echo "$stamp" > "$prefix/.$name"
  echo "$name: $tag installed in $prefix"
}

# With neither X11 nor Wayland headers (a CI runner), SDL3 is built for the console: its
# offscreen video driver and dummy audio, which is all --hidden replays use.
console=OFF
if [ "$(uname -s)" = Linux ] && ! pkg-config --exists x11 2>/dev/null && ! pkg-config --exists wayland-client 2>/dev/null; then
  console=ON; echo "sdl3: no X11 or Wayland headers, building without windows (offscreen video only)"
fi
build sdl3 "$sdl_tag" https://github.com/libsdl-org/SDL . -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF -DSDL_INSTALL_TESTS=OFF \
  -DSDL_UNIX_CONSOLE_BUILD=$console
build mt32emu "$mt_tag" https://github.com/munt/munt mt32emu -Dlibmt32emu_SHARED=ON
echo "PKG_CONFIG_PATH=$prefix/lib/pkgconfig"
