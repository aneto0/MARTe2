/**
 * @file Threads.cpp
 * @brief Source file for class Threads
 * @date 31/07/2015
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
 * the class Threads (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Threads.h"
#include "ThreadInformation.h"
#include "ErrorType.h"
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
/*lint -e{9141} namespaces are not currently used.*/
#if USE_THREADS_DATABASE
static void SystemThreadFunction(ThreadInformation * const threadInfo) {
    if (threadInfo != NULL) {
        bool ok = ThreadsDatabase::Lock();
        if (ok) {
            threadInfo->SetThreadIdentifier(Threads::Id());

            ok = ThreadsDatabase::NewEntry(threadInfo);
            ThreadsDatabase::UnLock();
        }
        if (ok) {
            threadInfo->SetPriorityLevel(0u);
            Threads::SetPriority(Threads::Id(), Threads::NormalPriorityClass, 0u);
            //Guarantee that the OS finishes the housekeeping before releasing the thread to the user
            ErrorType err = threadInfo->ThreadWait();
            //Start the user thread
            if (err == NoError) {
                threadInfo->UserThreadFunction();

                ok = ThreadsDatabase::Lock();
                if (ok) {
                    ThreadInformation *threadInfo2 = ThreadsDatabase::RemoveEntry(Threads::Id());
                    if (threadInfo != threadInfo2) {
                        //CStaticAssertErrorCondition(FatalError,"SystemThreadFunction TDB_RemoveEntry returns wrong threadInfo \n");
                    }
                }
                ThreadsDatabase::UnLock();
            }
            delete threadInfo;
            //Passing NULL will cause the calling task to be deleted.
            vTaskDelete(NULL);
        }
    }
    return static_cast<void *>(NULL);
}
#endif
/**
 * @brief Builds a ThreadInformation structure with the thread informations.
 * @param[in] userThreadFunction is the thread function.
 * @param[in] userData is the thread function argument.
 * @param[in] threadName is the desired name of the thread.
 * @return the ThreadInformation structure.
 */
/*lint -e{9141} namespaces are not currently used.*/
static ThreadInformation * threadInitialisationInterfaceConstructor(const ThreadFunctionType userThreadFunction, const void * const userData, const char8 * const threadName) {

    return new ThreadInformation(userThreadFunction, userData, threadName);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/*lint -e{715} Not implemented in Linux.*/
ThreadStateType GetState(const ThreadIdentifier &threadId) {
#if INCLUDE_eTaskGetState
    eTaskState state = eTaskGetState(threadId);
    if(state == eReady) {
        return ReadyState;
    }
    else if(state == eRunning) {
        return RunningState;
    }
    else if(state == eBlocked) {
        return BlockedState;
    }
    else if(state == eSuspended) {
        return SuspendedState;
    }
    else if(state == eDeleted) {
        return DeadState;
    }
#else
    return UnknownThreadStateType;
#endif
}

uint32 GetCPUs(const ThreadIdentifier &threadId) {
    return 1;
}

ThreadIdentifier Id() {
    return xTaskGetCurrentTaskHandle();
}

/**
 * The priority will vary between 0, i.e. priorityClass = Unknown
 * and priorityLevel = 0 and 99, i.e. priorityClass = RealTime
 * and priorityLevel = 15. This is then translated into a number
 * between 0 and configMAX_PRIORITIES (see FreeRTOSConfig.h)
 */
void SetPriority(const ThreadIdentifier &threadId, const PriorityClassType &priorityClass, const uint8 &priorityLevel) {
    uint8 prioLevel = priorityLevel;
    if (prioLevel > 15u) {
        prioLevel = 15u;
    }

    uint32 priorityClassNumber = 0u;
    switch (priorityClass) {
    case UnknownPriorityClass:
        priorityClassNumber = 0u;
        break;
    case IdlePriorityClass:
        priorityClassNumber = 1u;
        break;
    case NormalPriorityClass:
        priorityClassNumber = 2u;
        break;
    case RealTimePriorityClass:
        priorityClassNumber = 3u;
        break;
    }
    uint32 priorityLevelToAssign = 28u * priorityClassNumber;
    priorityLevelToAssign += (static_cast<uint32>(prioLevel));

    unsigned portBASE_TYPE
    priorityToSet = priorityLevelToAssign * configMAX_PRIORITIES / priorityLevelToAssign;
    vTaskPrioritySet(threadId, priorityToSet);
}

uint8 GetPriorityLevel(const ThreadIdentifier &threadId) {
    uint8 priorityLevel = 0u;
#if USE_THREADS_DATABASE
    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        if (threadInfo != NULL) {
            priorityLevel = threadInfo->GetPriorityLevel();
        }
    }
    ThreadsDatabase::UnLock();
#endif
    return priorityLevel;
}

PriorityClassType GetPriorityClass(const ThreadIdentifier &threadId) {
    PriorityClassType priorityClass = UnknownPriorityClass;
#if USE_THREADS_DATABASE
    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        if (threadInfo != NULL) {
            priorityClass = threadInfo->GetPriorityClass();
        }
    }
    ThreadsDatabase::UnLock();
#endif
    return priorityClass;
}

bool IsAlive(const ThreadIdentifier &threadId) {
    ThreadStateType state = GetState(threadId);
    return ((state != UnknownThreadStateType) && (state != DeadState));
}

bool Kill(const ThreadIdentifier &threadId) {
    bool ok = true;
    if (IsAlive(threadId)) {
        vTaskDelete(threadId);
    }
    return ok;
}

/*lint -e{715} the exceptionHandlerBehaviour implementation has not been agreed yet.*/
#if USE_THREADS_DATABASE
ThreadIdentifier BeginThread(const ThreadFunctionType function, const void * const parameters, const uint32 &stacksize, const char8 * const name, const uint32 exceptionHandlerBehaviour, ProcessorType runOnCPUs) {
    ThreadIdentifier threadId = InvalidThreadIdentifier;
    ThreadInformation *threadInfo = threadInitialisationInterfaceConstructor(function, parameters, name);
    if (threadInfo != static_cast<ThreadInformation *>(NULL)) {
        portBASE_TYPE ret = xTaskCreate(reinterpret_cast<void (*)(void *)>(SystemThreadFunction), reinterpret_cast<const char8 *>(name), stacksize, static_cast<void *>(threadInfo), tskIDLE_PRIORITY | portPRIVILEGE_BIT, &threadId);
        bool ok = (ret == pdPASS);
        if (ok) {
            ok = threadInfo->ThreadPost();
        }
        else {
            threadId = InvalidThreadIdentifier;
        }
    }

    return threadId;
}
#else
ThreadIdentifier BeginThread(const ThreadFunctionType function, const void * const parameters, const uint32 &stacksize, const char8 * const name, const uint32 exceptionHandlerBehaviour, ProcessorType runOnCPUs) {
    ThreadIdentifier threadId = InvalidThreadIdentifier;
    portBASE_TYPE ret = xTaskCreate(reinterpret_cast<void (*)(void *)>(function), reinterpret_cast<const char8 *>(name), stacksize, const_cast<void *>(parameters), tskIDLE_PRIORITY | portPRIVILEGE_BIT, &threadId);
    bool ok = (ret == pdPASS);
    if (!ok) {
        threadId = InvalidThreadIdentifier;
    }
    return threadId;
}
#endif

const char8 *Name(const ThreadIdentifier &threadId) {
#if INCLUDE_pcTaskGetTaskName
    return pcTaskGetTaskName(threadId);
#elif USE_THREADS_DATABASE
    const char8 *name = static_cast<const char8 *>(NULL);
    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        ThreadsDatabase::UnLock();
        if (threadInfo != NULL) {
            name = threadInfo->ThreadName();
        }
    }
    else {
        ThreadsDatabase::UnLock();
    }
#else
    return NULL;
#endif
}

ThreadIdentifier FindByIndex(const uint32 &n) {
#if USE_THREADS_DATABASE
    return ThreadsDatabase::GetThreadID(n);
#else
    return InvalidThreadIdentifier;
#endif
}

uint32 NumberOfThreads() {
    return uxTaskGetNumberOfTasks();
}

bool GetThreadInfoCopy(ThreadInformation &copy, const uint32 &n) {
#if USE_THREADS_DATABASE
    return ThreadsDatabase::GetInfoIndex(copy, n);
#else
    return false;
#endif
}

bool GetThreadInfoCopy(ThreadInformation &copy, const ThreadIdentifier &threadId) {
#if USE_THREADS_DATABASE
    return ThreadsDatabase::GetInfo(copy, threadId);
#else
    return false;
#endif
}

ThreadIdentifier FindByName(const char8 * const name) {
#if USE_THREADS_DATABASE
    return ThreadsDatabase::Find(name);
#else
    return InvalidThreadIdentifier;
#endif
}

}

}
