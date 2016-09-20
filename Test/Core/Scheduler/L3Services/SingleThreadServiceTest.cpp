/**
 * @file SingleThreadServiceTest.cpp
 * @brief Source file for class SingleThreadServiceTest
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
 * the class SingleThreadServiceTest (public, protected, and private). Be aware that some
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
#include "SingleThreadServiceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class SingleThreadServiceTestCallbackClass {
public:
    SingleThreadServiceTestCallbackClass() {
        internalState = 0u;
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information) {
        internalState++;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::uint32 internalState;
};

class SingleThreadServiceTestCallbackClassToKill {
public:
    SingleThreadServiceTestCallbackClassToKill() {
        internalState = 0u;
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information) {
        internalState = 1;
        if (information.GetStage() == MARTe::ExecutionInfo::MainStage) {
            while (1) {
                MARTe::Sleep::Sec(1.0);
            }
        }
        if (information.GetStage() == MARTe::ExecutionInfo::AsyncTerminationStage) {
            internalState = 15;
        }
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::uint32 internalState;
};

class SingleThreadServiceTestCallbackClassStage {
public:
    SingleThreadServiceTestCallbackClassStage() {
        completed = false;
        main = false;
        badTermination = false;
        startup = false;
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information) {
        if (information.GetStage() == MARTe::ExecutionInfo::StartupStage) {
            startup = true;
        }
        if (information.GetStage() == MARTe::ExecutionInfo::MainStage) {
            main = true;
            if (!completed) {
                return MARTe::ErrorManagement::Completed;
            }
            else {
                return MARTe::ErrorManagement::FatalError;
            }
        }
        if (information.GetStage() == MARTe::ExecutionInfo::TerminationStage) {
            completed = true;
        }
        if (information.GetStage() == MARTe::ExecutionInfo::BadTerminationStage) {
            badTermination = true;
        }
        return MARTe::ErrorManagement::NoError;
    }

    bool main;
    bool completed;
    bool badTermination;
    bool startup;
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

SingleThreadServiceTest::SingleThreadServiceTest() {
}

SingleThreadServiceTest::~SingleThreadServiceTest() {
}

bool SingleThreadServiceTest::TestDefaultConstructor() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binderT(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    EmbeddedServiceMethodBinderI &binder = binderT;

    SingleThreadService embeddedThread(binder);
    bool ok = (embeddedThread.GetTimeout() == TTInfiniteWait);
    ok &= (embeddedThread.GetThreadId() == 0);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);
    return ok;
}

bool SingleThreadServiceTest::TestDefaultConstructor_Template() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService embeddedThread(binder);
    bool ok = (embeddedThread.GetTimeout() == TTInfiniteWait);
    ok &= (embeddedThread.GetThreadId() == 0);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);
    return ok;
}

bool SingleThreadServiceTest::TestInitialise() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService embeddedThread(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    bool ok = embeddedThread.Initialise(config);
    ok &= (embeddedThread.GetTimeout() == 10);
    return ok;
}

bool SingleThreadServiceTest::TestInitialise_False() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService embeddedThread(binder);

    ConfigurationDatabase config;
    config.Write("Timeot", 10);

    return !embeddedThread.Initialise(config);
}

bool SingleThreadServiceTest::TestStart() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService embeddedThread(binder);

    bool ok = (embeddedThread.GetThreadId() == 0);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);

    ErrorManagement::ErrorType err = embeddedThread.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 10u);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetThreadId() != 0);

    embeddedThread.Stop();
    return ok;
}

bool SingleThreadServiceTest::TestStart_False() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService embeddedThread(binder);

    bool ok = (embeddedThread.GetThreadId() == 0);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);

    ErrorManagement::ErrorType err = embeddedThread.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 10u);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetThreadId() != 0);

    err = embeddedThread.Start();
    ok &= (err == ErrorManagement::IllegalOperation);

    embeddedThread.Stop();
    return ok;
}

bool SingleThreadServiceTest::TestStart_Restart() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService embeddedThread(binder);

    bool ok = (embeddedThread.GetThreadId() == 0);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);

    ErrorManagement::ErrorType err = embeddedThread.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 10u);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetThreadId() != 0);

    embeddedThread.Stop();

    callbackClass.internalState = 0u;
    err = embeddedThread.Start();
    ok &= (err == ErrorManagement::NoError);
    maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 10u);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetThreadId() != 0);
    return ok;
}

bool SingleThreadServiceTest::TestStop() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService embeddedThread(binder);

    bool ok = (embeddedThread.GetThreadId() == 0);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);

    ErrorManagement::ErrorType err = embeddedThread.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 10u);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetThreadId() != 0);

    err = embeddedThread.Stop();
    ok &= (err == ErrorManagement::NoError);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);
    ok &= (embeddedThread.GetThreadId() == 0);

    return ok;
}

bool SingleThreadServiceTest::TestStop_Kill() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClassToKill callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClassToKill> binder(callbackClass, &SingleThreadServiceTestCallbackClassToKill::CallbackFunction);
    SingleThreadService embeddedThread(binder);

    bool ok = (embeddedThread.GetThreadId() == 0);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);

    ErrorManagement::ErrorType err = embeddedThread.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState == 0u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState == 1u);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetThreadId() != 0);

    embeddedThread.SetTimeout(1000);
    err = embeddedThread.Stop();

    ok &= (err == ErrorManagement::Timeout);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::TimeoutStoppingState);
    ok &= (embeddedThread.GetThreadId() != 0);

    err = embeddedThread.Stop();

    ok &= (err == ErrorManagement::NoError);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);
    ok &= (embeddedThread.GetThreadId() == 0);
    ok &= (callbackClass.internalState == 15u);
    return ok;
}

bool SingleThreadServiceTest::TestThreadLoop() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClassStage callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClassStage> binder(callbackClass, &SingleThreadServiceTestCallbackClassStage::CallbackFunction);
    SingleThreadService embeddedThread(binder);

    bool ok = (embeddedThread.GetThreadId() == 0);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);

    ErrorManagement::ErrorType err = embeddedThread.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.badTermination != true)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.main && callbackClass.completed && callbackClass.startup && callbackClass.badTermination);

    embeddedThread.Stop();
    return ok;
}

bool SingleThreadServiceTest::TestGetStatus() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService embeddedThread(binder);

    bool ok = (embeddedThread.GetThreadId() == 0);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);

    ErrorManagement::ErrorType err = embeddedThread.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 10u);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::RunningState);
    embeddedThread.Stop();
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);
    return ok;
}

bool SingleThreadServiceTest::TestSetTimeout() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService embeddedThread(binder);
    embeddedThread.SetTimeout(100);
    bool ok = (embeddedThread.GetTimeout() == 100);
    embeddedThread.SetTimeout(TTInfiniteWait);
    ok &= (embeddedThread.GetTimeout() == TTInfiniteWait);
    return ok;
}

bool SingleThreadServiceTest::TestGetTimeout() {
    return TestSetTimeout();
}

bool SingleThreadServiceTest::TestGetThreadId() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService embeddedThread(binder);

    bool ok = (embeddedThread.GetThreadId() == 0);
    ok &= (embeddedThread.GetStatus() == EmbeddedServiceI::OffState);

    ErrorManagement::ErrorType err = embeddedThread.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (embeddedThread.GetThreadId() != 0);

    err = embeddedThread.Stop();
    ok &= (embeddedThread.GetThreadId() == 0);

    return ok;
}
