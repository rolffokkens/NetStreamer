/*
 * file: XxStream.cpp
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 * Rel  When   who  What
 */

#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "XxStream.h"

XxStream::XxStream (void)
{
    InCount  = 0;
    OutCount = 0;
};

void XxStream::IntHandleRead (void)
{
    char     Buf [2048];
    EzString RetVal, tStr;
    int      rStatus;

    if (GetStatus () == StatOpen) {
        rStatus = read (GetFd (), Buf, GetReadChunkSize ());

        switch (rStatus) {
        case 0:
            SetStatus (StatClosed);
            OnDisconnect ();
            break;
        case -1:
            switch (errno) {
            };
            break;
        default:
            InCount += rStatus;
            HandleRead (ProcessReadData (EzString (Buf, rStatus)));
        };
    };
};

void XxStream::Write (EzString Text)
{
    OutBuf += ProcessWriteData (Text);
};

void XxStream::IntHandleWrite (void)
{
    int sStatus, ChunkSize;

    if (GetStatus () == StatOpen) {
        ChunkSize = GetWriteChunkSize (OutBuf);
        sStatus = write (GetFd (), OutBuf.Text (), ChunkSize);

        if (sStatus < 0) {
            if (errno == EAGAIN) return;
            SetStatus (StatClosed);
            OnDisconnect ();
            return;
        };
        OutCount += sStatus;
        OutBuf = Substr (OutBuf, sStatus);
    };
    if (OutBuf.Length () == 0) OnBufEmpty ();
};

void XxStream::HandleAction (XxAction Action)
{
    switch (Action) {
    case ActRead:
        IntHandleRead ();
        break;
    case ActWrite:
        IntHandleWrite ();
        break;
    };
};

void XxStream::GetRWFlags (int &rFlag, int &wFlag)
{
    rFlag = 1;
    wFlag = (OutBuf != EzString (""));
};

XxBlocker::XxAction XxStream::GetAction  (int &rFlag, int &wFlag)
{
    if (rFlag) {
        wFlag = 0;
        return ActRead;
    };
    return (wFlag ? ActWrite : ActNone);
};

