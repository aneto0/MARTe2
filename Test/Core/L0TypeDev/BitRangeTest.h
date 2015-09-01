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
    bool TestCopyOperatorMinorSize(T2 input);

    template<typename T2>
    bool TestCopyOperatorMajorSize(T2 input);

    bool TestCopyOperatorUnion();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

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

template<typename T>
template<typename T2>
bool BitRangeTest<T>::TestCopyOperatorMinorSize(T2 input) {

    const uint8 max = sizeof(T) * 8;
    const uint8 half = max / 2;
    const uint8 inputSize = sizeof(T2) * 8;

    const uint8 minorSize = (inputSize < max) ? (inputSize - 1) : (half - 1);

    TypeDefinition::BitRange<T, minorSize, half> myBitRange;

    bool isInputSigned = TypeDefinition::TypeCharacteristics<T2>::IsSigned();
    T2 maxValue = isInputSigned ? ((((T) 1) << (inputSize - (T) 1)) - (T) 1) : ~((T2) 0);
    T2 minValue = isInputSigned ? ~((((T) 1) << (inputSize - (T) 1)) - (T) 1) : 0;
    T2 zero = (T2) 0;

    bool isSigned = TypeDefinition::TypeCharacteristics<T>::IsSigned();

    const T thisMaxValue = isSigned ? ((((T) 1) << (minorSize - (T) 1)) - (T) 1) : (((T) -1) >> (sizeof(T) * 8 - minorSize));
    const T thisMinValue = isSigned ? ~((((T) 1) << (minorSize - (T) 1)) - (T) 1) : (T) 0;

    myBitRange = maxValue;

    if (myBitRange != thisMaxValue) {
        printf("\n1 %x %x %d\n", (T) myBitRange, thisMaxValue, minorSize);
        return false;
    }
    myBitRange = minValue;
    if (myBitRange != thisMinValue) {
        if ((!isInputSigned) && (isSigned)) {
            if (myBitRange != 0) {
                return false;
            }
        }
        else {
            printf("\n2 %x %x %x\n", (T) myBitRange, thisMinValue, minValue);
            return false;
        }
    }

    myBitRange = zero;
    if (myBitRange != zero) {
        printf("\n3 %x %x\n", (T) myBitRange, zero);

        return false;
    }

    return true;
}

template<typename T>
template<typename T2>
bool BitRangeTest<T>::TestCopyOperatorMajorSize(T2 input) {

    const uint8 max = sizeof(T) * 8;
    const uint8 inputSize = sizeof(T2) * 8;
    const uint8 half = 0;//max / 2 - 2;
    const uint8 majorSize = (inputSize < max) ? (inputSize + 1) : (inputSize );

    TypeDefinition::BitRange<T, majorSize, half> myBitRange;

    bool isInputSigned = TypeDefinition::TypeCharacteristics<T2>::IsSigned();
    T2 maxValue = isInputSigned ? ((((T) 1) << (inputSize - (T) 1)) - (T) 1) : ~((T2) 0);
    T2 minValue = isInputSigned ? ~((((T) 1) << (inputSize - (T) 1)) - (T) 1) : 0;
    T2 zero = (T2) 0;

    bool isSigned = TypeDefinition::TypeCharacteristics<T>::IsSigned();

    const T thisMaxValue = isSigned ? ((((T) 1) << (majorSize - (T) 1)) - (T) 1) : (((T) -1) >> (sizeof(T) * 8 - majorSize));
    const T thisMinValue = isSigned ? ~((((T) 1) << (majorSize - (T) 1)) - (T) 1) : 0;

    myBitRange = maxValue;

    if (myBitRange != maxValue) {
        printf("\n1 %x %x\n", (T) myBitRange, maxValue);

        return false;
    }
    myBitRange = minValue;
    if (myBitRange != minValue) {

        if ((!isSigned) && (isInputSigned)) {
            if (myBitRange != 0) {
                return false;
            }
        }
        else {
            printf("\n2 %x %x\n", (T) myBitRange, minValue);

            return false;
        }
    }

    myBitRange = zero;
    if (myBitRange != zero) {
        printf("\n3 %x %x\n", myBitRange, zero);

        return false;
    }

    return true;
}
#if 0
template<typename T>
template<typename T2>
bool BitRangeTest<T>::TestCopyOperatorMinorSize(T2 input) {

// test with 3 different shifts
    const uint8 zero = 0;
    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;

    const uint8 inputSize = sizeof(T2) * 8 - 1;

    bool isInputSigned = TypeDefinition::TypeCharacteristics<T2>::IsSigned();
    const T2 maxValue = isInputSigned ? ((((T) 1) << (inputSize - (T) 1)) - (T) 1) : ~((T2) 0);
    const T2 minValue = isInputSigned ? ~((((T) 1) << (inputSize - (T) 1)) - (T) 1) : 0;

//the size is minor
    const uint8 minorSize = sizeof(T2) * 8 - 1;
    bool isSigned = TypeDefinition::TypeCharacteristics<T>::IsSigned();

    const T thisMaxValue = isSigned ? ((((T) 1) << (minorSize - (T) 1)) - (T) 1) : (~((T) 0) >> (sizeof(T) * 8 - minorSize));
    const T thisMinValue = isSigned ? ~((((T) 1) << (minorSize - (T) 1)) - (T) 1) : 0;

    TypeDefinition::BitRange<T, minorSize, zero> myZeroBitRange;

    uint8 shift = zero;
    T clipMask = ((shift + minorSize) > (sizeof(T) * 8)) ? ((((T) 1) << (sizeof(T) * 8 - shift)) - 1) : ~((T) 0);

    /*    int8 x = -1;

     uint32 x32= (uint32)x;

     printf("\nahaha %d", x32);

     myZeroBitRange = (unsigned char) 128;
     if (myZeroBitRange != 128) {
     return false;
     }
     */
    for (T2 i = minValue; i < maxValue; i++) {

        myZeroBitRange = i;

        if (i > thisMaxValue) {
            if (myZeroBitRange != (thisMaxValue & clipMask)) {
                printf("\n1\n");
                return false;
            }
            else {
                continue;
            }
        }

        if (i < thisMinValue) {
            if (myZeroBitRange != (thisMinValue & clipMask)) {
                return false;
                printf("\n2\n");
            }
            else {
                continue;
            }
        }

        if (myZeroBitRange != (i & clipMask)) {
            printf("\n3\n");
            return false;
        }
        if (i < 10)
        printf("\n%x %d\n", (T) myZeroBitRange, i);
    }

    shift = half;
    clipMask = ((shift + minorSize) > (sizeof(T) * 8)) ? (((T) 1) << (sizeof(T) * 8 - shift) - 1) : ~((T) 0);

    TypeDefinition::BitRange<T, minorSize, half> myHalfBitRange;

    for (T2 i = minValue; i < maxValue; i++) {
        myHalfBitRange = i;
        if (i > thisMaxValue) {
            if (myHalfBitRange != (thisMaxValue & clipMask)) {
                return false;
            }
            else {
                continue;
            }
        }

        if (i < thisMinValue) {
            if (myHalfBitRange != (thisMinValue & clipMask)) {
                return false;
            }
            else {
                continue;
            }
        }

        if (myHalfBitRange != (i & clipMask)) {
            return false;
        }
    }

    shift = max;
    clipMask = ((shift + minorSize) > (sizeof(T) * 8)) ? (((T) 1) << (sizeof(T) * 8 - shift) - 1) : ~((T) 0);

    TypeDefinition::BitRange<T, minorSize, max> myMaxBitRange;

    for (T2 i = minValue; i < maxValue; i++) {

        myMaxBitRange = i;
        if (i > thisMaxValue) {
            if (myMaxBitRange != (thisMaxValue & clipMask)) {
                return false;
            }
            else {
                continue;
            }
        }

        if (i < thisMinValue) {
            if (myMaxBitRange != (thisMinValue & clipMask)) {
                return false;
            }
            else {
                continue;
            }
        }

        if (myMaxBitRange != (i & clipMask)) {
            return false;
        }
    }

    return true;

}

template<typename T>
template<typename T2>
bool BitRangeTest<T>::TestCopyOperatorMajorSize(T2 input) {

// test with 3 different shifts
    const uint8 zero = 0;
    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;

    const uint8 inputSize = sizeof(T2) * 8 - 1;

    bool isInputSigned = TypeDefinition::TypeCharacteristics<T2>::IsSigned();
    const T2 maxValue = isInputSigned ? (~(((T2) 1) << inputSize)) : ~((T2) 0);
    const T2 minValue = isInputSigned ? (((T2) 1) << inputSize) : 0;

//the size is minor
    const uint8 majorSize = sizeof(T2) * 8 + 1;
    bool isSigned = TypeDefinition::TypeCharacteristics<T>::IsSigned();

    const T thisMaxValue = isSigned ? ((((T) 1) << (majorSize - (T) 1)) - (T) 1) : (~((T) 0) >> (sizeof(T) * 8 - majorSize));
    const T thisMinValue = isSigned ? ~((((T) 1) << (majorSize - (T) 1)) - (T) 1) : 0;

    TypeDefinition::BitRange<T, majorSize, zero> myZeroBitRange;

    uint8 shift = zero;
    T clipMask = ((shift + majorSize) > (sizeof(T) * 8)) ? ((((T) 1) << (sizeof(T) * 8 - shift)) - 1) : ~((T) 0);

    for (T2 i = minValue; i < maxValue; i++) {

        myZeroBitRange = i;

        if ((T) i > (T) thisMaxValue) {
            if (myZeroBitRange != (thisMaxValue & clipMask)) {
                return false;
            }
            else {
                continue;
            }
        }

        if ((T) i < (T) thisMinValue) {
            if (myZeroBitRange != (thisMinValue & clipMask)) {
                return false;
            }
            else {
                continue;
            }
        }

        if ((T) myZeroBitRange != (T) (i & clipMask)) {
            return false;
        }
    }

    shift = half;
    clipMask = ((shift + majorSize) > (sizeof(T) * 8)) ? (((T) 1) << (sizeof(T) * 8 - shift) - 1) : ~((T) 0);

    TypeDefinition::BitRange<T, majorSize, half> myHalfBitRange;

    for (T2 i = minValue; i < maxValue; i++) {
        myHalfBitRange = i;
        if ((T) i > (T) thisMaxValue) {
            if (myHalfBitRange != (thisMaxValue & clipMask)) {
                return false;
            }
            else {
                continue;
            }
        }

        if ((T) i < (T) thisMinValue) {
            if (myHalfBitRange != (thisMinValue & clipMask)) {
                return false;
            }
            else {
                continue;
            }
        }

        if (myHalfBitRange != (i & clipMask)) {
            return false;
        }
    }

    shift = max;
    clipMask = ((shift + majorSize) > (sizeof(T) * 8)) ? (((T) 1) << (sizeof(T) * 8 - shift) - 1) : ~((T) 0);

    TypeDefinition::BitRange<T, majorSize, max> myMaxBitRange;

    for (T2 i = minValue; i < maxValue; i++) {

        myMaxBitRange = i;
        if ((T) i > (T) thisMaxValue) {
            if (myMaxBitRange != (thisMaxValue & clipMask)) {
                return false;
            }
            else {
                continue;
            }
        }

        if ((T) i < (T) thisMinValue) {
            if (myMaxBitRange != (thisMinValue & clipMask)) {
                return false;
            }
            else {
                continue;
            }
        }

        if (myMaxBitRange != (i & clipMask)) {
            return false;
        }
    }

    return true;

}

#endif

#endif /* BITRANGETEST_H_ */

