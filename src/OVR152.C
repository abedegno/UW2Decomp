/* target: ovr152 */
/* opts: -mm -1 -G -O -Y -d */

#include <dos.h>

/* The light level set_light last loaded, 5 for mono.dat; FM Towns _cur_light_level, which
   its set_light_ tests the same way. DS:1AAC, the first byte of this file's _DATA (which
   holds DS:1AAD and is word-aligned). */
unsigned char cur_light_level = 0xFF;
extern unsigned char far *cLightTabs;

extern unsigned curvrad;
extern unsigned distpoly;
extern unsigned dist8;
extern unsigned char far cXfer[];
extern unsigned char far ModelData_seg052_519C_2600;
extern unsigned far smooth_div;
extern unsigned far smooth_base;
extern unsigned far smooth_lowpass;

int far our_open();
int far ReadFileToAddress(int fd, void far *buf, unsigned n);
void far close(int fd);
long far lseek(int fd, long offset, int origin);
int far read(int fd, void *buf, unsigned n);
void far preset_grid(int shade);
void far editchng(int bit);
void far movedata(unsigned srcseg, unsigned srcoff,
                                         unsigned dstseg, unsigned dstoff,
                                         unsigned n);

/* FM Towns lget_: target table currently calls this lget. */
int far lget(char *name, unsigned off, unsigned seg, unsigned n)
{
    register int got = -1;
    int handle;

    if ((handle = our_open(name, 1, 0)) != -1) {
        got = ReadFileToAddress(handle, MK_FP(seg, off), n);
        close(handle);
    }
    if (got == n)
        return 1;
    return 0;
}

/* FM Towns set_light_: same shades.dat lookup and smooth parameters. */
void far set_light(signed char lightLevel)
{
    int ShadesDataRow_var_C[6];
    int handle;
    int diValue;

    if (cur_light_level == lightLevel)
        return;
    if (cur_light_level == 5) {
        lget("light.dat", FP_OFF(cLightTabs), FP_SEG(cLightTabs), 0x1000);
    } else if (lightLevel == 5) {
        lget("mono.dat", FP_OFF(cLightTabs), FP_SEG(cLightTabs), 0x1000);
    }
    cur_light_level = lightLevel;
    if ((handle = our_open("shades.dat", 1, 0)) < 0)
        return;
    diValue = (int)lightLevel * 12;
    lseek(handle, diValue, 0);
    read(handle, ShadesDataRow_var_C, 12);
    smooth_div = ShadesDataRow_var_C[0];
    if ((int)smooth_div <= 1)
        smooth_div = 1;
    smooth_base = ShadesDataRow_var_C[1];
    smooth_lowpass = ShadesDataRow_var_C[2];
    curvrad = ShadesDataRow_var_C[3];
    distpoly = ShadesDataRow_var_C[4];
    dist8 = ShadesDataRow_var_C[5];
    close(handle);
    preset_grid(curvrad);
    editchng(2);
}

/* FM Towns init_lighting_: loads light.dat and xfer.dat through lget_. */
void far init_lighting(void)
{
    lget("light.dat", FP_OFF(cLightTabs), FP_SEG(cLightTabs), 0x1000);
    lget("xfer.dat", (unsigned)cXfer,
                         (unsigned)FP_SEG(cXfer), 0x500);
}

/* FM Towns random_light_: replaces light data and clears two bytes per row. */
void far random_light(char enabled)
{
    int i;

    if (enabled != 0) {
        movedata(ModelData_seg052_519C_2600 * 0,
                                        ModelData_seg052_519C_2600,
                                        FP_SEG(cLightTabs),
                                        FP_OFF(cLightTabs),
                                        0x1000);
        for (i = 0; i < 16; i++) {
            cLightTabs[i * 256] = 0;
            ((char far *)MK_FP(FP_SEG(cLightTabs), FP_OFF(cLightTabs)))[i * 256 + 1] = 0;
        }
    } else {
        lget(cur_light_level != 5 ?
                             "light.dat" : "mono.dat",
                             FP_OFF(cLightTabs), FP_SEG(cLightTabs), 0x1000);
    }
}
