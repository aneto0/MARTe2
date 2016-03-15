/**
 * @file RealTimeDataSourceBrokerGTest.cpp
 * @brief Source file for class RealTimeDataSourceBrokerGTest
 * @date 11/03/2016
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
 * the class RealTimeDataSourceBrokerGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeDataSourceBrokerTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeDataSourceBrokerGTest,TestConstructor) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestConstructor());
}

TEST(RealTimeDataSourceBrokerGTest,TestSetApplication) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestSetApplication());
}

TEST(RealTimeDataSourceBrokerGTest,TestAddVariable_Allocation) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestAddVariable_Allocation());
}

TEST(RealTimeDataSourceBrokerGTest,TestAddVariable_Static) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestAddVariable_Static());
}

TEST(RealTimeDataSourceBrokerGTest,TestAddVariableFalse_NoApplicationSet) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestAddVariableFalse_NoApplicationSet());
}

TEST(RealTimeDataSourceBrokerGTest,TestAddVariableFalse_InvalidType) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestAddVariableFalse_InvalidType());
}

TEST(RealTimeDataSourceBrokerGTest,TestAddVariableFalse_UnintrospectableType) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestAddVariableFalse_UnintrospectableType());
}

TEST(RealTimeDataSourceBrokerGTest,TestAddVariableFalse_NotInDataSource) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestAddVariableFalse_NotInDataSource());
}

TEST(RealTimeDataSourceBrokerGTest,TestAddVariable_MultiDimensional_Static_Vector) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestAddVariable_MultiDimensional_Static_Vector());
}

TEST(RealTimeDataSourceBrokerGTest,TestAddVariable_MultiDimensional_Allocation_Vector) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestAddVariable_MultiDimensional_Allocation_Vector());
}

TEST(RealTimeDataSourceBrokerGTest,TestAddVariable_MultiDimensional_Allocation_Matrix) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestAddVariable_MultiDimensional_Allocation_Matrix());
}

TEST(RealTimeDataSourceBrokerGTest,TestAddVariable_MultiDimensional_Static_Matrix) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestAddVariable_MultiDimensional_Static_Matrix());
}

TEST(RealTimeDataSourceBrokerGTest,TestGetData_Allocation) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestGetData_Allocation());
}

TEST(RealTimeDataSourceBrokerGTest,TestGetData_Static) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestGetData_Static());
}

TEST(RealTimeDataSourceBrokerGTest,TestGetMemoryPointer_Allocation) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestGetMemoryPointer_Allocation());
}

TEST(RealTimeDataSourceBrokerGTest,TestGetMemoryPointer_Static) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestGetMemoryPointer_Static());
}

TEST(RealTimeDataSourceBrokerGTest,TestFinalise) {
    RealTimeDataSourceBrokerTest rtdsbTest;
    ASSERT_TRUE(rtdsbTest.TestFinalise());
}
