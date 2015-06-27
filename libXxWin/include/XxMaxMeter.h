/*
 * file: XxMaxMeter.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_MAX_METER
#define H_XX_MAX_METER

#include "XxMeter.h"

class XxMaxMeter : public XxMeter {
private:
    int CurVal;
    int dVal;
protected:
public:
    XxMaxMeter
        ( EzString    Name
        , XxDrawable *pParent
        , int XPos,   int YPos
        , int Width,  int Height
        , int MinVal, int MaxVal
        , int Dir
        , int StepSize = 1
        , int Offset   = 0
        );

    virtual ~XxMaxMeter (void);

    virtual void SetCurVal (int Val);
};

#endif
