/*
 * file: XxSoundVolControl.h
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_SOUNDDEV

#define H_XX_SOUNDDEV

#include <unistd.h>

#ifndef __sparc__

#include "XxStream.h"

class XxSoundVolControl : virtual public XxStream {
private:
/*
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
*/
    int      Volume;
    int      MaxLevel;
/*
    void IntClose (void);
    int ChildGone (void);
*/
protected:
/*
    virtual int GetWriteChunkSize (EzString Data);
*/

    EzString AdjustVolume (EzString Data);
/*
    virtual EzString ProcessReadData  (EzString Data);
    virtual EzString ProcessWriteData  (EzString Data);
    virtual void Write (EzString Data);
    virtual void Close (void);
*/
public:
    XxSoundVolControl (void);

    virtual ~XxSoundVolControl (void) {};

    int  GetVolume (void) { return Volume; };
    void SetVolume (int Volume);

    int GetMaxLevel (void);
};

#endif

#endif
