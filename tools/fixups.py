"""Full OMF fixup reader for Turbo C objects.
fixups(objbytes) -> dict with segs, groups, externs, publics, data, and a list of fixups:
  {seg, off, loc, rel, frame:(method,idx), target:(method,idx), disp}
loc: 0 lobyte, 1 offset16, 2 base16, 3 ptr32, 4 hibyte, 5 offset16 (loader).
target/frame methods: 0 SEGDEF, 1 GRPDEF, 2 EXTDEF, 3 frame, 4 LOCATION, 5 TARGET."""
import struct
from omf import records, idx

def fixups(d):
    lnames=['']; segs=[None]; groups=[None]; ext=[None]; pubs={}; data={}; fx=[]
    tthr=[None]*4; fthr=[None]*4; last=None
    for t,b,p in records(d):
        if t==0x96:
            i=0
            while i<len(b): l=b[i]; lnames.append(b[i+1:i+1+l].decode('latin1')); i+=1+l
        elif t in (0x98,0x99):
            attr=b[0]; i=1
            if (attr>>5)==0: i+=3
            ln=struct.unpack_from('<H',b,i)[0]; i+=2
            sn,i=idx(b,i); cn,i=idx(b,i)
            segs.append((lnames[sn],lnames[cn],ln)); data[len(segs)-1]=bytearray(ln)
        elif t==0x9A:
            gn,i=idx(b,0); members=[]
            while i<len(b): i+=1; s,i=idx(b,i); members.append(s)
            groups.append((lnames[gn],members))
        elif t in (0x8C,0xB4):
            i=0
            while i<len(b):
                l=b[i]; ext.append(b[i+1:i+1+l].decode('latin1')); i+=1+l; _,i=idx(b,i)
        elif t in (0x90,0x91):
            g,i=idx(b,0); s,i=idx(b,i)
            if s==0: i+=2
            while i<len(b):
                l=b[i]; n=b[i+1:i+1+l].decode('latin1'); i+=1+l
                o=struct.unpack_from('<H',b,i)[0]; i+=2; _,i=idx(b,i); pubs[n]=(s,o)
        elif t in (0xA0,0xA1):
            si,i=idx(b,0); off=struct.unpack_from('<H',b,i)[0]; i+=2
            data[si][off:off+len(b)-i]=b[i:]; last=(si,off)
        elif t in (0x9C,0x9D):
            i=0
            while i<len(b):
                c=b[i]
                if not c&0x80:          # THREAD
                    dbit=c&0x40; meth=(c>>2)&7; num=c&3; i+=1; ix=None
                    if meth<3: ix,i=idx(b,i)
                    (fthr if dbit else tthr)[num]=(meth,ix)
                    continue
                rel=bool(c&0x40); loc=(c>>2)&0xF; off=((c&3)<<8)|b[i+1]; i+=2
                fd=b[i]; i+=1
                if fd&0x80: frame=fthr[(fd>>4)&3]
                else:
                    fm=(fd>>4)&7; fi=None
                    if fm<3: fi,i=idx(b,i)
                    frame=(fm,fi)
                if fd&0x08: tm,ti=tthr[fd&3]; tm=tm&3
                else:
                    tm=fd&3; ti,i=idx(b,i)
                disp=0
                if not fd&0x04: disp=struct.unpack_from('<H',b,i)[0]; i+=2
                fx.append(dict(seg=last[0],off=last[1]+off,loc=loc,rel=rel,frame=frame,target=(tm,ti),disp=disp))
    return dict(lnames=lnames,segs=segs,groups=groups,ext=ext,pubs=pubs,data=data,fixups=fx)

def target_name(o,tgt):
    m,i=tgt
    return {0:lambda:'SEG:'+o['segs'][i][0],1:lambda:'GRP:'+o['groups'][i][0],2:lambda:o['ext'][i]}.get(m,lambda:f'?{m}:{i}')()
