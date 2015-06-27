/*
 * file: XxStdio.h
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_STDIO

#define H_XX_STDIO

#include "XxStream.h"

class XxStdIn : virtual public XxStream {
public:
    XxStdIn (void);

    virtual ~XxStdIn (void);
};

class XxStdOut : virtual public XxStream {
private:
protected:
    virtual int GetWriteChunkSize (EzString Data);
public:
    XxStdOut (void);

    virtual ~XxStdOut (void);

    virtual void GetRWFlags (int &rFlag, int &wFlag);
};

#endif
