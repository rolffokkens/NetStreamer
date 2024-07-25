/*
 * file: XxLcdAlphaDigit.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 * Rel  When   who  What
 * 0.9  980104 Rolf Removed bug in XxLcdNumDigit: bad arraysize for DigitBitmaps
 * 0.13 980816 Rolf Added Ascii field in BitmapStruct for more flexibility
 */

#include "XxLcdAlphaDigit.h"
#include "XxBitmap.h"
#include "XxPixmap.h"

#include "XxLcdAlphaDigitBack.xbm"
#include "XxLcdAlphaDigit0x20.xbm" // " " 
#include "XxLcdAlphaDigit0x28.xbm" // "(" 
#include "XxLcdAlphaDigit0x29.xbm" // ")" 
#include "XxLcdAlphaDigit0x2d.xbm" // "-" 
#include "XxLcdAlphaDigit0.xbm"
#include "XxLcdAlphaDigit1.xbm"
#include "XxLcdAlphaDigit2.xbm"
#include "XxLcdAlphaDigit3.xbm"
#include "XxLcdAlphaDigit4.xbm"
#include "XxLcdAlphaDigit5.xbm"
#include "XxLcdAlphaDigit6.xbm"
#include "XxLcdAlphaDigit7.xbm"
#include "XxLcdAlphaDigit8.xbm"
#include "XxLcdAlphaDigit9.xbm"
#include "XxLcdAlphaDigit0x3a.xbm" // ":" 
#include "XxLcdAlphaDigitA.xbm"
#include "XxLcdAlphaDigitB.xbm"
#include "XxLcdAlphaDigitC.xbm"
#include "XxLcdAlphaDigitD.xbm"
#include "XxLcdAlphaDigitE.xbm"
#include "XxLcdAlphaDigitF.xbm"
#include "XxLcdAlphaDigitG.xbm"
#include "XxLcdAlphaDigitH.xbm"
#include "XxLcdAlphaDigitI.xbm"
#include "XxLcdAlphaDigitJ.xbm"
#include "XxLcdAlphaDigitK.xbm"
#include "XxLcdAlphaDigitL.xbm"
#include "XxLcdAlphaDigitM.xbm"
#include "XxLcdAlphaDigitN.xbm"
#include "XxLcdAlphaDigitO.xbm"
#include "XxLcdAlphaDigitP.xbm"
#include "XxLcdAlphaDigitQ.xbm"
#include "XxLcdAlphaDigitR.xbm"
#include "XxLcdAlphaDigitS.xbm"
#include "XxLcdAlphaDigitT.xbm"
#include "XxLcdAlphaDigitU.xbm"
#include "XxLcdAlphaDigitV.xbm"
#include "XxLcdAlphaDigitW.xbm"
#include "XxLcdAlphaDigitX.xbm"
#include "XxLcdAlphaDigitY.xbm"
#include "XxLcdAlphaDigitZ.xbm"
#include "XxLcdAlphaDigita.xbm"
#include "XxLcdAlphaDigitb.xbm"
#include "XxLcdAlphaDigitc.xbm"
#include "XxLcdAlphaDigitd.xbm"
#include "XxLcdAlphaDigite.xbm"
#include "XxLcdAlphaDigitf.xbm"
#include "XxLcdAlphaDigitg.xbm"
#include "XxLcdAlphaDigith.xbm"
#include "XxLcdAlphaDigiti.xbm"
#include "XxLcdAlphaDigitj.xbm"
#include "XxLcdAlphaDigitk.xbm"
#include "XxLcdAlphaDigitl.xbm"
#include "XxLcdAlphaDigitm.xbm"
#include "XxLcdAlphaDigitn.xbm"
#include "XxLcdAlphaDigito.xbm"
#include "XxLcdAlphaDigitp.xbm"
#include "XxLcdAlphaDigitq.xbm"
#include "XxLcdAlphaDigitr.xbm"
#include "XxLcdAlphaDigits.xbm"
#include "XxLcdAlphaDigitt.xbm"
#include "XxLcdAlphaDigitu.xbm"
#include "XxLcdAlphaDigitv.xbm"
#include "XxLcdAlphaDigitw.xbm"
#include "XxLcdAlphaDigitx.xbm"
#include "XxLcdAlphaDigity.xbm"
#include "XxLcdAlphaDigitz.xbm"

#include "XxLcdNumDot.xbm"

struct BitmapStruct {
    short Ascii;
    int   Width, Height;
    unsigned char *Data;
};

#define BMStruct(A,X) \
{A, XxLcdAlphaDigit ## X ## _width,  XxLcdAlphaDigit ## X ## _height, \
    XxLcdAlphaDigit ## X ## _bits}

static BitmapStruct BitMapData [] = {
    BMStruct (0, Back)
,   BMStruct (' ', 0x20), BMStruct ('-', 0x2d)
,   BMStruct ('(', 0x28), BMStruct (')', 0x29)
,   BMStruct ('0', 0), BMStruct ('1', 1), BMStruct ('2', 2)
,   BMStruct ('3', 3), BMStruct ('4', 4), BMStruct ('5', 5)
,   BMStruct ('6', 6), BMStruct ('7', 7), BMStruct ('8', 8)
,   BMStruct ('9', 9)
,   BMStruct (':', 0x3a)
,   BMStruct ('A', A), BMStruct ('B', B), BMStruct ('C', C)
,   BMStruct ('D', D), BMStruct ('E', E), BMStruct ('F', F)
,   BMStruct ('G', G), BMStruct ('H', H), BMStruct ('I', I)
,   BMStruct ('J', J), BMStruct ('K', K), BMStruct ('L', L)
,   BMStruct ('M', M), BMStruct ('N', N), BMStruct ('O', O)
,   BMStruct ('P', P), BMStruct ('Q', Q), BMStruct ('R', R)
,   BMStruct ('S', S), BMStruct ('T', T), BMStruct ('U', U)
,   BMStruct ('V', V), BMStruct ('W', W), BMStruct ('X', X)
,   BMStruct ('Y', Y), BMStruct ('Z', Z)
,   BMStruct ('a', a), BMStruct ('b', b), BMStruct ('c', c)
,   BMStruct ('d', d), BMStruct ('e', e), BMStruct ('f', f)
,   BMStruct ('g', g), BMStruct ('h', h), BMStruct ('i', i)
,   BMStruct ('j', j), BMStruct ('k', k), BMStruct ('l', l)
,   BMStruct ('m', m), BMStruct ('n', n), BMStruct ('o', o)
,   BMStruct ('p', p), BMStruct ('q', q), BMStruct ('r', r)
,   BMStruct ('s', s), BMStruct ('t', t), BMStruct ('u', u)
,   BMStruct ('v', v), BMStruct ('w', w), BMStruct ('x', x)
,   BMStruct ('y', y), BMStruct ('z', z)
};

#define NR_BITMAPS (int (sizeof(BitMapData) / sizeof (BitmapStruct)))

static int DigitCount = 0;

static XxPixmap *pTempPixmap;

static XxBitmap *DigitBitmaps[NR_BITMAPS];
static char     AsciiToBitmap[256];

static void InitDigitBitmaps (void)
{
    int i;
    BitmapStruct *pBD;

    pTempPixmap = new XxPixmap
        ("AlphaTemp", XxLcdAlphaDigitBack_width, XxLcdAlphaDigitBack_height);

    memset (AsciiToBitmap, sizeof (AsciiToBitmap), 0);

    for (i = 0, pBD = BitMapData; i < NR_BITMAPS; i++, pBD++) {
        DigitBitmaps [i]
            = new XxBitmap
                  ( EzString ("AlphaDigitMaps_") + EzString (i)
                  , pBD->Width, pBD->Height, pBD->Data
                  );
        AsciiToBitmap[pBD->Ascii] = i;
    };
};

XxLcdAlphaDigit::XxLcdAlphaDigit
    ( EzString Name
    , XxDrawable *pParent
    , int XOffset, int YOffset
    , XxColor OnColor, XxColor OffColor
    )
    : XxVirtual (Name, pParent, XOffset, YOffset, 23, 29)
{
    if (DigitCount++ == 0) InitDigitBitmaps ();

    XxLcdAlphaDigit::CharVal   = -1;
    XxLcdAlphaDigit::OnColor    = OnColor;
    XxLcdAlphaDigit::OffColor   = OffColor;

    DrawDigit (0);

#   ifdef CRE_TRACE
    printf ("Create XxLcdAlphaDigit %d\n", Pm);
#   endif

};

XxLcdAlphaDigit::~XxLcdAlphaDigit (void)
{
    int i;

    if (--DigitCount != 0) return;

    for (i = 0; i < NR_BITMAPS; i++) {
        delete DigitBitmaps [i];
    };

    delete pTempPixmap;
};

void XxLcdAlphaDigit::DrawDigit (int CharVal)
{
    if (XxLcdAlphaDigit::CharVal == CharVal) return;

    XxLcdAlphaDigit::CharVal = CharVal;

    pTempPixmap->CopyArea (DrawGC, this, 0, 0, GetWidth (), GetHeight (), 0, 0);

    DrawGC.SetForeground (OffColor);
    DrawGC.SetClipMask   (0, 0, DigitBitmaps [0]);
    pTempPixmap->FillRectangle (DrawGC, 0, 0, GetWidth (), GetHeight ());

    if (CharVal != -1) {
        DrawGC.SetForeground (OnColor);
        DrawGC.SetClipMask   (0, 0, DigitBitmaps [CharVal]);
        pTempPixmap->FillRectangle (DrawGC, 0, 0, GetWidth (), GetHeight ());
    };

    DrawGC.SetClipMask   (0, 0, NULL);

    CopyArea (DrawGC, pTempPixmap, 0, 0, GetWidth (), GetHeight (), 0, 0);
};

int XxLcdAlphaDigit::GetNetWidth ()
{
    return 18;
};

void XxLcdAlphaDigit::SetVal (int Val)
{
/*
    if (Val == ' ') {
        Val = 1;
    } else {
        if (Val >= '0' && Val <= 0x3a) {
            Val -= ('0' - 2);
        } else if (Val >= 'A' && Val <= 'Z') {
            Val -= ('A' - 13);
        } else if (Val >= 'a' && Val <= 'z') {
            Val -= ('a' - 13 - 26);
        } else {
            Val = 1;
        }
    };
    if (Val < 1)          Val = 1;
    if (Val > NR_BITMAPS) Val = 1;
*/
    DrawDigit (AsciiToBitmap[Val]);
};
