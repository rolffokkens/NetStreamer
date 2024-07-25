/*
 * file: NrReceiver.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <errno.h>
#include <iostream>
#include <fcntl.h>

#include "NrMessages.h"
#include "NrReceiver.h"

using namespace std;

NrRecSoundDev::NrRecSoundDev (NrRecConnection *pConnection, EzString Device, EzString AppName) : XXSOUNDDEV (Device, AppName)
{
    NrRecSoundDev::pConnection = pConnection;
};

void NrRecSoundDev::IntHandleWrite (void)
{
    if (XXSOUNDDEV::GetOutBufSize () < AUDIO_BLOCK) {
        XXSOUNDDEV::Write (pConnection->GetSound (AUDIO_BLOCK));
    };
    XXSOUNDDEV::IntHandleWrite ();
};

int NrRecSoundDev::Open
    (MODE_RW ModeRW, int SampleSize, int StereoFlag, int Speed)
{
    int  IntBufSize;
    char *cp;
    int  OpenOk, Dummy = 0;

    OpenOk = XXSOUNDDEV::Open (ModeRW, SampleSize, StereoFlag, Speed);

    if (OpenOk) {
        IntBufSize = GetIntOutBufFree ();

        cp = new char [IntBufSize];

        memset (cp, 0, IntBufSize);

        write (GetFd (), cp, IntBufSize);

        delete cp;
    };
    return OpenOk;
};

NrRecConnection::NrRecConnection (int Freq, char SampleRate, EzString Driver, EzString Device, EzString AddInfo)
    : NrRadioConnection ("Receiver")
    , SoundDev (this, Device, AddInfo)
    , Pump (this, 50000, "Receiver", SampleRate)
{
    NrRecConnection::Initialized    = 0;
    NrRecConnection::pCurMsgHandler = &NrRecConnection::PhHelloHandleMessage;
    NrRecConnection::Freq           = Freq;
    NrRecConnection::SampleRate     = SampleRate;
    NrRecConnection::MuteFlag       = 1;
    NrRecConnection::AddInfo        = AddInfo;
};

int NrRecConnection::OpenDev (void)
{
    int RetVal;

    RetVal = SoundDev.Open (ModeWrite, AUDIO_BITS, 0, RateHerz[SampleRate]);

    if (RetVal) {
        Pump.SetOsSoundDelay (SoundDev.GetIntOutDelay ());
    };

    return RetVal;
};

NrRecConnection::NrTapePump::NrTapePump
     ( NrRecConnection *pConnection, int AimSize
     , EzString LogPrefix
     , char SampleRate
     )
     : NrRecPump (AimSize, LogPrefix, SampleRate)
{
    NrTapePump::TapeFd      = -1;
    NrTapePump::pConnection = pConnection;
};

NrRecConnection::NrTapePump::~NrTapePump (void)
{
};

void NrRecConnection::NrTapePump::SetMissData (int Flag)
{
    pConnection->HandleMissData (Flag);
};

void NrRecConnection::NrTapePump::SetPrepare (int Flag)
{
    pConnection->HandlePrepare (Flag);
};

void NrRecConnection::NrTapePump::SetExpansion (int Expansion)
{
    pConnection->HandleExpansion (Expansion);
};

void NrRecConnection::NrTapePump::WriteTape   (EzString Data)
{
    if (TapeFd != -1) {
        write (TapeFd, Data.Text (), Data.Length ());
    };
};

void NrRecConnection::NrTapePump::SetRecording (int Flag)
{
    if (Flag) {
        if (TapeFd == -1) {
            TapeFd = open ( "default.tape"
                      , O_WRONLY | O_APPEND | O_CREAT, 0777);
            if (TapeFd == -1) {
                cerr << "Couldn't open NrRecFrontend.tape ("
                     << errno  << ")" << endl;
            };
        };
    } else {
        if (TapeFd != -1) {
            close (TapeFd);
            TapeFd = -1;
        };
    };
};

void NrRecConnection::HandleMissData (int Flag)
{
};

void NrRecConnection::HandlePrepare (int Flag)
{
};

void NrRecConnection::HandleExpansion (int Expansion)
{
};

void NrRecConnection::PhReadyHandleMessage   (NrMsgCode MsgCode, EzString Data)
{
};

void NrRecConnection::PhTuneHandleMessage   (NrMsgCode MsgCode, EzString Data)
{
    if (MsgCode == MsgTuning) {
        pCurMsgHandler = &NrRecConnection::PhReadyHandleMessage;
        Initialized = 1;
        HandleInitDone ();
    } else {
        CloseRec ();
    };
};

void NrRecConnection::PhMuteHandleMessage   (NrMsgCode MsgCode, EzString Data)
{
    if (MsgCode == MsgMuteOff || MsgCode == MsgMuteOn) {
        pCurMsgHandler = &NrRecConnection::PhReadyHandleMessage;
        Initialized = 1;
        HandleInitDone ();
        if (Freq != 0) DoTune (Freq);
    } else {
        CloseRec ();
    };
};

void NrRecConnection::PhModeHandleMessage   (NrMsgCode MsgCode, EzString Data)
{
    if (MsgCode == MsgModeReceiver) {
        pCurMsgHandler = &NrRecConnection::PhMuteHandleMessage;
        SendMessage ((MuteFlag ? MsgMuteOn : MsgMuteOff), "");
    } else {
        CloseRec ();
    };
};

void NrRecConnection::PhClientHandleMessage   (NrMsgCode MsgCode, EzString Data)
{
    switch (MsgCode) {
    case MsgServerInfo:
        SetFeatureLevel (int (Data));
        break;
    case MsgNoMode:
        SetFeatureLevel (0);
        break;
    default:
        CloseRec ();
        return;
    };

    pCurMsgHandler = &NrRecConnection::PhModeHandleMessage;
    SendMessage (MsgModeReceiver, "");
};

void NrRecConnection::PhBinaryHandleMessage (NrMsgCode MsgCode, EzString Data)
{
    if (MsgCode == MsgBinary) {
        pCurMsgHandler = &NrRecConnection::PhClientHandleMessage;
        SendMessage
            ( MsgClientInfo
            ,   EzString (FEATURE_LEVEL) + EzString (" ") + GetUname ()
              + EzString (" ") + AddInfo
            );
    } else {
        CloseRec ();
    };
};

void NrRecConnection::PhHelloHandleMessage (NrMsgCode MsgCode, EzString Data)
{
    if (MsgCode == MsgReady) {
        SendMessage (MsgBinary, "");

        pCurMsgHandler = &NrRecConnection::PhBinaryHandleMessage;
    } else {
        CloseRec ();
    };
};

void NrRecConnection::HandleMessage (NrMsgCode MsgCode, EzString Data)
{
    EzString Freq, Id;

    switch (MsgCode) {
    case MsgMuteOn:
        HandleMute (1);
        break;
    case MsgMuteOff:
        HandleMute (0);
        break;
    case MsgSignal:
        Pump.Reset ();
        Freq = GetWord (Data);
        HandleSignal (Freq, Data);
        break;
    case MsgNoSignal:
        HandleNoSignal (Data);
        break;
    case MsgTuning:
        HandleTuning (Data);
        break;
    default:
        break;
    };
    switch (MsgCode) {
    case MsgToAll:
    case MsgToReceiver:
    case MsgToTuned:
        Id = Upper (GetWord (Data));
        HandleMessage (MsgCode, Id, Data);
        break;
    default:
        (this->*pCurMsgHandler)(MsgCode, Data);
        break;
    };
};

void NrRecConnection::HandleRead (EzString Data)
{
    NrMsgCode    MsgCode;

    if (!SoundDev.IsOpen()) SetMute(1);

    Pump.PutData (Data);

    Pump.SetOsSoundDelay (SoundDev.GetIntOutDelay ());
    while (Pump.GetMessage (MsgCode, Data, XxTimer::GetMsCurTime ())) {
        HandleMessage (MsgCode, Data);
    };
};

void NrRecConnection::OnConnect (void)
{
    // Actually not needed: Interaction is
    // intiated by READY message from server

    NrRadioConnection::OnConnect ();
};

void NrRecConnection::HandleTuning (int Freq)
{
};

void NrRecConnection::HandleSignal (int Freq, EzString Description)
{
};

void NrRecConnection::HandleNoSignal (int Freq)
{
};

void NrRecConnection::HandleMute (int Flag)
{
};

void NrRecConnection::HandleInitDone (void)
{
};

void NrRecConnection::HandleStationName (EzString Name)
{
};

void NrRecConnection::HandleMessage
    (NrMsgCode MsgCode, EzString Id, EzString Mesg)
{
};

int NrRecConnection::DoTune     (int Freq)
{
    if (Initialized) SendMessage (MsgTune,     NumberToString (Freq));

    return Initialized;
};

int NrRecConnection::DoTuneUp   (int Freq)
{
    if (Initialized) SendMessage (MsgTuneUp,   NumberToString (Freq));

    return Initialized;
};

int NrRecConnection::DoTuneDown (int Freq)
{
    if (Initialized) SendMessage (MsgTuneDown, NumberToString (Freq));

    return Initialized;
};

void NrRecConnection::SetRecording (int Flag)
{
    Pump.SetRecording (Flag);
};

int NrRecConnection::SetMute (int Flag)
{
    if (Flag == -1) Flag = !MuteFlag;

    if (Flag == MuteFlag) return MuteFlag;

    if (Flag) {
        SoundDev.Close ();
    } else {
        Flag = !OpenDev ();
    };

    Pump.SetMute (Flag);

    if (Flag == MuteFlag) return MuteFlag;

    SendMessage ((Flag ? MsgMuteOn : MsgMuteOff), "");

    MuteFlag = Flag;

    return MuteFlag;
};
