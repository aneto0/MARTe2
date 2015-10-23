/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
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
 * $Id: BasicUDPSocket.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/**
 * @file
 * Basic definitions and implementation of the UDP socket
 */
#if !defined( BASIC_UDP_SOCKET_H)
#define BASIC_UDP_SOCKET_H

#include "BasicSocket.h"
#include "Sleep.h"

#define BasicUDPSocketVersion "$Id: BasicUDPSocket.h 3 2012-01-15 16:26:07Z aneto $"

class BasicUDPSocket: public BasicSocket {
public:

    // PURE STREAMING
    /** The basic Read function */
    bool BasicRead(void* buffer, uint32 &size){
        uint32 total = size;
        char *p = (char *)buffer;
#if (defined(_RTAI) || defined(_LINUX) || (defined _SOLARIS) || defined(_MACOSX))
        socklen_t sz = (socklen_t)source.Size();
#else
        int sz = source.Size();
#endif

        int32 ret = recvfrom(connectionSocket,p,total,0,source.Address(),&sz);
        if (ret == -1) return False;

        size = ret;
        // to avoid polling continuously release CPU time when reading 0 bytes
        if (size<=0) SleepMsec(1);
        return (ret >0);
    }

    /** The basic write function */
    bool BasicWrite(const void* buffer, uint32 &size){
        uint32 total = size;
        char *p = (char *)buffer;
        int32 ret = sendto(connectionSocket,p,size,0,destination.Address(),destination.Size());
        size = ret;
        // to avoid polling continuously release CPU time when reading 0 bytes
        if (size<=0) SleepMsec(1);
        return (ret >0);
    }

// class specific functions
    /** a constructor */
    BasicUDPSocket(int32 socket = 0){
        connectionSocket = socket;
    }

    /** Opens a socket to address address and with port port */
    bool Open(){
        SocketInit();
        connectionSocket = socket(PF_INET,SOCK_DGRAM,0);
        if (connectionSocket < 0){
            AssertSocketErrorCondition(OSError,"BasicUDPSocket::Open");
            return False;
        }

        return True;
    };

    /** Sets the port to listen from */
    bool Listen(int port,int maxConnections=1){
        InternetAddress    server;
        server.SetPort(port);

        int errorCode = bind(connectionSocket, server.Address(),server.Size());
            if (errorCode < 0) {
            AssertSocketErrorCondition(OSError,"BasicUDPSocket::Listen::bind");
            return False;
        }

        return True;
    }

    /** selects the destination of next sends */
    bool Connect(const char *address,int port){
        destination.SetPort(port);
        if (destination.SetAddressByDotName(address) == False)
            if (destination.SetAddressByName(address) == False) {
                CStaticAssertErrorCondition(ParametersError,"BasicUDPSocket::Connect:wrong address");
                return False;
            }
        return True;
    }

    /** selects the destination of next sends */
    bool Connect(InternetAddress &dest){
        destination = dest;
        return True;
    }

};

#endif









