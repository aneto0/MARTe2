/**
 * @file AnyType.h
 * @brief Header file for class AnyType
 * @date 26/08/2015
 * @author Giuseppe Ferrò
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

#ifndef ANYTYPE_H_
#define ANYTYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <typeinfo>
#include <stdio.h>

#define dprintf(a,...)
//#define dprintf printf


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeDescriptor.h"
#include "VariableDescriptor.h"
#include "ClassRegistryDatabase.h"
#include "Matrix.h"
#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class Object;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*lint -sem(MARTe::AnyType::Init,initializer)*/
namespace MARTe {

/**
 * @brief Class which provides a smart mechanism for the generic handling
 * of variables. It is a generic reference to a variable typically used to build functions able to handle any type of variables
 * @details Each instance of this class is made by:
 * - a void* pointer to the variable;
 * - a VariableDescriptor which contains the TypeDescriptor and a vector of modifiers
 *           - a TypeDescriptor which defines the final type of the data.
 * AnyType works with variables of any type.
 */
#if 0
/*lint -save -e925 -e926 -e929 -e9005 -e1773 .
 * (925) pointer cast required by this implementation of AnyType */
/* (9005,1773) Cast away of const required by this implementation of AnyType
 * and justified because in the TypeDescriptor attribute the flag "isConstant"
 * will be set to true.
 */
#endif

class DLL_API AnyType {

public:

    /**
     * @brief Copy constructor. Needed because non cost Anytype is not univocally matched here
     * @param[in] x is the AnyType to be copied.
     * @post
     *   GetDataPointer() == x.GetDataPointer() &&
     *   GetDataDescriptor() == x.GetDataDescriptor()
     */
    inline AnyType(AnyType &x);

    /**
     * @brief Copy constructor.
     * @param[in] x is the AnyType to be copied.
     * @post
     *   GetDataPointer() == x.GetDataPointer() &&
     *   GetDataDescriptor() == x.GetDataDescriptor()
     */
    inline AnyType(const AnyType &x);

    /**
     * @brief default constructor.
     * @post
     *   GetDataPointer() == NULL &&
     *   GetDataDescriptor() == InvalidType
     */
    inline AnyType();

    /**
     * @brief Generic constructor for a constant type non-bitset.
     * @param[in] dataDescriptorIn contains the type informations in a TypeDescriptor class.
     * @param[in] dataPointerIn is the pointer to the constant data.
     * @post
     *   GetDataPointer() == dataPointerIn &&
     *   GetDataDescriptor() == dataDescriptorIn
     */
    inline AnyType(const TypeDescriptor &dataDescriptorIn,const void* const dataPointerIn);

    /**
     * @brief Constructor from non-const non-ptr. Uses VariableDescriptor constructor.
     * @param[in] x is any variable non-const non-ptr
     * @post
     *   GetDataPointer() == &x && variableDescriptor(&T)
     */
    template <class T>
    inline AnyType(T &x);

    /**
     * @brief Constructor from const non-ptr. Uses VariableDescriptor constructor.
     * @param[in] x is any variable const non-ptr
     * @post
     *   GetDataPointer() == &x  variableDescriptor(&T)
     */
    template <class T>
    inline AnyType(T const &x);

    /**
     * @brief Allows to setup an Anytype of any type....
     * @param[in] dataDescriptorIn contains the type informations in a TypeDescriptor class.
     * @param[in] modifiers contains the modifiers string in the appropriate language (see VariableDescriptor).
     * @param[in] dataPointerIn is the pointer to the constant data.
     * @post
     *   GetDataPointer() == dataPointerIn &&
     *   GetDataDescriptor() == dataDescriptorIn
     */
    inline void Setup(TypeDescriptor dataDescriptorIn,CCString modifiers,const void* const dataPointerIn);

    /**
     * @brief moves the pointer to referencing the a variable that is referenced to by the current variable (if pointer)
     * @param[in] index allows addressing arrays
     * @return ErrorType::ErrorsCleared()= true if operation successful
     * @post
     *   if all ok pointer2Variable and variableDescriptor are updated
     *   otherwise the object potentially becomes invalid and cannot be used anymore
     */
    ErrorManagement::ErrorType  	Dereference (uint32 index=0);

    /**
     * @brief moves the pointer to referencing a field of the structure that is currently referred to
     * @param[in] field is the name of the desired field
     * @return ErrorType::ErrorsCleared()= true if operation successful
     * @post
     *   if all ok pointer2Variable and variableDescriptor are updated
     *   otherwise the object potentially becomes invalid and cannot be used anymore
     */
    ErrorManagement::ErrorType  	Dereference (CCString field);

    /**
     * @brief evaluates a C like dereferencing expression and moves the pointer
     * @param[in] CExpresssion is the expression: it is a sequence of dereference commands.
     * commands can be: "*" for pointer dereferencing;
     * "[NN]" for array dereferencing
     * ".field" for structure to field dereferencing
     * "->field" for structure pointer to field dereferencing
     * @return ErrorType::ErrorsCleared()= true if operation successful
     * @post
     *   if all ok pointer2Variable and variableDescriptor are updated
     *   otherwise the object potentially becomes invalid and cannot be used anymore
     */
    ErrorManagement::ErrorType  	MultipleDereference (CCString CExpression);

    /**
     * @briefs copies the content to destination.
     * Number of dimensions must be the same or compatible
     */
    ErrorManagement::ErrorType  	CopyTo(AnyType destination);

    /**
     * @briefs compare content with destination.
     * Number of dimensions must be the same or compatible
     */
    ErrorManagement::ErrorType  	CompareWith(AnyType destination);

    /**
     * @brief pointer2Variable = NULL
     * @return  (pointer2Variable == NULL)
     */
    inline bool IsVoid() const;

    /** INTERFACES GETTERS*/

    /**
     * @brief Returns the pointer to the data.
     * @return the pointer to the data.
     */
    inline const void* 				GetVariablePointer() const;

    /**
     * @brief Returns the full variableDescriptor.
     * This is the full Variable description with all modifiers
     */
    inline const VariableDescriptor &GetFullVariableDescriptor() const;

private:

    /**
     * contains type descriptor
     * contains variable modifiers
     */
    VariableDescriptor variableDescriptor;

    /**
     * Pointer to the variable.
     */
    const void* pointer2Variable;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

AnyType::AnyType(){
    pointer2Variable = NULL_PTR(void *);
}

bool AnyType::IsVoid() const{
	return (pointer2Variable == NULL_PTR(void *));
}


AnyType::AnyType(const TypeDescriptor &typeDescriptorIn,const void* const dataPointerIn):variableDescriptor(typeDescriptorIn){
    pointer2Variable = const_cast<void *>(dataPointerIn);
}

void AnyType::Setup(TypeDescriptor dataDescriptorIn,CCString modifiers,const void* const dataPointerIn){
	variableDescriptor = VariableDescriptor(dataDescriptorIn,modifiers);
    pointer2Variable = const_cast<void *>(dataPointerIn);
}


AnyType::AnyType( AnyType &x):variableDescriptor(x.variableDescriptor) {
    /*lint -e{1554} the pointer2Variable is to be shared with the copied AnyType.*/
    this->pointer2Variable = x.pointer2Variable;
}

AnyType::AnyType(const AnyType &x):variableDescriptor(x.variableDescriptor) {
    /*lint -e{1554} the pointer2Variable is to be shared with the copied AnyType.*/
    this->pointer2Variable = x.pointer2Variable;
}

template <class T>
AnyType::AnyType(T &x): variableDescriptor (reinterpret_cast<T *>(&x)){
    pointer2Variable = reinterpret_cast<void *>(const_cast<T *>(&x));
}

template <class T>
AnyType::AnyType(T const &x): variableDescriptor (reinterpret_cast<T const *>(&x)){
    pointer2Variable = const_cast<void *>(reinterpret_cast<const void *>(const_cast<T *>(&x)));
}

const void* AnyType::GetVariablePointer() const{
    return pointer2Variable;
}

const VariableDescriptor &AnyType::GetFullVariableDescriptor() const{
    return this->variableDescriptor;
}

/**
 * used as terminator
 */
static const AnyType voidAnyType;

}

#endif /* ANYTYPE_H_ */

