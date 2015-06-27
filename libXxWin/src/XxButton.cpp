/*
 * file: XxButton.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <stdio.h>

#include "XxButton.h"

template <>
void XxButton::XxButTimer::HandleTimeOut (int Count)
{
    pOwner->HandleTimeOut (Count);
};

XxButton::~XxButton (void)
{
};

void XxButton::HandleTimeOut (int Count)
{
    ShowPressed = IsPressed;
    OnExpose (0, 0, GetWidth (), GetHeight ());
};

void XxButton::SetLabel (EzString Label)
{
    XxButton::Label = Label;

    CreateMaps ();

    OnExpose (0, 0, GetWidth (), GetHeight ());
};

XxButton::XxButton
    ( EzString Name
    , XxWindow *pParent
    , int XPos, int YPos
    , int Width, int Height
    )
    : XxWindow (Name, pParent, XPos, YPos, Width, Height)
    , bTimer (this)
    , PressedMap  (Name + EzString ("_PressedMap"),  Width, Height)
    , ReleasedMap (Name + EzString ("_ReleasedMap"), Width, Height)
    , PressedBck  (Name + EzString ("_PressedBck"),  Width, Height)
    , ReleasedBck (Name + EzString ("_ReleasedBck"), Width, Height)
{
    ShowPressed = 0;
    IsPressed   = 0;
    IsInWindow  = 0;

    Label = "";

    SetBackground (XxGray4);

    CreateBckMaps ();
};

void XxButton::CreateMaps (void)
{
    PressedMap.CopyArea
        ( DrawGC, &PressedBck
        , 0, 0, GetWidth (), GetHeight ()
        , 0, 0
        );

    ReleasedMap.CopyArea
        ( DrawGC, &ReleasedBck
        , 0, 0, GetWidth (), GetHeight ()
        , 0, 0
        );

    PressedMap.SetLabel (Label);
    ReleasedMap.SetLabel (Label);
};

void XxButton::CreateBckMaps (void)
{
    int H, W;

    H = GetHeight () - 1;
    W = GetWidth ()  - 1;

    DrawGC.SetForeground (XxGray4);

    PressedBck.FillRectangle  (DrawGC, 0, 0, W, H);
    ReleasedBck.FillRectangle (DrawGC, 0, 0, W, H);

    DrawGC.SetLineWidth  (3);
    DrawGC.SetForeground (XxGray6);
    ReleasedBck.DrawLine (DrawGC, 0, 1, W, 1);
    ReleasedBck.DrawLine (DrawGC, 1, 0, 1, H);

    DrawGC.SetForeground (XxGray2);
    PressedBck.DrawLine  (DrawGC, 0, 1, W, 1);
    PressedBck.DrawLine  (DrawGC, 1, 0, 1, H);
    ReleasedBck.DrawLine (DrawGC, W-1, H, W-1, 0);
    ReleasedBck.DrawLine (DrawGC, W, H-1, 0, H-1);

    DrawGC.SetForeground (XxBlack);
    PressedBck.DrawLine  (DrawGC, W-1, H, W-1, 0);
    PressedBck.DrawLine  (DrawGC, W, H-1, 0, H-1);

    DrawGC.SetForeground (XxBlack);
    DrawGC.SetLineWidth  (1);
    PressedBck.DrawRectangle   (DrawGC, 0, 0, W, H);
    ReleasedBck.DrawRectangle  (DrawGC, 0, 0, W, H);
};

void XxButton::OnExpose (int XPos, int YPos, int Width, int Height)
{
    CopyArea
        ( DrawGC, ((ShowPressed  && IsInWindow)? &PressedMap : &ReleasedMap)
        , XPos, YPos, Width, Height
        , XPos, YPos
        );
};

void XxButton::OnButtonPress (int Button, int XPos, int YPos)
{
    if (Button != 1) return;

    bTimer.SetTimeOut (100);

    IsPressed   = 1;
    ShowPressed = 1;
    OnExpose (0, 0, GetWidth (), GetHeight ());
};

void XxButton::OnButtonRelease (int Button, int XPos, int YPos)
{
    if (Button != 1) return;

    IsPressed   = 0;

    if (IsInWindow) OnClick ();

    if (!bTimer.Running ()) {
        ShowPressed = 0;
        OnExpose (0, 0, GetWidth (), GetHeight ());
    };
};

void XxButton::OnEnterWindow   (int XPos, int YPos)
{
    IsInWindow = 1;

    OnExpose (0, 0, GetWidth (), GetHeight ());
};

void XxButton::OnLeaveWindow   (int XPos, int YPos)
{
    IsInWindow = 0;

    OnExpose (0, 0, GetWidth (), GetHeight ());
};

void XxButton::OnClick (void)
{
};

void XxButton::SetBckPixmaps (void)
{
    CreateBckMaps ();
};

void XxButton::SetBckPixmaps
     ( const XxDrawable &PressedBck
     , const XxDrawable &ReleasedBck
     )
{
    XxButton::PressedBck.CopyArea
        ( DrawGC, &PressedBck
        , 0, 0, GetWidth (), GetHeight ()
        , 0, 0
        );
    XxButton::ReleasedBck.CopyArea
        ( DrawGC, &ReleasedBck
        , 0, 0, GetWidth (), GetHeight ()
        , 0, 0
        );
};
