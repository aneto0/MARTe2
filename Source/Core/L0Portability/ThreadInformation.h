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
 * @brief A class which contains all threads informations (name, function, argument, id, priority info).
 *
 * @details This class is useful to the ThreadsDatabase implementation and allow the access
 * to the main thread information.
 *
 * @details Generally these methods and attributes are used for the communication and synchronization between threads,
 * allowing for example to a thread to understand if another thread is still alive or also to kill and terminate
 * other threads.
 */

class ThreadInformation {
public:

    /**
     * @brief The type of function that can be used to instantiate a thread initialization interface object.
     * @param[in] userFunction The thread entry point.
     * @param[in] userData A pointer to data that can be passed to the thread.
     * @param[in] threadName The thread name.
     * @param[in] exceptionHandlerAction Describes the behavior of threads when an exception occurs.
     */
    typedef ThreadInformation *(*ThreadInformationConstructorType)(ThreadFunctionType userFunction,
                                                                   void *userData,
                                                                   const char8 *threadName,
                                                                   uint32 exceptionHandlerAction);

    /**
     * @brief Default constructor.
     */
    ThreadInformation();

    /**
     * @brief Constructor.
     * @param[in] userThreadFunction Actually the thread that has to be executed.
     * @param[in] userData A pointer to a structure containing thread's data.
     * @param[in] name The name of the thread.
     */
    ThreadInformation(ThreadFunctionType userThreadFunction,
                      void *userData,
                      const char8 *name);

    /**
     * @brief Copy constructor.
     * @param[in] threadInfo contains informations to initialize this object.
     */
    ThreadInformation(const ThreadInformation &threadInfo);

    /**
     * @brief copy a thread info in this.
     * @param[in] threadInfo contains informations to initialize this object.
     */
    void Copy(const ThreadInformation &threadInfo);

    /**
     * @brief Destructor.
     * @details It just frees the memory allocated for the name string.
     */
    virtual ~ThreadInformation();

    /**
     * @brief Launch the function of the thread.
     * The function representing the thread. This is the most basic implementation.
     */
    virtual void UserThreadFunction();

    /**
     * @brief Get the name of the thread.
     * @return A reference to the dynamically allocated string representing the name of the thread.
     */
    virtual const char8 *ThreadName();

    /**
     * @brief Call the thread function with an exception handler protection.
     * @param[in] userFunction is the thread function.
     * @param[in] userData is the function argument.
     * @param[in] eh is the exception handler.
     */
    bool ExceptionProtectedExecute(ThreadFunctionType userFunction,
                                   void *userData,
                                   ExceptionHandler *eh);

    /**
     *  @brief The thread waits a post condition.
     */
    inline ErrorType ThreadWait();

    /**
     * @brief Stop waiting condition.
     */
    inline bool ThreadPost();

    /**
     * @brief Returns the thread identifier.
     * @return the thread identifier.
     */
    ThreadIdentifier GetThreadIdentifier() const;

    /**
     * @brief Updates the thread identifier value.
     * @param threadId the thread identifier to be set.
     */
    void SetThreadIdentifier(ThreadIdentifier threadId);

    /**
     * @brief Returns the thread priority class.
     * @return the thread priority class.
     */
    Threads::PriorityClassType GetPriorityClass() const;

    /**
     * @brief Updates the thread priority class value.
     * @param priorityClass the new priority class.
     */
    void SetPriorityClass(Threads::PriorityClassType priorityClass);

    /**
     * @brief Returns the thread priority level.
     * @return the thread priority level.
     */
    Threads::ThreadPriorityType GetPriorityLevel() const;

    /**
     * @brief Updates the thread priority level value.
     * @param priorityLevel the new priority level.
     */
    void SetPriorityLevel(Threads::ThreadPriorityType priorityLevel);

private:

    /** The thread identifier */
    ThreadIdentifier threadId;

    /** The thread priority class */
    Threads::PriorityClassType priorityClass;

    /** The thread priority level */
    Threads::ThreadPriorityType priorityLevel;

    /** The user thread entry point. */
    ThreadFunctionType userThreadFunction;

    /** A pointer to a structure containing thread data. */
    void * userData;

    /** The name of the thread. */
    const char8 * name;

    /** enables the operating system to perform some housekeeping
     * before releasing the thread to the user code. */
    EventSem startThreadSynchSem;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

ErrorType ThreadInformation::ThreadWait() {
    return startThreadSynchSem.Wait();
}

/**
 * @brief Stop waiting condition.
 */
bool ThreadInformation::ThreadPost() {
    return startThreadSynchSem.Post();
}

#endif /* THREADINFORMATION_H_ */

