/*
 * file: Nr8Log.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "Nr8Log.h"

static int InitLogTab[] = {
  32768
, 35734
, 38968
, 42495
, 46341
, 50535
, 55109
, 60097
};

static unsigned char *LogTab;

static void InitLog(void)
{
    int           *pIniTab;
    unsigned char *pLogTab;
    int           Max, Min, i;
    int           LogVal, CmpVal;

    LogTab  = new unsigned char [65536];
    pIniTab = InitLogTab + 7;
    Min     = 65536;
    LogVal  = 128;

    for (;;) {
        Max        = Min;
        Min        = *pIniTab;
        *pIniTab-- = Min >> 1;

        if (pIniTab < InitLogTab) pIniTab = InitLogTab + 7;

        for (i = Min; i < Max; i++) LogTab[i] = LogVal;

        LogVal--;

        if (Min == 0) break;
    };
};

class InitializerClass {
    int x;
public:
    InitializerClass (void) {
        InitLog ();
    };
}; 

static InitializerClass Dummy;

int Get8Log (int Val)
{
    return LogTab [Val];
};
