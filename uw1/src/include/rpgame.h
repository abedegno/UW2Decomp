/* rpgame.h: UW1's side of the record and replay harness, Exhume's runtime/replay/replay.c,
   which the replay DOS build compiles where it is (docs/BUILDING.md, "Recording and replaying
   a session") and a port will compile too; replay.c includes this file. What the hooks read
   (seg019's clock and key state, the mouse and joystick), how UW1 shuts down, its clock's
   rate, C0's null-pointer checksum, and the state dump's sections: the player record, the
   level block, the far blocks' segments and the graphics library's data. replay.c's first
   comment has the formats. The gate never compiles this file. */
#include <dos.h>
#include "sys.h"
#include "gfx.h"
#include "level.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "view3d.h"

/* PLAYER.C: the player record's storage, struct Player (0xD2 bytes) */
extern unsigned char PlayerDat[];

#define RP_MAGIC            "UW1R"
#define RP_CLOCK            (*Time)             /* the 1/256 s clock (SOUND.C's cllbck_tst) */
#define RP_KEY_READ()       key()
/* the key state seg063 keeps at 01E2..026A, as KEYQUEUE.ASM uses it (the same offsets as
   UW2's): Shift 1E2, Alt 1E3, Ctrl 1E4, the other modifiers 1E5..1E7, CapsLock 1E8, the layout
   word 1E9, key_on[0..127] 1EB..26A */
#define RP_KEYSTATE         Shift
#define RP_KEYSTATE_LEN     0x89
#define RP_STOP_SCAN        0x58                /* F12 ends a recording */
#define RP_MOUSE_READ()     mouse()
#define RP_MOUSE_DX         (*MouseDx)
#define RP_MOUSE_DY         (*MouseDy)
#define RP_BUTTONS_READ()   mbuttons()
/* the joystick: no UW1 source reads it (JOYPORT.ASM's routines have no C caller), so these
   are never reached; they are here because replay.c's rp_joy and rp_joyb name them */
#define RP_JOY_READ()       seg019_7CD()
#define RP_JOY_X            joy_position[0]
#define RP_JOY_Y            joy_position[1]
#define RP_JOYB_READ()      seg019_809()
#define RP_JOYB_0           joy_buttons[0]
#define RP_JOYB_1           joy_buttons[1]
#define RP_SHUTDOWN()       free_world(1)       /* what main does after mainloop */
/* SOUND.C's init_timers: an AIL timer at 0x100 Hz, so a period of 1000000 / 256 = 3906 us,
   and AIL.ASM's set_PIT_period makes the divisor 3906 * 10000 / 8380 = 4661 */
#define RP_TICK_US          3906UL
#define RP_PIT_DIVISOR      4661u
/* C0 (tools/link.py's c0_source) sums DS:4..30h, from DATASEG@, and subtracts 0CA5h, the sum
   of those bytes in UW.EXE (the copyright string's) */
#define RP_NULL_SUM_FROM    4
#define RP_NULL_SUM         0x0CA5
#define RP_LEVEL            (mapdata && LEVEL->magic == LEVEL_MAGIC)
/* seg048, the graphics library's data (FD52): the segment's paragraph 3963h from offset 0 to
   5E7Fh. FD52 itself is 3963:0008..5E7B in the link map; palette is seg048+5046h
   (GRCORE.ASM's _DATA), so palette - 5046h is 3963:0000 */
#define GFX_PALETTE_OFF     0x5046
#define GFX_LEN             0x5E80
/* the 3D view's frame buffer at stdat:0, through its last pixel, which GRENTRY.ASM's cLiteFB
   starts from (stdat:4CCE) */
#define FBUF_LEN            0x4CCF

/* the port: AIL's ticks follow the replayed clock, and the SOUND stream asks the port's drivers */
#ifndef __TURBOC__
void port_clock_read(uint32 t);                 /* the runtime's sound/ail.c */
#define RP_PORT_CLOCK_READ(t) port_clock_read(t)
#define RP_PORT_SOUND 1
void port_idle(void);                           /* the runtime's sys/pit.c: rest while the game waits on the clock */
#define RP_PORT_IDLE() port_idle()
void port_pause_wait(void);                     /* the runtime's sys/pit.c: wait while the settings screen is open */
#define RP_PORT_PAUSE() port_pause_wait()
#endif

/* The sections, in this order:
     PLYR  the player record, PlayerDat (D2h)
     RAND  Borland's rand seed
     LEVL  the level block mapdata points at (7C08h), once a level is in it (its magic word is
           "uw"; before that the block holds whatever memory held)
     NULL  DS:0..30h, C0's checksum and the vector table
     SEGS  the segments of the far blocks, as words: seg048 (palette's), stdat, cmpbuf1_start,
           seg_5DFD, EmsBuff's, seg063 (Shift's), mapdata, EmsBuff itself (the EMS page frame),
           and the VGA's window, A000h (VIDMODE.ASM keeps the segment it draws to at 3963:55EA,
           A000h for the screen)
     CNTS  the calls of each stream
     FBUF  with UWRPFB set, once a level is in: the 3D view's frame buffer, stdat's first
           4CCFh bytes, in every dump
   and in a full dump
     GFX   seg048's data, 3963:0000..5E7F
     PAL, CRTC, VGA */
#define RP_NSECTIONS(full, level, fb) (((level) ? 6 : 5) + ((full) ? 4 : 0) + ((fb) && (level)))
#define RP_DUMP(full, level, fb) do {                                   \
    section("PLYR", sizeof(struct Player));                             \
    dump_bytes(PlayerDat, sizeof(struct Player));                       \
    dump_rand();                                                        \
    if (level) {                                                        \
        section("LEVL", LEVEL_SIZE);                                    \
        dump_far((unsigned char far *)mapdata, LEVEL_SIZE);             \
    }                                                                   \
    dump_null();                                                        \
    section("SEGS", 18);                                                \
    dump_word(FP_SEG(palette));                                         \
    dump_word(FP_SEG(stdat));                                           \
    dump_word(FP_SEG(cmpbuf1_start));                                   \
    dump_word(FP_SEG(seg_5DFD));                                        \
    dump_word(FP_SEG(&EmsBuff));                                        \
    dump_word(FP_SEG(Shift));                                           \
    dump_word(FP_SEG(mapdata));                                         \
    dump_word(EmsBuff);                                                 \
    dump_word(FP_SEG((unsigned char far *)MK_FP(0xA000, 0)));          \
    dump_counts();                                                      \
    if ((fb) && (level)) {                                              \
        section("FBUF", FBUF_LEN);                                      \
        dump_far((unsigned char far *)stdat, FBUF_LEN);                 \
    }                                                                   \
    if (full) {                                                         \
        section("GFX ", GFX_LEN);                                       \
        dump_far(palette - GFX_PALETTE_OFF, GFX_LEN);                   \
        dump_screen();                                                  \
    }                                                                   \
} while (0)
