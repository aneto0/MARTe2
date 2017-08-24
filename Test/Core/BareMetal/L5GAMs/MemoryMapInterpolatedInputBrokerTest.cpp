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
    TypeDescriptor *signalTypes;
    uint32 *nElements;
    uint32 *offsets;
    uint32 timeSignalIdx;
    uint32 timeIntIncrement;
    float32 timeFloatIncrement;
    uint32 intIncrement;
    float32 floatIncrement;
    float64 interpolationPeriod;
};

MemoryMapInterpolatedInputBrokerDataSourceTestHelper::MemoryMapInterpolatedInputBrokerDataSourceTestHelper() :
        DataSourceI() {
    signalMemory = NULL_PTR(void *);
    offsets = NULL_PTR(uint32 *);
    signalTypes = NULL_PTR(TypeDescriptor *);
    nElements = NULL_PTR(uint32 *);
    timeSignalIdx = 0;
    timeIntIncrement = 1;
    timeFloatIncrement = 1.0;
    intIncrement = 1000;
    floatIncrement = 0.1;
    interpolationPeriod = 5;
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

bool MemoryMapInterpolatedInputBrokerDataSourceTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName,
                                                                           void * const gamMemPtr) {
    ReferenceT<MemoryMapInterpolatedInputBroker> broker("MemoryMapInterpolatedInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        void *timeSignal;
        GetSignalMemoryBuffer(timeSignalIdx, 0, timeSignal);
        broker->SetTimeSignal(timeSignal, GetSignalType(timeSignalIdx), interpolationPeriod);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool MemoryMapInterpolatedInputBrokerDataSourceTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName,
                                                                            void * const gamMemPtr) {
    return true;
}

bool MemoryMapInterpolatedInputBrokerDataSourceTestHelper::Synchronise() {
    bool ret = true;
    uint32 s;
    uint32 nSignals = GetNumberOfSignals();
    for (s = 0u; (s < nSignals) && (ret); s++) {
        char8 *memPtr = reinterpret_cast<char8 *>(signalMemory);
        memPtr += offsets[s];
        uint32 n;
        if (signalTypes[s] == UnsignedInteger8Bit) {
            uint8 *signal = reinterpret_cast<uint8 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                if (s == timeSignalIdx) {
                    signal[n] += timeIntIncrement;
                }
                else {
                    signal[n] += intIncrement;
                }
            }
        }
        else if (signalTypes[s] == UnsignedInteger16Bit) {
            uint16 *signal = reinterpret_cast<uint16 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                if (s == timeSignalIdx) {
                    signal[n] += timeIntIncrement;
                }
                else {
                    signal[n] += intIncrement;
                }
            }
        }
        else if (signalTypes[s] == UnsignedInteger32Bit) {
            uint32 *signal = reinterpret_cast<uint32 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                if (s == timeSignalIdx) {
                    signal[n] += timeIntIncrement;
                }
                else {
                    signal[n] += intIncrement;
                }
            }
        }
        else if (signalTypes[s] == UnsignedInteger64Bit) {
            uint64 *signal = reinterpret_cast<uint64 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                if (s == timeSignalIdx) {
                    signal[n] += timeIntIncrement;
                }
                else {
                    signal[n] += intIncrement;
                }
            }
        }
        else if (signalTypes[s] == SignedInteger8Bit) {
            int8 *signal = reinterpret_cast<int8 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                if (s == timeSignalIdx) {
                    signal[n] += timeIntIncrement;
                }
                else {
                    signal[n] += intIncrement;
                }
            }
        }
        else if (signalTypes[s] == SignedInteger16Bit) {
            int16 *signal = reinterpret_cast<int16 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                if (s == timeSignalIdx) {
                    signal[n] += timeIntIncrement;
                }
                else {
                    signal[n] += intIncrement;
                }
            }
        }
        else if (signalTypes[s] == SignedInteger32Bit) {
            int32 *signal = reinterpret_cast<int32 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                if (s == timeSignalIdx) {
                    signal[n] += timeIntIncrement;
                }
                else {
                    signal[n] += intIncrement;
                }
            }
        }
        else if (signalTypes[s] == SignedInteger64Bit) {
            int64 *signal = reinterpret_cast<int64 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                if (s == timeSignalIdx) {
                    signal[n] += timeIntIncrement;
                }
                else {
                    signal[n] += intIncrement;
                }
            }
        }
        else if (signalTypes[s] == Float32Bit) {
            float32 *signal = reinterpret_cast<float32 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                if (s == timeSignalIdx) {
                    signal[n] += timeFloatIncrement;
                }
                else {
                    signal[n] += floatIncrement;
                }
            }
        }
        else if (signalTypes[s] == Float64Bit) {
            float64 *signal = reinterpret_cast<float64 *>(memPtr);
            for (n = 0u; (n < nElements[s]) && (ret); n++) {
                if (s == timeSignalIdx) {
                    signal[n] += timeFloatIncrement;
                }
                else {
                    signal[n] += floatIncrement;
                }
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

#include <stdio.h>
bool MemoryMapInterpolatedInputBrokerTest::TestExecute() {
    bool ret = InitialiseMemoryMapInterpolatedInputBrokerEnviroment(config1);
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
    uint32 intIncrement = dataSource->intIncrement;
    float32 floatIncrement = dataSource->floatIncrement;
    uint32 s;

    uint32 e;
    uint32 nExecutes = 5;
    for (e = 1; (e < nExecutes) && (ret); e++) {
        ret = broker->Execute();
        for (s = 0; (s < nOfSignals) && (ret); s++) {
            void *gamPtr = gamA->GetInputSignalMemory(s);
            uint32 nElements;
            uint32 n;
            uint32 intMult = intIncrement;
            float32 floatMult = floatIncrement;
            if (s == dataSource->timeSignalIdx) {
                intMult = dataSource->timeIntIncrement;
                floatMult = dataSource->timeFloatIncrement;
            }

            gamA->GetSignalNumberOfElements(InputSignals, s, nElements);
            TypeDescriptor signalType = gamA->GetSignalType(InputSignals, s);
            if (signalType == UnsignedInteger8Bit) {
                uint8 *signal = static_cast<uint8 *>(gamPtr);
                uint8 expectedValue = static_cast<uint8>(intMult);
                expectedValue *= (e * dataSource->interpolationPeriod);
                for (n = 0u; (n < nElements) && (ret); n++) {
                    ret = (signal[n] == expectedValue);
                    printf("1. [%d] vs [%d]\n", signal[n], expectedValue);
                }
            }
            else if (signalType == UnsignedInteger16Bit) {
                uint16 *signal = static_cast<uint16 *>(gamPtr);
                uint16 expectedValue = static_cast<uint16>(intMult);
                expectedValue *= (e * dataSource->interpolationPeriod);
                for (n = 0u; (n < nElements) && (ret); n++) {
                    ret = (signal[n] == expectedValue);
                    printf("2. [%d] vs [%d]\n", signal[n], expectedValue);
                }
            }
            else if (signalType == UnsignedInteger32Bit) {
                uint32 *signal = static_cast<uint32 *>(gamPtr);
                uint32 expectedValue = static_cast<uint32>(intMult);
                expectedValue *= (e * dataSource->interpolationPeriod);
                for (n = 0u; (n < nElements) && (ret); n++) {
                    ret = (signal[n] == expectedValue);
                    printf("3. [%d] vs [%d]\n", signal[n], expectedValue);
                }
            }
            else if (signalType == UnsignedInteger64Bit) {
                uint64 *signal = static_cast<uint64 *>(gamPtr);
                uint64 expectedValue = static_cast<uint64>(intMult);
                expectedValue *= (e * dataSource->interpolationPeriod);
                for (n = 0u; (n < nElements) && (ret); n++) {
                    ret = (signal[n] == expectedValue);
                    printf("4. [%d] vs [%d]\n", signal[n], expectedValue);
                }
            }
            else if (signalType == SignedInteger8Bit) {
                int8 *signal = static_cast<int8 *>(gamPtr);
                int8 expectedValue = static_cast<int8>(intMult);
                expectedValue *= (e * dataSource->interpolationPeriod);
                for (n = 0u; (n < nElements) && (ret); n++) {
                    ret = (signal[n] == expectedValue);
                    printf("5. [%d] vs [%d]\n", signal[n], expectedValue);
                }
            }
            else if (signalType == SignedInteger16Bit) {
                int16 *signal = static_cast<int16 *>(gamPtr);
                int16 expectedValue = static_cast<int16>(intMult);
                expectedValue *= (e * dataSource->interpolationPeriod);
                for (n = 0u; (n < nElements) && (ret); n++) {
                    ret = (signal[n] == expectedValue);
                    printf("6. [%d] vs [%d]\n", signal[n], expectedValue);
                }
            }
            else if (signalType == SignedInteger32Bit) {
                int32 *signal = static_cast<int32 *>(gamPtr);
                int32 expectedValue = static_cast<int32>(intMult);
                expectedValue *= (e * dataSource->interpolationPeriod);
                for (n = 0u; (n < nElements) && (ret); n++) {
                    ret = (signal[n] == expectedValue);
                    printf("7. [%d] vs [%d]\n", signal[n], expectedValue);
                }
            }
            else if (signalType == SignedInteger64Bit) {
                int64 *signal = static_cast<int64 *>(gamPtr);
                int64 expectedValue = static_cast<int64>(intMult);
                expectedValue *= (e * dataSource->interpolationPeriod);
                for (n = 0u; (n < nElements) && (ret); n++) {
                    ret = (signal[n] == expectedValue);
                    printf("8. [%d] vs [%d]\n", signal[n], expectedValue);
                }
            }
            else if (signalType == Float32Bit) {
                float32 *signal = static_cast<float32 *>(gamPtr);
                float32 expectedValue = static_cast<float32>(floatMult);
                expectedValue *= (e * dataSource->interpolationPeriod);
                for (n = 0u; (n < nElements) && (ret); n++) {
                    ret = (signal[n] == expectedValue);
                    printf("9. [%e] vs [%e]\n", signal[n], expectedValue);
                }
            }
            else if (signalType == Float64Bit) {
                float64 *signal = static_cast<float64 *>(gamPtr);
                float64 expectedValue = static_cast<float64>(floatMult);
                expectedValue *= (e * dataSource->interpolationPeriod);
                for (n = 0u; (n < nElements) && (ret); n++) {
                    ret = (signal[n] == expectedValue);
                    printf("10. [%e] vs [%e]\n", signal[n], expectedValue);
                }
            }
        }
    }

    return ret;
}

