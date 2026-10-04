# The inventory

This page describes the player's inventory in UW1: the slots, the panel that shows them, open bags, and saving the inventory with a game. The sources are in `src/inv`; the declarations, the slot and display numbering and the panel records are in `src/include/inv.h` (`INVSAVE.C`'s prototypes are in `file.h`). The candidates for the findings page are in [inventory-findings.md](inventory-findings.md).

## Files

| File | Segment | What it does |
|---|---|---|
| [`INVPANEL.C`](../../src/inv/INVPANEL.C) | ovr121 | the panel: its rectangles, picking up and putting down, stacking, combining and swapping, what fits where, the paperdoll, the weight display, the hit test |
| [`INVDATA.C`](../../src/inv/INVDATA.C) | ovr120 | the inventory as data: adding and removing objects (whole or part of a stack), searching by class, worn equipment and its wear, weights |
| [`BAGS.C`](../../src/inv/BAGS.C) | ovr117 | open bags: opening, closing and scrolling containers in the panel, putting objects in them, the panel's other clicks |
| [`INVSAVE.C`](../../src/inv/INVSAVE.C) | ovr118 | saving the inventory to `PLAYER.DAT` and restoring it, through the workspace |

Names are UW2's (the FM Towns originals), the routines being the same; UW1 has no symbols of its own. The file names are UW2Decomp's.

## Slots and display positions

The inventory is the player object's contents list; `Inventory[28]` names the object shown in each slot (`inv.h`):

| Slots | Name | What |
|---|---|---|
| 0..4 | `INV_HEAD` .. `INV_BOOTS` | head, torso, gloves, legs, boots: the armour slots |
| 5, 6 | `INV_SHOULDER` | the shoulders |
| 7, 8 | `INV_HAND` | the hands: the weapon hand is `INV_WEAPON_HAND - lefty`, the shield hand `INV_HAND + lefty` |
| 9, 10 | `INV_RING` | the rings |
| 11..18 | `INV_PACK` .. `INV_PACK_LAST` | the backpack |
| 19 | `INV_BAG` | the innermost open bag |
| 20..27 | `INV_BAG_ITEMS` .. `INV_BAG_LAST` | the eight of its objects on show |

The panel's rectangles (`InvDisplay[23]`) are numbered differently, by display position: 0 the body, 1..5 the armour, 6..19 slots 5..18 (an open bag's objects take 12..19), 20 the open bag, 21 and 22 its scroll arrows. `SlotToDisplay` and `DisplayToSlot` translate. `FindInventoryHit` also returns 0x17 for the 3D view and, in barter mode, 0x18 for the player's trade area.

## What fits where

`ItemFitsSlot` (`INVPANEL.C`) holds the rules:

- The armour slots take armour of the matching category (`struct Armour`, OBJECTS.DAT): hat, body armour, gloves, leggings, boots; the ring slots take rings. Food dropped on the head slot is eaten.
- The weapon hand refuses a stack of weapons.
- A lit light stays lit only in a shoulder or hand slot; elsewhere it is tested and put down unlit.
- Into a container, the weight must fit its capacity and that of every open bag around it ("The <bag> is too full."), and a container with a mask takes only that item or kind (`CONT_*` in `object.h`): the rune bag runestones, the quiver sling stones, bolts and arrows, the map case scrolls and the map, the bowl food and drink (class 0x0B) and a few edible items (the plants, the candle, leeches, rotworm stew, the dead rotworm). The masks come from UW1's `OBJECTS.DAT`; UW2's key ring mask (`CONT_KEYS`) has no container in UW1.
- Otherwise the item's `ComObjData` pickup flag decides.

Stacks of the same item merge (`AddTogether`) while their total is below 999 and, except for sling stones, bolts and arrows, their quality bands agree; keys merge only with the same owner (the lock they open). The merged quality is the average. Dropping one object on another that a `CMB.DAT` rule combines makes the result (`COMBINE.C`).

## Open bags

Opening a container replaces the backpack rows with its contents (`OpenTheBag`, `BAGS.C`). Open bags form a chain (`struct Bag`), each remembering its contents' weight so capacity checks are cheap; the container's item switches to its open form while it is open. The rune bag shows the rune panel instead. The arrows scroll by rows of four.

## Weight and wear

The carried weight is the player record's `weight`, in tenths of a stone, kept in step with every move; the panel shows `max_weight - weight`. `DamageInventory` wears the item in a slot: a destroyed item can leave a pile of debris by the player. `COMBAT.C` wears the weapon on a bad miss or a blow at a door, and the armour at the hit location on a critical.

## Saving

`SavePlayerInv` (`INVSAVE.C`) copies the player object, the cursor object and every object reachable from them into the workspace, renumbered from 1, with a copy of the slots, and writes it to `PLAYER.DAT` after the player record; `RestorePlayerInv` reads it back. The inventory travels separately because the level's object lists are saved per level.

## Open questions

- Why `InvRemoveObject`'s path for an object without a slot redraws display position 19 (`DisplayInvObject(0x13)`) rather than the open bag.
