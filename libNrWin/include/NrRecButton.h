#ifndef H_NR_REC_BUTTON
#define H_NR_REC_BUTTON

#include "XxButton.h"

class NrRecFrontend;

class NrRecButton : public XxButton {
private:
protected:
    NrRecFrontend *pFrontend;
public:
    NrRecButton
        ( EzString Name
        , XxWindow *pParent
        , NrRecFrontend *pFrontend
        , int XPos, int YPos
        , int Width, int Height
        );

    virtual ~NrRecButton (void) {};
};

#endif
