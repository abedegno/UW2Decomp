/* frame.c: replaces nothing; the host memory under the emulated EMS page frame (mem/ems.c).
   A far pointer's offset wraps at 64 KB: in DOS a pointer that runs past E000:FFFF comes back
   to E000:0000 (CUTS.C walks a large page's records through the frame that way). A host
   pointer would run on past the end, so the frame's 64 KB are mapped twice in a row, and a
   pointer past the end reads and writes the start, as in DOS; a page after the second copy is
   left inaccessible, so a pointer that runs further faults at once (sys/crash.c reports where)
   instead of corrupting other data. Kept apart from ems.c because it needs the host's own
   memory-mapping calls, which the game's headers do not see. */
#ifdef _WIN32
/* Windows: one section mapped at two adjacent addresses. The 144 KB are reserved to find a
   free range, then released and mapped there (allocation granularity is 64 KB, so both views
   and the guard page start on its boundary); another thread can take the range in between,
   so it is tried a few times. The guard is left reserved and inaccessible. */
#include <windows.h>

unsigned char *port_frame_alloc(void)
{
    HANDLE section = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 0x10000, NULL);
    unsigned char *result = NULL;
    int attempt;
    if (!section) return NULL;
    for (attempt = 0; attempt < 16 && !result; attempt++) {
        unsigned char *base = VirtualAlloc(NULL, 0x30000, MEM_RESERVE, PAGE_NOACCESS);
        void *a, *b;
        if (!base) break;
        VirtualFree(base, 0, MEM_RELEASE);
        a = MapViewOfFileEx(section, FILE_MAP_WRITE, 0, 0, 0x10000, base);
        b = a ? MapViewOfFileEx(section, FILE_MAP_WRITE, 0, 0, 0x10000, base + 0x10000) : NULL;
        if (a && b) {
            VirtualAlloc(base + 0x20000, 0x10000, MEM_RESERVE, PAGE_NOACCESS);
            result = base;
        } else {
            if (a) UnmapViewOfFile(a);
            if (b) UnmapViewOfFile(b);
        }
    }
    CloseHandle(section);   /* the views keep the section alive */
    return result;
}
#else
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
#endif
