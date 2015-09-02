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
#include "AnyType.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
template<typename T>
class BitRangeTest {

public:

    template<typename T2>
    bool TestBasicTypeCastMinorSize(T2 input);

    template<typename T2>
    bool TestBasicTypeCastMajorSize(T2 input);

    bool TestCopyOperatorUnion();

    bool TestAnyTypeCast();

    bool TestBitSize();

    bool TestBitOffset();

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
bool BitRangeTest<T>::TestBasicTypeCastMinorSize(T2 input) {

    const uint8 max = sizeof(T) * 8;
    const uint8 half = max / 2;
    const uint8 inputSize = sizeof(T2) * 8;

    // the size of the bit range
    const uint8 minorSize = (inputSize < max) ? (inputSize - 1) : (half - 1);

    TypeDefinition::BitRange<T, minorSize, half> myBitRange;

    bool isInputSigned = TypeDefinition::TypeCharacteristics<T2>::IsSigned();

    // max and min values of the input
    T2 maxValue = isInputSigned ? ((((T2) 1) << (inputSize - (T2) 1)) - (T) 1) : ((T2) -1);
    T2 minValue = isInputSigned ? ~((((T2) 1) << (inputSize - (T2) 1)) - (T) 1) : 0;
    T2 zero = (T2) 0;

    bool isSigned = TypeDefinition::TypeCharacteristics<T>::IsSigned();

    // max and min values of the bit range
    const T thisMaxValue = isSigned ? ((((T) 1) << (minorSize - (T) 1)) - (T) 1) : (((T) -1) >> (sizeof(T) * 8 - minorSize));
    const T thisMinValue = isSigned ? ~((((T) 1) << (minorSize - (T) 1)) - (T) 1) : (T) 0;

    myBitRange = maxValue;

    // since the bit range has a size minor than the input normally the value should be saturated
    if (myBitRange != thisMaxValue) {
        return false;
    }
    myBitRange = minValue;
    if (myBitRange != thisMinValue) {

        //if the input is unsigned and the bit ragnge signed, the minValue passed is zero
        if ((!isInputSigned) && (isSigned)) {
            if (myBitRange != 0) {
                return false;
            }
        }
        else {
            return false;
        }
    }

    myBitRange = zero;
    if (myBitRange != zero) {

        return false;
    }

    return true;
}

template<typename T>
template<typename T2>
bool BitRangeTest<T>::TestBasicTypeCastMajorSize(T2 input) {

    const uint8 max = sizeof(T) * 8;
    const uint8 inputSize = sizeof(T2) * 8;
    const uint8 half = 0;
    const uint8 majorSize = (inputSize + 1);

    TypeDefinition::BitRange<T, majorSize, half> myBitRange;

    bool isInputSigned = TypeDefinition::TypeCharacteristics<T2>::IsSigned();
    T2 maxValue = isInputSigned ? ((((T2) 1) << (inputSize - (T2) 1)) - (T2) 1) : ((T2) -1);
    T2 minValue = isInputSigned ? ~((((T2) 1) << (inputSize - (T2) 1)) - (T2) 1) : 0;
    T2 zero = (T2) 0;

    bool isSigned = TypeDefinition::TypeCharacteristics<T>::IsSigned();


    myBitRange = maxValue;

    // since the bit range has a size greater than the input,
    // the bit range should contain the max input value
    if (myBitRange != maxValue) {

        return false;
    }

    // normally the bit range should contain the min input value
    myBitRange = minValue;
    if (myBitRange != minValue) {

        // if the bit range is unsigned and the input signed,
        // the negative value is saturated to zero in the bit range
        if ((!isSigned) && (isInputSigned)) {
            if (myBitRange != 0) {
                return false;
            }
        }
        else {
            return false;
        }
    }

    myBitRange = zero;
    if (myBitRange != zero) {

        return false;
    }

    return true;
}

template<typename T>
bool BitRangeTest<T>::TestAnyTypeCast() {
    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;
    const uint8 zero = 0;

    const uint8 size = 8;

    TypeDefinition::BitRange<T, size, half> myBitRange;
    myBitRange = 0;

    TypeDefinition::AnyType atTest = myBitRange;

    if (atTest.GetDataPointer() != (&myBitRange)) {
        return false;
    }

    TypeDefinition::TypeDescriptor tdTest = atTest.GetTypeDescriptor();


    if (tdTest.isStructuredData || tdTest.isConstant || tdTest.type != TypeDefinition::UnsignedInteger || tdTest.size != size) {
        return false;
    }

    if (atTest.GetBitAddress() != half) {
        return false;
    }

    return true;
}

template<typename T>
bool BitRangeTest<T>::TestBitSize() {

    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;
    const uint8 zero = 0;

    TypeDefinition::BitRange<T, zero, 0> myZeroBitRange;

    if (myZeroBitRange.BitSize() != zero) {
        return false;
    }

    TypeDefinition::BitRange<T, half, 0> myHalfBitRange;

    if (myHalfBitRange.BitSize() != half) {
        return false;
    }
    TypeDefinition::BitRange<T, max, 0> myMaxBitRange;

    if (myMaxBitRange.BitSize() != max) {
        return false;
    }

    return true;
}

template<typename T>
bool BitRangeTest<T>::TestBitOffset() {
    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;
    const uint8 zero = 0;

    TypeDefinition::BitRange<T, max, zero> myZeroBitRange;

    if (myZeroBitRange.BitOffset() != zero) {
        return false;
    }

    TypeDefinition::BitRange<T, half, half> myHalfBitRange;

    if (myHalfBitRange.BitOffset() != half) {
        return false;
    }
    TypeDefinition::BitRange<T, max, max> myMaxBitRange;

    if (myMaxBitRange.BitOffset() != max) {
        return false;
    }

    return true;
}

#endif /* BITRANGETEST_H_ */

