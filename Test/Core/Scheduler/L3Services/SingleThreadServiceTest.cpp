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
    ok &= (service.GetPriorityClass() == Threads::NormalPriorityClass);
    ok &= (service.GetPriorityLevel() == 0u);
    ok &= (service.GetCPUMask() == UndefinedCPUs);
    ok &= (service.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    return ok;
}

bool SingleThreadServiceTest::TestDefaultConstructor_Template() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);
    bool ok = (service.GetTimeout() == TTInfiniteWait);
    ok &= (service.GetStatus() == EmbeddedThreadI::OffState);
    ok &= (service.GetPriorityClass() == Threads::NormalPriorityClass);
    ok &= (service.GetPriorityLevel() == 0u);
    ok &= (service.GetCPUMask() == UndefinedCPUs);
    ok &= (service.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
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
    ok &= (service.GetPriorityClass() == Threads::NormalPriorityClass);
    ok &= (service.GetPriorityLevel() == 0u);
    ok &= (service.GetCPUMask() == UndefinedCPUs);
    ok &= (service.GetStackSize() == THREADS_DEFAULT_STACKSIZE);

    ConfigurationDatabase config2;
    config2.Write("Timeout", 0);
    config2.Write("PriorityLevel", 1);
    config2.Write("CPUMask", 0x3);
    config2.Write("PriorityClass", "RealTimePriorityClass");
    config2.Write("StackSize", 100000);
    ok &= service.Initialise(config2);
    ok &= (service.GetTimeout() == TTInfiniteWait);
    ok &= (service.GetPriorityClass() == Threads::RealTimePriorityClass);
    ok &= (service.GetPriorityLevel() == 1);
    ok &= (service.GetCPUMask() == 0x3);
    ok &= (service.GetStackSize() == 100000);

    ConfigurationDatabase config3;
    config3.Write("Timeout", 0);
    config3.Write("PriorityLevel", 2);
    config3.Write("CPUMask", 0x5);
    config3.Write("PriorityClass", "IdlePriorityClass");
    config2.Write("StackSize", 100000);
    ok &= service.Initialise(config3);
    ok &= (service.GetTimeout() == TTInfiniteWait);
    ok &= (service.GetPriorityClass() == Threads::IdlePriorityClass);
    ok &= (service.GetPriorityLevel() == 2);
    ok &= (service.GetCPUMask() == 0x5);
    ok &= (service.GetStackSize() == 100000);

    ConfigurationDatabase config4;
    config4.Write("Timeout", 0);
    config4.Write("PriorityLevel", 3);
    config4.Write("CPUMask", 0xf);
    config4.Write("PriorityClass", "NormalPriorityClass");
    config4.Write("StackSize", 1000000);
    ok &= service.Initialise(config4);
    ok &= (service.GetTimeout() == TTInfiniteWait);
    ok &= (service.GetPriorityClass() == Threads::NormalPriorityClass);
    ok &= (service.GetPriorityLevel() == 3);
    ok &= (service.GetCPUMask() == 0xf);
    ok &= (service.GetStackSize() == 1000000);
    return ok;
}

bool SingleThreadServiceTest::TestInitialise_False() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    ConfigurationDatabase config;
    config.Write("Timeot", 10);

    bool ok = !service.Initialise(config);

    ConfigurationDatabase config2;
    config2.Write("Timeout", 0);
    config2.Write("PriorityClass", "OtherPriorityClass");
    ok &= !service.Initialise(config2);

    return ok;
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
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClassToKill> binder(callbackClass,
                                                                                    &SingleThreadServiceTestCallbackClassToKill::CallbackFunction);
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

bool SingleThreadServiceTest::TestGetPriorityClass() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);
    bool ok = (service.GetPriorityClass() == Threads::NormalPriorityClass);
    service.SetPriorityClass(Threads::RealTimePriorityClass);
    ok &= (service.GetPriorityClass() == Threads::RealTimePriorityClass);
    return ok;
}

bool SingleThreadServiceTest::TestSetPriorityClass() {
    return TestGetPriorityClass();
}

bool SingleThreadServiceTest::TestGetPriorityLevel() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);
    bool ok = (service.GetPriorityLevel() == 0);
    service.SetPriorityLevel(10);
    ok &= (service.GetPriorityLevel() == 10);
    return ok;
}

bool SingleThreadServiceTest::TestSetStackSize() {
    return TestGetStackSize();
}

bool SingleThreadServiceTest::TestGetStackSize() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);
    bool ok = (service.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    service.SetStackSize(THREADS_DEFAULT_STACKSIZE * 2);
    ok &= (service.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 2));
    return ok;
}

bool SingleThreadServiceTest::TestSetPriorityLevel() {
    return TestGetPriorityLevel();
}

bool SingleThreadServiceTest::TestGetCPUMask() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);
    bool ok = (service.GetCPUMask() == UndefinedCPUs);
    service.SetCPUMask(0x3);
    ok &= (service.GetCPUMask() == 0x3);
    return ok;
}

bool SingleThreadServiceTest::TestSetCPUMask() {
    return TestGetCPUMask();
}

bool SingleThreadServiceTest::TestSetPriorityClass_Start() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    bool ok = (service.GetStatus() == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((service.GetStatus() != EmbeddedThreadI::RunningState) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus() == EmbeddedThreadI::RunningState);
    ok &= (service.GetPriorityClass() == Threads::NormalPriorityClass);
    service.SetPriorityClass(Threads::RealTimePriorityClass);
    ok &= (service.GetPriorityClass() == Threads::NormalPriorityClass);
    service.Stop();
    maxCounter = 10;
    while ((service.GetStatus() != EmbeddedThreadI::OffState) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.SetPriorityClass(Threads::RealTimePriorityClass);
    ok &= (service.GetPriorityClass() == Threads::RealTimePriorityClass);
    return ok;
}

bool SingleThreadServiceTest::TestSetPriorityLevel_Start() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    bool ok = (service.GetStatus() == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((service.GetStatus() != EmbeddedThreadI::RunningState) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus() == EmbeddedThreadI::RunningState);
    ok &= (service.GetPriorityLevel() == 0);
    service.SetPriorityLevel(10);
    ok &= (service.GetPriorityLevel() == 0);
    service.Stop();
    maxCounter = 10;
    while ((service.GetStatus() != EmbeddedThreadI::OffState) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.SetPriorityLevel(10);
    ok &= (service.GetPriorityLevel() == 10);
    return ok;
}

bool SingleThreadServiceTest::TestSetStackSize_Start() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    bool ok = (service.GetStatus() == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((service.GetStatus() != EmbeddedThreadI::RunningState) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus() == EmbeddedThreadI::RunningState);
    ok &= (service.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    service.SetStackSize(100000);
    ok &= (service.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    service.Stop();
    maxCounter = 10;
    while ((service.GetStatus() != EmbeddedThreadI::OffState) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.SetStackSize(100000);
    ok &= (service.GetStackSize() == 100000);
    return ok;
}

bool SingleThreadServiceTest::TestSetCPUMask_Start() {
    using namespace MARTe;
    SingleThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<SingleThreadServiceTestCallbackClass> binder(callbackClass, &SingleThreadServiceTestCallbackClass::CallbackFunction);
    SingleThreadService service(binder);

    bool ok = (service.GetStatus() == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((service.GetStatus() != EmbeddedThreadI::RunningState) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus() == EmbeddedThreadI::RunningState);
    ok &= (service.GetCPUMask() == UndefinedCPUs);
    service.SetCPUMask(0xfe);
    ok &= (service.GetCPUMask() == UndefinedCPUs);
    service.Stop();
    maxCounter = 10;
    while ((service.GetStatus() != EmbeddedThreadI::OffState) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.SetCPUMask(0xfe);
    ok &= (service.GetCPUMask() == 0xfe);
    return ok;
}
