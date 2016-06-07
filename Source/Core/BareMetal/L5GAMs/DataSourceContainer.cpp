/**
 * @file DataSourceContainer.cpp
 * @brief Source file for class DataSourceContainer
 * @date 11/04/2016
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
 * the class DataSourceContainer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DataSourceContainer.h"
#include "GAMSignalsContainer.h"
#include "AdvancedErrorManagement.h"
#include <DataSourceI.h>
#include <GAMDataSource.h>
#include "ConfigurationDatabase.h"
#include "GAMGenericSignal.h"
#include "GAMSampledSignal.h"
#include <DataSourceI.h>
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Recursively calls the Verify method on all the DataSourceSignal.
 * @param[in] ref ReferenceContainer to be recursively queried for DataSourceSignal elements.
 * @return true if all the DataSourceSignal::Verify return true.
 */
static bool VerifyPrivate(ReferenceT<ReferenceContainer> ref) {
    bool ret = ref.IsValid();
    if (ret) {
        uint32 size = ref->Size();

        for (uint32 i = 0u; (i < size) && (ret); i++) {
            Reference generic = ref->Get(i);
            ret = ref.IsValid();
            if (ret) {
                // case leaf
                ReferenceT<DataSourceSignal> rtLeaf = generic;
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

/**
 * @brief Recursively calls the PrepareNextState method on all the DataSourceSignal elements.
 * @param[in] ref ReferenceContainer to be recursively queried for DataSourceSignal elements.
 * @return true if all the DataSourceSignal::PrepareNextState return true.
 */
static bool PrepareNextStatePrivate(const RealTimeStateInfo &status,
                                    ReferenceT<ReferenceContainer> ref) {

// go inside definitions to prepare the next state
    uint32 numberOfContainers = ref->Size();
    bool ret = true;
    for (uint32 i = 0u; (i < numberOfContainers) && (ret); i++) {
        ReferenceT<ReferenceContainer> container = ref->Get(i);
        if (container.IsValid()) {
            ReferenceT<DataSourceI> def = container;
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

DataSourceContainer::DataSourceContainer() {
}

bool DataSourceContainer::AddDataDefinition(ReferenceT<GAM> gam) {

    bool ret = (gam.IsValid());
    if (ret) {

        // the number of definition containers (input, output, ...)
        uint32 numberOfElements = gam->Size();
        for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
            ReferenceT<GAMSignalsContainer> defContainer = gam->Get(i);
            if (defContainer.IsValid()) {
                bool isProducer = defContainer->IsOutput();
                bool isConsumer = defContainer->IsInput();

                uint32 numberOfDefinitions = defContainer->Size();
                for (uint32 j = 0u; (j < numberOfDefinitions) && (ret); j++) {
                    ReferenceT<GAMSignalI> definition = defContainer->Get(j);
                    if (definition.IsValid()) {
                        ret = AddSingleDataDefinition(definition, gam, isProducer, isConsumer);
                    }
                }
            }
        }

        // Creates the definitions of the time stamps
        if (ret) {
            // Insert the GAM Times ddb
            ReferenceT<GAMGenericSignal> absTimeDef = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

            StreamString str = gam->GetName();
            str += "AbsoluteUsecTime";

            absTimeDef->SetName(str.Buffer());
            ConfigurationDatabase absDefCDB;

            absDefCDB.Write("Class", "GAMGenericSignal");
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
                ReferenceT<GAMGenericSignal> relTimeDef = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

                str = gam->GetName();
                str += "RelativeUsecTime";

                relTimeDef->SetName(str.Buffer());
                ConfigurationDatabase relDefCDB;

                relDefCDB.Write("Class", "GAMGenericSignal");
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

bool DataSourceContainer::AddSingleDataDefinition(ReferenceT<GAMSignalI> definition,
                                                  ReferenceT<GAM> gam,
                                                  const bool isProducer,
                                                  const bool isConsumer,
                                                  StreamString defaultPath) {

    bool ret = true;
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

        if (ret) {
            ReferenceT<DataSourceSignal> element = Find(path.Buffer());
            if (element.IsValid()) {

                // if the path exists adds only the infos
                if (isConsumer) {
                    ret = element->AddConsumer(gam);
                }
                if (isProducer) {
                    ret = element->AddProducer(gam);
                }

                /*if (ret) {
                    ret = element->Configure(definition);
                }*/

                if (ret) {
                    RealTimeApplication *app = gam->GetApplication();
                    ret = (app != NULL);
                    if (ret) {
                        element->SetApplication(*app);
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Application not set in GAM %s", gam->GetName())
                    }
                }
                if (ret) {
                    definition->SetPath(path.Buffer());
                }

            }
            // if the definition does not exist, create it
            else {
                ReferenceT<GAMSampledSignal> sampDef = definition;
                ret = (!sampDef.IsValid());
                if (ret) {
                    ReferenceT<DataSourceSignal> newElement(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                    if (newElement.IsValid()) {

                        if (isConsumer) {
                            ret = newElement->AddConsumer(gam);
                        }
                        if (isProducer) {
                            ret = newElement->AddProducer(gam);
                        }
                        if (ret) {
                            StreamString dsName;
                            char8 terminator;
                            path.Seek(0u);
                            ret = path.GetToken(dsName, ".", terminator);
                            bool found = false;
                            uint32 numberOfDS = Size();
                            ReferenceT<DataSourceI> dataSource;
                            for (uint32 i = 0u; (i < numberOfDS) && (!found) && (ret); i++) {
                                dataSource = Get(i);
                                if (dataSource.IsValid()) {
                                    found = (dsName == dataSource->GetName());
                                }
                            }
                            if (!found) {
                                // for GAMTimes create it
                                if (dsName == "GAM_Times") {
                                    ReferenceT<GAMDataSource> gamTimesDS = ReferenceT<GAMDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                                    gamTimesDS->SetName("GAM_Times");
                                    ret = Insert(gamTimesDS);
                                    // to insert the rest
                                    found = true;
                                }
                            }

                            if (found) {
                                ret = Insert(path.Buffer(), newElement);
                                if (ret) {
                                    //ret = newElement->Configure(definition);
                                }
                                if (ret) {
                                    RealTimeApplication *app = gam->GetApplication();
                                    ret = (app != NULL);
                                    if (ret) {
                                        newElement->SetApplication(*app);
                                    }
                                    else {
                                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Application not set in GAM %s", gam->GetName())
                                    }
                                }
                            }
                            else {
                                ret = false;
                                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Data Source %s not found", dsName.Buffer())
                            }
                        }

                        if (ret) {
                            definition->SetPath(path.Buffer());
                        }
                    }
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The GAMSampledSignal %s must be linked to an existing DataSourceSignal",
                                            sampDef->GetName())
                }

            }
        }
    }
    else {

        // is structured, go inside to the sub members
        uint32 numberOfMembers = definition->Size();
        for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
            // should be a generic one!
            ReferenceT<GAMGenericSignal> subDefinition = definition->Get(i);
            if (subDefinition.IsValid()) {
                ret = AddSingleDataDefinition(subDefinition, gam, isProducer, isConsumer, newDefaultPath);
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The GAMGenericSignal subDefinition at index %d is not valid", i);
                ret = false;
            }
        }
    }

    return ret;

}

bool DataSourceContainer::Verify() {

    return VerifyPrivate(ReferenceT<ReferenceContainer>(this));
}

bool DataSourceContainer::PrepareNextState(const RealTimeStateInfo &status) {

    return PrepareNextStatePrivate(status, ReferenceT<ReferenceContainer>(this));
}

bool DataSourceContainer::Allocate() {
    bool ret = true;
    uint32 numberOfDS = Size();
    for (uint32 i = 0u; (i < numberOfDS) && (ret); i++) {
        ReferenceT<DataSourceI> dataSource = Get(i);
        ret = dataSource.IsValid();
        if (ret) {
            ret = dataSource->Allocate();
        }
        else {
            ReferenceT<ReferenceContainer> container = Get(i);
            ret = container.IsValid();
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "DataSourceContainer must contain only DataSource or ReferenceContainer references");
            }
        }
    }
    return ret;
}

CLASS_REGISTER(DataSourceContainer, "1.0")

}
