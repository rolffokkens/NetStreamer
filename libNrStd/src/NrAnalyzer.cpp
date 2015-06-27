/*
 * file: NrAnalyzer.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "NrAnalyzer.h"

#define MsHalveSecond 500

class NrAnalyzer::NrSubBand {
private:
    NrTimeLevelBuf LevelBuf;

    NrSubBand *pNextBand;
    int       ToDoSize;
    int       *ToDoBuf;
    int       *ToDoLast;
    int       *pCurToDo;
    int       Hist0, Hist1;
    int       Total, ToAdd;
    int       MaxLevel;
public:
    NrSubBand (int ToDoSize, NrSubBand *pNextBand);

    virtual ~NrSubBand (void);

    int ProcessData (int *pInBuf, int MsCurTime);

    int GetMaxLevel (int MsCurTime);
};

NrAnalyzer::NrSubBand::NrSubBand (int ToDoSize, NrSubBand *pNextBand)
    : LevelBuf (1024)
{
    NrSubBand::pNextBand    = pNextBand;
    NrSubBand::ToDoSize     = ToDoSize;
    NrSubBand::ToDoBuf      = new int [ToDoSize];
    NrSubBand::pCurToDo     = NrSubBand::ToDoBuf;
    NrSubBand::ToDoLast     = NrSubBand::ToDoBuf + ToDoSize;
    NrSubBand::Hist0        = 0;
    NrSubBand::Hist1        = 0;
    NrSubBand::Total        = 0;
    NrSubBand::ToAdd        = 0;
    NrSubBand::MaxLevel     = 0;
};

NrAnalyzer::NrSubBand::~NrSubBand (void)
{
// Seems OK! cout << "TODO: Unfinished destructor!!" << endl;
    delete [] ToDoBuf;
};

int NrAnalyzer::NrSubBand::ProcessData (int *pInBuf, int MsCurTime)
{
    int *pSample, *pLast, Sample, hSample, hHist;
    int PassNext, Diff, NextSum, NextVal, RetVal;

    if (pNextBand == NULL) return 1;

    pLast    = pInBuf + ToDoSize;
    pSample  = pInBuf;
    NextSum  = 0;
    PassNext = 0;

    while (pSample < pLast) {
        Sample   = *pSample++;
        hSample  = Sample >> 1;
        hHist    = Hist0  >> 1;

        Total   += hSample - hHist + ToAdd;
        ToAdd    = (Sample - hSample) - (Hist0 - hHist);

        NextVal  = (Total >> 1);
        Diff     = NextVal - Hist1;
        if (Diff < 0) Diff = -Diff;
        if (Diff > MaxLevel) MaxLevel = Diff;
        NextSum += NextVal;

        Hist0    = Hist1;
        Hist1    = Sample;
        if (PassNext) {
            *pCurToDo++ = (NextSum >> 1);
            NextSum     = 0;
        };
        PassNext = !PassNext;
    };

    if (pCurToDo == ToDoLast) {
        pCurToDo = ToDoBuf;
        RetVal = pNextBand->ProcessData (ToDoBuf, MsCurTime);
        if (RetVal) {
            LevelBuf.RegisterValue (MsCurTime, MaxLevel);
            MaxLevel = 0;
        };
    } else {
        RetVal = 0;
    };
    return RetVal;
};

static int ClipTimeVal (int Value, int MsOrigTime, int MsCurTime)
{
    MsOrigTime -= MsCurTime;
    if (MsOrigTime < 0) MsOrigTime = -MsOrigTime;
    if (MsOrigTime > MsHalveSecond) Value = 0;

    return Value;
};

int NrAnalyzer::NrSubBand::GetMaxLevel (int MsCurTime)
{
    int RetVal;
    int MsOrigTime;

    MsOrigTime = MsCurTime;

    RetVal     = LevelBuf.LookupValue (MsCurTime);

    return ClipTimeVal (RetVal, MsOrigTime, MsCurTime);
};

NrAnalyzer::NrAnalyzer (int BufSize, int NrBands)
    : LevelBuf (1024)
{
    int i;
    NrSubBand *pLast;

    NrAnalyzer::BufSize      = BufSize;
    NrAnalyzer::Buffer       = new int [BufSize];
    NrAnalyzer::BufLast      = Buffer + BufSize;
    NrAnalyzer::pCurBuf      = Buffer;
    NrAnalyzer::CurTimestamp = 0;
    NrAnalyzer::MaxLevel     = 0;
    NrAnalyzer::FilterVal    = 0;
    NrAnalyzer::OsSoundDelay = 0;

    NrAnalyzer::NrBands   = NrBands;
    NrAnalyzer::pSubBands = new (NrSubBand *) [NrBands];

    pLast = NULL;
    for (i = 0; i < NrBands; i++) {
        pSubBands[i] = new NrSubBand (BufSize, pLast);
        pLast = pSubBands [i];
    };
};

NrAnalyzer::~NrAnalyzer (void)
{
    int i;

    delete [] Buffer;

    for (i = 0; i < NrBands; i++) delete pSubBands[i];

    delete [] pSubBands;
};

void NrAnalyzer::ProcessData (EzString Data, int MsCurTime, int SampleRate)
{
    short *pData, *pFirst;
    int   DataLen, dT, Sample;
    int   FilterVal;

    MsCurTime += OsSoundDelay;

    pFirst    = (short *)(Data.Text ());
    pData     = pFirst;
    DataLen   = (Data.Length () >> 1);
    FilterVal = NrAnalyzer::FilterVal;

    if (CurTimestamp == 0) {
        CurTimestamp = MsCurTime;
    };

    while (DataLen--) {
        if (pCurBuf == BufLast) {
            if (SampleRate < 1000) {
                dT = 0;
            } else {
                dT = (((pData - pFirst) << 10) / SampleRate);
            };
            if (pSubBands[NrBands - 1]->ProcessData
                    (Buffer, CurTimestamp + dT)) {
                LevelBuf.RegisterValue (CurTimestamp + dT, MaxLevel);
                CurTimestamp = MsCurTime;
                MaxLevel     = 0;
            };
            pCurBuf = Buffer;
        };
        Sample     = *pData++;

        FilterVal  = ((FilterVal << 10) - FilterVal + (Sample << 6)) >> 10;
        Sample    -= (FilterVal >> 6);

        *pCurBuf++ = Sample;

        if (Sample < 0) Sample = -Sample;
        if (Sample > MaxLevel) MaxLevel = Sample;
    };

    NrAnalyzer::FilterVal = FilterVal;
};

int NrAnalyzer::GetMaxBandLevel (int Band, int MsCurTime)
{
    if (Band < 0 || Band >= NrBands) return 0;

    return pSubBands[Band]->GetMaxLevel (MsCurTime);
};

int NrAnalyzer::GetMaxLevel (int MsCurTime)
{
    int RetVal;
    int MsOrigTime;

    MsOrigTime = MsCurTime;

    RetVal = LevelBuf.LookupValue (MsCurTime);

    return ClipTimeVal (RetVal, MsOrigTime, MsCurTime);
};

void NrAnalyzer::SetOsSoundDelay (int OsSoundDelay)
{
    NrAnalyzer::OsSoundDelay = OsSoundDelay;
};


