/* frame.c: replaces nothing; the host memory under the emulated EMS page frame (mem/ems.c).
   A far pointer's offset wraps at 64 KB: in DOS a pointer that runs past E000:FFFF comes back
   to E000:0000 (CUTS.C walks a large page's records through the frame that way). A host
   pointer would run on past the end, so the frame's 64 KB are mapped twice in a row, and a
   pointer past the end reads and writes the start, as in DOS; a page after the second copy is
   left inaccessible, so a pointer that runs further faults at once (sys/crash.c reports where)
   instead of corrupting other data. Kept apart from ems.c because it needs the host's own
   memory-mapping calls, which the game's headers do not see. */
#undef _POSIX_C_SOURCE
#define _DARWIN_C_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

unsigned char *port_frame_alloc(void)
{
    char name[64];
    unsigned char *base;
    int fd;
    snprintf(name, sizeof name, "/uw2port-ems-%d", (int)getpid());
    fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd < 0) return NULL;
    shm_unlink(name);
    if (ftruncate(fd, 0x10000)) { close(fd); return NULL; }
    base = mmap(NULL, 0x24000, PROT_NONE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (base == MAP_FAILED) { close(fd); return NULL; }
    if (mmap(base, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0) == MAP_FAILED
        || mmap(base + 0x10000, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0) == MAP_FAILED) {
        close(fd);
        return NULL;
    }
    close(fd);
    return base;
}
