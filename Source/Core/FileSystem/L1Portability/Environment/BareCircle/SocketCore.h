/**
 * @file SocketCore.h
 * @brief Header file for class SocketCore
 * @date 27/10/2015
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

 * @details This header file contains the declaration of the class SocketCore
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOCKETCORE_H_
#define SOCKETCORE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <circle/net/in.h>
#include <circle/net/ipaddress.h>
#include <circle/net/socket.h>
#include <circle/netdevice.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

    typedef enum {
        SocketKindUndefined,
        SocketKindTCP,
        SocketKindUDP
    }SocketKind;

    class SocketCore {
        public:
            /**
             * @brief Construct a new Socket Core object
             */
            SocketCore();

            /**
             * @brief Destroy the Socket Core object
             */
            ~SocketCore();

            /**
             * @brief Type of socket, which can be TCP, UDP or undefined
             */
            SocketKind socketKind;

            /**
             * @brief Set the inner CSocket object to the parameter value
             * 
             * @param inputSocket The CSocket to copy inside
             * @return true if the passed socket is not null
             */
            bool SetSocket(CSocket *inputSocket, SocketKind inputSocketKind);

            /**
             * @brief Creates the instance of the inner CSocket with the specified transport layer
             * 
             * @param socketKind[in] TCP or UDP socket type only supported
             * @return true If the creation succeeds, false otherwise
             */
            bool Create(SocketKind socketKind);

            /**
             * @brief   Destroys the instance of the inner CSocket closing/terminating/aborting
             *          the current sessions
             */
            void Destroy();

            /**
             * @brief CSocket inner instance
             */
            CSocket* socket;
    };

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOCKETCORE_H_ */

