/**
 * @file RealTimeDataSourceOutputWriter.cpp
 * @brief Source file for class RealTimeDataSourceOutputWriter
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
 * the class RealTimeDataSourceOutputWriter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceOutputWriter.h"
#include "RealTimeDataSourceDef.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeDataSourceOutputWriter::RealTimeDataSourceOutputWriter() :
        BasicRealTimeDataSourceOutputWriter() {
    eventSem = NULL_PTR(EventSem *);
}

bool RealTimeDataSourceOutputWriter::Write(const uint8 activeDataSourceBuffer) const {

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
            dsDef->WriteStart();
            ret = MemoryOperationsHelper::Copy(*DSPointer, GAMPointer, size);
            dsDef->WriteEnd();
        }
    }

    return ret;
}

bool RealTimeDataSourceOutputWriter::Finalise() {

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
                        if(!synchronized) {
                            if(eventSem==NULL) {
                                eventSem=tempSem;
                                synchronized=true;
                            }
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

CLASS_REGISTER(RealTimeDataSourceOutputWriter, "1.0")

}
