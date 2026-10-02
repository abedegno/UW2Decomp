/* png.c: replaces nothing. Writes the port's screenshots: an indexed picture and the VGA DAC's
   6-bit palette as a 24-bit PNG, uncompressed (zlib stored blocks), so it needs no library;
   and an RGB picture (plat_write_png_rgb, for the window's own contents).
   Each 6-bit component v becomes (v << 2) | (v >> 4), as DOSBox converts the DAC, so a PNG
   from the port and a screenshot of DOS under DOSBox or js-dos agree byte for byte when the
   VGA state does. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plat.h"

static uint32_t crc_table[256];

static void crc_init(void)
{
    uint32_t c;
    int n, k;
    for (n = 0; n < 256; n++) {
        c = (uint32_t)n;
        for (k = 0; k < 8; k++) c = c & 1 ? 0xEDB88320u ^ (c >> 1) : c >> 1;
        crc_table[n] = c;
    }
}

static uint32_t crc(uint32_t c, const uint8_t *p, size_t n)
{
    c ^= 0xFFFFFFFFu;
    while (n--) c = crc_table[(c ^ *p++) & 0xFF] ^ (c >> 8);
    return c ^ 0xFFFFFFFFu;
}

static void put32(uint8_t *p, uint32_t v)
{
    p[0] = v >> 24; p[1] = v >> 16; p[2] = v >> 8; p[3] = v;
}

static void chunk(FILE *f, const char *type, const uint8_t *data, uint32_t n)
{
    uint8_t b[4];
    uint32_t c;
    put32(b, n);
    fwrite(b, 1, 4, f);
    fwrite(type, 1, 4, f);
    if (n) fwrite(data, 1, n, f);
    c = crc(0, (const uint8_t *)type, 4);
    c = crc(c, data, n);
    put32(b, c);
    fwrite(b, 1, 4, f);
}

int plat_write_png_rgb(const char *path, const uint8_t *rgb, int width, int height)
{
    static const uint8_t sig[8] = { 0x89, 'P', 'N', 'G', 13, 10, 26, 10 };
    size_t row = (size_t)width * 3 + 1, raw = row * (size_t)height, blocks = (raw + 65534) / 65535;
    uint8_t *img, *z, ihdr[13];
    uint32_t a = 1, b = 0;
    size_t i, zn, left, at;
    int y;
    FILE *f;

    crc_init();
    img = malloc(raw);
    z = malloc(raw + blocks * 5 + 6);
    if (!img || !z) { free(img); free(z); return -1; }
    for (y = 0; y < height; y++) {
        img[row * (size_t)y] = 0;
        memcpy(img + row * (size_t)y + 1, rgb + (size_t)y * (size_t)width * 3, (size_t)width * 3);
    }
    zn = 0;
    z[zn++] = 0x78; z[zn++] = 0x01;
    for (left = raw, at = 0; left; ) {
        size_t n = left > 65535 ? 65535 : left;
        z[zn++] = left == n;
        z[zn++] = n & 0xFF; z[zn++] = n >> 8;
        z[zn++] = ~n & 0xFF; z[zn++] = (~n >> 8) & 0xFF;
        memcpy(z + zn, img + at, n);
        zn += n; at += n; left -= n;
    }
    for (i = 0; i < raw; i++) { a = (a + img[i]) % 65521; b = (b + a) % 65521; }
    put32(z + zn, (b << 16) | a);
    zn += 4;
    put32(ihdr, (uint32_t)width);
    put32(ihdr + 4, (uint32_t)height);
    ihdr[8] = 8; ihdr[9] = 2; ihdr[10] = 0; ihdr[11] = 0; ihdr[12] = 0;
    f = fopen(path, "wb");
    if (f) {
        fwrite(sig, 1, 8, f);
        chunk(f, "IHDR", ihdr, 13);
        chunk(f, "IDAT", z, (uint32_t)zn);
        chunk(f, "IEND", NULL, 0);
        fclose(f);
    }
    free(img);
    free(z);
    return f ? 0 : -1;
}

int plat_write_png(const char *path, const uint8_t *pixels, int width, int height,
                   const uint8_t rgb6[768])
{
    uint8_t *rgb = malloc((size_t)width * (size_t)height * 3);
    int i, r;
    if (!rgb) return -1;
    for (i = 0; i < width * height; i++) {
        const uint8_t *c = rgb6 + 3 * pixels[i];
        rgb[3 * i] = (uint8_t)((c[0] << 2) | (c[0] >> 4));
        rgb[3 * i + 1] = (uint8_t)((c[1] << 2) | (c[1] >> 4));
        rgb[3 * i + 2] = (uint8_t)((c[2] << 2) | (c[2] >> 4));
    }
    r = plat_write_png_rgb(path, rgb, width, height);
    free(rgb);
    return r;
}
