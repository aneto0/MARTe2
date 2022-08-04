/**
 * @file SocketCoreSingleton.h
 * @brief Header file for class SocketCoreSingleton
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


#ifndef SOCKETCORESINGLETON_H
#define SOCKETCORESINGLETON_H

#include "CompilerTypes.h"
#include "SocketCore.h"

#ifdef LWIP_ENABLED

namespace MARTe {
    class SocketCoreSingleton {
        
        protected:
            static SocketCore* socketCoreRoot = NULL_PTR(SocketCore*);
            
            SocketCoreSingleton();
            
            SocketCoreSingleton(const SocketCoreSingleton&) = delete;
            
            void operator= (const SocketCoreSingleton&) = delete; 


        public:
            static SocketCoreSingleton& GetInstance();

            static SocketCore* FindByHandleI(HandleI* handleToSearch);

            static bool RegisterSocketCore(SocketCore* newSocket);

            static uint32 CountReadyToRead();

            static void ClearNonReady();
    };
}

#endif /* LWIP_ENABLED */

#endif /* SOCKETCORESINGLETON_H */