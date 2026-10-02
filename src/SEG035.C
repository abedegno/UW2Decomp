/* target: seg035_31AB */
/* opts: -mm -1 -G -O -d */
/* Player input and motion: the mouse and keyboard movement controls, the clock that
   drives player and object physics, head bob, footstep and swimming sounds, the noise
   the player makes, screen shake from water, lava and quakes, and placing the 3D camera.
   The whole of DOS resident segment seg035_31AB, in original order. Function and global
   names are the originals from the FM Towns symbol table except where a comment says the
   name is ours; the source file's own name is not known. */

#include <stdlib.h>
#include "critter.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"

/* This file's data, DS:073E to DS:079D. */
unsigned char pmouseHandled = 0;
unsigned char combEfflen = 0;           /* effect 0x20 */
unsigned char tremEfflen = 0;           /* effect 0x40 */
unsigned char slidEfflen = 0;           /* effect 0x80 */
unsigned char tsteps = 0;
signed char bobEffect[16] = { 1, 3, 4, 3, 1, -3, 0, 0, 1, 3, 4, 3, 1, -3, 0, 0 };
signed char sliEffect[16] = { 0, 0, -1, -2, -3, -4, -5, -6, -6, -4, -3, -2, -1, 0, 0, 0 };
signed char rorEffect[16] = { -4, -3, -2, -1, 0, 1, 2, 3, 4, 3, 2, 1, 0, -1, -2, -3 };
int PlayerInput = 0;
int PlayerTurn = 15;
/* Nothing reads these two. Their 3 bytes split as char and int, as in FM Towns, where
   _oldPlayerInput sits on the next even address after _IgnoringInput. */
char IgnoringInput = 0;
int oldPlayerInput = 0;
unsigned char water_eff = 0xFF;
char MoveCamera = 0;
int ForwInpRate = 0;
int TurnInpRate = 0;
unsigned char KeybUsed = 0;
/* The statics below have no FM Towns names (it addresses them from _TurnInpRate), so
   their names are ours. */
static unsigned char mouse_moves[3] = { 9, 8, 10 };
static unsigned char move_keys[9] = { 0x11, 0x1F, 0x1E, 0x20, 0x2C, 0x2E, 0x2D, 0x12, 0x10 };
static unsigned long last_time = 0;
static unsigned char frame_inc = 0;
static unsigned char step_sfx[6] = { 1, 2, 0x2F, 0x30, 0x1D, 0x1D };
static unsigned char step_pan[2] = { 0x38, 0x48 };
static unsigned char step_foot = 0;
static unsigned char swim_pan = 0;
static unsigned char swim_count = 0;
static unsigned char noise_count = 0;

extern struct Inplist near *inplist;
extern unsigned long far *Time;
extern int pFPS;
/* DS:19B2, the noise and visibility the player's actions add up to. */
extern char plyNotice[2];
/* This file's _BSS, DS:33C6..33E7 (seg034's ends at 33C5; seg037's starts at 33E8), laid
   out by name (tools/bssorder.py): doMod 28, vort_rad 126, vort_timer 286, vort_theta 406,
   playerMod and PlayerPitch 552, PlayerBank 576, camang 595, campos 603, vort_x and
   vort_y 1006. All FM Towns names. */
unsigned char doMod;
int playerMod[4];
int PlayerPitch;
int PlayerBank;
int campos[3];
int camang[3];
int vort_rad;
int vort_timer;
int vort_theta;
unsigned char vort_x, vort_y;

void far move_mobile(int frames);
unsigned char far play_effect_here(unsigned char fx, unsigned char pan, char vol);
char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type);

void far player_mous_move(void)
{
    if (WizEye)
        move_cam(0);
    else
    {
        parse_playin(-1);
        if (pmouseHandled == 0)
            mouse_constrain(PLeft, PBot, PLeft + PWid - 1, PBot + PHgt - 1);
        pmouseHandled = 2;
    }
}

void far parse_playin(int command)
{
    int buttons;

    if (!(KeybUsed = command >= 0))
    {
        mouse_getbut(&buttons);
        if (buttons == 1 || (buttons & 1) && player->drawn)
        {
            ForwInpRate = TurnInpRate = 0;
            if (inplist->y < PHgt / 5)
            {
                PlayerInput = mouse_moves[inplist->x * 3 / PWid];
            }
            else
            {
                if (inplist->x < PWid / 3)
                    TurnInpRate = -((PWid / 3 - inplist->x) * 0x180) / PWid;
                if (inplist->x > PWid * 2 / 3)
                    TurnInpRate = (inplist->x - PWid * 2 / 3) * 0x180 / PWid;
                if (inplist->y > PHgt * 2 / 5)
                    ForwInpRate = (inplist->y - PHgt * 2 / 5) * 0xC0 / PHgt;
                PlayerInput = 1;
            }
        }
        else if (buttons == 3)
        {
            PlayerInput = 1;
            if ((PN.terrain & 0x10) == 0 && player->motion_state != 1)
                PlayerInput = 7;
        }
    }
    else
    {
        do_player_keyboard();
        if (WizEye)
        {
            move_cam(PlayerInput + 1);
            command = 0;
        }
        switch (command)
        {
        case 6:
        case 7:
            if ((PN.terrain & 0x10) == 0 && player->motion_state != 1)
                PlayerInput = command;
            else
                PlayerInput = 1;
            break;
        case 0:
            TurnInpRate = ForwInpRate = 0;
            PlayerInput = command;
            break;
        }
    }
}

void far do_player_keyboard(void)
{
    int i;

    TurnInpRate = ForwInpRate = 0;
    if ((*Shift || *CapsLock) && (*Shift == 0 || *CapsLock == 0))
        return;
    if (*Alt || *Ctrl)
        return;
    for (i = 0; i < 9; i++)
    {
        if (key_on[move_keys[i]])
        {
            switch (move_keys[i])
            {
            case 0x11:
                ForwInpRate = 0x70;
                PlayerInput = 1;
                break;
            case 0x1F:
                ForwInpRate = 0x32;
                PlayerInput = 1;
                break;
            case 0x1E:
                TurnInpRate = -0x5A;
                PlayerInput = 1;
                break;
            case 0x20:
                TurnInpRate = 0x5A;
                PlayerInput = 1;
                break;
            case 0x2D:
                TurnInpRate = ForwInpRate = 0;
                PlayerInput = 8;
                break;
            case 0x2C:
                TurnInpRate = ForwInpRate = 0;
                PlayerInput = 9;
                break;
            case 0x2E:
                TurnInpRate = ForwInpRate = 0;
                PlayerInput = 10;
                break;
            case 0x12:
                if ((motionbits & 0x14) == 0)
                    PlayerInput = 0;
                else
                    PlayerInput = 12;
                break;
            case 0x10:
                if ((motionbits & 0x14) == 0)
                    PlayerInput = 0;
                else
                    PlayerInput = 13;
                break;
            }
        }
    }
}

void far player_simple_move(int dir)
{
    unsigned long start;
    int frames;

    start = *Time;
    if (simple_fizix(dir))
    {
        last_time = *Time;
        frame_inc = frame_inc + 4;
        PN.speed = 0;
        if (DoAnimO)
            update_animobj(1);
        player->game_clock += 0x40;
        last_time = *Time;
        frames = frame_inc;
        if (Hasted)
        {
            frames >>= 1;
            frame_inc = frame_inc & 1;
        }
        else
            frame_inc = 0;
        move_physics(0x40, frames, 1);
        editchng(10);
    }
    while (*Time - start < 0x18)
        ;
    mouse_clearQ();
}

void far check_physics(void)
{
    unsigned long delta;
    unsigned char frames;

    delta = *Time - last_time;
    if (delta > 0x40)
    {
        delta = 0x40;
        frame_inc = frame_inc + 4;
        frames = 1;
    }
    else
    {
        frame_inc = frame_inc + (unsigned char)((*Time >> 4) - (last_time >> 4));
        frames = (*Time >> 6) - (last_time >> 6);
    }
    if (delta != 0)
    {
        if (frames != 0 && DoAnimO)
            update_animobj(frames);
        player->game_clock += delta;
        last_time = *Time;
        frames = frame_inc;
        if (Hasted)
        {
            frames >>= 1;
            frame_inc = frame_inc & 1;
        }
        else
            frame_inc = 0;
        move_physics(delta, frames, 0);
    }
}

void far move_physics(int incr, int frames, unsigned char easy)
{
    doMod = 0;
    playerMod[0] = 0;
    tsteps += incr;
    if (PlayerInput == 0 && !WizEye)
        do_player_keyboard();
    if ((PlayerInput != 0 || PN.speed != 0 || PN.vel[2] != 0 || PN.acc[2] != 0 || PN.acc[1] != 0
         || PN.acc[0] != 0 || fiz_update) && !easy)
        move_player(incr);
    if (MoveCrits && !TimeStop && frames != 0)
        move_mobile(frames);
    if (player->motion_state)
        parse_effect();
    set_sound(easy);
    make_noise(easy);
}

void far finish_player(void)
{
    PlayerInput = 0;
    while (PN.speed != 0 || PN.vel[2] != 0 || PN.acc[2] != 0 || PN.acc[1] != 0 || PN.acc[0] != 0
           || fiz_update)
        move_player(0x40);
}

void far move_player(int incr)
{
    PN.radius = ComObjData[OBJ_ITEM(ThePlayer)].radius;
    PN.height = ComObjData[OBJ_ITEM(ThePlayer)].height;
    playerMod[1] = playerMod[2] = playerMod[3] = 0;
    set_player_phys_params(incr);
    do_physics(&PN, &PT);
    phys_affect_player();
    editchng(10);
    if ((PN.terrain & 0x10) == 0)
    {
        if (PN.speed > pFPS >> 2 && PlayerInput == 1)
        {
            char bob;

            if ((bob = PN.speed * 4 / (pFPS >> 1) - 1) < 2)
                bob = 2;
            doMod = 1;
            playerMod[0] = bobEffect[tsteps >> 4] * bob;
        }
        if (PlayerInput == 7)
        {
            playerMod[0] = -0x20;
            playerMod[2] = -0x100;
            doMod = 1;
            PlayerInput = 0;
        }
        if (PlayerInput == 9 || PlayerInput == 10)
        {
            doMod = 1;
            playerMod[0] = sliEffect[tsteps >> 4] * 2;
        }
    }
    PlayerInput = 0;
}

void far make_noise(char easy)
{
    unsigned char ice;
    unsigned delay;

    if (player->motion_state & 1)
    {
        if (water_eff != 0xFF && watertime + 0x1800 <= *Time)
        {
            kill_effect(water_eff);
            water_eff = 0xFF;
        }
        if (water_eff == 0xFF)
        {
            watertime = *Time;
            water_eff = play_effect_here(0, 0x40, 0);
        }
        swim_count++;
        swim_count = swim_count % 15;
        if (!swim_count)
        {
            swim_pan = !swim_pan;
            if (PN.speed != 0 && *Time > nextstep)
                play_effect_here(0x1A, step_pan[swim_pan], (PN.speed >> 5) - 0x10);
        }
    }
    else
    {
        if (water_eff != 0xFF)
        {
            kill_effect(water_eff);
            water_eff = 0xFF;
        }
        if (player->motion_state & 8)
            return;
        if (player->motion_state & 4)
        {
            if (frictionless)
                ice = 2;
            else
                ice = 1;
        }
        else
            ice = 0;
        if ((PN.terrain & 0x10) == 0)
        {
            if (easy != 0)
            {
                play_effect_here(step_sfx[ice * 2 + step_foot], step_pan[step_foot],
                                 (PN.speed >> 5) - 0x10);
                step_foot = !step_foot;
                nextstep = *Time + 100;
            }
            else if (PN.speed > 0x2F && *Time > nextstep)
            {
                play_effect_here(step_sfx[ice * 2 + step_foot], step_pan[step_foot],
                                 (PN.speed >> 5) - 0x10);
                step_foot = !step_foot;
                delay = 6000 / ((PN.speed >> 2) + 1) + 0x40;
                if (delay > 200)
                    delay = 200;
                nextstep = *Time + delay;
            }
        }
    }
}

/* IDA's ApplyPlayerSneakScore. FM Towns has set_sound_ at this position, between
   make_noise_ and parse_effect_, and its code is the same: the player's noise and
   visibility nibbles at playerdat+1Dh from _plyNotice. */
void far set_sound(char easy)
{
    char n;

    n = plyNotice[0];
    if (easy != 0)
        n = n + 4;
    else if (PN.speed == 0)
        n = 0;
    else
        n = PN.speed * 10 / pFPS + n - 5;
    if (player->motion_state)
        n = n + 4;
    if (n < 0)
        n = 0;
    else if (n > 15)
        n = 15;
    if (playerdat->noise > n)
    {
        if (noise_count == 0)
            playerdat->noise = playerdat->noise - 1;
    }
    else
        playerdat->noise = n;
    noise_count = (noise_count + 1) % 8;
    playerdat->visibility = plyNotice[1];
}

void far parse_effect(void)
{
    signed char amp = 1;
    signed char phase = 1;

    if (player->motion_state == 0)
    {
        doMod = 0;
        playerMod[0] = 0;
        return;
    }
    doMod = 1;
    editchng(2);
    playerMod[1] = playerMod[2] = playerMod[3] = 0;
    if (player->motion_state & 0x11)
    {
        playerMod[0] = -player->swim_count;
        if (player->swim_count > 0x50)
        {
            amp = PN.speed * 4 / (pFPS >> 1) - 3;
            if (amp < 1)
                amp = 1;
            phase = tsteps >> 4;
            if (PN.speed == 0)
                playerMod[3] = (rand() & 0x1FF) - 0x100;
            else
                playerMod[3] = amp * rorEffect[phase] << 6;
            playerMod[0] += rorEffect[(phase + 2) & 0xF] * 2 * amp;
            playerMod[1] = amp * ((rand() & 0x7F) - 0x40);
            playerMod[2] = amp * ((rand() & 0x7F) - 0x40);
        }
    }
    if (player->motion_state & 2)
    {
        if (rand() % 5 == 0)
        {
            damage_item(ThePlayer, 0L, 0, 0, 1, 8);
            if (player->xclock[XC_DJINN] == 3)
            {
                player->xclock[XC_DJINN] = 4;
                game_sprint(0x14E);
            }
        }
    }
    if (player->motion_state & 8)
        playerMod[0] = abs(0x10 - (tsteps >> 3)) * 3;
    if (player->motion_state & 0xE0)
    {
        if (player->motion_state & 0x40)
        {
            if (tremEfflen-- == 0)
            {
                player->motion_state = player->motion_state ^ 0x40;
                editchng(2);
            }
            amp = tremEfflen / 10;
            if (amp > 8)
                amp = 8;
        }
        if (player->motion_state & 0x20)
        {
            if (combEfflen-- == 0)
            {
                player->motion_state = player->motion_state ^ 0x20;
                editchng(2);
            }
            phase = combEfflen / 8;
            if (phase > 3)
                phase = 3;
        }
        if (player->motion_state & 0x80)
        {
            if (slidEfflen-- == 0)
            {
                player->motion_state = player->motion_state ^ 0x80;
                editchng(2);
            }
            phase = 0;
            playerMod[1] += ((rand() & 0x1FF) - 0x100) << 2;
        }
        amp += phase;
        playerMod[1] += amp * ((rand() & 0xFF) - 0x80);
        playerMod[2] += amp * ((rand() & 0x7F) - 0x40);
        playerMod[3] += amp * ((rand() & 0x1FF) - 0x100);
    }
}

void far set_effect(unsigned char which, char amount)
{
    switch (which)
    {
    case 0x20:
        combEfflen = amount;
        break;
    case 0x40:
        tremEfflen = amount;
        break;
    case 0x80:
        slidEfflen = amount;
        break;
    default:
        return;
    }
    player->motion_state |= which;
}

/* IDA's PositionCameraAtObject. FM Towns has get_eye_ at this position, after
   set_effect_, and its code is the same: the camera follows the player, a trap camera,
   another object, the object behind the player, or the moongate vortex. */
void far get_eye(void)
{
    int x, y;

    if (UsPtr == ThePlayer)
    {
        cPlayer->x = PN.x;
        cPlayer->y = PN.y;
        cPlayer->z = PN.z + 0xA4;
        cPlayer->heading = PlayerFacing;
        cPlayer->pitch = PlayerPitch;
        cPlayer->bank = PlayerBank;
        if (doMod)
        {
            cPlayer->z += playerMod[0];
            if (cPlayer->z > 1000)
                cPlayer->z = 1000;
            cPlayer->heading += playerMod[1];
            cPlayer->pitch += playerMod[2];
            cPlayer->bank += playerMod[3];
        }
    }
    else if (UsPtr == 0)
    {
        cPlayer->x = campos[0];
        cPlayer->z = campos[2];
        cPlayer->y = campos[1];
        cPlayer->heading = camang[0];
        cPlayer->pitch = camang[1];
        cPlayer->bank = camang[2];
    }
    else if (UsPtr > critdata)
    {
        cPlayer->x = (OBJ_HOMEX(UsPtr) << 8) + (OBJ_FINEX(UsPtr) << 5);
        cPlayer->y = (OBJ_HOMEY(UsPtr) << 8) + (OBJ_FINEY(UsPtr) << 5);
        cPlayer->z = (OBJ_Z(UsPtr) << 3) + 0xB0;
        cPlayer->heading = (OBJ_HEADING(UsPtr) << 13) + (OBJ_FINEHEAD(UsPtr) << 8);
    }
    else if (UsPtr == critdata - 1)
    {
        cSinCos(PlayerFacing, &x, &y);
        cPlayer->x = PN.x - (x >> 7);
        cPlayer->y = PN.y - (y >> 7);
        cPlayer->z = PN.z + 0x148;
        cPlayer->heading = PlayerFacing;
        cPlayer->pitch = PlayerPitch;
        cPlayer->bank = PlayerBank;
    }
    else if (UsPtr == critdata - 2)
    {
        cSinCos(vort_theta, &x, &y);
        x = x / 0x100;
        y = y / 0x100;
        x = x * (0x40 - vort_timer) / 0x40;
        y = y * (0x40 - vort_timer) / 0x40;
        cPlayer->x = (vort_x << 8) + x * vort_rad / 2 + 0x80;
        cPlayer->y = (vort_y << 8) + y * vort_rad / 2 + 0x80;
        cPlayer->z = PN.z + 0xA4 - vort_timer * 2;
        cPlayer->heading = vort_theta + 0x7FFF;
        cPlayer->pitch = 0;
        cPlayer->bank = vort_timer << 11;
    }
}
