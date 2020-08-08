/**
 * @file SafeMathTest.h
 * @brief Header file for class SafeMathTest
 * @date 17/04/2020
 * @author Dídac Magriñá
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

 * @details This header file contains the declaration of the class SafeMathTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SAFEMATHTEST_H_
#define SAFEMATHTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include <cstdio>
#include "SafeMath.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A Class to test the SafeMath functions.
 */
class SafeMathTest {

public:

    /**
     * @brief Constructor
     */
    SafeMathTest();

    /**
     * @brief Tests SignBit function
     */
    template<typename T> bool TestSignBit(const T expectedReturn);

    /**
     * @brief Tests AllOnes function
     */
    template<typename T> bool TestAllOnes(const T expectedReturn);

    /**
     * @brief Tests Addition with result within type T range
     */
    template<typename T> bool TestAdditionNoError();

    /**
     * @brief Tests Addition with negative result within type T range
     */
    template<typename T> bool TestAdditionNegativeNoError();

    /**
     * @brief Tests Addition with result overflowing type T
     */
    template<typename T> bool TestAdditionOverflow();

    /**
     * @brief Tests Addition with result undeerflowing type T
     */
    template<typename T> bool TestAdditionUnderflow();

    /**
     * @brief Tests Subtraction with result within type T range
     */
    template<typename T> bool TestSubtractionNoError();

    /**
     * @brief Tests Subtraction with negative result within type T range
     */
    template<typename T> bool TestSubtractionNegativeNoError();

    /**
     * @brief Tests Subtraction with result overflowing type T
     */
    template<typename T> bool TestSubtractionOverflow();

    /**
     * @brief Tests Subtraction with result underflowing type T
     */
    template<typename T> bool TestSubtractionUnderflow();

    /**
     * @brief Tests Multiplication with result within type T range
     */
    template<typename T> bool TestMultiplicationNoError();

    /**
     * @brief Tests Multiplication with negative result within type T range
     */
    template<typename T> bool TestMultiplicationNegativeNoError();

    /**
     * @brief Tests Multiplication with result overflowing type T
     */
    template<typename T> bool TestMultiplicationOverflow();

    /**
     * @brief Tests Multiplication with result underflowing type T
     */
    template<typename T> bool TestMultiplicationUnderflow();

    /**
     * @brief Tests IsEqual
     */
    template<typename T> bool TestIsEqual();

    /**
     * @brief Tests IsEqual for floating-point values using machine epsilon
     */
    template<typename T> bool TestIsEqualEpsilon();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


template<typename T> bool SafeMathTest::TestSignBit(const T expectedReturn){
    return (SafeMath::SignBit<T>() == expectedReturn);
}

template<typename T> bool SafeMathTest::TestAllOnes(const T expectedReturn){
    return (SafeMath::AllOnes<T>() == expectedReturn);
}

template<typename T> bool SafeMathTest::TestAdditionNoError(){
    T actualSum, max;
    ErrorManagement::ErrorType actualError;

    max = TypeCharacteristics<T>::MaxValue();

    actualError = SafeMath::Addition<T>(max - 1, 1, actualSum);
    return ((actualError == ErrorManagement::NoError) && (actualSum == max));
}

template<typename T> bool SafeMathTest::TestAdditionNegativeNoError(){
    T actualSum, min;
    ErrorManagement::ErrorType actualError;

    min = TypeCharacteristics<T>::MinValue();

    actualError = SafeMath::Addition<T>(min + 1, -1, actualSum);
    return ((actualError == ErrorManagement::NoError) && (actualSum == min));
}

template<typename T> bool SafeMathTest::TestAdditionOverflow(){
    T actualSum, max, min;
    ErrorManagement::ErrorType actualError;

    max = TypeCharacteristics<T>::MaxValue();
    min = TypeCharacteristics<T>::MinValue();

    actualError = SafeMath::Addition<T>(max, 1, actualSum);
    return ((actualError == ErrorManagement::Overflow) && (actualSum == min));
}

template<typename T> bool SafeMathTest::TestAdditionUnderflow(){
    T actualSum, max, min;
    ErrorManagement::ErrorType actualError;

    max = TypeCharacteristics<T>::MaxValue();
    min = TypeCharacteristics<T>::MinValue();

    actualError = SafeMath::Addition<T>(min, -1, actualSum);
    return ((actualError == ErrorManagement::Underflow) && (actualSum == max));
}


template<typename T> bool SafeMathTest::TestSubtractionNoError(){
    T actualSum, min;
    ErrorManagement::ErrorType actualError;

    min = TypeCharacteristics<T>::MinValue();

    actualError = SafeMath::Subtraction<T>(min + 1, 1, actualSum);
    return ((actualError == ErrorManagement::NoError) && (actualSum == min));
}

template<typename T> bool SafeMathTest::TestSubtractionNegativeNoError(){
    T actualSum, min;
    ErrorManagement::ErrorType actualError;

    min = TypeCharacteristics<T>::MinValue();

    actualError = SafeMath::Subtraction<T>(min + 1, 1, actualSum);
    return ((actualError == ErrorManagement::NoError) && (actualSum == min));
}

template<typename T> bool SafeMathTest::TestSubtractionOverflow(){
    T actualSum, max, min;
    ErrorManagement::ErrorType actualError;

    max = TypeCharacteristics<T>::MaxValue();
    min = TypeCharacteristics<T>::MinValue();

    actualError = SafeMath::Subtraction<T>(max, -1, actualSum);
    return ((actualError == ErrorManagement::Overflow) && (actualSum == min));
}

template<typename T> bool SafeMathTest::TestSubtractionUnderflow(){
    T actualSum, max, min;
    ErrorManagement::ErrorType actualError;

    max = TypeCharacteristics<T>::MaxValue();
    min = TypeCharacteristics<T>::MinValue();

    actualError = SafeMath::Subtraction<T>(min, 1, actualSum);
    return ((actualError == ErrorManagement::Underflow) && (actualSum == max));
}

template<typename T> bool SafeMathTest::TestMultiplicationNoError(){
    T actualProduct, evenMax;
    ErrorManagement::ErrorType actualError;

    evenMax = TypeCharacteristics<T>::MaxValue() - 1;

    actualError = SafeMath::Multiplication<T>(evenMax / 2, 2, actualProduct);
    return ((actualError == ErrorManagement::NoError) && (actualProduct == evenMax));
}

template<typename T> bool SafeMathTest::TestMultiplicationNegativeNoError(){
    T actualProduct, evenMin;
    ErrorManagement::ErrorType actualError;

    evenMin = TypeCharacteristics<T>::MinValue();

    actualError = SafeMath::Multiplication<T>(evenMin / 2, 2, actualProduct);
    return ((actualError == ErrorManagement::NoError) && (actualProduct == evenMin));
}

template<typename T> bool SafeMathTest::TestMultiplicationOverflow(){
    T actualProduct, evenMax, min;
    ErrorManagement::ErrorType actualError;

    evenMax = TypeCharacteristics<T>::MaxValue() - 1;
    min = TypeCharacteristics<T>::MinValue();

    actualError = SafeMath::Multiplication<T>((evenMax / 2) + 1, 2, actualProduct);
    return ((actualError == ErrorManagement::Overflow) && (actualProduct == min));
}

template<typename T> bool SafeMathTest::TestMultiplicationUnderflow(){
    T actualProduct, min, max;
    ErrorManagement::ErrorType actualError;

    min = TypeCharacteristics<T>::MinValue();
    max = TypeCharacteristics<T>::MaxValue();

    actualError = SafeMath::Multiplication<T>((min / 3) - 1, 3, actualProduct);
    return ((actualError == ErrorManagement::Underflow) && (actualProduct == max));
}

template<typename T> bool SafeMathTest::TestIsEqual(){
    T min, max, minDelta, maxDelta;
    bool ret = false;

    min = TypeCharacteristics<T>::MinValue();
    max = TypeCharacteristics<T>::MaxValue();
    minDelta = min/10;
    maxDelta = max/10;
    
    ret = (SafeMath::IsEqual(min, min) && SafeMath::IsEqual(max, max));
    
    if (ret) {
        ret = (!SafeMath::IsEqual(min + 1, min + minDelta) && !SafeMath::IsEqual(max - 1, max - maxDelta));
    }
    
    return ret;
}

template<typename T> bool SafeMathTest::TestIsEqualEpsilon(){
    T epsilon, one;
    bool ret = false;
    
    one = 1;
    epsilon = TypeCharacteristics<T>::Epsilon();
    
    ret = (SafeMath::IsEqual(one, one));
    
    if (ret) {
        ret = ( !SafeMath::IsEqual(one, one + epsilon) && !SafeMath::IsEqual(one, one - epsilon));
        
    }
    
    return ret;
}

#endif /* SAFEMATHTEST_H_ */

