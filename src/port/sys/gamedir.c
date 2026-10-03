/* gamedir.c: replaces nothing in the DOS build. Finds the user's copy of UW2 when --data does
   not name it, and keeps the port's settings file (docs/BUILDING.md, "Running the port").

   A game directory is one that holds the GOG release's UW2.EXE (checked by size and CRC-32,
   port_check_exe). The search tries, in order: $UW2PORT_DATA; the folder remembered in the
   settings file; the current directory and the executable's directory (and a UW2 folder in
   either); on Windows, the folders GOG's installers record in the registry; then GOG's install
   locations on each host, looking up to four levels into any folder there whose name has
   "Underworld" or "UW2" in it.

   GOG's Mac and Windows releases keep the game inside a CD image, game.gog (an ISO 9660 image
   of the Ultima Underworld 1 and 2 CD, which DOSBox mounts as D:), with UW2.EXE and its data
   in its UW2 directory. Where the search finds only such an image, the UW2 directory is copied
   out of it once into gog-cd/UW2 in the port's home directory, and that is the game
   directory. The image is read, never changed.

   The settings file is uw2port.cfg in the home directory: lines of key=value ("data", the
   game directory; "mt32-roms", the MT-32 ROM directory). */
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#endif
#include "port.h"
#include "plat.h"

#define MAXP 1024

static int is_dir(const char *p)
{
    struct stat st;
    return stat(p, &st) == 0 && S_ISDIR(st.st_mode);
}

static int mkdirs(const char *p)
{
    char buf[MAXP];
    char *s;
    if (strlen(p) >= sizeof buf) return -1;
    strcpy(buf, p);
    for (s = buf + 1; *s; s++) {
        if (*s == '/' || *s == '\\') {
            char c = *s;
            *s = 0;
            if (!is_dir(buf) && mkdir(buf, 0755) != 0 && errno != EEXIST && !(s == buf + 2 && buf[1] == ':')) return -1;
            *s = c;
        }
    }
    if (!is_dir(buf) && mkdir(buf, 0755) != 0 && errno != EEXIST) return -1;
    return 0;
}

/* dir/name with name matched without case: 1 and the path in out, else 0. */
static int find_ci(const char *dir, const char *name, char *out, size_t outsz)
{
    DIR *d = opendir(dir);
    struct dirent *e;
    int found = 0;
    if (!d) return 0;
    while ((e = readdir(d)) != NULL)
        if (strcasecmp(e->d_name, name) == 0) {
            found = snprintf(out, outsz, "%s/%s", dir, e->d_name) < (int)outsz;
            break;
        }
    closedir(d);
    return found;
}

/* The last game directory that had a UW2.EXE the check refused, for the message. */
static char refused[MAXP];
static int refused_why;

/* 0 if dir holds the GOG release's UW2.EXE. */
static int game_dir_ok(const char *dir)
{
    char exe[MAXP];
    int r;
    if (!find_ci(dir, "UW2.EXE", exe, sizeof exe)) return -1;
    r = port_check_exe(exe);
    if (r && r != -1) { snprintf(refused, sizeof refused, "%s", exe); refused_why = r; }
    return r;
}

/* ISO 9660, as much of it as GOG's CD image needs: the primary volume descriptor, directory
   records and single-extent files. Names are compared without the ";1" version and case. */
static uint32_t le32(const unsigned char *p) { return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24; }

static int iso_sector(FILE *f, uint32_t lba, unsigned char *buf)
{
    return fseek(f, (long)lba * 2048L, SEEK_SET) == 0 && fread(buf, 1, 2048, f) == 2048 ? 0 : -1;
}

static int iso_root(FILE *f, uint32_t *ext, uint32_t *size)
{
    unsigned char s[2048];
    if (iso_sector(f, 16, s) || s[0] != 1 || memcmp(s + 1, "CD001", 5)) return -1;
    *ext = le32(s + 156 + 2);
    *size = le32(s + 156 + 10);
    return 0;
}

/* Calls fn for each entry of the directory at ext (size bytes), but . and ..; stops when fn
   returns non-zero and returns that. */
static int iso_each(FILE *f, uint32_t ext, uint32_t size,
                    int (*fn)(const char *name, uint32_t ext, uint32_t size, int isdir, void *ctx), void *ctx)
{
    unsigned char s[2048];
    uint32_t sec;
    for (sec = 0; sec * 2048 < size; sec++) {
        unsigned pos = 0;
        if (iso_sector(f, ext + sec, s)) return -1;
        while (pos + 33 < 2048 && s[pos]) {
            unsigned len = s[pos], nl = s[pos + 32];
            char name[256];
            int r;
            if (pos + len > 2048 || 33 + nl > len) break;
            memcpy(name, s + pos + 33, nl);
            name[nl] = 0;
            if (!(nl == 1 && (name[0] == 0 || name[0] == 1))) {
                char *v = strchr(name, ';');
                if (v) *v = 0;
                if (*name && name[strlen(name) - 1] == '.') name[strlen(name) - 1] = 0;
                r = fn(name, le32(s + pos + 2), le32(s + pos + 10), (s[pos + 25] & 2) != 0, ctx);
                if (r) return r;
            }
            pos += len;
        }
    }
    return 0;
}

struct iso_find { const char *name; uint32_t ext, size; int isdir; };

static int find_fn(const char *name, uint32_t ext, uint32_t size, int isdir, void *ctx)
{
    struct iso_find *q = ctx;
    if (strcasecmp(name, q->name)) return 0;
    q->ext = ext; q->size = size; q->isdir = isdir;
    return 1;
}

/* The UW2 directory of a CD image: 0 with its extent and size if it has UW2.EXE. */
static int iso_uw2(FILE *f, uint32_t *ext, uint32_t *size)
{
    struct iso_find q;
    uint32_t re, rs;
    if (iso_root(f, &re, &rs)) return -1;
    q.name = "UW2";
    if (iso_each(f, re, rs, find_fn, &q) != 1 || !q.isdir) return -1;
    *ext = q.ext; *size = q.size;
    q.name = "UW2.EXE";
    return iso_each(f, *ext, *size, find_fn, &q) == 1 && !q.isdir ? 0 : -1;
}

struct iso_copy { FILE *f; const char *dest; };

static int copy_fn(const char *name, uint32_t ext, uint32_t size, int isdir, void *ctx)
{
    struct iso_copy *c = ctx, sub;
    char path[MAXP];
    unsigned char buf[2048];
    uint32_t done;
    FILE *out;
    if (snprintf(path, sizeof path, "%s/%s", c->dest, name) >= (int)sizeof path) return -1;
    if (isdir) {
        if (mkdirs(path)) return -1;
        sub.f = c->f; sub.dest = path;
        return iso_each(c->f, ext, size, copy_fn, &sub) < 0 ? -1 : 0;
    }
    if (!(out = fopen(path, "wb"))) return -1;
    for (done = 0; done < size; done += 2048) {
        uint32_t n = size - done < 2048 ? size - done : 2048;
        if (iso_sector(c->f, ext + done / 2048, buf) || fwrite(buf, 1, n, out) != n) { fclose(out); return -1; }
    }
    return fclose(out) ? -1 : 0;
}

/* Copies the UW2 directory of the CD image iso into home/gog-cd/UW2, once: a marker file
   written last says the copy is whole. 0 and the directory in out. */
static int extract_iso(const char *iso, const char *home, char *out, size_t outsz)
{
    char marker[MAXP];
    struct iso_copy c;
    uint32_t ext, size;
    FILE *f, *m;
    int r;
    snprintf(out, outsz, "%s/gog-cd/UW2", home);
    snprintf(marker, sizeof marker, "%s/gog-cd/.complete", home);
    if ((m = fopen(marker, "rb")) != NULL) {
        fclose(m);
        if (game_dir_ok(out) == 0) return 0;
    }
    if (!(f = fopen(iso, "rb"))) return -1;
    if (iso_uw2(f, &ext, &size) || mkdirs(out)) { fclose(f); return -1; }
    fprintf(stderr, "uw2port: copying the game out of %s into %s (once)\n", iso, out);
    c.f = f; c.dest = out;
    r = iso_each(f, ext, size, copy_fn, &c);
    fclose(f);
    if (r < 0 || game_dir_ok(out) != 0) return -1;
    if ((m = fopen(marker, "wb")) != NULL) fclose(m);
    return 0;
}

/* 1 if path is a CD image (.gog or .iso) with UW2.EXE in its UW2 directory. */
static int iso_has_uw2(const char *path)
{
    const char *dot = strrchr(path, '.');
    uint32_t e, s;
    FILE *f;
    int ok;
    if (!dot || (strcasecmp(dot, ".gog") && strcasecmp(dot, ".iso"))) return 0;
    if (!(f = fopen(path, "rb"))) return 0;
    ok = iso_uw2(f, &e, &s) == 0;
    fclose(f);
    return ok;
}

/* Looks in dir and up to depth levels below it: with iso 0 for a game directory, with iso 1
   for a CD image, which is then copied out. 1 with the game directory in out. */
static int search(const char *dir, int depth, int iso, const char *home, char *out, size_t outsz)
{
    DIR *d;
    struct dirent *e;
    char path[MAXP], **subs = NULL;
    int nsub = 0, found = 0, i;
    if (!iso && game_dir_ok(dir) == 0) { snprintf(out, outsz, "%s", dir); return 1; }
    if (!(d = opendir(dir))) return 0;
    while (!found && (e = readdir(d)) != NULL) {
        if (e->d_name[0] == '.') continue;
        if (snprintf(path, sizeof path, "%s/%s", dir, e->d_name) >= (int)sizeof path) continue;
        if (is_dir(path)) {
            if (depth > 0 && nsub < 256 && (subs = realloc(subs, sizeof *subs * (size_t)(nsub + 1))) != NULL)
                subs[nsub++] = strdup(path);
        } else if (iso && iso_has_uw2(path) && extract_iso(path, home, out, outsz) == 0) {
            found = 1;
        }
    }
    closedir(d);
    for (i = 0; i < nsub; i++) {
        if (!found) found = search(subs[i], depth - 1, iso, home, out, outsz);
        free(subs[i]);
    }
    free(subs);
    return found;
}

/* 1 if a folder's name says it may hold the game. */
static int looks_like_uw(const char *name)
{
    char low[256];
    size_t i;
    for (i = 0; name[i] && i < sizeof low - 1; i++) low[i] = (char)tolower((unsigned char)name[i]);
    low[i] = 0;
    return strstr(low, "underworld") != NULL || strstr(low, "uw2") != NULL;
}

/* The roots GOG installs into on this host, with ~ for the user's home and $NAME for an
   environment variable at the start. */
static const char *const roots[] = {
#if defined(__APPLE__)
    "/Applications", "~/Applications", "~/GOG Games", "~/Games", "~/UWGOG",
#elif defined(_WIN32)
    "C:/GOG Games", "D:/GOG Games", "$ProgramFiles(x86)/GOG Galaxy/Games", "$ProgramFiles/GOG Galaxy/Games",
    "$USERPROFILE/GOG Games", "$USERPROFILE/UWGOG",
#else
    "~/GOG Games", "~/Games", "~/Games/Heroic", "~/.wine/drive_c/GOG Games",
    "~/.wine/drive_c/Program Files (x86)/GOG Galaxy/Games", "~/UWGOG",
#endif
};

static int expand(const char *root, char *out, size_t outsz)
{
    const char *v, *rest;
    char name[64];
    size_t n;
    if (root[0] == '~') {
        v = getenv("HOME");
#ifdef _WIN32
        if (!v) v = getenv("USERPROFILE");
#endif
        rest = root + 1;
    } else if (root[0] == '$') {
        rest = strchr(root, '/');
        n = (size_t)(rest - root - 1);
        if (n >= sizeof name) return -1;
        memcpy(name, root + 1, n);
        name[n] = 0;
        v = getenv(name);
    } else {
        v = "";
        rest = root;
    }
    if (!v) return -1;
    return snprintf(out, outsz, "%s%s", v, rest) < (int)outsz ? 0 : -1;
}

/* 1 with the game directory in out if dir, or its folder UW2, is one. */
static int here_or_uw2(const char *dir, char *out, size_t outsz)
{
    char sub[MAXP];
    if (game_dir_ok(dir) == 0) { snprintf(out, outsz, "%s", dir); return 1; }
    if (find_ci(dir, "UW2", sub, sizeof sub) && game_dir_ok(sub) == 0) { snprintf(out, outsz, "%s", sub); return 1; }
    return 0;
}

/* A folder the user named: it, a folder up to four levels below it, or a CD image there. */
int port_game_in(const char *dir, const char *home, char *out, size_t outsz)
{
    return search(dir, 4, 0, home, out, outsz) || search(dir, 4, 1, home, out, outsz) ? 0 : -1;
}

#ifdef _WIN32
/* GOG's installers record the games they install in the registry (sys/gogreg.c). 1 with the
   game directory in out: Ultima Underworld 1+2's own entry first; then any entry whose title
   or folder looks like the game, searched as the install roots are; then every other entry's
   folder itself, its UW2 folder or a CD image in it, so that a renamed install is still found
   without walking every game's folders. */
struct gog_ctx { int pass; const char *home; char *out; size_t outsz; };

static int gog_entry(const char *id, const char *name, const char *path, void *ctx)
{
    struct gog_ctx *c = ctx;
    int own = !strcmp(id, PORT_GOG_UW12), uw = own || looks_like_uw(name) || looks_like_uw(path);
    if (c->pass == 0) return own && (search(path, 4, 0, c->home, c->out, c->outsz) || search(path, 4, 1, c->home, c->out, c->outsz));
    if (c->pass == 1) return !own && uw && (search(path, 4, 0, c->home, c->out, c->outsz) || search(path, 4, 1, c->home, c->out, c->outsz));
    return !uw && (here_or_uw2(path, c->out, c->outsz) || search(path, 0, 1, c->home, c->out, c->outsz));
}

static int gog_registry(const char *home, char *out, size_t outsz)
{
    struct gog_ctx c;
    c.home = home; c.out = out; c.outsz = outsz;
    for (c.pass = 0; c.pass < 3; c.pass++)
        if (port_gog_registry(gog_entry, &c)) return 1;
    return 0;
}
#endif

int port_find_game(const char *home, char *out, size_t outsz)
{
    char path[MAXP], cfg[MAXP];
    const char *base = plat_base_dir(), *env = getenv("UW2PORT_DATA");
    size_t i;
    int iso;
    if (env && *env && game_dir_ok(env) == 0) { snprintf(out, outsz, "%s", env); return 0; }
    if (port_config_get(home, "data", cfg, sizeof cfg) == 0 && game_dir_ok(cfg) == 0) {
        snprintf(out, outsz, "%s", cfg);
        return 0;
    }
    /* the current directory and the executable's (not inside an app bundle), and a UW2
       folder in either; nothing else there is opened, since macOS asks the user before a
       program reads Documents, Desktop or Downloads */
    if (here_or_uw2(".", out, outsz)) return 0;
    if (base && !strstr(base, ".app/Contents/") && snprintf(path, sizeof path, "%s", base) < (int)sizeof path) {
        if (strlen(path) > 1 && (path[strlen(path) - 1] == '/' || path[strlen(path) - 1] == '\\')) path[strlen(path) - 1] = 0;
        if (here_or_uw2(path, out, outsz)) return 0;
    }
#ifdef _WIN32
    if (gog_registry(home, out, outsz)) return 0;
#endif
    /* GOG's places: a game directory first, then a CD image */
    for (iso = 0; iso < 2; iso++)
        for (i = 0; i < sizeof roots / sizeof roots[0]; i++) {
            char root[MAXP], sub[MAXP];
            DIR *d;
            struct dirent *e;
            int found = 0;
            if (expand(roots[i], root, sizeof root) || !(d = opendir(root))) continue;
            while (!found && (e = readdir(d)) != NULL)
                if (e->d_name[0] != '.' && looks_like_uw(e->d_name)
                    && snprintf(sub, sizeof sub, "%s/%s", root, e->d_name) < (int)sizeof sub && is_dir(sub))
                    found = search(sub, 4, iso, home, out, outsz);
            closedir(d);
            if (found) return 0;
        }
    return -1;
}

/* What was found and refused, for the message when nothing was found: "" if nothing. */
const char *port_game_refused(void)
{
    static char msg[MAXP + 200];
    if (!refused[0]) return "";
    snprintf(msg, sizeof msg, "\n\nFound %s, but it is %s, not the GOG release's UW2.EXE.", refused,
             refused_why == -2 ? "another size" : "another build");
    return msg;
}

/* The settings file. */
static void cfg_path(const char *home, char *out, size_t outsz)
{
    snprintf(out, outsz, "%s/uw2port.cfg", home);
}

int port_config_get(const char *home, const char *key, char *out, size_t outsz)
{
    char path[MAXP], line[MAXP + 64];
    size_t kl = strlen(key);
    FILE *f;
    int r = -1;
    cfg_path(home, path, sizeof path);
    if (!(f = fopen(path, "r"))) return -1;
    while (fgets(line, sizeof line, f))
        if (!strncmp(line, key, kl) && line[kl] == '=') {
            line[strcspn(line, "\r\n")] = 0;
            if (strlen(line + kl + 1) < outsz) { strcpy(out, line + kl + 1); r = 0; }
        }
    fclose(f);
    return r;
}

int port_config_set(const char *home, const char *key, const char *value)
{
    char path[MAXP], tmp[MAXP + 8], line[MAXP + 64];
    size_t kl = strlen(key);
    FILE *in, *out;
    cfg_path(home, path, sizeof path);
    snprintf(tmp, sizeof tmp, "%s.new", path);
    if (!(out = fopen(tmp, "w"))) return -1;
    if ((in = fopen(path, "r")) != NULL) {
        while (fgets(line, sizeof line, in))
            if (!(!strncmp(line, key, kl) && line[kl] == '=')) fputs(line, out);
        fclose(in);
    } else {
        fputs("# uw2port settings (docs/BUILDING.md, \"Running the port\")\n", out);
    }
    fprintf(out, "%s=%s\n", key, value);
    if (fclose(out)) return -1;
    remove(path);
    return rename(tmp, path);
}
