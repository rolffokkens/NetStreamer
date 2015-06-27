/*
 * file: NrTransmitter.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <iostream.h>

#include "XxSocket.h"
#include "NrTransmitter.h"
#include "NrNetRadio.h"

long MsStartTime = 0;

NrTransSoundSource::NrTransSoundSource (NrTransConnection *pConnection)
{
    NrTransSoundSource::pConnection = pConnection;
};

NrTransSoundSource::~NrTransSoundSource (void)
{
};

void NrTransSoundSource::SetMute (int Flag)
{
};

void NrTransSoundSource::Start (void)
{
};

void NrTransSoundSource::HandleError (void)
{
    pConnection->Remove ();
};

void NrTransSoundSource::HandleRead (EzString Data)
{
    pConnection->HandleSoundRead (Data);
};

NrTransConnection::NrTransConnection
    ( EzString AddrPort
    , int Freq, EzString Description
    , EzString           AddInfo
    , NrTransSoundSource *pSoundSource
    , char               SampleRate
    )
    : NrRadioConnection ("Transmitter")
    , DisplayTimer (this)
{
    NrTransConnection::AudioSeq       = 0;
    NrTransConnection::pCurMsgHandler = &PhHelloHandleMessage;
    NrTransConnection::AddrPort       = AddrPort;
    NrTransConnection::Freq           = Freq;
    NrTransConnection::Description    = Description;
    NrTransConnection::pSoundSource   = pSoundSource;
    NrTransConnection::EncodingType   = EncCcittAdpcm;
    NrTransConnection::SampleRate     = SampleRate  ;
    NrTransConnection::AddInfo        = AddInfo;
};

NrTransConnection::~NrTransConnection (void)
{
    delete pSoundSource;
};

void NrTransConnection::TransDisplayTimer::HandleTimeOut (int)
{
    pOwner->SendInfo ();
};

void NrTransConnection::HandleSoundRead (EzString Data)
{
    EzString Header;

    Buffer += Data;

    while (Buffer.Length () > COMP_AUDIO_BLOCK) {
        Data   = Substr (Buffer, 0, COMP_AUDIO_BLOCK);
        Buffer = Substr (Buffer, COMP_AUDIO_BLOCK);
        if (!GetAudioMute ()) {
            //
            // The next FeatureLevel wrap is only for tuning
            // After all, the NrRadioConnection also wraps...
            // This prevents double wrapping
            //
            if (GetFeatureLevel () > 0) {
                AudioSeq = (AudioSeq + 1) & 0x0ff;
                Header   =   EzString (char (EncodingType << 4 | SampleRate))
                           + EzString (char (AudioSeq));
                Data     = Header + Data;
                SendMessage (MsgNumAudioData, Data);
            } else {
                SendMessage (MsgAudioData, Data);
            };
        };
    };
};

void NrTransConnection::PhModeHandleMessage   (NrMsgCode MsgCode, EzString Data)
{
    switch (MsgCode) {
    case MsgModeTransmitter:
        break;
    case MsgFrequencyInUse:
        CloseTrans ("Frequency In Use");
        break;
    case MsgBadFrequency:
        CloseTrans ("Bad Frequency");
        break;
    };
};

void NrTransConnection::PhClientHandleMessage (NrMsgCode MsgCode, EzString Data)
{
    switch (MsgCode) {
    case MsgServerInfo:
        SetFeatureLevel (int (Data));
        break;
    case MsgNoMode:
        SetFeatureLevel (0);
        break;
    default:
        CloseTrans ("Protocol Error");
        return;
    };

    pCurMsgHandler = &PhModeHandleMessage;
    SendMessage
        ( MsgModeTransmitter
        , NumberToString (Freq) + EzString (" ") + Description
        );
};

void NrTransConnection::PhBinaryHandleMessage (NrMsgCode MsgCode, EzString Data)
{
    if (MsgCode == MsgBinary) {
        pCurMsgHandler = &PhClientHandleMessage;

        SendMessage
            ( MsgClientInfo
            ,   EzString (FEATURE_LEVEL) + EzString (" ") + GetUname ()
              + EzString (" ") + AddInfo
            );
    } else {
        CloseTrans ("Protocol Error");
    };
};

void NrTransConnection::PhHelloHandleMessage (NrMsgCode MsgCode, EzString Data)
{
    if (MsgCode == MsgReady) {
        SendMessage (MsgBinary, "");

        pCurMsgHandler = &PhBinaryHandleMessage;
    } else {
        CloseTrans ("Protocol Error");
    };
};

void NrTransConnection::HandleMessage (NrMsgCode MsgCode, EzString Data)
{
    switch (MsgCode) {
    case MsgPing:
        SendMessage (MsgPing, "");
        break;
    case MsgMuteOn:
        pSoundSource->SetMute (1);
        SetAudioMute (1);
        break;
    case MsgMuteOff:
        pSoundSource->SetMute (0);
        SetAudioMute (0);
        break;
    case MsgToAll:
    case MsgToTuned:
    case MsgToTransmitter:
        break;
    default:
        (this->*pCurMsgHandler)(MsgCode, Data);
    };
};

void NrTransConnection::Start (void)
{
    if (Connect (AddrPort)) {
        pSoundSource->Start ();
    } else {
        cerr << "Cannot connect to " << AddrPort << endl;
        Remove ();
    };
};

void NrTransConnection::CloseTrans (EzString Message)
{
    cerr << Message << endl;

    Remove ();
};

void NrTransConnection::SendInfo (void)
{
    if (pCurMsgHandler == &PhModeHandleMessage) {
        SendMessage (MsgToTuned, MsgStrDisplay + EzString (" ") + SoundInfo);
    };
};

void NrTransConnection::HandleInfo (EzString SoundInfo)
{
    NrTransConnection::SoundInfo = SoundInfo;

    SendInfo ();

    DisplayTimer.SetRestart (30000);
};


