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

    MARTe::ErrorManagement::ErrorType CallbackFunction(const MARTe::ExecutionInfo &information) {
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

    MARTe::ErrorManagement::ErrorType CallbackFunction(const MARTe::ExecutionInfo &information) {
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

    SingleThreadService service(binder);
    bool ok = (service.GetTimeout() == TTInfiniteWait);
    ok &= (service.GetStatus() == EmbeddedThreadI::OffState);
    return ok;
}

bool SingleThreadServiceTest::TestDefaultConstructor_Template() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);
    bool ok = (service.GetTimeout() == TTInfiniteWait);
    ok &= (service.GetStatus() == EmbeddedThreadI::OffState);
    return ok;
}

bool SingleThreadServiceTest::TestInitialise() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    bool ok = service.Initialise(config);
    ok &= (service.GetTimeout() == 10);
    ConfigurationDatabase config2;
    config2.Write("Timeout", 0);
    ok &= service.Initialise(config2);
    ok &= (service.GetTimeout() == TTInfiniteWait);
    return ok;
}

bool SingleThreadServiceTest::TestInitialise_False() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    ConfigurationDatabase config;
    config.Write("Timeot", 10);

    return !service.Initialise(config);
}

bool SingleThreadServiceTest::TestStart() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    bool ok = (service.GetStatus() == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 10u);
    ok &= (service.GetStatus() == EmbeddedThreadI::RunningState);

    service.Stop();
    return ok;
}

bool SingleThreadServiceTest::TestStart_False() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    bool ok = (service.GetStatus() == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 10u);
    ok &= (service.GetStatus() == EmbeddedThreadI::RunningState);

    err = service.Start();
    ok &= (err == ErrorManagement::IllegalOperation);

    service.Stop();
    return ok;
}

bool SingleThreadServiceTest::TestStart_Restart() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    bool ok = (service.GetStatus() == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 10u);
    ok &= (service.GetStatus() == EmbeddedThreadI::RunningState);

    service.Stop();

    callbackClass.internalState = 0u;
    err = service.Start();
    ok &= (err == ErrorManagement::NoError);
    maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 10u);
    ok &= (service.GetStatus() == EmbeddedThreadI::RunningState);
    err = service.Stop();
    ok &= (err == ErrorManagement::NoError);
    return ok;
}

bool SingleThreadServiceTest::TestStop() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    bool ok = (service.GetStatus() == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState < 10u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState >= 10u);
    ok &= (service.GetStatus() == EmbeddedThreadI::RunningState);

    err = service.Stop();
    ok &= (err == ErrorManagement::NoError);
    ok &= (service.GetStatus() == EmbeddedThreadI::OffState);

    return ok;
}

bool SingleThreadServiceTest::TestStop_Kill() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClassToKill callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClassToKill> binder(callbackClass, &SingleThreadServiceTestCallbackClassToKill::CallbackFunction);
    SingleThreadService service(binder);

    bool ok = (service.GetStatus() == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (callbackClass.internalState == 0u)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (callbackClass.internalState == 1u);
    ok &= (service.GetStatus() == EmbeddedThreadI::RunningState);

    service.SetTimeout(1000);
    err = service.Stop();

    ok &= (err == ErrorManagement::Timeout);
    ok &= (service.GetStatus() == EmbeddedThreadI::TimeoutStoppingState);

    err = service.Stop();

    ok &= (err == ErrorManagement::NoError);
    ok &= (service.GetStatus() == EmbeddedThreadI::OffState);
    ok &= (callbackClass.internalState == 15u);
    return ok;
}

bool SingleThreadServiceTest::TestSetTimeout() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);
    service.SetTimeout(100);
    bool ok = (service.GetTimeout() == 100);
    service.SetTimeout(TTInfiniteWait);
    ok &= (service.GetTimeout() == TTInfiniteWait);
    return ok;
}

bool SingleThreadServiceTest::TestGetTimeout() {
    return TestSetTimeout();
}
