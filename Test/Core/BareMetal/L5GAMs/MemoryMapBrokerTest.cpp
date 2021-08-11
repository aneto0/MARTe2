/**
 * @file MemoryMapBrokerTest.cpp
 * @brief Source file for class MemoryMapBrokerTest
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
 * the class MemoryMapBrokerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"
#include "MemoryMapBrokerTest.h"
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
 * @brief GAMSchedulerI empty implementation to support the MemoryMapBroker tests
 */
class MemoryMapBrokerTestScheduler1: public GAMSchedulerI {
public:CLASS_REGISTER_DECLARATION()

    MemoryMapBrokerTestScheduler1();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();

    virtual void CustomPrepareNextState() {

    }

};

MemoryMapBrokerTestScheduler1::MemoryMapBrokerTestScheduler1() :
        GAMSchedulerI() {

}

MARTe::ErrorManagement::ErrorType MemoryMapBrokerTestScheduler1::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType MemoryMapBrokerTestScheduler1::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

CLASS_REGISTER(MemoryMapBrokerTestScheduler1, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapBroker tests
 */
class MemoryMapBrokerTestGAM1: public GAM {
public:CLASS_REGISTER_DECLARATION()

    MemoryMapBrokerTestGAM1();

    virtual bool Setup();

    virtual bool Execute();

    void* GetInputSignalsMemory();

    void* GetOutputSignalsMemory();

    void* GetInputSignalMemory(uint32 signalIdx);

    void* GetOutputSignalMemory(uint32 signalIdx);
};

MemoryMapBrokerTestGAM1::MemoryMapBrokerTestGAM1() :
        GAM() {
}

void* MemoryMapBrokerTestGAM1::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void* MemoryMapBrokerTestGAM1::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

void* MemoryMapBrokerTestGAM1::GetInputSignalMemory(uint32 signalIdx) {
    return GAM::GetInputSignalMemory(signalIdx);
}

void* MemoryMapBrokerTestGAM1::GetOutputSignalMemory(uint32 signalIdx) {
    return GAM::GetOutputSignalMemory(signalIdx);
}

bool MemoryMapBrokerTestGAM1::Setup() {
    return true;
}

bool MemoryMapBrokerTestGAM1::Execute() {
    return true;
}
CLASS_REGISTER(MemoryMapBrokerTestGAM1, "1.0")

/**
 * @brief The MemoryMapBroker is an abstract class. This implementation
 * allows to test if the Init() function is correctly performed.
 */
class MemoryMapBrokerTestHelper: public MemoryMapBroker {
public:CLASS_REGISTER_DECLARATION()

    virtual bool Execute();

};

/**
 * @brief Writes a known pattern to the memory so that the tests can check
 * if the Init() function behaves as expected.
 * @details The DataSource and GAM memory data is set as the sum of the
 * current copy index and the current signal index.
 */
bool MemoryMapBrokerTestHelper::Execute() {
    uint32 numberOfCopies = GetNumberOfCopies();
    uint32 n;
    uint32 s;
    for (n = 0; (n < numberOfCopies); n++) {
        char8 *dsPtr = reinterpret_cast<char8*>(copyTable[n].dataSourcePointer);
        char8 *gamPtr = reinterpret_cast<char8*>(copyTable[n].gamPointer);
        for (s = 0; s < copyTable[n].copySize; s++) {
            dsPtr[s] = static_cast<char8>(n + s);
            gamPtr[s] = static_cast<char8>(n + s);
        }
    }
    return true;
}

CLASS_REGISTER(MemoryMapBrokerTestHelper, "1.0");

/**
 * @brief DataSourceI implementation which returns a MemoryMapBrokerTestHelper broker.
 */
class MemoryMapBrokerDataSourceTestHelper: public DataSourceI {
public:CLASS_REGISTER_DECLARATION()

    MemoryMapBrokerDataSourceTestHelper();

    virtual ~MemoryMapBrokerDataSourceTestHelper();

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                                       const uint32 bufferIdx,
                                       void *&signalAddress);

    virtual const char8* GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    virtual bool PrepareNextState(const char8 *const currentStateName,
                                  const char8 *const nextStateName);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8 *const functionName,
                                 void *const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8 *const functionName,
                                  void *const gamMemPtr);

    virtual bool Synchronise();

    void *signalMemory;
    uint32 *offsets;
    //Store 10 samples per signal
    uint32 samples;

};

MemoryMapBrokerDataSourceTestHelper::MemoryMapBrokerDataSourceTestHelper() :
        DataSourceI() {
    signalMemory = NULL_PTR(void*);
    offsets = NULL_PTR(uint32*);
    samples = 10;
}

MemoryMapBrokerDataSourceTestHelper::~MemoryMapBrokerDataSourceTestHelper() {
    if (signalMemory != NULL_PTR(void*)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemory);
    }
    if (offsets != NULL_PTR(uint32*)) {
        delete[] offsets;
    }
}

bool MemoryMapBrokerDataSourceTestHelper::AllocateMemory() {
    uint32 numberOfSignals = GetNumberOfSignals();
    bool ret = (numberOfSignals > 0u);
    if (ret) {
        ret = (offsets == NULL_PTR(uint32*));
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
        MemoryOperationsHelper::Set(signalMemory, 0, totalMemorySize);
    }
    return ret;
}

uint32 MemoryMapBrokerDataSourceTestHelper::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool MemoryMapBrokerDataSourceTestHelper::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                                const uint32 bufferIdx,
                                                                void *&signalAddress) {
    char8 *memPtr = reinterpret_cast<char8*>(signalMemory);
    memPtr += offsets[signalIdx];
    signalAddress = reinterpret_cast<void*&>(memPtr);
    return true;
}

const char8* MemoryMapBrokerDataSourceTestHelper::GetBrokerName(StructuredDataI &data,
                                                                const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapBrokerTestHelper";
    }
    return "MemoryMapBrokerTestHelper";
}

bool MemoryMapBrokerDataSourceTestHelper::PrepareNextState(const char8 *const currentStateName,
                                                           const char8 *const nextStateName) {
    return true;
}

bool MemoryMapBrokerDataSourceTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                          const char8 *const functionName,
                                                          void *const gamMemPtr) {
    ReferenceT<MemoryMapBrokerTestHelper> broker("MemoryMapBrokerTestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapBrokerDataSourceTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                           const char8 *const functionName,
                                                           void *const gamMemPtr) {
    ReferenceT<MemoryMapBrokerTestHelper> broker("MemoryMapBrokerTestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapBrokerDataSourceTestHelper::Synchronise() {
    return false;
}

CLASS_REGISTER(MemoryMapBrokerDataSourceTestHelper, "1.0");

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapBrokerEnviroment(const char8 *const config) {
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
 * MARTe configuration structure to test the MemoryMapBroker
 */
static const char8 *const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal4A"
        "                   NumberOfDimensions = 2"
        "                   NumberOfElements = 10"
        "                   Ranges = {{1 1} {2 5} {9 9}}"
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
        "            Class = MemoryMapBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = MemoryMapBrokerTestGAM1"
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
        "                   Ranges = {{1 1} {2 5} {9 9}}"
        "               }"
        "               Signal5 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 3"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = MemoryMapBrokerTestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = MemoryMapBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = MemoryMapBrokerTestGAM1"
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
        "            Class = MemoryMapBrokerDataSourceTestHelper"
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
        "        Class = MemoryMapBrokerTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool MemoryMapBrokerTest::TestConstructor() {
    ReferenceT<MemoryMapBrokerTestHelper> broker("MemoryMapBrokerTestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->GetNumberOfCopies() == 0;
    }
    return ret;
}

bool MemoryMapBrokerTest::TestInit_Input() {
    bool ret = InitialiseMemoryMapBrokerEnviroment(config1);
    ReferenceT<MemoryMapBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapBrokerTestHelper> broker;
    ReferenceT<MemoryMapBrokerTestGAM1> gamA;
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
        ret = dataSource->GetInputBrokers(brokers, "GAMA", (void*) gamA->GetInputSignalsMemory());
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
    char8 *dataSourcePtr = NULL_PTR(char8*);
    char8 *gamPtr = NULL_PTR(char8*);
    uint32 copySize = 0;
    uint32 s;
    uint32 n;
    //Signal1A
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal1A");
    }
    if (ret) {
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void*&>(dataSourcePtr));
//        uint32 aux0 = 0;
//        ret = dataSource->GetSignalIndex(aux0, "Signal2");
//        uint32 aux = 0;
//        ret = dataSource->GetSignalIndex(aux, "Signal3");
//        uint32 aux1 = 0;
//        ret = dataSource->GetSignalIndex(aux1, "Signal4A");
//        uint32 aux2 = 0;
//        ret = dataSource->GetSignalIndex(aux2, "Signal5");
    }
    if (ret) {
        //4th signal of the GAM. The 3rd belongs to another DataSource (and thus to another Broker)
        gamPtr = reinterpret_cast<char8*>(gamA->GetInputSignalMemory(3));
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

bool MemoryMapBrokerTest::TestInit_Input_Ranges() {
    bool ret = InitialiseMemoryMapBrokerEnviroment(config1);
    ReferenceT<MemoryMapBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapBrokerTestHelper> broker;
    ReferenceT<MemoryMapBrokerTestGAM1> gamA;
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
        ret = dataSource->GetInputBrokers(brokers, "GAMA", (void*) gamA->GetInputSignalsMemory());
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
    char8 *dataSourcePtr = NULL_PTR(char8*);
    char8 *gamPtr = NULL_PTR(char8*);
    //Signal4A
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal4A");
    }
    if (ret) {
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void*&>(dataSourcePtr));
    }
    if (ret) {
        gamPtr = reinterpret_cast<char8*>(gamA->GetInputSignalMemory(0));
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

bool MemoryMapBrokerTest::TestInit_Input_Samples() {
    bool ret = InitialiseMemoryMapBrokerEnviroment(config1);
    ReferenceT<MemoryMapBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapBrokerTestHelper> broker;
    ReferenceT<MemoryMapBrokerTestGAM1> gamA;
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
        ret = dataSource->GetInputBrokers(brokers, "GAMA", (void*) gamA->GetInputSignalsMemory());
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
    char8 *dataSourcePtr = NULL_PTR(char8*);
    char8 *gamPtr = NULL_PTR(char8*);
    uint32 copySize = 0;
    uint32 s;
    uint32 n;

    //Signal5
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal5");
    }
    if (ret) {
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void*&>(dataSourcePtr));
    }
    if (ret) {
        gamPtr = reinterpret_cast<char8*>(gamA->GetInputSignalMemory(1));
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

bool MemoryMapBrokerTest::TestInit_Output() {
    bool ret = InitialiseMemoryMapBrokerEnviroment(config1);
    ReferenceT<MemoryMapBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapBrokerTestHelper> broker;
    ReferenceT<MemoryMapBrokerTestGAM1> gamC;
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
        ret = dataSource->GetOutputBrokers(brokers, "GAMC", (void*) gamC->GetOutputSignalsMemory());
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
    char8 *dataSourcePtr = NULL;
    char8 *gamPtr = NULL;
    if (ret) {
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void*&>(dataSourcePtr));
    }
    if (ret) {
        gamPtr = reinterpret_cast<char8*>(gamC->GetOutputSignalMemory(0));
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

bool MemoryMapBrokerTest::TestInit_Output_Ranges() {
    bool ret = InitialiseMemoryMapBrokerEnviroment(config1);
    ReferenceT<MemoryMapBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapBrokerTestHelper> broker;
    ReferenceT<MemoryMapBrokerTestGAM1> gamC;
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
        ret = dataSource->GetOutputBrokers(brokers, "GAMC", (void*) gamC->GetOutputSignalsMemory());
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
    char8 *dataSourcePtr = NULL_PTR(char8*);
    char8 *gamPtr = NULL_PTR(char8*);
    uint32 n = 0;
    uint32 copySize = broker->GetCopyByteSize(n);
    uint32 s;

    //Signal4
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal4A");
    }
    if (ret) {
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void*&>(dataSourcePtr));
    }
    if (ret) {
        //3rd signal of the GAM. The 2nd belongs to another DataSource (and thus to another Broker)
        gamPtr = reinterpret_cast<char8*>(gamC->GetOutputSignalMemory(2));
        uint32 signalNBytes = 0;
        gamC->GetSignalByteSize(OutputSignals, 2, signalNBytes);
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

bool MemoryMapBrokerTest::TestInit_Output_Samples() {
    bool ret = InitialiseMemoryMapBrokerEnviroment(config1);
    ReferenceT<MemoryMapBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapBrokerTestHelper> broker;
    ReferenceT<MemoryMapBrokerTestGAM1> gamC;
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
        ret = dataSource->GetOutputBrokers(brokers, "GAMC", (void*) gamC->GetOutputSignalsMemory());
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
    char8 *dataSourcePtr = NULL_PTR(char8*);
    char8 *gamPtr = NULL_PTR(char8*);
    uint32 n = 0;
    uint32 copySize = broker->GetCopyByteSize(n);
    uint32 s;

    //Signal5
    if (ret) {
        ret = dataSource->GetSignalIndex(signalIdx, "Signal5");
    }
    if (ret) {
        ret = dataSource->GetSignalMemoryBuffer(signalIdx, 0, reinterpret_cast<void*&>(dataSourcePtr));
    }
    if (ret) {
        gamPtr = reinterpret_cast<char8*>(gamC->GetOutputSignalMemory(3));
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
