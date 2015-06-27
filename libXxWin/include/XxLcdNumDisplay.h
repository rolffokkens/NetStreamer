/*
 * file: XxLcdNumDisplay.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_LCDNUMDISPLAY
#define H_XX_LCDNUMDISPLAY

#include "XxLcdNumDigit.h"

class XxLcdNumDisplay {
private:
    XxLcdNumDigit **pDigits;
    int     DisplayVal;
    int     NumDigits;
    int     EliminateZeroDigit;

    void DrawDisplay (int Val);
protected:
    XxGC DrawGC;
public:
    XxLcdNumDisplay
        ( EzString Name
        , XxDrawable *pParent
        , int XOffset, int YOffset
        , int NumDigits, int DotDigit
        , int EliminateZeroDigit
        , XxColor OnColor, XxColor OffColor, XxColor BackColor
        );

    virtual ~XxLcdNumDisplay (void);

    void SetVal (int Val);
};

#endif
