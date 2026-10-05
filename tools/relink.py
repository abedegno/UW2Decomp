"""Rewrites Markdown links after the merge into underworld-exhumed: GitHub URLs into UW1Decomp
or UW2Decomp's main branch become relative links into uw1/ or uw2/, and a relative link whose
target moved (the shared docs, now in docs/) is re-pointed. Run from the repository root; prints
each file it changed. Safe to run again."""
import os, re, subprocess

URL = re.compile(r'https://github\.com/abedegno/(UW1Decomp|UW2Decomp)/(?:blob|tree)/main/([^)\s#"`]*)(#[^)\s"`]*)?')
MOVED = {'uw2/docs/behaviour': 'docs/behaviour', 'uw2/docs/UW1-UW2-DIFFERENCES.md': 'docs/UW1-UW2-DIFFERENCES.md',
         'uw2/docs/FORMATS.md': 'docs/FORMATS.md', 'uw1/docs/UW1-UW2-DIFFERENCES.md': 'docs/UW1-UW2-DIFFERENCES.md',
         'uw1/docs/FORMATS.md': 'docs/FORMATS.md'}
# where a file in a moved directory used to live, for its relative links
OLD_HOME = {'docs/behaviour': 'uw2/docs/behaviour', 'docs': 'uw2/docs'}
LINK = re.compile(r'\]\(([^)\s#]+)(#[^)\s]*)?\)')


def moved(path):
    for old, new in MOVED.items():
        if path == old or path.startswith(old + '/'):
            return new + path[len(old):]
    return path


def fix(md):
    here = os.path.dirname(md) or '.'
    text = open(md, encoding='utf-8').read()

    def url(m):
        game = 'uw1' if m.group(1) == 'UW1Decomp' else 'uw2'
        target = moved(os.path.normpath(os.path.join(game, m.group(2) or '.')))
        return os.path.relpath(target, here) + (m.group(3) or '')
    new = URL.sub(url, text)

    def rel(m):
        t = m.group(1)
        if re.match(r'[a-z]+:', t) or os.path.exists(os.path.join(here, t)):
            return m.group(0)
        target = moved(os.path.normpath(os.path.join(OLD_HOME.get(here, here), t)))
        if not os.path.exists(target):
            return m.group(0)
        return '](' + os.path.relpath(target, here) + (m.group(2) or '') + ')'
    new = LINK.sub(rel, new)
    if new != text:
        open(md, 'w', encoding='utf-8').write(new)
        print(md)


for f in subprocess.run(['git', 'ls-files', '*.md'], capture_output=True, text=True, check=True).stdout.split():
    if not f.startswith('exhume/') and os.path.exists(f):
        fix(f)
