/**
 * @file RealTimeThread.cpp
 * @brief Source file for class RealTimeThread
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
#include "GAM.h"
#include "AdvancedErrorManagement.h"
#include "GAMSchedulerI.h"
#include "ReferenceContainerFilterReferences.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeThread::RealTimeThread() :
        ReferenceContainer() {
    functions = NULL_PTR(StreamString*);
    numberOfFunctions = 0u;
    numberOfGAMs = 0u;
    cpuMask = ProcessorType::GetDefaultCPUs();
    stackSize = THREADS_DEFAULT_STACKSIZE;
    configured = false;
}

/*lint -e{1551} no exception should be thrown*/
RealTimeThread::~RealTimeThread() {
    if (functions != NULL) {
        delete[] functions;
    }
}

bool RealTimeThread::ConfigureArchitecturePrivate(Reference functionGeneric) {
    bool ret = true;
    // case GAMGroup (stateful gams)
    ReferenceT<GAMGroup> functionGAMGroup = functionGeneric;
    if (functionGAMGroup.IsValid()) {
        uint32 nOfSubGAMs = functionGAMGroup->Size();
        // add all the gams in the order of the configuration inside GAMGroup
        for (uint32 j = 0u; (j < nOfSubGAMs) && (ret); j++) {
            ReferenceT<GAM> subGam = functionGAMGroup->Get(j);
            ret = subGam.IsValid();
            if (ret) {
                AddGAM(subGam);
            }
        }
    }
    else {
        // case stateless GAM
        ReferenceT<GAM> functionGAM = functionGeneric;
        if (functionGAM.IsValid()) {
            AddGAM(functionGAM);
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
                    ret = ConfigureArchitecturePrivate(newRef);
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "The function must be a GAM, GAMGroup or ReferenceContainer");
            }
        }

    }
    return ret;
}

void RealTimeThread::AddGAM(ReferenceT<GAM> element) {
    if (element.IsValid()) {
        GAMs.Insert(element);
    }
    numberOfGAMs = GAMs.Size();
}

bool RealTimeThread::ConfigureArchitecture() {

    bool ret = true;

    if (!configured) {
        ReferenceContainerFilterReferences findme(1, ReferenceContainerFilterMode::PATH, this);
        ReferenceContainer path;
        ObjectRegistryDatabase::Instance()->ReferenceContainer::Find(path, findme);
        uint32 numberOfNodes = path.Size();
        StreamString absoluteFunctionPath;

        ReferenceT<RealTimeApplication> app;
        for (uint32 i = 0u; (i < numberOfNodes) && (ret) && (!app.IsValid()); i++) {
            Reference node = path.Get(i);
            ret = node.IsValid();
            if (ret) {
                absoluteFunctionPath += node->GetName();
                absoluteFunctionPath += ".";
                app = node;
            }
        }
        absoluteFunctionPath += "Functions.";

        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {

            StreamString functionPath = absoluteFunctionPath;
            functionPath += functions[i].Buffer();
            //functionPath = functions[i].Buffer();

            // find the functions specified in cdb
            /*lint -e{613} Never enters here if (functions == NULL) because (numberOfFunctions == 0) */
            Reference functionGeneric = ObjectRegistryDatabase::Instance()->Find(functionPath.Buffer()/*, this*/);
            ret = functionGeneric.IsValid();
            if (ret) {
                //Discombobulate GAMGroups
                ret = ConfigureArchitecturePrivate(functionGeneric);
            }
            else {
                /*lint -e{613} Never enter here if (functions == NULL) because (numberOfFunctions == 0) */
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Undefined %s", functions[i].Buffer())
            }
        }
        configured = true;
    }
    return ret;
}

bool RealTimeThread::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);

    AnyType functionsArray = data.GetType("Functions");
    if(ret){
        ret = (functionsArray.GetDataPointer() != NULL);
    }

    if (ret) {
        numberOfFunctions = functionsArray.GetNumberOfElements(0u);
        ret = (numberOfFunctions > 0);
        if (ret) {
            functions = new StreamString[numberOfFunctions];

            Vector<StreamString> functionVector(functions, numberOfFunctions);
            ret = (data.Read("Functions", functionVector));
        }
        if (ret) {
            if (!data.Read("CPUs", cpuMask)) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "No CPUs defined for the RealTimeThread %s", GetName())
            }
            if (!data.Read("StackSize", stackSize)) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "No StackSize defined for the RealTimeThread %s", GetName())
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

void RealTimeThread::GetGAMs(ReferenceContainer &gamList) {
    uint32 n;
    for (n = 0u; n < GAMs.Size(); n++) {
        gamList.Insert(GAMs.Get(n));
    }
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
    StreamString objNameToPrint = (IsDomain()) ? ("$") : ("+");
    objNameToPrint += objName;
    bool ret = data.CreateRelative(objNameToPrint.Buffer());
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
                        ret = child->ExportData(data);
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
