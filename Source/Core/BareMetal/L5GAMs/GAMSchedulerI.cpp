/**
 * @file GAMSchedulerI.cpp
 * @brief Source file for class GAMSchedulerI
 * @date 22/03/2016
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
 * the class GAMSchedulerI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMSchedulerI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMSchedulerI::GAMSchedulerI() {
}

bool GAMSchedulerI::InsertRecord(const char8 * stateName,
                                 ReferenceT<RealTimeThread> thread) {
    uint32 numberOfStates = Size();
    ReferenceT<GAMSchedulerRecord> record;
    bool found = false;
    for (uint32 i = 0u; (i < numberOfStates) && (!found); i++) {
        record = Get(i);
        if (record.IsValid()) {
            found = (stateName == record->GetName());
        }
    }
    bool ret = true;

    // set the accelerator
    if (found) {
        record->AddThread(thread);
    }
    else {
        ReferenceT<GAMSchedulerRecord> newRecord = ReferenceT<GAMSchedulerRecord>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ret = newRecord.IsValid();
        if (ret) {
            newRecord->SetName(stateName);
            ret = Insert(newRecord);
        }
    }

    return ret;
}

bool GAMSchedulerI::PrepareNextState(RealTimeStateInfo info) {
    uint32 numberOfStates = Size();
    StreamString newStateName = info.nextState;
    ReferenceT<GAMSchedulerRecord> record;
    bool found = false;
    for (uint32 i = 0u; (i < numberOfStates) && (!found); i++) {
        record = Get(i);
        if (record.IsValid()) {
            found = (newStateName == record->GetName());
        }
    }
    // set the accelerator
    if (found) {
        uint32 nextBuffer = (info.activeBuffer + 1u) % 2u;
        statesInExecution[nextBuffer] = record;
    }
    return found;
}


void GAMSchedulerI::ChangeState(const uint32 activeBuffer){
    uint32 nextBuffer=(activeBuffer+1u)%2u;

    StopExecution();
    StartExecution(nextBuffer);

}

}
