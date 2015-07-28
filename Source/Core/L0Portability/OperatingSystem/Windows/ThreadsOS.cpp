/**
 * @file ThreadsOS.cpp
 * @brief Source file for class ThreadsOS
 * @date 27/lug/2015
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
 * the class ThreadsOS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Threads.h"
#include "ThreadInformation.h"
#include "ThreadsDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static void SystemThreadFunction(void *threadData) {
    ThreadInformation *threadInfo = (ThreadInformation *) threadData;
    if (threadInfo == NULL) {
        return;
    }

    ThreadsDatabase::Lock();
    threadInfo->threadId = Threads::Id();
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

    if (threadInfo != threadInfo2) {
        //CStaticAssertErrorCondition(FatalError,"SystemThreadFunction TDB_RemoveEntry returns wrong threadInfo \n");
    }

    delete threadInfo;
}

static ThreadInformation * threadInitialisationInterfaceConstructor(ThreadFunctionType userThreadFunction,
                                                                    void *userData,
                                                                    const char8 *threadName,
                                                                    uint32 exceptionHandlerBehaviour) {

    return new ThreadInformation(userThreadFunction, userData, threadName);
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void Threads::EndThread() {
    ThreadsDatabase::Lock();
    ThreadsDatabase::RemoveEntry(Threads::Id());
    ThreadsDatabase::UnLock();
    ExitThread(0);
    //_endthread();
}

Threads::ThreadStateType Threads::GetState(ThreadIdentifier threadId) {
    return STATE_UNKNOWN;
}

int32 Threads::GetCPUs(ThreadIdentifier threadId) {
    return -1;
}

ThreadIdentifier Threads::Id() {
    return GetCurrentThreadId();
}

void Threads::SetPriorityLevelAndClass(ThreadIdentifier threadId,
                                       PriorityClassType priorityClass,
                                       ThreadPriorityType priorityLevel) {

    //Cannot set an unknown priority
    if (priorityLevel == 0 && priorityClass == 0) {
        return;
    }

    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    if (threadInfo == NULL) {
        ThreadsDatabase::UnLock();
        return;
    }

    HANDLE threadHandle = OpenThread(THREAD_ALL_ACCESS, TRUE, threadId);

    //change the priority level
    if (priorityLevel > 0) {

        threadInfo->priorityLevel = priorityLevel;

        switch (priorityLevel) {
        case 1:
            SetThreadPriority(threadHandle, THREAD_PRIORITY_IDLE);
            break;
        case 2:
            SetThreadPriority(threadHandle, THREAD_PRIORITY_LOWEST);
            break;
        case 3:
            SetThreadPriority(threadHandle, THREAD_PRIORITY_BELOW_NORMAL);
            break;
        case 4:
            SetThreadPriority(threadHandle, THREAD_PRIORITY_NORMAL);
            break;
        case 5:
            SetThreadPriority(threadHandle, THREAD_PRIORITY_ABOVE_NORMAL);
            break;
        case 6:
            SetThreadPriority(threadHandle, THREAD_PRIORITY_HIGHEST);
            break;
        case 7:
            SetThreadPriority(threadHandle, THREAD_PRIORITY_TIME_CRITICAL);
            break;
        }

    }

    //Change the priority class: this is applied to the current process.
    if (priorityClass > 0) {

        threadInfo->priorityClass = priorityClass;

        switch (priorityClass) {
        case 1:
            ::SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
            break;
        case 2:
            ::SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
            break;
        case 3:
            ::SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
            break;
        case 4:
            ::SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
            break;
        }

    }

    ThreadsDatabase::UnLock();

}

Threads::ThreadPriorityType Threads::GetPriorityLevel(ThreadIdentifier threadId) {
    ThreadPriorityType priorityLevel = PRIORITY_UNKNOWN;

    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    if (threadInfo != NULL) {
        priorityLevel = threadInfo->priorityLevel;
    }
    ThreadsDatabase::UnLock();
    return priorityLevel;
}

Threads::PriorityClassType Threads::GetPriorityClass(ThreadIdentifier threadId) {

    PriorityClassType priorityClass = PRIORITY_CLASS_UNKNOWN;
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    if (threadInfo != NULL) {
        priorityClass = threadInfo->priorityClass;
    }
    ThreadsDatabase::UnLock();
    return priorityClass;

}

bool Threads::IsAlive(ThreadIdentifier threadId) {
    ThreadsDatabase::Lock();
    bool condition = (ThreadsDatabase::GetThreadInformation(threadId) != NULL);
    ThreadsDatabase::UnLock();
    if (condition) {
        //return GetThreadPriority((HANDLE)threadId) != THREAD_PRIORITY_ERROR_RETURN;
        HANDLE tid = OpenThread(PROCESS_ALL_ACCESS, TRUE, threadId);
        if (tid == NULL) {
            return false;
        }
        return WaitForSingleObject(tid, 0) != WAIT_OBJECT_0;
    }
    else {
        return false;
    }

}

bool Threads::Kill(ThreadIdentifier threadId) {
    //return GetThreadPriority((HANDLE)threadId) != THREAD_PRIORITY_ERROR_RETURN;
    HANDLE tid = OpenThread(PROCESS_ALL_ACCESS, TRUE, threadId);
    if (tid == NULL) {
        return false;
    }
    if (WaitForSingleObject(tid, 0) == WAIT_OBJECT_0) {
        return false;
    }

    ThreadsDatabase::Lock();
    bool condition = (ThreadsDatabase::GetThreadInformation(threadId) != NULL);
    ThreadsDatabase::UnLock();

    if (!condition) {
        return false;
    }

    ThreadsDatabase::Lock();
    ThreadsDatabase::RemoveEntry(threadId);
    ThreadsDatabase::UnLock();

    if (threadId == GetCurrentThreadId()) {
        ExitThread(0);
    }

    if (TerminateThread((HANDLE) tid, 0) == FALSE) {
        /*LPTSTR pTemp=NULL;
         DWORD retSize=FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
         FORMAT_MESSAGE_FROM_SYSTEM|
         FORMAT_MESSAGE_ARGUMENT_ARRAY,
         NULL,
         GetLastError(),
         LANG_NEUTRAL,
         (LPTSTR)&pTemp,
         0,
         NULL );
         printf("\n%s\n",pTemp);*/
        return false;
    }
    return true;
}

ThreadIdentifier Threads::BeginThread(ThreadFunctionType function,
                                      void *parameters,
                                      uint32 stacksize,
                                      const char8 *name,
                                      uint32 exceptionHandlerBehaviour,
                                      ProcessorType runOnCPUs) {

    if (runOnCPUs == ProcessorType::UndefinedCPUs) {
        if (ProcessorType::GetDefaultCPUs() != 0) {
            runOnCPUs = ProcessorType::GetDefaultCPUs();
        }
        else {
            runOnCPUs = 0xff;
        }
    }

    ThreadInformation *threadInfo = threadInitialisationInterfaceConstructor(function, parameters, name, exceptionHandlerBehaviour);
    if (threadInfo == NULL) {
        //CStaticAssertErrorCondition(InitialisationError,"Threads::ThreadsBeginThread (%s) threadInitialisationInterfaceConstructor returns NULL", name);
        return (ThreadIdentifier) 0;
    }

    DWORD threadId = 0;
    CreateThread(NULL, stacksize, (LPTHREAD_START_ROUTINE) SystemThreadFunction, threadInfo, 0, &threadId);

    //HANDLE threadId = (HANDLE)_beginthread((void (__cdecl *)(void *))SystemThreadFunction,stacksize,threadInfo);

    //Enable the user thread to run...
    threadInfo->ThreadPost();
    return (ThreadIdentifier) threadId;
}

const char8 *Threads::Name(ThreadIdentifier threadId) {
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    ThreadsDatabase::UnLock();

    if (threadInfo != NULL) {
        return threadInfo->ThreadName();
    }
    return NULL;

}

bool Threads::ProtectedExecute(ThreadFunctionType userFunction,
                               void *userData) {
    ThreadIdentifier threadId = Threads::Id();
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    ThreadsDatabase::UnLock();
    if (threadInfo != NULL) {
        return threadInfo->ExceptionProtectedExecute(userFunction, userData);
    }
    return false;
}

ThreadIdentifier Threads::FindByIndex(uint32 n) {
    return ThreadsDatabase::GetThreadID(n);
}

uint32 Threads::NumberOfThreads() {
    return ThreadsDatabase::NumberOfThreads();
}

bool Threads::GetThreadInfoCopy(ThreadInformation &copy,
                                int32 n,
                                ThreadIdentifier tid) {
    return ThreadsDatabase::GetInfo(copy, n, tid);
}

ThreadIdentifier Threads::FindByName(const char8 *name) {
    return ThreadsDatabase::Find(name);
}
