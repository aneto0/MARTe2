/**
 * @file MemoryGTest.cpp
 * @brief Source file for class MemoryGTest
 * @date 29/06/2015
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
 * the class MemoryGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TestSupport.h"
#include "SemaphoreTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(SemaphoreGTest,TestConstructor) {
    ASSERT_TRUE(SemaphoreTest::TestConstructor());
}

TEST(SemaphoreGTest,TestDestructor) {
    ASSERT_TRUE(SemaphoreTest::TestDestructor());
}

TEST(SemaphoreGTest,TestCreate) {
    ASSERT_TRUE(SemaphoreTest::TestCreate());
}

TEST(SemaphoreGTest,TestClose) {
    ASSERT_TRUE(SemaphoreTest::TestClose());
}

TEST(SemaphoreGTest,TestTake_Latching) {
    ASSERT_TRUE(SemaphoreTest::TestTake_Latching(MilliSeconds(100,Units::ms)));
}

TEST(SemaphoreGTest,TestSet_Latching) {
    ASSERT_TRUE(SemaphoreTest::TestSet_Latching());
}

TEST(SemaphoreGTest,TestReset_Latching) {
    ASSERT_TRUE(SemaphoreTest::TestReset_Latching());
}

TEST(SemaphoreGTest,TestTake_Latching_Timeout) {
    ASSERT_TRUE(SemaphoreTest::TestTake_Latching(MilliSeconds(500,Units::ms)));
}

TEST(SemaphoreGTest,TestTake_Latching_Threads) {
    ASSERT_TRUE(SemaphoreTest::TestTake_Latching_Threads(32,MilliSeconds(500,Units::ms)));
}

TEST(SemaphoreGTest,TestTake_Latching_Threads_InfTO) {
    ASSERT_TRUE(SemaphoreTest::TestTake_Latching_Threads(32,MilliSeconds::Infinite));
}

TEST(SemaphoreGTest,TestTake_Latching_Threads_0TO) {
    ASSERT_TRUE(SemaphoreTest::TestTake_Latching_Threads(32,MilliSeconds(1,Units::ms))==false);
}

TEST(SemaphoreGTest,TestTake_AutoResetting_Threads) {
    ASSERT_TRUE(SemaphoreTest::TestTake_AutoResetting_Threads(32,MilliSeconds(500,Units::s)));
}

TEST(SemaphoreGTest,TestTake_Counting_Threads) {
    ASSERT_TRUE(SemaphoreTest::TestTake_Counting_Threads(32,MilliSeconds(500,Units::s)));
}

TEST(SemaphoreGTest,TestTake_Combined_Threads) {
    ASSERT_TRUE(SemaphoreTest::TestTake_Combined_Threads(32,MilliSeconds(500,Units::s)));
}
