/*
 * file: XxFlashLight.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "XxFlashLight.h"

XxFlashLight::XxFlashLight
    ( EzString Name
    , XxWindow *pParent
    , int XOffset, int YOffset
    , int Width, int Height
    , const unsigned char *Data
    , XxColor OnColor, XxColor OffColor, XxColor BackColor
    , int MsFlashDelay
    )
    : XxVirtual
        ( Name
        , pParent
        , XOffset, YOffset
        , Width, Height
        )
    , BlinkTimer (this, MsFlashDelay)
{
    OnFlag    = -1;
    BlinkFlag = -1;
    OnLight   = new XxPixmap
                   ( Name + EzString ("_On")
                   , Width, Height
                   , Data
                   , OnColor, BackColor
                   );
    OffLight  = new XxPixmap
                   ( Name + EzString ("_Off")
                   , Width, Height
                   , Data
                   , OffColor, BackColor
                   );
    SetOnFlag (0);
};

XxFlashLight::~XxFlashLight (void)
{
    delete OnLight;
    delete OffLight;
};

void XxFlashLight::SetOnFlag (int Flag)
{
    Flag = (Flag != 0);
    if (OnFlag == Flag) return;

    OnFlag = Flag;

    UpdateLight (OnFlag);
}

void XxFlashLight::GetOnFlag (int &Flag)
{
    Flag = OnFlag;
};

void XxFlashLight::UpdateLight (int BlinkFlag)
{
    int OnTmp;

    if (XxFlashLight::BlinkFlag == BlinkFlag) return;

    XxFlashLight::BlinkFlag = BlinkFlag;

    CopyArea
        ( DrawGC
        , (OnFlag && BlinkFlag ? OnLight : OffLight)
        , 0, 0
        , GetWidth (), GetHeight ()
        , 0, 0
        );
};

template <>
void XxFlashLight::XxFlashLightTimer::HandleTimeOut (int Count)
{
    pOwner->UpdateLight (!pOwner->BlinkFlag);
};


