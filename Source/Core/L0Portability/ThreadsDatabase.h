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
 * @brief A database of ThreadInformation objects.
 */
class ThreadsDatabase {


    friend class ThreadsOS;
    friend class Threads;

public:
    /**
     * @brief Adds a new thread to the database.
     * @param[in] threadInformation a pointer to the thread information that is to be added.
     * @return true if threadInformation is successfully added to the database, false otherwise.
     * @pre threadInformation != NULL
     */
    static bool NewEntry(ThreadInformation *threadInformation);

    /**
     * @brief Removes the entry from database searching by a given ThreadIdentifier.
     * @param[in] threadId the identifier of the thread which is to be removed from database.
     * @return true if the thread identifier exists in the database and if it is removed without errors.
     */
    static ThreadInformation *RemoveEntry(const ThreadIdentifier &threadId);

    /**
     * @brief Returns the ThreadInformation associated to a given identifier.
     * @param[in] threadId the identifier of the requested thread.
     * @return the ThreadInformation object related to the to the requested identifier or NULL if the
     * identifier does not exist in the database.
     */
    static ThreadInformation *GetThreadInformation(const ThreadIdentifier &threadId);

    /**
     * @brief Locks a spinlock mutex to allow exclusive access to the database.
     * @return false if the mutex lock fails.
     */
    static bool Lock();

    /**
     * @brief Unlocks the internal mutex.
     */
    static void UnLock();

    /**
     * @brief Returns the number of threads registered.
     * @return the number of threads currently saved in database.
     */
    static uint32 NumberOfThreads();

    /**
     * @brief Returns the thread id of the n-th thread saved in database.
     * @param[in] n is index of the requested thread.
     * @return the ThreadIdentifier of the requested thread.
     */
    static ThreadIdentifier GetThreadID(const uint32 &n);

    /**
     * @brief Searches for a ThreadInformation object by index and copies into a destination.
     * @param[out] threadInfoCopy the destination TheadInformation object.
     * @param[in] n is the index of the requested thread.
     * @return true if the requested thread exists in the database.
     */
    static bool GetInfoIndex(ThreadInformation &threadInfoCopy, const uint32 &n);

    /**
     * @brief Searches for a ThreadInformation object by its thread identifier and copies into a destination.
     * @param[out] threadInfoCopy the destination TheadInformation object.
     * @param[in] threadId the ThreadIdentifier of the requested thread.
     * @return true if the requested thread exists in the database.
     */
    static bool GetInfo(ThreadInformation &threadInfoCopy, const ThreadIdentifier &threadId);

    /**
     * @brief Searches for a thread identifier by name.
     * @param[in] name the name of the thread to be searched.
     * @return the id of the first thread found with the specified name or 0 if it does not exist in the database.
     * @pre name != NULL
     */
    static ThreadIdentifier Find(const char8 * const name);

private:


    /**
     * Fast semaphore for exclusive access to the database.
     */
    static FastPollingMutexSem internalMutex;

    /**
     * Actual number of entries stored in the database.
     */
    static uint32              nOfEntries;

    /**
     * Maximum number of entries that can be stored in the database.
     */
    static uint32              maxNOfEntries;

    /**
     * Vector of thread information pointers.
     */
    static ThreadInformation **entries;

    /**
     * @brief Allocate more space in the database.
     * @return true if more space can be successfully allocated.
     */
    static bool AllocMore(void);

};

#endif /* THREADSDATABASE_H_ */
