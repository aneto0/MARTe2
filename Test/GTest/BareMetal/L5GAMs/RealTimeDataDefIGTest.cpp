/**
 * @file RealTimeDataDefIGTest.cpp
 * @brief Source file for class RealTimeDataDefIGTest
 * @date 04/mar/2016
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
 * the class RealTimeDataDefIGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeDataDefITest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeDataDefIGTest,TestConstructor) {
    RealTimeDataDefITest rtiTest;
    ASSERT_TRUE(rtiTest.TestConstructor());
}

TEST(RealTimeDataDefIGTest,TestInitialise) {
    RealTimeDataDefITest rtiTest;
    ASSERT_TRUE(rtiTest.TestInitialise());
}

TEST(RealTimeDataDefIGTest,TestGetType) {
    RealTimeDataDefITest rtiTest;
    ASSERT_TRUE(rtiTest.TestGetType());
}

TEST(RealTimeDataDefIGTest,TestGetPath) {
    RealTimeDataDefITest rtiTest;
    ASSERT_TRUE(rtiTest.TestGetPath());
}

TEST(RealTimeDataDefIGTest,TestNumberOfDimensions_Vector) {
    RealTimeDataDefITest rtiTest;
    ASSERT_TRUE(rtiTest.TestNumberOfDimensions("uint32", "[32]", 1));
}

TEST(RealTimeDataDefIGTest,TestNumberOfDimensions_Vector_1Dim) {
    RealTimeDataDefITest rtiTest;
    ASSERT_TRUE(rtiTest.TestNumberOfDimensions("uint32", "[1]", 1));
}

TEST(RealTimeDataDefIGTest,TestNumberOfDimensions_Matrix) {
    RealTimeDataDefITest rtiTest;
    ASSERT_TRUE(rtiTest.TestNumberOfDimensions("uint32", "[32][12]", 2));
}

TEST(RealTimeDataDefIGTest,TestNumberOfDimensions_Matrix_1Dim) {
    RealTimeDataDefITest rtiTest;
    ASSERT_TRUE(rtiTest.TestNumberOfDimensions("uint32", "[1][1]", 2));
}

TEST(RealTimeDataDefIGTest,TestNumberOfElements_Vector) {
    RealTimeDataDefITest rtiTest;
    uint32 ret[3] = { 32, 1, 1 };
    ASSERT_TRUE(rtiTest.TestNumberOfElements("uint32", "[32]", ret));
}

TEST(RealTimeDataDefIGTest,TestNumberOfElements_Matrix) {
    RealTimeDataDefITest rtiTest;
    uint32 ret[3] = { 12, 32, 1 };
    ASSERT_TRUE(rtiTest.TestNumberOfElements("uint32", "[32][12]", ret));
}

