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
 * @brief Thread functions implementation in Linux using pthread library and signals. 
 */
#ifndef THREADS_OS_H
#define THREADS_OS_H 

#include <pthread.h>
#include <signal.h>

#define __thread_decl 
/**
 * @brief Callback thread function as defined in Linux
 */
typedef void *(*StandardThreadFunction)(void *args);

void ThreadsOSEndThread() {
    pthread_exit(0);
}

/**
 * @brief Not implemented in Linux
 * @param threadid is the thread identifier.
 * @return the thread state.
 */
uint32 ThreadsOSGetState(TID threadId) {
    return Threads::STATE_UNKNOWN;
}


/** @see Threads::GetCPUs */
int32 ThreadsOSGetCPUs(TID threadId) {
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

TID ThreadsOSId() {
    return pthread_self();
}

/**
 * @see Threads::SetPriorityLevel.
 *
 * In linux the priority will vary between 33, i.e. priorityClass = IDLE_PRIORITY_CLASS
 * and priorityLevel = PRIORITY_IDLE and 99, i.e. priorityClass = REAL_TIME_PRIORITY_CLASS
 * and priorityLevel = PRIORITY_TIME_CRITICAL
 */
void ThreadsOSSetPriorityLevel(TID threadId, uint32 priorityClass,
                               uint32 priorityLevel) {
    priorityLevel = 20 * priorityClass + priorityLevel + 12;
    int32 policy = 0;
    sched_param param;
    pthread_getschedparam(threadId, &policy, &param);
    policy = SCHED_RR;
    param.sched_priority = priorityLevel;
    pthread_setschedparam(ThreadsOSId(), policy, &param);
}

/** @see Threads::Kill
  *
  * A thread cannot be deleted if it locks a mutex semaphore. */
bool ThreadsOSKill(TID threadId) {
    if (threadId == 0) {
        return True;
    }
    int32 ret = pthread_cancel(threadId);
    if (ret == 0) {
        pthread_join(threadId, NULL);
        return True;
    }
    return False;
}

/** @see Threads::IsAlive
  * 
  * A signal is used to know if the other thread is alive.  */
bool ThreadsOSIsAlive(TID threadId) {
    if (threadId == 0) {
        return False;
    }
    return (pthread_kill(threadId, 0) == 0);
}

/** @see Threads::BeginThread */
TID ThreadsOSBeginThread(StandardThreadFunction function,
                         ThreadInformation *threadInfo, uint32 stacksize,
                         ProcessorType runOnCPUs) {
    TID threadId = 0;
    pthread_attr_t stackSizeAttribute;
    pthread_attr_init(&stackSizeAttribute);
    pthread_attr_setstacksize(&stackSizeAttribute, stacksize);
    pthread_create(&threadId, &stackSizeAttribute, function, threadInfo);
    pthread_detach(threadId);
    pthread_setaffinity_np(threadId, sizeof(runOnCPUs.processorMask),
                           (cpu_set_t *) &runOnCPUs.processorMask);
    return threadId;
}
#endif

