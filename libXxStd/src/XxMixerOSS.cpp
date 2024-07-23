#include <iostream>
#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "XxMixerOSS.h"

int XxMixer::IntGetVolume (void)
{
    int RetVal;

    if (MixFd == -1) {
        RetVal = 0;
    } else {
        if (ioctl (MixFd, SOUND_MIXER_READ_VOLUME, &RetVal) == -1) {
            RetVal = 0;
        };
    };

    return RetVal;
};

void XxMixer::HandleTimeOut (void)
{
    int nVolume;

    nVolume = IntGetVolume ();

    if (nVolume != hVolume) {
        hVolume = nVolume;

        IntSetVolume (nVolume);
    };
};

XxMixer::XxMixer (long PollDelay) : Timer (this, PollDelay)
{
    hVolume = -1;
    sVolume = -1;

    MixFd = open ("/dev/mixer", O_RDWR);
};

XxMixer::~XxMixer (void)
{
    if (MixFd != -1) close (MixFd);
};

template <>
void XxMixer::MixTimer::HandleTimeOut (int Count)
{
    pOwner->HandleTimeOut ();
};

void XxMixer::IntSetVolume (int Volume)
{
    if (sVolume == Volume) return;

    sVolume = Volume;

    HandleVolume (Volume & 0x0ff, (Volume >> 8) & 0xff);
};

void XxMixer::SetVolume (char lVolume, char rVolume)
{
    int tVolume, Volume;

    if (lVolume > 100) lVolume = 100;
    if (rVolume > 100) rVolume = 100;
    if (lVolume < 0)   lVolume = 0;
    if (rVolume < 0)   rVolume = 0;

    Volume = lVolume + (rVolume << 8);

    if (sVolume == Volume) return;

    if (MixFd == -1) {
        tVolume = 0;
    } else {
        tVolume = Volume;
        if (ioctl (MixFd, SOUND_MIXER_WRITE_VOLUME, &tVolume) == -1) tVolume = 0;
    }

    hVolume = tVolume;

    IntSetVolume (Volume);
};

void XxMixer::GetVolume (char &lVolume, char &rVolume)
{
    int Volume;

    Volume = IntGetVolume ();

    lVolume = Volume        & 0x0ff;
    rVolume = (Volume >> 8) & 0x0ff;
};
