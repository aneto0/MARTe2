/**
 * @file GAM.cpp
 * @brief Source file for class GAM
 * @date 07/04/2016
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
 * the class GAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BrokerI.h"
#include "GAM.h"
#include "Reference.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

GAM::GAM() :
        ReferenceContainer(),
        ExecutableI() {
    numberOfInputSignals = 0u;
    numberOfOutputSignals = 0u;
    inputSignalsMemory = NULL_PTR(void *);
    outputSignalsMemory = NULL_PTR(void *);
    inputSignalsMemoryIndexer = NULL_PTR(void **);
    outputSignalsMemoryIndexer = NULL_PTR(void **);
    heap = GlobalObjectsDatabase::Instance()->GetStandardHeap();
}

/*lint -e{1551} no exception should be thrown*/
GAM::~GAM() {
    if (inputSignalsMemory != NULL_PTR(void *)) {
        heap->Free(inputSignalsMemory);
    }
    if (outputSignalsMemory != NULL_PTR(void *)) {
        heap->Free(outputSignalsMemory);
    }
    if (inputSignalsMemoryIndexer != NULL_PTR(void **)) {
        delete[] inputSignalsMemoryIndexer;
    }
    if (outputSignalsMemoryIndexer != NULL_PTR(void **)) {
        delete[] outputSignalsMemoryIndexer;
    }
    /*lint -e{1740} pointer member 'heap' points to a static object
     * returned by GlobalObjectsDatabase::Instance()->GetStandardHeap() */
}

bool GAM::Initialise(StructuredDataI & data) {

    bool ret = ReferenceContainer::Initialise(data);
    if (data.MoveRelative("InputSignals")) {
        ret = signalsDatabase.Write("InputSignals", data);
        if (ret) {
            ret = data.MoveToAncestor(1u);
        }
    }
    if (ret) {
        ret = signalsDatabase.MoveToRoot();
    }
    if (ret) {
        if (data.MoveRelative("OutputSignals")) {
            ret = signalsDatabase.Write("OutputSignals", data);
            if (ret) {
                ret = data.MoveToAncestor(1u);
            }
        }
    }
    if (ret) {
        ret = signalsDatabase.MoveToRoot();
    }
    return ret;
}

bool GAM::AddSignals(StructuredDataI &data) {
    return data.Write("Signals", signalsDatabase);
}

bool GAM::AllocateInputSignalsMemory() {
    const char8* dirStr = "Signals.InputSignals";
    bool ret = configuredDatabase.MoveToRoot();
    if (ret) {
        ret = (inputSignalsMemory == NULL_PTR(void *));
    }
    if ((ret) && (numberOfInputSignals > 0u)) {
        ret = configuredDatabase.MoveRelative(dirStr);
        uint32 totalByteSize = 0u;
        if (ret) {
            ret = configuredDatabase.Read("ByteSize", totalByteSize);
        }
        if (ret) {
            inputSignalsMemory = heap->Malloc(totalByteSize);
            if (inputSignalsMemory != NULL_PTR(void*)) {
                ret = MemoryOperationsHelper::Set(inputSignalsMemory, '\0', totalByteSize);
            }
            else {
                ret = false;
            }
        }
        if (ret) {
            if (inputSignalsMemory != NULL_PTR(void*)) {
                inputSignalsMemoryIndexer = new void*[numberOfInputSignals];
                uint32 i;
                uint32 totalByteOffset = 0u;
                char8 *inputSignalsMemoryChar = reinterpret_cast<char8 *>(inputSignalsMemory);
                for (i = 0u; (i < numberOfInputSignals) && (ret); i++) {
                    inputSignalsMemoryIndexer[i] = reinterpret_cast<void *>(&inputSignalsMemoryChar[totalByteOffset]);

                    uint32 numberOfByteOffsets = 0u;
                    uint32 thisSignalByteOffset = 0u;
                    ret = GetSignalNumberOfByteOffsets(InputSignals, i, numberOfByteOffsets);
                    if (ret) {
                        ret = GetSignalByteSize(InputSignals, i, thisSignalByteOffset);

                        if (ret) {
                            uint32 samples = 1u;
                            if (GetSignalNumberOfSamples(InputSignals, i, samples)) {
                                thisSignalByteOffset *= samples;
                            }
                        }
                        if (ret) {
                            totalByteOffset += thisSignalByteOffset;
                        }
                    }
                }
            }
        }
    }

    return ret;
}

bool GAM::AllocateOutputSignalsMemory() {
    const char8* dirStr = "Signals.OutputSignals";
    bool ret = configuredDatabase.MoveToRoot();
    if (ret) {
        ret = (outputSignalsMemory == NULL_PTR(void *));
    }
    if ((ret) && (numberOfOutputSignals > 0u)) {
        ret = configuredDatabase.MoveRelative(dirStr);
        uint32 totalByteSize = 0u;
        if (ret) {
            ret = configuredDatabase.Read("ByteSize", totalByteSize);
        }
        if (ret) {
            outputSignalsMemory = heap->Malloc(totalByteSize);
            if (outputSignalsMemory != NULL_PTR(void*)) {
                ret = MemoryOperationsHelper::Set(outputSignalsMemory, '\0', totalByteSize);
            }
            else {
                ret = false;
            }
        }
        if (ret) {
            if (outputSignalsMemory != NULL_PTR(void*)) {
                outputSignalsMemoryIndexer = new void*[numberOfOutputSignals];
                uint32 i;
                uint32 totalByteOffset = 0u;
                char8 *outputSignalsMemoryChar = reinterpret_cast<char8 *>(outputSignalsMemory);
                for (i = 0u; (i < numberOfOutputSignals) && (ret); i++) {
                    outputSignalsMemoryIndexer[i] = reinterpret_cast<void *>(&outputSignalsMemoryChar[totalByteOffset]);

                    uint32 numberOfByteOffsets = 0u;
                    uint32 thisSignalByteOffset = 0u;
                    ret = GetSignalNumberOfByteOffsets(OutputSignals, i, numberOfByteOffsets);
                    if (ret) {
                        ret = GetSignalByteSize(OutputSignals, i, thisSignalByteOffset);

                        if (ret) {
                            uint32 samples = 1u;
                            if (GetSignalNumberOfSamples(OutputSignals, i, samples)) {
                                thisSignalByteOffset *= samples;
                            }
                        }
                        if (ret) {
                            totalByteOffset += thisSignalByteOffset;
                        }
                    }

                }
            }
        }
    }
    return ret;
}

void *GAM::GetInputSignalsMemory() {
    return inputSignalsMemory;
}

void *GAM::GetOutputSignalsMemory() {
    return outputSignalsMemory;
}

void *GAM::GetInputSignalMemory(const uint32 signalIdx) const {
    void *ret = NULL_PTR(void *);
    if ((signalIdx < numberOfInputSignals) && (inputSignalsMemoryIndexer != NULL_PTR(void**))) {
        /*lint -e{796} The invariant of the class assures that
         *  inputSignalsMemoryIndexer ∈ [0..numberOfInputSignals-1] &&
         *  numberOfInputSignals ∈ [0..configuredDatabase.GetNumberOfChildren()-1u]*/
        ret = inputSignalsMemoryIndexer[signalIdx];
    }
    return ret;
}

void *GAM::GetOutputSignalMemory(const uint32 signalIdx) const {
    void *ret = NULL_PTR(void *);
    if ((signalIdx < numberOfOutputSignals) && (outputSignalsMemoryIndexer != NULL_PTR(void**))) {
        /*lint -e{796} The invariant of the class assures that
         *  outputSignalsMemoryIndexer ∈ [0..numberOfOutputSignals-1] &&
         *  numberOfOutputSignals ∈ [0..configuredDatabase.GetNumberOfChildren()-1u]*/
        ret = outputSignalsMemoryIndexer[signalIdx];
    }
    return ret;
}

bool GAM::SetConfiguredDatabase(StructuredDataI &data) {
    bool ret = data.Copy(configuredDatabase);
    if (ret) {
        ret = configuredDatabase.MoveToRoot();
    }
    if (configuredDatabase.MoveAbsolute("Signals.InputSignals")) {
        //-1 to ignore the ByteSize field
        numberOfInputSignals = (configuredDatabase.GetNumberOfChildren() - 1u);
    }
    if (configuredDatabase.MoveAbsolute("Signals.OutputSignals")) {
        //-1 to ignore the ByteSize field
        numberOfOutputSignals = (configuredDatabase.GetNumberOfChildren() - 1u);
    }

    return ret;
}

/*lint -e{715} The symbol 'context' is not referenced because
 * this is a default implementation, i.e. it is expected to be
 * implemented on derived classes.
 */
bool GAM::SetContext(ConstReference context) {
    return true;
}

uint32 GAM::GetNumberOfInputSignals() const {
    return numberOfInputSignals;
}

uint32 GAM::GetNumberOfOutputSignals() const {
    return numberOfOutputSignals;
}

bool GAM::GetQualifiedName(StreamString &qualifiedName) {
    bool ret = configuredDatabase.MoveToRoot();
    if (ret) {
        ret = configuredDatabase.Read("QualifiedName", qualifiedName);
    }
    return ret;
}

bool GAM::GetSignalName(const SignalDirection direction,
                        const uint32 signalIdx,
                        StreamString &signalName) {
    bool ret = MoveToSignalIndex(direction, signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("QualifiedName", signalName);
    }
    return ret;
}

bool GAM::GetSignalIndex(const SignalDirection direction,
                         uint32 &signalIdx,
                         const char8* const signalName) {
    uint32 numberOfSignals = 0u;
    if (direction == InputSignals) {
        numberOfSignals = GetNumberOfInputSignals();
    }
    else {
        numberOfSignals = GetNumberOfOutputSignals();
    }
    bool ret = true;
    bool found = false;
    uint32 i;
    for (i = 0u; (i < numberOfSignals) && (ret) && (!found); i++) {
        StreamString searchName;
        ret = GetSignalName(direction, i, searchName);
        if (ret) {
            found = (StringHelper::Compare(signalName, searchName.Buffer()) == 0);
            signalIdx = i;
        }
    }
    if (ret) {
        ret = found;
    }
    return ret;
}

bool GAM::GetSignalDataSourceName(const SignalDirection direction,
                                  const uint32 signalIdx,
                                  StreamString &dataSourceName) {

    bool ret = MoveToSignalIndex(direction, signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("DataSource", dataSourceName);
    }
    return ret;
}

TypeDescriptor GAM::GetSignalType(const SignalDirection direction,
                                  const uint32 signalIdx) {
    TypeDescriptor signalTypeDescriptor = InvalidType;
    bool ret = MoveToSignalIndex(direction, signalIdx);
    StreamString signalType;
    if (ret) {
        ret = configuredDatabase.Read("Type", signalType);
    }
    if (ret) {
        signalTypeDescriptor = TypeDescriptor::GetTypeDescriptorFromTypeName(signalType.Buffer());
    }
    return signalTypeDescriptor;
}

bool GAM::GetSignalNumberOfDimensions(const SignalDirection direction,
                                      const uint32 signalIdx,
                                      uint32 &numberOfDimensions) {
    bool ret = MoveToSignalIndex(direction, signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("NumberOfDimensions", numberOfDimensions);
    }
    return ret;
}

bool GAM::GetSignalNumberOfElements(const SignalDirection direction,
                                    const uint32 signalIdx,
                                    uint32 &numberOfElements) {
    bool ret = MoveToSignalIndex(direction, signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("NumberOfElements", numberOfElements);
    }
    return ret;
}

bool GAM::GetSignalDefaultValue(const SignalDirection direction,
                                const uint32 signalIdx,
                                const AnyType &defaultValue) {

    bool ret = MoveToSignalIndex(direction, signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("Default", defaultValue);
    }
    return ret;

}

bool GAM::GetSignalByteSize(const SignalDirection direction,
                            const uint32 signalIdx,
                            uint32 &byteSize) {

    bool ret = MoveToSignalIndex(direction, signalIdx);
    if (ret) {
        if (!configuredDatabase.Read("MemberSize", byteSize)) {
            ret = configuredDatabase.Read("ByteSize", byteSize);
        }
    }
    return ret;
}

bool GAM::GetSignalNumberOfByteOffsets(const SignalDirection direction,
                                       const uint32 signalIdx,
                                       uint32 &numberOfByteOffsets) {
    bool ret = MoveToSignalIndex(direction, signalIdx);
    AnyType byteOffset;
    numberOfByteOffsets = 0u;
    if (ret) {
        byteOffset = configuredDatabase.GetType("ByteOffset");
    }
    if (byteOffset.GetDataPointer() != NULL_PTR(void *)) {
        numberOfByteOffsets = byteOffset.GetNumberOfElements(1u);
    }
    return ret;
}

bool GAM::GetSignalByteOffsetInfo(const SignalDirection direction,
                                  const uint32 signalIdx,
                                  const uint32 byteOffsetIndex,
                                  uint32 &byteOffsetStart,
                                  uint32 &byteOffsetSize) {

    uint32 numberOfByteOffsets = 0u;
    bool ret = GetSignalNumberOfByteOffsets(direction, signalIdx, numberOfByteOffsets);
    if (ret) {
        ret = (numberOfByteOffsets > 0u);
    }
    if (ret) {
        ret = (numberOfByteOffsets > byteOffsetIndex);
    }
    if (ret) {
        ret = MoveToSignalIndex(direction, signalIdx);
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

bool GAM::GetSignalNumberOfRanges(const SignalDirection direction,
                                  const uint32 signalIdx,
                                  uint32 &numberOfRanges) {
    bool ret = MoveToSignalIndex(direction, signalIdx);
    AnyType ranges;
    numberOfRanges = 0u;
    if (ret) {
        ranges = configuredDatabase.GetType("Ranges");
    }
    if (ranges.GetDataPointer() != NULL_PTR(void *)) {
        numberOfRanges = ranges.GetNumberOfElements(1u);
    }
    return ret;
}

bool GAM::GetSignalRangesInfo(const SignalDirection direction,
                              const uint32 signalIdx,
                              const uint32 rangeIndex,
                              uint32 &rangeStart,
                              uint32 &rangeEnd) {
    uint32 numberOfRanges = 0u;
    bool ret = GetSignalNumberOfRanges(direction, signalIdx, numberOfRanges);
    if (ret) {
        ret = (numberOfRanges > 0u);
    }
    if (ret) {
        ret = (numberOfRanges > rangeIndex);
    }
    if (ret) {
        ret = MoveToSignalIndex(direction, signalIdx);
    }

    Matrix<uint32> rangesMat(numberOfRanges, 2u);
    if (ret) {
        ret = configuredDatabase.Read("Ranges", rangesMat);
    }
    if (ret) {
        rangeStart = rangesMat(rangeIndex, 0u);
        rangeEnd = rangesMat(rangeIndex, 1u);
    }

    return ret;
}

bool GAM::GetSignalNumberOfSamples(const SignalDirection direction,
                                   const uint32 signalIdx,
                                   uint32 &numberOfSamples) {
    StreamString dataSourceName;

    bool ret = GetSignalDataSourceName(direction, signalIdx, dataSourceName);

    if (ret) {
        ret = configuredDatabase.MoveToRoot();
    }
    //This information is stored in the Memory node
    const char8 * signalDirection = "Memory.InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "Memory.OutputSignals";
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative(signalDirection);
    }

    uint32 n;
    uint32 numberOfDataSources = configuredDatabase.GetNumberOfChildren();
    bool found = false;
    for (n = 0u; (n < numberOfDataSources) && (!found) && (ret); n++) {
        //Move to the next DataSource
        ret = configuredDatabase.MoveRelative(configuredDatabase.GetChildName(n));
        StreamString thisDataSourceName;
        if (ret) {
            ret = configuredDatabase.Read("DataSource", thisDataSourceName);
        }
        if (ret) {
            found = (thisDataSourceName == dataSourceName);
            if (found) {
                ret = configuredDatabase.MoveRelative("Signals");
                if (ret) {
                    StreamString signalIdxStr;
                    ret = signalIdxStr.Printf("%d", signalIdx);
                    if (ret) {
                        ret = configuredDatabase.MoveRelative(signalIdxStr.Buffer());
                    }
                }
                if (ret) {
                    ret = configuredDatabase.Read("Samples", numberOfSamples);
                }
            }
        }
        if (ret) {
            ret = configuredDatabase.MoveToAncestor(1u);
        }
    }
    return ret;
}

bool GAM::GetSignalFrequency(const SignalDirection direction,
                             const uint32 signalIdx,
                             float32 &frequency) {
    StreamString dataSourceName;

    bool ret = GetSignalDataSourceName(direction, signalIdx, dataSourceName);

    if (ret) {
        ret = configuredDatabase.MoveToRoot();
    }
    //This information is stored in the Memory node
    const char8 * signalDirection = "Memory.InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "Memory.OutputSignals";
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative(signalDirection);
    }

    uint32 n;
    uint32 numberOfDataSources = configuredDatabase.GetNumberOfChildren();
    bool found = false;
    for (n = 0u; (n < numberOfDataSources) && (!found) && (ret); n++) {
        //Move to the next DataSource
        ret = configuredDatabase.MoveRelative(configuredDatabase.GetChildName(n));
        StreamString thisDataSourceName;
        if (ret) {
            ret = configuredDatabase.Read("DataSource", thisDataSourceName);
        }
        if (ret) {
            found = (thisDataSourceName == dataSourceName);
            if (found) {
                ret = configuredDatabase.MoveRelative("Signals");
                if (ret) {
                    StreamString signalIdxStr;
                    ret = signalIdxStr.Printf("%d", signalIdx);
                    if (ret) {
                        ret = configuredDatabase.MoveRelative(signalIdxStr.Buffer());
                    }
                }
                if (ret) {
                    ret = configuredDatabase.Read("Frequency", frequency);
                }
            }
        }
        if (ret) {
            ret = configuredDatabase.MoveToAncestor(1u);
        }
    }
    return ret;
}

bool GAM::MoveToSignalIndex(const SignalDirection direction,
                            const uint32 signalIdx) {
    bool ret = configuredDatabase.MoveToRoot();
    const char8 * signalDirection = "Signals.InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "Signals.OutputSignals";
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative(signalDirection);
    }
    StreamString signalIdxStr;
    if (ret) {
        signalIdxStr = configuredDatabase.GetChildName(signalIdx);
        ret = configuredDatabase.MoveRelative(signalIdxStr.Buffer());
    }
    return ret;
}

bool GAM::AddInputBrokers(ReferenceContainer brokers) {
    bool ret = true;
    uint32 n = 0u;
    uint32 size = brokers.Size();
    while ((n < size) && (ret)) {
        ReferenceT<BrokerI> broker = brokers.Get(n);
        ret = broker.IsValid();
        if (ret) {
            ret = inputBrokers.Insert(broker);
        }
        n++;
    }
    return ret;
}

bool GAM::AddOutputBrokers(ReferenceContainer brokers) {
    bool ret = true;
    uint32 n = 0u;
    uint32 size = brokers.Size();
    while ((n < size) && ret) {
        ReferenceT<BrokerI> broker = brokers.Get(n);
        ret = broker.IsValid();
        if (ret) {
            ret = outputBrokers.Insert(broker);
        }
        n++;
    }
    return ret;
}

bool GAM::GetInputBrokers(ReferenceContainer &brokers) {
    bool ret = true;
    uint32 n = 0u;
    uint32 size = inputBrokers.Size();
    while ((n < size) && ret) {
        ret = brokers.Insert(inputBrokers.Get(n));
        n++;
    }
    return ret;
}

bool GAM::GetOutputBrokers(ReferenceContainer &brokers) {
    bool ret = true;
    uint32 n = 0u;
    uint32 size = outputBrokers.Size();
    while ((n < size) && ret) {
        ret = brokers.Insert(outputBrokers.Get(n));
        n++;
    }
    return ret;
}

}
