/*
 * file: XxSoundVolControl.h
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_SOUNDDEV

#define H_XX_SOUNDDEV

#include <unistd.h>

#ifndef __sparc__

#include "XxStream.h"

class XxSoundVolControl : virtual public XxStream {
private:
    int      Volume;
    int      MaxLevel;
protected:
    EzString AdjustVolume (EzString Data);
public:
    XxSoundVolControl (void);

    virtual ~XxSoundVolControl (void) {};

    int  GetVolume (void) { return Volume; };
    void SetVolume (int Volume);

    int GetMaxLevel (void);
};

#endif

#endif
