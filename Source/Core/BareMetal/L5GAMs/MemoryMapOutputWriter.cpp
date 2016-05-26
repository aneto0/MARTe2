/**
 * @file MemoryMapOutputWriter.cpp
 * @brief Source file for class MemoryMapOutputWriter
 * @date 11/apr/2016
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
 * the class MemoryMapOutputWriter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryMapOutputWriter.h"
#include "stdio.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MemoryMapOutputWriter::MemoryMapOutputWriter() :
        MemoryMapDataSourceBroker() {

}

bool MemoryMapOutputWriter::Write(const uint8 activeDataSourceBuffer,
                                  const TimeoutType &timeout) {

    bool ret = finalised;
    for (uint32 i = 0u; (i < GAMOffsets.GetSize()) && (ret); i++) {
        void ** DSPointer = (DSPointers[activeDataSourceBuffer])[i];
        void * GAMPointer = GAMPointers[i];
        DataSourceSignalI *dsDef = dataSourcesVars[i];
        uint32 sampleRows = samplesParamRows[i];
        uint32 **samplesParams = samplesParamList[i];
        uint32 nRows = blockParamRows[i];
        uint32 **blockParams = blockParamList[i];
        uint32 nCycles = numberOfCyclesPerVar[i];


        if (ret) {
            bool isTheSyncOne = ((synchronized) && (syncIndex == i));
            uint32 gamPtrIndex = 0u;
            char8* GAMPointerBeg = &(reinterpret_cast<char8*>(GAMPointer))[0];
            // do how many read operations as the number of cycles
            for (uint32 m = 0u; m < nCycles; m++) {
                if (isTheSyncOne) {
                    ret = dsDef->WaitOnEvent(timeout);
                }
                if (ret) {
                    // loop on the samples blocks. Takes the begin pointer of each block
                    // than added it to the sub block index to be write.
                    dsDef->WriteStart();
                    for (uint32 j = 0u; (j < sampleRows) && (ret); j++) {
                        uint32 samplePtrIndex = samplesParams[j][0];
                        uint32 nSamplesInBlock = samplesParams[j][1];
                        uint32 sampleSize = samplesParams[j][2];
                        // copy the internal variable blocks for each sample in the samples block
                        for (uint32 n = 0u; (n < nSamplesInBlock) && (ret); n++) {
                            for (uint32 k = 0u; (k < nRows) && (ret); k++) {
                                uint32 beg = blockParams[k][0];
                                uint32 size = blockParams[k][1];
                          //      REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "write: beg=%d, size=%d, gamPtrIndex=%d, of %s", beg, size, gamPtrIndex,
                           //                             dsDef->GetName())
                                char8 *DSptr = &(reinterpret_cast<char8*>(*DSPointer)[samplePtrIndex+beg]);
                                // shift the pointer
                                GAMPointer = &GAMPointerBeg[gamPtrIndex];
                                ret = MemoryOperationsHelper::Copy(DSptr, GAMPointer, size);
                                gamPtrIndex += size;
                            }
                            samplePtrIndex += sampleSize;
                        }
                    }
                }
                else {
                    //TODO TImeout expired
                }
                if (ret) {
                    dsDef->WriteEnd();
                }
            }
        }
    }

    return ret;
}

bool MemoryMapOutputWriter::Read(const uint8 activeDataSourceBuffer,
                                 const TimeoutType &timeout) {
    return false;
}


CLASS_REGISTER(MemoryMapOutputWriter, "1.0")

}
