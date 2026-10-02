/* overlay.c: replaces src/lib/OVERLAY.ASM, Borland's VROOMM overlay manager (OVERLAY.LIB). The
   port links every function into one image, so there is nothing to load or swap: telling the
   manager to keep overlays in EMS (TMPALLOC.C's init_mem) succeeds and does nothing. */
int _OvrInitEms(unsigned emsHandle, unsigned firstPage, unsigned pages)
{
    (void)emsHandle;
    (void)firstPage;
    (void)pages;
    return 0;
}
