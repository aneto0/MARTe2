/**
 * @file SocketCoreSingleton.h
 * @brief Source file for class SocketCoreSingleton
 * @date 10/06/2022
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

#include "ErrorManagement.h"
#include "HandleI.h"
#include "SocketCore.h"
#include "SocketCoreSingleton.h"

namespace MARTe {
    
    #ifdef LWIP_ENABLED
    SocketCore* SocketCoreSingleton::socketCoreRoot;

    SocketCoreSingleton::SocketCoreSingleton() {
        socketCoreRoot = NULL_PTR(SocketCore*);
    }

    SocketCoreSingleton& SocketCoreSingleton::GetInstance() {
        static SocketCoreSingleton instance;
        return instance;
    }

    SocketCore* SocketCoreSingleton::FindByHandleI(HandleI* handleToSearch) {
        SocketCore* tmpSocketCore = NULL;

        tmpSocketCore = socketCoreRoot;

        bool found = false;

        while((tmpSocketCore != NULL_PTR(SocketCore*)) && (!found)) {
            if(tmpSocketCore->handle == handleToSearch) {
                found = true;
            }
        }

        if(!found) {
            tmpSocketCore = NULL_PTR(SocketCore*);
        }

        return tmpSocketCore;
    }

    bool SocketCoreSingleton::RegisterSocketCore(SocketCore* newSocket) {
        /* TODO: WARNING! Check whether the socket already exists there */
        bool retVal = true;

        if(socketCoreRoot == NULL_PTR(SocketCore*)) {
            socketCoreRoot = newSocket;
            newSocket->nextSocketCore = NULL_PTR(SocketCore*);
        } else {
            SocketCore* tmpSocketCore = socketCoreRoot;

            while(tmpSocketCore->nextSocketCore != NULL_PTR(SocketCore*)) {
                /* Traverse only, until we find the last socket in the list */
                tmpSocketCore = tmpSocketCore->nextSocketCore;
            }

            tmpSocketCore->nextSocketCore = newSocket;
            newSocket->nextSocketCore = NULL_PTR(SocketCore*);
        }
        
        return retVal;
    }

    uint32 SocketCoreSingleton::CountReadyToRead() {
        uint32 readyCount = 0u;
        SocketCore* tmpSocketCore = socketCoreRoot;

        bool canIterate = (tmpSocketCore != NULL_PTR(SocketCore*));

        while(canIterate) {
            if(tmpSocketCore->isReadSelected && tmpSocketCore->isReadReady) {
                readyCount++;
            }

            tmpSocketCore = tmpSocketCore->nextSocketCore;
            canIterate = (tmpSocketCore != NULL_PTR(SocketCore*));
        }

        return readyCount;
    }

    void SocketCoreSingleton::ClearNonReady() {
        SocketCore* tmpSocketCore = socketCoreRoot;

        bool canIterate = (tmpSocketCore != NULL_PTR(SocketCore*));
        
        //Helps emulating the select behaviour. When it finishes, only the ready sockets are kept in the set
        while(canIterate) {
            if(tmpSocketCore->isReadSelected && !tmpSocketCore->isReadReady) {
                tmpSocketCore->isReadSelected = false;
            }

            tmpSocketCore = tmpSocketCore->nextSocketCore;
            canIterate = (tmpSocketCore != NULL_PTR(SocketCore*));
        }
    }

    #endif /* LWIP_ENABLED */
}