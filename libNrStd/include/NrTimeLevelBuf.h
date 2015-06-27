/*
 * file: NrTimeLevelBuf.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_NR_TIMELEVELBUF
#define H_NR_TIMELEVELBUF

class NrTimeRec;

class NrTimeLevelBuf {
private:
    NrTimeRec *pHist, *pLastHist, *pCurHist, *pPrvHist;
    int       HistSize;
public:
    NrTimeLevelBuf  (int HistSize);

    ~NrTimeLevelBuf (void);

    void RegisterValue (int MsTimeStamp, int Value);

    int LookupValue (int &MsTimeStamp);
};

#endif
