/* io.h for the port: Borland's handle I/O. open, read, write, close, lseek and access are
   renamed to bc_ in compat.h; these are the names the host has no form of at all. */
#ifndef UW2_PORT_IO_H
#define UW2_PORT_IO_H
long filelength(int fd);
long tell(int fd);
int eof(int fd);
int setmode(int fd, int mode);
#endif
