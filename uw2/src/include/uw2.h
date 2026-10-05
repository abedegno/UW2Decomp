/* uw2.h: what every header includes: union Link, the link word that chains objects into
   lists, and portable.h, the types and macros the DOS build and the port share. Each header declares the struct tags its declarations name and includes the
   headers that define them, so every tag a source sees is complete. */
#ifndef UW2_H
#define UW2_H

#include <stdio.h>
#include "portable.h"

/* A link word: an object's index in the top ten bits (0 for none, 1..0xFF a mobile object,
   0x100..0x3FF a static one) and six other bits below it. Object lists are chained through
   them: a tile's list head, each object's next link and its contents link. */
union Link {
    uint16 word;
    struct { uint16 low:6, index:10; } f;
};

#endif
