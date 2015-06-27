/*
 * file: XxPixmap.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <X11/xpm.h>

#include "Exception.h"
#include "XxPixmap.h"

using namespace std;

void XxPixmap::Constr (int Width, int Height)
{
    XxPixmap::Width  = Width;
    XxPixmap::Height = Height;

    Pm = XCreatePixmap
             ( GetDisplay ()
             , GetRootWindow ()
             , Width, Height
             , GetDepth ()
             );
    Bm = None;

#   ifdef CRE_TRACE
    printf ("Create Pixmap %x\n", int (Pm));
#   endif
};

XxPixmap::XxPixmap (EzString Name, int Width, int Height) : XxDrawable (Name)
{
    Constr (Width, Height);
};

XxPixmap::XxPixmap
    ( EzString Name
    , int Width, int Height
    , const char *Data
    , XxColor FrontColor, XxColor BackColor
    )
    : XxDrawable (Name)
{
    Pm = XCreatePixmapFromBitmapData
             ( GetDisplay ()
             , GetRootWindow ()
             , (char *)Data
             , Width, Height
             , FrontColor.GetPixel (), BackColor.GetPixel ()
             , GetDepth ()
             );
    Bm = None;

#   ifdef CRE_TRACE
    printf ("Create Pixmap %x\n", int (Pm));
#   endif
};

XxPixmap::XxPixmap
    ( EzString Name
    , char * const *Data
    )
    : XxDrawable (Name)
{
    int Err;

XpmAttributes attr;
attr.valuemask = 0;

    Err = XpmCreatePixmapFromData
              ( GetDisplay ()
              , GetRootWindow ()
              , (char **)Data
              , &Pm
              , &Bm
              , &attr
              );

cout << "Err,Pm,Bm = " <<  Err << "," << Pm << "," << Bm << endl;

Throw ("Test");

#   ifdef CRE_TRACE
    printf ("Create Pixmap %x\n", int (Pm));
#   endif
};

void XxPixmap::Destr (void)
{
    XFreePixmap (GetDisplay (), Pm);
    if (Bm != None) XFreePixmap (GetDisplay (), Bm);
};

XxPixmap::~XxPixmap (void)
{
#   ifdef CRE_TRACE
    printf ("Destroy Pixmap %x\n", int (Pm));
#   endif
    Destr ();
};

XxPixmap::XxPixmap (EzString Name, const XxPixmap &pixmap) : XxDrawable (Name)
{
    Constr (pixmap.Width, pixmap.Height);
    CopyArea (DrawGC, &pixmap, 0, 0, Width, Height, 0, 0);
};

XxPixmap &XxPixmap::operator= (const XxPixmap &pixmap)
{
    Destr ();
    Constr (pixmap.Width, pixmap.Height);
    CopyArea (DrawGC, &pixmap, 0, 0, Width, Height, 0, 0);
cout << "Warning: untested XxPixmap::operator=" << endl;
};

