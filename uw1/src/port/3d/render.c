/* render.c: replaces nothing. RENDER_TAG's port side (Exhume's portable.h): the game is about to
   hand an object to the renderer. UW2's port keeps these for a per-object sprite hook (an
   enhancement); UW1's port has none yet, so it records nothing, and the faithful renderer, the
   translation of seg004, draws every object as DOS did. */
void port_render_tag(const void *object, const void *db)
{
    (void)object;
    (void)db;
}
