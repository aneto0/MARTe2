/**
 * @file Threads.cpp
 * @brief Source file for module Threads
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
 * the module Threads (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

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

namespace MARTe {

namespace Threads {

bool isRealtimeClass = false;

static void SystemThreadFunction(ThreadInformation * const threadInfo) {
    if (threadInfo != NULL) {
        //Guarantee that the OS finishes the housekeeping before releasing the thread to the user
        ErrorManagement::ErrorType err = threadInfo->ThreadWait();
        //Start the user thread
        if (err == ErrorManagement::NoError) {
            threadInfo->UserThreadFunction();

            bool ok = ThreadsDatabase::Lock();
            if (ok) {
                ThreadInformation *threadInfo2 = ThreadsDatabase::RemoveEntry(Id());
                if (threadInfo != threadInfo2) {
                    //CStaticAssertErrorCondition(ErrorManagement::FatalError,"SystemThreadFunction TDB_RemoveEntry returns wrong threadInfo \n");
                }
            }
            ThreadsDatabase::UnLock();
        }
        delete threadInfo;
    }
}

static ThreadInformation * threadInitialisationInterfaceConstructor(const ThreadFunctionType userThreadFunction,
                                                                    const void * const userData,
                                                                    const char8 * const threadName) {

    return new ThreadInformation(userThreadFunction, userData, threadName);
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Threads {

ThreadStateType GetState(const ThreadIdentifier &threadId) {
    return UnknownThreadStateType;
}

uint32 GetCPUs(const ThreadIdentifier &threadId) {
    return 0;
}

ThreadIdentifier Id() {
    return GetCurrentThreadId();
}

void SetPriority(const ThreadIdentifier &threadId,
                 const PriorityClassType &priorityClass,
                 const uint8 &priorityLevel) {

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

    //Change the priority class: this is applied to the current process.
    if ((priorityClass != UnknownPriorityClass) && !(isRealtimeClass)) {

        switch (priorityClass) {
        case IdlePriorityClass:
            if (::SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS) != 0) {
                threadInfo->SetPriorityClass(priorityClass);
            }
            break;
        case NormalPriorityClass:
            if (::SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS)) {
                threadInfo->SetPriorityClass(priorityClass);
            }
            break;
        case RealTimePriorityClass:
            if (::SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS)) {
                threadInfo->SetPriorityClass(priorityClass);
                isRealtimeClass = true;
            }
            break;
        }
    }

    //change the priority level
    if (priorityLevel > 0) {
        uint8 prioLevel = priorityLevel;

        if (prioLevel > 15) {
            prioLevel = 15;
        }

        for (uint32 i = 0; i < 1; i++) {

            //In case of Idle Priority class put always the idle priority level
            if ((prioLevel <= 1) || (priorityClass == IdlePriorityClass)) {
                if (SetThreadPriority(threadHandle, THREAD_PRIORITY_IDLE) != 0) {
                    threadInfo->SetPriorityLevel(prioLevel);
                }
                break;
            }
            if (prioLevel <= 3) {
                if (SetThreadPriority(threadHandle, THREAD_PRIORITY_LOWEST) != 0) {
                    threadInfo->SetPriorityLevel(prioLevel);
                }
                break;
            }
            if (prioLevel <= 5) {
                if (SetThreadPriority(threadHandle, THREAD_PRIORITY_BELOW_NORMAL) != 0) {
                    threadInfo->SetPriorityLevel(prioLevel);
                }
                break;
            }
            if (prioLevel <= 9) {
                if (SetThreadPriority(threadHandle, THREAD_PRIORITY_NORMAL) != 0) {
                    threadInfo->SetPriorityLevel(prioLevel);
                }
                break;
            }
            if (prioLevel <= 11) {
                if (SetThreadPriority(threadHandle, THREAD_PRIORITY_ABOVE_NORMAL) != 0) {
                    threadInfo->SetPriorityLevel(prioLevel);
                }
                break;
            }
            if (prioLevel <= 13) {
                if (SetThreadPriority(threadHandle, THREAD_PRIORITY_HIGHEST) != 0) {
                    threadInfo->SetPriorityLevel(prioLevel);
                }
                break;
            }
            if (prioLevel <= 15) {
                if (SetThreadPriority(threadHandle, THREAD_PRIORITY_TIME_CRITICAL) != 0) {
                    threadInfo->SetPriorityLevel(prioLevel);
                }
                break;
            }
        }

    }

    ThreadsDatabase::UnLock();

}

uint8 GetPriorityLevel(const ThreadIdentifier &threadId) {
    uint8 priorityLevel = 0u;

    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    if (threadInfo != NULL) {
        priorityLevel = threadInfo->GetPriorityLevel();
    }
    ThreadsDatabase::UnLock();
    return priorityLevel;
}

PriorityClassType GetPriorityClass(const ThreadIdentifier &threadId) {

    PriorityClassType priorityClass = UnknownPriorityClass;
    if (isRealtimeClass) {
        priorityClass = RealTimePriorityClass;
    }
    else {
        ThreadsDatabase::Lock();
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        if (threadInfo != NULL) {
            priorityClass = threadInfo->GetPriorityClass();
        }
        ThreadsDatabase::UnLock();
    }
    return priorityClass;

}

bool IsAlive(const ThreadIdentifier &threadId) {
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

bool Kill(const ThreadIdentifier &threadId) {
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
        return false;
    }
    return true;
}

ThreadIdentifier BeginThread(const ThreadFunctionType function,
                             const void * const parameters,
                             const uint32 &stacksize,
                             const char8 * const name,
                             const uint32 exceptionHandlerBehaviour,
                             ProcessorType runOnCPUs) {

    if (runOnCPUs == UndefinedCPUs) {
        if (ProcessorType::GetDefaultCPUs() != 0) {
            runOnCPUs = ProcessorType::GetDefaultCPUs();
        }
        else {
            runOnCPUs = ProcessorType(0xf);
        }
    }

    ThreadInformation *threadInfo = threadInitialisationInterfaceConstructor(function, parameters, name);
    if (threadInfo == NULL) {
        //CStaticAssertErrorCondition(InitialisationError,"ThreadsBeginThread (%s) threadInitialisationInterfaceConstructor returns NULL", name);
        return (ThreadIdentifier) 0;
    }

    DWORD threadId = 0;
    CreateThread(NULL, stacksize, (LPTHREAD_START_ROUTINE) SystemThreadFunction, threadInfo, 0, &threadId);
    bool ok = ThreadsDatabase::Lock();
    if (ok) {
        threadInfo->SetThreadIdentifier(threadId);

        ok = ThreadsDatabase::NewEntry(threadInfo);
        ThreadsDatabase::UnLock();
    }
    if (ok) {
        threadInfo->SetPriorityLevel(0u);
        SetPriority(threadId, NormalPriorityClass, 0u);
    }
    //HANDLE threadId = (HANDLE)_beginthread((void (__cdecl *)(void *))SystemThreadFunction,stacksize,threadInfo);

    //Enable the user thread to run...
    threadInfo->ThreadPost();
    return (ThreadIdentifier) threadId;
}

void EndThread() {
}

const char8 *Name(const ThreadIdentifier &threadId) {
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    ThreadsDatabase::UnLock();

    if (threadInfo != NULL) {
        return threadInfo->ThreadName();
    }
    return NULL;
}

ThreadIdentifier FindByIndex(const uint32 &n) {
    return ThreadsDatabase::GetThreadID(n);
}

uint32 NumberOfThreads() {
    return ThreadsDatabase::NumberOfThreads();
}

bool GetThreadInfoCopy(ThreadInformation &copy,
                       const uint32 &n) {
    return ThreadsDatabase::GetInfoIndex(copy, n);
}

bool GetThreadInfoCopy(ThreadInformation &copy,
                       const ThreadIdentifier &threadId) {
    return ThreadsDatabase::GetInfo(copy, threadId);
}

ThreadIdentifier FindByName(const char8 * const name) {
    return ThreadsDatabase::Find(name);
}

}

}
