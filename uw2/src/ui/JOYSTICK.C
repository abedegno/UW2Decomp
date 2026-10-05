/* target: seg011 */
/* opts: -mm -1 -G -O -Y -d */
/* The joystick: walking and turning from the stick, its two buttons, calibration, and
   moving the mouse pointer with the stick. DOS only (the FM Towns build has none of it).

   Entry points: seg011_6, called by MOUSE.C's moveMouse on every poll when IsJoy is set,
   reads the stick and, unless button 1 is held, turns it into player motion
   (PlayerInput, ForwInpRate, TurnInpRate, as PLAYMOVE.C's keys do); it also copies the
   buttons into MOUSE.C's joymovecur (button 1: the stick steers the pointer) and fauxright
   (button 2: acts as the right mouse button). seg011_2C6 gives moveMouse the pointer
   motion while button 1 is held. JoyStickCalibration_seg011_1B8 is bound to Ctrl+J
   (0x16A, DEBUG.C) and asks the player to push the stick to two corners.

   Data owned: the calibration (centre, and the extremes each side, initially +-127), the
   pointer filter's constants and state, and the time of the last filter step. The raw
   readings come from seg021's joystick routines (seg021_22FD_7CD reads the position into
   joy_position, seg021_22FD_809 the buttons into joy_buttons, sys.h).

   Name: descriptive (joystick motion and calibration, DOS only). */

#include "motion.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"


/* this file's _DATA, DS:00E8 to DS:0106, in definition order */
int16 joy_center_x = 0, joy_center_y = 0;
int16 joy_min_x = -127, joy_max_x = 127, joy_min_y = -127, joy_max_y = 127;
int16 joy_divisor = 16, joy_interval = 5, joy_clamp = 16, joy_gain = 20, joy_limit = 40;
int16 joy_filter_x = 0, joy_filter_y = 0;
int32 joy_last_tick = 0;


/* The stick as movement: the reading is scaled to -127..127 by the calibration. With
   button 1 up, a dead zone of 20 each way; pulled back (y above 20) sets PlayerInput 8,
   which PLAYMOVE.C also uses for the X key (step back), with no rate; pushed forward walks
   at a rate of the push less 20 and left or right turns likewise. */
void far seg011_6(int unused)
{
    int x, y;
    JOY_READ();
    x = joy_position[0] - joy_center_x;
    y = joy_position[1] - joy_center_y;
    if (x > 0) x = (int)((int32)x * 127) / joy_max_x;
    else x = (int)((int32)x * 127) / -joy_min_x;
    if (y > 0) y = (int)((int32)y * 127) / joy_max_y;
    else y = (int)((int32)y * 127) / -joy_min_y;
    if (x > 127) x = 127;
    else if (x < -127) x = -127;
    if (y > 127) y = 127;
    else if (y < -127) y = -127;
    JOY_BUTTONS();
    joymovecur = joy_buttons[0];
    fauxright = joy_buttons[1];
    if (joymovecur == 0) {
        if (y > 20) {
            PlayerInput = 8;
            ForwInpRate = TurnInpRate = 0;
            return;
        } else if (y < -20) {
            PlayerInput = 1;
            ForwInpRate = -y - 20;
        }
        if (x < -20) {
            PlayerInput = 1;
            TurnInpRate = x + 20;
        } else if (x > 20) {
            PlayerInput = 1;
            TurnInpRate = x - 20;
        }
    }
}

/* For calibration: waits for both buttons to be released, then for a press (playing
   music meanwhile), and returns the stick's offset from the centre in *x and *y. Escape
   aborts and returns 0. */
unsigned char far seg011_12B(int16 *x, int16 *y)
{
    int key;
    for (JOY_BUTTONS(); joy_buttons[0] || joy_buttons[1]; JOY_BUTTONS()) ;
    goto check_press;
poll_again:
    JOY_BUTTONS();
    loop_music_maybe();
check_press:
    {
        if (joy_buttons[0] == 0 && joy_buttons[1] == 0) {
            if ((key = do_keyboard_input(0)) < 0) goto poll_again;
        }
        if (key == 27) return 0;
        if (joy_buttons[0] == 0 && joy_buttons[1] == 0) goto check_press;
    }
    JOY_READ();
    *x = joy_position[0] - joy_center_x;
    *y = joy_position[1] - joy_center_y;
    return 1;
}

/* Calibrates: the stick's rest position becomes the centre, then the player pushes it
   away and right (giving the maximum x and the minimum y) and pulls it toward and left
   (the minimum x and maximum y). A range under 10 or a zero reading on an axis falls back
   to +-127 for that axis. */
void far JoyStickCalibration_seg011_1B8(void)
{
    unsigned char ok;
    int16 low_x = joy_min_x;
    int16 high_x = joy_max_x;
    int16 low_y = joy_min_y;
    int16 high_y = joy_max_y;
    if (!IsJoy) return;
    scroll_clear(0);
    JOY_READ();
    joy_center_x = joy_position[0];
    joy_center_y = joy_position[1];
    game_sprint(0x16c);                 /* "Centering joystick....centered.\nPush ..." */
    ok = seg011_12B(&high_x, &low_y);
    if (!ok) {
        game_sprint(0x16d);             /* "Aborted.\n" */
        return;
    }
    game_sprint(0x16e);                 /* "ok\nPull joystick towards and to left, ..." */
    ok = seg011_12B(&low_x, &high_y);
    if (!ok) {
        game_sprint(0x16d);             /* "Aborted.\n" */
        return;
    }
    if (high_x - low_x < 10 || high_x == 0 || low_x == 0) {
        high_x = 127;
        low_x = -127;
    }
    if (high_y - low_y < 10 || high_y == 0 || low_y == 0) {
        high_y = 127;
        low_y = -127;
    }
    joy_min_x = low_x;
    joy_min_y = low_y;
    joy_max_x = high_x;
    joy_max_y = high_y;
    scroll_print("ok\n");
}

/* The stick as a pointer, called by moveMouse while button 1 is held: every
   joy_interval (5) ticks, the scaled reading divided by joy_divisor (16) is added to a
   running value that is then multiplied by a gain proportional to the deflection
   (joy_gain / joy_limit, at most 20/40 of it at full push) and clamped to +-16 pixels a
   step. Between steps it reports no motion. */
void far seg011_2C6(int16 *out_x, int16 *out_y, int unused)
{
    register int x, y;
    int scaled_x, rate_x, rate_y, max_step, abs_x, abs_y;
    x = joy_position[0] - joy_center_x;
    y = joy_position[1] - joy_center_y;
    if (x > 0) x = (int)((int32)x * 127) / joy_max_x;
    else x = (int)((int32)x * 127) / -joy_min_x;
    if (y > 0) y = (int)((int32)y * 127) / joy_max_y;
    else y = (int)((int32)y * 127) / -joy_min_y;
    if (x > 127) x = 127;
    else if (x < -127) x = -127;
    if (y > 127) y = 127;
    else if (y < -127) y = -127;
    if ((uint32)(GAME_TIME() - joy_last_tick) > (int32)joy_interval) {
        register int scaled_y;
        joy_last_tick = GAME_TIME();
        max_step = 127 / joy_divisor;
        scaled_x = x / joy_divisor;
        scaled_y = y / joy_divisor;
        abs_x = scaled_x < 0 ? -scaled_x : scaled_x;
        abs_y = scaled_y < 0 ? -scaled_y : scaled_y;
        rate_x = joy_gain * abs_x / max_step;
        rate_y = joy_gain * abs_y / max_step;
        joy_filter_x += scaled_x;
        joy_filter_y += scaled_y;
        joy_filter_x = rate_x * joy_filter_x / joy_limit;
        joy_filter_y = rate_y * joy_filter_y / joy_limit;
        if (joy_filter_x < -joy_clamp) joy_filter_x = -joy_clamp;
        else if (joy_filter_x > joy_clamp) joy_filter_x = joy_clamp;
        if (joy_filter_y < -joy_clamp) joy_filter_y = -joy_clamp;
        else if (joy_filter_y > joy_clamp) joy_filter_y = joy_clamp;
        *out_x = joy_filter_x;
        *out_y = joy_filter_y;
    } else {
        *out_x = 0;
        *out_y = 0;
    }
}
