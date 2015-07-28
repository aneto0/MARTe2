/**
 * @file ThreadsOS.cpp
 * @brief Source file for class ThreadsOS
 * @date 27/07/2015
 * @author Andre' Neto
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
 * the class ThreadsOS (public, protected, and private). Be aware that some 
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
#include "Threads.h"
#include "ThreadsDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief The function called when a thread is created.
 * @details Adds the thread in the database and calls the thread callback function.
 * When the callback finishes its execution this function removes the thread from the database.
 * @param[in] threadData the thread information structure.
 */
/*lint -e{9141} namespaces are not currently used.*/
static void * SystemThreadFunction(ThreadInformation * const threadInfo) {
    if (threadInfo != NULL) {
        bool ok = ThreadsDatabase::Lock();
        if (ok) {
            threadInfo->SetThreadIdentifier(Threads::Id());

            ok = ThreadsDatabase::NewEntry(threadInfo);
            ThreadsDatabase::UnLock();
        }
        if (ok) {
            threadInfo->SetPriorityLevel(0u);
            Threads::SetPriorityClass(Threads::Id(), Threads::NormalPriorityClass);
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
        }
    }
    return static_cast<void *>(NULL);
}

/**
 * @brief Builds a ThreadInformation structure with the thread informations.
 * @param[in] userThreadFunction is the thread function.
 * @param[in] userData is the thread function argument.
 * @param[in] threadName is the desired name of the thread.
 * @param[in] exceptionHandlerBehaviour is not used here.
 * @return the ThreadInformation structure.
 */
/*lint -e{9141} namespaces are not currently used.*/
static ThreadInformation * threadInitialisationInterfaceConstructor(const ThreadFunctionType userThreadFunction,
                                                                    const void * const userData,
                                                                    const char8 * const threadName) {

    return new ThreadInformation(userThreadFunction, userData, threadName);
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
void Threads::EndThread() {
    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        /*lint -e{534} possible failure is not handled nor propagated.*/
        ThreadsDatabase::RemoveEntry(Threads::Id());
    }
    ThreadsDatabase::UnLock();
    pthread_exit(static_cast<void *>(NULL));
}

/*lint -e{715} Not implemented in Linux.*/
Threads::ThreadStateType Threads::GetState(const ThreadIdentifier &threadId) {
    return UnknownThreadStateType;
}

uint32 Threads::GetCPUs(const ThreadIdentifier &threadId) {
    uint32 cpus = 0u;
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
    return cpus;
}

ThreadIdentifier Threads::Id() {
    return pthread_self();
}

/**
 * In linux the priority will vary between 0, i.e. priorityClass = Unknown
 * and priorityLevel = 0 and 99, i.e. priorityClass = RealTime
 * and priorityLevel = 15
 */
void Threads::SetPriorityLevelAndClass(const ThreadIdentifier &threadId,
                                       const Threads::PriorityClassType &priorityClass,
                                       const uint8 &priorityLevel) {

    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        ThreadsDatabase::UnLock();
        if (threadInfo != static_cast<ThreadInformation *>(NULL)) {
            uint8 prioLevel = priorityLevel;
            if (prioLevel > 15u) {
                prioLevel = 15u;
            }
            uint8 oldPriorityLevel = GetPriorityLevel(threadId);
            Threads::PriorityClassType oldPriorityClass = GetPriorityClass(threadId);
            threadInfo->SetPriorityLevel(prioLevel);
            threadInfo->SetPriorityClass(priorityClass);

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
            default:
                priorityClassNumber = 0u;
                break;
            }
            uint32 priorityLevelToAssign = 28u * priorityClassNumber;
            priorityLevelToAssign += (static_cast<uint32>(prioLevel));

            int32 policy = 0;
            sched_param param;
            ok = (pthread_getschedparam(threadId, &policy, &param) == 0);
            if (ok) {
                policy = SCHED_FIFO;
                param.sched_priority = static_cast<int32>(priorityLevelToAssign);
                if (pthread_setschedparam(threadId, policy, &param) != 0) {
                    threadInfo->SetPriorityLevel(oldPriorityLevel);
                    threadInfo->SetPriorityClass(oldPriorityClass);
                }
            }
        }
    }
    else {
        ThreadsDatabase::UnLock();
    }
}

uint8 Threads::GetPriorityLevel(const ThreadIdentifier &threadId) {
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

Threads::PriorityClassType Threads::GetPriorityClass(const ThreadIdentifier &threadId) {

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

/**
 * A signal is used to know if the other thread is alive.
 */
bool Threads::IsAlive(const ThreadIdentifier &threadId) {
    bool alive = false;
    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        ok = (ThreadsDatabase::GetThreadInformation(threadId) != NULL);
        ThreadsDatabase::UnLock();
        if (ok) {
            alive = (pthread_kill(threadId, 0) == 0);
        }
    }
    else {
        ThreadsDatabase::UnLock();
    }
    return alive;
}

/**
 * Note that a thread cannot be deleted if it locks a mutex semaphore.
 */
bool Threads::Kill(const ThreadIdentifier &threadId) {
    bool ok = false;
    if (IsAlive(threadId)) {
        ok = ThreadsDatabase::Lock();
        if (ok) {
            ThreadInformation *threadInfo = ThreadsDatabase::RemoveEntry(threadId);
            if (threadInfo == NULL) {
                ok = false;
            }
        }
        ThreadsDatabase::UnLock();

        if (ok) {
            ok = (pthread_cancel(threadId) == 0);
        }
        if (ok) {
            ok = (pthread_join(threadId, static_cast<void **>(NULL)) != 0);
        }
    }
    return ok;
}

/*lint -e{715} the exceptionHandlerBehaviour implementation has not been agreed yet.*/
ThreadIdentifier Threads::BeginThread(const ThreadFunctionType function,
                                      const void * const parameters,
                                      const uint32 &stacksize,
                                      const char8 * const name,
                                      const uint32 exceptionHandlerBehaviour,
                                      ProcessorType runOnCPUs) {

    ThreadIdentifier threadId = InvalidThreadIdentifier;
    if (runOnCPUs == ProcessorType::UndefinedCPUs) {
        if (ProcessorType::GetDefaultCPUs() != 0u) {
            runOnCPUs = ProcessorType::GetDefaultCPUs();
        }
        else {
            runOnCPUs = 0xffu;
        }
    }

    ThreadInformation *threadInfo = threadInitialisationInterfaceConstructor(function, parameters, name);
    if (threadInfo != static_cast<ThreadInformation *>(NULL)) {
        //CStaticAssertErrorCondition(InitialisationError,"Threads::ThreadsBeginThread (%s) threadInitialisationInterfaceConstructor returns NULL", name);
        pthread_attr_t stackSizeAttribute;
        bool ok = (pthread_attr_init(&stackSizeAttribute) == 0);
        if (ok) {
            ok = (pthread_attr_setstacksize(&stackSizeAttribute, static_cast<osulong>(stacksize)) == 0);
        }
        if (ok) {
            /*lint -e{929} cast from pointer to pointer required in order to cast into the pthread callback required function type.*/
            ok = (pthread_create(&threadId, &stackSizeAttribute, reinterpret_cast<void *(*)(void *)>(&SystemThreadFunction), threadInfo) == 0);
        }
        if (ok) {
            ok = (pthread_detach(threadId) == 0);
        }
        if (ok) {
            cpu_set_t processorCpuSet;
            uint32 processorMask = runOnCPUs.GetProcessorMask();
            CPU_ZERO(&processorCpuSet);
            uint32 j;
            for (j = 0u; j < static_cast<uint32>(CPU_SETSIZE); j++) {
                if (((processorMask >> j) & 0x1u) == 0x1u) {
                    CPU_SET(static_cast<int32>(j), &processorCpuSet);
                }
            }

            ok = (pthread_setaffinity_np(threadId, sizeof(processorCpuSet), &processorCpuSet) == 0);
            if (ok) {
                ok = threadInfo->ThreadPost();
            }
        }
        if (!ok) {
            threadId = InvalidThreadIdentifier;
        }
    }
    return threadId;
}

const char8 *Threads::Name(const ThreadIdentifier &threadId) {
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
    return name;
}

ThreadIdentifier Threads::FindByIndex(const uint32 &n) {
    return ThreadsDatabase::GetThreadID(n);
}

uint32 Threads::NumberOfThreads() {
    return ThreadsDatabase::NumberOfThreads();
}

bool Threads::GetThreadInfoCopy(ThreadInformation &copy,
                                const uint32 &n) {
    return ThreadsDatabase::GetInfoIndex(copy, n);
}

bool Threads::GetThreadInfoCopy(ThreadInformation &copy,
                                const ThreadIdentifier &threadId) {
    return ThreadsDatabase::GetInfo(copy, threadId);
}

ThreadIdentifier Threads::FindByName(const char8 * const name) {
    return ThreadsDatabase::Find(name);
}

