/**
 * @file RealTimeDataSourceDef.cpp
 * @brief Source file for class RealTimeDataSourceDef
 * @date 29/feb/2016
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
 * the class RealTimeDataSourceDef (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceDef.h"
#include "ReferenceT.h"
#include "RealTimeDataSourceDefRecord.h"
#include "StandardParser.h"
#include "ConfigurationDatabase.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeDataSourceDef::RealTimeDataSourceDef() {

    bufferPtrOffset[0] = 0u;
    bufferPtrOffset[1] = 0u;
    memory = NULL_PTR(MemoryArea *);
    usedBuffer[0] = NULL;
    usedBuffer[1] = NULL;

}

bool RealTimeDataSourceDef::AddConsumer(const char8 *stateIn,
                                        ReferenceT<GAM> gam) {
    uint32 index;
    bool found = false;
    bool ret = false;
    printf("\nadding consumer %s\n", stateIn);

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

bool RealTimeDataSourceDef::AddProducer(const char8 *stateIn,
                                        ReferenceT<GAM> gam) {

    uint32 index;
    bool found = false;
    bool ret = false;
    printf("\nadding producer %s\n", stateIn);

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

bool RealTimeDataSourceDef::SetDefaultValue(const char8 *stateIn,
                                            const char8* defaultIn) {
    uint32 index;
    bool found = false;
    bool ret = false;

    printf("\nset default value %s\n", defaultIn);
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
        record->SetDefaultValue(defaultIn);
        ret = true;
    }
    else {
        record = ReferenceT<RealTimeDataSourceDefRecord>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (record.IsValid()) {
            record->SetName(stateIn);
            record->SetDefaultValue(defaultIn);
            ret = Insert(record);
        }
    }
    return ret;
}

uint32 RealTimeDataSourceDef::GetNumberOfConsumers(const char8 * stateIn) {
    uint32 ret = 0u;
    uint32 numberOfRecords = Size();
    ReferenceT<RealTimeDataSourceDefRecord> record;
    for (uint32 i = 0u; i < numberOfRecords; i++) {
        record = Get(i);
        if (record.IsValid()) {
            if (StringHelper::Compare(record->GetName(), stateIn) == 0) {
                break;
            }
        }
    }

    if (record.IsValid()) {
        ret = record->GetNumberOfConsumers();
    }
    return ret;

}

uint32 RealTimeDataSourceDef::GetNumberOfProducers(const char8 * stateIn) {
    uint32 ret = 0u;
    uint32 numberOfRecords = Size();
    ReferenceT<RealTimeDataSourceDefRecord> record;
    for (uint32 i = 0u; i < numberOfRecords; i++) {
        record = Get(i);
        if (record.IsValid()) {
            if (StringHelper::Compare(record->GetName(), stateIn) == 0) {
                break;
            }
        }
    }
    if (record.IsValid()) {
        ret = record->GetNumberOfProducers();
    }
    return ret;
}

bool RealTimeDataSourceDef::Verify() {

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
                //TODO Warning variable not consumed
            }
        }
    }
    return ret;
}

bool RealTimeDataSourceDef::SetType(const char8 *typeName) {
    bool ret = true;
    if (type != typeName) {
        if (type != "") {
            ret = false;
            //TODO type mismatch
        }
        else {
            type = typeName;
        }
    }

    return ret;
}

const char8 *RealTimeDataSourceDef::GetType() {
    return type.Buffer();
}

void **RealTimeDataSourceDef::GetDataSourcePointer(uint8 bufferIndex) {
    if (bufferIndex > 1u) {
        //TODO Warning
        bufferIndex = bufferIndex % 2u;
    }

    void ** ret = NULL;
    if (memory != NULL) {
        printf("\nreturn a pointer to the buffer %d\n", bufferIndex);
        usedBuffer[bufferIndex] = memory->GetPointer(bufferPtrOffset[bufferIndex]);
        ret = &usedBuffer[bufferIndex];
    }
    else {
        //TODO
    }

    return ret;
}

bool RealTimeDataSourceDef::PrepareNextState(const RealTimeStateInfo &status) {

    bool ret = (memory != NULL);

    if (ret) {
        // by default use the same buffer in the next state
        uint8 nextBuffer = (status.activeBuffer + 1u) % 2u;
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
                StreamString defaultValue = record->GetDefaultValue();
                TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(type.Buffer());
                if (typeDes == InvalidType) {
                    const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(type.Buffer());
                    ret = (item != NULL);
                    if (ret) {
                        const ClassProperties *properties = item->GetClassProperties();
                        ret = (properties != NULL);
                        if (ret) {
                            // if defaultValue is not set, remain with the same buffer of the previous state
                            if (defaultValue != "") {
                                typeDes = TypeDescriptor(false, properties->GetUniqueIdentifier());
                                at = AnyType(typeDes, 0u, memory->GetPointer(bufferPtrOffset[nextBuffer]));
                                ConfigurationDatabase cdb;
                                StandardParser parser(defaultValue, cdb);
                                ret = parser.Parse();
                                if (ret) {
                                    ret = TypeConvert(at, cdb);
                                }

                                if (ret) {
                                    //set the next used buffer
                                    usedBuffer[nextBuffer] = memory->GetPointer(bufferPtrOffset[nextBuffer]);
                                }
                            }
                        }
                    }

                }
                else {
                    // if defaultValue is not set, remain with the same buffer of the previous state
                    if (defaultValue != "") {
                        printf("\nSet default value:: %s found: defaultValue= %s nextBuff=%d\n", GetName(), defaultValue.Buffer(), nextBuffer);
                        at = AnyType(typeDes, 0u, memory->GetPointer(bufferPtrOffset[nextBuffer]));
                        ret = TypeConvert(at, defaultValue);
                        if (ret) {
                            printf("\ndata is %d\n", *(uint32*) memory->GetPointer(bufferPtrOffset[nextBuffer]));
                            //set the next used buffer
                            usedBuffer[nextBuffer] = memory->GetPointer(bufferPtrOffset[nextBuffer]);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

bool RealTimeDataSourceDef::Allocate(MemoryArea &dsMemory) {

    bool ret = true;
    memory = &dsMemory;
    TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(type.Buffer());
    uint32 varSize = 0u;
    // structured type
    if (typeDes == InvalidType) {
        const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(type.Buffer());
        ret = (item != NULL);
        if (ret) {
            const ClassProperties *properties = item->GetClassProperties();
            ret = (properties != NULL);
            if (ret) {
                varSize = properties->GetSize();
            }
            else {
                //TODO ??
            }
        }
        else {
            //TODO type not registered
        }
    }
    // basic type
    else {
        varSize = (typeDes.numberOfBits + 7u) / 8u;
    }
    // allocate the memory
    if (varSize != 0u) {
        ret = memory->Add(varSize, bufferPtrOffset[0]);
        if (ret) {
            ret = memory->Add(varSize, bufferPtrOffset[1]);
        }
    }
    return ret;
}

CLASS_REGISTER(RealTimeDataSourceDef, "1.0")

}

