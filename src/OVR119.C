/* target: ovr119 */
/* opts: -mm -1 -G -O -Y -d */
/* Art loading: .GR picture files into EMS pages or video memory, and .TR textures.
   Function names are the FM Towns originals at the same positions, using the same
   globals in the same way (load_tr_ems is also the name OVR140 calls, stub +4D).
   LoadArtFile_ovr119_804 and ovr119_949 have no FM Towns counterpart: the FM Towns file
   has load_tr_ems_ alone after reload_obj_ems_, and nothing between load_gr_video_ and
   reload_gr_vpic_. Both keep their IDA names. */
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <alloc.h>

/* Graphics slot bookkeeping. These words start the file's _DATA, straight after
   ovr118's "pals.dat"; FM Towns keeps the first five as public globals. */
unsigned first_obj = 0;             /* DS:14FE */
unsigned first_button = 0;          /* DS:1500 */
unsigned first_tmobj = 0;           /* DS:1502 */
unsigned first_vram = 0;            /* DS:1504 */
void far *load_adr = 0;             /* DS:1506, not used by the DOS code */
/* FM Towns keeps the rest as statics after _grfx_driver (+0x38 onwards), so no names. */
static unsigned mapped_page = 0;    /* EMS page now mapped at physical page 2 */
static unsigned gr_index = 0;       /* next graphics slot in grs_off */
static unsigned ems_off = 0;        /* next free paragraph in the EMS page */
static unsigned char ems_page = 4;  /* EMS logical page being filled */
static char gr_ext[6][4] = { "", ".gr", ".tr", ".cr", ".sr", ".ar" };
static int reload_base = -1;        /* object slot that reload_obj_ems starts at */

extern char gsize;                  /* texture edge, from the .tr header */
extern void far *constadr;          /* where read_gr_far puts its picture */
extern unsigned char npals;
extern unsigned long far *tmpoffs;  /* the open file's offset table */
extern unsigned tmpcnt;             /* pictures in the open file */
extern unsigned char Palettes[][16];
extern FILE *grfp;
extern unsigned *PalStore;
extern unsigned far *grs_off;       /* EMS page and paragraph, or video address, per slot */
extern unsigned far *obj_tab;       /* two words per object; only the first is set here */
extern unsigned char TxmCol[];
extern int TxmID[];
extern unsigned char ActDoors[6];
extern unsigned far first_anim;
extern unsigned far ems_seg;
extern unsigned char far TextureLogicalPage;
extern char far stdat;
/* the 3D engine's buffers: gr_offs holds 570 offsets (FM Towns _gr_end follows it) */
extern unsigned long far gr_offs[];
extern unsigned char far cmpbuf1_start[];
#define GR_OFFS_MAX 570

int far ReadFileToAddress(int fd, void far *buf, unsigned n);
void far MapMemory_seg013_1D3C_C7();
int far valloc(int w, int h);
void far DRAW_RELATED_seg017_2179_2A2();
void far DRAW_RELATED_seg017_2179_361();
void far seg042_35ED_12B(void);
void far mem_set(void far *destination, int value, unsigned count);
unsigned char far *far grs_scaledown();
unsigned char far preload_cr();

unsigned char far get_pals(void)
{
    if (fread(&npals, 1, 1, grfp) != 1)
        return 0;
    if (*PalStore > 0) {
        *PalStore = (unsigned)malloc((unsigned)npals << 5);
        if (fread((void *)*PalStore, 1, (unsigned)npals << 5, grfp) !=
              ((unsigned)npals << 5))
            return 0;
    } else {
        fseek(grfp, (long)((int)npals << 5), 1);
    }
    return 1;
}

unsigned char far _ld_open(char *art, char type)
{
    char path[66];
    char found;
    strcpy(path, "DATA\\");
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
    if ((unsigned)((tmpcnt + 1) << 2) >
        (unsigned)((char far *)&gr_offs[GR_OFFS_MAX] - (char far *)gr_offs))
        goto bad;
    tmpoffs = gr_offs;
    fseek(grfp, 0L, 1);
    if (ReadFileToAddress(grfp->fd, tmpoffs, (tmpcnt + 1) << 2) !=
          ((tmpcnt + 1) << 2)) {
        goto bad;
    }
    fseek(grfp, 0L, 1);
    return 1;
bad:
    fclose(grfp);
nofile:
    return 0;
}

void far _ld_close(void)
{
    fclose(grfp);
}

/* Read one picture into dst; returns its size, or -1. */
int far _ld_gr(int image, void far *dst)
{
    long end;
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
    if (ReadFileToAddress(grfp->fd, dst, size) != size)
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
        MapMemory_seg013_1D3C_C7(2, ems_page);
        mapped_page = ems_page;
    }
    return MK_FP(ems_seg + ems_off + 0x800, 0);
}

void far *far adrold_ems(void)
{
    unsigned address = grs_off[gr_index];
    MapMemory_seg013_1D3C_C7(2, address >> 12);
    ems_off = address & 0xFFF;
    return MK_FP(ems_seg + ems_off + 0x800, 0);
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
    return &stdat;
}

void far *far adr_const(void)
{
    return constadr;
}

unsigned char far movenew_vram(unsigned char far *image, int unused, int index)
{
    int address = valloc(image[1], (unsigned)image[2] + 1);
    if (!address)
        return 0;
    if (image[0] == 4)
        DRAW_RELATED_seg017_2179_2A2(image + 5, address, image[1], image[2]);
    else
        DRAW_RELATED_seg017_2179_361(image, address, unused);
    grs_off[index + gr_index] = address;
    return 1;
}

unsigned char far move_vram(unsigned char far *image, int unused, int index)
{
    int address = grs_off[index + gr_index];
    if (image[0] == 4)
        DRAW_RELATED_seg017_2179_2A2(image + 5, address, image[1], image[2]);
    else
        DRAW_RELATED_seg017_2179_361(image, address, unused);
    return 1;
}

/* Load count pictures from start (all of them if count < 0): adr says where each goes,
   move records it. */
unsigned char far gronk_gr(char *art, int start, int count,
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

unsigned char far load_gr_ems(char *art)
{
    unsigned char ok = gronk_gr(art, 0, -1, (void far *(far *)(int))adrnew_ems,
                                 movenew_ems);
    gr_index += tmpcnt;
    return ok;
}

unsigned char far load_obj_ems(char *art)
{
    unsigned char ok = gronk_gr(art, 0, -1, (void far *(far *)(int))adrnew_ems,
                                 moveobj_ems);
    return ok;
}

unsigned char far reload_obj_ems(char *art, int base, int count)
{
    reload_base = base;
    return gronk_gr(art, 0, count, (void far *(far *)(int))adrnew_ems,
                    move_reload_obj_ems);
}

/* The level's 64 textures into EMS, noting each one's first pixel in TxmCol. */
unsigned char far load_tr_ems(char *art)
{
    register int i, bytes;
    if (!_ld_open(art, 2))
        return 0;
    gsize = 0x40;
    ems_off = 0;
    bytes = gsize * gsize;
    ems_page = TextureLogicalPage;
    MapMemory_seg013_1D3C_C7(3, ems_page);
    for (i = 0; i < 0x40; i++) {
        if (fseek(grfp, tmpoffs[TxmID[i]], 0))
            break;
        if ((ems_off << 4) + bytes > 0x4000) {
            ems_off = 0;
            ems_page++;
            MapMemory_seg013_1D3C_C7(3, ems_page);
        }
        if (tmpoffs[TxmID[i]] == tmpoffs[TxmID[i] + 1])
            mem_set(MK_FP(ems_seg + ems_off + 0xC00, 0), 0, bytes);
        else if (ReadFileToAddress(grfp->fd, MK_FP(ems_seg + ems_off + 0xC00, 0),
                                   bytes) != bytes)
            break;
        TxmCol[i] = *(unsigned char far *)MK_FP(ems_seg + 0xC00, ems_off << 4);
        ems_off += bytes >> 4;
    }
    _ld_close();
    return i == 0x40;
}

/* DOS only: all 256 textures, each reduced by grs_scaledown to 256 bytes, into destination. */
int far LoadArtFile_ovr119_804(char *art, void far *destination, int unused)
{
    void far *dst;
    void far *source;
    register int i = 0, bytes;
    if (!_ld_open(art, 2))
        return 0;
    gsize = 0x40;
    dst = destination;
    bytes = gsize * gsize;
    do {
        if (fseek(grfp, tmpoffs[i], 0))
            break;
        if (tmpoffs[i] == tmpoffs[i+1])
            mem_set(cmpbuf1_start, 0, bytes);
        else if (ReadFileToAddress(grfp->fd, cmpbuf1_start, bytes) != bytes)
            break;
        source = grs_scaledown(cmpbuf1_start, gsize, gsize, 4);
        movedata(FP_SEG(source), FP_OFF(source), FP_SEG(dst), FP_OFF(dst), 0x100);
        *((unsigned *)&dst) += 0x100;
        i++;
    } while (i < 0x100);
    _ld_close();
    return i;
}

unsigned char far load_gr_video(char *art)
{
    unsigned char ok = gronk_gr(art, 0, -1, (void far *(far *)(int))adrnew_vram,
                                 movenew_vram);
    gr_index += tmpcnt;
    return ok;
}

unsigned char far ovr119_949(int offset, char *art, int start, int count)
{
    register int old = gr_index;
    unsigned char ok;
    gr_index = (offset - 0x2000) + first_vram;
    ok = gronk_gr(art, start, count, (void far *(far *)(int))adrnew_vram,
                   move_vram);
    gr_index = old;
    return ok;
}

void far reload_gr_vpic(int offset, char *art, int image)
{
    register int old = gr_index;
    gr_index = (offset - 0x2000) + first_vram;
    gronk_gr(art, image, 1, (void far *(far *)(int))adrnew_vram,
                   move_vram);
    gr_index = old;
}

unsigned char far read_gr_far(char *art, int image, void far *dst)
{
    constadr = dst;
    return gronk_gr(art, image, 1, (void far *(far *)(int))adr_const, 0L);
}

int far load_all_gr(void)
{
    unsigned char ok = 1;
    register FILE *fp;
    if ((fp = fopen("data/allpals.dat", "rb")) == 0)
        return 0x3008;
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
    ok &= load_gr_ems("gempt");
    ok &= load_gr_ems("3dwin");
    first_tmobj = gr_index;
    ok &= reload_obj_ems("tmflat", 0x170, 0x10);
    ok &= load_gr_ems("tmobj");
    first_vram = gr_index;
    ok &= load_gr_video("lfti");
    ok &= load_gr_video("flasks");
    ok &= load_gr_video("compass");
    ok &= load_gr_video("inv");
    ok &= load_gr_video("power");
    ok &= load_gr_video("eyes");
    ok &= load_gr_video("chains");
    ok &= load_gr_video("spells");
    ok &= load_gr_video("scrledge");
    ok &= load_gr_video("optb");
    if (!ok)
        return 0x3004;
    ems_page++;
    if (preload_cr(1))
        return 0;
    return 0x3009;
}

/* The level's six door textures into the slots after the 64 textures. */
void far load_doors(void)
{
    unsigned oldIndex = gr_index;
    register int i;
    register int oldOffset = ems_off;
    gr_index = first_tmobj + 0x40;
    for (i = 0; i < 6; i++, gr_index++) {
        gronk_gr("doors", ActDoors[i], 1, (void far *(far *)(int))adrold_ems, 0L);
    }
    ems_off = oldOffset;
    gr_index = oldIndex;
    seg042_35ED_12B();
}
