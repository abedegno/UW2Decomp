/* target: ovr110 */
/* opts: -mm -1 -G -O -Y -d */
/* Error reporting and fatal exits: the whole of UW1's DOS overlay ovr110, in original
   order. UW1 has no symbol-bearing build; the names are UW2's (the FM Towns symbol table;
   UW2's ERROR.C, overlay ovr114), the code being the same routines. There are two paths.
   Before the game is running (start-up checks, out of memory or EMS), first_punt prints
   the reason and a code straight to the console with DOS function 9, frees the memory
   TMPALLOC.C took, and exits. Once it is running, pfatal_code and pfatal do not print;
   they copy the message into cExitMessage, point cPerror at it, and exit through
   free_world, and the exit routine prints the message cPerror points at. Error codes are
   the ERR_ kinds of sys.h plus a number; error_code shows them as a letter ('A' + kind)
   and three octal digits, so ERR_EMS | 3 is "C003".

   UW1's difference: first_punt frees only the memory (free_mem), not the timers and
   sounds.

   name: descriptive (error reporting). error_code is the target table's ovr110_0: UW2's
   name at the same position (FM Towns' error_code). */

#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include "sound.h"
#include "sys.h"
#include "gfx.h"

/* Prints "Cannot run Underworld.", the reason for the code's kind, and the code. The
   strings end in '$' for DOS function 9. */
void far error_code(int code)
{
    char error_code[0x28];

    seg015_1F9B_E8("Cannot run Underworld.\r\n$");
    switch ((code & 0xF000) >> 12) {
    case 1:
        seg015_1F9B_E8("Out of Low Memory.$");
        break;
    case 2:
        seg015_1F9B_E8("Out of EMS Memory.$");
        break;
    case 3:
        seg015_1F9B_E8("Could not read data.$");
        break;
    case 4:
        seg015_1F9B_E8("Could not write data.$");
        break;
    default:
        seg015_1F9B_E8("Resource problem or internal error.$");
        break;
    }
    strcpy(error_code, " Error code XXXX\r\n$");
    error_code[0x0C] = ((code & 0xF000) >> 12) + 'A';
    code = code & 0x0FFF;
    error_code[0x0D] = ((code >> 6) & 7) + '0';
    error_code[0x0E] = ((code >> 3) & 7) + '0';
    error_code[0x0F] = (code & 7) + '0';
    seg015_1F9B_E8((char far *)error_code);
}

void far first_punt(int code)
{
    error_code(code);
    free_mem();
    exit(-1);
}

/* Fatal error once the game is running: the message "Underworld can no longer run.
   Error code XXXX." goes into cExitMessage for the exit routine to print, then the world
   is freed and the program exits with -24. The message overwrites the start of the
   buffer, whose initial contents (in UW2) are 54 spaces and "Have Fun$"; nothing in the
   sources points cPerror at the buffer without first copying a message into it, so that text is
   probably never shown. */
void far pfatal_code(int code)
{
    char message[0x50];

    strcpy(message,
                                           "Underworld can no longer run.  Error code XXXX.\r\n$");
    message[0x2A] = ((code & 0xF000) >> 12) + 'A';
    code = code & 0x0FFF;
    message[0x2B] = ((code >> 6) & 7) + '0';
    message[0x2C] = ((code >> 3) & 7) + '0';
    message[0x2D] = (code & 7) + '0';
    *cPerror = FP_OFF(cExitMessage);
    FAR_COPY(cExitMessage, (char far *)message, strlen(message));
    free_world(0);
    exit(-24);
}

/* As pfatal_code, with a message of the caller's (which must end in '$'). */
void far pfatal(char *message)
{
    register char *s = message;
    *cPerror = FP_OFF(cExitMessage);
    FAR_COPY(cExitMessage, s, strlen(s));
    free_world(0);
    exit(-24);
}
