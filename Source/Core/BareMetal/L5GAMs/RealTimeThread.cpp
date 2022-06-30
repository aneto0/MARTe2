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

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "GAM.h"
#include "GAMSchedulerI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeThread.h"
#include "ReferenceContainerFilterReferences.h"
#include "Vector.h"
#include "ReferenceContainerFilterReferencesTemplate.h"
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
bool RealTimeThread::ConfigureArchitecture() {

    bool ret = true;

    if (!configured) {
        ReferenceContainerFilterReferences findme(1, ReferenceContainerFilterMode::PATH, this);
        ReferenceContainer path;
        ObjectRegistryDatabase::Instance()->ReferenceContainer::Find(path, findme);
        uint32 numberOfNodes = path.Size();
        StreamString absoluteFunctionPath;

        ReferenceT<RealTimeApplication> app;
        ReferenceT<RealTimeState> state;
        bool isAppFound = false;
        bool isStateFound = false;
        {
            uint32 index = 0u;
            while ((index < numberOfNodes) && ret && (!isAppFound)) {
                Reference node = path.Get(index);
                ret = node.IsValid();
                if (ret) {
                    absoluteFunctionPath += node->GetName();
                    absoluteFunctionPath += ".";
                    app = node;
                }
                index++;
                isAppFound = app.IsValid();
            }
            while ((index < numberOfNodes) && (!isStateFound)) {
                state = path.Get(index);
                index++;
                isStateFound = state.IsValid();
            }
        }
        ret = isAppFound && isStateFound;
        absoluteFunctionPath += "Functions.";

        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {

            StreamString functionPath = absoluteFunctionPath;
            /*lint -e{613} Never NULL if (functions == NULL) because (numberOfFunctions == 0) */
            functionPath += functions[i].Buffer();

            // find the functions specified in cdb
            /*lint -e{613} Never enters here if (functions == NULL) because (numberOfFunctions == 0) */
            ReferenceT<ReferenceContainer> functionGeneric = ObjectRegistryDatabase::Instance()->Find(functionPath.Buffer());
            ret = functionGeneric.IsValid();
            if (ret) {
                ReferenceT<GAM> gam = functionGeneric;
                if (gam.IsValid()) {
                    ret = GAMs.Insert(gam);
                }
                if (ret) {
                    //Look for all the GAMs inside the RealTimeApplication
                    ReferenceContainerFilterReferencesTemplate<GAM> gamFilter(-1, ReferenceContainerFilterMode::RECURSIVE);

                    functionGeneric->Find(GAMs, gamFilter);
                    numberOfGAMs = GAMs.Size();
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Insert into GAMs failed", "");
                }
            }
            else {
                /*lint -e{613} Never enter here if (functions == NULL) because (numberOfFunctions == 0) */
                REPORT_ERROR(ErrorManagement::FatalError, "Undefined %s", functions[i].Buffer());
            }
            if (ret) {
                ReferenceContainer statefuls;
                ReferenceT<StatefulI> stateful = functionGeneric;
                if (stateful.IsValid()) {
                    ret = statefuls.Insert(stateful);
                }
                if (ret) {
                    //Look for all the statefuls
                    ReferenceContainerFilterReferencesTemplate<StatefulI> statefulFilter(-1, ReferenceContainerFilterMode::RECURSIVE);

                    functionGeneric->Find(statefuls, statefulFilter);
                    ret = state->AddStatefuls(statefuls);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::FatalError, "Insert into state failed", "");
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Insert into statefuls failed", "");
                }
            }
            else {
                /*lint -e{613} Never enter here if (functions == NULL) because (numberOfFunctions == 0) */
                REPORT_ERROR(ErrorManagement::FatalError, "Undefined %s", functions[i].Buffer());
            }

        }
        configured = true;
    }
    return ret;
}

bool RealTimeThread::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);

    AnyType functionsArray = data.GetType("Functions");
    if (ret) {
        ret = (functionsArray.GetDataPointer() != NULL);
    }

    if (ret) {
        numberOfFunctions = functionsArray.GetNumberOfElements(0u);
        ret = (numberOfFunctions > 0u);
    }
    if (ret) {
        functions = new StreamString[numberOfFunctions];

        Vector<StreamString> functionVector(functions, numberOfFunctions);
        ret = (data.Read("Functions", functionVector));
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "No functions defined for the RealTimeThread %s", GetName());
    }
    if (ret) {
        // TODO using better way to read/write BitSet
        uint64 cpuConfig = 0ull;
        if (!data.Read("CPUs", cpuConfig)) {
            REPORT_ERROR(ErrorManagement::Information, "No CPUs defined for the RealTimeThread %s", GetName());
        }
        if (!data.Read("StackSize", stackSize)) {
            REPORT_ERROR(ErrorManagement::Information, "No StackSize defined for the RealTimeThread %s", GetName());
        }
        BitSet cpuset(cpuConfig);
        cpuMask = ProcessorType(cpuset);
    }

    return ret;

}

StreamString * RealTimeThread::GetFunctions() {
    return functions;
}

uint32 RealTimeThread::GetNumberOfFunctions() const {
    return numberOfFunctions;
}

bool RealTimeThread::GetGAMs(ReferenceContainer &gamList) {
    bool ret = true;
    for (uint32 n = 0u; (n < GAMs.Size()) && ret; n++) {
        ret = gamList.Insert(GAMs.Get(n));
    }
    return ret;
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
    }
    if (ret) {
        if (functions != NULL) {
            Vector<StreamString> functionVector(functions, numberOfFunctions);
            ret = data.Write("Functions", functionVector);
        }
    }
    if (ret) {
        if (numberOfGAMs > 0u) {
            StreamString *gamList = new StreamString[numberOfGAMs];
            uint32 n;
            for (n = 0u; n < numberOfGAMs; n++) {
                gamList[n] = GAMs.Get(n)->GetName();
            }
            Vector<StreamString> gamsVector(gamList, numberOfGAMs);
            ret = data.Write("GAMs", gamsVector);
            delete[] gamList;
        }
    }
    if (ret) {
        uint32 numberOfChildren = Size();
        for (uint32 i = 0u; i < numberOfChildren; i++) {
            Reference child = Get(i);
            ret = child.IsValid();
            if (ret) {
                ret = child->ExportData(data);
            }
        }
    }
    if (!data.MoveToAncestor(1u)) {
        ret = false;
    }

    return ret;
}

void RealTimeThread::Purge(ReferenceContainer &purgeList) {
    GAMs.Purge(purgeList);
    ReferenceContainer::Purge(purgeList);
}

CLASS_REGISTER(RealTimeThread, "1.0")

}
