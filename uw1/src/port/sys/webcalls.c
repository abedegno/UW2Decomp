/* webcalls.c: replaces nothing; the web build only (portbuild.py --web, which renames these
   calls in the files exhume.toml's [port.web.renames] names). A few of UW1's calls declare the
   function otherwise than its definition does, as the original's own sources did: Turbo C's far
   calls, and the desktop's, let a call with more or fewer arguments, or another return type,
   through, but WebAssembly's linker makes such a call a trap. Each adapter here has the caller's
   type and calls the definition; what it drops or supplies is never read by the function, and
   what it drops from a return the caller never reads. */
#ifdef __EMSCRIPTEN__
struct Object;

unsigned char grfx_load_font(char *name);                               /* GRFX.C */
void mantra_advance(void);                                              /* SKILLS.C */
void ExplodingBook_ovr107_1259(struct Object *trap, int x, int y);      /* WORLDEV.C */
void player_setup(int x, int y);                                        /* PHYSICS.C */

/* CONVERSE.C and BARTER.C declare it void (UW2's grfx_quikfont is) */
void web_grfx_load_font_void(char *name) { grfx_load_font(name); }

/* CONVERSE.C and USEITEMS.C pass an argument SKILLS.C's definition does not take */
void web_mantra_advance_int(int n) { (void)n; mantra_advance(); }

/* USEITEMS.C calls the hack trap with no arguments; it reads none of its three */
void web_ExplodingBook_void(void) { ExplodingBook_ovr107_1259(0, 0, 0); }

/* MAINMENU.C, SPELLS.C and UWEDIT.C pass a third argument PHYSICS.C's definition does not take */
void web_player_setup_how(int x, int y, int how) { (void)how; player_setup(x, y); }
#endif
