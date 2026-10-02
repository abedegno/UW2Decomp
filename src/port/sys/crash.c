/* crash.c: replaces nothing. A host fault (a bad pointer the port has not caught) prints the
   game thread's call stack before the program ends, so the port's first runs say where they
   stopped. */
#undef _POSIX_C_SOURCE
#define _DARWIN_C_SOURCE
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void on_fault(int sig)
{
    void *frames[64];
    int n = backtrace(frames, 64);
    static const char msg[] = "uw2port: host fault; the call stack:\n";
    if (write(2, msg, sizeof msg - 1) < 0) { }
    backtrace_symbols_fd(frames, n, 2);
    signal(sig, SIG_DFL);
    raise(sig);
}

void port_crash_handlers(void)
{
    signal(SIGSEGV, on_fault);
    signal(SIGBUS, on_fault);
    signal(SIGILL, on_fault);
}

/* The call stack, for port_halt when tracing. */
void port_backtrace(void)
{
    void *frames[32];
    int n = backtrace(frames, 32);
    backtrace_symbols_fd(frames, n, 2);
}
