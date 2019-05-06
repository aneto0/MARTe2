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
#include <stdio.h>
#include <unistd.h>
#include <circle/logger.h>
#include <circle/net/in.h>
#include <circle/logger.h>
#include <circle/net/socket.h>
#include <circle/net/netsubsystem.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicSocket.h"
#include "BasicTCPSocket.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#define sock_errno()  errno

namespace MARTe {

BasicTCPSocket::BasicTCPSocket() :
        BasicSocket() {
}

BasicTCPSocket::~BasicTCPSocket() {

}

bool BasicTCPSocket::Open() {

    connectionSocket.OpenTCP();
    return IsValid();
}

bool BasicTCPSocket::Listen(const uint16 port,
                            const int32 maxConnections) const {

    bool ret = false;
    if (IsValid()) {
        int32 errorCode = (connectionSocket.socket)->Bind(port);

        if (errorCode >= 0) {
            errorCode = (connectionSocket.socket)->Listen(maxConnections);
            if (errorCode >= 0) {
                ret = true;
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed listen()");
            }
        }
        else {

            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed bind()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }

    return ret;
}

bool BasicTCPSocket::Connect(const char8 * const address,
                             const uint16 port,
                             const TimeoutType &timeout) {

    bool ret = IsValid();
    if (ret) {
        destination.SetPort(port);
        ret = true;
        if (!destination.SetAddress(address)) {
            ret = false;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }

    if (ret) {
        source = destination;
        CIPAddress addr(destination.GetInternetHost()->ipAddr);
        ret = ((connectionSocket.socket)->Connect(addr, port) >= 0);
    }

    return ret;

}

bool BasicTCPSocket::IsConnected() const {

    return true;

}

BasicTCPSocket *BasicTCPSocket::WaitConnection(const TimeoutType &timeout,
                                               BasicTCPSocket *client) {
    BasicTCPSocket *ret = static_cast<BasicTCPSocket *>(NULL);
    if (IsValid()) {

        CIPAddress pForeignIP(source.GetInternetHost()->ipAddr);

        CSocket *newSock=(connectionSocket.socket)->Accept (&pForeignIP, &(source.GetInternetHost()->myPort));
        if(newSock!=NULL) {
            client = new BasicTCPSocket();
            client->SetDestination(source);
            client->SetSource(source);
            (client->connectionSocket).SetSocket(newSock);
            ret = client;
        }
    }
    return ret;

}

bool BasicTCPSocket::Peek(char8* const buffer,
                          uint32 &size) const {

    return false;
}

bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size) {
    uint32 sizetoRead = size;
    size = 0u;
    int32 readBytes = 0;
    if (IsValid()) {
        readBytes =(connectionSocket.socket)->Receive(output, sizetoRead, ((isBlocking) ? (0) : (MSG_DONTWAIT)));

        if (readBytes >= 0) {

            /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
            size = static_cast<uint32>(readBytes);
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed recv()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
    return (readBytes > 0);
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size) {
    int32 writtenBytes = 0;
    uint32 sizeToWrite = size;
    size = 0u;
    if (IsValid()) {
        writtenBytes = (connectionSocket.socket)->Send(input, sizeToWrite, 0);

        if (writtenBytes >= 0) {

            /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
            size = static_cast<uint32>(writtenBytes);
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed send()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
    return (writtenBytes > 0);
}

bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size,
                          const TimeoutType &timeout) {

    return (Read(output,size));
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size,
                           const TimeoutType &timeout) {

    return (Write(input,size));
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicTCPSocket::Seek(const uint64 pos) {
    return false;
}

uint64 BasicTCPSocket::Size() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicTCPSocket::RelativeSeek(const int64 deltaPos) {
    return false;
}

uint64 BasicTCPSocket::Position() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the size of a socket is undefined. */
bool BasicTCPSocket::SetSize(const uint64 size) {
    return false;
}

bool BasicTCPSocket::CanWrite() const {
    return true;
}

bool BasicTCPSocket::CanRead() const {
    return true;
}

bool BasicTCPSocket::CanSeek() const {
    return false;
}

}

