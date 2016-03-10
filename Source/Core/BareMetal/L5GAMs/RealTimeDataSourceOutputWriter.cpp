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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeDataSourceOutputWriter::RealTimeDataSourceOutputWriter() {

}

bool RealTimeDataSourceOutputWriter::Write(uint8 activeDataSourceBuffer) {

    bool ret = true;
    for (uint32 i = 0u; (i < GAMOffsets.GetSize()) && (ret); i++) {
        void ** DSPointer = NULL;
        DSPointers[activeDataSourceBuffer].Peek(i, DSPointer);
        ret=(DSPointer!=NULL);
        void * GAMPointer=NULL;
        if(ret) {
            uint32 offset=0u;
            GAMOffsets.Peek(i, offset);
            GAMPointer=memory.GetPointer(offset);
            ret=(GAMPointer!=NULL);
        }
        uint32 size=0u;
        if(ret) {
            sizes.Peek(i, size);
            ret=(size!=0u);
        }
        if(ret) {
            ret=MemoryOperationsHelper::Copy( *DSPointer, GAMPointer, size);
        }
    }

    return ret;
}
}
