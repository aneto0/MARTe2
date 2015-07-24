/**
 * @file Threads.h
 * @brief Header file for class Threads
 * @date 17/06/2015
 * @author Giuseppe Ferr�
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

 * @details This header file contains the declaration of the class Threads
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef THREADS_H_
#define           THREADS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
//#include "ThreadInformation.h"
#include "ExceptionHandler.h"
#include "ProcessorType.h"

/*---------------------------------------------------------------------------*/
/*                        Friend method definitions                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


/** a database of info related to a thread */
class ThreadInformation;

/** The type of a function that can be used for a thread. */
typedef void (*ThreadFunctionType)(void *parameters);


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
 * @details Most of the implementation is delegated to ThreadsOS.h which provides system level functions for threads management.\n
 * The default stack size associated to a thread THREADS_DEFAULT_STACKSIZE is defined in GeneralDefinitionsOS.h for each different OS.
 */
class Threads {

public:
    /** Possible thread states. This is the type returned by Threads::GetThreadState */
    enum ThreadStateType {
        /**
         * Unknown state.
         */
        STATE_UNKNOWN = -1,

        /**
         * Ready state.
         */
        STATE_READY = 1024,

        /**
         * Pendent state.
         */
        STATE_PEND = 512,

        /**
         * Suspended state.
         */
        STATE_SUSP = 256,

        /**
         * Blocked state.
         */
        STATE_BLOCKED = 2,

        /**
         * Semaphore waiting state.
         */
        STATE_SEM = 4,

        /**
         * Delay state.
         */
        STATE_DELAY = 8,

        /**
         * Tout state.
         */
        STATE_TOUT = 16,

        /**
         * Run state.
         */
        STATE_RUN = 32,

        /**
         * Dead state.
         */
        STATE_DEAD = 64

    } ;

    /** Possible priority classes. This is the type returned by Threads::GetPriorityClass and it is used to set the priority class in Threads::SetPriorityClass */
    enum PriorityClassType{

        /**
         * Unknown Class Priority 0.
         */
        PRIORITY_CLASS_UNKNOWN = 0,

        /**
         * Idle Class Priority 1.
         */
        PRIORITY_CLASS_IDLE = 1,

        /**
         * Normal Class Priority 2.
         */
        PRIORITY_CLASS_NORMAL = 2,

        /**
         * High Class Priority 3.
         */
        PRIORITY_CLASS_HIGH = 3,

        /**
         * Real Time Class Priority 4.
         */
        PRIORITY_CLASS_REAL_TIME = 4
    } ;

    /** Possible thread priority levels. This is the type returned by Threads::GetPriorityLevel and it is used to set the thread priority in Threads::SetPriorityLevel */
    enum ThreadPriorityType{
        /**
         * Unknown Thread Priority 0.
         */
        PRIORITY_UNKNOWN = 0,

        /**
         * Idle Thread Priority 1.
         */
        PRIORITY_IDLE = 1,

        /**
         * Lowest Thread Priority 2.
         */
        PRIORITY_LOWEST = 2,

        /**
         * Below Normal Thread Priority 3.
         */
        PRIORITY_BELOW_NORMAL = 3,

        /**
         * Normal Thread Priority 4.
         */
        PRIORITY_NORMAL = 4,
        /**
         * Above Normal Thread Priority 5.
         */
        PRIORITY_ABOVE_NORMAL = 5,

        /**
         * Highest Thread Priority 6.
         */
        PRIORITY_HIGHEST = 6,

        /**
         * Time Critical Thread Priority.
         */
        PRIORITY_TIME_CRITICAL = 7

    } ;

private:

    /**
     * @brief Called by Threads::SetPriorityLevel & SetPriorityClass
     *
     * @details In linux the priority will vary between 33, i.e. priorityClass = IDLE_PRIORITY_CLASS
     * and priorityLevel = PRIORITY_IDLE and 99, i.e. priorityClass = REAL_TIME_PRIORITY_CLASS
     * and priorityLevel = PRIORITY_TIME_CRITICAL
     *
     * @param[in] threadId is the thread identifier.
     * @param[in] priorityClass is the desired class priority to assign to the thread.
     * @param[in] priorityLevel is the desired thread priority to assign to the thread.
     */
    static void SetPriorityLevelAndClass(
                        const ThreadIdentifier    threadId,
                        const PriorityClassType   priorityClass,
                        const ThreadPriorityType  priorityLevel);

public:

    /**
     * @brief Change thread priority.
     * @param[in] tid is the thread identifier.
     * @param[in] level is the level to assign to the thread.
     */
    static void SetPriorityLevel(
                        const ThreadIdentifier    &threadId,
                        const ThreadPriorityType   priorityLevel)
    {
        SetPriorityLevelAndClass(threadId, PRIORITY_CLASS_UNKNOWN, priorityLevel);
    }
    /**
     * @brief Change thread priority class.
     * @param[in] tid is the thread identifier.
     * @param[in] priorityClass is the class to assign to the thread.
     */
    static void SetPriorityClass(
                        const ThreadIdentifier    &threadId,
                        const PriorityClassType    priorityClass){
        SetPriorityLevelAndClass(threadId, priorityClass, PRIORITY_UNKNOWN);
    }

    /**
     * @brief Called implicitly at the end of the main thread function.
     * @details Calling this leaves some allocated memory unfreed.
     */
    static void EndThread();

    /**
     * @brief A call to this function will start a thread.
     *
     * @details This function will dynamically allocate an object of type
     * ThreadInformation using the function hook ThreadInformationConstructor.
     * This allows the programmer to choose which constructor has to be used in the case
     * a ThreadInformation derived class has been used.
     *
     * @param[in] function The function main for the thread.
     * @param[in] parameters A pointer passed to the thread main function.
     * @param[in] stacksize The size of the stack.
     * @param[in] name The name of the thread.
     * @param[in] exceptionHandlerBehaviour The action to perform when an exception occurs.
     * @param[in] runOnCPUs The cpu mask where the thread can be executed.
     * @return The thread identification number.
     */
    static ThreadIdentifier BeginThread(
                           ThreadFunctionType    function,
                           void *                parameters                  = static_cast<void *>(NULL),
                           uint32                stacksize                   = static_cast<uint32>(THREADS_DEFAULT_STACKSIZE),
                           const char8 *         name                        = static_cast<char8* >(NULL),
                           uint32                exceptionHandlerBehaviour   = ExceptionHandler::NotHandled,
                           ProcessorType         runOnCPUs                   = ProcessorType::UndefinedCPUs);

    /**
     * @brief Gets the current thread id;
     * @return the current thread id.
     */
    static ThreadIdentifier Id(void);

    /** @brief Asynchronous thread kill.
     *
     * @details A thread cannot be killed in critical sections in Linux.
     * @param[in] tid is the id of the thread to kill.
     * @return true if system level kill function returns without errors.
     */
    static bool Kill(ThreadIdentifier threadId);

    /**
     * @brief Check if thread is still alive.
     * @param[in] tid is the id of the thread which must be checked.
     * @return true if thread is alive (checking before that it is in the database), false otherwise.
     */
    static bool IsAlive(ThreadIdentifier threadId);

    /**
     * @brief Get thread name.
     * @param[in] tid is the id of the thread,
     * @return the name of the thread with tid as id.
     */
    static const char8 *Name(ThreadIdentifier threadId);

    /**
     * @brief Returns the task state.
     *
     *@details The returned value is a ThreadStateType type.
     *
     * @param[in] tid is the id of the requested thread.
     * @return the thread state(s).
     */
    static ThreadStateType GetState(ThreadIdentifier threadId);

    /**
     * @brief Get the priority level of a thread.
     * @param[in] tid is the id of the requested thread.
     * @return the thread priority level.
     */
    static ThreadPriorityType GetPriorityLevel(ThreadIdentifier threadId);

    /**
     * @brief Get the priority class of a thread.
     * @param[in] tid is the id of the requested thread.
     * @return the thread priority class.
     */
    static PriorityClassType GetPriorityClass(ThreadIdentifier threadId);

    /**
     * @brief Get the cpu related to a specified thread.
     * @param[in] tid is the thread identifier.
     * @return the number if the cpu associated to the specified thread.
     */
    static int32 GetCPUs(ThreadIdentifier threadId);

    /**
     * @brief Executes the function specified.
     * @param[in] userFunction is the function to be called.
     * @param[in] userData is the function argument.
     * @param[in] ehi is the exception handler.
     * @return true.
     * @see ThreadInformation::ExceptionProtectedExecute.
     */
    static bool ProtectedExecute(
            ThreadFunctionType                 userFunction,
            void *                             userData,
            ExceptionHandler *                 ehi);
    /**
     * @brief Returns the id of the n-th thread in the database.
     * @param[in] n is the thread index.
     * @return the id of the n-th thread in the database, -1 if the database is empty.
     */
    static ThreadIdentifier FindByIndex(uint32 n);

    /**
     * @brief Returns the number of threads currently in the database.
     * @return the number of threads currently in the database.
     */
    static uint32 NumberOfThreads();

    /**
     * @brief Get a copy of the thread information stored in the database.
     * @param[out] copy is the thread information structure in return.
     * @param[in] n is the threads index, if it is <0 is ignored.
     * @param[in] tid is the thread identifier.
     * @return true if the requested element is in the database, false otherwise.
     */
    static bool GetThreadInfoCopy(ThreadInformation &   copy,
                                  int32                 n,
                                  ThreadIdentifier      threadId);
    /**
     * @brief Search the thread with the specified name.
     * @param[in] name is the thread name.
     * @return the id of the first found thread with the specified name.
     */
    static ThreadIdentifier FindByName(const char8 *name);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


#endif /* THREADS_H_ */

