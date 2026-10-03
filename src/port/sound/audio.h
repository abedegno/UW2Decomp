/* audio.h: replaces nothing; the port's sound hardware (audio.c), which the drivers of ail.c
   write to: the FM chips (Nuked OPL3), the MT-32 (libmt32emu), the Sound Blaster's DAC, and
   the mixer that renders them to the platform's audio stream and to a WAV file. */
#ifndef UW2_AUDIO_H
#define UW2_AUDIO_H

#include <stdint.h>

#define AUDIO_RATE 44100

/* the options (src/port/sys/main.c): a WAV file to write everything rendered to, the
   directory of the user's MT-32 or CM-32L ROMs, and whether to open the audio device */
void audio_config(const char *wav_path, const char *mt32_roms, int device);
void audio_start(void);
int audio_mt32_roms_present(const char *dir);
void audio_music_device(int kind);
void audio_opl_write(int chip, unsigned reg, unsigned val);
void audio_mpu_byte(unsigned b);
void audio_dac_play(const uint8_t *data, uint32_t n, uint32_t rate, uint64_t t_us);
void audio_dac_stop(uint64_t t_us);
void audio_dac_level(unsigned left, unsigned right, uint64_t t_us);
/* renders the hardware's output up to the AIL clock's t_us, with the registers as they are */
void audio_render_to(uint64_t t_us);
void audio_flush(void);
/* a host file for writing (the logs), closed at exit */
void *audio_host_fopen_v(const char *path);
#define audio_host_fopen(p) ((FILE *)audio_host_fopen_v(p))

/* ail.c, for main.c and pit.c */
void ail_set_logs(const char *ail_path, const char *hw_path);
void ail_set_slaved(int on);
void ail_pit_advance(uint64_t ns);
void port_clock_read(uint32_t t);

#endif
