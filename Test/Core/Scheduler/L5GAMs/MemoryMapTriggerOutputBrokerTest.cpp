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
 * @brief GAM which generates signals and triggers after a given number of cycles
 */
class MemoryMapTriggerOutputBrokerGAMTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()MemoryMapTriggerOutputBrokerGAMTestHelper() {
        triggeredCycles = 0;
        triggerForNCycles = 0;
        triggerAfterNCycles = 0;
        cycles = 0;
        counter = 0;
        signals = NULL;
        elements = NULL;
        trigger = NULL;
    }

    virtual ~MemoryMapTriggerOutputBrokerGAMTestHelper() {
        using namespace MARTe;
        if (signals != NULL) {
            delete [] signals;
        }
        if (elements != NULL) {
            delete [] elements;
        }
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        bool ok = GAM::Initialise(data);
        data.Read("TriggerAfterNCycles", triggerAfterNCycles);
        data.Read("TriggerForNCycles", triggerForNCycles);
        return ok;
    }

    virtual bool Setup() {
        using namespace MARTe;
        signals = new uint32*[GetNumberOfOutputSignals() - 1];
        elements = new uint32[GetNumberOfOutputSignals() - 1];
        trigger = reinterpret_cast<uint8 *>(GetOutputSignalMemory(0u));
        uint32 i;
        for (i=1; i<GetNumberOfOutputSignals(); i++) {
            uint32 el;
            GetSignalNumberOfElements(OutputSignals, i, el);
            elements[i-1] = el;
            signals[i-1] = reinterpret_cast<uint32 *>(GetOutputSignalMemory(i));
        }
        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;

        *trigger = 0;
        if (cycles >= triggerAfterNCycles) {
            if (cycles < (triggerAfterNCycles + triggerForNCycles)) {
                *trigger = 1;
            }
        }

        uint32 i;
        for (i=1; i<GetNumberOfOutputSignals(); i++) {
            uint32 e;
            for (e=0; e<elements[i-1]; e++) {
                signals[i-1][e] = counter;
            }
        }
        cycles++;
        counter++;
        return true;
    }

    MARTe::uint32 triggerAfterNCycles;
    MARTe::uint32 triggerForNCycles;
    MARTe::uint32 triggeredCycles;
    MARTe::uint32 cycles;
    MARTe::uint32 counter;
    MARTe::uint32 **signals;
    MARTe::uint32 *elements;
    MARTe::uint8 *trigger;
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
        signalMemory = NULL;
        cpuMask = 0xff;
        semWrite.Create();
        semRead.Create();
        terminated = false;
    }

    virtual ~MemoryMapTriggerOutputBrokerDataSourceTestHelper() {
        using namespace MARTe;
        if (signalMemory != NULL_PTR(void *)) {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemory);
        }
        if (offsets != NULL_PTR(uint32 *)) {
            delete[] offsets;
        }
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        data.Read("NumberOfBuffers", numberOfBuffers);
        data.Read("PreTriggerBuffers", preTriggerBuffers);
        data.Read("PostTriggerBuffers", postTriggerBuffers);
        data.Read("CPUMask", cpuMask);
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

    virtual bool Synchronise() {
        using namespace MARTe;
        if (!terminated) {
            semRead.Reset();
            semWrite.Post();
            semRead.Wait();
        }
        return !terminated;
    }

    void Terminate () {
        terminated = true;
        semWrite.Post();
        semRead.Post();
        semWrite.Close();
        semRead.Close();
    }

    MARTe::uint32 numberOfBuffers;
    MARTe::uint32 preTriggerBuffers;
    MARTe::uint32 postTriggerBuffers;
    MARTe::uint32 *offsets;
    MARTe::uint32 cpuMask;
    void *signalMemory;
    MARTe::EventSem semWrite;
    MARTe::EventSem semRead;
    bool terminated;
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

static bool TestExecute_PreTriggerBuffers_PostTriggerBuffers(const MARTe::char8 * const config, MARTe::uint32 triggerAfterNCycles,
                                                             MARTe::uint32 triggerForNCycles, MARTe::uint32 preTriggerBuffers, MARTe::uint32 postTriggerBuffers,
                                                             MARTe::uint32 cyclesBetweenTests, MARTe::uint32 numberOfTests) {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config, false);

    ReferenceT<MemoryMapTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<RealTimeApplication> application;
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
    uint8 *trigger = NULL;
    uint32 *signal1 = NULL;
    uint32 *signal2 = NULL;
    uint32 *signal3 = NULL;
    if (ok) {
        dataSource->GetSignalMemoryBuffer(0, 0, (void *&) trigger);
        dataSource->GetSignalMemoryBuffer(1, 0, (void *&) signal1);
        dataSource->GetSignalMemoryBuffer(2, 0, (void *&) signal2);
        dataSource->GetSignalMemoryBuffer(3, 0, (void *&) signal3);
    }
    if (ok) {
        uint32 n;
        uint32 cycles = 0;
        uint32 lastTrigger = 0;
        for (n = 0; n < numberOfTests; n++) {
            uint32 i;
            //There is a mistake here. TODO
            if (gam->counter < preTriggerBuffers) {
                cycles = static_cast<uint32>(0);
            }
            else {
                cycles = gam->counter - preTriggerBuffers;
            }
            //Reset the counter in the gam
            gam->cycles = 0;
            for (i = 0; i < triggerAfterNCycles; i++) {
                scheduler->ExecuteThreadCycle(0);
                ok &= (*trigger == lastTrigger);
                ok &= (*signal1 == cycles);
                ok &= (*signal2 == cycles);
                ok &= (*signal3 == cycles);
            }

            for (i = 0; i < preTriggerBuffers; i++) {
                dataSource->semWrite.Reset();
                //Wait for the Synchronise to be called on the DataSourceI
                scheduler->ExecuteThreadCycle(0);
                dataSource->semWrite.Wait();
                ok &= (*trigger == 0);
                lastTrigger = 0;
                ok &= (*signal1 == cycles);
                ok &= (*signal2 == cycles);
                ok &= (*signal3 == cycles);
                //Warn the DataSourceI that we have already checked the data
                dataSource->semRead.Post();
            }
            for (i = 0; i < triggerForNCycles; i++) {
                dataSource->semWrite.Reset();
                //Wait for the Synchronise to be called on the DataSourceI
                scheduler->ExecuteThreadCycle(0);
                dataSource->semWrite.Wait();
                ok &= (*trigger == 1);
                lastTrigger = 1;
                ok &= (*signal1 == cycles);
                ok &= (*signal2 == cycles);
                ok &= (*signal3 == cycles);
                cycles++;
                //Warn the DataSourceI that we have already checked the data
                dataSource->semRead.Post();
            }
            for (i = 0; i < postTriggerBuffers; i++) {
                dataSource->semWrite.Reset();
                //Wait for the Synchronise to be called on the DataSourceI
                scheduler->ExecuteThreadCycle(0);
                dataSource->semWrite.Wait();
                ok &= (*trigger == 0);
                lastTrigger = 0;
                ok &= (*signal1 == cycles);
                ok &= (*signal2 == cycles);
                ok &= (*signal3 == cycles);
                cycles++;
                //Warn the DataSourceI that we have already checked the data
                dataSource->semRead.Post();
            }
            for (i = 0; i < cyclesBetweenTests; i++) {
                scheduler->ExecuteThreadCycle(0);
            }
        }
    }

    //Post to kill
    dataSource->semRead.Post();

    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        dataSource->Terminate();
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
    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config1, 2, 1, 1, 0, 1, 3);
#if 0
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config1, false);

    ReferenceT<MemoryMapTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<RealTimeApplication> application;
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
    uint8 *trigger = NULL;
    uint32 *signal1 = NULL;
    uint32 *signal2 = NULL;
    uint32 *signal3 = NULL;
    if (ok) {
        dataSource->GetSignalMemoryBuffer(0, 0, (void *&) trigger);
        dataSource->GetSignalMemoryBuffer(1, 0, (void *&) signal1);
        dataSource->GetSignalMemoryBuffer(2, 0, (void *&) signal2);
        dataSource->GetSignalMemoryBuffer(3, 0, (void *&) signal3);
    }
    if (ok) {
        scheduler->ExecuteThreadCycle(0); //cycles = 0
        scheduler->ExecuteThreadCycle(0);//cycles = 1
        ok &= (*trigger == 0);
        ok &= (*signal1 == 0);
        ok &= (*signal2 == 0);
        ok &= (*signal3 == 0);

        dataSource->semWrite.Reset();
        scheduler->ExecuteThreadCycle(0);//cycles = 2
        //Wait for the Synchronise to be called on the DataSourceI
        dataSource->semWrite.Wait();
        ok &= (*trigger == 0);
        ok &= (*signal1 == 1);//Prebuffer => signal1 = 1
        ok &= (*signal2 == 1);
        ok &= (*signal3 == 1);
        dataSource->semWrite.Reset();
        //Warn the DataSourceI that we have already checked the data
        dataSource->semRead.Post();
        //Trigger the update of the next buffer (since the Broker is always PreTriggerBufffers behind the writer we have to cycle the RTThread)
        scheduler->ExecuteThreadCycle(0);//cycles = 3
        dataSource->semWrite.Wait();
        ok &= (*trigger == 1);
        ok &= (*signal1 == 2);
        ok &= (*signal2 == 2);
        ok &= (*signal3 == 2);
        dataSource->semRead.Post();

        //No PostBuffer configured
        scheduler->ExecuteThreadCycle(0);//cycles = 4
        ok &= (*trigger == 1);
        ok &= (*signal1 == 2);
        ok &= (*signal2 == 2);
        ok &= (*signal3 == 2);

        //Reset the counter in the gam
        gam->cycles = 0;
        scheduler->ExecuteThreadCycle(0);//cycles = 5
        scheduler->ExecuteThreadCycle(0);//cycles = 6
        dataSource->semWrite.Reset();
        scheduler->ExecuteThreadCycle(0);//cycles = 7
        dataSource->semWrite.Wait();
        ok &= (*trigger == 0);
        ok &= (*signal1 == 6);
        ok &= (*signal2 == 6);
        ok &= (*signal3 == 6);
        dataSource->semWrite.Reset();
        dataSource->semRead.Post();
        scheduler->ExecuteThreadCycle(0);//cycles = 8
        dataSource->semWrite.Wait();
        ok &= (*trigger == 1);
        ok &= (*signal1 == 7);
        ok &= (*signal2 == 7);
        ok &= (*signal3 == 7);
        dataSource->semRead.Post();

        //No PostBuffer configured
        scheduler->ExecuteThreadCycle(0);//cycles = 9
        ok &= (*trigger == 1);
        ok &= (*signal1 == 7);
        ok &= (*signal2 == 7);
        ok &= (*signal3 == 7);

        //Reset the counter in the gam
        gam->cycles = 0;
        scheduler->ExecuteThreadCycle(0);//cycles = 10
        scheduler->ExecuteThreadCycle(0);//cycles = 11
        dataSource->semWrite.Reset();
        scheduler->ExecuteThreadCycle(0);//cycles = 12
        dataSource->semWrite.Wait();
        ok &= (*trigger == 0);
        ok &= (*signal1 == 11);
        ok &= (*signal2 == 11);
        ok &= (*signal3 == 11);
        dataSource->semWrite.Reset();
        dataSource->semRead.Post();
        scheduler->ExecuteThreadCycle(0);
        dataSource->semWrite.Wait();
        ok &= (*trigger == 1);
        ok &= (*signal1 == 12);
        ok &= (*signal2 == 12);
        ok &= (*signal3 == 12);
        dataSource->semRead.Post();
        //No PostBuffer configured
        scheduler->ExecuteThreadCycle(0);
        ok &= (*trigger == 1);
        ok &= (*signal1 == 12);
        ok &= (*signal2 == 12);
        ok &= (*signal3 == 12);

        //Post to kill
        dataSource->semRead.Post();
    }

    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        dataSource->Terminate();
    }

    godb->Purge();
    return ok;
#endif
}

bool MemoryMapTriggerOutputBrokerTest::TestExecute_1_PreTriggerBuffers_1_PostTriggerBuffers() {
    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config2, 2, 1, 1, 1, 2, 3);
#if 0
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config2, false);

    ReferenceT<MemoryMapTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<RealTimeApplication> application;
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
    uint8 *trigger = NULL;
    uint32 *signal1 = NULL;
    uint32 *signal2 = NULL;
    uint32 *signal3 = NULL;
    if (ok) {
        dataSource->GetSignalMemoryBuffer(0, 0, (void *&) trigger);
        dataSource->GetSignalMemoryBuffer(1, 0, (void *&) signal1);
        dataSource->GetSignalMemoryBuffer(2, 0, (void *&) signal2);
        dataSource->GetSignalMemoryBuffer(3, 0, (void *&) signal3);
    }
    if (ok) {
        scheduler->ExecuteThreadCycle(0); //cycles = 0
        scheduler->ExecuteThreadCycle(0);//cycles = 1
        ok &= (*trigger == 0);
        ok &= (*signal1 == 0);
        ok &= (*signal2 == 0);
        ok &= (*signal3 == 0);

        dataSource->semWrite.Reset();
        //Wait for the Synchronise to be called on the DataSourceI
        scheduler->ExecuteThreadCycle(0);//cycles = 2
        dataSource->semWrite.Wait();
        ok &= (*trigger == 0);
        ok &= (*signal1 == 1);
        ok &= (*signal2 == 1);
        ok &= (*signal3 == 1);

        dataSource->semWrite.Reset();
        //Warn the DataSourceI that we have already checked the data
        dataSource->semRead.Post();
        scheduler->ExecuteThreadCycle(0);//cycles = 3
        dataSource->semWrite.Wait();
        ok &= (*trigger == 1);
        ok &= (*signal1 == 2);
        ok &= (*signal2 == 2);
        ok &= (*signal3 == 2);

        dataSource->semWrite.Reset();
        dataSource->semRead.Post();
        scheduler->ExecuteThreadCycle(0);//Cycles = 4
        dataSource->semWrite.Wait();
        //1 PostBuffer configured
        ok &= (*trigger == 0);
        ok &= (*signal1 == 3);
        ok &= (*signal2 == 3);
        ok &= (*signal3 == 3);
        dataSource->semRead.Post();
        //Only 1 PostBuffer configured
        scheduler->ExecuteThreadCycle(0);//Cycles = 5
        ok &= (*trigger == 0);
        ok &= (*signal1 == 3);
        ok &= (*signal2 == 3);
        ok &= (*signal3 == 3);

        //Reset the counter in the gam
        gam->cycles = 0;
        scheduler->ExecuteThreadCycle(0);//Cycles = 6
        scheduler->ExecuteThreadCycle(0);//Cycles = 7
        dataSource->semWrite.Reset();
        scheduler->ExecuteThreadCycle(0);//Cycles = 8
        dataSource->semWrite.Wait();
        ok &= (*trigger == 0);
        ok &= (*signal1 == 7);
        ok &= (*signal2 == 7);
        ok &= (*signal3 == 7);
        dataSource->semWrite.Reset();
        dataSource->semRead.Post();
        scheduler->ExecuteThreadCycle(0);//Cycles = 9
        dataSource->semWrite.Wait();
        ok &= (*trigger == 1);
        ok &= (*signal1 == 8);
        ok &= (*signal2 == 8);
        ok &= (*signal3 == 8);
        dataSource->semRead.Post();
        dataSource->semWrite.Reset();
        scheduler->ExecuteThreadCycle(0);//Cycles = 10
        dataSource->semWrite.Wait();
        ok &= (*trigger == 0);
        ok &= (*signal1 == 9);
        ok &= (*signal2 == 9);
        ok &= (*signal3 == 9);
        dataSource->semRead.Post();
        scheduler->ExecuteThreadCycle(0);//Cycles = 11
        ok &= (*trigger == 0);
        ok &= (*signal1 == 9);
        ok &= (*signal2 == 9);
        ok &= (*signal3 == 9);

        //Reset the counter in the gam
        gam->cycles = 0;
        scheduler->ExecuteThreadCycle(0);//Cycles = 12
        scheduler->ExecuteThreadCycle(0);//Cycles = 13
        dataSource->semWrite.Reset();
        scheduler->ExecuteThreadCycle(0);//Cycles = 14
        dataSource->semWrite.Wait();
        ok &= (*trigger == 0);
        ok &= (*signal1 == 13);
        ok &= (*signal2 == 13);
        ok &= (*signal3 == 13);
        dataSource->semWrite.Reset();
        dataSource->semRead.Post();
        scheduler->ExecuteThreadCycle(0);//Cycles = 15
        dataSource->semWrite.Wait();
        ok &= (*trigger == 1);
        ok &= (*signal1 == 14);
        ok &= (*signal2 == 14);
        ok &= (*signal3 == 14);
        dataSource->semRead.Post();
        dataSource->semWrite.Reset();
        scheduler->ExecuteThreadCycle(0);//Cycles = 16
        dataSource->semWrite.Wait();
        //PostBuffer configured
        ok &= (*trigger == 0);
        ok &= (*signal1 == 15);
        ok &= (*signal2 == 15);
        ok &= (*signal3 == 15);
        dataSource->semRead.Post();
        //Only one post-buffer
        scheduler->ExecuteThreadCycle(0);//Cycles = 17
        ok &= (*trigger == 0);
        ok &= (*signal1 == 15);
        ok &= (*signal2 == 15);
        ok &= (*signal3 == 15);

        //Post to kill
        dataSource->semRead.Post();
    }

    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        dataSource->Terminate();
    }

    godb->Purge();
    return ok;
#endif
}

bool MemoryMapTriggerOutputBrokerTest::TestExecute_0_PreTriggerBuffers_1_PostTriggerBuffers() {
    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config3, 2, 1, 0, 1, 2, 3);
    /*using namespace MARTe;
     bool ok = TestIntegratedInApplication(config3, false);

     ReferenceT<MemoryMapTriggerOutputBrokerSchedulerTestHelper> scheduler;
     ReferenceT<MemoryMapTriggerOutputBrokerGAMTestHelper> gam;
     ReferenceT<MemoryMapTriggerOutputBrokerDataSourceTestHelper> dataSource;
     ReferenceT<RealTimeApplication> application;
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
     uint8 *trigger = NULL;
     uint32 *signal1 = NULL;
     uint32 *signal2 = NULL;
     uint32 *signal3 = NULL;
     if (ok) {
     dataSource->GetSignalMemoryBuffer(0, 0, (void *&) trigger);
     dataSource->GetSignalMemoryBuffer(1, 0, (void *&) signal1);
     dataSource->GetSignalMemoryBuffer(2, 0, (void *&) signal2);
     dataSource->GetSignalMemoryBuffer(3, 0, (void *&) signal3);
     }
     if (ok) {
     scheduler->ExecuteThreadCycle(0); //cycles = 0
     scheduler->ExecuteThreadCycle(0); //cycles = 1
     ok &= (*trigger == 0);
     ok &= (*signal1 == 0);
     ok &= (*signal2 == 0);
     ok &= (*signal3 == 0);

     dataSource->semWrite.Reset();
     //Wait for the Synchronise to be called on the DataSourceI
     scheduler->ExecuteThreadCycle(0); //cycles = 2
     dataSource->semWrite.Wait();
     ok &= (*trigger == 1);
     ok &= (*signal1 == 2);
     ok &= (*signal2 == 2);
     ok &= (*signal3 == 2);
     dataSource->semWrite.Reset();
     //Warn the DataSourceI that we have already checked the data
     dataSource->semRead.Post();
     scheduler->ExecuteThreadCycle(0); //cycles = 3
     dataSource->semWrite.Wait();
     //1 PostBuffer configured
     ok &= (*trigger == 0);
     ok &= (*signal1 == 3);
     ok &= (*signal2 == 3);
     ok &= (*signal3 == 3);
     dataSource->semRead.Post();
     //Only 1 PostBuffer configured
     scheduler->ExecuteThreadCycle(0); //Cycles = 4
     ok &= (*trigger == 0);
     ok &= (*signal1 == 3);
     ok &= (*signal2 == 3);
     ok &= (*signal3 == 3);

     //Reset the counter in the gam
     gam->cycles = 0;
     scheduler->ExecuteThreadCycle(0); //Cycles = 5
     scheduler->ExecuteThreadCycle(0); //Cycles = 6
     dataSource->semWrite.Reset();
     scheduler->ExecuteThreadCycle(0); //Cycles = 7
     dataSource->semWrite.Wait();
     ok &= (*trigger == 1);
     ok &= (*signal1 == 7);
     ok &= (*signal2 == 7);
     ok &= (*signal3 == 7);
     dataSource->semWrite.Reset();
     dataSource->semRead.Post();
     scheduler->ExecuteThreadCycle(0); //Cycles = 8
     dataSource->semWrite.Wait();
     ok &= (*trigger == 0);
     ok &= (*signal1 == 8);
     ok &= (*signal2 == 8);
     ok &= (*signal3 == 8);
     dataSource->semRead.Post();
     scheduler->ExecuteThreadCycle(0); //Cycles = 9
     ok &= (*trigger == 0);
     ok &= (*signal1 == 8);
     ok &= (*signal2 == 8);
     ok &= (*signal3 == 8);

     //Reset the counter in the gam
     gam->cycles = 0;
     scheduler->ExecuteThreadCycle(0); //Cycles = 10
     scheduler->ExecuteThreadCycle(0); //Cycles = 11
     dataSource->semWrite.Reset();
     scheduler->ExecuteThreadCycle(0); //Cycles = 12
     dataSource->semWrite.Wait();
     ok &= (*trigger == 1);
     ok &= (*signal1 == 12);
     ok &= (*signal2 == 12);
     ok &= (*signal3 == 12);
     dataSource->semWrite.Reset();
     dataSource->semRead.Post();
     scheduler->ExecuteThreadCycle(0); //Cycles = 13
     dataSource->semWrite.Wait();
     ok &= (*trigger == 0);
     ok &= (*signal1 == 13);
     ok &= (*signal2 == 13);
     ok &= (*signal3 == 13);
     dataSource->semRead.Post();
     scheduler->ExecuteThreadCycle(0); //Cycles = 14
     ok &= (*trigger == 0);
     ok &= (*signal1 == 13);
     ok &= (*signal2 == 13);
     ok &= (*signal3 == 13);

     //Post to kill
     dataSource->semRead.Post();
     }

     if (ok) {
     ok = application->StopCurrentStateExecution();
     }
     if (ok) {
     dataSource->Terminate();
     }

     godb->Purge();
     return ok;*/
}

bool MemoryMapTriggerOutputBrokerTest::TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers() {
    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config4, 2, 1, 0, 0, 2, 3);

#if 0
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config4, false);

    ReferenceT<MemoryMapTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<RealTimeApplication> application;
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
    uint8 *trigger = NULL;
    uint32 *signal1 = NULL;
    uint32 *signal2 = NULL;
    uint32 *signal3 = NULL;
    if (ok) {
        dataSource->GetSignalMemoryBuffer(0, 0, (void *&) trigger);
        dataSource->GetSignalMemoryBuffer(1, 0, (void *&) signal1);
        dataSource->GetSignalMemoryBuffer(2, 0, (void *&) signal2);
        dataSource->GetSignalMemoryBuffer(3, 0, (void *&) signal3);
    }
    if (ok) {
        scheduler->ExecuteThreadCycle(0); //cycles = 0
        scheduler->ExecuteThreadCycle(0);//cycles = 1
        ok &= (*trigger == 0);
        ok &= (*signal1 == 0);
        ok &= (*signal2 == 0);
        ok &= (*signal3 == 0);

        dataSource->semWrite.Reset();
        //Wait for the Synchronise to be called on the DataSourceI
        scheduler->ExecuteThreadCycle(0);//cycles = 2
        dataSource->semWrite.Wait();
        ok &= (*trigger == 1);
        ok &= (*signal1 == 2);
        ok &= (*signal2 == 2);
        ok &= (*signal3 == 2);
        //Warn the DataSourceI that we have already checked the data
        dataSource->semRead.Post();
        scheduler->ExecuteThreadCycle(0);//cycles = 3
        //No PostBuffer configured
        ok &= (*trigger == 1);
        ok &= (*signal1 == 2);
        ok &= (*signal2 == 2);
        ok &= (*signal3 == 2);

        //Reset the counter in the gam
        gam->cycles = 0;
        scheduler->ExecuteThreadCycle(0);//Cycles = 4
        scheduler->ExecuteThreadCycle(0);//Cycles = 5
        dataSource->semWrite.Reset();
        scheduler->ExecuteThreadCycle(0);//Cycles = 6
        dataSource->semWrite.Wait();
        ok &= (*trigger == 1);
        ok &= (*signal1 == 6);
        ok &= (*signal2 == 6);
        ok &= (*signal3 == 6);
        dataSource->semRead.Post();
        scheduler->ExecuteThreadCycle(0);//Cycles = 7
        ok &= (*trigger == 1);
        ok &= (*signal1 == 6);
        ok &= (*signal2 == 6);
        ok &= (*signal3 == 6);

        //Reset the counter in the gam
        gam->cycles = 0;
        scheduler->ExecuteThreadCycle(0);//Cycles = 8
        scheduler->ExecuteThreadCycle(0);//Cycles = 9
        dataSource->semWrite.Reset();
        scheduler->ExecuteThreadCycle(0);//Cycles = 10
        dataSource->semWrite.Wait();
        ok &= (*trigger == 1);
        ok &= (*signal1 == 10);
        ok &= (*signal2 == 10);
        ok &= (*signal3 == 10);
        dataSource->semRead.Post();
        scheduler->ExecuteThreadCycle(0);//Cycles = 11
        ok &= (*trigger == 1);
        ok &= (*signal1 == 10);
        ok &= (*signal2 == 10);
        ok &= (*signal3 == 10);

        //Post to kill
        dataSource->semRead.Post();
    }

    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        dataSource->Terminate();
    }

    godb->Purge();
    return ok;
#endif
}

bool MemoryMapTriggerOutputBrokerTest::TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers_AlwaysTriggering() {
    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config5, 0, 20, 0, 0, 2, 3);
#if 0
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config4, false);

    ReferenceT<MemoryMapTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<RealTimeApplication> application;
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
    uint8 *trigger = NULL;
    uint32 *signal1 = NULL;
    uint32 *signal2 = NULL;
    uint32 *signal3 = NULL;
    if (ok) {
        dataSource->GetSignalMemoryBuffer(0, 0, (void *&) trigger);
        dataSource->GetSignalMemoryBuffer(1, 0, (void *&) signal1);
        dataSource->GetSignalMemoryBuffer(2, 0, (void *&) signal2);
        dataSource->GetSignalMemoryBuffer(3, 0, (void *&) signal3);
    }
    if (ok) {
        uint32 i;
        for (i = 0; i < 20; i++) {
            dataSource->semWrite.Reset();
            //Wait for the Synchronise to be called on the DataSourceI
            scheduler->ExecuteThreadCycle(0);//cycles = i
            dataSource->semWrite.Wait();
            ok &= (*trigger == 1);
            ok &= (*signal1 == i);
            ok &= (*signal2 == i);
            ok &= (*signal3 == i);
            //Warn the DataSourceI that we have already checked the data
            dataSource->semRead.Post();
        }

        //Post to kill
        dataSource->semRead.Post();
    }

    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        dataSource->Terminate();
    }

    godb->Purge();
    return ok;
#endif
}

bool MemoryMapTriggerOutputBrokerTest::TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_AlwaysTriggering() {
    return TestExecute_PreTriggerBuffers_PostTriggerBuffers(config6, 0, 20, 4, 4, 2, 3);
#if 0
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config5, false);

    ReferenceT<MemoryMapTriggerOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapTriggerOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapTriggerOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<RealTimeApplication> application;
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
    uint8 *trigger = NULL;
    uint32 *signal1 = NULL;
    uint32 *signal2 = NULL;
    uint32 *signal3 = NULL;
    if (ok) {
        dataSource->GetSignalMemoryBuffer(0, 0, (void *&) trigger);
        dataSource->GetSignalMemoryBuffer(1, 0, (void *&) signal1);
        dataSource->GetSignalMemoryBuffer(2, 0, (void *&) signal2);
        dataSource->GetSignalMemoryBuffer(3, 0, (void *&) signal3);
    }
    if (ok) {
        uint32 i;
        for (i = 0; i < 4; i++) {
            dataSource->semWrite.Reset();
            //Wait for the Synchronise to be called on the DataSourceI
            scheduler->ExecuteThreadCycle(0);//cycles = i
            dataSource->semWrite.Wait();
            ok &= (*trigger == 0);
            ok &= (*signal1 == 0);
            ok &= (*signal2 == 0);
            ok &= (*signal3 == 0);
            //Warn the DataSourceI that we have already checked the data
            dataSource->semRead.Post();
        }
        for (i = 0; i < 20; i++) {
            dataSource->semWrite.Reset();
            //Wait for the Synchronise to be called on the DataSourceI
            scheduler->ExecuteThreadCycle(0);//cycles = i
            dataSource->semWrite.Wait();
            ok &= (*trigger == 1);
            ok &= (*signal1 == i);
            ok &= (*signal2 == i);
            ok &= (*signal3 == i);
            //Warn the DataSourceI that we have already checked the data
            dataSource->semRead.Post();
        }

        //Post to kill
        dataSource->semRead.Post();
    }

    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        dataSource->Terminate();
    }

    godb->Purge();
    return ok;
#endif
}

