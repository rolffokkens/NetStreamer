/*
 * file: XxSocket.h
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#ifndef H_XX_SOCKET

#define H_XX_SOCKET

#include <netinet/in.h>
#include <unistd.h>

#include "EzString.h"
#include "XxBlocker.h"
#include "XxStream.h"

class XxSocket;
class XxUdpSocket;
class XxTcpSocket;
class XxListener;
class XxConnection;
class XxIntSelector;

class XxSocketAddr {
private:
    void Constr (int Port, EzString Addr);
public:
    int Addr;
    int Port;
    XxSocketAddr (int Port = -1, int Addr = INADDR_ANY) {
        XxSocketAddr::Addr = Addr;
        XxSocketAddr::Port = Port;
    };
    XxSocketAddr (int Port, EzString Addr);
    XxSocketAddr (EzString Addr);
    sockaddr *CreSockAddr (sockaddr_in &SockAddr) const;
    operator EzString (void) const;
};

class XxSocket : virtual public XxBlocker {
    friend XxUdpSocket;
    friend XxConnection;
private:
    XxSocketAddr HisAddr;
    XxSocketAddr MyAddr;

    int IntConnect (const XxSocketAddr &Addr);
protected:
    virtual int CreSocket () = 0;

    int CheckFdOK (void);

    int  SetMyAddr  (const XxSocketAddr &Addr);
    void SetHisAddr (const XxSocketAddr &Addr) { HisAddr = Addr; };

    virtual void OnConnect (void) { };
public:
    XxSocketAddr GetHisAddr (void) { return HisAddr; };
    XxSocketAddr GetMyAddr  (void) { return MyAddr ; };

    XxSocket (void) { };

    virtual ~XxSocket (void) { };
};

class XxUdpSocket : public XxSocket {
private:
    class MessageQueue;

    long     InCount, OutCount;

    MessageQueue *pOutQueue;
protected:
    int CreSocket ();

    virtual void IntHandleRead  (void);
    virtual void IntHandleWrite (void);

    virtual void HandleRead  (XxSocketAddr &Addr, EzString Data) { };
    virtual void Write (const XxSocketAddr &Addr, EzString Data);

    virtual void HandleAction (XxAction Action);
public:
    XxUdpSocket (void);

    virtual ~XxUdpSocket (void);

    virtual void     GetRWFlags (int &rFlag, int &wFlag);
    virtual XxAction GetAction  (int &rFlag, int &wFlag);

    int Bind (const XxSocketAddr &Addr);

    int Connect (int Port, EzString Addr) {
        return IntConnect (XxSocketAddr (Port, Addr));
    };
    int Connect (EzString Addr) {
        return IntConnect (XxSocketAddr (Addr));
    };
    int Connect (int Port, int Addr) {
        return IntConnect (XxSocketAddr (Port, Addr));
    };
    long GetInCount  (void) { return InCount;  };
    long GetOutCount (void) { return OutCount; };
};

class XxTcpSocket : public XxSocket {
    friend XxListener;
    friend XxConnection;
private:
protected:
    int CreSocket ();
    virtual void HandleAction (XxAction Action) = 0;
public:
    XxTcpSocket (void) { };

    virtual ~XxTcpSocket (void) { };

    virtual void     GetRWFlags (int &rFlag, int &wFlag) = 0;
    virtual XxAction GetAction  (int &rFlag, int &wFlag) = 0;
};

class XxListener : public XxTcpSocket {
private:
    int CheckListen (const XxSocketAddr &Addr);

    virtual XxConnection *GetNewConnection
        (int Fd, const XxSocketAddr &MyAddr, const XxSocketAddr &HisAddr);
protected:
    virtual void OnAccept (void);

    using XxTcpSocket::Remove;

    void HandleAction (XxAction Action);
public:
    XxListener (void) : XxTcpSocket () { };
    XxListener (const XxListener &Listener) : XxTcpSocket () { };

    virtual ~XxListener (void) { };

    XxListener &operator= (const XxListener &Listener) {
        XxTcpSocket::operator= (Listener);
	return *this;
    };

    virtual void GetRWFlags (int &rFlag, int &wFlag) {
        rFlag = 1;
        wFlag = 0;
    };

    virtual XxAction GetAction  (int &rFlag, int &wFlag) {
        wFlag = 0;
        return (rFlag ? ActAccept : ActNone) ;
    };

    int Listen (int Port, EzString Addr) {
        return CheckListen (XxSocketAddr (Port, Addr));
    };
    int Listen (EzString Addr) {
        return CheckListen (XxSocketAddr (Addr));
    };
    int Listen (int Port, int Addr) {
        return CheckListen (XxSocketAddr (Port, Addr));
    };
    XxConnection *Accept (void);

    long GetInCount  (void) { return 0; };
    long GetOutCount (void) { return 0; };
};

class XxConnection : virtual public XxTcpSocket, virtual public XxStream {
    friend XxListener;
private:
protected:
    XxConnection (void) : XxTcpSocket () { };

    XxConnection
        (int Fd, const XxSocketAddr &MyAddr, const XxSocketAddr &HisAddr);

    using XxTcpSocket::Remove;

    void Write (EzString Data) { XxStream::Write (Data); };

    void HandleAction (XxAction Action) { XxStream::HandleAction (Action); };
public:
    XxConnection (const XxConnection &Connector) : XxTcpSocket () { };

    virtual ~XxConnection (void) { };

    XxConnection &operator= (const XxConnection &Connector) {
        XxTcpSocket::operator= (Connector);
	return *this;
    };

    void GetRWFlags (int &rFlag, int &wFlag) {
        XxStream::GetRWFlags (rFlag, wFlag);
    };
    XxAction GetAction  (int &rFlag, int &wFlag) {
        return XxStream::GetAction  (rFlag, wFlag);
    };

    int Connect (int Port, EzString Addr) {
        return IntConnect (XxSocketAddr (Port, Addr));
    };
    int Connect (EzString Addr) {
        return IntConnect (XxSocketAddr (Addr));
    };
    int Connect (int Port, int Addr) {
        return IntConnect (XxSocketAddr (Port, Addr));
    };
};

inline XxConnection *XxListener::GetNewConnection
    (int Fd, const XxSocketAddr &MyAddr, const XxSocketAddr &HisAddr)
{
    return new XxConnection (Fd, MyAddr, HisAddr);
};

inline void XxListener::OnAccept (void)
{
    delete Accept ();
};

#endif
