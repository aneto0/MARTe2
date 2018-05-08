/**
 * @file MemoryMapMultiBufferBrokerTest.cpp
 * @brief Source file for class MemoryMapMultiBufferBrokerTest
 * @date 04/05/2018
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
 * the class MemoryMapMultiBufferBrokerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "MemoryDataSourceI.h"
#include "MemoryMapMultiBufferBroker.h"
#include "MemoryMapMultiBufferBrokerTest.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Dummy DataSource to test the broker
 */
class MemoryMapMultiBufferBrokerDSTest: public MemoryDataSourceI {

public:
    CLASS_REGISTER_DECLARATION()

MemoryMapMultiBufferBrokerDSTest    ();

    virtual ~MemoryMapMultiBufferBrokerDSTest();

    virtual bool GetInputOffset(const uint32 signalIdx,const uint32 samples, uint32 &offset);

    virtual bool GetOutputOffset(const uint32 signalIdx,const uint32 samples, uint32 &offset);

    virtual uint32 GetCurrentStateBuffer();

    virtual uint32 GetNumberOfStatefulMemoryBuffers();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool AllocateMemory();

    virtual bool Synchronise();

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr);
protected:

    uint32 currentOffsets[3];
    uint32 currentBuffer;
};

MemoryMapMultiBufferBrokerDSTest::MemoryMapMultiBufferBrokerDSTest() {
    currentOffsets[0] = 0u;
    currentOffsets[1] = 0u;
    currentOffsets[2] = 0u;
    currentBuffer = 0u;

}

MemoryMapMultiBufferBrokerDSTest::~MemoryMapMultiBufferBrokerDSTest() {

}

bool MemoryMapMultiBufferBrokerDSTest::GetInputOffset(const uint32 signalIdx, const uint32 samples, uint32 &offset) {
    offset = currentOffsets[signalIdx % 3];
    return true;
}

bool MemoryMapMultiBufferBrokerDSTest::GetOutputOffset(const uint32 signalIdx, const uint32 samples, uint32 &offset) {
    offset = currentOffsets[signalIdx % 3];
    return true;
}

uint32 MemoryMapMultiBufferBrokerDSTest::GetNumberOfMemoryBuffers() {
    return numberOfBuffers;
}

uint32 MemoryMapMultiBufferBrokerDSTest::GetNumberOfStatefulMemoryBuffers() {
    return 2u;
}

bool MemoryMapMultiBufferBrokerDSTest::Synchronise() {
    for (uint32 i = 0u; i < numberOfSignals; i++) {
        currentOffsets[i] += signalSize[i];
        currentOffsets[i] %= (signalSize[i] * numberOfBuffers);
    }
    return true;
}

bool MemoryMapMultiBufferBrokerDSTest::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}


bool MemoryMapMultiBufferBrokerDSTest::AllocateMemory() {
    bool ret = MemoryDataSourceI::AllocateMemory();
    if (ret) {
        uint32 *memint = (uint32 *) memory;
        for (uint32 i = 0u; i < (totalMemorySize / 4); i++) {
            memint[i] = i;
        }
    }

    return ret;

}

uint32 MemoryMapMultiBufferBrokerDSTest::GetCurrentStateBuffer() {
    return currentBuffer;
}

const char8 *MemoryMapMultiBufferBrokerDSTest::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapMultiBufferBrokerTestInputBroker";
    }
    return "MemoryMapMultiBufferBrokerTestOutputBroker";
}

CLASS_REGISTER(MemoryMapMultiBufferBrokerDSTest, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapInputBroker tests
 */
class MemoryMapMultiBufferBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapMultiBufferBrokerTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();

    void *GetInputMemoryBuffer();
};

MemoryMapMultiBufferBrokerTestGAM1::MemoryMapMultiBufferBrokerTestGAM1() :
        GAM() {
}

bool MemoryMapMultiBufferBrokerTestGAM1::Setup() {

    uint32 numberOfSignals = GetNumberOfOutputSignals();

    uint32 totalSize = 0u;
    for (uint32 i = 0u; i < numberOfSignals; i++) {
        uint32 byteSize;
        GetSignalByteSize(OutputSignals, i, byteSize);
        uint32 nSamples = 0u;
        GetSignalNumberOfSamples(OutputSignals, i, nSamples);
        totalSize += (byteSize * nSamples);
    }

    uint32 *mem = (uint32 *) GetOutputSignalsMemory();
    for (uint32 i = 0u; i < (totalSize / 4); i++) {
        mem[i] = i;
    }

    return true;
}

bool MemoryMapMultiBufferBrokerTestGAM1::Execute() {
    return true;
}

void *MemoryMapMultiBufferBrokerTestGAM1::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

CLASS_REGISTER(MemoryMapMultiBufferBrokerTestGAM1, "1.0")

class MemoryMapMultiBufferBrokerTestInputBroker: public MemoryMapMultiBufferInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapMultiBufferBrokerTestInputBroker    () {

    }

    virtual ~MemoryMapMultiBufferBrokerTestInputBroker() {

    }

    virtual uint32 *GetSignalIdxArr(uint32 &size) {
        size = numberOfCopies;
        return signalIdxArr;
    }

    bool CopyInputs() {
        return MemoryMapMultiBufferBroker::CopyInputs();
    }

};

CLASS_REGISTER(MemoryMapMultiBufferBrokerTestInputBroker, "1.0")

class MemoryMapMultiBufferBrokerTestOutputBroker: public MemoryMapMultiBufferOutputBroker {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapMultiBufferBrokerTestOutputBroker    () {

    }

    virtual ~MemoryMapMultiBufferBrokerTestOutputBroker() {

    }

    bool CopyOutputs() {
        return MemoryMapMultiBufferBroker::CopyOutputs();
    }
};

CLASS_REGISTER(MemoryMapMultiBufferBrokerTestOutputBroker, "1.0")

bool MemoryMapMultiBufferBrokerDSTest::GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr) {
    ReferenceT<MemoryMapMultiBufferBrokerTestInputBroker> broker("MemoryMapMultiBufferBrokerTestInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapMultiBufferBrokerDSTest::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr) {
    ReferenceT<MemoryMapMultiBufferBrokerTestOutputBroker> broker("MemoryMapMultiBufferBrokerTestOutputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

/**
 * MARTe configuration structure to test the MemoryMapInputBroker
 */
static const char8 * const config = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapMultiBufferBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 10"
        "                   Type = uint32"
        "                   Ranges = {{0, 0}, {2, 2}}"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = MemoryMapMultiBufferBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0, 0}, {9, 9}}"
        "                   Type = uint32"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = MemoryMapMultiBufferBrokerDSTest"
        "            NumberOfBuffers = 2"
        "            Signals = {"
        "               Signal4 = {"
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
        "                    Functions = {GAMA GAMB}"
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

MemoryMapMultiBufferBrokerTest::MemoryMapMultiBufferBrokerTest() {
}

MemoryMapMultiBufferBrokerTest::~MemoryMapMultiBufferBrokerTest() {
}

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

bool MemoryMapMultiBufferBrokerTest::TestConstructor() {
    MemoryMapMultiBufferBrokerTestInputBroker brokerTest;
    uint32 nOfCopies = 0;
    uint32 *signalIdxArr = brokerTest.GetSignalIdxArr(nOfCopies);
    bool ret = nOfCopies == 0;
    if (ret) {
        ret = signalIdxArr == NULL;
    }

    return ret;
}

bool MemoryMapMultiBufferBrokerTest::TestInit() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    MemoryMapMultiBufferBrokerTestInputBroker brokerTest;

    ReferenceT<MemoryMapMultiBufferBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    MemoryMapMultiBufferBrokerDSTest *dataSourceInstance = dataSource.operator->();

    ReferenceT<MemoryMapMultiBufferBrokerTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        void* gamPtr = gam->GetInputMemoryBuffer();
        ret = brokerTest.Init(InputSignals, *dataSourceInstance, "GAMA", gamPtr);
    }

    if (ret) {
        uint32 nOfCopies = 0;
        uint32 *signalIdxArr = brokerTest.GetSignalIdxArr(nOfCopies);
        ret = (signalIdxArr != NULL);
        if (ret) {
            ret = (nOfCopies == 4);
        }
        if (ret) {
            ret = (signalIdxArr[0] == 1);
            ret &= (signalIdxArr[1] == 2);
            ret &= (signalIdxArr[2] == 2);
            ret &= (signalIdxArr[3] == 3);
        }
    }

    return ret;
}

bool MemoryMapMultiBufferBrokerTest::TestInit1() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    MemoryMapMultiBufferBrokerTestInputBroker brokerTest;

    ReferenceT<MemoryMapMultiBufferBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    MemoryMapMultiBufferBrokerDSTest *dataSourceInstance = dataSource.operator->();

    ReferenceT<MemoryMapMultiBufferBrokerTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gam.IsValid();
    }
    if (ret) {
        void* gamPtr = gam->GetInputMemoryBuffer();
        ret = brokerTest.Init(InputSignals, *dataSourceInstance, "GAMB", gamPtr);
    }

    if (ret) {
        uint32 nOfCopies = 0;
        uint32 *signalIdxArr = brokerTest.GetSignalIdxArr(nOfCopies);
        ret = (signalIdxArr != NULL);
        if (ret) {
            ret = (nOfCopies == 2);
        }
        if (ret) {
            ret = (signalIdxArr[0] == 2);
            ret &= (signalIdxArr[1] == 2);
        }
    }

    return ret;
}

bool MemoryMapMultiBufferBrokerTest::TestCopyInputs() {
    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = MemoryMapMultiBufferBrokerTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   Type = uint32"
            "                   Ranges = {{0, 0}, {2, 2}}"
            "                   Samples = 3"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = MemoryMapMultiBufferBrokerDSTest"
            "            NumberOfBuffers = 2"
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
    MemoryMapMultiBufferBrokerTestInputBroker brokerTest;

    ReferenceT<MemoryMapMultiBufferBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<MemoryMapMultiBufferBrokerTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    ReferenceContainer brokers;
    ReferenceT<MemoryMapMultiBufferBrokerTestInputBroker> broker;
    uint32* data = NULL;
    if (ret) {
        gam->GetInputBrokers(brokers);
        broker = brokers.Get(0);
        data = (uint32*) gam->GetInputMemoryBuffer();
    }

    if (ret) {
        broker->CopyInputs();
        ret = (data[0] == 0);
        ret &= (data[1] == 1);
        ret &= (data[2] == 0);
        ret &= (data[3] == 2);
        ret &= (data[4] == 5);
        ret &= (data[5] == 2);
        ret &= (data[6] == 4);
        ret &= (data[7] == 7);
        ret &= (data[8] == 4);
    }

    if (ret) {
        dataSource->Synchronise();
        broker->CopyInputs();
        ret = (data[0] == 1);
        ret &= (data[1] == 0);
        ret &= (data[2] == 1);
        ret &= (data[3] == 5);
        ret &= (data[4] == 2);
        ret &= (data[5] == 5);
        ret &= (data[6] == 7);
        ret &= (data[7] == 4);
        ret &= (data[8] == 7);
    }

    return ret;
}

bool MemoryMapMultiBufferBrokerTest::TestCopyOutputs() {
    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = MemoryMapMultiBufferBrokerTestGAM1"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   Type = uint32"
            "                   Ranges = {{0, 0}, {2, 2}}"
            "                   Samples = 3"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = MemoryMapMultiBufferBrokerDSTest"
            "            NumberOfBuffers = 2"
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
    MemoryMapMultiBufferBrokerTestInputBroker brokerTest;

    ReferenceT<MemoryMapMultiBufferBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<MemoryMapMultiBufferBrokerTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    ReferenceContainer brokers;
    ReferenceT<MemoryMapMultiBufferBrokerTestOutputBroker> broker;
    uint32* data = NULL;
    if (ret) {
        gam->GetOutputBrokers(brokers);
        broker = brokers.Get(0);
        dataSource->GetSignalMemoryBuffer(0, 0, (void*&) data);
    }

    if (ret) {
        broker->CopyOutputs();
        ret = (data[0] == 2);
        ret &= (data[1] == 1);
        ret &= (data[2] == 5);
        ret &= (data[3] == 3);
        ret &= (data[4] == 8);
        ret &= (data[5] == 4);
        ret &= (data[6] == 6);
        ret &= (data[7] == 7);

    }
    if (ret) {
        dataSource->Synchronise();
        broker->CopyOutputs();
        ret = (data[0] == 1);
        ret &= (data[1] == 2);
        ret &= (data[2] == 4);
        ret &= (data[3] == 3);
        ret &= (data[4] == 7);
        ret &= (data[5] == 5);
        ret &= (data[6] == 6);
        ret &= (data[7] == 8);
    }
    return ret;
}

