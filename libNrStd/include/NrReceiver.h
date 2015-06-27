/*
 * file: NrReceiver.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_TRANS
#define H_TRANS

#include "BiLink.h"
#include "EzString.h"
#include "XxTimer.h"
#include "XxSocket.h"
#include "NrNetRadio.h"
#include "NrComp.h"
#include "NrDecomp.h"
#include "XxSoundDev.h"
#include "NrRecPump.h"

class NrRecConnection;

class NrRecSoundDev : public XxSoundDev {
    friend NrRecConnection;
private:
    NrRecConnection *pConnection;

    NrRecSoundDev (NrRecConnection *pConnection);

    virtual void IntHandleWrite (void);
public:
    virtual void GetRWFlags (int &rFlag, int &wFlag) {
        rFlag = 1;
        wFlag = 1;
    };
    virtual int Open
        (MODE_RW ModeRW, int SampleSize, int StereoFlag, int Speed);
};

class NrRecConnection : public NrRadioConnection {
    friend NrRecSoundDev;
private:
    class NrTapePump : public NrRecPump {
    private:
        int              TapeFd;
        NrRecConnection *pConnection;
    protected:
        void SetMissData  (int Flag);
        void SetPrepare   (int Flag);
        void WriteTape    (EzString Data);
        void SetExpansion (int Expansion);
    public:
        NrTapePump
            (NrRecConnection *pConnection, int AimSize
           , EzString LogPrefix, char SampleRate
           );
        virtual ~NrTapePump (void);
        void SetRecording (int Flag);
    };
    friend NrTapePump;

    EzString   AddInfo;
    int        MuteFlag;
    int        SampleRate;

    NrTapePump Pump;
    int        Initialized;

    typedef void (NrRecConnection::*TRANS_MSGHND)
                     (NrMsgCode MsgCode, EzString Data);

    int           Freq;
    NrRecSoundDev SoundDev;
    TRANS_MSGHND  pCurMsgHandler;

    int OpenDev (void);

    void PhReadyHandleMessage  (NrMsgCode MsgCode, EzString Data);
    void PhTuneHandleMessage   (NrMsgCode MsgCode, EzString Data);
    void PhMuteHandleMessage   (NrMsgCode MsgCode, EzString Data);
    void PhModeHandleMessage   (NrMsgCode MsgCode, EzString Data);
    void PhClientHandleMessage (NrMsgCode MsgCode, EzString Data);
    void PhBinaryHandleMessage (NrMsgCode MsgCode, EzString Data);
    void PhHelloHandleMessage  (NrMsgCode MsgCode, EzString Data);
    void HandleMessage         (NrMsgCode MsgCode, EzString Data);

    void CloseRec (void) { };
    void Constr (int Freq);

    EzString GetSound (int Amount) {
        return Pump.GetPCM (Amount, XxTimer::GetMsCurTime ());
    };
protected:
    virtual void HandleRead      (EzString Data);
    virtual void OnConnect       (void);

    virtual void HandleTuning      (int Freq);
    virtual void HandleSignal      (int Freq, EzString Description);
    virtual void HandleNoSignal    (int Freq);
    virtual void HandleInitDone    (void);
    virtual void HandleStationName (EzString Name);
    virtual void HandleMessage (NrMsgCode MsgCode, EzString Id, EzString Data);

    int  DoTune     (int Freq);
    int  DoTuneUp   (int Freq);
    int  DoTuneDown (int Freq);

    int GetMaxLevel         (int MsCurTime) {
        return Pump.GetMaxLevel (MsCurTime);
    };
    int GetMaxBandLevel  (int Band, int MsCurTime) {
        return Pump.GetMaxBandLevel (Band, MsCurTime);
    };
    int GetBufRate          (void) { return Pump.GetBufRate ();         };
    int GetSoundDelay       (void) { return SoundDev.GetIntOutDelay (); };
    NrAnalyzer *GetAnalyzer (void) { return Pump.GetAnalyzer ();        };
    int GetAdjustRate       (void) { return Pump.GetAdjustRate ();      };

    virtual void HandleMissData  (int Flag);
    virtual void HandlePrepare   (int Flag);
    virtual void HandleExpansion (int Expansion);
    virtual void HandleMute      (int Flag);
public:
    NrRecConnection (int Freq, char SampleRate, EzString AddInfo);

    virtual ~NrRecConnection (void) { };

    void SetRecording (int Flag);

    int  SetMute      (int Flag);
};

#endif
