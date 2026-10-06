# Traps and triggers

This page describes how traps and triggers work in Ultima Underworld I and II: what sets a trigger off and who may, how a trigger finds its trap, how traps chain and branch, what each kind of trap does with its fields, the special-case "hack" traps, how used triggers and their traps are removed, and finding and disarming traps on objects. It is written for someone who wants to reproduce or mod the rules and will not read the C. Every number comes from the matched sources of both games in this repository ([uw1/](../../uw1/), [uw2/](../../uw2/)).

Each rule is marked **both**, **UW1** or **UW2**. [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md#traps-and-triggers) lists the differences in short, and this page does not repeat its references. Statements about the shipped levels were measured from the GOG release's `DATA\LEV.ARK` of each game and say so. Nothing here was checked in a running game.

The object layout is in [FORMATS.md](../FORMATS.md), the code-oriented notes in [subsystems/events.md](../../uw2/docs/subsystems/events.md#triggers-and-traps). The numbered variables that traps share with schedules and conversations are described in [schedules.md](schedules.md#the-row-events).

## Contents

- [Traps and triggers as objects](#traps-and-triggers-as-objects)
- [The trigger kinds](#the-trigger-kinds)
- [Setting a trigger off](#setting-a-trigger-off)
- [Where the actions come from](#where-the-actions-come-from)
- [Pressure plates, enter and exit (UW2)](#pressure-plates-enter-and-exit-uw2)
- [Timer triggers (UW2)](#timer-triggers-uw2)
- [Traps held directly by an object](#traps-held-directly-by-an-object)
- [Running a chain](#running-a-chain)
- [Used triggers and removing traps](#used-triggers-and-removing-traps)
- [The trap kinds](#the-trap-kinds)
- [Damage, arrow, spell and ward](#damage-arrow-spell-and-ward)
- [Teleport and jump](#teleport-and-jump)
- [Changing the map](#changing-the-map)
- [Create, delete and door](#create-delete-and-door)
- [Variables, text and experience](#variables-text-and-experience)
- [Condition traps](#condition-traps)
- [Hack traps, UW1](#hack-traps-uw1)
- [Hack traps, UW2](#hack-traps-uw2)
- [Finding and disarming traps](#finding-and-disarming-traps)
- [Traps laid by spells, wandering monsters, ice and currents](#traps-laid-by-spells-wandering-monsters-ice-and-currents)
- [Open questions](#open-questions)

## Traps and triggers as objects

**Both.** Traps and triggers are objects of major class 6, items 0x180 to 0x1BF. Minor classes 0 and 1 (items 0x180 to 0x19F) are traps, and minor classes 2 and 3 (items 0x1A0 to 0x1BF) are triggers. **UW1** treats only minor class 2 as a trigger.

They use the ordinary object fields:

| Field | Bits | In a trigger | In a trap |
| --- | --- | --- | --- |
| quality | 6 | x of the target square | parameter |
| owner | 6 | y of the target square | parameter |
| link | 10 | the trap it sets off | the next object of the chain (see below) |
| z | 7 | height; for a look trigger, the Search difficulty; for a timer, the interval | parameter |
| heading | 3 | | parameter |
| fine x, fine y | 3 each | for a pressure plate, its state | parameters |
| flags (id bits 9 to 12) | 4 | | how many triggers point at this trap |
| id bit 9 | 1 | UW2: objects may set it off | |
| id bit 10 | 1 | it keeps its trap after use | |
| id bit 11 | 1 | the player may set it off | |
| id bit 12 (enchanted) | 1 | critters may set it off | |

A trigger lies on a tile's object list or inside an object (a door, a container, a switch). Its quality and owner name the square where the trap is, and its link names the trap. A trap lies in the object list of that square.

A trap's link is read like a container's contents. The first object it names is what happens next. For the condition traps, the object after that one in the same list (its "next") is the other branch. For the create-object, delete-object and door traps the linked object is a template, a target or a lock instead ([below](#create-delete-and-door)).

Source: `trap_class_data` UW2 [TRIGGER.C:70](../../uw2/src/event/TRIGGER.C#L70), UW1 [TRIGGER.C:88](../../uw1/src/event/TRIGGER.C#L88); the id bits [object.h:133](../../uw2/src/include/object.h#L133); `trap_init` [TRIGGER.C:62](../../uw2/src/event/TRIGGER.C#L62).

## The trigger kinds

**Both.** Each trigger item has a mode, read at start-up from the 16-byte trigger table in `OBJECTS.DAT` at offset 0xD92 ([FORMATS.md](../FORMATS.md#objectsdat)). The game reports each action with a mode, and a trigger goes off only for its own mode. The tables below are the shipped values (measured). The names are string block 4's.

| Item | Name | UW1 mode | UW2 mode |
| --- | --- | --- | --- |
| 0x1A0 | move trigger | 0 | 0 |
| 0x1A1 | pick up trigger | 2 | 2 |
| 0x1A2 | use trigger | 4 | 4 |
| 0x1A3 | look trigger | 5 | 5 |
| 0x1A4 | UW1 step on trigger, UW2 pressure trigger | 1 | 7 |
| 0x1A5 | UW1 open trigger, UW2 pressure release trigger | 7 | 0xF |
| 0x1A6 | UW1 unlock trigger, UW2 enter trigger | 6 | 6 |
| 0x1A7 | exit trigger (UW2) | | 0xE |
| 0x1A8 | unlock trigger (UW2) | | 0xB |
| 0x1A9 | timer trigger (UW2) | | 0xA |
| 0x1AA | open trigger (UW2) | | 8 |
| 0x1AB | close trigger (UW2) | | 9 |
| 0x1AC | scheduled trigger (UW2) | | 0xC |

**UW2.** Items 0x1B0 to 0x1BC (minor class 3) have the same names and modes as 0x1A0 to 0x1AC. They are the triggers that the Search skill and the Detect Trap spell can find ([Finding and disarming traps](#finding-and-disarming-traps)).

**UW1.** No code reports mode 1, so a step on trigger never goes off. The shipped levels have none (measured).

Source: `OBJECTS.DAT` and string block 4 of each game (measured); `Triggers[]` [TRIGGER.C:52](../../uw2/src/event/TRIGGER.C#L52).

## Setting a trigger off

**Both.** An action calls the trigger with who did it (the player, a critter, another object, or nobody), the object acted on, and a mode. A mode of -1 means the trigger was reached along a trap chain or by a special case, and skips steps 2 to 4.

1. **Is it a trigger?** Anything that is not a trigger does nothing.
2. **Switches.** If the object acted on is a switch (class 0x17) whose index within the class is above 7 (the switch is on), and the trigger has a next object, the action passes to that next object instead. This repeats, so an "on" switch reaches the last trigger in its list.
3. **Mode.** The trigger's mode must equal the action's.
4. **Who.**
   - **The player** needs id bit 11. A look trigger whose z is above 0 also needs `skill_check(Search, z)` to succeed (grade 1 or 2, [combat.md](combat.md#the-dice)).
   - **A critter** needs the enchanted bit (id bit 12).
   - **Any other object**, or nobody: **UW2** needs id bit 9. **UW1** goes off unless the trigger is enchanted and lacks bit 11, so a thrown object sets off almost any trigger. In the shipped UW1 levels 189 of the 190 move triggers that have a trap can be set off by an object this way (measured).
   - A trigger set off by nobody reads its "who" through a null pointer and behaves as for an object ([FINDINGS.md](../../uw2/docs/FINDINGS.md), "A scheduled trigger reads the interrupt vector table").
5. **A trap?** If the trigger's link is empty, nothing happens.
6. **Run the trap** at the target square (the trigger's quality and owner), remembering who set the chain off and the object acted on ([Running a chain](#running-a-chain)).
7. **Used up.** If the trigger lacks id bit 10, its trap is deleted with its whole chain and every other trigger that points at it ([Used triggers and removing traps](#used-triggers-and-removing-traps)). The trigger itself goes with them.
8. **UW2.** A pressure or release trigger then flips its plate ([below](#pressure-plates-enter-and-exit-uw2)).

The critter test also refuses a trigger of major class 5, which a trigger never is, so that part of the test has no effect (it may have been meant for the object acted on; inferred).

Source: `UseTrigger` UW2 [TRIGGER.C:86](../../uw2/src/event/TRIGGER.C#L86), UW1 [TRIGGER.C:104](../../uw1/src/event/TRIGGER.C#L104); `skill_check` [SKILLCHK.C:39](../../uw2/src/game/SKILLCHK.C#L39).

## Where the actions come from

**Both** unless marked. "Who" is who the trigger sees.

| Mode | Action | Who | When |
| --- | --- | --- | --- |
| 0 | move | the mover | An object moving under physics touches a trigger: the player, a critter, a thrown object or a missile. UW2 checks the trigger classes; UW1 passes any trap or trigger and the trigger test throws out the traps. Also the step keys: after a step the player sets off every item 0x1A0 he now overlaps. |
| 2 | pick up | the player | The player takes an object from the world, before it leaves its tile. |
| 4 | use | the user | After an object is used; after food is eaten; after a scroll or a wand is used. Also when a static object is destroyed by damage, as by whoever damaged it. **UW2:** triggers among a dead critter's belongings, or in a container emptied onto the floor, go off as they fall out, as by the player. **UW2:** hack 18 uses a switch, as by nobody. |
| 5 | look | the player | The player looks at an object in the view or in the inventory. The Reveal spell (UW2 `sp_true_sight`, UW1 the same) sets off a look trigger 0x1A3 inside its target with the player's Search raised to 45 for the call. |
| 6 | UW1 unlock | the user | A lock opens by key, by picking or by the Open spell. Locking a lock with a key does not count. |
| 7 | UW1 open | the user | A closed door starts to open (not a moving door turned round). Also UW1's one special case: the Key of Infinity (item 0xE7) used on a special texture-map object (0x16E) that shows a door texture. |
| 6, 0xE | UW2 enter, exit | the mover | An object or the player changes tile ([below](#pressure-plates-enter-and-exit-uw2)). |
| 7, 0xF | UW2 pressure, release | the player | The weight on a plate changes ([below](#pressure-plates-enter-and-exit-uw2)). |
| 8 | UW2 open | the user, or nobody | A closed door starts to open; a moving door is turned round to open; a closing door finds something in the doorway and turns back. |
| 9 | UW2 close | the user, or nobody | An open door starts to close; a moving door is turned round to close. |
| 0xA | UW2 timer | the player | [Timer triggers](#timer-triggers-uw2). |
| 0xB | UW2 unlock | the user | As UW1's mode 6. |
| 0xC | UW2 scheduled | nobody | Schedule event 5 fires every trigger of index 0xC on a square ([schedules.md](schedules.md#the-row-events)). When the castle guards are called out (a castle person would be killed, [subsystems/events.md](../../uw2/docs/subsystems/events.md#world-events)), each scheduled trigger on square (0xF, 0x1D) has the object its trap links to sent to the place of the attack, made temporary and powerful with goal 1, and is then set off with mode -1, as by nobody. |

**Both.** Some special cases set off the first trap or trigger on a square with mode -1: UW2's `fire_trigger_at` (after certain plot deaths, jail, Bliy Skup's chamber and entering certain levels), and a disarm that fails ([below](#finding-and-disarming-traps)).

Source: UW2 `do_objhit` [OBJPHYS.C:135](../../uw2/src/motion/OBJPHYS.C#L135), `simple_fizix` [PHYSICS.C:195](../../uw2/src/motion/PHYSICS.C#L195), `release_3d` [INTERACT.C:333](../../uw2/src/ui/INTERACT.C#L333), `player_3dlook` [INTERACT.C:416](../../uw2/src/ui/INTERACT.C#L416), `UseObj` [OBJUSE.C:48](../../uw2/src/obj/OBJUSE.C#L48), `checkLock` [OBJUSE.C:301](../../uw2/src/obj/OBJUSE.C#L301), `damage_object` [DAMAGE.C:257](../../uw2/src/combat/DAMAGE.C#L257), `drop_link_chain` [TREASURE.C:37](../../uw2/src/obj/TREASURE.C#L37), `changeDoor`, `OpenDoor`, `CloseDoor` [USEITEMS.C:716](../../uw2/src/obj/USEITEMS.C#L716), [USEITEMS.C:738](../../uw2/src/obj/USEITEMS.C#L738), [USEITEMS.C:772](../../uw2/src/obj/USEITEMS.C#L772), `check_door` [EFFECT.C:445](../../uw2/src/obj/EFFECT.C#L445), `sp_true_sight` [SPELLS2.C:439](../../uw2/src/combat/SPELLS2.C#L439), `ev_trigger` [SCDEVENT.C:236](../../uw2/src/event/SCDEVENT.C#L236), `call_out_the_guards` [WORLDEV.C:931](../../uw2/src/event/WORLDEV.C#L931), `fire_trigger_at` [WORLDEV.C:1758](../../uw2/src/event/WORLDEV.C#L1758); UW1 `do_objhit` [OBJPHYS.C:141](../../uw1/src/motion/OBJPHYS.C#L141), `simple_fizix` [PHYSICS.C:188](../../uw1/src/motion/PHYSICS.C#L188), `release_3d` [INTERACT.C:433](../../uw1/src/ui/INTERACT.C#L433), `player_3dlook` [INTERACT.C:509](../../uw1/src/ui/INTERACT.C#L509), `checkLock` [USEITEMS.C:1004](../../uw1/src/obj/USEITEMS.C#L1004), `OpenDoor` [USEITEMS.C:1134](../../uw1/src/obj/USEITEMS.C#L1134).

## Pressure plates, enter and exit (UW2)

**UW2.** When an object or the player leaves a tile, the game looks at the tile's triggers with mode 0xE (exit), and on arriving with mode 6 (enter). Other changes use the pressure modes directly: a change of height within a tile tests release (0xF) at the old height and then pressure (7) at the new one, picking an object up tests release, and an object coming to rest or a missile landing tests pressure.

1. **Enter and exit.** Only the first trigger object (class 0x1A or 0x1B) in the tile's list is tested as an enter or exit trigger. If its mode matches, it goes off with the mover as who. After that first trigger the mode becomes the pressure mode: enter becomes pressure (7), exit becomes release (0xF). In the shipped levels 6 enter or exit triggers are not first in their tile's list and so never go off (measured: level 3 squares (15, 53) and (16, 53), level 5 (35, 29), level 15 (28, 31), level 17 (27, 35), level 52 (30, 47)).
2. **Plates.** A trigger whose mode is the pressure mode in hand, at the same height as the mover, is a plate. Fine y bit 0 is its state, 1 for pressed.
   - The weight test counts everything on the tile at the plate's height: each object's mass times its quantity, with the contents of containers, and for the player his own mass plus what he carries and the object in his hand. The threshold is 4, or 84 when fine y bit 2 is set. The units are tenths of a stone (inferred from the inventory panel).
   - A released plate (bit 0 clear) on pressure (7) goes off when the weight is now above the threshold. A pressed plate (bit 0 set) on release (0xF) goes off when it is not.
   - It goes off as set off by the player, whoever stood on it, so it needs id bit 11.
3. **The plate's state.** After a plate goes off, every plate trigger on the tile flips bit 0. With fine y bit 1 set, the tile's floor texture also steps, up by one when pressed and down by one when released.
4. If the tile had a plate of the other mode and nothing went off, the same weight test is made against it and, if it passes, the plates flip without going off.

Source: `check_pplate`, `update_pplate`, `Ply_Weight` [TRIGGER.C:1091](../../uw2/src/event/TRIGGER.C#L1091), [TRIGGER.C:1147](../../uw2/src/event/TRIGGER.C#L1147), [TRIGGER.C:1079](../../uw2/src/event/TRIGGER.C#L1079), `check_weight` [OBJECTS.C:530](../../uw2/src/obj/OBJECTS.C#L530); callers `change_GrSq`, `hgt_change` [PHYSICS.C:779](../../uw2/src/motion/PHYSICS.C#L779), [PHYSICS.C:812](../../uw2/src/motion/PHYSICS.C#L812), `set_phys_data` [OBJPHYS.C:267](../../uw2/src/motion/OBJPHYS.C#L267), [INTERACT.C:385](../../uw2/src/ui/INTERACT.C#L385), [MISSILE.C:249](../../uw2/src/combat/MISSILE.C#L249).

## Timer triggers (UW2)

**UW2.** Each level keeps a list of up to 64 timer triggers, saved with the level's animation list. Only the level data fills it: nothing in the code adds a trigger to it, so a mod cannot make a new timer at run time without new code. Deleting a timer trigger takes it off the list.

- The animations step 4 times a second of game time (once per 0x40 units of the 256-a-second clock). A counter of these steps runs 0 to 127 and wraps.
- At each step, a timer trigger with height z goes off once each time the counter passes a multiple of z + 1. So it goes off every (z + 1) / 4 seconds, except across the wrap at 128 when z + 1 does not divide 128.
- It goes off only while the player, or the eye of Roaming Sight, is within 8 squares of its target square on both axes (each of |dx| and |dy| below 8).
- It goes off as set off by the player, so it needs id bit 11.
- Nothing runs while time is stopped.

Source: `update_animobj` [EFFECT.C:243](../../uw2/src/obj/EFFECT.C#L243), `add_timer_obj`, `rem_timer_obj` [EFFECT.C:483](../../uw2/src/obj/EFFECT.C#L483), `Anim_Load` [MAP.C:147](../../uw2/src/map/MAP.C#L147).

## Traps held directly by an object

**Both.** When an object is acted on, the game looks for the traps and triggers in its contents (`checkTrap`).

- **UW2.** It takes the trap-class objects in the contents in order. Each trigger is set off with the action's mode, then the next one. For a switch only one runs: the first, or for a switch that is on (index above 7) the second.
- **UW1.** Only the first trap-class object in the contents is looked at.
- **Both.** A plain trap (not a trigger) met there goes off only on use (mode 4), only if its flags are 0 (no trigger points at it), and in UW2 not if it is a door trap. It runs at the object's square and is then deleted. It ends the walk.

In the shipped UW2 levels three objects hold such a trap: a piece of cheese on level 9 and two green potions on level 17, each holding a damage trap (measured). UW1 has one, a red potion on level 2 holding a damage trap (measured).

Source: `checkTrap` UW2 [OBJUSE.C:388](../../uw2/src/obj/OBJUSE.C#L388), UW1 [USEITEMS.C:1079](../../uw1/src/obj/USEITEMS.C#L1079).

## Running a chain

**Both.** The trap runs at the target square. Who set the chain off, and the object acted on, are kept for the whole chain: the first trap of a chain records them and they are cleared when it ends.

1. The trap does its work ([The trap kinds](#the-trap-kinds)).
2. Unless the trap ends the chain, if its link names a major class 6 object, that object runs next:
   - **a trap** runs at the same square;
   - **a trigger** is set off with mode -1 (no mode or who test). Its trap runs at the trigger's own target square, and if it lacks id bit 10 that chain is deleted afterwards, as in step 7 [above](#setting-a-trigger-off).
3. A linked object of any other class does not run.

**These traps end the chain:** create object (always), delete object, ward (and UW1's tell), the UW1 door trap, the UW2 door trap when its owner is not 0, UW1's inventory trap when the item is missing, and a condition trap that branches.

**A branch.** A condition trap that takes its other branch runs the object after its linked object (the linked object's next), a trigger with mode -1 or a trap at the same square, and the chain ends there. If there is no next object, the chain just ends. **UW1:** the check-variable trap is UW1's only branch, and its other branch must be a trigger; a trap there does nothing. Both such branches in the shipped UW1 levels are triggers (measured).

Source: `SetOffTrap`, the end of `UseTrap`, `RUN_ELSE_CHAIN` UW2 [TRIGGER.C:135](../../uw2/src/event/TRIGGER.C#L135), [TRIGGER.C:702](../../uw2/src/event/TRIGGER.C#L702), [TRIGGER.C:234](../../uw2/src/event/TRIGGER.C#L234); UW1 [TRIGGER.C:152](../../uw1/src/event/TRIGGER.C#L152), [TRIGGER.C:426](../../uw1/src/event/TRIGGER.C#L426), [TRIGGER.C:270](../../uw1/src/event/TRIGGER.C#L270).

## Used triggers and removing traps

**Both.** A trap's flags count the triggers that point at it. Deleting a trap (`delete_trap`) goes in this order.

1. If the count is not 0, the game walks every tile list of the 64 by 64 map, and the contents of every object in them, and removes each trigger whose link is the trap, counting down. It stops when the count reaches 0. **UW2:** a removed timer trigger also leaves the timer list. **UW1:** the walk of one list stops at the first trigger it removes there, so a second trigger for the same trap later in that list is left behind ([UW1 FINDINGS.md](../../uw1/docs/FINDINGS.md)). No shipped UW1 list has two such triggers.
2. The trap is found in the given square's list (or inside an object there) and freed with everything its link reaches. Triggers met on the way are deleted as triggers (below), traps as traps.

**Deleting a single trigger** (when a disarm or an object's destruction removes it): if it is the last trigger of its trap (count 1), the trap is deleted as above at the trigger's target square, which removes the trigger too. Otherwise the trigger is freed and the trap's count goes down by one.

**When this happens:**

- a trigger without id bit 10 has run its chain ([Setting a trigger off](#setting-a-trigger-off), step 7);
- a plain trap held by an object has gone off ([above](#traps-held-directly-by-an-object));
- a trap is disarmed ([below](#finding-and-disarming-traps));
- a delete-object trap names a trap or trigger;
- an object holding triggers is destroyed or deleted.

Source: `delete_trap`, `kill_triggers`, `trigger_obj_del`, `trap_obj_del` UW2 [TRIGGER.C:743](../../uw2/src/event/TRIGGER.C#L743), [TRIGGER.C:717](../../uw2/src/event/TRIGGER.C#L717), [TRIGGER.C:969](../../uw2/src/event/TRIGGER.C#L969), [TRIGGER.C:989](../../uw2/src/event/TRIGGER.C#L989), `Obj_FreeChain` [OBJECTS.C:316](../../uw2/src/obj/OBJECTS.C#L316); UW1 [TRIGGER.C:464](../../uw1/src/event/TRIGGER.C#L464), [TRIGGER.C:441](../../uw1/src/event/TRIGGER.C#L441), [TRIGGER.C:596](../../uw1/src/event/TRIGGER.C#L596).

## The trap kinds

The kind is the item's index within major class 6. Names are string block 4's.

| Item | UW1 | UW2 | Section |
| --- | --- | --- | --- |
| 0x180 | damage | damage | [Damage](#damage-arrow-spell-and-ward) |
| 0x181 | teleport | teleport | [Teleport](#teleport-and-jump) |
| 0x182 | arrow | arrow | [Arrow](#damage-arrow-spell-and-ward) |
| 0x183 | do (hack) | hack | [UW1](#hack-traps-uw1), [UW2](#hack-traps-uw2) |
| 0x184 | pit: does nothing | special effects | [Special effects](#damage-arrow-spell-and-ward) |
| 0x185 | change terrain | change terrain | [Changing the map](#changing-the-map) |
| 0x186 | spell | spell | [Spell](#damage-arrow-spell-and-ward) |
| 0x187 | create object | create object | [Create](#create-delete-and-door) |
| 0x188 | door | door | [Door](#create-delete-and-door) |
| 0x189 | ward | ward | [Ward](#damage-arrow-spell-and-ward) |
| 0x18A | tell: acts as a ward | skill | [Ward](#damage-arrow-spell-and-ward), [Skill](#condition-traps) |
| 0x18B | delete object | delete object | [Delete](#create-delete-and-door) |
| 0x18C | inventory | inventory | [Inventory](#condition-traps) |
| 0x18D | set variable | set variable | [Variables](#variables-text-and-experience) |
| 0x18E | check variable | check variable | [Check variable](#condition-traps) |
| 0x18F | combination: does nothing | null: does nothing | |
| 0x190 | text string | text string | [Text](#variables-text-and-experience) |
| 0x191 | | experience | [Experience](#variables-text-and-experience) |
| 0x192 | | jump | [Jump](#teleport-and-jump) |
| 0x193 | | change from | [Changing the map](#changing-the-map) |
| 0x194 | | change to: data for change from, does nothing itself | |
| 0x195 | | oscillator | [Changing the map](#changing-the-map) |
| 0x196 | | proximity | [Proximity](#condition-traps) |
| 0x197 | | pit | [Changing the map](#changing-the-map) |
| 0x198 | | bridge | [Changing the map](#changing-the-map) |

A kind with no action ("does nothing", and any unlisted item) still passes the chain on. The UW2 flam and tym runes (0x19E, 0x19F) are trap-class items that act through being used, not through a chain ([subsystems/objects.md](../../uw2/docs/subsystems/objects.md#using-objects)). The UW1 pit trap appears once in the shipped levels (level 3), the tell and combination traps not at all (measured).

In the sections below, "x, y" is the target square the trap runs at, and "who" is whoever set the chain off.

## Damage, arrow, spell and ward

**Damage (both).** A roll is made first (`rand() * 10 / 0x8000 < 7`) whose result is passed on and not used. Then:

- owner 0: who takes `quality` damage of type 4, through resistances and the damage rules of [combat.md](combat.md#damage-to-critters-the-player-and-objects);
- owner not 0, a poison trap: the player is poisoned at strength `quality` if that is above his current poison, unless the adventurer item's resistances include poison. Poison is a 4-bit field, so a strength above 15 is kept modulo 16. Anyone else takes `quality` damage as above.

The disarm message calls it a "poison trap". In the shipped UW2 levels 9 damage traps have the poison owner, with strengths 0 to 12 (measured).

**Arrow (both).** A missile of item `quality * 32 + owner` (quality's low bit and owner's bit 5 overlap, the two are or'ed) is fired from the trap: from square x, y at the trap's fine position and height, in the direction of the trap's heading plus 2/256 of a turn, as missile class 0x14 ([combat.md](combat.md#missiles)).

**Special effects (UW2).** By quality:

| Quality | Effect |
| --- | --- |
| 2 | plays sound `owner + 0x64` |
| 4 | shakes the screen with strength `owner * 2` |
| 5 to 8 | fills the 3D view with palette colour `owner + 0x40 * (8 - quality)`, a flash |
| others | nothing |

**Spell (both).** Casts the spell of class `quality` and minor `owner` ([magic.md](magic.md#the-spell-classes)), with the trap as the caster, at square x, y, and who as the target.

**Ward (both), and UW1's tell trap.** If `quality` is 0x3F (anyone) or equals the index of who's item within its class of 16, the player reads "Your Rune of Warding has been set off" with the direction to it, and who takes `3 + rand() * Casting / 0x8000` damage of type 4, Casting being the player's. The chain always ends. **UW1:** the Rune of Warding spell lays one ([below](#traps-laid-by-spells-wandering-monsters-ice-and-currents)). **UW2:** nothing in the code lays one and the shipped levels have none (measured).

Source: UW2 [TRIGGER.C:506](../../uw2/src/event/TRIGGER.C#L506), `whack_thing` [WORLDEV.C:478](../../uw2/src/event/WORLDEV.C#L478), `trap_fire` [MISSILE.C:262](../../uw2/src/combat/MISSILE.C#L262), `do_sfx` [WORLDEV.C:635](../../uw2/src/event/WORLDEV.C#L635), `inanimate_spell` [WORLDEV.C:505](../../uw2/src/event/WORLDEV.C#L505), the ward [TRIGGER.C:680](../../uw2/src/event/TRIGGER.C#L680); UW1 [TRIGGER.C:280](../../uw1/src/event/TRIGGER.C#L280), [TRIGGER.C:403](../../uw1/src/event/TRIGGER.C#L403), `whack_thing` [WORLDEV.C:272](../../uw1/src/event/WORLDEV.C#L272).

## Teleport and jump

**Teleport (both).** Moves who to square (`quality`, `owner`) of level `z`. A z of 0 means this level.

1. **UW2, the player only.** While he sleeps in the Ethereal Void (the dream), a teleport to a level outside world 8 is refused. While jailed (quest 112), any teleport except to the cell (x 0x2A or 0x2B, y 0x26, with z 0 or 1) is an escape: quest 112 is cleared and quest 124 set.
2. **Only the player is moved.** For a critter or an object the trap finds nothing to do: with z naming another level (and z 0 counts as another level here) it stops at once, and on this level it only searches for room (step 3) and moves nothing.
3. **Same level.** With z 0 or this level, and neither coordinate 0x3F, the nearest free square to the target is searched for, breadth first in the 9 by 9 box around it. If none is free, the teleport fails.
4. **UW2.** A fighter in the Pits of Carnage who teleports off the level, out of the arena, or into another quarter of it as seen from its centre (0x1F, 0x1F), has run away ([conversations.md](conversations.md#the-pits-of-carnage-and-other-hacks-uw2)).
5. The player's move is only recorded. The main loop carries it out: it changes level if needed and searches again for a free square. If none is found even with the looser search, the player dies (his hit points become 0).
6. **UW2, fine x of the trap:**
   - bit 0 set: the player arrives near the ceiling and falls; clear: on the floor;
   - bit 1 set: the player faces heading `heading` of the trap (eighths of a turn);
   - bit 2 set, when the player set it off: the player is lost. If his automap was on, the current level is remembered as the map scrap level, and the automap is switched off. It stays off until he walks onto a tile whose map entry is 1 to 9 ([UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md#light-exploration-and-the-automap)).

**Jump (UW2).** Acts on the object being moved by physics when the chain runs, so in practice it belongs under a move trigger (inferred). If that object's horizontal speed (|vx| + |vy|) is at least `heading * 0x2F`:

- its gravity becomes -2 unless it is already falling (-4);
- its vertical velocity becomes `quality * 0x2F / 2` for the player, `quality * 0x8D / 4` for anything else;
- its horizontal velocity is halved, or with owner not 0 set to 0.

Below that speed nothing happens; heading 0 always acts. Of the 25 shipped jump traps (levels 33, 34, 46, 57 and 65), 9 have heading 3 and need a speed of 0x8D, and 16 have heading 0 (measured).

Source: UW2 [TRIGGER.C:309](../../uw2/src/event/TRIGGER.C#L309), `do_teleport` [WORLDEV.C:180](../../uw2/src/event/WORLDEV.C#L180), `find_good_x_and_y` [WORLDEV.C:65](../../uw2/src/event/WORLDEV.C#L65), `new_player_pos` [UWEDIT.C:452](../../uw2/src/game/UWEDIT.C#L452), `player_newsq` [PHYSICS.C:627](../../uw2/src/motion/PHYSICS.C#L627), `set_jmp` [MOTION.C:843](../../uw2/src/motion/MOTION.C#L843); UW1 [TRIGGER.C:212](../../uw1/src/event/TRIGGER.C#L212), `do_teleport` [WORLDEV.C:175](../../uw1/src/event/WORLDEV.C#L175).

## Changing the map

**Change terrain (both).** Changes the rectangle from x, y to x + fine x, y + fine y (up to 8 by 8 squares):

| Field | Becomes | Left unchanged when |
| --- | --- | --- |
| wall texture | `owner` | UW1 48 or more, UW2 63 |
| floor texture | `quality / 2` | UW1 11 or more, UW2 15 or more |
| height | `z / 8` | UW1 14 or more, UW2 15 |
| tile type | `heading * 2 + (quality & 1)`, with 15 read as 10 | 10 or more |

The tile types are 0 solid, 1 open, 2 to 5 the diagonals and 6 to 9 the slopes ([map.h](../../uw2/src/include/map.h)).

When a tile's height changes, what stands on it moves with the floor. Objects below a rising floor are lifted onto it, those resting on a sinking floor go down with it, and the player's physics follows.

- **UW2** also moves objects on the neighbouring squares whose footprint reaches over the edge, and never moves traps or major class 5 objects (doors and other fixtures). An object lifted into the ceiling takes 255 damage. A new water floor, a solid type, or (one time in two) a lava floor removes static objects that end up under the new floor or on the old one.
- **UW1** moves only objects on the tile itself, never traps.

**Change from and change to (UW2).** The change-from trap's link is a change-to trap. Over the rectangle from x, y of fine x by fine y squares (fine y 0: the same as fine x), or over the whole map (squares 1 to 62) when fine x is 0, every tile that matches the change-from values gets the change-to values:

| Value | Change from | Change to | "Any" or "leave" |
| --- | --- | --- | --- |
| wall | quality | quality | 63 |
| floor | heading + 8 * (z bit 4) | the same | 15 |
| type | owner | owner | 10 |
| height | z bits 0 to 3 | the same | 15 |

A change-to value at its limit leaves that part alone and is not compared; otherwise the tile must match the change-from value, or the change-from value must be at its limit.

**Oscillator (UW2).** Steps one property of the square up or down by one each time. Fine x bits 1 and 2 choose it: 0 height, 1 floor texture, 2 wall texture. Fine x bit 0 is the direction, 1 up and 0 down.

1. The new value is the old plus or minus 1. For height: a solid tile going down becomes open at height 15, and an open tile reaching 16 becomes solid.
2. If `quality <= new <= owner`, the square is changed.
3. Going up, on reaching `owner`, the direction turns to down. Going down, on reaching `quality`, it turns to up. The turn is made only while the other bound is below a limit: 15 for height (63 when owner is 16), 63 for textures.
4. On level 68 (0x44), each time it turns to up: if square (0x18, 2) is raised, it is lowered to 0 with wall 0x17, floor 4, open, over the 7 by 6 rectangle from it, and square (2, 0x15) is set to wall 0x14, floor 4, height 4, open. The name says this is the sunken moongate.

Mode 3 is not handled, and what it does depends on values left over in the routine. All 64 shipped oscillators move height (measured).

**Pit (UW2).** Toggles the square between height 0 and a stored height.

- At height 0: the new height is `fine x + 8 * fine y` and the floor `owner`.
- Above 0: the new height is 0 and the floor `quality`.
- If the new height is above 15 the tile becomes solid. Otherwise it becomes open, or stays open. Height 15 is allowed. 4 shipped pit traps (levels 51 and 73) store height 15 (measured).

**Bridge (UW2).** Lays or removes `quality` bridges in a row from x, y.

- The trap's heading is first rounded down to even and saved. Heading 0 steps y + 1, 2 steps x + 1, 4 steps y - 1, 6 steps x - 1.
- `owner / 16` is 1 to lay, 2 to remove, anything else to do nothing.
- Laying puts a bridge (item 0x164) at the trap's height and heading, unless one with that height and heading is there, and sets the bridge's flags to `owner & 15`. Removing deletes the bridge with that height and heading.

Source: UW2 [TRIGGER.C:324](../../uw2/src/event/TRIGGER.C#L324), [TRIGGER.C:397](../../uw2/src/event/TRIGGER.C#L397), [TRIGGER.C:401](../../uw2/src/event/TRIGGER.C#L401), [TRIGGER.C:450](../../uw2/src/event/TRIGGER.C#L450), [TRIGGER.C:469](../../uw2/src/event/TRIGGER.C#L469), `change_terrain` [WORLDEV.C:338](../../uw2/src/event/WORLDEV.C#L338), `do_change_grokking` [WORLDEV.C:422](../../uw2/src/event/WORLDEV.C#L422), `check_for_sunken_moongate` [TRIGGER.C:1247](../../uw2/src/event/TRIGGER.C#L1247), `place_bridge` [TRIGGER.C:1190](../../uw2/src/event/TRIGGER.C#L1190); UW1 [TRIGGER.C:218](../../uw1/src/event/TRIGGER.C#L218), `change_terrain` [WORLDEV.C:210](../../uw1/src/event/WORLDEV.C#L210).

## Create, delete and door

**Create object (both).** The chain always ends here.

1. With chance `quality` in 63 nothing happens (`rand() * 63 / 0x8000 < quality`). So quality 0 always creates.
2. Nothing happens if the trap has no link (or its link is a quantity).
3. If the template (the linked object) is a critter and a created (temporary) object other than the player is within 4 squares of it on both axes, nothing happens.
4. **UW2.** In world 6 (levels 49 to 56), once quest 7 is set, nothing happens. Praecor Loth's death sets quest 7 ([WORLDEV.C:805](../../uw2/src/event/WORLDEV.C#L805)).
5. **UW2, the adventurer template.** If the template is item 0x7F, a random monster is chosen:
   - `lo = (level within world * 3 + castle clock) / 9`, at least 1, the level within the world being 1 to 8 and the castle clock X clock 1;
   - `range = player level + castle clock + 1`, at most 16;
   - the creature is `0x40 + (rand() % lo) * 16 + rand() % range`, drawn again until it passes: a creature of strength s passes with chance 1 in s, and one of strength 0 never.
   The template becomes that creature for the copy, is set up as a fresh critter and marked a loner, and is turned back into the adventurer afterwards. The copy is hostile and temporary, and its home is the square where the template lies.
6. A copy of the template is made and placed on square x, y at the template's fine position and height. If the template holds anything, only a copy of its first held object is put in the copy, with nothing inside it. An animated object is added to the animation list.

**Delete object (both).** The linked object is removed from square (`quality`, `owner`) and freed with its contents. The chain ends. **UW2:** a trap or trigger goes through the trap deletion rules ([above](#used-triggers-and-removing-traps)), and an animated object also leaves the animation list.

**Door (both).** Acts on the first door (class 5, minor 0) on square x, y, or, if there is none, a door in motion there (animated item 0x1CF):

| Quality | A door at rest | A door in motion |
| --- | --- | --- |
| 1 | opens it | turns a closing door round to open |
| 2 | closes it | turns an opening door round to close |
| 3 | toggles it (opens a closed door, closes an open one) | turns it round |

Opening, and in UW2 closing, sets off the door's own triggers ([Where the actions come from](#where-the-actions-come-from)).

The lock:

- **UW1.** Every time, for a door at rest, before it opens or closes: the door's lock is removed, and if the trap has a link, a copy of the linked object (a lock template) becomes the door's lock. A door trap without a link therefore unlocks the door. The chain always ends. Of the 27 shipped UW1 door traps, 24 link a locked lock template (measured).
- **UW2.** Only when the trap's owner is not 0, after the door moves, for either kind of door: the lock is replaced the same way, and the chain ends. With owner 0 the chain goes on.

Source: UW2 [TRIGGER.C:523](../../uw2/src/event/TRIGGER.C#L523), [TRIGGER.C:594](../../uw2/src/event/TRIGGER.C#L594), [TRIGGER.C:649](../../uw2/src/event/TRIGGER.C#L649), `dont_create_wandering_monster_here` [TRIGGER.C:1011](../../uw2/src/event/TRIGGER.C#L1011), `eligible_castle_monster` [TRIGGER.C:1240](../../uw2/src/event/TRIGGER.C#L1240), `ToggleDoor` [USEITEMS.C:805](../../uw2/src/obj/USEITEMS.C#L805); UW1 [TRIGGER.C:297](../../uw1/src/event/TRIGGER.C#L297), [TRIGGER.C:339](../../uw1/src/event/TRIGGER.C#L339), [TRIGGER.C:387](../../uw1/src/event/TRIGGER.C#L387).

## Variables, text and experience

**Set variable, UW2.** Changes numbered variable `z + 128 * fine x` by operation `heading` with the value `quality * 64 + owner`. The numbering and the eight operations are in [schedules.md](schedules.md#the-row-events). If fine y is not 0, X clock 15 also goes up by 1, so the schedules notice ([schedules.md](schedules.md#the-x-clocks)).

**Set variable, UW1.** The value is `(quality * 32 | owner) * 8 + fine y`, and the heading is the operation.

- **z 0: a quest bit.** Quest bit `value` (0 to 31 name the 32 quest bits) is toggled by heading 5, cleared by heading 1, and set by any other heading.
- **z 1 or more: game variable z** (of 64). Heading 0 add, 1 subtract, 2 set, 3 and, 4 or, 5 xor, 6 shift left, 7 nothing. The result is kept to 6 bits (0 to 63).

**Text string (both).** Prints string `quality * 32 + owner` of block 9 to the scroll. **UW2** uses only `owner & 31` and prints only when owner bit 5 is clear, or when a player flag (word 0x62 bit 5) is set. Nothing in the code sets that flag (character creation clears it), and no shipped UW2 text trap has owner bit 5 (measured). In the shipped UW1 levels every text trap's quality is `2 * (level - 1)`, so the string is `64 * (level - 1) + owner` (measured).

**Experience (UW2).** Gives `(quality * 8 + (owner & 7) - 256) * 2^(owner / 8)` experience, from -256 up to 255 times 1 to 128. It goes through the usual rules for a gain or a loss ([player-upkeep.md](player-upkeep.md#experience-and-levels)).

Source: UW2 [TRIGGER.C:338](../../uw2/src/event/TRIGGER.C#L338), `set_numbered_variable` [TRIGGER.C:169](../../uw2/src/event/TRIGGER.C#L169), [TRIGGER.C:695](../../uw2/src/event/TRIGGER.C#L695), [TRIGGER.C:501](../../uw2/src/event/TRIGGER.C#L501), `player_get_exp` [SKILLCHK.C:63](../../uw2/src/game/SKILLCHK.C#L63); UW1 [TRIGGER.C:229](../../uw1/src/event/TRIGGER.C#L229), [TRIGGER.C:418](../../uw1/src/event/TRIGGER.C#L418).

## Condition traps

These test something. When the test says "go on", the chain continues through the trap's link as usual. When it says "branch", the object after the linked one runs instead ([Running a chain](#running-a-chain)).

**Check variable (both).** Reads `heading + 1` variables in a row and compares with the value `(quality * 32 | owner) * 8 + fine y`.

- **UW2.** The first variable is numbered `z + 128 * (fine x & 3)`. With fine x bit 2 set the variables are summed; otherwise each one's low 3 bits are packed in turn (`v = v * 8 + (var & 7)`).
- **UW1.** The variables are game variables z to z + heading, summed when fine x is not 0, else packed the same way.
- Equal: go on. Not equal: branch, if the trap has a link. A trap with no link just ends.

**Inventory (both).** Looks for item `quality * 32 | owner` anywhere the player carries it, inside containers too.

- **UW2.** It must be there; with fine x not 0 it must be worn (in an armour slot, a ring slot, or a shield in the shield hand); and with z above 0, a stack must hold at least z. If any of these fails: branch. Otherwise go on.
- **UW1.** If it is missing, or a stack holds fewer than z, the chain ends. Otherwise go on.

**Skill (UW2).** The value tested is, by `quality`: 0 to 2 strength, dexterity or intelligence; 3 the number 15; 4 and up the player's skill `quality - 4` (0 Attack, 1 Defense, ... 9 Casting, 10 Traps, 11 Search, 13 Stealth, 16 Picklock, 17 Acrobat, 19 Swimming; the order of `enum Skill` in [player.h](../../uw2/src/include/player.h)). The difficulty is `owner * 3`.

- heading 1: a skill check, `skill_check(value, difficulty)`, failing at grade 0 or -1;
- any other heading: a plain comparison, failing when the value is below the difficulty.
- **Passing branches; failing goes on.** So the linked chain is what happens on failure, and the object after it what happens on success.

The 12 shipped skill traps test Casting, Search, Stealth, Picklock and the number 15. Eight make a skill check, and seven of those check the number 15 against 15 or 21, a plain chance of 15 or 9 in 31 (measured).

**Proximity (UW2).** Tests where who stands: the tile must be within x to x + `quality` and y to y + `owner`, and its height must pass this test against the trap's z: `(fine x != 0 or height <= z) and (fine y != 0 or height > z)`. So with both fine x and fine y set any height passes, with only fine x set only heights above z, with only fine y set only heights at or below z, and with neither none. Inside: go on. Outside: branch. The shipped proximity traps are 5 with only fine y set and 3 with both (measured).

Source: UW2 [TRIGGER.C:346](../../uw2/src/event/TRIGGER.C#L346), [TRIGGER.C:664](../../uw2/src/event/TRIGGER.C#L664), [TRIGGER.C:364](../../uw2/src/event/TRIGGER.C#L364), [TRIGGER.C:386](../../uw2/src/event/TRIGGER.C#L386), `get_numbered_variable` [TRIGGER.C:210](../../uw2/src/event/TRIGGER.C#L210), `FindObj` [INVDATA.C:126](../../uw2/src/inv/INVDATA.C#L126), `ObjWorn` [INVDATA.C:382](../../uw2/src/inv/INVDATA.C#L382); UW1 [TRIGGER.C:258](../../uw1/src/event/TRIGGER.C#L258), [TRIGGER.C:394](../../uw1/src/event/TRIGGER.C#L394).

## Hack traps, UW1

**UW1.** The "do" trap (0x183) does a special case chosen by its quality. Any other quality does nothing. The chain goes on afterwards.

| Quality | What it does |
| --- | --- |
| 2 | **Crystal ball** (a camera view): shows the view from square x, y at the trap's fine position, height and heading, at light level 6, with automap updates off while it is shown. |
| 3 | Sets the height of square x, y to `(z + 8 * position) / 8`, position being the flags of the switch that set it off, if `z + 8 * position` is below 0x68. |
| 4 | Sets the height (z) of the trap's linked object to the same `z + 8 * position`. |
| 5 | **Trespass**: race `owner` is told the player has done something it objects to, as when he takes its property ([npc-ai.md](npc-ai.md#how-hostility-starts-and-spreads)). |
| 0x18 | **The bullfrog puzzle**, level 4 only (elsewhere a message). Game variables 24 and 25 select a square of the 8 by 8 area at (0x30, 0x30) and variable 26 counts the moves left. By `owner`: 0 and 1 lower or raise the selected square by one and its neighbours inside the area by one too, costing a move ("There is an empty clicking sound." when none is left); 2 and 3 step the selection along y or x, wrapping at 8; 4 resets the area to height 4 and the count to 63 ("Reset Activated."). |
| 0x28 | **The emerald puzzle**: when each corner of the 9 by 9 square around x, y (four squares off each way) holds an emerald, a Vas runestone is put at x, y + 1 and the four emeralds are destroyed. |
| 0x29 | **The exploding book**: if the player's square holds the book (class 4, 1, 4), "The book explodes in your face!", quest 8 is set, the player suffers a backfire of strength 3, and the book is destroyed. |
| 0x2A | **The talking door**: a conversation with whoami 0x19, with no body. |
| 0x32 | Each waiting critter with conversation 0xD8 talks to the player; one that was waiting (goal 7) goes home afterwards. |
| 0x39 | **Arial**: four animation steps, then cutscene 3. |
| 0x3C to 0x3E | **Earthquake**, only when the player set it off: kind `quality - 0x3B` (bit 0 shakes the screen, bit 1 bounces the player), strength `owner`. |
| 0x3F | **The end of the game**: the ending begins, mode `owner + 1`. |

Source: `do_trap_hack` UW1 [TRIGGER.C:501](../../uw1/src/event/TRIGGER.C#L501), `crystal_ball` [PLAYER.C:369](../../uw1/src/game/PLAYER.C#L369), `work_bullfrog_tiles` [WORLDEV.C:355](../../uw1/src/event/WORLDEV.C#L355), `ExplodingBook_ovr107_1259` [WORLDEV.C:466](../../uw1/src/event/WORLDEV.C#L466).

## Hack traps, UW2

**UW2.** The hack trap (0x183) does a special case chosen by its quality. Any other quality does nothing. The chain goes on afterwards. Most are written for one place in the game, and the place is named here only where the code or its strings name it.

| Quality | What it does |
| --- | --- |
| 2 | **Crystal ball**, as UW1's, except that nothing happens when the player's Search skill is exactly 45. Why is not known. |
| 3, 4 | As UW1's. |
| 5 | Trespass, as UW1's, when `owner` is not 0. |
| 10 | Turns the weapon of class `owner` on x, y into something for the player's best skill: a mani stone (or a wooden shield without magic) for a caster, leather gloves for a brawler, 30 to 41 sling stones for a missile user, else a short sword, hand axe or cudgel. The arena's prize (inferred). |
| 11 | **Fraznium**: the force field on x, y is lifted out of the way (height 0x7F) when the player wears the fraznium gauntlets or circlet, else lowered; owner not 0 lowers it regardless. |
| 12 | **A standing wave**: the seven squares north of x, y (y - 1 to y - 7) take heights `h + a * w[i] / 2`, h being the height of x, y, w = 1, 2, 1, 0, -1, -2, -1, and `a = 4 - |4 - owner|` for owner below 8, else `|12 - owner| - 4`. Then owner goes up by 1, modulo 16. |
| 14 | Cycles textures in the rectangle x to x + 2 * fine x, y to y + 2 * fine y: each floor texture from `owner` to `z` steps up by one, wrapping to `owner`; with heading not 0 the wall textures too. |
| 17 | **Thin ice**: if the player's square has floor texture `owner` and he fails an Acrobat check (`skill_check(Acrobat, weight / 12)`, the difficulty 0 when weight / 12 is 20 or less), the square drops by one height step (not below 0) and takes floor texture `fine x + 8 * fine y`. |
| 18 | Flips the switch on x, y to position `owner` and sets off its use triggers. |
| 19 | Resets the arrow pillars: from x, y along each column to the first solid tile, a square holding a trigger of index 4 is set back to that trigger's height, with floor texture chosen by direction when owner is not 0. |
| 20 | Toggles the pillars of a 5 by 3 grid of squares three apart from x, y, those marked in the 15-bit mask `fine x + 8 * fine y + 64 * heading + 512 * z`, between heights `lower = ((owner & 3) + 1) * 2` and `lower + 2 + (owner & 0x1C) / 2`. The Scintillus pillars (from the name). |
| 21, 22 | Sets the linked object's z to the trap's z, or to z + owner if it was not above z already. 21 also does the next object in its group of five. A platform that moves up and down (inferred). |
| 23, 28 | Sets the linked object's owner to `owner`. |
| 24 | **Graffiti**: in the 9 by 9 squares from x, y, each writing (class 5, 2, 0xE) showing one text changes to another with chance p in 16. By owner: 1 0x3E to 0x25, p 16; 2 0x3F to 0x2A, p 10; 3 0x3D to 0x3C, p 6; 4 0x3B to 0x3A, p 6; 5 0x29 to 0x2A, p 16. |
| 25 | **Bliy Skup Ductosnore's chamber**: after his death (quest 122), with the right objects on squares (0x3A, 4), (0x39, 4) and (0x3B, 4), the crystals move to the middle and the trigger at (0x3A, 5) goes off. |
| 26 | Raises or lowers the force fields on x, y. |
| 27 | Sets the linked object's quality to `owner`. |
| 29 | Flips switches at random in the 5 by 2 squares from x, y. |
| 30 | In the Pits of Carnage only: whoever set it off ran from the fight. The player loses the fight; an opponent counts as a win ([conversations.md](conversations.md#the-pits-of-carnage-and-other-hacks-uw2)). |
| 31 | In the Pits only: tries to rescue each pit fighter from the fire. Whether this can ever work is an open question ([FINDINGS.md](../../uw2/docs/FINDINGS.md)). |
| 32 | **The q\*bert floor** (from the name): a square changes colour in a sequence kept in game variables 100 and up; when the 5 by 5 pyramid is all one colour, the walls take it and the reward objects are set. |
| 33 | Turns the empty bottles on x, y into coins. |
| 34 | **Britannia goes dry**, by the castle plot's stage: plants wilt, fountains dry, mushrooms spread. |
| 35 | Recharges the light spheres on x, y to full quality. |
| 36 | The castle household's round ([schedules.md](schedules.md#the-castle-households-day)). |
| 37 | Brings every schedule up to date ([schedules.md](schedules.md#how-a-schedule-runs)). |
| 38 | Spoils the cure potions on x, y, in the cursor and in the player's inventory: a red potion with id bit 11 becomes a green potion holding a poison trap of strength 1 to 8, or water if no trap can be made. |
| 39 | Sets the linked object's invisible bit to `owner`. |
| 40 to 42 | **A vending machine**, `owner` the machine: 40 selects the next item, 41 sells it for half its value plus one in coins put on x, y, 42 prints "<item> is the current selection (N gp).". |
| 43 | Sets the goal of the first critter (every one with fine x not 0) whose whoami is `z + 128 * (heading != 0)`: goal `owner`, target 1 when fine y is not 0, else the index of who. |
| 44 | The player sleeps, as by `owner` ([player-upkeep.md](player-upkeep.md#sleep)). |
| 45 | Marks the `owner` by `owner` block of the map from x, y as not seen on the automap. |
| 54 | The blackrock gem shows a new facet. |
| 55 | Travel through the blackrock gem's facet the player stands at. |
| 62 | Sets the goal of who to `owner`, if the trap's link is who or is 1. |

Source: `do_trap_hack` [TRIGGER.C:773](../../uw2/src/event/TRIGGER.C#L773), `do_ice_hack` [TRIGGER.C:1168](../../uw2/src/event/TRIGGER.C#L1168), `crystal_ball` [PLAYER.C:384](../../uw2/src/game/PLAYER.C#L384), the handlers in [WORLDEV.C](../../uw2/src/event/WORLDEV.C) (`eight_pos_switch` [WORLDEV.C:596](../../uw2/src/event/WORLDEV.C#L596), `toggle_object_height` [WORLDEV.C:614](../../uw2/src/event/WORLDEV.C#L614), `toggle_pillars_hack` [WORLDEV.C:1138](../../uw2/src/event/WORLDEV.C#L1138), `standing_wave` [WORLDEV.C:1313](../../uw2/src/event/WORLDEV.C#L1313), `cycle_floor` [WORLDEV.C:1333](../../uw2/src/event/WORLDEV.C#L1333), `do_graffiti` [WORLDEV.C:1364](../../uw2/src/event/WORLDEV.C#L1364), `reset_arrow_pillars` [WORLDEV.C:1406](../../uw2/src/event/WORLDEV.C#L1406), `go_vend` [WORLDEV.C:2176](../../uw2/src/event/WORLDEV.C#L2176)), `arena_opponent_runs` [CRITTIME.C:604](../../uw2/src/critter/CRITTIME.C#L604), `make_terrain_unseen` [AUTOMAP.C:922](../../uw2/src/ui/AUTOMAP.C#L922).

## Finding and disarming traps

**Both.** In look mode, looking at an object makes a detection roll with the player's Search skill. If it succeeds (grade 1 or 2), the game asks "You found a trap! Do you wish to try to disarm it?", and on yes makes a disarm roll with the Traps skill. Either way the object's look triggers then go off. The Remove Trap spell makes the same two rolls with a skill of 45, and UW2's Detect Trap spell only the first, printing "You have detected a trap." or "You detect no traps.".

**Which traps can be found:**

- **UW1.** The first trap-class object in the object's contents, if it is a damage, teleport or arrow trap, or a trigger whose trap is one of those.
- **UW2.** A trigger of minor class 3 (items 0x1B0 to 0x1BC) anywhere in the object's contents, whatever its trap.

**The rolls** ([combat.md](combat.md#the-dice) for `skill_check`; world = (level - 1) / 8):

| Roll | UW1 | UW2 |
| --- | --- | --- |
| detect | `skill_check(Search, 8)` | `skill_check(Search, 10 + 2 * world)` |
| disarm | `skill_check(Traps, 8)` | `skill_check(Traps, 8 + world)` |

**The disarm result:**

- **Success** prints "The <trap> on the <object> was successfully disarmed." **UW2** deletes the trap at the trigger's target square, with every trigger that points at it. **UW1** frees the object's contents list from its start up to and including the first trap-class object (a trigger goes with its trap if it was the last). So any objects ahead of the trap in the list are destroyed with it. The shipped data was not checked for a trapped object whose trap is not first.
- **Failure (-1)** prints "Your bumbling attempts have set off the <trap>." and sets the trigger off with mode -1, or a plain trap directly and then deletes it.
- **A near miss (0)** prints "Unable to defuse trap.".

**UW2** names a null trap "trap" and a damage trap with owner not 0 "poison trap" in these messages. Naming a special effects trap overruns the name buffer by one byte ([FINDINGS.md](../../uw2/docs/FINDINGS.md)).

Source: `player_3dlook` UW2 [INTERACT.C:416](../../uw2/src/ui/INTERACT.C#L416), `DetectedTrap`, `RemoveTrap` [SKILLS.C:874](../../uw2/src/game/SKILLS.C#L874), [SKILLS.C:893](../../uw2/src/game/SKILLS.C#L893), the spells [SPELLS.C:845](../../uw2/src/combat/SPELLS.C#L845), [SPELLS.C:861](../../uw2/src/combat/SPELLS.C#L861); UW1 [INTERACT.C:509](../../uw1/src/ui/INTERACT.C#L509), [SKILLS.C:884](../../uw1/src/game/SKILLS.C#L884), [SKILLS.C:905](../../uw1/src/game/SKILLS.C#L905), `obj_spells` [SPELLS.C:734](../../uw1/src/combat/SPELLS.C#L734).

## Traps laid by spells, wandering monsters, ice and currents

**The Rune of Warding (UW1).** The spell lays a trap on the square about 9 fine units in front of the caster, with a random spread of up to 13/256 of a turn either way. It makes a move trigger (item 0x1A0, invisible, enchanted so that only critters set it off, used up after one go) aimed at its own square, and a ward trap of quality 0x3F (any critter) with a trigger count of 1. The player reads one of two messages, the second when there was no room for the objects. **UW2** has the same routine (`cast_trap_spell`) but nothing calls it.

**Wards on a wandering monster's path (both).** When a sleeping player is found by a wandering monster, which is moved along a path towards him, the game runs every ward trap that a trigger on the path's squares points at ([npc-ai.md](npc-ai.md#while-the-player-is-away-or-asleep)). It runs the trap directly, outside any chain, so nobody is recorded as having set it off, and the ward's first test (who is not nobody) fails: the monster is not hurt and the ward stays. Read from the code; whether a ward was meant to hurt it is not known. (If the sleep itself came from a trap chain, UW2's hack 44, whoever set that chain off is still recorded and would be the one tested.)

**Wandering monsters (both)** are create-object traps with no trigger pointing at them and a critter template, run by the clock and by sleep ([schedules.md](schedules.md#other-timed-events)).

**Ice and currents (UW2)** are not traps. They come from the floor texture's terrain class and belong to the movement code ([subsystems/motion.md](../../uw2/docs/subsystems/motion.md)). The traps that touch them are hack 17, thin ice, and the schedule's freezing of the ice caverns ([schedules.md](schedules.md#the-row-events)).

**Changing level (both)** is the teleport trap with a level number ([above](#teleport-and-jump)). UW2's blackrock gem (hack 55) is the other way between worlds.

Source: `cast_trap_spell` UW2 [TRIGGER.C:919](../../uw2/src/event/TRIGGER.C#L919), UW1 [TRIGGER.C:547](../../uw1/src/event/TRIGGER.C#L547), `creat_spell` UW1 [SPELLS.C:570](../../uw1/src/combat/SPELLS.C#L570), `wander_that_monster` UW2 [CRITTIME.C:305](../../uw2/src/critter/CRITTIME.C#L305), UW1 [CRITTIME.C:303](../../uw1/src/critter/CRITTIME.C#L303), `DoWanderingMonsters` [TRIGGER.C:1033](../../uw2/src/event/TRIGGER.C#L1033).

## Open questions

- The critter test in `UseTrigger` refuses a trigger of major class 5, which a trigger never is. What it was meant to test is not known.
- UW2's text traps with owner bit 5 depend on a player flag that nothing sets. No shipped trap uses it, so the flag may be a leftover of an editor or a cut feature (inferred).
- The UW2 crystal ball refuses when Search is exactly 45. Why is not known.
- The UW1 disarm frees everything ahead of the trap in the object's contents. Whether any shipped object has contents ahead of its trap was not checked.
- The damage trap's unused roll (7 in 10) may be left from a version that passed a damage kind. It still uses up a random number, which matters to anyone reproducing the random sequence.
- Wards on a wandering monster's path never hurt the monster (above). The code looks for exactly that case, so the omission of a recorded "who" may be a slip.
- The places of most UW2 hack traps were not looked up in the levels.
