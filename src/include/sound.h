/* sound.h: Sound and music, and the AIL driver API. */
#ifndef SOUND_H
#define SOUND_H

#include "uw2.h"

struct DrvrDesc;
struct Object;
struct SoundBuff;

#include "object.h"

/* AIL's sound buffer, 12 bytes. */
struct SoundBuff {
    unsigned pack_type;
    unsigned sample_rate;
    char far *data;                     /* 0x04 */
    unsigned long len;                  /* 0x08 */
};

/* AIL's description of a driver. */
struct DrvrDesc {
    unsigned min_api;
    unsigned drvr_type;                 /* 0x02: 2 digital, 3 XMIDI */
    char data_suffix[4];                /* 0x04 */
    char far *dev_names;                /* 0x08 */
    int io, irq, dma, drq;              /* 0x0C */
};

/* SOUND.C: sound and music */
extern struct SoundBuff dsbuf[2];
extern unsigned char speechok;
extern int sphdriver;
extern char far *dsdata[2];
unsigned char far digi_fx_play(unsigned char fx, unsigned char vol, unsigned char pan);
void far stop_digi_file(unsigned char chan);
void far free_s_mem(void);
unsigned char far init_speech(void);
unsigned char far init_fx(void);
void far do_settings(struct DrvrDesc far *d, int *s);
void far set_random_walking_music(int pick);
unsigned char far read_fx_data(void);
void far kill_all_effects(void);
void far * far load_sound_driver(char *name, int n);
unsigned char far init_timbres(void);
unsigned char far install_timbre(unsigned char bank, unsigned char patch);
unsigned char far read_file_to_mbuf(char *name);
unsigned char far fx_play(unsigned char fx, unsigned char patch, unsigned char note,
                          unsigned char vel, unsigned char pan, int length);
unsigned char far music_over(void);
void far punt_all_digi_fx(void);
void far update_digi_playback(void);
void far play_music(void);
unsigned char far get_current_music(void);
unsigned char far music_is_on(void);
unsigned char far fx_is_on(void);
unsigned char far fx_available(void);
unsigned char far music_available(void);
void far turn_music(unsigned char on);
void far turn_fx(unsigned char on);
void far stop_music(void);
unsigned char far play_effect_on_mobile_src(unsigned char fx, struct Object far *obj, char vol);
void far kill_effect(unsigned char n);
void far play_instrument(int which);
void far free_timers(void);
void far free_sounds(void);
void far loop_music_maybe(void);
void far change_music_maybe(void);
unsigned char far speech_available(void);
void far free_speech_stuff(void);

/* AIL.ASM */
unsigned far AIL_default_timbre_cache_size(int drv);
void far AIL_define_timbre_cache(int drv, void far *cache, unsigned size);
struct DrvrDesc far * far AIL_describe_driver(int drv);
int far AIL_detect_device(int drv, int io, int irq, int dma, int drq);
int far AIL_index_VOC_block(int drv, void far *voc, int block, struct SoundBuff far *buf);
void far AIL_init_driver(int drv, int io, int irq, int dma, int drq);
void far AIL_install_timbre(int drv, int bank, int patch, void far *src);
int far AIL_lock_channel(int drv);
void far AIL_pause_digital_playback(int drv);
int far AIL_register_driver(void far *driver);
int far AIL_register_sequence(int drv, void far *xmid, int n, void far *state, void far *ctrl);
void far AIL_register_sound_buffer(int drv, int n, struct SoundBuff far *buf);
int far AIL_register_timer(void (far *fn)(void));
void far AIL_release_channel(int drv, int ch);
void far AIL_release_sequence_handle(int drv, int seq);
void far AIL_release_timer_handle(int timer);
void far AIL_resume_digital_playback(int drv);
void far AIL_send_channel_voice_message(int drv, int status, int d1, int d2);
unsigned far AIL_sequence_status(int drv, int seq);
void far AIL_set_digital_playback_panpot(int drv, int p);
void far AIL_set_digital_playback_volume(int drv, int v);
void far AIL_set_relative_tempo(int drv, int seq, int percent, int ms);
void far AIL_set_relative_volume(int drv, int seq, int percent, int ms);
void far AIL_set_timer_frequency(int timer, unsigned long hertz);
void far AIL_shutdown(char far *msg);
void far AIL_shutdown_driver(int drv, char far *msg);
unsigned far AIL_sound_buffer_status(int driver, int buffer);
void far AIL_start_digital_playback(int driver);
void far AIL_start_sequence(int drv, int seq);
void far AIL_start_timer(int timer);
/* AIL, the Audio Interface Library (seg022). Its entry points are stubs in the order of the
   FM Towns build's _AIL_ symbols, which name them; the timer and driver calls are confirmed
   by the FM Towns callers of each. */
void far AIL_startup(void);
unsigned far AIL_state_table_size(int drv);
void far AIL_stop_digital_playback(int driver);
void far AIL_stop_sequence(int drv, int seq);
void far AIL_stop_timer(int timer);
unsigned far AIL_timbre_request(int drv, int seq);
int far AIL_timbre_status(int drv, int bank, int patch);

#endif
