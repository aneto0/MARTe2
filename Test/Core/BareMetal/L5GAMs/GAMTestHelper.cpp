/**
 * @file GAMTestHelper.cpp
 * @brief Source file for class GAMTestHelper
 * @date 01/03/2016
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
 * the class GAMTestHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "GAMSchedulerI.h"
#include "GAMTestHelper.h"
#include "Introspection.h"
#include "IntrospectionT.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "Sleep.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

DECLARE_CLASS_MEMBER(TestStructB, b1, int32, "", "");

DECLARE_CLASS_MEMBER(TestStructB, b2, int32, "", "");

static const IntrospectionEntry* TestStructBEntries[] = { &TestStructB_b1_introspectionEntry, &TestStructB_b2_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestStructB, TestStructBEntries)

DECLARE_CLASS_MEMBER(TestStructA, a1, TestStructB, "", "");

DECLARE_CLASS_MEMBER(TestStructA, a2, float32, "", "");

static const IntrospectionEntry* TestStructAEntries[] = { &TestStructA_a1_introspectionEntry, &TestStructA_a2_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestStructA, TestStructAEntries);

DECLARE_CLASS_MEMBER(TestStructC, c1, TestStructB, "", "");

DECLARE_CLASS_MEMBER(TestStructC, c2, float32, "[3]", "");

DECLARE_CLASS_MEMBER(TestStructC, c3, int32, "[2][4]", "");

static const IntrospectionEntry* TestStructCEntries[] = { &TestStructC_c1_introspectionEntry, &TestStructC_c2_introspectionEntry,
        &TestStructC_c3_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestStructC, TestStructCEntries);

DECLARE_CLASS_MEMBER(TestStructD, c1, TestStructB, "", "");

DECLARE_CLASS_MEMBER(TestStructD, c2, float32, "[3]", "");

DECLARE_CLASS_MEMBER(TestStructD, c3, int32, "[2][4]", "");

static const IntrospectionEntry* TestStructDEntries[] = { &TestStructD_c1_introspectionEntry, &TestStructD_c2_introspectionEntry,
        &TestStructD_c3_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestStructD, TestStructDEntries);

DECLARE_CLASS_MEMBER(TestStructBB, b1, int8, "", "");

DECLARE_CLASS_MEMBER(TestStructBB, b2, int64, "", "");

static const IntrospectionEntry* TestStructBBEntries[] = { &TestStructBB_b1_introspectionEntry, &TestStructBB_b2_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestStructBB, TestStructBBEntries)

DECLARE_CLASS_MEMBER(TestStructCC, c1, TestStructBB, "", "");

DECLARE_CLASS_MEMBER(TestStructCC, c2, float32, "[3]", "");

DECLARE_CLASS_MEMBER(TestStructCC, c3, int32, "[2][4]", "");

static const IntrospectionEntry* TestStructCCEntries[] = { &TestStructCC_c1_introspectionEntry, &TestStructCC_c2_introspectionEntry,
        &TestStructCC_c3_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestStructCC, TestStructCCEntries);

DECLARE_CLASS_MEMBER(TestStructE, e1, TestStructC, "[2][1][2]", "");

DECLARE_CLASS_MEMBER(TestStructE, e2, int32, "", "");

static const IntrospectionEntry* TestStructEEntries[] = { &TestStructE_e1_introspectionEntry, &TestStructE_e2_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestStructE, TestStructEEntries);

DECLARE_CLASS_MEMBER(TestStructF, f1, TestStructE, "[2]", "");

DECLARE_CLASS_MEMBER(TestStructF, f2, int32, "", "");

static const IntrospectionEntry* TestStructFEntries[] = { &TestStructF_f1_introspectionEntry, &TestStructF_f2_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestStructF, TestStructFEntries);

DECLARE_CLASS_MEMBER(TestStructG, g1, TestStructF, "[2][2]", "");

DECLARE_CLASS_MEMBER(TestStructG, g2, int32, "", "");

static const IntrospectionEntry* TestStructGEntries[] = { &TestStructG_g1_introspectionEntry, &TestStructG_g2_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestStructG, TestStructGEntries);

DECLARE_CLASS_MEMBER(TestStructH, str, char8, "[64]", "");

static const IntrospectionEntry* TestStructHEntries[] = { &TestStructH_str_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestStructH, TestStructHEntries);


///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

GAM1::GAM1() :
        GAM() {
    numberOfExecutions = 0u;
    context = 0u;

}

GAM1::~GAM1() {
}

bool GAM1::Initialise(StructuredDataI & data) {
    return GAM::Initialise(data);
}

bool GAM1::PrepareNextState(const char8 * currentStateName, const char8 * nextStateName) {
    context++;
    return true;
}

bool GAM1::Execute() {
    uint32 b;
    for (b = 0u; b < inputBrokers.Size(); b++) {
        ReferenceT<ExecutableI> broker = inputBrokers.Get(b);
        broker->Execute();
    }
    uint32 *inputBuffer = (uint32 *) GetInputSignalsMemory();
    uint32 *outputBuffer = (uint32 *) GetOutputSignalsMemory();

    outputBuffer[0] = inputBuffer[0] + inputBuffer[1];

    for (b = 0u; b < outputBrokers.Size(); b++) {
        ReferenceT<ExecutableI> broker = outputBrokers.Get(b);
        broker->Execute();
    }
    numberOfExecutions++;
    Sleep::MSec(100);

    return true;
}

bool GAM1::Setup() {
    return true;
}


uint8 *GAM1::GetInputMem() {
    return (uint8*)GetInputSignalsMemory();
}

uint8 *GAM1::GetOutputMem() {
    return (uint8*)GetOutputSignalsMemory();
}

CLASS_REGISTER(GAM1, "1.0");

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

bool GAMGroup1::PrepareNextState(const char8 * currentStateName, const char8 * nextStateName) {
    return true;

}

void GAMGroup1::SetUp() {

}
CLASS_REGISTER(GAMGroup1, "1.0");

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

DS1::DS1() :
        DataSourceI() {

}

DS1::~DS1() {

}

bool DS1::Initialise(StructuredDataI & data) {
    return DataSourceI::Initialise(data);
}

uint32 DS1::GetCurrentBufferIndex() {
    return 0u;
}

uint32 DS1::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool DS1::AllocateMemory() {
    return false;
}

bool DS1::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    return true;
}

const char8 *DS1::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    const char8* brokerName = NULL_PTR(const char8 *);

    float32 freq;
    if (!data.Read("Frequency", freq)) {
        freq = -1;
    }
    uint32 samples;
    if (!data.Read("Samples", samples)) {
        samples = 1u;
    }

    if ((freq < 0.) && (samples == 1u)) {
        if (direction == InputSignals) {
            brokerName = "MemoryMapInputBroker";
        }
        else {
            brokerName = "MemoryMapOutputBroker";
        }
    }
    return brokerName;

}

bool DS1::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}

bool DS1::ChangeState() {
    return true;
}

bool DS1::GetInputBrokers(ReferenceContainer &inputBrokers, const char8 * const functionName, void* const gamMemPtr) {
    bool ret = true;
    //generally a loop for each supported broker
    ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
    ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        if (broker->GetNumberOfCopies() > 0u) {
            ret = inputBrokers.Insert(broker);
        }
    }
    return ret;
}

bool DS1::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8 * const functionName, void* const gamMemPtr) {

    bool ret = true;
    //generally a loop for each supported broker
    ReferenceT<MemoryMapOutputBroker> broker("MemoryMapOutputBroker");

    ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        if (broker->GetNumberOfCopies() > 0u) {
            ret = outputBrokers.Insert(broker);
        }
    }
    return ret;

}

bool DS1::Synchronise() {
    return false;
}

CLASS_REGISTER(DS1, "1.0");

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

Driver1::Driver1() :
        DataSourceI() {

}

Driver1::~Driver1() {

}

bool Driver1::Initialise(StructuredDataI & data) {
    return DataSourceI::Initialise(data);
}

uint32 Driver1::GetCurrentBufferIndex() {
    return 0u;
}

uint32 Driver1::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool Driver1::AllocateMemory() {
    return true;
}

bool Driver1::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    return true;
}

const char8 *Driver1::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    const char8* brokerName = NULL_PTR(const char8 *);

    float32 freq;
    if (!data.Read("Frequency", freq)) {
        freq = -1;
    }
    uint32 samples;
    if (!data.Read("Samples", samples)) {
        samples = 1u;
    }

    if (freq < 0.) {
        if (samples == 1) {
            if (direction == InputSignals) {
                brokerName = "MemoryMapInputBroker";
            }
            else {
                brokerName = "MemoryMapOutputBroker";
            }
        }
        else {
            if (direction == InputSignals) {
                brokerName = "SampleInputBroker";
            }
            else {
                brokerName = "SampleOutputBroker";
            }
        }
    }
    else {
        if (direction == InputSignals) {
            brokerName = "SyncInputBroker";
        }
        else {
            brokerName = "SyncOutputBroker";
        }
    }
    return brokerName;

}

bool Driver1::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}

bool Driver1::ChangeState() {
    return true;
}

bool Driver1::GetInputBrokers(ReferenceContainer &inputBrokers, const char8 * const functionName, void* const gamMemPtr) {
    bool ret = true;
    //generally a loop for each supported broker
    ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
    ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        if (broker->GetNumberOfCopies() > 0u) {
            ret = inputBrokers.Insert(broker);
        }
    }
    return ret;
}

bool Driver1::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8 * const functionName, void* const gamMemPtr) {

    bool ret = true;
    //generally a loop for each supported broker
    ReferenceT<MemoryMapOutputBroker> broker("MemoryMapOutputBroker");

    ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        if (broker->GetNumberOfCopies() > 0u) {
            ret = outputBrokers.Insert(broker);
        }
    }
    return ret;

}

bool Driver1::Synchronise() {
    return false;
}

CLASS_REGISTER(Driver1, "1.0");

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

DefaultSchedulerForTests::DefaultSchedulerForTests() :
        GAMSchedulerI() {
}
MARTe::ErrorManagement::ErrorType DefaultSchedulerForTests::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType DefaultSchedulerForTests::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

void DefaultSchedulerForTests::CustomPrepareNextState() {

}

CLASS_REGISTER(DefaultSchedulerForTests, "1.0")

