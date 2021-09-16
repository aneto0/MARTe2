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

#include "MARTe2UTest.h"
#include "ExecutionInfoTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(Scheduler_L3Services_ExecutionInfoGTest,TestConstructor) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(Scheduler_L3Services_ExecutionInfoGTest,TestSetThreadNumber) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestSetThreadNumber());
}

TEST(Scheduler_L3Services_ExecutionInfoGTest,TestSetThreadNumber_StageNotStartup) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestSetThreadNumber_StageNotStartup());
}

TEST(Scheduler_L3Services_ExecutionInfoGTest,TestSetStage) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestSetStage());
}

TEST(Scheduler_L3Services_ExecutionInfoGTest,TestSetStageSpecific) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestSetStageSpecific());
}

TEST(Scheduler_L3Services_ExecutionInfoGTest,TestGetThreadNumber) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestGetThreadNumber());
}

TEST(Scheduler_L3Services_ExecutionInfoGTest,TestGetStage) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestGetStage());
}

TEST(Scheduler_L3Services_ExecutionInfoGTest,TestGetStageSpecific) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestGetStageSpecific());
}

TEST(Scheduler_L3Services_ExecutionInfoGTest,TestReset) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestReset());
}

TEST(Scheduler_L3Services_ExecutionInfoGTest,TestSetThreadSpecificContext) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestSetThreadSpecificContext());
}

TEST(Scheduler_L3Services_ExecutionInfoGTest,TestGetThreadSpecificContext) {
    ExecutionInfoTest target;
    ASSERT_TRUE(target.TestGetThreadSpecificContext());
}

