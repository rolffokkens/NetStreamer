/*
 * file: NrRateAdjust.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1998 Rolf Fokkens
 *
 */

#include "Exception.h"
#include "NrRateAdjust.h"

using namespace std;

NrRateAdjust::NrRateAdjust (char InSampleRate, int OutSampleRate)
{
    NrRateAdjust::Sum         = 0;
    NrRateAdjust::Divisor     = 0;

    NrRateAdjust::CounterVal  = OutSampleRate;
    NrRateAdjust::DivisorVal  = InSampleRate;
    NrRateAdjust::RmStereo    = 1;
};

NrRateAdjust::~NrRateAdjust (void)
{
};

EzString NrRateAdjust::ProcessData (EzString Data)
{
    EzString    Temp, RetVal;
    const short *pIn,  *pInStart,  *pInLast;
    short       *pOut, *pOutStart, *pOutLast;
    int         InLen;

    pInStart = (const short *)(Data.Text ());
    InLen    = Data.Length () >> 1;
    if (RmStereo) {
        InLen &= ~1;
    };
    pInLast  = pInStart + InLen;

    pOutStart = new short [InLen];
    pOut      = pOutStart;
    pOutLast  = pOutStart + InLen;

    for (pIn = pInStart; pIn < pInLast;) {
        Sample = *pIn++;
        if (RmStereo) {
            Sample = (Sample + *pIn++) >> 1;
        };
        for (Counter = 0; Counter < CounterVal; Counter++) {
            Sum += Sample;
            Divisor++;
            if (Divisor == DivisorVal) {
                *pOut++ = (Sum / DivisorVal);
                if (pOut == pOutLast) {
                    throw Exception ("NrRateAdjust: Internal error: Outbuf too small");
                };
                Sum     = 0;
                Divisor = 0;
            };
        };
    };
    RetVal = EzString ((char *)pOutStart, (pOut - pOutStart) << 1);
    delete [] pOutStart;

    return RetVal;
};
