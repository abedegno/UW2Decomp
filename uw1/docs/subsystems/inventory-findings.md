# The inventory: candidates for the findings page

Candidates from `src/inv` for the project's findings page; [inventory.md](inventory.md) describes the subsystem. Each candidate was re-read in the matched source; what the code does is read from the code, and the effect in the game is an inference unless it says it was checked.

## Engine findings

- UW1's container masks (`OBJECTS.DAT`) are the rune bag, the quiver, the map case and the bowl. `ItemFitsSlot` still knows UW2's mask numbering, and a mask it has no case for (UW2's key ring, 0x204) refuses everything; no UW1 container has one.
- The bowl's rule (`CONT_FOOD`) takes the whole food class 0x0B, drinks and the two potions included, plus the plants, the candle, leeches, rotworm stew and the dead rotworm.
- `DamageInventory` names a destroyed object `ITEM_FIST` when it is the player object itself (`obj == ThePlayer`), which an object in a slot never is: inherited from UW2, with no effect in UW1.

## Recovered rules

- Stacks merge while their total stays below 999; sling stones, bolts and arrows merge whatever their quality, other items only within the same quality band (quality / 16) and with neither ruined unless both are; keys only with the same lock. The merged quality is the average of the two.
- A light burns only in the shoulder and hand slots; put anywhere else it goes out.
- The carried weight is in tenths of a stone; the panel shows (max_weight - weight) / 10.
- `PLAYER.DAT` holds the player record, then the count of saved objects plus one, then the workspace copy: the player object, the cursor object, the 28 slots and the saved objects, 8 bytes each, numbered from 1.

## Dead code

- `FindEmptySlot` in [inv/INVDATA.C](../../src/inv/INVDATA.C): nothing calls it.
- `EndInventory` in [inv/INVPANEL.C](../../src/inv/INVPANEL.C) is empty, as in UW2.
