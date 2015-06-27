/*
 * file: NrDecomp.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "NrDecomp.h"

inline void AdjustAvg (int &Avg, int New, int Weight)
{
    Avg = ((Avg << Weight) - Avg + New ) >> Weight;
};

NrDecomp::NrDecomp (int nBits)
{
    NrDecomp::nBits     = nBits;
    NrDecomp::InBits    = 0;
    NrDecomp::InBuffer  = 0;

    switch (nBits) {
    case 3:
        DecRoutine = g723_24_decoder;
        break;
    case 5:
        DecRoutine = g723_40_decoder;
        break;
    default:
        DecRoutine = g721_decoder;
        break;
    };
    Reset ();
};

void NrDecomp::Reset (void)
{
    g72x_init_state (&State);
};

unsigned char NrDecomp::Unpack (const char *&pIn)
{
    unsigned char Byte, RetVal;

    if (InBits < nBits) {
        Byte      = *pIn++;
        InBuffer |= (Byte << InBits);
        InBits   += 8;
    };
    RetVal     = InBuffer & ((1 << nBits) - 1);
    InBuffer >>= nBits;
    InBits    -= nBits;

    return RetVal;
}

EzString NrDecomp::Decompress (EzString Sound, int OutSize)
{
    const char     *pIn, *pInLast;
    short          *pOutBuf, *pOut, *pOutLast;
    EzString       RetVal;
    unsigned char  Code;
    int            InLen;

    InLen    = Sound.Length ();
    pIn      = Sound.Text ();
    pInLast  = pIn + InLen + 1;

    OutSize /= sizeof (short);

    pOutBuf  = new short [2 + OutSize];
    pOut     = pOutBuf;
    pOutLast = pOut + OutSize;

    NrDecomp::InBits   = 0;
    NrDecomp::InBuffer = 0;

    while (pIn < pInLast && pOut < pOutLast) {
        Code    = Unpack (pIn);
        *pOut++ = DecRoutine (Code, AUDIO_ENCODING_LINEAR, &State); //  << 1;
    };

    RetVal  = EzString ((char *)pOutBuf, (char *)pOut - (char *)pOutBuf);

    delete pOutBuf;

    return RetVal;
};
