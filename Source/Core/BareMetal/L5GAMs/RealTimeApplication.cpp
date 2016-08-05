/**
 * @file RealTimeApplication.cpp
 * @brief Source file for class RealTimeApplication
 * @date 19/02/2016
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
 * the class RealTimeApplication (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeApplication.h"
#include "RealTimeState.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferencesTemplate.h"
#include "GAM.h"
#include "GAMSchedulerI.h"
#include "AdvancedErrorManagement.h"
#include "Matrix.h"
#include "RealTimeApplicationConfigurationBuilder.h"
#include "RealTimeThread.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

uint32 RealTimeApplication::index = 1u;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplication::RealTimeApplication() :
        ReferenceContainer() {
}

RealTimeApplication::~RealTimeApplication() {

}
bool RealTimeApplication::Initialise(StructuredDataI & data) {
    index = 1u;

    bool ret = ReferenceContainer::Initialise(data);
    if (data.MoveRelative("+Data")) {
        if (!data.Read("DefaultDataSource", defaultDataSourceName)) {
            defaultDataSourceName = "";
        }
        ret = data.MoveToAncestor(1u);
    }
    if (ret) {
        ret = defaultDataSourceName.Seek(0ull);
    }
    uint32 numberOfContainers = 0u;
    if (ret) {
        numberOfContainers = Size();
        ret = false;
        for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
            Reference item = Get(i);
            if (item.IsValid()) {
                if (StringHelper::Compare(item->GetName(), "States") == 0) {
                    statesContainer = item;
                    ret = statesContainer.IsValid();
                }
            }
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "No States block found in RealTimeApplication %s", GetName())
            }
        }

        if (ret) {
            ret = false;
            for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
                Reference container = Get(i);
                if (container.IsValid()) {
                    if (StringHelper::Compare(container->GetName(), "Data") == 0) {
                        dataSourceContainer = container;
                        ret = dataSourceContainer.IsValid();
                    }
                }
            }
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "No Data block found in RealTimeApplication %s", GetName())
            }
        }

        if (ret) {
            ret = false;
            for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
                Reference item = Get(i);
                if (item.IsValid()) {
                    if (StringHelper::Compare(item->GetName(), "Functions") == 0) {
                        functionsContainer = item;
                        ret = functionsContainer.IsValid();
                    }
                }
            }
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "No Functions block found in RealTimeApplication %s", GetName())
            }
        }

        if (ret) {
            numberOfContainers = Size();
            ret = false;
            for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
                Reference item = Get(i);
                if (item.IsValid()) {
                    if (StringHelper::Compare(item->GetName(), "States") == 0) {
                        statesContainer = item;
                        ret = statesContainer.IsValid();
                    }
                }
            }
            if (ret) {
                ret = false;
                for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
                    Reference container = Get(i);
                    if (container.IsValid()) {
                        if (StringHelper::Compare(container->GetName(), "Data") == 0) {
                            dataSourceContainer = container;
                            ret = dataSourceContainer.IsValid();
                        }
                    }
                }
            }
            if (ret) {
                ret = false;
                for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
                    Reference item = Get(i);
                    if (item.IsValid()) {
                        if (StringHelper::Compare(item->GetName(), "Functions") == 0) {
                            functionsContainer = item;
                            ret = functionsContainer.IsValid();
                        }
                    }
                }
            }

            if (ret) {
                ret = false;
                for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
                    Reference container = Get(i);
                    if (container.IsValid()) {
                        if (StringHelper::Compare(container->GetName(), "Scheduler") == 0) {
                            scheduler = container;
                            ret = scheduler.IsValid();
                        }
                    }
                }
                if (!ret) {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "No Scheduler block in RealTimeApplication %s", GetName())
                }
            }
        }
    }
    return ret;

}

bool RealTimeApplication::ConfigureApplication() {
    RealTimeApplicationConfigurationBuilder rtAppBuilder(*this, "DDB1");
    bool ret = rtAppBuilder.ConfigureAfterInitialisation();

    if (ret) {
        ret = rtAppBuilder.PostConfigureDataSources();
    }
    if (ret) {
        ret = rtAppBuilder.PostConfigureFunctions();
    }
    if (ret) {
        ret = rtAppBuilder.Copy(functionsDatabase, dataSourcesDatabase);
    }
    if (ret) {
        ret = AllocateGAMMemory();
    }
    if (ret) {
        ret = AllocateDataSourceMemory();
    }
    if (ret) {
        ret = AddBrokersToFunctions();
    }
    if (ret) {
        ret = FindStatefulDataSources();
    }
    if (ret) {
        ret = scheduler.IsValid();
        if (ret) {
            ret = scheduler->ConfigureScheduler();
        }
    }

    return ret;
}

bool RealTimeApplication::ConfigureApplication(ConfigurationDatabase &functionsDatabaseIn,
                                               ConfigurationDatabase &dataDatabaseIn) {

    RealTimeApplicationConfigurationBuilder configuration(*this, "DDB1");
    bool ret = configuration.Set(functionsDatabaseIn, dataDatabaseIn);
    if (ret) {
        ret = configuration.AssignBrokersToFunctions();
    }
    if (ret) {
        ret = configuration.Copy(functionsDatabase, dataSourcesDatabase);
    }

    if (ret) {
        ret = configuration.PostConfigureDataSources();
    }
    if (ret) {
        ret = configuration.PostConfigureFunctions();
    }
    if (ret) {
        ret = configuration.ConfigureThreads();
    }
    if (ret) {
        ret = AllocateGAMMemory();
    }
    if (ret) {
        ret = AllocateDataSourceMemory();
    }
    if (ret) {
        ret = AddBrokersToFunctions();
    }
    if (ret) {
        ret = FindStatefulDataSources();
    }
    if (ret) {
        ret = scheduler.IsValid();
        if (ret) {
            ret = scheduler->ConfigureScheduler();
        }
    }
    return ret;
}

bool RealTimeApplication::AllocateGAMMemory() {

    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
        const char8 * functionId = functionsDatabase.GetChildName(i);
        ret = functionsDatabase.MoveRelative(functionId);
        if (ret) {
            StreamString fullGAMName = "Functions.";
            ret = functionsDatabase.Read("QualifiedName", fullGAMName);
            if (ret) {
                ReferenceT<GAM> gam = Find(fullGAMName.Buffer());
                ret = gam.IsValid();

                if (ret) {

                    ret = gam->AllocateInputSignalsMemory();
                    if (ret) {
                        ret = gam->AllocateOutputSignalsMemory();
                    }
                }
            }
        }
        if (ret) {
            ret = functionsDatabase.MoveToAncestor(1u);
        }
    }
    return ret;
}

bool RealTimeApplication::AllocateDataSourceMemory() {
    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    uint32 numberOfDs = dataSourcesDatabase.GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfDs) && (ret); i++) {
        const char8* dsId = dataSourcesDatabase.GetChildName(i);
        ret = dataSourcesDatabase.MoveRelative(dsId);
        if (ret) {
            StreamString fullDsName = "Data.";
            ret = dataSourcesDatabase.Read("QualifiedName", fullDsName);
            if (ret) {
                ReferenceT<DataSourceI> ds = Find(fullDsName.Buffer());
                ret = ds.IsValid();
                if (ret) {
                    ret = ds->AllocateMemory();
                }
            }
        }
        if (ret) {
            ret = dataSourcesDatabase.MoveToAncestor(1u);
        }
    }
    return ret;
}

bool RealTimeApplication::AddBrokersToFunctions() {
    //pre: called after ConfigureApplication(*)
    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    if (ret) {
        uint32 numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
        for (uint32 i = 0u; (i < numberOfDataSources) && (ret); i++) {
            const char8 * dataSourceId = dataSourcesDatabase.GetChildName(i);
            ret = dataSourcesDatabase.MoveRelative(dataSourceId);
            StreamString fullDataSourcePath = "Data.";
            if (ret) {
                ret = dataSourcesDatabase.Read("QualifiedName", fullDataSourcePath);
            }
            ReferenceT<DataSourceI> dataSource;
            if (ret) {
                dataSource = Find(fullDataSourcePath.Buffer());
                ret = dataSource.IsValid();
            }
            if (ret) {

                ret = dataSource->AddBrokers(InputSignals);
                if (ret) {
                    ret = dataSource->AddBrokers(OutputSignals);
                }

            }
            if (ret) {
                ret = dataSourcesDatabase.MoveToAncestor(1u);
            }
        }
    }
    return ret;
}

bool RealTimeApplication::PrepareNextState(const char8 * const nextStateName) {

    bool ret = statesContainer.IsValid();
    if (ret) {
        uint32 numberOfStates = statesContainer->Size();
        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
            ReferenceT<RealTimeState> state = statesContainer->Get(i);
            ret = state.IsValid();
            if (ret) {
                if (StringHelper::Compare(state->GetName(), nextStateName) == 0) {
                    ret = state->PrepareNextState(stateNameHolder[index].Buffer(), nextStateName);
                    break;
                }
            }
        }
    }

    uint32 numberOfStatefulsInData = statefulsInData.Size();
    for (uint32 i = 0u; (i < numberOfStatefulsInData) && (ret); i++) {
        ReferenceT<StatefulI> statefulInData = statefulsInData.Get(i);
        ret = statefulInData.IsValid();
        if (ret) {
            ret = statefulInData->PrepareNextState(stateNameHolder[index].Buffer(), nextStateName);
        }
    }

    if (ret) {
        ret = scheduler.IsValid();
        if (ret) {
            ret = scheduler->PrepareNextState(stateNameHolder[index].Buffer(), nextStateName);
        }
    }
    uint32 nextIndex = (index + 1u) % 2u;
    stateNameHolder[nextIndex] = nextStateName;
    return ret;

}

void RealTimeApplication::StartExecution() {
    index = (index + 1u) % 2u;

    scheduler->StartExecution();
}

void RealTimeApplication::StopExecution() {
    scheduler->StopExecution();
}

bool RealTimeApplication::FindStatefulDataSources() {
    bool ret = dataSourceContainer.IsValid();
    if (ret) {
        //Look for all the StatefulI inside the Data node
        ReferenceContainerFilterReferencesTemplate<StatefulI> dataFilter(-1, ReferenceContainerFilterMode::RECURSIVE);
        dataSourceContainer->Find(statefulsInData, dataFilter);
    }

    return ret;
}

bool RealTimeApplication::GetStates(ReferenceContainer &states) {
    bool ret = statesContainer.IsValid();

    if (ret) {
        // States contains RealTimeState references
        // for each of them call Validate(*)
        uint32 numberOfStates = statesContainer->Size();
        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
            ReferenceT<RealTimeState> state = statesContainer->Get(i);
            if (state.IsValid()) {
                ret = states.Insert(state);
            }
        }
    }
    return ret;
}

uint32 RealTimeApplication::GetIndex() {
    return index;
}

CLASS_REGISTER(RealTimeApplication, "1.0");
}
