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

class XxApplication {
    friend class XxCore;
private:
    static int      InitDone;
    static Display  *XxDisplay;
    static int      XxScreen, XxWidth, XxHeight, XxDepth;
    static Atom     WmDeleteWindow;
    static long     BGW [8];
    static Window   ButtWindow;
    static void        Flush (void);
public:
    static void Init (void);

    XxApplication (void);
    virtual ~XxApplication (void);

    static Display    *GetDisplay  (void) { return XxDisplay;  };
    static int         GetScreen   (void) { return XxScreen;   };
    static int         GetWidth    (void) { return XxWidth;    };
    static int         GetHeight   (void) { return XxHeight;   };
    static int         GetDepth    (void) { return XxDepth;    };
    static Window      GetRootWindow (void) {
        return RootWindow (XxDisplay, XxScreen);
    };
    static Atom        GetWmDeleteWindow (void) { return WmDeleteWindow; };

    static int HandleEvents (void);
    static int MainLoop     (void);
};

class XxApplication {
    friend class XxCore;
private:
    static int      InitDone;
    static Display  *XxDisplay;
    static int      XxScreen, XxWidth, XxHeight, XxDepth;
    static Atom     WmDeleteWindow;
    static long     BGW [8];
    static Window   ButtWindow;
    static void        Flush (void);
public:
    static void Init (void);

    XxApplication (void);
    virtual ~XxApplication (void);

    static Display    *GetDisplay  (void) { return XxDisplay;  };
    static int         GetScreen   (void) { return XxScreen;   };
    static int         GetWidth    (void) { return XxWidth;    };
    static int         GetHeight   (void) { return XxHeight;   };
    static int         GetDepth    (void) { return XxDepth;    };
    static Window      GetRootWindow (void) {
        return RootWindow (XxDisplay, XxScreen);
    };
    static Atom        GetWmDeleteWindow (void) { return WmDeleteWindow; };

    static int HandleEvents (void);
    static int MainLoop     (void);
};

class XxDisplay {
private:
    Display Display;
    Atom    WmDeleteWindow;

    int     GetDefaultScreen (void);
public:
    XxDisplay ();
    virtual ~XxDisplay ();
};

class XxScreen {
private:
    int Screen, Width, Height, Depth;
public:
    XxScreen (XxDisplay Display);
    XxScreen (XxDisplay Display, int Screen);

    virtual ~XxScreen (void);

    int GetScreen   (void) { return Screen;   };
    int GetWidth    (void) { return Width;    };
    int GetHeight   (void) { return Height;   };
    int GetDepth    (void) { return Depth;    };
};

#endif

