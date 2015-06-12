/**
 * @file Threads.cpp
 * @brief Header file for class Threads
 * @date 09/06/2015
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
 *
 * @details This header file contains the declaration of the class Threads
 * (all of its public, protected and private members). It may also include
 * definitions for inline and friend methods which need to be visible to
 * the compiler.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Threads.h"
#include "ThreadsDatabase.h"

#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,ThreadsOS.h)

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,ThreadsOS.h)

TID ThreadsBeginThread(ThreadFunctionType function, void *parameters,
        uint32 stacksize, const char *name,
        uint32 exceptionHandlerBehaviour,
        ProcessorType runOnCPUs) {

    return ThreadsOS::BeginThread(
            function, parameters,
            stacksize, name, exceptionHandlerBehaviour, runOnCPUs);

}

TID ThreadsId() {
    return ThreadsOS::Id();
}

bool ThreadsIsAlive(TID threadId) {

    return ThreadsOS::IsAlive(threadId);

}

bool ThreadsKill(TID threadId) {

    return ThreadsOS::Kill(threadId);
}

void ThreadsSetPriorityLevel(TID threadId,
                             ThreadPriorityType priorityLevel) {

    ThreadsOS::SetPriorityLevel(threadId, PRIORITY_CLASS_UNKNOWN, priorityLevel);

}

void ThreadsSetPriorityClass(TID threadId,
                             PriorityClassType priorityClass) {

    ThreadsOS::SetPriorityLevel(threadId, priorityClass, PRIORITY_UNKNOWN);

}

int32 ThreadsGetCPUs(TID threadId) {
    return ThreadsOS::GetCPUs(threadId);
}

ThreadPriorityType ThreadsGetPriorityLevel(TID threadId) {
    return ThreadsOS::GetPriorityLevel(threadId);
}

PriorityClassType ThreadsGetPriorityClass(TID threadId) {
    return ThreadsOS::GetPriorityClass(threadId);
}

ThreadStateType ThreadsGetState(TID threadId) {
    return ThreadsOS::GetState(threadId);
}

void ThreadsEndThread() {

    ThreadsOS::EndThread();
}

const char *ThreadsName(TID threadId) {
    return ThreadsOS::Name(threadId);
}

