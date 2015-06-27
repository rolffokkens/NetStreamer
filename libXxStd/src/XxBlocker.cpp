/*
 * file: XxBlocker.cpp
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 * Rel  When   who  What
 * 0.9  980101 Rolf Fixed a bug an handling return value from select ()
 * 0.10 980314 Rolf MainLoop now ends when no blockers are left, and
 *                  no more PENDING timers exist.
 * 0.10 980315 Rolf Fixed bug in XxBlocker::Select
 * 0.13 980802 Rolf Fixed a bug in Select, didn't check the bStatus before
 *                  checking FD_ISSET which caused core dumps when Fd == -1
 * 0.13 980802 Rolf Fixed a bug in Close, it didn't change the bStatus to
 *                  StatClosed, which caused core dumps in Select.
 */

#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <iostream>

#include "XxBlocker.h"
#include "XxTimer.h"
#include "XxAutoInit.h"

using namespace std;

static void SigPipeIgnore (void)
{
    struct sigaction old_act, new_act;

    new_act.sa_handler = SIG_IGN;
    new_act.sa_flags   = SA_RESTART;
    memset (&(new_act.sa_mask), 0, sizeof (new_act.sa_mask));

    sigaction (SIGPIPE, &new_act, &old_act);
};

class XxSelector {
private:
    fd_set sRead, sWrite, sExc, sTimer;
    int    sMax;
    XxBlocker *PrepareFdSets (void);
    XxBlocker *PrepareFdSets (XxBlocker::XxAction &Action);
public:
    XxSelector (void) {
        FD_ZERO (&sTimer);
        SigPipeIgnore ();
        sMax = -1;
    };
    XxBlocker *Select (int mSec, XxBlocker::XxAction &Action);
};

NULLINIT (XxSelector *pSelector);
NULLINIT (XxBlocker::XxBlockerSet::SetOwner *pBlockersOwner);

static int InitDone = 0;

void XxBlocker::Init (void)
{
    if (InitDone) return;

    pSelector      = new XxSelector;
    pBlockersOwner = new XxBlocker::XxBlockerSet::SetOwner;

    AUTOINIT_TRACE ("XxBlocker::Init");

    InitDone = 1;
};

int XxBlocker::BlockerCount = 0;

XxBlocker::XxBlocker (void)
    : Member (this)
{
    Init ();

    Member.Attach (*pBlockersOwner);
    Fd      = -1;
    bStatus = StatClosed;
    BlockerCount++;
}

XxBlocker::~XxBlocker (void)
{
    Close ();

    BlockerCount--;
};

void XxBlocker::SetStatus (XxStatus bStatus)
{
    XxBlocker::bStatus = bStatus;
};

void XxBlocker::SetFd (int Fd)
{
    if (XxBlocker::Fd != -1 && Fd != -1) {
        cerr << "Changing active Fd" << endl;
        exit (1);
    };
    XxBlocker::Fd = Fd;
};


XxBlocker *XxSelector::PrepareFdSets (XxBlocker::XxAction &Action)
{
    XxBlocker *pSock;
    int Fd, tMax, rFlag, wFlag;

    FD_ZERO (&sRead);
    FD_ZERO (&sWrite);
    FD_ZERO (&sExc);

    sMax = 0;

    pSock = pBlockersOwner->GetFirst ();

    while (pSock != NULL) {
        Fd  = pSock->GetFd ();
        tMax = -1;
        if (Fd != -1 && pSock->GetStatus () == XxBlocker::StatOpen) {
            pSock->GetRWFlags (rFlag, wFlag);
            if (rFlag) {
                tMax = Fd;
                FD_SET (tMax, &sRead);
            };
            if (wFlag) {
                tMax = Fd;
                FD_SET (tMax, &sWrite);
            };
        };
        if (tMax > sMax) sMax = tMax;
        pSock = pSock->Member.GetNext ();
    };

    FD_ZERO (&sTimer);

    Action = XxBlocker::ActNone;

    return NULL;
};

XxBlocker *XxSelector::Select (int mSec, XxBlocker::XxAction &Action)
{
    XxBlocker *pSock;
    int Fd, Err, rFlag, wFlag;
    int ActWait;
    struct timeval TimeVal, *pTime;

    if (mSec >= 0) {
        TimeVal.tv_sec  = mSec / 1000;
        TimeVal.tv_usec = (mSec % 1000) * 1000;
        pTime = &TimeVal;
    } else {
        pTime =  NULL;
    };

    if (sMax < 0) {
        pSock = PrepareFdSets (Action);
        if (pSock != NULL) {
            return pSock;
        };
        for (;;) {
            Err = select ( sMax + 1, &sRead, &sWrite, &sExc, pTime);
            if (Err != -1 || errno != EINTR) break;
        };
    };
    pSock = pBlockersOwner->GetFirst ();

    for (;;) {
        if (pSock == NULL) {
            sMax = -1;
            Action = XxBlocker::ActNone;
            break;
        };
        if (pSock->GetStatus () == XxBlocker::StatOpen) {
            Fd  = pSock->GetFd ();
            pSock->GetRWFlags (rFlag, wFlag);
            rFlag &= (FD_ISSET (Fd, &sRead) ? 1 : 0);
            wFlag &= (FD_ISSET (Fd, &sWrite) ? 1 : 0);
            wFlag &= !rFlag;

            Action = pSock->GetAction (rFlag, wFlag);
            if (rFlag) FD_CLR (Fd, &sRead);
            if (wFlag) FD_CLR (Fd, &sWrite);
            if (Action != XxBlocker::ActNone) break;
        };
        pSock = pSock->Member.GetNext ();
    };
    XxTimer::RefreshMsCurTime ();

    return pSock;
};

XxBlocker *XxBlocker::Select (int mSec, XxBlocker::XxAction &Action)
{
    XxBlocker *pSock;

    Init ();

    pSock = pSelector->Select (mSec, Action);
    if (pSock == NULL) pSock = pSelector->Select (0, Action);

    return pSock;
};

void XxBlocker::Close (void)
{
    if (Fd != -1) {
        close (Fd);
        Fd      = -1;
        bStatus = StatClosed;
    };
};

int XxBlocker::HandleTimers (void)
{
    XxTimer::TimeType mSec;

    Init ();

    XxTimer::RefreshMsCurTime ();

    XxTimer::HandleTimers (mSec);

    return mSec;
};

int XxBlocker::HandleBlocker (int mSec)
{
    XxBlocker           *pBlocker;
    XxBlocker::XxAction bAction;
    int                 RetVal;

    pBlocker = XxBlocker::Select (mSec, bAction);

    if (bAction == XxBlocker::ActNone) {
        RetVal = 0;
    } else {
        pBlocker->HandleAction (bAction);
        RetVal = 1;
    };
    return RetVal;
};

int XxBlocker::MainLoop (void)
{
    int                 mSec;

    Init ();

    for (;;) {
        mSec = HandleTimers ();
        if (BlockerCount == 0 && mSec == -1) break;
        for (;;) {
            CleanupWasteBasket ();
            if (BlockerCount == 0 && mSec == -1) break;
            if (!HandleBlocker (mSec)) break;
        };
    };

    return 0;
};

