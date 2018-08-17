/**
 * @file ExecutionInfo.cpp
 * @brief Source file for class ExecutionInfo
 * @date 20/09/2016
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
 * the class ExecutionInfo (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#define DLL_API

#include "ExecutionInfo.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ExecutionInfo::ExecutionInfo() {
    Reset();
}

void ExecutionInfo::SetThreadNumber(const uint32 number) {
    if (stage == StartupStage) {
        threadNumber = number;
    }
}

void ExecutionInfo::SetStage(const uint8 number) {
    stage = number;
}

void ExecutionInfo::SetStageSpecific(const uint8 number) {
    stageSpecific = number;
}

uint32 ExecutionInfo::GetThreadNumber() const {
    return threadNumber;
}

uint8 ExecutionInfo::GetStage() const {
    return stage;
}

uint8 ExecutionInfo::GetStageSpecific() const {
    return stageSpecific;
}

void ExecutionInfo::Reset() {
    threadNumber = 0xFFFFu;
    stage = StartupStage;
    stageSpecific = NullStageSpecific;

}
}
