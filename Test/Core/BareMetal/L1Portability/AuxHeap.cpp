/**
 * @file AuxHeap.cpp
 * @brief Source file for class AuxHeap
 * @date 13/08/2015
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

 * @details This source file contains the definition of all the methods for
 * the class AuxHeap (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AuxHeap.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief constructor
 */
AuxHeap::AuxHeap() {

}
/**
 * @brief destructor
 */
AuxHeap::~AuxHeap() {
}

/**
 * @brief allocates size bytes of data in the heap. Maximum allocated size is 4Gbytes
 * @return a pointer to the allocated memory or NULL if the allocation fails.
 */
/*lint -e{586} use of malloc function (deprecated) */
void *AuxHeap::Malloc(const uint32 size) {
	return HeapManager::Malloc(size);
}

/**
 * @brief free the pointer data and its associated memory.
 * @param data the data to be freed.
 */
/*lint -e{586} use of free function (deprecated) */
void AuxHeap::Free(void *&data) {
	return HeapManager::Free(data);
}

/*lint -e{586} use of realloc function (deprecated) */
void *AuxHeap::Realloc(void *&data,const uint32 newSize) {

	return HeapManager::Realloc(data,newSize);
}

}

