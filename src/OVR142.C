/* target: ovr142 */
/* opts: -mm -1 -G -O -Y -d */

struct Player {
    unsigned char key;
    char pad01[0x1e - 1];
    unsigned char strength, dexterity, intelligence;
    unsigned char attack, defence;
    char pad23[0x2e - 0x23];
    unsigned char sneak;
    char pad2f[0x35 - 0x2f];
    unsigned char health, maxhealth;
    char pad37[0x3e - 0x37];
    unsigned spells[17];
    union { unsigned word; struct { unsigned low:5, count:4, rest:3, shrooms:2, high:2; } bits; } spell_count;
    char pad62[2];
    unsigned char light;
    union { unsigned char handedness; struct { unsigned char hand:1, hand_rest:7; } bits; } handopts;
    char pad66[0x302 - 0x66];
    union { unsigned options; struct { unsigned sound:2, music:2, padopts:12; } opt; } opts;
    char pad304[0x369 - 0x304];
    unsigned long clock;
};
struct MotionOpts { unsigned low:3, state:8, high:5; };
struct HandBits { unsigned hand:1, high:15; };
struct DreamOpts { unsigned low:6, dream:3, active:1, high:6; };
struct Creature { unsigned char armour[4]; unsigned char vitality, strength, dexterity, intelligence; char pad08[0x12-8]; unsigned char defence; };
struct Object { unsigned id, pos; union { unsigned word; struct { unsigned quality:6, next:10; } f; } qn; unsigned owner; unsigned char hp; };
struct Motion { int x,y,z; char pad06[0x25-6]; unsigned char state; };
extern struct Motion PN;

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
extern unsigned char motionbits, TimeStop, Hasted, WizEye, PoisonWeap;
extern unsigned char Blessed;
/* The common object properties, 11 bytes per item (ovr134 loads them). Only the
   resistance byte of item 127, the player's own object type, is used here. */
struct ComObj { char pad0[8]; unsigned char resist; char pad9[2]; };
extern struct ComObj ComObjData[];
extern unsigned char cmbModTH[4];
extern unsigned char Valor, plyregen;
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
unsigned char far fx_is_on(void);
unsigned char far music_is_on(void);
void far turn_fx(int n);
void far turn_music(int n);
void far set_graphics_level(void);
void far newFPS(int n);
void far memset(void *p, int value, int count);
int far rand(void);
void far set_cyb(int n);
void far grfx_quikpal(int n);
void far random_light(int n);
int far our_open(char *name, int mode, int flags);
long far lseek(int fd, long pos, int whence);
void far close(int fd);

/* IDA left this empty function unnamed; FM Towns correspondence is unconfirmed. */
void far MaybePlayerDayLoadrelated_ovr142_0(void) {}

void far save_player_data(int fd)
{
    unsigned char key;
    key = player->key ^ 0xAA;
    player->strength = playerdat->strength;
    player->dexterity = playerdat->dexterity;
    player->intelligence = playerdat->intelligence;
    player->health = ThePlayer->hp;
    player->maxhealth = playerdat->vitality;
    *(int *)((char *)player + 0x54) = PN.x;
    *(int *)((char *)player + 0x56) = PN.y;
    *(int *)((char *)player + 0x58) = PN.z;
    *(int *)((char *)player + 0x5A) = PlayerFacing;
    *(int *)((char *)player + 0x5C) = PlayerLevel;
    player->opts.opt.sound = (unsigned)fx_is_on();
    player->opts.opt.music = (unsigned)music_is_on();
    ((struct MotionOpts *)((char *)player + 0x303))->state = PN.state;
    write(fd, &key, 1);
    xorwrite(fd, key, player, 0x37D);
}

void far read_player_data(int fd)
{
    unsigned char key;
    read(fd, &key, 1);
    xorread(fd, key, player, 0x37D);
    playerdat->strength = player->strength;
    playerdat->dexterity = player->dexterity;
    playerdat->intelligence = player->intelligence;
    ThePlayer->hp = player->health;
    playerdat->vitality = player->maxhealth;
    PN.x = *(int *)((char *)player + 0x54);
    PN.y = *(int *)((char *)player + 0x56);
    PN.z = *(int *)((char *)player + 0x58);
    PlayerFacing = *(int *)((char *)player + 0x5A);
    PlayerLevel = *(int *)((char *)player + 0x5C);
    PN.state = ((struct MotionOpts *)((char *)player + 0x303))->state;
    lastDurCheck = player->clock >> 8;
    turn_fx(player->opts.opt.sound);
    turn_music(player->opts.opt.music);
    set_graphics_level();
    newFPS(((struct MotionOpts *)((char *)player + 0x303))->low);
}

void far init_spells(void)
{
    ComObjData[127].resist = 0;
    plyNotice[0] = 13 - player->sneak / 3;
    plyNotice[1] = 15 - player->sneak / 5;
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
        case 1: grfx_quikpal(0); break;
        case 2: random_light(0); break;
        }
        ShroomsEnabled = -1;
    }
}

void far backfire(struct Object far *obj, char strength);

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
            Valor = 10 + (*(unsigned char *)((char *)player + 0x2A)) / 5;
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
extern unsigned char Armor[];
void far active_spells(unsigned char *p);

void far parse_aspells(unsigned char *out)
{
    unsigned char i;
    memset(out, 0x1E, 3);
    for (i = 0; i < player->spell_count.bits.count; i++) {
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
    if (((obj->id & 0x1C0) >> 6) == 0 && ((obj->id & 0x30) >> 4) < 2)
        return 0;
    armour = Armor[((obj->id & 0x1FF) - 0x20) * 4];
    protection = ((unsigned)(obj->qn.f.quality * armour)) >> 6;
    protection++;
    return protection;
}

extern struct Object far *ActiveObj;
extern struct Object far *CursorObjPtr;
extern signed char ValidLightSlots[];
extern unsigned char Weapons[];
struct Object far * far AskInventory(int slot);
char far ObjWorn(int item, int slot);
char far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag);
void far remove_spell(struct Object far *obj);
char far GetItemEnchantment(struct Object far *obj, int *major, int *effect, unsigned char *flag);
void far load_weapon(char n);
void far set_light(signed char n);
void far fizix_update(void);
void far newFPS(int n);
unsigned char * far get_class_data(void);

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
    item = AskInventory((((struct HandBits *)((char *)player + 0x65))->hand) + 7);
    if (item && ((item->id & 0x1C0) >> 6) == 0 &&
        ((item->id & 0x30) >> 4) == 3 &&
        (item->id & 0xF) >= 11 && (item->id & 0xF) <= 15) {
        armour = armor_val(item);
        playerdat->armour[0] += armour;
        playerdat->armour[1] += armour;
    }
    playerdat->defence = player->defence;
    ActiveObj = AskInventory(8 - (((struct HandBits *)((char *)player + 0x65))->hand));
    armour = 2;
    if (ActiveObj && ((ActiveObj->id & 0x1C0) >> 6) == 0 &&
        ((ActiveObj->id & 0x30) >> 4) < 2) {
        if (((ActiveObj->id & 0x30) >> 4) == 0) {
            armour = Weapons[(ActiveObj->id & 0xF) * 8 + 6];
            if (armour < 3) armour = 3;
            else if (armour > 5) armour = 5;
            load_weapon((unsigned char)armour + 0xFD);
        } else if ((ActiveObj->id & 0xF) <= 7)
            load_weapon(3);
        else
            load_weapon(-1);
    } else load_weapon(3);
    playerdat->defence = playerdat->defence + (((unsigned char *)player + armour)[0x21] >> 1);
    init_spells();
    best_slot = 0;
    brightness = 0;
    for (slot = 0; slot <= 4; slot++) {
        if (slot == 4) ActiveObj = CursorObjPtr;
        else ActiveObj = AskInventory(ValidLightSlots[slot]);
        if (ActiveObj && ((ActiveObj->id & 0x1F0) >> 4) == 9 &&
            (ActiveObj->id & 0xF) >= 4 && (ActiveObj->id & 0xF) < 8) {
            data = get_class_data();
            if (data[1] > brightness) {
                brightness = data[1];
                best_slot = slot;
            }
        }
    }
    player->light = (brightness << 4) + best_slot;
    for (slot = 0; slot < player->spell_count.bits.count; slot++)
        player_affected_by(player->spells[slot] & 0xF,
                           (player->spells[slot] & 0xF0) >> 4, &bonuses, -1);
    for (slot = 0; slot <= 10; slot++) {
        ActiveObj = AskInventory(slot);
        if (ActiveObj && ObjWorn(ActiveObj->id & 0x1FF, slot) &&
            decode_obj_spell(ActiveObj, &major, &effect, &flag) && !flag &&
            player_affected_by(major, effect, &bonuses, slot))
            remove_spell(ActiveObj);
    }
    parse_spells(bonuses);
    if (WizEye) light_act = 6;
    else light_act = (player->light & 0xF0) >> 4;
    if (light_act > loc_lght) set_light(light_act);
    else set_light(loc_lght);
    set_drugged(player->spell_count.bits.shrooms > 0);
    if (((struct DreamOpts *)((char *)player + 0x62))->dream &&
        ((struct DreamOpts *)((char *)player + 0x62))->active)
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
