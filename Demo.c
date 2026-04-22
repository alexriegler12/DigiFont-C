/* DigiFont library example - SDL 1.2 port */
/* All styles demo */
/* (c) 2020-24 Pawel A. Hernik */
/* SDL 1.2 / ANSI C89 conversion */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL/SDL.h>

#include "DigiFont.h"

/* ----------------------------------------------------------------
   Screen / SDL globals
   ---------------------------------------------------------------- */
#define SCR_WD 240
#define SCR_HT 320

static SDL_Surface *screen = NULL;

/* ----------------------------------------------------------------
   Color helpers
   ---------------------------------------------------------------- */

/* Pack RGB into a 32-bit SDL color (matches the surface format) */
static Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b)
{
    return SDL_MapRGB(screen->format, r, g, b);
}

/* Encode like the original RGBto565 macro but store as full SDL Uint32 */
#define RGBto565(r,g,b) mapRGB((r),(g),(b))

#define BLACK   mapRGB(0,   0,   0)
#define WHITE   mapRGB(255, 255, 255)

/* ----------------------------------------------------------------
   Low-level drawing primitives (SDL 1.2)
   ---------------------------------------------------------------- */
static void sdl_hline(int x0, int x1, int y, Uint32 col)
{
    SDL_Rect r;
    if (x1 < x0) { int tmp = x0; x0 = x1; x1 = tmp; }
    r.x = (Sint16)x0;
    r.y = (Sint16)y;
    r.w = (Uint16)(x1 - x0 + 1);
    r.h = 1;
    SDL_FillRect(screen, &r, col);
}

static void sdl_vline(int x, int y0, int y1, Uint32 col)
{
    SDL_Rect r;
    if (y1 < y0) { int tmp = y0; y0 = y1; y1 = tmp; }
    r.x = (Sint16)x;
    r.y = (Sint16)y0;
    r.w = 1;
    r.h = (Uint16)(y1 - y0 + 1);
    SDL_FillRect(screen, &r, col);
}

static void sdl_rect(int x, int y, int w, int h, Uint32 col)
{
    SDL_Rect r;
    r.x = (Sint16)x;
    r.y = (Sint16)y;
    r.w = (Uint16)w;
    r.h = (Uint16)h;
    SDL_FillRect(screen, &r, col);
}

/* ----------------------------------------------------------------
   DigiFont callbacks – must match the int-colour signature
   We store SDL Uint32 colours cast to int and cast back here.
   ---------------------------------------------------------------- */
static void customLineH(int x0, int x1, int y, int c)
{
    sdl_hline(x0, x1, y, (Uint32)c);
}

static void customLineV(int x, int y0, int y1, int c)
{
    sdl_vline(x, y0, y1, (Uint32)c);
}

static void customRect(int x, int y, int w, int h, int c)
{
    sdl_rect(x, y, w, h, (Uint32)c);
}

/* ----------------------------------------------------------------
   Simple text rendering (8x8 bitmap font, SDL_DrawText replacement)
   We use a tiny built-in 7x8 ASCII font so we have no extra deps.
   ---------------------------------------------------------------- */

/* 5x7 font glyphs for ASCII 32-126, stored as 5 bytes per char   */
/* (standard Adafruit/Arduino GFX font subset)                     */
static const unsigned char font5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, /* 32 space */
    {0x00,0x00,0x5F,0x00,0x00}, /* 33 !     */
    {0x00,0x07,0x00,0x07,0x00}, /* 34 "     */
    {0x14,0x7F,0x14,0x7F,0x14}, /* 35 #     */
    {0x24,0x2A,0x7F,0x2A,0x12}, /* 36 $     */
    {0x23,0x13,0x08,0x64,0x62}, /* 37 %     */
    {0x36,0x49,0x55,0x22,0x50}, /* 38 &     */
    {0x00,0x05,0x03,0x00,0x00}, /* 39 '     */
    {0x00,0x1C,0x22,0x41,0x00}, /* 40 (     */
    {0x00,0x41,0x22,0x1C,0x00}, /* 41 )     */
    {0x14,0x08,0x3E,0x08,0x14}, /* 42 *     */
    {0x08,0x08,0x3E,0x08,0x08}, /* 43 +     */
    {0x00,0x50,0x30,0x00,0x00}, /* 44 ,     */
    {0x08,0x08,0x08,0x08,0x08}, /* 45 -     */
    {0x00,0x60,0x60,0x00,0x00}, /* 46 .     */
    {0x20,0x10,0x08,0x04,0x02}, /* 47 /     */
    {0x3E,0x51,0x49,0x45,0x3E}, /* 48 0     */
    {0x00,0x42,0x7F,0x40,0x00}, /* 49 1     */
    {0x42,0x61,0x51,0x49,0x46}, /* 50 2     */
    {0x21,0x41,0x45,0x4B,0x31}, /* 51 3     */
    {0x18,0x14,0x12,0x7F,0x10}, /* 52 4     */
    {0x27,0x45,0x45,0x45,0x39}, /* 53 5     */
    {0x3C,0x4A,0x49,0x49,0x30}, /* 54 6     */
    {0x01,0x71,0x09,0x05,0x03}, /* 55 7     */
    {0x36,0x49,0x49,0x49,0x36}, /* 56 8     */
    {0x06,0x49,0x49,0x29,0x1E}, /* 57 9     */
    {0x00,0x36,0x36,0x00,0x00}, /* 58 :     */
    {0x00,0x56,0x36,0x00,0x00}, /* 59 ;     */
    {0x08,0x14,0x22,0x41,0x00}, /* 60 <     */
    {0x14,0x14,0x14,0x14,0x14}, /* 61 =     */
    {0x00,0x41,0x22,0x14,0x08}, /* 62 >     */
    {0x02,0x01,0x51,0x09,0x06}, /* 63 ?     */
    {0x32,0x49,0x79,0x41,0x3E}, /* 64 @     */
    {0x7E,0x11,0x11,0x11,0x7E}, /* 65 A     */
    {0x7F,0x49,0x49,0x49,0x36}, /* 66 B     */
    {0x3E,0x41,0x41,0x41,0x22}, /* 67 C     */
    {0x7F,0x41,0x41,0x22,0x1C}, /* 68 D     */
    {0x7F,0x49,0x49,0x49,0x41}, /* 69 E     */
    {0x7F,0x09,0x09,0x09,0x01}, /* 70 F     */
    {0x3E,0x41,0x49,0x49,0x7A}, /* 71 G     */
    {0x7F,0x08,0x08,0x08,0x7F}, /* 72 H     */
    {0x00,0x41,0x7F,0x41,0x00}, /* 73 I     */
    {0x20,0x40,0x41,0x3F,0x01}, /* 74 J     */
    {0x7F,0x08,0x14,0x22,0x41}, /* 75 K     */
    {0x7F,0x40,0x40,0x40,0x40}, /* 76 L     */
    {0x7F,0x02,0x0C,0x02,0x7F}, /* 77 M     */
    {0x7F,0x04,0x08,0x10,0x7F}, /* 78 N     */
    {0x3E,0x41,0x41,0x41,0x3E}, /* 79 O     */
    {0x7F,0x09,0x09,0x09,0x06}, /* 80 P     */
    {0x3E,0x41,0x51,0x21,0x5E}, /* 81 Q     */
    {0x7F,0x09,0x19,0x29,0x46}, /* 82 R     */
    {0x46,0x49,0x49,0x49,0x31}, /* 83 S     */
    {0x01,0x01,0x7F,0x01,0x01}, /* 84 T     */
    {0x3F,0x40,0x40,0x40,0x3F}, /* 85 U     */
    {0x1F,0x20,0x40,0x20,0x1F}, /* 86 V     */
    {0x3F,0x40,0x38,0x40,0x3F}, /* 87 W     */
    {0x63,0x14,0x08,0x14,0x63}, /* 88 X     */
    {0x07,0x08,0x70,0x08,0x07}, /* 89 Y     */
    {0x61,0x51,0x49,0x45,0x43}, /* 90 Z     */
    {0x00,0x7F,0x41,0x41,0x00}, /* 91 [     */
    {0x02,0x04,0x08,0x10,0x20}, /* 92 \     */
    {0x00,0x41,0x41,0x7F,0x00}, /* 93 ]     */
    {0x04,0x02,0x01,0x02,0x04}, /* 94 ^     */
    {0x40,0x40,0x40,0x40,0x40}, /* 95 _     */
    {0x00,0x01,0x02,0x04,0x00}, /* 96 `     */
    {0x20,0x54,0x54,0x54,0x78}, /* 97 a     */
    {0x7F,0x48,0x44,0x44,0x38}, /* 98 b     */
    {0x38,0x44,0x44,0x44,0x20}, /* 99 c     */
    {0x38,0x44,0x44,0x48,0x7F}, /* 100 d    */
    {0x38,0x54,0x54,0x54,0x18}, /* 101 e    */
    {0x08,0x7E,0x09,0x01,0x02}, /* 102 f    */
    {0x0C,0x52,0x52,0x52,0x3E}, /* 103 g    */
    {0x7F,0x08,0x04,0x04,0x78}, /* 104 h    */
    {0x00,0x44,0x7D,0x40,0x00}, /* 105 i    */
    {0x20,0x40,0x44,0x3D,0x00}, /* 106 j    */
    {0x7F,0x10,0x28,0x44,0x00}, /* 107 k    */
    {0x00,0x41,0x7F,0x40,0x00}, /* 108 l    */
    {0x7C,0x04,0x18,0x04,0x78}, /* 109 m    */
    {0x7C,0x08,0x04,0x04,0x78}, /* 110 n    */
    {0x38,0x44,0x44,0x44,0x38}, /* 111 o    */
    {0x7C,0x14,0x14,0x14,0x08}, /* 112 p    */
    {0x08,0x14,0x14,0x18,0x7C}, /* 113 q    */
    {0x7C,0x08,0x04,0x04,0x08}, /* 114 r    */
    {0x48,0x54,0x54,0x54,0x20}, /* 115 s    */
    {0x04,0x3F,0x44,0x40,0x20}, /* 116 t    */
    {0x3C,0x40,0x40,0x40,0x7C}, /* 117 u    */
    {0x1C,0x20,0x40,0x20,0x1C}, /* 118 v    */
    {0x3C,0x40,0x30,0x40,0x3C}, /* 119 w    */
    {0x44,0x28,0x10,0x28,0x44}, /* 120 x    */
    {0x0C,0x50,0x50,0x50,0x3C}, /* 121 y    */
    {0x44,0x64,0x54,0x4C,0x44}  /* 122 z    */
};

/* Draw a single character at pixel position (px,py) in colour col */
static void drawChar(int px, int py, char ch, Uint32 col)
{
    int col_idx, row;
    const unsigned char *glyph;
    if (ch < 32 || ch > 122) ch = '?';
    glyph = font5x7[(int)(ch - 32)];
    for (col_idx = 0; col_idx < 5; col_idx++) {
        unsigned char line = glyph[col_idx];
        for (row = 0; row < 7; row++) {
            if (line & (1 << row)) {
                sdl_rect(px + col_idx, py + row, 1, 1, col);
            }
        }
    }
}

/* Draw a null-terminated string; newlines advance y by 8 px */
static void drawText(int px, int py, const char *str, Uint32 col)
{
    int cx = px;
    while (*str) {
        if (*str == '\n') { cx = px; py += 9; }
        else { drawChar(cx, py, *str, col); cx += 6; }
        str++;
    }
}

/* Convenience: println equivalent (draws then moves y down by 9) */
static int g_textY = 0; /* tracks current y for println calls     */
static int g_textX = 0;

static void setCursor(int x, int y) { g_textX = x; g_textY = y; }

static void println(const char *str, Uint32 col)
{
    drawText(g_textX, g_textY, str, col);
    g_textY += 9;
}

/* ----------------------------------------------------------------
   Timing helpers (replaces Arduino millis() / delay())
   ---------------------------------------------------------------- */
static Uint32 millis(void)
{
    return SDL_GetTicks();
}

static void delay_ms(Uint32 ms)
{
    SDL_Delay(ms);
}

/* ----------------------------------------------------------------
   SDL event pump – returns 0 if user requested quit, 1 otherwise
   ---------------------------------------------------------------- */
static int pumpEvents(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) return 0;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return 0;
    }
    return 1;
}

static void flipScreen(void)
{
    SDL_Flip(screen);
}

static void fillScreen(Uint32 col)
{
    SDL_FillRect(screen, NULL, col);
}

/* ----------------------------------------------------------------
   DigiFont instance
   ---------------------------------------------------------------- */
static DigiFont digi;

/* ----------------------------------------------------------------
   Demo state
   ---------------------------------------------------------------- */
static char buf[20];
static char buf2[10];
static Uint32 demoTime = 10000;
static int g_running = 1;

/* ----------------------------------------------------------------
   Demo functions (translated directly from Arduino sketch)
   ---------------------------------------------------------------- */

static void style7(void)
{
    int x = 0, y = 0, hi = 9, ht = 44;
    Uint32 ms;

    fillScreen(BLACK);
    DigiFont_setSpacing(&digi, 2);

    setCursor(0, y);
    println("setSize7(24,40,7,3)", WHITE);
    y += hi;
    DigiFont_setColors3(&digi, RGBto565(0,250,250), RGBto565(0,190,190), RGBto565(0,40,40));
    DigiFont_setSize7(&digi, 24, 40, 7, 3);
    x = DigiFont_printNumber7(&digi, "23:45", x, y);
    x = DigiFont_printNumber7(&digi, "6.78'C", x + 10, y);
    y += ht; x = 0;

    setCursor(0, y);
    println("setSize7(24,40,7,2)", WHITE);
    y += hi;
    DigiFont_setColors3(&digi, RGBto565(250,50,250), RGBto565(190,20,190), RGBto565(60,0,60));
    DigiFont_setSize7(&digi, 24, 40, 7, 2);
    x = DigiFont_printNumber7(&digi, "23:45", x, y);
    x = DigiFont_printNumber7(&digi, "6.78'C", x + 10, y);
    y += ht; x = 0;

    setCursor(0, y);
    println("setSize7(24,40,7,0)", WHITE);
    y += hi;
    DigiFont_setColors2(&digi, RGBto565(250,0,0), RGBto565(80,0,0));
    DigiFont_setSize7(&digi, 24, 40, 7, 0);
    x = DigiFont_printNumber7(&digi, "23:45", x, y);
    x = DigiFont_printNumber7(&digi, "6.78'C", x + 10, y);
    y += ht; x = 0;

    setCursor(0, y);
    println("setSize7(24,40,7,1)", WHITE);
    y += hi;
    DigiFont_setSize7(&digi, 24, 40, 7, 1);
    x = DigiFont_printNumber7(&digi, "23:45", x, y);
    x = DigiFont_printNumber7(&digi, "6.78'C", x + 10, y);
    y += ht; x = 0;

    setCursor(0, y);
    println("setSize7(24,40,7,2)", WHITE);
    y += hi;
    DigiFont_setSize7(&digi, 24, 40, 7, 2);
    x = DigiFont_printNumber7(&digi, "23:45", x, y);
    x = DigiFont_printNumber7(&digi, "6.78'C", x + 10, y);
    y += ht; x = 0;

    setCursor(0, y);
    println("setSize7(24,40,7,3)", WHITE);
    y += hi;
    DigiFont_setSize7(&digi, 24, 40, 7, 3);
    x = DigiFont_printNumber7(&digi, "23:45", x, y);
    x = DigiFont_printNumber7(&digi, "6.78'C", x + 10, y);

    flipScreen();

    ms = millis();
    while (millis() - ms < demoTime) {
        if (!pumpEvents()) { g_running = 0; return; }
        delay_ms(50);
    }
}

static void style7var(void)
{
    int x = 0, y = 0, ht = 44 + 9;
    Uint32 ms;

    fillScreen(BLACK);
    DigiFont_setSpacing(&digi, 2);

    setCursor(0, y); println("setSize7(24,40,7,3)", WHITE); y += ht;
    setCursor(0, y); println("setSize7(24,40,7,2)", WHITE); y += ht;
    setCursor(0, y); println("setSize7(24,40,7,0)", WHITE); y += ht;
    setCursor(0, y); println("setSize7(24,40,7,1)", WHITE); y += ht;
    setCursor(0, y); println("setSize7(24,40,7,2)", WHITE); y += ht;
    setCursor(0, y); println("setSize7(24,40,7,3)", WHITE); y += ht;

    flipScreen();

    ms = millis();
    while (millis() - ms < demoTime) {
        long v;
        if (!pumpEvents()) { g_running = 0; return; }

        y = 9; x = 0;

        DigiFont_setColors3(&digi, RGBto565(0,250,250), RGBto565(0,190,190), RGBto565(0,40,40));
        v = (long)rand() % 60000 * (long)(rand() % 60000);
        snprintf(buf, 20, "%10ld", v); buf[6] = '.';
        DigiFont_setSize7(&digi, 24, 40, 7, 3);
        DigiFont_printNumber7(&digi, buf, x, y); y += ht;

        DigiFont_setColors3(&digi, RGBto565(250,50,250), RGBto565(200,20,200), RGBto565(40,0,40));
        v = (long)rand() % 60000 * (long)(rand() % 60000);
        snprintf(buf, 20, "%10ld", v); buf[6] = '.';
        DigiFont_setSize7(&digi, 24, 40, 7, 2);
        DigiFont_printNumber7(&digi, buf, x, y); y += ht;

        DigiFont_setColors2(&digi, RGBto565(50,250,50), RGBto565(0,40,0));
        v = (long)rand() % 60000 * (long)(rand() % 60000);
        snprintf(buf, 20, "%10ld", v); buf[6] = '.';
        DigiFont_setSize7(&digi, 24, 40, 7, 0);
        DigiFont_printNumber7(&digi, buf, x, y); y += ht;

        v = (long)rand() % 60000 * (long)(rand() % 60000);
        snprintf(buf, 20, "%10ld", v); buf[6] = '.';
        DigiFont_setSize7(&digi, 24, 40, 7, 1);
        DigiFont_printNumber7(&digi, buf, x, y); y += ht;

        v = (long)rand() % 60000 * (long)(rand() % 60000);
        snprintf(buf, 20, "%10ld", v); buf[6] = '.';
        DigiFont_setSize7(&digi, 24, 40, 7, 2);
        DigiFont_printNumber7(&digi, buf, x, y); y += ht;

        v = (long)rand() % 60000 * (long)(rand() % 60000);
        snprintf(buf, 20, "%10ld", v); buf[6] = '.';
        DigiFont_setSize7(&digi, 24, 40, 7, 3);
        DigiFont_printNumber7(&digi, buf, x, y); y += ht;

        flipScreen();
        delay_ms(500);
    }
}

static void all(void)
{
    int x = 0, y = 0, hi = 10, ht = 45;
    Uint32 ms;

    fillScreen(BLACK);
    DigiFont_setSpacing(&digi, 2);

    snprintf(buf,  20, "23:45");
    snprintf(buf2, 10, "67.8'C");

    ms = millis();
    while (millis() - ms < demoTime * 3 / 2) {
        int h, m, t;
        if (!pumpEvents()) { g_running = 0; return; }

        x = 0; y = 0;
        fillScreen(BLACK);

        /* --- style 7, st=3 --- */
        setCursor(0, y);
        println("setSize7(24,40,7,3); printNumber7();", WHITE);
        y += hi;
        DigiFont_setColors3(&digi, RGBto565(0,250,250), RGBto565(0,190,190), RGBto565(0,40,40));
        DigiFont_setSize7(&digi, 24, 40, 7, 3);
        x = DigiFont_printNumber7(&digi, buf,  x, y);
        x = DigiFont_printNumber7(&digi, buf2, x + 10, y);
        y += ht; x = 0;

        /* --- style 7, st=2 --- */
        setCursor(0, y);
        println("setSize7(24,40,7,2); printNumber7();", WHITE);
        y += hi;
        DigiFont_setSize7(&digi, 24, 40, 7, 2);
        DigiFont_setColors2(&digi, RGBto565(250,0,0), RGBto565(80,0,0));
        x = DigiFont_printNumber7(&digi, buf,  x, y);
        x = DigiFont_printNumber7(&digi, buf2, x + 10, y);
        y += ht; x = 0;

        /* --- style F --- */
        setCursor(0, y);
        println("setSizeF(24,40,6); printNumberF();", WHITE);
        y += hi;
        DigiFont_setColors2(&digi, RGBto565(0,250,0), RGBto565(0,40,0));
        DigiFont_setSizeF(&digi, 24, 40, 6);
        x = DigiFont_printNumberF(&digi, buf,  x, y);
        x = DigiFont_printNumberF(&digi, buf2, x + 10, y);
        y += ht; x = 0;

        /* --- style O --- */
        setCursor(0, y);
        println("setSizeO(24,40,6); printNumberO();", WHITE);
        y += hi;
        DigiFont_setColors2(&digi, RGBto565(250,250,0), BLACK);
        DigiFont_setSizeO(&digi, 24, 40, 6);
        x = DigiFont_printNumberO(&digi, buf,  x, y);
        x = DigiFont_printNumberO(&digi, buf2, x + 10, y);
        y += ht; x = 0;

        /* --- style O + F overlay --- */
        setCursor(0, y);
        println("setSizeO/F(24,40,6); printNumberF/O();", WHITE);
        y += hi;
        DigiFont_setSizeO(&digi, 24, 40, 6);
        DigiFont_setColors2(&digi, RGBto565(0,140,0), BLACK);
        x = DigiFont_printNumberF(&digi, buf,  x, y);
        x = DigiFont_printNumberF(&digi, buf2, x + 10, y);
        DigiFont_setClearBg(&digi, 0);
        DigiFont_setColors2(&digi, RGBto565(160,230,0), BLACK);
        x = 0;
        x = DigiFont_printNumberO(&digi, buf,  x, y);
        x = DigiFont_printNumberO(&digi, buf2, x + 10, y);
        DigiFont_setClearBg(&digi, 1);
        y += ht; x = 0;

        /* --- style Fr --- */
        setCursor(0, y);
        println("setSizeF(16,24,4); printNumberFr();", WHITE);
        y += hi;
        y += 5; x = 5;
        DigiFont_setFrame(&digi, 5, 5);
        DigiFont_setColors2(&digi, RGBto565(200,200,200), RGBto565(50,50,50));
        DigiFont_setSizeF(&digi, 16, 24, 4);
        x = DigiFont_printNumberFr(&digi, buf,  x, y);
        x = DigiFont_printNumberFr(&digi, buf2, x + 20, y);

        /* Randomise next frame's strings */
        h = rand() % 24;
        m = rand() % 60;
        t = rand() % 1000;
        snprintf(buf,  20, "%02d:%02d", h, m);
        snprintf(buf2, 10, "%3d.'C",    t);
        /* shift decimal: "XYZ.'C" -> "XY.Z'C" */
        buf2[3] = buf2[2]; buf2[2] = '.';

        flipScreen();

        /* Wait until at least half the demoTime has elapsed before
           moving on, matching the original while(millis()-ms<...) */
        while (millis() - ms < demoTime / 2) {
            if (!pumpEvents()) { g_running = 0; return; }
            delay_ms(10);
        }
        delay_ms(800);
    }
}

/* ----------------------------------------------------------------
   Entry point
   ---------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    srand((unsigned int)time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    screen = SDL_SetVideoMode(SCR_WD, SCR_HT, 32, SDL_SWSURFACE);
    if (!screen) {
        fprintf(stderr, "SDL_SetVideoMode failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_WM_SetCaption("DigiFont Demo", NULL);

    /* Initialise DigiFont */
    DigiFont_init(&digi, customLineH, customLineV, customRect);

    /* Main demo loop (mirrors Arduino loop()) */
    while (g_running) {
        if (!pumpEvents()) break;
        all();
        if (!g_running) break;
        style7();
        if (!g_running) break;
        style7var();
    }

    SDL_Quit();
    return 0;
}