/*
 * file: NrRecMain.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <malloc.h>
#include <iostream>

#include "XxSocket.h"
#include "NrReceiver.h"
#include "XxSoundDev.h"

using namespace std;

static void ShowArgErrorMessage (EzString ProgName)
{
    cerr
    << "Bad parameters\n"
    << "Usage:" << endl
    << "    " << ProgName << " Freq ServerAddr\n"
    << "Where:\n"
    << "    Freq       : a Frequency between 880 an 1080\n"
    << "                 e.g. 918 for 91.8 MHz\n"
    << "    ServerAddr : IpAddr:Port of a server\n"
    << "                 e.g. flits102-126.flits.rug.nl:8888\n";
};

int main (int argc, char *argv[])
{
    NrRecConnection    *pTransConnection;
    EzString            Freq, AddrPort;
    int                 RetVal;

    if (argc < 3) {
        ShowArgErrorMessage (argv[0]);
        return 1;
    };

    Freq        = argv [1];
    AddrPort    = argv [2];

    pTransConnection = new NrRecConnection
                           ( Freq, 1
                           , EzString ("NrReceiver ") + EzString (NR_VERSION)
                           );

    if (!pTransConnection->Connect (AddrPort)) {
        cerr << "Cannot connect to " << AddrPort << endl;
        delete pTransConnection;
        return 1;
    };
    if (pTransConnection->SetMute (0)) {
        cerr << "Cannot open sound device " << endl;
        delete pTransConnection;
        return 1;
    };

    XxTimer::RefreshMsCurTime ();

    RetVal = XxBlocker::MainLoop ();

    return RetVal;
};
