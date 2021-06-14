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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "BasicTypeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L0Types_BasicTypeGTest,TestConstructorInt8) {
     BasicTypeTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestConstructorInt8());
}

TEST(BareMetal_L0Types_BasicTypeGTest,TestConstructorUInt8) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestConstructorUInt8());
}

TEST(BareMetal_L0Types_BasicTypeGTest,TestConstructorInt16) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestConstructorInt16());
}

TEST(BareMetal_L0Types_BasicTypeGTest,TestConstructorUInt16) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestConstructorUInt16());
}

TEST(BareMetal_L0Types_BasicTypeGTest,TestConstructorInt32) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestConstructorInt32());
}

TEST(BareMetal_L0Types_BasicTypeGTest,TestConstructorUInt32) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestConstructorUInt32());
}

TEST(BareMetal_L0Types_BasicTypeGTest,TestConstructorInt64) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestConstructorInt64());
}

TEST(BareMetal_L0Types_BasicTypeGTest,TestConstructorUInt64) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestConstructorUInt64());
}

TEST(BareMetal_L0Types_BasicTypeGTest,TestConstructorFloat32) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestConstructorFloat32());
}

TEST(BareMetal_L0Types_BasicTypeGTest,TestConstructorFloat64) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestConstructorFloat64());
}

TEST(BareMetal_L0Types_BasicTypeGTest,TestConstructorChar8) {
    BasicTypeTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestConstructorChar8());
}

