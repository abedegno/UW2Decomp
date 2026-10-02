/* stub.c: replaces nothing. The body of every generated link stub (tools/portstubs.py): the
   port links with these in place of the assembly modules, Borland's library, the AIL API,
   the platform layer and the data taken from UW2.EXE, and stops at the first one it calls. */
#include <stdio.h>
#include <stdlib.h>
#include "stub.h"

void port_stub(const char *name)
{
    fprintf(stderr, "uw2port: %s is a stub (src/port/stubs); stopping\n", name);
    abort();
}
