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

/**
 * @brief Default constructor.
 */
ThreadInformation::ThreadInformation() {
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
 * @brief Constructor.
 * @param[in] userThreadFunction Actually the thread that has to be executed.
 * @param[in] userData A pointer to a structure containing thread's data.
 * @param[in] name The name of the thread.
 */
ThreadInformation::ThreadInformation(ThreadFunctionType userThreadFunction,
                                     void *userData,
                                     const char8 *name) {
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
 * @brief Copy constructor.
 * @param[in] threadInfo contains informations to initialize this object.
 */
ThreadInformation::ThreadInformation(const ThreadInformation &threadInfo) {
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
 * @brief Destructor.
 * @details It just frees the memory allocated for the name string.
 */
ThreadInformation::~ThreadInformation() {
    MemoryFree((void *&) name);
    startThreadSynchSem.Close();
}

/**
 * @brief Assign operator to copy a thread info in this.
 * @param[in] threadInfo contains informations to initialize this object.
 */
void ThreadInformation::operator=(const ThreadInformation &threadInfo) {
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
bool ThreadInformation::ExceptionProtectedExecute(ThreadFunctionType userFunction,
                                                  void *userData,
                                                  ExceptionHandler *eh) {
    userFunction(userData);
    return true;
}

