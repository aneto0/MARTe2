/**
 * @file MemoryMapTriggerOutputBrokerTest.cpp
 * @brief Source file for class MemoryMapTriggerOutputBrokerTest
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
 * the class MemoryMapTriggerOutputBrokerTest (public, protected, and private). Be aware that some 
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
#include "MemoryMapTriggerOutputBroker.h"
#include "MemoryMapTriggerOutputBrokerTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAM which generates a given trigger and signal pattern which is then confirmed by the DataSourceI
 */
class MemoryMapTriggerOutputBrokerGAMTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()MemoryMapTriggerOutputBrokerGAMTestHelper() {
        counter = 0;
        numberOfExecutes = 0;
        signalToGenerate = NULL;
        triggerToGenerate = NULL;
        elements = NULL;
    }

    virtual ~MemoryMapTriggerOutputBrokerGAMTestHelper() {
        using namespace MARTe;
        if (signalToGenerate != NULL) {
            delete [] signalToGenerate;
        }
        if (triggerToGenerate != NULL) {
            delete [] triggerToGenerate;
        }
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        using namespace MARTe;
        bool ok = GAM::Initialise(data);
        AnyType triggerAT = data.GetType("Trigger");
        AnyType signalAT = data.GetType("Signal");
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
        elements = new uint32[GetNumberOfOutputSignals() - 1];
        uint32 n;
        for (n=0; n<GetNumberOfOutputSignals() - 1; n++) {
            uint32 el;
            GetSignalNumberOfElements(OutputSignals, n+1, el);
            elements[n] = el;
        }
        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        uint8 *triggerOut = reinterpret_cast<uint8*>(GetOutputSignalMemory(0));
        if (counter < numberOfExecutes) {
            *triggerOut = triggerToGenerate[counter];
            uint32 n;
            for (n=0; n<GetNumberOfOutputSignals() - 1; n++) {
                uint32 *signalOut = reinterpret_cast<uint32*>(GetOutputSignalMemory(n+1));
                uint32 e;
                for (e=0; e<elements[n]; e++) {
                    signalOut[e] = signalToGenerate[counter];
                }
            }

        }
        counter++;
        return true;
    }

    MARTe::uint8 *triggerToGenerate;
    MARTe::uint32 *signalToGenerate;
    MARTe::uint32 *elements;
    MARTe::uint32 counter;
    MARTe::uint32 numberOfExecutes;
};
CLASS_REGISTER(MemoryMapTriggerOutputBrokerGAMTestHelper, "1.0")
/**
 * @brief DataSourceI implementation which returns a MemoryMapBrokerTestHelper broker.
 */
class MemoryMapTriggerOutputBrokerDataSourceTestHelper: public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapTriggerOutputBrokerDataSourceTestHelper    () {
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
    }

    virtual ~MemoryMapTriggerOutputBrokerDataSourceTestHelper() {
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

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        using namespace MARTe;
        data.Read("NumberOfBuffers", numberOfBuffers);
        data.Read("PreTriggerBuffers", preTriggerBuffers);
        data.Read("PostTriggerBuffers", postTriggerBuffers);
        data.Read("CPUMask", cpuMask);
        AnyType triggerAT = data.GetType("ExpectedTrigger");
        AnyType signalAT = data.GetType("ExpectedSignal");
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
        return numberOfBuffers;
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
        return "MemoryMapTriggerOutputBroker";
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
        ReferenceT<MARTe::MemoryMapTriggerOutputBroker> broker = ReferenceT<MARTe::MemoryMapTriggerOutputBroker>("MemoryMapTriggerOutputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            broker->SetCPUMask(cpuMask);
            broker->SetPreTriggerBuffers(preTriggerBuffers);
            broker->SetPostTriggerBuffers(postTriggerBuffers);
        }
        if (ret) {
            ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
        }
        if (ret) {
            ret = outputBrokers.Insert(broker);
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

    MARTe::uint32 numberOfBuffers;
    MARTe::uint32 preTriggerBuffers;
    MARTe::uint32 postTriggerBuffers;
    MARTe::uint32 *offsets;
    MARTe::uint32 cpuMask;
    MARTe::uint8 *expectedTrigger;
    MARTe::uint32 *expectedSignal;
    MARTe::uint32 numberOfExecutes;
    MARTe::uint32 totalNumberOfSignalElements;
    MARTe::uint32 counter;
    bool memoryOK;
    void *signalMemory;
};
CLASS_REGISTER(MemoryMapTriggerOutputBrokerDataSourceTestHelper, "1.0")

/**
 * @brief Manual scheduler to test the correct interface between the Broker and the DataSourceI
 */
class MemoryMapTriggerOutputBrokerSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

MemoryMapTriggerOutputBrokerSchedulerTestHelper    () : MARTe::GAMSchedulerI() {
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
        ExecuteSingleCycle(scheduledStates[RealTimeApplication::GetIndex()]->threads[threadId].executables,
                scheduledStates[RealTimeApplication::GetIndex()]->threads[threadId].numberOfExecutables);
    }

    virtual bool ConfigureScheduler() {

        bool ret = GAMSchedulerI::ConfigureScheduler();
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

CLASS_REGISTER(MemoryMapTriggerOutputBrokerSchedulerTestHelper, "1.0")

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

static bool TestExecute_PreTriggerBuffers_PostTriggerBuffers(const MARTe::char8 * const config, MARTe::uint8* triggerToGenerate,
                                                             MARTe::uint32 *signalToGenerate, MARTe::uint32 toGenerateNumberOfElements,
                                                             MARTe::uint8* expectedTrigger, MARTe::uint32* expectedSignal,
                                                             MARTe::uint32 expectedNumberOfElements, MARTe::uint32 preTriggerBuffers,
                                                             MARTe::uint32 postTriggerBuffers, MARTe::uint32 numberOfBuffers) {
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
    ReferenceT<MemoryMapTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapTriggerOutputBrokerDataSourceTestHelper> dataSource;
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
        Sleep::MSec(10);
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
static const MARTe::char8 * const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapTriggerOutputBrokerGAMTestHelper"
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
        "            Class = MemoryMapTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
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
        "        Class = MemoryMapTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//1 PreTrigger and 1 PostTrigger
static const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapTriggerOutputBrokerGAMTestHelper"
        "            TriggerAfterNCycles = 2"
        "            TriggerForNCycles = 1"
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
        "            Class = MemoryMapTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 1"
        "            PostTriggerBuffers = 1"
        "            CPUMask = 15"
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
        "        Class = MemoryMapTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//1 PostTrigger
static const MARTe::char8 * const config3 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapTriggerOutputBrokerGAMTestHelper"
        "            TriggerAfterNCycles = 2"
        "            TriggerForNCycles = 1"
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
        "            Class = MemoryMapTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 0"
        "            PostTriggerBuffers = 1"
        "            CPUMask = 15"
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
        "        Class = MemoryMapTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//0 PreTrigger 0 PostTrigger
static const MARTe::char8 * const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapTriggerOutputBrokerGAMTestHelper"
        "            TriggerAfterNCycles = 2"
        "            TriggerForNCycles = 1"
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
        "            Class = MemoryMapTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 0"
        "            PostTriggerBuffers = 0"
        "            CPUMask = 15"
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
        "        Class = MemoryMapTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//0 PreTrigger 0 PostTrigger always triggering
static const MARTe::char8 * const config5 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapTriggerOutputBrokerGAMTestHelper"
        "            TriggerAfterNCycles = 0"
        "            TriggerForNCycles = 20"
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
        "            Class = MemoryMapTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 0"
        "            PostTriggerBuffers = 0"
        "            CPUMask = 15"
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
        "        Class = MemoryMapTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Many PreTrigger and many PostTrigger
static const MARTe::char8 * const config6 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapTriggerOutputBrokerGAMTestHelper"
        "            TriggerAfterNCycles = 0"
        "            TriggerForNCycles = 20"
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
        "            Class = MemoryMapTriggerOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            PreTriggerBuffers = 4"
        "            PostTriggerBuffers = 4"
        "            CPUMask = 15"
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
        "        Class = MemoryMapTriggerOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool MemoryMapTriggerOutputBrokerTest::TestConstructor() {
    using namespace MARTe;
    MemoryMapTriggerOutputBroker broker;
    return true;
}

bool MemoryMapTriggerOutputBrokerTest::TestInit() {
    using namespace MARTe;
    return TestIntegratedInApplication(config1, true);
}

bool MemoryMapTriggerOutputBrokerTest::TestExecute_1_PreTriggerBuffers_0_PostTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };
    uint8 expectedTrigger[] = { 0, 1, 0, 1, 0, 1, 0, 1, 1 };
    uint32 expectedSignal[] = { 1, 2, 4, 5, 8, 9, 8, 7, 6 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8),
                                                            expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 1, 0, 10);
}

bool MemoryMapTriggerOutputBrokerTest::TestExecute_1_PreTriggerBuffers_1_PostTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };
    uint8 expectedTrigger[] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0 };
    uint32 expectedSignal[] = { 1, 2, 3, 4, 5, 6, 8, 9, 8, 7, 6, 5 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8),
                                                            expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 1, 1, 10);
}

bool MemoryMapTriggerOutputBrokerTest::TestExecute_0_PreTriggerBuffers_1_PostTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };
    uint8 expectedTrigger[] = { 1, 0, 1, 0, 1, 0, 1, 1, 0 };
    uint32 expectedSignal[] = { 2, 3, 5, 6, 9, 8, 7, 6, 5 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8),
                                                            expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 0, 1, 10);
}

bool MemoryMapTriggerOutputBrokerTest::TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };
    uint8 expectedTrigger[] = { 1, 1, 1, 1, 1 };
    uint32 expectedSignal[] = { 2, 5, 9, 7, 6 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8),
                                                            expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 0, 0, 10);
}

bool MemoryMapTriggerOutputBrokerTest::TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers_AlwaysTriggering() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };
    uint8 expectedTrigger[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    uint32 expectedSignal[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8),
                                                            expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 0, 0, 10);
}

bool MemoryMapTriggerOutputBrokerTest::TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_AlwaysTriggering() {
    using namespace MARTe;
    uint8 triggerToGenerate[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    uint32 signalToGenerate[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5 };
    uint8 expectedTrigger[] = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    uint32 expectedSignal[] = { 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, triggerToGenerate, signalToGenerate, sizeof(triggerToGenerate) / sizeof(uint8),
                                                            expectedTrigger, expectedSignal, sizeof(expectedTrigger) / sizeof(uint8), 4, 4, 10);
}

