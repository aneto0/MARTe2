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
 * @brief Information to be stored associated with a specific thread
 */
#ifndef THREAD_INITIALISATION_H
#define THREAD_INITIALISATION_H

#include "EventSem.h"
#include "Memory.h"
#include "ExceptionHandler.h"




/** The type of a function that can be used for a thread. */
typedef void (*ThreadFunctionType)(void *parameters);

/** This class is an interface used to implement a common thread initialisation procedure.
 *  Despite the name this class is also provides a minimal set of
 *  functionalities. For this reason has not been made pure virtual and can
 *  be istantiated.
 */


/** @brief A class which contains all threads informations (name, function, argument, id, priority info).
  * 
  * This class is useful to the ThreadsDatabase implementation and allow the access
  * to the main thread information.
  *
  * Generally this methods and attributes are used for the comunication and syncronization between threads,
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
    /** the thread number */
    TID threadId;

    /** The thread priority class */
    uint32 priorityClass;

    /** The thread priority level */
    uint32 priorityLevel;

    /** @brief Default constructor. */
    ThreadInformation() {
        userThreadFunction = NULL;
        userData = NULL;
        name = NULL;
        threadId = (TID) 0;
        priorityClass = 0;
        priorityLevel = 0;
        startThreadSynchSem.Create();
        startThreadSynchSem.Reset();
    }

    /** @brief Constructor. 
      * @param threadInfo contains informations to initialize this object. */
    ThreadInformation(ThreadInformation &threadInfo) {
        userThreadFunction = threadInfo.userThreadFunction;
        userData = threadInfo.userData;
        name = MemoryStringDup(threadInfo.name);
        threadId = threadInfo.threadId;
        priorityClass = threadInfo.priorityClass;
        priorityLevel = threadInfo.priorityLevel;
    }

    /** @brief Equal operator to copy a thread info in this. 
      * @param threadInfo contains informations to initialize this object. */
    void operator=(ThreadInformation &threadInfo) {
        userThreadFunction = threadInfo.userThreadFunction;
        userData = threadInfo.userData;
        name = MemoryStringDup(threadInfo.name);
        threadId = threadInfo.threadId;
        priorityClass = threadInfo.priorityClass;
        priorityLevel = threadInfo.priorityLevel;
    }

    /** Constructor.
      * @param userThreadFunction Actually the thread that has to be executed.
      * @param userData A pointer to a structure containing thread data.
      * @param name The name of the thread. */
    ThreadInformation(ThreadFunctionType userThreadFunction, void *userData,
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
        priorityClass = 0;
        priorityLevel = 0;
        startThreadSynchSem.Create();
        startThreadSynchSem.Reset();
    }

    /** @brief Destructor. 
      * It just frees the memory allocated for the name string. */
    virtual ~ThreadInformation() {
        MemoryFree((void *&) name);
        startThreadSynchSem.Close();
    }
    ;

    /** @brief Launch the function of the thread.
      * The function representing the thread. This is the most basic implementation. */
    virtual void UserThreadFunction() {
        if (userThreadFunction != NULL)
            userThreadFunction(userData);
    }

    /** @brief Get the name of the thread.
      * @return A reference to the dynamically allocated string representing the name of the thread.
      * An internal structure that can potentially be referenced after the destruction of the object. */
    virtual const char *ThreadName() {
        return name;
    }

    /** @brief Call the thread function with an exception handler protection. */
    bool ExceptionProtectedExecute(ThreadFunctionType userFunction,
                                   void *userData, ExceptionHandler *eh) {
        userFunction(userData);
        return True;
    }

    /** @brief The thread wait a post condition. */
    inline void ThreadWait() {
        startThreadSynchSem.Wait();
    }

    /** @brief If the thread was in waiting now it can proceed its execution. */
    inline void ThreadPost() {
        startThreadSynchSem.Post();
    }
};

/** The type of function that can be used to instantiate a thread initialization interface object.
 * @param userFunction The thread entry point.
 * @param userData A pointer to data that can be passed to the thread.
 * @param threadName The thread name.
 * @param exceptionHandlerAction Describes the behaviour of threads when an exception occurr. @see ExceptionHandler
 */
typedef ThreadInformation *(*ThreadInformationConstructorType)(
        ThreadFunctionType userFunction, void *userData, const char *threadName,
        uint32 exceptionHandlerAction);

#endif

