/**
 * @file CircularBufferThreadInputDataSourceTest.cpp
 * @brief Source file for class CircularBufferThreadInputDataSourceTest
 * @date 26/04/2018
 * @author Giuseppe Ferro
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
 * the class CircularBufferThreadInputDataSourceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
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
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class CircularBufferThreadInputDataSourceTestDS: public CircularBufferThreadInputDataSource {
public:
    CLASS_REGISTER_DECLARATION()

CircularBufferThreadInputDataSourceTestDS    ();
    virtual ~CircularBufferThreadInputDataSourceTestDS();

    virtual bool DriverRead(char8 * const bufferToFill,
            uint32 &sizeToRead,
            const uint32 signalIdx);

    virtual uint32 *GetCurrentInternalBuffer();

    virtual uint8 *GetIsRefreshed();

    virtual uint32 *GetLastReadBuffer();

    virtual uint32 GetTriggerAfterNPackets();

    virtual uint32 *NBrokerOpPerSignal();

    virtual uint32 GetNumberOfChannels();

    void SetDecrementSignal(uint32 decrementSignal);

    void SetSignalDriverFalse(uint32 signal);

    void SetSignalNoRead(uint32 signal);

    virtual bool TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples);

    virtual FastPollingMutexSem *GetMutex();

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    void ContinueRead();

private:
    uint32 decrementOnSignal;
    uint32 signalDriverFalse;
    uint32 signalNoRead;
    uint32 counter;
    volatile int32 continueRead;

};

CircularBufferThreadInputDataSourceTestDS::CircularBufferThreadInputDataSourceTestDS() {
    counter = 0u;
    continueRead = 0;
    decrementOnSignal = 0;
    signalDriverFalse = 10;
    signalNoRead = 10;
}

CircularBufferThreadInputDataSourceTestDS::~CircularBufferThreadInputDataSourceTestDS() {

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

uint32 CircularBufferThreadInputDataSourceTestDS::GetTriggerAfterNPackets() {
    return triggerAfterNPackets;
}

uint32 *CircularBufferThreadInputDataSourceTestDS::NBrokerOpPerSignal() {
    return nBrokerOpPerSignal;
}

uint32 CircularBufferThreadInputDataSourceTestDS::GetNumberOfChannels() {
    return numberOfChannels;
}

const char8 *CircularBufferThreadInputDataSourceTestDS::GetBrokerName(StructuredDataI &data,
                                                                      const SignalDirection direction) {

    const char8 *brokerName = CircularBufferThreadInputDataSource::GetBrokerName(data, direction);
    if (brokerName == NULL) {
        brokerName="Invalid";
    }
    return brokerName;

}

bool CircularBufferThreadInputDataSourceTestDS::DriverRead(char8 * const bufferToFill,
                                                           uint32 &sizeToRead,
                                                           const uint32 signalIdx) {
    //give time to sync
    if (signalIdx == 0u) {
        while (continueRead == 0) {
            Sleep::MSec(5);
        }
        Sleep::MSec(100);
    }

    printf("WRITE %d %d\n", signalIdx, continueRead);
    uint32 *bufferPtr = (uint32 *) bufferToFill;
    for (uint32 i = 0u; i < sizeToRead / 4; i++) {
        bufferPtr[i] = counter;
        counter++;
    }

    if (signalNoRead == signalIdx) {
        printf("WTF IM HERE!!!\n");
        sizeToRead = 0;
    }

    return (signalDriverFalse != signalIdx);
}

void CircularBufferThreadInputDataSourceTestDS::ContinueRead() {

    Atomic::TestAndSet(&continueRead);
}

bool CircularBufferThreadInputDataSourceTestDS::TerminateInputCopy(const uint32 signalIdx,
                                                                   const uint32 offset,
                                                                   const uint32 numberOfSamples) {
    CircularBufferThreadInputDataSource::TerminateInputCopy(signalIdx, offset, numberOfSamples);

    if (signalIdx == decrementOnSignal) {
        Atomic::Decrement(&continueRead);

        printf("HERE %d %d\n", signalIdx, continueRead);
    }
    return true;
}

FastPollingMutexSem *CircularBufferThreadInputDataSourceTestDS::GetMutex() {
    return &mutex;
}

CLASS_REGISTER(CircularBufferThreadInputDataSourceTestDS, "1.0")

class CircularBufferThreadInputDataSourceTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

CircularBufferThreadInputDataSourceTestGAM1    ();

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
        "                   Trigger = 2"
        "                   Samples = 3"
        "                   Frequency = 0"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0,0}, {2,2}}"
        "                   Trigger = 1"
        "                   Samples = 2"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Trigger = 1"
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
        "            CpuMask = 1"
        "            ReceiverThreadPriority = 31"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

CircularBufferThreadInputDataSourceTest::CircularBufferThreadInputDataSourceTest() {
    // Auto-generated constructor stub for CircularBufferThreadInputDataSourceTest
    // TODO Verify if manual additions are needed
}

CircularBufferThreadInputDataSourceTest::~CircularBufferThreadInputDataSourceTest() {
    // Auto-generated destructor stub for CircularBufferThreadInputDataSourceTest
    // TODO Verify if manual additions are needed
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
        ret = (dataSource.GetTriggerAfterNPackets() == 1);
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Trigger = 0"
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
            ret = offset == 0;
        }
        printf("offset=%d\n", offset);
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
            ret = offset == 200;
        }
        printf("offset=%d\n", offset);
    }
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Trigger = 0"
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
    //uint64 store = HighResolutionTimer::Counter();
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {

        dataSource->ContinueRead();

        if (ret) {
            if (n == 2) {
                return !dataSource->Synchronise();
            }

            ret = dataSource->Synchronise();
            if (ret) {
                //ret = broker->Execute();
                ret = broker1->Execute();
            }
        }
    }
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Trigger = 0"
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
    //uint64 store = HighResolutionTimer::Counter();
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
                //ret = broker->Execute();
                ret = broker1->Execute();
            }
        }
    }
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
    }
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
        ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker;
        ReferenceT<MemoryMapMultiBufferInputBroker> broker1;
        ReferenceContainer brokers;
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
            ret &= broker.IsValid();
        }
    }

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
            "                   Trigger = 2"
            "                   Samples = 10"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Trigger = 1"
            "                   Samples = 5"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 1"
            "                   Samples = 12"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   Trigger = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   Trigger = 1"
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
            "                   Trigger = 2"
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

    return !InitialiseMemoryMapInputBrokerEnviroment(config1);
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
            printf("I am %d\n", currentBuffer[0]);
        }
    }
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
        printf("offset = %d\n", offset);
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;

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

        ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker;
        ReferenceT<MemoryMapMultiBufferInputBroker> broker1;
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
            ret = broker->Execute();
            ret = broker1->Execute();
        }

    }

    if (ret) {
        uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();
        uint32 numberOfIntegers = 11;
        for (uint32 i = 0u; i < numberOfIntegers; i++) {
            printf("mem[%d]=%d\n", i, mem[i]);
        }
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,9}}"
            "                   Trigger = 1"
            "                   Samples = 3"
            "                   Alias = Signal1"
            "               }"
            "               Signal2a = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,1}, {8,9}}"
            "                   Trigger = 1"
            "                   Samples = 2"
            "                   Alias = Signal1"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 0"
            "                   Samples = 4"
            "                   Alias = Signal1"
            "               }"
            "               Signal4 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 1"
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

    uint32 numberOfReads = 5;
    uint32 offsetRead = 0u;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        if (ret) {
            dataSource->ContinueRead();
            ret = broker->Execute();
            ret = broker1->Execute();
        }

        if (ret) {
            printf("read %d\n", n);
            uint32 numberOfIntegers = 148;
            for (uint32 i = 0u; i < numberOfIntegers; i++) {
                printf("mem[%d]=%d\n", i, mem[i]);
            }
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
            "                   Trigger = 3"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Trigger = 1"
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
                printf("error[%d]=%d\n", i, mem[10 + i]);
                ret = (mem[10 + i] == 0);
            }
        }
    }
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Trigger = 0"
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
            //ret = broker1->Execute();
        }

        if (ret) {
            uint32 nSamples = 5;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                printf("error[%d]=%d\n", i, mem[10 + i]);
                if (n < 2) {
                    ret = (mem[10 + i] == 0);
                }
                else {
                    ret = (mem[10 + i] == 2);
                }

            }
        }
    }
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 1"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Trigger = 0"
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
        if (n == 2) {
            dataSource->SetSignalDriverFalse(1);
        }
        else {
            dataSource->SetSignalDriverFalse(10);
        }

        dataSource->ContinueRead();
        if (ret) {
            ret = broker->Execute();
            ret &= broker1->Execute();
        }

        if (ret) {
            uint32 nSamples = 10;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                printf("error[%d]=%d\n", i, mem[15 + i]);
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 1"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Trigger = 0"
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
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        if (n == 2) {
            dataSource->SetSignalDriverFalse(1);
        }
        else {
            dataSource->SetSignalDriverFalse(10);
        }

        dataSource->ContinueRead();
        if (ret) {
            ret = broker->Execute();
            //ret &= broker1->Execute();
        }

        if (ret) {
            uint32 nSamples = 10;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                printf("error[%d]=%d\n", i, mem[15 + i]);
                if (n < 2) {
                    ret = (mem[15 + i] == 0);
                }
                else if (n == 2) {
                    if (i % 2 == 0) {
                        ret = (mem[15 + i] == 2);
                    }
                    else {
                        ret = (mem[15 + i] == 1);
                    }
                }
                else if (n > 2) {
                    ret = (mem[15 + i] == 2);
                }
            }
        }
    }
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Trigger = 0"
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

    return !InitialiseMemoryMapInputBrokerEnviroment(config2);
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 1"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 0"
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

    return !InitialiseMemoryMapInputBrokerEnviroment(config2);
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Trigger = 0"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 0"
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

        dataSource->ContinueRead();
        if (ret) {
            ret = broker->Execute();
            ret &= broker1->Execute();
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
                printf("ts[%d]=%llu, delta=%f\n", i, *(uint64*) (&mem[10 + 2 * i]), delta);
                if (i > 0) {
                    ret = ((delta - delta_1) < resolution) || ((delta - delta_1) > -resolution);
                }
                delta_1 = delta;
            }
        }
    }
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Trigger = 0"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Trigger = 0"
            "                   Samples = 5"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 1"
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
                    printf("ts[%d]=%llu, delta=%f\n", i, *(uint64*) (&mem[10 + 2 * i]), delta);
                    store = *(uint64*) (&mem[10 + 2 * i]);
                    if (n > 0) {
                        ret &= ((delta - delta_1) < resolution) || ((delta - delta_1) > -resolution);
                    }
                    delta1_1 = delta1;
                }
                else {
                    delta1 = (*(uint64*) (&mem[10 + 2 * i]) - store1) * HighResolutionTimer::Period() * 1e3;
                    printf("ts[%d]=%llu, delta=%f\n", i, *(uint64*) (&mem[10 + 2 * i]), delta1);
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Trigger = 0"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Trigger = 0"
            "                   Samples = 5"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 1"
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
                    printf("ts[%d]=%llu, delta=%f\n", i, *(uint64*) (&mem[10 + 2 * i]), delta);
                    store = *(uint64*) (&mem[10 + 2 * i]);
                    if (n > 0) {
                        ret &= ((delta - delta_1) < resolution) || ((delta - delta_1) > -resolution);
                    }
                    delta1_1 = delta1;
                }
                else {
                    delta1 = (*(uint64*) (&mem[10 + 2 * i]) - store1) * HighResolutionTimer::Period() * 1e3;
                    printf("ts[%d]=%llu, delta=%f\n", i, *(uint64*) (&mem[10 + 2 * i]), delta1);
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 0"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 0"
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

    return !InitialiseMemoryMapInputBrokerEnviroment(config2);
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
            "                   Trigger = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Trigger = 0"
            "                   Samples = 5"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 0"
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

    return !InitialiseMemoryMapInputBrokerEnviroment(config2);
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

