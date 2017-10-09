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
     * @brief copy constructor
     */
    VariableDescriptor(const VariableDescriptor &x );

    /**
     * @brief copy operator
     */
    VariableDescriptor &operator=(const VariableDescriptor &x );

    /**
     * @brief sets the typeDescriptor
     */
    VariableDescriptor(const TypeDescriptor &td);

    /**
     * @brief Returns the TypeDescriptor describing the most inner storage
     * @return the data TypeDescriptor.
     */
    inline const TypeDescriptor &GetFullTypeDescriptor() const;

#if 0
    /**
     * @brief Returns the TypeDescriptor
     * @return the data TypeDescriptor.
     */
    inline TypeDescriptor &AccessTypeDescriptor() ;

    /**
     * @brief Returns the TypeDescriptor describing the overall variable
     * @return the data TypeDescriptor.
     */
    bool GetTopTypeDescriptor( TypeDescriptor &td, uint32 depth) const;
    /**
     * @brief removes one layer of modifiers from the top
     * @return true if operation succeeded, false if no modifiers
     * */
    bool RemoveModifiersLayer(char8 &modifier,uint64 &size){
        return GetModifiersLayer(modifier,size,true);
    }
    /**
     * @brief read and possibly removes top layer of modifiers
     * @return true if operation succeeded, false if no modifiers
     */
    bool GetModifiersLayer(char8 &modifier,uint64 &size,bool remove=false);
#endif

    /**
     * @brief checks if it has modifiers
     * @return true if there are modifiers
     * */
    inline bool HasModifiers() const;

#if 0
    /**
     * @brief adds one layer of modifiers to the top
     * @return true if operation succeeded                       */
    bool InsertModifiersLayer(char8 modifier,uint64 size);
#endif
    void AddModifiersLayer(char8 modifier, uint64 size);
    void AddModifiersLayerConst(char8 modifier, uint64 size);


    /**
     * @brief reads one layer of modifiers
     * @return true if operation succeeded, false if no modifiers
     */
    bool BrowseModifiersLayer(char8 &modifier,uint64 &size,uint32 layerNo) const;

    /**
     * @brief returns size of all the memory addressed by this variable.
     * @param[in] pointer, the pointer to the variable
     * @param[in] maxDepth  determine the number of indirected memory to include.
     * limit 0 means just the top layer
     * Note that pointers to type only address one element
     * Vector, Matrix, ZeroTermArray are all supported
     * @param[out] dataSize is the full size of the memory necessary to store this var including redirections
     * @param[out] storageSize is the memory used in intermediate redirection layers
     * @return true if all ok
     */
    ErrorManagement::ErrorType GetSize(const uint8 *pointer,uint64 &dataSize, uint64 *storageSize=NULL,uint8 maxDepth=10) const;

    /**
     * @brief removes one indirection layer and update variable pointer
     * @param[in,out] pointer, the pointer to the variable
     * @param[in] index the offset
     * @param[in] modifierString, used internally to perform recursion
     * @return true if all ok or the error
     */
    ErrorManagement::ErrorType Redirect(const uint8 *&pointer,uint32 index,CCString modifierString=NULL) ;

    /**
     * @brief copies the variable layer by layer. The copied layer is implemented in contiguous memory
     * @param[in] sourcePtr, the pointer to the variable to be copied
     * @param[in,out] destFreePtr, pointer to the memory area to copy the variable to. Returns the pointer to the unused area
     * @param[in,out] destFreeSize, length of the memory area to copy to. Returns the unused area size
     * @param[out] destVd, the variable descriptor of the used area, must be empty to start!. Note that all varieties of ZeroTermarrays become ZeroTermArray<const T>
     * @param[in] maxDepth, the max number of pointer redirection to include in the copy
     * @param[in] modifierString, at start points at full modifiers.After each recursion it is progressively consumed.
     * @param[in] dereferenceCompound. modifiers as Z,S etc are in fact a combination of PZ, PS etc... as the individual is not supported
     * dereferenceCompound = true implies that the Z is interpreted as Z while if it is false as P
     * @param[in] destPtr, pointer to the area reserved for next layer. use if provided or take from destFreePtr
     * @return true if all ok or the error
     */
    ErrorManagement::ErrorType Copy(
    		const uint8 *sourcePtr,
    		uint8 *&destFreePtr,
			uint64 &destFreeSize,
			VariableDescriptor &destVd,
			uint8 maxDepth=100,
			CCString modifierString = modifiers,
			bool dereferenceCompound = false,
			uint8 *destPtr=NULL) const;


#if 0
    /**
     * TODO
     */
    CCString GetModifierString() const{
    	return modifiers.GetList();
    }
#endif
    /**
     * @brief Converts type descriptor to C/c++ equivalent
     *
     */
    bool ToString(DynamicCString &string) const;

private:

    /**
     * @brief calculate size of a full layer - n of array alements * elementsize
     * @param[in] modifierString, the string of variable modifiers
     * @return the full size of the memory necessary to store this layer
     */
    uint64 FullLayerSize(CCString modifierString,const uint8 *pointer) const;

    /**
     * @brief calculate size of a full layer - n of array alements * elementsize
     * @param[in,out] modifierString, the string of variable modifiers
     * @param[out] numberOfElements is the number of elements in this layer
     * @param[out] elementSize is the size of each element
     * @param[out] arrayStringSize number of characters in modifierString consumed in arrays 'a' layers
     * @param[out] numberOfTermElements number of elements of size elementSize used as terminator in a ZTA
     * @param[out] modifier is the last code that terminated this scan (0 for the end of modifiers or one of PVNZSDpvnzsd
     */
    ErrorManagement::ErrorType FullLayerInfo(CCString &modifierString,const uint8 *pointer,
    		uint64 &numberOfElements,uint32 &elementSize,
			uint32 &arrayStringSize,uint32 &numberOfTermElements,
			char8 &modifier) const;

    /**
     * @brief returns size of all the memory addressed by this variable.
     * @param[in] modifierString, the string of variable modifiers
     * @param[in] pointer, the pointer to the variable
     * @param[out] dataSize is the full size of the memory necessary to store this var including redirections
     * @param[out] storageSize is the memory used in intermediate redirection layers
     * @param[in] maxDepth  determine the number of indirected memory to include.
     * -1 means no limit 0 means just the top layer
     * Note that pointers to type only address one element
     * Vector, Matrix, ZeroTermArray are all supported
     * @param[in] layerMultiplier >1 it is an array[layerMultiplier] of the type described by modifierString
     * @return true if all ok
     */
    ErrorManagement::ErrorType GetDeepSize(CCString modifierString, const uint8 *pointer,
    		uint64 &dataSize, uint64 &storageSize,uint8 maxDepth=100,uint32 layerMultiplier=1) const;
#if 0
    /**
     * @brief removes one indirection layer without redirection. Does not reallocate the modifierString
     * @param[in out] modifierString, the variable descriptor modifier string
     * @param[out]   nOfElements elements in the next layer (beyond indirection or not)
     * @param[out]   size the size of the next elements (beyond indirection or not)
     * @return true if all ok or the error
     */
    void ExamineLayer(CCString &modifierString,uint64 &nOfElements,uint64 &size,const uint8 *pointer) const;

    /**
     * @brief removes one indirection layer and update variable pointer. Does not reallocate the modifierString
     * @param[in out] modifierString, the variable descriptor modifier string
     * @param[in out] pointer, the pointer to the variable updated
     * @param[out]   nOfElements elements in the next layer (beyond indirection or not)
     * @param[out]   storageSize size to store the pointers (etc) 0 if no indirection
     * @param[out]   size the size of the next elements (beyond indirection or not)
     * @return true if all ok or the error
     */
    ErrorManagement::ErrorType ExamineAndRedirect(CCString &modifierString,const uint8 *&pointer,uint64 &nOfElements,
    			uint64 &storageSize,uint64 &size) const;
#endif

#if 0
    /**
     * TODO
     *  Internal use
     *  To encode an array of specific type and with potential know size size1
     */
    void AddArrayCode(BasicArrayType bat, uint32 size1);
#endif
    /**
     * TODO
     *  Internal use
     *  To encode the fact that what is coming next is a constant
     */
    inline void AddConstantCode();

    /**
     * TODO
     * Internal use
     * To encode the actual data type.
     * It tries to incorporate the const and Array code
     */
    void FinaliseCode(TypeDescriptor td);
#if 0
    /**
     * TODO
     * Internal use
     * To write the encoded modifiers to the modifiers stack
     * outputs the modifiers stored in the typeDescriptor into the modifier string
     */
    void MoveCodeToModifiers();
#endif
    /**
     *  @brief a zero terminated sequence of tokens.
     *  @full each token can be a character or a sequence of characters and bytes
    */
    DynamicCString      modifiers;

    /**
     * The type of the (final after redirections) variable
    */
    TypeDescriptor     typeDescriptor;

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
     * @brief Matches a Stream.
     * @param[in] s the Stream
     * @post
     */
    inline void Match(StreamI *s);
#if 0
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
#endif
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
//	AddArrayCode(Array1D,0);
	AddModifiersLayerConst('V', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T>
void VariableDescriptor::Match(Matrix<T> * mat) {
//	AddArrayCode(Array2D,0);
	AddModifiersLayerConst('M', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T>
void VariableDescriptor::Match(ZeroTerminatedArray<T> * vec){
//	AddArrayCode(ZeroTermArray,0);
	AddModifiersLayerConst('P', 0);
	AddModifiersLayerConst('Z', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T >
void VariableDescriptor::Match(DynamicZeroTerminatedArray<T,16u> * vec){
//	AddArrayCode(DynamicZeroTermArray,0);
	AddModifiersLayerConst('P', 0);
	AddModifiersLayerConst('D', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T, uint32 sz >
void VariableDescriptor::Match(StaticZeroTerminatedArray<T,sz> * vec){
//	AddArrayCode(StaticZeroTermArray,sz);
	AddModifiersLayerConst('P', 0);
	AddModifiersLayerConst('S', sz);

    T *pp = NULL;
    Match(pp);
}

template <class T,unsigned int n>
inline void VariableDescriptor::Match(T (*x) [n]){
//	AddArrayCode(SizedCArray,n);
	AddModifiersLayerConst('A', n);

    T *pp = NULL;
    Match(pp);
}

template <class T,unsigned int n>
inline void VariableDescriptor::Match( const T (*x) [n]){
//	AddArrayCode(SizedCArray,n);
	AddModifiersLayerConst('A', n);

    const T *pp = NULL;
    Match(pp);
}


template <class T>
inline void VariableDescriptor::Match(T ** x){
	AddModifiersLayerConst('P', 0);
//	AddArrayCode(PointerArray,0);

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
//	AddArrayCode(PointerArray,0);
	AddModifiersLayerConst('P', 0);

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
void VariableDescriptor::Match(StreamI *s){
	FinaliseCode(StreamType);
}
#if 0
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

const TypeDescriptor &VariableDescriptor::GetFullTypeDescriptor() const {
    return typeDescriptor;
}
//TypeDescriptor &VariableDescriptor::AccessTypeDescriptor() {
//	return typeDescriptor;
//}

void VariableDescriptor::AddConstantCode(){
	typeDescriptor.dataIsConstant = true;
}

bool VariableDescriptor::HasModifiers() const{
	return (modifiers.GetSize() > 0);
}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}
#endif /* L2OBJECTS_VARIABLEDESCRIPTOR_H_ */
	
