/*
 * file: XxSerial.cpp
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "XxSerial.h"

#define SERIAL "/dev/cua1"

XxSerial::XxSerial (void)
{
    Speed          = 2400;
    nBits          = 8;
    ParEnable      = 0;
    ParEven        = 0;
};

XxSerial::~XxSerial (void)
{
};

int XxSerial::Open (int Speed, int nBits, int ParEnable, int ParEven)
{
    int     Fd, CSize, CParity;
    termios TermIos;

    Close ();

    switch (nBits) {
    case 5:
        CSize = CS5;
        break;
    case 6:
        CSize = CS6;
        break;
    case 7:
        CSize = CS7;
        break;
    case 8:
    default:
        CSize = CS8;
        nBits = 8;
        break;
    };

    if (ParEnable) {
       CParity = (ParEven ? 0 : PARODD) | PARENB;
    } else {
       CParity = 0;
    };

    XxSerial::Speed     = Speed;
    XxSerial::nBits     = nBits;
    XxSerial::ParEnable = ParEnable;
    XxSerial::ParEven   = ParEven;

    Fd = open (SERIAL, O_RDWR | O_NONBLOCK, 0);

    if (Fd == -1) return 0;

    tcgetattr (Fd, &TermIos);

    TermIos.c_cflag &= ~(CSIZE | CSTOPB  | PARENB | CLOCAL);
    TermIos.c_cflag |= ~(CSize | CParity | CREAD  | HUPCL );

    TermIos.c_iflag = IGNBRK | IGNPAR;
    TermIos.c_oflag = 0;
    TermIos.c_lflag = 0;

    cfsetospeed (&TermIos, Speed);
    cfsetispeed (&TermIos, Speed);

    tcsetattr (Fd, TCSANOW, &TermIos);

    SetFd (Fd);
    SetStatus (StatOpen);

    return 1;
};

int XxSerial::GetWriteChunkSize (EzString Data)
{
    return Data.Length ();
};
