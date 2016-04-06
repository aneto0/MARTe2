/**
 * @file GAMSchedulerI.cpp
 * @brief Source file for class GAMSchedulerI
 * @date 22/03/2016
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
 * the class GAMSchedulerI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMSchedulerI.h"
#include "ConfigurationDatabase.h"
#include "RealTimeGenericDataDef.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMSchedulerI::GAMSchedulerI() {
    writer[0] = NULL_PTR(BasicRealTimeDataSourceOutputWriter*);
    writer[1] = NULL_PTR(BasicRealTimeDataSourceOutputWriter*);
    application = NULL_PTR(RealTimeApplication*);
}

bool GAMSchedulerI::InsertRecord(const char8 * stateName,
                                 ReferenceT<RealTimeThread> thread) {
    uint32 numberOfStates = Size();
    ReferenceT<GAMSchedulerRecord> record;
    bool found = false;
    for (uint32 i = 0u; (i < numberOfStates) && (!found); i++) {
        record = Get(i);
        if (record.IsValid()) {
            found = (StringHelper::Compare(stateName, record->GetName()) == 0);
        }
    }
    bool ret = true;

    // set the accelerator
    if (found) {
        record->AddThread(thread);
    }
    else {
        ReferenceT<GAMSchedulerRecord> newRecord = ReferenceT<GAMSchedulerRecord>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ret = newRecord.IsValid();
        if (ret) {
            newRecord->SetName(stateName);
            newRecord->AddThread(thread);
            ret = Insert(newRecord);
        }
    }

    return ret;
}

bool GAMSchedulerI::PrepareNextState(RealTimeStateInfo info) {
    uint32 numberOfStates = Size();
    StreamString newStateName = info.nextState;
    ReferenceT<GAMSchedulerRecord> record;
    bool ret = false;
    for (uint32 i = 0u; (i < numberOfStates) && (!ret); i++) {
        record = Get(i);
        if (record.IsValid()) {
            ret = (newStateName == record->GetName());
        }
    }
    // set the accelerator
    if (ret) {
        uint32 nextBuffer = (info.activeBuffer + 1u) % 2u;
        statesInExecution[nextBuffer] = record;

        // generate the output writer
        if (writer[nextBuffer] != NULL) {
            delete[] writer[nextBuffer];
        }
        uint32 numberOfThreads = record->GetNumberOfThreads();

        // creates a writer for each thread
        writer[nextBuffer] = new BasicRealTimeDataSourceOutputWriter[numberOfThreads];
        ret = (application != NULL);
        if (ret) {
            for (uint32 i = 0u; (i < numberOfThreads) && (ret); i++) {
                (writer[nextBuffer])[i].SetApplication(*application);
                ReferenceT<RealTimeThread> thread = record->Peek(i);
                ret = thread.IsValid();
                if (ret) {
                    uint32 numberOfGAMs = thread->GetNumberOfGAMs();
                    ReferenceT<GAMI> *gamArray = thread->GetGAMs();
                    // adds for each gam the relative and absolute time definitions to the
                    // specific writer.
                    for (uint32 j = 0u; (j < numberOfGAMs) && (ret); j++) {
                        ReferenceT<GAMI> gam = gamArray[j];
                        ret = gam.IsValid();
                        if (ret) {
                            ConfigurationDatabase defCDBAbs;
                            defCDBAbs.Write("Class", "RealTimeGenericDataDef");
                            defCDBAbs.Write("Type", "uint64");
                            defCDBAbs.Write("Default", "0");
                            defCDBAbs.Write("IsFinal", "true");
                            StreamString path = "GAM_Times.";
                            path += gam->GetName();
                            path += ".AbsoluteUsecTime";
                            defCDBAbs.Write("Path", path.Buffer());
                            defCDBAbs.MoveToRoot();

                            RealTimeGenericDataDef defAbs;
                            ret = defAbs.Initialise(defCDBAbs);
                            if (ret) {
                                ret = (writer[nextBuffer])[i].AddVariable(ReferenceT<RealTimeGenericDataDef>(&defAbs));
                            }
                            if (ret) {
                                ConfigurationDatabase defCDBRel;
                                defCDBRel.Write("Class", "RealTimeGenericDataDef");
                                defCDBRel.Write("Type", "uint64");
                                defCDBRel.Write("Default", "0");
                                defCDBRel.Write("IsFinal", "true");
                                path = "GAM_Times.";
                                path += gam->GetName();
                                path += ".RelativeUsecTime";
                                defCDBRel.Write("Path", path.Buffer());
                                defCDBRel.MoveToRoot();

                                RealTimeGenericDataDef defRel;
                                ret = defRel.Initialise(defCDBRel);
                                if (ret) {
                                    ret = (writer[nextBuffer])[i].AddVariable(ReferenceT<RealTimeGenericDataDef>(&defRel));
                                }
                            }
                        }
                    }
                    if (ret) {
                        ret = (writer[nextBuffer])[i].Finalise();
                    }
                }
                REPORT_ERROR(ErrorManagement::FatalError, "Invalid Thread in GAMSchedulerRecord");
            }
        }
        REPORT_ERROR(ErrorManagement::FatalError, "Application not set");
    }
    else{
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Next state %s not found", newStateName)
    }
    return ret;
}

void GAMSchedulerI::ChangeState(const uint32 activeBuffer) {
    uint32 nextBuffer = (activeBuffer + 1u) % 2u;

    StopExecution();
    StartExecution(nextBuffer);

}

void GAMSchedulerI::SetApplication(RealTimeApplication &rtApp) {
    application = &rtApp;
}

}
