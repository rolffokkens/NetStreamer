/*
 * file: XxCore.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_CORE
#define H_XX_CORE

#include <X11/Xlib.h>

#include "BiLink.h"
#include "XxBase.h"

#undef CRE_TRACE

class XxApplication;
class XxDrawable;
class XxFont;
class XxColor;

class XxCore : public virtual XxBase {
    friend XxApplication;
    friend XxDrawable;
    friend XxFont;
    friend XxColor;
private:
    typedef BlSet<XxCore> XxCoreSet;

    XxCoreSet PreFlushMember;

    XxCore (int DoCount);

    int DoCount;

    static void Init (void);

    static void     DoPreFlushActions  (void);
protected:   
    static Display *GetDisplay         (void);
    static int      GetScreen          (void);
    static int      GetWidth           (void);
    static Window   GetRootWindow      (void);
    static int      GetDepth           (void);
    static Atom     GetWmDeleteWindow  (void);

    virtual void MarkPreFlush ();
    virtual void OnPreFlush ();
public:
    virtual ~XxCore (void);

    static int GetCount (void);
};

#endif
