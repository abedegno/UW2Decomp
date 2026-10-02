/* target: ovr114 */
/* opts: -mm -1 -G -O -Y -d */
/* Error reporting and fatal exits: the whole of DOS overlay ovr114. There are two paths.
   Before the game is running (start-up checks, out of memory or EMS), first_punt prints
   the reason and a code straight to the console with DOS function 9, frees EMS, the
   timers and the sounds, and exits. Once it is running, pfatal_code and pfatal do not
   print (probably because the screen is in a graphics mode by then); they copy the
   message into cExitMessage, the buffer in seg021's data segment (SYSENTRY.ASM), point
   cPerror at it, and exit through free_world (UWEDIT.C), whose grfx_close (GRFX.C) shuts
   the system modules down. When DOS terminates the program it jumps to the exit routine
   init installed in the PSP (SYSINIT.ASM), which prints the message cPerror points at.
   Error codes are the ERR_ kinds of sys.h plus a number; error_code shows them as a
   letter ('A' + kind) and three octal digits, so ERR_EMS | 3 is "C003".
   name: descriptive (error reporting). FM Towns' error_code and pfatal occupy the same
   positions and do the same work as the DOS IDA-named ErrorHandler_ovr114_0 and
   ExitWithError_ovr114_15D. */

#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include "sound.h"
#include "sys.h"


/* Prints "Cannot run Underworld.", the reason for the code's kind, and the code. The
   strings end in '$' for DOS function 9 (PrintStringToConsole, MODEX.ASM). */
void far error_code(int code)
{
    char error_code[0x28];

    PrintStringToConsole_seg017_DE("Cannot run Underworld.\r\n$");
    switch ((code & 0xF000) >> 12) {
    case 1:
        PrintStringToConsole_seg017_DE("Out of Low Memory.$");
        break;
    case 2:
        PrintStringToConsole_seg017_DE("Out of EMS Memory.$");
        break;
    case 3:
        PrintStringToConsole_seg017_DE("Could not read data.$");
        break;
    case 4:
        PrintStringToConsole_seg017_DE("Could not write data.$");
        break;
    default:
        PrintStringToConsole_seg017_DE("Resource problem or internal error.$");
        break;
    }
    strcpy(error_code, " Error code XXXX\r\n$");
    error_code[0x0C] = ((code & 0xF000) >> 12) + 'A';
    code = code & 0x0FFF;
    error_code[0x0D] = ((code >> 6) & 7) + '0';
    error_code[0x0E] = ((code >> 3) & 7) + '0';
    error_code[0x0F] = (code & 7) + '0';
    PrintStringToConsole_seg017_DE((char far *)error_code);
}

void far first_punt(int code)
{
    error_code(code);
    /* match: original order and call signatures from the DOS routine. */
    free_mem();
    free_timers();
    free_sounds();
    exit(-1);
}

/* Fatal error once the game is running: the message "Underworld can no longer run.
   Error code XXXX." goes into cExitMessage for the exit routine to print, then the world
   is freed and the program exits with -24. The message overwrites the start of the
   buffer, whose initial contents are 54 spaces and "Have Fun$"; nothing in the sources
   points cPerror at the buffer without first copying a message into it, so that text is
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
    *cPerror = (int)cExitMessage;
    movedata(FP_SEG((char far *)message), FP_OFF((char far *)message),
        FP_SEG(cExitMessage), FP_OFF(cExitMessage), strlen(message));
    free_world(0);
    exit(-24);
}

/* As pfatal_code, with a message of the caller's (which must end in '$'). */
void far pfatal(char *message)
{
    register char *s = message;
    *cPerror = (int)cExitMessage;
    movedata(FP_SEG(s), FP_OFF(s), FP_SEG(cExitMessage),
                                     FP_OFF(cExitMessage), strlen(s));
    free_world(0);
    exit(-24);
}
