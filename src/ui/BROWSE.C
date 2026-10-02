/* target: ovr099 */
/* opts: -mm -1 -G -O -Y -d */
/* The browse hooks: seven functions, six empty and place_browse returning 1, that nothing
   in UW2.EXE calls. Their bodies were compiled out of the DOS build (FM Towns has the same
   seven, also empty), so whatever "browsing" meant in Looking Glass's code (probably an
   object or inventory browser, by the names alone) did not ship. The file owns no data.

   Name: inferred (all seven functions are *_browse in the FM Towns build: init_browse ...
   browse_chain). */

/* name: FM Towns has the seven browse functions, with free_browse and init_browse sharing
   one empty function, place_browse returning 1, and browse_chain, browse_hide,
   browse_redisplay and browse_show sharing another empty one; DOS has two empty functions,
   one returning 1, then four empty ones. Which name goes with which empty function is set
   by the EXE's overlay stub order: Turbo C lists publics by the tools/bssorder.py key of
   each name, and only this assignment of the names reproduces it. */

void far init_browse(void) { }
void far free_browse(void) { }
int far place_browse(void) { return 1; }
void far browse_show(void) { }
void far browse_hide(void) { }
void far browse_redisplay(void) { }
void far browse_chain(void) { }
