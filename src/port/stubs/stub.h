/* stub.h: what the generated link stubs (tools/portstubs.py) share. Not a game header: the
   stub files include no game header, so a stub's type never has to match the game's
   declaration of the name it stands in for. */
#ifndef UW2_PORT_STUB_H
#define UW2_PORT_STUB_H

/* Reports that a stubbed function was called and aborts. */
void port_stub(const char *name);

/* Stubbed variables are zeroed byte arrays, aligned for any type they stand in for. */
#define STUB_ALIGN __attribute__((aligned(16)))

#endif
