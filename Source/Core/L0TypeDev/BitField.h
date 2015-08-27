/**
 * @file BitField.h
 * @brief Header file for class BitField
 * @date Aug 26, 2015
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

 * @details This header file contains the declaration of the class BitField
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BIT_FIELD_H_
#define BIT_FIELD_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "TypeDescriptor.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace TypeDefinition {

template <typename T>
class TypeCharacteristics{
public:

static inline bool IsSigned(){
    return ((static_cast<T>(-1))<0);
}

static inline const T MaxValue() {
    if (IsSigned()){
        T temp = 1 << (sizeof (T)*8-1);
        return ~temp;
    } else {
        T temp = 0;
        return ~temp;
    }
}

static inline const T MinValue() {
    if (IsSigned()){
        T temp = 1 << (sizeof (T)*8-1);
        return temp;
    } else {
        T temp = 0;
        return temp;
    }
}
};

template <typename baseType, int bitSize>
class FractionalTypeCharacteristics{
public:

static inline bool IsSigned(){
    return ((static_cast<baseType>(-1))<0);
}

static inline const baseType MaxValue() {
    baseType temp;
    if (TypeCharacteristics<baseType>::IsSigned()){
        temp = (0x1 << (bitSize-1))-1;

    } else {
        temp = static_cast<baseType>(-1) >> ((sizeof(baseType)*8)-bitSize);
    }
    return temp;
}
static inline const baseType MinValue() {
    baseType temp;
    if (TypeCharacteristics<baseType>::IsSigned()){
        temp = (-1 << (bitSize-1));
    } else {
        temp = 0;
    }

    return temp;
}
};


template <typename baseType, int bitSize>
class FractionalInteger{
    baseType value;

    static const bool isSigned = ((static_cast<baseType>(-1))<0);

    static const baseType minValue = (isSigned?(-1  << (bitSize-1))   :0);
    static const baseType maxValue = (isSigned?((0x1 << (bitSize-1))-1):(static_cast<baseType>(-1) >> ((sizeof(baseType)*8)-bitSize)) );

public:


    static inline int BitSize(){ return bitSize; }

    FractionalInteger(){
        value = 0;
    }

    /**
     *  NON overlapping
     *  Saturating fractional unsigned integer
     */
    template <typename inputType>FractionalInteger(inputType input){
        baseType temporaryValue = 0;
        if (input >= maxValue) {
            value = maxValue;
            // ERROR LOGGING
        }
        else {
            if (input <= minValue) {
                value = minValue;
                // ERROR LOGGING
            } else {
                value = input;
            }
        }
    }

    inline operator baseType() const {
        return value;
    }

    inline operator AnyType() const {
        BasicType bt = UnsignedInteger;
        if (TypeCharacteristics<baseType>::IsSigned()){
            bt = SignedInteger;
        }
        const TypeDescriptor td = { false, false, { { bt, bitSize } } };
        return AnyType (td, 0, this);
    }


};




template <typename T, uint8 bitSize,uint8 bitOffset>
class UnsignedBitRange{
    T value;

public:

    static const T mask  = (~0u >> ((sizeof(T)*8)-bitSize)) << bitOffset ;
    static const T notMask  = ~((~0u >> ((sizeof(T)*8)-bitSize)) << bitOffset) ;

    static const T maxValue = (0x1 << bitSize)-1;
    static const T minValue = 0;

    /**
     *
     */
    //    inline UnsignedBitRange(T x){
    //       value = (x << bitOffset) & mask;
    //    }
    template <typename T2>UnsignedBitRange(T2 x){
        T temporaryValue = 0;
        if (x >= maxValue) {
            temporaryValue = maxValue;
            // ERROR LOGGING
        }
        else {
            if (x <= minValue) {
                temporaryValue = minValue;
                // ERROR LOGGING
            } else {
                temporaryValue = x;
            }
        }
        temporaryValue <<= bitOffset;
        temporaryValue &= mask;
        value = value & notMask;
        value = value | temporaryValue;
    }

    inline operator T() const {
        T temporaryValue = value;
        temporaryValue &=mask;
        // to align sign bits
        temporaryValue <<= ((sizeof(T)*8)-bitOffset-bitSize);
        // this should sign extend
        temporaryValue >>= ((sizeof(T)*8)-bitSize);

        return temporaryValue;
    }

    inline operator AnyType() const {
        const TypeDescriptor td = { false, false, { { UnsignedInteger, bitSize } } };
        return AnyType (td, bitOffset, this);
    }

    static inline T BitSize(){ return bitSize; }

    static inline T BitOffset(){ return bitOffset; }

};


template <typename T, uint8 bitSize,uint8 bitOffset>
class SignedBitRange{
    T value;

public:

    static const T mask  = (~0u >> ((sizeof(T)*8)-bitSize)) << bitOffset ;
    static const T notMask  = ~((~0u >> ((sizeof(T)*8)-bitSize)) << bitOffset) ;

    static const T maxValue = (0x1 << (bitSize-1))-1;
    static const T minValue = ~maxValue;

    /**
     *
     */
    //    inline UnsignedBitRange(T x){
    //       value = (x << bitOffset) & mask;
    //    }
    template <typename T2>SignedBitRange(T2 x){
        T temporaryValue = 0;
        if (x >= maxValue) {
            temporaryValue = maxValue;
            // ERROR LOGGING
        }
        else
            if (x <= minValue) {
                temporaryValue = minValue;
                // ERROR LOGGING
            } else {
                temporaryValue = x;
            }
        temporaryValue <<= bitOffset;
        temporaryValue &= mask;
        value = value & notMask;
        value = value | temporaryValue;
    }

    inline operator T() const {
        T temporaryValue = value;
        temporaryValue &=mask;
        // to align sign bits
        temporaryValue <<= ((sizeof(T)*8)-bitOffset-bitSize);
        // this should sign extend
        temporaryValue >>= ((sizeof(T)*8)-bitSize);

        return temporaryValue;
    }

    inline operator AnyType() const {
        const TypeDescriptor td = { false, false, { { SignedInteger, bitSize } } };
        return AnyType (td, bitOffset, this);
    }

    static inline T BitSize(){ return bitSize; }

    static inline T BitOffset(){ return bitOffset; }

};



}

typedef TypeDefinition::FractionalInteger<uint8  ,1>  uint1;
typedef TypeDefinition::FractionalInteger<uint8  ,2>  uint2;
typedef TypeDefinition::FractionalInteger<uint8  ,3>  uint3;
typedef TypeDefinition::FractionalInteger<uint8  ,4>  uint4;
typedef TypeDefinition::FractionalInteger<uint8  ,5>  uint5;
typedef TypeDefinition::FractionalInteger<uint8  ,6>  uint6;
typedef TypeDefinition::FractionalInteger<uint8  ,7>  uint7;
typedef TypeDefinition::FractionalInteger<uint16, 9>  uint9;
typedef TypeDefinition::FractionalInteger<uint16,10>  uint10;
typedef TypeDefinition::FractionalInteger<uint16,11>  uint11;
typedef TypeDefinition::FractionalInteger<uint16,12>  uint12;
typedef TypeDefinition::FractionalInteger<uint16,13>  uint13;
typedef TypeDefinition::FractionalInteger<uint16,14>  uint14;
typedef TypeDefinition::FractionalInteger<uint16,15>  uint15;
typedef TypeDefinition::FractionalInteger<uint32,17>  uint17;
typedef TypeDefinition::FractionalInteger<uint32,18>  uint18;
typedef TypeDefinition::FractionalInteger<uint32,19>  uint19;
typedef TypeDefinition::FractionalInteger<uint32,20>  uint20;
typedef TypeDefinition::FractionalInteger<uint32,21>  uint21;
typedef TypeDefinition::FractionalInteger<uint32,22>  uint22;
typedef TypeDefinition::FractionalInteger<uint32,23>  uint23;
typedef TypeDefinition::FractionalInteger<uint32,24>  uint24;
typedef TypeDefinition::FractionalInteger<uint32,25>  uint25;
typedef TypeDefinition::FractionalInteger<uint32,26>  uint26;
typedef TypeDefinition::FractionalInteger<uint32,27>  uint27;
typedef TypeDefinition::FractionalInteger<uint32,28>  uint28;
typedef TypeDefinition::FractionalInteger<uint32,29>  uint29;
typedef TypeDefinition::FractionalInteger<uint32,30>  uint30;
typedef TypeDefinition::FractionalInteger<uint32,31>  uint31;

typedef TypeDefinition::FractionalInteger<int8  ,2>  int2;
typedef TypeDefinition::FractionalInteger<int8  ,3>  int3;
typedef TypeDefinition::FractionalInteger<int8  ,4>  int4;
typedef TypeDefinition::FractionalInteger<int8  ,5>  int5;
typedef TypeDefinition::FractionalInteger<int8  ,6>  int6;
typedef TypeDefinition::FractionalInteger<int8  ,7>  int7;
typedef TypeDefinition::FractionalInteger<int16, 9>  int9;
typedef TypeDefinition::FractionalInteger<int16,10>  int10;
typedef TypeDefinition::FractionalInteger<int16,11>  int11;
typedef TypeDefinition::FractionalInteger<int16,12>  int12;
typedef TypeDefinition::FractionalInteger<int16,13>  int13;
typedef TypeDefinition::FractionalInteger<int16,14>  int14;
typedef TypeDefinition::FractionalInteger<int16,15>  int15;
typedef TypeDefinition::FractionalInteger<int32,17>  int17;
typedef TypeDefinition::FractionalInteger<int32,18>  int18;
typedef TypeDefinition::FractionalInteger<int32,19>  int19;
typedef TypeDefinition::FractionalInteger<int32,20>  int20;
typedef TypeDefinition::FractionalInteger<int32,21>  int21;
typedef TypeDefinition::FractionalInteger<int32,22>  int22;
typedef TypeDefinition::FractionalInteger<int32,23>  int23;
typedef TypeDefinition::FractionalInteger<int32,24>  int24;
typedef TypeDefinition::FractionalInteger<int32,25>  int25;
typedef TypeDefinition::FractionalInteger<int32,26>  int26;
typedef TypeDefinition::FractionalInteger<int32,27>  int27;
typedef TypeDefinition::FractionalInteger<int32,28>  int28;
typedef TypeDefinition::FractionalInteger<int32,29>  int29;
typedef TypeDefinition::FractionalInteger<int32,30>  int30;
typedef TypeDefinition::FractionalInteger<int32,31>  int31;



typedef union {
    TypeDefinition::UnsignedBitRange<uint32,4,0>  a;
    TypeDefinition::UnsignedBitRange<uint32,4,4>  b;
    TypeDefinition::UnsignedBitRange<uint32,10,8>  c;
    TypeDefinition::UnsignedBitRange<uint32,14,18>  d;

}
myBitStruct;


const myBitStruct *p;


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BITFIELD_H_ */

