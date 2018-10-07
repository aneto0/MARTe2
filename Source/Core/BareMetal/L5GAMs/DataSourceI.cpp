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
        ret = signalsDatabase.Write("Signals", data);
        if (ret) {
            ret = data.MoveToAncestor(1u);
        }
    }
    if (ret) {
        ret = signalsDatabase.MoveToRoot();
    }

    return ret;
}

bool DataSourceI::AddSignals(StructuredDataI &data) {
    bool ret;
    /*lint -e{534} [MISRA C++ Rule 0-1-7], [MISRA C++ Rule 0-3-2].
     *Justification: "Signals" is an optional field, so the move command must
     *Justification: not be checked.*/
    signalsDatabase.MoveAbsolute("Signals");
    ret = data.Write("Signals", signalsDatabase.operator MARTe::AnyType());
    return ret;
}

bool DataSourceI::SetConfiguredDatabase(StructuredDataI & data) {
    configuredDatabase = dynamic_cast<ConfigurationDatabase &>(data);
    configuredDatabase.SetCurrentNodeAsRootNode();
    if (configuredDatabase.MoveAbsolute("Functions")) {
        functionsDatabaseNode = configuredDatabase;
    }
    bool ret = configuredDatabase.MoveAbsolute("Signals");
    if (ret) {
        signalsDatabaseNode = configuredDatabase;
        numberOfSignals = configuredDatabase.GetNumberOfChildren();
    }
    return ret;
}

uint32 DataSourceI::GetNumberOfSignals() const {
    return numberOfSignals;
}

bool DataSourceI::GetSignalName(const uint32 signalIdx, DynamicCString &signalName) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("QualifiedName", signalName);
    }
    return ret;
}

bool DataSourceI::GetSignalIndex(uint32 &signalIdx, CCString signalName) {

    const uint32 numSignals = GetNumberOfSignals();
    bool ret = true;
    bool found = false;
    uint32 i;
    for (i = 0u; (i < numSignals) && (ret) && (!found); i++) {
        DynamicCString searchName;
        ret = GetSignalName(i, searchName);
        if (ret) {
            found = (signalName == searchName);
            signalIdx = i;
        }
    }
    if (ret) {
        ret = found;
    }
    return ret;
}

TypeDescriptor DataSourceI::GetSignalType(const uint32 signalIdx) {
    TypeDescriptor signalTypeDescriptor = InvalidType(0);
    bool ret = MoveToSignalIndex(signalIdx);
    DynamicCString signalType;
    if (ret) {
        ret = configuredDatabase.Read("Type", signalType);
    }
    if (ret) {
        signalTypeDescriptor = TypeDescriptor(signalType);
    }
    return signalTypeDescriptor;
}

bool DataSourceI::GetSignalNumberOfDimensions(const uint32 signalIdx, uint8 &numberOfDimensions) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("NumberOfDimensions", numberOfDimensions);
    }
    return ret;
}

bool DataSourceI::GetSignalNumberOfElements(const uint32 signalIdx, uint32 &numberOfElements) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("NumberOfElements", numberOfElements);
    }
    return ret;
}

bool DataSourceI::GetSignalByteSize(const uint32 signalIdx, uint32 &byteSize) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        if (!configuredDatabase.Read("MemberSize", byteSize)) {
            ret = configuredDatabase.Read("ByteSize", byteSize);
        }
    }
    return ret;
}

bool DataSourceI::GetSignalNumberOfStates(const uint32 signalIdx, uint32 &numberOfStates) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative("States");
    }
    if (ret) {
        numberOfStates = configuredDatabase.GetNumberOfChildren();
    }
    return ret;
}

bool DataSourceI::GetSignalStateName(const uint32 signalIdx, const uint32 stateIdx, DynamicCString &stateName) {
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
        ret = (stateIdx < numberOfStates);
    }
    if (ret) {
        ret = MoveToSignalIndex(signalIdx);
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative("States");
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
        stateName = configuredDatabase.GetChildName(stateIdx).GetList();
    }

    return ret;
}

bool DataSourceI::GetSignalNumberOfConsumers(const uint32 signalIdx, CCString stateName, uint32 &numberOfConsumers) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative("States");
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative(stateName);
    }
    if (ret){
    	uint32 maxDim = 1;
    	TypeDescriptor td;
    	ret = configuredDatabase.GetVariableInformation("GAMConsumers",td,maxDim,&numberOfConsumers).ErrorsCleared();
    	if (maxDim == 0){
    		numberOfConsumers = 1;
    	}
    }
    return ret;
}

bool DataSourceI::GetSignalNumberOfProducers(const uint32 signalIdx, CCString stateName, uint32 &numberOfProducers) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative("States");
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative(stateName);
    }
    if (ret){
    	uint32 maxDim = 1;
    	TypeDescriptor td;
    	ret = configuredDatabase.GetVariableInformation("GAMProducers",td,maxDim,&numberOfProducers).ErrorsCleared();
    	if (maxDim == 0){
    		numberOfProducers = 1;
    	}
    }
    return ret;
}

bool DataSourceI::GetSignalConsumerName(const uint32 signalIdx, CCString stateName, const uint32 consumerIdx, DynamicCString &consumerName) {
    bool ret = MoveToSignalIndex(signalIdx);
    uint32 numberOfConsumers = 0u;
    if (ret) {
        ret = GetSignalNumberOfConsumers(signalIdx, stateName, numberOfConsumers);
    }
    if (ret) {
        if (numberOfConsumers > 0u) {
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
                DynamicCString *consumerArray = new DynamicCString[numberOfConsumers];
                Vector<DynamicCString> consumerVector(consumerArray, numberOfConsumers);
                ret = configuredDatabase.Read("GAMNamesConsumers", consumerVector);
                if (ret) {
                    consumerName = consumerVector[consumerIdx];
                }
                if (ret) {

                }
                delete[] consumerArray;
            }
        }
        else {
            ret = false;
        }
    }
    return ret;
}

bool DataSourceI::GetSignalProducerName(const uint32 signalIdx, CCString stateName, const uint32 producerIdx, DynamicCString &producerName) {
    bool ret = MoveToSignalIndex(signalIdx);
    uint32 numberOfProducers = 0u;
    if (ret) {
        ret = GetSignalNumberOfProducers(signalIdx, stateName, numberOfProducers);
    }
    if (ret) {
        if (numberOfProducers > 0u) {
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
                DynamicCString *producerArray = new DynamicCString[numberOfProducers];
                Vector<DynamicCString> producerVector(producerArray, numberOfProducers);
                ret = configuredDatabase.Read("GAMNamesProducers", producerVector);
                if (ret) {
                    producerName = producerVector[producerIdx];
                }
                delete[] producerArray;
            }
        }
        else {
            ret = false;
        }
    }
    return ret;
}

bool DataSourceI::GetSignalDefaultValue(const uint32 signalIdx, const AnyType &defaultValue) {
    bool ret = MoveToSignalIndex(signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("Default", defaultValue);
    }
    return ret;
}

#if 0
AnyType DataSourceI::GetSignalDefaultValueType(const uint32 signalIdx) {
    AnyType retType = voidAnyType;
    if (MoveToSignalIndex(signalIdx)) {
        retType = configuredDatabase.GetType("Default");
    }
    return retType;
}

#endif

bool DataSourceI::MoveToSignalIndex(const uint32 signalIdx) {
    configuredDatabase = signalsDatabaseNode;
    return configuredDatabase.MoveToChild(signalIdx);
}

uint32 DataSourceI::GetNumberOfFunctions() {
    configuredDatabase = functionsDatabaseNode;
    return configuredDatabase.GetNumberOfChildren();
}

bool DataSourceI::GetFunctionName(const uint32 functionIdx, DynamicCString &functionName) {
    bool ret = MoveToFunctionIndex(functionIdx);
    if (ret) {
        ret = configuredDatabase.Read("QualifiedName", functionName);
    }
    return ret;
}

bool DataSourceI::GetFunctionIndex(uint32 &functionIdx, CCString functionName) {

    uint32 numberOfFunctions = GetNumberOfFunctions();
    bool ret = true;
    bool found = false;
    uint32 i;
    for (i = 0u; (i < numberOfFunctions) && (ret) && (!found); i++) {
        DynamicCString searchName;
        ret = GetFunctionName(i, searchName);
        if (ret) {
            found = (functionName == searchName) ;
            functionIdx = i;
        }
    }
    if (ret) {
        ret = found;
    }
    return ret;
}

bool DataSourceI::GetFunctionNumberOfSignals(const SignalDirection direction, const uint32 functionIdx, uint32 &numSignals) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }
    numSignals = 0u;
    bool ret = MoveToFunctionIndex(functionIdx);
    if (ret) {
        if (configuredDatabase.MoveRelative(signalDirection)) {
            //Ignore the ByteSize
            ret = (configuredDatabase.GetNumberOfChildren() > 1u);
            if (ret) {
                numSignals = configuredDatabase.GetNumberOfChildren() - 1u;
            }
        }
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalsByteSize(const SignalDirection direction, const uint32 functionIdx, uint32 &byteSize) {
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

bool DataSourceI::GetFunctionSignalName(const SignalDirection direction, const uint32 functionIdx, const uint32 functionSignalIdx, DynamicCString &functionSignalName) {

    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (ret) {
        ret = configuredDatabase.Read("QualifiedName", functionSignalName);
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalAlias(const SignalDirection direction, const uint32 functionIdx, const uint32 functionSignalIdx, DynamicCString &functionSignalAlias) {

    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (ret) {
        if (!configuredDatabase.Read("Alias", functionSignalAlias)) {
            functionSignalAlias = "";
        }
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalIndex(const SignalDirection direction, const uint32 functionIdx, uint32 &functionSignalIdx, CCString functionSignalName) {
    uint32 numberOfFunctionSignals = 0u;
    bool ret = GetFunctionNumberOfSignals(direction, functionIdx, numberOfFunctionSignals);
    bool found = false;
    uint32 i;
    for (i = 0u; (i < numberOfFunctionSignals) && (ret) && (!found); i++) {
        DynamicCString searchName;
        ret = GetFunctionSignalName(direction, functionIdx, i, searchName);
        if (ret) {
            found = (functionSignalName == searchName);
            functionSignalIdx = i;
        }
    }
    if (ret) {
        ret = found;
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalNumberOfByteOffsets(const SignalDirection direction, const uint32 functionIdx, const uint32 functionSignalIdx, uint32 &numberOfByteOffsets) {
    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    AnyType byteOffset;
    numberOfByteOffsets = 0u;

    if (ret){
    	uint32 maxDim = 1;
    	TypeDescriptor td;
    	ret = configuredDatabase.GetVariableInformation("ByteOffset",td,maxDim,&numberOfByteOffsets).ErrorsCleared();
    	if (maxDim == 0){
    		numberOfByteOffsets = 1;
    	}
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalByteOffsetInfo(const SignalDirection direction, const uint32 functionIdx, const uint32 functionSignalIdx, const uint32 byteOffsetIndex, uint32 &byteOffsetStart,
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
        byteOffsetStart = byteOffsetMat(byteOffsetIndex, 0u);
        byteOffsetSize = byteOffsetMat(byteOffsetIndex, 1u);
    }

    return ret;
}

bool DataSourceI::GetFunctionSignalSamples(const SignalDirection direction, const uint32 functionIdx, const uint32 functionSignalIdx, uint32 &samples) {
    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (!configuredDatabase.Read("Samples", samples)) {
        samples = 1u;
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalReadFrequency(const SignalDirection direction, const uint32 functionIdx, const uint32 functionSignalIdx, float32 &frequency) {
    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (!configuredDatabase.Read("Frequency", frequency)) {
        frequency = -1.0F;
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalTrigger(const SignalDirection direction, const uint32 functionIdx, const uint32 functionSignalIdx, uint32 &trigger) {
    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (!configuredDatabase.Read("Trigger", trigger)) {
        trigger = 0u;
    }
    return ret;
}

bool DataSourceI::GetFunctionSignalGAMMemoryOffset(const SignalDirection direction, const uint32 functionIdx, const uint32 functionSignalIdx, uint32 &memoryOffset) {

    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (ret) {
        ret = configuredDatabase.Read("GAMMemoryOffset", memoryOffset);
    }
    return ret;
}

bool DataSourceI::IsSupportedBroker(const SignalDirection direction, const uint32 functionIdx, const uint32 functionSignalIdx, CCString brokerClassName) {
    bool ret = MoveToFunctionSignalIndex(direction, functionIdx, functionSignalIdx);
    if (ret) {
        DynamicCString broker;
        ret = configuredDatabase.Read("Broker", broker);
        if (ret) {
            ret = (broker == brokerClassName);
        }
    }
    return ret;
}

bool DataSourceI::MoveToFunctionIndex(const uint32 functionIdx) {
    configuredDatabase = functionsDatabaseNode;
    return configuredDatabase.MoveToChild(functionIdx);
}

bool DataSourceI::MoveToFunctionSignalIndex(const SignalDirection direction, const uint32 functionIdx, const uint32 functionSignalIdx) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }
    bool ret = MoveToFunctionIndex(functionIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative(signalDirection);
    }
    if (ret) {
        ret = configuredDatabase.MoveToChild(functionSignalIdx);
    }
    return ret;
}

bool DataSourceI::AddBrokers(const SignalDirection direction) {
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
        configuredDatabase = functionsDatabaseNode;
        uint32 numberOfFunctions = configuredDatabase.GetNumberOfChildren();

        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            configuredDatabase = functionsDatabaseNode;
            ret = configuredDatabase.MoveToChild(i);
            DynamicCString functionName;
            if (ret) {
                ret = configuredDatabase.Read("QualifiedName", functionName);
            }
            if (ret) {
                DynamicCString fullFunctionName = "Functions.";
                fullFunctionName.Append(functionName);

                ReferenceT<GAM> gam = application->Find(fullFunctionName);
                ret = gam.IsValid();
                void *gamMemoryAddress = NULL_PTR(void *);

                bool relevant = false;
                if (direction == InputSignals) {
                    if (gam->GetNumberOfInputSignals() > 0u) {
                        gamMemoryAddress = gam->GetInputSignalsMemory();
                        relevant = true;
                    }
                }
                else if (direction == OutputSignals) {
                    if (gam->GetNumberOfOutputSignals() > 0u) {
                        gamMemoryAddress = gam->GetOutputSignalsMemory();
                        relevant = true;
                    }
                }
                else {
                    //There is no direction set
                    REPORT_ERROR(ErrorManagement::FatalError, "No direction set when adding brokers to DataSourceI");
                    ret = false;
                }

                if (relevant) {
                    if (ret) {
                        ret = (gamMemoryAddress != NULL);
                    }
                    if (ret) {
                        if (configuredDatabase.MoveRelative(dirStr)) {
                            if (direction == InputSignals) {
                                ReferenceContainer inputBrokers;
                                ret = GetInputBrokers(inputBrokers, functionName, gamMemoryAddress);
                                if (ret) {
                                    ret = gam->AddInputBrokers(inputBrokers);
                                }
                            }
                            else {
                                ReferenceContainer outputBrokers;
                                ret = GetOutputBrokers(outputBrokers, functionName, gamMemoryAddress);
                                if (ret) {
                                    ret = gam->AddOutputBrokers(outputBrokers);
                                }
                            }

                        }
                    }
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "No RealTimeApplication found for DataSourceI");
    }
    return ret;
}

bool DataSourceI::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                     CCString functionName,
                                                     void * const gamMemPtr) {

    uint32 functionIdx = 0u;
    bool ret = GetFunctionIndex(functionIdx, functionName);

    uint32 numSignals = 0u;
    if (ret) {
        ret = GetFunctionNumberOfSignals(InputSignals, functionIdx, numSignals);
    }

    for (uint32 i = 0u; (i < numSignals) && (ret); i++) {
        //search the GAM in the configured database
        ret = MoveToFunctionSignalIndex(InputSignals, functionIdx, i);
        DynamicCString suggestedBrokerNameIn;
        if (ret) {
            //This was returned by GetBrokerName
            ret = configuredDatabase.Read("Broker", suggestedBrokerNameIn);
        }

        bool isSyncOrTrigger = false;
        if (ret) {
            float32 freq;
            ret = GetFunctionSignalReadFrequency(InputSignals, functionIdx, i, freq);
            if (ret) {
                isSyncOrTrigger = (freq >= 0.F);
            }
        }
        if (ret) {
            if (!isSyncOrTrigger) {
                uint32 trigger;
                ret = GetFunctionSignalTrigger(InputSignals, functionIdx, i, trigger);
                if (ret) {
                    isSyncOrTrigger = (trigger > 0u);
                }
            }
        }
        if (ret) {
            DynamicCString signalName;
            ret = GetFunctionSignalAlias(InputSignals, functionIdx, i, signalName);
            uint32 signalIdx = 0u;
            if (ret) {
                ret = GetSignalIndex(signalIdx, signalName);
            }
            if (ret) {
                bool createBroker = true;
                uint32 nBrokers = inputBrokers.Size();
                for (uint32 j = 0u; (j < nBrokers) && (ret) && (createBroker); j++) {
                    Reference brokerRef = inputBrokers.Get(j);
                    if (brokerRef.IsValid()) {
                        if (suggestedBrokerNameIn == brokerRef->GetClassRegistryItem()->GetClassName()) {
                            createBroker = false;
                        }
                    }
                }
                if (createBroker) {
                	FORMATTED_REPORT_ERROR(ErrorManagement::Information, "Creating broker %s for %s and signal %s(%d)", suggestedBrokerNameIn,
                                            functionName, signalName, signalIdx);

                    ReferenceT<BrokerI> broker(suggestedBrokerNameIn);

                    ret = broker.IsValid();
                    if (!ret) {
                    	FORMATTED_REPORT_ERROR(ErrorManagement::FatalError, "Broker %s not valid", suggestedBrokerNameIn);
                    }
                    if (ret) {
                        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
                        if (ret) {
                            //Insert at the beginning the sync/trigger broker
                            if (isSyncOrTrigger) {
                                ret = inputBrokers.Insert(broker, 0);
                            }
                            else {
                                ret = inputBrokers.Insert(broker);
                            }
                        }
                        else {
                        	FORMATTED_REPORT_ERROR(ErrorManagement::FatalError, "Failed broker %s Init", suggestedBrokerNameIn);
                        }

                    }
                }
            }
        }
    }
    return ret;
}


bool DataSourceI::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                      CCString functionName,
                                                      void * const gamMemPtr) {
    uint32 functionIdx = 0u;
    bool ret = GetFunctionIndex(functionIdx, functionName);

    uint32 numSignals = 0u;
    if (ret) {
        ret = GetFunctionNumberOfSignals(OutputSignals, functionIdx, numSignals);
    }

    for (uint32 i = 0u; (i < numSignals) && (ret); i++) {
        //search the gam in the configured database
        ret = MoveToFunctionSignalIndex(OutputSignals, functionIdx, i);
        DynamicCString suggestedBrokerNameIn;
        if (ret) {
            //lets try...
            ret = configuredDatabase.Read("Broker", suggestedBrokerNameIn);
        }

        bool isSyncOrTrigger = false;
        if (ret) {
            float32 freq;
            ret = GetFunctionSignalReadFrequency(OutputSignals, functionIdx, i, freq);
            if (ret) {
                isSyncOrTrigger = (freq > 0.F);
            }
        }
        if (ret) {
            if (!isSyncOrTrigger) {
                uint32 trigger;
                ret = GetFunctionSignalTrigger(OutputSignals, functionIdx, i, trigger);
                if (ret) {
                    isSyncOrTrigger = (trigger > 0u);
                }
            }
        }
        if (ret) {
            DynamicCString signalName;
            ret = GetFunctionSignalAlias(OutputSignals, functionIdx, i, signalName);
            uint32 signalIdx = 0u;
            if (ret) {
                ret = GetSignalIndex(signalIdx, signalName);
            }
            if (ret) {
                bool createBroker = true;
                uint32 nBrokers = outputBrokers.Size();
                for (uint32 j = 0u; (j < nBrokers) && (ret) && (createBroker); j++) {
                    Reference brokerRef = outputBrokers.Get(j);
                    if (brokerRef.IsValid()) {
                        if (suggestedBrokerNameIn == brokerRef->GetClassRegistryItem()->GetClassName()) {
                            createBroker = false;
                        }
                    }
                }

                if (createBroker) {
                	FORMATTED_REPORT_ERROR(ErrorManagement::Information, "Creating broker %s for %s and signal %s(%d)", suggestedBrokerNameIn,
                                            functionName, signalName, signalIdx);

                    ReferenceT<BrokerI> broker(suggestedBrokerNameIn);

                    ret = broker.IsValid();
                    if (!ret) {
                    	FORMATTED_REPORT_ERROR(ErrorManagement::FatalError, "Broker %s not valid", suggestedBrokerNameIn);
                    }
                    if (ret) {
                        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
                        if (ret) {
                            //Insert at the end the sync/trigger broker
                            if (!isSyncOrTrigger) {
                                ret = outputBrokers.Insert(broker, 0);
                            }
                            else {
                                ret = outputBrokers.Insert(broker);
                            }
                        }
                        else {
                        	FORMATTED_REPORT_ERROR(ErrorManagement::FatalError, "Failed broker %s Init", suggestedBrokerNameIn);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

void DataSourceI::Purge(ReferenceContainer &purgeList){
    signalsDatabaseNode.Purge(purgeList);
    functionsDatabaseNode.Purge(purgeList);
    ReferenceContainer::Purge(purgeList);
}

uint32 DataSourceI::GetNumberOfStatefulMemoryBuffers() {
    return 1u;
}

uint32 DataSourceI::GetNumberOfMemoryBuffers() {
    return 1u;
}

uint32 DataSourceI::GetCurrentStateBuffer() {
    return 0u;
}

/*lint -estring(1960, "*external side-effects*") only the default implementation has no external side effects*/
void DataSourceI::PrepareInputOffsets() {
}

void DataSourceI::PrepareOutputOffsets() {
}

/*lint -e{715} The symbols are not referenced because this is a default implementation, i.e. it is expected to be implemented on derived classes.*/
bool DataSourceI::GetInputOffset(const uint32 signalIdx, const uint32 numberOfSamples, uint32 &offset) {
    return false;
}

/*lint -e{715} The symbols are not referenced because this is a default implementation, i.e. it is expected to be implemented on derived classes.*/
bool DataSourceI::GetOutputOffset(const uint32 signalIdx, const uint32 numberOfSamples, uint32 &offset) {
    return false;
}

/*lint -e{715} The symbols are not referenced because this is a default implementation, i.e. it is expected to be implemented on derived classes.*/
bool DataSourceI::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples) {
    return true;
}

/*lint -e{715} The symbols are not referenced because this is a default implementation, i.e. it is expected to be implemented on derived classes.*/
bool DataSourceI::TerminateOutputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples) {
    return true;
}

}
