/**
 * @file Heap.h
 * @brief Header file for class Heap
 * @date 4 Aug 2015
 * @author andre
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

 * @details This header file contains the declaration of the class Heap
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L0PORTABILITY_HEAP_H_
#define SOURCE_CORE_L0PORTABILITY_HEAP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Heap interface and standard implementation.
 * @details Framework objects can be allocated in different types of heap memory.
 * This class provides the interface method definition and a basic implementation
 * which calls the Memory::Malloc and Memory::Free functions.
 */
class Heap {
public:

    /**
     * @brief No operation.
     */
    virtual ~Heap();

    /**
     * @brief allocates size bytes of data in the heap.
     * @return a pointer to the allocated memory or NULL if the allocation fails.
     */
    virtual void *Malloc(const uint32 &size);

    /**
     * @brief free the pointer data and its associated memory.
     * @param data the data to be freed.
     */
    virtual void Free(void *&data);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L0PORTABILITY_HEAP_H_ */

