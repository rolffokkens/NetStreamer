/*
 * file: XxApplication.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <fcntl.h>

#include <iostream>

#include "XxBlocker.h"
#include "XxTimer.h"
#include "XxApplication.h"
#include "XxWindow.h"
#include "XxAutoInit.h"

#include "XxButton.h"

using namespace std;

class XxAppBlocker : public XxBlocker {
protected:
public:
    XxAppBlocker (int XxFd);
    ~XxAppBlocker (void);

    virtual void     GetRWFlags (int &rFlag, int &wFlag);
    virtual XxAction GetAction  (int &rFlag, int &wFlag);

    virtual void HandleAction (XxAction Action);

    long GetInCount  (void) { return 0; };
    long GetOutCount (void) { return 0; };
};

NULLINIT (XxAppBlocker  *pDummyAppBlocker);

static int InitDone = 0;
static XErrorEvent LastXErrorEvent = {};
static int XxErrorHandler (Display *d, XErrorEvent *e)
{
    LastXErrorEvent = *e;
    return 0;
}

static EzString FormatX11Exception (Display *XxDisplay, EzString Message, EzString Subject)
{
    char buf[64];

    if (Subject != EzString ("")) {
        Subject = EzString (" \"") + Subject + EzString ("\"");
    }
    XGetErrorText (XxDisplay, LastXErrorEvent.error_code, buf, sizeof(buf));
    return Message + Subject + EzString (": ") + EzString (buf);
}

X11Exception::X11Exception (Display *XxDisplay, EzString Message, EzString Subject)
    : Exception (FormatX11Exception (XxDisplay, Message, Subject))
{
}

void XxApplication::Init (void)
{
    int    i, Fd;
    XColor color;
    Status s;

    if (InitDone) return;

    XxDisplay  = XOpenDisplay  (NULL);
    if (!XxDisplay) throw Exception("Display error: Connection to X server failed");

    XSetErrorHandler(XxErrorHandler);

    XxScreen   = DefaultScreen (XxDisplay);

    XxWidth    = DisplayWidth  (XxDisplay, XxScreen);
    XxHeight   = DisplayHeight (XxDisplay, XxScreen);
    XxDepth    = DefaultDepth  (XxDisplay, XxScreen);

    WmDeleteWindow = XInternAtom   (XxDisplay, "WM_DELETE_WINDOW", False);

    InitDone = 1;

    BGW [0] = BlackPixel (GetDisplay (), GetScreen ());
    BGW [7] = WhitePixel (GetDisplay (), GetScreen ());

    for (i = 1; i < 7; i++) {
        color.red   = (i * 0x2000);
        color.green = (i * 0x2000);
        color.blue  = (i * 0x2000);

        XAllocColor (XxDisplay, DefaultColormap (XxDisplay, XxScreen), &color);

        BGW [i] = color.pixel;
    };

    Fd               = ConnectionNumber (XxDisplay);
    pDummyAppBlocker = new XxAppBlocker (Fd);

    fcntl (Fd, F_SETFL, O_NONBLOCK);

    AUTOINIT_TRACE ("XxApplication::Init");
};

XxAppBlocker::XxAppBlocker (int XxFd)
{
    SetFd (XxFd);
    SetStatus (StatOpen);
};

XxAppBlocker::~XxAppBlocker (void)
{
    XCloseDisplay (XxApplication::GetDisplay ());
    SetFd (-1);
};

void XxAppBlocker::GetRWFlags (int &rFlag, int &wFlag)
{
    rFlag = 1;
    wFlag = 0;
};

XxBlocker::XxAction XxAppBlocker::GetAction  (int &rFlag, int &wFlag)
{
    return (rFlag ? ActRead : ActNone);
};

void XxAppBlocker::HandleAction (XxAction Action)
{
    XxApplication::HandleEvents ();
};

Display *XxApplication::XxDisplay;
int      XxApplication::XxScreen, XxApplication::XxWidth;
int      XxApplication::XxHeight, XxApplication::XxDepth;
Atom     XxApplication::WmDeleteWindow;
long     XxApplication::BGW [8];
Window   XxApplication::ButtWindow = 0;

XxApplication::XxApplication (void)
{
    Init ();
};

XxApplication::~XxApplication (void)
{
    delete pDummyAppBlocker;
};

int XxApplication::HandleEvents (void)
{
    XEvent   event;
    XxWindow *pWindow;
    Window    PassOnWindow;

    Init ();

    for (;;) {
        XNextEvent (GetDisplay (), &event);

        switch (event.type) {
        case ButtonPress:
            PassOnWindow = event.xexpose.window;
            ButtWindow   = PassOnWindow;
            break;
        case ButtonRelease:
            PassOnWindow = ButtWindow;
            ButtWindow   = 0;
            break;
        case ClientMessage:
        case Expose:
        case EnterNotify:
        case LeaveNotify:
        case ResizeRequest:
            PassOnWindow = event.xexpose.window;
            break;
        default:
            PassOnWindow = 0;
            break;
        };

        if (PassOnWindow != 0) {
            pWindow = WindowsOwner.GetFirst ();
            while (pWindow != NULL) {
                if (pWindow->Xid == PassOnWindow) {
                    pWindow->HandleEvent (event);
                    break;
                };
                pWindow = pWindow->WindowsMember.GetNext ();
            };
        };
        if (!XPending (GetDisplay ())) break;
    };
    return 0;
};

void XxApplication::Flush (void)
{
    Init ();

    XxCore::DoPreFlushActions ();

    XFlush (GetDisplay ());
};

int XxApplication::MainLoop (void)
{
    XEvent              event;
    XxWindow           *pWindow;
    XxBlocker          *pBlocker;
    int                 mSec;
    XxBlocker::XxAction bAction;
    int                 mSize, lSize = - 1;

    Init ();

    XxTimer::RefreshMsCurTime ();

    for (;;) {
        Flush ();

        if (XxCore::GetCount () == 0) break;

        XxCore::CleanupWasteBasket ();

        mSec = XxBlocker::HandleTimers ();

        for (;;) {
            Flush ();

            if (XxCore::GetCount () == 0) break;

            if (!XxBlocker::HandleBlocker (mSec)) break;
        };
    };

    return 0;
};

