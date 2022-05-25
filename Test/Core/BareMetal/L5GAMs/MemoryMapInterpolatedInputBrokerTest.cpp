/**
 * @file MemoryMapInterpolatedInputBrokerTest.cpp
 * @brief Source file for class MemoryMapInterpolatedInputBrokerTest
 * @date 24/08/2017
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
 * the class MemoryMapInterpolatedInputBrokerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"
#include "MemoryMapInterpolatedInputBrokerTest.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "MemoryMapInterpolatedInputBroker.h"
#include "MemoryMapInterpolatedInputBrokerTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;
/**
 * @brief GAMSchedulerI empty implementation to support the MemoryMapInterpolatedInputBroker tests
 */
class MemoryMapInterpolatedInputBrokerTestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapInterpolatedInputBrokerTestScheduler1    ();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();

    virtual void CustomPrepareNextState();

};

MemoryMapInterpolatedInputBrokerTestScheduler1::MemoryMapInterpolatedInputBrokerTestScheduler1() :
        GAMSchedulerI() {

}

MARTe::ErrorManagement::ErrorType MemoryMapInterpolatedInputBrokerTestScheduler1::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType MemoryMapInterpolatedInputBrokerTestScheduler1::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

void MemoryMapInterpolatedInputBrokerTestScheduler1::CustomPrepareNextState() {

}

CLASS_REGISTER(MemoryMapInterpolatedInputBrokerTestScheduler1, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapInterpolatedInputBroker tests
 */
class MemoryMapInterpolatedInputBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapInterpolatedInputBrokerTestGAM1    ();

    void *GetInputSignalsMemory();

    void *GetOutputSignalsMemory();

    void *GetInputSignalMemory(uint32 signalIdx);

    void *GetOutputSignalMemory(uint32 signalIdx);

    virtual bool Setup();

    virtual bool Execute();
};

MemoryMapInterpolatedInputBrokerTestGAM1::MemoryMapInterpolatedInputBrokerTestGAM1() :
        GAM() {
}

void *MemoryMapInterpolatedInputBrokerTestGAM1::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void *MemoryMapInterpolatedInputBrokerTestGAM1::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

void *MemoryMapInterpolatedInputBrokerTestGAM1::GetInputSignalMemory(uint32 signalIdx) {
    return GAM::GetInputSignalMemory(signalIdx);
}

void *MemoryMapInterpolatedInputBrokerTestGAM1::GetOutputSignalMemory(uint32 signalIdx) {
    return GAM::GetOutputSignalMemory(signalIdx);
}

bool MemoryMapInterpolatedInputBrokerTestGAM1::Setup() {
    return true;
}

bool MemoryMapInterpolatedInputBrokerTestGAM1::Execute() {
    return true;
}

CLASS_REGISTER(MemoryMapInterpolatedInputBrokerTestGAM1, "1.0")

/**
 * @brief DataSourceI implementation which returns a MemoryMapInterpolatedInputBroker broker
 * and which offers a linear memory back-end that increment the value of each signal depending on the signal type.
 */
class MemoryMapInterpolatedInputBrokerDataSourceTestHelper: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapInterpolatedInputBrokerDataSourceTestHelper    ();

    virtual ~ MemoryMapInterpolatedInputBrokerDataSourceTestHelper();

    virtual bool AllocateMemory();

    virtual bool Initialise(StructuredDataI & data);

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

    void Init();

    void *signalMemory;
    TypeDescriptor *signalTypes;
    uint32 *nElements;
    uint32 *offsets;
    uint32 maxSynchronises;
    uint32 synchCounter;
    uint64 timeIncrement;
    uint64 timeSignal;
    uint64 startTime;
    uint32 intIncrement;
    uint32 initialIntValue;
    float32 floatIncrement;
    float32 initialFloatValue;
    uint64 interpolationPeriod;
};

MemoryMapInterpolatedInputBrokerDataSourceTestHelper::MemoryMapInterpolatedInputBrokerDataSourceTestHelper() :
        DataSourceI() {
    signalMemory = NULL_PTR(void *);
    offsets = NULL_PTR(uint32 *);
    signalTypes = NULL_PTR(TypeDescriptor *);
    nElements = NULL_PTR(uint32 *);
    timeIncrement = 5;
    intIncrement = 10;
    floatIncrement = 0.1345;
    interpolationPeriod = 10;
    maxSynchronises = 8;
    startTime = 5;
    timeSignal = startTime;
    synchCounter = 0;
    initialIntValue = 0;
    initialFloatValue = 0;
}

MemoryMapInterpolatedInputBrokerDataSourceTestHelper::~MemoryMapInterpolatedInputBrokerDataSourceTestHelper() {
    if (signalMemory != NULL_PTR(void *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(signalMemory);
    }
    if (offsets != NULL_PTR(uint32 *)) {
        delete[] offsets;
    }
    if (nElements != NULL_PTR(uint32 *)) {
        delete[] nElements;
    }
    if (signalTypes != NULL_PTR(TypeDescriptor *)) {
        delete[] signalTypes;
    }
}

/**
 * @brief Initialise parameters
 */
bool MemoryMapInterpolatedInputBrokerDataSourceTestHelper::Initialise(StructuredDataI & data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("IntIncrement", intIncrement);
    }
    if (ok) {
        ok = data.Read("FloatIncrement", floatIncrement);
    }
    if (ok) {
        ok = data.Read("TimeIncrement", timeIncrement);
    }
    if (ok) {
        ok = data.Read("InterpolationPeriod", interpolationPeriod);
    }
    return ok;
}

/**
 * @brief Allocates the memory for all the signals
 */
bool MemoryMapInterpolatedInputBrokerDataSourceTestHelper::AllocateMemory() {
    uint32 numberOfSignals = GetNumberOfSignals();
    bool ret = (numberOfSignals > 0u);
    if (ret) {
        ret = (offsets == NULL_PTR(uint32 *));
    }
    if (ret) {
        offsets = new uint32[numberOfSignals];
        nElements = new uint32[numberOfSignals];
        signalTypes = new TypeDescriptor[numberOfSignals];
    }
    uint32 totalMemorySize = 0u;
    for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
        uint32 memorySize;
        ret = GetSignalByteSize(s, memorySize);
        if (ret) {
            offsets[s] = totalMemorySize;
            totalMemorySize += memorySize;
            signalTypes[s] = GetSignalType(s);
            uint32 typeSize = signalTypes[s].numberOfBits / 8u;
            nElements[s] = memorySize / typeSize;
        }
    }
    if (ret) {
        signalMemory = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalMemorySize);
        MemoryOperationsHelper::Set(signalMemory, 0, totalMemorySize);
    }
    return ret;
}

uint32 MemoryMapInterpolatedInputBrokerDataSourceTestHelper::GetNumberOfMemoryBuffers() {
    return 1u;
}

/**
 * @brief Returns the correct memory buffer and generates a pattern where the signal
 * address byte N contains the value N*N
 */
bool MemoryMapInterpolatedInputBrokerDataSourceTestHelper::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    char8 *memPtr = reinterpret_cast<char8 *>(signalMemory);
    memPtr += offsets[signalIdx];
    signalAddress = reinterpret_cast<void *&>(memPtr);
    return true;
}

const char8 * MemoryMapInterpolatedInputBrokerDataSourceTestHelper::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapInterpolatedInputBroker";
    }
    return "MemoryMapInterpolatedInputBroker";
}

bool MemoryMapInterpolatedInputBrokerDataSourceTestHelper::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}

bool MemoryMapInterpolatedInputBrokerDataSourceTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr) {
    ReferenceT<MemoryMapInterpolatedInputBroker> broker("MemoryMapInterpolatedInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        broker->SetIndependentVariable(&timeSignal, interpolationPeriod);
    }
    if (ret) {
        broker->Reset();
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    Init();
    return ret;
}

bool MemoryMapInterpolatedInputBrokerDataSourceTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr) {
    return true;
}

void MemoryMapInterpolatedInputBrokerDataSourceTestHelper::Init() {
    uint32 s;
    uint32 nSignals = GetNumberOfSignals();
    for (s = 0u; (s < nSignals); s++) {
        char8 *memPtr = reinterpret_cast<char8 *>(signalMemory);
        memPtr += offsets[s];
        uint32 n;
        if (signalTypes[s] == UnsignedInteger8Bit) {
            uint8 *signal = reinterpret_cast<uint8 *>(memPtr);
            for (n = 0u; (n < nElements[s]); n++) {
                uint8 val = initialIntValue;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(uint8));
            }
        }
        else if (signalTypes[s] == UnsignedInteger16Bit) {
            uint16 *signal = reinterpret_cast<uint16 *>(memPtr);
            for (n = 0u; (n < nElements[s]); n++) {
                uint16 val = initialIntValue;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(uint16));
            }
        }
        else if (signalTypes[s] == UnsignedInteger32Bit) {
            uint32 *signal = reinterpret_cast<uint32 *>(memPtr);
            for (n = 0u; (n < nElements[s]); n++) {
                uint32 val = initialIntValue;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(uint32));
            }
        }
        else if (signalTypes[s] == UnsignedInteger64Bit) {
            uint64 *signal = reinterpret_cast<uint64 *>(memPtr);
            for (n = 0u; (n < nElements[s]); n++) {
                uint64 val = initialIntValue;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(uint64));
            }
        }
        else if (signalTypes[s] == SignedInteger8Bit) {
            int8 *signal = reinterpret_cast<int8 *>(memPtr);
            for (n = 0u; (n < nElements[s]); n++) {
                int8 val = initialIntValue;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(int8));
            }
        }
        else if (signalTypes[s] == SignedInteger16Bit) {
            int16 *signal = reinterpret_cast<int16 *>(memPtr);
            for (n = 0u; (n < nElements[s]); n++) {
                int16 val = initialIntValue;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(int16));
            }
        }
        else if (signalTypes[s] == SignedInteger32Bit) {
            int32 *signal = reinterpret_cast<int32 *>(memPtr);
            for (n = 0u; (n < nElements[s]); n++) {
                int32 val = initialIntValue;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(int32));
            }
        }
        else if (signalTypes[s] == SignedInteger64Bit) {
            int64 *signal = reinterpret_cast<int64 *>(memPtr);
            for (n = 0u; (n < nElements[s]); n++) {
                int64 val = initialIntValue;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(int64));
            }
        }
        else if (signalTypes[s] == Float32Bit) {
            float32 *signal = reinterpret_cast<float32 *>(memPtr);
            for (n = 0u; (n < nElements[s]); n++) {
                float32 val = initialFloatValue;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(float32));
            }
        }
        else if (signalTypes[s] == Float64Bit) {
            float64 *signal = reinterpret_cast<float64 *>(memPtr);
            for (n = 0u; (n < nElements[s]); n++) {
                float64 val = initialFloatValue;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(float64));
            }
        }
    }
}

bool MemoryMapInterpolatedInputBrokerDataSourceTestHelper::Synchronise() {
    bool ret = true;
    uint32 s;
    uint32 nSignals = GetNumberOfSignals();
    if (synchCounter < maxSynchronises) {
        timeSignal += timeIncrement;
    }
    synchCounter++;
    for (s = 0u; (s < nSignals) && (ret); s++) {
        char8 *memPtr = reinterpret_cast<char8 *>(signalMemory);
        memPtr += offsets[s];
        uint32 n;
        if (signalTypes[s] == UnsignedInteger8Bit) {
            uint8 *signal = reinterpret_cast<uint8 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                uint8 val;
                MemoryOperationsHelper::Copy(&val, &signal[n], sizeof(uint8));
                val += intIncrement;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(uint8));
            }
        }
        else if (signalTypes[s] == UnsignedInteger16Bit) {
            uint16 *signal = reinterpret_cast<uint16 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                uint16 val;
                MemoryOperationsHelper::Copy(&val, &signal[n], sizeof(uint16));
                val += intIncrement;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(uint16));
            }
        }
        else if (signalTypes[s] == UnsignedInteger32Bit) {
            uint32 *signal = reinterpret_cast<uint32 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                uint32 val;
                MemoryOperationsHelper::Copy(&val, &signal[n], sizeof(uint32));
                val += intIncrement;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(uint32));
            }
        }
        else if (signalTypes[s] == UnsignedInteger64Bit) {
            uint64 *signal = reinterpret_cast<uint64 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                uint64 val;
                MemoryOperationsHelper::Copy(&val, &signal[n], sizeof(uint64));
                val += intIncrement;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(uint64));
            }
        }
        else if (signalTypes[s] == SignedInteger8Bit) {
            int8 *signal = reinterpret_cast<int8 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                int8 val;
                MemoryOperationsHelper::Copy(&val, &signal[n], sizeof(int8));
                val += intIncrement;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(int8));
            }
        }
        else if (signalTypes[s] == SignedInteger16Bit) {
            int16 *signal = reinterpret_cast<int16 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                int16 val;
                MemoryOperationsHelper::Copy(&val, &signal[n], sizeof(int16));
                val += intIncrement;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(int16));
            }
        }
        else if (signalTypes[s] == SignedInteger32Bit) {
            int32 *signal = reinterpret_cast<int32 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                int32 val;
                MemoryOperationsHelper::Copy(&val, &signal[n], sizeof(int32));
                val += intIncrement;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(int32));
            }
        }
        else if (signalTypes[s] == SignedInteger64Bit) {
            int64 *signal = reinterpret_cast<int64 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                int64 val;
                MemoryOperationsHelper::Copy(&val, &signal[n], sizeof(int64));
                val += intIncrement;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(int64));
            }
        }
        else if (signalTypes[s] == Float32Bit) {
            float32 *signal = reinterpret_cast<float32 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                float32 val;
                MemoryOperationsHelper::Copy(&val, &signal[n], sizeof(float32));
                val += floatIncrement;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(float32));
            }
        }
        else if (signalTypes[s] == Float64Bit) {
            float64 *signal = reinterpret_cast<float64 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                float64 val;
                MemoryOperationsHelper::Copy(&val, &signal[n], sizeof(float64));
                val += floatIncrement;
                MemoryOperationsHelper::Copy(&signal[n], &val, sizeof(float64));
            }
        }
    }
    return ret;
}
CLASS_REGISTER(MemoryMapInterpolatedInputBrokerDataSourceTestHelper, "1.0");

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapInterpolatedInputBrokerEnviroment(const char8 * const config) {
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
 * MARTe configuration structure to test the MemoryMapInterpolatedInputBroker
 */
static const char8 * const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapInterpolatedInputBrokerTestGAM1"
        "            InputSignals = {"
        "               SignalUInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "               }"
        "               SignalUInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               SignalUInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = uint16"
        "               }"
        "               SignalUInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               SignalInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = int8"
        "               }"
        "               SignalInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = int16"
        "               }"
        "               SignalInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = int32"
        "               }"
        "               SignalInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = int64"
        "               }"
        "               SignalFloat32 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "               SignalFloat64 = {"
        "                   DataSource = Drv1"
        "                   Type = float64"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = MemoryMapInterpolatedInputBrokerDataSourceTestHelper"
        "            IntIncrement = 10"
        "            FloatIncrement = 0.4"
        "            InterpolationPeriod = 2"
        "            TimeIncrement = 5"
        "            Signals = {"
        "               SignalUInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "               }"
        "               SignalUInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               SignalUInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = uint16"
        "               }"
        "               SignalUInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               SignalInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = int8"
        "               }"
        "               SignalInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = int16"
        "               }"
        "               SignalInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = int32"
        "               }"
        "               SignalInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = int64"
        "               }"
        "               SignalFloat32 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "               SignalFloat64 = {"
        "                   DataSource = Drv1"
        "                   Type = float64"
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
        "        Class = MemoryMapInterpolatedInputBrokerTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//As config1 but with under-sampling
static const char8 * const config2 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapInterpolatedInputBrokerTestGAM1"
        "            InputSignals = {"
        "               SignalUInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "               }"
        "               SignalUInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               SignalUInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = uint16"
        "               }"
        "               SignalUInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               SignalInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = int8"
        "               }"
        "               SignalInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = int16"
        "               }"
        "               SignalInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = int32"
        "               }"
        "               SignalInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = int64"
        "               }"
        "               SignalFloat32 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "               SignalFloat64 = {"
        "                   DataSource = Drv1"
        "                   Type = float64"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = MemoryMapInterpolatedInputBrokerDataSourceTestHelper"
        "            IntIncrement = 10"
        "            FloatIncrement = 0.4"
        "            InterpolationPeriod = 10"
        "            TimeIncrement = 5"
        "            Signals = {"
        "               SignalUInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "               }"
        "               SignalUInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               SignalUInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = uint16"
        "               }"
        "               SignalUInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               SignalInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = int8"
        "               }"
        "               SignalInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = int16"
        "               }"
        "               SignalInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = int32"
        "               }"
        "               SignalInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = int64"
        "               }"
        "               SignalFloat32 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "               SignalFloat64 = {"
        "                   DataSource = Drv1"
        "                   Type = float64"
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
        "        Class = MemoryMapInterpolatedInputBrokerTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

/**
 * As config1 but with arrays
 */
static const char8 * const config3 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapInterpolatedInputBrokerTestGAM1"
        "            InputSignals = {"
        "               SignalUInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "                   NumberOfElements = 10"
        "               }"
        "               SignalUInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfElements = 14"
        "               }"
        "               SignalUInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = uint16"
        "                   NumberOfElements = 2"
        "               }"
        "               SignalUInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 3"
        "               }"
        "               SignalInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = int8"
        "                   NumberOfElements = 8"
        "               }"
        "               SignalInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = int16"
        "                   NumberOfElements = 5"
        "               }"
        "               SignalInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = int32"
        "                   NumberOfElements = 6"
        "               }"
        "               SignalInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = int64"
        "                   NumberOfElements = 3"
        "               }"
        "               SignalFloat32 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "                   NumberOfElements = 5"
        "               }"
        "               SignalFloat64 = {"
        "                   DataSource = Drv1"
        "                   Type = float64"
        "                   NumberOfElements = 5"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = MemoryMapInterpolatedInputBrokerDataSourceTestHelper"
        "            IntIncrement = 10"
        "            FloatIncrement = 0.4"
        "            InterpolationPeriod = 2"
        "            TimeIncrement = 5"
        "            Signals = {"
        "               SignalUInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "               }"
        "               SignalUInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               SignalUInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = uint16"
        "               }"
        "               SignalUInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               SignalInt8 = {"
        "                   DataSource = Drv1"
        "                   Type = int8"
        "               }"
        "               SignalInt16 = {"
        "                   DataSource = Drv1"
        "                   Type = int16"
        "               }"
        "               SignalInt32 = {"
        "                   DataSource = Drv1"
        "                   Type = int32"
        "               }"
        "               SignalInt64 = {"
        "                   DataSource = Drv1"
        "                   Type = int64"
        "               }"
        "               SignalFloat32 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "               SignalFloat64 = {"
        "                   DataSource = Drv1"
        "                   Type = float64"
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
        "        Class = MemoryMapInterpolatedInputBrokerTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

/**
 * Check the expected value already knowing that the MemoryMapInterpolatedInputBrokerDataSourceTestHelper is generating an increasing monotonic ramp with constant derivative
 */
template<typename T>
static bool CheckExpectedValue(uint64 executeNumber, uint32 valueIncrement, uint32 initValue, uint64 timeIncrement, uint64 interpolationPeriod, uint32 nElements, void *gamPtr) {
    T *signal = static_cast<T *>(gamPtr);
    float64 m = static_cast<float64>(valueIncrement) / static_cast<float64>(timeIncrement);

    T expectedValue = static_cast<T>(m * interpolationPeriod * executeNumber + initValue);

    bool ret = true;
    uint32 n;
    for (n = 0u; (n < nElements) && (ret); n++) {
	T sn;
	MemoryOperationsHelper::Copy(&sn, &signal[n], sizeof(T));
        ret = (sn == expectedValue);
    }

    return ret;
}

template<typename T>
static bool CheckExpectedValueD(uint64 executeNumber, float32 valueIncrement, float32 initValue, uint64 timeIncrement, uint64 interpolationPeriod, uint32 nElements, void *gamPtr) {
    T *signal = static_cast<T *>(gamPtr);
    T expectedValue = valueIncrement * executeNumber;
    float64 dt = static_cast<float64>(interpolationPeriod);
    dt /= static_cast<float64>(timeIncrement);
    expectedValue *= dt;
    expectedValue += initValue;

    bool ret = true;
    uint32 n;
    for (n = 0u; (n < nElements) && (ret); n++) {
	T ft; 
	MemoryOperationsHelper::Copy(&ft, &signal[n], sizeof(T));
        float64 f1 = ft;
        float64 f2 = static_cast<float32>(expectedValue);
        float64 min = 1e-6;
        ret = ((f1 - f2) < (min)) && ((f1 - f2) > -(min));
    }

    return ret;
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool MemoryMapInterpolatedInputBrokerTest::TestConstructor() {
    using namespace MARTe;
    ReferenceT<MemoryMapInterpolatedInputBroker> broker("MemoryMapInterpolatedInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->GetNumberOfCopies() == 0;
    }
    return ret;
}

bool MemoryMapInterpolatedInputBrokerTest::TestExecute_1() {
    return TestExecuteP(config1);
}

bool MemoryMapInterpolatedInputBrokerTest::TestExecute_2() {
    return TestExecuteP(config2);
}

bool MemoryMapInterpolatedInputBrokerTest::TestExecute_3() {
    return TestExecuteP(config3);
}

bool MemoryMapInterpolatedInputBrokerTest::TestExecute_False() {
    return !TestExecuteP(config2, 6);
}

bool MemoryMapInterpolatedInputBrokerTest::TestInit() {
    return TestExecuteP(config1);
}

bool MemoryMapInterpolatedInputBrokerTest::TestReset() {
    return TestExecuteP(config1);
}

bool MemoryMapInterpolatedInputBrokerTest::TestSetIndependentVariable() {
    return TestExecuteP(config1);
}

bool MemoryMapInterpolatedInputBrokerTest::TestExecuteP(StreamString config1, uint32 nExecutes) {
    bool ret = InitialiseMemoryMapInterpolatedInputBrokerEnviroment(config1.Buffer());
    ReferenceT<MemoryMapInterpolatedInputBrokerDataSourceTestHelper> dataSource;
    ReferenceT<MemoryMapInterpolatedInputBroker> broker;
    ReferenceT<MemoryMapInterpolatedInputBrokerTestGAM1> gamA;
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
        ret = (numberOfCopies == dataSource->GetNumberOfSignals());
    }

    //Verify if the GAM has the expected pattern
    uint32 nOfSignals = gamA->GetNumberOfInputSignals();
    uint32 s;
    uint32 i;
    uint64 e;

    for (i = 0; i < 2; i++) {
        if (i == 1) {
            dataSource->synchCounter = 0;
            dataSource->startTime = 1000;
            dataSource->timeSignal = dataSource->startTime;
            dataSource->initialIntValue = 4;
            dataSource->initialFloatValue = 2;
            dataSource->Init();
            broker->Reset();
        }

        for (e = 0; (e < nExecutes) && (ret); e++) {
            ret = broker->Execute();
            for (s = 0; (s < nOfSignals) && (ret); s++) {
                void *gamPtr = gamA->GetInputSignalMemory(s);
                uint32 nElements;
                gamA->GetSignalNumberOfElements(InputSignals, s, nElements);
                TypeDescriptor signalType = gamA->GetSignalType(InputSignals, s);
                if (signalType == UnsignedInteger8Bit) {
                    ret = CheckExpectedValue<uint8>(e, dataSource->intIncrement, dataSource->initialIntValue, dataSource->timeIncrement, dataSource->interpolationPeriod, nElements, gamPtr);
                }
                else if (signalType == UnsignedInteger16Bit) {
                    ret = CheckExpectedValue<uint16>(e, dataSource->intIncrement, dataSource->initialIntValue, dataSource->timeIncrement, dataSource->interpolationPeriod, nElements, gamPtr);
                }
                else if (signalType == UnsignedInteger32Bit) {
                    ret = CheckExpectedValue<uint32>(e, dataSource->intIncrement, dataSource->initialIntValue, dataSource->timeIncrement, dataSource->interpolationPeriod, nElements, gamPtr);
                }
                else if (signalType == UnsignedInteger64Bit) {
                    ret = CheckExpectedValue<uint64>(e, dataSource->intIncrement, dataSource->initialIntValue, dataSource->timeIncrement, dataSource->interpolationPeriod, nElements, gamPtr);
                }
                else if (signalType == SignedInteger8Bit) {
                    ret = CheckExpectedValue<int8>(e, dataSource->intIncrement, dataSource->initialIntValue, dataSource->timeIncrement, dataSource->interpolationPeriod, nElements, gamPtr);
                }
                else if (signalType == SignedInteger16Bit) {
                    ret = CheckExpectedValue<int16>(e, dataSource->intIncrement, dataSource->initialIntValue, dataSource->timeIncrement, dataSource->interpolationPeriod, nElements, gamPtr);
                }
                else if (signalType == SignedInteger32Bit) {
                    ret = CheckExpectedValue<int32>(e, dataSource->intIncrement, dataSource->initialIntValue, dataSource->timeIncrement, dataSource->interpolationPeriod, nElements, gamPtr);
                }
                else if (signalType == SignedInteger64Bit) {
                    ret = CheckExpectedValue<int64>(e, dataSource->intIncrement, dataSource->initialIntValue, dataSource->timeIncrement, dataSource->interpolationPeriod, nElements, gamPtr);
                }
                else if (signalType == Float32Bit) {
                    ret = CheckExpectedValueD<float32>(e, dataSource->floatIncrement, dataSource->initialFloatValue, dataSource->timeIncrement, dataSource->interpolationPeriod, nElements, gamPtr);
                }
                else if (signalType == Float64Bit) {
                    ret = CheckExpectedValueD<float64>(e, dataSource->floatIncrement, dataSource->initialFloatValue, dataSource->timeIncrement, dataSource->interpolationPeriod, nElements, gamPtr);
                }
            }
        }
    }

    return ret;
}

