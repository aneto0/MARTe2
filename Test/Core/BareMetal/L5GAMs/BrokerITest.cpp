/**
 * @file BrokerITest.cpp
 * @brief Source file for class BrokerITest
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
 * the class BrokerITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"
#include "BrokerITest.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "ErrorManagement.h"
#include "GAMSchedulerI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class BrokerITestScheduler1: public GAMSchedulerI {
public:CLASS_REGISTER_DECLARATION()

    BrokerITestScheduler1();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();

    virtual void CustomPrepareNextState();

};

BrokerITestScheduler1::BrokerITestScheduler1() :
        GAMSchedulerI() {

}

MARTe::ErrorManagement::ErrorType BrokerITestScheduler1::StartNextStateExecution() {
    return ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType BrokerITestScheduler1::StopCurrentStateExecution() {
    return ErrorManagement::NoError;
}

void BrokerITestScheduler1::CustomPrepareNextState() {

}

CLASS_REGISTER(BrokerITestScheduler1, "1.0")

class BrokerITestGAM1: public GAM {
public:CLASS_REGISTER_DECLARATION()

    BrokerITestGAM1();
    virtual bool Setup();
    virtual bool Execute();
    void* GetInputSignalsMemory();

    void* GetOutputSignalsMemory();

    void* GetInputSignalMemory(uint32 signalIdx);

    void* GetOutputSignalMemory(uint32 signalIdx);
};

BrokerITestGAM1::BrokerITestGAM1() :
        GAM() {

}
void* BrokerITestGAM1::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void* BrokerITestGAM1::BrokerITestGAM1::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

void* BrokerITestGAM1::BrokerITestGAM1::GetInputSignalMemory(uint32 signalIdx) {
    return GAM::GetInputSignalMemory(signalIdx);
}

void* BrokerITestGAM1::GetOutputSignalMemory(uint32 signalIdx) {
    return GAM::GetOutputSignalMemory(signalIdx);
}

bool BrokerITestGAM1::Setup() {
    return true;
}

bool BrokerITestGAM1::Execute() {
    return true;
}
CLASS_REGISTER(BrokerITestGAM1, "1.0")

class BrokerITestHelper: public BrokerI {
public:CLASS_REGISTER_DECLARATION()

    virtual bool Init(SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 *const functionName,
                      void *gamMemoryAddress);

    virtual bool Execute() {
        return true;
    }
};

bool BrokerITestHelper::Init(SignalDirection direction,
                             DataSourceI &dataSourceIn,
                             const char8 *const functionName,
                             void *gamMemoryAddress) {
    return InitFunctionPointers(direction, dataSourceIn, functionName, gamMemoryAddress);
}
CLASS_REGISTER(BrokerITestHelper, "1.0");

class BrokerIOptimTestHelper: public BrokerI {
public:CLASS_REGISTER_DECLARATION()

    virtual bool Init(SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 *const functionName,
                      void *gamMemoryAddress);

    virtual bool Execute() {
        return true;
    }
};

bool BrokerIOptimTestHelper::Init(SignalDirection direction,
                                  DataSourceI &dataSourceIn,
                                  const char8 *const functionName,
                                  void *gamMemoryAddress) {
    return InitFunctionPointersOptim(direction, dataSourceIn, functionName, gamMemoryAddress);
}
CLASS_REGISTER(BrokerIOptimTestHelper, "1.0");

/**
 * @brief DataSource helper similar to \a BrokerIDataSourceTestHelper but with interleaved memory (Signal1_sample1, Signal2_sample1, Signal1_sample2, Signal2_sample2...)
 */
class BrokerIDSInterleavedTestHelper: public DataSourceI {
public:CLASS_REGISTER_DECLARATION()

    BrokerIDSInterleavedTestHelper();

    virtual ~BrokerIDSInterleavedTestHelper();

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

BrokerIDSInterleavedTestHelper::BrokerIDSInterleavedTestHelper() :
        DataSourceI() {
    signalMemory = NULL_PTR(void*);
    offsets = NULL_PTR(uint32*);
    samples = 10;
}

BrokerIDSInterleavedTestHelper::~BrokerIDSInterleavedTestHelper() {
    if (signalMemory != NULL_PTR(void*)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemory);
    }
    if (offsets != NULL_PTR(uint32*)) {
        delete[] offsets;
    }
}

/*
 * @brief allocated dataSource memory using interleaved
 */
bool BrokerIDSInterleavedTestHelper::AllocateMemory() {
    uint32 numberOfSignals = GetNumberOfSignals();
    bool ret = (numberOfSignals > 0u);
    if (ret) {
        ret = (offsets == NULL_PTR(uint32*));
    }
    if (ret) {
        offsets = new uint32[numberOfSignals * samples];
    }
    uint32 totalMemorySize = 0u;
    for (uint32 sample = 0u; (sample < samples) && ret; sample++) {
        for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
            uint32 memorySize;
            ret = GetSignalByteSize(s, memorySize);
            if (ret) {
                offsets[s + sample * numberOfSignals] = totalMemorySize;
                totalMemorySize += memorySize;
            }
        }
    }
    if (ret) {
        signalMemory = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalMemorySize);
        MemoryOperationsHelper::Set(signalMemory, 0, totalMemorySize);
    }
    return ret;
}

uint32 BrokerIDSInterleavedTestHelper::GetNumberOfMemoryBuffers() {
    return 0u;
}

bool BrokerIDSInterleavedTestHelper::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                           const uint32 bufferIdx,
                                                           void *&signalAddress) {
    char8 *memPtr = reinterpret_cast<char8*>(signalMemory);
    memPtr += offsets[signalIdx];
    signalAddress = reinterpret_cast<void*&>(memPtr);
    return true;
}

const char8* BrokerIDSInterleavedTestHelper::GetBrokerName(StructuredDataI &data,
                                                           const SignalDirection direction) {
    if (direction == InputSignals) {
        return "BrokerITestHelper";
    }
    return "BrokerITestHelper";
}

bool BrokerIDSInterleavedTestHelper::PrepareNextState(const char8 *const currentStateName,
                                                      const char8 *const nextStateName) {
    return true;
}

bool BrokerIDSInterleavedTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                     const char8 *const functionName,
                                                     void *const gamMemPtr) {
    ReferenceT<BrokerITestHelper> broker("BrokerITestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool BrokerIDSInterleavedTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                      const char8 *const functionName,
                                                      void *const gamMemPtr) {
    ReferenceT<BrokerITestHelper> broker("BrokerITestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool BrokerIDSInterleavedTestHelper::Synchronise() {
    return false;
}

CLASS_REGISTER(BrokerIDSInterleavedTestHelper, "1.0");

/**
 * @brief DataSource helper similar to \a BrokerIDataSourceTestHelper but with interleaved memory (Signal1_sample1, Signal2_sample1, Signal1_sample2, Signal2_sample2...)
 * using the optim broker option
 */
class BrokerIDSInterleavedOptimTestHelper: public DataSourceI {
public:CLASS_REGISTER_DECLARATION()

    BrokerIDSInterleavedOptimTestHelper();

    virtual ~BrokerIDSInterleavedOptimTestHelper();

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

BrokerIDSInterleavedOptimTestHelper::BrokerIDSInterleavedOptimTestHelper() :
        DataSourceI() {
    signalMemory = NULL_PTR(void*);
    offsets = NULL_PTR(uint32*);
    samples = 10;
}

BrokerIDSInterleavedOptimTestHelper::~BrokerIDSInterleavedOptimTestHelper() {
    if (signalMemory != NULL_PTR(void*)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemory);
    }
    if (offsets != NULL_PTR(uint32*)) {
        delete[] offsets;
    }
}

/*
 * @brief allocated dataSource memory using interleaved
 */
bool BrokerIDSInterleavedOptimTestHelper::AllocateMemory() {
    uint32 numberOfSignals = GetNumberOfSignals();
    bool ret = (numberOfSignals > 0u);
    if (ret) {
        ret = (offsets == NULL_PTR(uint32*));
    }
    if (ret) {
        offsets = new uint32[numberOfSignals * samples];
    }
    uint32 totalMemorySize = 0u;
    for (uint32 sample = 0u; (sample < samples) && ret; sample++) {
        for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
            uint32 memorySize;
            ret = GetSignalByteSize(s, memorySize);
            if (ret) {
                offsets[s + sample * numberOfSignals] = totalMemorySize;
                totalMemorySize += memorySize;
            }
        }
    }
    if (ret) {
        signalMemory = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalMemorySize);
        MemoryOperationsHelper::Set(signalMemory, 0, totalMemorySize);
    }
    return ret;
}

uint32 BrokerIDSInterleavedOptimTestHelper::GetNumberOfMemoryBuffers() {
    return 0u;
}

bool BrokerIDSInterleavedOptimTestHelper::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                                const uint32 bufferIdx,
                                                                void *&signalAddress) {
    char8 *memPtr = reinterpret_cast<char8*>(signalMemory);
    memPtr += offsets[signalIdx];
    signalAddress = reinterpret_cast<void*&>(memPtr);
    return true;
}

const char8* BrokerIDSInterleavedOptimTestHelper::GetBrokerName(StructuredDataI &data,
                                                                const SignalDirection direction) {
    if (direction == InputSignals) {
        return "BrokerIOptimTestHelper";
    }
    return "BrokerIOptimTestHelper";
}

bool BrokerIDSInterleavedOptimTestHelper::PrepareNextState(const char8 *const currentStateName,
                                                           const char8 *const nextStateName) {
    return true;
}

bool BrokerIDSInterleavedOptimTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                          const char8 *const functionName,
                                                          void *const gamMemPtr) {
    ReferenceT<BrokerIOptimTestHelper> broker("BrokerIOptimTestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool BrokerIDSInterleavedOptimTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                           const char8 *const functionName,
                                                           void *const gamMemPtr) {
    ReferenceT<BrokerIOptimTestHelper> broker("BrokerIOptimTestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool BrokerIDSInterleavedOptimTestHelper::Synchronise() {
    return false;
}

CLASS_REGISTER(BrokerIDSInterleavedOptimTestHelper, "1.0");
/**
 *
 */
class BrokerIDataSourceTestHelper: public DataSourceI {
public:CLASS_REGISTER_DECLARATION()

    BrokerIDataSourceTestHelper();

    virtual ~BrokerIDataSourceTestHelper();

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
    //Store 10 samples per signal. keep in mind it is fast short-cut which will crash if in the configuration of the MARTe app the number of samples is larger than 10
    uint32 samples;

};

BrokerIDataSourceTestHelper::BrokerIDataSourceTestHelper() :
        DataSourceI() {
    signalMemory = NULL_PTR(void*);
    offsets = NULL_PTR(uint32*);
    samples = 10;
}

BrokerIDataSourceTestHelper::~BrokerIDataSourceTestHelper() {
    if (signalMemory != NULL_PTR(void*)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemory);
    }
    if (offsets != NULL_PTR(uint32*)) {
        delete[] offsets;
    }
}

bool BrokerIDataSourceTestHelper::AllocateMemory() {
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

uint32 BrokerIDataSourceTestHelper::GetNumberOfMemoryBuffers() {
    return 0u;
}

bool BrokerIDataSourceTestHelper::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                        const uint32 bufferIdx,
                                                        void *&signalAddress) {
    char8 *memPtr = reinterpret_cast<char8*>(signalMemory);
    memPtr += offsets[signalIdx];
    signalAddress = reinterpret_cast<void*&>(memPtr);
    return true;
}

const char8* BrokerIDataSourceTestHelper::GetBrokerName(StructuredDataI &data,
                                                        const SignalDirection direction) {
    if (direction == InputSignals) {
        return "BrokerITestHelper";
    }
    return "BrokerITestHelper";
}

bool BrokerIDataSourceTestHelper::PrepareNextState(const char8 *const currentStateName,
                                                   const char8 *const nextStateName) {
    return true;
}

bool BrokerIDataSourceTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                  const char8 *const functionName,
                                                  void *const gamMemPtr) {
    ReferenceT<BrokerITestHelper> broker("BrokerITestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool BrokerIDataSourceTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                   const char8 *const functionName,
                                                   void *const gamMemPtr) {
    ReferenceT<BrokerITestHelper> broker("BrokerITestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool BrokerIDataSourceTestHelper::Synchronise() {
    return false;
}

CLASS_REGISTER(BrokerIDataSourceTestHelper, "1.0");

/*
 *
 */

class BrokerIDataSourceOptimTestHelper: public DataSourceI {
public:CLASS_REGISTER_DECLARATION()

    BrokerIDataSourceOptimTestHelper();

    virtual ~BrokerIDataSourceOptimTestHelper();

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
    //Store 10 samples per signal. keep in mind it is fast short-cut which will crash if in the configuration of the MARTe app the number of samples is larger than 10
    uint32 samples;

};

BrokerIDataSourceOptimTestHelper::BrokerIDataSourceOptimTestHelper() :
        DataSourceI() {
    signalMemory = NULL_PTR(void*);
    offsets = NULL_PTR(uint32*);
    samples = 10;
}

BrokerIDataSourceOptimTestHelper::~BrokerIDataSourceOptimTestHelper() {
    if (signalMemory != NULL_PTR(void*)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemory);
    }
    if (offsets != NULL_PTR(uint32*)) {
        delete[] offsets;
    }
}

bool BrokerIDataSourceOptimTestHelper::AllocateMemory() {
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

uint32 BrokerIDataSourceOptimTestHelper::GetNumberOfMemoryBuffers() {
    return 0u;
}

bool BrokerIDataSourceOptimTestHelper::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                             const uint32 bufferIdx,
                                                             void *&signalAddress) {
    char8 *memPtr = reinterpret_cast<char8*>(signalMemory);
    memPtr += offsets[signalIdx];
    signalAddress = reinterpret_cast<void*&>(memPtr);
    return true;
}

const char8* BrokerIDataSourceOptimTestHelper::GetBrokerName(StructuredDataI &data,
                                                             const SignalDirection direction) {
    if (direction == InputSignals) {
        return "BrokerIOptimTestHelper";
    }
    return "BrokerIOptimTestHelper";
}

bool BrokerIDataSourceOptimTestHelper::PrepareNextState(const char8 *const currentStateName,
                                                        const char8 *const nextStateName) {
    return true;
}

bool BrokerIDataSourceOptimTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                       const char8 *const functionName,
                                                       void *const gamMemPtr) {
    ReferenceT<BrokerIOptimTestHelper> broker("BrokerIOptimTestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool BrokerIDataSourceOptimTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                        const char8 *const functionName,
                                                        void *const gamMemPtr) {
    ReferenceT<BrokerIOptimTestHelper> broker("BrokerIOptimTestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool BrokerIDataSourceOptimTestHelper::Synchronise() {
    return false;
}

CLASS_REGISTER(BrokerIDataSourceOptimTestHelper, "1.0");
/*
 *
 */
static bool InitialiseBrokerIEnviroment(const char8 *const config) {
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

static const char8 *const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = BrokerITestGAM1"
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
        "            Class = BrokerITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = BrokerITestGAM1"
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
        "            Class = BrokerITestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = BrokerITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = BrokerITestGAM1"
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
        "            Class = BrokerIDataSourceTestHelper"
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
        "        TimingDataSource = Timings"
        "        Class = BrokerITestScheduler1"
        "    }"
        "}";

static const char8 *const config2 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = BrokerITestGAM1"
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
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Default = 7"
        "                   Type = float32"
        "                   Samples = 10"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = BrokerITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = BrokerITestGAM1"
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
        "            Class = BrokerITestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = BrokerITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = BrokerITestGAM1"
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
        "            Class = BrokerIDSInterleavedTestHelper"
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
        "        TimingDataSource = Timings"
        "        Class = BrokerITestScheduler1"
        "    }"
        "}";
static const char8 *const config3 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = BrokerITestGAM1"
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
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Default = 7"
        "                   Type = float32"
        "                   Samples = 10"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = BrokerITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = BrokerITestGAM1"
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
        "            Class = BrokerITestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = BrokerITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = BrokerITestGAM1"
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
        "            Class = BrokerIDataSourceTestHelper"
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
        "        TimingDataSource = Timings"
        "        Class = BrokerITestScheduler1"
        "    }"
        "}";

static const char8 *const config4 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = BrokerITestGAM1"
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
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Default = 7"
        "                   Type = float32"
        "                   Samples = 10"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = BrokerITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = BrokerITestGAM1"
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
        "            Class = BrokerITestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = BrokerITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = BrokerITestGAM1"
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
        "            Class = BrokerIDataSourceOptimTestHelper"
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
        "        TimingDataSource = Timings"
        "        Class = BrokerITestScheduler1"
        "    }"
        "}";

static const char8 *const config5 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = BrokerITestGAM1"
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
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Default = 7"
        "                   Type = float32"
        "                   Samples = 10"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = BrokerITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = BrokerITestGAM1"
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
        "            Class = BrokerITestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = BrokerITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = BrokerITestGAM1"
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
        "            Class = BrokerIDSInterleavedOptimTestHelper"
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
        "        TimingDataSource = Timings"
        "        Class = BrokerITestScheduler1"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool BrokerITest::TestConstructor() {
    ReferenceT<BrokerITestHelper> broker("BrokerITestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->GetNumberOfCopies() == 0;
    }
    return ret;
}

bool BrokerITest::TestInitFunctionPointers() {
    bool ret = InitialiseBrokerIEnviroment(config1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> broker;
    ReferenceContainer brokers;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = dataSource->GetInputBrokers(brokers, "GAMA", (void*) 0xA0A0A0A0);
    }
    if (ret) {
        ret = (brokers.Size() > 0u);
    }
    if (ret) {
        broker = brokers.Get(0);
        ret = broker.IsValid();
    }

    return ret;
}

bool BrokerITest::TestGetNumberOfCopies() {
    bool ret = InitialiseBrokerIEnviroment(config1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> inputBroker;
    ReferenceT<BrokerITestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    uint32 numberOfCopiesInput[] = { 5, 1, 0, 0, 1, 0 };
    uint32 numberOfCopiesOutput[] = { 0, 0, 5, 1, 0, 1 };
    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            uint32 numberOfCopies = inputBroker->GetNumberOfCopies();
            ret = (numberOfCopies == numberOfCopiesInput[n]);
        }
        if (ret) {
            uint32 numberOfCopies = outputBroker->GetNumberOfCopies();
            ret = (numberOfCopies == numberOfCopiesOutput[n]);
        }
    }
    return ret;
}

bool BrokerITest::TestGetNumberOfCopiesOptim() {
    bool ret = InitialiseBrokerIEnviroment(config4);
    ReferenceT<BrokerIDataSourceOptimTestHelper> dataSource;
    ReferenceT<BrokerIOptimTestHelper> inputBroker;
    ReferenceT<BrokerIOptimTestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    uint32 numberOfCopiesInput[] = { 6, 1, 0, 0, 1, 0 };
    uint32 numberOfCopiesOutput[] = { 0, 0, 5, 1, 0, 1 };
    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            uint32 numberOfCopies = inputBroker->GetNumberOfCopies();
            ret = (numberOfCopies == numberOfCopiesInput[n]);
        }
        if (ret) {
            uint32 numberOfCopies = outputBroker->GetNumberOfCopies();
            ret = (numberOfCopies == numberOfCopiesOutput[n]);
        }
    }
    return ret;
}
#include <stdio.h>
bool BrokerITest::TestGetCopyByteSize() {
    bool ret = InitialiseBrokerIEnviroment(config1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> inputBroker;
    ReferenceT<BrokerITestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const uint32 maxNumberOfCopyByteSize = 5;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    uint32 copyByteSizeInput[][maxNumberOfCopyByteSize] = { { 4, 16, 4, 12, 4 }, { 4, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 4, 0, 0, 0, 0 }, {
            0, 0, 0, 0, 0 } };
    uint32 copyByteSizeOutput[][maxNumberOfCopyByteSize] = { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 4, 4, 16, 4, 12 }, { 4, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, {
            24, 0, 0, 0, 0 } };

    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            uint32 byteSize;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                byteSize = inputBroker->GetCopyByteSize(c);
                ret = (byteSize == copyByteSizeInput[n][c]);
            }
        }
        if (ret) {
            uint32 byteSize;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                byteSize = outputBroker->GetCopyByteSize(c);
                ret = (byteSize == copyByteSizeOutput[n][c]);
            }
        }
    }
    return ret;
}

bool BrokerITest::TestGetCopyByteSizeOptim() {
    bool ret = InitialiseBrokerIEnviroment(config4);
    ReferenceT<BrokerIDataSourceOptimTestHelper> dataSource;
    ReferenceT<BrokerIOptimTestHelper> inputBroker;
    ReferenceT<BrokerIOptimTestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const uint32 maxNumberOfCopyByteSize = 6;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    uint32 copyByteSizeInput[][maxNumberOfCopyByteSize] = { { 4, 16, 4, 12, 4, 40 }, { 4, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 4, 0,
            0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 } };
    uint32 copyByteSizeOutput[][maxNumberOfCopyByteSize] = { { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 4, 4, 16, 4, 12, 0 }, { 4, 0, 0, 0, 0, 0 }, { 0, 0, 0,
            0, 0, 0 }, { 24, 0, 0, 0, 0, 0 } };

    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            uint32 byteSize;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                byteSize = inputBroker->GetCopyByteSize(c);
                ret = (byteSize == copyByteSizeInput[n][c]);
            }
        }
        if (ret) {
            uint32 byteSize;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                byteSize = outputBroker->GetCopyByteSize(c);
                ret = (byteSize == copyByteSizeOutput[n][c]);
            }
        }
    }
    return ret;
}

bool BrokerITest::TestGetCopyOffset() {
    bool ret = InitialiseBrokerIEnviroment(config1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> inputBroker;
    ReferenceT<BrokerITestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const uint32 maxNumberOfCopyByteSize = 5;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    uint32 copyOffsetInput[][maxNumberOfCopyByteSize] = { { 0, 8, 36, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0,
            0, 0, 0, 0 } };
    uint32 copyOffsetOutput[][maxNumberOfCopyByteSize] = { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 8, 36, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0,
            0, 0, 0, 0 } };

    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            uint32 offset;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                offset = inputBroker->GetCopyOffset(c);
                ret = (offset == copyOffsetInput[n][c]);
            }
        }
        if (ret) {
            uint32 offset;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                offset = outputBroker->GetCopyOffset(c);
                ret = (offset == copyOffsetOutput[n][c]);
            }
        }
    }
    return ret;
}

bool BrokerITest::TestGetCopyOffsetOptim() {
    bool ret = InitialiseBrokerIEnviroment(config4);
    ReferenceT<BrokerIDataSourceOptimTestHelper> dataSource;
    ReferenceT<BrokerIOptimTestHelper> inputBroker;
    ReferenceT<BrokerIOptimTestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const uint32 maxNumberOfCopyByteSize = 6;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    uint32 copyOffsetInput[][maxNumberOfCopyByteSize] = { { 0, 8, 36, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0,
            0, 0 }, { 0, 0, 0, 0, 0, 0 } };
    uint32 copyOffsetOutput[][maxNumberOfCopyByteSize] = { { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 8, 36, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0,
            0, 0, 0 }, { 0, 0, 0, 0, 0, 0 } };

    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            uint32 offset;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                offset = inputBroker->GetCopyOffset(c);
                ret = (offset == copyOffsetInput[n][c]);
            }
        }
        if (ret) {
            uint32 offset;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                offset = outputBroker->GetCopyOffset(c);
                ret = (offset == copyOffsetOutput[n][c]);
            }
        }
    }
    return ret;
}

bool BrokerITest::TestGetFunctionPointer() {
    bool ret = InitialiseBrokerIEnviroment(config1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> inputBroker;
    ReferenceT<BrokerITestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const uint32 maxNumberOfCopyByteSize = 5;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };

    const char *basePointer = reinterpret_cast<const char*>(0xA);
    const char *pointerInput[][maxNumberOfCopyByteSize] = { { basePointer, basePointer + 4, basePointer + 20, basePointer + 24, basePointer + 40 }, {
            basePointer, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { basePointer, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } };

    const char *pointerOutput[][maxNumberOfCopyByteSize] = { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { basePointer, basePointer + 8, basePointer + 12, basePointer
            + 28, basePointer + 32 }, { basePointer, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { basePointer, 0, 0, 0, 0 } };

    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            void *functionPointer;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                functionPointer = inputBroker->GetFunctionPointer(c);
                ret = (functionPointer == pointerInput[n][c]);
            }
        }
        if (ret) {
            void *functionPointer;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                functionPointer = outputBroker->GetFunctionPointer(c);
                ret = (functionPointer == pointerOutput[n][c]);
            }
        }
    }
    return ret;
}

bool BrokerITest::TestGetFunctionPointerOptim() {
    bool ret = InitialiseBrokerIEnviroment(config4);
    ReferenceT<BrokerIDataSourceOptimTestHelper> dataSource;
    ReferenceT<BrokerIOptimTestHelper> inputBroker;
    ReferenceT<BrokerIOptimTestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const uint32 maxNumberOfCopyByteSize = 6;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };

    const char *basePointer = reinterpret_cast<const char*>(0xA);
    const char *pointerInput[][maxNumberOfCopyByteSize] = { { basePointer, basePointer + 4, basePointer + 20, basePointer + 24, basePointer + 40, basePointer
            + 44 }, { basePointer, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { basePointer, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 } };

    const char *pointerOutput[][maxNumberOfCopyByteSize] = { { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { basePointer, basePointer + 8, basePointer + 12,
            basePointer + 28, basePointer + 32, 0 }, { basePointer, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { basePointer, 0, 0, 0, 0, 0 } };

    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void*) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            void *functionPointer;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                functionPointer = inputBroker->GetFunctionPointer(c);
                ret = (functionPointer == pointerInput[n][c]);
            }
        }
        if (ret) {
            void *functionPointer;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                functionPointer = outputBroker->GetFunctionPointer(c);
                ret = (functionPointer == pointerOutput[n][c]);
            }
        }
    }
    return ret;
}

bool BrokerITest::TestGetOwnerFunctionName() {
    bool ret = InitialiseBrokerIEnviroment(config1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> broker;
    ReferenceContainer brokers;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = dataSource->GetInputBrokers(brokers, "GAMA", (void*) 0xA0A0A0A0);
    }
    if (ret) {
        ret = (brokers.Size() > 0u);
    }
    if (ret) {
        broker = brokers.Get(0);
        ret = broker.IsValid();
    }
    if (ret) {
        ret = (broker->GetOwnerFunctionName() == "GAMA");
    }

    return ret;
}

bool BrokerITest::TestGetOwnerDataSourceName() {
    bool ret = InitialiseBrokerIEnviroment(config1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> broker;
    ReferenceContainer brokers;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = dataSource->GetInputBrokers(brokers, "GAMA", (void*) 0xA0A0A0A0);
    }
    if (ret) {
        ret = (brokers.Size() > 0u);
    }
    if (ret) {
        broker = brokers.Get(0);
        ret = broker.IsValid();
    }
    if (ret) {
        ret = (broker->GetOwnerDataSourceName() == "Drv1");
    }

    return ret;
}

bool BrokerITest::TestInterleavedDSMemory() {
    bool ret = InitialiseBrokerIEnviroment(config2);
    ReferenceT<BrokerIDSInterleavedTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> broker;
    ReferenceT<BrokerITestGAM1> gamA;
    ReferenceContainer brokers;
    uint32 nOfCopiesAux = 0;
    uint32 copyByteSizeAux = 0;
    uint32 copyOffsetAux = 0;
    char8 *dataSourcePtr = NULL_PTR(char8*);
    uint32 copyByteSizeAux2 = 0;
    uint32 copyOffsetAux2 = 0;
    char8 *dataSourcePtr2 = NULL_PTR(char8*);

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
    if (ret) {
        nOfCopiesAux = broker->GetNumberOfCopies();
        for (uint32 copyIdx = 0u; (copyIdx < nOfCopiesAux) && ret; copyIdx++) {
            copyByteSizeAux = broker->GetCopyByteSize(copyIdx);
            copyOffsetAux = broker->GetCopyOffset(copyIdx);
            dataSource->GetSignalMemoryBuffer(broker->GetDSCopySignalIndex(copyIdx), 0, reinterpret_cast<void*&>(dataSourcePtr));
            for (uint32 copyIdx2 = copyIdx + 1; (copyIdx2 < nOfCopiesAux) && ret; copyIdx2++) {
                copyByteSizeAux2 = broker->GetCopyByteSize(copyIdx2);
                copyOffsetAux2 = broker->GetCopyOffset(copyIdx2);
                dataSource->GetSignalMemoryBuffer(broker->GetDSCopySignalIndex(copyIdx2), 0, reinterpret_cast<void*&>(dataSourcePtr2));
                if (&dataSourcePtr[copyOffsetAux] > &dataSourcePtr2[copyOffsetAux2]) {
                    ret = &dataSourcePtr2[copyOffsetAux2 + copyByteSizeAux2] <= &dataSourcePtr[copyOffsetAux];
                }
                else {
                    ret = &dataSourcePtr[copyOffsetAux + copyByteSizeAux] <= &dataSourcePtr2[copyOffsetAux2];
                }
            }
        }
    }

    return ret;
}

bool BrokerITest::TestInterleavedDSMemoryOptim() {
    bool ret = InitialiseBrokerIEnviroment(config5);
    ReferenceT<BrokerIDSInterleavedOptimTestHelper> dataSource;
    ReferenceT<BrokerIOptimTestHelper> broker;
    ReferenceT<BrokerITestGAM1> gamA;
    ReferenceContainer brokers;
    uint32 nOfCopiesAux = 0;
    uint32 copyByteSizeAux = 0;
    uint32 copyOffsetAux = 0;
    char8 *dataSourcePtr = NULL_PTR(char8*);
    uint32 copyByteSizeAux2 = 0;
    uint32 copyOffsetAux2 = 0;
    char8 *dataSourcePtr2 = NULL_PTR(char8*);

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
    if (ret) {
        nOfCopiesAux = broker->GetNumberOfCopies();
        for (uint32 copyIdx = 0u; (copyIdx < nOfCopiesAux) && ret; copyIdx++) {
            copyByteSizeAux = broker->GetCopyByteSize(copyIdx);
            copyOffsetAux = broker->GetCopyOffset(copyIdx);
            dataSource->GetSignalMemoryBuffer(broker->GetDSCopySignalIndex(copyIdx), 0, reinterpret_cast<void*&>(dataSourcePtr));
            for (uint32 copyIdx2 = copyIdx + 1; (copyIdx2 < nOfCopiesAux) && ret; copyIdx2++) {
                copyByteSizeAux2 = broker->GetCopyByteSize(copyIdx2);
                copyOffsetAux2 = broker->GetCopyOffset(copyIdx2);
                dataSource->GetSignalMemoryBuffer(broker->GetDSCopySignalIndex(copyIdx2), 0, reinterpret_cast<void*&>(dataSourcePtr2));
                if (&dataSourcePtr[copyOffsetAux] > &dataSourcePtr2[copyOffsetAux2]) {
                    ret = &dataSourcePtr2[copyOffsetAux2 + copyByteSizeAux2] <= &dataSourcePtr[copyOffsetAux];
                }
                else {
                    ret = &dataSourcePtr[copyOffsetAux + copyByteSizeAux] <= &dataSourcePtr2[copyOffsetAux2];
                }
            }
        }
    }

    return ret;
}

bool BrokerITest::TestFlatDSMemory() {
    bool ret = InitialiseBrokerIEnviroment(config3);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> broker;
    ReferenceT<BrokerITestGAM1> gamA;
    ReferenceContainer brokers;
    uint32 nOfCopiesAux = 0;
    uint32 copyByteSizeAux = 0;
    uint32 copyOffsetAux = 0;
    char8 *dataSourcePtr = NULL_PTR(char8*);
    uint32 copyByteSizeAux2 = 0;
    uint32 copyOffsetAux2 = 0;
    char8 *dataSourcePtr2 = NULL_PTR(char8*);

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
    if (ret) {
        nOfCopiesAux = broker->GetNumberOfCopies();
        for (uint32 copyIdx = 0u; (copyIdx < nOfCopiesAux) && ret; copyIdx++) {
            copyByteSizeAux = broker->GetCopyByteSize(copyIdx);
            copyOffsetAux = broker->GetCopyOffset(copyIdx);
            dataSource->GetSignalMemoryBuffer(broker->GetDSCopySignalIndex(copyIdx), 0, reinterpret_cast<void*&>(dataSourcePtr));
            for (uint32 copyIdx2 = copyIdx + 1; (copyIdx2 < nOfCopiesAux) && ret; copyIdx2++) {
                copyByteSizeAux2 = broker->GetCopyByteSize(copyIdx2);
                copyOffsetAux2 = broker->GetCopyOffset(copyIdx2);
                dataSource->GetSignalMemoryBuffer(broker->GetDSCopySignalIndex(copyIdx2), 0, reinterpret_cast<void*&>(dataSourcePtr2));
                if (&dataSourcePtr[copyOffsetAux] > &dataSourcePtr2[copyOffsetAux2]) {
                    ret = &dataSourcePtr2[copyOffsetAux2 + copyByteSizeAux2] <= &dataSourcePtr[copyOffsetAux];
                }
                else {
                    ret = &dataSourcePtr[copyOffsetAux + copyByteSizeAux] <= &dataSourcePtr2[copyOffsetAux2];
                }
            }
        }
    }

    return ret;
}

