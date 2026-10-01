/* target: seg011 */
/* opts: -mm -1 -G -O -Y -d */
/* Joystick motion, button sampling, calibration and filtered cursor input. */

extern int far *joy_position, far *joy_buttons;
extern long far *Time;
extern unsigned char IsJoy;
extern int PlayerInput;
extern int ForwInpRate, TurnInpRate;
extern int joy_center_x, joy_center_y;
extern int joy_min_x, joy_max_x, joy_min_y, joy_max_y;
extern int joymovecur, fauxright;
extern long joy_last_tick;
extern int joy_interval, joy_divisor, joy_limit, joy_gain, joy_clamp;
extern int joy_filter_x, joy_filter_y;

void far seg021_22FD_7CD(void);
void far seg021_22FD_809(void);
void far loop_music_maybe(void);
int far do_keyboard_input(int mode);
void far game_sprint(int string);
void far scroll_print(char far *string);
void far scroll_clear(int value);

void far seg011_6(void)
{
    int x, y;
    seg021_22FD_7CD();
    x = joy_position[0] - joy_center_x;
    y = joy_position[1] - joy_center_y;
    if (x > 0) x = (int)((long)x * 127) / joy_max_x;
    else x = (int)((long)x * 127) / -joy_min_x;
    if (y > 0) y = (int)((long)y * 127) / joy_max_y;
    else y = (int)((long)y * 127) / -joy_min_y;
    if (x > 127) x = 127;
    else if (x < -127) x = -127;
    if (y > 127) y = 127;
    else if (y < -127) y = -127;
    seg021_22FD_809();
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

unsigned char far seg011_12B(int *x, int *y)
{
    int key;
    for (seg021_22FD_809(); joy_buttons[0] || joy_buttons[1]; seg021_22FD_809()) ;
    goto check_press;
poll_again:
    seg021_22FD_809();
    loop_music_maybe();
check_press:
    {
        if (joy_buttons[0] == 0 && joy_buttons[1] == 0) {
            if ((key = do_keyboard_input(0)) < 0) goto poll_again;
        }
        if (key == 27) return 0;
        if (joy_buttons[0] == 0 && joy_buttons[1] == 0) goto check_press;
    }
    seg021_22FD_7CD();
    *x = joy_position[0] - joy_center_x;
    *y = joy_position[1] - joy_center_y;
    return 1;
}

void far JoyStickCalibration_seg011_1B8(void)
{
    unsigned char ok;
    int low_x = joy_min_x;
    int high_x = joy_max_x;
    int low_y = joy_min_y;
    int high_y = joy_max_y;
    if (!IsJoy) return;
    scroll_clear(0);
    seg021_22FD_7CD();
    joy_center_x = joy_position[0];
    joy_center_y = joy_position[1];
    game_sprint(0x16c);
    ok = seg011_12B(&high_x, &low_y);
    if (!ok) {
        game_sprint(0x16d);
        return;
    }
    game_sprint(0x16e);
    ok = seg011_12B(&low_x, &high_y);
    if (!ok) {
        game_sprint(0x16d);
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

void far seg011_2C6(int *out_x, int *out_y)
{
    register int x, y;
    int scaled_x, rate_x, rate_y, max_step, abs_x, abs_y;
    x = joy_position[0] - joy_center_x;
    y = joy_position[1] - joy_center_y;
    if (x > 0) x = (int)((long)x * 127) / joy_max_x;
    else x = (int)((long)x * 127) / -joy_min_x;
    if (y > 0) y = (int)((long)y * 127) / joy_max_y;
    else y = (int)((long)y * 127) / -joy_min_y;
    if (x > 127) x = 127;
    else if (x < -127) x = -127;
    if (y > 127) y = 127;
    else if (y < -127) y = -127;
    if ((unsigned long)(*Time - joy_last_tick) > (long)joy_interval) {
        register int scaled_y;
        joy_last_tick = *Time;
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
