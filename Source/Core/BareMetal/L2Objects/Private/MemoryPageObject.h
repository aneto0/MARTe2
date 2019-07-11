/**
 * @file MemoryPageObject.h
 * @brief Header file for class AnyType
 * @date 3 Feb 2018
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

#ifndef MEMORYPAGEOBJECT_H_
#define MEMORYPAGEOBJECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MemoryPage.h"
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
 * @brief a class to contain synthetic objects
 */
class MemoryPageObject: public Object{

public:
    CLASS_REGISTER_DECLARATION()


	/**
	 * constructor
	 */
	MemoryPageObject();

	/**
	 * destructor
	 */
	virtual ~MemoryPageObject();

    /**
     * @brief Allows to setup an Anytype of any type....
     * @param[in] dataDescriptorIn contains the type informations in a TypeDescriptor class.
     * @param[in] modifiers contains the modifiers string in the appropriate language (see VariableDescriptor).
     * @param[in] dataPointerIn is the pointer to the constant data.
     * @param[in,out] pagesToTransfer is the MemoryPage from where to take the memory of this object. On return is set to NULL
     */
    void Setup(TypeDescriptor dataDescriptorIn,CCString modifiers,const void* const dataPointerIn,MemoryPage &pagesToTransfer);

	/**
	 * @brief The only interface provided by an AnyObject is the ability to provide its data via an AnyType.
	 * @return a valid AnyType that describes the content of this object and allows read only access to its content
	 */
    virtual void ToAnyType(AnyType &at);

private:
	/**
	 * contains the memory of the object
	 */
	MemoryPage 	mp;

	/**
	 * the descriptor of the type
	 * and the reference to it
	 */
	AnyType 	anyType;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_MEMORYPAGEOBJECT_H_ */
