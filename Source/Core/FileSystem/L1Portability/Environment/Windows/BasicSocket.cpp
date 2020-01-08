/**
 * @file BasicSocket.cpp
 * @brief Source file for class BasicSocket
 * @date Nov 18, 2015
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
 * the class BasicSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include <stdio.h>
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
        StreamI()/*,
        HandleI() */{
    connectionSocket = INVALID_SOCKET ;
    isBlocking = true;
    WSADATA wsaData;
    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

BasicSocket::~BasicSocket() {
    if (IsValid()) {
        if (!Close()) {
            REPORT_ERROR(ErrorManagement::FatalError, "BasicSocket: failed to close socket");
        }
    }
    WSACleanup();
}

bool BasicSocket::SetBlocking(const bool flag) {
    int32 ret = -1;
    if (IsValid()) {
        u_long iMode = 0;
        if (flag) {
            iMode = 0; ///If iMode = 0, blocking is enabled
        }
        else {
            iMode = 1;
        }
        ret = ioctlsocket(connectionSocket, static_cast<osulong>(FIONBIO), &iMode);

        if (ret == 0) {
            isBlocking = flag;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicSocket: The socket handle is invalid");
    }
    return (ret == 0);
}

bool BasicSocket::Close() {
    int32 ret = -1;
    if (IsValid()) {
        ret = closesocket(connectionSocket);
        connectionSocket = INVALID_SOCKET ;
        if (ret != 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "BasicSocket::Close failed returning");
            ret = -1;
        }
    }
    else {
        ret = -1;
    }
    return (ret == 0);
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
    ///Modified the return, in windows socket is not an int32 and the comparison is true, it's necessary a cast
    return (connectionSocket != INVALID_SOCKET);
}

bool BasicSocket::IsBlocking() const {
    return isBlocking;
}

EventSource BasicSocket::GetEventSource(EventInterface::Event eventMask) const{
	ErrorManagement::ErrorType ret;
	long OSEvents = 0;

	if (eventMask.In(readEvent)){
		OSEvents |= FD_READ;
	}
	if (eventMask.In(writeEvent)){
		OSEvents |= FD_WRITE;
	}
	if (eventMask.In(exceptionEvent)){
		OSEvents |= (FD_ACCEPT | FD_CONNECT | FD_CLOSE);
	}
	if (eventMask.In(acceptEvent)){
		OSEvents |= FD_ACCEPT;
	}
	if (eventMask.In(connectionEvent)){
		OSEvents |= FD_CONNECT;
	}
	if (eventMask.In(closeEvent)){
		OSEvents |= FD_CLOSE;
	}

	HANDLE hEvent = NULL;
	if (ret){
		hEvent = WSACreateEvent();
		ret.OSError = (hEvent == NULL);
		REPORT_ERROR(ret," WSACreateEvent failed");
	}

	if (ret){
		ret.OSError = (WSAEventSelect(connectionSocket,hEvent,OSEvents)!=0);
		REPORT_ERROR(ret,"WSAEventSelect failed");

		if (!ret){
			CloseHandle(hEvent);
			hEvent = NULL;
		}
	}

	EventSource es;
	if (ret){
		EventSourceData *esd = es.GetData();
		if (esd != NULL_PTR(EventSourceData *)){
			esd->SetHandle(hEvent,true);
		}
	}
	return es;

}



}

