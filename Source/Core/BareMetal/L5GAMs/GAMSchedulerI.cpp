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
#include "AdvancedErrorManagement.h"
#include "DataSourceI.h"
#include "GAM.h"
#include "ReferenceContainerFilterReferences.h"
#include "RealTimeThread.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMSchedulerI::GAMSchedulerI() {
    states = NULL_PTR(ScheduledState *);
    statesInExecution[0] = NULL_PTR(ScheduledState *);
    statesInExecution[1] = NULL_PTR(ScheduledState *);
    numberOfStates = 0u;

}

GAMSchedulerI::~GAMSchedulerI() {
    if (states != NULL) {
        if(states->threads!=NULL) {
            if(states->threads->executables!=NULL) {
                delete [] states->threads->executables;
            }
            if(states->threads->timeAddresses!=NULL) {
                delete [] states->threads->timeAddresses;
            }
            delete[] states->threads;
        }
        delete [] states;
    }
}

bool GAMSchedulerI::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        timeDsAddress = "Data.";
        ret = data.Read("TimingDataSource", timeDsAddress);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please specify the TimingDataSource address");
        }
    }
    return ret;
}

bool GAMSchedulerI::ConfigureScheduler() {

    /*
     * struct ScheduledState (array) {
     *     Struct ScheduledThread (array) {
     *         Struct ScheduledExecutable (array){
     *             ExecutableI
     *             bool SumOrReset
     *             void* TimeAddress
     *         }
     *         name
     *         numberOfExecutables
     *     }
     *     name
     *     numberOfThreads
     * }
     */

    ReferenceT<RealTimeApplication> app;

    ReferenceContainerFilterReferences findme(1, ReferenceContainerFilterMode::PATH, this);
    ReferenceContainer path;
    ObjectRegistryDatabase::Instance()->ReferenceContainer::Find(path, findme);
    uint32 numberOfNodes = path.Size();

    for (uint32 i = 0u; (i < numberOfNodes) && (!app.IsValid()); i++) {
        app = path.Get(i);
    }
    bool ret = app.IsValid();

    ReferenceT<ReferenceContainer> statesContainer;
    if (ret) {
        statesContainer = app->Find("States");
        ret = statesContainer.IsValid();
    }

    if (ret) {
        timeDataSource = app->Find(timeDsAddress.Buffer());
        ret = timeDataSource.IsValid();
        if (!ret) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "TimingDataSource %s not found", timeDsAddress.Buffer())
        }
    }

    if (ret) {
        numberOfStates = statesContainer->Size();
        states = new ScheduledState[numberOfStates];
        for (uint32 i = 0u; i < numberOfStates && ret; i++) {
            ReferenceT<RealTimeState> stateElement = statesContainer->Get(i);
            ret = stateElement.IsValid();
            if (ret) {
                ReferenceT<ReferenceContainer> threadContainer = stateElement->Find("Threads");
                ret = threadContainer.IsValid();
                if (ret) {
                    uint32 numberOfThreads = threadContainer->Size();

                    states[i].numberOfThreads = numberOfThreads;
                    states[i].name = stateElement->GetName();

                    states[i].threads = new ScheduledThread[numberOfThreads];

                    for (uint32 j = 0u; j < numberOfThreads && ret; j++) {
                        ReferenceT<RealTimeThread> threadElement = threadContainer->Get(j);
                        ret = threadElement.IsValid();
                        if (ret) {

                            ReferenceContainer gams;
                            threadElement->GetGAMs(gams);
                            uint32 numberOfGams = threadElement->GetNumberOfGAMs();
                            uint32 numberOfExecutables = numberOfGams;

                            for (uint32 k = 0u; k < numberOfGams && ret; k++) {
                                ReferenceT<GAM> gam = gams.Get(k);
                                ret = gam.IsValid();
                                if (ret) {
                                    ReferenceT<ReferenceContainer> ib = gam->GetInputBrokers();
                                    ret = ib.IsValid();
                                    if (ret) {
                                        numberOfExecutables += ib->Size();

                                    }
                                    if (ret) {
                                        ReferenceT<ReferenceContainer> ob = gam->GetOutputBrokers();
                                        ret = ob.IsValid();
                                        if (ret) {
                                            numberOfExecutables += ob->Size();
                                        }
                                    }
                                }
                            }
                            if (ret) {
                                states[i].threads[j].executables = new ExecutableI*[numberOfExecutables];
                                states[i].threads[j].timeAddresses = new uint32*[numberOfExecutables];

                                states[i].threads[j].numberOfExecutables = numberOfExecutables;
                                states[i].threads[j].name = threadElement->GetName();
                            }
                            uint32 c = 0u;
                            for (uint32 k = 0u; k < numberOfGams && ret; k++) {
                                //add input brokers
                                StreamString gamFullName = threadElement->GetFunctions()[k];
                                ret = InsertInputBrokers(gams.Get(k), gamFullName.Buffer(), i, j, c);
                                //add gam
                                if (ret) {
                                    ret = InsertGam(gams.Get(k), gamFullName.Buffer(), i, j, c);
                                }

                                //add output brokers
                                if (ret) {
                                    ret = InsertOutputBrokers(gams.Get(k), gamFullName.Buffer(), i, j, c);
                                }
                            }

                            //Add the cycle time
                            if (ret) {
                                StreamString threadFullName = states[i].name;
                                threadFullName += ".";
                                threadFullName += states[i].threads[j].name;
                                threadFullName += "_CycleTime";
                                uint32 signalIdx;
                                ret = timeDataSource->GetSignalIndex(signalIdx, threadFullName.Buffer());
                                if (ret) {
                                    ret = timeDataSource->GetSignalMemoryBuffer(signalIdx, 0u, reinterpret_cast<void*&>(states[i].threads[j].cycleTime));
                                    printf("\nInserted CycleTime\n");
                                }
                            }
                        }
                    }
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "Invalid RealTimeState in states container");
            }
        }
    }

    return ret;
}

bool GAMSchedulerI::InsertInputBrokers(ReferenceT<GAM> gam,
                                       const char8 * gamFullName,
                                       uint32 i,
                                       uint32 j,
                                       uint32 &index) {
    ReferenceT<ReferenceContainer> inputBrokers = gam->GetInputBrokers();
    uint32 numberOfIB = inputBrokers->Size();
    //add input brokers
    StreamString timeSignalName = gamFullName;
    timeSignalName += "_ReadTime";
    uint32 signalIdx;
    bool ret = timeDataSource->GetSignalIndex(signalIdx, timeSignalName.Buffer());
    void* signalAddress = NULL_PTR(void*);
    if (ret) {
        ret = timeDataSource->GetSignalMemoryBuffer(signalIdx, 0u, signalAddress);
    }
    if (ret) {
        ret = (signalAddress != NULL);
    }
    for (uint32 n = 0u; n < numberOfIB && ret; n++) {
        ReferenceT<ExecutableI> input = inputBrokers->Get(n);
        ret = input.IsValid();
        if (ret) {
            states[i].threads[j].executables[index] = input.operator->();
            states[i].threads[j].timeAddresses[index] = reinterpret_cast<uint32*>(signalAddress);

            index++;

        }

    }
    return ret;
}

bool GAMSchedulerI::InsertGam(ReferenceT<GAM> gam,
                              const char8 * gamFullName,
                              uint32 i,
                              uint32 j,
                              uint32 &index) {

    StreamString timeSignalName = gamFullName;
    timeSignalName += "_ExecTime";

    uint32 signalIdx;
    bool ret = timeDataSource->GetSignalIndex(signalIdx, timeSignalName.Buffer());
    void* signalAddress = NULL_PTR(void*);
    if (ret) {
        ret = timeDataSource->GetSignalMemoryBuffer(signalIdx, 0u, signalAddress);
    }
    if (ret) {
        ret = signalAddress != NULL;
    }
    states[i].threads[j].executables[index] = gam.operator->();
    states[i].threads[j].timeAddresses[index] = reinterpret_cast<uint32*>(signalAddress);
    index++;
    return ret;
}

bool GAMSchedulerI::InsertOutputBrokers(ReferenceT<GAM> gam,
                                        const char8 * gamFullName,
                                        uint32 i,
                                        uint32 j,
                                        uint32 &index) {
    StreamString timeSignalName = gamFullName;
    timeSignalName += "_WriteTime";
    uint32 signalIdx;
    bool ret = timeDataSource->GetSignalIndex(signalIdx, timeSignalName.Buffer());
    void* signalAddress = NULL_PTR(void*);
    if (ret) {
        ret = timeDataSource->GetSignalMemoryBuffer(signalIdx, 0u, signalAddress);
    }
    if (ret) {
        ret = signalAddress != NULL;
    }

    ReferenceT<ReferenceContainer> outputBrokers = gam->GetOutputBrokers();
    uint32 numberOfOB = outputBrokers->Size();
    for (uint32 n = 0u; n < numberOfOB && ret; n++) {
        ReferenceT<ExecutableI> output = outputBrokers->Get(n);
        ret = output.IsValid();
        if (ret) {
            states[i].threads[j].executables[index] = output.operator->();
            states[i].threads[j].timeAddresses[index] = reinterpret_cast<uint32*>(signalAddress);
            index++;
        }
    }
    return ret;
}

bool GAMSchedulerI::PrepareNextState(const char8 * const currentStateName,
                                     const char8 * const nextStateName) {

    // Find the next state and prepare the pointer to
    uint32 nextBuffer = (RealTimeApplication::GetIndex() + 1u) % 2u;

    bool ret = false;
    for (uint32 i = 0u; i < numberOfStates && !ret; i++) {
        ret = (StringHelper::Compare(nextStateName, states[i].name) == 0);
        if (ret) {
            statesInExecution[nextBuffer] = &states[i];
        }
    }

    return ret;
}

uint64 GAMSchedulerI::ExecuteSingleCycle(ExecutableI** executables,
                                         uint32 **timeAddress,
                                         uint32 numberOfExecutables) {
    // warning: possible segmentation faults if the previous operations
    // lack or fail and the pointers are invalid.

    uint64 absTic = HighResolutionTimer::Counter();
    for (uint32 i = 0u; i < numberOfExecutables; i++) {
        // save the time before
        // execute the gam
        executables[i]->Execute();
        uint32 absTime = static_cast<uint32>(HighResolutionTimer::TicksToTime(HighResolutionTimer::Counter(), absTic) * 10e6);        //us
        MemoryOperationsHelper::Copy(timeAddress[i], &absTime, sizeof(uint32));
        executables[i]->SetLastExecutionTime(absTime);
    }
    return absTic;
}

uint32 GAMSchedulerI::GetNumberOfExecutables(const char8* stateName,
                                             const char8 *threadName) {
    uint32 numberOfExecutables = 0u;
    if (states != NULL) {
        bool foundState= false;
        for(uint32 i=0u;i<numberOfStates && !foundState; i++) {
            foundState = StringHelper::Compare(stateName, states[i].name)==0;
            if(foundState) {
                uint32 numberOfThreads= states[i].numberOfThreads;
                if(states[i].threads!=NULL) {
                    bool foundThread=false;
                    for(uint32 j=0u; j<numberOfThreads && !foundThread; j++) {

                        foundThread=StringHelper::Compare(threadName, states[i].threads[j].name)==0;
                        if(foundThread) {
                            numberOfExecutables=states[i].threads[j].numberOfExecutables;
                        }
                    }
                }
            }
        }

    }
    return numberOfExecutables;
}
}

