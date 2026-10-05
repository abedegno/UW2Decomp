/* fardata.c: replaces src/sys/FARDATA.ASM and the far data tools/extract.py takes from UW2.EXE
   (docs/PORT.md, "Replacing the assembly modules"): the far data segments from 3705:0000 to
   5F5D:104C, laid out as the EXE has them, and seg021's data segment, dseg062_62a6. Their
   bytes are read from the user's own UW2.EXE at start-up (port_load_exe), so the port holds no
   game data, and the EXE is checked by its size and CRC-32 first.

   The far segments are one block, in the EXE's order and at the EXE's distances, so that what
   runs past the end of one segment lands in the next as it did in DOS (ACLZW.C's work area runs
   from stdat into cmpbuf1_start). Each segment is a region of the paragraph map with its DOS
   paragraph, and the names the C uses are labels inside the block at their DOS offsets:

     3705:0000  checkerboard1 (FD50)          3CF5:0000  stdat, ATM_Strings at 468E (FD52)
     370D:0008  seg_370D, seg003's data (FD51) 43A0:0000  cmpbuf1_start, cmpbuf2_start, gr_offs
     4EAF:0000  dfx_buffer (FD54)             4FAF:0000  seg052_519C, seg004's data (FD58)
     5DFD:0000  seg_5DFD (FD60)               5EFD:0000  sp_inf_tab (FD62)
     5F5D:0004  seg_5F5D (FD63)

   The labels are made with top-level assembly: C cannot put a named object inside another.
   On Mach-O each inner label is an .alt_entry, so the linker keeps the block in one piece. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"

#define FAR_FIRST 0x37050u                      /* 3705:0000, linear */
#define FAR_END   0x6061Cu                      /* 5F5D:104C */
#define FAR_SIZE  (FAR_END - FAR_FIRST)
#define DGROUP_SEG 0x65E9u                      /* DS, as an EXE paragraph */
#define FD71_SEG  0x60B9u                       /* dseg062_62a6 */
#define EXE_SIZE  675184L
#define EXE_CRC   0xC824FB97u                   /* the GOG release's UW2.EXE */

#ifdef __APPLE__
#define S(n) "_" #n
#define ALT(n) ".alt_entry _" #n "\n"
#define SECTION ".section __DATA,__data\n"
#define HERE "Lport_far_block"
#else
#define S(n) #n
#define ALT(n) ""
#define SECTION ".data\n"
#define HERE ".Lport_far_block"
#endif
#define AT(n, off) ".globl " S(n) "\n" ALT(n) ".org " HERE " + " #off "\n" S(n) ":\n"

__asm__(
    SECTION
    ".p2align 4\n"
    ".globl " S(port_far_block) "\n"
    S(port_far_block) ":\n" HERE ":\n"
    AT(checkerboard1, 0x0)
    AT(seg_370D, 0x88)                          /* 370D:0008 */
    AT(smooth_div, 0x6CC)                       /* 370D:064C */
    AT(smooth_base, 0x6CE)                      /* 370D:064E */
    AT(smooth_lowpass, 0x6D0)                   /* 370D:0650 */
    AT(ShowClip, 0xE44)                         /* 370D:0DC4 */
    AT(Transparency, 0xE45)                     /* 370D:0DC5 */
    AT(stdat, 0x5F00)                           /* 3CF5:0000 */
    AT(seg049_3EE2, 0x5F00)
    AT(ATM_Strings, 0xA58E)                     /* 3CF5:468E */
    AT(cmpbuf1_start, 0xC9B0)                   /* 43A0:0000 */
    AT(cmpbuf2_start, 0x11DB0)                  /* 43A0:5400 */
    AT(gr_offs, 0x171B0)                        /* 43A0:A800 */
    AT(dfx_buffer, 0x17AA0)                     /* 4EAF:0000 */
    AT(seg052_519C, 0x18AA0)                    /* 4FAF:0000 */
    AT(ModelData_seg052_519C_2600, 0x1B0A0)     /* 4FAF:2600 */
    AT(_dblen, 0x2521E)                         /* 4FAF:C77E */
    AT(bmhgtoff, 0x25222)                       /* 4FAF:C782 */
    AT(bmsegoff, 0x25224)                       /* 4FAF:C784 */
    AT(first_anim, 0x25AD0)                     /* 4FAF:D030 */
    AT(grs_3dinf, 0x26AE9)                      /* 4FAF:E049 */
    AT(CmaptoPg, 0x26B69)                       /* 4FAF:E0C9 */
    AT(PgtoCmap, 0x26C69)                       /* 4FAF:E1C9 */
    AT(CmapFrm, 0x26D69)                        /* 4FAF:E2C9 */
    AT(CmapCache, 0x26E69)                      /* 4FAF:E3C9 */
    AT(crit_fpage, 0x26F69)                     /* 4FAF:E4C9 */
    AT(crit_nlpages, 0x26F6B)                   /* 4FAF:E4CB */
    AT(crit_inpage, 0x26F6D)                    /* 4FAF:E4CD */
    AT(tmap_inpage, 0x26F6E)                    /* 4FAF:E4CE */
    AT(tmap_fpage, 0x26F6F)                     /* 4FAF:E4CF */
    AT(scrgr_fpage, 0x26F70)                    /* 4FAF:E4D0 */
    AT(obj_inpage1, 0x26F71)                    /* 4FAF:E4D1 */
    AT(EmsBuff, 0x26F72)                        /* 4FAF:E4D2 */
    AT(seg052_519C_E4D4, 0x26F74)               /* 4FAF:E4D4 */
    AT(seg_5DFD, 0x26F80)                       /* 5DFD:0000 */
    AT(sp_inf_tab, 0x27F80)                     /* 5EFD:0000 */
    AT(seg_5F5D, 0x28584)                       /* 5F5D:0004 */
    ".org " HERE " + 0x295CC\n"
    ".p2align 4\n"
);

/* The code segments seg003 (the graphics library, 0085:0000) to seg004 (the renderer, 065C:FFFF)
   as one block, as DOS loaded them (the two segments' windows overlap: seg003 reads seg004's
   bytes at es:6D20h). The translated assembly modules keep data in their code segments, and
   patch their own immediates, so the bytes are here and the code reads and writes them
   (x86/asmrt.h). The names the C reaches are labels inside it: cXfer (SCALEBM.ASM's XFER.DAT
   tables, 0085:0E73), uncmp_pal (EXPAND.ASM's, 065C:0000), PGCACHE.ASM's 16 bytes at
   065C:6D20 and lightabs (065C:6D3E, to the segment's end and on: a shade row from 16 up
   reads what follows it in DOS). */
#define CODE_FIRST 0x00850u
#define CODE_SIZE  (0x065C0u - CODE_FIRST + 0x10010u)
#ifdef __APPLE__
#define HEREC "Lport_code_block"
#else
#define HEREC ".Lport_code_block"
#endif
#define ATC(n, off) ".globl " S(n) "\n" ALT(n) ".org " HEREC " + " #off "\n" S(n) ":\n"
__asm__(
    SECTION
    ".p2align 4\n"
    ".globl " S(port_code_block) "\n"
    S(port_code_block) ":\n" HEREC ":\n"
    ATC(cXfer, 0xE73)                           /* 0085:0E73 */
    ATC(uncmp_pal, 0x5D70)                      /* 065C:0000 */
    ATC(seg004_0849_6D20, 0xCA90)               /* 065C:6D20 */
    ATC(lightabs, 0xCAAE)                       /* 065C:6D3E */
    ".org " HEREC " + 0x15D80\n"
    ".p2align 4\n"
);
extern unsigned char port_code_block[];

extern unsigned char port_far_block[];
unsigned char dseg062_62a6[DSEG062_62A6_SIZE];
unsigned char port_dgroup_image[0x10000];

/* The DOS segments inside the block, for the paragraph map. */
static const struct { const char *name; unsigned seg; uint32_t lin, size; } segs[] = {
    { "FD50 checkerboard1", 0x3705, 0x37050, 0x80 },
    { "FD51 seg_370D", 0x370D, 0x370D0, 0x5E80 },
    { "FD52 stdat", 0x3CF5, 0x3CF50, 0x6AB0 },
    { "FD53 cmpbuf", 0x43A0, 0x43A00, 0xB0F0 },
    { "FD54 dfx_buffer", 0x4EAF, 0x4EAF0, 0x1000 },
    { "FD58 seg052_519C", 0x4FAF, 0x4FAF0, 0xE4E0 },
    { "FD60 seg_5DFD", 0x5DFD, 0x5DFD0, 0x1000 },
    { "FD62 sp_inf_tab", 0x5EFD, 0x5EFD0, 0x600 },
    { "FD63 seg_5F5D", 0x5F5D, 0x5F5D0, 0x104C },
};

static uint32_t crc32(const unsigned char *p, size_t n)
{
    uint32_t c = 0xFFFFFFFFu;
    int k;
    while (n--) {
        c ^= *p++;
        for (k = 0; k < 8; k++) c = c & 1 ? 0xEDB88320u ^ (c >> 1) : c >> 1;
    }
    return c ^ 0xFFFFFFFFu;
}

/* DOS adds the load segment to every word the EXE's relocation table lists as it loads the
   program; those in the blocks copied above are segment parts of far pointers in the data
   (seg003's pointers into its own data and stdat, seg021's, DGROUP's), so they get the port's
   load segment, which the paragraph map gives the blocks. */
static void relocate(const unsigned char *exe)
{
    unsigned n = (unsigned)(exe[6] | exe[7] << 8), at = (unsigned)(exe[0x18] | exe[0x19] << 8), i;
    for (i = 0; i < n; i++) {
        const unsigned char *r = exe + at + 4 * i;
        uint32_t lin = (uint32_t)(r[2] | r[3] << 8) * 16 + (uint32_t)(r[0] | r[1] << 8);
        unsigned char *w = NULL;
        unsigned v;
        if (lin >= FAR_FIRST && lin + 1 < FAR_END) w = port_far_block + (lin - FAR_FIRST);
        else if (lin >= CODE_FIRST && lin + 1 < CODE_FIRST + CODE_SIZE) w = port_code_block + (lin - CODE_FIRST);
        else if (lin >= FD71_SEG * 16u && lin + 1 < FD71_SEG * 16u + DSEG062_62A6_SIZE) w = dseg062_62a6 + (lin - FD71_SEG * 16u);
        else if (lin >= DGROUP_SEG * 16u && lin + 1 < DGROUP_SEG * 16u + sizeof port_dgroup_image)
            w = port_dgroup_image + (lin - DGROUP_SEG * 16u);
        if (!w) continue;
        v = (unsigned)(w[0] | w[1] << 8) + PORT_LOAD_SEG;
        w[0] = (unsigned char)v;
        w[1] = (unsigned char)(v >> 8);
    }
}

/* Whether path is the GOG release's UW2.EXE, without a message: 0 if it is, -1 if it cannot be
   read, -2 if its size is wrong, -3 if its CRC-32 is (sys/gamedir.c, looking for the game). */
int port_check_exe(const char *path)
{
    FILE *f = fopen(path, "rb");
    unsigned char *exe;
    long n;
    int r;
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n != EXE_SIZE) { fclose(f); return -2; }
    exe = malloc((size_t)n);
    if (!exe || fread(exe, 1, (size_t)n, f) != (size_t)n) { fclose(f); free(exe); return -1; }
    fclose(f);
    r = crc32(exe, (size_t)n) == EXE_CRC ? 0 : -3;
    free(exe);
    return r;
}

int port_load_exe(const char *path)
{
    FILE *f = fopen(path, "rb");
    unsigned char *exe;
    long n;
    unsigned hdr;
    size_t i;
    if (!f) { fprintf(stderr, "uw2port: cannot open %s\n", path); return -1; }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n != EXE_SIZE) {
        fprintf(stderr, "uw2port: %s is %ld bytes, not the %ld of the GOG release's UW2.EXE\n", path, n, EXE_SIZE);
        fclose(f);
        return -1;
    }
    exe = malloc((size_t)n);
    if (!exe || fread(exe, 1, (size_t)n, f) != (size_t)n) { fclose(f); free(exe); return -1; }
    fclose(f);
    if (crc32(exe, (size_t)n) != EXE_CRC) {
        fprintf(stderr, "uw2port: %s is not the GOG release's UW2.EXE (CRC-32 %08X)\n", path, crc32(exe, (size_t)n));
        free(exe);
        return -1;
    }
    hdr = (unsigned)(exe[8] | exe[9] << 8) * 16;
    memcpy(port_far_block, exe + hdr + FAR_FIRST, FAR_SIZE);
    memcpy(dseg062_62a6, exe + hdr + FD71_SEG * 16, DSEG062_62A6_SIZE);
    memcpy(port_code_block, exe + hdr + CODE_FIRST, CODE_SIZE);
    memcpy(port_dgroup_image, exe + hdr + DGROUP_SEG * 16, sizeof port_dgroup_image);
    relocate(exe);
    free(exe);
    for (i = 0; i < sizeof segs / sizeof segs[0]; i++)
        pm_add(segs[i].name, port_far_block + (segs[i].lin - FAR_FIRST), segs[i].size, segs[i].seg + PORT_LOAD_SEG);
    pm_add("FD71 dseg062_62a6", dseg062_62a6, DSEG062_62A6_SIZE, FD71_SEG + PORT_LOAD_SEG);
    pm_add("seg003-seg004 code", port_code_block, CODE_SIZE, (CODE_FIRST >> 4) + PORT_LOAD_SEG);
    return 0;
}
