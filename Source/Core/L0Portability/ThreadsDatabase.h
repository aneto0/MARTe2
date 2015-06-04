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

/**
 * @file
 * @brief Threads database, which can be queried to know the number of threads running in the application
 */
#if !defined (THREADS_DATABASE_H)
#define THREADS_DATABASE_H

#include "ThreadInformation.h"
#include "TimeoutType.h"

extern "C" {

/** @see ThreadsDatabase::DatabaseNewEntry */
bool ThreadsDatabaseNewEntry(ThreadInformation *ti);

/** @see ThreadsDatabase::RemoveEntry */
ThreadInformation *ThreadsDatabaseRemoveEntry(TID tid);

/** @see ThreadsDatabase::GetThreadInformation */
ThreadInformation *ThreadsDatabaseGetThreadInformation(TID tid);

/** @see ThreadsDatabase::Lock */
bool ThreadsDatabaseLock(TimeoutType tt);

/** @see ThreadsDatabase::UnLock*/
bool ThreadsDatabaseUnLock();

/** @see ThreadsDatabase::NumberOfThreads*/
int32 ThreadsDatabaseNumberOfThreads();

/** @see ThreadsDatabase::GetThreadId*/
TID ThreadsDatabaseGetThreadID(int32 n);

/** @see ThreadsDatabase::GetInfo*/
bool ThreadsDatabaseGetInfo(ThreadInformation &tiCopy, int32 n, TID tid);

}


/** @brief A database of pointers to threadInformation objects.
  * 
  * These methods allows to store and remove pointers to threadInformation objects. A new entry is automatically
  * add to the database when a thread begin its execution and it's removed from database automatically when the
  * thread terminate. A spinlock mutex it's implemented here to make consistent all operations on database between
  * threads. The database allow to threads the access to other threads informations.
  *
  * The database is fundamental for all operations between threads. Without it, threads could not know nothing about 
  * other threads.
  */ 


class ThreadsDatabase {
private:
    /** Number of ThreadInformation objects that are created each time 
     a request for more space in the entries array is needed*/
    static const int32 GRANULARITY = 64;

    /* Fast ram semafore using Atomic TestAndSet*/
    static int32 atomicSem;

    /* actual number of entries used */
    static int32 nOfEntries;

    /* Max number of entries currently possible */
    static int maxNOfEntries;

    /* vector of ti pointers */
    static ThreadInformation **entries;

    /** Allocate more space in the database*/
    static bool AllocMore();
public:
    friend bool ThreadsDatabaseNewEntry(ThreadInformation *ti);
    friend ThreadInformation *ThreadsDatabaseRemoveEntry(TID tid);
    friend ThreadInformation *ThreadsDatabaseGetThreadInformation(TID tid);
    friend bool ThreadsDatabaseLock(TimeoutType tt);
    friend bool ThreadsDatabaseUnLock();
    friend int32 ThreadsDatabaseNumberOfThreads();
    friend TID ThreadsDatabaseGetThreadID(int32 n);
    friend TID ThreadsDatabaseFind(const char *name);
    friend bool ThreadsDatabaseGetInfo(ThreadInformation &tiCopy, int32 n,
                                       TID tid);

public:
    /** @brief Add a new thread to the database.
      * The database memory is allocated dinamically truth malloc and realloc functions.
      * @param ti is a pointer to the thread information.
      * @return true if ti is added to the database, false if something wrong happened with memory allocation. */
    static bool NewEntry(ThreadInformation *ti) {
        return ThreadsDatabaseNewEntry(ti);
    }

    /** @brief Remove the entry from database searching by tid.
      * @param tid is the id of the threads which must be removed from database.
      * @return true if the thread with tid as id is in the database and if it is removed without errors. */
    static ThreadInformation *RemoveEntry(TID tid) {
        return ThreadsDatabaseRemoveEntry(tid);
    }

    /** @brief Get thread informations.
      * @param tid is the id of the requested thread.
      * @return the ThreadInformation object related to the thread with tid as id. */
    static ThreadInformation *GetThreadInformation(TID tid) {
        return ThreadsDatabaseGetThreadInformation(tid);
    }

    /** @brief Lock a spinlock mutex to allow exclusive access to the database.
        With timeout it wait until it's expired, then return false.
      * @param tt is the timeout. 
      * @return false if the lock fails because the timeout. */
    static bool Lock(TimeoutType tt = TTInfiniteWait) {
        return ThreadsDatabaseLock(tt);
    }

    /** @brief Unlock a spinlock mutex.
      * @return true. */
    static bool UnLock() {
        return ThreadsDatabaseUnLock();
    }

    /** @brief Return the number of threads registered.
      * @return the number of threads currently saved in database. */
    static int32 NumberOfThreads() {
        return ThreadsDatabaseNumberOfThreads();
    }

    /** @brief Get the thread id of the n-th thread saved in database.
      * @param n is the index of the requested thread.
      * @return the tid of the requested thread. */
    static TID GetThreadID(int32 n) {
        return ThreadsDatabaseGetThreadID(n);
    }

    /** @brief Get the informations searching a thread by index or by id.
      * @param n is the index of the requested thread.
      * @param tid is the tid of the requested thread.
      * @return true if the requested thread is in the database. */
    static bool GetInfo(ThreadInformation &tiCopy, int32 n = -1,
                        TID tid = (TID) - 1) {
        return ThreadsDatabaseGetInfo(tiCopy, n, tid);
    }
};

#endif

