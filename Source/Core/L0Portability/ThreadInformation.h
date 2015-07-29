/**
 * @file ThreadInformation.h
 * @brief Header file for class ThreadInformation
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

 * @details This header file contains the declaration of the class ThreadInformation
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef THREADINFORMATION_H_
#define THREADINFORMATION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EventSem.h"
#include "Memory.h"
#include "ExceptionHandler.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class which stores information about a thread: name, function, argument, identifier and priority related information.
 */
class ThreadInformation {

public:

    /**
     * @brief Default constructor.
     */
    ThreadInformation();

    /**
     * @brief Constructor.
     * @param[in] threadFunction the user callback function.
     * @param[in] threadData a pointer to a structure containing user thread data.
     * @param[in] threadName the name of the thread.
     */
    ThreadInformation(const ThreadFunctionType threadFunction,
                      const void * const threadData,
                      const char8 * const threadName);

    /**
     * @brief Copy a thread information into this.
     * @param[in] threadInfo source ThreadInformation object.
     */
    void Copy(const ThreadInformation &threadInfo);

    /**
     * @brief Destructor.
     * @details It frees the memory allocated for the name string.
     */
    ~ThreadInformation();

    /**
     * @brief Executes the user callback function.
     */
    void UserThreadFunction() const;

    /**
     * @brief Returns the name of the thread.
     * @return A reference to the dynamically allocated string containing the name of the thread.
     */
    const char8 *ThreadName () const;

    /**
     *  @brief Locks an internal EventSem.
     *  @details An internal EventSem is used by the Threads implementation to guarantee
     *  that all the necessary housekeeping is performed before calling the user callback function.
     *  @return the value returned by EventSem::Wait
     */
    ErrorType ThreadWait();

    /**
     *  @brief Post an internal EventSem.
     *  @details An internal EventSem is used by the Threads implementation to guarantee
     *  that all the necessary housekeeping is performed before calling the user callback function.
     *  @return the value returned by EventSem::Post
     */
    bool ThreadPost();

    /**
     * @brief Returns the thread identifier.
     * @return the thread identifier.
     */
    ThreadIdentifier GetThreadIdentifier() const;

    /**
     * @brief Updates the thread identifier value.
     * @param newThreadId the thread identifier to be set.
     */
    void SetThreadIdentifier(const ThreadIdentifier &newThreadId);

    /**
     * @brief Returns the thread priority class.
     * @return the thread priority class.
     */
    Threads::PriorityClassType GetPriorityClass() const;

    /**
     * @brief Updates the thread priority class value.
     * @param newPriorityClass the new priority class.
     */
    void SetPriorityClass(const Threads::PriorityClassType &newPriorityClass);

    /**
     * @brief Returns the thread priority level.
     * @return the thread priority level.
     */
    uint8 GetPriorityLevel() const;

    /**
     * @brief Updates the thread priority level value.
     * @param newPriorityLevel the new priority level.
     */
    void SetPriorityLevel(const uint8 &newPriorityLevel);

private:

    /**
     * The thread identifier
     */
    ThreadIdentifier threadId;

    /**
     * The thread priority class
     */
    Threads::PriorityClassType priorityClass;

    /**
     * The thread priority level
     */
    uint8 priorityLevel;

    /**
     * The user thread callback function.
     */
    ThreadFunctionType userThreadFunction;

    /**
     * A pointer to a structure containing thread data.
     * The life-cycle of this structure is externally constructed and managed.
     */
    const void * userData;

    /**
     * The name of the thread.
     */
    char8 * name;

    /**
     * Enables the operating system to perform some housekeeping
     * before releasing the thread to the user callback function.
     */
    EventSem startThreadSynchSem;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADINFORMATION_H_ */

