/*
 * file: XxLcdAlphaDigit.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_LCDALPHADIGIT
#define H_XX_LCDALPHADIGIT

#include "EzString.h"

#include "XxGC.h"
#include "XxVirtual.h"

class XxLcdAlphaDigit : public XxVirtual {
private:
    int     CharVal;
    XxColor OnColor, OffColor;

    void DrawDigit (int CharVal);
protected:
    XxGC DrawGC;
public:
    XxLcdAlphaDigit
        ( EzString Name
        , XxDrawable *pParent
        , int XOffset, int YOffset
        , XxColor OnColor, XxColor OffColor
        );

    int GetNetWidth ();

    virtual ~XxLcdAlphaDigit (void);

    void SetVal (int Val);
};

#endif
