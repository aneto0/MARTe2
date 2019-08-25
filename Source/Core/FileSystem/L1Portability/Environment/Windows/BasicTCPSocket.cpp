/**
 * @file BasicTCPSocket.cpp
 * @brief Source file for class BasicTCPSocket
 * @date Nov 23, 2015
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
 * the class BasicTCPSocket (public, protected, and private). Be aware that some 
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
#include "BasicTCPSocket.h"
#include "ErrorManagement.h"
#include "Select.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#define sock_errno()  errno

namespace MARTe {
BasicTCPSocket::BasicTCPSocket() : BasicSocket() {
    WSADATA wsaData;
    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

BasicTCPSocket::~BasicTCPSocket() {
    if (IsValid()) {
        Close();
    }
    WSACleanup();
}

#define CHECK_VALID  \
ret.illegalOperation = !IsValid(); \
REPORT_ERROR(ret, "BasicTCPSocket: The socked handle is not valid");

#define CHECK_SIZE  \
int32 size_I32 = 0; \
if (ret){ \
    ret.outOfRange = !SafeNumber2Number(size,size_I32); \
    REPORT_ERROR(ret, "BasicTCPSocket: size too large"); \
    size = 0u; \
}


//set as unblocking if the timeout is finite.
#define UNBLOCK_IF_NEEDED \
bool wasBlocking = IsBlocking();\
if (ret && timeout.IsValid() && wasBlocking) {\
    ret.fatalError = !SetBlocking(false);\
    REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Socket set to non-block mode failed.");\
}

#define RESTORE_BLOCKING \
if (timeout.IsValid() && wasBlocking) { \
   	ret.fatalError = !SetBlocking(true); \
    REPORT_ERROR(ret, "BasicTCPSocket: Socket reset to blocking mode failed"); \
}


bool BasicTCPSocket::Open() {
    ErrorManagement::ErrorType ret;
    const int32 one = 1;
    connectionSocket = socket(PF_INET, SOCK_STREAM, 0);
    ret.OSError = (connectionSocket == INVALID_SOCKET);
    REPORT_ERROR(ret, "BasicTCPSocket: socket(PF_INET, SOCK_STREAM,0)");

    if (ret) {
    	ret.OSError = (setsockopt(connectionSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char8*>(&one), static_cast<uint32>(sizeof(one))) != 0);
        REPORT_ERROR(ret, "BasicTCPSocket: setsockopt");
    }
    return ret;
}

bool BasicTCPSocket::Listen(const uint16 port,const int32 maxConnections) const {
    ErrorManagement::ErrorType ret;

    CHECK_VALID

    InternetHost server;
    if (ret) {
        server.SetPort(port);

        ret.OSError = (bind(connectionSocket, reinterpret_cast<SOCKADDR *>(server.GetInternetHost()), static_cast<int32>(server.Size()))!= 0);
        REPORT_ERROR(ret, "BasicTCPSocket: bind failed");
    }

    if (ret){
    	ret.OSError = (listen(connectionSocket, maxConnections)!= 0);
        REPORT_ERROR(ret, "BasicTCPSocket: listen failed");
    }

    return ret;
}


bool BasicTCPSocket::Connect(CCString address,const uint16 port,
                             const MilliSeconds &timeout) {
    ErrorManagement::ErrorType ret;

    destination.SetPort(port);

    CHECK_VALID

    if (ret) {
    	if (!destination.SetAddress(address)){
    		ret.OSError = !destination.SetAddressByHostName(address);
            REPORT_ERROR(ret, "Connect: SetAddress failed");
    	}
    }

    if (ret) {
        source = destination;
    }

    UNBLOCK_IF_NEEDED

    if (ret) {
        int32 errorCode = connect(connectionSocket, reinterpret_cast<struct sockaddr *>(destination.GetInternetHost()), static_cast<int32>(destination.Size()));

        if (errorCode != 0) {
            errorCode = WSAGetLastError();
            switch (errorCode) {

            case (WSAEINTR): {
            	ret.OSError = true;
                REPORT_ERROR(ret, "BasicTCPSocket: failed connect() because interrupted function call");
            } break;

            //On Windows a non-blocking socket is very likely to call WSAEWOULDBLOCK so this has to be trapped and retried with an
            //arbitrary timeout
            case (WSAEWOULDBLOCK): {
                fd_set writeFDS;

                FD_ZERO(&writeFDS);
                FD_SET(connectionSocket, &writeFDS);

                timeval timeWait;
                if (wasBlocking) {
                	MS2TV(timeout, timeWait);
                } else {
                    timeWait.tv_sec = 1u;
                    timeWait.tv_usec = 0u;
                }
                int32 readySockets = select(256, static_cast<fd_set*>(NULL), &writeFDS, static_cast<fd_set*>(NULL), &timeWait);

                ret.OSError = (readySockets <= 0);
                REPORT_ERROR(ret, "BasicTCPSocket: Failed connection on select() with errno = WSAEWOULDBLOCK.");

                int32 valopt = 0;
                if (ret) {
                    int32 lon = static_cast<int32>(sizeof(int32));

                    ret.OSError = (getsockopt(connectionSocket, SOL_SOCKET, SO_ERROR, reinterpret_cast<char8 *>(&valopt), &lon) < 0);
                    REPORT_ERROR(ret, "BasicTCPSocket: failed getsockopt() trying to check if the connection is alive");

                }
                if (ret){
                	ret.timeout = (valopt > 0);
                    REPORT_ERROR(ret, "BasicTCPSocket: connection with timeout failed");
                }
            } break;

            default: {
            	ret.OSError = true;
                REPORT_ERROR(ret, "BasicTCPSocket: Failed connect()");
            }

            } // end switch
        } // end if
    } // end if (ret)

    RESTORE_BLOCKING

    return ret;
}

bool BasicTCPSocket::IsConnected() const {
    ErrorManagement::ErrorType ret;
    bool isConnected = false;

    CHECK_VALID

    if (ret){
        InternetHost information;
        // ok as information.Size() is small
        int32 len = static_cast<int32>(information.Size());

        isConnected = (getpeername(connectionSocket, reinterpret_cast<struct sockaddr *>(information.GetInternetHost()), &len) != 0);
    }

    return isConnected;
}

BasicTCPSocket *BasicTCPSocket::WaitConnection(const MilliSeconds &timeout,BasicTCPSocket *client) {
    ErrorManagement::ErrorType ret;

    CHECK_VALID

    UNBLOCK_IF_NEEDED

    if (ret) {
    	// size value is small
        int32 size = static_cast<int32>(source.Size());
        SOCKET newSocket = accept(connectionSocket, reinterpret_cast<struct sockaddr *>(source.GetInternetHost()), &size);

        if (newSocket != INVALID_SOCKET) {
            if (client == NULL) {
                client = new BasicTCPSocket();
            }

            client->SetDestination(source);
            client->SetSource(source);
            client->connectionSocket = newSocket;
        } else {
        	ret.fatalError =  !wasBlocking;
            REPORT_ERROR(ret, "BasicTCPSocket: Failed accept in unblocking mode");

            if (ret){
            	ret.timeout = !timeout.IsValid();
                REPORT_ERROR(ErrorManagement::Timeout, "BasicTCPSocket: Timeout expired");
            }
            if (ret) {
                int32 errorCode;
                errorCode = WSAGetLastError();
                if ((errorCode == 0) || (errorCode == WSAEINPROGRESS) || (errorCode == WSAEWOULDBLOCK)) {
                    fd_set readFDS;
                    FD_ZERO(&readFDS);
                    FD_SET(connectionSocket, &readFDS);
                    timeval timeWait;
                	MS2TV(timeout,timeWait);
                    int32 readySockets = select(256, &readFDS, &readFDS, static_cast<fd_set*>(NULL), &timeWait);
                    if (readySockets > 0) {
// TODO avoid recursion
                        ret.fatalError = !WaitConnection(timeout, client);
                    }
                }
            }
        }
    }

    RESTORE_BLOCKING

    return client;
}

bool BasicTCPSocket::Peek(char8* const buffer,uint32 &size) const {
    ErrorManagement::ErrorType ret;

    CHECK_VALID

	CHECK_SIZE

    int32 readBytes = 0;
    if (ret) {
    	readBytes =  recv(connectionSocket, buffer, size_I32, MSG_PEEK);
    	ret.OSError = (readBytes == SOCKET_ERROR);
    }

    if (ret && (readBytes > 0)){
    	size = static_cast<uint32>(readBytes);
    }
    return ret ;
}

bool BasicTCPSocket::Read(char8* const output,uint32 &size) {
    ErrorManagement::ErrorType ret;

    CHECK_VALID

	CHECK_SIZE

    int32 readBytes = 0;

    if (ret) {
        readBytes = recv(connectionSocket, output, size_I32, 0);

        ret.fatalError = (readBytes == 0);
        REPORT_ERROR(ret, "BasicTCPSocket: Connection Close");

    }

    if (ret && (readBytes < 0)){
    	int error = WSAGetLastError();
        bool ewouldblock = (error == WSAEWOULDBLOCK); ///Resource temporarily unavailable, from operation on nonblocking.
        bool eagain = (error == WSATRY_AGAIN); ///Temporary error during host name, retry at some time later may be successful.
        ret.timeout = ((ewouldblock || eagain) && (IsBlocking()));
        REPORT_ERROR(ret, "BasicTCPSocket: Timeout expired in recv()");
    }

    return ret;
}


bool BasicTCPSocket::Read(char8* const output,uint32 &size,
                          const MilliSeconds &timeout) {

    ErrorManagement::ErrorType ret;

    CHECK_VALID

    bool timeoutInstalled = false;
    if (ret && timeout.IsValid()) {

        struct timeval timeoutVal;
        MS2TV(timeout, timeoutVal);
        ret.OSError = (setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char8 *>(&timeoutVal), sizeof(timeoutVal)) == SOCKET_ERROR );
        REPORT_ERROR(ret, "BasicTCPSocket: Failed setsockopt() setting the socket timeout");

        timeoutInstalled = !ret.OSError;
	}

    if (ret){
        ret.fatalError = BasicTCPSocket::Read(output, size);
        REPORT_ERROR(ret, "BasicTCPSocket: Failed Read()");
    }

    if (timeoutInstalled){

    	struct timeval timeoutVal;
        timeoutVal.tv_sec = 0;
        timeoutVal.tv_usec = 0;
        ret.OSError = (setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char8 *>(&timeoutVal), static_cast<uint32>(sizeof(timeoutVal)))== SOCKET_ERROR);
        REPORT_ERROR(ret, "BasicTCPSocket: Failed setsockopt() removing the socket timeout");
    }

    return ret;
}

bool BasicTCPSocket::Write(const char8* const input,uint32 &size) {
    ErrorManagement::ErrorType ret;

    CHECK_VALID

	CHECK_SIZE

	int32 writtenBytes = 0;

    if (ret) {
        writtenBytes = send(connectionSocket, input, size_I32, 0);

        if (writtenBytes >= 0) {
        	// always valid as writtenBytes is positive
            size = static_cast<uint32>(writtenBytes);
        } else {
        	int error = WSAGetLastError();
            bool ewouldblock = (error == WSAEWOULDBLOCK); ///Resource temporarily unavailable, from operation on nonblocking.
            bool eagain = (error == WSATRY_AGAIN); ///Temporary error during host name, retry at some time later may be successful.
            ret.timeout = ((ewouldblock || eagain) && (IsBlocking()));
            REPORT_ERROR(ret, "BasicTCPSocket: Timeout expired in send()");
            if (ret){
            	ret.OSError = true;
                REPORT_ERROR(ret, "BasicTCPSocket: Failed send()");
            }
        }
    }
    return ret;
}

bool BasicTCPSocket::Write(const char8* const input,uint32 &size,
                           const MilliSeconds &timeout) {

    ErrorManagement::ErrorType ret;

    CHECK_VALID

    bool timeoutInstalled = false;
    if (ret && timeout.IsValid()) {

        struct timeval timeoutVal;
        MS2TV(timeout, timeoutVal);
        ret.OSError = (setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char8 *>(&timeoutVal), sizeof(timeoutVal)) == SOCKET_ERROR );
        REPORT_ERROR(ret, "BasicTCPSocket: Failed setsockopt() setting the socket timeout");

        timeoutInstalled = !ret.OSError;
	}

    if (ret){
        ret.fatalError = BasicTCPSocket::Write(input, size);
        REPORT_ERROR(ret, "BasicTCPSocket: Failed Read()");
    }

    if (timeoutInstalled){

    	struct timeval timeoutVal;
        timeoutVal.tv_sec = 0;
        timeoutVal.tv_usec = 0;
        ret.OSError = (setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char8 *>(&timeoutVal), static_cast<uint32>(sizeof(timeoutVal)))== SOCKET_ERROR);
        REPORT_ERROR(ret, "BasicTCPSocket: Failed setsockopt() removing the socket timeout");
    }

    return ret;
}

bool BasicTCPSocket::Seek(const uint64 pos) {
    return false;
}

uint64 BasicTCPSocket::Size()const {
	return StreamI::Invalid;
}

bool BasicTCPSocket::RelativeSeek(const int64 deltaPos) {
    return false;
}

uint64 BasicTCPSocket::Position()const {
	return StreamI::Invalid;
}

bool BasicTCPSocket::SetSize(const uint64 size) {
    return false;
}

bool BasicTCPSocket::CanWrite() const {
    return true;
}

bool BasicTCPSocket::CanRead() const {
    return true;
}

bool BasicTCPSocket::CanSeek() const {
    return false;
}
}

