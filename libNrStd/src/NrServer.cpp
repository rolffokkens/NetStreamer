/*
 * file: NrServer.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <iostream>

#include "XxSocket.h"
#include "NrServer.h"
#include "NrNetRadio.h"
#include "XxAutoInit.h"

static int InitDone = 0;

#define FREQ_MIN  880
#define FREQ_MAX 1080

NrServerHandler::~NrServerHandler (void)
{
    pConnection->pHandler = NULL;
};

int NrServerHandler::GetConnectionId (void) {
    return pConnection->GetConnectionId ();
};

void NrServerHandler::SetAudioMute (int Mute)
{
    pConnection->SetAudioMute (Mute);
};

void NrServerHandler::Remove (void)
{
    pConnection->Remove ();
};

void NrServerTransmitter::HandleTimeOut (int Count)
{
    MissPingCount++;
    if (MissPingCount < 6) {
        SendMessage (MsgPing, "");
    } else {
        LogMessage
            (  EzString ("Conn [") + EzString (GetConnectionId ())
             + EzString ("] Ping Timeout")
            );

        Remove ();
    };
};

void NrServerReceiver::SetAudioMute (int Mute)
{
    NrServerTransmitter *pTrans;

    if (GetAudioMute () != Mute) {
        NrServerHandler::SetAudioMute (Mute);
        pTrans = GetTransmitter ();
        if (pTrans != NULL) pTrans->AdjustMuteCount (Mute);
    };
};

void NrServerReceiver::TryReceive (NrServerTransmitter *pTrans, int Freq)
{
    if (Freq == TryFreq && GetStatus () != StatTuning) {
        TranRecMember.Attach (pTrans->TranRecOwner);
    };
};

template <>
void NrServerReceiver::NrTranRecSet::TrigAttach (NrServerReceiver *pRec, NrServerTransmitter *pTrans)
{
    pRec->TrigTranRecAttach (pTrans);
};

template <>
void NrServerReceiver::NrTranRecSet::TrigDetach (NrServerReceiver *pRec, NrServerTransmitter *pTrans)
{
    pRec->TrigTranRecDetach (pTrans);
};

template <>
void NrServerReceiver::NrServerReceiverTimer::HandleTimeOut (int Count)
{
    pOwner->HandleTimeOut (Count);
};

NrServerReceiver::NrServerReceiver
    ( int HandlerId
    , NrServerConnection *pConnection
    )
    : NrServerHandler (HandlerId, pConnection)
    , Timer (this)
    , RecMember (this)
    , TranRecMember (this)
{
    TryFreq = 0;
    TuneDir = 0;
    RecMember.Attach (GetRecOwner ());

    LogMessage
        (  EzString ("Conn [") + EzString (GetConnectionId ())
         + EzString ("] is now a Receiver")
        );
};

EzString NrServerReceiver::GetType (void)
{
    return EzString ("RECEIVER");
};

EzString NrServerTransmitter::GetType (void) 
{
    return EzString ("TRANSMITTER");
};

template <>
void NrServerTransmitter::NrServerTransmitterTimer::HandleTimeOut (int Count)
{
    pOwner->HandleTimeOut (Count);
};

void NrServerTransmitter::AdjustMuteCount (int Mute)
{
    if (Mute) {
        MuteOffCount--;
        if (MuteOffCount == 0) {
            SendMessage (MsgMuteOn, "");
        };
    } else {
        if (MuteOffCount == 0) {
            SendMessage (MsgMuteOff, "");
        };
        MuteOffCount++;
    };
};

void NrServerReceiver::TrigTranRecAttach (NrServerTransmitter *pTrans)
{
    int    Freq;
    EzString Description;

    if (!GetAudioMute ()) pTrans->AdjustMuteCount (0);

    if (GetStatus () != StatTuning) {
        UpdateTuneStatus (pTrans);
    };
};

void NrServerReceiver::TrigTranRecDetach (NrServerTransmitter *pTrans)
{
    if (!GetAudioMute ()) pTrans->AdjustMuteCount (1);

    if (GetStatus () != StatTuning) {
        UpdateTuneStatus (NULL);
    };
};

NrMsgCode NrServerReceiver::HandleCmdTune (EzString Line, EzString &Data, int Dir)
{
    int Freq;

    SetStatus (StatOK);
    Timer.Stop ();
    TuneDir = 0;
    Freq    = Line;

    switch (Dir) {
    case -1:
        if (Freq == FREQ_MIN - 1) Freq = FREQ_MAX;
        break;
    case  1:
        if (Freq == FREQ_MAX + 1) Freq = FREQ_MIN;
        break;
    };

    if (Freq < FREQ_MIN || Freq > FREQ_MAX) {
        return MsgBadFrequency;
    };

    SetStatus (StatTuning);

    TranRecMember.Detach ();
    TryFreq = Freq;
    TuneDir = Dir;

    if (Dir == 0) {
        Timer.SetTimeOut (50);
    } else {
        Timer.SetRestart (50);
    };

    Data = NumberToString (TryFreq);
    return MsgTuning;
};

NrMsgCode NrServerReceiver::HandleCmd (NrMsgCode MsgCode, EzString Line, EzString &Data)
{
    switch (MsgCode) {
    case MsgTune:
        return HandleCmdTune (Line, Data,  0);
    case MsgTuneUp:
        return HandleCmdTune (Line, Data,  1);
    case MsgTuneDown:
        return HandleCmdTune (Line, Data, -1);
    case MsgMuteOn:
        SetAudioMute (1);
        return MsgMuteOn;
    case MsgMuteOff:
        SetAudioMute (0);
        return MsgMuteOff;
    };

    return MsgBadCommand;
};

NrServerTransmitter::NrServerTransmitter
    ( int HandlerId
    , NrServerConnection *pConnection
    )
    : NrServerHandler (HandlerId, pConnection)
    , TranRecOwner (this)
    , TransMember (this)
    , Timer (this, 5000)
{
    Freq          = 0;
    MuteOffCount  = 0;
    MissPingCount = 0;

    TransMember.Attach (GetTransOwner ());

    LogMessage
        (  EzString ("Conn [") + EzString (GetConnectionId ())
         + EzString ("] is now a Transmitter")
        );
};

void NrServerReceiver::DeliverAudio (NrMsgCode MsgCode, EzString Audio)
{
    if (!GetAudioMute ()) SendMessage (MsgCode, Audio);
};

NrMsgCode NrServerTransmitter::HandleCmdAudio
    (NrMsgCode MsgCode, EzString Line, EzString &Data)
{
    NrServerReceiver *pRec;

    pRec = TranRecOwner.GetNext ();

    while (pRec != NULL) {
        pRec->DeliverAudio (MsgCode, Line);
        pRec = pRec->TranRecMember.GetNext ();
    }
    return MsgNoMessage;
};

NrMsgCode NrServerTransmitter::HandleCmdToTuned (EzString Line, EzString &Data)
{
    NrServerReceiver *pRec;

    pRec = TranRecOwner.GetNext ();

    while (pRec != NULL) {
        pRec->SendMessage (MsgToTuned, Line);
        pRec = pRec->TranRecMember.GetNext ();
    }
    return MsgNoMessage;
};

NrMsgCode NrServerTransmitter::HandleCmd (NrMsgCode MsgCode, EzString Line, EzString &Data)
{
    switch (MsgCode) {
    case MsgPing:
        MissPingCount = 0;
        return MsgNoMessage;
    case MsgAudioData:
    case MsgNumAudioData:
        return HandleCmdAudio (MsgCode, Line, Data);
    case MsgToTuned:
        return HandleCmdToTuned (Line, Data);
    };
    return MsgBadCommand;
};

NrMsgCode NrServerTransmitter::SetProperties (int Freq, EzString  Description)
{
    NrServerTransmitter *pTrans;
    NrServerReceiver    *pRec;

    if (Freq < FREQ_MIN || Freq > FREQ_MAX) return MsgBadFrequency;
    if (Description == EzString("")) return MsgBadProperties;

    // pTrans = pTransOwner->GetFirst ();
    pTrans = GetTransOwner ().GetFirst ();

    while (pTrans != NULL) {
        if (Freq == pTrans->Freq) return MsgFrequencyInUse;
        pTrans = pTrans->GetNextAnyTransmitter ();
    };

    NrServerTransmitter::Freq        = Freq;
    NrServerTransmitter::Description = Description;

    // pRec = pRecOwner->GetFirst ();
    pRec = GetRecOwner ().GetFirst ();

    while (pRec != NULL) {
        pRec->TryReceive (this, Freq);
        pRec = pRec->GetNextAnyReceiver ();
    };

    return MsgModeTransmitter;
};

void NrServerTransmitter::GetProperties (int &Freq, EzString &Description)
{
    Freq        = NrServerTransmitter::Freq;
    Description = NrServerTransmitter::Description;
};

NrServerListener::~NrServerListener (void)
{
    NrServerConnection *pConnection;

    for (;;) {
        pConnection = ListOwner.GetFirst ();
        if (pConnection == NULL) break;
        delete pConnection;
    };
};

static int ConnectionIdCount = 0;

XxConnection *NrServerListener::GetNewConnection
    (int Fd, const XxSocketAddr &MyAddr, const XxSocketAddr &HisAddr)
{
    NrServerConnection *pConnection;

    pConnection = new NrServerConnection
                         (this, ConnectionIdCount++, Fd, MyAddr, HisAddr);

    return pConnection;
};

void NrServerListener::OnAccept (void)
{
    XxConnection *pConnection;

    pConnection = Accept ();
};

NrServerConnection::~NrServerConnection (void)
{
    if (pHandler != NULL) delete pHandler;

    LogMessage
        (  EzString ("Conn [") + EzString (GetConnectionId ())
         + EzString ("] Removed")
        );
};

NrMsgCode NrServerConnection::HandleCmdModeRec (EzString Line, EzString &Data)
{
    if (pHandler != NULL) delete pHandler;

    pHandler = new NrServerReceiver (0, this);

    return MsgModeReceiver;
};

NrMsgCode NrServerConnection::HandleCmdModeTrans (EzString Line, EzString &Data)
{
    int         Freq;
    NrMsgCode     RetVal;
    NrServerTransmitter *pTrans;

    if (pHandler != NULL) delete pHandler;

    Freq     = GetWord (Line);
    pTrans   = new NrServerTransmitter (0, this);
    RetVal   = pTrans->SetProperties (Freq, Line);
    if (RetVal == MsgModeTransmitter) {
        pHandler = pTrans;
    } else {
        delete pTrans;
    };

    return RetVal;
};

NrMsgCode NrServerConnection::HandleCmdListRec (EzString Line, EzString &Data)
{
    int                  Freq;
    EzString             Description;
    NrServerTransmitter *pTrans;
    NrServerReceiver    *pRec;

    // pRec = pRecOwner->GetFirst ();
    pRec = GetRecOwner ().GetFirst ();

    while (pRec != NULL) {
        Data =   EzString (pRec->pConnection->GetConnectionId ())
               + EzString (" ") + pRec->GetAddr ();

        pTrans = pRec->GetTransmitter ();
        if (pTrans != NULL) {
            pTrans->GetProperties (Freq, Description);
            Data += EzString (" ") + NumberToString (Freq);
        };
        SendMessage (MsgReceiverInfo, Data);
        pRec = pRec->GetNextAnyReceiver ();
    };

    return MsgNoMessage;
};

NrMsgCode NrServerConnection::HandleCmdListConn (EzString Line, EzString &Data)
{
    NrServerConnection *pConn;

    pConn = pListener->ConnOwner.GetFirst ();

    while (pConn != NULL) {
        Data =   EzString (pConn->GetConnectionId ()) + EzString (" ")
               + pConn->GetHisAddr () + EzString (" ") + pConn->GetType ()
               + EzString (" ") + pConn->ClientInfo;
        SendMessage (MsgConnectionInfo, Data);
        pConn = pConn->ConnMember.GetNext ();
    };

    return MsgNoMessage;
};

NrMsgCode NrServerConnection::HandleCmdToAll (EzString Line, EzString &Data)
{
    NrServerConnection *pConnection;

    pConnection = ListMember.GetFirst ();

    while (pConnection != NULL) {
        pConnection->SendMessage (MsgToAll, Line);
        pConnection = pConnection->ListMember.GetNext ();
    };
    return MsgNoMessage;
};

NrMsgCode NrServerConnection::HandleCmdToRec (EzString Line, EzString &Data)
{
    NrServerReceiver    *pRec;

    // pRec = pRecOwner->GetFirst ();
    pRec = GetRecOwner ().GetFirst ();

    while (pRec != NULL) {
        pRec->SendMessage (MsgToReceiver, Line);
        pRec = pRec->GetNextAnyReceiver ();
    };

    return MsgNoMessage;
};

NrMsgCode NrServerConnection::HandleCmdToTrans (EzString Line, EzString &Data)
{
    NrServerTransmitter *pTrans;

    pTrans = GetTransOwner ().GetFirst ();

    while (pTrans != NULL) {
        pTrans->SendMessage (MsgToTransmitter, Line);
        pTrans = pTrans->GetNextAnyTransmitter ();
    };

    return MsgNoMessage;
};

NrMsgCode NrServerConnection::HandleCmdClientInfo
    (EzString Line, EzString &Data)
{
    ClientInfo = Line;

    Data = EzString (FEATURE_LEVEL) + EzString (" ") + GetUname ();

    SetFeatureLevel (int (Line));

    return MsgServerInfo;
};

NrMsgCode NrServerConnection::HandleCmdListTrans (EzString Line, EzString &Data)
{
    int         Freq;
    EzString      Description;
    NrServerTransmitter *pTrans;

    // pTrans = pTransOwner->GetFirst ();
    pTrans = GetTransOwner ().GetFirst ();

    while (pTrans != NULL) {
        pTrans->GetProperties (Freq, Description);
        Data =   EzString (pTrans->pConnection->GetConnectionId ())
               + EzString (" ") + pTrans->GetAddr () + EzString (" ")
               + NumberToString (Freq) + EzString (" ") + Description;

        SendMessage (MsgTransmitterInfo, Data);
        pTrans = pTrans->GetNextAnyTransmitter ();
    };

    return MsgNoMessage;
};

NrMsgCode NrServerConnection::HandleCmdStatConn (EzString Line, EzString &Data)
{
    NrServerConnection *pConn;
    int                StatId;

    StatId = int (GetWord (Line));

    pConn = pListener->ConnOwner.GetFirst ();

    while (pConn != NULL) {
        if (StatId == pConn->GetConnectionId ()) {
            Data = EzString ("IN_COUNT ")  + EzString (pConn->GetInCount  ());
            SendMessage (MsgConnectionStatus, Data);
            Data = EzString ("OUT_COUNT ") + EzString (pConn->GetOutCount ());
            SendMessage (MsgConnectionStatus, Data);
            return MsgNoMessage;
        };
        pConn = pConn->ConnMember.GetNext ();
    };

    return MsgNoMessage;
};

NrMsgCode NrServerConnection::HandleCmd
    (NrMsgCode MsgCode, EzString Line, EzString &Data)
{
    int     Freq;

    switch (MsgCode) {
    case MsgModeReceiver:
        return HandleCmdModeRec    (Line, Data);
    case MsgModeTransmitter:
        return HandleCmdModeTrans  (Line, Data);
    case MsgListReceiver:
        return HandleCmdListRec    (Line, Data);
    case MsgListTransmitter:
        return HandleCmdListTrans  (Line, Data);
    case MsgListConnection:
        return HandleCmdListConn   (Line, Data);
    case MsgToAll:
        return HandleCmdToAll      (Line, Data);
    case MsgToReceiver:
        return HandleCmdToRec      (Line, Data);
    case MsgToTransmitter:
        return HandleCmdToTrans    (Line, Data);
    case MsgBinary:
        SendMessage (MsgBinary, "");
        return MsgNoMessage;
    case MsgQuit:
        Remove ();
        return MsgNoMessage;
    case MsgNoMessage:
        return MsgBadCommand;
    case MsgClientInfo:
        return HandleCmdClientInfo (Line, Data);
    case MsgStatConnection:
        return HandleCmdStatConn   (Line, Data);
    };

    if (pHandler == NULL) {
        Data = "";
        return MsgNoMode;
    };

    return pHandler->HandleCmd (MsgCode, Line, Data);
};

void NrServerConnection::HandleMessage (NrMsgCode MsgCode, EzString Message)
{
    EzString Data;

    MsgCode = HandleCmd (MsgCode, Message, Data);

    SendMessage (MsgCode, Data);
};

void NrServerConnection::Init (void)
{
    if (InitDone) return;

    AUTOINIT_TRACE ("NrServerConnection::Init");
    InitDone = 1;
};

EzString NrServerConnection::GetType (void)
{
    if (pHandler == NULL) {
        return EzString ("-");
    };
    return pHandler->GetType ();
};

void NrServerReceiver::UpdateTuneStatus (NrServerTransmitter *pTrans)
{
    int         Freq;
    EzString      Description;

    if (pTrans != NULL) {
        pTrans->GetProperties (Freq, Description);
        SendMessage
            ( MsgSignal
            , NumberToString (Freq) + EzString (" ") + Description
            );
    } else {
        SendMessage
            ( MsgNoSignal
            , NumberToString (TryFreq)
            );
    };
};

void NrServerReceiver::HandleTimeOut (int Count)
{
    NrServerTransmitter *pTrans;
    int         AdjTryFreq;

    if (GetStatus () != StatTuning) {
        Timer.Stop ();
        return;
    };
    // pTrans = pTransOwner->GetFirst ();
    pTrans = GetTransOwner ().GetFirst ();

    while (pTrans != NULL) {
        if (TryFreq == pTrans->Freq) break;
        pTrans = pTrans->GetNextAnyTransmitter ();
    };

    if (pTrans != NULL) {
        TranRecMember.Attach (pTrans->TranRecOwner);
        TuneDir = 0;
        SetStatus (StatOK);
        Timer.Stop ();
        AdjTryFreq = 0;
    } else {
        switch (TuneDir) {
        case -1:
            AdjTryFreq = -1;
            if (TryFreq < FREQ_MIN) TryFreq = FREQ_MAX;
            break;
        case  1:
            AdjTryFreq = +1;
            if (TryFreq > FREQ_MAX) TryFreq = FREQ_MIN;
            break;
        default:
            AdjTryFreq = 0;
            SetStatus (StatOK);
            break;
        };
    };
    UpdateTuneStatus (pTrans);

    TryFreq += AdjTryFreq;
};

static EzString GetConnPrefix (int ConnectionId)
{
    return EzString ("Conn [") + EzString (ConnectionId) + EzString ("]");
};

NrServerConnection::NrServerConnection
    ( NrServerListener *pListener
    , int ConnectionId
    , int Fd
    , const XxSocketAddr &MyAddr, const XxSocketAddr &HisAddr
    )
    : NrRadioConnection (Fd, MyAddr, HisAddr, GetConnPrefix (ConnectionId))
    , ConnMember (this)
    , ListMember (this)
{
    Init ();

    NrServerConnection::rStatus      = StatOK;
    NrServerConnection::pHandler     = NULL;
    NrServerConnection::ConnectionId = ConnectionId;
    NrServerConnection::pListener    = pListener;

    ListMember.Attach (pListener->ListOwner);
    // ConnMember.Attach (*pConnOwner);
    ConnMember.Attach (pListener->ConnOwner);

    LogMessage
        (  EzString ("Conn [")  + EzString (GetConnectionId ())
         + EzString ("] from ") + EzString (GetHisAddr ())
        );

    SendMessage (MsgReady, "Radio Server " NR_VERSION);
};

