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
#include "MutexSem.h"
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
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information) {
        if (information.GetStage() == MARTe::ExecutionInfo::AsyncTerminationStage) {
            internalState--;
        }
        else {
            internalState++;
            while (1) {
                MARTe::Sleep::Sec(1.0);
            }
        }

        return MARTe::ErrorManagement::NoError;
    }

    MARTe::uint32 internalState;
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MultiClientServiceTest::MultiClientServiceTest() {
}

MultiClientServiceTest::~MultiClientServiceTest() {
}

bool MultiClientServiceTest::TestDefaultConstructor_Template() {
    using namespace MARTe;
    MultiClientServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiClientServiceTestCallbackClass> binder(callbackClass, &MultiClientServiceTestCallbackClass::CallbackFunction);

    MultiClientService multiThreadService(binder);
    bool ok = (multiThreadService.GetMinimumNumberOfPoolThreads() == 1u);
    ok &= (multiThreadService.GetMaximumNumberOfPoolThreads() == 3u);
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
