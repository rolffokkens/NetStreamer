/*
 * file: XxFlashLight.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_FLASHLIGHT
#define H_XX_FLASHLIGHT

#include "XxWindow.h"
#include "XxVirtual.h"
#include "XxTimer.h"
#include "XxPixmap.h"

class XxFlashLight : public XxVirtual {
private:
    int OnFlag, BlinkFlag;
    XxPixmap *OnLight, *OffLight;

    typedef tXxTimer<XxFlashLight> XxFlashLightTimer;
    friend XxFlashLightTimer;

    XxFlashLightTimer BlinkTimer;

    void UpdateLight (int BlinkFlag);
public:
    XxFlashLight
        ( EzString Name
        , XxWindow *pParent
        , int XOffset, int YOffset
        , int Width, int Height
        , const char *Data
        , XxColor OnColor, XxColor OffColor, XxColor BackColor
        , int MsFlashDelay = 0
        );
    virtual ~XxFlashLight (void);
    void SetOnFlag (int  Flag);
    void GetOnFlag (int &Flag);
};

#endif

