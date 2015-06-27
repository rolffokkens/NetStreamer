/*
 * file: XxLcdAlphaDisplay.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "XxLcdAlphaDigit.h"
#include "XxLcdAlphaDisplay.h"

XxLcdAlphaDisplay::XxLcdAlphaDisplay
    ( EzString Name, XxDrawable *pParent
    , int XOffset, int YOffset
    , int nDigits
    , XxColor OnColor, XxColor OffColor
    )
{
    int i;

    XxLcdAlphaDisplay::nDigits            = nDigits;
    XxLcdAlphaDisplay::DisplayVal         = -1;
    XxLcdAlphaDisplay::pDigits            = new XxLcdAlphaDigit *[nDigits];
    XxLcdAlphaDisplay::EliminateZeroDigit = EliminateZeroDigit;

    for (i = 0; i < nDigits; i++) {
        pDigits[i] = new XxLcdAlphaDigit
                         ( Name + EzString ("_") + EzString (i)
                         , pParent , XOffset, YOffset
                         , OnColor, OffColor
                         );

        XOffset += pDigits[i]->GetNetWidth ();
    };

    DrawDisplay (EzString (""));
};

XxLcdAlphaDisplay::~XxLcdAlphaDisplay (void)
{
    int i;

    for (i = 0; i < nDigits; i++) delete pDigits[i];

    delete [] pDigits;
};

void XxLcdAlphaDisplay::DrawDisplay (EzString Val)
{
    int i, DigitVal;

    Val = Rpad (Val, nDigits);

    if (Val == DisplayVal) return;

    DisplayVal = Val;

    for (i = 0; i < nDigits; i++) {
        pDigits[i]->SetVal (Val[i]);
    };
};


void XxLcdAlphaDisplay::SetVal (EzString Val)
{
    DrawDisplay (Val);
};
