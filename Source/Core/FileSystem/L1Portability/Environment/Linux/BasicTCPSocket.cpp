/**
 * @file BasicTCPSocket.cpp
 * @brief Source file for class BasicTCPSocket
 * @date 23/10/2015
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
 * the class BasicTCPSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicSocket.h"
#include "BasicTCPSocket.h"
#include "Sleep.h"
#include "SocketSelect.h"
#include "InternetService.h"
#include "ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#define sock_errno()  errno

namespace MARTe {

bool BasicTCPSocket::Peek(char8* const buffer,
                          uint32 &size) const {
    int32 ret = static_cast<int32>(recv(GetConnectionSocket(), buffer, static_cast<size_t>(size), MSG_PEEK));
    if (ret < 0) {
        size = 0u;
    }
    else {
        /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
        size = static_cast<uint32>(ret);
    }
    return (ret >= 0);
}

BasicTCPSocket::BasicTCPSocket() :
        BasicSocket() {
    /*lint -e{1924} [MISRA C++ Rule 5-2-4]. Justification: C-style cast made at operating system API.*/
    /*lint -e{923} [MISRA C++ Rule 5-2-7]. Justification: cast from integer to pointer made at operating system API level. */
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed signal() trying to ignore SIGPIPE signal");
    }
}

BasicTCPSocket::~BasicTCPSocket(){

}


BasicTCPSocket::BasicTCPSocket(const int32 socketIn) :
        BasicSocket() {
    /*lint -e{1924} [MISRA C++ Rule 5-2-4]. Justification: C-style cast made at operating system API.*/
    /*lint -e{923} [MISRA C++ Rule 5-2-7]. Justification: cast from integer to pointer made at operating system API level. */
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed signal() trying to ignore SIGPIPE signal");
    }
    SetConnectionSocket(socketIn);
}

bool BasicTCPSocket::Open() {

    /*lint e{641} .Justification: The function socket returns an integer.*/
    SetConnectionSocket(socket(PF_INET, SOCK_STREAM, 0));
    const int32 one = 1;
    bool ret = false;
    if (setsockopt(GetConnectionSocket(), SOL_SOCKET, SO_REUSEADDR, &one, static_cast<uint32>(sizeof(one))) >= 0) {

        if (GetConnectionSocket() >= 0) {
            ret = true;
        }
    }
    else{
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed setsockopt() setting the address as reusable");

    }
    return ret;
}

bool BasicTCPSocket::Listen(const uint16 port,
                            const int32 maxConnections) const {
    InternetAddress server;

    server.SetPort(port);
    int32 errorCode = bind(GetConnectionSocket(), reinterpret_cast<struct sockaddr *>(server.GetAddress()), server.Size());
    bool ret = false;
    if (errorCode >= 0) {

        errorCode = listen(GetConnectionSocket(), maxConnections);
        if (errorCode >= 0) {
            ret = true;
        }
        else{
            REPORT_ERROR(ErrorManagement::OSError,"Error: Failed listen()");

        }
    }
    else{
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed bind()");

    }

    return ret;
}

bool BasicTCPSocket::Listen(const char8 * const serviceName,
                            const int32 maxConnections) const {
    int32 port = InternetService::GetPortByName(serviceName);
    /*lint -e{9119} -e{9117} -e{734} [MISRA C++ Rule 5-0-6] [MISRA C++ Rule 5-0-4]. Justification: the operating system InternetAddress struct has
     * an unsigned short "port" member attribute. */
    return (port == -1) ? (false) : (Listen(static_cast<uint16>(port), maxConnections));
}


bool BasicTCPSocket::Connect(const char8 * const address,
                             const uint16 port,
                             const TimeoutType &msecTimeout,
                             int32 retry) {
    GetDestination().SetPort(port);
    bool ret = false;
    if (GetDestination().SetAddressByDotName(address)) {
        if (GetDestination().SetAddressByName(address)) {

            int32 errorCode = connect(GetConnectionSocket(), reinterpret_cast<struct sockaddr *>(GetDestination().GetAddress()), GetDestination().Size());
            if (errorCode < 0) {
                errorCode = sock_errno();
                switch (errorCode) {
                case (EINTR): {
                    if (retry > 0) {
                        retry--;
                        ret = Connect(address, port, msecTimeout, retry);
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::OSError,"Error: failed connect() because interrupted");
                        ret = false;
                    }
                }
                    break;
                case (EINPROGRESS):
                case (EWOULDBLOCK): {
                    if (msecTimeout.IsFinite()) {
                        SocketSelect sel;
                        sel.AddWaitOnWriteReady(this);
                        ret = sel.WaitWrite(msecTimeout);
                    }
                    else {
                        ret = false;
                        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed connect() because the socked is blocked");
                    }
                }
                    break;
                default: {
                    ret = false;
                    REPORT_ERROR(ErrorManagement::OSError,"Error: Failed connect()");
                }
                }
            }
        }
    }
    return ret;
}

bool BasicTCPSocket::Connect(const char8 * const address,
                             const char8 * const serviceName,
                             const TimeoutType &msecTimeout) {
    int32 port = InternetService::GetPortByName(serviceName);
    /*lint -e{9119} -e{9117} -e{734} [MISRA C++ Rule 5-0-6] [MISRA C++ Rule 5-0-4]. Justification: the operating system InternetAddress struct has
     * an unsigned short "port" member attribute. */
    return (port == -1) ? (false) : (Connect(address, static_cast<uint16>(port), msecTimeout));
}

bool BasicTCPSocket::IsConnected() const {

    InternetAddress information;

    socklen_t len = information.Size();

    int32 ret = getpeername(GetConnectionSocket(), reinterpret_cast<struct sockaddr *>(information.GetAddress()), &len);

    return (ret == 0);

}

BasicTCPSocket *BasicTCPSocket::WaitConnection(const TimeoutType &msecTimeout,
                                               BasicTCPSocket *client) {
    uint32 size = GetSource().Size();
    int32 newSocket = accept(GetConnectionSocket(), reinterpret_cast<struct sockaddr *>(GetSource().GetAddress()), reinterpret_cast<socklen_t *>(&size));

    BasicTCPSocket *ret = static_cast<BasicTCPSocket *>(NULL);
    if (newSocket != -1) {
        if (client == NULL) {
            client = new BasicTCPSocket(newSocket);
        }
        client->SetDestination(GetSource());
        client->SetSource(GetDestination());/////
        client->SetConnectionSocket(newSocket);
        ret = client;
    }
    else {
        if (msecTimeout.IsFinite()) {
            int32 errorCode;
            errorCode = sock_errno();
            if ((errorCode == 0) || (errorCode == EINPROGRESS) || (errorCode == EWOULDBLOCK)) {
                SocketSelect sel;
                sel.AddWaitOnReadReady(this);
                if (sel.WaitRead(msecTimeout)) {
                    ret = WaitConnection(TTDefault, client);
                }
            }
        }
    }
    return ret;
}

bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size) {
    int32 readBytes = static_cast<int32>(recv(GetConnectionSocket(), output, static_cast<size_t>(size), 0));
    bool ret = (readBytes >= 0);

    if (ret) {
        /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
        size = static_cast<uint32>(readBytes);
        // to avoid polling continuously release CPU time when reading 0 bytes
        if (size == 0u) {
            Sleep::MSec(1);
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed recv()");
        size = 0u;
    }
    return ret;
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size) {
    int32 writtenBytes = static_cast<int32>(send(GetConnectionSocket(), input, static_cast<size_t>(size), 0));
    bool ret = (writtenBytes >= 0);
    if (ret) {
        /*lint -e{9117} -e{732}  [MISRA C++ Rule 5-0-4]. Justification: the casted number is positive. */
        size = static_cast<uint32>(writtenBytes);
    }
    else {
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed send()");
        size = 0u;
    }

    return ret;
}

bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size,
                          const TimeoutType &msecTimeout) {
    struct timeval timeout;
    /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
    timeout.tv_sec = static_cast<int32>(msecTimeout.GetTimeoutMSec() / 1000u);
    /*lint -e{9117} -e{9114} -e{9125} [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
    timeout.tv_usec = static_cast<int32>((msecTimeout.GetTimeoutMSec() % 1000u) * 1000u);
    int32 ret = setsockopt(GetConnectionSocket(), SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char8 *>(&timeout), static_cast<socklen_t>(sizeof(timeout)));

    bool retVal = true;

    if (ret < 0) {
        size = 0u;
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed setsockopt() setting the socket timeout");
        retVal = false;
    }
    else {
        retVal = Read(output, size);
    }

    if (setsockopt(GetConnectionSocket(), SOL_SOCKET, SO_RCVTIMEO, static_cast<void*>(NULL), static_cast<socklen_t> (sizeof(timeout)))<0) {
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed setsockopt() removing the socket timeout");
    }
    return retVal;
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size,
                           const TimeoutType &msecTimeout) {
    struct timeval timeout;
    /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
    timeout.tv_sec = msecTimeout.GetTimeoutMSec() / 1000u;
    /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
    timeout.tv_usec = (msecTimeout.GetTimeoutMSec() % 1000u) * 1000u;
    int32 ret = setsockopt(GetConnectionSocket(), SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char8 *>(&timeout), static_cast<socklen_t>(sizeof(timeout)));

    bool retVal = true;

    if (ret < 0) {
        size = 0u;
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed setsockopt() setting the socket timeout");
        retVal = false;
    }
    else {
        retVal = Write(input, size);
    }
    if (setsockopt(GetConnectionSocket(), SOL_SOCKET, SO_SNDTIMEO, static_cast<void*>(NULL), static_cast<socklen_t>(sizeof(timeout))) < 0) {
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed setsockopt() removing the socket timeout");
        retVal = false;
    }
    return retVal;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicTCPSocket::Seek(const uint64 pos) {
    return false;
}

uint64 BasicTCPSocket::Size() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicTCPSocket::RelativeSeek(const int32 deltaPos) {
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
    return true;
}

bool BasicTCPSocket::CanRead() const {
    return true;
}

bool BasicTCPSocket::CanSeek() const {
    return false;
}

}

