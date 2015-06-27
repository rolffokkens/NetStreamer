/*
 * file:  XxTimer.h
 *
 * This file is distributed under the GNU GENERAL PUBLIC LICENSE.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *                    fokkensr@freemail.nl
 *                    fokkensr@vertis.nl
 *
 */
#ifndef H_XX_TIMER
#define H_XX_TIMER

#include "BiLink.h"

class XxTimer {
public:
    typedef long TimeType;

    typedef BlSet<XxTimer> XxTimerSet;
private:
    XxTimerSet Member;

    static TimeType MsCurTime;
    static TimeType MsStartTime;
    static int      TimerCount;

    int             TimeOutSet;
    int             RestartSet;

    TimeType        MsTimeOut;
    TimeType        MsRestart;

    static void Init (void);

    int RestartTimer (void);
    virtual void HandleTimeOut (int Count);
protected:
    XxTimer (TimeType MsRestart = 0);
    virtual ~XxTimer (void);
    TimeType GetTimeOut (void) { return MsTimeOut; };
public:
    void SetTimeOut (int mSec);
    void SetRestart (int mSec);
    void Stop (void);
    int  Running (void);
    static void HandleTimers (TimeType &mSec);
    static void RefreshMsCurTime (void);
    static TimeType GetMsCurTime  (void) { return MsCurTime;  };
    static int  GetTimerCount (void) { return TimerCount; };
};

template<class Owner> class tXxTimer : public XxTimer {
private:
    Owner *pOwner;
    virtual void HandleTimeOut (int Count);
public:
    tXxTimer (Owner *pOwner, int MsRestart = 0)
        : XxTimer (MsRestart)
    {
        tXxTimer::pOwner = pOwner;
    };
};

#endif
