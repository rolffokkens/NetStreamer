/*
 * file: NrNetRadio.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_NETRADIO
#define H_NETRADIO

#include "EzString.h"
#include "XxSocket.h"
#include "NrMessages.h"

class NrRadioConnection : public XxConnection {
public:
    NrMessageDecoder MessageDecoder;
    NrMessageEncoder MessageEncoder;
private:
    EzString LogPrefix;
    EzString InBuf;
    int      AudioMute;
    int      FeatureLevel;

    virtual void HandleMessage (NrMsgCode MsgCode, EzString Data) = 0;
//    void SendMessage (EzString Data);
protected:
    void SendMessage (NrMsgCode MsgCode, EzString Data);

    NrRadioConnection
        ( int Fd
        , const XxSocketAddr &MyAddr, const XxSocketAddr &HisAddr
        , EzString LogPrefix
        );
    NrRadioConnection (EzString LogPrefix);

    virtual void OnDisconnect (void);
    virtual void HandleRead (EzString Data);

    void SetFeatureLevel (int Level);

    EzString GetUname (void);
public:
    void SetAudioMute    (int Mute)  { AudioMute = Mute;    };
    int  GetAudioMute    (void)      { return AudioMute;    };
    int  GetFeatureLevel (void)      { return FeatureLevel; };
};

#endif
