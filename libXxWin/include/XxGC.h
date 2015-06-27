/*
 * file: XxGC.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_GC
#define H_XX_GC

#include <X11/Xlib.h>

#include "EzString.h"
#include "XxFont.h"
#include "XxColor.h"

class XxDrawable;
class XxBitmap;

class XxGC {
    friend XxDrawable;
private:
    XxFont    xFont;
    GC        gc;
    XGCValues gcval;
    long      gcmask;

    void Activate (void);
public:
    XxGC (void);
    virtual ~XxGC (void);

    XxGC (XxGC &) { };
    XxGC &operator= (XxGC &) { return *this; };

    void SetForeground    (XxColor Color);
    void SetBackground    (XxColor Color);
    void SetLineWidth     (int  width);

    void SetClipMask      (int XPos, int YPos, XxBitmap *pBitmap);
    void SetClipRectangle (int XPos, int YPos, int Width, int Height);

    void SetFont          (XxFont xFont);

    void GetTextSize (EzString Str, int &Width, int &Ascent, int &Descent);
};

#endif
