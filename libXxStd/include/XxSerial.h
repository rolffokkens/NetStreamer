/*
 * file: XxSerial.h
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_SERIAL

#define H_XX_SERIAL

#include "XxStream.h"

class XxSerial : virtual public XxStream {
private:
    int     Speed;
    int     nBits;
    int     ParEnable;
    int     ParEven;
protected:
    virtual int GetWriteChunkSize (EzString Data);
public:
    XxSerial (void);

    virtual ~XxSerial (void);

    virtual int Open
        (int Speed, int nBits, int ParEnable, int ParEven);
};

#endif
