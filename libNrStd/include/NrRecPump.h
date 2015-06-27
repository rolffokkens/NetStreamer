/*
 * file: NrRecPump.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_RECPUMP
#define H_RECPUMP

#include <iostream.h>

#include "NrMessages.h"
#include "NrSpeedBuf.h"
#include "NrDecomp.h"
#include "NrAnalyzer.h"
#include "NrHistSoundBuf.h"

class NrRecPump {
private:
    class NrPumpSpeedBuf : public NrSpeedBuf {
    private:
        NrRecPump *pParent;
    protected:
        void HandleRecordData (EzString RecData, long MsCurTime);
        void HandlePreparing  (int Flag);
    public:
        NrPumpSpeedBuf (NrRecPump *pParent, int AimSize);
    };
    friend NrPumpSpeedBuf;

    int              AudioSeq;
    int              OsSoundDelay;
    char             SampleRate;
    int              Expansion;

    NrHistSoundBuf   HistBuf;
    NrMessageDecoder Decoder;
    NrMessageEncoder Encoder;
    NrPumpSpeedBuf   SpeedBuf;
    NrAnalyzer       Analyzer;

    void HandleRecordData (EzString RecData, long MsCurTime);
protected:
    virtual void SetMissData  (int Flag);
    virtual void SetPrepare   (int Flag);
    virtual void WriteTape    (EzString Data);
    virtual void SetExpansion (int Expand);
public:
    NrRecPump (int AimSize, EzString LogPrefix, char SampleRate);
    virtual ~NrRecPump (void);

    void Reset (void);

    void PutData     (EzString Data) { Decoder.PutData (Data); };

    int  GetMessage  (NrMsgCode &MsgCode, EzString &Data, int MsCurTime);

    EzString PutMessage (NrMsgCode MsgCode, EzString Data);

    EzString GetPCM (int Amount, int MsCurTime);

    int GetBufRate (void) { return SpeedBuf.GetBufRate (); };

    void SetOsSoundDelay (int OsSoundDelay);

    int GetMaxLevel (int MsCurTime);
    int GetMaxBandLevel (int Band, int MsCurTime);

    NrAnalyzer *GetAnalyzer (void) { return &Analyzer; };

    int  GetAdjustRate (void);

    void SetMute (int Flag);
};

#endif
