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

#ifndef BITFIELD_H_
#define BITFIELD_H_

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

template <typename T, uint8 bitSize,uint8 bitShift>
class BitField{

    T value;

public:

    static const uint8 Mask  = (0xFF >> (8-bitSize)) << bitShift ;
    static const uint8 Shift = bitShift;
    static const uint8 Bitsize = bitSize;

    /**
     *
     */
    BitField(T x){
       value = (x << Shift) & Mask;
    }

    operator T() const {
        return (value >> Shift);
    }

    operator AnyType() const {
        TypeDescriptor td;
        td.isStructuredData = false;
        td.isConstant = false;

        AnyType at();

    }
};

typedef BitField<uint8  ,1,0>  uint1;
typedef BitField<uint8  ,2,0>  uint2;
typedef BitField<uint8  ,3,0>  uint3;
typedef BitField<uint8  ,4,0>  uint4;
typedef BitField<uint8  ,5,0>  uint5;
typedef BitField<uint8  ,6,0>  uint6;
typedef BitField<uint8  ,7,0>  uint7;
typedef BitField<uint16, 9,0>  uint9;
typedef BitField<uint16,10,0>  uint10;
typedef BitField<uint16,11,0>  uint11;
typedef BitField<uint16,12,0>  uint12;
typedef BitField<uint16,13,0>  uint13;
typedef BitField<uint16,14,0>  uint14;
typedef BitField<uint16,15,0>  uint15;
typedef BitField<uint32,17,0>  uint17;
typedef BitField<uint32,18,0>  uint18;
typedef BitField<uint32,19,0>  uint19;






/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L0TYPEDEV_BITFIELD_H_ */
	
