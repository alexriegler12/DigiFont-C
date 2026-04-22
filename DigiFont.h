/* 7-segment and outline font rendering library */
/* (c) 2020-24 by Pawel A. Hernik */
/* ANSI C89 conversion */

#ifndef DIGI_FONT_H
#define DIGI_FONT_H

/* ---------------------------------
   DigiFont context structure
   --------------------------------- */
typedef struct {
    void (*linehFun)(int x0, int x1, int y, int c);
    void (*linevFun)(int x, int y0, int y1, int c);
    void (*rectFun)(int x, int y, int w, int h, int c);

    int digWd, digHt;
    int segWd, segHt;
    int segThick, segSt;
    int colOn, colOn2, colOff;
    int spacing;
    int clearBg;
    int frWd, frHt;
} DigiFont;

extern unsigned char DigiFont_digits[16];

/* Constructor / init */
void DigiFont_init(DigiFont *df,
                   void (*linehFun)(int x0, int x1, int y, int c),
                   void (*linevFun)(int x, int y0, int y1, int c),
                   void (*rectFun)(int x, int y, int w, int h, int c));

/* Segment draw methods */
void DigiFont_drawSeg1(DigiFont *df, int seg, int x, int y, int c);
void DigiFont_drawSeg2(DigiFont *df, int seg, int x, int y, int c);
void DigiFont_drawSeg2c(DigiFont *df, int seg, int x, int y, int c);
void DigiFont_drawSeg3(DigiFont *df, int seg, int x, int y, int c);
void DigiFont_drawSeg7(DigiFont *df, int seg, int x, int y, int c);

/* Digit draw methods */
int DigiFont_drawDigit1(DigiFont *df, int ch, int x, int y);
int DigiFont_drawDigit2(DigiFont *df, int ch, int x, int y);
int DigiFont_drawDigit2c(DigiFont *df, int ch, int x, int y);
int DigiFont_drawDigit3(DigiFont *df, int ch, int x, int y);
int DigiFont_drawDigitF(DigiFont *df, int ch, int x, int y);
int DigiFont_drawDigitO(DigiFont *df, int ch, int x, int y);
int DigiFont_drawDigit7(DigiFont *df, int ch, int x, int y);

/* Aliases matching original style 4/5 */
#define DigiFont_drawDigit4(df, ch, x, y) DigiFont_drawDigitF(df, ch, x, y)
#define DigiFont_drawDigit5(df, ch, x, y) DigiFont_drawDigitO(df, ch, x, y)

/* Print number methods */
int DigiFont_printNumber1(DigiFont *df, char *txt, int x, int y);
int DigiFont_printNumber2(DigiFont *df, char *txt, int x, int y);
int DigiFont_printNumber2c(DigiFont *df, char *txt, int x, int y);
int DigiFont_printNumber3(DigiFont *df, char *txt, int x, int y);
int DigiFont_printNumberF(DigiFont *df, char *txt, int x, int y);
int DigiFont_printNumberFr(DigiFont *df, char *txt, int x, int y);
int DigiFont_printNumberO(DigiFont *df, char *txt, int x, int y);
int DigiFont_printNumber7(DigiFont *df, char *txt, int x, int y);

/* Aliases matching original style 4/5 */
#define DigiFont_printNumber4(df, txt, x, y) DigiFont_printNumberF(df, txt, x, y)
#define DigiFont_printNumber5(df, txt, x, y) DigiFont_printNumberO(df, txt, x, y)

/* Clear methods */
void DigiFont_clearFO(DigiFont *df, int ch, int x, int y);
void DigiFont_clearField(DigiFont *df, int ch, int x, int y);

/* Size setters */
void DigiFont_setSize1(DigiFont *df, int wd, int ht, int th);
void DigiFont_setSize2(DigiFont *df, int wd, int ht, int th);
void DigiFont_setSize3(DigiFont *df, int wd, int ht, int th);
void DigiFont_setSizeF(DigiFont *df, int wd, int ht, int th);
void DigiFont_setSizeO(DigiFont *df, int wd, int ht, int th);
void DigiFont_setSize7(DigiFont *df, int wd, int ht, int th, int st);
void DigiFont_setFrame(DigiFont *df, int wd, int ht);

/* Aliases matching original style 4/5 */
#define DigiFont_setSize2c(df, wd, ht, th) DigiFont_setSize2(df, wd, ht, th)
#define DigiFont_setSize4(df, wd, ht, th)  DigiFont_setSizeF(df, wd, ht, th)
#define DigiFont_setSize5(df, wd, ht, th)  DigiFont_setSizeO(df, wd, ht, th)

/* Common settings */
void DigiFont_setSegment(DigiFont *df, int wd, int ht, int th);

#define DigiFont_setSpacing(df, sp)     ((df)->spacing = (sp))
#define DigiFont_setClearBg(df, cl)     ((df)->clearBg = (cl))
#define DigiFont_setColors2(df, c1, c0) ((df)->colOn = (df)->colOn2 = (c1), (df)->colOff = (c0))
#define DigiFont_setColors3(df, c1, c2, c0) ((df)->colOn = (c1), (df)->colOn2 = (c2), (df)->colOff = (c0))
#define DigiFont_getWidth(df)           ((df)->digWd)
#define DigiFont_getHeight(df)          ((df)->digHt)

int  DigiFont_getNumberWidth(DigiFont *df, char *txt);
void DigiFont_rect(DigiFont *df, int x, int y, int w, int h, int col);

#endif /* DIGI_FONT_H */