#ifndef H_XX_MIXER
#define H_XX_MIXER

#include "XxTimer.h"

class XxMixer {
private:
    typedef tXxTimer<XxMixer> MixTimer;
    friend MixTimer;

    int MixFd;

    MixTimer Timer;

    int hVolume;
    int sVolume;

    int  IntGetVolume (void);
    void IntSetVolume (int Volume);

    void HandleTimeOut (void);
protected:
    virtual void HandleVolume (char lVolume, char rVolume) { };
public:
    XxMixer (long PollDelay = 0);
    virtual ~XxMixer (void);

    void SetPollDelay (int mSec);
    void GetVolume (char &lVolume, char &rVolume);
    void SetVolume (char  lVolume, char  rVolume);
};

#endif
