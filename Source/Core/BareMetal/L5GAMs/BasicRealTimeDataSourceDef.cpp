/**
 * @file BasicRealTimeDataSourceDef.cpp
 * @brief Source file for class BasicRealTimeDataSourceDef
 * @date 21/mar/2016
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
 * the class BasicRealTimeDataSourceDef (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicRealTimeDataSourceDef.h"
#include "ReferenceT.h"
#include "RealTimeDataSourceDefRecord.h"
#include "StandardParser.h"
#include "ConfigurationDatabase.h"
#include "AdvancedErrorManagement.h"
#include "GAMI.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicRealTimeDataSourceDef::BasicRealTimeDataSourceDef() :
        ReferenceContainer() {

    bufferPtrOffset[0] = 0u;
    bufferPtrOffset[1] = 0u;
    memory = NULL_PTR(MemoryArea *);
    usedBuffer[0] = NULL_PTR(void *);
    usedBuffer[1] = NULL_PTR(void *);
    spinLockSem = NULL_PTR(FastPollingEventSem *);
    numberOfDimensions = 0u;
    for (uint32 k = 0u; k < 3u; k++) {
        numberOfElements[k] = 1u;
    }

}

bool BasicRealTimeDataSourceDef::AddConsumer(const char8 * const stateIn,
                                             ReferenceT<GAMI> gam) {
    uint32 index;
    bool found = false;
    bool ret = gam.IsValid();

    ReferenceT<RealTimeDataSourceDefRecord> record;
    uint32 numberOfStates = Size();
    for (index = 0u; (index < numberOfStates) && (!found); index++) {
        StreamString stateName = stateIn;
        record = Get(index);
        if (record.IsValid()) {
            if (stateName == record->GetName()) {
                found = true;
            }
        }
    }
    if (found) {
        ret = record->AddConsumer(gam);
    }
    else {
        record = ReferenceT<RealTimeDataSourceDefRecord>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (record.IsValid()) {
            record->SetName(stateIn);
            ret = record->AddConsumer(gam);
            if (ret) {
                ret = Insert(record);
            }
        }
    }
    return ret;
}

bool BasicRealTimeDataSourceDef::AddProducer(const char8 * const stateIn,
                                             ReferenceT<GAMI> gamIn) {

    uint32 index;
    bool found = false;

    ReferenceT<GAMI> gam = gamIn;
    bool ret = gam.IsValid();

    ReferenceT<RealTimeDataSourceDefRecord> record;
    uint32 numberOfStates = Size();
    for (index = 0u; (index < numberOfStates) && (!found); index++) {
        StreamString stateName = stateIn;
        record = Get(index);
        if (record.IsValid()) {
            if (stateName == record->GetName()) {
                found = true;
            }
        }
    }
    if (found) {
        ret = record->AddProducer(gam);
    }
    else {
        record = ReferenceT<RealTimeDataSourceDefRecord>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (record.IsValid()) {
            record->SetName(stateIn);
            ret = record->AddProducer(gam);
            if (ret) {
                ret = Insert(record);
            }
        }
    }
    return ret;

}

void BasicRealTimeDataSourceDef::SetDefaultValue(const char8 * const defaultIn) {

    if (defaultIn != NULL) {
        if (defaultValue != "") {
            if (defaultValue != defaultIn) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "A different default value equal to \"%s\" was set. It will be refreshed by the new one", defaultValue.Buffer())
            }
        }
        defaultValue = defaultIn;
    }
}

const char8 *BasicRealTimeDataSourceDef::GetDefaultValue() {
    return defaultValue.Buffer();
}

uint32 BasicRealTimeDataSourceDef::GetNumberOfConsumers(const char8 * const stateIn) {
    uint32 ret = 0u;
    uint32 numberOfRecords = Size();
    ReferenceT<RealTimeDataSourceDefRecord> record;
    bool found = false;
    for (uint32 i = 0u; (i < numberOfRecords) && (!found); i++) {
        record = Get(i);
        if (record.IsValid()) {
            if (StringHelper::Compare(record->GetName(), stateIn) == 0) {
                found = true;
            }
        }
    }

    if (found) {
        ret = record->GetNumberOfConsumers();
    }
    return ret;

}

uint32 BasicRealTimeDataSourceDef::GetNumberOfProducers(const char8 * const stateIn) {
    uint32 ret = 0u;
    uint32 numberOfRecords = Size();
    ReferenceT<RealTimeDataSourceDefRecord> record;
    bool found = false;
    for (uint32 i = 0u; (i < numberOfRecords) && (!found); i++) {
        record = Get(i);
        if (record.IsValid()) {
            if (StringHelper::Compare(record->GetName(), stateIn) == 0) {
                found = true;
            }
        }
    }
    if (found) {
        ret = record->GetNumberOfProducers();
    }
    return ret;
}

bool BasicRealTimeDataSourceDef::Verify() {

    bool ret = true;
    uint32 numberOfStates = Size();
    for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
        ReferenceT<RealTimeDataSourceDefRecord> record = Get(i);
        if (record.IsValid()) {
            // no more than one producer for each state
            if (record->GetNumberOfProducers() > 1u) {
                ret = false;
            }
            if (record->GetNumberOfConsumers() == 0u) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Variable %s not consumed", GetName())
            }
        }
    }
    return ret;
}

bool BasicRealTimeDataSourceDef::SetType(const char8 * const typeName) {
    bool ret = true;
    if (type != typeName) {
        if (type != "") {
            ret = false;
            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Type already set to %s", type.Buffer())
        }
        else {
            type = typeName;
        }
    }
    return ret;
}

const char8 *BasicRealTimeDataSourceDef::GetType() {
    return type.Buffer();
}

void **BasicRealTimeDataSourceDef::GetDataSourcePointer(uint8 bufferIndex) {
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

bool BasicRealTimeDataSourceDef::PrepareNextState(const RealTimeStateInfo &status) {

    bool ret = (memory != NULL);

    if (ret) {
        // by default use the same buffer in the next state
        uint8 nextBuffer = (status.activeBuffer + 1u) % 2u;
        /*lint -e{613} NULL pointer checking done before entering here */
        usedBuffer[nextBuffer] = memory->GetPointer(bufferPtrOffset[status.activeBuffer]);

        // search the current state
        uint32 numberOfStates = Size();
        bool found = false;
        ReferenceT<RealTimeDataSourceDefRecord> record;
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
                            // if defaultValue is not set, remain with the same buffer of the previous state
                            if (defaultValue != "") {
                                typeDes = TypeDescriptor(false, properties->GetUniqueIdentifier());
                                /*lint -e{613} NULL pointer checking done before entering here */
                                at = AnyType(typeDes, 0u, memory->GetPointer(bufferPtrOffset[nextBuffer]));
                                ConfigurationDatabase cdb;
                                ret=defaultValue.Seek(0ull);
                                if(ret) {
                                    StandardParser parser(defaultValue, cdb);
                                    ret = parser.Parse();
                                    if(ret) {
                                        ret=cdb.Write("Class", type.Buffer());
                                    }
                                    if (ret) {
                                        ret = TypeConvert(at, cdb);
                                    }

                                    if (ret) {
                                        //set the next used buffer
                                        /*lint -e{613} NULL pointer checking done before entering here */
                                        usedBuffer[nextBuffer] = memory->GetPointer(bufferPtrOffset[nextBuffer]);
                                    }
                                }
                            }
                        }
                    }

                }
                else {
                    // if defaultValue is not set, remain with the same buffer of the previous state
                    if (defaultValue != "") {
                        /*lint -e{613} NULL pointer checking done before entering here */
                        at = AnyType(typeDes, 0u, memory->GetPointer(bufferPtrOffset[nextBuffer]));
                        if (numberOfDimensions > 0u) {
                            // consider the multi-dimensional
                            // set the output
                            at.SetNumberOfDimensions(numberOfDimensions);
                            for (uint32 k = 0u; k < 3u; k++) {
                                at.SetNumberOfElements(k, numberOfElements[k]);
                            }
                            // set the input
                            ConfigurationDatabase cdb;
                            // create a stream with "node = { element1, element2, ...}
                            StreamString fakeNodeConfig = "node = ";
                            fakeNodeConfig += defaultValue;
                            ret=fakeNodeConfig.Seek(0ull);
                            if(ret) {
                                // parse it
                                StandardParser parser(fakeNodeConfig, cdb);
                                ret = parser.Parse();
                                // get the input
                                AnyType multiDim = cdb.GetType("node");

                                if (ret) {
                                    ret = TypeConvert(at, multiDim);
                                }
                            }
                        }
                        else {
                            ret = TypeConvert(at, defaultValue);
                        }
                        if (ret) {
                            //set the next used buffer
                            /*lint -e{613} NULL pointer checking done before entering here */
                            usedBuffer[nextBuffer] = memory->GetPointer(bufferPtrOffset[nextBuffer]);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

bool BasicRealTimeDataSourceDef::Allocate(MemoryArea &dsMemory) {

    bool ret = true;
    memory = &dsMemory;
    TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(type.Buffer());
    uint32 varSize = 0u;
    // structured type
    if (typeDes == InvalidType) {
        ret = (numberOfDimensions == 0u);
        if (ret) {
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
        else {
            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Unsupported multi-dimensional structured types for the type %s", type.Buffer())
        }
    }
    // basic type
    else {
        varSize = (static_cast<uint32>(typeDes.numberOfBits) + 7u) / 8u;
        // consider the multi - dimensional
        for (uint32 k = 0u; k < numberOfDimensions; k++) {
            varSize *= numberOfElements[k];
        }

    }
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

void BasicRealTimeDataSourceDef::SetNumberOfElements(uint32 dimension,
                                                     const uint32 nElements) {
    if (dimension > 2u) {
        REPORT_ERROR(ErrorManagement::Warning, "The dimension id in input is too high (max dimension id = 2). The 2nd dimension id will be considered");
        dimension = 2u;
    }
    numberOfElements[dimension] = nElements;

}

void BasicRealTimeDataSourceDef::SetNumberOfDimensions(uint8 nDimensions) {
    if (nDimensions > 2u) {
        REPORT_ERROR(ErrorManagement::Warning,
                     "The number of dimensions in input is too high (max number of dimensions = 2). The 2nd dimension will be considered");
        nDimensions = 2u;
    }
    numberOfDimensions = nDimensions;
}

uint8 BasicRealTimeDataSourceDef::GetNumberOfDimensions() const {
    return numberOfDimensions;
}

uint32 BasicRealTimeDataSourceDef::GetNumberOfElements(uint32 dimension) const {
    if (dimension > 2u) {
        REPORT_ERROR(ErrorManagement::Warning, "The dimension id in input is too high (max dimension id = 2). The 2nd dimension id will be considered");
        dimension = 2u;
    }
    return numberOfElements[dimension];
}

bool BasicRealTimeDataSourceDef::ToStructuredData(StructuredDataI& data) {

    const char8 * objName = GetName();
    bool ret = data.CreateRelative(objName);
    if (ret) {
        ret = data.Write("Class", "BasicRealTimeDataSourceDef");
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
            if (numberOfDimensions > 0u) {
                ret = data.Write("NumberOfDimensions", numberOfDimensions);
                if (ret) {
                    ret = data.Write("NumberOfElements", numberOfElements);
                }
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

void BasicRealTimeDataSourceDef::WriteStart() {

}

void BasicRealTimeDataSourceDef::ReadStart() {

}

void BasicRealTimeDataSourceDef::WriteEnd() {

}

void BasicRealTimeDataSourceDef::ReadEnd() {

}

FastPollingEventSem * BasicRealTimeDataSourceDef::GetSpinLockSemaphore() const {
    return spinLockSem;
}

CLASS_REGISTER(BasicRealTimeDataSourceDef, "1.0")

}
