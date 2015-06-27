/*
 * file: XxButton.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_BUTTON
#define H_XX_BUTTON

#include "EzString.h"

#include "XxWindow.h"
#include "XxTimer.h"

class XxButton : public XxWindow {
private:
    typedef tXxTimer<XxButton> XxButTimer;

    friend class tXxTimer<XxButton>;
private:
    XxButTimer bTimer;
    int        ShowPressed;
    int        IsPressed;
    int        IsInWindow;
    XxPixmap   PressedBck, ReleasedBck;
    XxPixmap   PressedMap, ReleasedMap;
    EzString   Label;
    void       CreateBckMaps (void);
    void       CreateMaps (void);
    void       HandleTimeOut (int Count);
protected:
    virtual void OnExpose        (int XPos, int YPos, int Width, int Height);
    virtual void OnButtonPress   (int Button, int XPos, int YPos);
    virtual void OnButtonRelease (int Button, int XPos, int YPos);
    virtual void OnEnterWindow   (int XPos, int YPos);
    virtual void OnLeaveWindow   (int XPos, int YPos);

    virtual void OnClick (void);
public:
    XxButton
        ( EzString Name
        , XxWindow *pParent
        , int XPos, int YPos
        , int Width, int Height
        );

    virtual ~XxButton (void);
    void SetLabel (EzString Label);
    void SetBckPixmaps (void);
    void SetBckPixmaps
        ( const XxDrawable &pPressedBck
        , const XxDrawable &pReleasedBck
        );
};

#endif
