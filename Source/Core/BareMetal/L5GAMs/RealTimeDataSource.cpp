/**
 * @file RealTimeDataSource.cpp
 * @brief Source file for class RealTimeDataSource
 * @date 29/02/2016
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
 * the class RealTimeDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

bool RealTimeDataSource::AddDataDefinition(RealTimeDataDefContainer &definitionContainer) {

    bool ret = true;
    uint32 numberOfDefinitions = definitionContainer.Size();
    bool isProducer = definitionContainer.IsOutput();
    bool isConsumer = definitionContainer.IsInput();
    const char8* userName = definitionContainer.GetUserName();
    const char8* threadName = definitionContainer.GetThreadName();
    const char8* stateName = definitionContainer.GetStateName();
    for (uint32 i = 0u; (i < numberOfDefinitions) && (ret); i++) {
        ReferenceT<RealTimeDataDefI> definition = definitionContainer.Get(i);
        ret = definition.IsValid();
        if (ret) {
            ret = AddSingleDataDefinition(definition, userName, threadName, stateName, isProducer, isConsumer);
        }
    }
    return ret;
}

bool RealTimeDataSource::AddSingleDataDefinition(ReferenceT<RealTimeDataDefI> definition,
                                                 const char8 * userName,
                                                 const char8 * threadName,
                                                 const char8 * stateName,
                                                 bool isProducer,
                                                 bool isConsumer) {

    bool ret = false;
// check myself
    const char8 * path = definition->GetPath();
    ReferenceContainer resultSingle;

    if (StringHelper::Compare(path, "") != 0) {
        // the path exists?
        ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE, path);
        Find(resultSingle, filter);

        uint32 numberOfResults = resultSingle.Size();
        // if it exists adds the info to the record
        if (numberOfResults > 0u) {
            ReferenceT<RealTimeDataSourceDef> element = resultSingle.Get(numberOfResults - 1u);
            if (element.IsValid()) {
                if (isConsumer) {
                    element->AddConsumer(userName);
                }
                if (isProducer) {
                    element->Addproducer(userName);
                }
                element->AddState(stateName);
                element->AddThread(threadName);
            }
        }
        // if the definition does not exist creates it
        else {
            ReferenceT<RealTimeDataSourceDef> element(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            if (element.IsValid()) {
                if (isConsumer) {
                    element->AddConsumer(userName);
                }
                if (isProducer) {
                    element->Addproducer(userName);
                }
                element->AddState(stateName);
            }
            ret = Insert(path, element);
        }
    }
    else {
        // the path is empty go inside to the sub members
        uint32 numberOfMembers = definition->Size();
        ret = true;
        for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
            ReferenceT<RealTimeDataDefI> subDefinition = definition->Get(i);
            if (subDefinition.IsValid()) {
                ret = AddSingleDataDefinition(subDefinition, userName, threadName, stateName, isProducer, isConsumer);
            }
        }
    }

    return ret;

}

/*
 bool RealTimeDataSource::AddDefinitionInterface(RealTimeDataDefContainer &dataDefinition,
 uint32 userId,
 bool isConsumer,
 bool isProducer) {

 bool ret = true;
 uint32 numberOfDefinitions = dataDefinition.Size();
 for (uint32 i = 0u; (i < numberOfDefinitions) && (ret); i++) {
 ReferenceT<RealTimeDataDefI> definition = dataDefinition.Get(i);
 AddSingleData(definition, userId, isConsumer, isProducer);
 }
 return true;
 }

 bool RealTimeDataSource::AddSingleData(ReferenceT<RealTimeDataDefI> definition,
 uint32 userId,
 bool isConsumer,
 bool isProducer) {
 bool ret = false;
 if (definition.IsValid()) {
 uint32 size = definition->Size();
 if (size == 0u) {
 if (StringHelper::Compare(definition->GetType(), "") != 0) {
 // allocate memory
 // copy the default value
 // the default value must be parsed by cfg?
 const char8 * typeName = definition->GetType();
 // structured type
 TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(typeName);
 uint32 size = 0u;
 if (typeDes == InvalidType) {
 const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(typeName);
 if (item != NULL) {
 const ClassProperties* properties=item->GetClassProperties();
 if(properties !=NULL) {
 size=properties->GetSize();
 }
 }
 }
 else {
 size=(typeDes.numberOfBits)/8u;
 }

 if (size != 0u) {
 void *ptr = memory.Add(size);
 RealTimeDataSourceRecord record(ptr, definition->GetPath(), userId, isConsumer, isProducer);
 if (!records.Add(record)) {
 //TODO
 }
 }
 }
 else {
 //TODO What is this?
 }
 }
 else {
 ret = true;
 for (uint32 j = 0u; (j < size) && (ret); j++) {
 ReferenceT<RealTimeDataDefI> definitionChild = definition->Get(j);
 if (definitionChild.IsValid()) {
 // go recursively
 ret = AddSingleData(definitionChild, userId, isConsumer, isProducer);
 }
 }
 }
 }
 return ret;
 }
 */
}

