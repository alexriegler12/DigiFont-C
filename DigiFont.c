/* 7-segment and outline font rendering library */
/* (c) 2020-24 by Pawel A. Hernik */
/* ANSI C89 conversion */

#include "DigiFont.h"

/* ---------------------------------------------------------------- */
/* 0123456789AbCdEF */
unsigned char DigiFont_digits[16] = {
    0x3f, /* 0b0111111 */
    0x06, /* 0b0000110 */
    0x5b, /* 0b1011011 */
    0x4f, /* 0b1001111 */
    0x66, /* 0b1100110 */
    0x6d, /* 0b1101101 */
    0x7d, /* 0b1111101 */
    0x07, /* 0b0000111 */
    0x7f, /* 0b1111111 */
    0x6f, /* 0b1101111 */
    0x77, /* 0b1110111 */
    0x7c, /* 0b1111100 */
    0x39, /* 0b0111001 */
    0x5e, /* 0b1011110 */
    0x79, /* 0b1111001 */
    0x71  /* 0b1110001 */
};

/* ---------------------------------------------------------------- */
void DigiFont_init(DigiFont *df,
                   void (*linehFun)(int x0, int x1, int y, int c),
                   void (*linevFun)(int x, int y0, int y1, int c),
                   void (*rectFun)(int x, int y, int w, int h, int c))
{
    df->linehFun = linehFun;
    df->linevFun = linevFun;
    df->rectFun  = rectFun;
    DigiFont_setSize1(df, 10, 20, 5);
    DigiFont_setColors2(df, 1, 0);
    df->spacing  = 1;
    df->clearBg  = 1;
    df->frWd     = 4;
    df->frHt     = 4;
}

/* ---------------------------------------------------------------- */
void DigiFont_setFrame(DigiFont *df, int wd, int ht)
{
    df->frWd = wd;
    df->frHt = ht;
}

/* ---------------------------------------------------------------- */
void DigiFont_setSize1(DigiFont *df, int wd, int ht, int th)
{
    df->digWd    = wd;
    df->digHt    = ht;
    df->segThick = th;
    df->segWd    = wd - 2;
    df->segHt    = (ht - 3) / 2;
}

/* segThick should be an odd value for the best appearance */
void DigiFont_setSize2(DigiFont *df, int wd, int ht, int th)
{
    df->digWd    = wd;
    df->digHt    = ht;
    df->segThick = th;
    df->segWd    = wd - th / 2 - th / 2 - 2;
    df->segHt    = (ht - th - 2) / 2;
}

void DigiFont_setSize3(DigiFont *df, int wd, int ht, int th)
{
    df->digWd    = wd;
    df->digHt    = ht;
    df->segThick = th;
    df->segWd    = wd - th * 2 - 2;
    df->segHt    = (ht - th / 2 - th / 2) / 2 - 1;
}

/* rectangular style */
void DigiFont_setSizeF(DigiFont *df, int wd, int ht, int th)
{
    df->digWd    = wd;
    df->digHt    = ht;
    df->segThick = th;
    df->segWd    = wd;
    df->segHt    = (ht - th * 3) / 2;
}

/* outline style */
void DigiFont_setSizeO(DigiFont *df, int wd, int ht, int th)
{
    df->digWd    = wd;
    df->digHt    = ht;
    df->segThick = th;
    df->segWd    = wd;
    df->segHt    = (ht - th * 3) / 2;
}

/*
 * Style #7 can replace styles #1 and #2
 * segThick value should be an odd value for the best appearance
 * segSt should be from 0 to int(segThick/2) for the best results
 * for example for segThick=7 valid segSt is 0,1,2,3
 */
void DigiFont_setSize7(DigiFont *df, int wd, int ht, int th, int st)
{
    df->digWd    = wd;
    df->digHt    = ht;
    df->segThick = th;
    df->segSt    = st;
    df->segWd    = wd - (st + 1) * 2;
    df->segHt    = (ht - st * 2 - 3) / 2;
}

/* ---------------------------------------------------------------- */
void DigiFont_setSegment(DigiFont *df, int wd, int ht, int th)
{
    df->segThick = th;
    df->segWd    = wd;
    df->segHt    = ht;
}

/* ---------------------------------------------------------------- */
void DigiFont_drawSeg1(DigiFont *df, int seg, int x, int y, int c)
{
    int i, ofs;
    if (!c && !df->clearBg) return;
    ofs = df->segThick / 2;
    switch (seg) {
        case 0: /* top */
            for (i = 0; i < df->segThick; i++)
                (*df->linehFun)(x + i, x + df->segWd - 1 - i, y + i,
                                c ? df->colOn : df->colOff);
            break;
        case 3: /* bottom */
            for (i = 0; i < df->segThick; i++)
                (*df->linehFun)(x + i, x + df->segWd - 1 - i,
                                y + df->segThick - 1 - i,
                                c ? df->colOn : df->colOff);
            break;
        case 6: /* middle */
            for (i = 0; i < 1 + ofs; i++)
                (*df->linehFun)(x + i, x + df->segWd - 1 - i, y + ofs - i,
                                c ? df->colOn : df->colOff);
            for (i = 1; i < 1 + ofs; i++)
                (*df->linehFun)(x + i, x + df->segWd - 1 - i, y + ofs + i,
                                c ? df->colOn : df->colOff);
            break;
        case 4: /* left */
        case 5: /* left */
            for (i = 0; i < df->segThick; i++)
                (*df->linevFun)(x + i, y + i, y + df->segHt - 1 - i,
                                c ? df->colOn : df->colOff);
            break;
        case 1: /* right */
        case 2: /* right */
            for (i = 0; i < df->segThick; i++)
                (*df->linevFun)(x + df->segThick - 1 - i, y + i,
                                y + df->segHt - 1 - i,
                                c ? df->colOn : df->colOff);
            break;
    }
}

/* ---------------------------------------------------------------- */
void DigiFont_drawSeg2(DigiFont *df, int seg, int x, int y, int c)
{
    int i, ofs;
    if (!c && !df->clearBg) return;
    ofs = df->segThick / 2;
    switch (seg) {
        case 0: /* top */
        case 3: /* bottom */
        case 6: /* middle */
            for (i = 0; i < ofs + 1; i++)
                (*df->linehFun)(x + i, x + df->segWd - 1 - i, y + ofs - i,
                                c ? df->colOn : df->colOff);
            for (i = 1; i < ofs + 1; i++)
                (*df->linehFun)(x + i, x + df->segWd - 1 - i, y + ofs + i,
                                c ? df->colOn : df->colOff);
            break;
        case 1: /* right */
        case 2: /* right */
        case 4: /* left */
        case 5: /* left */
            for (i = 0; i < ofs + 1; i++)
                (*df->linevFun)(x + ofs - i, y + i, y + df->segHt - 1 - i,
                                c ? df->colOn : df->colOff);
            for (i = 1; i < ofs + 1; i++)
                (*df->linevFun)(x + ofs + i, y + i, y + df->segHt - 1 - i,
                                c ? df->colOn : df->colOff);
            break;
    }
}

/* ---------------------------------------------------------------- */
void DigiFont_drawSeg2c(DigiFont *df, int seg, int x, int y, int c)
{
    int i, ofs;
    if (!c && !df->clearBg) return;
    ofs = df->segThick / 2;
    switch (seg) {
        case 0: /* top */
        case 3: /* bottom */
        case 6: /* middle */
            for (i = 0; i < ofs + 1; i++)
                (*df->linehFun)(x + i, x + df->segWd - 1 - i, y + ofs - i,
                                c ? df->colOn  : df->colOff);
            for (i = 1; i < ofs + 1; i++)
                (*df->linehFun)(x + i, x + df->segWd - 1 - i, y + ofs + i,
                                c ? df->colOn2 : df->colOff);
            break;
        case 1: /* right */
        case 2: /* right */
        case 4: /* left */
        case 5: /* left */
            for (i = 0; i < ofs + 1; i++)
                (*df->linevFun)(x + ofs - i, y + i, y + df->segHt - 1 - i,
                                c ? df->colOn  : df->colOff);
            for (i = 1; i < ofs + 1; i++)
                (*df->linevFun)(x + ofs + i, y + i, y + df->segHt - 1 - i,
                                c ? df->colOn2 : df->colOff);
            break;
    }
}

/* ---------------------------------------------------------------- */
void DigiFont_drawSeg3(DigiFont *df, int seg, int x, int y, int c)
{
    if (!c && !df->clearBg) return;
    switch (seg) {
        case 0: /* top */
        case 3: /* bottom */
        case 6: /* middle */
            (*df->rectFun)(x, y, df->segWd, df->segThick,
                           c ? df->colOn : df->colOff);
            break;
        case 1: /* right */
        case 2: /* right */
        case 4: /* left */
        case 5: /* left */
            (*df->rectFun)(x, y, df->segThick, df->segHt,
                           c ? df->colOn : df->colOff);
            break;
    }
}

/* ---------------------------------------------------------------- */
void DigiFont_drawSeg7(DigiFont *df, int seg, int x, int y, int c)
{
    int i, ofs;
    if (!c && !df->clearBg) return;
    ofs = df->segThick / 2;
    switch (seg) {
        case 0: /* top */
            for (i = 0; i < df->segThick; i++) {
                int d = (i < df->segSt) ? df->segSt - i : i - df->segSt;
                (*df->linehFun)(x + d, x + df->segWd - 1 - d, y + i,
                                c ? (i <= df->segSt ? df->colOn : df->colOn2) : df->colOff);
            }
            break;
        case 3: /* bottom */
            for (i = 0; i < df->segThick; i++) {
                int d = (i < df->segSt) ? df->segSt - i : i - df->segSt;
                (*df->linehFun)(x + d, x + df->segWd - 1 - d,
                                y + df->segThick - 1 - i,
                                c ? (i < df->segSt ? df->colOn2 : df->colOn) : df->colOff);
            }
            break;
        case 6: /* middle */
            for (i = 0; i < ofs + 1; i++)
                (*df->linehFun)(x + i, x + df->segWd - 1 - i, y - i,
                                c ? df->colOn : df->colOff);
            for (i = 1; i < ofs + 1; i++)
                (*df->linehFun)(x + i, x + df->segWd - 1 - i, y + i,
                                c ? df->colOn2 : df->colOff);
            break;
        case 1: /* right */
        case 2: /* right */
            for (i = 0; i < df->segThick; i++) {
                int d = (i < df->segSt) ? df->segSt - i : i - df->segSt;
                (*df->linevFun)(x + df->segThick - 1 - i,
                                y + d, y + df->segHt - 1 - d,
                                c ? (i < df->segSt ? df->colOn2 : df->colOn) : df->colOff);
            }
            break;
        case 4: /* left */
        case 5: /* left */
            for (i = 0; i < df->segThick; i++) {
                int d = (i < df->segSt) ? df->segSt - i : i - df->segSt;
                (*df->linevFun)(x + i,
                                y + d, y + df->segHt - 1 - d,
                                c ? (i <= df->segSt ? df->colOn : df->colOn2) : df->colOff);
            }
            break;
    }
}

/*
 Segment bits:
   00
  5  1
  5  1
   66
  4  2
  4  2
   33
*/

/* ---------------------------------------------------------------- */
int DigiFont_drawDigit1(DigiFont *df, int ch, int x, int y)
{
    int s;
    switch (ch) {
        case '.':
            (*df->rectFun)(x, y + df->segHt + df->segHt + 3 - df->segThick,
                           df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case '\'':
            (*df->rectFun)(x, y, df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ':':
            (*df->rectFun)(x, y + 2 + (df->segHt - df->segThick) / 2,
                           df->segThick, df->segThick, df->colOn);
            (*df->rectFun)(x, y + df->segHt + 1 + (df->segHt - df->segThick) / 2,
                           df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ' ':
            s = 0; break;
        case '-':
            s = 0x40; break; /* 0b1000000 */
        default:
            ch = (ch >= 'A') ? ch - 'A' + 10 : ch;
            s = DigiFont_digits[ch & 0xf];
            break;
    }
    DigiFont_drawSeg1(df, 0, x + 1,                    y + 0,                              s & 0x01);
    DigiFont_drawSeg1(df, 1, x + df->segWd + 2 - df->segThick, y + 1,                      s & 0x02);
    DigiFont_drawSeg1(df, 2, x + df->segWd + 2 - df->segThick, y + df->segHt + 2,          s & 0x04);
    DigiFont_drawSeg1(df, 3, x + 1,  y + df->segHt + 2 + df->segHt - df->segThick + 1,     s & 0x08);
    DigiFont_drawSeg1(df, 4, x + 0,  y + df->segHt + 2,                                    s & 0x10);
    DigiFont_drawSeg1(df, 5, x + 0,  y + 1,                                                s & 0x20);
    DigiFont_drawSeg1(df, 6, x + 1,  y + df->segHt - df->segThick / 2 + 1,                 s & 0x40);
    return df->digWd;
}

/* ---------------------------------------------------------------- */
int DigiFont_drawDigit2(DigiFont *df, int ch, int x, int y)
{
    int s, ofs;
    ofs = 1 + df->segThick / 2;
    switch (ch) {
        case '.':
            (*df->rectFun)(x, y + df->segHt + df->segHt + 2,
                           df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case '\'':
            (*df->rectFun)(x, y, df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ':':
            (*df->rectFun)(x, y + 1 + ofs + (df->segHt - df->segThick) / 2,
                           df->segThick, df->segThick, df->colOn);
            (*df->rectFun)(x, y + df->segHt + 1 + ofs + (df->segHt - df->segThick) / 2,
                           df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ' ':
            s = 0; break;
        case '-':
            s = 0x40; break;
        default:
            ch = (ch >= 'A') ? ch - 'A' + 10 : ch;
            s = DigiFont_digits[ch & 0xf];
            break;
    }
    DigiFont_drawSeg2(df, 0, x + ofs,          y + 0,                    s & 0x01);
    DigiFont_drawSeg2(df, 1, x + df->segWd + 1, y + ofs,                 s & 0x02);
    DigiFont_drawSeg2(df, 2, x + df->segWd + 1, y + ofs + df->segHt + 1, s & 0x04);
    DigiFont_drawSeg2(df, 3, x + ofs,           y + df->segHt + df->segHt + 2, s & 0x08);
    DigiFont_drawSeg2(df, 4, x + 0,             y + ofs + df->segHt + 1, s & 0x10);
    DigiFont_drawSeg2(df, 5, x + 0,             y + ofs,                 s & 0x20);
    DigiFont_drawSeg2(df, 6, x + ofs,           y + df->segHt + 1,       s & 0x40);
    return df->digWd;
}

/* ---------------------------------------------------------------- */
int DigiFont_drawDigit2c(DigiFont *df, int ch, int x, int y)
{
    int s, ofs;
    ofs = 1 + df->segThick / 2;
    switch (ch) {
        case '.':
            (*df->rectFun)(x, y + df->segHt + df->segHt + 2,
                           df->segThick, df->segThick, df->colOn);
            (*df->rectFun)(x, y + df->segHt + df->segHt + 2 + df->segThick / 2,
                           df->segThick, df->segThick - df->segThick / 2, df->colOn2);
            return df->segThick;
        case '\'':
            (*df->rectFun)(x, y, df->segThick, df->segThick / 2, df->colOn);
            (*df->rectFun)(x, y + df->segThick / 2,
                           df->segThick, df->segThick / 2, df->colOn2);
            return df->segThick;
        case ':':
            (*df->rectFun)(x, y + ofs + (df->segHt - df->segThick) / 2,
                           df->segThick, df->segThick / 2, df->colOn);
            (*df->rectFun)(x, y + ofs + (df->segHt - df->segThick) / 2 + df->segThick / 2,
                           df->segThick, df->segThick - df->segThick / 2, df->colOn2);
            (*df->rectFun)(x, y + df->segHt + 1 + ofs + (df->segHt - df->segThick) / 2,
                           df->segThick, df->segThick, df->colOn);
            (*df->rectFun)(x, y + df->segHt + 1 + ofs + (df->segHt - df->segThick) / 2 + df->segThick / 2,
                           df->segThick, df->segThick - df->segThick / 2, df->colOn2);
            return df->segThick;
        case ' ':
            s = 0; break;
        case '-':
            s = 0x40; break;
        default:
            ch = (ch >= 'A') ? ch - 'A' + 10 : ch;
            s = DigiFont_digits[ch & 0xf];
            break;
    }
    DigiFont_drawSeg2c(df, 0, x + ofs,           y + 0,                    s & 0x01);
    DigiFont_drawSeg2c(df, 1, x + df->segWd + 1,  y + ofs,                 s & 0x02);
    DigiFont_drawSeg2c(df, 2, x + df->segWd + 1,  y + ofs + df->segHt + 1, s & 0x04);
    DigiFont_drawSeg2c(df, 3, x + ofs,            y + df->segHt + df->segHt + 2, s & 0x08);
    DigiFont_drawSeg2c(df, 4, x + 0,              y + ofs + df->segHt + 1, s & 0x10);
    DigiFont_drawSeg2c(df, 5, x + 0,              y + ofs,                 s & 0x20);
    DigiFont_drawSeg2c(df, 6, x + ofs,            y + df->segHt + 1,       s & 0x40);
    return df->digWd;
}

/* ---------------------------------------------------------------- */
int DigiFont_drawDigit3(DigiFont *df, int ch, int x, int y)
{
    int s, ofs;
    ofs = df->segThick / 2;
    switch (ch) {
        case '.':
            (*df->rectFun)(x, y + df->digHt - 1 - df->segThick,
                           df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case '\'':
            (*df->rectFun)(x, y, df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ':':
            (*df->rectFun)(x, y + (df->digHt / 2 - df->segThick) / 2 + 1,
                           df->segThick, df->segThick, df->colOn);
            (*df->rectFun)(x,
                           y + df->digHt - 1 - (df->digHt / 2 - df->segThick) / 2 - df->segThick - 1,
                           df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ' ':
            s = 0; break;
        case '-':
            s = 0x40; break;
        default:
            ch = (ch >= 'A') ? ch - 'A' + 10 : ch;
            s = DigiFont_digits[ch & 0xf];
            break;
    }
    DigiFont_drawSeg3(df, 0, x + df->segThick + 1,       y + 0,                               s & 0x01);
    DigiFont_drawSeg3(df, 1, x + df->segThick + df->segWd + 2, y + ofs,                        s & 0x02);
    DigiFont_drawSeg3(df, 2, x + df->segThick + df->segWd + 2, y + df->digHt - 1 - ofs - df->segHt, s & 0x04);
    DigiFont_drawSeg3(df, 3, x + df->segThick + 1,       y + df->digHt - 1 - df->segThick,    s & 0x08);
    DigiFont_drawSeg3(df, 4, x + 0,                      y + df->digHt - 1 - ofs - df->segHt, s & 0x10);
    DigiFont_drawSeg3(df, 5, x + 0,                      y + ofs,                             s & 0x20);
    DigiFont_drawSeg3(df, 6, x + df->segThick + 1,       y + (df->digHt - df->segThick) / 2,  s & 0x40);
    return df->digWd;
}

/* ---------------------------------------------------------------- */
int DigiFont_drawDigit7(DigiFont *df, int ch, int x, int y)
{
    int s, offs;
    offs = df->segSt + 1;
    switch (ch) {
        case '.':
            (*df->rectFun)(x,
                           y + df->segHt + df->segHt + offs - df->segThick + df->segSt + 2,
                           df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case '\'':
            (*df->rectFun)(x, y, df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ':':
            (*df->rectFun)(x, y + offs + 1 + (df->segHt - df->segThick) / 2,
                           df->segThick, df->segThick, df->colOn);
            (*df->rectFun)(x, y + df->segHt + offs + 1 + (df->segHt - df->segThick) / 2,
                           df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ' ':
            s = 0; break;
        case '-':
            s = 0x40; break;
        default:
            ch = (ch >= 'A') ? ch - 'A' + 10 : ch;
            s = DigiFont_digits[ch & 0xf];
            break;
    }
    DigiFont_drawSeg7(df, 0, x + offs,                y,                                                         s & 0x01);
    DigiFont_drawSeg7(df, 1, x + df->digWd - df->segThick, y + offs,                                             s & 0x02);
    DigiFont_drawSeg7(df, 2, x + df->digWd - df->segThick, y + offs + df->segHt + 1,                             s & 0x04);
    DigiFont_drawSeg7(df, 3, x + offs, y + df->segHt + df->segHt + offs - df->segThick + df->segSt + 2,          s & 0x08);
    DigiFont_drawSeg7(df, 4, x,                        y + offs + df->segHt + 1,                                  s & 0x10);
    DigiFont_drawSeg7(df, 5, x,                        y + offs,                                                  s & 0x20);
    DigiFont_drawSeg7(df, 6, x + offs,                 y + df->segHt + offs,                                      s & 0x40);
    return df->digWd;
}

/* ---------------------------------------------------------------- */
void DigiFont_clearField(DigiFont *df, int ch, int x, int y)
{
    int hc = (df->digHt - df->segThick * 3) / 2;
    switch (ch) {
        case '.':
            (*df->rectFun)(x, y, df->segThick, df->digHt - df->segThick, df->colOff);
            break;
        case '\'':
            (*df->rectFun)(x, y + df->segThick, df->segThick, df->digHt - df->segThick, df->colOff);
            break;
        case ':': {
            int a = (hc + df->segThick * 2 - df->segThick) / 2;
            int b = (df->digHt - hc - df->segThick - df->segThick) / 2;
            (*df->rectFun)(x, y,                  df->segThick, a,                                          df->colOff);
            (*df->rectFun)(x, y + a + df->segThick, df->segThick, df->digHt - b - df->segThick - (a + df->segThick), df->colOff);
            (*df->rectFun)(x, y + df->digHt - b,   df->segThick, b,                                         df->colOff);
            break;
        }
        case ' ':
            (*df->rectFun)(x, y, df->digWd - df->segThick, df->digHt, df->colOff);
            break;
        case '-':
            (*df->rectFun)(x, y,                    df->digWd, hc + df->segThick,                df->colOff);
            (*df->rectFun)(x, y + hc + df->segThick * 2, df->digWd, df->digHt - hc - df->segThick * 2, df->colOff);
            break;
        case 'C':
        case 'c':
            (*df->rectFun)(x + df->segThick, y + df->segThick,
                           df->digWd - df->segThick, df->digHt - df->segThick * 2, df->colOff);
            break;
        case '0':
            (*df->rectFun)(x + df->segThick, y + df->segThick,
                           df->digWd - df->segThick * 2, df->digHt - df->segThick * 2, df->colOff);
            break;
        case '1':
            (*df->rectFun)(x, y, df->digWd - df->segThick, df->digHt, df->colOff);
            break;
        case '2':
            (*df->rectFun)(x,               y + df->segThick,         df->digWd - df->segThick, hc,                           df->colOff);
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick * 2, df->digWd - df->segThick, df->digHt - hc - df->segThick * 3, df->colOff);
            break;
        case '3':
            (*df->rectFun)(x, y + df->segThick,       df->digWd - df->segThick, hc,                           df->colOff);
            (*df->rectFun)(x, y + hc + df->segThick,  df->digWd / 3,            df->segThick,                 df->colOff);
            (*df->rectFun)(x, y + hc + df->segThick * 2, df->digWd - df->segThick, df->digHt - hc - df->segThick * 3, df->colOff);
            break;
        case '4':
            (*df->rectFun)(x + df->segThick, y,                  df->digWd - df->segThick * 2, hc + df->segThick,           df->colOff);
            (*df->rectFun)(x,               y + hc + df->segThick * 2, df->digWd - df->segThick, df->digHt - hc - df->segThick * 2, df->colOff);
            break;
        case '5':
            (*df->rectFun)(x + df->segThick, y + df->segThick,       df->digWd - df->segThick, hc,                           df->colOff);
            (*df->rectFun)(x,               y + hc + df->segThick * 2, df->digWd - df->segThick, df->digHt - hc - df->segThick * 3, df->colOff);
            break;
        case '6':
            (*df->rectFun)(x + df->segThick, y + df->segThick,         df->digWd - df->segThick,     hc,                           df->colOff);
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick * 2, df->digWd - df->segThick * 2, df->digHt - hc - df->segThick * 3, df->colOff);
            break;
        case '7':
            (*df->rectFun)(x, y + df->segThick, df->digWd - df->segThick, df->digHt - df->segThick, df->colOff);
            break;
        case '8':
            (*df->rectFun)(x + df->segThick, y + df->segThick,         df->digWd - df->segThick * 2, hc,                           df->colOff);
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick * 2, df->digWd - df->segThick * 2, df->digHt - hc - df->segThick * 3, df->colOff);
            break;
        case '9':
            (*df->rectFun)(x + df->segThick, y + df->segThick,       df->digWd - df->segThick * 2, hc,                           df->colOff);
            (*df->rectFun)(x,               y + hc + df->segThick * 2, df->digWd - df->segThick,   df->digHt - hc - df->segThick * 3, df->colOff);
            break;
    }
}

/* ---------------------------------------------------------------- */
void DigiFont_clearFO(DigiFont *df, int ch, int x, int y)
{
    int hc = (df->digHt - df->segThick * 3) / 2;
    switch (ch) {
        case ' ':
            (*df->rectFun)(x,                    y, df->segThick,              df->digHt,          df->colOff);
            (*df->rectFun)(x + df->digWd - df->segThick, y, df->segThick,      df->digHt,          df->colOff);
            (*df->rectFun)(x + df->segThick, y,              df->digWd - df->segThick * 2, df->segThick, df->colOff);
            (*df->rectFun)(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOff);
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick,        df->digWd - df->segThick * 2, df->segThick, df->colOff);
            /* fall through */
        case '-':
            (*df->rectFun)(x, y,                      df->digWd, hc + df->segThick,                df->colOff);
            (*df->rectFun)(x, y + hc + df->segThick * 2, df->digWd, df->digHt - hc - df->segThick * 2, df->colOff);
            break;
        case 'C':
        case 'c':
            (*df->rectFun)(x + df->segThick,       y + hc + df->segThick,    df->digWd - df->segThick * 2, df->segThick, df->colOff);
            (*df->rectFun)(x + df->digWd - df->segThick, y + df->segThick,   df->segThick, df->digHt - df->segThick * 2, df->colOff);
            break;
        case '0':
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOff);
            break;
        case '1':
            (*df->rectFun)(x,               y,                   df->segThick,              df->digHt,          df->colOff);
            (*df->rectFun)(x + df->segThick, y,                   df->digWd - df->segThick * 2, df->segThick,   df->colOff);
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick,   df->digWd - df->segThick * 2, df->segThick, df->colOff);
            (*df->rectFun)(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOff);
            break;
        case '2':
            (*df->rectFun)(x,                    y + df->segThick,        df->segThick,              hc,                           df->colOff);
            (*df->rectFun)(x + df->digWd - df->segThick, y + df->segThick * 2 + hc, df->segThick,   df->digHt - hc - df->segThick * 3, df->colOff);
            break;
        case '3':
            (*df->rectFun)(x,               y + df->segThick,       df->segThick,          df->digHt - df->segThick * 2,         df->colOff);
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick, df->digWd / 3 - df->segThick, df->segThick,                 df->colOff);
            break;
        case '4':
            (*df->rectFun)(x,               y + df->segThick * 2 + hc, df->segThick,              df->digHt - df->segThick * 2 - hc, df->colOff);
            (*df->rectFun)(x + df->segThick, y,                         df->digWd - df->segThick * 2, df->segThick,                   df->colOff);
            (*df->rectFun)(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick * 2, df->segThick,               df->colOff);
            break;
        case '5':
            (*df->rectFun)(x + df->digWd - df->segThick, y + df->segThick,       df->segThick, hc,                           df->colOff);
            (*df->rectFun)(x,                             y + df->segThick * 2 + hc, df->segThick, df->digHt - hc - df->segThick * 3, df->colOff);
            break;
        case '6':
            (*df->rectFun)(x + df->digWd - df->segThick, y + df->segThick, df->segThick, hc, df->colOff);
            break;
        case '7':
            (*df->rectFun)(x,               y + df->segThick,           df->segThick,              df->digHt - df->segThick,   df->colOff);
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick,     df->digWd - df->segThick * 2, df->segThick,            df->colOff);
            (*df->rectFun)(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick * 2, df->segThick,         df->colOff);
            break;
        case '8':
            /* nothing */
            break;
        case '9':
            (*df->rectFun)(x, y + hc + df->segThick * 2, df->segThick, df->digHt - hc - df->segThick * 3, df->colOff);
            break;
    }
}

/* ---------------------------------------------------------------- */
int DigiFont_drawDigitF(DigiFont *df, int ch, int x, int y)
{
    int hc = (df->digHt - df->segThick * 3) / 2;
    switch (ch) {
        case '.':
            (*df->rectFun)(x, y + df->digHt - df->segThick,
                           df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case '\'':
            (*df->rectFun)(x, y, df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ':':
            (*df->rectFun)(x, y + (hc + df->segThick * 2 - df->segThick) / 2,
                           df->segThick, df->segThick, df->colOn);
            (*df->rectFun)(x, y + df->digHt - (df->digHt - hc - df->segThick - df->segThick) / 2 - df->segThick,
                           df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ' ':
            break;
        case '-':
            (*df->rectFun)(x, y + hc + df->segThick, df->digWd, df->segThick, df->colOn);
            break;
        case 'C':
        case 'c':
            (*df->rectFun)(x,               y,      df->segThick,              df->digHt,          df->colOn);
            (*df->rectFun)(x + df->segThick, y,      df->digWd - df->segThick, df->segThick,        df->colOn);
            (*df->rectFun)(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick, df->segThick, df->colOn);
            break;
        case '0':
            (*df->rectFun)(x,                    y, df->segThick,              df->digHt,          df->colOn);
            (*df->rectFun)(x + df->digWd - df->segThick, y, df->segThick,      df->digHt,          df->colOn);
            (*df->rectFun)(x + df->segThick, y,                    df->digWd - df->segThick * 2, df->segThick, df->colOn);
            (*df->rectFun)(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOn);
            break;
        case '1':
            (*df->rectFun)(x + df->digWd - df->segThick, y, df->segThick, df->digHt, df->colOn);
            break;
        case '2':
            (*df->rectFun)(x, y,                    df->digWd, df->segThick, df->colOn);
            (*df->rectFun)(x, y + hc + df->segThick,   df->digWd, df->segThick, df->colOn);
            (*df->rectFun)(x, y + df->digHt - df->segThick, df->digWd, df->segThick, df->colOn);
            (*df->rectFun)(x,               y + df->segThick * 2 + hc, df->segThick, df->digHt - hc - df->segThick * 3, df->colOn);
            (*df->rectFun)(x + df->digWd - df->segThick, y + df->segThick, df->segThick, hc, df->colOn);
            break;
        case '3':
            (*df->rectFun)(x, y,                    df->digWd - df->segThick, df->segThick, df->colOn);
            (*df->rectFun)(x + df->digWd / 3, y + hc + df->segThick, df->digWd - df->segThick - df->digWd / 3, df->segThick, df->colOn);
            (*df->rectFun)(x, y + df->digHt - df->segThick, df->digWd - df->segThick, df->segThick, df->colOn);
            (*df->rectFun)(x + df->digWd - df->segThick, y, df->segThick, df->digHt, df->colOn);
            break;
        case '4':
            (*df->rectFun)(x + df->digWd - df->segThick, y,              df->segThick,              df->digHt,           df->colOn);
            (*df->rectFun)(x,               y,              df->segThick,              hc + df->segThick * 2, df->colOn);
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick, df->digWd - df->segThick * 2, df->segThick,  df->colOn);
            break;
        case '5':
            (*df->rectFun)(x, y,                    df->digWd, df->segThick, df->colOn);
            (*df->rectFun)(x, y + hc + df->segThick,   df->digWd, df->segThick, df->colOn);
            (*df->rectFun)(x, y + df->digHt - df->segThick, df->digWd, df->segThick, df->colOn);
            (*df->rectFun)(x,               y + df->segThick,        df->segThick, hc,                           df->colOn);
            (*df->rectFun)(x + df->digWd - df->segThick, y + df->segThick * 2 + hc, df->segThick, df->digHt - hc - df->segThick * 3, df->colOn);
            break;
        case '6':
            (*df->rectFun)(x, y,                             df->digWd, df->segThick,                           df->colOn);
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick, df->digWd - df->segThick, df->segThick,     df->colOn);
            (*df->rectFun)(x, y + df->digHt - df->segThick, df->digWd, df->segThick,                            df->colOn);
            (*df->rectFun)(x,               y + df->segThick,        df->segThick, df->digHt - df->segThick * 2, df->colOn);
            (*df->rectFun)(x + df->digWd - df->segThick, y + hc + df->segThick * 2, df->segThick, df->digHt - hc - df->segThick * 3, df->colOn);
            break;
        case '7':
            (*df->rectFun)(x + df->digWd - df->segThick, y, df->segThick,              df->digHt,          df->colOn);
            (*df->rectFun)(x,               y, df->digWd - df->segThick, df->segThick,                      df->colOn);
            break;
        case '8':
            (*df->rectFun)(x,               y, df->segThick,              df->digHt,          df->colOn);
            (*df->rectFun)(x + df->digWd - df->segThick, y, df->segThick, df->digHt,          df->colOn);
            (*df->rectFun)(x + df->segThick, y,                    df->digWd - df->segThick * 2, df->segThick, df->colOn);
            (*df->rectFun)(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOn);
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick,            df->digWd - df->segThick * 2, df->segThick, df->colOn);
            break;
        case '9':
            (*df->rectFun)(x,               y,                  df->segThick,              hc + df->segThick * 2, df->colOn);
            (*df->rectFun)(x + df->digWd - df->segThick, y,     df->segThick,              df->digHt,             df->colOn);
            (*df->rectFun)(x + df->segThick, y,                  df->digWd - df->segThick * 2, df->segThick,       df->colOn);
            (*df->rectFun)(x + df->segThick, y + hc + df->segThick, df->digWd - df->segThick * 2, df->segThick,   df->colOn);
            (*df->rectFun)(x,               y + df->digHt - df->segThick, df->digWd - df->segThick, df->segThick, df->colOn);
            break;
    }
    return df->digWd;
}

/* ---------------------------------------------------------------- */
void DigiFont_rect(DigiFont *df, int x, int y, int w, int h, int col)
{
    (*df->linehFun)(x,     x + w - 1, y + h - 1, col);
    (*df->linehFun)(x,     x + w - 1, y,         col);
    (*df->linevFun)(x,     y + 1,     y + h - 2, col);
    (*df->linevFun)(x+w-1, y + 1,     y + h - 2, col);
}

/* ---------------------------------------------------------------- */
int DigiFont_drawDigitO(DigiFont *df, int ch, int x, int y)
{
    int hc = (df->digHt - df->segThick * 3) / 2;
    if (df->clearBg) {
        DigiFont_clearFO(df, ch, x, y);
        (*df->rectFun)(x + 1,                    y + 1,                     df->segThick - 1, df->segThick - 1, df->colOff);
        (*df->rectFun)(x + df->digWd - df->segThick, y + 1,                 df->segThick - 1, df->segThick - 1, df->colOff);
        (*df->rectFun)(x + df->digWd - df->segThick, y + df->digHt - df->segThick, df->segThick - 1, df->segThick - 1, df->colOff);
        (*df->rectFun)(x + df->digWd - df->segThick, y + hc + df->segThick, df->segThick - 1, df->segThick,     df->colOff);
        (*df->rectFun)(x + 1,                    y + hc + df->segThick,     df->segThick - 1, df->segThick,     df->colOff);
        (*df->rectFun)(x + 1,                    y + df->digHt - df->segThick, df->segThick - 1, df->segThick - 1, df->colOff);
        (*df->linevFun)(x + df->digWd / 3 - 1,  y + df->segThick + hc + 1, y + df->segThick + hc + 1 + df->segThick - 2, df->colOff);
        (*df->linehFun)(x + 1, x + df->segThick - 1, y + df->digHt - df->segThick, df->colOff);
    }
    switch (ch) {
        case '.':
            DigiFont_rect(df, x, y + df->digHt - df->segThick, df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case '\'':
            DigiFont_rect(df, x, y, df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ':':
            DigiFont_rect(df, x, y + (hc + df->segThick * 2 - df->segThick) / 2, df->segThick, df->segThick, df->colOn);
            DigiFont_rect(df, x, y + df->digHt - (df->digHt - hc - df->segThick - df->segThick) / 2 - df->segThick, df->segThick, df->segThick, df->colOn);
            return df->segThick;
        case ' ':
            break;
        case '-':
            DigiFont_rect(df, x, y + hc + df->segThick, df->digWd, df->segThick, df->colOn);
            break;
        case 'C':
        case 'c':
            (*df->linehFun)(x,            x + df->digWd - 1,     y,                      df->colOn);
            (*df->linehFun)(x,            x + df->digWd - 1,     y + df->digHt - 1,      df->colOn);
            (*df->linevFun)(x,            y + 1,                  y + df->digHt - 2,      df->colOn);
            (*df->linevFun)(x + df->digWd - 1, y + 1,            y + df->segThick - 2,   df->colOn);
            (*df->linevFun)(x + df->digWd - 1, y + df->digHt - df->segThick + 1, y + df->digHt - 1, df->colOn);
            (*df->linevFun)(x + df->segThick - 1, y + df->segThick, y + df->digHt - df->segThick - 1, df->colOn);
            (*df->linehFun)(x + df->segThick - 1, x + df->digWd - 1, y + df->segThick - 1, df->colOn);
            (*df->linehFun)(x + df->segThick - 1, x + df->digWd - 1, y + df->digHt - df->segThick,   df->colOn);
            break;
        case '0':
            DigiFont_rect(df, x, y, df->digWd, df->digHt, df->colOn);
            DigiFont_rect(df, x + df->segThick - 1, y + df->segThick - 1,
                          df->digWd - df->segThick * 2 + 2, df->digHt - df->segThick * 2 + 2, df->colOn);
            break;
        case '1':
            DigiFont_rect(df, x + df->digWd - df->segThick, y, df->segThick, df->digHt, df->colOn);
            break;
        case '2':
            (*df->linehFun)(x,                    x + df->digWd - 1,        y,                          df->colOn);
            (*df->linehFun)(x,                    x + df->digWd - df->segThick, y + df->segThick - 1,   df->colOn);
            (*df->linehFun)(x,                    x + df->digWd - 1,        y + df->digHt - 1,          df->colOn);
            (*df->linehFun)(x + df->segThick,     x + df->digWd - 1,        y + df->digHt - df->segThick, df->colOn);
            (*df->linehFun)(x,                    x + df->digWd - df->segThick, y + df->segThick + hc,  df->colOn);
            (*df->linehFun)(x + df->segThick,     x + df->digWd - 1,        y + df->segThick + hc + df->segThick - 1, df->colOn);
            (*df->linevFun)(x + df->digWd - 1,        y + 1,               y + hc + df->segThick * 2 - 1, df->colOn);
            (*df->linevFun)(x + df->digWd - df->segThick, y + df->segThick - 1, y + hc + df->segThick,  df->colOn);
            (*df->linevFun)(x,                         y + hc + df->segThick + 1, y + df->digHt - 2,    df->colOn);
            (*df->linevFun)(x + df->segThick - 1,      y + hc + df->segThick * 2 - 1, y + df->digHt - df->segThick, df->colOn);
            (*df->linevFun)(x,                         y + 1,               y + df->segThick - 2,        df->colOn);
            (*df->linevFun)(x + df->digWd - 1,         y + df->digHt - df->segThick + 1, y + df->digHt - 2, df->colOn);
            break;
        case '3':
            (*df->linehFun)(x,                    x + df->digWd - 1,        y,                          df->colOn);
            (*df->linehFun)(x,                    x + df->digWd - 1,        y + df->digHt - 1,          df->colOn);
            (*df->linehFun)(x,                    x + df->digWd - df->segThick, y + df->segThick - 1,   df->colOn);
            (*df->linehFun)(x,                    x + df->digWd - df->segThick, y + df->digHt - df->segThick, df->colOn);
            (*df->linehFun)(x + df->digWd / 3,   x + df->digWd - df->segThick, y + df->segThick + hc,  df->colOn);
            (*df->linehFun)(x + df->digWd / 3,   x + df->digWd - df->segThick, y + df->segThick * 2 + hc - 1, df->colOn);
            (*df->linevFun)(x + df->digWd - 1,       y + 1,                y + df->digHt - 2,                df->colOn);
            (*df->linevFun)(x,                        y + 1,                y + df->segThick - 2,             df->colOn);
            (*df->linevFun)(x,                        y + df->digHt - df->segThick + 1, y + df->digHt - 2,  df->colOn);
            (*df->linevFun)(x + df->digWd / 3 - 1,   y + df->segThick + hc,  y + df->segThick + hc + 1 + df->segThick - 2, df->colOn);
            (*df->linevFun)(x + df->digWd - df->segThick, y + df->segThick,  y + df->segThick + 1 + hc - 1,  df->colOn);
            (*df->linevFun)(x + df->digWd - df->segThick, y + df->segThick * 2 + hc, y + df->segThick * 2 + hc + 1 + hc - 1, df->colOn);
            break;
        case '4':
            (*df->linehFun)(x,                  x + df->segThick - 1,       y,                          df->colOn);
            (*df->linehFun)(x + df->digWd - df->segThick, x + df->digWd - 1, y,                         df->colOn);
            (*df->linehFun)(x + df->digWd - df->segThick, x + df->digWd - 1, y + df->digHt - 1,         df->colOn);
            (*df->linehFun)(x + df->segThick,   x + df->digWd - df->segThick, y + df->segThick + hc,    df->colOn);
            (*df->linehFun)(x,                  x + df->digWd - df->segThick, y + df->segThick + hc + df->segThick - 1, df->colOn);
            (*df->linevFun)(x + df->digWd - 1,        y + 1,            y + df->digHt - 2,              df->colOn);
            (*df->linevFun)(x + df->digWd - df->segThick, y + 1,        y + df->segThick + hc - 1,      df->colOn);
            (*df->linevFun)(x + df->digWd - df->segThick, y + hc + df->segThick * 2, y + df->digHt - 2, df->colOn);
            (*df->linevFun)(x,                         y + 1,            y + df->segThick * 2 + hc - 1,  df->colOn);
            (*df->linevFun)(x + df->segThick - 1,      y + 1,            y + df->segThick + hc,          df->colOn);
            break;
        case '5':
            (*df->linehFun)(x,                    x + df->digWd - 1,        y,                          df->colOn);
            (*df->linehFun)(x + df->segThick,     x + df->digWd - 1,        y + df->segThick - 1,       df->colOn);
            (*df->linehFun)(x,                    x + df->digWd - 1,        y + df->digHt - 1,          df->colOn);
            (*df->linehFun)(x,                    x + df->digWd - df->segThick, y + df->digHt - df->segThick, df->colOn);
            (*df->linehFun)(x + df->segThick,     x + df->digWd - 1,        y + df->segThick + hc,      df->colOn);
            (*df->linehFun)(x,                    x + df->digWd - df->segThick, y + df->segThick + hc + df->segThick - 1, df->colOn);
            (*df->linevFun)(x,                         y + 1,               y + hc + df->segThick * 2 - 1, df->colOn);
            (*df->linevFun)(x + df->segThick - 1,      y + df->segThick - 1, y + hc + df->segThick,     df->colOn);
            (*df->linevFun)(x + df->digWd - 1,         y + hc + df->segThick + 1, y + df->digHt - 2,   df->colOn);
            (*df->linevFun)(x + df->digWd - df->segThick, y + hc + df->segThick * 2, y + df->digHt - df->segThick - 1, df->colOn);
            (*df->linevFun)(x + df->digWd - 1,         y + 1,               y + df->segThick - 2,       df->colOn);
            (*df->linevFun)(x,                          y + df->digHt - df->segThick + 1, y + df->digHt - 2, df->colOn);
            break;
        case '6': {
            DigiFont_rect(df, x + df->segThick - 1, y + hc + df->segThick * 2 - 1,
                          df->digWd - df->segThick * 2 + 2, df->digHt - hc - df->segThick * 3 + 2, df->colOn);
            (*df->linehFun)(x,               x + df->digWd - 1,     y,                         df->colOn);
            (*df->linehFun)(x + df->segThick, x + df->digWd - 1,    y + df->segThick - 1,      df->colOn);
            (*df->linehFun)(x,               x + df->digWd - 1,     y + df->digHt - 1,         df->colOn);
            (*df->linehFun)(x + df->segThick, x + df->digWd - 1,    y + df->segThick + hc,     df->colOn);
            (*df->linevFun)(x,                 y + 1,               y + df->digHt - 2,          df->colOn);
            (*df->linevFun)(x + df->segThick - 1, y + df->segThick - 1, y + hc + df->segThick, df->colOn);
            (*df->linevFun)(x + df->digWd - 1, y + hc + df->segThick + 1, y + df->digHt - 2,  df->colOn);
            (*df->linevFun)(x + df->digWd - 1, y + 1,               y + df->segThick - 2,      df->colOn);
            break;
        }
        case '7':
            (*df->linehFun)(x,                    x + df->digWd - 1,            y,                           df->colOn);
            (*df->linehFun)(x + df->digWd - df->segThick, x + df->digWd - 1,    y + df->digHt - 1,           df->colOn);
            (*df->linehFun)(x,                    x + df->digWd - 1 - df->segThick, y + df->segThick - 1,    df->colOn);
            (*df->linevFun)(x + df->digWd - 1,        y + 1,                    y + df->digHt - 2,           df->colOn);
            (*df->linevFun)(x,                         y + 1,                   y + df->segThick - 2,         df->colOn);
            (*df->linevFun)(x + df->digWd - df->segThick, y + df->segThick - 1, y + df->digHt - 1,           df->colOn);
            break;
        case '8': {
            DigiFont_rect(df, x, y, df->digWd, df->digHt, df->colOn);
            DigiFont_rect(df, x + df->segThick - 1, y + df->segThick - 1,
                          df->digWd - df->segThick * 2 + 2, hc + 2, df->colOn);
            DigiFont_rect(df, x + df->segThick - 1, y + hc + df->segThick * 2 - 1,
                          df->digWd - df->segThick * 2 + 2, df->digHt - hc - df->segThick * 3 + 2, df->colOn);
            break;
        }
        case '9': {
            DigiFont_rect(df, x + df->segThick - 1, y + df->segThick - 1,
                          df->digWd - df->segThick * 2 + 2, hc + 2, df->colOn);
            (*df->linehFun)(x, x + df->digWd - 1, y,                          df->colOn);
            (*df->linehFun)(x, x + df->digWd - 1, y + df->digHt - 1,          df->colOn);
            (*df->linehFun)(x, x + df->digWd - 1 - df->segThick, y + df->segThick * 2 + hc - 1, df->colOn);
            (*df->linehFun)(x, x + df->digWd - 1 - df->segThick, y + df->digHt - df->segThick,  df->colOn);
            (*df->linevFun)(x + df->digWd - 1,         y + 1,               y + df->digHt - 2,    df->colOn);
            (*df->linevFun)(x,                          y + 1,               y + hc + df->segThick * 2 - 2, df->colOn);
            (*df->linevFun)(x,                          y + df->digHt - df->segThick + 1, y + df->digHt - 2, df->colOn);
            (*df->linevFun)(x + df->digWd - df->segThick, y + hc + df->segThick * 2 - 1, y + df->digHt - df->segThick, df->colOn);
            break;
        }
    }
    return df->digWd;
}

/* ---------------------------------------------------------------- */
int DigiFont_printNumber1(DigiFont *df, char *txt, int x, int y)
{
    while (*txt) x += DigiFont_drawDigit1(df, *txt++, x, y) + df->spacing;
    return x - df->spacing;
}

int DigiFont_printNumber2(DigiFont *df, char *txt, int x, int y)
{
    while (*txt) x += DigiFont_drawDigit2(df, *txt++, x, y) + df->spacing;
    return x - df->spacing;
}

int DigiFont_printNumber2c(DigiFont *df, char *txt, int x, int y)
{
    while (*txt) x += DigiFont_drawDigit2c(df, *txt++, x, y) + df->spacing;
    return x - df->spacing;
}

int DigiFont_printNumber3(DigiFont *df, char *txt, int x, int y)
{
    while (*txt) x += DigiFont_drawDigit3(df, *txt++, x, y) + df->spacing;
    return x - df->spacing;
}

int DigiFont_printNumberF(DigiFont *df, char *txt, int x, int y)
{
    while (*txt) {
        if (df->clearBg) DigiFont_clearFO(df, *txt, x, y);
        x += DigiFont_drawDigitF(df, *txt++, x, y) + df->spacing;
    }
    return x - df->spacing;
}

int DigiFont_printNumberFr(DigiFont *df, char *txt, int x, int y)
{
    int xf = x - df->frWd;
    while (*txt) {
        if (df->clearBg) DigiFont_clearField(df, *txt, x, y);
        x += DigiFont_drawDigitF(df, *txt++, x, y) + df->spacing;
        if (*txt && df->clearBg)
            (*df->rectFun)(x - df->spacing, y, df->spacing, df->digHt, df->colOff);
    }
    if (df->clearBg) {
        (*df->rectFun)(xf, y - df->frHt,  x - df->spacing + df->frWd - xf, df->frHt, df->colOff);
        (*df->rectFun)(xf, y + df->digHt, x - df->spacing + df->frWd - xf, df->frHt, df->colOff);
        (*df->rectFun)(xf, y,             df->frWd,                         df->digHt, df->colOff);
        (*df->rectFun)(x - df->spacing, y, df->frWd,                        df->digHt, df->colOff);
    }
    return x - df->spacing;
}

int DigiFont_printNumberO(DigiFont *df, char *txt, int x, int y)
{
    while (*txt) x += DigiFont_drawDigitO(df, *txt++, x, y) + df->spacing;
    return x - df->spacing;
}

int DigiFont_printNumber7(DigiFont *df, char *txt, int x, int y)
{
    while (*txt) x += DigiFont_drawDigit7(df, *txt++, x, y) + df->spacing;
    return x - df->spacing;
}

/* ---------------------------------------------------------------- */
int DigiFont_getNumberWidth(DigiFont *df, char *txt)
{
    int wd = 0;
    while (*txt) {
        wd += ((*txt == '.' || *txt == ':' || *txt == '\'')
               ? df->segThick : df->digWd) + df->spacing;
        txt++;
    }
    return wd - df->spacing;
}