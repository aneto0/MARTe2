/**
 * @file Threads.h
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

#ifndef THREADS_H_
#define 		THREADS_H_

/** Defines the default stack size for a thread. */
#ifndef THREADS_DEFAULT_STACKSIZE
#define THREADS_DEFAULT_STACKSIZE 32768
#endif

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "ThreadInformation.h"
#include "ExceptionHandler.h"
#include "ProcessorType.h"

/*---------------------------------------------------------------------------*/
/*                        Friend method definitions                          */
/*---------------------------------------------------------------------------*/

extern "C" {
/** @brief Threads::BeginThread. */
TID ThreadsBeginThread(ThreadFunctionType function,
                       void *parameters = NULL,
                       uint32 stacksize = THREADS_DEFAULT_STACKSIZE,
                       const char *name = NULL,
                       uint32 exceptionHandlerBehaviour = ExceptionHandler::NotHandled,
                       ProcessorType runOnCPUs = PTUndefinedCPUs);

/** @brief Threads::EndThread. */
void ThreadsEndThread();

/** @brief Threads::Id. */
TID ThreadsId();

/** @brief Threads::Kill. */
bool ThreadsKill(TID tid);

/** @brief Threads::IsAlive. */
bool ThreadsIsAlive(TID tid);

/** @brief Threads::Name. */
const char *ThreadsName(TID tid);

/** @brief Threads::GetCPUs. */
int32 ThreadsGetCPUs(TID tid);

/** @brief Threads::GetState. */
ThreadStateType ThreadsGetState(TID tid);

/** @brief Threads::GetPriorityLevel. */
ThreadPriorityType ThreadsGetPriorityLevel(TID tid);

/** @brief Threads::GetPriorityClass. */
PriorityClassType ThreadsGetPriorityClass(TID tid);

/** @brief Threads::SetPriorityLevel. */
void ThreadsSetPriorityLevel(TID tid,
                             ThreadPriorityType level);

/** @brief Threads::SetPriorityClass. */
void ThreadsSetPriorityClass(TID tid,
                             PriorityClassType priotityClass);

/**
 * @brief This function allows to call a subroutine within an exception handler protection.
 * @param[in] userFunction is a pointer to the thread function.
 * @param[in] userData is the argument of the thread function.
 * @param[in] eh is the exception handler.
 */
bool ThreadProtectedExecute(ThreadFunctionType userFunction,
                            void *userData,
                            ExceptionHandler *eh);
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Functions for threads management.
 *
 * @details These methods allows to begin and end threads holding all information in a general thread database (ThreadsDatabase.h),
 * other functions allows to a thread to kill, check the life, get informations of other threads by their identifier.
 *
 * @details It is possible to query the state of a thread. The return value is a ThreadStateType value (defined in ThreadInformation.h) and could be one of the following:
 *
 * STATE_UNKNOWN = -1\n
 * STATE_READY = 1024\n
 * STATE_PEND = 512\n
 * STATE_SUSP = 256\n
 * STATE_BLOCKED = 2\n
 * STATE_SEM = 4\n
 * STATE_DELAY = 8\n
 * STATE_TOUT = 16\n
 * STATE_RUN = 32\n
 * STATE_DEAD = 64
 *
 * @details It is possible to query or set the priority class of a thread. The priority class is a PriorityClassType and could be one of the following:
 *
 * PRIORITY_CLASS_UNKNOWN = 0\n
 * PRIORITY_CLASS_IDLE = 1\n
 * PRIORITY_CLASS_NORMAL = 2\n
 * PRIORITY_CLASS_HIGH = 3\n
 * PRIORITY_CLASS_REAL_TIME = 4
 *
 * @details It is possible to query or set the priority of a thread. The thread priority is a ThreadPriorityType and could be one of the following:
 *
 * PRIORITY_UNKNOWN = 0\n
 * PRIORITY_IDLE = 1\n
 * PRIORITY_LOWEST = 2\n
 * PRIORITY_BELOW_NORMAL = 3\n
 * PRIORITY_NORMAL = 4\n
 * PRIORITY_ABOVE_NORMAL = 5\n
 * PRIORITY_HIGHEST = 6\n
 * PRIORITY_TIME_CRITICAL = 7\n
 *
 * @details Most of the implementation is delegated to ThreadsOS.h which provides system level functions for threads management.
 */
class Threads {

public:

    friend void ThreadsSetInitialisationInterfaceConstructor(ThreadInformationConstructorType tiic);
    friend void ThreadsSetPriorityLevel(TID tid,
                                        uint32 level);
    friend void ThreadsSetPriorityClass(TID tid,
                                        uint32 priotityClass);
    friend TID ThreadsBeginThread(ThreadFunctionType function,
                                  void *parameters,
                                  uint32 stacksize,
                                  const char *name,
                                  uint32 exceptionHandlerBehaviour,
                                  ProcessorType runOnCPUs);
    friend TID ThreadsId();
    friend void ThreadsEndThread();
    friend bool ThreadsKill(TID tid);
    friend bool ThreadsIsAlive(TID tid);

    /*
     friend void ThreadsSetNormalClass();
     friend void ThreadsSetRealTimeClass();
     friend void ThreadsSetIdleClass();
     friend void ThreadsSetHighClass();
     */

public:

    /*
     * List of possible priorities. Each of the above classes can contain any of the
     * these subpriorities. e.g. REAL_TIME_PRIORITY_CLASS with PRIORITY_LOWEST has
     * less priority then REAL_TIME_PRIORITY_CLASS with PRIORITY_BELOW_NORMAL but
     * has more priority than HIGH_PRIORITY_CLASS with PRIORITY_TIME_CRITICAL
     */

    /**
     * @brief Change thread priority.
     * @param[in] tid is the thread identifier.
     * @param[in] level is the level to assign to the thread.
     */
    static void SetPriorityLevel(TID tid,
                                 ThreadPriorityType level) {
        ThreadsSetPriorityLevel(tid, level);
    }

    /**
     * @brief Change thread priority class.
     * @param[in] tid is the thread identifier.
     * @param[in] priorityClass is the class to assign to the thread.
     */
    static void SetPriorityClass(TID tid,
                                 PriorityClassType priorityClass) {
        ThreadsSetPriorityClass(tid, priorityClass);
    }

    /**
     * @brief Called implicitly at the end of the main thread function.
     * @details Calling this leaves some allocated memory unfreed.
     */
    static void EndThread() {
        ThreadsEndThread();
    }

    /**
     * @brief A call to this function will start a thread.
     * @param[in] function The function main for the thread.
     * @param[in] parameters A pointer passed to the thread main function.
     * @param[in] stacksize The size of the stack.
     * @param[in] name The name of the thread.
     * @param[in] exceptionHandlerBehaviour The action to perform when an exception occurs.
     * @param[in] runOnCPUs The cpu mask where the thread can be executed.
     * @return The thread identification number.
     *
     * @details This function will dynamically allocate an object of type
     * ThreadInformation using the function hook ThreadInformationConstructor.
     * This allows the programmer to choose which constructor has to be used in the case
     * a ThreadInformation derived class has been used.
     */
    static TID BeginThread(ThreadFunctionType function,
                           void *parameters = NULL,
                           uint32 stacksize = THREADS_DEFAULT_STACKSIZE,
                           const char *name = NULL,
                           uint32 exceptionHandlerBehaviour = ExceptionHandler::NotHandled,
                           ProcessorType runOnCPUs = PTUndefinedCPUs) {
        return ThreadsBeginThread(function, parameters, stacksize, name, exceptionHandlerBehaviour, runOnCPUs);
    }

    /**
     * @brief Gets the current thread id;
     * @return the current thread id.
     */
    static TID Id() {
        return ThreadsId();
    }

    /** @brief Asynchronous thread kill.
     *
     * @details A thread cannot be killed in critical sections in Linux.
     * @param[in] tid is the id of the thread to kill.
     * @return true if system level kill function returns without errors.
     */
    static bool Kill(TID tid) {
        return ThreadsKill(tid);
    }

    /**
     * @brief Check if thread is still alive.
     * @param[in] tid is the id of the thread which must be checked.
     * @return true if thread is alive (checking before that it is in the database), false otherwise.
     */
    static bool IsAlive(TID tid) {
        return ThreadsIsAlive(tid);
    }

    /**
     * @brief Get thread name.
     * @param[in] tid is the id of the thread,
     * @return the name of the thread with tid as id.
     */
    static const char *Name(TID tid) {
        return ThreadsName(tid);
    }

    /**
     * @brief Returns the task state.
     *
     * @details This can be a masked combination of any of the
     * defined ThreadStateType. So for instance a value of "6" means:
     * ThreadStateType_BLOCKED + ThreadStateType_SEM.
     *
     * @param[in] tid is the id of the requested thread.
     * @return the thread state(s).
     */
    static ThreadStateType GetState(TID tid) {
        return ThreadsGetState(tid);
    }

    /**
     * @brief Get the priority level of a thread.
     * @param[in] tid is the id of the requested thread.
     * @return the thread priority level.
     */
    static ThreadPriorityType GetPriorityLevel(TID tid) {
        return ThreadsGetPriorityLevel(tid);
    }

    /**
     * @brief Get the priority class of a thread.
     * @param[in] tid is the id of the requested thread.
     * @return the thread priority class.
     */
    static PriorityClassType GetPriorityClass(TID tid) {
        return ThreadsGetPriorityClass(tid);
    }
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADS_H_ */

