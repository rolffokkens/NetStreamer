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

#include "NrTransStdIn.h"

static int GetTimerDelay (int SampleRate)
{
    return ((1024 * 1000) / RateHerz [SampleRate]);
};

NrTransStdIn::NrTransStdIn
    (NrTransConnection *pConnection, char SampleRate, int InSampleRate)
    : NrTransSoundSource (pConnection)
    , Timer (this, GetTimerDelay (SampleRate))
    , Comp (3)
    , Adjust (InSampleRate, RateHerz [SampleRate] / 1000)
{
    NrTransStdIn::AvailToRead = 0;
    NrTransStdIn::MuteFlag    = 1;
    NrTransStdIn::SampleRate  = SampleRate;
};

NrTransStdIn::~NrTransStdIn (void)
{
};

EzString NrTransStdIn::ProcessReadData (EzString Data)
{
    EzString     Temp, RetVal;
    const short *pIn, *pInStart, *pInLast;
    short       *pOut;
    int          InLen;

    Temp = XxStdIn::ProcessReadData (Data);

    BlockBuffer += Adjust.ProcessData (Temp);

    if (BlockBuffer.Length () > 2048) {
        Temp        = Substr (BlockBuffer, 0, 2048);
        BlockBuffer = Substr (BlockBuffer, 2048);
    } else {
        Temp = "";
    };

    AvailToRead -= Temp.Length ();

    if (MuteFlag) {
        RetVal = "";
    } else {
        RetVal = Comp.Compress (Temp);
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
    )
    : NrTransConnection
         ( AddrPort, Freq, Description
         , AddInfo
         , new NrTransStdIn (this, SampleRate, InSampleRate)
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
