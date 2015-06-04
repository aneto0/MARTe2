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
 * @brief Multi-thread support
 */
#ifndef THREADS_H
#define THREADS_H

#include "GeneralDefinitions.h"
#include "ThreadInformation.h"
#include "ExceptionHandler.h"
#include "ProcessorType.h"



/** Defines the default stack size for a thread. */
#ifndef THREADS_DEFAULT_STACKSIZE
#define THREADS_DEFAULT_STACKSIZE 32768
#endif

/** Assign default initialisation. */
extern ThreadInformationConstructorType threadInitialisationInterfaceConstructor;

extern "C" {
/** @see Threads::BeginThread. */
TID ThreadsBeginThread(ThreadFunctionType function, void *parameters = NULL,
                       uint32 stacksize = THREADS_DEFAULT_STACKSIZE,
                       const char *name = NULL,
                       uint32 exceptionHandlerBehaviour =
                               ExceptionHandler::NotHandled,
                       ProcessorType runOnCPUs = PTUndefinedCPUs);

/** @see Threads::EndThread. */
void ThreadsEndThread();

/** @see Threads::Id. */
TID ThreadsId();

/** @see Threads::Kill. */
bool ThreadsKill(TID tid);

/** @see Threads::IsAlive. */
bool ThreadsIsAlive(TID tid);

/** @see Threads::Name. */
const char *ThreadsName(TID tid);

/** @see Threads::GetCPUs. */
int32 ThreadsGetCPUs(TID tid);

/** @see Threads::GetState. */
uint32 ThreadsGetState(TID tid);

/** @see Threads::GetPriorityLevel. */
uint32 ThreadsGetPriorityLevel(TID tid);

/** @see Threads::GetPriorityClass. */
uint32 ThreadsGetPriorityClass(TID tid);

/** @see Threads::SetPriorityLevel. */
void ThreadsSetPriorityLevel(TID tid, uint32 level);

/** @see Threads::SetPriorityClass. */
void ThreadsSetPriorityClass(TID tid, uint32 priotityClass);

/** Allows to set the thread initialisation method. */
void ThreadsSetInitialisationInterfaceConstructor(
        ThreadInformationConstructorType threadInitialisationInterfaceConstructor);

/** This function allows to call a subroutine within an exception handler protection. */
bool ThreadProtectedExecute(ThreadFunctionType userFunction, void *userData,
                            ExceptionHandler *eh);
}


/** @brief This is the default ThreadInformation object instantiator eventually used in the BeginThread method.
  * @param userThreadFunction The thread entry point.
  * @param userData A pointer to data that can be passed to the thread.
  * @param threadName The thread name.
  * @param exceptionHandlerBehaviour Describes the behaviour of threads when an exception occurr. */
ThreadInformation * DefaultThreadInformationConstructor(
        ThreadFunctionType userThreadFunction, void *userData,
        const char *threadName, uint32 exceptionHandlerBehaviour);

/**
 * @brief Functions for threads management.
 * 
 * These methods allows to begin and end threads holding all information in a general thread database (@see ThreadsDatabase.h),
 * other functions allows to a thread to kill, check the life, get informations of other threads by their identifier. 
 *
 * Most of the implementation is delegated to ThreadsOS.h which provides system level functions for threads management.
 *
 * These are the basic functions to use threads.  
 */


class Threads {
public:

    friend void ThreadsSetInitialisationInterfaceConstructor(
            ThreadInformationConstructorType tiic);
    friend void ThreadsSetPriorityLevel(TID tid, uint32 level);
    friend void ThreadsSetPriorityClass(TID tid, uint32 priotityClass);
    friend TID ThreadsBeginThread(ThreadFunctionType function, void *parameters,
                                  uint32 stacksize, const char *name,
                                  uint32 exceptionHandlerBehaviour,
                                  ProcessorType runOnCPUs);
    friend TID ThreadsId();
    friend void ThreadsEndThread();
    friend bool ThreadsKill(TID tid);
    friend bool ThreadsIsAlive(TID tid);
    friend void ThreadsSetNormalClass();
    friend void ThreadsSetRealTimeClass();
    friend void ThreadsSetIdleClass();
    friend void ThreadsSetHighClass();

public:
/** The three main states are: READY, PENDING and SUSPENDED 
  * All the other states are substates of these and may or not be available depending on the OS */

    static const uint32 STATE_UNKNOWN = -1;
    static const uint32 STATE_READY = 1024;
    static const uint32 STATE_PEND = 512;
    static const uint32 STATE_SUSP = 256;
    static const uint32 STATE_BLOCKED = 2;
    static const uint32 STATE_SEM = 4;
    static const uint32 STATE_DELAY = 8;
    static const uint32 STATE_TOUT = 16;
    static const uint32 STATE_RUN = 32;
    static const uint32 STATE_DEAD = 64;

    /**
     * List of possible priority classes in ascending order of priority
     */
    static const uint32 PRIORITY_CLASS_UNKNOWN = 0;
    static const uint32 PRIORITY_CLASS_IDLE = 1;
    static const uint32 PRIORITY_CLASS_NORMAL = 2;
    static const uint32 PRIORITY_CLASS_HIGH = 3;
    static const uint32 PRIORITY_CLASS_REAL_TIME = 4;

    /**
     * List of possible priorities. Each of the above classes can contain any of the 
     * these subpriorities. e.g. REAL_TIME_PRIORITY_CLASS with PRIORITY_LOWEST has
     * less priority then REAL_TIME_PRIORITY_CLASS with PRIORITY_BELOW_NORMAL but 
     * has more priority than HIGH_PRIORITY_CLASS with PRIORITY_TIME_CRITICAL
     */
    static const uint32 PRIORITY_UNKNOWN = 0;
    static const uint32 PRIORITY_IDLE = 1;
    static const uint32 PRIORITY_LOWEST = 2;
    static const uint32 PRIORITY_BELOW_NORMAL = 3;
    static const uint32 PRIORITY_NORMAL = 4;
    static const uint32 PRIORITY_ABOVE_NORMAL = 5;
    static const uint32 PRIORITY_HIGHEST = 6;
    static const uint32 PRIORITY_TIME_CRITICAL = 7;

    /** @brief Sets the function used to build the thread initialization interface.
      *
      * An initialisation interface object is created using either the default value
      * or the parameter passed to this function by the BeginThread method.
      * @param tiic A pointer to the function to be used in the BeginThread method. */
    static void SetThreadInformationConstructor(
            ThreadInformationConstructorType tiic) {
        ThreadsSetInitialisationInterfaceConstructor(tiic);
    }

    /** @brief Change thread priority.
      * @param tid is the thread identifier.
      * @param level is the level to assign to the thread. 
      * Applies only to current thread 0-31 (on windows it is actually/4) */
    static void SetPriorityLevel(TID tid, uint32 level) {
        ThreadsSetPriorityLevel(tid, level);
    }

    /** @brief Change thread priority class.
      * @param tid is the thread identifier.
      * @param priorityClass is the class to assign to the thread. */
    static void SetPriorityClass(TID tid, uint32 priorityClass) {
        ThreadsSetPriorityClass(tid, priorityClass);
    }

    /** @brief Called implicitly at the end of the main thread function. 
      * Calling this leaves some allocated memory unfreed */
    static void EndThread() {
        ThreadsEndThread();
    }

    /**
     * @brief A call to this function will start a thred.
     * @param function The function main for the thread.
     * @param parameters A pointer passed to the thread main function.
     * @param stacksize The size of the stack.
     * @param name The name of the thread.
     * @param exceptionHandlerBehaviour The action to perform when an exception occurs.
     * @param runOnCPUs The cpu mask where the thread can be executed.
     * @return The thread identification number.
     * 
     * This function will dynamically allocate an object of type
     * ThreadInformation using the function hook ThreadInformationConstructor.
     * This allows the programmer to choose which constructor has to be used in the case
     * a ThreadInformation derived class had been used.
     */
    static TID BeginThread(ThreadFunctionType function, void *parameters = NULL,
                           uint32 stacksize = THREADS_DEFAULT_STACKSIZE,
                           const char *name = NULL,
                           uint32 exceptionHandlerBehaviour =
                                   ExceptionHandler::NotHandled,
                           ProcessorType runOnCPUs = PTUndefinedCPUs) {
        return ThreadsBeginThread(function, parameters, stacksize, name,
                                  exceptionHandlerBehaviour, runOnCPUs);
    }

    /** @brief Gets the current thread id; 
      * @return the current thread id. */
    static TID Id() {
        return ThreadsId();
    }

    /** @brief Asynchronous thread kill.
      *
      * A thread cannot be killed while locks a mutex semaphore in Linux.
      * @param tid is the id of the thread to kill.
      * @return true if system level kill function returns without errors. */
    static bool Kill(TID tid) {
        return ThreadsKill(tid);
    }

    /** @brief Check if thread is still alive.
      * @param tid is the id of the thread which must be checked.
      * @return true if thread is alive (checking before that it is in the database), false otherwise. */
    static bool IsAlive(TID tid) {
        return ThreadsIsAlive(tid);
    }

    /** @brief Get thread name.
      * @param tid is the id of the requested thread,
      * @return the name of the thread with tid as id. */
    static const char *Name(TID tid) {
        return ThreadsName(tid);
    }

    
   /** @brief Returns the task state.
     * 
     * This can be a masked combination of any of the
     * defined THREAD_STATE. So for instance a value of "6" means:
     * THREAD_STATE_BLOCKED + THREAD_STATE_SEM.
     * @param tid is the id of the requested thread.
     * @return the thread state(s). */
    static uint32 GetState(TID tid) {
        return ThreadsGetState(tid);
    }

    /** @brief Get the priority level of a thread.
      * @param tid is the id of the requested thread.
      * @return the thread priority level. */
    static int32 GetPriorityLevel(TID tid) {
        return ThreadsGetPriorityLevel(tid);
    }

    /** @brief Get the priority class of a thread.
      * @param tid is the id of the requested thread.
      * @return the thread priority class. */
    static int32 GetPriorityClass(TID tid) {
        return ThreadsGetPriorityClass(tid);
    }

};
// end class Thread

#endif

