/*
 * file: NrAnalyzer.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_NR_ANALYZER
#define H_NR_ANALYZER

#include "EzString.h"
#include "NrTimeLevelBuf.h"

class NrAnalyzer {
private:
    class NrSubBand;

    int BufSize;
    int *Buffer;
    int *BufLast;
    int *pCurBuf;
    int CurTimestamp;
    int FilterVal;

    int OsSoundDelay;

    int       NrBands;
    NrSubBand **pSubBands;

    NrTimeLevelBuf LevelBuf;
    int            MaxLevel;
protected:
public:
    NrAnalyzer (int BufSize, int NrBands);

    ~NrAnalyzer (void);

    void ProcessData     (EzString Data, int MsCurTime, int SampleRate);

    int  GetMaxBandLevel (int Band, int MsCurTime);
    int  GetMaxLevel     (int MsCurTime);

    void SetOsSoundDelay (int OsSoundDelay);
};

#endif;
