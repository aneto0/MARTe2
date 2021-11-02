/**
 * @file BasicTCPSocket.cpp
 * @brief Source file for class BasicTCPSocket
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
 * the class BasicTCPSocket (public, protected, and private). Be aware that some 
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

BasicTCPSocket::BasicTCPSocket() :
        BasicSocket() {
}

BasicTCPSocket::~BasicTCPSocket() {

}

bool BasicTCPSocket::Open() {
    bool ret = false;
#ifdef LWIP_ENABLED
    /*lint -e{641} .Justification: The function socket returns an integer.*/
    connectionSocket = socket(PF_INET, SOCK_STREAM, 0);
    ret = (connectionSocket >= 0);

    if(ret) {
        //SO_REUSEADDR is here to restart a closed/killed process on the same address
        //as TIME_WAIT state to ensure all data is transferred
        //Here because close is not always honored immediately, to avoid bind failures especially in tests
        //const int32 one = 1;
        //ret = (setsockopt(connectionSocket, SOL_SOCKET, SO_REUSEADDR, &one, static_cast<uint32>(sizeof(one))) >= 0);
        if(!ret) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed setsockopt() setting the address as reusable");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: failure creating socket()");
    }
#endif
    return ret;
}

bool BasicTCPSocket::Listen(const uint16 port,
                            const int32 maxConnections) const {

    bool ret = false;
#ifdef LWIP_ENABLED
    if (IsValid()) {
        InternetHost server;

        server.SetPort(port);
        int32 errorCode = bind(connectionSocket, reinterpret_cast<struct sockaddr *>(server.GetInternetHost()), server.Size());

        if (errorCode >= 0) {
            errorCode = listen(connectionSocket, maxConnections);
            if (errorCode >= 0) {
                ret = true;
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed listen()");
            }
        }
        else {

            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed bind()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
#endif
    return ret;
}

bool BasicTCPSocket::Connect(const char8 * const address,
                             const uint16 port,
                             const TimeoutType &timeout) {
    destination.SetPort(port);
    bool ret = IsValid();
    bool wasBlocking = IsBlocking();
#ifdef LWIP_ENABLED

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
                        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Socket set to non-block mode failed.");
                    }
                }
            }
            if (ret) {
                /*lint -e{740} [MISRA C++ Rule 5-2-6], [MISRA C++ Rule 5-2-7]. Justification: Pointer to Pointer cast required by operating system API.*/
                int32 errorCode = connect(connectionSocket, reinterpret_cast<struct sockaddr *>(destination.GetInternetHost()), destination.Size());
                if (errorCode < 0) {
                    errorCode = sock_errno();
                    switch (errorCode) {
                    case (EINTR): {
                        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: failed connect() because interrupted by a signal");
                        ret = false;

                    }
                        break;
                    case (EINPROGRESS): {
                        if (timeout.IsFinite() || (!wasBlocking)) {
                            Select sel;
                            ret = sel.AddWriteHandle(*this);
                            if (ret) {
                                if (wasBlocking) {
                                    ret = (sel.WaitUntil(timeout) > 0);
                                }
                                else {
                                    ret = (sel.WaitUntil(0u) > 0);
                                }
                            }
                            if (ret) {
                                socklen_t lon = static_cast<socklen_t>(sizeof(int32));
                                int32 valopt;
                                if (getsockopt(connectionSocket, SOL_SOCKET, SO_ERROR, static_cast<void*>(&valopt), &lon) < 0) {
                                    ret = false;
                                    REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: failed getsockopt() trying to check if the connection is alive");
                                }
                                else {
                                    if (valopt > 0) {
                                        REPORT_ERROR_STATIC_0(ErrorManagement::Timeout, "BasicTCPSocket: connection with timeout failed");
                                        ret = false;
                                    }
                                }
                            }
                            else {
                                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed connection on select().");
                            }
                        }
                        else {
                            ret = false;
                            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed connect(); errno = EINPROGRESS");
                        }

                    }
                        break;
                    default: {
                        ret = false;
                        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed connect()");
                    }
                    }
                }
            }

            if (timeout.IsFinite()) {
                if (wasBlocking) {
                    if (!SetBlocking(true)) {
                        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: Socket reset to blocking mode failed");
                        ret = false;
                    }
                }
            }
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: Failed setting the destination address");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
#endif

    return ret;
}

bool BasicTCPSocket::IsConnected() const {

    int32 ret = -1;
#ifdef LWIP_ENABLED
    if (IsValid()) {
        InternetHost information;

        socklen_t len = information.Size();
        /*lint -e{740} [MISRA C++ Rule 5-2-6], [MISRA C++ Rule 5-2-7]. Justification: Pointer to Pointer cast required by operating system API.*/
        ret = getpeername(connectionSocket, reinterpret_cast<struct sockaddr *>(information.GetInternetHost()), &len);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
#endif

    return (ret == 0);

}

BasicTCPSocket *BasicTCPSocket::WaitConnection(const TimeoutType &timeout,
                                               BasicTCPSocket *client) {
    BasicTCPSocket *ret = static_cast<BasicTCPSocket *>(NULL);
#ifdef LWIP_ENABLED

    if (IsValid()) {
        bool created=false;
        bool wasBlocking = IsBlocking();

        bool ok=true;
        if (timeout.IsFinite()) {
            if(wasBlocking) {
                if(!SetBlocking(false)) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Socket set to non-block mode failed.");
                    ok=false;
                }
            }
        }

        uint32 occasions=1u;
        for(uint32 i=0u; (i<occasions) && (ok); i++) {
            uint32 size = source.Size();
            /*lint -e{740} [MISRA C++ Rule 5-2-6], [MISRA C++ Rule 5-2-7]. Justification: Pointer to Pointer cast required by operating system API.*/
            int32 newSocket = accept(connectionSocket, reinterpret_cast<struct sockaddr *>(source.GetInternetHost()), reinterpret_cast<socklen_t *>(&size));

            if (newSocket != -1) {
                if (client == NULL) {
                    client = new BasicTCPSocket();
                    created=true;
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
                        errorCode = sock_errno();
                        if ((errorCode == 0) || (errorCode == EINPROGRESS) || (errorCode == EWOULDBLOCK)) {
                            Select sel;
                            if(sel.AddReadHandle(*this)) {
                                if (sel.WaitUntil(timeout)>0) {
                                    if(occasions==1u) {
                                        occasions++;
                                    }
                                }
                            }

                        }
                    }
                    else {

                        REPORT_ERROR_STATIC_0(ErrorManagement::Timeout, "BasicTCPSocket: Timeout expired");
                    }
                }
                else {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: Failed accept in unblocking mode");
                }

            }
        }
        if (timeout.IsFinite()) {
            if(wasBlocking) {
                if(!SetBlocking(true)) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Socket reset to non-block mode failed.");
                    if(created) {
                        delete client;
                    }
                    ret=static_cast<BasicTCPSocket *>(NULL);
                }
            }
        }
    }
    else {

        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
#endif

    return ret;
}

bool BasicTCPSocket::Peek(char8* const buffer,
                          uint32 &size) const {
    int32 ret = -1;
#ifdef LWIP_ENABLED

    uint32 sizeToRead = size;
    size = 0u;

    if (IsValid()) {
        ret = static_cast<int32>(recv(connectionSocket, buffer, static_cast<size_t>(sizeToRead), MSG_PEEK));
        if (ret > 0) {
            /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
            size = static_cast<uint32>(ret);
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
#endif

    return (ret > 0);
}

bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size) {
    uint32 sizetoRead = size;
    size = 0u;
    int32 readBytes = 0;
#ifdef LWIP_ENABLED
    if (IsValid()) {
        readBytes = static_cast<int32>(recv(connectionSocket, output, static_cast<size_t>(sizetoRead), 0));

        if (readBytes >= 0) {

            /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
            size = static_cast<uint32>(readBytes);
        }
        else {
            bool ewouldblock = (sock_errno() == EWOULDBLOCK);
            bool eagain = (sock_errno() == EAGAIN);
            bool blocking = IsBlocking();
            if ((ewouldblock || eagain) && (blocking)) {
                REPORT_ERROR_STATIC_0(ErrorManagement::Timeout, "BasicTCPSocket: Timeout expired in recv()");
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed recv()");
            }
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
#endif

    return (readBytes > 0);
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size) {
    int32 writtenBytes = 0;
    uint32 sizeToWrite = size;
    size = 0u;
#ifdef LWIP_ENABLED
    if (IsValid()) {
        writtenBytes = static_cast<int32>(send(connectionSocket, input, static_cast<size_t>(sizeToWrite), 0));
        if (writtenBytes >= 0) {

            /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
            size = static_cast<uint32>(writtenBytes);
        }
        else {
            bool ewouldblock = (sock_errno() == EWOULDBLOCK);
            bool eagain = (sock_errno() == EAGAIN);
            bool blocking = IsBlocking();
            if ((ewouldblock || eagain) && (blocking)) {
                REPORT_ERROR_STATIC_0(ErrorManagement::Timeout, "BasicTCPSocket: Timeout expired in send()");
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed send()");
            }
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
#endif
    return (writtenBytes > 0);
}

bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size,
                          const TimeoutType &timeout) {

    uint32 sizeToRead = size;
    size = 0u;
#ifdef LWIP_ENABLED
    if (IsValid()) {
        if (timeout.IsFinite()) {

            struct timeval timeoutVal;
            /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_sec = static_cast<int32>(timeout.GetTimeoutMSec() / 1000u);
            /*lint -e{9117} -e{9114} -e{9125} [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_usec = static_cast<int32>((timeout.GetTimeoutMSec() % 1000u) * 1000u);
            int32 ret = setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, &timeoutVal, static_cast<socklen_t>(sizeof(timeoutVal)));

            if (ret < 0) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed setsockopt() setting the socket timeout");
            }
            else {
                if (BasicTCPSocket::Read(output, sizeToRead)) {
                    size = sizeToRead;
                }
            }
            timeoutVal.tv_sec = 0;
            timeoutVal.tv_usec = 0;
            if (setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, &timeoutVal, static_cast<socklen_t>(sizeof(timeoutVal))) < 0) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed setsockopt() removing the socket timeout");
            }
        }

        else {

            if (BasicTCPSocket::Read(output, sizeToRead)) {
                size = sizeToRead;
            }
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
#endif
    return (size > 0u);
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size,
                           const TimeoutType &timeout) {

    uint32 sizeToWrite = size;
    size = 0u;
#ifdef LWIP_ENABLED
    if (IsValid()) {
        if (timeout.IsFinite()) {
            struct timeval timeoutVal;
            /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_sec = timeout.GetTimeoutMSec() / 1000u;
            /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
            timeoutVal.tv_usec = (timeout.GetTimeoutMSec() % 1000u) * 1000u;
            int32 ret = setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, &timeoutVal, static_cast<socklen_t>(sizeof(timeoutVal)));

            if (ret < 0) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed setsockopt() setting the socket timeoutVal");
            }
            else {
                if (BasicTCPSocket::Write(input, sizeToWrite)) {
                    size = sizeToWrite;
                }
            }
            timeoutVal.tv_sec = 0;
            timeoutVal.tv_usec = 0;
            if (setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, &timeoutVal, static_cast<socklen_t>(sizeof(timeoutVal))) < 0) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket: Failed setsockopt() removing the socket timeoutVal");
            }
        }
        else {
            if (BasicTCPSocket::Write(input, sizeToWrite)) {
                size = sizeToWrite;
            }
        }
    }

    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicTCPSocket: The socked handle is not valid");
    }
#endif
    return (size > 0u);
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicTCPSocket::Seek(const uint64 pos) {
    return false;
}

uint64 BasicTCPSocket::Size() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicTCPSocket::RelativeSeek(const int64 deltaPos) {
    return false;
}

uint64 BasicTCPSocket::Position() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the size of a socket is undefined. */
bool BasicTCPSocket::SetSize(const uint64 size) {
    return false;
}

bool BasicTCPSocket::CanWrite() const {
    return IsValid();
}

bool BasicTCPSocket::CanRead() const {
    return IsValid();
}

bool BasicTCPSocket::CanSeek() const {
    return false;
}

}

