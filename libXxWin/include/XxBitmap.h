/*
 * file: XxBitmap.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_BITMAP
#define H_XX_BITMAP

#include "XxDrawable.h"

class XxWindow;

class XxBitmap : public XxDrawable {
    friend XxGC;
private:
    int Width, Height;
    Pixmap Pm;

    XxBitmap (EzString Name, Pixmap &) : XxDrawable (Name) { };

    void Constr (int Width, int Height);
    void Destr (void);

    virtual Drawable qGetDrawable (void) { return Pm; };
protected:
public:
    XxBitmap (EzString Name, int Width, int Height);
    XxBitmap (EzString Name, int Width, int Height, const char *Data);
    XxBitmap (EzString Name, const XxBitmap &pixmap);

    virtual ~XxBitmap (void);

    int      GetHeight   (void) { return Height; };
    int      GetWidth    (void) { return Width;  };

    XxBitmap &operator= (const XxBitmap &pixmap);
};

#endif
