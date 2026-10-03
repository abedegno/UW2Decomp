"""Package the release build of the port for players (make package, .github/workflows/release.yml):
the program from build/port-release (tools/portbuild.py --release), the shared libraries it
loads, tools/dist/README-dist.txt as README.txt, LICENSE, NOTICE and THIRD-PARTY-NOTICES (as
.txt), and the licence texts of the libraries in licenses/. No game data, no ROM and nothing
from the Borland toolchain goes in: the package is built from the sources alone.

    python3 tools/package.py [--version V] [--out DIR] [--strict]

  macOS    UW2-V-macos.zip: UW2.app (Contents/MacOS/uw2port, the libraries in
           Contents/Frameworks with their install names made @rpath ones, ad-hoc signed, since
           arm64 runs nothing unsigned) and the text files beside it
  Linux    UW2-V-linux-ARCH.tar.gz: ./uw2 (the launcher), bin/uw2port, lib/ (SDL3, libmt32emu,
           Nuked OPL3; the program's run path is $ORIGIN/../lib) and the text files
  Windows  UW2-V-windows-x86_64.zip (MSYS2 CLANG64): uw2port.exe and every DLL it loads that is
           not Windows's own (from ldd), and the text files

V defaults to $GITHUB_REF_NAME, else `git describe`. --strict fails when a library's licence
text cannot be found (CI uses it). The package goes to --out (default build/dist).
"""
import os, re, sys, shutil, argparse, subprocess, tarfile, zipfile, platform

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
REL = os.path.join(root, 'build', 'port-release')
LIBS = os.path.join(root, 'tools', 'libs')
DIST = os.path.join(here, 'dist')
WINDOWS = os.name == 'nt' or bool(os.environ.get('MSYSTEM'))

# The licence texts each bundled library needs, by the start of its file name, and where to look.
LICENCES = [
    ('SDL3', ('libSDL3', 'SDL3'), ['share/licenses/SDL3/LICENSE.txt', 'LICENSE.txt']),
    ('mt32emu', ('libmt32emu',), ['share/doc/munt/libmt32emu/COPYING.LESSER.txt']),
    ('Nuked-OPL3', ('libnukedopl3', 'nukedopl3'), []),
]


def run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, check=True, **kw).stdout


def version(arg):
    v = arg or os.environ.get('GITHUB_REF_NAME')
    if not v:
        try: v = run(['git', 'describe', '--tags', '--always', '--dirty'], cwd=root).strip()
        except (OSError, subprocess.CalledProcessError): v = 'dev'
    return re.sub(r'[^A-Za-z0-9._-]', '-', v)


def text_files(dest, ver):
    """README.txt, LICENSE.txt, NOTICE.txt, THIRD-PARTY-NOTICES.txt into dest."""
    readme = open(os.path.join(DIST, 'README-dist.txt'), encoding='utf-8').read().replace('@VERSION@', ver)
    with open(os.path.join(dest, 'README.txt'), 'w', encoding='utf-8', newline='\r\n' if WINDOWS else '\n') as f: f.write(readme)
    for name in ('LICENSE', 'NOTICE', 'THIRD-PARTY-NOTICES'):
        shutil.copy(os.path.join(root, name), os.path.join(dest, name + '.txt'))


def licence_texts(dest, libs, strict, extra=()):
    """licenses/NAME/... for each bundled library, from tools/libs, Homebrew or the fetched
    Nuked OPL3; extra is (name, path) pairs found by the caller."""
    out = os.path.join(dest, 'licenses')
    os.makedirs(out, exist_ok=True)
    missing = []
    bases = [LIBS, '/opt/homebrew/opt/sdl3', '/opt/homebrew/opt/mt32emu', '/usr/local/opt/sdl3', '/usr/local/opt/mt32emu']
    if WINDOWS: bases.append(winpath('/clang64'))       # MSYS2's SDL3 package
    for name, prefixes, rels in LICENCES:
        if not any(os.path.basename(l).startswith(prefixes) for l in libs): continue
        cands = [os.path.join(b, r) for b in bases for r in rels]
        if name == 'Nuked-OPL3': cands = [os.path.join(root, 'tools', 'nuked-opl3', 'LICENSE')]
        found = next((c for c in cands if os.path.isfile(c)), None)
        if not found: missing.append(name); continue
        os.makedirs(os.path.join(out, name), exist_ok=True)
        shutil.copy(found, os.path.join(out, name, os.path.basename(found)))
    for name, path in extra:
        os.makedirs(os.path.join(out, name), exist_ok=True)
        shutil.copy(path, os.path.join(out, name, os.path.basename(path)))
    for m in missing: print(f'package.py: no licence text found for {m}')
    if missing and strict: raise SystemExit(1)


# macOS

def macho_deps(path):
    lines = run(['otool', '-L', path]).splitlines()
    # a universal binary has a heading line (ending in a colon) before each slice's list
    return list(dict.fromkeys(l.strip().split(' (')[0] for l in lines if l.strip() and not l.rstrip().endswith(':')))


def rpaths(path):
    out, lines = [], run(['otool', '-l', path]).splitlines()
    for i, l in enumerate(lines):
        if 'cmd LC_RPATH' in l:
            out.append(lines[i + 2].strip().split()[1])
    return list(dict.fromkeys(out))      # a universal binary lists each slice's


def resolve_macho(dep, exe_rpaths):
    if dep.startswith('@rpath/'):
        name = dep[len('@rpath/'):]
        for d in [REL] + [p for p in exe_rpaths if not p.startswith('@')] + [os.path.join(LIBS, 'lib')]:
            if os.path.exists(os.path.join(d, name)): return os.path.join(d, name)
        return None
    return dep if os.path.exists(dep) else None


def package_macos(ver, out, strict):
    stage = os.path.join(out, f'UW2-{ver}-macos')
    shutil.rmtree(stage, ignore_errors=True)
    app = os.path.join(stage, 'UW2.app', 'Contents')
    fw = os.path.join(app, 'Frameworks')
    for d in ('MacOS', 'Frameworks', 'Resources'): os.makedirs(os.path.join(app, d))
    exe = os.path.join(app, 'MacOS', 'uw2port')
    shutil.copy(os.path.join(REL, 'uw2port'), exe)
    plist = open(os.path.join(DIST, 'Info.plist'), encoding='utf-8').read().replace('@VERSION@', ver.lstrip('v'))
    open(os.path.join(app, 'Info.plist'), 'w', encoding='utf-8').write(plist)
    open(os.path.join(app, 'PkgInfo'), 'w').write('APPL????')
    exe_rp = rpaths(exe)
    # copy every library outside the system's, following each library's own dependencies
    todo, copied = [exe], {}
    while todo:
        f = todo.pop()
        for dep in macho_deps(f):
            if dep.startswith(('/usr/lib/', '/System/')) or os.path.basename(dep) in copied: continue
            if f != exe and os.path.basename(dep) == os.path.basename(f): continue     # its own id
            src = resolve_macho(dep, exe_rp)
            if not src: raise SystemExit(f'package.py: cannot find {dep}, needed by {f}')
            name = os.path.basename(dep)
            dst = os.path.join(fw, name)
            shutil.copy(os.path.realpath(src), dst)
            os.chmod(dst, 0o755)
            copied[name] = dep
            todo.append(dst)
    for f in [exe] + [os.path.join(fw, n) for n in copied]:
        for dep in macho_deps(f):
            name = os.path.basename(dep)
            if name in copied and dep != '@rpath/' + name and not (f != exe and name == os.path.basename(f)):
                run(['install_name_tool', '-change', dep, '@rpath/' + name, f])
        if f != exe: run(['install_name_tool', '-id', '@rpath/' + os.path.basename(f), f])
    for rp in exe_rp:
        if not rp.startswith('@'): run(['install_name_tool', '-delete_rpath', rp, exe])
    if '@executable_path/../Frameworks' not in rpaths(exe):
        run(['install_name_tool', '-add_rpath', '@executable_path/../Frameworks', exe])
    for f in [os.path.join(fw, n) for n in copied] + [exe]:
        run(['codesign', '--force', '--sign', '-', f])
    run(['codesign', '--force', '--sign', '-', os.path.join(stage, 'UW2.app')])
    archs = {f: run(['lipo', '-archs', f]).strip() for f in [exe] + [os.path.join(fw, n) for n in copied]}
    for f, a in archs.items(): print(f'{os.path.relpath(f, stage)}: {a}')
    text_files(stage, ver)
    licence_texts(stage, list(copied), strict)
    zpath = os.path.join(out, f'UW2-{ver}-macos.zip')
    if os.path.exists(zpath): os.remove(zpath)
    run(['ditto', '-c', '-k', '--norsrc', '--noextattr', '--keepParent', stage, zpath])
    return zpath


# Linux

def package_linux(ver, out, strict):
    arch = platform.machine() or 'unknown'
    stage = os.path.join(out, f'UW2-{ver}-linux-{arch}')
    shutil.rmtree(stage, ignore_errors=True)
    for d in ('bin', 'lib'): os.makedirs(os.path.join(stage, d))
    exe = os.path.join(stage, 'bin', 'uw2port')
    shutil.copy(os.path.join(REL, 'uw2port'), exe)
    shutil.copy(os.path.join(DIST, 'uw2'), os.path.join(stage, 'uw2'))
    os.chmod(os.path.join(stage, 'uw2'), 0o755)
    env = dict(os.environ, LD_LIBRARY_PATH=os.pathsep.join([REL, os.path.join(LIBS, 'lib')]))
    ours = (REL + os.sep, os.path.join(LIBS, 'lib') + os.sep)
    libs = []
    for l in run(['ldd', os.path.join(REL, 'uw2port')], env=env).splitlines():
        m = re.match(r'\s*(\S+) => (\S+)', l)
        if not m: continue
        if m.group(2) == 'not': raise SystemExit(f'package.py: {m.group(1)} not found')
        if os.path.realpath(m.group(2)).startswith(ours) or m.group(2).startswith(ours):
            shutil.copy(os.path.realpath(m.group(2)), os.path.join(stage, 'lib', m.group(1)))
            libs.append(m.group(1))
    for l in libs: print('lib/' + l)
    text_files(stage, ver)
    licence_texts(stage, libs, strict)
    tpath = os.path.join(out, f'UW2-{ver}-linux-{arch}.tar.gz')
    with tarfile.open(tpath, 'w:gz') as t: t.add(stage, arcname=os.path.basename(stage))
    return tpath


# Windows (MSYS2)

def winpath(p):
    return run(['cygpath', '-w', p]).strip() if p.startswith('/') else p


def package_windows(ver, out, strict):
    stage = os.path.join(out, f'UW2-{ver}-windows-x86_64')
    shutil.rmtree(stage, ignore_errors=True)
    os.makedirs(stage)
    exe = os.path.join(REL, 'uw2port.exe')
    shutil.copy(exe, stage)
    env = dict(os.environ)
    env['PATH'] = os.pathsep.join([REL, os.path.join(LIBS, 'bin'), env.get('PATH', '')])
    dlls, extra = [], []
    for l in run(['ldd', exe], env=env).splitlines():
        m = re.match(r'\s*(\S+) => (.+?) \(0x', l)
        if not m: continue
        name, path = m.group(1), m.group(2)
        if path == 'not found': raise SystemExit(f'package.py: {name} not found')
        if re.match(r'(?i)(/c/windows/|c:\\windows\\)', path): continue      # Windows's own
        shutil.copy(winpath(path), os.path.join(stage, name))
        dlls.append(name)
        # the licence of an MSYS2 package's DLL (the compiler runtime's), from its package
        if path.startswith('/clang64/'):
            try:
                pkg = run(['pacman', '-Qqo', path]).strip()
                d = '/clang64/share/licenses/' + re.sub(r'^mingw-w64-clang-x86_64-', '', pkg)
                if os.path.isdir(winpath(d)):
                    for f in sorted(os.listdir(winpath(d))):
                        extra.append((re.sub(r'^mingw-w64-clang-x86_64-', '', pkg), os.path.join(winpath(d), f)))
            except (OSError, subprocess.CalledProcessError):
                pass
    for d in dlls: print(d)
    text_files(stage, ver)
    licence_texts(stage, dlls, strict, extra)
    zpath = os.path.join(out, f'UW2-{ver}-windows-x86_64.zip')
    with zipfile.ZipFile(zpath, 'w', zipfile.ZIP_DEFLATED) as z:
        for d, _, fs in os.walk(stage):
            for f in fs:
                p = os.path.join(d, f)
                z.write(p, os.path.relpath(p, out))
    return zpath


def main(argv):
    ap = argparse.ArgumentParser(description='Package the release build of the port.')
    ap.add_argument('--version')
    ap.add_argument('--out', default=os.path.join(root, 'build', 'dist'))
    ap.add_argument('--strict', action='store_true')
    a = ap.parse_args(argv)
    ver = version(a.version)
    if not os.path.exists(os.path.join(REL, 'uw2port.exe' if WINDOWS else 'uw2port')):
        print('package.py: build/port-release has no program; run make port-release first')
        return 1
    os.makedirs(a.out, exist_ok=True)
    if sys.platform == 'darwin': p = package_macos(ver, a.out, a.strict)
    elif WINDOWS: p = package_windows(ver, a.out, a.strict)
    else: p = package_linux(ver, a.out, a.strict)
    print(f'wrote {os.path.relpath(p, root)} ({os.path.getsize(p)} bytes)')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
