/**
 * @file MemoryMapStatefulInputBroker.cpp
 * @brief Source file for class MemoryMapStatefulInputBroker
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
 * the class MemoryMapStatefulInputBroker (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MemoryMapStatefulInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
MemoryMapStatefulInputBroker::MemoryMapStatefulInputBroker() {

}

MemoryMapStatefulInputBroker::~MemoryMapStatefulInputBroker() {

}

bool MemoryMapStatefulInputBroker::Execute() {
    uint32 n;
    for (n = 0u; n < numberOfCopies; n++) {
        MemoryOperationsHelper::Copy(copyTable[n].gamPointer, copyTable[n].dataSourcePointer[RealTimeApplication::index][copyTable[n].dataSourceOffset], copyTable[n].copySize);
    }
    return true;
}

CLASS_REGISTER(MemoryMapStatefulInputBroker, "1.0")
}

