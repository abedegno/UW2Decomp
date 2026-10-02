/* main.c: replaces nothing in the game; it is what DOS and C0 did before the game's main
   (UWEDIT.C, renamed uw2_main by compat.h): it reads the port's options, maps the user's data
   (the data root) and the port's home directory, loads and checks the far data from the user's
   UW2.EXE, starts the PIT, and runs the game on its own thread under the platform layer.

   uw2port [options] [game arguments]
     --data DIR             the UW2 directory: UW2.EXE, DATA, CRIT, CUTS, SOUND (default: .)
     --home DIR             where the game's files go (default: $UW2PORT_HOME, else ~/.uw2port)
     --scale N              initial window scale (3)
     --no-aspect            square pixels (default: 200 lines shown as 240, as on a 4:3 CRT)
     --no-integer           scale freely (default: whole multiples)
     --hidden               no window (tests); the scan-out still runs
     --screenshot-after MS  write the screen to a PNG MS milliseconds after start
     --screenshot FILE      that PNG's name (uw2port.png)
     --window-shot FILE     with --screenshot-after, also the window's scaled contents
     --shot-at-flip K:FILE  write the screen right after the game's K-th page flip
     --exit-after MS        quit MS milliseconds after start
     --exit-on-halt         quit (status 3) where the port stops, at a stub or an unported path,
                            instead of leaving the window up
     -v                     trace (also UW2PORT_TRACE=1)
   Anything else goes to the game as its own command line (the dungeon file, UWEDIT.C). */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include "plat.h"

int uw2_main(int argc, char *argv[]);
void borland_init(void);
void port_crash_handlers(void);

int port_trace;

void port_log(const char *fmt, ...)
{
    va_list ap;
    if (!port_trace) return;
    va_start(ap, fmt);
    fputs("uw2port: ", stderr);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

void port_fatal(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fputs("uw2port: ", stderr);
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
    exit(1);
}

void port_backtrace(void);

static int exit_on_halt;

void port_halt(const char *why)
{
    fprintf(stderr, "uw2port: %s; the game stops here%s\n", why, exit_on_halt ? "" : " (the window stays)");
    if (port_trace) port_backtrace();
    fflush(stdout);
    if (exit_on_halt) plat_game_exit(3);
    plat_game_park();
}

/* --shot-at-flip K:FILE: the scan-out right after the game's K-th grPageFlip, so a screen
   can be compared with DOS's whatever the timing (display_screen(5, 6) is flip 1,
   display_screen(6, 7) flip 2). */
#define MAXSHOTS 8
static struct { int flip; const char *path; } shots[MAXSHOTS];
static int nshots, flips;

void port_on_flip(void)
{
    static uint8_t pix[640 * 480];
    uint8_t pal[768];
    int i, w, h;
    flips++;
    for (i = 0; i < nshots; i++)
        if (shots[i].flip == flips) {
            vga_scanout(pix, &w, &h, pal);
            if (plat_write_png(shots[i].path, pix, w, h, pal) == 0)
                fprintf(stderr, "uw2port: wrote %s (%dx%d) at flip %d\n", shots[i].path, w, h, flips);
        }
}

static int game_argc;
static char **game_argv;

static int game(void *arg)
{
    (void)arg;
    return uw2_main(game_argc, game_argv);
}

static void on_lifecycle(int ev)
{
    port_log("lifecycle event %d\n", ev);
}

static void usage(void)
{
    fprintf(stderr, "usage: uw2port [--data DIR] [--home DIR] [--scale N] [--no-aspect] [--no-integer]\n"
                    "               [--hidden] [--screenshot-after MS] [--screenshot FILE] [--window-shot FILE]\n"
                    "               [--shot-at-flip K:FILE] [--exit-after MS] [--exit-on-halt]\n"
                    "               [-v] [game arguments]\n");
    exit(2);
}

int main(int argc, char *argv[])
{
    static char home_buf[1024], exe[1200];
    const char *data = ".", *home = getenv("UW2PORT_HOME");
    PlatConfig cfg;
    PlatHooks hooks;
    int i;

    memset(&cfg, 0, sizeof cfg);
    cfg.title = "Ultima Underworld II";
    cfg.scale = 3;
    cfg.integer_scale = 1;
    cfg.aspect = 1;
    game_argv = calloc((size_t)argc + 1, sizeof *game_argv);
    game_argv[game_argc++] = "UW2.EXE";
    port_trace = getenv("UW2PORT_TRACE") != NULL;
    for (i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "--data") && i + 1 < argc) data = argv[++i];
        else if (!strcmp(a, "--home") && i + 1 < argc) home = argv[++i];
        else if (!strcmp(a, "--scale") && i + 1 < argc) cfg.scale = atoi(argv[++i]);
        else if (!strcmp(a, "--no-aspect")) cfg.aspect = 0;
        else if (!strcmp(a, "--no-integer")) cfg.integer_scale = 0;
        else if (!strcmp(a, "--hidden")) cfg.hidden = 1;
        else if (!strcmp(a, "--screenshot-after") && i + 1 < argc) cfg.screenshot_after_ms = atol(argv[++i]);
        else if (!strcmp(a, "--screenshot") && i + 1 < argc) cfg.screenshot_path = argv[++i];
        else if (!strcmp(a, "--window-shot") && i + 1 < argc) cfg.window_shot_path = argv[++i];
        else if (!strcmp(a, "--shot-at-flip") && i + 1 < argc && nshots < MAXSHOTS && strchr(argv[i + 1], ':')) {
            shots[nshots].flip = atoi(argv[++i]);
            shots[nshots++].path = strchr(argv[i], ':') + 1;
        }
        else if (!strcmp(a, "--exit-after") && i + 1 < argc) cfg.exit_after_ms = atol(argv[++i]);
        else if (!strcmp(a, "--exit-on-halt")) exit_on_halt = 1;
        else if (!strcmp(a, "-v")) port_trace = 1;
        else if (!strcmp(a, "--help") || !strcmp(a, "-h")) usage();
        else if (a[0] == '-' && a[1] == '-') usage();
        else game_argv[game_argc++] = argv[i];
    }
    if (!home) {
        const char *h = getenv("HOME");
        snprintf(home_buf, sizeof home_buf, "%s/.uw2port", h ? h : ".");
        home = home_buf;
    }
    if (plat_files_init(data, home)) {
        fprintf(stderr, "uw2port: %s is not a directory (give the UW2 directory with --data), "
                        "or %s cannot be created\n", data, home);
        return 1;
    }
    if (plat_resolve("UW2.EXE", PLAT_READ, exe, sizeof exe) || port_load_exe(exe)) return 1;
    borland_init();
    port_crash_handlers();
    /* The game finds its home through UWHOME; the port's home directory plays that part,
       laid over the data root, so the game's default (".", the game's own directory) is
       right and the host's own UWHOME must not leak in. */
    unsetenv("UWHOME");
    pit_start();
    memset(&hooks, 0, sizeof hooks);
    hooks.scanout = vga_scanout;
    hooks.key = kbd_byte;
    hooks.pointer = mouse_event;
    hooks.lifecycle = on_lifecycle;
    return plat_run(&cfg, &hooks, game, NULL);
}
