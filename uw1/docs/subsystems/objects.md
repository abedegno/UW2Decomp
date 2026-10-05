# Objects: the object store, classes, using and looking at objects, animations

This page describes how UW1 keeps its objects and what it does when the player uses, looks at or combines them. The sources are in `src/obj`; the declarations, the record layouts and the accessor macros are in `src/include/object.h`, and the item ids in `src/include/items.h`. The candidates for the findings page are in [objects-findings.md](objects-findings.md).

## Files

| File | Segment | What it does |
|---|---|---|
| [`OBJECTS.C`](../../src/obj/OBJECTS.C) | seg027_2861 | the object store: allocating and freeing objects, lists, indices and pointers, searches, garbage collection, the list consistency checks |
| [`USEITEMS.C`](../../src/obj/USEITEMS.C) | seg040_352B | `UseObj` and every item's use handler, locks, doors, the spells and traps objects carry |
| [`LOOK.C`](../../src/obj/LOOK.C) | ovr122 | `LookAt`: the description of an object, a critter, a book, a gravestone, a talisman |
| [`EFFECT.C`](../../src/obj/EFFECT.C) | seg044 | the animated objects (`animlist`): moving doors, explosions, splashes and other effects; their LEV.ARK block |
| [`COMBINE.C`](../../src/obj/COMBINE.C) | ovr099 | combining two objects by `DATA\CMB.DAT`, and rotworm stew |
| [`TREASURE.C`](../../src/obj/TREASURE.C) | ovr150 | spilling a container, and the loot a critter is given |
| [`OBJCLASS.C`](../../src/obj/OBJCLASS.C) | ovr129 | loading `DATA\COMOBJ.DAT` and `DATA\OBJECTS.DAT`; the class data lookup |
| [`HACK.C`](../../src/obj/HACK.C), [`MISC.C`](../../src/obj/MISC.C), [`ANIMOBJ.C`](../../src/obj/ANIMOBJ.C) | ovr116, ovr125, ovr089 | the class tables of OBJECTS.DAT: weapons, missiles and armour; containers, lights and food; animation classes |
| [`OVR139.C`](../../src/obj/OVR139.C), [`OVR144.C`](../../src/obj/OVR144.C), [`OVR146.C`](../../src/obj/OVR146.C) | ovr139, ovr144, ovr146 | the empty class data handlers of the rect, spec and stuff classes |

`DAMAGE.C` (damage to objects) is in `src/combat` and described in [combat.md](combat.md); its prototypes and the damage types are in `object.h`.

UW1 has no build with symbols. The names are UW2's (the FM Towns originals) where the routine is the same one, else the listing's or ours, as each source says. The file names are UW2Decomp's, except `OVR139.C`, `OVR144.C` and `OVR146.C`, which are named after their segments.

## The object store

Every object of a level is a record in the level block (`level.h`): 256 mobile records of 27 bytes (`critdata`; index 0 unused, 1 the player) and 768 static records of 8 bytes (`objdata`, indices 0x100 to 0x3FF). An object is named by its index, and lists are chained through 10-bit link fields: each tile's list head, each object's next link and, unless the object is a quantity (`ID_ISQUANT`), its contents link. The first four words are the same for both kinds (UW-Formats 4.2): the id word (item id, flags, enchanted, door direction, invisible, is-quantity), the position word (height, heading, fine x and y), the quality and next word, and the owner and link word. `object.h` names their masks (`ID_*`, `POS_*`) and has one getter and setter macro for each field (`OBJ_*`, `SET_*`).

Free objects are two stacks of indices, 254 mobile and 768 static. `Obj_Alloc` pops one; when a stack is empty it first culls distant objects (`Obj_GarbageCollect`, range 3), which deletes objects at random, by their `ComObjData` fate, from tiles more than 7 squares from the player. Sleeping culls again (range 1, 20 objects). A tenacious object (id bit 13, `ID_DOORDIR`'s bit) is never culled: the silver seed is made tenacious when it is picked from the silver tree.

`ActiveMob..LastActiveMob` is a byte list of the mobile objects in use; the AI walks it every frame.

## Class data

Each item has its common properties in `ComObjData[512]` (`COMOBJ.DAT`, 11 bytes: height, radius, mass, the flags, value, quality class and type, fate, resistances). Some major classes have a record of their own in `OBJECTS.DAT`: weapons, missiles and armour (`HACK.C`), creatures (`critter/CREATURE.C`), containers, lights and food (`MISC.C`), the trap trigger modes (`event/TRIGGER.C`) and the animation classes (`ANIMOBJ.C`). A caller sets `ActiveObj` and calls `get_class_data`, which dispatches on the major class.

## Using objects

`UseObj` (`USEITEMS.C`) is the one entry for every use: the player using an object in the 3D view or the inventory (`how` 1), a critter opening a door, an object used from the world (`how` 0). It dispatches on the major and minor class to a handler, then sets off any trap or trigger the object holds (`checkTrap`, trigger kind 4) and casts any spell it carries (`checkSpell`, at most once every 0x2FD clock ticks). Objects that act on a second object (keys, the lockpick, spikes, bones, the pole, the anvil, the rock hammer, the oil flask, the orb rock, the Key of Infinity) put themselves on the cursor (`UseThing`), and the handler runs when the player clicks the target.

The rules the handlers implement, read from the code:

- Locks (`checkLock`): a key opens a lock whose link names it; picking needs `skill_check(Pick Lock, 3 * z)` and is impossible for z 15, or z 14 below skill 31; an opened lock with `ID_FLAG10` stays (locked again by a key), else it is removed. Opening sets off the door's traps with kind 6.
- Doors: a closed door (0x140..0x147) becomes a moving door (0x1CF) for 5 frames (a portcullis 4), and its owner bit 0 is the spike (`SpikeDoor`): a critter cannot open a spiked door.
- Food and drink (`UseFood`): the Food table gives the nourishment; a negative value is a drink, and below -1 alcohol, raising the drunk level, with a Strength check that may put the player to sleep. The toadstool poisons; leeches cure poison at the cost of a small backfire.
- Lights burn only in the shoulder and hand slots; using one in the backpack first moves it there.
- Wands cast their spell and become broken wands when the spell object is gone.
- Books read their text from string block 3, play a cutscene with `ID_FLAG10`, or (text 0x100 and up) are the rotworm stew recipe: a bowl holding only a dead rotworm, a mushroom and a flask of port becomes rotworm stew (`make_stew`, `COMBINE.C`).
- Garamon's bones (owner 0x3E) used on his grave bury him: his conversation runs and a trigger at tile (0x36, 0x34) is set off. The orb rock used on the orb shatters it and restores the player's maximum mana.
- Combining (`COMBINE.C`): a rule of `CMB.DAT` (first, second, result; bit 15 of a source word uses that source up) fires when the player drops one object on the other in the inventory. UW1 counts only rules that use up a source.

## Looking

`LookAt` (`LOOK.C`) builds "You see a <quality> <name> of <enchantment> with N charges belonging to <race>." from the string blocks: the quality word from block 5 by the item's quality type and quality band, the enchantment from block 6, the owner's race from block 1. The `lore` argument is what the player knows: 2 shows that an object is magical, 3 names the enchantment and, for the nine talismans (the items with fate 10 in `COMOBJ.DAT`), prints their own text. `SpecialLook` adds a book's text, a key's description, whose bones these are, and a spiked door; `RectLook` reads gravestones (with a picture from `DATA\GRAVE.DAT`) and plaques, and describes the wall behind a texture decal.

## Animated objects

An animated object (major class 7, 0x1C0..0x1CF) is an ordinary object in a tile's list plus an entry in `animlist` (at most 64), with the frames left and its tile (`EFFECT.C`). Its animation class (`animclassd`, the low four bits of the item) has flags (`ANIMF_*`): step the frame, pick a random frame, move as a door, remove at the end, finish the frames first. `update_animobj` runs every frame from the main loop. A moving door swings (a portcullis rises 6 units a frame); a closing door that meets something turns round (`check_door`). `put_effect` makes blood, dust, sparks and spell effects; `mts_doanim` turns a fireball into an explosion with flames and a lightning bolt into a splash.

In UW1 the animation list is a LEV.ARK block of its own, the level plus 8 (`LEVARK_ANIM`), 0x180 bytes, read and written by `Anim_Load` and `Anim_Save` with the level.

## Loot

A critter is given its loot the first time it is needed (`generate_inventory`, `TREASURE.C`, by its `Creature` record): treasure by chance with a type rolled from the dungeon level, food, its two weapons (ammunition in a stack of 4 to 11) and two other items. `drop_link_chain` spills a container or a dead critter where it stands.

## Open questions

- What the remaining unnamed mobile fields mean (`OBJ_B15_6`, `OBJ_B18_*`, `OBJ_B19_*`): `object.h` names them by byte and bit only.
- What the missile ammunition value 0xC0 (`Missile[].ammo`) marks, which `generate_weapons` tests to give a stack.
- Whether UW1's two-function table rows (`seg044_368F_392`, `CreateAnimoForSrcObject_seg044_368F_CE3`) are reachable at all: nothing in the listing calls them.
