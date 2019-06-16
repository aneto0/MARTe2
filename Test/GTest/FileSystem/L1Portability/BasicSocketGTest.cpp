/**
 * @file BasicSocketGTest.cpp
 * @brief Source file for class BasicSocketGTest
 * @date 29/10/2015
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
 * the class BasicSocketGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicSocketTest.h"
#include "TestSupport.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BasicSocketGTest,TesDefaultConstructor) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestDefaultConstructor());
}

TEST(BasicSocketGTest,TestSetBlocking_block) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestSetBlocking(true));
}

TEST(BasicSocketGTest,TestSetBlocking_nonblock) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestSetBlocking(false));
}

TEST(BasicSocketGTest,TestSetBlocking_false_block) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestSetBlocking_false(true));
}

TEST(BasicSocketGTest,TestSetBlocking_false_nonblock) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestSetBlocking_false(false));
}

TEST(BasicSocketGTest,TestClose) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestClose());
}

TEST(BasicSocketGTest,TestClose_false) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestClose_false());
}

TEST(BasicSocketGTest,TestGetSource) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestGetSource());
}

TEST(BasicSocketGTest,TestGetDestination) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestGetDestination());
}

TEST(BasicSocketGTest,TestSetSource) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestGetSource());
}

TEST(BasicSocketGTest,TestSetDestination) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestGetDestination());
}

TEST(BasicSocketGTest,TestIsBlocking_block) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestIsBlocking(true));
}

TEST(BasicSocketGTest,TestIsBlocking_nonblock) {
    BasicSocketTest basicSocketTest;
    ASSERT_TRUE(basicSocketTest.TestIsBlocking(false));
}
