/*
 * file: NrTransSoundDev.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "NrTransSoundDev.h"

NrTransSoundDev::NrTransSoundDev
    ( NrTransConnection *pConnection
    , char SampleRate
    , EzString Device
    , EzString AppName
    )
    : NrTransSoundSource (pConnection)
    , Comp (3)
    , XxSoundDevOSS (Device, AppName)
{
    NrTransSoundDev::MuteFlag   = 1;
    NrTransSoundDev::SampleRate = SampleRate;
};

NrTransSoundDev::~NrTransSoundDev (void)
{
};

EzString NrTransSoundDev::ProcessReadData  (EzString Data)
{
    EzString RetVal;
    if (MuteFlag) {
        RetVal = "";
    } else {
        RetVal = Comp.Compress (XxSoundDevOSS::ProcessReadData (Data));
    };

    return RetVal;
};

void NrTransSoundDev::SetMute (int Flag)
{
    MuteFlag = Flag;
};

void NrTransSoundDev::Start (void)
{
    if (!Open (ModeRead, AUDIO_BITS, 0, RateHerz[SampleRate])) {
        HandleError ();
    };
};

NrTransConnSoundDev::NrTransConnSoundDev
    ( EzString AddrPort
    , int Freq, EzString Description
    , EzString AddInfo
    , char SampleRate
    , EzString Device
    , EzString AppName
    )
    : NrTransConnection
          ( AddrPort, Freq, Description
          , AddInfo
          , new NrTransSoundDev (this, SampleRate, Device, AppName)
	  , SampleRate
          )
{
};

NrTransConnSoundDev::~NrTransConnSoundDev (void)
{
};
