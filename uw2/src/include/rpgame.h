/* rpgame.h: UW2's side of the record and replay harness, Exhume's runtime/replay/replay.c,
   which both the replay DOS build and the port compile where it is (docs/PORT.md, "The
   runtime"); replay.c includes this file. What the hooks read (seg021's clock and key state,
   the mouse and joystick), how UW2 shuts down, its clock's rate, C0's null-pointer checksum,
   and the state dump's sections: the player record, the level block, the far blocks'
   segments and the graphics library's data. replay.c's first comment has the formats. */
#include "sys.h"
#include "gfx.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "view3d.h"

#define RP_MAGIC            "UW2R"
#define RP_CLOCK            (*Time)             /* the 1/256 s clock */
#define RP_KEY_READ()       key()
#define RP_KEYSTATE         Shift               /* FD71:01E2..026A: Shift, Alt, Ctrl, the other */
#define RP_KEYSTATE_LEN     0x89                /* modifiers, CapsLock, the layout, key_on[0..127] */
#define RP_STOP_SCAN        0x58                /* F12 ends a recording */
#define RP_MOUSE_READ()     mouse()
#define RP_MOUSE_DX         (*MouseDx)
#define RP_MOUSE_DY         (*MouseDy)
#define RP_BUTTONS_READ()   mbuttons()
#define RP_JOY_READ()       seg021_22FD_7CD()
#define RP_JOY_X            joy_position[0]
#define RP_JOY_Y            joy_position[1]
#define RP_JOYB_READ()      seg021_22FD_809()
#define RP_JOYB_0           joy_buttons[0]
#define RP_JOYB_1           joy_buttons[1]
#define RP_SHUTDOWN()       free_world(1)
#define RP_TICK_US          3906UL              /* the 256 Hz clock's PIT period */
#define RP_PIT_DIVISOR      4661u               /* 3906 * 10000 / 8380, AIL's set_PIT_period */
#define RP_NULL_SUM_FROM    4                   /* UW2's C0 sums DS:4..30h */
#define RP_NULL_SUM         0x0CA5
#define RP_LEVEL            (mapdata && LEVEL->magic == LEVEL_MAGIC)
#define GFX_LEN             0x5E80              /* seg003's data, 370D:0000..5E7F */

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

/* The sections, in UW2's order:
     PLYR  the player record, PlayerDat (37Eh)
     RAND  Borland's rand seed
     LEVL  the level block mapdata points at (7E08h), once a level is in it (its magic word is
           "uw"; before that the block holds whatever memory held)
     NULL  DS:0..30h, C0's checksum and the vector table
     SEGS  the segments of the far blocks, as words: seg_370D (the graphics data), stdat,
           cmpbuf1_start, dfx_buffer, seg004's data (EmsBuff's), seg_5DFD, seg021's data
           (Shift's), mapdata, and EmsBuff itself, the EMS page frame
     CNTS  the calls of each stream
     FBUF  with UWRPFB set, once a level is in: the 3D view's frame buffer, stdat's first 69D6h
           bytes, in every dump, so that with a short UWRPCK every 3D frame is compared
   and in a full dump
     GFX   seg003's data, seg_370D, 370D:0000..5E7F
     PAL, CRTC, VGA */
#define RP_NSECTIONS(full, level, fb) (((level) ? 6 : 5) + ((full) ? 4 : 0) + ((fb) && (level)))
#define RP_DUMP(full, level, fb) do {                                   \
    section("PLYR", sizeof PlayerDat);                                  \
    dump_bytes(&PlayerDat, sizeof PlayerDat);                           \
    dump_rand();                                                        \
    if (level) {                                                        \
        section("LEVL", 0x7E08);                                        \
        dump_far((unsigned char far *)mapdata, 0x7E08);                 \
    }                                                                   \
    dump_null();                                                        \
    section("SEGS", 18);                                                \
    dump_word(FP_SEG(palette));                                         \
    dump_word(FP_SEG(stdat));                                           \
    dump_word(FP_SEG(cmpbuf1_start));                                   \
    dump_word(FP_SEG(dfx_buffer));                                      \
    dump_word(FP_SEG(&EmsBuff));                                        \
    dump_word(FP_SEG(seg_5DFD));                                        \
    dump_word(FP_SEG(Shift));                                           \
    dump_word(FP_SEG(mapdata));                                         \
    dump_word(EmsBuff);                                                 \
    dump_counts();                                                      \
    if ((fb) && (level)) {                                              \
        section("FBUF", 0x69D6);                                        \
        dump_far((unsigned char far *)stdat, 0x69D6);                   \
    }                                                                   \
    if (full) {                                                         \
        section("GFX ", GFX_LEN);                                       \
        dump_far(palette - 0x5048, GFX_LEN);                            \
        dump_screen();                                                  \
    }                                                                   \
} while (0)
