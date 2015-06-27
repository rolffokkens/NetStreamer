/*
 * file: NrGraphicAnalyzer.h
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_GRAPHIC_ANALYZER
#define H_XX_GRAPHIC_ANALYZER

#include "XxVirtual.h"
#include "XxMaxMeter.h"
#include "NrAnalyzer.h"

class NrGraphicAnalyzer : public XxVirtual  {
private:
    NrAnalyzer *pAnalyzer;
    XxMaxMeter **BandMeters;
    int        NrBands;
protected:
public:
    NrGraphicAnalyzer
        ( EzString Name
        , XxDrawable *pParent
        , NrAnalyzer *pAnalyzer
        , int XOffset, int YOffset
        , int NrBands
        , int BandWidth, int BandSpace
        , int Height
        );

    virtual ~NrGraphicAnalyzer (void);

    virtual void Refresh (int MsCurTime);
};

#endif
