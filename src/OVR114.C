/* target: ovr114 */
/* opts: -mm -1 -G -O -Y -d */
/* Error reporting and fatal exit routines. FM Towns' error_code and pfatal
   occupy the same positions and perform the same work as the DOS IDA-named
   ErrorHandler_ovr114_0 and ExitWithError_ovr114_15D. */

#include <dos.h>
#include <string.h>

extern int far *cPerror;
extern char far *cExitMessage;

void far PrintStringToConsole_seg017_DE(char far *text);
void far exit(int code);
void far free_mem(void);
void far free_timers(void);
void far free_sounds(void);
void far free_timers(void);
void far free_sounds(void);
void far stub112_25(int code);
void far free_world(char flag);

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
    /* Original order and call signatures from the DOS routine. */
    free_mem();
    free_timers();
    free_sounds();
    exit(-1);
}

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

void far pfatal(char *message)
{
    register char *s = message;
    *cPerror = (int)cExitMessage;
    movedata(FP_SEG(s), FP_OFF(s), FP_SEG(cExitMessage),
                                     FP_OFF(cExitMessage), strlen(s));
    free_world(0);
    exit(-24);
}
