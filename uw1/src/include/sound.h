/* sound.h: Sound and music, and the AIL driver API. SOUND.C (resident seg016) plays the
   effects and music; AIL.ASM (seg022) is Miles' AIL 2 application interface, which
   loads nothing itself and passes most calls on to the .ADV driver.
   docs/subsystems/sound.md describes the subsystem. */
#ifndef SOUND_H
#define SOUND_H

#include "uw2.h"

struct DrvrDesc;
struct Object;
struct SoundBuff;

#include "object.h"

/* AIL's sound buffer, 12 bytes: what AIL_register_sound_buffer plays. SOUND.C fills two
   of them (dsbuf) in turn from its EMS cache; AIL_index_VOC_block sets pack_type and
   sample_rate from a .VOC header. */
HOST_LAYOUT_BEGIN
struct SoundBuff {
    uint16 pack_type;
    uint16 sample_rate;
    char far *data;                     /* 0x04 */
    uint32 len;                         /* 0x08 */
};
HOST_LAYOUT_END

/* AIL's description of a driver, returned by AIL_describe_driver. AIL_init_driver also
   reads a word at 0x14, past these fields, as the driver's service rate in hertz (-1
   for none). */
HOST_LAYOUT_BEGIN
struct DrvrDesc {
    uint16 min_api;
    uint16 drvr_type;                   /* 0x02: 2 digital, 3 XMIDI */
    char data_suffix[4];                /* 0x04 */
    char far *dev_names;                /* 0x08 */
    int16 io, irq, dma, drq;            /* 0x0C */
};
HOST_LAYOUT_END

/* Music themes, for set_new_music and load_new_music: theme n is SOUND\UWAnn.XMI (UWRnn.XMI
   for the Roland card) with nn the number in octal (load_new_music builds the name). The
   names say when the game asks for each; UW-Formats' song list ("Enemy wounded", "Combat",
   "Dangerous Situation", "Armed", "Victory") agrees for 2 to 6. */
#define MUSIC_THEME     1               /* the main menu and start-up */
#define MUSIC_FOE_HURT  2               /* the player's foe is nearly dead (AI.C) */
#define MUSIC_COMBAT    3
#define MUSIC_DANGER    4               /* the player is badly hurt */
#define MUSIC_ARMED     5               /* the weapon is drawn */
#define MUSIC_VICTORY   6               /* a creature is killed (COMBAT.C) */
#define MUSIC_DEATH     7               /* the player dies (SKILLS.C) */
#define MUSIC_WALK_FIRST 8              /* the walking themes, 8 to 15 (UWA10..UWA17), chosen */
#define MUSIC_WALK_LAST 15              /* by world (SOUND.C's walking_music) */
#define MUSIC_INTRO     0x18            /* UWA30 */

/* SOUND.C: sound and music */
extern char speechok;
extern int16 sphdriver;
extern char far *dsdata[2];
char far init_speech(void);
char far init_fx(void);
void far do_settings(struct DrvrDesc far *d, int16 *s);
char far read_fx_data(void);
void far * far load_sound_driver(char *name);
char far init_timbres(void);
char far install_timbre(unsigned char bank, unsigned char patch);
char far read_file_to_mbuf(char *name);
unsigned char far fx_play(unsigned char fx, unsigned char patch, unsigned char note,
                          unsigned char vel, unsigned char pan, int length);
char far music_over(void);
void far play_music(void);
unsigned char far get_current_music(void);
char far music_is_on(void);
char far fx_is_on(void);
void far turn_music(char on);
void far turn_fx(char on);
void far stop_music(void);
void far kill_effect(unsigned char n);
void far play_instrument(int which);
void far free_timers(void);
void far free_sounds(void);
void far loop_music_maybe(void);
void far change_music_maybe(void);
char far speech_available(void);
void far free_speech_stuff(void);
unsigned char far init_timers(void);
unsigned char far init_sounds(void);
unsigned char far load_new_music(unsigned char music, char start);
unsigned char far play_effect(unsigned char fx, int x, int y, char vol);
unsigned char far play_effect_here(unsigned char fx, unsigned char pan, char vol);
unsigned char far play_effect_on_mobile(unsigned char fx, struct Object far *obj, char vol);
void far set_new_music(unsigned char m);
void far seg014_1DC5_C7C(void);
char far play_cup_tune(char *played);
void far seg014_1DC5_15C5(void);
char far init_voc(void);
char far play_speech(int n);
void far update_speech(void);
char far speech_over(void);
void far stop_speech(void);
void far seg014_1DC5_1D0D(FILE *fp);

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
void far AIL_set_timer_frequency(int timer, uint32 hertz);
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
