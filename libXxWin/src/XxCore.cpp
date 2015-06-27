/*
 * file: XxCore.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <iostream.h>

#include "XxCore.h"
#include "BiLink.h"
#include "XxAutoInit.h"

#include "XxApplication.h"

static XxCore::XxCoreSet::SetOwner *pPreFlushOwner = NULL;

static int CoreCount = 0;

static int InitDone = 0;

void XxCore::Init (void)
{
    if (InitDone) return;

    pPreFlushOwner = new XxCore::XxCoreSet::SetOwner;
    AUTOINIT_TRACE ("XxCore::Init");

    InitDone = 1;
};

void XxCore::MarkPreFlush ()
{
    PreFlushMember.Attach (*pPreFlushOwner);
};

void XxCore::OnPreFlush ()
{
    cout << "Unhandled OnPreFlush" << endl;
};

XxCore::XxCore (int DoCount)
    : PreFlushMember (this)
{
    Init ();

    XxCore::DoCount = DoCount;

    if (DoCount) CoreCount++;
};

XxCore::~XxCore (void)
{
    if (DoCount) CoreCount--;
};

void XxCore::DoPreFlushActions (void)
{
    XxCore *pBase;

    Init ();

    for (;;) {
        pBase = pPreFlushOwner->GetFirst ();
        if (pBase == NULL) break;
        pBase->OnPreFlush ();
        pBase->PreFlushMember.Detach ();
    };
};

Display *XxCore::GetDisplay (void)
{
    Init ();

    return XxApplication::GetDisplay ();
};

int XxCore::GetScreen  (void)
{
    Init ();

    return XxApplication::GetScreen ();
};

int XxCore::GetWidth   (void)
{
    Init ();

    return XxApplication::GetWidth  ();
};

Window XxCore::GetRootWindow (void)
{
    Init ();

    return XxApplication::GetRootWindow ();
};

int XxCore::GetDepth (void)
{
    Init ();

    return XxApplication::GetDepth ();
};

Atom XxCore::GetWmDeleteWindow (void)
{
    Init ();

    return XxApplication::GetWmDeleteWindow ();
};

int XxCore::GetCount (void)
{
    Init ();

    return CoreCount;
};
