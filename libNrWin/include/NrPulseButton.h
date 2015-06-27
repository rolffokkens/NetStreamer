#ifndef H_NR_PULSE_BUTTON
#define H_NR_PULSE_BUTTON

#include "NrRecButton.h"

class NrPulseButton : public NrRecButton {
private:
    typedef tXxTimer<NrPulseButton> NrButTimer;

    friend NrButTimer;

    NrButTimer bTimer;
    int MsPulse;
protected:
    void OnButtonPress   (int Button, int XPos, int YPos);
    void OnButtonRelease (int Button, int XPos, int YPos);

    virtual void OnPulse (void) { };
public:
    NrPulseButton
        ( EzString Name
        , XxWindow *pParent
        , NrRecFrontend *pFrontend
        , int XPos, int YPos
        , int Width, int Height
        , int MsPulse
        );

    virtual ~NrPulseButton (void) { };
};

#endif
