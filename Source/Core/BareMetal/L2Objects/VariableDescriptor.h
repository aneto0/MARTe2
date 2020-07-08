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
#include "ClassRegistryDatabase.h"
#include "FractionalInteger.h"
#include "Matrix.h"
#include "ZeroTerminatedArray.h"
#include "DynamicZeroTerminatedArray.h"
#include "StaticZeroTerminatedArray.h"

namespace MARTe {
class Object;
class DynamicCString;
class StreamString;
class StructuredDataI;
class testStruct;
class Reference;
template <uint32 sz>  class StaticCString;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


/**
 * @brief full description of the type of a variable including modifiers
 */
class DLL_API VariableDescriptor{

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
     * @brief copy constructor
     */
    VariableDescriptor(const VariableDescriptor &x );

    /**
     * @brief copy operator
     */
    VariableDescriptor &operator=(const VariableDescriptor &x );

    /**
     * @brief sets the typeDescriptor
     * @param[in] dataDescriptorIn contains the type informations in a TypeDescriptor class.
     * @param[in] modifiers contains the modifiers string in the appropriate language (see VariableDescriptor).
     */
    VariableDescriptor(const TypeDescriptor &td, CCString modifierString=emptyString);


    /**
     * @brief compare two VariableDescriptors
     * compares both TypeDescriptor using SameAs()
     * and compare the two modifier strings to be an exact match
     */
    inline bool SameAs(const VariableDescriptor &compare) const;

    /**
     * @brief returns size of all the memory addressed by this variable.
     * @param[in] pointer, the pointer to the variable
     * The functions handles complex arrays where dimensions are linked by pointers and maybe of variable size
     * The dimensions are considered linked if:
     * 1) linked by an Array or a vector:  Vector<int [][]>[][] is considered a 5 dimensional array (1 variable)
     * 2) linked by a Zero Terminated:     ZeroTerminatedArray<int [][]>[][] is considered a 5 dimensional array (1 variable)
     * 3) linked by a pointer to an Array: (* [][])[][] is considered a 4 dimensional array (the * dimension is dropped)
     * 4) will not link pointer to data types: a float * is simply a pointer
     * Note that pointers to type are considered just pointers
     * @param[out] dataSize is the full size of the memory necessary to store this var: auxilliary storages + actual data
     * @param[out] overHeadSize is the memory used as overhead: pointers etc
     * @return true if all ok
     */
    ErrorManagement::ErrorType GetSize(const uint8 *pointer,uint64 &dataSize, uint64 *overHeadSize=NULL) const;

    /**
     * @brief removes one indirection layer and update variable pointer and type.
     * Can be used to navigate into arrays of all types, pointers
     * (NEW OPTION) to be enabled in c++ also into structure members (order not necessary following memory address)
     * @param[in,out] pointer, the pointer to the variable
     * @param[in] index the offset
     * @param[in] modifierString, used internally to perform recursion
     * @return true if all ok or the error
     */
    ErrorManagement::ErrorType Redirect(const uint8 *&pointer,uint32 index,CCString modifierString=emptyString) ;

    /**
     * @see TypeDescriptor::SetFormattedStreamType
     * @return unsupportedFeature if type is not compatible
     */
    ErrorManagement::ErrorType SetFormattedStreamType(CCString format) ;

    /**
    * @brief creates an object clone of the variable
    * @details:
    * The functions also copies complex arrays where dimensions are linked by pointers and maybe of variable size
    * The dimensions are considered linked if:
    * 1) linked by an Array or a vector:  Vector<int [][]>[][] is considered a 5 dimensional array (1 variable)
    * 2) linked by a Zero Terminated:     ZeroTerminatedArray<int [][]>[][] is considered a 5 dimensional array (1 variable)
    * 3) linked by a pointer to an Array: (* [][])[][] is considered a 4 dimensional array (the * dimension is dropped)
    * 4) will not link pointer to data types: a float * is simply a pointer
    * The function will copy/compare among object with the same number of dimensions if each dimension has the same size
    * @param[in] sourcePtr, the pointer to the variable to be copied
    * @param[in] destVd, the variable descriptor of the destination
    * @param[out] reference, the Reference to an Object clone of this data
    * @return true if all ok or the error
    */
    ErrorManagement::ErrorType Clone(
    		const uint8 *		Pointer,
			Reference 		   &reference
			) const;

    /**
     * @brief converts the variable to the destination type and then copies/compares to/with dest
     * @details
     * The functions also copies complex arrays where dimensions are linked by pointers and maybe of variable size
     * The dimensions are considered linked if:
     * 1) linked by an Array or a vector:  Vector<int [][]>[][] is considered a 5 dimensional array (1 variable)
     * 2) linked by a Zero Terminated:     ZeroTerminatedArray<int [][]>[][] is considered a 5 dimensional array (1 variable)
     * 3) linked by a pointer to an Array: (* [][])[][] is considered a 4 dimensional array (the * dimension is dropped)
     * 4) will not link pointer to data types: a float * is simply a pointer
     * The function will copy/compare among object with the same number of dimensions if each dimension has the same size
     * @param[in] sourcePtr, the pointer to the variable to be copied
     * @param[in] destPtr, the pointer to the destination
     * @param[in] destVd, the variable descriptor of the destination
     * @param[in] isCompare, true means comparison
     * @return true if all ok or the error
     */
    ErrorManagement::ErrorType CopyTo(
    		const uint8 *sourcePtr,
    			  uint8 *destPtr,
				  const VariableDescriptor &destVd,
				  bool isCompare= false
    		) const ;

    /**
     * @brief Converts type descriptor to C/c++ equivalent
     * @return true if all ok
     */
    ErrorManagement::ErrorType  ToString(CStringTool &string,bool rawFormat=false) const;

    /**
     * @brief Converts type descriptor to C/c++ equivalent
     * @return true if all ok
     */
    inline ErrorManagement::ErrorType  ToString(DynamicCString &string,bool rawFormat=false) const;


    /**
     * @brief Converts C/c++ string to type descriptor
     * @return true if all ok
     */
    ErrorManagement::ErrorType  FromString(CCString typeName);

    /**
     * @brief provides a typeDescriptor for the overall variable.
     * @details uses GetVariableDimensions with nOfDimensions= 0
     * @see GetVariableDimensions
     * @return the summary type descriptor
     */
    inline TypeDescriptor GetSummaryTypeDescriptor() const;

    /**
     * @brief measure the size of each dimensions of a variable
     * @details delves into a variable counting the number of dimensions. It also returns the size of each dimensions
     * it delves as deep as specified by nOfDiemsions.
     * simple variable are considered 0 dimension variables
     * Vector<type> or type[] have 1 dimensions array<type> or type[][] have 2 dimensions
     * combinations like Vector<type[]> are processed
     * td describes the actual type of the variable only if all the dimensions have been parsed. otherwise returns GenericArray
     * For instance a zero dim variable will be described correctly if nOfDimensions is >=0
     * For instance a vector will be described completely if nOfDimensions is >=1.
     * If nOfDimensions= 0 will be reported as a GenericArray and no size will be returned
     * in case of a variable that a combination of array and pointers, the dimension scan will stop at the occurrence of the first pointer
     * for Matrix, Vector, ZeroTerminatedArray, as their dimensions is not predetermined they will reported as size 0 unless
     * they occur as the first dimension.
     * The function called with both NULL pointers will return just the equivalent TypeDescriptor at a given depth of redirection
     * @param[in] variablePtr is the pointer to the variable (needed to get actual information of variable dimensions for some types). If NULL size of Matrix, Vector, ZTAs is reported as 0
     * @param[in] dimensionSizes is the pointer to a vector of uint32 larger than the value of depth. Upon return will contain
     * the size of each dimensions. If a dimensions has a variable size its size is reported as zero. For instance
     * Vector<int> a[N] will be reported as {N,0} as the size of each Vector may be different. If NULL the dimensions will no be reported.
     * @param[in,out] nOfDimensions specifies the max number of dimensions to examine. Upon return will contain the number of
     * dimensions encountered if not more than originally specified in depth
     * @param[in,out] td will be the actual variable type as seen at the last dimension explored
     * @return internalStateError if a ZeroTerminatedArray of elements that are too large is encountered
     */
    ErrorManagement::ErrorType GetVariableDimensions(
    		const void *		variablePtr,
    		TypeDescriptor &	td,
			uint32 &			nOfDimensions,
			uint32 *			dimensionSizes) const;

    /**
     * @brief getter for modifiers
     */
    inline const char8 *GetModifiers() const;

    /**
     * @brief getter for typeDescriptor
     */
    inline TypeDescriptor GetFinalTypeDescriptor() const;

private:
    /**
     *  @brief a zero terminated sequence of tokens.
     *  @full each token can be a character or a sequence of characters and bytes
    */
    DynamicCString      modifiers;

    /**
     * @brief The type of the (final after redirections) variable
    */
    TypeDescriptor     typeDescriptor;

/*
 *  PRIVATE METHODS
 */

    /**
     * @brief adds one layer of modifiers to the top used in building the object
     * @return true if operation succeeded
     */
    void AddModifiersLayerConst(char8 modifier, uint64 size);

    /**
     * @brief to encode the actual data type. It will incorporate the const and Array code
     */
    inline void FinaliseCode(TypeDescriptor td);

    // GENERAL MATCHES
    // called by the main constructor

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
    void Match(DynamicZeroTerminatedArray<T> * vec);

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
     * @brief Matches a T[n]
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a [n] to the modifiers
     */
    template <class T,unsigned int n>
    inline void Match(const T (*x) [n]);

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


    // GENERIC FINAL  MATCHES
    /**
     * @brief Matches a T
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post delegates to MatchFinal
     */
    template <class T>
    inline void Match(T * x);
    //{    	MatchFinal(x,x);    }

    /**
     * @brief Matches a T if not derivative of StreamI Object or StructuredDataI
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void MatchFinal(T *y,typename enable_if<!isSameOrBaseOf(StreamI,T)&&!isSameOrBaseOf(Reference,T)/*&&!isSameOrBaseOf(Object,T)*/&&!isSameOrBaseOf(StructuredDataI,T), T>::type *x);

    /**
     * @brief Matches a T if derivative of StreamI but not StreamString
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void MatchFinal(T *y,typename enable_if<isSameOrBaseOf(StreamI,T) && !isSame(StreamString,T), T>::type *x);

    /**
     * @brief Matches a T if StreamString - late matching allows sizeof incomplete class
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void MatchFinal(T *y,typename enable_if<isSame(StreamString,T), T>::type *x);

    /**
     * @brief Matches a T if derivative of StructuredDataI
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void MatchFinal(T *y,typename enable_if<isSameOrBaseOf(StructuredDataI,T), T>::type *x);

    // SPECIFIC FINAL MATCHES
    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     */
    inline void Match(char8 *i);

    /**
     * @brief Constructor from signed 8 bit integer.
     * @param[in] i is the signed 8 bit integer input.
     */
    inline void Match(int8 *i);

    /**
     * @brief Constructor from unsigned 8 bit integer.
     * @param[in] i is the unsigned 8 bit integer input.
     */
    inline void Match(uint8 *i);

    /**
     * @brief Constructor from constant signed 8 bit integer.
     * @param[in] i is the constant signed 8 bit integer input.
     */

    inline void Match(int16 *i);

    /**
     * @brief Constructor from unsigned 16 bit integer.
     * @param[in] i is the unsigned 16 bit integer input.
     */

    inline void Match(uint16 *i);

    /**
     * @brief Constructor from signed 32 bit integer.
     * @param[in] i is the signed 32 bit integer input.
     */
    inline void Match(int32 *i);

    /**
     * @brief Constructor from unsigned 32 bit integer.
     * @param[in] i is the unsigned 32 bit integer input.
     */
    inline void Match(uint32 *i);

    /**
     * @brief Constructor from signed 64 bit integer.
     * @param[in] i is the signed 64 bit integer input.
     */
    inline void Match(int64 *i);

    /**
     * @brief Constructor from unsigned 64 bit integer.
     * @param[in] i is the unsigned 64 bit integer input.
     */
    inline void Match(uint64 *i);

    /**
     * @brief Constructor from 32 bit float32 number.
     * @param[in] i is the 32 bit float32 number input.
     */
    inline void Match(float32 *i);

    /**
     * @brief Constructor from 64 bit float32 number.
     * @param[in] i the 64 bit float32 number input.
     */
    inline void Match(float64 *i);

    /**
     * @brief Constructor from void pointer.
     * @param[in] p is the void pointer input.
     */
//    inline void Match(void * *p);
    inline void Match(void * p);

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

    /**
     * @brief Constructor from zeroterm malloced char *
     * @param[in] s is the string
     */
    inline void Match(DynamicCString *s);


    /**
     * @brief Constructor from zeroterm char[]
     * @param[in] s is the string
     */
    template <uint32 sz>
    inline void Match(StaticCString<sz> *s);

    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     */
    inline void Match(CString *s);

    /**
     * @brief Constructor from CString.
     * @param[in] s is the CString.
     */
    inline void Match(CCString *s);
    /**
     * @brief Constructor from StreamString (or inherited class).
     * @param[in] obj the source Object.
     * @details function templated to allow late calculation of sizeof(StreamString)
     */
    template <class T>
    inline void Match(StreamString *obj);

    /**
     * @brief Constructor from structured data.
     * @param[in] s is a structured data.
     */
    inline void Match(StructuredDataI *s);


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool VariableDescriptor::SameAs(const VariableDescriptor &compare) const{
	bool isSame = compare.typeDescriptor.SameAs(typeDescriptor);
	if (isSame){
		isSame = (compare.modifiers == modifiers);
	}
	return isSame;
}


template <class T>
inline  VariableDescriptor::VariableDescriptor( T  x){
    typeDescriptor = VoidType;
    Match(x);
}

template<typename T>
void VariableDescriptor::Match(Vector<T> * vec) {
	AddModifiersLayerConst('V', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T>
void VariableDescriptor::Match(Matrix<T> * mat) {
	AddModifiersLayerConst('M', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T>
void VariableDescriptor::Match(ZeroTerminatedArray<T> * vec){
	AddModifiersLayerConst('Z', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T >
void VariableDescriptor::Match(DynamicZeroTerminatedArray<T> * vec){
	AddModifiersLayerConst('D', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T, uint32 sz >
void VariableDescriptor::Match(StaticZeroTerminatedArray<T,sz> * vec){
	AddModifiersLayerConst('S', sz);

    T *pp = NULL;
    Match(pp);
}

void VariableDescriptor::Match(DynamicCString *s){
	bool isConstant = typeDescriptor.DataIsConstant();
	if (isConstant){
		FinaliseCode(ConstCharString);
	} else {
		typeDescriptor.SetDataConstant(false);
		FinaliseCode(DynamicCharString);
	}
}

void VariableDescriptor::Match(CString *s){
	FinaliseCode(CharString(0));
}

void VariableDescriptor::Match(CCString *s){
	FinaliseCode(ConstCharString);
}

// TODO wrong. this is not a pointer!
template <uint32 sz>
void VariableDescriptor::Match(StaticCString<sz> *s){
	FinaliseCode(CharString(sz));
}

template <class T,unsigned int n>
inline void VariableDescriptor::Match(T (*x) [n]){
	if (n > 1){ // collapsing away [1]
		AddModifiersLayerConst('A', n);
	}

    T *pp = NULL;
    Match(pp);
}

template <class T,unsigned int n>
inline void VariableDescriptor::Match( const T (*x) [n]){
	if (n > 1){// collapsing away [1]
		AddModifiersLayerConst('A', n);
	}

    const T *pp = NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T ** x){
	AddModifiersLayerConst('P', 0);
    T *pp = NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T * const * x){
	typeDescriptor.SetDataConstant(true);;
    T ** pp=NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T const * * x){
	AddModifiersLayerConst('P', 0);

    T const * pp=NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T const * const * x){
	typeDescriptor.SetDataConstant(true);
    T const * * pp=NULL;
    Match(pp);
}

template <class T>
void VariableDescriptor::Match(T const * x){
	typeDescriptor.SetDataConstant(true);
    T * pp=NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T * x){
	MatchFinal(x,x);
}

template <class T>
void VariableDescriptor::Match(StreamString *s){
	FinaliseCode(StreamStringType(sizeof(T)));
}

template <class T>
void VariableDescriptor::MatchFinal(T *y,typename enable_if<!isSameOrBaseOf(StreamI,T)&&!isSameOrBaseOf(Reference,T)/*&&!isSameOrBaseOf(Object,T)*/&&!isSameOrBaseOf(StructuredDataI,T), T>::type *x){
	ClassRegistryItem *cri =  ClassRegistryItem::Instance<T>();
    if (cri != NULL) {
    	FinaliseCode(cri->GetTypeDescriptor());
    }
    else {
    	FinaliseCode(VoidType);
    }
}

template <class T>
void VariableDescriptor::MatchFinal(T *y,typename enable_if<isSameOrBaseOf(StreamI,T) && !isSame(StreamString,T), T>::type *x){
	FinaliseCode(StreamIType(sizeof(T)));
}

template <class T>
void VariableDescriptor::MatchFinal(T *y,typename enable_if<isSame(StreamString,T), T>::type *x){
	FinaliseCode(StreamStringType(sizeof(T)));
}

template <class T>
void VariableDescriptor::MatchFinal(T *y,typename enable_if<isSameOrBaseOf(StructuredDataI,T), T>::type *x){
	FinaliseCode(StructuredDataIType(sizeof(T)));
}

#if 0 // Object knows how to generate AnyType
template <class T>
void VariableDescriptor::MatchFinal(T *y,typename enable_if<isSameOrBaseOf(Object,T), T>::type *x){
	FinaliseCode(ObjectType(sizeof(T)));
}
#endif


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

//void VariableDescriptor::Match(void * * p) {
//	FinaliseCode(PointerType);
//}

void VariableDescriptor::Match(void * p) {
	FinaliseCode(VoidType);
}

void VariableDescriptor::Match(StructuredDataI *s){
	FinaliseCode(StructuredDataIType(0));
}


template<typename baseType, uint8 bitOffset>
void VariableDescriptor::Match(BitBoolean<baseType, bitOffset> * bitBool) {
	FinaliseCode( BitSetBoolean(baseType,bitOffset));
}

template<typename baseType, uint8 bitSize, uint8 bitOffset>
void VariableDescriptor::Match(BitRange<baseType, bitSize, bitOffset> * bitRange) {
	FinaliseCode((TypeCharacteristics<baseType>::IsSigned()) ? SignedBitSet(baseType,bitSize,bitOffset) : UnsignedBitSet(baseType,bitSize,bitOffset));
}

template<typename baseType, uint8 bitSize>
void VariableDescriptor::Match(FractionalInteger<baseType, bitSize> * fractionalInt) {
	FinaliseCode((TypeCharacteristics<baseType>::IsSigned()) ? SignedBitSet(baseType,bitSize,0) : UnsignedBitSet(baseType,bitSize,0));
}

void VariableDescriptor::FinaliseCode(TypeDescriptor td){
	bool isConst = typeDescriptor.DataIsConstant();
	typeDescriptor = td;
	if (isConst) typeDescriptor.SetDataConstant(true);
}

const char8 *VariableDescriptor::GetModifiers() const{
	return modifiers.GetList();
}

// TODO use VariableDescriptorLib::Type2TypeDescriptor()
inline TypeDescriptor VariableDescriptor::GetSummaryTypeDescriptor() const{
    TypeDescriptor td =InvalidType(0);
    uint32 nOfDimensions  = 0;
    GetVariableDimensions(NULL,td,nOfDimensions,NULL);
    return td;
}

inline TypeDescriptor VariableDescriptor::GetFinalTypeDescriptor() const{
    return typeDescriptor;
}

ErrorManagement::ErrorType
VariableDescriptor::ToString(DynamicCString &string,bool rawFormat) const{
	CStringTool cst = string();
	return ToString(cst,rawFormat);
}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}
#endif /* L2OBJECTS_VARIABLEDESCRIPTOR_H_ */
	
