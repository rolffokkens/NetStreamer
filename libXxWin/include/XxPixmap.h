/*
 * file: XxPixmap.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_PIXMAP
#define H_XX_PIXMAP

#include "XxDrawable.h"

class XxWindow;

class XxPixmap : public XxDrawable {
    friend XxWindow;
private:
    int Width, Height;
    Pixmap Pm;
    Pixmap Bm;

    XxPixmap (EzString Name, Pixmap &) : XxDrawable (Name) { };

    void Constr (int Width, int Height);
    void Destr (void);

    virtual Drawable qGetDrawable (void) { return Pm; };
protected:
public:
    XxPixmap (EzString Name, int Width, int Height);
    XxPixmap (EzString Name, const XxPixmap &pixmap);
    XxPixmap 
        ( EzString Name
        , int Width, int Height
        , const unsigned char *Data
        , XxColor FrontColor, XxColor BackColor
        );

    XxPixmap 
        ( EzString Name
        , char * const *Data
        );

    virtual ~XxPixmap (void);

    int      GetHeight   (void) { return Height; };
    int      GetWidth    (void) { return Width;  };

    XxPixmap &operator= (const XxPixmap &pixmap);
};

#endif
