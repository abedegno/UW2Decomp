"""Minimal OMF reader: segment names and LEDATA/LIDATA bytes per segment, and module names in a .LIB."""
import struct,sys
def records(d,start=0,end=None):
    p=start; end=len(d) if end is None else end
    while p+3<=end:
        t=d[p]; n=struct.unpack_from('<H',d,p+1)[0]
        yield t,d[p+3:p+3+n-1],p
        p+=3+n
        if t==0x8A or t==0x8B: return
def idx(b,i):
    v=b[i]; 
    if v&0x80: return ((v&0x7f)<<8)|b[i+1], i+2
    return v, i+1
def module(d,start=0):
    lnames=['']; segs=[]; data={}; name=None
    for t,b,p in records(d,start):
        if t==0x80: name=b[1:1+b[0]].decode('latin1')
        elif t==0x96:
            i=0
            while i<len(b): l=b[i]; lnames.append(b[i+1:i+1+l].decode('latin1')); i+=1+l
        elif t in (0x98,0x99):
            attr=b[0]; i=1
            if (attr>>5)==0: i+=3
            ln=struct.unpack_from('<H',b,i)[0]; i+=2
            sn,i=idx(b,i); cn,i=idx(b,i)
            segs.append((lnames[sn],lnames[cn],ln)); data[len(segs)]=bytearray(ln)
        elif t in (0xA0,0xA1):
            si,i=idx(b,0); off=struct.unpack_from('<H',b,i)[0]; i+=2
            chunk=b[i:]; buf=data[si]; buf[off:off+len(chunk)]=chunk
        if t in (0x8A,0x8B): return name,segs,data,p+3+struct.unpack_from('<H',d,p+1)[0]
    return name,segs,data,len(d)
if __name__=='__main__':
    d=open(sys.argv[1],'rb').read(); n,segs,data,_=module(d)
    print(n); [print(' ',i+1,s) for i,s in enumerate(segs)]

def module_masked(d,start=0):
    """Like module(), but also returns per-segment masks: 1 where a fixup writes."""
    lnames=['']; segs=[]; data={}; mask={}; name=None; last=None
    for t,b,p in records(d,start):
        if t==0x80: name=b[1:1+b[0]].decode('latin1')
        elif t==0x96:
            i=0
            while i<len(b): l=b[i]; lnames.append(b[i+1:i+1+l].decode('latin1')); i+=1+l
        elif t in (0x98,0x99):
            attr=b[0]; i=1
            if (attr>>5)==0: i+=3
            ln=struct.unpack_from('<H',b,i)[0]; i+=2
            sn,i=idx(b,i); cn,i=idx(b,i)
            segs.append((lnames[sn],lnames[cn],ln)); data[len(segs)]=bytearray(ln); mask[len(segs)]=bytearray(ln)
        elif t in (0xA0,0xA1):
            si,i=idx(b,0); off=struct.unpack_from('<H',b,i)[0]; i+=2
            chunk=b[i:]
            if si not in data: last=None; continue
            data[si][off:off+len(chunk)]=chunk; last=(si,off)
        elif t in (0x9C,0x9D) and last:
            i=0
            while i<len(b):
                c=b[i]
                if c&0x80:
                    loc=(c>>2)&0xF; off=((c&3)<<8)|b[i+1]; i+=2
                    size={0:1,1:2,2:2,3:4,4:1,5:2,9:2,11:6,13:4}.get(loc,2)
                    si,base=last
                    for k in range(size):
                        if si in mask and base+off+k<len(mask[si]): mask[si][base+off+k]=1
                    fd=b[i]; i+=1
                    if not fd&0x80 and ((fd>>4)&7)<3: _,i=idx(b,i)
                    if not fd&0x08 and (fd&3)<3: _,i=idx(b,i)
                    if not fd&0x04: i+=2
                else:
                    meth=(c>>2)&7; i+=1
                    if meth<3: _,i=idx(b,i)
        if t in (0x8A,0x8B): return name,segs,data,mask,p+3+struct.unpack_from('<H',d,p+1)[0]
    return name,segs,data,mask,len(d)

def lib_modules(d):
    """Iterate modules in a Borland/Microsoft .LIB (page-aligned OMF)."""
    page=struct.unpack_from('<H',d,1)[0]+3
    p=page
    while p<len(d) and d[p]==0x80:
        r=module_masked(d,p)
        yield r
        end=r[4]; p=((end+page-1)//page)*page
