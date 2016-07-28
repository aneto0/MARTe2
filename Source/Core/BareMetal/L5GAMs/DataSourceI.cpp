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
#include "MemoryMapBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "ReferenceContainerFilterReferences.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

DataSourceI::DataSourceI() :
        ReferenceContainer() {
    numberOfSignals = 0u;
}

DataSourceI::~DataSourceI() {

}

bool DataSourceI::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (data.MoveRelative("Signals")) {
        signalsDatabase.Write("Signals", data);
        ret = data.MoveToAncestor(1);
    }
    if (ret) {
        ret = signalsDatabase.MoveToRoot();
    }

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
    uint32 i;
    for (i = 0u; (i < numberOfSignals) && (ret) && (!found); i++) {
        StreamString searchName;
        ret = GetSignalName(i, searchName);
        if (ret) {
            found = (StringHelper::Compare(signalName, searchName.Buffer()) == 0u);
            signalIdx = i;
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

bool DataSourceI::GetSignalNumberOfElements(uint32 signalIdx,
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
        consumers = configuredDatabase.GetType("GAMConsumers");
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
        producers = configuredDatabase.GetType("GAMProducers");
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
        ret = configuredDatabase.Read("GAMNamesConsumers", consumerVector);
        if (ret) {
            consumerName = consumerVector[consumerIdx];
        }
        if (ret) {

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
        ret = configuredDatabase.Read("GAMNamesProducers", producerVector);
        if (ret) {
            producerName = producerVector[producerIdx];
        }
        delete[] producerArray;
    }

    return ret;
}

bool DataSourceI::GetSignalDefaultValue(uint32 signalIdx,
                                        const AnyType &defaultValue) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("Default", defaultValue);
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
    uint32 i;
    for (i = 0u; (i < numberOfFunctions) && (ret) && (!found); i++) {
        StreamString searchName;
        ret = GetFunctionName(i, searchName);
        if (ret) {
            found = (StringHelper::Compare(functionName, searchName.Buffer()) == 0u);
            functionIdx = i;
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
    numberOfSignals = 0u;
    bool ret = MoveToFunctionIndex(functionIdx);
    if (ret) {
        if (configuredDatabase.MoveRelative(signalDirection)) {
            //Ignore the ByteSize
            ret = (configuredDatabase.GetNumberOfChildren() > 1u);
            if (ret) {
                numberOfSignals = configuredDatabase.GetNumberOfChildren() - 1u;
            }
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
    byteSize = 0u;
    bool ret = MoveToFunctionIndex(functionIdx);
    if (ret) {
        if (configuredDatabase.MoveRelative(signalDirection)) {
            ret = configuredDatabase.Read("ByteSize", byteSize);
        }
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

bool DataSourceI::GetFunctionSignalAlias(SignalDirection direction,
                                         uint32 functionIdx,
                                         uint32 functionSignalIdx,
                                         StreamString &functionSignalAlias) {

    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (ret) {
        if (!configuredDatabase.Read("Alias", functionSignalAlias)) {
            functionSignalAlias = "";
        }
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
    uint32 i;
    for (i = 0u; (i < numberOfFunctionSignals) && (ret) && (!found); i++) {
        StreamString searchName;
        ret = GetFunctionSignalName(direction, functionIdx, i, searchName);
        if (ret) {
            found = (StringHelper::Compare(functionSignalName, searchName.Buffer()) == 0u);
            functionSignalIdx = i;
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
    numberOfByteOffsets = 0u;
    if (ret) {
        byteOffset = configuredDatabase.GetType("ByteOffset");
        if (byteOffset.GetDataPointer() != NULL_PTR(void *)) {
            numberOfByteOffsets = byteOffset.GetNumberOfElements(1u);
        }
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

bool DataSourceI::GetFunctionSignalSamples(SignalDirection direction,
                                           uint32 functionIdx,
                                           uint32 functionSignalIdx,
                                           uint32 &samples) {
    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (!configuredDatabase.Read("Samples", samples)) {
        samples = 1u;
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalReadFrequency(SignalDirection direction,
                                                 uint32 functionIdx,
                                                 uint32 functionSignalIdx,
                                                 float32 &frequency) {
    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (!configuredDatabase.Read("Frequency", frequency)) {
        frequency = -1.0;
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalGAMMemoryOffset(SignalDirection direction,
                                                   uint32 functionIdx,
                                                   uint32 functionSignalIdx,
                                                   uint32 &memoryOffset) {

    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (ret) {
        ret = configuredDatabase.Read("GAMMemoryOffset", memoryOffset);
    }
    return ret;
}

bool DataSourceI::IsSupportedBroker(SignalDirection direction,
                                    uint32 functionIdx,
                                    uint32 functionSignalIdx,
                                    const char8* brokerClassName) {
    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (ret) {
        StreamString broker;
        ret = configuredDatabase.Read("Broker", broker);
        if (ret) {
            ret = (broker == brokerClassName);
        }
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

bool DataSourceI::AddBrokers(SignalDirection direction) {
    // Each ds has a Functions area
    // For each Function allocate memory
    // Search the signal and get the memory pointer for each signal linked it to the correct broker
    // Assign the broker to the gam
    const char8 * dirStr = "InputSignals";
    if (direction == OutputSignals) {
        dirStr = "OutputSignals";
    }

    //Find the application name
    ReferenceContainer result;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH, this);
    ObjectRegistryDatabase::Instance()->ReferenceContainer::Find(result, filter);
    ReferenceT<RealTimeApplication> application;
    uint32 c;
    bool found = false;
    for (c = 0u; (c < result.Size()) && (!found); c++) {
        application = result.Get(c);
        found = (application.IsValid());
    }
    bool ret = found;

    if (ret) {
        configuredDatabase.MoveAbsolute("Functions");
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "No RealTimeApplication found for DataSourceI : %s", GetName())
    }
    if (ret) {
        uint32 numberOfFunctions = configuredDatabase.GetNumberOfChildren();
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            const char8* functionId = configuredDatabase.GetChildName(i);
            ret = configuredDatabase.MoveRelative(functionId);
            StreamString functionName;
            if (ret) {
                ret = configuredDatabase.Read("QualifiedName", functionName);
            }
            if (ret) {
                StreamString fullFunctionName = "Functions.";
                fullFunctionName += functionName;

                ReferenceT<GAM> gam = application->Find(fullFunctionName.Buffer());
                ret = gam.IsValid();
                void *gamMemoryAddress = NULL_PTR(void *);

                bool relevant = false;
                if (direction == InputSignals) {
                    if (gam->GetNumberOfInputSignals() > 0u) {
                        gamMemoryAddress = gam->GetInputMemoryPointer();
                        relevant = true;
                    }
                }
                else if (direction == OutputSignals) {
                    if (gam->GetNumberOfOutputSignals() > 0u) {
                        gamMemoryAddress = gam->GetOutputMemoryPointer();
                        relevant = true;
                    }
                }

                if (relevant) {
                    if (ret) {
                        ret = (gamMemoryAddress != NULL);
                    }
                    if (ret) {
                        if (configuredDatabase.MoveRelative(dirStr)) {
                            if (direction == InputSignals) {
                                ReferenceContainer inputBrokers;
                                ret = GetInputBrokers(inputBrokers, functionName.Buffer(), gamMemoryAddress);
                                if (ret) {
                                    gam->AddInputBrokers(inputBrokers);
                                }
                            }
                            else {
                                ReferenceContainer outputBrokers;
                                ret = GetOutputBrokers(outputBrokers, functionName.Buffer(), gamMemoryAddress);
                                if (ret) {
                                    gam->AddOutputBrokers(outputBrokers);
                                }
                            }

                        }
                    }
                }
            }
            if (ret) {
                ret = configuredDatabase.MoveAbsolute("Functions");
            }
        }
    }
    return ret;
}

}
