# The inventory

This page describes the player's inventory: what the slots are, how objects move between the slots, the cursor, open bags and the world, how weight and container capacity are checked, and how the inventory is saved. The sources are in `src/inv`; the declarations are in `src/include/inv.h`.

## Files

| File | Segment | Name | What it does |
|---|---|---|---|
| [`INVDATA.C`](../../src/inv/INVDATA.C) | ovr124 | descriptive | the inventory as data: slots, adding and taking objects, searching, damage to worn equipment, weight |
| [`INVPANEL.C`](../../src/inv/INVPANEL.C) | ovr125 | descriptive | the inventory panel: drawing the paperdoll and slots, picking up and putting down, stacking and combining, what fits where |
| [`BAGS.C`](../../src/inv/BAGS.C) | ovr121 | descriptive | open containers in the panel, scrolling them, putting objects in, and the panel's special actions |
| [`INVSAVE.C`](../../src/inv/INVSAVE.C) | ovr122 | descriptive | copying the inventory to and from the workspace and `PLAYER.DAT` |

Function and global names are the FM Towns originals where that build has them. `map/filenames.tsv` has the evidence for each file name.

## Objects and slots

The objects the player carries are ordinary objects in the level block's object store: the player object (`ThePlayer`) is a container, and its contents list (`ThePlayer->ol.link`) holds everything carried, with containers holding their own lists in turn. `Inventory[28]` is a second view: one `union Link` per slot naming the object shown there. Every move has to keep the two in step, and the weights with them; that is most of `INVDATA.C`.

The slot layout, from the code (the comment above `INVPANEL.C`'s section in `inv.h` has it too):

| Slots | Meaning |
|---|---|
| 0..4 | head, torso, gloves, legs, boots: the armour slots |
| 5, 6 | the shoulders |
| 7, 8 | the hands; the weapon hand is `8 - lefty`, the shield hand `7 + lefty` |
| 9, 10 | the rings |
| 11..18 | the backpack |
| 19 | the open bag (the innermost container open in the panel) |
| 20..27 | the eight of the open bag's objects on show |

The panel works in display positions (`InvDisplay[23]`, one `struct InvRect` each), mapped to slots by `SlotToDisplay` and `DisplayToSlot`: 0 is the body picture, 1..5 the armour, 6..19 slots 5..18, 20 the open bag and 21, 22 its scroll arrows. `FindInventoryHit` adds 0x17 for a point in the 3D view and 0x18 for the player's barter area in barter mode.

The object on the mouse cursor is `CursorObjPtr`. It belongs to no list while it is held: `SetCursorObj` takes it out of its slot, and it goes back through `RearrangeInventory`, `PutObjectInBag` or `ReturnObject`.

## Data flow

1. A click in the panel reaches `DoInventoryMouse` (registered by `BeginInventory` through `mous_in_panel`). With an empty cursor a press on an object picks it up; dragging a stack asks "Move how many?" (`AskHowMany`) and splits it. With an object on the cursor, `RearrangeInventory` puts it into an empty slot (`AddToEmptySlot`, through `AddToInventory`) or onto an occupied one (`AddToOccupiedSlot`): into a bag that is there, merged with a matching stack (`AddTogether`), combined (`ObjsBeCombinable` and `CombineObjs` in `obj/COMBINE.C`), or swapped.
2. Positions that are not slots go to `DoSpecialActions` (`BAGS.C`): the bag arrows, closing the bag, dropping into the 3D view (`ReturnObject` in `combat/MISSILE.C`, which also throws), the barter area (`conv_inv_special`), fight mode from the weapon hand, and using an object in a slot (`UseObj`).
3. Objects dragged in from the 3D view arrive through `DoInventoryDrag`.
4. The rest of the game asks the inventory through `INVDATA.C`: `AskInventory(slot)` for the weapon hand and armour (`combat/COMBAT.C`), `FindObj` for ammunition, keys and quest items, `DamageInventory` to wear out weapons and armour, `ItemWeight` and `EncumCheck` before a pick-up.
5. Every change of equipment ends with `FixPlayerEquips`, which recomputes what the worn objects give the player (in `game/PLAYDATA.C`).

## Rules found in the code

- **What fits where** (`ItemFitsSlot`). The armour slots take armour (MAJOR_HACK minor 2 and 3) whose wearable type, byte 3 of its class data, matches the slot: 8 head, 1 torso, 4 gloves, 3 legs, 5 boots; the ring slots take type 9. These are the Guide's "Wearable slot" values. The weapon hand refuses a stack of weapons.
- **Eating from the paperdoll.** Food put on slot 0 (the head) is offered to `UseFood`; if it is eaten, `ItemFitsSlot` returns -1 and the object is gone (inferred from the return value; the eating itself is `obj/USEITEMS.C`).
- **Lights.** A lit light (`FIRST_LIT_LIGHT` = 0x94 and on, unlit item + 4) stays lit only in the shoulder and hand slots (`ValidLightSlots`, 5..8). Put anywhere else, into a bag (`PutObjectInBag`) or dropped on the floor (`ReturnObject`), it goes out.
- **Stacking** (`AddTogether`). Two objects merge when they are the same item, both plain stacks, stackable by `ComObjData`'s stack field (not 1 or 3), keys of the same lock (owner), under 999 together, and, except for sling stones, bolts and arrows, in the same quality band (quality / 16), with a ruined (quality 0) object merging only with another. A merged stack's quality is the average of the two. Storage crystals never merge.
- **Containers** (`ItemFitsSlot`, `Containers[]`). A container's capacity (0 for no limit) applies to it and to every open bag around it ("The X is too full."). A container with a mask takes one item, or one kind: runestones (the rune bag, which turns them into runes in `player->runebag`), sling stones, bolts, arrows and wands, scrolls and maps, food and reagents but not drinks, or keys. Which container item has which mask is in `OBJECTS.DAT`, not the code; by what they accept these are probably the rune bag, quiver, map case, bowl and key ring.
- **Opening a bag** switches the container to its open item (even minor class to the odd one after it, below class 12) and closing switches it back (`OpenTheBag`, `MakeBagClose`). The rune bag (class 0xF) opens the rune panel instead.
- **Barter.** In barter mode a container other than class 0xF cannot be picked up: "You cannot barter a container. Instead, remove the contents you want to trade."
- **Moonstones.** Dropping a moonstone, or a container holding one, into the world records the level in a free `player->moonstones` entry, which Gate Travel uses (`combat/SPELLS.C`).
- **Weight.** `ItemWeight` is mass times quantity, or mass plus contents for a container. `PlayerDat.weight` is the carried total and `max_weight` the limit; the panel shows `(max_weight - weight) / 10`, so weights are probably in tenths of a stone. Each open bag keeps its contents' weight (`struct Bag`'s weight) for the capacity checks.
- **Wear** (`DamageInventory`). Damage to a slot's object goes through `damage_item` (`combat/DAMAGE.C`), so resistances and toughness apply. A destroyed object can leave debris by the player; the scroll says "Your X was damaged." or "... destroyed." The player's weapon wears on a bad miss and when striking doors, his armour when a critter scores a critical (`frp_check` in `combat/COMBAT.C`).

## Saving

The inventory is not part of a level's object lists when a level is saved. `SavePlayerInv` copies the player object, the cursor object and everything reachable from them into the workspace, renumbering links to the copy's own indexes: the player at 0, the cursor object at 0x1B, a copy of slots 0..18 at 0x23, and from 0x5B the objects as 8-byte static records numbered from 1. With a directory name it writes `PLAYER.DAT`: the player data (`save_player_data`, `game/PLAYDATA.C`), a word (one more than the object count) and the copy. Without one it only fills the workspace; `game/GAMEWRAP.C`'s `GetLevel` and `SaveLevel` use that pair to keep the inventory out of the way while a level is loaded or written. `RestorePlayerInv` frees the inventory (`Punt_player_inv`) and rebuilds it.

## Open questions

- Which container has which `Containers[]` mask (data in `OBJECTS.DAT`, not checked).
- `FindEmptySlot` (INVDATA.C) is never called and is not in the FM Towns build.
- What using each kind of object from a slot does is `UseObj` in `obj/OBJUSE.C` and `obj/USEITEMS.C`, not traced here.
- The unit of weight (tenths of a stone) is inferred from the display only.
