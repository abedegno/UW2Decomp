/* plat_sdl3.c: replaces nothing in the DOS build. The SDL3 backend of the platform API
   (plat.h): the window and its scaling, the event loop, key and pointer events, lifecycle
   events, the high-resolution counter, threads and the audio stream. The only file of the port
   that includes SDL.

   The game runs on its own thread (plat_run); this file's loop runs on the main thread, as SDL
   needs on macOS and iOS. Each pass it reads the emulated VGA's picture through the scanout
   hook, converts it through the DAC's palette and presents it, so the screen updates however
   the game spends its time, as a CRT did. */
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plat.h"

static const PlatHooks *hooks;
static SDL_AtomicInt game_done;
static int game_status;

/* SDL scan codes (USB HID usages) to PC set-1 make codes. 0 means no key; a value with bit 8
   set is sent after an E0 prefix. Print Screen and Pause have sequences of their own (key()). */
#define E0(c) (0x100 | (c))
static const uint16_t set1[SDL_SCANCODE_COUNT] = {
    [SDL_SCANCODE_ESCAPE] = 0x01,
    [SDL_SCANCODE_1] = 0x02, [SDL_SCANCODE_2] = 0x03, [SDL_SCANCODE_3] = 0x04, [SDL_SCANCODE_4] = 0x05,
    [SDL_SCANCODE_5] = 0x06, [SDL_SCANCODE_6] = 0x07, [SDL_SCANCODE_7] = 0x08, [SDL_SCANCODE_8] = 0x09,
    [SDL_SCANCODE_9] = 0x0A, [SDL_SCANCODE_0] = 0x0B, [SDL_SCANCODE_MINUS] = 0x0C,
    [SDL_SCANCODE_EQUALS] = 0x0D, [SDL_SCANCODE_BACKSPACE] = 0x0E, [SDL_SCANCODE_TAB] = 0x0F,
    [SDL_SCANCODE_Q] = 0x10, [SDL_SCANCODE_W] = 0x11, [SDL_SCANCODE_E] = 0x12, [SDL_SCANCODE_R] = 0x13,
    [SDL_SCANCODE_T] = 0x14, [SDL_SCANCODE_Y] = 0x15, [SDL_SCANCODE_U] = 0x16, [SDL_SCANCODE_I] = 0x17,
    [SDL_SCANCODE_O] = 0x18, [SDL_SCANCODE_P] = 0x19, [SDL_SCANCODE_LEFTBRACKET] = 0x1A,
    [SDL_SCANCODE_RIGHTBRACKET] = 0x1B, [SDL_SCANCODE_RETURN] = 0x1C, [SDL_SCANCODE_LCTRL] = 0x1D,
    [SDL_SCANCODE_A] = 0x1E, [SDL_SCANCODE_S] = 0x1F, [SDL_SCANCODE_D] = 0x20, [SDL_SCANCODE_F] = 0x21,
    [SDL_SCANCODE_G] = 0x22, [SDL_SCANCODE_H] = 0x23, [SDL_SCANCODE_J] = 0x24, [SDL_SCANCODE_K] = 0x25,
    [SDL_SCANCODE_L] = 0x26, [SDL_SCANCODE_SEMICOLON] = 0x27, [SDL_SCANCODE_APOSTROPHE] = 0x28,
    [SDL_SCANCODE_GRAVE] = 0x29, [SDL_SCANCODE_LSHIFT] = 0x2A, [SDL_SCANCODE_BACKSLASH] = 0x2B,
    [SDL_SCANCODE_NONUSHASH] = 0x2B,
    [SDL_SCANCODE_Z] = 0x2C, [SDL_SCANCODE_X] = 0x2D, [SDL_SCANCODE_C] = 0x2E, [SDL_SCANCODE_V] = 0x2F,
    [SDL_SCANCODE_B] = 0x30, [SDL_SCANCODE_N] = 0x31, [SDL_SCANCODE_M] = 0x32, [SDL_SCANCODE_COMMA] = 0x33,
    [SDL_SCANCODE_PERIOD] = 0x34, [SDL_SCANCODE_SLASH] = 0x35, [SDL_SCANCODE_RSHIFT] = 0x36,
    [SDL_SCANCODE_KP_MULTIPLY] = 0x37, [SDL_SCANCODE_LALT] = 0x38, [SDL_SCANCODE_SPACE] = 0x39,
    [SDL_SCANCODE_CAPSLOCK] = 0x3A,
    [SDL_SCANCODE_F1] = 0x3B, [SDL_SCANCODE_F2] = 0x3C, [SDL_SCANCODE_F3] = 0x3D, [SDL_SCANCODE_F4] = 0x3E,
    [SDL_SCANCODE_F5] = 0x3F, [SDL_SCANCODE_F6] = 0x40, [SDL_SCANCODE_F7] = 0x41, [SDL_SCANCODE_F8] = 0x42,
    [SDL_SCANCODE_F9] = 0x43, [SDL_SCANCODE_F10] = 0x44, [SDL_SCANCODE_NUMLOCKCLEAR] = 0x45,
    [SDL_SCANCODE_SCROLLLOCK] = 0x46,
    [SDL_SCANCODE_KP_7] = 0x47, [SDL_SCANCODE_KP_8] = 0x48, [SDL_SCANCODE_KP_9] = 0x49,
    [SDL_SCANCODE_KP_MINUS] = 0x4A, [SDL_SCANCODE_KP_4] = 0x4B, [SDL_SCANCODE_KP_5] = 0x4C,
    [SDL_SCANCODE_KP_6] = 0x4D, [SDL_SCANCODE_KP_PLUS] = 0x4E, [SDL_SCANCODE_KP_1] = 0x4F,
    [SDL_SCANCODE_KP_2] = 0x50, [SDL_SCANCODE_KP_3] = 0x51, [SDL_SCANCODE_KP_0] = 0x52,
    [SDL_SCANCODE_KP_PERIOD] = 0x53, [SDL_SCANCODE_NONUSBACKSLASH] = 0x56,
    [SDL_SCANCODE_F11] = 0x57, [SDL_SCANCODE_F12] = 0x58,
    [SDL_SCANCODE_KP_ENTER] = E0(0x1C), [SDL_SCANCODE_RCTRL] = E0(0x1D),
    [SDL_SCANCODE_KP_DIVIDE] = E0(0x35), [SDL_SCANCODE_RALT] = E0(0x38),
    [SDL_SCANCODE_HOME] = E0(0x47), [SDL_SCANCODE_UP] = E0(0x48), [SDL_SCANCODE_PAGEUP] = E0(0x49),
    [SDL_SCANCODE_LEFT] = E0(0x4B), [SDL_SCANCODE_RIGHT] = E0(0x4D), [SDL_SCANCODE_END] = E0(0x4F),
    [SDL_SCANCODE_DOWN] = E0(0x50), [SDL_SCANCODE_PAGEDOWN] = E0(0x51), [SDL_SCANCODE_INSERT] = E0(0x52),
    [SDL_SCANCODE_DELETE] = E0(0x53), [SDL_SCANCODE_LGUI] = E0(0x5B), [SDL_SCANCODE_RGUI] = E0(0x5C),
    [SDL_SCANCODE_APPLICATION] = E0(0x5D),
};

static void key(SDL_Scancode sc, int down)
{
    uint16_t c;
    if (!hooks->key) return;
    if (sc == SDL_SCANCODE_PRINTSCREEN) {
        static const uint8_t make[] = { 0xE0, 0x2A, 0xE0, 0x37 }, brk[] = { 0xE0, 0xB7, 0xE0, 0xAA };
        const uint8_t *s = down ? make : brk;
        int i;
        for (i = 0; i < 4; i++) hooks->key(s[i]);
        return;
    }
    if (sc == SDL_SCANCODE_PAUSE) {
        static const uint8_t pause[] = { 0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5 };
        int i;
        if (down) for (i = 0; i < 6; i++) hooks->key(pause[i]);
        return;
    }
    if ((unsigned)sc >= SDL_SCANCODE_COUNT || !(c = set1[sc])) return;
    if (c & 0x100) hooks->key(0xE0);
    hooks->key((uint8_t)((c & 0x7F) | (down ? 0 : 0x80)));
}

static int game_thread(void *p)
{
    void **a = p;
    int (*game)(void *) = (int (*)(void *))a[0];
    int st = game(a[1]);
    game_status = st;
    SDL_SetAtomicInt(&game_done, 1);
    return st;
}

void plat_game_exit(int status)
{
    game_status = status;
    SDL_SetAtomicInt(&game_done, 1);
    for (;;) SDL_Delay(1000);
}

void plat_game_park(void)
{
    for (;;) SDL_Delay(1000);
}

uint64_t plat_counter(void) { return SDL_GetPerformanceCounter(); }
uint64_t plat_counter_hz(void) { return SDL_GetPerformanceFrequency(); }
void plat_sleep_ns(uint64_t ns) { SDL_DelayPrecise(ns); }

int plat_thread_start(const char *name, int (*fn)(void *), void *arg)
{
    SDL_Thread *t = SDL_CreateThread(fn, name, arg);
    if (!t) return -1;
    SDL_DetachThread(t);
    return 0;
}

static void (*audio_fill)(int16_t *, int);

static void SDLCALL audio_cb(void *ud, SDL_AudioStream *s, int additional, int total)
{
    int16_t buf[2048];
    (void)ud; (void)total;
    while (additional > 0) {
        int frames = additional / 4;
        if (frames > 512) frames = 512;
        if (frames <= 0) frames = 1;
        audio_fill(buf, frames);
        SDL_PutAudioStreamData(s, buf, frames * 4);
        additional -= frames * 4;
    }
}

int plat_audio_open(int rate, void (*fill)(int16_t *, int))
{
    SDL_AudioSpec spec;
    SDL_AudioStream *s;
    if (!SDL_WasInit(SDL_INIT_AUDIO) && !SDL_InitSubSystem(SDL_INIT_AUDIO)) return -1;
    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;
    spec.freq = rate;
    audio_fill = fill;
    s = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, audio_cb, NULL);
    if (!s) return -1;
    SDL_ResumeAudioStreamDevice(s);
    return 0;
}

/* The scaled picture's place in the render output: the 4:3 shape when aspect is on, the
   largest that fits, by whole multiples when integer_scale is on, centred. */
static SDL_FRect place(SDL_Renderer *r, const PlatConfig *cfg, int w, int h)
{
    int ow, oh;
    float lw = (float)w, lh = cfg->aspect ? (float)w * 3.0f / 4.0f : (float)h, s;
    SDL_FRect d;
    SDL_GetCurrentRenderOutputSize(r, &ow, &oh);
    s = (float)ow / lw < (float)oh / lh ? (float)ow / lw : (float)oh / lh;
    if (cfg->integer_scale && s >= 1.0f) s = (float)(int)s;
    d.w = lw * s; d.h = lh * s;
    d.x = ((float)ow - d.w) / 2; d.y = ((float)oh - d.h) / 2;
    return d;
}

int plat_run(const PlatConfig *cfg, const PlatHooks *h, int (*game)(void *), void *arg)
{
    static uint8_t pix[640 * 480];
    static uint32_t rgb[640 * 480];
    uint8_t pal[768];
    uint32_t lut[256];
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    SDL_Texture *tex = NULL;
    int tw = 0, th = 0, w = 320, hgt = 200, quit = 0, shot = 0, i, scale = cfg->scale > 0 ? cfg->scale : 3;
    unsigned buttons = 0;
    void *targ[2];
    SDL_FRect dst = { 0, 0, 0, 0 };
    Uint64 start;
    SDL_Event e;
    SDL_Thread *gt;

    hooks = h;
    if (cfg->hidden) SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "offscreen");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        fprintf(stderr, "uw2port: SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (!cfg->hidden) {
        win = SDL_CreateWindow(cfg->title ? cfg->title : "UW2", 320 * scale,
                               (cfg->aspect ? 240 : 200) * scale, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
        if (win) ren = SDL_CreateRenderer(win, NULL);
        if (!win || !ren) {
            fprintf(stderr, "uw2port: no window: %s\n", SDL_GetError());
            return 1;
        }
        SDL_SetRenderVSync(ren, 1);
    }
    targ[0] = (void *)game;
    targ[1] = arg;
    start = SDL_GetTicks();
    {
        /* the game's thread gets a large stack: host frames are bigger than DOS's */
        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetPointerProperty(props, SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER, (void *)game_thread);
        SDL_SetStringProperty(props, SDL_PROP_THREAD_CREATE_NAME_STRING, "game");
        SDL_SetPointerProperty(props, SDL_PROP_THREAD_CREATE_USERDATA_POINTER, targ);
        SDL_SetNumberProperty(props, SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER, 16 << 20);
        gt = SDL_CreateThreadWithProperties(props);
        SDL_DestroyProperties(props);
    }
    if (!gt) {
        fprintf(stderr, "uw2port: no game thread: %s\n", SDL_GetError());
        return 1;
    }
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            PlatPointer p;
            float x, y;
            memset(&p, 0, sizeof p);
            switch (e.type) {
            case SDL_EVENT_QUIT:
                if (hooks->lifecycle) hooks->lifecycle(PLAT_QUIT_REQUEST);
                quit = 1;
                break;
            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
                if (!e.key.repeat) key(e.key.scancode, e.type == SDL_EVENT_KEY_DOWN);
                break;
            case SDL_EVENT_MOUSE_MOTION:
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (e.motion.which == SDL_TOUCH_MOUSEID || !hooks->pointer || !ren || dst.w <= 0) break;
                if (e.type == SDL_EVENT_MOUSE_MOTION) {
                    SDL_RenderCoordinatesFromWindow(ren, e.motion.x, e.motion.y, &x, &y);
                    p.type = PLAT_POINTER_MOVE;
                    {
                        /* window points to render pixels to screen pixels */
                        float dens = win ? SDL_GetWindowPixelDensity(win) : 1.0f;
                        p.dx = e.motion.xrel * dens * (float)w / dst.w;
                        p.dy = e.motion.yrel * dens * (float)hgt / dst.h;
                    }
                } else {
                    unsigned b = e.button.button == SDL_BUTTON_LEFT ? PLAT_BUTTON_LEFT
                               : e.button.button == SDL_BUTTON_RIGHT ? PLAT_BUTTON_RIGHT : PLAT_BUTTON_MIDDLE;
                    SDL_RenderCoordinatesFromWindow(ren, e.button.x, e.button.y, &x, &y);
                    p.type = e.type == SDL_EVENT_MOUSE_BUTTON_DOWN ? PLAT_POINTER_DOWN : PLAT_POINTER_UP;
                    p.button = b;
                    buttons = p.type == PLAT_POINTER_DOWN ? buttons | b : buttons & ~b;
                }
                p.x = (x - dst.x) * (float)w / dst.w;
                p.y = (y - dst.y) * (float)hgt / dst.h;
                p.buttons = buttons;
                hooks->pointer(&p);
                break;
            case SDL_EVENT_FINGER_DOWN:
            case SDL_EVENT_FINGER_UP:
            case SDL_EVENT_FINGER_MOTION: {
                int ow, oh;
                if (!hooks->pointer || !ren || dst.w <= 0) break;
                SDL_GetCurrentRenderOutputSize(ren, &ow, &oh);
                x = e.tfinger.x * (float)ow; y = e.tfinger.y * (float)oh;
                p.id = (int)(e.tfinger.fingerID & 0x7FFFFFFF) + 1;
                p.type = e.type == SDL_EVENT_FINGER_DOWN ? PLAT_POINTER_DOWN
                       : e.type == SDL_EVENT_FINGER_UP ? PLAT_POINTER_UP : PLAT_POINTER_MOVE;
                p.button = PLAT_BUTTON_LEFT;
                p.buttons = p.type == PLAT_POINTER_UP ? 0 : PLAT_BUTTON_LEFT;
                p.x = (x - dst.x) * (float)w / dst.w;
                p.y = (y - dst.y) * (float)hgt / dst.h;
                p.dx = e.tfinger.dx * (float)ow * (float)w / dst.w;
                p.dy = e.tfinger.dy * (float)oh * (float)hgt / dst.h;
                hooks->pointer(&p);
                break;
            }
            case SDL_EVENT_WILL_ENTER_BACKGROUND:
            case SDL_EVENT_WINDOW_MINIMIZED:
                if (hooks->lifecycle) hooks->lifecycle(PLAT_SUSPEND);
                break;
            case SDL_EVENT_DID_ENTER_FOREGROUND:
            case SDL_EVENT_WINDOW_RESTORED:
                if (hooks->lifecycle) hooks->lifecycle(PLAT_RESUME);
                break;
            default:
                break;
            }
        }
        hooks->scanout(pix, &w, &hgt, pal);
        if (!shot && cfg->screenshot_after_ms > 0 && SDL_GetTicks() - start >= (Uint64)cfg->screenshot_after_ms) {
            shot = 1;
            if (plat_write_png(cfg->screenshot_path ? cfg->screenshot_path : "uw2port.png", pix, w, hgt, pal) == 0)
                fprintf(stderr, "uw2port: wrote %s (%dx%d) at %lu ms\n", cfg->screenshot_path ? cfg->screenshot_path : "uw2port.png",
                        w, hgt, (unsigned long)(SDL_GetTicks() - start));
        }
        if (cfg->exit_after_ms > 0 && SDL_GetTicks() - start >= (Uint64)cfg->exit_after_ms) quit = 1;
        if (SDL_GetAtomicInt(&game_done)) quit = 1;
        if (ren) {
            if (w != tw || hgt != th) {
                if (tex) SDL_DestroyTexture(tex);
                tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, w, hgt);
                SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
                tw = w; th = hgt;
            }
            for (i = 0; i < 256; i++) {
                const uint8_t *c = pal + 3 * i;
                lut[i] = (uint32_t)((c[0] << 2) | (c[0] >> 4)) << 16 | (uint32_t)((c[1] << 2) | (c[1] >> 4)) << 8
                       | (uint32_t)((c[2] << 2) | (c[2] >> 4));
            }
            for (i = 0; i < w * hgt; i++) rgb[i] = lut[pix[i]];
            SDL_UpdateTexture(tex, NULL, rgb, w * 4);
            dst = place(ren, cfg, w, hgt);
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_RenderClear(ren);
            SDL_RenderTexture(ren, tex, NULL, &dst);
            if (shot == 1 && cfg->window_shot_path) {
                /* the window's contents as scaled, for checking the presentation */
                SDL_Surface *s = SDL_RenderReadPixels(ren, NULL), *c = s ? SDL_ConvertSurface(s, SDL_PIXELFORMAT_RGB24) : NULL;
                if (c) {
                    uint8_t *rgbp = malloc((size_t)c->w * (size_t)c->h * 3);
                    int yy;
                    for (yy = 0; rgbp && yy < c->h; yy++)
                        memcpy(rgbp + (size_t)yy * (size_t)c->w * 3, (uint8_t *)c->pixels + (size_t)yy * (size_t)c->pitch, (size_t)c->w * 3);
                    if (rgbp && plat_write_png_rgb(cfg->window_shot_path, rgbp, c->w, c->h) == 0)
                        fprintf(stderr, "uw2port: wrote %s (%dx%d, the window)\n", cfg->window_shot_path, c->w, c->h);
                    free(rgbp);
                }
                if (c) SDL_DestroySurface(c);
                if (s) SDL_DestroySurface(s);
                shot = 2;
            }
            SDL_RenderPresent(ren);
        } else {
            SDL_Delay(10);
        }
    }
    if (tex) SDL_DestroyTexture(tex);
    if (ren) SDL_DestroyRenderer(ren);
    if (win) SDL_DestroyWindow(win);
    SDL_Quit();
    return game_status;
}

/* Dialogs (plat.h): SDL's message box and folder picker, before plat_run. */
void plat_message(int error, const char *title, const char *text)
{
    fprintf(stderr, "uw2port: %s\n%s\n", title, text);
    SDL_ShowSimpleMessageBox(error ? SDL_MESSAGEBOX_ERROR : SDL_MESSAGEBOX_INFORMATION, title, text, NULL);
}

static struct { SDL_AtomicInt done; char path[1024]; int ok; } picked;

static void SDLCALL folder_cb(void *ud, const char * const *list, int filter)
{
    (void)ud; (void)filter;
    picked.ok = 0;
    if (list && list[0] && strlen(list[0]) < sizeof picked.path) {
        strcpy(picked.path, list[0]);
        picked.ok = 1;
    } else if (!list) {
        fprintf(stderr, "uw2port: no folder dialog: %s\n", SDL_GetError());
    }
    SDL_SetAtomicInt(&picked.done, 1);
}

int plat_choose_folder(const char *title, const char *text, char *out, size_t outsz)
{
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Quit" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Choose folder..." },
    };
    SDL_MessageBoxData box;
    int id = 0;
    fprintf(stderr, "uw2port: %s\n%s\n", title, text);
    memset(&box, 0, sizeof box);
    box.flags = SDL_MESSAGEBOX_INFORMATION;
    box.title = title;
    box.message = text;
    box.numbuttons = 2;
    box.buttons = buttons;
    if (!SDL_ShowMessageBox(&box, &id) || id != 1) return -1;
    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;
    SDL_SetAtomicInt(&picked.done, 0);
    SDL_ShowOpenFolderDialog(folder_cb, NULL, NULL, NULL, false);
    while (!SDL_GetAtomicInt(&picked.done)) {
        SDL_Event e;
        SDL_WaitEventTimeout(&e, 50);
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    if (!picked.ok || strlen(picked.path) >= outsz) return -1;
    strcpy(out, picked.path);
    return 0;
}

const char *plat_base_dir(void)
{
    return SDL_GetBasePath();
}
