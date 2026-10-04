/* target: ovr112 */
/* opts: -mm -1 -G -O -Y -d */
/* Saving the screen as a GIF file: the whole of DOS overlay ovr112, the same code as
   UW2's ovr116 (SCRSHOT.C there) byte for byte.

   ovr109 (UW1's counterpart of UW2's UWEDIT.C) binds key 271h (Alt+Q) to save_screenshot
   through RegisterEventHandler, passing the paragraph after seg049's start as scratch
   space for the encoder's tables. save_screenshot picks the first unused name
   UWPICnnn.GIF (nnn in octal, as the digits are built three bits at a time), writes a
   GIF87a header for a 320 by 200 picture with a 256-colour global table (the VGA palette
   scaled from 6 to 8 bits), an image descriptor and the LZW data, and the ';' trailer.
   The encoder (ovr112_2A3) is the usual GIF LZW with a 5003-entry open-addressed hash
   table (as in the Unix compress lineage): codes start at bits + 1 bits, grow to 12, and
   a clear code restarts the table when it fills. ovr112_1C3 packs codes into 255-byte
   sub-blocks that ovr112_194 writes; GifPixel_ovr116_420 reads the screen a row at a time,
   top row first, with grab (MODEX.ASM). The file owns the header templates (DS:1572) and
   the encoder state (DS:570A).

   name: descriptive, the UW2 file's names with UW1's segment; save_screenshot is a
   provisional name. targets/ovr112.tsv calls the function at 0x194 "pfatal"; it is UW2's
   ovr116_194 (kin: same) and is named after its offset here. */
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <stat.h>
#include <string.h>
#include "file.h"
#include "gfx.h"
#include "sys.h"

static unsigned char gif_header[13] = {
    'G','I','F','8','7','a', 0x40,0x01,0xC8,0,0xF7,0,0
};
static unsigned char gif_image[10] = {
    ',',0,0,0,0,0x40,0x01,0xC8,0,7
};

/* GIF encoder state, DS:570A through DS:5731 (UW2: DS:5E0C): codes, hash and suffix are the LZW table's
   prefix codes, codes and suffix bytes; pixels a row of the screen; bytes the sub-block
   being built.
   match: the field order follows the DOS offsets. */
HOST_LAYOUT_BEGIN
static struct {
    int16 bits, bitpos;
    uint16 far *codes;
    int16 bytepos, clear;
    unsigned char far *pixels;
    uint16 far *hash;
    unsigned char far *bytes;
    int16 codebits, x, y, end, limit;
    unsigned char far *suffix;
    int16 next;
} gif;
HOST_LAYOUT_END

/* UW1: the overlay's own names (UW2's are ovr116's) */

static void far ovr112_149(int bits);
static void far ovr112_1C3(int fd, int code);

void far save_screenshot(int seg)
{
    int number = 0;
    unsigned char c;
    void far *screen;
    char name[12] = "uwpic000.gif";
    int fd;
    register int i;

    screen = MK_FP(seg, 0);
    while ((fd = open(name, 0x8001)) != -1) {
        close(fd);
        number++;
        name[7] = (number & 7) + '0';
        name[6] = ((number >> 3) & 7) + '0';
        name[5] = ((number >> 6) & 7) + '0';
    }
    if ((fd = open(name, 0x8102, 0x180)) == -1) return;
    gif.codes = screen;
    gif.hash = gif.codes + 0x138B;
    gif.suffix = (unsigned char far *)(gif.hash + 0x138B);
    gif.pixels = gif.suffix + 0x138B;
    gif.bytes = gif.pixels + 0x142;
    gif.x = 0;
    gif.y = 0;
    write(fd, gif_header, 13);
    for (i = 0; i < 0x300; i++) {
        c = palette[i] << 2;
        write(fd, &c, 1);
    }
    write(fd, gif_image, 10);
    ovr112_2A3(fd, 8);
    write(fd, ";", 1);              /* the GIF trailer: the string pool's ";" at DS:1595, ending at 1597 */
    close(fd);
}

/* Resets the LZW table for a root size of bits. */
static void far ovr112_149(int bits)
{
    register int i;
    gif.codebits = bits + 1;
    gif.clear = 1 << bits;
    gif.end = gif.clear + 1;
    gif.next = gif.clear + 2;
    gif.limit = 1 << gif.codebits;
    for (i = 0; i < 0x138B; i++) gif.hash[i] = 0;
}

/* Writes a data sub-block: its length byte, then size bytes of gif.bytes. */
void far ovr112_194(int fd, char size)
{
    write(fd, &size, 1);
    FarWrite_ovr167_627(fd, gif.bytes, (unsigned char)size);
}

/* Appends code, gif.codebits wide, to the bit stream, flushing a sub-block once 254
   bytes are full. */
static void far ovr112_1C3(int fd, int code)
{
    int32 value;
    gif.bytepos = gif.bitpos >> 3;
    gif.bits = gif.bitpos & 7;
    if (gif.bytepos >= 0xFE) {
        ovr112_194(fd, gif.bytepos);
        gif.bytes[0] = gif.bytes[gif.bytepos];
        gif.bitpos = gif.bits;
        gif.bytepos = 0;
    }
    if (gif.bits > 0) {
        value = ((int32)code << gif.bits) | gif.bytes[gif.bytepos];
        gif.bytes[gif.bytepos] = value;
        gif.bytes[gif.bytepos + 1] = value >> 8;
        gif.bytes[gif.bytepos + 2] = (value >> 16);
    } else {
        gif.bytes[gif.bytepos] = code;
        gif.bytes[gif.bytepos + 1] = code >> 8;
    }
    gif.bitpos += gif.codebits;
}

/* Writes the LZW-coded image: the minimum code size byte, then the sub-blocks and the
   empty block that ends them. */
void far ovr112_2A3(int fd, int bits)
{
    int next;
    int step;
    register int slot;
    register int prefix;
    write(fd, &bits, 1);
    gif.bitpos = 0;
    ovr112_149(bits);
    ovr112_1C3(fd, gif.clear);
    prefix = GifPixel_ovr116_420();
    while ((next = GifPixel_ovr116_420()) != -1) {
        slot = (prefix ^ (next << 5)) % 0x138B;
        step = 1;
        for (;;) {
            if (gif.hash[slot] == 0) {
                ovr112_1C3(fd, prefix);
                step = gif.next;
                if (gif.next <= 0xFFF) {
                    gif.codes[slot] = prefix;
                    gif.suffix[slot] = next;
                    gif.hash[slot] = gif.next;
                    gif.next++;
                }
                if (step == gif.limit) {
                    if (gif.codebits < 12) {
                        gif.codebits++;
                        gif.limit <<= 1;
                    } else {
                        ovr112_1C3(fd, gif.clear);
                        ovr112_149(bits);
                    }
                }
                prefix = next;
                break;
            }
            if (gif.codes[slot] == prefix && gif.suffix[slot] == next) {
                prefix = gif.hash[slot];
                break;
            }
            slot += step;
            step += 2;
            if (slot < 0x138B) continue;
            slot -= 0x138B;
        }
    }
    ovr112_1C3(fd, prefix);
    ovr112_1C3(fd, gif.end);
    if (gif.bitpos > 0) ovr112_194(fd, (gif.bitpos + 7) / 8);
    ovr112_194(fd, 0);
}

/* The next pixel for the GIF encoder, or -1 at the end.
   match: the name is provisional (IDA's ovr116_420), chosen so that its tools/bssorder.py
   key puts it in the EXE's overlay stub order. */
int far GifPixel_ovr116_420(void)
{
    if (gif.x == 0x140 && gif.y == 0xC7) return -1;
    if (gif.x == 0x140) {
        gif.x = 0;
        gif.y++;
    }
    if (gif.x == 0) {
        grab(gif.pixels + 1, 0, 0xC7 - gif.y, 0xA0, 1);
        grab(gif.pixels + 0xA1, 0xA0, 0xC7 - gif.y, 0xA0, 1);
    }
    gif.x++;
    return gif.pixels[gif.x];
}
