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
 * @brief Class which provides a smart mechanism for the generic representation
 * of types.
 * @details Each instance of this class is made by:
 * - a void* pointer to the data;
 * - an uint8 representing the bit-shift from the void* pointer to the actual
 * data (used in bitfields);
 * - a TypeDescriptor which defines the type, constantness, signedness, etc.
 * of the data.
 *
 * AnyType works with basic types as well as classes, as long as they are
 * registered in the ClassRegistryDatabase.
 * @note A constructor for each basic type has been defined and implemented in
 * order to automatically build the relative AnyType object. Some of these
 * constructors are templates.
 */
/*lint -save -e925 -e926 -e929 -e9005 -e1773 .
 * (925) pointer cast required by this implementation of AnyType */
/* (9005,1773) Cast away of const required by this implementation of AnyType
 * and justified because in the TypeDescriptor attribute the flag "isConstant"
 * will be set to true.
 */
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
     * @brief Constructor from non-const ptr
     * @param[in] x is any variable non-const ptr
     * @post
     *   GetDataPointer() == x && variableDescriptor(&T)
     */
    template <class T>
    inline AnyType(T * &x);


    /**
     * @brief Constructor from const ptr
     * @param[in] x is any variable const ptr
     * @post
     *   GetDataPointer() == x  variableDescriptor(&T)
     */
    template <class T>
    inline AnyType(T const * &x);


    /** INTERFACES GETTERS*/

    /**
     * @brief Returns the pointer to the data.
     * @return the pointer to the data.
     */
    inline void* GetDataPointer() const;

    /**
     * @brief Returns the full variableDescriptor.
     * This is the full Varaiable description with all modifiers
     */
    inline const VariableDescriptor &GetFullVariableDescriptor() const;

    /**
     * @brief Returns the a variableDescriptor that contains sufficent information to use the pointer.
     * This is the synthetic Variable description with no modifiers
     */
     const VariableDescriptor GetSyntheticVariableDescriptor() const;

    /**
     * @brief Returns the synthetic type descriptor of *x. In case of error he type is void;
     * @return voidAnyType if this AnyType is scalar or in case of errors, a scalar AnyType if this
     * @return a new AnyType
     */
    AnyType operator *() const;

    /**
     * @brief Retrieves the element in the specified position.
     * @param[in] position is the position of the required element.
     * @return voidAnyType if this AnyType is scalar or in case of errors, a scalar AnyType if this
     * AnyType is a vector, a vector AnyType if this AnyType is a scalar.
     */
    AnyType operator[](const uint32 position) const;

    /**
     * @brief Returns the data bit address (i.e  the bit shift respect to the data pointer).
     * @return the data bit address.
     */
    inline uint8 GetBitAddress() const;

    /**
     * @brief Gets the number of elements for a given number of \a redirections.
     * @param[in] dimension the dimension to be queried.
     * @return the number of elements in a given \a redirections.
     * @pre
     *   dimension < 3
     */
    inline uint32 GetNumberOfElements(const uint32 redirections) const;

    /**
     * @brief Retrieves the byte size of this type.
     * @return the byte size of this type.
     */
    inline uint32 GetByteSize() const;

    /**
     * @brief Retrieves the bit size of this type.
     * @return the bit size of this type.
     */
    inline uint32 GetBitSize() const;

private:

    /**
     * contains type descriptor
     * contains variable modifiers
     */
    VariableDescriptor variableDescriptor;

    /**
     * Pointer to the data.
     */
    void * dataPointer;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

AnyType::AnyType(){
    dataPointer = NULL_PTR(void *);
}

AnyType::AnyType(const TypeDescriptor &typeDescriptorIn,const void* const dataPointerIn):variableDescriptor(typeDescriptorIn){

    dataPointer = const_cast<void *>(dataPointerIn);
}

AnyType::AnyType( AnyType &x):variableDescriptor(x.variableDescriptor) {
    /*lint -e{1554} the dataPointer is to be shared with the copied AnyType.*/
    this->dataPointer = x.dataPointer;
}

AnyType::AnyType(const AnyType &x):variableDescriptor(x.variableDescriptor) {
    /*lint -e{1554} the dataPointer is to be shared with the copied AnyType.*/
    this->dataPointer = x.dataPointer;
}

template <class T>
AnyType::AnyType(T &x): variableDescriptor (reinterpret_cast<T *>(&x)){
    dataPointer = reinterpret_cast<void *>(const_cast<T *>(&x));
}

template <class T>
AnyType::AnyType(T const &x): variableDescriptor (reinterpret_cast<T const *>(&x)){
    dataPointer = reinterpret_cast<void *>(const_cast<T *>(&x));
}

template <class T>
AnyType::AnyType(T * &x): variableDescriptor (reinterpret_cast<T **>(&x)){
    dataPointer = reinterpret_cast<void *>(const_cast<T *>(x));
}

template <class T>
AnyType::AnyType(T const * &x): variableDescriptor (reinterpret_cast< T const **>(&x)){
    dataPointer = reinterpret_cast<void *>(const_cast<T *>(x));
}

void* AnyType::GetDataPointer() const{
    return dataPointer;
}

const VariableDescriptor &AnyType::GetFullVariableDescriptor() const{
    return this->variableDescriptor;
}

}

#endif /* ANYTYPE_H_ */

