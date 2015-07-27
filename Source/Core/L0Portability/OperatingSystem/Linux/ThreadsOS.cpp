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
#ifndef LINT
#include <pthread.h>
#include <signal.h>
#endif
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Threads.h"
#include "ThreadsDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#define __thread_decl
/**
 * @brief Callback thread function as defined in Linux
 */
typedef void *(*StandardThreadFunction)(void *args);
/**
 * @brief Platform dependent implementations of threads.
 * @details These functions calls system APIs for threads. It is possible use the thread database
 * which provides a smart way to obtain informations of all alive threads.
 */
class ThreadsOS {

public:

    /**
     * @brief The function called by each thread.
     * @details Adds the thread in the database, call the thread function and at last remove the the thread from the database.
     * @param[in] threadData is the thread information structure. */
    static void __thread_decl SystemThreadFunction(void *threadData) {
        ThreadInformation *threadInfo = (ThreadInformation *) threadData;
        if (threadInfo == NULL) {
            return;
        }

        ThreadsDatabase::Lock();
        threadInfo->SetThreadIdentifier(Threads::Id());
        ThreadsDatabase::NewEntry(threadInfo);
        ThreadsDatabase::UnLock();

        threadInfo->SetPriorityLevel(Threads::PRIORITY_NORMAL);
        Threads::SetPriorityClass(Threads::Id(), Threads::PRIORITY_CLASS_NORMAL);
        //Guarantee that the OS finishes the housekeeping before releasing the thread to the user
        threadInfo->ThreadWait();
        //Start the user thread
        threadInfo->UserThreadFunction();

        ThreadsDatabase::Lock();
        ThreadInformation *threadInfo2 = ThreadsDatabase::RemoveEntry(Threads::Id());
        ThreadsDatabase::UnLock();

        if (threadInfo != threadInfo2) {
            //CStaticAssertErrorCondition(FatalError,"SystemThreadFunction TDB_RemoveEntry returns wrong threadInfo \n");
        }

        delete threadInfo;
    }

    /**
     * @brief Builds a ThreadInformation structure with the thread informations.
     * @param[in] userThreadFunction is the thread function.
     * @param[in] userData is the thread function argument.
     * @param[in] threadName is the desired name of the thread.
     * @param[in] exceptionHandlerBehaviour is not used here.
     * @return the ThreadInformation structure.
     */
    static ThreadInformation * threadInitialisationInterfaceConstructor(ThreadFunctionType userThreadFunction,
                                                                        void *userData,
                                                                        const char8 *threadName,
                                                                        uint32 exceptionHandlerBehaviour) {

        return new ThreadInformation(userThreadFunction, userData, threadName);
    }
};
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
void Threads::EndThread() {
    ThreadsDatabase::Lock();
    ThreadsDatabase::RemoveEntry(Threads::Id());
    ThreadsDatabase::UnLock();
    pthread_exit(0);
}

/**
 * @brief Not implemented in Linux
 * @param[in] threadId is the thread identifier.
 * @return the thread state.
 */
Threads::ThreadStateType Threads::GetState(ThreadIdentifier threadId) {
    return STATE_UNKNOWN;
}

/**
 * @brief Called by Threads::GetCPUs
 * @param[in] threadId is the thread identifier.
 * @return the cpu where the thread is executing.
 */
int32 Threads::GetCPUs(ThreadIdentifier threadId) {
    int32 cpus = -1;
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    if (pthread_getaffinity_np(threadId, sizeof(cpuset), &cpuset) != 0) {
        return cpus;
    }
    cpus = 0;
    int32 j = 0;
    for (j = 0; j < CPU_SETSIZE; j++) {
        if (CPU_ISSET(j, &cpuset)) {
            cpus |= (1 << j);
        }
    }
    return cpus;
}

/**
 * @brief Called by Threads::Id.
 * @return the thread identifier.
 */
ThreadIdentifier Threads::Id() {
    return pthread_self();
}

/**
 * @brief Called by Threads::SetPriorityLevel.
 *
 * @details In linux the priority will vary between 33, i.e. priorityClass = IDLE_PRIORITY_CLASS
 * and priorityLevel = PRIORITY_IDLE and 99, i.e. priorityClass = REAL_TIME_PRIORITY_CLASS
 * and priorityLevel = PRIORITY_TIME_CRITICAL
 *
 * @param[in] threadId is the thread identifier.
 * @param[in] priorityClass is the desired class priority to assign to the thread.
 * @param[in] priorityLevel is the desired thread priority to assign to the thread.
 */
void Threads::SetPriorityLevelAndClass(ThreadIdentifier threadId,
                                       Threads::PriorityClassType priorityClass,
                                       Threads::ThreadPriorityType priorityLevel) {

    //Cannot set an unknown priority
    if (priorityLevel == 0 && priorityClass == 0) {
        return;
    }

    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    if (threadInfo == NULL) {
        ThreadsDatabase::UnLock();
        return;
    }

    if (priorityClass == 0) {
        threadInfo->SetPriorityLevel(priorityLevel);
        priorityClass = threadInfo->GetPriorityClass();
    }

    if (priorityLevel == 0) {
        threadInfo->SetPriorityClass(priorityClass);
        priorityLevel = threadInfo->GetPriorityLevel();
    }

    uint32 priorityLevelToAssign = 20 * (uint32) priorityClass + (uint32) priorityLevel + 12;

    int32 policy = 0;
    sched_param param;
    pthread_getschedparam(threadId, &policy, &param);
    policy = SCHED_RR;
    param.sched_priority = priorityLevelToAssign;
    pthread_setschedparam(Id(), policy, &param);

    ThreadsDatabase::UnLock();
}

/**
 * @brief Called by Threads::GetPriorityLevel.
 * @details Returns the priority of a thread.
 * @param[in] threadId is the thread identifier.
 * @return the priority of the specified thread.
 */
Threads::ThreadPriorityType Threads::GetPriorityLevel(ThreadIdentifier threadId) {
    ThreadPriorityType priorityLevel = PRIORITY_UNKNOWN;

    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    if (threadInfo != NULL) {
        priorityLevel = threadInfo->GetPriorityLevel();
    }
    ThreadsDatabase::UnLock();
    return priorityLevel;
}

/**
 * @brief Called by Threads::GetPriorityClass.
 * @details Returns the priority class of a thread.
 * @param[in] threadId is the thread identifier.
 * @return the priority class of the specified thread.
 */
Threads::PriorityClassType Threads::GetPriorityClass(ThreadIdentifier threadId) {

    PriorityClassType priorityClass = PRIORITY_CLASS_UNKNOWN;
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    if (threadInfo != NULL) {
        priorityClass = threadInfo->GetPriorityClass();
    }
    ThreadsDatabase::UnLock();
    return priorityClass;

}

/**
 * @brief Called by Threads::IsAlive
 *
 * @details A signal is used to know if the other thread is alive.
 *
 * @param[in] threadId is the thread identifier.
 */
bool Threads::IsAlive(ThreadIdentifier threadId) {

    ThreadsDatabase::Lock();
    bool condition = (ThreadsDatabase::GetThreadInformation(threadId) != NULL);
    ThreadsDatabase::UnLock();
    if (condition) {

        if (threadId == 0) {
            return false;
        }
        return (pthread_kill(threadId, 0) == 0);
    }
    else {
        return false;
    }
}

/**
 * @brief Called by Threads::Kill
 * @param[in] threadId is the thread identifier.
 * @details A thread cannot be deleted if it locks a mutex semaphore.
 */
bool Threads::Kill(ThreadIdentifier threadId) {

    if (!IsAlive(threadId)) {
        return false;
    }

    ThreadsDatabase::Lock();
    ThreadsDatabase::RemoveEntry(threadId);
    ThreadsDatabase::UnLock();

    int32 ret = pthread_cancel(threadId);
    if (ret == 0) {
        pthread_join(threadId, NULL);
        return true;
    }
    return false;
}

/**
 * @brief Called by Threads::BeginThread.
 * @param[in] function is the thread function.
 * @param[in] parameters is the thread function parameters structure.
 * @param[in] name is the desired name for the thread.
 * @param[in] stacksize is the desired stack size to assign to the thread.
 * @param[in] exceptionHandlerBehaviour is not used here.
 * @param[in] runOnCPUs is used to set an affinity with a desired cpu.
 */
ThreadIdentifier Threads::BeginThread(ThreadFunctionType function,
                                      void *parameters,
                                      uint32 stacksize,
                                      const char8 *name,
                                      uint32 exceptionHandlerBehaviour,
                                      ProcessorType runOnCPUs) {

    if (runOnCPUs == ProcessorType::UndefinedCPUs) {
        if (ProcessorType::GetDefaultCPUs() != 0) {
            runOnCPUs = ProcessorType::GetDefaultCPUs();
        }
        else {
            runOnCPUs = 0xff;
        }
    }

    ThreadInformation *threadInfo = ThreadsOS::threadInitialisationInterfaceConstructor(function, parameters, name, exceptionHandlerBehaviour);
    if (threadInfo == NULL) {
        //CStaticAssertErrorCondition(InitialisationError,"Threads::ThreadsBeginThread (%s) threadInitialisationInterfaceConstructor returns NULL", name);
        return InvalidThreadIdentifier;
    }

    ThreadIdentifier threadId = 0;
    pthread_attr_t stackSizeAttribute;
    pthread_attr_init(&stackSizeAttribute);
    pthread_attr_setstacksize(&stackSizeAttribute, stacksize);
    pthread_create(&threadId, &stackSizeAttribute, (StandardThreadFunction) ThreadsOS::SystemThreadFunction, threadInfo);
    pthread_detach(threadId);
    uint32 processorMask = runOnCPUs.GetProcessorMask();
    pthread_setaffinity_np(threadId, sizeof(processorMask), (cpu_set_t *) &processorMask);
    threadInfo->ThreadPost();

    return threadId;
}

/**
 * @brief Called by Threads::Name
 * @param[in] threadId is the thread identifier.
 * @return the name of the specified thread.
 */
const char8 *Threads::Name(ThreadIdentifier threadId) {
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    ThreadsDatabase::UnLock();

    if (threadInfo != NULL) {
        return threadInfo->ThreadName();
    }
    return NULL;
}

/**
 * @brief Executes the function specified.
 * @param[in] userFunction is the function to be called.
 * @param[in] userData is the function argument.
 * @param[in] ehi is the exception handler.
 * @return true.
 * @see ThreadInformation::ExceptionProtectedExecute.
 */
bool Threads::ProtectedExecute(ThreadFunctionType userFunction,
                               void *userData,
                               ExceptionHandler *ehi) {
    ThreadIdentifier threadId = Threads::Id();
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    ThreadsDatabase::UnLock();
    if (threadInfo != NULL) {
        return threadInfo->ExceptionProtectedExecute(userFunction, userData, ehi);
    }
    return false;
}

/**
 * @brief Returns the id of the n-th thread in the database.
 * @param[in] n is the thread index.
 * @return the id of the n-th thread in the database, -1 if the database is empty.
 */
ThreadIdentifier Threads::FindByIndex(uint32 n) {
    return ThreadsDatabase::GetThreadID(n);
}

/**
 * @brief Returns the number of threads currently in the database.
 * @return the number of threads currently in the database.
 */
uint32 Threads::NumberOfThreads() {
    return ThreadsDatabase::NumberOfThreads();
}

/**
 * @brief Get a copy of the thread information stored in the database.
 * @param[out] copy is the thread information structure in return.
 * @param[in] n is the threads index, if it is <0 is ignored.
 * @param[in] tid is the thread identifier.
 * @return true if the requested element is in the database, false otherwise.
 */
bool Threads::GetThreadInfoCopy(ThreadInformation &copy,
                                int32 n,
                                ThreadIdentifier threadId) {
    return ThreadsDatabase::GetInfo(copy, n, threadId);
}

/**
 * @brief Search the thread with the specified name.
 * @param[in] name is the thread name.
 * @return the id of the first found thread with the specified name.
 */
ThreadIdentifier Threads::FindByName(const char8 *name) {
    return ThreadsDatabase::Find(name);
}

