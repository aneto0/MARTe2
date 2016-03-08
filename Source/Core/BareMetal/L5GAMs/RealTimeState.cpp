/**
 * @file RealTimeState.cpp
 * @brief Source file for class RealTimeState
 * @date 19/feb/2016
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
 * the class RealTimeState (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeState.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

const uint32 functionArrayGranularity = 16u;
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeState::RealTimeState() {
    statefulGAMs = NULL_PTR(ReferenceT<GAM>*);
    numberOfFunctions = 0u;
}

RealTimeState::~RealTimeState() {
    if (statefulGAMs != NULL) {
        if(!HeapManager::Free(reinterpret_cast<void*&>(statefulGAMs))) {
            //TODO
        }
    }
}

bool RealTimeState::Validate(RealTimeApplication & rtApp) {

    bool ret = true;
    // for each thread call the Validate
    for (uint32 i = 0u; (i < Size()) && (ret); i++) {
        ReferenceT<RealTimeThread> rtThread = Get(i);
        if (rtThread.IsValid()) {
            ret = rtThread->Validate(rtApp, *this);
        }
        else {
            //TODO Error??
        }
    }

    return ret;
}

bool RealTimeState::AddFunction(ReferenceT<GAM> element) {
    bool ret = true;
    if ((numberOfFunctions % functionArrayGranularity) == 0u) {
        Reference<GAM>* temp =
                reinterpret_cast<Reference<GAM>*>(HeapManager::Realloc(statefulGAMs, sizeof(ReferenceT<GAM> ) * (numberOfFunctions + functionArrayGranularity)));
        ret = (temp != NULL);
        if (ret) {
            statefulGAMs = temp;
        }
        else {
            //TODO Error in reallocation
        }
    }

    if (ret) {
        statefulGAMs[numberOfFunctions] = element;
        numberOfFunctions++;
    }

    return ret;

}

void RealTimeState::ChangeState() {
    for (uint32 i = 0u; i < numberOfFunctions; i++) {

        // to be developed
        statefulGAMs[i]->ChangeState();
    }
}

}
