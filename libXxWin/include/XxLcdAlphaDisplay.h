/*
 * file: XxLcdAlphaDisplay.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_LCDALPHADISPLAY
#define H_XX_LCDALPHADISPLAY

#include "XxLcdAlphaDigit.h"

class XxLcdAlphaDisplay {
private:
    XxLcdAlphaDigit **pDigits;
    EzString          DisplayVal;
    int               nDigits;
    int               EliminateZeroDigit;

    void DrawDisplay (EzString Val);
protected:
    XxGC DrawGC;
public:
    XxLcdAlphaDisplay
        ( EzString Name
        , XxDrawable *pParent
        , int XOffset, int YOffset
        , int nDigits
        , XxColor OnColor, XxColor OffColor
        );

    virtual ~XxLcdAlphaDisplay (void);

    virtual void SetVal (EzString Val);

    virtual EzString GetVal (void) { return DisplayVal; };

    int GetNumDigits (void) { return nDigits; };
};

#endif
