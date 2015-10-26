/**
 * @file BasicTCPSocket.cpp
 * @brief Source file for class BasicTCPSocket
 * @date 23/10/2015
 * @author Giuseppe Ferrò
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class BasicTCPSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicSocket.h"
#include "BasicTCPSocket.h"
#include "Sleep.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


#define sock_errno()  errno

namespace MARTe {

static const uint32 SELECT_WIDTH= 256u;


/** Allows synchronising to a group of sockets.
CAN ONLY SELECT SOCKETS FROM A SINGLE SOURCE TYPE. CANNOT MIX ATM and UDP (on NT only?) */
class  SocketSelect {
private:
    /** */
    fd_set readFDS;

    /** */
    fd_set writeFDS;

    /** */
    fd_set exceptFDS;

    /** */
    fd_set readFDS_done;

    /** */
    fd_set writeFDS_done;

    /** */
    fd_set exceptFDS_done;

    /** */
    int32 readySockets;

public:
    /** */
    SocketSelect(){
        Reset();
    }

    /** */
    void Reset(){
        FD_ZERO(&readFDS);
        FD_ZERO(&writeFDS);
        FD_ZERO(&exceptFDS);
        FD_ZERO(&readFDS_done);
        FD_ZERO(&writeFDS_done);
        FD_ZERO(&exceptFDS_done);
    }

    /** */
    void AddWaitOnWriteReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_SET(s->Socket(),&writeFDS);
    }
    /** */
    void DeleteWaitOnWriteReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_CLR(s->Socket(),&writeFDS);
    }
    /** */
    void AddWaitOnReadReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_SET(s->Socket(),&readFDS);
    }
    /** */
    void DeleteWaitOnReadReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_CLR(s->Socket(),&readFDS);
    }
    /** */
    void AddWaitOnExceptReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_SET(s->Socket(),&exceptFDS);
    }
    /** */
    void DeleteWaitOnExceptReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_CLR(s->Socket(),&exceptFDS);
    }
    /** Wait for all the event*/
    bool Wait(TimeoutType msecTimeout = TTInfiniteWait){
        readFDS_done   = readFDS;
        writeFDS_done  = writeFDS;
        exceptFDS_done = exceptFDS;

        timeval timeWait;
        if (msecTimeout == TTInfiniteWait) {
            readySockets = select(SELECT_WIDTH,&readFDS_done,&writeFDS_done,&exceptFDS_done,NULL);
        } else {
            timeWait.tv_sec  = msecTimeout.GetTimeoutMSec() / 1000;
            timeWait.tv_usec = 1000 * (msecTimeout.GetTimeoutMSec() - (timeWait.tv_sec * 1000));
            readySockets = select(SELECT_WIDTH,&readFDS_done,&writeFDS_done,&exceptFDS_done,&timeWait);
        }
        return (readySockets > 0);
    }

    /** wait for data on the input */
    bool WaitRead(TimeoutType msecTimeout = TTInfiniteWait){
        readFDS_done   = readFDS;

        timeval timeWait;
        if (msecTimeout == TTInfiniteWait) {
            readySockets = select(SELECT_WIDTH,&readFDS_done,NULL,NULL,NULL);
        } else {
            timeWait.tv_sec  = msecTimeout.GetTimeoutMSec() / 1000;
            timeWait.tv_usec = 1000 * (msecTimeout.GetTimeoutMSec() - (timeWait.tv_sec * 1000));
            readySockets = select(SELECT_WIDTH,&readFDS_done,NULL,NULL,&timeWait);
        }
        return (readySockets > 0);
    }

    /** wait on free space on the output buffer*/
    bool WaitWrite(TimeoutType msecTimeout = TTInfiniteWait){
        writeFDS_done   = writeFDS;

        timeval timeWait;
        if (msecTimeout == TTInfiniteWait) {
            readySockets = select(SELECT_WIDTH,NULL,&writeFDS_done,NULL,NULL);
        } else {
            timeWait.tv_sec  = msecTimeout.GetTimeoutMSec() / 1000;
            timeWait.tv_usec = 1000 * (msecTimeout.GetTimeoutMSec() - (timeWait.tv_sec * 1000));
            readySockets = select(SELECT_WIDTH,NULL,&writeFDS_done,NULL,&timeWait);
        }
        return (readySockets > 0);
    }

    /** wait for an exception */
    bool WaitExcept(TimeoutType msecTimeout = TTInfiniteWait){
        exceptFDS_done = exceptFDS;

        timeval timeWait;
        if (msecTimeout == TTInfiniteWait) {
            readySockets = select(SELECT_WIDTH,NULL,NULL,&exceptFDS_done,NULL);
        } else {
            timeWait.tv_sec  = msecTimeout.GetTimeoutMSec() / 1000;
            timeWait.tv_usec = 1000 * (msecTimeout.GetTimeoutMSec() - (timeWait.tv_sec * 1000));
            readySockets = select(SELECT_WIDTH,NULL,NULL,&exceptFDS_done,&timeWait);
        }
        return (readySockets > 0);
    }

    /** */
    int32 ReadySockets(){ return readySockets; }

    /** */
    bool CheckRead(BasicSocket *s){ return (FD_ISSET(s->Socket(),&readFDS_done)!= 0); }

    /** */
    bool CheckWrite(BasicSocket *s){ return (FD_ISSET(s->Socket(),&writeFDS_done)!= 0); }

    /** */
    bool CheckExcept(BasicSocket *s){ return (FD_ISSET(s->Socket(),&exceptFDS_done)!= 0); }

    /** */
    fd_set &ReadFDS(){ return readFDS_done; }

    /** */
    fd_set &WriteFDS(){ return readFDS_done; }

    /** */
    fd_set &ExceptFDS(){ return readFDS_done; }

};




/**
 * @file
 * Allows handling ports by name
 */

class InternetService{
    /** */
    servent service;

public:
    /** */
    bool SearchByName(const char8 *name,char8 *protocol=NULL){
        servent *serv = getservbyname(name,protocol);
        if (serv == NULL) return false;
        service = *serv;
        return true;

    }

    /** */
    bool SearchByPort(int32 port,char8 *protocol=NULL){
        servent *serv = getservbyport(port,protocol);
        if (serv == NULL) return false;
        service = *serv;
        return true;

    }

    /** */
    int32 Port() {
        return service.s_port;
    }

    /** */
    const char8 *Name() {
        return service.s_name;
    }

    /** */
    const char8 *Protocol() {
        return service.s_proto;
    }

    /** */
    static int32 GetPortByName(const char8 *name){
        InternetService service;
        service.SearchByName(name);
        return service.Port();
    }
};







/** basic Read*/
bool BasicTCPSocket::BasicRead(void* buffer,
                               uint32 &size) {
    int32 ret = recv(GetConnectionSocket(), (char8 *) buffer, size, 0);
    if (ret < 0) {
        size = 0;
        return false;
    }
    size = ret;
    // to avoid polling continuously release CPU time when reading 0 bytes
    if (size == 0)
        Sleep::MSec(1);
    return true;
}

/** basic write */
bool BasicTCPSocket::BasicWrite(const void* buffer,
                                uint32 &size) {
    int32 ret = send(GetConnectionSocket(), (char8 *) buffer, size, 0);
    if (ret < 0) {
        size = 0;
        return false;
    }
    size = ret;
    return true;
}

/** Read without consuming */
bool BasicTCPSocket::Peek(void* buffer,
                          uint32 &size) {
    int32 ret = recv(GetConnectionSocket(), (char8 *) buffer, size, MSG_PEEK);
    if (ret < 0) {
        size = 0;
        return false;
    }
    size = ret;
    return true;
}


/** just a constructor */
BasicTCPSocket::BasicTCPSocket() {
    signal(SIGPIPE, SIG_IGN);
}

/** just a constructor */
BasicTCPSocket::BasicTCPSocket(int32 socket) {
    signal(SIGPIPE, SIG_IGN);
    SetConnectionSocket(socket);
}

/** Opens a stream socket */
bool BasicTCPSocket::Open() {
    InternetAddress::SocketInit();
    SetConnectionSocket(socket(PF_INET, SOCK_STREAM, 0));
    const int32 one = 1;
    if (setsockopt(GetConnectionSocket(), SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0) {
        return false;
    }

    if (GetConnectionSocket() < 0) {
        return false;
    }

    return true;
}


/** Opens a socket as a server at port port */
bool BasicTCPSocket::Listen(int32 port,
                            int32 maxConnections) {
    InternetAddress server;

    server.SetPort(port);
    int32 errorCode = bind(GetConnectionSocket(), server.GetAddress(), server.Size());
    if (errorCode < 0) {
        return false;
    }

    errorCode = listen(GetConnectionSocket(), maxConnections);
    if (errorCode < 0) {
        return false;
    }

    return true;
}

/** Opens a socket as a server at port port */
bool BasicTCPSocket::Listen(char8 *serviceName,
                            int32 maxConnections) {
    int32 port = InternetService::GetPortByName(serviceName);
    if (port == -1)
        return false;
    return Listen(port, maxConnections);
}

/** connects an unconnected socket to address address and with port port
 if msecTimeout is TTDefault then select is not used */
bool BasicTCPSocket::Connect(const char8 *address,
                             int32 port,
                             TimeoutType msecTimeout,
                             int32 retry) {
    GetDestination().SetPort(port);
    if (GetDestination().SetAddressByDotName(address) == false) {
        if (GetDestination().SetAddressByName(address) == false) {
            return false;
        }
    }

    int32 errorCode = connect(GetConnectionSocket(), GetDestination().GetAddress(), GetDestination().Size());
    if (errorCode < 0) {

        errorCode = sock_errno();
        if (errorCode == EINTR) {
            if (retry > 0) {
                return Connect(address, port, msecTimeout, retry--);
            }
            else {
                return false;
            }
        }
        if ((errorCode != 0) && (errorCode != EINPROGRESS) && (errorCode != EWOULDBLOCK)) {
            return false;
        }
        else {
            if (msecTimeout != TTDefault) {
                SocketSelect sel;
                sel.AddWaitOnWriteReady(this);
                bool ret = sel.WaitWrite(msecTimeout);
                if (ret)
                    return true;
            }
            return false;
        }
    }
    return true;
}

/** connects an unconnected socket to address address and with port port */
bool BasicTCPSocket::Connect(const char8 *address,
                             const char8 *serviceName,
                             TimeoutType msecTimeout) {
    int32 port = InternetService::GetPortByName(serviceName);
    if (port == -1)
        return false;
    return Connect(address, port, msecTimeout);
}

/** true if we are still connected  Still experimental */
bool BasicTCPSocket::IsConnected() {

    int32 errorCode;
    InternetAddress information;

    socklen_t len = information.Size();

    int32 ret = getpeername(GetConnectionSocket(), information.GetAddress(), &len);

    errorCode = sock_errno();

    return (ret == 0);

}

/** this is a BasicTCPSocket constructor .. */
BasicTCPSocket *BasicTCPSocket::WaitConnection(TimeoutType msecTimeout,
                                               BasicTCPSocket *client) {
    int32 size = GetSource().Size();
    int32 newSocket = accept(GetConnectionSocket(), GetSource().GetAddress(), (socklen_t *) &size);

    if (newSocket != -1) {
        if (!client)
            client = new BasicTCPSocket(newSocket);
        client->SetDestination(GetSource());
        client->SetSource(GetSource());
        client->SetConnectionSocket(newSocket);
        return client;
    }
    else if (msecTimeout != TTDefault) {
        int32 errorCode;
        errorCode = sock_errno();

        if ((errorCode != 0) && (errorCode != EINPROGRESS) && (errorCode != EWOULDBLOCK)) {
            return NULL;
        }
        else {
            SocketSelect sel;
            sel.AddWaitOnReadReady(this);
            bool ret = sel.WaitRead(msecTimeout);
            if (ret) return WaitConnection(TTDefault,client);
            return NULL;
        }

    }
    return NULL;}

}
