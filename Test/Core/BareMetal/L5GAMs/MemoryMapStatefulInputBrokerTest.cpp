/**
 * @file MemoryMapStatefulInputBrokerTest.cpp
 * @brief Source file for class MemoryMapStatefulInputBrokerTest
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
 * the class MemoryMapStatefulInputBrokerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "MemoryMapStatefulInputBroker.h"
#include "MemoryMapStatefulInputBrokerTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAMSchedulerI empty implementation to support the MemoryMapStatefulInputBroker tests
 */
class MemoryMapStatefulInputBrokerTestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapStatefulInputBrokerTestScheduler1    ();

    virtual void StartExecution();

    virtual void StopExecution();
};

MemoryMapStatefulInputBrokerTestScheduler1::MemoryMapStatefulInputBrokerTestScheduler1() :
        GAMSchedulerI() {

}

void MemoryMapStatefulInputBrokerTestScheduler1::StartExecution() {

}

void MemoryMapStatefulInputBrokerTestScheduler1::StopExecution() {

}

CLASS_REGISTER(MemoryMapStatefulInputBrokerTestScheduler1, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapStatefulInputBroker tests
 */
class MemoryMapStatefulInputBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapStatefulInputBrokerTestGAM1    ();

    void *GetInputSignalsMemory();

    void *GetOutputSignalsMemory();

    void *GetInputSignalMemory(uint32 signalIdx);

    void *GetOutputSignalMemory(uint32 signalIdx);

    virtual bool Execute();
};

MemoryMapStatefulInputBrokerTestGAM1::MemoryMapStatefulInputBrokerTestGAM1() :
        GAM() {
}

void *MemoryMapStatefulInputBrokerTestGAM1::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void *MemoryMapStatefulInputBrokerTestGAM1::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

void *MemoryMapStatefulInputBrokerTestGAM1::GetInputSignalMemory(uint32 signalIdx) {
    return GAM::GetInputSignalMemory(signalIdx);
}

void *MemoryMapStatefulInputBrokerTestGAM1::GetOutputSignalMemory(uint32 signalIdx) {
    return GAM::GetOutputSignalMemory(signalIdx);
}

bool MemoryMapStatefulInputBrokerTestGAM1::Execute() {
    return true;
}

CLASS_REGISTER(MemoryMapStatefulInputBrokerTestGAM1, "1.0")

/**
 * @brief Dual-buffer implementation of a DataSourceI.
 * @details the DataSourceI as two memory buffers (stored in signalMemoryBuffer) where
 *  the signals are stored.
 * The signalMemoryAddress array contains the address of each signal in the signalMemoryBuffer.
 * Finally the bufferedMemoryAddress[0] may contain the value of signalMemoryAddress[0] or the value
 * of signalMemoryAddress[1] and bufferedMemoryAddress[1] may contain the value of signalMemoryAddress[0] or the value
 * of signalMemoryAddress[1].
 * This allows to change where any of the two bufferedMemoryAddress is pointing to and thus by dereferencing bufferedMemoryAddress[]
 *  one gets the signalMemoryBuffer memory where the Broker will have to copy the data from/to.
 *
 *            signalMemoryBuffer[0]    | signalMemoryBuffer[1]
 *  signal0 | 0xAAA0                   | 0xBBB0                   //Real signal address
 *  signal1 | 0xAAA4                   | 0xBBB4
 *  signal2 | 0xAAAC                   | 0xBBBC
 *
 *            signalMemoryAddress[0]          | signalMemoryAddress[1]
 *  signal0 | 0xAAA0                          | 0xBBB0
 *  signal1 | 0xAAA4                          | 0xBBB4
 *  signal2 | 0xAAAC                          | 0xBBBC
 *
 *            bufferedMemoryAddress[0]        | bufferedMemoryAddress[1]
 *  signal0 | signalMemoryAddress[0][signal0] | signalMemoryAddress[1][signal0]
 *  signal1 | signalMemoryAddress[1][signal1] | signalMemoryAddress[0][signal1]
 *  signal2 | signalMemoryAddress[1][signal2] | signalMemoryAddress[0][signal2]
 *
 *  (*bufferedMemoryAddress[0][signal0] = 0xAAA0)
 *  (*bufferedMemoryAddress[0][signal1] = 0xBBB4)
 *  ...
 *  That is when the Broker accesses bufferedMemoryAddress[0/1] it will transparently get access to the memory as decided by the DataSource.
 *  One example is that the Source is updating the memory at signalMemoryBuffer[0] and wants for the state 0 that the memory is copied from
 *  the buffer with index 1.
 */
class MemoryMapStatefulInputBrokerDataSourceTestHelper: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapStatefulInputBrokerDataSourceTestHelper    ();

    virtual ~MemoryMapStatefulInputBrokerDataSourceTestHelper();

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

    void **bufferedMemoryAddress[2];
    void **signalMemoryAddress[2];
    void *signalMemoryBuffer[2];
    //Store 10 samples per signal
    uint32 samples;

};

MemoryMapStatefulInputBrokerDataSourceTestHelper::MemoryMapStatefulInputBrokerDataSourceTestHelper() :
        DataSourceI() {
    signalMemoryBuffer[0] = NULL_PTR(void *);
    signalMemoryBuffer[1] = NULL_PTR(void *);
    signalMemoryAddress[0] = NULL_PTR(void **);
    signalMemoryAddress[1] = NULL_PTR(void **);
    bufferedMemoryAddress[0] = NULL_PTR(void **);
    bufferedMemoryAddress[1] = NULL_PTR(void **);
    samples = 10;
}

MemoryMapStatefulInputBrokerDataSourceTestHelper::~MemoryMapStatefulInputBrokerDataSourceTestHelper() {
    if (signalMemoryAddress[0] != NULL_PTR(void **)) {
        delete[] signalMemoryAddress[0];
    }
    if (signalMemoryAddress[1] != NULL_PTR(void **)) {
        delete[] signalMemoryAddress[1];
    }
    if (bufferedMemoryAddress[0] != NULL_PTR(void **)) {
        delete[] bufferedMemoryAddress[0];
    }
    if (bufferedMemoryAddress[1] != NULL_PTR(void **)) {
        delete[] bufferedMemoryAddress[1];
    }
    if (signalMemoryBuffer[0] != NULL_PTR(void *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemoryBuffer[0]);
    }
    if (signalMemoryBuffer[1] != NULL_PTR(void *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemoryBuffer[1]);
    }
}

bool MemoryMapStatefulInputBrokerDataSourceTestHelper::AllocateMemory() {
    uint32 numberOfSignals = GetNumberOfSignals();
    bool ret = (numberOfSignals > 0u);
    if (ret) {
        ret = (signalMemoryAddress[0] == NULL_PTR(void *));
    }
    if (ret) {
        signalMemoryAddress[0] = new void*[numberOfSignals];
        signalMemoryAddress[1] = new void*[numberOfSignals];
        bufferedMemoryAddress[0] = new void*[numberOfSignals];
        bufferedMemoryAddress[1] = new void*[numberOfSignals];
    }

    uint32 totalMemorySize = 0u;
    for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
        uint32 memorySize;
        ret = GetSignalByteSize(s, memorySize);
        //Allocate memory to hold sufficient samples per signal
        if (ret) {
            totalMemorySize += memorySize * samples;
        }
    }
    if (ret) {
        signalMemoryBuffer[0] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalMemorySize);
        MemoryOperationsHelper::Set(signalMemoryBuffer[0], 0, totalMemorySize);
        signalMemoryBuffer[1] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalMemorySize);
        MemoryOperationsHelper::Set(signalMemoryBuffer[1], 0, totalMemorySize);
    }
    totalMemorySize = 0u;
    for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
        uint32 memorySize;
        ret = GetSignalByteSize(s, memorySize);
        if (ret) {
            char8 *ptr = reinterpret_cast<char8 *>(signalMemoryBuffer[0]);
            ptr += totalMemorySize;
            signalMemoryAddress[0][s] = reinterpret_cast<void *>(ptr);
            bufferedMemoryAddress[0][s] = signalMemoryAddress[0][s];

            ptr = reinterpret_cast<char8 *>(signalMemoryBuffer[1]);
            ptr += totalMemorySize;
            signalMemoryAddress[1][s] = reinterpret_cast<void *>(ptr);
            bufferedMemoryAddress[1][s] = signalMemoryAddress[1][s];

            totalMemorySize += memorySize * samples;
        }
    }
    return ret;
}

uint32 MemoryMapStatefulInputBrokerDataSourceTestHelper::GetNumberOfMemoryBuffers() {
    return 2u;
}

/**
 * Generate a memory pattern that can be checked later.
 */
bool MemoryMapStatefulInputBrokerDataSourceTestHelper::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                                             const uint32 bufferIdx,
                                                                             void *&signalAddress) {
    char8 *memPtr = reinterpret_cast<char8 *>(signalMemoryAddress[bufferIdx][signalIdx]);
    uint32 byteSize;
    bool ret = GetSignalByteSize(signalIdx, byteSize);
    uint32 n;
    for (n = 0; n < byteSize * samples; n++) {
        if (bufferIdx == 0) {
            memPtr[n] = static_cast<char8>(n * n);
        }
        else {
            memPtr[n] = static_cast<char8>(n + n);
        }
    }

    signalAddress = &bufferedMemoryAddress[bufferIdx][signalIdx];
    return ret;
}

const char8 *MemoryMapStatefulInputBrokerDataSourceTestHelper::GetBrokerName(StructuredDataI &data,
                                                                             const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapStatefulInputBroker";
    }
    return "MemoryMapStatefulOutputBroker";
}

bool MemoryMapStatefulInputBrokerDataSourceTestHelper::PrepareNextState(const char8 * const currentStateName,
                                                                        const char8 * const nextStateName) {
    //All the odd signals of Buffer 1 will know be pointing at the memory[2]
    //All the even signals of Buffer 2 will know be pointing at the memory[1]
    uint32 s = 0;
    for (s = 0; s < GetNumberOfSignals(); s++) {
        if ((s % 2) != 0) {
            bufferedMemoryAddress[1][s] = signalMemoryAddress[0][s];
        }
        else {
            bufferedMemoryAddress[0][s] = signalMemoryAddress[1][s];
        }

    }
    return true;
}

bool MemoryMapStatefulInputBrokerDataSourceTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                                       const char8* const functionName,
                                                                       void * const gamMemPtr) {
    ReferenceT<MemoryMapStatefulInputBroker> broker("MemoryMapStatefulInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapStatefulInputBrokerDataSourceTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                                        const char8* const functionName,
                                                                        void * const gamMemPtr) {
    return true;
}

bool MemoryMapStatefulInputBrokerDataSourceTestHelper::Synchronise() {
    return false;
}

CLASS_REGISTER(MemoryMapStatefulInputBrokerDataSourceTestHelper, "1.0");

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapStatefulInputBrokerEnviroment(const char8 * const config) {
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
 * MARTe configuration structure to test the MemoryMapStatefulInputBroker
 */
static const char8 * const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapStatefulInputBrokerTestGAM1"
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
        "            Class = MemoryMapStatefulInputBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = MemoryMapStatefulInputBrokerTestGAM1"
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
        "            Class = MemoryMapStatefulInputBrokerTestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = MemoryMapStatefulInputBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = MemoryMapStatefulInputBrokerTestGAM1"
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
        "            Class = MemoryMapStatefulInputBrokerDataSourceTestHelper"
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
        "        +Times = {"
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
        "        Class = MemoryMapStatefulInputBrokerTestScheduler1"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool MemoryMapStatefulInputBrokerTest::TestConstructor() {
    ReferenceT<MemoryMapStatefulInputBroker> broker("MemoryMapStatefulInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->GetNumberOfCopies() == 0;
    }
    return ret;
}

bool MemoryMapStatefulInputBrokerTest::TestExecute_0() {
    bool ret = InitialiseMemoryMapStatefulInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulInputBroker> broker;
    ReferenceT<MemoryMapStatefulInputBrokerTestGAM1> gamA;
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

    RealTimeApplication::index = 0u;
    uint32 numberOfCopies;
    if (ret) {
        numberOfCopies = broker->GetNumberOfCopies();
        ret = (numberOfCopies == 5u);
    }
    if (ret) {
        ret = broker->Execute();
    }
    //Verify if the GAM has the expected pattern
    char8 *gamPtr;
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(1));
    }
    uint32 copySize;
    if (ret) {
        copySize = broker->GetCopyByteSize(3);
    }
    uint32 s;
    for (s = 0; (s < copySize) && (ret); s++) {
        ret = (*(gamPtr++) == static_cast<char8>(s * s));
    }

    return ret;
}

bool MemoryMapStatefulInputBrokerTest::TestExecute_1() {
    bool ret = InitialiseMemoryMapStatefulInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulInputBroker> broker;
    ReferenceT<MemoryMapStatefulInputBrokerTestGAM1> gamA;
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

    RealTimeApplication::index = 1u;
    uint32 numberOfCopies;
    if (ret) {
        numberOfCopies = broker->GetNumberOfCopies();
        ret = (numberOfCopies == 5u);
    }
    if (ret) {
        ret = broker->Execute();
    }
    //Verify if the GAM has the expected pattern
    char8 *gamPtr;
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(1));
    }
    uint32 copySize;
    if (ret) {
        copySize = broker->GetCopyByteSize(3);
    }
    uint32 s;
    for (s = 0; (s < copySize) && (ret); s++) {
        ret = (*(gamPtr++) == static_cast<char8>(s + s));
    }

    return ret;
}

bool MemoryMapStatefulInputBrokerTest::TestExecute_ChangeState() {
    bool ret = InitialiseMemoryMapStatefulInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulInputBroker> broker;
    ReferenceT<MemoryMapStatefulInputBrokerTestGAM1> gamA;
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

    if (ret) {
        ret = dataSource->PrepareNextState("", "");
    }

    RealTimeApplication::index = 0u;
    uint32 numberOfCopies;
    if (ret) {
        numberOfCopies = broker->GetNumberOfCopies();
        ret = (numberOfCopies == 5u);
    }
    if (ret) {
        ret = broker->Execute();
    }
    //Verify if the GAM has the expected pattern
    char8 *gamPtr;
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(1));
    }
    uint32 copySize;
    if (ret) {
        copySize = broker->GetCopyByteSize(3);
    }
    uint32 signalIdx;
    //Signal1A
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal1A");
    }
    uint32 s;
    for (s = 0; (s < copySize) && (ret); s++) {
        if ((signalIdx % 2) == 0) {
            ret = (*(gamPtr++) == static_cast<char8>(s + s));
        }
        else {
            ret = (*(gamPtr++) == static_cast<char8>(s * s));
        }
    }

    return ret;
}

bool MemoryMapStatefulInputBrokerTest::TestExecute_Ranges_0() {
    bool ret = InitialiseMemoryMapStatefulInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulInputBroker> broker;
    ReferenceT<MemoryMapStatefulInputBrokerTestGAM1> gamA;
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
    RealTimeApplication::index = 0u;
    if (ret) {
        ret = broker->Execute();
    }
    //Verify if the GAM has the expected pattern
    char8 *gamPtr;
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

bool MemoryMapStatefulInputBrokerTest::TestExecute_Ranges_1() {
    bool ret = InitialiseMemoryMapStatefulInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulInputBroker> broker;
    ReferenceT<MemoryMapStatefulInputBrokerTestGAM1> gamA;
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
    RealTimeApplication::index = 1u;
    if (ret) {
        ret = broker->Execute();
    }
    //Verify if the GAM has the expected pattern
    char8 *gamPtr;
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(0));
    }

    uint32 copySize = 0;
    uint32 s;
    copySize = broker->GetCopyByteSize(0);
    for (s = 0; (s < copySize) && (ret); s++) {
        ret = (*(gamPtr++) == static_cast<char8>(s + s));
    }
    copySize = broker->GetCopyByteSize(1);
    for (s = 0; (s < copySize) && (ret); s++) {
        //In the DataSource memory there are other ranges in the middle
        ret = (*(gamPtr++) == static_cast<char8>((s + 2 * 4) + (s + 2 * 4)));
    }
    copySize = broker->GetCopyByteSize(2);
    for (s = 0; (s < copySize) && (ret); s++) {
        //In the DataSource memory there are other ranges in the middle
        ret = (*(gamPtr++) == static_cast<char8>((s + 9 * 4) + (s + 9 * 4)));
    }

    return ret;
}

bool MemoryMapStatefulInputBrokerTest::TestExecute_Ranges_ChangeState() {
    bool ret = InitialiseMemoryMapStatefulInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulInputBroker> broker;
    ReferenceT<MemoryMapStatefulInputBrokerTestGAM1> gamA;
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
        ret = dataSource->PrepareNextState("", "");
    }
    RealTimeApplication::index = 0u;
    if (ret) {
        ret = broker->Execute();
    }
    //Verify if the GAM has the expected pattern
    char8 *gamPtr;
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(0));
    }

    uint32 copySize = broker->GetCopyByteSize(0);
    uint32 s;
    uint32 signalIdx;
    //Signal1A
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal4A");
    }

    if ((signalIdx % 2) == 0) {
        for (s = 0; (s < copySize) && (ret); s++) {
            ret = (*(gamPtr++) == static_cast<char8>(s + s));
        }
        copySize = broker->GetCopyByteSize(1);
        for (s = 0; (s < copySize) && (ret); s++) {
            //In the DataSource memory there are other ranges in the middle
            ret = (*(gamPtr++) == static_cast<char8>((s + 2 * 4) + (s + 2 * 4)));
        }
        copySize = broker->GetCopyByteSize(2);
        for (s = 0; (s < copySize) && (ret); s++) {
            //In the DataSource memory there are other ranges in the middle
            ret = (*(gamPtr++) == static_cast<char8>((s + 9 * 4) + (s + 9 * 4)));
        }
    }
    else {
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
    }
    return ret;
}

bool MemoryMapStatefulInputBrokerTest::TestExecute_Samples_0() {
    bool ret = InitialiseMemoryMapStatefulInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulInputBroker> broker;
    ReferenceT<MemoryMapStatefulInputBrokerTestGAM1> gamA;
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
    RealTimeApplication::index = 0u;
    if (ret) {
        ret = broker->Execute();
    }
    uint32 s;
    char8 *gamPtr;
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

bool MemoryMapStatefulInputBrokerTest::TestExecute_Samples_1() {
    bool ret = InitialiseMemoryMapStatefulInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulInputBroker> broker;
    ReferenceT<MemoryMapStatefulInputBrokerTestGAM1> gamA;
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
    RealTimeApplication::index = 1u;
    if (ret) {
        ret = broker->Execute();
    }
    uint32 s;
    char8 *gamPtr;
    //Skip the signal that belongs to the other DataSource...
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(3));
    }
    uint32 copySize = broker->GetCopyByteSize(4);
    for (s = 0; (s < copySize) && (ret); s++) {
        ret = (*(gamPtr++) == static_cast<char8>(s + s));
    }

    return ret;
}

bool MemoryMapStatefulInputBrokerTest::TestExecute_Samples_ChangeState() {
    bool ret = InitialiseMemoryMapStatefulInputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulInputBroker> broker;
    ReferenceT<MemoryMapStatefulInputBrokerTestGAM1> gamA;
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
        ret = dataSource->PrepareNextState("", "");
    }
    RealTimeApplication::index = 0u;
    if (ret) {
        ret = broker->Execute();
    }
    uint32 s;
    char8 *gamPtr;
    //Skip the signal that belongs to the other DataSource...
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(3));
    }
    uint32 signalIdx;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal5");
    }
    uint32 copySize = broker->GetCopyByteSize(4);
    for (s = 0; (s < copySize) && (ret); s++) {
        if ((signalIdx % 2) == 0) {
            ret = (*(gamPtr++) == static_cast<char8>(s + s));
        }
        else {
            ret = (*(gamPtr++) == static_cast<char8>(s * s));
        }
    }

    return ret;
}

