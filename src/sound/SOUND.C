/* target: seg016_1E73 */
/* opts: -mm -1 -G -O -Y -d */
/* Sound and music: digitised effects played through the AIL digital driver from sound
   pages kept in EMS, MIDI effects and instruments played on spare channels of the XMIDI
   driver, the music sequences and their timbres, and the sound settings. The whole of
   DOS resident segment seg016_1E73, in original order. Function and global names are
   the originals from the FM Towns symbol table where it has them.

   Start-up (UWEDIT.C): seg016_1E73_2FCB reads the music and speech card numbers from
   UW.CFG, init_sounds loads SOUND\DMnn.ADV for the music card and SOUND\DDnn.ADV for the
   speech card (nn the card number in octal), registers them with AIL (AIL.ASM), reads
   SOUND\SOUNDS.DAT into `effects` and the timbre library SOUND\UW.<suffix>, and
   init_timers starts the game clock: an AIL timer at 256 Hz whose callback increments
   *Time, so the game's time counts 1/256 seconds. A second AIL timer at 16 Hz runs
   digi_fx_timer (or fx_timer without a digital driver).

   Effects: play_effect and its variants take an effect number. Below 100 it is an entry
   of SOUNDS.DAT (patch, note, volume, length, priority, and whether SOUND\SPnn.VOC
   exists); 100 and up name SOUND\UWnn.VOC (nn = number - 100), played centred at full
   volume. A sound with a .VOC goes to the one digital channel (digi_fx_play); otherwise,
   or when that fails, it is played as a MIDI note on one of three channels locked from
   the music driver (fx_play). Digital sounds are cached in four 16 KB EMS pages from
   sound_fpage, mapped through frame page 2, and streamed to the driver through two
   2 KB buffers (dsdata, in dfx_buffer) that AIL plays in turn; update_digi_playback,
   called from the main loop, refills them and moves the pan and volume with the source.
   Positioned sounds take their pan and volume from sound_move.

   Music: the theme playing (curmusic) and the one wanted next (newmusic) are numbers
   whose two octal digits name SOUND\UWAnn.XMI (UWRnn.XMI for the MT-32), so the files
   run 01 to 07, 10 to 17, 30 and 31. Other files ask for a theme with set_new_music;
   change_music_maybe, from the main loop, switches themes and picks a walking theme for
   the world. Themes 2 to 4 are the combat themes and 8 to 15 the walking themes (macros
   COMBAT and WALKING below); 5 plays while the player's weapon is drawn, 6 must finish
   before anything replaces it, and 0x18 (UWx30.XMI) is left to repeat. UW-Formats'
   titles for the UW2 files agree for 2 to 6 (enemy wounded, combat, dangerous situation,
   armed, victory) and call 30 the introduction.

   name: descriptive (the file's own name is not known); see the note below for the
   function names. */
/* name: The FM Towns build has this file's functions in the same order, so the IDA-named ones
   take the FM Towns name at the same place among their neighbours where the code
   corresponds (same callees, same globals): digi_fx_timer, init_digi_fx, free_dfx_ems,
   digi_fx_play, stop_digi_file, free_digi_stuff, cllbck_tst, load_global_timbre,
   init_timbres, load_sound_driver, play_music, read_file_to_mbuf, fx_available,
   music_available, stop_music, kill_all_digi_effects, read_fx_data, do_settings,
   init_speech, speech_available and free_speech_stuff. FM Towns has no counterpart of
   seg016_1E73_19DE, seg016_1E73_1BC3 or seg016_1E73_2FCB, nor of cache_timbre_header_
   here. Six small functions sit inside their neighbours' ranges in the target table and
   are static: toggle_music, fade_music, fx_timer, stop_speech, init_voc_playback and
   voc_stub. Only fx_timer is used (init_fx takes its address). */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <stat.h>
#include <alloc.h>
#include "critter.h"
#include "file.h"
#include "gfx.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"

/* An entry of a global timbre library's directory, 6 bytes. */
struct GtlHdr {
    unsigned char patch;
    unsigned char bank;                 /* 0xFF ends the directory */
    uint32 offset;
};

/* A digital channel's playback state, 13 bytes. */
struct DsChannel {
    uint16 priority;                    /* 0x00 */
    unsigned char slot;                 /* 0x02: index into ds_sounds_in_ems */
    int16 loops;                        /* 0x03 */
    int32 pos;                          /* 0x05 */
    int32 remaining;                    /* 0x09 */
};

/* A digitised sound held in EMS pages, 14 bytes. */
struct EmsSound {
    unsigned char sound;                /* 0xFF when free */
    char pages[4];                      /* 0x01: -1 when unused */
    int16 loops;                        /* 0x05 */
    int32 size;                         /* 0x07 */
    uint16 priority;                    /* 0x0B */
    unsigned char channels;             /* 0x0D: mask of the channels playing it */
};

/* Where the sound playing on the digital channel comes from. */
HOST_LAYOUT_BEGIN
struct DigiSrc {
    int16 x, y, z;
    char type;                          /* 0x06: 1 a position, 2 an object */
    char pad7;
    struct Object far *obj;             /* 0x08 */
};
HOST_LAYOUT_END

/* An entry of SOUNDS.DAT, 8 bytes. */
struct Effect {
    unsigned char patch;                /* in bank 1 of the timbres */
    unsigned char note;                 /* 0x01 */
    unsigned char vol;                  /* 0x02 */
    uint16 length;                      /* 0x03 */
    unsigned char digi;                 /* 0x05: nonzero when there is a .VOC */
    uint16 priority;                    /* 0x06 */
};

/* The effects table, SOUNDS.DAT's entries (49 at most). */
/* match: far, so its own segment (60A0:0000, segment table entry 70, after AI's);
   EFFECT.C and CUTS.C, its other users, are linked too late to own it. */
struct Effect far effects[49];

/* The workspace may have taken EMS page 2: put back whichever mapping it should have. */
#define RESTORE_EMS() \
    if (ws_active) { seg013_1D3C_E4(0, 0, 4); seg042_35ED_12B(); } \
    else if (obj_inpage1 != 0xFF) MapMemory_seg013_1D3C_C7(2, obj_inpage1)

/* Uninitialised data, DS:23E2 to DS:24AB. */
/* match: Turbo C lays _BSS out by a hash of the names, ties in definition order (see
   MOUSE.C for the hash). The publics are the FM Towns names; FM Towns has no names for
   the statics, so theirs are ours, chosen to land where the EXE has them (bucket in
   brackets). */
static uint16 timbre_size;              /* DS:23E2 (4), the size of the timbre being read */
static struct GtlHdr timbre_entry;      /* DS:23E4 (4), its directory entry */
static int16 seq_state_sz;              /* DS:23EA (11), AIL's state table size */
struct SoundBuff dsbuf[2];              /* DS:23EC (76) */
static int16 htimer;                    /* DS:2404 (272), the game clock's AIL timer */
static int16 m_settings[3];             /* DS:2406 (293), the music card's IRQ, port and DMA */
static int16 fx_clock;                  /* DS:240C (382), the effects' AIL timer */
static unsigned char fx_mask;           /* DS:240E (446), the MIDI effects playing */
static unsigned char fx_note[3];        /* DS:240F (454), each one's note */
static void far *speech_mem;            /* DS:2412 (539), the digital driver */
static int16 speech_cfg[3];             /* DS:2416 (547), the speech card's IRQ, port and DMA */
static unsigned char fx_timbre[3];      /* DS:241C (566), each effect's patch */
struct DsChannel ds_channel_info[1];    /* DS:2420 (596) */
static unsigned char fx_midi_chan[3];   /* DS:242D (622), each effect's MIDI channel */
static struct DrvrDesc far *speech_descr;   /* DS:2430 (651) */
uint16 ds_page_status;                  /* DS:2434 (652), the EMS pages in use */
void far *state_table;                  /* DS:2436 (659) */
unsigned char which_buffer;             /* DS:243A (671) */
unsigned char pending[2];               /* DS:243B (672), buffers waiting to be registered */
static unsigned char curmusic;          /* DS:243D (683), the theme playing */
static unsigned char newmusic;          /* DS:243E (694), the theme wanted next */
struct EmsSound ds_sounds_in_ems[4];    /* DS:2440 (716) */
static int16 fx_ticks_left[3];          /* DS:2478 (726), -1 for an effect that holds */
static uint32 theme_changed;            /* DS:247E (732), when a combat theme last began */
struct DigiSrc digi_src;                /* DS:2482 (756) */
static void far *midi_drv;              /* DS:248E (765), the XMIDI driver */
static void far *midi_buf;              /* DS:2492 (789), the XMI file being played */
int16 ds_channel_status;                /* DS:2496 (844), the digital channels playing */
static void far *drv_mem[2];            /* DS:2498 (844), the blocks the drivers sit in */
static unsigned char numeffects;        /* DS:24A0 (910), the entries of SOUNDS.DAT */
static int16 xmi_sequence;              /* DS:24A2 (912), the music's AIL sequence, -1 for none */
static void far *timbre_cache;          /* DS:24A4 (948) */
static struct DrvrDesc far *drv_desc;   /* DS:24A8 (1020), the XMIDI driver's description */

/* Initialised data, DS:0292 onwards. */
static unsigned char sound_b292 = 1;    /* cleared on init and punt, never read */
static unsigned char music_on = 1;
static unsigned char fx_on = 1;
static unsigned char fx_ok = 1;
static unsigned char music_ok = 1;
unsigned char speechok = 0;
static unsigned char sound_card = 0;
static unsigned char speech_card = 0;
/* The walking themes, by (level - 1) / 8 and a random pick of three. */
static unsigned char walking_music[9][3] = {
    { 0x0A, 0x0C, 0x0E }, { 0x09, 0x0A, 0x0F }, { 0x0B, 0x0D, 0x0A },
    { 0x0C, 0x0D, 0x09 }, { 0x08, 0x0B, 0x0E }, { 0x0D, 0x08, 0x0F },
    { 0x0E, 0x09, 0x08 }, { 0x0F, 0x0B, 0x0A }, { 0x08, 0x0C, 0x09 }
};
static unsigned char music_world = 0xFF;
static int16 music_driver = -1;
int16 sphdriver = -1;
static int16 timbre_fd = -1;
char far *dsdata[2] = { 0, 0 };
unsigned char ds_volume = 0;
unsigned char ds_pan = 0;
static uint16 fx_channels = 0;
unsigned char channel_punt = 0;
unsigned char channel_sem = 0;
unsigned char load_only = 0;
static unsigned char sound_started = 0;

/* The 16 Hz effects timer when there is a digital driver. If a new sound has interrupted
   the one playing (channel_punt) and digi_fx_play is not in the middle of setting up
   (channel_sem), it registers the waiting buffers and starts playback. Then, like
   fx_timer, it counts down each MIDI effect's ticks and silences it when they run out
   (all notes off on the PC speaker, else a note off and the channel released). Also
   keeps dsfx_playing, which other files read, equal to whether the digital channel is
   busy. */
void far digi_fx_timer(void)
{
    unsigned char i, bit;

    if (ds_channel_status) {
        dsfx_playing = 1;
        if (channel_punt == 1 && channel_sem == 0
                && AIL_sound_buffer_status(sphdriver, which_buffer) != 2) {
            if (pending[0] == 1) {
                AIL_register_sound_buffer(sphdriver, 0, &dsbuf[0]);
                pending[0] = 0;
            }
            if (pending[1] == 1) {
                AIL_register_sound_buffer(sphdriver, 1, &dsbuf[1]);
                pending[1] = 0;
            }
            AIL_set_digital_playback_volume(sphdriver, ds_volume);
            AIL_set_digital_playback_panpot(sphdriver, ds_pan);
            channel_punt = 0;
            AIL_start_digital_playback(sphdriver);
        }
    } else
        dsfx_playing = 0;
    if (!fx_mask)
        return;
    for (i = 0, bit = 1; i < 3; i = i + 1, bit <<= 1) {
        if ((fx_mask & bit) && fx_ticks_left[i] != -1 && --fx_ticks_left[i] == 0) {
            fx_mask = fx_mask & ~bit;
            if (sound_card == 1)
                AIL_send_channel_voice_message(music_driver, fx_midi_chan[i] + 0xAF, 0x7B, 0);
            else {
                AIL_send_channel_voice_message(music_driver, fx_midi_chan[i] + 0x7F, fx_note[i], 0);
                AIL_release_channel(music_driver, fx_midi_chan[i]);
                fx_channels &= ~(1 << fx_midi_chan[i]);
            }
        }
    }
}

unsigned char far init_digi_fx(void)
{
    int i, j;

    ds_channel_status = 0;
    ds_page_status = 0;
    if (!speechok)
        return 0;
    if (dsdata[0] == 0)
        dsdata[0] = dfx_buffer;
    if (dsdata[1] == 0)
        dsdata[1] = dsdata[0] + 0x800;
    for (i = 0; i < 1; i++)
        ds_channel_info[i].priority = 0;
    for (i = 0; i < 4; i++) {
        ds_sounds_in_ems[i].sound = 0xFF;
        ds_sounds_in_ems[i].channels = 0;
        for (j = 0; j < 4; j++)
            ds_sounds_in_ems[i].pages[j] = -1;
    }
    digi_src.type = 0;
    return 1;
}

/* Loads effect fx's .VOC into the EMS cache without playing it (UWEDIT.C preloads 1, 2
   and 0xB). */
void far load_digi_fx(unsigned char fx)
{
    unsigned char r;

    load_only = 1;
    r = digi_fx_play(fx, 0, 0x40);
    load_only = 0;
}

void far punt_all_digi_fx(void)
{
    int i, j;

    stop_digi_file(0);
    channel_punt = 0;
    for (i = 0; i < 4; i++) {
        ds_sounds_in_ems[i].priority = 0;
        ds_sounds_in_ems[i].channels = 0;
        ds_sounds_in_ems[i].sound = 0xFF;
        for (j = 0; j < 4; j++)
            ds_sounds_in_ems[i].pages[j] = -1;
    }
    ds_page_status = 0;
}

/* Frees the EMS pages of cache slot `slot`, appending each page number to list and
   counting it in *count. */
void far free_dfx_ems(char slot, char *list, char *count)
{
    int i;

    for (i = 0; i < 4 && ds_sounds_in_ems[slot].pages[i] != -1; i++) {
        if (ds_page_status & (1 << ds_sounds_in_ems[slot].pages[i])) {
            ds_page_status &= ~(1 << ds_sounds_in_ems[slot].pages[i]);
            list[*count] = ds_sounds_in_ems[slot].pages[i];
            ds_sounds_in_ems[slot].pages[i] = -1;
            (*count)++;
        }
    }
}

/* Copies the next 2 KB (or what is left) of the sound playing on channel chan, from
   offset off of its cached EMS page `page`, into buffer buf, and sets up dsbuf[buf] for
   AIL. The first chunk of a sound (page 0, offset 0) starts with the .VOC header, which
   AIL_index_VOC_block parses for the sample rate and packing; its 0x20 header bytes are
   not counted as sound. Returns 0 if the .VOC is not one AIL accepts. */
unsigned char far load_dfx_page(int chan, int page, int32 off, int buf)
{
    int32 len;

    MapMemory_seg013_1D3C_C7(2, sound_fpage + ds_sounds_in_ems[ds_channel_info[chan].slot].pages[page]);
    len = ds_channel_info[chan].remaining > 0x800 ? 0x800L : ds_channel_info[chan].remaining;
    FAR_COPY(dsdata[buf], MK_FP(EmsBuff + 0x800, (unsigned)off), (unsigned)len);
    RESTORE_EMS();
    if (page == 0 && off == 0) {
        if (AIL_index_VOC_block(sphdriver, dsdata[buf], -1, &dsbuf[buf]) == 0) {
            ds_sounds_in_ems[ds_channel_info[chan].slot].channels =
                ds_sounds_in_ems[ds_channel_info[chan].slot].channels & ~(1 << chan);
            return 0;
        }
        ds_channel_info[chan].remaining -= len + 0x20;
        ds_channel_info[chan].pos = len;
        if (ds_channel_info[chan].pos == 0x800)
            dsbuf[buf].len = len - 0x20;
    } else {
        ds_channel_info[chan].remaining -= len;
        ds_channel_info[chan].pos += len;
        dsbuf[buf].data = dsdata[buf];
        dsbuf[buf].len = len;
    }
    return 1;
}

/* Plays effect fx on the digital channel at volume vol and pan pan (0 to 0x7F, 0x40 the
   centre). The .VOC is read into the EMS cache unless it is already there: a free slot
   and enough free pages (one per 16 KB) are found, first by evicting cached sounds that
   are not playing and then by stopping a playing one of no higher priority. The new
   sound then takes the channel if its priority (SOUNDS.DAT's plus vol, or 0x7D00 for the
   UWnn files) is at least the playing one's. Effects of length 5000 or more loop
   (length >> 6) - 1 times. With load_only set it stops after caching. Returns 100 (the
   channel plus 100, as kill_effect expects) on success, 100 when only loaded, and 0xFF on
   failure. A sound that fills the cache with no slot free, or that AIL cannot parse,
   turns the digital sound off for the session (speechok = 0). */
unsigned char far digi_fx_play(unsigned char fx, unsigned char vol, unsigned char pan)
{
    char num[4];
    char chan;
    char list[4];
    char eof_hit;
    char paused;
    char slot;
    int32 total;
    int32 needed;
    char i;
    char count;
    int fd;
    unsigned prio;
    char found;
    char path[80];
    struct stat st;
    register int j;
    register int buf;

    if (!speechok)
        return 0xFF;
    channel_sem = 1;
    if (fx > 0x63)
        prio = 0x7D00;
    else if (!effects[fx].digi)
        goto fail_sem;
    else
        prio = effects[fx].priority + vol;
    for (i = 0; i < 4; i++)
        if (ds_sounds_in_ems[i].sound == fx) {
            slot = i;
            goto have_slot;
        }
    fd = -1;
    strcpy(path, "SOUND\\");
    if (fx > 0x63) {
        strcat(path, "UW");
        if (fx - 100 < 10)
            strcat(path, "0");
        strcat(path, itoa(fx - 100, num, 10));
    } else {
        strcat(path, "SP");
        if (fx < 10)
            strcat(path, "0");
        strcat(path, itoa(fx, num, 10));
    }
    strcat(path, ".VOC");
    if ((fd = open(path, O_RDONLY | O_BINARY)) == -1)
        goto fail_close;
    if (fstat(fd, &st) == -1)
        goto fail_close;
    needed = (st.st_size >> 14) + 1;
    count = 0;
    memset(list, -1, 4);
    for (i = 0; i < 4; i++)
        if (!(ds_page_status & (1 << i)))
            list[count++] = i;
    if (count >= needed) {
        slot = -1;
        for (i = 0; i < 4; i++)
            if (ds_sounds_in_ems[i].sound == 0xFF)
                slot = i;
        if (slot != -1)
            goto load;
        goto punt;
    } else {
        if (!load_only) {
            for (i = 0; i < 4; i++) {
                if (!ds_sounds_in_ems[i].channels) {
                    ds_sounds_in_ems[i].sound = 0xFF;
                    free_dfx_ems(i, list, &count);
                    if (count >= needed) {
                        slot = i;
                        goto load;
                    }
                }
            }
        }
        for (i = 0; i < 4; i++) {
            if (ds_sounds_in_ems[i].channels != 0 && ds_sounds_in_ems[i].priority <= prio) {
                ds_sounds_in_ems[i].sound = 0xFF;
                free_dfx_ems(i, list, &count);
                channel_punt = 1;
                pending[0] = pending[1] = 0;
                ds_channel_status &= ~ds_sounds_in_ems[i].channels;
                ds_sounds_in_ems[i].channels = 0;
                if (count >= needed) {
                    slot = i;
                    goto load;
                }
            }
        }
        goto fail_close;
    }
load:
    total = 0;
    eof_hit = 0;
    for (i = 0, j = 0; i < count; i++) {
        MapMemory_seg013_1D3C_C7(2, sound_fpage + list[i]);
        total += intoFarBuffer_ovr167_5DA(fd, MK_FP(EmsBuff + 0x800, 0), 0x4000);
        RESTORE_EMS();
        ds_sounds_in_ems[slot].pages[j] = list[i];
        ds_page_status |= 1 << list[i];
        j++;
        if (eof(fd)) {
            eof_hit = 1;
            for (; j < 4; j++)
                ds_sounds_in_ems[slot].pages[j] = -1;
            break;
        }
    }
    if (eof_hit == 0)
        goto fail_close;
    close(fd);
    ds_sounds_in_ems[slot].sound = fx;
    ds_sounds_in_ems[slot].size = total;
    ds_sounds_in_ems[slot].priority = 0;
    if (fx > 0x63)
        ds_sounds_in_ems[slot].loops = 0;
    else if (effects[fx].length >= 5000)
        ds_sounds_in_ems[slot].loops = (effects[fx].length >> 6) - 1;
    else
        ds_sounds_in_ems[slot].loops = 0;
have_slot:
    if (load_only) {
        channel_sem = 0;
        return 100;
    }
    if (ds_channel_status == 0) {
        channel_punt = 0;
        chan = 0;
    } else if (prio >= ds_channel_info[0].priority) {
        chan = 0;
        channel_punt = 1;
        pending[0] = pending[1] = 0;
        ds_channel_status = 1;
        ds_sounds_in_ems[ds_channel_info[chan].slot].channels =
            ds_sounds_in_ems[ds_channel_info[chan].slot].channels & ~(1 << chan);
        ds_sounds_in_ems[ds_channel_info[chan].slot].priority = 0;
    } else {
        channel_sem = 0;
        return 0xFF;
    }
    ds_sounds_in_ems[slot].channels = ds_sounds_in_ems[slot].channels | (1 << chan);
    ds_channel_info[chan].loops = ds_sounds_in_ems[slot].loops;
    ds_channel_info[chan].slot = slot;
    ds_channel_info[chan].remaining = ds_sounds_in_ems[slot].size;
    ds_channel_info[chan].priority = prio;
    if (ds_sounds_in_ems[slot].priority < prio)
        ds_sounds_in_ems[slot].priority = prio;
    found = 0;
    for (i = 0; i < 2; i++) {
        buf = AIL_sound_buffer_status(sphdriver, i);
        if (buf == 3) {
            buf = i;
            goto got_buf;
        }
    }
    paused = 1;
    AIL_pause_digital_playback(sphdriver);
    for (i = 0; i < 2; i++) {
        buf = AIL_sound_buffer_status(sphdriver, i);
        if (buf == 0) {
            buf = i;
            found = 1;
            goto got_buf;
        }
    }
    buf = 0;
got_buf:
    which_buffer = !buf;
    if (!load_dfx_page(chan, 0, 0L, buf))
        goto punt;
    if (buf == 0) {
        dsbuf[1].sample_rate = dsbuf[0].sample_rate;
        dsbuf[1].pack_type = dsbuf[0].pack_type;
    } else {
        dsbuf[0].sample_rate = dsbuf[1].sample_rate;
        dsbuf[0].pack_type = dsbuf[1].pack_type;
    }
    if (channel_punt == 1 && found == 0)
        pending[buf] = 1;
    else
        AIL_register_sound_buffer(sphdriver, buf, &dsbuf[buf]);
    ds_volume = vol;
    ds_pan = pan;
    if (channel_punt == 0) {
        AIL_set_digital_playback_panpot(sphdriver, ds_pan);
        AIL_set_digital_playback_volume(sphdriver, ds_volume);
    }
    ds_channel_status |= 1 << chan;
    if (paused == 1)
        AIL_resume_digital_playback(sphdriver);
    channel_sem = 0;
    return chan + 100;
punt:
    speechok = 0;
    kill_all_digi_effects();
fail_close:
    close(fd);
fail_sem:
    channel_sem = 0;
    return 0xFF;
}

/* The pan and volume for a sound at x, y (in eighths of a tile) heard by the player.
   pan is 0x40 less the sideways part of the direction to the source in the player's
   frame, clamped to 0..0x7F; volume is vol within one tile, nothing beyond six tiles,
   and falls off linearly between (vol * (48 - d) / 40), clamped to 0..0x7F. */
void far sound_move(int x, int y, int vol, int16 *pan, int16 *volume)
{
    int32 ldx;
    int32 ldy;
    int32 d2;
    unsigned dist;
    int angle;
    int side;
    int nx;
    int ny;
    int16 b;
    int16 a;
    int px;
    int py;

    px = (OBJ_HOMEX(ThePlayer) << 3) + OBJ_FINEX(ThePlayer);
    py = (OBJ_HOMEY(ThePlayer) << 3) + OBJ_FINEY(ThePlayer);
    angle = ((OBJ_HEADING(ThePlayer) << 5) + OBJ_FINEHEAD(ThePlayer)) << 8;
    angle = (0x4000 - angle) & 0xFFFF;
    cFstSinCos(angle, &a, &b);
    ldx = x - px;
    ldy = y - py;
    d2 = ldx * ldx + ldy * ldy;
    dist = cSqRt(d2);
    if (dist == 0)
        *pan = 0x40;
    else {
        nx = (0x7F * ldx) / dist;
        ny = (0x7F * ldy) / dist;
        a >>= 8;
        b >>= 8;
        side = (a * nx - ny * b) >> 8;
        *pan = 0x40 - side;
        if (*pan > 0x7F)
            *pan = 0x7F;
        else if (*pan < 0)
            *pan = 0;
    }
    if (dist > 0x30)
        *volume = 0;
    else if (dist >= 8)
        *volume = vol * (0x30 - dist) / 0x28;
    else
        *volume = vol;
    if (*volume > 0x7F)
        *volume = 0x7F;
    else if (*volume < 0)
        *volume = 0;
}

/* Called each pass of the main loop: re-pans the digital sound if it has a source
   (a fixed point or a moving object), restarts a looping sound when both buffers have
   played out, refills whichever buffer AIL has finished with (status 3) from the EMS
   cache, and keeps playback running. */
void far update_digi_playback(void)
{
    unsigned char chan;
    int16 st[2];
    char page;
    unsigned char slot;
    int32 off;
    int16 vol;
    int16 pan;
    register int i;

    channel_sem = 1;
    if (digi_src.type != 0 && channel_punt == 0) {
        if (digi_src.type == 1)
            sound_move(digi_src.x, digi_src.y, digi_src.z, &pan, &vol);
        else
            sound_move((OBJ_HOMEX(digi_src.obj) << 3) + OBJ_FINEX(digi_src.obj),
                       (OBJ_HOMEY(digi_src.obj) << 3) + OBJ_FINEY(digi_src.obj),
                       digi_src.z, &pan, &vol);
        AIL_set_digital_playback_panpot(sphdriver, pan);
        AIL_set_digital_playback_volume(sphdriver, vol);
    }
    chan = 0;
    if (ds_channel_status & (1 << chan)) {
        slot = ds_channel_info[chan].slot;
        st[0] = AIL_sound_buffer_status(sphdriver, 0);
        st[1] = AIL_sound_buffer_status(sphdriver, 1);
        if (st[0] == 3 && st[1] == 3 && ds_channel_info[chan].remaining <= 0) {
            if (ds_channel_info[chan].loops <= 0) {
stop:
                ds_channel_status &= ~(1 << chan);
                ds_sounds_in_ems[slot].channels = ds_sounds_in_ems[slot].channels & ~(1 << chan);
                ds_sounds_in_ems[slot].priority = 0;
                ds_channel_info[chan].priority = 0;
                digi_src.type = 0;
                goto done;
            }
            ds_channel_info[chan].loops--;
            ds_channel_info[chan].remaining = ds_sounds_in_ems[slot].size;
            if (!load_dfx_page(chan, 0, 0L, 0))
                goto stop;
            AIL_register_sound_buffer(sphdriver, 0, &dsbuf[0]);
        }
        for (i = 0; i < 2; i++) {
            if (st[i] == 3 && ds_channel_info[chan].remaining > 0 && pending[i] == 0) {
                off = ds_channel_info[chan].pos & 0x3FFF;
                page = ds_channel_info[chan].pos >> 14;
                load_dfx_page(chan, page, off, i);
                if (channel_punt == 1)
                    pending[i] = 1;
                else
                    AIL_register_sound_buffer(sphdriver, i, &dsbuf[i]);
            }
        }
    }
    if (channel_punt == 0)
        AIL_start_digital_playback(sphdriver);
done:
    channel_sem = 0;
}

void far stop_digi_file(unsigned char chan)
{
    unsigned char slot;

    if (!speechok)
        return;
    if (ds_channel_status & (1 << chan)) {
        slot = ds_channel_info[chan].slot;
        ds_channel_status &= ~(1 << chan);
        ds_sounds_in_ems[slot].channels = ds_sounds_in_ems[slot].channels & ~(1 << chan);
        ds_sounds_in_ems[slot].priority = 0;
        ds_channel_info[chan].priority = 0;
        AIL_stop_digital_playback(sphdriver);
        digi_src.type = 0;
    }
}

void far free_digi_stuff(void)
{
    dsdata[0] = dsdata[1] = 0;
}

/* The game clock: AIL calls it 256 times a second (init_timers). */
void far cllbck_tst(void)
{
    (*Time)++;
}

unsigned char far init_timers(void)
{
    htimer = AIL_register_timer(cllbck_tst);
    if (htimer == -1)
        first_punt(1);
    AIL_set_timer_frequency(htimer, 0x100);
    AIL_start_timer(htimer);
    sound_started = sound_started | 1;
    return 1;
}

void far punt_sound_stuff(unsigned char failed)
{
    free_s_mem();
    fx_ok = 0;
    music_ok = 0;
    music_on = 0;
    fx_on = 0;
    sound_b292 = 0;
    if (failed)
        printf("Sound system initialization failed.\n");
}

/* Sets up the music driver for sound_card and, when there is one, the speech driver;
   the timbres; the effects table and timer (init_fx); and the 9000-byte music buffer.
   Card 1 (DM01.ADV, the PC speaker) gets effects only, no music. Any failure turns all
   sound off and prints "Sound system initialization failed." Note that the driver name
   is built by writing into the string literal. */
unsigned char far init_sounds(void)
{
    register char *name = "dm00.adv";
    unsigned char failed = 0;
    char path[80];

    midi_buf = state_table = timbre_cache = midi_drv = 0;
    drv_mem[0] = drv_mem[1] = 0;
    AIL_startup();
    sound_started = sound_started | 2;
    if (sound_card == 0) {
        if (speech_card != 0)
            init_speech();
    } else {
        if (sound_card == 1) {
            music_ok = 0;
            music_on = 0;
        }
        name[1] = 'm';
        name[2] = (sound_card >> 3) + '0';
        name[3] = (sound_card & 7) + '0';
        strcpy(path, "SOUND\\");
        strcat(path, name);
        if ((midi_drv = load_sound_driver(path, 0)) == 0)
            goto fail;
        if ((music_driver = AIL_register_driver(midi_drv)) == -1)
            goto fail;
        drv_desc = AIL_describe_driver(music_driver);
        if (drv_desc->drvr_type != 3)
            goto fail;
        do_settings(drv_desc, m_settings);
        if (!AIL_detect_device(music_driver, drv_desc->io, drv_desc->irq, drv_desc->dma, drv_desc->drq))
            goto fail;
        AIL_init_driver(music_driver, drv_desc->io, drv_desc->irq, drv_desc->dma, drv_desc->drq);
        if (speech_card != 0)
            init_speech();
        if (!init_fx())
            goto fail;
        newmusic = 0;
        if (music_ok) {
            if ((midi_buf = farmalloc(9000)) == 0)
                goto fail;
            seq_state_sz = AIL_state_table_size(music_driver);
            if ((state_table = farmalloc(seq_state_sz)) == 0)
                goto fail;
            if (!init_timbres())
                goto fail;
            if (!(unsigned char)OkEnoughMem_ovr167_463())
                goto fail;
        }
        sound_b292 = 0;
        return 1;
fail:
        failed = 1;
    }
    punt_sound_stuff(failed);
    return 0;
}

/* Reads one timbre from the global timbre library open on fd: a directory of 6-byte
   entries (struct GtlHdr) ending at bank 0xFF, each pointing at a timbre that starts
   with its own length word. Returns the timbre in a new far block (the caller frees it)
   or 0. */
void far * far load_global_timbre(int fd, unsigned char bank, unsigned char patch)
{
    uint16 far *p;

    if (fd == -1)
        return 0;
    lseek(fd, 0L, 0);
    do {
        if (intoFarBuffer_ovr167_5DA(fd, &timbre_entry, 6) != 6)
            return 0;
        if (timbre_entry.bank == 0xFF)
            return 0;
    } while (timbre_entry.bank != bank || timbre_entry.patch != patch);
    lseek(fd, timbre_entry.offset, 0);
    intoFarBuffer_ovr167_5DA(fd, &timbre_size, 2);
    if ((p = farmalloc(timbre_size)) == 0)
        return 0;
    *p = timbre_size;
    if (intoFarBuffer_ovr167_5DA(fd, p + 1, timbre_size - 2) != timbre_size - 2)
        return 0;
    return p;
}

/* Opens SOUND\UW.<suffix> (the driver's data_suffix: AD, MT, OPL), gives the driver the
   timbre cache it asks for, and on the MT-32 (card 5) installs every effect's timbre
   from bank 1 at once. */
unsigned char far init_timbres(void)
{
    int i;
    char path[80];
    register int size;
    register int n;

    strcpy(path, "SOUND\\");
    strcat(path, "uw.");
    str_cat(path, drv_desc->data_suffix);
    size = AIL_default_timbre_cache_size(music_driver);
    if (size > 0) {
        n = size;
        if ((timbre_cache = farmalloc(n)) == 0)
            goto fail;
        AIL_define_timbre_cache(music_driver, timbre_cache, n);
    }
    if ((timbre_fd = open(path, O_RDONLY | O_BINARY)) == -1)
        goto fail;
    if (sound_card == 5)
        for (i = 0; i < numeffects; i++)
            install_timbre(1, i);
    return 1;
fail:
    music_on = 0;
    music_ok = 0;
    return 0;
}

unsigned char far install_timbre(unsigned char bank, unsigned char patch)
{
    void far *p;

    p = load_global_timbre(timbre_fd, bank, patch);
    if (p) {
        AIL_install_timbre(music_driver, bank, patch, p);
        farfree(p);
        return 1;
    }
    return 0;
}

/* Loads theme `music` (0: the current one) as the AIL sequence, unless it is already
   loaded, installing every timbre the sequence asks for; with start, starts it at normal
   tempo and volume 0x60 and centres the pitch bend of channels 2 to 10. A failure turns
   the music off for the session (music_ok = 0). */
unsigned char far load_new_music(unsigned char music, char start)
{
    register char *name = "uwr00.xmi";
    unsigned t;
    char ch;
    char path[80];

    if (!music_ok || !music_on)
        return 0;
    if (music == 0)
        music = curmusic;
    if (music != curmusic) {
        if (sound_card != 5)
            name[2] = 'a';
        name[3] = (music >> 3) + '0';
        name[4] = (music & 7) + '0';
        strcpy(path, "SOUND\\");
        strcat(path, name);
        if (!read_file_to_mbuf(path))
            goto fail;
        AIL_stop_sequence(music_driver, xmi_sequence);
        AIL_release_sequence_handle(music_driver, xmi_sequence);
        if ((xmi_sequence = AIL_register_sequence(music_driver, midi_buf, 0, state_table, 0L)) == -1)
            goto fail;
        for (t = AIL_timbre_request(music_driver, xmi_sequence); t != 0xFFFF;
             t = AIL_timbre_request(music_driver, xmi_sequence))
            if (!install_timbre(t >> 8, t & 0xFF))
                goto fail;
    }
    if (start) {
        AIL_start_sequence(music_driver, xmi_sequence);
        AIL_set_relative_tempo(music_driver, xmi_sequence, 100, 0);
        AIL_set_relative_volume(music_driver, xmi_sequence, 0x60, 0);
        for (ch = 2; ch <= 10; ch++)
            AIL_send_channel_voice_message(music_driver, ch + 0xDF, 0, 0x40);
    }
    curmusic = music;
    newmusic = 0;
    return 1;
fail:
    xmi_sequence = -1;
    music_ok = 0;
    return 0;
}

/* Loads an AIL driver file into drv_mem[n]. The block is a paragraph larger than the
   file and the driver is put at the start of its next paragraph, so it begins at
   offset 0 of a segment. */
void far * far load_sound_driver(char *name, int n)
{
    void far *p;
    int32 len;
    register FILE *fp;
    register int fd;

    if ((fp = fopen(name, "rb")) == 0)
        return 0;
    fd = fileno(fp);
    len = filelength(fd);
    fclose(fp);
    if ((drv_mem[n] = farmalloc(len + 0x10)) == 0)
        return 0;
    p = MK_FP(FP_SEG(drv_mem[n]) + 1, 0);
    if (!bltfromdrive(name, p, (unsigned)len))
        return 0;
    return p;
}

void far play_music(void)
{
    if (!music_ok || !music_on || xmi_sequence == -1)
        return;
    if (AIL_sequence_status(music_driver, xmi_sequence) != 1)
        AIL_start_sequence(music_driver, xmi_sequence);
    AIL_set_relative_volume(music_driver, xmi_sequence, 0x60, 0);
}

/* Reads a whole file into a new far block. Nothing in the sources calls it. */
void far * far seg016_1E73_19DE(char *name)
{
    int32 len;
    void far *p;
    register FILE *fp;
    register int fd;

    if ((fp = fopen(name, "rb")) == 0)
        goto fail;
    fd = fileno(fp);
    len = filelength(fd);
    fclose(fp);
    if ((p = farmalloc(len)) == 0)
        goto fail;
    if (!bltfromdrive(name, p, (unsigned)len))
        goto fail;
    return p;
fail:
    return 0;
}

unsigned char far read_file_to_mbuf(char *name)
{
    int32 len;
    register FILE *fp;
    register int fd;

    if ((fp = fopen(name, "rb")) == 0)
        goto fail;
    fd = fileno(fp);
    len = filelength(fd);
    fclose(fp);
    if (!bltfromdrive(name, midi_buf, (unsigned)len))
        goto fail;
    return 1;
fail:
    return 0;
}

unsigned char far get_current_music(void)
{
    return curmusic;
}

unsigned char far music_is_on(void)
{
    if (!music_ok)
        return 0;
    return music_on;
}

unsigned char far fx_is_on(void)
{
    if (!fx_ok)
        return 0;
    return fx_on;
}

unsigned char far fx_available(void)
{
    return fx_ok;
}

unsigned char far music_available(void)
{
    return music_ok;
}

void far turn_music(unsigned char on)
{
    if (!music_ok)
        return;
    if (on && !music_on) {
        music_on = 1;
        if (xmi_sequence == -1) {
            set_random_walking_music(-1);
            load_new_music(newmusic, 1);
        }
        AIL_start_sequence(music_driver, xmi_sequence);
    } else if ((!on & music_on) && xmi_sequence != -1) {
        music_on = 0;
        AIL_stop_sequence(music_driver, xmi_sequence);
    }
}

/* Not called anywhere and not in the FM Towns build. */
static void far toggle_music(void)
{
    if (!music_ok)
        return;
    turn_music(!music_on);
}

void far turn_fx(unsigned char on)
{
    if (!fx_ok)
        return;
    if (on)
        fx_on = 1;
    else
        fx_on = 0;
    if (!fx_on)
        kill_all_effects();
}

/* Toggles the effects. Not in the FM Towns build. */
void far seg016_1E73_1BC3(void)
{
    if (!fx_ok)
        return;
    fx_on = !fx_on;
    if (!fx_on)
        kill_all_effects();
}

void far stop_music(void)
{
    if (!music_ok || !music_on)
        return;
    AIL_stop_sequence(music_driver, xmi_sequence);
}

/* Fades the music up or down over four seconds. Not called anywhere and not in the FM
   Towns build. */
static void far fade_music(unsigned char up)
{
    if (!music_ok || !music_on || xmi_sequence == -1)
        return;
    if (up)
        AIL_set_relative_volume(music_driver, xmi_sequence, 0x60, 4000);
    else
        AIL_set_relative_volume(music_driver, xmi_sequence, 0, 4000);
}

/* Plays effect fx as if from x, y (eighths of a tile), vol added to its own volume.
   Tries the digital channel and falls back to a MIDI note. Effects 0x5A and 0x5B are
   played as effects 1 and 2, and as MIDI notes only on speech card 1. Returns the
   effect's handle for kill_effect (a MIDI slot 0 to 2, or 100 for the digital channel)
   or 0xFF. */
unsigned char far play_effect(unsigned char fx, int x, int y, char vol)
{
    struct Effect far *e;
    unsigned char midi;
    int16 v;
    int16 pan;
    unsigned char r;

    midi = 0;
    if (!fx_ok || !fx_on || fx == 0xFF)
        return 0xFF;
    if (fx > 0x63) {
        v = 0x7F;
        pan = 0x40;
    } else {
        if (fx == 0x5A)
            e = &effects[1];
        else if (fx == 0x5B)
            e = &effects[2];
        else
            e = &effects[fx];
        sound_move(x, y, e->vol + vol, &pan, &v);
        if (v == 0)
            return 0xFF;
        if (fx == 0x5A) {
            if (speech_card == 1)
                midi = 1;
            fx = 1;
        } else if (fx == 0x5B) {
            if (speech_card == 1)
                midi = 1;
            fx = 2;
        } else
            midi = 0;
    }
    if (!midi) {
        r = digi_fx_play(fx, v, pan);
        if (r != 0xFF) {
            if (fx <= 0x63) {
                digi_src.type = 1;
                digi_src.x = x;
                digi_src.y = y;
                digi_src.z = e->vol + vol;
            } else
                digi_src.type = 0;
            return r;
        }
    }
    if (fx > 0x63)
        return 0xFF;
    return fx_play(fx, e->patch, e->note, v, pan, e->length);
}

/* Plays effect fx at a given pan, with no position (the digital source is cleared, so
   it is not re-panned). */
unsigned char far play_effect_here(unsigned char fx, unsigned char pan, char vol)
{
    int v;
    unsigned char r;
    struct Effect far *e;

    if (!fx_ok || !fx_on || fx == 0xFF)
        return 0xFF;
    if (fx > 0x63)
        v = 0x7F;
    else {
        e = &effects[fx];
        v = e->vol + vol;
        if (v > 0x7F)
            v = 0x7F;
        else if (v < 0)
            v = 0;
        if (v == 0)
            return 0xFF;
    }
    r = digi_fx_play(fx, v, pan);
    if (r != 0xFF) {
        digi_src.type = 0;
        return r;
    }
    if (fx > 0x63)
        return 0xFF;
    return fx_play(fx, e->patch, e->note, v, pan, e->length);
}

unsigned char far play_effect_on_mobile(unsigned char fx, struct Object far *obj, char vol)
{
    if (!fx_ok || !fx_on || fx == 0xFF)
        return 0xFF;
    return play_effect(fx, (OBJ_HOMEX(obj) << 3) + OBJ_FINEX(obj),
                       (OBJ_HOMEY(obj) << 3) + OBJ_FINEY(obj), vol);
}

/* Plays effect fx from object obj, and if it went to the digital channel, keeps
   following the object as it moves (digi_src type 2). */
unsigned char far play_effect_on_mobile_src(unsigned char fx, struct Object far *obj, char vol)
{
    struct Effect far *e;
    int16 pan;
    int16 v;
    int r;

    if (!fx_ok || !fx_on || fx == 0xFF)
        return 0xFF;
    if (fx > 0x63)
        return 0xFF;
    e = &effects[fx];
    sound_move((OBJ_HOMEX(obj) << 3) + OBJ_FINEX(obj), (OBJ_HOMEY(obj) << 3) + OBJ_FINEY(obj),
               e->vol + vol, &pan, &v);
    r = digi_fx_play(fx, v, pan);
    if (r != 0xFF) {
        digi_src.type = 2;
        digi_src.z = e->vol + vol;
        digi_src.obj = obj;
    } else {
        r = fx_play(fx, e->patch, e->note, v, pan, e->length);
        /* match: an empty test: the direction cannot be recovered from the bytes */
        if (r == 0xFF)
            ;
    }
    return r;
}

void far kill_effect(unsigned char n)
{
    if (n >= 100) {
        stop_digi_file(n - 100);
        return;
    }
    if (fx_mask & (1 << n)) {
        fx_mask = fx_mask - (1 << n);
        if (sound_card == 1)
            AIL_send_channel_voice_message(music_driver, fx_midi_chan[n] + 0xAF, 0x7B, 0);
        else {
            AIL_send_channel_voice_message(music_driver, fx_midi_chan[n] + 0x7F, fx_note[n], 0);
            AIL_release_channel(music_driver, fx_midi_chan[n]);
        }
        fx_channels &= ~(1 << fx_midi_chan[n]);
    }
}

void far kill_all_effects(void)
{
    unsigned char i;

    for (i = 0; i < 3; i = i + 1) {
        if (fx_mask & (1 << i)) {
            fx_mask = fx_mask - (1 << i);
            if (sound_card == 1)
                AIL_send_channel_voice_message(music_driver, fx_midi_chan[i] + 0xAF, 0x7B, 0);
            else {
                AIL_send_channel_voice_message(music_driver, fx_midi_chan[i] + 0x7F, fx_note[i], 0);
                AIL_release_channel(music_driver, fx_midi_chan[i]);
            }
            fx_channels &= ~(1 << fx_midi_chan[i]);
        }
    }
    kill_all_digi_effects();
}

void far kill_all_digi_effects(void)
{
    unsigned char i;

    for (i = 0; i < 1; i++)
        stop_digi_file(i);
}

/* The timer for MIDI effects when there is no digital driver: the second half of
   digi_fx_timer. */
/* name: FM Towns has it as _fx_timer. match: static here because the target table
   counts it in kill_all_digi_effects. */
static void far fx_timer(void)
{
    unsigned char i, bit;

    if (!fx_mask)
        return;
    for (i = 0, bit = 1; i < 3; i = i + 1, bit <<= 1) {
        if ((fx_mask & bit) && fx_ticks_left[i] != -1 && --fx_ticks_left[i] == 0) {
            fx_mask = fx_mask & ~bit;
            if (sound_card == 1)
                AIL_send_channel_voice_message(music_driver, fx_midi_chan[i] + 0xAF, 0x7B, 0);
            else {
                AIL_send_channel_voice_message(music_driver, fx_midi_chan[i] + 0x7F, fx_note[i], 0);
                AIL_release_channel(music_driver, fx_midi_chan[i]);
                fx_channels &= ~(1 << fx_midi_chan[i]);
            }
        }
    }
}

/* Reads SOUND\SOUNDS.DAT: a count byte, then 8 bytes an effect: patch, note, volume,
   length (high byte first), the .VOC flag, and a priority stored as low + high * 200. */
unsigned char far read_fx_data(void)
{
    struct Effect far *e;
    unsigned char patch;
    unsigned char note;
    unsigned char vol;
    unsigned char len_hi;
    unsigned char len_lo;
    unsigned char digi;
    unsigned char pri_lo;
    unsigned char pri_hi;
    unsigned char i;
    char path[80];
    register FILE *fp;

    strcpy(path, "SOUND\\");
    strcat(path, "sounds.dat");
    if ((fp = fopen(path, "rb")) == 0)
        return 0;
    fread(&numeffects, 1, 1, fp);
    for (i = 0; i < numeffects; i = i + 1) {
        e = &effects[i];
        fread(&patch, 1, 1, fp);
        fread(&note, 1, 1, fp);
        fread(&vol, 1, 1, fp);
        fread(&len_hi, 1, 1, fp);
        fread(&len_lo, 1, 1, fp);
        fread(&digi, 1, 1, fp);
        fread(&pri_lo, 1, 1, fp);
        fread(&pri_hi, 1, 1, fp);
        e->patch = patch;
        e->note = note;
        e->vol = vol;
        e->length = (len_hi << 8) + len_lo;
        e->digi = digi;
        e->priority = pri_lo + pri_hi * 200;
    }
    fclose(fp);
    return 1;
}

unsigned char far init_fx(void)
{
    fx_mask = 0;
    if (!speechok)
        fx_clock = AIL_register_timer(fx_timer);
    else {
        init_digi_fx();
        fx_clock = AIL_register_timer(digi_fx_timer);
    }
    if (fx_clock == -1) {
        speechok = 0;
        return 0;
    }
    AIL_set_timer_frequency(fx_clock, 16);
    AIL_start_timer(fx_clock);
    if (!read_fx_data())
        return 0;
    return 1;
}

/* Plays an effect as a MIDI note on the first of three free effect slots: locks a
   channel from the music driver, selects timbre bank 1 (controller 0x72, which AIL's
   XMIDI drivers use as the patch bank select), sets the patch, centres pitch bend,
   resets the controllers, sets volume, expression and pan, and starts the note. length
   is in 1/256 seconds (the timer counts it down at 16 Hz), -1 to hold. On the PC speaker
   (card 1) only six effects sound, at fixed notes and lengths on channel 2. Returns the
   slot, or 0xFF. MIDI status bytes are written as channel + 0x8F and so on because AIL's
   channels count from 1. */
unsigned char far fx_play(unsigned char fx, unsigned char patch, unsigned char note,
                          unsigned char vel, unsigned char pan, register int length)
{
    unsigned char bit;
    unsigned char ch;
    unsigned char i;

    for (i = 0, bit = 1; (fx_mask & bit) && i < 3; i = i + 1, bit <<= 1)
        ;
    if (i == 3)
        return 0xFF;
    if (patch == 0)
        return 0xFF;
    if (sound_card == 1) {
        switch (fx) {
        case 4:
        case 0x10:
            note = 0x48;
            length = 0x10;
            break;
        case 7:
        case 8:
        case 0x15:
            note = 0x42;
            length = 8;
            break;
        case 3:
        case 0x16:
            note = 0x38;
            length = 4;
            break;
        default:
            return 0xFF;
        }
        fx_mask |= bit;
        fx_midi_chan[i] = 2;
        fx_ticks_left[i] = length;
        ch = 2;
        goto note_on;
    }
    if (AIL_timbre_status(music_driver, 1, patch) == 0)
        if (!install_timbre(1, patch))
            return 0xFF;
    if ((ch = AIL_lock_channel(music_driver)) == 0)
        return 0xFF;
    fx_channels |= 1 << ch;
    fx_mask |= bit;
    fx_midi_chan[i] = ch;
    fx_ticks_left[i] = length == -1 ? -1 : length / 16;
    fx_note[i] = note;
    fx_timbre[i] = patch;
    AIL_send_channel_voice_message(music_driver, ch + 0xAF, 0x72, 1);
    AIL_send_channel_voice_message(music_driver, ch + 0xBF, patch, 0);
    AIL_send_channel_voice_message(music_driver, ch + 0xDF, 0, 0x40);
    AIL_send_channel_voice_message(music_driver, ch + 0xAF, 0x79, 0);
    AIL_send_channel_voice_message(music_driver, ch + 0xAF, 7, 0x7F);
    AIL_send_channel_voice_message(music_driver, ch + 0xAF, 0x0B, 0x7F);
    AIL_send_channel_voice_message(music_driver, ch + 0xAF, 0x0A, pan);
note_on:
    AIL_send_channel_voice_message(music_driver, ch + 0x8F, note, vel);
    return i;
}

/* Lets the player play a musical instrument object (which: 0 to 2, patches 0x39, 0x48 and
   0x5C of bank 0). Keys 1 to 9 and 0 play ten notes upward from middle C (0x3C), Alt
   an octave higher and Ctrl an octave lower; a note stops after half a second (0x80
   ticks) or at the next key; Escape ends. Prints strings 0x109 and 0x10A around it, or
   only 0x10B when there is no sound card. The last 16 notes go into hist, which nothing
   reads here. */
void far play_instrument(register int which)
{
    char hpos;
    char n;
    int k;
    int32 t = -1;
    unsigned char ch;
    unsigned char inst[3] = { 0x39, 0x48, 0x5C };
    char notes[10] = { 0x3C, 0x3E, 0x40, 0x41, 0x43, 0x45, 0x47, 0x48, 0x4A, 0x4C };
    unsigned char last = 0xFF;
    unsigned char patch;
    unsigned char ok = 1;
    char hist[16];
    register int key;

    memset(hist, 0, 16);
    hpos = 0;
    if (sound_card == 0)
        game_sprint(0x10B);  /* 'You play the instrument.' */
    else {
        game_sprint(0x109);
        patch = inst[which];
        if (AIL_timbre_status(music_driver, 0, patch) == 0)
            if (!install_timbre(0, patch))
                ok = 0;
        if (ok == 1 && music_driver == -1)
            ok = 0;
        if (ok) {
            if (sound_card == 1)
                ch = 2;
            else {
                ch = AIL_lock_channel(music_driver);
                fx_channels |= 1 << ch;
                AIL_send_channel_voice_message(music_driver, ch + 0xAF, 0x72, 0);
                AIL_send_channel_voice_message(music_driver, ch + 0xDF, 0, 0x40);
                AIL_send_channel_voice_message(music_driver, ch + 0xBF, inst[which], 0);
                AIL_send_channel_voice_message(music_driver, ch + 0xAF, 0x79, 0);
                AIL_send_channel_voice_message(music_driver, ch + 0xAF, 7, 0x7F);
                AIL_send_channel_voice_message(music_driver, ch + 0xAF, 0x0B, 0x7F);
                AIL_send_channel_voice_message(music_driver, ch + 0xAF, 0x0A, 0x40);
            }
        }
        while ((key = mouse_get_input()) != 0x1B) {
            if (key != 0) {
                k = key & 0xFCFF;
                if (k >= '0' && k <= '9') {
                    n = k - '0';
                    if (n == 0)
                        n = 10;
                    n--;
                    n = notes[n];
                    if (key & KEY_ALT)
                        n += 12;
                    if (key & KEY_CTRL)
                        n -= 12;
                    if (t > 0 && last != 0xFF && ok)
                        AIL_send_channel_voice_message(music_driver, ch + 0x7F, last, 0);
                    hist[hpos] = n;
                    hpos++;
                    hpos &= 0x0F;
                    if (ok)
                        AIL_send_channel_voice_message(music_driver, ch + 0x8F, n, 0x7F);
                    last = n;
                    t = *Time;
                }
            }
            if (t > 0 && *Time - t > 0x80) {
                t = -1;
                if (ok)
                    AIL_send_channel_voice_message(music_driver, ch + 0x7F, last, 0);
                last = 0xFF;
            }
        }
        AIL_release_channel(music_driver, ch);
        game_sprint(0x10A);  /* 'You put the instrument away.' */
    }
}

void far free_timers(void)
{
    if (sound_started & 1) {
        AIL_stop_timer(htimer);
        AIL_release_timer_handle(htimer);
    }
}

void far free_s_mem(void)
{
    if (midi_buf)
        farfree(midi_buf);
    if (state_table)
        farfree(state_table);
    if (timbre_cache)
        farfree(timbre_cache);
    if (midi_drv)
        farfree(midi_drv);
    if (drv_mem[0]) {
        AIL_shutdown_driver(music_driver, 0L);
        farfree(drv_mem[0]);
        drv_mem[0] = 0;
    }
    if (drv_mem[1]) {
        AIL_shutdown_driver(sphdriver, 0L);
        farfree(drv_mem[1]);
        drv_mem[1] = 0;
    }
    midi_buf = state_table = timbre_cache = midi_drv = 0;
    drv_mem[0] = drv_mem[1] = 0;
}

void far free_sounds(void)
{
    if (sound_started & 2)
        AIL_shutdown("");
    free_digi_stuff();
    free_s_mem();
}

/* Asks for theme m next, unless theme 6 is playing. */
void far set_new_music(unsigned char m)
{
    if (curmusic != MUSIC_VICTORY)
        newmusic = m;
}

/* Chooses the walking theme for the player's world ((PlayerLevel - 1) / 8, nine
   worlds): pick 0 to 2, -1 for a random one, -2 to keep the current choice unless the
   world has changed. Entering a new world always takes its first theme. */
void far set_random_walking_music(register int pick)
{
    unsigned char world;

    world = (PlayerLevel - 1) / 8;
    if (world != music_world) {
        music_world = world;
        pick = 0;
    } else if (pick == -2)
        return;
    if (pick == -1)
        pick = rand() % 3;
    newmusic = walking_music[(PlayerLevel - 1) / 8][pick];
}

/* Restarts the theme when it has ended, except that themes 1, 6 and 7 are followed by
   theme 10. */
void far loop_music_maybe(void)
{
    int m;

    m = curmusic;
    if (music_over()) {
        if (curmusic == MUSIC_THEME || curmusic == MUSIC_VICTORY || curmusic == MUSIC_DEATH)
            m = 10;
        load_new_music(m, 1);
    }
}

#define WALKING(m)  ((m) >= MUSIC_WALK_FIRST && (m) <= MUSIC_WALK_LAST)
#define COMBAT(m)   ((m) >= MUSIC_FOE_HURT && (m) <= MUSIC_DANGER)

/* Called from the main loop to keep the right theme playing. Ten seconds (0xA00 ticks)
   after the last combat a combat theme gives way to theme 5 if the weapon is drawn, or
   to a walking theme. A requested theme starts at once, except that one combat theme
   replaces another at most every eight seconds (0x800 ticks). When a theme ends without
   a request: unless it was a combat theme or 0x18, a walking theme is picked when
   scrmode is 1 (probably the game screen; BAGS.C treats modes 1 and 4 so); then theme 5
   follows if the weapon is drawn and it was not a combat theme, and otherwise a walking
   theme, a combat theme or 0x18 repeats. */
void far change_music_maybe(void)
{
    if (!music_ok)
        return;
    if (!music_on)
        return;
    if (curmusic == MUSIC_VICTORY && !music_over())
        return;
    if (COMBAT(curmusic) && *Time > lastcombattime + 0xA00) {
        if (player->drawn)
            newmusic = MUSIC_ARMED;
        else
            set_random_walking_music(-1);
    }
    if (newmusic != 0 && newmusic != curmusic) {
        if (COMBAT(curmusic) && COMBAT(newmusic)) {
            if (*Time > theme_changed + 0x800) {
                load_new_music(newmusic, 1);
                theme_changed = *Time;
            } else
                newmusic = curmusic;
        } else
            load_new_music(newmusic, 1);
        if (COMBAT(newmusic))
            theme_changed = *Time;
    } else if (music_over()) {
        if ((!(WALKING(curmusic) || COMBAT(curmusic) || curmusic == MUSIC_INTRO) || WALKING(curmusic))
                && scrmode == 1)
            set_random_walking_music(-1);
        if (player->drawn && !COMBAT(curmusic))
            newmusic = MUSIC_ARMED;
        else if ((WALKING(curmusic) || COMBAT(curmusic) || curmusic == MUSIC_INTRO) && newmusic == 0)
            newmusic = curmusic;
        load_new_music(newmusic, 1);
        if (COMBAT(newmusic))
            theme_changed = *Time;
        else
            theme_changed = 0;
    }
}

unsigned char far music_over(void)
{
    if (xmi_sequence == -1)
        return 1;
    return AIL_sequence_status(music_driver, xmi_sequence) != 1;
}

/* Overrides the driver's default port, IRQ and DMA with UW.CFG's (s: IRQ, port, DMA)
   where both are set (not -1). */
void far do_settings(struct DrvrDesc far *d, register int16 *s)
{
    if (d->io != -1 && s[1] != -1)
        d->io = s[1];
    if (d->irq != -1 && s[0] != -1)
        d->irq = s[0];
    if (d->dma != -1 && s[2] != -1)
        d->dma = s[2];
}

/* Loads and starts the digital driver SOUND\DDnn.ADV for speech_card, trying UW.CFG's
   port, IRQ and DMA first and then the driver's own defaults. Sets speechok. */
unsigned char far init_speech(void)
{
    register char *name = "dd00.adv";
    int io;
    int irq;
    int dma;
    char path[80];
    register int drq;

    name[2] = (speech_card >> 3) + '0';
    name[3] = (speech_card & 7) + '0';
    strcpy(path, "SOUND\\");
    strcat(path, name);
    if ((speech_mem = load_sound_driver(path, 1)) == 0)
        goto fail;
    if ((sphdriver = AIL_register_driver(speech_mem)) == -1)
        goto fail;
    speech_descr = AIL_describe_driver(sphdriver);
    if (speech_descr->drvr_type != 2)
        goto fail;
    io = speech_descr->io;
    irq = speech_descr->irq;
    dma = speech_descr->dma;
    drq = speech_descr->drq;
    do_settings(speech_descr, speech_cfg);
    if (!AIL_detect_device(sphdriver, speech_descr->io, speech_descr->irq, speech_descr->dma, speech_descr->drq)) {
        speech_descr->io = io;
        speech_descr->irq = irq;
        speech_descr->dma = dma;
        speech_descr->drq = drq;
        if (!AIL_detect_device(sphdriver, speech_descr->io, speech_descr->irq, speech_descr->dma, speech_descr->drq))
            goto fail;
    }
    AIL_init_driver(sphdriver, speech_descr->io, speech_descr->irq, speech_descr->dma, speech_descr->drq);
    speechok = 1;
    return 1;
fail:
    speechok = 0;
    return 0;
}

unsigned char far speech_available(void)
{
    return speechok;
}

/* Stops the speech. Not called anywhere and not in the FM Towns build. */
static void far stop_speech(void)
{
    if (!speechok)
        return;
    AIL_stop_digital_playback(sphdriver);
}

void far free_speech_stuff(void)
{
}

/* Not called anywhere. FM Towns has a function of the same body, init_voc_playback_,
   elsewhere in its order. */
static unsigned char far init_voc_playback(void)
{
    return 0;
}

/* Empty, and not called anywhere. */
static void far voc_stub(void)
{
}

/* Reads the two lines of UW.CFG that set up the sound: the music card, then the speech
   card, each with its IRQ, port (hex) and DMA. Music cards are the DMnn.ADV drivers
   (1 PC speaker, 2 AdLib, 3 Sound Blaster FM, 4 and 7 Sound Blaster Pro FM, 5 MT-32,
   6 Pro Audio Spectrum FM, by the strings in the GOG release's drivers), speech cards
   the DDnn.ADV ones (1 Sound Blaster, 2 Sound Blaster Pro, 3 Pro Audio Spectrum); 0 is
   none. */
void far seg016_1E73_2FCB(FILE *fp)
{
    int card;
    int irq;
    int io;
    int dma;
    char line[100];

    fgets(line, 99, fp);
    sscanf(line, "%d %d %x %d\n", &card, &irq, &io, &dma);
    sound_card = card;
    m_settings[0] = irq;
    m_settings[1] = io;
    m_settings[2] = dma;
    fgets(line, 99, fp);
    sscanf(line, "%d %d %x %d\n", &card, &irq, &io, &dma);
    speech_card = card;
    speech_cfg[0] = irq;
    speech_cfg[1] = io;
    speech_cfg[2] = dma;
}
