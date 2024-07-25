/*
 * file: XxFont.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <stdio.h>

#include <X11/Xlib.h>

#include <iostream>

#include "XxFont.h"
#include "BiLink.h"
#include "XxApplication.h"

using namespace std;

struct XxIntFontCommon {
    typedef BlSet<XxIntFontCommon> FontCommonSet;

    FontCommonSet Member;

    int          RefCount;
    Font         xFont;
    XFontStruct *pFontInfo;

    EzString Name;
    int      Size;

    XxIntFontCommon (void) : Member (this) { };
};

static XxIntFontCommon::FontCommonSet::SetOwner CommonOwner;

static XxIntFontCommon *LookupFont (EzString Name, int Size)
{
    XxIntFontCommon *pCommon;

    pCommon = CommonOwner.GetFirst ();
    while (pCommon!= NULL) {
        if (pCommon->Name == Name && pCommon->Size == Size) return pCommon;
        pCommon = pCommon->Member.GetNext ();
    };
    return pCommon;
};

static EzString GetFullFontName (EzString Name, int Size)
{
    EzString RetVal;

    RetVal =   EzString ("-*-") + Name
             + EzString ("-bold-r-normal-*-") + EzString (Size)
             + EzString ("-*-*-*-*-*-iso8859-1");

    return RetVal;
};

XxIntFontCommon *XxFont::IntGetFont (EzString Name, int Size)
{
    XxIntFontCommon *pCommon;

    pCommon = LookupFont (Name, Size);
    if (pCommon == NULL) {
        pCommon = new XxIntFontCommon;

        pCommon->xFont = XLoadFont (GetDisplay (), GetFullFontName (Name, Size));
        if (!pCommon->xFont) throw X11Exception (GetDisplay (), "XLoadFont error for font", Name);

        pCommon->pFontInfo = XQueryFont (GetDisplay (), pCommon->xFont);
        if (!pCommon->pFontInfo) throw X11Exception (GetDisplay (), "XQueryFont error for font", Name);
        pCommon->Name     = Name;
        pCommon->Size     = Size;
        pCommon->RefCount = 0;
        pCommon->Member.Attach (CommonOwner);

#       ifdef CRE_TRACE
        printf ("Create Font %d\n", pCommon->xFont);
#       endif
    };
    pCommon->RefCount++;

    return pCommon;
};

static void PutFont (Display *xDisplay, XxIntFontCommon *pCommon)
{
    EzString Temp;

    if (--(pCommon->RefCount) == 0) {
#       ifdef CRE_TRACE
        printf ("Destroy Font %d\n", pCommon->xFont);
#       endif
        Temp  = GetFullFontName (pCommon->Name, pCommon->Size);
        XFreeFontInfo (NULL, pCommon->pFontInfo, 1);
        XUnloadFont (xDisplay, pCommon->xFont);
        delete pCommon;
    };
};

Font XxFont::GetXFont ()
{
    return pCommon->xFont;
};

XxFont::XxFont (EzString Name, int Size)
    : XxCore (0)
{
    pCommon = IntGetFont (Name, Size);
};

XxFont::XxFont (XxFont &xFont)
    : XxCore (0)
{
    pCommon = xFont.pCommon;
    pCommon->RefCount++;
};

XxFont::~XxFont (void)
{
    PutFont (GetDisplay (), pCommon);
};

XxFont &XxFont::operator= (XxFont &xFont)
{
    xFont.pCommon->RefCount++;
    PutFont (GetDisplay (), pCommon);

    pCommon = xFont.pCommon;

    return *this;
};

void XxFont::GetTextSize
     (EzString Str, int &Width, int &Ascent, int &Descent)
{
    XGCValues    GCval;
    XCharStruct  CharInfo;
    XFontStruct *pFontInfo;
    int          Direction;
/*
    XGetGCValues
        ( XxApplication::GetDisplay ()
        , gc
        , 0x0ffff
        , &GCval
        );

    pFontInfo = XQueryFont (XxApplication::GetDisplay (), GCval.font);
*/
    XTextExtents
        ( pCommon->pFontInfo
        , Str
        , strlen (Str)
        , &Direction
        , &Ascent
        , &Descent
        , &CharInfo
        );
    Width = CharInfo.width;
};
