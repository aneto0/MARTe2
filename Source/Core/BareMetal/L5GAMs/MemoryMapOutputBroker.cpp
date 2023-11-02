/**
 * @file MemoryMapOutputBroker.cpp
 * @brief Source file for class MemoryMapOutputBroker
 * @date 18/07/2016
 * @author Andre Neto
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
 * the class MemoryMapOutputBroker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MemoryMapOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
MemoryMapOutputBroker::MemoryMapOutputBroker() :
        MemoryMapBroker() {

}

MemoryMapOutputBroker::~MemoryMapOutputBroker() {

}

bool MemoryMapOutputBroker::Execute() {
    uint32 n;
    /*lint -e{613} null pointer checked before.*/
    uint32 i = dataSource->GetCurrentStateBuffer();
    bool ret = true;
    for (n = 0u; (n < numberOfCopies) && (ret); n++) {
        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
            uint32 dataSourceIndex = ((i * numberOfCopies) + n);
            ret = MemoryOperationsHelper::Copy(copyTable[dataSourceIndex].dataSourcePointer, copyTable[n].gamPointer, copyTable[n].copySize);
        }
    }
    return ret;
}

CLASS_REGISTER(MemoryMapOutputBroker, "1.0")
}

