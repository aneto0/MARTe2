/**
 * @file EmbeddedThreadITest.cpp
 * @brief Source file for class EmbeddedThreadITest
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
 * the class EmbeddedThreadITest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "EmbeddedThreadI.h"
#include "EmbeddedThreadITest.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EmbeddedServiceMethodBinderT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/\

class EmbeddedThreadITestStub: public MARTe::EmbeddedThreadI {
public:
    EmbeddedThreadITestStub(MARTe::EmbeddedServiceMethodBinderI &binder) :
            MARTe::EmbeddedThreadI(binder) {
    }

    void ThreadLoop() {
        commands = KeepRunningCommand;
        MARTe::ExecutionInfo info;
        info.SetStage(MARTe::ExecutionInfo::MainStage);
        Execute(info);
    }
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

EmbeddedThreadITest::EmbeddedThreadITest() {
    executeCalled = false;
}

EmbeddedThreadITest::~EmbeddedThreadITest() {
}

MARTe::ErrorManagement::ErrorType EmbeddedThreadITest::CallbackFunction(MARTe::ExecutionInfo &information) {
    executeCalled = true;
    MARTe::Sleep::Sec(0.1);
    return MARTe::ErrorManagement::NoError;
}

bool EmbeddedThreadITest::TestDefaultConstructor() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ok &= (embeddedThreadI.GetThreadNumber() == 0);
    ok &= (embeddedThreadI.GetCommands() == EmbeddedThreadI::StopCommand);
    return ok;
}

bool EmbeddedThreadITest::TestGetThreadNumber() {
    return TestSetThreadNumber();
}

bool EmbeddedThreadITest::TestSetThreadNumber() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    embeddedThreadI.SetThreadNumber(100);
    return (embeddedThreadI.GetThreadNumber() == 100);
}

bool EmbeddedThreadITest::TestGetThreadId() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    embeddedThreadI.LaunchThread();
    ok &= (embeddedThreadI.GetThreadId() != InvalidThreadIdentifier);
    //The thread must die before EmbeddedThreadITest::CallbackFunction gets out of scope!
    if (Threads::IsAlive(embeddedThreadI.GetThreadId())) {
        Threads::Kill(embeddedThreadI.GetThreadId());
    }
    return ok;
}

bool EmbeddedThreadITest::TestLaunchThread() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    embeddedThreadI.LaunchThread();
    ok &= (embeddedThreadI.GetThreadId() != InvalidThreadIdentifier);
    uint32 counter = 10;
    while ((counter > 0) && (!executeCalled)) {
        Sleep::Sec(0.5);
        counter--;
    }
    ok &= executeCalled;
    //The thread must die before EmbeddedThreadITest::CallbackFunction gets out of scope!
    if (Threads::IsAlive(embeddedThreadI.GetThreadId())) {
        Threads::Kill(embeddedThreadI.GetThreadId());
    }
    return ok;
}

bool EmbeddedThreadITest::TestGetCommands() {
    return TestSetCommands();
}

bool EmbeddedThreadITest::TestSetCommands() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    embeddedThreadI.SetCommands(EmbeddedThreadI::KeepRunningCommand);
    bool ok = (embeddedThreadI.GetCommands() == EmbeddedThreadI::KeepRunningCommand);
    embeddedThreadI.SetCommands(EmbeddedThreadI::KillCommand);
    ok = (embeddedThreadI.GetCommands() == EmbeddedThreadI::KillCommand);
    embeddedThreadI.SetCommands(EmbeddedThreadI::StartCommand);
    ok = (embeddedThreadI.GetCommands() == EmbeddedThreadI::StartCommand);
    embeddedThreadI.SetCommands(EmbeddedThreadI::StopCommand);
    ok = (embeddedThreadI.GetCommands() == EmbeddedThreadI::StopCommand);
    return ok;
}

bool EmbeddedThreadITest::TestResetThreadId() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    embeddedThreadI.LaunchThread();
    ok &= (embeddedThreadI.GetThreadId() != InvalidThreadIdentifier);
    uint32 counter = 10;
    while ((counter > 0) && (!executeCalled)) {
        Sleep::Sec(0.5);
        counter--;
    }

    embeddedThreadI.ResetThreadId();
    ok &= (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    //The thread must die before EmbeddedThreadITest::CallbackFunction gets out of scope!
    if (Threads::IsAlive(embeddedThreadI.GetThreadId())) {
        Threads::Kill(embeddedThreadI.GetThreadId());
    }
    return ok;
}

bool EmbeddedThreadITest::TestExecute() {
    return TestLaunchThread();
}
