/* entry.c: replaces nothing. How the game's C calls an assembly routine the port has as its
   translation (x86/asmrt.h) and that takes C arguments: as Turbo C's far call did, with the
   arguments pushed on the stack, right to left, and a far return address, on a stack that stands
   for DGROUP's (DS = SS = DGROUP in DOS). That stack is a block of the paragraph map of its own,
   so that the translation addresses it as a segment, at DGROUP's paragraph plus 800h. Below the
   arguments go eight zero bytes: a routine that reads words past the C's arguments reads the
   caller's frame in DOS. A far pointer argument is pushed as the segment and offset its DOS
   maker gave it where the paragraph map knows (port_fp_split_recent), else as the map's split.
   The registers are as they were before the call when it returns. */
#include "x86/asmrt.h"
#include "entry.h"

#define CSTACK_SEG (PORT_DGROUP_PARA + PORT_LOAD_SEG + 0x0800u)
#define CSTACK_SIZE 0x1000u

static uint8_t cstack[CSTACK_SIZE];
static int depth;

uint32_t port_far_entry(uint16_t seg, uint16_t off, const uint16_t *w, int n)
{
    static int registered;
    struct asm_state s;
    uint32_t r;
    uint16_t sp;
    int i;
    if (!registered) {
        pm_add("C stack (DGROUP's)", cstack, sizeof cstack, CSTACK_SEG);
        registered = 1;
    }
    sp = (uint16_t)(CSTACK_SIZE - 0x200u * (unsigned)depth);
    if (sp < 0x200) port_halt("C calls into translated code nested too deep");
    asm_save(&s);
    depth++;
    SET_SS(CSTACK_SEG);
    SET_DS(CSTACK_SEG);
    SET_ES(CSTACK_SEG);
    SP = sp;
    for (i = 0; i < 4; i++) push16(0);
    for (i = n - 1; i >= 0; i--) push16(w[i]);
    asm_run_far(seg, off);
    r = (uint32_t)DX << 16 | AX;
    depth--;
    asm_restore(&s);
    return r;
}

/* Near pointer arguments: the words at the bottom of the C stack's segment stand for DGROUP
   variables whose address the C passes (cSinCos's results); slot i is at offset 10h + 2i. */
uint16_t port_near_slot(int i)
{
    return (uint16_t)(0x10 + 2 * i);
}

int16_t port_near_get(int i)
{
    return (int16_t)(cstack[0x10 + 2 * i] | cstack[0x11 + 2 * i] << 8);
}

/* DGROUP's segment as translated code loads it (mov ax,DGROUP): the C stack's */
uint16_t port_dgroup_seg(void)
{
    return CSTACK_SEG;
}

void port_far_arg(const void *p, uint16_t *w)
{
    unsigned seg = 0, off = 0;
    if (p) port_fp_split_recent(p, &seg, &off);
    w[0] = (uint16_t)off;
    w[1] = (uint16_t)seg;
}

void *port_far_result(uint32_t r)
{
    return (r >> 16) || (r & 0xFFFF) ? port_mk_fp(r >> 16, r & 0xFFFF) : 0;
}
