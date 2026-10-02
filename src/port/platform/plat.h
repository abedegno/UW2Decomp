/* plat.h: the platform API of the native port (docs/PORT.md, "The platform layer"). Replaces
   nothing in the DOS build: it is the edge between the port's portable C (the game, the C
   written for its assembly modules, the VGA, PIT and keyboard emulation) and the host.

   Nothing here names SDL, so a new host is one backend file: src/port/platform/sdl3/ is the
   SDL3 backend (macOS, Windows, Linux, iOS and iPadOS, Android and Emscripten through SDL), and
   a libretro core or a bare web build would implement the same functions. files.c and png.c
   are the parts every POSIX host shares and need no backend.

   The model is the PC's. The game runs on a thread of its own, as the CPU did. The backend
   owns the window and the event loop on the host's main thread, and calls back into the port
   the way the PC's hardware interrupted the CPU: a scan code byte as from port 60h for each key
   change, pointer events, and lifecycle events. It reads the picture out of the emulated VGA
   whenever it presents a frame, as the VGA's CRT controller did, so the game never presents
   anything itself. The port's own timer thread (the PIT) uses plat_counter and plat_sleep_ns.

   Rules for everything above this API: no JIT and no self-modifying code (iOS forbids writable
   executable memory), no host path or file name written by the game reaches the host
   unmapped (plat_resolve), and no SDL type. */
#ifndef UW2_PLAT_H
#define UW2_PLAT_H

#include <stddef.h>
#include <stdint.h>

/* Pointer events. A pointer is a mouse, a pen or a finger: the game's mouse is built from
   them by the port (src/port/sys/mousedrv.c), so a touch screen needs no other path. x and y
   are in the game's screen pixels (0..319, 0..199, from the top left), already undone from the
   window's scaling and letterbox; dx and dy are the relative motion in host pixels at the
   window's scale, for the mickeys of int 33h function 0Bh. */
enum { PLAT_POINTER_MOVE = 0, PLAT_POINTER_DOWN = 1, PLAT_POINTER_UP = 2 };
enum { PLAT_BUTTON_LEFT = 1, PLAT_BUTTON_RIGHT = 2, PLAT_BUTTON_MIDDLE = 4 };
typedef struct PlatPointer {
    int type;               /* PLAT_POINTER_MOVE, _DOWN or _UP */
    int id;                 /* 0 for the mouse; a finger or pen number otherwise */
    float x, y;             /* position in screen pixels */
    float dx, dy;           /* relative motion */
    unsigned button;        /* the button that changed (PLAT_BUTTON_*), for _DOWN and _UP */
    unsigned buttons;       /* every button now held */
} PlatPointer;

/* Lifecycle events: the host is about to suspend the program (a phone going to the
   background, a window minimised) or has resumed it, or the user asked to quit (the window's
   close box). */
enum { PLAT_SUSPEND = 1, PLAT_RESUME = 2, PLAT_QUIT_REQUEST = 3 };

/* What the backend calls. Every hook runs on the backend's thread, not the game's, as an
   interrupt handler did; each must only do what an interrupt handler could. */
typedef struct PlatHooks {
    /* Fills pixels (at least 640 by 480 bytes) with the indexed picture now on the screen and
       rgb6 with the DAC's 256 6-bit entries; returns the picture's width and height. */
    void (*scanout)(uint8_t *pixels, int *width, int *height, uint8_t rgb6[768]);
    /* One byte from the keyboard controller: a PC set-1 scan code, make or break (bit 7),
       with E0 and E1 prefixes as separate bytes, in the order the keyboard sends them. */
    void (*key)(uint8_t scancode);
    void (*pointer)(const PlatPointer *ev);
    void (*lifecycle)(int event);
} PlatHooks;

/* How the backend presents the screen, and the debug options. */
typedef struct PlatConfig {
    const char *title;
    int scale;                  /* initial window scale (default 3) */
    int integer_scale;          /* scale by whole multiples only (default 1) */
    int aspect;                 /* stretch 200 lines to 240, as a 4:3 monitor showed them (default 1) */
    int hidden;                 /* no visible window (tests, CI); the scan-out still runs */
    long screenshot_after_ms;   /* > 0: write the scan-out as a PNG this long after start */
    const char *screenshot_path;
    const char *window_shot_path;   /* with a screenshot, also the window's scaled contents */
    long exit_after_ms;         /* > 0: quit this long after start */
} PlatConfig;

/* Runs the program: opens the window, starts game(arg) on a thread of its own and runs the
   event loop until the game ends or quits, then returns the game's exit status. */
int plat_run(const PlatConfig *cfg, const PlatHooks *hooks, int (*game)(void *), void *arg);

/* Ends the program from the game's thread (exit, a fatal error): the event loop stops and
   plat_run returns status. Does not return. */
void plat_game_exit(int status) __attribute__((noreturn));

/* Parks the game's thread for good, leaving the window up, so that the last screen stays
   visible (a stub the port reached, docs/PORT.md). Does not return. */
void plat_game_park(void) __attribute__((noreturn));

/* Time: a monotonic high-resolution counter and its rate, and a precise sleep. */
uint64_t plat_counter(void);
uint64_t plat_counter_hz(void);
void plat_sleep_ns(uint64_t ns);

/* A thread for the port's own use (the PIT). */
int plat_thread_start(const char *name, int (*fn)(void *), void *arg);

/* Audio: an output stream of signed 16-bit stereo samples at rate Hz; fill is called on the
   audio thread for each block. Milestone 3 opens it and feeds silence. */
int plat_audio_open(int rate, void (*fill)(int16_t *samples, int frames));

/* Files (files.c, any POSIX host). The data root is the user's own copy of UW2, read-only;
   the home directory takes every file the game creates or changes, and is searched before
   the data root, so the game sees one tree: the data root with the home directory laid over
   it. DOS paths (backslashes, any case, a drive letter) are matched case-insensitively. */
enum { PLAT_READ = 0, PLAT_WRITE = 1, PLAT_CREATE = 2 };
int plat_files_init(const char *data_root, const char *home);
const char *plat_data_root(void);
const char *plat_home(void);
/* Maps a DOS path to a host path. PLAT_READ: the home copy if there is one, else the data
   root's, else the home path it would have (so that a failing open fails as in DOS).
   PLAT_WRITE: a file to be changed: copied from the data root into the home directory first.
   PLAT_CREATE: the home path. Returns 0, or -1 if the path is too long. */
int plat_resolve(const char *dospath, int mode, char *out, size_t outsz);
/* Lists the DOS directory dir: calls fn for each entry of the merged tree, once per name. */
int plat_listdir(const char *dosdir, void (*fn)(const char *name, int isdir, long size,
                                               long mtime, void *ctx), void *ctx);

/* Writes an indexed picture with a 6-bit palette as a PNG (png.c). 0 if it was written. */
int plat_write_png(const char *path, const uint8_t *pixels, int width, int height,
                   const uint8_t rgb6[768]);
int plat_write_png_rgb(const char *path, const uint8_t *rgb, int width, int height);

#endif
