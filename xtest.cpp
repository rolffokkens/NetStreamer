/*
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <string.h>

#include "BiLink.h"
*/

#undef TRACE_EVENTS

#include "Exception.h"
#include "XxApplication.h"
#include "XxGC.h"
#include "XxBase.h"
#include "XxDrawable.h"
#include "XxPixmap.h"
#include "XxWindow.h"
#include "XxButton.h"

class ByeButton : public XxButton {
private:
protected:
    void OnClick (void) { Remove (); };
public:
    ByeButton (XxWindow *pParent, int XPos, int YPos, int Width, int Height)
        : XxButton ("ByeButton", pParent, XPos, YPos, Width, Height) {
        SetLabel ("Remove Button");
    };
    virtual ~ByeButton (void) { };
};

class NewButton : public XxButton {
private:
protected:
    void OnClick (void) { new ByeButton (NULL, 0, 0, 100, 30); };
public:
    NewButton (XxWindow *pParent, int XPos, int YPos, int Width, int Height)
        : XxButton ("NewButton", pParent, XPos, YPos, Width, Height) {
        SetLabel ("Another Button");
    };
    virtual ~NewButton (void) { };
};

class TestButton : public XxButton {
private:
    int FlipFlag;
protected:
    void OnClick (void) {
        if (FlipFlag == 0 ) {
            SetLabel ("Well Done!");
            FlipFlag = 1;
        };
    };
public:
    TestButton (XxWindow *pParent, int XPos, int YPos, int Width, int Height)
        : XxButton ("TestButton", pParent, XPos, YPos, Width, Height) {
        FlipFlag = 0;
        SetLabel ("Push Here!");
    };
    virtual ~TestButton (void) { };
};

class TopWindow : public XxWindow {
private:
    class SubWindow : public XxWindow {
    protected:
        virtual void OnExpose (int XPos, int YPos, int Width, int Height);
    public:
        SubWindow (TopWindow *pTop);
        virtual ~SubWindow (void) { };
    };
    SubWindow  SubWin;
    TestButton But;
public:
    TopWindow (void);
    virtual ~TopWindow (void) { };
};

TopWindow::TopWindow (void)
    : XxWindow ("TopWindow", NULL, 100, 100, 100, 130)
    , SubWin (this)
    , But (this, 10, 100, 80, 20)
{
    XxColor Tmp (0, 0, 0);
    Tmp = XxGray4;

    SetBackground (XxGray4);
};

TopWindow::SubWindow::SubWindow (TopWindow *pTop)
    : XxWindow ("SubWindow", pTop, 10, 10, 80, 80)
{
};

void TopWindow::SubWindow::OnExpose (int XPos, int YPos, int Width, int Height)
{
    // XxGC      tGC;
    XGCValues tGCval;
    XxPixmap  Pixmap1 ("Pixmap1", GetWidth (), GetHeight ());
    int       W, H;

    W = GetWidth ();
    H = GetHeight ();

    DrawGC.SetForeground (XxGray4);

    Pixmap1.FillRectangle (DrawGC, 0, 0, W, H);

    DrawGC.SetForeground (XxWhite);
    DrawGC.SetLineWidth  (4);
    Pixmap1.DrawLine (DrawGC, 0, 0, W, H);

    DrawGC.SetLineWidth  (2);
    DrawGC.SetForeground (XxGray6);
    Pixmap1.DrawLine (DrawGC, 0, 1, W, 1);
    Pixmap1.DrawLine (DrawGC, 1, 0, 1, H);
    DrawGC.SetForeground (XxGray2);
    Pixmap1.DrawLine (DrawGC, W-1, H, W-1, 0);
    Pixmap1.DrawLine (DrawGC, W, H-1, 0, H-1);

    DrawGC.SetForeground (XxGray6);
    Pixmap1.DrawString (DrawGC, 10, 10, "Test");

    CopyArea
        ( DrawGC, &Pixmap1
        , 0, 0, GetWidth (), GetHeight ()
        , 0, 0
        );
};

int main (void)
{
    XxPixmap *pXpm;

/*
    TopWindow  TopWin;
    XxButton   SillyButton (NULL, 10, 10, 100, 30);
    TestButton testButton  (NULL, 10, 10, 100, 30);

    XxButton   *pBye = new NewButton (NULL, 10, 10, 100, 30);
*/
/*
    NrRecConnection RecConnection (918);

    if (!RecConnection.Connect (":8888")) {
        cerr << "Cannot Connect to :8888" << endl;
        return 1;
    };
*/

    // try {
//        pXpm = new XxPixmap ("testXpm", xtest_xpm);
//        delete pXpm;

        new TopWindow;
        new NewButton  (NULL, 10, 10, 100, 30);
        new TestButton (NULL, 10, 10, 100, 30);
        XxButton   *pBut = new XxButton ("Button", NULL, 10, 10, 100, 30);

        pBut->SetLabel ("Silly huh?");

        new TopWindow;

        return XxApplication::MainLoop ();
    // } catch (Exception Ex) {
    //     cout << "Unhandled exception [" << Ex.GetText () << "]" << endl;
    //     return 1;
    // };
};
