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
#include "TimeoutType.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {


BasicSocket::BasicSocket() {
    connectionSocket = 0;
}

BasicSocket::BasicSocket(int32 socket) {
    connectionSocket = socket;
}

/** destructor */
BasicSocket::~BasicSocket() {
    Close();
}

/**  set blocking mode for the stream! */
bool BasicSocket::SetBlocking(bool flag) {
    int stat;
    if (flag == true)
        stat = 0;
    else
        stat = 1;

    int ret = ioctl(connectionSocket, FIONBIO, (char *) &stat, sizeof(stat));
    return (ret >= 0);
}

/** returns the socket number */
int32 BasicSocket::Socket() {
    return connectionSocket;
}

/** closes the socket */
bool BasicSocket::Close() {
    if (connectionSocket != 0) {
        int ret = close(connectionSocket);
        connectionSocket = 0;
        if (ret < 0) {
            return false;
        }
    }
    else
        return false;
    return true;
}

/** where the packet came from */
InternetAddress &BasicSocket::GetSource() {
    return source;
}

/** where the packet is going to */
InternetAddress &BasicSocket::GetDestination() {
    return destination;
}


int32 BasicSocket::GetConnectionSocket() const {
    return connectionSocket;
}

void BasicSocket::SetDestination(const InternetAddress &destinationIn) {
    destination = destinationIn;
}

void BasicSocket::SetSource(const InternetAddress &sourceIn) {
    source = sourceIn;
}

void BasicSocket::SetConnectionSocket(int32 connectionSocketIn) {
    connectionSocket = connectionSocketIn;
}

}
