/**
 * @file Threads.cpp
 * @brief Source file for module Threads
 * @date 27/07/2015
 * @author André Neto
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
 * the module Threads (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Threads.h"
#include "ThreadsDatabase.h"
#include "Scheduler.h"
#include "Task.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace Threads {

/**
 * @brief The function called when a thread is created.
 * @details Adds the thread in the database and calls the thread callback function.
 * When the callback finishes its execution this function removes the thread from the database.
 * @param[in,out] threadInfo the thread information structure.
 */
static ThreadIdentifier SystemThreadFunction(ThreadInformation * const threadInfo) {
    ThreadIdentifier id = 0u;
    if (threadInfo != NULL) {
        //Start the user thread
        Task *newTask= new Task();
        newTask->SetThreadInformation(*threadInfo);

        id=Scheduler::Get()->AddTask(newTask);
    }
    return id;
}

/**
 * @brief Builds a ThreadInformation structure with the thread informations.
 * @param[in] userThreadFunction is the thread function.
 * @param[in] userData is the thread function argument.
 * @param[in] threadName is the desired name of the thread.
 * @return the ThreadInformation structure.
 */
static ThreadInformation * threadInitialisationInterfaceConstructor(const ThreadFunctionType userThreadFunction,
                                                                    const void * const userData,
                                                                    const char8 * const threadName) {

    return NULL;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/*lint -e{715} Not implemented in Linux.*/
ThreadStateType GetState(const ThreadIdentifier &threadId) {
    return UnknownThreadStateType;
}

uint32 GetCPUs(const ThreadIdentifier &threadId) {
    return 0u;
}

ThreadIdentifier Id() {
    return Scheduler::Get()->Id(Scheduler::Get()->GetCurrentTask());
}

/*
 * In linux the priority will vary between 0, i.e. priorityClass = Unknown
 * and priorityLevel = 0 and 99, i.e. priorityClass = RealTime
 * and priorityLevel = 15
 */
void SetPriority(const ThreadIdentifier &threadId,
                 const Threads::PriorityClassType &priorityClass,
                 const uint8 &priorityLevel) {

}

uint8 GetPriorityLevel(const ThreadIdentifier &threadId) {

}

PriorityClassType GetPriorityClass(const ThreadIdentifier &threadId) {
    return UnknownPriorityClass;
}

/*
 * A signal is used to know if the other thread is alive.
 */
bool IsAlive(const ThreadIdentifier &threadId) {

    return Scheduler::Get()->IsAlive(threadId);
}

/*
 * Note that a thread cannot be deleted if it locks a mutex semaphore.
 */
bool Kill(const ThreadIdentifier &threadId) {
    Task* x = Scheduler::Get()->GetTask(threadId);
    bool ret = (x != NULL);
    if (ret) {
        Scheduler::Get()->RemoveTask(x);
    }
    return ret;
}

/*lint -e{715} the exceptionHandlerBehaviour implementation has not been agreed yet.*/
ThreadIdentifier BeginThread(const ThreadFunctionType function,
                             const void * const parameters,
                             const uint32 &stacksize,
                             const char8 * const name,
                             const uint32 exceptionHandlerBehaviour,
                             ProcessorType runOnCPUs) {

    ThreadInformation *threadInfo = threadInitialisationInterfaceConstructor(function, parameters, name);
    ThreadIdentifier id = InvalidThreadIdentifier;
    if (threadInfo != static_cast<ThreadInformation *>(NULL)) {
        //CStaticAssertErrorCondition(InitialisationError,"Threads::ThreadsBeginThread (%s) threadInitialisationInterfaceConstructor returns NULL", name);
        id=SystemThreadFunction(threadInfo);
    }

    return id;
}

void EndThread() {
}

const char8 *Name(const ThreadIdentifier &threadId) {

    return NULL;
}

ThreadIdentifier FindByIndex(const uint32 &n) {
    return InvalidThreadIdentifier;
}

uint32 NumberOfThreads() {
    return Scheduler::Get()->NumberOfThreads();

}

bool GetThreadInfoCopy(ThreadInformation &copy,
                       const uint32 &n) {
    return false;
}

uint32 GetThreadNumber(const ThreadIdentifier &threadId) {
    //TODO: Verify here if 1 is a suitable constant
    return 1u;
}

/*
 bool GetThreadInfoCopy(ThreadInformation &copy,
 const ThreadIdentifier &threadId) {
 return false;
 }*/

ThreadIdentifier FindByName(const char8 * const name) {
    return InvalidThreadIdentifier;
}

}

}
