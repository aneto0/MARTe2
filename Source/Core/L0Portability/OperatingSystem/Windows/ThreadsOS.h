/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id: $
 *
 **/
/**
 * @file
 * @brief Thread functions implementation in Window using process library. */
#ifndef THREADS_OS_H
#define THREADS_OS_H 

#include <process.h>
#include <signal.h>

#define __thread_decl
/**
 * @brief Callback thread function
 */
typedef void *(*StandardThreadFunction)(void *args);

void ThreadsOSEndThread() {
    _endthread();
}

/**
 * @brief Not implemented in Windows
 * @param threadid is the thread identifier.
 * @return the state of the thread.
 */
uint32 ThreadsOSGetState(TID threadId) {
    return Threads::STATE_UNKNOWN;
}

/** @brief Not implemented in Windows.
  * @see Threads::GetCPUs */
int32 ThreadsOSGetCPUs(TID threadId) {
    return -1;
}

/** @see Threads::Id */
TID ThreadsOSId() {
    return (TID) GetCurrentThreadId();
}


/** @see Threads::SetPriorityLevel */
void ThreadsOSSetPriorityLevel(TID threadId, uint32 priorityClass,
                               uint32 priorityLevel) {
    priorityLevel /= 5;

    switch (GetPriorityClass(GetCurrentProcess())) {
        case IDLE_PRIORITY_CLASS :
        if (priorityLevel > 5) priorityLevel = 5;
        break;
        case REALTIME_PRIORITY_CLASS :
        if (priorityLevel > 6) priorityLevel = 6;
        break;
        case HIGH_PRIORITY_CLASS :
        case NORMAL_PRIORITY_CLASS :
        default:
        if (priorityLevel > 5) priorityLevel = 5;
        if (priorityLevel == 0) priorityLevel = 1;
        break;
    }

    switch (priorityLevel) {
    case 0:
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);
        break;
    case 1:
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
        break;
    case 2:
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
        break;
    case 3:
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
        break;
    case 4:
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
        break;
    case 5:
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
        break;
    case 6:
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
        break;
    }
}

/** @see Threads::Kill */
bool ThreadsOSKill(TID threadId) {
    if (TerminateThread((HANDLE) threadId, 0) == FALSE)
        return False;
    return True;
}

/** @see Threads::IsAlive */
bool ThreadsOSIsAlive(TID threadId) {
    return (GetThreadPriority((HANDLE) threadId) != THREAD_PRIORITY_ERROR_RETURN);
}

TID ThreadsOSBeginThread(StandardThreadFunction function,
                         ThreadInformation *threadInfo, uint32 stacksize,
                         ProcessorType runOnCPUs) {
return (HANDLE)_beginthread((void (__cdecl *)(void *))function,stacksize,threadInfo);
}
#endif

