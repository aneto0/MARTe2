/**
 * @file Threads.h
 * @brief Header file for module Threads
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

 * @details This header file contains the declaration of the module Threads
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef THREADS_H_
#define THREADS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ExceptionHandler.h"
#include "Threads.h"
#include "GeneralDefinitions.h"
#include "ProcessorType.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Forward declaration of a thread information database.
 */
class ThreadInformation;

/**
 * Definition of the thread callback function.
 */
typedef void (*ThreadFunctionType)(const void * const parameters);

/**
 * @brief Threads management module.
 *
 * @details These module allows to create, manage and monitor the life-cycle of any framework thread/
 * The default stack size associated to a thread THREADS_DEFAULT_STACKSIZE is defined in GeneralDefinitionsOS.h
 * for each different operating system.
 */
namespace Threads {

/**
 * @brief Possible thread states. This is the type returned by Threads::GetThreadState
 */
enum ThreadStateType {
    /**
     * Unknown state.
     */
    UnknownThreadStateType = 0,

    /**
     * Ready to run.
     */
    ReadyState,

    /**
     * Suspended.
     */
    SuspendedState,

    /**
     * Blocked or waiting on a semaphore.
     */
    BlockedState,

    /**
     * Running state.
     */
    RunningState,

    /**
     * Dead (should no longer be available).
     */
    DeadState
};

/**
 * @brief Available priority classes.
 * @details The thread priority is defined by a priority class and a priority level.
 * The priority class defines the policy and the level defines the rank of the priority
 * inside a given class.
 */
enum PriorityClassType {
    /**
     * Unknown priority class.
     */
    UnknownPriorityClass = 0,

    /**
     * Lowest priority class.
     */
    IdlePriorityClass,

    /**
     * Normal priority class. Higher than Idle.
     */
    NormalPriorityClass,

    /**
     * Real-time priority class. This is the highest possible priority/
     */
    RealTimePriorityClass
};

/**
 * @brief Changes the thread priority level for the already set priority class.
 * @details The currently set priority class (GetPriorityClass()) will not be changed.
 * @param[in] threadId the thread identifier.
 * @param[in] priorityClass the class to assign to the thread.
 * @param[in] priorityLevel the priority level to be set. This must be a
 * value between 0 and 15.
 */
void SetPriority(const ThreadIdentifier &threadId,
                 const PriorityClassType &priorityClass,
                 const uint8 &priorityLevel);

/**
 * @brief Start a new thread.
 * @details This function will dynamically allocate an object of type
 * ThreadInformation using the function hook ThreadInformationConstructor.
 * This allows the programmer to choose which constructor has to be used in the case
 * a ThreadInformation derived class has been used.
 * @param[in] function the callback function for the thread.
 * @param[in] parameters a pointer passed to the thread callback function.
 * @param[in] stacksize size of the stack.
 * @param[in] name name of the thread.
 * @param[in] exceptionHandlerBehaviour action to perform when an exception occurs.
 * @param[in] runOnCPUs cpu mask where the thread can be executed.
 * @return The thread identification number.
 */
ThreadIdentifier BeginThread(const ThreadFunctionType function,
                             const void * const parameters = static_cast<void *>(NULL),
                             const uint32 &stacksize = static_cast<uint32>(THREADS_DEFAULT_STACKSIZE),
                             const char8 * name = static_cast<char8*>(NULL),
                             uint32 exceptionHandlerBehaviour = ExceptionHandler::NotHandled,
                             ProcessorType runOnCPUs = UndefinedCPUs);

/**
 * @brief Gets the current thread id;
 * @return the current thread id.
 */
ThreadIdentifier Id(void);

/**
 * @brief Asynchronously kill a thread.
 * @details Allows to force the termination of a thread.
 * @param[in] threadId the id of the thread to kill.
 * @return true if system level kill function returns without errors.
 */
bool Kill(const ThreadIdentifier &threadId);

/**
 * @brief Checks if a thread is still alive.
 * @param[in] threadId the id of the thread which must be checked.
 * @return true if the thread is still alive , false otherwise.
 */
bool IsAlive(const ThreadIdentifier &threadId);

/**
 * @brief Returns the thread name.
 * @param[in] threadId the id of the thread,
 * @return the name of the thread.
 */
const char8 *Name(const ThreadIdentifier &threadId);

/**
 * @brief Returns the task state.
 * @param[in] threadId the id of the requested thread.
 * @return the thread state(s).
 */
ThreadStateType GetState(const ThreadIdentifier &threadId);

/**
 * @brief Returns the priority level of a thread.
 * @param[in] threadId is the id of the requested thread.
 * @return the thread priority level.
 */
uint8 GetPriorityLevel(const ThreadIdentifier &threadId);

/**
 * @brief Returns the priority class of a thread.
 * @param[in] threadId is the id of the requested thread.
 * @return the thread priority class.
 */
PriorityClassType GetPriorityClass(const ThreadIdentifier &threadId);

/**
 * @brief Returns the CPU mask associated to the specified thread.
 * @param[in] threadId is the thread identifier.
 * @return the CPU mask or 0 if the information cannot be retrieved.
 */
uint32 GetCPUs(const ThreadIdentifier &threadId);

/**
 * @brief Returns the id of the n-th thread in the database.
 * @param[in] n the thread index.
 * @return the id of the n-th thread in the database, -1 if the database is empty.
 */
ThreadIdentifier FindByIndex(const uint32 &n);

/**
 * @brief Returns the number of threads currently in the database.
 * @return the number of threads currently in the database.
 */
uint32 NumberOfThreads();

/**
 * @brief Get a copy of the thread information stored in the database.
 * @param[out] copy is the thread information structure in return.
 * @param[in] n is the threads index, if it is <0 is ignored.
 * @return true if the requested element is in the database, false otherwise.
 */
bool GetThreadInfoCopy(ThreadInformation & copy,
                       const uint32 &n);

/**
 * @brief Get a copy of the thread information stored in the database.
 * @param[out] copy is the thread information structure in return.
 * @param[in] threadId is the thread identifier.
 * @return true if the requested element is in the database, false otherwise.
 */
bool GetThreadInfoCopy(ThreadInformation & copy,
                       const ThreadIdentifier &threadId);

/**
 * @brief Search the thread with the specified name.
 * @param[in] name is the thread name.
 * @return the id of the first found thread with the specified name.
 */
ThreadIdentifier FindByName(const char8 * const name);

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADS_H_ */
