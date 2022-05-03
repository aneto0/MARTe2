/**
 * @file CircularBufferThreadInputDataSourceTest.cpp
 * @brief Source file for class CircularBufferThreadInputDataSourceTest.
 * @date 12/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class CircularBufferThreadInputDataSourceTest (public, protected, and
 * private). Be aware that some methods, such as those inline could be defined
 * on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CircularBufferThreadInputDataSourceTest.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "MutexSem.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "ThreadInformation.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief Dummy implementation of CircularBufferThreadInputDataSource
 */
class CircularBufferThreadInputDataSourceTestDS: public CircularBufferThreadInputDataSource {
public:
    CLASS_REGISTER_DECLARATION()

    CircularBufferThreadInputDataSourceTestDS();
    
    virtual ~CircularBufferThreadInputDataSourceTestDS();

    virtual bool DriverRead(char8 * const bufferToFill,
            uint32 &sizeToRead,
            const uint32 signalIdx);

    virtual uint32 *GetCurrentInternalBuffer();

    virtual uint8 *GetIsRefreshed();

    virtual uint32 *GetLastReadBuffer();

    virtual uint32 GetTriggerAfterNSamples();

    virtual uint32 *NBrokerOpPerSignal();

    virtual uint32 GetNumberOfChannels();

    uint32 *GetNumberOfInterleavedSamples();

    uint32 *GetInterleavedSignalByteSize();

    uint32 *GetNumberOfInterleavedSignalMembers();

    uint32 *GetPacketMemberByteSize();

    void SetDecrementSignal(uint32 decrementSignal);

    void SetSignalDriverFalse(uint32 signal);

    void SetSignalNoRead(uint32 signal);

    virtual bool TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples);

    virtual FastPollingMutexSem *GetMutex();

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    void ContinueRead();

    ProcessorType GetCpuMask();

    uint8 GetPriorityLevel();

    uint32 GetStackSize();

    float GetSleepTime();

    void Stop() {
        sem.FastLock();
        stopped = true;
        sem.FastUnLock();
    }

private:
    uint32 decrementOnSignal;
    uint32 signalDriverFalse;
    uint32 signalNoRead;
    uint32 counter;
    bool stopped;
    volatile int32 continueRead;
    FastPollingMutexSem sem;

};

CircularBufferThreadInputDataSourceTestDS::CircularBufferThreadInputDataSourceTestDS() {
    counter = 0u;
    continueRead = 0;
    decrementOnSignal = 0;
    signalDriverFalse = 10;
    signalNoRead = 10;
    stopped = false;
    sem.Create();
}

CircularBufferThreadInputDataSourceTestDS::~CircularBufferThreadInputDataSourceTestDS() {

}

uint32 *CircularBufferThreadInputDataSourceTestDS::GetNumberOfInterleavedSamples() {
    return numberOfInterleavedSamples;
}

uint32 *CircularBufferThreadInputDataSourceTestDS::GetInterleavedSignalByteSize() {
    return interleavedSignalByteSize;
}

uint32 *CircularBufferThreadInputDataSourceTestDS::GetNumberOfInterleavedSignalMembers() {
    return numberOfInterleavedSignalMembers;
}

uint32 *CircularBufferThreadInputDataSourceTestDS::GetPacketMemberByteSize() {
    return interleavedPacketMemberByteSize;
}

void CircularBufferThreadInputDataSourceTestDS::SetDecrementSignal(uint32 decrementSignal) {
    decrementOnSignal = decrementSignal;
}

void CircularBufferThreadInputDataSourceTestDS::SetSignalDriverFalse(uint32 signal) {
    signalDriverFalse = signal;
}

void CircularBufferThreadInputDataSourceTestDS::SetSignalNoRead(uint32 signal) {
    signalNoRead = signal;
}

uint32 *CircularBufferThreadInputDataSourceTestDS::GetCurrentInternalBuffer() {
    return currentBuffer;
}

uint8 *CircularBufferThreadInputDataSourceTestDS::GetIsRefreshed() {
    return isRefreshed;
}

uint32 *CircularBufferThreadInputDataSourceTestDS::GetLastReadBuffer() {
    return lastReadBuffer;
}

uint32 CircularBufferThreadInputDataSourceTestDS::GetTriggerAfterNSamples() {
    return triggerAfterNSamples;
}

uint32 *CircularBufferThreadInputDataSourceTestDS::NBrokerOpPerSignal() {
    return nBrokerOpPerSignal;
}

uint32 CircularBufferThreadInputDataSourceTestDS::GetNumberOfChannels() {
    return numberOfChannels;
}

const char8 *CircularBufferThreadInputDataSourceTestDS::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {

    const char8 *brokerName = CircularBufferThreadInputDataSource::GetBrokerName(data, direction);

    if (brokerName == NULL) {
        brokerName = "Invalid";
    }

    return brokerName;
}

bool CircularBufferThreadInputDataSourceTestDS::DriverRead(char8 * const bufferToFill, uint32 &sizeToRead, const uint32 signalIdx) {
    sem.FastLock();
    if (!stopped) {
        sem.FastUnLock();
        //give time to sync
        if (signalIdx == 0u) {
            while (continueRead == 0) {
                sem.FastLock();
                if (stopped) {
                    sem.FastUnLock();
                    break;
                }
                else {
                    sem.FastUnLock();
                }
                Sleep::MSec(5);
            }
            Sleep::MSec(100);
        }

        uint32 *bufferPtr = (uint32 *) bufferToFill;
        for (uint32 i = 0u; i < sizeToRead / 4; i++) {
            bufferPtr[i] = counter;
            counter++;
        }

        if (signalNoRead == signalIdx) {
            sizeToRead = 0;
        }
    }
    else {
        sem.FastUnLock();
    }

    return (signalDriverFalse != signalIdx);
}

void CircularBufferThreadInputDataSourceTestDS::ContinueRead() {

    Atomic::TestAndSet(&continueRead);
}

bool CircularBufferThreadInputDataSourceTestDS::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples) {
    
    CircularBufferThreadInputDataSource::TerminateInputCopy(signalIdx, offset, numberOfSamples);

    if (signalIdx == decrementOnSignal) {
        Atomic::Decrement(&continueRead);
    }

    return true;
}

FastPollingMutexSem *CircularBufferThreadInputDataSourceTestDS::GetMutex() {
    return &mutex;
}

ProcessorType CircularBufferThreadInputDataSourceTestDS::GetCpuMask() {
    return executor.GetCPUMask();
}

uint8 CircularBufferThreadInputDataSourceTestDS::GetPriorityLevel() {
    return executor.GetPriorityLevel();
}

uint32 CircularBufferThreadInputDataSourceTestDS::GetStackSize() {
    return executor.GetStackSize();
}

float CircularBufferThreadInputDataSourceTestDS::GetSleepTime() {
    return sleepTime;
}

CLASS_REGISTER(CircularBufferThreadInputDataSourceTestDS, "1.0")

class CircularBufferThreadInputDataSourceTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    CircularBufferThreadInputDataSourceTestGAM1();

    virtual bool Setup();

    virtual bool Execute();
    void *GetInputMemoryBuffer();

    void *GetOutputMemoryBuffer();

private:
    uint32 totalSize;

};

CircularBufferThreadInputDataSourceTestGAM1::CircularBufferThreadInputDataSourceTestGAM1() :
        GAM() {
    totalSize = 0u;
}

bool CircularBufferThreadInputDataSourceTestGAM1::Setup() {

    return true;
}

bool CircularBufferThreadInputDataSourceTestGAM1::Execute() {
    return true;
}

void *CircularBufferThreadInputDataSourceTestGAM1::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

void *CircularBufferThreadInputDataSourceTestGAM1::GetOutputMemoryBuffer() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(CircularBufferThreadInputDataSourceTestGAM1, "1.0")

static const char8 * const config = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = CircularBufferThreadInputDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 3"
        "                   Frequency = 0"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0,0}, {2,2}}"
        "                   Samples = 2"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 4"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = CircularBufferThreadInputDataSourceTestDS"
        "            NumberOfBuffers = 100"
        "            SleepInMutexSec = 1e-9"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
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
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapInputBrokerTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapInputBrokerEnviroment(const char8 * const config) {

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
        application = god->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    return ok;
}

/**
 * Helper function to setup a MARTe execution environment with delete operations
 */
static bool InitialiseMemoryMapInputBrokerEnviromentRecall(const char8 * const config) {

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
        application = god->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

CircularBufferThreadInputDataSourceTest::CircularBufferThreadInputDataSourceTest() {
    //Taking a snapshot of the number of threads which were running before starting the test
    //This comes handy when running in FreeRTOS
    numOfThreadsBefore = Threads::NumberOfThreads();
}
#include <stdio.h>
CircularBufferThreadInputDataSourceTest::~CircularBufferThreadInputDataSourceTest() {

    ObjectRegistryDatabase::Instance()->Purge();
    uint32 nThreads = Threads::NumberOfThreads();
    printf("Killing threads\n");
    while (nThreads > numOfThreadsBefore) {
        printf("Thread killed\n");
        ThreadInformation tinfo;
        Threads::GetThreadInfoCopy(tinfo, 0u);
        ThreadIdentifier tid = tinfo.GetThreadIdentifier();
        Threads::Kill(tid);
    }

}

bool CircularBufferThreadInputDataSourceTest::TestConstructor() {

    CircularBufferThreadInputDataSourceTestDS dataSource;

    bool ret = (dataSource.GetCurrentInternalBuffer() == NULL);

    if (ret) {
        ret = (dataSource.GetIsRefreshed() == NULL);
    }
    if (ret) {
        ret = (dataSource.GetLastReadBuffer() == NULL);
    }
    if (ret) {
        ret = (dataSource.GetTriggerAfterNSamples() == 1);
    }
    if (ret) {
        ret = (dataSource.NBrokerOpPerSignal() == NULL);
    }
    if (ret) {
        ret = (dataSource.GetNumberOfChannels() == 0);
    }

    return true;
}

bool CircularBufferThreadInputDataSourceTest::TestInitialise() {

    CircularBufferThreadInputDataSourceTestDS dataSource;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
    }
    if (ret) {
        ret = dataSource.Initialise(cdb);
    }
    if (ret) {
        ret = (dataSource.GetCpuMask() == 0xFFFFu);
    }
    if (ret) {
        ret = (dataSource.GetPriorityLevel() == 31u);
    }
    if (ret) {
        ret = (dataSource.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    }
    dataSource.Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestInitialise_CpuMask() {

    CircularBufferThreadInputDataSourceTestDS dataSource;
    static const char8 * const configL = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            CpuMask = 1"
            "            SleepInMutexSec = 1e-9"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    StreamString configStream = configL;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
    }
    if (ret) {
        ret = dataSource.Initialise(cdb);
    }
    if (ret) {
        ret = (dataSource.GetCpuMask() == 1);
    }
    if (ret) {
        ret = (dataSource.GetPriorityLevel() == 31u);
    }
    if (ret) {
        ret = (dataSource.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    }
    dataSource.Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestInitialise_PriorityLevel() {

    CircularBufferThreadInputDataSourceTestDS dataSource;
    static const char8 * const configL = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            ReceiverThreadPriority = 30"
            "            SleepInMutexSec = 1e-9"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    StreamString configStream = configL;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
    }
    if (ret) {
        ret = dataSource.Initialise(cdb);
    }
    if (ret) {
        ret = (dataSource.GetCpuMask() == 0xFFFFu);
    }
    if (ret) {
        ret = (dataSource.GetPriorityLevel() == 30u);
    }
    if (ret) {
        ret = (dataSource.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    }
    dataSource.Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestInitialise_StackSize() {

    CircularBufferThreadInputDataSourceTestDS dataSource;
    static const char8 * const configL = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            ReceiverThreadStackSize = 300000"
            "            SleepInMutexSec = 1e-9"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    StreamString configStream = configL;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
    }
    if (ret) {
        ret = dataSource.Initialise(cdb);
    }
    if (ret) {
        ret = (dataSource.GetCpuMask() == 0xFFFFu);
    }
    if (ret) {
        ret = (dataSource.GetPriorityLevel() == 31u);
    }
    if (ret) {
        ret = (dataSource.GetStackSize() == 300000);
    }
    dataSource.Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestInitialise_SleepTime() {

    CircularBufferThreadInputDataSourceTestDS dataSource;
    static const char8 * const configL = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            ReceiverThreadStackSize = 300000"
            "            SleepTime = 0.1e-2"
            "            SleepInMutexSec = 1e-9"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    StreamString configStream = configL;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
    }
    if (ret) {
        ret = dataSource.Initialise(cdb);
    }
    if (ret) {
        ret = (dataSource.GetCpuMask() == 0xFFFFu);
    }
    if (ret) {
        ret = (dataSource.GetPriorityLevel() == 31u);
    }
    if (ret) {
        ret = (dataSource.GetStackSize() == 300000);
    }
    if (ret) {
        ret = (dataSource.GetSleepTime() == 0.1e-2F);
    }
    dataSource.Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestInitialise_SignalDefinitionInterleaved() {

    CircularBufferThreadInputDataSourceTestDS dataSource;
    static const char8 * const configL = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            ReceiverThreadStackSize = 300000"
            "            SignalDefinitionInterleaved = 1"
            "            SleepInMutexSec = 1e-9"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    StreamString configStream = configL;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
    }
    if (ret) {
        ret = dataSource.Initialise(cdb);
    }
    if (ret) {
        ret = (dataSource.GetCpuMask() == 0xFFFFu);
    }
    if (ret) {
        ret = (dataSource.GetPriorityLevel() == 31u);
    }
    if (ret) {
        ret = (dataSource.GetStackSize() == 300000);
    }
    dataSource.Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSynchronise() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");

    }
    if (ret) {
        dataSource->ContinueRead();
        ret = dataSource->Synchronise();
        dataSource->TerminateInputCopy(0, 0, 5);

    }
    if (ret) {
        uint32 offset;
        ret = dataSource->GetInputOffset(0, 5, offset);
        if (ret) {
            ret = (offset == 0);
        }
    }
    if (ret) {
        dataSource->ContinueRead();
        ret = dataSource->Synchronise();
        dataSource->TerminateInputCopy(0, 0, 5);

    }
    if (ret) {
        uint32 offset;
        ret = dataSource->GetInputOffset(0, 5, offset);
        if (ret) {
            ret = (offset == 200);
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSynchronise_FullRolling() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapMultiBufferInputBroker> broker1;
    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker1 = brokers.Get(1);
            ret = broker1.IsValid();
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
    }

    uint32 numberOfReads = 3;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {

        dataSource->ContinueRead();

        if (ret) {
            if (n == 2) {
                //It will fail as the new buffers are not being marked as read.
                ret = !dataSource->Synchronise();
            }
            else {
                ret = dataSource->Synchronise();
                if (ret) {
                    ret = broker1->Execute();
                }
            }
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSynchronise_GetLatest() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 12"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapMultiBufferInputBroker> broker1;
    ReferenceContainer brokers;
    FastPollingMutexSem *mux;
    uint8 *refreshed;
    uint32 numberOfSignals;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker1 = brokers.Get(1);
            ret = broker1.IsValid();
        }
        if (ret) {
            mux = dataSource->GetMutex();
            refreshed = dataSource->GetIsRefreshed();
            numberOfSignals = dataSource->GetNumberOfSignals();
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
    }

    uint32 numberOfReads = 3;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {

        dataSource->ContinueRead();
        uint32 counter = 0u;
        while (counter < 8) {
            mux->FastLock();
            counter += (refreshed[n * numberOfSignals] == 1);
            mux->FastUnLock();
            Sleep::MSec(5);
        }

        if (ret) {
            ret = dataSource->Synchronise();
            if (ret) {
                ret = broker1->Execute();
            }
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSynchronise_SleepTime() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            SleepTime = 1e-9"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");

    }
    if (ret) {
        dataSource->ContinueRead();
        ret = dataSource->Synchronise();
        dataSource->TerminateInputCopy(0, 0, 5);
    }
    if (ret) {
        uint32 offset;
        ret = dataSource->GetInputOffset(0, 5, offset);
        if (ret) {
            ret = (offset == 0);
        }
    }
    if (ret) {
        dataSource->ContinueRead();
        ret = dataSource->Synchronise();
        dataSource->TerminateInputCopy(0, 0, 5);

    }
    if (ret) {
        uint32 offset;
        ret = dataSource->GetInputOffset(0, 5, offset);
        if (ret) {
            ret = (offset == 200);
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestGetBrokerName() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ConfigurationDatabase cdb;
        float32 freq = 0.;
        cdb.Write("Frequency", freq);
        const char8* brokerName = dataSource->GetBrokerName(cdb, InputSignals);
        ret = StringHelper::Compare(brokerName, "MemoryMapSynchronisedMultiBufferInputBroker") == 0;
        if (ret) {
            freq = -1.;
            cdb.Delete("Frequency");
            cdb.Write("Frequency", freq);
            const char8* brokerName = dataSource->GetBrokerName(cdb, InputSignals);
            ret = StringHelper::Compare(brokerName, "MemoryMapMultiBufferInputBroker") == 0;
        }
        if (ret) {
            cdb.Delete("Frequency");
            const char8* brokerName = dataSource->GetBrokerName(cdb, InputSignals);
            ret = StringHelper::Compare(brokerName, "MemoryMapMultiBufferInputBroker") == 0;
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestGetInputBrokers() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        ReferenceT<MemoryMapMultiBufferInputBroker> broker;
        ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker1;
        ReferenceContainer brokers;
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
            ret &= broker.IsValid();
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestGetOutputBrokers() {

    CircularBufferThreadInputDataSourceTestDS dataSource;

    ReferenceContainer inputBrokers;
    uint32 x;
    bool ret = !dataSource.GetOutputBrokers(inputBrokers, "GAMA", (void*) &x);

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 10"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 12"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetIsRefreshed() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetLastReadBuffer() != NULL);
    }
    if (ret) {
        ret = (dataSource->NBrokerOpPerSignal() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetNumberOfChannels() == 3);
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_PacketMemberSizes() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 10"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 12"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            Signals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   PacketMemberSizes = {1, 1}"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   PacketMemberSizes = {4, 4}"
            "               }"
            "            }"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetIsRefreshed() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetLastReadBuffer() != NULL);
    }

    if (ret) {
        ret = (dataSource->NBrokerOpPerSignal() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetNumberOfChannels() == 3);
    }
    uint32 numberOfSignals = 0u;
    if (ret) {
        numberOfSignals = dataSource->GetNumberOfSignals();
    }
    if (ret) {
        uint32 testInputSamples[] = { 2, 5, 1, 1, 1 };
        uint32 *inputSamples = dataSource->GetNumberOfInterleavedSamples();
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            ret = (inputSamples[i] == testInputSamples[i]);
        }
    }
    if (ret) {
        uint32 testInputByteSize[] = { 2, 8, 4, 24, 12 };
        uint32 *inputByteSize = dataSource->GetInterleavedSignalByteSize();
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            ret = (testInputByteSize[i] == inputByteSize[i]);
        }
    }
    if (ret) {
        uint32 testNInputChunks[] = { 2, 2, 0, 0, 0 };
        uint32 *nInputChunks = dataSource->GetNumberOfInterleavedSignalMembers();
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            ret = (testNInputChunks[i] == nInputChunks[i]);
        }
    }
    if (ret) {
        uint32 testPacketInputChunkSize[] = { 1, 1, 4, 4 };
        uint32 *packetInputChunkSize = dataSource->GetPacketMemberByteSize();
        for (uint32 i = 0u; (i < 4) && (ret); i++) {
            ret = (testPacketInputChunkSize[i] == packetInputChunkSize[i]);
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_SignalDefinitionInterleaved() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 10"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 10"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 10"
            "               }"
            "               Signal4 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 10"
            "               }"
            "               Signal5 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 10"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            SignalDefinitionInterleaved = 1"
            "            Signals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "               Signal4 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "               Signal5 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetIsRefreshed() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetLastReadBuffer() != NULL);
    }
    if (ret) {
        ret = (dataSource->NBrokerOpPerSignal() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetNumberOfChannels() == 5);
    }
    if (ret) {
        uint32 *inputSamples = dataSource->GetNumberOfInterleavedSamples();
        ret = (inputSamples[0] == 10u);
    }
    if (ret) {
        uint32 *interleavedInputByteSize = dataSource->GetInterleavedSignalByteSize();
        ret = (interleavedInputByteSize[0] == (4 + 20 + 4 + 8 + 1));
    }
    if (ret) {
        uint32 *nInputChunks = dataSource->GetNumberOfInterleavedSignalMembers();
        ret = (nInputChunks[0] == dataSource->GetNumberOfChannels());
    }
    if (ret) {
        uint32 testPacketInputChunkSize[] = { 4, 20, 4, 8, 1 };
        uint32 *packetInputChunkSize = dataSource->GetPacketMemberByteSize();
        for (uint32 i = 0u; (i < 5) && (ret); i++) {
            ret = (testPacketInputChunkSize[i] == packetInputChunkSize[i]);
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_False_SignalDefinitionInterleaved_NOfSamples() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 10"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 10"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 8"
            "               }"
            "               Signal4 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 10"
            "               }"
            "               Signal5 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 10"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            SignalDefinitionInterleaved = 1"
            "            Signals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "               Signal4 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "               Signal5 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config1);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_SignalDefinitionInterleaved_HeaderSize() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 1"
            "                   Frequency = 0"
            "                   HeaderSize = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            SignalDefinitionInterleaved = 1"
            "            Signals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_PacketMemberSizes_RecallAllocation() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 10"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 12"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            Signals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   PacketMemberSizes = {1, 1}"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   PacketMemberSizes = {4, 4}"
            "               }"
            "            }"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviromentRecall(config1);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_SignalDefinitionInterleaved_RecallAllocation() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 1"
            "                   Frequency = 0"
            "                   HeaderSize = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            SignalDefinitionInterleaved = 1"
            "            Signals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviromentRecall(config1);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_PacketMemberSizes_HeaderSize() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 10"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 12"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            Signals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   PacketMemberSizes = {1, 1}"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   PacketMemberSizes = {4, 4}"
            "                   HeaderSize = 8"
            "               }"
            "            }"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetIsRefreshed() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetLastReadBuffer() != NULL);
    }
    if (ret) {
        ret = (dataSource->NBrokerOpPerSignal() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetNumberOfChannels() == 3);
    }

    uint32 numberOfSignals = 0u;
    if (ret) {
        numberOfSignals = dataSource->GetNumberOfSignals();
    }
    if (ret) {
        //4 because 40-8=32/(4+4)=4
        uint32 testInputSamples[] = { 2, 4, 1, 1, 1 };
        uint32 *inputSamples = dataSource->GetNumberOfInterleavedSamples();
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            ret = (inputSamples[i] == testInputSamples[i]);
        }
    }
    if (ret) {
        uint32 testInputByteSize[] = { 2, 8, 4, 24, 12 };
        uint32 *inputByteSize = dataSource->GetInterleavedSignalByteSize();
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            ret = (testInputByteSize[i] == inputByteSize[i]);
        }
    }
    if (ret) {
        uint32 testNInputChunks[] = { 2, 2, 0, 0, 0 };
        uint32 *nInputChunks = dataSource->GetNumberOfInterleavedSignalMembers();
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            ret = (testNInputChunks[i] == nInputChunks[i]);
        }
    }
    if (ret) {

        uint32 testPacketInputChunkSize[] = { 1, 1, 4, 4 };
        uint32 *packetInputChunkSize = dataSource->GetPacketMemberByteSize();
        for (uint32 i = 0u; (i < 4) && (ret); i++) {
            ret = (testPacketInputChunkSize[i] == packetInputChunkSize[i]);
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_False_WrittenSignal() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 10"
            "                   Frequency = 0"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 10"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config1);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestPrepareNextState() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    if (ret) {
        uint32 *currentBuffer = dataSource->GetCurrentInternalBuffer();
        FastPollingMutexSem *mutex = dataSource->GetMutex();
        while (1) {
            dataSource->ContinueRead();
            mutex->FastLock();
            if (currentBuffer[0] >= 2) {
                mutex->FastUnLock();
                break;
            }
            mutex->FastUnLock();
            Sleep::MSec(5);
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return true;
}

bool CircularBufferThreadInputDataSourceTest::TestGetInputOffset() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    uint32 nTests = 4;
    for (uint32 i = 0u; (i < nTests) && (ret); i++) {
        dataSource->ContinueRead();
        dataSource->Synchronise();
        dataSource->PrepareInputOffsets();
        dataSource->TerminateInputCopy(0, 0, 0);

        uint32 offset;
        ret = dataSource->GetInputOffset(0, 10, offset);
        if (ret) {
            ret = offset == 0;
        }

        ret = (offset == (372 + 12 * i) % 400);
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;

}

bool CircularBufferThreadInputDataSourceTest::PrepareInputOffsets() {
    return TestGetInputOffset();
}

bool CircularBufferThreadInputDataSourceTest::TestGetOutputOffset() {

    CircularBufferThreadInputDataSourceTestDS dataSource;
    uint32 offset;

    return !dataSource.GetOutputOffset(0, 0, offset);
}

bool CircularBufferThreadInputDataSourceTest::TestExecute() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        ReferenceT<MemoryMapMultiBufferInputBroker> broker;
        ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker1;
        ReferenceContainer brokers;
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
        if (ret) {
            dataSource->ContinueRead();
            ret = broker1->Execute();
            ret = broker->Execute();
        }
    }
    if (ret) {
        uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();
        ret &= mem[0] == 0;
        ret &= mem[1] == 12;
        ret &= mem[2] == 24;
        ret &= mem[3] == 13;
        ret &= mem[4] == 25;
        ret &= mem[5] == 15;
        ret &= mem[6] == 27;
        ret &= mem[7] == 0;
        ret &= mem[8] == 11;
        ret &= mem[9] == 23;
        ret &= mem[10] == 35;

    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestExecute_SameSignalDifferentModes() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,9}}"
            "                   Samples = 3"
            "                   Alias = Signal1"
            "               }"
            "               Signal2a = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,1}, {8,9}}"
            "                   Samples = 2"
            "                   Alias = Signal1"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 4"
            "                   Alias = Signal1"
            "               }"
            "               Signal4 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 6"
            "                   Alias = Signal1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 20"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapMultiBufferInputBroker> broker;
    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker1;
    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
    }
    uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();

    uint32 numberOfReads = 5;
    uint32 offsetRead = 0u;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        if (ret) {
            dataSource->ContinueRead();
            ret = broker1->Execute();
            ret = broker->Execute();
        }
        if (ret) {
            //the first are separated by 10
            uint32 nSamples = 5;
            uint32 j = 0u;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == i * 10 + offsetRead);
                j++;
            }
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == (i * 10 + 2 + offsetRead));
                j++;
            }
            nSamples = 30;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == (i + 20 + offsetRead));
                j++;
            }
            nSamples = 2;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == (30 + i * 10 + offsetRead));
                j++;
                ret &= (mem[j] == (31 + i * 10 + offsetRead));
                j++;
            }
            nSamples = 2;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == (i * 10 + 38 + offsetRead));
                j++;
                ret &= (mem[j] == (i * 10 + 39 + offsetRead));
                j++;
            }
            nSamples = 40;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == i + 10 + offsetRead);
                j++;
            }
            nSamples = 10;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == (i + offsetRead - 10) * (offsetRead > 0));
                j++;
            }
            nSamples = 50;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {

                ret &= (mem[j] == (i + offsetRead));
                j++;
            }
        }
        offsetRead += 50;
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestExecute_ErrorCheck() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 20"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker;
    ReferenceT<MemoryMapMultiBufferInputBroker> broker1;
    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
    }

    uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();

    uint32 numberOfReads = 4;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        if (ret) {
            dataSource->ContinueRead();
            ret = broker->Execute();
            ret = broker1->Execute();
        }
        if (ret) {
            uint32 nSamples = 5;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret = (mem[10 + i] == 0);
            }
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestExecute_ErrorCheck_Overwrite() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        dataSource->SetDecrementSignal(1);
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker;
    ReferenceT<MemoryMapMultiBufferInputBroker> broker1;
    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
            if (ret) {
                ret = broker1.IsValid();
            }
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
    }
    uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();

    uint32 numberOfReads = 4;
    //uint64 store = HighResolutionTimer::Counter();
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        dataSource->ContinueRead();
        if (ret) {
            ret = broker->Execute();
        }
        if (ret) {
            uint32 nSamples = 5;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                if (n < 2) {
                    ret = (mem[10 + i] == 0);
                }
                else {
                    ret = (mem[10 + i] == 2);
                }

            }
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestExecute_ErrorCheck_DriverRead() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        dataSource->SetDecrementSignal(1);
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapMultiBufferInputBroker> broker;
    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker1;
    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
            if (ret) {
                ret = broker1.IsValid();
            }
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
    }
    uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();

    uint32 numberOfReads = 4;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        if (n == 2) {
            dataSource->SetSignalDriverFalse(1);
        }
        else {
            dataSource->SetSignalDriverFalse(10);
        }
        dataSource->ContinueRead();
        if (ret) {
            ret = broker1->Execute();
            ret &= broker->Execute();
        }
        if (ret) {
            uint32 nSamples = 10;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                if (n != 2) {
                    ret = (mem[15 + i] == 0);
                }
                else {
                    if (i % 2 == 0) {
                        ret = (mem[15 + i] == 0);
                    }
                    else {
                        ret = (mem[15 + i] == 1);
                    }
                }
            }
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestExecute_ErrorCheck_Both() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        dataSource->SetDecrementSignal(1);
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapMultiBufferInputBroker> broker;
    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker1;

    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
            if (ret) {
                ret = broker1.IsValid();
            }
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
    }
    uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();

    uint32 numberOfReads = 4;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        if (n == 2) {
            dataSource->SetSignalDriverFalse(1);
        }
        else {
            dataSource->SetSignalDriverFalse(10);
        }
        dataSource->ContinueRead();
        if (ret) {
            ret = broker1->Execute();
            //ret &= broker1->Execute();
        }
        if (ret) {
            uint32 nSamples = 10;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                //The offset of 15 in 'mem[15 + i]' gets the ErrorCheck signals.
                if (n < 2) {
                    //See CircularBufferThreadInputDataSource.h.
                    //2u = 2'b00 = {Write overlap==false, DriverRead(*)==true].
                    ret = (mem[15 + i] == 0);
                }
                else if (n == 2) {
                    if (i % 2 == 0) {
                        //See CircularBufferThreadInputDataSource.h.
                        //2u = 2'b10 = {Write overlap==true, DriverRead(*)==true]
                        ret = (mem[15 + i] == 2);
                    }
                    else {
                        //See CircularBufferThreadInputDataSource.h.
                        //3u = 2'b11 = {Write overlap==true, DriverRead(*)==false]
                        ret = (mem[15 + i] == 3);
                    }
                }
                else if (n > 2) {
                    //See CircularBufferThreadInputDataSource.h.
                    //2u = 2'b10 = {Write overlap==true, DriverRead(*)==true]
                    ret = (mem[15 + i] == 2);
                }
            }
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_ErrorCheck_False_BadType() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config2);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_ErrorCheck_False_BadSize() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config2);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_False_InvalidPacketMemberSize() {
    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            Signals = {"
            "               Signal1 = {"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   PacketMemberSizes = {7}"
            "               }"
            "            }"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config2);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_False_PacketMemberSizeGreater() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            Signals = {"
            "               Signal1 = {"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   PacketMemberSizes = {4, 8, 16, 16}"
            "               }"
            "            }"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config2);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestExecute_TimeStamp() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        dataSource->SetDecrementSignal(0);
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapMultiBufferInputBroker> broker;
    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker1;
    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
            if (ret) {
                ret = broker1.IsValid();
            }
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
    }
    uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();

    uint32 numberOfReads = 4;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        dataSource->ContinueRead();
        if (ret) {
            ret = broker1->Execute();
            ret &= broker->Execute();
        }
        if (ret) {
            uint32 nSamples = 5;
            float32 delta = 0;
            float32 delta_1 = 0;
            float32 resolution = 10.F;
            uint64 store = *(uint64*) (&mem[10]);
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                delta = (*(uint64*) (&mem[10 + 2 * i]) - store) * HighResolutionTimer::Period() * 1e3;
                store = *(uint64*) (&mem[10 + 2 * i]);
                if (i > 0) {
                    ret = ((delta - delta_1) < resolution) || ((delta - delta_1) > -resolution);
                }
                delta_1 = delta;
            }
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestExecute_TimeStamp_NoRead() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Samples = 5"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        dataSource->SetDecrementSignal(0);
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker;
    ReferenceT<MemoryMapMultiBufferInputBroker> broker1;
    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
            if (ret) {
                ret = broker1.IsValid();
            }
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
    }
    uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();

    uint32 numberOfReads = 4;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        if (n > 1) {
            dataSource->SetSignalNoRead(3);
        }
        dataSource->ContinueRead();
        if (ret) {
            ret = broker->Execute();
            ret &= broker1->Execute();
        }
        if (ret) {
            //2 signals, 5 samples
            uint32 nSamples = 10;
            float32 delta = 0;
            float32 delta_1 = 0;
            uint64 store = *(uint64*) (&mem[10]);
            float32 delta1 = 0;
            float32 delta1_1 = 0;
            uint64 store1 = *(uint64*) (&mem[10 + 2]);
            float32 resolution = 10.F;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                if (i % 2 == 0) {
                    delta = (*(uint64*) (&mem[10 + 2 * i]) - store) * HighResolutionTimer::Period() * 1e3;
                    store = *(uint64*) (&mem[10 + 2 * i]);
                    if (n > 0) {
                        ret &= ((delta - delta_1) < resolution) || ((delta - delta_1) > -resolution);
                    }
                    delta1_1 = delta1;
                }
                else {
                    delta1 = (*(uint64*) (&mem[10 + 2 * i]) - store1) * HighResolutionTimer::Period() * 1e3;
                    store1 = *(uint64*) (&mem[10 + 2 * i]);
                    if (n > 0) {
                        ret &= ((delta1 - delta1_1) < resolution) || ((delta1 - delta1_1) > -resolution);
                    }
                    if (n > 1) {
                        ret &= delta1 == 0;
                    }
                    delta1_1 = delta1;
                }

            }
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestExecute_TimeStamp_FalseDriverRead() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Samples = 5"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        dataSource->SetDecrementSignal(0);
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker;
    ReferenceT<MemoryMapMultiBufferInputBroker> broker1;
    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
            if (ret) {
                ret = broker1.IsValid();
            }
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
    }
    uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();

    uint32 numberOfReads = 4;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        if (n > 1) {
            dataSource->SetSignalDriverFalse(3);
        }
        dataSource->ContinueRead();
        if (ret) {
            ret = broker->Execute();
            ret &= broker1->Execute();
        }
        if (ret) {
            //2 signals, 5 samples
            uint32 nSamples = 10;
            float32 delta = 0;
            float32 delta_1 = 0;
            uint64 store = *(uint64*) (&mem[10]);
            float32 delta1 = 0;
            float32 delta1_1 = 0;
            uint64 store1 = *(uint64*) (&mem[10 + 2]);
            float32 resolution = 10.F;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                if (i % 2 == 0) {
                    delta = (*(uint64*) (&mem[10 + 2 * i]) - store) * HighResolutionTimer::Period() * 1e3;
                    store = *(uint64*) (&mem[10 + 2 * i]);
                    if (n > 0) {
                        ret &= ((delta - delta_1) < resolution) || ((delta - delta_1) > -resolution);
                    }
                    delta1_1 = delta1;
                }
                else {
                    delta1 = (*(uint64*) (&mem[10 + 2 * i]) - store1) * HighResolutionTimer::Period() * 1e3;
                    store1 = *(uint64*) (&mem[10 + 2 * i]);
                    if (n > 0) {
                        ret &= ((delta1 - delta1_1) < resolution) || ((delta1 - delta1_1) > -resolution);
                    }
                    if (n > 1) {
                        ret &= delta1 == 0;
                    }
                    delta1_1 = delta1;
                }

            }
        }
    }
    dataSource->Stop();
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_TimeStamp_False_BadType() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config2);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase_TimeStamp_False_BadSize() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = MemoryMapInputBrokerTestScheduler1"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config2);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestPurge() {

    CircularBufferThreadInputDataSourceTestDS ds;
    ReferenceContainer purgeList;
    ds.Purge(purgeList);

    return true;
}

bool CircularBufferThreadInputDataSourceTest::TestTerminateInputCopy() {
    return TestExecute();
}
