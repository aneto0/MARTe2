/**
 * @file ClassRegistryItemI.h
 * @brief Header file for class AnyType
 * @date 21 Dec 2018
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

#ifndef CLASSREGISTRYITEMI_H_
#define CLASSREGISTRYITEMI_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * declared and implemented in level2
 * Here only to allow defining pointers to it
 * To access its content
 * */
class ClassRegistryItem;


/**
 * Interface requirement for ClassRegistryItem
 */
class ClassRegistryItemI{

public:

	/**
	 *
	 */
	virtual ~ClassRegistryItemI(){}

    /**
     * @brief Sets the type descriptor for the class described by this ClassRegistryItem.
     * @return the TypeDescriptor.
     */
    virtual const TypeDescriptor &  GetTypeDescriptor() const = 0;

    /**
     * @brief  Get the name of the class (by default the same as returned by typeid.
     */
    virtual CCString                GetClassName() const 	= 0;

    /**
     * @brief  The name of the class as returned by typeid.
     */
    virtual CCString                GetTypeidName() const = 0;

    /**
     * @brief  The version of the class.
     */
    virtual CCString                GetClassVersion() const = 0;

    /**
     * @brief  The size of the class.
     */
    virtual uint32                  GetSizeOfClass() const	= 0;

    /**
     * @brief  allows recovering the base pointer to ClassRegistryItem from this interface
     * without using dynamic_cast which might not works as this class is not known here
     */
    virtual ClassRegistryItem* 		GetBasePtr() = 0;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L1PORTABILITY_CLASSREGISTRYITEMI_H_ */
