/*
 * file: NrTransStdIn.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1998 Rolf Fokkens
 *
 */

#include <dirent.h>
#include <fcntl.h>
#include <iostream>

#include "NrTransStdIn.h"

using namespace std;

static int GetTimerDelay (int SampleRate)
{
    return ((1024 * 1000) / RateHerz [SampleRate]);
};

NrTransStdIn::NrTransStdIn
    (NrTransConnection *pConnection, char SampleRate, int InSampleRate, int DataID, int MetaDataID)
    : NrTransSoundSource (pConnection)
    , Timer (this, GetTimerDelay (SampleRate))
    , Comp (3)
    , Adjust (InSampleRate, RateHerz [SampleRate] / 1000)
{
    NrTransStdIn::AvailToRead = 0;
    NrTransStdIn::MuteFlag    = 1;
    NrTransStdIn::SampleRate  = SampleRate;
    NrTransStdIn::DataID      = DataID;
    NrTransStdIn::MetaDataID  = MetaDataID;
cerr << "wah:" << DataID << endl;
};

NrTransStdIn::~NrTransStdIn (void)
{
};

EzString NrTransStdIn::ExtractAudio (EzString Data)
{
    EzString Temp, Block;
    int len, id;
    if (DataID == -1) return Data;

    DataBuffer += Data;
    for (;;) {
        Block = "";
        if (DataBuffer.Length () < (unsigned int)2) break;
        len = (unsigned char)DataBuffer[1];
        if (!len) len = 256;
        if (DataBuffer.Length () < (unsigned int)(len + 2)) break;
        id = (unsigned char)DataBuffer[0];
        Block = Substr (DataBuffer, 2, len);
        DataBuffer = Substr (DataBuffer, 2 + len);
        if (id == DataID) Temp += Block;
	if (id == MetaDataID) HandleInfo (Block);
    }
    Data = Temp;

    return Data;
}

EzString NrTransStdIn::ProcessReadData (EzString Data)
{
    EzString     RetVal;
    const short *pIn, *pInStart, *pInLast;
    short       *pOut;
    int          InLen;

    Data = XxStdIn::ProcessReadData (Data);

    Data = ExtractAudio (Data);

    BlockBuffer += Adjust.ProcessData (Data);

    if (BlockBuffer.Length () > 2048) {
        Data        = Substr (BlockBuffer, 0, 2048);
        BlockBuffer = Substr (BlockBuffer, 2048);
    } else {
        Data = "";
    };

    AvailToRead -= Data.Length ();

    if (MuteFlag) {
        RetVal = "";
    } else {
        RetVal = Comp.Compress (Data);
    };

    return RetVal;
};

int NrTransStdIn::GetReadChunkSize  (void)
{
    return (BlockBuffer.Length () > 2048 ? 4 : 2048);
};          

void NrTransStdIn::OnDisconnect (void)
{
    Close ();
    HandleError ();
};

template <>
void NrTransStdIn::TransStdInTimer::HandleTimeOut (int Count)
{
    pOwner->HandleTimeOut (Count);
};

void NrTransStdIn::HandleTimeOut (int Count)
{
    AvailToRead += 2048 * Count;

    if (AvailToRead > 4096) {
        LogMessage ("Late Input");
    };
};

void NrTransStdIn::GetRWFlags (int &rFlag, int &wFlag)
{
    rFlag = (AvailToRead > 0);
    wFlag = 0;
};

NrTransConnStdIn::NrTransConnStdIn
    ( EzString AddrPort
    , int Freq, EzString Description
    , EzString AddInfo
    , char SampleRate, int InSampleRate
    , int DataID, int MetaDataID
    )
    : NrTransConnection
         ( AddrPort, Freq, Description
         , AddInfo
         , new NrTransStdIn (this, SampleRate, InSampleRate, DataID, MetaDataID)
         , SampleRate
         )
{
};

NrTransConnStdIn::~NrTransConnStdIn (void)
{
};

void NrTransStdIn::SetMute (int Flag)
{
    MuteFlag = Flag;
};

void NrTransStdIn::Start (void)
{
};
