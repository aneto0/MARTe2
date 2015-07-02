/**
 * @file ThreadsOS.h
 * @brief Header file for class ThreadsOS
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

 * @details This header file contains the declaration of the class ThreadsOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef THREADSOS_H_
#define 		THREADSOS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <process.h>
#include <signal.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#define __thread_decl
/**
 * @brief Callback thread function
 */
typedef void *(*StandardThreadFunction)(void *args);
/**
 * @brief The function called by each thread.
 * @details Adds the thread in the database, call the thread function and at last remove the the thread from the database.
 * @param[in] threadData is the thread information structure.
 */
void __thread_decl SystemThreadFunction(void *threadData) {
    ThreadInformation *threadInfo = (ThreadInformation *) threadData;
    if (threadInfo == NULL) {
        return;
    }

    ThreadsDatabase::Lock();
    threadInfo->threadId = Threads::Id();
    ThreadsDatabase::NewEntry(threadInfo);
    ThreadsDatabase::UnLock();

    threadInfo->priorityLevel = PRIORITY_NORMAL;
    Threads::SetPriorityClass(Threads::Id(), PRIORITY_CLASS_NORMAL);
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

/**
 * @brief Builds a ThreadInformation structure with the thread informations.
 * @param[in] userThreadFunction is the thread function.
 * @param[in] userData is the thread function argument.
 * @param[in] threadName is the desired name of the thread.
 * @param[in] exceptionHandlerBehaviour is not used here.
 * @return the ThreadInformation structure.
 */
ThreadInformation * threadInitialisationInterfaceConstructor(ThreadFunctionType userThreadFunction,
                                                             void *userData,
                                                             const char *threadName,
                                                             uint32 exceptionHandlerBehaviour) {

    return new ThreadInformation(userThreadFunction, userData, threadName);
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Platform dependent implementations of threads.
 * @details These functions calls system APIs for threads. It is possible use the thread database
 * which provides a smart way to obtain informations of all alive threads.
 */
class ThreadsOS {

public:

    /** @brief Called by Threads::EndThread */
    static void EndThread() {
        ThreadsDatabase::Lock();
        ThreadsDatabase::RemoveEntry(Threads::Id());
        ThreadsDatabase::UnLock();
        ExitThread(0);
        //_endthread();
    }

    /**
     * @brief Not implemented in Windows
     * @param[in] threadId is the thread identifier.
     * @return the thread state.
     */
    static ThreadStateType GetState(TID threadId) {
        return STATE_UNKNOWN;
    }

    /**
     * @brief Called by Threads::GetCPUs
     * @param[in] threadId is the thread identifier.
     * @return the cpu where the thread is executing.
     */
    static int32 GetCPUs(TID threadId) {
        return -1;
    }

    /**
     * @brief Called by Threads::Id.
     * @return the thread identifier.
     */
    static TID Id() {
        return (TID) GetCurrentThreadId();
    }

    /**
     * @brief Called by Threads::SetPriorityLevel.
     * @param[in] threadId is the thread identifier.
     * @param[in] priorityClass is the desired class priority to assign to the thread.
     * @param[in] priorityLevel is the desired thread priority to assign to the thread.
     */
    static void SetPriorityLevel(TID threadId,
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
                SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
                break;
            case 2:
                SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
                break;
            case 3:
                SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
                break;
            case 4:
                SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
                break;
            }

        }

        ThreadsDatabase::UnLock();

    }

    /**
     * @brief Called by Threads::GetPriorityLevel.
     * @details Returns the priority of a thread.
     * @param[in] threadId is the thread identifier.
     * @return the priority of the specified thread.
     */
    static ThreadPriorityType GetPriorityLevel(TID threadId) {
        ThreadPriorityType priorityLevel = PRIORITY_UNKNOWN;

        ThreadsDatabase::Lock();
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        if (threadInfo != NULL) {
            priorityLevel = threadInfo->priorityLevel;
        }
        ThreadsDatabase::UnLock();
        return priorityLevel;
    }

    /**
     * @brief Called by Threads::GetPriorityClass.
     * @details Returns the priority class of a thread.
     * @param[in] threadId is the thread identifier.
     * @return the priority class of the specified thread.
     */
    static PriorityClassType GetPriorityClass(TID threadId) {

        PriorityClassType priorityClass = PRIORITY_CLASS_UNKNOWN;
        ThreadsDatabase::Lock();
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        if (threadInfo != NULL) {
            priorityClass = threadInfo->priorityClass;
        }
        ThreadsDatabase::UnLock();
        return priorityClass;

    }

    /**
     * @brief Called by Threads::IsAlive
     *
     * @details A signal is used to know if the other thread is alive.
     *
     * @param[in] threadId is the thread identifier.
     */
    static bool IsAlive(TID threadId) {
        ThreadsDatabase::Lock();
        bool condition = (ThreadsDatabase::GetThreadInformation(threadId) != NULL);
        ThreadsDatabase::UnLock();
        if (condition) {
            //return GetThreadPriority((HANDLE)threadId) != THREAD_PRIORITY_ERROR_RETURN;
            HANDLE tid = OpenThread(PROCESS_ALL_ACCESS, TRUE, threadId);
            if (tid == NULL) {
                return False;
            }
            return WaitForSingleObject(tid, 0) != WAIT_OBJECT_0;
        }
        else {
            return False;
        }

    }

    /**
     * @brief Called by Threads::Kill
     * @param[in] threadId is the thread identifier.
     * @details A thread cannot be deleted if it locks a mutex semaphore.
     */
    static bool Kill(TID threadId) {

        //return GetThreadPriority((HANDLE)threadId) != THREAD_PRIORITY_ERROR_RETURN;
        HANDLE tid = OpenThread(PROCESS_ALL_ACCESS, TRUE, threadId);
        if (tid == NULL) {
            return False;
        }
        if (WaitForSingleObject(tid, 0) == WAIT_OBJECT_0) {
            return False;
        }

        ThreadsDatabase::Lock();
        bool condition = (ThreadsDatabase::GetThreadInformation(threadId) != NULL);
        ThreadsDatabase::UnLock();

        if (!condition) {
            return False;
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
            return False;
        }
        return True;
    }

    /**
     * @brief Called by Threads::BeginThread.
     * @param[in] function is the thread function.
     * @param[in] parameters is the thread function parameters structure.
     * @param[in] name is the desired name for the thread.
     * @param[in] stacksize is the desired stack size to assign to the thread.
     * @param[in] exceptionHandlerBehaviour is not used here.
     * @param[in] runOnCPUs is used to set an affinity with a desired cpu.
     */
    static TID BeginThread(ThreadFunctionType function,
                           void *parameters,
                           uint32 stacksize,
                           const char* name,
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

        ThreadInformation *threadInfo = threadInitialisationInterfaceConstructor(function, parameters, name, exceptionHandlerBehaviour);
        if (threadInfo == NULL) {
            //CStaticAssertErrorCondition(InitialisationError,"Threads::ThreadsBeginThread (%s) threadInitialisationInterfaceConstructor returns NULL", name);
            return (TID) 0;
        }

        DWORD threadId = 0;
        CreateThread(NULL, stacksize, (LPTHREAD_START_ROUTINE) SystemThreadFunction, threadInfo, 0, &threadId);

        //HANDLE threadId = (HANDLE)_beginthread((void (__cdecl *)(void *))SystemThreadFunction,stacksize,threadInfo);

        //Enable the user thread to run...
        threadInfo->ThreadPost();
        return (TID) threadId;
    }

    /**
     * @brief Called by Threads::Name
     * @param[in] threadId is the thread identifier.
     * @return the name of the specified thread.
     */
    static const char* Name(TID threadId) {
        ThreadsDatabase::Lock();
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        ThreadsDatabase::UnLock();

        if (threadInfo != NULL) {
            return threadInfo->ThreadName();
        }
        return NULL;
    }

    /**
     * @brief Executes the function specified.
     * @param[in] userFunction is the function to be called.
     * @param[in] userData is the function argument.
     * @param[in] ehi is the exception handler.
     * @return true.
     * @see ThreadInformation::ExceptionProtectedExecute.
     */
    static bool ProtectedExecute(ThreadFunctionType userFunction,
                                 void *userData,
                                 ExceptionHandler *ehi) {
        TID threadId = Threads::Id();
        ThreadsDatabase::Lock();
        ThreadInformation *threadInfo = ThreadsDatabase::GetThreadInformation(threadId);
        ThreadsDatabase::UnLock();
        if (threadInfo != NULL) {
            return threadInfo->ExceptionProtectedExecute(userFunction, userData, ehi);
        }
        return False;
    }

    /**
     * @brief Returns the id of the n-th thread in the database.
     * @param[in] n is the thread index.
     * @return the id of the n-th thread in the database, -1 if the database is empty.
     */
    static TID FindByIndex(uint32 n) {
        return ThreadsDatabase::GetThreadID(n);
    }

    /**
     * @brief Returns the number of threads currently in the database.
     * @return the number of threads currently in the database.
     */
    static uint32 NumberOfThreads() {
        return ThreadsDatabase::NumberOfThreads();
    }

    /**
     * @brief Get a copy of the thread information stored in the database.
     * @param[out] copy is the thread information structure in return.
     * @param[in] n is the threads index, if it is <0 is ignored.
     * @param[in] tid is the thread identifier.
     * @return true if the requested element is in the database, false otherwise.
     */
    static bool GetThreadInfoCopy(ThreadInformation &copy,
                                  int32 n,
                                  TID tid) {
        return ThreadsDatabase::GetInfo(copy, n, tid);
    }

    /**
     * @brief Search the thread with the specified name.
     * @param[in] name is the thread name.
     * @return the id of the first found thread with the specified name.
     */
    static TID FindByName(const char* name) {
        return ThreadsDatabase::Find(name);
    }

private:

    /**
     * @brief The function called by each thread.
     * @details Adds the thread in the database, call the thread function and at last remove the the thread from the database.
     * @param[in] threadData is the thread information structure. */
    static void __thread_decl SystemThreadFunction(void *threadData) {
        ThreadInformation *threadInfo = (ThreadInformation *) threadData;
        if (threadInfo == NULL) {
            return;
        }

        ThreadsDatabase::Lock();
        threadInfo->threadId = Threads::Id();
        ThreadsDatabase::NewEntry(threadInfo);
        ThreadsDatabase::UnLock();

        threadInfo->priorityLevel = PRIORITY_NORMAL;
        Threads::SetPriorityClass(Threads::Id(), PRIORITY_CLASS_NORMAL);
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

    /**
     * @brief Builds a ThreadInformation structure with the thread informations.
     * @param[in] userThreadFunction is the thread function.
     * @param[in] userData is the thread function argument.
     * @param[in] threadName is the desired name of the thread.
     * @param[in] exceptionHandlerBehaviour is not used here.
     * @return the ThreadInformation structure.
     */
    static ThreadInformation * threadInitialisationInterfaceConstructor(ThreadFunctionType userThreadFunction,
                                                                        void *userData,
                                                                        const char *threadName,
                                                                        uint32 exceptionHandlerBehaviour) {

        return new ThreadInformation(userThreadFunction, userData, threadName);
    }

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADSOS_H_ */

