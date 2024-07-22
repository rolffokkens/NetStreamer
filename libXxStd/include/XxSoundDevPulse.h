/*
 * file: XxSoundDevPulse.h
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_SOUNDDEVPULSE

#define H_XX_SOUNDDEVPULSE

#include <unistd.h>

#ifndef __sparc__

#include "XxStream.h"

class XxSoundDevPulse : virtual public XxStream {
private:
    MODE_RW  ModeRW;
    int      IntBufSize;
    int      FragSize;
    int      SampleRate;
    int      IntStereo;
    int      IntSampleBytes;
    EzString Device;
    pid_t    ChildPID;
    pid_t    StatusFd;
    int      Latency;
    int      PipeLatency;

    int      Deliv16;
    int      DelivStereo;

    int      Emul16;
    int      EmulStereo;
    int      EmulMono;

    int      MaxLevel;

    void IntClose (void);
protected:
    virtual int GetWriteChunkSize (EzString Data);

    virtual EzString ProcessReadData  (EzString Data);
    virtual EzString ProcessWriteData  (EzString Data);

    virtual void Write (EzString Data);

    virtual void Close (void);
public:
    XxSoundDevPulse (EzString Device);

    virtual ~XxSoundDevPulse (void);

    virtual int Open
        (MODE_RW ModeRW, int SampleSize, int StereoFlag, int SampleRate);

    MODE_RW GetOpenMode (void) { return ModeRW; };

    virtual int GetIntOutBufSize (void);

    virtual int GetIntOutBufFree (void);

    virtual int GetIntOutDelay (void);

    int GetMaxLevel (void);
};

#endif

#endif
