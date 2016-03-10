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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeApplication.h"
#include "RealTimeState.h"
#include "ReferenceContainerFilterObjectName.h"
#include "RealTimeDataSource.h"
#include "GAM.h"
#include "stdio.h"
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

            ReferenceT<GAM> gam = genericFunction;
            // a GAM
            if (gam.IsValid()) {
                printf("\nConfiguration of %s\n", gam->GetName());
                // call the single gam configuration
                gam->ConfigureDataSource();
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
                            printf("\nConfiguration of %s\n", subGam->GetName());
                            // call the single gam configuration
                            ret = subGam->ConfigureDataSource();
                        }
                    }
                }
                else {
                    // a ReferenceContainer
                    ReferenceT<ReferenceContainer> gamContainer = genericFunction;
                    bool ret = gamContainer.IsValid();
                    if (ret) {
                        // go recursively
                        ret = ConfigureDataSourcePrivate(gamContainer);
                    }
                    else {
                        //TODO What is it?
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
                printf("\nConfiguration of %s\n", gam->GetName());
                // call the single gam configuration
                gam->ConfigureDataSourceLinks();
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
                            printf("\nConfiguration of %s\n", subGam->GetName());
                            // call the single gam configuration
                            ret = subGam->ConfigureDataSourceLinks();
                        }
                    }
                }
                else {
                    // a ReferenceContainer
                    ReferenceT<ReferenceContainer> gamContainer = genericFunction;
                    bool ret = gamContainer.IsValid();
                    if (ret) {
                        // go recursively
                        ret = ConfigureDataSourceLinksPrivate(gamContainer);
                    }
                    else {
                        //TODO What is it?
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

RealTimeApplication::RealTimeApplication() {
    activeBuffer = 0u;
}

bool RealTimeApplication::ConfigureArchitecture() {

    printf("\nStart ConfigureArchitecture\n");
    ReferenceT<ReferenceContainer> statesContainer;
    uint32 numberOfContainers = Size();
    for (uint32 i = 0u; i < numberOfContainers; i++) {
        Reference item = Get(i);
        if (item.IsValid()) {
            if (StringHelper::Compare(item->GetName(), "+States") == 0) {
                statesContainer = item;
                break;
            }
        }
    }

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
        //TODO States container not found
    }

    printf("\nEnd of ConfigureArchitecture with result %d\n", ret);
    return ret;
}

bool RealTimeApplication::ConfigureDataSource() {

    printf("\nStart ConfigureDataSource\n");
    ReferenceT<ReferenceContainer> functionsContainer;
    uint32 numberOfContainers = Size();
    for (uint32 i = 0u; i < numberOfContainers; i++) {
        Reference item = Get(i);
        if (item.IsValid()) {
            if (StringHelper::Compare(item->GetName(), "+Functions") == 0) {
                functionsContainer = item;
                break;
            }
        }
    }

    bool ret = functionsContainer.IsValid();
    if (ret) {
        // configure
        ret = ConfigureDataSourcePrivate(functionsContainer);
    }
    else {
        // TODO Functions container not found
    }

    printf("\nEnd of ConfigureDataSource with result %d\n", ret);

    return ret;

}

bool RealTimeApplication::ValidateDataSource() {

    ReferenceT<RealTimeDataSource> dataContainer;
    uint32 numberOfContainers = Size();
    for (uint32 i = 0u; i < numberOfContainers; i++) {
        Reference item = Get(i);
        if (item.IsValid()) {
            if (StringHelper::Compare(item->GetName(), "+Data") == 0) {
                dataContainer = item;
                break;
            }
        }
    }
    // there must be the container called "States"
    bool ret = dataContainer.IsValid();
    if (ret) {
        // States contains RealTimeState references
        // for each of them call Validate(*)
        ret = dataContainer->Verify();
    }
    return ret;
}

bool RealTimeApplication::AllocateDataSource() {
    ReferenceT<RealTimeDataSource> dataSource;
    uint32 numberOfContainers = Size();
    for (uint32 i = 0u; i < numberOfContainers; i++) {
        Reference container = Get(i);
        if (container.IsValid()) {
            if (StringHelper::Compare(container->GetName(), "+Data") == 0) {
                dataSource = container;
                break;
            }
        }
    }
    bool ret = dataSource.IsValid();
    if (ret) {
        ret = dataSource->Allocate();
    }
    return ret;
}

bool RealTimeApplication::ConfigureDataSourceLinks() {
    ReferenceT<ReferenceContainer> functionsContainer;
    uint32 numberOfContainers = Size();
    for (uint32 i = 0u; i < numberOfContainers; i++) {
        Reference item = Get(i);
        if (item.IsValid()) {
            if (StringHelper::Compare(item->GetName(), "+Functions") == 0) {
                functionsContainer = item;
                break;
            }
        }
    }    // there must be the container called "States"
    bool ret = functionsContainer.IsValid();
    if (ret) {
        // configure
        ret = ConfigureDataSourceLinksPrivate(functionsContainer);
    }
    else {
        // TODO Functions container not found
    }


    return ret;
}

bool RealTimeApplication::PrepareNextState(const RealTimeStateInfo &status) {

    StreamString nextStatePath = "+States.";
    nextStatePath += status.nextState;

    ReferenceT<RealTimeState> nextState = Find(nextStatePath.Buffer());
    bool ret = nextState.IsValid();
    if (ret) {
        nextState->PrepareState(status);
    }
    if (ret) {
        ReferenceT<RealTimeDataSource> dataSource;
        uint32 numberOfContainers = Size();
        for (uint32 i = 0u; i < numberOfContainers; i++) {
            Reference container = Get(i);
            if (container.IsValid()) {
                if (StringHelper::Compare(container->GetName(), "+Data") == 0) {
                    dataSource = container;
                    break;
                }
            }
        }
        ret = dataSource.IsValid();
        if (ret) {
            ret = dataSource->PrepareNextState(status);
        }
    }

    return ret;
}

uint8 RealTimeApplication::ChangeState() {
    activeBuffer = (activeBuffer + 1u) % 2u;
    return activeBuffer;
}

uint8 RealTimeApplication::GetActiveBuffer() const {
    return activeBuffer;
}

CLASS_REGISTER(RealTimeApplication, "1.0");
}
