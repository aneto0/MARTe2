/**
 * @file GAMDataSourceGTest.cpp
 * @brief Source file for class GAMDataSourceGTest
 * @date 28/07/2016
 * @author Andre Neto
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
 * the class GAMDataSourceGTest (public, protected, and private). Be aware that some 
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
#include "GAMDataSourceTest.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(GAMDataSourceGTest,TestConstructor) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(GAMDataSourceGTest,TestPrepareNextState_NoDefault) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState_NoDefault());
}

TEST(GAMDataSourceGTest,TestPrepareNextState_Default_Basic) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState_Default_Basic());
}
	
TEST(GAMDataSourceGTest,TestPrepareNextState_Default_Dimensions) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState_Default_Dimensions());
}

TEST(GAMDataSourceGTest,TestPrepareNextState_Default_Dimensions_Mismatch_1) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState_Default_Dimensions_Mismatch_1());
}

TEST(GAMDataSourceGTest,TestPrepareNextState_Default_Dimensions_Mismatch_2) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState_Default_Dimensions_Mismatch_2());
}

TEST(GAMDataSourceGTest,TestPrepareNextState_Default_Elements_Mismatch) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState_Default_Elements_Mismatch());
}

TEST(GAMDataSourceGTest,TestGetNumberOfMemoryBuffers) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(GAMDataSourceGTest,TestGetSignalMemoryBuffer) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(GAMDataSourceGTest,TestAllocateMemory) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(GAMDataSourceGTest,TestGetBrokerName) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(GAMDataSourceGTest,TestGetInputBrokers) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(GAMDataSourceGTest,TestGetOutputBrokers) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(GAMDataSourceGTest,TestSetConfiguredDatabase) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(GAMDataSourceGTest,TestSetConfiguredDatabase_MoreThanOneProducer_State_1) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_MoreThanOneProducer_State_1());
}

TEST(GAMDataSourceGTest,TestSetConfiguredDatabase_MoreThanOneProducer_State_2) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_MoreThanOneProducer_State_2());
}

TEST(GAMDataSourceGTest,TestSetConfiguredDatabase_NoProducer_State_1) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_NoProducer_State_1());
}

TEST(GAMDataSourceGTest,TestSetConfiguredDatabase_NoProducer_State_2) {
    GAMDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_NoProducer_State_2());
}
