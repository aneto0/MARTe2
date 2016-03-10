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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceInputReader.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeDataSourceInputReader::RealTimeDataSourceInputReader() {

}

bool RealTimeDataSourceInputReader::Read(uint8 activeDataSourceBuffer) {

    bool ret = true;
    for (uint32 i = 0u; (i < GAMOffsets.GetSize()) && (ret); i++) {
        void ** DSPointer = NULL;
        DSPointers[activeDataSourceBuffer].Peek(i, DSPointer);
        ret=(DSPointer!=NULL);
        void * GAMPointer=NULL;
        uint32 offset=0u;
        if(ret) {
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
            printf("\ntest in read %d size = %d, offs = %d\n", *(uint32*)(*DSPointer), size, offset);
            ret=MemoryOperationsHelper::Copy(GAMPointer, *DSPointer, size);
        }
    }

    return ret;
}

}
