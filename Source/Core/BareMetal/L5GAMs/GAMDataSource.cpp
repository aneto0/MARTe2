/**
 * @file GAMDataSource.cpp
 * @brief Source file for class DataSourceSignal
 * @date 11/04/2016
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
 * the class DataSourceSignal (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "GAMDataSource.h"
#include "GAM.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "ReferenceT.h"
#include "StandardParser.h"
#include "CLASSREGISTER.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMDataSource::GAMDataSource() :
        DataSourceI() {
    signalMemory = NULL_PTR(void*);
    signalOffsets = NULL_PTR(uint32*);
    memoryHeap = NULL_PTR(HeapI*);
    allowNoProducers = false;
    resetUnusedVariablesAtStateChange = true;
    forceResetUnusedVariablesAtStateChange = true;
}

GAMDataSource::~GAMDataSource() {
    if (memoryHeap != NULL_PTR(HeapI*)) {
        if (signalMemory != NULL_PTR(void*)) {
            /*lint -e{1551} HeapManager::Free is expected to be exception free*/
            memoryHeap->Free(signalMemory);
        }
        if (signalOffsets != NULL_PTR(uint32*)) {
            delete[] signalOffsets;
        }
    }
    /*lint -e{1740} memoryHeap+ was zero or it is freed and zeroed by HeapManager::Free*/
}

bool GAMDataSource::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        StreamString heapName;
        if (data.Read("HeapName", heapName)) {
            memoryHeap = HeapManager::FindHeap(heapName.Buffer());
            if (memoryHeap == NULL_PTR(HeapI*)) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not instantiate an memoryHeap with the name: %s", heapName.Buffer());
                ret = false;
            }
        }
        else {
            memoryHeap = GlobalObjectsDatabase::Instance()->GetStandardHeap();
        }
    }
    if (ret) {
        uint32 allowNoProducersUInt32 = 0u;
        (void) (data.Read("AllowNoProducers", allowNoProducersUInt32));
        allowNoProducers = (allowNoProducersUInt32 == 1u);
    }
    if (ret) {
        uint32 resetUnusedVariablesAtStateChangeUInt32 = 1u;
        (void) (data.Read("ResetUnusedVariablesAtStateChange", resetUnusedVariablesAtStateChangeUInt32));
        resetUnusedVariablesAtStateChange = (resetUnusedVariablesAtStateChangeUInt32 == 1u);
    }
    forceResetUnusedVariablesAtStateChange = true;
    return ret;
}

uint32 GAMDataSource::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool GAMDataSource::GetSignalMemoryBuffer(const uint32 signalIdx,
                                          const uint32 bufferIdx,
                                          void *&signalAddress) {
    bool ret = (bufferIdx < 1u);
    uint32 nOfSignals = GetNumberOfSignals();
    if (ret) {
        ret = (signalIdx < nOfSignals);
    }

    if (ret) {
        char8 *signalAddressChar = reinterpret_cast<char8*>(signalMemory);
        uint32 offset = 0u;
        if (signalOffsets != NULL_PTR(uint32*)) {
            offset = signalOffsets[signalIdx];
        }
        if (signalAddressChar != NULL_PTR(char8*)) {
            signalAddressChar = &signalAddressChar[offset];
            signalAddress = reinterpret_cast<void*&>(signalAddressChar);
        }
    }

    return ret;
}

bool GAMDataSource::AllocateMemory() {
    uint32 nOfSignals = GetNumberOfSignals();
    bool ret = (nOfSignals > 0u);
    if (ret) {
        ret = (signalMemory == NULL_PTR(void*));
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "No signals defined for DataSource with name %s", GetName());
    }
    if (ret) {
        signalOffsets = new uint32[nOfSignals];
    }

    uint32 memorySize = 0u;
    for (uint32 s = 0u; (s < nOfSignals) && (ret); s++) {
        uint32 thisSignalMemorySize;
        ret = GetSignalByteSize(s, thisSignalMemorySize);
        if (ret) {
            if (signalOffsets != NULL_PTR(uint32*)) {
                signalOffsets[s] = memorySize;
            }
        }
        if (ret) {
            ret = (thisSignalMemorySize > 0u);
        }
        if (ret) {
            memorySize += thisSignalMemorySize;
        }
    }
    if (ret) {
        if (memoryHeap != NULL_PTR(HeapI*)) {
            signalMemory = memoryHeap->Malloc(memorySize);
        }
        ret = MemoryOperationsHelper::Set(signalMemory, '\0', memorySize);
    }
    return ret;
}

const char8* GAMDataSource::GetBrokerName(StructuredDataI &data,
                                          const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8*);

    float32 freq;
    if (!data.Read("Frequency", freq)) {
        freq = -1.0F;
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

/*lint -e{715} this implementation of the StatefulI interface does not need to know about the nextStateName*/
bool GAMDataSource::PrepareNextState(const char8 *const currentStateName,
                                     const char8 *const nextStateName) {
//Set the default value for all the input signals
    uint32 numberOfFunctions = GetNumberOfFunctions();
    bool ret = true;

    //At least the first time, reset all the variables to the default value.
    bool resetUnusedVariables = resetUnusedVariablesAtStateChange;
    if (forceResetUnusedVariablesAtStateChange) {
        resetUnusedVariables = true;
        forceResetUnusedVariablesAtStateChange = false;
    }

    for (uint32 n = 0u; (n < numberOfFunctions) && (ret) && (resetUnusedVariables); n++) {
        uint32 numberOfFunctionInputSignals;
        ret = GetFunctionNumberOfSignals(InputSignals, n, numberOfFunctionInputSignals);
        for (uint32 i = 0u; (i < numberOfFunctionInputSignals) && (ret); i++) {
            StreamString functionSignalAlias;
            ret = GetFunctionSignalAlias(InputSignals, n, i, functionSignalAlias);
            uint32 signalIdx = 0u;
            if (ret) {
                ret = GetSignalIndex(signalIdx, functionSignalAlias.Buffer());
            }
            bool update = false;
            //Variable used in the current state?
            if (ret) {
                uint32 numberOfProducersCurrentState;
                if (!GetSignalNumberOfProducers(signalIdx, currentStateName, numberOfProducersCurrentState)) {
                    numberOfProducersCurrentState = 0u;
                }
                //If the variable was not used update!
                update = (numberOfProducersCurrentState == 0u);
            }
            TypeDescriptor typeDesc;
            if (update) {
                if (ret) {
                    typeDesc = GetSignalType(signalIdx);
                    ret = (typeDesc != InvalidType);
                }
            }
            if (update) {
                //if the default value is declared use it to initialise
                //otherwise null the memory
                AnyType defaultValueType = GetSignalDefaultValueType(signalIdx);
                void *thisSignalMemory = NULL_PTR(void*);
                if (ret) {
                    ret = GetSignalMemoryBuffer(signalIdx, 0u, thisSignalMemory);
                }
                if (defaultValueType.GetTypeDescriptor() == VoidType) {
                    uint32 size = 0u;
                    if (ret) {
                        ret = GetSignalByteSize(signalIdx, size);
                    }
                    if (ret) {
                        ret = MemoryOperationsHelper::Set(thisSignalMemory, '\0', size);
                    }
                }
                else {
                    AnyType thisSignal(typeDesc, 0u, thisSignalMemory);
                    uint32 thisSignalNumberOfElements = 0u;
                    uint8 thisSignalNumberOfDimensions = 0u;
                    StreamString signalName;
                    if (ret) {
                        ret = GetSignalName(signalIdx, signalName);
                    }
                    if (ret) {
                        ret = signalName.Seek(0LLU);
                    }
                    if (ret) {
                        ret = GetSignalNumberOfElements(signalIdx, thisSignalNumberOfElements);
                    }
                    if (ret) {
                        ret = GetSignalNumberOfDimensions(signalIdx, thisSignalNumberOfDimensions);
                    }
                    if (ret) {
                        ret = (thisSignalNumberOfDimensions == defaultValueType.GetNumberOfDimensions());
                        if (!ret) {
                            if (thisSignalNumberOfDimensions == 1u) {
                                if (defaultValueType.GetNumberOfDimensions() == 0u) {
                                    ret = (defaultValueType.GetNumberOfElements(0u) == 1u);
                                }
                            }
                        }
                    }
                    if (ret) {
                        thisSignal.SetNumberOfDimensions(defaultValueType.GetNumberOfDimensions());
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "Default value has different number of dimensions w.r.t. to the signal %s", signalName);
                    }

                    uint32 defaultValueNumberOfElements = 1u;
                    uint32 d;
                    for (d = 0u; (d < thisSignalNumberOfDimensions) && (ret); d++) {
                        uint32 elementsInDimensionN = defaultValueType.GetNumberOfElements(d);
                        defaultValueNumberOfElements *= elementsInDimensionN;
                    }
                    if (ret) {
                        ret = (thisSignalNumberOfElements == defaultValueNumberOfElements);
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "Default value has different number of elements w.r.t. to the signal %s", signalName);
                    }
                    for (d = 0u; (d < thisSignalNumberOfDimensions) && (ret); d++) {
                        uint32 elementsInDimensionN = defaultValueType.GetNumberOfElements(d);
                        thisSignal.SetNumberOfElements(d, elementsInDimensionN);
                    }
                    if (!GetSignalDefaultValue(signalIdx, thisSignal)) {
                        ret = false;
                        REPORT_ERROR(ErrorManagement::FatalError, "Could not read existent Default value for signal %s", signalName);
                    }

                }
            }
        }
    }
    return ret;
}

bool GAMDataSource::GetInputBrokers(ReferenceContainer &inputBrokers,
                                    const char8 *const functionName,
                                    void *const gamMemPtr) {
//generally a loop for each supported broker
    ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr, true);
    }
    if (ret) {
        if (broker->GetNumberOfCopies() > 0u) {
            ret = inputBrokers.Insert(broker);
        }
    }
    return ret;
}

bool GAMDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                     const char8 *const functionName,
                                     void *const gamMemPtr) {
    ReferenceT<MemoryMapOutputBroker> broker("MemoryMapOutputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr, true);
    }
    if (ret) {
        if (broker->GetNumberOfCopies() > 0u) {
            ret = outputBrokers.Insert(broker);
        }
    }
    return ret;
}

bool GAMDataSource::SetConfiguredDatabase(StructuredDataI &data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);
    uint32 nSignals = GetNumberOfSignals();
    uint32 nStates = 0u;
    uint32 n;
    for (n = 0u; (n < nSignals) && (ret); n++) {
        StreamString signalName;
        ret = GetSignalName(n, signalName);
        if (ret) {
            if (!GetSignalNumberOfStates(n, nStates)) {
                nStates = 0u;
            }
            if (nStates == 0u) {
                REPORT_ERROR(
                        ErrorManagement::Information,
                        "In GAMDataSource %s, signal %s will never be produced nor consumed because there is no GAM with this signal being executed in any state.",
                        GetName(), signalName.Buffer());
            }
        }
        uint32 s;
        for (s = 0u; (s < nStates) && (ret); s++) {
            uint32 nProducers = 0u;
            StreamString stateName;
            ret = GetSignalStateName(n, s, stateName);
            if (ret) {
                ret = GetSignalNumberOfProducers(n, stateName.Buffer(), nProducers);
            }
            if (ret) {
                ret = (nProducers > 0u);
            }
            if (!ret) {
                ErrorManagement::ErrorType errLog = ErrorManagement::FatalError;
                if (allowNoProducers) {
                    ret = true;
                    errLog = ErrorManagement::Warning;
                }
                REPORT_ERROR(errLog, "In GAMDataSource %s, state %s, signal %s has an invalid number of producers. Should be > 0 but is %d", GetName(),
                             stateName.Buffer(), signalName.Buffer(), nProducers);
            }
        }
    }
    return ret;
}

bool GAMDataSource::Synchronise() {
    return false;
}

CLASS_REGISTER(GAMDataSource, "1.0")

}
