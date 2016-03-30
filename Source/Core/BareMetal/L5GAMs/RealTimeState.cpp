/**
 * @file RealTimeState.cpp
 * @brief Source file for class RealTimeState
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
 * the class RealTimeState (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeState.h"
#include "RealTimeThread.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

// the allocation granularity
static const uint32 functionArrayGranularity = 8u;
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeState::RealTimeState() :
        ReferenceContainer() {
    statefulGAMGroups = reinterpret_cast<ReferenceT<GAMGroup>*>(NULL);
    numberOfGAMGroups = 0u;
}

/*lint -e{1551} no exception should be thrown*/
RealTimeState::~RealTimeState() {
    if (statefulGAMGroups != NULL) {
        delete[] statefulGAMGroups;
    }
}

bool RealTimeState::ConfigureArchitecture(RealTimeApplication & rtApp) {

    // there must be a container called Threads
    ReferenceT<ReferenceContainer> threadContainer;
    uint32 numberOfContainers = Size();
    bool ret = false;
    for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
        Reference item = Get(i);
        if (item.IsValid()) {
            if (StringHelper::Compare(item->GetName(), "+Threads") == 0) {
                threadContainer = item;
                ret = threadContainer.IsValid();
            }
        }
    }

    if (ret) {
        // for each thread call the Validate
        uint32 numberOfThreads = threadContainer->Size();
        for (uint32 i = 0u; (i < numberOfThreads) && (ret); i++) {
            ReferenceT<RealTimeThread> rtThread = threadContainer->Get(i);
            if (rtThread.IsValid()) {
                // call the configure function for each thread
                ret = rtThread->ConfigureArchitecture(rtApp, *this);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+Threads container not found");
    }

    return ret;
}

bool RealTimeState::ValidateDataSourceLinks() {
    // there must be a container called Threads
    ReferenceT<ReferenceContainer> threadContainer;
    uint32 numberOfContainers = Size();
    bool ret = false;
    for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
        Reference item = Get(i);
        if (item.IsValid()) {
            if (StringHelper::Compare(item->GetName(), "+Threads") == 0) {
                threadContainer = item;
                ret = threadContainer.IsValid();
            }
        }
    }

    if (ret) {
        // for each thread call the Validate
        uint32 numberOfThreads = threadContainer->Size();
        for (uint32 i = 0u; (i < numberOfThreads) && (ret); i++) {
            ReferenceT<RealTimeThread> rtThread = threadContainer->Get(i);
            if (rtThread.IsValid()) {
                // call the configure function for each thread
                ret = rtThread->ValidateDataSourceLinks();
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+Threads container not found");
    }

    return ret;
}

bool RealTimeState::InsertFunction(Reference functionReference) {
    ReferenceT<ReferenceContainer> functionsContainer;
    uint32 numberOfContainers = Size();
    for (uint32 i = 0u; i < numberOfContainers; i++) {
        Reference item = Get(i);
        if (item.IsValid()) {
            if (StringHelper::Compare(item->GetName(), "+Functions") == 0) {
                functionsContainer = item;
            }
        }
    }
    bool ret = functionsContainer.IsValid();
    // if the functions container does not exists, create it.
    if (!ret) {
        functionsContainer = ReferenceT<ReferenceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ret = functionsContainer.IsValid();
        if (ret) {
            functionsContainer->SetName("+Functions");
            ret = Insert(functionsContainer);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed creation of +Functions container");
        }
    }

    if (ret) {
        ret = functionsContainer->Insert(functionReference);
    }
    return ret;
}

void RealTimeState::AddGAMGroup(ReferenceT<GAMGroup> element) {
    if (element.IsValid()) {
        bool found = false;

        for (uint32 i = 0u; (i < numberOfGAMGroups) && (!found); i++) {
            /*lint -e{613} cannot enter here if (statefulGAMGroups == NULL) because (numberOfGAMGroups == 0)*/
            found = (element == statefulGAMGroups[i]);
        }

        if (!found) {
            if ((numberOfGAMGroups % functionArrayGranularity) == 0u) {
                uint32 newSize = numberOfGAMGroups + functionArrayGranularity;
                ReferenceT<GAMGroup> *temp = new ReferenceT<GAMGroup> [newSize];
                if (statefulGAMGroups != NULL) {
                    for (uint32 i = 0u; i < numberOfGAMGroups; i++) {
                        temp[i] = statefulGAMGroups[i];
                    }
                    delete[] statefulGAMGroups;
                }

                statefulGAMGroups = temp;
            }

            /*lint -e{613} before entering the memory for statefulGAMGroups is allocated because (numberOfGAMGroups == 0)*/
            statefulGAMGroups[numberOfGAMGroups] = element;
            numberOfGAMGroups++;
        }
    }

}

void RealTimeState::PrepareState(const RealTimeStateInfo &status) {
    for (uint32 i = 0u; i < numberOfGAMGroups; i++) {
        /*lint -e{613} cannot enter here if (statefulGAMGroups == NULL) because (numberOfGAMGroups == 0)*/
        statefulGAMGroups[i]->PrepareNextState(status);
    }
}

ReferenceT<GAMGroup> * RealTimeState::GetGAMGroups() {
    return statefulGAMGroups;
}

uint32 RealTimeState::GetNumberOfGAMGroups() const {
    return numberOfGAMGroups;
}

CLASS_REGISTER(RealTimeState, "1.0");

}
