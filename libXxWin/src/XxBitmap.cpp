/*
 * file: XxBitmap.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <stdio.h>

#include "XxBitmap.h"

void XxBitmap::Constr (int Width, int Height)
{
    XxBitmap::Width  = Width;
    XxBitmap::Height = Height;

    Pm = XCreatePixmap
             ( GetDisplay ()
             , GetRootWindow ()
             , Width, Height
             , 1
             );
#   ifdef CRE_TRACE
    printf ("Create Bitmap %x\n", int (Pm));
#   endif
};

XxBitmap::XxBitmap (EzString Name, int Width, int Height)
    : XxDrawable (Name)
{
    Constr (Width, Height);
};

XxBitmap::XxBitmap (EzString Name, int Width, int Height, const char *Data)
    : XxDrawable (Name)
{
    Pm = XCreateBitmapFromData
             ( GetDisplay ()
             , GetRootWindow ()
             , Data
             , Width, Height
             );
#   ifdef CRE_TRACE
    printf ("Create Bitmap %x\n", int (Pm));
#   endif
};

void XxBitmap::Destr (void)
{
    XFreePixmap (GetDisplay (), Pm);
};

XxBitmap::~XxBitmap (void)
{
#   ifdef CRE_TRACE
    printf ("Destroy Bitmap %x\n", int (Pm));
#   endif
    Destr ();
};

XxBitmap::XxBitmap (EzString Name, const XxBitmap &pixmap) : XxDrawable (Name)
{
    Constr (pixmap.Width, pixmap.Height);
    CopyArea (DrawGC, &pixmap, 0, 0, Width, Height, 0, 0);
};

XxBitmap &XxBitmap::operator= (const XxBitmap &pixmap)
{
    Destr ();
    Constr (pixmap.Width, pixmap.Height);
    CopyArea (DrawGC, &pixmap, 0, 0, Width, Height, 0, 0);
cout << "Warning: untested XxBitmap::operator=" << endl;
};
