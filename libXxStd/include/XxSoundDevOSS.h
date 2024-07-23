/*
 * file: XxSoundDevOSS.h
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_SOUNDDEVOSS

#define H_XX_SOUNDDEVOSS

#ifndef __sparc__

#include "XxStream.h"

class XxSoundDevOSS : virtual public XxStream {
private:
    MODE_RW  ModeRW;
    int      IntBufSize;
    int      FragSize;
    int      SampleRate;
    int      IntStereo;
    int      IntSampleBytes;
    EzString Device;

    int      Deliv16;
    int      DelivStereo;

    int      Emul16;
    int      EmulStereo;
    int      EmulMono;

    int      MaxLevel;

    int SetSampleSize (int Fd, int SampleSize);
    int SetStereo     (int Fd, int StereoFlag);
    int SetSpeed      (int Fd, int SampleRate);

    void IntClose (void);
protected:
    virtual int GetWriteChunkSize (EzString Data);

    virtual EzString ProcessReadData  (EzString Data);
    virtual EzString ProcessWriteData  (EzString Data);

    virtual void Close (void);
public:
    XxSoundDevOSS (EzString Device);

    virtual ~XxSoundDevOSS (void);

    virtual void GetRWFlags (int &rFlag, int &wFlag) {
        rFlag = 1;
        wFlag = 1;
    };

    virtual int Open
        (MODE_RW ModeRW, int SampleSize, int StereoFlag, int Speed);

    MODE_RW GetOpenMode (void) { return ModeRW; };

    virtual int GetIntOutBufSize (void);

    virtual int GetIntOutBufFree (void);

    virtual int GetIntOutDelay (void);

    int GetMaxLevel (void);
};

#endif

#endif
