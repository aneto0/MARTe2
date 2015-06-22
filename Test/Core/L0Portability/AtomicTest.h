/**
 * @file AtomicTest.h
 * @brief Header file for class AtomicTest
 * @date 22/06/2015
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

 * @details This header file contains the declaration of the class AtomicTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ATOMICTEST_H_
#define 		ATOMICTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Template class for Atomic tests.
 */
template<class T>
class AtomicTest {

public:

    /**
     * @brief Constructor.
     * @param[in] testValue the value to be tested by all
     * the test functions.
     */
    AtomicTest(T testValue);

    /**
     * @brief Tests the increment function.
     * @return true if the value is incremented.
     */
    bool TestIncrement();

    /**
     * @brief Tests the decrement function.
     * @return true if the value is decremented.
     */
    bool TestDecrement();

    /**
     * @brief Tests the test and set function.
     * @return true if when the value is 0, is setted to one.
     */
    bool TestTestAndSet();

    /**
     * @brief Tests the exchange function.
     * @return true if the values are exchanged.
     */
    bool TestExchange();

    /**
     * @brief Tests the addition function.
     * @return true if the sum works correctly.
     */
    bool TestAdd();

    /**
     * @brief Tests the subtraction function.
     * @return true if the subtraction works correctly.
     */
    bool TestSub();

    /**
     * @brief Executes all the tests.
     * @return true if all tests return true.
     */
    bool All();

private:
    /** The atomic variable */
    volatile T testValue;

};

/*---------------------------------------------------------------------------*/
/*                        Template method definitions                        */
/*---------------------------------------------------------------------------*/

template<class T>
AtomicTest<T>::AtomicTest(T testValue) {
    this->testValue = testValue;
}

template<class T>
bool AtomicTest<T>::TestIncrement() {
    bool testResult = false;
    T auxValue = testValue;

    Atomic::Increment(&auxValue);
    testResult = (auxValue == (testValue + 1));

    return testResult;
}

template<class T>
bool AtomicTest<T>::TestDecrement() {
    bool testResult = false;
    T auxValue = testValue;

    Atomic::Decrement(&auxValue);
    testResult = (auxValue == (testValue - 1));

    return testResult;
}

template<class T>
bool AtomicTest<T>::TestTestAndSet() {
    T testVal = 0;
    bool ok = false;

    // Set the semaphore
    ok = Atomic::TestAndSet(&testVal);

    // When the semaphore is set, the test and set function should fail
    ok = ok && !Atomic::TestAndSet(&testVal);

    // Unset the semaphore and set it again
    testVal = 0;
    ok = ok && Atomic::TestAndSet(&testVal);

    return ok;
}

template<class T>
bool AtomicTest<T>::TestExchange() {
    bool testResult = false;
    T auxValue = testValue;

    Atomic::Exchange(&auxValue, 1234);
    testResult = (auxValue == 1234);

    return testResult;
}

template<class T>
bool AtomicTest<T>::TestAdd() {
    bool testResult = false;
    T auxValue = testValue;

    Atomic::Add(&auxValue, 10);
    testResult = (auxValue == testValue + 10);

    return testResult;
}

template<class T>
bool AtomicTest<T>::TestSub() {
    bool testResult = false;
    T auxValue = testValue;

    Atomic::Sub(&auxValue, 10);
    testResult = (auxValue == testValue - 10);

    return testResult;
}

template<class T>
bool AtomicTest<T>::All() {
    bool ok = TestIncrement();
    ok = ok && TestDecrement();
    ok = ok && TestTestAndSet();
    return ok;
}

#endif /* ATOMICTEST_H_ */

