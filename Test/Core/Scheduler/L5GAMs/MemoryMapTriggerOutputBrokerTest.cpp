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

        if (cycles >= triggerAfterNCycles) {
            if (cycles < (triggerAfterNCycles + triggerForNCycles + 1)) {
                *trigger = 1;
            }
        }

        uint32 i;
        for (i=1; i<GetNumberOfOutputSignals(); i++) {
            uint32 e;
            for (e=0; e<elements[i-1]; e++) {
                signals[i-1][e] = cycles;
            }
        }
        cycles++;
        return true;
    }

    MARTe::uint32 triggerAfterNCycles;
    MARTe::uint32 triggerForNCycles;
    MARTe::uint32 triggeredCycles;
    MARTe::uint32 cycles;
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

bool MemoryMapTriggerOutputBrokerTest::TestExecute_1() {
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
        dataSource->semWrite.Reset();
        scheduler->ExecuteThreadCycle(0);
        ok &= (*trigger == 0);
        ok &= (*signal1 == 0);
        ok &= (*signal2 == 0);
        ok &= (*signal3 == 0);

        scheduler->ExecuteThreadCycle(0);
        ok &= (*trigger == 0);
        ok &= (*signal1 == 0);
        ok &= (*signal2 == 0);
        ok &= (*signal3 == 0);

        //Wait for the Synchronise to be called on the DataSourceI
        scheduler->ExecuteThreadCycle(0);
        dataSource->semWrite.Wait();
        ok &= (*trigger == 0);
        ok &= (*signal1 == 1);
        ok &= (*signal2 == 1);
        ok &= (*signal3 == 1);

        /*dataSource->semWrite.Reset();
         //Warn the DataSourceI that we have already checked the data
         dataSource->semRead.Post();
         dataSource->semWrite.Wait();
         ok &= (*trigger == 1);
         ok &= (*signal1 == 2);
         ok &= (*signal2 == 2);
         ok &= (*signal3 == 2);
         dataSource->semRead.Post();

         //No PostBuffer configured
         scheduler->ExecuteThreadCycle(0);
         ok &= (*trigger == 1);
         ok &= (*signal1 == 2);
         ok &= (*signal2 == 2);
         ok &= (*signal3 == 2);*/
        dataSource->semRead.Post();
    }
    if (!ok) {
        ok = true;
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        dataSource->Terminate();
    }

    godb->Purge();
    //Wait for the DataSourceThread to end.
    /*while (Threads::NumberOfThreads() > 0) {
     Sleep::Sec(0.1);
     }*/
    return ok;

}

