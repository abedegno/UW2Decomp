/* entry.h: the game's C calling translated assembly routines that take C arguments (x86/entry.c). */
#ifndef UW1_ENTRY_H
#define UW1_ENTRY_H
#include <stdint.h>
/* seg:off the routine's far entry (the EXE's paragraph, as modtab.c lists the modules), w[0] the
   first argument word; DX:AX on return */
uint32_t port_far_entry(uint16_t seg, uint16_t off, const uint16_t *w, int n);
/* a far pointer argument as two words, offset then segment */
void port_far_arg(const void *p, uint16_t *w);
/* a far pointer result in DX:AX as a host pointer (0:0 is a null pointer) */
void *port_far_result(uint32_t r);
/* a near pointer argument the routine writes a word through: slot i's offset, and the word */
uint16_t port_near_slot(int i);
int16_t port_near_get(int i);
#define W16(v) ((uint16_t)(int16_t)(v))
#endif
