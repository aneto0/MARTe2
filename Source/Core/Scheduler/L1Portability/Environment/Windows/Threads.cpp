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
#include "CompositeErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#include <Winternl.h>

typedef struct {
  HANDLE UniqueProcess;
  HANDLE UniqueThread;
} CLIENT_ID;

typedef LONG       KPRIORITY;

typedef struct _THREAD_BASIC_INFORMATION
{
    NTSTATUS ExitStatus;
    PTEB TebBaseAddress;
    CLIENT_ID ClientId;
    ULONG_PTR AffinityMask;
    KPRIORITY Priority;
    LONG BasePriority;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

typedef NTSTATUS ( WINAPI *NtQueryInformationThreadType )( HANDLE, LONG, PVOID, ULONG, PULONG );
typedef NTSTATUS ( WINAPI *NtSetInformationThreadType )( HANDLE, LONG, PVOID, ULONG);

static const ULONG ThreadBasicInformation = 0;
static const ULONG ThreadPriority = 2;
static const ULONG ThreadBasePriority = 3;

class NtInformationThreadProvider{
public:

	static inline NtQueryInformationThreadType NtQueryInformationThread(){
		return Access().NtQueryInformationThreadPtr;
	}
	static inline NtSetInformationThreadType NtSetInformationThread(){
		return Access().NtSetInformationThreadPtr;
	}
	static inline bool SetThreadPriorityEx(HANDLE threadHandle, MARTe::uint8 priority){
		if (priority > 32){
			priority = 32;
		}
		ULONG priorityEx = priority;
	    MARTe::ErrorManagement::ErrorType ret;
	    ret.OSError = (NtInformationThreadProvider::NtSetInformationThread()(
	    	threadHandle,
			ThreadBasePriority,
	        &priorityEx,
	        sizeof(priorityEx))!=0);

	    COMPOSITE_REPORT_ERROR(ret,"NtSetInformationThread(ThreadBasePriority,",priority,") returned ",ret);
	    return ret;
	}

	static inline bool GetThreadAffinityMask(HANDLE threadHandle, MARTe::uint64 &mask){
	    THREAD_BASIC_INFORMATION tbi;

	    ZeroMemory(&tbi, sizeof(tbi));

	    NTSTATUS ret = NtInformationThreadProvider::NtQueryInformationThread()(
	    	threadHandle,
			ThreadBasicInformation, // ThreadBasicInformation, undocumented
	        &tbi,
	        sizeof(tbi),
	        nullptr);
	    mask = static_cast<MARTe::uint64>(tbi.AffinityMask);
	    return (ret == 0);
	}

private:
	static inline NtInformationThreadProvider &Access(){
		static NtInformationThreadProvider nqit;
		return nqit;
	}

	NtInformationThreadProvider(){
		NtQueryInformationThreadPtr = reinterpret_cast<NtQueryInformationThreadType>(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationThread"));
		NtSetInformationThreadPtr   = reinterpret_cast<NtSetInformationThreadType>  (GetProcAddress(GetModuleHandle("ntdll.dll"), "NtSetInformationThread"));
	}
	NtQueryInformationThreadType NtQueryInformationThreadPtr;
	NtSetInformationThreadType   NtSetInformationThreadPtr;
};


namespace MARTe {

namespace Threads {

// removed and substituted with high priority class
//bool isRealtimeClass = false;

static uint32 SystemThreadFunction(ThreadInformation * const threadInfo) {
    if (threadInfo != NULL) {
        //Guarantee that the OS finishes the housekeeping before releasing the thread to the user
        ErrorManagement::ErrorType err = threadInfo->ThreadWait();
        //Start the user thread
        if (err == ErrorManagement::NoError) {
            threadInfo->UserThreadFunction();

            bool ok = ThreadsDatabase::Lock();
            if (ok) {
            	ThreadIdentifier tid = Id();
                ThreadInformation *threadInfo2 = ThreadsDatabase::RemoveEntry(tid);
                if (threadInfo != threadInfo2) {
                    //CStaticAssertErrorCondition(ErrorManagement::FatalError,"SystemThreadFunction TDB_RemoveEntry returns wrong threadInfo \n");
                }
            }
            ThreadsDatabase::UnLock();
        }
        delete threadInfo;
    }
    return 0;
}

static ThreadInformation * threadInitialisationInterfaceConstructor(const ThreadFunctionType userThreadFunction,
                                                                    const void * const userData,
                                                                    CCString threadName) {

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

uint64 GetCPUs(const ThreadIdentifier &threadId) {
    ErrorManagement::ErrorType ret;

    HANDLE threadHandle = NULL;
    if (ret){
        threadHandle = OpenThread(THREAD_ALL_ACCESS, TRUE, threadId);
        ret.internalSetupError = (threadHandle == NULL);
        REPORT_ERROR(ret,"OpenThread failed");
    }

	uint64 mask = 0;
	if (ret){
		ret.OSError = !NtInformationThreadProvider::GetThreadAffinityMask(threadHandle, mask);
		REPORT_ERROR(ret,"GetThreadAffinityMask failed");
	}

    if (threadHandle != NULL){
    	CloseHandle(threadHandle);
    }

    return mask;
}

ThreadIdentifier Id() {
    return GetCurrentThreadId();
}

static const int priorityTranslationTable[16]={
		THREAD_PRIORITY_IDLE,THREAD_PRIORITY_IDLE,
		THREAD_PRIORITY_LOWEST,THREAD_PRIORITY_LOWEST,
		THREAD_PRIORITY_BELOW_NORMAL,THREAD_PRIORITY_BELOW_NORMAL,
		THREAD_PRIORITY_NORMAL,THREAD_PRIORITY_NORMAL,THREAD_PRIORITY_NORMAL,THREAD_PRIORITY_NORMAL,
		THREAD_PRIORITY_ABOVE_NORMAL,THREAD_PRIORITY_ABOVE_NORMAL,
		THREAD_PRIORITY_HIGHEST,THREAD_PRIORITY_HIGHEST,
		THREAD_PRIORITY_TIME_CRITICAL,THREAD_PRIORITY_TIME_CRITICAL
};
/*
static const int priorityTranslationTableRT[16]={
		THREAD_PRIORITY_IDLE,
		-7,
		-6,
		-5,
		-4,
		-3,
		THREAD_PRIORITY_LOWEST,
		THREAD_PRIORITY_BELOW_NORMAL,
		THREAD_PRIORITY_NORMAL,
		THREAD_PRIORITY_ABOVE_NORMAL,
		THREAD_PRIORITY_HIGHEST,
		3,
		4,
		5,
		6,
		THREAD_PRIORITY_TIME_CRITICAL
};
*/

void SetPriority(const ThreadIdentifier &threadId,
                 const PriorityClassType &priorityClass,
                 const uint8 &priorityLevel) {

    ErrorManagement::ErrorType ret;

    if (ret){
        ret.parametersError = (priorityLevel > 15);
        REPORT_ERROR(ret,"cannot set PriorityLevel > 15");
    }

    bool locked = false;
    if (ret){
    	locked = ThreadsDatabase::Lock();
        ret.internalStateError = !locked;
        REPORT_ERROR(ret,"cannot lock ThreadsDatabase");
    }

    ThreadInformation *threadInfo = NULL_PTR(ThreadInformation *);
    if (ret){
        threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        ret.internalSetupError = (threadInfo == NULL_PTR(ThreadInformation *));
        REPORT_ERROR(ret,"cannot access ThreadsDatabase");
    }

    PriorityClassType priorityClassCopy = priorityClass;
    if (ret){
    	if (priorityClassCopy == UnknownPriorityClass){
    		priorityClassCopy = threadInfo->GetPriorityClass();
    	}
    	ret.internalSetupError = ((priorityClassCopy > RealTimePriorityClass) || (priorityClassCopy < IdlePriorityClass));
        COMPOSITE_REPORT_ERROR(ret,"priorityClass out ot range = ",priorityClassCopy);
    }

    HANDLE threadHandle = NULL;
    if (ret){
        threadHandle = OpenThread(THREAD_ALL_ACCESS, TRUE, threadId);
        ret.internalSetupError = (threadHandle == NULL);
        REPORT_ERROR(ret,"OpenThread failed");
    }

    int priorityDelta = 0;
    if (ret){
    	if (priorityClassCopy == RealTimePriorityClass){
    		ret.OSError = (SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS)== FALSE);
//    		priorityDelta = priorityTranslationTableRT[priorityLevel];
       	} else
    	if (priorityClassCopy == IdlePriorityClass){
    		ret.OSError = (SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS)== FALSE);
//    		priorityDelta = priorityTranslationTable[priorityLevel];
       	} else {
    		ret.OSError = (SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS)== FALSE);
//    		priorityDelta = priorityTranslationTable[priorityLevel];
       	}

		REPORT_ERROR(ret,"SetPriorityClass failed");
    }

    if (ret){
		priorityDelta = priorityTranslationTable[priorityLevel];
    	ret.OSError = (SetThreadPriority(threadHandle, priorityDelta) == FALSE);
		COMPOSITE_REPORT_ERROR(ret,"SetThreadPriority(",priorityDelta,") failed");
    }

    if (ret){
        threadInfo->SetPriorityClass(priorityClassCopy);
        threadInfo->SetPriorityLevel(priorityLevel);
    }

    if (threadHandle != NULL){
    	CloseHandle(threadHandle);
    }

    if (locked){
        ThreadsDatabase::UnLock();
    }

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
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    if (threadInfo != NULL) {
        priorityClass = threadInfo->GetPriorityClass();
    }
    ThreadsDatabase::UnLock();
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
                             CCString	 name,
                             const uint32 exceptionHandlerBehaviour,
                             ProcessorType runOnCPUs) {

    if (runOnCPUs == UndefinedCPUs) {
        if (ProcessorType::GetDefaultCPUs() != 0) {
            runOnCPUs = ProcessorType::GetDefaultCPUs();
        }
        else {
            runOnCPUs = 0xff;
        }
    }

    ErrorManagement::ErrorType ret;

    ret.parametersError = (stacksize == 0);
	REPORT_ERROR(ret,"stacksize = 0");

	ThreadInformation *threadInfo = NULL;
	if (ret){
	    threadInfo = threadInitialisationInterfaceConstructor(function, parameters, name);
		ret.fatalError = (threadInfo == NULL) ;
		REPORT_ERROR(ret,"threadInitialisationInterfaceConstructor failed");
	}

    DWORD threadId = 0;
    HANDLE hThread = NULL;
	if (ret){
	    hThread = CreateThread(NULL, stacksize, reinterpret_cast<LPTHREAD_START_ROUTINE>(SystemThreadFunction) , threadInfo, 0, &threadId);
	    ret.OSError = (hThread == NULL);
		REPORT_ERROR(ret,"CreateThread failed");
	}

	/*
	if (ret){
		ret.OSError = (SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS) == FALSE);
		REPORT_ERROR(ret,"SetPriorityClass failed");
	}*/

	if (ret){
		ret.OSError = (SetThreadAffinityMask(hThread,runOnCPUs.GetProcessorMask()) == FALSE);
		COMPOSITE_REPORT_ERROR(ret,"SetThreadAffinityMask (",runOnCPUs.GetProcessorMask(),")failed");
	}

    /**
     * stores the threadInfo object pointer in the database
     */
    bool locked = false;
    if (ret){
        locked = ThreadsDatabase::Lock();
        ret.internalSetupError = (locked==false);
		REPORT_ERROR(ret,"database lock failed");
    }

    if (ret) {
	    threadInfo->SetThreadIdentifier(threadId);
        ret.internalSetupError = !ThreadsDatabase::NewEntry(threadInfo);
		REPORT_ERROR(ret,"newEntry failed");
    }

    if (locked){
    	ThreadsDatabase::UnLock();
    }

    if (ret) {
        threadInfo->SetPriorityLevel(0u);
        SetPriority(threadId, NormalPriorityClass, 0u);

        //Enable the user thread to run...
        threadInfo->ThreadPost();
    }


    return (ThreadIdentifier) threadId;
}

void EndThread() {
}

CCString Name(const ThreadIdentifier &threadId) {
	CCString name = emptyString;
    ThreadsDatabase::Lock();
    ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
    ThreadsDatabase::UnLock();

    if (threadInfo != NULL) {
        name =  threadInfo->ThreadName();
    }
    return name;
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

ThreadIdentifier FindByName(CCString name) {
    return ThreadsDatabase::Find(name);
}

}

}
