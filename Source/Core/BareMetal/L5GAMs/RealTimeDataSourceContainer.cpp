/**
 * @file RealTimeDataSourceContainer.cpp
 * @brief Source file for class RealTimeDataSourceContainer
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
 * the class RealTimeDataSourceContainer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceContainer.h"
#include "RealTimeDataDefContainer.h"
#include "AdvancedErrorManagement.h"
#include "RealTimeDataSource.h"
#include "BasicRealTimeDataSourceDef.h"
#include "ConfigurationDatabase.h"
#include "RealTimeGenericDataDef.h"
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
                ReferenceT<BasicRealTimeDataSourceDef> rtLeaf = generic;
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
            ReferenceT<BasicRealTimeDataSourceDef> def = container;
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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeDataSourceContainer::RealTimeDataSourceContainer() {
}

bool RealTimeDataSourceContainer::AddDataDefinition(ReferenceT<BasicGAM> gam) {

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
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The Reference in the RealTimeDataDefContainer %s is not a RealTimeDataDefI",
                                                defContainer->GetName())
                    }

                }
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The Reference in the GAM %s is not a RealTimeDataDefContainer", gam->GetName())
            }
        }

        // Creates the definitions of the time stamps
        if (ret) {
            // Insert the GAM Times ddb
            ReferenceT<RealTimeGenericDataDef> absTimeDef = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

            StreamString str = gam->GetName();
            str += "AbsoluteUsecTime";

            absTimeDef->SetName(str.Buffer());
            ConfigurationDatabase absDefCDB;

            absDefCDB.Write("Class", "RealTimeGenericDataDef");
            absDefCDB.Write("Type", "uint64");
            absDefCDB.Write("IsFinal", "true");
            absDefCDB.Write("Default", "0");
            str = "GAM_Times.";
            str += gam->GetName();
            str += ".AbsoluteUsecTime";
            absDefCDB.Write("Path", str.Buffer());
            absDefCDB.MoveToRoot();

            ret = absTimeDef->Initialise(absDefCDB);

            if (ret) {
                ret = AddSingleDataDefinition(absTimeDef, gam, false, false);
            }

            if (ret) {
                ReferenceT<RealTimeGenericDataDef> relTimeDef = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

                str = gam->GetName();
                str += "RelativeUsecTime";

                relTimeDef->SetName(str.Buffer());
                ConfigurationDatabase relDefCDB;

                relDefCDB.Write("Class", "RealTimeGenericDataDef");
                relDefCDB.Write("Type", "uint64");
                relDefCDB.Write("IsFinal", "true");
                relDefCDB.Write("Default", "0");
                str = "GAM_Times.";
                str += gam->GetName();
                str += ".RelativeUsecTime";
                relDefCDB.Write("Path", str.Buffer());
                relDefCDB.MoveToRoot();

                ret = relTimeDef->Initialise(relDefCDB);
                if (ret) {
                    ret = AddSingleDataDefinition(relTimeDef, gam, false, false);
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Invalid Reference to GAM in input");
    }
    return ret;
}

bool RealTimeDataSourceContainer::AddSingleDataDefinition(ReferenceT<RealTimeDataDefI> definition,
                                                          ReferenceT<BasicGAM> gam,
                                                          const bool isProducer,
                                                          const bool isConsumer,
                                                          StreamString defaultPath) {

    bool ret = true;
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

        ReferenceT<BasicRealTimeDataSourceDef> element = Find(path.Buffer());
        if (element.IsValid()) {

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
                element->SetNumberOfDimensions(definition->GetNumberOfDimensions());
                for (uint32 k = 0u; k < 3u; k++) {
                    element->SetNumberOfElements(k, definition->GetNumberOfElements(k));
                }
            }

            if (ret) {
                definition->SetPath(path.Buffer());
            }

        }
        // if the definition does not exist creates it
        else {

            ReferenceT<BasicRealTimeDataSourceDef> newElement(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            if (newElement.IsValid()) {

                if (isConsumer) {
                    for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
                        newElement->SetDefaultValue(defaultValue.Buffer());
                        if (ret) {
                            ret = newElement->AddConsumer(supportedStates[i].Buffer(), gam);
                        }
                    }
                }
                if (isProducer) {
                    for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
                        newElement->SetDefaultValue(defaultValue.Buffer());
                        if (ret) {
                            ret = newElement->AddProducer(supportedStates[i].Buffer(), gam);
                        }
                    }
                }
                if (ret) {
                    StreamString dsName;
                    char8 terminator;
                    path.Seek(0u);
                    ret = path.GetToken(dsName, ".", terminator);
                    if (ret) {
                        uint32 numberOfDS = Size();

                        bool found = false;
                        for (uint32 i = 0u; (i < numberOfDS) && (!found) && (ret); i++) {
                            ReferenceT<RealTimeDataSource> ref = Get(i);
                            ret = ref.IsValid();
                            if (ret) {
                                found = (dsName == ref->GetName());
                            }
                            else {
                                REPORT_ERROR(ErrorManagement::FatalError, "RealTimeDataSourceContainer must contain only RealTimeDataSource elements");
                            }
                        }
                        if (!found) {
                            // for GAMTimes create it
                            if (dsName == "GAM_Times") {
                                ReferenceT<RealTimeDataSource> gamTimesDS = ReferenceT<RealTimeDataSource>(
                                        GlobalObjectsDatabase::Instance()->GetStandardHeap());
                                gamTimesDS->SetName("GAM_Times");
                                ret = Insert(gamTimesDS);
                                // to insert the rest
                                found = true;
                            }
                        }

                        if (found) {
                            ret = Insert(path.Buffer(), newElement);
                        }
                        else {
                            ret = false;
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Data Source %s not found", dsName.Buffer())
                        }
                    }
                }

                if (ret) {
                    ret = newElement->SetType(definition->GetType());
                }
                if (ret) {
                    newElement->SetNumberOfDimensions(definition->GetNumberOfDimensions());
                    for (uint32 k = 0u; k < 3u; k++) {
                        newElement->SetNumberOfElements(k, definition->GetNumberOfElements(k));
                    }
                }

                if (ret) {
                    definition->SetPath(path.Buffer());
                }
            }
        }
    }
    else {

        // is structured, go inside to the sub members
        uint32 numberOfMembers = definition->Size();
        for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
            ReferenceT<RealTimeDataDefI> subDefinition = definition->Get(i);
            if (subDefinition.IsValid()) {
                ret = AddSingleDataDefinition(subDefinition, gam, isProducer, isConsumer, newDefaultPath);
            }
            else {
            }
        }
    }

    return ret;

}

bool RealTimeDataSourceContainer::Verify() {

    return VerifyPrivate(ReferenceT<ReferenceContainer>(this));
}

bool RealTimeDataSourceContainer::PrepareNextState(const RealTimeStateInfo &status) {

    return PrepareNextStatePrivate(status, ReferenceT<ReferenceContainer>(this));
}

bool RealTimeDataSourceContainer::Allocate() {
    bool ret = true;
    uint32 numberOfDS = Size();
    for (uint32 i = 0u; (i < numberOfDS) && (ret); i++) {
        ReferenceT<RealTimeDataSource> dataSource = Get(i);
        ret = dataSource.IsValid();
        if (ret) {
            ret = dataSource->Allocate();
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "RealTimeDataSourceContainer must contain only RealTimeDataSource references");
        }
    }
    return ret;
}

CLASS_REGISTER(RealTimeDataSourceContainer, "1.0")

}
