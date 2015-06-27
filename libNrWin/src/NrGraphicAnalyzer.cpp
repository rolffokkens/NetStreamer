/*
 * file: NrGraphicAnalyzer.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "NrGraphicAnalyzer.h"
#include "Nr8Log.h"

static int CalcWidth (int NrBands, int BandWidth, int BandSpace)
{
    return (NrBands * (BandWidth + BandSpace)) - BandSpace;
};

NrGraphicAnalyzer::NrGraphicAnalyzer
    ( EzString Name
    , XxDrawable *pParent
    , NrAnalyzer *pAnalyzer
    , int XOffset, int YOffset
    , int NrBands
    , int BandWidth, int BandSpace
    , int Height
    )
    : XxVirtual
        ( Name, pParent, XOffset, YOffset
        , CalcWidth (NrBands, BandWidth, BandSpace), Height
        )
{
    int      i, l1, l2;
    XxPixmap OnMap  (Name + EzString ("_OnMap"),  BandWidth, Height);
    XxPixmap OffMap (Name + EzString ("_OffMap"), BandWidth, Height);

    l2 = Height / 2;
    l1 = l2 / 2;

    DrawGC.SetForeground (XxGreen4);
    OnMap.FillRectangle (DrawGC, 0, l2, BandWidth, (Height - l2));
    DrawGC.SetForeground (XxYellow4);
    OnMap.FillRectangle (DrawGC, 0, l1, BandWidth, (l2 - l1));
    DrawGC.SetForeground (XxRed4);
    OnMap.FillRectangle (DrawGC, 0, 0,  BandWidth, l1);

    DrawGC.SetForeground (XxGreen1);
    OffMap.FillRectangle (DrawGC, 0, l2, BandWidth, (Height - l2));
    DrawGC.SetForeground (XxYellow1);
    OffMap.FillRectangle (DrawGC, 0, l1, BandWidth, (l2 - l1));
    DrawGC.SetForeground (XxRed1);
    OffMap.FillRectangle (DrawGC, 0, 0,  BandWidth, l1);

    for (i = 0; i < Height; i += 3) {
        DrawGC.SetForeground (XxBlack);
        OnMap.DrawLine  (DrawGC, 0, i, BandWidth, i);
        OffMap.DrawLine (DrawGC, 0, i, BandWidth, i);
    };

    NrGraphicAnalyzer::NrBands   = NrBands;
    NrGraphicAnalyzer::pAnalyzer = pAnalyzer;

    BandMeters = new (XxMaxMeter *) [NrBands];

    for (i = 0; i < NrBands; i++) {
        BandMeters[i]
            = new XxMaxMeter
                  ( Name + EzString ("_MaxMeter_") + EzString (i)
                  , this
                  , i * (BandWidth + BandSpace), 0
                  , BandWidth, Height
                  , 70, 100
                  , 2
                  , 3 						// StepSize
                  );
        BandMeters[i]->SetOffPixmap (OffMap);
        BandMeters[i]->SetOnPixmap  (OnMap);
    };
};

NrGraphicAnalyzer::~NrGraphicAnalyzer (void)
{
    int i;

    for (i = 0; i < NrBands; i++) {
        delete BandMeters[i];
    };

    delete [] BandMeters;
};

void NrGraphicAnalyzer::Refresh (int MsCurTime)
{
    int i, Value;

    for (i = 0; i < NrBands; i++) {
        Value = pAnalyzer->GetMaxBandLevel (i + 1, MsCurTime);

        BandMeters[i]->SetCurVal (Get8Log (Value));
    };
};
