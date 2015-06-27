/*
 * file: XxLcdScrollDisplay.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "XxLcdScrollDisplay.h"

XxLcdScrollDisplay::XxLcdScrollDisplay
    ( EzString Name
    , XxDrawable *pParent
    , int XOffset, int YOffset
    , int nDigits
    , XxColor OnColor, XxColor OffColor
    , int RefreshDelay
    ) : ScrollTimer
            (this)
      , XxLcdAlphaDisplay
            (Name, pParent, XOffset, YOffset, nDigits, OnColor, OffColor)
{
    XxLcdScrollDisplay::RefreshDelay = RefreshDelay;
};

XxLcdScrollDisplay::~XxLcdScrollDisplay (void)
{
};

void XxLcdScrollDisplay::SetVal (EzString Val)
{
    DisplayVal   = Val;
    CurScrollVal = Val;

    XxLcdAlphaDisplay::SetVal (Val);

    ScrollTimer.Stop ();
};

void XxLcdScrollDisplay::SetScrollVal (EzString Val)
{
    DisplayVal   = Val;
    CurScrollVal = Rpad (' ', GetNumDigits ()) + Val;

    ScrollTimer.SetRestart (RefreshDelay);
};


void XxLcdScrollDisplay::ScrollTimerTyp::HandleTimeOut (int Count)
{
    pOwner->HandleScrollTimeout ();
};

void XxLcdScrollDisplay::HandleScrollTimeout ()
{
    CurScrollVal = Substr (CurScrollVal, 1);

    XxLcdAlphaDisplay::SetVal (Substr (CurScrollVal, 0, GetNumDigits ()));

    if (CurScrollVal == EzString ("")) HandleScrollDone ();
};

void XxLcdScrollDisplay::HandleScrollDone (void)
{
    SetScrollVal (DisplayVal);
};

