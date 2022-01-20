/**
 * @file BasicUDPSocket.cpp
 * @brief Source file for class BasicUDPSocket
 * @date 20/04/2019
 * @author Andre Neto
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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#ifdef LWIP_ENABLED
#include "lwip/sockets.h"
#endif
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicUDPSocket.h"
#include "ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BasicUDPSocket::BasicUDPSocket() :
        BasicSocket() {

}

BasicUDPSocket::~BasicUDPSocket() {

}

bool BasicUDPSocket::Peek(char8* const output,
                          uint32 &size) {

    int32 ret = -1;
#ifdef LWIP_ENABLED
    uint32 sizeToRead = size;
    size = 0u;
    if (IsValid()) {
        uint32 sourceSize = source.Size();
        /*lint -e{740} [MISRA C++ Rule 5-2-6], [MISRA C++ Rule 5-2-7]. Justification: Pointer to Pointer cast required by operating system API.*/
        ret = static_cast<int32>(lwip_recvfrom(connectionSocket, output, static_cast<size_t>(sizeToRead), MSG_PEEK,
                                          reinterpret_cast<struct sockaddr*>(source.GetInternetHost()), reinterpret_cast<socklen_t*>(&sourceSize)));
        if (ret >= 0) {
            /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
            size = static_cast<uint32>(ret);
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket: Failed recvfrom()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
#endif
    return (ret > 0);

}

bool BasicUDPSocket::Read(char8* const output,
                          uint32 &size) {

    int32 ret = -1;
#ifdef LWIP_ENABLED
    uint32 sizeToRead = size;
    size = 0u;
    if (IsValid()) {
        uint32 sourceSize = source.Size();
        /*lint -e{740} [MISRA C++ Rule 5-2-6], [MISRA C++ Rule 5-2-7]. Justification: Pointer to Pointer cast required by operating system API.*/
        ret = static_cast<int32>(recvfrom(connectionSocket, output, static_cast<size_t>(sizeToRead), 0,
                                          reinterpret_cast<struct sockaddr*>(source.GetInternetHost()), reinterpret_cast<socklen_t*>(&sourceSize)));
        if (ret >= 0) {
            /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
            size = static_cast<uint32>(ret);
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket: Failed recvfrom()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
#endif
    return (ret > 0);
}

bool BasicUDPSocket::Write(const char8* const input,
                           uint32 &size) {

    int32 ret = -1;
#ifdef LWIP_ENABLED
    uint32 sizeToWrite = size;
    size = 0u;
    if (IsValid()) {
        /*lint -e{740} [MISRA C++ Rule 5-2-6], [MISRA C++ Rule 5-2-7]. Justification: Pointer to Pointer cast required by operating system API.*/
        ret = static_cast<int32>(sendto(connectionSocket, input, static_cast<size_t>(sizeToWrite), 0,
                                        reinterpret_cast<struct sockaddr*>(destination.GetInternetHost()), destination.Size()));
        if (ret >= 0) {
            /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
            size = static_cast<uint32>(ret);
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket: Failed sendto()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");

    }
#endif
    return (ret > 0);
}

bool BasicUDPSocket::Join(const char8 *const group) const {
    bool ok = false;
    /* lwIP needs IGMP, which in turn needs SOCKET_API which in turn needs FreeRTOS or libMetal. Here we are BareMetal */
    return ok;
}

bool BasicUDPSocket::Open() {
#ifdef LWIP_ENABLED
    connectionSocket = (socket(PF_INET, SOCK_DGRAM, 0));
#endif
    return (connectionSocket >= 0);
}

/*lint -e{1762}  [MISRA C++ Rule 9-3-3]. Justification: The function member could be non-const in other operating system implementations*/
bool BasicUDPSocket::Listen(const uint16 port) {
    int32 errorCode = -1;
#ifdef LWIP_ENABLED
    if (IsValid()) {
        InternetHost server;
        server.SetPort(port);
        /*lint -e{740} [MISRA C++ Rule 5-2-6], [MISRA C++ Rule 5-2-7]. Justification: Pointer to Pointer cast required by operating system API.*/
        errorCode = bind(connectionSocket, reinterpret_cast<struct sockaddr*>(server.GetInternetHost()), static_cast<socklen_t>(server.Size()));
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
#endif
    return (errorCode >= 0);
}

bool BasicUDPSocket::Connect(const char8 * const address,
                             const uint16 port) {

    bool ret = IsValid();
#ifdef LWIP_ENABLED
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
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
#endif

    return ret;
}

bool BasicUDPSocket::CanWrite() const {
    return IsValid();
}

bool BasicUDPSocket::CanRead() const {
    return IsValid();
}

bool BasicUDPSocket::CanSeek() const {
    return IsValid();
}

bool BasicUDPSocket::Read(char8 * const output,
                          uint32 & size,
                          const TimeoutType &timeout) {
    uint32 sizeToRead = size;
    size = 0u;
#ifdef LWIP_ENABLED
    if (IsValid()) {

        if (timeout.IsFinite()) {
            struct timeval timeoutVal;
            /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_sec = static_cast<int32>(timeout.GetTimeoutMSec() / 1000u);
            /*lint -e{9117} -e{9114} -e{9125} [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_usec = static_cast<int32>((timeout.GetTimeoutMSec() % 1000u) * 1000u);
            int32 ret = setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, &timeoutVal,
                                   static_cast<socklen_t>(sizeof(timeoutVal)));

            if (ret < 0) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() setting the read timeout");
            }
            else {
                if (BasicUDPSocket::Read(output, sizeToRead)) {
                    size = sizeToRead;
                }
            }
            timeoutVal.tv_sec = 0;
            timeoutVal.tv_usec = 0;
            if (setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, &timeoutVal, static_cast<socklen_t>(sizeof(timeoutVal))) < 0) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() removing the read timeout");
            }
        }
        else {
            if (BasicUDPSocket::Read(output, sizeToRead)) {
                size = sizeToRead;
            }
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
#endif
    return (size > 0u);
}

bool BasicUDPSocket::Write(const char8 * const input,
                           uint32 & size,
                           const TimeoutType &timeout) {
    uint32 sizeToWrite = size;
    size = 0u;
#ifdef LWIP_ENABLED
    if (IsValid()) {
        if (timeout.IsFinite()) {

            struct timeval timeoutVal;
            /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_sec = timeout.GetTimeoutMSec() / 1000u;
            /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_usec = (timeout.GetTimeoutMSec() % 1000u) * 1000u;
            int32 ret = setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, &timeoutVal,
                                   static_cast<socklen_t>(sizeof(timeoutVal)));

            if (ret < 0) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() setting the write timeout");
            }
            else {
                if (BasicUDPSocket::Write(input, sizeToWrite)) {
                    size = sizeToWrite;
                }
            }
            timeoutVal.tv_sec = 0;
            timeoutVal.tv_usec = 0;
            if (setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, &timeoutVal, static_cast<socklen_t>(sizeof(timeoutVal))) < 0) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket: Failed setsockopt() removing the write timeout");
            }
        }
        else {
            if (BasicUDPSocket::Write(input, sizeToWrite)) {
                size = sizeToWrite;
            }
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
#endif
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
bool BasicUDPSocket::RelativeSeek(const int64 deltaPos) {
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
