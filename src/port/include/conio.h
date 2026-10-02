/* conio.h for the port: Borland's console I/O. Not included by any game source today; here so
   that a source that adds it compiles in both builds. */
#ifndef UW2_PORT_CONIO_H
#define UW2_PORT_CONIO_H
int kbhit(void);
int getch(void);
int cprintf(const char *fmt, ...);
void clrscr(void);
#endif
