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
        if (information.GetThreadNumber() == 0u) {
            internalStateThread0++;
        }
        else if (information.GetThreadNumber() == 1u) {
            internalStateThread1++;
        }
        else if (information.GetThreadNumber() == 2u) {
            internalStateThread2++;
        }
        //Allow to reschedule
        MARTe::Sleep::Sec(0.1);
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
        if (information.GetStage() == MARTe::ExecutionInfo::MainStage) {
            internalState++;
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
    multiThreadService.Stop();

    ConfigurationDatabase config2;
    config2.Write("Timeout", 20);
    config2.Write("NumberOfPoolThreads", 30);
    config2.Write("PriorityLevel", 1);
    config2.Write("CPUMask", 0x3);
    config2.Write("PriorityClass", "RealTimePriorityClass");
    config2.Write("StackSize", THREADS_DEFAULT_STACKSIZE * 2);
    ok &= multiThreadService.Initialise(config2);
    ok &= (multiThreadService.GetTimeout() == 20);
    ok &= (multiThreadService.GetNumberOfPoolThreads() == 30);
    ok &= (multiThreadService.GetPriorityClass() == Threads::RealTimePriorityClass);
    ok &= (multiThreadService.GetPriorityLevel() == 1);
    ok &= (multiThreadService.GetCPUMask() == 0x3);
    ok &= (multiThreadService.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 2));
    multiThreadService.Stop();

    ConfigurationDatabase config3;
    config3.Write("Timeout", 20);
    config3.Write("NumberOfPoolThreads", 10);
    config3.Write("PriorityLevel", 5);
    config3.Write("CPUMask", 0x3);
    config3.Write("PriorityClass", "RealTimePriorityClass");
    config3.Write("StackSize", THREADS_DEFAULT_STACKSIZE * 5);

    uint32 prioritiesLevel[3][2] = { { 0, 10 }, { 1, 9 }, { 9, 1 } };
    config3.Write("PrioritiesLevel", prioritiesLevel);

    const char8 * prioritiesClass[4][2] = { { "0", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" }, { "8", "RealTimePriorityClass" } };
    config3.Write("PrioritiesClass", prioritiesClass);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config3.Write("CPUMasks", cpuMasks);

    uint32 stackSizes[3][2] = { { 0, THREADS_DEFAULT_STACKSIZE * 2 }, { 1, THREADS_DEFAULT_STACKSIZE * 3 }, { 9, THREADS_DEFAULT_STACKSIZE * 4 } };
    config3.Write("StackSizes", stackSizes);

    ok &= multiThreadService.Initialise(config3);
    ok &= (multiThreadService.GetTimeout() == 20);
    ok &= (multiThreadService.GetNumberOfPoolThreads() == 10);
    ok &= (multiThreadService.GetPriorityClass() == Threads::RealTimePriorityClass);
    ok &= (multiThreadService.GetPriorityLevel() == 5);
    ok &= (multiThreadService.GetCPUMask() == 0x3);
    ok &= (multiThreadService.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 5));

    ok &= (multiThreadService.GetPriorityClassThreadPool(0) == Threads::NormalPriorityClass);
    ok &= (multiThreadService.GetPriorityClassThreadPool(1) == Threads::IdlePriorityClass);
    ok &= (multiThreadService.GetPriorityClassThreadPool(8) == Threads::RealTimePriorityClass);
    ok &= (multiThreadService.GetPriorityClassThreadPool(9) == Threads::NormalPriorityClass);

    ok &= (multiThreadService.GetPriorityLevelThreadPool(0) == 10);
    ok &= (multiThreadService.GetPriorityLevelThreadPool(1) == 9);
    ok &= (multiThreadService.GetPriorityLevelThreadPool(9) == 1);

    ok &= (multiThreadService.GetCPUMaskThreadPool(0) == 0x8);
    ok &= (multiThreadService.GetCPUMaskThreadPool(1) == 0x4);
    ok &= (multiThreadService.GetCPUMaskThreadPool(9) == 0x2);

    ok &= (multiThreadService.GetStackSizeThreadPool(0) == (THREADS_DEFAULT_STACKSIZE * 2));
    ok &= (multiThreadService.GetStackSizeThreadPool(1) == (THREADS_DEFAULT_STACKSIZE * 3));
    ok &= (multiThreadService.GetStackSizeThreadPool(9) == (THREADS_DEFAULT_STACKSIZE * 4));

    multiThreadService.Stop();

    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_Timeout() {
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

bool MultiThreadServiceTest::TestInitialise_False_PrioritiesClass_NotMatrix() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3] = { "NormalPriorityClass", "IdlePriorityClass", "NormalPriorityClass" };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 prioritiesLevel[3][2] = { { 0, 10 }, { 1, 9 }, { 9, 1 } };
    config.Write("PrioritiesLevel", prioritiesLevel);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_PrioritiesClass_Columns() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][3] = { { "0", "NormalPriorityClass", "A" }, { "1", "IdlePriorityClass", "B" }, { "9", "NormalPriorityClass", "C" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 prioritiesLevel[3][2] = { { 0, 10 }, { 1, 9 }, { 9, 1 } };
    config.Write("PrioritiesLevel", prioritiesLevel);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_PrioritiesClass_OutOfRangeIndex() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "0", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "10", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 prioritiesLevel[3][2] = { { 0, 10 }, { 1, 9 }, { 9, 1 } };
    config.Write("PrioritiesLevel", prioritiesLevel);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_PrioritiesClass_TypeConvert() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "zxcf", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 prioritiesLevel[3][2] = { { 0, 10 }, { 1, 9 }, { 9, 1 } };
    config.Write("PrioritiesLevel", prioritiesLevel);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_PrioritiesClass_Invalid() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "0", "Invalid" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 prioritiesLevel[3][2] = { { 0, 10 }, { 1, 9 }, { 9, 1 } };
    config.Write("PrioritiesLevel", prioritiesLevel);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_PrioritiesLevel_NotMatrix() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "0", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 prioritiesLevel[3] = { 0, 10, 1 };
    config.Write("PrioritiesLevel", prioritiesLevel);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_PrioritiesLevel_Columns() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "0", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 prioritiesLevel[3][3] = { { 0, 10, 11 }, { 1, 9, 12 }, { 9, 1, 13 } };
    config.Write("PrioritiesLevel", prioritiesLevel);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_PrioritiesLevel_OutOfRangeIndex() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "0", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 prioritiesLevel[3][2] = { { 0, 10 }, { 1, 9 }, { 10, 1 } };
    config.Write("PrioritiesLevel", prioritiesLevel);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_StackSizes_NotMatrix() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "0", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 stackSizes[3] = { 0, 10, 1 };
    config.Write("StackSizes", stackSizes);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_StackSizes_Columns() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "0", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 stackSizes[3][3] = { { 0, 10, 11 }, { 1, 9, 12 }, { 9, 1, 13 } };
    config.Write("StackSizes", stackSizes);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_StackSizes_OutOfRangeIndex() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "0", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 stackSizes[3][2] = { { 0, 10 }, { 1, 9 }, { 10, 1 } };
    config.Write("StackSizes", stackSizes);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 9, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}


bool MultiThreadServiceTest::TestInitialise_False_CPUMasks_NotMatrix() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "0", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 prioritiesLevel[3][2] = { { 0, 10 }, { 1, 9 }, { 9, 1 } };
    config.Write("PrioritiesLevel", prioritiesLevel);

    uint32 cpuMasks[3] = { 0, 0x8, 1 };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_CPUMasks_Columns() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "0", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 prioritiesLevel[3][2] = { { 0, 10 }, { 1, 9 }, { 9, 1 } };
    config.Write("PrioritiesLevel", prioritiesLevel);

    uint32 cpuMasks[3][3] = { { 0, 0x8, 1 }, { 1, 0x4, 2 }, { 9, 0x2, 3 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestInitialise_False_CPUMasks_OutOfRangeIndex() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);

    ConfigurationDatabase config;
    config.Write("Timeout", 10);
    config.Write("NumberOfPoolThreads", 10);
    config.Write("PriorityLevel", 5);
    config.Write("CPUMask", 0x3);
    config.Write("PriorityClass", "RealTimePriorityClass");

    const char8 * prioritiesClass[3][2] = { { "0", "NormalPriorityClass" }, { "1", "IdlePriorityClass" }, { "9", "NormalPriorityClass" } };
    config.Write("PrioritiesClass", prioritiesClass);

    uint32 prioritiesLevel[3][2] = { { 0, 10 }, { 1, 9 }, { 9, 1 } };
    config.Write("PrioritiesLevel", prioritiesLevel);

    uint32 cpuMasks[3][2] = { { 0, 0x8 }, { 1, 0x4 }, { 10, 0x2 } };
    config.Write("CPUMasks", cpuMasks);

    bool ok = !multiThreadService.Initialise(config);
    multiThreadService.Stop();
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

bool MultiThreadServiceTest::TestCreateThreads() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);
    multiThreadService.SetNumberOfPoolThreads(3);

    bool ok = multiThreadService.CreateThreads();
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestCreateThreads_False() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);
    multiThreadService.SetNumberOfPoolThreads(3);

    bool ok = multiThreadService.CreateThreads();
    ok &= (multiThreadService.CreateThreads() == ErrorManagement::IllegalOperation);
    multiThreadService.Stop();
    return ok;
}

bool MultiThreadServiceTest::TestCreateThreads_Restart() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService multiThreadService(binder);
    multiThreadService.SetNumberOfPoolThreads(3);

    bool ok = multiThreadService.CreateThreads();
    multiThreadService.Stop();
    ok &= (multiThreadService.CreateThreads() == ErrorManagement::NoError);
    multiThreadService.Stop();
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

bool MultiThreadServiceTest::TestSetPriorityClass() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    embeddedThread.SetNumberOfPoolThreads(3);
    embeddedThread.CreateThreads();
    bool ok = (embeddedThread.GetPriorityClass() == Threads::NormalPriorityClass);
    embeddedThread.SetPriorityClass(Threads::RealTimePriorityClass);
    ok &= (embeddedThread.GetPriorityClass() == Threads::RealTimePriorityClass);
    return ok;
}

bool MultiThreadServiceTest::TestSetPriorityLevel() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    embeddedThread.SetNumberOfPoolThreads(3);
    embeddedThread.CreateThreads();
    bool ok = (embeddedThread.GetPriorityLevel() == 0);
    embeddedThread.SetPriorityLevel(10);
    ok &= (embeddedThread.GetPriorityLevel() == 10);
    return ok;
}

bool MultiThreadServiceTest::TestSetStackSize() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    embeddedThread.SetNumberOfPoolThreads(3);
    embeddedThread.CreateThreads();
    bool ok = (embeddedThread.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    embeddedThread.SetStackSize(THREADS_DEFAULT_STACKSIZE * 2);
    ok &= (embeddedThread.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 2));
    return ok;
}

bool MultiThreadServiceTest::TestSetCPUMask() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    embeddedThread.SetNumberOfPoolThreads(3);
    embeddedThread.CreateThreads();
    bool ok = (embeddedThread.GetCPUMask() == UndefinedCPUs);
    embeddedThread.SetCPUMask(0x3);
    ok &= (embeddedThread.GetCPUMask() == 0x3);
    return ok;
}

bool MultiThreadServiceTest::TestSetPriorityClass_Start() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService service(binder);
    service.SetNumberOfPoolThreads(3);
    bool ok = (service.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok &= (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::RunningState) || (service.GetStatus(1) != EmbeddedThreadI::RunningState)
            || (service.GetStatus(2) != EmbeddedThreadI::RunningState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::RunningState);
    ok &= (service.GetPriorityClass() == Threads::NormalPriorityClass);
    service.SetPriorityClass(Threads::RealTimePriorityClass);
    ok &= (service.GetPriorityClass() == Threads::NormalPriorityClass);
    service.Stop();
    maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::OffState) || (service.GetStatus(1) != EmbeddedThreadI::OffState)
            || (service.GetStatus(2) != EmbeddedThreadI::OffState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.SetPriorityClass(Threads::RealTimePriorityClass);
    ok &= (service.GetPriorityClass() == Threads::RealTimePriorityClass);
    return ok;
}

bool MultiThreadServiceTest::TestSetPriorityLevel_Start() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService service(binder);
    service.SetNumberOfPoolThreads(3);
    bool ok = (service.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::RunningState) || (service.GetStatus(1) != EmbeddedThreadI::RunningState)
            || (service.GetStatus(2) != EmbeddedThreadI::RunningState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::RunningState);
    ok &= (service.GetPriorityLevel() == 0);
    service.SetPriorityLevel(10);
    ok &= (service.GetPriorityLevel() == 0);
    service.Stop();
    maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::OffState) || (service.GetStatus(1) != EmbeddedThreadI::OffState)
            || (service.GetStatus(2) != EmbeddedThreadI::OffState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.SetPriorityLevel(10);
    ok &= (service.GetPriorityLevel() == 10);
    return ok;
}

bool MultiThreadServiceTest::TestSetStackSize_Start() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService service(binder);
    service.SetNumberOfPoolThreads(3);
    bool ok = (service.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::RunningState) || (service.GetStatus(1) != EmbeddedThreadI::RunningState)
            || (service.GetStatus(2) != EmbeddedThreadI::RunningState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    service.SetStackSize(THREADS_DEFAULT_STACKSIZE * 2);
    ok &= (service.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    service.Stop();
    maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::OffState) || (service.GetStatus(1) != EmbeddedThreadI::OffState)
            || (service.GetStatus(2) != EmbeddedThreadI::OffState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.SetStackSize(THREADS_DEFAULT_STACKSIZE * 2);
    ok &= (service.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 2));
    return ok;
}

bool MultiThreadServiceTest::TestSetCPUMask_Start() {
    using namespace MARTe;
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService service(binder);
    service.SetNumberOfPoolThreads(3);
    bool ok = (service.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::RunningState) || (service.GetStatus(1) != EmbeddedThreadI::RunningState)
            || (service.GetStatus(2) != EmbeddedThreadI::RunningState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::RunningState);
    ok &= (service.GetCPUMask() == UndefinedCPUs);
    service.SetCPUMask(0xfe);
    ok &= (service.GetCPUMask() == UndefinedCPUs);
    service.Stop();
    maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::OffState) || (service.GetStatus(1) != EmbeddedThreadI::OffState)
            || (service.GetStatus(2) != EmbeddedThreadI::OffState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.SetCPUMask(0xfe);
    ok &= (service.GetCPUMask() == 0xfe);
    return ok;
}

bool MultiThreadServiceTest::TestSetPriorityClassThreadPool() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    embeddedThread.SetNumberOfPoolThreads(10);
    embeddedThread.CreateThreads();
    bool ok = true;
    uint32 i;
    for (i = 0; i < 10; i++) {
        ok &= (embeddedThread.GetPriorityClassThreadPool(i) == Threads::NormalPriorityClass);
    }
    embeddedThread.SetPriorityClassThreadPool(Threads::RealTimePriorityClass, 0);
    embeddedThread.SetPriorityClassThreadPool(Threads::RealTimePriorityClass, 9);
    for (i = 1; i < 9; i++) {
        ok &= (embeddedThread.GetPriorityClassThreadPool(i) == Threads::NormalPriorityClass);
    }
    ok &= (embeddedThread.GetPriorityClassThreadPool(0) == Threads::RealTimePriorityClass);
    ok &= (embeddedThread.GetPriorityClassThreadPool(9) == Threads::RealTimePriorityClass);
    return ok;
}

bool MultiThreadServiceTest::TestSetPriorityLevelThreadPool() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    embeddedThread.SetNumberOfPoolThreads(10);
    embeddedThread.CreateThreads();
    bool ok = true;
    uint32 i;
    for (i = 0; i < 10; i++) {
        ok &= (embeddedThread.GetPriorityLevelThreadPool(i) == 0);
    }
    embeddedThread.SetPriorityLevelThreadPool(5, 0);
    embeddedThread.SetPriorityLevelThreadPool(8, 9);
    for (i = 1; i < 9; i++) {
        ok &= (embeddedThread.GetPriorityLevelThreadPool(i) == 0);
    }
    ok &= (embeddedThread.GetPriorityLevelThreadPool(0) == 5);
    ok &= (embeddedThread.GetPriorityLevelThreadPool(9) == 8);
    return ok;
}

bool MultiThreadServiceTest::TestSetStackSizeThreadPool() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    embeddedThread.SetNumberOfPoolThreads(10);
    embeddedThread.CreateThreads();
    bool ok = true;
    uint32 i;
    for (i = 0; i < 10; i++) {
        ok &= (embeddedThread.GetStackSizeThreadPool(i) == THREADS_DEFAULT_STACKSIZE);
    }
    embeddedThread.SetStackSizeThreadPool(THREADS_DEFAULT_STACKSIZE * 2, 0);
    embeddedThread.SetStackSizeThreadPool(THREADS_DEFAULT_STACKSIZE * 3, 9);
    for (i = 1; i < 9; i++) {
        ok &= (embeddedThread.GetStackSizeThreadPool(i) == THREADS_DEFAULT_STACKSIZE);
    }
    ok &= (embeddedThread.GetStackSizeThreadPool(0) == (THREADS_DEFAULT_STACKSIZE * 2));
    ok &= (embeddedThread.GetStackSizeThreadPool(9) == (THREADS_DEFAULT_STACKSIZE * 3));
    return ok;
}

bool MultiThreadServiceTest::TestSetThreadNameThreadPool() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    uint32 nOfThreads = 5;
    embeddedThread.SetNumberOfPoolThreads(nOfThreads);
    embeddedThread.CreateThreads();
    StreamString *threadNames = new StreamString[nOfThreads];
    uint32 i;
    for (i = 0; i < nOfThreads; i++) {
        threadNames[i].Printf("TestSetCPUMaskThreadPool_%d", i);
        embeddedThread.SetThreadNameThreadPool(threadNames[i].Buffer(), i);
    }
    bool ok = true;
    for (i = 0; (i < nOfThreads) && (ok); i++) {
        StreamString expectedName;
        expectedName.Printf("TestSetCPUMaskThreadPool_%d", i);
        ok = (expectedName == embeddedThread.GetThreadNameThreadPool(i));
    }
    delete [] threadNames;
    return ok;
}


bool MultiThreadServiceTest::TestSetCPUMaskThreadPool() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    embeddedThread.SetNumberOfPoolThreads(10);
    embeddedThread.CreateThreads();
    bool ok = true;
    uint32 i;
    for (i = 0; i < 10; i++) {
        ok &= (embeddedThread.GetCPUMaskThreadPool(i) == UndefinedCPUs);
    }
    embeddedThread.SetCPUMaskThreadPool(0x4, 0);
    embeddedThread.SetCPUMaskThreadPool(0x8, 9);
    for (i = 1; i < 9; i++) {
        ok &= (embeddedThread.GetCPUMaskThreadPool(i) == UndefinedCPUs);
    }
    ok &= (embeddedThread.GetCPUMaskThreadPool(0) == 0x4);
    ok &= (embeddedThread.GetCPUMaskThreadPool(9) == 0x8);
    return ok;
}

bool MultiThreadServiceTest::TestGetPriorityClassThreadPool() {
    return TestSetPriorityClassThreadPool();
}

bool MultiThreadServiceTest::TestGetPriorityLevelThreadPool() {
    return TestSetPriorityLevelThreadPool();
}

bool MultiThreadServiceTest::TestGetStackSizeThreadPool() {
    return TestSetStackSizeThreadPool();
}

bool MultiThreadServiceTest::TestGetThreadNameThreadPool() {
    return TestSetThreadNameThreadPool();
}

bool MultiThreadServiceTest::TestGetCPUMaskThreadPool() {
    return TestSetCPUMaskThreadPool();
}

bool MultiThreadServiceTest::TestGetPriorityClassThreadPool_OutOfIndex() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    return (embeddedThread.GetPriorityClassThreadPool(1) == Threads::UnknownPriorityClass);
}

bool MultiThreadServiceTest::TestGetPriorityLevelThreadPool_OutOfIndex() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    return (embeddedThread.GetPriorityLevelThreadPool(1) == 0);
}

bool MultiThreadServiceTest::TestGetStackSizeThreadPool_OutOfIndex() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    return (embeddedThread.GetStackSizeThreadPool(1) == 0);
}


bool MultiThreadServiceTest::TestGetCPUMaskThreadPool_OutOfIndex() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService embeddedThread(binder);
    return (embeddedThread.GetCPUMaskThreadPool(1) == UndefinedCPUs);
}

bool MultiThreadServiceTest::TestSetPriorityLevelThreadPool_Start() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService service(binder);
    service.SetNumberOfPoolThreads(3);
    bool ok = (service.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::RunningState) || (service.GetStatus(1) != EmbeddedThreadI::RunningState)
            || (service.GetStatus(2) != EmbeddedThreadI::RunningState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::RunningState);
    ok &= (service.GetPriorityLevelThreadPool(1) == 0);
    service.SetPriorityLevelThreadPool(10, 1);
    ok &= (service.GetPriorityLevelThreadPool(1) == 0);
    service.Stop();
    maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::OffState) || (service.GetStatus(1) != EmbeddedThreadI::OffState)
            || (service.GetStatus(2) != EmbeddedThreadI::OffState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.CreateThreads();
    service.SetPriorityLevelThreadPool(10, 1);
    ok &= (service.GetPriorityLevelThreadPool(1) == 10);
    return ok;
}

bool MultiThreadServiceTest::TestSetPriorityClassThreadPool_Start() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService service(binder);
    service.SetNumberOfPoolThreads(3);
    bool ok = (service.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::RunningState) || (service.GetStatus(1) != EmbeddedThreadI::RunningState)
            || (service.GetStatus(2) != EmbeddedThreadI::RunningState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::RunningState);
    ok &= (service.GetPriorityClassThreadPool(1) == Threads::NormalPriorityClass);
    service.SetPriorityClassThreadPool(Threads::RealTimePriorityClass, 1);
    ok &= (service.GetPriorityClassThreadPool(1) == Threads::NormalPriorityClass);
    service.Stop();
    maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::OffState) || (service.GetStatus(1) != EmbeddedThreadI::OffState)
            || (service.GetStatus(2) != EmbeddedThreadI::OffState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.CreateThreads();
    service.SetPriorityClassThreadPool(Threads::RealTimePriorityClass, 1);
    ok &= (service.GetPriorityClassThreadPool(1) == Threads::RealTimePriorityClass);
    return ok;
}

bool MultiThreadServiceTest::TestSetCPUMaskThreadPool_Start() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService service(binder);
    service.SetNumberOfPoolThreads(3);
    bool ok = (service.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::RunningState) || (service.GetStatus(1) != EmbeddedThreadI::RunningState)
            || (service.GetStatus(2) != EmbeddedThreadI::RunningState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::RunningState);
    ok &= (service.GetCPUMaskThreadPool(1) == UndefinedCPUs);
    service.SetCPUMaskThreadPool(0xe, 1);
    ok &= (service.GetCPUMaskThreadPool(1) == UndefinedCPUs);
    service.Stop();
    maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::OffState) || (service.GetStatus(1) != EmbeddedThreadI::OffState)
            || (service.GetStatus(2) != EmbeddedThreadI::OffState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.CreateThreads();
    service.SetCPUMaskThreadPool(0xe, 1);
    ok &= (service.GetCPUMaskThreadPool(1) == 0xe);
    return ok;
}

bool MultiThreadServiceTest::TestSetStackSizeThreadPool_Start() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService service(binder);
    service.SetNumberOfPoolThreads(3);
    bool ok = (service.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::OffState);

    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::RunningState) || (service.GetStatus(1) != EmbeddedThreadI::RunningState)
            || (service.GetStatus(2) != EmbeddedThreadI::RunningState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStackSizeThreadPool(1) == THREADS_DEFAULT_STACKSIZE);
    service.SetStackSizeThreadPool(THREADS_DEFAULT_STACKSIZE * 2, 1);
    ok &= (service.GetStackSizeThreadPool(1) == THREADS_DEFAULT_STACKSIZE);
    service.Stop();
    maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::OffState) || (service.GetStatus(1) != EmbeddedThreadI::OffState)
            || (service.GetStatus(2) != EmbeddedThreadI::OffState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.CreateThreads();
    service.SetStackSizeThreadPool(THREADS_DEFAULT_STACKSIZE * 2, 1);
    ok &= (service.GetStackSizeThreadPool(1) == THREADS_DEFAULT_STACKSIZE * 2);
    return ok;
}

bool MultiThreadServiceTest::TestSetThreadNameThreadPool_Start() {
    using namespace MARTe;
    MultiThreadServiceTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<MultiThreadServiceTestCallbackClass> binder(callbackClass, &MultiThreadServiceTestCallbackClass::CallbackFunction);
    MultiThreadService service(binder);
    service.SetNumberOfPoolThreads(3);
    service.CreateThreads();
    bool ok = (service.GetStatus(0) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::OffState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::OffState);

    service.SetThreadNameThreadPool("A", 0);
    service.SetThreadNameThreadPool("B", 1);
    service.SetThreadNameThreadPool("C", 2);
    ErrorManagement::ErrorType err = service.Start();
    ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::RunningState) || (service.GetStatus(1) != EmbeddedThreadI::RunningState)
            || (service.GetStatus(2) != EmbeddedThreadI::RunningState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    ok &= (service.GetStatus(0) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(1) == EmbeddedThreadI::RunningState);
    ok &= (service.GetStatus(2) == EmbeddedThreadI::RunningState);
    StreamString threadName = service.GetThreadNameThreadPool(1);
    ok &= (threadName == "B"); 
    service.SetThreadNameThreadPool("ShouldNotWork", 1);
    threadName = service.GetThreadNameThreadPool(1);
    ok &= (threadName == "B"); 
    service.Stop();
    maxCounter = 10;
    while (((service.GetStatus(0) != EmbeddedThreadI::OffState) || (service.GetStatus(1) != EmbeddedThreadI::OffState)
            || (service.GetStatus(2) != EmbeddedThreadI::OffState)) && (maxCounter > 0)) {
        maxCounter--;
        Sleep::Sec(1);
    }
    service.CreateThreads();
    service.SetThreadNameThreadPool("B", 1);
    threadName = service.GetThreadNameThreadPool(1);
    ok &= (threadName == "B"); 
    return ok;
}

