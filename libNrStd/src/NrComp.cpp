/*
 * file: NrComp.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <stdint.h>
#include <iostream>

#include "NrComp.h"

inline void AdjustAvg (int &Avg, int New, int Weight)
{
    Avg = ((Avg << Weight) - Avg + New ) >> Weight;
};

NrComp::NrComp (int nBits)
{
    NrComp::nBits     = nBits;
    NrComp::OutBits   = 0;
    NrComp::OutBuffer = 0;

    switch (nBits) {
    case 3:
        EncRoutine = g723_24_encoder;
        break;
    case 5:
        EncRoutine = g723_40_encoder;
        break;
    default:
        EncRoutine = g721_encoder;
        break;
    };

    Reset ();
};

void NrComp::Reset (void)
{
    g72x_init_state (&State);
};

int NrComp::Pack (unsigned Code, char *&pOut)
{
        unsigned char OutByte;

        OutBuffer |= (Code << OutBits);
        OutBits   += nBits;
        if (OutBits >= 8) {
                OutByte     = OutBuffer & 0xff;
                OutBits    -= 8;
                OutBuffer >>= 8;

                *pOut++ = OutByte;
        }
        return (OutBits > 0);
}


EzString NrComp::Compress (EzString Sound)
{
    const char     *pSound;
    char           *pOutBuf, *pOut;
    EzString         RetVal;
    unsigned char  Code;
    int            InLen;
    short          Sample;
    int            Residu;
    short          *pEven;
    const short    *pIn, *pInLast;

    InLen   = Sound.Length () >> 1;
    pSound  = Sound.Text ();

    if ((uint64_t (pSound) & (sizeof (Sample) - 1)) != 0) {
        pEven = new short[(InLen >> 1) + 1];
        memcpy (pEven, pSound, InLen);
        pIn = pEven;
    } else {
        pEven = NULL;
        pIn   = (short *)pSound;
    };
    pInLast = pIn + InLen;

    pOutBuf = new char [1 + (InLen * nBits) / 4];
    pOut    = pOutBuf;
    Residu  = 0;

    NrComp::OutBits   = 0;
    NrComp::OutBuffer = 0;

    while (pIn < pInLast) {
        Sample = (*pIn++ >> 1);

        Code = EncRoutine (Sample, AUDIO_ENCODING_LINEAR, &State);

        Residu = Pack (Code, pOut);
    };
    while (Residu) Residu = Pack (0, pOut);

    RetVal  = EzString (pOutBuf, pOut - pOutBuf);

    delete pOutBuf;

    if (pEven != NULL) delete pEven;

    return RetVal;
};
