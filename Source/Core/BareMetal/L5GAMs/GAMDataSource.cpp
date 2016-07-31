/**
 * @file DataSourceSignal.cpp
 * @brief Source file for class DataSourceSignal
 * @date 11/apr/2016
 * @author pc
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
#include <GAMDataSource.h>
#include "DataSourceSignalRecord.h"
#include "GAM.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "ReferenceT.h"
#include "StandardParser.h"
#include "CLASSREGISTER.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMDataSource::GAMDataSource() :
        DataSourceI() {
    signalMemory = NULL_PTR(void *);
    signalOffsets = NULL_PTR(uint32 *);
    heap = NULL_PTR(HeapI *);
}

GAMDataSource::~GAMDataSource() {
    if (heap != NULL_PTR(HeapI *)) {
        if (signalMemory != NULL_PTR(void *)) {
            heap->Free(signalMemory);
        }
        if (signalOffsets != NULL_PTR(uint32 *)) {
            delete[] signalOffsets;
        }
    }
}

bool GAMDataSource::Initialise(StructuredDataI & data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        StreamString heapName;
        if (data.Read("HeapName", heapName)) {
            heap = HeapManager::FindHeap(heapName.Buffer());
            if (heap == NULL_PTR(HeapI *)) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Could not instantiate an heap with the name: %s", heapName.Buffer())
                ret = false;
            }
        }
        else {
            heap = GlobalObjectsDatabase::Instance()->GetStandardHeap();
        }
    }
    return ret;
}

uint32 GAMDataSource::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool GAMDataSource::GetSignalMemoryBuffer(uint32 signalIdx,
                                          uint32 bufferIdx,
                                          void *&signalAddress) {
    bool ret = (bufferIdx < 1u);
    if (ret) {
        ret = (signalIdx < numberOfSignals);
    }
    if (ret) {
        char8 *signalAddressChar = reinterpret_cast<char8 *>(signalMemory);
        signalAddressChar += signalOffsets[signalIdx];
        signalAddress = reinterpret_cast<void *&>(signalAddressChar);
    }

    return ret;
}

bool GAMDataSource::AllocateMemory() {
    uint32 numberOfSignals = GetNumberOfSignals();
    bool ret = (numberOfSignals > 0u);
    if (ret) {
        ret = (signalMemory == NULL_PTR(void *));
    }
    if (ret) {
        signalOffsets = new uint32[numberOfSignals];
    }
    uint32 memorySize = 0u;
    for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
        uint32 thisSignalMemorySize;
        ret = GetSignalByteSize(s, thisSignalMemorySize);
        if (ret) {
            signalOffsets[s] = memorySize;
        }
        if (ret) {
            ret = (thisSignalMemorySize > 0u);
        }
        if (ret) {
            memorySize += thisSignalMemorySize;
        }
    }
    if (ret) {
        signalMemory = heap->Malloc(memorySize);
        MemoryOperationsHelper::Set(signalMemory, 0, memorySize);
    }
    return ret;
}

const char8 *GAMDataSource::GetBrokerName(StructuredDataI &data,
                                          SignalDirection direction) {
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

bool GAMDataSource::PrepareNextState(const RealTimeStateInfo &status) {
//Set the default value for all the input signals
    uint32 numberOfFunctions = GetNumberOfFunctions();
    bool ret = true;

    for (uint32 n = 0u; (n < numberOfFunctions) && (ret); n++) {
        uint32 numberOfFunctionInputSignals;
        ret = GetFunctionNumberOfSignals(InputSignals, n, numberOfFunctionInputSignals);
        for (uint32 i = 0u; (i < numberOfFunctionInputSignals) && (ret); i++) {
            StreamString functionSignalAlias;
            if (ret) {
                ret = GetFunctionSignalAlias(InputSignals, n, i, functionSignalAlias);
            }
            uint32 signalIdx;
            if (ret) {
                ret = GetSignalIndex(signalIdx, functionSignalAlias.Buffer());
            }
            bool update = false;
            //Variable used in the current state?
            if (ret) {
                uint32 numberOfProducersCurrentState;
                if (!GetSignalNumberOfProducers(signalIdx, status.currentState, numberOfProducersCurrentState)) {
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
                void *signalMemory = NULL_PTR(void *);
                if (ret) {
                    ret = GetSignalMemoryBuffer(signalIdx, 0u, signalMemory);
                }
                if (defaultValueType.GetTypeDescriptor() == VoidType) {
                    uint32 size;
                    if (ret) {
                        ret = GetSignalByteSize(signalIdx, size);
                    }
                    if (ret) {
                        MemoryOperationsHelper::Set(signalMemory, '\0', size);
                    }
                }
                else {
                    AnyType thisSignal(typeDesc, 0u, signalMemory);
                    uint32 thisSignalNumberOfElements;
                    uint32 thisSignalNumberOfDimensions;
                    StreamString signalName;
                    if (ret) {
                        ret = GetSignalName(signalIdx, signalName);
                        signalName.Seek(0u);
                    }
                    if (ret) {
                        ret = GetSignalNumberOfElements(signalIdx, thisSignalNumberOfElements);
                    }
                    if (ret) {
                        ret = GetSignalNumberOfDimensions(signalIdx, thisSignalNumberOfDimensions);
                    }
                    if (ret) {
                        ret = (thisSignalNumberOfDimensions == defaultValueType.GetNumberOfDimensions());
                    }
                    if (ret) {
                        thisSignal.SetNumberOfDimensions(thisSignalNumberOfDimensions);
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Default value has different number of dimensions w.r.t. to the signal %s",
                                                signalName)
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
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Default value has different number of elements w.r.t. to the signal %s",
                                                signalName)
                    }
                    for (d = 0u; (d < thisSignalNumberOfDimensions) && (ret); d++) {
                        uint32 elementsInDimensionN = defaultValueType.GetNumberOfElements(d);
                        thisSignal.SetNumberOfElements(d, elementsInDimensionN);
                    }
                    if (!GetSignalDefaultValue(signalIdx, thisSignal)) {
                        ret = false;
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Could not read existent Default value for signal %s", signalName)
                    }

                }
            }
        }
    }
    return ret;
}

bool GAMDataSource::GetInputBrokers(ReferenceContainer &inputReaders,
                                    const char8* functionName,
                                    void * gamMemPtr) {
//generally a loop for each supported broker
    ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        if (broker->GetNumberOfCopies() > 0u) {
            ret = inputReaders.Insert(broker);
        }
    }
    return ret;
}

bool GAMDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                     const char8* functionName,
                                     void * gamMemPtr) {
    ReferenceT<MemoryMapOutputBroker> broker("MemoryMapOutputBroker");
    bool ret = broker.IsValid();
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

CLASS_REGISTER(GAMDataSource, "1.0")

}
