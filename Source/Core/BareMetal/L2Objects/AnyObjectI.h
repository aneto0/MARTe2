/**
 * @file AnyObjectI.h
 * @brief Header file for class AnyType
 * @date 24 Sep 2017
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

#ifndef ANYOBJECTI_H_
#define ANYOBJECTI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Object.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief Class which allows creating Objects managing any sort of data structures and allowing access to this data via an AnyType.
 * @details The main difference with an AnyType is that the management of the memory storage is handled by the object.
 * AnyObjectI provides you with the functionalities of AnyType with the addition of the memory management.
 * AnyObjectI does not provide nor prescribe any specific method of memory management, it is left to the final implementation.
 * The user shall expect that a AnyObjectI implementation will destroy the memory when destroyed.
 */
class AnyObjectI: public Object{
public:

	/**
	 * @brief The only interface provided by an AnyObject is the ability to provide its data via an AnyType.
	 * @return a valid AnyType that describes the content of this object and allows read only access to its content
	 */
	virtual operator AnyType()= 0;

	inline AnyType GetAnyType(){
		return this->operator AnyType();
	}

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_ANYOBJECTI_H_ */
