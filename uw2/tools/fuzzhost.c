/* fuzzhost.c: the port's side of tools/fuzzasm.py, the differential fuzzing of single routines
   (docs/BUILDING.md, "Testing"). Not part of the port: tools/fuzzasm.py compiles it and links it
   with the port's objects (build/port, or build/port-cov for the coverage report) in place of
   src/port/sys/main.c, so the routines it calls are the port's own C and translations.

   It loads the far data from the user's UW2.EXE as the port does (port_load_exe), adds two
   scratch segments to the paragraph map (E000, data, and F000, the stack), and then serves
   cases on its standard input, one at a time. Before each case every region is put back as it
   was after loading. A case is text lines:

     R eax ebx ecx edx esi edi ebp esp ds es ss fs gs flags   the registers (hex)
     M linear hexbytes                                       memory to set first (any number)
     C kind seg off retcs retip                              the call (kind below)

   and the answer is the registers in the same order after the call, `S status message` (0 the
   routine returned, 1 the port stopped: port_halt, as a divide the port does not model), the
   memory that changed (`D linear hexbytes`, runs of changed bytes) and `E`. The kinds:

     near, far     a near or far call of seg:off (an EXE paragraph and offset, as ASM_JMP takes
                   them) through x86/asmrt.c: the translated code there, or the hand-written C
                   its glue sends the call to
     cfst          cFstSinCos(BX): AX, BX = the two results (sys/imath.c)
     csqrt         cSqRt(CX:BX): DI = the result
     csincos       cSinCos(BX): AX, BX
     catan2        cAtan2(AX, BX): CX
     uncmp         seg004_uncmp(BX format, AX image, BP size offset, DS:SI palette, DH row):
                   AX = the result (3d/expand.c, cFrmtoRaw's decoder table) */
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include "x86/asmrt.h"

#define FAR_FIRST 0x37050u
#define FAR_END 0x6061Cu
#define CODE_FIRST 0x00850u
#define CODE_SIZE (0x065C0u - CODE_FIRST + 0x10010u)
#define FD71_SEG 0x60B9u
#define SCRATCH_SEG 0xE000u
#define STACK_SEG 0xF000u
#define LOAD (PORT_LOAD_SEG * 16u)

extern unsigned char port_far_block[];
extern unsigned char port_code_block[];
extern int asm_level;
extern int16_t rp_request;
void cFstSinCos(int angle, int16_t *a, int16_t *b);
int cSqRt(int32_t v);
void cSinCos(int angle, int16_t *x, int16_t *y);
int cAtan2(int x, int y);
uint16_t seg004_uncmp(uint16_t bx, uint16_t ax, uint16_t bp, const uint8_t *pal, uint8_t dh);

static unsigned char scratch[0x10010], stack[0x10010];
static struct { const char *name; unsigned char *p; uint32_t lin, size; unsigned char *saved; } R[] = {
    { "far data", port_far_block, FAR_FIRST + LOAD, FAR_END - FAR_FIRST, 0 },
    { "seg021 data", dseg062_62a6, FD71_SEG * 16u + LOAD, DSEG062_62A6_SIZE, 0 },
    { "seg003-seg004 code", port_code_block, CODE_FIRST + LOAD, CODE_SIZE, 0 },
    { "scratch E000", scratch, SCRATCH_SEG * 16u, 0x10000, 0 },
    { "stack F000", stack, STACK_SEG * 16u, 0x10000, 0 },
};
#define NR ((int)(sizeof R / sizeof R[0]))

/* what src/port/sys/main.c gives the rest of the port */
int port_trace;
void port_log(const char *fmt, ...) { (void)fmt; }
void port_fatal(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    exit(1);
}
static jmp_buf halted;
static char halt_why[256];
void port_halt(const char *why)
{
    snprintf(halt_why, sizeof halt_why, "%s", why);
    longjmp(halted, 1);
}
void port_on_flip(void) {}

static unsigned char *at(uint32_t lin)
{
    int i;
    for (i = 0; i < NR; i++)
        if (lin >= R[i].lin && lin < R[i].lin + R[i].size) return R[i].p + (lin - R[i].lin);
    return NULL;
}

static int hexval(int c) { return c <= '9' ? c - '0' : (c | 32) - 'a' + 10; }

int main(int argc, char **argv)
{
    static char line[1 << 21];
    uint32_t r[14];
    int i;
    if (argc < 2) { fprintf(stderr, "usage: fuzzhost UW2.EXE\n"); return 2; }
    if (port_load_exe(argv[1])) return 1;
    pm_add("fuzz scratch", scratch, 0x10000, SCRATCH_SEG);
    pm_add("fuzz stack", stack, 0x10000, STACK_SEG);
    for (i = 0; i < NR; i++) {
        R[i].saved = malloc(R[i].size);
        memcpy(R[i].saved, R[i].p, R[i].size);
    }
    printf("READY\n");
    fflush(stdout);
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'R') {
            for (i = 0; i < NR; i++) memcpy(R[i].p, R[i].saved, R[i].size);
            sscanf(line + 1, "%x %x %x %x %x %x %x %x %x %x %x %x %x %x", &r[0], &r[1], &r[2], &r[3], &r[4],
                   &r[5], &r[6], &r[7], &r[8], &r[9], &r[10], &r[11], &r[12], &r[13]);
        } else if (line[0] == 'M') {
            uint32_t lin = (uint32_t)strtoul(line + 2, NULL, 16);
            char *h = strchr(line + 2, ' ');
            for (h = h ? h + 1 : line + strlen(line); h[0] && h[1] && h[0] != '\n'; h += 2, lin++) {
                unsigned char *p = at(lin);
                if (p) *p = (unsigned char)(hexval(h[0]) << 4 | hexval(h[1]));
            }
        } else if (line[0] == 'C') {
            char kind[16];
            unsigned seg, off, rcs, rip;
            int status = 0;
            uint32_t c = 0;
            sscanf(line + 2, "%15s %x %x %x %x", kind, &seg, &off, &rcs, &rip);
            EAX = r[0]; EBX = r[1]; ECX = r[2]; EDX = r[3]; ESI = r[4]; EDI = r[5]; EBP = r[6]; ESP = r[7];
            SET_DS(r[8]); SET_ES(r[9]); SET_SS(r[10]); SET_FS(r[11]); SET_GS(r[12]);
            asm_set_flags((uint16_t)r[13]);
            asm_level = 0;
            halt_why[0] = 0;
            if (setjmp(halted)) status = 1;
            else if (!strcmp(kind, "near")) c = asm_call(ASM_JMP(seg, off), (uint16_t)rip);
            else if (!strcmp(kind, "far")) c = asm_callf(ASM_JMP(seg, off), (uint16_t)rcs, (uint16_t)rip);
            else if (!strcmp(kind, "cfst")) {
                int16_t a, b;
                cFstSinCos(BX, &a, &b);
                AX = (uint16_t)a; BX = (uint16_t)b;
            } else if (!strcmp(kind, "csqrt")) DI = (uint16_t)cSqRt((int32_t)((uint32_t)CX << 16 | BX));
            else if (!strcmp(kind, "csincos")) {
                int16_t a, b;
                cSinCos(BX, &a, &b);
                AX = (uint16_t)a; BX = (uint16_t)b;
            } else if (!strcmp(kind, "catan2")) CX = (uint16_t)cAtan2((int16_t)AX, (int16_t)BX);
            else if (!strcmp(kind, "uncmp")) AX = seg004_uncmp(BX, AX, BP, pDS + SI, DH);
            else { status = 1; snprintf(halt_why, sizeof halt_why, "unknown kind %s", kind); }
            if (!status && c) { status = 1; snprintf(halt_why, sizeof halt_why, "returned past the call (%08X)", c); }
            printf("R %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n", EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP,
                   asm_ds, asm_es, asm_ss, asm_fs, asm_gs, asm_flags());
            printf("S %d %s\n", status, halt_why);
            for (i = 0; i < NR; i++) {
                uint32_t k = 0;
                while (k < R[i].size) {
                    uint32_t e;
                    if (R[i].p[k] == R[i].saved[k]) { k++; continue; }
                    for (e = k; e < R[i].size && R[i].p[e] != R[i].saved[e]; e++) ;
                    printf("D %x ", R[i].lin + k);
                    for (; k < e; k++) printf("%02x", R[i].p[k]);
                    putchar('\n');
                }
            }
            printf("E\n");
            fflush(stdout);
        }
    }
    return 0;
}
