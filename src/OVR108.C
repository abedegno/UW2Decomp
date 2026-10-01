/* target: ovr108 */
/* opts: -mm -1 -G -O -Y -d */
/* The cutscene player: speech streamed from SOUND\BSPnn.VOC through EMS, the CUTS\csXXX.nXX
   Deluxe Paint LPF animations and their opcode scripts, palette fades and colour cycling run
   as time-based tasks, subtitles, and the LBACK backgrounds. The whole of DOS overlay ovr108,
   in original order.

   Names are the FM Towns originals where it has the function, matched by position among the
   neighbours, by the opcode table (_cuts_dispatch gives each cutsop_ its number) and by the
   same callees and globals. FM Towns has no counterpart of ovr108_671, OpenCutsFile_ovr108_2EAC,
   ovr108_3200, ovr108_3333 or ovr108_3620, which keep the IDA names; it has get_token_,
   draw_into_buffer_ and do_update_ where DOS has them inside draw_rsd and cuts_process_lp.
   virtual_screen and lback_vscreen sit elsewhere in FM Towns but here in DOS. */

#include <dos.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stat.h>

/* AIL's sound buffer, 12 bytes (SEG016.C). */
struct SoundBuff { unsigned pack_type, sample_rate; char far *data; unsigned long len; };

/* A colour cycle of the LPF header, 8 bytes, 16 of them at +0x80. */
struct Cycle { unsigned started, period; char pad4[2]; unsigned char first, last; };

/* The state of a cutscene, on show_anm's stack, 0x5C bytes. */
struct CutsState {
    char name[0x13];                    /* CUTS\csXXX.nXX */
    int x, y, w, h;                     /* 0x13, the window; w and h are 320 and 200 for full screen */
    unsigned char windowed;             /* 0x1B */
    unsigned char far *palette;         /* 0x1C */
    char far *text_lines[6];            /* 0x20 */
    unsigned char color38;              /* 0x38 */
    int flag39;                         /* 0x39, subtitle lines to draw */
    int frame3B, frame3D;               /* 0x3B */
    unsigned frame3F;                   /* 0x3F, pause length */
    unsigned repeat41;                  /* 0x41, loops left */
    int repeat43;                       /* 0x43 */
    int fade45, fade47, fade49;         /* 0x45, speech playing, fade in, fade out */
    int file4B, file4D;                 /* 0x4B */
    int vscr4F;                         /* 0x4F */
    int panx51, pany53, dir55, step57, remaining59; /* 0x51 */
    union {
        unsigned char value;
        struct { unsigned b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1; } bit;
    } flags;                            /* 0x5B */
};

/* The cutscene player's far work area (seg049). */
struct Stdat {
    int n0x_fd, pad2;                   /* 0x00, the opcode file */
    int anm_fd, pad6;                   /* 0x04, the LPF file */
    int ems_page, padA;                 /* 0x08 */
    int lp_index, padE;                 /* 0x0C */
    int ahead_page, pad12;              /* 0x10 */
    int ahead_index, pad16;             /* 0x14 */
    int frame, pad1A;                   /* 0x18 */
    unsigned far *code;                 /* 0x1C, the next opcode */
    unsigned char far *n00;             /* 0x20, the opcode buffer, then the LPF header */
    unsigned char far *screen;          /* 0x24 */
    unsigned char far *lptab;           /* 0x28 */
    unsigned long start;                /* 0x2C */
    unsigned long tick;                 /* 0x30 */
    char pad34[4];
    unsigned char n00buf[0x400];        /* 0x38 */
};

/* The LPF file's header as read_anmhdr leaves it, 0xB00 bytes: colour cycles at 0x80, the
   palette at 0x100, the large page descriptors at 0x500. */
struct AnmHdr {
    char pad0[6];
    unsigned nlps;                      /* 0x06, large pages */
    char pad8[0x1A - 8];
    unsigned char lastdelta;            /* 0x1A */
    char pad1B[0x44 - 0x1B];
    unsigned rate;                      /* 0x44, frames per second */
};
/* Large page n's descriptor: its first record, its records and its bytes. */
#define LPDESC(anm, n) ((unsigned far *)((unsigned char far *)(anm) + 0x500 + (n) * 6))

struct FontInfo { char pad0[6]; int height; };
struct Inplist { char pad0[8]; int cmd; };

typedef int (far *CutsOp)(unsigned far *code, struct CutsState *st);
typedef void (far *Task)(int task, int done);

extern struct Stdat far stdat;

/* Sound (SEG016.C) and AIL. */
extern unsigned char speechok;
extern int sphdriver;
extern char far *dsdata[2];
extern struct SoundBuff dsbuf[2];
void far kill_all_digi_effects(void);
void far punt_all_digi_fx(void);
void far free_speech_stuff(void);
void far stop_music(void);
void far set_new_music(unsigned char);
void far load_new_music(int, int);
void far change_music_maybe(void);
unsigned char far speech_available(void);
unsigned far AIL_sound_buffer_status(int driver, int buffer);
void far AIL_start_digital_playback(int driver);
void far AIL_stop_digital_playback(int driver);
int far AIL_index_VOC_block(int drv, void far *voc, int block, struct SoundBuff far *buf);
void far AIL_register_sound_buffer(int drv, int n, struct SoundBuff far *buf);
void far AIL_set_digital_playback_volume(int drv, int v);
void far AIL_set_digital_playback_panpot(int drv, int p);

/* EMS and files. */
extern char ws_active;
extern unsigned char far obj_inpage1;
extern unsigned char far tmap_fpage;
extern unsigned far EmsBuff;
extern unsigned ems_frame;
extern unsigned char far sound_fpage;     /* DS:34AA: the first EMS page of the sounds */
/* The first EMS page of the speech being streamed: the one-byte far variable at 6388:0000.
   FM Towns has it as a static (_task_sofar+0x22, beside sp_npages and audio_inpage at +0x20
   and +0x21), so the name is provisional. cutsop_say passes sound_fpage instead, as FM
   Towns' cutsop_say_ does (_sound_fpage). */
extern unsigned char far speech_fpage;
extern char far dfx_buffer[];         /* the digital effects' buffer (SEG016.C) */
void far MapMemory_seg013_1D3C_C7(int phys, int page);
unsigned char far seg013_1D3C_E4(int bank, int page, int count);
void far seg042_35ED_12B(void);
int far ReadFileToAddress(int fd, void far *buf, unsigned n);
unsigned char far LoadDATFile(char *name, void far *buf, unsigned n);

/* Graphics. */
extern unsigned long far *Time;
extern unsigned char far *palette;
extern unsigned char far *foreground_color;
extern unsigned char far *background_color;
extern struct FontInfo far *cur_font;
extern unsigned far *Ytab;
extern unsigned char far ShowClip;
extern unsigned char far Transparency;
void far rotate_bank(unsigned char first, unsigned char count, int unused);
void far local_do_palette(int count, unsigned char first);
void far set_the_color(int);
void far urectangle(int, int, int, int);
void far rectangle(int, int, int, int);
void far set_the_window(int, int, int, int);
void far show(int x, int y, unsigned char far *buf, int h, int w, int skipx, int skipy);
void far grSoftPageFlip(void);
void far grPageFlip(void);
void far copy_visible_to_hidden(void);
void far vscreen_focus(int, int);
void far seg003_0272_4A3A(int, int, int);
int far string_width(char far *);
void far string_to_screen(char far *, int, int);
void far mem_set(void far *p, int value, int count);
void far fadein(unsigned char far *, int, int);
void far fadeout(unsigned char far *, int, int);
void far grfx_setpal(unsigned char far *);
void far grfx_clear(void);
void far grfx_quikpal(int);
void far grfx_quikfont(int);
unsigned char far read_quikpal(int which, unsigned char far *pal);
void far grfx_palrange(unsigned char far *pal, int first, int count);

/* Elsewhere. */
extern unsigned char in_game;
extern int CutsceneOrConversationStringBlock;
extern struct Inplist *inplist;
char far * far get_string(int);
char far * far FindStringDelimiter(char far *, int);
int far mouse_get_input(void);
void far mouse_hide(void);
void far mouse_show(void);
void far editchng(int);
void far newscr(int);
void far load_txtmaps(void);

#define SPEECH_BUF(o) MK_FP(EmsBuff + 0x800, (o))
#define RESTORE_EMS() \
    if (ws_active) seg013_1D3C_E4(0, 0, 4); \
    else if (obj_inpage1 != 0xFF) MapMemory_seg013_1D3C_C7(2, obj_inpage1)

/* This file's functions referenced before their definitions. */
int far cutsop_txt(unsigned far *, struct CutsState *);
int far cutsop_erase(unsigned far *, struct CutsState *);
int far cutsop_func(unsigned far *, struct CutsState *);
int far cutsop_pause(unsigned far *, struct CutsState *);
int far cutsop_skip(unsigned far *, struct CutsState *);
int far cutsop_next(unsigned far *, struct CutsState *);
int far cutsop_end(unsigned far *, struct CutsState *);
int far cutsop_loop(unsigned far *, struct CutsState *);
int far cutsop_data(unsigned far *, struct CutsState *);
int far cutsop_fadeout(unsigned far *, struct CutsState *);
int far cutsop_fadein(unsigned far *, struct CutsState *);
int far cutsop_jump(unsigned far *, struct CutsState *);
int far cutsop_punt(unsigned far *, struct CutsState *);
int far cutsop_say(unsigned far *, struct CutsState *);
int far cutsop_wait(unsigned far *, struct CutsState *);
int far cutsop_clang(unsigned far *, struct CutsState *);
int far cutsop_palrange(unsigned far *, struct CutsState *);
int far cutsop_palfade(unsigned far *, struct CutsState *);
int far cutsop_palset(unsigned far *, struct CutsState *);
int far cutsop_palsimplefade(unsigned far *, struct CutsState *);
int far cutsop_vscreen(unsigned far *, struct CutsState *);
int far cutsop_focus(unsigned far *, struct CutsState *);
int far cutsop_lback(unsigned far *, struct CutsState *);
int far cutsop_pan(unsigned far *, struct CutsState *);
int far cutsop_splity(unsigned far *, struct CutsState *);
int far cutsop_music(unsigned far *, struct CutsState *);
int far cutsop_test(unsigned far *, struct CutsState *);
int far cutsop_wait_for_sound(unsigned far *, struct CutsState *);
int far gobble_input_events(struct CutsState *);
void far run_timebased_tasks(int reset);
void far punt_tasks(void);
void far punt_single_task(int task);
int far install_timebased_task(Task fn, int period, int total);
void far task_palfade(int task, int done);
void far remove_task(int task);
void far palette_fade(int step, int total, int first, int last, unsigned char far *pal);
int far virtual_screen(int w, int h, int split);
int far lback_vscreen(int x, int y, unsigned n);
void far show_anm(int cuts, int x, int y, int w, int h);

/* Initialised data, DS:1064 to DS:1141. */
static int lp_page = -1;                /* the LPF page readlpinc is reading */
static int lp_left = -1;                /* and the bytes of it still to read */
int pan_dx[4] = { 0, 1, 0, -1 };
int pan_dy[4] = { 1, 0, -1, 0 };
int focus_x = 0;
int focus_y = 0xC7;
int splity = -1;
CutsOp cuts_dispatch[29] = {
    cutsop_txt, cutsop_erase, cutsop_func, cutsop_pause, cutsop_skip, cutsop_next,
    cutsop_end, cutsop_loop, cutsop_data, cutsop_fadeout, cutsop_fadein, cutsop_jump,
    cutsop_punt, cutsop_say, cutsop_wait, cutsop_clang, cutsop_palrange, cutsop_palfade,
    cutsop_palset, cutsop_palsimplefade, cutsop_vscreen, cutsop_focus, cutsop_lback,
    cutsop_pan, cutsop_splity, cutsop_music, cutsop_test, cutsop_wait_for_sound, 0
};
unsigned char lmask[4] = { 0x0F, 0x0E, 0x0C, 0x08 };    /* planes from the first pixel on */
unsigned char rmask[4] = { 0x01, 0x03, 0x07, 0x0F };    /* planes up to the last pixel */

/* Uninitialised data, DS:5550 to DS:5D1B. Turbo C lays _BSS out by a hash of the names
   (tools/bssorder.py). The publics are the FM Towns names; FM Towns has no names for the
   statics, so theirs are ours, chosen to land where the EXE has them. */
static unsigned long tclock;            /* DS:5550 (12) */
static unsigned long prev_time;         /* DS:5554 (24) */
static int prevunused;                  /* DS:5558 (24), never referenced */
int task_residue[16];                   /* DS:555A (28) */
static long voc_left;                   /* DS:557A (166), speech bytes not yet queued */
unsigned char far *task_pointer_data[16];   /* DS:557E (340) */
static long sp_pos;                     /* DS:55BE (363), the next chunk's place in the file */
static long sp_nread;                   /* DS:55C2 (379), bytes read into EMS */
static long voc_length;                 /* DS:55C6 (406) */
unsigned char start_pal[0x300];         /* DS:55CA (443) */
static unsigned char sp_npages;         /* DS:58CA (475), EMS pages holding speech */
Task tasks[16];                         /* DS:58CC (524) */
int pan_dir;                            /* DS:590C (632) */
static int audio_fd;                    /* DS:590E (657) */
int pan_step;                           /* DS:5910 (664) */
int num_buf;                            /* DS:5912 (726) */
int task_sofar[16];                     /* DS:5914 (764) */
int task_total[16];                     /* DS:5934 (764) */
int task_data1[16];                     /* DS:5954 (764) */
int task_data2[16];                     /* DS:5974 (764) */
static unsigned long task_time[16];     /* DS:5994 (796) */
int task_flags[16];                     /* DS:59D4 (812) */
unsigned char end_pal[0x300];           /* DS:59F4 (813) */
static int cuts_number;                 /* DS:5CF4 (843) */
static unsigned char audio_inpage;      /* DS:5CF6 (921), the speech page last read */
int logw;                               /* DS:5CF8 (932) */
int logh;                               /* DS:5CFA (932) */
int task_period[16];                    /* DS:5CFC (940) */

/* 0x0 */
unsigned char far big_speech_play(unsigned char file, unsigned char volume,
    unsigned char pan, unsigned char page, unsigned char pages)
{
    char num[4];
    long amount;
    char name[80];
    struct stat sb;
    register int i, buf;

    if (!speechok) return 0xFF;
    if (pages < 1) return 0xFF;
    if (audio_fd != -1) close(audio_fd);
    audio_fd = -1;
    strcpy(name, "SOUND\\");
    strcat(name, "BSP");
    if (file < 10) strcat(name, "0");
    strcat(name, itoa(file, num, 10));
    strcat(name, ".VOC");
    if ((audio_fd = open(name, 0x8001)) == -1) goto fail;
    if (fstat(audio_fd, &sb) == -1) goto fail;
    voc_length = sb.st_size;
    speech_fpage = page;
    sp_npages = pages;
    sp_nread = 0;
    for (i = 0; i < pages; i++) {
        MapMemory_seg013_1D3C_C7(2, page + i);
        sp_nread += ReadFileToAddress(audio_fd, SPEECH_BUF(0), 0x4000);
        RESTORE_EMS();
        if (eof(audio_fd)) {
            close(audio_fd);
            audio_fd = -1;
            break;
        }
    }
    voc_left = voc_length;
    audio_inpage = 0;
    buf = 0;
    MapMemory_seg013_1D3C_C7(2, page);
    amount = voc_left > 0x800 ? 0x800 : voc_left;
    movedata(FP_SEG(SPEECH_BUF(0)), FP_OFF(SPEECH_BUF(0)), FP_SEG(dsdata[buf]),
        FP_OFF(dsdata[buf]), amount);
    RESTORE_EMS();
    if (!AIL_index_VOC_block(sphdriver, dsdata[buf], -1, &dsbuf[buf])) goto fail_sound;
    dsbuf[1] = dsbuf[0];
    voc_left -= amount + 0x20;
    sp_pos = amount;
    dsbuf[buf].len = amount - 0x20;
    AIL_register_sound_buffer(sphdriver, buf, &dsbuf[buf]);
    AIL_set_digital_playback_volume(sphdriver, volume);
    AIL_set_digital_playback_panpot(sphdriver, pan);
    return 100;
fail_sound:
    speechok = 0;
    kill_all_digi_effects();
fail:
    close(audio_fd);
    return 0xFF;
}

/* 0x32F */
void far update_big_speech(void)
{
    long amount;
    unsigned char page;
    char done[2];
    long off;
    register int i;

    done[0] = done[1] = 0;
    for (i = 0; i < 2; i++) {
        if (AIL_sound_buffer_status(sphdriver, i) == 3 && voc_left > 0) {
            done[i] = 1;
            off = sp_pos & 0x3FFF;
            page = sp_pos >> 14;
            if (page != audio_inpage) {
                if (sp_nread < voc_length) {
                    MapMemory_seg013_1D3C_C7(2, speech_fpage + audio_inpage % sp_npages);
                    sp_nread += ReadFileToAddress(audio_fd, SPEECH_BUF(0), 0x4000);
                    RESTORE_EMS();
                    if (sp_nread >= voc_length) {
                        close(audio_fd);
                        audio_fd = -1;
                    }
                }
                audio_inpage = page;
            }
            page = page % sp_npages;
            MapMemory_seg013_1D3C_C7(2, speech_fpage + page);
            amount = voc_left > 0x800 ? 0x800 : voc_left;
            movedata(FP_SEG(SPEECH_BUF(off)), FP_OFF(SPEECH_BUF(off)), FP_SEG(dsdata[i]),
                FP_OFF(dsdata[i]), amount);
            RESTORE_EMS();
            voc_left -= amount;
            sp_pos += amount;
            dsbuf[i].data = dsdata[i];
            dsbuf[i].len = amount;
            AIL_register_sound_buffer(sphdriver, i, &dsbuf[i]);
        }
    }
    AIL_start_digital_playback(sphdriver);
}

/* 0x5A8 */
unsigned char far speech_over(void)
{
    int a, b;
    if (!speechok) return 1;
    a = AIL_sound_buffer_status(sphdriver, 0);
    b = AIL_sound_buffer_status(sphdriver, 1);
    if (a == 3 && b == 3 && voc_left == 0) return 1;
    return 0;
}

/* 0x5F7 */
void far cutoff_cutscene_speech(void)
{
    voc_left = 0;
    AIL_stop_digital_playback(sphdriver);
}

/* 0x613 */
int far get_cuts_ems(void)
{
    seg042_35ED_12B();
    return num_buf = 3;
}

/* 0x623 */
unsigned char far * far set_cuts_ems(int which)
{
    seg042_35ED_12B();
    if (num_buf - 1 < which) return 0L;
    if (seg013_1D3C_E4(0, tmap_fpage + (which << 2), 4))
        return MK_FP(ems_frame, 0);
    return 0L;
}

/* 0x667 */
void far free_cuts_ems(void)
{
    seg042_35ED_12B();
}

/* 0x671 */
char * far ovr108_671(unsigned long value, char *out)
{
    int i;
    for (i = 0; i < 4; i++) out[i] = (value >> (i * 8)) & 0xFF;
    out[4] = 0;
    return out;
}

/* 0x6AD */
void far build_lptab(unsigned far *desc, unsigned n, unsigned char far *order)
{
    int tmp;
    unsigned char changed;
    int i;
    changed = 0;
    for (i = 0; i < n; i++) order[i] = i;
    do {
        changed = 1;
        for (i = 1; i < n; i++) {
            if (desc[order[i - 1] * 3] > desc[order[i] * 3]) {
                changed = 0;
                tmp = order[i - 1];
                order[i - 1] = order[i];
                order[i] = tmp;
            }
        }
    } while (!changed);
}

/* 0x751 */
unsigned char far read_anmhdr(unsigned char far *dst)
{
    int wanted, actual;
    int i;
    unsigned value;
    wanted = 0xB00;
    actual = ReadFileToAddress(stdat.anm_fd, dst, wanted);
    for (i = 0; i < 16; i++) {
        value = *(unsigned far *)(dst + 0x82 + i * 8);
        *(unsigned far *)(dst + 0x82 + i * 8) = (value >> 8) & 0x3F;
    }
    return actual == wanted;
}

/* 0x7BF */
int far readlp(unsigned page, unsigned far *desc, void far *dst)
{
    int size;
    register int got;
    lseek(stdat.anm_fd, ((long)page << 16) + 0xB00L, 0);
    size = desc[2] + desc[1] * 2 + 8;
    got = ReadFileToAddress(stdat.anm_fd, dst, size);
}

/* 0x819 */
int far readlpinc(unsigned page, unsigned far *desc, unsigned n, void far *dst)
{
    char far *target = dst;
    int out_size, amount;
    if (page < 0 || n == 0) return 0;
    out_size = desc[1] * 2 + desc[2] + 8;
    if (page != lp_page) {
        lp_page = page;
        lseek(stdat.anm_fd, ((long)page << 16) + 0xB00L, 0);
        lp_left = out_size;
    }
    if (lp_left == 0) return 0;
    if (n > lp_left) n = lp_left;
    amount = ReadFileToAddress(stdat.anm_fd,
             target + out_size - lp_left, n);
    lp_left -= amount;
    return amount;
}

/* 0x8C8 */
void far conv_anmpal(unsigned char far *src, unsigned char far *dst)
{
    unsigned char far *pal;
    unsigned char far *output;
    int i;
    pal = src;
    output = dst;
    for (i = 0; i < 256; i++) {
        output[0] = pal[2] >> 2;
        output[1] = pal[1] >> 2;
        output[2] = pal[0] >> 2;
        pal += 4;
        output += 3;
    }
}

/* 0x934 */
void far anm_cycle(struct Cycle far *cycles)
{
    int i;
    unsigned char n;
    for (i = 0; i < 16; i++) {
        if (cycles[i].period != 0) {
            if ((unsigned)(*Time - cycles[i].started) <
                0x38E / cycles[i].period) continue;
            n = cycles[i].last - cycles[i].first + 1;
            rotate_bank(cycles[i].first, n, 0);
            local_do_palette(n, cycles[i].first);
            cycles[i].started = *Time;
        }
    }
}

/* 0x9FE, opcode 27 */
int far cutsop_wait_for_sound(unsigned far *code, struct CutsState *st)
{
    unsigned long until;
    until = *Time + (*code << 8);
    while (!speech_over()) {
        if (*Time >= until) {
            cutoff_cutscene_speech();
            break;
        }
        update_big_speech();
    }
    return 1;
}

/* 0xA55 */
void far cuts_perform_fadein(struct CutsState *st)
{
    set_the_color(0xF0);
    urectangle(0, splity, 0x13F, 0);
    if (st->fade47 > -1) {
        if (st->flags.bit.b6 && st->fade45 != -1)
            fadein(st->palette, st->fade47, 1);
        else fadein(st->palette, st->fade47, 0);
        gobble_input_events(st);
        st->flags.bit.b1 = 0;
        st->flags.bit.b0 = 0;
        st->fade47 = -2;
        st->fade49 = -1;
    }
}

/* 0xAC6 */
void far cuts_perform_fadeout(struct CutsState *st)
{
    if (st->fade49 > -1) {
        if (st->flags.bit.b6 && st->fade45 != -1)
            fadeout(st->palette, st->fade49, 1);
        else fadeout(st->palette, st->fade49, 0);
        gobble_input_events(st);
        st->flags.bit.b1 = 0;
        st->flags.bit.b0 = 0;
        st->fade49 = -2;
        st->fade47 = -1;
    }
}

/* 0xB1A */
void far cuts_do_fadein(struct CutsState *st, int apply)
{
    set_the_color(0x1B);
    urectangle(0, splity, 0x13F, 0);
    if (!st->flags.bit.b0) {
        if (st->fade47 > -1) cuts_perform_fadein(st);
        else if (!apply) grfx_setpal(st->palette);
    }
}

/* 0xB6D */
void far cuts_do_fadeout(struct CutsState *st, int frame)
{
    if (!st->flags.bit.b0 && st->fade49 > -1)
        cuts_perform_fadeout(st);
}

/* 0xB8E */
void far do_pan(struct CutsState *st)
{
    focus_x = st->panx51 + (stdat.frame + 1) * st->step57 * pan_dx[st->dir55];
    focus_y = st->pany53 + (stdat.frame + 1) * st->step57 * pan_dy[st->dir55];
    vscreen_focus(focus_x, focus_y);
    st->remaining59--;
}

/* 0xBE9, opcode 26 */
int far cutsop_test(unsigned far *code, struct CutsState *st)
{
    return 1;
}

/* 0xBF1, opcode 24 */
int far cutsop_splity(unsigned far *code, struct CutsState *st)
{
    splity = *code == 999 ? -1 : *code - 1;
    return 1;
}

/* 0xC12, opcode 25 */
int far cutsop_music(unsigned far *code, struct CutsState *st)
{
    if (*code == 0) stop_music();
    else set_new_music(*(unsigned char far *)code);
    return 1;
}

/* 0xC38, opcode 0: a subtitle, split at newlines and then to lines 320 pixels wide */
int far cutsop_txt(unsigned far *code, struct CutsState *st)
{
    char far *text;
    char far *next;
    int width;
    int used;
    char saved;
    int parts;
    char far *lines[6];
    register int count, line;
    if (st->flags.bit.b0) return 2;
    if (code[1] == 0xFFFF) {
        st->flag39 = 0;
        return 2;
    }
    st->color38 = *(unsigned char far *)code;
    text = get_string(code[1]);
    lines[0] = text;
    line = count = parts = 0;
    while ((lines[line + 1] = FindStringDelimiter(lines[line], '\n')) != 0 && line < 6) {
        line++;
        *lines[line]++ = 0;
    }
    for (line = 0; line < 6 && lines[line] && count < 6; line++) {
        next = lines[line];
        used = 0;
        while (next) {
            text = next;
            if ((next = FindStringDelimiter(text, ' ')) != 0) {
                next++;
                saved = *next;
                *next = 0;
                width = string_width(text);
                *next = saved;
            } else {
                width = string_width(text);
                if (used + width <= 320) {
                    st->text_lines[count++] = lines[line];
                    parts++;
                }
            }
            if (used + width > 320) {
                text[-1] = 0;
                st->text_lines[count++] = lines[line];
                lines[line] = next = text;
                parts++;
                used = 0;
            } else used += width;
        }
    }
    st->flag39 = parts < 6 ? parts : 6;
    return 2;
}

/* 0xE4D, opcode 1 */
int far cutsop_erase(unsigned far *code, struct CutsState *st)
{
    st->flag39 = 0;
    return 0;
}

/* 0xE5C, opcode 2 */
int far cutsop_func(unsigned far *code, struct CutsState *st)
{
    return 2;
}

/* 0xE64, opcode 3 */
int far cutsop_pause(unsigned far *code, struct CutsState *st)
{
    if (st->flags.bit.b0) return 1;
    st->flags.bit.b1 = 0;
    st->frame3D = code[-2];
    st->frame3F = code[0];
    return 1;
}

/* 0xE91, opcode 14 */
int far cutsop_wait(unsigned far *code, struct CutsState *st)
{
    if (st->flags.bit.b0) return 2;
    st->flags.bit.b1 = 0;
    st->frame3D = code[-2];
    if (st->flags.bit.b5) {
        st->flags.bit.b7 = 1;
        st->frame3F = code[1];
    } else st->frame3F = code[0];
    return 2;
}

/* 0xED8, opcode 4 */
int far cutsop_skip(unsigned far *code, struct CutsState *st)
{
    if (st->flags.bit.b0) return 2;
    st->flags.bit.b1 = 0;
    st->frame3B = code[0];
    if (!st->flags.bit.b5) {
        st->frame3F = code[1];
        st->frame3D = code[0] - 1;
    }
    return 2;
}

/* 0xF19, opcode 5 */
int far cutsop_next(unsigned far *code, struct CutsState *st)
{
    st->flags.bit.b2 = 0;
    return 0;
}

/* 0xF27, opcode 6 */
int far cutsop_end(unsigned far *code, struct CutsState *st)
{
    st->flags.bit.b2 = 0;
    st->flags.bit.b3 = 0;
    return 0;
}

/* 0xF3B, opcode 7 */
int far cutsop_loop(unsigned far *code, struct CutsState *st)
{
    st->repeat41 = code[0];
    st->repeat43 = code[-2];
    st->frame3B = code[-2] + 1;
    st->frame3D = 0;
    st->flags.bit.b1 = 0;
    return 1;
}

/* 0xF69, opcode 8 */
int far cutsop_data(unsigned far *code, struct CutsState *st)
{
    int file;
    if (code[0] == 996) file = rand() % 4 + 0x1C;
    ((char *)st)[7] = ((file >> 6) & 7) + '0';
    ((char *)st)[8] = ((file >> 3) & 7) + '0';
    ((char *)st)[9] = (file & 7) + '0';
    ((char *)st)[12] = ((code[1] >> 3) & 7) + '0';
    ((char *)st)[13] = (code[1] & 7) + '0';
    st->file4B = code[1];
    return 2;
}

/* 0xFDA, opcode 13: speech, or its subtitle when there is no speech */
int far cutsop_say(unsigned far *code, register struct CutsState *st)
{
    if (st->flags.bit.b5) {
        st->fade45 = code[2];
        if (st->fade45 == 999) {
            st->fade45 = -1;
            cutsop_txt(code, st);
        } else if (st->fade45 != 998) {
            if (big_speech_play(code[2], 0x7F, 0x40, sound_fpage, 4) == 0xFF)
                cutsop_txt(code, st);
            else st->flags.bit.b6 = 1;
        }
    } else cutsop_txt(code, st);
    return 3;
}

/* 0x104C, opcode 9 */
int far cutsop_fadeout(unsigned far *code, struct CutsState *st)
{
    if (st->windowed == 0 && st->fade49 > -2) st->fade49 = code[0];
    cuts_do_fadeout(st, stdat.frame);
    return 1;
}

/* 0x1080, opcode 10 */
int far cutsop_fadein(unsigned far *code, struct CutsState *st)
{
    if (st->windowed == 0 && st->fade47 > -2) st->fade47 = code[0];
    cuts_do_fadein(st, stdat.frame);
    return 1;
}

/* 0x10B4, opcode 11 */
int far cutsop_jump(unsigned far *code, struct CutsState *st)
{
    if (code[-2] != code[0] - 1) {
        st->frame3B = code[0] - 1;
        st->frame3D = 0;
        st->frame3F = 0;
        st->flags.bit.b1 = 1;
    }
    return 1;
}

/* 0x10ED, opcode 12 */
int far cutsop_punt(unsigned far *code, struct CutsState *st)
{
    st->flags.bit.b4 = code[0];
    return 1;
}

/* 0x110B, opcode 15 */
int far cutsop_clang(unsigned far *code, struct CutsState *st)
{
    return 0;
}

/* 0x1112, opcode 16: set palette entries from the script */
int far cutsop_palrange(unsigned far *code, struct CutsState *st)
{
    int first, end, i;
    first = code[0] * 3;
    end = code[1] * 3 + first;
    for (i = first; i < end; i++)
        st->palette[i] = code[i - first + 2];
    return code[1] * 3 + 2;
}

/* 0x1165, opcode 17: fade palette entries towards the script's colours */
int far cutsop_palfade(unsigned far *code, register struct CutsState *st)
{
    int task;
    int first;
    int end;
    register int i;
    task = install_timebased_task(task_palfade, code[0], code[1]);
    task_data1[task] = code[2];
    task_data2[task] = code[2] + code[3];
    task_pointer_data[task] = st->palette;
    first = code[2] * 3;
    end = first + code[3] * 3;
    for (i = first; i < end; i++) {
        start_pal[i] = st->palette[i];
        end_pal[i] = code[i - first + 4];
    }
    return code[3] * 3 + 4;
}

/* 0x1221, opcode 18 */
int far cutsop_palset(unsigned far *code, struct CutsState *st)
{
    return 4;
}

/* 0x1229, opcode 19: fade the whole palette towards one of PALS.DAT */
int far cutsop_palsimplefade(unsigned far *code, register struct CutsState *st)
{
    int task;
    register int i;
    task = install_timebased_task(task_palfade, code[1], code[2]);
    if (Transparency == 1) task_data1[task] = 1;
    else task_data1[task] = 0;
    task_data2[task] = 0x100;
    task_pointer_data[task] = st->palette;
    if (!read_quikpal(code[0], end_pal)) remove_task(task);
    else for (i = 0; i < 0x300; i++) start_pal[i] = st->palette[i];
    return 3;
}

/* 0x12D3, opcode 20 */
int far cutsop_vscreen(unsigned far *code, register struct CutsState *st)
{
    virtual_screen(code[0], code[1], code[2] == 999 ? -1 : code[2] - 1);
    if (code[0] == 320 && code[1] == 200 && code[2] == 999) {
        if (st->windowed == 0) st->vscr4F = 0;
    } else st->vscr4F = 1;
    return 3;
}

/* 0x1337, opcode 21 */
int far cutsop_focus(unsigned far *code, struct CutsState *st)
{
    vscreen_focus(code[0], code[1] == 999 ? -1 : code[1]);
    focus_x = code[0];
    focus_y = code[1];
    return 2;
}

/* 0x1375, opcode 22 */
int far cutsop_lback(unsigned far *code, struct CutsState *st)
{
    lback_vscreen(code[0], code[1], code[2]);
    return 3;
}

/* 0x1393, opcode 23 */
int far cutsop_pan(unsigned far *code, register struct CutsState *st)
{
    int dir, step, count;
    dir = code[0];
    step = code[1];
    count = code[2];
    st->panx51 = focus_x;
    st->pany53 = focus_y;
    st->dir55 = dir;
    st->step57 = step;
    st->remaining59 = count;
    return 3;
}

/* 0x13CD */
void far anm_sound_callback(void)
{
    update_big_speech();
}

/* 0x13D6 */
void far anm_sound(register struct CutsState *st)
{
    change_music_maybe();
    if (st->flags.bit.b6 && st->fade45 != -1) {
        update_big_speech();
        if (speech_over()) {
            st->flags.bit.b6 = 0;
            st->fade45 = -1;
        }
    }
}

/* 0x140D */
struct CutsState * far cuts_init_info(register struct CutsState *st)
{
    st->fade45 = -1;
    st->flags.bit.b5 = speech_available();
    st->flags.bit.b4 = 1;
    st->fade47 = -1;
    if (st->windowed == 0) st->fade49 = -2;
    else st->fade49 = -1;
    st->flag39 = 0;
    st->frame3B = 0;
    st->frame3D = 0;
    st->frame3F = 0;
    st->repeat41 = 0;
    st->vscr4F = 0;
    st->remaining59 = 0;
    st->flags.bit.b6 = 0;
    st->flags.bit.b0 = 0;
    st->flags.bit.b1 = 0;
    st->flags.bit.b2 = 1;
    st->flags.bit.b3 = 1;
    st->flags.bit.b4 = 1;
    st->flags.bit.b7 = 0;
    return st;
}

/* 0x148C */
char * far cuts_make_fname(register char *name, int n, int ext)
{
    strcpy(name, "CUTS\\csXXX.nXX");
    name[7] = ((n >> 6) & 7) + '0';
    name[8] = ((n >> 3) & 7) + '0';
    name[9] = (n & 7) + '0';
    name[12] = ((ext >> 3) & 7) + '0';
    name[13] = (ext & 7) + '0';
    return name;
}

/* 0x14DF */
int far gobble_input_events(register struct CutsState *st)
{
    int key;
    register int in;
    key = -1;
    while ((in = mouse_get_input()) > 3) key = in;
    if (in > -1 && in < 4) key = in;
    if (st) switch (key) {
    case -1:
        break;
    case 0x1B:
        if (st->flags.bit.b4 && cuts_number != 2)
            st->flags.bit.b3 = st->flags.bit.b2 = 0;
        cutoff_cutscene_speech();
    default:
        st->flags.bit.b1 = 1;
    }
    if (st && !st->flags.bit.b0 && st->frame3B && st->flags.bit.b1) {
        st->flags.bit.b0 = 1;
        st->repeat41 = 0;
    }
    return key;
}

/* 0x157B */
void far cuts_draw_text(register struct CutsState *st)
{
    int x;
    int y;
    register int i;
    i = 0;
    set_the_color(0xF0);
    rectangle(0, splity, 0x13F, 0);
    *foreground_color = st->color38;
    *background_color = st->color38;
    y = st->y - st->h + cur_font->height * st->flag39 + 2;
    for (i = 0; i < st->flag39; i++) {
        x = (st->x + st->w - string_width(st->text_lines[i])) / 2;
        string_to_screen(st->text_lines[i], x, y);
        y -= cur_font->height;
    }
}

/* 0x162B */
int far run_time_critical_things(struct CutsState *st, struct AnmHdr far *hdr)
{
    anm_cycle((struct Cycle far *)((unsigned char far *)hdr + 0x80));
    anm_sound(st);
    if (*Time - stdat.tick >= 8) {
        stdat.tick = *Time;
        run_timebased_tasks(0);
    }
    return 0;
}

/* 0x1693 */
int far cuts_run_pause(register struct CutsState *st, struct AnmHdr far *hdr)
{
    unsigned long last;
    unsigned long now;
    register int in;
    in = -1;
    last = (*Time - stdat.start) >> 8;
    while (in == -1 && !st->flags.bit.b1 &&
           (now = (*Time - stdat.start) >> 8) < st->frame3F) {
        if (st->flags.bit.b7) in = gobble_input_events(0);
        else in = gobble_input_events(st);
        if (now > last) last = now;
        run_time_critical_things(st, hdr);
    }
    return 0;
}

/* 0x175F */
void far cuts_process_opcodes(int frame, register struct CutsState *st)
{
    while (*stdat.code == frame && st->repeat41 == 0) {
        if ((unsigned far *)stdat.n00 + 0x200 - ((*stdat.code >> 5) ? 1 << (*stdat.code >> 5) : 4) > stdat.code) {
            stdat.code += 2;
            if (stdat.code[-1] < 0x1D)
                stdat.code += cuts_dispatch[stdat.code[-1]](stdat.code, st);
            else return;
        } else {
            lseek(stdat.n0x_fd, (stdat.code - (unsigned far *)stdat.n00 - 0x200) * 2, 1);
            ReadFileToAddress(stdat.n0x_fd, stdat.n00, 0x400);
            stdat.code = (unsigned far *)stdat.n00;
        }
    }
}

/* 0x189F */
#define SCREEN ((unsigned char far *)MK_FP(0xA000, 0))

/* Fetch the next run/skip/dump token of the delta stream. */
#define GET_TOKEN() \
    if (count == 0) { \
        if (*src == 0) { \
            type = 0; count = src[1]; src += 2; \
        } else if (*src < 0x80) { \
            type = 2; count = *src; src++; \
        } else if (*src != 0x80) { \
            type = 1; count = *src & 0x7F; src++; \
        } else { \
            unsigned far *w; \
            if ((w = (unsigned far *)++src, *w) >= 0x8000) { \
                if (*w >= 0xC000) { type = 0; count = *w & 0x3FFF; } \
                else { type = 2; count = *w & 0x7FFF; } \
            } else { \
                if (*w) { type = 1; count = *w; } \
                else return; \
            } \
            src += 2; \
        } \
    }

/* Move the destination k pixels to the right. */
#define ADVANCE(k) \
    dst += (k) / 4; \
    plane += (k) % 4; \
    if (plane >= 4) { plane -= 4; dst++; }

/* Fill k pixels with the run's byte. */
#define RUN(k) { \
    unsigned char mask; int i; int last; \
    i = 0; \
    last = (plane + (k) - 1) / 4; \
    if (i == last) { \
        mask = lmask[plane] & rmask[(plane + (k) - 1) % 4]; \
        outportb(0x3C4, 2); outportb(0x3C5, mask); \
        *dst = *src; \
    } else { \
        mask = lmask[plane]; \
        outportb(0x3C4, 2); outportb(0x3C5, mask); \
        *dst = *src; \
        i++; \
        mask = rmask[(plane + (k) - 1) % 4]; \
        outportb(0x3C4, 2); outportb(0x3C5, mask); \
        dst[last] = *src; \
        if (i < last) { \
            outportb(0x3C4, 2); outportb(0x3C5, 0x0F); \
            mem_set(dst + i, *src, last - i); \
        } \
    } \
}

#define DUMP_PLANE() \
    s2 = s; d2 = d; \
    n4 = cnt / 4; \
    cnt--; \
    outportb(0x3C4, 2); outportb(0x3C5, 1 << p); \
    while (n4-- > 0) { *d2 = *s2; s2 += 4; d2++; }

#define NEXT_PLANE() \
    if (++p >= 4) { p -= 4; d++; } \
    s++;

/* Copy k literal pixels, one plane at a time. */
#define DUMP(k) { \
    int n4; unsigned char far *s; unsigned char far *d; int p; int cnt; \
    unsigned char far *s2; unsigned char far *d2; \
    s = src; d = dst; p = plane; cnt = (k) + 3; \
    DUMP_PLANE() NEXT_PLANE() \
    DUMP_PLANE() NEXT_PLANE() \
    DUMP_PLANE() NEXT_PLANE() \
    s2 = s; d2 = d; \
    n4 = cnt / 4; \
    outportb(0x3C4, 2); outportb(0x3C5, 1 << p); \
    while (n4-- > 0) { *d2 = *s2; s2 += 4; d2++; } \
}

void far draw_rsd(unsigned char far *src, int x, int y, int w, int h,
    int skipx, int skipy)
{
    unsigned char far *dst;
    int col, row, xend, yend, pos, type, n, skip;
    int plane, count;

    plane = x % 4;
    w -= skipx;
    h -= skipy;
    col = row = pos = count = 0;
    dst = SCREEN + (Ytab[y] + x / 4);
    if (skipy > 0 || skipx > 0) {
        skip = skipy * 320 + skipx;
        while (pos < skip) {
            GET_TOKEN()
            if (pos + count <= skip) {
                switch (type) {
                case 0: src++; break;
                case 1: break;
                default: src += count; break;
                }
                pos += count;
                count = 0;
            } else {
                n = skip - pos;
                switch (type) {
                case 0: case 1: break;
                default: src += n; break;
                }
                count -= n;
                pos += n;
            }
        }
        row = skipy;
        col = skipx;
    }
    yend = row + h - 1;
    xend = col + w - 1;
    while (row <= yend) {
        while (col < skipx) {
            GET_TOKEN()
            if (col + count <= skipx) {
                switch (type) {
                case 0: ADVANCE(count) src++; break;
                case 1: ADVANCE(count) break;
                default: ADVANCE(count) src += count; break;
                }
                col += count;
                count = 0;
            } else {
                n = skipx - col;
                switch (type) {
                case 0: ADVANCE(n) break;
                case 1: ADVANCE(n) break;
                default: ADVANCE(n) src += n; break;
                }
                count -= n;
                col += n;
            }
        }
        while (col <= xend) {
            GET_TOKEN()
            if (col + count <= xend + 1) {
                switch (type) {
                case 0: RUN(count) ADVANCE(count) src++; break;
                case 1: ADVANCE(count) break;
                default: DUMP(count) ADVANCE(count) src += count; break;
                }
                col += count;
                count = 0;
            } else {
                n = xend - col + 1;
                switch (type) {
                case 0: RUN(n) ADVANCE(n) break;
                case 1: ADVANCE(n) break;
                default: DUMP(n) ADVANCE(n) src += n; break;
                }
                col += n;
                count -= n;
            }
        }
        row++;
        y--;
        col -= 320;
        dst = SCREEN + Ytab[y] - (320 - w - x) / 4;
    }
}

/* 0x2359 */
int far cuts_process_lp(register struct CutsState *st, struct AnmHdr far *anm,
    unsigned far *lp)
{
    unsigned char far *page;
    unsigned char far *ems2;
    unsigned perrec;
    unsigned far *sizes;
    unsigned char far *data;
    int size;
    int last;
    int nrec;
    unsigned long now;
    unsigned char first;
    unsigned char flipped;
    register int i;

    first = 1;
    stdat.start = *Time;
    page = set_cuts_ems(stdat.ems_page);
    sizes = (unsigned far *)(page + 8);
    data = (unsigned char far *)(sizes + lp[1]);
    last = anm->nlps - 1 == stdat.lp_index && anm->lastdelta ? 1 : 0;
    nrec = lp[1] - last;
    st->frame3D = st->frame3F = 0;
    st->flags.bit.b1 = 0;
    if (nrec > 0 && anm->nlps - 1 > stdat.lp_index) {
        unsigned far *d;
        register unsigned sz;
        d = LPDESC(anm, stdat.lptab[stdat.lp_index + 1]);
        sz = d[2] + d[1] * 2 + 8;
        perrec = (nrec + sz - 1) / nrec;
    } else perrec = -2;
    for (i = 0; i < nrec && st->flags.bit.b2 && st->flags.bit.b3; i++) {
        page = set_cuts_ems(stdat.ems_page);
        size = sizes[i];
        if (size != 0) size -= 4;
        if (st->vscr4F == 0 && stdat.frame == 0 && st->fade49 > -2 && st->windowed == 0) {
            grSoftPageFlip();
            grfx_clear();
            flipped = 1;
        } else flipped = 0;
        if (!st->flags.bit.b0 && st->remaining59 != 0) do_pan(st);
        if (size != 0) {
            int hb;
            int wd;
            int off;
            hb = data[1];
            wd = (*(unsigned far *)(data + 2) + 1) & ~1;
            off = hb ? wd + 4 : 2;
            if (!st->flags.bit.b0) {
                if (splity == -1 && st->vscr4F == 0 && !flipped) grSoftPageFlip();
                if (data[off] == 0)
                    show(st->x + (focus_x & 3), st->y, data + off + 2, 200 - (splity + 1),
                        320, 320 - st->w, 200 - st->h);
                else if (data[off] == 1) {
                    if (splity == -1 && st->vscr4F == 0 && !flipped) copy_visible_to_hidden();
                    draw_rsd(data + off + 2, st->x + (focus_x & 3), st->y,
                        320, 200 - (splity + 1), 320 - st->w, 200 - st->h);
                }
                if (splity == -1 && st->vscr4F == 0 && !flipped) {
                    grPageFlip();
                    grSoftPageFlip();
                }
            }
        }
        if (st->vscr4F == 0 && stdat.frame == 0 && st->fade49 > -2 && st->windowed == 0) {
            grPageFlip();
            grfx_setpal(st->palette);
            grSoftPageFlip();
        }
        data += sizes[i];
        if (stdat.ahead_page >= 0 && anm->nlps > stdat.ahead_index) {
            ems2 = set_cuts_ems(stdat.ahead_page);
            readlpinc(stdat.lptab[stdat.ahead_index], LPDESC(anm, stdat.lptab[stdat.ahead_index]), perrec, ems2);
        }
        stdat.frame++;
        run_time_critical_things(st, anm);
        gobble_input_events(st);
        if (st->repeat41 == 0) cuts_process_opcodes(stdat.frame, st);
        if (!st->flags.bit.b0 && first)
            while (!st->flags.bit.b0 && (now = *Time) - stdat.start < 0x100 / anm->rate) {
                run_time_critical_things(st, anm);
                gobble_input_events(st);
            }
        if (!st->flags.bit.b0) first = 1;
        if (!st->flags.bit.b0 && st->flag39 > 0) {
            cuts_draw_text(st);
            st->flag39 = 0;
        }
        if (!st->flags.bit.b0 && st->frame3F != 0 && st->frame3D == stdat.frame)
            cuts_run_pause(st, anm);
        if (st->frame3B == stdat.frame) {
            st->frame3B = 0;
            if (st->flags.bit.b0) st->flags.bit.b0 = 0;
        }
        if (st->repeat41 > 0 && st->repeat43 == stdat.frame) {
            first = 1;
            st->repeat41--;
            st->file4B = st->file4D;
            return 1;
        }
        stdat.start = now;
    }
    return 0;
}

/* 0x287B */
void far reset_inf_values_eof(register struct CutsState *st)
{
    st->flags.bit.b0 = st->flags.bit.b1 = st->flags.bit.b6 = 0;
    st->flags.bit.b4 = st->flags.bit.b2 = 1;
    st->flag39 = st->frame3B = st->frame3D = st->remaining59 = st->frame3F = 0;
    st->flags.bit.b7 = 0;
}

/* 0x28C7 */
void far cuts_process_anm(struct CutsState *st)
{
    unsigned char far *img;
    unsigned char far *n0x;
    struct AnmHdr far *hdr;
    unsigned char far *desc;
    int result;
    int i;

    result = 0;
    run_timebased_tasks(1);
    n0x = stdat.n00 + 0x400;
    hdr = (struct AnmHdr far *)n0x;
    desc = n0x + 0x500;
    st->palette = n0x + 0xB00;
    stdat.lptab = st->palette + 0x300;
    cuts_process_opcodes(0x3E5, st);
    if ((stdat.anm_fd = open(st->name, 0x8001)) == -1) {
        st->flags.bit.b3 = 0;
        return;
    }
    if (!read_anmhdr(n0x)) {
        st->flags.bit.b3 = 0;
        goto done;
    }
    if (st->windowed == 0) conv_anmpal(n0x + 0x100, st->palette);
    build_lptab((unsigned far *)desc, hdr->nlps, stdat.lptab);
    lp_page = -1;
    for (i = 0; i < num_buf && i < hdr->nlps; i++) {
        img = set_cuts_ems(i);
        if (img != 0)
            if (readlp(stdat.lptab[i], (unsigned far *)(desc + stdat.lptab[i] * 6), img) == -1) ;
    }
    stdat.ahead_page = 1 - num_buf;
    stdat.ems_page = 0;
    stdat.ahead_index = 1;
    stdat.lp_index = 0;
    stdat.frame = 0;
    st->flags.bit.b1 = 0;
    st->flags.bit.b2 = 1;
    if (st->repeat41 == 0) cuts_process_opcodes(stdat.frame, st);
    for (i = 0; i < hdr->nlps && st->flags.bit.b2 && st->flags.bit.b3 && !result; i++) {
        result = cuts_process_lp(st, (struct AnmHdr far *)n0x,
            (unsigned far *)(desc + stdat.lptab[stdat.lp_index] * 6));
        stdat.lp_index++;
        stdat.ahead_index++;
        if (++stdat.ems_page >= num_buf) stdat.ems_page = 0;
        if (++stdat.ahead_page >= num_buf) stdat.ahead_page = 0;
    }
    if (st->repeat41 == 0) {
        reset_inf_values_eof(st);
        cuts_process_opcodes(0x3E7, st);
        if (st->fade49 == -2) grfx_clear();
    }
done:
    close(stdat.anm_fd);
}

/* 0x2B73 */
void far show_anm(int cuts, int x, int y, int w, int h)
{
    unsigned char far *n0x;
    struct CutsState st;

    gobble_input_events(0);
    focus_x = 0;
    focus_y = 199;
    st.x = x;
    st.y = y;
    st.w = w;
    st.h = h;
    if (x == 0 && y == 199 && w == 320 && h == 200) st.windowed = 0;
    else st.windowed = 1;
    stdat.n00 = stdat.n00buf;
    n0x = stdat.n00 + 0x400;
    st.palette = n0x + 0xB00;
    stdat.screen = MK_FP(FP_SEG(&stdat) + 0x134, 0);
    if (get_cuts_ems() >= 3) {
        st.file4B = 0;
        cuts_make_fname(st.name, cuts, st.file4B);
        if ((stdat.n0x_fd = open(st.name, 1)) == -1) {
            seg042_35ED_12B();
            return;
        }
        if (st.windowed == 0) grfx_clear();
        ReadFileToAddress(stdat.n0x_fd, stdat.n00, 0x400);
        stdat.code = (unsigned far *)stdat.n00;
        cuts_init_info(&st);
        movedata(FP_SEG(palette), FP_OFF(palette), FP_SEG(st.palette), FP_OFF(st.palette), 0x300);
        if (st.windowed == 0) fadeout(st.palette, 2, 1);
        st.file4B = st.file4D = 1;
        while (st.flags.bit.b3) {
            st.file4D = st.file4B;
            cuts_make_fname(st.name, cuts, st.file4B);
            st.file4B++;
            cuts_process_anm(&st);
            punt_tasks();
        }
        if (st.fade45 != -1) cutoff_cutscene_speech();
        if (st.windowed == 0) {
            if (st.fade49 != -2) fadeout(st.palette, 2, 1);
            grfx_clear();
        }
        if (st.windowed == 0) {
            virtual_screen(320, 200, -1);
            vscreen_focus(0, 199);
        }
        set_the_window(0, 199, 319, 0);
        if (st.windowed == 0) grfx_clear();
        free_cuts_ems();
    }
    free_speech_stuff();
    close(stdat.n0x_fd);
    mem_set(stdat.screen, 0, 0xFA00);
}

/* 0x2DC5 */
void far show_cutscene(register unsigned n)
{
    int x;
    int y;
    int w;
    register int h;
    cuts_number = n;
    punt_all_digi_fx();
    cutoff_cutscene_speech();
    if (n < 0x100) {
        x = 0;
        y = 0xC7;
        w = 0x140;
        h = 0xC8;
    } else {
        x = 0x10;
        y = 0xB6;
        w = 0xD0;
        h = 0x7F;
    }
    if (n == 2) load_new_music(1, 1);
    grfx_quikfont(3);
    CutsceneOrConversationStringBlock = n + 0xC00;
    mouse_hide();
    if (n == 0x103) {
        y += 4;
        h += 5;
    }
    show_anm(n, x, y, w, h);
    grfx_quikfont(1);
    if (in_game) load_txtmaps();
    if (n < 0x100) {
        if (inplist->cmd == 1) newscr(1);
        else if (inplist->cmd != 0) {
            grfx_quikpal(0);
            editchng(0x7FFE);
        }
    } else editchng(2);
    mouse_show();
}

/* 0x2EA7 */
void far init_cutscene(void)
{
}

/* 0x2EAC: write a value into a cutscene's opcode file, then show it */
void far OpenCutsFile_ovr108_2EAC(unsigned n, int value)
{
    unsigned char ok;
    char name[16];
    register int fd;
    strcpy(name, "CUTS\\csXXX.n00");
    name[7] = ((n >> 6) & 7) + '0';
    name[8] = ((n >> 3) & 7) + '0';
    name[9] = (n & 7) + '0';
    ok = (fd = open(name, 0x8004)) != -1;
    ok &= lseek(fd, 4L, 0) != -1L;
    ok &= write(fd, &value, 2) == 2;
    ok &= write(fd, &value, 2) == 2;
    ok &= lseek(fd, 4L, 1) != -1L;
    ok &= write(fd, &value, 2) == 2;
    ok &= close(fd) != -1;
    if (!ok) return;
    show_cutscene(n);
}

/* 0x2FF0 */
void far run_timebased_tasks(int reset)
{
    static int residue = 0;
    static unsigned long last = 0;
    unsigned long diff;
    int old;
    register int i, steps;
    if (reset) {
        last = 0;
        return;
    }
    if (last) diff = *Time - last;
    else diff = 0;
    last = *Time;
    steps = diff >> 3;
    residue += (int)diff & 7;
    if (residue > 7) {
        residue -= 8;
        steps++;
    }
    for (i = 0; i < 16; i++) {
        if (tasks[i] && (task_flags[i] & 1)) {
            old = task_sofar[i];
            task_sofar[i] += steps / task_period[i];
            task_residue[i] += steps % task_period[i];
            if (task_residue[i] >= task_period[i]) {
                task_residue[i] -= task_period[i];
                task_sofar[i]++;
            }
            if (task_sofar[i] >= task_total[i]) {
                (*tasks[i])(i, 1);
                remove_task(i);
                return;
            }
            if (task_sofar[i] > old) (*tasks[i])(i, 0);
        }
    }
}

/* 0x3148 */
void far punt_tasks(void)
{
    register int i;
    for (i = 0; i < 16; i++) punt_single_task(i);
}

/* 0x3161 */
void far punt_single_task(register int task)
{
    if (tasks[task]) {
        (*tasks[task])(task, 1);
        remove_task(task);
    }
}

/* 0x3191 */
int far install_timebased_task(Task fn, int period, int total)
{
    register int i;
    for (i = 0; i < 16; i++) {
        if (!tasks[i]) {
            tasks[i] = fn;
            task_sofar[i] = task_residue[i] = 0;
            task_period[i] = period;
            task_total[i] = total;
            task_flags[i] = 1;
            return i;
        }
    }
    return 0;
}

/* 0x3200: a task that notes when it last ran */
void far ovr108_3200(int task)
{
    tclock = *Time;
    prev_time = task_time[task];
    task_time[task] = tclock;
}

/* 0x3241 */
void far task_palfade(register int task, int done)
{
    if (done)
        palette_fade(task_total[task], task_total[task], task_data1[task], task_data2[task],
            task_pointer_data[task]);
    else
        palette_fade(task_sofar[task], task_total[task], task_data1[task], task_data2[task],
            task_pointer_data[task]);
}

/* 0x32B5 */
void far remove_task(int task)
{
    tasks[task] = 0;
}

/* 0x32CC */
void far palette_fade(int step, int total, int first, int last, unsigned char far *pal)
{
    int from;
    register int i, end;
    from = first * 3;
    end = last * 3;
    for (i = from; i < end; i++)
        pal[i] = start_pal[i] + step * (end_pal[i] - start_pal[i]) / total;
    grfx_palrange(pal, first, last - first);
}

/* 0x3333: a task that pans the view */
void far ovr108_3333(int task, int done)
{
    register int d, t;
    if (done) t = task_total[task];
    else t = task_sofar[task];
    d = t * pan_step;
    focus_x = task_data1[task] + d * pan_dx[pan_dir];
    focus_y = task_data2[task] + d * pan_dy[pan_dir];
    vscreen_focus(focus_x, focus_y);
}

/* 0x33A8 */
int far virtual_screen(int w, int h, int split)
{
    logw = w;
    logh = h;
    splity = split == 999 ? -1 : split;
    seg003_0272_4A3A(w, h, splity);
    return 1;
}

/* Focus the view and set the window to the part of the screen it covers. */
#define LBACK_WINDOW(fy) \
    fx = x; \
    fy_ = (fy); \
    vscreen_focus(fx, fy_); \
    top = 0; \
    bottom = 0xC7; \
    right = logw - fx; \
    left = 0xC7 - fy_ - 1; \
    if (left < 0) left = 0; \
    if (right >= 0x140) right = 0x13F; \
    ShowClip = 1; \
    set_the_window(top, bottom, right, left)

/* 0x33E0 */
int far lback_vscreen(int x, int y, unsigned n)
{
    int split = splity == -1 ? 0 : splity;
    int ok;
    int fx;
    int fy_;
    int top;
    int bottom;
    char name[18] = "CUTS\\lbackxxx.byt";
    register int left, right;
    name[10] = ((n >> 6) & 7) + '0';
    name[11] = ((n >> 3) & 7) + '0';
    name[12] = (n & 7) + '0';
    ok = LoadDATFile(name, stdat.screen, 0xFA00);
    if (split == 0) {
        LBACK_WINDOW(y);
        show(0, 0xC7, stdat.screen, 200, 320, 0, 0);
    } else {
        LBACK_WINDOW(y);
        show(0, 0xC7, stdat.screen, 200 - (split + 1), 320, 0, 0);
        ShowClip = 0;
        LBACK_WINDOW(y - (0xC7 - split));
        if (bottom != left)
            show(0, 0xC7, stdat.screen, 200, 320, 0, 200 - (split + 1));
    }
    ShowClip = 0;
    vscreen_focus(focus_x, focus_y);
    set_the_window(0, 0xC7, 0x13F, 0);
    return ok;
}

/* 0x3620 */
char far * far ovr108_3620(void)
{
    return dfx_buffer;
}
