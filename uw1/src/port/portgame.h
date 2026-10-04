/* portgame.h: UW1's side of Exhume's runtime (Exhume's docs/port.md, "The runtime"; its
   runtime/README.md, "The project's side"). The runtime's port.h, compat.h, plat.h and its sound
   and system C include it, ahead of their own defaults; compat.h brings it into the game's C too,
   so it declares only the port's names.

   The program's name in its messages, and the game's main it runs; DGROUP's paragraph in UW.EXE,
   which the pseudo-registers _DS and _SS start with, and where Borland's _ctype table sits in
   DGROUP (DS:1E00, after the C library's ATEXIT word at 1DFE: build/LINK/out/UW.MAP, and the
   bytes); the termination chain exit runs, which seg019's init hooked. */
#ifndef UW1_PORTGAME_H
#define UW1_PORTGAME_H

#define PORT_NAME "uw1port"
#define PORT_GAME_MAIN uw1_main                 /* UWEDIT.C's main (compat.h) */
#define PORT_DGROUP_PARA 0x5AACu
#define PORT_CTYPE_AT 0x1E00
void seg019_exit_chain(void);                   /* sys/sysentry.c */
#define PORT_EXIT_CHAIN() seg019_exit_chain()

/* Names of UW1's that the host's C library also declares, even with only POSIX 2008's names
   visible: renamed in the port, after compat.h has included the host's headers.
   dprintf is DEBUG.C's (ovr106_1B, UW1's debug print); POSIX's writes to a descriptor. */
#define dprintf uw1_dprintf

#endif
