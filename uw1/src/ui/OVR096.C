/* target: ovr096 */
/* opts: -mm -1 -G -O -Y -d */
/* DOS overlay ovr096: the browse hooks, seven functions, six empty and place_browse
   returning 1, that nothing in UW.EXE calls (only their overlay stubs refer to them).
   Byte for byte the same as UW2's ovr099 (UW2Decomp src/ui/BROWSE.C), whose names are
   the FM Towns build's. The file owns no data.
   File name: provisional (the original name is unknown; named after the segment; UW2's
   copy is BROWSE.C).

   name: as in UW2, which name goes with which empty function is set by the EXE's overlay
   stub order (17 12 21 5 0 1C A); UW2's assignment reproduces it here too. */

void far init_browse(void) { }
void far free_browse(void) { }
int far place_browse(void) { return 1; }
void far browse_show(void) { }
void far browse_hide(void) { }
void far browse_redisplay(void) { }
void far browse_chain(void) { }
