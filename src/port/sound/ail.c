/* ail.c: replaces src/sound/AIL.ASM, the Miles Audio Interface Library 2.0 API (seg022), and
   the .ADV driver files it loads (docs/PORT.md, "Sound"). The API is a C translation of the
   matched AIL.ASM (which is John Miles' public-domain AIL.ASM of AIL 2.14): the timers, the
   driver table and the calls the game makes. The drivers are C too (xmidi.c, yamaha.c,
   mt32.c, dmasound.c); AIL_register_driver recognises which one the game loaded by the
   device names in the user's own .ADV file and reads its description table from it.

   The timers are AIL's API_timer: one PIT, programmed for the shortest period any timer
   needs, and per timer a period and an accumulator that each PIT tick adds the PIT's period
   to, firing the timer when it reaches its period (a DDA, exact on average). The ticks come
   from the port's timer thread in real time (pit.c), or, under replay, from the replayed
   game clock (port_clock_read, called by src/replay/REPLAY.C), so that the music driver's
   state follows the session's own time. Every driver entry and every tick hold one lock, as
   AIL held interrupts off. The driver writes go to the FM chips, the MT-32 and the DAC
   (audio.c) at the time of the tick that made them.

   Logs for checking the drivers (docs/PORT.md, "Sound"): --ail-log FILE writes every AIL
   call the game makes and every driver service, with the data it passed, and --hw-log FILE
   every register write and MIDI byte, both with the tick they belong to; tools/ailcheck.py
   replays an AIL log through the real .ADV driver in an x86 emulator and compares the writes. */
#include "compat.h"
#include <pthread.h>
#include <stdarg.h>
#include "sound.h"
#include "port.h"
#include "plat.h"
#include "aildrv.h"
#include "audio.h"

#define NTIMERS     17                  /* 0..15 and the BIOS's, 16 */
#define NDRIVERS    16
#define API_VERSION 0xD3

/* ---- the lock ------------------------------------------------------------------------- */

static pthread_mutex_t mx;
static pthread_once_t mx_once = PTHREAD_ONCE_INIT;

static void mx_init(void)
{
    pthread_mutexattr_t a;
    pthread_mutexattr_init(&a);
    pthread_mutexattr_settype(&a, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mx, &a);
}

static void lock(void)
{
    pthread_once(&mx_once, mx_init);
    pthread_mutex_lock(&mx);
}

static void unlock(void)
{
    pthread_mutex_unlock(&mx);
}

/* ---- the logs ------------------------------------------------------------------------- */

static FILE *ail_log_fp, *hw_log_fp;
static uint64_t vtime;                  /* the AIL clock: microseconds of PIT ticks */
static uint32_t ticks;                  /* PIT ticks since AIL_startup */

void ail_log(const char *fmt, ...)
{
    va_list ap;
    if (!ail_log_fp) return;
    va_start(ap, fmt);
    vfprintf(ail_log_fp, fmt, ap);
    va_end(ap);
}

static void log_bytes(const char *tag, const void *p, unsigned n)
{
    const uint8_t *b = p;
    unsigned i;
    if (!ail_log_fp) return;
    fprintf(ail_log_fp, "D %s %u ", tag, n);
    for (i = 0; i < n; i++) fprintf(ail_log_fp, "%02x", b[i]);
    fputc('\n', ail_log_fp);
}

void ail_set_logs(const char *ail_path, const char *hw_path)
{
    /* host files, not the game's: compat.h's fopen is Borland's, on the game's DOS paths */
    if (ail_path && !(ail_log_fp = audio_host_fopen(ail_path))) port_fatal("cannot write %s", ail_path);
    if (hw_path && !(hw_log_fp = audio_host_fopen(hw_path))) port_fatal("cannot write %s", hw_path);
}

uint64_t ail_now_us(void)
{
    return vtime;
}

/* ---- the hardware: timestamped and passed on to audio.c ------------------------------- */

void hw_opl_write(int chip, unsigned reg, unsigned val)
{
    if (hw_log_fp) fprintf(hw_log_fp, "%u O%d %03X %02X\n", ticks, chip, reg, val);
    audio_opl_write(chip, reg, val);
}

void hw_opl_mode(int kind)
{
    audio_music_device(kind);
}

void hw_mpu_byte(unsigned b)
{
    if (hw_log_fp) fprintf(hw_log_fp, "%u M %02X\n", ticks, b);
    audio_mpu_byte(b);
}

uint64_t hw_dac_play(const uint8_t *data, uint32_t n, unsigned tc, uint64_t t)
{
    uint32_t rate = 1000000u / (256u - (tc & 0xFF));
    if (hw_log_fp) fprintf(hw_log_fp, "%u D play %u tc %02X at %llu\n", ticks, n, tc, (unsigned long long)t);
    audio_dac_play(data, n, rate, t);
    return t + (uint64_t)n * 1000000u / rate;
}

void hw_dac_stop(uint64_t t)
{
    if (hw_log_fp) fprintf(hw_log_fp, "%u D stop at %llu\n", ticks, (unsigned long long)t);
    audio_dac_stop(t);
}

void hw_dac_level(unsigned l, unsigned r, uint64_t t)
{
    if (hw_log_fp) fprintf(hw_log_fp, "%u D level %u %u\n", ticks, l, r);
    audio_dac_level(l, r, t);
}

/* ---- the timers (API_timer and its DDA) ----------------------------------------------- */

static struct {
    void (*fn)(void);                   /* a game callback, or */
    int drv;                            /* a driver's service (-1 for none) */
    int state;                          /* 0 free, 1 registered and stopped, 2 running */
    uint32_t accum, period;             /* microseconds */
} tm[NTIMERS];
static uint32_t pit_period = 0xFFFFFFFFu;   /* L0114 */
static int ntimers;                         /* L0000 */
static int slaved;                          /* ticks come from the replayed clock */

static void drv_serve(int h);
static void digi_ticks(void);

static void init_DDA_arrays(void)
{
    int i;
    pit_period = 0xFFFFFFFFu;
    for (i = 0; i < NTIMERS; i++) {
        tm[i].state = 0;
        tm[i].accum = 0;
        tm[i].period = 0;
    }
}

/* program_timers: the PIT runs at the shortest period of any timer in use; a change clears
   every accumulator */
static void program_timers(void)
{
    uint32_t m = 0xFFFFFFFFu;
    int i;
    for (i = 0; i < NTIMERS; i++)
        if (tm[i].state && tm[i].period < m) m = tm[i].period;
    if (m != pit_period) {
        pit_period = m;
        for (i = 0; i < NTIMERS; i++) tm[i].accum = 0;
    }
}

/* API_timer: one PIT tick. Timer 16 is the BIOS's, which the port keeps in pit.c. */
static void api_timer(void)
{
    int i;
    if (pit_period == 0xFFFFFFFFu || pit_period == 0) return;
    vtime += pit_period;
    ticks++;
    digi_ticks();
    audio_render_to(vtime);
    for (i = 0; i < 16; i++) {
        if (tm[i].state != 2) continue;
        tm[i].accum += pit_period;
        if (tm[i].accum < tm[i].period) continue;
        tm[i].accum -= tm[i].period;
        if (tm[i].drv >= 0) {
            if (ail_log_fp) fprintf(ail_log_fp, "S %u %d\n", ticks, tm[i].drv);
            drv_serve(tm[i].drv);
        } else if (tm[i].fn)
            tm[i].fn();
    }
}

/* the timer thread, in real time: ns of host time have passed since its last call */
void ail_pit_advance(uint64_t ns)
{
    static uint64_t due;
    if (slaved) return;
    lock();
    due += ns;
    while (pit_period != 0xFFFFFFFFu && pit_period && due >= (uint64_t)pit_period * 1000u) {
        due -= (uint64_t)pit_period * 1000u;
        api_timer();
    }
    if (pit_period == 0xFFFFFFFFu) due = 0;
    unlock();
}

/* under replay: the game read the clock as t (1/256 s); the PIT has run for as long */
void port_clock_read(uint32_t t)
{
    static int started;
    static uint32_t last;
    static uint64_t target, done;
    if (!slaved) return;
    lock();
    if (!started || t < last) {
        started = 1;
        last = t;
        unlock();
        return;
    }
    target += (uint64_t)(t - last) * 1000000u / 256u;
    last = t;
    while (pit_period != 0xFFFFFFFFu && pit_period && done + pit_period <= target) {
        done += pit_period;
        api_timer();
    }
    if (pit_period == 0xFFFFFFFFu) done = target;
    unlock();
}

/* Under replay, every read of the sound hardware's state (SND_READ) gets DOS's recorded
   value; this is the value the port's own drivers had at the same read, so a replay measures
   how closely the drivers follow DOS's timing: the reads where they differ, and the clock
   ticks they spanned, are summed and printed at the end (UW2PORT_SNDCHECK=1 lists each). */
static uint32_t snd_reads, snd_diff, snd_diff_ticks, snd_last_diff_clock;
static int snd_list = -1;

static void snd_report(void)
{
    if (snd_reads)
        fprintf(stderr, "uw2port: sound reads: %u, %u where the port's drivers differed from DOS's"
                " (over %u clock ticks)\n", snd_reads, snd_diff, snd_diff_ticks);
}

void port_sound_read(unsigned own, unsigned recorded, uint32_t clock)
{
    if (snd_list < 0) {
        snd_list = getenv("UW2PORT_SNDCHECK") != NULL;
        atexit(snd_report);
    }
    snd_reads++;
    if ((own & 0xFFFF) == (recorded & 0xFFFF)) return;
    if (snd_diff == 0 || clock != snd_last_diff_clock) snd_diff_ticks++;
    snd_diff++;
    snd_last_diff_clock = clock;
    if (snd_list) fprintf(stderr, "uw2port: sound read at clock %X: the port %X, DOS %X\n", clock, own, recorded);
}

void ail_set_slaved(int on)
{
    slaved = on;
    digi_set_lenient(on);
}

int AIL_register_timer(void (*fn)(void))
{
    int i, r = -1;
    lock();
    for (i = 0; i < 16; i++)
        if (tm[i].state == 0) break;
    if (i < 16) {
        r = i;
        if (++ntimers == 1) {           /* the first: the tables, and the BIOS at 54925 us */
            init_DDA_arrays();
            tm[16].fn = 0;
            tm[16].drv = -1;
            tm[16].state = 1;
            tm[16].period = 54925;
            program_timers();
            tm[16].state = 2;
        }
        tm[i].state = 1;
        tm[i].fn = fn;
        tm[i].drv = -1;
        tm[i].period = 0xFFFFFFFFu;
    }
    ail_log("C %u register_timer %d\n", ticks, r);
    unlock();
    return r;
}

void AIL_release_timer_handle(int h)
{
    lock();
    if (h >= 0 && h < 16 && tm[h].state) {
        tm[h].state = 0;
        --ntimers;
    }
    unlock();
}

static void set_timer_period(int h, uint32_t us)
{
    int st = tm[h].state;
    tm[h].state = 1;
    tm[h].period = us;
    tm[h].accum = 0;
    program_timers();
    tm[h].state = st;
}

void AIL_set_timer_frequency(int h, uint32 hz)
{
    lock();
    if (h >= 0 && h < NTIMERS) set_timer_period(h, hz ? 1000000u / (uint32_t)hz : 0xFFFFFFFFu);
    ail_log("C %u set_timer_frequency %d %u\n", ticks, h, (unsigned)hz);
    unlock();
}

void AIL_start_timer(int h)
{
    lock();
    if (h >= 0 && h < NTIMERS && tm[h].state == 1) tm[h].state = 2;
    ail_log("C %u start_timer %d\n", ticks, h);
    unlock();
}

void AIL_stop_timer(int h)
{
    lock();
    if (h >= 0 && h < NTIMERS && tm[h].state == 2) tm[h].state = 1;
    unlock();
}

/* ---- the drivers ---------------------------------------------------------------------- */

static struct Driver {
    int kind;                           /* 0: a free handle */
    const uint8_t *image;
    struct DrvrDesc desc;               /* the game's view of its description table */
    int service_rate, timer, initialised;
    struct Xmidi *x;
    Synth *s;
    struct Digi *d;
} drv[NDRIVERS];

static int has(const uint8_t *img, size_t n, const char *s)
{
    size_t k = strlen(s), i;
    for (i = 0; i + k <= n; i++)
        if (!memcmp(img + i, s, k)) return 1;
    return 0;
}

/* Which driver an image is, by its device names; the description table (DDT) by the offset
   describe_driver loads into AX: mov dx,cs; mov cs:[device_name_s],dx; mov ax,OFFSET DDT. */
static int identify(const uint8_t *img, unsigned *ddt)
{
    unsigned table = img[0] | img[1] << 8, fn, off;
    int i;
    *ddt = 0;
    for (i = 0; i < 64; i++) {
        fn = img[table + 4 * i] | img[table + 4 * i + 1] << 8;
        off = img[table + 4 * i + 2] | img[table + 4 * i + 3] << 8;
        if (fn == 0xFFFF) break;
        if (fn == 0x64) {
            const uint8_t *p = img + off;
            int k;
            for (k = 0; k < 24; k++)
                if (p[k] == 0x8C && p[k + 1] == 0xCA && p[k + 7] == 0xB8) {
                    *ddt = p[k + 8] | p[k + 9] << 8;
                    break;
                }
        }
    }
    if (!*ddt) return DRV_NONE;
    if (has(img, 0x400, "Digital Sound")) {
        if (has(img, 0x400, "Sound Blaster Pro")) return DRV_DIG_SBPRO;
        if (has(img, 0x400, "Pro Audio Spectrum")) return DRV_DIG_PAS;
        return DRV_DIG_SB;
    }
    if (has(img, 0x400, "Roland MT-32")) return DRV_MT32;
    if (has(img, 0x400, "internal speaker")) return DRV_SPKR;
    if (has(img, 0x400, "Ad Lib(R) Music")) return DRV_ADLIB;
    if (has(img, 0x400, "Pro Audio Spectrum")) return DRV_PASFM;
    if (has(img, 0x400, "Sound Blaster Pro")) return !memcmp(img + *ddt + 4, "OPL", 3) ? DRV_SBPRO2 : DRV_SBPRO1;
    if (has(img, 0x400, "Sound Blaster(TM) FM")) return DRV_SBFM;
    return DRV_NONE;
}

static uint16_t rd16(const void *p)
{
    const uint8_t *b = p;
    return (uint16_t)(b[0] | b[1] << 8);
}

/* the PC speaker driver: an XMIDI shell with nothing to sound (the port has no speaker) */
static void null_v(Synth *s) { (void)s; }
static void null_send(Synth *s, unsigned a, unsigned b, unsigned c) { (void)s; (void)a; (void)b; (void)c; }
static unsigned null_size(Synth *s) { (void)s; return 0; }
static void null_cache(Synth *s, uint8_t *a, unsigned n) { (void)s; (void)a; (void)n; }
static int null_index(Synth *s, unsigned g) { (void)s; (void)g; return -1; }
static void null_install(Synth *s, unsigned b, unsigned n, const uint8_t *a) { (void)s; (void)b; (void)n; (void)a; }
static void null_protect(Synth *s, unsigned b, unsigned n, int on) { (void)s; (void)b; (void)n; (void)on; }
static unsigned null_status(Synth *s, unsigned b, unsigned n) { (void)s; (void)b; (void)n; return 1; }

static Synth *null_synth(void)
{
    Synth *s = calloc(1, sizeof *s);
    s->min_true_chan = 2;
    s->max_true_chan = 9;
    s->max_rec_chan = 10;
    s->def_synth_vol = 100;
    s->reset = s->init = null_v;
    s->send = null_send;
    s->cache_size = null_size;
    s->define_cache = null_cache;
    s->index_timbre = null_index;
    s->install = null_install;
    s->protect = null_protect;
    s->timbre_status = null_status;
    return s;
}

static struct Driver *D(int h)
{
    return h >= 0 && h < NDRIVERS && drv[h].kind ? &drv[h] : 0;
}

static void digi_ticks(void)
{
    int i;
    for (i = 0; i < NDRIVERS; i++)
        if (drv[i].kind && drv[i].d && drv[i].initialised) digi_tick(drv[i].d);
}

static void drv_serve(int h)
{
    struct Driver *d = D(h);
    if (d && d->x) xmidi_serve(d->x);
}

void AIL_startup(void)
{
    static int audio_open;
    lock();
    ntimers = 0;
    memset(drv, 0, sizeof drv);
    if (!audio_open) {
        audio_open = 1;
        audio_start();
    }
    ail_log("C %u startup\n", ticks);
    unlock();
}

void AIL_shutdown_driver(int h, char *msg);

void AIL_shutdown(char *msg)
{
    int i;
    lock();
    for (i = 0; i < NDRIVERS; i++) {
        if (!drv[i].kind) continue;
        if (drv[i].timer != -1) AIL_release_timer_handle(drv[i].timer);
        AIL_shutdown_driver(i, msg);
    }
    for (i = 15; i >= 0; i--) AIL_release_timer_handle(i);
    audio_flush();
    unlock();
}

int AIL_register_driver(void *image)
{
    const uint8_t *img = image;
    unsigned ddt;
    int h, kind;
    lock();
    for (h = 0; h < NDRIVERS && drv[h].kind; h++)
        ;
    if (h == NDRIVERS || memcmp(img + 2, "Copy", 4) || !(kind = identify(img, &ddt))) {
        ail_log("C %u register_driver -1\n", ticks);
        unlock();
        return -1;
    }
    memset(&drv[h], 0, sizeof drv[h]);
    drv[h].kind = kind;
    drv[h].image = img;
    drv[h].timer = -1;
    drv[h].desc.min_api = rd16(img + ddt);
    drv[h].desc.drvr_type = rd16(img + ddt + 2);
    memcpy(drv[h].desc.data_suffix, img + ddt + 4, 4);
    drv[h].desc.dev_names = (char *)img + rd16(img + ddt + 8);
    drv[h].desc.io = (int16)rd16(img + ddt + 0x0C);
    drv[h].desc.irq = (int16)rd16(img + ddt + 0x0E);
    drv[h].desc.dma = (int16)rd16(img + ddt + 0x10);
    drv[h].desc.drq = (int16)rd16(img + ddt + 0x12);
    drv[h].service_rate = (int16)rd16(img + ddt + 0x14);
    if (kind >= DRV_DIG_SB)
        drv[h].d = digi_new(kind);
    else {
        drv[h].s = kind == DRV_MT32 ? mt32_new() : kind == DRV_SPKR ? null_synth() : yamaha_new(kind);
        drv[h].x = xmidi_new(drv[h].s);
    }
    ail_log("C %u register_driver %d kind %d\n", ticks, h, kind);
    if (drv[h].desc.min_api > API_VERSION) h = -1;
    unlock();
    return h;
}

struct DrvrDesc *AIL_describe_driver(int h)
{
    struct Driver *d = D(h);
    return d ? &d->desc : 0;
}

int AIL_detect_device(int h, int io, int irq, int dma, int drq)
{
    (void)io; (void)irq; (void)dma; (void)drq;
    return D(h) ? 1 : 0;
}

void AIL_init_driver(int h, int io, int irq, int dma, int drq)
{
    struct Driver *d = D(h);
    if (!d) return;
    lock();
    ail_log("C %u init_driver %d %x %d %d %d\n", ticks, h, io & 0xFFFF, irq, dma, drq);
    d->timer = -1;
    if (d->service_rate != -1 && d->x) {
        d->timer = AIL_register_timer(0);
        if (d->timer != -1) {
            tm[d->timer].drv = h;
            AIL_set_timer_frequency(d->timer, (uint32)d->service_rate);
        }
    }
    if (d->x) xmidi_init_driver(d->x);
    else digi_init(d->d);
    d->initialised = 1;
    if (d->timer != -1) AIL_start_timer(d->timer);
    unlock();
}

void AIL_shutdown_driver(int h, char *msg)
{
    struct Driver *d = D(h);
    (void)msg;
    if (!d || !d->initialised) return;
    lock();
    ail_log("C %u shutdown_driver %d\n", ticks, h);
    d->initialised = 0;
    if (d->timer != -1) AIL_release_timer_handle(d->timer);
    if (d->x) xmidi_shutdown_driver(d->x);
    else digi_shutdown(d->d);
    unlock();
}

/* ---- XMIDI calls: a driver without the function returns 0, as call_driver does -------- */

#define XCALL(h, expr, dflt) do { struct Driver *d_ = D(h); if (!d_ || !d_->x) return dflt; \
    lock(); expr; unlock(); } while (0)

unsigned AIL_state_table_size(int h)
{
    unsigned r = 0;
    XCALL(h, r = xmidi_state_size(d_->x), 0);
    return r;
}

int AIL_register_sequence(int h, void *xmid, int n, void *state, void *ctrl)
{
    int r;
    struct Driver *d = D(h);
    if (!d || !d->x) return 0;
    lock();
    log_bytes("xmid", xmid, 9000);
    r = xmidi_register_seq(d->x, xmid, n, state, ctrl);
    ail_log("C %u register_sequence %d %d -> %d\n", ticks, h, n, r);
    unlock();
    return r;
}

void AIL_release_sequence_handle(int h, int seq)
{
    XCALL(h, (ail_log("C %u release_sequence %d %d\n", ticks, h, seq), xmidi_release_seq(d_->x, seq)), );
}

void AIL_start_sequence(int h, int seq)
{
    XCALL(h, (ail_log("C %u start_sequence %d %d\n", ticks, h, seq), xmidi_start_seq(d_->x, seq)), );
}

void AIL_stop_sequence(int h, int seq)
{
    XCALL(h, (ail_log("C %u stop_sequence %d %d\n", ticks, h, seq), xmidi_stop_seq(d_->x, seq)), );
}

unsigned AIL_sequence_status(int h, int seq)
{
    unsigned r = 0;
    XCALL(h, r = xmidi_seq_status(d_->x, seq), 0);
    return r;
}

void AIL_set_relative_tempo(int h, int seq, int percent, int ms)
{
    XCALL(h, (ail_log("C %u set_relative_tempo %d %d %d %d\n", ticks, h, seq, percent, ms),
              xmidi_set_rel_tempo(d_->x, seq, (unsigned)percent, (unsigned)ms)), );
}

void AIL_set_relative_volume(int h, int seq, int percent, int ms)
{
    XCALL(h, (ail_log("C %u set_relative_volume %d %d %d %d\n", ticks, h, seq, percent, ms),
              xmidi_set_rel_volume(d_->x, seq, (unsigned)percent, (unsigned)ms)), );
}

void AIL_send_channel_voice_message(int h, int status, int d1, int d2)
{
    XCALL(h, (ail_log("C %u send_cv %d %x %x %x\n", ticks, h, status & 0xFF, d1 & 0xFF, d2 & 0xFF),
              xmidi_send_cv(d_->x, (unsigned)status, (unsigned)d1, (unsigned)d2)), );
}

int AIL_lock_channel(int h)
{
    int r = 0;
    XCALL(h, (r = (int)xmidi_lock_channel(d_->x), ail_log("C %u lock_channel %d -> %d\n", ticks, h, r)), 0);
    return r;
}

void AIL_release_channel(int h, int ch)
{
    XCALL(h, (ail_log("C %u release_channel %d %d\n", ticks, h, ch), xmidi_release_channel(d_->x, ch)), );
}

unsigned AIL_default_timbre_cache_size(int h)
{
    unsigned r = 0;
    XCALL(h, r = d_->s->cache_size(d_->s), 0);
    return r;
}

void AIL_define_timbre_cache(int h, void *cache, unsigned size)
{
    XCALL(h, (ail_log("C %u define_timbre_cache %d %u\n", ticks, h, size),
              d_->s->define_cache(d_->s, cache, size)), );
}

unsigned AIL_timbre_request(int h, int seq)
{
    unsigned r = 0;
    XCALL(h, (r = xmidi_timbre_request(d_->x, seq), ail_log("C %u timbre_request %d %d -> %x\n", ticks, h, seq, r)), 0);
    return r;
}

void AIL_install_timbre(int h, int bank, int patch, void *src)
{
    XCALL(h, (src ? log_bytes("timbre", src, rd16(src)) : (void)0,
              ail_log("C %u install_timbre %d %d %d\n", ticks, h, bank & 0xFF, patch & 0xFF),
              d_->s->install(d_->s, (unsigned)bank, (unsigned)patch, src)), );
}

int AIL_timbre_status(int h, int bank, int patch)
{
    int r = 0;
    XCALL(h, (r = (int)d_->s->timbre_status(d_->s, (unsigned)bank, (unsigned)patch),
              ail_log("C %u timbre_status %d %d %d -> %x\n", ticks, h, bank & 0xFF, patch & 0xFF, r)), 0);
    return r;
}

/* ---- digital calls -------------------------------------------------------------------- */

#define DCALL(h, expr, dflt) do { struct Driver *d_ = D(h); if (!d_ || !d_->d) return dflt; \
    lock(); expr; unlock(); } while (0)

int AIL_index_VOC_block(int h, void *voc, int block, struct SoundBuff *buf)
{
    struct SoundBuffDesc o;
    int r = 0;
    DCALL(h, (r = digi_index_voc(d_->d, voc, block, &o),
              ail_log("C %u index_VOC_block %d %d -> %d\n", ticks, h, block, r)), 0);
    if (r) {
        buf->pack_type = (uint16)o.pack_type;
        buf->sample_rate = (uint16)o.sample_rate;
        buf->data = (char *)o.data;
        buf->len = o.len;
    }
    return r;
}

void AIL_register_sound_buffer(int h, int n, struct SoundBuff *buf)
{
    struct SoundBuffDesc o;
    o.pack_type = buf->pack_type;
    o.sample_rate = buf->sample_rate;
    o.data = (const uint8_t *)buf->data;
    o.len = buf->len;
    DCALL(h, (ail_log("C %u register_sound_buffer %d %d %u\n", ticks, h, n, (unsigned)o.len),
              digi_register_sb(d_->d, n, &o)), );
}

unsigned AIL_sound_buffer_status(int h, int n)
{
    unsigned r = 0;
    DCALL(h, r = digi_sb_status(d_->d, n), 0);
    return r;
}

void AIL_start_digital_playback(int h) { DCALL(h, (ail_log("C %u start_digital %d\n", ticks, h), digi_start(d_->d)), ); }
void AIL_stop_digital_playback(int h) { DCALL(h, (ail_log("C %u stop_digital %d\n", ticks, h), digi_stop(d_->d)), ); }
void AIL_pause_digital_playback(int h) { DCALL(h, (ail_log("C %u pause_digital %d\n", ticks, h), digi_pause(d_->d)), ); }
void AIL_resume_digital_playback(int h) { DCALL(h, (ail_log("C %u resume_digital %d\n", ticks, h), digi_resume(d_->d)), ); }
void AIL_set_digital_playback_volume(int h, int v)
{
    DCALL(h, (ail_log("C %u digital_volume %d %d\n", ticks, h, v), digi_set_volume(d_->d, (unsigned)v)), );
}
void AIL_set_digital_playback_panpot(int h, int p)
{
    DCALL(h, (ail_log("C %u digital_panpot %d %d\n", ticks, h, p), digi_set_pan(d_->d, (unsigned)p)), );
}
