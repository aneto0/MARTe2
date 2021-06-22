/**
 * @file GAMBareScheduler.cpp
 * @brief Source file for class GAMBareScheduler
 * @date 01/06/2021
 * @author Andre Neto
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

GAMBareScheduler::GAMBareScheduler() : GAMSchedulerI() {
    isAlive = false;
    maxCycles = 0u;
    nCycle = 0u;
}

/*lint -e{1540} the scheduledStates life-cycle is managed by the GAMSchedulerI*/
GAMBareScheduler::~GAMBareScheduler() {
}

bool GAMBareScheduler::ConfigureScheduler(Reference realTimeAppIn) {
    bool ret = GAMSchedulerI::ConfigureScheduler(realTimeAppIn);
    if (ret) {
        realTimeApplication = realTimeApp;
        ret = realTimeApplication.IsValid();
    }
    return ret;
}

ErrorManagement::ErrorType GAMBareScheduler::StartNextStateExecution() {
    isAlive = true;
    
    while(isAlive) { 
        Cycle(0u); 
        if (maxCycles != 0u) {
            isAlive = (nCycle < maxCycles);
            nCycle++;
        }
    }
    
    return ErrorManagement::NoError;
}

bool GAMBareScheduler::Initialise(StructuredDataI & data) {
    bool ok = GAMSchedulerI::Initialise(data);
    if (ok) {
        (void) (data.Read("MaxCycles", maxCycles));
        REPORT_ERROR(ErrorManagement::Information, "MaxCycles set to %d", maxCycles);
    }
    return ok;
}

void GAMBareScheduler::CustomPrepareNextState() {   
    nCycle = 0u;
}

ErrorManagement::ErrorType GAMBareScheduler::StopCurrentStateExecution() {
    isAlive = false;
    return ErrorManagement::NoError;
}

/*lint -e{1762} function cannot be made constant as it indirectly modifies the scheduledStates.*/
void GAMBareScheduler::Cycle(const uint32 threadId) {
    
    uint32 rtAppIndex = realTimeApplication->GetIndex();
    /*lint -e{613} scheduledStates != NULL as otherwise StartNextStateExecution (and thus Cycle) would never be called.*/
    (void) ExecuteSingleCycle(
        scheduledStates[rtAppIndex]->threads[threadId].executables, 
        scheduledStates[rtAppIndex]->threads[threadId].numberOfExecutables);
}
CLASS_REGISTER(GAMBareScheduler, "1.0")
}

