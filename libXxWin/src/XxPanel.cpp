/*
 * file: XxPanel.cpp
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <iostream.h>

#include "XxPanel.h"

XxPanel::XxPanel
    ( EzString Name
    , XxWindow *Parent
    , int XPos, int YPos, int Width, int Height
    , XxColor BackColor
    )
    : XxWindow (Name, Parent, XPos, YPos, Width, Height)
    , DrawMap (Name + EzString ("_DrawMap"), Width, Height)
{
    DrawGC.SetForeground (BackColor);
    DrawMap.FillRectangle (DrawGC, 0, 0, GetWidth (), GetHeight ());

    FillRectangle (DrawGC, 0, 0, GetWidth (), GetHeight ());
};

XxDrawable *XxPanel::GetEffDrawable (void)
{
    return &DrawMap;
};

void XxPanel::OnExpose (int XPos, int YPos, int Width, int Height)
{
    DRAW_TRACE ((cout << Name << "::OnExpose" << endl));

    CopyArea
        ( DrawGC, &DrawMap, this
        , XPos, YPos, Width, Height
        , XPos, YPos
        );
    OnChange ();
};

void XxPanel::OnChange (void)
{
    DRAW_TRACE ((cout << Name << "::OnChange" << endl));

    MarkPreFlush ();
};

void XxPanel::OnPreFlush ()
{
    DRAW_TRACE ((cout << Name << "::OnPreFlush" << endl));

    OnExpose (0, 0, GetWidth (), GetHeight ());
};
