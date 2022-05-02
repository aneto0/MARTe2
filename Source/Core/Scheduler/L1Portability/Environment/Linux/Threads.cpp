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

#include <pthread.h>
#include <signal.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StringHelper.h"
#include "Threads.h"
#include "ThreadsDatabase.h"

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
static void* SystemThreadFunction(ThreadInformation *const threadInfo) {

    if (threadInfo != NULL) {

        //Guarantee that the OS finishes the housekeeping before releasing the thread to the user
        ErrorManagement::ErrorType err = threadInfo->ThreadWait();
        //Start the user thread
        if (err == ErrorManagement::NoError) {
            threadInfo->UserThreadFunction();

            bool ok = ThreadsDatabase::Lock();
            if (ok) {
                ThreadInformation *threadInfo2 = ThreadsDatabase::RemoveEntry(Threads::Id());
                if (threadInfo != threadInfo2) {
                    //CStaticAssertErrorCondition(ErrorManagement::FatalError,"SystemThreadFunction TDB_RemoveEntry returns wrong threadInfo \n");
                }
            }
            delete threadInfo;
            ThreadsDatabase::UnLock();
        }
    }
    return static_cast<void *>(NULL);
}

/**
 * @brief Builds a ThreadInformation structure with the thread informations.
 * @param[in] userThreadFunction is the thread function.
 * @param[in] userData is the thread function argument.
 * @param[in] threadName is the desired name of the thread.
 * @return the ThreadInformation structure.
 */
static ThreadInformation* threadInitialisationInterfaceConstructor(const ThreadFunctionType userThreadFunction,
                                                                   const void *const userData,
                                                                   const char8 *const threadName) {

    return new ThreadInformation(userThreadFunction, userData, threadName);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/*lint -e{715} Not implemented in Linux.*/
ThreadStateType GetState(const ThreadIdentifier &threadId) {
    return UnknownThreadStateType;
}

uint32 GetCPUs(const ThreadIdentifier &threadId) {
    uint32 cpus = 0u;
    (void) ThreadsDatabase::Lock();
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    if (pthread_getaffinity_np(threadId, sizeof(cpuset), &cpuset) == 0) {
        int32 j;
        for (j = 0; j < CPU_SETSIZE; j++) {
            if (CPU_ISSET(j, &cpuset) > 0) {
                uint32 cpuBit = 1u;
                cpus |= (cpuBit << j);
            }
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_getaffinity_np()");
    }
    (void) ThreadsDatabase::UnLock();
    return cpus;
}

ThreadIdentifier Id() {
    return pthread_self();
}

/*
 * In linux the priority will vary between 0, i.e. priorityClass = Unknown
 * and priorityLevel = 0 and 99, i.e. priorityClass = RealTime
 * and priorityLevel = 15
 */
void SetPriority(const ThreadIdentifier &threadId,
                 const Threads::PriorityClassType &priorityClass,
                 const uint8 &priorityLevel) {

    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        if (threadInfo != static_cast<ThreadInformation*>(NULL)) {
            uint8 prioLevel = priorityLevel;
            if (prioLevel > 15u) {
                prioLevel = 15u;
            }
            uint8 oldPriorityLevel = threadInfo->GetPriorityLevel();
            Threads::PriorityClassType oldPriorityClass = threadInfo->GetPriorityClass();
            threadInfo->SetPriorityLevel(prioLevel);
            threadInfo->SetPriorityClass(priorityClass);

            int32 policy = SCHED_OTHER;//Default Linux time-sharing scheduling
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
                policy = SCHED_FIFO;//Only put FIFO (more aggressive) for the RealTimePriorityClass
                break;
            }
            uint32 priorityLevelToAssign = 28u * priorityClassNumber;
            priorityLevelToAssign += (static_cast<uint32>(prioLevel));
            //Bound the priority to its maximum
            uint32 maxPriority = static_cast<uint32>(sched_get_priority_max(policy));
            if (priorityLevelToAssign > maxPriority) {
                REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Requested a thread priority that is higher than the one supported by the selected policy - clipping to the maximum value supported by the policy.");
                priorityLevelToAssign = maxPriority;
            }

            sched_param param;
            int32 ignore = 0;
            ok = (pthread_getschedparam(threadId, &ignore, &param) == 0);
            if (ok) {
                param.sched_priority = static_cast<int32>(priorityLevelToAssign);
                if (pthread_setschedparam(threadId, policy, &param) != 0) {
                    threadInfo->SetPriorityLevel(oldPriorityLevel);
                    threadInfo->SetPriorityClass(oldPriorityClass);
                    REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Failed to change the thread priority (likely due to insufficient permissions)");
                }
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_getschedparam()");
            }
        }
    }
    ThreadsDatabase::UnLock();
}

uint8 GetPriorityLevel(const ThreadIdentifier &threadId) {
    uint8 priorityLevel = 0u;

    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        if (threadInfo != NULL) {
            priorityLevel = threadInfo->GetPriorityLevel();
        }
    }
    ThreadsDatabase::UnLock();
    return priorityLevel;
}

PriorityClassType GetPriorityClass(const ThreadIdentifier &threadId) {

    PriorityClassType priorityClass = UnknownPriorityClass;
    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        if (threadInfo != NULL) {
            priorityClass = threadInfo->GetPriorityClass();
        }
    }
    ThreadsDatabase::UnLock();
    return priorityClass;

}

/*
 * A signal is used to know if the other thread is alive.
 */
bool IsAlive(const ThreadIdentifier &threadId) {
    bool alive = false;
    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ok = (ThreadsDatabase::GetThreadInformation(threadId) != NULL);
        ThreadsDatabase::UnLock();
        if(ok) {
            ok=(threadId!=InvalidThreadIdentifier);
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: Invalid Thread Id");
            }
        }
        if (ok) {
            alive = (pthread_kill(threadId, 0) == 0);
        }
    }
    else {
        ThreadsDatabase::UnLock();
    }
    return alive;
}

/*
 * Note that a thread cannot be deleted if it locks a mutex semaphore.
 */
bool Kill(const ThreadIdentifier &threadId) {
    bool ok = false;

    if (IsAlive(threadId)) {
        ok = ThreadsDatabase::Lock();
        if (ok) {
            ThreadInformation *threadInfo = ThreadsDatabase::RemoveEntry(threadId);
            if (threadInfo == NULL) {
                ok = false;
            }
            else {
                delete threadInfo;
            }
        }
        ThreadsDatabase::UnLock();
        if (ok) {
            ok = (threadId != InvalidThreadIdentifier);
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: Invalid Thread Id");
            }
        }
        if (ok) {
            ok = (pthread_cancel(threadId) == 0);
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_cancel()");
            }
        }
        if (ok) {
            ok = (pthread_join(threadId, static_cast<void**>(NULL)) != 0);
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_join()");
            }
        }

    }
    return ok;
}

/*lint -e{715} the exceptionHandlerBehaviour implementation has not been agreed yet.*/
ThreadIdentifier BeginThread(const ThreadFunctionType function,
                             const void *const parameters,
                             const uint32 &stacksize,
                             const char8 *const name,
                             const uint32 exceptionHandlerBehaviour,
                             ProcessorType runOnCPUs) {

    ThreadIdentifier threadId = InvalidThreadIdentifier;
    if (runOnCPUs == UndefinedCPUs) {
        if (ProcessorType::GetDefaultCPUs() != 0u) {
            runOnCPUs = ProcessorType::GetDefaultCPUs();
        }
        else {
            runOnCPUs = ProcessorType(0xffu);
        }
    }

    ThreadInformation *threadInfo = threadInitialisationInterfaceConstructor(function, parameters, name);
    if (threadInfo != static_cast<ThreadInformation*>(NULL)) {
        //CStaticAssertErrorCondition(InitialisationError,"Threads::ThreadsBeginThread (%s) threadInitialisationInterfaceConstructor returns NULL", name);
        pthread_attr_t stackSizeAttribute;
        bool ok = (pthread_attr_init(&stackSizeAttribute) == 0);
        if (ok) {
            ok = (pthread_attr_setstacksize(&stackSizeAttribute, static_cast<osulong>(stacksize)) == 0);
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_attr_setstacksize()");
            }
        }

        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_attr_init()");
        }

        if (ok) {
            /*lint -e{929} cast from pointer to pointer required in order to cast into the pthread callback required function type.*/
            ok = (pthread_create(&threadId, &stackSizeAttribute, reinterpret_cast<void *(*)(void *)>(&SystemThreadFunction), threadInfo) == 0);
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_create()");
            }

            if (ok) {
                threadInfo->SetThreadIdentifier(threadId);
                if(ok) {
                    threadInfo->SetPriorityLevel(0u);
                    threadInfo->SetPriorityClass(Threads::NormalPriorityClass);
                }
            }
        }
        if (ok) {
            if (name != NULL_PTR(const char8 * const)) {
                //pthread_setname_np name maximum size is 16, including the \0
                const uint32 PTHREAD_NAME_MAX_SIZE = 16u;
                char8 threadName[PTHREAD_NAME_MAX_SIZE];
                //The StringHelper::CopyN will already copy the \0 if it is within the PTHREAD_NAME_MAX_SIZE (see man strncpy)
                ok = StringHelper::CopyN(&threadName[0u], name, PTHREAD_NAME_MAX_SIZE);
                if (ok) {
                    //The last char must be \0. This is for the case where the name size >= 16
                    threadName[PTHREAD_NAME_MAX_SIZE - 1u] = '\0';
                    ok = (pthread_setname_np(threadId, &threadName[0u]) == 0);
                    if (!ok) {
                        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_setname_np()");
                    }
                }
            }
        }
        if (ok) {
            ok = (pthread_detach(threadId) == 0);
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_detach()");
            }
        }
        if (ok) {
            cpu_set_t processorCpuSet;
            CPU_ZERO(&processorCpuSet);
            uint32 j;

            for (j = 0u; (j < runOnCPUs.GetCPUsNumber()) && (j < static_cast<uint32>(CPU_SETSIZE)); j++) {
                if (runOnCPUs.CPUEnabled(j + 1u)) {
                    CPU_SET(static_cast<int32>(j), &processorCpuSet);
                }
            }
            ok = (pthread_setaffinity_np(threadId, sizeof(processorCpuSet), &processorCpuSet) == 0);
            if(!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_setaffinity_np()");
            }
        }
        if (ok) {
            ok = ThreadsDatabase::Lock();
            if (ok) {
                threadInfo->SetThreadIdentifier(threadId);
                ok = ThreadsDatabase::NewEntry(threadInfo);
            }
            ThreadsDatabase::UnLock();
            SetPriority(threadId, Threads::NormalPriorityClass, 0u);
        }
        if (ok) {
            ok = ThreadsDatabase::Lock();
            if (ok) {
                ok = threadInfo->ThreadPost();
            }
            ThreadsDatabase::UnLock();
        }
        if(!ok) {
            threadId = InvalidThreadIdentifier;
            delete threadInfo;
        }
    }

    return threadId;
}

void EndThread() {
}

const char8* Name(const ThreadIdentifier &threadId) {
    const char8 *name = static_cast<const char8*>(NULL);
    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        if (threadInfo != NULL) {
            name = threadInfo->ThreadName();
        }
    }
    ThreadsDatabase::UnLock();
    return name;
}

ThreadIdentifier FindByIndex(const uint32 &n) {
    return ThreadsDatabase::GetThreadID(n);
}

uint32 NumberOfThreads() {
    bool ok = ThreadsDatabase::Lock();
    uint32 ret = 0u;
    if (ok) {
        ret = ThreadsDatabase::NumberOfThreads();
    }
    ThreadsDatabase::UnLock();
    return ret;
}

bool GetThreadInfoCopy(ThreadInformation &copy,
                       const uint32 &n) {
    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ok = ThreadsDatabase::GetInfoIndex(copy, n);
    }
    ThreadsDatabase::UnLock();
    return ok;

}

bool GetThreadInfoCopy(ThreadInformation &copy,
                       const ThreadIdentifier &threadId) {

    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ok = ThreadsDatabase::GetInfo(copy, threadId);
    }
    ThreadsDatabase::UnLock();
    return ok;
}

ThreadIdentifier FindByName(const char8 *const name) {
    ThreadIdentifier ret = InvalidThreadIdentifier;
    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ret = ThreadsDatabase::Find(name);
    }
    ThreadsDatabase::UnLock();
    return ret;
}
}

}
