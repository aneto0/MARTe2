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

#include "AdvancedErrorManagement.h"
#include "BrokerI.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAM.h"
#include "GAMSchedulerI.h"
#include "RealTimeApplication.h"
#include "RealTimeThread.h"
#include "ReferenceContainerFilterReferences.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMSchedulerI::GAMSchedulerI() :
        ReferenceContainer(),
        clockPeriod(HighResolutionTimer::Period()) {
    states = NULL_PTR(ScheduledState *);
    scheduledStates[0] = NULL_PTR(ScheduledState *);
    scheduledStates[1] = NULL_PTR(ScheduledState *);
    numberOfStates = 0u;
    currentStateIdentifier = NULL_PTR(uint32 *);
    nextStateIdentifier = 0u;
}

/*lint -e{1740} currentStateIdentifier is a pointer to a memory block allocated elsewhere*/
GAMSchedulerI::~GAMSchedulerI() {
    if (states != NULL) {
        if (numberOfStates > 0u) {
            uint32 s;
            for (s=0u; s<numberOfStates; s++) {
                uint32 numberOfThreads = states[s].numberOfThreads;
                if (states[s].threads != NULL_PTR(ScheduledThread *)) {
                    uint32 t;
                    for (t=0u; t<numberOfThreads; t++) {
                        if (states[s].threads[t].executables != NULL_PTR(ExecutableI **)) {
                            delete [] states[s].threads[t].executables;
                        }
                    }
                    delete [] states[s].threads;
                }
            }
        }
        delete[] states;
        states = NULL_PTR(ScheduledState *);
    }
}

bool GAMSchedulerI::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        timingDataSourceAddress = "Data.";
        ret = data.Read("TimingDataSource", timingDataSourceAddress);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please specify the TimingDataSource address");
        }
    }

    return ret;
}

bool GAMSchedulerI::ConfigureScheduler(Reference realTimeAppIn) {
    realTimeApp = realTimeAppIn;
    ReferenceT<RealTimeApplication> rtApp = realTimeApp;
    bool ret = rtApp.IsValid();

    ReferenceT<ReferenceContainer> statesContainer;
    if (ret) {
        statesContainer = rtApp->Find("States");
        ret = statesContainer.IsValid();
    }

    if (ret) {
        timingDataSource = rtApp->Find(timingDataSourceAddress.Buffer());
        ret = timingDataSource.IsValid();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "TimingDataSource %s not found", timingDataSourceAddress.Buffer());
        }
    }

    if (ret) {
        numberOfStates = statesContainer->Size();
        states = new ScheduledState[numberOfStates];
        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
            states[i].numberOfThreads = 0u;
            states[i].threads = NULL_PTR(ScheduledThread *);
        }
        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
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

                    for (uint32 j = 0u; (j < numberOfThreads) && (ret); j++) {
                        ReferenceT<RealTimeThread> threadElement = threadContainer->Get(j);
                        ret = threadElement.IsValid();
                        if (ret) {

                            ReferenceContainer gams;
                            ret = threadElement->GetGAMs(gams);
                            uint32 numberOfGams = threadElement->GetNumberOfGAMs();
                            uint32 numberOfExecutables = numberOfGams;

                            for (uint32 k = 0u; (k < numberOfGams) && (ret); k++) {
                                ReferenceT<GAM> gam = gams.Get(k);
                                ret = gam.IsValid();
                                if (ret) {
                                    ReferenceContainer inputBrokers;
                                    ret = gam->GetInputBrokers(inputBrokers);
                                    if (ret) {
                                        numberOfExecutables += inputBrokers.Size();

                                    }
                                    if (ret) {
                                        ReferenceContainer outputBrokers;
                                        ret = gam->GetOutputBrokers(outputBrokers);
                                        if (ret) {
                                            numberOfExecutables += outputBrokers.Size();
                                        }
                                    }
                                }
                            }
                            if (ret) {
                                states[i].threads[j].executables = new ExecutableI*[numberOfExecutables];

                                states[i].threads[j].numberOfExecutables = numberOfExecutables;
                                states[i].threads[j].name = threadElement->GetName();
                                states[i].threads[j].cpu = threadElement->GetCPU();
                                states[i].threads[j].stackSize = threadElement->GetStackSize();
                            }
                            uint32 c = 0u;
                            for (uint32 k = 0u; (k < numberOfGams) && (ret); k++) {
                                //add input brokers
                                StreamString gamFullName;
                                ReferenceT<GAM> gam = gams.Get(k);
                                ret = gam->GetQualifiedName(gamFullName);
                                if (ret) {
                                    ret = InsertInputBrokers(gam, gamFullName.Buffer(), i, j, c);
                                }
                                //add gam
                                if (ret) {
                                    ret = InsertGAM(gam, gamFullName.Buffer(), i, j, c);
                                    if (ret) {
                                        c++;
                                    }
                                }

                                //add output brokers
                                if (ret) {
                                    ret = InsertOutputBrokers(gam, gamFullName.Buffer(), i, j, c);
                                }
                            }

                            //Add the cycle time
                            if (ret) {
                                StreamString threadFullName = states[i].name;
                                threadFullName += ".";
                                threadFullName += states[i].threads[j].name;
                                threadFullName += "_CycleTime";
                                uint32 signalIdx;
                                ret = timingDataSource->GetSignalIndex(signalIdx, threadFullName.Buffer());
                                if (ret) {
                                    ret = timingDataSource->GetSignalMemoryBuffer(signalIdx, 0u, reinterpret_cast<void*&>(states[i].threads[j].cycleTime));
                                }
                            }

                            //Get the current state identifier
                            if(ret) {
                                uint32 signalIdx;
                                ret = timingDataSource->GetSignalIndex(signalIdx, "CurrentState");
                                if (ret) {
                                    ret = timingDataSource->GetSignalMemoryBuffer(signalIdx, 0u, reinterpret_cast<void*&>(currentStateIdentifier));
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
                                       const char8 * const gamFullName,
                                       const uint32 stateIdx,
                                       const uint32 threadIdx,
                                       uint32 &executableIdx) const{

    //add input brokers
    StreamString timeSignalName = gamFullName;
    timeSignalName += "_ReadTime";
    uint32 signalIdx;
    bool ret = timingDataSource->GetSignalIndex(signalIdx, timeSignalName.Buffer());
    void* signalAddress = NULL_PTR(void*);
    if (ret) {
        ret = timingDataSource->GetSignalMemoryBuffer(signalIdx, 0u, signalAddress);
    }
    if (ret) {
        ret = (signalAddress != NULL);
    }
    if (ret) {
        ret = (states != NULL_PTR(ScheduledState *));
    }
    ReferenceContainer inputBrokers;
    if (ret) {
        ret = gam->GetInputBrokers(inputBrokers);
    }
    uint32 numberOfInputBrokers = 0u;
    if (ret) {
        numberOfInputBrokers = inputBrokers.Size();
    }
    for (uint32 n = 0u; (n < numberOfInputBrokers) && (ret); n++) {
        ReferenceT<ExecutableI> input = inputBrokers.Get(n);
        ret = input.IsValid();
        if (ret) {
            //lint -e{613} states != NULL checked before entering here.
            ret = (states[stateIdx].threads != NULL_PTR(ScheduledThread *));
        }
        if (ret) {
            //lint -e{613} states != NULL checked before entering here.
            states[stateIdx].threads[threadIdx].executables[executableIdx] = input.operator->();
            //lint -e{613} states != NULL checked before entering here.
            states[stateIdx].threads[threadIdx].executables[executableIdx]->SetTimingSignalAddress(reinterpret_cast<uint32*>(signalAddress));
        }
        executableIdx++;
    }

    return ret;
}

bool GAMSchedulerI::InsertGAM(ReferenceT<GAM> gam,
                              const char8 * const gamFullName,
                              const uint32 stateIdx,
                              const uint32 threadIdx,
                              const uint32 executableIdx) const {

    StreamString timeSignalName = gamFullName;
    timeSignalName += "_ExecTime";

    uint32 signalIdx;
    bool ret = timingDataSource->GetSignalIndex(signalIdx, timeSignalName.Buffer());
    void* signalAddress = NULL_PTR(void*);
    if (ret) {
        ret = timingDataSource->GetSignalMemoryBuffer(signalIdx, 0u, signalAddress);
    }
    if (ret) {
        ret = (signalAddress != NULL);
    }
    if (ret) {
        ret = (states != NULL_PTR(ScheduledState *));
    }
    if (ret) {
        //lint -e{613} states != NULL checked before entering here.
        ret = (states[stateIdx].threads != NULL_PTR(ScheduledThread *));
    }
    if (ret) {
        //lint -e{613} states != NULL checked before entering here.
        states[stateIdx].threads[threadIdx].executables[executableIdx] = gam.operator->();
        //lint -e{613} states != NULL checked before entering here.
        states[stateIdx].threads[threadIdx].executables[executableIdx]->SetTimingSignalAddress(reinterpret_cast<uint32*>(signalAddress));
    }
    return ret;
}

bool GAMSchedulerI::InsertOutputBrokers(ReferenceT<GAM> gam,
                                        const char8 * const gamFullName,
                                        const uint32 stateIdx,
                                        const uint32 threadIdx,
                                        uint32 &executableIdx) const {
    StreamString timeSignalName = gamFullName;
    timeSignalName += "_WriteTime";
    uint32 signalIdx;
    bool ret = timingDataSource->GetSignalIndex(signalIdx, timeSignalName.Buffer());
    void* signalAddress = NULL_PTR(void*);
    if (ret) {
        ret = timingDataSource->GetSignalMemoryBuffer(signalIdx, 0u, signalAddress);
    }
    if (ret) {
        ret = signalAddress != NULL;
    }
    if (ret) {
        ret = (states != NULL_PTR(ScheduledState *));
    }
    if (ret) {
        //lint -e{613} states != NULL checked before entering here.
        ret = (states[stateIdx].threads != NULL_PTR(ScheduledThread *));
    }
    ReferenceContainer outputBrokers;
    if (ret) {
        ret = gam->GetOutputBrokers(outputBrokers);
    }
    uint32 numberOfOutputBrokers = 0u;
    if (ret) {
        numberOfOutputBrokers = outputBrokers.Size();
    }
    for (uint32 n = 0u; (n < numberOfOutputBrokers) && (ret); n++) {
        ReferenceT<ExecutableI> output = outputBrokers.Get(n);
        ret = output.IsValid();
        if (ret) {
            //lint -e{613} states != NULL checked before entering here.
            states[stateIdx].threads[threadIdx].executables[executableIdx] = output.operator->();
            //lint -e{613} states != NULL checked before entering here.
            states[stateIdx].threads[threadIdx].executables[executableIdx]->SetTimingSignalAddress(reinterpret_cast<uint32*>(signalAddress));
        }
        executableIdx++;
    }
    return ret;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The GAMSchedulerI implementation does not need
 to know the currentStateName (but other implementations of the GAMSchedulerI might need to know).*/
bool GAMSchedulerI::PrepareNextState(const char8 * const currentStateName,
                                     const char8 * const nextStateName) {

    // Find the next state and prepare the pointer to
    bool ret = (states != NULL_PTR(ScheduledState *));

    uint32 nextBuffer = 0u;
    if (ret) {
        ReferenceT<RealTimeApplication> rtApp = realTimeApp;
        if (rtApp.IsValid()) {
            nextBuffer = (rtApp->GetIndex() + 1u) % 2u;
        }
    }

    bool found = false;
    nextStateIdentifier = numberOfStates;
    for (uint32 i = 0u; (i < numberOfStates) && (ret) && (!found); i++) {
        //lint -e{613} states != NULL checked before entering here.
        found = (StringHelper::Compare(nextStateName, states[i].name) == 0);
        if (found) {
            //lint -e{613} states != NULL checked before entering here.
            scheduledStates[nextBuffer] = &states[i];
            nextStateIdentifier = i;
        }
    }
    if (ret) {
        ret = found;
    }
    if (ret) {
        CustomPrepareNextState();
    }

    return ret;
}

bool GAMSchedulerI::ExecuteSingleCycle(ExecutableI * const * const executables,
                                       const uint32 numberOfExecutables) const {
    // warning: possible segmentation faults if the previous operations
    // lack or fail and the pointers are invalid.

    bool ret = true;
    uint64 absTicks = HighResolutionTimer::Counter();
    for (uint32 i = 0u; (i < numberOfExecutables) && (ret); i++) {
        // save the time before
        // execute the gam/broker
        if (executables[i]->IsEnabled()) {
            ret = executables[i]->Execute();
        }

        uint64 tmp = (HighResolutionTimer::Counter() - absTicks);
        float64 ticksToTime = (static_cast<float64>(tmp) * clockPeriod) * 1e6;
        uint32 absTime = static_cast<uint32>(ticksToTime);  //us
        if (ret) {
            uint32 sizeToCopy = static_cast<uint32>(sizeof(uint32));
            ret = MemoryOperationsHelper::Copy(executables[i]->GetTimingSignalAddress(), &absTime, sizeToCopy);
        }
        else {
            BrokerI *broker = dynamic_cast<BrokerI *>(executables[i]);
            if (broker != NULL_PTR(BrokerI *)) {
                StreamString ownerFunctionName = broker->GetOwnerFunctionName();
                StreamString ownerDataSourceName = broker->GetOwnerDataSourceName();
                StreamString brokerName = "unnamed";
                if (broker->GetName() != NULL_PTR(const char8 * const)) {
                    brokerName = broker->GetName();
                }
                REPORT_ERROR (ErrorManagement::Warning, "BrokerI %s failed, owner function: %s, owner DataSource: %s", brokerName.Buffer(), ownerFunctionName.Buffer(), ownerDataSourceName.Buffer());
            }
            else {
                Object *obj = dynamic_cast<Object *>(executables[i]);
                if (obj != NULL_PTR(Object *)) {
                    REPORT_ERROR (ErrorManagement::Warning, "ExecutableI %s failed", obj->GetName());
                }
            }
        }
    }

    return ret;
}

uint32 GAMSchedulerI::GetNumberOfExecutables(const char8 * const stateName,
                                             const char8 * const threadName) const {
    uint32 numberOfExecutables = 0u;
    if (states != NULL) {
        bool foundState = false;
        for (uint32 i = 0u; (i < numberOfStates) && (!foundState); i++) {
            foundState = StringHelper::Compare(stateName, states[i].name) == 0;
            if (foundState) {
                uint32 numberOfThreads = states[i].numberOfThreads;
                if (states[i].threads != NULL) {
                    bool foundThread = false;
                    for (uint32 j = 0u; (j < numberOfThreads) && (!foundThread); j++) {

                        foundThread = StringHelper::Compare(threadName, states[i].threads[j].name) == 0;
                        if (foundThread) {
                            numberOfExecutables = states[i].threads[j].numberOfExecutables;
                        }
                    }
                }
            }
        }

    }
    return numberOfExecutables;
}

ScheduledState * const * GAMSchedulerI::GetSchedulableStates() {
    return scheduledStates;

}

void GAMSchedulerI::Purge(ReferenceContainer &purgeList) {
    if (timingDataSource.IsValid()) {
        timingDataSource->Purge(purgeList);
    }
    ReferenceContainer::Purge(purgeList);
}

}

