/**
 * @file AtomicGTest.cpp
 * @brief Source file for class AtomicGTest
 * @date 25/giu/2015
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

 * @details This source file contains the definition of all the methods for
 * the class AtomicGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "AtomicTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

class AtomicGTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor
        // (right before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test
        // (right before the destructor).
    }
};

TEST_F(AtomicGTest,Int32Test) {
    int32 testValue = 32;
    AtomicTest<int32> int32Test(testValue);
    ASSERT_TRUE(int32Test.All());
}

TEST_F(AtomicGTest,Int16Test) {
    int16 testValue = 16;
    AtomicTest<int16> int16Test(testValue);
    ASSERT_TRUE(int16Test.All());
}

TEST_F(AtomicGTest,Int8Test) {
    int8 testValue = 8;
    AtomicTest<int8> int8Test(testValue);
    ASSERT_TRUE(int8Test.All());
}

TEST_F(AtomicGTest,ExchangeTest) {
    int32 testValue = 32;
    AtomicTest<int32> exchangeTest(testValue);
    ASSERT_TRUE(exchangeTest.TestExchange());
}

TEST_F(AtomicGTest,AddTest) {
    int32 testValue = 32;
    AtomicTest<int32> addTest(testValue);
    ASSERT_TRUE(addTest.TestAdd());
}

TEST_F(AtomicGTest,SubTest) {
    int32 testValue = 32;
    AtomicTest<int32> subTest(testValue);
    ASSERT_TRUE(subTest.TestSub());
}

