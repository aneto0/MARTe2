/**
 * @file SocketCore.h
 * @brief Header file for class SocketCore
 * @date 19/04/2019
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

 * @details This header file contains the declaration of the class SocketCore
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOCKETCORE_H_
#define SOCKETCORE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#ifdef LWIP_ENABLED
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/pbuf.h"
#endif

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
#ifdef LWIP_ENABLED

//Use 9000 to support Jumbo Frame
#define MAX_RX_PACKET_BUFFERSIZE 1700

/**
 * When lwIP operates in raw mode, two different handles are needed, based on the underlying socket kind
 */

typedef enum _e_socketcore_kind {
    SocketCoreKindUndefined,
    SocketCoreKindTCP,
    SocketCoreKindUDP
}SocketCoreKind;

class SocketCore {
    public:

       
        /**
         * @brief The UDP handle (PCB)
         */
        udp_pcb *UDPHandle;

        /**
         * @brief The TCP handle (PCB)
         */
        tcp_pcb *TCPHandle;

        /**
         * @brief Indicates the underlying socket type
         */
        SocketCoreKind socketKind;

        /**
         * @brief Latest packet from the lwIP stack
         * 
         */
        char8* packetBuffer;

        /**
         * @brief Latest packet length from the lwIP stack
         * 
         */
        uint32 packetLen;

        /**
         * @brief Latest packet source IP Address
         */
        ip_addr_t packetSourceIpAddress;

        /**
         * @brief Latest packet source port
         */
        uint16 packetSourcePort;

        /**
         * @brief Contains the HRT Counter marked on packet arrival
         * 
         */
        uint64 lastPacketArrivalTimestamp;

        /**
         * @brief Indicates whether the socket has been written (packet received) from the last time it was consumed (read or selected)
         * 
         */
        bool isWritten;

        /**
         * @brief Indicates whether the socket has been prepared for the read select operation
         * 
         */
        bool isReadSelected;

        /**
         * @brief Indicates whether the socket read select signal has been raised
         */
        bool isReadReady;

        /**
         * @brief Timestamp on which the select was raised (oldest)
         * 
         */
        uint64 readReadyAt;

        /**
         * @brief The handle of the socket
         * 
         */
        HandleI *handle;

        /**
         * @brief Simply linked list of currently active (created) sockets
         * 
         */
        SocketCore* nextSocketCore;
    
};

#endif /* LWIP_ENABLED */
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOCKETCORE_H_ */

