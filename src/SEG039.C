/* target: seg039_3452 */
/* opts: -mm -1 -G -O -Y -d */
#include <ctype.h>
#include "conv.h"
#include "file.h"
#include "object.h"
#include "sys.h"
#include "ui.h"

struct StringBlock {
    int block;
    char far *strings[512];
    int count;
};

/* The string decoder's buffer and the two cached string blocks: far, so a segment each
   (617D:0000 and 627D:0000, segment table entries 72 and 73), defined in this order. Only
   this file uses them. FM Towns names the buffer str_buff (read_string_); its blocks
   follow str_file (StringsPak_FileHandle here) unnamed, so they were static. */
char far str_buff[0x1000];
static struct StringBlock far Strings[2];

int OutString = 0;
int string_bits_used = 8;
int string_blocks = 0;
char SPACE[] = " ";
char aStrings_pak[] = "strings.pak";
char aRb_4[] = "rb";

int far replace_string(char far *s, int id);

void far scroll_print(char far *s);
int far read(int file, void *p, int count);
void far * far farmalloc(unsigned long size);
int far close(int file);
int far fclose(int file);
void far farfree(void far *p);
int far data_fopen(char *name, char *mode);
int far fread(void *p, int size, int count, int file);
int far fseek(int file, long offset, int whence);
int far fgetc(int file);

unsigned char far init_strings(void)
{
    int i, j;
    for (i = 0; i < 2; i++) {
        Strings[i].block = -1;
        Strings[i].count = 0;
        for (j = 0; j < 512; j++) Strings[i].strings[j] = 0;
    }
    if ((i = LoadFileStringsPak_seg039_547()) != 0) first_punt(i);
    return 1;
}

void far free_strings(void) { seg039_3452_5E1(); }

char far * far get_string(int id)
{
    int found;
    int string;
    register int i;
    register int block;
    block = (unsigned)id >> 9;
    string = id & 0x1ff;
    found = -1;
    for (i = 0; i < string_blocks; i++)
        if (Strings[i].block == block) { found = i; break; }
    if (found >= 0) return Strings[found].strings[string];
    if (block == 0) return read_string(CutsceneOrConversationStringBlock, string);
    return read_string(block, string);
}

int far make_string(char far *s, int block)
{
    int index;
    int found = -1;
    int i;
    for (i = 0; i < string_blocks; i++)
        if (Strings[i].block == block) { found = i; break; }
    if (found < 0) {
        if (string_blocks >= 2) return 0;
        found = string_blocks;
        Strings[found].block = block;
        Strings[found].count = 0;
        for (i = 0; i < 512; i++) Strings[found].strings[i] = 0;
        string_blocks++;
    }
    index = Strings[found].count;
    Strings[found].strings[index] = s;
    Strings[found].count++;
    return index | (block << 9);
}

/* FM Towns replace_string_ occupies the corresponding slot and stores a new pointer. */
int far replace_string(char far *s, int id)
{
    int string;
    int block;
    int found;
    int i;
    block = (unsigned)id >> 9;
    string = id & 0x1ff;
    found = -1;
    for (i = 0; i < string_blocks; i++)
        if (Strings[i].block == block) { found = i; break; }
    if (found >= 0) { Strings[found].strings[string] = s; return id; }
    return 0;
}

/* FM Towns clear_dynamics_ occupies this slot and clears a block's pointers. */
void far clear_dynamics(int block)
{
    int found = -1;
    int i;
    for (i = 0; i < string_blocks; i++)
        if (Strings[i].block == block) { found = i; break; }
    if (found >= 0) {
        Strings[found].block = block;
        Strings[found].count = 0;
        for (i = 0; i < 512; i++) Strings[found].strings[i] = 0;
    }
}

int far get_name(char far *dst, struct Object far *obj, char article, char plural)
{
    int item = obj->id & 0x1ff;
    char far *name;
    unsigned char who;
    if (((obj->id & 0x1c0) >> 6) == 1) {
        who = obj->whoami;
        if (who > 0 && (unsigned char)who < 0xf0) {
            name = get_string(((unsigned char)who + 0x10) | 0xe00);
            if (name && *name) { str_copy(dst, name); return 1; }
        }
    }
    name = get_string(item | 0x800);
    if (name == 0 || name[0] == 0) { str_copy(dst, SPACE); return 0; }
    str_copy(dst, fix_name_string(name, article, plural));
    return 1;
}

char far * far fix_name_string(char far *s, unsigned char article, char plural)
{
    char far *amp = FindStringDelimiter(s, '&');
    char far *under;
    if (plural) {
        if (amp) s = amp + 1;
        else { under = s + str_len(s); *under = 's'; under[1] = 0; }
    } else if (amp) *amp = 0;
    under = FindStringDelimiter(s, '_');
    if (under) {
        if (!article) s = under + 1;
        else *under = ' ';
    }
    return s;
}

void far game_sprint(int id)
{
    char far *s = get_string(id | 0x200);
    scroll_print(s);
}

void far game_strings_3(int first, int second, int third)
{
    char text[256];
    str_copy(text, get_string(first | 0x200));
    if (second >= 0) str_cat(text, get_string(second | 0x200));
    if (third >= 0) str_cat(text, get_string(third | 0x200));
    scroll_print(text);
}

int far LoadFileStringsPak_seg039_547(void)
{
    int file;
    if ((file = our_open(aStrings_pak, 1, 0)) == -1) return 0x3002;
    read(file, &StringsPak_NoOfNodes, 2);
    StringsPak_Address_Indices = farmalloc((unsigned)(StringsPak_NoOfNodes << 2));
    if (!StringsPak_Address_Indices) {
        close(file);
        return 0x1001;
    }
    intoFarBuffer_ovr167_5DA(file, StringsPak_Address_Indices, StringsPak_NoOfNodes << 2);
    close(file);
    if ((StringsPak_FileHandle = data_fopen(aStrings_pak, aRb_4)) == 0) return 0x3002;
    return 0;
}

void far seg039_3452_5E1(void)
{
    fclose(StringsPak_FileHandle);
    farfree(StringsPak_Address_Indices);
}

char far * far read_string(int block, int string)
{
    unsigned char c;
    char far *result;
    int count, item, relative, string_count;
    long address;
    register int index, found;
    result = str_buff + OutString;
    index = 0;
    fseek(StringsPak_FileHandle, (unsigned)(StringsPak_NoOfNodes << 2) + 2, 0);
    fread(&count, 2, 1, StringsPak_FileHandle);
    for (found = 0; found < count; found++) {
        fread(&item, 2, 1, StringsPak_FileHandle);
        if (item == block) break;
        fseek(StringsPak_FileHandle, 4L, 1);
    }
    if (found == count) { result[index] = 0; return result; }
    fread(&address, 4, 1, StringsPak_FileHandle);
    fseek(StringsPak_FileHandle, address, 0);
    fread(&string_count, 2, 1, StringsPak_FileHandle);
    if (string_count <= string) { result[index] = 0; return result; }
    fseek(StringsPak_FileHandle, (unsigned)(string << 1), 1);
    fread(&relative, 2, 1, StringsPak_FileHandle);
    fseek(StringsPak_FileHandle,
        (unsigned)(((string_count - (string + 1)) << 1) + relative), 1);
    string_bits_used = 8;
    c = '|';
    do {
        c = seg039_3452_7B2(StringsPak_FileHandle, StringsPak_NoOfNodes - 1);
        result[index] = c;
        index++;
    } while (c != 0xff && c != '|' && index < 512);
    result[index - 1] = 0;
    OutString += 512;
    if (OutString > 0xfff) OutString = 0;
    return result;
}

int far seg039_3452_781(int file)
{
    int bit;
    if (string_bits_used == 8) {
        string_bits = fgetc(file);
        string_bits_used = 0;
    }
    bit = string_bits & 0x80;
    string_bits <<= 1;
    string_bits_used++;
    return bit;
}

int far seg039_3452_7B2(int file, int index)
{
    register int value;
    register int f;
    f = file;
    value = index;
    while (StringsPak_Address_Indices[value].left != 0xff) {
        if (seg039_3452_781(f)) value = StringsPak_Address_Indices[value].right;
        else value = StringsPak_Address_Indices[value].left;
    }
    return StringsPak_Address_Indices[value].value;
}

char far * far seg039_3452_814(char far *s)
{
    char far *start = s;
    while (*s) { if (islower(*s)) *s = *s - 0x20; s++; }
    return start;
}

char far * far seg039_3452_857(char far *s)
{
    char far *start = s;
    while (*s) { if (isupper(*s)) *s = *s + 0x20; s++; }
    return start;
}

/* isspace and isdigit by hand: <ctype.h>'s _ctype indexed with the character as a signed
   char, plus one (the C library's table at DS:1BF6 starts with the entry for EOF). */
int far seg039_3452_89A(char far *s)
{
    int i = 0, n = 0;
    while ((_ctype + 1)[(signed char)s[i]] & _IS_SP) i++;
    i += (*s == '-');
    while ((_ctype + 1)[(signed char)s[i]] & _IS_DIG) {
        n *= 10;
        n += (signed char)s[i] - '0';
        i++;
    }
    return *s == '-' ? -n : n;
}

char far * far str_cat(char far *dst, char far *src)
{
    char far *start = dst;
    while (*dst) dst++;
    while (*src) { *dst = *src; src++; dst++; }
    *dst = *src; src++; dst++;
    return start;
}
