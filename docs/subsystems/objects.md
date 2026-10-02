# Objects

This page describes how UW2 stores the objects of a level, the fixed data of each kind of object, using and looking at objects, animated objects and loot. The sources are in `src/obj`; the declarations are in `src/include/object.h` (with the item ids, classes and trap types in `items.h`, which `object.h` includes) and `src/include/uw2.h` (`union Link`).

## Files

| File | Segment | Name | What it does |
|---|---|---|---|
| [`OBJECTS.C`](../../src/obj/OBJECTS.C) | seg029 | inferred (the `Obj_` prefix; System Shock's `OBJECTS.C`) | the object store: allocating and freeing, lists, index and pointer conversion, searches, pressure plate weights, garbage collection |
| [`OBJCLASS.C`](../../src/obj/OBJCLASS.C) | ovr134 | descriptive | loading `OBJECTS.DAT` and `COMOBJ.DAT`, `get_class_data` |
| [`HACK.C`](../../src/obj/HACK.C), [`MISC.C`](../../src/obj/MISC.C), [`ANIMOBJ.C`](../../src/obj/ANIMOBJ.C), [`STUFF.C`](../../src/obj/STUFF.C), [`SPEC.C`](../../src/obj/SPEC.C), [`RECT.C`](../../src/obj/RECT.C) | ovr120, ovr130, ovr091, ovr159, ovr155, ovr148 | inferred (the class prefixes) | each major class's class-data loader and lookup |
| [`OBJUSE.C`](../../src/obj/OBJUSE.C) | seg040 | inferred (System Shock's `OBJUSE.C`) | `UseObj`, two-object uses, locks, the spells and traps objects carry |
| [`USEITEMS.C`](../../src/obj/USEITEMS.C) | ovr138 | descriptive | the use of each kind of object: food, keys, lights, doors, books, containers, tools, runes |
| [`LOOK.C`](../../src/obj/LOOK.C) | ovr126 | descriptive | descriptions: `LookAt`, `GetObjDesc`, books, gravestones, bones, keys, critters |
| [`EFFECT.C`](../../src/obj/EFFECT.C) | seg044 | inferred (System Shock's `EFFECT.C`) | the animation list (moving doors, explosions, effects) and the timer triggers |
| [`COMBINE.C`](../../src/obj/COMBINE.C) | ovr102 | descriptive | combining two objects into a third (`DATA\CMB.DAT`) |
| [`TREASURE.C`](../../src/obj/TREASURE.C) | ovr163 | descriptive | a critter's loot, spilling containers |

Function and global names are the FM Towns originals where that build has them. `map/filenames.tsv` has the evidence for each file name.

## The object store

Every object of a level lives in the level block (`struct LevelBlock`, `level.h`; [map.md](map.md) has its layout): 256 mobile records of 27 bytes (`critdata`; index 0 unused, index 1 the player) and 768 static records of 8 bytes (`objdata`, indices 0x100 to 0x3FF). An object is named by its index. Lists are chained through link words (`union Link`, `uw2.h`): each tile's object list head, each object's next link (`qn`), and, unless the object is a quantity (`is_quant`), its contents link (`ol`). A quantity object uses `ol` for the count instead, and a count with `LINK_SPECIAL` (0x200) set is a special value rather than a count (used for map scraps and enchantments).

`struct Object` (`object.h`) is the mobile record; its first 8 bytes are `struct StaticObj`, the whole of a static object. The id word holds the item id (9 bits: major class, minor class, index), the flags, the door direction bit, the enchanted bit, the invisible bit and `is_quant`; the position word the height, heading and fine position; the quality and owner fields serve many purposes (a critter's home square, a trigger's target square, a key's lock number, an owner race). `object.h` has accessor macros for every packed field (`OBJ_ITEM`, `OBJ_Z`, `SET_HEADING` ...).

Free objects sit on two stacks (`critbot`..`critptr`, 254 mobile; `objbot`..`objptr`, 768 static); `Obj_Alloc` pops one and `Obj_Free` pushes it back. `ActiveMob`..`LastActiveMob` lists the mobile objects in use, which the critter code walks each frame. When a free list runs out, and when the player sleeps, `Obj_GarbageCollect` culls distant objects.

## Fixed data per item

Each item id has common properties, `struct ComObj` (`object.h`), 11 bytes each from `DATA\COMOBJ.DAT` in `ComObjData[]`: height, radius, mass, value, quality class (toughness; 3 is indestructible), quality type (which group of quality words describes it), whether it can be picked up, owned or looked at, its fate, resistances and how it is drawn. Some major classes also have a class record from `DATA\OBJECTS.DAT` (UW-Formats 6.3): the weapons, missiles and armour tables (`HACK.C`; `struct Weapon`, `struct MissileInfo`, `struct Armour` in `combat.h`), the creature table (`critter/CREATURE.C`), containers, lights and food (`MISC.C`), the trigger types (`event/TRIGGER.C`) and the animation classes (`ANIMOBJ.C`). `get_class_data` returns the class record of `ActiveObj`.

The major classes (`items.h`, by their FM Towns names): `MAJOR_HACK` weapons, missiles and armour; `MAJOR_CREATURE`; `MAJOR_MISC` containers, lights, wands, treasure and food; `MAJOR_STUFF` scenery, potions and runestones; `MAJOR_SPEC` keys, quest items, magic items and books; `MAJOR_RECT` doors, furniture, decals and switches; `MAJOR_TRAP` traps and triggers; `MAJOR_ANIMOBJ` animated objects.

## Using objects

`UseObj` (`OBJUSE.C`) is every use: the player in the 3D view or the inventory, a critter opening a door, two objects colliding. It dispatches on the class to the `Use*` functions of `USEITEMS.C`, then fires the traps and triggers the object contains (`checkTrap` with mode 4, use) and casts the spell it carries (`checkSpell`). An object that needs a target (a key, a pole, oil, a rock hammer, an anvil) goes through `UseThing`: "Use X on what?", the object on the cursor, and the `*On` function called on the next click.

Rules found in the code:

- **Locks** (`checkLock`). A lock is an object (`ITEM_LOCK`) in the door's or container's contents: its id bit 9 means locked, its link is the number of the key that fits (the key's owner field), its height the difficulty. Picking rolls `skill_check(Picklock, 3 * difficulty)`; difficulty 0xE needs a skill of at least 0x20 and 0xF cannot be picked. A fumble can break the pick unless a dexterity check passes. Unlocking fires the door's unlock triggers (mode 0xB) and deletes the lock, unless id bit 10 marks a lock that can be locked again. A key can lock an unlocked lock, but not on an open door.
- **Spells in objects** (`decode_obj_spell`). A spell object (item 0x120) inside an object, or the object's own enchanted bit, holds a spell; with id bit 11 it is cast on use and its quality counts the charges (`useNSpellCharges`; when charges run out the spell object is deleted 4 times in 10). Casting from objects is limited to once per 0x2FD clock ticks (about 3 game seconds). A wand whose spell is gone breaks ("With a loud <SNAP!>, the wand cracks.").
- **Food** (`UseFood`). A food item's byte in the food table is its nourishment; negative values are alcohol, which raises the drunk level and rolls a strength check (passing out on -1). Taste follows quality ("tasted putrid" to "tasted great"). Some foods leave a stick, a bone, wax or an empty bottle. The dream plant (0x114) puts the player to sleep for the Ethereal Void dream.
- **Lights** (`UseLight`) burn only in the shoulder and hand slots and must be single items; a used-up light (quality 1 or less) will not light. Oil (`UseOilOn`) refuels a lantern or torch by 0x20 quality or makes a torch from wood.
- **Doors** (`OpenDoor`, `CloseDoor`, `moveDoor`). Opening or closing turns the door into the moving door animation object (0x1CF) for 5 frames (a portcullis 4) and fires its open (8) or close (9) triggers; `EFFECT.C` turns it back into a door. A closing door that meets an obstacle turns round.
- **Containers.** Used in the world, a container's contents are dumped on the floor (`DumpTheBag`), which is theft if it is owned; in the inventory it opens in the panel.
- **The blackrock gem** (`UseKeyGem`). Each treated key gem used on the large gem counts `XC_GEMS` and sets its bit in `QB_GEMS_USED`, opening that facet for travel (`event/WORLDEV.C`'s `black_gem_trip`); gem 4 also advances the castle plot.
- **Combining** (`COMBINE.C`). `DATA\CMB.DAT` has five rules: pole and thread make a fishing pole, thread and wax a candle, a lit torch and corn popcorn, a lit torch and a honeycomb wax, a nutritious wafer and water ale.
- **Rune traps** (`UseRune`). A rune of flame does 3d4 + 4 fire damage to whoever touches it and explodes; a rune of stasis paralyses.

## Looking

`LookAt` (`LOOK.C`) builds "You see a <quality> <name> of <spell> with N full charges belonging to <race>." from the string blocks: the quality word from block 5 by quality type and quality band, the name from block 4, the enchantment from block 6, the owner race from block 1 (strings 0x172 on: a worm, a slug, ... a human). The lore level decides how much shows: 2 that an object is magical, 3 its enchantment, charges, curses and poisoned potions. Looking at an object in the inventory rolls the Lore skill once (`ui/INTERACT.C`'s `inv_look`) and keeps the result in the object's heading until the Lore skill rises (`event/WORLDEV.C`'s `clear_all_loretries`).

## Animated objects

`EFFECT.C` keeps up to 64 running animations (`animlist`): an animated object in a tile's list, its frames left and its tile. The animation class (16 of them, `animclassd`, from `OBJECTS.DAT`) says each frame what to do: cycle or pick a random frame, or move a door. `update_animobj` steps them each frame and ends the expired ones. `put_effect` makes blood, dust, sparks and spell effects. The same file runs the timer triggers: each fires every z + 1 ticks while the player or the roaming eye is within 8 squares of its target.

## Loot

`generate_inventory` (`TREASURE.C`) gives a critter, once, the loot of its creature record: treasure with a chance (better treasure is commoner in later worlds), food, two weapons at a random quality, and two other items. A dead critter's objects are dropped where it stands and belong to its race, so taking them can count as theft.

## Open questions

[FINDINGS.md](../FINDINGS.md) collects the open questions and likely bugs of every subsystem in one place.

- Several id bits are named only by number (`ID_FLAG9`, `ID_FLAG10`, `ID_FLAG11`); their meaning differs by class (triggers, books, spells).
- `make_stew` is an empty stub in both builds; what a link of 0x100 or more on a book meant is not known.
- `BonesLook` (`LOOK.C`) has a case for "Relk." (owner 0x3D) that its own condition makes unreachable.
- The food table's values are taken as nourishment from the code; UW-Formats lists the table as unknown.
