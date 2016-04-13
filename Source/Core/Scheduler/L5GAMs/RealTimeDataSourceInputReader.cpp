/**
 * @file RealTimeDataSourceInputReader.cpp
 * @brief Source file for class RealTimeDataSourceInputReader
 * @date 09/mar/2016
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
 * the class RealTimeDataSourceInputReader (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceInputReader.h"
#include "RealTimeDataSourceDef.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeDataSourceInputReader::RealTimeDataSourceInputReader() :
        BasicRealTimeDataSourceInputReader() {
    eventSem = NULL_PTR(EventSem *);
}

bool RealTimeDataSourceInputReader::Read(const uint8 activeDataSourceBuffer) const {

    bool ret = finalised;
    for (uint32 i = 0u; (i < GAMOffsets.GetSize()) && (ret); i++) {
        void ** DSPointer = NULL_PTR(void **);
        ret = DSPointers[activeDataSourceBuffer].Peek(i, DSPointer);
        if (ret) {
            ret = (DSPointer != NULL);
        }
        void * GAMPointer = NULL_PTR(void *);
        if (ret) {
            ret = GAMPointers.Peek(i, GAMPointer);
        }
        if (ret) {
            ret = (GAMPointer != NULL);
        }
        uint32 size = 0u;
        if (ret) {
            ret = sizes.Peek(i, size);
        }
        BasicRealTimeDataSourceDef *dsDef = NULL_PTR(BasicRealTimeDataSourceDef *);
        if (ret) {
            ret = dataSources.Peek(i, dsDef);

        }
        if (ret) {
            ret = (dsDef != NULL);
        }
        if (ret) {
            dsDef->ReadStart();
            ret = MemoryOperationsHelper::Copy(GAMPointer, *DSPointer, size);
            dsDef->ReadEnd();
        }
    }

    return ret;
}

bool RealTimeDataSourceInputReader::Finalise() {

    bool ret = RealTimeDataSourceBroker::Finalise();

    if (ret) {
        uint32 numberOfDS = dataSources.GetSize();
        for (uint32 i = 0u; i < numberOfDS; i++) {
            BasicRealTimeDataSourceDef * bdsDef = NULL_PTR(BasicRealTimeDataSourceDef *);
            ret = dataSources.Peek(i, bdsDef);
            if (ret) {
                RealTimeDataSourceDef *dsDef = dynamic_cast<RealTimeDataSourceDef *>(bdsDef);
                ret = (dsDef != NULL);
                if (ret) {
                    EventSem *tempSem = dsDef->GetEventSemaphore();
                    if (tempSem != NULL) {
                        ret=(!synchronized);
                        if(ret) {
                            eventSem=tempSem;
                            synchronized=true;
                        }
                        else {
                            //TODO Already sync  !!
                        }
                    }
                }
            }
        }
    }

    finalised = ret;
    return ret;
}

bool RealTimeDataSourceInputReader::SynchroniseOnEventSem(const uint8 activeDataSourceBuffer,
                                                          float64 sampleTime,
                                                          uint32 numberOfReads,
                                                          TimeoutType timeout,
                                                          float64 sleepTime) {
    uint64 tic = HighResolutionTimer::Counter();
    bool ret = true;
    if (synchronized) {
        ret = (eventSem != NULL);
        // blocks the function on the spin-lock
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            ret=(eventSem->ResetWait(timeout)==ErrorManagement::NoError);
        }

        if (ret) {
            ret = Read(activeDataSourceBuffer);
        }

    }
    // performs a single read
    else {
        ret = Read(activeDataSourceBuffer);
    }

    if (ret && synchronized) {
        // wait the sample time
        // possible error on counter overflow
        if (sleepTime < 0.0) {
            Sleep::Sec(sampleTime - HighResolutionTimer::TicksToTime(HighResolutionTimer::Counter(), tic));
        }
        else {
            while (HighResolutionTimer::TicksToTime(HighResolutionTimer::Counter(), tic) < sampleTime) {
                Sleep::Sec(sleepTime);
            }
        }
    }

    return ret;
}

CLASS_REGISTER(RealTimeDataSourceInputReader, "1.0")

}
