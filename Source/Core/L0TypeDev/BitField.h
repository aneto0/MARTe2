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
        if (x >= maxValue) {
            value = (value & notMask) | (maxValue << bitOffset);
// ERROR LOGGING
        }
        else
        if (x <= minValue) {
            value = (minValue << bitOffset);
// ERROR LOGGING
        } else {
            value = (x << bitOffset);
        }
    }



    inline operator T() const {
        return (value >> bitOffset);
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

    static const T mask  = (0xFF >> (8-bitSize)) << bitOffset ;

    /**
     *
     */
    inline SignedBitRange(T x){
       value = (x << bitOffset) & mask;
    }

    inline operator T() const {
        return (value >> bitOffset);
    }

    inline operator AnyType() const {
        const TypeDescriptor td = { false, false, { { SignedInteger, bitSize } } };
        return AnyType (td, bitOffset, this);
    }

    static inline T BitSize(){ return bitSize; }

    static inline T BitOffset(){ return bitOffset; }

};

}

typedef TypeDefinition::UnsignedBitRange<uint8  ,1,0>  uint1;
typedef TypeDefinition::UnsignedBitRange<uint8  ,2,0>  uint2;
typedef TypeDefinition::UnsignedBitRange<uint8  ,3,0>  uint3;
typedef TypeDefinition::UnsignedBitRange<uint8  ,4,0>  uint4;
typedef TypeDefinition::UnsignedBitRange<uint8  ,5,0>  uint5;
typedef TypeDefinition::UnsignedBitRange<uint8  ,6,0>  uint6;
typedef TypeDefinition::UnsignedBitRange<uint8  ,7,0>  uint7;
typedef TypeDefinition::UnsignedBitRange<uint16, 9,0>  uint9;
typedef TypeDefinition::UnsignedBitRange<uint16,10,0>  uint10;
typedef TypeDefinition::UnsignedBitRange<uint16,11,0>  uint11;
typedef TypeDefinition::UnsignedBitRange<uint16,12,0>  uint12;
typedef TypeDefinition::UnsignedBitRange<uint16,13,0>  uint13;
typedef TypeDefinition::UnsignedBitRange<uint16,14,0>  uint14;
typedef TypeDefinition::UnsignedBitRange<uint16,15,0>  uint15;
typedef TypeDefinition::UnsignedBitRange<uint32,17,0>  uint17;
typedef TypeDefinition::UnsignedBitRange<uint32,18,0>  uint18;
typedef TypeDefinition::UnsignedBitRange<uint32,19,0>  uint19;
typedef TypeDefinition::UnsignedBitRange<uint32,20,0>  uint20;
typedef TypeDefinition::UnsignedBitRange<uint32,21,0>  uint21;
typedef TypeDefinition::UnsignedBitRange<uint32,22,0>  uint22;
typedef TypeDefinition::UnsignedBitRange<uint32,23,0>  uint23;
typedef TypeDefinition::UnsignedBitRange<uint32,24,0>  uint24;
typedef TypeDefinition::UnsignedBitRange<uint32,25,0>  uint25;
typedef TypeDefinition::UnsignedBitRange<uint32,26,0>  uint26;
typedef TypeDefinition::UnsignedBitRange<uint32,27,0>  uint27;
typedef TypeDefinition::UnsignedBitRange<uint32,28,0>  uint28;
typedef TypeDefinition::UnsignedBitRange<uint32,29,0>  uint29;
typedef TypeDefinition::UnsignedBitRange<uint32,30,0>  uint30;
typedef TypeDefinition::UnsignedBitRange<uint32,31,0>  uint31;



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
	
