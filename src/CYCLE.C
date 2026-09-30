struct CRNG { unsigned int last; unsigned int rate; unsigned int flags; unsigned char low, high; };
extern unsigned int far *PITTimerGlobal;
void far RotatePaletteEntry(unsigned char first, unsigned char count, int unused);
void far ApplyPalette(int count, unsigned char first);

void far CycleColours(struct CRNG far *r)
{
    unsigned char unused;
    unsigned char count;
    int i;

    for (i = 0; i < 16; i++)
    {
        if (r[i].rate != 0)
        {
            if (*PITTimerGlobal - r[i].last < 0x38E / r[i].rate)
                continue;
            count = r[i].high - r[i].low + 1;
            RotatePaletteEntry(r[i].low, count, 0);
            ApplyPalette(count, r[i].low);
            r[i].last = *PITTimerGlobal;
        }
    }
}
