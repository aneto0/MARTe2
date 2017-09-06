/**
 * @file AnyTypeStructuredData.h
 * @brief Header file for class AnyType
 * @date 5 Sep 2017
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

#ifndef ANYTYPESTRUCTUREDDATA_H_
#define ANYTYPESTRUCTUREDDATA_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StructuredDataI.h"
#include "AnyType.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * It allows accessing AnyType via a StructuredDataI interface
 * If the data type is structured a vector or is a pointer, it allows browsing into it.
 * There a number of conventions:
 *  address '*'allows deferencing a pointer
 *  address [N] allows choosing the Nth element of a Vector or a ZeroTerm array (if within range)
 *  .nnn allows choosing a member of a structure named .nnn (. needed only when combining addresses)
 */
class AnyTypeStructuredData: public StructuredDataI{

public:
	/**
	 * First method of instantiation.
	 * Any variable can be used.
	 */
	template <class T >
	AnyTypeStructuredData(const T &data){
		// create AnyObject and add it to the path
	}

	/**
	 * First method of instantiation.
	 * The content of an AnyType is used.
	 */
	AnyTypeStructuredData(const AnyType &data){
		// create AnyObject and add it to the path
	}

	/**
	 * virtual destructor
	 */
	virtual ~AnyTypeStructuredData(){

	}
private:
	/**
	 * a stack of AnyObjects containing the navigation path
	 */
	ReferenceContainer path;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}; // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_ANYTYPESTRUCTUREDDATA_H_ */
