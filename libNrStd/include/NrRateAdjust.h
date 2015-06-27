/*
 * file: NrRateAdjust.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1998 Rolf Fokkens
 *
 */

#ifndef H_NR_RATEADJUST
#define H_NR_RATEADJUST

#include "EzString.h"

class NrRateAdjust {
private:
    static const int BufferSize = 1024;

    int CounterVal;
    int DivisorVal;
    int RmStereo;

    int Sample, Sum;
    int Counter, Divisor;

    // short OutBuffer [BufferSize];
public:
    NrRateAdjust
        (char InSampleRate, int OutSampleRate);
    virtual ~NrRateAdjust (void);

    EzString ProcessData  (EzString Data);
};


#endif
