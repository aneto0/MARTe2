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
#include "sleep.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "BasicUDPSocket.h"
#include "HighResolutionTimer.h"
#include "MemoryOperationsHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#ifdef LWIP_ENABLED
extern void NetworkInterfaceHook(void* netifParams);
void EnqueueJoinRequest(udp_pcb *pcb, ip4_addr_t *ifaddr, ip4_addr_t *groupaddr);
#endif

namespace MARTe {

void UDPRegistrationCallback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    SocketCore* tmpSocketCore = static_cast<SocketCore*>(arg);
    //uint64 nowTicks = HighResolutionTimer::Counter();

    //Copy locally the relevant packet data. WARNING! Do NOT use direct memcpy instead of pbuf_copy_partial
    tmpSocketCore->packetLen = pbuf_copy_partial(p, tmpSocketCore->packetBuffer, MAX_RX_PACKET_BUFFERSIZE, 0);

    tmpSocketCore->packetSourceIpAddress = *addr;
    tmpSocketCore->packetSourcePort = port;

    //Update control variables to emulate sequential read behaviour on socket
    tmpSocketCore->isWritten = true;
    //tmpSocketCore->lastPacketArrivalTimestamp = nowTicks;

    //Update control variables to emulate read select behaviour on socket
    if(tmpSocketCore->isReadSelected && !tmpSocketCore->isReadReady) {
        tmpSocketCore->isReadReady = true;
        //tmpSocketCore->readReadyAt = nowTicks;
    }

    //Free the lwIP buffer and return it to the pbuf pool
    pbuf_free(p);
}

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

    bool ret = false;
#ifdef LWIP_ENABLED
    ret = Read(output, size, TTInfiniteWait);
#endif
    return ret;
}

bool BasicUDPSocket::Write(const char8* const input,
                           uint32 &size) {
    bool retVal = false;

#ifdef LWIP_ENABLED    
    ip_addr_t destIPAddress;

    destIPAddress = (destination.GetInternetHost())->addr;
    uint16 destPort = (destination.GetInternetHost())->port;

    struct pbuf *tmpPacketBuffer = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
    
    //WARNING! Do not use memcpy to fill the packet payload
    pbuf_take(tmpPacketBuffer, (void*)input, (uint16)size);
    err_t err = udp_sendto(connectionSocket.UDPHandle, tmpPacketBuffer, &destIPAddress, destPort);
    pbuf_free(tmpPacketBuffer);

    NetworkInterfaceHook(NULL);

    retVal = (err == ERR_OK);
#endif

    return retVal;
}

bool BasicUDPSocket::Join(const char8 * const group, const char8 * const multicastInterfaceAddress) const {
    //TODO: actually there is only one network interface to manage, so the address is implicitly omitted here
    //In the event of managing multiple interfaces, one should take into account the possibility to tell apart interfaces and select
    bool ok = false;
    #ifdef LWIP_ENABLED
    ok = Join(group);
    #endif /* LWIP_ENABLED */

    return ok;
}

bool BasicUDPSocket::Join(const char8 *const group) const {
    //TODO: Can the real value for the Join Request, be checked?
    //WARNING: in Xilinx SDK 2018.1 this requires a patching in the xemacpsif porting, to always enable interface hash
    //Otherwise, no more than 5 sockets can be joined
    bool ok = true;
    
    ip_addr_t multicastIp;
    
    multicastIp.addr = ipaddr_addr(group);
    
    //err_t iret = igmp_joingroup(IP_ADDR_ANY, (const struct ip4_addr *)(&multicastIp));
    EnqueueJoinRequest(connectionSocket.UDPHandle, IP_ADDR_ANY, (const struct ip4_addr *)(&multicastIp));
    NetworkInterfaceHook(NULL);
    
    //ok = (iret == ERR_OK);
    
    //if(ok) {
        //udp_set_multicast_netif_addr(connectionSocket.UDPHandle,(const struct ip4_addr *)(&multicastIp));
        //TODO: Evaluate right multicast ttl, if necessary
        //udp_set_multicast_ttl(connectionSocket.UDPHandle, 5);
    //}

    return ok;
}

bool BasicUDPSocket::Open() {
#ifdef LWIP_ENABLED
    connectionSocket.UDPHandle = udp_new();
    //Here we have to register the receive callback
    udp_recv(connectionSocket.UDPHandle, UDPRegistrationCallback, static_cast<void*>(&connectionSocket));
    return (connectionSocket.UDPHandle != NULL);
#else
    return false;
#endif
}

/*lint -e{1762}  [MISRA C++ Rule 9-3-3]. Justification: The function member could be non-const in other operating system implementations*/
bool BasicUDPSocket::Listen(const uint16 port) {
    bool retVal = false;
    
    #ifdef LWIP_ENABLED
        err_t err = udp_bind(connectionSocket.UDPHandle, IP_ADDR_ANY, port);
        retVal = (err == ERR_OK);
    #endif /* ! LWIP_ENABLED */

    return retVal;
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
    
    bool retVal = false;
    bool readRetry = true;
    bool canRead = false;

    retVal = (size <= MAX_RX_PACKET_BUFFERSIZE);

    if(!retVal) {
        REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "Read currently does not support reading more than 9000 bytes per call");
    } else {
        uint64 currentTicks = HighResolutionTimer::Counter();
        uint64 endTicks = 0u;
        
        if(timeout.IsFinite()) {
            endTicks = currentTicks + timeout.HighResolutionTimerTicks();
        }
    
        while(readRetry) {
            NetworkInterfaceHook(NULL);
            
            canRead = connectionSocket.isWritten;

            readRetry = !canRead;
            if(timeout.IsFinite()) {
                if(readRetry) {
                    currentTicks = HighResolutionTimer::Counter();
                    readRetry = (endTicks > currentTicks);
                }
            }
        }

        if(canRead) {
            //TODO: Check this
            //retVal = (size <= connectionSocket.packetLen);
            
            if(retVal) {
                size = connectionSocket.packetLen;
                if(connectionSocket.packetBuffer != output) {
                    MemoryOperationsHelper::Copy(output, connectionSocket.packetBuffer, size);
                }
                
                connectionSocket.isWritten = false;
                connectionSocket.isReadReady = false;
                connectionSocket.isReadSelected = false;
            }
        } else {
            size = 0u;
        }
    }

    return retVal;
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
