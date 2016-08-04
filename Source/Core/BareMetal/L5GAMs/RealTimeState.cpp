/**
 * @file RealTimeState.cpp
 * @brief Source file for class RealTimeState
 * @date 19/02/2016
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
 * the class RealTimeState (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "RealTimeState.h"
#include "RealTimeThread.h"
#include "ReferenceContainerFilterReferences.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeState::RealTimeState() :
        ReferenceContainer() {
}

RealTimeState::~RealTimeState() {
}

bool RealTimeState::AddStatefuls(ReferenceContainer &statefulsIn) {
    uint32 numberOfStatefuls = statefulsIn.Size();
    bool ret = true;
    for (uint32 i = 0u; (i < numberOfStatefuls) && (ret); i++) {
        ReferenceT<StatefulI> toAdd = statefulsIn.Get(i);
        ret = toAdd.IsValid();
        if (ret) {
            ReferenceContainer result;
            ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::SHALLOW, toAdd);
            statefuls.Find(result, filter);
            if (result.Size() == 0u) {
                ret = statefuls.Insert(toAdd);
            }
        }
    }
    return ret;
}

bool RealTimeState::PrepareNextState(const char8 * const currentStateName,
                                     const char8 * const nextStateName) {
    uint32 numberOfStatefuls = statefuls.Size();
    bool ret = true;
    for (uint32 i = 0u; (i < numberOfStatefuls) && (ret); i++) {
        ReferenceT<StatefulI> stateful = statefuls.Get(i);
        ret = stateful.IsValid();
        if (ret) {
            ret = stateful->PrepareNextState(currentStateName, nextStateName);
        }
    }
    return ret;
}

uint32 RealTimeState::GetNumberOfStatefuls() {
    return statefuls.Size();
}

CLASS_REGISTER(RealTimeState, "1.0");

}
