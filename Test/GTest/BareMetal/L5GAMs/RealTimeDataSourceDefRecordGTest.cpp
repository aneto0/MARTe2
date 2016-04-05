/**
 * @file RealTimeDataSourceDefRecordGTest.cpp
 * @brief Source file for class RealTimeDataSourceDefRecordGTest
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
 * the class RealTimeDataSourceDefRecordGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeDataSourceDefRecordTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeDataSourceDefRecordGTest,TestConstructor) {
    RealTimeDataSourceDefRecordTest rtsdrTest;
    ASSERT_TRUE(rtsdrTest.TestConstructor());
}

TEST(RealTimeDataSourceDefRecordGTest,TestSetStateName) {
    RealTimeDataSourceDefRecordTest rtsdrTest;
    ASSERT_TRUE(rtsdrTest.TestSetStateName("state"));
}

TEST(RealTimeDataSourceDefRecordGTest,TestSetStateName_NULL) {
    RealTimeDataSourceDefRecordTest rtsdrTest;
    ASSERT_TRUE(rtsdrTest.TestSetStateName(NULL));
}

TEST(RealTimeDataSourceDefRecordGTest,TestGetStateName) {
    RealTimeDataSourceDefRecordTest rtsdrTest;
    ASSERT_TRUE(rtsdrTest.TestGetStateName("state"));
}

TEST(RealTimeDataSourceDefRecordGTest,TestGetStateName_NULL) {
    RealTimeDataSourceDefRecordTest rtsdrTest;
    ASSERT_TRUE(rtsdrTest.TestGetStateName(NULL));
}

TEST(RealTimeDataSourceDefRecordGTest,TestAddConsumer) {
    RealTimeDataSourceDefRecordTest rtsdrTest;
    ASSERT_TRUE(rtsdrTest.TestAddConsumer());
}

TEST(RealTimeDataSourceDefRecordGTest,TestAddProducer) {
    RealTimeDataSourceDefRecordTest rtsdrTest;
    ASSERT_TRUE(rtsdrTest.TestAddProducer());
}

TEST(RealTimeDataSourceDefRecordGTest,TestGetNumberOfConsumers) {
    RealTimeDataSourceDefRecordTest rtsdrTest;
    ASSERT_TRUE(rtsdrTest.TestGetNumberOfConsumers(32));
}

TEST(RealTimeDataSourceDefRecordGTest,TestGetNumberOfProducers) {
    RealTimeDataSourceDefRecordTest rtsdrTest;
    ASSERT_TRUE(rtsdrTest.TestGetNumberOfProducers(32));
}

TEST(RealTimeDataSourceDefRecordGTest,TestGetConsumers) {
    RealTimeDataSourceDefRecordTest rtsdrTest;
    ASSERT_TRUE(rtsdrTest.TestGetConsumers());
}

TEST(RealTimeDataSourceDefRecordGTest,TestGetProducers) {
    RealTimeDataSourceDefRecordTest rtsdrTest;
    ASSERT_TRUE(rtsdrTest.TestGetProducers());
}

