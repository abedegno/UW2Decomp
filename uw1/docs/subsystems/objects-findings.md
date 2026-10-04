# Objects: candidates for the findings page

Candidates from `src/obj` for the project's findings page; [objects.md](objects.md) describes the subsystem. Each bug candidate was re-read in the matched source; what the code does is read from the code, and the effect in the game is an inference unless it says it was checked. "Both games" means UW2Decomp's matched source has the same code.

## Likely bugs

### Valuable gems skip the treasure chance and always come singly

- **What happens:** `generate_treasure` keeps the treasure type's value in a `char` and stretches it (`value * 8 - 68` from 12 up). From 25 up the result does not fit and goes negative: the ruby (25) becomes -124, the large blue gem (30) -84 and the sapphire (40) -4. With a negative value the test that makes valuable treasure rare (`(prob << 2) < value`) is false, the dice count `((prob << 2) / value) << 1` is 0 or negative, and `rollem` given no sides returns its dice count, 4, so the quantity is 1.
- **Where:** `generate_treasure` in [obj/TREASURE.C](../../src/obj/TREASURE.C).
- **Evidence:** code reading, with the values of items 0xA2, 0xA5 and 0xA6 read from the shipped `COMOBJ.DAT`; both games.
- **Confidence:** confirmed (the shipped data reaches the overflow).
- **Effect:** when a critter's loot roll picks a ruby, large blue gem or sapphire, it always gets exactly one, where the rule makes the cheaper red and small blue gems rarer. Not checked in the game.
- **For a port:** matching DOS means keeping the 8-bit arithmetic.

## Possible bugs

### Loot objects are not checked for null

- **What happens:** `generate_treasure`, `generate_food`, `generate_weapons` and `generate_equipment` write to the object `CreateObj` returns without testing it; `CreateObj` returns 0 when no object can be allocated even after garbage collection.
- **Where:** [obj/TREASURE.C](../../src/obj/TREASURE.C).
- **Evidence:** code reading; both games.
- **Confidence:** possible (needs an object store that garbage collection cannot relieve).
- **Effect:** a write through a null far pointer, into the interrupt vector table, when the object store is full.

### The list checker forgets a duplicate

- **What happens:** `count_list`, a debugging aid, sets `bad` when it meets an index twice, but then assigns the result of checking a container's contents to the same flag, so a duplicate found earlier in the list is forgotten when the contents after it are clean.
- **Where:** `count_list` in [obj/OBJECTS.C](../../src/obj/OBJECTS.C) (UW1 only).
- **Evidence:** code reading.
- **Confidence:** possible; it only affects the consistency report (`seg027_2861_EF9`).

## Recovered rules

- Culling: when a free list runs out, `Obj_Alloc` deletes objects at random by their `ComObjData` fate from tiles more than 7 squares (taxicab) from the player, 5 mobile or 10 static at a time; sleeping culls 20 more. Objects with id bit 13 (tenacious) are never culled; the silver seed is made tenacious.
- Using an object from the world casts its spell at most once every 0x2FD clock ticks (`checkSpell`), and spends a charge; a spell object with no charges left disappears four times in ten.
- A spell object of quality 0 is not recognised four times in ten (`decode_obj_spell`) unless the look code asks with `always_decode`.
- Rotworm stew: a bowl holding only a dead rotworm, a mushroom and a flask of port, each at least once, when the recipe is read.
- Burning incense plays the three dream cutscenes in turn, then one at random.
- The talisman descriptions (`talisman_desc`) are chosen by item: the nine items with fate 10 in `COMOBJ.DAT`. Its `which` would be left unset for any other fate-10 item, but the shipped data has none.

## Dead code

- `seg044_368F_392` (end an object's animation at once) and `CreateAnimoForSrcObject_seg044_368F_CE3` in [obj/EFFECT.C](../../src/obj/EFFECT.C): nothing in UW1 calls them; they are UW2's, kept static.
- EFFECT.C's four bytes `unknown[4]` (DS:3698): nothing reads or writes them.

## Candidates that did not hold

- `generate_weapons` gives ammunition a stack count without setting `ID_ISQUANT`: `CreateObj` already sets it for stackable items.
