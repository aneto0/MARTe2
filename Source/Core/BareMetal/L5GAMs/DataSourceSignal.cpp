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

#include "DataSourceSignal.h"
#include "ReferenceT.h"
#include "DataSourceSignalRecord.h"
#include "StandardParser.h"
#include "ConfigurationDatabase.h"
#include "AdvancedErrorManagement.h"
#include "MemoryMapDataSourceBroker.h"
#include "MemoryMapInputReader.h"
#include "MemoryMapOutputWriter.h"
#include "GAM.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

DataSourceSignal::DataSourceSignal() :
        DataSourceSignalI() {

    bufferPtrOffset[0] = 0u;
    bufferPtrOffset[1] = 0u;
    memory = NULL_PTR(MemoryArea *);
    usedBuffer[0] = NULL_PTR(void *);
    usedBuffer[1] = NULL_PTR(void *);
}

DataSourceSignal::~DataSourceSignal() {

}

void **DataSourceSignal::GetDataSourcePointer(uint8 bufferIndex) {
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

bool DataSourceSignal::PrepareNextState(const RealTimeStateInfo &status) {

    bool ret = (memory != NULL);

    if (ret) {
        // by default use the same buffer in the next state
        uint8 nextBuffer = (status.activeBuffer + 1u) % 2u;
        /*lint -e{613} NULL pointer checking done before entering here */
        usedBuffer[nextBuffer] = memory->GetPointer(bufferPtrOffset[status.activeBuffer]);

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
        // this variable was dead
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
                    char8* memPtr = reinterpret_cast<char8 *>(memory->GetPointer(bufferPtrOffset[nextBuffer]));
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
                        usedBuffer[nextBuffer] = memory->GetPointer(bufferPtrOffset[nextBuffer]);
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

bool DataSourceSignal::Allocate(MemoryArea &dsMemory) {

    bool ret = true;
    memory = &dsMemory;
    TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(type.Buffer());
    uint32 varSize = 0u;
    // structured type
    if (typeDes == InvalidType) {
        const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(type.Buffer());
        ret = (item != NULL);
        if (ret) {
            /*lint -e{613} NULL pointer checking done before entering here */
            const ClassProperties *properties = item->GetClassProperties();
            ret = (properties != NULL);
            if (ret) {
                varSize = properties->GetSize();
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s does not provide ClassProperties", type.Buffer())
            }
        }
        else {
            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s is not registered", type.Buffer())
        }
    }
    // basic type
    else {
        varSize = (static_cast<uint32>(typeDes.numberOfBits) + 7u) / 8u;
        // consider the multi - dimensional
        varSize *= numberOfElements;
    }

    // consider the number of samples per cycle
    varSize *= numberOfSamples;

    // allocate the memory
    if (ret) {
        /*lint -e{613} NULL pointer checking done before entering here */
        ret = memory->Add(varSize, bufferPtrOffset[0]);
        if (ret) {
            /*lint -e{613} NULL pointer checking done before entering here */
            ret = memory->Add(varSize, bufferPtrOffset[1]);
        }
    }
    return ret;
}

bool DataSourceSignal::Initialise(StructuredDataI & data) {

    bool ret = ReferenceContainer::Initialise(data);

    if (ret) {

        if (!data.Read("Type", type)) {
        }

        if (!data.Read("Default", defaultValue)) {
        }

        if (data.Read("Dimensions", dimensions)) {
            // use introspection entry to parse the modifiers
            IntrospectionEntry entry("", "", dimensions.Buffer(), "", 0u, 0u);
            for (uint32 i = 0u; i < 3u; i++) {
                numberOfElements *= entry.GetNumberOfElements(i);
            }
            if (entry.GetMemberPointerLevel() > 0u) {
                REPORT_ERROR(ErrorManagement::Warning, "Pointers not supported. The statement will be ignored");
            }
        }

        if (!data.Read("Samples", numberOfSamples)) {

        }

    }
    return ret;
}

bool DataSourceSignal::ToStructuredData(StructuredDataI& data) {

    const char8 * objName = GetName();
    StreamString objNameToPrint = (IsDomain()) ? ("$") : ("+");
    objNameToPrint += objName;
    bool ret = data.CreateRelative(objNameToPrint.Buffer());
    if (ret) {
        ret = data.Write("Class", "DataSourceSignal");
        if (ret) {
            if (type != "") {
                ret = data.Write("Type", type);
            }
        }

        if (ret) {
            if (defaultValue != "") {
                ret = data.Write("Default", defaultValue);
            }
        }

        if (ret) {
            if (numberOfSamples != 1) {
                ret = data.Write("Samples", numberOfSamples);
            }
        }

        if (ret) {
            if (dimensions != "") {
                ret = data.Write("Dimensions", dimensions);
            }
        }

        if (ret) {
            uint32 numberOfChildren = Size();
            for (uint32 i = 0u; i < numberOfChildren; i++) {
                Reference child = Get(i);
                ret = child.IsValid();
                if (ret) {
                    if (ret) {
                        ret = child->ToStructuredData(data);
                    }
                }
            }
        }
        if (!data.MoveToAncestor(1u)) {
            ret = false;
        }
    }

    return ret;
}

void DataSourceSignal::WriteStart() {

}

void DataSourceSignal::ReadStart() {

}

void DataSourceSignal::WriteEnd() {

}

void DataSourceSignal::ReadEnd() {

}

bool DataSourceSignal::WaitOnEvent(const TimeoutType &timeout) {
    return true;
}

bool DataSourceSignal::Configure(Reference gamSignalIn) {

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
        MemoryMapDataSourceBroker broker;
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

Reference DataSourceSignal::GetInputReader(Reference defIn,
                                           void * varPtr) {
    ReferenceT<MemoryMapInputReader> ret;
    ReferenceT<GAMSignalI> def = defIn;

    if (def.IsValid()) {
        // try the default reader
        ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (reader.IsValid()) {
            //  sets the same name of the data source
            reader->SetName(GetName());
            if (application != NULL) {
                reader->SetApplication(*application);
                if (reader.IsValid()) {
                    // can link data source to internal static variables
                    if (reader->AddSignal(def, varPtr)) {
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

Reference DataSourceSignal::GetOutputWriter(Reference defIn,
                                            void * varPtr) {
    ReferenceT<MemoryMapOutputWriter> ret;
    ReferenceT<GAMSignalI> def = defIn;

    if (def.IsValid()) {
        // try the default reader
        ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (writer.IsValid()) {
            //  sets the same name of the data source
            writer->SetName(GetName());
            if (application != NULL) {
                writer->SetApplication(*application);
                if (writer.IsValid()) {
                    // can link data source to internal static variables
                    if (writer->AddSignal(def, varPtr)) {
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

bool DataSourceSignal::IsSupportedBroker(DataSourceBrokerI &testBroker) {

    // only memory map broker descendants supported
    MemoryMapDataSourceBroker *test = dynamic_cast<MemoryMapDataSourceBroker *>(&testBroker);
    return (test != NULL);

}



CLASS_REGISTER(DataSourceSignal, "1.0")

}
