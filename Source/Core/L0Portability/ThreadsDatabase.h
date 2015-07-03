/**
 * @file ThreadsDatabase.h
 * @brief Header file for class ThreadsDatabase
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

 * @details This header file contains the declaration of the class ThreadsDatabase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef THREADSDATABASE_H_
#define THREADSDATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ThreadInformation.h"
#include "TimeoutType.h"
#include "FastPollingMutexSem.h"

extern "C" {

/**
 * @brief Called by ThreadsDatabase::DatabaseNewEntry.
 * @param[in] threadInfo contains the thread informations.
 * @return true if the informations are correctly added to the database.
 */
bool ThreadsDatabaseNewEntry(ThreadInformation *threadInfo);

/**
 *  @brief Called by ThreadsDatabase::RemoveEntry
 *  @param[in] threadId is the id of the thread to remove from database.
 *  @return a pointer to the ThreadInformation structure of the thread removed.
 */
ThreadInformation *ThreadsDatabaseRemoveEntry(TID threadId);

/**
 * @brief Called by ThreadsDatabase::GetThreadInformation.
 * @param[in] threadId is the thread identifier.
 * @return a pointer to the thread informations structure.
 */
ThreadInformation *ThreadsDatabaseGetThreadInformation(TID threadId);

/**
 * @brief Called by ThreadsDatabase::Lock.
 * @param[in] tt is the desired timeout.
 * @return true if the lock success, false otherwise.
 */
bool ThreadsDatabaseLock(TimeoutType tt);

/**
 * @brief Called by ThreadsDatabase::UnLock.
 * @return true if the unlock success, false otherwise.
 */
void ThreadsDatabaseUnLock();

/**
 * @brief Called by ThreadsDatabase::NumberOfThreads.
 * @return the number of elements in the database.
 */
uint32 ThreadsDatabaseNumberOfThreads();

/**
 * @brief Called by ThreadsDatabase::GetThreadId.
 * @param[in] n is the index of the element in the database.
 * @return the identifier of the n-th thread in the database.
 */
TID ThreadsDatabaseGetThreadID(uint32 n);

/**
 * @brief Called by ThreadsDatabase::GetInfo.
 * @param[out] threadInfoCopy is a copy of the thread information structure in return.
 * @param[in] n is the index of the element in the database.
 * @param[in] threadId is the identifier of the thread.
 * @return true if the requested thread is in the database.
 */
bool ThreadsDatabaseGetInfo(ThreadInformation &threadInfoCopy,
                            int32 n,
                            TID threadId);

/**
 * @brief Called by ThreadsDatabase::Find.
 * @param[in] name is the name of the thread.
 * @return the identifier of the thread with the name specified, 0 if it is not in the database.
 */
TID ThreadsDatabaseFind(const char8 *name);

}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A database of pointers to threadInformation objects.
 *
 * @details These methods allows to store and remove pointers to threadInformation objects. A new entry is automatically
 * add to the database when a thread begin its execution and it's removed from database automatically when the
 * thread terminate. An internal mutex could be implemented here to make consistent all operations on database between
 * threads. The database allows to threads the access to other threads informations.
 *
 * @details Most of the implementation is delegated to ThreadsDatabaseOS because there are OS which implements most
 * of these functions natively. It is possible define the memory granularity (the size allocated in each allocation function)
 * changing the value of THREADS_DATABASE_GRANULARITY in GeneralDefinitionsOS.h for each operating system.
 */
class ThreadsDatabase {

public:
    friend bool ThreadsDatabaseNewEntry(ThreadInformation *threadInfo);
    friend ThreadInformation *ThreadsDatabaseRemoveEntry(TID threadId);
    friend ThreadInformation *ThreadsDatabaseGetThreadInformation(TID threadId);
    friend bool ThreadsDatabaseLock(TimeoutType tt);
    friend void ThreadsDatabaseUnLock();
    friend uint32 ThreadsDatabaseNumberOfThreads();
    friend TID ThreadsDatabaseGetThreadID(uint32 n);
    friend TID ThreadsDatabaseFind(const char8 *name);
    friend bool ThreadsDatabaseGetInfo(ThreadInformation &threadInfoCopy,
                                       int32 n,
                                       TID threadId);

    friend class ThreadsOS;
private:
    /**
     * @brief Add a new thread to the database.
     * @details The database memory could be allocated dinamically using malloc and realloc functions.
     * @param[in] ti is a pointer to the thread information.
     * @return true if ti is added to the database, false in case of failure.
     */
    static bool NewEntry(ThreadInformation *ti);
    /**
     * @brief Remove the entry from database searching by tid.
     * @param[in] tid is the id of the threads which must be removed from database.
     * @return true if the thread with tid as id is in the database and if it is removed without errors.
     */
    static ThreadInformation *RemoveEntry(TID tid);

    /**
     * @brief Get thread informations.
     * @param[in] tid is the id of the requested thread.
     * @return the ThreadInformation object related to the thread with tid as id.
     */
    static ThreadInformation *GetThreadInformation(TID tid);

    /**
     * @brief Lock a spinlock mutex to allow exclusive access to the database.
     * @details With timeout it wait until it's expired, then return false.
     * @param[in] tt is the timeout.
     * @return false if the lock fails because the timeout.
     */
    static bool Lock(TimeoutType tt = TTInfiniteWait);

    /**
     * @brief Unlock the internal mutex.
     * @return true.
     */
    static void UnLock();

    /**
     * @brief Return the number of threads registered.
     * @return the number of threads currently saved in database.
     */
    static uint32 NumberOfThreads();

    /**
     * @brief Get the thread id of the n-th thread saved in database.
     * @param[in] n is the index of the requested thread.
     * @return the tid of the requested thread.
     */
    static TID GetThreadID(uint32 n);

    /**
     * @brief Get the informations searching a thread by index or by id.
     * @param[out] tiCopy contains the thread informations in return.
     * @param[in] n is the index of the requested thread.
     * @param[in] tid is the tid of the requested thread.
     * @return true if the requested thread is in the database.
     */
    static bool GetInfo(ThreadInformation &tiCopy,
                        int32 n = -1,
                        TID tid = (TID) - 1);

    /**
     * @brief Returns the id searching by name.
     * @param[in] name is the name of the thread.
     * @return the id of the first thread found with the name specified or 0 if it is not in the database.
     */
    static TID Find(const char8 *name);

private:


    /** Fast semaphore */
    static FastPollingMutexSem internalMutex;

    /** Actual number of entries used */
    static uint32 nOfEntries;

    /** Max number of entries currently possible */
    static uint32 maxNOfEntries;

    /** Vector of thread information pointers */
    static ThreadInformation **entries;

    /** Allocate more space in the database*/
    static bool AllocMore();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADSDATABASE_H_ */

