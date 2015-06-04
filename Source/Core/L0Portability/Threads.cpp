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

#include "GeneralDefinitions.h"
#include "Threads.h"
#include "ProcessorType.h"
#include "ThreadsDatabase.h"

#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,ThreadsOS.h)

void __thread_decl SystemThreadFunction(void *threadData)
{
    ThreadInformation *threadInfo = (ThreadInformation *)threadData;
    if(threadInfo == NULL)
    {
        return;
    }

    ThreadsDatabase::Lock();
    threadInfo->threadId=Threads::Id();
    ThreadsDatabase::NewEntry(threadInfo);
    ThreadsDatabase::UnLock();

    threadInfo->priorityLevel = Threads::PRIORITY_NORMAL;
    Threads::SetPriorityClass(Threads::Id(), Threads::PRIORITY_CLASS_NORMAL);
    //Guarantee that the OS finishes the housekeeping before releasing the thread to the user
    threadInfo->ThreadWait();
    //Start the user thread
    threadInfo->UserThreadFunction();

    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo2 = ThreadsDatabase::RemoveEntry(Threads::Id());
    ThreadsDatabase::UnLock();

    if (threadInfo != threadInfo2)
    {
        //CStaticAssertErrorCondition(FatalError,"SystemThreadFunction TDB_RemoveEntry returns wrong threadInfo \n");
    }

    delete threadInfo;
}

ThreadInformation * DefaultThreadInformationConstructor(
        ThreadFunctionType userThreadFunction, void *userData,
        const char *threadName, uint32 exceptionHandlerBehaviour) {

    return new ThreadInformation(userThreadFunction, userData, threadName);
}

/// Allows to set the thread initialisation method
void ThreadsSetInitialisationInterfaceConstructor(
        ThreadInformationConstructorType threadInfoc) {
    if (threadInfoc != NULL) {
        ::threadInitialisationInterfaceConstructor = threadInfoc;
    }
    else {
        ::threadInitialisationInterfaceConstructor =
                DefaultThreadInformationConstructor;
    }
}

/** Assign default initialisation. */
ThreadInformationConstructorType threadInitialisationInterfaceConstructor =
        DefaultThreadInformationConstructor;

/** This function allows to call a subroutine within an exception handler protection */
bool ThreadProtectedExecute(ThreadFunctionType userFunction, void *userData,
                            ExceptionHandler *ehi) {
    TID threadId = Threads::Id();
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(
            threadId);
    ThreadsDatabase::UnLock();
    if (threadInfo != NULL) {
        return threadInfo->ExceptionProtectedExecute(userFunction, userData,
                                                     ehi);
    }
    return False;
}

TID ThreadsBeginThread(ThreadFunctionType function, void *parameters,
                       uint32 stacksize, const char *name,
                       uint32 exceptionHandlerBehaviour,
                       ProcessorType runOnCPUs) {

    if (runOnCPUs == PTUndefinedCPUs) {
        if (ProcessorType::GetDefaultCPUs() != 0) {
            runOnCPUs = ProcessorType::GetDefaultCPUs();
        }
        else {
            runOnCPUs = 0xff;
        }
    }

    if (threadInitialisationInterfaceConstructor == NULL) {
        //CStaticAssertErrorCondition(ParametersError,"Threads::ThreadsBeginThread (%s) threadInitialisationInterfaceConstructor is NULL", name);
        return (TID) - 1;
    }
    ThreadInformation *threadInfo = threadInitialisationInterfaceConstructor(
            function, parameters, name, exceptionHandlerBehaviour);
    if (threadInfo == NULL) {
        //CStaticAssertErrorCondition(InitialisationError,"Threads::ThreadsBeginThread (%s) threadInitialisationInterfaceConstructor returns NULL", name);
        return (TID) - 1;
    }

    TID threadId = ThreadsOSBeginThread(
            (StandardThreadFunction) SystemThreadFunction, threadInfo,
            stacksize, runOnCPUs);
    //Enable the user thread to run...
    threadInfo->ThreadPost();
    return threadId;
}

TID ThreadsId() {
    return ThreadsOSId();
}

bool ThreadsIsAlive(TID threadId) {
    ThreadsDatabase::Lock();
    bool condition = (ThreadsDatabase::GetThreadInformation(threadId) != NULL);
    ThreadsDatabase::UnLock();
    if (condition) {
        return ThreadsOSIsAlive(threadId);
    }
    else {
        return False;
    }
}

bool ThreadsKill(TID threadId) {
    if (!ThreadsIsAlive(threadId)) {
        return False;
    }
    ThreadsDatabase::Lock();
    ThreadsDatabase::RemoveEntry(threadId);
    ThreadsDatabase::UnLock();
    return ThreadsOSKill(threadId);
}

void ThreadsSetPriorityLevel(TID threadId, uint32 priorityLevel) {
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(
            threadId);
    if (threadInfo != NULL) {
        threadInfo->priorityLevel = priorityLevel;
        ThreadsOSSetPriorityLevel(threadId, threadInfo->priorityClass,
                                  priorityLevel);
    }
    ThreadsDatabase::UnLock();

}

void ThreadsSetPriorityClass(TID threadId, uint32 priorityClass) {
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(
            threadId);
    if (threadInfo != NULL) {
        threadInfo->priorityClass = priorityClass;
        ThreadsOSSetPriorityLevel(threadId, priorityClass,
                                  threadInfo->priorityLevel);
    }
    ThreadsDatabase::UnLock();
}

int32 ThreadsGetCPUs(TID threadId) {
    return ThreadsOSGetCPUs(threadId);
}

uint32 ThreadsGetPriorityLevel(TID threadId) {
    uint32 priorityLevel = Threads::PRIORITY_UNKNOWN;
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(
            threadId);
    if (threadInfo != NULL) {
        priorityLevel = threadInfo->priorityLevel;
    }
    ThreadsDatabase::UnLock();
    return priorityLevel;
}

uint32 ThreadsGetPriorityClass(TID threadId) {
    uint32 priorityClass = Threads::PRIORITY_CLASS_UNKNOWN;
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(
            threadId);
    if (threadInfo != NULL) {
        priorityClass = threadInfo->priorityClass;
    }
    ThreadsDatabase::UnLock();
    return priorityClass;
}

uint32 ThreadsGetState(TID threadId) {
    return ThreadsOSGetState(threadId);
}

void ThreadsEndThread() {
    ThreadsDatabase::Lock();
    ThreadsDatabase::RemoveEntry(Threads::Id());
    ThreadsDatabase::UnLock();
    ThreadsOSEndThread();
}

const char *ThreadsName(TID threadId) {
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(
            threadId);
    if (threadInfo != NULL) {
        ThreadsDatabase::UnLock();
        return threadInfo->ThreadName();
    }
    ThreadsDatabase::UnLock();
    return NULL;
}

