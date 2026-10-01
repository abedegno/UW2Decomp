/* target: seg033_2FBE */
/* opts: -mm -1 -G -O -Y -d */
/* Drawing one object into the 3D view's render database: do_obj picks the kind of
   drawing from the object's common data, do_rect sends a 3D model, do_door a door.
   The whole of DOS resident segment seg033_2FBE, in original order. Function and global
   names are the originals from the FM Towns symbol table where it has them. */

#include <dos.h>

struct Object {
    unsigned id;                        /* item 0-8, flags 9-15 */
    unsigned pos;                       /* z 0-6, heading 7-9, y fine 10-12, x fine 13-15 */
    unsigned qn;                        /* 0x04 */
    unsigned owner:6;                   /* 0x06 */
    unsigned link:10;
    char pad08[0x0B - 0x08];
    unsigned w0B;                       /* 0x0B, x in bits 0-7, frame in bits 12-15 */
    unsigned w0D;                       /* 0x0D, y in bits 0-7 */
    char pad0F[0x14 - 0x0F];
    unsigned char b14;                  /* 0x14 */
    unsigned char b15;                  /* 0x15, animation in bits 0-5 */
    char pad16[0x18 - 0x16];
    unsigned char b18;                  /* 0x18 */
};

struct Player {
    char pad0[0xE8];
    unsigned char gems;                 /* 0xE8 */
    char padE9[0x105 - 0xE9];
    unsigned w105;                      /* 0x105 */
    char pad107[0x302 - 0x107];
    unsigned b302:4;                    /* 0x302 */
    unsigned detail:4;
};

/* The common object properties, one 11-byte record per item. */
struct ComObj {
    char pad0[9];
    unsigned char b9_0:2;               /* 0x09, how the object is drawn */
    unsigned char b9_2:6;
    char padA[0x0B - 0x0A];
};

/* The camera, a copy of the player's position. */
struct Eye {
    char pad0[0x0A];
    int x;                              /* 0x0A, in 1/256 tiles */
    char pad1[0x12 - 0x0C];
    int y;                              /* 0x12 */
    char pad2[0x2C - 0x14];
    unsigned heading;                   /* 0x2C */
};

struct Grs3d {
    unsigned char page;
    char b1;
};

struct Tile {
    unsigned type:4;
    unsigned height:4;
    unsigned b1:2;
    unsigned floor:4;                   /* bits 10..13 */
    unsigned b1_6:2;
    unsigned wall:6;                    /* 0x02, wall texture */
    unsigned objects:10;                /* head of the tile's object list */
};

/* tmapson, lighton, curautocode and the other scalars at DS:534-53F, and the tables
   after them up to DS:5F9, belong to the file before this one in the data segment
   (FM Towns puts them with process_grid and txtwal), so they are extern here. */
extern struct Object far *ThePlayer;
extern unsigned char PickUp;
extern struct Tile far *mlowptr;
extern struct Tile far *tmptr;
extern int mptrmod;
extern int TileIndices[];
extern int ObjectIndices[];
extern int far *dbptr;
extern char quad;
extern int objxloc, objyloc, objzloc;
extern struct ComObj ComObjData[];
extern char AnimObjInPipe;
extern struct Eye far *cPlayer;
extern unsigned headmod[4];
extern struct Grs3d far grs_3dinf[];
extern unsigned far EmsBuff;
extern unsigned char locsqmod;
extern int lighton;
extern int tmapson;
extern unsigned first_tmobj;
extern char tCacheOK;
extern unsigned char sqmod;
extern int cTmBm;
extern int cTmDm;
extern unsigned TxmTerr[];
extern unsigned char curautocode;
extern unsigned char TxmCol[];
extern unsigned long far *Time;
extern struct Player near *player;
extern struct Object far *objdata;
extern unsigned far bmhgtoff;

/* Per model, as do_rect reads it: the flags (bits 0-2 the number of colours that
   follow, 0x08 an extra 0x4C command below the model, 0x10 a textured face, 0x20 the
   caller's texture colour, 0x40 turned by the object's own heading, 0x80 colours
   animated by the clock), three colours, then the first texture and the number of
   textures for a textured face. */
unsigned char rect_cols[32][5] = {
    { 0x01, 0x8F, 0x00, 0x00, 0x00 },
    { 0x21, 0x93, 0x00, 0x00, 0x00 },
    { 0x11, 0x8F, 0x00, 0x1E, 0x02 },
    { 0x01, 0x8C, 0x00, 0x00, 0x00 },
    { 0x02, 0x21, 0x1B, 0x00, 0x00 },
    { 0x02, 0xC6, 0xCC, 0x00, 0x00 },
    { 0x02, 0xC6, 0xCC, 0x00, 0x00 },
    { 0x02, 0xC6, 0xCC, 0x00, 0x00 },
    { 0x43, 0x88, 0x23, 0x56, 0x00 },
    { 0x01, 0x8C, 0x00, 0x00, 0x00 },
    { 0x19, 0x8C, 0x00, 0x00, 0x04 },
    { 0x03, 0xB9, 0xBA, 0xBC, 0x00 },
    { 0x01, 0xCA, 0x00, 0x00, 0x00 },
    { 0x11, 0x8D, 0x00, 0x2A, 0x06 },
    { 0x11, 0x8F, 0x00, 0x00, 0x00 },
    { 0x21, 0x8F, 0x00, 0x00, 0x00 },
    { 0x51, 0x1B, 0x00, 0x04, 0x08 },
    { 0x51, 0x1B, 0x00, 0x0C, 0x08 },
    { 0x11, 0x1B, 0x00, 0x14, 0x08 },
    { 0x11, 0xCB, 0x00, 0x1C, 0x02 },
    { 0x51, 0x1B, 0x00, 0x00, 0x00 },
    { 0x11, 0x1B, 0x00, 0x00, 0x00 },
    { 0x21, 0x1B, 0x00, 0x00, 0x00 },
    { 0x83, 0x00, 0x02, 0x04, 0x00 },
    { 0x12, 0x8C, 0xCA, 0x20, 0x04 },
    { 0x02, 0x8E, 0xCA, 0x00, 0x00 },
    { 0x01, 0x8C, 0x00, 0x00, 0x00 },
    { 0x02, 0x8C, 0xCB, 0x8E, 0x00 },
    { 0x11, 0x8F, 0x00, 0x26, 0x04 },
    { 0x04, 0x31, 0xC6, 0x52, 0x4D },
    { 0x02, 0xCC, 0x02, 0x00, 0x00 },
    { 0x12, 0x15, 0x35, 0x24, 0x02 }
};
/* The model drawn for each 3D object type 0x10-0x2F, -1 for none. */
int rect_sub[32] = {
    0x03, 0x08, 0x08, 0x07, 0x07, 0x06, 0x05, 0x0B,
    0x18, 0x09, 0x17, 0x1B, 0x1C, 0x19, 0x1A, 0x04,
    0x0A, 0x10, 0x11, 0x0D, 0x02, 0x13, 0x12, 0x1D,
    0x1E, 0x1F, -1, -1, -1, -1, -1, -1
};
/* An NPC's heading relative to the camera, in 32nds, as one of eight animation views. */
unsigned char dirtab[32] = {
    0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4,
    4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 0, 0
};
/* Colours for model 2 when its flags are below its texture count, indexed by the
   object's flags. Static: FM Towns has no name for it and keeps it after dirtab too
   (as dirtab+0x20), so the name is ours. */
static int rect_flagcol[2] = { 0x8C, 0xC8 };

int far Obj_MemTPtr(struct Object far *obj);
unsigned char far IsMobElem(struct Object far *obj);
int far Clk(int n);
void far txtwal(int a, unsigned char shade, unsigned char b, int tex);
void far txtflr(int a, char shade, char tex);
void far Ref(char n, int a);

void far do_rect(unsigned char model, struct Object far *o, char heading, int tex);
void far do_door(unsigned char item, struct Object far *o);

void far do_obj(struct Object far *o)
{
    int frame;
    int type;
    int item;
    int model;
    int dir;
    int crit;
    int pix;
    int is15;
    int x;
    int y;
    char tm;
    register int fx;
    register int fy;

    if (o == ThePlayer)
        return;
    if (((o->id & 0x4000) >> 14) == 1)
        return;
    if (PickUp) {
        TileIndices[PickUp] = (int)(tmptr - mlowptr) + mptrmod;
        ObjectIndices[PickUp] = Obj_MemTPtr(o);
        *dbptr++ = 0xAE;
        *dbptr++ = PickUp;
        if (++PickUp >= 0xAC)
            PickUp = 1;
    }
    if (IsMobElem(o) && ((o->id & 0x1C0) >> 6) != 1) {
        fx = o->w0B & 0xFF;
        fy = o->w0D & 0xFF;
        switch (quad) {
        case 0:
            y = fy;
            x = fx;
            break;
        case 1:
            y = fx;
            x = 0xFF - fy;
            break;
        case 2:
            x = 0xFF - fx;
            y = 0xFF - fy;
            break;
        case 3:
            y = 0xFF - fx;
            x = fy;
            break;
        }
        objxloc = (objxloc & 0xFF00) + x;
        objzloc = (objzloc & 0xFF00) + y;
    }
    type = ComObjData[o->id & 0x1FF].b9_0;
    if (((o->id & 0x1C0) >> 6) == 7) {
        AnimObjInPipe = 1;
        item = o->owner;
        if (type == 0) {
            if (item > 0)
                item += 0x1C0;
            else
                item = o->id & 0x1FF;
        }
    }
    else
        item = o->id & 0x1FF;
    switch (type) {
    case 2:
        item &= 0x3F;
        if (item >> 4) {
            item -= 0x10;
            if ((model = rect_sub[item]) < 0)
                return;
            if (item >= 0x20)
                return;
            do_rect(model, o, -1, -1);
            return;
        }
        do_door(item, o);
        return;
    case 1:
        *dbptr++ = 0x7A;
        *dbptr++ = objxloc;
        *dbptr++ = objzloc;
        *dbptr++ = objyloc;
        *dbptr++ = 0x7F8;
        frame = o->b15 & 0x3F;
        dir = dirtab[((((o->pos & 0x380) >> 7) << 2) + 0x20
                      - ((cPlayer->heading + headmod[quad]) >> 11)) % 0x20];
        crit = grs_3dinf[item & 0x3F].page;
        frame = ((((crit << 3) + (o->b15 & 0x3F) << 3) + dir) << 3) + ((o->w0B & 0xF000) >> 12);
        pix = *(unsigned char far *)MK_FP(EmsBuff + 0xC00, frame);
        is15 = ((o->w0B & 0xF) >> 0) == 0xF;
        if (pix == 0xFF)
            return;
        if (frame == 0xFF)
            return;
        *dbptr++ = 0x5A;
        *dbptr++ = item & 0x3F;
        *dbptr++ = is15;
        *dbptr++ = locsqmod * lighton;
        *dbptr++ = pix;
        *dbptr++ = 0x7F8;
        break;
    case 0:
        if ((item & 0x1E0) == 0xE0 && (item & 0x18))
            item = 0xE0;
        *dbptr++ = 0x7A;
        *dbptr++ = objxloc;
        *dbptr++ = objzloc;
        *dbptr++ = objyloc;
        *dbptr++ = 0x7F8;
        *dbptr++ = 0x3A;
        *dbptr++ = item;
        *dbptr++ = locsqmod * lighton;
        *dbptr++ = 0x7F8;
        break;
    case 3:
        if (((o->id & 0x30) >> 4) == 3) {
            if ((tmapson | PickUp) == 0) {
                *dbptr++ = 2;
                *dbptr++ = Clk(8);
                *dbptr++ = 0;
            }
            do_rect(0x14, o, -1, first_tmobj + (item & 0xF));
            if (tmapson | PickUp)
                return;
            *dbptr++ = 2;
            *dbptr++ = Clk(8);
            *dbptr++ = 1;
        }
        else {
            tCacheOK = 0xE0;
            txtwal(0, sqmod, 4, o->owner);
            *dbptr++ = 0xB2;
            *dbptr++ = cTmBm;
            tm = TxmTerr[o->owner] & 7;
            if (tm == 3 || tm == 4) {
                curautocode = 3;
                tm = 1;
            }
            else
                tm = 0;
            tm = tm && (tmapson | PickUp) == 0;
            if (tm) {
                *dbptr++ = 2;
                *dbptr++ = Clk(8);
                *dbptr++ = 0;
            }
            do_rect(0x16, o, -1, o->owner);
            if (tm) {
                *dbptr++ = 2;
                *dbptr++ = Clk(8);
                *dbptr++ = 1;
            }
        }
        break;
    }
}

void far do_rect(unsigned char model, struct Object far *o, char heading, int tex)
{
    int head;
    int flags;
    int anim;
    int hilite;
    register int i;
    register int j;

    hilite = -1;
    flags = rect_cols[model][0];
    *dbptr++ = 2;
    *dbptr++ = Clk(0xA);
    *dbptr++ = locsqmod * lighton;
    if (flags & 0x20) {
        unsigned char col;

        col = TxmCol[tex];
        *dbptr++ = 2;
        *dbptr++ = Clk(0);
        *dbptr++ = col;
        *dbptr++ = 2;
        *dbptr++ = Clk(0xA);
        *dbptr++ = sqmod * lighton;
    }
    else if (flags & 0x80) {
        AnimObjInPipe = 1;
        anim = (*Time & 0x1FF) >> 6;
        if (anim & 4)
            anim = 3 - (anim & 3);
        for (i = 0; i < (flags & 7); i++) {
            *dbptr++ = 2;
            *dbptr++ = Clk(i);
            *dbptr++ = (o->link & 0x1FF) + anim + rect_cols[model][i + 1];
        }
    }
    else
        for (i = 0; i < (flags & 7); i++) {
            *dbptr++ = 2;
            *dbptr++ = Clk(i);
            *dbptr++ = rect_cols[model][i + 1];
        }
    if (model == 0x1D) {
        *dbptr++ = 2;
        *dbptr++ = Clk(2);
        *dbptr++ = (o->owner << 2) + 5;
        *dbptr++ = 2;
        *dbptr++ = Clk(3);
        *dbptr++ = o->owner << 2;
    }
    if (flags & 0x10) {
        unsigned char ntex;
        unsigned char first;

        ntex = rect_cols[model][4];
        first = rect_cols[model][3];
        if (tex < 0) {
            if (model == 2) {
                if (((o->id & 0x1E00) >> 9) >= ntex) {
                    tex = ((o->id & 0x1E00) >> 9) - ntex;
                    txtflr(0, sqmod, tex);
                    *dbptr++ = 2;
                    *dbptr++ = Clk(0xB);
                    *dbptr++ = TxmCol[tex];
                    *dbptr++ = 2;
                    *dbptr++ = Clk(0);
                    *dbptr++ = TxmCol[tex];
                    *dbptr++ = 0xB2;
                    *dbptr++ = cTmBm;
                    tex = -1;
                }
                else {
                    curautocode = 2;
                    /* the cast keeps the compiler from moving the 0x10 to the end */
                    tex = (int)(first_tmobj + first + 0x10) + ((o->id & 0x1E00) >> 9) % ntex;
                    *dbptr++ = 2;
                    *dbptr++ = Clk(0xB);
                    *dbptr++ = rect_flagcol[(o->id & 0x1E00) >> 9];
                    *dbptr++ = 2;
                    *dbptr++ = Clk(0);
                    *dbptr++ = rect_flagcol[(o->id & 0x1E00) >> 9];
                    *dbptr++ = 0xB2;
                    *dbptr++ = 6;
                }
            }
            else
                tex = (int)(first_tmobj + first + 0x10) + ((o->id & 0x1E00) >> 9) % ntex;
        }
        if (tex >= 0) {
            *dbptr++ = 0xC0;
            *dbptr++ = tex;
            *dbptr++ = locsqmod * lighton;
        }
        tCacheOK = 0xE0;
    }
    if (model == 0x1E) {
        int face;
        int colour;
        int blink;

        blink = (unsigned char)((*Time >> 7) & 1);
        for (j = 0, face = 0; j <= 0x10; j++, face++) {
            if (player->gems != 0xFF) {
                colour = 0x52;
                if ((1 << face) & player->gems)
                    colour = 0x4D;
                else if ((player->w105 & 7) == face)
                    colour = 0x4F;
            }
            else
                colour = ((face + blink) & 1) * 3 + 0x4C;
            if (j == 3)
                j = 0xC;
            *dbptr++ = 2;
            *dbptr++ = Clk(j);
            *dbptr++ = colour;
        }
        if (player->gems != 0xFF) {
            j = (*Time >> 6) & 7;
            if (j > 3)
                j = 7 - j;
            colour = j + 0x53;
        }
        else
            colour = 0x50;
        *dbptr++ = 2;
        *dbptr++ = Clk(3);
        *dbptr++ = colour;
    }
    if (flags & 8) {
        i = 0x400 - objyloc;
        *dbptr++ = 0x4C;
        *dbptr++ = 0;
        *dbptr++ = 0;
        *dbptr++ = i;
        *dbptr++ = 0x800;
        *dbptr++ = 2;
        *dbptr++ = bmhgtoff + 0x30;
        *dbptr++ = i * 2 - 1;
    }
    *dbptr++ = 0x18;
    *dbptr++ = objxloc & 0xFFFF;
    *dbptr++ = objxloc >> 16;
    *dbptr++ = objyloc & 0xFFFF;
    *dbptr++ = objyloc >> 16;
    *dbptr++ = objzloc & 0xFFFF;
    *dbptr++ = objzloc >> 16;
    if (heading < 0)
        head = ((((o->pos & 0x380) >> 7) + 8 - quad * 2) % 8) << 13;
    else
        head = ((heading + 0x10 - quad * 4) & 0xF) << 12;
    if ((flags & 0x40) && !(flags & 0x10)) {
        if (o >= objdata)
            j = 0;
        else {
            j = (((o->b14 & 0xF8) >> 3) - 0x10) * 0x266;
            head = (((((o->pos & 0x380) >> 7) << 5) + (o->b18 & 0x1F) + 0x100 - quad * 64) % 0x100) << 8;
        }
        *dbptr++ = 2;
        *dbptr++ = Clk(5);
        *dbptr++ = j;
    }
    if ((model == 0x10 || model == 0x11) && (tmapson | PickUp) == 0)
        hilite = 0;
    else if (PickUp == 0 && player->detail == 1 && model == 2)
        hilite = 1;
    if (hilite != -1) {
        *dbptr++ = 2;
        *dbptr++ = Clk(8);
        *dbptr++ = hilite;
    }
    if (head) {
        *dbptr++ = 0x50;
        *dbptr++ = head;
    }
    else
        *dbptr++ = 0x12;
    Ref(model + 0x60, 1);
    if (hilite != -1) {
        *dbptr++ = 2;
        *dbptr++ = Clk(8);
        *dbptr++ = (hilite + 1) & 1;
    }
    *dbptr++ = 0x18;
    *dbptr++ = 0;
    *dbptr++ = 0;
    *dbptr++ = 0;
    *dbptr++ = 0;
    *dbptr++ = 0;
    *dbptr++ = 0;
}

void far do_door(unsigned char item, struct Object far *o)
{
    int h;
    int dir;
    int oldz;
    char first;
    char step;
    char pass;
    char vis;
    register int z;

    z = -1;
    first = 0;
    step = 1;
    vis = 0;
    curautocode = 1;
    if ((item & 7) == 6) {
        dir = 0;
        z = objyloc;
        oldz = objyloc - (((o->id & 0x1E00) >> 9) & 7) * 0x30;
        vis = 1;
        h = 0x400 - oldz - 0xD0;
        first = 1;
        step = -1;
        *dbptr++ = 2;
        *dbptr++ = Clk(5);
        *dbptr++ = ((o->id & 0x1E00) >> 9) & 7;
        *dbptr++ = 0x4C;
        *dbptr++ = 0;
        *dbptr++ = 0;
        *dbptr++ = 0xD0 - (((o->id & 0x1E00) >> 9) & 7) * 0x30;
        *dbptr++ = 0x400;
    }
    else {
        if ((((o->id & 0x1E00) >> 9) & 7) || ((o->id & 0x1C0) >> 6) == 7)
            objyloc -= 0xC0;
        dir = (((o->id & 0x1E00) >> 9) & 7) * (((o->id & 0x2000) >> 13) * 2 - 1);
        h = 0x400 - objyloc - 0xD0;
        *dbptr++ = 2;
        *dbptr++ = Clk(5);
        *dbptr++ = dir << 12;
    }
    /* a portcullis gets this after its own 0x4C block: the 0x400 store jumps into
       the shared tail here */
    *dbptr++ = 0x4C;
    *dbptr++ = 0;
    *dbptr++ = 0;
    *dbptr++ = h;
    *dbptr++ = 0x800;
    if (vis == 0) {
        vis = (((o->pos & 0x380) >> 7) - ((ThePlayer->pos & 0x380) >> 7) + 8) & 7;
        if (vis > 4)
            vis -= 8;
        vis = vis >= -1 && vis <= 2;
    }
    if ((((o->id & 0x1E00) >> 9) & 7) && z == -1) {
        int q;
        int q3;
        int ddx;
        register int k;
        register int dy;

        q = (((o->pos & 0x380) >> 7) - quad * 2) & 7;
        q3 = q & 3;
        ddx = objxloc - cPlayer->x;
        dy = objzloc - cPlayer->y;
        switch (q3) {
        case 0:
            k = dy < 0;
            break;
        case 1:
            k = -dy > ddx;
            break;
        case 2:
            k = ddx < 0;
            break;
        case 3:
            k = ddx < dy;
            break;
        }
        k = q / 4 + ((o->id & 0x2000) >> 13) + k;
        if (!(k & 1)) {
            first = 1;
            step = -1;
        }
    }
    *dbptr++ = 2;
    *dbptr++ = Clk(3);
    *dbptr++ = (((o->id & 0x1E00) >> 9) & 7) + (z >= 0);
    for (pass = first; pass <= 1 && pass >= 0; pass += step) {
        if (pass == 0) {
            if (tCacheOK < 1)
                txtwal(0, locsqmod, objyloc >> 6, tmptr->wall);
            *dbptr++ = 2;
            *dbptr++ = bmhgtoff + (cTmBm << 3);
            *dbptr++ = (cTmDm * cTmDm >> 8) * h - 1;
            *dbptr++ = 2;
            *dbptr++ = Clk(7);
            *dbptr++ = cTmDm * cTmDm - 1;
            *dbptr++ = 2;
            *dbptr++ = Clk(6);
            *dbptr++ = bmhgtoff + (cTmBm << 3);
            if (PickUp) {
                *dbptr++ = 0xAE;
                *dbptr++ = tmptr->wall + 0xAC;
            }
            *dbptr++ = 0xB2;
            *dbptr++ = cTmBm;
            do_rect(1, o, ((o->pos & 0x380) >> 7) << 1, tmptr->wall);
        }
        else {
            if (PickUp) {
                *dbptr++ = 0xAE;
                *dbptr++ = PickUp - 1;
            }
            if (z >= 0) {
                objyloc = z;
                do_rect(0xC, o, ((o->pos & 0x380) >> 7) << 1, 0);
                objyloc = oldz;
            }
            else {
                register int k;

                k = item & 7;
                if (k == 7) {
                    if (tCacheOK < 1)
                        txtwal(0, locsqmod, objyloc >> 6, tmptr->wall);
                    *dbptr++ = 0xB2;
                    *dbptr++ = cTmBm;
                    *dbptr++ = 2;
                    *dbptr++ = bmhgtoff + (cTmBm << 3);
                    *dbptr++ = cTmDm * cTmDm - 1;
                    do_rect(0xF, o, ((o->pos & 0x380) >> 7) << 1, tmptr->wall);
                }
                else
                    do_rect(0xE, o, ((o->pos & 0x380) >> 7) << 1, first_tmobj + k + 0x40);
            }
        }
    }
}
