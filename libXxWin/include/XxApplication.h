/*
 * file: XxApplication.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_APPLICATION
#define H_XX_APPLICATION

#include "XxCore.h"
#include "EzString.h"
#include "Exception.h"

class X11Exception : public Exception {
public:
    X11Exception (Display *XxDisplay, EzString Message, EzString Subject);
    virtual ~X11Exception (void) {};
};

class XxApplication {
    friend class XxCore;
private:
    static void Init (void);

    static Display  *XxDisplay;
    static int      XxScreen, XxWidth, XxHeight, XxDepth;
    static Atom     WmDeleteWindow;
    static long     BGW [8];
    static Window   ButtWindow;
    static void     Flush (void);
public:
    XxApplication (void);
    virtual ~XxApplication (void);

    static Display *GetDisplay  (void) {
        Init ();
        return XxDisplay;
    };
    static int GetScreen   (void) {
        Init ();
        return XxScreen;
    };
    static int GetWidth    (void) {
        Init ();
        return XxWidth;
    };
    static int GetHeight   (void) {
        Init ();
        return XxHeight;
    };
    static int GetDepth    (void) {
        Init ();
        return XxDepth;
    };
    static Window GetRootWindow (void) {
        Init ();
        return RootWindow (XxDisplay, XxScreen);
    };
    static Atom GetWmDeleteWindow (void) {
        Init ();
        return WmDeleteWindow;
    };

    static int HandleEvents (void);
    static int MainLoop     (void);
};

#endif

