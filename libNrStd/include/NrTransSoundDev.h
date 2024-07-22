/*
 * file: NrTransSoundDev.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_NR_TRANSSOUNDDEV
#define H_NR_TRANSSOUNDDEV

#include "BiLink.h"
#include "EzString.h"
#include "XxTimer.h"
#include "XxSocket.h"
#include "NrNetRadio.h"
#include "XxSoundDevOSS.h"
#include "NrComp.h"

#include "NrTransmitter.h"

class NrTransSoundDev : public virtual NrTransSoundSource
                      , public virtual XxSoundDevOSS {
private:
    int    MuteFlag;
    NrComp Comp;
    char   SampleRate;
protected:
    virtual EzString ProcessReadData  (EzString Data);
public:
    NrTransSoundDev (NrTransConnection *pConnection, char SampleRate, EzString Device);
    virtual ~NrTransSoundDev (void);
    virtual void SetMute (int Flag);
    virtual void Start (void);
};

class NrTransConnSoundDev : public NrTransConnection {
public:
    NrTransConnSoundDev
        ( EzString AddrPort, int Freq, EzString Description
        , EzString AddInfo,  char SampleRate, EzString Device
        );
    virtual ~NrTransConnSoundDev
        (void);
};

#endif
