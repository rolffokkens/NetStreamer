/*
 * file: XxDrawable.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_DRAWABLE
#define H_XX_DRAWABLE

#include "EzString.h"
#include "Exception.h"

#include "XxGC.h"
#include "XxCore.h"

#define DRAW_TRACE(x)

class XxVirtual;
class XxPixmap;
class XxBitmap;
class XxWindow;

class XxDrawable : public XxCore {
    friend XxVirtual;
    friend XxPixmap;
    friend XxBitmap;
    friend XxWindow;
public:
    class Exception;
private:
    virtual Drawable qGetDrawable (void) = 0;
    Drawable GetIntDrawable (void);

    XxDrawable (EzString Name);
protected:
    EzString Name;
    XxGC DrawGC;

    void Throw (EzString Text);

    void CopyArea
        ( XxGC &Gc, XxDrawable *pSrc, XxDrawable *pDst
        , int XSrc, int YSrc, int Width, int Height
        , int XDst, int YDst
        );

    virtual void GetOffset (int &XOffs, int &YOffs);

    virtual void OnChange (void);
public:
    virtual ~XxDrawable (void);

    virtual XxDrawable *GetEffDrawable (void);

    virtual int GetHeight (void) = 0;
    virtual int GetWidth  (void) = 0;

    virtual void FillRectangle
        (XxGC &Gc, int XPos, int YPos, int Width, int Height);
    virtual void DrawRectangle
        (XxGC &Gc, int XPos, int YPos, int Width, int Height);
    virtual void DrawLine
        (XxGC &Gc, int X1Pos, int Y1Pos, int X2Pos, int Y2Pos);
    virtual void CopyArea
        ( XxGC &Gc, const XxDrawable *pDrawable
        , int XSrc, int YSrc, int Width, int Height
        , int XDst, int YDst
        );
    virtual void DrawString
        ( XxGC &Gc, int XPos, int YPos, EzString Str);

    virtual void SetLabel
        (EzString Label, int XPos, int YPos, int Width, int Height);

    virtual void SetLabel (EzString Label) {
        SetLabel (Label, 0, 0, GetWidth (), GetHeight ());
    };
};

#endif
