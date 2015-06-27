/*
 * file: NrSpeedBuf.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 * Rel  When   who  What
 * 0.11 980104 Rolf ..
 */

#ifndef H_NR_SPEEDBUF

#define H_NR_SPEEDBUF

#include <fstream>

#include "EzString.h"
#include "NrMessages.h"
#include "NrFifo.h"
#include "NrDecomp.h"
#include "NrHistSoundBuf.h"
#include "NrSpeedCalc.h"

#define BUFHISTSIZE 32

class NrSpeedBuf : public NrFifo {
private:
    enum BufState {Off, Preparing, On};

    int      MuteFlag;
    int      OnFlag;

    BufState State;

    NrHistSoundBuf HistBuf;
    NrDecomp       Decomp;

    long MsPutLast;
    long MsGetLast;

    long PutCount;
    long GetCount;

    int SpeedCount;

    int ExpCount;

    int AimSize;
    int AvgAimVal;
    int AvgBufSize;

    NrSpeedCalc PutCalc1;
    NrSpeedCalc PutCalc2;
    NrSpeedCalc GetCalc;

    int AvgSpeedVar;
    int AvgPutSpeed;
    int AvgGetSpeed;

    int AvgCounter;
    int AvgDivisor;

    void IntTurnOn  (void);
    void IntTurnOff (void);
protected:
    virtual void HandleRecordData (EzString RecData, long MsCurTime);
    virtual void HandlePreparing  (int Flag);
public:
    NrSpeedBuf (int AimSize);

    virtual ~NrSpeedBuf (void);

    void   PutSound (EzString Sound, long MsCurTime, int Expansion);

    EzString GetSound (unsigned int MaxSize, long MsCurTime);

    void TurnOff (void);
    void TurnOn  (void);

    int  GetBufRate  (void);

    int  GetPutSpeed (int shift);
    int  GetGetSpeed (int shift);

    int  GetAdjustRate (void);

    void SetMute (int Flag);
};

#endif
