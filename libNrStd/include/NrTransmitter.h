/*
 * file: NrTransmitter.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_NR_TRANSMITTER
#define H_NR_TRANSMITTER

#include "BiLink.h"
#include "EzString.h"
#include "XxTimer.h"
#include "XxSocket.h"
#include "NrNetRadio.h"
#include "XxStream.h"
#include "NrComp.h"

class NrTransConnection;

class NrTransSoundSource : public virtual XxStream {
    friend NrTransConnection;
private:
    NrTransConnection *pConnection;
protected:
    NrTransSoundSource (NrTransConnection *pConnection);
    virtual ~NrTransSoundSource (void);
    void HandleRead (EzString Data);
    virtual void SetMute (int Flag);
    virtual void Start (void);
    void HandleError (void);
    void HandleInfo  (EzString SoundInfo);
};

class NrTransConnection : public NrRadioConnection {
    friend NrTransSoundSource;
private:
    typedef tXxTimer<NrTransConnection> TransDisplayTimer;
    friend TransDisplayTimer;

    typedef void (NrTransConnection::*NrTransMsgHnd)
                     ( NrMsgCode MsgCode, EzString Data);

    TransDisplayTimer   DisplayTimer;

    short               AudioSeq;
    char                EncodingType;
    char                SampleRate;

    EzString            AddInfo;
    EzString            AddrPort;
    int                 Freq;
    EzString            Description;
    NrTransSoundSource *pSoundSource;
    NrTransMsgHnd       pCurMsgHandler;

    EzString            Buffer;
    EzString            SoundInfo;

    void PhModeHandleMessage   (NrMsgCode MsgCode, EzString Data);
    void PhClientHandleMessage (NrMsgCode MsgCode, EzString Data);
    void PhBinaryHandleMessage (NrMsgCode MsgCode, EzString Data);
    void PhHelloHandleMessage  (NrMsgCode MsgCode, EzString Data);
    void HandleMessage         (NrMsgCode MsgCode, EzString Data);

    void HandleSoundRead (EzString Data);

    virtual void CloseTrans (EzString Message);
protected:
    void SendInfo (void);
    void HandleInfo (EzString SoundInfo);
public:
    NrTransConnection
        ( EzString AddrPort
        , int Freq, EzString Description
        , EzString           AddInfo
        , NrTransSoundSource *pSoundSource
        , char               SampleRate
        );

    virtual ~NrTransConnection (void);

    void Start (void);
};

inline void NrTransSoundSource::HandleInfo (EzString SoundInfo)
{
    pConnection->HandleInfo (SoundInfo);
};

#endif
