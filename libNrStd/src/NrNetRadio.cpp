/*
 * file: NrNetRadio.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <netinet/in.h>
#include <sys/utsname.h>

#include "EzString.h"
#include "NrNetRadio.h"

NrRadioConnection::NrRadioConnection
    ( int Fd
    , const XxSocketAddr &MyAddr, const XxSocketAddr &HisAddr
    , EzString LogPrefix
    )
    : XxConnection (Fd, MyAddr, HisAddr) 
    , MessageDecoder (LogPrefix), MessageEncoder (LogPrefix)
{
    NrRadioConnection::AudioMute    = 1;
    NrRadioConnection::FeatureLevel = 0;
    NrRadioConnection::LogPrefix    = LogPrefix;
};

NrRadioConnection::NrRadioConnection (EzString LogPrefix)
    : MessageDecoder (LogPrefix), MessageEncoder (LogPrefix)
{
    NrRadioConnection::AudioMute    = 1;
    NrRadioConnection::LogPrefix    = LogPrefix;
};

void NrRadioConnection::OnDisconnect ()
{
    LogMessage
        (  LogPrefix + EzString (" Disconnected")
        );

    Remove ();
};

void NrRadioConnection::SendMessage (NrMsgCode MsgCode, EzString Data)
{
    if (MsgCode == MsgAudioData || MsgCode == MsgNumAudioData) {
        if (GetAudioMute ()) return;

        if (MsgCode == MsgNumAudioData && GetFeatureLevel () < 1) {
            MsgCode = MsgAudioData;
            Data = Substr (Data, 2);
        };

        if (GetOutBufSize () > 10000) {
            LogMessage
                (   LogPrefix + EzString (" Dropped ")
                  + EzString (int (Data.Length ()))
                  + EzString (" Audio")
                );
            return;
        };
    };

    Write (MessageEncoder.PutMessage (MsgCode, Data));
};

void NrRadioConnection::HandleRead    (EzString Data)
{
    NrMsgCode    MsgCode;

    MessageDecoder.PutData (Data);

    while (MessageDecoder.GetMessage (MsgCode, Data)) {
        HandleMessage (MsgCode, Data);
    };
};

void NrRadioConnection::SetFeatureLevel (int Level)
{
    if (Level <= FEATURE_LEVEL) FeatureLevel = Level;
};

EzString NrRadioConnection::GetUname (void)
{
    struct utsname Uname;

    uname (&Uname);

    return   EzString (Uname.sysname) + EzString (" ")
           + EzString (Uname.release) + EzString (" ")
           + EzString (Uname.machine);
};
