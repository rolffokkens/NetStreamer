/*
 * file: XxWindow.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_WINDOW
#define H_XX_WINDOW

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "BiLink.h"
#include "XxDrawable.h"
#include "XxPixmap.h"
#include "XxColor.h"

class XxWindow : public XxDrawable {
    friend class XxApplication;
public:
    typedef BlSet<XxWindow> XxWindowSet;
private:
    XxWindowSet           WindowsMember;
    XxWindowSet::SetOwner Owner;
    XxWindowSet           Member;

    XSizeHints            Hints;
    XxWindow             *Parent;
    Window                Xid;

    XSetWindowAttributes  XxAttrs;
    unsigned long         XxAttrMask;

    void IntUpdateAttrs         (void);
    void IntSetBackgroundPixmap (XxPixmap *pPixmap);

    virtual Drawable qGetDrawable (void) { return Xid; };
protected:
    virtual void HandleEvent (const XEvent &WinEvent);

    virtual void OnDelete        (void);
    virtual void OnExpose        (int XPos, int YPos, int Width, int Height);
    virtual void OnButtonPress   (int Button, int XPos, int YPos);
    virtual void OnButtonRelease (int Button, int XPos, int YPos);
    virtual void OnEnterWindow   (int XPos, int YPos);
    virtual void OnLeaveWindow   (int XPos, int YPos);
    virtual void OnResize        (int Width, int Height);
public:
    XxWindow
        ( EzString Name
        , XxWindow *Parent
        , int XPos, int YPos, int Width, int Height
        , EzString Title = EzString ("")
        );

    virtual ~XxWindow (void);

    int      GetXPos       (void) { return Hints.x;      };
    int      GetYPos       (void) { return Hints.y;      };
    int      GetHeight     (void) { return Hints.height; };
    int      GetWidth      (void) { return Hints.width;  };

    void SetBackground       (XxColor Color);
    void SetBackgroundPixmap (XxPixmap *pPixmap);
    void SetSize             (int Width, int Height);
    void Move                (int XPos,  int YPos);
};

extern XxWindow::XxWindowSet::SetOwner WindowsOwner;

#endif
