/*
 * file: XxSocket.cpp
 *
 * This file is part of the XxStdLib library which is developed to support
 * the development of NetStreamer. This file is distributed under the
 * GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "XxSocket.h"
#include "BiLink.h"

//
// The next is a hack to allow compilation on both libc5 and libc6/glibc2
//
// glibc2 defines _SOCKETBITS_H and libc5 doesn't
//
#ifndef _SOCKETBITS_H
typedef int socklen_t;
#endif

int XxListener::CheckListen (const XxSocketAddr &Addr)
{
    if (!SetMyAddr (Addr)) return 0;

    listen (GetFd (), 5);

    SetStatus (StatOpen);

    return 1;
};

void XxSocketAddr::Constr (int Port,  EzString Addr)
{
    int addrsize, hostaddrPtr[2];
    int tAddr, tPort;
    struct hostent *HostEnt;
    struct in_addr InAddr;

    tPort = Port;

    if (Addr == EzString ("")) {
        tAddr = INADDR_ANY;
    } else {
        if (inet_aton (Addr, &InAddr)) {
            memcpy ( (char *) &tAddr
                   , (char *) &InAddr
                   , (size_t) sizeof (tAddr));
        } else {
            HostEnt = gethostbyname (Addr);
            if (HostEnt == NULL) {
                tAddr = INADDR_ANY;
                tPort = -1;
            } else {
                memcpy ( (char *) &tAddr
                       , (char *) HostEnt->h_addr_list[0]
                       , (size_t) sizeof (tAddr));
            };
        };
    }
    XxSocketAddr::Addr = ntohl (tAddr);
    XxSocketAddr::Port = tPort;
};

XxSocketAddr::XxSocketAddr (int Port, EzString Addr)
{
    Constr (Port, Addr);
};

XxSocketAddr::XxSocketAddr (EzString Addr)
{
    EzString tAddr;
    int Pos, tPort;

    Pos = Instr (Addr, ":");
    if (Pos < 0) {
        XxSocketAddr::Addr = INADDR_ANY;
        XxSocketAddr::Port = -1;
    } else {
        tPort = int (Substr (Addr, Pos + 1));
        tAddr = Substr (Addr, 0, Pos);
        Constr (tPort, tAddr);
    };
};

sockaddr *XxSocketAddr::CreSockAddr (sockaddr_in &SockAddr) const
{
    int tAddr, tPort;

    tAddr = htonl (Addr);
    tPort = htons (Port);

    memset (&SockAddr, 0, sizeof (SockAddr));

    SockAddr.sin_family = AF_INET;
    memcpy((char *) &(SockAddr.sin_addr.s_addr),
           (char *) &tAddr,
           (size_t) sizeof (tAddr));
    SockAddr.sin_port = tPort;

    return (sockaddr *)&SockAddr;
};

XxSocketAddr::operator EzString (void) const
{
    EzString RetVal, Dot = ":";
    int    i, tAddr = Addr;

    for (i = 0; i < 4; i++) {
        RetVal = NumberToString (tAddr & 0x00ff) + Dot + RetVal;
        tAddr >>= 8;
        Dot = ".";
    };
    RetVal += NumberToString (Port);

    return RetVal;
};

int XxSocket::IntConnect (const XxSocketAddr &Addr) {
    struct sockaddr_in SockAddr;
    struct hostent *HostEnt, mkHost;
    int tPort, tAddr;
    socklen_t AddrSize = sizeof(SockAddr);

    if (!CheckFdOK ()) return 0;

    Addr.CreSockAddr (SockAddr);

    if (connect (GetFd (), (struct sockaddr *) &SockAddr, AddrSize) < 0) {
        Close ();
        return 0;
    };
    fcntl (GetFd (), F_SETFL, O_NONBLOCK);

    tPort = ntohs (SockAddr.sin_port);
    tAddr = ntohl (SockAddr.sin_addr.s_addr);

    SetHisAddr (XxSocketAddr (tPort, tAddr));

    SetStatus (StatOpen);

    OnConnect ();

    return 1;
};

int XxSocket::SetMyAddr (const XxSocketAddr &Addr) {
    sockaddr_in SockAddr;
    sockaddr    *pSockAddr;
    int         Fd;

    if (!CheckFdOK ()) return 0;

    MyAddr = Addr;
    Fd     = GetFd ();

    pSockAddr = MyAddr.CreSockAddr (SockAddr);
    if (Fd >= 0 && bind (Fd, pSockAddr, sizeof(SockAddr)) >= 0) {
        MyAddr = Addr;
        return 1;
    } else {
        MyAddr = XxSocketAddr ();
        return 0;
    };
};

int XxSocket::CheckFdOK (void) {
    if (GetFd () == -1) SetFd (CreSocket ());

    return (GetFd () != -1);
};

/****************************************************************************
*
* XxUdpSocket
*
****************************************************************************/

class XxUdpSocket::MessageQueue {
private:
    class Message;

    typedef BlSet<Message> MessageSet;

    struct Message {
        XxSocketAddr Addr;
        EzString     Data;
        MessageSet   QueueMem;

        Message (XxSocketAddr Addr, EzString Data) : QueueMem (this) {
            Message::Addr = Addr;
            Message::Data = Data;
        };
    };

    MessageSet::SetOwner QueueOwn;
protected:
public:
    MessageQueue (void) {};

    virtual ~MessageQueue (void) {};

    void PutMessage (XxSocketAddr Addr,  EzString Data );
    int  GetMessage (XxSocketAddr &Addr, EzString &Data);
    int  MessageAvail (void);
};

void XxUdpSocket::MessageQueue::PutMessage (XxSocketAddr Addr,  EzString Data )
{
    Message *pMesg;

    pMesg = new Message (Addr, Data);

    pMesg->QueueMem.Attach (QueueOwn);
};

int  XxUdpSocket::MessageQueue::GetMessage (XxSocketAddr &Addr, EzString &Data)
{
    Message *pMesg;

    pMesg = QueueOwn.GetLast ();

    if (pMesg == NULL) return 0;

    Addr = pMesg->Addr;
    Data = pMesg->Data;

    delete pMesg;

    return 1;
};

int  XxUdpSocket::MessageQueue::MessageAvail (void)
{
    return (QueueOwn.GetLast () != NULL);
};

XxUdpSocket::XxUdpSocket (void)
{
    pOutQueue = new MessageQueue;
};

XxUdpSocket::~XxUdpSocket (void)
{
    delete pOutQueue;
};

int XxUdpSocket::Bind (const XxSocketAddr &Addr)
{
    int RetVal;

    RetVal = SetMyAddr  (Addr);

    if (RetVal) SetStatus (StatOpen);

    return RetVal;
};

int XxUdpSocket::CreSocket (void)
{
    int retval, optval;

    retval = socket (PF_INET, SOCK_DGRAM,  0);

    optval = 1;
    setsockopt (retval, SOL_SOCKET, SO_BROADCAST, &optval, sizeof (optval));

    return retval;
};

static char UdpBuf [32767];

void XxUdpSocket::IntHandleRead  (void)
{
    struct sockaddr_in SockAddr;
    socklen_t AddrSize = sizeof(SockAddr);
    XxSocketAddr Addr;
    EzString     Data;
    int          Length;

    Length = recvfrom
                 ( GetFd (), UdpBuf, sizeof (UdpBuf), 0
                 , (struct sockaddr *) &SockAddr, &AddrSize
                 );
    if (Length < 0) {
        cerr << "XxUdpSocket::IntHandleRead(" << (void *)this 
             << "):WAAAAAAAAAAAH" << strerror (errno) << endl;
        exit (1);
    };

    Addr  = XxSocketAddr
                ( ntohs (SockAddr.sin_port)
                , ntohl (SockAddr.sin_addr.s_addr)
                );
    Data  = EzString (UdpBuf, Length);

    HandleRead (Addr, Data);
};

void XxUdpSocket::IntHandleWrite (void)
{
    XxSocketAddr Addr;
    EzString     Data;
    int          len;

    sockaddr_in  SockAddr;
    sockaddr     *pSockAddr;

    if (pOutQueue->GetMessage (Addr, Data)) {
        pSockAddr = Addr.CreSockAddr (SockAddr);

        len = sendto ( GetFd (), Data.Text (), Data.Length (), 0
                     , pSockAddr, sizeof(SockAddr)
                     );
    };
};

void XxUdpSocket::Write (const XxSocketAddr &Addr, EzString Data)
{
    pOutQueue->PutMessage (Addr, Data);
};

void XxUdpSocket::GetRWFlags (int &rFlag, int &wFlag)
{
    rFlag = 1;
    wFlag = (pOutQueue->MessageAvail ());
};

void XxUdpSocket::HandleAction (XxAction Action)
{
    switch (Action) {
    case ActRead:
        IntHandleRead ();
        break;
    case ActWrite:
        IntHandleWrite ();
        break;
    };
};

XxBlocker::XxAction XxUdpSocket::GetAction  (int &rFlag, int &wFlag)
{
    if (rFlag) {
        wFlag = 0;
        return ActRead;
    };
    return (wFlag ? ActWrite : ActNone);
};

/****************************************************************************/

int XxTcpSocket::CreSocket (void) {
    return socket (PF_INET, SOCK_STREAM, 0);
};

void XxListener::HandleAction (XxAction Action)
{
    if (Action == ActAccept) OnAccept ();
};

XxConnection::XxConnection
    (int Fd, const XxSocketAddr &MyAddr, const XxSocketAddr &HisAddr)
{
    XxSocketAddr *pAdr;

    XxTcpSocket::HisAddr = HisAddr;
    XxTcpSocket::MyAddr  = MyAddr;

    SetFd     (Fd);
    SetStatus (StatOpen);

    OnConnect ();
};

XxConnection *XxListener::Accept (void)
{
    XxConnection *pConnection;
    struct sockaddr_in SockAddr;
    socklen_t AddrSize = sizeof(SockAddr);
    int nFd, tFd, tPort, tAddr;
    XxSocketAddr nMyAddr, nHisAddr;

    AddrSize = sizeof(SockAddr);
    tFd = GetFd ();
    if (tFd < 0) return NULL;

    nFd = accept (tFd, (struct sockaddr *) &SockAddr, &AddrSize);

    if (nFd < 0) return NULL;

    fcntl (nFd, F_SETFL, O_NONBLOCK);

    nHisAddr = XxSocketAddr
                   ( ntohs (SockAddr.sin_port)
                   , ntohl (SockAddr.sin_addr.s_addr)
                   );

    AddrSize = sizeof(SockAddr);
    getsockname (nFd, (struct sockaddr *) &SockAddr, &AddrSize);

    nMyAddr  = XxSocketAddr
                   ( ntohs (SockAddr.sin_port)
                   , ntohl (SockAddr.sin_addr.s_addr)
                   );
    pConnection = GetNewConnection (nFd, nMyAddr, nHisAddr);

    return pConnection;
};
