/**
 * @file SocketCore.cpp
 * @brief Source file for class SocketCore
 * @date 23 apr 2019
 * @author pc
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
 * the class SocketCore (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "../Pi3_BM/SocketCore.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

SocketCore::SocketCore() {
    socket=NULL;
    isTCP=true;
}


SocketCore::~SocketCore() {
    if(socket!=NULL){
        delete socket;
    }
}

void SocketCore::SetSocket(CSocket *socketIn) {
    Close();
    socket=socketIn;
}


void SocketCore::OpenTCP() {
    Close();
    socket=new CSocket(CNetSubSystem::Get(), IPPROTO_TCP);
    isTCP=true;
}

void SocketCore::OpenUDP() {
    Close();
    socket=new CSocket(CNetSubSystem::Get(), IPPROTO_UDP);
    isTCP=false;
}

void SocketCore::Close(){
    if(socket!=NULL){
        delete socket;
    }
}




}

