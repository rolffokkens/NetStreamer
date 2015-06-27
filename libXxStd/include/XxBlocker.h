/*
 * file: XxBlocker.h
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_BLOCKER
#define H_XX_BLOCKER

#include "BiLink.h"
#include "XxBase.h"

class XxSelector;

class XxBlocker : public virtual XxBase {
    friend XxSelector;
public:
    enum XxStatus { StatOpen, StatClosed };
    enum XxAction { ActNone = 0, ActRead = 1, ActWrite = 2, ActAccept = 4 };

    typedef BlSet<XxBlocker> XxBlockerSet;
private:
    static int BlockerCount;

    int      Fd;
    XxStatus bStatus;

    XxBlockerSet Member;

    static XxBlocker *Select (int mSec, XxBlocker::XxAction &Action);

    static void Init (void);

    virtual void HandleAction (XxAction Action) = 0;
protected:
    XxBlocker (void);

    void SetStatus (XxStatus bStatus);
    void SetFd     (int      Fd     );

    virtual void OnDisconnect (void) { };
    virtual void OnBufEmpty   (void) { };

    virtual void Close (void);
public:
    virtual ~XxBlocker (void);

    virtual XxStatus GetStatus  (void) const { return bStatus; };
    virtual int      GetFd      (void) const { return Fd;    };

    virtual void     GetRWFlags (int &rFlag, int &wFlag) = 0;
    virtual XxAction GetAction  (int &rFlag, int &wFlag) = 0;

    virtual long GetInCount  (void) = 0;
    virtual long GetOutCount (void) = 0;

    static int HandleTimers (void);
    static int HandleBlocker (int mSec);
    static int MainLoop     (void);

    static int  GetBlockerCount (void) { return BlockerCount; };
};

#endif
