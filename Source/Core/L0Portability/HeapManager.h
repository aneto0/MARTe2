/**
 * @file HeapManager.h
 * @brief Header file for class HeapManager
 * @date Aug 6, 2015
 * @author fsartori
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

 * @details This header file contains the declaration of the class HeapManager
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HEAPMANAGER_H_
#define HEAPMANAGER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Heap.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief TODO
 */
static const int MaximumNumberOfHeaps = 16;


/**
 * @brief TODO
 */
namespace HeapManager{

    /**
     * @brief Finds the Heap that manages the specified memory location
     * returns NULL if not found
     */
    Heap *FindHeap(void * address);

    /**
     * @brief free the pointer data and its associated memory.
     * @param data the data to be freed.
     * returns false if no heap is found that can handle this data
     */
    bool Free(void *&data);

    /**
     * @brief
     * @param
     * returns
     */
    bool AddHeap(Heap &heap);

    /**
     * @brief
     * @param
     * returns
     */
    bool RemoveHeap(Heap &heap);


} ;

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L0PORTABILITY_HEAPMANAGER_H_ */
	
