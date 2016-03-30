/**
 * @file RealTimeThread.cpp
 * @brief Source file for class RealTimeThread
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
 * the class RealTimeThread (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeThread.h"
#include "Vector.h"
#include "ObjectRegistryDatabase.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferences.h"
#include "GAMI.h"
#include "AdvancedErrorManagement.h"
#include "GAMSchedulerI.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

// the allocation granularity
static const uint32 gamsArrayGranularity = 4u;
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeThread::RealTimeThread() :
        ReferenceContainer() {
    functions = NULL_PTR(StreamString*);
    numberOfFunctions = 0u;
    GAMs = reinterpret_cast<ReferenceT<GAMI>*>(NULL);
    numberOfGAMs = 0u;
    cpuMask = 0u;
    stackSize = 0u;
}

/*lint -e{1551} no exception should be thrown*/
RealTimeThread::~RealTimeThread() {
    if (functions != NULL) {
        delete[] functions;
    }
    if (GAMs != NULL) {
        delete[] GAMs;
    }
}

bool RealTimeThread::ConfigureArchitecturePrivate(Reference functionGeneric,
                                                  RealTimeApplication &rtApp,
                                                  RealTimeState &rtState) {

    bool ret = true;
    // case GAMGroup (stateful gams)
    ReferenceT<GAMGroup> functionGAMGroup = functionGeneric;
    if (functionGAMGroup.IsValid()) {
        // add the GAMGroup to the RealTimeState accelerator array
        functionGAMGroup->AddState(rtState.GetName());
        rtState.AddGAMGroup(functionGAMGroup);
        uint32 nOfSubGAMs = functionGAMGroup->Size();
        // add all the gams in the order of the configuration inside GAMGroup
        for (uint32 j = 0u; (j < nOfSubGAMs) && (ret); j++) {
            ReferenceT<GAMI> subGam = functionGAMGroup->Get(j);
            ret = subGam.IsValid();
            if (ret) {
                AddGAM(subGam);
                subGam->SetApplication(rtApp);
                subGam->SetGAMGroup(functionGAMGroup);
            }
        }
    }
    else {
        // case stateless GAMI
        ReferenceT<GAMI> functionGAM = functionGeneric;
        if (functionGAM.IsValid()) {
            AddGAM(functionGAM);
            functionGAM->SetApplication(rtApp);
            // if it is a stateful GAMI, add its GAMGroup
            ReferenceContainerFilterReferences filterGAM(1, ReferenceContainerFilterMode::PATH, functionGAM);
            ReferenceContainer result;
            ObjectRegistryDatabase::Instance()->ReferenceContainer::Find(result, filterGAM);
            ret = (result.Size() > 1u); //there must be the father!
            if (ret) {
                ReferenceT<GAMGroup> gamGroup = result.Get(result.Size() - 2u);
                if (gamGroup.IsValid()) {
                    rtState.AddGAMGroup(gamGroup);
                    functionGAM->SetGAMGroup(gamGroup);
                    gamGroup->AddState(rtState.GetName());
                }
                else {
                    functionGAM->AddState(rtState.GetName());
                }
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The GAMI %s must be defined in the +Function container of the application",
                                        functionGAM->GetName())
            }
        }
        else {
            // a generic container
            ReferenceT<ReferenceContainer> functionContainer = functionGeneric;
            ret = functionContainer.IsValid();
            if (ret) {
                uint32 size = functionContainer->Size();
                for (uint32 i = 0u; (i < size) && (ret); i++) {
                    Reference newRef = functionContainer->Get(i);
                    // go recursively
                    ret = ConfigureArchitecturePrivate(newRef, rtApp, rtState);
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "The function be a GAMI, GAMGroups or ReferenceContainer");
            }
        }

    }
    return ret;
}

bool RealTimeThread::ValidateDataSourceLinks() {
    bool ret = true;
    uint32 contSyncGAMs = 0u;
    for (uint32 i = 0u; (i < numberOfGAMs) && (ret); i++) {
        if (GAMs[i]->IsSync()) {
            contSyncGAMs++;
        }
    }
    if (contSyncGAMs > 1u) {
        //TODO Error
        ret = false;
    }
    return ret;

}

void RealTimeThread::AddGAM(ReferenceT<GAMI> element) {
    if (element.IsValid()) {
        if ((numberOfGAMs % gamsArrayGranularity) == 0u) {
            uint32 newSize = numberOfGAMs + gamsArrayGranularity;
            ReferenceT<GAMI> *temp = new ReferenceT<GAMI> [newSize];

            if (GAMs != NULL) {
                for (uint32 i = 0u; i < numberOfGAMs; i++) {
                    temp[i] = GAMs[i];
                }
                delete[] GAMs;
            }
            GAMs = temp;
        }
        /*lint -e{613} before entering the memory for GAMs is allocated because (numberOfGAMs == 0)*/
        GAMs[numberOfGAMs] = element;
        numberOfGAMs++;

    }
}

bool RealTimeThread::ConfigureArchitecture(RealTimeApplication &rtApp,
                                           RealTimeState &rtState) {

    bool ret = true;

    for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {

        // find the functions specified in cdb
        /*lint -e{613} Never enter here if (functions == NULL) because (numberOfFunctions == 0) */
        Reference functionGeneric = ObjectRegistryDatabase::Instance()->Find(functions[i].Buffer(), Reference(this));
        ret = functionGeneric.IsValid();
        if (ret) {
            // configure
            ret = ConfigureArchitecturePrivate(functionGeneric, rtApp, rtState);
            if (ret) {
                // insert here the reference
                ret = Insert(functionGeneric);
                if (ret) {
                    // insert the reference into the state
                    ret = rtState.InsertFunction(functionGeneric);
                }
                // insert this thread in the scheduler
                if (ret) {
                    //search the scheduler
                    uint32 numberOfItems = rtApp.Size();
                    ret = false;
                    ReferenceT<GAMSchedulerI> scheduler;
                    for (uint32 i = 0u; (i < numberOfItems) && (!ret); i++) {
                        Reference item = rtApp.Get(i);
                        if (item.IsValid()) {
                            if (StringHelper::Compare(item->GetName(), "+Scheduler") == 0) {
                                scheduler = item;
                                ret = scheduler.IsValid();
                            }
                        }
                    }

                    if (ret) {
                        ret = scheduler->InsertRecord(rtState.GetName(), ReferenceT<RealTimeThread>(this));
                    }
                    else {
                        //TODO Scheduler not found or invalid
                    }
                }
            }
        }
        else {
            /*lint -e{613} Never enter here if (functions == NULL) because (numberOfFunctions == 0) */
            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Undefined %s", functions[i].Buffer())
        }
    }

    return ret;
}

bool RealTimeThread::Initialise(StructuredDataI & data) {
    // set up the string array
    AnyType functionsArray = data.GetType("Functions");
    bool ret = (functionsArray.GetDataPointer() != NULL);

    if (ret) {
        numberOfFunctions = functionsArray.GetNumberOfElements(0u);
        functions = new StreamString[numberOfFunctions];

        Vector<StreamString> functionVector(functions, numberOfFunctions);

        ret = (data.Read("Functions", functionVector));
        if (ret) {
            if (data.Read("CPUs", cpuMask)) {

            }
            if (data.Read("StackSize", stackSize)) {

            }
        }

    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "No functions defined for the RealTimeThread %s", GetName())
    }
    return ret;
}

StreamString * RealTimeThread::GetFunctions() {
    return functions;
}

uint32 RealTimeThread::GetNumberOfFunctions() const {
    return numberOfFunctions;
}

ReferenceT<GAMI> *RealTimeThread::GetGAMs() {
    return GAMs;
}

uint32 RealTimeThread::GetNumberOfGAMs() const {
    return numberOfGAMs;
}

uint32 RealTimeThread::GetStackSize() const {
    return stackSize;
}

ProcessorType RealTimeThread::GetCPU() const {
    return cpuMask;
}

bool RealTimeThread::ToStructuredData(StructuredDataI& data) {
    const char8 * objName = GetName();
    bool ret = data.CreateRelative(objName);
    if (ret) {
        ret = data.Write("Class", "RealTimeThread");
        if (ret) {
            if (functions != NULL) {
                Vector<StreamString> functionVector(functions, numberOfFunctions);
                ret = data.Write("Functions", functionVector);
            }
        }
        if (ret) {
            uint32 numberOfChildren = Size();
            for (uint32 i = 0u; i < numberOfChildren; i++) {
                Reference child = Get(i);
                ret = child.IsValid();
                if (ret) {
                    if (ret) {
                        ret = child->ToStructuredData(data);
                    }
                }
            }
        }
        if (!data.MoveToAncestor(1u)) {
            ret = false;
        }
    }
    return ret;
}

CLASS_REGISTER(RealTimeThread, "1.0");

}
