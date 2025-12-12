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

#ifdef UDP_CALLBACK_STATS_ENABLE
    #define MAX_NUM_OF_SOCKETS 30
    #define ALPHA 0.975
#endif


namespace MARTe {

#ifdef UDP_CALLBACK_STATS_ENABLE
    uint64_t statsCtr = 0u;
    uint32_t statsCounter = 0u;
    uint32_t currentSockIdentifier = 0u;
    uint32_t statsArray[MAX_NUM_OF_SOCKETS] = { 0u };
    double maxDeltaArray[MAX_NUM_OF_SOCKETS] = { 0.0 };  
    double avgArray[MAX_NUM_OF_SOCKETS] = { 0.0 };
    uint16_t socketPortId[MAX_NUM_OF_SOCKETS] = { 0u };
    //SocketCore* socketsArray[MAX_NUM_OF_SOCKETS] { NULL };
#endif

void UDPRegistrationCallback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    if(arg != NULL) {
        uint64 nowTicks = HighResolutionTimer::Counter();
        SocketCore* tmpSocketCore = static_cast<SocketCore*>(arg);
        //Copy locally the relevant packet data. WARNING! Do NOT use direct memcpy instead of pbuf_copy_partial
        tmpSocketCore->packetLen = pbuf_copy_partial(p, tmpSocketCore->packetBuffer, MAX_RX_PACKET_BUFFERSIZE, 0);

        if((tmpSocketCore->packetLen == 0) || (tmpSocketCore->packetLen > MAX_RX_PACKET_BUFFERSIZE)) {
            pbuf_free(p);
            return;
        }

        tmpSocketCore->packetSourceIpAddress = *addr;
        tmpSocketCore->packetSourcePort = port;
        //Update control variables to emulate sequential read behaviour on socket
        tmpSocketCore->isWritten = true;
        tmpSocketCore->isReadReady = true;
        tmpSocketCore->lastPacketArrivalTimestamp = nowTicks;

        //Free the lwIP buffer and return it to the pbuf pool
        pbuf_free(p);

        #ifdef UDP_CALLBACK_STATS_ENABLE
            uint64 lastDeltaTicks = (uint64)nowTicks - tmpSocketCore->lastPacketArrivalTimestamp;
            double lastDelta = (double)lastDeltaTicks / (double)COUNTS_PER_SECOND;
            avgArray[tmpSocketCore->socketIdentifier] = avgArray[tmpSocketCore->socketIdentifier] + ALPHA * (lastDelta - avgArray[tmpSocketCore->socketIdentifier]);

            if(lastDelta > maxDeltaArray[tmpSocketCore->socketIdentifier]) {
                maxDeltaArray[tmpSocketCore->socketIdentifier] = lastDelta;
            }

            if((statsCtr % 280000) == 0) {
                //uint64 maxArrivalTimestamp;
                //uint64 minArrivalTimestamp = __UINT64_MAX__;

                printf("\r\nHEAD");
                for(int i = 0; i < currentSockIdentifier; i++) {
                    // if(socketsArray[i]->lastPacketArrivalTimestamp > maxArrivalTimestamp) {
                    //     maxArrivalTimestamp = socketsArray[i]->lastPacketArrivalTimestamp;
                    // }
                    // if(socketsArray[i]->lastPacketArrivalTimestamp < minArrivalTimestamp) {
                    //     minArrivalTimestamp = socketsArray[i]->lastPacketArrivalTimestamp;
                    // }
                    printf("%u@%hu |", i, socketPortId[i]);
                }
                printf("\r\nCNT: ");
                for(int i = 0; i < currentSockIdentifier; i++) {
                    printf("%u | ", statsArray[i]);
                }
                printf("\r\n");

                printf("MAX: ");
                for(int i = 0; i < currentSockIdentifier; i++) {
                    printf("%f | ", (double)maxDeltaArray[i]*1.0e6);
                }
                printf("\r\n");

                printf("AVG: ");
                for(int i = 0; i < currentSockIdentifier; i++) {
                    printf("%f | ", avgArray[i]);
                }

                printf("\r\n");
                //printf("DELTA = %u\r\n", (maxArrivalTimestamp - minArrivalTimestamp));
                printf("\r\n---\r\n");
            }

            if((tmpSocketCore->packetCounter % 28000) == 0) {
                maxDeltaArray[tmpSocketCore->socketIdentifier] = 0;
                
            }
            statsArray[tmpSocketCore->socketIdentifier]++;
            tmpSocketCore->packetCounter++;
            statsCtr++;
        #endif
    }

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
        
        #ifdef UDP_CALLBACK_STATS_ENABLE
            connectionSocket.socketIdentifier = currentSockIdentifier;
            socketPortId[currentSockIdentifier] = port;
            //socketsArray[currentSockIdentifier] = &connectionSocket;
            currentSockIdentifier++;
            printf("\r\n\r\n------>Setting port %hu in slot %u\r\n\r\n", port, connectionSocket.socketIdentifier);
        #endif

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
        REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "Read currently does not support reading more than 1700 bytes per call");
    } else {
        uint64 currentTicks = HighResolutionTimer::Counter();
        uint64 endTicks = 0u;
        
        if(timeout.IsFinite()) {
            endTicks = currentTicks + timeout.HighResolutionTimerTicks();
        }
    
        while(readRetry) {

            canRead = connectionSocket.isWritten;

            readRetry = !canRead;
            if(timeout.IsFinite()) {
                if(readRetry) {
                    currentTicks = HighResolutionTimer::Counter();
                    readRetry = (endTicks > currentTicks);
                }
            }

            if(!canRead) {
                NetworkInterfaceHook(NULL);
            }
        }

        if(canRead) {
            //If size is less than connectionSocket.packetLen, the copy will be truncated to "size"
            if(connectionSocket.packetBuffer != output) {
                MemoryOperationsHelper::Copy(output, connectionSocket.packetBuffer, size);
            }
            if(size < connectionSocket.packetLen) {
                //Size updated as less bytes than effectively arrived were copied
                size = connectionSocket.packetLen;
            }
            connectionSocket.isWritten = false;
            connectionSocket.isReadReady = false;
            connectionSocket.isReadSelected = false;

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
