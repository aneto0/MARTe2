/**
 * @file MemoryMapInputBrokerTest.cpp
 * @brief Source file for class MemoryMapInputBrokerTest
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
 * the class MemoryMapInputBrokerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"
#include "MemoryMapInputBrokerTest.h"
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
 * @brief GAMSchedulerI empty implementation to support the MemoryMapInputBroker tests
 */
class MemoryMapInputBrokerTestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapInputBrokerTestScheduler1    ();

    virtual MARTe::ErrorManagement::ErrorType  StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType  StopCurrentStateExecution();

    virtual void CustomPrepareNextState();

};

MemoryMapInputBrokerTestScheduler1::MemoryMapInputBrokerTestScheduler1() :
        GAMSchedulerI() {

}

MARTe::ErrorManagement::ErrorType  MemoryMapInputBrokerTestScheduler1::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType   MemoryMapInputBrokerTestScheduler1::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

void MemoryMapInputBrokerTestScheduler1::CustomPrepareNextState(){

}


CLASS_REGISTER(MemoryMapInputBrokerTestScheduler1, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapInputBroker tests
 */
class MemoryMapInputBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapInputBrokerTestGAM1    ();

    void *GetInputSignalsMemory();

    void *GetOutputSignalsMemory();

    void *GetInputSignalMemory(uint32 signalIdx);

    void *GetOutputSignalMemory(uint32 signalIdx);

    virtual bool Setup();

    virtual bool Execute();
};

MemoryMapInputBrokerTestGAM1::MemoryMapInputBrokerTestGAM1() :
        GAM() {
}

void *MemoryMapInputBrokerTestGAM1::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void *MemoryMapInputBrokerTestGAM1::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

void *MemoryMapInputBrokerTestGAM1::GetInputSignalMemory(uint32 signalIdx) {
    return GAM::GetInputSignalMemory(signalIdx);
}

void *MemoryMapInputBrokerTestGAM1::GetOutputSignalMemory(uint32 signalIdx) {
    return GAM::GetOutputSignalMemory(signalIdx);
}

bool MemoryMapInputBrokerTestGAM1::Setup() {
    return true;
}

bool MemoryMapInputBrokerTestGAM1::Execute() {
    return true;
}

CLASS_REGISTER(MemoryMapInputBrokerTestGAM1, "1.0")

/**
 * @brief DataSourceI implementation which returns a MemoryMapInputBrokerTestHelper broker
 * and which offers a linear memory back-end implementation to read the signals.
 * This memory is set with a pattern (see GetSignalMemoryBuffer) which allows to verify if the MemoryMapInputBroker is
 * correctly copying the signals to the GAM memory.
 */
class MemoryMapInputBrokerDataSourceTestHelper: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapInputBrokerDataSourceTestHelper    ();

    virtual ~ MemoryMapInputBrokerDataSourceTestHelper();

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfStatefulMemoryBuffers();

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

    virtual uint32 GetCurrentStateBuffer();

    void *signalMemory;
    uint32 *offsets;

    //Store 10 samples per signal.
    uint32 samples;

    uint32 bufferIndex;
};

MemoryMapInputBrokerDataSourceTestHelper::MemoryMapInputBrokerDataSourceTestHelper() :
        DataSourceI() {
    signalMemory = NULL_PTR(void *);
    offsets = NULL_PTR(uint32 *);
    samples = 10;
    bufferIndex = 0;
}

MemoryMapInputBrokerDataSourceTestHelper::~MemoryMapInputBrokerDataSourceTestHelper() {
    if (signalMemory != NULL_PTR(void *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemory);
    }
    if (offsets != NULL_PTR(uint32 *)) {
        delete[] offsets;
    }
}

/**
 * @brief Allocates the memory for all the signals
 */
bool MemoryMapInputBrokerDataSourceTestHelper::AllocateMemory() {
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

uint32 MemoryMapInputBrokerDataSourceTestHelper::GetNumberOfStatefulMemoryBuffers() {
    return samples;
}

uint32 MemoryMapInputBrokerDataSourceTestHelper::GetNumberOfMemoryBuffers() {
    return 1u;
}

/**
 * @brief Returns the correct memory buffer and generates a pattern where the signal
 * address byte N contains the value N*N
 */
bool MemoryMapInputBrokerDataSourceTestHelper::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                                     const uint32 bufferIdx,
                                                                     void *&signalAddress) {
    char8 *memPtr = reinterpret_cast<char8 *>(signalMemory);
    memPtr += offsets[signalIdx];
    uint32 byteSize;
    bool ret = GetSignalByteSize(signalIdx, byteSize);
    uint32 n;
    for (n = 0; n < byteSize * samples; n++) {
        memPtr[n] = static_cast<char8>(n * n);
    }
    if (ret) {
        signalAddress = (void *)(&memPtr[bufferIdx*byteSize]);
    }
    return ret;
}

const char8 * MemoryMapInputBrokerDataSourceTestHelper::GetBrokerName(StructuredDataI &data,
                                                                      const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapInputBroker";
    }
    return "MemoryMapInputBroker";
}

bool MemoryMapInputBrokerDataSourceTestHelper::PrepareNextState(const char8 * const currentStateName,
                                                                const char8 * const nextStateName) {
    return true;
}

bool MemoryMapInputBrokerDataSourceTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                               const char8* const functionName,
                                                               void * const gamMemPtr) {
    ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapInputBrokerDataSourceTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                                const char8* const functionName,
                                                                void * const gamMemPtr) {
    return true;
}

uint32 MemoryMapInputBrokerDataSourceTestHelper::GetCurrentStateBuffer() {
    return bufferIndex;
}

bool MemoryMapInputBrokerDataSourceTestHelper::Synchronise() {
    bufferIndex++;
    bufferIndex %= samples;

    return true;
}

CLASS_REGISTER(MemoryMapInputBrokerDataSourceTestHelper, "1.0");

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
 * MARTe configuration structure to test the MemoryMapInputBroker
 */
static const char8 * const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapInputBrokerTestGAM1"
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
        "            Class = MemoryMapInputBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = MemoryMapInputBrokerTestGAM1"
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
        "            Class = MemoryMapInputBrokerTestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = MemoryMapInputBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = MemoryMapInputBrokerTestGAM1"
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
        "            Class = MemoryMapInputBrokerDataSourceTestHelper"
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
        "        Class = MemoryMapInputBrokerTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool MemoryMapInputBrokerTest::TestConstructor() {
    ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->GetNumberOfCopies() == 0;
    }
    return ret;
}

bool MemoryMapInputBrokerTest::TestExecute() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapInputBroker> broker;
    ReferenceT<MemoryMapInputBrokerTestGAM1> gamA;
    ReferenceContainer brokers;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }

    if (ret) {
        ret = dataSource->GetInputBrokers(brokers, "GAMA", (void *) gamA->GetInputSignalsMemory());
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
        ret = broker->Execute();
    }
    //Verify if the GAM has the expected pattern
    char8 *gamPtr = NULL_PTR(char8 *);
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(1));
    }
    uint32 copySize = broker->GetCopyByteSize(3);
    uint32 s;
    for (s = 0; (s < copySize) && (ret); s++) {
        ret = (*(gamPtr++) == static_cast<char8>(s * s));
    }

    return ret;
}

bool MemoryMapInputBrokerTest::TestExecute_Ranges() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapInputBroker> broker;
    ReferenceT<MemoryMapInputBrokerTestGAM1> gamA;
    ReferenceContainer brokers;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }

    if (ret) {
        ret = dataSource->GetInputBrokers(brokers, "GAMA", (void *) gamA->GetInputSignalsMemory());
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
        ret = broker->Execute();
    }
    //Verify if the GAM has the expected pattern
    char8 *gamPtr = NULL_PTR(char8 *);
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(0));
    }
    uint32 copySize = 0;
    uint32 s;
    copySize = broker->GetCopyByteSize(0);
    for (s = 0; (s < copySize) && (ret); s++) {
        ret = (*(gamPtr++) == static_cast<char8>(s * s));
    }
    copySize = broker->GetCopyByteSize(1);
    for (s = 0; (s < copySize) && (ret); s++) {
        //In the DataSource memory there are other ranges in the middle
        ret = (*(gamPtr++) == static_cast<char8>((s + 2 * 4) * (s + 2 * 4)));
    }
    copySize = broker->GetCopyByteSize(2);
    for (s = 0; (s < copySize) && (ret); s++) {
        //In the DataSource memory there are other ranges in the middle
        ret = (*(gamPtr++) == static_cast<char8>((s + 9 * 4) * (s + 9 * 4)));
    }

    return ret;
}

bool MemoryMapInputBrokerTest::TestExecute_Samples() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapInputBroker> broker;
    ReferenceT<MemoryMapInputBrokerTestGAM1> gamA;
    ReferenceContainer brokers;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }

    if (ret) {
        ret = dataSource->GetInputBrokers(brokers, "GAMA", (void *) gamA->GetInputSignalsMemory());
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
        ret = broker->Execute();
    }
    uint32 s;
    char8 *gamPtr = NULL_PTR(char8 *);
    //Skip the signal that belongs to the other DataSource...
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(3));
    }
    uint32 copySize = broker->GetCopyByteSize(4);
    for (s = 0; (s < copySize) && (ret); s++) {
        ret = (*(gamPtr++) == static_cast<char8>(s * s));
    }

    return ret;
}

bool MemoryMapInputBrokerTest::TestExecute_MultiStateBuffer() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT < MemoryMapInputBroker > broker;
    ReferenceT<MemoryMapInputBrokerTestGAM1> gamA;
    ReferenceContainer brokers;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }

    if (ret) {
        ret = dataSource->GetInputBrokers(brokers, "GAMA", (void *) gamA->GetInputSignalsMemory());
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
        ret = broker->Execute();
    }
    uint32 s;
    uint8 *gamPtr = NULL_PTR(uint8 *);
    //Skip the signal that belongs to the other DataSource...
    if (ret) {
        gamPtr = reinterpret_cast<uint8 *>(gamA->GetInputSignalMemory(3));
    }
    uint32 copySize = broker->GetCopyByteSize(4);

    uint8* orig = gamPtr;
    const uint32 nBuffers=10;

    for (s = 0; (s < copySize * nBuffers) && (ret); s++) {
        ret = ((*gamPtr) == (uint8) (s * s));
        gamPtr++;
        if ((s % copySize) == (copySize-1)) {
            gamPtr = orig;
            dataSource->Synchronise();
            if (ret) {
                ret = broker->Execute();
            }
        }
    }

    return ret;
}

