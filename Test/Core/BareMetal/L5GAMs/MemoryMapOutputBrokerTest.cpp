/**
 * @file MemoryMapOutputBrokerTest.cpp
 * @brief Source file for class MemoryMapOutputBrokerTest
 * @date 30/07/2016
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
 * the class MemoryMapOutputBrokerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"
#include "MemoryMapOutputBrokerTest.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAMSchedulerI empty implementation to support the MemoryMapOutputBroker tests
 */
class MemoryMapOutputBrokerTestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapOutputBrokerTestScheduler1    ();

    virtual void StartExecution();

    virtual void StopExecution();

    virtual void CustomPrepareNextState(){

    }

};

MemoryMapOutputBrokerTestScheduler1::MemoryMapOutputBrokerTestScheduler1() :
        GAMSchedulerI() {

}

void MemoryMapOutputBrokerTestScheduler1::StartExecution() {

}

void MemoryMapOutputBrokerTestScheduler1::StopExecution() {

}

CLASS_REGISTER(MemoryMapOutputBrokerTestScheduler1, "1.0")

/**
 * @brief GAM implementation to support the MemoryMapOutputBroker tests.
 * @details This GAM generates a memory pattern (see Execute) which allows to verify if the
 *  MemoryMapOutputBroker is correctly copying the signal into the DataSource.
 */
class MemoryMapOutputBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapOutputBrokerTestGAM1    ();

    void *GetInputSignalsMemory();

    void *GetOutputSignalsMemory();

    void *GetInputSignalMemory(uint32 signalIdx);

    void *GetOutputSignalMemory(uint32 signalIdx);

    virtual bool Setup();

    virtual bool Execute();
};

MemoryMapOutputBrokerTestGAM1::MemoryMapOutputBrokerTestGAM1() :
        GAM() {
}

void *MemoryMapOutputBrokerTestGAM1::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void *MemoryMapOutputBrokerTestGAM1::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

void *MemoryMapOutputBrokerTestGAM1::GetInputSignalMemory(uint32 signalIdx) {
    return GAM::GetInputSignalMemory(signalIdx);
}

void *MemoryMapOutputBrokerTestGAM1::GetOutputSignalMemory(uint32 signalIdx) {
    return GAM::GetOutputSignalMemory(signalIdx);
}

bool MemoryMapOutputBrokerTestGAM1::Setup() {
    return true;
}

/**
 * @brief For each signal byte N, generates a pattern given by N*N
 */
bool MemoryMapOutputBrokerTestGAM1::Execute() {
    uint32 numberOfOutputSignals = GetNumberOfOutputSignals();
    uint32 n;
    bool ret = true;
    for (n = 0; (n < numberOfOutputSignals) && (ret); n++) {
        char8 *ptr = reinterpret_cast<char8 *>(GetOutputSignalMemory(n));
        uint32 byteSize;
        ret = GetSignalByteSize(OutputSignals, n, byteSize);
        uint32 samples = 0;
        ret = GetSignalNumberOfSamples(OutputSignals, n, samples);
        if (samples < 1) {
            samples = 1;
        }
        uint32 s;
        for (s = 0; s < byteSize * samples; s++) {
            ptr[s] = s * s;
        }
    }
    return true;
}

CLASS_REGISTER(MemoryMapOutputBrokerTestGAM1, "1.0")

/**
 * @brief DataSourceI implementation which returns a MemoryMapOutputBrokerTestHelper broker.
 */
class MemoryMapOutputBrokerDataSourceTestHelper: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapOutputBrokerDataSourceTestHelper    ();

    virtual ~ MemoryMapOutputBrokerDataSourceTestHelper();

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    virtual bool GetInputBrokers(
            ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    virtual bool GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    virtual bool Synchronise();

    void *signalMemory;
    uint32 *offsets;

    //Store 10 samples per signal.
    uint32 samples;
};

MemoryMapOutputBrokerDataSourceTestHelper::MemoryMapOutputBrokerDataSourceTestHelper() :
        DataSourceI() {
    signalMemory = NULL_PTR(void *);
    offsets = NULL_PTR(uint32 *);
    samples = 10;
}

MemoryMapOutputBrokerDataSourceTestHelper::~MemoryMapOutputBrokerDataSourceTestHelper() {
    if (signalMemory != NULL_PTR(void *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemory);
    }
    if (offsets != NULL_PTR(uint32 *)) {
        delete[] offsets;
    }
}

bool MemoryMapOutputBrokerDataSourceTestHelper::AllocateMemory() {
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
            totalMemorySize += memorySize * samples;
        }
    }
    if (ret) {
        signalMemory = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalMemorySize);
    }
    return ret;
}

uint32 MemoryMapOutputBrokerDataSourceTestHelper::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool MemoryMapOutputBrokerDataSourceTestHelper::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                                      const uint32 bufferIdx,
                                                                      void *&signalAddress) {
    char8 *memPtr = reinterpret_cast<char8 *>(signalMemory);
    memPtr += offsets[signalIdx];
    uint32 byteSize;
    bool ret = GetSignalByteSize(signalIdx, byteSize);
    if (ret) {
        signalAddress = reinterpret_cast<void *&>(memPtr);
    }
    return ret;
}

const char8 * MemoryMapOutputBrokerDataSourceTestHelper::GetBrokerName(StructuredDataI &data,
                                                                       const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapOutputBroker";
    }
    return "MemoryMapOutputBroker";
}

bool MemoryMapOutputBrokerDataSourceTestHelper::PrepareNextState(const char8 * const currentStateName,
                                                                 const char8 * const nextStateName) {
    return true;
}

bool MemoryMapOutputBrokerDataSourceTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                                const char8* const functionName,
                                                                void * const gamMemPtr) {

    return true;
}

bool MemoryMapOutputBrokerDataSourceTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                                 const char8* const functionName,
                                                                 void * const gamMemPtr) {
    ReferenceT<MemoryMapOutputBroker> broker("MemoryMapOutputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapOutputBrokerDataSourceTestHelper::Synchronise() {
    return false;
}

CLASS_REGISTER(MemoryMapOutputBrokerDataSourceTestHelper, "1.0");

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapOutputBrokerEnviroment(const char8 * const config) {
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->CleanUp();
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
 * MARTe configuration structure to test the MemoryMapOutputBroker
 */
static const char8 * const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapOutputBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal4A"
        "                   NumberOfDimensions = 2"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0 0} {2 5} {9 9}}"
        "                   Frequency = 5.0"
        "               }"
        "               Signal5 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 3"
        "               }"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Default = 7"
        "                   Type = uint32"
        "                   Alias = Signal1A"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = MemoryMapOutputBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = MemoryMapOutputBrokerTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal1A"
        "               }"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal4A"
        "                   NumberOfDimensions = 2"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0 0} {2 5} {9 9}}"
        "               }"
        "               Signal5 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 3"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = MemoryMapOutputBrokerTestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = MemoryMapOutputBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = MemoryMapOutputBrokerTestGAM1"
        "            OutputSignals = {"
        "               Signal3 = {"
        "                    DataSource = Drv1"
        "                    Type = int32"
        "                    NumberOfDimensions = 2"
        "                    NumberOfElements = 6"
        "                    Default = \"{3 6 9 12 15 18}\""
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = MemoryMapOutputBrokerDataSourceTestHelper"
        "            Signals = {"
        "                Signal1A = {"
        "                    Type = uint32"
        "                }"
        "                Signal2 = {"
        "                    Type = float32"
        "                }"
        "                Signal3 = {"
        "                    Type = int32"
        "                    NumberOfDimensions = 2"
        "                    NumberOfElements = 6"
        "                }"
        "            }"
        "        }"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
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
        "                    Functions = {GAMC GAMA GAMF}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMD GAMB GAME GAMF}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapOutputBrokerTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool MemoryMapOutputBrokerTest::TestConstructor() {
    ReferenceT<MemoryMapOutputBroker> broker("MemoryMapOutputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->GetNumberOfCopies() == 0;
    }
    return ret;
}

bool MemoryMapOutputBrokerTest::TestExecute() {
    bool ret = InitialiseMemoryMapOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapOutputBroker> broker;
    ReferenceT<MemoryMapOutputBrokerTestGAM1> gamC;
    ReferenceContainer brokers;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gamC = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMC");
        ret = gamC.IsValid();
    }

    if (ret) {
        ret = dataSource->GetOutputBrokers(brokers, "GAMC", (void *) gamC->GetOutputSignalsMemory());
    }
    if (ret) {
        ret = (brokers.Size() > 0u);
    }
    if (ret) {
        broker = brokers.Get(0);
        ret = broker.IsValid();
    }
    uint32 numberOfCopies;
    if (ret) {
        numberOfCopies = broker->GetNumberOfCopies();
        ret = (numberOfCopies == 5u);
    }
    if (ret) {
        ret = gamC->Execute();
    }
    if (ret) {
        ret = broker->Execute();
    }
    uint32 signalIdx;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal1A");
    }

    char8 *dsPtr;
    if (ret) {
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(dsPtr));
    }
    uint32 byteSize;
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }
    uint32 s;
    for (s = 0; (s < byteSize) && (ret); s++) {
        ret = (*(dsPtr++) == static_cast<char8>(s * s));
    }

    return ret;
}

bool MemoryMapOutputBrokerTest::TestExecute_Ranges() {
    bool ret = InitialiseMemoryMapOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapOutputBroker> broker;
    ReferenceT<MemoryMapOutputBrokerTestGAM1> gamC;
    ReferenceContainer brokers;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gamC = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMC");
        ret = gamC.IsValid();
    }

    if (ret) {
        ret = dataSource->GetOutputBrokers(brokers, "GAMC", (void *) gamC->GetOutputSignalsMemory());
    }
    if (ret) {
        ret = (brokers.Size() > 0u);
    }
    if (ret) {
        broker = brokers.Get(0);
        ret = broker.IsValid();
    }
    uint32 numberOfCopies;
    if (ret) {
        numberOfCopies = broker->GetNumberOfCopies();
        ret = (numberOfCopies == 5u);
    }
    if (ret) {
        ret = gamC->Execute();
    }
    if (ret) {
        ret = broker->Execute();
    }
    uint32 signalIdx;
    char8 *dsPtr;
    uint32 byteSize;
    uint32 s;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal4A");
    }
    if (ret) {
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(dsPtr));
    }
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }
    //First range
    for (s = 0; (s < 4) && (ret); s++) {
        ret = (*(dsPtr++) == static_cast<char8>(s * s));
    }
    //Second range
    dsPtr += 4;
    for (s = 4; (s < 20) && (ret); s++) {
        ret = (*(dsPtr++) == static_cast<char8>(s * s));
    }
    //Third range
    dsPtr += 12;
    for (s = 20; (s < 24) && (ret); s++) {
        ret = (*(dsPtr++) == static_cast<char8>(s * s));
    }

    return ret;
}

bool MemoryMapOutputBrokerTest::TestExecute_Samples() {
    bool ret = InitialiseMemoryMapOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapOutputBroker> broker;
    ReferenceT<MemoryMapOutputBrokerTestGAM1> gamC;
    ReferenceContainer brokers;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gamC = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMC");
        ret = gamC.IsValid();
    }

    if (ret) {
        ret = dataSource->GetOutputBrokers(brokers, "GAMC", (void *) gamC->GetOutputSignalsMemory());
    }
    if (ret) {
        ret = (brokers.Size() > 0u);
    }
    if (ret) {
        broker = brokers.Get(0);
        ret = broker.IsValid();
    }
    uint32 numberOfCopies;
    if (ret) {
        numberOfCopies = broker->GetNumberOfCopies();
        ret = (numberOfCopies == 5u);
    }
    if (ret) {
        ret = gamC->Execute();
    }
    if (ret) {
        ret = broker->Execute();
    }
    uint32 signalIdx;
    char8 *dsPtr;
    uint32 byteSize;
    uint32 s;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal5");
    }
    if (ret) {
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(dsPtr));
    }
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }
    //First range
    for (s = 0; (s < 12) && (ret); s++) {
        ret = (*(dsPtr++) == static_cast<char8>(s * s));
    }
    return ret;
}

