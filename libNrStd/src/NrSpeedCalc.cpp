/*
 * file: NrSpeedCalc.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <iostream>
#include "NrSpeedCalc.h"

struct NrSpeedCalc::NrSpeedSample {
    long Amount;
    long MsTime;
};

NrSpeedCalc::NrSpeedCalc (int NrSamples) 
{
    LastSample  = NrSamples;
    Samples     = new NrSpeedSample [LastSample];
    Reset (0);
};

void NrSpeedCalc::Reset (int Delay)
{
    NrSamples   = -Delay;
    CurSample   = 0;
    AmountTotal = 0;
    MsTimeTotal = 0;
};

void NrSpeedCalc::AddData (long Amount, long MsCurTime)
{
    if (NrSamples < 0) {
        NrSamples++;
        return;
    };

    if (NrSamples == 0) {
        Amount = 0;
    } else {
        if (NrSamples < LastSample) {
            MsTimeTotal  = Samples[0].MsTime;
        } else {
            MsTimeTotal  = Samples[CurSample].MsTime;
            AmountTotal -= Samples[CurSample].Amount;
        };
        MsTimeTotal  = MsCurTime - MsTimeTotal;
    };
    AmountTotal += Amount;

    Samples [CurSample].MsTime = MsCurTime;
    Samples [CurSample].Amount = Amount;

    CurSample++;

    if (CurSample == LastSample) CurSample = 0;

    if (NrSamples < LastSample) NrSamples++;
};

long NrSpeedCalc::GetAvgSpeed (int ShLeft)
{
    long RetVal;

    if (MsTimeTotal <= 0) {
        return 0;
    };

    return (AmountTotal << ShLeft) / MsTimeTotal;
};
