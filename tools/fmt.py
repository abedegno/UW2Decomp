"""Disassemble a function from the FM Towns UW2 build, naming calls and globals.
usage: fmt.py NAME_        (e.g. fmt.py dream_)
Needs fmtowns/uw2fmt.img and fmtowns/syms.tsv (see README). 32-bit Watcom register-call code:
arguments arrive in EAX, EDX, EBX, ECX. A second witness for meaning; DOS decides the bytes."""
import sys,os,re
from iced_x86 import Decoder,Formatter,FormatterSyntax
root=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
img=open(os.path.join(root,'fmtowns','uw2fmt.img'),'rb').read()
syms={}
for l in open(os.path.join(root,'fmtowns','syms.tsv')):
    n,a,s=l.split('\t'); syms[int(a,16)]=n
addrs=sorted(syms); byname={v:k for k,v in syms.items()}
a=byname[sys.argv[1]]; end=next(x for x in addrs if x>a)
f=Formatter(FormatterSyntax.NASM)
def name(v):
    if v in syms: return syms[v]
    lo=max([x for x in addrs if x<=v],default=None)
    return f'{syms[lo]}+{v-lo:#x}' if lo is not None and v-lo<0x400 else None
for i in Decoder(32,img[a:end],ip=a):
    s=f.format(i)
    for h in re.findall(r'\b([0-9A-F]{5,8})h\b',s):
        n=name(int(h,16))
        if n: s=s.replace(h+'h',n)
    print(f'{i.ip:08x}  {s}')
