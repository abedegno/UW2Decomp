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
     --record               record the session to RECORD.OUT in the home directory, with the
                            state dumps in STATE.OUT (src/replay/REPLAY.C; F12 ends it)
     --replay FILE          replay the recording FILE (copied to the home directory as
                            REPLAY.IN) instead of reading the clock, keyboard and mouse, writing
                            the state dumps to STATE.OUT; the game quits at its end
     --sound CARD[,SPEECH]  the sound cards, as UW.CFG names them (docs/BUILDING.md, "Sound"):
                            music 0 none, 2 Ad Lib, 3 Sound Blaster, 4 Sound Blaster Pro 1,
                            5 MT-32, 6 Pro Audio Spectrum, 7 Sound Blaster Pro 2; speech 0
                            none, 1 Sound Blaster, 2 Sound Blaster Pro, 3 Pro Audio Spectrum.
                            Written to DATA\UW.CFG in the home directory, which the game reads
     --mt32-roms DIR        the user's MT-32 or CM-32L ROMs (also UW2PORT_MT32_ROMS)
     --audio-wav FILE       write everything the sound cards play to a WAV file (44100 Hz)
     --no-audio             open no audio device
     --ail-log FILE         log every AIL call and driver service (src/port/sound/ail.c)
     --hw-log FILE          log every register write and MIDI byte the drivers make
     -v                     trace (also UW2PORT_TRACE=1)
   Anything else goes to the game as its own command line (the dungeon file, UWEDIT.C). */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include "plat.h"
#include "sound/audio.h"

int uw2_main(int argc, char *argv[]);
void borland_init(void);
void port_crash_handlers(void);

int port_trace;
extern int16_t rp_request;              /* src/replay/REPLAY.C: 0 off, 1 record, 2 replay */

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

void rp_halt(void);

void port_halt(const char *why)
{
    rp_halt();
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

/* --replay: the recording goes into the home directory as REPLAY.IN, where the game's own
   open finds it. */
static int copy_to_home(const char *src, const char *home, const char *name)
{
    char path[1200];
    unsigned char buf[65536];
    size_t n;
    FILE *in = fopen(src, "rb"), *out;
    snprintf(path, sizeof path, "%s/%s", home, name);
    out = in ? fopen(path, "wb") : NULL;
    if (!out) {
        fprintf(stderr, "uw2port: cannot copy %s to %s\n", src, path);
        if (in) fclose(in);
        return 1;
    }
    while ((n = fread(buf, 1, sizeof buf, in)) > 0) fwrite(buf, 1, n, out);
    fclose(in);
    fclose(out);
    return 0;
}

/* --sound CARD[,SPEECH]: DATA\UW.CFG in the home directory, the file UWSOUND.EXE writes: the
   music card and the speech card, each with its IRQ, port (hex) and DMA, for the cards'
   factory settings (SOUND.C, seg016_1E73_2FCB, reads it; the port's drivers take any) */
static int write_uw_cfg(const char *spec)
{
    char path[1200];
    int music = atoi(spec), speech = strchr(spec, ',') ? atoi(strchr(spec, ',') + 1) : 0;
    FILE *f;
    if (plat_resolve("DATA\\UW.CFG", PLAT_CREATE, path, sizeof path) || !(f = fopen(path, "wb"))) {
        fprintf(stderr, "uw2port: cannot write DATA\\UW.CFG in the home directory\n");
        return 1;
    }
    fprintf(f, "%d %s sound\r\n%d %s speech\r\n", music,
            music == 0 ? "-1 -1 -1" : music == 2 ? "-1 388 -1" : music == 5 ? "-1 330 -1" : "7 220 1",
            speech, speech == 0 ? "-1 -1 -1" : "7 220 1");
    fclose(f);
    return 0;
}

static void usage(void)
{
    fprintf(stderr, "usage: uw2port [--data DIR] [--home DIR] [--scale N] [--no-aspect] [--no-integer]\n"
                    "               [--hidden] [--screenshot-after MS] [--screenshot FILE] [--window-shot FILE]\n"
                    "               [--shot-at-flip K:FILE] [--exit-after MS] [--exit-on-halt]\n"
                    "               [--record | --replay FILE] [--sound CARD[,SPEECH]] [--mt32-roms DIR]\n"
                    "               [--audio-wav FILE] [--no-audio] [--ail-log FILE] [--hw-log FILE]\n"
                    "               [-v] [game arguments]\n");
    exit(2);
}

int main(int argc, char *argv[])
{
    static char home_buf[1024], exe[1200];
    const char *data = ".", *home = getenv("UW2PORT_HOME"), *replay = NULL;
    const char *sound = NULL, *roms = NULL, *wav = NULL, *ail_log = NULL, *hw_log = NULL;
    int audio_device = 1;
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
        else if (!strcmp(a, "--record")) rp_request = 1;
        else if (!strcmp(a, "--replay") && i + 1 < argc) { rp_request = 2; replay = argv[++i]; }
        else if (!strcmp(a, "--sound") && i + 1 < argc) sound = argv[++i];
        else if (!strcmp(a, "--mt32-roms") && i + 1 < argc) roms = argv[++i];
        else if (!strcmp(a, "--audio-wav") && i + 1 < argc) wav = argv[++i];
        else if (!strcmp(a, "--no-audio")) audio_device = 0;
        else if (!strcmp(a, "--ail-log") && i + 1 < argc) ail_log = argv[++i];
        else if (!strcmp(a, "--hw-log") && i + 1 < argc) hw_log = argv[++i];
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
    vga_window_init();
    if (rp_request != 1 && rp_request != 2) rp_request = 0;   /* the port reads the world */
    if (replay && copy_to_home(replay, home, "REPLAY.IN")) return 1;
    if (sound && write_uw_cfg(sound)) return 1;
    audio_config(wav, roms, audio_device && !cfg.hidden);
    ail_set_logs(ail_log, hw_log);
    ail_set_slaved(rp_request == 2);    /* under replay AIL's timers follow the replayed clock */
    borland_init();
    port_crash_handlers();
    /* The game finds its home through UWHOME; the port's home directory plays that part,
       laid over the data root, so the game's default (".", the game's own directory) is
       right and the host's own UWHOME must not leak in. */
#ifdef _WIN32
    _putenv("UWHOME=");                 /* Windows has no unsetenv; an empty value removes it */
#else
    unsetenv("UWHOME");
#endif
    pit_start();
    memset(&hooks, 0, sizeof hooks);
    hooks.scanout = vga_scanout;
    hooks.key = kbd_byte;
    hooks.pointer = mouse_event;
    hooks.lifecycle = on_lifecycle;
    return plat_run(&cfg, &hooks, game, NULL);
}
