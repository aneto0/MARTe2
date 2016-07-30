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
    signalMemory = NULL_PTR(void **);
    heap = NULL_PTR(HeapI *);
}

GAMDataSource::~GAMDataSource() {
    if (signalMemory != NULL_PTR(void **)) {
        delete[] signalMemory;
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
        signalAddress = signalMemory[signalIdx];
    }

    return ret;
}

bool GAMDataSource::AllocateMemory() {
    uint32 numberOfSignals = GetNumberOfSignals();
    bool ret = (numberOfSignals > 0u);
    if (ret) {
        ret = (signalMemory == NULL_PTR(void **));
    }
    if (ret) {
        signalMemory = new void*[numberOfSignals];
    }
    for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
        uint32 memorySize;
        ret = GetSignalByteSize(s, memorySize);
        if (ret) {
            ret = (memorySize > 0u);
        }
        if (ret) {
            signalMemory[s] = heap->Malloc(memorySize);
            MemoryOperationsHelper::Set(signalMemory[s], 0, memorySize);
        }
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
            if (update && ret) {
                typeDesc = GetSignalType(signalIdx);
                ret = (typeDesc != InvalidType);
            }
            if (update && ret) {
                //if the default value is declared use it to initialise
                //otherwise null the memory
                AnyType defaultValueType = GetSignalDefaultValueType(signalIdx);
                if (defaultValueType.GetTypeDescriptor() == VoidType) {
                    uint32 size;
                    ret = GetSignalByteSize(signalIdx, size);
                    if (ret) {
                        MemoryOperationsHelper::Set(signalMemory[signalIdx], '\0', size);
                    }
                }
                else {
                    StreamString signalName;
                    ret = GetSignalName(signalIdx, signalName);
                    signalName.Seek(0u);

                    AnyType thisSignal(typeDesc, 0u, signalMemory[signalIdx]);
                    uint32 thisSignalNumberOfElements;
                    uint32 thisSignalNumberOfDimensions;
                    ret = GetSignalNumberOfElements(signalIdx, thisSignalNumberOfElements);
                    if (ret) {
                        ret = GetSignalNumberOfDimensions(signalIdx, thisSignalNumberOfDimensions);
                    }
                    if (ret) {
                        ret = (thisSignalNumberOfDimensions == defaultValueType.GetNumberOfDimensions());
                    }

                    uint32 defaultValueNumberOfElements = 1u;
                    if (ret) {
                        thisSignal.SetNumberOfDimensions(thisSignalNumberOfDimensions);
                        uint32 d;
                        for (d = 0u; (d < thisSignalNumberOfDimensions) && (ret); d++) {
                            uint32 elementsInDimensionN = defaultValueType.GetNumberOfElements(d);
                            defaultValueNumberOfElements *= elementsInDimensionN;
                        }
                        ret = (thisSignalNumberOfElements == defaultValueNumberOfElements);
                        if (ret) {
                            for (d = 0u; (d < thisSignalNumberOfDimensions) && (ret); d++) {
                                uint32 elementsInDimensionN = defaultValueType.GetNumberOfElements(d);
                                thisSignal.SetNumberOfElements(d, elementsInDimensionN);
                            }
                        }
                        else {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Default value has different number of elements w.r.t. to the signal %s",
                                                    signalName)
                        }
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Default value has different number of dimensions w.r.t. to the signal %s",
                                                signalName)
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
