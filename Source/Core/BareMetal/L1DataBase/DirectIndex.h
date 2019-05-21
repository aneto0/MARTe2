/**
 * @file DirectIndex.h
 * @brief Header file for class AnyType
 * @date 30 Apr 2018
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

#ifndef DIRECTINDEX_H_
#define DIRECTINDEX_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * use memory addresses directly as database keys
 * implements the Index template interface
 */
class DirectIndex{

public:

	// main function of this class
	// allows retrieving the address of the data from the index
	inline void *operator[](uintp index);

	// allocates an index to a data pointer
	inline ErrorManagement::ErrorType Store(void *data,uintp &index);

	/**
	 * the index must be valid! Invalid indexes will result in access to invalid memory
	 * removes the element at the index position
	 */
	inline ErrorManagement::ErrorType FreeIndex(uintp index);

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void *DirectIndex::operator[](uintp index){
	return static_cast<void *> (index);
}

ErrorManagement::ErrorType  DirectIndex::Store(void *data,uintp &index){
	ErrorManagement::ErrorType ret;
	index = static_cast<uintp> (data);
	return ret;
}

ErrorManagement::ErrorType DirectIndex::FreeIndex(uintp index){
	ErrorManagement::ErrorType ret;
	return ret;
}

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L1DATABASE_DIRECTINDEX_H_ */
