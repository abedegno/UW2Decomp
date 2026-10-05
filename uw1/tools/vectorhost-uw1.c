/* vectorhost-uw1.c: the port's side of the test vectors (make vectors; Exhume's tools/vectors.py,
   [vectors] host_glue): each routine of tools/vector_targets.py run through the port's C on a
   case's inputs, so that vectors/ is known to be what the matched sources compute as well as
   what UW.EXE computes. Compiled as port C and linked into Exhume's tools/fuzzhost.c, which
   calls fuzz_vector with a kind it does not know.

   block holds the inputs as 32-bit little-endian words in the order of the target's input
   columns; the outputs go at block + 0x100 in the order of its output columns. A routine that
   calls rand() starts from the input seed (the generator's whole 32-bit state) and reports the
   state it leaves. Each kind sets only the globals its routine reads. */
#include <string.h>
#include "compat.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "port.h"

void port_set_rand_seed(uint32_t seed);
uint32_t port_rand_seed(void);

/* COMBAT.C's, not in a header */
int far pickloc(int dz, int dtop, int az, int atop);
void far compute_hitangle(void);
extern int16 hitobj;
extern int16 fromwho;
extern unsigned char hitangle;

static int32_t in(const unsigned char *b, int i)
{
    return (int32_t)((uint32_t)b[4 * i] | (uint32_t)b[4 * i + 1] << 8 | (uint32_t)b[4 * i + 2] << 16 | (uint32_t)b[4 * i + 3] << 24);
}

static void out(unsigned char *b, int i, int32_t v)
{
    unsigned char *p = b + 0x100 + 4 * i;
    p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); p[2] = (unsigned char)(v >> 16); p[3] = (unsigned char)(v >> 24);
}

int fuzz_vector(const char *kind, unsigned char *b)
{
    static struct Object objs[3];
    struct Object obj;

    if (!strcmp(kind, "v_skill_check")) {
        port_set_rand_seed((uint32_t)in(b, 2));
        out(b, 0, skill_check(in(b, 0), in(b, 1)));
        out(b, 1, (int32_t)port_rand_seed());
    } else if (!strcmp(kind, "v_rollem")) {
        port_set_rand_seed((uint32_t)in(b, 2));
        out(b, 0, rollem(in(b, 0), in(b, 1)));
        out(b, 1, (int32_t)port_rand_seed());
    } else if (!strcmp(kind, "v_pickloc")) {
        port_set_rand_seed((uint32_t)in(b, 4));
        out(b, 0, pickloc(in(b, 0), in(b, 1), in(b, 2), in(b, 3)));
        out(b, 1, (int32_t)port_rand_seed());
    } else if (!strcmp(kind, "v_move_along")) {
        int16 x = (int16)in(b, 2), y = (int16)in(b, 3);
        move_along(in(b, 0), in(b, 1), &x, &y);
        out(b, 0, x);
        out(b, 1, y);
    } else if (!strcmp(kind, "v_check_res")) {
        int item = in(b, 0);
        memset(&obj, 0, sizeof obj);
        obj.id = (uint16)item;
        ComObjData[item].resist = (unsigned char)in(b, 1);
        port_set_rand_seed((uint32_t)in(b, 4));
        out(b, 0, check_res(&obj, (unsigned char)in(b, 2), (unsigned char)in(b, 3)));
        out(b, 1, (int32_t)port_rand_seed());
    } else if (!strcmp(kind, "v_compute_hitangle")) {
        memset(objs, 0, sizeof objs);
        objs[1].id = 0x7F;
        objs[1].pos = (uint16)in(b, 1);
        objs[2].id = (uint16)in(b, 2);
        objs[2].pos = (uint16)in(b, 0);
        critdata = objs;
        hitobj = 2;
        fromwho = 1;
        compute_hitangle();
        out(b, 0, hitangle);
    } else if (!strcmp(kind, "v_sqrt")) {
        out(b, 0, (uint16)cSqRt(in(b, 0)));
    } else if (!strcmp(kind, "v_sincos")) {
        int16 x = 0, y = 0;
        cSinCos(in(b, 0), &x, &y);
        out(b, 0, x);
        out(b, 1, y);
    } else
        return 0;
    return 1;
}
