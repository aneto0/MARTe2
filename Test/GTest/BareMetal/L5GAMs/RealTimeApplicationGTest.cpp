/**
 * @file RealTimeApplicationGTest.cpp
 * @brief Source file for class RealTimeApplicationGTest
 * @date 01/03/2016
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
 * the class RealTimeApplicationGTest (public, protected, and private). Be aware that some 
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
#include "RealTimeApplicationTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeApplicationGTest,TestConstructor) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestConstructor());
}

TEST(RealTimeApplicationGTest,TestInitialise) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestInitialise());
}

TEST(RealTimeApplicationGTest,TestConfigureArchitecture) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestConfigureArchitecture());
}

TEST(RealTimeApplicationGTest,TestConfigureArchitectureFalse_NoStates) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestConfigureArchitectureFalse_NoStates());
}

TEST(RealTimeApplicationGTest,TestConfigureDataSource) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestConfigureDataSource());
}

TEST(RealTimeApplicationGTest,TestConfigureDataSourceFalse_NoFunctions) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestConfigureDataSourceFalse_NoFunctions());
}

TEST(RealTimeApplicationGTest,TestConfigureDataSourceFalse_InvalidFunction) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestConfigureDataSourceFalse_InvalidFunction());
}

TEST(RealTimeApplicationGTest,TestValidateDataSource) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestValidateDataSource());
}

TEST(RealTimeApplicationGTest,TestValidateDataSourceFalse_NoData) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestValidateDataSourceFalse_NoData());
}

TEST(RealTimeApplicationGTest,TestValidateDataSourceFalse_MoreThanOneProducer) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestValidateDataSourceFalse_MoreThanOneProducer());
}

TEST(RealTimeApplicationGTest,TestAllocateDataSource) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestAllocateDataSource());
}

TEST(RealTimeApplicationGTest,TestAllocateDataSourceFalse_NoData) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestAllocateDataSourceFalse_NoData());
}

TEST(RealTimeApplicationGTest,TestConfigureDataSourceLinks) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestConfigureDataSourceLinks());
}

TEST(RealTimeApplicationGTest,TestConfigureDataSourceLinksFalse_NoFunctions) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestConfigureDataSourceLinksFalse_NoFunctions());
}

TEST(RealTimeApplicationGTest,TestConfigureDataSourceLinksFalse_InvalidFunction) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestConfigureDataSourceLinksFalse_InvalidFunction());
}

TEST(RealTimeApplicationGTest,TestPrepareNextState) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestPrepareNextState());
}

TEST(RealTimeApplicationGTest,TestPrepareNextStateFalse_NoData) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestPrepareNextStateFalse_NoData());
}

TEST(RealTimeApplicationGTest,TestGetActiveBuffer) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestGetActiveBuffer());
}

TEST(RealTimeApplicationGTest,TestValidateDataSourceLinks) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestValidateDataSourceLinks());
}

TEST(RealTimeApplicationGTest,TestValidateDataSourceLinksFalse_MoreSync) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestValidateDataSourceLinksFalse_MoreSync());
}

TEST(RealTimeApplicationGTest,TestStopExecution) {
    RealTimeApplicationTest rtappTest;
    ASSERT_TRUE(rtappTest.TestStopExecution());
}
