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

const EventInterface::Event BasicSocket::readEvent(0x1);
const EventInterface::Event BasicSocket::writeEvent(0x2);
const EventInterface::Event BasicSocket::exceptionEvent(0x4);
const EventInterface::Event BasicSocket::acceptEvent(0x8);
const EventInterface::Event BasicSocket::connectionEvent(0x10);
const EventInterface::Event BasicSocket::closeEvent(0x20);

BasicSocket::BasicSocket() :
        StreamI() {
    connectionSocket = -1;
    isBlocking = true;
}

/*lint -e{1551} .Justification: Removes the warning "Function may throw exception '...' in destructor". */
BasicSocket::~BasicSocket() {
    if (BasicSocket::IsValid()) {
        if (!BasicSocket::Close()) {
            REPORT_ERROR(ErrorManagement::FatalError, "BasicSocket: The socket handle is invalid");
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
        REPORT_ERROR(ErrorManagement::FatalError, "BasicSocket: The socket handle is invalid");
    }
    return (ret >= 0);
}

bool BasicSocket::Close() {
    int32 ret = -1;
    if (IsValid()) {
        ret = close(connectionSocket);
        connectionSocket = -1;
        if (ret < 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "BasicSocket::Close failed");
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

bool BasicSocket::IsBlocking() const {
    return isBlocking;
}

// TODO check flags
EventSource BasicSocket::GetEventSource(EventInterface::Event eventMask) const{
	ErrorManagement::ErrorType ret;
	uint16 OSEvents = 0;

	if (eventMask.In(readEvent)){
		OSEvents |= POLLIN;
	}
	if (eventMask.In(writeEvent)){
		OSEvents |= POLLOUT;
	}
	if (eventMask.In(exceptionEvent)){
		OSEvents |= (POLLRDHUP | POLLPRI );
	}
	if (eventMask.In(acceptEvent)){
		OSEvents |= POLLPRI;
	}
	if (eventMask.In(connectionEvent)){
		OSEvents |= POLLPRI;
	}
	if (eventMask.In(closeEvent)){
		OSEvents |= POLLRDHUP;
	}


	EventSource es;
	if (ret){
		EventSourceData *esd = es.GetData();
		if (esd != NULL_PTR(EventSourceData *)){
			esd->pfd.fd = connectionSocket;
			esd->pfd.events = OSEvents;
		}
	}
	return es;

}


}
