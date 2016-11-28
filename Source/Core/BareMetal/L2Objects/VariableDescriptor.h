/**
 * @file VariableDescriptor.h
 * @brief Header file for class VariableDescriptor
 * @date Nov 23, 2016
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

 * @details This header file contains the declaration of the class VariableDescriptor
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L2OBJECTS_VARIABLEDESCRIPTOR_H_
#define L2OBJECTS_VARIABLEDESCRIPTOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeDescriptor.h"
#include "BasicType.h"
#include "ClassRegistryDatabase.h"
#include "Matrix.h"
#include "StringHelper.h"


namespace MARTe {
class Object;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class ModifierStringType{
public:

    inline ModifierStringType(const ModifierStringType &x){
        modifierString = StringHelper::StringDup(CCString(x.modifierString));
    }

    ModifierStringType(){
        modifierString = NULL_PTR(char *);
    }
    ~ModifierStringType(){
        if (modifierString != NULL_PTR(char *)){
            free((void *)modifierString);
        }
    }

    void Putc(char c,uint32 &len){
        if (modifierString[len-1]=='C') {
            modifierString[len-1] = 'K';
            return;
        }

        unsigned int newLen = len+2;
        modifierString = (char *)realloc((void *)modifierString,newLen);
        modifierString[len] = c;
        modifierString[len+1] = 0;
        len++;
    }

    void Putn(uint32 i,uint32 &len){
        unsigned int test = 1;
        while (test <= i) test *=10;
        test/=10;
        do {
            char c = (i /test) +'0';
            Putc(c,len);
            i = i % test;
            test/=10;
        }  while (test > 1 );
    }

    operator CString(){
        return modifierString;
    }
private:
    CString modifierString;
};

/**
 * @brief full description of the type of a variable including modifiers
 */
class VariableDescriptor{

public:


    /**
     * @biref copy constructor
     */
    inline VariableDescriptor(const VariableDescriptor &x) ;


    /**
     * @brief main constructor by learning
     */
    template <class T>
    inline  VariableDescriptor( T & x){
        uint32 size = 0;
        Match(x,size);
    }

    /**
     * @brief buids a variable descriptor for a variable with no modifiers (a part from those held by typeDescriptor)
     */
    VariableDescriptor(const TypeDescriptor &td){
        typeDescriptor = td;
    }

private:

    /**
     *  C means constant K means constant pointer * means variable pointer.
     *  The order is variable first type latest
     *  ex: char const *  = *C ...  char * const = K  ... char const ** = **C    char const * const = KC
     */
    ModifierStringType modifierString;

    /**
     * The type of the (final after redirections) variable
    */
    TypeDescriptor     typeDescriptor;

    // GENERAL MATCHES

    /**
     * @brief Matches a Matrix.
     * @tparam T the type of the elements in the matrix
     * @param[in] mat the matrix from whose this Match will be constructed.
     * @post Adds a M to the modifiers
     */
    template<typename T>
    void Match(Matrix<T> * &mat,uint32 &size);

    /**
     * @brief Matches a Vector
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T>
    void Match(Vector<T> * &vec,uint32 &size);

    /**
     * @brief Matches a T[n]
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a [n] to the modifiers
     */
    template <class T,int n>
    inline void Match(T (*x) [n],uint32 &size);

    /**
     * @brief Matches a T*
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a * to the modifiers
     */
    template <class T>
    inline void Match(T ** &x,uint32 &size);

    /**
     * @brief Matches a T* const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T * const * &x,uint32 &size);

    /**
     * @brief Matches a T const *
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a * to the modifiers
     */
    template <class T>
    inline void Match(T const * * &x,uint32 &size);

    /**
     * @brief Matches a T const * const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T const * const * &x,uint32 &size);

    /**
     * @brief Matches a T const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T const * &x,uint32 &size);

    /**
     * @brief Matches a T
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void Match(T * &x,uint32 &size);

 // SPECIFIC MATCHES


    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == Character8Bit &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(char8 *&i,uint32 &size);


    /**
     * @brief Constructor from signed 8 bit integer.
     * @param[in] i is the signed 8 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger8Bit &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(int8 *&i,uint32 &size);

    /**
     * @brief Constructor from unsigned 8 bit integer.
     * @param[in] i is the unsigned 8 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger8Bit &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(uint8 *&i,uint32 &size);

    /**
     * @brief Constructor from constant signed 8 bit integer.
     * @param[in] i is the constant signed 8 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger8Bit &&
     *   GetTypeDescriptor().isConstant == true &&
     *   GetNumberOfElements(0:2) == 0
     */

    inline void Match(int16 *&i,uint32 &size);

    /**
     * @brief Constructor from unsigned 16 bit integer.
     * @param[in] i is the unsigned 16 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger16Bit &&
     *   GetTypeDescriptor().isConstant == false &&
     *   GetNumberOfElements(0:2) == 0
     */

    inline void Match(uint16 *&i,uint32 &size);

    /**
     * @brief Constructor from signed 32 bit integer.
     * @param[in] i is the signed 32 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger32Bit &&
     *   GetTypeDescriptor().isConstant == false &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(int32 *&i,uint32 &size);

    /**
     * @brief Constructor from unsigned 32 bit integer.
     * @param[in] i is the unsigned 32 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger32Bit &&
     *   GetTypeDescriptor().isConstant == false &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(uint32 *&i,uint32 &size);

    /**
     * @brief Constructor from signed 64 bit integer.
     * @param[in] i is the signed 64 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger64Bit &&
     *   GetTypeDescriptor().isConstant == false &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(int64 *&i,uint32 &size);

    /**
     * @brief Constructor from unsigned 64 bit integer.
     * @param[in] i is the unsigned 64 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger64Bit &&
     *   GetTypeDescriptor().isConstant == false &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(uint64 *&i,uint32 &size);

    /**
     * @brief Constructor from 32 bit float32 number.
     * @param[in] i is the 32 bit float32 number input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(float32 *&i,uint32 &size);

    /**
     * @brief Constructor from 64 bit float32 number.
     * @param[in] i the 64 bit float32 number input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(float64 *&i,uint32 &size);

    /**
     * @brief Constructor from void pointer.
     * @param[in] p is the void pointer input.
     * @post
     *   GetDataPointer() == p &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(void * *&p,uint32 &size);

    /**
     * @brief Constructor from Object (or inherited class).
     * @param[in] obj the source Object.
     * @post
     *   GetDataPointer() == &obj &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    void Match(Object *&obj,uint32 &size);

    /**
     * @brief Constructor by BitBoolean.
     * @tparam baseType the standard type which is used as a base for bitBool's type
     * @tparam bitOffset the actual bit offset of the bitBool's type
     * @param[in] bitBool is the BitBoolean object input.
     */
    template<typename baseType, uint8 bitOffset>
    void Match(BitBoolean<baseType, bitOffset> * &bitBool,uint32 &size);

    /**
     * @brief Constructor by BitRange.
     * @tparam baseType the standard type which is used as a base for bitRange's type
     * @tparam bitSize the actual bit size of the bitRange's type
     * @tparam bitOffset the actual bit offset of the bitRange's type
     * @param[in] bitRange is the BitRange object input.
     */
    template<typename baseType, uint8 bitSize, uint8 bitOffset>
    void Match(BitRange<baseType, bitSize, bitOffset> * &bitRange,uint32 &size);

    /**
     * @brief Constructor by FractionalInteger.
     * @tparam baseType the standard type which is used as a base for fractionalInt's type
     * @tparam bitSize the actual bit size of the fractionalInt's type
     * @param[in] fractionalInt is the FractionalInteger object input.
     */
    template<typename baseType, uint8 bitSize>
    void Match(FractionalInteger<baseType, bitSize> * &fractionalInt,uint32 &size);



    /**
     * @brief Returns the pointed data TypeDescriptor.
     * @return the data TypeDescriptor.
     */
    TypeDescriptor GetTypeDescriptor() const;


    /**
     * @brief Gets the number of dimensions associated to this Match.
     * @details GetNumberOfDimensions() == 0 => scalar, GetNumberOfDimensions() == 1 => vector
     * GetNumberOfDimensions() == 2 => matrix
     * @return the number of dimensions associated to this Match.
     */
    inline uint8 GetNumberOfDimensions() const;

    /**
     * @brief Gets the number of elements in a given \a dimension.
     * @param[in] dimension the dimension to be queried.
     * @return the number of elements in a given \a dimension.
     * @pre
     *   dimension < 3
     */
    inline uint32 GetNumberOfElements(const uint32 dimension) const;

    /**
     * @brief Checks if GetDataPointer() is pointing at a statically allocated memory block.
     * @return true if GetDataPointer() is pointing at a statically allocated memory block.
     */
    inline bool IsStaticDeclared() const;

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


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


VariableDescriptor::VariableDescriptor(const VariableDescriptor &x) {
    this->typeDescriptor = x.typeDescriptor;
    this->modifierString = x.modifierString;
}


//bool VariableDescriptor::IsVoid() const {
//    return (typeDescriptor == VoidType);
//}

/*---------------------------------------------------------------------------*/

template<typename T>
void VariableDescriptor::Match(Vector<T> * &vec,uint32 &size) {
    modifierString.Putc('V',size);
    T *pp = NULL;
    Match(pp,size);
}

template<typename T>
void VariableDescriptor::Match(Matrix<T> * &mat,uint32 &size) {
    modifierString.Putc('M',size);
    T *pp = NULL;
    Match(pp,size);
}

template <class T,int n>
inline void VariableDescriptor::Match(T (*x) [n],uint32 &size){
    T *pp = NULL;
    modifierString.Putc('[',size);
    modifierString.Putn(n,size);
    modifierString.Putc(']',size);
    Match(pp,size);
}

template <class T>
inline void VariableDescriptor::Match(T ** &x,uint32 &size){
    T *pp = NULL;
    modifierString.Putc('*',size);
    Match (pp,size);
}

template <class T>
inline void VariableDescriptor::Match(T * const * &x,uint32 &size){
    T ** pp=NULL;
    modifierString.Putc('C',size);
    Match (pp,size);
}

template <class T>
inline void VariableDescriptor::Match(T const * * &x,uint32 &size){
    T const * pp=NULL;
    modifierString.Putc('*',size);
    Match (pp,size);
}

template <class T>
inline void VariableDescriptor::Match(T const * const * &x,uint32 &size){
    T const * * pp=NULL;
    modifierString.Putc('C',size);
    Match (pp,size);
}

template <class T>
void VariableDescriptor::Match(T const * &x,uint32 &size){
    T * pp=NULL;
    modifierString.Putc('C',size);
    Match (pp,size);
}

template <class T>
void VariableDescriptor::Match(T * &x,uint32 &size){
    ClassRegistryItem *cri =  ClassRegistryItem::Instance<T>();
    if (cri != NULL) typeDescriptor = cri->GetTypeDescriptor();
    else typeDescriptor = VoidType;
}

void VariableDescriptor::Match(char8 * &i,uint32 &size) {
    typeDescriptor = Character8Bit;
}

void VariableDescriptor::Match(int8 * &i,uint32 &size) {
    typeDescriptor = SignedInteger8Bit;
}

void VariableDescriptor::Match(uint8 * &i,uint32 &size) {
    typeDescriptor = UnsignedInteger8Bit;
}


void VariableDescriptor::Match(int16 * &i,uint32 &size) {
    typeDescriptor = SignedInteger16Bit;
}

void VariableDescriptor::Match(uint16 * &i,uint32 &size) {
    typeDescriptor = UnsignedInteger16Bit;
}

void VariableDescriptor::Match(int32 * &i,uint32 &size) {
    typeDescriptor = SignedInteger32Bit;
}

void VariableDescriptor::Match(uint32 * &i,uint32 &size) {
    typeDescriptor = UnsignedInteger32Bit;
}

void VariableDescriptor::Match(int64 * &i,uint32 &size) {
    typeDescriptor = SignedInteger64Bit;
}

void VariableDescriptor::Match(uint64 * &i,uint32 &size) {
    typeDescriptor = UnsignedInteger64Bit;
}

void VariableDescriptor::Match(float32 * &i,uint32 &size) {
    typeDescriptor = Float32Bit;
}

void VariableDescriptor::Match(float64 * &i,uint32 &size) {
    typeDescriptor = Float64Bit;
}

void VariableDescriptor::Match(void * * &p,uint32 &size) {
    typeDescriptor.isStructuredData = false;
    typeDescriptor.isConstant = false;
    typeDescriptor.type = Pointer;
    typeDescriptor.numberOfBits = sizeof(void*) * 8u;
}


template<typename baseType, uint8 bitOffset>
void VariableDescriptor::Match(BitBoolean<baseType, bitOffset> * &bitBool,uint32 &size) {
    typeDescriptor.isStructuredData = false;
    typeDescriptor.isConstant = false;
    typeDescriptor.type = UnsignedInteger;
    typeDescriptor.numberOfBits = 1u;
//    bitAddress = bitBool.BitOffset();
}

template<typename baseType, uint8 bitSize, uint8 bitOffset>
void VariableDescriptor::Match(BitRange<baseType, bitSize, bitOffset> * &bitRange,uint32 &size) {
    BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    typeDescriptor.isStructuredData = false;
    typeDescriptor.isConstant = false;
    typeDescriptor.type = type;
    typeDescriptor.numberOfBits = bitRange->GetNumberOfBits();
//    bitAddress = bitRange.BitOffset();
}

template<typename baseType, uint8 bitSize>
void VariableDescriptor::Match(FractionalInteger<baseType, bitSize> * &fractionalInt,uint32 &size) {
    BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    typeDescriptor.isStructuredData = false;
    typeDescriptor.isConstant = false;
    typeDescriptor.type = type;
    typeDescriptor.numberOfBits = fractionalInt->GetNumberOfBits();
}


TypeDescriptor VariableDescriptor::GetTypeDescriptor() const {
    return typeDescriptor;
}






/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}
#endif /* L2OBJECTS_VARIABLEDESCRIPTOR_H_ */
	
