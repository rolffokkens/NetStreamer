/*
 * file: NrSpeedBuf.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

static const int Factor1 = 100;
static const int Shift1  = 8;

#include <limits.h>

#include "EzString.h"
#include "NrSpeedBuf.h"

inline void AdjustAvg (int &Avg, int New, int Weight)
{
    Avg = ((Avg << Weight) - Avg + New ) >> Weight;
};

int NrSpeedBuf::GetPutSpeed (int shift)
{
    return PutCalc1.GetAvgSpeed (shift);
};

int NrSpeedBuf::GetGetSpeed (int shift)
{
    return GetCalc.GetAvgSpeed (shift);
};

void NrSpeedBuf::IntTurnOff (void)
{
    if (!MuteFlag && OnFlag) return;

    if (State == Off) return;

    if (State == Preparing) HandlePreparing (0);

    State = Off;

    NrFifo::Reset ();
};

void NrSpeedBuf::IntTurnOn (void)
{
    if (MuteFlag || !OnFlag) return;

    PutCalc1.Reset (5);
    PutCalc2.Reset (5);
    GetCalc.Reset (5);

    AvgPutSpeed = (32 * Factor1) << Shift1;
    AvgGetSpeed = (32 * Factor1) << Shift1;

    AvgCounter  = 0;
    AvgDivisor  = 1;

    if (State != Preparing) HandlePreparing (1);
    State = Preparing;
};

void NrSpeedBuf::TurnOff (void)
{
    if (!OnFlag) return;

    OnFlag = 0;

    IntTurnOff ();
};

void NrSpeedBuf::TurnOn (void)
{
    if (OnFlag) return;

    OnFlag = 1;

    IntTurnOn ();
};

void NrSpeedBuf::SetMute (int Flag)
{
    if (MuteFlag == Flag) return;

    MuteFlag = Flag;

    if (MuteFlag) {
        IntTurnOff ();
    } else {
        IntTurnOn ();
    }
};

void NrSpeedBuf::HandlePreparing (int Flag)
{
};

NrSpeedBuf::NrSpeedBuf (int AimSize)
    : NrFifo (AimSize << 1)
    , Decomp (3)
    , GetCalc  (2048)
    , PutCalc1 (2048)
    , PutCalc2 (64)
{
    NrSpeedBuf::SpeedCount  = 0;
    NrSpeedBuf::ExpCount    = 0;

    NrSpeedBuf::MsPutLast   = 0;
    NrSpeedBuf::MsGetLast   = 0;
    NrSpeedBuf::PutCount    = 0;
    NrSpeedBuf::GetCount    = 0;

    NrSpeedBuf::AimSize     = AimSize;
    NrSpeedBuf::AvgAimVal   = 4000;

    NrSpeedBuf::AvgSpeedVar = 0;
    NrSpeedBuf::AvgPutSpeed = (32 * Factor1) << Shift1;
    NrSpeedBuf::AvgGetSpeed = (32 * Factor1) << Shift1;

    NrSpeedBuf::AvgCounter  = 0;
    NrSpeedBuf::AvgDivisor  = 1;

    NrSpeedBuf::MuteFlag    = 0;
    NrSpeedBuf::OnFlag      = 0;

    IntTurnOff ();
};

NrSpeedBuf::~NrSpeedBuf ()
{
};

void NrSpeedBuf::PutSound (EzString Sound, long MsCurTime, int Expansion)
{
    int TmpPutSpeed;
    int TmpSpeedVar;
    int SoundSize;

    if (State == Off) return;

    AdjustAvg (AvgBufSize, GetBufUsed (), 10);

    SoundSize = NrFifo::PutSound
                    (Decomp.Decompress (Sound, AUDIO_BLOCK), Expansion);

    HistBuf.PutData (Sound, SoundSize);

    PutCount += SoundSize * 2;

    if (MsCurTime != MsPutLast) {
        PutCalc1.AddData (PutCount, MsCurTime);
        PutCalc2.AddData (PutCount, MsCurTime);

        MsPutLast = MsCurTime;
        PutCount  = 0;
    } else {
        0; // cerr << "*"; cerr.flush ();
    };

    if (State == On) {
        TmpPutSpeed = (PutCalc1.GetAvgSpeed (Shift1) * Factor1);
        TmpSpeedVar = (PutCalc1.GetAvgSpeed (Shift1) * Factor1);
    } else {
        TmpPutSpeed = (32 << Shift1) * Factor1;
        TmpSpeedVar = TmpPutSpeed;
    };

    AdjustAvg (AvgPutSpeed, TmpPutSpeed, 2);

    TmpSpeedVar -= AvgPutSpeed;
    if (TmpSpeedVar < 0) TmpSpeedVar = -TmpSpeedVar;

    if (TmpSpeedVar > AvgSpeedVar) {
        AvgSpeedVar = TmpSpeedVar;
    } else {
        AdjustAvg (AvgSpeedVar, TmpSpeedVar, 3);
    };

    if (State == Preparing && GetBufUsed () > (GetBufSize () >> 1)) {
        HandlePreparing (0);
        State       = On;
        AvgPutSpeed = PutCalc1.GetAvgSpeed (Shift1) * Factor1;
        AvgGetSpeed = GetCalc.GetAvgSpeed  (Shift1) * Factor1;
    };
};

static void ReduceFract
    ( int &Counter1, int &Divisor1
    )
{
    int C, D, S;

    if (Divisor1 < 0) {
        C = -Counter1;
        D = -Divisor1;
    } else {
        C =  Counter1;
        D =  Divisor1;
    };
    S = C < 0;

    if (S != 0) C = -C;

    while (C > 0x08000 || D > 0x08000) {
        C >>= 1;
        D >>= 1;
    };
    if (S !=0) C = -C;

    Counter1 = C;
    Divisor1 = D;
};

static void MultFract
    ( int Counter1,  int Divisor1
    , int Counter2,  int Divisor2
    , int &Counter3, int &Divisor3
    )
{
    ReduceFract (Counter1, Divisor1);
    ReduceFract (Counter2, Divisor2);

    Counter3 = Counter1 * Counter2;
    Divisor3 = Divisor1 * Divisor2;
};

static void AddFract
    ( int Counter1,  int Divisor1
    , int Counter2,  int Divisor2
    , int &Counter3, int &Divisor3
    )
{
    ReduceFract (Counter1, Divisor1);
    ReduceFract (Counter2, Divisor2);

    Counter3 = Counter1 * Divisor2 + Counter2 * Divisor1;
    Divisor3 = Divisor1 * Divisor2;
};

static void MultRate
    ( int Counter1,  int Divisor1
    , int Counter2,  int Divisor2
    , int &Counter3, int &Divisor3
    )
{
    int Ca, Da, Cb, Db;

    AddFract
        ( Counter1, Divisor1
        , Counter2, Divisor2
        , Ca,       Da
        );

    MultFract
        ( Counter1, Divisor1
        , Counter2, Divisor2
        , Cb,       Db
        );

    AddFract
        ( Ca,       Da
        , Cb,       Db
        , Counter3, Divisor3
        );
};

EzString NrSpeedBuf::GetSound (unsigned int MaxSize, long MsCurTime)
{
    EzString RetVal, HistSound;
    int      Counter, Divisor, TmpGetSpeed, CorrVal, DiffCount;
    int      tC, tD, tSC, tSD;
    int      ExpSize;

    GetCalc.AddData (MaxSize, MsCurTime);

// FKS
    if (State == On) {
        TmpGetSpeed = (GetCalc.GetAvgSpeed (Shift1) * Factor1);
    } else {
        TmpGetSpeed = (32 << Shift1) * Factor1;
    };

    AdjustAvg (AvgGetSpeed, TmpGetSpeed, 2); // FKS

    CorrVal = GetBufUsed () - (GetBufSize () >> 1);
    Counter = AvgPutSpeed - AvgGetSpeed;
    Divisor = AvgPutSpeed;

    CorrVal >>= 4;

    MultRate
        ( CorrVal,  1000000
        , Counter,  Divisor
        , tC,       tD
        );

    if (AvgDivisor == 0) return 0;

    tSC = tC;
    tSD = tD;

    if (tSC < -(tSD >> 2)) tSC = -(tSD >> 2);
    if (tSC >  (tSD >> 2)) tSC =  (tSD >> 2);

    tSC <<= 2;

    ReduceFract (tSC, tSD);

    AdjustAvg (AvgCounter, tSC, 2);
    AdjustAvg (AvgDivisor, tSD, 2);

    DiffCount = GetBufUsed ();

    if (State == On) {
        RetVal = NrFifo::GetSound
            ( MaxSize
            , tC, tD
            , SpeedCount
            );
    } else {
        RetVal = Rpad (EzString (""), MaxSize, 0);
    };

    DiffCount -= GetBufUsed ();
    ExpCount  += DiffCount;

    while (ExpCount > 0 && HistBuf.GetDataSize () > 0) {
        HistSound = HistBuf.GetData (ExpSize);
        HandleRecordData (HistSound, MsCurTime);
        ExpCount  -= ExpSize;
    };

    return RetVal;
};

int NrSpeedBuf::GetBufRate (void)
{
    return (100 * GetBufUsed ()) / (GetBufSize () >> 1);
};

void NrSpeedBuf::HandleRecordData (EzString RecData, long MsCurTime)
{
};

int NrSpeedBuf::GetAdjustRate (void)
{
    int tC, tD;

    if (AvgDivisor == 0) return 0;

    tC = AvgCounter << 8;
    tD = AvgDivisor;

    return tC / tD;
};

