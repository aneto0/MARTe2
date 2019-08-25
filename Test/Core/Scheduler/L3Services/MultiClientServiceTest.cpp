/**
 * @file MultiClientServiceTest.cpp
 * @brief Source file for class MultiClientServiceTest
 * @date 23/09/2016
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
 * the class MultiClientServiceTest (public, protected, and private). Be aware that some
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
#include "EventSem.h"
#include "ExecutionInfo.h"
#include "MultiClientServiceTest.h"

#include "../../../../Source/Core/Scheduler/L1Portability/MutexSem.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class MultiClientServiceTestCallbackClass {
public:
    MultiClientServiceTestCallbackClass() {
        connectionRequested = false;
        connectIsServed = false;
        numberConnectionsWaiting = 0u;
        numberConnectionsServing = 0u;
        mux.Create();
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information) {
        if (information.GetStageSpecific() == MARTe::ExecutionInfo::StartupStage) {
            mux.Lock();
            numberConnectionsWaiting++;
            mux.UnLock();
        }
        if (information.GetStageSpecific() == MARTe::ExecutionInfo::WaitRequestStageSpecific) {
            MARTe::Sleep::Sec(0.1);
            mux.Lock();
            if (connectionRequested) {
                connectionRequested = false;
                numberConnectionsWaiting--;
                numberConnectionsServing++;
                void *invalidPtr = (void *)0xAABBCCDD;
                information.SetThreadSpecificContext(invalidPtr);
                mux.UnLock();
                return MARTe::ErrorManagement::NoError;
            }
            mux.UnLock();
            return MARTe::ErrorManagement::Timeout;
        }
        if (information.GetStageSpecific() == MARTe::ExecutionInfo::ServiceRequestStageSpecific) {
            MARTe::Sleep::Sec(0.1);
            mux.Lock();
            if (connectIsServed) {
                connectIsServed = false;
                numberConnectionsServing--;
                mux.UnLock();
                return MARTe::ErrorManagement::Completed;
            }
            mux.UnLock();
            return MARTe::ErrorManagement::NoError;
        }
        return MARTe::ErrorManagement::NoError;
    }

    bool connectionRequested;
    bool connectIsServed;

    MARTe::MutexSem mux;
    MARTe::uint32 numberConnectionsWaiting;
    MARTe::uint32 numberConnectionsServing;

};

class MultiClientServiceTestCallbackClassToKill {
public:
    MultiClientServiceTestCallbackClassToKill() {
        internalState = 0u;
        done = false;
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information) {
        if (information.GetStage() == MARTe::ExecutionInfo::AsyncTerminationStage) {
            internalState--;
        }
        else {
            internalState++;
            while (!done) {
                MARTe::Sleep::Sec(1.0);
            }
            return MARTe::ErrorManagement::Completed;
        }

        return MARTe::ErrorManagement::NoError;
    }

    MARTe::uint32 internalState;
    bool done;
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MultiClientServiceTest::MultiClientServiceTest() {
}

MultiClientServiceTest::~MultiClientServiceTest() {
}

bool MultiClientServiceTest::TestDefaultConstructor() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    EmbeddedServiceMethodBinderI &binderI = binder;

    MultiClientService service(binderI);
    bool ok = (service.GetMinimumNumberOfPoolThreads() == 1u);
    ok &= (service.GetMaximumNumberOfPoolThreads() == 3u);
    return ok;
}

bool MultiClientServiceTest::TestDefaultConstructor_Template() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);

    MultiClientService service(binder);
    bool ok = (service.GetMinimumNumberOfPoolThreads() == 1u);
    ok &= (service.GetMaximumNumberOfPoolThreads() == 3u);
    ok &= (service.GetPriorityClass() == Threads::NormalPriorityClass);
    ok &= (service.GetPriorityLevel() == 0u);
    ok &= (service.GetCPUMask() == UndefinedCPUs);
    return ok;
}

bool MultiClientServiceTest::TestInitialise() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("MinNumberOfThreads", 5);
    config.Write("MaxNumberOfThreads", 15);
    bool ok = multiThreadService.Initialise(config);
    ok &= (multiThreadService.GetMinimumNumberOfPoolThreads() == 5);
    ok &= (multiThreadService.GetMaximumNumberOfPoolThreads() == 15);
    ok &= (multiThreadService.GetTimeout() == 10);
    ok &= (multiThreadService.GetPriorityClass() == Threads::NormalPriorityClass);
    ok &= (multiThreadService.GetPriorityLevel() == 0u);
    ok &= (multiThreadService.GetCPUMask() == UndefinedCPUs);

    ConfigurationDatabase config2;
    config2.Write("Timeout", 20);
    config2.Write("MinNumberOfThreads", 15);
    config2.Write("MaxNumberOfThreads", 25);
    config2.Write("PriorityLevel", 1);
    config2.Write("CPUMask", 0x3);
    config2.Write("PriorityClass", "RealTimePriorityClass");
    ok &= multiThreadService.Initialise(config2);
    ok &= (multiThreadService.GetMinimumNumberOfPoolThreads() == 15);
    ok &= (multiThreadService.GetMaximumNumberOfPoolThreads() == 25);
    ok &= (multiThreadService.GetTimeout() == 20);
    ok &= (multiThreadService.GetPriorityClass() == Threads::RealTimePriorityClass);
    ok &= (multiThreadService.GetPriorityLevel() == 1);
    ok &= (multiThreadService.GetCPUMask() == 0x3);

    return ok;
}

bool MultiClientServiceTest::TestInitialise_False_Timeout() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);

    ConfigurationDatabase config;
    config.Write("Timeot", 10);
    config.Write("MinNumberOfThreads", 5);
    config.Write("MaxNumberOfThreads", 15);
    return !service.Initialise(config);
}

bool MultiClientServiceTest::TestInitialise_False_MinNumberOfThreads() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("MnNumberOfThreads", 5);
    config.Write("MaxNumberOfThreads", 15);
    return !service.Initialise(config);
}

bool MultiClientServiceTest::TestInitialise_False_MaxNumberOfThreads() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("MinNumberOfThreads", 5);
    config.Write("MxNumberOfThreads", 15);
    return !service.Initialise(config);
}

bool MultiClientServiceTest::TestInitialise_False_MinGreaterThanMax() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("MinNumberOfThreads", 15);
    config.Write("MaxNumberOfThreads", 5);
    return !service.Initialise(config);
}

bool MultiClientServiceTest::TestInitialise_False_MinNumberOfThreads_0() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("MinNumberOfThreads", 0);
    config.Write("MaxNumberOfThreads", 35);
    return !service.Initialise(config);
}

bool MultiClientServiceTest::TestStart() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMinimumNumberOfPoolThreads(2);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetTimeout(1000);
    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    //Now mark one connection request. This should trigger the creation of a new thread which will wait and this thread will start serving
    callbackClass.connectionRequested = true;
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    maxCounter = 10;
    while ((maxCounter > 0)
            && ((callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())
                    || (callbackClass.numberConnectionsServing != (service.GetMinimumNumberOfPoolThreads() - 1u)))) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    ok &= (callbackClass.numberConnectionsServing == (service.GetMinimumNumberOfPoolThreads() - 1u));
    ok &= (service.GetNumberOfActiveThreads() == (service.GetMinimumNumberOfPoolThreads() + 1u));
    //Marking the connection has served will make this thread die (because the service.GetNumberOfActiveThreads() > (service.GetMinimumNumberOfPoolThreads()))
    callbackClass.connectIsServed = true;
    maxCounter = 10;
    while ((maxCounter > 0) && service.GetNumberOfActiveThreads() != (service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (service.GetNumberOfActiveThreads() == service.GetMinimumNumberOfPoolThreads());
    service.Stop();
    return ok;
}

bool MultiClientServiceTest::TestStart_NotEnoughThreads() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMinimumNumberOfPoolThreads(2);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetTimeout(1000);
    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    //Now mark one connection request. This should trigger the creation of a new thread which will wait and this thread will start serving
    callbackClass.connectionRequested = true;
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    maxCounter = 10;
    while ((maxCounter > 0)
            && ((callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())
                    || (callbackClass.numberConnectionsServing != (service.GetMinimumNumberOfPoolThreads() - 1u)))) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    ok &= (callbackClass.numberConnectionsServing == (service.GetMinimumNumberOfPoolThreads() - 1u));
    ok &= (service.GetNumberOfActiveThreads() == (service.GetMinimumNumberOfPoolThreads() + 1u));
    //Ask for a new connection. This should fail and the number of connections waiting should decrease
    callbackClass.connectionRequested = true;
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    maxCounter = 10;
    while ((maxCounter > 0)
            && ((callbackClass.numberConnectionsWaiting != (service.GetMinimumNumberOfPoolThreads() - 1u))
                    || (callbackClass.numberConnectionsServing != (service.GetMinimumNumberOfPoolThreads())))) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == (service.GetMinimumNumberOfPoolThreads() - 1u));
    ok &= (callbackClass.numberConnectionsServing == (service.GetMinimumNumberOfPoolThreads()));
    ok &= (service.GetNumberOfActiveThreads() == (service.GetMinimumNumberOfPoolThreads() + 1u));

    //Marking the connection has served will make this thread die (because the service.GetNumberOfActiveThreads() > (service.GetMinimumNumberOfPoolThreads()))
    callbackClass.connectIsServed = true;
    maxCounter = 10;
    while ((maxCounter > 0) && service.GetNumberOfActiveThreads() != (service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (service.GetNumberOfActiveThreads() == service.GetMinimumNumberOfPoolThreads());
    service.Stop();
    return ok;
}

bool MultiClientServiceTest::TestStart_False() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetTimeout(1000);
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    err = service.Start();
    ok &= (err == ErrorManagement::IllegalOperation);

    service.Stop();
    return ok;
}

bool MultiClientServiceTest::TestStart_Restart() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMinimumNumberOfPoolThreads(2);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetTimeout(1000);
    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    //Now mark one connection request. This should trigger the creation of a new thread which will wait and this thread will start serving
    callbackClass.connectionRequested = true;
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    maxCounter = 10;
    while ((maxCounter > 0)
            && ((callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())
                    || (callbackClass.numberConnectionsServing != (service.GetMinimumNumberOfPoolThreads() - 1u)))) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    ok &= (callbackClass.numberConnectionsServing == (service.GetMinimumNumberOfPoolThreads() - 1u));
    ok &= (service.GetNumberOfActiveThreads() == (service.GetMinimumNumberOfPoolThreads() + 1u));
    //Marking the connection has served will make this thread die (because the service.GetNumberOfActiveThreads() > (service.GetMinimumNumberOfPoolThreads()))
    callbackClass.connectIsServed = true;
    maxCounter = 10;
    while ((maxCounter > 0) && service.GetNumberOfActiveThreads() != (service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (service.GetNumberOfActiveThreads() == service.GetMinimumNumberOfPoolThreads());

    err = service.Stop();
    ok &= (err == ErrorManagement::NoError);

    callbackClass.numberConnectionsWaiting = 0u;
    callbackClass.numberConnectionsServing = 0u;

    err = service.Start();
    ok &= (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    //Now mark one connection request. This should trigger the creation of a new thread which will wait and this thread will start serving
    callbackClass.connectionRequested = true;
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    maxCounter = 10;
    while ((maxCounter > 0)
            && ((callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())
                    || (callbackClass.numberConnectionsServing != (service.GetMinimumNumberOfPoolThreads() - 1u)))) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    ok &= (callbackClass.numberConnectionsServing == (service.GetMinimumNumberOfPoolThreads() - 1u));
    ok &= (service.GetNumberOfActiveThreads() == (service.GetMinimumNumberOfPoolThreads() + 1u));
    //Marking the connection has served will make this thread die (because the service.GetNumberOfActiveThreads() > (service.GetMinimumNumberOfPoolThreads()))
    callbackClass.connectIsServed = true;
    maxCounter = 10;
    while ((maxCounter > 0) && service.GetNumberOfActiveThreads() != (service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (service.GetNumberOfActiveThreads() == service.GetMinimumNumberOfPoolThreads());
    err = service.Stop();
    return ok;
}

bool MultiClientServiceTest::TestStop() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMinimumNumberOfPoolThreads(2);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetTimeout(1000);
    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    //Now mark one connection request. This should trigger the creation of a new thread which will wait and this thread will start serving
    callbackClass.connectionRequested = true;
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    maxCounter = 10;
    while ((maxCounter > 0)
            && ((callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())
                    || (callbackClass.numberConnectionsServing != (service.GetMinimumNumberOfPoolThreads() - 1u)))) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    ok &= (callbackClass.numberConnectionsServing == (service.GetMinimumNumberOfPoolThreads() - 1u));
    ok &= (service.GetNumberOfActiveThreads() == (service.GetMinimumNumberOfPoolThreads() + 1u));
    //Marking the connection has served will make this thread die (because the service.GetNumberOfActiveThreads() > (service.GetMinimumNumberOfPoolThreads()))
    callbackClass.connectIsServed = true;
    maxCounter = 10;
    while ((maxCounter > 0) && service.GetNumberOfActiveThreads() != (service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (service.GetNumberOfActiveThreads() == service.GetMinimumNumberOfPoolThreads());
    err = service.Stop();

    ok &= (err == ErrorManagement::NoError);
    ok &= (service.GetNumberOfActiveThreads() == 0u);
    return ok;
}

bool MultiClientServiceTest::TestStop_Kill() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClassToKill callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClassToKill> binder(callbackClass, &MultiClientServiceTestCallbackClassToKill::CallbackFunction);
    MultiClientService service(binder);
    service.SetMinimumNumberOfPoolThreads(2);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetTimeout(1000);
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState != 2u)) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.internalState == 2u);

    service.SetTimeout(100);
    err = service.Stop();
    ok &= (err == ErrorManagement::NoError);
    maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState != 0u)) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.internalState == 0u);
    ok &= (service.GetNumberOfActiveThreads() == 0u);
    return ok;
}

bool MultiClientServiceTest::TestSetMaximumNumberOfPoolThreads() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMaximumNumberOfPoolThreads(100);
    return (service.GetMaximumNumberOfPoolThreads() == 100);
}

bool MultiClientServiceTest::TestSetMaximumNumberOfPoolThreads_MinGreaterThanMax() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMaximumNumberOfPoolThreads(100);
    bool ok = (service.GetMaximumNumberOfPoolThreads() == 100);
    service.SetMinimumNumberOfPoolThreads(50);
    service.SetMaximumNumberOfPoolThreads(50);
    ok = (service.GetMaximumNumberOfPoolThreads() == 100);

    return ok;
}

bool MultiClientServiceTest::TestSetMaximumNumberOfPoolThreads_AfterStart() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetMinimumNumberOfPoolThreads(2);

    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());

    //Not allowed as the thread is running!
    service.SetMaximumNumberOfPoolThreads(90);
    ok &= (service.GetMaximumNumberOfPoolThreads() == 3);
    service.Stop();
    service.SetMaximumNumberOfPoolThreads(90);
    ok &= (service.GetMaximumNumberOfPoolThreads() == 90);

    return ok;
}

bool MultiClientServiceTest::TestSetMinimumNumberOfPoolThreads() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMaximumNumberOfPoolThreads(200);
    service.SetMinimumNumberOfPoolThreads(100);
    return (service.GetMinimumNumberOfPoolThreads() == 100);
}

bool MultiClientServiceTest::TestSetMinimumNumberOfPoolThreads_MinGreaterThanMax() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMaximumNumberOfPoolThreads(200);
    service.SetMinimumNumberOfPoolThreads(100);
    return (service.GetMinimumNumberOfPoolThreads() == 100);
}

bool MultiClientServiceTest::TestSetMinimumNumberOfPoolThreads_AfterStart() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetMinimumNumberOfPoolThreads(2);
    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    //Not allowed as the thread is running!
    service.SetMinimumNumberOfPoolThreads(1);
    ok &= (service.GetMinimumNumberOfPoolThreads() == 2);
    service.Stop();
    service.SetMinimumNumberOfPoolThreads(1);
    ok &= (service.GetMinimumNumberOfPoolThreads() == 1);

    return ok;

}

bool MultiClientServiceTest::TestSetMinimumNumberOfPoolThreads_0() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMaximumNumberOfPoolThreads(200);
    service.SetMinimumNumberOfPoolThreads(100);
    service.SetMinimumNumberOfPoolThreads(0);
    return (service.GetMinimumNumberOfPoolThreads() == 100);
}

bool MultiClientServiceTest::TestGetMaximumNumberOfPoolThreads() {
    return TestSetMaximumNumberOfPoolThreads();
}

bool MultiClientServiceTest::TestGetMinimumNumberOfPoolThreads() {
    return TestSetMinimumNumberOfPoolThreads();
}

bool MultiClientServiceTest::TestGetNumberOfActiveThreads() {
    return TestStart();
}

bool MultiClientServiceTest::TestAddThread() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClassToKill callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClassToKill> binder(callbackClass, &MultiClientServiceTestCallbackClassToKill::CallbackFunction);
    MultiClientService service(binder);
    service.SetMinimumNumberOfPoolThreads(2);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetTimeout(100);
    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.internalState != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.internalState == service.GetMinimumNumberOfPoolThreads());
    err = service.AddThread();
    ok &= (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    maxCounter = 10u;
    while ((maxCounter > 0) && (callbackClass.internalState != (service.GetMinimumNumberOfPoolThreads() + 1u))) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= ((callbackClass.internalState == service.GetMinimumNumberOfPoolThreads() + 1u));
    err = service.AddThread();
    ok &= (err == ErrorManagement::IllegalOperation);
    service.Stop();
    return ok;
}

bool MultiClientServiceTest::TestRemoveThread() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClassToKill callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClassToKill> binder(callbackClass, &MultiClientServiceTestCallbackClassToKill::CallbackFunction);
    MultiClientService service(binder);
    service.SetMinimumNumberOfPoolThreads(2);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetTimeout(100);
    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.internalState != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.internalState == service.GetMinimumNumberOfPoolThreads());
    err = service.AddThread();
    ok &= (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    maxCounter = 10u;
    while ((maxCounter > 0) && (callbackClass.internalState != (service.GetMinimumNumberOfPoolThreads() + 1u))) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= ((callbackClass.internalState == service.GetMinimumNumberOfPoolThreads() + 1u));
    err = service.AddThread();
    ok &= (err == ErrorManagement::IllegalOperation);
    callbackClass.done = true;
    //The thread in excess will destroy itself by calling RemoveThread
    maxCounter = 10u;
    while ((maxCounter > 0) && (service.GetNumberOfActiveThreads() != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (service.GetNumberOfActiveThreads() == service.GetMinimumNumberOfPoolThreads());
    service.Stop();
    return ok;
}

bool MultiClientServiceTest::TestMoreThanEnoughThreads() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClassToKill callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClassToKill> binder(callbackClass, &MultiClientServiceTestCallbackClassToKill::CallbackFunction);
    MultiClientService service(binder);
    service.SetMinimumNumberOfPoolThreads(2);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetTimeout(100);
    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.internalState != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.internalState == service.GetMinimumNumberOfPoolThreads());
    ok &= !service.MoreThanEnoughThreads();
    err = service.AddThread();
    ok &= service.MoreThanEnoughThreads();
    service.Stop();
    return ok;
}

bool MultiClientServiceTest::TestSetPriorityClass() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    bool ok = (service.GetPriorityClass() == Threads::NormalPriorityClass);
    service.SetPriorityClass(Threads::RealTimePriorityClass);
    ok &= (service.GetPriorityClass() == Threads::RealTimePriorityClass);
    return ok;
}

bool MultiClientServiceTest::TestSetPriorityLevel() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    bool ok = (service.GetPriorityLevel() == 0);
    service.SetPriorityLevel(10);
    ok &= (service.GetPriorityLevel() == 10);
    return ok;
}

bool MultiClientServiceTest::TestSetCPUMask() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    bool ok = (service.GetCPUMask() == UndefinedCPUs);
    service.SetCPUMask(0x3);
    ok &= (service.GetCPUMask() == 0x3);
    return ok;
}

bool MultiClientServiceTest::TestSetPriorityClass_AfterStart() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetMinimumNumberOfPoolThreads(2);

    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    ok &= (service.GetPriorityClass() == Threads::NormalPriorityClass);
    service.SetPriorityClass(Threads::RealTimePriorityClass);
    ok &= (service.GetPriorityClass() == Threads::NormalPriorityClass);

    service.Stop();

    service.SetPriorityClass(Threads::RealTimePriorityClass);
    ok &= (service.GetPriorityClass() == Threads::RealTimePriorityClass);
    return ok;
}

bool MultiClientServiceTest::TestSetPriorityLevel_AfterStart() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetMinimumNumberOfPoolThreads(2);

    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    ok &= (service.GetPriorityLevel() == 0u);
    service.SetPriorityLevel(10u);
    ok &= (service.GetPriorityLevel() == 0u);

    service.Stop();

    service.SetPriorityLevel(10u);
    ok &= (service.GetPriorityLevel() == 10u);
    return ok;
}

bool MultiClientServiceTest::TestSetCPUMask_AfterStart() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);
    MultiClientService service(binder);
    service.SetMaximumNumberOfPoolThreads(3);
    service.SetMinimumNumberOfPoolThreads(2);

    uint32 maxCounter = 10;
    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    //Check that GetMinimumNumberOfPoolThreadswo threads are awaiting connection
    while ((maxCounter > 0) && (callbackClass.numberConnectionsWaiting != service.GetMinimumNumberOfPoolThreads())) {
        Sleep::Sec(0.5);
        maxCounter--;
    }
    ok &= (callbackClass.numberConnectionsWaiting == service.GetMinimumNumberOfPoolThreads());
    ok &= (service.GetCPUMask() == UndefinedCPUs);
    service.SetCPUMask(0xf);
    ok &= (service.GetCPUMask() == UndefinedCPUs);

    service.Stop();

    service.SetCPUMask(0xf);
    ok &= (service.GetCPUMask() == 0xf);
    return ok;
}
