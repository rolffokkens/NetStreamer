/*
 * file: NrHistSoundBuf.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <iostream>
#include "NrHistSoundBuf.h"

//  static SeqId = 0;

class NrHistSoundRec {
    friend NrHistSoundBuf;
public:
    NrHistSoundSet SetMember;
    long           MsTimestamp;
    int            TimestampSet;
    EzString       Data;
    int            ExpSize;

//  int SeqId;

    NrHistSoundRec
        ( NrHistSoundBuf *pOwner, EzString Data, int ExpSize
        , long MsTimestamp, int TimestampSet
        );
    virtual ~NrHistSoundRec (void);
};

NrHistSoundRec::NrHistSoundRec
    ( NrHistSoundBuf *pOwner, EzString Data, int ExpSize
    , long MsTimestamp, int TimestampSet)
    : SetMember (this)
{
    NrHistSoundRec::Data         = Data;
    NrHistSoundRec::ExpSize      = ExpSize;
    NrHistSoundRec::MsTimestamp  = MsTimestamp;
    NrHistSoundRec::TimestampSet = TimestampSet;

//  SeqId = ::SeqId++;

    SetMember.Attach (pOwner->SetOwner);
};

NrHistSoundRec::~NrHistSoundRec (void)
{
};

NrHistSoundBuf::NrHistSoundBuf (void)
{
    DataSize = 0;
};

NrHistSoundBuf::~NrHistSoundBuf (void)
{
    NrHistSoundRec *pRec;

    for (;;) {
        pRec = SetOwner.GetFirst ();
        if (pRec == NULL) break;
        delete pRec;
    };
};

void NrHistSoundBuf::PutData
    (EzString Data, int ExpSize, long MsTimestamp, int TimestampSet)
{
    new NrHistSoundRec (this, Data, ExpSize, MsTimestamp, TimestampSet);

    DataSize += Data.Length ();
};

EzString NrHistSoundBuf::GetData
    (int &ExpSize, long MsTimestamp, int TimestampSet)
{
    NrHistSoundRec *pRec;
    EzString        RetVal;

    pRec = SetOwner.GetLast ();

    if (pRec == NULL || (   TimestampSet && pRec->TimestampSet
                         && (pRec->MsTimestamp - MsTimestamp) > 0)) {
        RetVal    = EzString ("");
        ExpSize   = 0;
    } else {
        RetVal    = pRec->Data;
        ExpSize   = pRec->ExpSize;
        DataSize -= RetVal.Length ();
        delete pRec;
    };

    return RetVal;
};

int NrHistSoundBuf::GetDataSize (void)
{
    return DataSize;
};

