/**
 * @file BasicTypeGTest.cpp
 * @brief Source file for class BasicTypeGTest
 * @date 02/09/2015
 * @author Llorenç Capellà
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
 * the class BasicTypeGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "BasicTypeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(BasicTypeGTest,TestBasicTypeConstructorInt8) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestBasicTypeConstructorInt8());
}

TEST(BasicTypeGTest,TestBasicTypeConstructorUInt8) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestBasicTypeConstructorUInt8());
}

TEST(BasicTypeGTest,TestBasicTypeConstructorInt16) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestBasicTypeConstructorInt16());
}

TEST(BasicTypeGTest,TestBasicTypeConstructorUInt16) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestBasicTypeConstructorUInt16());
}

TEST(BasicTypeGTest,TestBasicTypeConstructorInt32) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestBasicTypeConstructorInt32());
}

TEST(BasicTypeGTest,TestBasicTypeConstructorUInt32) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestBasicTypeConstructorUInt32());
}

TEST(BasicTypeGTest,TestBasicTypeConstructorInt64) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestBasicTypeConstructorInt64());
}

TEST(BasicTypeGTest,TestBasicTypeConstructorUInt64) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestBasicTypeConstructorUInt64());
}

TEST(BasicTypeGTest,TestBasicTypeConstructorFloat32) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestBasicTypeConstructorFloat32());
}

TEST(BasicTypeGTest,TestBasicTypeConstructorFloat64) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestBasicTypeConstructorFloat64());
}

TEST(BasicTypeGTest,TestBasicTypeConstructorChar8) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestBasicTypeConstructorChar8());
}




	
