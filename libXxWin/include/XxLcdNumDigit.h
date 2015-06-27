/*
 * file: XxLcdNumDigit.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_LCDNUMDIGIT
#define H_XX_LCDNUMDIGIT

#include "EzString.h"

#include "XxGC.h"
#include "XxVirtual.h"

class XxLcdNumDigit : public XxVirtual {
private:
    int     DigitVal;
    int     DotVal;
    XxColor OnColor, OffColor, BackColor;

    void DrawDigit (int DigitVal, int DotVal);
protected:
    XxGC DrawGC;
public:
    XxLcdNumDigit
        ( EzString Name
        , XxDrawable *pParent
        , int XOffset, int YOffset
        , XxColor OnColor, XxColor OffColor, XxColor BackColor
        );

    virtual ~XxLcdNumDigit (void);

    void SetVal (int Val);
    void SetDot (int Val);
};

#endif
