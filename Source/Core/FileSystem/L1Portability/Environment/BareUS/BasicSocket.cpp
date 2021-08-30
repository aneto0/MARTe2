/**
 * @file BasicSocket.cpp
 * @brief Source file for class BasicSocket
 * @date 26/10/2015
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
 * the class BasicSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicSocket.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    BasicSocket::BasicSocket() :
            StreamI(),
            HandleI(),
            source(),
            destination(),
            connectionSocket() {

    }

    BasicSocket::~BasicSocket() {

    } 

    bool BasicSocket::SetBlocking(const bool flag) {
        isBlocking = flag;
        //Always returns true, as we deal internally with the variable
        return true;
    }

    bool BasicSocket::Close() {
        connectionSocket.Destroy();
        return true;
    }

    InternetHost BasicSocket::GetSource() const {
        return source;
    }

    InternetHost BasicSocket::GetDestination() const {
        return destination;
    }

    void BasicSocket::SetDestination(const InternetHost &destinationIn) {
        destination = destinationIn;
    }

    void BasicSocket::SetSource(const InternetHost &sourceIn) {
        source = sourceIn;
    }

    bool BasicSocket::IsValid() const {
        return false;
    }

    Handle BasicSocket::GetReadHandle() const {
        return (void*)(&connectionSocket);
    }

    Handle BasicSocket::GetWriteHandle() const {
        return (void*)(&connectionSocket);
    }

    bool BasicSocket::IsBlocking() const {
        return isBlocking;
    }

}
