/*
 * file: XxWindow.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <stdio.h>

#include "XxWindow.h"

#undef TRACE_EVENTS

XxWindow::XxWindowSet::SetOwner WindowsOwner;

void XxWindow::HandleEvent (const XEvent &WinEvent)
{
    switch (WinEvent.type) {
    case ClientMessage:
        if (WinEvent.xclient.format == 32) {
            const long *l = WinEvent.xclient.data.l;
            if (*l == (long)GetWmDeleteWindow ()) OnDelete ();
        };
        break;
    case Expose:
        OnExpose
            ( WinEvent.xexpose.x
            , WinEvent.xexpose.y
            , WinEvent.xexpose.width
            , WinEvent.xexpose.height
            );
        break;
    case ButtonPress:
        OnButtonPress
            ( WinEvent.xbutton.button
            , WinEvent.xbutton.x
            , WinEvent.xbutton.y
            );
        break;
    case ButtonRelease:
        OnButtonRelease
            ( WinEvent.xbutton.button
            , WinEvent.xbutton.x
            , WinEvent.xbutton.y
            );
        break;
    case EnterNotify:
        OnEnterWindow
            ( WinEvent.xcrossing.x
            , WinEvent.xcrossing.y
            );
        break;
    case LeaveNotify:
        OnLeaveWindow
            ( WinEvent.xcrossing.x
            , WinEvent.xcrossing.y
            );
        break;
    case ResizeRequest:
        OnResize
            ( WinEvent.xresizerequest.width
            , WinEvent.xresizerequest.height
            );
        break;
    default:
        break;
    };
};

void XxWindow::OnDelete (void)
{
#ifdef TRACE_EVENTS
    printf ("Delete\n");
#endif
    Remove ();
};

void XxWindow::OnExpose (int XPos, int YPos, int Width, int Height)
{
#ifdef TRACE_EVENTS
    printf ("Expose %d, %d, %d, %d\n", XPos, YPos, Width, Height);
#endif
};

void XxWindow::OnButtonPress (int XPos, int YPos, int Button)
{
#ifdef TRACE_EVENTS
    printf ("ButtonPress %d, %d, %d\n", XPos, YPos, Button);
#endif
};

void XxWindow::OnButtonRelease (int XPos, int YPos, int Button)
{
#ifdef TRACE_EVENTS
    printf ("ButtonRelease %d, %d, %d\n", XPos, YPos, Button);
#endif
};

void XxWindow::OnEnterWindow   (int XPos, int YPos)
{
#ifdef TRACE_EVENTS
    printf ("EnterWindow %d, %d\n", XPos, YPos);
#endif
};

void XxWindow::OnLeaveWindow   (int XPos, int YPos)
{
#ifdef TRACE_EVENTS
    printf ("LeaveWindow %d, %d\n", XPos, YPos);
#endif
};

void XxWindow::OnResize (int Width, int Height)
{
#ifdef TRACE_EVENTS
    printf ("Resize %d, %d\n", Width, Height);
#endif
    SetSize (GetWidth (), GetHeight ());
};

void XxWindow::IntUpdateAttrs (void)
{
    XChangeWindowAttributes (GetDisplay (), Xid, XxAttrMask, &XxAttrs);
    XxAttrMask = 0;
};

void XxWindow::SetBackground (XxColor Color)
{
    DrawGC.SetForeground (Color);
    FillRectangle (DrawGC, 0, 0, GetWidth (), GetHeight ());

    XxAttrs.background_pixel  = Color.GetPixel ();
    XxAttrMask |= CWBackPixel;
    IntUpdateAttrs ();
};

void XxWindow::IntSetBackgroundPixmap (XxPixmap *pPixmap)
{
    XSetWindowBackgroundPixmap (GetDisplay (), Xid, pPixmap->Pm);
};

void XxWindow::SetBackgroundPixmap (XxPixmap *pPixmap)
{
    IntSetBackgroundPixmap (pPixmap);
};

void XxWindow::SetSize (int Width, int Height)
{
    Hints.width      = Width;
    Hints.height     = Height;

    XResizeWindow (GetDisplay (), Xid, Width, Height);
};

void XxWindow::Move (int XPos,  int YPos)
{
    Hints.x = XPos;
    Hints.y = YPos;

    XMoveWindow (GetDisplay (), Xid, XPos, YPos);
};

XxWindow::XxWindow
    ( EzString Name
    , XxWindow *Parent
    , int XPos, int YPos, int Width, int Height
    , EzString Title
    )
    : XxDrawable (Name), Member (this), WindowsMember (this)
{
    Window               XxParentWindow;
    Atom                 atom;
    XxWindow::Parent   = Parent;

    Hints.x            = XPos;
    Hints.y            = YPos;
    Hints.width        = Width;
    Hints.height       = Height;

    XxAttrs.event_mask =   ExposureMask
                         | EnterWindowMask | LeaveWindowMask
                         | ButtonPressMask | ButtonReleaseMask;

    XxAttrMask         = CWEventMask;

    WindowsMember.Attach  (WindowsOwner);
    if (Parent != NULL) Member.Attach (Parent->Owner);

    XxParentWindow =
        (Parent == NULL ? GetRootWindow () : Parent->Xid);

    Xid = XCreateWindow
        ( GetDisplay (), XxParentWindow
        , Hints.x, Hints.y, Hints.width, Hints.height
        , 0, GetDepth ()
        , InputOutput, CopyFromParent
        , XxAttrMask, &XxAttrs);

    XxAttrMask = 0;

    Hints.max_width    = Width;
    Hints.max_height   = Height;
    Hints.min_width    = Width;
    Hints.min_height   = Height;
    Hints.flags        = PMinSize | PMaxSize | PPosition | PSize;

    XSetNormalHints (GetDisplay (), Xid, &Hints);
    // XStoreName      (GetDisplay (), Xid, "Test");

    XMapRaised      (GetDisplay (), Xid);
    XSelectInput    (GetDisplay (), Xid, XxAttrs.event_mask);

    if (Parent == NULL) {
        atom = GetWmDeleteWindow ();

        XStoreName      (GetDisplay (), Xid, Title);
        XSetWMProtocols (GetDisplay (), Xid, &atom, 1);
    };
};

XxWindow::~XxWindow (void)
{
   XDestroyWindow (GetDisplay (), Xid);
};
