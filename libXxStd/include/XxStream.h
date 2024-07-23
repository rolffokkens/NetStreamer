/*
 * file: XxStream.h
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 * Rel  When   who  What
 * 0.10 980315 Rolf Added XxStream::OnBufEmpty
 * 0.11 980426 Rolf Added InCount, OutCount
 */

#ifndef H_XX_STREAM

#define H_XX_STREAM

#include "EzString.h"
#include "XxBlocker.h"

class XxStream : virtual public XxBlocker {
public:
    enum MODE_RW { ModeRead, ModeWrite };
private:
    EzString OutBuf;
    long     InCount, OutCount;
protected:
    virtual void IntHandleRead  (void);
    virtual void IntHandleWrite (void);

    XxStream (void);

    virtual EzString ProcessWriteData (EzString Data) { return Data; };
    virtual EzString ProcessReadData  (EzString Data) { return Data; };

    virtual int GetWriteChunkSize (EzString Data) { return Data.Length (); };
    virtual int GetReadChunkSize  (void)          { return 2048;           };

    virtual void HandleRead (EzString Data) { };

    virtual void Write (EzString Data);

    void HandleAction (XxAction Action);
public:
    virtual ~XxStream (void) { };

    virtual void GetRWFlags (int &rFlag, int &wFlag);

    XxAction GetAction  (int &rFlag, int &wFlag);

    int GetOutBufSize (void) { return OutBuf.Length (); };

    long GetInCount  (void) { return InCount;  };
    long GetOutCount (void) { return OutCount; };

    virtual int IsOpen (void) { return 1; };
};

#endif
