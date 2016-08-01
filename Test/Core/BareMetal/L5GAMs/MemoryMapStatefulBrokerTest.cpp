/**
 * @file MemoryMapStatefulBrokerTest.cpp
 * @brief Source file for class MemoryMapStatefulBrokerTest
 * @date 27/07/2016
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
 * the class MemoryMapStatefulBrokerTest (public, protected, and private). Be aware that some
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
#include "MemoryMapStatefulBrokerTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAMSchedulerI empty implementation to support the MemoryMapStatefulBroker tests
 */
class MemoryMapStatefulBrokerTestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapStatefulBrokerTestScheduler1    ();

    virtual void StartExecution(const uint32 activeBuffer);

    virtual void StopExecution();
};

MemoryMapStatefulBrokerTestScheduler1::MemoryMapStatefulBrokerTestScheduler1() :
        GAMSchedulerI() {

}

void MemoryMapStatefulBrokerTestScheduler1::StartExecution(const uint32 activeBuffer) {

}

void MemoryMapStatefulBrokerTestScheduler1::StopExecution() {

}

CLASS_REGISTER(MemoryMapStatefulBrokerTestScheduler1, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapStatefulBroker tests.
 * Allows access to the protected methods of GAM (so that these can be tested).
 */
class MemoryMapStatefulBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapStatefulBrokerTestGAM1    ();

    virtual bool Execute();

    void *GetInputSignalsMemory();

    void *GetOutputSignalsMemory();

    void *GetInputSignalMemory(uint32 signalIdx);

    void *GetOutputSignalMemory(uint32 signalIdx);
};

MemoryMapStatefulBrokerTestGAM1::MemoryMapStatefulBrokerTestGAM1() :
        GAM() {
}

void *MemoryMapStatefulBrokerTestGAM1::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void *MemoryMapStatefulBrokerTestGAM1::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

void *MemoryMapStatefulBrokerTestGAM1::GetInputSignalMemory(uint32 signalIdx) {
    return GAM::GetInputSignalMemory(signalIdx);
}

void *MemoryMapStatefulBrokerTestGAM1::GetOutputSignalMemory(uint32 signalIdx) {
    return GAM::GetOutputSignalMemory(signalIdx);
}

bool MemoryMapStatefulBrokerTestGAM1::Execute() {
    return true;
}
CLASS_REGISTER(MemoryMapStatefulBrokerTestGAM1, "1.0")

/**
 * @brief The MemoryMapStatefulBroker is an abstract class. This implementation
 * allows to test if the Init() function is correctly performed.
 */
class MemoryMapStatefulBrokerTestHelper: public MemoryMapStatefulBroker {
public:
    CLASS_REGISTER_DECLARATION()

virtual    bool Execute();

};

/**
 * @brief Writes a known pattern to the memory so that the tests can check
 * if the Init() function behaves as expected.
 * @details For the dataSourcePointer[0] the DataSource memory data is set as the sum of the
 * current copy index and the current signal index.
 * For the dataSourcePointer[1] the DataSource memory data is set as the multiplication of the
 * current copy index and the current signal index + 1.
 */
bool MemoryMapStatefulBrokerTestHelper::Execute() {
    uint32 numberOfCopies = GetNumberOfCopies();
    uint32 n;
    uint32 s;
    for (n = 0; (n < numberOfCopies); n++) {
        void *dataSourceSignalPointer = *(copyTable[n].dataSourcePointer[0]);
        char8 *dsPtr = reinterpret_cast<char8 *>(dataSourceSignalPointer);
        char8 *gamPtr = reinterpret_cast<char8 *>(copyTable[n].gamPointer);
        for (s = 0; s < copyTable[n].copySize; s++) {
            dsPtr[s + copyTable[n].dataSourceOffset] = static_cast<char8>(n + s);
            gamPtr[s] = static_cast<char8>(n + s);
        }

        dataSourceSignalPointer = *(copyTable[n].dataSourcePointer[1]);
        dsPtr = reinterpret_cast<char8 *>(dataSourceSignalPointer);
        for (s = 0; s < copyTable[n].copySize; s++) {
            dsPtr[s + copyTable[n].dataSourceOffset] = static_cast<char8>((n + 1) * s);
        }
    }
    return true;
}

CLASS_REGISTER(MemoryMapStatefulBrokerTestHelper, "1.0");

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
class MemoryMapStatefulBrokerDataSourceTestHelper: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapStatefulBrokerDataSourceTestHelper    ();

    virtual ~MemoryMapStatefulBrokerDataSourceTestHelper();

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(uint32 signalIdx,
            uint32 bufferIdx,
            void *&signalAddress);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            SignalDirection direction);

    virtual bool PrepareNextState(const RealTimeStateInfo &status);

    virtual bool GetInputBrokers(
            ReferenceContainer &inputBrokers,
            const char8* functionName,
            void * gamMemPtr);

    virtual bool GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const char8* functionName,
            void * gamMemPtr);

    void **bufferedMemoryAddress[2];
    void **signalMemoryAddress[2];
    void *signalMemoryBuffer[2];
    //Store 10 samples per signal
    uint32 samples;

};

MemoryMapStatefulBrokerDataSourceTestHelper::MemoryMapStatefulBrokerDataSourceTestHelper() :
        DataSourceI() {
    signalMemoryBuffer[0] = NULL_PTR(void *);
    signalMemoryBuffer[1] = NULL_PTR(void *);
    signalMemoryAddress[0] = NULL_PTR(void **);
    signalMemoryAddress[1] = NULL_PTR(void **);
    bufferedMemoryAddress[0] = NULL_PTR(void **);
    bufferedMemoryAddress[1] = NULL_PTR(void **);
    samples = 10;
}

MemoryMapStatefulBrokerDataSourceTestHelper::~MemoryMapStatefulBrokerDataSourceTestHelper() {
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

bool MemoryMapStatefulBrokerDataSourceTestHelper::AllocateMemory() {
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

uint32 MemoryMapStatefulBrokerDataSourceTestHelper::GetNumberOfMemoryBuffers() {
    return 2u;
}

bool MemoryMapStatefulBrokerDataSourceTestHelper::GetSignalMemoryBuffer(uint32 signalIdx,
                                                                        uint32 bufferIdx,
                                                                        void *&signalAddress) {
    signalAddress = &bufferedMemoryAddress[bufferIdx][signalIdx];
    return true;
}

const char8 *MemoryMapStatefulBrokerDataSourceTestHelper::GetBrokerName(StructuredDataI &data,
                                                                        SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapStatefulBrokerTestHelper";
    }
    return "MemoryMapStatefulBrokerTestHelper";
}

bool MemoryMapStatefulBrokerDataSourceTestHelper::PrepareNextState(const RealTimeStateInfo &status) {
    //All the odd signals of Buffer 1 will know be pointing at the memory[2]
    //All the even signals of Buffer 2 will know be pointing at the memory[1]
    uint32 s = 0;
    for (s = 0; s < numberOfSignals; s++) {
        if ((s % 2) != 0) {
            bufferedMemoryAddress[1][s] = signalMemoryAddress[0][s];
        }
        else {
            bufferedMemoryAddress[0][s] = signalMemoryAddress[1][s];
        }

    }
    return true;
}

bool MemoryMapStatefulBrokerDataSourceTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                                  const char8* functionName,
                                                                  void * gamMemPtr) {
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker("MemoryMapStatefulBrokerTestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapStatefulBrokerDataSourceTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                                   const char8* functionName,
                                                                   void * gamMemPtr) {
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker("MemoryMapStatefulBrokerTestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}
CLASS_REGISTER(MemoryMapStatefulBrokerDataSourceTestHelper, "1.0");

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapStatefulBrokerEnviroment(const char8 * const config) {
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
 * MARTe configuration structure to test the MemoryMapStatefulBroker
 */
static const char8 * const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapStatefulBrokerTestGAM1"
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
        "            Class = MemoryMapStatefulBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = MemoryMapStatefulBrokerTestGAM1"
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
        "            Class = MemoryMapStatefulBrokerTestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = MemoryMapStatefulBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = MemoryMapStatefulBrokerTestGAM1"
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
        "            Class = MemoryMapStatefulBrokerDataSourceTestHelper"
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
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMC :Functions.GAMA :Functions.GAMF}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMD :Functions.GAMB :Functions.GAME :Functions.GAMF}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapStatefulBrokerTestScheduler1"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool MemoryMapStatefulBrokerTest::TestConstructor() {
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker("MemoryMapStatefulBrokerTestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->GetNumberOfCopies() == 0;
    }
    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Input_0() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamA;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }

    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    char8 *gamPtr;
    uint32 copySize = 0;
    uint32 s;
    uint32 n;
    uint32 signalIdx;
    //Signal1A
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal1A");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }
    if (ret) {
        //4th signal of the GAM. The 3rd belongs to another DataSource (and thus to another Broker)
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(3));
    }
    n = 4;
    copySize = broker->GetCopyByteSize(n);
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (dataSourcePtr[s] == static_cast<char8>(n + s));
    }
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (gamPtr[s] == static_cast<char8>(n + s));
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Input_1() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamA;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }

    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    uint32 copySize = 0;
    uint32 s;
    uint32 n;
    uint32 signalIdx;
    //Signal1A
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal1A");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }

    n = 4;
    copySize = broker->GetCopyByteSize(n);
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (dataSourcePtr[s] == static_cast<char8>((n + 1) * s));
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Input_Ranges_0() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamA;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }

    //Check for signal 4 (has ranges)
    uint32 signalIdx;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal4A");
    }

    //The dual buffer implementation has a pointer to the variable containing the pointer. This way the data source
    //can change in real-time this variable value and we will be copying/writing to another buffer.
    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    char8 *gamPtr;
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(0));
    }

    uint32 copySize = 0;
    uint32 s;
    uint32 n;
    for (n = 0u; (n < 3) && (ret); n++) {
        uint32 startOffset = 0;
        uint32 byteOffset = 0;
        uint32 functionIdx;
        uint32 functionSignalIdx;
        if (ret) {
            ret = dataSource->GetFunctionIndex(functionIdx, "GAMA");
        }
        if (ret) {
            ret = dataSource->GetFunctionSignalIndex(InputSignals, functionIdx, functionSignalIdx, "Signal4");
        }
        if (ret) {
            ret = dataSource->GetFunctionSignalByteOffsetInfo(InputSignals, functionIdx, functionSignalIdx, n, startOffset, byteOffset);
        }
        copySize = broker->GetCopyByteSize(n);

        for (s = 0; (s < copySize) && (ret); s++) {
            ret = (dataSourcePtr[s + startOffset] == static_cast<char8>(n + s));
        }
        for (s = 0; (s < copySize) && (ret); s++) {
            ret = (*(gamPtr++) == static_cast<char8>(n + s));
        }
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Input_Ranges_1() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamA;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }

    //Check for signal 4 (has ranges)
    uint32 signalIdx;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal4A");
    }

    //The dual buffer implementation has a pointer to the variable containing the pointer. This way the data source
    //can change in real-time this variable value and we will be copying/writing to another buffer.
    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;

    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }
    uint32 n;
    for (n = 0u; (n < 3) && (ret); n++) {
        uint32 startOffset = 0;
        uint32 byteOffset = 0;
        uint32 functionIdx;
        uint32 functionSignalIdx;
        if (ret) {
            ret = dataSource->GetFunctionIndex(functionIdx, "GAMA");
        }
        if (ret) {
            ret = dataSource->GetFunctionSignalIndex(InputSignals, functionIdx, functionSignalIdx, "Signal4");
        }
        if (ret) {
            ret = dataSource->GetFunctionSignalByteOffsetInfo(InputSignals, functionIdx, functionSignalIdx, n, startOffset, byteOffset);
        }
        uint32 copySize = broker->GetCopyByteSize(n);
        uint32 s;
        for (s = 0; (s < copySize) && (ret); s++) {
            ret = (dataSourcePtr[s + startOffset] == static_cast<char8>((n + 1) * s));
        }
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Input_Samples_0() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamA;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }

    //The dual buffer implementation has a pointer to the variable containing the pointer. This way the data source
    //can change in real-time this variable value and we will be copying/writing to another buffer.
    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    char8 *gamPtr;
    uint32 copySize = 0;
    uint32 s;
    uint32 n;
    uint32 signalIdx;
    //Signal5
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal5");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamA->GetInputSignalMemory(1));
    }
    n = 3;
    copySize = broker->GetCopyByteSize(n);
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (dataSourcePtr[s] == static_cast<char8>(n + s));
    }
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (gamPtr[s] == static_cast<char8>(n + s));
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Input_Samples_1() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamA;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }

    //The dual buffer implementation has a pointer to the variable containing the pointer. This way the data source
    //can change in real-time this variable value and we will be copying/writing to another buffer.
    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    uint32 signalIdx;

    //Signal5
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal5");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }

    uint32 n = 3;
    uint32 copySize = broker->GetCopyByteSize(n);
    uint32 s;
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (dataSourcePtr[s] == static_cast<char8>((n + 1) * s));
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Output_0() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamC;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }
    uint32 signalIdx;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal1A");
    }

    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    char8 *gamPtr;
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamC->GetOutputSignalMemory(0));
    }
    uint32 n = 0;
    uint32 copySize = broker->GetCopyByteSize(n);
    uint32 s;
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (dataSourcePtr[s] == static_cast<char8>(n + s));
    }
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (gamPtr[s] == static_cast<char8>(n + s));
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Output_1() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamC;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }
    uint32 signalIdx;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal1A");
    }

    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    uint32 n = 0;
    uint32 copySize = broker->GetCopyByteSize(n);
    uint32 s;

    //Repeat for the other buffer
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal1A");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (dataSourcePtr[s] == static_cast<char8>((n + 1) * s));
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Output_Ranges_0() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamC;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }
    uint32 signalIdx;
    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    char8 *gamPtr;
    uint32 n;
    uint32 copySize;
    uint32 s;

    //Signal4
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal4A");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }
    if (ret) {
        //3rd signal of the GAM. The 2nd belongs to another DataSource (and thus to another Broker)
        gamPtr = reinterpret_cast<char8 *>(gamC->GetOutputSignalMemory(2));
    }
    for (n = 1u; (n < 4) && (ret); n++) {
        uint32 startOffset = 0;
        uint32 byteOffset = 0;
        uint32 functionIdx;
        uint32 functionSignalIdx;
        if (ret) {
            ret = dataSource->GetFunctionIndex(functionIdx, "GAMC");
        }
        if (ret) {
            ret = dataSource->GetFunctionSignalIndex(OutputSignals, functionIdx, functionSignalIdx, "Signal4");
        }
        if (ret) {
            ret = dataSource->GetFunctionSignalByteOffsetInfo(OutputSignals, functionIdx, functionSignalIdx, n - 1, startOffset, byteOffset);
        }
        copySize = broker->GetCopyByteSize(n);

        for (s = 0; (s < copySize) && (ret); s++) {
            ret = (dataSourcePtr[s + startOffset] == static_cast<char8>(n + s));
        }
        for (s = 0; (s < copySize) && (ret); s++) {
            ret = (*(gamPtr++) == static_cast<char8>(n + s));
        }
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Output_Ranges_1() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamC;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }
    uint32 signalIdx;
    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    uint32 n;
    uint32 copySize;
    uint32 s;

    //Signal4
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal4A");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }
    for (n = 1u; (n < 4) && (ret); n++) {
        uint32 startOffset = 0;
        uint32 byteOffset = 0;
        uint32 functionIdx;
        uint32 functionSignalIdx;
        if (ret) {
            ret = dataSource->GetFunctionIndex(functionIdx, "GAMC");
        }
        if (ret) {
            ret = dataSource->GetFunctionSignalIndex(OutputSignals, functionIdx, functionSignalIdx, "Signal4");
        }
        if (ret) {
            ret = dataSource->GetFunctionSignalByteOffsetInfo(OutputSignals, functionIdx, functionSignalIdx, n - 1, startOffset, byteOffset);
        }
        copySize = broker->GetCopyByteSize(n);

        for (s = 0; (s < copySize) && (ret); s++) {
            ret = (dataSourcePtr[s + startOffset] == static_cast<char8>((n + 1) * s));
        }
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Output_Samples_0() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamC;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }
    uint32 signalIdx;
    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    char8 *gamPtr;
    uint32 n = 0;
    uint32 copySize;
    uint32 s;
    //Signal5
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal5");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }
    if (ret) {
        gamPtr = reinterpret_cast<char8 *>(gamC->GetOutputSignalMemory(3));
    }
    n = 4;
    copySize = broker->GetCopyByteSize(n);
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (dataSourcePtr[s] == static_cast<char8>(n + s));
    }
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (gamPtr[s] == static_cast<char8>(n + s));
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_Output_Samples_1() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamC;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }
    uint32 signalIdx;
    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    uint32 n;
    uint32 copySize;
    uint32 s;

    //Signal5
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal5");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 1, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }
    n = 4;
    copySize = broker->GetCopyByteSize(n);
    for (s = 0; (s < copySize) && ret; s++) {
        ret = (dataSourcePtr[s] == static_cast<char8>((n + 1) * s));
    }

    return ret;
}

bool MemoryMapStatefulBrokerTest::TestInit_PrepareNextState() {
    bool ret = InitialiseMemoryMapStatefulBrokerEnviroment(config1);
    ReferenceT<MemoryMapStatefulBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapStatefulBrokerTestHelper> broker;
    ReferenceT<MemoryMapStatefulBrokerTestGAM1> gamA;
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
    //Verify if the DataSourceMemory has the correct pattern
    if (ret) {
        ret = broker->Execute();
    }
    //Change the state
    if (ret) {
        RealTimeStateInfo info;
        ret = dataSource->PrepareNextState(info);
    }
    //Now the odd signal indexes will have their buffer 0 pointing at the memory held by the first buffer
    //And the even signal indexes will have their buffer 1 pointing at the memory held by the second buffer
    //Check for signal 4 (has ranges)
    uint32 signalIdx;
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal4A");
    }

    //The dual buffer implementation has a pointer to the variable containing the pointer. This way the data source
    //can change in real-time this variable value and we will be copying/writing to another buffer.
    void **dataSourcePtrPtr;
    char8 *dataSourcePtr;
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }

    uint32 copySize = 0;
    uint32 s;
    uint32 n;
    for (n = 0u; (n < 3) && (ret); n++) {
        uint32 startOffset = 0;
        uint32 byteOffset = 0;
        uint32 functionIdx;
        uint32 functionSignalIdx;
        if (ret) {
            ret = dataSource->GetFunctionIndex(functionIdx, "GAMA");
        }
        if (ret) {
            ret = dataSource->GetFunctionSignalIndex(InputSignals, functionIdx, functionSignalIdx, "Signal4");
        }
        if (ret) {
            ret = dataSource->GetFunctionSignalByteOffsetInfo(InputSignals, functionIdx, functionSignalIdx, n, startOffset, byteOffset);
        }
        copySize = broker->GetCopyByteSize(n);

        if ((signalIdx % 2) != 0) {
            for (s = 0; (s < copySize) && (ret); s++) {
                ret = (dataSourcePtr[s + startOffset] == static_cast<char8>(n + s));
            }
        }
        else {
            for (s = 0; (s < copySize) && (ret); s++) {
                ret = (dataSourcePtr[s + startOffset] == static_cast<char8>((n + 1) * s));
            }
        }
    }

    //Signal5
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal5");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }

    n = 3;
    copySize = broker->GetCopyByteSize(n);
    if ((signalIdx % 2) != 0) {
        for (s = 0; (s < copySize) && ret; s++) {
            ret = (dataSourcePtr[s] == static_cast<char8>(n + s));
        }
    }
    else {
        for (s = 0; (s < copySize) && (ret); s++) {
            ret = (dataSourcePtr[s] == static_cast<char8>((n + 1) * s));
        }
    }
    //Signal1A
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal1A");
    }
    if (ret) {
        void *getPtr;
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void *&>(getPtr));
        if (ret) {
            dataSourcePtrPtr = reinterpret_cast<void **>(getPtr);
        }
        if (ret) {
            dataSourcePtr = reinterpret_cast<char8 *>(*dataSourcePtrPtr);
        }
    }

    n = 4;
    copySize = broker->GetCopyByteSize(n);
    if ((signalIdx % 2) != 0) {
        for (s = 0; (s < copySize) && ret; s++) {
            ret = (dataSourcePtr[s] == static_cast<char8>(n + s));
        }
    }
    else {
        for (s = 0; (s < copySize) && (ret); s++) {
            ret = (dataSourcePtr[s] == static_cast<char8>((n + 1) * s));
        }
    }
    return ret;
}
