/*
 * file: XxDrawable.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <iostream.h>

#include "XxDrawable.h"

XxDrawable::XxDrawable (EzString Name)
    : XxCore (1)
{
    XxDrawable::Name = Name;

    DRAW_TRACE ((cout << "Create " << Name << endl));
};

XxDrawable::~XxDrawable (void)
{
    DRAW_TRACE ((cout << "Destroy " << Name << endl));
};

class XxDrawable::Exception : public ::Exception {
public:
    Exception (EzString Text);
    ~Exception (void) { };
};

XxDrawable::Exception::Exception (EzString Text) : ::Exception (Text) { };

void XxDrawable::Throw (EzString Text)
{
/*
    throw Exception (Name + EzString (":") + Text);
*/
    cerr << Name << ":" << Text << endl;

    exit (1);
};

void XxDrawable::GetOffset (int &XOffs, int &YOffs)
{
    DRAW_TRACE ((cout << Name << "::GetOffset:" << ":(0,0)" << endl));

    XOffs = 0;
    YOffs = 0;
};

Drawable XxDrawable::GetIntDrawable (void)
{
    return (GetEffDrawable ())->qGetDrawable ();
};

XxDrawable *XxDrawable::GetEffDrawable (void)
{
    return this;
};

void XxDrawable::OnChange (void)
{
};

void XxDrawable::SetLabel
    (EzString Label, int XPos, int YPos, int Width, int Height)
{
    int  W, A, D;
    int  X, Y;

    DrawGC.GetTextSize (Label, W, A, D);
    X = (Width  - W)     / 2 + 1;
    Y = (Height - A - D) / 2 + A;

    DrawGC.SetForeground (XxBlack);
    DrawString  (DrawGC, X + XPos + 1, Y + YPos+ 1, Label);
    DrawGC.SetForeground (XxWhite);   
    DrawString  (DrawGC, X + XPos    , Y + YPos   , Label);
};

void XxDrawable::DrawString
    ( XxGC &Gc, int XPos, int YPos, EzString Str)
{
    int XOffs = 0, YOffs = 0;

    Gc.Activate ();

    GetOffset (XOffs, YOffs);

    XDrawString
        ( GetDisplay (), GetIntDrawable (), Gc.gc
        , XPos + XOffs, YPos + YOffs
        , Str, strlen (Str)
        );

    OnChange ();
};

void XxDrawable::CopyArea
    ( XxGC &Gc, XxDrawable *pSrc, XxDrawable *pDst
    , int XSrc, int YSrc, int Width, int Height
    , int XDst, int YDst
    )
{
    Drawable DSrc, DDst;
    int XSOffs = 0, YSOffs = 0;

    Gc.Activate ();

    pSrc->GetOffset (XSOffs, YSOffs);

    pSrc = pSrc->GetEffDrawable ();

    DSrc = pSrc->qGetDrawable ();
    DDst = pDst->qGetDrawable ();

    if (DSrc == DDst) return;

    XSrc += XSOffs;
    YSrc += YSOffs;

    DRAW_TRACE ((   cout << Name << "::CopyArea (" << Width << "," << Height
                 << ") from " << pSrc->Name << "(" << XSrc << "," << YSrc
                 << ")->(" << pDst->Name << "(" << XDst << "," << YDst
                 << ")" << endl));

    XCopyArea
        ( GetDisplay (), DSrc, DDst
        , Gc.gc, XSrc, YSrc, Width, Height
        , XDst, YDst
        );
};

void XxDrawable::CopyArea
    ( XxGC &Gc, const XxDrawable *pDrawable
    , int XSrc, int YSrc, int Width, int Height
    , int XDst, int YDst
    )
{
    XxDrawable *pNonConst, *pDst;
    int XDOffs = 0, YDOffs = 0;

    pNonConst = (XxDrawable *)pDrawable;

    DRAW_TRACE ((   cout << Name << "::CopyArea ("
                 << pDrawable->Name << ")" << endl));

    GetOffset (XDOffs, YDOffs);

    XDst += XDOffs;
    YDst += YDOffs;

    CopyArea
        ( Gc, pNonConst, GetEffDrawable ()
        , XSrc, YSrc, Width, Height
        , XDst, YDst
        );

    OnChange ();
};

void XxDrawable::DrawLine
    (XxGC &Gc, int X1Pos, int Y1Pos, int X2Pos, int Y2Pos)
{
    int XOffs = 0, YOffs = 0;

    Gc.Activate ();

    GetOffset (XOffs, YOffs);

    XDrawLine
        ( GetDisplay (), GetIntDrawable (), Gc.gc
        , X1Pos + XOffs, Y1Pos + YOffs, X2Pos + XOffs, Y2Pos + YOffs
        );

    OnChange ();
};

void XxDrawable::FillRectangle
    (XxGC &Gc, int XPos, int YPos, int Width, int Height)
{
    int XOffs = 0, YOffs = 0;

    Gc.Activate ();

    GetOffset (XOffs, YOffs);

    XFillRectangle
        ( GetDisplay (), GetIntDrawable (), Gc.gc
        , XPos + XOffs, YPos + YOffs, Width, Height);

    OnChange ();
};

void XxDrawable::DrawRectangle
    (XxGC &Gc, int XPos, int YPos, int Width, int Height)
{
    int XOffs = 0, YOffs = 0;

    Gc.Activate ();

    GetOffset (XOffs, YOffs);

    XDrawRectangle
        ( GetDisplay (), GetIntDrawable (), Gc.gc
        , XPos + XOffs, YPos + YOffs, Width, Height);

    OnChange ();
};


