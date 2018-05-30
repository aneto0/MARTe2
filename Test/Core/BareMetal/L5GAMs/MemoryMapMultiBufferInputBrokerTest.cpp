/**
 * @file MemoryMapMultiBufferInputBrokerTest.cpp
 * @brief Source file for class MemoryMapMultiBufferInputBrokerTest
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
 * the class MemoryMapMultiBufferInputBrokerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "MemoryDataSourceI.h"
#include "GAMSchedulerI.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapMultiBufferInputBrokerTest.h"
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
class MemoryMapMultiBufferInputBrokerDSTest: public MemoryDataSourceI {

public:
    CLASS_REGISTER_DECLARATION()

MemoryMapMultiBufferInputBrokerDSTest    ();

    ~MemoryMapMultiBufferInputBrokerDSTest();

    virtual bool GetInputOffset(const uint32 signalIdx,const uint32 samples, uint32 &offset);

    virtual bool GetOutputOffset(const uint32 signalIdx,const uint32 samples, uint32 &offset);

    virtual uint32 GetCurrentStateBuffer();

    virtual void PrepareInputOffsets();

    virtual bool AllocateMemory();

    virtual bool TerminateInputCopy(const uint32 signalIdx,
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

MemoryMapMultiBufferInputBrokerDSTest::MemoryMapMultiBufferInputBrokerDSTest() {
    currentOffsets[0] = 0u;
    currentOffsets[1] = 0u;
    currentOffsets[2] = 0u;
    currentBuffer = 0u;
}

MemoryMapMultiBufferInputBrokerDSTest::~MemoryMapMultiBufferInputBrokerDSTest() {

}

bool MemoryMapMultiBufferInputBrokerDSTest::GetInputOffset(const uint32 signalIdx, const uint32 samples, uint32 &offset) {
    offset = currentOffsets[signalIdx % 3];
    return true;
}

bool MemoryMapMultiBufferInputBrokerDSTest::GetOutputOffset(const uint32 signalIdx, const uint32 samples, uint32 &offset) {
    offset = currentOffsets[signalIdx % 3];
    return true;
}

uint32 MemoryMapMultiBufferInputBrokerDSTest::GetNumberOfStatefulMemoryBuffers() {
    return 2u;
}

bool MemoryMapMultiBufferInputBrokerDSTest::Synchronise() {
    return true;
}

bool MemoryMapMultiBufferInputBrokerDSTest::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}

bool MemoryMapMultiBufferInputBrokerDSTest::GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr) {
    ReferenceT<MemoryMapMultiBufferInputBroker> broker("MemoryMapMultiBufferInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapMultiBufferInputBrokerDSTest::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr) {
    ReferenceT<MemoryMapMultiBufferOutputBroker> broker("MemoryMapMultiBufferOutputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

const char8 *MemoryMapMultiBufferInputBrokerDSTest::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapMultiBufferInputBroker";
    }
    return "MemoryMapMultiBufferOutputBroker";
}

bool MemoryMapMultiBufferInputBrokerDSTest::AllocateMemory() {
    bool ret = MemoryDataSourceI::AllocateMemory();

    if (ret) {
        uint32 *memint = (uint32 *) memory;
        for (uint32 i = 0u; i < (totalMemorySize / 4); i++) {
            memint[i] = i;
        }
    }

    return ret;

}

bool MemoryMapMultiBufferInputBrokerDSTest::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 samples) {

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

uint32 MemoryMapMultiBufferInputBrokerDSTest::GetCurrentStateBuffer() {
    return currentBuffer;
}

void MemoryMapMultiBufferInputBrokerDSTest::PrepareInputOffsets() {
    currentBuffer++;
    currentBuffer %= 2;
}

CLASS_REGISTER(MemoryMapMultiBufferInputBrokerDSTest, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapInputBroker tests
 */
class MemoryMapMultiBufferInputBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapMultiBufferInputBrokerTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();

    void *GetInputMemoryBuffer();
};

MemoryMapMultiBufferInputBrokerTestGAM1::MemoryMapMultiBufferInputBrokerTestGAM1() :
        GAM() {
}

bool MemoryMapMultiBufferInputBrokerTestGAM1::Setup() {
    return true;
}

bool MemoryMapMultiBufferInputBrokerTestGAM1::Execute() {
    return true;
}

void *MemoryMapMultiBufferInputBrokerTestGAM1::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

CLASS_REGISTER(MemoryMapMultiBufferInputBrokerTestGAM1, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MemoryMapMultiBufferInputBrokerTest::MemoryMapMultiBufferInputBrokerTest() {
}

MemoryMapMultiBufferInputBrokerTest::~MemoryMapMultiBufferInputBrokerTest() {
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

bool MemoryMapMultiBufferInputBrokerTest::TestConstructor() {
    MemoryMapMultiBufferInputBroker broker;
    bool ret = (broker.GetNumberOfCopies() == 0u);

    return ret;
}

bool MemoryMapMultiBufferInputBrokerTest::TestExecute() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = MemoryMapMultiBufferInputBrokerTestGAM1"
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
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = MemoryMapMultiBufferInputBrokerDSTest"
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

    ReferenceT<MemoryMapMultiBufferInputBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<MemoryMapMultiBufferInputBrokerTestGAM1> gam;
    ReferenceContainer brokers;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam->GetInputBrokers(brokers);
    }

    ReferenceT<MemoryMapMultiBufferInputBroker> broker;
    if (ret) {
        ret = brokers.Size() == 1;
        if (ret) {
            broker = brokers.Get(0);
            ret = broker.IsValid();
        }
    }

    uint32 *gamPtr = (uint32*) gam->GetInputMemoryBuffer();
    if (ret) {
        ret = broker->Execute();
        uint32 nBuffers = dataSource->GetNumberOfMemoryBuffers();
        uint32 signal4Shift = 2; //2 buffers
        uint32 bufferOffset = 26;

        if (ret) {
            ret = (nBuffers == 2);
            if (ret) {
                ret &= (gamPtr[0] == 0 + signal4Shift + bufferOffset);
                ret &= (gamPtr[1] == 2 + signal4Shift + bufferOffset);
                //signal 2 shift of 3!
                ret &= (gamPtr[2] == 4 + (3 % nBuffers) * 10 + signal4Shift + bufferOffset);
                //signal 3 shift of 4%2=21!
                ret &= (gamPtr[3] == 22 + signal4Shift + bufferOffset);
            }
        }
        bufferOffset = 0;
        if (ret) {
            ret = broker->Execute();
            if (ret) {
                ret &= (gamPtr[0] == 0 + (2 % nBuffers) + signal4Shift + bufferOffset);
                ret &= (gamPtr[1] == 2 + (6 % nBuffers) * 10 + signal4Shift + bufferOffset);
                ret &= (gamPtr[2] == 4 + (9 % nBuffers) * 10 + signal4Shift + bufferOffset);
                ret &= (gamPtr[3] == 22 + (1 % nBuffers) + signal4Shift + bufferOffset);
            }
        }

    }

    return ret;
}

