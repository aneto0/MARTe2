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
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

uint32 RealTimeApplication::index = 1u;

#if 0
static bool ConfigureDataSourcePrivate(ReferenceT<ReferenceContainer> functions) {

    bool ret = functions.IsValid();

    if (ret) {
        uint32 numberOfFunctions = functions->Size();
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            Reference genericFunction = functions->Get(i);

            ReferenceT<GAM> gam = genericFunction;
            // a GAM
            if (gam.IsValid()) {
                // call the single gam configuration
                ret = gam->ConfigureDataSource();
            }
            else {
                // a GAMGroup
                ReferenceT<GAMGroup> gamGroup = genericFunction;
                if (gamGroup.IsValid()) {
                    uint32 numberOfSubGams = gamGroup->Size();
                    for (uint32 j = 0u; (j < numberOfSubGams) && (ret); j++) {
                        ReferenceT<GAM> subGam = gamGroup->Get(j);
                        ret = subGam.IsValid();
                        if (ret) {
                            // call the single gam configuration
                            ret = subGam->ConfigureDataSource();
                        }
                    }
                }
                else {
                    // a ReferenceContainer
                    ReferenceT<ReferenceContainer> gamContainer = genericFunction;
                    ret = gamContainer.IsValid();
                    if (ret) {
                        // go recursively
                        ret = ConfigureDataSourcePrivate(gamContainer);
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "+Functions must contain GAM, GAMGroup or ReferenceContainer references");
                    }
                }
            }
        }
    }

    return ret;
}

static bool ConfigureDataSourceLinksPrivate(ReferenceT<ReferenceContainer> functions) {

    bool ret = functions.IsValid();

    if (ret) {
        uint32 numberOfFunctions = functions->Size();
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            Reference genericFunction = functions->Get(i);

            ReferenceT<GAM> gam = genericFunction;
            // a GAM
            if (gam.IsValid()) {
                // call the single gam configuration
                ret = gam->ConfigureDataSourceLinks();
            }
            else {
                // a GAMGroup
                ReferenceT<GAMGroup> gamGroup = genericFunction;
                if (gamGroup.IsValid()) {
                    uint32 numberOfSubGams = gamGroup->Size();
                    for (uint32 j = 0u; (j < numberOfSubGams) && (ret); j++) {
                        ReferenceT<GAM> subGam = gamGroup->Get(j);
                        ret = subGam.IsValid();
                        if (ret) {
                            // call the single gam configuration
                            ret = subGam->ConfigureDataSourceLinks();
                        }
                    }
                }
                else {
                    // a ReferenceContainer
                    ReferenceT<ReferenceContainer> gamContainer = genericFunction;
                    ret = gamContainer.IsValid();
                    if (ret) {
                        // go recursively
                        ret = ConfigureDataSourceLinksPrivate(gamContainer);
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "+Functions must contain GAM, GAMGroup or ReferenceContainer references");
                    }
                }
            }
        }
    }

    return ret;
}
#endif
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplication::RealTimeApplication() :
        ReferenceContainer() {
}

RealTimeApplication::~RealTimeApplication() {

}
#if 0

bool RealTimeApplication::ConfigureDataSource() {

    bool ret = functionsContainer.IsValid();

    if (ret) {
        // configure
        ret = ConfigureDataSourcePrivate(functionsContainer);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+Functions container not found");
    }

    return ret;

}

bool RealTimeApplication::ValidateDataSource() {
    ReferenceT<DataSourceContainer> data = dataSourceContainer;

    bool ret = data.IsValid();
    // there must be the container called "States"

    if (ret) {
        // States contains RealTimeState references
        // for each of them call Validate(*)
        ret = data->Verify();
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+Data container not found");
    }
    return ret;
}

bool RealTimeApplication::AllocateDataSource() {
    ReferenceT<DataSourceContainer> data = dataSourceContainer;

    bool ret = data.IsValid();
    if (ret) {
        ret = data->Allocate();
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+Data container not found");
    }
    return ret;
}

bool RealTimeApplication::ConfigureDataSourceLinks() {
    // there must be the container called "States"
    bool ret = functionsContainer.IsValid();
    if (ret) {
        // configure
        ret = ConfigureDataSourceLinksPrivate(functionsContainer);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+Functions container not found");
    }

    return ret;
}

bool RealTimeApplication::ValidateDataSourceLinks() {
    // there must be the container called "States"
    bool ret = statesContainer.IsValid();
    if (ret) {
        uint32 numberOfStates = statesContainer->Size();
        for (uint32 i = 0u; (i < numberOfStates) && ret; i++) {
            ReferenceT<RealTimeState> state = statesContainer->Get(i);
            ret = state.IsValid();
            if (ret) {
                ret = state->ValidateDataSourceLinks();
            }
        }

    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+States container not found");
    }

    return ret;
}
bool RealTimeApplication::PrepareNextState(const char8 * stateNameHolder,
        const char8 * nextStateName) {

    RealTimeStateInfo status;
    status.currentState = stateNameHolder.Buffer();
    status.nextState = nextStateName;
    status.activeBuffer = activeBuffer;

    StreamString nextStatePath = "States.";
    nextStatePath += nextStateName;

    ReferenceT<RealTimeState> nextState = Find(nextStatePath.Buffer());
    bool ret = nextState.IsValid();
    if (ret) {
        // change the context in gam groups if needed
        nextState->PrepareState(status);
    }
    if (ret) {
        ReferenceT<DataSourceContainer> dataSource = dataSourceContainer;
        ret = dataSource.IsValid();
        if (ret) {
            // resets the default value in data sources if needed
            ret = dataSource->PrepareNextState(status);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "+Data container not found");
        }
    }
    if (ret) {
        ReferenceT<GAMSchedulerI> scheduler = scheduler;
        ret = scheduler.IsValid();
        // save the accelerator to the next group of threads to be executed
        if (ret) {
            ret = scheduler->PrepareNextState(status);
        }
        if (ret) {
            scheduler->ChangeState(activeBuffer);
        }
    }
    activeBuffer = (activeBuffer + 1u) % 2u;

    return ret;
}

bool RealTimeApplication::StopExecution() {
    ReferenceT<GAMSchedulerI> scheduler = scheduler;
    bool ret = scheduler.IsValid();
    if (ret) {
        scheduler->StopExecution();
    }
    return ret;
}
#endif

bool RealTimeApplication::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (data.MoveRelative("+Data")) {
        if (!data.Read("DefaultDataSource", defaultDataSourceName)) {
            defaultDataSourceName = "";
        }
        data.MoveToAncestor(1u);
    }
    defaultDataSourceName.Seek(0u);

    if (ret) {
        uint32 numberOfContainers = Size();
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
        rtAppBuilder.Copy(functionsDatabase, dataSourcesDatabase);
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
        ret = scheduler.IsValid();
        if (ret) {
            ret = scheduler->ConfigureScheduler();
        }
    }

    return ret;
}

bool RealTimeApplication::ConfigureApplication(ConfigurationDatabase &functionsDatabaseIn,
                                               ConfigurationDatabase &dataDatabaseIn) {
    // no check..someone else did it

    //-External compiling
    //-Initialization
    // This Function {
    //    -Brokers negotiation
    //    -PostConfig
    // }

    // TODO Standard checks can be done (the Verifies)
    //TODO

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
        ret = ConfigureArchitecture();
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
    for (uint32 i = 0u; i < numberOfFunctions && ret; i++) {
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
    for (uint32 i = 0u; i < numberOfDs && ret; i++) {
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
        for (uint32 i = 0u; i < numberOfDataSources && ret; i++) {
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
        for (uint32 i = 0u; i < numberOfStates && ret; i++) {
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
    for (uint32 i = 0u; i < numberOfStatefulsInData && ret; i++) {
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
    stateNameHolder[(index + 1u) % 2u] = nextStateName;
    return ret;

}

void RealTimeApplication::StartExecution() {
    index = (index + 1u) % 2u;

    scheduler->StartExecution();
}

void RealTimeApplication::StopExecution() {
    scheduler->StopExecution();
}

bool RealTimeApplication::ConfigureArchitecture() {

    // there must be the container called "States"
    bool ret = statesContainer.IsValid();

    if (ret) {
        // States contains RealTimeState references
        // for each of them call Validate(*)
        uint32 numberOfStates = statesContainer->Size();
        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
            ReferenceT<RealTimeState> state = statesContainer->Get(i);
            if (state.IsValid()) {

                // for each state call the configuration function
                uint32 numberOfThreads = state->Size();
                for (uint32 j = 0u; (j < numberOfThreads) && (ret); j++) {
                    ReferenceT<RealTimeThread> thread = state->Get(j);
                    if (thread.IsValid()) {
                        ret = thread->ConfigureArchitecture();
                    }
                }
            }
        }
    }

    if (ret) {
        ret = dataSourceContainer.IsValid();
    }
    if (ret) {
        //Look for all the GAMs inside the RealTimeApplication
        ReferenceContainerFilterReferencesTemplate<StatefulI> dataFilter(-1, ReferenceContainerFilterMode::RECURSIVE);
        dataSourceContainer->Find(statefulsInData, dataFilter);
    }

    if (ret) {
        ret = (scheduler.IsValid());
        if (ret) {
            ret = scheduler->ConfigureScheduler();
        }
    }
    return ret;
}

uint32 RealTimeApplication::GetIndex() {
    return index;
}

#if 0
uint8 RealTimeApplication::GetActiveBuffer() const {
    return activeBuffer;
}

ReferenceT<ReferenceContainer> RealTimeApplication::GetDataSourceContainer() {
    return dataSourceContainer;
}
#endif

CLASS_REGISTER(RealTimeApplication, "1.0");
}
