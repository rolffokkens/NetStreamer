/*
 * file: NrServer.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_NR_SERVER
#define H_NR_SERVER

#include "BiLink.h"
#include "EzString.h"
#include "XxTimer.h"
#include "XxSocket.h"
#include "NrNetRadio.h"

class NrServerReceiver;
class NrServerTransmitter;
class NrServerConnection;

typedef BlSet<NrServerConnection>  NrConnSet;
typedef BlSet<NrServerTransmitter> NrServerTransmitterSet;
typedef BlSet<NrServerReceiver>    NrServerReceiverSet;

enum NrServerRadioStatus {StatOK, StatTuning, StatStop};

class NrServerHandler {
    friend NrServerConnection;
private:
    int HandlerId;

    NrServerConnection *pConnection;
protected:
    void SetStatus (NrServerRadioStatus rStatus);
    NrServerRadioStatus GetStatus (void);
    void SendMessage (NrMsgCode MsgCode, EzString Data);

    virtual void SetAudioMute (int Mute);

    int  GetAudioMute    (void);
    int  GetFeatureLevel (void);

    void Remove (void);

    int  GetConnectionId (void);

    NrServerReceiverSet::SetOwner    &GetRecOwner (void);
    NrServerTransmitterSet::SetOwner &GetTransOwner (void);
public:
    virtual NrMsgCode HandleCmd
        (NrMsgCode MsgCode, EzString Line, EzString &Data) = 0;
    NrServerHandler
        ( int HandlerId, NrServerConnection *pConnection) {
        NrServerHandler::pConnection = pConnection;
    };
    virtual ~NrServerHandler (void);
    EzString GetAddr (void);
    virtual EzString GetType (void) = 0;
};

class NrServerReceiver : public NrServerHandler {
    friend NrServerTransmitter;
private:
    typedef BlSet2<NrServerTransmitter,NrServerReceiver> NrTranRecSet;
    typedef tXxTimer<NrServerReceiver>                   NrServerReceiverTimer;

    friend NrServerReceiver::NrTranRecSet;
    friend NrServerReceiver::NrServerReceiverTimer;

    int                   TryFreq;
    int                   TuneDir;
    NrTranRecSet          TranRecMember;
    NrServerReceiverSet   RecMember;
    NrServerReceiverTimer Timer;

    void TrigTranRecAttach (NrServerTransmitter *pTrans);
    void TrigTranRecDetach (NrServerTransmitter *pTrans);

    void TryReceive (NrServerTransmitter *pTrans, int Freq);
    void HandleTimeOut (int Count);
    void UpdateTuneStatus (NrServerTransmitter *pTrans);

    NrMsgCode HandleCmdTune (EzString Line, EzString &Data, int Dir);
    void SetAudioMute (int Mute);
    void DeliverAudio (NrMsgCode MsgCode, EzString Audio);
public:
    void Receive (EzString Data);
    NrMsgCode HandleCmd (NrMsgCode MsgCode, EzString Line, EzString &Data);
    NrServerReceiver
        (int HandlerId, NrServerConnection *pConnection);

    NrServerTransmitter *GetTransmitter (void);
    NrServerReceiver *GetNextAnyReceiver (void) { return RecMember.GetNext (); };
    virtual ~NrServerReceiver (void) { SetAudioMute (1); };

    EzString GetType (void);
};

class NrServerTransmitter : public NrServerHandler {
    friend NrServerReceiver;
private:
    typedef tXxTimer<NrServerTransmitter> NrServerTransmitterTimer;

    friend NrServerTransmitter::NrServerTransmitterTimer;

    NrServerTransmitterSet         TransMember;
    int                            MuteOffCount;
    int                            Freq;
    EzString                       Description;
    NrServerReceiver::NrTranRecSet TranRecOwner;
    NrServerTransmitterTimer       Timer;
    int                            MissPingCount;

    void      AdjustMuteCount  (int Mute);
    NrMsgCode HandleCmdAudio
                  (NrMsgCode MsgCode, EzString Line, EzString &Data);
    NrMsgCode HandleCmdToTuned
                  (EzString Line, EzString &Data);
    void      HandleTimeOut    (int Count);
public:
    void Transmit (EzString Data);
    NrMsgCode HandleCmd (NrMsgCode MsgCode, EzString Line, EzString &Data);
    NrServerTransmitter
        (int HandlerId, NrServerConnection *pConnection);

    NrServerTransmitter *GetNextAnyTransmitter (void) {
        return TransMember.GetNext ();
    };
    NrMsgCode SetProperties (int  Freq, EzString  Description);
    void    GetProperties (int &Freq, EzString &Description);

    EzString GetType (void);
};

class NrServerListener : public XxListener {
    friend NrServerConnection;
private:
    NrConnSet::SetOwner              ListOwner;
    NrConnSet::SetOwner              ConnOwner;
    NrServerReceiverSet::SetOwner    RecOwner;
    NrServerTransmitterSet::SetOwner TransOwner;

    XxConnection *GetNewConnection
        (int Fd, const XxSocketAddr &MyAddr, const XxSocketAddr &HisAddr);
protected:
    virtual void OnAccept (void);
public:
    virtual ~NrServerListener (void);
};

class NrServerConnection : public NrRadioConnection {
    friend NrServerHandler;
private:
    int                       ConnectionId;
    NrServerListener         *pListener;
    NrServerHandler          *pHandler;
    NrServerRadioStatus       rStatus;
    NrConnSet                 ConnMember;
    NrConnSet                 ListMember;
    EzString                  ClientInfo;

    NrMsgCode HandleCmdModeRec    (EzString Line, EzString &Data);
    NrMsgCode HandleCmdModeTrans  (EzString Line, EzString &Data);
    NrMsgCode HandleCmdListRec    (EzString Line, EzString &Data);
    NrMsgCode HandleCmdListTrans  (EzString Line, EzString &Data);
    NrMsgCode HandleCmdListConn   (EzString Line, EzString &Data);
    NrMsgCode HandleCmdToAll      (EzString Line, EzString &Data);
    NrMsgCode HandleCmdToRec      (EzString Line, EzString &Data);
    NrMsgCode HandleCmdToTrans    (EzString Line, EzString &Data);
    NrMsgCode HandleCmdClientInfo (EzString Line, EzString &Data);
    NrMsgCode HandleCmdStatConn   (EzString Line, EzString &Data);

    NrMsgCode HandleCmd
          (NrMsgCode MsgCode, EzString Line, EzString &Data);
protected:
    void HandleMessage   (NrMsgCode MsgCode, EzString Message);
    void OnDisconnect    (void) { Remove (); };

    NrServerReceiverSet::SetOwner &GetRecOwner (void) {
        return pListener->RecOwner;
    };

    NrServerTransmitterSet::SetOwner &GetTransOwner (void) {
        return pListener->TransOwner;
    };
public:
    static void Init (void);

    NrServerConnection
        ( NrServerListener *pListener
        , int ConnectionId
        , int Fd
        , const XxSocketAddr &MyAddr, const XxSocketAddr &HisAddr
        );
    virtual ~NrServerConnection (void);

    int  GetConnectionId (void) { return ConnectionId; };

    NrServerListener *GetListener (void) {return pListener; };
    EzString GetType (void);
};

inline EzString NrServerHandler::GetAddr (void)
{
    return pConnection->GetHisAddr ();
};

inline NrServerRadioStatus NrServerHandler::GetStatus (void)
{
    return pConnection->rStatus;
};

inline NrServerTransmitter *NrServerReceiver::GetTransmitter (void)
{
    return TranRecMember.GetOwn ();
};

inline void NrServerHandler::SetStatus (NrServerRadioStatus rStatus)
{
    pConnection->rStatus = rStatus;
};

inline void NrServerHandler::SendMessage (NrMsgCode MsgCode, EzString Data)
{
    pConnection->SendMessage (MsgCode, Data);
};

inline int NrServerHandler::GetAudioMute (void)
{
    return pConnection->GetAudioMute ();
};

inline int NrServerHandler::GetFeatureLevel (void)
{
    return pConnection->GetFeatureLevel ();
};

inline NrServerReceiverSet::SetOwner &NrServerHandler::GetRecOwner (void) {
    return pConnection->GetRecOwner ();
};

inline NrServerTransmitterSet::SetOwner &NrServerHandler::GetTransOwner (void) {
    return pConnection->GetTransOwner ();
};

#endif;
