/* target: ovr116 */
/* opts: -mm -1 -G -O -Y -d */
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <stat.h>
#include <string.h>

extern unsigned char far *palette;
int far FileWriteWithParams(int fd, void far *buf, unsigned n);
void far grab(void far *dst, int x, int y, int w, int h);

static unsigned char gif_header[13] = {
    'G','I','F','8','7','a', 0x40,0x01,0xC8,0,0xF7,0,0
};
static unsigned char gif_image[10] = {
    ',',0,0,0,0,0x40,0x01,0xC8,0,7
};

/* GIF encoder state, DS:5E0C through DS:5E33. The field order follows the DOS offsets. */
static struct {
    int bits, bitpos;
    unsigned far *codes;
    int bytepos, clear;
    unsigned char far *pixels;
    unsigned far *hash;
    unsigned char far *bytes;
    int codebits, x, y, end, limit;
    unsigned char far *suffix;
    int next;
} gif;

static void far ovr116_149(int bits);
void far ovr116_194(int fd, char size);
static void far ovr116_1C3(int fd, int code);
void far ovr116_2A3(int fd, int bits);
int far ovr116_420(void);

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
    ovr116_2A3(fd, 8);
    write(fd, ";", 1);              /* the GIF trailer: the string pool's ";" at DS:1493, ending at 1495 */
    close(fd);
}

static void far ovr116_149(int bits)
{
    register int i;
    gif.codebits = bits + 1;
    gif.clear = 1 << bits;
    gif.end = gif.clear + 1;
    gif.next = gif.clear + 2;
    gif.limit = 1 << gif.codebits;
    for (i = 0; i < 0x138B; i++) gif.hash[i] = 0;
}

void far ovr116_194(int fd, char size)
{
    write(fd, &size, 1);
    FileWriteWithParams(fd, gif.bytes, (unsigned char)size);
}

static void far ovr116_1C3(int fd, int code)
{
    long value;
    gif.bytepos = gif.bitpos >> 3;
    gif.bits = gif.bitpos & 7;
    if (gif.bytepos >= 0xFE) {
        ovr116_194(fd, gif.bytepos);
        gif.bytes[0] = gif.bytes[gif.bytepos];
        gif.bitpos = gif.bits;
        gif.bytepos = 0;
    }
    if (gif.bits > 0) {
        value = ((long)code << gif.bits) | gif.bytes[gif.bytepos];
        gif.bytes[gif.bytepos] = value;
        gif.bytes[gif.bytepos + 1] = value >> 8;
        gif.bytes[gif.bytepos + 2] = (value >> 16);
    } else {
        gif.bytes[gif.bytepos] = code;
        gif.bytes[gif.bytepos + 1] = code >> 8;
    }
    gif.bitpos += gif.codebits;
}

void far ovr116_2A3(int fd, int bits)
{
    int next;
    int step;
    register int slot;
    register int prefix;
    write(fd, &bits, 1);
    gif.bitpos = 0;
    ovr116_149(bits);
    ovr116_1C3(fd, gif.clear);
    prefix = ovr116_420();
    while ((next = ovr116_420()) != -1) {
        slot = (prefix ^ (next << 5)) % 0x138B;
        step = 1;
        for (;;) {
            if (gif.hash[slot] == 0) {
                ovr116_1C3(fd, prefix);
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
                        ovr116_1C3(fd, gif.clear);
                        ovr116_149(bits);
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
    ovr116_1C3(fd, prefix);
    ovr116_1C3(fd, gif.end);
    if (gif.bitpos > 0) ovr116_194(fd, (gif.bitpos + 7) / 8);
    ovr116_194(fd, 0);
}

int far ovr116_420(void)
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
