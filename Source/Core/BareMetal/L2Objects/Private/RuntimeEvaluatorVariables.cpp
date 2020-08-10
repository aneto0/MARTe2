/**
 * @file RuntimeEvaluatorVariables.cpp
 * @brief Header file for class AnyType
 * @date Jun 18, 2020
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#include "RuntimeEvaluatorVariables.h"

namespace MARTe{


RuntimeEvaluatorVariables::RuntimeEvaluatorVariables(RuntimeEvaluator::DataMemoryElement *&variablesMemoryPtr):memory(variablesMemoryPtr){
    allocatedSize   = 0;
    usedSize        = 0;
    DeAllocate();
//    memory = NULL_PTR(DataMemoryElement *);
}

void RuntimeEvaluatorVariables::DeAllocate(){
    if (memory != NULL_PTR(RuntimeEvaluator::DataMemoryElement *)){
        HeapManager::Free((void *&)memory);
    }
}

RuntimeEvaluatorVariables::~RuntimeEvaluatorVariables(){
}

void RuntimeEvaluatorVariables::Clean(){
    usedSize = 0;
}

ErrorManagement::ErrorType RuntimeEvaluatorVariables::RequestMemory(RuntimeEvaluator::DataMemoryAddress size){
    ErrorManagement::ErrorType ret;
    if ((usedSize + size) >= allocatedSize){
        uint32 newAllocatedSize = allocatedSize + 64;
        memory = reinterpret_cast<RuntimeEvaluator::DataMemoryElement *>(HeapManager::Realloc((void *&)memory, newAllocatedSize*sizeof(RuntimeEvaluator::DataMemoryElement)));

        ret.outOfMemory = (memory == NULL_PTR(RuntimeEvaluator::DataMemoryElement *));

        if (ret){
            allocatedSize = newAllocatedSize;
        } else {
            allocatedSize = 0;
        }
    }

    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluatorVariables::AllocateBySize(uint32 byteSize, RuntimeEvaluator::DataMemoryAddress &location){
    ErrorManagement::ErrorType ret;
    RuntimeEvaluator::DataMemoryAddress size = RuntimeEvaluator::ByteSizeToDataMemorySize(byteSize);

    ret = RequestMemory(size);

    if (ret){
        location = usedSize;
        usedSize += size;
    }

    return ret;
}


} //MARTe
