/*
 * file: NrComp.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_COMP

#define H_COMP

#include "EzString.h"
#include "g72x.h"

class NrComp {
private:
    EzString Buf;

    int nBits;
    int (*EncRoutine)(int, int, struct g72x_state *);
        
    struct g72x_state State;
    unsigned int      OutBuffer;
    int               OutBits;

    int Pack (unsigned Code, char *&pOut);
public:
    NrComp (int nBits);

    virtual ~NrComp (void) { };

    void Reset (void);

    EzString Compress (EzString Sound);
};

#endif
