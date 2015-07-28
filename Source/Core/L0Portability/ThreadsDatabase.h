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


    friend class ThreadsOS;
    friend class Threads;

public:
    /**
     * @brief Adds a new thread to the database.
     * @param[in] threadInformation is a pointer to the thread information.
     * @return true if threadInformation is successfully added to the database, false otherwise.
     * @pre threadInformation != NULL
     */
    static bool NewEntry(ThreadInformation *threadInformation);
    /**
     * @brief Remove the entry from database searching by ThreadIdentifier.
     * @param[in] ThreadIdentifier is the id of the threads which must be removed from database.
     * @return true if the thread with ThreadIdentifier as id is in the database and if it is removed without errors.
     */
    static ThreadInformation *RemoveEntry(const ThreadIdentifier &threadId);

    /**
     * @brief Get thread informations.
     * @param[in] ThreadIdentifier is the id of the requested thread.
     * @return the ThreadInformation object related to the thread with ThreadIdentifier as id.
     */
    static ThreadInformation *GetThreadInformation(const ThreadIdentifier &threadId);

    /**
     * @brief Lock a spinlock mutex to allow exclusive access to the database.
     * @details With timeout it wait until it's expired, then return false.
     * @return false if the internal mutex lock fails.
     */
    static bool Lock();

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
     * @return the ThreadIdentifier of the requested thread.
     */
    static ThreadIdentifier GetThreadID(const uint32 &n);

    /**
     * @brief Get the informations searching a thread by index.
     * @param[out] threadInfoCopy contains the thread informations in return.
     * @param[in] n is the index of the requested thread.
     * @return true if the requested thread is in the database.
     */
    static bool GetInfoIndex(ThreadInformation &threadInfoCopy, const uint32 &n);

    /**
     * @brief Get the informations searching a thread by index or by id.
     * @param[out] threadInfoCopy contains the thread informations in return.
     * @param[in] n is the index of the requested thread.
     * @param[in] ThreadIdentifier is the ThreadIdentifier of the requested thread.
     * @return true if the requested thread is in the database.
     */
    static bool GetInfo(ThreadInformation &threadInfoCopy, const ThreadIdentifier &threadId);

    /**
     * @brief Returns the id searching by name.
     * @param[in] name is the name of the thread.
     * @return the id of the first thread found with the name specified or 0 if it is not in the database.
     * @pre name != NULL
     */
    static ThreadIdentifier Find(const char8 * const name);

private:


    /** Fast semaphore */
    static FastPollingMutexSem internalMutex;

    /** Actual number of entries used */
    static uint32              nOfEntries;

    /** Max number of entries currently possible */
    static uint32              maxNOfEntries;

    /** Vector of thread information pointers */
    static ThreadInformation **entries;

    /** Allocate more space in the database*/
    static bool AllocMore(void);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADSDATABASE_H_ */

