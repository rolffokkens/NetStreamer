/*
 * file: XxMeter.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_METER
#define H_XX_METER

#include "XxVirtual.h"
#include "XxPixmap.h"

class XxMeter : public XxVirtual {
private:
    int MinVal, MaxVal, CurVal;
    int IntVal;
    int NrLines;
    int Dir;
    int StepSize;
    int Offset;

    XxPixmap OnMap, OffMap;

    int CalcIntVal (int Val);
protected:
    void OnPreFlush ();
    int GetMinVal (void) { return MinVal; };
    int GetMaxVal (void) { return MaxVal; };
public:
    XxMeter
        ( EzString    Name
        , XxDrawable *pParent
        , int XPos,   int YPos
        , int Width,  int Height
        , int MinVal, int MaxVal
        , int Dir
        , int StepSize = 1
        , int Offset   = 0
        );

    virtual ~XxMeter (void) {};

    void SetCurVal (int Val);

    void SetOnColor   (XxColor Color);
    void SetOffColor  (XxColor Color);

    void SetOnPixmap  (XxPixmap &pixmap);
    void SetOffPixmap (XxPixmap &pixmap);
};

#endif
