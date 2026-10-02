/* mem.h for the port: Borland's <mem.h> is the memory half of <string.h>, plus movedata. */
#ifndef UW2_PORT_MEM_H
#define UW2_PORT_MEM_H
#include <string.h>
#include <dos.h>
void movmem(const void *src, void *dst, unsigned n);
void setmem(void *dst, unsigned n, char value);
#endif
