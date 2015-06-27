/*
 * file: NrTransFiles.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_NR_TRANSFILES
#define H_NR_TRANSFILES

#include "BiLink.h"
#include "EzString.h"
#include "XxTimer.h"
#include "XxSocket.h"
#include "NrNetRadio.h"
#include "NrComp.h"

#include "NrTransmitter.h"

class NrTransFiles : public NrTransSoundSource {
    typedef tXxTimer<NrTransFiles> TransFilesTimer;

    friend class tXxTimer<NrTransFiles>;
private:
    int             AvailToRead;
    EzString        FilesDir;
    EzString       *pCurFiles;
    int             nCurFiles;
    int             lCurFiles;
    EzString       *pPrvFiles;
    int             nPrvFiles;
    TransFilesTimer Timer;
    char            SampleRate;

    void HandleTimeOut (int Count);
    void OpenNext (void);
protected:
    virtual EzString ProcessReadData  (EzString Data);

    virtual int GetReadChunkSize  (void);

    virtual void OnDisconnect (void);
public:
    NrTransFiles
        ( NrTransConnection *pConnection, EzString FilesDir, char SampleRate);
    virtual ~NrTransFiles (void);

    virtual void GetRWFlags (int &rFlag, int &wFlag);

    virtual void Start (void);
};

class NrTransConnFiles : public NrTransConnection {
public:
    NrTransConnFiles
        ( EzString AddrPort, int Freq, EzString Description
        , EzString AddInfo, EzString FilesDir, char SampleRate
        );
    ~NrTransConnFiles (void);
};

#endif
