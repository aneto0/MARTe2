/**
 * @file DataSourceI.cpp
 * @brief Source file for class DataSourceI
 * @date 18/04/2016
 * @author Giuseppe Ferrò
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
 * the class DataSourceI (public, protected, and private). Be aware that some
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
#include "DataSourceI.h"
#include "GAM.h"
#include "GAMDataSource.h"
#include "GAMSignalsContainer.h"
#include "MemoryMapBroker.h"
#include "ReferenceContainerFilterObjectName.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
#if 0
static bool Allocate(ReferenceT<DataSourceSignal> dataSourceSignal,
        MemoryArea &memory) {
    bool ret = dataSourceSignal.IsValid();
    if (ret) {
        TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(dataSourceSignal->GetType());
        uint32 varSize = 0u;
        // structured type
        if (typeDes == InvalidType) {
            const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(dataSourceSignal->GetType());
            ret = (item != NULL);
            if (ret) {
                /*lint -e{613} NULL pointer checking done before entering here */
                const ClassProperties *properties = item->GetClassProperties();
                ret = (properties != NULL);
                if (ret) {
                    varSize = properties->GetSize();
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s does not provide ClassProperties", dataSourceSignal->GetType())
                }
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s is not registered", dataSourceSignal->GetType())
            }
        }
        // basic type
        else {
            varSize = (static_cast<uint32>(typeDes.numberOfBits) + 7u) / 8u;
            // consider the multi - dimensional
            varSize *= dataSourceSignal->GetNumberOfElements();
        }

        // consider the number of samples per cycle
        varSize *= dataSourceSignal->GetNumberOfSamples();

        // allocate the memory
        if (ret) {
            /*lint -e{613} NULL pointer checking done before entering here */
            //TODO likely that these offsets have to be set in DataSourceSignal
            uint32 offset = 0;
            ret = memory.Add(varSize, offset);
            //ret = memory->Add(varSize, dataSourceSignal->bufferPtrOffset[0]);
            if (ret) {
                /*lint -e{613} NULL pointer checking done before entering here */
                ret = memory.Add(varSize, offset);
                //ret = memory->Add(varSize, bufferPtrOffset[1]);
            }
        }
    }
    return ret;
}

static bool AllocatePrivate(ReferenceT<ReferenceContainer> container,
        MemoryArea &memory) {
    bool ret = true;
    uint32 numberOfNodes = container->Size();
    for (uint32 i = 0u; (i < numberOfNodes) && (ret); i++) {
        ReferenceT<ReferenceContainer> subContainer = container->Get(i);
        ret = subContainer.IsValid();
        if (ret) {
            ReferenceT<DataSourceSignal> def = subContainer;
            if (def.IsValid()) {
                ret = Allocate(def, memory);
            }
            else {
                ret = AllocatePrivate(subContainer, memory);
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "DataSourceI must contain an object inheriting from ReferenceContainer");
        }
    }
    return ret;
}
#endif
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

DataSourceI::DataSourceI() :
        ReferenceContainer() {
}
#if 0
bool DataSourceI::Allocate() {
    return AllocatePrivate(ReferenceT<ReferenceContainer>(this), memory);
}
#endif
bool DataSourceI::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (data.MoveRelative("Signals")) {
        signalsDatabase.Write("Signals", data);
        ret = data.MoveToAncestor(1);
    }
    signalsDatabase.MoveToRoot();

#if 0
    if (ret) {
        if (data.Read("HeapName", heapName)) {
            memory.SetHeapName(heapName.Buffer());
        }
    }
#endif
    return ret;
}

bool DataSourceI::AddSignals(StructuredDataI &data) {
    signalsDatabase.MoveAbsolute("Signals");
    return data.Write("Signals", signalsDatabase);
}

bool DataSourceI::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = data.Copy(configuredDatabase);
    return ret;
}

uint32 DataSourceI::GetNumberOfSignals() {
    bool ret = configuredDatabase.MoveToRoot();
    if (ret) {
        ret = configuredDatabase.MoveRelative("Signals");
    }
    uint32 numberOfSignals = 0u;
    if (ret) {
        numberOfSignals = configuredDatabase.GetNumberOfChildren();
    }
    return numberOfSignals;
}

bool DataSourceI::GetSignalName(uint32 signalIdx,
                                StreamString &signalName) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("QualifiedName", signalName);
    }
    return ret;
}

bool DataSourceI::GetSignalIndex(uint32 &signalIdx,
                                 const char8* const signalName) {

    uint32 numberOfSignals = GetNumberOfSignals();
    bool ret = true;
    bool found = false;
    for (signalIdx = 0u; (signalIdx < numberOfSignals) && ret && (!found); signalIdx++) {
        StreamString searchName;
        ret = GetSignalName(signalIdx, searchName);
        if (ret) {
            found = (StringHelper::Compare(signalName, searchName.Buffer()) == 0u);
        }
    }
    if (ret) {
        ret = found;
    }
    return ret;
}

TypeDescriptor DataSourceI::GetSignalType(uint32 signalIdx) {
    TypeDescriptor signalTypeDescriptor = InvalidType;
    bool ret = MoveToSignalIndex(signalIdx);
    StreamString signalType;
    if (ret) {
        ret = configuredDatabase.Read("Type", signalType);
    }
    if (ret) {
        signalTypeDescriptor = TypeDescriptor::GetTypeDescriptorFromTypeName(signalType.Buffer());
    }
    return signalTypeDescriptor;
}

bool DataSourceI::GetSignalNumberOfDimensions(uint32 signalIdx,
                                              uint32 &numberOfDimensions) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("NumberOfDimensions", numberOfDimensions);
    }
    return ret;
}

bool DataSourceI::GetSignalNumberElements(uint32 signalIdx,
                                          uint32 &numberOfElements) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("NumberOfElements", numberOfElements);
    }
    return ret;
}

bool DataSourceI::GetSignalByteSize(uint32 signalIdx,
                                    uint32 &byteSize) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("ByteSize", byteSize);
    }
    return ret;
}

bool DataSourceI::GetSignalNumberOfStates(uint32 signalIdx,
                                          uint32 &numberOfStates) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative("States");
    }
    if (ret) {
        numberOfStates = configuredDatabase.GetNumberOfChildren();
    }
    return ret;
}

bool DataSourceI::GetSignalStateName(uint32 signalIdx,
                                     uint32 stateIdx,
                                     StreamString &stateName) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative("States");
    }
    uint32 numberOfStates = 0u;
    if (ret) {
        ret = GetSignalNumberOfStates(signalIdx, numberOfStates);
    }
    if (ret) {
        ret = (stateIdx < numberOfStates);
    }
    if (ret) {
        ret = MoveToSignalIndex(signalIdx);
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative("States");
    }
    if (ret) {
        stateName = configuredDatabase.GetChildName(stateIdx);
    }

    return ret;
}

bool DataSourceI::GetSignalNumberOfConsumers(uint32 signalIdx,
                                             const char8 *stateName,
                                             uint32 &numberOfConsumers) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative("States");
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative(stateName);
    }
    AnyType consumers;
    if (ret) {
        consumers = configuredDatabase.GetType("Consumers");
        ret = (consumers.GetDataPointer() != NULL_PTR(void *));
    }
    numberOfConsumers = 0u;
    if (ret) {
        numberOfConsumers = consumers.GetNumberOfElements(0u);
    }
    return ret;
}

bool DataSourceI::GetSignalNumberOfProducers(uint32 signalIdx,
                                             const char8 *stateName,
                                             uint32 &numberOfProducers) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative("States");
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative(stateName);
    }
    AnyType producers;
    if (ret) {
        producers = configuredDatabase.GetType("Producers");
        ret = (producers.GetDataPointer() != NULL_PTR(void *));
    }
    numberOfProducers = 0u;
    if (ret) {
        numberOfProducers = producers.GetNumberOfElements(0u);
    }
    return ret;
}

bool DataSourceI::GetSignalConsumerName(uint32 signalIdx,
                                        const char8 *stateName,
                                        uint32 consumerIdx,
                                        StreamString &consumerName) {
    bool ret = MoveToSignalIndex(signalIdx);
    uint32 numberOfConsumers;
    if (ret) {
        ret = GetSignalNumberOfConsumers(signalIdx, stateName, numberOfConsumers);
    }
    if (ret) {
        ret = (consumerIdx < numberOfConsumers);
    }
    if (ret) {
        ret = MoveToSignalIndex(signalIdx);
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative("States");
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative(stateName);
    }
    if (ret) {
        StreamString *consumerArray = new StreamString[numberOfConsumers];
        Vector<StreamString> consumerVector(consumerArray, numberOfConsumers);
        ret = configuredDatabase.Read("Consumers", consumerVector);
        if (ret) {
            consumerName = consumerVector[consumerIdx];
        }
        delete[] consumerArray;
    }

    return ret;
}

bool DataSourceI::GetSignalProducerName(uint32 signalIdx,
                                        const char8 *stateName,
                                        uint32 producerIdx,
                                        StreamString &producerName) {
    bool ret = MoveToSignalIndex(signalIdx);
    uint32 numberOfProducers;
    if (ret) {
        ret = GetSignalNumberOfProducers(signalIdx, stateName, numberOfProducers);
    }
    if (ret) {
        ret = (producerIdx < numberOfProducers);
    }
    if (ret) {
        ret = MoveToSignalIndex(signalIdx);
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative("States");
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative(stateName);
    }
    if (ret) {
        StreamString *producerArray = new StreamString[numberOfProducers];
        Vector<StreamString> producerVector(producerArray, numberOfProducers);
        ret = configuredDatabase.Read("Producers", producerVector);
        if (ret) {
            producerName = producerVector[producerIdx];
        }
        delete[] producerArray;
    }

    return ret;
}

bool DataSourceI::MoveToSignalIndex(uint32 signalIdx) {
    bool ret = configuredDatabase.MoveToRoot();
    if (ret) {
        ret = configuredDatabase.MoveRelative("Signals");
    }
    StreamString signalIdxStr;
    signalIdxStr.Printf("%d", signalIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative(signalIdxStr.Buffer());
    }
    return ret;
}

uint32 DataSourceI::GetNumberOfFunctions() {
    bool ret = configuredDatabase.MoveToRoot();
    if (ret) {
        ret = configuredDatabase.MoveRelative("Functions");
    }
    uint32 numberOfFunctions = 0u;
    if (ret) {
        numberOfFunctions = configuredDatabase.GetNumberOfChildren();
    }
    return numberOfFunctions;
}

bool DataSourceI::GetFunctionName(uint32 functionIdx,
                                  StreamString &functionName) {
    bool ret = MoveToFunctionIndex(functionIdx);
    if (ret) {
        ret = configuredDatabase.Read("QualifiedName", functionName);
    }
    return ret;
}

bool DataSourceI::GetFunctionIndex(uint32 &functionIdx,
                                   const char8* const functionName) {

    uint32 numberOfFunctions = GetNumberOfFunctions();
    bool ret = true;
    bool found = false;
    for (functionIdx = 0u; (functionIdx < numberOfFunctions) && ret && (!found); functionIdx++) {
        StreamString searchName;
        ret = GetFunctionName(functionIdx, searchName);
        if (ret) {
            found = (StringHelper::Compare(functionName, searchName.Buffer()) == 0u);
        }
    }
    if (ret) {
        ret = found;
    }
    return ret;
}

bool DataSourceI::GetFunctionNumberOfSignals(SignalDirection direction,
                                             uint32 functionIdx,
                                             uint32 &numberOfSignals) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }
    bool ret = MoveToFunctionIndex(functionIdx);
    if (ret) {
        if (configuredDatabase.MoveRelative(signalDirection)) {
            //Ignore the ByteSize and the Address
            ret = (configuredDatabase.GetNumberOfChildren() > 2u);
        }
        if (ret) {
            numberOfSignals = configuredDatabase.GetNumberOfChildren() - 2u;
        }
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalsByteSize(SignalDirection direction,
                                             uint32 functionIdx,
                                             uint32 &byteSize) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }
    bool ret = MoveToFunctionIndex(functionIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative(signalDirection);
    }
    if (ret) {
        ret = configuredDatabase.Read("ByteSize", byteSize);
    }

    return ret;
}

bool DataSourceI::GetFunctionSignalsAddress(SignalDirection direction,
                                            uint32 functionIdx,
                                            void *&address) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }
    bool ret = MoveToFunctionIndex(functionIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative(signalDirection);
    }
    uint64 temp = 0u;
    if (ret) {
        ret = configuredDatabase.Read("Address", temp);
    }
    if (ret) {
        address = reinterpret_cast<void *>(temp);
    }

    return ret;
}

bool DataSourceI::GetFunctionSignalName(SignalDirection direction,
                                        uint32 functionIdx,
                                        uint32 functionSignalIdx,
                                        StreamString &functionSignalName) {

    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (ret) {
        ret = configuredDatabase.Read("QualifiedName", functionSignalName);
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalIndex(SignalDirection direction,
                                         uint32 functionIdx,
                                         uint32 &functionSignalIdx,
                                         const char8* const functionSignalName) {
    uint32 numberOfFunctionSignals = 0u;
    bool ret = GetFunctionNumberOfSignals(direction, functionIdx, numberOfFunctionSignals);
    bool found = false;
    for (functionSignalIdx = 0u; (functionSignalIdx < numberOfFunctionSignals) && ret && (!found); functionSignalIdx++) {
        StreamString searchName;
        ret = GetFunctionName(functionSignalIdx, searchName);
        if (ret) {
            found = (StringHelper::Compare(functionSignalName, searchName.Buffer()) == 0u);
        }
    }
    if (ret) {
        ret = found;
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalNumberOfByteOffsets(SignalDirection direction,
                                                       uint32 functionIdx,
                                                       uint32 functionSignalIdx,
                                                       uint32 &numberOfByteOffsets) {
    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    AnyType byteOffset;
    if (ret) {
        byteOffset = configuredDatabase.GetType("ByteOffset");
        ret = (byteOffset.GetDataPointer() != NULL_PTR(void *));
    }
    numberOfByteOffsets = 0u;
    if (ret) {
        numberOfByteOffsets = byteOffset.GetNumberOfElements(1u);
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalByteOffsetInfo(SignalDirection direction,
                                                  uint32 functionIdx,
                                                  uint32 functionSignalIdx,
                                                  uint32 byteOffsetIndex,
                                                  uint32 &byteOffsetStart,
                                                  uint32 &byteOffsetSize) {
    uint32 numberOfByteOffsets = 0u;
    bool ret = GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, functionSignalIdx, numberOfByteOffsets);
    if (ret) {
        ret = (numberOfByteOffsets > 0u);
    }
    if (ret) {
        ret = (numberOfByteOffsets > byteOffsetIndex);
    }
    if (ret) {
        ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    }

    Matrix<uint32> byteOffsetMat(numberOfByteOffsets, 2u);
    if (ret) {
        ret = configuredDatabase.Read("ByteOffset", byteOffsetMat);
    }
    if (ret) {
        byteOffsetStart = byteOffsetMat[byteOffsetIndex][0u];
        byteOffsetSize = byteOffsetMat[byteOffsetIndex][1u];
    }

    return ret;
}

bool DataSourceI::GetFunctionSignalTimeCyclesInfo(SignalDirection direction,
                                                  uint32 functionIdx,
                                                  uint32 functionSignalIdx,
                                                  uint32 &timeCycles,
                                                  uint32 &timeSamples) {

    Vector<uint32> timeCyclesSamplesVec(2u);
    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (configuredDatabase.Read("TimeCyclesSamples", timeCyclesSamplesVec)) {
        timeCycles = timeCyclesSamplesVec[0u];
        timeSamples = timeCyclesSamplesVec[1u];
    }
    else {
        timeCycles = 1u;
        timeSamples = 1u;
    }
    return ret;
}

bool DataSourceI::MoveToFunctionIndex(uint32 functionIdx) {
    bool ret = configuredDatabase.MoveToRoot();
    if (ret) {
        ret = configuredDatabase.MoveRelative("Functions");
    }
    StreamString functionIdxStr;
    functionIdxStr.Printf("%d", functionIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative(functionIdxStr.Buffer());
    }
    return ret;
}

bool DataSourceI::MoveToFunctionSignalIndex(SignalDirection direction,
                                            uint32 functionIdx,
                                            uint32 functionSignalIdx) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }
    bool ret = MoveToFunctionIndex(functionIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative(signalDirection);
    }
    StreamString functionSignalIdxStr;
    functionSignalIdxStr.Printf("%d", functionSignalIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative(functionSignalIdxStr.Buffer());
    }
    return ret;
}

}
