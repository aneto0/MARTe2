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
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class EmbeddedThreadITestStub: public MARTe::EmbeddedThreadI {
public:
    EmbeddedThreadITestStub(MARTe::EmbeddedServiceMethodBinderI &binder) :
            MARTe::EmbeddedThreadI(binder) {
    }

    EmbeddedThreadITestStub(MARTe::EmbeddedServiceMethodBinderI &binder, MARTe::uint32 threadNumber) :
            MARTe::EmbeddedThreadI(binder, threadNumber) {
    }

    void ThreadLoop() {
        SetCommands(KeepRunningCommand);
        MARTe::ExecutionInfo info;
        info.SetStage(MARTe::ExecutionInfo::MainStage);
        Execute(info);
    }
};

class EmbeddedThreadITestStubStartCommand: public MARTe::EmbeddedThreadI {
public:
    EmbeddedThreadITestStubStartCommand(MARTe::EmbeddedServiceMethodBinderI &binder) :
            MARTe::EmbeddedThreadI(binder) {
    }

    void ThreadLoop() {
        SetCommands(StartCommand);
        MARTe::ExecutionInfo info;
        info.SetStage(MARTe::ExecutionInfo::StartupStage);
        Execute(info);
        while (1) {
            MARTe::Sleep::Sec(0.1);
        }
    }
};

class EmbeddedThreadITestCallbackClass {
public:
    EmbeddedThreadITestCallbackClass() {
        internalState = 0u;
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information) {
        internalState++;
        MARTe::Sleep::Sec(0.1);
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::uint32 internalState;
};

class EmbeddedThreadITestCallbackClassToKill {
public:
    EmbeddedThreadITestCallbackClassToKill() {
        internalState = 0u;
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information) {
        if (information.GetStage() == MARTe::ExecutionInfo::AsyncTerminationStage) {
            internalState--;
        }
        else {
            internalState++;
            while (1) {
                MARTe::Sleep::Sec(0.1);
            }
            return MARTe::ErrorManagement::FatalError;
        }
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::uint32 internalState;
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
    MARTe::Sleep::Sec(0.1);
    executeCalled = true;
    return MARTe::ErrorManagement::NoError;
}

bool EmbeddedThreadITest::TestDefaultConstructor() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ok &= (embeddedThreadI.GetCommands() == EmbeddedThreadI::StopCommand);
    ok &= (embeddedThreadI.GetPriorityClass() == Threads::NormalPriorityClass);
    ok &= (embeddedThreadI.GetPriorityLevel() == 0u);
    ok &= (embeddedThreadI.GetCPUMask() == UndefinedCPUs);
    return ok;
}

bool EmbeddedThreadITest::TestConstructorThreadNumber() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder, 5);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ok &= (embeddedThreadI.GetCommands() == EmbeddedThreadI::StopCommand);
    ok &= (embeddedThreadI.GetPriorityClass() == Threads::NormalPriorityClass);
    ok &= (embeddedThreadI.GetPriorityLevel() == 0u);
    ok &= (embeddedThreadI.GetCPUMask() == UndefinedCPUs);
    ok &= (embeddedThreadI.GetThreadNumber() == 5u);
    return ok;
}

bool EmbeddedThreadITest::TestGetThreadNumber() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder, 7);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ok &= (embeddedThreadI.GetCommands() == EmbeddedThreadI::StopCommand);
    ok &= (embeddedThreadI.GetPriorityClass() == Threads::NormalPriorityClass);
    ok &= (embeddedThreadI.GetPriorityLevel() == 0u);
    ok &= (embeddedThreadI.GetCPUMask() == UndefinedCPUs);
    ok &= (embeddedThreadI.GetThreadNumber() == 7u);
    return ok;
}

bool EmbeddedThreadITest::TestGetTimeout() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder, 7);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ok &= (embeddedThreadI.GetCommands() == EmbeddedThreadI::StopCommand);
    ok &= (embeddedThreadI.GetPriorityClass() == Threads::NormalPriorityClass);
    ok &= (embeddedThreadI.GetPriorityLevel() == 0u);
    ok &= (embeddedThreadI.GetCPUMask() == UndefinedCPUs);
    ok &= (embeddedThreadI.GetThreadNumber() == 7u);
    ok &= (embeddedThreadI.GetTimeout() == TTInfiniteWait);
    return ok;
}

bool EmbeddedThreadITest::TestGetThreadId() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    embeddedThreadI.Start();
    ok &= (embeddedThreadI.GetThreadId() != InvalidThreadIdentifier);
    uint32 counter = 10;
    while ((counter > 0) && (!executeCalled)) {
        Sleep::Sec(0.5);
        counter--;
    }
    //The thread must die before EmbeddedThreadITest::CallbackFunction gets out of scope!
    if (Threads::IsAlive(embeddedThreadI.GetThreadId())) {
        Threads::Kill(embeddedThreadI.GetThreadId());
    }
    return ok;
}

bool EmbeddedThreadITest::TestGetPriorityClass() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetPriorityClass() == Threads::NormalPriorityClass);
    embeddedThreadI.SetPriorityClass(Threads::RealTimePriorityClass);
    ok &= (embeddedThreadI.GetPriorityClass() == Threads::RealTimePriorityClass);
    return ok;
}

bool EmbeddedThreadITest::TestSetPriorityClass() {
    return TestGetPriorityClass();
}

bool EmbeddedThreadITest::TestGetPriorityLevel() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetPriorityLevel() == 0);
    embeddedThreadI.SetPriorityLevel(10);
    ok &= (embeddedThreadI.GetPriorityLevel() == 10);
    return ok;
}

bool EmbeddedThreadITest::TestSetStackSize() {
    return TestGetStackSize();
}

bool EmbeddedThreadITest::TestGetStackSize() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    embeddedThreadI.SetStackSize(THREADS_DEFAULT_STACKSIZE * 2);
    ok &= (embeddedThreadI.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 2));
    return ok;
}

bool EmbeddedThreadITest::TestSetPriorityLevel() {
    return TestGetPriorityLevel();
}

bool EmbeddedThreadITest::TestSetTimeout() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder, 7);
    bool ok = (embeddedThreadI.GetTimeout() == TTInfiniteWait);
    embeddedThreadI.SetTimeout(100);
    ok &= (embeddedThreadI.GetTimeout() == 100);
    embeddedThreadI.SetTimeout(0x80000000U);
    TimeoutType saturatedTimeout;
    saturatedTimeout.SetTimeoutHighResolutionTimerTicks(0x7FFFFFFFU);
    ok &= (embeddedThreadI.GetTimeout() == saturatedTimeout);
    return ok;
}

bool EmbeddedThreadITest::TestGetCPUMask() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetCPUMask() == UndefinedCPUs);
    embeddedThreadI.SetCPUMask(0x3);
    ok &= (embeddedThreadI.GetCPUMask() == 0x3);
    return ok;
}

bool EmbeddedThreadITest::TestSetCPUMask() {
    return TestGetCPUMask();
}

bool EmbeddedThreadITest::TestGetStatus() {
    using namespace MARTe;
    EmbeddedThreadITestCallbackClass callbackClass;
    //Easier to test through the SingleThreadService
    EmbeddedServiceMethodBinderT<EmbeddedThreadITestCallbackClass> binder(callbackClass, &EmbeddedThreadITestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    bool ok = (service.GetStatus() == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 1u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 1u);
    ok &= (service.GetStatus() == EmbeddedThreadI::RunningState);
    service.Stop();
    ok &= (service.GetStatus() == EmbeddedThreadI::OffState);
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
    embeddedThreadI.Start();
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
    return TestGetStatus();
}

bool EmbeddedThreadITest::TestStart() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ErrorManagement::ErrorType err = embeddedThreadI.Start();
    ok &= (err == ErrorManagement::NoError);
    ok &= (embeddedThreadI.GetThreadId() != InvalidThreadIdentifier);
    uint32 counter = 10;
    while ((counter > 0) && (!executeCalled)) {
        Sleep::Sec(0.5);
        counter--;
    }
    ok &= executeCalled;
    embeddedThreadI.Stop();
    return ok;
}

bool EmbeddedThreadITest::TestStart_False() {
    using namespace MARTe;
    EmbeddedThreadITestCallbackClassToKill callbackClass;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITestCallbackClassToKill> binder(callbackClass, &EmbeddedThreadITestCallbackClassToKill::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    embeddedThreadI.SetTimeout(1000);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ErrorManagement::ErrorType err = embeddedThreadI.Start();
    ok &= (err == ErrorManagement::NoError);
    ok &= (embeddedThreadI.GetThreadId() != InvalidThreadIdentifier);
    uint32 counter = 10;
    while ((counter > 0) && (!callbackClass.internalState)) {
        Sleep::Sec(0.5);
        counter--;
    }
    err = embeddedThreadI.Start();
    ok &= (err == ErrorManagement::IllegalOperation);

    embeddedThreadI.Stop();
    //kill the thread otherwise it will stay alive and will stop other tests that depend on while(Threads::NumberOfThreads() > 0)
    embeddedThreadI.Stop();
    return ok;
}

bool EmbeddedThreadITest::TestStart_Restart() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ErrorManagement::ErrorType err = embeddedThreadI.Start();
    ok &= (err == ErrorManagement::NoError);
    ok &= (embeddedThreadI.GetThreadId() != InvalidThreadIdentifier);
    uint32 counter = 10;
    while ((counter > 0) && (!executeCalled)) {
        Sleep::Sec(0.5);
        counter--;
    }
    ok &= executeCalled;
    embeddedThreadI.Stop();
    executeCalled = false;
    err = embeddedThreadI.Start();
    counter = 10;
    while ((counter > 0) && (!executeCalled)) {
        Sleep::Sec(0.5);
        counter--;
    }
    ok &= executeCalled;
    embeddedThreadI.Stop();
    return ok;
}

bool EmbeddedThreadITest::TestStop() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ErrorManagement::ErrorType err = embeddedThreadI.Start();
    ok &= (err == ErrorManagement::NoError);
    ok &= (embeddedThreadI.GetThreadId() != InvalidThreadIdentifier);
    uint32 counter = 10;
    while ((counter > 0) && (!executeCalled)) {
        Sleep::Sec(0.5);
        counter--;
    }
    ok &= executeCalled;
    err = embeddedThreadI.Stop();
    ok &= (err == ErrorManagement::NoError);
    return ok;
}

bool EmbeddedThreadITest::TestStop_Kill() {
    using namespace MARTe;
    EmbeddedThreadITestCallbackClassToKill callbackClass;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITestCallbackClassToKill> binder(callbackClass, &EmbeddedThreadITestCallbackClassToKill::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    embeddedThreadI.SetTimeout(1000);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ErrorManagement::ErrorType err = embeddedThreadI.Start();
    ok &= (err == ErrorManagement::NoError);
    ok &= (embeddedThreadI.GetThreadId() != InvalidThreadIdentifier);

    uint32 counter = 10;
    while ((counter > 0) && (callbackClass.internalState != 1u)) {
        Sleep::Sec(0.5);
        counter--;
    }
    ok &= (callbackClass.internalState == 1u);
    err = embeddedThreadI.Stop();
    ok &= (err == ErrorManagement::Timeout);
    err = embeddedThreadI.Stop();

    while ((counter > 0) && (callbackClass.internalState != 0u)) {
        Sleep::Sec(0.5);
        counter--;
    }
    ok &= (callbackClass.internalState == 0u);
    ok &= (err == ErrorManagement::NoError);
    return ok;
}

bool EmbeddedThreadITest::TestStart_StartCommand() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStubStartCommand embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ErrorManagement::ErrorType err = embeddedThreadI.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 counter = 10;
    while ((counter > 0) && (!executeCalled)) {
        Sleep::Sec(0.1);
    }
    ok &= (embeddedThreadI.GetStatus() == EmbeddedThreadI::StartingState);
    embeddedThreadI.SetTimeout(1);
    embeddedThreadI.Stop();
    embeddedThreadI.Stop();
    return ok;
}

bool EmbeddedThreadITest::TestStart_StartCommandTimeout() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITest> binder(*this, &EmbeddedThreadITest::CallbackFunction);
    EmbeddedThreadITestStubStartCommand embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    embeddedThreadI.SetTimeout(1);
    ErrorManagement::ErrorType err = embeddedThreadI.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 counter = 10;
    while ((counter > 0) && (!executeCalled)) {
        Sleep::Sec(0.1);
    }
    ok &= (embeddedThreadI.GetStatus() == EmbeddedThreadI::TimeoutStartingState);
    embeddedThreadI.Stop();
    embeddedThreadI.Stop();
    return ok;
}
