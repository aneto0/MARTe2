/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or – as soon they 
   will be approved by the European Commission - subsequent  
   versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
   Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
   writing, software distributed under the Licence is 
   distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
   express or implied. 
 * See the Licence for the specific language governing 
   permissions and limitations under the Licence. 
 *
 * $Id: BasicTCPSocket.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/**
 * @file 
 * Implements TCP stream-sockets basics
 */
#if !defined( BASIC_TCP_SOCKET_H)
#define BASIC_TCP_SOCKET_H

#include "System.h"
#include "BasicSocket.h"
#include "SocketSelect.h"
#include "InternetService.h"
#include "Sleep.h"

#define BasicTCPSocketVersion "$Id: BasicTCPSocket.h 3 2012-01-15 16:26:07Z aneto $"
class BasicTCPSocket: public BasicSocket {
public:

    /** basic Read*/
    bool BasicRead(void* buffer, uint32 &size){
        int32 ret = recv(connectionSocket,(char *)buffer,size,0);
        if (ret < 0) {
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::BlockRead");
            size = 0;
            return False;
        }
        size = ret;
        // to avoid polling continuously release CPU time when reading 0 bytes
        if (size == 0) SleepMsec(1);
        return True;
    }

    /** basic write */
    bool BasicWrite(const void* buffer, uint32 &size){
        int32 ret = send(connectionSocket,(char *)buffer,size,0);
        if (ret < 0) {
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::BlockWrite");
            size = 0;
            return False;
        }
        size = ret;
        return True;
    }

    /** Read without consuming */
    bool Peek(void* buffer, uint32 &size){
        int32 ret = recv(connectionSocket,(char *)buffer,size,MSG_PEEK);
        if (ret < 0) {
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::BlockPeek");
            size = 0;
            return False;
        }
        size = ret;
        return True;
    }

    /** just a constructor */
    BasicTCPSocket(int32 socket = 0){
#if ( defined(_LINUX) || defined(_MACOSX) )
        signal (SIGPIPE, SIG_IGN);
#endif        
        connectionSocket = socket;
    }

    /** Opens a stream socket */
    bool Open(){
        SocketInit();
        connectionSocket = socket(PF_INET,SOCK_STREAM,0);
#if defined(_LINUX) || defined(_RTAI) || defined(_MACOSX)
        const int one = 1;
        if (setsockopt(connectionSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0){
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::constructor failed calling setsockopt(connectionSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one) (this machine ip is %s)",GetLocalIpNumber());
            return False;
        }
            
#endif
        if (connectionSocket < 0){
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::constructor failed calling socket(PF_INET,SOCK_STREAM,0) (this machine ip is %s)",GetLocalIpNumber());
            return False;
        }

        return True;
    };

    /** Opens a socket as a server at port port */
    bool Listen(int port,int maxConnections=1){
        InternetAddress    server;

        server.SetPort(port);
        int errorCode = bind(connectionSocket, server.Address(),server.Size());
        if (errorCode < 0) {
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::Listen::bind");
            return False;
        }

        errorCode = listen (connectionSocket,maxConnections);
        if (errorCode < 0) {
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::Listen::listen");
            return False;
        }

        return True;
    }

    /** Opens a socket as a server at port port */
    bool Listen(char *serviceName,int maxConnections=1){
        int32 port = InternetService::GetPortByName(serviceName);
        if (port == -1) return False;
        return Listen(port,maxConnections);
    }

    /** connects an unconnected socket to address address and with port port
        if msecTimeout is TTDefault then select is not used */
    bool Connect(const char *address,int port,TimeoutType msecTimeout = TTInfiniteWait, int retry=12){
        destination.SetPort(port);
        if (destination.SetAddressByDotName(address) == False) {
            if (destination.SetAddressByName(address) == False) {
                CStaticAssertErrorCondition(ParametersError,"BasicTCPSocket::Connect:wrong address [%s], port [%d]",address,port);
                return False;
            }
        }

        int32 errorCode = connect(connectionSocket,destination.Address(),destination.Size());
        if (errorCode < 0){
#if defined (_WIN32)
            errorCode = WSAGetLastError();
#else
            errorCode = sock_errno();
            if (errorCode == EINTR){
                if(retry > 0){
                    return Connect(address, port, msecTimeout, retry--);
                }
                else{
                    AssertSocketErrorCondition(OSError,"BasicTCPSocket::Connect failed. Interrupted system call...");
                    return False;
                }
            }
#endif
            if ((errorCode != 0) &&
                (errorCode != EINPROGRESS) &&
                (errorCode != EWOULDBLOCK)){
                AssertSocketErrorCondition(OSError,"BasicTCPSocket::Connect failed ");
                return False;
            } else {
                if (msecTimeout != TTDefault){
                    SocketSelect sel;
                    sel.AddWaitOnWriteReady(this);
                    bool ret = sel.WaitWrite(msecTimeout);
                    if (ret) return True;
                }
                AssertSocketErrorCondition(OSError,"BasicTCPSocket::Timeout on Connect");
                return False;
            }
        }
        return True;
    }

    /** connects an unconnected socket to address address and with port port */
    bool Connect(const char *address,const char *serviceName,TimeoutType msecTimeout = TTInfiniteWait){
        int32 port = InternetService::GetPortByName(serviceName);
        if (port == -1) return False;
        return Connect(address,port,msecTimeout);
    }

    /** True if we are still connected  Still experimental */
    bool IsConnected(){

#if defined _VXWORKS
    return False;
#else
        int32           errorCode;
        InternetAddress information;
#if defined(_WIN32)
        int len         = information.Size();
#else
        socklen_t len   = information.Size();
#endif

        int32 ret = getpeername(connectionSocket,information.Address(),&len);

#if !defined (_WIN32)
        errorCode = sock_errno();
#else
        errorCode = WSAGetLastError();
        if (errorCode == WSAENOTCONN) return False;
        if (errorCode == WSAENETDOWN) return False;
        if (errorCode == WSAESHUTDOWN) return False;

        return True;
#endif

        return (ret == 0);
#endif

    }

    /** this is a BasicTCPSocket constructor .. */
    BasicTCPSocket *WaitConnection(TimeoutType msecTimeout = TTInfiniteWait,BasicTCPSocket *client=NULL){
        int size = source.Size();
#if (defined (_RTAI) || defined(_LINUX) || (defined _SOLARIS) || defined(_MACOSX))
        int newSocket = accept(connectionSocket,source.Address(),(socklen_t *)&size);
#else
        int newSocket = accept(connectionSocket,source.Address(),&size);
#endif
        if  ( newSocket != -1 ) {
            if (!client) client = new BasicTCPSocket(newSocket);
            client->destination = source;
            client->source      = source;
            client->connectionSocket = newSocket;
            return client;
        } else
        if (msecTimeout != TTDefault){
            int32 errorCode;
#if !defined (_WIN32)
            errorCode = sock_errno();
#else
            errorCode = WSAGetLastError();
#endif
            if ((errorCode != 0) &&
                (errorCode != EINPROGRESS) &&
                (errorCode != EWOULDBLOCK)){
                AssertSocketErrorCondition(OSError,"BasicTCPSocket::WaitConnection failed ");
                return NULL;
            } else {
                SocketSelect sel;
                sel.AddWaitOnReadReady(this);
                bool ret = sel.WaitRead(msecTimeout);
                if (ret) return WaitConnection(TTDefault,client);
                //AssertSocketErrorCondition(Warning,"BasicTCPSocket::Timeout on Connect");
                return NULL;
            }

        }
        return NULL;
    }

};

#endif
