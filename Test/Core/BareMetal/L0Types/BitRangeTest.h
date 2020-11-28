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

using namespace MARTe;

/**
 * @brief Tests all the BitRange functions.
 * @tparam T TODO Define T
 */
template<typename T>
class BitRangeTest {

public:

    /**
     * @brief Tests the cast to the base type using for BitRange with a bit size
     * minor than the input size.
     * @tparam T2 TODO Define T2
     * @param[in] input specifies the input type.
     * @return true if the maximum input value is saturated to the maximum BitRange value and
     * if the minimum input value is saturated to the minimum BitRange value. If this type is
     * signed and the input is unsigned returns true if BitRange value is set to zero. False otherwise
     */
    template<typename T2>
    bool TestBasicTypeCastMinorSize(T2 input);

    /**
     * @brief Tests the cast to the base type using for BitRange a bit size
     * greater than the input size.
     * @tparam T2 TODO Define T2
     * @param[in] input specifies the input type.
     * @return true if the BitRange value is equal to the maximum input value and
     * if the BitRange value is equal to the minimum input value is saturated to the minimum. If this type is
     * unsigned and the input is negative returns true if BitRange is set to zero. False otherwise.
     */
    template<typename T2>
    bool TestBasicTypeCastMajorSize(T2 input);

    /**
     * @brief Tests if the = operator of BitRange does not overlap memory outside its bit range in a union.
     * @return true if the = operator changes only the specified bit range in the union memory. False otherwise.
     */
    bool TestCopyOperatorUnion();

    /**
     * @brief Tests the cast to AnyType.
     * @return true if the AnyType attributes are initialized correctly. False otherwise.
     */
    bool TestAnyTypeCast();

    /**
     * @brief Tests if the returned bit size is correct.
     * @return true if the bit size returned is equal to the specified bit size in template initialization.
     */
    bool TestNumberOfBits();

    /**
     * @brief Tests if the returned bit offset is correct.
     * @return true if the bit offset returned is equal to the specified bit size in template initialization.
     */
    bool TestBitOffset();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

union BitRangeUnionExample {
    BitRange<uint64, 8, 0> first;
    BitRange<uint64, 8, 8> second;
    BitRange<uint64, 16, 16> third;
    BitRange<uint64, 32, 32> fourth;

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

    union {
        BitRange<T, minorSize, half> myBitRange;
        T initBitRange;
    };
    initBitRange = 0;

    bool isInputSigned = TypeCharacteristics<T2>::IsSigned();

    // max and min values of the input
    T2 maxValue = 0;
    T2 minValue = 0;
    if (isInputSigned) {
        maxValue = (((T2) 1) << (inputSize - (T2) 1));
        maxValue -= (T2) 1;
        minValue = (((T2) 1) << (inputSize - (T2) 1));
        minValue = ~(minValue - (T2) 1);
    }
    else {
        maxValue = (T2) -1;
        minValue = 0;
    }

    T2 zero = (T2) 0;

    bool isSigned = TypeCharacteristics<T>::IsSigned();

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

        //if the input is unsigned and the bit range signed, the minValue passed is zero
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
    if (myBitRange != static_cast<T>(zero)) {

        return false;
    }

    return true;
}

template<typename T>
template<typename T2>
bool BitRangeTest<T>::TestBasicTypeCastMajorSize(T2 input) {

    const uint8 max = sizeof(T) * 8;
    const uint8 inputSize = sizeof(T2) * 8;
    const uint8 half = max / 2 - 2;
    const uint8 majorSize = (inputSize + 1);

    union {
        BitRange<T, majorSize, half> myBitRange;
        T initBitRange;
    };
    initBitRange = 0;

    bool isInputSigned = TypeCharacteristics<T2>::IsSigned();
    T2 maxValue = 0;
    T2 minValue = 0;
    if (isInputSigned) {
        maxValue = (((T2) 1) << (inputSize - (T2) 1));
        maxValue -= ((T2) 1);
        minValue = (((T2) 1) << (inputSize - (T2) 1));
        minValue = ~(minValue - (T2) 1);
    }
    else {
        maxValue = (T2) -1;
        minValue = 0;
    }
    T2 zero = (T2) 0;

    bool isSigned = TypeCharacteristics<T>::IsSigned();

    myBitRange = maxValue;

    // since the bit range has a size greater than the input,
    // the bit range should contain the max input value
    if (myBitRange != static_cast<T>(maxValue)) {

        return false;
    }

    // normally the bit range should contain the min input value
    myBitRange = minValue;
    if (myBitRange != static_cast<T>(minValue)) {

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
    if (myBitRange != static_cast<T>(zero)) {

        return false;
    }

    return true;
}

template<typename T>
bool BitRangeTest<T>::TestAnyTypeCast() {
    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;

    const uint8 size = 8;

    union {
        BitRange<T, size, half> myBitRange;
        T initBitRange;
    };
    initBitRange = 0;
    myBitRange = 0;

    AnyType atTest = myBitRange;

    if (atTest.GetDataPointer() != (&myBitRange)) {
        return false;
    }

    TypeDescriptor tdTest = atTest.GetTypeDescriptor();
    bool isSigned = T(-1) < 0;

    BasicType type = (isSigned) ? SignedInteger : UnsignedInteger;

    if ((tdTest.isStructuredData) || (tdTest.isConstant) || (tdTest.type != type) || (tdTest.numberOfBits != size)) {
        return false;
    }

    if (atTest.GetBitAddress() != half) {
        return false;
    }

    return true;
}

template<typename T>
bool BitRangeTest<T>::TestNumberOfBits() {

    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;
    const uint8 one = 1;

    BitRange<T, one, 0> myZeroBitRange;

    if (myZeroBitRange.GetNumberOfBits() != one) {
        return false;
    }

    BitRange<T, half, 0> myHalfBitRange;

    if (myHalfBitRange.GetNumberOfBits() != half) {
        return false;
    }
    BitRange<T, max, 0> myMaxBitRange;

    if (myMaxBitRange.GetNumberOfBits() != max) {
        return false;
    }

    return true;
}

template<typename T>
bool BitRangeTest<T>::TestBitOffset() {
    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;
    const uint8 one = 1;

    BitRange<T, max, one> myZeroBitRange;

    if (myZeroBitRange.BitOffset() != one) {
        return false;
    }

    BitRange<T, half, half> myHalfBitRange;

    if (myHalfBitRange.BitOffset() != half) {
        return false;
    }
    BitRange<T, max, one> myMaxBitRange;

    if (myMaxBitRange.BitOffset() != one) {
        return false;
    }

    return true;
}

#endif /* BITRANGETEST_H_ */

