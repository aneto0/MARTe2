/**
 * @file MemoryMapAsyncOutputBrokerTest.cpp
 * @brief Source file for class MemoryMapAsyncOutputBrokerTest
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
 * the class MemoryMapAsyncOutputBrokerTest (public, protected, and private). Be aware that some
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
#include "MemoryMapAsyncOutputBroker.h"
#include "MemoryMapAsyncOutputBrokerTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAM which generates a given trigger and signal pattern which is then confirmed by the DataSourceI
 */
class MemoryMapAsyncOutputBrokerGAMTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()MemoryMapAsyncOutputBrokerGAMTestHelper() {
        counter = 0;
        numberOfExecutes = 0;
        signalToGenerate = NULL;
        elements = NULL;
    }

    virtual ~MemoryMapAsyncOutputBrokerGAMTestHelper() {
        using namespace MARTe;
        if (signalToGenerate != NULL) {
            delete [] signalToGenerate;
        }
        if (elements != NULL) {
            delete [] elements;
        }
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        using namespace MARTe;
        bool ok = GAM::Initialise(data);
        AnyType signalAT = data.GetType("Signal");
        numberOfExecutes = signalAT.GetNumberOfElements(0);
        signalToGenerate = new uint32[numberOfExecutes];
        Vector<uint32> signalV(signalToGenerate, numberOfExecutes);
        data.Read("Signal", signalV);
        return ok;
    }

    virtual bool Setup() {
        using namespace MARTe;
        elements = new uint32[GetNumberOfOutputSignals()];
        uint32 n;
        for (n=0; n<GetNumberOfOutputSignals(); n++) {
            uint32 el;
            GetSignalNumberOfElements(OutputSignals, n, el);
            elements[n] = el;
        }
        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        if (counter < numberOfExecutes) {
            uint32 n;
            for (n=0; n<GetNumberOfOutputSignals(); n++) {
                uint32 *signalOut = reinterpret_cast<uint32*>(GetOutputSignalMemory(n));
                uint32 e;
                for (e=0; e<elements[n]; e++) {
                    signalOut[e] = signalToGenerate[counter];
                }
            }

        }
        counter++;
        return true;
    }

    MARTe::uint32 *signalToGenerate;
    MARTe::uint32 *elements;
    MARTe::uint32 counter;
    MARTe::uint32 numberOfExecutes;
};
CLASS_REGISTER(MemoryMapAsyncOutputBrokerGAMTestHelper, "1.0")
/**
 * @brief DataSourceI implementation which returns a MemoryMapBrokerTestHelper broker.
 * @details This DataSource will verify in run-time if the signals being written by the Broker are the ones
 *  expected any particular test configuration.
 */
class MemoryMapAsyncOutputBrokerDataSourceTestHelper: public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapAsyncOutputBrokerDataSourceTestHelper    () {
        numberOfBuffers = 0u;
        offsets = NULL;
        totalNumberOfSignalElements = 0;
        signalMemory = NULL;
        cpuMask = 0xff;
        stackSize = 65536;
        numberOfExecutes = 0;
        expectedSignal = NULL;
        counter = 0;
        memoryOK = true;
    }

    virtual ~MemoryMapAsyncOutputBrokerDataSourceTestHelper() {
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
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        using namespace MARTe;
        data.Read("NumberOfBuffers", numberOfBuffers);
        data.Read("CPUMask", cpuMask);
        data.Read("StackSize", stackSize);
        AnyType signalAT = data.GetType("ExpectedSignal");
        numberOfExecutes = signalAT.GetNumberOfElements(0);
        expectedSignal = new uint32[numberOfExecutes];
        Vector<uint32> signalV(expectedSignal, numberOfExecutes);
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
        return "MemoryMapAsyncOutputBroker";
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
        broker = ReferenceT<MARTe::MemoryMapAsyncOutputBroker>("MemoryMapAsyncOutputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            ret = broker->InitWithBufferParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, cpuMask, stackSize);
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
            uint32 *signalMemory32 = reinterpret_cast<uint32 *>(signalMemory);
            if (memoryOK) {
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

    bool Flush() {
        return broker->Flush();
    }

    virtual void Purge(ReferenceContainer &purgeList) {
        if (broker.IsValid()) {
            broker->UnlinkDataSource();
        }
        DataSourceI::Purge(purgeList);
    }

    MARTe::uint32 numberOfBuffers;
    MARTe::uint32 *offsets;
    MARTe::uint32 cpuMask;
    MARTe::uint32 stackSize;
    MARTe::uint32 *expectedSignal;
    MARTe::uint32 numberOfExecutes;
    MARTe::uint32 totalNumberOfSignalElements;
    MARTe::uint32 counter;
    MARTe::ReferenceT<MARTe::MemoryMapAsyncOutputBroker> broker;
    bool memoryOK;
    void *signalMemory;
};
CLASS_REGISTER(MemoryMapAsyncOutputBrokerDataSourceTestHelper, "1.0")

/**
 * @brief Manual scheduler to test the correct interface between the Broker and the DataSourceI
 */
class MemoryMapAsyncOutputBrokerSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

MemoryMapAsyncOutputBrokerSchedulerTestHelper    () : MARTe::GAMSchedulerI() {
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

CLASS_REGISTER(MemoryMapAsyncOutputBrokerSchedulerTestHelper, "1.0")

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
 * Runs a mini MARTe application against the provide config. It patches the input configuration file with signalToGenerate and numberOfBuffer parameters
 *  and then it verifies that the broker correctly propagates this signal to the DataSource.
 */
static bool TestExecute_Buffers(const MARTe::char8 * const config, MARTe::uint32 *signalToGenerate, MARTe::uint32 toGenerateNumberOfElements,
                                MARTe::uint32 numberOfBuffers, MARTe::uint32 sleepMSec = 10) {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    cdb.MoveAbsolute("$Test.+Functions.+GAM1");
    cdb.Delete("Signal");
    Vector<uint32> signalV(signalToGenerate, toGenerateNumberOfElements);
    cdb.Write("Signal", signalV);

    cdb.MoveAbsolute("$Test.+Data.+Drv1");
    cdb.Delete("ExpectedSignal");
    cdb.Write("ExpectedSignal", signalV);
    cdb.Delete("NumberOfBuffers");
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
    ReferenceT<MemoryMapAsyncOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncOutputBrokerDataSourceTestHelper> dataSource;
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
        ok = dataSource->Flush();
    }

    godb->Purge();
    return ok;
}

//Base configuration file.
static const MARTe::char8 * const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncOutputBrokerGAMTestHelper"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
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
        "            Class = MemoryMapAsyncOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            CPUMask = 15"
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
        "        Class = MemoryMapAsyncOutputBrokerSchedulerTestHelper"
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
        "            Class = MemoryMapAsyncOutputBrokerGAMTestHelper"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
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
        "            Class = MemoryMapAsyncOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 0"
        "            CPUMask = 15"
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
        "        Class = MemoryMapAsyncOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//More than one GAM writing to the same DataSourceI (and as a consequence to the same MemoryMapAsyncOutputBroker)
static const MARTe::char8 * const config6 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncOutputBrokerGAMTestHelper"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
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
        "            Class = MemoryMapAsyncOutputBrokerGAMTestHelper"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
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
        "            Class = MemoryMapAsyncOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            CPUMask = 15"
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
        "        Class = MemoryMapAsyncOutputBrokerSchedulerTestHelper"
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
        "            Class = MemoryMapAsyncOutputBrokerGAMTestHelper"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
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
        "            Class = MemoryMapAsyncOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            CPUMask = 15"
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
        "        Class = MemoryMapAsyncOutputBrokerSchedulerTestHelper"
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
        "            Class = MemoryMapAsyncOutputBrokerGAMTestHelper"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
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
        "            Class = MemoryMapAsyncOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 1"
        "            CPUMask = 15"
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
        "        Class = MemoryMapAsyncOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

static const MARTe::char8 * const config10 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = MemoryMapAsyncOutputBrokerGAMTestHelper"
        "            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
        "            OutputSignals = {"
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
        "            Class = MemoryMapAsyncOutputBrokerDataSourceTestHelper"
        "            NumberOfBuffers = 10"
        "            CPUMask = 15"
        "            StackSize = 32768"
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
        "        Class = MemoryMapAsyncOutputBrokerSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool MemoryMapAsyncOutputBrokerTest::TestConstructor() {
    using namespace MARTe;
    MemoryMapAsyncOutputBroker broker;
    return true;
}

bool MemoryMapAsyncOutputBrokerTest::TestInit() {
    using namespace MARTe;
    MemoryMapAsyncOutputBroker broker;
    MemoryMapAsyncOutputBrokerDataSourceTestHelper ignore;
    return !broker.Init(OutputSignals, ignore, "", NULL);
}

bool MemoryMapAsyncOutputBrokerTest::TestInitWithBufferParameters() {
    using namespace MARTe;
    return TestIntegratedInApplication(config1, true);
}

bool MemoryMapAsyncOutputBrokerTest::TestInitWithBufferParameters_False_0_NumberOfBuffers() {
    using namespace MARTe;
    return !TestIntegratedInApplication(config2, true);
}

bool MemoryMapAsyncOutputBrokerTest::TestInitWithBufferParameters_False_MoreThanOneGAM() {
    using namespace MARTe;
    return !TestIntegratedInApplication(config6, true);
}

bool MemoryMapAsyncOutputBrokerTest::TestInitWithBufferParameters_False_Samples() {
    using namespace MARTe;
    return !TestIntegratedInApplication(config7, true);
}

bool MemoryMapAsyncOutputBrokerTest::TestExecute_N_Buffers() {
    using namespace MARTe;
    uint32 signalToGenerate[] = { 1, 2, 3, 4, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3,
            4, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

    return TestExecute_Buffers(config1, signalToGenerate, sizeof(signalToGenerate) / sizeof(uint32), 10);
}

bool MemoryMapAsyncOutputBrokerTest::TestExecute_1_Buffer() {
    using namespace MARTe;
    uint32 signalToGenerate[] = { 1, 2, 3 };

    return TestExecute_Buffers(config1, signalToGenerate, sizeof(signalToGenerate) / sizeof(uint32), 1, 100);
}

bool MemoryMapAsyncOutputBrokerTest::TestGetCPUMask() {
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
    ReferenceT<MemoryMapAsyncOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncOutputBrokerDataSourceTestHelper> dataSource;
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
    ReferenceT<MemoryMapAsyncOutputBroker> broker = brokers.Get(0);
    if (ok) {
        ok = (broker->GetCPUMask() == 0xf);
    }
    delete fakeMem;

    godb->Purge();
    return ok;
}

bool MemoryMapAsyncOutputBrokerTest::TestGetStackSize() {
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
    ReferenceT<MemoryMapAsyncOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncOutputBrokerDataSourceTestHelper> dataSource;
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
    ReferenceT<MemoryMapAsyncOutputBroker> broker = brokers.Get(0);
    if (ok) {
        ok = (broker->GetStackSize() == 32768);
    }
    delete fakeMem;

    godb->Purge();
    return ok;
}

bool MemoryMapAsyncOutputBrokerTest::TestGetNumberOfBuffers() {
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
    ReferenceT<MemoryMapAsyncOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncOutputBrokerDataSourceTestHelper> dataSource;
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
    ReferenceT<MemoryMapAsyncOutputBroker> broker = brokers.Get(0);
    if (ok) {
        ok = (broker->GetNumberOfBuffers() == 10);
    }
    delete fakeMem;

    godb->Purge();
    return ok;
}

bool MemoryMapAsyncOutputBrokerTest::TestExecute_Buffer_Overrun() {
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
    ReferenceT<MemoryMapAsyncOutputBrokerSchedulerTestHelper> scheduler;
    ReferenceT<MemoryMapAsyncOutputBrokerGAMTestHelper> gam;
    ReferenceT<MemoryMapAsyncOutputBrokerDataSourceTestHelper> dataSource;
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
    ReferenceT<MemoryMapAsyncOutputBroker> broker = brokers.Get(0);
    if (ok) {
        ok = broker->Execute();
    }
    if (ok) {
        ok = !broker->Execute();
    }
    delete fakeMem;

    godb->Purge();
    return ok;
}

bool MemoryMapAsyncOutputBrokerTest::TestIsIgnoringBufferOverrun() {
    using namespace MARTe;
    MemoryMapAsyncOutputBroker broker;

    bool ok = !broker.IsIgnoringBufferOverrun();
    if (ok) {
        broker.SetIgnoreBufferOverrun(true);
        ok = broker.IsIgnoringBufferOverrun();
    }
    if (ok) {
        broker.SetIgnoreBufferOverrun(false);
        ok = !broker.IsIgnoringBufferOverrun();
    }
    return ok;
}

bool MemoryMapAsyncOutputBrokerTest::TestSetIgnoreBufferOverrun() {
    return TestIsIgnoringBufferOverrun();
}
