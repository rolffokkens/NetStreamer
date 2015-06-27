/*
 * file: XxMeter.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "XxMeter.h"

XxMeter::XxMeter
    ( EzString Name
    , XxDrawable *pParent
    , int XPos,   int YPos
    , int Width,  int Height
    , int MinVal, int MaxVal
    , int Dir
    , int StepSize
    , int Offset
    )
    : XxVirtual (Name, pParent, XPos, YPos, Width, Height)
    , OnMap  (Name + EzString ("_OnMap"),  Width, Height)
    , OffMap (Name + EzString ("_OffMap"), Width, Height)
{
    XxMeter::MinVal   = MinVal;
    XxMeter::MaxVal   = MaxVal;
    XxMeter::Dir      = Dir ^ 2;
    XxMeter::StepSize = StepSize;
    XxMeter::Offset   = Offset;
    XxMeter::NrLines  = (Dir & 1 ? Width : Height);

    DrawGC.SetForeground (XxWhite);
    OnMap.FillRectangle  (DrawGC, 0, 0, Width, Height);
    DrawGC.SetForeground (XxBlack);
    OffMap.FillRectangle (DrawGC, 0, 0, Width, Height);

    IntVal  = CalcIntVal (MaxVal);
    SetCurVal (MinVal);
};

void XxMeter::OnPreFlush ()
{
    IntVal  = CalcIntVal (MaxVal);
    SetCurVal (CurVal);
    IntVal  = -1;
    SetCurVal (CurVal);
};

int XxMeter::CalcIntVal (int Val)
{
    int RetVal;

    if (Val > MaxVal) Val = MaxVal;
    if (Val < MinVal) Val = MinVal;

    RetVal = ((NrLines * (Val - MinVal)) / (MaxVal - MinVal));

    if (StepSize > 1) {
        RetVal += StepSize / 2;
        RetVal -= Offset;
        RetVal -= ((RetVal + StepSize) % StepSize);
        RetVal += Offset;
    };

    RetVal--;

    if (RetVal < -1) RetVal = -1;

    return RetVal;
};

void XxMeter::SetCurVal (int Val)
{
    int  TmpVal, i, j;
    int  L, R, Y1, Y2, W, H;
    int  dY;
    XxPixmap *pMap;

    CurVal = Val;
    TmpVal = CalcIntVal (Val);

    if (TmpVal == IntVal) return;

    if (TmpVal < IntVal) {
       DrawGC.SetForeground (XxGray3);
       pMap  = &OffMap;
       i     = IntVal;
       j     = TmpVal;

       Y1    = TmpVal + 1;
       Y2    = IntVal;
    } else {
       DrawGC.SetForeground (XxWhite);
       pMap  = &OnMap;
       i     = TmpVal;
       j     = IntVal;

       Y1    = IntVal + 1;
       Y2    = TmpVal;
    };

    if (Dir & 1) {
        W = GetHeight ();
        H = GetWidth  ();
    } else {
        W = GetWidth  ();
        H = GetHeight ();
    };

    L = 0;
    R = W - 1;

    if ((Dir & 2) == 0) {
        i = H - 1 - Y1;
        j = H - 1 - Y2;
        Y2 = i;
        Y1 = j;
    };

    if (Dir & 1) {
        CopyArea (DrawGC, pMap, Y1, L, (Y2 - Y1 + 1), (R - L + 1), Y1, L);
    } else {
        CopyArea (DrawGC, pMap, L, Y1, (R - L + 1), (Y2 - Y1 + 1), L, Y1);
    };

    IntVal = TmpVal;
};

void XxMeter::SetOnColor  (XxColor Color)
{
    DrawGC.SetForeground (Color);

    OnMap.FillRectangle (DrawGC, 0, 0, GetWidth (), GetHeight ());

    MarkPreFlush ();
};

void XxMeter::SetOffColor (XxColor Color)
{
    DrawGC.SetForeground (Color);

    OffMap.FillRectangle (DrawGC, 0, 0, GetWidth (), GetHeight ());

    MarkPreFlush ();
};

void XxMeter::SetOnPixmap  (XxPixmap &pixmap)
{
    OnMap.CopyArea (DrawGC, &pixmap, 0, 0, GetWidth (), GetHeight (), 0, 0);

    MarkPreFlush ();
};

void XxMeter::SetOffPixmap (XxPixmap &pixmap)
{
    OffMap.CopyArea (DrawGC, &pixmap, 0, 0, GetWidth (), GetHeight (), 0, 0);

    MarkPreFlush ();
};

