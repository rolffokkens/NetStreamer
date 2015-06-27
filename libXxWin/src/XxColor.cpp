/*
 * file: XxColor.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <iostream.h>
#include "XxColor.h"

struct ColorEntry {
    XColor     Color;
    long       pixel;
    ColorEntry *pNext;
};

static ColorEntry *pColorList = NULL;

static void GetColor
   ( Display *display, Colormap colormap
   , XColor &Color
   )
{
    ColorEntry *pColor;
    XColor     tColor;

    pColor = pColorList;

    while (pColor != NULL) {
        if (   pColor->Color.red   == Color.red 
            && pColor->Color.green == Color.green
            && pColor->Color.blue  == Color.blue  ) {
            Color.pixel = pColor->pixel;
            return;
        };
        pColor = pColor->pNext;
    };
    pColor = new ColorEntry;

    pColor->Color = Color;
    pColor->pNext = pColorList;
    pColorList    = pColor;

    tColor = Color;

    XAllocColor
    ( display, colormap
    , &tColor
    );
    pColor->pixel = tColor.pixel;
    Color.pixel   = tColor.pixel;

    return;
};

XxColor::XxColor (const XxColor &Color)
    : XxCore (0)
{
    XxColor::Color     = Color.Color;
    XxColor::AllocDone = Color.AllocDone;
};

void XxColor::Constr (int Red, int Green, int Blue, char AllocDone)
{
    Color.red          = Red;
    Color.green        = Green;
    Color.blue         = Blue;
    XxColor::AllocDone = AllocDone;
};

XxColor::XxColor (int Red, int Green, int Blue)
    : XxCore (0)
{
    Constr (Red, Green, Blue, 0);
};

XxColor::XxColor (void)
    : XxCore (0)
{
    Constr (0, 0, 0, 0);
};

long XxColor::GetPixel (void)
{
    if (!AllocDone) {
        GetColor
        ( GetDisplay ()
        , DefaultColormap (GetDisplay (), GetScreen ())
        , Color
        );
/*
        Color.
cerr << "C" << endl;
        XAllocColor
        ( GetDisplay ()
        , DefaultColormap (GetDisplay (), GetScreen ())
        , &Color
        );
*/
        AllocDone   = 1;
    };

    return Color.pixel;
};

XxColor &XxColor::operator= (const XxColor &Color)
{
    XxColor::Color     = Color.Color;
    XxColor::AllocDone = Color.AllocDone;

    return *this;
};

const XxColor XxBlack   (0x0000,0x0000,0x0000);
const XxColor XxGray1   (0x2000,0x2000,0x2000);
const XxColor XxGray2   (0x4000,0x4000,0x4000);
const XxColor XxGray3   (0x6000,0x6000,0x6000);
const XxColor XxGray4   (0x8000,0x8000,0x8000);
const XxColor XxGray5   (0xA000,0xA000,0xA000);
const XxColor XxGray6   (0xC000,0xC000,0xC000);
const XxColor XxGray7   (0xE000,0xE000,0xE000);
const XxColor XxWhite   (0xffff,0xffff,0xffff);

const XxColor XxRed1    (0x4000,0x0000,0x0000);
const XxColor XxRed2    (0x8000,0x0000,0x0000);
const XxColor XxRed3    (0xC000,0x0000,0x0000);
const XxColor XxRed4    (0xffff,0x0000,0x0000);

const XxColor XxYellow1 (0x4000,0x4000,0x0000);
const XxColor XxYellow2 (0x8000,0x8000,0x0000);
const XxColor XxYellow3 (0xC000,0xC000,0x0000);
const XxColor XxYellow4 (0xffff,0xffff,0x0000);

const XxColor XxGreen1  (0x0000,0x4000,0x0000);
const XxColor XxGreen2  (0x0000,0x8000,0x0000);
const XxColor XxGreen3  (0x0000,0xC000,0x0000);
const XxColor XxGreen4  (0x0000,0xffff,0x0000);

const XxColor XxBlue1   (0x0000,0x0000,0x4000);
const XxColor XxBlue2   (0x0000,0x0000,0x8000);
const XxColor XxBlue3   (0x0000,0x0000,0xC000);
const XxColor XxBlue4   (0x0000,0x0000,0xffff);
