/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id$
 *
 **/
/** @file AtomicTest.h
   @brief Tests of Atomic.h functions. */

#ifndef ATOMIC_TEST_H
#define ATOMIC_TEST_H

#include "Atomic.h"


/** @brief Template class for Atomic tests. */
template<class T>
class AtomicTest {

private:
    volatile T testValue;

public:

    /**@brief Constructor.
     * @param testValue the value to be tested by the all
     * the test functions.
     */
    AtomicTest(T testValue) {
        this->testValue = testValue;
    }

    /**@brief Tests the increment function.
     * @return true if the value is incremented.
     */
    bool TestIncrement() {
        bool testResult = false;
        T auxValue = testValue;

        Atomic::Increment(&auxValue);
        testResult = (auxValue == (testValue + 1));

        return testResult;
    }

    /**
     * @brief Tests the decrement function.
     * @return true if the value is decremented.
     */
    bool TestDecrement() {
        bool testResult = false;
        T auxValue = testValue;

        Atomic::Decrement(&auxValue);
        testResult = (auxValue == (testValue - 1));

        return testResult;
    }

    /**
     * @brief Tests the test and set function.
     * @return true if when the value is 0, is setted to one.
     */
    bool TestTestAndSet() {
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

    /**
     * @brief Tests the exchange function.
     * @return true if the values are exchanged.
     */
    bool TestExchange() {
        bool testResult = false;
        T auxValue = testValue;

        Atomic::Exchange(&auxValue, 1234);
        testResult = (auxValue == 1234);

        return testResult;
    }

    /**
     * @brief Tests the addition function.
     * @return true if the sum works correctly.
     */
    bool TestAdd() {
        bool testResult = false;
        T auxValue = testValue;

        Atomic::Add(&auxValue, 10);
        testResult = (auxValue == testValue + 10);

        return testResult;
    }

    /**
     * @brief Tests the subtraction function.
     * @return true if the subtraction works correctly.
     */
    bool TestSub() {
        bool testResult = false;
        T auxValue = testValue;

        Atomic::Sub(&auxValue, 10);
        testResult = (auxValue == testValue - 10);

        return testResult;
    }

    /**
     * @brief Executes all the tests.
     * @return true if all tests return true.
     */
    bool All() {
        bool ok = TestIncrement();
        ok = ok && TestDecrement();
        ok = ok && TestTestAndSet();
        return ok;
    }

};

#endif
