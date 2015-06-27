/*
 * file: XxBase.h
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_BASE
#define H_XX_BASE

#include <X11/Xlib.h>

#include "BiLink.h"

#undef CRE_TRACE

// class XxApplication;

class XxBase {
public:
    typedef BlSet<XxBase> XxBaseSet;
private:
    static void Init (void);

    XxBaseSet RemoveMember;
protected:   
    void Remove (void);
public:
    XxBase ();
    virtual ~XxBase (void);

    static void CleanupWasteBasket (void);

    static long GetAllocMem (void);
};

#endif
