/*
 * file: XxFont.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_FONT
#define H_XX_FONT

#include <X11/Xlib.h>

#include "EzString.h"
#include "XxCore.h"

class XxIntFontCommon;
class XxGC;

class XxFont : public XxCore {
    friend XxGC;
private:
    XxIntFontCommon *IntGetFont (EzString Name, int Size);

    XxIntFontCommon *pCommon;
    Font GetXFont ();
public:
    XxFont (EzString Name = EzString ("helvetica"), int Size = 12);
    XxFont (XxFont &);

    virtual ~XxFont (void);

    XxFont &operator= (XxFont &);

    void GetTextSize (EzString Str, int &Width, int &Ascent, int &Descent);
};

#endif
