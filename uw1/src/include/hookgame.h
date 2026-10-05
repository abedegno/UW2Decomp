/* hookgame.h: UW1's original tokens for the record and replay hooks of portable.h, which is
   Exhume's runtime/include/portable.h, staged beside these headers by the DOS build and
   found on the port's include path (Exhume's docs/port.md, "The runtime"). portable.h
   includes this file first. Under Turbo C each hook expands to these tokens, so the DOS
   bytes are the original ones; the names are declared by sys.h, which the hooks' users
   include. */
#define ORIG_GAME_TIME()   (*Time)              /* the 1/256 s game clock TICKS.ASM keeps (SOUND.C's cllbck_tst) */
#define ORIG_KEY()         key()                /* SYSENTRY.ASM: the next key event */
#define ORIG_MOUSE()       mouse()              /* SYSENTRY.ASM: the motion into *MouseDx, *MouseDy */
#define ORIG_MBUTTONS()    mbuttons()
#define ORIG_JOY_READ()    seg019_7CD()         /* JOYPORT.ASM: joy_position (no C source calls it in UW1) */
#define ORIG_JOY_BUTTONS() seg019_809()         /* joy_buttons (likewise) */
#define SLAVE_TIMER_HZ     16                   /* SOUND.C's effects timer, fx_timer, which AIL ran */
#define RENDER_TAG_CONTEXT dbptr                /* DRAWOBJ.C: where the sprite opcode goes */
