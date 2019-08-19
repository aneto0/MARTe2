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

#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ThreadInformation.h"
#include "HeapManager.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ThreadInformation::ThreadInformation() {
    userThreadFunction = static_cast<ThreadFunctionType>(NULL);
    userData = static_cast<void *>(NULL);
    name = static_cast<char8 *>(NULL);
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
                                     CCString threadName): name(threadName) {
    this->userThreadFunction = threadFunction;
    this->userData = threadData;
    if (threadName.GetSize() == 0) {
    	name = CCString ("Unknown");
    }
    threadId = InvalidThreadIdentifier;
    priorityClass = Threads::UnknownPriorityClass;
    priorityLevel = 0u;
    /*lint -e{534} possible failure is not handled nor propagated.*/
    startThreadSynchSem.Create();
    /*lint -e{534} possible failure is not handled nor propagated.*/
    startThreadSynchSem.Reset();
}

/*lint -e{1551} only C calls are performed. No exception can be raised*/
ThreadInformation::~ThreadInformation() {
    /*lint -e{534} possible failure is not handled nor propagated.*/
    /*lint -e{929} cast required to be able to use Memory::Free interface.*/
    HeapManager::Free(reinterpret_cast<void *&>(name));
    /*lint -e{534} possible failure is not handled nor propagated.*/
    startThreadSynchSem.Close();
    userData = static_cast<void *>(NULL);
    name = static_cast<char8 *>(NULL);
}

void ThreadInformation::Copy(const ThreadInformation &threadInfo) {
    userThreadFunction = threadInfo.userThreadFunction;
    userData = threadInfo.userData;
    name = threadInfo.name;
    threadId = threadInfo.threadId;
    priorityClass = threadInfo.priorityClass;
    priorityLevel = threadInfo.priorityLevel;
}

void ThreadInformation::UserThreadFunction() const {
    if (userThreadFunction != NULL) {
        userThreadFunction(userData);
    }
}

CCString ThreadInformation::ThreadName() const {
    return name.GetList();
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

ErrorManagement::ErrorType ThreadInformation::ThreadWait() {
    return startThreadSynchSem.Wait();
}

bool ThreadInformation::ThreadPost() {
    return startThreadSynchSem.Post();
}

}
