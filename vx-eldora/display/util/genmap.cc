#include <stream.h>
#include <math.h>

#include "bitchars.h"

char *characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.:;,=+-";

static char fontlist[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x81,
    0xA5, 0x81, 0x81, 0xBD, 0x99, 0x81, 0x7E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x7E, 0xFF, 0xDB, 0xFF, 0xFF, 0xC3,
    0xE7, 0xFF, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x36, 0x7F, 0x7F, 0x7F, 0x7F, 0x3E, 0x1C, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x3E,
    0x7F, 0x3E, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x3C, 0x3C, 0xE7, 0xE7, 0xE7, 0x18, 0x18,
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3C,
    0x7E, 0xFF, 0xFF, 0x7E, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3C, 0x3C,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66,
    0x42, 0x42, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x07,
    0x0D, 0x19, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18,
    0x7E, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3F, 0x33, 0x3F, 0x30, 0x30, 0x30, 0x70, 0xF0, 0xE0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x63, 0x7F, 0x63,
    0x63, 0x63, 0x67, 0xE7, 0xE6, 0xC0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x18, 0xDB, 0x3C, 0xE7, 0x3C, 0xDB, 0x18,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x60,
    0x70, 0x7C, 0x7F, 0x7C, 0x70, 0x60, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x1F, 0x7F, 0x1F,
    0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x00, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x7F, 0xDB, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B,
    0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x30,
    0x1C, 0x36, 0x63, 0x63, 0x36, 0x1C, 0x06, 0x63, 0x3E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x7E,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3C, 0x7E, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0C, 0x06, 0x7F, 0x06, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x30, 0x7F, 0x30,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x7F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x66,
    0xFF, 0x66, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x1C, 0x1C, 0x3E, 0x3E, 0x7F, 0x7F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F,
    0x7F, 0x3E, 0x3E, 0x1C, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x3C, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x63, 0x22, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x36, 0x36, 0x7F, 0x36, 0x36, 0x36, 0x7F, 0x36,
    0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x3E, 0x63,
    0x61, 0x60, 0x3E, 0x03, 0x43, 0x63, 0x3E, 0x0C, 0x0C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x63, 0x06, 0x0C,
    0x18, 0x33, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1C, 0x36, 0x36, 0x1C, 0x3B, 0x6E, 0x66, 0x66, 0x3B, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x60, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18,
    0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x0C,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x3C, 0xFF, 0x3C,
    0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x0C, 0x18, 0x30,
    0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3E, 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x3E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x1C, 0x3C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3E, 0x63, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x63,
    0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63,
    0x03, 0x03, 0x1E, 0x03, 0x03, 0x63, 0x3E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x0E, 0x1E, 0x36, 0x66, 0x7F,
    0x06, 0x06, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7F, 0x60, 0x60, 0x60, 0x7E, 0x03, 0x03, 0x63, 0x3E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x30, 0x60, 0x60,
    0x7E, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x7F, 0x63, 0x03, 0x06, 0x0C, 0x18, 0x18, 0x18,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63,
    0x63, 0x63, 0x3E, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x3F, 0x03,
    0x03, 0x06, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,
    0x00, 0x00, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C,
    0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x0C,
    0x18, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3E, 0x63, 0x63, 0x06, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x63, 0x6F,
    0x6F, 0x6F, 0x6E, 0x60, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x1C, 0x36, 0x63, 0x63, 0x7F, 0x63, 0x63,
    0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x33,
    0x33, 0x33, 0x3E, 0x33, 0x33, 0x33, 0x7E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x1E, 0x33, 0x61, 0x60, 0x60, 0x60,
    0x61, 0x33, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7C, 0x36, 0x33, 0x33, 0x33, 0x33, 0x33, 0x36, 0x7C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x33, 0x31, 0x34,
    0x3C, 0x34, 0x31, 0x33, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x7F, 0x33, 0x31, 0x34, 0x3C, 0x34, 0x30, 0x30,
    0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x33,
    0x61, 0x60, 0x60, 0x6F, 0x63, 0x33, 0x1D, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x63, 0x63, 0x7F, 0x63,
    0x63, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x73, 0x33, 0x36, 0x36, 0x3C, 0x36, 0x36, 0x33,
    0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x31, 0x33, 0x7F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xC3, 0xE7, 0xFF, 0xDB, 0xC3, 0xC3,
    0xC3, 0xC3, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x63, 0x73, 0x7B, 0x7F, 0x6F, 0x67, 0x63, 0x63, 0x63, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x36, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x36, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x7E, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x30, 0x30,
    0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63,
    0x63, 0x63, 0x63, 0x6B, 0x6F, 0x3E, 0x06, 0x07, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x7E, 0x33, 0x33, 0x33, 0x3E, 0x36,
    0x33, 0x33, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3E, 0x63, 0x63, 0x30, 0x1C, 0x06, 0x63, 0x63, 0x3E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xDB, 0x99, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63,
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3,
    0xC3, 0xC3, 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0xC3, 0xDB, 0xDB,
    0xFF, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0xC3, 0xC3, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0x66,
    0x3C, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xC3, 0x86, 0x0C, 0x18, 0x30, 0x61, 0xC3,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x60, 0x70, 0x38, 0x1C, 0x0E,
    0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x18, 0x18, 0x0C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x06, 0x3E,
    0x66, 0x66, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x70, 0x30, 0x30, 0x3C, 0x36, 0x33, 0x33, 0x33, 0x6E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E,
    0x63, 0x60, 0x60, 0x63, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0E, 0x06, 0x06, 0x1E, 0x36, 0x66, 0x66, 0x66,
    0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3E, 0x63, 0x7F, 0x60, 0x63, 0x3E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x1C, 0x36, 0x32, 0x30, 0x7C, 0x30,
    0x30, 0x30, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3B, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x66,
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x30, 0x30, 0x36,
    0x3B, 0x33, 0x33, 0x33, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0C, 0x0C, 0x00, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06,
    0x00, 0x0E, 0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x70, 0x30, 0x30, 0x33, 0x36, 0x3C,
    0x36, 0x33, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE6,
    0xFF, 0xDB, 0xDB, 0xDB, 0xDB, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3E, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x33, 0x33,
    0x33, 0x3E, 0x30, 0x30, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3B, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x06,
    0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6E,
    0x3B, 0x33, 0x30, 0x30, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x38, 0x0E, 0x63,
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18,
    0x18, 0x7E, 0x18, 0x18, 0x18, 0x1B, 0x0E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66,
    0x66, 0x66, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3,
    0xC3, 0xDB, 0xDB, 0xFF, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x36, 0x1C, 0x1C, 0x36,
    0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x63, 0x63, 0x63, 0x63, 0x3F, 0x03, 0x06, 0x3C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x66, 0x0C,
    0x18, 0x33, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0E, 0x18, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x0E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18,
    0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x70, 0x18, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x18,
    0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x6E,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x36, 0x63,
    0x63, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1E, 0x33, 0x61, 0x60, 0x60, 0x61, 0x33, 0x1E, 0x06, 0x03,
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x00, 0x66,
    0x66, 0x66, 0x66, 0x66, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x06, 0x0C, 0x18, 0x00, 0x3E, 0x63, 0x7F, 0x60, 0x63,
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x36,
    0x00, 0x3C, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x00, 0x3C, 0x06, 0x3E,
    0x66, 0x66, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
    0x18, 0x0C, 0x00, 0x3C, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x36, 0x1C, 0x00, 0x3C,
    0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x60, 0x66, 0x3C, 0x0C,
    0x06, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x36,
    0x00, 0x3E, 0x63, 0x7F, 0x60, 0x63, 0x3E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x00, 0x3E, 0x63, 0x7F,
    0x60, 0x63, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
    0x18, 0x0C, 0x00, 0x3E, 0x63, 0x7F, 0x60, 0x63, 0x3E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x00, 0x38,
    0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x18, 0x3C, 0x66, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18,
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x18,
    0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x63, 0x63, 0x08, 0x1C, 0x36, 0x63, 0x63,
    0x7F, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x36,
    0x1C, 0x00, 0x1C, 0x36, 0x63, 0x63, 0x7F, 0x63, 0x63, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0C, 0x18, 0x30, 0x00, 0x7F, 0x33,
    0x30, 0x3E, 0x30, 0x33, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x6E, 0x3B, 0x1B, 0x7E, 0xD8, 0xDC,
    0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x36,
    0x66, 0x66, 0x7F, 0x66, 0x66, 0x66, 0x67, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x1C, 0x36, 0x00, 0x3E, 0x63, 0x63,
    0x63, 0x63, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x63, 0x63, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x0C, 0x00, 0x3E,
    0x63, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x18, 0x3C, 0x66, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66,
    0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x0C,
    0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3B, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x00, 0x63, 0x63, 0x63,
    0x63, 0x3F, 0x03, 0x06, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x63,
    0x63, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x00, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x18, 0x18, 0x7E, 0xC3, 0xC0, 0xC0, 0xC3, 0x7E, 0x18,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x36, 0x32,
    0x30, 0x78, 0x30, 0x30, 0x30, 0x73, 0x7E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xC3, 0x66, 0x3C, 0x18, 0xFF, 0x18,
    0xFF, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
    0x66, 0x66, 0x7C, 0x62, 0x66, 0x6F, 0x66, 0x66, 0xF3, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x1B, 0x18, 0x18, 0x18,
    0x7E, 0x18, 0x18, 0x18, 0x18, 0xD8, 0x70, 0x00, 0x00, 0x00,
    0x00, 0x0C, 0x18, 0x30, 0x00, 0x3C, 0x06, 0x3E, 0x66, 0x66,
    0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x18, 0x30,
    0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0C, 0x18, 0x30, 0x00, 0x3E, 0x63, 0x63,
    0x63, 0x63, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C,
    0x18, 0x30, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3B, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x6E, 0x00, 0x6E,
    0x33, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3B, 0x6E, 0x00, 0x63, 0x73, 0x7B, 0x7F, 0x6F, 0x67, 0x63,
    0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x6C, 0x6C,
    0x3E, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x38, 0x6C, 0x6C, 0x38, 0x00, 0x7C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x18, 0x00, 0x18, 0x18, 0x30, 0x63, 0x63, 0x3E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7F, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x03, 0x03, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xE0, 0x63,
    0x66, 0x6C, 0x18, 0x30, 0x6E, 0xC3, 0x06, 0x0C, 0x1F, 0x00,
    0x00, 0x00, 0x00, 0x60, 0xE0, 0x63, 0x66, 0x6C, 0x18, 0x33,
    0x67, 0xCF, 0x1F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x18, 0x00, 0x18, 0x18, 0x3C, 0x3C, 0x3C, 0x18, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x36,
    0x6C, 0x36, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x6C, 0x36, 0x1B, 0x36, 0x6C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x44, 0x11, 0x44,
    0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44,
    0x00, 0x00, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA,
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x00, 0x00, 0xDD, 0x77,
    0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77,
    0xDD, 0x77, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18,
    0x18, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x00, 0x00, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xF6,
    0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x36, 0x36, 0x36, 0x36,
    0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
    0x18, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00,
    0x36, 0x36, 0x36, 0x36, 0x36, 0xF6, 0x06, 0xF6, 0x36, 0x36,
    0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36,
    0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x06, 0xF6,
    0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 0x36, 0x36,
    0x36, 0x36, 0x36, 0xF6, 0x06, 0xFE, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
    0x36, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0xF8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
    0x36, 0x37, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00, 0x00,
    0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x30, 0x3F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3F, 0x30, 0x37, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
    0x00, 0x00, 0x36, 0x36, 0x36, 0x36, 0x36, 0xF7, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0xF7, 0x36, 0x36, 0x36, 0x36,
    0x36, 0x36, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36, 0x36, 0x37,
    0x30, 0x37, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36,
    0x36, 0xF7, 0x00, 0xF7, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
    0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36,
    0x36, 0x36, 0x36, 0x36, 0x36, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x36, 0x36,
    0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36,
    0x36, 0x36, 0x36, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x1F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3F, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00, 0x00,
    0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xFF, 0x36, 0x36,
    0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18,
    0x18, 0xFF, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
    0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B,
    0x6E, 0x6C, 0x6C, 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x7E, 0x63, 0x63, 0x7E,
    0x60, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x63,
    0x63, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x36, 0x36, 0x36,
    0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7F, 0x63, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x63, 0x7F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
    0x6C, 0x6C, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x3E, 0x30,
    0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3B, 0x6E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x7E, 0x18, 0x3C, 0x66, 0x66, 0x66,
    0x3C, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1C, 0x36, 0x63, 0x63, 0x7F, 0x63, 0x63, 0x36, 0x1C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x36, 0x63, 0x63,
    0x63, 0x36, 0x36, 0x36, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1E, 0x30, 0x18, 0x0C, 0x3E, 0x66, 0x66, 0x66,
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x7E, 0xDB, 0xDB, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x7E, 0xDB, 0xDB, 0xF3,
    0x7E, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1C, 0x30, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x30, 0x1C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x7F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18,
    0x18, 0xFF, 0x18, 0x18, 0x18, 0x00, 0xFF, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18,
    0x30, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x00, 0x7E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x1B, 0x1B, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8,
    0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18,
    0x00, 0x00, 0xFF, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x6E, 0x00, 0x3B,
    0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
    0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0xEC, 0x6C, 0x3C, 0x1C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xD8, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70,
    0xD8, 0x30, 0x60, 0xC8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x3E,
    0x3E, 0x3E, 0x3E, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

struct character {
    unsigned short bits[14];
};

static struct character bits;

main(int argc, char **argv)
{
    int x = -7;
    int y = 0;
    int xsize = 1;
    int ysize = xsize;
    int yend = 14;

    char *ptr = characters;

    for (int i = 0; *ptr; i++, ptr++)
      {
	  char *font = fontlist + (*ptr << 4);

	  for (int j = y; j < yend; j++)
	    {
		int l = 128;

		unsigned short tmp = 0;

		for (int k = x + (i << 3); l; k++)
		  {
		      if (!(l & *font))
			tmp <<= 1;
		      else
			{
			    tmp <<= 1;
			    tmp |= 1;
			}

 		      if (k % xsize == (xsize - 1))
                        l >>= 1;
                  }

		cout << tmp << ",\n";

		if (j % ysize == (ysize - 1))
                  font++;
	    }
      }
    exit(0);
}
