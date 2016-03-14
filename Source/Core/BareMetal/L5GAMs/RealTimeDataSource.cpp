/**
 * @file RealTimeDataSource.cpp
 * @brief Source file for class RealTimeDataSource
 * @date 09/mar/2016
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
#include "ReferenceContainerFilterObjectName.h"
#include "RealTimeDataDefContainer.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static bool VerifyPrivate(ReferenceT<ReferenceContainer> ref) {
    bool ret = ref.IsValid();
    if (ret) {
        uint32 size = ref->Size();

        for (uint32 i = 0u; (i < size) && (ret); i++) {
            Reference generic = ref->Get(i);
            ret = ref.IsValid();
            if (ret) {
                // case leaf
                ReferenceT<RealTimeDataSourceDef> rtLeaf = generic;
                if (rtLeaf.IsValid()) {
                    // verify the leaf
                    ret = rtLeaf->Verify();
                }
                else {
                    ReferenceT<ReferenceContainer> rtNode = generic;
                    if (rtNode.IsValid()) {
                        // go recursively
                        ret = VerifyPrivate(rtNode);
                    }
                }
            }
        }
    }
    return ret;
}

static bool PrepareNextStatePrivate(const RealTimeStateInfo &status,
                                    ReferenceT<ReferenceContainer> ref) {

// go inside definitions to prepare the next state
    uint32 numberOfContainers = ref->Size();
    bool ret = true;
    for (uint32 i = 0u; (i < numberOfContainers) && (ret); i++) {
        ReferenceT<ReferenceContainer> container = ref->Get(i);
        if (container.IsValid()) {
            ReferenceT<RealTimeDataSourceDef> def = container;
            if (def.IsValid()) {
                ret = def->PrepareNextState(status);
            }
            else {
                ret = PrepareNextStatePrivate(status, container);
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
        if (subContainer.IsValid()) {
            ReferenceT<RealTimeDataSourceDef> def = subContainer;
            if (def.IsValid()) {
                printf("\nallocation of %s %s\n", def->GetName(), def->GetType());
                ret = def->Allocate(memory);
            }
            else {
                ret = AllocatePrivate(subContainer, memory);
            }
        }
        else {
            ret = false;
            //TODO ??
        }

    }
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeDataSource::RealTimeDataSource() {
    numberOfInitialDDBs = 0u;

    final = false;
}

bool RealTimeDataSource::Allocate() {
    return AllocatePrivate(ReferenceT<ReferenceContainer>(this), memory);
}

bool RealTimeDataSource::AddDataDefinition(ReferenceT<GAM> gam) {

    bool ret = (gam.IsValid());
    if (ret) {

        // the number of definition containers (input, output, ecc)
        uint32 numberOfElements = gam->Size();
        for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
            ReferenceT<RealTimeDataDefContainer> defContainer = gam->Get(i);
            ret = defContainer.IsValid();
            if (ret) {
                bool isProducer = defContainer->IsOutput();
                bool isConsumer = defContainer->IsInput();

                uint32 numberOfDefinitions = defContainer->Size();
                for (uint32 j = 0u; (j < numberOfDefinitions) && (ret); j++) {
                    ReferenceT<RealTimeDataDefI> definition = defContainer->Get(j);
                    ret = definition.IsValid();
                    if (ret) {
                        ret = AddSingleDataDefinition(definition, gam, isProducer, isConsumer);
                        printf("\nAdded %s\n", definition->GetName());
                    }
                    else {
                        //TODO Does not contain a definition
                    }
                }
            }
            else {
                //TODO Does not contain a definition container
            }
        }
    }
    else {
        //TODO invalid
        printf("\nError, data container not found\n");
    }
    return ret;
}

bool RealTimeDataSource::AddSingleDataDefinition(ReferenceT<RealTimeDataDefI> definition,
                                                 ReferenceT<GAM> gam,
                                                 bool isProducer,
                                                 bool isConsumer,
                                                 StreamString defaultPath) {

    bool ret = definition.IsValid();
    if (ret) {
        const char8 * userName = gam->GetName();
        StreamString *supportedStates = gam->GetSupportedStates();
        uint32 numberOfStates = gam->GetNumberOfSupportedStates();
        StreamString defaultValue = definition->GetDefaultValue();

        StreamString path = definition->GetPath();
        bool isLeaf = (definition->Size() == 0u);

        // the path exists?
        // default refresh
        StreamString newDefaultPath = path;
        if (path == "") {
            // set the path
            newDefaultPath = defaultPath;
            newDefaultPath += definition->GetName();
            path = newDefaultPath;
        }
        newDefaultPath += ".";

        if (isLeaf) {
            printf("\nfound a leaf, the path is %s the user is %s type is %s\n", path.Buffer(), userName, definition->GetType());

            ReferenceT<RealTimeDataSourceDef> element = Find(path.Buffer());
            if (element.IsValid()) {

                printf("\nleaf already inside\n");

                // if the path exists adds only the infos
                if (isConsumer) {
                    for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
                        element->SetDefaultValue(defaultValue.Buffer());
                        if (ret) {
                            ret = element->AddConsumer(supportedStates[i].Buffer(), gam);
                        }
                    }
                }
                if (isProducer) {
                    for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
                        element->SetDefaultValue(defaultValue.Buffer());
                        if (ret) {
                            ret = element->AddProducer(supportedStates[i].Buffer(), gam);
                        }
                    }
                }
                if (ret) {
                    ret = element->SetType(definition->GetType());
                }

                if (ret) {
                    definition->SetPath(path.Buffer());
                }

            }
            // if the definition does not exist creates it
            else {

                ReferenceT<RealTimeDataSourceDef> element(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                if (element.IsValid()) {
                    printf("\nleaf added\n");

                    if (isConsumer) {
                        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
                            element->SetDefaultValue(defaultValue.Buffer());
                            if (ret) {
                                ret = element->AddConsumer(supportedStates[i].Buffer(), gam);
                            }
                        }
                    }
                    if (isProducer) {
                        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
                            element->SetDefaultValue(defaultValue.Buffer());
                            if (ret) {
                                ret = element->AddProducer(supportedStates[i].Buffer(), gam);
                            }
                        }
                    }
                    if (ret) {
                        ret = Insert(path.Buffer(), element);
                    }
                    if (ret) {
                        ret = element->SetType(definition->GetType());
                    }
                    if (ret) {
                        definition->SetPath(path.Buffer());
                    }
                }
            }
        }
        else {
            printf("\nfound a structure, the path is %s the user is %s\n", path.Buffer(), userName);

            // is structured, go inside to the sub members
            uint32 numberOfMembers = definition->Size();
            for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                ReferenceT<RealTimeDataDefI> subDefinition = definition->Get(i);
                if (subDefinition.IsValid()) {
                    ret = AddSingleDataDefinition(subDefinition, gam, isProducer, isConsumer, newDefaultPath);
                    printf("\nAdded %s %d\n", subDefinition->GetName(), numberOfMembers);
                }
                else {
                    printf("\n???\n");
                }
            }
        }

        if (ret) {
            // if final can not add another definition
            if (final) {
                ret = (Size() == numberOfInitialDDBs);
            }
        }
    }
    else {
        //TODO Invalid
    }
    return ret;

}

bool RealTimeDataSource::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        StreamString isFinal;
        if (data.Read("IsFinal", isFinal)) {
            if (isFinal == "true") {
                final = true;
            }
            else
                final = false;
        }
    }
    numberOfInitialDDBs = Size();
    return ret;
}

bool RealTimeDataSource::Verify() {

    return VerifyPrivate(ReferenceT<ReferenceContainer>(this));
}

bool RealTimeDataSource::PrepareNextState(const RealTimeStateInfo &status) {

    return PrepareNextStatePrivate(status, ReferenceT<ReferenceContainer>(this));
}

CLASS_REGISTER(RealTimeDataSource, "1.0")

}
