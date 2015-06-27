/*
 * file: NrTransStdIn.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1998 Rolf Fokkens
 *
 */

#ifndef H_NR_TRANSSTDIN
#define H_NR_TRANSSTDIN

#include "BiLink.h"
#include "EzString.h"
#include "XxTimer.h"
#include "XxSocket.h"
#include "XxStdio.h"
#include "NrNetRadio.h"
#include "NrComp.h"
#include "NrRateAdjust.h"

#include "NrTransmitter.h"

class NrTransStdIn : public NrTransSoundSource
                   , public virtual XxStdIn {
    typedef tXxTimer<NrTransStdIn> TransStdInTimer;

    friend class tXxTimer<NrTransStdIn>;
private:
    NrRateAdjust    Adjust;

    EzString        BlockBuffer;

    int             AvailToRead;

    TransStdInTimer Timer;
    int             MuteFlag;
    NrComp          Comp;
    char            SampleRate;

    void HandleTimeOut (int Count);
protected:
    virtual EzString ProcessReadData  (EzString Data);

    virtual int GetReadChunkSize  (void);

    virtual void OnDisconnect (void);
public:
    NrTransStdIn
        (NrTransConnection *pConnection, char SampleRate, int InSampleRate);
    virtual ~NrTransStdIn (void);

    virtual void GetRWFlags (int &rFlag, int &wFlag);

    virtual void SetMute (int Flag);
    virtual void Start   (void);
};

class NrTransConnStdIn : public NrTransConnection {
public:
    NrTransConnStdIn
        ( EzString AddrPort, int Freq, EzString Description
        , EzString AddInfo, char SampleRate, int InSampleRate
        );
    virtual ~NrTransConnStdIn (void);
};

#endif
