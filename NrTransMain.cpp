/*
 * file: NrTransMain.cpp
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
#include "NrTransmitter.h"
#include "NrTransSoundDev.h"
#include "NrTransFiles.h"
#include "NrTransStdIn.h"
#include "XxSoundDevOSS.h"

using namespace std;

static void ShowArgErrorMessage (EzString ProgName)
{
    cerr
    << "Bad parameters\n"
    << "Usage:" << endl
    << "    " << ProgName
    << " Mode SampleRate Freq Station ServerAddr [Dir | InRate]\n"
    << "Where:\n"
    << "    Mode       : \"Device\" or \"Directory\" or \"StdIn\"\n"
    << "    SampleRate : 8 or 16 (kHz)\n"
    << "    Freq       : a Frequency between 880 an 1080\n"
    << "                 e.g. 918 for 91.8 MHz\n"
    << "    Station    : Name for the station\n"
    << "                 e.g. \"Radio 123\"\n"
    << "    ServerAddr : IpAddr:Port of a server\n"
    << "                 e.g. flits102-126.flits.rug.nl:8888\n"
    << "    Dir        : (Optional) A Directory\n"
    << "                 Only when mode = \"Directory\"\n"
    << "                 e.g. /usr/tapes\n"
    << "    InRate     : (Optional) The Input SampleRate\n"
    << "                 Only when mode = \"StdIn\"\n"
    << "                 e.g. 44\n";
};

enum {ModeDirectory, ModeDevice, ModeStdIn};

int main (int argc, char *argv[])
{
    EzString            Freq, Description, AddrPort, TapeDir, SampleRate;
    EzString            InSampleRate, Driver, Device;
    NrTransConnection  *pTransConnection;
    EzString            Mode;
    int                 ModeCode;
    int                 ModeArgs;
    int                 SampleRateIdx;
    EzString            AddInfo;

    if (argc < 6) {
        ShowArgErrorMessage (argv[0]);
        return 1;
    };
    Mode = Upper (EzString (argv[1]));

    if (Mode == EzString ("DEVICE")) {
        ModeCode = ModeDevice;
        ModeArgs = 6;
    } else
    if (Mode == EzString ("DIRECTORY")) {
        ModeCode = ModeDirectory;
        ModeArgs = 7;
    } else
    if (Mode == EzString ("STDIN")) {
        ModeCode = ModeStdIn;
        ModeArgs = 7;
    } else {
        ShowArgErrorMessage (argv[0]);
        return 1;
    };

    if (argc != ModeArgs) {
        ShowArgErrorMessage (argv[0]);
        return 1;
    };

    SampleRate   = argv [2];
    Freq         = argv [3];
    Description  = argv [4];
    AddrPort     = argv [5];
    TapeDir      = (ModeCode == ModeDirectory ? argv[6] : "");
    InSampleRate = (ModeCode == ModeStdIn     ? argv[6] : "");
    Driver       = (argc < 8 ? "Oss" : argv[7]);
    Device       = (argc < 9 ? "/dev/dsp" : argv[8]);

    SampleRateIdx = -1;

    if (SampleRate == EzString ("8")) {
        SampleRateIdx = 0;
    };
    if (SampleRate == EzString ("16")) {
        SampleRateIdx = 1;
    };
    if (SampleRateIdx == -1) {
        ShowArgErrorMessage (argv[0]);
        return 1;
    };

    AddInfo = EzString ("NrTransmitter ") + EzString (NR_VERSION);

    switch (ModeCode) {
    case ModeDirectory:
        pTransConnection
            = new NrTransConnFiles
                  ( AddrPort, Freq, Description, AddInfo
                  , TapeDir, SampleRateIdx
                  );
        break;
    case ModeDevice:
        pTransConnection
            = new NrTransConnSoundDev
                  ( AddrPort, Freq, Description, AddInfo
                  , SampleRateIdx, Device
                  );
        break;
    case ModeStdIn:
        pTransConnection
            = new NrTransConnStdIn
                  ( AddrPort, Freq, Description, AddInfo
                  , SampleRateIdx, InSampleRate
                  );
        break;
    default:
        return 1; // To get rid of gcc warning
    };
    pTransConnection->Start ();

    XxBlocker::MainLoop ();

    return 0;
};
