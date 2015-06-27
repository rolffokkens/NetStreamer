#include <iostream>
#include "XxStdio.h"

using namespace std;

class TestStdIn : public XxStdIn {
    virtual void HandleRead (EzString Text);
};

void TestStdIn::HandleRead (EzString Text)
{
    cout << Text << endl;
    Remove ();
};

int main (void)
{
    TestStdIn *pIn;

    pIn = new TestStdIn;

    XxBlocker::MainLoop ();

    return 0;
};

