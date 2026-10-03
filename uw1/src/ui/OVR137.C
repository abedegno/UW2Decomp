/* target: ovr137 */
/* opts: -mm -1 -G -O -Y -d */
/* DOS overlay ovr137: one empty far function, which nothing in UW.EXE calls (only its
   overlay stub refers to it). The same bytes as UW2's mous_3d_show (UW2Decomp
   src/ui/MOUSE.C), which is empty too, but any empty function compiles to these five
   bytes, so the kin says nothing of what it was.
   File name: provisional (the original name is unknown; named after the segment).
   name: the listing name. */

void far ovr137_0(void) { }
