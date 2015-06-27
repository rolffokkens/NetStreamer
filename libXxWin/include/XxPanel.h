/*
 * file: XxPanel.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_PANEL
#define H_XX_PANEL

#include "XxWindow.h"

class XxPanel : public XxWindow {
private:
    XxPixmap DrawMap;
protected:
    virtual void OnExpose (int XPos, int YPos, int Width, int Height);
    virtual void OnChange (void);

    virtual void OnPreFlush ();
public:
    XxPanel
        ( EzString Name
        , XxWindow *Parent
        , int XPos, int YPos, int Width, int Height
        , XxColor BackColor = XxGray5
        );

    virtual ~XxPanel (void) {};

    virtual XxDrawable *GetEffDrawable (void);
};

#endif
