/*
 * file: XxStdio.cpp
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 * Rel  When   who  What
 * 0.10 980315 Rolf Fixed bug in XxStdOut::GetRWFlags
 *
 */

#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "XxStdio.h"

XxStdIn::XxStdIn (void)
{
    fcntl (STDIN_FILENO, F_SETFL, O_NONBLOCK);
    SetFd (STDIN_FILENO);
    SetStatus (StatOpen);
};

XxStdIn::~XxStdIn (void)
{
};

XxStdOut::XxStdOut (void)
{
    fcntl (STDOUT_FILENO, F_SETFL, O_NONBLOCK);
    SetFd (STDOUT_FILENO);
    SetStatus (StatOpen);
};

XxStdOut::~XxStdOut (void)
{
};

int XxStdOut::GetWriteChunkSize (EzString Data)
{
    return Data.Length ();
};

void XxStdOut::GetRWFlags (int &rFlag, int &wFlag)
{
    XxStream::GetRWFlags (rFlag, wFlag);
    rFlag = 0;
};

