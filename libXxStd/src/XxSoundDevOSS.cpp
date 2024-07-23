/*
 * file: XxSoundDevOSS.cpp 
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef __sparc__

#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/soundcard.h>
#include <sys/ioctl.h>

#include "XxSoundDevOSS.h"

using namespace std;

XxSoundDevOSS::XxSoundDevOSS (EzString Device)
{
    ModeRW                = ModeRead;
    IntBufSize            = -1;
    FragSize              = -1;
    SampleRate            = 0;
    IntStereo             = 0;
    IntSampleBytes        = 0;
    XxSoundDevOSS::Device = Device;
};

void XxSoundDevOSS::IntClose (void)
{
    if (GetStatus () == StatOpen) {
        ioctl (GetFd (), SNDCTL_DSP_RESET, NULL);
    };
};

XxSoundDevOSS::~XxSoundDevOSS (void)
{
    IntClose ();
};


void XxSoundDevOSS::Close (void)
{
    IntClose ();
    XxStream::Close ();
};

EzString XxSoundDevOSS::ProcessReadData  (EzString Data)
{
    short      *pOutBuf16, *pO16;
    char       *pOutBuf8,  *pO8;
    const char *pInBuf8,   *pI8, *pText;
    short      *pInBuf16,  *pI16;
    int        InLen, OutLen;
    EzString   RetVal;
    short      SampleL, SampleR;
    int        Sample;

    Data = XxStream::ProcessReadData (Data);

    if (Deliv16) {
        if (Emul16) { // Deliv16 && Emul16
            InLen      = Data.Length ();

            pInBuf8    = Data.Text ();
            pI8        = pInBuf8;

            if (EmulMono) { // Deliv16 && Emul16 && EmulMono
                OutLen     = InLen >> 1;
                pOutBuf16  = new short [OutLen + 1];
                pO16       = pOutBuf16;

                while (InLen > 0) {
                    SampleL = (((*pI8++) ^ 0x80) << 7);
                    SampleR = (((*pI8++) ^ 0x80) << 7);
 
                    *pO16++ = SampleL + SampleR;
                    InLen  -= 2;
                };
            } else {
                if (EmulStereo) { // Deliv16 && Emul16 && EmulStereo
                    OutLen     = InLen + InLen;
                    pOutBuf16  = new short [OutLen + 1];
                    pO16       = pOutBuf16;

                    while (InLen > 0) {
                        SampleL = (((*pI8++) ^ 0x80) << 8);

                        *pO16++ = SampleL;
                        *pO16++ = SampleL;
                        InLen  -= 1;
                    };
                } else { // Deliv16 && Emul16 && !EmulMono && !EmulStereo
                    OutLen     = InLen;
                    pOutBuf16  = new short [OutLen + 1];
                    pO16       = pOutBuf16;

                    while (InLen > 0) {
                        SampleL = (((*pI8++) ^ 0x80) << 8);

                        *pO16++ = SampleL;
                        InLen--;
                    };
                };
            };
        } else { // Deliv16 && !Emul16
            if (!EmulMono && !EmulStereo) return Data;

            InLen    = Data.Length () >> 1;
            pText    = Data.Text ();

            if (uint64_t (pText) & (sizeof (short) - 1)) {
                pInBuf16 = new short [InLen + 1];
                memcpy (pInBuf16, Data.Text (), InLen + InLen);
            } else {
                pInBuf16 = (short *)pText;
            };
            pI16     = pInBuf16;

            if (EmulMono) { // Deliv16 && !Emul16 && EmulMono
                OutLen     = InLen >> 1;
                pOutBuf16  = new short [OutLen + 1];
                pO16       = pOutBuf16;

                while (InLen > 0) {
                    SampleL = *pI16++;
                    SampleR = *pI16++;

                    *pO16++ = short ((int (SampleL) + int (SampleR)) >> 1);
                    InLen  -= 2;
                };
            } else { // Deliv16 && !Emul16 && EmulStereo
                OutLen     = InLen + InLen;
                pOutBuf16  = new short [OutLen + 1];
                pO16       = pOutBuf16;

                while (InLen > 0) {
                    SampleL = *pI16++;

                    *pO16++ = SampleL;
                    *pO16++ = SampleL;
                    InLen  -= 1;
                };
            };
            if (uint64_t (pText) & (sizeof (short) - 1)) {
                delete pInBuf16;
            };
        };
        RetVal = EzString ((char *)pOutBuf16, (char *)pO16 - (char *)pOutBuf16);

        delete pOutBuf16;
    } else { // !Deliv16
        if (!EmulMono && !EmulStereo) return Data;

        InLen      = Data.Length ();
        pInBuf8    = Data.Text ();
        pI8        = pInBuf8;

        if (EmulMono) { // !Deliv16 && EmulMono
            OutLen    = InLen >> 1;
            pOutBuf8  = new char [OutLen + 1];
            pO8       = pOutBuf8;
            while (InLen > 0) {
                SampleL = ((*pI8++) ^ 0x80);
                SampleR = ((*pI8++) ^ 0x80);
 
                *pO8++ = ((SampleL + SampleR) >> 1) ^ 0x80;
                InLen -= 2;
            };
        } else { // !Deliv16 && EmulStereo
            OutLen    = InLen + InLen;
            pOutBuf8  = new char [OutLen + 1];
            pO8       = pOutBuf8;

            while (InLen > 0) {
                SampleL = *pI8++;

                *pO8++ = SampleL;
                *pO8++ = SampleL;
                InLen -= 1;
            };
        };
        RetVal = EzString (pOutBuf8, pO8 - pOutBuf8);

        delete pOutBuf8;
    };

    return RetVal;
};

EzString XxSoundDevOSS::ProcessWriteData  (EzString Data)
{
    const short *pShort;
    short       Sample;
    int         i, Size;
    EzString    RetVal;

    //
    // Remark: Only Positive Samples are seen.
    // 
    // Dangerous in odd addressing for non Intel!
    // ToDo: use memcpy first when odd!
    //       8 bits samples???

    Data = XxSoundVolControl::AdjustVolume (Data);

    pShort = (const short *)(Data.Text ());
    Size   = (Data.Length () >> 1);

    if (Emul16) {
        char *pOutBuf, *pOut;

        pOutBuf = new char[Size];
        pOut    = pOutBuf;

        for (i = Size; i > 0; i--) {
            Sample = *pShort++;
            *pOut++ = (Sample >> 8) ^ 0x80;
        };

        RetVal = EzString (pOutBuf, Size);

        delete [] pOutBuf;
    } else {
        RetVal = Data;
    };
    RetVal = XxStream::ProcessWriteData (RetVal);

    return RetVal;
};

int XxSoundDevOSS::GetWriteChunkSize (EzString Data)
{
    struct audio_buf_info BufInfo;
    int RetVal;

    ioctl (GetFd (), SNDCTL_DSP_GETOSPACE, &BufInfo);

    RetVal = Data.Length ();

    if (RetVal > BufInfo.bytes) RetVal = BufInfo.bytes;

    return RetVal;
};

int XxSoundDevOSS::GetIntOutBufFree (void)
{
    int Fd = GetFd ();
    struct audio_buf_info BufInfo;

    ioctl (Fd, SNDCTL_DSP_GETOSPACE, &BufInfo);

    return BufInfo.bytes;
};

int XxSoundDevOSS::GetIntOutBufSize (void)
{
    return IntBufSize - GetIntOutBufFree ();
};

int XxSoundDevOSS::SetSampleSize (int Fd, int SampleSize)
{
    int Size = SampleSize;

    while (ioctl (Fd, SNDCTL_DSP_SAMPLESIZE, &Size) < 0) {
        if (Size == 8) {
cerr << "Sample Size Selection Error" << endl;
            return 0;
        };
        Size = 8;
    };

    IntSampleBytes = (SampleSize + 7) / 8;
    Deliv16        = (SampleSize == 16);
    Emul16         = (Size != SampleSize);

    return 1;
};

int XxSoundDevOSS::SetStereo (int Fd, int StereoFlag)
{
    int Stereo;

    Stereo = (ModeRW == ModeRead ? 1 : StereoFlag);

    while (ioctl (Fd, SNDCTL_DSP_STEREO,     &Stereo) < 0) {
        if (Stereo == 0) {
cerr << "Stereo Selection Error" << endl;
            return 0;
        };
        Stereo = 0;
    };

    IntStereo   =  Stereo;
    DelivStereo =  StereoFlag;
    EmulStereo  =  StereoFlag && (Stereo != StereoFlag);
    EmulMono    = !StereoFlag && (Stereo != StereoFlag);

    return 1;
};

int XxSoundDevOSS::SetSpeed (int Fd, int SampleRate)
{
    int RetVal;

    RetVal = (ioctl (Fd, SNDCTL_DSP_SPEED, &SampleRate) == 0);

    if (!RetVal) {
cerr << "Speed Selection Error" << endl;
    };

{
    int max_fragments = 32;
    int size_selector = 10;
    int frag = (max_fragments << 16) | (size_selector);
    RetVal = (ioctl (Fd, SNDCTL_DSP_SETFRAGMENT, &frag) == 0);
}

    return RetVal;
};

int XxSoundDevOSS::Open (MODE_RW ModeRW, int SampleSize, int StereoFlag, int Speed)
{
    struct audio_buf_info BufInfo;
    int  Fd, oMode;
    char buf[128];

    Close ();

    XxSoundDevOSS::ModeRW = ModeRW;
    oMode = (ModeRW == ModeRead ? O_RDONLY : O_WRONLY);
    Fd = open (Device, oMode | O_NONBLOCK, 0);

    if (Fd == -1) return 0;

    if (   ioctl (Fd, SNDCTL_DSP_SYNC,       NULL       ) < 0
        || !SetSampleSize (Fd, SampleSize)
        || !SetStereo     (Fd, StereoFlag)
        || !SetSpeed      (Fd, Speed     ) ) {
        close (Fd);

        return 0;
    }
    if (ModeRW == ModeRead) {
        read (Fd, buf, 128); // Workaround for bug in pre OSS 3.6
    };

    ioctl (Fd, SNDCTL_DSP_GETOSPACE, &BufInfo);

    IntBufSize     = BufInfo.fragstotal * BufInfo.fragsize;
    FragSize       = BufInfo.fragsize;
    SampleRate     = Speed;

    SetFd (Fd);
    SetStatus (StatOpen);

    return 1;
};

int XxSoundDevOSS::GetIntOutDelay (void)
{
    int div = (IntStereo ? 2 : 1) * IntSampleBytes * SampleRate;
    int delay = div ? (IntBufSize * 1000) / div : 1000;

    return delay;
};

#endif
