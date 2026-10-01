"""Compile a source file and compare every function with UW2.EXE.
usage: match.py src/FILE.C [--dis NAME] [--no-build]
The target table is targets/<segment>.tsv, named by a '/* target: ovr154 */' line in the source.
Fixup bytes are masked; the linker's far->near call rewrite (9A .. -> 90 0E E8 ..) counts as equal."""
import sys,os,re,struct,subprocess
here=os.path.dirname(os.path.abspath(__file__)); root=os.path.dirname(here)
sys.path.insert(0,here)
from omf import records,idx,module_masked
OPTS='-mm -1 -G -O -Z'
EXE=os.path.expanduser(os.environ.get('UW2_EXE','~/UWGOG/UW2/UW2.EXE'))

def publics(d):
    lnames=['']; segs=[]; pubs={}
    for t,b,p in records(d):
        if t==0x96:
            i=0
            while i<len(b): l=b[i]; lnames.append(b[i+1:i+1+l].decode('latin1')); i+=1+l
        elif t in (0x98,0x99): segs.append(1)
        elif t in (0x90,0x91):
            g,i=idx(b,0); s,i=idx(b,i)
            if s==0: i+=2
            while i<len(b):
                l=b[i]; n=b[i+1:i+1+l].decode('latin1'); i+=1+l
                o=struct.unpack_from('<H',b,i)[0]; i+=2; _,i=idx(b,i)
                pubs[n]=(s,o)
    return pubs

def load_targets(seg):
    rows=[]; base=size=None; org=0
    for l in open(os.path.join(root,'targets',seg+'.tsv')):
        m=re.match(r'# segment \S+ base 0x([0-9A-F]+) size 0x([0-9A-F]+)(?: org 0x([0-9A-F]+))?',l)
        if m: base,size=int(m.group(1),16),int(m.group(2),16); org=int(m.group(3) or '0',16); continue
        if l.startswith('#') or not l.strip(): continue
        c,ida,o,s=l.split('\t'); rows.append((c,ida,int(o,16),int(s,16)))
    return base,size,rows,org

def compare(b,m,o):
    b=bytearray(b); m=list(m)
    for j in range(len(b)-4):
        if b[j]==0x9a and all(m[j+1:j+5]) and o[j:j+3]==b'\x90\x0e\xe8':
            b[j:j+2]=b'\x90\x0e'; m[j+2]=0; b[j+2]=0xe8; m[j+1]=0
    return [k for k in range(min(len(b),len(o))) if not m[k] and b[k]!=o[k]]

def dis(buf):
    from iced_x86 import Decoder,Formatter,FormatterSyntax
    f=Formatter(FormatterSyntax.NASM); r=[]
    for i in Decoder(16,bytes(buf)):
        s=f.format(i); mn=s.split()[0]
        r.append(mn if mn.startswith('j') or mn in('call','loop') else s)
    return r

def main():
    a=sys.argv[1:]; src=a[0]; stem=os.path.splitext(os.path.basename(src))[0].upper()
    text=open(src,encoding='latin1').read()
    seg=re.search(r'/\*\s*target:\s*(\w+)\s*\*/',text).group(1)
    opts=re.search(r'/\*\s*opts:\s*([^*]+?)\s*\*/',text); opts=opts.group(1) if opts else OPTS
    out=os.path.join(root,'build',stem)
    if '--no-build' not in a:
        r=subprocess.run(['node',os.path.join(here,'tcc.mjs'),out,opts,src],capture_output=True,text=True)
        log=open(os.path.join(out,'BUILD.LOG'),encoding='latin1').read()
        msgs=[l for l in log.splitlines() if re.search(r'(Error|Warning|Fatal)',l)]
        for l in msgs: print(l)
        if any('Error' in l or 'Fatal' in l for l in msgs): sys.exit(1)
    d=open(os.path.join(out,stem+'.OBJ'),'rb').read()
    _,segs,data,mask,_=module_masked(d); pubs=publics(d)
    ci=[i for i,(sn,cn,ln) in enumerate(segs,1) if cn=='CODE']
    if not ci and '--no-build' not in a and '--retried' not in a:
        # the emulator occasionally returns a truncated object; build once more
        print('object has no code segment; rebuilding'); sys.argv.append('--retried'); return main()
    ci=ci[0]
    code,cm=data[ci],mask[ci]
    base,size,rows,org=load_targets(seg); exe=open(EXE,'rb').read()
    whole=len(code)==size and not compare(code,cm,exe[base:base+size])
    total=done=0
    for c,ida,off,sz in rows:
        p=pubs.get(('_'+c)[:33])      # Turbo C keeps 32 characters of a name, after the underscore
        if p is None: continue
        o=p[1]; total+=sz
        # a function's end in the object: the next public, or the end of the code
        nxt=min([q[1] for q in pubs.values() if q[0]==ci and q[1]>o]+[len(code)])
        mine=nxt-o; bad=compare(code[o:nxt],cm[o:nxt],exe[base+off:base+off+sz])
        ok=mine==sz and not bad
        if ok: done+=sz
        state='MATCH' if ok else (f'{len(bad)} bytes differ, first at +0x{bad[0]:X}' if bad else '')+('' if mine==sz else f'  size 0x{mine:X} want 0x{sz:X}')
        print(f'{c:22} {ida:45} {state}')
        if '--dis' in a and a[a.index('--dis')+1]==c:
            import difflib
            for t in difflib.unified_diff(dis(exe[base+off:base+off+sz]),dis(code[o:nxt]),'exe','obj',lineterm='',n=2): print('   ',t)
    print(f'-- {done}/{size} bytes of {seg} matched' + ('; WHOLE SEGMENT MATCHES' if whole else ''))
if __name__ == "__main__":
    main()
