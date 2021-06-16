/**
 * @file GAMBareScheduler.cpp
 * @brief Source file for class GAMBareScheduler
 * @date 6 Aug 2016
 * @author andre
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
 * the class GAMBareScheduler (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "GAMBareScheduler.h"
#include "RealTimeApplication.h"

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
GAMBareScheduler::GAMBareScheduler() {
    scheduledStates = NULL;
}

bool GAMBareScheduler::ConfigureScheduler(Reference realTimeAppIn) {
    bool ret = GAMSchedulerI::ConfigureScheduler(realTimeAppIn);
    if (ret) {
        scheduledStates = GetSchedulableStates();
        realTimeApplication = realTimeApp;
        ret = realTimeApplication.IsValid();
    }
    return ret;
}

ErrorManagement::ErrorType GAMBareScheduler::StartNextStateExecution() {
    while(1) { Cycle(0); }
    return ErrorManagement::NoError;
}

void GAMBareScheduler::CustomPrepareNextState() {
    
}

ErrorManagement::ErrorType GAMBareScheduler::StopCurrentStateExecution() {
    return ErrorManagement::NoError;
}

void GAMBareScheduler::Cycle(uint32 threadId) {
    
    uint32 rtAppIndex = realTimeApplication->GetIndex();

    ExecuteSingleCycle(
        scheduledStates[rtAppIndex]->threads[threadId].executables, 
        scheduledStates[rtAppIndex]->threads[threadId].numberOfExecutables);
}
CLASS_REGISTER(GAMBareScheduler, "1.0")
}

