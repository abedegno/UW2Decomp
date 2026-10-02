/* stub.c: replaces nothing. The body of every generated link stub (tools/portstubs.py): the
   port links with these in place of the assembly modules, Borland's library, the AIL API and
   the data taken from UW2.EXE that it does not replace yet, and stops at the first one it
   calls. The game's thread parks there and the window stays up, showing the last screen. */
#include <stdio.h>
#include "stub.h"

void port_halt(const char *why) __attribute__((noreturn));

void port_stub(const char *name)
{
    char why[160];
    snprintf(why, sizeof why, "%s is a stub (src/port/stubs)", name);
    port_halt(why);
}
