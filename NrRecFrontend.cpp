/*
 * file: NrRecFrontend.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <string.h>

#include "BiLink.h"

#undef TRACE_EVENTS
#define REFRESH_DELAY 50

#include "XxApplication.h"
#include "XxGC.h"
#include "XxBase.h"
#include "XxDrawable.h"
#include "XxPixmap.h"
#include "XxBitmap.h"
#include "XxWindow.h"
#include "XxButton.h"
#include "XxPanel.h"
#include "XxMaxMeter.h"
#include "XxLcdNumDisplay.h"
#include "XxLcdScrollDisplay.h"
#include "XxFlashLight.h"
#include "XxTimer.h"
#include "Nr8Log.h"
#include "NrGraphicAnalyzer.h"
#include "NrReceiver.h"

#include "NrRecButton.h"
#include "NrPulseButton.h"
#include "NrPresetMemory.h"

#include "NetStreamer.xbm"
#include "NrMHz.xbm"
#include "NrAnalyzer.xbm"
#include "NrBuffer.xbm"
#include "NrVolume.xbm"
#include "NrRecord.xbm"
#include "NrMiss.xbm"
#include "Nr8kHz.xbm"
#include "Nr16kHz.xbm"
#include "NrCharge.xbm"
#include "NrAdjust.xbm"
#include "NrMute.xbm"
#include "NrMem.xbm"
#include "NrP1.xbm"
#include "NrP2.xbm"
#include "NrP3.xbm"
#include "NrP4.xbm"
#include "NrP5.xbm"
#include "NrP6.xbm"
#include "NrP7.xbm"
#include "NrP8.xbm"

using namespace std;

const int pButtonCount        = 8;

const int RecDisplayMargin    = 10;
const int NrRecFrontendMargin = 10;

const int RecDisplayPosX      = NrRecFrontendMargin;
const int RecDisplayPosY      = NrRecFrontendMargin;

const int RecDisplayWidth     = 376 + 2 * RecDisplayMargin;
const int RecDisplayHeight    =  94 + 2 * RecDisplayMargin;

const int ButtonPanelPosX     = NrRecFrontendMargin;
const int ButtonPanelPosY     =   RecDisplayHeight + NrRecFrontendMargin
                                + NrRecFrontendMargin;

const int FreqDisplayPosX     = RecDisplayMargin;
const int FreqDisplayPosY     = RecDisplayMargin;

const int StationDisplayPosX  = 124 + RecDisplayMargin;
const int StationDisplayPosY  = RecDisplayMargin;

const int InfoDisplayPosX     = RecDisplayMargin - 2;
const int InfoDisplayPosY     = RecDisplayMargin + 30;

const int NrMHzPosX           = 72 + RecDisplayMargin;
const int NrMHzPosY           = RecDisplayMargin;

const int VolMeterPosX        = RecDisplayMargin;
const int VolMeterPosY        = 61 + RecDisplayMargin;

const int AdjustMeterPosX     = 295 + RecDisplayMargin;
const int AdjustMeterPosY     = RecDisplayMargin;

const int BufMeterPosX        = AdjustMeterPosX;
const int BufMeterPosY        = 25 + RecDisplayMargin;

const int AnalyzerPosX        = AdjustMeterPosX;
const int AnalyzerPosY        = 49 + RecDisplayMargin;

const int LightPanelPosX      = RecDisplayMargin;
const int LightPanelPosY      = 83 + RecDisplayMargin;

#define GetLightPanelPosX(i)  (LightPanelPosX + (i * 43))

const int Margin1             = 4;

const int pmButtonWidth       = 20;
const int pmButtonHeight      = 19;

const int Border1PosX         = 0;
const int Border1PosY         = 0;

const int tDownButtonPosX     = Border1PosX + Margin1;
const int tDownButtonPosY     = Border1PosY + Margin1;

const int Label1Width         = 56;
const int Label1Height        = pmButtonHeight;
const int Label1PosX          = tDownButtonPosX + pmButtonWidth;
const int Label1PosY          = tDownButtonPosY;

const int tUpButtonPosX       = Label1PosX + Label1Width;
const int tUpButtonPosY       = Label1PosY;

const int MemButtonWidth      = 40;
const int MemButtonHeight     = pmButtonHeight;

const int MemButtonPosX       = tUpButtonPosX + pmButtonWidth + (Margin1 * 2);
const int MemButtonPosY       = tDownButtonPosY;

const int pButtonWidth        = 26;
const int pButtonHeight       = pmButtonHeight;

#define pButtonPosX(i)                           \
    (  MemButtonPosX + MemButtonWidth + 2 * Margin1  \
     + ((pButtonWidth + Margin1) * i))

const int pButtonPosX         = tDownButtonPosY;

const int Border1Width        = pButtonPosX (pButtonCount) - 1;
const int Border1Height       = pmButtonHeight + Margin1  * 2 - 1;

const int Border2PosX         = Border1PosX;
const int Border2PosY         = Border1PosY + Border1Height + 2 * Margin1;

const int vDownButtonPosX     = Border2PosX + Margin1;
const int vDownButtonPosY     = Border2PosY + Margin1;

const int Label2Width         = Label1Width;
const int Label2Height        = Label1Height;
const int Label2PosX          = Label1PosX;
const int Label2PosY          = vDownButtonPosY;

const int vUpButtonPosX       = Label2PosX + Label2Width;
const int vUpButtonPosY       = vDownButtonPosY;

const int MutButtonWidth      = MemButtonWidth;
const int MutButtonHeight     = pmButtonHeight;

const int MutButtonPosX       = MemButtonPosX;
const int MutButtonPosY       = vDownButtonPosY;

const int Border2Width        = MutButtonPosX + MutButtonWidth + Margin1 - 1;
const int Border2Height       = Border1Height;

const int RecButtonWidth      = 50;
const int RecButtonHeight     = pmButtonHeight;

const int Border3PosX         = 396 - RecButtonWidth - Margin1 * 2;
const int Border3PosY         = Border2PosY;

const int Border3Width        = RecButtonWidth + 2 * Margin1 - 1;
const int Border3Height       = Border2Height;

const int RecButtonPosX       = Border3PosX + Margin1;
const int RecButtonPosY       = Border3PosY + Margin1;

const int NetStreamerWidth    = NetStreamer_width;
const int NetStreamerHeight   = NetStreamer_height;

const int NetStreamerPosX     = (  Border2Width + Border2PosX
                                 + Border3PosX  - NetStreamerWidth
                                ) / 2;
              
const int NetStreamerPosY     = Border2PosY + Border2Height - NetStreamerHeight;

const int ButtonPanelWidth    = RecDisplayWidth;
const int ButtonPanelHeight   = Border2Height + Border2PosY + 1;

const int NrRecFrontendWidth  =   RecDisplayPosX  + RecDisplayWidth
                                + NrRecFrontendMargin + 1;
const int NrRecFrontendHeight =   ButtonPanelPosY + ButtonPanelHeight
                                + NrRecFrontendMargin + 1;

struct BitmapStruct {
    int Width, Height;
    char *Data;
};

#define BMStruct(X) \
{NrP ## X ## _width,  NrP ## X ## _height, \
 (char *)(NrP ## X ## _bits)}

static BitmapStruct PresetBitMapData [pButtonCount] = {
    BMStruct (1)
,   BMStruct (2)
,   BMStruct (3)
,   BMStruct (4)
,   BMStruct (5)
,   BMStruct (6)
,   BMStruct (7)
,   BMStruct (8)
};

class NrRecFrontend;

class NrRecDisplay : public XxPanel {
public:
    NrRecDisplay
        ( EzString Name
        , XxWindow *Parent
        , int XPos, int YPos, int Width, int Height
        , XxColor BackColor = XxGray5
        );

    virtual ~NrRecDisplay (void) {};
};

NrRecDisplay::NrRecDisplay
    ( EzString Name
    , XxWindow *Parent
    , int XPos, int YPos, int Width, int Height
    , XxColor BackColor
    ) : XxPanel
        ( Name
        , Parent
        , XPos, YPos, Width, Height
        , BackColor
        )
{
    DrawGC.SetForeground (XxGray3);

    DrawRectangle (DrawGC, 0, 0, Width - 1, Height - 1);
};

class NrVolMeter : public XxMeter {
private:
    static const int BufStep   = 4;
    static const int NrSteps   = 20;
    static const int BufWidth  = NrSteps * BufStep + 1;
    static const int BufHeight = 5;
public:
    NrVolMeter (XxDrawable *pParent, int XPos, int YPos);
    virtual ~NrVolMeter (void);
};

NrVolMeter::NrVolMeter
    ( XxDrawable *pParent
    , int XPos, int YPos
    )
    : XxMeter
        ( "NrVolMeter"
        , pParent
        , XPos, YPos
        , BufWidth, BufHeight
        , 0, 100
        , 1
        , BufStep
        )
{
    int L012pct = BufStep * 2;
    int L087pct = BufStep * 7;
    int L112pct = BufStep * (NrSteps - 7);
    int L187pct = BufStep * (NrSteps - 2);
    int VSize   = BufHeight;

    XxPixmap TmpMapOn  ("NrVolMeter_On",  BufWidth, BufHeight);
    XxPixmap TmpMapOff ("NrVolMeter_Off", BufWidth, BufHeight);

    DrawGC.SetForeground (XxYellow4);
    TmpMapOn.FillRectangle  (DrawGC, 0,       0, BufWidth,          BufHeight);
    DrawGC.SetForeground (XxYellow1);
    TmpMapOff.FillRectangle (DrawGC, 0,       0, BufWidth,          BufHeight);

    DrawGC.SetForeground (XxBlack);
    for (XPos = 0; XPos < BufWidth; XPos += BufStep) {
        TmpMapOn.DrawLine  (DrawGC, XPos, 0, XPos, BufHeight);
        TmpMapOff.DrawLine (DrawGC, XPos, 0, XPos, BufHeight);
    };

    SetOnPixmap  (TmpMapOn);
    SetOffPixmap (TmpMapOff);
};

NrVolMeter::~NrVolMeter (void)
{
};

class NrBufMeter : public XxMeter {
private:
    static const int BufStep   = 4;
    static const int NrSteps   = 20;
    static const int BufWidth  = NrSteps * BufStep + 1;
    static const int BufHeight = 5;
public:
    NrBufMeter (XxDrawable *pParent, int XPos, int YPos);
    virtual ~NrBufMeter (void);
};

NrBufMeter::NrBufMeter
    ( XxDrawable *pParent
    , int XPos, int YPos
    )
    : XxMeter
        ( "NrBufMeter"
        , pParent
        , XPos, YPos
        , BufWidth, BufHeight
        , 0, 200
        , 1
        , BufStep
        )
{
    int L012pct = BufStep * 2;
    int L087pct = BufStep * 7;
    int L112pct = BufStep * (NrSteps - 7);
    int L187pct = BufStep * (NrSteps - 2);
    int VSize   = BufHeight;

    XxPixmap TmpMapOn  ("NrBufMeter_On",  BufWidth, BufHeight);
    XxPixmap TmpMapOff ("NrBufMeter_Off", BufWidth, BufHeight);

    DrawGC.SetForeground (XxRed4);
    TmpMapOn.FillRectangle  (DrawGC, 0,       0, BufWidth,          BufHeight);
    DrawGC.SetForeground (XxRed1);
    TmpMapOff.FillRectangle (DrawGC, 0,       0, BufWidth,          BufHeight);

    DrawGC.SetForeground (XxYellow4);
    TmpMapOn.FillRectangle  (DrawGC, L012pct, 0, L187pct - L012pct, BufHeight);
    DrawGC.SetForeground (XxYellow1);
    TmpMapOff.FillRectangle (DrawGC, L012pct, 0, L187pct - L012pct, BufHeight);

    DrawGC.SetForeground (XxGreen4);
    TmpMapOn.FillRectangle  (DrawGC, L087pct, 0, L112pct - L087pct, BufHeight);
    DrawGC.SetForeground (XxGreen1);
    TmpMapOff.FillRectangle (DrawGC, L087pct, 0, L112pct - L087pct, BufHeight);

    DrawGC.SetForeground (XxBlack);
    for (XPos = 0; XPos < BufWidth; XPos += BufStep) {
        TmpMapOn.DrawLine  (DrawGC, XPos, 0, XPos, BufHeight);
        TmpMapOff.DrawLine (DrawGC, XPos, 0, XPos, BufHeight);
    };

    SetOnPixmap  (TmpMapOn);
    SetOffPixmap (TmpMapOff);
};

NrBufMeter::~NrBufMeter (void)
{
};

/*===========================================================================*/

class NrAdjustMeter : public XxMeter {
private:
    static const int BufStep   = 4;
    static const int NrSteps   = 20;
    static const int BufWidth  = NrSteps * BufStep + 1;
    static const int BufHeight = 5;
public:
    NrAdjustMeter (XxDrawable *pParent, int XPos, int YPos);
    virtual ~NrAdjustMeter (void);
};

NrAdjustMeter::NrAdjustMeter
    ( XxDrawable *pParent
    , int XPos, int YPos
    )
    : XxMeter
        ( "NrAdjustMeter"
        , pParent
        , XPos, YPos
        , BufWidth, BufHeight
        , -100, 100
        , 1
        , BufStep
        )
{
    int Lm06pct = BufStep * 1;
    int Lp94pct = BufStep * (NrSteps - 1);
    int VSize   = BufHeight;

    XxPixmap TmpMapOn  ("NrAdjustMeter_On",  BufWidth, BufHeight);
    XxPixmap TmpMapOff ("NrAdjustMeter_Off", BufWidth, BufHeight);

    DrawGC.SetForeground (XxRed4);
    TmpMapOn.FillRectangle  (DrawGC, 0,       0, BufWidth,          BufHeight);
    DrawGC.SetForeground (XxRed1);
    TmpMapOff.FillRectangle (DrawGC, 0,       0, BufWidth,          BufHeight);

    DrawGC.SetForeground (XxYellow4);
    TmpMapOn.FillRectangle  (DrawGC, Lm06pct, 0, Lp94pct - Lm06pct, BufHeight);
    DrawGC.SetForeground (XxYellow1);
    TmpMapOff.FillRectangle (DrawGC, Lm06pct, 0, Lp94pct - Lm06pct, BufHeight);

    DrawGC.SetForeground (XxBlack);
    for (XPos = 0; XPos < BufWidth; XPos += BufStep) {
        TmpMapOn.DrawLine  (DrawGC, XPos, 0, XPos, BufHeight);
        TmpMapOff.DrawLine (DrawGC, XPos, 0, XPos, BufHeight);
    };

    SetOnPixmap  (TmpMapOn);
    SetOffPixmap (TmpMapOff);
};

NrAdjustMeter::~NrAdjustMeter (void)
{
};

/*===========================================================================*/

class NrRecFrontend : public XxWindow, public NrRecConnection {
    class TuneUpButton;
    class TuneDownButton;
    class RecordButton;
    class MuteButton;
    class VolUpButton;
    class VolDownButton;
    class MemoryButton;
    class PresetButton;

    friend TuneUpButton;
    friend TuneDownButton;
    friend RecordButton;
    friend MuteButton;
    friend VolUpButton;
    friend VolDownButton;
    friend MemoryButton;
    friend PresetButton;
private:
    int            RecFlag;
    EzString       StationName;
    EzString       Message;
    NrPresetMemory PresetMem;

    void UpdateInfoDisplay (void);
    void SetStationName    (EzString Name);
    void SetMessage        (EzString Mesg);

    typedef tXxTimer<NrRecFrontend> FrontendTimer;
    friend FrontendTimer;

    typedef tXxLcdScrollDisplay<NrRecFrontend> RecScrollDisplay;
    friend RecScrollDisplay;

    void ClearPreset (void);

    void IntTuneUp   (void);
    void IntTuneDown (void);

    int Volume;

    class VolUpButton : public NrPulseButton {
    private:
    protected:
        void OnPulse (void);
    public:
        VolUpButton
            ( XxWindow *pParent
            , NrRecFrontend *pFrontend
            , int XPos, int YPos
            , int MsPulse
            )
            : NrPulseButton
                ( "VolUpButton"
                , pParent
                , pFrontend
                , XPos, YPos, pmButtonWidth, pmButtonHeight, MsPulse
                ) {
            SetLabel ("+");
        };
        virtual ~VolUpButton (void) { };
    };

    class VolDownButton : public NrPulseButton {
    private:
    protected:
        void OnPulse (void);
    public:
        VolDownButton
            ( XxWindow *pParent
            , NrRecFrontend *pFrontend
            , int XPos, int YPos
            , int MsPulse
            )
            : NrPulseButton
                ( "VolDownButton"
                , pParent
                , pFrontend
                , XPos, YPos, pmButtonWidth, pmButtonHeight, MsPulse
                ) {
            SetLabel ("-");
        };
        virtual ~VolDownButton (void) { };
    };

    class TuneUpButton : public NrRecButton {
    private:
    protected:
        void OnClick (void) { pFrontend->IntTuneUp (); };
    public:
        TuneUpButton
            ( XxWindow *pParent
            , NrRecFrontend *pFrontend
            , int XPos, int YPos
            )
            : NrRecButton
                ( "TuneUpButton"
                , pParent
                , pFrontend
                , XPos, YPos, pmButtonWidth, pmButtonHeight
                ) {
            SetLabel ("+");
        };
        virtual ~TuneUpButton (void) { };
    };

    class TuneDownButton : public NrRecButton {
    private:
    protected:
        void OnClick (void) { pFrontend->IntTuneDown (); };
    public:
        TuneDownButton
            ( XxWindow *pParent
            , NrRecFrontend *pFrontend
            , int XPos, int YPos
            )
            : NrRecButton
                ( "TuneDownButton"
                , pParent
                , pFrontend
                , XPos, YPos, pmButtonWidth, pmButtonHeight
                ) {
            SetLabel ("-");
        };
        virtual ~TuneDownButton (void) { };
    };

    class MuteButton : public NrRecButton {
    private:
    protected:
        void OnClick (void);
    public:
        MuteButton
            ( XxWindow *pParent
            , NrRecFrontend *pFrontend
            , int XPos, int YPos
            )
            : NrRecButton
                ( "MuteButton"
                , pParent
                , pFrontend
                , XPos, YPos, MutButtonWidth, MutButtonHeight
                ) {
            SetLabel ("mute");
        };
        virtual ~MuteButton (void) { };
    };

    class MemoryButton : public NrRecButton {
    private:
    protected:
        void OnClick (void);
    public:
        MemoryButton
            ( XxWindow *pParent
            , NrRecFrontend *pFrontend
            , int XPos, int YPos
            )
            : NrRecButton
                ( "MemoryButton"
                , pParent
                , pFrontend
                , XPos, YPos, MemButtonWidth, MemButtonHeight
                ) {
            SetLabel ("mem");
        };
        virtual ~MemoryButton (void) { };
    };

    class PresetButton : public NrRecButton {
    private:
        int PresetId;
    protected:
        void OnClick (void);
    public:
        PresetButton
            ( XxWindow *pParent
            , NrRecFrontend *pFrontend
            , int XPos, int YPos
            , int PresetId
            )
            : NrRecButton
                ( EzString ("PresetButton") + EzString (PresetId)
                , pParent
                , pFrontend
                , XPos, YPos, pButtonWidth, pButtonHeight
                ) {
            PresetButton::PresetId = PresetId;
            SetLabel (EzString ("P") + EzString (PresetId));
        };
        virtual ~PresetButton (void) { };
    };

    class RecordButton : public NrRecButton {
    private:
    protected:
        void OnClick (void);
    public:
        RecordButton
            ( XxWindow *pParent
            , NrRecFrontend *pFrontend
            , int XPos, int YPos
            )
            : NrRecButton
                ( "RecordButton"
                , pParent
                , pFrontend
                , XPos, YPos, RecButtonWidth, RecButtonHeight
                ) {
            SetLabel ("record");
        };
        virtual ~RecordButton (void) { };
    };

    XxPanel            ButtonPanel;

    FrontendTimer      RefreshTimer;
    TuneUpButton       tUpButton;
    TuneDownButton     tDownButton;
    VolUpButton        vUpButton;
    VolDownButton      vDownButton;
    RecordButton       RecButton;
    MuteButton         MutButton;
    MemoryButton       MemButton;
    PresetButton       *pPresButton[pButtonCount];
    XxFlashLight       *pPresLight[pButtonCount];

    NrRecDisplay       RecDisplay;
    NrGraphicAnalyzer  Analyzer;
    NrVolMeter         VolMeter;
    NrBufMeter         BufMeter;
    NrAdjustMeter      AdjustMeter;

    XxLcdNumDisplay    FreqDisplay;
    RecScrollDisplay   StationDisplay;
    RecScrollDisplay   InfoDisplay;

    XxFlashLight       RecLight;
    XxFlashLight       MissLight;
    XxFlashLight       kHz8Light;
    XxFlashLight       kHz16Light;
    XxFlashLight       ChargeLight;
    XxFlashLight       MuteLight;
    XxFlashLight       MemLight;

    int                CurPreset;
    int                CurBufLevel;

    void DoVolAdjust      (int AdjVal);
    void DoRecord         (void);
    void DoMute           (void);

    void DoMem            (void);
    void DoPreset         (int PresetId);

    void SetFreq          (int Freq, EzString Description = EzString (""));
    void RefreshMeters    (void);

    void HandleSignal     (int Freq, EzString Description);
    void HandleNoSignal   (int Freq);
    void HandleInitDone   (void);
    void HandleScrollDone (void);
protected:
    virtual void HandleMissData  (int Flag);
    virtual void HandlePrepare   (int Flag);
    virtual void HandleExpansion (int Expansion);
    virtual void HandleMute      (int Flag);
    virtual void HandleMessage (NrMsgCode MsgCode, EzString Id, EzString Mesg);
public:
    NrRecFrontend (EzString ConnectString, char SampleRate, EzString Driver, EzString Device);
    virtual ~NrRecFrontend (void);
};

static void PutBitMap
    ( XxDrawable     &Drawable
    , EzString      TmpName
    , int           XPos,  int YPos
    , int           Width, int Height
    , unsigned char *Bits
    )
{
    XxGC     TmpGC;

    XxPixmap TmpMap
        (TmpName, Width, Height, Bits, XxRed4, XxBlack);

    Drawable.CopyArea
        (TmpGC, &TmpMap, 0, 0, Width, Height, XPos, YPos);
};


NrRecFrontend::NrRecFrontend (EzString ConnectString, char SampleRate, EzString Driver, EzString Device)
    : XxWindow ( "NrRecFrontend", NULL, 100, 100
               , NrRecFrontendWidth, NrRecFrontendHeight
               , EzString ("NetStreamer ") + EzString (NR_VERSION)
               )
    , PresetMem   (pButtonCount + 1)
    , ButtonPanel ( "ButtonPanel", this, ButtonPanelPosX, ButtonPanelPosY
                  , ButtonPanelWidth, ButtonPanelHeight
                  )
    , NrRecConnection
                  ( 0, SampleRate, Driver, Device
                  , EzString ("NrRecFrontend ") + EzString (NR_VERSION)
                  )
    , tUpButton   (&ButtonPanel, this, tUpButtonPosX,   tUpButtonPosY  )
    , tDownButton (&ButtonPanel, this, tDownButtonPosX, tDownButtonPosY)
    , vUpButton   (&ButtonPanel, this, vUpButtonPosX,   vUpButtonPosY  , 200)
    , vDownButton (&ButtonPanel, this, vDownButtonPosX, vDownButtonPosY, 200)
    , RecButton   (&ButtonPanel, this, RecButtonPosX,   RecButtonPosY  )
    , MutButton   (&ButtonPanel, this, MutButtonPosX,   MutButtonPosY  )
    , MemButton   (&ButtonPanel, this, MemButtonPosX,   MemButtonPosY  )

    , RecDisplay ( "RecDisplay", this
                 , RecDisplayPosX, RecDisplayPosY
                 , RecDisplayWidth, RecDisplayHeight, XxBlack)
    , VolMeter   (&RecDisplay, VolMeterPosX,   VolMeterPosY  )
    , BufMeter   (&RecDisplay, BufMeterPosX,   BufMeterPosY  )
    , AdjustMeter (&RecDisplay, AdjustMeterPosX, AdjustMeterPosY)
    , Analyzer ( "Analyzer", &RecDisplay, GetAnalyzer ()
               , AnalyzerPosX + 2, AnalyzerPosY, 8, 7, 3, 30)
    , RefreshTimer (this, REFRESH_DELAY)
    , CurBufLevel (0)
    , FreqDisplay ( "FreqDisplay", &RecDisplay
                  , FreqDisplayPosX, FreqDisplayPosY, 4, 2, 2
                  , XxRed4, XxRed1, XxBlack)
    , StationDisplay ( this, "Station", &RecDisplay
                     , StationDisplayPosX, StationDisplayPosY, 9
                     , XxRed4, XxRed1, 200)
    , InfoDisplay ( this, "Info", &RecDisplay
                  , InfoDisplayPosX, InfoDisplayPosY, 16, XxRed4, XxRed1, 200)
    , MissLight ( "MissLight", &RecDisplay
               , GetLightPanelPosX (0), LightPanelPosY
               , NrMiss_width, NrMiss_height
               , NrMiss_bits
               , XxRed4, XxRed1, XxBlack
               )
    , kHz8Light ( "8kHzLight", &RecDisplay
               , GetLightPanelPosX (1), LightPanelPosY
               , Nr8kHz_width, Nr8kHz_height
               , Nr8kHz_bits
               , XxRed4, XxRed1, XxBlack
               )
    , kHz16Light ( "16kHzLight", &RecDisplay
               , GetLightPanelPosX (2), LightPanelPosY
               , Nr16kHz_width, Nr16kHz_height
               , Nr16kHz_bits
               , XxRed4, XxRed1, XxBlack
               )
    , ChargeLight
               ( "ChargeLight", &RecDisplay
               , GetLightPanelPosX (3), LightPanelPosY
               , NrCharge_width, NrCharge_height
               , NrCharge_bits
               , XxRed4, XxRed1, XxBlack
               )
    , MuteLight
               ( "MuteLight", &RecDisplay
               , GetLightPanelPosX (4), LightPanelPosY
               , NrMute_width, NrMute_height
               , NrMute_bits
               , XxRed4, XxRed1, XxBlack
               , 500
               )
    , MemLight
               ( "MemLight", &RecDisplay
               , GetLightPanelPosX (5), LightPanelPosY
               , NrMem_width, NrMem_height
               , NrMem_bits
               , XxRed4, XxRed1, XxBlack
               , 500
               )
    , RecLight ( "RecLight", &RecDisplay
               , GetLightPanelPosX (6), LightPanelPosY
               , NrRecord_width, NrRecord_height
               , NrRecord_bits
               , XxRed4, XxRed1, XxBlack
               , 500
               )
{
    RecFlag   = 0;
    CurPreset = 0;
    Volume    = 80;

    if (!Connect (ConnectString)) {
        cerr << "Cannot Connect to " << ConnectString << endl;
        Remove ();
        return;
    };

    {
        int i;
        for (i = 0; i < pButtonCount; i++) {
            pPresButton[i]
                = new PresetButton
                    (&ButtonPanel, this, pButtonPosX(i), pButtonPosX, i + 1);
            pPresLight[i]
                = new XxFlashLight
                    ( "RecLight", &RecDisplay
                    , VolMeterPosX + 86 + i * 26
                    , VolMeterPosY + 4
                    , PresetBitMapData[i].Width
                    , PresetBitMapData[i].Height
                    , (unsigned char *)PresetBitMapData[i].Data
                    , XxRed4, XxRed1, XxBlack
                    );
        };
    };

    DrawGC.SetForeground (XxGray1);
    ButtonPanel.FillRectangle
        (DrawGC, 0, 0, ButtonPanelWidth, ButtonPanelHeight);

    {
        XxGC     TmpGC;
/*
        XxPixmap TmpMap
            ("TmpLogo", NetStreamer_width, NetStreamer_height, NetStreamer_bits, XxWhite, XxGray1);

        ButtonPanel.CopyArea
            ( TmpGC, &TmpMap, 0, 0
            , NetStreamerWidth, NetStreamerHeight
            , NetStreamerPosX, NetStreamerPosY
            );
*/
        XxBitmap TmpMap
            ( "TmpLogo"
            , NetStreamer_width, NetStreamer_height, NetStreamer_bits
            );
        TmpGC.SetClipMask (NetStreamerPosX, NetStreamerPosY, &TmpMap);

        TmpGC.SetForeground (XxGray5); // qqq

        ButtonPanel.FillRectangle
            ( TmpGC
            , NetStreamerWidth, NetStreamerHeight
            , NetStreamerPosX, NetStreamerPosY
            );
    }

    {
        XxPixmap TmpMap ("TmpLabel", Label1Width, Label1Height);

        DrawGC.SetForeground (XxGray2);
        DrawGC.SetForeground (XxBlack); // qqqq
        ButtonPanel.FillRectangle
            ( DrawGC
            , Border1PosX, Border1PosY, Border1Width, Border1Height
            );
        ButtonPanel.FillRectangle
            ( DrawGC
            , Border2PosX, Border2PosY, Border2Width, Border2Height
            );
        ButtonPanel.FillRectangle
            ( DrawGC
            , Border3PosX, Border3PosY, Border3Width, Border3Height
	    );

        DrawGC.SetForeground (XxGray2);
        ButtonPanel.DrawRectangle
            ( DrawGC
            , Border1PosX, Border1PosY, Border1Width, Border1Height
            );
        ButtonPanel.DrawRectangle
            ( DrawGC
            , Border2PosX, Border2PosY, Border2Width, Border2Height
            );
        ButtonPanel.DrawRectangle
            ( DrawGC
            , Border3PosX, Border3PosY, Border3Width, Border3Height
            );


        DrawGC.SetForeground (XxBlack);
        TmpMap.FillRectangle (DrawGC, 0, 0, Label1Width, Label1Height);
        TmpMap.SetLabel ("tuning");
        ButtonPanel.CopyArea
            ( DrawGC, &TmpMap, 0, 0, Label1Width, Label1Height
            , Label1PosX, Label1PosY
            );
        TmpMap.FillRectangle (DrawGC, 0, 0, Label1Width, Label1Height);
        TmpMap.SetLabel ("volume");
        ButtonPanel.CopyArea
            ( DrawGC, &TmpMap, 0, 0, Label2Width, Label2Height
            , Label2PosX, Label2PosY
            );
    }

    PutBitMap
        ( RecDisplay, "MHz", NrMHzPosX, NrMHzPosY
        , NrMhz_width, NrMhz_height, NrMhz_bits
        );

    PutBitMap
        ( RecDisplay, "RecDisplay", AnalyzerPosX, AnalyzerPosY + 32
        , NrAnalyzer_width, NrAnalyzer_height, NrAnalyzer_bits
        );

    PutBitMap
        ( RecDisplay, "MHz", VolMeterPosX, VolMeterPosY + 7
        , NrVolume_width, NrVolume_height, NrVolume_bits
        );

    PutBitMap
        ( RecDisplay, "MHz", BufMeterPosX, BufMeterPosY + 7
        , NrBuffer_width, NrBuffer_height, NrBuffer_bits
        );

    PutBitMap
        ( RecDisplay, "MHz", AdjustMeterPosX, AdjustMeterPosY + 7
        , NrAdjust_width, NrAdjust_height, NrAdjust_bits
        );

    SetBackground (XxGray1); // qqqq

    VolMeter.SetCurVal (Volume);

    SetMute (0);
};

void NrRecFrontend::UpdateInfoDisplay (void)
{
    if (Message != EzString ("")) {
        InfoDisplay.SetScrollVal
            (EzString ("  ") + Message + EzString ("  "));
    };
};

void NrRecFrontend::SetStationName (EzString Name)
{
    if (StationName != Name) {
        StationName = Name;
        StationDisplay.SetVal (StationName);
    };
};

void NrRecFrontend::SetMessage (EzString Mesg)
{
    if (Message != Mesg) {
        Message = Mesg;
        UpdateInfoDisplay ();
    };
};

void NrRecFrontend::HandleMissData (int Flag)
{
    MissLight.SetOnFlag (Flag);
};

void NrRecFrontend::HandlePrepare (int Flag)
{
    ChargeLight.SetOnFlag (Flag);
};

void NrRecFrontend::HandleExpansion (int Expansion)
{
    kHz16Light.SetOnFlag (Expansion <  1);
    kHz8Light.SetOnFlag  (Expansion >= 1);
};

void NrRecFrontend::HandleMute (int Flag)
{
    MuteLight.SetOnFlag (Flag);
};

void NrRecFrontend::HandleMessage
    (NrMsgCode MsgCode, EzString Id, EzString Mesg)
{
    if (Id == MsgStrDisplay) SetMessage (Mesg);
};

void NrRecFrontend::SetFreq (int Freq, EzString Description)
{
    PresetMem.SetVal (0, Freq);
    DrawGC.SetForeground (XxGray5);
    FreqDisplay.SetVal (Freq);
    SetStationName (Description);
    DrawGC.SetForeground (XxBlack);
};

void NrRecFrontend::HandleSignal (int Freq, EzString Description)
{
    SetFreq (Freq, Description);
};

void NrRecFrontend::HandleNoSignal (int Freq)
{
    SetFreq (Freq);
    MissLight.SetOnFlag (0);
};

void NrRecFrontend::HandleInitDone  (void)
{
    int Freq;

    PresetMem.GetVal (0, Freq);

    DoTune (Freq);
};

NrRecFrontend::~NrRecFrontend (void)
{
    int i;
    for (i = 0; i < pButtonCount; i++) {
        delete pPresButton [i];
        delete pPresLight  [i];
    };
};

static int TmpDelayCnt = 0;
static int TmpCurVal   = 0;

void NrRecFrontend::DoVolAdjust (int AdjVal)
{
    Volume += AdjVal;
    if (Volume < 0) Volume = 0;
    if (Volume > 99) Volume = 99;

    SetVolume (1000 + (Volume * Volume * 64535) / 10000);
    VolMeter.SetCurVal (Volume);
};

void NrRecFrontend::DoRecord (void)
{
    RecFlag = !RecFlag;

    SetRecording (RecFlag);
    RecLight.SetOnFlag (RecFlag);
};

void NrRecFrontend::DoMute (void)
{
    SetMute (-1);
};

void NrRecFrontend::DoMem (void)
{
    int Flag;

    MemLight.GetOnFlag (Flag);
    MemLight.SetOnFlag (!Flag);
};

void NrRecFrontend::DoPreset (int PresetId)
{
    int Flag, Freq;

    if (PresetId != CurPreset) {
        ClearPreset ();

        CurPreset = PresetId;

        pPresLight[CurPreset - 1]->SetOnFlag (1);
    };

    MemLight.GetOnFlag (Flag);

    if (Flag) {
        PresetMem.GetVal (0, Freq);
        PresetMem.SetVal (PresetId, Freq);
        MemLight.SetOnFlag (0);
    } else {
        PresetMem.GetVal (PresetId, Freq);

        DoTune (Freq);
    };
};

void NrRecFrontend::HandleScrollDone (void)
{
    SetMessage (EzString (""));
};

void NrRecFrontend::RefreshMeters   (void)
{
    int TmpAdjust, MsCurTime;

    MsCurTime = XxTimer::GetMsCurTime ();

    // TmpLevel = Get8Log (GetMaxLevel (MsCurTime));

    CurBufLevel = ((CurBufLevel << 2)  + GetBufRate () - CurBufLevel) >> 2;
    TmpAdjust   = (GetAdjustRate () * 1000) >> 10;

    if (TmpAdjust < -90) TmpAdjust = -90;

    BufMeter.SetCurVal (CurBufLevel);
    AdjustMeter.SetCurVal (TmpAdjust);

    Analyzer.Refresh (MsCurTime);
};

template <>
void NrRecFrontend::FrontendTimer::HandleTimeOut (int Count)
{
    pOwner->RefreshMeters ();
};

template <>
void NrRecFrontend::RecScrollDisplay::HandleScrollDone (void)
{
    pOwner->HandleScrollDone ();
};

void NrRecFrontend::ClearPreset (void)
{
    if (CurPreset != 0) {
        pPresLight[CurPreset - 1]->SetOnFlag (0);
        CurPreset = 0;
    };
};

void NrRecFrontend::IntTuneUp (void)
{
    int Freq;

    PresetMem.GetVal (0, Freq);

    DoTuneUp (Freq + 1);

    ClearPreset ();
};

void NrRecFrontend::IntTuneDown (void)
{
    int Freq;

    PresetMem.GetVal (0, Freq);

    DoTuneDown (Freq - 1);

    ClearPreset ();
};

void NrRecFrontend::VolUpButton::OnPulse (void)
{
    pFrontend->DoVolAdjust (3);
};

void NrRecFrontend::VolDownButton::OnPulse (void)
{
    pFrontend->DoVolAdjust (-3);
};

void NrRecFrontend::RecordButton::OnClick (void)
{
    pFrontend->DoRecord ();
};

void NrRecFrontend::MuteButton::OnClick (void)
{
    pFrontend->DoMute ();
};

void NrRecFrontend::MemoryButton::OnClick (void)
{
    pFrontend->DoMem ();
};

void NrRecFrontend::PresetButton::OnClick (void)
{
    pFrontend->DoPreset (PresetId);
};

int main (int argc, char *argv[])
{
    int      RetVal;

    try {
        EzString ConnectString, SampleRate, Driver, Device;

        ConnectString = EzString (argc < 2 ? ":8888"    : argv[1]);
        Driver        = EzString (argc < 3 ? "Oss"      : argv[2]);
        Device        = EzString (argc < 4 ? "/dev/dsp" : argv[3]);

        new NrRecFrontend (ConnectString, 1, Driver, Device);

        RetVal = XxApplication::MainLoop ();
    } catch (Exception e) {
        cerr << e.GetText() << endl;
	RetVal = 1;
    };

    return RetVal;
};
