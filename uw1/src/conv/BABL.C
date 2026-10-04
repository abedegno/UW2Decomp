/* target: ovr093 */
/* opts: -mm -1 -G -O -Y -d */
/* The conversation interpreter ("babl"). A conversation is a compiled script in
   DATA\CNV.ARK, one block per conversation slot (cnv_id: the NPC's whoami, or 0x100 plus
   the creature class for whoami 0). This file loads it, runs it on a small stack machine,
   and binds the script's imports, by name, to game variables and built-in functions.
   The whole of UW1's DOS overlay ovr093 (UW2's ovr095), in original order.

   Entry points: init_babl (a new game: copies DATA\babglobs.dat to SAVE0\bglobals.dat),
   load_script (Converse: opens the archive it names, reads the block, the header and
   import table, the code, the conversation's saved globals, and binds the string
   built-ins here), bab_fun, bab_var and bab_var_out (bind a built-in, write and read an
   imported variable), babl_run (runs the script to its end and saves its globals back),
   convert_string (the @-variable substitution in conversation text), getmem, getmem_addr
   and babl_setmem (the built-ins' access to script memory), and the script heap
   bab_malloc, bab_free and bab_realloc.

   Machine: code is an array of words; mem is babl_nvars words of variables (imports,
   the conversation's private globals) followed by a stack of 0x800 words; stack points
   at the stack's start, sp and bp index it, pc indexes code, reg is the result register.
   Variable addresses are indexes into mem, and PUSHI_EFF turns a frame offset into one.
   Opcodes are listed in opcode_text and decoded in babl_run; built-ins are called by
   CALLI through funcs[], with a pointer to the top of the stack (see talk_calli). Strings
   are string ids: below 0x200 they are the conversation's own block in STRINGS.PAK
   (get_string reads block 0 as CutsceneOrConversationStringBlock, set here from the
   header), and strings a script builds are made in block STRBLK_DYNAMIC (0x7C), cleared
   when the script ends.

   The heap: bab_malloc carves blocks out of the work area load_script is given, taken as
   one free block of 0xFFFF bytes, first fit, each block 8 bytes of header (size, next)
   and a 4-byte tag at the end pointing back at its data.

   UW1 against UW2: the heap's free block is 0xFFFF bytes (UW2 0xFBFF); bab_free checks
   the tag against the block's header rather than its data, so the check never passes and
   nothing is ever freed (and its merging loop, which would not end, never runs);
   bab_realloc only shrinks in place, without a new tag, and to grow allocates without
   copying; convert_string never grows its buffer (no add_to); the bglobals.dat files are
   opened with open() on built paths (DATA\, SAVE0\), not our_open; the archive is
   opened through ovr091 with a 12-byte handle on the stack, and the block buffer is
   0x4000 bytes. unbound looks the current built-in up in the import table and does
   nothing with what it finds. UW1 still has the text-mode debugging versions of the
   conversation built-ins (ovr093_9F8, ovr093_A7D, ovr093_ADB, ovr093_BE7, ovr093_E4B,
   ovr093_E61): print, ask, menu, fmenu, say and respond on the console through scanf
   and dprintf. Nothing binds them, and they are static (no overlay stub entry).

   Name: UW2's (map/filenames.tsv: the conversation interpreter, init_babl, babl_run
   and the bab_ prefix). */
/* name: The function names are UW2's (the FM Towns originals, or UW2Decomp's
   provisional names for FM Towns statics), the routines being the same; the debugging
   built-ins UW2 does not have keep the listing's names. */
/* match: UW2's provisional names reproduce UW1's stub order too: Turbo C lists a file's
   publics by the tools/bssorder.py key of each name and TLINK numbers overlay stub
   entries from the last one listed (checked against the EXE's 53 stub entries). */

#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "conv.h"
#include "file.h"
#include "gfx.h"
#include "sys.h"
#include "ui.h"

/* UW1: declarations the headers do not have (or have in UW2's form). */
char far open_arc(char far *arc, char *name);          /* opens an archive into arc */
char far close_arc(char far *arc);                    /* closes it */
int far get_arc(char far *arc, int block, void far *buf);
void far dprintf(char *fmt, ...);
char far * far seg039_3495_85A(char far *s);          /* UW2's seg039_3452_857 */
int far seg039_3495_89D(char far *s);                 /* UW2's seg039_3452_89A */

/* A block of the script heap: its size in bytes (header and tag included) and, while
   free, the next free block. An allocated block points at itself and ends with a tag,
   a far pointer to its own data, which bab_free checks before releasing it. */
HOST_LAYOUT_BEGIN
struct BabBlock {
    int32 size;
    struct BabBlock far *next;
};
HOST_LAYOUT_END

/* One entry of the script's import table, 32 bytes. */
struct BablImport {
    char name[24];
    int16 count;                        /* 0x18, array length; 0 ends the table */
    int16 index;                        /* 0x1A, memory address or function number */
    int16 type;                         /* 0x1C, the value's type */
    int16 kind;                         /* 0x1E, 0x111 for a function */
};

/* This file's _BSS: the interpreter's state (see the header). */
/* name: arc_buffer is public in the FM Towns build; the rest are statics there, so they
   have no original names. */
/* match: The names were chosen with tools/bssorder.py so that Turbo C lays them out
   where the EXE has them (DS:4826..485D). */
char far *arc_buffer;
static struct BabBlock far *free_list;
static int16 sp;
static int16 far *stack;
static struct BablImport far *babl_import_table;
static int16 far *mem;
static int16 reg;
static int32 code_size;
static void (far * far *funcs)();
static int16 hdr_word;
static int16 stack_base;
static char *file_name;
static int16 func_count;
/* UW1: a word nothing reads or writes (DS:484E). */
/* name: chosen for layout (tools/bssorder.py puts it between func_count and pc). */
static int16 word_484E;
static int16 pc;
static int16 babl_nvars;
static int16 far *code;
static int16 current_func;
static int16 empty_string;
static int16 bp;

/* The names of the opcodes, indexed by opcode, unused by the code. UW1's data has a
   zero word after the 42 pointers (DS:B9E), which nothing reads: written as a null
   43rd entry, which gives the same bytes as a separate zero int would. */
/* name: FM Towns calls the table opcode_text. */
char *opcode_text[] = {
    "NOP", "OPADD", "OPMUL", "OPSUB", "OPDIV", "OPMOD", "OPOR", "OPAND", "OPNOT",
    "TSTGT", "TSTGE", "TSTLT", "TSTLE", "TSTEQ", "TSTNE", "JMP", "BEQ", "BNE", "BRA",
    "CALL", "CALLI", "RET", "PUSHI", "PUSHI_EFF", "POP", "SWAP", "PUSHBP", "POPBP",
    "SPTOBP", "BPTOSP", "ADDSP", "FETCHM", "STO", "OFFSET", "START", "SAVE_REG",
    "PUSH_REG", "STRCMP", "EXIT_OP", "SAY_OP", "RESPOND_OP", "OPNEG", 0
};

/* 40h bytes, far, so its own segment (58DE:0000 in UW1); load_script clears the first
   byte and nothing else here touches it. */
/* name: The byte in a segment of its own is a static in the FM Towns build: FM Towns'
   load_script_ stores to it unnamed, so the name is provisional. */
static char far seg066_0[0x40];

/* The whole work area starts as one free block of 0xFFFF bytes. */
/* name: A static in the FM Towns build. */
static void far ovr093_0(char far *work)
{
    free_list = (struct BabBlock far *)work;
    free_list->size = 0xffffL;
    free_list->next = 0;
}

/* First-fit allocation from the script heap: n is rounded up to 4 and 12 added for the
   header and tag; a free block more than 16 bytes larger is split. Returns 0 when nothing
   fits (callers mostly call pfatal_code(4) then). */
char far * far bab_malloc(int32 n)
{
    char far *result;
    struct BabBlock far *current;
    struct BabBlock far *previous;
    struct BabBlock far *split;
    struct BabBlock far *tail;
    n = ((n + 3) & -4L) + 12;
    current = free_list;
    previous = 0;
    result = 0;
    while (current != 0) {
        if (current->size >= n) {
            if (current->size - n > 16) {
                split = (struct BabBlock far *)((char far *)current + n);
                split->next = current->next;
                split->size = current->size - n;
                current->size = n;
                if (previous) previous->next = split;
                else free_list = split;
            } else {
                if (previous) previous->next = current->next;
                else free_list = current->next;
            }
            current->next = current;
            tail = current;
            result = (char far *)current + 8;
            TAG_SLOT(tail, (int)(current->size / 4) - 1) = TAG_VAL(result);
            break;
        }
        previous = current;
        current = current->next;
    }
    return result;
}

/* Release a block, keeping the free list in address order and merging neighbours. A
   pointer without a valid tag is ignored, and in UW1 that is every pointer: the tag
   holds the block's data address (block + 8), but it is compared with the block's
   header address. The rest therefore never runs, which is as well: it adds sizes to
   struct pointers (scaling them by 8), and its loop, which never advances after a
   merge, would not end. */
void far bab_free(char far *data)
{
    struct BabBlock far *block;
    struct BabBlock far *previous;
    struct BabBlock far *current;
    char far *tail;
    block = (struct BabBlock far *)(data - 8);
    data = (char far *)block;
    tail = data;
    if (TAG_SLOT(tail, (*(uint16 far *)block >> 2) - 1) == TAG_VAL(data) &&
        block->next == (struct BabBlock far *)data) {
        current = free_list;
        previous = 0;
        while (current != 0) {
            if ((UNEARPTR)current->next > (UNEARPTR)block || current->next == 0) {
                if (previous != 0) {
                    if (previous + previous->size == block) {
                        previous->size += block->size;
                        block = previous;
                    } else {
                        block->next = previous->next;
                        previous->next = block;
                    }
                }
                if (block + block->size == current) {
                    block->next = current->next;
                    block->size += current->size;
                }
            } else {
                previous = current;
                current = current->next;
            }
        }
        if (free_list == 0) {
            free_list = block;
            block->next = 0;
        }
    }
}

/* Shrink a block in place (splitting off the tail onto the free list when more than 16
   bytes are left over; the block keeps its old size and tag, and with an empty free
   list the split reads next through a null pointer), or "grow" it by allocating a new
   block and freeing the old, without copying anything. */
char far * far bab_realloc(char far *p, int32 n)
{
    struct BabBlock far *block;
    struct BabBlock far *split;
    struct BabBlock far *current;
    char far *result;
    int32 original;

    original = n;
    n = ((n + 3) & -4L) + 12;
    block = (struct BabBlock far *)(p - 8);
    p = (char far *)block;
    if (block->size > n) {
        current = free_list;
        result = 0;
        if (current == 0) {
            result = p + 8;
            if (block->size - n > 16) {
                split = (struct BabBlock far *)(p + n);
                split->next = current->next;
                split->size = block->size - n;
                free_list = split;
                return result;
            }
            return result;
        } else {
            while (current != 0) {
                if ((UNEARPTR)current->next > (UNEARPTR)block || current->next == 0) {
                    result = p + 8;
                    if (block->size - n > 16) {
                        split = (struct BabBlock far *)(p + n);
                        split->next = current->next;
                        split->size = block->size - n;
                        current->next = split;
                        return result;
                    }
                    return result;
                }
                current = current->next;
            }
        }
    } else {
        result = bab_malloc(original);
        bab_free(p);
    }
    return result;
}

/* Copies DATA\babglobs.dat, which lists each conversation's slot and number of private
   globals, to SAVE0\bglobals.dat with every global zeroed (a new game). Returns 0, or an
   error code: ERR_READ | 7 (0x3007) when babglobs.dat cannot be opened, ERR_WRITE | 1
   (0x4001) when bglobals.dat cannot be written. Each record is the 4 bytes read into block
   and, by the stack layout, size (READ_PAIR and WRITE_PAIR, portable.h, give the host the
   same two words). */
int far init_babl(void)
{
    char good;
    int size, block;
    char path[80];
    register int source;
    register int dest;
    good = 1;
    strcpy(path, "DATA\\");
    strcat(path, "babglobs.dat");
    source = open(path, O_RDONLY | O_BINARY);
    if (source < 0)
        return ERR_READ | 7;
    strcpy(path, "SAVE0\\");
    strcat(path, "bglobals.dat");
    dest = open(path, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0x180);
    if (dest >= 0) {
        mem_set(stdat, 0, 0x1000);
        while (good && READ_PAIR(source, block, size) == 4) {
            if (WRITE_PAIR(dest, block, size) != 4)
                good = 0;
            else
                good = FarWrite_ovr167_627(dest, stdat, size << 1) == size << 1;
        }
        close(source);
        close(dest);
        if (good) return 0;
    }
    return ERR_WRITE | 1;
}

/* Read conversation cnv_id's private globals from SAVE0\bglobals.dat into the start of
   script memory (at most count words). Records are in slot order, so the search stops at a
   higher slot. */
void far bab_get_globals(int16 far *memory, int count)
{
    int size;
    unsigned block;
    char path[80];
    register int handle;
    register int done;
    strcpy(path, "SAVE0\\");
    strcat(path, "bglobals.dat");
    handle = open(path, O_RDONLY | O_BINARY);
    if (handle >= 0) {
        done = 0;
        while (!done) {
            if (READ_PAIR(handle, block, size) < 4 || block > cnv_id) {
                done = 1;
                break;
            }
            if (block == cnv_id) {
                if (count < size) size = count;
                if (intoFarBuffer_ovr167_5DA(handle, memory, size << 1) <
                    (unsigned)(size << 1)) done = 1;
            } else
                lseek(handle, (uint32)(unsigned)(size << 1), 1);
        }
        close(handle);
    }
}

/* Write them back over the same record when the script ends. */
void far bab_put_globals(int16 far *memory, int count)
{
    int size;
    unsigned block;
    char path[80];
    register int handle;
    register int done;
    strcpy(path, "SAVE0\\");
    strcat(path, "bglobals.dat");
    handle = open(path, O_RDWR | O_BINARY);
    if (handle >= 0) {
        done = 0;
        while (!done) {
            if (READ_PAIR(handle, block, size) < 4 || block > cnv_id) {
                done = 1;
                break;
            }
            if (block == cnv_id) {
                if (count < size) size = count;
                FarWrite_ovr167_627(handle, memory, size << 1);
                done = 1;
            } else
                lseek(handle, (uint32)(unsigned)(size << 1), 1);
        }
        close(handle);
    }
}

/* Load conversation cnv_id: the heap starts afresh in work, the archive name names is
   opened (ovr091, into a 12-byte handle) and the conversation's block read into a
   0x4000-byte buffer, the header and code are copied out, script memory is allocated
   (babl_nvars + 0x800 words), the private globals read in, imported variables cleared
   and the string built-ins bound. Returns 1, or -1 when the header is rejected (never,
   as DoReadHeader always returns 1); an empty block prints 'You get no response.' and
   also returns 1. */
int far load_script(char *name, char far *work)
{
    char far *empty;
    char far *buffer;
    char arc[12];
    register int size;
    ovr093_0(work);
    seg066_0[0] = 0;
    file_name = name;
    if (open_arc(arc, name)) {
        if ((arc_buffer = bab_malloc(0x4000L)) == 0)
            pfatal_code(4);
        buffer = arc_buffer;
        size = get_arc(arc, cnv_id, arc_buffer);
        close_arc(arc);
        if (size <= 0) {
            scroll_print(get_string(0xe01));  /* "You get no response.\n" */
            return 1;
        }
    } else
        pfatal_code(ERR_READ | 0xa);
    if (DoReadHeader_ovr095_12C3() < 0) return -1;
    DoCopyCode_ovr095_14B1();
    bab_free(buffer);
    mem = (int16 far *)bab_malloc((int32)((babl_nvars + 0x800) * sizeof(int16)));
    bab_get_globals(mem, babl_nvars);
    stack_base = babl_nvars;
    stack = mem + stack_base;
    empty = bab_malloc(1L);
    *empty = 0;
    empty_string = make_string(empty, STRBLK_DYNAMIC);
    bab_var_clear();
    bab_fun("compare", (void (far *)())bab_compare_ovr095_A8B);
    bab_fun("random", (void (far *)())BabRand_ovr095_A5B);
    bab_fun("plural", (void (far *)())babPluralize_ovr095_B95);
    bab_fun("contains", (void (far *)())StringContains_ovr095_BDB);
    bab_fun("append", (void (far *)())babl_str_append_ovr095_D0A);
    bab_fun("copy", (void (far *)())STRING_COPY_ovr095_DC7);
    bab_fun("find", (void (far *)())conv_find_ovr095_E36);
    bab_fun("length", (void (far *)())conv_length_ovr095_E8D);
    bab_fun("val", (void (far *)())DoVal_ovr095_EB2);
    return 1;
}

/* What an import the game never bound calls: every funcs[] entry starts as this. UW1
   looks the built-in being called (current_func) up in the import table and, when it
   is there, sets a local nothing reads: what was done with the entry has been compiled
   out (a debugging message, presumably). */
int far unbound(void)
{
    struct BablImport far *entry;
    int unused;
    register int found = 0;
    entry = babl_import_table;
    while (entry->count != 0) {
        if (entry->index == current_func) {
            found = 1;
            break;
        }
        entry++;
    }
    if (!found)
        return 0;
    unused = 0;
    return 0;
}

/* Text-mode debugging "print": substitutes arg1's string and frees the result; the
   printing itself has been compiled out. Static and unbound, as are the other debugging
   built-ins below. */
static void far ovr093_9F8(int16 far *args)
{
    char far *str;
    char far *conv;
    str = get_string(getmem(args[-1]));
    conv = convert_string(str);
    if (conv != str) bab_free(conv);
}

/* The script's "random": 1 .. arg1, from rand(). The string built-ins below read their
   arguments as the built-ins in CONVERSE.C do: args[-1] holds the address of arg1. */
int far BabRand_ovr095_A5B(int16 far *args)
{
    return (int)(((int32)rand() *
        getmem(args[-1])) / 0x8000L) + 1;
}

/* Debugging "ask": reads a word from the console into a new dynamic string, whose id
   is left in AX. */
static int far ovr093_A7D(void)
{
    char far *out;
    char buf[80];
    int len;
    register int id;
    scanf("%s", buf);
    len = strlen(buf);
    out = bab_malloc((uint32)(unsigned)(len + 1));
    str_copy(out, buf);
    id = make_string(out, STRBLK_DYNAMIC);
    AX_RESULT(id);
}

/* Debugging "menu": prints the strings of the 0-terminated id list at arg1, numbered
   from 1, and returns the number typed. */
static int far ovr093_ADB(int16 far *args)
{
    int choice;
    int start;
    char far *str[20];
    char far *conv[20];
    char buf[160];
    register int i;
    register int id;
    i = 1;
    start = args[-1];
    id = getmem(start);
    while (id != 0) {
        str[i] = get_string(id);
        conv[i] = convert_string(str[i]);
        i++;
        id = getmem(start + i - 1);
    }
    for (choice = 1; choice < i; choice++) {
        str_copy(buf, conv[choice]);
        dprintf("%d  %s\n", choice, buf);
        if (str[choice] != conv[choice]) bab_free(conv[choice]);
    }
    dprintf("Type an item number\n");
    scanf("%d", &choice);
    return choice;
}

/* Debugging "fmenu": as the menu, but only the entries whose flag in the list at arg2
   is set, returning the chosen entry's string id. */
static int far ovr093_BE7(int16 far *args)
{
    int choice;
    int start;
    int flags;
    int id;
    int flag;
    int ids[20];
    char far *str[20];
    char far *conv[20];
    char buf[160];
    register int n;
    register int j;
    j = 1;
    n = 1;
    start = args[-1];
    flags = args[-2];
    id = getmem(start);
    flag = getmem(flags);
    while (id != 0) {
        if (flag != 0) {
            str[n] = get_string(id);
            conv[n] = convert_string(str[n]);
            ids[n] = id;
            n++;
        }
        j++;
        id = getmem(start + j - 1);
        flag = getmem(flags + j - 1);
    }
    for (choice = 1; choice < n; choice++) {
        str_copy(buf, conv[choice]);
        dprintf("%d  %s\n", choice, buf);
        if (str[choice] != conv[choice]) bab_free(conv[choice]);
    }
    dprintf("Type an item number\n");
    scanf("%d", &choice);
    return ids[choice];
}

/* The script's "compare": 1 when two strings are equal after @-substitution, ignoring
   case. */
int far bab_compare_ovr095_A8B(int16 far *args)
{
    char far *str1;
    char far *str2;
    char far *conv2;
    char far *conv1;
    char a[256];
    char b[256];
    register int result;
    str1 = get_string(getmem(args[-1]));
    conv1 = convert_string(str1);
    str2 = get_string(getmem(args[-2]));
    conv2 = convert_string(str2);
    str_copy(a, conv2);
    str_copy(b, conv1);
    seg039_3495_85A(a);
    seg039_3495_85A(b);
    result = str_cmp(a, b);
    if (str2 != conv2) bab_free(conv2);
    if (str1 != conv1) bab_free(conv1);
    return result == 0;
}

/* Debugging "say" and "respond": the text on the console. */
static void far ovr093_E4B(char far *text)
{
    dprintf("NPC says - %Fs\n", text);
}

static void far ovr093_E61(char far *text)
{
    char buf[800];
    str_copy(buf, text);
    dprintf("PLAYER says - %s\n", buf);
}

/* The script's "plural": args[-2]'s string id when the count at args[-3] is 1 or less,
   args[-1]'s when it is more. */
int far babPluralize_ovr095_B95(int16 far *args)
{
    int count;
    register int plural;
    register int singular;
    count = getmem(args[-3]);
    singular = getmem(args[-2]);
    plural = getmem(args[-1]);
    if (count > 1) return plural;
    return singular;
}

/* The script's "contains": does arg1's string contain arg2's as a whole word (bounded
   by the ends, white space or punctuation)? It lowercases the two source strings in
   place, but searches the @-substituted copies, so case is only ignored when no
   substitution happened; the copies are never freed (they live until the heap is reset
   by the next load_script). */
int far StringContains_ovr095_BDB(int16 far *args)
{
    char far *str1;
    char far *str2;
    char far *conv2;
    char far *conv1;
    char far *p;
    register int len;
    str1 = get_string(getmem(args[-1]));
    conv1 = convert_string(str1);
    str2 = get_string(getmem(args[-2]));
    conv2 = convert_string(str2);
    seg039_3495_85A(str2);
    seg039_3495_85A(str1);
    for (p = conv1; (p = str_str(p, conv2)) != 0; p += len) {
        len = str_len(conv2);
        if (p != 0 && (p == conv1 || isspace(p[-1]) || ispunct(p[-1])))
            if (p[len] == 0 || isspace(p[len]) || ispunct(p[len]))
                return 1;
    }
    return 0;
}

/* The script's "append": a new dynamic string, arg2's string followed by arg1's. */
int far babl_str_append_ovr095_D0A(int16 far *args)
{
    int len1, length;
    char far *s1, far *s2, far *out;
    register int len2;
    register int id;
    s1 = get_string(getmem(args[-1]));
    s2 = get_string(getmem(args[-2]));
    len2 = str_len(s2);
    len1 = str_len(s1);
    length = len2 + len1 + 1;
    out = bab_malloc((uint32)(unsigned)length);
    str_copy(out, s2);
    str_copy(out + len2, s1);
    id = make_string(out, STRBLK_DYNAMIC);
    AX_RESULT(id);
}

/* The script's "copy": a new dynamic string holding a copy of arg1's. */
int far STRING_COPY_ovr095_DC7(int16 far *args)
{
    char far *source, far *out;
    int length;
    register int id;
    source = get_string(getmem(args[-1]));
    length = str_len(source) + 1;
    out = bab_malloc((uint32)(unsigned)length);
    str_copy(out, source);
    id = make_string(out, STRBLK_DYNAMIC);
    AX_RESULT(id);
}

/* The script's "find": the 1-based position of value arg1 in the arg2 words of the
   array at arg3 (passed by address, not read through), or 0. */
int far conv_find_ovr095_E36(int16 far *args)
{
    int value, count;
    register int i;
    register int start;
    value = getmem(args[-1]);
    count = getmem(args[-2]);
    start = args[-3];
    for (i = 0; i < count; i++) {
        if (getmem(start + i) == value)
            return i + 1;
    }
    return 0;
}

/* The script's "length" and "val": a string's length, and its value as a decimal
   number. */
int far conv_length_ovr095_E8D(int16 far *args)
{
    return str_len(get_string(getmem(args[-1])));
}

int far DoVal_ovr095_EB2(int16 far *args)
{
    return seg039_3495_89D(get_string(getmem(args[-1])));
}

/* Replaces each @-variable in a conversation string with its value: @ then the kind
   (G a global, P a parameter, S a stack slot, C a constant), then for all but C the
   type (I an integer, otherwise a string id), then the address, optionally followed
   by another variable giving an index. "@@" is a literal @.

   G reads mem[number + index - 1]; S reads stack[bp + number + index - 1], a local of
   the current frame; P reads stack[bp + number], which holds an address (a parameter
   passed by reference), and reads mem at that address + index - 1. A string value is
   substituted recursively. Returns text itself when it has no @, else a new heap string
   the caller frees. UW1 allocates twice the text's length plus 0x80 bytes once and never
   grows the buffer (UW2's add_to enlarges it as needed). */
char far * far convert_string(char far *text)
{
    char far *s;
    char far *buffer;
    char far *out;
    char kind;
    char type;
    char far *conv;
    char far *str;
    int number;
    char digits[20];
    register int value;
    register int extra;
    if (FindStringDelimiter(text, '@') == 0)
        return text;
    buffer = bab_malloc((uint32)(unsigned)((str_len(text) + 0x40) * 2));
    s = text;
    out = FARNULLTRAP(buffer);  /* not checked: 0 when the heap is full */
    while (*s != 0) {
        if (*s == '@') {
            s++;
            if (*s != '@') {
                kind = *s++;
                if (kind != 'C')
                    type = *s++;
                else
                    type = 'I';
                str_ncopy(digits, s, 19);
                number = atoi(digits);
                while (*s != 0) {
                    if (isdigit(*s) || *s == '-') s++;
                    else break;
                }
                if (*s == 'G' || *s == 'S' || *s == 'P' || *s == 'C')
                    extra = AtIndex_ovr095_11C0(&s) - 1;
                else
                    extra = 0;
                if (kind == 'G')
                    value = getmem(number + extra);
                else if (kind == 'P')
                    value = getmem(
                        conv_local_ovr095_2030(number) + extra);
                else if (kind == 'S')
                    value = conv_local_ovr095_2030(number + extra);
                else
                    value = number;
                if (type == 'I') {
                    itoa(value, digits, 10);
                    str_copy(out, digits);
                } else {
                    str = get_string(value);
                    if (str != 0) {
                        conv = convert_string(str);
                        str_copy(out, conv);
                        if (str != conv) bab_free(conv);
                    }
                }
                out = buffer + str_len(buffer);
                continue;
            }
        }
        *out++ = *s++;
    }
    *out = 0;
    buffer = bab_realloc(buffer, str_len(buffer) + 1);
    return buffer;
}

/* Reads the index part of an @-variable, which is itself a variable (only an integer
   one, else 0), and advances *s past it. */
int far AtIndex_ovr095_11C0(register char far **s)
{
    char kind, type;
    int parsed, extra;
    char buf[20];
    register int result;
    kind = *(*s)++;
    type = 'I';
    if (kind != 'C') type = *(*s)++;
    if (type != 'I') return 0;
    str_ncopy(buf, *s, 19);
    parsed = atoi(buf);
    while (**s != 0) {
        if (isdigit(**s) || **s == '-') (*s)++;
        else break;
    }
    if (**s == 'G' || **s == 'S' || **s == 'P' || **s == 'C')
        extra = AtIndex_ovr095_11C0(s) - 1;
    else extra = 0;
    if (kind == 'G')
        result = getmem(parsed + extra);
    else if (kind == 'P')
        result = getmem(
            conv_local_ovr095_2030(parsed) + extra);
    else if (kind == 'S')
        result = conv_local_ovr095_2030(parsed + extra);
    else result = parsed;
    return result;
}

/* Reads the script header and its import table from arc_buffer: a long read into a
   local and not used (UW2 skips it), the
   code size in words (a long), a word kept in hdr_word, the string block, babl_nvars,
   the number of imports, then each import as a length-prefixed name followed by the
   words index (variable address or function number), count, kind (0x111 a function,
   else a variable; UW-Formats gives 0x10F) and type. Leaves arc_buffer at the code and
   points every function slot at unbound. Always returns 1. */
int far DoReadHeader_ovr095_12C3(void)
{
    int nimports;
    int index;
    int count;
    int kind;
    int type;
    int32 first;
    char name[64];
    register int i;
    register int len;
    first = *(int32 far *)arc_buffer;
    arc_buffer += 4;
    code_size = *(int32 far *)arc_buffer;
    arc_buffer += 4;
    code = (int16 far *)bab_malloc(code_size << 1);
    hdr_word = *(int16 far *)arc_buffer;
    arc_buffer += 2;
    CutsceneOrConversationStringBlock = *(int16 far *)arc_buffer;
    arc_buffer += 2;
    babl_nvars = *(int16 far *)arc_buffer;
    arc_buffer += 2;
    nimports = *(int16 far *)arc_buffer;
    arc_buffer += 2;
    func_count = 0;
    babl_import_table = (struct BablImport far *)bab_malloc(
        (int32)((nimports + 1) * sizeof(struct BablImport)));
    for (i = 0; i < nimports; i++) {
        len = *(int16 far *)arc_buffer;
        arc_buffer += 2;
        FAR_COPY(name, arc_buffer, len);
        arc_buffer += len;
        name[len] = 0;
        index = *(int16 far *)arc_buffer;
        arc_buffer += 2;
        count = *(int16 far *)arc_buffer;
        arc_buffer += 2;
        kind = *(int16 far *)arc_buffer;
        arc_buffer += 2;
        type = *(int16 far *)arc_buffer;
        arc_buffer += 2;
        str_copy(babl_import_table[i].name, name);
        babl_import_table[i].index = index;
        babl_import_table[i].count = count;
        babl_import_table[i].type = type;
        babl_import_table[i].kind = kind;
        if (kind == 0x111) func_count++;
    }
    babl_import_table[i].name[0] = 0;
    babl_import_table[i].count = 0;
    babl_import_table[i].index = 0;
    if (func_count > 0)
        funcs = HOST_TABLE((void (far * far *)())bab_malloc(
            (int32)(func_count * DOS_SIZEOF(void (far *)(), 4))), func_count);
    for (i = 0; i < func_count; i++)
        funcs[i] = (void (far *)())unbound;
    return 1;
}

void far DoCopyCode_ovr095_14B1(void)
{
    FAR_COPY(code, arc_buffer, (unsigned)code_size << 1);
}

/* At the end of a script: drop the dynamic strings, save the private globals. */
static void far ExitConversation_ovr095_14D4(void)
{
    clear_dynamics(STRBLK_DYNAMIC);
    bab_put_globals(mem, babl_nvars);
}

/* Run the loaded script from word 0, which must be START (0x22, else -1). It stops at
   EXIT_OP, at a RET with nothing on the stack, or at an unknown opcode, then saves the
   globals and returns 1. Branch operands (BEQ, BNE, BRA) are relative to the operand
   word; JMP and CALL take absolute word addresses. Nothing checks the stack or pc
   bounds. */
int far babl_run(void)
{
    register int running;
    register int tmp;
    sp = 0;
    bp = 0;
    pc = 0;
    if (code[pc] != 0x22) return -1;
    running = 1;
    while (running) {
        switch (code[pc]) {
        case 0x00:      /* NOP */
            pc++;
            break;
        case 0x01:
            vmAdd_ovr095_18CF();
            pc++;
            break;
        case 0x02:
            MUL_OPCODE_ovr095_192C();
            pc++;
            break;
        case 0x03:
            subOpcode_ovr095_1965();
            pc++;
            break;
        case 0x04:
            BABL_DIV_ovr095_199E();
            pc++;
            break;
        case 0x05:
            BablMod_ovr095_19EE();
            pc++;
            break;
        case 0x29:
            babl_neg_ovr095_1908();
            pc++;
            break;
        case 0x0F:      /* JMP */
            pc = code[pc + 1];
            break;
        case 0x12:      /* BRA */
            pc = code[pc + 1] + pc + 1;
            break;
        case 0x06:
            babBitOr_ovr095_1A3E();
            pc++;
            break;
        case 0x07:
            babl_and_ovr095_1A83();
            pc++;
            break;
        case 0x08:      /* OPNOT */
            stack[sp] = !stack[sp];
            pc++;
            break;
        case 0x09:
            VM_GT_ovr095_1AC8();
            pc++;
            break;
        case 0x0A:
            vmTstge_ovr095_1B0A();
            pc++;
            break;
        case 0x0B:
            vmTstlt_ovr095_1B4C();
            pc++;
            break;
        case 0x0C:
            VmTstle_ovr095_1B8E();
            pc++;
            break;
        case 0x0D:
            ExecTsteq_ovr095_1BD0();
            pc++;
            break;
        case 0x0E:
            opcode_tstne_ovr095_1C12();
            pc++;
            break;
        case 0x10:      /* BEQ */
            if (stack[sp--] == 0)
                pc = code[pc + 1] + pc + 1;
            else
                pc += 2;
            break;
        case 0x13:
            babl_call_ovr095_1C54();
            break;
        case 0x15:
            running = bab_ret_ovr095_1C82();
            break;
        case 0x11:      /* BNE */
            if (stack[sp--] != 0)
                pc = code[pc + 1] + pc + 1;
            else
                pc += 2;
            break;
        case 0x18:      /* POP */
            sp--;
            pc++;
            break;
        case 0x1F:
            exec_fetchm_ovr095_1CAA();
            pc++;
            break;
        case 0x21:
            vmOffset_ovr095_1CD7();
            pc++;
            break;
        case 0x22:      /* START */
            pc++;
            break;
        case 0x19:      /* SWAP */
            tmp = stack[sp];
            stack[sp] = stack[sp - 1];
            stack[sp - 1] = tmp;
            pc++;
            break;
        case 0x1A:      /* PUSHBP */
            sp++;
            stack[sp] = bp;
            pc++;
            break;
        case 0x1B:      /* POPBP */
            bp = stack[sp];
            sp--;
            pc++;
            break;
        case 0x1C:      /* SPTOBP */
            bp = sp;
            pc++;
            break;
        case 0x1D:      /* BPTOSP */
            sp = bp;
            pc++;
            break;
        case 0x1E:      /* ADDSP: pop n and reserve n words */
            sp += stack[sp] - 1;
            pc++;
            break;
        case 0x16:      /* PUSHI */
            sp++;
            stack[sp] = code[pc + 1];
            pc += 2;
            break;
        case 0x20:
            BABL_STORE_ovr095_1D11();
            pc++;
            break;
        case 0x17:      /* PUSHI_EFF: the mem address of frame slot bp + n */
            sp++;
            stack[sp] = stack_base + code[pc + 1] + bp;
            pc += 2;
            break;
        case 0x14:      /* CALLI */
            talk_calli_ovr095_1D44();
            break;
        case 0x23:      /* SAVE_REG */
            reg = stack[sp];
            pc++;
            break;
        case 0x24:      /* PUSH_REG */
            sp++;
            stack[sp] = reg;
            pc++;
            break;
        case 0x25:
            vm_strcmp_ovr095_1DC1();
            pc++;
            break;
        case 0x26:      /* EXIT_OP */
            running = 0;
            break;
        case 0x27:
            vmSay_ovr095_1EA2();
            pc++;
            break;
        case 0x28:
            babl_respond_ovr095_1F4A();
            pc++;
            break;
        default:
            running = 0;
        }
    }
    ExitConversation_ovr095_14D4();
    return 1;
}

void far vmAdd_ovr095_18CF(void)
{
    int result = stack[sp] + stack[sp - 1];
    sp--;
    stack[sp] = result;
}
void far babl_neg_ovr095_1908(void)
{
    stack[sp] = -stack[sp];
}
void far MUL_OPCODE_ovr095_192C(void)
{
    int result = stack[sp] * stack[sp - 1];
    sp--;
    stack[sp] = result;
}
void far subOpcode_ovr095_1965(void)
{
    int result = stack[sp - 1] - stack[sp];
    sp--;
    stack[sp] = result;
}
/* Division and remainder by zero give 0x7FFF. */
void far BABL_DIV_ovr095_199E(void)
{
    int result;
    if (stack[sp] != 0)
        result = stack[sp - 1] / stack[sp];
    else result = 0x7fff;
    sp--;
    stack[sp] = result;
}
void far BablMod_ovr095_19EE(void)
{
    int result;
    if (stack[sp] != 0)
        result = stack[sp - 1] % stack[sp];
    else result = 0x7fff;
    sp--;
    stack[sp] = result;
}
/* OPOR and OPAND are logical, giving 0 or 1. */
void far babBitOr_ovr095_1A3E(void)
{
    int result = stack[sp - 1] || stack[sp];
    sp--;
    stack[sp] = result;
}
void far babl_and_ovr095_1A83(void)
{
    int result = stack[sp - 1] && stack[sp];
    sp--;
    stack[sp] = result;
}
void far VM_GT_ovr095_1AC8(void)
{
    int result = stack[sp - 1] > stack[sp];
    sp--;
    stack[sp] = result;
}
void far vmTstge_ovr095_1B0A(void)
{
    int result = stack[sp - 1] >= stack[sp];
    sp--;
    stack[sp] = result;
}
void far vmTstlt_ovr095_1B4C(void)
{
    int result = stack[sp - 1] < stack[sp];
    sp--;
    stack[sp] = result;
}
void far VmTstle_ovr095_1B8E(void)
{
    int result = stack[sp - 1] <= stack[sp];
    sp--;
    stack[sp] = result;
}
void far ExecTsteq_ovr095_1BD0(void)
{
    int result = stack[sp - 1] == stack[sp];
    sp--;
    stack[sp] = result;
}
void far opcode_tstne_ovr095_1C12(void)
{
    int result = stack[sp - 1] != stack[sp];
    sp--;
    stack[sp] = result;
}

/* CALL pushes the address after its operand; RET pops it, or ends the script (0) when
   the stack is empty. */
void far babl_call_ovr095_1C54(void)
{
    stack[++sp] = pc + 2;
    pc = code[pc + 1];
}
int far bab_ret_ovr095_1C82(void)
{
    if (sp > 0) {
        pc = stack[sp];
        sp--;
        return 1;
    }
    return 0;
}
void far exec_fetchm_ovr095_1CAA(void)
{
    stack[sp] = mem[stack[sp]];
}
/* OFFSET: address + index - 1, so script arrays count from 1. */
void far vmOffset_ovr095_1CD7(void)
{
    int result = stack[sp] + stack[sp - 1] - 1;
    sp--;
    stack[sp] = result;
}
void far BABL_STORE_ovr095_1D11(void)
{
    mem[stack[sp - 1]] = stack[sp];
    sp -= 2;
}
/* CALLI n: call built-in n with a pointer to the top of the stack, where the script has
   pushed its argument count (args[0]) above the addresses of the arguments (args[-1]
   the last pushed before the count). The result replaces the count and goes into reg. */
void far talk_calli_ovr095_1D44(void)
{
    int (far *fn)(int16 far *);
    register int result;
    fn = (int (far *)(int16 far *))funcs[code[pc + 1]];
    current_func = code[pc + 1];
    result = fn(stack + sp);
    stack[sp] = result;
    reg = stack[sp];
    pc += 2;
}
/* STRCMP: pop two string ids, push 1 when they are equal after substitution (case
   counts, unlike "compare"). */
void far vm_strcmp_ovr095_1DC1(void)
{
    char far *s1;
    char far *s2;
    char far *r2;
    char far *r1;
    register int result;
    s1 = get_string(stack[sp]);
    r1 = convert_string(s1);
    s2 = get_string(stack[sp - 1]);
    r2 = convert_string(s2);
    result = str_cmp(r2, r1);
    if (r2 != s2) bab_free(r2);
    if (r1 != s1) bab_free(r1);
    sp--;
    if (result == 0)
        stack[sp] = 1;
    else
        stack[sp] = 0;
}
/* SAY_OP and RESPOND_OP: pop a string id, substitute it, and pass it to whatever is
   bound to the import "say" or "respond" (npc_say, play_respond in CONVERSE.C). */
void far vmSay_ovr095_1EA2(void)
{
    struct BablImport far *entry;
    char far *source;
    char far *text;
    source = get_string(stack[sp]);
    text = convert_string(source);
    sp--;
    entry = babl_import_table;
    while (entry->count != 0) {
        if (str_cmp("say", entry->name) == 0) {
            ((void (far *)(char far *))funcs[entry->index])(text);
            break;
        }
        entry++;
    }
    if (text != source) bab_free(text);
}
void far babl_respond_ovr095_1F4A(void)
{
    struct BablImport far *entry;
    char far *source;
    char far *text;
    source = get_string(stack[sp]);
    text = convert_string(source);
    sp--;
    entry = babl_import_table;
    while (entry->count != 0) {
        if (str_cmp("respond", entry->name) == 0) {
            ((void (far *)(char far *))funcs[entry->index])(text);
            break;
        }
        entry++;
    }
    if (text != source) bab_free(text);
}
int16 far * far getmem_addr(int addr)
{
    return mem + addr;
}
int far getmem(int addr)
{
    return mem[addr];
}
void far babl_setmem(int addr, int value)
{
    mem[addr] = value;
}
int far conv_local_ovr095_2030(int addr)
{
    return stack[bp + addr];
}
/* Bind built-in fn to the import called name; a name the script does not import is
   ignored. */
void far bab_fun(char *name, void (far *fn)())
{
    struct BablImport far *entry;
    register char *search = name;
    register int found = 0;
    entry = babl_import_table;
    while (entry->count != 0) {
        if (*search == entry->name[0] && str_cmp(search, entry->name) == 0) {
            found = 1;
            funcs[entry->index] = fn;
            break;
        }
        entry++;
    }
}
/* Copy count values into the imported variable called name (at most its declared
   count). The lowercased copy of name it builds is never used. */
void far bab_var(char *name, int16 *values, int count)
{
    struct BablImport far *entry;
    int found;
    char lower[26];
    register int i;
    register int limit = count;
    for (i = 0; i < strlen(name); i++)
        lower[i] = tolower(name[i]);
    lower[i] = 0;
    found = 0;
    entry = babl_import_table;
    while (entry->count != 0) {
        if (str_cmp(name, entry->name) == 0) {
            found = 1;
            for (i = 0; i < limit && i < entry->count; i++)
                mem[entry->index + i] = values[i];
            break;
        }
        entry++;
    }
}
void far bab_var_out(char *name, int16 *values, int count)
{
    struct BablImport far *entry;
    int found;
    register int i;
    register int limit = count;
    found = 0;
    entry = babl_import_table;
    while (entry->count != 0) {
        if (str_cmp(name, entry->name) == 0) {
            found = 1;
            for (i = 0; i < limit && i < entry->count; i++)
                values[i] = mem[entry->index + i];
            break;
        }
        entry++;
    }
}
/* Clear every imported variable: type 0x126 sets one word and 0x12B every element to 0,
   0x128 one word and 0x12A every element to the empty string, so probably int, int
   array, string and string array (inferred; UW-Formats lists 0x129 int and 0x12B
   string as return types). Other types are left alone. */
void far bab_var_clear(void)
{
    struct BablImport far *entry;
    register int i;
    entry = babl_import_table;
    while (entry->count != 0) {
        if (entry->kind == 0x111) {
            entry++;
            continue;
        }
        {
            switch (entry->type) {
            case 0x126:
                mem[entry->index] = 0;
                break;
            case 0x12b:
                for (i = 0; i < entry->count; i++)
                    mem[entry->index + i] = 0;
                break;
            case 0x128:
                mem[entry->index] = empty_string;
                break;
            case 0x12a:
                for (i = 0; i < entry->count; i++)
                    mem[entry->index + i] = empty_string;
                break;
            }
        }
        entry++;
    }
}

