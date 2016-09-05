/**
 * @file EmbeddedServiceI.cpp
 * @brief Source file for class EmbeddedServiceI
 * @date Sep 5, 2016
 * @author fsartori
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
 * the class EmbeddedServiceI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EmbeddedServiceI.h"

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
EmbeddedServiceI::ExecutionInfo::ExecutionInfo(){
    Reset();
}


void EmbeddedServiceI::ExecutionInfo::SetThreadNumber(uint16 number){
    if (stage == startupStage){
        threadNumber = number;
    }
}

void EmbeddedServiceI::ExecutionInfo::SetStage(uint8 number){
    stage = number;
}

void EmbeddedServiceI::ExecutionInfo::SetStage2(uint8 number){
    stage2 = number;
}

uint16 EmbeddedServiceI::ExecutionInfo::GetThreadNumber(){
    return threadNumber;
}

uint8 EmbeddedServiceI::ExecutionInfo::GetStage(){
    return stage;
}

uint8 EmbeddedServiceI::ExecutionInfo::GetStage2(){
    return stage2;
}

void EmbeddedServiceI::ExecutionInfo::Reset(){
    threadNumber = 0;
    stage = startupStage;
    stage2 = nullStage2;

}

}
