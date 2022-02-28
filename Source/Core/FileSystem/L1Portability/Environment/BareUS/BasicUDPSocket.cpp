/**
 * @file BasicUDPSocket.cpp
 * @brief Source file for class BasicUDPSocket
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
 * the class BasicUDPSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
 #if defined(LWIP_ENABLED) || defined(LWIP_RAW_ENABLED)

#include "lwip/dhcp.h"
#include "lwip/err.h"
#include "lwip/igmp.h"
#include "lwip/netif.h"
#include "lwip/opt.h"
#include "lwip/pbuf.h"
#include "lwip/sockets.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicUDPSocket.h"
#include "ErrorManagement.h"
#include "MemoryOperationsHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#ifdef LWIP_ENABLED
extern void NetworkInterfaceHook(void* netifParams);
#endif

namespace MARTe {

BasicUDPSocket::BasicUDPSocket() :
        BasicSocket() {
    #ifdef LWIP_ENABLED
        connectionSocket.socketKind = SocketCoreKindUDP;
        connectionSocket.UDPHandle = NULL;
    #endif
}

BasicUDPSocket::~BasicUDPSocket() {

}

bool BasicUDPSocket::Peek(char8* const output,
                          uint32 &size) {

    int32 ret = -1;
#ifdef LWIP_ENABLED
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket::Peek() Cannot peek raw lwIP socket");
#endif
    return (ret > 0);

}

bool BasicUDPSocket::Read(char8* const output,
                          uint32 &size) {

    int32 ret = -1;
#ifdef LWIP_ENABLED
    //TODO: To be implemented
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket::Read() Not implemented");
#endif
    return (ret > 0);
}

bool BasicUDPSocket::Write(const char8* const input,
                           uint32 &size) {

    err_t err;
#ifdef LWIP_ENABLED
    ip_addr_t destIPAddress;
    destIPAddress = (destination.GetInternetHost())->addr;
    uint16 destPort = (destination.GetInternetHost())->port;

    struct pbuf *packetBuffer = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_POOL);
    MemoryOperationsHelper::Copy(packetBuffer->payload, input, size);
    err = udp_sendto(connectionSocket.UDPHandle, packetBuffer, &destIPAddress, destPort);
    pbuf_free(packetBuffer);

    NetworkInterfaceHook(NULL);

#endif
    return (err == ERR_OK);
}

bool BasicUDPSocket::Join(const char8 *const group) const {
    bool ok = false;
    /* lwIP needs IGMP, which in turn needs SOCKET_API which in turn needs FreeRTOS or libMetal. Here we are BareMetal */
    return ok;
}

bool BasicUDPSocket::Open() {
#ifdef LWIP_ENABLED
    connectionSocket.UDPHandle = udp_new();
    return (connectionSocket.UDPHandle != NULL);
#endif
}

/*lint -e{1762}  [MISRA C++ Rule 9-3-3]. Justification: The function member could be non-const in other operating system implementations*/
bool BasicUDPSocket::Listen(const uint16 port) {
    int32 errorCode = -1;
#ifdef LWIP_ENABLED

#endif /* ! LWIP_ENABLED */

    return (errorCode >= 0);
}

bool BasicUDPSocket::Connect(const char8 * const address,
                             const uint16 port) {

    bool ret = IsValid();
#ifdef LWIP_ENABLED
    if (ret) {
        destination.SetPort(port);
        ret = true;
        if (!destination.SetAddress(address)) {
            if (!destination.SetAddressByHostName(address)) {
                ret = false;
            }
        }
        if (ret) {
            NetworkInterfaceHook(NULL);
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicUDPSocket: The socket handle is not valid");
    }
#endif

    return ret;
}

bool BasicUDPSocket::CanWrite() const {
    return true;
}

bool BasicUDPSocket::CanRead() const {
    return true;
}

bool BasicUDPSocket::CanSeek() const {
    return false;
}

bool BasicUDPSocket::Read(char8 * const output,
                          uint32 & size,
                          const TimeoutType &timeout) {
    uint32 sizeToRead = size;
    size = 0u;
    return (size > 0u);
}

bool BasicUDPSocket::Write(const char8 * const input,
                           uint32 & size,
                           const TimeoutType &timeout) {
    uint32 sizeToWrite = size;
    size = 0u;
    #ifdef LWIP_ENABLED
    if (BasicUDPSocket::Write(input, sizeToWrite)) {
        size = sizeToWrite;
    }
    #endif
    return (size > 0u);
}

uint64 BasicUDPSocket::Size() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicUDPSocket::Seek(const uint64 pos) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicUDPSocket::RelativeSeek(const int64 deltaPos) {
    return false;
}

uint64 BasicUDPSocket::Position() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the size of a socket is undefined. */
bool BasicUDPSocket::SetSize(const uint64 size) {
    return false;
}

}
