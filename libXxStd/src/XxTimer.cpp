/*
 * file:  XxTimer.cpp
 *
 * This file is distributed under the GNU GENERAL PUBLIC LICENSE.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *                    fokkensr@freemail.nl
 *                    fokkensr@vertis.nl
 *
 * Rel  When   who  What
 * 0.13 980807 Rolf A Timestamp of 0 had a meaning of "nothing", which
 *                  could cause problems after 49 days when the milliseconds
 *                  would recycle and maybe reach 0. Changed avery time
 *                  comparision to cope with recycling times.
 */
#include <iostream>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "XxTimer.h"
#include "XxAutoInit.h"

NULLINIT (XxTimer::XxTimerSet::SetOwner *pTimerOwner);

static int InitDone = 0;

void XxTimer::Init (void)
{
    if (InitDone) return;

    pTimerOwner = new XxTimerSet::SetOwner;
    XxTimer::RefreshMsCurTime ();

    AUTOINIT_TRACE ("XxTimer::Init");

    InitDone = 1;
};

XxTimer::TimeType XxTimer::MsCurTime   = 0;
XxTimer::TimeType XxTimer::MsStartTime = 0;
int               XxTimer::TimerCount  = 0;

int XxTimer::RestartTimer (void)
{
    int RetVal;

    if (RestartSet) {
        TimeOutSet = 1;
        RetVal     = 1 + (MsCurTime - MsTimeOut) / MsRestart;
        MsTimeOut += (MsRestart * RetVal);
    } else {
        TimeOutSet = 0;
        RetVal     = 1;
    };
    return RetVal;
};

void XxTimer::HandleTimeOut (int)
{
}; 

XxTimer::XxTimer (TimeType MsRestart) : Member (this)
{
    Init ();

    Member.Attach (*pTimerOwner);

    XxTimer::TimeOutSet = (MsRestart != 0);
    XxTimer::RestartSet = (MsRestart != 0);

    XxTimer::MsTimeOut  = MsCurTime;
    XxTimer::MsRestart  = MsRestart;

    XxTimer::TimerCount++;
    RestartTimer ();
};

XxTimer::~XxTimer (void)
{
    XxTimer::TimerCount--;
};

void XxTimer::RefreshMsCurTime (void)
{
    struct timeval  CurTime;
    struct timezone TimeZone;
    TimeType        MsNewTime;
    static int      DoInit = 1;

    gettimeofday (&CurTime, &TimeZone);

    MsNewTime =   (CurTime.tv_usec / 1000) + (CurTime.tv_sec * 1000);

    if (DoInit) {
        MsStartTime = MsNewTime - 1;
        MsCurTime   = 1;
        DoInit      = 0;
    } else {
        MsCurTime   = MsNewTime - MsStartTime;
    };
}

void XxTimer::HandleTimers (TimeType &mSec)
{
    XxTimer *pTimer;
    long     MsMinTime, MsTmpTime;
    int      Count;
    int      MinTimeSet;
    int      TmpTimeSet;

    Init ();

    MinTimeSet = 0;
    MsMinTime  = 0; // Only to stop the compiler warnings

    pTimer = pTimerOwner->GetFirst ();

    while (pTimer != NULL) {
        MsTmpTime  = pTimer->MsTimeOut;
        if (pTimer->TimeOutSet) {
            if (MsTmpTime - MsCurTime <= 0) {
                Count = pTimer->RestartTimer ();
                pTimer->HandleTimeOut (Count);
                MsTmpTime = pTimer->MsTimeOut;
            };
            if (MsTmpTime - MsCurTime >= 0) {
                if (!MinTimeSet || (MsTmpTime - MsMinTime) < 0) {
                    MsMinTime  = MsTmpTime;
                    MinTimeSet = 1;
                };
            };
        };
        pTimer = pTimer->Member.GetNext ();
    };
    if (MinTimeSet) {
        mSec = MsMinTime - MsCurTime;
    } else {
        mSec = -1;
    };
};

void XxTimer::SetTimeOut (int mSec)
{
    MsTimeOut  = MsCurTime + mSec;
    TimeOutSet = 1;
};

void XxTimer::SetRestart (int mSec)
{
    MsRestart  = mSec;
    RestartSet = (mSec != 0);

    RestartTimer ();
};

void XxTimer::Stop (void)
{
    SetRestart (0);
};

int XxTimer::Running (void)
{
    return TimeOutSet;
};
