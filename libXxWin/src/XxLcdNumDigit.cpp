/*
 * file: XxLcdNumDigit.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include "XxLcdNumDigit.h"
#include "XxBitmap.h"
#include "XxPixmap.h"

#include "XxLcdNumDigit0.xbm"
#include "XxLcdNumDigit1.xbm"
#include "XxLcdNumDigit2.xbm"
#include "XxLcdNumDigit3.xbm"
#include "XxLcdNumDigit4.xbm"
#include "XxLcdNumDigit5.xbm"
#include "XxLcdNumDigit6.xbm"
#include "XxLcdNumDigit7.xbm"
#include "XxLcdNumDigit8.xbm"
#include "XxLcdNumDigit9.xbm"

#include "XxLcdNumDot.xbm"

struct BitmapStruct {
    int Width, Height;
    char *Data;
};

static BitmapStruct BitMapData [] = {
    {XxLcdNumDigit0_width, XxLcdNumDigit0_height, XxLcdNumDigit0_bits}
,   {XxLcdNumDigit1_width, XxLcdNumDigit1_height, XxLcdNumDigit1_bits}
,   {XxLcdNumDigit2_width, XxLcdNumDigit2_height, XxLcdNumDigit2_bits}
,   {XxLcdNumDigit3_width, XxLcdNumDigit3_height, XxLcdNumDigit3_bits}
,   {XxLcdNumDigit4_width, XxLcdNumDigit4_height, XxLcdNumDigit4_bits}
,   {XxLcdNumDigit5_width, XxLcdNumDigit5_height, XxLcdNumDigit5_bits}
,   {XxLcdNumDigit6_width, XxLcdNumDigit6_height, XxLcdNumDigit6_bits}
,   {XxLcdNumDigit7_width, XxLcdNumDigit7_height, XxLcdNumDigit7_bits}
,   {XxLcdNumDigit8_width, XxLcdNumDigit8_height, XxLcdNumDigit8_bits}
,   {XxLcdNumDigit9_width, XxLcdNumDigit9_height, XxLcdNumDigit9_bits}
};

static int DigitCount = 0;

static XxPixmap *pTempPixmap;

static XxBitmap *DigitBitmaps[10];
static XxBitmap *DotBitmap;

static void InitDigitBitmaps (void)
{
    int i;
    BitmapStruct *pBD;

    pTempPixmap = new XxPixmap
        ("XxLcdNumDigitTemp", XxLcdNumDigit8_width, XxLcdNumDigit8_height);

    for (i = 0, pBD = BitMapData; i < 10; i++, pBD++) {
        DigitBitmaps [i]
            = new XxBitmap
                  ( EzString ("NumDigitMaps_") + EzString (i)
                  , pBD->Width, pBD->Height, pBD->Data
                  );
    };
    DotBitmap = new XxBitmap
                    ( "DotMap"
                    , XxLcdNumDot_width, XxLcdNumDot_height, XxLcdNumDot_bits
                    );
};

XxLcdNumDigit::XxLcdNumDigit
    ( EzString Name
    , XxDrawable *pParent
    , int XOffset, int YOffset
    , XxColor OnColor, XxColor OffColor, XxColor BackColor
    )
    : XxVirtual (Name, pParent, XOffset, YOffset, 18, 21)
{
    if (DigitCount++ == 0) InitDigitBitmaps ();

    XxLcdNumDigit::DotVal     = 0;
    XxLcdNumDigit::DigitVal   = -1;
    XxLcdNumDigit::OnColor    = OnColor;
    XxLcdNumDigit::OffColor   = OffColor;
    XxLcdNumDigit::BackColor  = BackColor;

    DrawDigit (0, 0);

#   ifdef CRE_TRACE
    printf ("Create XxLcdNumDigit %d\n", Pm);
#   endif

};

XxLcdNumDigit::~XxLcdNumDigit (void)
{
    int i;

    if (--DigitCount != 0) return;

    for (i = 0; i < 10; i++) {
        delete DigitBitmaps [i];
    };

    delete DotBitmap;

    delete pTempPixmap;
};

void XxLcdNumDigit::DrawDigit (int DigitVal, int DotVal)
{
    if (   XxLcdNumDigit::DigitVal == DigitVal
        && XxLcdNumDigit::DotVal   == DotVal  ) return;

    XxLcdNumDigit::DigitVal = DigitVal;
    XxLcdNumDigit::DotVal   = DotVal;

    DrawGC.SetForeground (BackColor);
    pTempPixmap->FillRectangle (DrawGC, 0, 0, GetWidth (), GetHeight ());

    DrawGC.SetForeground (OffColor);
    DrawGC.SetClipMask   (0, 0, DigitBitmaps [8]);
    pTempPixmap->FillRectangle (DrawGC, 0, 0, GetWidth (), GetHeight ());

    if (DigitVal != -1) {
        DrawGC.SetForeground (OnColor);
        DrawGC.SetClipMask   (0, 0, DigitBitmaps [DigitVal]);
        pTempPixmap->FillRectangle (DrawGC, 0, 0, GetWidth (), GetHeight ());
    };

    if (!DotVal ) DrawGC.SetForeground (OffColor);
    DrawGC.SetClipMask   (14, 18, DotBitmap);
    pTempPixmap->FillRectangle (DrawGC, 0, 0, GetWidth (), GetHeight ());

    DrawGC.SetClipMask   (0, 0, NULL);

    CopyArea (DrawGC, pTempPixmap, 0, 0, GetWidth (), GetHeight (), 0, 0);
};

void XxLcdNumDigit::SetVal (int Val)
{
    if (Val < 0) Val = -1;
    if (Val > 9) Val = -1;

    DrawDigit (Val, DotVal);
};

void XxLcdNumDigit::SetDot (int Val)
{
    Val = (Val != 0);

    DrawDigit (DigitVal, Val);
};

