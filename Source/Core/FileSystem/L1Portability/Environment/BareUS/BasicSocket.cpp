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

    #ifdef LWIP_ENABLED
    connectionSocket.UDPHandle = NULL;
    connectionSocket.TCPHandle = NULL;
    connectionSocket.socketKind = SocketCoreKindUndefined;
    #endif
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

    bool ret = false;
    if (IsValid()) {
#ifdef LWIP_ENABLED
    //TODO: this can be handled better, if blocking, the socket itself may call the network interface update hook
    //      as natively, lwIP is non-blocking by design
    ret = flag;
    REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "BasicSocket::SetBlocking() lwIP in raw mode is non-blocking by design");
#endif
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicSocket::SetBlocking() The socket handle is invalid");
    }
    return flag;
}

bool BasicSocket::Close() {
    int32 ret = -1;
#ifdef LWIP_ENABLED
    //TODO: TCP side must be implemented
    if(IsValid()) {
        if(connectionSocket.socketKind == SocketCoreKindTCP) {
            ret = -1;
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicSocket::Close() Not implemented");
        }
        else if(connectionSocket.socketKind == SocketCoreKindUDP) {
            if(connectionSocket.UDPHandle != NULL) {
                udp_disconnect(connectionSocket.UDPHandle);
                udp_remove(connectionSocket.UDPHandle);
                ret = 0;
            }
            else {
                ret = -1;
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicSocket::Close() Trying to close a null socket");
            }
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "BasicSocket::Close() A wrong socket kind was specified");
        }
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
    #ifdef LWIP_ENABLED
    return (connectionSocket.UDPHandle != NULL);
    #endif
}

Handle BasicSocket::GetReadHandle() const {
    REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "BasicSocket::GetReadHandle() Cannot get handle in lwIP raw mode");
    return 0;
}

Handle BasicSocket::GetWriteHandle() const {
    REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "BasicSocket::GetReadHandle() Cannot get handle in lwIP raw mode");
    return 0;
}

bool BasicSocket::IsBlocking() const {
    return true;
}

}
