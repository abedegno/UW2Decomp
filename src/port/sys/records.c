/* records.c: replaces nothing; FILE_RECORDS (src/include/portable.h) on the host. A struct the
   original lays over file data but which holds pointers has the host's layout, so the file's
   records are copied into host structs field by field: each field is aligned to its own size
   (8 for a pointer), as clang lays out a struct between HOST_LAYOUT_BEGIN and _END, and the
   pointer fields start null. Each conversion is remembered by its result, so that
   FILE_RECORDS_END can give the file's bytes after the records. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"

#define MAXCONV 16
static struct { void *host; unsigned char *file; int n; unsigned dos_size; } conv[MAXCONV];
static int nconv;

static unsigned field_size(char c, int host)
{
    switch (c) {
    case 'w': return 2;
    case 'n': return host ? 8 : 2;
    case 'f': return host ? 8 : 4;
    default: port_fatal("FILE_RECORDS: unknown field letter %c", c);
    }
}

void *port_file_records(void *p, int n, const char *layout, unsigned host_size)
{
    unsigned char *file = p, *out;
    unsigned dos_size = 0, off, hoff, sz;
    const char *c;
    int i, k;
    for (c = layout; *c; c++) dos_size += field_size(*c, 0);
    out = calloc((size_t)n, host_size);
    if (!out) port_fatal("FILE_RECORDS: out of memory");
    for (i = 0; i < n; i++) {
        off = 0;
        hoff = 0;
        for (c = layout; *c; c++) {
            sz = field_size(*c, 1);
            hoff = (hoff + sz - 1) & ~(sz - 1);
            if (*c == 'w') memcpy(out + (size_t)i * host_size + hoff, file + (size_t)i * dos_size + off, 2);
            off += field_size(*c, 0);
            hoff += sz;
        }
        if (((hoff + 7) & ~7u) != host_size) port_fatal("FILE_RECORDS: layout %s gives %u bytes, the struct %u", layout, (hoff + 7) & ~7u, host_size);
    }
    k = nconv < MAXCONV ? nconv++ : MAXCONV - 1;
    conv[k].host = out;
    conv[k].file = file;
    conv[k].n = n;
    conv[k].dos_size = dos_size;
    return out;
}

void *port_file_records_end(const void *q)
{
    int i;
    for (i = 0; i < nconv; i++)
        if (conv[i].host == q) return conv[i].file + (size_t)conv[i].n * conv[i].dos_size;
    port_fatal("FILE_RECORDS_END: not a FILE_RECORDS result");
}

/* FARNULLREC (portable.h): a null far pointer to a struct with the host's layout reads the
   vector table by the struct's DOS layout; one conversion per layout, kept, and logged as
   port_null_far's reads are. */
void *port_null_far_record(const char *layout, unsigned host_size, const char *file, int line)
{
    static struct { const char *layout; void *host; } done[8];
    unsigned char *port_null_copy(int far_table);
    int i;
    fprintf(stderr, "uw2port: far null pointer read at %s:%d\n", file, line);
    for (i = 0; i < 8 && done[i].layout; i++)
        if (!strcmp(done[i].layout, layout)) return done[i].host;
    if (i == 8) port_fatal("FARNULLREC: more than 8 layouts");
    done[i].layout = layout;
    done[i].host = port_file_records(port_null_copy(1), 1, layout, host_size);
    nconv--;                            /* not a FILE_RECORDS result */
    return done[i].host;
}
