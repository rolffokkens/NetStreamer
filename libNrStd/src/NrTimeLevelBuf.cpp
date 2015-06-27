/*
 * file: NrTimeLevelBuf.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <iostream.h>

#include "NrTimeLevelBuf.h"

struct NrTimeRec {
    int MsTimeStamp;
    int Value;
    NrTimeRec (void) {
        MsTimeStamp = 0;
        Value       = 0;
    };
};

NrTimeLevelBuf::NrTimeLevelBuf  (int HistSize)
{
    NrTimeLevelBuf::pHist     = new NrTimeRec [HistSize];
    NrTimeLevelBuf::pCurHist  = NrTimeLevelBuf::pHist;
    NrTimeLevelBuf::pPrvHist  = NrTimeLevelBuf::pHist;
    NrTimeLevelBuf::pLastHist = NrTimeLevelBuf::pHist + HistSize;
    NrTimeLevelBuf::HistSize  = HistSize;
};

NrTimeLevelBuf::~NrTimeLevelBuf (void)
{
    delete [] pHist;
};

void NrTimeLevelBuf::RegisterValue (int MsTimeStamp, int Value)
{
    pCurHist->MsTimeStamp = MsTimeStamp;
    pCurHist->Value       = Value;
    if (++pCurHist == pLastHist) pCurHist = pHist;
};

int NrTimeLevelBuf::LookupValue (int &MsTimeStamp)
{
    NrTimeRec *pTemp;
    int       MinValue;
    int       MinMsTime;

    pTemp     = pPrvHist;
    MinValue  = -1;
    MinMsTime = 0;

    for (;;) {
        if (MinValue < pTemp->Value) {
            MinValue  = pTemp->Value;
        };
        MinMsTime = pTemp->MsTimeStamp;

        if ((pTemp->MsTimeStamp - MsTimeStamp) > 0) {
            pPrvHist = pTemp;
            break;
        };
        if (++pTemp == pLastHist) pTemp = pHist;
        if (pTemp == pPrvHist) break;
    };

    MsTimeStamp = MinMsTime;

    return MinValue;
};
