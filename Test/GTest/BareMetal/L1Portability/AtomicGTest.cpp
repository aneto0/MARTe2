/**
 * @file AtomicGTest.cpp
 * @brief Source file for class AtomicGTest
 * @date 25/giu/2015
 * @author Giuseppe Ferr�
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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "AtomicTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



TEST(BareMetal_L1Portability_AtomicGTest,Int32TestIncrement) {
    int32 testValue = 32;
    AtomicTest<int32> int32Test(testValue);
    ASSERT_TRUE(int32Test.TestIncrement());
}

TEST(BareMetal_L1Portability_AtomicGTest,Int32TestDecrement) {
    int32 testValue = 32;
    AtomicTest<int32> int32Test(testValue);
    ASSERT_TRUE(int32Test.TestDecrement());
}



TEST(BareMetal_L1Portability_AtomicGTest,Int32TestTestAndSet) {
    int32 testValue = 32;
    AtomicTest<int32> int32Test(testValue);
    ASSERT_TRUE(int32Test.TestTestAndSet());
}

TEST(BareMetal_L1Portability_AtomicGTest,Int32TestBoundaryIncrement) {
    int32 testValue = 32;
    AtomicTest<int32> int32Test(testValue);
    ASSERT_TRUE(int32Test.TestBoundaryIncrement());
}


TEST(BareMetal_L1Portability_AtomicGTest,Int32TestBoundaryDecrement) {
    int32 testValue = 32;
    AtomicTest<int32> int32Test(testValue);
    ASSERT_TRUE(int32Test.TestBoundaryDecrement());
}

TEST(BareMetal_L1Portability_AtomicGTest,Int16TestIncrement) {
    int32 testValue = 16;
    AtomicTest<int16> int16Test(testValue);
    ASSERT_TRUE(int16Test.TestIncrement());
}

TEST(BareMetal_L1Portability_AtomicGTest,Int16TestDecrement) {
    int32 testValue = 16;
    AtomicTest<int16> int16Test(testValue);
    ASSERT_TRUE(int16Test.TestDecrement());
}



TEST(BareMetal_L1Portability_AtomicGTest,Int16TestTestAndSet) {
    int32 testValue = 16;
    AtomicTest<int16> int16Test(testValue);
    ASSERT_TRUE(int16Test.TestTestAndSet());
}

TEST(BareMetal_L1Portability_AtomicGTest,Int16TestBoundaryIncrement) {
    int32 testValue = 16;
    AtomicTest<int16> int16Test(testValue);
    ASSERT_TRUE(int16Test.TestBoundaryIncrement());
}


TEST(BareMetal_L1Portability_AtomicGTest,Int16TestBoundaryDecrement) {
    int32 testValue = 16;
    AtomicTest<int16> int16Test(testValue);
    ASSERT_TRUE(int16Test.TestBoundaryDecrement());
}


TEST(BareMetal_L1Portability_AtomicGTest,Int8TestIncrement) {
    int32 testValue = 8;
    AtomicTest<int8> int8Test(testValue);
    ASSERT_TRUE(int8Test.TestIncrement());
}

TEST(BareMetal_L1Portability_AtomicGTest,Int8TestDecrement) {
    int32 testValue = 8;
    AtomicTest<int8> int8Test(testValue);
    ASSERT_TRUE(int8Test.TestDecrement());
}



TEST(BareMetal_L1Portability_AtomicGTest,Int8TestTestAndSet) {
    int32 testValue = 8;
    AtomicTest<int8> int8Test(testValue);
    ASSERT_TRUE(int8Test.TestTestAndSet());
}

TEST(BareMetal_L1Portability_AtomicGTest,Int8TestBoundaryIncrement) {
    int32 testValue = 8;
    AtomicTest<int8> int8Test(testValue);
    ASSERT_TRUE(int8Test.TestBoundaryIncrement());
}


TEST(BareMetal_L1Portability_AtomicGTest,Int8TestBoundaryDecrement) {
    int32 testValue = 8;
    AtomicTest<int8> int8Test(testValue);
    ASSERT_TRUE(int8Test.TestBoundaryDecrement());
}


TEST(BareMetal_L1Portability_AtomicGTest,ExchangeTest) {
    int32 testValue = 32;
    AtomicTest<int32> exchangeTest(testValue);
    ASSERT_TRUE(exchangeTest.TestExchange());
}

TEST(BareMetal_L1Portability_AtomicGTest,AddTest) {
    int32 testValue = 32;
    AtomicTest<int32> addTest(testValue);
    ASSERT_TRUE(addTest.TestAdd());
}

TEST(BareMetal_L1Portability_AtomicGTest,SubTest) {
    int32 testValue = 32;
    AtomicTest<int32> subTest(testValue);
    ASSERT_TRUE(subTest.TestSub());
}

