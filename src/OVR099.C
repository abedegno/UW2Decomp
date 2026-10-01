/* target: ovr099 */
/* opts: -mm -1 -G -O -Y -d */
/* Seven empty functions, one returning 1: hooks whose bodies were compiled out of the DOS
   build. FM Towns has the same seven, the browse functions: free_browse and init_browse share
   one empty function, place_browse returns 1, and browse_chain, browse_hide, browse_redisplay
   and browse_show share another empty one, as DOS has two empty functions, one returning 1,
   then four empty ones. Which name goes with which empty function is set by the EXE's overlay
   stub order: Turbo C lists publics by the tools/bssorder.py key of each name, and only this
   assignment of the names reproduces it. */

void far init_browse(void) { }
void far free_browse(void) { }
int far place_browse(void) { return 1; }
void far browse_show(void) { }
void far browse_hide(void) { }
void far browse_redisplay(void) { }
void far browse_chain(void) { }
