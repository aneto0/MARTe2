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

#if 0
/**
 *
 */
class ModifierCode{

public:
	///
	void SetConstant(){
		constant = true;
	}

	///
	void SetArrayCode(char8 codeIn,uint32 sizeIn){
		code = codeIn;
		size = size;
	}

	///
	void SetCode(char8 codeIn){
		code = codeIn;
	}

	///
	ModifierCode(){
		Clear();
	}

	///
	void Clear(){
		size = 0;
		code = '\0';
		constant = false;
	}
	///
	bool HasCode(){
		return (code != '\0');
	}
};
#endif

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
     * @brief Returns the pointed data TypeDescriptor.
     * @return the data TypeDescriptor.
     */
    const TypeDescriptor &GetFullTypeDescriptor() const;

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
    //void QueueCode(ModifierCode &code);

    void QueueCode(BasicArrayType bat, uint32 size1,bool constant);

    void ResolveCode(TypeDescriptor td,bool constant);

    void MoveCodeToModifiers(uint32 nextArraySize);

    /**
     *  @brief a zero terminated sequence of tokens.
     *  @full each token can be a character or a sequence of characters and bytes
     *  simple tokens are pPMV (const pointer,pointer, Vector,Matrix)
     *  tokens followed by one byte:    aA  (const array[<256], array[<256])
     *  tokens followed by two bytes:   bB  (const array[<65536], array[<65536])
     *  tokens followed by four bytes:  cC  (const array[], array[])
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
    void Match(Matrix<T> * mat,bool constant);

    /**
     * @brief Matches a Vector
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T>
    void Match(Vector<T> * vec,bool constant);

    /**
     * @brief Matches a static zero terminated array
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T>
    void Match(ZeroTerminatedArray<T> * vec,bool constant);

    /**
     * @brief Matches a dynamic zero terminated array
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T >
    void Match(DynamicZeroTerminatedArray<T,16u> * vec,bool constant);

    /**
     * @brief Matches a zero terminated array statically allocated T[N]
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T,uint32 sz >
    void Match(StaticZeroTerminatedArray<T,sz> * vec,bool constant);

    /**
     * @brief Matches a T* const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T * const * x,bool constant);

    /**
     * @brief Matches a T[n]
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a [n] to the modifiers
     */
    template <class T,unsigned int n>
    inline void Match(T (*x) [n],bool constant);

    /**
     * @brief Matches a T*
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a * to the modifiers
     */
    template <class T>
    inline void Match(T ** x,bool constant);


    /**
     * @brief Matches a T const *
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a * to the modifiers
     */
    template <class T>
    inline void Match(T const * * x,bool constant);

    /**
     * @brief Matches a T const * const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T const * const * x,bool constant);

    /**
     * @brief Matches a T const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T const * x,bool constant);

    /**
     * @brief Matches a T
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void Match(T * x,bool constant);

 // SPECIFIC MATCHES

    /**
     * @brief Constructor from zeroterm malloced char *
     * @param[in] s is the string
     * * @post TODO
     *   GetDataPointer() == &i &&
     *   GetTypeDescriptor() == CString
     */
    inline void Match(DynamicCString *s,bool constant);

    /**
     * @brief Constructor from zeroterm char[]
     * @param[in] s is the string
     * * @post TODO
     *   GetDataPointer() == &i &&
     *   GetTypeDescriptor() == CString
     */
    template <uint32 sz>
    inline void Match(StaticCString<sz> *s,bool constant);

    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     * * @post TODO
     *   GetDataPointer() == &i &&
     *   GetTypeDescriptor() == CString
     */
    inline void Match(CString *s,bool constant);

    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     * @post
     *   GetDataPointer() == &i &&
     *   GetTypeDescriptor() == CString &&
     */
    inline void Match(CCString *s,bool constant);

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
    inline void Match(char8 *i,bool constant);


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
    inline void Match(int8 *i,bool constant);

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
    inline void Match(uint8 *i,bool constant);

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

    inline void Match(int16 *i,bool constant);

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

    inline void Match(uint16 *i,bool constant);

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
    inline void Match(int32 *i,bool constant);

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
    inline void Match(uint32 *i,bool constant);

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
    inline void Match(int64 *i,bool constant);

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
    inline void Match(uint64 *i,bool constant);

    /**
     * @brief Constructor from 32 bit float32 number.
     * @param[in] i is the 32 bit float32 number input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(float32 *i,bool constant);

    /**
     * @brief Constructor from 64 bit float32 number.
     * @param[in] i the 64 bit float32 number input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(float64 *i,bool constant);

    /**
     * @brief Constructor from void pointer.
     * @param[in] p is the void pointer input.
     * @post
     *   GetDataPointer() == p &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline void Match(void * *p,bool constant);

    /**
     * @brief Constructor from Object (or inherited class).
     * @param[in] obj the source Object.
     * @post
     *   GetDataPointer() == &obj &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    void Match(Object *obj,bool constant);

    /**
     * @brief Constructor by BitBoolean.
     * @tparam baseType the standard type which is used as a base for bitBool's type
     * @tparam bitOffset the actual bit offset of the bitBool's type
     * @param[in] bitBool is the BitBoolean object input.
     */
    template<typename baseType, uint8 bitOffset>
    void Match(BitBoolean<baseType, bitOffset> * bitBool,bool constant);

    /**
     * @brief Constructor by BitRange.
     * @tparam baseType the standard type which is used as a base for bitRange's type
     * @tparam bitSize the actual bit size of the bitRange's type
     * @tparam bitOffset the actual bit offset of the bitRange's type
     * @param[in] bitRange is the BitRange object input.
     */
    template<typename baseType, uint8 bitSize, uint8 bitOffset>
    void Match(BitRange<baseType, bitSize, bitOffset> * bitRange,bool constant);

    /**
     * @brief Constructor by FractionalInteger.
     * @tparam baseType the standard type which is used as a base for fractionalInt's type
     * @tparam bitSize the actual bit size of the fractionalInt's type
     * @param[in] fractionalInt is the FractionalInteger object input.
     */
    template<typename baseType, uint8 bitSize>
    void Match(FractionalInteger<baseType, bitSize> * fractionalInt,bool constant);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


template <class T>
inline  VariableDescriptor::VariableDescriptor( T  x):VariableDescriptor(){
    Match(x,false);
}


template<typename T>
void VariableDescriptor::Match(Vector<T> * vec,bool constant) {
	QueueCode(Array1D,0,constant);
	constant = false;
    T *pp = NULL;
    Match(pp,constant);
}

template<typename T>
void VariableDescriptor::Match(Matrix<T> * mat,bool constant) {
	QueueCode(Array2D,0,constant);
	constant = false;
    T *pp = NULL;
    Match(pp,constant);
}

template<typename T>
void VariableDescriptor::Match(ZeroTerminatedArray<T> * vec,bool constant){
	QueueCode(ZeroTermArray,0,constant);
	constant = false;
    T *pp = NULL;
    Match(pp,constant);
}

template<typename T >
void VariableDescriptor::Match(DynamicZeroTerminatedArray<T,16u> * vec,bool constant){
	QueueCode(DynamicZeroTermArray,0,constant);
	constant = false;
    T *pp = NULL;
    Match(pp,constant);
}

template<typename T, uint32 sz >
void VariableDescriptor::Match(StaticZeroTerminatedArray<T,sz> * vec,bool constant){
	QueueCode(StaticZeroTermArray,sz,constant);
	constant = false;
    T *pp = NULL;
    Match(pp,constant);
}

template <class T,unsigned int n>
inline void VariableDescriptor::Match(T (*x) [n],bool constant){
	QueueCode(Array1D,n,constant);
	constant = false;
    T *pp = NULL;
    Match(pp,constant);
}

template <class T>
inline void VariableDescriptor::Match(T ** x,bool constant){
	QueueCode(PointerArray,0,constant);
	constant = false;
    T *pp = NULL;
    Match(pp,constant);
}

template <class T>
inline void VariableDescriptor::Match(T * const * x,bool constant){
    constant = true;
    T ** pp=NULL;
    Match(pp,constant);
}

template <class T>
inline void VariableDescriptor::Match(T const * * x,bool constant){
	QueueCode(PointerArray,0,constant);
	constant = false;
    T const * pp=NULL;
    Match(pp,constant);
}

template <class T>
inline void VariableDescriptor::Match(T const * const * x,bool constant){
    constant = true;
    T const * * pp=NULL;
    Match(pp,constant);
}

template <class T>
void VariableDescriptor::Match(T const * x,bool constant){
    constant = true;
    T * pp=NULL;
    Match(pp,constant);
}

template <class T>
void VariableDescriptor::Match(T * x,bool constant){
    ClassRegistryItem *cri =  ClassRegistryItem::Instance<T>();
    if (cri != NULL) {
    	ResolveCode(cri->GetTypeDescriptor(),constant);
    }
    else {
    	ResolveCode(VoidType,constant);
    }

}

void VariableDescriptor::Match(DynamicCString *s,bool constant){
	ResolveCode(DynamicCharString,constant);
}

void VariableDescriptor::Match(CString *s,bool constant){
	ResolveCode(CharString,constant);
}

void VariableDescriptor::Match(CCString *s,bool constant){
	ResolveCode(ConstCharString,constant);
}

template <uint32 sz>
void VariableDescriptor::Match(StaticCString<sz> *s,bool constant){
	TypeDescriptor td(false,Char, Size8bit,StaticZeroTermArray, sz,  0);
	ResolveCode(td,constant);
}


void VariableDescriptor::Match(char8 * i,bool constant) {
	ResolveCode(Character8Bit,constant);
}

void VariableDescriptor::Match(int8 * i,bool constant) {
	ResolveCode(SignedInteger8Bit,constant);
}

void VariableDescriptor::Match(uint8 * i,bool constant) {
	ResolveCode(UnsignedInteger8Bit,constant);
}

void VariableDescriptor::Match(int16 * i,bool constant) {
	ResolveCode(SignedInteger16Bit,constant);
}

void VariableDescriptor::Match(uint16 * i,bool constant) {
	ResolveCode(UnsignedInteger16Bit,constant);
}

void VariableDescriptor::Match(int32 * i,bool constant) {
	ResolveCode(SignedInteger32Bit,constant);
}

void VariableDescriptor::Match(uint32 * i,bool constant) {
	ResolveCode(UnsignedInteger32Bit,constant);
}

void VariableDescriptor::Match(int64 * i,bool constant) {
	ResolveCode(SignedInteger64Bit,constant);
}

void VariableDescriptor::Match(uint64 * i,bool constant) {
	ResolveCode(UnsignedInteger64Bit,constant);
}

void VariableDescriptor::Match(float32 * i,bool constant) {
	ResolveCode(Float32Bit,constant);
}

void VariableDescriptor::Match(float64 * i,bool constant) {
	ResolveCode(Float64Bit,constant);
}

void VariableDescriptor::Match(void * * p,bool constant) {
    TypeDescriptor td(false,Pointer,1,sizeof(void*),1,0);
	ResolveCode(td,constant);
}

template<typename baseType, uint8 bitOffset>
void VariableDescriptor::Match(BitBoolean<baseType, bitOffset> * bitBool,bool constant) {
	TypeDescriptor td(false,UnsignedInteger,1,bitOffset);
	ResolveCode(td,constant);
}

template<typename baseType, uint8 bitSize, uint8 bitOffset>
void VariableDescriptor::Match(BitRange<baseType, bitSize, bitOffset> * bitRange,bool constant) {
    BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    TypeDescriptor td(false,type,bitSize,bitOffset);
	ResolveCode(td,constant);
}

template<typename baseType, uint8 bitSize>
void VariableDescriptor::Match(FractionalInteger<baseType, bitSize> * fractionalInt,bool constant) {
    BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    TypeDescriptor td(false,type,bitSize,0);
	ResolveCode(td,constant);
}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}
#endif /* L2OBJECTS_VARIABLEDESCRIPTOR_H_ */
	
