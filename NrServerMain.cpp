/*
 * file: NrServerMain.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <malloc.h>
#include <iostream.h>

#include "XxSocket.h"
#include "NrServer.h"

static void ShowArgErrorMessage (EzString ProgName)
{
    cerr
    << "Bad parameters\n"
    << "Usage:" << endl
    << "    " << ProgName << " ServerAddr\n"
    << "Where:\n"
    << "    ServerAddr : [IpAddr]:Port of a server\n"
    << "                 e.g. :8888\n";
};

static void CreateListener (EzString AddrPort)
{
    NrServerListener *pListener;

    pListener = new NrServerListener;

    if (!pListener->Listen (AddrPort)) {
        cerr << "Cannot listen on " << AddrPort << endl;
        delete pListener;
    };
};

int main (int argc, char *argv[])
{
/*
    NrServerListener TestListener;
    XxConnection *pConnection;
    XxBlocker *pBlocker;
    XxBlocker::XxAction Action;
    struct mallinfo MallInfo;
    int mSec;
*/
    NrServerListener *pListener;
    EzString          AddrPort;
    int               i;

    for (i = 1; i < (argc > 1 ? argc : 2); i++) {
        AddrPort = (i < argc ? argv [i] : ":8888");

        CreateListener (AddrPort);
    };

    XxBlocker::MainLoop ();

    return 0;
};
