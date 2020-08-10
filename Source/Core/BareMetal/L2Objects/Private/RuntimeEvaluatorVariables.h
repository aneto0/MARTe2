/**
 * @file RuntimeEvaluatorVariables.h
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

#ifndef RUNTIMEEVALUATORVARIABLES_H_
#define RUNTIMEEVALUATORVARIABLES_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "RuntimeEvaluator.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 *
 */
class RuntimeEvaluatorVariables {

public:
    /**
     * initialises it with minimum memory
     */
                                              RuntimeEvaluatorVariables(RuntimeEvaluator::DataMemoryElement *&variablesMemoryPtr);

    /**
     * frees all memory
     * does not call destructors of objects created in this
     */
                                              ~RuntimeEvaluatorVariables();


    /**
     * allocates the object T using the memory of this pool
     * NOTE: T must be relocatable
     * NOTE the destructor must be called explicitly
     */
    template <class T>
    inline ErrorManagement::ErrorType           Allocate(RuntimeEvaluator::DataMemoryAddress &location);

    /**
     *
     */
    ErrorManagement::ErrorType                  AllocateBySize(uint32 byteSize, RuntimeEvaluator::DataMemoryAddress &location);

    /**
     * sets usedSize to 0
     * allows reusing this object
     */
    void                                        Clean();

    /**
     * deallocated memory
     */
    void                                        DeAllocate();

    /**
     * Reads from Data Memory
     */
    template<class T>
    inline  T &                                 Variable(RuntimeEvaluator::DataMemoryAddress variableIndex);

    /**
     * Get Address of data
     */
    template<class T>
    inline  T *                                 Address(RuntimeEvaluator::DataMemoryAddress variableIndex);

    /**
     *
     */
    inline RuntimeEvaluator::DataMemoryAddress  GetUsedSize();

    /**
     * next address that will be used or the amount of DataMemoryElement used
     */
    inline RuntimeEvaluator::DataMemoryAddress  NextAddress();
    /**
     * size expressed in RuntimeEvaluatorInfo::DataMemoryElement
     */
    ErrorManagement::ErrorType                  RequestMemory(RuntimeEvaluator::DataMemoryAddress size);

private:
    // how many RuntimeEvaluatorInfo::DataMemoryElement
    RuntimeEvaluator::DataMemoryAddress         allocatedSize;

    // how many RuntimeEvaluatorInfo::DataMemoryElement
    RuntimeEvaluator::DataMemoryAddress         usedSize;

    //
    RuntimeEvaluator::DataMemoryElement *&      memory;

};


RuntimeEvaluator::DataMemoryAddress RuntimeEvaluatorVariables::GetUsedSize(){
    return usedSize;
}



template <class T>
ErrorManagement::ErrorType RuntimeEvaluatorVariables::Allocate(RuntimeEvaluator::DataMemoryAddress &location){
    ErrorManagement::ErrorType ret;
//    DataMemoryAddress ret = (DataMemoryAddress)-1;
    RuntimeEvaluator::DataMemoryAddress  size = RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(T));

    ret = RequestMemory(size);

    if (ret){

        T *pT = reinterpret_cast<T *>(new (reinterpret_cast<void *>(&memory[usedSize])) T);

        ret.outOfMemory = (pT == NULL_PTR(T*));
    }

    if (ret){
        location = usedSize;
        usedSize += size;
    }

    return ret;
}


template<typename T>
T &RuntimeEvaluatorVariables::Variable(RuntimeEvaluator::DataMemoryAddress variableIndex){
    // note that variableIndex is an address to the memory with a granularity of sizeof(MemoryElement)
    return (T&)memory[variableIndex];
}

template<typename T>
T *RuntimeEvaluatorVariables::Address(RuntimeEvaluator::DataMemoryAddress variableIndex){
    // note that variableIndex is an address to the memory with a granularity of sizeof(MemoryElement)
    return reinterpret_cast<T*>(&memory[variableIndex]);
}


RuntimeEvaluator::DataMemoryAddress  RuntimeEvaluatorVariables::NextAddress(){
    return usedSize;
}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_PRIVATE_RUNTIMEEVALUATORVARIABLES_H_ */
