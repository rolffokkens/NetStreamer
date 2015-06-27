/*
 * file: NrTransFiles.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <dirent.h>
#include <fcntl.h>

#include "NrTransFiles.h"

using namespace std;

static int SelectFile (const struct dirent *pDirEnt)
{
    const char *cp = pDirEnt->d_name;
    int         len;

    len = strlen (cp);

    if (len < 5) return 0;

    cp = cp + len - 5;

    return (strcmp (cp, ".tape") == 0);
};

static int ScanDir (EzString Dir, EzString *&pFiles, int &nFiles)
{
    struct dirent **namelist;
    int           i;

    nFiles = scandir ( (const char *)Dir
                     , &namelist
                     , SelectFile
                     , alphasort
                     );
    if (nFiles == -1) {
        nFiles = 0;
        return 0;
    };

    pFiles = new EzString [nFiles];

    for (i = 0; i < nFiles; i++) {
        pFiles[i] = namelist[i]->d_name;
        free (namelist[i]);
    };

    free (namelist);

    return 1;
};

static int GetTimerDelay (int SampleRate)
{
    return ((1024 * 1000) / RateHerz [SampleRate]);
};

NrTransFiles::NrTransFiles
    ( NrTransConnection *pConnection, EzString FilesDir, char SampleRate
    )
    : NrTransSoundSource (pConnection)
    , Timer (this, GetTimerDelay (SampleRate))
{
    NrTransFiles::AvailToRead = 0;
    NrTransFiles::nCurFiles   = 0;
    NrTransFiles::lCurFiles   = 0;
    NrTransFiles::nPrvFiles   = 0;
    NrTransFiles::pCurFiles   = NULL;
    NrTransFiles::pPrvFiles   = NULL;
    NrTransFiles::FilesDir    = FilesDir;
    NrTransFiles::SampleRate  = SampleRate;
};

NrTransFiles::~NrTransFiles (void)
{
    if (pCurFiles != NULL) delete [] pCurFiles;
    if (pPrvFiles != NULL) delete [] pPrvFiles;
};

void NrTransFiles::OpenNext (void)
{
    int      Fd, Rnd, Retry;
    EzString SelFile, FileName;

    SetFd (-1);
    SetStatus (StatClosed);

    for (;;) {
        if (lCurFiles == 0) {
            if (pPrvFiles != NULL) {
                delete [] pPrvFiles;
            };
            pPrvFiles = pCurFiles;
            nPrvFiles = nCurFiles >> 1;

            if (!ScanDir (FilesDir, pCurFiles, nCurFiles)) {
                cerr << "Couldn't scan directory " << FilesDir << endl;
                HandleError ();
                return;
            };
            if (nCurFiles == 0) {
                cerr << "No .tape files found in " << FilesDir << endl;
                HandleError ();
                return;
            };
            lCurFiles = nCurFiles;
        };

        do {
            Rnd = (lCurFiles * (rand () & 0x0fff)) >> 12;
            FileName = pCurFiles [Rnd];
            Retry = 0;

            for (int i = 0; i < nPrvFiles; i++) {
                if (FileName == pPrvFiles [i]) {
                    Retry = 1;
                    break;
                };
            };
        } while (Retry);

        if (nPrvFiles > 0) nPrvFiles--;

        pCurFiles [Rnd]           = pCurFiles [lCurFiles - 1];
        pCurFiles [lCurFiles - 1] = FileName;
        SelFile  = FilesDir + EzString ("/") + FileName;
        lCurFiles--;

        LogMessage (EzString ("Opening ") + SelFile);

        Fd = open (SelFile, O_RDONLY);

        if (Fd >= 0) {
            SetFd (Fd);
            SetStatus (StatOpen);
            FileName = Substr (FileName, 0, FileName.Length () - 5);
            HandleInfo (FileName);
            break;
        };
        cerr << "Unable to open " << SelFile << endl;
    };
};

EzString NrTransFiles::ProcessReadData (EzString Data) 
{
    AvailToRead -= Data.Length ();

    return NrTransSoundSource::ProcessReadData (Data);
};

int NrTransFiles::GetReadChunkSize  (void)
{
    return 384;
};          

void NrTransFiles::OnDisconnect (void)
{
    Close ();
    OpenNext ();
};

template <>
void NrTransFiles::TransFilesTimer::HandleTimeOut (int Count)
{
    pOwner->HandleTimeOut (Count);
};

void NrTransFiles::HandleTimeOut (int Count)
{
    AvailToRead += 384 * Count;
};

void NrTransFiles::GetRWFlags (int &rFlag, int &wFlag)
{
    rFlag = (AvailToRead > 0);
    wFlag = 0;
};

NrTransConnFiles::NrTransConnFiles
    ( EzString AddrPort
    , int Freq, EzString Description
    , EzString AddInfo
    , EzString FilesDir
    , char SampleRate
    )
    : NrTransConnection
         ( AddrPort, Freq, Description
         , AddInfo
         , new NrTransFiles (this, FilesDir, SampleRate)
         , SampleRate
         )
{
};

NrTransConnFiles::~NrTransConnFiles (void)
{
};

void NrTransFiles::Start (void)
{
    OpenNext ();
};

