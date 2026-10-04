/* modtab.c: replaces nothing. The translated modules (Exhume's tools/asm2c.py writes this file
   and them): the code segment, the range of offsets and the function of each. None yet. */
#include "x86/asmrt.h"

const struct asm_module asm_modules[1];
const int asm_nmodules = 0;

/* the ranges of the modules that are hand-written C (asm2c.py's HANDWRITTEN) */
const struct asm_hand asm_hand_ranges[1];
const int asm_nhand_ranges = 0;
/* the places in them the translated code calls or jumps to */
const struct asm_hand_call asm_hand_calls[1];
const int asm_nhand_calls = 0;
