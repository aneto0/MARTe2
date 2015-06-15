/**
 * @file ThreadInformation.h
 * @brief Header file for class ThreadInformation
 * @date 11/06/2015
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
 * @details This header file contains the declaration of the class ThreadInformation
 * (all of its public, protected and private members). It may also include
 * definitions for inline and friend methods which need to be visible to
 * the compiler.
 */

#ifndef THREADINFORMATION_H_
#define 		THREADINFORMATION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EventSem.h"
#include "Memory.h"
#include "ExceptionHandler.h"

/** Possible thread states. This is the type returned by Threads::GetThreadState */
typedef enum {
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

} ThreadStateType;

/** Possible priority classes. This is the type returned by Threads::GetPriorityClass and it is used to set the priority class in Threads::SetPriorityClass */
typedef enum {

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

} PriorityClassType;

/** Possible thread priority levels. This is the type returned by Threads::GetPriorityLevel and it is used to set the thread priority in Threads::SetPriorityLevel */
typedef enum {
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

} ThreadPriorityType;

/** The type of a function that can be used for a thread. */
typedef void (*ThreadFunctionType)(void *parameters);

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
protected:
    /** The user thread entry point. */
    ThreadFunctionType userThreadFunction;

    /** A pointer to a structure containing thread data. */
    void *userData;

    /** The name of the thread. */
    const char *name;

    /** enables the operating system to perform some housekeeping
     * before releasing the thread to the user code. */
    EventSem startThreadSynchSem;

public:
    /** The thread identifier */
    TID threadId;

    /** The thread priority class */
    PriorityClassType priorityClass;

    /** The thread priority level */
    ThreadPriorityType priorityLevel;

    /**
     * @brief Default constructor.
     */
    ThreadInformation() {
        userThreadFunction = NULL;
        userData = NULL;
        name = NULL;
        threadId = (TID) 0;
        priorityClass = PRIORITY_CLASS_UNKNOWN;
        priorityLevel = PRIORITY_UNKNOWN;
        startThreadSynchSem.Create();
        startThreadSynchSem.Reset();
    }

    /**
     * @brief Copy constructor.
     * @param[in] threadInfo contains informations to initialize this object.
     */
    ThreadInformation(ThreadInformation &threadInfo) {
        userThreadFunction = threadInfo.userThreadFunction;
        userData = threadInfo.userData;
        name = MemoryStringDup(threadInfo.name);
        threadId = threadInfo.threadId;
        priorityClass = threadInfo.priorityClass;
        priorityLevel = threadInfo.priorityLevel;
        startThreadSynchSem.Create();
        startThreadSynchSem.Reset();
    }

    /**
     * @brief Equal operator to copy a thread info in this.
     * @param[in] threadInfo contains informations to initialize this object.
     */
    void operator=(ThreadInformation &threadInfo) {
        userThreadFunction = threadInfo.userThreadFunction;
        userData = threadInfo.userData;
        name = MemoryStringDup(threadInfo.name);
        threadId = threadInfo.threadId;
        priorityClass = threadInfo.priorityClass;
        priorityLevel = threadInfo.priorityLevel;
    }

    /**
     * @brief Constructor.
     * @param[in] userThreadFunction Actually the thread that has to be executed.
     * @param[in] userData A pointer to a structure containing thread data.
     * @param[in] name The name of the thread.
     */
    ThreadInformation(ThreadFunctionType userThreadFunction,
                      void *userData,
                      const char *name) {
        this->userThreadFunction = userThreadFunction;
        this->userData = userData;
        if (name != NULL) {
            this->name = MemoryStringDup(name);
        }
        else {
            this->name = MemoryStringDup("Unknown");
        }
        threadId = (TID) 0;
        priorityClass = PRIORITY_CLASS_UNKNOWN;
        priorityLevel = PRIORITY_UNKNOWN;
        startThreadSynchSem.Create();
        startThreadSynchSem.Reset();
    }

    /**
     * @brief Destructor.
     * @details It just frees the memory allocated for the name string.
     */
    virtual ~ThreadInformation() {
        MemoryFree((void *&) name);
        startThreadSynchSem.Close();
    }
    ;

    /**
     * @brief Launch the function of the thread.
     * The function representing the thread. This is the most basic implementation.
     */
    virtual void UserThreadFunction() {
        if (userThreadFunction != NULL)
            userThreadFunction(userData);
    }

    /**
     * @brief Get the name of the thread.
     * @return A reference to the dynamically allocated string representing the name of the thread.
     */
    virtual const char *ThreadName() {
        return name;
    }

    /**
     * @brief Call the thread function with an exception handler protection.
     * @param[in] userFunction is the thread function.
     * @param[in] userData is the function argument.
     * @param[in] eh is the exception handler.
     */
    bool ExceptionProtectedExecute(ThreadFunctionType userFunction,
                                   void *userData,
                                   ExceptionHandler *eh) {
        userFunction(userData);
        return True;
    }

    /**
     *  @brief The thread waits a post condition.
     * */
    inline void ThreadWait() {
        startThreadSynchSem.Wait();
    }

    /**
     * @brief Stop waiting condition.
     */
    inline void ThreadPost() {
        startThreadSynchSem.Post();
    }
};

/**
 * @brief The type of function that can be used to instantiate a thread initialization interface object.
 * @param[in] userFunction The thread entry point.
 * @param[in] userData A pointer to data that can be passed to the thread.
 * @param[in] threadName The thread name.
 * @param[in] exceptionHandlerAction Describes the behaviour of threads when an exception occurs.
 */
typedef ThreadInformation *(*ThreadInformationConstructorType)(ThreadFunctionType userFunction,
                                                               void *userData,
                                                               const char *threadName,
                                                               uint32 exceptionHandlerAction);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Friend method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADINFORMATION_H_ */

