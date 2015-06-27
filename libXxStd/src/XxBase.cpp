/*
 * file: XxBase.cpp
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <iostream>
#include <malloc.h>

#include "XxBase.h"
#include "BiLink.h"
#include "XxAutoInit.h"

static XxBase::XxBaseSet::SetOwner *pRemoveOwner = NULL;

static int InitDone = 0;

void XxBase::Init (void)
{
    if (InitDone) return;

    pRemoveOwner   = new XxBase::XxBaseSet::SetOwner;

    AUTOINIT_TRACE ("XxBase::Init");

    InitDone = 1;
};

XxBase::XxBase (void)
    : RemoveMember   (this)
{
    Init ();
};

XxBase::~XxBase (void)
{
};

long XxBase::GetAllocMem (void)
{
    struct mallinfo     mInfo;

    Init ();

    mInfo = mallinfo ();

    return mInfo.uordblks;
};

void XxBase::CleanupWasteBasket (void)
{
    XxBase *pBase;

    Init ();

    for (;;) {
        pBase = pRemoveOwner->GetFirst ();
        if (pBase == NULL) break;
        delete pBase;
    };
};

void XxBase::Remove (void)
{
   RemoveMember.Attach (*pRemoveOwner);
};

