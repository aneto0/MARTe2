/**
 * @file ThreadInformation.cpp
 * @brief Source file for class ThreadInformation
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

 * @details This source file contains the definition of all the methods for
 * the class ThreadInformation (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ThreadInformation.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ThreadInformation::ThreadInformation() {
    userThreadFunction = static_cast<ThreadFunctionType>(NULL);
    userData = static_cast<void *>(NULL);
    name = static_cast<char8 *>(NULL);
    exceptionHandlerAction = 0u;
    threadId = InvalidThreadIdentifier;
    priorityClass = Threads::UnknownPriorityClass;
    priorityLevel = 0u;
    /*lint -e{534} possible failure is not handled nor propagated.*/
    startThreadSynchSem.Create();
    /*lint -e{534} possible failure is not handled nor propagated.*/
    startThreadSynchSem.Reset();
}

ThreadInformation::ThreadInformation(const ThreadFunctionType threadFunction,
                                     const void * const threadData,
                                     const char8 * const threadName,
                                     const uint32 &threadExceptionHandler) {
    this->userThreadFunction = threadFunction;
    this->userData = threadData;
    this->exceptionHandlerAction = threadExceptionHandler;
    if (threadName != NULL) {
        this->name = MemoryStringDup(threadName);
    }
    else {
        this->name = MemoryStringDup("Unknown");
    }
    threadId = InvalidThreadIdentifier;
    priorityClass = Threads::UnknownPriorityClass;
    priorityLevel = 0u;
    /*lint -e{534} possible failure is not handled nor propagated.*/
    startThreadSynchSem.Create();
    /*lint -e{534} possible failure is not handled nor propagated.*/
    startThreadSynchSem.Reset();
}

/**
 * @brief Copy constructor.
 * @param[in] threadInfo contains informations to initialize this object.
 */
ThreadInformation::ThreadInformation(const ThreadInformation &threadInfo) {
    /*lint -e{1554} pointer to a function.*/
    userThreadFunction = threadInfo.userThreadFunction;
    /*lint -e{1554} the userData is externally constructed managed.*/
    userData = threadInfo.userData;
    name = MemoryStringDup(threadInfo.name);
    exceptionHandlerAction = threadInfo.exceptionHandlerAction;
    threadId = threadInfo.threadId;
    priorityClass = threadInfo.priorityClass;
    priorityLevel = threadInfo.priorityLevel;
    /*lint -e{534} possible failure is not handled nor propagated.*/
    startThreadSynchSem.Create();
    /*lint -e{534} possible failure is not handled nor propagated.*/
    startThreadSynchSem.Reset();
}

/**
 * @brief Destructor.
 * @details It just frees the memory allocated for the name string.
 */
/*lint -e{1551} only C calls are performed. No exception can be raised*/
ThreadInformation::~ThreadInformation() {
    /*lint -e{534} possible failure is not handled nor propagated.*/
    MemoryFree(reinterpret_cast<void *&>(name));
    /*lint -e{534} possible failure is not handled nor propagated.*/
    startThreadSynchSem.Close();
    userData = static_cast<void *>(NULL);
    name = static_cast<char8 *>(NULL);
}

/**
 * @brief Assign operator to copy a thread info in this.
 * @param[in] threadInfo contains informations to initialize this object.
 */
void ThreadInformation::Copy(const ThreadInformation &threadInfo) {
    userThreadFunction = threadInfo.userThreadFunction;
    userData = threadInfo.userData;
    name = MemoryStringDup(threadInfo.name);
    threadId = threadInfo.threadId;
    priorityClass = threadInfo.priorityClass;
    priorityLevel = threadInfo.priorityLevel;
}

/**
 * @brief Launch the function of the thread.
 * The function representing the thread. This is the most basic implementation.
 */
void ThreadInformation::UserThreadFunction() {
    if (userThreadFunction != NULL) {
        userThreadFunction(userData);
    }
}

/**
 * @brief Get the name of the thread.
 * @return A reference to the dynamically allocated string representing the name of the thread.
 */
const char8 *ThreadInformation::ThreadName() {
    return name;
}

/**
 * @brief Call the thread function with an exception handler protection.
 * @param[in] userFunction is the thread function.
 * @param[in] userData is the function argument.
 * @param[in] eh is the exception handler.
 */
bool ThreadInformation::ExceptionProtectedExecute(const ThreadFunctionType userFunction,
                                                  const void * const threadUserData) const {
    userFunction(threadUserData);
    return true;
}

Threads::PriorityClassType ThreadInformation::GetPriorityClass() const {
    return priorityClass;
}

void ThreadInformation::SetPriorityClass(const Threads::PriorityClassType &newPriorityClass) {
    this->priorityClass = newPriorityClass;
}

uint8 ThreadInformation::GetPriorityLevel() const {
    return priorityLevel;
}

void ThreadInformation::SetPriorityLevel(const uint8 &newPriorityLevel) {
    this->priorityLevel = newPriorityLevel;
}

ThreadIdentifier ThreadInformation::GetThreadIdentifier() const {
    return threadId;
}

void ThreadInformation::SetThreadIdentifier(const ThreadIdentifier &newThreadId) {
    this->threadId = newThreadId;
}
