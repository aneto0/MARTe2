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
        thread0Id = 0u;
        thread1Id = 0u;
        thread2Id = 0u;
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(const MARTe::ExecutionInfo &information) {
        if (thread0Id == 0u) {
            thread0Id = information.GetThreadNumber();
        }
        if (thread1Id == 0u) {
            if (thread0Id != information.GetThreadNumber()) {
                if (thread2Id != information.GetThreadNumber()) {
                    thread1Id = information.GetThreadNumber();
                }
            }
        }
        if (thread2Id == 0u) {
            if (thread0Id != information.GetThreadNumber()) {
                if (thread1Id != information.GetThreadNumber()) {
                    thread2Id = information.GetThreadNumber();
                }
            }
        }
        if (information.GetThreadNumber() == thread0Id) {
            internalStateThread0++;
        }
        if (information.GetThreadNumber() == thread1Id) {
            internalStateThread1++;
        }
        if (information.GetThreadNumber() == thread2Id) {
            internalStateThread2++;
        }
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::uint32 internalStateThread0;
    MARTe::uint32 internalStateThread1;
    MARTe::uint32 internalStateThread2;
    MARTe::ThreadIdentifier thread0Id;
    MARTe::ThreadIdentifier thread1Id;
    MARTe::ThreadIdentifier thread2Id;
};

class MultiThreadServiceTestCallbackClassToKill {
public:
    MultiThreadServiceTestCallbackClassToKill() {
        internalState = 0u;
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(const MARTe::ExecutionInfo &information) {
        if (information.GetStage() == MARTe::ExecutionInfo::MainStage) {
            if (information.GetThreadNumber() > internalState) {
                internalState++;
            }
            while (1) {
                MARTe::Sleep::Sec(1.0);
            }
        }
        if (information.GetStage() == MARTe::ExecutionInfo::AsyncTerminationStage) {
            internalState--;
        }
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::uint32 internalState;
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MultiThreadServiceTest::MultiThreadServiceTest() {
}

MultiThreadServiceTest::~MultiThreadServiceTest() {
}

bool MultiThreadServiceTest::TestDefaultConstructor() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    EmbeddedServiceMethodBinderI &binderI = binder;

    MultiThreadService multiThreadService(binderI);
    bool ok = (multiThreadService.GetNumberOfPoolThreads() == 1u);
    return ok;
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
    ok &= (embeddedThread.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedThreadI::RunningState);

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
    ok &= (embeddedThread.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedThreadI::RunningState);

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
    ok &= (embeddedThread.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedThreadI::RunningState);

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
    ok &= (embeddedThread.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedThreadI::RunningState);
    embeddedThread.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestStop() {
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
    ok &= (embeddedThread.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedThreadI::RunningState);

    err = embeddedThread.Stop();

    ok &= (err == ErrorManagement::NoError);
    ok &= (embeddedThread.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedThreadI::OffState);

    return ok;
}

bool MultiThreadServiceTest::TestStop_Kill() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClassToKill callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClassToKill> binder(callbackClass, &MultiThreadServiceTestCallbackClassToKill::CallbackFunction);
    MultiThreadService embeddedThread(binder);

    embeddedThread.SetNumberOfPoolThreads(3);

    ErrorManagement::ErrorType err = embeddedThread.Start();
    bool ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState != 3u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState == 3u);
    ok &= (embeddedThread.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedThreadI::RunningState);

    embeddedThread.SetTimeout(1000);
    err = embeddedThread.Stop();
    maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState != 0u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }

    ok &= (err == ErrorManagement::NoError);
    ok &= (embeddedThread.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedThreadI::OffState);
    ok &= (callbackClass.internalState == 0u);
    return ok;
}

bool MultiThreadServiceTest::TestGetStatus() {
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
    ok &= (embeddedThread.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedThreadI::RunningState);

    embeddedThread.Stop();
    ok &= (embeddedThread.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (embeddedThread.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (embeddedThread.GetStatus(2) == EmbeddedThreadI::OffState);

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

bool MultiThreadServiceTest::TestSetNumberOfPoolThreads() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    embeddedThread.SetNumberOfPoolThreads(100);
    bool ok = (embeddedThread.GetNumberOfPoolThreads() == 100);
    return ok;
}

bool MultiThreadServiceTest::TestGetNumberOfPoolThreads() {
    return TestSetNumberOfPoolThreads();
}
