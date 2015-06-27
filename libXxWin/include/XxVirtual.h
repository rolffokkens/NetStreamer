/*
 * file: XxVirtual.h
 *
 * This file is part of the XxWinLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_VIRTUAL
#define H_XX_VIRTUAL

#include "EzString.h"

#include "XxGC.h"
#include "XxDrawable.h"

class XxVirtual : public XxDrawable {
private:
    XxDrawable *pParent;
    int XOffset,  YOffset;
    int Width, Height;
    virtual Drawable qGetDrawable (void) {
        return pParent->qGetDrawable ();
    };
    Drawable GetIntDrawable (void);
protected:
    XxGC DrawGC;

    virtual void GetOffset (int &XOffs, int &YOffs);

    virtual void OnChange (void);
public:
    XxVirtual
        ( EzString Name
        , XxDrawable *pParent
        , int XOffset, int YOffset
        , int Width,   int Height
        );

    virtual ~XxVirtual (void) { };

    virtual XxDrawable *GetEffDrawable (void);

    int GetHeight (void) { return Height; };
    int GetWidth  (void) { return Width;  };
/*
    virtual void DrawString
        ( XxGC &Gc, int XPos, int YPos, EzString Str);
    virtual void SetLabel
        (EzString Label, int XPos, int YPos, int Width, int Height);
*/
};

#endif
