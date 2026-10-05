/* int0trap.c: replaces src/sys/INT0TRAP.ASM (seg018), the divide-by-zero trap, and DOS's own
   int 0 handler behind it. In DOS a divide by zero raised int 0; init_world (UWEDIT.C) points
   the vector at int0_trap, which switches to the game's start-up stack (int0_ss:int0_sp) and
   calls div_zero_ovr112_661, which shuts the game down and chains to the vector it found,
   DOS's, which prints "Divide overflow" and ends the program.

   On the host: x86-64 raises SIGFPE for an integer divide by zero, and the handler here runs
   whatever int 0 points at. arm64 does not trap at all (SDIV and UDIV give 0), so on Apple
   Silicon a divide by zero goes on with 0, where DOS would have stopped; the promotion audit's
   divide sites (docs/PORT.md) are where to look if the replay shows one. */
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "port.h"

uint16_t int0_ss, int0_sp;

void div_zero_ovr112_661(void);
typedef void (*isr_fn)(void);
isr_fn port_vector(int n);
void bc_exit(int status);

void int0_trap(void)
{
    div_zero_ovr112_661();
}

/* DOS's int 0 handler: the message, and the program ends with errorlevel 255... as DOS's
   terminate does after "Divide overflow". */
void port_dos_int0(void)
{
    static const char msg[] = "\r\nDivide overflow\r\n";
    fwrite(msg, 1, sizeof msg - 1, stderr);
    bc_exit(0xFF);
}

static void on_fpe(int sig)
{
    isr_fn f = port_vector(0);
    (void)sig;
    if (f) f();
    port_dos_int0();
}

void int0_install(void)
{
    signal(SIGFPE, on_fpe);
}
