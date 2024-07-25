/*
 * file: XxSoundVolControl.cpp 
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/volume.h>

#include "XxSoundVolControl.h"

using namespace std;

XxSoundVolControl::XxSoundVolControl (EzString AppName)
{
    XxSoundVolControl::Volume   = 65536;
    XxSoundVolControl::MaxLevel = 0;
    XxSoundVolControl::AppName = AppName;
};

EzString XxSoundVolControl::AdjustVolume (EzString Data)
{
    const short *pShort;
    short       Sample;
    int         i, Size, Max;
    EzString    RetVal;
    short *pOutBuf, *pOut;

    //
    // Remark: Only Positive Samples are seen.
    // 
    // Dangerous in odd addressing for non Intel!
    // ToDo: use memcpy first when odd!
    //       8 bits samples???

    pShort = (const short *)(Data.Text ());
    Size   = (Data.Length () >> 1);
    Max    = MaxLevel;

    pOutBuf = new short[Size];
    pOut    = pOutBuf;

    for (i = Size; i > 0; i--) {
        Sample = (*pShort++ * Volume) >> 16;
        if (Sample > Max) Max = Sample;
        *pOut++ = Sample;
    };
    RetVal = EzString ((char *)pOutBuf, Size * 2);

    delete [] pOutBuf;

    RetVal = XxStream::ProcessWriteData (RetVal);

    MaxLevel = Max;

    return RetVal;
};

int XxSoundVolControl::GetMaxLevel (void)
{
    int RetVal;

    RetVal   = MaxLevel;
    MaxLevel = 0;

    return RetVal;
};

void XxSoundVolControl::SetVolume (int Volume)
{
    XxSoundVolControl::Volume = Volume;
}
