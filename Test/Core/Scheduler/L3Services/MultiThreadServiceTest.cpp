/**
 * @file MultiThreadServiceTest.cpp
 * @brief Source file for class MultiThreadServiceTest
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
 * the class MultiThreadServiceTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "EmbeddedServiceI.h"
#include "MultiThreadServiceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class MultiThreadServiceTestCallbackClass {
public:
    MultiThreadServiceTestCallbackClass() {
        internalStateThread0 = 0u;
        internalStateThread1 = 0u;
        internalStateThread2 = 0u;
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information) {
        if (information.GetThreadNumber() == 0) {
            internalStateThread0++;
        }
        if (information.GetThreadNumber() == 1) {
            internalStateThread1++;
        }
        if (information.GetThreadNumber() == 2) {
            internalStateThread2++;
        }
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::uint32 internalStateThread0;
    MARTe::uint32 internalStateThread1;
    MARTe::uint32 internalStateThread2;
};

class MultiThreadServiceTestCallbackClassToKill {
public:
    MultiThreadServiceTestCallbackClassToKill() {
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

class MultiThreadServiceTestCallbackClassStage {
public:
    MultiThreadServiceTestCallbackClassStage() {
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

MultiThreadServiceTest::MultiThreadServiceTest() {
}

MultiThreadServiceTest::~MultiThreadServiceTest() {
}

bool MultiThreadServiceTest::TestDefaultConstructor_Template() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);

    MultiThreadService multiThreadService(binder);
    bool ok = (multiThreadService.GetNumberOfPoolThreads() == 1u);
    return ok;
}

bool MultiThreadServiceTest::TestInitialise() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 20);
    bool ok = multiThreadService.Initialise(config);
    ok &= (multiThreadService.GetNumberOfPoolThreads() == 20);
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);

    ConfigurationDatabase config;
    config.Write("Timeot", 10);
    config.Write("NumberOfPoolThreads", 20);
    bool ok = !embeddedThread.Initialise(config);

    ConfigurationDatabase config2;
    config2.Write("Timeout", 10);
    config2.Write("NumberOfPoolThread", 20);
    ok &= !embeddedThread.Initialise(config2);

    ConfigurationDatabase config3;
    config3.Write("Timeot", 10);
    config3.Write("NumberOfPoolThread", 20);
    ok &= !embeddedThread.Initialise(config3);

    return ok;
}

bool MultiThreadServiceTest::TestStart() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);

    embeddedThread.SetNumberOfPoolThreads(3);
    ErrorManagement::ErrorType err = embeddedThread.Start();
    bool ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0)
            && ((callbackClass.internalStateThread0 < 10u) || (callbackClass.internalStateThread1 < 10u) || (callbackClass.internalStateThread2 < 10u))) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalStateThread0 >= 10u);
    ok &= (callbackClass.internalStateThread1 >= 10u);
    ok &= (callbackClass.internalStateThread2 >= 10u);
    ok &= (embeddedThread.GetStatus(0) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetThreadId(0) != 0);
    ok &= (embeddedThread.GetThreadId(1) != 0);
    ok &= (embeddedThread.GetThreadId(2) != 0);

    embeddedThread.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestStart_False() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);

    embeddedThread.SetNumberOfPoolThreads(3);
    ErrorManagement::ErrorType err = embeddedThread.Start();
    bool ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0)
            && ((callbackClass.internalStateThread0 < 10u) || (callbackClass.internalStateThread1 < 10u) || (callbackClass.internalStateThread2 < 10u))) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalStateThread0 >= 10u);
    ok &= (callbackClass.internalStateThread1 >= 10u);
    ok &= (callbackClass.internalStateThread2 >= 10u);
    ok &= (embeddedThread.GetStatus(0) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetThreadId(0) != 0);
    ok &= (embeddedThread.GetThreadId(1) != 0);
    ok &= (embeddedThread.GetThreadId(2) != 0);

    err = embeddedThread.Start();
    ok &= (err == ErrorManagement::IllegalOperation);

    embeddedThread.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestStart_Restart() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);

    embeddedThread.SetNumberOfPoolThreads(3);
    ErrorManagement::ErrorType err = embeddedThread.Start();
    bool ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0)
            && ((callbackClass.internalStateThread0 < 10u) || (callbackClass.internalStateThread1 < 10u) || (callbackClass.internalStateThread2 < 10u))) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalStateThread0 >= 10u);
    ok &= (callbackClass.internalStateThread1 >= 10u);
    ok &= (callbackClass.internalStateThread2 >= 10u);
    ok &= (embeddedThread.GetStatus(0) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetThreadId(0) != 0);
    ok &= (embeddedThread.GetThreadId(1) != 0);
    ok &= (embeddedThread.GetThreadId(2) != 0);

    embeddedThread.Stop();

    callbackClass.internalStateThread0 = 0u;
    callbackClass.internalStateThread1 = 0u;
    callbackClass.internalStateThread2 = 0u;
    err = embeddedThread.Start();
    ok &= (err == ErrorManagement::NoError);
    maxCounter = 10;
    while ((maxCounter > 0)
            && ((callbackClass.internalStateThread0 < 10u) || (callbackClass.internalStateThread1 < 10u) || (callbackClass.internalStateThread2 < 10u))) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalStateThread0 >= 10u);
    ok &= (callbackClass.internalStateThread1 >= 10u);
    ok &= (callbackClass.internalStateThread2 >= 10u);
    ok &= (embeddedThread.GetStatus(0) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedServiceI::RunningState);
    ok &= (embeddedThread.GetThreadId(0) != 0);
    ok &= (embeddedThread.GetThreadId(1) != 0);
    ok &= (embeddedThread.GetThreadId(2) != 0);
    return ok;
}
#if 0
bool MultiThreadServiceTest::TestStop() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);

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

bool MultiThreadServiceTest::TestStop_Kill() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClassToKill callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClassToKill> binder(callbackClass, &MultiThreadServiceTestCallbackClassToKill::CallbackFunction);
    MultiThreadService embeddedThread(binder);

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

bool MultiThreadServiceTest::TestThreadLoop() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClassStage callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClassStage> binder(callbackClass, &MultiThreadServiceTestCallbackClassStage::CallbackFunction);
    MultiThreadService embeddedThread(binder);

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

bool MultiThreadServiceTest::TestGetStatus() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);

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

bool MultiThreadServiceTest::TestSetTimeout() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    embeddedThread.SetTimeout(100);
    bool ok = (embeddedThread.GetTimeout() == 100);
    embeddedThread.SetTimeout(TTInfiniteWait);
    ok &= (embeddedThread.GetTimeout() == TTInfiniteWait);
    return ok;
}

bool MultiThreadServiceTest::TestGetTimeout() {
    return TestSetTimeout();
}

bool MultiThreadServiceTest::TestGetThreadId() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);

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
#endif
