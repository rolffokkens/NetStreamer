/*
 * file: XxColor.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_COLOR
#define H_XX_COLOR

#include "XxCore.h"

class XxGC;
class XxWindow;
class XxPixmap;

class XxColor : public XxCore {
    friend XxGC;
    friend XxWindow;
    friend XxPixmap;
private:
    char   AllocDone;
    XColor Color;
    long GetPixel (void);
    void Constr (int Red, int Green, int Blue, char AllocDone);
public:
    XxColor (void);
    XxColor (const XxColor &Color);
    XxColor (int Red, int Green, int Blue);
    XxColor &operator= (const XxColor &Color);
};

extern const XxColor XxBlack;
extern const XxColor XxGray1;
extern const XxColor XxGray2;
extern const XxColor XxGray3;
extern const XxColor XxGray4;
extern const XxColor XxGray5;
extern const XxColor XxGray6;
extern const XxColor XxWhite;

extern const XxColor XxRed1;
extern const XxColor XxRed2;
extern const XxColor XxRed3;
extern const XxColor XxRed4;

extern const XxColor XxYellow1;
extern const XxColor XxYellow2;
extern const XxColor XxYellow3;
extern const XxColor XxYellow4;

extern const XxColor XxGreen1;
extern const XxColor XxGreen2;
extern const XxColor XxGreen3;
extern const XxColor XxGreen4;

extern const XxColor XxBlue1;
extern const XxColor XxBlue2;
extern const XxColor XxBlue3;
extern const XxColor XxBlue4;

#endif
