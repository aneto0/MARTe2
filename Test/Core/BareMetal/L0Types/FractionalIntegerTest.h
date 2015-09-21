/**
 * @file FractionalIntegerTest.h
 * @brief Header file for class FractionalIntegerTest
 * @date 01/09/2015
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

 * @details This header file contains the declaration of the class FractionalIntegerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FRACTIONALINTEGERTEST_H_
#define FRACTIONALINTEGERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "FractionalInteger.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the FractionalInteger functions.
 */
template<typename T>
class FractionalIntegerTest {
public:

    /**
     * @brief Tests the cast to the base type using for FractionalInteger with a bit size
     * minor than the input size.
     * @param[in] input specifies the input type.
     * @return true if the maximum input value is saturated to the maximum FractionalInteger value and
     * if the minimum input value is saturated to the minimum FractionalInteger value. If this type is
     * signed and the input is unsigned returns true if FractionalInteger value is set to zero. False otherwise
     */
    template<typename T2>
    bool TestBasicTypeCastMinorSize(T2 input);

    /**
     * @brief Tests the cast to the base type using for FractionalInteger a bit size
     * greater than the input size.
     * @param[in] input specifies the input type.
     * @return true if the FractionalInteger value is equal to the maximum input value and
     * if the FractionalInteger value is equal to the minimum input value is saturated to the minimum. If this type is
     * unsigned and the input is negative returns true if FractionalInteger is set to zero. False otherwise.
     */
    template<typename T2>
    bool TestBasicTypeCastMajorSize(T2 input);

    /**
     * @brief Checks if BitSize function returns the same size specified in template initialization.
     * @return true if BitSize function returns the same size specified in template initialization, false otherwise.
     */
    bool TestBitSize();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
template<typename T2>
bool FractionalIntegerTest<T>::TestBasicTypeCastMinorSize(T2 input) {

    const uint8 max = sizeof(T) * 8;
    const uint8 half = max / 2;
    const uint8 inputSize = sizeof(T2) * 8;

    // the size of the bit range
    const uint8 minorSize = (inputSize < max) ? (inputSize - 1) : (half - 1);

    FractionalInteger<T, minorSize> myFractionalInteger;

    bool isInputSigned = TypeCharacteristics::IsSigned<T2>();

    // max and min values of the input
    T2 maxValue = isInputSigned ? ((((T2) 1) << (inputSize - (T2) 1)) - (T2) 1) : ((T2) -1);
    T2 minValue = isInputSigned ? ~((((T2) 1) << (inputSize - (T2) 1)) - (T2) 1) : (T2) 0;
    T2 zero = (T2) 0;

    bool isSigned = TypeCharacteristics::IsSigned<T>();

    // max and min values of the bit range
    const T thisMaxValue = isSigned ? ((((T) 1) << (minorSize - (T) 1)) - (T) 1) : (((T) -1) >> (sizeof(T) * 8 - minorSize));
    const T thisMinValue = isSigned ? ~((((T) 1) << (minorSize - (T) 1)) - (T) 1) : (T) 0;

    myFractionalInteger = maxValue;

    // since the bit range has a size minor than the input normally the value should be saturated
    if (myFractionalInteger != thisMaxValue) {
        return false;
    }
    myFractionalInteger = minValue;
    if (myFractionalInteger != thisMinValue) {
        //if the input is unsigned and the bit range signed, the minValue passed is zero
        if ((!isInputSigned) && (isSigned)) {
            if (myFractionalInteger != minValue) {
                return false;
            }
        }
        //if the input is signed and the bit range unsigned, the minValue passed is zero alias thisMinValue
        else {
            return false;
        }
    }

    myFractionalInteger = zero;
    if (myFractionalInteger != zero) {

        return false;
    }

    return true;
}

template<typename T>
template<typename T2>
bool FractionalIntegerTest<T>::TestBasicTypeCastMajorSize(T2 input) {

    const uint8 inputSize = sizeof(T2) * 8;
    const uint8 majorSize = (inputSize + 1);

    FractionalInteger<T, majorSize> myFractionalInteger;

    bool isInputSigned = TypeCharacteristics::IsSigned<T2>();
    T2 maxValue = isInputSigned ? ((((T2) 1) << (inputSize - (T2) 1)) - (T2) 1) : ((T2) -1);
    T2 minValue = isInputSigned ? ~((((T2) 1) << (inputSize - (T2) 1)) - (T2) 1) : 0;
    T2 zero = (T2) 0;

    bool isSigned = TypeCharacteristics::IsSigned<T>();

    myFractionalInteger = maxValue;

    // since the bit range has a size greater than the input,
    // the bit range should contain the max input value
    if (myFractionalInteger != maxValue) {
        return false;
    }

    // normally the bit range should contain the min input value
    myFractionalInteger = minValue;
    if (myFractionalInteger != minValue) {

        // if the bit range is unsigned and the input signed,
        // the negative value is saturated to zero in the bit range
        if ((!isSigned) && (isInputSigned)) {
            if (myFractionalInteger != 0) {
                return false;
            }
        }
        else {

            return false;
        }
    }

    myFractionalInteger = zero;
    if (myFractionalInteger != zero) {

        return false;
    }

    return true;
}

template<typename T>
bool FractionalIntegerTest<T>::TestBitSize() {

    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;
    const uint8 zero = 0;

    FractionalInteger<T, zero> myZeroFractionalInteger;

    if (myZeroFractionalInteger.BitSize() != zero) {
        return false;
    }

    FractionalInteger<T, half> myHalfFractionalInteger;

    if (myHalfFractionalInteger.BitSize() != half) {
        return false;
    }
    FractionalInteger<T, max> myMaxFractionalInteger;

    if (myMaxFractionalInteger.BitSize() != max) {
        return false;
    }

    return true;
}

#endif /* FRACTIONALINTEGERTEST_H_ */

