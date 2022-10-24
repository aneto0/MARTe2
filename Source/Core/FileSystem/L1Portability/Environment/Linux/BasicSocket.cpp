/**
 * @file BasicSocket.cpp
 * @brief Source file for class BasicSocket
 * @date 26/10/2015
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
 * the class BasicSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicSocket.h"
#include "ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BasicSocket::BasicSocket() :
        StreamI(),
        HandleI() {
    connectionSocket = -1;
    isBlocking = true;
}

/*lint -e{1551} .Justification: Removes the warning "Function may throw exception '...' in destructor". */
BasicSocket::~BasicSocket() {
    if (BasicSocket::IsValid()) {
        if (!BasicSocket::Close()) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicSocket: The socket handle is invalid");
        }
    }
}

bool BasicSocket::SetBlocking(const bool flag) {
    int32 ret = -1;
    if (IsValid()) {
        int32 stat = 0;
        if (flag) {
            stat = 0;
        }
        else {
            stat = 1;
        }
        ret = ioctl(connectionSocket, static_cast<osulong>(FIONBIO), &stat, sizeof(stat));

        if (ret >= 0) {
            isBlocking = flag;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicSocket: The socket handle is invalid");
    }
    return (ret >= 0);
}

bool BasicSocket::Close() {
    int32 ret = -1;
    if (IsValid()) {
        ret = close(connectionSocket);
        connectionSocket = -1;
        if (ret < 0) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicSocket::Close failed");
            ret = 0;
        }
    }
    else {
        ret = -1;
    }
    return (ret >= 0);
}

InternetHost BasicSocket::GetSource() const {
    return source;
}

InternetHost BasicSocket::GetDestination() const {
    return destination;
}

void BasicSocket::SetDestination(const InternetHost &destinationIn) {
    destination = destinationIn;
}

void BasicSocket::SetSource(const InternetHost &sourceIn) {
    source = sourceIn;
}

bool BasicSocket::IsValid() const {
    return (connectionSocket >= 0);
}

Handle BasicSocket::GetReadHandle() const {
    return connectionSocket;
}

Handle BasicSocket::GetWriteHandle() const {
    return connectionSocket;
}

bool BasicSocket::IsBlocking() const {
    return isBlocking;
}

bool BasicSocket::SetReuseAddr(const bool reuse) {
    int32 reuseInt32 = (reuse ? 1 : 0);
    bool ok = (setsockopt(connectionSocket, SOL_SOCKET, SO_REUSEADDR, &reuseInt32, static_cast<uint32>(sizeof(int32))) >= 0);
    if (!ok) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicSocket::SetReuseAddr failed.");
    }
    return ok;
/*lint -e{1762}  [MISRA C++ Rule 9-3-3]. Justification: The function member could be non-const in other operating system implementations*/
}

}
