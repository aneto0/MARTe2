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
#include "GAMGenericSignal.h"
#include "AdvancedErrorManagement.h"
#include "DataSource.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMSchedulerI::GAMSchedulerI() {
    writer[0] = NULL_PTR(MemoryMapOutputWriter*);
    writer[1] = NULL_PTR(MemoryMapOutputWriter*);
    application = NULL_PTR(RealTimeApplication*);
}

GAMSchedulerI::~GAMSchedulerI() {
    if (writer[0] != NULL) {
        delete[] writer[0];
    }
    if (writer[1] != NULL) {
        delete[] writer[1];
    }
}

bool GAMSchedulerI::InsertRecord(const char8 * stateName,
                                 ReferenceT<RealTimeThread> thread) {
    uint32 numberOfStates = Size();
    ReferenceT < GAMSchedulerRecord > record;
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
        ReferenceT < GAMSchedulerRecord > newRecord = ReferenceT < GAMSchedulerRecord > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
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
    ReferenceT < GAMSchedulerRecord > record;
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

        //printf("\nnBuff=%d \n", nextBuffer);
        statesInExecution[nextBuffer] = record;

        // generate the output writer
        if (writer[nextBuffer] != NULL) {
            delete[] writer[nextBuffer];
        }
        uint32 numberOfThreads = record->GetNumberOfThreads();

        // creates a writer for each thread
        writer[nextBuffer] = new MemoryMapOutputWriter[numberOfThreads];

        ret = (application != NULL);
        if (ret) {
            for (uint32 i = 0u; (i < numberOfThreads) && (ret); i++) {
                const char8 *dsPath = "Data.GAM_Times";
                ReferenceT < DataSource > timesDS = application->Find(dsPath);
                ret = (timesDS.IsValid());
                if (ret) {
                    (writer[nextBuffer])[i].SetApplication(*application);
                    ReferenceT < RealTimeThread > thread = record->Peek(i);
                    ret = thread.IsValid();
                    if (ret) {

                        uint32 numberOfGAMs = thread->GetNumberOfGAMs();
                        ReferenceT < GAM > *gamArray = thread->GetGAMs();
                        // adds for each gam the relative and absolute time definitions to the
                        // specific writer.
                        for (uint32 j = 0u; (j < numberOfGAMs) && (ret); j++) {
                            ReferenceT < GAM > gam = gamArray[j];
                            ret = gam.IsValid();
                            if (ret) {

                                ConfigurationDatabase defCDBAbs;
                                defCDBAbs.Write("Class", "GAMGenericSignal");
                                defCDBAbs.Write("Type", "uint64");
                                defCDBAbs.Write("Default", "0");
                                defCDBAbs.Write("IsFinal", "true");
                                StreamString path = "GAM_Times.";
                                path += gam->GetName();
                                path += ".AbsoluteUsecTime";
                                defCDBAbs.Write("Path", path.Buffer());
                                defCDBAbs.MoveToRoot();

                                ReferenceT < GAMGenericSignal > defAbs = ReferenceT < GAMGenericSignal > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
                                ret = defAbs->Initialise(defCDBAbs);
                                if (ret) {
                                    ret = (writer[nextBuffer])[i].AddSignal(defAbs);
                                }
                                if (ret) {
                                    ConfigurationDatabase defCDBRel;
                                    defCDBRel.Write("Class", "GAMGenericSignal");
                                    defCDBRel.Write("Type", "uint64");
                                    defCDBRel.Write("Default", "0");
                                    defCDBRel.Write("IsFinal", "true");
                                    path = "GAM_Times.";
                                    path += gam->GetName();
                                    path += ".RelativeUsecTime";
                                    defCDBRel.Write("Path", path.Buffer());
                                    defCDBRel.MoveToRoot();

                                    ReferenceT < GAMGenericSignal > defRel = ReferenceT < GAMGenericSignal
                                            > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
                                    ret = defRel->Initialise(defCDBRel);
                                    if (ret) {
                                        ret = (writer[nextBuffer])[i].AddSignal(defRel);
                                    }
                                }
                            }
                        }
                        if (ret) {
                            ret = (writer[nextBuffer])[i].Finalise();
                            uint32 numberOfSignals = (writer[nextBuffer])[i].GetNumberOfSignals();
                            for (uint32 n = 0u; n < numberOfSignals; n++) {
                                *(reinterpret_cast<uint64*>((writer[nextBuffer])[i].GetSignal(n))) = 0ull;
                            }
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "Invalid Thread in GAMSchedulerRecord");
                    }
                }
                else {
                    //TODO times data source not created
                }
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Application not set");
        }
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Next state %s not found", newStateName)
    }
    return ret;
}

void GAMSchedulerI::ChangeState(const uint32 activeBuffer) {
    uint32 nextBuffer = (activeBuffer + 1u) % 2u;

    StopExecution();
    StartExecution(nextBuffer);

}

void GAMSchedulerI::ExecuteSingleCycle(const uint32 threadId,
                                       const uint32 activeBuffer) {
    // warning: possible segmentation faults if the previous operations
    // lack or fail and the pointers are invalid.

    //printf("Executing thread %d", threadId);
    ReferenceT < RealTimeThread > thread = statesInExecution[activeBuffer]->Peek(threadId);
    if (thread.IsValid()) {
        // resets the flag
        ReferenceT < GAM > *gamArray = thread->GetGAMs();
        uint32 numberOfGAMs = thread->GetNumberOfGAMs();
        uint64 absTic = HighResolutionTimer::Counter();
        for (uint32 i = 0u; i < numberOfGAMs; i++) {
            // save the time before
            uint64 relTic = HighResolutionTimer::Counter();
            // execute the gam
            gamArray[i]->Execute(activeBuffer);
            // writes the time stamps
            // 2*i because for each gam we have absolute and relative variable inserted
            uint64 * relTime = reinterpret_cast<uint64 *>((writer[activeBuffer])[threadId].GetSignal((2 * i) + 1u));
            *relTime = static_cast<uint64>(HighResolutionTimer::TicksToTime(HighResolutionTimer::Counter(), relTic) * 1e6);
            uint64 * absTime = reinterpret_cast<uint64 *>((writer[activeBuffer])[threadId].GetSignal(2 * i));
            *absTime = static_cast<uint64>(HighResolutionTimer::TicksToTime(HighResolutionTimer::Counter(), absTic) * 1e6);
            (writer[activeBuffer])[threadId].Write(activeBuffer);

        }
    }
}

void GAMSchedulerI::SetApplication(RealTimeApplication &rtApp) {
    application = &rtApp;
}

}
