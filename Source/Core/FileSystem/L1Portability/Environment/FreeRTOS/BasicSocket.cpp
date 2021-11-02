/**
 * @file BasicSocket.cpp
 * @brief Source file for class BasicSocket
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
 * the class BasicSocket (public, protected, and private). Be aware that some 
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
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicSocket::Close() the socket handle is invalid");
        }
    }
}

bool BasicSocket::SetBlocking(const bool flag) {
    int32 ret = -1;
    if (IsValid()) {
#ifdef LWIP_ENABLED
        uint32 opt = lwip_fcntl(connectionSocket, F_GETFL, 0);
        if (flag) {
            opt |= O_NONBLOCK;
        }
        else {
            opt &= ~O_NONBLOCK;
        }
        ret = lwip_fcntl(connectionSocket, F_SETFL, opt);

        if (ret >= 0) {
            isBlocking = flag;
        }
#endif
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicSocket::SetBlocking() The socket handle is invalid");
    }
    return (ret >= 0);
}

bool BasicSocket::Close() {
    int32 ret = -1;
#ifdef LWIP_ENABLED
    if (IsValid()) {
        //Avoiding close() to be non-blocking if the socket is in non-blocking mode
        if(isBlocking) {
            uint32 opt = lwip_fcntl(connectionSocket, F_GETFL, 0);
            opt |= O_NONBLOCK;
            ret = lwip_fcntl(connectionSocket, F_SETFL, opt);
        }
        ret = lwip_close(connectionSocket);
        connectionSocket = -1;

        if (ret != 0) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicSocket::Close failed");
            ret = -1;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicSocket::Close() closing an invalid socket");
        ret = -1;
    }
#endif
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

}
