/*
 * file: XxLcdScrollDisplay.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_LCDSCROLLDISPLAY
#define H_XX_LCDSCROLLDISPLAY

#include "XxLcdAlphaDisplay.h"
#include "XxTimer.h"

class XxLcdScrollDisplay : public XxLcdAlphaDisplay {
private:
    typedef tXxTimer<XxLcdScrollDisplay> ScrollTimerTyp;
    friend ScrollTimerTyp;

    ScrollTimerTyp ScrollTimer;

    void HandleScrollTimeout ();

    int      RefreshDelay;
    EzString DisplayVal;
    EzString CurScrollVal;
protected:
    virtual void HandleScrollDone (void);
public:
    XxLcdScrollDisplay
        ( EzString Name
        , XxDrawable *pParent
        , int XOffset, int YOffset
        , int nDigits
        , XxColor OnColor, XxColor OffColor
        , int RefreshDelay
        );

    virtual ~XxLcdScrollDisplay (void);

    virtual void SetVal       (EzString Val);
    virtual void SetScrollVal (EzString Val);

    virtual EzString GetVal (void) { return DisplayVal; };
};

template<class Owner> class tXxLcdScrollDisplay : public XxLcdScrollDisplay {
private:
    Owner *pOwner;
protected:
    virtual void HandleScrollDone (void);
public:
    tXxLcdScrollDisplay
        ( Owner *pOwner
        , EzString Name
        , XxDrawable *pParent
        , int XOffset, int YOffset
        , int nDigits
        , XxColor OnColor, XxColor OffColor
        , int RefreshDelay
        ) : XxLcdScrollDisplay
            ( Name
            , pParent
            , XOffset, YOffset
            , nDigits
            , OnColor, OffColor
            , RefreshDelay
            )
    {
        tXxLcdScrollDisplay::pOwner = pOwner;
    };
};

#endif
