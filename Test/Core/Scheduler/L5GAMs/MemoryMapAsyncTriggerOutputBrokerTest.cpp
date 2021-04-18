/**
 * @file MemoryMapAsyncTriggerOutputBrokerTest.cpp
 * @brief Source file for class MemoryMapAsyncTriggerOutputBrokerTest
 * @date 24/01/2017
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
 * the class MemoryMapAsyncTriggerOutputBrokerTest (public, protected, and private). Be aware that some
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
#include "DataSourceI.h"
#include "GAM.h"
#include <MemoryMapAsyncTriggerOutputBroker.h>
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "MemoryMapAsyncTriggerOutputBrokerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAM which generates a given trigger and signal pattern which is then confirmed by the DataSourceI
 */
class MemoryMapAsyncTriggerOutputBrokerGAMTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()MemoryMapAsyncTriggerOutputBrokerGAMTestHelper() {
        counter = 0;
        triggerIdx = 0;
        numberOfExecutes = 0;
        signalToGenerate = NULL;
        triggerToGenerate = NULL;
        elements = NULL;
    }

    virtual ~MemoryMapAsyncTriggerOutputBrokerGAMTestHelper() {
        using namespace MARTe;
        if (signalToGenerate != NULL) {
            delete [] signalToGenerate;
        }
        if (elements != NULL) {
            delete [] elements;
        }
        if (triggerToGenerate != NULL) {
            delete [] triggerToGenerate;
        }
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        using namespace MARTe;
        bool ok = GAM::Initialise(data);
        AnyType triggerAT = data.GetType("Trigger");
        numberOfExecutes = triggerAT.GetNumberOfElements(0);
        triggerToGenerate = new uint8[numberOfExecutes];
        signalToGenerate = new uint32[numberOfExecutes];
        Vector<uint8> triggerV(triggerToGenerate, numberOfExecutes);
        Vector<uint32> signalV(signalToGenerate, numberOfExecutes);
        data.Read("Trigger", triggerV);
        data.Read("Signal", signalV);
        return ok;
    }

    virtual bool Setup() {
        using namespace MARTe;
        elements = new uint32[GetNumberOfOutputSignals()];
        uint32 n;
        for (n=0; n<GetNumberOfOutputSignals(); n++) {
            TypeDescriptor td = GetSignalType(OutputSignals, n);
            if (td == UnsignedInteger8Bit) {
                triggerIdx = n;
                elements[n] = 1;
            }
            else {
                uint32 el;
                GetSignalNumberOfElements(OutputSignals, n, el);
                elements[n] = el;
            }
        }
        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        uint8 *triggerOut = reinterpret_cast<uint8*>(GetOutputSignalMemory(triggerIdx));
        if (counter < numberOfExecutes) {
            *triggerOut = triggerToGenerate[counter];
            uint32 n;
            for (n=0; n<GetNumberOfOutputSignals(); n++) {
                if (n != triggerIdx) {
                    uint32 *signalOut = reinterpret_cast<uint32*>(GetOutputSignalMemory(n));
                    uint32 e;
                    for (e=0; e<elements[n]; e++) {
                        signalOut[e] = signalToGenerate[counter];
                    }
                }
            }

        }
        counter++;
        return true;
    }

    MARTe::uint32 triggerIdx;
    MARTe::uint8 *triggerToGenerate;
    MARTe::uint32 *signalToGenerate;
    MARTe::uint32 *elements;
    MARTe::uint32 counter;
    MARTe::uint32 numberOfExecutes;
};
CLASS_REGISTER(MemoryMapAsyncTriggerOutputBrokerGAMTestHelper, "1.0")
/**
 * @brief DataSourceI implementation which returns a MemoryMapBrokerTestHelper broker.
 * @details This DataSource will verify in run-time if the signals being written by the Broker are the ones
 *  expected any particular test configuration.
 */
class MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper: public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper() : MARTe::DataSourceI() {
        numberOfBuffers = 0u;
        preTriggerBuffers = 0u;
        postTriggerBuffers = 0u;
        offsets = NULL;
        totalNumberOfSignalElements = 0;
        signalMemory = NULL;
        cpuMask = 0xff;
        numberOfExecutes = 0;
        expectedSignal = NULL;
        expectedTrigger = NULL;
        counter = 0;
        memoryOK = true;
        stackSize = 65536;
        broker = NULL;
    }

    virtual ~MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper() {
        using namespace MARTe;
        if (signalMemory != NULL_PTR(void *)) {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemory);
        }
        if (offsets != NULL_PTR(uint32 *)) {
            delete[] offsets;
        }
        if (expectedSignal != NULL) {
            delete [] expectedSignal;
        }
        if (expectedTrigger != NULL) {
            delete [] expectedTrigger;
        }
    }

    virtual void Purge(ReferenceContainer &purgeList) {
        if (broker != NULL) {
            broker->UnlinkDataSource();
        }
        DataSourceI::Purge(purgeList);
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        using namespace MARTe;
        DataSourceI::Initialise(data);
        data.Read("NumberOfBuffers", numberOfBuffers);
        data.Read("PreTriggerBuffers", preTriggerBuffers);
        data.Read("PostTriggerBuffers", postTriggerBuffers);
        data.Read("CPUMask", cpuMask);
        data.Read("StackSize", stackSize);
        AnyType triggerAT = data.GetType("ExpectedTrigger");
        numberOfExecutes = triggerAT.GetNumberOfElements(0);
        expectedTrigger = new uint8[numberOfExecutes];
        expectedSignal = new uint32[numberOfExecutes];
        Vector<uint8> triggerV(expectedTrigger, numberOfExecutes);
        Vector<uint32> signalV(expectedSignal, numberOfExecutes);
        data.Read("ExpectedTrigger", triggerV);
        data.Read("ExpectedSignal", signalV);
        return true;

    }

    virtual bool AllocateMemory() {
        using namespace MARTe;
        uint32 numberOfSignals = GetNumberOfSignals();
        bool ret = (numberOfSignals > 0u);
        if (ret) {
            ret = (offsets == NULL_PTR(uint32 *));
        }
        if (ret) {
            offsets = new uint32[numberOfSignals];
        }
        uint32 totalMemorySize = 0u;
        for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
            uint32 memorySize;

            ret = GetSignalByteSize(s, memorySize);
            if (ret) {
                offsets[s] = totalMemorySize;
                totalMemorySize += memorySize;
            }
        }
        for (uint32 s = 1u; (s < numberOfSignals) && (ret); s++) {
            uint32 el;
            ret = GetSignalNumberOfElements(s, el);
            totalNumberOfSignalElements += el;
        }
        if (ret) {
            signalMemory = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalMemorySize);
            MemoryOperationsHelper::Set(signalMemory, 0, totalMemorySize);
        }
        return ret;
    }

    virtual MARTe::uint32 GetNumberOfMemoryBuffers() {
        return 1u;
    }

    virtual bool GetSignalMemoryBuffer(const MARTe::uint32 signalIdx,
            const MARTe::uint32 bufferIdx,
            void *&signalAddress) {
        using namespace MARTe;
        char8 *memPtr = reinterpret_cast<char8 *>(signalMemory);
        memPtr += offsets[signalIdx];
        signalAddress = reinterpret_cast<void *&>(memPtr);
        return true;
    }

    virtual const MARTe::char8 *GetBrokerName(MARTe::StructuredDataI &data,
            const MARTe::SignalDirection direction) {
        return "MemoryMapAsyncTriggerOutputBroker";
    }

    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
            const MARTe::char8 * const nextStateName) {
        return true;
    }

    virtual bool GetInputBrokers(
            ReferenceContainer &inputBrokers,
            const MARTe::char8* const functionName,
            void * const gamMemPtr) {
        return false;
    }

    virtual bool GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const MARTe::char8* const functionName,
            void * const gamMemPtr) {
        using namespace MARTe;
        ReferenceT<MARTe::MemoryMapAsyncTriggerOutputBroker> brokerNew = ReferenceT<MARTe::MemoryMapAsyncTriggerOutputBroker>("MemoryMapAsyncTriggerOutputBroker");
        bool ret = brokerNew.IsValid();
        if (ret) {
            ret = brokerNew->InitWithTriggerParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, preTriggerBuffers, postTriggerBuffers, cpuMask, stackSize);
        }
        if (ret) {
            ret = outputBrokers.Insert(brokerNew);
            broker = brokerNew.operator ->();
        }
        return ret;
    }

    //Check that the memory is indeed set as expected
    virtual bool Synchronise() {
        using namespace MARTe;
        if (counter < numberOfExecutes) {
            uint8 *trigger = static_cast<uint8 *>(signalMemory);
            uint32 *signalMemory32 = reinterpret_cast<uint32 *>(&trigger[1]);
            if (memoryOK) {
                memoryOK &= (*trigger == expectedTrigger[counter]);
                uint32 n;
                for (n=0; n<totalNumberOfSignalElements; n++) {
                    memoryOK &= (signalMemory32[n] == expectedSignal[counter]);
                }
            }
        }
        else {
            memoryOK = false;
        }
        counter++;
        return memoryOK;
    }

    void Reset() {
        counter = 0;
        broker->ResetPreTriggerBuffers();
    }

    MARTe::uint32 numberOfBuffers;
    MARTe::uint32 preTriggerBuffers;
    MARTe::uint32 postTriggerBuffers;
    MARTe::uint32 *offsets;
    MARTe::uint32 cpuMask;
    MARTe::uint32 stackSize;
    MARTe::uint8 *expectedTrigger;
    MARTe::uint32 *expectedSignal;
    MARTe::uint32 numberOfExecutes;
    MARTe::uint32 totalNumberOfSignalElements;
    MARTe::uint32 counter;
    MARTe::MemoryMapAsyncTriggerOutputBroker *broker;
    bool memoryOK;
    void *signalMemory;
};
CLASS_REGISTER(MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper, "1.0")

/**
 * @brief Manual scheduler to test the correct interface between the Broker and the DataSourceI
 */
class MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper    () : MARTe::GAMSchedulerI() {
        scheduledStates = NULL;
    }

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    void ExecuteThreadCycle(MARTe::uint32 threadId) {
        using namespace MARTe;
        ReferenceT<RealTimeApplication> realTimeAppT = realTimeApp;
        ExecuteSingleCycle(scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].executables,
                scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].numberOfExecutables);
    }

    virtual bool ConfigureScheduler(MARTe::Reference realTimeApp) {

        bool ret = GAMSchedulerI::ConfigureScheduler(realTimeApp);
        if (ret) {
            scheduledStates = GetSchedulableStates();
        }
        return ret;
    }

    virtual void CustomPrepareNextState() {
    }

private:

    MARTe::ScheduledState * const * scheduledStates;
};

CLASS_REGISTER(MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper, "1.0")

/**
 * Loads a configuration file and checks that all the components can be successfully created.
 */
static bool TestIntegratedInApplication(const MARTe::char8 * const config, bool destroy) {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    if (destroy) {
        god->Purge();
    }
    return ok;
}

/**
 * Runs a mini MARTe application against the provide config. It patches the input configuration file with signalToGenerate, trigger and buffering parameters
 *  and then it verifies that the broker correctly propagates this signal to the DataSource.
 */
static bool TestExecute_PreTriggerBuffers_PostTriggerBuffers(const MARTe::char8 * const config, MARTe::uint8* triggerToGenerate, MARTe::uint32 *signalToGenerate, MARTe::uint32 toGenerateNumberOfElements, MARTe::uint8* expectedTrigger,
                                                             MARTe::uint32* expectedSignal, MARTe::uint32 expectedNumberOfElements, MARTe::uint32 preTriggerBuffers, MARTe::uint32 postTriggerBuffers, MARTe::uint32 numberOfBuffers,
                                                             MARTe::uint32 sleepMSec = 10) {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    cdb.MoveAbsolute("$Test.+Functions.+GAM1");
    cdb.Delete("Trigger");
    cdb.Delete("Signal");
    Vector<uint8> triggerV(triggerToGenerate, toGenerateNumberOfElements);
    Vector<uint32> signalV(signalToGenerate, toGenerateNumberOfElements);
    cdb.Write("Trigger", triggerV);
    cdb.Write("Signal", signalV);

    Vector<uint8> expectedTriggerV(expectedTrigger, expectedNumberOfElements);
    Vector<uint32> expectedSignalV(expectedSignal, expectedNumberOfElements);
    cdb.MoveAbsolute("$Test.+Data.+Drv1");
    cdb.Delete("ExpectedTrigger");
    cdb.Delete("ExpectedSignal");
    cdb.Write("ExpectedTrigger", expectedTriggerV);
    cdb.Write("ExpectedSignal", expectedSignalV);
    cdb.Delete("PreTriggerBuffers");
    cdb.Delete("PostTriggerBuffers");
    cdb.Delete("NumberOfBuffers");
    cdb.Write("PreTriggerBuffers", preTriggerBuffers);
    cdb.Write("PostTriggerBuffers", postTriggerBuffers);
    cdb.Write("NumberOfBuffers", numberOfBuffers);
    cdb.MoveToRoot();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    if (ok) {
        dataSource = application->Find("Data.Drv1");
        ok = dataSource.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }

    uint32 i;
    for (i = 0; (i < gam->numberOfExecutes) && (ok); i++) {
        scheduler->ExecuteThreadCycle(0);
        Sleep::MSec(sleepMSec);
    }

    //2 seconds to finish
    uint64 maxTimeInCounts = HighResolutionTimer::Counter() + 2 * HighResolutionTimer::Frequency();
    //Force flushing of BufferLoop
    scheduler->ExecuteThreadCycle(0);
    while (dataSource->counter != dataSource->numberOfExecutes) {
        Sleep::Sec(0.1);
        if (HighResolutionTimer::Counter() > maxTimeInCounts) {
            ok = false;
            break;
        }
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        ok = dataSource->memoryOK;
    }

    godb->Purge();
    return ok;
}

//1 PreTrigger
static const MARTe::char8 * const config0 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 1"
        "            PostTriggerBuffers = 0"
        "            CPUMask = 15"
        "            ExpectedTrigger = {0 1 0 1 0 1 0 1 1}"
        "            ExpectedSignal =  {1 2 4 5 8 9 8 7 6}"
        "            Signals = {"
        "                Trigger = {"
        "                    Type = uint8"
        "                }"
        "                Signal1 = {"
        "                   Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//1 PreTrigger
static const MARTe::char8 * const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 1"
        "            PostTriggerBuffers = 0"
        "            CPUMask = 15"
        "            ExpectedTrigger = {0 1 0 1 0 1 0 1 1}"
        "            ExpectedSignal =  {1 2 4 5 8 9 8 7 6}"
        "            Signals = {"
        "                Trigger = {"
        "                    Type = uint8"
        "                }"
        "                Signal1 = {"
        "                   Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Wrong NumberOfBuffers
static const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 0"
        "            PreTriggerBuffers = 1"
        "            PostTriggerBuffers = 0"
        "            CPUMask = 15"
        "            ExpectedTrigger = {0 1 0 1 0 1 0 1 1}"
        "            ExpectedSignal =  {1 2 4 5 8 9 8 7 6}"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//PreTriggerBuffers > NumberOfBuffers
static const MARTe::char8 * const config3 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 10"
        "            PostTriggerBuffers = 0"
        "            CPUMask = 15"
        "            ExpectedTrigger = {0 1 0 1 0 1 0 1 1}"
        "            ExpectedSignal =  {1 2 4 5 8 9 8 7 6}"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//PostTriggerBuffers > NumberOfBuffers
static const MARTe::char8 * const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 1"
        "            PostTriggerBuffers = 10"
        "            CPUMask = 15"
        "            ExpectedTrigger = {0 1 0 1 0 1 0 1 1}"
        "            ExpectedSignal =  {1 2 4 5 8 9 8 7 6}"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//PreTriggerBuffers + PostTriggerBuffers > NumberOfBuffers
static const MARTe::char8 * const config5 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 5"
        "            PostTriggerBuffers = 5"
        "            CPUMask = 15"
        "            ExpectedTrigger = {0 1 0 1 0 1 0 1 1}"
        "            ExpectedSignal =  {1 2 4 5 8 9 8 7 6}"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//More than one GAM writing to the same DataSourceI (and as a consequence to the same MemoryMapAsyncTriggerOutputBroker)
static const MARTe::char8 * const config6 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 1"
        "            PostTriggerBuffers = 1"
        "            CPUMask = 15"
        "            ExpectedTrigger = {0 1 0 1 0 1 0 1 1}"
        "            ExpectedSignal =  {1 2 4 5 8 9 8 7 6}"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1 GAM2}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Samples > 1
static const MARTe::char8 * const config7 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 0"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 3"
        "            PostTriggerBuffers = 4"
        "            CPUMask = 15"
        "            ExpectedTrigger = {0 1 0 1 0 1 0 1 1}"
        "            ExpectedSignal =  {1 2 4 5 8 9 8 7 6}"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Wrong type
static const MARTe::char8 * const config8 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 2"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 5"
        "            PostTriggerBuffers = 5"
        "            CPUMask = 15"
        "            ExpectedTrigger = {0 1 0 1 0 1 0 1 1}"
        "            ExpectedSignal =  {1 2 4 5 8 9 8 7 6}"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//To force buffer overrun
static const MARTe::char8 * const config9 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 1"
        "            PreTriggerBuffers = 0"
        "            PostTriggerBuffers = 0"
        "            CPUMask = 15"
        "            ExpectedTrigger = {0 1 0 1 0 1 0 1 1}"
        "            ExpectedSignal =  {1 2 4 5 8 9 8 7 6}"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

static const MARTe::char8 * const config10 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerGAMTestHelper"
        "            Trigger = {0 0 1 0 0 1 0 0 0 1 0 1 1 0}"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
        "               Trigger = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 4"
        "            PostTriggerBuffers = 3"
        "            CPUMask = 15"
        "            StackSize = 32768"
        "            ExpectedTrigger = {0 1 0 1 0 1 0 1 1}"
        "            ExpectedSignal =  {1 2 4 5 8 9 8 7 6}"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool MemoryMapAsyncTriggerOutputBrokerTest::TestConstructor() {
    using namespace MARTe;
    MemoryMapAsyncTriggerOutputBroker broker;
    return true;
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestInit() {
    using namespace MARTe;
    MemoryMapAsyncTriggerOutputBroker broker;
    MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper ignore;
    return !broker.Init(OutputSignals, ignore, "", NULL);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestInitWithTriggerParameters() {
    using namespace MARTe;
    return TestIntegratedInApplication(config1, true);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestInitWithTriggerParameters_False_0_NumberOfBuffers() {
    using namespace MARTe;
    return !TestIntegratedInApplication(config2, true);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestInitWithTriggerParameters_False_PreTriggerGreaterThanNumberOfBuffers() {
    using namespace MARTe;
    return !TestIntegratedInApplication(config3, true);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestInitWithTriggerParameters_False_PostTriggerGreaterThanNumberOfBuffers() {
    using namespace MARTe;
    return !TestIntegratedInApplication(config4, true);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestInitWithTriggerParameters_False_PrePlusPostTriggerGreaterThanNumberOfBuffers() {
    using namespace MARTe;
    return !TestIntegratedInApplication(config5, true);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestInitWithTriggerParameters_False_MoreThanOneGAM() {
    using namespace MARTe;
    return !TestIntegratedInApplication(config6, true);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestInitWithTriggerParameters_False_Samples() {
    using namespace MARTe;
    return !TestIntegratedInApplication(config7, true);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestInitWithTriggerParameters_False_WrongTriggerSignalType() {
    using namespace MARTe;
    return !TestIntegratedInApplication(config8, true);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_ManyCycles() {
    using namespace MARTe;
    const uint32 numberOfCycles = 2400;
    const uint32 numberOfBuffers = 10;
    const uint32 numberOfPreTriggers = 2;
    const uint32 numberOfPostTriggers = 3;
    const uint32 triggerEveryNCycles = 23;
    const uint32 numberOfTriggers = (numberOfCycles / triggerEveryNCycles);
    const uint32 numberOfExpectedElements = (numberOfPreTriggers + numberOfPostTriggers + 1) * numberOfTriggers;

    uint8 *triggerToGenerate = new uint8[numberOfCycles];
    uint32 *signalToGenerate = new uint32[numberOfCycles];
    uint8 *expectedTrigger = new uint8[numberOfExpectedElements];
    uint32 *expectedSignal = new uint32[numberOfExpectedElements];
    uint32 i;
    uint32 j = 0;
    uint32 z;
    for (i = 0; i < numberOfCycles; i++) {
        signalToGenerate[i] = i;
        if ((i > 0) && ((i % triggerEveryNCycles) == 0)) {
            triggerToGenerate[i] = 1;
            for (z = 0; z < numberOfPreTriggers; z++) {
                expectedTrigger[j] = 0;
                expectedSignal[j] = (i - numberOfPreTriggers + z);
                j++;
            }
            expectedTrigger[j] = 1;
            expectedSignal[j] = i;
            j++;
            for (z = 0; z < numberOfPostTriggers; z++) {
                expectedTrigger[j] = 0;
                expectedSignal[j] = (i + z + 1);
                j++;
            }

        }
        else {
            triggerToGenerate[i] = 0;
        }
    }
    bool ok = TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, numberOfCycles, expectedTrigger, expectedSignal, numberOfExpectedElements, numberOfPreTriggers, numberOfPostTriggers,
                                                               numberOfBuffers, 2);
    delete triggerToGenerate;
    delete signalToGenerate;
    delete expectedTrigger;
    delete expectedSignal;
    return ok;
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_1_PreTriggerBuffers_0_PostTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };
    uint8 expectedTrigger[] = { 0, 1, 0, 1, 0, 1, 0, 1, 1 };
    uint32 expectedSignal[] = { 1, 2, 4, 5, 8, 9, 8, 7, 6 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 1, 0, 10);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_1_PreTriggerBuffers_0_PostTriggerBuffers_TriggerNotGAMFirstSignal() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };
    uint8 expectedTrigger[] = { 0, 1, 0, 1, 0, 1, 0, 1, 1 };
    uint32 expectedSignal[] = { 1, 2, 4, 5, 8, 9, 8, 7, 6 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config0, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 1, 0, 10);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_1_PreTriggerBuffers_1_PostTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };
    uint8 expectedTrigger[] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0 };
    uint32 expectedSignal[] = { 1, 2, 3, 4, 5, 6, 8, 9, 8, 7, 6, 5 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 1, 1, 10);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_0_PreTriggerBuffers_1_PostTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };
    uint8 expectedTrigger[] = { 1, 0, 1, 0, 1, 0, 1, 1, 0 };
    uint32 expectedSignal[] = { 2, 3, 5, 6, 9, 8, 7, 6, 5 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 0, 1, 10);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };
    uint8 expectedTrigger[] = { 1, 1, 1, 1, 1 };
    uint32 expectedSignal[] = { 2, 5, 9, 7, 6 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 0, 0, 10);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers_AlwaysTriggering() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3 };
    uint8 expectedTrigger[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    uint32 expectedSignal[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 0, 0, 10);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint32 signalToGenerate[] = { 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5 };
    uint8 expectedTrigger[] = { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 };
    uint32 expectedSignal[] = { 9, 1, 2, 3, 8, 7, 6, 5, 4, 3, 2 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 4, 2, 10);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_PreTriggerIncludesBeforeZero() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint32 signalToGenerate[] = { 1, 2, 3, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5 };
    uint8 expectedTrigger[] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 };
    uint32 expectedSignal[] = { 1, 2, 3, 9, 1, 2, 3, 8, 7, 6, 5, 4, 3, 2 };

    bool ok = TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 4, 2, 10);
    uint8 triggerToGenerate2[] = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint32 signalToGenerate2[] =  { 2, 3, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5 };
    uint8 expectedTrigger2[] = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 };
    uint32 expectedSignal2[] =  { 2, 3, 9, 1, 2, 3, 8, 7, 6, 5, 4, 3, 2 };

    if (ok) {
        ok = TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate2, signalToGenerate2, sizeof(triggerToGenerate2) / sizeof(uint8), expectedTrigger2, expectedSignal2, sizeof(expectedTrigger2) / sizeof(uint8), 4, 2, 10);
    }

    uint8 triggerToGenerate3[] = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint32 signalToGenerate3[] =  { 3, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5 };
    uint8 expectedTrigger3[] = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 };
    uint32 expectedSignal3[] =  { 3, 9, 1, 2, 3, 8, 7, 6, 5, 4, 3, 2 };

    if (ok) {
        ok = TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate3, signalToGenerate3, sizeof(triggerToGenerate3) / sizeof(uint8), expectedTrigger3, expectedSignal3, sizeof(expectedTrigger3) / sizeof(uint8), 4, 2, 10);
    }

    uint8 triggerToGenerate4[] = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint32 signalToGenerate4[] =  { 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5 };
    uint8 expectedTrigger4[] = { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 };
    uint32 expectedSignal4[] =  { 9, 1, 2, 3, 8, 7, 6, 5, 4, 3, 2 };

    if (ok) {
        ok = TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate4, signalToGenerate4, sizeof(triggerToGenerate4) / sizeof(uint8), expectedTrigger4, expectedSignal4, sizeof(expectedTrigger4) / sizeof(uint8), 4, 2, 10);
    }

    uint8 triggerToGenerate5[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint32 signalToGenerate5[] =  { 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5 };
    uint8 expectedTrigger5[] = { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 };
    uint32 expectedSignal5[] =  { 1, 2, 3, 8, 7, 6, 5, 4, 3, 2 };

    if (ok) {
        ok = TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate5, signalToGenerate5, sizeof(triggerToGenerate5) / sizeof(uint8), expectedTrigger5, expectedSignal5, sizeof(expectedTrigger5) / sizeof(uint8), 4, 2, 10);
    }

    return ok;
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_AlwaysTriggering() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 0, 0, 0, 0, 0 };
    uint8 expectedTrigger[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 };
    uint32 expectedSignal[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 0, 0 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 3, 2, 10);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_N_PreTriggerBuffers_0_PostTriggerBuffers_OverwritingPreTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1 };
    uint32 signalToGenerate[] = { 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6 };
    uint8 expectedTrigger[] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    uint32 expectedSignal[] = { 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 5, 4, 3, 2, 0, 1, 2, 3 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 3, 0, 10);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_0_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPostTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1 };
    uint32 signalToGenerate[] = { 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6 };
    uint8 expectedTrigger[] = { 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1 };
    uint32 expectedSignal[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 2, 1, 0, 1, 3, 4, 5, 6 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 0, 3, 10);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPreAndPostTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1 };
    uint32 signalToGenerate[] = { 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6 };
    uint8 expectedTrigger[] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 };
    uint32 expectedSignal[] = { 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 3, 3, 10);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_1_Buffer_AlwaysTriggering() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 1 };
    uint32 signalToGenerate[] = { 8 };
    uint8 expectedTrigger[] = { 1 };
    uint32 expectedSignal[] = { 8 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8), expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 0, 0, 1);
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestGetCPUMask() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config10;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    if (ok) {
        dataSource = application->Find("Data.Drv1");
        ok = dataSource.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }

    ReferenceContainer brokers;
    char8 *fakeMem = new char8[1024];
    dataSource->GetOutputBrokers(brokers, "GAM1", fakeMem);
    ReferenceT<MemoryMapAsyncTriggerOutputBroker> broker = brokers.Get(0);
    if (ok) {
        ok = (broker->GetCPUMask() == 0xf);
    }
    delete fakeMem;

    godb->Purge();
    return ok;
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestGetNumberOfBuffers() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config10;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    if (ok) {
        dataSource = application->Find("Data.Drv1");
        ok = dataSource.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }

    ReferenceContainer brokers;
    char8 *fakeMem = new char8[1024];
    dataSource->GetOutputBrokers(brokers, "GAM1", fakeMem);
    ReferenceT<MemoryMapAsyncTriggerOutputBroker> broker = brokers.Get(0);
    if (ok) {
        ok = (broker->GetNumberOfBuffers() == 10);
    }
    delete fakeMem;

    godb->Purge();
    return ok;
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestGetPreTriggerBuffers() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config10;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    if (ok) {
        dataSource = application->Find("Data.Drv1");
        ok = dataSource.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }

    ReferenceContainer brokers;
    char8 *fakeMem = new char8[1024];
    dataSource->GetOutputBrokers(brokers, "GAM1", fakeMem);
    ReferenceT<MemoryMapAsyncTriggerOutputBroker> broker = brokers.Get(0);
    if (ok) {
        ok = (broker->GetPreTriggerBuffers() == 4);
    }
    delete fakeMem;

    godb->Purge();
    return ok;
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestGetPostTriggerBuffers() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config10;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    if (ok) {
        dataSource = application->Find("Data.Drv1");
        ok = dataSource.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }

    ReferenceContainer brokers;
    char8 *fakeMem = new char8[1024];
    dataSource->GetOutputBrokers(brokers, "GAM1", fakeMem);
    ReferenceT<MemoryMapAsyncTriggerOutputBroker> broker = brokers.Get(0);
    if (ok) {
        ok = (broker->GetPostTriggerBuffers() == 3);
    }
    delete fakeMem;

    godb->Purge();
    return ok;
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestExecute_Buffer_Overrun() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config9;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    if (ok) {
        dataSource = application->Find("Data.Drv1");
        ok = dataSource.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }

    ReferenceContainer brokers;
    char8 *fakeMem = new char8[1024];
    uint32 n;
    for (n=0u; n<1024; n++) {
        fakeMem[n] = 1;
    }
    dataSource->GetOutputBrokers(brokers, "GAM1", fakeMem);
    ReferenceT<MemoryMapAsyncTriggerOutputBroker> broker = brokers.Get(0);
    if (ok) {
        ok = broker->Execute();
    }
    if (ok) {
        uint32 nOfTries = 20u;
        ok = !broker->Execute();
        while ((!ok) && (nOfTries > 0u)) {
            ok = !broker->Execute();
            nOfTries--;
        }

    }
    delete fakeMem;

    godb->Purge();
    return ok;
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestGetStackSize() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config10;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    if (ok) {
        dataSource = application->Find("Data.Drv1");
        ok = dataSource.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }

    ReferenceContainer brokers;
    char8 *fakeMem = new char8[1024];
    dataSource->GetOutputBrokers(brokers, "GAM1", fakeMem);
    ReferenceT<MemoryMapAsyncTriggerOutputBroker> broker = brokers.Get(0);
    if (ok) {
        ok = (broker->GetStackSize() == 32768);
    }
    delete fakeMem;

    godb->Purge();
    return ok;
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestResetPreTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint32 signalToGenerate[] = { 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5 };
    uint8 expectedTrigger[] = { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 };
    uint32 expectedSignal[] = { 9, 1, 2, 3, 8, 7, 6, 5, 4, 3, 2 };

    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    uint32 toGenerateNumberOfElements = sizeof(triggerToGenerate) / sizeof(uint8);
    uint32 expectedNumberOfElements = sizeof(expectedTrigger) / sizeof(uint8);
    uint32 preTriggerBuffers = 4;
    uint32 postTriggerBuffers = 2u;
    uint32 numberOfBuffers = 10u;
    uint32 sleepMSec = 10u;

    bool ok = parser.Parse();

    cdb.MoveAbsolute("$Test.+Functions.+GAM1");
    cdb.Delete("Trigger");
    cdb.Delete("Signal");
    Vector<uint8> triggerV(triggerToGenerate, toGenerateNumberOfElements);
    Vector<uint32> signalV(signalToGenerate, toGenerateNumberOfElements);
    cdb.Write("Trigger", triggerV);
    cdb.Write("Signal", signalV);

    Vector<uint8> expectedTriggerV(expectedTrigger, expectedNumberOfElements);
    Vector<uint32> expectedSignalV(expectedSignal, expectedNumberOfElements);
    cdb.MoveAbsolute("$Test.+Data.+Drv1");
    cdb.Delete("ExpectedTrigger");
    cdb.Delete("ExpectedSignal");
    cdb.Write("ExpectedTrigger", expectedTriggerV);
    cdb.Write("ExpectedSignal", expectedSignalV);
    cdb.Delete("PreTriggerBuffers");
    cdb.Delete("PostTriggerBuffers");
    cdb.Delete("NumberOfBuffers");
    cdb.Write("PreTriggerBuffers", preTriggerBuffers);
    cdb.Write("PostTriggerBuffers", postTriggerBuffers);
    cdb.Write("NumberOfBuffers", numberOfBuffers);
    cdb.MoveToRoot();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    if (ok) {
        dataSource = application->Find("Data.Drv1");
        ok = dataSource.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }

    uint32 i;
    for (i = 0; (i < gam->numberOfExecutes) && (ok); i++) {
        scheduler->ExecuteThreadCycle(0);
        Sleep::MSec(sleepMSec);
    }

    //2 seconds to finish
    uint64 maxTimeInCounts = HighResolutionTimer::Counter() + 2 * HighResolutionTimer::Frequency();
    //Force flushing of BufferLoop
    scheduler->ExecuteThreadCycle(0);
    while (dataSource->counter != dataSource->numberOfExecutes) {
        Sleep::Sec(0.1);
        if (HighResolutionTimer::Counter() > maxTimeInCounts) {
            ok = false;
            break;
        }
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        ok = dataSource->memoryOK;
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }
    if (ok) {
        gam->counter = 0;
    }
    if (ok) {
        dataSource->Reset();
    }
    for (i = 0; (i < gam->numberOfExecutes) && (ok); i++) {
        scheduler->ExecuteThreadCycle(0);
        Sleep::MSec(sleepMSec);
    }

    //2 seconds to finish
    maxTimeInCounts = HighResolutionTimer::Counter() + 2 * HighResolutionTimer::Frequency();
    //Force flushing of BufferLoop
    scheduler->ExecuteThreadCycle(0);
    while (dataSource->counter != dataSource->numberOfExecutes) {
        Sleep::Sec(0.1);
        if (HighResolutionTimer::Counter() > maxTimeInCounts) {
            ok = false;
            break;
        }
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        ok = dataSource->memoryOK;
    }

    godb->Purge();
    return ok;
}

bool MemoryMapAsyncTriggerOutputBrokerTest::TestFlushAllTriggers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 };
    uint32 signalToGenerate[] = { 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 2, 1, 0, 1, 1, 2, 3, 4, 5 };
    uint8 expectedTrigger[] = { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
    uint32 expectedSignal[] = { 9, 1, 2, 3, 8, 7, 6, 5, 4, 3, 2, 2, 1, 0, 1, 1, 2, 3, 4, 5};

    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    uint32 toGenerateNumberOfElements = sizeof(triggerToGenerate) / sizeof(uint8);
    uint32 expectedNumberOfElements = sizeof(expectedTrigger) / sizeof(uint8);
    uint32 preTriggerBuffers = 4;
    uint32 postTriggerBuffers = 2u;
    uint32 numberOfBuffers = 100u;
    uint32 sleepMSec = 10u;

    bool ok = parser.Parse();

    cdb.MoveAbsolute("$Test.+Functions.+GAM1");
    cdb.Delete("Trigger");
    cdb.Delete("Signal");
    Vector<uint8> triggerV(triggerToGenerate, toGenerateNumberOfElements);
    Vector<uint32> signalV(signalToGenerate, toGenerateNumberOfElements);
    cdb.Write("Trigger", triggerV);
    cdb.Write("Signal", signalV);

    Vector<uint8> expectedTriggerV(expectedTrigger, expectedNumberOfElements);
    Vector<uint32> expectedSignalV(expectedSignal, expectedNumberOfElements);
    cdb.MoveAbsolute("$Test.+Data.+Drv1");
    cdb.Delete("ExpectedTrigger");
    cdb.Delete("ExpectedSignal");
    cdb.Write("ExpectedTrigger", expectedTriggerV);
    cdb.Write("ExpectedSignal", expectedSignalV);
    cdb.Delete("PreTriggerBuffers");
    cdb.Delete("PostTriggerBuffers");
    cdb.Delete("NumberOfBuffers");
    cdb.Write("PreTriggerBuffers", preTriggerBuffers);
    cdb.Write("PostTriggerBuffers", postTriggerBuffers);
    cdb.Write("NumberOfBuffers", numberOfBuffers);
    cdb.MoveToRoot();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    if (ok) {
        dataSource = application->Find("Data.Drv1");
        ok = dataSource.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }

    uint32 i;
    for (i = 0; (i < gam->numberOfExecutes) && (ok); i++) {
        scheduler->ExecuteThreadCycle(0);
        Sleep::MSec(sleepMSec);
    }

    //Do not wait to finish
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        ok = dataSource->broker->FlushAllTriggers();
    }
    if (ok) {
        ok = dataSource->memoryOK;
    }

    godb->Purge();
    return ok;
}
