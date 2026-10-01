"""Predict where Turbo C 1.01 puts uninitialised file-scope variables in _BSS.
usage: bssorder.py NAME [NAME ...]      prints the names in emitted order with their keys

Turbo C emits a file's _BSS variables in ascending order of a key computed from the name
(without the leading underscore); names with equal keys keep definition order. Found from
probe compiles while matching seg015, and it predicts seg010's and ovr137's layouts too.
Use it to choose names for statics that have no original name."""
import sys

def key(name):
    b = name.encode(); n = len(b)
    second = b[1] if n > 1 else 0
    penult = b[n - 2] if n > 1 else 0
    return (b[0] + 256 * second + 8 * penult + 64 * n) & 1023

if __name__ == '__main__':
    for i, n in sorted(enumerate(sys.argv[1:]), key=lambda t: (key(t[1]), t[0])):
        print(f'{key(n):4}  {n}')
