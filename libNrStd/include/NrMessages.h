/*
 * file: NrMessages.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_MESSAGES
#define H_MESSAGES

#include "EzString.h"

extern int RateHerz[];

void LogMessage (EzString Message);

/*
 * EncodingType = 0
 * reflects CCITT_ADPCM
 *
 * Currently the only encoding.
 */

enum EncodingTypes {
    EncCcittAdpcm = 0
};

// const unsigned char EncodingType = (EncCcittAdpcm << 4);

enum SampleRates {
    Rate8kHz  = 0
,   Rate16kHz = 1
,   RateCount
};

#define AUDIO_BITS       16
#define AUDIO_SAMPLERATE Rate16kHz
#define AUDIO_BLOCK      2048
#define AUDIO_OVERLAP    0

#define COMP_AUDIO_BLOCK ((AUDIO_BLOCK * 3) / 16)

enum NrMsgCode {
    MsgNoMessage = -1

,   MsgReady
,   MsgBinary
,   MsgNoMode
,   MsgTune
,   MsgReceiverInfo

,   MsgTransmitterInfo
,   MsgBadProperties
,   MsgBadCommand
,   MsgTuning
,   MsgNoSignal

,   MsgSignal
,   MsgModeReceiver
,   MsgModeTransmitter
,   MsgListReceiver
,   MsgListTransmitter

,   MsgMuteOn
,   MsgMuteOff
,   MsgQuit
,   MsgFrequencyInUse
,   MsgAudioData

,   MsgToAll
,   MsgToReceiver
,   MsgToTransmitter
,   MsgToTuned
,   MsgTuneUp

,   MsgTuneDown
,   MsgBadFrequency
,   MsgPing
,   MsgNumAudioData
,   MsgClientInfo

,   MsgServerInfo
,   MsgListConnection
,   MsgConnectionInfo
,   MsgStatConnection
,   MsgStatReceiver

,   MsgStatTransmitter
,   MsgConnectionStatus
,   MsgReceiverStatus
,   MsgTransmitterStatus

,   MsgCount
};

extern EzString ShortToString (short i);
extern int      StringToShort (EzString s, short &i);

enum NrMessageMode {MessageBinary, MessageAscii};

class NrMessageDecoder {
private:
    NrMessageMode MessageMode; 
    EzString      Buffer;
    EzString      LogPrefix;
public:
    NrMessageDecoder (EzString LogPrefix);
    void SetMode (NrMessageMode Mode) { MessageMode = Mode; };

    void PutData (EzString Data);
    int  GetMessage (NrMsgCode &MsgCode, EzString &Data);
};

class NrMessageEncoder {
private:
    NrMessageMode MessageMode; 
    EzString      LogPrefix;
public:
    NrMessageEncoder (EzString LogPrefix);
    void SetMode (NrMessageMode Mode) { MessageMode = Mode; };

    NrMessageMode GetMode (void) { return MessageMode; };
    EzString PutMessage (NrMsgCode MsgCode, EzString Data);
};

EzString  GetWord  (EzString &Line);

const EzString MsgStrDisplay = EzString ("DISPLAY");

// char GetSampleRate (void);

#endif
