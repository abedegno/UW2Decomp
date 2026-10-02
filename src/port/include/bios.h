/* bios.h for the port: Borland's BIOS calls. Not included by any game source today. */
#ifndef UW2_PORT_BIOS_H
#define UW2_PORT_BIOS_H
int bioskey(int cmd);
long biostime(int cmd, long newtime);
#endif
