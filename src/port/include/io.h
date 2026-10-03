/* io.h for the port: Borland's handle I/O. open, read, write, close, lseek and access are
   renamed to bc_ in compat.h; these are the names the host has no form of at all. */
#ifndef UW2_PORT_IO_H
#define UW2_PORT_IO_H
#ifdef _WIN32
/* MinGW has an io.h of its own, which its dirent.h, fcntl.h and unistd.h include and which
   declares open, read and the rest: this file stands in front of it, so it brings it in. */
#include_next <io.h>
#endif
long filelength(int fd);
long tell(int fd);
int eof(int fd);
int setmode(int fd, int mode);
#endif
