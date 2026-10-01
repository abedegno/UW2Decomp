/* target: ovr165 */
/* opts: -mm -1 -G -O -Y -d */

void far newscr(int screen);

/* Declared here because TLINK numbers the overlay's stub entries in the order Turbo C lists
   the publics, which for names with the same hash key is the order they were first seen:
   the EXE's stub has ovr165_E before ovr165_0. */
void far ovr165_E(void);

void far ovr165_0(void) { newscr(16); }
void far ovr165_E(void) { }
