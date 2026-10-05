/* target: ovr144 */
/* opts: -mm -1 -G -O -Y -d */
/* DOS overlay ovr144: spec_class_data, an object class's data handler that returns 0 (OBJCLASS.C's
   class table holds its stub, so that is its name; the bytes are the same as UW2's unbound,
   UW2Decomp src/conv/BABL.C). ovr139, ovr144 and ovr146 each hold one such handler, for the
   rect, spec and stuff classes, which in UW1 have no data of their own.
   Data owned: none.
   File name: provisional (the original name is unknown; named after the segment).
   name: from the class table's reference (OBJCLASS.C). */

int far spec_class_data(void) { return 0; }
