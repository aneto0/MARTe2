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
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicSocket.h"
#include "BasicTCPSocket.h"
#include "ErrorManagement.h"
#include "Select.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#define sock_errno()  errno

namespace MARTe {
BasicTCPSocket::BasicTCPSocket() :
        BasicSocket() {
    WSADATA wsaData;
    // Initialize Winsock
    int32 iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
}

BasicTCPSocket::~BasicTCPSocket() {

}

bool BasicTCPSocket::Open() {
    WSADATA wsaData;
    // Initialize Winsock
    int32 iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicUDPSocket: fail WSAStartup");
    }
    bool ret = false;
    /*lint -e{641} .Justification: The function socket returns an integer.*/
    connectionSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (connectionSocket == INVALID_SOCKET) {
        REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed");
        ret = false;
    }
    if (connectionSocket >= 0) {
        ret = true;
    }
    return ret;
}

bool BasicTCPSocket::Listen(const uint16 port,
                            const int32 maxConnections) const {
    bool ret = false;
    if (IsValid()) {
        InternetHost server;
        server.SetPort(port);
        int32 errorCode = bind(connectionSocket, reinterpret_cast<sockaddr *>(server.GetInternetHost()), server.Size());

        if (errorCode == 0) {
            errorCode = listen(connectionSocket, maxConnections);
            if (errorCode == 0) {
                ret = true;
            }
            else {
                REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed listen()");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed bind()");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }

    return ret;
}

bool BasicTCPSocket::Connect(const char8 * const address,
                             const uint16 port,
                             const TimeoutType &timeout) {
    destination.SetPort(port);
    bool ret = IsValid();
    bool wasBlocking = IsBlocking();
    if (ret) {
        if (!destination.SetAddress(address)) {
            if (!destination.SetAddressByHostName(address)) {
                ret = false;
            }
        }
        if (ret) {
            source = destination;
            if (timeout.IsFinite()) {
                //set as unblocking if the timeout is finite.
                if (wasBlocking) {
                    ret = SetBlocking(false);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Socket set to non-block mode failed.");
                    }
                }
            }
            if (ret) {

                int32 errorCode = connect(connectionSocket, reinterpret_cast<struct sockaddr *>(destination.GetInternetHost()), destination.Size());

                if (errorCode != 0) {
                    errorCode = WSAGetLastError();

                    switch (errorCode) {
                    case (WSAEINTR): {
                        REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: failed connect() because interrupted function call");
                        ret = false;
                    }
                        break;
                        //On Windows a non-blocking socket is very likely to call WSAEWOULDBLOCK so this has to be trapped and retried with an
                        //arbitrary timeout
                    case (WSAEWOULDBLOCK): {
                        Select sel;
                        sel.AddWriteHandle(*this);
                        TimeoutType timeoutWouldBlock = 10;
                        ret = (sel.WaitUntil(timeoutWouldBlock) > 0);
                        if (ret) {
                            int32 lon = static_cast<int32>(sizeof(int32));
                            int32 valopt;
                            if (getsockopt(connectionSocket, SOL_SOCKET, SO_ERROR, reinterpret_cast<char8 *>(&valopt), &lon) < 0) {
                                REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: failed getsockopt() trying to check if the connection is alive");
                                ret = false;
                            }
                            else {
                                if (valopt > 0) {
                                    REPORT_ERROR(ErrorManagement::Timeout, "BasicTCPSocket: connection with timeout failed");
                                    ret = false;
                                }
                            }
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed connection on select() with errno = WSAEWOULDBLOCK.");
                            ret = false;
                        }
                    }
                        break;
                    case (WSAEINPROGRESS): {
                        if (timeout.IsFinite()) {
                            Select sel;
                            sel.AddWriteHandle(*this);
                            ret = (sel.WaitUntil(timeout) > 0);
                            if (ret) {
                                int32 lon = static_cast<int32>(sizeof(int32));
                                int32 valopt;
                                if (getsockopt(connectionSocket, SOL_SOCKET, SO_ERROR, reinterpret_cast<char8 *>(&valopt), &lon) < 0) {
                                    REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: failed getsockopt() trying to check if the connection is alive");
                                    ret = false;
                                }
                                else {
                                    if (valopt > 0) {
                                        REPORT_ERROR(ErrorManagement::Timeout, "BasicTCPSocket: connection with timeout failed");
                                        ret = false;
                                    }
                                }
                            }
                            else {
                                REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed connection on select().");
                            }
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed connect(); errno = EINPROGRESS");
                            ret = false;
                        }

                    }
                        break;
                    default: {
                        REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed connect()");
                        ret = false;
                    }
                    }
                }
            }

            if (timeout.IsFinite()) {
                if (wasBlocking) {
                    if (!SetBlocking(true)) {
                        REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: Socket reset to blocking mode failed");
                        ret = false;
                    }
                }
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: Failed setting the destination address");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }

    return ret;
}

bool BasicTCPSocket::IsConnected() const {

    int32 ret = -1;
    if (IsValid()) {
        InternetHost information;

        int32 len = information.Size();

        ret = getpeername(connectionSocket, reinterpret_cast<struct sockaddr *>(information.GetInternetHost()), &len);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
    return (ret == 0);

}

BasicTCPSocket *BasicTCPSocket::WaitConnection(const TimeoutType &timeout,
                                               BasicTCPSocket *client) {
    BasicTCPSocket *ret = static_cast<BasicTCPSocket *>(NULL);

    if (IsValid()) {
        bool created=false;
        bool wasBlocking = IsBlocking();

        bool ok=true;
        if (timeout.IsFinite()) {
            if(wasBlocking) {
                if(!SetBlocking(false)) {
                    REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Socket set to non-block mode failed.");
                    ok=false;
                }
            }
        }
        if(ok) {
            uint32 size = source.Size();
            SOCKET newSocket = accept(connectionSocket, reinterpret_cast<struct sockaddr *>(source.GetInternetHost()), reinterpret_cast<int32 *>(&size));

            if (newSocket != -1) {
                if (client == NULL) {
                    client = new BasicTCPSocket();
                    created = true;
                }

                client->SetDestination(source);
                client->SetSource(source);
                client->connectionSocket = newSocket;
                ret = client;
            }
            else {
                if (wasBlocking) {
                    if (timeout.IsFinite()) {
                        int32 errorCode;
                        errorCode = WSAGetLastError();
                        if ((errorCode == 0) || (errorCode == WSAEINPROGRESS) || (errorCode == WSAEWOULDBLOCK)) {
                            Select sel;
                            sel.AddReadHandle(*this);

                            if (sel.WaitUntil(timeout)>0) {
                                ret = WaitConnection(TTDefault, client);
                            }
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::Timeout, "BasicTCPSocket: Timeout expired");
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: Failed accept in unblocking mode");
                }
            }
        }
        if (timeout.IsFinite()) {
            if(wasBlocking) {
                if(!SetBlocking(true)) {
                    REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Socket reset to non-block mode failed.");
                    if(created) {
                        delete client;
                    }
                    ret=static_cast<BasicTCPSocket *>(NULL);
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }

    return ret;
}

bool BasicTCPSocket::Peek(char8* const buffer,
                          uint32 &size) const {
    int32 ret = -1;
    uint32 sizeToRead = size;
    size = 0u;

    if (IsValid()) {
        ret = static_cast<int32>(recv(connectionSocket, buffer, static_cast<size_t>(sizeToRead), MSG_PEEK));
        if (ret > 0) {
            size = static_cast<uint32>(ret);
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
    return (ret > 0);
}

bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size) {
    uint32 sizetoRead = size;
    int32 readBytes = 0;
    size = 0u;

    if (IsValid()) {
        readBytes = static_cast<int32>(recv(connectionSocket, output, static_cast<size_t>(sizetoRead), 0));

        if (readBytes > 0) {
            size = static_cast<uint32>(readBytes);
        }
        else if (readBytes == 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: Connection Close");
        }
        else {
            bool ewouldblock = (WSAGetLastError() == WSAEWOULDBLOCK); ///Resource temporarily unavailable, from operation on nonblocking.
            bool eagain = (WSAGetLastError() == WSATRY_AGAIN); ///Temporary error during host name, retry at some time later may be successful.
            if ((ewouldblock || eagain) && (IsBlocking())) {
                REPORT_ERROR(ErrorManagement::Timeout, "BasicTCPSocket: Timeout expired in recv()");
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
    return (readBytes > 0);
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size) {
    int32 writtenBytes = 0;
    uint32 sizeToWrite = size;
    size = 0u;

    if (IsValid()) {
        writtenBytes = static_cast<int32>(send(connectionSocket, input, static_cast<size_t>(sizeToWrite), 0));
        if (writtenBytes >= 0) {
            size = static_cast<uint32>(writtenBytes);
        }
        else {
            bool ewouldblock = (WSAGetLastError() == WSAEWOULDBLOCK); ///Resource temporarily unavailable, from operation on nonblocking.
            bool eagain = (WSAGetLastError() == WSATRY_AGAIN); ///Temporary error during host name, retry at some time later may be successful.
            if ((ewouldblock || eagain) && (IsBlocking())) {
                REPORT_ERROR(ErrorManagement::Timeout, "BasicTCPSocket: Timeout expired in send()");
            }
            else {
                REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed send()");
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
    return (writtenBytes > 0);
}

bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size,
                          const TimeoutType &timeout) {

    uint32 sizeToRead = size;
    size = 0u;
    if (IsValid()) {
        if (timeout.IsFinite()) {

            struct timeval timeoutVal;
            timeoutVal.tv_sec = static_cast<int32>(timeout.GetTimeoutMSec() / 1000u);
            timeoutVal.tv_usec = static_cast<int32>((timeout.GetTimeoutMSec() % 1000u) * 1000u);

            int32 iResult = bind(connectionSocket, reinterpret_cast<struct sockaddr*>(source.GetInternetHost()), static_cast<int32>(source.Size()));
            if (iResult == SOCKET_ERROR) {
                closesocket (connectionSocket);
                WSACleanup();
                return 0;
            }

            int32 ret = setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char8 *>(&timeoutVal), static_cast<uint32>(sizeof(timeoutVal)));
            if (ret != 0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed setsockopt() setting the socket timeout");
            }
            else {
                if (Read(output, sizeToRead)) {
                    size = sizeToRead;
                }
            }
            timeoutVal.tv_sec = 0;
            timeoutVal.tv_usec = 0;
            if (setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char8 *>(&timeoutVal), static_cast<uint32>(sizeof(timeoutVal))) < 0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed setsockopt() removing the socket timeout");
            }
        }
        else if (Read(output, sizeToRead)) {
            size = sizeToRead;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
    return (size > 0u);
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size,
                           const TimeoutType &timeout) {

    uint32 sizeToWrite = size;
    size = 0u;
    if (IsValid()) {
        if (timeout.IsFinite()) {
            struct timeval timeoutVal;
            timeoutVal.tv_sec = timeout.GetTimeoutMSec() / 1000u;
            timeoutVal.tv_usec = (timeout.GetTimeoutMSec() % 1000u) * 1000u;

            int32 iResult = bind(connectionSocket, reinterpret_cast<struct sockaddr*>(source.GetInternetHost()), static_cast<int32>(source.Size()));
            if (iResult == SOCKET_ERROR) {
                closesocket (connectionSocket);
                WSACleanup();
                return 0;
            }

            int32 ret = setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char8 *>(&timeoutVal), static_cast<uint32>(sizeof(timeoutVal)));

            if (ret != 0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed setsockopt() setting the socket timeoutVal");
            }
            else if (Write(input, sizeToWrite)) {
                size = sizeToWrite;
            }

            timeoutVal.tv_sec = 0;
            timeoutVal.tv_usec = 0;
            if (setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char8 *>(&timeoutVal), static_cast<uint32>(sizeof(timeoutVal))) < 0) {
                REPORT_ERROR(ErrorManagement::OSError, "BasicTCPSocket: Failed setsockopt() removing the socket timeoutVal");
            }
        }
        else if (Write(input, sizeToWrite)) {
            size = sizeToWrite;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
    return (size > 0u);
}

bool BasicTCPSocket::Seek(const uint64 pos) {
    return false;
}

uint64 BasicTCPSocket::Size() {
    return 0xffffffffffffffffu;
}

bool BasicTCPSocket::RelativeSeek(const int32 deltaPos) {
    return false;
}

uint64 BasicTCPSocket::Position() {
    return 0xffffffffffffffffu;
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

