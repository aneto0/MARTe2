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
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
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
    if (iResult != 0) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: fail WSAStartup");
    }
}

BasicUDPSocket::~BasicUDPSocket() {
    WSACleanup();
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
        if (ret > 0) {
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
            size = static_cast<uint32>(ret);
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
    connectionSocket = (socket(PF_INET, SOCK_DGRAM, 0));
    if (connectionSocket != INVALID_SOCKET) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: connectionSocket == INVALID_SOCKET");
    }
    return (connectionSocket >= 0);
}

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
            timeoutVal.tv_sec = static_cast<int32>(timeout.GetTimeoutMSec() / 1000u);
            timeoutVal.tv_usec = static_cast<int32>((timeout.GetTimeoutMSec() % 1000u) * 1000u);

            int32 ret = setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char8 *>(&timeoutVal), static_cast<int32>(sizeof(timeoutVal))); //int32 -> socklen_t

            if (ret != 0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() setting the read timeout");
            }
            else {
                if (BasicUDPSocket::Read(output, sizeToRead)) {
                    size = sizeToRead;
                }
            }
            timeoutVal.tv_sec = 0;
            timeoutVal.tv_usec = 0;
            if (setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, static_cast<char*>(NULL), static_cast<int32>(sizeof(timeoutVal)))<0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() removing the read timeout");
            }
        }
        else {
            if (BasicUDPSocket::Read(output, sizeToRead)) {
                size = sizeToRead;
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
            timeoutVal.tv_sec = timeout.GetTimeoutMSec() / 1000u;
            timeoutVal.tv_usec = (timeout.GetTimeoutMSec() % 1000u) * 1000u;
            int32 ret = setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char8 *>(&timeoutVal), static_cast<int32>(sizeof(timeoutVal)));

            if (ret < 0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() setting the write timeout");
            }
            else {
                if (BasicUDPSocket::Write(input, sizeToWrite)) {
                    size = sizeToWrite;
                }
            }
            timeoutVal.tv_sec = 0;
            timeoutVal.tv_usec = 0;
            if (setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, static_cast<char*>(NULL), static_cast<uint32>(sizeof(timeoutVal))) < 0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() removing the write timeout");
            }
        }
        else {
            if (BasicUDPSocket::Write(input, sizeToWrite)) {
                size = sizeToWrite;
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

bool BasicUDPSocket::Seek(const uint64 pos) {
    return false;
}

bool BasicUDPSocket::RelativeSeek(const int64 deltaPos) {
    return false;
}

uint64 BasicUDPSocket::Position() {
    return 0xffffffffffffffffu;
}

bool BasicUDPSocket::SetSize(const uint64 size) {
    return false;
}

}

