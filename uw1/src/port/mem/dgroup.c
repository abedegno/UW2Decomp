/* dgroup.c: replaces the DGROUP gaps tools/extract.py places (docs/LINKING.md, "The DGROUP
   gaps"): data the link puts between two files' data, which no source defines yet, with the
   names other files use. All start as zeros but dseg_5c99_12B6 (DS:12B6), whose byte the
   game reads as 64h (GRIDDB.C); its value is read from the user's UW.EXE (port_dgroup_image)
   at start-up, so the port holds no game data. The gaps' other bytes are named by nothing. */
#include "compat.h"
#include "conv.h"
#include "inv.h"
#include "map.h"
#include "player.h"
#include "ui.h"
#include "view3d.h"
#include "port.h"

int16 CutsceneOrConversationStringBlock;        /* _BSS DS:3638..364A */
struct StringNode far *StringsPak_Address_Indices;
FILE *StringsPak_FileHandle;
int16 StringsPak_NoOfNodes;
int16 string_bits;
union Link Inventory[NUM_INV_SLOTS];            /* _BSS DS:5A92 */
uint16 dseg_5c99_7178;                          /* _BSS DS:7178 */
struct Object far *curelem;                     /* _DATA DS:0DBC */
unsigned char plyregen[2];                      /* _DATA DS:03F8 */
unsigned char realDScheck;                      /* _DATA DS:12B3 */
unsigned char dseg_5c99_12B6;                   /* _DATA DS:12B6 */

void port_dgroup_gaps(void)
{
    dseg_5c99_12B6 = port_dgroup_image[0x12B6];
}
