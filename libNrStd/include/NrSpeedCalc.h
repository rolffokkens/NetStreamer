/*
 * file: NrSpeedCalc.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_NR_SPEEDCALC

#define H_NR_SPEEDCALC

#define BUFHISTSIZE 32

class NrSpeedCalc {
private:
    struct NrSpeedSample;

    int           NrSamples;
    int           CurSample;
    int           LastSample;

    unsigned long AmountTotal;
    unsigned long MsTimeTotal;
    NrSpeedSample *Samples;
public:
    void Reset (int Delay);

    NrSpeedCalc (int NrBits);

    void AddData (long Amount, long MsCurTime);

    long GetAvgSpeed (int ShLeft);
};

#endif
