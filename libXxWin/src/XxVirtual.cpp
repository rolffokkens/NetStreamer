/*
 * file: XxVirtual.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <iostream>

#include "XxVirtual.h"

using namespace std;

XxVirtual::XxVirtual
    ( EzString Name
    , XxDrawable *pParent
    , int XOffset, int YOffset
    , int Width,   int Height
    )
    : XxDrawable (Name)
{
    XxVirtual::pParent = pParent;
    XxVirtual::XOffset = XOffset;
    XxVirtual::YOffset = YOffset;
    XxVirtual::Width   = Width;
    XxVirtual::Height  = Height;
};

Drawable XxVirtual::GetIntDrawable (void)
{
    return pParent->GetIntDrawable ();
};

XxDrawable *XxVirtual::GetEffDrawable (void)
{
    return pParent->GetEffDrawable ();
};

void XxVirtual::GetOffset (int &XOffs, int &YOffs)
{
    pParent->GetOffset (XOffs, YOffs);

    DRAW_TRACE ((   cout << Name << "::GetOffset:" << ":(" << XOffset << ","
                 << YOffset << ")" << "->" << endl));

    XOffs += XOffset;
    YOffs += YOffset;
};

void XxVirtual::OnChange (void)
{
    pParent->OnChange ();
};
