/*
 * file: XxMaxMeter.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "XxMaxMeter.h"

XxMaxMeter::XxMaxMeter
    ( EzString    Name
    , XxDrawable *pParent
    , int XPos,   int YPos
    , int Width,  int Height
    , int MinVal, int MaxVal
    , int Dir
    , int StepSize
    , int Offset
    ) : XxMeter
        ( Name
        , pParent
        , XPos,   YPos
        , Width,  Height
        , MinVal * 64, MaxVal * 64
        , Dir
        , StepSize
        , Offset
        )
{
    CurVal = 0;
    dVal   = MaxVal - MinVal;
    if (dVal < 0) dVal = 1;
};

XxMaxMeter::~XxMaxMeter (void)
{
};

void XxMaxMeter::SetCurVal (int Val)
{
    Val *= 64;

    if (Val > CurVal) {
        CurVal = Val;
    } else {
        if (CurVal > GetMinVal ()) CurVal -= dVal;
    };
    XxMeter::SetCurVal (CurVal);
};
