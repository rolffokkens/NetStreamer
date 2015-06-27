/*
 * file: NrFifo.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <limits.h>

#include "NrFifo.h"

inline void AdjustAvg (int &Avg, int New, int Weight)
{
    Avg = ((Avg << Weight) - Avg + New ) >> Weight;
};

class NrFifoInternal {
private:
    short *pBuf, *pBufLast, *pBufPut, *pBufGet;
    int    BufSize, BufUsed;

    int    LastAvg, LastSample, LastPutSample;
public:
    NrFifoInternal (int BufSize);

    virtual ~NrFifoInternal (void);

    EzString GetSound
        ( int MaxSize
        , int Counter, int Divisor
        , int &LastCount
        );

    int PutSound (EzString Sound, int Expansion = 0);

    void Reset (void);

    int  GetBufSize (void);
    int  GetBufUsed (void);
};

NrFifoInternal::NrFifoInternal (int BufSize)
{
    NrFifoInternal::BufSize    = BufSize;
    NrFifoInternal::BufUsed    = 0;
    NrFifoInternal::LastSample = 0;
    NrFifoInternal::LastAvg    = 0;
    NrFifoInternal::pBuf       = new short [BufSize];
    NrFifoInternal::pBufLast   = pBuf + BufSize;
    NrFifoInternal::pBufPut    = pBuf;
    NrFifoInternal::pBufGet    = pBuf;

    Reset ();
};

NrFifoInternal::~NrFifoInternal (void)
{
    delete [] pBuf;
};

void NrFifoInternal::Reset (void)
{
    BufUsed    = 0;
    pBufPut    = pBuf;
    pBufGet    = pBuf;
};

static void DoExpansion
    ( const short *&pIn
    , int   &InSize
    , int   Expansion
    , short *&pClear
    , int   &LastSample
    , int   &LastAvg
    )
{
    const short *pLast, *pSample;
    short       *pOut;
    int         tSample = LastSample, tAvg = LastAvg, tSample2;

    LastSample = pIn [InSize - 1];

    if (Expansion == 0) {
        pClear     = NULL;
        return;
    };
    pSample = pIn;
    pLast   = pIn + InSize;

    if (Expansion < 0) {
        pLast--;
        InSize >>= 1;
        pClear   = new short [InSize];
        pOut     = pClear;
        while (pSample < pLast) {
            *pOut++ = (int (*pSample++) + int (*pSample++)) >> 1;
        };
    } else {
        InSize <<= 1;
        pClear   = new short [InSize];
        pOut     = pClear;
        while (pSample < pLast) {
            tSample2 = *pSample++;
            tSample  = (tSample + tSample2) >> 1;
            tAvg     = (tAvg + tSample) >> 1;
            *pOut++  = tAvg;
            tSample  = tSample2;
            tAvg     = (tAvg + tSample) >> 1;
            *pOut++  = tAvg;
        };
        LastAvg = tAvg;
    };
    pIn = pClear;
};

int NrFifoInternal::PutSound (EzString Sound, int Expansion)
{
    const short *pIn, *pInLast;
    short       *pClear;
    int          RetVal, InSize;

    pIn     = (const short *) Sound.Text ();
    RetVal  = Sound.Length () >> 1;

    DoExpansion (pIn, RetVal, Expansion, pClear, LastPutSample, LastAvg);

    pInLast = pIn + RetVal;

    while (pIn < pInLast && BufUsed < BufSize) {
        InSize  = (BufSize - BufUsed);
        if (pIn + InSize > pInLast) {
            InSize = pInLast - pIn;
        };
        if (pBufPut + InSize > pBufLast) {
            InSize = pBufLast - pBufPut;
        };
        memcpy (pBufPut, pIn, (InSize << 1));
        pIn     += InSize;
        pBufPut += InSize;
        BufUsed += InSize;
        if (pBufPut == pBufLast) pBufPut = pBuf;
    };

    if (pClear != NULL) delete pClear;

    return RetVal;
};

#define OUT_SIZE 2048

EzString NrFifoInternal::GetSound
    ( int MaxSize
    , int Counter, int Divisor
    , int &LastCount
    )
{
    int       InSize, OutSize, Count, AbsDiv, TmpGetSpeed, FillSize;
    EzString  RetVal;
    short    *pIn, *pInLast, OutBuf[OUT_SIZE], *pOut, *pOutLast;
    short     Sample;
    int       LastSample = NrFifoInternal::LastSample;

    MaxSize >>= 1;

    if (Counter < 0) {
        Counter = -Counter;
        Divisor = -Divisor;
    };

    AbsDiv   = (Divisor < 0 ? -Divisor : Divisor);
    Count    = LastCount;

    while (MaxSize > 0 && BufUsed > 0) {
        pIn        = pBufGet;
        pInLast    = pBufGet + BufUsed;
        if (pInLast > pBufLast) pInLast = pBufLast;

        pOut       = OutBuf;
        pOutLast   = OutBuf + MaxSize;
        if (pOutLast > OutBuf + OUT_SIZE) pOutLast = OutBuf + OUT_SIZE;

        while (pIn < pInLast && pOut < pOutLast) {
            Sample = *pIn++;

            if (Count > AbsDiv) {		// Time to ajust?
                if (Divisor < 0) {		// If Expand..
                    if (pOut < pOutLast - 1) {	// only if place to do it now!
                        LastSample  += Sample;	// Interpolate
                        LastSample >>= 1;	// ...the...
                        *pOut++ = LastSample;	// Extra Sample
                        // *pOut++ = Sample;	// Extra Sample
                        Count  -= AbsDiv;	//
                    };				//
                    *pOut++ = Sample;		// The normal Sample copy
                } else {			// Or Compress...
                    Count  -= AbsDiv;		//
                };
            } else {				// If not the time to ajust..
                *pOut++ = Sample;		// just copy the sample
            };
            LastSample = Sample;
            Count     += Counter;
        };

        BufUsed -= (pIn - pBufGet); 
        pBufGet  = (pIn < pBufLast ? pIn : pBuf);

        RetVal  += EzString ((char *)OutBuf, (pOut - OutBuf) << 1);
        MaxSize -= (pOut - OutBuf);
    };

    LastCount = Count;

    FillSize = MaxSize << 1;
    if (FillSize != 0) {
        memset (OutBuf, 0, FillSize);
        RetVal += EzString ((char *)OutBuf, FillSize);
    };

    NrFifoInternal::LastSample = LastSample;

    return RetVal;
};

int NrFifoInternal::GetBufSize (void)
{
    return BufSize;
};

int NrFifoInternal::GetBufUsed (void)
{
    return BufUsed;
};

/*****************************************************************************/

void NrFifo::Reset (void)
{
    pInternal->Reset ();
};

NrFifo::NrFifo (int BufSize)
{
    pInternal = new NrFifoInternal (BufSize);
};

NrFifo::~NrFifo (void)
{
    delete pInternal;
};

int NrFifo::PutSound (EzString Sound, int Expansion)
{
    return pInternal->PutSound (Sound, Expansion);
};

EzString NrFifo::GetSound
    ( int MaxSize
    , int Counter, int Divisor
    , int &LastCount
    )
{
    EzString RetVal;

    RetVal = pInternal->GetSound
                 (MaxSize, Counter, Divisor, LastCount);

    if (int (RetVal.Length ()) != MaxSize) {
        cout << "GetSound (" << MaxSize << ")->" << RetVal.Length () << endl;
    };

    return RetVal;
};

unsigned int NrFifo::GetBufSize (void)
{
    return pInternal->GetBufSize ();
};

unsigned int NrFifo::GetBufUsed (void)
{
    return pInternal->GetBufUsed ();
};


