/**
 * @file BasicTCPSocketGTest.cpp
 * @brief Source file for class BasicTCPSocketGTest
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
 * the class BasicTCPSocketGTest (public, protected, and private). Be aware that some 
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
#include "BasicTCPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BasicTCPSocketGTest,TesDefaultConstructor) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestDefaultConstructor());
}

TEST(BasicTCPSocketGTest,TestSeek) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestSeek());
}

TEST(BasicTCPSocketGTest,TestSize) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestSize());
}

TEST(BasicTCPSocketGTest,TestRelativeSeek) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestRelativeSeek());
}

TEST(BasicTCPSocketGTest,TestPosition) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestPosition());
}

TEST(BasicTCPSocketGTest,TestSetSize) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestSetSize());
}

TEST(BasicTCPSocketGTest,TestCanWrite) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestCanWrite());
}

TEST(BasicTCPSocketGTest,TestCanRead) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestCanRead());
}

TEST(BasicTCPSocketGTest,TestCanSeek) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestCanSeek());
}

