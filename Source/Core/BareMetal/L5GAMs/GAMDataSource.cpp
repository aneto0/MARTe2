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

#if 0
/**
 * @brief Computes the dimension of the signal.
 * @param[in] gamSignalIn the signal to be computed.
 * @return the dimension of the signal (i.e. GetNumberOfDimensions(i) * GetNumberOfElements(j)).
 */
static uint32 GetDataSourceDimension(Reference gamSignalIn) {
    uint32 requiredDimension = 0u;
    ReferenceT<GAMSignalI> gamSignal = gamSignalIn;

    if (gamSignal.IsValid()) {
        bool ret = true;
        uint32 numberOfGAMElements = 1u;
        for (uint32 k = 0u; k < gamSignal->GetNumberOfDimensions(); k++) {
            numberOfGAMElements *= gamSignal->GetNumberOfElements(k);
        }

        StreamString confString = "SignalBlocks=";
        confString += gamSignal->GetOperation();

        bool init = (confString != "SignalBlocks=");
        if (init) {
            // a set of blocks to be read
            confString.Seek(0ULL);
            ConfigurationDatabase cdb;

            StandardParser parser(confString, cdb);
            init = parser.Parse();
            // the field exists
            if (init) {

                AnyType at = cdb.GetType("SignalBlocks");
                ret = (at.GetDataPointer() != NULL);
                if (ret) {
                    uint32 nBlockCols = at.GetNumberOfElements(0u);
                    ret = (nBlockCols == 2);
                    if (ret) {
                        uint32 nBlockRows = at.GetNumberOfElements(1u);
                        Matrix<uint32> paramListMatrix = Matrix<uint32>(nBlockRows, nBlockCols);

                        ret = (cdb.Read("SignalBlocks", paramListMatrix));
                        if (ret) {
                            // checks if the size in the GAM variable matches with the
                            // sum of the block sizes
                            uint32 maxEnd = 0u;
                            for (uint32 i = 0u; (i < nBlockRows); i++) {
                                uint32 end = paramListMatrix[i][1];
                                // begin and end must be into the DS memory
                                if (end > maxEnd) {
                                    maxEnd = end;
                                }
                            }
                            requiredDimension = maxEnd;
                        }
                    }
                    else {
                        //TODO the samples blocks matrix must have 2 columns
                    }
                }
            }
        }
        // no blocks field
        if (!init) {
            requiredDimension = numberOfGAMElements;
        }
        // something wrong
        if (!ret) {
            requiredDimension = 0u;
        }
    }

    return requiredDimension;
}
#endif
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMDataSource::GAMDataSource() :
        DataSourceI() {
    currentBufferIndex = 0u;
    signalMemory[0] = NULL_PTR(void **);
    signalMemory[1] = NULL_PTR(void **);
    heap = GlobalObjectsDatabase::Instance()->GetStandardHeap();
#if 0
    bufferPtrOffset[0] = 0u;
    bufferPtrOffset[1] = 0u;
    memory = NULL_PTR(MemoryArea *);
    usedBuffer[0] = NULL_PTR(void *);
    usedBuffer[1] = NULL_PTR(void **);
#endif
}

GAMDataSource::~GAMDataSource() {
    if (signalMemory[0] != NULL_PTR(void **)) {
        delete[] signalMemory[0];
    }
    if (signalMemory[1] != NULL_PTR(void **)) {
        delete[] signalMemory[1];
    }
}

uint32 GAMDataSource::GetCurrentBufferIndex() {
    return currentBufferIndex;
}

uint32 GAMDataSource::GetNumberOfMemoryBuffers() {
    return 2u;
}

bool GAMDataSource::GetSignalMemoryBuffer(uint32 signalIdx,
                                          uint32 bufferIdx,
                                          void *&signalAddress) {
    StreamString signalName;
    bool ret = (bufferIdx < 2u);
    if (ret) {
        signalAddress = signalMemory[bufferIdx][signalIdx];
    }

    return ret;
}

bool GAMDataSource::AllocateMemory() {
    uint32 numberOfSignals = GetNumberOfSignals();
    uint32 s;
    bool ret = true;
    signalMemory[0] = new void*[numberOfSignals];
    signalMemory[1] = new void*[numberOfSignals];
    for (s = 0u; (s < numberOfSignals) && (ret); s++) {
        uint32 memorySize;
        ret = GetSignalByteSize(s, memorySize);
        if (ret) {
            ret = (memorySize > 0u);
        }
        if (ret) {
            signalMemory[0][s] = heap->Malloc(memorySize);
            signalMemory[1][s] = heap->Malloc(memorySize);
        }
    }
    return ret;
}

const char8 *GAMDataSource::Negotiate(StructuredDataI &data,
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

bool GAMDataSource::GetInputReaders(const char8 * const functionName,
                                    ReferenceContainer &output) {
    ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
    bool ret = broker->Init(InputSignals, this, functionName);
    if (ret) {
        ret = output.Insert(broker);
    }
    return ret;
}

bool GAMDataSource::GetOutputWriters(const char8 * const functionName,
                                     ReferenceContainer &output) {
    ReferenceT<MemoryMapOutputBroker> broker("MemoryMapOutputBroker");
    bool ret = broker->Init(OutputSignals, this, functionName);
    if (ret) {
        ret = output.Insert(broker);
    }
    return ret;
}

bool GAMDataSource::PrepareNextState(const RealTimeStateInfo &status) {
    uint32 nextBufferIndex = ((currentBufferIndex + 1u) % 2u);
    //Set the default value for all the input signals
    uint32 numberOfFunctions = GetNumberOfFunctions();
    uint32 n;
    bool ret = true;

    for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
        uint32 numberOfFunctionInputSignals;
        ret = GetFunctionNumberOfSignals(InputSignals, n, numberOfFunctionInputSignals);
        uint32 i;
        for (i = 0u; (i < numberOfFunctionInputSignals) && (ret); i++) {
            StreamString functionSignalName;
            if (ret) {
                ret = GetFunctionSignalName(InputSignals, n, i, functionSignalName);
            }
            uint32 signalIdx;
            if (ret) {
                ret = GetSignalIndex(signalIdx, functionSignalName.Buffer());
            }
            StreamString defaultValueStr = "";
            if (ret) {
                ret = GetSignalDefaultValue(i, defaultValueStr);
            }
            bool update = (defaultValueStr.Size() > 0u);
            //Variable used in the current state?
            if (update && ret) {
                uint32 numberOfProducersCurrentState;
                if (!GetSignalNumberOfProducers(i, status.currentState, numberOfProducersCurrentState)) {
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
                AnyType defaultValue(typeDesc, 0u, signalMemory[nextBufferIndex][signalIdx]);
                ret = TypeConvert(defaultValue, defaultValueStr);
            }
        }

    }
    return ret;
}

bool GAMDataSource::ChangeState() {
    uint32 nextBufferIndex = ((currentBufferIndex + 1u) % 2u);
    currentBufferIndex = nextBufferIndex;
    return true;
}

#if 0
void **GAMDataSource::GetDataSourcePointer(uint8 bufferIndex) {
    if (bufferIndex > 1u) {
        bufferIndex = bufferIndex % 2u;
    }

    void ** ret = NULL_PTR(void **);
    if (memory != NULL) {
        /*lint -e{613} NULL pointer checking done before entering here */
        usedBuffer[bufferIndex] = memory->GetPointer(bufferPtrOffset[bufferIndex]);
        ret = &usedBuffer[bufferIndex];
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "The RealTimeDataSource memory has to be allocated before calling this function");
    }

    return ret;
}

bool GAMDataSource::PrepareNextState(const RealTimeStateInfo &status) {

    bool ret = (memory != NULL);

    if (ret) {
        // by default use the same buffer in the next state
        uint8 nextBufferIdx = (status.activeBuffer + 1u) % 2u;
        /*lint -e{613} NULL pointer checking done before entering here */
        usedBuffer[nextBufferIdx] = memory->GetPointer(bufferPtrOffset[status.activeBuffer]);

        // search the current state
        uint32 numberOfStates = Size();
        bool found = false;
        ReferenceT<DataSourceSignalRecord> record;
        for (uint32 i = 0u; (i < numberOfStates) && (!found); i++) {
            record = Get(i);
            if (record.IsValid()) {
                // match
                if (StringHelper::Compare(record->GetName(), status.currentState) == 0) {
                    found = true;
                }
            }
        }
        // this variable was not being used in this state
        if (!found) {
            found = false;
            for (uint32 i = 0u; (i < numberOfStates) && (!found); i++) {
                record = Get(i);
                if (record.IsValid()) {
                    // match
                    if (StringHelper::Compare(record->GetName(), status.nextState) == 0) {
                        found = true;
                    }
                }
            }

            // this variable will be used in the next
            if (found) {
                // if defaultValue is not set, remain with the same buffer of the previous state
                if (defaultValue != "") {
                    char8* memPtr = reinterpret_cast<char8 *>(memory->GetPointer(bufferPtrOffset[nextBufferIdx]));
                    AnyType at;
                    TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(type.Buffer());
                    if (typeDes == InvalidType) {
                        const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(type.Buffer());
                        ret = (item != NULL);
                        if (ret) {
                            /*lint -e{613} NULL pointer checking done before entering here */
                            const ClassProperties *properties = item->GetClassProperties();
                            ret = (properties != NULL);
                            if (ret) {
                                uint32 typeSize = properties->GetSize();
                                typeDes = TypeDescriptor(false, properties->GetUniqueIdentifier());
                                /*lint -e{613} NULL pointer checking done before entering here */
                                at = AnyType(typeDes, 0u, memPtr);
                                ConfigurationDatabase cdb;
                                ret = defaultValue.Seek(0ull);
                                if (ret) {
                                    StandardParser parser(defaultValue, cdb);
                                    ret = parser.Parse();
                                    if (ret) {
                                        ret = cdb.Write("Class", type.Buffer());
                                    }
                                    if (ret) {
                                        for (uint32 n = 0u; (n < numberOfSamples) && (ret); n++) {
                                            at.SetDataPointer(memPtr);
                                            ret = TypeConvert(at, cdb);
                                            // shift to the next sample
                                            memPtr = &memPtr[typeSize];
                                        }
                                    }
                                }

                            }
                        }
                    }
                    else {
                        uint32 typeSize = (typeDes.numberOfBits + 7u) / 8u;
                        at = AnyType(typeDes, 0u, memPtr);
                        /*lint -e{613} NULL pointer checking done before entering here */
                        if (numberOfElements > 1u) {
                            // set the input
                            ConfigurationDatabase cdb;
                            // create a stream with "node = { element1, element2, ...}
                            StreamString fakeNodeConfig = "node = ";
                            fakeNodeConfig += defaultValue;
                            ret = fakeNodeConfig.Seek(0ull);
                            if (ret) {
                                // parse it
                                StandardParser parser(fakeNodeConfig, cdb);
                                ret = parser.Parse();
                                if (ret) {
                                    // get the input
                                    AnyType multiDim = cdb.GetType("node");
                                    ret = (multiDim.GetDataPointer() != NULL);
                                    if (ret) {
                                        uint32 defaultNumberOfElements = 1u;
                                        uint32 defaultNumberOfDimensions = multiDim.GetNumberOfDimensions();
                                        for (uint32 k = 0u; k < defaultNumberOfDimensions; k++) {
                                            defaultNumberOfElements *= multiDim.GetNumberOfElements(k);
                                        }
                                        ret = (defaultNumberOfElements == numberOfElements);
                                        if (ret) {
                                            // consider the multi-dimensional
                                            // set the output
                                            at.SetNumberOfDimensions(defaultNumberOfDimensions);
                                            for (uint32 k = 0u; k < defaultNumberOfDimensions; k++) {
                                                at.SetNumberOfElements(k, multiDim.GetNumberOfElements(k));
                                                typeSize *= multiDim.GetNumberOfElements(k);
                                            }
                                            for (uint32 n = 0u; (n < numberOfSamples) && (ret); n++) {
                                                at.SetDataPointer(memPtr);
                                                ret = TypeConvert(at, multiDim);
                                                // shift to the next sample
                                                memPtr = &memPtr[typeSize];
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            for (uint32 n = 0u; (n < numberOfSamples) && (ret); n++) {
                                at.SetDataPointer(memPtr);
                                ret = TypeConvert(at, defaultValue);
                                // shift to the next sample
                                memPtr = &memPtr[typeSize];
                            }
                        }
                    }
                    if (ret) {
                        //set the next used buffer
                        /*lint -e{613} NULL pointer checking done before entering here */
                        usedBuffer[nextBufferIdx] = memory->GetPointer(bufferPtrOffset[nextBufferIdx]);
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failed reset of the signal %s", GetName())
                    }
                }
                REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Default Value not set for the signal %s, the value will not be reset", GetName())
            }
        }
    }
    return ret;
}

bool GAMDataSource::WaitOnEvent(const TimeoutType &timeout) {
    return true;
}

bool GAMDataSource::Configure(ReferenceT<GAMSignalI> gamSignalIn) {

    ReferenceT<GAMSignalI> gamSignal = gamSignalIn;

    bool ret = gamSignal.IsValid();
    if (ret) {
        const char8 * typeName = gamSignal->GetType();
        if (StringHelper::Length(typeName) != 0) {
            if (type != typeName) {
                if (type != "") {
                    ret = false;
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Type already set to %s", type.Buffer())
                }
                else {
                    type = typeName;
                }
            }
        }
    }
    if (ret) {
        const char8 *defaultIn = gamSignal->GetDefaultValue();
        if (StringHelper::Length(defaultIn) != 0) {
            if (defaultValue != defaultIn) {
                if (defaultValue != "") {
                    ret = false;
                    REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "A different default value equal to \"%s\" was set. It will be refreshed by the new one",
                            defaultValue.Buffer())
                }
                else {
                    defaultValue = defaultIn;
                }
            }
        }
    }

    if (ret) {
        uint32 requiredDimension = GetDataSourceDimension(gamSignal);
        ret = (requiredDimension > 0u);
        if (ret) {
            // refresh only when the input is greater!
            if (numberOfElements < requiredDimension) {
                numberOfElements = requiredDimension;
            }
        }
    }
    return ret;

}

ReferenceT<BrokerI> GAMDataSource::GetInputReader(ReferenceT<GAMSignalI> signalIn,
        void * varPtr) {
    ReferenceT<MemoryMapBroker> ret;

    if (signalIn.IsValid()) {
        // try the default reader
        ReferenceT<MemoryMapBroker> reader = ReferenceT<MemoryMapBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (reader.IsValid()) {
            //  sets the same name of the data source
            reader->SetName(GetName());
            if (application != NULL) {
                reader->SetApplication(*application);
                if (reader.IsValid()) {
                    // can link data source to internal static variables
                    if (reader->AddSignal(signalIn, varPtr)) {
                        ret = reader;
                    }
                }
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Application not set in %s", GetName())
            }
        }
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Invalid GAM signal in input in %s", GetName())
    }

    return ret;
}

ReferenceT<BrokerI> GAMDataSource::GetOutputWriter(ReferenceT<GAMSignalI> signalOut,
        void * varPtr) {
    ReferenceT<MemoryMapBroker> ret;

    if (signalOut.IsValid()) {
        // try the default reader
        ReferenceT<MemoryMapBroker> writer = ReferenceT<MemoryMapBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (writer.IsValid()) {
            //  sets the same name of the data source
            writer->SetName(GetName());
            if (application != NULL) {
                writer->SetApplication(*application);
                if (writer.IsValid()) {
                    // can link data source to internal static variables
                    if (writer->AddSignal(signalOut, varPtr)) {
                        ret = writer;
                    }
                }
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Application not set in %s", GetName())
            }
        }
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Invalid GAM signal in input in %s", GetName())
    }
    return ret;
}

bool GAMDataSource::IsSupportedBroker(BrokerI &testBroker) {

// only memory map broker descendants supported
    ReferenceT<MemoryMapBroker> test = testBroker;
    return test.IsValid();

}
#endif

CLASS_REGISTER(GAMDataSource, "1.0")

}
