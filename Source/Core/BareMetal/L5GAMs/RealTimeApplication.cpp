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

#include "AdvancedErrorManagement.h"
#include "GAM.h"
#include "GAMSchedulerI.h"
#include "Matrix.h"
#include "RealTimeApplication.h"
#include "RealTimeApplicationConfigurationBuilder.h"
#include "RealTimeState.h"
#include "RealTimeThread.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferencesTemplate.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplication::RealTimeApplication() :
        ReferenceContainer(), MessageI() {
    filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
    defaultDataSourceName = "";
    index=1u;
    checkSameGamInMoreThreads=true;
    checkMultipleProducersWrites=true;
}

/*lint -e{1551} Guarantess that the execution is stopped upon destrucion of the RealTimeApplication*/
RealTimeApplication::~RealTimeApplication() {
    ErrorManagement::ErrorType ret = StopCurrentStateExecution();
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not stop the RealTimeApplication. Was it ever started?");
    }

}
bool RealTimeApplication::Initialise(StructuredDataI & data) {
    index = 1u;

    bool ret = ReferenceContainer::Initialise(data);

    if(ret){
        uint8 checkSameGamInMoreThreadsT=1u;
        if(!data.Read("CheckSameGamInMoreThreads", checkSameGamInMoreThreadsT)){
            checkSameGamInMoreThreadsT=1u;
        }
        checkSameGamInMoreThreads=(checkSameGamInMoreThreadsT>0u);
    }
    if(ret){
        uint8 checkMultipleProducersWritesT=1u;
        if(!data.Read("CheckMultipleProducersWrites", checkMultipleProducersWritesT)){
            checkMultipleProducersWritesT=1u;
        }
        checkMultipleProducersWrites=(checkMultipleProducersWritesT>0u);
    }


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
        }
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "No States block found in RealTimeApplication %s", GetName());
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
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "No Data block found in RealTimeApplication %s", GetName());
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
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "No Functions block found in RealTimeApplication %s", GetName());
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
                    REPORT_ERROR(ErrorManagement::InitialisationError, "No Scheduler block in RealTimeApplication %s", GetName());
                }
            }
        }
    }
    return ret;

}

bool RealTimeApplication::ConfigureApplication() {
    RealTimeApplicationConfigurationBuilder rtAppBuilder(*this, defaultDataSourceName.Buffer());
    REPORT_ERROR(ErrorManagement::Information, "Going to rtAppBuilder.ConfigureAfterInitialisation()");
    bool ret = rtAppBuilder.ConfigureAfterInitialisation();
    if (!ret) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Failed to rtAppBuilder.ConfigureAfterInitialisation()");
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to rtAppBuilder.PostConfigureDataSources()");
        ret = rtAppBuilder.PostConfigureDataSources();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to rtAppBuilder.PostConfigureDataSources()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to rtAppBuilder.PostConfigureFunctions()");
        ret = rtAppBuilder.PostConfigureFunctions();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to rtAppBuilder.PostConfigureFunctions()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to rtAppBuilder.Copy()");
        ret = rtAppBuilder.Copy(functionsDatabase, dataSourcesDatabase);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to rtAppBuilder.Copy()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to AllocateGAMMemory");
        ret = AllocateGAMMemory();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to AllocateGAMMemory");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to AllocateDataSourceMemory()");
        ret = AllocateDataSourceMemory();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to AllocateDataSourceMemory()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to AddBrokersToFunctions");
        ret = AddBrokersToFunctions();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to AddBrokersToFunctions");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to FindStatefulDataSources");
        ret = FindStatefulDataSources();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to FindStatefulDataSources");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to configure scheduler");
        ret = scheduler.IsValid();
        if (ret) {
            ret = scheduler->ConfigureScheduler(this);
        }
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to configure scheduler");
        }
    }

    return ret;
}

bool RealTimeApplication::ConfigureApplication(ConfigurationDatabase &functionsDatabaseIn, ConfigurationDatabase &dataDatabaseIn) {

    RealTimeApplicationConfigurationBuilder configuration(*this, "DDB1");
    bool ret = configuration.Set(functionsDatabaseIn, dataDatabaseIn);
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to configuration.AssignBrokersToFunctions()");
        ret = configuration.AssignBrokersToFunctions();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to configuration.AssignBrokersToFunctions()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to configuration.Copy()");
        ret = configuration.Copy(functionsDatabase, dataSourcesDatabase);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to configuration.Copy()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to configuration.PostConfigureDataSources()");
        ret = configuration.PostConfigureDataSources();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to configuration.PostConfigureDataSources()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to configuration.PostConfigureFunctions()");
        ret = configuration.PostConfigureFunctions();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to configuration.PostConfigureFunctions()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to configuration.ConfigureThreads()");
        ret = configuration.ConfigureThreads();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to configuration.ConfigureThreads()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to AllocateGAMMemory()");
        ret = AllocateGAMMemory();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to AllocateGAMMemory()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to AllocateDataSourceMemory()");
        ret = AllocateDataSourceMemory();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to AllocateDataSourceMemory()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to AddBrokersToFunctions()");
        ret = AddBrokersToFunctions();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to AddBrokersToFunctions()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to FindStatefulDataSources()");
        ret = FindStatefulDataSources();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to FindStatefulDataSources()");
        }
    }
    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Going to check configure scheduler");
        ret = scheduler.IsValid();
        if (ret) {
            ret = scheduler->ConfigureScheduler(this);
        }
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to configure scheduler");
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
                    if (ret) {
                        ret = gam->Setup();
                        if (!ret) {
                            REPORT_ERROR(ErrorManagement::ParametersError, "GAM %s Setup failed", fullGAMName.Buffer());
                        }
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
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource %s AllocateMemory failed", fullDsName.Buffer());
                    }
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
                REPORT_ERROR(ErrorManagement::Information, "Getting input brokers for %s", dataSource->GetName());
                if (ret) {
                    ret = dataSource->AddBrokers(OutputSignals);
                    REPORT_ERROR(ErrorManagement::Information, "Getting output brokers for %s", dataSource->GetName());
                }

            }
            if (ret) {
                ret = dataSourcesDatabase.MoveToAncestor(1u);
            }
        }
    }

    //sort the data sources
    uint32 numberOfFunctions = functionsContainer->Size();
    for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
        ReferenceT<GAM> function = functionsContainer->Get(i);
        if (function.IsValid()) {
            ret = function->SortBrokers();
        }
    }

    return ret;
}

ErrorManagement::ErrorType RealTimeApplication::PrepareNextState(StreamString nextStateName) {
    bool ret = nextStateName.Seek(0LLU);
    if (ret) {
        ret = statesContainer.IsValid();
    }
    if (ret) {
        uint32 numberOfStates = statesContainer->Size();
        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
            ReferenceT<RealTimeState> state = statesContainer->Get(i);
            ret = state.IsValid();
            if (ret) {
                if (StringHelper::Compare(state->GetName(), nextStateName.Buffer()) == 0) {
                    ret = state->PrepareNextState(stateNameHolder[index].Buffer(), nextStateName.Buffer());
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
            ret = statefulInData->PrepareNextState(stateNameHolder[index].Buffer(), nextStateName.Buffer());
        }
    }

    if (ret) {
        ret = scheduler.IsValid();
        if (ret) {
            ret = scheduler->PrepareNextState(stateNameHolder[index].Buffer(), nextStateName.Buffer());
        }
    }
    uint32 nextIndex = (index + 1u) % 2u;
    stateNameHolder[nextIndex] = nextStateName;
    return ret;

}

ErrorManagement::ErrorType RealTimeApplication::StartNextStateExecution() {
    index = (index + 1u) % 2u;
    ErrorManagement::ErrorType ret = scheduler.IsValid();
    if (ret.ErrorsCleared()) {
        ret = scheduler->StartNextStateExecution();
    }
    return ret;
    /*lint -e{1762} Member function cannot be made const as it is registered as an RPC in CLASS_METHOD_REGISTER*/
}

ErrorManagement::ErrorType RealTimeApplication::StopCurrentStateExecution() {
    ErrorManagement::ErrorType ret = scheduler.IsValid();
    if (ret.ErrorsCleared()) {
        ret = scheduler->StopCurrentStateExecution();
    }
    return ret;
    /*lint -e{1762} Member function cannot be made const as it is registered as an RPC in CLASS_METHOD_REGISTER*/
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

bool RealTimeApplication::GetStates(ReferenceContainer &states) const {
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

uint32 RealTimeApplication::GetIndex() const {
    return index;
}

void RealTimeApplication::Purge(ReferenceContainer &purgeList) {
    if (statesContainer.IsValid()) {
        statesContainer->Purge(purgeList);
        statesContainer.RemoveReference();
    }
    if (functionsContainer.IsValid()) {
        functionsContainer->Purge(purgeList);
        functionsContainer.RemoveReference();
    }
    if (scheduler.IsValid()) {
        scheduler->Purge(purgeList);
        scheduler.RemoveReference();
    }
    if (dataSourceContainer.IsValid()) {
        dataSourceContainer->Purge(purgeList);
        dataSourceContainer.RemoveReference();
    }
    statefulsInData.Purge(purgeList);
    functionsDatabase.Purge();
    dataSourcesDatabase.Purge();
    ReferenceContainer::Purge(purgeList);
}


bool RealTimeApplication::CheckSameGamInMoreThreads() const{
    return checkSameGamInMoreThreads;
}

bool RealTimeApplication::CheckMultipleProducersWrites() const{
    return checkMultipleProducersWrites;
}


CLASS_REGISTER(RealTimeApplication, "1.0")

CLASS_METHOD_REGISTER(RealTimeApplication, PrepareNextState)
CLASS_METHOD_REGISTER(RealTimeApplication, StartNextStateExecution)
CLASS_METHOD_REGISTER(RealTimeApplication, StopCurrentStateExecution)

}
