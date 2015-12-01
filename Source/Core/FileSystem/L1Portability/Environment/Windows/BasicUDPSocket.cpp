/**
 * @file BasicUDPSocket.cpp
 * @brief Source file for class BasicUDPSocket
 * @date Nov 23, 2015
 * @author Frank Perez Paz
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
 * the class BasicUDPSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <windows.h>
#include <winsock.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicUDPSocket.h"
#include "ErrorManagement.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

BasicUDPSocket::BasicUDPSocket() :
        BasicSocket() {
    WSADATA wsaData;
    // Initialize Winsock
    int32 iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
}

BasicUDPSocket::~BasicUDPSocket() {

}

bool BasicUDPSocket::Peek(char8* const output,
                          uint32 &size) {

    int32 ret = -1;
    uint32 sizeToRead = size;
    size = 0u;
    if (IsValid()) {
        uint32 sourceSize = source.Size();
        ret = static_cast<int32>(recvfrom(connectionSocket, output, static_cast<size_t>(sizeToRead), MSG_PEEK,
                                          reinterpret_cast<struct sockaddr*>(source.GetInternetHost()), reinterpret_cast<int32 *>(&sourceSize)));
        if (ret != SOCKET_ERROR) {
            /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
            size = static_cast<uint32>(ret);
            // to avoid polling continuously release CPU time when reading 0 bytes
            if (size <= 0u) {
                Sleep::MSec(1);
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::OSError, "BasicUDPSocket: Failed recvfrom()");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
    return (ret > 0);
}

bool BasicUDPSocket::Read(char8* const output,
                          uint32 &size) {
    int32 ret = -1;
    uint32 sizeToRead = size;
    size = 0u;
    if (IsValid()) {
        int32 sourceSize = source.Size();
        ret = static_cast<int32>(recvfrom(connectionSocket, output, sizeToRead, 0, reinterpret_cast<struct sockaddr*>(source.GetInternetHost()), &sourceSize));

        //printf("inside read: ret= %d  output= %s  size= %d\n",ret,output,sizeToRead);
        if (ret > 0) {

            /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
            size = ret;
            // to avoid polling continuously release CPU time when reading 0 bytes
            if (size <= 0u) {
                Sleep::MSec(1);
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::OSError, "BasicUDPSocket: Failed recvfrom()");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
    //printf("leaving the read with value %d\n",(ret > 0));
    return (ret > 0);
}

bool BasicUDPSocket::Write(const char8* const input,
                           uint32 &size) {

    int32 ret = -1;
    uint32 sizeToWrite = size;
    size = 0u;
    if (IsValid()) {
        ret = static_cast<int32>(sendto(connectionSocket, input, sizeToWrite, 0, reinterpret_cast<struct sockaddr*>(destination.GetInternetHost()),
                                        destination.Size()));
        if (ret != SOCKET_ERROR) {
            /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
            size = static_cast<uint32>(ret);
            // to avoid polling continuously release CPU time when reading 0 bytes
            if (size <= 0u) {
                Sleep::MSec(1);
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::OSError, "BasicUDPSocket: Failed sendto()");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");

    }
    return (ret > 0);
}

bool BasicUDPSocket::Open() {
    WSADATA wsaData;
    int iResult;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: fail WSAStartup");
    }

    /*lint -e{641} .Justification the socket type descriptor is an integer */
    connectionSocket = (socket(PF_INET, SOCK_DGRAM, 0));
    if (connectionSocket == INVALID_SOCKET) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: connectionSocket == INVALID_SOCKET");
    }
    return (connectionSocket >= 0);
}

/*lint -e{1762}  [MISRA C++ Rule 9-3-3]. Justification: The function member could be non-const in other operating system implementations*/
bool BasicUDPSocket::Listen(const uint16 port) {
    int32 errorCode = -1;
    if (IsValid()) {
        InternetHost server;
        server.SetPort(port);

        errorCode = bind(connectionSocket, reinterpret_cast<struct sockaddr*>(server.GetInternetHost()), static_cast<int32>(server.Size()));
        if (errorCode < 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: Listen made a errorCode");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
    return (errorCode >= 0);
}

bool BasicUDPSocket::Connect(const char8 * const address,
                             const uint16 port) {

    bool ret = IsValid();
    if (ret) {
        destination.SetPort(port);
        ret = true;
        if (!destination.SetAddress(address)) {
            if (!destination.SetAddressByHostName(address)) {
                ret = false;
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }

    return ret;
}

bool BasicUDPSocket::CanWrite() const {
    return true;
}

bool BasicUDPSocket::CanRead() const {
    return true;
}

bool BasicUDPSocket::CanSeek() const {
    return false;
}

bool BasicUDPSocket::Read(char8 * const output,
                          uint32 & size,
                          const TimeoutType &timeout) {
    uint32 sizeToRead = size;
    size = 0u;
    if (IsValid()) {

        if (timeout.IsFinite()) {
            struct timeval timeoutVal;
            /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_sec = static_cast<int32>(timeout.GetTimeoutMSec() / 1000u);
            /*lint -e{9117} -e{9114} -e{9125} [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_usec = static_cast<int32>((timeout.GetTimeoutMSec() % 1000u) * 1000u);

            int32 iResult = bind(connectionSocket, reinterpret_cast<struct sockaddr*>(source.GetInternetHost()), static_cast<int32>(source.Size()));
            if (iResult == SOCKET_ERROR) {
                closesocket (connectionSocket);
                WSACleanup();
                return 0;
            }

            int32 ret = setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char8 *>(&timeoutVal), static_cast<int32>(sizeof(timeoutVal))); //int32 -> socklen_t

            if (ret != 0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() setting the read timeout");
            }
            else {
                if (Read(output, sizeToRead)) {
                    size = sizeToRead;
                }
            }

            if (setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, static_cast<char*>(NULL), static_cast<int32> (sizeof(timeoutVal)))<0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() removing the read timeout");
            }
        }
        else {
            if(Read(output, sizeToRead)) {
                size=sizeToRead;
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
    return (size > 0u);
}

bool BasicUDPSocket::Write(const char8 * const input,
                           uint32 & size,
                           const TimeoutType &timeout) {
    uint32 sizeToWrite = size;
    size = 0u;
    if (IsValid()) {
        if (timeout.IsFinite()) {

            struct timeval timeoutVal;
            /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_sec = timeout.GetTimeoutMSec() / 1000u;
            /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_usec = (timeout.GetTimeoutMSec() % 1000u) * 1000u;
            int32 ret = setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char8 *>(&timeoutVal), static_cast<int32>(sizeof(timeoutVal)));

            if (ret < 0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() setting the write timeout");
            }
            else {
                if (Write(input, sizeToWrite)) {
                    size = sizeToWrite;
                }
            }
            if (setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, static_cast<char*>(NULL), static_cast<uint32>(sizeof(timeoutVal))) < 0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() removing the write timeout");
            }
        }
        else {
            if(Write(input, sizeToWrite)) {
                size=sizeToWrite;
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
    return (size > 0u);
}

uint64 BasicUDPSocket::Size() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicUDPSocket::Seek(const uint64 pos) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicUDPSocket::RelativeSeek(const int32 deltaPos) {
    return false;
}

uint64 BasicUDPSocket::Position() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the size of a socket is undefined. */
bool BasicUDPSocket::SetSize(const uint64 size) {
    return false;
}

}

