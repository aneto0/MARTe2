/**
 * @file AnyObjectT.h
 * @brief Header file for class AnyObjectT
 * @date 30/10/2017
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

 * @details This header file contains the declaration of the class AnyObjectT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */


#ifndef ANYOBJECT_T_H_
#define ANYOBJECT_T_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Object.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief An Object derivative either referring to or containing a generic variable described by an AnyType .
 * @details This class allows to associate a name and a reference to an AnyType.
 * It may holds and manage a memory space with a copy of the data of a generic variable or is a simply a reference to it
 */
template <unsigned int objectSize>
class DLL_API AnyObjectT: public Object {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP.
     * @post
     *   GetType().GetDataPointer() == NULL
     */
	AnyObjectT();

    /**
     * @brief Default destructor.
     * @details Calls CleanUp()
     */
    virtual ~AnyObjectT();

    /**
     * @brief Allows to setup an Anytype of any type....
     * @param[in] dataDescriptorIn contains the type informations in a TypeDescriptor class.
     * @param[in] modifiers contains the modifiers string in the appropriate language (see VariableDescriptor).
     * @param[in] dataPointerIn is the pointer to the constant data.
     */
    void Setup(TypeDescriptor dataDescriptorIn,CCString modifiers,const void* const dataPointerIn,uint32 size);

#if 0

    /**
     * @brief frees any memory and the pointer.
     */
    void CleanUp();

    /**
     * @brief Cleanup() then allocates memory and copies the provided data
     * @details The input AnyType \a typeIn will be copied and its contents
     * (provided by typeIn.GetDataPointer()) copied into a memory space managed by this
     * AnyObjectT. This memory will be freed by the destructor of the AnyType.
     * @param[in] typeIn the AnyType whose data is to be copied.
     * @return true if all the memory allocation and copy operations are successful.
     * @pre
     *   GetType().GetDataPointer() == NULL
     * @post
     *   GetType() == typeIn
     *   canDestroy = true
     */
    ErrorManagement::ErrorType Copy(const AnyType &typeIn);

    /**
     * @brief Cleanup() then Holds a reference to the provided data together with its type information
     * @details The input AnyType \a typeIn will be copied to the local Anytype
     * @param[in] typeIn the AnyType to be copied.
     * @return true if all the memory allocation and copy operations are successful.
     * @pre
     *   GetType().GetDataPointer() == NULL
     * @post
     *   GetType() == typeIn
     *   canDestroy = false
     */
    ErrorManagement::ErrorType Refer(const AnyType &typeIn);
#endif
    /*
	 * @brief The main interface provided by an AnyObjectT is the ability to provide its data via an AnyType.
	 * @return a valid AnyType that describes the content of this object and allows read only access to its content
	 */
	virtual operator AnyType();

private:

    /**
     * @brief The Type and address of the data
     */
    VariableDescriptor vd;

    ///
    char8 data[objectSize];
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <unsigned int objectSize>
AnyObjectT<objectSize>::AnyObjectT():vd(InvalidType(0),""){
};

template <unsigned int objectSize>
AnyObjectT<objectSize>::~AnyObjectT(){

};

template <unsigned int objectSize>
 AnyObjectT<objectSize>::operator AnyType(){
	return AnyType(vd,&data);
}

template <unsigned int objectSize>
void AnyObjectT<objectSize>::Setup(TypeDescriptor dataDescriptorIn,CCString modifiers,const void* const dataPointerIn,uint32 size){
	vd = VariableDescriptor(dataDescriptorIn,modifiers);
	if (size > objectSize){
		size = objectSize;
	}
	MemoryOperationsHelper::Copy(data,dataPointerIn,size);
}


TEMPLATE_CLASS_REGISTER(AnyObjectT<objectSize>,"1.0",unsigned int objectSize)


}

#endif /* ANYOBJECT_H_ */
