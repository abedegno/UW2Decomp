/* blackbox.c: replaces nothing. Every player's session is recorded, so that a crash, or
   anything else the game does differently from DOS, can be replayed exactly (docs/BUILDING.md,
   "Recording and replaying a session"). Each session gets a folder, recordings/YYYYMMDD-HHMMSS
   in the home directory, holding RECORD.OUT, the inputs as src/replay/REPLAY.C records them,
   and stage/, a copy of the home directory's game files (the saved games and DATA\UW.CFG) as
   they were when the session began, which is what a replay starts from:

       python3 tools/replay.py port recordings/S/RECORD.OUT OUT --stage recordings/S/stage

   No state dumps are written and F12 does not stop the recording. A fault in the port writes
   the recording's buffered streams before the program ends (crash.c). The newest KEEP
   sessions are kept. --no-recording turns it off. */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#ifdef _WIN32
#include <direct.h>
#define mkdir(p, m) _mkdir(p)
#else
#include <unistd.h>
#endif
#include "port.h"

#define KEEP 5

extern int16_t rp_request;
extern int16_t rp_blackbox;
extern const char *rp_blackbox_name;
void rp_blackbox_close(int crashed);

static int is_dir(const char *p)
{
    struct stat st;
    return stat(p, &st) == 0 && S_ISDIR(st.st_mode);
}

static int copy_file(const char *from, const char *to)
{
    unsigned char buf[65536];
    size_t n;
    FILE *in = fopen(from, "rb"), *out;
    if (!in) return 1;
    if (!(out = fopen(to, "wb"))) { fclose(in); return 1; }
    while ((n = fread(buf, 1, sizeof buf, in)) > 0) fwrite(buf, 1, n, out);
    fclose(in);
    return fclose(out) != 0;
}

/* from's files and folders into to, which is made; skip names the top level leaves out */
static int copy_tree(const char *from, const char *to, int top)
{
    DIR *d = opendir(from);
    struct dirent *e;
    char a[1200], b[1200];
    int bad = 0;
    if (!d) return 1;
    mkdir(to, 0755);
    while ((e = readdir(d)) != NULL) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        /* the recordings themselves, and the port's own settings and logs, are not the game's */
        if (top && (!strcmp(e->d_name, "recordings") || !strcmp(e->d_name, "uw2port.cfg") ||
                    !strcmp(e->d_name, "STATE.OUT") || !strcmp(e->d_name, "RECORD.OUT") ||
                    !strcmp(e->d_name, "REPLAY.IN") || !strcmp(e->d_name, "TRACE.OUT")))
            continue;
        snprintf(a, sizeof a, "%s/%s", from, e->d_name);
        snprintf(b, sizeof b, "%s/%s", to, e->d_name);
        bad |= is_dir(a) ? copy_tree(a, b, 0) : copy_file(a, b);
    }
    closedir(d);
    return bad;
}

/* removes the folder p and everything in it; p is always a session folder under recordings/ */
static void remove_tree(const char *p)
{
    DIR *d = opendir(p);
    struct dirent *e;
    char a[1200];
    if (!d) return;
    while ((e = readdir(d)) != NULL) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        snprintf(a, sizeof a, "%s/%s", p, e->d_name);
        if (is_dir(a)) remove_tree(a);
        else remove(a);
    }
    closedir(d);
    rmdir(p);
}

static int by_name(const void *x, const void *y)
{
    return strcmp(*(char *const *)x, *(char *const *)y);
}

/* all but the newest KEEP - 1 sessions go, to leave room for this one; session folders are
   named by their time, so the oldest sort first */
static void prune(const char *dir)
{
    DIR *d = opendir(dir);
    struct dirent *e;
    char *names[256], path[1200];
    int n = 0, i;
    if (!d) return;
    while ((e = readdir(d)) != NULL && n < 256)
        if (strlen(e->d_name) == 15 && e->d_name[8] == '-') names[n++] = strdup(e->d_name);
    closedir(d);
    qsort(names, (size_t)n, sizeof *names, by_name);
    for (i = 0; i < n; i++) {
        if (i < n - (KEEP - 1)) {
            snprintf(path, sizeof path, "%s/%s", dir, names[i]);
            remove_tree(path);
        }
        free(names[i]);
    }
}

/* Called by main.c before the game starts, for a player's run (not a test's, not --record or
   --replay). Returns 0 when the session is being recorded. */
int port_blackbox_start(const char *home)
{
    static char dospath[64];
    char dir[1100], session[1200], stage[1300];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char stamp[16];
    strftime(stamp, sizeof stamp, "%Y%m%d-%H%M%S", tm);
    snprintf(dir, sizeof dir, "%s/recordings", home);
    mkdir(dir, 0755);
    prune(dir);
    snprintf(session, sizeof session, "%s/%s", dir, stamp);
    if (mkdir(session, 0755) != 0 && !is_dir(session)) return 1;
    snprintf(stage, sizeof stage, "%s/stage", session);
    if (copy_tree(home, stage, 1)) fprintf(stderr, "uw2port: the session's starting files were not all copied\n");
    snprintf(dospath, sizeof dospath, "RECORDINGS\\%s\\RECORD.OUT", stamp);
    rp_blackbox_name = dospath;
    rp_blackbox = 1;
    rp_request = 1;
    fprintf(stderr, "uw2port: recording this session to %s/RECORD.OUT\n", session);
    return 0;
}

/* The fault handler's and exit's part: the recording's buffered streams written. */
void port_blackbox_close(int crashed)
{
    if (rp_blackbox) rp_blackbox_close(crashed);
}
