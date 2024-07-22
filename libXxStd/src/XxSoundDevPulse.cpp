/*
 * file: XxSoundDevPulse.cpp 
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
#include <signal.h>
/*
#include <sys/soundcard.h>
*/
#include <sys/ioctl.h>

#include <pulse/simple.h>
#include <pulse/error.h>

#include "XxSoundDevPulse.h"

using namespace std;


#define BUFSIZE 1024

int pulse_server_play(int datafd, int statusfd, const char *name, int SampleSize, int StereoFlag, int SampleRate, const char *dev) {
    /* The Sample format to use */
    static const pa_sample_spec ss = {
        .format = (SampleSize == 16 ? PA_SAMPLE_S16LE : PA_SAMPLE_U8),
        .rate = (uint32_t)SampleRate,
        .channels = (uint8_t)(StereoFlag ? 2 : 1)
    };
 
    pa_simple *s = NULL;
    int ret = 1;
    int error;

    if (dev[0] == '\0') dev = NULL;

    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, name, PA_STREAM_PLAYBACK, dev, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
 
    for (;;) {
        uint8_t buf[BUFSIZE];
        ssize_t r;
	int lat;
 
        pa_usec_t latency;
 
        if ((latency = pa_simple_get_latency(s, &error)) == (pa_usec_t) -1) {
            fprintf(stderr, __FILE__": pa_simple_get_latency() failed: %s\n", pa_strerror(error));
            goto finish;
        }
	if (latency) {
            lat = latency;
            write(statusfd, (char*)(&lat), sizeof(lat));
        }
/* 
        fprintf(stderr, "%0.0f usec    \n", (float)latency);
*/ 
        /* Read some data ... */
        if ((r = read(datafd, buf, sizeof(buf))) <= 0) {
            if (r == 0) /* EOF */
                break;
 
            fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
            goto finish;
        }
 
        /* ... and play it */
        if (pa_simple_write(s, buf, (size_t) r, &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            goto finish;
        }
    }
 
    /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        goto finish;
    }
 
    ret = 0;
 
finish:
 
    if (s)
        pa_simple_free(s);
 
    return ret;
}






XxSoundDevPulse::XxSoundDevPulse (EzString Device)
{
    ModeRW                  = ModeRead;
    IntBufSize              = -1;
    FragSize                = -1;
    MaxLevel                = 0;
    SampleRate              = 0;
    IntStereo               = 0;
    IntSampleBytes          = 0;
    ChildPID                = -1;
    StatusFd                = -1;
    XxSoundDevPulse::Device = Device;
    Latency                 = 0;
    PipeLatency             = 0;
};

void XxSoundDevPulse::IntClose (void)
{
/*
    if (GetStatus () == StatOpen) {
        ioctl (GetFd (), SNDCTL_DSP_RESET, NULL);
    };
*/
    if (ChildPID != -1) {
        kill (ChildPID, SIGKILL);
        ChildPID = -1;
    };
};

XxSoundDevPulse::~XxSoundDevPulse (void)
{
    IntClose ();
};


void XxSoundDevPulse::Write (EzString Data)
{
    ssize_t siz;
    int lat;

    XxStream::Write (Data);
    for (;;) {
        siz = read (StatusFd, (void *)(&lat), sizeof(lat));
	if (siz == -1) break;
        if (siz == sizeof(lat)) {
            Latency = (Latency * 7 + lat) / 8;
        }
    }
};


void XxSoundDevPulse::Close (void)
{
    IntClose ();
    XxStream::Close ();
};

EzString XxSoundDevPulse::ProcessReadData  (EzString Data)
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

EzString XxSoundDevPulse::ProcessWriteData  (EzString Data)
{
    const short *pShort;
    short       Sample;
    int         i, Size, Max;
    EzString    RetVal;

    //
    // Remark: Only Positive Samples are seen.
    // 
    // Dangerous in odd addressing for non Intel!
    // ToDo: use memcpy first when odd!
    //       8 bits samples???

    pShort = (const short *)(Data.Text ());
    Size   = (Data.Length () >> 1);
    Max    = MaxLevel;

    if (Emul16) {
        char *pOutBuf, *pOut;

        pOutBuf = new char[Size];
        pOut    = pOutBuf;

        for (i = Size; i > 0; i--) {
            Sample = *pShort++;
            if (Sample > Max) Max = Sample;
            *pOut++ = (Sample >> 8) ^ 0x80;
        };

        RetVal = EzString (pOutBuf, Size);

        delete [] pOutBuf;
    } else {
        for (i = Size; i > 0; i--) {
            Sample = *pShort++;
            if (Sample > Max) Max = Sample;
        };
        RetVal = Data;
    };
    RetVal = XxStream::ProcessWriteData (RetVal);

    MaxLevel = Max;

    return RetVal;
};

int XxSoundDevPulse::GetWriteChunkSize (EzString Data)
{
    int RetVal;
    RetVal = Data.Length ();
    return (RetVal > 2048 ? 2048 : RetVal);
/*
    struct audio_buf_info BufInfo;
    int RetVal;

    ioctl (GetFd (), SNDCTL_DSP_GETOSPACE, &BufInfo);

    RetVal = Data.Length ();

    if (RetVal > BufInfo.bytes) RetVal = BufInfo.bytes;

    return RetVal;
*/
};

int XxSoundDevPulse::GetIntOutBufFree (void)
{
    return 2048;
/*
    int Fd = GetFd ();
    struct audio_buf_info BufInfo;

    ioctl (Fd, SNDCTL_DSP_GETOSPACE, &BufInfo);

    return BufInfo.bytes;
*/
};

int XxSoundDevPulse::GetIntOutBufSize (void)
{
    return 2048;
/*
    return IntBufSize - GetIntOutBufFree ();
*/
};

/*
int XxSoundDevPulse::SetSampleSize (int Fd, int SampleSize)
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

int XxSoundDevPulse::SetStereo (int Fd, int StereoFlag)
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

int XxSoundDevPulse::SetSpeed (int Fd, int SampleRate)
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
*/

int XxSoundDevPulse::Open (MODE_RW ModeRW, int SampleSize, int StereoFlag, int SampleRate)
{
/*
    struct audio_buf_info BufInfo;
*/
    int  Fd, ChildFd, oMode;
    char buf[128];
    int datafd[2], statusfd[2], pid;
    int pipe_size;

    Close ();

    XxSoundDevPulse::ModeRW     = ModeRW;
    XxSoundDevPulse::SampleRate = SampleRate;

    pipe(datafd);
    pipe(statusfd);

    Fd      = datafd[ModeRW == ModeRead ? 0 : 1];
    ChildFd = datafd[ModeRW == ModeRead ? 1 : 0];

    pid = fork ();
    if (pid == 0) {
        exit (pulse_server_play (ChildFd, statusfd[1], "XxSoundDevPulse", SampleSize, StereoFlag, SampleRate, Device));
    }
    ChildPID = pid;
    StatusFd = statusfd[0];

    fcntl (StatusFd, F_SETFL, O_NONBLOCK);
    pipe_size = fcntl (Fd, F_SETPIPE_SZ, 8000);

    PipeLatency = pipe_size * 1000 / (SampleSize/8) / SampleRate / (StereoFlag ? 2 : 1);

    cerr << "fcnt:" << pipe_size << ":" << PipeLatency << endl;

/*
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

*/
    SetFd (Fd);
    SetStatus (StatOpen);

    return 1;
};

int XxSoundDevPulse::GetMaxLevel (void)
{
    int RetVal;

    RetVal   = MaxLevel;
    MaxLevel = 0;

    return RetVal;
};

int XxSoundDevPulse::GetIntOutDelay (void)
{
    return (Latency/1000 + PipeLatency);

    return Latency == 0 ? 200 : Latency / 1000;
/*
    return    (IntBufSize * 1000)
            / ((IntStereo ? 2 : 1) * IntSampleBytes * SampleRate);
*/
};

#endif
