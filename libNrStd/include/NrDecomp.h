/*
 * file: NrDecomp.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_DECOMP

#define H_DECOMP

#include "EzString.h"
#include "g72x.h"

class NrDecomp {
private:
    EzString Buf;

    int nBits;
    int (*DecRoutine)(int, int, struct g72x_state *);
        
    struct g72x_state State;
    unsigned int      InBuffer;
    int               InBits;

    unsigned char Unpack (const char *&pIn);
public:
    NrDecomp (int nBits);

    virtual ~NrDecomp (void) { };

    void Reset (void);
    EzString Decompress (EzString Sound, int OutSize);
};

#endif
