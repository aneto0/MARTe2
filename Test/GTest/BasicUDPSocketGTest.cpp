/**
 * @file BasicUDPSocketGTest.cpp
 * @brief Source file for class BasicUDPSocketGTest
 * @date 02/11/2015
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
 * the class BasicUDPSocketGTest (public, protected, and private). Be aware that some 
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
#include "BasicUDPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BasicUDPSocketGTest,TesDefaultConstructor) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestDefaultConstructor());
}

TEST(BasicUDPSocketGTest,TestSeek) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestSeek());
}

TEST(BasicUDPSocketGTest,TestSize) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestSize());
}

TEST(BasicUDPSocketGTest,TestRelativeSeek) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestRelativeSeek());
}

TEST(BasicUDPSocketGTest,TestPosition) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestPosition());
}

TEST(BasicUDPSocketGTest,TestSetSize) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestSetSize());
}

TEST(BasicUDPSocketGTest,TestCanWrite) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestCanWrite());
}

TEST(BasicUDPSocketGTest,TestCanRead) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestCanRead());
}

TEST(BasicUDPSocketGTest,TestCanSeek) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestCanSeek());
}
