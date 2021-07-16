/**
 * @file SocketCore.cpp
 * @brief Source file for class SocketCore
 * @date 30/04/2020
 * @author Giuseppe Avon
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

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ErrorManagement.h"
#include "SocketCore.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

extern CNetSubSystem *networkSubSystem;

namespace MARTe {
    SocketCore::SocketCore() {
        socketKind = SocketKindUndefined;
        socket = NULL_PTR(CSocket*);
    }

    SocketCore::~SocketCore() {
        socketKind = SocketKindUndefined;
        if(socket != NULL_PTR(CSocket*)) {
            //CSocket::~CSocket() destroys the object and terminates any active connection
            delete socket;
        }
    }

    bool SocketCore::Create(SocketKind socketKind) {
        
        bool retVal = true;

        if(socket != NULL_PTR(CSocket*)) {
            REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "SocketCore::Create() Create call on already created socket, old socket will be dismissed");
            delete socket;
        }
        
        socket = NULL_PTR(CSocket*);
        
        retVal = (networkSubSystem != NULL);

        if(!retVal) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SocketCore::Create() Failure retrieving the network subsystem");
        }

        if(retVal) {
            switch(socketKind) {
                case SocketKindTCP:
                    REPORT_ERROR_STATIC_0(ErrorManagement::Information, "SocketCore::Create() TCP Socket created");
                    socket = new CSocket(networkSubSystem, IPPROTO_TCP);
                    this->socketKind = SocketKindTCP;
                    break;
                case SocketKindUDP:
                    REPORT_ERROR_STATIC_0(ErrorManagement::Information, "SocketCore::Create() UDP Socket created");
                    socket = new CSocket(networkSubSystem, IPPROTO_UDP);
                    this->socketKind = SocketKindUDP;
                    break;
                default:
                    retVal = false;
                    this->socketKind = SocketKindUndefined;
                    REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "SocketCore::Create() Specified socket kind is invalid");
                    break;
            }
        }

        if(socket == NULL_PTR(CSocket*)) {
            this->socketKind = SocketKindUndefined;
            retVal = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SocketCore::Create() Create socket call failed");
        }

        return retVal;
    }

    void SocketCore::Destroy() {
        if(socket != NULL_PTR(CSocket*)) {
            socketKind = SocketKindUndefined;

            //CSocket::~CSocket() destroys the object and terminates any active connection
            delete socket;
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "SocketCore::Destroy() Destroying an already null socket, no action taken");

        }
    }

    bool SocketCore::SetSocket(CSocket *inputSocket, SocketKind inputSocketKind) {
        bool retVal = true;

        if(inputSocket == NULL_PTR(CSocket*)) {
            retVal = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "SocketCore::SetSocket() Set called with null socket");
        }

        if(retVal) {
            if((inputSocketKind != SocketKindTCP) && (inputSocketKind != SocketKindUDP)) {
                retVal = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "SocketCore::SetSocket() Set called with invalid socket kind");
            }
        }

        if(socket != NULL_PTR(CSocket*)) {
            REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "SocketCore::SetSocket() Set called on already assigned socket, previous is being destroyed");
            Destroy();
        }
        
        if(retVal) {
            socket = inputSocket;
            socketKind = inputSocketKind;
        }

        return retVal;
    }
    
}