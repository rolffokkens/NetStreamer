/*
 * file: XxGC.cpp
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
#include <X11/Xutil.h>

#include "XxGC.h"
#include "XxApplication.h"
#include "XxColor.h"
#include "XxBitmap.h"

XxGC::XxGC (void)
{
    gcmask = 0;

    gcval.font = xFont.GetXFont ();
    gcmask    |= GCFont;

    gc = XCreateGC
             ( XxApplication::GetDisplay ()
             , XxApplication::GetRootWindow ()
             , gcmask
             , &gcval
             );
#   ifdef CRE_TRACE
    printf ("Create GC %x\n", int (gc));
#   endif;
};

XxGC::~XxGC (void)
{
#   ifdef CRE_TRACE
    printf ("Destroy GC %x\n", int (gc));
#   endif;

    XFreeGC (XxApplication::GetDisplay (), gc);
};

void XxGC::Activate (void)
{
    if (gcmask == 0) return;

    XChangeGC
        ( XxApplication::GetDisplay ()
        , gc
        , gcmask
        , &gcval
        );

    gcmask = 0;
};

void XxGC::SetForeground (XxColor Color)
{
    gcmask |= GCForeground;
    gcval.foreground = Color.GetPixel ();
};

void XxGC::SetBackground (XxColor Color)
{
    gcmask |= GCBackground;
    gcval.background = Color.GetPixel ();
};

void XxGC::SetLineWidth (int width)
{
    gcmask |= GCLineWidth;
    gcval.line_width = width;
};

void XxGC::SetClipMask   (int XPos, int YPos, XxBitmap *pBitmap)
{
    XSetClipMask
        ( XxApplication::GetDisplay ()
        , gc
        , (pBitmap == NULL ? None : pBitmap->Pm)
        );
    XSetClipOrigin
        ( XxApplication::GetDisplay ()
        , gc
        , XPos, YPos
        );
};

void XxGC::SetClipRectangle (int XPos, int YPos, int Width, int Height)
{
    XRectangle Rect;
    int        n;

    Rect.x      = XPos;
    Rect.y      = YPos;
    Rect.width  = Width;
    Rect.height = Height;

    n = (Width == 0 && Height == 0 ? 0 : 1);

    XSetClipRectangles
        ( XxApplication::GetDisplay ()
        , gc
        , 0, 0
        , &Rect
        , n
        , Unsorted
        );
};

void XxGC::SetFont (XxFont xFont)
{
    XxGC::xFont = xFont;

    gcval.font  = xFont.GetXFont ();
    gcmask     |= GCFont;
};

void XxGC::GetTextSize
     (EzString Str, int &Width, int &Ascent, int &Descent)
{
    xFont.GetTextSize (Str, Width, Ascent, Descent);
};
