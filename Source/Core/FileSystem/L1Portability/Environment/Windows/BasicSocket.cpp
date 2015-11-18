/**
 * @file BasicSocket.cpp
 * @brief Source file for class BasicSocket
 * @date Nov 18, 2015
 * @author CCS Portable Station
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
#include DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <windows.h>
#include <winsock.h>
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
        StreamI() {
    connectionSocket = -1;
    isBlocking = true;
}

BasicSocket::~BasicSocket() {
    if (!Close()) {
        //TODO
    }
}
bool BasicSocket::SetBlocking(const bool flag) {
    int32 ret = -1;
    if (IsValid()) {
        int32 stat = 0;
        if (flag) {
            u_long iMode = 0;
        }
        else {
            u_long iMode = 1;
        }
        ret = ioctlsocket(connectionSocket, static_cast<osulong>(FIONBIO), &iMode);
        if (ret >= 0) {
            isBlocking = flag;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicSocket: The socket handle is invalid");
    }
    return (ret >= 0);
}

bool BasicSocket::Close() {
    int32 ret = -1;
    if (IsValid()) {
        ret = closesocket(connectionSocket);
        connectionSocket = -1;
        if (ret < 0) {
            CStaticAssertErrorCondition(OSError, "BasicSocket::Close failed returning %i\n", ret);
            ret = 0;
        }
    }
    else {
        ret = 0;
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

bool BasicSocket::IsBlocking() const {
    return isBlocking;
}
}

