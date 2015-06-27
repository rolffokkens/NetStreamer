/*
 * file: NrHistSoundBuf.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_NR_HIST_SOUNDBUF
#define H_NR_HIST_SOUNDBUF

#include "BiLink.h"
#include "EzString.h"

class NrHistSoundRec;

typedef BlSet<NrHistSoundRec> NrHistSoundSet;

class NrHistSoundBuf {
    friend NrHistSoundRec;
private:
    NrHistSoundSet::SetOwner SetOwner;
    int                      DataSize;

    void     PutData ( EzString Data,    int ExpSize
                     , long MsTimestamp, int TimestampSet);

    EzString GetData ( int &ExpSize
                     , long MsTimestamp, int TimestampSet);
public:
    NrHistSoundBuf (void);
    virtual ~NrHistSoundBuf (void);

    void PutData (EzString Data, int ExpSize) {
        PutData (Data, ExpSize, 0, 0);
    };

    void PutData (EzString Data, int ExpSize, long MsTimestamp) {
        PutData (Data, ExpSize, MsTimestamp, 1);
    };

    EzString GetData (int &ExpSize) {
        return GetData (ExpSize, 0, 0);
    };

    EzString GetData (int &ExpSize, long MsTimestamp) {
        return GetData (ExpSize, MsTimestamp, 1);
    };

    int GetDataSize (void);
};

#endif
