/* target: ovr095 */
/* opts: -mm -1 -G -O -Y -d */
/* The conversation interpreter ("babl"): the script heap (bab_malloc, bab_free,
   bab_realloc), the per-conversation globals file (init_babl, bab_get_globals,
   bab_put_globals), loading a script (load_script), the string built-ins a script can
   call, the @-variable substitution in conversation text (convert_string), the virtual
   machine itself (babl_run) and its opcodes, and the import table through which the
   game's built-in functions and variables are bound (bab_fun, bab_var, bab_var_out,
   bab_var_clear). The whole of DOS overlay ovr095, in original order.

   Function names are the originals from the FM Towns symbol table where the FM Towns
   build has the function at the same place (it keeps the string built-ins, the opcodes
   and the header reader as statics, so those keep their IDA names). */

#include <dos.h>
#include <io.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* A block of the script heap: its size in bytes (header and tag included) and, while
   free, the next free block. An allocated block points at itself and ends with a tag,
   a far pointer to its own data, which bab_free checks before releasing it. */
struct BabBlock {
    long size;
    struct BabBlock far *next;
};

/* One entry of the script's import table, 32 bytes. */
struct BablImport {
    char name[24];
    int count;                          /* 0x18, array length; 0 ends the table */
    int index;                          /* 0x1A, memory address or function number */
    int type;                           /* 0x1C, the value's type */
    int kind;                           /* 0x1E, 0x111 for a function */
};

/* This file's _BSS. arc_buffer is public in the FM Towns build; the rest are statics
   there, so they have no original names: these were chosen with tools/bssorder.py so
   that Turbo C lays them out where the EXE has them (DS:4702..4737). */
char far *arc_buffer;
static struct BabBlock far *free_list;
static int sp;
static int far *stack;
static struct BablImport far *babl_import_table;
static int far *mem;
static int reg;
static long code_size;
static void (far * far *funcs)();
static int hdr_word;
static int stack_base;
static char *file_name;
static int func_count;
static int pc;
static int babl_nvars;
static int far *code;
static int current_func;
static int empty_string;
static int bp;

/* The names of the opcodes, unused by the code; FM Towns calls the table opcode_text. */
char *opcode_text[] = {
    "NOP", "OPADD", "OPMUL", "OPSUB", "OPDIV", "OPMOD", "OPOR", "OPAND", "OPNOT",
    "TSTGT", "TSTGE", "TSTLT", "TSTLE", "TSTEQ", "TSTNE", "JMP", "BEQ", "BNE", "BRA",
    "CALL", "CALLI", "RET", "PUSHI", "PUSHI_EFF", "POP", "SWAP", "PUSHBP", "POPBP",
    "SPTOBP", "BPTOSP", "ADDSP", "FETCHM", "STO", "OFFSET", "START", "SAVE_REG",
    "PUSH_REG", "STRCMP", "EXIT_OP", "SAY_OP", "RESPOND_OP", "OPNEG"
};

/* Other files' data. The byte in a segment of its own is a static in the FM Towns
   build, written only by load_script; its name is provisional. */
extern unsigned cnv_id;
extern int CutsceneOrConversationStringBlock;
extern char far stdat[];
extern char far seg066_0;

/* Far string routines (seg017 and seg039). */
int far str_len(char far *s);
char far * far str_copy(char far *dst, char far *src);
void far str_ncopy(char far *dst, char far *src, int n);
char far * far FindStringDelimiter(char far *s, int c);
char far * far str_str(char far *s, char far *find);
int far str_cmp(char far *a, char far *b);
void far mem_set(void far *dst, char value, int size);
char far * far seg039_3452_857(char far *s);
int far seg039_3452_89A(char far *s);
char far * far get_string(int id);
int far make_string(char far *s, int block);
void far clear_dynamics(int block);

int far our_open(char *name, int mode, int flags);
int far ReadFileToAddress(int fd, void far *buf, int n);
int far FileWriteWithParams(int fd, void far *buf, int n);
unsigned char far open_arc(int arc, char *dir);
int far get_arc(int arc, int blk, char far *buf);
void far close_arc(int arc);
int far scroll_print(char far *s);
void far pfatal_code(int code);

/* This file's functions called before their definitions. */
int far ReadConversationHeader_ovr095_12C3(void);
void far CopyConversationCode_ovr095_14B1(void);
char far * far convert_string(char far *s);
int far GetStringReplacementNumberValue_ovr095_11C0(char far **s);
void far OPADD_ovr095_18CF(void);
void far OPNEG_ovr095_1908(void);
void far OPMUL_ovr095_192C(void);
void far OPSUB_ovr095_1965(void);
void far OPDIV_ovr095_199E(void);
void far OPMOD_ovr095_19EE(void);
void far OPOR_ovr095_1A3E(void);
void far OPAND_ovr095_1A83(void);
void far TSTGT_ovr095_1AC8(void);
void far TSTGE_ovr095_1B0A(void);
void far TSTLT_ovr095_1B4C(void);
void far TSTLE_ovr095_1B8E(void);
void far TSTEQ_ovr095_1BD0(void);
void far TSTNE_ovr095_1C12(void);
void far CALL_ovr095_1C54(void);
int far RET_ovr095_1C82(void);
void far FETCHM_ovr095_1CAA(void);
void far OFFSET_ovr095_1CD7(void);
void far STO_ovr095_1D11(void);
void far CallImportedFunction_ovr095_1D44(void);
void far STRCMP_ovr095_1DC1(void);
void far SAY_OP_ovr095_1EA2(void);
void far RESPOND_OP_ovr095_1F4A(void);
int far getmem(int addr);
int far GetValueAtBasePTRPlusArg0_ovr095_2030(int addr);
void far bab_fun(char *name, void (far *fn)());
void far bab_var_clear(void);
int far do_input_wait_ovr095_A54(void);
int far Random_ovr095_A5B(int far *args);
int far Compare_ovr095_A8B(int far *args);
int far Plural_ovr095_B95(int far *args);
int far Contains_ovr095_BDB(int far *args);
int far Append_ovr095_D0A(int far *args);
int far Copy_ovr095_DC7(int far *args);
int far ovr095_E36(int far *args);
int far Length_ovr095_E8D(int far *args);
int far Val_ovr095_EB2(int far *args);

/* The whole work area starts as one free block. A static in the FM Towns build. */
void far ovr095_0(char far *work)
{
    free_list = (struct BabBlock far *)work;
    free_list->size = 0xfbffL;
    free_list->next = 0;
}

/* Does the block holding p end with the tag bab_malloc put there? The FM Towns
   tag_check takes only p; bab_free passes a second argument that is never read. */
unsigned char far tag_check(char far *p, int unused)
{
    char far *base;
    char far *q;
    base = p - 8;
    p = base;
    q = p;
    if (((char far * far *)q)[(*(unsigned far *)base >> 2) - 1] == p + 8)
        return 1;
    return 0;
}

char far * far bab_malloc(long n)
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
            ((char far * far *)tail)[(((unsigned far *)current)[0] >> 2) - 1] = result;
            break;
        }
        previous = current;
        current = current->next;
    }
    return result;
}

void far bab_free(char far *data)
{
    struct BabBlock far *block;
    struct BabBlock far *previous;
    struct BabBlock far *current;
    if (!tag_check(data, 0)) return;
    block = (struct BabBlock far *)(data - 8);
    data = (char far *)block;
    current = free_list;
    previous = 0;
    while (current != 0) {
        if ((unsigned)current > (unsigned)block || current->next == 0) {
            if (previous != 0) {
                if ((char far *)previous + previous->size == (char far *)block) {
                    previous->size += block->size;
                    block = previous;
                } else previous->next = block;
            }
            block->next = current;
            if ((char far *)block + block->size == (char far *)current) {
                block->next = current->next;
                block->size += current->size;
            }
            if (current == free_list)
                free_list = block;
            break;
        }
        previous = current;
        current = current->next;
    }
    if (free_list == 0) {
        free_list = block;
        block->next = 0;
    }
}

char far * far bab_realloc(char far *p, long n)
{
    struct BabBlock far *block;
    struct BabBlock far *split;
    struct BabBlock far *current;
    char far *olddata;
    char far *result;
    long original;
    struct BabBlock far *tail;

    original = n;
    n = ((n + 3) & -4L) + 12;
    block = (struct BabBlock far *)(p - 8);
    olddata = p;
    p = (char far *)block;
    if (block->size > n) {
        current = free_list;
        result = 0;
        if (current == 0 || (unsigned)current > (unsigned)block) {
            result = p + 8;
            if (block->size - n > 16) {
                split = (struct BabBlock far *)(p + n);
                split->next = current;
                split->size = block->size - n;
                block->size = n;
                tail = block;
                ((char far * far *)tail)[(((unsigned far *)block)[0] >> 2) - 1] =
                    (char far *)block + 8;
                free_list = split;
                return result;
            }
            return result;
        } else {
            while (current != 0) {
                if ((unsigned)current->next > (unsigned)block || current->next == 0) {
                    result = p + 8;
                    if (block->size - n > 16) {
                        split = (struct BabBlock far *)(p + n);
                        split->next = current->next;
                        split->size = block->size - n;
                        block->size = n;
                        tail = block;
                        result = (char far *)block + 8;
                        ((char far * far *)tail)[(((unsigned far *)block)[0] >> 2) - 1] = result;
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
        if (result == 0) pfatal_code(4);
        if (block->size - 16 <= original)
            original = block->size - 16;
        movedata(FP_SEG(olddata), FP_OFF(olddata),
            FP_SEG(result), FP_OFF(result), (unsigned)original);
        bab_free(olddata);
    }
    return result;
}

/* Copies babglobs.dat, the initial globals of every conversation, to bglobals.dat. */
int far init_babl(void)
{
    char good;
    int size, block;
    register int source;
    register int dest;
    good = 1;
    if ((source = our_open("babglobs.dat", 1, 0)) < 0)
        return 0x3007;
    if ((dest = our_open("bglobals.dat", 0, 1)) >= 0) {
        mem_set(stdat, 0, 0x1000);
        while (good && read(source, &block, 4) == 4) {
            if (write(dest, &block, 4) != 4)
                good = 0;
            else
                good = FileWriteWithParams(dest, stdat, size << 1) == size << 1;
        }
        close(source);
        close(dest);
        if (good) return 0;
    }
    return 0x4001;
}

void far bab_get_globals(int far *memory, int count)
{
    int size;
    unsigned block;
    register int handle;
    register int done;
    if ((handle = our_open("bglobals.dat", 0, 0)) >= 0) {
        done = 0;
        while (!done) {
            if (read(handle, &block, 4) < 4 || block > cnv_id) {
                done = 1;
                break;
            }
            if (block == cnv_id) {
                if (count < size) size = count;
                if (ReadFileToAddress(handle, memory, size << 1) <
                    (unsigned)(size << 1)) done = 1;
            } else
                lseek(handle, (unsigned long)(unsigned)(size << 1), 1);
        }
        close(handle);
    }
}

void far bab_put_globals(int far *memory, int count)
{
    int size;
    unsigned block;
    register int handle;
    register int done;
    if ((handle = our_open("bglobals.dat", 0, 3)) >= 0) {
        done = 0;
        while (!done) {
            if (read(handle, &block, 4) < 4 || block > cnv_id) {
                done = 1;
                break;
            }
            if (block == cnv_id) {
                if (count < size) size = count;
                FileWriteWithParams(handle, memory, size << 1);
                done = 1;
            } else
                lseek(handle, (unsigned long)(unsigned)(size << 1), 1);
        }
        close(handle);
    }
}

int far load_script(char *name, char far *work)
{
    char far *empty;
    char far *buffer;
    register int size;
    ovr095_0(work);
    seg066_0 = 0;
    file_name = name;
    if (open_arc(2, "DATA\\")) {
        if ((arc_buffer = bab_malloc(0x5000L)) == 0)
            pfatal_code(4);
        buffer = arc_buffer;
        size = get_arc(2, cnv_id, arc_buffer);
        close_arc(2);
        if (size <= 0) {
            scroll_print(get_string(0xe01));
            return 1;
        }
    } else
        pfatal_code(0x300a);
    if (ReadConversationHeader_ovr095_12C3() < 0) return -1;
    CopyConversationCode_ovr095_14B1();
    bab_free(buffer);
    mem = (int far *)bab_malloc((long)((babl_nvars + 0x800) * sizeof(int)));
    bab_get_globals(mem, babl_nvars);
    stack_base = babl_nvars;
    stack = mem + stack_base;
    empty = bab_malloc(1L);
    *empty = 0;
    empty_string = make_string(empty, 0x7c);
    bab_var_clear();
    bab_fun("compare", (void (far *)())Compare_ovr095_A8B);
    bab_fun("random", (void (far *)())Random_ovr095_A5B);
    bab_fun("plural", (void (far *)())Plural_ovr095_B95);
    bab_fun("contains", (void (far *)())Contains_ovr095_BDB);
    bab_fun("append", (void (far *)())Append_ovr095_D0A);
    bab_fun("copy", (void (far *)())Copy_ovr095_DC7);
    bab_fun("find", (void (far *)())ovr095_E36);
    bab_fun("length", (void (far *)())Length_ovr095_E8D);
    bab_fun("val", (void (far *)())Val_ovr095_EB2);
    return 1;
}

/* What an import the game never bound calls. */
int far do_input_wait_ovr095_A54(void) { return 0; }

int far Random_ovr095_A5B(int far *args)
{
    return (int)(((long)rand() *
        getmem(args[-1])) / 0x8000L) + 1;
}

int far Compare_ovr095_A8B(int far *args)
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
    seg039_3452_857(a);
    seg039_3452_857(b);
    result = str_cmp(a, b);
    if (str2 != conv2) bab_free(conv2);
    if (str1 != conv1) bab_free(conv1);
    return result == 0;
}

int far Plural_ovr095_B95(int far *args)
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

/* Does the first string contain the second as a whole word? */
int far Contains_ovr095_BDB(int far *args)
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
    seg039_3452_857(str2);
    seg039_3452_857(str1);
    for (p = conv1; (p = str_str(p, conv2)) != 0; p += len) {
        len = str_len(conv2);
        if (p != 0 && (p == conv1 || isspace(p[-1]) || ispunct(p[-1])))
            if (p[len] == 0 || isspace(p[len]) || ispunct(p[len]))
                return 1;
    }
    return 0;
}

int far Append_ovr095_D0A(int far *args)
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
    out = bab_malloc((unsigned long)(unsigned)length);
    str_copy(out, s2);
    str_copy(out + len2, s1);
    id = make_string(out, 0x7c);
    return id;
}

int far Copy_ovr095_DC7(int far *args)
{
    char far *source, far *out;
    register int length;
    register int id;
    source = get_string(getmem(args[-1]));
    length = str_len(source) + 1;
    out = bab_malloc((unsigned long)(unsigned)length);
    str_copy(out, source);
    id = make_string(out, 0x7c);
    return id;
}

/* The script's "find": the 1-based position of a value in an array, or 0. */
int far ovr095_E36(int far *args)
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

int far Length_ovr095_E8D(int far *args)
{
    return str_len(get_string(getmem(args[-1])));
}

int far Val_ovr095_EB2(int far *args)
{
    return seg039_3452_89A(get_string(getmem(args[-1])));
}

/* Appends src at dest inside the growing buffer *buffer, enlarging it if needed. */
void far add_to(register char far **buffer, char far *dest, int *capacity, char far *src)
{
    int needed;
    register int offset;
    offset = dest - *buffer;
    if ((needed = offset + str_len(src) + 1) >= *capacity) {
        *buffer = bab_realloc(*buffer, (long)(needed + 16));
        *capacity = needed + 16;
        dest = *buffer + offset;
    }
    str_copy(dest, src);
}

/* Replaces each @-variable in a conversation string with its value: @ then the kind
   (G a global, P a parameter, S a stack slot, C a constant), then for all but C the
   type (I an integer, otherwise a string id), then the address, optionally followed
   by another variable giving an index. "@@" is a literal @. */
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
    int capacity;
    char digits[20];
    register int value;
    register int extra;
    if (FindStringDelimiter(text, '@') == 0)
        return text;
    capacity = (str_len(text) + 0x40) * 2;
    buffer = bab_malloc((long)capacity);
    s = text;
    out = buffer;
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
                    extra = GetStringReplacementNumberValue_ovr095_11C0(&s) - 1;
                else
                    extra = 0;
                if (kind == 'G')
                    value = getmem(number + extra);
                else if (kind == 'P')
                    value = getmem(
                        GetValueAtBasePTRPlusArg0_ovr095_2030(number) + extra);
                else if (kind == 'S')
                    value = GetValueAtBasePTRPlusArg0_ovr095_2030(number + extra);
                else
                    value = number;
                if (type == 'I') {
                    itoa(value, digits, 10);
                    add_to(&buffer, out, &capacity, digits);
                } else {
                    str = get_string(value);
                    if (str != 0) {
                        conv = convert_string(str);
                        add_to(&buffer, out, &capacity, conv);
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
    buffer = bab_realloc(buffer, (long)(out - buffer) + 1);
    return buffer;
}

/* Reads the index part of an @-variable, which is itself a variable. */
int far GetStringReplacementNumberValue_ovr095_11C0(register char far **s)
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
        extra = GetStringReplacementNumberValue_ovr095_11C0(s) - 1;
    else extra = 0;
    if (kind == 'G')
        result = getmem(parsed + extra);
    else if (kind == 'P')
        result = getmem(
            GetValueAtBasePTRPlusArg0_ovr095_2030(parsed) + extra);
    else if (kind == 'S')
        result = GetValueAtBasePTRPlusArg0_ovr095_2030(parsed + extra);
    else result = parsed;
    return result;
}

/* Reads the script header and its import table from arc_buffer. */
int far ReadConversationHeader_ovr095_12C3(void)
{
    int nimports;
    int index;
    int count;
    int kind;
    int type;
    char name[64];
    register int i;
    register int len;
    arc_buffer += 4;
    code_size = *(long far *)arc_buffer;
    arc_buffer += 4;
    code = (int far *)bab_malloc(code_size << 1);
    hdr_word = *(int far *)arc_buffer;
    arc_buffer += 2;
    CutsceneOrConversationStringBlock = *(int far *)arc_buffer;
    arc_buffer += 2;
    babl_nvars = *(int far *)arc_buffer;
    arc_buffer += 2;
    nimports = *(int far *)arc_buffer;
    arc_buffer += 2;
    func_count = 0;
    babl_import_table = (struct BablImport far *)bab_malloc(
        (long)((nimports + 1) * sizeof(struct BablImport)));
    for (i = 0; i < nimports; i++) {
        len = *(int far *)arc_buffer;
        arc_buffer += 2;
        movedata(FP_SEG(arc_buffer), FP_OFF(arc_buffer), FP_SEG(name), FP_OFF(name), len);
        arc_buffer += len;
        name[len] = 0;
        index = *(int far *)arc_buffer;
        arc_buffer += 2;
        count = *(int far *)arc_buffer;
        arc_buffer += 2;
        kind = *(int far *)arc_buffer;
        arc_buffer += 2;
        type = *(int far *)arc_buffer;
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
        funcs = (void (far * far *)())bab_malloc(
            (long)(func_count * sizeof(void (far *)())));
    for (i = 0; i < func_count; i++)
        funcs[i] = (void (far *)())do_input_wait_ovr095_A54;
    return 1;
}

void far CopyConversationCode_ovr095_14B1(void)
{
    movedata(FP_SEG(arc_buffer), FP_OFF(arc_buffer), FP_SEG(code), FP_OFF(code),
        (unsigned)code_size << 1);
}

void far ExitConversation_ovr095_14D4(void)
{
    clear_dynamics(0x7c);
    bab_put_globals(mem, babl_nvars);
}

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
            OPADD_ovr095_18CF();
            pc++;
            break;
        case 0x02:
            OPMUL_ovr095_192C();
            pc++;
            break;
        case 0x03:
            OPSUB_ovr095_1965();
            pc++;
            break;
        case 0x04:
            OPDIV_ovr095_199E();
            pc++;
            break;
        case 0x05:
            OPMOD_ovr095_19EE();
            pc++;
            break;
        case 0x29:
            OPNEG_ovr095_1908();
            pc++;
            break;
        case 0x0F:      /* JMP */
            pc = code[pc + 1];
            break;
        case 0x12:      /* BRA */
            pc = code[pc + 1] + pc + 1;
            break;
        case 0x06:
            OPOR_ovr095_1A3E();
            pc++;
            break;
        case 0x07:
            OPAND_ovr095_1A83();
            pc++;
            break;
        case 0x08:      /* OPNOT */
            stack[sp] = !stack[sp];
            pc++;
            break;
        case 0x09:
            TSTGT_ovr095_1AC8();
            pc++;
            break;
        case 0x0A:
            TSTGE_ovr095_1B0A();
            pc++;
            break;
        case 0x0B:
            TSTLT_ovr095_1B4C();
            pc++;
            break;
        case 0x0C:
            TSTLE_ovr095_1B8E();
            pc++;
            break;
        case 0x0D:
            TSTEQ_ovr095_1BD0();
            pc++;
            break;
        case 0x0E:
            TSTNE_ovr095_1C12();
            pc++;
            break;
        case 0x10:      /* BEQ */
            if (stack[sp--] == 0)
                pc = code[pc + 1] + pc + 1;
            else
                pc += 2;
            break;
        case 0x13:
            CALL_ovr095_1C54();
            break;
        case 0x15:
            running = RET_ovr095_1C82();
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
            FETCHM_ovr095_1CAA();
            pc++;
            break;
        case 0x21:
            OFFSET_ovr095_1CD7();
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
        case 0x1E:      /* ADDSP */
            sp += stack[sp] - 1;
            pc++;
            break;
        case 0x16:      /* PUSHI */
            sp++;
            stack[sp] = code[pc + 1];
            pc += 2;
            break;
        case 0x20:
            STO_ovr095_1D11();
            pc++;
            break;
        case 0x17:      /* PUSHI_EFF */
            sp++;
            stack[sp] = stack_base + code[pc + 1] + bp;
            pc += 2;
            break;
        case 0x14:      /* CALLI */
            CallImportedFunction_ovr095_1D44();
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
            STRCMP_ovr095_1DC1();
            pc++;
            break;
        case 0x26:      /* EXIT_OP */
            running = 0;
            break;
        case 0x27:
            SAY_OP_ovr095_1EA2();
            pc++;
            break;
        case 0x28:
            RESPOND_OP_ovr095_1F4A();
            pc++;
            break;
        default:
            running = 0;
        }
    }
    ExitConversation_ovr095_14D4();
    return 1;
}

void far OPADD_ovr095_18CF(void)
{
    int result = stack[sp] + stack[sp - 1];
    sp--;
    stack[sp] = result;
}
void far OPNEG_ovr095_1908(void)
{
    stack[sp] = -stack[sp];
}
void far OPMUL_ovr095_192C(void)
{
    int result = stack[sp] * stack[sp - 1];
    sp--;
    stack[sp] = result;
}
void far OPSUB_ovr095_1965(void)
{
    int result = stack[sp - 1] - stack[sp];
    sp--;
    stack[sp] = result;
}
void far OPDIV_ovr095_199E(void)
{
    int result;
    if (stack[sp] != 0)
        result = stack[sp - 1] / stack[sp];
    else result = 0x7fff;
    sp--;
    stack[sp] = result;
}
void far OPMOD_ovr095_19EE(void)
{
    int result;
    if (stack[sp] != 0)
        result = stack[sp - 1] % stack[sp];
    else result = 0x7fff;
    sp--;
    stack[sp] = result;
}
void far OPOR_ovr095_1A3E(void)
{
    int result = stack[sp - 1] || stack[sp];
    sp--;
    stack[sp] = result;
}
void far OPAND_ovr095_1A83(void)
{
    int result = stack[sp - 1] && stack[sp];
    sp--;
    stack[sp] = result;
}
void far TSTGT_ovr095_1AC8(void)
{
    int result = stack[sp - 1] > stack[sp];
    sp--;
    stack[sp] = result;
}
void far TSTGE_ovr095_1B0A(void)
{
    int result = stack[sp - 1] >= stack[sp];
    sp--;
    stack[sp] = result;
}
void far TSTLT_ovr095_1B4C(void)
{
    int result = stack[sp - 1] < stack[sp];
    sp--;
    stack[sp] = result;
}
void far TSTLE_ovr095_1B8E(void)
{
    int result = stack[sp - 1] <= stack[sp];
    sp--;
    stack[sp] = result;
}
void far TSTEQ_ovr095_1BD0(void)
{
    int result = stack[sp - 1] == stack[sp];
    sp--;
    stack[sp] = result;
}
void far TSTNE_ovr095_1C12(void)
{
    int result = stack[sp - 1] != stack[sp];
    sp--;
    stack[sp] = result;
}

void far CALL_ovr095_1C54(void)
{
    stack[++sp] = pc + 2;
    pc = code[pc + 1];
}
int far RET_ovr095_1C82(void)
{
    if (sp > 0) {
        pc = stack[sp];
        sp--;
        return 1;
    }
    return 0;
}
void far FETCHM_ovr095_1CAA(void)
{
    stack[sp] = mem[stack[sp]];
}
void far OFFSET_ovr095_1CD7(void)
{
    int result = stack[sp] + stack[sp - 1] - 1;
    sp--;
    stack[sp] = result;
}
void far STO_ovr095_1D11(void)
{
    mem[stack[sp - 1]] = stack[sp];
    sp -= 2;
}
void far CallImportedFunction_ovr095_1D44(void)
{
    int (far *fn)(int far *);
    register int result;
    fn = (int (far *)(int far *))funcs[code[pc + 1]];
    current_func = code[pc + 1];
    result = fn(stack + sp);
    stack[sp] = result;
    reg = stack[sp];
    pc += 2;
}
void far STRCMP_ovr095_1DC1(void)
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
void far SAY_OP_ovr095_1EA2(void)
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
void far RESPOND_OP_ovr095_1F4A(void)
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
int far * far getmem_addr(int addr)
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
int far GetValueAtBasePTRPlusArg0_ovr095_2030(int addr)
{
    return stack[bp + addr];
}
void far bab_fun(char *name, void (far *fn)())
{
    struct BablImport far *entry;
    register char *search = name;
    entry = babl_import_table;
    while (entry->count != 0) {
        if (*search == entry->name[0] && str_cmp(search, entry->name) == 0) {
            funcs[entry->index] = fn;
            break;
        }
        entry++;
    }
}
void far bab_var(char *name, int *values, int count)
{
    struct BablImport far *entry;
    char lower[26];
    register int i;
    register int limit = count;
    for (i = 0; i < strlen(name); i++)
        lower[i] = tolower(name[i]);
    lower[i] = 0;
    entry = babl_import_table;
    while (entry->count != 0) {
        if (str_cmp(name, entry->name) == 0) {
            for (i = 0; i < limit && i < entry->count; i++)
                mem[entry->index + i] = values[i];
            break;
        }
        entry++;
    }
}
void far bab_var_out(char *name, int *values, int count)
{
    struct BablImport far *entry;
    register int i;
    register int limit = count;
    entry = babl_import_table;
    while (entry->count != 0) {
        if (str_cmp(name, entry->name) == 0) {
            for (i = 0; i < limit && i < entry->count; i++)
                values[i] = mem[entry->index + i];
            break;
        }
        entry++;
    }
}
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
void far ovr095_2296(void) { }
