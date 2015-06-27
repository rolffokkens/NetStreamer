/*
 * file: NrFifo.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_NR_FIFO

#define H_NR_FIFO

#include <fstream.h>

#include "EzString.h"
#include "NrSpeedCalc.h"

#define BUFHISTSIZE 32

class NrFifoInternal;

class NrFifo {
    friend NrFifoInternal;
private:
    NrFifoInternal *pInternal;
public:
    NrFifo (int BufSize);

    virtual ~NrFifo (void);

    EzString GetSound
        ( int MaxSize // , long MsCurTime
        , int Counter, int Divisor
        , int &LastCount
        );

    int  PutSound
        ( EzString Sound
        , int      Expansion
        );

    void Reset (void);

    unsigned int GetBufSize  (void);
    unsigned int GetBufUsed  (void);

    unsigned int GetPutSpeed (int shift);
    unsigned int GetGetSpeed (int shift);
};

#endif
