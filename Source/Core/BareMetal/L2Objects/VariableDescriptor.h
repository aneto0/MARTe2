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

#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeDescriptor.h"
#include "BasicType.h"
#include "ClassRegistryDatabase.h"
#include "Matrix.h"
#include "StringHelper.h"
#include "ZeroTerminatedArray.h"
#include "DynamicZeroTerminatedArray.h"
#include "StaticZeroTerminatedArray.h"


namespace MARTe {
class Object;
class DynamicCString;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief full description of the type of a variable including modifiers
 */
class VariableDescriptor{

public:

    /**
     * default constructor
     * */
    VariableDescriptor();

    /**
     * destructor
     * */
    ~VariableDescriptor();

    /**
     * @brief main constructor by learning
     */
    template <class T>
    inline  VariableDescriptor( T  x);

    /**
     * @brief copy
     * Note that const VariableDescription on its own will not work as it will not match univocally a non cost  VariableDescriptor
     */
    VariableDescriptor(VariableDescriptor &x ) ;

    /**
     * @brief copy
     * Note that const VariableDescription will not work
     */
    VariableDescriptor(const VariableDescriptor &x );

    /**
     * @brief sets the typeDescriptor
     */
    VariableDescriptor(const TypeDescriptor &td);

    /**
     * @brief gets the modifier token at given depth
     * @in depth the number of dereferencing 0 means none
     * @out  token is zero if no modifier at this depth
     *       possible values are  A=array P=pointer V=Vector M=Matrix
     * @out  constant if the var is constant
     * @out  only for A(arrays) size will return as size of the array
     */
    void GetModifier(char8 &token, bool &constant, uint32 &size,uint32 depth) const;

    /**
     * @brief Returns the TypeDescriptor describing the most inner storage
     * @return the data TypeDescriptor.
     */
    const TypeDescriptor &GetFullTypeDescriptor() const;

    /**
     * @brief Returns the TypeDescriptor describing the overall variable
     * @return the data TypeDescriptor.
     */
    const TypeDescriptor &GetTopTypeDescriptor() const;

    /**
     * @brief Gets the number of dimensions associated to this Match.
     * @details GetNumberOfDimensions() == 0 => scalar, GetNumberOfDimensions() == 1 => vector
     * GetNumberOfDimensions() == 2 => matrix
     * @return the number of dimensions associated to this Match.
     */
//    inline uint8 GetNumberOfDimensions() const;

    /**
     * @brief Gets the number of elements in a given \a dimension.
     * @param[in] dimension the dimension to be queried.
     * @return the number of elements in a given \a dimension.
     * @pre
     *   dimension < 3
     */
//    inline uint32 GetNumberOfElements(const uint32 dimension) const;

    /**
     * @brief Checks if GetDataPointer() is pointing at a statically allocated memory block.
     * @return true if GetDataPointer() is pointing at a statically allocated memory block.
     */
//    inline bool IsStaticDeclared() const;

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

    /**
     * TODO remove?
     */
//    char8 *GetRawModifiers(){
//        return modifiers;
//    }

private:

    /**
     * Adds the declaration of a pointer/vector/matrix to the modifiers
     * It will include the constant
     */
    //void AddGenericToModifiers(char8 token,bool &constant);

    /**
     * Adds the declaration of a vector to the modifiers
     */
    //void AddArrayToModifiers(uint32 vectorSize,bool &constant);

    /**
     * Adds a code in the modifiers if one present in code
     * Clears code and
     */
    //void AddArrayCode(ModifierCode &code);

    void AddArrayCode(BasicArrayType bat, uint32 size1);

    void AddConstantCode();

    void FinaliseCode(TypeDescriptor td);

    /// outputs the modifiers stored in the typeDescriptor into the modifier string
    void MoveCodeToModifiers();

    /**
     *  @brief a zero terminated sequence of tokens.
     *  @full each token can be a character or a sequence of characters and bytes
    */
//    char8 *             modifiers;
    DynamicCString      modifiers;

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
    void Match(Matrix<T> * mat);

    /**
     * @brief Matches a Vector
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T>
    void Match(Vector<T> * vec);

    /**
     * @brief Matches a static zero terminated array
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T>
    void Match(ZeroTerminatedArray<T> * vec);

    /**
     * @brief Matches a dynamic zero terminated array
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T >
    void Match(DynamicZeroTerminatedArray<T,16u> * vec);

    /**
     * @brief Matches a zero terminated array statically allocated T[N]
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T,uint32 sz >
    void Match(StaticZeroTerminatedArray<T,sz> * vec);

    /**
     * @brief Matches a T* const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T * const * x);

    /**
     * @brief Matches a T[n]
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a [n] to the modifiers
     */
    template <class T,unsigned int n>
    inline void Match(T (*x) [n]);

    /**
     * @brief Matches a T*
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a * to the modifiers
     */
    template <class T>
    inline void Match(T ** x);


    /**
     * @brief Matches a T const *
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a * to the modifiers
     */
    template <class T>
    inline void Match(T const * * x);

    /**
     * @brief Matches a T const * const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T const * const * x);

    /**
     * @brief Matches a T const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T const * x);

    /**
     * @brief Matches a T
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void Match(T * x);

 // SPECIFIC MATCHES

    /**
     * @brief Constructor from zeroterm malloced char *
     * @param[in] s is the string
     * * @post TODO
     *   GetDataPointer() == &i &&
     *   GetTypeDescriptor() == CString
     */
    inline void Match(DynamicCString *s);

    /**
     * @brief Constructor from zeroterm char[]
     * @param[in] s is the string
     * * @post TODO
     *   GetDataPointer() == &i &&
     *   GetTypeDescriptor() == CString
     */
    template <uint32 sz>
    inline void Match(StaticCString<sz> *s);

    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     * * @post TODO
     *   GetDataPointer() == &i &&
     *   GetTypeDescriptor() == CString
     */
    inline void Match(CString *s);

    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     * @post
     *   GetDataPointer() == &i &&
     *   GetTypeDescriptor() == CString &&
     */
    inline void Match(CCString *s);

    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     * @post TODO
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == Character8Bit &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(char8 *i);


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
    inline void Match(int8 *i);

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
    inline void Match(uint8 *i);

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

    inline void Match(int16 *i);

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

    inline void Match(uint16 *i);

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
    inline void Match(int32 *i);

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
    inline void Match(uint32 *i);

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
    inline void Match(int64 *i);

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
    inline void Match(uint64 *i);

    /**
     * @brief Constructor from 32 bit float32 number.
     * @param[in] i is the 32 bit float32 number input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(float32 *i);

    /**
     * @brief Constructor from 64 bit float32 number.
     * @param[in] i the 64 bit float32 number input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(float64 *i);

    /**
     * @brief Constructor from void pointer.
     * @param[in] p is the void pointer input.
     * @post
     *   GetDataPointer() == p &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(void * *p);

    /**
     * @brief Constructor from Object (or inherited class).
     * @param[in] obj the source Object.
     * @post
     *   GetDataPointer() == &obj &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    void Match(Object *obj);

    /**
     * @brief Constructor by BitBoolean.
     * @tparam baseType the standard type which is used as a base for bitBool's type
     * @tparam bitOffset the actual bit offset of the bitBool's type
     * @param[in] bitBool is the BitBoolean object input.
     */
    template<typename baseType, uint8 bitOffset>
    void Match(BitBoolean<baseType, bitOffset> * bitBool);

    /**
     * @brief Constructor by BitRange.
     * @tparam baseType the standard type which is used as a base for bitRange's type
     * @tparam bitSize the actual bit size of the bitRange's type
     * @tparam bitOffset the actual bit offset of the bitRange's type
     * @param[in] bitRange is the BitRange object input.
     */
    template<typename baseType, uint8 bitSize, uint8 bitOffset>
    void Match(BitRange<baseType, bitSize, bitOffset> * bitRange);

    /**
     * @brief Constructor by FractionalInteger.
     * @tparam baseType the standard type which is used as a base for fractionalInt's type
     * @tparam bitSize the actual bit size of the fractionalInt's type
     * @param[in] fractionalInt is the FractionalInteger object input.
     */
    template<typename baseType, uint8 bitSize>
    void Match(FractionalInteger<baseType, bitSize> * fractionalInt);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


template <class T>
inline  VariableDescriptor::VariableDescriptor( T  x){
    typeDescriptor = VoidType;
    Match(x);
}


template<typename T>
void VariableDescriptor::Match(Vector<T> * vec) {
	AddArrayCode(Array1D,0);

    T *pp = NULL;
    Match(pp);
}

template<typename T>
void VariableDescriptor::Match(Matrix<T> * mat) {
	AddArrayCode(Array2D,0);

    T *pp = NULL;
    Match(pp);
}

template<typename T>
void VariableDescriptor::Match(ZeroTerminatedArray<T> * vec){
	AddArrayCode(ZeroTermArray,0);

    T *pp = NULL;
    Match(pp);
}

template<typename T >
void VariableDescriptor::Match(DynamicZeroTerminatedArray<T,16u> * vec){
	AddArrayCode(DynamicZeroTermArray,0);

    T *pp = NULL;
    Match(pp);
}

template<typename T, uint32 sz >
void VariableDescriptor::Match(StaticZeroTerminatedArray<T,sz> * vec){
	AddArrayCode(StaticZeroTermArray,sz);

    T *pp = NULL;
    Match(pp);
}

template <class T,unsigned int n>
inline void VariableDescriptor::Match(T (*x) [n]){
	AddArrayCode(SizedCArray,n);

    T *pp = NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T ** x){
	AddArrayCode(PointerArray,0);

    T *pp = NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T * const * x){

    AddConstantCode();
    T ** pp=NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T const * * x){
	AddArrayCode(PointerArray,0);

    T const * pp=NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T const * const * x){

    AddConstantCode();
    T const * * pp=NULL;
    Match(pp);
}

template <class T>
void VariableDescriptor::Match(T const * x){

    AddConstantCode();
    T * pp=NULL;
    Match(pp);
}

template <class T>
void VariableDescriptor::Match(T * x){
    ClassRegistryItem *cri =  ClassRegistryItem::Instance<T>();
    if (cri != NULL) {
    	FinaliseCode(cri->GetTypeDescriptor());
    }
    else {
    	FinaliseCode(VoidType);
    }

}

void VariableDescriptor::Match(DynamicCString *s){
	FinaliseCode(DynamicCharString);
}

void VariableDescriptor::Match(CString *s){
	FinaliseCode(CharString);
}

void VariableDescriptor::Match(CCString *s){
	FinaliseCode(ConstCharString);
}

template <uint32 sz>
void VariableDescriptor::Match(StaticCString<sz> *s){
	TypeDescriptor td(false,Char, Size8bit,StaticZeroTermArray, sz);
	FinaliseCode(td);
}


void VariableDescriptor::Match(char8 * i) {
	FinaliseCode(Character8Bit);
}

void VariableDescriptor::Match(int8 * i) {
	FinaliseCode(SignedInteger8Bit);
}

void VariableDescriptor::Match(uint8 * i) {
	FinaliseCode(UnsignedInteger8Bit);
}

void VariableDescriptor::Match(int16 * i) {
	FinaliseCode(SignedInteger16Bit);
}

void VariableDescriptor::Match(uint16 * i) {
	FinaliseCode(UnsignedInteger16Bit);
}

void VariableDescriptor::Match(int32 * i) {
	FinaliseCode(SignedInteger32Bit);
}

void VariableDescriptor::Match(uint32 * i) {
	FinaliseCode(UnsignedInteger32Bit);
}

void VariableDescriptor::Match(int64 * i) {
	FinaliseCode(SignedInteger64Bit);
}

void VariableDescriptor::Match(uint64 * i) {
	FinaliseCode(UnsignedInteger64Bit);
}

void VariableDescriptor::Match(float32 * i) {
	FinaliseCode(Float32Bit);
}

void VariableDescriptor::Match(float64 * i) {
	FinaliseCode(Float64Bit);
}

void VariableDescriptor::Match(void * * p) {
//    TypeDescriptor td(false,Pointer,1,sizeof(void*),1);
	FinaliseCode(VoidPointer);
}

template<typename baseType, uint8 bitOffset>
void VariableDescriptor::Match(BitBoolean<baseType, bitOffset> * bitBool) {
	TypeDescriptor td(false,UnsignedInteger,1,bitOffset);
	FinaliseCode(td);
}

template<typename baseType, uint8 bitSize, uint8 bitOffset>
void VariableDescriptor::Match(BitRange<baseType, bitSize, bitOffset> * bitRange) {
    BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    TypeDescriptor td(false,type,bitSize,bitOffset);
	FinaliseCode(td);
}

template<typename baseType, uint8 bitSize>
void VariableDescriptor::Match(FractionalInteger<baseType, bitSize> * fractionalInt) {
    BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    TypeDescriptor td(false,type,bitSize,0);
	FinaliseCode(td);
}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}
#endif /* L2OBJECTS_VARIABLEDESCRIPTOR_H_ */
	
