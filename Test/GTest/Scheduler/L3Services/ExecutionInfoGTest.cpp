/**
 * @file ExecutionInfoGTest.cpp
 * @brief Source file for class ExecutionInfoGTest
 * @date 19/09/2016
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
 * the class MessageGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "ExecutionInfoTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ExecutionInfoGTest,TestConstructor) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(ExecutionInfoGTest,TestSetThreadNumber) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestSetThreadNumber());
}

TEST(ExecutionInfoGTest,TestSetThreadNumber_StageNotStartup) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestSetThreadNumber_StageNotStartup());
}

TEST(ExecutionInfoGTest,TestSetStage) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestSetStage());
}

TEST(ExecutionInfoGTest,TestSetStageSpecific) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestSetStageSpecific());
}

TEST(ExecutionInfoGTest,TestGetThreadNumber) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestGetThreadNumber());
}

TEST(ExecutionInfoGTest,TestGetStage) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestGetStage());
}

TEST(ExecutionInfoGTest,TestGetStageSpecific) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestGetStageSpecific());
}

TEST(ExecutionInfoGTest,TestReset) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestReset());
}
