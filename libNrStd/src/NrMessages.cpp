/*
 * file: NrMessages.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

/*
#include <stdlib.h>
*/
#include <time.h>
#include "EzString.h"
#include "XxTimer.h"
#include "NrMessages.h"

int RateHerz[RateCount] = {
    8000
,   16000
};

static DoLogFlag = -1;

inline short SizeHostToNet (short i)
{
    unsigned char *cp = (unsigned char *) (&i);

    return (cp[0] | (cp[1] << 8));
};

inline short SizeNetToHost (short i)
{
    short RetVal;
    unsigned char *cp = (unsigned char *) (&RetVal);

    cp[0] = (char) i;
    cp[1] = (char) (i >> 8);

    return RetVal;
};

struct NrIntMsgInfo {
    EzString  Text;
    NrIntMsgInfo (const char *Text) {
        NrIntMsgInfo::Text = Text;
    };
};

static NrIntMsgInfo MsgInfo[MsgCount] = {
    NrIntMsgInfo ("READY"             )
,   NrIntMsgInfo ("BINARY"            )
,   NrIntMsgInfo ("NO_MODE_SELECTED"  )
,   NrIntMsgInfo ("TUNE"              )
,   NrIntMsgInfo ("RECEIVER_INFO"     )

,   NrIntMsgInfo ("TRANSMITTER_INFO"  )
,   NrIntMsgInfo ("BAD_PROPERTIES"    )
,   NrIntMsgInfo ("BAD_COMMAND"       )
,   NrIntMsgInfo ("TUNING"            )
,   NrIntMsgInfo ("NO_SIGNAL"         )

,   NrIntMsgInfo ("SIGNAL"            )
,   NrIntMsgInfo ("MODE_RECEIVER"     )
,   NrIntMsgInfo ("MODE_TRANSMITTER"  )
,   NrIntMsgInfo ("LIST_RECEIVER"     )
,   NrIntMsgInfo ("LIST_TRANSMITTER"  )

,   NrIntMsgInfo ("MUTE_ON"           )
,   NrIntMsgInfo ("MUTE_OFF"          )
,   NrIntMsgInfo ("QUIT"              )
,   NrIntMsgInfo ("FREQUENCY_IN_USE"  )
,   NrIntMsgInfo ("AUDIO_DATA"        )

,   NrIntMsgInfo ("TO_ALL"            )
,   NrIntMsgInfo ("TO_RECEIVER"       )
,   NrIntMsgInfo ("TO_TRANSMITTER"    )
,   NrIntMsgInfo ("TO_TUNED"          )
,   NrIntMsgInfo ("TUNE_UP"           )

,   NrIntMsgInfo ("TUNE_DOWN"         )
,   NrIntMsgInfo ("BAD_FREQUENCY"     )
,   NrIntMsgInfo ("PING"              )
,   NrIntMsgInfo ("NUM_AUDIO_DATA"    )
,   NrIntMsgInfo ("CLIENT_INFO"       )

,   NrIntMsgInfo ("SERVER_INFO"       )
,   NrIntMsgInfo ("LIST_CONNECTION"   )
,   NrIntMsgInfo ("CONNECTION_INFO"   )
,   NrIntMsgInfo ("STAT_CONNECTION"   )
,   NrIntMsgInfo ("STAT_RECEIVER"     )

,   NrIntMsgInfo ("STAT_TRANSMITTER"  )
,   NrIntMsgInfo ("CONNECTION_STATUS" )
,   NrIntMsgInfo ("RECEIVER_STATUS"   )
,   NrIntMsgInfo ("TRANSMITTER_STATUS")
};

static EzString GetMsgText (int MsgCode)
{
    return MsgInfo [MsgCode].Text;
};

EzString GetWord (EzString &Line)
{
    EzString RetVal;
    int Pos;

    Pos = Instr (Line, " ");
    if (Pos == -1) Pos = Line.Length ();

    RetVal = Substr (Line, 0, Pos);
    Line   = Substr (Line, Pos + 1);

    return RetVal;
};

static NrMsgCode GetToken (EzString &Line)
{
    NrIntMsgInfo *pMsgInfo;
    EzString  Word, LineCopy = Line;
    int RetVal;

    Word = Upper (GetWord (Line));

    for (RetVal = MsgReady; RetVal < MsgCount; RetVal++) {
        pMsgInfo = MsgInfo + RetVal;
        if (pMsgInfo->Text == Word) return (NrMsgCode) RetVal;
        pMsgInfo++;
    };
    Line = LineCopy;
    return MsgNoMessage;
};

EzString ShortToString (short i)
{
    i = SizeHostToNet (i);

    return EzString ((char *)&i, sizeof (i));
};

int StringToShort (EzString s, short &i)
{
    const char *cp;

    if (s.Length () < sizeof (i)) return 0;
    cp = s;
    memcpy (&i, cp, sizeof (i));

    i = SizeNetToHost (i);

    return 1;
};

class MsgTimer : public XxTimer {
    virtual void HandleTimeOut (int Count);
};

void MsgTimer::HandleTimeOut (int Count)
{
    LogMessage ("");
};

static MsgTimer *pMsgTimer = NULL;

void LogMessage (EzString Message)
{
    time_t          Time;
    static time_t   LstTime;
    static EzString LstMessage;
    static int      MsgCount = 0;
    char            *penv;

    if (DoLogFlag == -1) {
        penv = getenv ("NETSTREAMER_LOG");
        if (penv == NULL) {
            DoLogFlag = 0;
        } else {
            DoLogFlag = atoi (penv);
        };
        pMsgTimer = new MsgTimer;
    };

    if (!DoLogFlag) return;

    if (Message == LstMessage) {
        if (Message != EzString ("")) MsgCount++;
        return;
    };

    time (&Time);
    if (MsgCount > 1) {
        cout << "[" << Remove (ctime (&LstTime), '\n') << "]Repeat " << EzString (MsgCount) << endl;
        LstTime = Time;
    };

    if (MsgCount > 0) {
        cout << "[" << Remove (ctime (&LstTime), '\n') << "]" << LstMessage << endl;
    };

    time (&LstTime);
    LstMessage = Message;
    if (Message == EzString ("")) {
        MsgCount = 0;
    } else {
        MsgCount = 1;
        pMsgTimer->SetTimeOut (10000);
    };
};

static void DebugMessage (EzString Prefix, NrMsgCode MsgCode, EzString Data)
{
    EzString Message;

    switch (MsgCode) {
    case MsgAudioData:
    case MsgNumAudioData:
    case MsgNoSignal:
    case MsgPing:
    case MsgToTuned:
        return;
    };

    if (MsgCode == MsgNoMessage) {
        Message = EzString ("-");
    } else {
        Message =   EzString ("[") + MsgInfo[MsgCode].Text
                  + EzString ("][") + Data + EzString ("]");
    }

    LogMessage (Prefix + Message);
};

NrMessageDecoder::NrMessageDecoder (EzString LogPrefix)
{
    SetMode (MessageAscii);
    NrMessageDecoder::LogPrefix = LogPrefix;
};

void NrMessageDecoder::PutData (EzString Data)
{
    if (MessageMode == MessageAscii) {
        Buffer += Remove (Data, "\r");
    } else {
        Buffer += Data;
    };
};

int NrMessageDecoder::GetMessage (NrMsgCode &MsgCode, EzString &Data)
{
    int        i, Len;
    short      Size, Code;

    if (MessageMode == MessageAscii) {
        i = Instr (Buffer, '\n');
        if (i == -1) return 0;
        Data    = Substr (Buffer, 0, i);
        Buffer  = Substr (Buffer, i + 1);
        MsgCode = GetToken (Data);
    } else {
        Len = Buffer.Length ();

        if (!StringToShort (Buffer, Size)) return 0;

        if (Len < int (sizeof (Size) + Size)) return 0;

        Data   = Substr (Buffer, sizeof (Size), Size);
        Buffer = Substr (Buffer, sizeof (Size) + Size);

        if (!StringToShort (Data, Code))  return 0;

        MsgCode = (NrMsgCode) Code;
        Data = Substr (Data, sizeof (Code));
    };

    DebugMessage (LogPrefix + EzString (" Get "), MsgCode, Data);

    if (MsgCode == MsgBinary) {
        SetMode (MessageBinary);
    };

    return 1;
};

static EzString CreateBinaryPacket (EzString Message)
{
    short Size;

    Size = Message.Length ();
    return ShortToString (Size) + Message;
};

NrMessageEncoder::NrMessageEncoder (EzString LogPrefix)
{
    SetMode (MessageAscii);
    NrMessageEncoder::LogPrefix = LogPrefix;
};

EzString NrMessageEncoder::PutMessage (NrMsgCode MsgCode, EzString Data)
{
    EzString      RetVal;
    unsigned char Encoding;
    unsigned char Seq;

    if (MsgCode == MsgNoMessage) return "";

    DebugMessage (LogPrefix + EzString (" Put "), MsgCode, Data);

    if (MessageMode == MessageAscii) {
        switch (MsgCode) {
        case MsgAudioData:
            Data     = EzString (int (Data.Length ()));
            break;
        case MsgNumAudioData:
            Encoding = Data[0];
            Seq      = Data[1];
            Data     =   EzString (Encoding) + EzString (" ")
                       + EzString (Seq) + EzString (" ")
                       + EzString (int (Data.Length () - 2));
            break;
        };

        RetVal =   GetMsgText (MsgCode)
			     + EzString (" ") + Data + EzString ("\n");
    } else {
        Data = ShortToString (MsgCode) + Data;

        RetVal = ShortToString (Data.Length ()) + Data;
    };

    if (MsgCode == MsgBinary) {
        SetMode (MessageBinary);
    }

    return RetVal;
};
