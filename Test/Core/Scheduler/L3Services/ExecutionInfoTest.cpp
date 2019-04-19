/**
 * @file ExecutionInfoTest.cpp
 * @brief Source file for class ExecutionInfoTest
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
 * the class ExecutionInfoTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "EmbeddedServiceI.h"
#include "ExecutionInfoTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ExecutionInfoTest::ExecutionInfoTest() {
}

ExecutionInfoTest::~ExecutionInfoTest() {
}

bool ExecutionInfoTest::TestDefaultConstructor() {
    using namespace MARTe;
    ExecutionInfo info;
    bool ok = (info.GetThreadNumber() == 0xFFFFu);
    ok &= (info.GetStage() == ExecutionInfo::StartupStage);
    ok &= (info.GetStageSpecific() == ExecutionInfo::NullStageSpecific);
    return ok;
}

bool ExecutionInfoTest::TestSetThreadNumber() {
    using namespace MARTe;
    ExecutionInfo info;
    bool ok = (info.GetThreadNumber() == 0xFFFFu);
    info.SetThreadNumber(100);
    ok &= (info.GetThreadNumber() == 100u);
    return ok;
}

bool ExecutionInfoTest::TestSetThreadNumber_StageNotStartup() {
    using namespace MARTe;
    ExecutionInfo info;
    bool ok = (info.GetThreadNumber() == 0xFFFFu);
    info.SetThreadNumber(100);
    ok &= (info.GetThreadNumber() == 100u);
    info.SetStage(ExecutionInfo::MainStage);
    info.SetThreadNumber(200);
    ok &= (info.GetThreadNumber() == 100u);
    return ok;
}

bool ExecutionInfoTest::TestSetStage() {
    using namespace MARTe;
    ExecutionInfo info;
    bool ok = (info.GetStage() == ExecutionInfo::StartupStage);
    info.SetStage(ExecutionInfo::MainStage);
    ok &= (info.GetStage() == ExecutionInfo::MainStage);

    return ok;
}

bool ExecutionInfoTest::TestSetStageSpecific() {
    using namespace MARTe;
    ExecutionInfo info;
    bool ok = (info.GetStageSpecific() == ExecutionInfo::NullStageSpecific);
    info.SetStageSpecific(ExecutionInfo::WaitRequestStageSpecific);
    ok &= (info.GetStageSpecific() == ExecutionInfo::WaitRequestStageSpecific);
    return ok;
}

bool ExecutionInfoTest::TestGetThreadNumber() {
    return TestSetThreadNumber();
}

bool ExecutionInfoTest::TestGetStage() {
    return TestSetStage();
}

bool ExecutionInfoTest::TestGetStageSpecific() {
    return TestSetStageSpecific();
}

bool ExecutionInfoTest::TestReset() {
    using namespace MARTe;
    ExecutionInfo info;
    bool ok = (info.GetThreadNumber() == 0xFFFFu);
    ok &= (info.GetStage() == ExecutionInfo::StartupStage);
    ok &= (info.GetStageSpecific() == ExecutionInfo::NullStageSpecific);
    return ok;
}

bool ExecutionInfoTest::TestSetThreadSpecificContext() {
    using namespace MARTe;
    ExecutionInfo info;
    void *ptr = (void *)0xAABBCCDD;
    info.SetThreadSpecificContext(ptr);
    void *ptr2 = info.GetThreadSpecificContext();
    bool ok = (ptr == ptr2);
    return ok;
}

bool ExecutionInfoTest::TestGetThreadSpecificContext() {
    return TestSetThreadSpecificContext();
}
