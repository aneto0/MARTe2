/**
 * @file GAMSchedulerRecord.cpp
 * @brief Source file for class GAMSchedulerRecord
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
 * the class GAMSchedulerRecord (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMSchedulerRecord.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static const uint32 threadsArrayGranularity = 2u;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMSchedulerRecord::GAMSchedulerRecord() :
        ReferenceContainer() {
    threads = NULL_PTR(ReferenceT<RealTimeThread> *);
    numberOfThreads = 0u;
}

GAMSchedulerRecord::~GAMSchedulerRecord() {
    if (threads != NULL) {
        delete [] threads;
    }
    numberOfThreads=0u;
}

void GAMSchedulerRecord::AddThread(ReferenceT<RealTimeThread> newThread) {
    if (newThread.IsValid()) {
        if ((numberOfThreads % threadsArrayGranularity) == 0u) {
            uint32 newSize = numberOfThreads + threadsArrayGranularity;
            ReferenceT<RealTimeThread> *temp = new ReferenceT<RealTimeThread> [newSize];

            if (threads != NULL) {
                for (uint32 i = 0u; i < numberOfThreads; i++) {
                    temp[i] = threads[i];
                }
                delete[] threads;
            }
            threads = temp;
        }
        /*lint -e{613} before entering the memory for threads is allocated because (numberOfThreads == 0)*/
        threads[numberOfThreads] = newThread;
        numberOfThreads++;

    }
}

ReferenceT<RealTimeThread> GAMSchedulerRecord::Peek(uint32 index) {

    ReferenceT<RealTimeThread> ret;
    if (index < numberOfThreads) {
        ret = threads[index];
    }
    else{
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Position %d out of range (size = %d)", index, numberOfThreads)
    }
    return ret;
}

uint32 GAMSchedulerRecord::GetNumberOfThreads() const {
    return numberOfThreads;
}

}
