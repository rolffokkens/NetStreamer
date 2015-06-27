/*
 * file:  BiLink.cpp
 *
 * This file is distributed under the GNU GENERAL PUBLIC LICENSE.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *                    fokkensr@freemail.nl
 *                    fokkensr@vertis.nl
 *
 */

#include <iostream>
#include "BiLink.h"

using namespace std;

BiLink::BiLink (BiLinkType Type, void *pObj)
{
    BiLink *blp;

    blp  = (Type == BilinkOwner ? this : (BiLink *)NULL);

    type = Type;
    prv  = blp;
    nxt  = blp;
    own  = blp;
    // own  = this;
    objp = pObj;
};

BiLink::~BiLink (void)
{
    if (IsMember ()) {
        Detach ();
    } else {
        while (nxt != this) nxt->Detach ();
    };
};

void BiLink::Detach (void)
{
    if (!IsMember ()) {
        cerr << "BiLink::Detach: Owner!" << endl;
        exit (1);
    };

    if (nxt != NULL) trig_detach ();

    if (nxt != NULL) {
        nxt->prv = prv;
        prv->nxt = nxt;

        nxt = NULL;
        prv = NULL;
        own = NULL;
        // own = this;
    }
};

void BiLink::AttachFirst (BiLink &blk)
{
    if (!IsMember ()) {
        cerr << "BiLink::AttachFirst: Owner!" << endl;
        exit (1);
    };

    Detach ();

    BiLink &next = *(blk.nxt);
    BiLink &prev = blk;

    next.prv = this;
    prev.nxt = this;

    nxt = &next;
    prv = &prev;
    own = &blk;

    trig_attach ();
};

void BiLink::AttachLast (BiLink &blk)
{
    if (!IsMember ()) {
        cerr << "BiLink::AttachLast: Owner!" << endl;
        exit (1);
    };

    Detach ();

    BiLink &next = blk;
    BiLink &prev = *(blk.prv);

    next.prv = this;
    prev.nxt = this;

    nxt = &next;
    prv = &prev;
    own = &blk;

    trig_attach ();
};

void *BiLink::GetNext (void)
{
    if (nxt == own) {
        return NULL;
    } else {
        if (nxt == NULL) {
            cerr << "BiLink::GetNext (Internal): nxt == NULL" << endl;
            exit (1);
        };
        return nxt->objp;
    }
};

void *BiLink::GetPrev (void)
{
    if (prv == own) {
        return NULL;
    } else {
        if (prv == NULL) {
            cerr << "BiLink::GetNext (Internal): nxt == NULL" << endl;
            exit (1);
        };
        return prv->objp;
    }
};

void *BiLink::GetFirst (void)
{
    return (own == NULL ? NULL : own->nxt->GetMemObj ());
};

void *BiLink::GetLast (void)
{
    return (own == NULL ? NULL : own->prv->GetMemObj ());
};

