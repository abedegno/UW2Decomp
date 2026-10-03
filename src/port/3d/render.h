/* render.h: the port's interface to the 3D renderer (docs/PORT.md, "The render interface and
   the sprite hook"). Not a game header.

   The faithful renderer is seg004 translated from the assembly (x86/asmrt.h). It draws into a
   frame buffer, and it draws every object and critter sprite through one call per object;
   these two places are where a later renderer (higher resolution, voxel sprites, a GPU) can be
   put in without touching the faithful code, and with every enhancement off the output is DOS's
   pixel for pixel. */
#ifndef UW2_PORT_RENDER_H
#define UW2_PORT_RENDER_H

#include <stdint.h>

/* A frame buffer the renderer draws into: one byte a pixel (a palette index), row y at
   pixels + y * pitch, y counting up from the bottom of the view as seg003's rows do. */
struct uw_framebuffer {
    uint8_t *pixels;
    int width, height, pitch;
};

/* The frame buffer the faithful renderer draws into: stdat's, at the segment seg003 keeps at
   370D:0958, laid out by cPlaceFB (rows of width + 2 bytes from offset 2). */
void port_render_framebuffer(struct uw_framebuffer *fb);

/* cRender's work: clear the frame buffer, run the render database through the model
   interpreter into fb, stamp the frame time and age the critter page cache. In phase 1 fb
   must be the faithful one (port_render_framebuffer): the translated renderer addresses it
   through seg003's 16-bit row table, 200 rows within one 64 KB segment. */
void port_render_3d(const struct uw_framebuffer *fb);

/* One sprite the renderer is about to draw (TMAPOPS.ASM's in_scalebm, after projecting it). */
struct uw_sprite_draw {
    int kind;                   /* 0 an object's picture (do_uwobj), 1 a critter frame (do_uwcrit) */
    const void *object;         /* the object (struct Object *) DRAWOBJ.C emitted it for, or NULL */
    int item;                   /* the object's type (do_uwobj) or the critter type (do_uwcrit) */
    int frame;                  /* the critter's animation frame number; 0 for an object */
    int tile_x, tile_y;         /* the tile the object is on, or -1 */
    int x, y, z;                /* its position: within the tile (0..7 each way) and its height */
    int heading;                /* its heading, 0..7 */
    int view_x, view_y, view_z; /* the sprite's corner nearest its hot spot, in view space */
    int screen_x, screen_y;     /* the projected top left corner, y counting up */
    int screen_w, screen_h;     /* the projected size in pixels */
    int light;                  /* the distance shade the renderer gives it, 0 (lit) .. 15 */
    const struct uw_framebuffer *fb;
};

/* The per-object sprite hook. The default draws the sprite as DOS did (SCALEBM.ASM's
   scale_nibble_bitmap); a replacement returns 1 when it drew the object itself (a voxel model,
   say) and 0 to have the sprite drawn. NULL restores the default. */
typedef int (*uw_sprite_fn)(const struct uw_sprite_draw *d);
void port_set_sprite_hook(uw_sprite_fn fn);

/* DRAWOBJ.C, through RENDER_TAG (portable.h): the object whose sprite opcode is about to be
   written at db (the render database), so the hook can name it. */
void port_render_tag(const void *object, const void *db);

#endif
