/* target: ovr146 */
/* opts: -mm -1 -G -O -Y -d */
/* One empty far function, the whole of DOS overlay ovr146. Nothing in the sources or the
   extracted modules calls it by name, and FM Towns has no counterpart, so what it once held
   is not known; probably a routine emptied for the DOS build, as in ovr092 (STUBS.C) and
   ovr165 (STUBS2.C).
   name: descriptive (one empty function); the function keeps its IDA name. */

void far ovr146_0(void)
{
}
