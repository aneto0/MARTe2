/**
 * @file BitRangeTest.h
 * @brief Header file for class BitRangeTest
 * @date 31/08/2015
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

 * @details This header file contains the declaration of the class BitRangeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BITRANGETEST_H_
#define BITRANGETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BitRange.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
template<typename T>
class BitRangeTest {

public:

    template<typename T2>
    bool TestCopyOperatorMinorSizeUnSigned(T thisType,
                                           T2 input);

    bool TestCopyOperatorUnion();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
template<typename T2>
bool BitRangeTest<T>::TestCopyOperatorMinorSizeUnSigned(T thisType,
                                                        T2 input) {

    // test with 3 different shifts
    const uint8 zero = 0;
    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;

    const uint8 inputSize = sizeof(T2) * 8 - 1;

    bool isSigned = TypeDefinition::TypeCharacteristics<T>::IsSigned();
    const T2 maxValue = isSigned ? (~(((T2) 1) << inputSize)) : ~((T2) 0);
    const T2 minValue = isSigned ? (((T2) 1) << inputSize) : 0;

    const T thisMaxValue = isSigned ? (~(((T) 1) << max)) : ~((T) 0);
    const T thisMinValue = isSigned ? (((T) 1) << max) : 0;

    //the size is minor
    const uint8 minorSize = sizeof(T2) * 8 - 1;

    TypeDefinition::BitRange<T, minorSize, zero> myZeroBitRange;

    for (T2 i = minValue; i < maxValue; i++) {

        myZeroBitRange = i;
        if (i > thisMaxValue) {
            if (myZeroBitRange != thisMaxValue) {
                return false;
            }
            else {
                continue;
            }
        }

        if (i < thisMinValue) {
            if (myZeroBitRange != thisMinValue) {
                return false;
            }
            else
                continue;
        }

        if (myZeroBitRange != i) {
            return false;
        }
    }

    TypeDefinition::BitRange<T, minorSize, half> myHalfBitRange;

    for (T2 i = minValue; i < maxValue; i++) {

        myHalfBitRange = i;
        if (i > thisMaxValue) {
            if (myHalfBitRange != thisMaxValue) {
                return false;
            }
            else {
                continue;
            }
        }

        if (i < thisMinValue) {
            if (myHalfBitRange != thisMinValue) {
                return false;
            }
            else {
                continue;
            }
        }

        if (myHalfBitRange != i) {
            return false;
        }
    }

    TypeDefinition::BitRange<T, minorSize, max> myMaxBitRange;

    for (T2 i = minValue; i < maxValue; i++) {

        myMaxBitRange = i;
        if (myMaxBitRange != 1) {
            return false;
        }
    }

    return true;

}

union BitRangeUnionExample {
    TypeDefinition::BitRange<uint64, 8, 0> first;
    TypeDefinition::BitRange<uint64, 8, 8> second;
    TypeDefinition::BitRange<uint64, 16, 16> third;
    TypeDefinition::BitRange<uint64, 32, 32> fourth;

    uint64 alternative;

};

template<typename T>
bool BitRangeTest<T>::TestCopyOperatorUnion() {

    BitRangeUnionExample testUnion;
    testUnion.alternative = 0x400030201;

    if (testUnion.first != 1) {
        return false;
    }

    if (testUnion.second != 2) {
        return false;
    }

    if (testUnion.third != 3) {
        return false;
    }
    if (testUnion.fourth != 4) {
        return false;
    }

    return sizeof(BitRangeUnionExample) == 8;
}

#endif /* BITRANGETEST_H_ */

