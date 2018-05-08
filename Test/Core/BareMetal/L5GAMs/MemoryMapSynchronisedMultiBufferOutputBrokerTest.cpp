/**
 * @file MemoryMapSynchronisedMultiBufferOutputBrokerTest.cpp
 * @brief Source file for class MemoryMapSynchronisedMultiBufferOutputBrokerTest
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
 * the class MemoryMapSynchronisedMultiBufferOutputBrokerTest (public, protected, and private). Be aware that some
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
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBrokerTest.h"
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
class MemoryMapSynchronisedMultiBufferOutputBrokerDSTest: public MemoryDataSourceI {

public:
    CLASS_REGISTER_DECLARATION()

MemoryMapSynchronisedMultiBufferOutputBrokerDSTest    ();

    ~MemoryMapSynchronisedMultiBufferOutputBrokerDSTest();

    virtual bool GetInputOffset(const uint32 signalIdx,const uint32 samples, uint32 &offset);

    virtual bool GetOutputOffset(const uint32 signalIdx,const uint32 samples, uint32 &offset);

    virtual uint32 GetCurrentStateBuffer();

    virtual bool TerminateOutputCopy(const uint32 signalIdx,
            const uint32 offset, const uint32 samples);

    virtual bool Synchronise();

    virtual uint32 GetNumberOfStatefulMemoryBuffers();

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr);
protected:

    uint32 currentOffsets[3];
    uint32 currentBuffer;
};

MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::MemoryMapSynchronisedMultiBufferOutputBrokerDSTest() {
    currentOffsets[0] = 0u;
    currentOffsets[1] = 0u;
    currentOffsets[2] = 0u;
    currentBuffer = 0u;
}

MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::~MemoryMapSynchronisedMultiBufferOutputBrokerDSTest() {

}

bool MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::GetInputOffset(const uint32 signalIdx, const uint32 samples, uint32 &offset) {
    offset = currentOffsets[signalIdx % 3];
    return true;
}

bool MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::GetOutputOffset(const uint32 signalIdx, const uint32 samples, uint32 &offset) {
    offset = currentOffsets[signalIdx % 3];
    return true;
}

uint32 MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::GetNumberOfStatefulMemoryBuffers() {
    return 2u;
}

bool MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::Synchronise() {
    currentBuffer++;
    currentBuffer %= 2;
    return true;
}

bool MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}

bool MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName,
                                                                         void * const gamMemPtr) {
    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> broker("MemoryMapSynchronisedMultiBufferInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName,
                                                                          void * const gamMemPtr) {
    ReferenceT<MemoryMapSynchronisedMultiBufferOutputBroker> broker("MemoryMapSynchronisedMultiBufferOutputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

const char8 *MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapSynchronisedMultiBufferInputBroker";
    }
    return "MemoryMapSynchronisedMultiBufferOutputBroker";
}

bool MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::TerminateOutputCopy(const uint32 signalIdx, const uint32 offset, const uint32 samples) {

    uint32 index = (signalIdx % 3);
    if (index == 0) {
        currentOffsets[index] += signalSize[index];
    }
    if (index == 1) {
        currentOffsets[index] += 2 * signalSize[index];
    }
    if (index == 2) {
        currentOffsets[index] += 3 * signalSize[index];
    }
    currentOffsets[index] %= (signalSize[index] * numberOfBuffers);
    return true;
}

uint32 MemoryMapSynchronisedMultiBufferOutputBrokerDSTest::GetCurrentStateBuffer() {
    return currentBuffer;
}

CLASS_REGISTER(MemoryMapSynchronisedMultiBufferOutputBrokerDSTest, "1.0")
/**
 * @brief GAM empty implementation to support the MemoryMapInputBroker tests
 */
class MemoryMapSynchronisedMultiBufferOutputBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapSynchronisedMultiBufferOutputBrokerTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();

    void *GetOutputMemoryBuffer();
private:
    uint32 totalSize;
};

MemoryMapSynchronisedMultiBufferOutputBrokerTestGAM1::MemoryMapSynchronisedMultiBufferOutputBrokerTestGAM1() :
        GAM() {
    totalSize = 0u;
}

bool MemoryMapSynchronisedMultiBufferOutputBrokerTestGAM1::Setup() {

    uint32 numberOfSignals = GetNumberOfOutputSignals();

    totalSize = 0u;
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

bool MemoryMapSynchronisedMultiBufferOutputBrokerTestGAM1::Execute() {
    return true;
}

void *MemoryMapSynchronisedMultiBufferOutputBrokerTestGAM1::GetOutputMemoryBuffer() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(MemoryMapSynchronisedMultiBufferOutputBrokerTestGAM1, "1.0")

/**
 * MARTe configuration structure to test the MemoryMapInputBroker
 */
static const char8 * const config = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapSynchronisedMultiBufferOutputBrokerTestGAM1"
        "            OutputSignals = {"
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
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = MemoryMapSynchronisedMultiBufferOutputBrokerDSTest"
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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MemoryMapSynchronisedMultiBufferOutputBrokerTest::MemoryMapSynchronisedMultiBufferOutputBrokerTest() {
}

MemoryMapSynchronisedMultiBufferOutputBrokerTest::~MemoryMapSynchronisedMultiBufferOutputBrokerTest() {
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

bool MemoryMapSynchronisedMultiBufferOutputBrokerTest::TestConstructor() {
    MemoryMapSynchronisedMultiBufferOutputBroker brokerTest;
    bool ret = (brokerTest.GetNumberOfCopies() == 0u);
    return ret;
}

bool MemoryMapSynchronisedMultiBufferOutputBrokerTest::TestExecute() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<MemoryMapSynchronisedMultiBufferOutputBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<MemoryMapSynchronisedMultiBufferOutputBrokerTestGAM1> gam;
    ReferenceContainer brokers;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam->GetOutputBrokers(brokers);
    }

    ReferenceT<MemoryMapSynchronisedMultiBufferOutputBroker> broker;
    if (ret) {
        ret = brokers.Size() == 1;
        if (ret) {
            broker = brokers.Get(0);
            ret = broker.IsValid();
        }
    }

    uint32 *dataPtr = NULL;
    if (ret) {
        ret = dataSource->GetSignalMemoryBuffer(0, 0, (void*&) dataPtr);
    }

    if (ret) {
        ret = broker->Execute();
        uint32 nBuffers = dataSource->GetNumberOfMemoryBuffers();

        if (ret) {
            ret = (nBuffers == 2);
            if (ret) {
                ret &= (dataPtr[0] == 0);
                ret &= (dataPtr[2] == 1);
                ret &= (dataPtr[4 + (2 % nBuffers) * 10] == 2);
                ret &= (dataPtr[22] == 3);
            }
        }

        if (ret) {
            ret = broker->Execute();
            uint32 bufferShift = 24;
            if (ret) {
                ret &= (dataPtr[0 + (1 % nBuffers) + bufferShift] == 0);
                ret &= (dataPtr[2 + (4 % nBuffers) * 10 + bufferShift] == 1);
                ret &= (dataPtr[4 + (6 % nBuffers) * 10 + bufferShift] == 2);
                ret &= (dataPtr[22 + (3 % nBuffers) + bufferShift] == 3);
            }
        }

    }

    return ret;
}

