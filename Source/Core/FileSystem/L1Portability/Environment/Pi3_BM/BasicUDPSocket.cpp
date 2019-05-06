/**
 * @file BasicUDPSocket.cpp
 * @brief Source file for class BasicUDPSocket
 * @date 28/10/2015
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
 * the class BasicUDPSocket (public, protected, and private). Be aware that some 
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
    return false;

}

bool BasicUDPSocket::Read(char8* const output,
                          uint32 &size) {

    int32 ret = -1;
    uint32 sizeToRead = size;
    size = 0u;
    if (IsValid()) {

        CIPAddress pForeignIP(source.GetInternetHost()->ipAddr);
        ret = (connectionSocket.socket)->ReceiveFrom(output, sizeToRead, ((isBlocking) ? (0) : (MSG_DONTWAIT)), &pForeignIP, &(source.GetInternetHost()->myPort));

        if (ret >= 0) {
            size = static_cast<uint32>(ret);
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket: Failed recvfrom()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
    return (ret > 0);
}

bool BasicUDPSocket::Write(const char8* const input,
                           uint32 &size) {

    int32 ret = -1;
    uint32 sizeToWrite = size;
    size = 0u;
    if (IsValid()) {
        CIPAddress pForeignIP(destination.GetInternetHost()->ipAddr);

        ret = (connectionSocket.socket)->SendTo(input, size, MSG_DONTWAIT, &pForeignIP, &(destination.GetInternetHost()->myPort));

        if (ret >= 0) {
            size = static_cast<uint32>(ret);
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket: Failed sendto()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");

    }
    return (ret > 0);
}

bool BasicUDPSocket::Open() {
    connectionSocket.OpenUDP();
    return IsValid();
}

/*lint -e{1762}  [MISRA C++ Rule 9-3-3]. Justification: The function member could be non-const in other operating system implementations*/
bool BasicUDPSocket::Listen(const uint16 port) {
    int32 errorCode = -1;
    if (IsValid()) {
        errorCode = (connectionSocket.socket)->Bind(port);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
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
            ret = false;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }

    if (ret) {
        CIPAddress addr(destination.GetInternetHost()->ipAddr);
        ret = ((connectionSocket.socket)->Connect(addr, port) >= 0);
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

    return Read(output, size);
}

bool BasicUDPSocket::Write(const char8 * const input,
                           uint32 & size,
                           const TimeoutType &timeout) {

    return Read(input, size);
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
