/**
 * @file RealTimeApplicationGTest.cpp
 * @brief Source file for class RealTimeApplicationGTest
 * @date 04/ago/2016
 * @author pc
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
 * the class RealTimeApplicationGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "RealTimeApplicationTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(RealTimeApplicationGTest,TestConstructor) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(RealTimeApplicationGTest,TestInitialise) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestInitialise());
}


TEST(RealTimeApplicationGTest,TestConfigureApplication) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication());
}
TEST(RealTimeApplicationGTest,TestConfigureApplicationNoInit) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationNoInit());
}


TEST(RealTimeApplicationGTest,TestPrepareNextState) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(RealTimeApplicationGTest,TestStartExecution) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestStartExecution());
}

TEST(RealTimeApplicationGTest,TestStopExecution) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestStopExecution());
}

