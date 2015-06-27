#include "NrPulseButton.h"

NrPulseButton::NrPulseButton
    ( EzString Name
    , XxWindow *pParent
    , NrRecFrontend *pFrontend
    , int XPos, int YPos
    , int Width, int Height
    , int MsPulse
    ) : NrRecButton
        ( Name
        , pParent
        , pFrontend
        , XPos, YPos
        , Width, Height
        )
      , bTimer (this)
{
    NrPulseButton::MsPulse = MsPulse;
};

void NrPulseButton::OnButtonPress (int Button, int XPos, int YPos)
{
    NrRecButton::OnButtonPress   (Button, XPos, YPos);
    bTimer.SetRestart (MsPulse);
    OnPulse ();
};

void NrPulseButton::OnButtonRelease (int Button, int XPos, int YPos)
{
    NrRecButton::OnButtonRelease (Button, XPos, YPos);
    bTimer.Stop ();
};

template <>
void NrPulseButton::NrButTimer::HandleTimeOut (int Count)
{
    pOwner->OnPulse ();
};

