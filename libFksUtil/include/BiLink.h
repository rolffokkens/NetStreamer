/*
 * file:  BiLink.h
 *
 * This file is distributed under the GNU GENERAL PUBLIC LICENSE.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *                    fokkensr@freemail.nl
 *                    fokkensr@vertis.nl
 *
 */

#ifndef H_BILINK
#define H_BILINK

#include <stdlib.h>

enum BiLinkType {BilinkOwner, BilinkMember};

class BiLink {
private:
    void        *objp;
    BiLinkType type;
    BiLink      *prv, *nxt, *own;

    virtual void trig_attach (void) { };
    virtual void trig_detach (void) { };
protected:
    int IsAttached (void) { 
        return (own != NULL) && (own != this);
    };
    // int IsAttached (void) { return own != this; };
    int IsMember (void) { return type == BilinkMember; };
    void AttachFirst (BiLink &blk);
    void AttachLast  (BiLink &blk);
    void Detach (void);
    void *GetMemObj (void) { return (!IsMember () ? NULL : objp); };
    void *GetOwnObj (void) { return ( IsMember () ? NULL : objp); };
    void *GetNext (void);
    void *GetPrev (void);
    void *GetOwn (void) { return (IsAttached () ? own->objp : NULL); };
    void *GetFirst (void);
    void *GetLast  (void);
    BiLink (BiLinkType Type, void *pObj);
public:
    virtual ~BiLink (void);
};

template<class OwnClass, class MemClass> class BlSet2 : public BiLink {
private:
    virtual void TrigAttach (MemClass *pMem, OwnClass *pOwn);
    virtual void TrigDetach (MemClass *pMem, OwnClass *pOwn);

    void trig_attach (void) {
        TrigAttach ((MemClass *)GetMemObj (), (OwnClass *) GetOwn ());
    };
    void trig_detach (void) {
        TrigDetach ((MemClass *)GetMemObj (), (OwnClass *) GetOwn ());
    };
public:
    void GetObj (MemClass *&pMem) { pMem = (MemClass *) GetMemObj (); };
    void GetObj (OwnClass *&pOwn) { pOwn = (OwnClass *) GetOwnObj (); };

    MemClass *GetNext (void) { return (MemClass *)(BiLink::GetNext ()); };
    MemClass *GetPrev (void) { return (MemClass *)(BiLink::GetPrev ()); };
    OwnClass *GetOwn (void)  { return (OwnClass *)(BiLink::GetOwn ());  };

    void Attach (BlSet2 &blk) { AttachFirst (blk); };
    void Detach (void)        { BiLink::Detach (); };

    BlSet2 (MemClass *pOwn) : BiLink (BilinkMember, pOwn) { };
    BlSet2 (OwnClass *pMem) : BiLink (BilinkOwner,  pMem) { };

    virtual ~BlSet2 (void) { if (IsMember ()) BiLink::Detach (); };
};

template<class OwnClass, class MemClass> class BlSet1 : public BiLink {
private:
    void trig_attach (void) { };
    void trig_detach (void) { };
public:
    void Attach (BlSet1 &blk) { BiLink::AttachFirst (blk); };
    void Detach (void)        { BiLink::Detach ();        };

    MemClass *GetNext (void) { return (MemClass *)(BiLink::GetNext ()); };
    MemClass *GetPrev (void) { return (MemClass *)(BiLink::GetPrev ()); };
    OwnClass *GetOwn (void)  { return (OwnClass *)(BiLink::GetOwn ());  };

    BlSet1 (MemClass *pMem) : BiLink (BilinkMember, pMem) { };
    BlSet1 (OwnClass *pOwn) : BiLink (BilinkOwner,  pOwn) { };
};

template<class MemClass> class BlSet : public BiLink {
private:
    void trig_attach (void) { };
    void trig_detach (void) { };
public:
    class SetOwner : public BiLink {
    private:
        void trig_attach (void) { };
        void trig_detach (void) { };
    public:
        SetOwner (void) : BiLink (BilinkOwner, this) { };
        MemClass *GetFirst (void) { return (MemClass *) BiLink::GetFirst (); };
        MemClass *GetLast  (void) { return (MemClass *) BiLink::GetLast  (); };
    };
    void Attach (SetOwner &Owner) { AttachFirst (Owner); };
    void Detach (void)            { BiLink::Detach ();   };

    MemClass *GetNext  (void) { return (MemClass *) BiLink::GetNext  (); };
    MemClass *GetPrev  (void) { return (MemClass *) BiLink::GetPrev  (); };

    MemClass *GetFirst (void) { return (MemClass *) BiLink::GetFirst (); };
    MemClass *GetLast  (void) { return (MemClass *) BiLink::GetLast  (); };

    BlSet (MemClass *pMem) : BiLink (BilinkMember, pMem) { };
};

#endif
