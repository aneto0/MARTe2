/**
 * @file MemoryMapStatefulOutputBrokerTest.cpp
 * @brief Source file for class MemoryMapStatefulOutputBrokerTest
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
 * the class MemoryMapStatefulOutputBrokerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"
#include "MemoryMapStatefulOutputBrokerTest.h"
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
 * @brief GAMSchedulerI empty implementation to support the MemoryMapStatefulOutputBroker tests
 */
class MemoryMapStatefulOutputBrokerTestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapStatefulOutputBrokerTestScheduler1    ();

    virtual void StartExecution();

    virtual void StopExecution();
};

MemoryMapStatefulOutputBrokerTestScheduler1::MemoryMapStatefulOutputBrokerTestScheduler1() :
        GAMSchedulerI() {

}

void MemoryMapStatefulOutputBrokerTestScheduler1::StartExecution() {

}

void MemoryMapStatefulOutputBrokerTestScheduler1::StopExecution() {

}

CLASS_REGISTER(MemoryMapStatefulOutputBrokerTestScheduler1, "1.0")

/**
 * @brief GAM implementation to support the MemoryMapStatefulOutputBroker tests.
 * @details This GAM generates a memory pattern (see Execute) which allows to verify if the
 *  MemoryMapStatefulOutputBroker is correctly copying the signal into the DataSource.
 */
class MemoryMapStatefulOutputBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapStatefulOutputBrokerTestGAM1    ();

    void *GetInputSignalsMemory();

    void *GetOutputSignalsMemory();

    void *GetInputSignalMemory(uint32 signalIdx);

    void *GetOutputSignalMemory(uint32 signalIdx);

    virtual bool Execute();
};

MemoryMapStatefulOutputBrokerTestGAM1::MemoryMapStatefulOutputBrokerTestGAM1() :
        GAM() {
}

void *MemoryMapStatefulOutputBrokerTestGAM1::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void *MemoryMapStatefulOutputBrokerTestGAM1::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

void *MemoryMapStatefulOutputBrokerTestGAM1::GetInputSignalMemory(uint32 signalIdx) {
    return GAM::GetInputSignalMemory(signalIdx);
}

void *MemoryMapStatefulOutputBrokerTestGAM1::GetOutputSignalMemory(uint32 signalIdx) {
    return GAM::GetOutputSignalMemory(signalIdx);
}

/**
 * @brief For each signal byte N, generates a pattern given by N*N
 */
bool MemoryMapStatefulOutputBrokerTestGAM1::Execute() {
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

CLASS_REGISTER(MemoryMapStatefulOutputBrokerTestGAM1, "1.0")

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
class MemoryMapStatefulOutputBrokerDataSourceTestHelper: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapStatefulOutputBrokerDataSourceTestHelper    ();

    virtual ~MemoryMapStatefulOutputBrokerDataSourceTestHelper();

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateNames);

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

MemoryMapStatefulOutputBrokerDataSourceTestHelper::MemoryMapStatefulOutputBrokerDataSourceTestHelper() :
        DataSourceI() {
    signalMemoryBuffer[0] = NULL_PTR(void *);
    signalMemoryBuffer[1] = NULL_PTR(void *);
    signalMemoryAddress[0] = NULL_PTR(void **);
    signalMemoryAddress[1] = NULL_PTR(void **);
    bufferedMemoryAddress[0] = NULL_PTR(void **);
    bufferedMemoryAddress[1] = NULL_PTR(void **);
    samples = 10;
}

MemoryMapStatefulOutputBrokerDataSourceTestHelper::~MemoryMapStatefulOutputBrokerDataSourceTestHelper() {
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

bool MemoryMapStatefulOutputBrokerDataSourceTestHelper::AllocateMemory() {
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

uint32 MemoryMapStatefulOutputBrokerDataSourceTestHelper::GetNumberOfMemoryBuffers() {
    return 2u;
}

bool MemoryMapStatefulOutputBrokerDataSourceTestHelper::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                                              const uint32 bufferIdx,
                                                                              void *&signalAddress) {
    signalAddress = &bufferedMemoryAddress[bufferIdx][signalIdx];
    return true;
}

const char8 *MemoryMapStatefulOutputBrokerDataSourceTestHelper::GetBrokerName(StructuredDataI &data,
                                                                              const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapStatefulInputBroker";
    }
    return "MemoryMapStatefulOutputBroker";
}

bool MemoryMapStatefulOutputBrokerDataSourceTestHelper::PrepareNextState(const char8 * const currentStateName,
                                                                         const char8 * const nextStateName) {
    //All the odd signals of Buffer 1 will know be pointing at the memory[2]
    //All the even signals of Buffer 2 will know be pointing at the memory[1]
    uint32 s = 0;
    for (s = 0; s < GetNumberOfSignals(); s++) {
        if ((s % 2) == 0) {
            bufferedMemoryAddress[0][s] = signalMemoryAddress[1][s];
        }
        else {
            bufferedMemoryAddress[1][s] = signalMemoryAddress[0][s];
        }

    }
    return true;
}

bool MemoryMapStatefulOutputBrokerDataSourceTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                                        const char8* const functionName,
                                                                        void * const gamMemPtr) {

    return true;
}

bool MemoryMapStatefulOutputBrokerDataSourceTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                                         const char8* const functionName,
                                                                         void * const gamMemPtr) {
    ReferenceT<MemoryMapStatefulOutputBroker> broker("MemoryMapStatefulOutputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapStatefulOutputBrokerDataSourceTestHelper::Synchronise() {
    return false;
}

CLASS_REGISTER(MemoryMapStatefulOutputBrokerDataSourceTestHelper, "1.0");

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapStatefulOutputBrokerEnviroment(const char8 * const config) {
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
 * MARTe configuration structure to test the MemoryMapStatefulOutputBroker
 */
static const char8 * const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapStatefulOutputBrokerTestGAM1"
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
        "            Class = MemoryMapStatefulOutputBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = MemoryMapStatefulOutputBrokerTestGAM1"
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
        "            Class = MemoryMapStatefulOutputBrokerTestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = MemoryMapStatefulOutputBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = MemoryMapStatefulOutputBrokerTestGAM1"
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
        "            Class = MemoryMapStatefulOutputBrokerDataSourceTestHelper"
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
        "        Class = MemoryMapStatefulOutputBrokerTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool MemoryMapStatefulOutputBrokerTest::TestConstructor() {
    ReferenceT<MemoryMapStatefulOutputBroker> broker("MemoryMapStatefulOutputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->GetNumberOfCopies() == 0;
    }
    return ret;
}

bool MemoryMapStatefulOutputBrokerTest::TestExecute_0() {
    bool ret = InitialiseMemoryMapStatefulOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulOutputBroker> broker;
    ReferenceT<MemoryMapStatefulOutputBrokerTestGAM1> gamC;
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
    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }
    //RealTimeApplication::index = 0u;
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
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    uint32 byteSize;
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }
    uint32 s;
    for (s = 0; (s < byteSize) && (ret); s++) {
        ret = (*(dsPtr++) == static_cast<char8>(s * s));
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }
    for (s = 0; (s < byteSize) && (ret); s++) {
        ret = (*(dsPtr++) == 0);
    }

    return ret;
}

bool MemoryMapStatefulOutputBrokerTest::TestExecute_1() {
    bool ret = InitialiseMemoryMapStatefulOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulOutputBroker> broker;
    ReferenceT<MemoryMapStatefulOutputBrokerTestGAM1> gamC;
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
    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }
    if (ret) {
        app->StartExecution();
    }
    //RealTimeApplication::index = 1u;
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
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    uint32 byteSize;
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }
    uint32 s;
    for (s = 0; (s < byteSize) && (ret); s++) {
        ret = (*(dsPtr++) == 0);
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }
    for (s = 0; (s < byteSize) && (ret); s++) {
        ret = (*(dsPtr++) == static_cast<char8>(s * s));
    }

    return ret;
}

bool MemoryMapStatefulOutputBrokerTest::TestExecute_ChangeState() {
    bool ret = InitialiseMemoryMapStatefulOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulOutputBroker> broker;
    ReferenceT<MemoryMapStatefulOutputBrokerTestGAM1> gamC;
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
    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }
    if (ret) {
        app->StartExecution();
    }
    //RealTimeApplication::index = 0u;

    if (ret) {
        ret = gamC->Execute();
    }
    if (ret) {
        ret = broker->Execute();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("", "");
    }
    uint32 signalIdx;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal1A");
    }

    char8 *dsPtr;
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    uint32 byteSize;
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }
    uint32 s;
    for (s = 0; (s < byteSize) && (ret); s++) {
        if ((signalIdx % 2 == 0)) {
            ret = (*(dsPtr++) == 0);
        }
        else {
            ret = (*(dsPtr++) == static_cast<char8>(s * s));
        }
    }

    return ret;
}

bool MemoryMapStatefulOutputBrokerTest::TestExecute_Ranges_0() {
    bool ret = InitialiseMemoryMapStatefulOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulOutputBroker> broker;
    ReferenceT<MemoryMapStatefulOutputBrokerTestGAM1> gamC;
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

    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }
    //RealTimeApplication::index = 0u;
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
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
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
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    //First range
    for (s = 0; (s < 4) && (ret); s++) {
        ret = (*(dsPtr++) == 0);
    }
    //Second range
    dsPtr += 4;
    for (s = 4; (s < 20) && (ret); s++) {
        ret = (*(dsPtr++) == 0);
    }
    //Third range
    dsPtr += 12;
    for (s = 20; (s < 24) && (ret); s++) {
        ret = (*(dsPtr++) == 0);
    }

    return ret;
}

bool MemoryMapStatefulOutputBrokerTest::TestExecute_Ranges_1() {
    bool ret = InitialiseMemoryMapStatefulOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulOutputBroker> broker;
    ReferenceT<MemoryMapStatefulOutputBrokerTestGAM1> gamC;
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
    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }
    if (ret) {
        app->StartExecution();
    }
    //RealTimeApplication::index = 1u;
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
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }
    //First range
    for (s = 0; (s < 4) && (ret); s++) {
        ret = (*(dsPtr++) == 0);
    }
    //Second range
    dsPtr += 4;
    for (s = 4; (s < 20) && (ret); s++) {
        ret = (*(dsPtr++) == 0);
    }
    //Third range
    dsPtr += 12;
    for (s = 20; (s < 24) && (ret); s++) {
        ret = (*(dsPtr++) == 0);
    }

    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
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

bool MemoryMapStatefulOutputBrokerTest::TestExecute_Ranges_ChangeState() {
    bool ret = InitialiseMemoryMapStatefulOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulOutputBroker> broker;
    ReferenceT<MemoryMapStatefulOutputBrokerTestGAM1> gamC;
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
    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }
    if (ret) {
        app->StartExecution();
    }
    //RealTimeApplication::index = 0u;

    if (ret) {
        ret = gamC->Execute();
    }
    if (ret) {
        ret = broker->Execute();
    }

    //Swap the memory for the odd buffers
    if (ret) {
        ret = dataSource->PrepareNextState("", "");
    }
    uint32 signalIdx;
    char8 *dsPtr;
    uint32 byteSize;
    uint32 s;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal4A");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }

    //First range
    for (s = 0; (s < 4) && (ret); s++) {
        if ((signalIdx % 2 == 0)) {
            ret = (*(dsPtr++) == 0);
        }
        else {
            ret = (*(dsPtr++) == static_cast<char8>(s * s));
        }
    }
    //Second range
    dsPtr += 4;
    for (s = 4; (s < 20) && (ret); s++) {
        if ((signalIdx % 2 == 0)) {
            ret = (*(dsPtr++) == 0);
        }
        else {
            ret = (*(dsPtr++) == static_cast<char8>(s * s));
        }
    }
    //Third range
    dsPtr += 12;
    for (s = 20; (s < 24) && (ret); s++) {
        if ((signalIdx % 2 == 0)) {
            ret = (*(dsPtr++) == 0);
        }
        else {
            ret = (*(dsPtr++) == static_cast<char8>(s * s));
        }
    }
    return ret;
}

bool MemoryMapStatefulOutputBrokerTest::TestExecute_Samples_0() {
    bool ret = InitialiseMemoryMapStatefulOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulOutputBroker> broker;
    ReferenceT<MemoryMapStatefulOutputBrokerTestGAM1> gamC;
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
    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }
    //RealTimeApplication::index = 0u;
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
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }

    for (s = 0; (s < 12) && (ret); s++) {
        ret = (*(dsPtr++) == static_cast<char8>(s * s));
    }

    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    for (s = 0; (s < 12) && (ret); s++) {
        ret = (*(dsPtr++) == 0);
    }
    return ret;
}

bool MemoryMapStatefulOutputBrokerTest::TestExecute_Samples_1() {
    bool ret = InitialiseMemoryMapStatefulOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulOutputBroker> broker;
    ReferenceT<MemoryMapStatefulOutputBrokerTestGAM1> gamC;
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
    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }
    if (ret) {
        app->StartExecution();
    }
    //RealTimeApplication::index = 1u;
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
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }
    for (s = 0; (s < 12) && (ret); s++) {
        ret = (*(dsPtr++) == 0);
    }

    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }

    for (s = 0; (s < 12) && (ret); s++) {
        ret = (*(dsPtr++) == static_cast<char8>(s * s));
    }
    return ret;
}

bool MemoryMapStatefulOutputBrokerTest::TestExecute_Samples_ChangeState() {
    bool ret = InitialiseMemoryMapStatefulOutputBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulOutputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulOutputBroker> broker;
    ReferenceT<MemoryMapStatefulOutputBrokerTestGAM1> gamC;
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
    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }
    if (ret) {
        app->StartExecution();
    }
    //RealTimeApplication::index = 0u;
    if (ret) {
        ret = gamC->Execute();
    }
    if (ret) {
        ret = broker->Execute();
    }
    //Swap the memory for the odd buffers
    if (ret) {
        ret = dataSource->PrepareNextState("", "");
    }
    uint32 signalIdx;
    char8 *dsPtr;
    uint32 byteSize;
    uint32 s;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal5");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dsPtr = reinterpret_cast<char8 *>(*reinterpret_cast<void **>(getPtr));
        }
    }
    if (ret) {
        ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
    }
    for (s = 0; (s < 12) && (ret); s++) {
        if ((signalIdx % 2 == 0)) {
            ret = (*(dsPtr++) == 0);
        }
        else {
            ret = (*(dsPtr++) == static_cast<char8>(s * s));
        }
    }

    return ret;
}

