/* target: ovr142 */
/* opts: -mm -1 -G -O -Y -d */

#include "combat.h"
#include "critter.h"
#include "file.h"
#include "gfx.h"
#include "inv.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "ui.h"
#include "view3d.h"

/* This file's _BSS, DS:8288..8297, laid out by name (tools/bssorder.py): player_name_handle
   80, player 280, playerdat 440, ThePlayer 444, PlayerLevel 568, PlayerFacing 704,
   PlayerHeading 768. It follows ovr140's TxmID (988) and ends where ovr143's IsJoy (1)
   starts another run, and ovr143's run holds its own static region handles, so this run
   is not ovr143's. */
int player_name_handle;
struct Player *player;
struct Creature *playerdat;
struct Object far *ThePlayer;
int PlayerLevel;
int PlayerFacing;
int PlayerHeading;
extern unsigned long lastDurCheck;
extern unsigned char cmbModTH[4];
extern unsigned char plyregen;
extern unsigned PickDist;

/* This file's _DATA runs from DS:19AC to the end of "dl.dat" at DS:19DB. */
/* Body slot to defence index; FM Towns keeps it as a static (_k_modes+0x10). */
static signed char defence_slot_index[6] = {3, 0, 1, 2, 2, 0};
/* [0] noise, [1] visibility; seg035 reads both as plyNotice[2]. */
unsigned char plyNotice[2] = {0x0F, 0x0F};
char UsingPole = 0;
unsigned char light_mod = 0xFF;
signed char light_act = 0xFF;
signed char loc_lght = 0xFF;
unsigned char light_hi = 0;
unsigned char last_light = 0x0C;
/* Statics in FM Towns (_last_light+1, +2); the names are descriptive. */
static char ShroomsEnabled = -1;
static char ShroomsRelated = 1;
/* Damage-protection bits for class 3 minors 5..9; FM Towns indexes it from
   _loc_lght as well, so the source subtracted 5 from the minor class. */
static unsigned char damage_protection_flags[5] = {0x40, 0x08, 0x10, 0x01, 0x02};
void far write(int fd, void *p, int n);
void far read(int fd, void *p, int n);
void far xorwrite(int fd, unsigned char key, void far *p, unsigned n);
void far xorread(int fd, unsigned char key, void far *p, unsigned n);
void far newFPS(int n);
void far memset(void *p, int value, int count);
int far rand(void);
void far grfx_quikpal(int n);
long far lseek(int fd, long pos, int whence);
void far close(int fd);

/* IDA left this empty function unnamed; FM Towns correspondence is unconfirmed. */
void far MaybePlayerDayLoadrelated_ovr142_0(void) {}

void far save_player_data(int fd)
{
    unsigned char key;
    key = player->name[0] ^ 0xAA;
    player->strength = playerdat->attr[0];
    player->dexterity = playerdat->attr[1];
    player->intelligence = playerdat->attr[2];
    player->health = ThePlayer->hp;
    player->maxhealth = playerdat->avghit;
    player->saved_x = PN.x;
    player->saved_y = PN.y;
    player->saved_z = PN.z;
    player->saved_facing = PlayerFacing;
    player->saved_level = PlayerLevel;
    player->sound = (unsigned)fx_is_on();
    player->music = (unsigned)music_is_on();
    player->terrain = PN.terrain;
    write(fd, &key, 1);
    xorwrite(fd, key, player, 0x37D);
}

void far read_player_data(int fd)
{
    unsigned char key;
    read(fd, &key, 1);
    xorread(fd, key, player, 0x37D);
    playerdat->attr[0] = player->strength;
    playerdat->attr[1] = player->dexterity;
    playerdat->attr[2] = player->intelligence;
    ThePlayer->hp = player->health;
    playerdat->avghit = player->maxhealth;
    PN.x = player->saved_x;
    PN.y = player->saved_y;
    PN.z = player->saved_z;
    PlayerFacing = player->saved_facing;
    PlayerLevel = player->saved_level;
    PN.terrain = player->terrain;
    lastDurCheck = player->game_clock >> 8;
    turn_fx(player->sound);
    turn_music(player->music);
    set_graphics_level();
    newFPS(player->fps);
}

void far init_spells(void)
{
    ComObjData[127].resist = 0;         /* item 127 is the player's own object type */
    plyNotice[0] = 13 - player->skills[SKILL_STEALTH] / 3;
    plyNotice[1] = 15 - player->skills[SKILL_STEALTH] / 5;
    motionbits = 0;
    memset(cmbModTH, 0, 4);
    PoisonWeap = Hasted = WizEye = Blessed = TimeStop = 0;
    Valor = 0;
    PickDist = 0x90;
    if (UsingPole) PickDist = 0x190;
    plyregen = 0;
}

void far swap_tmap(void) {}

void far set_drugged(char on)
{
    if (on) {
        if (ShroomsEnabled < 0) {
            if (ShroomsRelated) {
                ShroomsEnabled = 0;
                ShroomsRelated = 0;
            } else
                ShroomsEnabled = rand() % 3;
            switch (ShroomsEnabled) {
            case 0: set_cyb(1); break;
            case 1: grfx_quikpal(rand() & 7); break;
            case 2: random_light(1); break;
            }
        }
    } else if (ShroomsEnabled >= 0) {
        switch (ShroomsEnabled) {
        case 0: set_cyb(0); break;
        case 1: grfx_quikpal(PAL_GAME); break;
        case 2: random_light(0); break;
        }
        ShroomsEnabled = -1;
    }
}

/* FM Towns player_affected_by_ is between set_drugged_ and parse_aspells_ and
   applies the same spell classes; IDA left its DOS name descriptive. */
unsigned char far player_affected_by(unsigned char major, unsigned char minor,
                                    register unsigned *bonuses, int slot)
{
    register int i;
    switch (major) {
    case 2:
        if ((*bonuses >> 4) < minor)
            *bonuses = (*bonuses & 0xF) + (minor << 4);
        break;
    case 3:
        switch (minor - 1) {
        case 0:
            cmbModTH[0] += 3;
            cmbModTH[1] += 3;
            cmbModTH[2] += 3;
            cmbModTH[3] += 3;
            break;
        case 1: case 2: case 3:
            *bonuses |= 1 << (minor - 1);
            if (minor == 4) *bonuses |= 2;
            break;
        case 4: case 5: case 6: case 7: case 8:
            ComObjData[127].resist |= damage_protection_flags[minor - 5];
            break;
        case 9:
            Valor = 10 + player->skills[SKILL_CASTING] / 5;
            break;
        case 10:
            PoisonWeap = 1;
            break;
        }
        break;
    case 1:
        motionbits = motionbits | (1 << (minor - 1));
        break;
    case 0:
        if (((player->light & 0xF0) >> 4) < minor)
            player->light = minor << 4;
        break;
    case 11:
        switch (minor) {
        case 0: TimeStop = 1; break;
        case 1: WizEye = 1; break;
        case 2: Hasted = 1; break;
        case 3: PickDist = 0; break;
        case 14: plyregen |= 1; break;
        case 15: plyregen |= 2; break;
        }
        break;
    case 9:
        backfire(ThePlayer, minor);
        break;
    case 12:
        if (slot < 0) break;
        {
        int slots[2] = {-1, -1};
        i = 0;
        if (slot > 4) {
            slots[0] = 0;
            slots[1] = 1;
        } else
            slots[0] = defence_slot_index[slot];
        for (; slots[i] != -1 && i < 2; i++) {
            register int toughness = 0;
            if (minor & 8)
                toughness = (minor & 7) + 1;
            else
                cmbModTH[slots[i]] = (minor & 7) + cmbModTH[slots[i]] + 1;
            playerdat->armour[slots[0]] = playerdat->armour[slots[0]] + toughness;
        }
        }
        break;
    }
    return 0;
}

/* Spell icon base for each major class; a static in FM Towns (_last_light+8). */
static unsigned char spell_class_values[16] = {
    0x14, 0xFF, 0x13, 0x05, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x11, 0x80, 0x80, 0x80, 0x80
};
extern struct Armour Armor[];

void far parse_aspells(unsigned char *out)
{
    unsigned char i;
    memset(out, 0x1E, 3);
    for (i = 0; i < player->active_spells; i++) {
        out[i] = spell_class_values[player->spells[i] & 0xF];
        out[i] = out[i] + ((player->spells[i] & 0xF0) >> 4);
    }
}

void far parse_spells(unsigned bonuses)
{
    unsigned char i;
    unsigned char spells[3];
    for (i = 0; i < 4; i++, bonuses = bonuses >> 1) {
        if (bonuses & 1) {
            switch (i) {
            case 1:
                plyNotice[0] -= plyNotice[0] > 0x10 ? 0x10 : plyNotice[0];
                break;
            case 2:
                plyNotice[1] -= plyNotice[1] > 5 ? 5 : plyNotice[1];
                break;
            case 3:
                plyNotice[1] -= plyNotice[1] > 0x10 ? 0x10 : plyNotice[1];
                break;
            }
        }
    }
    for (i = 0; i < 4; i++)
        playerdat->armour[i] = playerdat->armour[i] + ((bonuses >> 4) & 0xF);
    parse_aspells(spells);
    active_spells(spells);
}

/* FM Towns armor_val_ occupies this position and performs the same calculation. */
int far armor_val(struct Object far *obj)
{
    register int armour;
    register int protection;
    if (OBJ_MAJOR(obj) == MAJOR_HACK && OBJ_MINOR(obj) < 2)
        return 0;
    armour = Armor[OBJ_ITEM(obj) - FIRST_ARMOR].protection;
    protection = ((unsigned)(obj->qn.f.quality * armour)) >> 6;
    protection++;
    return protection;
}

extern signed char ValidLightSlots[];
extern struct Weapon Weapons[];
char far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag);
void far set_light(signed char n);
void far newFPS(int n);

void far FixPlayerEquips(void)
{
    int brightness, best_slot, armour;
    unsigned bonuses;
    unsigned char flag;
    int major, effect;
    struct Object far *item;
    register int slot;
    unsigned char *data;
    bonuses = 0;
    for (slot = 0; slot < 4; slot++) playerdat->armour[slot] = 0;
    for (slot = 0; slot <= 4; slot++) {
        if (item = AskInventory(slot))
            playerdat->armour[defence_slot_index[slot]] += armor_val(item);
    }
    item = AskInventory(player->lefty + 7);
    if (item && OBJ_MAJOR(item) == MAJOR_HACK &&
        OBJ_MINOR(item) == 3 &&
        OBJ_INCLASS(item) >= 11 && OBJ_INCLASS(item) <= 15) {
        armour = armor_val(item);
        playerdat->armour[0] += armour;
        playerdat->armour[1] += armour;
    }
    playerdat->defence = player->skills[SKILL_DEFENSE];
    ActiveObj = AskInventory(8 - player->lefty);
    armour = 2;
    if (ActiveObj && OBJ_MAJOR(ActiveObj) == MAJOR_HACK &&
        OBJ_MINOR(ActiveObj) < 2) {
        if (OBJ_MINOR(ActiveObj) == 0) {
            armour = Weapons[ActiveObj->id & ID_INCLASS].skill;
            if (armour < 3) armour = 3;
            else if (armour > 5) armour = 5;
            load_weapon((unsigned char)armour + 0xFD);
        } else if (OBJ_INCLASS(ActiveObj) <= 7)
            load_weapon(3);
        else
            load_weapon(-1);
    } else load_weapon(3);
    playerdat->defence = playerdat->defence + (player->skills[armour] >> 1);
    init_spells();
    best_slot = 0;
    brightness = 0;
    for (slot = 0; slot <= 4; slot++) {
        if (slot == 4) ActiveObj = CursorObjPtr;
        else ActiveObj = AskInventory(ValidLightSlots[slot]);
        if (ActiveObj && OBJ_CLASS(ActiveObj) == CLASS_LIGHT &&
            OBJ_INCLASS(ActiveObj) >= 4 && OBJ_INCLASS(ActiveObj) < 8) {
            data = get_class_data();
            if (data[1] > brightness) {
                brightness = data[1];
                best_slot = slot;
            }
        }
    }
    player->light = (brightness << 4) + best_slot;
    for (slot = 0; slot < player->active_spells; slot++)
        player_affected_by(player->spells[slot] & 0xF,
                           (player->spells[slot] & 0xF0) >> 4, &bonuses, -1);
    for (slot = 0; slot <= 10; slot++) {
        ActiveObj = AskInventory(slot);
        if (ActiveObj && ObjWorn(ActiveObj->id & ID_ITEM, slot) &&
            decode_obj_spell(ActiveObj, &major, &effect, &flag) && !flag &&
            player_affected_by(major, effect, &bonuses, slot))
            remove_spell(ActiveObj);
    }
    parse_spells(bonuses);
    if (WizEye) light_act = 6;
    else light_act = (player->light & 0xF0) >> 4;
    if (light_act > loc_lght) set_light(light_act);
    else set_light(loc_lght);
    set_drugged(player->shrooms > 0);
    if (player->sleepbits && player->in_void)
        motionbits |= 0x10;
    fizix_update();
    newFPS(-1);
}

void far load_dl(void)
{
    unsigned char value;
    register int fd;
    fd = our_open("dl.dat", 1, 0);
    if (fd >= 0) {
        lseek(fd, (long)(PlayerLevel - 1), 0);
        read(fd, &value, 1);
        close(fd);
        light_mod = value % 10;
        light_hi = value >= 10;
        if (light_hi)
            loc_lght = light_mod;
        else
            loc_lght = 0xFF;
        last_light = 0x0C;
    }
}
