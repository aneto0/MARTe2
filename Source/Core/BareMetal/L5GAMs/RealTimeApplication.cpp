/**
 * @file RealTimeApplication.cpp
 * @brief Source file for class RealTimeApplication
 * @date 19/feb/2016
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
#include "ReferenceContainerFilterObjectName.h"
#include "RealTimeDataSourceContainer.h"
#include "GAMI.h"
#include "GAMSchedulerI.h"
#include "RealTimeDataSourceContainer.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static bool ConfigureDataSourcePrivate(ReferenceT<ReferenceContainer> functions) {

    bool ret = functions.IsValid();

    if (ret) {
        uint32 numberOfFunctions = functions->Size();
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            Reference genericFunction = functions->Get(i);

            ReferenceT<GAMI> gam = genericFunction;
            // a GAMI
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
                        ReferenceT<GAMI> subGam = gamGroup->Get(j);
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
                        REPORT_ERROR(ErrorManagement::FatalError, "+Functions must contain GAMI, GAMGroup or ReferenceContainer references");
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

            ReferenceT<GAMI> gam = genericFunction;
            // a GAMI
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
                        ReferenceT<GAMI> subGam = gamGroup->Get(j);
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
                        REPORT_ERROR(ErrorManagement::FatalError, "+Functions must contain GAMI, GAMGroup or ReferenceContainer references");
                    }
                }
            }
        }
    }

    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplication::RealTimeApplication() :
        ReferenceContainer() {
    activeBuffer = 1u;
}

RealTimeApplication::~RealTimeApplication() {

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
                ret = state->ConfigureArchitecture(*this);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+States container not found");
    }
    if (ret) {
        ReferenceT<GAMSchedulerI> scheduler = schedulerContainer;
        ret = (scheduler.IsValid());
        if (ret) {
            scheduler->SetApplication(*this);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "+Scheduler container not found");
        }
    }
    return ret;
}

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
    ReferenceT<RealTimeDataSourceContainer> data = dataSourceContainer;

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
    ReferenceT<RealTimeDataSourceContainer> data = dataSourceContainer;

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
            ReferenceT<RealTimeState> state = Get(i);
            if (state.IsValid()) {
                ret = state->ValidateDataSourceLinks();
            }
        }

    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+States container not found");
    }

    return ret;
}

bool RealTimeApplication::PrepareNextState(const char8 * const nextStateName) {

    RealTimeStateInfo status;
    status.currentState = currentStateName.Buffer();
    status.nextState = nextStateName;
    status.activeBuffer = activeBuffer;

    StreamString nextStatePath = "+States.";
    nextStatePath += nextStateName;

    ReferenceT<RealTimeState> nextState = Find(nextStatePath.Buffer());
    bool ret = nextState.IsValid();
    if (ret) {
        // change the context in gam groups if needed
        nextState->PrepareState(status);
    }
    if (ret) {
        ReferenceT<RealTimeDataSourceContainer> dataSource = dataSourceContainer;
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
        ReferenceT<GAMSchedulerI> scheduler = schedulerContainer;
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
    ReferenceT<GAMSchedulerI> scheduler = schedulerContainer;
    bool ret = scheduler.IsValid();
    // save the accelerator to the next group of threads to be executed
    if (ret) {
        scheduler->StopExecution();
    }
    return ret;
}

bool RealTimeApplication::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        //TODO Read the name of the first state
        // do the PrepareNextState here.

        uint32 numberOfContainers = Size();
        bool found = false;
        for (uint32 i = 0u; (i < numberOfContainers) && (!found); i++) {
            Reference item = Get(i);
            if (item.IsValid()) {
                if (StringHelper::Compare(item->GetName(), "+States") == 0) {
                    statesContainer = item;
                    found = statesContainer.IsValid();
                }
            }
        }
        if (ret) {
            bool found = false;
            for (uint32 i = 0u; (i < numberOfContainers) && (!found); i++) {
                Reference container = Get(i);
                if (container.IsValid()) {
                    if (StringHelper::Compare(container->GetName(), "+Data") == 0) {
                        dataSourceContainer = container;
                        found = dataSourceContainer.IsValid();
                    }
                }
            }
        }
        if (ret) {
            bool found = false;
            for (uint32 i = 0u; (i < numberOfContainers) && (!found); i++) {
                Reference item = Get(i);
                if (item.IsValid()) {
                    if (StringHelper::Compare(item->GetName(), "+Functions") == 0) {
                        functionsContainer = item;
                        found = functionsContainer.IsValid();
                    }
                }
            }
        }
        if (ret) {
            bool found = false;
            for (uint32 i = 0u; (i < numberOfContainers) && (!found); i++) {
                Reference container = Get(i);
                if (container.IsValid()) {
                    if (StringHelper::Compare(container->GetName(), "+Scheduler") == 0) {
                        schedulerContainer = container;
                        found = schedulerContainer.IsValid();
                    }
                }
            }
        }
    }
    return ret;
}

uint8 RealTimeApplication::GetActiveBuffer() const {
    return activeBuffer;
}

CLASS_REGISTER(RealTimeApplication, "1.0");
}
