#include <iostream.h>
#include "XxSocket.h"

class TestSocket : public XxUdpSocket {
private:
protected:
    virtual void HandleRead (XxSocketAddr &Addr, EzString Data) {
        cerr << "HandleRead from " << EzString (Addr)  << ":" << Data << endl;
    };
public:
    void DoTest (void) {
        Write (XxSocketAddr (EzString ("145.66.1.255:8889")), EzString ("ABCD"));
    };
};

int main (void)
{
    {
        TestSocket Sock1, Sock2;

        cerr << Sock1.Bind (XxSocketAddr (EzString (":8889"))) << endl;

//        Sock1.Connect (XxSocketAddr (EzString ("145.66.1.1:8887")));

        cerr << Sock2.Bind (XxSocketAddr (EzString (":8887"))) << endl;

        Sock2.DoTest ();

        XxBlocker::MainLoop ();
    };
};
