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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BasicSocket::BasicSocket() :
        StreamI() {
    connectionSocket = 0;
}

BasicSocket::BasicSocket(const SocketCore socketIn) :
        StreamI() {
    connectionSocket = socketIn;
}

/*lint -e{1551} .Justification: Removes the warning "Function may throw exception '...' in destructor". */
BasicSocket::~BasicSocket() {
    if(!Close()){
        //TODO
    }
}

bool BasicSocket::SetBlocking(const bool flag) const{
    int32 stat = 0;
    if (flag) {
        stat = 0;
    }
    else {
        stat = 1;
    }

    int32 ret = ioctl(connectionSocket, static_cast<osulong>(FIONBIO), reinterpret_cast<char8 *> (&stat), sizeof(stat));
    return (ret >= 0);
}

bool BasicSocket::Close() {
    int32 ret = -1;
    if (connectionSocket != 0) {
        ret = close(connectionSocket);
        connectionSocket = 0;
    }
    return (ret >= 0);
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: the member is exposed
 * because some operating system functions may change it.
 */
InternetAddress &BasicSocket::GetSource() {
    return source;
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: the member is exposed
 * because some operating system functions may change it.
 */
InternetAddress &BasicSocket::GetDestination() {
    return destination;
}

SocketCore BasicSocket::GetConnectionSocket() const {
    return connectionSocket;
}

void BasicSocket::SetDestination(const InternetAddress &destinationIn) {
    destination = destinationIn;
}

void BasicSocket::SetSource(const InternetAddress &sourceIn) {
    source = sourceIn;
}

void BasicSocket::SetConnectionSocket(const SocketCore connectionSocketIn) {
    connectionSocket = connectionSocketIn;
}

}
