/* fuzzhost-uw1.c: UW1's side of Exhume's tools/fuzzhost.c ([fuzz] host_glue in exhume.toml): the
   memory a fuzzing case may set and the routines may change, and UW1's C entries as call kinds.
   Included by fuzzhost.c, which tools/fuzzasm.py links with the port's objects in place of
   src/port/sys/main.c. UW2's is Exhume's examples/uw2/port/fuzzhost-uw2.c.

   The regions (src/port/mem/fardata.c): the far data block the port loads from UW.EXE (395B:0000
   to the end of FD63, the graphics library's data seg048, stdat, cmpbuf1_start and the 3D data
   seg051 among them), seg063 (FD72, the system library's data, which IMATH reads its tables
   from), the code of seg000 to seg019 (the translated code reads and patches bytes there), and
   two scratch segments, E000 for data and F000 for the stack. The kinds, each a C entry of
   sys/c3dentry.c run against the routine the DOS side calls directly:

     cfst          cFstSinCos(BX): AX, BX = the two results
     csincos       cSinCos(BX): AX, BX
     csqrt         cSqRt(CX:BX): DI = the result
     catan2        cAtan2(AX, BX): CX */
#define FAR_FIRST 0x395B0u
#define FAR_END 0x55DC4u
#define FD72_SEG 0x5624u
#define CODE_FIRST 0x00000u
#define CODE_SIZE 0x2F3A0u
#define SCRATCH_SEG 0xE000u
#define STACK_SEG 0xF000u
#define LOAD (PORT_LOAD_SEG * 16u)

extern unsigned char port_far_block[];
extern unsigned char port_code_block[];
void cFstSinCos(int angle, int16_t *a, int16_t *b);
void cSinCos(int angle, int16_t *x, int16_t *y);
int cAtan2(int x, int y);
int cSqRt(int32_t v);

static unsigned char scratch[0x10010], stack[0x10010];
struct fuzz_region fuzz_regions[] = {
    { "far data", port_far_block, FAR_FIRST + LOAD, FAR_END - FAR_FIRST, 0 },
    { "seg063", seg063, FD72_SEG * 16u + LOAD, SEG063_SIZE, 0 },
    { "seg000-seg019 code", port_code_block, CODE_FIRST + LOAD, CODE_SIZE, 0 },
    { "scratch E000", scratch, SCRATCH_SEG * 16u, 0x10000, 0 },
    { "stack F000", stack, STACK_SEG * 16u, 0x10000, 0 },
};
int fuzz_nregions = (int)(sizeof fuzz_regions / sizeof fuzz_regions[0]);

int fuzz_init(const char *exe)
{
    if (port_load_exe(exe)) return 1;
    pm_add("fuzz scratch", scratch, 0x10000, SCRATCH_SEG);
    pm_add("fuzz stack", stack, 0x10000, STACK_SEG);
    return 0;
}

int fuzz_call(const char *kind, unsigned seg, unsigned off, uint32_t *c)
{
    (void)seg; (void)off; (void)c;
    if (!strcmp(kind, "cfst")) {
        int16_t a, b;
        cFstSinCos((int16_t)BX, &a, &b);
        AX = (uint16_t)a; BX = (uint16_t)b;
    } else if (!strcmp(kind, "csincos")) {
        int16_t a, b;
        cSinCos((int16_t)BX, &a, &b);
        AX = (uint16_t)a; BX = (uint16_t)b;
    } else if (!strcmp(kind, "csqrt")) DI = (uint16_t)cSqRt((int32_t)((uint32_t)CX << 16 | BX));
    else if (!strcmp(kind, "catan2")) CX = (uint16_t)cAtan2((int16_t)AX, (int16_t)BX);
    else return 0;
    return 1;
}
