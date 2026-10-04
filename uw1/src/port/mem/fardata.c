/* fardata.c: replaces src/sys/FARDATA.ASM and the far data tools/extract.py takes from UW.EXE
   (Exhume's docs/port.md, "Platform and machine"): the far data segments from 395B:0000 to
   54D7:105C (FD51 to FD63), laid out as the EXE has them, and seg019's data segment, seg063
   (FD72, 5624:0000). Their bytes are read from the user's own UW.EXE at start-up
   (port_load_exe), so the port holds no game data, and the EXE is checked by its size and CRC-32
   first. The addresses are the link's (build/LINK/out/UW.MAP, which docs/LINKING.md explains).

   The far segments are one block, in the EXE's order and at the EXE's distances, so that what
   runs past the end of one segment lands in the next as it did in DOS. Each segment is a region
   of the paragraph map with its DOS paragraph, and the names the C uses are labels inside the
   block at their DOS offsets:

     395B:0000  checkerboard1 (FD51)            3963:0000  seg048, seg003's data (FD52)
     3F4B:0000  stdat, ATM_Strings at 3336 (FD53)   4423:0000  cmpbuf1_start, fade_buffer (FD54)
     4723:0000  seg051, seg004's data (FD58)    5371:0000  seg_5DFD (FD60)
     5471:0000  seg053 (FD61)                   5477:0008  sp_inf_tab (FD62)
     54D7:000C  seg055, VALLOC's records (FD63)

   The C files' own far variables (PATHFIND5_FAR and the rest, 55DD:0000 on) are the port's C.
   The labels are made with top-level assembly: C cannot put a named object inside another.
   On Mach-O each inner label is an .alt_entry, so the linker keeps the block in one piece. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"

#define FAR_FIRST 0x395B0u                      /* 395B:0000, linear */
#define FAR_END   0x55DC4u                      /* 54D7:105C, FD63's end */
#define FAR_SIZE  (FAR_END - FAR_FIRST)
#define DGROUP_SEG 0x5AACu                      /* DS, as an EXE paragraph */
#define FD72_SEG  0x5624u                       /* seg063 */
#define EXE_SIZE  547248L
#define EXE_CRC   0xF2BF2527u                   /* the GOG release's UW.EXE */

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
    AT(checkerboard1, 0x0)                      /* 395B:0000 */
    AT(seg048, 0x80)                            /* 3963:0000 */
    AT(smooth_div, 0x6CC)                       /* 3963:064C */
    AT(smooth_base, 0x6CE)                      /* 3963:064E */
    AT(smooth_lowpass, 0x6D0)                   /* 3963:0650 */
    AT(ShowClip, 0xE46)                         /* 3963:0DC6 */
    AT(Transparency, 0xE47)                     /* 3963:0DC7 */
    AT(stdat, 0x5F00)                           /* 3F4B:0000 */
    AT(ATM_Strings, 0x9236)                     /* 3F4B:3336 */
    AT(cmpbuf1_start, 0xAC80)                   /* 4423:0000 */
    AT(fade_buffer, 0xAC80)
    AT(seg051, 0xDC80)                          /* 4723:0000 */
    AT(seg051_28A0, 0x10520)                    /* 4723:28A0 */
    AT(_dblen, 0x18C74)                         /* 4723:AFF4 */
    AT(bmhgtoff, 0x18C80)                       /* 4723:B000 */
    AT(bmsegoff, 0x18C82)                       /* 4723:B002 */
    AT(first_anim, 0x18D70)                     /* 4723:B0F0 */
    AT(seg051_C10F, 0x19D8F)                    /* 4723:C10F */
    AT(CmaptoPg, 0x19E0F)                       /* 4723:C18F */
    AT(PgtoCmap, 0x19E8F)                       /* 4723:C20F */
    AT(CmapFrm, 0x19F0F)                        /* 4723:C28F */
    AT(CmapCache, 0x19F6F)                      /* 4723:C2EF */
    AT(crit_fpage, 0x19FEF)                     /* 4723:C36F */
    AT(crit_nlpages, 0x19FF1)                   /* 4723:C371 */
    AT(crit_inpage, 0x19FF3)                    /* 4723:C373 */
    AT(tmap_inpage, 0x19FF4)                    /* 4723:C374 */
    AT(seg051_C375, 0x19FF5)                    /* 4723:C375 */
    AT(seg051_C376, 0x19FF6)                    /* 4723:C376 */
    AT(seg051_C377, 0x19FF7)                    /* 4723:C377 */
    AT(seg051_C378, 0x19FF8)                    /* 4723:C378 */
    AT(seg051_C3B2, 0x1A032)                    /* 4723:C3B2 */
    AT(seg051_C3EC, 0x1A06C)                    /* 4723:C3EC */
    AT(seg051_C460, 0x1A0E0)                    /* 4723:C460 */
    AT(obj_inpage1, 0x1A154)                    /* 4723:C4D4 */
    AT(EmsBuff, 0x1A155)                        /* 4723:C4D5 */
    AT(seg051_C4D7, 0x1A157)                    /* 4723:C4D7, the EMS handle (mem/ems.c) */
    AT(seg_5DFD, 0x1A160)                       /* 5371:0000 */
    AT(seg053, 0x1B160)                         /* 5471:0000 */
    AT(sp_inf_tab, 0x1B1C8)                     /* 5477:0008 */
    AT(seg055, 0x1B7CC)                         /* 54D7:000C */
    ".org " HERE " + 0x1C820\n"
    ".p2align 4\n"
);

/* The code segments seg003 (the graphics library, 0090:0000) to seg004 (the renderer,
   06E7:FFFF) as one block, as DOS loaded them. The translated assembly modules keep data in
   their code segments and patch their own immediates, so the bytes are here and the code
   reads and writes them (x86/asmrt.h, asmgame.h). The names the C reaches are labels inside
   it: cXfer (SCALEBM.ASM's colour tables, 0090:1201). */
#define CODE_FIRST 0x00900u
#define CODE_SIZE  (0x06E70u - CODE_FIRST + 0x10010u)
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
    ATC(cXfer, 0x1201)                          /* 0090:1201 */
    ".org " HEREC " + 0x15580\n"
    ".p2align 4\n"
);
extern unsigned char port_code_block[];

extern unsigned char port_far_block[];
void port_dgroup_gaps(void);                    /* mem/dgroup.c */
unsigned char seg063[SEG063_SIZE];
unsigned char port_dgroup_image[0x10000];

/* The DOS segments inside the block, for the paragraph map: each from its paragraph to the
   next one's (FD62 and FD63 start at offsets 8 and 0Ch of theirs). */
static const struct { const char *name; unsigned seg; uint32_t lin, size; } segs[] = {
    { "FD51 checkerboard1", 0x395B, 0x395B0, 0x80 },
    { "FD52 seg048", 0x3963, 0x39630, 0x5E80 },
    { "FD53 stdat", 0x3F4B, 0x3F4B0, 0x4D80 },
    { "FD54 cmpbuf1_start", 0x4423, 0x44230, 0x3000 },
    { "FD58 seg051", 0x4723, 0x47230, 0xC4E0 },
    { "FD60 seg_5DFD", 0x5371, 0x53710, 0x1000 },
    { "FD61 seg053", 0x5471, 0x54710, 0x60 },
    { "FD62 sp_inf_tab", 0x5477, 0x54770, 0x600 },
    { "FD63 seg055", 0x54D7, 0x54D70, 0x1054 },
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
   program; those in the blocks copied above are segment parts of far pointers in the data, so
   they get the port's load segment, which the paragraph map gives the blocks. */
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
        else if (lin >= FD72_SEG * 16u && lin + 1 < FD72_SEG * 16u + SEG063_SIZE) w = seg063 + (lin - FD72_SEG * 16u);
        else if (lin >= DGROUP_SEG * 16u && lin + 1 < DGROUP_SEG * 16u + sizeof port_dgroup_image)
            w = port_dgroup_image + (lin - DGROUP_SEG * 16u);
        if (!w) continue;
        v = (unsigned)(w[0] | w[1] << 8) + PORT_LOAD_SEG;
        w[0] = (unsigned char)v;
        w[1] = (unsigned char)(v >> 8);
    }
}

/* Whether path is the GOG release's UW.EXE, without a message: 0 if it is, -1 if it cannot be
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
    if (!f) { fprintf(stderr, PORT_NAME ": cannot open %s\n", path); return -1; }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n != EXE_SIZE) {
        fprintf(stderr, PORT_NAME ": %s is %ld bytes, not the %ld of the GOG release's UW.EXE\n", path, n, EXE_SIZE);
        fclose(f);
        return -1;
    }
    exe = malloc((size_t)n);
    if (!exe || fread(exe, 1, (size_t)n, f) != (size_t)n) { fclose(f); free(exe); return -1; }
    fclose(f);
    if (crc32(exe, (size_t)n) != EXE_CRC) {
        fprintf(stderr, PORT_NAME ": %s is not the GOG release's UW.EXE (CRC-32 %08X)\n", path, crc32(exe, (size_t)n));
        free(exe);
        return -1;
    }
    hdr = (unsigned)(exe[8] | exe[9] << 8) * 16;
    memcpy(port_far_block, exe + hdr + FAR_FIRST, FAR_SIZE);
    memcpy(seg063, exe + hdr + FD72_SEG * 16, SEG063_SIZE);
    memcpy(port_code_block, exe + hdr + CODE_FIRST, CODE_SIZE);
    memcpy(port_dgroup_image, exe + hdr + DGROUP_SEG * 16, sizeof port_dgroup_image);
    relocate(exe);
    free(exe);
    port_dgroup_gaps();
    for (i = 0; i < sizeof segs / sizeof segs[0]; i++)
        pm_add(segs[i].name, port_far_block + (segs[i].lin - FAR_FIRST), segs[i].size, segs[i].seg + PORT_LOAD_SEG);
    pm_add("FD72 seg063", seg063, SEG063_SIZE, FD72_SEG + PORT_LOAD_SEG);
    pm_add("seg003-seg004 code", port_code_block, CODE_SIZE, (CODE_FIRST >> 4) + PORT_LOAD_SEG);
    return 0;
}
