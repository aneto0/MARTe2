/**
 * @file BasicRealTimeDataSourceDefGTest.cpp
 * @brief Source file for class BasicRealTimeDataSourceDefGTest
 * @date 30 Mar 2016 TODO Verify the value and format of the date
 * @author perezfr TODO Verify the name and format of the author
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
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestConstructor());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAddConsumer) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestAddConsumer());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAddProducer) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestAddProducer());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetNumberOfConsumers) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestGetNumberOfConsumers());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetNumberOfProducers) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestGetNumberOfProducers());
}

TEST(BasicRealTimeDataSourceDefGTest,TestVerifyTrue) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestVerifyTrue());
}

TEST(BasicRealTimeDataSourceDefGTest,TestVerifyFalse_MoreThanOneProducer) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestVerifyFalse_MoreThanOneProducer());
}

TEST(BasicRealTimeDataSourceDefGTest,TestVerifyNoConsumers) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestVerifyNoConsumers());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetType) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestGetType());
}

TEST(BasicRealTimeDataSourceDefGTest,TestSetType) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestSetType());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocate_Basic) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestAllocate_Basic());
}

TEST(BasicRealTimeDataSourceDefGTest,TestSetType_WarningAlreadySet) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestSetType_WarningAlreadySet());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocate_Structured) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestAllocate_Structured());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocate_Unintrospectable) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestAllocate_Unintrospectable());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocateFalse_Invalid) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestAllocateFalse_Invalid());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocateMultiDim_Vector) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestAllocateMultiDim_Vector());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocateMultiDim_Matrix) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestAllocateMultiDim_Matrix());
}

TEST(BasicRealTimeDataSourceDefGTest,TestAllocateFalse_MultiDimStructured) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestAllocateFalse_MultiDimStructured());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetDataSourcePointer) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestGetDataSourcePointer());
}

TEST(BasicRealTimeDataSourceDefGTest,TestPrepareNextState_Basic_ContinueVar) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestPrepareNextState_Basic_ContinueVar());
}

TEST(BasicRealTimeDataSourceDefGTest,TestPrepareNextState_Basic_DeadVar) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestPrepareNextState_Basic_DeadVar());
}

TEST(BasicRealTimeDataSourceDefGTest,TestPrepareNextState_Structured_ContinueVar) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestPrepareNextState_Structured_ContinueVar());
}

TEST(BasicRealTimeDataSourceDefGTest,TestPrepareNextState_Structured_DeadVar) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestPrepareNextState_Structured_DeadVar());
}

TEST(BasicRealTimeDataSourceDefGTest,TestPrepareNextStateMultiDimensional) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestPrepareNextStateMultiDimensional());
}

TEST(BasicRealTimeDataSourceDefGTest,TestSetDefaultValue) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestSetDefaultValue());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetDefaultValue) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestGetDefaultValue());
}

TEST(BasicRealTimeDataSourceDefGTest,TestSetNumberOfElements) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestSetNumberOfElements());
}

TEST(BasicRealTimeDataSourceDefGTest,TestSetNumberOfDimensions) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestSetNumberOfDimensions());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetNumberOfElements) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestGetNumberOfElements());
}

TEST(BasicRealTimeDataSourceDefGTest,TestGetNumberOfDimensions) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestGetNumberOfDimensions());
}

TEST(BasicRealTimeDataSourceDefGTest,TestToStructuredData) {
    BasicRealTimeDataSourceDefTest myBasicRealTimeDataSourceDef;
    ASSERT_TRUE(myBasicRealTimeDataSourceDef.TestToStructuredData());
}
