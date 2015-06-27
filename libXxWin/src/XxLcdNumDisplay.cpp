/*
 * file: XxLcdNumDisplay.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "XxLcdNumDisplay.h"

XxLcdNumDisplay::XxLcdNumDisplay
    ( EzString Name, XxDrawable *pParent
    , int XOffset, int YOffset
    , int NumDigits, int DotDigit
    , int EliminateZeroDigit
    , XxColor OnColor, XxColor OffColor, XxColor BackColor
    )
{
    int i;

    XxLcdNumDisplay::NumDigits          = NumDigits;
    XxLcdNumDisplay::DisplayVal         = -1;
    XxLcdNumDisplay::pDigits            = new XxLcdNumDigit *[NumDigits];
    XxLcdNumDisplay::EliminateZeroDigit = EliminateZeroDigit;

    for (i = 0; i < NumDigits; i++) {
        pDigits[i] = new XxLcdNumDigit
                         ( Name + EzString ("_") + EzString (i)
                         , pParent , XOffset, YOffset
                         , OnColor, OffColor, BackColor
                         );
        if (i == DotDigit) pDigits[i]->SetDot (1);

        XOffset += pDigits[i]->GetWidth ();
    };

    DrawDisplay (0);
};

XxLcdNumDisplay::~XxLcdNumDisplay (void)
{
    int i;

    for (i = 0; i < NumDigits; i++) delete pDigits[i];

    delete [] pDigits;
};

void XxLcdNumDisplay::DrawDisplay (int Val)
{
    int i, DigitVal;

    if (Val == DisplayVal) return;

    DisplayVal = Val;

    for (i = NumDigits - 1; i > -1; i--) {
        DigitVal = (Val % 10);
        if (Val == 0 && i < EliminateZeroDigit) DigitVal = -1;
        pDigits[i]->SetVal (DigitVal);
        Val /= 10;
    };
};


void XxLcdNumDisplay::SetVal (int Val)
{
    DrawDisplay (Val);
};
