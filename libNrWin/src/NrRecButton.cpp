#include "NrRecButton.h"

NrRecButton::NrRecButton
    ( EzString Name
    , XxWindow *pParent
    , NrRecFrontend *pFrontend
    , int XPos, int YPos
    , int Width, int Height
    ) : XxButton (Name, pParent, XPos, YPos, Width, Height)
{
    NrRecButton::pFrontend = pFrontend;

    XxPixmap TmpMapRel  ("NrRecButton_TmpRel",  Width, Height);
    XxPixmap TmpMapPsh  ("NrRecButton_TmpPsh",  Width, Height);

    DrawGC.SetForeground (XxGray1);
    TmpMapRel.FillRectangle (DrawGC, 0, 0, Width, Height);
    DrawGC.SetForeground (XxGray4);
    TmpMapPsh.FillRectangle (DrawGC, 0, 0, Width, Height);
    TmpMapRel.DrawRectangle (DrawGC, 0, 0, Width - 1, Height - 1);
    SetBckPixmaps (TmpMapPsh, TmpMapRel);
};
