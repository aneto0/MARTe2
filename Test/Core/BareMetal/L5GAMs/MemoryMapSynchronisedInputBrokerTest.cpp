/**
 * @file MemoryMapSynchronisedInputBrokerTest.cpp
 * @brief Source file for class MemoryMapSynchronisedInputBrokerTest
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
 * the class MemoryMapSynchronisedInputBrokerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"
#include "MemoryMapSynchronisedInputBrokerTest.h"
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
 * @brief GAMSchedulerI empty implementation to support the MemoryMapSynchronisedInputBroker tests
 */
class MemoryMapSynchronisedInputBrokerTestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapSynchronisedInputBrokerTestScheduler1    ();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();

    virtual void CustomPrepareNextState(){

    }

};

MemoryMapSynchronisedInputBrokerTestScheduler1::MemoryMapSynchronisedInputBrokerTestScheduler1() :
        GAMSchedulerI() {

}

MARTe::ErrorManagement::ErrorType MemoryMapSynchronisedInputBrokerTestScheduler1::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType MemoryMapSynchronisedInputBrokerTestScheduler1::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

CLASS_REGISTER(MemoryMapSynchronisedInputBrokerTestScheduler1, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapSynchronisedInputBroker tests
 */
class MemoryMapSynchronisedInputBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapSynchronisedInputBrokerTestGAM1    ();

    void *GetInputSignalsMemory();

    void *GetOutputSignalsMemory();

    void *GetInputSignalMemory(uint32 signalIdx);

    void *GetOutputSignalMemory(uint32 signalIdx);

    virtual bool Setup();

    virtual bool Execute();
};

MemoryMapSynchronisedInputBrokerTestGAM1::MemoryMapSynchronisedInputBrokerTestGAM1() :
        GAM() {
}

void *MemoryMapSynchronisedInputBrokerTestGAM1::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void *MemoryMapSynchronisedInputBrokerTestGAM1::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

void *MemoryMapSynchronisedInputBrokerTestGAM1::GetInputSignalMemory(uint32 signalIdx) {
    return GAM::GetInputSignalMemory(signalIdx);
}

void *MemoryMapSynchronisedInputBrokerTestGAM1::GetOutputSignalMemory(uint32 signalIdx) {
    return GAM::GetOutputSignalMemory(signalIdx);
}

bool MemoryMapSynchronisedInputBrokerTestGAM1::Setup() {
    return true;
}

bool MemoryMapSynchronisedInputBrokerTestGAM1::Execute() {
    return true;
}

CLASS_REGISTER(MemoryMapSynchronisedInputBrokerTestGAM1, "1.0")

/**
 * @brief DataSourceI implementation which returns a MemoryMapSynchronisedInputBrokerTestHelper broker
 * and which offers a linear memory back-end implementation to read the signals.
 * This memory is set with a pattern (see GetSignalMemoryBuffer) which allows to verify if the MemoryMapSynchronisedInputBroker is
 * correctly copying the signals to the GAM memory.
 */
class MemoryMapSynchronisedInputBrokerDataSourceTestHelper: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapSynchronisedInputBrokerDataSourceTestHelper    ();

    virtual ~ MemoryMapSynchronisedInputBrokerDataSourceTestHelper();

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

    //Fail synchronisation?
    bool failSynchronisation;
};

MemoryMapSynchronisedInputBrokerDataSourceTestHelper::MemoryMapSynchronisedInputBrokerDataSourceTestHelper() :
        DataSourceI() {
    signalMemory = NULL_PTR(void *);
    offsets = NULL_PTR(uint32 *);
    samples = 10;
    failSynchronisation = false;
}

MemoryMapSynchronisedInputBrokerDataSourceTestHelper::~MemoryMapSynchronisedInputBrokerDataSourceTestHelper() {
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
bool MemoryMapSynchronisedInputBrokerDataSourceTestHelper::AllocateMemory() {
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

uint32 MemoryMapSynchronisedInputBrokerDataSourceTestHelper::GetNumberOfMemoryBuffers() {
    return 1u;
}

/**
 * @brief Returns the correct memory buffer and generates a pattern where the signal
 * address byte N contains the value N*N
 */
bool MemoryMapSynchronisedInputBrokerDataSourceTestHelper::GetSignalMemoryBuffer(const uint32 signalIdx,
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
        signalAddress = reinterpret_cast<void *&>(memPtr);
    }
    return ret;
}

const char8 * MemoryMapSynchronisedInputBrokerDataSourceTestHelper::GetBrokerName(StructuredDataI &data,
                                                                                  const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapSynchronisedInputBroker";
    }
    return "MemoryMapSynchronisedInputBroker";
}

bool MemoryMapSynchronisedInputBrokerDataSourceTestHelper::PrepareNextState(const char8 * const currentStateName,
                                                                            const char8 * const nextStateName) {
    return true;
}

bool MemoryMapSynchronisedInputBrokerDataSourceTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                                           const char8* const functionName,
                                                                           void * const gamMemPtr) {
    ReferenceT<MemoryMapSynchronisedInputBroker> broker("MemoryMapSynchronisedInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapSynchronisedInputBrokerDataSourceTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                                            const char8* const functionName,
                                                                            void * const gamMemPtr) {
    return true;
}

bool MemoryMapSynchronisedInputBrokerDataSourceTestHelper::Synchronise() {
    return !failSynchronisation;
}

CLASS_REGISTER(MemoryMapSynchronisedInputBrokerDataSourceTestHelper, "1.0");

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapSynchronisedInputBrokerEnviroment(const char8 * const config) {
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
 * MARTe configuration structure to test the MemoryMapSynchronisedInputBroker
 */
static const char8 * const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapSynchronisedInputBrokerTestGAM1"
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
        "            Class = MemoryMapSynchronisedInputBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = MemoryMapSynchronisedInputBrokerTestGAM1"
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
        "            Class = MemoryMapSynchronisedInputBrokerTestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = MemoryMapSynchronisedInputBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = MemoryMapSynchronisedInputBrokerTestGAM1"
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
        "            Class = MemoryMapSynchronisedInputBrokerDataSourceTestHelper"
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
        "        Class = MemoryMapSynchronisedInputBrokerTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool MemoryMapSynchronisedInputBrokerTest::TestConstructor() {
    ReferenceT<MemoryMapSynchronisedInputBroker> broker("MemoryMapSynchronisedInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->GetNumberOfCopies() == 0;
    }
    return ret;
}

bool MemoryMapSynchronisedInputBrokerTest::TestExecute() {
    bool ret = InitialiseMemoryMapSynchronisedInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapSynchronisedInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapSynchronisedInputBroker> broker;
    ReferenceT<MemoryMapSynchronisedInputBrokerTestGAM1> gamA;
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

bool MemoryMapSynchronisedInputBrokerTest::TestExecute_FailSynchronisation() {
    bool ret = InitialiseMemoryMapSynchronisedInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapSynchronisedInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapSynchronisedInputBroker> broker;
    ReferenceT<MemoryMapSynchronisedInputBrokerTestGAM1> gamA;
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
        dataSource->failSynchronisation = true;
        ret = !broker->Execute();
    }
    //Verify if the GAM has the expected pattern
    char8 *gamPtr = NULL_PTR(char8 *);
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(1));
    }
    uint32 copySize = broker->GetCopyByteSize(3);
    uint32 s;
    //After failing the values should not be updated...
    for (s = 0; (s < copySize) && (ret); s++) {
        ret = (*(gamPtr++) == 0);
    }

    return ret;
}

bool MemoryMapSynchronisedInputBrokerTest::TestExecute_Ranges() {
    bool ret = InitialiseMemoryMapSynchronisedInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapSynchronisedInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapSynchronisedInputBroker> broker;
    ReferenceT<MemoryMapSynchronisedInputBrokerTestGAM1> gamA;
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

bool MemoryMapSynchronisedInputBrokerTest::TestExecute_Samples() {
    bool ret = InitialiseMemoryMapSynchronisedInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapSynchronisedInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapSynchronisedInputBroker> broker;
    ReferenceT<MemoryMapSynchronisedInputBrokerTestGAM1> gamA;
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

