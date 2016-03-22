/**
 * @file BasicGAMScheduler.cpp
 * @brief Source file for class BasicGAMScheduler
 * @date 22/mar/2016
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
 * the class BasicGAMScheduler (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicGAMScheduler.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
BasicGAMScheduler::BasicGAMScheduler() :
        GAMSchedulerI() {
    numberOfCycles = MAX_UINT32;
}

void BasicGAMScheduler::StartExecution(const uint32 activeBuffer) {

    if (statesInExecution[activeBuffer]->GetNumberOfThreads() > 1u) {
        //TODO Warning
    }
    ReferenceT<RealTimeThread> thread = statesInExecution[activeBuffer]->Peek(0);
    if (thread.IsValid()) {
        ReferenceT<GAMI> *gamArray = thread->GetGAMs();
        uint32 numberOfGAMs = thread->GetNumberOfGAMs();
        for (uint32 n = 0u; n < numberOfCycles; n++) {
            for (uint32 i = 0u; i < numberOfGAMs; i++) {
                if (gamArray[i].IsValid()) {
                    gamArray[i]->Execute(activeBuffer);
                }
                else {
                    //TODO Invalid gam?
                }
            }
        }
    }
    else {
        //TODO Invalid thread?
    }
}

void BasicGAMScheduler::StopExecution() {
    // nothing to do
}

bool BasicGAMScheduler::Initialise(StructuredDataI &data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        if (data.Read("NumberOfCycles", numberOfCycles)) {
            // default is max uint32
        }
    }
    return ret;
}
CLASS_REGISTER(BasicGAMScheduler, "1.0")

}
