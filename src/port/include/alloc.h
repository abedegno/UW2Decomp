/* alloc.h for the port: Borland's far heap. The port allocates far blocks in its paragraph
   map, so that FP_SEG of a block and segment arithmetic on it keep working (docs/PORT.md). */
#ifndef UW2_PORT_ALLOC_H
#define UW2_PORT_ALLOC_H
#include <stdlib.h>
void *farmalloc(unsigned long n);
void *farcalloc(unsigned long count, unsigned long size);
void *farrealloc(void *block, unsigned long n);
void farfree(void *block);
unsigned long farcoreleft(void);
unsigned coreleft(void);
#endif
