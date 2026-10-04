/* target: ovr115 */
/* opts: -mm -1 -G -O -Y -d */
/* Art loading: .GR picture files into EMS pages or video memory, and .TR textures: the
   whole of UW1's DOS overlay ovr115, in original order. UW1 has no symbol-bearing build;
   the names are UW2's (the FM Towns symbol table; UW2's LOADGR.C, overlay ovr119), the
   code being the same routines.

   load_all_gr runs once at start-up. It reads the 32 auxiliary 16-colour palettes of
   DATA\ALLPALS.DAT into Palettes, then loads every .GR file the game keeps resident and
   assigns each picture a slot of grs_off, in this order: question, views, objects (slots
   from first_obj, through obj_tab), animo (first_anim), buttons, cursors and 3dwin (from
   first_button), tmflat and tmobj (from first_tmobj), and, in video memory, lfti, flasks,
   compass, dragons, inv, power, eyes, chains, spells, scrledge and optb (from first_vram).
   grs_which1 (GRSPIC.C) maps the game's icon numbers onto those ranges. EMS pictures are
   packed a paragraph-aligned picture after another into 16 KB logical pages, mapped one
   at a time into physical page 2; a slot records the page in its top four bits and the
   paragraph in the low twelve. Video memory pictures get a block from valloc and are
   copied in by the graphics library; their slot holds the video offset.

   Every load goes through gronk_gr, which opens a file with _ld_open and reads each
   picture with _ld_gr to the address an adr callback gives, then lets a move callback
   record it. Per level, load_tr_ems loads textures into EMS, and load_doors the six door
   pictures.

   UW1's differences, marked "UW1:" where they are not obvious: _ld_open reads a file's
   offset table into a block from malloc (UW2: the 3D library's gr_offs) and _ld_close
   frees it; a file that cannot be opened or parsed is reported through DEBUG.C's
   dprintf; load_tr_ems loads textures 12 to 47 of TxmID (the .tr file's own size, 64
   or 32, choosing the first EMS page from one of two seg051 bytes) and keeps no TxmCol;
   there is no LoadScaled; load_all_gr loads "dragons" and not "gempt", and sets the
   second texture page nine pages after the first; _ld_open, gronk_gr, load_tr_ems and
   the loaders load_all_gr calls return a signed char (callers cbw; load_all_gr's
   `ok &= ...` on a char compiles to `and [bp-1],al` only with char results).

   name: descriptive. Function names are the FM Towns originals at the same positions
   (UW2). GrLoadAt_ovr119_949 keeps UW2's provisional name. free_timers in the target
   table (kin with a UW2 sound routine of the same bytes' shape) is _ld_close here: it is
   what UW2's _ld_close is, at the same position, and gronk_gr and load_tr_ems call it
   after their loads. */
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <alloc.h>
#include "critter.h"
#include "file.h"
#include "gfx.h"
#include "map.h"
#include "sys.h"
#include "view3d.h"

/* UW1: the callees under their UW1 listing names (UW2: seg012_10F,
   seg015_1F9B_2A7 and _361, seg042_35ED_12B, preload_cr), and DEBUG.C's
   printer. preload_cr is critter/CRPAGES.C's, declared in critter.h (UW2's name, which the
   stub order confirms). */
/* match: load_tr_ems first, as gfx.h has it: it and load_gr_ems have the same public-order
   key (404), and the first seen is listed last. */
/* UW1: the texture pages, the animation slot base and EmsBuff are in seg051 (the 3D
   renderer's far data, assembly); the listing has no names for the two page bytes. */

/* Graphics slot bookkeeping: where each range of grs_off starts.
   match: these words start the file's _DATA (DS:15DE..16C6), straight after GRFX.C's
   "data/pals.dat"; FM Towns (UW2) keeps the first five as public globals. */
uint16 first_obj = 0;               /* DS:15DE */
uint16 first_button = 0;            /* DS:15E0 */
uint16 first_tmobj = 0;             /* DS:15E2 */
uint16 first_vram = 0;              /* DS:15E4 */
void far *load_adr = 0;             /* DS:15E6, not used by the DOS code */
/* name: FM Towns (UW2) keeps the rest as statics, so no names. */
static uint16 mapped_page = 0;      /* EMS page now mapped at physical page 2 */
static uint16 gr_index = 0;         /* next graphics slot in grs_off */
static uint16 ems_off = 0;          /* next free paragraph in the EMS page */
static unsigned char ems_page = 4;  /* EMS logical page being filled */
static char gr_ext[6][4] = { "", ".gr", ".tr", ".cr", ".sr", ".ar" };
static int16 reload_base = -1;      /* object slot that reload_obj_ems starts at */

/* match: this file's _BSS, DS:5732..5941, laid out by name (tools/bssorder.py): gsize 119,
   constadr 131, npals 270, tmpoffs and tmpcnt 612, Palettes 632, grfp 663, PalStore 736. */
char gsize;                         /* texture edge, from the .tr header */
void far *constadr;                 /* where read_gr_far puts its picture */
unsigned char npals;
uint32 *tmpoffs;                    /* the open file's offset table (UW1: near, from malloc) */
uint16 tmpcnt;                      /* pictures in the open file */
unsigned char Palettes[32][16];
FILE *grfp;
UNEARPTR *PalStore;

/* Reads a .CR file's palette count and palettes (32 bytes each). They are kept in a
   malloc'd block whose near address goes to *PalStore when *PalStore is nonzero, and
   skipped otherwise. In UW2 nothing sets PalStore, so *PalStore reads DS:0; but _ld_open
   is only called for types 1 and 2, so this never runs. */
unsigned char far get_pals(void)
{
    if (fread(&npals, 1, 1, grfp) != 1)
        return 0;
    if (*NULLTRAP(PalStore) > 0) {
        *NULLTRAP(PalStore) = (UNEARPTR)malloc((unsigned)npals << 5);
        if (fread((void *)*NULLTRAP(PalStore), 1, (unsigned)npals << 5, grfp) !=
              ((unsigned)npals << 5))
            return 0;
    } else {
        fseek(grfp, (int32)((int)npals << 5), 1);
    }
    return 1;
}

/* Opens DATA\<art><ext> for reading, checks its type byte and reads its header and offset
   table (see the file comment). Returns 1 with grfp open, else 0 with it closed. */
char far _ld_open(char *art, char type)
{
    char path[66];
    char found;
    strcpy(path, "data\\");
    strcat(path, art);
    strcat(path, gr_ext[type]);
    if ((grfp = fopen(path, "rb")) == 0)
        goto nofile;
    if (fread(&found, 1, 1, grfp) != 1)
        goto bad;
    if (found != type)
        goto bad;
    if (type == 2 && fread(&gsize, 1, 1, grfp) != 1)
        goto bad;
    if (fread(&tmpcnt, 2, 1, grfp) != 1)
        goto bad;
    if (type == 3 && get_pals() == 0)
        goto bad;
    if ((tmpoffs = malloc((tmpcnt + 1) << 2)) == 0)
        goto bad;
    if (fread(tmpoffs, 4, tmpcnt + 1, grfp) == tmpcnt + 1)
        return 1;
    free(tmpoffs);
bad:
    fclose(grfp);
nofile:
    dprintf("gr no parse data %s\n", path);
    return 0;
}

void far _ld_close(void)
{
    fclose(grfp);
    free(tmpoffs);      /* UW1 */
}

/* Read one picture into dst; returns its size, or -1. */
int far _ld_gr(int image, void far *dst)
{
    int32 end;
    int size;
    if (image != tmpcnt - 1) {
        size = (unsigned)tmpoffs[image + 1] - (unsigned)tmpoffs[image];
    } else {
        fseek(grfp, 0L, 2);
        end = ftell(grfp);
        size = (unsigned)end - (unsigned)tmpoffs[image];
    }
    if (fseek(grfp, tmpoffs[image], 0))
        return -1;
    if (!size)
        return 0;
    if (intoFarBuffer_ovr167_5DA(fileno(grfp), dst, size) != size)
        return -1;
    return size;
}

/* gronk_gr callbacks for pictures kept in EMS: where the next one goes, and recording it. */
void far *far adrnew_ems(unsigned size)
{
    if ((ems_off << 4) + size > 0x4000) {
        ems_off = 0;
        ems_page++;
    }
    if (ems_page != mapped_page) {
        seg012_10F(2, ems_page);
        mapped_page = ems_page;
    }
    return MK_FP(EmsBuff + ems_off + 0x800, 0);
}

void far *far adrold_ems(void)
{
    unsigned address = grs_off[gr_index];
    seg012_10F(2, address >> 12);
    ems_off = address & 0xFFF;
    return MK_FP(EmsBuff + ems_off + 0x800, 0);
}

unsigned char far movenew_ems(void far *image, int size, int index)
{
    grs_off[index + gr_index] = ((unsigned)ems_page << 12) + ems_off;
    ems_off += size >> 4;
    if (size % 16)
        ems_off++;
    return 1;
}

unsigned char far moveobj_ems(void far *image, int size, int index)
{
    if (size == 0) {
        obj_tab[index * 2] = 0;
    } else {
        obj_tab[index * 2] = gr_index;
        grs_off[gr_index] = ((unsigned)ems_page << 12) + ems_off;
        gr_index++;
        ems_off += size >> 4;
        if (size % 16)
            ems_off++;
    }
    return 1;
}

unsigned char far move_reload_obj_ems(void far *image, int size, int index)
{
    if (size == 0) {
        obj_tab[(reload_base + index) * 2] = 0;
    } else {
        obj_tab[(reload_base + index) * 2] = gr_index;
        grs_off[gr_index] = ((unsigned)ems_page << 12) + ems_off;
        gr_index++;
        ems_off += size >> 4;
        if (size % 16)
            ems_off++;
    }
    return 1;
}

/* gronk_gr callbacks for pictures kept in video memory, read through stdat. */
void far *far adrnew_vram(void)
{
    return stdat;
}

void far *far adr_const(void)
{
    return constadr;
}

/* Copies a picture read into stdat to a new video memory block: an 8-bit bitmap (type 4)
   is stored as w, h and its pixels by MODEX's 2A2; any other is copied as it is (361).
   The slot holds the video offset. */
unsigned char far movenew_vram(unsigned char far *image, int unused, int index)
{
    int address = valloc(image[1], (unsigned)image[2] + 1);
    if (!address)
        return 0;
    if (image[0] == 4)
        seg015_1F9B_2A7(image + 5, address, image[1], image[2]);
    else
        seg015_1F9B_366(image, address, unused);
    grs_off[index + gr_index] = address;
    return 1;
}

/* As movenew_vram, into the block the slot already has (reloading a picture). */
unsigned char far move_vram(unsigned char far *image, int unused, int index)
{
    int address = grs_off[index + gr_index];
    if (image[0] == 4)
        seg015_1F9B_2A7(image + 5, address, image[1], image[2]);
    else
        seg015_1F9B_366(image, address, unused);
    return 1;
}

/* Load count pictures from start (all of them if count < 0): adr says where each goes,
   move records it. */
char far gronk_gr(char *art, int start, int count,
                           void far *(far *adr)(int),
                           unsigned char (far *move)(void far *, int, int))
{
    int size;
    void far *image;
    unsigned char ok;
    register int i, imageNo;
    ok = 1;
    if (!_ld_open(art, 1))
        return 0;
    if (count < 0)
        count = tmpcnt - start;
    for (i = 0; i < count && ok; i++) {
        imageNo = start + i;
        if (imageNo >= tmpcnt) {
            ok = 0;
            break;
        }
        size = (unsigned)tmpoffs[imageNo + 1] - (unsigned)tmpoffs[imageNo];
        image = adr(size);
        if (!image) {
            ok = 0;
        } else if (_ld_gr(imageNo, image) != size) {
            ok = 0;
        } else if (move) {
            ok &= move(image, size, i);
        }
    }
    _ld_close();
    return ok;
}

/* Loads every picture of art into EMS at the next slots. */
char far load_gr_ems(char *art)
{
    char ok = gronk_gr(art, 0, -1, (void far *(far *)(int))adrnew_ems,
                                 movenew_ems);
    gr_index += tmpcnt;
    return ok;
}

/* As load_gr_ems for object art: obj_tab[2 * i] gets each picture's slot (0 for an empty
   picture), so empty pictures take no slot. */
char far load_obj_ems(char *art)
{
    char ok = gronk_gr(art, 0, -1, (void far *(far *)(int))adrnew_ems,
                                 moveobj_ems);
    return ok;
}

/* As load_obj_ems for count pictures, recorded from object base on (tmflat's 16 pictures
   at object 170h). */
char far reload_obj_ems(char *art, int base, int count)
{
    reload_base = base;
    return gronk_gr(art, 0, count, (void far *(far *)(int))adrnew_ems,
                    move_reload_obj_ems);
}

/* The level's 64 textures into EMS, noting each one's first pixel in TxmCol. */
char far load_tr_ems(char *art)
{
    register int i, bytes;
    if (!_ld_open(art, 2))
        return 0;
    ems_off = 0;
    bytes = gsize * gsize;
    if (gsize == 0x40)
        ems_page = seg051_C375;
    else
        ems_page = seg051_C376;
    seg012_10F(3, ems_page);
    for (i = 0x0C; i < 0x30; i++) {
        if (fseek(grfp, tmpoffs[TxmID[i]], 0))
            break;
        if ((ems_off << 4) + bytes > 0x4000) {
            ems_off = 0;
            ems_page++;
            seg012_10F(3, ems_page);
        }
        if (intoFarBuffer_ovr167_5DA(fileno(grfp), MK_FP(EmsBuff + ems_off + 0xC00, 0),
                                     bytes) != bytes)
            break;
        ems_off += bytes >> 4;
    }
    _ld_close();
    return i == 0x30;
}

char far load_gr_video(char *art)
{
    char ok = gronk_gr(art, 0, -1, (void far *(far *)(int))adrnew_vram,
                                 (ArtMoveFn)movenew_vram);
    gr_index += tmpcnt;
    return ok;
}

/* Reloads count pictures of art from start into the video memory blocks of the slots from
   icon offset (a 2000h-range icon number) on. */
unsigned char far GrLoadAt_ovr119_949(int offset, char *art, int start, int count)
{
    register int old = gr_index;
    unsigned char ok;
    gr_index = (offset - ICON_VRAM) + first_vram;
    ok = gronk_gr(art, start, count, (void far *(far *)(int))adrnew_vram,
                   (ArtMoveFn)move_vram);
    gr_index = old;
    return ok;
}

/* Reloads one picture into the video memory block of icon offset. */
void far reload_gr_vpic(int offset, char *art, int image)
{
    register int old = gr_index;
    gr_index = (offset - ICON_VRAM) + first_vram;
    gronk_gr(art, image, 1, (void far *(far *)(int))adrnew_vram,
                   (ArtMoveFn)move_vram);
    gr_index = old;
}

/* Reads one picture of art to dst, recording nothing. */
unsigned char far read_gr_far(char *art, int image, void far *dst)
{
    constadr = dst;
    return gronk_gr(art, image, 1, (void far *(far *)(int))adr_const, 0L);
}

/* The start-up load (see the file comment). Returns 0, or an ERR_READ code: 8 no
   ALLPALS.DAT, 4 a .GR file failed, 9 the critter art (preload_cr) failed. */
int far load_all_gr(void)
{
    char ok = 1;
    register FILE *fp;
    if ((fp = fopen("data/allpals.dat", "rb")) == 0)
        return ERR_READ | 0x008;
    fread(Palettes, 0x200, 1, fp);
    fclose(fp);
    ok &= load_gr_ems("question");
    ok &= load_gr_ems("views");
    first_obj = gr_index;
    ok &= load_obj_ems("objects");
    first_anim = gr_index;
    ok &= load_gr_ems("animo");
    first_button = gr_index;
    ok &= load_gr_ems("buttons");
    ok &= load_gr_ems("cursors");
    ok &= load_gr_ems("3dwin");
    first_tmobj = gr_index;
    ok &= reload_obj_ems("tmflat", 0x170, 0x10);
    ok &= load_gr_ems("tmobj");
    first_vram = gr_index;
    ok &= load_gr_video("lfti");
    ok &= load_gr_video("flasks");
    ok &= load_gr_video("compass");
    ok &= load_gr_video("dragons");
    ok &= load_gr_video("inv");
    ok &= load_gr_video("power");
    ok &= load_gr_video("eyes");
    ok &= load_gr_video("chains");
    ok &= load_gr_video("spells");
    ok &= load_gr_video("scrledge");
    ok &= load_gr_video("optb");
    if (!ok)
        return ERR_READ | 0x004;
    ems_page++;
    seg051_C376 = seg051_C375 + 9;      /* UW1 */
    if (preload_cr(1))
        return 0;
    return ERR_READ | 0x009;
}

/* The level's six door textures into the slots after the 64 textures. */
void far load_doors(void)
{
    unsigned oldIndex = gr_index;
    register int i;
    register int oldOffset = ems_off;
    gr_index = first_tmobj + 0x30;
    for (i = 0; i < 6; i++, gr_index++) {
        gronk_gr("doors", ActDoors[i], 1, (void far *(far *)(int))adrold_ems, 0L);
    }
    ems_off = oldOffset;
    gr_index = oldIndex;
    seg042_19B();
}
