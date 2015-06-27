/*
 * file: NrRecPump.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <errno.h>
#include "NrRecPump.h"

void NrRecPump::NrPumpSpeedBuf::HandleRecordData
    (EzString RecData, long MsCurTime)
{
    pParent->HandleRecordData (RecData, MsCurTime);
};

void NrRecPump::NrPumpSpeedBuf::HandlePreparing  (int Flag)
{
    pParent->SetPrepare (Flag);
};

NrRecPump::NrPumpSpeedBuf::NrPumpSpeedBuf (NrRecPump *pParent, int AimSize)
    : NrSpeedBuf (AimSize)
{
    NrRecPump::NrPumpSpeedBuf::pParent = pParent;
};

NrRecPump::NrRecPump (int AimSize, EzString LogPrefix, char SampleRate)
    : SpeedBuf (this, AimSize)
    , Analyzer (4, 9)
    , Encoder (LogPrefix)
    , Decoder (LogPrefix)
{
    NrRecPump::OsSoundDelay  = 0;
    NrRecPump::SampleRate    = SampleRate;
    NrRecPump::Expansion     = 32767;
};

NrRecPump::~NrRecPump (void)
{
};

void NrRecPump::Reset (void)
{
    SpeedBuf.Reset ();
};

void NrRecPump::HandleRecordData (EzString RecData, long MsCurTime)
{
    int Dummy;

    HistBuf.PutData (RecData, 0, MsCurTime);

    for (;;) {
        RecData = HistBuf.GetData (Dummy, MsCurTime - OsSoundDelay);
        if (RecData.Length () == 0) break;

        WriteTape (RecData);
    };
};

int NrRecPump::GetMessage (NrMsgCode &MsgCode, EzString &Data, int MsCurTime)
{
    int           RetVal;
    unsigned char Encoding;
    unsigned char Seq;
    int           IsAudio, AudioOK, Expansion;

    for (;;) {
        RetVal = Decoder.GetMessage (MsgCode, Data);

        if (!RetVal) break;

        IsAudio   = 0;
        AudioOK   = 0;
        Expansion = 0;

        if (MsgCode == MsgNumAudioData) {
            Encoding = Data[0];
            Seq      = Data[1];
            Data     = Substr (Data, 2);

            AudioSeq  = (AudioSeq == -1 ? -1 : (AudioSeq + 1) & 0x0ff);
            Expansion = SampleRate - (Encoding & 0x0f);
            IsAudio   = 1;
            AudioOK   = ((Encoding >> 4) == EncCcittAdpcm);

            if (int (Seq) == AudioSeq) {
                SetMissData (0);
            } else {
                AudioSeq = Seq;
                SetMissData (1);
            };
        };
        if (MsgCode == MsgAudioData) {
            AudioSeq  = -1;
            Expansion = 0;
            IsAudio   = 1;
            AudioOK   = 1;
            SetMissData (1);
        };

        if (IsAudio) {
            if (AudioOK) {
                SpeedBuf.PutSound (Data, MsCurTime, Expansion);
                if (NrRecPump::Expansion != Expansion) {
                    NrRecPump::Expansion = Expansion;
                    SetExpansion (Expansion);
                };
            };
        } else {
            break;
        };
    };

    switch (MsgCode) {
    case MsgSignal:
        SpeedBuf.TurnOn ();
        break;
    case MsgNoSignal:
    case MsgTuning:
        SpeedBuf.TurnOff ();
        break;
    };

    return RetVal;
};

EzString NrRecPump::PutMessage (NrMsgCode MsgCode, EzString Data)
{
    EzString RetVal;

    RetVal = Encoder.PutMessage (MsgCode, Data);

    return RetVal;
};

EzString NrRecPump::GetPCM (int Amount, int MsCurTime)
{
    EzString RetVal;

    RetVal = SpeedBuf.GetSound (Amount, MsCurTime);

    Analyzer.ProcessData (RetVal, MsCurTime, SpeedBuf.GetGetSpeed (9));

    return RetVal;
};

void NrRecPump::SetOsSoundDelay (int OsSoundDelay)
{
    NrRecPump::OsSoundDelay = OsSoundDelay;

    Analyzer.SetOsSoundDelay (OsSoundDelay);
};

int NrRecPump::GetMaxLevel (int MsCurTime)
{
    return Analyzer.GetMaxLevel (MsCurTime);
};

int NrRecPump::GetMaxBandLevel (int Band, int MsCurTime)
{
    return Analyzer.GetMaxBandLevel (Band, MsCurTime);
};

int NrRecPump::GetAdjustRate (void)
{
    return SpeedBuf.GetAdjustRate ();
};

void NrRecPump::SetMissData (int Flag)
{
};

void NrRecPump::SetPrepare (int Flag)
{
};

void NrRecPump::WriteTape (EzString Data)
{
};

void NrRecPump::SetExpansion (int Expand)
{
};

void NrRecPump::SetMute (int Flag)
{
    SpeedBuf.SetMute (Flag);
};
