/**
 * @file BasicRealTimeDataSourceDefGTest.cpp
 * @brief Source file for class BasicRealTimeDataSourceDefGTest
 * @date 22/mar/2016
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
 * the class BasicRealTimeDataSourceDefGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/





#include <limits.h>
#include "gtest/gtest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicRealTimeDataSourceDefTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BasicRealTimeDataSourceDefGTest,TestConstructor) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestConstructor());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAddConsumer) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestAddConsumer());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAddProducer) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestAddProducer());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetNumberOfConsumers) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestGetNumberOfConsumers());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetNumberOfProducers) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestGetNumberOfProducers());
}

TEST(BasicRealTimeDataSourceDefGTest,TestVerifyTrue) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestVerifyTrue());
}

TEST(BasicRealTimeDataSourceDefGTest,TestVerifyFalse_MoreThanOneProducer) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestVerifyFalse_MoreThanOneProducer());
}

TEST(BasicRealTimeDataSourceDefGTest,TestVerifyNoConsumers) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestVerifyNoConsumers());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetType) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestGetType());
}

TEST(BasicRealTimeDataSourceDefGTest,TestSetType) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestSetType());
}

TEST(BasicRealTimeDataSourceDefGTest,TestSetType_WarningAlreadySet) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestSetType_WarningAlreadySet());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocate_Basic) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestAllocate_Basic());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocate_Structured) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestAllocate_Structured());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocate_Unintrospectable) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestAllocate_Unintrospectable());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocateFalse_Invalid) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestAllocateFalse_Invalid());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocateMultiDim_Vector) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestAllocateMultiDim_Vector());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocateMultiDim_Matrix) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestAllocateMultiDim_Matrix());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocateFalse_MultiDimStructured) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestAllocateFalse_MultiDimStructured());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetDataSourcePointer) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestGetDataSourcePointer());
}

TEST(BasicRealTimeDataSourceDefGTest,TestPrepareNextState_Basic_ContinueVar) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestPrepareNextState_Basic_ContinueVar());
}

TEST(BasicRealTimeDataSourceDefGTest,TestPrepareNextState_Basic_DeadVar) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestPrepareNextState_Basic_DeadVar());
}

TEST(BasicRealTimeDataSourceDefGTest,TestPrepareNextState_Structured_ContinueVar) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestPrepareNextState_Structured_ContinueVar());
}

TEST(BasicRealTimeDataSourceDefGTest,TestPrepareNextState_Structured_DeadVar) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestPrepareNextState_Structured_DeadVar());
}

TEST(BasicRealTimeDataSourceDefGTest,TestPrepareNextStateMultiDimensional) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestPrepareNextStateMultiDimensional());
}

TEST(BasicRealTimeDataSourceDefGTest,TestSetDefaultValue) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestSetDefaultValue());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetDefaultValue) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestGetDefaultValue());
}

TEST(BasicRealTimeDataSourceDefGTest,TestSetNumberOfElements) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestSetNumberOfElements());
}

TEST(BasicRealTimeDataSourceDefGTest,TestSetNumberOfDimensions) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestSetNumberOfDimensions());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetNumberOfElements) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestGetNumberOfElements());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetNumberOfDimensions) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestGetNumberOfDimensions());
}

TEST(BasicRealTimeDataSourceDefGTest,TestToStructuredData) {
    BasicRealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestToStructuredData());
}
