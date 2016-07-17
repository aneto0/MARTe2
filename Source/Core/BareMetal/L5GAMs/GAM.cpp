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

#include "GAM.h"
#include "GAMSignalsContainer.h"
#include "DataSourceContainer.h"
#include "AdvancedErrorManagement.h"
#include <BrokerI.h>
#include <DataSourceI.h>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static const uint32 stateNamesGranularity = 4u;
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAM::GAM() :
        ReferenceContainer() {
    numberOfInputBrokers = 0u;
    numberOfOutputBrokers = 0u;
    numberOfInputSignals = 0u;
    numberOfOutputSignals = 0u;
    numberOfInputSignalsMemoryBlocks = 0u;
    numberOfOutputSignalsMemoryBlocks = 0u;
    inputSignalsMemoryBlocks = NULL_PTR(void **);
    outputSignalsMemoryBlocks = NULL_PTR(void **);
    inputSignalsAbsoluteAddress = NULL_PTR(void **);
    outputSignalsAbsoluteAddress = NULL_PTR(void **);
    inputBrokers = NULL_PTR(ReferenceT<BrokerI> *);
    outputBrokers = NULL_PTR(ReferenceT<BrokerI> *);

    heap = GlobalObjectsDatabase::Instance()->GetStandardHeap();
#if 0
    localData = NULL_PTR(StructuredDataI*);
    numberOfSupportedStates = 0u;
    supportedStates = NULL_PTR(StreamString *);
    supportedThreads = NULL_PTR(StreamString *);
    group = NULL_PTR(GAMGroup*);
    application = NULL_PTR(RealTimeApplication *);
    inputReaders = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    outputWriters = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
#endif
}

/*lint -e{1551} no exception should be thrown*/
GAM::~GAM() {
    if (inputSignalsMemoryBlocks != NULL_PTR(void **)) {
        uint32 n;
        for (n = 0u; n < numberOfInputSignalsMemoryBlocks; n++) {
            heap->Free(inputSignalsMemoryBlocks[n]);
        }
        delete[] inputSignalsMemoryBlocks;
    }
    if (outputSignalsMemoryBlocks != NULL_PTR(void **)) {
        uint32 n;
        for (n = 0u; n < numberOfOutputSignalsMemoryBlocks; n++) {
            heap->Free(outputSignalsMemoryBlocks[n]);
        }
        delete[] outputSignalsMemoryBlocks;
    }
    if (inputSignalsAbsoluteAddress != NULL_PTR(void **)) {
        delete[] inputSignalsAbsoluteAddress;
    }
    if (outputSignalsAbsoluteAddress != NULL_PTR(void **)) {
        delete[] outputSignalsAbsoluteAddress;
    }
    if (inputBrokers != NULL_PTR(ReferenceT<BrokerI> *)) {
        delete[] inputBrokers;
    }
    if (outputBrokers != NULL_PTR(ReferenceT<BrokerI> *)) {
        delete[] outputBrokers;
    }

#if 0
    if (supportedStates != NULL) {
        delete[] supportedStates;
    }

    if (supportedThreads != NULL) {
        delete[] supportedThreads;
    }

    application = NULL_PTR(RealTimeApplication *);
    group = NULL_PTR(GAMGroup*);
#endif
}

#include <stdio.h>
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
#if 0
    ret = inputReaders.IsValid();
    if (ret) {
        StreamString irName = GetName();
        irName += "_InputReader";
        inputReaders->SetName(irName.Buffer());
        inputReaders->SetInput(true);
    }

    if (ret) {
        ret = outputWriters.IsValid();
        if (ret) {
            StreamString owName = GetName();
            owName += "_OutputWriter";
            outputWriters->SetName(owName.Buffer());
            outputWriters->SetInput(false);
        }
        else {
            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The GAM %s does not have a valid input BrokerContainer", GetName())
        }
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The GAM %s does not have a valid output BrokerContainer", GetName())
    }

    if (ret) {
        // implemented in the derived classes
        // get the local cdb
        SetUp();
        // merge definitions
        ret = ConfigureFunction();
    }
#endif
    return ret;
}

bool GAM::AddSignals(StructuredDataI &data) {
    return data.Write("Signals", signalsDatabase);
}

bool GAM::Finalise() {
    return true;
}

void * GAM::AllocateInputSignalsMemory(uint32 numberOfBytes) {
    void **tempSignalsMemoryBlocks = new void*[numberOfInputSignalsMemoryBlocks + 1u];
    if (numberOfInputSignalsMemoryBlocks > 0u) {
        uint32 n;
        for (n = 0u; n < numberOfInputSignalsMemoryBlocks; n++) {
            tempSignalsMemoryBlocks[n] = inputSignalsMemoryBlocks[n];
        }
        delete[] inputSignalsMemoryBlocks;
    }

    inputSignalsMemoryBlocks = tempSignalsMemoryBlocks;

    inputSignalsMemoryBlocks[numberOfInputSignalsMemoryBlocks] = heap->Malloc(numberOfBytes);
    numberOfInputSignalsMemoryBlocks++;
    return inputSignalsMemoryBlocks[numberOfInputSignalsMemoryBlocks - 1u];
}

void * GAM::AllocateOutputSignalsMemory(uint32 numberOfBytes) {
    void **tempSignalsMemoryBlocks = new void*[numberOfOutputSignalsMemoryBlocks + 1u];

    if (numberOfOutputSignalsMemoryBlocks > 0u) {
        uint32 n;
        for (n = 0u; n < numberOfOutputSignalsMemoryBlocks; n++) {
            tempSignalsMemoryBlocks[n] = outputSignalsMemoryBlocks[n];
        }
        delete[] outputSignalsMemoryBlocks;
    }

    outputSignalsMemoryBlocks = tempSignalsMemoryBlocks;

    outputSignalsMemoryBlocks[numberOfOutputSignalsMemoryBlocks] = heap->Malloc(numberOfBytes);
    numberOfOutputSignalsMemoryBlocks++;
    return outputSignalsMemoryBlocks[numberOfOutputSignalsMemoryBlocks - 1u];
}

bool GAM::SetConfiguredDatabase(StructuredDataI &data) {
    bool ret = data.Copy(configuredDatabase);
    if (ret) {
        ret = configuredDatabase.MoveToRoot();
    }
    if (configuredDatabase.MoveAbsolute("Signals.InputSignals")) {
        numberOfInputSignals = configuredDatabase.GetNumberOfChildren();
    }
    if (configuredDatabase.MoveAbsolute("Signals.OutputSignals")) {
        numberOfOutputSignals = configuredDatabase.GetNumberOfChildren();
    }
    if (ret) {
        inputSignalsAbsoluteAddress = new void*[numberOfInputSignals];
        outputSignalsAbsoluteAddress = new void*[numberOfOutputSignals];
    }
    uint32 k;

    //Compute the DataSource indexes
    const char8 **inputDataSourceIndexes = NULL_PTR(const char8 **);
    uint32 *inputDataSourceOffsets = NULL_PTR(uint32 *);
    if (numberOfInputSignalsMemoryBlocks > 0u) {
        inputDataSourceOffsets = new uint32[numberOfInputSignalsMemoryBlocks];
        inputDataSourceIndexes = new const char8*[numberOfInputSignalsMemoryBlocks];
        for (k = 0u; k < numberOfInputSignalsMemoryBlocks; k++) {
            inputDataSourceIndexes[k] = NULL_PTR(const char8 *);
            inputDataSourceOffsets[k] = 0u;
        }
    }
    const char8 **outputDataSourceIndexes = NULL_PTR(const char8 **);
    uint32 *outputDataSourceOffsets = NULL_PTR(uint32 *);
    if (numberOfOutputSignalsMemoryBlocks > 0u) {
        outputDataSourceOffsets = new uint32[numberOfOutputSignalsMemoryBlocks];
        outputDataSourceIndexes = new const char8*[numberOfOutputSignalsMemoryBlocks];
        for (k = 0u; k < numberOfOutputSignalsMemoryBlocks; k++) {
            outputDataSourceIndexes[k] = NULL_PTR(const char8 *);
            outputDataSourceOffsets[k] = 0u;
        }
    }

    if (numberOfInputSignals > 0u) {
        ret = (inputDataSourceIndexes != NULL_PTR(const char8 **));
    }

    if (numberOfOutputSignals > 0u) {
        ret = (outputDataSourceIndexes != NULL_PTR(const char8 **));
    }
    uint32 n;

    for (n = 0u; (n < numberOfInputSignals) && (ret); n++) {
        //Look for the DataSource (remember that the signals can be interleaved between different DataSources)
        StreamString dataSourceName;
        ret = GetSignalDataSourceName(InputSignals, n, dataSourceName);
        bool found = false;
        uint32 idx;
        for (k = 0u; (k < numberOfInputSignalsMemoryBlocks) && (ret) && (!found); k++) {
            if (inputDataSourceIndexes[k] == NULL_PTR(const char8 *)) {
                //First instance of this DataSource name
                found = true;
                inputDataSourceIndexes[k] = dataSourceName.Buffer();
            }
            else {
                found = (StringHelper::Compare(inputDataSourceIndexes[k], dataSourceName.Buffer()) == 0);
            }
            idx = k;
        }
        //We know the index of the DataSource in the inputSignalsMemoryBlocks
        ret = found;
        uint32 signalByteSize = 0u;
        if (ret) {
            //Assign the absolute addresses
            char8 *signalAddress = reinterpret_cast<char8 *>(inputSignalsMemoryBlocks[idx]);
            signalAddress += inputDataSourceOffsets[idx];
            inputSignalsAbsoluteAddress[n] = reinterpret_cast<void *>(signalAddress);
            ret = GetSignalByteSize(InputSignals, n, signalByteSize);
        }
        if (ret) {
            inputDataSourceOffsets[idx] += signalByteSize;
        }
    }
    for (n = 0u; (n < numberOfOutputSignals) && (ret); n++) {
        //Look for the DataSource (remember that the signals can be interleaved between different DataSources)
        StreamString dataSourceName;
        ret = GetSignalDataSourceName(OutputSignals, n, dataSourceName);
        bool found = false;
        uint32 idx;
        for (k = 0u; (k < numberOfOutputSignalsMemoryBlocks) && (ret) && (!found); k++) {
            if (outputDataSourceIndexes[k] == NULL_PTR(const char8 *)) {
                //First instance of this DataSource name
                found = true;
                outputDataSourceIndexes[k] = dataSourceName.Buffer();
            }
            else {
                found = (StringHelper::Compare(outputDataSourceIndexes[k], dataSourceName.Buffer()) == 0);
            }
            idx = k;

        }
        //We know the index of the DataSource in the outputSignalsMemoryBlocks
        ret = found;
        uint32 signalByteSize = 0u;
        if (ret) {
            //Assign the absolute addresses
            char8 *signalAddress = reinterpret_cast<char8 *>(outputSignalsMemoryBlocks[idx]);
            signalAddress += outputDataSourceOffsets[idx];
            outputSignalsAbsoluteAddress[n] = reinterpret_cast<void *>(signalAddress);
            ret = GetSignalByteSize(OutputSignals, n, signalByteSize);
        }
        if (ret) {
            outputDataSourceOffsets[idx] += signalByteSize;
        }
    }

    if (inputDataSourceIndexes != NULL_PTR(const char8 **)) {
        delete[] inputDataSourceIndexes;
        delete[] inputDataSourceOffsets;
    }
    if (outputDataSourceIndexes != NULL_PTR(const char8 **)) {
        delete[] outputDataSourceIndexes;
        delete[] outputDataSourceOffsets;
    }
    return ret;
}

void * GAM::GetInputSignalAddress(uint32 signalIdx) {
    return inputSignalsAbsoluteAddress[signalIdx];
}

void * GAM::GetOutputSignalAddress(uint32 signalIdx) {
    return outputSignalsAbsoluteAddress[signalIdx];
}

uint32 GAM::GetNumberOfInputSignals() {
    return numberOfInputSignals;
}

uint32 GAM::GetNumberOfOutputSignals() {
    return numberOfOutputSignals;
}

bool GAM::GetSignalName(SignalDirection direction,
                        uint32 signalIdx,
                        StreamString &signalName) {
    bool ret = MoveToSignalIndex(direction, signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("QualifiedName", signalName);
    }
    return ret;
}

bool GAM::GetSignalIndex(SignalDirection direction,
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
            found = (StringHelper::Compare(signalName, searchName.Buffer()) == 0u);
            signalIdx = i;
        }
    }
    if (ret) {
        ret = found;
    }
    return ret;
}

bool GAM::GetSignalDataSourceName(SignalDirection direction,
                                  uint32 signalIdx,
                                  StreamString &dataSourceName) {

    bool ret = MoveToSignalIndex(direction, signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("DataSource", dataSourceName);
    }
    return ret;
}

TypeDescriptor GAM::GetSignalType(SignalDirection direction,
                                  uint32 signalIdx) {
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

bool GAM::GetSignalNumberOfDimensions(SignalDirection direction,
                                      uint32 signalIdx,
                                      uint32 &numberOfDimensions) {
    bool ret = MoveToSignalIndex(direction, signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("NumberOfDimensions", numberOfDimensions);
    }
    return ret;
}

bool GAM::GetSignalNumberElements(SignalDirection direction,
                                  uint32 signalIdx,
                                  uint32 &numberOfElements) {
    bool ret = MoveToSignalIndex(direction, signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("NumberOfElements", numberOfElements);
    }
    return ret;
}

bool GAM::GetSignalByteSize(SignalDirection direction,
                            uint32 signalIdx,
                            uint32 &byteSize) {

    bool ret = MoveToSignalIndex(direction, signalIdx);
    if (ret) {
        ret = configuredDatabase.Read("ByteSize", byteSize);
    }
    return ret;
}

bool GAM::GetSignalNumberOfByteOffsets(SignalDirection direction,
                                       uint32 signalIdx,
                                       uint32 &numberOfByteOffsets) {
    bool ret = MoveToSignalIndex(direction, signalIdx);
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

bool GAM::GetSignalByteOffsetInfo(SignalDirection direction,
                                  uint32 signalIdx,
                                  uint32 byteOffsetIndex,
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
        byteOffsetStart = byteOffsetMat[byteOffsetIndex][0u];
        byteOffsetSize = byteOffsetMat[byteOffsetIndex][1u];
    }

    return ret;
}

bool GAM::GetSignalNumberOfRanges(SignalDirection direction,
                                  uint32 signalIdx,
                                  uint32 &numberOfRanges) {
    bool ret = MoveToSignalIndex(direction, signalIdx);
    AnyType ranges;
    if (ret) {
        ranges = configuredDatabase.GetType("Ranges");
        ret = (ranges.GetDataPointer() != NULL_PTR(void *));
    }
    numberOfRanges = 0u;
    if (ret) {
        numberOfRanges = ranges.GetNumberOfElements(1u);
    }
    return ret;
}

bool GAM::GetSignalRangesInfo(SignalDirection direction,
                              uint32 signalIdx,
                              uint32 rangeIndex,
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
        rangeStart = rangesMat[rangeIndex][0u];
        rangeEnd = rangesMat[rangeIndex][1u];
    }

    return ret;
}

bool GAM::GetSignalTimeCyclesInfo(SignalDirection direction,
                                  uint32 signalIdx,
                                  uint32 &timeCycles,
                                  uint32 &timeSamples) {

    Vector<uint32> timeCyclesSamplesVec(2u);
    bool ret = MoveToSignalIndex(direction, signalIdx);
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

bool GAM::MoveToSignalIndex(SignalDirection direction,
                            uint32 signalIdx) {
    bool ret = configuredDatabase.MoveToRoot();
    const char8 * signalDirection = "Signals.InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "Signals.OutputSignals";
    }
    if (ret) {
        ret = configuredDatabase.MoveRelative(signalDirection);
    }
    StreamString signalIdxStr;
    signalIdxStr.Printf("%d", signalIdx);
    if (ret) {
        ret = configuredDatabase.MoveRelative(signalIdxStr.Buffer());
    }
    return ret;
}

void GAM::AddInputBroker(ReferenceT<BrokerI> broker) {
    ReferenceT<BrokerI> *temp = new ReferenceT<BrokerI> [numberOfInputBrokers + 1u];

    if (inputBrokers != NULL_PTR(ReferenceT<BrokerI> *)) {
        uint32 n;
        for (n = 0u; n < numberOfInputBrokers; n++) {
            temp[n] = inputBrokers[n];
        }
        delete[] inputBrokers;
    }
    inputBrokers = temp;
    inputBrokers[numberOfInputBrokers] = broker;
    numberOfInputBrokers++;
}

void GAM::AddOutputBroker(ReferenceT<BrokerI> broker) {
    ReferenceT<BrokerI> *temp = new ReferenceT<BrokerI> [numberOfOutputBrokers + 1u];

    if (outputBrokers != NULL_PTR(ReferenceT<BrokerI> *)) {
        uint32 n;
        for (n = 0u; n < numberOfOutputBrokers; n++) {
            temp[n] = outputBrokers[n];
        }
        delete[] outputBrokers;
    }
    outputBrokers = temp;
    outputBrokers[numberOfOutputBrokers] = broker;
    numberOfOutputBrokers++;
}

bool GAM::Read() {
    uint32 b;
    bool ret = true;
    for (b = 0u; (b < numberOfInputBrokers) && (ret); b++) {
        ret = inputBrokers[b]->Read();
    }
    return ret;
}

bool GAM::Write() {
    uint32 b;
    bool ret = true;
    for (b = 0u; (b < numberOfOutputBrokers) && (ret); b++) {
        ret = outputBrokers[b]->Write();
    }
    return ret;
}

#if 0
/*void GAM::SetUp() {
 // initialises the local status
 }*/

/*
 virtual void Execute(){
 // execution routine
 */

bool GAM::ConfigureFunction() {
// use for each of them before RealTimeDataContainer::MergeWithLocal(localData)
// and merge the result with the existing one
    bool ret = true;
    if (localData != NULL) {
        uint32 numberOfElements = Size();
        for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {

            ReferenceT<GAMSignalsContainer> def = Get(i);

// must be all GAMSignalsContainer ??

            if (def.IsValid()) {
                StreamString defName = "+";
                defName += def->GetName();
                if (localData->MoveRelative(defName.Buffer())) {
                    // the partial definitions after this must become complete
                    ret = def->MergeWithLocal(*localData);
                    if (ret) {
                        ret = localData->MoveToAncestor(1u);
                    }
                }
                if (ret) {
                    // check if the definitions are meaningful
                    ret = def->Verify();
                }
            }
        }
    }
    return ret;
}

bool GAM::ConfigureDataSource() {

    bool ret = true;
    if (GetNumberOfSupportedStates() > 0u) {
        ret = (application != NULL);
        if (ret) {
            ReferenceT<DataSourceContainer> dataContainer = application->GetDataSourceContainer();
            ret = dataContainer.IsValid();
            if (ret) {
                ret = dataContainer->AddDataDefinition(ReferenceT<GAM>(this));
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "+Data container not found or invalid in the application");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Application not set");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::Warning, "GAM never called in states or threads");
    }
    return ret;
}

void GAM::SetApplication(RealTimeApplication &rtApp) {
    if (application == NULL) {
        application = &rtApp;
    }
    inputReaders->SetApplication(rtApp);
    outputWriters->SetApplication(rtApp);
}

void GAM::SetGAMGroup(ReferenceT<GAMGroup> gamGroup) {
    if (group == NULL) {
        group = gamGroup.operator->();
    }
}

bool GAM::AddState(const char8 * const stateName,
        const char8 * const threadName) {

    bool ret = true;
    bool found = false;
    for (uint32 i = 0u; (i < numberOfSupportedStates) && (!found); i++) {
        /*lint -e{613} never enter here if supportedStates is NULL (because numberOfSupportedStates == 0) */
        found = (supportedStates[i] == stateName);

        if (found) {
            ret = (supportedThreads[i] == threadName);
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The GAM %s cannot be in both %s and %s threads into the state %s", GetName(),
                        supportedThreads[i].Buffer(), threadName, stateName)
                //TODO same gam in two different threads!
            }
        }

    }
    if (!found) {
        if ((numberOfSupportedStates % stateNamesGranularity) == 0u) {
            uint32 newSize = numberOfSupportedStates + stateNamesGranularity;
            StreamString *tempStates = new StreamString[newSize];
            StreamString *tempThreads = new StreamString[newSize];
            if (supportedStates != NULL) {
                for (uint32 i = 0u; i < numberOfSupportedStates; i++) {
                    tempStates[i] = supportedStates[i];
                    tempThreads[i] = supportedThreads[i];
                }
                delete[] supportedStates;
                delete[] supportedThreads;
            }
            supportedStates = tempStates;
            supportedThreads = tempThreads;

        }
        /*lint -e{613} the memory of supportedStates is already allocated (numberOfSupportedStates == 0) */
        supportedStates[numberOfSupportedStates] = stateName;
        supportedThreads[numberOfSupportedStates] = threadName;
        numberOfSupportedStates++;
    }
    return ret;
}

StreamString * GAM::GetSupportedStates() {
    return supportedStates;
}

StreamString * GAM::GetSupportedThreads() {
    return supportedThreads;
}

uint32 GAM::GetNumberOfSupportedStates() const {
    return numberOfSupportedStates;

}

/*void GAM::SetUp() {
 // initialises the local status
 }*/

/*
 virtual void Execute(){
 // execution routine
 */

bool GAM::ConfigureDataSourceLinks() {
    bool ret = true;
    uint32 numberOfElements = Size();

    for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
        ReferenceT<GAMSignalsContainer> defContainer = Get(i);
        if (defContainer.IsValid()) {
            uint32 numberOfDefs = defContainer->Size();
            for (uint32 j = 0u; (j < numberOfDefs) && (ret); j++) {
                ReferenceT<GAMSignalI> def = defContainer->Get(j);
                ret = def.IsValid();
                if (ret) {

                    if (defContainer->IsInput()) {
                        ret = (inputReaders->AddSignal(def));
                        if (!ret) {
                            // TODO definition not compatible with
                            // its data source !!!

                        }
                    }
                    if (defContainer->IsOutput()) {
                        ret = (outputWriters->AddSignal(def));
                        if (!ret) {
                            // TODO definition not compatible with
                            // its data source !!!

                        }
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "The GAMSignalsContainer must contain GAMSignalI objects");
                }
            }
        }
    }

    if (ret) {
        ret = inputReaders->Finalise();
        if (!ret) {
//TODO Failed readers finalisation
        }
    }
    if (ret) {
        ret = outputWriters->Finalise();
        if (!ret) {
//TODO Failed readers finalisation
        }
    }

    if (ret) {
// not both sync!
        ret = !((inputReaders->IsSync()) && (outputWriters->IsSync()));
    }

    return ret;
}

bool GAM::IsSync() {
    return (inputReaders->IsSync() || outputWriters->IsSync());
}

Reference GAM::GetInputReader() {
    return inputReaders;
}

Reference GAM::GetOutputWriter() {
    return outputWriters;
}

RealTimeApplication *GAM::GetApplication() {
    return application;
}
#endif
}
