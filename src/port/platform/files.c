/* files.c: replaces nothing in the DOS build. The port's file system (plat.h, "Files"): DOS
   paths onto the user's data root, read-only, with the port's home directory laid over it.

   The game names files as DOS does: relative to its current directory (the game's own
   directory), with backslashes, in any case ("DATA\\uw.cfg", "data", "SAVE0\\LEV.ARK",
   "CRIT\\CR01.00"), and it creates scratch files next to them (MISCUTIL.C's check_fds makes
   a.tmp to h.tmp) and keeps its game in progress in SAVE0. Each path is looked up one component
   at a time, case-insensitively, first under the home directory and then under the data root.
   A file the game creates goes to the home directory, and a file it opens to change is copied
   there first, so the data root is never written. Any POSIX host can use this file as it is. */
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>
#include "plat.h"

#define MAXP 1024

static char data_root[MAXP];
static char home_root[MAXP];

static int is_dir(const char *p)
{
    struct stat st;
    return stat(p, &st) == 0 && S_ISDIR(st.st_mode);
}

static int exists(const char *p)
{
    struct stat st;
    return stat(p, &st) == 0;
}

static int mkdirs(const char *p)
{
    char buf[MAXP];
    char *s;
    if (strlen(p) >= sizeof buf) return -1;
    strcpy(buf, p);
    for (s = buf + 1; *s; s++) {
        if (*s == '/') {
            *s = 0;
            if (!is_dir(buf) && mkdir(buf, 0755) != 0 && errno != EEXIST) return -1;
            *s = '/';
        }
    }
    if (!is_dir(buf) && mkdir(buf, 0755) != 0 && errno != EEXIST) return -1;
    return 0;
}

int plat_files_init(const char *data, const char *home)
{
    if (strlen(data) >= MAXP - 1 || strlen(home) >= MAXP - 1) return -1;
    strcpy(data_root, data);
    strcpy(home_root, home);
    while (strlen(data_root) > 1 && data_root[strlen(data_root) - 1] == '/') data_root[strlen(data_root) - 1] = 0;
    while (strlen(home_root) > 1 && home_root[strlen(home_root) - 1] == '/') home_root[strlen(home_root) - 1] = 0;
    if (!is_dir(data_root)) return -1;
    return mkdirs(home_root);
}

const char *plat_data_root(void) { return data_root; }
const char *plat_home(void) { return home_root; }

/* The name of entry `name` of directory dir as it is on disk, matched without case. */
static int find_ci(const char *dir, const char *name, char *out, size_t outsz)
{
    DIR *d;
    struct dirent *e;
    int found = 0;
    if (!*name) return 0;
    d = opendir(dir);
    if (!d) return 0;
    while ((e = readdir(d)) != NULL) {
        if (strcasecmp(e->d_name, name) == 0) {
            if (strlen(e->d_name) < outsz) { strcpy(out, e->d_name); found = 1; }
            if (strcmp(e->d_name, name) == 0) break;
        }
    }
    closedir(d);
    return found;
}

/* Splits a DOS path into components: drive letter dropped, both separators, "." dropped. */
static int split(const char *dospath, char comp[][64], int max)
{
    int n = 0, k = 0;
    const char *s = dospath;
    if (s[0] && s[1] == ':') s += 2;
    for (;; s++) {
        if (*s == '\\' || *s == '/' || *s == 0) {
            comp[n][k] = 0;
            if (k && strcmp(comp[n], ".") != 0) {
                if (strcmp(comp[n], "..") == 0) { if (n) n--; }
                else if (++n == max) return -1;
            }
            k = 0;
            if (!*s) break;
        } else if (k < 63) {
            comp[n][k++] = *s;
        }
    }
    return n;
}

/* Walks the components under base, matching case; 1 if every one exists. out gets the path
   as far as it could be matched, then the rest as given. */
static int walk(const char *base, char comp[][64], int n, char *out, size_t outsz)
{
    char name[256];
    int i, ok = 1;
    snprintf(out, outsz, "%s", base);
    for (i = 0; i < n; i++) {
        size_t l = strlen(out);
        if (ok && find_ci(out, comp[i], name, sizeof name))
            snprintf(out + l, outsz - l, "/%s", name);
        else {
            ok = 0;
            snprintf(out + l, outsz - l, "/%s", comp[i]);
        }
    }
    return ok;
}

static int copy_file(const char *from, const char *to)
{
    char buf[65536];
    ssize_t n;
    int in = open(from, O_RDONLY), out;
    if (in < 0) return -1;
    out = open(to, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) { close(in); return -1; }
    while ((n = read(in, buf, sizeof buf)) > 0)
        if (write(out, buf, (size_t)n) != n) { n = -1; break; }
    close(in);
    close(out);
    return n < 0 ? -1 : 0;
}

/* The home path for the components: the directories named as in whichever tree has them,
   created in the home directory; the file named as the home tree has it, else as given. */
static int home_path(char comp[][64], int n, char *out, size_t outsz, int makedirs)
{
    char name[256], dpath[MAXP];
    int i;
    snprintf(out, outsz, "%s", home_root);
    snprintf(dpath, sizeof dpath, "%s", data_root);
    for (i = 0; i < n; i++) {
        size_t l = strlen(out), dl = strlen(dpath);
        const char *use = comp[i];
        if (find_ci(out, comp[i], name, sizeof name)) use = name;
        else if (find_ci(dpath, comp[i], name, sizeof name)) use = name;
        snprintf(out + l, outsz - l, "/%s", use);
        snprintf(dpath + dl, sizeof dpath - dl, "/%s", use);
        if (makedirs && i < n - 1 && !is_dir(out) && mkdir(out, 0755) != 0 && errno != EEXIST) return -1;
    }
    return 0;
}

int plat_resolve(const char *dospath, int mode, char *out, size_t outsz)
{
    char comp[32][64], h[MAXP], d[MAXP];
    int n = split(dospath, comp, 32);
    if (n < 0) return -1;
    if (n == 0) { snprintf(out, outsz, "%s", mode == PLAT_READ && !exists(home_root) ? data_root : home_root); return 0; }
    if (mode == PLAT_READ) {
        if (walk(home_root, comp, n, h, sizeof h)) { snprintf(out, outsz, "%s", h); return 0; }
        if (walk(data_root, comp, n, d, sizeof d)) { snprintf(out, outsz, "%s", d); return 0; }
        return home_path(comp, n, out, outsz, 0);
    }
    if (walk(home_root, comp, n, h, sizeof h)) { snprintf(out, outsz, "%s", h); return 0; }
    if (home_path(comp, n, out, outsz, 1)) return -1;
    if (mode == PLAT_WRITE && walk(data_root, comp, n, d, sizeof d) && !is_dir(d))
        copy_file(d, out);
    return 0;
}

struct seen { char **names; int n, cap; };

static int seen_add(struct seen *s, const char *name)
{
    int i;
    for (i = 0; i < s->n; i++) if (strcasecmp(s->names[i], name) == 0) return 0;
    if (s->n == s->cap) {
        s->cap = s->cap ? s->cap * 2 : 64;
        s->names = realloc(s->names, sizeof *s->names * (size_t)s->cap);
    }
    s->names[s->n++] = strdup(name);
    return 1;
}

static void list_one(const char *dir, struct seen *s,
                     void (*fn)(const char *, int, long, long, void *), void *ctx)
{
    DIR *dd = opendir(dir);
    struct dirent *e;
    char p[MAXP];
    struct stat st;
    if (!dd) return;
    while ((e = readdir(dd)) != NULL) {
        if (e->d_name[0] == '.' && (e->d_name[1] == 0 || (e->d_name[1] == '.' && e->d_name[2] == 0)))
            continue;
        snprintf(p, sizeof p, "%s/%s", dir, e->d_name);
        if (stat(p, &st) != 0) continue;
        if (seen_add(s, e->d_name))
            fn(e->d_name, S_ISDIR(st.st_mode), (long)st.st_size, (long)st.st_mtime, ctx);
    }
    closedir(dd);
}

int plat_listdir(const char *dosdir, void (*fn)(const char *, int, long, long, void *), void *ctx)
{
    char comp[32][64], h[MAXP], d[MAXP];
    struct seen s = { 0, 0, 0 };
    int n = split(dosdir, comp, 32), i, any = 0;
    if (n < 0) return -1;
    if (walk(home_root, comp, n, h, sizeof h) && is_dir(h)) { list_one(h, &s, fn, ctx); any = 1; }
    if (walk(data_root, comp, n, d, sizeof d) && is_dir(d)) { list_one(d, &s, fn, ctx); any = 1; }
    for (i = 0; i < s.n; i++) free(s.names[i]);
    free(s.names);
    return any ? 0 : -1;
}
